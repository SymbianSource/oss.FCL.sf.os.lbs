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
// LBS includes. 
// 
//

#include <lbs/test/lbsnetprotocolproxy.h>
#include <lbs/lbsnetprotocolbase.h>
#include <lbs/lbsassistancedatabuilderset.h>
// LBS test includes.
#include "ctlbsptbmtlr_beforereq_cancel.h"
#include <lbs/test/tlbsutils.h>
#include "argutils.h"
#include <lbs/test/activeyield.h>
/**
Static Constructor
*/
CT_LbsPTBMTLR_Before_Req_Cancel* CT_LbsPTBMTLR_Before_Req_Cancel::New(CT_LbsPTBMTLRServer& aParent)
	{
	// Note that we do not use ELeave.
	// This means that having insufficient memory will return NULL;
	CT_LbsPTBMTLR_Before_Req_Cancel* testStep = new CT_LbsPTBMTLR_Before_Req_Cancel(aParent);
	if (testStep)
		{
		TInt err = KErrNone;

		TRAP(err, testStep->ConstructL());
		if (err)
			{
			delete testStep;
			testStep = NULL;
			}
		}
	return testStep;
	}


/**
 * Constructor
 */
CT_LbsPTBMTLR_Before_Req_Cancel::CT_LbsPTBMTLR_Before_Req_Cancel(CT_LbsPTBMTLRServer& aParent) : CT_LbsPTBMTLRStep(aParent)
	{
	SetTestStepName(KLbsPTBMTLR_BeforeReq_Cancel);
	iState = EInitializing;
	iSessionId.SetSessionOwner(KRequestUid);
	iSessionId.SetSessionNum(0x0005);
	}


void CT_LbsPTBMTLR_Before_Req_Cancel::ConstructL()
	{
	// Create the base class objects.
	CT_LbsPTBMTLRStep::ConstructL();
	iController = CLbsPrivacyController::NewL(*this);
	iProxy = CNetProtocolProxy::NewL();	
	}


/**
 * Destructor
 */
CT_LbsPTBMTLR_Before_Req_Cancel::~CT_LbsPTBMTLR_Before_Req_Cancel()
	{
	delete iController;
	delete iProxy;
	}


TVerdict CT_LbsPTBMTLR_Before_Req_Cancel::doTestStepL()
	{
	INFO_PRINTF1(_L("CT_LbsPTBMTLR_Before_Req_Cancel::doTestStepL()"));	
	// Stop the test if the preamble failed
	TESTL(TestStepResult() == EPass);
	
	const TInt KTimeOut = 60*1000*1000;
	const TInt KAdviceSystemStatusTimeout = 40*1000*1000;
	const TInt KSmallTimeOut = 3*1000*1000;

	// reset integration modules count of number of cancels recieved from LBS 
	T_LbsUtils utils;
	utils.ResetIntegrationModulesCountOfCancelsL();

	// >> AdviceSystemStatus(0)
	
	TInt response = iProxy->WaitForResponse(KAdviceSystemStatusTimeout);
	if(response!=ENetMsgGetCurrentCapabilitiesResponse)
		{
		INFO_PRINTF2(_L("Response was %d"),response);
		}
	
	TESTL(response == ENetMsgGetCurrentCapabilitiesResponse);
	CLbsNetworkProtocolBase::TLbsSystemStatus status;
	TInt cleanupCnt;
	cleanupCnt = iProxy->GetArgsLC(ENetMsgGetCurrentCapabilitiesResponse, &status); 
	TESTL(status == CLbsNetworkProtocolBase::ESystemStatusNone);
	CleanupStack::PopAndDestroy(cleanupCnt);

// Initiate MTLR Start
	// << ProcessStatusUpdate()
	MLbsNetworkProtocolObserver::TLbsNetProtocolServiceMask activeServiceMask = MLbsNetworkProtocolObserver::EServiceMobileTerminated;
	iProxy->CallL(ENetMsgProcessStatusUpdate, &activeServiceMask);

	// << ProcessPrivacyRequest()
	const TBool emergency = EFalse;
	TLbsNetPosRequestPrivacy privacy    = ArgUtils::Privacy();
	TLbsExternalRequestInfo requestInfo = ArgUtils::RequestInfo();
	iProxy->CallL(ENetMsgProcessPrivacyRequest, &iSessionId, &emergency, &privacy, &requestInfo);

	// >> Callback from RespondNetworkLocationRequest(ERequestAccepted)
	CheckForObserverEventTestsL(KTimeOut, *this);
	
	// >> Respond Privacy Request
	TESTL(iProxy->WaitForResponse(KTimeOut) == ENetMsgRespondPrivacyRequest);
	TLbsNetSessionId* getSessionId = NULL;
	CLbsNetworkProtocolBase::TLbsPrivacyResponse getPrivacy;
	cleanupCnt = iProxy->GetArgsLC(ENetMsgRespondPrivacyRequest, &getSessionId, &getPrivacy);
	TESTL(getSessionId->SessionNum()==iSessionId.SessionNum());
	TESTL(getPrivacy==CLbsNetworkProtocolBase::EPrivacyResponseAccepted);
	CleanupStack::PopAndDestroy(cleanupCnt);//getSessionId
// Initiate MTLR End

// MTLR Reference Position Notification Start
	// << ProcessLocationUpdate()
	TPositionInfo positionInfo = ArgUtils::ReferencePositionInfo();
	iProxy->CallL(ENetMsgProcessLocationUpdate, &iSessionId, &positionInfo);
// MTLR Reference Position Notification Stop

// MTLR Assistance Data Notification Start
	// << ProcessAssistanceData()
	TLbsAsistanceDataGroup dataRequestMask = EAssistanceDataReferenceTime;
	RLbsAssistanceDataBuilderSet assistanceData;
	ArgUtils::PopulateLC(assistanceData);
	TInt reason = KErrNone;
	iProxy->CallL(ENetMsgProcessAssistanceData, &dataRequestMask, &assistanceData, &reason);
	CleanupStack::PopAndDestroy(); //assistanceData
// MTLR Assistance Data Notification Stop

	// >> Callback from ProcessNetworkLocationRequest()
	CheckForObserverEventTestsL(KTimeOut, *this);
	TESTL(iState==ERefLocReceived);
	
	// Initiate a Cancel
	InitiateCancelMTLR(iSessionId.SessionNum());
	
// MTLR Network Location Request Start
	// << ProcessLocationRequest()
	MLbsNetworkProtocolObserver::TLbsNetProtocolService service = MLbsNetworkProtocolObserver::EServiceMobileTerminated;
	TLbsNetPosRequestQuality quality = ArgUtils::QualityAlpha2(); 
	TLbsNetPosRequestMethod method   = ArgUtils::RequestTerminalBasedMethod();
	iProxy->CallL(ENetMsgProcessLocationRequest, &iSessionId, &emergency, &service, &quality, &method);
// MTLR Network Location Request Stop

	// >> RespondLocationRequest()
	response = iProxy->WaitForResponse(KSmallTimeOut);
	TESTL(response == ENetMsgRespondLocationRequest);
	 
	getSessionId = NULL;
	TInt getReason = KErrCancel;
	TPositionSatelliteInfo* getPositionInfo = NULL;
	cleanupCnt = iProxy->GetArgsLC(ENetMsgRespondLocationRequest, &getSessionId, &getReason, &getPositionInfo);
	TESTL(getSessionId->SessionNum() == iSessionId.SessionNum());
	TESTL(getReason==KErrCancel);
	CleanupStack::PopAndDestroy(cleanupCnt); //getSessionId,getPositionInfo

// MTLR Session Completion Start
	// << ProcessSessionComplete() 
	reason = KErrCancel;
	iProxy->CallL(ENetMsgProcessSessionComplete, &iSessionId, &reason);

	// >> Callback from ProcessRequestComplete()
	CheckForObserverEventTestsL(KTimeOut, *this);

	// Verify that the last callback was to ProcessSessionComplete()
	TESTL(iState==ERequestComplete);
// MTLR Session Completion Stop
	
	// and finally check that no cancels are sent to test integration module
	// this is because no location request has been sent to the integration module.
	TInt cancelCount  = utils.IntegrationModulesCountOfCancelsL();

	INFO_PRINTF2(_L("--- cancel count = %d"), cancelCount);
	TESTL(cancelCount== 0);
	INFO_PRINTF1(_L("CT_LbsPTBMTLR_Before_Req_Cancel::doTestStepL() END"));	

	return TestStepResult();
	}
	
void CT_LbsPTBMTLR_Before_Req_Cancel::ProcessNetworkLocationRequest(TUint aRequestId, const TLbsExternalRequestInfo& /*aRequestInfo*/, const TNotificationType& /*aNotificationType*/)
	{
	INFO_PRINTF1(_L("&gt;&gt;CT_LbsPTBMTLR_Before_Req_Cancel::ProcessNetworkLocationRequest()"));
	TEST(iState==EInitializing);
	iController->RespondNetworkLocationRequest(aRequestId, CLbsPrivacyController::ERequestAccepted);
	iState = EPrivacyCheckOk;
	ReturnToTestStep();
	}
   
void CT_LbsPTBMTLR_Before_Req_Cancel::ProcessNetworkPositionUpdate(TUint /*aRequestId*/, const TPositionInfo& aPosInfo)
	{
	if(iState==EPrivacyCheckOk)
		{
		// check that it is the REF postion we are expecting!
		if (aPosInfo.PositionMode() == TPositionModuleInfo::ETechnologyNetwork)
			{
			TPosition getPos;
			aPosInfo.GetPosition(getPos);
			if (getPos.Longitude()==-0.096) // and others ...
				{
				iState=ERefLocReceived;
				}
			}
		INFO_PRINTF1(_L("&gt;&gt;CT_LbsPTBMTLR_Before_Req_Cancel::ProcessNetworkPositionUpdate(RefPosition)"));
		}
	ReturnToTestStep();
	}

void CT_LbsPTBMTLR_Before_Req_Cancel::ProcessRequestComplete(TUint /*aRequestId*/, TInt aReason)
	{
	INFO_PRINTF2(_L("&gt;&gt;CT_LbsPTBMTLR_Before_Req_Cancel::ProcessRequestComplete(%d)"), aReason);
	TEST(KErrCancel==aReason);
	TEST(iState==ERefLocReceived);
	iState=ERequestComplete; 
	ReturnToTestStep();
	}

void CT_LbsPTBMTLR_Before_Req_Cancel::InitiateCancelMTLR(TUint aRequestId)
	{
	INFO_PRINTF2(_L("&gt;&gt;CT_LbsPTBMTLR_Before_Req_Cancel::initiateCancelMTLR(%d)"), aRequestId);
	iController->CancelNetworkLocationRequest(aRequestId);
	}
