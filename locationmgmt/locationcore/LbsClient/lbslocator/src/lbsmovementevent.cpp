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

#include <lbsmovementevent.h>

//=============================================================================
// Constructor
//=============================================================================

EXPORT_C TLbsMovementEvent::TLbsMovementEvent()
	{
	iClassSize = sizeof(TLbsMovementEvent);
	iClassType |= ELbsMovementEventClassType;

	iEvent = EMovementEventUnknown;
    
#pragma message ("***Need to initiase timestamp***")
    //iSystemTimeStamp = ...
	}



//=============================================================================
// Movement Events
//=============================================================================

/**
  * Retrieves a bit mask of detected movement events.
  * 
  * @return EMovementEventUnknown When no event as been assigned.
  *
  * see TLbsMovementEventType
  */
EXPORT_C TLbsMovementEvent::TLbsMovementEventType TLbsMovementEvent::MovementEvent() const
     {
     return iEvent;
     }

/**
 * Sets a bit mask of detected movement events.
 * 
 * @param[in] aEvent A bit mask of detected movement events.
 * 
 * @see TLbsMovementEventType
 * @see MovementEvent()
 */
EXPORT_C void TLbsMovementEvent::SetMovementEvent(TLbsMovementEventType aEvent)
   {
   iEvent = aEvent;
   }


//=============================================================================
// Timestamp
//=============================================================================

/**
  * Retrieves the system time when the movement event was generated.
  * 
  * If the time stamp has not been assigned, a TTime of "zero" will be returned.
  * @return The time when the movement event was detected. 
  */
EXPORT_C TTime TLbsMovementEvent::SystemTimeStamp() const
     {
     return iSystemTimeStamp;
     }

/**
 * Sets the system time when the movement event was detected.
 * 
 * @param[in] aTimeStamp The system time when the movement event was detected. The value cannot be zero.
 * 
 * @return KErrNone If the time stamp has been set.
 * @return KErrArgument If the parameter is invalid. For example, a time stamp of zero was supplied.   
 * @see SystemTimeStamp()
 */
EXPORT_C TInt TLbsMovementEvent::SetSystemTimeStamp(TTime aTimeStamp)
   {
   iSystemTimeStamp = aTimeStamp;
   return KErrNone;
   }
