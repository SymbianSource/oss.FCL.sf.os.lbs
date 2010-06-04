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
// incoming SUPL POS with RRLP PROTOCOL ERROR payload
// 
//

/**
 @file suplrrlpasn7step.cpp
 @internalTechnology
*/
#include "suplrrlpasn7step.h"
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


CSuplRrlpAsn7Step::~CSuplRrlpAsn7Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn7Step::CSuplRrlpAsn7Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn7Step);
	}

TVerdict CSuplRrlpAsn7Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn7Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestReceiveSuplPosRrlpProtocolErrorL());
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

TVerdict CSuplRrlpAsn7Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}



void CSuplRrlpAsn7Step::TestReceiveSuplPosRrlpProtocolErrorL()
	{
	TInt err = KErrNone;

	// Receive a SUPL POS with an RRLP PROTOCOL ERROR payload
	TPtr8 decodePtr(receiveBuffer.Des());
	err = iSpoofServer->GenerateSuplPosRrlpProtocolErrorL(decodePtr);
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

	err = CheckSessionIdContentL(*sessionId);
	LeaveIfErrorL(err);
	
	// message type
	CSuplMessageBase::TSuplMessageType type = receivedMessage->MessageType();

	// handle message type specific stuff
	if (type == CSuplMessageBase::ESuplPos)
		{
		CSuplPos* pos = static_cast <CSuplPos*> (receivedMessage);
		
		// VELOCITY component
		if (pos->VelocityPresent())
			{
			// extract velocity.
			}
		
		// POSITIONING PAYLOAD
		CSuplPosPayload* posPayload = pos->PosPayload();
		CleanupStack::PushL(posPayload);
		if (posPayload->MessageType() == CSuplPosPayload::ERrlpProtocolError)
			{
			CRrlpProtocolError* protocolError = static_cast <CRrlpProtocolError*>(posPayload);
			
			// REFERENCE (inc Rel-5 Extended Reference)
			TRrlpReference rrlpReference;
			err = protocolError->GetReference(rrlpReference);
			LeaveIfErrorL(err);
			
			err = CheckRrlpReferenceContentL(rrlpReference);
			LeaveIfErrorL(err);
			
			// ERROR CAUSE
			TRrlpErrorCode errorCause;
			protocolError->GetProtocolError(errorCause);
			
			err = CheckRrlpErrorL(errorCause);
			LeaveIfErrorL(err);
			}
		else
			{
			// unexpected payload received
			err = KErrGeneral;
			LeaveIfErrorL(err);
			}
		CleanupStack::PopAndDestroy(posPayload);
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

TInt CSuplRrlpAsn7Step::CheckRrlpErrorL(TRrlpErrorCode& aErrorCause)
	{
	if (aErrorCause != ERrlpErrorMissingIEorComponentElement)
		{
		return KErrGeneral;
		}
	return KErrNone;
	}

