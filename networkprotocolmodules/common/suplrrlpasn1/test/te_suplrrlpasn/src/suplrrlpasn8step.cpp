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
// outgoing SUPL POS with RRLP PROTOCOL ERROR payload
// 
//

/**
 @file suplrrlpasn8step.cpp
 @internalTechnology
*/
#include "suplrrlpasn8step.h"
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


CSuplRrlpAsn8Step::~CSuplRrlpAsn8Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn8Step::CSuplRrlpAsn8Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn8Step);
	}

TVerdict CSuplRrlpAsn8Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn8Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestEncodeSuplPosRrlpProtocolErrorL());
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

TVerdict CSuplRrlpAsn8Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}


void CSuplRrlpAsn8Step::TestEncodeSuplPosRrlpProtocolErrorL()
	{
	// check for any returned error code
	TInt err = KErrNone;

	// VELOCITY (optional)
	
	// POSITIONING PAYLOAD (mandatory)
	CRrlpProtocolError* protocolError = CRrlpProtocolError::NewL(ETrue);
	CleanupStack::PushL(protocolError);

	// REFERENCE (inc Rel-5 Extended Reference)
	TRrlpReference reference;
	reference.aRefNum = 5; 
	reference.aRel5EntendedRefPresent = ETrue;
	reference.aRel5SmlcCode = 39;
	reference.aRel5TransactionId = 13337;
	err = protocolError->SetReference(reference);
	LeaveIfErrorL(err);

	// ERROR CAUSE
	TRrlpErrorCode aErrorCode = ERrlpErrorUnknowReferenceNumber;
	protocolError->SetProtocolError(aErrorCode);
	
	// Create the SUPL POS Message, and pass ownership of the payload.
	CSuplPos* suplPos = CSuplPos::NewL(ETrue);
	CleanupStack::Pop(protocolError);
	suplPos->SetPosPayload(protocolError);
	protocolError = NULL;
 	CleanupStack::PushL(suplPos);
	
	// populate the remaining SUPL MESSAGE components	
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId, EFalse);
	
 	err = suplPos->SetSessionId(*sessionId);
	LeaveIfErrorL(err);

	// encode it to the output buffer
	TPtr8 encodePtr(encodeBuffer.Des());
	err = suplPos->EncodeToL(encodePtr);
	LeaveIfErrorL(err);
	
	// test decode
	TPtrC8 decodePtr(encodePtr);
	err = iSpoofServer->DecodeL(&decodePtr);
	LeaveIfErrorL(err);

	CleanupStack::PopAndDestroy(sessionId);
	CleanupStack::PopAndDestroy(suplPos);
	}
