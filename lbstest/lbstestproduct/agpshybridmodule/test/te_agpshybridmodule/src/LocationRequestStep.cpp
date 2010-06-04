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

// Project
#include "lbsdevloggermacros.h"

// LBS Test
#include <lbs/test/tlbsutils.h>
#include "LocationRequestStep.h"


_LIT(KFileName, "c:\\testdata\\configs\\te_agpshybridmoduleSuite.ini");
_LIT(KSection, "section0");


//************************************************************************************************************
//   CTestManagerLocReqStep
//************************************************************************************************************
CTestManagerLocReqStep* CTestManagerLocReqStep::NewL()
   {
	CTestManagerLocReqStep* self = new(ELeave) CTestManagerLocReqStep();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
   };


//------------------------------------------------------------------------------------------------------------
CTestManagerLocReqStep::CTestManagerLocReqStep()
	:CTestManagerBase()
	{
	CActiveScheduler::Add(this);
	}


//------------------------------------------------------------------------------------------------------------
void CTestManagerLocReqStep::ConstructL()
	{
	CTestManagerBase::ConstructL();
	}


//------------------------------------------------------------------------------------------------------------
CTestManagerLocReqStep::~CTestManagerLocReqStep()
	{
	}


//------------------------------------------------------------------------------------------------------------
void CTestManagerLocReqStep::RunL()
	{
	if (iShutdown)
		{
		CActiveScheduler::Stop();
		return;
		}

	if (!iInitDone)
		{
		// Now publish the file and section names to the module, to inform the module it has to
		// carry out a re-read of the ini file.
		iAGpsHandler->SendRequestUpdateInitMsg(KFileName, KSection);		
		}
	
	else
		{
		// Test Actions
		// Single request with normal gps options.
		if (iRequestCounter == 0)
			// Start using a real time from now on
			{
			// Set gps options.
			TLbsGpsOptions gpsOptions;
			
			iModule->SetGpsOptions(gpsOptions); // Note it's important we pass the old version (TLbsGpsOptions) of the options for this test
			
			// Also setup the test gps options, for verification.
			iGpsOptions.SetGpsMode(CLbsAdmin::EGpsAutonomous);
			iGpsOptions.ClearOptionItems();
			}

		// Single request with new gps options. Options of position first, measurement second.
		else if (iRequestCounter == 1)
			{
			// Check previous update.
			CheckLocationUpdateReceived();
			
			// Set gps options. 
			TLbsGpsOptionsItem optionsItem;

			iGpsOptions.SetGpsMode(CLbsAdmin::EGpsAutonomous);
			iGpsOptions.ClearOptionItems();

			optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateCalculation);
			iGpsOptions.AppendOptionItem(optionsItem);

			optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateMeasurement);
			iGpsOptions.AppendOptionItem(optionsItem);

			iModule->SetGpsOptions(iGpsOptions);
			}

		// Single request with new gps options. Options of measurent first position second.
		else if (iRequestCounter == 2)
			{
			// Check previous update.
			CheckLocationUpdateReceived();
			
			// Set gps options. 
			TLbsGpsOptionsItem optionsItem;

			iGpsOptions.SetGpsMode(CLbsAdmin::EGpsAutonomous);
			iGpsOptions.ClearOptionItems();

			optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateMeasurement);
			iGpsOptions.AppendOptionItem(optionsItem);

			optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateCalculation);
			iGpsOptions.AppendOptionItem(optionsItem);

			iModule->SetGpsOptions(iGpsOptions);
			}


		// Multi-update test starts here.
		else if ((iRequestCounter >= 3) && (iRequestCounter <= 5))
			{
			// Switch on multi-update module option.
			if (!iMultiUpdateOnDone)
				{
				iAGpsHandler->SendRequestModuleOption(ELbsHybridModuleOptions_MultiUpdatesOn, ETrue);
				
				return;
				}
				
			// First request.
			else if (iRequestCounter == 3)
				{
				// now do update...
				
				// then we get 3 results, as a result fo the request at the bottom, ensure we
				// don't keep re-requesting...

				// Check previous update.
				CheckLocationUpdateReceived();
				
				// Set gps options. 
				TLbsGpsOptionsItem optionsItem;

				iGpsOptions.SetGpsMode(CLbsAdmin::EGpsAutonomous);
				iGpsOptions.ClearOptionItems();

				optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateCalculation);
				iGpsOptions.AppendOptionItem(optionsItem);

				optionsItem.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateMeasurement);
				iGpsOptions.AppendOptionItem(optionsItem);

				iModule->SetGpsOptions(iGpsOptions);
				
				// Set quality.
				TTimeIntervalMicroSeconds maxFixTime(10000000); // 10 secs.
				TLbsLocRequestQuality quality;

				iQuality.SetMaxFixTime(maxFixTime);
				}
				
			// Remaining requests.
			else
				{
				// Check previous update.
				CheckLocationUpdateReceived();

				// Dummy request.
				iRequestCounter++;
				
				return;
				}
			}

		// Stop
		else if (iRequestCounter == 6)
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

void CTestManagerLocReqStep::ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse)
	{
	if (!iInitDone)
		{
		iInitDone = ETrue;
		}
	else
		{
		iMultiUpdateOnDone = ETrue;
		}

	CTestManagerBase::ProcessAGpsResponseMessage(aResponse);	
	}	
	
	
//************************************************************************************************************
// CLocationRequestStep
//************************************************************************************************************
CLocationRequestStep::~CLocationRequestStep()
/**
 * Destructor
 */
	{
	}

CLocationRequestStep::CLocationRequestStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KLocationRequestStep);
	}

TVerdict CLocationRequestStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Test Preamble. Creating the Main Logic"));
	LBSNEWLOG(">>>>>>>>>>>>>>>>> A-GPS Hybrid Module Main Unit Test Started >>>>>>>>>>>>>>>>>");

	__UHEAP_MARK;

	iTestManager = CTestManagerLocReqStep::NewL();
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CLocationRequestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		CActiveScheduler::Start();

		SetTestStepResult(EPass);
		}
	  return TestStepResult();
	}

TVerdict CLocationRequestStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	LBSLOG(ELogP5, "<<<<<<<<<<<<<<<<< A-GPS Hybrid Module Unit Test Terminating <<<<<<<<<<<<<<<<<");
	INFO_PRINTF1(_L("Test Postamble. Deleting the module"));

	delete iTestManager;
	iTestManager = NULL;
	
	__UHEAP_MARKEND;
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

