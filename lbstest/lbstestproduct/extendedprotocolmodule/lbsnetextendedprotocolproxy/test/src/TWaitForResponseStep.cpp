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
 @file TWaitForResponseStep.cpp
 @internalTechnology
*/
#include "TWaitForResponseStep.h"
#include "Te_LbsNetProtocolProxyUnitTestSuiteDefs.h"

CTWaitForResponseStep::~CTWaitForResponseStep()
/**
 * Destructor
 */
	{
	}

CTWaitForResponseStep::CTWaitForResponseStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTWaitForResponseStep);
	}

TVerdict CTWaitForResponseStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Please delete this line or modify it. I am in Test Step Preamble in Class CTWaitForResponseStep"));
	CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPreambleL();
	if (TestStepResult()!=EPass)
	   return   TestStepResult();
	// process some pre setting to this test step then set SetTestStepResult to EFail or Epass.
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CTWaitForResponseStep::doTestStepL()
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
		
		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);
		
		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		response = iProxy->WaitForResponse(KTestTimeout);
		INFO_PRINTF2(_L("Response %d"), response);

		SetTestStepResult(EPass);
		}
	  return TestStepResult();
	}



TVerdict CTWaitForResponseStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Please delete this line or modify it. I am in Test Step Postamble in Class CTWaitForResponseStep"));
	// process something post setting to the test step
	// uncomment the following line if you have common post setting to all the test steps in there
	// CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPostambleL();
	// uncomment the following line if you have post process or remove the following line if no post process
	// SetTestStepResult(EPass);		// or EFail
	return TestStepResult();
	}
