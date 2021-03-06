/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CPOSITIONEREXTENSION_H
#define CPOSITIONEREXTENSION_H

#include <e32base.h>
#include <lbscommon.h>

// FORWARD DECLARATIONS
class MPosStatusObserver;
class MPosParameterObserver;
class CPositionerRegistry;
class CPosPsyExtension;

// CLASS DECLARATION

/**
* Class used to extend the CPositionerExtension member data list. Holds 
* subsession specific information and services.
*/
class CPositionerExtension : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Creates a CPositionerExtension instance.
        *
        * @param aImplementationUid The UID of the CPositioner implementation
        *                           to instantiate.
        * @param aStatusObserver The server object watching for status updates.
        * @param aParamObserver The server object which holds the positioning
        *        parameter values.
        * @return a CPositionerExtension instance.
        */
        static CPositionerExtension* NewL(
        /* IN  */       TUid aImplementationUid,
        /* IN  */       MPosStatusObserver* aStatusObserver,
        /* IN  */       MPosParameterObserver* aParamObserver
        );

        /**
        * Destructor.
        */
        ~CPositionerExtension();

    private:

        /**
        * 2nd phase constructor.
        */
        void ConstructL(
        /* IN  */       TUid aImplementationUid,
        /* IN  */       MPosStatusObserver* aStatusObserver
        );

        /**
        * C++ default constructor.
        */
        CPositionerExtension(
        /* IN  */       MPosParameterObserver* aParamObserver
        );

        // By default, prohibit copy constructor
        CPositionerExtension( const CPositionerExtension& );
        // Prohibit assigment operator
        CPositionerExtension& operator= ( const CPositionerExtension& );

    public: // Data

        MPosParameterObserver*  iParamObserver;
        CPosPsyExtension*       iPsyExtension;

    private:

        CPositionerRegistry*    iRegistry;
        
    };  

#endif  // CPOSITIONEREXTENSION_H

// End of File
