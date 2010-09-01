/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This contains the header file for Get last known position 
* 
*
*/



/**
 @file ctlbsclientlastknownpospnslistener.h
*/
#ifndef __CT_LBS_CLIENT_LKP_PNS_LISTENER_H__
#define __CT_LBS_CLIENT_LKP_PNS_LISTENER_H__

#include <e32base.h>
#include <e32property.h>
#include <lbs.h>

class MT_LastKnownPosPnsObserver
    {
public:
    virtual void NotifyLastKnownPosPnsUpdate(TPositionInfo& aPositionInfo, TRequestStatus& aStatus) = 0;
    };

class CT_LbsClientLastKnownPosPnsListener: public CActive
	{
public:
	~CT_LbsClientLastKnownPosPnsListener();
	static CT_LbsClientLastKnownPosPnsListener* NewL(MT_LastKnownPosPnsObserver* aObserver);
	void Result(TPositionInfo& aPositionInfo);
	
protected:
//	CActive:
	virtual void DoCancel();
	virtual void RunL();
	virtual TInt RunError(TInt aError);

private:
	CT_LbsClientLastKnownPosPnsListener(MT_LastKnownPosPnsObserver* aObserver);
	void ConstructL();
	
private:
	MT_LastKnownPosPnsObserver*  iObserver;
	RProperty      iLKPPnsProperty;
	TPositionInfo  iPositionInfo; 
	};

#endif // __CT_LBS_CLIENT_LKP_PNS_LISTENER_H__
