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
// incoming SUPL INIT
// 
//

/**
 @file suplrrlpasn12step.cpp
 @internalTechnology
*/
#include "suplrrlpasn12step.h"
#include "te_suplrrlpasnsuitedefs.h"


// items under test
#include "suplmessagebase.h"
#include "suplasn1decoder.h"
#include "suplmessagecommon.h"
#include "suplstart.h"
#include "suplresponse.h"
#include "suplposinit.h"
#include "suplinit.h"
#include "suplpos.h"
#include "suplpospayload.h"
#include "suplend.h"
#include "rrlpassistancedata.h"
#include "rrlpassistancedataack.h"
#include "rrlpmeasureposrequest.h"
#include "rrlpmeasureposresponse.h"
#include "rrlpprotocolerror.h"

#include <lbs/lbsloccommon.h>

CSuplRrlpAsn12Step::~CSuplRrlpAsn12Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn12Step::CSuplRrlpAsn12Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn12Step);
	}

TVerdict CSuplRrlpAsn12Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn12Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestReceiveSuplInitL());
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

TVerdict CSuplRrlpAsn12Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}


void CSuplRrlpAsn12Step::TestReceiveSuplInitL()
	{
	TInt err;

	// Receive a SUPL INIT
	TPtr8 decodePtr(receiveBuffer.Des());
	err = iSpoofServer->GenerateSuplInitL(decodePtr);
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
	if (type == CSuplMessageBase::ESuplInit)
		{
		CSuplInit* suplInit = static_cast <CSuplInit*> (receivedMessage);
		
		// SLP MODE
		CSuplInit::TSuplSlpMode slpMode = suplInit->SlpMode();
		if (slpMode != CSuplInit::ESuplSlpModeProxy)
			{
			User::Leave(KErrGeneral);
			}
		
		// POS METHOD
		TLbsNetPosRequestMethod posMethod;
		err = suplInit->GetPosMethod(posMethod);
		LeaveIfErrorL(err);
		err = CheckPosMethodContentL(posMethod);
		LeaveIfErrorL(err);
		
		// NOTIFICATION
		if (suplInit->NotificationPresent())
			{
			TLbsNetPosRequestPrivacy privacy;
			err = suplInit->GetNotificationType(privacy);
			LeaveIfErrorL(err);
			err = CheckNotificationContentL(privacy);
			LeaveIfErrorL(err);
			}
		
		// NOTIFICATION/external request info
		if (suplInit->ExternalRequestInfoPresent())
			{
			TLbsExternalRequestInfo requestInfo;
			err = suplInit->GetExternalRequestInfo(requestInfo);
			LeaveIfErrorL(err);
			err = CheckExternalRequestInfoContentL(requestInfo);
			LeaveIfErrorL(err);
			}
			
		
		// SLP ADDRESS
		if (suplInit->SlpAddressPresent())
			{
			CSuplSlpAddress* suplSlpAddress = CSuplSlpAddress::NewL();
			CleanupStack::PushL(suplSlpAddress);
			err = suplInit->GetSlpAddress(*suplSlpAddress);
			LeaveIfErrorL(err);
			err = CheckSlpAddressContentL(*suplSlpAddress);
			LeaveIfErrorL(err);
			CleanupStack::PopAndDestroy(suplSlpAddress);		
			}
		
		// QOP
		if (suplInit->QopPresent())
			{
			TLbsNetPosRequestQuality quality;
			err = suplInit->GetQop(quality);
			LeaveIfErrorL(err);
			err = CheckQopL(quality);
			LeaveIfErrorL(err);
			}
		
		// VER (note test does not check the result of the calculated hash)
		_LIT8(KSlpAddress, "address");
		TPtrC8 ver = suplInit->GetVerL(KSlpAddress);
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


TInt CSuplRrlpAsn12Step::CheckPosMethodContentL(TLbsNetPosRequestMethod& aMethod)
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


TInt CSuplRrlpAsn12Step::CheckNotificationContentL(TLbsNetPosRequestPrivacy& aPrivacy)
	{
	TLbsNetPosRequestPrivacy::TLbsRequestAdvice requestAdvice = aPrivacy.RequestAdvice();
	if (requestAdvice != TLbsNetPosRequestPrivacy::ERequestAdviceVerify)
		{
		return KErrGeneral;
		}
	
	TLbsNetPosRequestPrivacy::TLbsRequestAction requestAction = aPrivacy.RequestAction();
	if (requestAction != TLbsNetPosRequestPrivacy::ERequestActionAllow)
		{
		return KErrGeneral;
		}
	return KErrNone;
	}

TInt CSuplRrlpAsn12Step::CheckExternalRequestInfoContentL(TLbsExternalRequestInfo& aRequestInfo)
	{
	_LIT8(KExpectedRequesterId, "reques");
	TBuf8<128> requesterId;
	aRequestInfo.GetRequesterId(requesterId);
	if (requesterId.Compare(KExpectedRequesterId) != 0)
		{
		return KErrGeneral;
		}
	
	_LIT8(KExpectedClientName, "clientNameR");
	TBuf8<128> clientName;	
	aRequestInfo.GetClientName(clientName);
	if (clientName.Compare(KExpectedClientName) != 0)
		{
		return KErrGeneral;
		}

// these elements are not set...
//	TLbsExternalRequestInfo::TRequestType     requestType   = aRequestInfo.RequestType();
//	TLbsExternalRequestInfo::TRequestSource   requestSource = aRequestInfo.RequestSource();
//	TLbsExternalRequestInfo::TNetworkType     networkType   = aRequestInfo.NetworkType();

	if (aRequestInfo.RequesterIdFormat() != TLbsExternalRequestInfo::EFormatLogicalName)
		{
		return KErrGeneral;
		}

	if (aRequestInfo.ClientNameFormat() != TLbsExternalRequestInfo::EFormatEmailAddress)
		{
		return KErrGeneral;
		}
	
	if (aRequestInfo.RequesterIdCodingScheme() != TLbsExternalRequestInfo::ECodingSchemeUCS2)
		{
		return KErrGeneral;
		}

	if (aRequestInfo.ClientNameCodingScheme() != TLbsExternalRequestInfo::ECodingSchemeUCS2)
		{
		return KErrGeneral;
		}


	return KErrNone;
	}



TInt CSuplRrlpAsn12Step::CheckSlpAddressContentL(CSuplSlpAddress& aAddress)
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

TInt CSuplRrlpAsn12Step::CheckQopL(TLbsNetPosRequestQuality& aQuality)
	{
	// max fix time == delay
	if (aQuality.MaxFixTime().Int64() != 32*1000*1000) // 2^5 seconds
		return KErrGeneral;
	
	// max fix age == max location age
	if (aQuality.MaxFixAge().Int64() != 74*1000*1000)
		return KErrGeneral;
	
	TReal32 temp = 88.49733 - aQuality.MinHorizontalAccuracy();
	
	if (temp < -0.5 || temp > 0.5)
		return KErrGeneral;
	
	temp = 54.16906 - aQuality.MinVerticalAccuracy();
	if (temp < -0.5 || temp > 0.5)
		return KErrGeneral;
	
	return KErrNone;
	}
	

