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

#ifndef __LBSMOVEMENTOBSERVER_H__
#define __LBSMOVEMENTOBSERVER_H__

/**
@file
@internalTechnology
@prototype
*/

#include <e32base.h>
#include <lbsmovementevent.h>

/**
 * MLbsMovementObserver - Provides callback methods for CLbsLocator to send movement events
 * to the application.  
 * 
 * Movement events provide an indication of wherether the user is stationary or moving.
 * Potentially, they may also indicate whether the user is walking.
 * 
 * The range of movement detection supported and the power consumed is dependent on
 * the performance profile being used.
 * 
 * Applications need only to implement the HandleMovementUpdate() method of this class.
 * If status information is important then HandleMovementStatus() should also be defined.
 *  
 * @see CLbsLocator
 * @see TLbsMovementEvent
 */
class MLbsMovementObserver
    {
public: 
    virtual void HandleMovementUpdate(const TLbsMovementEvent& aEvent) = 0;
    virtual void HandleMovementStatus(TInt aStatus);
    virtual TVersion Version() const;
    };


#endif //__LBSMOVEMENTOBSERVER_H__
