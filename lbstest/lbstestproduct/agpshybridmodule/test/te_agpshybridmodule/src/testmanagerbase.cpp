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
#include "testmanagerbase.h"

_LIT(KTestMgrBase, "TestManagerBase");
_LIT(KTimeFormat, "%H:%T:%S.%C");

const TReal32						KTargetAccuracy		= 100;		// 100 metres
const TTimeIntervalMicroSeconds32	KFirstRequestAfter	= 1000000;	//1s

//************************************************************************************************************
//   CTestManagerBase
//************************************************************************************************************
//------------------------------------------------------------------------------------------------------------
CTestManagerBase::CTestManagerBase()
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
void CTestManagerBase::ConstructL()
	{
	CTimer::ConstructL();

	iModule = CLbsLocationSourceGpsBase::NewL(*this, KAGpsHybridModuleUid);

	// create the test channel handler
	iAGpsHandler = CT_LbsAGpsHandler::NewL(this);

	After(KFirstRequestAfter);
	}

//------------------------------------------------------------------------------------------------------------
CTestManagerBase::~CTestManagerBase()
	{
	delete iAGpsHandler;
	iAGpsHandler = NULL;

	delete iModule;
	REComSession::FinalClose();
	iModule = NULL;
	}

//------------------------------------------------------------------------------------------------------------
void CTestManagerBase::CompleteSelf(TInt aReason)
	{
	TRequestStatus* pStat = &iStatus;
	   
	User::RequestComplete(pStat, aReason);
	SetActive();
	}

//------------------------------------------------------------------------------------------------------------	
void CTestManagerBase::RequestLocation()
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
		User::Panic(KTestMgrBase, err);
	
	LBSLOG(ELogP1, "");
	LBSLOG3(ELogP5, "NEW TEST: Requesting Location[%d]. Target Time: %S.\n", iRequestCounter, &tTimeStr);
	
	// Request
	iModule->RequestLocationUpdate(iTargetTime, iQuality);
	iUpdateReceived = EFalse;
	iRequestCounter++;	
	}
	
//------------------------------------------------------------------------------------------------------------	
void CTestManagerBase::CheckLocationUpdateReceived()
	{
	// Was update received
	if (iUpdateReceived == EFalse)
		{
		LBSLOG_ERR(ELogP5, "NEW TEST: Location Update not received\n");
		
		TInt err = KErrNone;
		
		TRAP(err, User::Leave(KErrGeneral);)
		if(err)
			User::Panic(KTestMgrBase, err);
		}
		
	iUpdateReceived = EFalse;
	}


//************************************************************************************************************
//   From MLbsLocationSourceGpsObserver
//************************************************************************************************************
//------------------------------------------------------------------------------------------------------------
void CTestManagerBase::UpdateLocation(TInt aStatus, const TPositionInfoBase* aPosInfoArray[], TInt aNumItems, const TTime& aTargetTime)
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


	// Determine is measurements are present. We are making the assumption that if the option arr is empty with have no measurements.
	TInt numUpdates = iGpsOptions.NumOptionItems();

	// No measurements present. Position only available at index 0.
	if (numUpdates == 0) 
		{
		posInfo = reinterpret_cast<const TPositionSatelliteInfo*>(aPosInfoArray[0]);
		numUpdates++;
		}

	// Measurements should be present.
	else if (numUpdates == 2)
		{
		const TPositionGpsMeasurementInfo* measurement = NULL;

		// Determine/access measurement and position data.
		for (TInt i = 0; i < numUpdates; i++)
			{
			TLbsGpsOptionsItem item;

			// Decide which data to deliver, either measurement or position.
			err = iGpsOptions.GetOptionItem(i, item);

			if (err == KErrNone)
				{
				switch (item.PosUpdateType())
					{
					case TLbsGpsOptionsItem::EPosUpdateCalculation:
						{
						posInfo = reinterpret_cast<const TPositionSatelliteInfo*>(aPosInfoArray[i]);
						}
						break;
					
					case TLbsGpsOptionsItem::EPosUpdateMeasurement:
						{
						measurement = reinterpret_cast<const TPositionGpsMeasurementInfo*>(aPosInfoArray[i]);
						}
						break;
					
					default:
						{
						// err or something
						err = KErrNotSupported;
						}
						break;
					}
				}
			}

		// Check measurment is correct type.
		if (measurement->PositionClassType() != EPositionGpsMeasurementInfoClass)
			{
			LBSLOG_ERR2(ELogP5, "Measurement type is: %d\n", measurement->PositionClassType());
			User::Panic(KTestMgrBase, KErrGeneral);
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
		User::Panic(KTestMgrBase, err);
		}

	// Check posinfo is correct type.
	if (!(posInfo->PositionClassType() & EPositionSatelliteInfoClass))
		{
		LBSLOG_ERR2(ELogP5, "Position type is: %d\n", posInfo->PositionClassType());
		User::Panic(KTestMgrBase, KErrGeneral);
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
		User::Panic(KTestMgrBase, KErrGeneral);
		}

	// Number of updates got check.
	if (aNumItems != numUpdates)
		{
		LBSLOG_ERR2(ELogP5, "aNumItems is: %d\n", aNumItems);
		User::Panic(KTestMgrBase, KErrGeneral);
		}

	// Target time check.
	if (aTargetTime != iTargetTime)
		{
		LBSLOG_ERR(ELogP5, "NEW TEST: aTargetTime != iTargetTime\n");
		User::Panic(KTestMgrBase, KErrGeneral);
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
void CTestManagerBase::UpdateDeviceStatus(TPositionModuleStatus::TDeviceStatus aDeviceStatus)
	{
	(void)aDeviceStatus;
	
	LBSLOG2(ELogP5, "NEW TEST: <Device Status Update Received: %d", aDeviceStatus);
	}

//------------------------------------------------------------------------------------------------------------
void CTestManagerBase::UpdateDataQualityStatus(TPositionModuleStatus::TDataQualityStatus aDataQuality)
	{
	(void)aDataQuality;
	
	LBSLOG2(ELogP5, "NEW TEST: <Quality Status Update Received: %d", aDataQuality);
	}

//------------------------------------------------------------------------------------------------------------
TInt CTestManagerBase::GetAssistanceDataItem(TLbsAssistanceDataItem aItem, RDataReaderRootBase& aDataRoot, TTime& aTimeStamp)
	{
	TInt err = KErrNone;
	
	LBSLOG(ELogP1, "NEW TEST: <Get Assistance Data Item Received");

	if (aItem == EAssistanceDataReferenceTime)
		{
		// so we have a builder set
		RLbsAssistanceDataBuilderSet dataBuilderSet;
		
		TRAP(err, dataBuilderSet.OpenL());
		
		// access the ref time builder
		RUEPositioningGpsReferenceTimeBuilder* refTimeBuilder = NULL;

		dataBuilderSet.GetDataBuilder(refTimeBuilder);
		if (refTimeBuilder)
			{
			refTimeBuilder->SetField(TUEPositioningGpsReferenceTime::EGpsWeek, KAssistanceData_RefTime_Week);
			refTimeBuilder->SetField(TUEPositioningGpsReferenceTime::EGpsTow1Msec, KAssistanceData_RefTime_Tow1MSec);

			// Time stamp the data.
			aTimeStamp.UniversalTime();

			refTimeBuilder->SetTimeStamp(aTimeStamp);

			// populate the reader from the builder
			aDataRoot.DataBuffer() = refTimeBuilder->DataBuffer();
			
			dataBuilderSet.Close();
			}
			
		else
			{
			err = KErrNoMemory;	
			}
		}
	
	else
		{
		err = KErrNotSupported;		
		}	


	return err;
	}


//------------------------------------------------------------------------------------------------------------
TInt CTestManagerBase::GetAssistanceDataItemTimeStamp(TLbsAssistanceDataItem /*aItem*/, TTime& /*aTimeStamp*/)
	{
	LBSLOG(ELogP1, "NEW TEST: <Get Assistance Data Item Time Stamp Received");

	return KErrNotSupported;
	}

//------------------------------------------------------------------------------------------------------------
void CTestManagerBase::RequestAssistanceData(TLbsAsistanceDataGroup aDataItemMask)
	{
	LBSLOG(ELogP1, "NEW TEST: <Request Assistance Data Received");

	// Only support reference time.
	if (aDataItemMask == EAssistanceDataReferenceTime)
		{
		iModule->AssistanceDataEvent(KErrNone, EAssistanceDataReferenceTime);
		}
		
	else if (aDataItemMask == 0)
		{
		return;		
		}
		
	else
		{
		iModule->AssistanceDataEvent(KErrNotSupported, aDataItemMask);
		}
	}

//------------------------------------------------------------------------------------------------------------
void CTestManagerBase::Shutdown()
	{
	LBSLOG(ELogP1, "NEW TEST: <Shutdown Received");

	iShutdown = ETrue;
	
	Cancel();
	CompleteSelf();
	}

//************************************************************************************************************
//   From MT_ResponseObserver
//************************************************************************************************************
//------------------------------------------------------------------------------------------------------------
void CTestManagerBase::ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse)
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
