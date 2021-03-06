// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <lbspositioninfo.h>
#include "ctlbstestpsybase.h"
#include "ctlbstestpsy21crcapmain.h"
#include "ctlbstestpsy21crcap.h"
#include "testpsy21.hrh"

// C++ default constructor can NOT contain any code, that
// might leave.
//
CT_LbsTestPsy21Positioner::CT_LbsTestPsy21Positioner() 
    {
    }

// EPOC default constructor can leave.
void CT_LbsTestPsy21Positioner::ConstructL(TAny* aConstructionParameters)
    {
    BaseConstructL(aConstructionParameters);
	iRequestHandler = CT_LbsTestPsyBase::InstanceL(PositionerStatus());		
	iRequestHandler->SetRequestsHandling(CT_LbsTestPsyBase::EBundling);
        
	TUid implUid = { KPosImplementationUid };
	iRequestHandler->SetModuleId(implUid);
	}

// Two-phased constructor.
CT_LbsTestPsy21Positioner* CT_LbsTestPsy21Positioner::NewLC(TAny* aConstructionParameters)
    {
    CT_LbsTestPsy21Positioner* self = new(ELeave) CT_LbsTestPsy21Positioner;
    CleanupStack::PushL(self); 
    self->ConstructL(aConstructionParameters);
    return self;
    }
    
// Two-phased constructor.
CT_LbsTestPsy21Positioner* CT_LbsTestPsy21Positioner::NewL(TAny* aConstructionParameters)
    {
    CT_LbsTestPsy21Positioner* self = NewLC(aConstructionParameters);
    CleanupStack::Pop();
    return self;
    }
    
// Destructor
CT_LbsTestPsy21Positioner::~CT_LbsTestPsy21Positioner()
    {
	iRequestHandler->Release();
	}

// ---------------------------------------------------------
// CT_LbsTestPsy21Positioner::NotifyPositionUpdate
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CT_LbsTestPsy21Positioner::NotifyPositionUpdate(TPositionInfoBase& aPosInfo, TRequestStatus& aStatus)
	{
	iStatus = &aStatus;
	
	iRequestHandler->NotifyPositionUpdate(static_cast<TPositionInfo*> (&aPosInfo), &aStatus);
	}

// ---------------------------------------------------------
// CT_LbsTestPsy21Positioner::CancelNotifyPositionUpdate
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CT_LbsTestPsy21Positioner::CancelNotifyPositionUpdate()
	{
	iRequestHandler->CancelNotifyPositionUpdate(iStatus);
	}

TBool CT_LbsTestPsy21Positioner::TrackingOverridden() const
{
	return EFalse;
}

