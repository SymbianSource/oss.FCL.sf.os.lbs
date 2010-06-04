// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file Te_suplconnectionmanagerSuiteStepBase.cpp
 @internalTechnology
*/

#include "Te_suplconnectionmanagerSuiteStepBase.h"
#include "Te_suplconnectionmanagerSuiteDefs.h"
#include "suplspoofserver.h"
#include "rrlpmeasureposresponse.h"

// Device driver constants

TVerdict CTe_suplconnectionmanagerSuiteStepBase::doTestStepPreambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all initialisation common to derived classes in here.
 * Make it being able to leave if there are any errors here as there's no point in
 * trying to run a test step if anything fails.
 * The leave will be picked up by the framework.
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTe_suplconnectionmanagerSuiteStepBase::doTestStepPostambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all after test treatment common to derived classes in here.
 * Make it being able to leave
 * The leave will be picked up by the framework.
 */
	{

	// process some common post setting to test steps then set SetTestStepResult to EFail or Epass.
	SetTestStepResult(EPass);  // or EFail
	return TestStepResult();
	}

CTe_suplconnectionmanagerSuiteStepBase::~CTe_suplconnectionmanagerSuiteStepBase()
	{
	delete iSocketHandler;
	delete iConnectionManager;
	iTxBuffer.Close();
	}

CTe_suplconnectionmanagerSuiteStepBase::CTe_suplconnectionmanagerSuiteStepBase() :
	CActive(EPriorityStandard), iState(EIdle), iConnectionManager(NULL)
	{
	CActiveScheduler::Add(this);
	iSocketHandler = CSocketHandler::NewL(*this);
	iTxBuffer.CreateL(8192);
	}

void CTe_suplconnectionmanagerSuiteStepBase::Start()
	{
	iSocketHandler->StartL();
	SetTestStepResult(EFail);
	CActiveScheduler::Start();
	}

void CTe_suplconnectionmanagerSuiteStepBase::Stop()
	{
	CActiveScheduler::Stop();
	}



void CTe_suplconnectionmanagerSuiteStepBase::Connected(TInt aWhat)
	{
	switch (aWhat)
		{
		case MSocketHandler::EWhatConnection:
			{
			iState = EConnect;
			TRequestStatus* temp = &iStatus;
			User::RequestComplete(temp, KErrNone);
			SetActive();
			break;
			}
		case MSocketHandler::EWhatSocket:
			{
			// Dont do anything here as we need to wait for the call back from the connection manager to say
			// we are connected
			break;
			}
		}
	}

void CTe_suplconnectionmanagerSuiteStepBase::RunL()
	{
	switch (iState)
		{
		case EConnect:
			{
			if (iConnectionManager == NULL) // We may want to test multiple connections
				{
				iConnectionManager = CSuplConnectionManager::NewL();
				}
			TSuplLocalSessionId sessionId(12345); // Defaults in spoof server
			TLbsHostSettingsId hostSettings = TUid::Uid(0);
			CSuplSessionRecord::TServiceType serviceType = CSuplSessionRecord::EServiceMolr;
			
			iConnectionManager->Connect(sessionId, hostSettings, serviceType, *this);
			break;
			}
		case ESendSuplStart:
			{
			// Lets send a message
			CSuplStart* message;
			BuildSuplStartL(message);
			iConnectionManager->SendMessage(message, 12345);
			break;
			}
		case ESendSuplEnd:
			{
			// Send back supl end
			CSuplSpoofServer* spoof = CSuplSpoofServer::NewL();
			CleanupStack::PushL(spoof);
			HBufC8* buffer = HBufC8::NewLC(8192);
			TPtr8 ptr(buffer->Des());
			spoof->GenerateSuplEndL(ptr);
			iSocketHandler->SendMessageL(ptr);
			
			CleanupStack::PopAndDestroy(2);
			break;
			}
		case ESendSuplResponse:
			{
			CSuplSpoofServer* spoof = CSuplSpoofServer::NewL();
			CleanupStack::PushL(spoof);
			HBufC8* buffer = HBufC8::NewLC(8192);
			TPtr8 ptr(buffer->Des());
			spoof->GenerateSuplResponseL(ptr);
			iSocketHandler->SendMessageL(ptr);
			
			CleanupStack::PopAndDestroy(2);
			break;
			}
		case ESendSuplPosInit:
			{
			CSuplPosInit* posinit = NULL;
			BuildSuplPosInitL(posinit);
			
			iConnectionManager->SendMessage(posinit, 12345);
			
			break;
			}
		case ESendSuplPosAssData:
			{
			CSuplSpoofServer* spoof = CSuplSpoofServer::NewL();
			CleanupStack::PushL(spoof);
			HBufC8* buffer = HBufC8::NewLC(8192);
			TPtr8 ptr(buffer->Des());
			spoof->GenerateSuplPosRrlpAssistanceDataL(ptr);
			iSocketHandler->SendMessageL(ptr);
			
			CleanupStack::PopAndDestroy(2);
			break;
			}
		case ESendSuplPosAck:
			{
			CSuplPos* pos = NULL;
			BuildSuplPosAckL(pos);
			
			iConnectionManager->SendMessage(pos, 12345);
			
			break;
			}
		case ESendSuplLocReq:
			{
			CSuplSpoofServer* spoof = CSuplSpoofServer::NewL();
			CleanupStack::PushL(spoof);
			HBufC8* buffer = HBufC8::NewLC(8192);
			TPtr8 ptr(buffer->Des());
			spoof->GenerateSuplPosRrlpMeasurePositionRequestL(ptr);
			iSocketHandler->SendMessageL(ptr);
			
			CleanupStack::PopAndDestroy(2);
			break;
			}
		case ESendSuplLocResp:
			{
			CSuplPos* pos = NULL;
			BuildSuplPosRespL(pos);
			
			iConnectionManager->SendMessage(pos, 12345);
			
			
			break;
			}
		default:
			{
			ASSERT(EFalse);
			}
		};
	}

TInt CTe_suplconnectionmanagerSuiteStepBase::RunError(TInt aError)
	{
	ASSERT(EFalse);
	
	return aError;
	}

void CTe_suplconnectionmanagerSuiteStepBase::DoCancel()
	{
	}



void CTe_suplconnectionmanagerSuiteStepBase::SendSuplStart()
	{
	iState = ESendSuplStart;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}


void CTe_suplconnectionmanagerSuiteStepBase::SendSuplEnd()
	{
	iState = ESendSuplEnd;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}

void CTe_suplconnectionmanagerSuiteStepBase::SendSuplResponse()
	{
	iState = ESendSuplResponse;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}

void CTe_suplconnectionmanagerSuiteStepBase::SendSuplPosInit()
	{
	iState = ESendSuplPosInit;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}

void CTe_suplconnectionmanagerSuiteStepBase::SendSuplPosAssData()
	{
	iState = ESendSuplPosAssData;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}

void CTe_suplconnectionmanagerSuiteStepBase::SendSuplPosAck()
	{
	iState = ESendSuplPosAck;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}

void CTe_suplconnectionmanagerSuiteStepBase::SendSupPosLocReq()
	{
	iState = ESendSuplLocReq;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}

void CTe_suplconnectionmanagerSuiteStepBase::SendSupPosLocResp()
	{
	iState = ESendSuplLocResp;
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}




void CTe_suplconnectionmanagerSuiteStepBase::BuildSuplStartL(CSuplStart*& aMessage)
	{
	// check for any returned error code
	TInt err = KErrNone;

	// create a CSuplSessionId and populate the SET session ID only
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId, ETrue);
	
	// SET positioning capabilities
	TLbsNetPosCapabilities caps; // note protocols parameter is ignored so do not set
	TInt numMethods = 3;
	TLbsNetPosMethod posMethods[KLbsMaxNetPosMethods];
	posMethods[0].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyTerminal | TPositionModuleInfo::ETechnologyAssisted);
	posMethods[1].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyNetwork | TPositionModuleInfo::ETechnologyAssisted);
	posMethods[2].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyTerminal);
	err = caps.SetPosMethods(posMethods, numMethods);
	User::LeaveIfError(err);
	
	// location id
	CSuplLocationId* locationId = CSuplLocationId::NewL(ESuplLocationTypeGsm);
	CleanupStack::PushL(locationId);
	locationId->iGsmCellInfo->iRefMCC = 123;   //  (0..999),   -- Mobile Country Code
	locationId->iGsmCellInfo->iRefMNC = 456;   //  (0..999),   -- Mobile Network Code
	locationId->iGsmCellInfo->iRefLAC = 23456; //  (0..65535), -- Location area code
	locationId->iGsmCellInfo->iRefCI  = 65432; //  (0..65535), -- Cell identity
	locationId->iGsmCellInfo->iNMR    = 2;     //  (0..15)     --  no. NMR elements
	locationId->iGsmCellInfo->iNmrElements[0].iARFCN = 357; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[0].iBSIC  = 24;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[0].iRxLev = 63;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iARFCN = 631; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[1].iBSIC  = 01;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iRxLev = 12;  // (0..63)
	locationId->iGsmCellInfo->iTA     = 33;     //  (0..255) OPTIONAL, -- Timing Advance
	
	// pos request quality
	TLbsNetPosRequestQuality quality;
	TReal32 horizontalAccuracy = 57.3;
	TReal32 verticalAccuracy = 75.8;
	TTimeIntervalMicroSeconds maxFixTime(30 * 1000 * 1000);
	TTimeIntervalMicroSeconds maxFixAge(0);
	quality.SetMinHorizontalAccuracy(horizontalAccuracy);
	quality.SetMinVerticalAccuracy(verticalAccuracy);
	quality.SetMaxFixTime(maxFixTime);
	quality.SetMaxFixAge(maxFixAge);
		
	// Create the SUPL START Message
	aMessage = CSuplStart::NewL();
	CleanupStack::PushL(aMessage);
	
	// Populate using the parameters above
	err = aMessage->SetSessionId(*sessionId);
	User::LeaveIfError(err);
	err = aMessage->SetCapabilities(caps);
	User::LeaveIfError(err);
	err = aMessage->SetLocationId(*locationId);
	User::LeaveIfError(err);
	err = aMessage->SetQoP(quality);
	User::LeaveIfError(err);
	
	CleanupStack::Pop(aMessage);
	CleanupStack::PopAndDestroy(locationId);
	CleanupStack::PopAndDestroy(sessionId);
	}


void CTe_suplconnectionmanagerSuiteStepBase::PopulateSessionId(CSuplSessionId* aSessionId, TBool aIsSuplStart)
	{
	aSessionId->iSetSessionId->iSessionId = 12345; // 0..65535;
	aSessionId->iSetSessionId->iSetId->iSetIdType = ESuplSetIdTypeIPAddress; // TSuplSetIdType
	aSessionId->iSetSessionId->iSetId->iIpAddress->iIpAddressType = ESuplIpAddressTypeV4; // TSuplIpAddressType
	_LIT8(KSetIpv4Address,"abcd");
	aSessionId->iSetSessionId->iSetId->iIpAddress->iIpAddress.Copy(KSetIpv4Address); // TBuf8<16>
	aSessionId->iSetSessionIdPresent=ETrue;
	
	// SLP SESSION ID
	if (!aIsSuplStart)
		{
		_LIT8(KSlpIdSessionId, "EFGH");
		aSessionId->iSlpSessionId->iSessionId.Copy(KSlpIdSessionId);
		aSessionId->iSlpSessionId->iSlpAddress->iSlpAddressType = ESuplSlpAddressTypeIp; 
		aSessionId->iSlpSessionId->iSlpAddress->iIpAddress->iIpAddressType = ESuplIpAddressTypeV4;
		_LIT8(KSlpIdIpAddress, "VWXY");
		aSessionId->iSlpSessionId->iSlpAddress->iIpAddress->iIpAddress.Copy(KSlpIdSessionId);
		}
	}

void CTe_suplconnectionmanagerSuiteStepBase::BuildSuplPosAckL(CSuplPos*& aMessage)
	{
	aMessage = CSuplPos::NewL(ETrue);
	CleanupStack::PushL(aMessage);
	
	CRrlpAssistanceDataAck* payload = CRrlpAssistanceDataAck::NewL();
	CleanupStack::PushL(payload);
	TRrlpReference ref;
	ref.aRefNum = 1;
	ref.aRel5EntendedRefPresent = EFalse;
	payload->SetReference(ref);
	aMessage->SetPosPayload(payload);

	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId, ETrue);
	aMessage->SetSessionId(*sessionId);

	
	CleanupStack::PopAndDestroy(sessionId);
	CleanupStack::Pop(payload);
	CleanupStack::Pop(aMessage);
	}

void CTe_suplconnectionmanagerSuiteStepBase::BuildSuplPosRespL(CSuplPos*& aMessage)
	{
	aMessage = CSuplPos::NewL(ETrue);
	CleanupStack::PushL(aMessage);
	
	CRrlpMeasurePositionResponse* payload = CRrlpMeasurePositionResponse::NewL();
	CleanupStack::PushL(payload);
	
	TRrlpReference reference;
	reference.aRefNum = 5; 
	reference.aRel5EntendedRefPresent = ETrue;
	reference.aRel5SmlcCode = 39;
	reference.aRel5TransactionId = 13337;
	payload->SetReference(reference);
	
	aMessage->SetPosPayload(payload);
	
	CleanupStack::Pop(payload);
	CleanupStack::Pop(aMessage);
	}

void CTe_suplconnectionmanagerSuiteStepBase::BuildSuplPosInitL(CSuplPosInit*& aMessage)
	{
	aMessage = CSuplPosInit::NewL();
 	CleanupStack::PushL(aMessage);
 	
	TLbsNetPosCapabilities capabilities;
	aMessage->SetCapabilities(capabilities);

	TLbsAsistanceDataGroup assitDataReqMask;
 	aMessage->SetRequestedAssistanceData(assitDataReqMask);

	// location id
	CSuplLocationId* locationId = CSuplLocationId::NewL(ESuplLocationTypeGsm);
	CleanupStack::PushL(locationId);
	locationId->iGsmCellInfo->iRefMCC = 123;   //  (0..999),   -- Mobile Country Code
	locationId->iGsmCellInfo->iRefMNC = 456;   //  (0..999),   -- Mobile Network Code
	locationId->iGsmCellInfo->iRefLAC = 23456; //  (0..65535), -- Location area code
	locationId->iGsmCellInfo->iRefCI  = 65432; //  (0..65535), -- Cell identity
	locationId->iGsmCellInfo->iNMR    = 2;     //  (0..15)     --  no. NMR elements
	locationId->iGsmCellInfo->iNmrElements[0].iARFCN = 357; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[0].iBSIC  = 24;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[0].iRxLev = 63;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iARFCN = 631; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[1].iBSIC  = 01;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iRxLev = 12;  // (0..63)
	locationId->iGsmCellInfo->iTA     = 33;     //  (0..255) OPTIONAL, -- Timing Advance
 	aMessage->SetLocationId(*locationId);
	
	
	CleanupStack::PopAndDestroy(locationId);
 	CleanupStack::Pop(aMessage);
	}
