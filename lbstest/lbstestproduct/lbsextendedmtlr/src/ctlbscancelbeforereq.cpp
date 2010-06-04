// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <lbs/test/lbsnetextendedprotocolproxy.h>
#include <lbs/lbsnetprotocolbase.h>
#include <lbs/lbsassistancedatabuilderset.h>
// LBS test includes.
#include "ctlbscancelbeforereq.h"
#include <lbs/test/tlbsutils.h>
#include "argutils.h"
#include <lbs/test/activeyield.h>
/**
Static Constructor
*/
CT_LbsCancelBeforeReq* CT_LbsCancelBeforeReq::New(CT_LbsExtendedMTLRServer& aParent)
	{
	// Note that we do not use ELeave.
	// This means that having insufficient memory will return NULL;
	CT_LbsCancelBeforeReq* testStep = new CT_LbsCancelBeforeReq(aParent);
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
CT_LbsCancelBeforeReq::CT_LbsCancelBeforeReq(CT_LbsExtendedMTLRServer& aParent) : CT_LbsExtendedMTLRStep(aParent)
	{
	SetTestStepName(KLbsCancelBeforeReq);
	iState = EInitializing;
	iSessionId.SetSessionOwner(KRequestUid);
	iSessionId.SetSessionNum(0x0005);
	}


void CT_LbsCancelBeforeReq::ConstructL()
	{
	// Create the base class objects.
	CT_LbsExtendedMTLRStep::ConstructL();
	iController = CLbsPrivacyController::NewL(*this);
	iProxy = CNetProtocolProxy::NewL();	
	}


/**
 * Destructor
 */
CT_LbsCancelBeforeReq::~CT_LbsCancelBeforeReq()
	{
	delete iController;
	delete iProxy;
	}


TVerdict CT_LbsCancelBeforeReq::doTestStepL()
	{
	INFO_PRINTF1(_L("CT_LbsPTBMTLRCancel::doTestStepL()"));	
	// Stop the test if the preamble failed
	TESTL(TestStepResult() == EPass);

	const TInt KTimeOut = 60*1000*1000;
	const TInt KAdviceSystemStatusTimeout = 40*1000*1000;
	const TInt KSmallTimeOut = 3*1000*1000;

	// reset integration modules count of number of cancels recieved from LBS 
	T_LbsUtils utils;
	utils.ResetIntegrationModulesCountOfCancelsL();

	// >> AdviceSystemStatus(0)
	TESTL(iProxy->WaitForResponse(KAdviceSystemStatusTimeout) == ENetMsgGetCurrentCapabilitiesResponse);
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

//now cancel BEFORE input the location request
	InitiateCancelMTLR(iSessionId.SessionNum());

	// for this test the protocol module is the extended module and therefore the 
	// CancelExternalLocation is expected now ...
	
	// >> CancelExternalLocation()
	TInt response = iProxy->WaitForResponse(KSmallTimeOut);
	TESTL(response == ENetMsgCancelExternalLocation);
	
	TInt getReason = KErrGeneral;
	getSessionId = NULL;
	cleanupCnt = iProxy->GetArgsLC(ENetMsgCancelExternalLocation, &getSessionId, &getReason);
	TESTL(getSessionId->SessionNum() == iSessionId.SessionNum());
	TESTL(getReason==KErrCancel);
	CleanupStack::PopAndDestroy(cleanupCnt); 
	
// MTLR Network Location Request Start
	// << ProcessLocationRequest()
	MLbsNetworkProtocolObserver::TLbsNetProtocolService service = MLbsNetworkProtocolObserver::EServiceMobileTerminated;
	TLbsNetPosRequestQuality quality = ArgUtils::QualityAlpha2(); 
	TLbsNetPosRequestMethod method   = ArgUtils::RequestTerminalBasedMethod();
	iProxy->CallL(ENetMsgProcessLocationRequest, &iSessionId, &emergency, &service, &quality, &method);

	// >> RespondLocationRequest()

	response = iProxy->WaitForResponse(KSmallTimeOut);
	TESTL(response == ENetMsgRespondLocationRequest);
	
	getSessionId = NULL;
	getReason = KErrCancel;
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
	
	// and finally check that no cancel was sent to test integration module
	// because, of course, no location request was sent to the agps module
	// - a cancel occured before the location request and therfore no request
	// was sent to the agps module

	TInt cancelCount  = utils.IntegrationModulesCountOfCancelsL();

	INFO_PRINTF2(_L("--- cancel count = %d"), cancelCount);

	TESTL(cancelCount== 0);
	INFO_PRINTF1(_L("CT_LbsPTBMTLRCancel::doTestStepL() END"));	

	return TestStepResult();
	}
	
void CT_LbsCancelBeforeReq::ProcessNetworkLocationRequest(TUint aRequestId, const TLbsExternalRequestInfo& /*aRequestInfo*/, const TNotificationType& /*aNotificationType*/)
	{
	INFO_PRINTF1(_L("&gt;&gt;CT_LbsCancelBeforeReq::ProcessNetworkLocationRequest()"));
	TEST(iState==EInitializing);
	iController->RespondNetworkLocationRequest(aRequestId, CLbsPrivacyController::ERequestAccepted);
	iState = EPrivacyCheckOk;
	ReturnToTestStep();
	}
   
void CT_LbsCancelBeforeReq::ProcessNetworkPositionUpdate(TUint /*aRequestId*/, const TPositionInfo& aPosInfo)
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
		INFO_PRINTF1(_L("&gt;&gt;CT_LbsCancelBeforeReq::ProcessNetworkPositionUpdate(RefPosition)"));
		}
	ReturnToTestStep();
	}

void CT_LbsCancelBeforeReq::ProcessRequestComplete(TUint /*aRequestId*/, TInt aReason)
	{
	INFO_PRINTF2(_L("&gt;&gt;CT_LbsPTBMTLRCancel::ProcessRequestComplete(%d)"), aReason);
	TEST(KErrCancel==aReason);
	TEST(iState==ERefLocReceived);
	iState=ERequestComplete; 
	ReturnToTestStep();
	}

void CT_LbsCancelBeforeReq::InitiateCancelMTLR(TUint aRequestId)
	{
	INFO_PRINTF1(_L("&gt;&gt;CT_LbsCancelBeforeReq::initiateCancelMTLR()"));
	iController->CancelNetworkLocationRequest(aRequestId);
	INFO_PRINTF1(_L("Cancel Initiated"));
	}
