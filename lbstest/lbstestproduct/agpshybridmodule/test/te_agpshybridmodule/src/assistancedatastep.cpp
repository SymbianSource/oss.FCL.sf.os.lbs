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
//

/**
 @file LocationRequestStep.cpp
*/

// LBS Project
#include "lbsdevloggermacros.h"

// LBS Test
#include <lbs/test/tlbsutils.h>
#include "assistancedatastep.h"


_LIT(KFileName, "c:\\testdata\\configs\\te_agpshybridmoduleSuite.ini");
_LIT(KSection, "section0");

//************************************************************************************************************
//   CTestManagerLocReqStep
//************************************************************************************************************
CTestManagerAssistDataStep* CTestManagerAssistDataStep::NewL()
   {
	CTestManagerAssistDataStep* self = new(ELeave) CTestManagerAssistDataStep();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
   };


//------------------------------------------------------------------------------------------------------------
CTestManagerAssistDataStep::CTestManagerAssistDataStep()
	:CTestManagerBase()
	{
	CActiveScheduler::Add(this);
	}


//------------------------------------------------------------------------------------------------------------
void CTestManagerAssistDataStep::ConstructL()
	{
	CTestManagerBase::ConstructL();
	
	iAssistDataOnDone = EFalse;
	}


//------------------------------------------------------------------------------------------------------------
CTestManagerAssistDataStep::~CTestManagerAssistDataStep()
	{
	}


//------------------------------------------------------------------------------------------------------------
void CTestManagerAssistDataStep::RunL()
	{
	if (iShutdown)
		{
		CActiveScheduler::Stop();
		return;
		}

	// Setup module update data.
	if (!iInitDone)
		{
		// Now publish the file and section names to the module, to inform the module it has to
		// carry out a re-read of the ini file.
		iAGpsHandler->SendRequestUpdateInitMsg(KFileName, KSection);
		}
		
	// Switch on assistance data within the module.
	else if (!iAssistDataOnDone)
		{
		iAGpsHandler->SendRequestModuleOption(ELbsHybridModuleOptions_AssistanceDataOn, ETrue);
		}
		
	else
		{
		// Test Actions
		// Single request with new gps options. Options of measurent first position second.
		if (iRequestCounter == 0)
			// Start using a real time from now on
			{
			// Set gps options.
			TLbsGpsOptionsItem optionsItem;
			
			iGpsOptions.SetGpsMode(CLbsAdmin::EGpsPreferTerminalBased);
			iGpsOptions.ClearOptionItems();

			optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateCalculation);
			iGpsOptions.AppendOptionItem(optionsItem);

			optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateMeasurement);
			iGpsOptions.AppendOptionItem(optionsItem);

			iModule->SetGpsOptions(iGpsOptions);
			
			// Since Assistance is On and we are in PreferTerminal mode
			// set this flag to let the test manager know we are not interested in the initial partial update.
			iExpectNanPosition = ETrue;
			}

		// Stop
		else if (iRequestCounter == 1)
			{
			// Check previous update.
			CheckLocationUpdateReceived();
			
			// Shutdown module which invokes test end.
			iModule->AdvisePowerMode(CLbsLocationSourceGpsBase::EPowerModeClose);
			return;
			}
		else
			{
			User::Leave(KErrGeneral);
			}

		RequestLocation();
		}
	}


//------------------------------------------------------------------------------------------------------------
void CTestManagerAssistDataStep::ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse)
	{
	if (!iInitDone)
		{
		iInitDone = ETrue;
		}
	else
		{
		iAssistDataOnDone = ETrue;
		}
		
	CTestManagerBase::ProcessAGpsResponseMessage(aResponse);	
	}



//************************************************************************************************************
// CAssistanceDataStep
//************************************************************************************************************
CAssistanceDataStep::~CAssistanceDataStep()
/**
 * Destructor
 */
	{
	}

CAssistanceDataStep::CAssistanceDataStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KAssistanceDataStep);
	}

TVerdict CAssistanceDataStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Test Preamble. Creating the Main Logic"));
	LBSNEWLOG(">>>>>>>>>>>>>>>>> A-GPS Hybrid Module Main Logic Assistance Data Unit Test Started >>>>>>>>>>>>>>>>>");

	__UHEAP_MARK;

	iTestManager = CTestManagerAssistDataStep::NewL();
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CAssistanceDataStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		// Carry out the test.
		CActiveScheduler::Start();

		SetTestStepResult(EPass);
		}
	  return TestStepResult();
	}

TVerdict CAssistanceDataStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	LBSLOG(ELogP5, "<<<<<<<<<<<<<<<<< A-GPS Hybrid Module Assistance Data Unit Test Terminating <<<<<<<<<<<<<<<<<");
	INFO_PRINTF1(_L("Test Postamble. Deleting the module"));

	delete iTestManager;
	iTestManager = NULL;
	
	__UHEAP_MARKEND;
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

