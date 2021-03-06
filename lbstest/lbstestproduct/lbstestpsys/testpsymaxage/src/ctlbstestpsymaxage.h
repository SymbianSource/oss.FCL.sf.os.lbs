/*
* Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __CT_LBS_TEST_PSY_MAX_AGE_H__
#define __CT_LBS_TEST_PSY_MAX_AGE_H__

//  INCLUDES
#include <e32def.h>
#include <e32base.h>	
#include <lbs/epos_cpositioner.h>
#include <lbspositioninfo.h>
#include <lbserrors.h>


// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* PSY used in MaxAge tests TP217, TP218 and TPXXX
* This Psy supports maxage
*
*/
class CT_LbsTestPsyMaxAge : public CPositioner
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CT_LbsTestPsyMaxAge* NewLC(TAny* aConstructionParameters);
        
        /**
        * Two-phased constructor.
        */
        static CT_LbsTestPsyMaxAge* NewL(TAny* aConstructionParameters);
        
        /**
        * Destructor.
        */
        ~CT_LbsTestPsyMaxAge();

    protected:  // Functions from base classes
        
       /**
		* Requests position info asynchronously.
		*
		* @param aPosInfo A reference to a position info object. This object
		*                 must be in scope until the request has completed.
		* @param aStatus The request status
		*/
        void NotifyPositionUpdate(
        /* IN/OUT */    TPositionInfoBase& aPosInfo,
		/* OUT    */	TRequestStatus& aStatus
		);

		/**
        * From CPositioner. Cancels position info request.
        */
        void CancelNotifyPositionUpdate(); 

    private:

        /**
        * C++ default constructor.
        */
        CT_LbsTestPsyMaxAge();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL(TAny* aConstructionParameters);

    private:

        TPositionInfo iOldPositionInfo;
        TTime iTimeOfOldestFix;
        TTime iMaxAge;
        TReal32 iHorAccuracy;
        TBool iEveryOther;

    };

#endif      // __CT_LBS_TEST_PSY_MAX_AGE_H__   
            
// End of File
