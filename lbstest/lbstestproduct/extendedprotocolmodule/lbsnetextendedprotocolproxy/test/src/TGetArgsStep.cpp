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
 @file TGetArgsStep.cpp
 @internalTechnology
*/
#include "TGetArgsStep.h"
#include "Te_LbsNetProtocolProxyUnitTestSuiteDefs.h"

CTGetArgsStep::~CTGetArgsStep()
/**
 * Destructor
 */
	{
	}

CTGetArgsStep::CTGetArgsStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTGetArgsStep);
	}

TVerdict CTGetArgsStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Please delete this line or modify it. I am in Test Step Preamble in Class CTGetArgsStep"));
	// uncomment the following 3 lines if you have common pre setting to all the test steps in there
	CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPreambleL();
	if (TestStepResult()!=EPass)
		return   TestStepResult();
	// process some pre setting to this test step then set SetTestStepResult to EFail or Epass.
	SetTestStepResult(EPass);
	return TestStepResult();
	}


void CTGetArgsStep::ProcessArgsL(TNetProtocolResponseType aResponse)
	{
	TInt cleanupStackCount = 0;
	switch(aResponse)
		{
		case ENetMsgRespondPrivacyRequest :
			{
			TLbsNetSessionId* pSessionId = NULL;
			CLbsNetworkProtocolBase::TLbsPrivacyResponse privacy;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &privacy);
			INFO_PRINTF2(_L("RespondPrivacyRequest(sessionId, %d)"), privacy);			
			break;
			}

		case ENetMsgRespondLocationRequest :
			{
			TLbsNetSessionId* pSessionId = NULL;			
			TInt reason = KErrNone;
			TPositionInfo* pPositionInfo = NULL;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &reason, &pPositionInfo);
			INFO_PRINTF2(_L("RespondLocationRequest(sessionId, %d, positionInfo)"), reason);
			break;
			}
			
		case ENetMsgRequestTransmitLocation :
			{
			TLbsNetSessionId* pSessionId = NULL;			
			TDesC* pDestination = NULL;
			TInt priority = 0;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &pDestination, &priority);
			INFO_PRINTF4(_L("RequestTransmitLocation(sessionId, %S, %d)"), pSessionId, pDestination, priority);
			break;
			}
			
		case ENetMsgCancelTransmitLocation :
			{
			TLbsNetSessionId* pSessionId = NULL;
			TInt reason = KErrNone;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &reason);
			INFO_PRINTF2(_L("CancelTransmitLocation(sessionId, %d)"), reason);
			break;
			}
						
		case ENetMsgRequestAssistanceData :
			{
			TLbsAssistanceDataItem dataRequestMask;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &dataRequestMask);
			INFO_PRINTF2(_L("RequestAssistanceData(%d)"), dataRequestMask);			
			break;
			}
			
		case ENetMsgRequestSelfLocation :
			{
			TLbsNetSessionId* pSessionId = NULL;				
			TLbsNetPosRequestOptionsAssistance* pOptions = NULL;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &pOptions);
			INFO_PRINTF1(_L("RequestSelfLocation()"));
			break;
			}

		case ENetMsgCancelSelfLocation :
			{
			TLbsNetSessionId* pSessionId = NULL;
			TInt reason = KErrNone;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &reason);
			INFO_PRINTF2(_L("CancelSelfLocation(sessionId, %d)"), reason);
			break;
			}
		case ENetMsgRequestNetworkLocation :
			{
			TLbsNetSessionId* pSessionId = NULL;				
			TLbsNetPosRequestOptionsAssistance* pOptions = NULL;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &pOptions);
			INFO_PRINTF1(_L("RequestNetworkLocation()"));
			break;
			}

		case ENetMsgCancelNetworkLocation :
			{
			TLbsNetSessionId* pSessionId = NULL;
			TInt reason = KErrNone;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &pSessionId, &reason);
			INFO_PRINTF2(_L("CancelNetworkLocation(sessionId, %d)"), reason);
			break;
			}
			
		case ENetMsgGetCurrentCapabilitiesResponse :	//assumed to map onto 	ENetMsgAdviceSystemStatus
			{
			CLbsNetworkProtocolBase::TLbsSystemStatus status;
			cleanupStackCount = iProxy->GetArgsLC(aResponse, &status);
			INFO_PRINTF2(_L("AdviceSystemStatus(%d)"), status);
			break;
			}
			
		default:
			break;
		}
	CleanupStack::PopAndDestroy(cleanupStackCount);
	}


TVerdict CTGetArgsStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{

		const TInt KTestTimeout = 60*1000*1000;
		TNetProtocolResponseType response;
		
		TInt expectedResponse = ENetMsgRespondPrivacyRequest;
		
		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		TEST(response == expectedResponse++);
		ProcessArgsL(response);

		SetTestStepResult(EPass);
		}
	  return TestStepResult();
	}




TVerdict CTGetArgsStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Please delete this line or modify it. I am in Test Step Postamble in Class CTGetArgsStep"));
	// process something post setting to the test step
	// uncomment the following line if you have common post setting to all the test steps in there
	// CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPostambleL();
	// uncomment the following line if you have post process or remove the following line if no post process
	// SetTestStepResult(EPass);		// or EFail
	return TestStepResult();
	}
