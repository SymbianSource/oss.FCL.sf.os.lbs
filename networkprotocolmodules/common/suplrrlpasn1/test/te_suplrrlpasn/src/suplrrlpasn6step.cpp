// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// outgoing SUPL POS with RRLP MEASURE POSITION RESPONSE payload
// 
//

/**
 @file suplrrlpasn6step.cpp
 @internalTechnology
*/
#include "suplrrlpasn6step.h"
#include "te_suplrrlpasnsuitedefs.h"

// LBS
#include <lbs/lbsnetcommon.h>
#include <lbs/lbsassistancedatabase.h>
#include <lbspositioninfo.h>
#include <lbs/lbsgpsmeasurement.h>

// items under test
#include "suplmessagebase.h"
#include "suplasn1decoder.h"
#include "suplmessagecommon.h"
#include "suplstart.h"
#include "suplresponse.h"
#include "suplposinit.h"
#include "suplpos.h"
#include "suplpospayload.h"
#include "suplend.h"
#include "rrlpassistancedata.h"
#include "rrlpassistancedataack.h"
#include "rrlpmeasureposrequest.h"
#include "rrlpmeasureposresponse.h"
#include "rrlpprotocolerror.h"


CSuplRrlpAsn6Step::~CSuplRrlpAsn6Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn6Step::CSuplRrlpAsn6Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn6Step);
	}

TVerdict CSuplRrlpAsn6Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn6Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{

	// test 1
	if (TestStepResult()==EPass)
		{
		TRAPD(err, TestEncodeSuplPosRrlpMeasurePositionResponseL());
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			}
		}

	// test 2 - with 
	if (TestStepResult()==EPass)
		{
		TRAPD(err, TestEncodeSuplPosRrlpMeasurePositionResponse2L());
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			}
		}

	return TestStepResult();
	}

TVerdict CSuplRrlpAsn6Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}


void CSuplRrlpAsn6Step::TestEncodeSuplPosRrlpMeasurePositionResponseL()
	{
	// check for any returned error code
	TInt err = KErrNone;
	
	// create a CSuplSessionId and populate
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId);

	// VELOCITY (optional)
	
	// POSITIONING PAYLOAD (mandatory)
	CRrlpMeasurePositionResponse* mPosResp = CRrlpMeasurePositionResponse::NewL();
	CleanupStack::PushL(mPosResp);

	// REFERENCE (inc Rel-5 Extended Reference)
	TRrlpReference reference;
	reference.aRefNum = 5; 
	reference.aRel5EntendedRefPresent = ETrue;
	reference.aRel5SmlcCode = 39;
	reference.aRel5TransactionId = 13337;
	err = mPosResp->SetReference(reference);
	LeaveIfErrorL(err);


	// LOCATION INFORMATION	(optional) (note only using 2D coordinate)
	TPositionInfo posInfo;
	TPosition position;
	position.SetCoordinate(23.456, 45.678, 25.34);
	position.SetHorizontalAccuracy(9.345);
	position.SetVerticalAccuracy(12.5);
	TDateTime dateTime(2008, EMarch, 12, 11, 54, 23, 00);
	TTime time(dateTime);
	position.SetTime(time);
	posInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal);
	posInfo.SetPositionModeReason(EPositionModeReasonNone);
	posInfo.SetUpdateType(EPositionUpdateUnknown);
	posInfo.SetPosition(position);

	err = mPosResp->SetLocationInformation(posInfo);
	LeaveIfErrorL(err);
	
	// GPS MEASUREMENT INFORMATION (optional, TA only)
	TPositionGpsMeasurementInfo measureInfo;
	measureInfo.SetGpsTimeOfWeek(987654);

		// satellite info #1
		TPositionGpsMeasurementData gpsSatData;
		gpsSatData.SetSatelliteId(12);					// 0..63
		gpsSatData.SetCarrierNoiseRatio(23);			// 0..63
		gpsSatData.SetDoppler(789);						// +- 6553.6
		gpsSatData.SetWholeGpsChips(321);				// 0..1022
		gpsSatData.SetFractionalGpsChips(654);			// 0 - 1-2^-10 
		gpsSatData.SetMultiPathIndicator(TPositionGpsMeasurementData::EMultiPathLow);
		gpsSatData.SetPseudoRangeRmsError(0);			// 0.5..112
		measureInfo.AppendMeasurementData(gpsSatData);

		// satellite info #2
		gpsSatData.SetSatelliteId(2);
		gpsSatData.SetCarrierNoiseRatio(56);
		gpsSatData.SetDoppler(54);
		gpsSatData.SetWholeGpsChips(12);
		gpsSatData.SetFractionalGpsChips(78);
		gpsSatData.SetMultiPathIndicator(TPositionGpsMeasurementData::EMultiPathNotMeasured);
		gpsSatData.SetPseudoRangeRmsError(0);
		measureInfo.AppendMeasurementData(gpsSatData);
		
		// GPS Timing Measurements
        TGpsTimingMeasurementData aGpsTimingData;
        aGpsTimingData.SetDataType(TGpsTimingMeasurementData::EGpsTimingDataTypeGsm);
        aGpsTimingData.SetReferenceFrameMsb(28);
        aGpsTimingData.SetGpsTowSubms(6666);
        aGpsTimingData.SetDeltaTow(111);
        aGpsTimingData.SetGpsReferenceTimeUncertainty(82);
        measureInfo.SetGpsTimingData(aGpsTimingData);
	
	err = mPosResp->SetMeasurementInformation(measureInfo);
	LeaveIfErrorL(err);

	
	// LOCATION ERROR (optional)
	TRrlpLocError locError = ERrlpLocErrorGpsAssDataMissing;
	TLbsAsistanceDataGroup dataReqMask;
	dataReqMask = EAssistanceDataNone;
	dataReqMask = EAssistanceDataAquisitionAssistance |
			   	  EAssistanceDataNavigationModel |
			   	  EAssistanceDataReferenceLocation;

	err = mPosResp->SetLocationError(locError, dataReqMask);
	
	// Create the SUPL POS Message, and pass ownership of the payload.
	CSuplPos* suplPos = CSuplPos::NewL(ETrue);
	CleanupStack::Pop(mPosResp);
	suplPos->SetPosPayload(mPosResp);
	mPosResp = NULL;
 	CleanupStack::PushL(suplPos);
	
	// populate the remaining SUPL MESSAGE components	
 	err = suplPos->SetSessionId(*sessionId);					// (mandatory)
	LeaveIfErrorL(err);
 	
	// POSITION (optional)
	TPositionCourseInfo courseInfo;
	courseInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal);
	courseInfo.SetPositionModeReason(EPositionModeReasonNone);
	courseInfo.SetUpdateType(EPositionUpdateUnknown);
	courseInfo.SetPosition(position);
	TCourse course;
	course.SetHeading(239.2233);
	course.SetHeadingAccuracy(3.8);
	course.SetSpeed(6.94444444); // meters per second. == 25 km per hour
	course.SetSpeedAccuracy(25);
	courseInfo.SetCourse(course);
 	err = suplPos->SetVelocity(courseInfo);
	LeaveIfErrorL(err);
	
	// encode it to the output buffer
	TPtr8 encodePtr(encodeBuffer.Des());
	err = suplPos->EncodeToL(encodePtr);
	LeaveIfErrorL(err);
	
	// test decode
	TPtrC8 decodePtr(encodePtr);
	err = iSpoofServer->DecodeL(&decodePtr);
	LeaveIfErrorL(err);

	CleanupStack::PopAndDestroy(suplPos);
	CleanupStack::PopAndDestroy(sessionId);
	}

/**
Test #2 

This test uses a different value for GPS TOW - however should encode to the same
value so the same decode/check method is used to check encoded values.

also uses a location in the southern hemisphere / west

*/
void CSuplRrlpAsn6Step::TestEncodeSuplPosRrlpMeasurePositionResponse2L()
	{
	// check for any returned error code
	TInt err = KErrNone;
	
	// create a CSuplSessionId and populate
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId);

	// VELOCITY (optional)
	
	// POSITIONING PAYLOAD (mandatory)
	CRrlpMeasurePositionResponse* mPosResp = CRrlpMeasurePositionResponse::NewL();
	CleanupStack::PushL(mPosResp);

	// REFERENCE (inc Rel-5 Extended Reference)
	TRrlpReference reference;
	reference.aRefNum = 5; 
	reference.aRel5EntendedRefPresent = ETrue;
	reference.aRel5SmlcCode = 39;
	reference.aRel5TransactionId = 13337;
	err = mPosResp->SetReference(reference);
	LeaveIfErrorL(err);

	// LOCATION INFORMATION	(optional) (note only using 2D coordinate)
	TPositionInfo posInfo;
	TPosition position;
	position.SetCoordinate(-23.456, -45.678, 25.34);
	position.SetHorizontalAccuracy(9.345);
	position.SetVerticalAccuracy(12.5);
	TDateTime dateTime(2008, EMarch, 12, 11, 54, 23, 00);
	TTime time(dateTime);
	position.SetTime(time);
	posInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal);
	posInfo.SetPositionModeReason(EPositionModeReasonNone);
	posInfo.SetUpdateType(EPositionUpdateUnknown);
	posInfo.SetPosition(position);

	err = mPosResp->SetLocationInformation(posInfo);
	LeaveIfErrorL(err);
	
	// GPS MEASUREMENT INFORMATION (optional, TA only)
	TPositionGpsMeasurementInfo measureInfo;
	measureInfo.SetGpsTimeOfWeek(15387654);   // *** this parameter different to test #1 ***

		// satellite info #1
		TPositionGpsMeasurementData gpsSatData;
		gpsSatData.SetSatelliteId(12);					// 0..63
		gpsSatData.SetCarrierNoiseRatio(23);			// 0..63
		gpsSatData.SetDoppler(789);						// +- 6553.6
		gpsSatData.SetWholeGpsChips(321);				// 0..1022
		gpsSatData.SetFractionalGpsChips(654);			// 0 - 1-2^-10 
		gpsSatData.SetMultiPathIndicator(TPositionGpsMeasurementData::EMultiPathLow);
		gpsSatData.SetPseudoRangeRmsError(0);			// 0.5..112
		measureInfo.AppendMeasurementData(gpsSatData);

		// satellite info #2
		gpsSatData.SetSatelliteId(2);
		gpsSatData.SetCarrierNoiseRatio(56);
		gpsSatData.SetDoppler(54);
		gpsSatData.SetWholeGpsChips(12);
		gpsSatData.SetFractionalGpsChips(78);
		gpsSatData.SetMultiPathIndicator(TPositionGpsMeasurementData::EMultiPathNotMeasured);
		gpsSatData.SetPseudoRangeRmsError(0);
		measureInfo.AppendMeasurementData(gpsSatData);
	
	err = mPosResp->SetMeasurementInformation(measureInfo);
	LeaveIfErrorL(err);

	
	// LOCATION ERROR (optional)
	TRrlpLocError locError = ERrlpLocErrorGpsAssDataMissing;
	TLbsAsistanceDataGroup dataReqMask;
	dataReqMask = EAssistanceDataNone;
	dataReqMask = EAssistanceDataAquisitionAssistance |
			   	  EAssistanceDataNavigationModel |
			   	  EAssistanceDataReferenceLocation;

	err = mPosResp->SetLocationError(locError, dataReqMask);
	
	// Create the SUPL POS Message, and pass ownership of the payload.
	CSuplPos* suplPos = CSuplPos::NewL(ETrue);
	CleanupStack::Pop(mPosResp);
	suplPos->SetPosPayload(mPosResp);
	mPosResp = NULL;
 	CleanupStack::PushL(suplPos);
	
	// populate the remaining SUPL MESSAGE components	
 	err = suplPos->SetSessionId(*sessionId);					// (mandatory)
	LeaveIfErrorL(err);
 	
	// POSITION (optional)
	TPositionCourseInfo courseInfo;
	courseInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal);
	courseInfo.SetPositionModeReason(EPositionModeReasonNone);
	courseInfo.SetUpdateType(EPositionUpdateUnknown);
	courseInfo.SetPosition(position);
	TCourse course;
	course.SetHeading(239.2233);
	course.SetHeadingAccuracy(3.8);
	course.SetSpeed(6.94444444); // meters per second. == 25 km per hour
	course.SetSpeedAccuracy(25);
	courseInfo.SetCourse(course);
 	err = suplPos->SetVelocity(courseInfo);
	LeaveIfErrorL(err);
	
	// encode it to the output buffer
	TPtr8 encodePtr(encodeBuffer.Des());
	err = suplPos->EncodeToL(encodePtr);
	LeaveIfErrorL(err);
	
	// test decode
	TPtrC8 decodePtr(encodePtr);
	err = iSpoofServer->DecodeL(&decodePtr);
	LeaveIfErrorL(err);

	CleanupStack::PopAndDestroy(suplPos);
	CleanupStack::PopAndDestroy(sessionId);
	}
	
