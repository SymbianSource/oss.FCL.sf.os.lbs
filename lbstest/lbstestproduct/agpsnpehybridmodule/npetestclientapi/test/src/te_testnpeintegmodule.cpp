// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

// System
#include <ecom/ecom.h> 

// Project
#include "lbsdevloggermacros.h"
#include <lbs/lbsassistancedatabuilderset.h>

// LBS Test
#include <lbs/test/tlbsutils.h>
#include "te_testnpeintegmodule.h"

_LIT(KTestNpeIntegModule, "TestNpeInteg");
_LIT(KTimeFormat, "%H:%T:%S.%C");

const TReal32						KTargetAccuracy		= 100;		// 100 metres
const TTimeIntervalMicroSeconds32	KFirstRequestAfter	= 1000000;	//1s

//************************************************************************************************************
//   CTestManagerBase
//************************************************************************************************************
//------------------------------------------------------------------------------------------------------------
CTe_TestNpeIntegModule::CTe_TestNpeIntegModule()
	:CTimer(CActive::EPriorityHigh),
	iShutdown(EFalse),
	iInitDone(EFalse),
	iUpdateReceived(EFalse),
	iRequestCounter(0),
	iExpectNanPosition(EFalse),
	iTargetTime(0),
	iModule(NULL)
	{
	}

//------------------------------------------------------------------------------------------------------------
void CTe_TestNpeIntegModule::ConstructL()
	{
	CTimer::ConstructL();

	iModule = CNpeApi::NewL(this);

	// create the test channel handler
	iAGpsHandler = CT_LbsAGpsHandler::NewL(this);

	After(KFirstRequestAfter);
	}

//------------------------------------------------------------------------------------------------------------
CTe_TestNpeIntegModule::~CTe_TestNpeIntegModule()
	{
	delete iAGpsHandler;
	iAGpsHandler = NULL;

	delete iModule;
	//REComSession::FinalClose();
	iModule = NULL;
	}

//------------------------------------------------------------------------------------------------------------
void CTe_TestNpeIntegModule::CompleteSelf(TInt aReason)
	{
	TRequestStatus* pStat = &iStatus;
	   
	User::RequestComplete(pStat, aReason);
	SetActive();
	}

//------------------------------------------------------------------------------------------------------------	
void CTe_TestNpeIntegModule::RequestLocation()
	{
	TInt err = KErrNone;

	// Set quality here
	iQuality.SetMinHorizontalAccuracy(KTargetAccuracy);
	iQuality.SetMinVerticalAccuracy(KTargetAccuracy);

	iTargetTime.UniversalTime();
	
	// Logging
	TBuf<100> tTimeStr;
	TRAP(err, iTargetTime.FormatL(tTimeStr, KTimeFormat);)
	if(err)
		User::Panic(KTestNpeIntegModule, err);
	
	LBSLOG(ELogP1, "");
	LBSLOG3(ELogP5, "NEW TEST: Requesting Location[%d]. Target Time: %S.\n", iRequestCounter, &tTimeStr);
	
	// Request
	iModule->PositionRequest(iTargetTime, 0, KTargetAccuracy, KTargetAccuracy);
	iUpdateReceived = EFalse;
	iRequestCounter++;	
	}
	
//------------------------------------------------------------------------------------------------------------	
void CTe_TestNpeIntegModule::CheckLocationUpdateReceived()
	{
	// Was update received
	if (iUpdateReceived == EFalse)
		{
		LBSLOG_ERR(ELogP5, "NEW TEST: Location Update not received\n");
		
		TInt err = KErrNone;
		
		TRAP(err, User::Leave(KErrGeneral);)
		if(err)
			User::Panic(KTestNpeIntegModule, err);
		}
		
	iUpdateReceived = EFalse;
	}


//************************************************************************************************************
//   From MLbsLocationSourceGpsObserver
//************************************************************************************************************
//------------------------------------------------------------------------------------------------------------
void CTe_TestNpeIntegModule::PositionUpdate(TInt aStatus, const TPositionInfoBase* aPosInfoArray[], TInt aNumItems, const TTime& aTargetTime)
	{
	LBSLOG(ELogP1, "NEW TEST: <Location Update Received");

	if (iExpectNanPosition)
	{
		iExpectNanPosition = EFalse;
		return;
	}

	iUpdateReceived = ETrue;
	
	TInt err = KErrNone;
	const TPositionSatelliteInfo* posInfo = NULL;

	// No measurements present (unless mode is EAssisted). Position only available at index 0.
	if (aNumItems == 1) 
		{
		posInfo = reinterpret_cast<const TPositionSatelliteInfo*>(aPosInfoArray[0]);
		}
	// Measurements should be present.
	else if (aNumItems == 2)
		{
        const TPositionGpsMeasurementInfo* measurement = NULL;
        measurement = reinterpret_cast<const TPositionGpsMeasurementInfo*>(aPosInfoArray[0]);
        posInfo = reinterpret_cast<const TPositionSatelliteInfo*>(aPosInfoArray[1]);
        
		// Check measurment is correct type.
		if (measurement->PositionClassType() != EPositionGpsMeasurementInfoClass)
			{
			LBSLOG_ERR2(ELogP5, "Measurement type is: %d\n", measurement->PositionClassType());
			User::Panic(KTestNpeIntegModule, KErrGeneral);
			}
		}
	else
		{
		err = KErrNotSupported;
		}

	// Update err check
	if (err)
		{
		LBSLOG_ERR2(ELogP5, "Update arr processing err is: %d\n", err);			
		User::Panic(KTestNpeIntegModule, err);
		}

	// Check posinfo is correct type.
	if (!(posInfo->PositionClassType() & EPositionSatelliteInfoClass) && !(iGpsMode == ETerminalAssisted))
		{
		LBSLOG_ERR2(ELogP5, "Position type is: %d\n", posInfo->PositionClassType());
		User::Panic(KTestNpeIntegModule, KErrGeneral);
		}

	// Accuracy check
	TPosition position;
	posInfo->GetPosition(position);
	TReal32 horAccur = position.HorizontalAccuracy();
	TReal32 vertAccur = position.VerticalAccuracy();

	// Logging
	TInt minHorAccurInt = iQuality.MinHorizontalAccuracy();
	TInt minVertAccurInt = iQuality.MinVerticalAccuracy();

	LBSLOG5(ELogP5, "Location Update Received. Accuracy %d/%d [%d/%d]\n", horAccur, vertAccur, minHorAccurInt, minVertAccurInt);

	// Status check.
	if (aStatus != KErrNone)
		{
		LBSLOG_ERR2(ELogP5, "aStatus is: %d\n", aStatus);
		User::Panic(KTestNpeIntegModule, aStatus); //panic should be KErrGeneral aStatus is currently return for debug purposes
		}

	// Target time check.
	if (aTargetTime != iTargetTime)
		{
		LBSLOG_ERR(ELogP5, "NEW TEST: aTargetTime != iTargetTime\n");
		User::Panic(KTestNpeIntegModule, KErrGeneral);
		}

	// Quality check and end of validation.
	if (((Math::IsNaN(horAccur) && Math::IsNaN(vertAccur))) ||
		((horAccur <= iQuality.MinHorizontalAccuracy()) &&
		(vertAccur <= iQuality.MinVerticalAccuracy())))
		{
		Cancel();
		CompleteSelf();
		}

	}

//------------------------------------------------------------------------------------------------------------
void CTe_TestNpeIntegModule::GpsStatus(TNpeGpsStatus aGpsStatus)
	{
	(void)aGpsStatus;
	
	LBSLOG2(ELogP5, "NEW TEST: <Device Status Update Received: %d", aDeviceStatus);
	}
	
void CTe_TestNpeIntegModule::GpsQualityStatus(TNpeGpsQualityStatus aQualityStatus)
	{
	(void)aQualityStatus;
	LBSLOG2(ELogP5, "NEW TEST: <Device Status Update Received: %d", aQualityStatus);
	}


//------------------------------------------------------------------------------------------------------------
void CTe_TestNpeIntegModule::AssistanceDataRequest(TLbsAsistanceDataGroup aDataItemMask)
	{
	LBSLOG(ELogP1, "NEW TEST: <Request Assistance Data Received");

	// Only support reference time.
	if (aDataItemMask == EAssistanceDataReferenceTime)
		{
        RLbsAssistanceDataReaderSet  aDataReader;
		iModule->DeliverAssistanceData(EAssistanceDataReferenceTime, aDataReader);
		}
		
	else if (aDataItemMask == 0)
		{
		return;		
		}
		
	else
		{
		//Should we raise error????
		}
	}

//------------------------------------------------------------------------------------------------------------
void CTe_TestNpeIntegModule::Shutdown()
    {
    iShutdown = ETrue;
    
    Cancel();
    CompleteSelf();
    }


//************************************************************************************************************
//   From MT_ResponseObserver
//************************************************************************************************************
//------------------------------------------------------------------------------------------------------------
void CTe_TestNpeIntegModule::ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse)
	{
	LBSLOG(ELogP1, "NEW TEST: <Process AGps Response Message Received");

	if (TT_LbsAGpsResponseMsg::EModuleResponseOk != aResponse)
		{
		CompleteSelf(KErrArgument);
		}
	else
		{
		CompleteSelf(KErrNone);
		}
	}
