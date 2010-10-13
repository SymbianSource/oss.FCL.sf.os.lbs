// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// incoming SUPL RESPONSE 
// 
//

/**
 @file suplrrlpasn1step.cpp
 @internalTechnology
*/
#include "suplrrlpasn1step.h"
#include "te_suplrrlpasnsuitedefs.h"

// LBS
#include <lbs/lbsnetcommon.h>
#include <lbs/lbsassistancedatabase.h>
#include <lbspositioninfo.h>
#include <lbs/lbsgpsmeasurement.h>

// items under test
#include "suplmessagebase.h"
#include "suplasn1decoder.h"
#include "suplmessagecommon.h"
#include "suplstart.h"
#include "suplresponse.h"
#include "suplposinit.h"
#include "suplpos.h"
#include "suplpospayload.h"
#include "suplend.h"
#include "rrlpassistancedata.h"
#include "rrlpassistancedataack.h"
#include "rrlpmeasureposrequest.h"
#include "rrlpmeasureposresponse.h"
#include "rrlpprotocolerror.h"


CSuplRrlpAsn1Step::~CSuplRrlpAsn1Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn1Step::CSuplRrlpAsn1Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn1Step);
	}

TVerdict CSuplRrlpAsn1Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn1Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestReceiveSuplResponseL());
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			}
		else
			{
			SetTestStepResult(EPass);
			}
		}
	  return TestStepResult();
	}

TVerdict CSuplRrlpAsn1Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}

void CSuplRrlpAsn1Step::TestReceiveSuplResponseL()
	{
	TInt err;

	// Receive a SUPL RESPONSE
	TPtr8 decodePtr(receiveBuffer.Des());
	err = iSpoofServer->GenerateSuplResponseL(decodePtr);
	LeaveIfErrorL(err);

	// Create an ASN1 Decoder object...
	CSuplAsn1Decoder* suplAsn1Decoder = CSuplAsn1Decoder::NewL();
	CleanupStack::PushL(suplAsn1Decoder);

	// ...and decode the received message
	TPtrC8 receivePtr(decodePtr);
	CSuplMessageBase* receivedMessage = suplAsn1Decoder->DecodeL(&receivePtr, err);
	CleanupStack::PushL(receivedMessage);

	// extract the version
	CSuplVersion* version = CSuplVersion::NewL();
	CleanupStack::PushL(version);
	err = receivedMessage->GetVersion(*version);
	LeaveIfErrorL(err);
	err = CheckVersionContentL(*version);
	LeaveIfErrorL(err);

	// extract the Session ID
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	err = receivedMessage->GetSessionId(*sessionId);
	LeaveIfErrorL(err);

	CheckSessionIdContentL(*sessionId);
	LeaveIfErrorL(err);

	// message type
	CSuplMessageBase::TSuplMessageType type = receivedMessage->MessageType();

	// handle message type specific stuff
	if (type == CSuplMessageBase::ESuplResponse)
		{
		CSuplResponse* response = static_cast <CSuplResponse*> (receivedMessage);
		
		// extract the Pos Method
		TLbsNetPosRequestMethod posMethod;
		err = response->GetPosMethod(posMethod);
		LeaveIfErrorL(err);
		
		err = CheckPosMethodContentL(posMethod);
		LeaveIfErrorL(err);
		
		if (response->SlpAddressPresent())
			{
			CSuplSlpAddress* suplSlpAddress = CSuplSlpAddress::NewL();
			CleanupStack::PushL(suplSlpAddress);
			err = response->GetSlpAddress(*suplSlpAddress);
			LeaveIfErrorL(err);
			
			err = CheckSlpAddressContentL(*suplSlpAddress);
			LeaveIfErrorL(err);
			
			CleanupStack::PopAndDestroy(suplSlpAddress);		
			}
		}
	else
		{
		// unexpected message received
		err = KErrGeneral;
		LeaveIfErrorL(err);
		}
	
	CleanupStack::PopAndDestroy(sessionId);
	CleanupStack::PopAndDestroy(version);		
	CleanupStack::PopAndDestroy(receivedMessage);	
	CleanupStack::PopAndDestroy(suplAsn1Decoder);
	}


// check received supl response content
TInt CSuplRrlpAsn1Step::CheckPosMethodContentL(TLbsNetPosRequestMethod& aMethod)
	{
	if (aMethod.NumPosMethods() != 1)
		{
		return KErrGeneral;
		}
	
	TLbsNetPosMethod posMethod;
	if (aMethod.GetPosMethod(0, posMethod) != KErrNone)
		{
		return KErrGeneral;
		}

	TUid means = posMethod.PosMeans();
	if (means != KLbsPositioningMeansGps)
		{
		return KErrGeneral;
		}
	
	TPositionModuleInfo::TTechnologyType expectedPosMode = TPositionModuleInfo::ETechnologyTerminal | TPositionModuleInfo::ETechnologyAssisted;
	if (posMethod.PosMode() != expectedPosMode)
		{
		return KErrGeneral;
		}
	
	return KErrNone;
	}
	
TInt CSuplRrlpAsn1Step::CheckSlpAddressContentL(CSuplSlpAddress& aAddress)
	{
	if (aAddress.iSlpAddressType != ESuplSlpAddressTypeFqdn)
		{
		return KErrGeneral;
		}

	if (aAddress.iFqdn == NULL)
		{
		return KErrGeneral;
		}

	_LIT8(KExpectedSlpFqdn, "test.supl.fqdn");
	if (aAddress.iFqdn->iFqdn.Compare(KExpectedSlpFqdn) != 0)
		{
		return KErrGeneral;
		}
	
	return KErrNone;	
	}




