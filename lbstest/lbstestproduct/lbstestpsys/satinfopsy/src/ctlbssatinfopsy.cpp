// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include <lbspositioninfo.h>
#include <lbssatellite.h>
#include "ctlbssatinfopsy.h"
#include "satinfopsy.hrh"
#include "ctlbspositionconstants.h"
#include "ctlbstestproxypsybase.h"

_LIT(KFileName,  "c:\\logs\\satinfopsy.txt");

// C++ default constructor can NOT contain any code, that
// might leave.
//
CT_LbsSatInfoPsy::CT_LbsSatInfoPsy() 
    {
    }

// EPOC default constructor can leave.
void CT_LbsSatInfoPsy::ConstructL(TAny* aConstructionParameters)
    {
    BaseConstructL(aConstructionParameters);
    iRequestHandler = new (ELeave) CT_LbsTestProxyPsyBase(PositionerStatus());
	
	TUid implUid = { KPosImplementationUid };
	iRequestHandler->SetModuleId(implUid);
	
	User::LeaveIfError(iFs.Connect());
    }

// Two-phased constructor.
CT_LbsSatInfoPsy* CT_LbsSatInfoPsy::NewLC(TAny* aConstructionParameters)
    {
    CT_LbsSatInfoPsy* self = new (ELeave) CT_LbsSatInfoPsy;
    CleanupStack::PushL(self); 
    self->ConstructL(aConstructionParameters);
    return self;
    }
    
// Two-phased constructor.
CT_LbsSatInfoPsy* CT_LbsSatInfoPsy::NewL(TAny* aConstructionParameters)
    {
    CT_LbsSatInfoPsy* self = NewLC(aConstructionParameters);
    CleanupStack::Pop();
    return self;
    }
    
// Destructor
CT_LbsSatInfoPsy::~CT_LbsSatInfoPsy()
    {
    delete iRequestHandler; 
    iFs.Close();
    }

// ---------------------------------------------------------
// CT_LbsSatInfoPsy::NotifyPositionUpdate
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CT_LbsSatInfoPsy::NotifyPositionUpdate(
	TPositionInfoBase& aPosInfo,
    TRequestStatus& aStatus)
	{    
    TBuf<256> buf;
    OpenFileForAppend();
    _LIT(KSatInfoPsy, " Request issued to SatInfoPsy");
    buf.Append(KSatInfoPsy);
    _LIT(KAppend, "\r");
    buf.Append(KAppend);
    iFileText.Write(buf);    
    iFile.Close();
    
    TPositionInfo* position = static_cast<TPositionInfo*> (&aPosInfo);
    TUint32 request = position->UpdateType();
    TRequestStatus* status = &aStatus;

	TUid implUid = { KPosImplementationUid };
	position->SetModuleId(implUid);
    TUint32 classType = position->PositionClassType();

	TUint32 generic = EPositionInfoClass | EPositionGenericInfoClass;
	TUint32 info = EPositionInfoClass;
	TUint32 course = info | EPositionCourseInfoClass;
	TUint32 sat = course | EPositionSatelliteInfoClass;

    if (classType == (classType & info))
        {
        SetTPositionInfo(*position);
        }
	else if (classType == (classType & course))
		{
		TPositionCourseInfo& course = static_cast<TPositionCourseInfo&> (*position);
		SetTPositionCourseInfo(course);
		}
	else if (classType == (classType & sat))
		{
		TPositionSatelliteInfo& satellite = static_cast<TPositionSatelliteInfo&> (*position);
		SetTPositionSatelliteInfo(satellite);
		}
	else if (classType == (generic & classType))
		{
		HPositionGenericInfo& generic = static_cast <HPositionGenericInfo&> (*position);
		SetHPositionGenericInfo(generic);
		}
	else
		{
		User::RequestComplete(status, KErrGeneral); 
        return;
		}
	
	 switch( request)
		{
    	case 4722:
    	       iRequestHandler->SetErrorCode(KErrNone);
    	       iRequestHandler->SetTimerDelay(1000000);
    	       iRequestHandler->NotifyPositionUpdate(position, status);
    	       break;
    	 
    	case 100:
             iRequestHandler->ReportStatus(
				TPositionModuleStatus::EDeviceInitialising, 
				TPositionModuleStatus::EDataQualityUnknown);
			 User::RequestComplete( status, KErrNone );
			 break; 
			 
		case 200:
             iRequestHandler->ReportStatus(
				TPositionModuleStatus::EDeviceActive, 
				TPositionModuleStatus::EDataQualityUnknown);
			 User::RequestComplete( status, KErrNone );
			 break; 
    	
    	default:
    	       User::RequestComplete(status, KErrNone);
    	       break;          
		}
    }

// ---------------------------------------------------------
// CT_LbsSatInfoPsy::CancelNotifyPositionUpdate
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CT_LbsSatInfoPsy::CancelNotifyPositionUpdate()
	{
	TBuf<256> buf;
	OpenFileForAppend();
	_LIT(KSatInfoPsy, " Request issued to SatInfoPsy is cancelled");
    buf.Append(KSatInfoPsy);
    _LIT(KAppend, "\r");
    buf.Append(KAppend);
    iFileText.Write(buf);
    iFile.Close();
    
      if(iRequestHandler->IsActive())
	    {
	      iRequestHandler->CancelNotifyPositionUpdate(); 
		}
	}

void CT_LbsSatInfoPsy::SetTPositionInfo(TPositionInfo& aInfo)
	{
	
	TCoordinate coor(KLatitude, KLongitude, KAltitude);
	TLocality loc(coor, KHorizontalAcc, KVerticalAcc);
	TPosition pos(loc, TTime(KPositionTime));
	aInfo.SetPosition(pos);
	}

void CT_LbsSatInfoPsy::SetTPositionCourseInfo(TPositionCourseInfo& aCourseInfo)
	{
	SetTPositionInfo(aCourseInfo);
	TCourse course;
	course.SetSpeed(KSpeed);
	course.SetHeading(KHeading);
	course.SetSpeedAccuracy(KSpeedAcc);
	course.SetHeadingAccuracy(KHeadingAcc);
	aCourseInfo.SetCourse(course);
	}

void CT_LbsSatInfoPsy::SetTPositionSatelliteInfo(TPositionSatelliteInfo& aSatelliteInfo)
	{
	SetTPositionCourseInfo(aSatelliteInfo);
	aSatelliteInfo.SetSatelliteTime(TTime(KSatelliteTime));
	aSatelliteInfo.SetHorizontalDoP(KHorizontalDoPValue);
	aSatelliteInfo.SetVerticalDoP(KVerticalDoPValue);
	aSatelliteInfo.SetTimeDoP(KTimeDoPValue);
	for (TInt  i = 0; i < KNumberOfSatellitesInView; i++)
		{
		TSatelliteData satellite;
		if ( (i%2) == 0 )
			{
			satellite.SetSatelliteId(KSatelliteId+i);
			satellite.SetAzimuth(KAzimuth);
			satellite.SetElevation(KElevation);
			satellite.SetIsUsed(KIsUsed);
			satellite.SetSignalStrength(KSignalStrength);
			}
		else
			{
			satellite.SetSatelliteId(KSatelliteId+i);
			satellite.SetAzimuth(KAzimuthOdd);
			satellite.SetElevation(KElevationOdd);
			satellite.SetIsUsed(KIsUsedOdd);
			satellite.SetSignalStrength(KSignalStrengthOdd);
			}
		aSatelliteInfo.AppendSatelliteData(satellite);
		}
	}

void CT_LbsSatInfoPsy::SetHPositionGenericInfo(HPositionGenericInfo& aGenericInfo)
	{
	SetTPositionInfo(aGenericInfo);
	}

TInt CT_LbsSatInfoPsy::OpenFileForAppend()
    {
    TInt err = iFile.Open(iFs, KFileName, EFileStreamText|EFileWrite);
    if (err != KErrNone)
        {
        if (err == KErrNotFound)
            {
            err = iFile.Create(iFs, KFileName, EFileStreamText|EFileWrite);
            }

        if (err != KErrNone)
            {
            return err;
            }
        
        }
    iFileText.Set(iFile);
    return KErrNone;
    }
