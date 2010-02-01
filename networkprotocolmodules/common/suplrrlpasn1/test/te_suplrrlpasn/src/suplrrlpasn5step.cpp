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
// incoming SUPL POS with RRLP MEASURE POSITION REQUEST payload
// 
//

/**
 @file suplrrlpasn5step.cpp
 @internalTechnology
*/
#include "suplrrlpasn5step.h"
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


CSuplRrlpAsn5Step::~CSuplRrlpAsn5Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn5Step::CSuplRrlpAsn5Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn5Step);
	}

TVerdict CSuplRrlpAsn5Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn5Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestReceiveSuplPosRrlpMeasurePositionRequestL());
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			}
		else
			{
			SetTestStepResult(EPass);
			}
		}
	  return TestStepResult();
	}

TVerdict CSuplRrlpAsn5Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}


void CSuplRrlpAsn5Step::TestReceiveSuplPosRrlpMeasurePositionRequestL()
	{
	TInt err = KErrNone;

	// Receive a SUPL POS with an RRLP ASSISTANCE DATA payload
	TPtr8 decodePtr(receiveBuffer.Des());
	err = iSpoofServer->GenerateSuplPosRrlpMeasurePositionRequestL(decodePtr);
	LeaveIfErrorL(err);
	
	// Create an ASN1 Decoder object...
	CSuplAsn1Decoder* suplAsn1Decoder = CSuplAsn1Decoder::NewL();
	CleanupStack::PushL(suplAsn1Decoder);

	// ...and decode the received message
	TPtrC8 receivePtr(decodePtr);
	CSuplMessageBase* receivedMessage = suplAsn1Decoder->DecodeL(&receivePtr, err);
	CleanupStack::PushL(receivedMessage);

	// extract the version
	CSuplVersion* version = CSuplVersion::NewL();
	CleanupStack::PushL(version);
	err = receivedMessage->GetVersion(*version);
	LeaveIfErrorL(err);
	err = CheckVersionContentL(*version);
	LeaveIfErrorL(err);

	// extract the Session ID
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	err = receivedMessage->GetSessionId(*sessionId);
	LeaveIfErrorL(err);
	err = CheckSessionIdContentL(*sessionId);
	LeaveIfErrorL(err);
	
	// message type
	CSuplMessageBase::TSuplMessageType type = receivedMessage->MessageType();

	// handle message type specific stuff
	if (type == CSuplMessageBase::ESuplPos)
		{
		CSuplPos* pos = static_cast <CSuplPos*> (receivedMessage);
		
		// VELOCITY component
		if (pos->VelocityPresent())
			{
			// extract velocity.
			}
		
		// POSITIONING PAYLOAD
		CSuplPosPayload* posPayload = pos->PosPayload();
		CleanupStack::PushL(posPayload);
		if (posPayload->MessageType() == CSuplPosPayload::ERrlpMeasurePositionReq)
			{
			CRrlpMeasurePositionRequest* posRequest = static_cast <CRrlpMeasurePositionRequest*>(posPayload);
			
			// REFERENCE (inc Rel-5 Extended Reference)
			TRrlpReference rrlpReference;
			err = posRequest->GetReference(rrlpReference);
			LeaveIfErrorL(err);
			
			err = CheckRrlpReferenceContentL(rrlpReference);
			LeaveIfErrorL(err);
			
			// POSITION INSTRUCT accesses mandatory components
			TLbsNetPosRequestQuality quality;
			TLbsNetPosRequestMethod method;
			err = posRequest->GetPositionInstruct(quality, method);
			LeaveIfErrorL(err);

			err = CheckPositionInstructContentL(quality, method);
			LeaveIfErrorL(err);
				
			// ASSISTANCE DATA is optionally included
			if (posRequest->AssistanceDataPresent())
				{
				TLbsAsistanceDataGroup dataMask;
				RLbsAssistanceDataBuilderSet builderSet;
				CleanupClosePushL(builderSet);
				builderSet.OpenL();
				err = posRequest->BuildAssistanceData(dataMask, builderSet);

				CleanupStack::PopAndDestroy(&builderSet);
				}
			}
		else
			{
			// unexpected payload received
			err = KErrGeneral;
			LeaveIfErrorL(err);
			}
		CleanupStack::PopAndDestroy(posPayload);
		}
	else
		{
		// unexpected message received
		err = KErrGeneral;
		LeaveIfErrorL(err);
		}

	CleanupStack::PopAndDestroy(sessionId);
	CleanupStack::PopAndDestroy(version);		
	CleanupStack::PopAndDestroy(receivedMessage);	
	CleanupStack::PopAndDestroy(suplAsn1Decoder);
	}


// check received supl pos rrlp measure position request content
TInt CSuplRrlpAsn5Step::CheckPositionInstructContentL(TLbsNetPosRequestQuality& aQuality, TLbsNetPosRequestMethod& aMethod)
	{
	// max fix time is 2^n seconds (n = 3)
	TTimeIntervalMicroSeconds maxFixTime = aQuality.MaxFixTime();
	if (maxFixTime.Int64() != 8*1000*1000)
		{
		return KErrGeneral;
		}
	
	// no corresponding value in RRLP for max fix age, should not be set.
	TTimeIntervalMicroSeconds maxFixAge  = aQuality.MaxFixAge();
	if (maxFixAge != 0)
		{
		return KErrGeneral;
		}
	
	// horizontal accuracy has been converted to meters from k=10;
	TReal32 minHorizontalAccuracy = aQuality.MinHorizontalAccuracy();
	TReal expectedMinAccuracy; 
	Math::Pow(expectedMinAccuracy, 1.1, 10);
	expectedMinAccuracy -= 1;
	expectedMinAccuracy *= 10;
	if (minHorizontalAccuracy != (TReal32)expectedMinAccuracy)
		{
		return KErrGeneral;
		}

	// min vertical accuracy is set to same as horizontal accuracy (no separate info)
	TReal32 minVerticalAccuracy = aQuality.MinVerticalAccuracy();
	if (minVerticalAccuracy != (TReal32)expectedMinAccuracy)
		{
		return KErrGeneral;
		}
	
	// pos method should be A-GPS, terminal based, assisted.
	if (aMethod.NumPosMethods() != 1)
		{
		return KErrGeneral;
		}
	
	TLbsNetPosMethod posMethod;
	if (aMethod.GetPosMethod(0, posMethod) != KErrNone)
		{
		return KErrGeneral;
		}

	TUid means = posMethod.PosMeans();
	if (means != KLbsPositioningMeansGps)
		{
		return KErrGeneral;
		}
	
	TPositionModuleInfo::TTechnologyType expectedPosMode = TPositionModuleInfo::ETechnologyTerminal | TPositionModuleInfo::ETechnologyAssisted;
	if (posMethod.PosMode() != expectedPosMode)
		{
		return KErrGeneral;
		}
	
     // check GPS timing measurements requested
     if (!aMethod.GpsTimingOfCellFramesRequested())
         {
         return KErrGeneral;
         }
     return KErrNone;
     }
	

