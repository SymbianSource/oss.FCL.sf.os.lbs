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

#include "lbsdevloggermacros.h"

#include <lbs/test/tlbsutils.h>
#include "te_npetestclientapilocreqstep.h"
#include "LbsExtendModuleInfo.h"
//#include "lbsnetinternalapi.h"
#include "LbsInternalInterface.h"


_LIT(KFileName, "c:\\testdata\\configs\\te_npetestclientapi.ini");
_LIT(KSection, "section0");


//************************************************************************************************************
//   CTe_LocReqTestNpiModule
//************************************************************************************************************
CTe_LocReqTestNpiModule* CTe_LocReqTestNpiModule::NewL()
   {
	CTe_LocReqTestNpiModule* self = new(ELeave) CTe_LocReqTestNpiModule();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
   };


//------------------------------------------------------------------------------------------------------------
CTe_LocReqTestNpiModule::CTe_LocReqTestNpiModule()
	:CTe_TestNpeIntegModule()
	{
   iAssistDataOnDone = EFalse;

	CActiveScheduler::Add(this);
	}


//------------------------------------------------------------------------------------------------------------
void CTe_LocReqTestNpiModule::ConstructL()
	{
	CTe_TestNpeIntegModule::ConstructL();
	}


//------------------------------------------------------------------------------------------------------------
CTe_LocReqTestNpiModule::~CTe_LocReqTestNpiModule()
	{
	}


//------------------------------------------------------------------------------------------------------------
void CTe_LocReqTestNpiModule::RunL()
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
		switch(iRequestCounter)
		    {
		    case 0:
		        {
	            iGpsMode = EAutonomous;
                break;
		        }
            case 1:
                {
                // Check previous update.
                CheckLocationUpdateReceived();
                iGpsMode = ETerminalAssisted;
                break;
                }
            case 2:
                {
                // Check previous update.
                CheckLocationUpdateReceived();
                iGpsMode = ETerminalBased;
                break;
                }
            case 3:
                {
                // Check previous update.
                CheckLocationUpdateReceived();
                iGpsMode = EAutonomousAndTerminalAssisted;
                break;
                }
            case 4:
                {
                // Check previous update.
                CheckLocationUpdateReceived();
                iGpsMode = ETerminalBasedAndAssisted;
                break;
                }
            case 5:
                {
                if (!iAssistDataOnDone)
                    {
                    iAGpsHandler->SendRequestModuleOption(ELbsHybridModuleOptions_AssistanceDataOn, ETrue);
                    return;
                    }
                iGpsMode = ETerminalBased;
                
                // Since Assistance is On and we are in PreferTerminal mode
                // set this flag to let the test manager know we are not interested in the initial partial update.
                iExpectNanPosition = ETrue;
                break;
                }
            case 6:
            case 7:
            case 8:
                {
                // Switch on multi-update module option.
                if (!iMultiUpdateOnDone)
                    {
                    iAGpsHandler->SendRequestModuleOption(ELbsHybridModuleOptions_MultiUpdatesOn, ETrue);
                    return;
                    }
                    
                // First request.
                if (iRequestCounter == 6)
                    {
                    // now do update...
                    
                    // then we get 3 results, as a result fo the request at the bottom, ensure we
                    // don't keep re-requesting...

                    // Check previous update.
                    CheckLocationUpdateReceived();
                    
                    // Set gps options. 
                    iGpsMode = EAutonomous;
                    
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
                break;
                }
            case 9:
                {
                // Check previous update.
                CheckLocationUpdateReceived();
                
                // Shutdown which invokes test end.
                Shutdown();
                return;
                }
            default:
                {
                User::Leave(KErrGeneral);
                }
		    }
		
        iModule->SetGpsOptions(iGpsMode, ETrue);
		RequestLocation();
		}
	}

void CTe_LocReqTestNpiModule::ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse)
	{
	if (!iInitDone)
		{
		iInitDone = ETrue;
		}
	else
		{
		if(iRequestCounter == 5)
		    {
		    iAssistDataOnDone  = ETrue;
		    }
		else if(iRequestCounter == 6)
            {
            iMultiUpdateOnDone = ETrue;
            }
		else
		    {
		    //Some Error???
		    }
		}

	CTe_TestNpeIntegModule::ProcessAGpsResponseMessage(aResponse);	
	}	
	
	
//************************************************************************************************************
// CLocationRequestStep
//************************************************************************************************************
CTe_NpeTestClientApiLocReqStep::~CTe_NpeTestClientApiLocReqStep()
/**
 * Destructor
 */
	{
	}

CTe_NpeTestClientApiLocReqStep::CTe_NpeTestClientApiLocReqStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KNpeTestClientApiLocReqStep);
	}

TVerdict CTe_NpeTestClientApiLocReqStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Test Preamble. Creating the Main Logic"));
	LBSNEWLOG(">>>>>>>>>>>>>>>>> A-GPS Hybrid Module Main Unit Test Started >>>>>>>>>>>>>>>>>");

	__UHEAP_MARK;
	
	INFO_PRINTF1(_L("Test Preamble. Creating test manager"));
	iTestManager = CTe_LocReqTestNpiModule::NewL();
	
	TPositionModuleInfoExtended moduleInfo1;
		
	const TUid KModuleUid1 = {0x10281d44};
		
	TPositionModuleInfo* posModInfo;
	posModInfo = 	(TPositionModuleInfo*)(&moduleInfo1.iPositionModuleInfoBytes[0]);
	posModInfo->SetModuleId(TPositionModuleId(KModuleUid1));
	moduleInfo1.iModuleId = KModuleUid1;
		
	const TPositionModuleInfoExtended* KModuleInfoArray[] =
		{
		&moduleInfo1
		};
	
	INFO_PRINTF1(_L("Test Preamble. LbsModuleInfo::InitializeL"));
	LbsModuleInfo::InitializeL(KModuleInfoArray, 1);
	INFO_PRINTF1(_L("Test Preamble. LbsModuleInfo::Initialized"));
	
	TUint numModules = 0;
		
	TInt err = LbsModuleInfo::GetNumModules(numModules);
	INFO_PRINTF3(_L("numModules: %d, err: %d"), numModules, err);
	
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTe_NpeTestClientApiLocReqStep::doTestStepL()
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

TVerdict CTe_NpeTestClientApiLocReqStep::doTestStepPostambleL()
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

