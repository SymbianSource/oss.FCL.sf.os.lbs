/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implentation of functions shared between all 
* implementations of the LBS Network Protocol Module 
* interface/ECom plugin.
*
*/

#include <e32base.h>
#include <lbs/lbslocinfoconverterplugincommon.h>


// ---------------------------------------------------------------------------
// TLbsLocInfoConverterPluginClassTypeBase::TLbsLocInfoConverterPluginClassTypeBase()
// Default constructor for TLbsLocInfoConverterPluginClassTypeBase
// ---------------------------------------------------------------------------
//
EXPORT_C TLbsLocInfoConverterPluginClassTypeBase::TLbsLocInfoConverterPluginClassTypeBase()
:   iClassType(0),
    iClassSize(0)
    {
    }

// ---------------------------------------------------------------------------
// TLbsLocInfoConverterPluginClassTypeBase::ClassSize()
// Returns the size of the TLbsLocInfoConverterPluginClassTypeBase-derived class
// ---------------------------------------------------------------------------
//
EXPORT_C TUint TLbsLocInfoConverterPluginClassTypeBase::ClassSize() const
    {
    return iClassSize;
    }

// ---------------------------------------------------------------------------
// TLbsLocInfoConverterPluginClassTypeBase::ClassType()
// Returns the type of the TLbsLocInfoConverterPluginClassTypeBase-derived class
// ---------------------------------------------------------------------------
//
EXPORT_C TUint32 TLbsLocInfoConverterPluginClassTypeBase::ClassType() const
    {
    return iClassType;
    }
