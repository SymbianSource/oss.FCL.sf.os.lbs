// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file npeclientstub.cpp
*/
#include <e32base.h>
#include "clocationengineapiimpl.h"

EXPORT_C CNpeApi::~CNpeApi()
    {
	RDebug::Print(_L("CNpeApi::~CNpeApi"));
  
    CNpeApiImpl::Cleanup();
    // Do nothing
    }

/**
 * Two-phased constructor.
 */
EXPORT_C CNpeApi* CNpeApi::NewL(MNpeObserver* aObservers)
    {
    CNpeApi* self = new(ELeave) CNpeApi(aObservers);
    return self;
    }

/**
 * Two-phased constructor.
 */
EXPORT_C CNpeApi* CNpeApi::NewLC(MNpeObserver* aObservers)
    {
    CNpeApi* self = new(ELeave) CNpeApi(aObservers);
    CleanupStack::PushL(self);
    return self;
    }

// API Version
EXPORT_C void CNpeApi::Version(TVersion& aApi)
    {
    CNpeApiImpl::NpeApiImpl().Version(aApi);
    }


// Delete Assistance Data
EXPORT_C void CNpeApi::DeleteAssistanceData(TNpeAdTypes aData)
    {
    CNpeApiImpl::NpeApiImpl().DeleteAssistanceData(aData);
    }

// Misc. interfaces
EXPORT_C void CNpeApi::SetClientUsage(TNpeClientInfo aClientInfo)
    {
    CNpeApiImpl::NpeApiImpl().SetClientUsage(aClientInfo);
    }

EXPORT_C void CNpeApi::SetGpsOptions(TNpeGpsMode aGpsMode, TBool aGpsTimeRelationReq)
    {
    CNpeApiImpl::NpeApiImpl().SetGpsOptions(aGpsMode, aGpsTimeRelationReq);
    }

EXPORT_C void CNpeApi::AdvisePowerMode(TNpePowerMode aPowerMode)
    {
    CNpeApiImpl::NpeApiImpl().AdvisePowerMode(aPowerMode);    
    }

// Position interface
EXPORT_C void CNpeApi::PositionRequest(const TTime& aTargetTime, TInt aMaxFixTime, TInt aHorUncertainty, TInt aVerUncertainty)
    {
    CNpeApiImpl::NpeApiImpl().PositionRequest(aTargetTime, aMaxFixTime, aHorUncertainty, aVerUncertainty);
    }

EXPORT_C void CNpeApi::CancelPositionRequest(void)
    {
    CNpeApiImpl::NpeApiImpl().CancelPositionRequest();
    }

// Assistance data delivery interface
EXPORT_C void CNpeApi::DeliverAssistanceData(TLbsAsistanceDataGroup aDataGroup, RLbsAssistanceDataReaderSet &aDataReaderSet)
    {
    CNpeApiImpl::NpeApiImpl().DeliverAssistanceData(aDataGroup, aDataReaderSet);
    }

/**
 * Constructor for performing 1st stage construction
 */
CNpeApi::CNpeApi(MNpeObserver* aObservers)
    {
    CNpeApiImpl::InstantiateL(aObservers);
    }

EXPORT_C TAny* CNpeApi::ExtendedInterface(TInt /*aFunctionNumber*/, TAny* /*aPtr1*/, TAny* /*aPtr2*/)
	{
	return NULL;
	}

// end of file
