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
// outgoing SUPL POS with RRLP ASSISTANCE DATA ack payload
// 
//

/**
 @file suplrrlpasn4step.cpp
 @internalTechnology
*/
#include "suplrrlpasn4step.h"
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


CSuplRrlpAsn4Step::~CSuplRrlpAsn4Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn4Step::CSuplRrlpAsn4Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn4Step);
	}

TVerdict CSuplRrlpAsn4Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn4Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestEncodeSuplPosRrlpAssistanceDataAckL());
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

TVerdict CSuplRrlpAsn4Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}



void CSuplRrlpAsn4Step::TestEncodeSuplPosRrlpAssistanceDataAckL()
	{
	// check for any returned error code
	TInt err = KErrNone;

	// create a CSuplSessionId and populate
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId);
	
	// VELOCITY (optional)
	
	// POSITIONING PAYLOAD (mandatory)
	CRrlpAssistanceDataAck* assistDataAck = CRrlpAssistanceDataAck::NewL();
	CleanupStack::PushL(assistDataAck);

	// REFERENCE (inc Rel-5 Extended Reference)
	TRrlpReference reference;
	reference.aRefNum = 5; 
	reference.aRel5EntendedRefPresent = ETrue;
	reference.aRel5SmlcCode = 39;
	reference.aRel5TransactionId = 13337;
	err = assistDataAck->SetReference(reference);
	LeaveIfErrorL(err);
	
	// Create the SUPL POS Message, and pass ownership of the payload.
	CSuplPos* suplPos = CSuplPos::NewL(ETrue);
	CleanupStack::Pop(assistDataAck);
	suplPos->SetPosPayload(assistDataAck);
	assistDataAck = NULL;
 	CleanupStack::PushL(suplPos);
	
	// populate the remaining SUPL MESSAGE components	
 	err = suplPos->SetSessionId(*sessionId);					// (mandatory)
	LeaveIfErrorL(err);
 	//err = suplPos->SetVelocity(TPositionInfoBase& aPosInfo);	// (optional)
 	//LeaveIfErrorL(err);

	// encode it to the output buffer
	TPtr8 encodePtr(encodeBuffer.Des());
	err = suplPos->EncodeToL(encodePtr);
	LeaveIfErrorL(err);
	
	CleanupStack::PopAndDestroy(suplPos);
	CleanupStack::PopAndDestroy(sessionId);
	}

