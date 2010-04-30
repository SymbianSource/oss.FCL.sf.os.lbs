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

#ifndef __LBSLOCATOR_H__
#define __LBSLOCATOR_H__

/*! \mainpage Locator API (CLbsLocator)
 *
 * Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.\n\n
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".\n\n
 * 
 * Initial Contributors:\n\n
 * - Nokia Corporation - initial contribution.\n\n
 * 
 * Contributors:\n\n
 * 
 * Description:\n\n
 * 
 * <b>API Version 0.5. Status: Draft.</b>
 * 
 * The interface class CLbsLocator provides a simple but powerful means of obtaining
 * location and movement information. Application should #include <lbslocator.h> and
 * link against the library lbslocator.lib
 * 
 * A location contains a latitude and longitude and may include altitude, speed and direction.
 * Movement information provides a general indication of whether the device is stationary or moving.
 * It may also indicate whether the user is walking.
 * 
 * Applications request location and movement information via CLbsLocator with updates being
 * sent to observer classes. To receive Location information, the application must derive from
 * MLbsLocationObserver. The data class TLbsLocation is supplied as the callback parameter and
 * holds the location information. 
 * 
 * Movement updates are sent to the application via the observer class MLbsMovementObserver.
 * The movement details are passed as a callback parameter of type TLbsMovementEvent.
 * 
 * The CLbsLocator API supersedes the RPositionServer/RPositioner interface. However, those interfaces are
 * still supported.
 * 
 * The API abstracts which technologies are used to provide location and movement information.
 * Although the underlying technologies could include GPS, Network Cell, W-LAN positioning,
 * accelerometer and compass, these are hidden from the application.
 * 
 * The application indicates its desired level of accuracy and performance by specifying the
 * <I>profile</I> to use. Profiles are predefined by the API and encompass a set of rules and
 * performance targets - for example, position accuracy and the distance between updates.
 * Each profile is suited to a particular use case. The method CLbsLocator::SetLocatorProfile()
 * is used to select the active profile.
 * 
 * This version of the API defines the following profiles:
 * - KLbsProfileIdDefault
 *   - Suitable for legacy applications requiring satellite based position updates every second.\n\n
 * - KLbsProfileIdAreaMonitor
 *   - Enables applications to determine the town or which part of a city the device is
 *    currently situated. Locations are updated as the device moves into other areas.
 *    This profile is designed to use very low power.\n\n
 * - KLbsProfileIdLazyTracker
 *   - Provides a more power effecient means than the default profile of accurately recording
 *     the movements of the device over an extended period. 
 * 
 * See CLbsLocator::SetLocatorProfile() for more information.
 * 
 * The active profile can be dynamically changed by the application. This allows applications
 * to easily switch to a different level of performance depending on the stage of their lifecycle.
 * 
 * <b>Change History</b>
 *  - 0.5: 30 Arpil 2010
 *    - Initial draft contribued to Foundation
 */

/**
@file
@internalTechnology
@prototype
*/

#include <e32base.h>

#include <lbserrors.h>
#include <lbslocation.h>
#include <lbsmovementevent.h>
#include <lbslocationobserver.h>
#include <lbsmovementobserver.h>

/** Target accuracy of 20m. Updates every second. See CLbsLocator::SetLocatorProfile() for more information */
const TUid KLbsProfileIdDefault          = { 0x00000000 }; 
/** Accuracy of approximately 3km. Updates between 100m and 3km. See CLbsLocator::SetLocatorProfile() for more information */
const TUid KLbsProfileIdAreaMonitor      = { 0x00000001 };
/** Target accuracy of 20m. Updates when device moves 30m or more. See CLbsLocator::SetLocatorProfile() for more information */
const TUid KLbsProfileIdLazyTracker      = { 0x00000002 };

//Forward declarations
class CLbsLocatorImpl;

/**
 * CLbsLocator - Interface class to request location and movement information. 
 * 
 * After creating a new instance of the interface via one of the NewL() methods, applications
 * should set their desired profile using SetLocatorProfile(). To begin receiving
 * updates, the StartUpdatesL() method must be called.
 * 
 * The frequency and accuracy of these updates and the power consumed is dependent on
 * the profile being used.
 * 
 * Updates are sent to one or more Observers.
 * The MLbsLocationObserver class is used to receive location updates (i.e. containing
 * latitude and longitude). The MLbsMovementObserver class is used to receive events
 * related to the movement of the device (for example stationary or moving). 
 * 
 * A location observer may be supplied via the NewL() but observers can also be added using
 * AddLocationObserverL() and AddMovementObserverL(). It is possible to have multiple
 * observers for each type of update.
 * 
 * At least one observer should be present before the application calls StartUpdatesL().
 * 
 * Applications must have the Location capability to receive location updates.
 * 
 * @see MLbsLocationObserver
 * @see MLbsMovementObserver  
 */
class CLbsLocator : public CBase
	{
public:
    /** Used a parameter to SetLocatorOption() to tailor the behaviour of the active profile.
     * When the profile is changed, any previously modified option is returned to the default.  
     * @see SetLocatorOption() @see GetLocatorOption()*/
    enum TLbsLocatorOption
        {
		/** Controls when status updates are generated. */
    ELocatorStatusTimer = 1,  
        };

    IMPORT_C static CLbsLocator* NewL();
    IMPORT_C static CLbsLocator* NewL(MLbsLocationObserver& aObserver);

	IMPORT_C ~CLbsLocator();

	// Select the desired profile. If none selected "TLbsProfileIdDefault" is assumed. 
	IMPORT_C TInt SetLocatorProfile(TUid aProfileId);																				 
	IMPORT_C TUid LocatorProfile() const;

	// Start and stop requesting updates. Updates are sent to "observers" of the interface.
	// StartUpdatesL() will consume resources even if there are no observers.
	IMPORT_C void StartUpdatesL();
	IMPORT_C void StopUpdates();

    // Modify the bahviour of the active profile.
	IMPORT_C TInt SetLocatorOption(TLbsLocatorOption aOption, TInt aValue);
	IMPORT_C TInt GetLocatorOption(TLbsLocatorOption aOption, TInt& aValue) const;

	// Add and Remove more position observers.
	// If no observers present, then resouces may be consumed until StopUpdates() is called.
	IMPORT_C void AddLocationObserverL(MLbsLocationObserver& aObserver);
	IMPORT_C TInt RemoveLocationObserver(MLbsLocationObserver& aObserver);

    IMPORT_C void AddMovementObserverL(MLbsMovementObserver& aObserver);
	IMPORT_C TInt RemoveMovementObserver(MLbsMovementObserver& aObserver);
	
private:
	CLbsLocator();
	void ConstructL(MLbsLocationObserver* aObserver);
	
	CLbsLocatorImpl *iImpl;
	};

#endif //__LBSLOCATOR_H__

