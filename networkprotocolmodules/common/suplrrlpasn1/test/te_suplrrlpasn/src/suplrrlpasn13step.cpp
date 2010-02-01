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
// outgoing SUPL START
// 
//

/**
 @file suplrrlpasn13step.cpp
 @internalTechnology
*/
#include "suplrrlpasn13step.h"
#include "te_suplrrlpasnsuitedefs.h"

// LBS
#include <lbs/lbsnetcommon.h>
#include <lbs/lbsassistancedatabase.h>
#include <lbspositioninfo.h>
#include <lbs/lbsgpsmeasurement.h>

// items under test
#include "suplmessagebase.h"
#include "suplasn1decoder.h"
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


CSuplRrlpAsn13Step::~CSuplRrlpAsn13Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn13Step::CSuplRrlpAsn13Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn0Step);
	}

TVerdict CSuplRrlpAsn13Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn13Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, doTestL());
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

TVerdict CSuplRrlpAsn13Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}



void CSuplRrlpAsn13Step::doTestL()
	{
	// check for any returned error code
	TInt err = KErrNone;
	
	err = iSpoofServer->TestAsn1UtcTime();
	
	if (err != KErrNone)
		{
		User::Leave(err);
		}

	}


