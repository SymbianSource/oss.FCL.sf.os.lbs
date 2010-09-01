// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file ctlbsclientlastknownpospnslistener.cpp
*/

#include "ctlbsclientlastknownpospnslistener.h"
#include "epos_lastknownlocationpskeys.h"


CT_LbsClientLastKnownPosPnsListener::~CT_LbsClientLastKnownPosPnsListener()
/**
 * 	Destructor
 */
	{
	Cancel();
	}
	

CT_LbsClientLastKnownPosPnsListener::CT_LbsClientLastKnownPosPnsListener(MT_LastKnownPosPnsObserver* aObserver) : CActive(EPriorityIdle), iObserver(aObserver)
/**
 * 	Constructor - will not leave
 */
 	{ 	
 	CActiveScheduler::Add(this);
	}


CT_LbsClientLastKnownPosPnsListener* CT_LbsClientLastKnownPosPnsListener::NewL(MT_LastKnownPosPnsObserver* aObserver)
/**
 *	'public constructor' may leave
 */
	{
	CT_LbsClientLastKnownPosPnsListener* self = new(ELeave)CT_LbsClientLastKnownPosPnsListener(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
    CleanupStack::Pop(self);
	return self;
	}

void CT_LbsClientLastKnownPosPnsListener::ConstructL()
    {
    User::LeaveIfError(iLKPPnsProperty.Attach(KPosLastKnownLocationCategory, 
                                         KPosLastKnownLocation,
                                         EOwnerThread));
    
    TPckg<TPositionInfo> positionDes(iPositionInfo);
    iLKPPnsProperty.Get(positionDes);
    iLKPPnsProperty.Subscribe(iStatus);
    SetActive();
    }
	
void CT_LbsClientLastKnownPosPnsListener::DoCancel()
	{
    iLKPPnsProperty.Cancel();
	}

void CT_LbsClientLastKnownPosPnsListener::RunL()
	{
    TPckg<TPositionInfo> positionDes(iPositionInfo);
    iLKPPnsProperty.Get(positionDes);
    iObserver->NotifyLastKnownPosPnsUpdate(iPositionInfo, iStatus);
	}

void CT_LbsClientLastKnownPosPnsListener::Result(TPositionInfo& aPositionInfo)
    {
    aPositionInfo = iPositionInfo;
    }

TInt CT_LbsClientLastKnownPosPnsListener::RunError(TInt aError)
	{	// called if RunL leaves. aError contains the error code
    return aError;
	}

