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

#include <lbslocatorclasstypes.h>

//=============================================================================
// Constructor
//=============================================================================

EXPORT_C TLbsLocatorClassTypeBase::TLbsLocatorClassTypeBase()
	{
	iClassSize = sizeof(TLbsLocatorClassTypeBase);
	iClassType = ELbsLocatorBaseClassType;
	}

/**
 * Retrives a bit mask containing the type of the locator generated class.
 * @return A bit mask containing the type of the TLbsLocatorClassTypeBase-derived class
 */
EXPORT_C TUint32 TLbsLocatorClassTypeBase::LocatorClassType() const
	{
	return iClassType;
	}

/**
 * Retrieves the size of the locator based class.
 * @return The size of the TPositionClassTypeBase-derived class
 */
EXPORT_C TUint TLbsLocatorClassTypeBase::LocatorClassSize() const
	{
	return iClassSize;
	}
