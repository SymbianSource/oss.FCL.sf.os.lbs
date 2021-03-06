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
// @file ctlbsassdatastepx3ppushselflocate.cpp
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
#include "ctlbsconflictstepx3ppushselflocate.h"
#include <lbs/test/tlbsutils.h>


_LIT(KTransmitDestination, "07712345678");
const TUint KTransmitPriority = 2;	// X3P Push.

//const TUint KLbsAbortPeriod = 10000000;	// The test case should finish before 10 seconds.


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
CT_LbsConflictStepx3ppushselflocate* CT_LbsConflictStepx3ppushselflocate::New(CT_LbsConflictServer& aParent)
	{
	// Note the lack of ELeave.
	// This means that having insufficient memory will return NULL;
	CT_LbsConflictStepx3ppushselflocate* testStep = new CT_LbsConflictStepx3ppushselflocate(aParent);
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
CT_LbsConflictStepx3ppushselflocate::CT_LbsConflictStepx3ppushselflocate(CT_LbsConflictServer& aParent) : CT_LbsConflictStep(aParent)
	{
	SetTestStepName(KLbsConflictStepx3ppushselflocate);
	}


void CT_LbsConflictStepx3ppushselflocate::ConstructL()
	{
	// Create the base class objects.
	CT_LbsConflictStep::ConstructL();
	
	// Self locate async wrapper.
	iDoPosUpdate = CT_LbsDoPosUpdate::NewL(this);

	// X3P async wrapper.
	iDoX3P = CT_LbsDoX3P::NewL(this);
	}


/**
 * Destructor
 */
CT_LbsConflictStepx3ppushselflocate::~CT_LbsConflictStepx3ppushselflocate()
	{
	iDoPosUpdate->Cancel();
	delete iDoPosUpdate;

	iDoX3P->Cancel();
	delete iDoX3P;
	}


/** Called at the start of the test...

	Each test case MUST implement this.

	The table below describes the expcted order that the callbacks should be called
	during the test.
	
	Note that the tests deliberateley do NOT check the order that the client requests
	are completed. This is because the delivery order is not guaranteed by LBS.
	The tests will check that all client requests are completed with the correct error
	code and position at the end of the test.
*/
void CT_LbsConflictStepx3ppushselflocate::SetExpectedSeq()
	{
	
//!		Client:TransmitPosition(PUSH)							->LBS
//!																			RegisterLcsMoLr				->NET
//!		Client:NotifyPositionUpdate								->LBS
//!																			MeasurementControlLocation	<-NET
//!																			MeasurementReportLocation	->NET
//!																			FacilityLcsMoLrResult		<-NET
//!																			ReleaseLcsMoLr				->NET
//!		Client:RunL(NotifyPositionUpdate(pos,KErrServerBusy))	->LBS
//!		Client:RunL(TransmitPosition(PUSH,pos,KErrNone))		->LBS
	
	
	// Self locate MOLR (iState = ERegPosUpdate)
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);				// --> To Net
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementControlLocation);	// <-- From Net
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementReportLocation);	// --> To Net
	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyFacilityLcsMoLrResult);		// <-- From Net

	SetVerifySeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);				// --> To Net
	// (after X3P release iState = EWaiting).
	
	SetVerifyAdhocEvent(EClient_Got_PosUpdate_Complete);
	SetVerifyAdhocEvent(EClient_Got_X3P_Complete);
	}



/** Called at the end of the test to verify the correct position data has been returned to the
	client.
	
	Each test case SHOULD implement a version of this.
*/
void CT_LbsConflictStepx3ppushselflocate::VerifyPosInfos()
	{
	T_LbsUtils utils;
    RPointerArray<TAny>& verifyPosInfoArr = iParent.iSharedData->iVerifyPosInfoArr;
	RPointerArray<TAny>& currPosInfoArr = iParent.iSharedData->iCurrentPosInfoArr;
	TPositionInfo* currPosInfo;
	

	// Verify entry 0 for the X3P. We expect a real location value, compare using the data
	// sent to the test APGS module.
	TPositionInfo* verifyPosInfo = reinterpret_cast<TPositionInfo*>(verifyPosInfoArr[0]);
	currPosInfo = reinterpret_cast<TPositionInfo*>(currPosInfoArr[0]);
	if (!utils.Compare_PosInfo(*verifyPosInfo, *currPosInfo))
		{
		INFO_PRINTF1(_L("Failed test, X3P position incorrect."));
		SetTestStepResult(EFail);
		}
		
	// entry 1 - the self locate pos should have ll NANs in it!	
	currPosInfo = reinterpret_cast<TPositionInfo*>(currPosInfoArr[1]);
	TPosition pos;
	currPosInfo->GetPosition(pos);
	if (!Math::IsNaN(pos.Latitude()))
		{
		INFO_PRINTF1(_L("Failed test, Position does not contain NANs"));

		SetTestStepResult(EFail);
		}
	if (!Math::IsNaN(pos.Longitude()))
		{
		INFO_PRINTF1(_L("Failed test, Position does not contain NANs"));

		SetTestStepResult(EFail);
		}

	}


/** A standard TEF test case doTestStepL, this SHOULD only support a single test case.

	Typically the function will look much like this.
 */
TVerdict CT_LbsConflictStepx3ppushselflocate::doTestStepL()
	{
	// Generic test step used to test the LBS Client Notify position update API.
	INFO_PRINTF1(_L("&gt;&gt;CT_LbsConflictStepx3ppushselflocate::doTestStepL()"));

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

	INFO_PRINTF1(_L("&lt;&lt;CT_LbsConflictStepx3ppushselflocate::doTestStepL()"));

	return TestStepResult();
	}


/** NetSim callbacks given for a MoLr, which we invoke as a result of the notify position update.
*/
void CT_LbsConflictStepx3ppushselflocate::Connected()
	{
	// Call base implementation.
	CT_LbsConflictStep::Connected();
	
	// Kick off first pos update.
	
	// Create a posinfo and store in our shared array for later verification.
	RPointerArray<TAny>& posInfoArr = iParent.iSharedData->iCurrentPosInfoArr;
	TPositionInfo* posInfo = new(ELeave) TPositionInfo();

	T_LbsUtils utils;
	utils.ResetAndDestroy_PosInfoArr(posInfoArr);	// Clear previous entries before new entry is appended.

	posInfoArr.Append(posInfo);

	TLbsTransmitPositionOptions transOpts;
	
	iDoX3P->SetOptions(transOpts);	// Set timeout value to 0, to disable the timeout.
	iDoX3P->StartL(KTransmitDestination, KTransmitPriority, *posInfo);

	}


void CT_LbsConflictStepx3ppushselflocate::Disconnected()
	{
	// Call base implementation.
	CT_LbsConflictStep::Disconnected();
	}


void CT_LbsConflictStepx3ppushselflocate::NotifyRegisterLcsMoLr(const TDesC& aData)
	{
	// Determine and record the sequence event. A blank aData indicates a self locate
	// MOLR, otherwise we have a X3P MOLR. Also the current test state indicates which
	// type MOLR is being carried out.
	if (aData != KNullDesC)
		{
		INFO_PRINTF1(_L("Got - X3P MOLR - NetSim Notify Register Lcs MoLr - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);
				
		// Verify telephone number.
		if (aData.Compare(KTransmitDestination))
			{
			INFO_PRINTF1(_L("Failed test, bad X3P register data."));
			SetTestStepResult(EFail);
			}
		
		iState = EReqX3P;
		
		}

	else if (aData == KNullDesC)
		{
		iState = EReqPosUpdate;
		INFO_PRINTF1(_L("Got - Self MOLR - NetSim Notify Register Lcs MoLr - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyRegisterLcsMoLr);
		}


	}


void CT_LbsConflictStepx3ppushselflocate::NotifyReleaseLcsMoLr(TInt aReason)
	{
	TInt expectedErr = KErrNone;

	// Determine and set sequence event.
	if (iState == EReqPosUpdate)
		{
		INFO_PRINTF1(_L("Got - Self MOLR - NetSim Notify Release Lcs MoLr - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);
		
		expectedErr = KErrPositionHighPriorityReceive;
		}
		
	else if(iState == EReqX3P)
		{
		INFO_PRINTF1(_L("Got - X3P MOLR - NetSim Notify Release Lcs MoLr - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyReleaseLcsMoLr);

		iState = EWaiting; // End of test.
		}
	
	// Verify reason code.
	if (aReason != expectedErr)
		{	
		INFO_PRINTF2(_L("Failed test, bad release reason %d."), aReason);
		SetTestStepResult(EFail);
		}
	}


void CT_LbsConflictStepx3ppushselflocate::NotifyMeasurementControlLocation(const TPositionInfo& aPosition, 
															  const RLbsAssistanceDataBuilderSet& aData, 
															  const TLbsNetPosRequestQuality& aQuality)
	{
	T_LbsUtils utils;	
	TInt err;
	
	// Determine and record sequence event.
	if (iState == EReqPosUpdate)
		{
		INFO_PRINTF1(_L("Got - Self MOLR - NetSim Notify Measurement Control Location - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyMeasurementControlLocation);
		}
		
	else if(iState == EReqX3P)
		{
		INFO_PRINTF1(_L("Got - X3P MOLR - NetSim Notify Measurement Control Location - Event."));
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

	
	
	// Start the self locate
	// Create a posinfo and store in our shared array for later verification.
	RPointerArray<TAny>& posInfoArr = iParent.iSharedData->iCurrentPosInfoArr;
	TPositionInfo* posInfo = new(ELeave) TPositionInfo();

	posInfoArr.Append(posInfo);

	// Kick off self locate pos update.
	
#if 0
	T_LbsUtils utils;

	utils.WaitForModuleToRequestAssistanceDataL();
#endif

	iDoPosUpdate->StartL(*posInfo);


	
	
	
	
	// TODO: Check if we can verify aQuality in any way.
	(void)aQuality;
	}


void CT_LbsConflictStepx3ppushselflocate::NotifyReleaseLcsLocationNotification(const CLbsNetworkProtocolBase::TLbsPrivacyResponse& aResult)
	{
	(void)aResult;

	// Unexpected callback for this test log the sequence event to fail test.
	if (iState == EReqPosUpdate)
		{
		INFO_PRINTF1(_L("Got - Self MOLR - NetSim Notify Release Lcs Location Notification - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyReleaseLcsLocationNotification);
		}
		
	else
		{
		INFO_PRINTF1(_L("Got - X3P MOLR - NetSim Notify Release Lcs Location Notification - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyReleaseLcsLocationNotification);
		}
	}


void CT_LbsConflictStepx3ppushselflocate::NotifyFacilityLcsMoLrResult(TInt aReason, const TPositionInfo& aPosition)
	{
	
	// Determine and record sequence event
	if (iState == EReqPosUpdate)
		{
		INFO_PRINTF1(_L("Got - Self MOLR - NetSim Notify Facility Lcs MoLr Result - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyFacilityLcsMoLrResult);
		}
		
	else if(iState == EReqX3P)
		{
		INFO_PRINTF1(_L("Got - X3P MOLR - NetSim Notify Facility Lcs MoLr Result - Event."));
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


	
void CT_LbsConflictStepx3ppushselflocate::NotifyMeasurementReportLocation(const TPositionInfo& aPosition)
	{	
	// Determine and record sequence event.
	if (iState == EReqPosUpdate)
		{
		INFO_PRINTF1(_L("Got - Self MOLR - NetSim Notify Measurement Report - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyMeasurementReportLocation);
		}
		
	else if(iState == EReqX3P)
		{
		INFO_PRINTF1(_L("Got - X3P MOLR - NetSim Notify Measurement Report - Event."));
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


void CT_LbsConflictStepx3ppushselflocate::NotifyMeasurementReportRequestMoreAssistanceData(const TLbsAssistanceDataGroup& aFilter)
	{
	(void)aFilter;

	// Unexpected callback for this test, record the sequence event to fail test.
	if (iState == EReqPosUpdate)
		{
		INFO_PRINTF1(_L("Got - Self MOLR - NetSim Notify Measurement Report Request More Assistance Data - Event."));
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyMeasurementReportRequestMoreAssistanceData);
		}
		
	else
		{
		INFO_PRINTF1(_L("Got - X3P MOLR - NetSim Notify Measurement Report Request More Assistance Data - Event."));
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementReportRequestMoreAssistanceData);
		}
	}


void CT_LbsConflictStepx3ppushselflocate::NotifyMeasurementReportControlFailure(TInt aReason)
	{
	// Unexpected callback for this test, record the sequence event to fail test.
	if (iState == EReqPosUpdate)
		{
		INFO_PRINTF2(_L("Got - Self MOLR - Net Sim Notify Measurement Report Control Failure - Event. Reason = %d"), aReason);
		SetCurrentSeqEvent(ESelf_MOLR_NetSim_Got_NotifyMeasurementReportControlFailure);
		}
		
	else
		{
		INFO_PRINTF2(_L("Got - X3P MOLR - Net Sim Notify Measurement Report Control Failure - Event. Reason = %d"), aReason);
		SetCurrentSeqEvent(EX3P_PUSH_MOLR_NetSim_Got_NotifyMeasurementReportControlFailure);
		}
	}


/**	Notify position update callback.

	The notify position update has completed.
*/
void CT_LbsConflictStepx3ppushselflocate::MT_LbsDoPosUpdateCallback(TRequestStatus& aStatus)
	{
	INFO_PRINTF1(_L("Got - Client Notify Update Complete - Callback Event."));
	SetCurrentSeqEvent(EClient_Got_PosUpdate_Complete);
	
	// Expect the self locate MOLR to be stopped because of the X3P.
	if (KErrServerBusy != aStatus.Int())
		{
		INFO_PRINTF2(_L("Failed test, pos info request err = %d."), aStatus.Int());
		SetTestStepResult(EFail);
		}
	else
		{
		INFO_PRINTF2(_L("Got KErrServerBusy as expected err = %d."), aStatus.Int());
		}
	}	
	

/** X3P transmit callback.

	The X3P transmit request has completed.
*/	
void CT_LbsConflictStepx3ppushselflocate::MT_LbsDoX3PCallback(TInt aTransmitId, TRequestStatus& aStatus)
	{
	(void)aTransmitId;
	
	INFO_PRINTF1(_L("Got - Client X3P Complete - Callback Event."));
	SetCurrentSeqEvent(EClient_Got_X3P_Complete);
	
	if (KErrNone != aStatus.Int())
		{

		INFO_PRINTF2(_L("Failed test, X3P transmit request err = %d."), aStatus.Int());
		SetTestStepResult(EFail);
		}
	}
		

