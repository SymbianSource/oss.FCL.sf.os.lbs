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
 @file suplrrlpasn0step.cpp
 @internalTechnology
*/
#include "suplrrlpasn0step.h"
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


CSuplRrlpAsn0Step::~CSuplRrlpAsn0Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn0Step::CSuplRrlpAsn0Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn0Step);
	}

TVerdict CSuplRrlpAsn0Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn0Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestEncodeSuplStartL());
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

TVerdict CSuplRrlpAsn0Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}



void CSuplRrlpAsn0Step::TestEncodeSuplStartL()
	{
	// check for any returned error code
	TInt err = KErrNone;

	// create a CSuplSessionId and populate the SET session ID only
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId, ETrue);
	
	// SET positioning capabilities
	TLbsNetPosCapabilities caps; // note protocols parameter is ignored so do not set
	TInt numMethods = 3;
	TLbsNetPosMethod posMethods[KLbsMaxNetPosMethods];
	posMethods[0].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyTerminal | TPositionModuleInfo::ETechnologyAssisted);
	posMethods[1].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyNetwork | TPositionModuleInfo::ETechnologyAssisted);
	posMethods[2].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyTerminal);
	err = caps.SetPosMethods(posMethods, numMethods);
	LeaveIfErrorL(err);
	
	// location id
	CSuplLocationId* locationId = CSuplLocationId::NewL(ESuplLocationTypeGsm);
	CleanupStack::PushL(locationId);
	locationId->iGsmCellInfo->iRefMCC = 123;   //  (0..999),   -- Mobile Country Code
	locationId->iGsmCellInfo->iRefMNC = 456;   //  (0..999),   -- Mobile Network Code
	locationId->iGsmCellInfo->iRefLAC = 23456; //  (0..65535), -- Location area code
	locationId->iGsmCellInfo->iRefCI  = 65432; //  (0..65535), -- Cell identity
	locationId->iGsmCellInfo->iNMR    = 2;     //  (0..15)     --  no. NMR elements
	locationId->iGsmCellInfo->iNmrElements[0].iARFCN = 357; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[0].iBSIC  = 24;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[0].iRxLev = 63;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iARFCN = 631; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[1].iBSIC  = 01;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iRxLev = 12;  // (0..63)
	locationId->iGsmCellInfo->iTA     = 33;     //  (0..255) OPTIONAL, -- Timing Advance
	
	// pos request quality
	TLbsNetPosRequestQuality quality;
	TReal32 horizontalAccuracy = 57.3;
	TReal32 verticalAccuracy = 75.8;
	TTimeIntervalMicroSeconds maxFixTime(30 * 1000 * 1000);
	TTimeIntervalMicroSeconds maxFixAge(0);
	quality.SetMinHorizontalAccuracy(horizontalAccuracy);
	quality.SetMinVerticalAccuracy(verticalAccuracy);
	quality.SetMaxFixTime(maxFixTime);
	quality.SetMaxFixAge(maxFixAge);
		
	// Create the SUPL START Message
	CSuplStart* suplStart = CSuplStart::NewL();
	CleanupStack::PushL(suplStart);

	// Populate using the parameters above
	err = suplStart->SetSessionId(*sessionId);
	LeaveIfErrorL(err);
	err = suplStart->SetCapabilities(caps);
	LeaveIfErrorL(err);
	err = suplStart->SetLocationId(*locationId);
	LeaveIfErrorL(err);
	err = suplStart->SetQoP(quality);
	LeaveIfErrorL(err);
	
	// encode it to the output buffer
	TPtr8 encodePtr(encodeBuffer.Des());
	err = suplStart->EncodeToL(encodePtr);
	LeaveIfErrorL(err);
	
	// ---test decode and check content---
	TPtrC8 decodePtr(encodePtr);
	err = iSpoofServer->DecodeL(&decodePtr);
	LeaveIfErrorL(err);
	
	CleanupStack::PopAndDestroy(suplStart);
	CleanupStack::PopAndDestroy(locationId);
	CleanupStack::PopAndDestroy(sessionId);
	}


