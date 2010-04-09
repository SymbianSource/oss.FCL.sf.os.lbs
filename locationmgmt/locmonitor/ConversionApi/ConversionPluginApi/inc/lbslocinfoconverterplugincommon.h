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
* Description: Base class for all the class representing
* location informations.
*
*/

/**
@file
@publishedPartner
@prototype
*/

#ifndef LBS_LOCATION_CONVERTER_PLUGIN_COMMON_H
#define LBS_LOCATION_CONVERTER_PLUGIN_COMMON_H

#include <e32std.h>


/**
The generic base for classes that store conversion Plugin Interface 
related information
*/
class TLbsLocInfoConverterPluginClassTypeBase
	{
public:
	IMPORT_C TUint32 ClassType() const;
	IMPORT_C TUint ClassSize() const;

protected:
	IMPORT_C TLbsLocInfoConverterPluginClassTypeBase();

protected:
	/** The type of the derived class */
	TUint32 iClassType;
	/** The size of the derived class */
	TUint iClassSize;
	};


#endif // LBS_LOCATION_CONVERTER_PLUGIN_COMMON_H
