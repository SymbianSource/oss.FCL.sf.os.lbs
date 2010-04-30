/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#ifndef __LBSLOCATION_H__
#define __LBSLOCATION_H__

#include <e32base.h>
#include <lbslocatorclasstypes.h>

/**
@file
@internalTechnology
@prototype
*/

/**
 * TLbsLocation - Contains a position and its accuracy. May contain altitude, speed and course.
 * See .cpp file for extended comments.
 */

class TLbsLocation : public TLbsLocatorClassTypeBase
    {
public:
    IMPORT_C TLbsLocation();

    /* Latitude, Longitude and Accuracy methods return 0 if not assigned */ 
    IMPORT_C TBool   IsLatLongValid()  const;   
    IMPORT_C TReal64 Latitude()        const;
    IMPORT_C TReal64 Longitude()       const;
    IMPORT_C TReal32 Accuracy()        const;
    
    /* Altitude and AltitudeAccuracy return 0 if not assigned */
    IMPORT_C TBool   IsAltitudeValid() const;
    IMPORT_C TReal32 Altitude()        const;    
    IMPORT_C TReal32 AltitudeAccuracy()const;
    
    /*Speed and accuracy return 0 if not assigned */
    IMPORT_C TBool   IsSpeedValid()    const;
    IMPORT_C TReal32 Speed()           const;
    IMPORT_C TReal32 SpeedAccuracy()   const;

    /* Course and accuracy returns 0 if not assigned */
    IMPORT_C TBool   IsCourseValid()   const;
    IMPORT_C TReal32 Course()          const;
    IMPORT_C TReal32 CourseAccuracy()  const;
   
    /* Calculate the differences from this location to another location*/
    IMPORT_C TInt    GetMeanSpeed(const TLbsLocation& aOtherLocation, TReal32& aMeanSpeed)const;
    IMPORT_C TInt    GetDistance(const TLbsLocation&  aOtherLocation, TReal32& aDistance) const;
    IMPORT_C TInt    GetBearingTo(const TLbsLocation& aOtherLocation, TReal32& aBearing)  const;    
    
    /* Translate this location along the bearing to the specified distance */
    IMPORT_C TInt    Move(TReal32 aBearing, TReal32 aDistance);
    
    /* SystemTimeStamp returns a time of 0 if not assigned */
    IMPORT_C TTime   SystemTimeStamp() const;        

    /* Methods to set values - see detailed comments for valid parameter ranges */
    IMPORT_C TInt    SetLocation(TReal64 aLatitude, TReal64 aLongitude, TReal32 aAccuracy);
    IMPORT_C TInt    SetAltitude(TReal32 aAltitude, TReal32 aAccuracy);
    IMPORT_C TInt    SetSpeed(TReal32  aSpeed,  TReal32 aAccuracy);
    IMPORT_C TInt    SetCourse(TReal32 aCourse, TReal32 aAccuracy);  
    IMPORT_C TInt    SetSystemTimeStamp(TTime aTimeStamp);
    
private:   
    TBool   iIsLocationValid;
    TReal64 iLatitude;
    TReal64 iLongitude;
    TReal32 iHorizontalAccuracy;
    
    TBool   iIsAltitudeValid;
    TReal32 iAltitude;
    TReal32 iAltitudeAccuracy;
    
    TBool   iIsSpeedValid;
    TReal32 iSpeed;
    TReal32 iSpeedAccuracy;
    
    TBool   iIsCourseValid;
    TReal32 iCourse;
    TReal32 iCourseAccuracy;
    
    TTime   iSystemTimeStamp;
    
    TInt    iReserved[24];
    };


#endif //__LBSLOCATION_H__
