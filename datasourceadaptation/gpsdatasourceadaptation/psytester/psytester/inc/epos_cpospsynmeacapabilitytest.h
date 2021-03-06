/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CPOSPSYNMEACAPABILITYTEST_H
#define CPOSPSYNMEACAPABILITYTEST_H

//  INCLUDES
#include "epos_cpospsycapabilitytestbase.h"

// CLASS DECLARATION
/**
*  Class for testing NMEA capability
*/
class CPosPSYNMEACapabilityTest : public CPosPSYCapabilityTestBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPosPSYNMEACapabilityTest* NewLC(
        /* IN/OUT */ CPosPSYResults& aResults,
        /* IN */     const RPointerArray<TPositionInfo>& aPositionArray,
        /* IN */     const TPosPSYInfo& aPSYInfo
        );

        /**
        * Destructor.
        */
        virtual ~CPosPSYNMEACapabilityTest();

    public:  // Functions from base classes

        /**
        * Function for testing NMEA capability, if set
        */
        void TestCapabilityL();

    private:

        /**
        * C++ constructor.
        */
        CPosPSYNMEACapabilityTest(
        /* IN/OUT */ CPosPSYResults& aResults,
        /* IN */     const RPointerArray<TPositionInfo>& aPositionArray,
        /* IN */     const TPosPSYInfo& aPSYInfo
        );

        /**
        * Function for checking result of capability test
        */
        void CheckResultsL();

        // By default, prohibit copy constructor
        CPosPSYNMEACapabilityTest( const CPosPSYNMEACapabilityTest& );
        
        // Prohibit assigment operator
        CPosPSYNMEACapabilityTest& operator= ( const CPosPSYNMEACapabilityTest& );

    private:    // Data
        
        TInt iNrOfIncompleteNmea;
        TInt iNrOfNMEANotSet;
        TInt iNrOfNMEAStartNotSet;
        TInt iNrOfEmptyNMEASentences;
    };

#endif      // CPOSPSYNMEACAPABILITYTEST_H

// End of File
