/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of the locationengine API
*
*/


//  Include Files
#include <lbs/test/ctdumper.h>

// Lbs includes
#include <LbsPositionInfo.h>
#include <lbs/lbsextendedsatellite.h>
#include <lbs/test/lbsparamlogger.h>
#include "LbsInternalInterface.h"
#include "lbsdevloggermacros.h"

#include <lbs/test/tlbsdefs.h>
#include "clocationengineapiimpl.h"

#define __ASSERT_ALWAYSX(c,p) (void)((c)||(RDebug::Printf("Assert at line %d in file %s ",__LINE__,__FILE__),p,0));

const TInt KAssistanceDataTimeDelay = 1000000;
const TInt KDelayUpdateTimeDelay = 500000;

const TInt KExpectedNumOfEntries = 2;		// Expected number of update items to report when in hybird mode.
const TInt KExtraUpdates = 2;				// Number of additional updates when in multi-update mode. NOTE: may allow user to update, via option.
const TInt KIntervalOffset = 1000000;		// 1 second to ensure to ensure updates are not delivered too close to either 'target time' or 'max fix time'.

const TInt KPosUpdateTimerId = 1;
const TInt KDelayUpdateTimerId = 2;
const TInt KQuickPositionUpdate = 1000;

const TInt KLbsModuleNumOfUpdates = 3;  // Can't be greater than 16

#define POS_LAT                         49.2
#define POS_LONG                        3.5
#define POS_ALT                         50.0
#define POS_HORZ_ACCURACY               2
#define POS_VERT_ACCURACY               3
#define SPEED                           26.0
#define VERTICAL_SPEED                  20.0
#define HEADING                         25.0
#define COURSE                          30.0
#define SPEED_ACCURACY                  2.0
#define VERTICAL_SPEED_ACCURACY         3.0
#define HEADING_ACCURACY                10.0    
#define COURSE_ACCURACY                 4.0
//TGpsTimingMeasurementData
#define GPS_TIMING_OF_CELL_MsPart       16383                          
#define GPS_TIMING_OF_CELL_LsPart       4294967295 
#define REFERENCE_IDENTITY              511
#define SFN                             4095
//TDetailedErrorReport
#define SD_OF_LONG_ERROR                5.0 
#define SD_OF_LAT_ERROR                 6.0
#define SD_OF_ALT_ERROR                 7.0 
#define SD_OF_SEMI_MAJOR_AXIS_ERROR     8.0
#define SD_OF_SEMI_MINOR_AXIS_ERROR     9.0
#define ORIEN_OF_SEMI_MAJOR_AXIS_ERROR  10.0
#define RMS_VAL_OF_SD_OF_RANGE          11.0

#define GEOIDAL_SEPARATION              12.0
#define MAGNETIC_VARIATION              13.0
#define COURSE_OVER_GROUND_MAGNETIC     14.0


CNpeApiImpl& CNpeApiImpl::NpeApiImpl()
    {
    RDebug::Print(_L("CNpeApiImpl::NpeApiImpl"));
    CNpeApiImpl* self = static_cast<CNpeApiImpl*>(Dll::Tls());
    // temp change jcm __ASSERT_DEBUG(self, User::Invariant());
    __ASSERT_ALWAYSX(self, User::Invariant());
  
    
    return *self;
    }
    
void CNpeApiImpl::InstantiateL(MNpeObserver* aObserver)
    {
    RDebug::Print(_L("CNpeApiImpl::InstantiateL"));
    CNpeApiImpl* self = static_cast<CNpeApiImpl*>(Dll::Tls());

    // TEMP CHANGE jcm  __ASSERT_DEBUG(!self, User::Invariant());
    __ASSERT_ALWAYSX(!self, User::Invariant());
    self = new(ELeave) CNpeApiImpl(aObserver);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    Dll::SetTls(self);
    }

void CNpeApiImpl::Cleanup()
    {
    RDebug::Print(_L("CNpeApiImpl::Cleanup"));
    CNpeApiImpl* self = static_cast<CNpeApiImpl*>(Dll::Tls());
    // TEMP CHANGE jcm __ASSERT_DEBUG(self, User::Invariant());
    __ASSERT_ALWAYSX(self, User::Invariant());

    
    self->DataCleanup();
    delete self;
    Dll::FreeTls();
    }

void CNpeApiImpl::DataCleanup()
    {
    iUpdateArr.ResetAndDestroy();

    iTHHandler->Cancel();
    delete iTHHandler;
    
    delete iDelayUpdateTimer;
    delete iPosUpdateTimer;
    }

/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         RNpeApi
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
CNpeApiImpl::CNpeApiImpl(MNpeObserver* aObservers): iGPSModeNotSupported(EFalse)
    {
    iNpeObserver = aObservers;
    }

//=============================================================================
//  Global methods
//=============================================================================

/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         ConstructL
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
void CNpeApiImpl::ConstructL()
    {
    LBSLOG(ELogP1, "CNpeApiImpl::ConstructL()\n");

    // Create position update and assistance data timers.
    iPosUpdateTimer = CT_LbsTimerUtils::NewL(this, KPosUpdateTimerId);
    iDelayUpdateTimer = CT_LbsTimerUtils::NewL(this, KDelayUpdateTimerId);  

    // Create the AGPS2TH channel handler to listen for incoming data.  
    iTHHandler = CT_TestHarnessHandler::NewL(this);

    // Set default error code.
    iError = KErrNone;
    
    // Assistance data processing on.
    SetOption(ELbsHybridModuleOptions_AssistanceDataOn, ETrue);
    
    //Setup the assistance data that we need to request
    ClearAssistanceData();

    // Provide some default update data.
    PopulateUpdateArrayL(); 
    iUpdateArrIndex = 0;

    iNpeObserver->GpsStatus(EGpsReady);

    TInt error = RProperty::Define(KUidSystemCategory, ELbsTestAGpsModuleResetAssistanceDataFlag, RProperty::EInt);
    error = RProperty::Set(KUidSystemCategory, ELbsTestAGpsModuleResetAssistanceDataFlag, ELbsTestAGpsResetAssistanceDataNotReceived);
    
    error = RProperty::Define(KUidSystemCategory, ELbsTestAGpsModuleAssistanceDataRequestFlag, RProperty::EInt);
    error = RProperty::Set(KUidSystemCategory, ELbsTestAGpsModuleAssistanceDataRequestFlag, ELbsTestAGpsModuleAssistanceDataExpected);
    
    error = RProperty::Define(KUidSystemCategory, ELbsTestAGpsModuleClientMode, RProperty::EInt);
    error = RProperty::Set(KUidSystemCategory, ELbsTestAGpsModuleClientMode, KErrNone);

    LBSDUMPNEWLOG();
    }

/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         Version
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
void CNpeApiImpl::Version(TVersion& aNpeVersion)
    {
    aNpeVersion = TVersion(1,0,0);
    }


/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         SetGpsOptions
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
void CNpeApiImpl::SetGpsOptions(TNpeGpsMode aGpsMode, TBool /*aGpsTimeRelationReq*/)
    {
    LBSTESTLOG_METHOD1(INBOUND, "CNpeApiImpl::SetGpsOptions", aGpsMode);
    LBSLOG(ELogP1, "CNpeApiImpl::SetGpsOptions()\n");

    PublishGpsMode(aGpsMode);
    
    iGpsMode = aGpsMode;
    iGPSModeNotSupported = EFalse;
    
    // Determine if assistance data will ever be required.
    switch (iGpsMode)
        {
        case ETerminalBased:
        case ETerminalAssisted:
        case ETerminalBasedAndAssisted:
            {
            // Currently it is possible for the test harness to switch on/off assistance data requesting.
            if (GetOption(ELbsHybridModuleOptions_AssistanceDataOn))
                {
                iAssistanceDataOn = ETrue;
                }
            else
                {
                iAssistanceDataOn = EFalse;
                }   
            }
            break;
        
        case EAutonomousAndTerminalAssisted:
        case EAutonomous:
        default:
            {
            iAssistanceDataOn = EFalse;
            }
        }
    }

/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         AdvisePowerMode
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
void CNpeApiImpl::AdvisePowerMode(TNpePowerMode aPowerMode)
    {
    LBSLOG(ELogP1, "CNpeApiImpl::AdvisePowerMode()\n");

    // NOTE: could report device status on power mode change.
    // TBD:
    (void)aPowerMode;
    }

/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         PositionRequest
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
void CNpeApiImpl::PositionRequest(const TTime& aTargetTime, TInt aMaxFixTime, TInt aHorUncertainty, TInt aVerUncertainty)
    {
    LBSTESTLOG_METHOD4(INBOUND, "CNpeApiImpl::PositionRequest", aTargetTime, aMaxFixTime, aHorUncertainty, aVerUncertainty);
    LBSLOG(ELogP1, "CNpeApiImpl::PositionRequest()\n");
    
    // If there is currently a position request outstanding, cancel it.
    if (iPosUpdateTimer->IsActive())
        {
        iPosUpdateTimer->Cancel();
        }                                   
    if (iDelayUpdateTimer->IsActive())
        {
        iDelayUpdateTimer->Cancel();
        }

    // Response with update at the required target time + any time out period. The time out allows
    // the module to delay return giving a position update, normally it will be set to zero.
    if (aTargetTime == TTime(0))
        {
        iTargetTime.UniversalTime();
        }
    else
        {   
        iTargetTime = aTargetTime + iTimeOut;
        }
        
    // Is there a delay to add on!
    if (iUpdateArrIndex < iUpdateArr.Count())
        {
        TInt delay = iUpdateArr[iUpdateArrIndex]->Delay() * 1000;
        if (delay != 0)
            {
            if(delay < 0)
                {
                iTargetTime -= TTimeIntervalMicroSeconds(-delay);
                }
            else
                {
                iTargetTime += TTimeIntervalMicroSeconds(delay);
                }   
            }
        }
    
    if(iGPSModeNotSupported)
        {
        iTargetTime = KQuickPositionUpdate;
        }

    iPosUpdateTimer->SetTimer(iTargetTime);
    

    // Reset interval update variables is multi-update module option is on.
    if (GetOption(ELbsHybridModuleOptions_MultiUpdatesOn))
        {
        iExtraUpdatesCtr = KExtraUpdates;
        iIntervalDelay = (aMaxFixTime - 2 * KIntervalOffset) / iExtraUpdatesCtr;
        }
    }

/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         CancelPositionRequest
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
void CNpeApiImpl::CancelPositionRequest(void)
    {
    LBSTESTLOG_METHOD(INBOUND, "CNpeApiImpl::CancelLocationRequest");
    
    T_LbsUtils utils;
    utils.IncrementIntegrationModulesCountOfCancels();

    iPosUpdateTimer->CancelTimer();
    iDelayUpdateTimer->CancelTimer();
    }

/*
-----------------------------------------------------------------------------
    Class:          RNpeApi
    Method:         DeliverAssistanceData
    Description:    
    Return Values:  
-----------------------------------------------------------------------------
*/
void CNpeApiImpl::DeliverAssistanceData(TLbsAsistanceDataGroup aDataGroup, RLbsAssistanceDataReaderSet& /* aDataReaderSet*/)
    {
    //LBSTESTLOG_METHOD2(INBOUND, "CNpeApiImpl::DeliverAssistanceData", aType, aBaseReader);
    LBSLOG(ELogP1, "CNpeApiImpl::DeliverAssistanceData()\n");

    //Well, it looks like we just need to check if received requested Assistance Data
    //TODO - Not sure what to do with aDataReaderSet - Ignore for now
    
    if (iAssistanceDataRequested)
        {
        iAssistanceDataRequested = EFalse;
        iAssistanceDataAvailable = ETrue;

        // check the assistance data received was part of what was requested
        if(iOutstandingAssistanceData & aDataGroup != aDataGroup)
        	{
            // NOTE: maybe panic - at the moment log a warning.
            LBSLOG(ELogP1, "CNpeApiImpl - WARNING assistance data did not verify.\n");
        	}

        iOutstandingAssistanceData &= ~aDataGroup;
        
        // Return position now the assistance data is available.
        // Take some time to process the assistance data, before the position is actually returned.
        TTime time;
            
        time.UniversalTime();
        time += TTimeIntervalMicroSeconds(KAssistanceDataTimeDelay);
        time += iTimeOut;
        iDelayUpdateTimer->SetTimer(time);          
        }
    // Unsolicited, mark as data available and don't verify.
    else
        {
        iAssistanceDataAvailable = ETrue;

        // NOTE: may verify correct type.
        }
    }


void CNpeApiImpl::DeleteAssistanceData(TNpeAdTypes aData)
    {
    TLbsAsistanceDataGroup data=0;
    if (aData == ENpeAdAll)
        {
        data = 0xFFFFFFFF;
        }
    else
        {
        if (aData & ENpeAdEphemeris)
            {data |= EAssistanceDataNavigationModel;}
        if (aData & ENpeAdRefTime)
            {data |=EAssistanceDataReferenceTime;}
        if (aData & ENpeAdRefPosition)
            {data |=EAssistanceDataReferenceLocation;}
        if (aData & ENpeAdAlmanac)
            {data |=EAssistanceDataAlmanac;}
        }

    //For test module, we only care about EAssistanceDataReferenceTime
    if(data & EAssistanceDataReferenceTime)
        {
        if(iAssistanceDataAvailable && iDelayUpdateTimer->IsActive())
            { 
            // If the module was waiting to return a position and the reset affects it, then it will
            // start waiting to ask for assistance data
            iDelayUpdateTimer->CancelTimer();
            iPosUpdateTimer->SetTimer(iTargetTime);
            }
        
        // indicate that we need the full set of assistance data now
        ClearAssistanceData();
        }
    //Setup the required assistance data here
    RProperty::Set(KUidSystemCategory, ELbsTestAGpsModuleResetAssistanceDataFlag, ELbsTestAGpsResetAssistanceDataReceived);
    }

void CNpeApiImpl::SetClientUsage(TNpeClientInfo aClientInfo)
    {
    // publish the client usage flag 
    TInt clientInfo = aClientInfo;
    TInt err = RProperty::Set(KUidSystemCategory, ELbsTestAGpsModuleClientMode, clientInfo);
    }

void CNpeApiImpl::PublishGpsMode(const TNpeGpsMode aGpsMode)
    {
    TLbsGpsOptionsArray gpsOptionsArray; 
    TLbsGpsOptions gpsOptions;

    switch(aGpsMode)
        {
        case EAutonomous:
            {
            gpsOptions.SetGpsMode(CLbsAdmin::EGpsAutonomous);
            gpsOptionsArray = reinterpret_cast<const TLbsGpsOptionsArray&>(gpsOptions);
            break;
            }

        case ETerminalAssisted:
            {
            gpsOptions.SetGpsMode(CLbsAdmin::EGpsPreferTerminalAssisted);
            gpsOptionsArray = reinterpret_cast<const TLbsGpsOptionsArray&>(gpsOptions);
            break;
            }

        case ETerminalBased:
            {
            gpsOptions.SetGpsMode(CLbsAdmin::EGpsPreferTerminalBased);
            gpsOptionsArray = reinterpret_cast<const TLbsGpsOptionsArray&>(gpsOptions);
            break;
            }

        case EAutonomousAndTerminalAssisted:
            {
            gpsOptionsArray.SetGpsMode(CLbsAdmin::EGpsAutonomous);
            TLbsGpsOptionsItem item;
            item.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateCalculation);
            gpsOptionsArray.AppendOptionItem(item);
            item.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateMeasurement);
            gpsOptionsArray.AppendOptionItem(item);
            break;
            }

        case ETerminalBasedAndAssisted:
            {
            gpsOptionsArray.SetGpsMode(CLbsAdmin::EGpsPreferTerminalBased);
            TLbsGpsOptionsItem item;
            item.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateCalculation);
            gpsOptionsArray.AppendOptionItem(item);
            item.SetLocUpdateType(TLbsGpsOptionsItem::EPosUpdateMeasurement);
            gpsOptionsArray.AppendOptionItem(item);
            break;
            }

        default:
            {
            gpsOptionsArray.SetGpsMode(CLbsAdmin::EGpsModeUnknown);
            }
        }
    
    TPckgC<TLbsGpsOptionsArray> pckgOptions(gpsOptionsArray);
    RProperty::Set(KUidSystemCategory, ELbsTestAGpsModuleModeChanges, pckgOptions);
    }


void CNpeApiImpl::HandleTimerL(TInt aTimerId, const TTime& aTargetTime)
	{
	if(iGPSModeNotSupported)
		{
		TPositionInfoBase* deliveryArr[1];
		TPositionModuleInfoExtended::TDeviceGpsModeCapabilities deviceCapabilities;
		TInt err = LbsModuleInfo::GetDeviceCapabilities(KLbsGpsLocManagerUid, deviceCapabilities);
		TPositionGpsMeasurementInfo mes;
		TPositionExtendedSatelliteInfo posInfo;
		
		if(err == KErrNone && deviceCapabilities == TPositionModuleInfoExtended::EDeviceGpsModeTerminalAssisted)
			{ // return measurment with KErrNotSupported if the module can only do that
			deliveryArr[0] = &mes;
			}
		else
			{ // return a position with KErrNotSupported
			deliveryArr[0] = &posInfo;			
			}
		iNpeObserver->PositionUpdate(KErrNotSupported, const_cast<const TPositionInfoBase**>(&deliveryArr[0]), 1, iTargetTime);
		return;
		}

	(void)aTargetTime;

	LBSLOG(ELogP1, "CNpeApiImpl::HandleTimerL()\n");
	LBSLOG2(ELogP1, "\t\taTimerId = %d\n", aTimerId);
	LBSLOG2(ELogP1, "\t\tiAssistanceDataOn = %d\n", iAssistanceDataOn);
	LBSLOG2(ELogP1, "\t\tiAssistanceDataRequested = %d\n", iAssistanceDataRequested);
	LBSLOG2(ELogP1, "\t\tiAssistanceDataAvailable = %d\n", iAssistanceDataAvailable);
	LBSLOG(ELogP1, "\t\t\n");
	
	// Our timer fired, handle assistance data request and pos update.
	if (aTimerId == KPosUpdateTimerId)
	{
		// Only request assistance data if it's not available and we have not asked before.
		if ((iAssistanceDataOn) && (!iAssistanceDataAvailable) && (!iAssistanceDataRequested))
			{
			// Now requesting assistance data.
			iAssistanceDataRequested = ETrue;

			// cooment1
			if (!GetOption(ELbsHybridModuleOptions_DisableReqAssistData))
				{
				// Request some simple assistance data.
				iNpeObserver->AssistanceDataRequest(iOutstandingAssistanceData);
				
				//In some of the tests, Assistance Data is not supplied and position is expected.
				//There is no way to notify Test client API of assistance data is not delivered so the code
				//below is a work around for those scenarios - Need to find a better way to do this.
			    RProperty assDataReqProperty;
			    User::LeaveIfError(assDataReqProperty.Attach(KUidSystemCategory, ELbsTestAGpsModuleAssistanceDataRequestFlag));
			    TInt assDataReqStatus;
			    TInt error = assDataReqProperty.Get(assDataReqStatus);
			    if (error == KErrNone && assDataReqStatus == ELbsTestAGpsModuleAssistanceDataNotExpected)
			        {
		            TTime time;
		                            
		            time.UniversalTime();
		            time += TTimeIntervalMicroSeconds(KAssistanceDataTimeDelay);
		            time += iTimeOut;
		            iDelayUpdateTimer->SetTimer(time);
			        }
				}
				
			// Return an initial position. Once the assistance data is delivered, an additional update will be returned
			// (read from the update array).
			if(iGpsMode == ETerminalAssisted )
				{
				ReturnNanMeas();
				}
			else
				{
				ReturnNanPosition();
				}
			}
		else
			{
			// For each location request, we must allways call AssistanceDataRequest if data is required or not,
			// when no data is required use EAssistanceDataNone.
			if (!GetOption(ELbsHybridModuleOptions_DisableReqAssistData))
				{
				//Check to see whether the Dynamic AssistanceData flag is enabled, if it is re-request
				// any assistance data we have not yet received (this is required for SUPL)
				if(GetOption(ELbsHybridModuleOptions_DynamicAssistanceData))
					{
					iNpeObserver->AssistanceDataRequest(iOutstandingAssistanceData);
					}
				else
					{
					iNpeObserver->AssistanceDataRequest(EAssistanceDataNone);
					}
				}

			// Delay before returning the actual position.
			TTime time;
							
			time.UniversalTime();
			time += TTimeIntervalMicroSeconds(KDelayUpdateTimeDelay);
			time += iTimeOut;
			iDelayUpdateTimer->SetTimer(time);
			}
		}

	// Done processing assistance data or delaying the update, return position.
	else if (aTimerId == KDelayUpdateTimerId)
		{
		// Issue an update.
		UpdateLocation();
		
		// If multi-updating is on, re-issue an update at interval time.
		if (GetOption(ELbsHybridModuleOptions_MultiUpdatesOn) && (iExtraUpdatesCtr > 0))
			{
			TTime time;
			
			time.UniversalTime();
			time += iIntervalDelay;
			
			iDelayUpdateTimer->SetTimer(time);
			iExtraUpdatesCtr--;
			}
		}
	}

void CNpeApiImpl::UpdateLocation()
	{
	LBSTESTLOG_METHOD(OUTBOUND, "CNpeApiImpl::UpdateLocation");
	LBSLOG(ELogP1, "CNpeApiImpl::UpdateLocation()\n");

    // If updates available.
	if (iUpdateArrIndex < iUpdateArr.Count())
		{
		TInt err = KErrNone;
		TPositionInfoBase* deliveryArr[2];
		TInt numOfEntries = 1;

		// Configure position info.
		TPositionExtendedSatelliteInfo posInfo = iUpdateArr[iUpdateArrIndex]->Position();

		// Set base class items.
		posInfo.SetModuleId(TUid::Uid(APGS_MODINFO_MODULE_ID));
		posInfo.SetUpdateType(EPositionUpdateGeneral);
		if (iAssistanceDataOn)
			{
			posInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal | TPositionModuleInfo::ETechnologyAssisted);
			}
		else
			{
			posInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal);
			}


		posInfo.SetPositionModeReason(EPositionModeReasonNone);
					
		// Set the position timestamp.
		TPosition position;
		
		posInfo.GetPosition(position);
		position.SetCurrentTime();
		if (iExtraUpdatesCtr) 
			{
			TRealX nan;
			nan.SetNaN();			
			TReal64 nanValue = nan;
			position.SetCoordinate(nanValue, nanValue, nanValue);			
			position.SetAccuracy(nanValue, nanValue);
			}
		posInfo.SetPosition(position);
		
		// Set the satelliteTime.
		TTime timeStamp;
		timeStamp.UniversalTime();
		posInfo.SetSatelliteTime(timeStamp);
		
		// Access the measurement.
		TPositionGpsMeasurementInfo measurement = iUpdateArr[iUpdateArrIndex]->Measurement();
		if (iImmediateMeasurements)
			{
			measurement.SetPositionCalculationPossible(ETrue);
			}
        // Determine how and what to deliver.
        if (iGpsMode == EAutonomousAndTerminalAssisted  || iGpsMode == ETerminalBasedAndAssisted )
            {
            numOfEntries = 2;
            deliveryArr[0] = &measurement;
            deliveryArr[1] = &posInfo;
            }
        else
            {
            numOfEntries = 1;
            if(iGpsMode == ETerminalAssisted ) 
                { // if in terminal assisted then return measurments
                deliveryArr[0] = &measurement;
                }
            else
                { // otherwise return positions
                deliveryArr[0] = &posInfo;
                }
            }

		err = iUpdateArr[iUpdateArrIndex]->Error();

		// Deliver the required measurement and position data.
		iNpeObserver->PositionUpdate(err, const_cast<const TPositionInfoBase**>(&deliveryArr[0]), numOfEntries, iTargetTime);

		// Log update data.
		DumpUpdateToRefFile(const_cast<const TPositionInfoBase**>(&deliveryArr[0]), numOfEntries);

		// Select next update, for future request.
		if (!iExtraUpdatesCtr) 
			{
			iUpdateArrIndex++;
			}
		}

	// We have no more updates to return.
	else
		{
		iNpeObserver->PositionUpdate(KErrGeneral, NULL, 0, iTargetTime);
		}
	}

void CNpeApiImpl::PopulateUpdateArrayL()
    {
    TLbsModuleUpdateItem* update;
    TPositionExtendedSatelliteInfo posInfo;
    TCourse course;
    TPosition pos;
    TPositionGpsMeasurementInfo measureInfo;    
    TPositionGpsMeasurementData measureData;
    TGpsTimingMeasurementData gpsTimingData;
    TDetailedErrorReport detailedErrorReport;
    for (TInt i = 0; i < KLbsModuleNumOfUpdates; i++)
        {
        // Create the update.
        update = new (ELeave) TLbsModuleUpdateItem();
        
        // Set position info.
        pos.SetCoordinate(i + POS_LAT, i + POS_LONG, i + POS_ALT);
        pos.SetAccuracy(POS_HORZ_ACCURACY, POS_VERT_ACCURACY);
        pos.SetCurrentTime();
        posInfo.SetPosition(pos);
        
        // Set Speed and Direction
        course.SetSpeed(SPEED);
        course.SetVerticalSpeed(VERTICAL_SPEED);
        course.SetHeading(HEADING);
        course.SetSpeedAccuracy(SPEED_ACCURACY);
        course.SetVerticalSpeedAccuracy(VERTICAL_SPEED_ACCURACY);
        course.SetHeadingAccuracy(HEADING_ACCURACY);
        course.SetCourse(COURSE);
        course.SetCourseAccuracy(COURSE_ACCURACY);
        posInfo.SetCourse(course);
        
        // Set timing assistance measurement data
        gpsTimingData.SetDataType(TGpsTimingMeasurementData::EGpsTimingDataTypeUtran);
        gpsTimingData.SetNetworkMode(TGpsTimingMeasurementData::ENetworkModeFdd);
        gpsTimingData.SetGPSTimingOfCellMsPart(GPS_TIMING_OF_CELL_MsPart);
        gpsTimingData.SetGPSTimingOfCellLsPart(GPS_TIMING_OF_CELL_LsPart);
        gpsTimingData.SetReferenceIdentity(REFERENCE_IDENTITY);
        gpsTimingData.SetSfn(SFN);
        posInfo.SetGpsTimingData(gpsTimingData);
                
        //Set GNSS Pseudorange Error Statistics 
        detailedErrorReport.SetStanDeviOfLongitudeError(SD_OF_LONG_ERROR);
        detailedErrorReport.SetStanDeviOfLatiitudeError(SD_OF_LAT_ERROR);
        detailedErrorReport.SetStanDeviOfAltitudeError(SD_OF_ALT_ERROR);
        detailedErrorReport.SetStanDeviOfSemiMajorAxisError(SD_OF_SEMI_MAJOR_AXIS_ERROR);
        detailedErrorReport.SetStanDeviOfSemiMinorAxisError(SD_OF_SEMI_MINOR_AXIS_ERROR);
        detailedErrorReport.SetOrientationOfSemiMajorAxisError(ORIEN_OF_SEMI_MAJOR_AXIS_ERROR);
        detailedErrorReport.SetRmsValOfStanDeviOfRange(RMS_VAL_OF_SD_OF_RANGE);
        posInfo.SetDetailedErrorReport(detailedErrorReport);
        // Set Geoidal separation
        posInfo.SetGeoidalSeparation(GEOIDAL_SEPARATION);
        //Set Magnetic variation
        posInfo.SetMagneticVariation(MAGNETIC_VARIATION);
        //Set Course over ground
        posInfo.SetCourseOverGroundMagnetic(COURSE_OVER_GROUND_MAGNETIC);
        update->SetPosition(posInfo);               
        
        // Set measurement info.
        for (TInt j = 0; j < i; j++)
            {
            measureData.SetSatelliteId(j + 1);
            measureData.SetCarrierNoiseRatio((j+1) + 1);
            measureData.SetDoppler((j+1) + 2);
            measureData.SetWholeGpsChips((j+1) + 3);
            measureData.SetFractionalGpsChips((j+1) + 4);
            measureData.SetMultiPathIndicator(TPositionGpsMeasurementData::EMultiPathLow);
            measureData.SetPseudoRangeRmsError((j+1) + 5);

            User::LeaveIfError(measureInfo.AppendMeasurementData(measureData));
            }
        measureInfo.SetGpsTimeOfWeek(i + 1000);
        update->SetMeasurement(measureInfo);
        
        // Set error.
        update->SetError(KErrNone);

        // Add new update to update array.
        iUpdateArr.AppendL(update);

        // Clear structs for next time around.
        measureInfo.ClearMeasurementData();
        }
    }


void CNpeApiImpl::ReturnNanPosition()
	{
	LBSTESTLOG_METHOD(OUTBOUND, "CNpeApiImpl::ReturnNanPosition");
	LBSLOG(ELogP1, "CNpeApiImpl::ReturnNanPosition()\n");

	// Module is in the in-correct state, for this call. Should be in prefer terminal, and non-hybrid mode.
	if (iAssistanceDataOn == EFalse)
		{
		User::Invariant();
		}

	// Configure position.
	TRealX nan;
	nan.SetNaN();
	TReal64 nanValue = nan;
			
	TPosition pos;
	TCourse course;
	TDetailedErrorReport detailedErrorReport;
	TGpsTimingMeasurementData gpsTimingData;
	TPositionExtendedSatelliteInfo posInfo;
		
	pos.SetCoordinate(nanValue, nanValue, nanValue);			
	pos.SetAccuracy(nanValue, nanValue);
	pos.SetCurrentTime();
	posInfo.SetPosition(pos);

	course.SetSpeed(nanValue);
    course.SetVerticalSpeed(nanValue);
    course.SetHeading(nanValue);
    course.SetSpeedAccuracy(nanValue);
    course.SetVerticalSpeedAccuracy(nanValue);
    course.SetHeadingAccuracy(nanValue);
    course.SetCourse(nanValue);
    course.SetCourseAccuracy(nanValue);
    posInfo.SetCourse(course);
    
    // Set timing assistance measurement data
    gpsTimingData.SetDataType(TGpsTimingMeasurementData::EGpsTimingDataTypeUtran);
    gpsTimingData.SetNetworkMode(TGpsTimingMeasurementData::ENetworkModeFdd);
    gpsTimingData.SetGPSTimingOfCellMsPart(nanValue);
    gpsTimingData.SetGPSTimingOfCellLsPart(nanValue);
    gpsTimingData.SetReferenceIdentity(nanValue);
    gpsTimingData.SetSfn(nanValue);
    posInfo.SetGpsTimingData(gpsTimingData);
    
    
    //Set GNSS Pseudorange Error Statistics 
    detailedErrorReport.SetStanDeviOfLongitudeError(nanValue);
    detailedErrorReport.SetStanDeviOfLatiitudeError(nanValue);
    detailedErrorReport.SetStanDeviOfAltitudeError(nanValue);
    detailedErrorReport.SetStanDeviOfSemiMajorAxisError(nanValue);
    detailedErrorReport.SetStanDeviOfSemiMinorAxisError(nanValue);
    detailedErrorReport.SetOrientationOfSemiMajorAxisError(nanValue);
    detailedErrorReport.SetRmsValOfStanDeviOfRange(nanValue);
    posInfo.SetDetailedErrorReport(detailedErrorReport);
    // Set Geoidal separation
    posInfo.SetGeoidalSeparation(nanValue);
    //Set Magnetic variation
    posInfo.SetMagneticVariation(nanValue);
    //Set Course over ground
    posInfo.SetCourseOverGroundMagnetic(nanValue);
	// Configure position info.

	// Set base class items.
	//TODO: Might need to change module id
	posInfo.SetModuleId(TUid::Uid(APGS_MODINFO_MODULE_ID));
	posInfo.SetUpdateType(EPositionUpdateGeneral);
	posInfo.SetPositionMode(TPositionModuleInfo::ETechnologyTerminal | TPositionModuleInfo::ETechnologyAssisted);
	posInfo.SetPositionModeReason(EPositionModeReasonNone);

	// Set the satelliteTime.
	TTime timeStamp;	
	timeStamp.UniversalTime();
	posInfo.SetSatelliteTime(timeStamp);


	// Deliver the position data.
	TInt err = KErrNone;
	TPositionInfoBase* deliveryArr[KExpectedNumOfEntries];
		
	deliveryArr[0] = &posInfo;
	iNpeObserver->PositionUpdate(err, const_cast<const TPositionInfoBase**>(&deliveryArr[0]), 1, iTargetTime);

	// Log update data.
	DumpUpdateToRefFile(const_cast<const TPositionInfoBase**>(&deliveryArr[0]), 1);
	}


void CNpeApiImpl::ReturnNanMeas()
	{
	LBSTESTLOG_METHOD(OUTBOUND, "CNpeApiImpl::ReturnNanMeasurement");
	LBSLOG(ELogP1, "CNpeApiImpl::ReturnNanMeasurement()\n");	
	
	TRealX nan;
	nan.SetNaN();
	TReal64 nanValue = nan;
		
	TPositionGpsMeasurementData measureData;	
	TPositionGpsMeasurementInfo measurement;
	
	measureData.SetSatelliteId(nan);
	measureData.SetCarrierNoiseRatio(nan);
	measureData.SetDoppler(nan);
	measureData.SetWholeGpsChips(nan);
	measureData.SetFractionalGpsChips(nan);
	measureData.SetMultiPathIndicator(TPositionGpsMeasurementData::EMultiPathLow);
	measureData.SetPseudoRangeRmsError(nan);

    TInt err = KErrNone;
	err = measurement.AppendMeasurementData(measureData);
	if(err != KErrNone)
	    {
	    //It might be a good idea to log error : TBD
	    }
	
	measurement.SetGpsTimeOfWeek(nan);

	// Deliver the position data.
	TPositionInfoBase* deliveryArr[KExpectedNumOfEntries];
	deliveryArr[0] = &measurement;

	iNpeObserver->PositionUpdate(err, const_cast<const TPositionInfoBase**>(&deliveryArr[0]), 1, iTargetTime);

	// Log update data.
	DumpUpdateToRefFile(const_cast<const TPositionInfoBase**>(&deliveryArr[0]), 1);
	}

/** Dump update to a log file (useful for test verification required).
*/  
void CNpeApiImpl::DumpUpdateToRefFile(const TPositionInfoBase* aPosInfoArray[], TInt aNumItems)
    {
    LBSDUMP("New Update:\n");   

    for (TInt i = 0; i < aNumItems; i++)
        {
        if (aPosInfoArray[i]->PositionClassType() & EPositionExtendedSatelliteInfoClass)
            {
            LBSDUMP("   TPositionExtendedSatelliteInfo\n"); 
            TPosition position;
            const TPositionExtendedSatelliteInfo* posInfo = reinterpret_cast<const TPositionExtendedSatelliteInfo*>(aPosInfoArray[i]);
            posInfo->GetPosition(position);
            LBSDUMP2("\tHorizontalAccuracy: %f\n", position.HorizontalAccuracy());
            LBSDUMP2("\tVerticalAccuracy: %f\n", position.VerticalAccuracy());
            LBSDUMP2("\tLatitude: %f\n", position.Latitude());
            LBSDUMP2("\tLongitude: %f\n", position.Longitude());
            LBSDUMP2("\tAltitude: %f\n", position.Altitude());
            LBSDUMP2("\tTimeStamp: %d\n", position.Time().Int64());
            TCourse course;
            posInfo->GetCourse(course);
            LBSDUMP2("\tSpeed: %f\n", course.Speed());
            LBSDUMP2("\tVerticalSpeed: %f\n", course.VerticalSpeed());
            LBSDUMP2("\tHeading: %f\n", course.Heading());
            LBSDUMP2("\tCourse: %f\n", course.Course());
            LBSDUMP2("\tSpeedAccuracy: %f\n", course.SpeedAccuracy());
            LBSDUMP2("\tVerticalSpeedAccuracy: %f\n", course.VerticalSpeedAccuracy());
            LBSDUMP2("\tHeadingAccuracy: %f\n", course.HeadingAccuracy());
            LBSDUMP2("\tCourseAccuracy: %f\n", course.CourseAccuracy());
            
            TGpsTimingMeasurementData gpsTimingData;
            posInfo->GetGpsTimingData(gpsTimingData);
            
            LBSDUMP2("\tDataType: %f\n", gpsTimingData.DataType());
            LBSDUMP2("\tNetworkMode: %f\n", gpsTimingData.NetworkMode());
            LBSDUMP2("\tGPSTimingOfCellMsPart: %f\n", gpsTimingData.GPSTimingOfCellMsPart());
            LBSDUMP2("\tGPSTimingOfCellLsPart: %f\n", gpsTimingData.GPSTimingOfCellLsPart());
            LBSDUMP2("\tReferenceIdentity: %f\n", gpsTimingData.ReferenceIdentity());
            LBSDUMP2("\tSfn: %f\n", gpsTimingData.Sfn());
            
                        
            TDetailedErrorReport detailedErrorReport;
            posInfo->GetDetailedErrorReport(detailedErrorReport);
            LBSDUMP2("\tStanDeviOfLongitudeError: %f\n", detailedErrorReport.StanDeviOfLongitudeError());
            LBSDUMP2("\tStanDeviOfLatiitudeError: %f\n", detailedErrorReport.StanDeviOfLatiitudeError());
            LBSDUMP2("\tStanDeviOfAltitudeError: %f\n", detailedErrorReport.StanDeviOfAltitudeError());
            LBSDUMP2("\tStanDeviOfSemiMajorAxisError: %f\n",detailedErrorReport.StanDeviOfSemiMajorAxisError());
            LBSDUMP2("\tStanDeviOfAltitudeError: %f\n", detailedErrorReport.StanDeviOfAltitudeError());
            LBSDUMP2("\tStanDeviOfSemiMajorAxisError: %f\n",detailedErrorReport.StanDeviOfSemiMajorAxisError());
            LBSDUMP2("\tStanDeviOfSemiMinorAxisError: %f\n", detailedErrorReport.StanDeviOfSemiMinorAxisError());
            LBSDUMP2("\tOrientationOfSemiMajorAxisError: %f\n",detailedErrorReport.OrientationOfSemiMajorAxisError());
            LBSDUMP2("\tRmsValOfStanDeviOfRange: %f\n",detailedErrorReport.RmsValOfStanDeviOfRange());
            
            LBSDUMP2("\tGeoidalSeparation: %f\n", posInfo->GeoidalSeparation());
            LBSDUMP2("\tMagneticVariation: %f\n",posInfo->MagneticVariation());
            LBSDUMP2("\tCourseOverGroundMagnetic: %f\n",posInfo->CourseOverGroundMagnetic());
            }                       

        if (aPosInfoArray[i]->PositionClassType() & EPositionGpsMeasurementInfoClass)
            {
            LBSDUMP("   TPositionGpsMeasurementInfo\n");    
            const TPositionGpsMeasurementInfo* measurement = reinterpret_cast<const TPositionGpsMeasurementInfo*>(aPosInfoArray[i]);
            TInt numMeasurements = measurement->NumMeasurements();
            
            for(TInt n = 0; n < numMeasurements; n++)
                {
                TPositionGpsMeasurementData measurementData;
                measurement->GetMeasurementData(n, measurementData);
                LBSDUMP2("      SatelliteId: %d\n", measurementData.SatelliteId());
                LBSDUMP2("\tCarrierNoiseRatio: %d\n", measurementData.CarrierNoiseRatio());
                LBSDUMP2("\tDoppler: %d\n", measurementData.Doppler());
                LBSDUMP2("\tWholeGpsChips: %d\n", measurementData.WholeGpsChips());
                LBSDUMP2("\tFractionalGpsChips: %d\n", measurementData.FractionalGpsChips());
                LBSDUMP2("\tMultiPathIndicator: %d\n", measurementData.MultiPathIndicator());
                LBSDUMP2("\tPseudoRangeRmsError: %u\n", measurementData.PseudoRangeRmsError());
                }                           
            }               
        }
    }

void CNpeApiImpl::ClearAssistanceData()
    {
    iAssistanceDataAvailable = EFalse;
    iAssistanceDataRequested = EFalse;
    //Setup the required assistance data here
    iOutstandingAssistanceData = EAssistanceDataReferenceTime;
    }



TInt CNpeApiImpl::ProcessRequestUpdateInit(const TDesC& aConfigFileName, const TDesC& aConfigSection)
	{
	LBSLOG(ELogP1, "CNpeApiImpl::ProcessRequestConfigL()\n");

	T_LbsUtils utils;

	TInt err = KErrNone;
	// Reset the update array, before we re-assign new updates.
	iUpdateArr.ResetAndDestroy();
	iUpdateArrIndex = 0;

	// Read the configuration ini file to update the update data items.
	TRAP(err, utils.GetConfigured_UpdateArrayL(aConfigFileName, aConfigSection, iUpdateArr));
						
	// NOTE: in the old module we used to reset everything, this maybe required here also.

	return err;
	}
 

// We're being asked to update the current time out value.
void CNpeApiImpl::ProcessRequestTimeOut(const TTimeIntervalMicroSeconds& aTimeOut)
	{
	iTimeOut = aTimeOut;
	}
			
// We're being asked to update the current error
void CNpeApiImpl::ProcessRequestError(TInt aError)
	{
	iError = aError;	
	}

/* We're being asked to send all measurements as immediate ones */
void CNpeApiImpl::ProcessImmediateMeasurements(TInt aImmediateMeasurements)
	{
	iImmediateMeasurements = aImmediateMeasurements;	
	}


// We're being asked to update  the test module options
TInt CNpeApiImpl::ProcessRequestModuleOptions(TLbsHybridModuleOptions aModuleOption, TBool aValue)
	{
	TInt err = KErrNone;
	
	//Reset the assistance Data the may have previously been requested
	ClearAssistanceData();

	switch (aModuleOption)
		{
		case EHybridModuleOptions_ClearAll:
		case ELbsHybridModuleOptions_SetAll:
			{
			// We're being asked to update all the test module options
			iModuleOptions = aModuleOption;
			break;				
			}
		case ELbsHybridModuleOptions_AssistanceDataOn:
			{
			// We're being asked to update a particular test mode options
			SetOption(aModuleOption, aValue);	
			break;				
			}
		case ELbsHybridModuleOptions_MultiUpdatesOn:
			{
			// We're being asked to update a particular test mode options
			SetOption(aModuleOption, aValue);	
			// If MultiUpdates is being switch off reset the counter
			if (!aValue)
				{
				iExtraUpdatesCtr = 0;	
				}
			break;		
			}
		case ELbsHybridModuleOptions_DisableReqAssistData:
			{
			// We're being asked to update a particular test mode options
			SetOption(aModuleOption, aValue);	
			break;
			}
		case ELbsHybridModuleOptions_DynamicAssistanceData:
			{
			//Turn on dynamic assistance data to always re-request assistance data we have not received
			SetOption(aModuleOption, aValue);
			break;
			}
		default:
			{
			err = KErrNotSupported;
			break;				
			}
		}

	return err;
	}


/** Force the agps module to send the next update in the array to LBS.

This function is used to force an update even if there is no outstanding
request from LBS.
*/
void CNpeApiImpl::ProcessRequestForcedUpdate()
	{
	UpdateLocation();
	}

/** Enable or disable a particular module option.
*/
void CNpeApiImpl::SetOption(TLbsHybridModuleOptions aModuleOption, TBool aValue)
	{
	if (aValue)
		{
		iModuleOptions |= aModuleOption; // Set bit.
		}
		
	else
		{
		iModuleOptions &= ~aModuleOption; // Clear bit.
		}
	}


/** Determine if a particular module option is enabled.
*/	
TBool CNpeApiImpl::GetOption(TLbsHybridModuleOptions aModuleOption)
	{
	if (aModuleOption & iModuleOptions)
		{
		return ETrue;
		}
	return EFalse;
	}
