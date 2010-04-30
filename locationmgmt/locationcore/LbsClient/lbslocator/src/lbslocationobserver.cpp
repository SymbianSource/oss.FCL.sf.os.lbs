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

#include "lbslocationobserver.h"

//=============================================================================
// MLbsLocationLocationObserver
//=============================================================================

/**
 * Applications must implement this method to receive location updates.
 * 
 * To begin receiving updates the application must call CLbsLocator::StartUpdatesL().
 * 
 * After calling StartUpdatesL(), the first update an application will receive is a cached location
 * (if available). The application should check the accuracy of this initial update to determine
 * how useful it is. The accuracy of cached positions is likely to vary from a few kilometers up
 * to the country level. That is, an application may receive an initial position only based on the
 * country they are in.
 * 
 * Only after delivering any initial cached position will the system attempt obtain the current
 * location. Application will normally be sent location updates with improving accuracy until
 * the target accuracy for the active profile is achieved. 
 * 
 * When an application no longer wishes to receive updates it should call CLbsLocator::StopUpdates().
 * It is legal, valid and usual to stop updates from within the HandleLocationUpdate() callback.
 * Stopping updates while the initial cached position is being deliverd, prevents the search for the
 * current positioning being initiated. This is a useful technique to reduce cost and power consumption.
 * 
 * The point at which subsequent updates sent to the application depends on the active profile.
 * Applications will receive updates when the device has moved a designated distance and/or after
 * a time period.
 *   
 * Applications may received notification about the loss of accuracy and unavailability of position
 * information by implementing the HandleLocationStatus() method. This method is also used to pass error
 * information to the application.
 * 
 * @param[in] aLocation Initially may contain a cached location if available, subsequent updates hold the current location of the device.\n
 * The accuracy of the location should be checked to determine how useful it is to the application.   
 * 
 * @see TLbsLocation
 * @see HandleLocationStatus
 * @see CLbsLocator::StartUpdatesL()
 * @see CLbsLocator::StopUpdates()
 * @see CLbsLocator::SetLocatorProfile()
 * @see CLbsLocator::SetLocatorOption()    
 */
void MLbsLocationObserver::HandleLocationUpdate(const TLbsLocation& /*aLocation*/)
    {
    }

/**
 * Applications must implement this method to receive location status updates.
 * 
 * To begin receiving status updates the application must call CLbsLocator::StartUpdatesL().
 * 
 * Status updates are typically generated when it has not been possible to obtain either a sufficiently
 * accurate position for the active profile or indeed any position. Such events are normal and are
 * often transient. They do not indicate an error with the hardware or system.
 * 
 * In these types of situations, the parameter aStatus will contain a positive value. The
 * system will continue to try and obtain a more accurate position and there is
 * generally no need for the application to change profiles, stop or restart updates.
 * 
 * When positions of target accuracy has been achieved, HandleLocationStatus() will be called with
 * the status code of KErrNone.
 * 
 * It depends on the active profile when status updates about the accuracy or availability
 * of positions are sent to the application . Applications can change this timer period by calling
 * CLbsLocator::SetLocatorOption() with the options ELocatorStatusTimer. 
 * 
 * If HandleLocationStatus() is called with a negative status code, this indicates there has been an
 * error inside the location subsystem or with the connected hardware. These situations are generally
 * not recoverable and the application would normally stop using the interface. 
 * 
 * When an application no longer wishes to receive updates it should call CLbsLocator::StopUpdates().
 * It is possible to call this method from within the HandleLocationStatus() callback.
 * 
 * Typical values of parameter aStatus include:
 *  - <I>KPositionQualityLoss:</I> Indicates that although it is still possible to obtain a position for
 *    the current location, its accuracy does not meet that expected for the active profile.
 *    For example, in the Lazy Tracker profile this will occurr when it has not been possible to
 *    obatin a GPS position but a cell based location is available.\n\n
 *     
 *  - <I>KPositionNotAvailable:</I> This status code informs that application that no position information
 *    can be obatined for the current location.\n\n
 *    
 *  - <I>KErrNone:</I> Indicates that a previously reported problem related to the position availability
 *    or accuracy has passed. It is has now been possible to obtain positions that meets the target
 *    of the active profile.\n\n
 *    
 *  - When a Symbian OS (negative) error code is passed, this indicates there is a problem with
 *    the system and/or the hardware. This will normally prevent any position information being
 *    returned.
 * 
 * @param[in] aStatus A positive value indicates there is a loss of accuracy or no position
 * is currently available. This is generally a transient situation. A negative value means an
 * error has occurred that will normally prevent any position information being returned.
 * KErrNone is returned when normal service has been restored.   
 * 
 * @see HandleLocationUpdate
 * @see CLbsLocator::SetLocatorOption() 
 * @see CLbsLocator::StartUpdatesL()
 * @see CLbsLocator::StopUpdates()
 * @see CLbsLocator::SetLocatorProfile() 
 */
void MLbsLocationObserver::HandleLocationStatus(TInt /*aStatus*/)
    {
    }


/**
 * This method should not be overridden by the application.
 * 
 * It is queried by the location subsystem to determine the version of the observer interface used
 * by the application.
 * 
 */
TVersion MLbsLocationObserver::Version() const
    {
    return TVersion(0,0,1);
    }

