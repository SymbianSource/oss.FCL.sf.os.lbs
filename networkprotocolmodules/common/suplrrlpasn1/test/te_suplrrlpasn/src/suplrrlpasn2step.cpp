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
// outgoing SUPL POS INIT
// 
//

/**
 @file suplrrlpasn2step.cpp
 @internalTechnology
*/
#include "suplrrlpasn2step.h"
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


CSuplRrlpAsn2Step::~CSuplRrlpAsn2Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn2Step::CSuplRrlpAsn2Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn2Step);
	}

TVerdict CSuplRrlpAsn2Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn2Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestEncodeSuplPosInitL());
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

TVerdict CSuplRrlpAsn2Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}


void CSuplRrlpAsn2Step::TestEncodeSuplPosInitL()
	{
	// check for any returned error code
	TInt err = KErrNone;
	
	// create a CSuplSessionId and populate
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	PopulateSessionId(sessionId);

	// SET positioning capabilities (mandatory)
	TLbsNetPosCapabilities caps;
	TInt numMethods = 3;
	TLbsNetPosMethod posMethods[KLbsMaxNetPosMethods];
	posMethods[0].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyTerminal | TPositionModuleInfo::ETechnologyAssisted);
	posMethods[1].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyNetwork  | TPositionModuleInfo::ETechnologyAssisted);
	posMethods[2].SetPosMethod(KLbsPositioningMeansGps, TPositionModuleInfo::ETechnologyTerminal);
	err = caps.SetPosMethods(posMethods, numMethods);
	LeaveIfErrorL(err);
	
	// requested assistance data (optional)
	TLbsAsistanceDataGroup dataReqMask;
	dataReqMask = EAssistanceDataNone;
	dataReqMask = EAssistanceDataAquisitionAssistance |
			   	  EAssistanceDataNavigationModel |
			   	  EAssistanceDataReferenceLocation;
	
	// location id (mandatory)
	CSuplLocationId* locationId = CSuplLocationId::NewL(ESuplLocationTypeGsm);
	CleanupStack::PushL(locationId);
	locationId->iGsmCellInfo->iRefMCC  = 123;   //  (0..999),   -- Mobile Country Code
	locationId->iGsmCellInfo->iRefMNC = 456;   //  (0..999),   -- Mobile Network Code
	locationId->iGsmCellInfo->iRefLAC = 24680; //  (0..65535), -- Location area code
	locationId->iGsmCellInfo->iRefCI  = 13579; //  (0..65535), -- Cell identity
	locationId->iGsmCellInfo->iNMR    = 2;     //  (0..15)     --  no. NMR elements
	locationId->iGsmCellInfo->iNmrElements[0].iARFCN = 357; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[0].iBSIC  = 24;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[0].iRxLev = 63;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iARFCN = 631; // (0..1023)
	locationId->iGsmCellInfo->iNmrElements[1].iBSIC  = 01;  // (0..63)
	locationId->iGsmCellInfo->iNmrElements[1].iRxLev = 12;  // (0..63)
	locationId->iGsmCellInfo->iTA     = 33;     //  (0..255) OPTIONAL, -- Timing Advance

	
	// position (optional)
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
	
	// SUPL POS (optional) 
	// not supported when using RRLP positioning protocol
	
	// VER
	_LIT8(ver, "abcdefgh");
	
	// Create the SUPL POS INIT Message
	CSuplPosInit* suplPosInit = CSuplPosInit::NewL();
 	CleanupStack::PushL(suplPosInit);
 	
 	err = suplPosInit->SetSessionId(*sessionId);
	LeaveIfErrorL(err);
 	err = suplPosInit->SetCapabilities(caps); 					// (mandatory)
 	LeaveIfErrorL(err);
	err = suplPosInit->SetRequestedAssistanceData(dataReqMask); // (optional)
	LeaveIfErrorL(err);
	err = suplPosInit->SetLocationId(*locationId);	 			// (mandatory)
	LeaveIfErrorL(err);
	err = suplPosInit->SetPosition(posInfo); 					// (optional)
	LeaveIfErrorL(err);
	err = suplPosInit->SetVer(ver);								// (conditional - MT-LR only)
	LeaveIfErrorL(err);

	// encode it to the output buffer
	TPtr8 encodePtr(encodeBuffer.Des());
	err = suplPosInit->EncodeToL(encodePtr);
	LeaveIfErrorL(err);
	
	// ---test decode and check content---
	TPtrC8 decodePtr(encodePtr);
	err = iSpoofServer->DecodeL(&decodePtr);
	LeaveIfErrorL(err);
	
		
	CleanupStack::PopAndDestroy(suplPosInit);
	CleanupStack::PopAndDestroy(locationId);
	CleanupStack::PopAndDestroy(sessionId);
	}

