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
 @file suplrrlp0Step.cpp
 @internalTechnology
*/
#include "suplrrlp0Step.h"
#include "Te_suplrrlpSuiteDefs.h"

Csuplrrlp0Step::~Csuplrrlp0Step()
/**
 * Destructor
 */
	{
	}

Csuplrrlp0Step::Csuplrrlp0Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ksuplrrlp0Step);
	}

TVerdict Csuplrrlp0Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSched = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iSched);
	
	iRrlpTest = CRrlpTest::NewL();
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Csuplrrlp0Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		/* 
		 * Test 1: 
		 */
		const TInt cntTest1 = 8;
		TInt seqTest1[cntTest1] = {CRrlpTest::ESendAssistanceDataFinal, CRrlpTest::EWaitingAssistanceDataAck,
					  	   		   CRrlpTest::EWaitingProcessAssistanceData, CRrlpTest::ESendMeasurementRequest, 
					  	   		   CRrlpTest::EWaitingProcessPositionRequest, CRrlpTest::ESendLocationResponse, 
					  	   		   CRrlpTest::EWaitingLocationResponse, CRrlpTest::EFinished};
		
		/*
		 * Test 2:
		 */
		const TInt cntTest2 = 10;
		TInt seqTest2[cntTest2] = {CRrlpTest::ESendAssistanceData, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::ESendAssistanceDataFinal, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::EWaitingProcessAssistanceData, CRrlpTest::ESendMeasurementRequest, 
								   CRrlpTest::EWaitingProcessPositionRequest, CRrlpTest::ESendLocationResponse, 
								   CRrlpTest::EWaitingLocationResponse, CRrlpTest::EFinished};
		
		/*
		 * Test 3:
		 */
		const TInt cntTest3 = 10;
		TInt seqTest3[cntTest3] = {CRrlpTest::ESendAssistanceData, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::ESendAssistanceData, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::ESendMeasurementRequestWithAssistanceData, CRrlpTest::EWaitingProcessAssistanceData, 
								   CRrlpTest::EWaitingProcessPositionRequest, CRrlpTest::ESendLocationResponse, 
								   CRrlpTest::EWaitingLocationResponse, CRrlpTest::EFinished};

		/*
		 * Test 4:
		 */
		const TInt cntTest4 = 6;
		TInt seqTest4[cntTest4] = {CRrlpTest::ESendMeasurementRequestWithAssistanceData, CRrlpTest::EWaitingProcessAssistanceData, 
								   CRrlpTest::EWaitingProcessPositionRequest, CRrlpTest::ESendLocationResponse, 
								   CRrlpTest::EWaitingLocationResponse, CRrlpTest::EFinished};

		/*
		 * Test 5:
		 */
		const TInt cntTest5 = 9;
		TInt seqTest5[cntTest5] = {CRrlpTest::ESendAssistanceData, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::ESendAssistanceData, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::ESendMeasurementRequestWithAssistanceData, CRrlpTest::EWaitingProcessAssistanceData, 
								   CRrlpTest::ESendProtocolError, CRrlpTest::EWaitingProtocolError, 
								   CRrlpTest::EFinished};

		/*
		 * Test 6:
		 */
		const TInt cntTest6 = 6;
		TInt seqTest6[cntTest6] = {CRrlpTest::ESendAssistanceData, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::EDoTimeDelay, 65, CRrlpTest::EWaitingProtocolError, CRrlpTest::EFinished};

		/*
		 * Test 7:
		 */
		const TInt cntTest7 = 9;
		TInt seqTest7[cntTest7] = {CRrlpTest::ESendAssistanceData, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::ESendAssistanceDataFinal, CRrlpTest::EWaitingAssistanceDataAck,
								   CRrlpTest::EWaitingProcessAssistanceData,  CRrlpTest::EDoTimeDelay, 65, 
								   CRrlpTest::EWaitingProtocolError, CRrlpTest::EFinished};	
		
		/**
		 *  Test 8:
		 */
		const TInt cntTest8 = 9;
		TInt seqTest8[cntTest8] = {CRrlpTest::ESendAssistanceDataFinal, CRrlpTest::EWaitingAssistanceDataAck,
					  	   		   CRrlpTest::EWaitingProcessAssistanceData, CRrlpTest::ESendMeasurementRequest, 
					  	   		   CRrlpTest::EWaitingProcessPositionRequest, CRrlpTest::EDoTimeDelay, 90, 
								   CRrlpTest::EWaitingProtocolError, CRrlpTest::EFinished};	
		
		// Run test 1 
		iRrlpTest->SetSequence(seqTest1, cntTest1);
		iRrlpTest->GoL();
		CActiveScheduler::Start();
		
		// Run test 2
		iRrlpTest->SetSequence(seqTest2, cntTest2);
		iRrlpTest->GoL();
		CActiveScheduler::Start();

		// Run test 3
		iRrlpTest->SetSequence(seqTest3, cntTest3);
		iRrlpTest->GoL();
		CActiveScheduler::Start();

		// Run test 4
		iRrlpTest->SetSequence(seqTest4, cntTest4);
		iRrlpTest->GoL();
		CActiveScheduler::Start();

		// Run test 5
		iRrlpTest->SetSequence(seqTest5, cntTest5);
		iRrlpTest->GoL();
		CActiveScheduler::Start();

		// Run test 6
		iRrlpTest->SetSequence(seqTest6, cntTest6);
		iRrlpTest->GoL();
		CActiveScheduler::Start();

		// Run test 7
		iRrlpTest->SetSequence(seqTest7, cntTest7);
		iRrlpTest->GoL();
		CActiveScheduler::Start();

		// Run test 8
		iRrlpTest->SetSequence(seqTest8, cntTest8);
		iRrlpTest->GoL();
		CActiveScheduler::Start();

		SetTestStepResult(EPass);
		}
	  return TestStepResult();
	}

TVerdict Csuplrrlp0Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSched;
	delete iRrlpTest;
	
	return TestStepResult();
	}
