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
// incoming SUPL END
// 
//

/**
 @file suplrrlpasn11step.cpp
 @internalTechnology
*/
#include "suplrrlpasn11step.h"
#include "te_suplrrlpasnsuitedefs.h"

#include "e32cmn.h"

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


CSuplRrlpAsn11Step::~CSuplRrlpAsn11Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn11Step::CSuplRrlpAsn11Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn11Step);
	}

TVerdict CSuplRrlpAsn11Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn11Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestReceiveSuplEndL());
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

TVerdict CSuplRrlpAsn11Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}

void CSuplRrlpAsn11Step::TestReceiveSuplEndL()
	{
	TInt err;

	// Receive a SUPL END
	TPtr8 decodePtr(receiveBuffer.Des());
	err = iSpoofServer->GenerateSuplEndL(decodePtr);
	LeaveIfErrorL(err);

	// Create an ASN1 Decoder object...
	CSuplAsn1Decoder* suplAsn1Decoder = CSuplAsn1Decoder::NewL();
	CleanupStack::PushL(suplAsn1Decoder);

	// ...and decode the received message
	TPtrC8 receivePtr(decodePtr);
	CSuplMessageBase* receivedMessage = suplAsn1Decoder->DecodeL(&receivePtr, err);
	CleanupStack::PushL(receivedMessage);

	// VERSION
	CSuplVersion* version = CSuplVersion::NewL();
	CleanupStack::PushL(version);
	err = receivedMessage->GetVersion(*version);
	LeaveIfErrorL(err);
	CheckVersionContentL(*version);
	LeaveIfErrorL(err);

	// SESSION ID
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	err = receivedMessage->GetSessionId(*sessionId);
	LeaveIfErrorL(err);
	CheckSessionIdContentL(*sessionId);

	// MESSAGE TYPE
	CSuplMessageBase::TSuplMessageType type = receivedMessage->MessageType();

	// handle message type specific stuff
	if (type == CSuplMessageBase::ESuplEnd)
		{
		CSuplEnd* end = static_cast <CSuplEnd*> (receivedMessage);
		
		// POSITION (o)
		if (end->PositionPresent())
			{
			TPosition position;
			err = end->GetPosition(position);
			LeaveIfErrorL(err);
			err = CheckPositionContentL(position);
			LeaveIfErrorL(err);
			}
		
		// STATUS CODE
		if (end->StatusCodePresent())
			{
			TSuplStatusCode status;
			status = end->StatusCode();
			if (status != ESuplStatusConsentGrantedByUser)
				LeaveIfErrorL(KErrGeneral);
			}
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


TInt CSuplRrlpAsn11Step::CheckPositionContentL(TPosition& aPosition)
	{
	TTime timestamp = aPosition.Time();
	// day is actually 20th, but corrected for TDateTime day-1 enumeration offset
	TDateTime testDateTime(2007, EDecember, 19, 13, 44, 34, 00);
	TTime testTime(testDateTime);
	TTimeIntervalMicroSeconds interval = timestamp.MicroSecondsFrom(testTime);
		
	// make sure its the right time
	TInt intervalMs = interval.Int64();
	if (intervalMs != 0)
		{
		return KErrGeneral;
		}
	
	TReal64 expectedLatitude  = 23.44999552;
	TReal64 temp = aPosition.Latitude() - expectedLatitude;
	if (Abs(temp) > 0.001)
		{
		return KErrGeneral;
		}
		
	TReal64 expectedLongitude = -125;
	temp = aPosition.Longitude() - expectedLongitude;
	if (Abs(temp) > 0.001)
		{
		return KErrGeneral;
		}
		
	TReal32 expectedHorAcc = 7.71561;
	temp = aPosition.HorizontalAccuracy() - expectedHorAcc;
	if (Abs(temp) > 0.001)
		{
		return KErrGeneral;
		}
	
	TReal32 expectedAlt = 19596;
	if (aPosition.Altitude() != expectedAlt)
		{
		return KErrGeneral;
		}
	
	TReal32 expectedVerAcc = 109.6698924;
	temp = aPosition.VerticalAccuracy() - expectedVerAcc;
	if (Abs(temp) > 0.001)
		{
		return KErrGeneral;
		}
	
	return KErrNone;
	}
