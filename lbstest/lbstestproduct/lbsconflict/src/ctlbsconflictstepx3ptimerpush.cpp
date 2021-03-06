// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file ctlbsassdatastepmain.cpp
// This is the class implementation for the Module Information Tests
// EPOC includes.
// 
//

// LBS includes. 
#include <lbs.h>
#include <lbs/lbsnetcommon.h>
#include <lbssatellite.h>
#include <lbs/lbsnetprotocolbase.h>
#include <lbs/lbsassistancedatabuilderset.h>
#include <e32math.h>

// LBS test includes.
#include "ctlbsconflictstepx3ptimerpush.h"
#include <lbs/test/tlbsutils.h>


/**	Test case development notes
	===========================

	It was decided to aid readablity each test class shall only support a single test case.
	This example has been produced to show how a typical conflict test case should be coded.


	Net Sim and Sequence Events
	---------------------------
	Every test case implementation will use the ctlbsconflictstep class which supports Net Sim
	interactions and a way to ensure Net Sim events and client completion events have a occured
	in the correct order. An array is used to hold the expected sequence event values, and a second
	array contains the actual events produced during the test. Once the test moves into the EWaiting
	state typically after all the client requests have been made, the event arrays are compared to
	ensure the correct events were given.

	To set the expected sequence events populate the array in the SetExpectedSeq() function.
	

	Async wappers
	-------------
	Each Lbs client async request is wappared in a active object class, to allow a number of
	outstanding async requests during the test. Any async calls you wish to make should also
	be wrapped in the same way, see CT_LbsDoX3P and CT_LbsDoPosUpdate for examples.
	These classes are constructed in the test class ConstructL, and maybe started or cancelled
	during the test at any point.
	

	Verify Position Information
	---------------------------
	A test should verify each client call which expected a location to be returned. The
	location data should be validation to ensure it's correct. See VerifyPosInfos.
*/


/**
Static Constructor
*/

CT_LbsConflictStep_X3PTimerPush* CT_LbsConflictStep_X3PTimerPush::New(CT_LbsConflictServer& aParent)
	{
	// Note the lack of ELeave.
	// This means that having insufficient memory will return NULL;
	CT_LbsConflictStep_X3PTimerPush* testStep = new CT_LbsConflictStep_X3PTimerPush(aParent);
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
CT_LbsConflictStep_X3PTimerPush::CT_LbsConflictStep_X3PTimerPush(CT_LbsConflictServer& aParent) : CT_LbsConflictStep(aParent)
	{
	SetTestStepName(KLbsConflictStep_X3PTimerPush);
	}


void CT_LbsConflictStep_X3PTimerPush::ConstructL()
	{
	// Create the base class objects.
	CT_LbsConflictStep::ConstructL();
	
	// X3P async wrappers.
	iDoX3PTimer = CT_LbsDoX3P::NewL(this, KLbsConflictX3PTimerId);	
	iDoX3PPush = CT_LbsDoX3P::NewL(this, KLbsConflictX3PPushId);
	}


/**
 * Destructor
 */
CT_LbsConflictStep_X3PTimerPush::~CT_LbsConflictStep_X3PTimerPush()
	{
	iDoX3PTimer->Cancel();
	delete iDoX3PTimer;

	iDoX3PPush->Cancel();
	delete iDoX3PPush;
	}


/** Expected sequence events.
*/
void CT_LbsConflictStep_X3PTimerPush::SetExpectedSeq()
	{
	// X3P Timer (iState = EReqX3pTimer)
	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);			// --> To Net
//	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementControlLocation);	// <-- From Net

	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);				// --> To Net
	
		// followed by a X3P Push (after X3P Push register iState = ERegX3PPush),
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);				// --> To Net	
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementControlLocation);	// <-- From Net
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementReportLocation);	// --> To Net
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyFacilityLcsMoLrResult);		// <-- From Net
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);				// --> To Net

	// followed by the queued X3P Timer,
	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);				// --> To Net	
	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementControlLocation);	// <-- From Net
	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementReportLocation);	// --> To Net
	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyFacilityLcsMoLrResult);		// <-- From Net
	SetVerifySeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);				// --> To Net

	// (after X3P Push release iState = EWaiting).

	SetVerifyAdhocEvent(EClient_Got_X3P_Complete);
	SetVerifyAdhocEvent(EClient_Got_X3P_Complete);
}



/** Called at the end of the test to verify the correct position data has been returned to the
	client.
	
	Each test case SHOULD implement a version of this.
*/
void CT_LbsConflictStep_X3PTimerPush::VerifyPosInfos()
	{
	T_LbsUtils utils;
    RPointerArray<TAny>& verifyPosInfoArr = iParent.iSharedData->iVerifyPosInfoArr;
	RPointerArray<TAny>& currPosInfoArr = iParent.iSharedData->iCurrentPosInfoArr;
	TPositionInfo* currPosInfo;
	
	// Verify both X3P position information.
	
	// Verify entry 0 for the X3P TIMER. 
	// We expect a real location value, compare using the data
	// sent to the test APGS module.
	TPositionInfo* verifyPosInfo = reinterpret_cast<TPositionInfo*>(verifyPosInfoArr[0]);
	currPosInfo = reinterpret_cast<TPositionInfo*>(currPosInfoArr[0]);
	if (!utils.Compare_PosInfo(*verifyPosInfo, *currPosInfo))
		{
		INFO_PRINTF1(_L("Failed test, X3P position incorrect."));
		SetTestStepResult(EFail);
		}
	// Verify entry 1 for the X3P PUSH. 
	// We expect a real location value, compare using the data
	// sent to the test APGS module.
	verifyPosInfo = reinterpret_cast<TPositionInfo*>(verifyPosInfoArr[0]);
	currPosInfo = reinterpret_cast<TPositionInfo*>(currPosInfoArr[1]);
	if (!utils.Compare_PosInfo(*verifyPosInfo, *currPosInfo))
		{
		INFO_PRINTF1(_L("Failed test, X3P position incorrect."));
		SetTestStepResult(EFail);
		}
	}


/** Helper function to start X3Ps.
*/
void CT_LbsConflictStep_X3PTimerPush::StartX3PL(TInt aTransmitId)
	{
	// Create a posinfo and store in our shared array for later verification.
	RPointerArray<TAny>& posInfoArr = iParent.iSharedData->iCurrentPosInfoArr;
	TPositionInfo* posInfo = new(ELeave) TPositionInfo();

	//T_LbsUtils utils;
	// utils.ResetAndDestroy_PosInfoArr(posInfoArr);	// Clear previous entries before new entry is appended.

	posInfoArr.Append(posInfo);


	// Start X3P,
	TLbsTransmitPositionOptions transOpts;	// Set timeout value to 0 (done by the default constructor), to disable the timeout.

	// Timer.	
	if (aTransmitId == KLbsConflictX3PTimerId)
		{	
		iDoX3PTimer->SetOptions(transOpts);
		iDoX3PTimer->StartL(KTransmitTimerDestination, KTransmitTimerPriority, *posInfo);
		}
		
	// Push.
	else if (aTransmitId == KLbsConflictX3PPushId)
		{
		iDoX3PPush->SetOptions(transOpts);
		iDoX3PPush->StartL(KTransmitPushDestination, KTransmitPushPriority, *posInfo);
		}
		
	else
		{
		User::LeaveIfError(KErrNotSupported);
		}
	}


/** Run the test.
 */
TVerdict CT_LbsConflictStep_X3PTimerPush::doTestStepL()
	{
	INFO_PRINTF1(_L("&gt;&gt;CT_LbsConflictStep_X3PTimerPush::doTestStepL()"));

	if (TestStepResult() == EPass)
		{		
		// Setup the expected sequence events for the test.
		SetExpectedSeq();
				
		// Open and setup net sim.
		OpenNetSim(this);

		// Kick off the test abort and keep alive timers.
		TTimeIntervalMicroSeconds32 abortInterval(KLbsAbortPeriod);
		TTimeIntervalMicroSeconds32 keepAliveInterval(KLbsKeepAlivePeriod);

		iAbortTimer->SetTimer(abortInterval);
		iKeepAliveTimer->SetTimer(keepAliveInterval);

		// Kick off test.
		CActiveScheduler::Start();

		// Verify location data.
		VerifyPosInfos();

		// Clean up.
		CloseNetSim();
		}

	INFO_PRINTF1(_L("&lt;&lt;CT_LbsConflictStep_X3PTimerPush::doTestStepL()"));

	return TestStepResult();
	}


/** NetSim callbacks given for a MoLr, which we invoke as a result of the notify position update.
*/
void CT_LbsConflictStep_X3PTimerPush::Connected()
	{
	// Call base implementation.
	CT_LbsConflictStep::Connected();
	
	RPointerArray<TAny>& posInfoArr = iParent.iSharedData->iCurrentPosInfoArr;

	T_LbsUtils utils;
	utils.ResetAndDestroy_PosInfoArr(posInfoArr);	// Clear previous entries before new entry is appended.

	// Kick off first X3P.
	StartX3PL(KLbsConflictX3PTimerId);
	iState = EReqX3PTimer;
	}


void CT_LbsConflictStep_X3PTimerPush::Disconnected()
	{
	// Call base implementation.
	CT_LbsConflictStep::Disconnected();
	}


void CT_LbsConflictStep_X3PTimerPush::NotifyRegisterLcsMoLr(const TDesC& aData)
	{
	// Determine and record the sequence event. A blank aData indicates a self locate
	// un-expected MOLR, otherwise we have a X3P MOLR.
	if (aData != KNullDesC)
		{
		// Process X3P Timer register.
		if (iState == EReqX3PTimer)
			{
			INFO_PRINTF1(_L("Got - X3P(TIMER1) MOLR - NetSim Notify Register Lcs MoLr - Event."));
			SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);

			// Verify telephone number.
			if (aData.Compare(KTransmitTimerDestination))
				{
				INFO_PRINTF1(_L("Failed test, bad X3P Timer register data."));
				SetTestStepResult(EFail);
				}

			// Start X3P Push (second X3P).
			StartX3PL(KLbsConflictX3PPushId);
			}

		// Process X3P Push register.			
		else
		if (iState == EReqX3PTimer2)
			{
			INFO_PRINTF1(_L("Got - X3P(TIMER2) MOLR - NetSim Notify Register Lcs MoLr - Event."));
			SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);
			if (aData.Compare(KTransmitTimerDestination))
				{
				INFO_PRINTF1(_L("Failed test, bad X3P Timer register data."));
				SetTestStepResult(EFail);
				}
			}
		else
			{
			INFO_PRINTF1(_L("Got - X3P(PUSH) MOLR - NetSim Notify Register Lcs MoLr - Event."));
			SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);

			// Verify telephone number.
			if (aData.Compare(KTransmitPushDestination))
				{
				INFO_PRINTF1(_L("Failed test, bad X3P Push register data."));
				SetTestStepResult(EFail);
				}
			
			// Indicate we have really started the second X3P.
//			iState = EReqX3PPush;			
			}		
		}

	else
		{
		INFO_PRINTF1(_L("Got unexpected - Self MOLR - NetSim Notify Register Lcs MoLr - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);
		}
	}


void CT_LbsConflictStep_X3PTimerPush::NotifyReleaseLcsMoLr(TInt aReason)
	{
	TInt expectedErr = KErrNone;

	// Determine and set sequence event.
	if (iState == EReqX3PTimer)
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER1) MOLR - NetSim Notify Release Lcs MoLr - Event."));
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);
		
		expectedErr = KErrPositionHighPriorityReceive;
		
		// Indicate we should have started the second X3P by now.
		iState = EReqX3PPush;
		}
	else if (iState == EReqX3PTimer2)
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER2) MOLR - NetSim Notify Release Lcs MoLr - Event."));
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);
		
		expectedErr = KErrNone;
		
		// Indicate we are now done
		iState = EWaiting; // End of test.
		}
		
	else if(iState == EReqX3PPush)
		{
		INFO_PRINTF1(_L("Got - X3P(PUSH) MOLR - NetSim Notify Release Lcs MoLr - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);

		// iState = EWaiting; // End of test.
		iState = EReqX3PTimer2;
		}

	// Verify reason code.
	if (aReason != expectedErr)
		{	
		INFO_PRINTF2(_L("Failed test, bad release reason %d."), aReason);
		SetTestStepResult(EFail);
		}
	}


void CT_LbsConflictStep_X3PTimerPush::NotifyMeasurementControlLocation(const TPositionInfo& aPosition, 
															  const RLbsAssistanceDataBuilderSet& aData, 
															  const TLbsNetPosRequestQuality& aQuality)
	{
	T_LbsUtils utils;	
	TInt err;
	
	// Determine and record sequence event.
	if (iState == EReqX3PTimer)
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER1) MOLR - NetSim Notify Measurement Control Location - Event."));
		// For the first attempt (before pre-emption) we *may* get this, depending how far
		// the request has run, but we can't guarantee it, so it's not added to the
		// verify list but ...
		// ... if we do get one of these, we must also get a failure message so add that to adhoc list
		SetVerifyAdhocEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementReportControlFailure);
		}
		
	else if(iState == EReqX3PTimer2)
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER2) MOLR - NetSim Notify Measurement Control Location - Event."));
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementControlLocation);
		}

	else if(iState == EReqX3PPush)
		{
		INFO_PRINTF1(_L("Got - X3P(PUSH) MOLR - NetSim Notify Measurement Control Location - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementControlLocation);
		}


	// Verify the reference position.
	TPositionInfo verifyRefPosInfo;
	
	verifyRefPosInfo.SetPosition(iRefPos);
	if (!utils.Compare_PosInfo(verifyRefPosInfo, aPosition))
		{
		INFO_PRINTF1(_L("Failed test, position incorrect."));
		SetTestStepResult(EFail);
		}

   
	// Verify the assistance data.	
	RLbsAssistanceDataBuilderSet& data = const_cast<RLbsAssistanceDataBuilderSet&>(aData);
	RUEPositioningGpsReferenceTimeBuilder* refTimeBuilder = NULL;
	
	data.GetDataBuilder(refTimeBuilder);

	// Create a reader from the builder's data to allow us to verify the actual
	// assistance data.
	RUEPositioningGpsReferenceTimeReader refTimeReader;
		
	TRAP(err, refTimeReader.OpenL());
	if (err == KErrNone)
		{
		refTimeReader.DataBuffer() = refTimeBuilder->DataBuffer();
		
		if (!utils.VerifySimpleAssistanceData(refTimeReader))
			{
			INFO_PRINTF1(_L("Failed test, assistance data incorrect."));
			SetTestStepResult(EFail);
			}
		refTimeReader.Close();
		}
			
	else
		{
		INFO_PRINTF2(_L("Failed test, assistance data reader err %d."), err);
		SetTestStepResult(EFail);
		}


	// TODO: Check if we can verify aQuality in any way.
	(void)aQuality;
	}


void CT_LbsConflictStep_X3PTimerPush::NotifyReleaseLcsLocationNotification(const CLbsNetworkProtocolBase::TLbsPrivacyResponse& aResult)
	{
	(void)aResult;

	// Unexpected callback for this test log the sequence event to fail test.
	if ((iState == EReqX3PTimer) || (iState == EReqX3PTimer2))
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER) MOLR - NetSim Notify Release Lcs Location Notification - Event."));
//		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyReleaseLcsLocationNotification);
		}
		
	else
		{
		INFO_PRINTF1(_L("Got - X3P(PUSH) MOLR - NetSim Notify Release Lcs Location Notification - Event."));
//		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyReleaseLcsLocationNotification);
		}
	}


void CT_LbsConflictStep_X3PTimerPush::NotifyFacilityLcsMoLrResult(TInt aReason, const TPositionInfo& aPosition)
	{
	// Determine and record sequence event.
	if ((iState == EReqX3PTimer) || (iState == EReqX3PTimer2))
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER) MOLR - NetSim Notify Facility Lcs MoLr Result - Event."));
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyFacilityLcsMoLrResult);
		}
		
	else
		{
		INFO_PRINTF1(_L("Got - X3P(PUSH) MOLR - NetSim Notify Facility Lcs MoLr Result - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyFacilityLcsMoLrResult);
		}

	// Verify reason code..	
	if (aReason != KErrNone)
		{	
		INFO_PRINTF2(_L("Failed test, bad release reason %d."), aReason);
		SetTestStepResult(EFail);
		}

	// Verify the real position returned from the network, this will be the same position
	// we sent to the network as the result of the MO-LR, thus use the entry given by
	// the test module.
	T_LbsUtils utils;
	RPointerArray<TAny>& verifyPosInfoArr = iParent.iSharedData->iVerifyPosInfoArr;
	TPositionInfo* verifyRealPosInfo = reinterpret_cast<TPositionInfo*>(verifyPosInfoArr[0]);
	
	if (!utils.Compare_PosInfo(*verifyRealPosInfo, aPosition))
		{
		INFO_PRINTF1(_L("Failed test, position incorrect."));
		SetTestStepResult(EFail);
	    } 
	}


void CT_LbsConflictStep_X3PTimerPush::NotifyMeasurementReportLocation(const TPositionInfo& aPosition)
	{	
	// Determine and record sequence event.
	if ((iState == EReqX3PTimer) || (iState == EReqX3PTimer2))
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER) MOLR - NetSim Notify Measurement Report - Event."));
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementReportLocation);
		}
		
	else
		{
		INFO_PRINTF1(_L("Got - X3P(PUSH) MOLR - NetSim Notify Measurement Report - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementReportLocation);
		}

		
	// Verify the real position given to the network, this will be the same position
	// returned as the result of the MO-LR, thus use the entry given by
	// the test module.	
	T_LbsUtils utils;
    RPointerArray<TAny>& verifyPosInfoArr = iParent.iSharedData->iVerifyPosInfoArr;
	TPositionInfo* verifyRealPosInfo = reinterpret_cast<TPositionInfo*>(verifyPosInfoArr[0]);
	
	if (!utils.Compare_PosInfo(*verifyRealPosInfo, aPosition))
		{
		INFO_PRINTF1(_L("Failed test, position incorrect."));
		SetTestStepResult(EFail);
		} 
    }


void CT_LbsConflictStep_X3PTimerPush::NotifyMeasurementReportRequestMoreAssistanceData(const TLbsAssistanceDataGroup& aFilter)
	{
	(void)aFilter;

	// Unexpected callback for this test, record the sequence event to fail test.
	if ((iState == EReqX3PTimer) || (iState == EReqX3PTimer2))
		{
		INFO_PRINTF1(_L("Got - X3P(TIMER) MOLR - NetSim Notify Measurement Report Request More Assistance Data - Event."));
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementReportRequestMoreAssistanceData);
		}
		
	else
		{
		INFO_PRINTF1(_L("Got - X3P(PUSH) MOLR - NetSim Notify Measurement Report Request More Assistance Data - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementReportRequestMoreAssistanceData);
		}
	}


void CT_LbsConflictStep_X3PTimerPush::NotifyMeasurementReportControlFailure(TInt aReason)
	{
	// Unexpected callback for this test, record the sequence event to fail test.
	if (iState == EReqX3PTimer)
		{
		INFO_PRINTF2(_L("Got - X3P(TIMER1) MOLR - Net Sim Notify Measurement Report Control Failure - Event. Reason = %d"), aReason);
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementReportControlFailure);
		if(aReason != KErrPositionHighPriorityReceive)
			{
			SetTestStepResult(EFail);
			}
		}
		
	else if(iState == EReqX3PTimer2)
		{
		INFO_PRINTF2(_L("Got - X3P(TIMER) MOLR - Net Sim Notify Measurement Report Control Failure - Event. Reason = %d"), aReason);
		SetCurrentSeqEvent(EX3P_TIMER_MOLR_NetSim_Got_NotifyMeasurementReportControlFailure);
		}

	else if(iState == EReqX3PPush)
		{
		INFO_PRINTF2(_L("Got - X3P(PUSH) MOLR - Net Sim Notify Measurement Report Control Failure - Event. Reason = %d"), aReason);
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementReportControlFailure);
		}
	}

	

/** X3P transmit callback.

	The X3P transmit request has completed.
*/	
void CT_LbsConflictStep_X3PTimerPush::MT_LbsDoX3PCallback(TInt aTransmitId, TRequestStatus& aStatus)
	{
	TInt expectedErr = KErrNone;
	SetCurrentSeqEvent(EClient_Got_X3P_Complete);
	
	if (aTransmitId == KLbsConflictX3PTimerId)
		{
		INFO_PRINTF1(_L("Got - Client X3P(TIMER) Complete - Callback Event."));
		}
		
	else if (aTransmitId == KLbsConflictX3PPushId)
		{
		INFO_PRINTF1(_L("Got - Client X3P(PUSH) Complete - Callback Event."));
		}

	// Verify error code.
	if (expectedErr != aStatus.Int())
		{
		INFO_PRINTF2(_L("Failed test, X3P transmit request err = %d."), aStatus.Int());
		SetTestStepResult(EFail);
		}
	}
		

