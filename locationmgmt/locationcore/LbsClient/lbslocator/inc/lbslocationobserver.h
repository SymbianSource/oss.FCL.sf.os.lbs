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

#ifndef __LBSLOCATIONOBSERVER_H__
#define __LBSLOCATIONOBSERVER_H__

/**
@file
@internalTechnology
@prototype
*/

#include <lbslocation.h>

/**
 * MLbsLocationObserver - Provides callback methods for CLbsLocator to send location updates
 * and status reports to the application.  
 * 
 * Location updates provide Latitude and Longitude information and may contain Altitude,
 * Course and Speed data. 
 * 
 * The frequency and accuracy of these updates and the power consumed is dependent on
 * the performance profile being used.
 * 
 * Applications need only to implement the HandleLocationUpdate() method of this class.
 * If status information is important then HandleLocationStatus() should also be implemented.
 * 
 * @see CLbsLocator
 * @see TLbsLocation
 */
class MLbsLocationObserver
	{
public: 
	virtual void HandleLocationUpdate(const TLbsLocation& aLocation) = 0;
	virtual void HandleLocationStatus(TInt aStatus);
	virtual TVersion Version() const;
	};

#endif //__LBSLOCATIONOBSERVER_H__
