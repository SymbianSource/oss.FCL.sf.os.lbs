// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include <e32base.h>
#include <ecom/ecom.h>
#include <lbs/epos_cpositioner.h>
#include "EPos_MPosParameterObserver.h"
#include "EPos_PositionerConstructParams.h"
#include "EPos_CPositionerExtension.h"
#include "EPos_CPosPsyExtension.h"

// ================= MEMBER FUNCTIONS =======================

/**
* Creates a new instance of a CPositioner implementation.
*
* @param aConstructionParameters Construction parameters for
*                                CPositioner.
* @return The positioner implementation.
*/
EXPORT_C CPositioner* CPositioner::NewL(
    TAny* aConstructionParameters)
    {
    TUid implUid = reinterpret_cast<TPositionerConstructParams*>
            (aConstructionParameters)->iImplementationUid;

    TInt32 offset = static_cast<TInt32> _FOFF(CPositioner, iDtorIdKey);

    TAny* ptr = REComSession::CreateImplementationL(implUid,
                                offset, aConstructionParameters);

    return reinterpret_cast<CPositioner*> (ptr);
    }
    
/**
* Creates a new instance of a CPositioner implementation.
*
* This variant of NewL takes a CPositioner as input. The new
* positioner will get a copy of the construction parameters of the
* specified positioner.
*
* This NewL variant should be used by proxy positioners who need
* to load other positioners. A proxy positioner cannot use the other
* NewL method since the construction parameters are not public.
*
* @param aImplementationUid The implementation uid of the positioner
*   which should be created.
* @param aPositioner The positioner to copy construction parameters
*   from.
* @return The positioner implementation.
*/
EXPORT_C CPositioner* CPositioner::NewL(
    TUid aImplementationUid,
    const CPositioner& aPositioner)
    {
    TPositionerConstructParams params;

    params.iImplementationUid = aImplementationUid;
    params.iParamObserver = aPositioner.iExtension->iParamObserver;
    params.iStatusObserver = 
        aPositioner.iExtension->iPsyExtension->iStatusObserver;

    return NewL(&params);
    }
    
/**
* Destructor.
*/
EXPORT_C CPositioner::~CPositioner()
    {
    delete iExtension;
    REComSession::DestroyedImplementation(iDtorIdKey);
    }

/**
* C++ default constructor.
*/
EXPORT_C CPositioner::CPositioner()
    {
    }

/**
* Creates the internals of the positioner.
*
* This function must be called first thing in the positioner's
* ConstructL() method.
*
* @param aConstructionParameters The construction parameters supplied
*                                in the factory call.
*/
EXPORT_C void CPositioner::BaseConstructL(TAny* aConstructionParameters)
    {
    TPositionerConstructParams* params = 
        reinterpret_cast<TPositionerConstructParams*>(aConstructionParameters);

    iExtension = CPositionerExtension::NewL(
        params->iImplementationUid, 
        params->iStatusObserver,
        params->iParamObserver);
    }

/**
* Indicate if the PSY has overridden tracking. The default
* implementation returns EFalse. Override and return ETrue to use PSY:s
* own timer.
* @return ETrue if PSY has own timer, otherwise EFalse.
*/
EXPORT_C TBool CPositioner::TrackingOverridden() const
    {
    return EFalse;
    }

/**
* Initiate a tracking session.
*
* @param aInterval [in] Interval for position requests.
*/
EXPORT_C void CPositioner::StartTrackingL(
    const TTimeIntervalMicroSeconds& /*aInterval*/)
    {
    User::Leave(KErrNotSupported);
    }

/**
* Stop a periodic update session.
* Any outstanding requests will be cancelled.
*/
EXPORT_C void CPositioner::StopTracking()
    {
    }

/**
* Service a PSY specific request.
*
* The PSY uses the RMessage object to read input parameters, write
* return parameters and complete the request.
*
* ServiceL() should be implemented to leave with code
* KErrNotSupported if the request id is not recognized.
*
* @param aMessage The client server request message object.
*/
EXPORT_C void CPositioner::ServiceL(RMessage2& /*aMessage*/)
    {
    User::Leave(KErrNotSupported);
    }

/**
* Called to return the implementation UID set when calling NewL.
*
* @return The UID of the handler implementation.
*/
EXPORT_C TUid CPositioner::ImplementationUid() const
    {
    return iExtension->iPsyExtension->iImplementationUid;
    }

/**
* Get the quality of service requested by the client.
*
* This method can be used by positioning modules which have the ability
* to deliver different quality of service.
*
* @param aPositionQuality The requested quality of service.
* @return KErrNotFound if there is no requested quality of service,
*   KErrNone if there is.
*/
EXPORT_C TInt CPositioner::GetRequiredPositionQuality(
    TPositionQuality& aPositionQuality) const
    {
    return iExtension->iParamObserver->GetRequiredPositionQuality(
        aPositionQuality);
    }

/**
* Get the earliest allowed time of an old position fix, based on the
* current max age set by the client.
*
* The PSY may save the result from the latest position request and
* return the same result in the next position request if the client
* says it's ok. Max age tells the PSY how old the stored position is
* allowed to be. If the stored position is too old or the PSY does not
* support max age, a normal positioning should be performed.
*
* @param aMaxAge On return, will contain the earliest allowed time of 
*   an old position fix. If no max age is defined aMaxAge will contain 
*   a time set to zero.
*/
EXPORT_C void CPositioner::GetMaxAge(TTime& aMaxAge) const
    {
    iExtension->iParamObserver->GetMaxAge(aMaxAge);
    }

/**
* Checks if the client allows a partial position update.
*
* A partial update result does not need to contain all parameters. The
* only mandatory parameters are latitude, longitude and time of fix.
* Everything else is optional.
*
* If a partial update is returned to the client in a
* NotifyPositionUpdate() call, the completion code must be set to
* KPositionPartialUpdate.
* 
* @return ETrue if partial position updates are allowed, otherwise
*   EFalse.
*/
EXPORT_C TBool CPositioner::IsPartialUpdateAllowed() const
    {
    return iExtension->iParamObserver->IsPartialUpdateAllowed();
    }

/**
* Retrieve the interface for reporting positioner status.
*
* @return The interface for reporting positioner status.
*/
EXPORT_C MPositionerStatus* CPositioner::PositionerStatus()
    {
    return iExtension->iPsyExtension;
    }



/**
 * Cancels the request with the error code provided.
 * 
 * @param aError The error code to cancel request with.
 * 
 * @prototype
 */

EXPORT_C void CPositioner::CancelNotifyPositionUpdate(TInt /*aError*/)
    {
    CancelNotifyPositionUpdate();
    }

//EXPORT_C void CPositioner::CPositioner_Reserved1()
//    {
//    }

/**
 * Reserved function
 */
EXPORT_C void CPositioner::CPositioner_Reserved2()
    {
    }

/**
 * Returns the location update timeout associated with the subsession. The method returns a real timeout,
 * which is defined and used by the Location Server. It may be different from the timeout set by the client
 * or default PSY timeout.
 * 
 * @param aUpdateTimeOut [Out] The location update timeout.
 * 
 * @prototype
 */
EXPORT_C void CPositioner::GetUpdateTimeOut(TTimeIntervalMicroSeconds& aUpdateTimeOut) const
	{
	iExtension->iParamObserver->GetUpdateTimeOut(aUpdateTimeOut);
	}

/**
 * Called to inform the Location Server about the PSY default update timeout. The Location
 * Server may use this value to calculate the update timeout it uses. 
 * When this method is called, it does not affect the currect request. So, it must be called in advance
 * before a request comes from the Location Server. 
 * 
 * @param aUpdateTimeOut [In] The PSY default location update timeout.
 * 
 * @prototype
 */
EXPORT_C void CPositioner::SetPsyDefaultUpdateTimeOut(const TTimeIntervalMicroSeconds& aUpdateTimeOut)
	{
	iExtension->iParamObserver->SetPsyDefaultUpdateTimeOut(aUpdateTimeOut);
	}

/**
 * Extends the active request timeout by the value specified. Only the current active request is 
 * affected and the changes are not stored. That is the value returned by GetUpdateTimeOut and
 * the default PSY timeout set by SetPsyDefaultUpdateTimeOut are not changed. 
 * This method is used, for example, to apply FNP lag.   
 * 
 * @param aUpdateTimeOut [In] The time to extend timeout for.
 * 
 * @prototype
 */
EXPORT_C void CPositioner::ExtendUpdateTimeOut(const TTimeIntervalMicroSeconds& aAdditionalTime)
	{
	iExtension->iParamObserver->ExtendUpdateTimeOut(aAdditionalTime);
	}



//  End of File  
