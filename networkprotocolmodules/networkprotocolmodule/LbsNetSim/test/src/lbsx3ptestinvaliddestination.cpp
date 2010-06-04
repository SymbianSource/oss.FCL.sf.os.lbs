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
 @file Lbsx3pTest.cpp
*/
#include "lbsx3ptestinvaliddestination.h"
#include "Te_lbsnetsimtestSuiteDefs.h"


_LIT(KX3PDestination, "I AM INVALID");


CLbsX3PTestInvalidDestination::~CLbsX3PTestInvalidDestination()
/**
 * Destructor
 */
	{
	}

CLbsX3PTestInvalidDestination::CLbsX3PTestInvalidDestination()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KX3PTestInvalidDestination);
	}

TVerdict CLbsX3PTestInvalidDestination::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	__UHEAP_MARK;
	
	// Get the delay
	GetIntFromConfig(ConfigSection(), KDelay, iDelay);
	// Assistance data provider
	TInt provider;
	GetIntFromConfig(ConfigSection(), KProvider, provider);
	iProvider = TUid::Uid(provider);
	// Step mode?
	TBool stepMode = EFalse;
	GetBoolFromConfig(ConfigSection(), KStepMode, stepMode);
	
	iState = EStart;
	iTest = new (ELeave) CAOTest(this, iDelay, stepMode);  // Stopped in postamble
	iGateway = new (ELeave) CAOGateway(this); // Stopped during test
	iState = EGatewayRegisterLcsMoLr;
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CLbsX3PTestInvalidDestination::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	
	if (TestStepResult()==EPass)
		{
		iGateway->Activate();
		iAs->Start();

		if (iTest->Finished() && iGateway->Finished() && (State() == EFinished))
			{
			SetTestStepResult(EPass);
			}
		else
			{
			SetTestStepResult(EFail);
			}
		}
		
	  return TestStepResult();
	}



TVerdict CLbsX3PTestInvalidDestination::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iTest;
	delete iGateway;
	
	__UHEAP_MARKEND;
	
	return TestStepResult();
	}

//
// CAOGateway
CLbsX3PTestInvalidDestination::CAOGateway::CAOGateway(CLbsX3PTestInvalidDestination* aBase) :
	CAOGateway_Base(aBase), iDestination(KX3PDestination), iMeasurementControlLocationError(EFalse)
	{
	iLbsNetSim = new(ELeave) RLbsNetSim();
	iLbsNetSim->ConnectL(this);
	}
	
CLbsX3PTestInvalidDestination::CAOGateway::~CAOGateway()
	{
	Cancel();	
	
	iLbsNetSim->Close();
	delete iLbsNetSim;
	}
	
void CLbsX3PTestInvalidDestination::CAOGateway::RunL()
	{
	switch (iBase->State())
		{
		case EError:
			{
			ASSERT(EFalse);
			}
		case EGatewayRegisterLcsMoLr:
			{
			iLbsNetSim->RegisterLcsMoLrL(iDestination);
			iBase->SetState(EError);
			break;
			}
		case EGatewayReleaseLcsMoLr:
			{
			iLbsNetSim->ReleaseLcsMoLrL(KErrNone);
			iBase->SetState(EError);
			break;
			}
		case EFinished:
			{
			CActiveScheduler::Stop();
			break;
			}
			
		} // switch
	}
	
void CLbsX3PTestInvalidDestination::CAOGateway::DoCancel()
	{
	}
	
TBool CLbsX3PTestInvalidDestination::CAOGateway::Finished()
	{
	return (iMeasurementControlLocationError);
	}
	
void CLbsX3PTestInvalidDestination::CAOGateway::ProcessMeasurementControlLocationError(TInt)
	{
	iMeasurementControlLocationError = ETrue;
	
	iBase->SetState(EGatewayReleaseLcsMoLr);
	Activate();
	}

//
// CAOTest

CLbsX3PTestInvalidDestination::CAOTest::CAOTest(CLbsX3PTestInvalidDestination* aBase, TInt aDelay, TBool aStepMode) :
	CAOTest_Base(aBase, aDelay, aStepMode), iNotificationReleaseLcsMoLr(EFalse)
	{
	iLbsNetSimTest = new(ELeave) RLbsNetSimTest();
	iLbsNetSimTest->ConnectL(this);
	if (aDelay > 0)
		{
		iLbsNetSimTest->SetResponseTime(aDelay, ETrue);
		}
	iLbsNetSimTest->SetAssistanceDataProvider(aBase->Provider());
	iLbsNetSimTest->SetStepMode(iStepMode);
	}
	
CLbsX3PTestInvalidDestination::CAOTest::~CAOTest()
	{
	Cancel();	

	iLbsNetSimTest->Close();
	delete iLbsNetSimTest;
	}
	
void CLbsX3PTestInvalidDestination::CAOTest::RunL()
	{
	switch (iBase->State())
		{
		} // switch
		
	}
	
void CLbsX3PTestInvalidDestination::CAOTest::DoCancel()
	{
	}
	
TBool CLbsX3PTestInvalidDestination::CAOTest::Finished()
	{
	return (iNotificationReleaseLcsMoLr);
	}
	
void CLbsX3PTestInvalidDestination::CAOTest::NotifyReleaseLcsMoLr(TInt)
	{
	iNotificationReleaseLcsMoLr = ETrue;
	
	iBase->SetState(EFinished);
	iBase->ActivateGateway();
	if (iStepMode)
		{
		iLbsNetSimTest->Next(KErrNone);
		}
	}
