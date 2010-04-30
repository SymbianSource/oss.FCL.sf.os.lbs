// Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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


/**
@file
@internalTechnology
@prototype
*/

#include <lbslocation.h>

//=============================================================================
// Constructor
//=============================================================================

EXPORT_C TLbsLocation::TLbsLocation()
	{
	iClassSize = sizeof(TLbsLocation);
	iClassType |= ELbsLocationClassType;

	iIsLocationValid    = EFalse;
	iLatitude           = 0.0;
    iLongitude          = 0.0;
    iHorizontalAccuracy = 0.0;
    
    iIsAltitudeValid    = EFalse;
    iAltitude           = 0.0;
    iAltitudeAccuracy   = 0.0;
    
    iIsSpeedValid       = EFalse;
    iSpeed              = 0.0;
    iSpeedAccuracy      = 0.0;
    
    iIsCourseValid      = EFalse;
    iCourse             = 0.0;
    iCourseAccuracy     = 0.0;
    
#pragma message ("***Need to initiase timestamp***")
    //iSystemTimeStamp = ...
	}


//=============================================================================
// Latitude and Longitude
//=============================================================================

/**
 * Determines if a latitude and longitude and (horizontal) accuracy has been assigned.
 * 
 * Positions returned by the location subsystem will have a valid latitude, longitude
 * and accuracy.
 * 
 * When applications create their own instances of the TLbsLocation class this
 * method will return EFalse until a latitude and longitude has been assigned.
 *      
 * @return ETrue When a valid latitude and longitude are available for the location. 
 * @return EFalse When no valid latitude and longitude are available for the location.
 * @see Latitude()
 * @see Longitude()
 * @see Accuracy()
 * @see SetLocation()
 */
EXPORT_C TBool TLbsLocation::IsLatLongValid() const
    {
    return iIsLocationValid;
    }

/**
 * Retrieves the latitude of the location. Coordinates are referenced to WGS84 datum.
 * 
 * If the latitude has not been assigned the method returns zero.
 * 
 * Applications should use IsLatLongValid() to determine if there is a valid latitude.
 * 
 * @return The latitude of the location in degrees from the range -90 to +90 when the location contains a valid latitude.
 * @return Zero if the latitude has not been assigned.
 * @see IsLatLongValid()
 * @see Longitude()
 * @see Accuracy()
 * @see SetLocation()
 */
EXPORT_C TReal64 TLbsLocation::Latitude() const
    {
    return iLatitude;
    }

/**
 * Retrieves the longitude of the location. Coordinates are referenced to WGS84 datum.
 * 
 * If the longitude has not been assigned the method returns zero.
 * 
 * Applications should use IsLatLongValid() to determine if there is a valid longitude.
 * 
 * @return The longitude of the location in degrees from the range 0 up to (but not including) 360 when the location contains a valid longitude.
 * @return Zero if the longitude has not been assigned.
 * @see IsLatLongValid()
 * @see Latitude()
 * @see Accuracy()
 * @see SetLocation()
 */
EXPORT_C TReal64 TLbsLocation::Longitude() const
    {
    return iLongitude;
    }

/**
 * Retrieves the horizontal (circular) accuracy of the latitude and longitude in meters.
 * 
 * If the accuracy has not been assigned the method returns zero.
 * 
 * Applications should use IsLatLongValid() to determine if there is a valid accuracy.
 * 
 * @return The circular accuracy in meters of the latitude and longitude 
 * @return Zero if the location accuracy has not been assigned.
 * @see IsLatLongValid()
 * @see Latitude()
 * @see Longitude()
 * @see SetLocation()
 */
EXPORT_C TReal32 TLbsLocation::Accuracy() const
    {
    return iHorizontalAccuracy;
    }

/**
 * Sets the position information for the location.
 * 
 * If the latitude and longitude paramaters are outside the range [-90,+90], [0,360) the position
 * will be automatically adjusted to fall within the valid range. For example, attempting to set 
 * a position of latitude 91, longitude 0 will be converted into 89, 180, respectively.
 *
 * After these parameters have been successfully assigned, the method IsLatLongValid() will return ETrue.
 * 
 * @param[in] aLatitude The latitude of the location in degrees. Normal range is 0 up to (but not including) 360 degrees.
 * @param[in] aLongitude The longitude of the location in degrees. Normal range is -90 to +90 (inclusive).
 * @param[in] aAccuracy The accuracy of the course in degrees. Value cannot be negative
 * @return KErrNone If parameter aAccuracy is in range and have been set successfully.
 * @return KErrArgument If parameter aAccuracy is negative.
 * @see IsLatLongValid()
 * @see Latitude()
 * @see Longitude()
 * @see Accuracy()
 */
EXPORT_C TInt TLbsLocation::SetLocation(TReal64 aLatitude, TReal64 aLongitude, TReal32 aAccuracy)
   {
   iLatitude  = aLatitude;
   iLongitude = aLongitude;
   iHorizontalAccuracy = aAccuracy;
   iIsLocationValid = ETrue;
   return KErrNone;
   }

//=============================================================================
// Altitude
//=============================================================================

/**
 * Determines if an altitude has been assigned.
 * 
 * Availability of altitude information is dependent on the underlying positioning technology and
 * will not always be present.
 *   
 * @return ETrue When a valid altitude is available for the location. 
 * @return EFalse When altitude information is unavailable.
 * @see Altitude()
 * @see AltitudeAccuracy()
 * @see SetAltitude()
 */
EXPORT_C TBool TLbsLocation::IsAltitudeValid() const
    {
    return iIsAltitudeValid;
    }

/**
 * Retrieves the altitude in meters for the location.
 * 
 * If the altitude is not available the method returns zero.
 * Availability of altitude information is dependent on the underlying positioning technology and
 * will not always be present.
 * 
 * Applications should use IsAltitudeValid() to determine if there is a valid altitude.
 * 
 * @return The altitude in meters when the location contains a valid altitude.
 * @return Zero when altitude information is unavailable.
 * @see IsAltitudeValid()
 * @see AltitudeAccuracy()
 * @see SetAltitude()
 */
EXPORT_C TReal32 TLbsLocation::Altitude() const
    {
    return iAltitude;
    }

/**
 * Retrieves the vertical accuracy of the location.
 * 
 * If the altitude is not available the method returns zero.
 * Availability of altitude information is dependent on the underlying positioning technology and
 * will not always be present.
 * 
 * Applications should use IsAltitudeValid() to determine if there is a valid altitude.
 * 
 * @return The vertical accuracy in meters when the location contains a valid altitide.
 * @return Zero when altitude information is unavailable.
 * @see IsAltitudeValid()
 * @see Altitude()
 * @see SetAltitude()
 */
EXPORT_C TReal32 TLbsLocation::AltitudeAccuracy() const
    {
    return iAltitudeAccuracy;
    }

/**
 * Sets the altitude information for the location.
 * 
 * After the altitude has been successfully assigned, the method IsAltitudeValid will return ETrue.
 * 
 * @param[in] aAltitude The altitude of the location in meters.
 * @param[in] aAccuracy The accuracy of the course in degrees. Value cannot be negative
 * @return KErrNone If parameter aAccuracy is in range and have been set successfully.
 * @return KErrArgument If parameter aAccuracy is negative.
 * @see IsAltitudeValid()
 * @see Altitude()
 * @see AltitudeAccuracy()
 */
EXPORT_C TInt TLbsLocation::SetAltitude(TReal32 aAltitude, TReal32 aAccuracy)
   {
   iAltitude = aAltitude;
   iAltitudeAccuracy = aAccuracy;
   iIsAltitudeValid = ETrue;
   return KErrNone;
   }


//=============================================================================
// Speed
//=============================================================================

/**
 * Determines if a valid speed is available.
 * 
 * Availability of speed information is dependent on the underlying positioning technology and
 * will not always be present.
 * Even satellite based technologies may not provide speed information in certain circumstances.
 *  
 * @return ETrue When a valid speed is available for the location. 
 * @return EFalse When speed information is unavailable.
 * @see Speed()
 * @see SpeedAccuracy()
 * @see SetSpeed()
 */
EXPORT_C TBool   TLbsLocation::IsSpeedValid() const
	{
	return iIsSpeedValid;
	}

/**
 * Retrieves the speed in meters per second.
 * 
 * If the speed is not available the method returns zero.
 * 
 * Availability of speed information is dependent on the underlying positioning technology and
 * will not always be present.
 * Even satellite based technologies may not provide speed information in certain circumstances.
 *  
 * Applications should use IsSpeedValid() to determine if there is a valid speed.
 * 
 * @return The speed in meters per second when the location contains a valid speed.
 * @return Zero when speed is unavailable.
 * @see IsSpeedValid()
 * @see SpeedAccuracy()
 * @see SetSpeed()
 */
EXPORT_C TReal32 TLbsLocation::Speed() const
	{
	return iSpeed;
	}

/**
 * Retrieves the accuracy of the speed information in meters per second.
 * 
 * If the speed is not available the method returns zero.
 * 
 * Availability of speed information is dependent on the underlying positioning
 * technology and will not always be present.
 * 
 * When speed information is available, the accuracy indicates the size of the error band.
 * The actual speed may be above or below the reported speed but the difference should be
 * less than or equal to the speed accuracy.
 * 
 * Applications should use IsSpeedValid() to determine if there is a valid speed.
 * 
 * @return The accuracy speed in meters per second when the location contains a valid speed.
 * @return Zero when the speed is unavailable.
 * @see IsSpeedValid()
 * @see Speed()
 * @see SetSpeed()
 */
EXPORT_C TReal32 TLbsLocation::SpeedAccuracy() const
    {
    return iSpeedAccuracy;
    }

/**
 * Sets the speed information for the location.
 * 
 * After the speed has been successfully assigned, the method IsSpeedValid() will return ETrue.
 * 
 * @param[in] aSpeed The speed that is related to the location in meters per second. Value cannot be negative.
 * @param[in] aAccuracy The accuracy of the speed in meters per second. Value cannot be negative.
 * 
 * @return KErrNone If all parameters are in range and have been set successfully.
 * @return KErrArgument If either parameter is negative.
 * @see IsSpeedValid()
 * @see Speed()
 * @see SpeedAccuracy()
 */
EXPORT_C TInt TLbsLocation::SetSpeed(TReal32 aSpeed, TReal32 aAccuracy)
   {
   iSpeed = aSpeed;
   iSpeedAccuracy = aAccuracy;
   iIsSpeedValid = ETrue;
   return KErrNone;
   }

//=============================================================================
// Course
//=============================================================================

/**
 * Determines if a valid course is available.
 * 
 * Availability of course information is dependent on the underlying positioning technology and
 * will not always be present.
 * Even satellite based technologies may not provide course information in certain circumstances. 
 * 
 * @return ETrue When a valid course is available for the location. 
 * @return EFalse When course information is unavailable.
 * @see Course()
 * @see CourseAccuracy()
 * @see SetCourse()
 */
EXPORT_C TBool   TLbsLocation::IsCourseValid() const
	{
	return iIsCourseValid;
	}

/**
 * Retrieves the course in degrees counting clockwise relative to true north.
 * 
 * If the course is not available, the method returns zero.
 * 
 * Availability of course information is dependent on the underlying positioning technology and
 * will not always be present.
 * Even satellite based technologies may not provide course information in certain circumstances.
 *  
 * Applications should use IsCourseValid() to determine if there is a valid course.
 * 
 * When there is a valid course, the range of values will be from [0 to 360).
 * That is: values can be up to (but will not include) 360. A value of 0 indicates the device is heading north.
 * 
 * @return The course in degrees from the range [0 to 360) when the location contains a valid course.
 * @return Zero when course information is unavailable.
 * @see IsCourseValid()
 * @see CourseAccuracy()
 * @see SetCourse()
 */
EXPORT_C TReal32 TLbsLocation::Course() const
	{
	return iCourse;
	}

/**
 * Retrieves the accuracy of the course information in degrees.
 * 
 * If the course is not available the method returns zero.
 * 
 * Availability of course information is dependent on the underlying positioning technology and
 * will not always be present.
 * 
 * Applications should use IsCourseValid() to determine if there is a valid course.
 * 
 * When course information is available, the accuracy indicates the size of the error band.
 * The actual course may be vary from the reported course but the difference should be less
 * than or equal to the course accuracy.
 * 
 * @return The accuracy of the course in degrees when the location contains a valid course.
 * @return Zero when course information is unavailable.
 * @see IsCourseValid()
 * @see Course()
 * @see SetCourse()
 */
EXPORT_C TReal32 TLbsLocation::CourseAccuracy() const
    {
    return iCourseAccuracy;
    }

/**
 * Sets the course information for the location.
 * 
 * If the course paramater is outside the range [0,360) it will be automatically
 * adjusted to fall within the valid range. For example, attempting to set a course
 * 361 degrees will be converted to 1 degree.
 *
 * After the course has been successfully assigned, the method IsCourseValid() will return ETrue.
 * 
 * @param[in] aCourse Degrees counting clockwise relative to true north. Normal range is 0 up to (but not including) 360.
 * @param[in] aAccuracy The accuracy of the course in degrees. Value cannot be negative
 * @return KErrNone If parameter aAccuracy is in range and have been set successfully.
 * @return KErrArgument If parameter aAccuracy is negative.
 * @see IsCourseValid()
 * @see Course()
 * @see CourseAccuracy()
 */
EXPORT_C TInt TLbsLocation::SetCourse(TReal32 aCourse, TReal32 aAccuracy)
   {
   iCourse = aCourse;
   iCourseAccuracy = aAccuracy;
   iIsCourseValid = ETrue;
   return KErrNone;
   }


//=============================================================================
// Timestamp
//=============================================================================

/**
  * Retrieves the time on the device when the location entered the system.
  * 
  * If this time stamp has not been assigned, a TTime of "zero" will be returned.
  *  
  * This timestamp is assigned by the local device and reflects when the location information injected.   
  * In particular, this timestamp does not provide technology specific information such as GPS time.
  * 
  * @return The time when the location entered the system. 
  * @return Zero If no valid course is available.
  * @see SetSystemTimeStamp()
  */
EXPORT_C TTime TLbsLocation::SystemTimeStamp() const
     {
     return iSystemTimeStamp;
     }

/**
 * Sets the system time stamp for the location information.
 * 
 * This is the time when the location information entered the system.
 * 
 * The parameter aTimeStamp must contain a valid time and cannot be "zero".
 *   
 * @param[in] aTimeStamp The system time that is related to the location information.
 * @return KErrNone If the time stamp has been set.
 * @return KErrArgument If the parameter is invalid. For example, a time stamp of zero was supplied. 
 * @see SystemTimeStamp()
 */
EXPORT_C TInt TLbsLocation::SetSystemTimeStamp(TTime aTimeStamp)
   {
   iSystemTimeStamp = aTimeStamp;
   return KErrNone;
   }

//=============================================================================
// GetMeanSpeed / GetDistance / GetBearingTo / Move
//=============================================================================

/**
 * This method calculates the mean horizontal speed in meters per second between
 * this location and the supplied aOtherLocation. The speed is calculated based on the
 * latitude and longitude of each of the locations and their system timestamps.
 *
 * @param[in] aOtherLocation A point different from this Location.
 * @param[out] aMeanSpeed Upon successful completion, this is set to the speed 
 * indicated by being at this position at its recorded time, and at aOtherLocation
 * at its recorded time. Always positive, in meters per second.
 * @return KErrNone When the mean speed has been successfully calculated.
 * @return KErrArgument If any of the Latitudes, Longitudes, or System Timestamps have not been assigned.
 * @return KErrArgument If the System Timestamp for this or aOtherLocation are the same.
 * @return A Symbian OS error code if some other problem prevents the calculation.
 */
EXPORT_C TInt TLbsLocation::GetMeanSpeed(const TLbsLocation& /*aOtherLocation*/, TReal32& /*aMeanSpeed*/) const
    {
    return KErrNotSupported;
    }

/**
 * Calculate the distance between this location and the supplied location.
 * @param[in] aOtherLocation A point different from this Location.
 * @param[out] aDistance Upon successful completion, this is set to the distance
 * between this location and aOtherLocation, in meters.
 * @return KErrNone When the distance has been successfully calculated.
 * @return KErrArgument If any of the Latitudes or Longitudes have not been assigned.
 * @return A Symbian OS error code if some other problem prevents the calculation.
 */
EXPORT_C TInt TLbsLocation::GetDistance(const TLbsLocation& /*aOtherLocation*/, TReal32& /*aDistance*/) const
    {
    return KErrNotSupported;
    }

/**
 * Calculate the bearing in degrees from this location to the supplied location.
 * 
 * @param[in] aOtherLocation Target location.
 * @param[out] aBearing Upon successful completion, this is set to the bearing
 * from this location to aOtherLocation. Value is in degrees counting clockwise relative to true north.
 * 
 * @return KErrNone When the bearing has been successfully calculated.
 * @return KErrArgument If any of the Latitudes or Longitudes have not been assigned.
 * @return KErrPositionIncalculable if this location is at a pole or if the 
 * two locations are the same or antipodal.
 * @return A Symbian OS error code if some other problem prevents the calculation.
 */
EXPORT_C TInt TLbsLocation::GetBearingTo(const TLbsLocation& /*aOtherLocation*/, TReal32& /*aBearing*/) const
    {
    return KErrNotSupported;
    }


/**
 * This method moves this location aDistance in the direction specified
 * by aBearing
 *
 * @param[in] aBearing The direction to move this location. Value is in degrees 
 * counting clockwise relative to true north.
 * @param[in] aDistance The distance to move this Location. Value is in meters.
 * 
 * @return KErrNone When the move has been successfully completed.
 * @return KErrArgument If the Latitude or Longitude for this location have not been assigned.
 * @return KErrPositionIncalculable if this location is at a pole.
 * @return A Symbian OS error code if some other problem prevents the calculation.
 */
EXPORT_C TInt TLbsLocation::Move(TReal32 /*aBearing*/, TReal32 /*aDistance*/)
    {
    return KErrNotSupported;
    }
