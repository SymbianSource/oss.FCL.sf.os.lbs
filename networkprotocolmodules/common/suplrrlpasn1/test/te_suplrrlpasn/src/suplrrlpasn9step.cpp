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
// outgoing SUPL END
// 
//

/**
 @file suplrrlpasn9step.cpp
 @internalTechnology
*/
#include "suplrrlpasn9step.h"
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


CSuplRrlpAsn9Step::~CSuplRrlpAsn9Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn9Step::CSuplRrlpAsn9Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn9Step);
	}

TVerdict CSuplRrlpAsn9Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn9Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestEncodeSuplEndL());
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

TVerdict CSuplRrlpAsn9Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}


void CSuplRrlpAsn9Step::TestEncodeSuplEndL()
	{
	// check for any returned error code
	TInt err = KErrNone;
	
	// create a CSuplSessionId and populate
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId);

	// POSITION (optional)
	TPositionInfo posInfo;
	TPosition position;
	position.SetCoordinate(23.456, 45.678, 25.34);
	position.SetHorizontalAccuracy(9.345);
	position.SetVerticalAccuracy(12.5);
	TDateTime dateTime(2008, EMarch, 12, 11, 54, 23, 00);
	TTime time(dateTime);
	position.SetTime(time);
	//posInfo.SetModuleId();
	posInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal);
	posInfo.SetPositionModeReason(EPositionModeReasonNone);
	posInfo.SetUpdateType(EPositionUpdateUnknown);
	posInfo.SetPosition(position);
	
	// STATUS CODE (optional)
	TSuplStatusCode status = ESuplStatusAuthSuplinitFailure;

	// VER (conditional - MT-LR only)
	_LIT8(ver, "abcdefgh");
	
	// Create the SUPL END Message
	CSuplEnd* suplEnd = CSuplEnd::NewL(ETrue);
 	CleanupStack::PushL(suplEnd);
 	
 	err = suplEnd->SetSessionId(*sessionId);
	LeaveIfErrorL(err);
	err = suplEnd->SetPosition(posInfo);
 	LeaveIfErrorL(err);
	err = suplEnd->SetStatusCode(status);
	LeaveIfErrorL(err);
	err = suplEnd->SetVer(ver);
	LeaveIfErrorL(err);

	// encode it to the output buffer
	TPtr8 encodePtr(encodeBuffer.Des());
	err = suplEnd->EncodeToL(encodePtr);
	LeaveIfErrorL(err);
	
	// test decode
	TPtrC8 decodePtr(encodePtr);
	err = iSpoofServer->DecodeL(&decodePtr);
	LeaveIfErrorL(err);

	CleanupStack::PopAndDestroy(suplEnd);
	CleanupStack::PopAndDestroy(sessionId);
	}
