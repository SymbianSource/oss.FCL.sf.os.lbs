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

#ifndef __LBSMOVEMENTEVENT_H__
#define __LBSMOVEMENTEVENT_H__

#include <e32base.h>
#include <lbslocatorclasstypes.h>

/**
@file
@internalTechnology
@prototype
*/

/**
 * TLbsMovementEvent - Contains the movement status of the device.
 *
 * To receive movement events, the application must add an movement event observer using
 * CLbsLocator::AddMovementObserverL() and implement the callback method
 * MLbsMovementObserver::HandleMovementUpdate().
 *
 * To begin receiving updates, the application should call CLbsLocator::StartUpdatesL().
 * Although this only needs to be called once irrespective of the number or type of
 * observers.
 *
 * Not all profiles support movement events. Some profiles only support a limited
 * subset of the possible event types. See CLbsLocator::SetLocatorProfile() for information
 * about the movement events supported by each profile.
 * 
 */
class TLbsMovementEvent : public TLbsLocatorClassTypeBase
    {
public:
    /** TLbsMovementEventType - Contain a bit mask of movement event types.
	  * For example, the application be notified that both walking and general movement has been detected.
	  * Note: This enum may be expanded to include other types of events related to movement.
	  */
    enum TLbsMovementEventType
        {
	EMovementEventUnknown    = 0,
    EMovementEventStationary = 1,
	EMovementEventMoving     = 2,
	EMovementEventWalking    = 4,
	EMovementEventLastBit    = 1 << 31
        };

    IMPORT_C TLbsMovementEvent();

    IMPORT_C TLbsMovementEventType MovementEvent() const;        
    IMPORT_C TTime SystemTimeStamp() const;        

	IMPORT_C void SetMovementEvent(TLbsMovementEventType aEvent);   
    IMPORT_C TInt SetSystemTimeStamp(TTime aTimeStamp);
  
private:
    TLbsMovementEventType iEvent;
    TTime   iSystemTimeStamp;
    TInt    iReserved[24];
    };


#endif //__LBSMOVEMENTEVENT_H__
