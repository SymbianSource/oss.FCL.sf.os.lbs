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
// Example CTestStep derived implementation
// 
//

/**
 @file TExampleSequenceStep.cpp
 @internalTechnology
*/
#include "TExampleSequenceStep.h"
#include "Te_LbsNetProtocolProxyUnitTestSuiteDefs.h"

CTExampleSequenceStep::~CTExampleSequenceStep()
/**
 * Destructor
 */
	{
	}

CTExampleSequenceStep::CTExampleSequenceStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTExampleSequenceStep);
	}

TVerdict CTExampleSequenceStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Please delete this line or modify it. I am in Test Step Preamble in Class CTExampleSequenceStep"));
	// uncomment the following 3 lines if you have common pre setting to all the test steps in there
	CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPreambleL();
	if (TestStepResult()!=EPass)
	   return   TestStepResult();
	// process some pre setting to this test step then set SetTestStepResult to EFail or Epass.
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CTExampleSequenceStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	INFO_PRINTF1(_L("CTExampleSequenceStep::doTestStepL"));
/*	if (TestStepResult()==EPass)
		{
		TLbsNetSessionId sessionId(TUid::Uid(0x87654321), 0x1111);
		
		//	function: PrivacyCheck() //
		TNetProtocolActionType a1 = ENetMsgProcessStatusUpdate;
		TNetProtocolActionType a2 = ENetMsgProcessPrivacyRequest;
		iProxy->Call(a1, MLbsNetworkProtocolObserver::EServiceMobileTerminated);
		iProxy->Call(a2, sessionId);
		// sessionId = 4 // type sessionId is TLbsNetSessionId

		TInt someTimeOut(60*1000*1000);

		ASSERT_TRUE(iProxy->WaitForResponse(someTimeOut) == ENetMsgRespondPrivacyRequest);

/*		SData data;
		iProxy->GetArgs(ENetMsgRespondPrivacyRequest, &data);
		// 
		ASSERT_TRUE(data.sessionId.SessionNum()==sessionId);
		ASSERT_TRUE(data.response == CLbsNetworkProtocolBase::EPrivacyResponseAccepted);
*/
/*		// function: ProcessLocationUpdate??? //
		TNetProtocolActionType a3 = ENetMsgProcessLocationUpdate;
		TNetProtocolActionType a4 = ENetMsgProcessAssistanceData;
		iProxy->Call(a3, sessionId);
		iProxy->Call(a4);

		TNetProtocolActionType a5 = ENetMsgProcessLocationRequest;
		iProxy->Call(a5, sessionId);
	
		ASSERT_TRUE(iProxy->WaitForResponse(someTimeOut) == ENetMsgRespondLocationRequest);

/*
		iProxy->GetArgs(ENetMsgRespondLocationRequest, &data);
		ASSERT_TRUE(data.sessionId.SessionNum()==sessionId);
		//
*/
/*		TNetProtocolActionType a6 = ENetMsgProcessSessionComplete;
		iProxy->Call(a6, sessionId);

		TNetProtocolActionType a7 = ENetMsgProcessStatusUpdate;
		iProxy->Call(a7, MLbsNetworkProtocolObserver::EServiceNone);

		SetTestStepResult(EPass);
		}
*/	return TestStepResult();
	}



TVerdict CTExampleSequenceStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Please delete this line or modify it. I am in Test Step Postamble in Class CTExampleSequenceStep"));
	// process something post setting to the test step
	// uncomment the following line if you have common post setting to all the test steps in there
	// CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPostambleL();
	// uncomment the following line if you have post process or remove the following line if no post process
	// SetTestStepResult(EPass);		// or EFail
	return TestStepResult();
	}
