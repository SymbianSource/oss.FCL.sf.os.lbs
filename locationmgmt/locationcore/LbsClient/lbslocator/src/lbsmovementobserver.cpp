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

#include "lbsmovementobserver.h"

//=============================================================================
// MLbsLocationLocationObserver
//=============================================================================


/**
 * Informs the application of movement events.
 * 
 * To begin receiving updates the application must call CLbsLocator::StartUpdatesL().
 * 
 * After calling StartUpdatesL(), the first update an application will receive is the current
 * movement status if known. The application will then receive movement events but the
 * frequency and information available depends on the active profile. See
 * CLbsLocator::SetLocatorProfile() for more information.
 * 
 * When an application no longer wishes to receive updates it should call CLbsLocator::StopUpdates().
 * It is legal, valid and usual to stop updates from within the HandleMovementUpdate() callback.
 * 
 * @param[in] aEvent The current movement indicator.   
 * 
 * @see TLbsMovementEvent
 * @see HandleMovementStatus
 * @see CLbsLocator::SetLocatorProfile() 
 * @see CLbsLocator::StartUpdatesL()
 * @see CLbsLocator::StopUpdates()
 */
void MLbsMovementObserver::HandleMovementUpdate(const TLbsMovementEvent& /*aEvent*/)
    {
    }

/**
 * Applications must implement this method to receive movement status updates.
 * 
 * To begin receiving status updates the application must call CLbsLocator::StartUpdatesL().
 * 
 * 
 * When HandleMovementStatus() is called with a negative status code, this indicates there has been an
 * error inside the location subsystem or with the connected hardware. These situations are generally
 * not recoverable and the application would normally stop using the interface. 
 * 
 * When an application no longer wishes to receive updates it should call CLbsLocator::StopUpdates().
 * It is possible to call this method from within the HandleLocationStatus() callback.
 * 
 * 
 * @param[in] aStatus A negative value represents a Symbian OS system wide error. This will normally prevent any
 * movement information from being returned. KErrNone is returned when normal service has been restored.   
 * 
 * @see HandleMovementUpdate
 * @see CLbsLocator::StartUpdatesL()
 * @see CLbsLocator::StopUpdates()
 */
void MLbsMovementObserver::HandleMovementStatus(TInt /*aStatus*/)
    {
    }


/**
 * This method should not be overridden by the application.
 * 
 * It is queried by the location subsystem to determine the version of the observer interface used
 * by the application.
 * 
 */
TVersion MLbsMovementObserver::Version() const
    {
    return TVersion(0,0,1);
    }

