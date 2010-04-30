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

//#include <e32base.h>

#include "lbslocator.h"

//=============================================================================
// CLbsLocator - Constructors and Destructor
//=============================================================================

/**
 * Default constructor for the cell information retriever.
 */  
CLbsLocator::CLbsLocator()
	{	
	}

/**
 * Default constructor for the cell information retriever.
 */  
void CLbsLocator::ConstructL(MLbsLocationObserver* /*aObserver*/)
	{
	//iImpl = CLbsCellInfoImpl::NewL(aObserver);	
	}

/**
 * Closes all open resources and stops updates being sent to the application.
 * 
 * If an application wishes to temporarily halt updates it should use the StopUpdates() method.
 *  
 * The destructor is normally called as the application is closing down. It is not a requirement
 * to call StopUpdates() before deleting the interface but it is recommended to do so.
 * 
 * Note: The destructor must not be invoked during any form of callback from the interface to one
 * of the application's observers. For example during a location or movement update.
 * This is a programming error and is likely to result in an application crash.
 * 
 * It is however possible and legal to call StopUpdates() during an update callback. 
 * 
 * @see StopUpdates() 
 */  
EXPORT_C CLbsLocator::~CLbsLocator()
	{
	//delete iImpl;
	//iImpl = NULL;	
	}

/**
 * Creates a new instance of the CLbsLocator interface class.
 * 
 * After the instance of the interface has been created, the application should select the desired
 * profile using SetLocatorProfile(). To begin receiving location and/or movement information,
 * the application must call StartUpdatesL(). The frequency and accuracy of these updates
 * depend on the profile selected.
 * 
 * Additional observers can be added by the methods AddLocationObserverL() or AddMovementObserverL().
 * 
 * Note: The application must have the Location capability to call this version of NewL().
 * Otherwise, the method will leave with the error KErrPermissionDenied. 
 *  
 * @param[in] aObserver The observer will receive location updates.
 * 
 * @return A pointer to a new instance of the CLbsLocator interface class. The calling application becomes the
 * owner of the returned object and is responsible its disposal.
 *
 * @leave KErrPermissionDenied When the application does not have the Location capability.
 * @leave KErrNoMemory If there has been a memory allocation problem. 
 * @leave OtherStatusCode When if some other issue prevents the observer being added
 * 
 * @capability Location
 * 
 * @see NewL()
 * @see SetLocatorProfile()
 * @see StartUpdatesL()
 * @see StopUpdates()
 * @see AddLocationObserverL()
 * @see RemoveLocationObserver()
 */  
EXPORT_C CLbsLocator* CLbsLocator::NewL(MLbsLocationObserver& aObserver)
	{
	CLbsLocator* newClass = new (ELeave) CLbsLocator();
	CleanupStack::PushL(newClass);
	newClass->ConstructL(&aObserver);
	CleanupStack::Pop(newClass);
	return newClass;
	}


/**
 * Creates a new instance of the CLbsLocator interface class.
 * 
 * After the instance of the interface has been created using this method, the application should select
 * the desired profile using SetLocatorProfile() and add one or more observers. 
 * To begin receiving location and/or movement information, the application must call StartUpdatesL().
 * The frequency and accuracy of these updates depend on the profile selected.
 *
 * Observers can be added by the methods AddLocationObserverL() or AddMovementObserverL().
 * 
 * Note: No platform capabilities are required to create an instance of the interface using this NewL()
 * overload. However, the application must have the Location capability if it intends to received location
 * updates. Platform security capabilities are checked when AddLocationObserverL() is called.
 * No capabilities are required to receive movement events using AddMovementObserverL().       
 * 
 * @return A pointer to a new instance of the CLbsLocator interface class. The calling application becomes the
 * owner of the returned class and is responsible its disposal.
 * 
 * @see NewL(MLbsLocationObserver& aObserver)
 * @see SetLocatorProfile()
 * @see AddLocationObserverL()
 * @see AddMovementObserverL()
 * @see StartUpdatesL()
 * @see StopUpdates()
 */  
EXPORT_C CLbsLocator* CLbsLocator::NewL()
    {
    CLbsLocator* newClass = new (ELeave) CLbsLocator();
    CleanupStack::PushL(newClass);
    newClass->ConstructL(NULL);
    CleanupStack::Pop(newClass);
    return newClass;
    }


//=============================================================================
// CLbsLocator - Set / Get Profile
//=============================================================================

/**
 * Specify the active profile to use for location and status updates.
 * 
 * The profile should initially be selected before StartUpdatesL() is called but can be 
 * changed at any time. Selecting a profile automatically affects subsequent updates.
 * There is no need to request that updates are stopped and/or re-started. 
 *
 * If no profile is selected by an application the "Default" profile is used (KLbsProfileIdDefault).
 *
 * Available Profiles are:
 * - KLbsProfileIdDefault
 * - KLbsProfileIdAreaMonitor
 * - KLbsProfileIdLazyTracker
 * 
 * The "Default" profile (KLbsProfileIdDefault) allows existing S60 location based applications
 * to easily move to the CLbsLocator API. 
 * - The target accuracy of this profile is 20 meters or better.
 * - Applications will initially receive a cached location if available.
 * - The minimum time between location updates is 1 second.
 * - No movement information is supported.
 * 
 * The "Area Monitor" profile (KLbsProfileIdAreaMonitor) is designed to allow applications
 * to discover the town or which part of a city the device is currently situated.
 * The profile is designed to be power efficient and is suitable for long running applications
 * that only need to know when their approximate location changes.
 * - The target accuracy of this profile is 3km.
 * - Applications will initially receive a cached location if available.
 * - Location updates are generated when the device moves between 100m and 3km.
 * - Stationary and Movement detection are supported.
 * - Movement updates indicate the device has moved more than 100m (but more typically 3km).
 * 
 * The "Lazy Tracker" profile (KLbsProfileIdLazyTracker) enables applications to record
 * the movements of the device over an extended period of time. A key goal of this
 * profile is to be more power efficient than the default profile. This profile will
 * attempt to save power by various techniques including switching GPS off if the device
 * has been stationary for an extended period. It will also attempt to use other
 * positioning technologies such as WLAN (if available) when it is not possible to
 * obtain a satellite based location.
 * The profile is not normally suited to turn-by-turn navigation.  
 * - The target accuracy of this profile is 20m or better.
 * - Applications will initially receive a cached location if available.
 * - Location updates are generated when the device moves 30 meters or more.
 * - Stationary, Movement and Walking detection are supported.   
 * 
 * For all profiles, if position of the target accuracy cannot be obtained within the time out period,
 * the location observer will receive an error notification of KErrTimedOut. See SetLocatorOption().  
 *
 * @param[in] aProfileId The UID of the profiles will receive location and status updates.
 * @return KErrNone When the specified profile was successfully selected.
 * @return KErrArgument When parameter aProfileId contains an invalid profile ID.
 * @return KErrNotSupported When parameter aProfileId contains a profile that is not currently available.
 * @see StartUpdatesL()
 * @see StopUpdates()
 * @see LocatorProfile()
 * @see SetLocatorOption()
 */ 
EXPORT_C TInt CLbsLocator::SetLocatorProfile(TUid /*aProfileId*/)
	{
	return KErrNotSupported;
	}

/**
 * Return the ID of the profile in use.
 * 
 * If the application has not selected a profile, the "Default" profile is used.
 * In this situation LocatorProfile() will return KLbsProfileIdDefault. 
 * @return The UID of the profile currently in use.
 * @see SetLocatorProfile()
 */ 
EXPORT_C TUid CLbsLocator::LocatorProfile() const
    {
    return KLbsProfileIdDefault;
    }

//=============================================================================
// CLbsLocator - Start, Stop and Force Updates
//=============================================================================

/**
 * Requests the location related updates are sent to the observers.
 * 
 * After the instance of the interface has been created, the application should select
 * the desired profile using SetLocatorProfile() and ensure there is at least one observer. 
 * To begin receiving location information, the application must call StartUpdatesL().
 * 
 * After calling StartUpdatesL(), the observers will receive a cached location and current
 * movement status if available. The frequency and accuracy of subsequent updates depend on
 * the active profile. See SetLocatorProfile() for more information.
 * 
 * The StopUpdates() method should be used when an application no longer wishes to receive
 * any new location, movement or status updates.
 * 
 * Calling StartUpdatesL() when there are no specified observers will not result in an error but
 * will consume system resources and may drain additional battery power.
 * 
 * NewL()
 * @see SetLocatorProfile()
 * @see StopUpdates()
 * @see AddLocationObserverL()
 * @see RemoveLocationObserver()
 */ 
EXPORT_C void CLbsLocator::StartUpdatesL()
	{
	}

/**
 * Requests that all location and status updates are stopped.
 * 
 * The StopUpdates() method is used when an application no longer wishes to receive
 * new location, movement or status updates. It can be used to temporarily or permanently
 * halt updates. It is recommended (but not obligatory) to call StopUpdates() before the
 * destructor is called.
 *
 * It is legal and often convenient to call StopUdatesL() during an update callback.
 * For example, within the application's HandleLocationUpdate() method. This pattern
 * can be used when the application only wants to find the current location and has received an
 * update of satisfactory accuracy.
 * 
 * When there are multiple observers and the application calls StopUpdates() from within an
 * observer's callback, the current update will continue to be delivered to any remaining
 * observers. That is, StopUpdates() prevents subsequent (i.e. new) updates being generated.  
 *  
 * To re-start updates the StartUpdatesL() method must be used. That re-activates the
 * profile from the beginning. In particular, a cached position will initially be sent to the
 * application.
 * 
 * @see StartUpdatesL()
 * @see RemoveLocationObserver()
 * @see ~CLbsLocator()
 */ 
EXPORT_C void CLbsLocator::StopUpdates()
	{
	}

//=============================================================================
// CLbsLocator - Set and Get Options
//=============================================================================

/**
 * Modifies the behaviour of the currently active profile.
 *
 * 
 * When an application changes the active profile using SetLocatorProfile()any previously modified
 * option is lost. Applications must re-specify its desired options each time it changes profiles.   
 * 
 * Changing a profile option has effect immediately. 
 *
 * Parameter aOption can be one of the following
 * - <I>ELocatorStatusTimer</I>
 *   - Parameter aValue is the timer period in seconds.\n\n
 *   - The status timer indicates how long the subsystem should wait before informing the application
 *     that it has not been able to obtain a position of the target accuracy for the current profile.
 *     The method MLbsLocationObserver::HandleLocationStatus() is called after that period.\n\n
 *     Note: The status timer does NOT indicate how long the subsystem should try and get accurate
 *     positions. It only controls when the application is informed about the accuracy or position loss.\n\n
 *     The location subsystem will continue to try and obtain accurate positions until the 
 *     application calls StopUpdates().\n\n
 *   - A value of -1 is the default and indicates that profile specific logic should be used.\n\n
 *   - A value of 0 can be used by the application when it wants to know if a cached
 *     position for the current location is NOT available.\n\n
 *     When a cached position is available, it is always the first location sent to the application
 *     after it calls StartUpdatesL(). When there is no cache position available the subsystem
 *     will (normally) start to calculate a new position start away.\n\n 
 *     Setting the Status Timer to 0 gives the application an opportunity to prevent a new position
 *     being calculated when there is no cached position. In this situation, the status message
 *     KPositionNotAvailable is sent to the application after it calls StartUpdatesL().
 *     If the application does not want a new location to be calculated it should call StopUpdates()
 *     during the status callback. If updates are not stopped at this stage then a new position
 *     will be calculated.\n\n
 *     After the cached position or the initial status message has been sent to the application,
 *     the default value of the status timer is then used.\n\n
 *     Note: Although this mechanism allows an application to query the cached position it is not
 *     intended for "listening" for location updates. Applications should use one of the 
 *     alternative intefaces if this is required.
 *
 * For more information on target accuracies and supported options for each profile see SetLocatorProfile(). 
 * 
 * @param[in] aOption The profile option to be modified from TLbsLocatorOption. 
 * @param[in] aValue The new value of option.
 * @return KErrNone When the specified option and value were accepted.
 * @return KErrNotSupported When parameter aOption is not supported by the current profile. 
 * @return KErrArgument When the parameter aValue is not with the allowed range for aOption. 
 * 
 * 
 * @see TLbsLocatorOption
 * @see MLbsLocationObserver::HandleLocationStatus()
 * @see SetLocatorProfile()
 * @see GetLocatorOption()
 */
EXPORT_C TInt CLbsLocator::SetLocatorOption(TLbsLocatorOption /*aOption*/, TInt /*aValue*/)
	{
	return KErrNotSupported;
	}


/**
 * Retrieves the value for of an Option of the currently active profile.
 *
 * @param[in] aOption The profile option to be modified from TLbsLocatorOption. 
 * @param[out] aValue The current value of option.
 * @return KErrNone When the specified current value was successfully retrieved.
 * @return KErrNotSupported When parameter aOption is not supported by the current profile. 
 * @see TLbsLocatorOption
 * @see SetLocatorOption
 * @see SetLocatorProfile()
 */
EXPORT_C TInt CLbsLocator::GetLocatorOption(TLbsLocatorOption /*aOption*/, TInt& /*aValue*/) const
	{
	return KErrNotSupported;
	}

//=============================================================================
// CLbsLocator - Add and Remove Location Observers
//=============================================================================


/**
 * Adds a Location Observer.
 * 
 * The specified observer will start to receive location updates when they are next generated.
 * It is possible to have multiple location observers.
 * 
 * Location observers can be specified using AddLocationObserverL() or (optionally) when
 * the interface class is created via the NewL(MLbsLocationObserver& aObserver) overload.  
 * To begin receiving updates the application must first call StartUpdatesL().
 *
 * Observers can be added before or after StartUpdatesL() is called. If an observer is after
 * updates have been started it will automatically receive the next location update.
 * 
 * Attempting to add an observer that is already present has no effect. That is, duplicate
 * observers are not added but the method does not leave.
 * 
 * Location observers can be removed using RemoveLocationObserver().
 * 
 * In order to add a location observer, the application must have the Location capability.
 * Otherwise, this method will Leave with the error KErrPermissionDenied.
 *    
 * The observer will not be added if the method leaves. 
 *
 * @param[in] aObserver The observer to be added.
 * 
 * @leave KErrPermissionDenied When the application does not have the Location capability.
 * @leave KErrNoMemory If there has been a memory allocation problem. 
 * @leave OtherStatusCode When if some other issue prevents the observer being added
 * @capability Location
 * 
 * @see StartUpdatesL()
 * @see NewL(MLbsLocationObserver& aObserver)
 * @see RemoveLocationObserver()
 */
EXPORT_C void CLbsLocator::AddLocationObserverL(MLbsLocationObserver& /*aObserver*/)
	{
	User::Leave(KErrNoMemory);
	User::Leave(KErrPermissionDenied);
	}

/**
 * Removes a Location Observer. The specified observer will no longer receive location updates.
 * 
 * It is possible to remove any of the current observers - including that supplied via
 * NewL(MLbsLocationObserver& aObserver) overload.
 * 
 * Removing all observes without calling StopUpdates() may continue to consume resources 
 * and is not recommended.
 * 
 * Observers may be removed whilst the application is currently processing an update.
 * For example, from within one observer's MLbsLocationObserver::HandleLocationUpdate() method.
 * In this situation, the removed observer will not receive the current update (if it has
 * has still to be delivered to it).  
 * 
 * @param[in] aObserver The observer to be removed.
 * @return KErrNone When the specified observer has been removed. 
 * @return KErrNotFound When aObserver could not be found in the current list of location observers
 * 
 * @see NewL(MLbsLocationObserver& aObserver)
 * @see AddLocationObserverL()
 * @see StopUpdates()
 */
EXPORT_C TInt CLbsLocator::RemoveLocationObserver(MLbsLocationObserver& /*aObserver*/)
	{
	return KErrNotSupported;
	}


//=============================================================================
// CLbsLocator - Add and Remove Movement Observers
//=============================================================================


/**
 * Adds a Movement Observer.
 * 
 * The specified observer will start to receive movement event updates when they are next
 * generated. It is possible to have multiple movement observers and no location observers.
 * 
 * To begin receiving updates the application must first call StartUpdatesL().
 *
 * Observers can be added before or after StartUpdatesL() is called. If an observer is after
 * updates have been started it will automatically receive the next movement update.
 * 
 * The range of movement detection supported and the power consumed is dependent on
 * the profile being used. See SetLocatorProfile() for more information.   
 *
 * Attempting to add an observer that is already present has no effect. That is, duplicate
  * observers are not added but the method does not leave.
 * 
 * Movement observers can be removed using RemoveMovementObserver().
 * 
 * Note: No security capabilities are required to add a movement observer or receive movement events.
 *  
 * The observer will not be added if the method leaves. 
 *
 * @param[in] aObserver The observer to be added.
 * 
 * @leave KErrNoMemory If there has been a memory allocation problem. 
 * @leave OtherStatusCode When if some other issue prevents the observer being added
 *
 * @param[in] aObserver The observer to be added.
 * @see SetLocatorProfile()
 * @see StartUpdatesL()
 * @see NewL()
 * @see RemoveMovementObserver()
 */
EXPORT_C void CLbsLocator::AddMovementObserverL(MLbsMovementObserver& /*aObserver*/)
    {
    }

/**
 * Removes a Movement Observer. The specified observer will no longer receive Movement updates.
 * 
 * Removing all observes without calling StopUpdates() may continue to consume resources 
 * and is not recommended.
 * 
 * Observers may be removed whilst the application is currently processing an update.
 * For example, from within one observer's MLbsMovementObserver::HandleMovementUpdate() method.
 * In this situation, the removed observer will not receive the current update (if it has
 * has still to be delivered to it).    
 *
 * @param[in] aObserver The observer to be removed.
 * @return KErrNone When the specified observer has been removed. 
 * @return KErrNotFound When aObserver could not be found in the current list of Movement observers
 * @see AddMovementObserverL()
 * @see StopUpdates()
 */
EXPORT_C TInt CLbsLocator::RemoveMovementObserver(MLbsMovementObserver& /*aObserver*/)
    {
    return KErrNotSupported;
    }
