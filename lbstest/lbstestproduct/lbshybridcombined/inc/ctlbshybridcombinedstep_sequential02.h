// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This contains the header file for Location Data Source Tests
// 
//

#ifndef __CT_LBS_HYBRIDCOMBINED_SEQUENTIAL02_H__
#define __CT_LBS_HYBRIDCOMBINED_SEQUENTIAL02_H__

// LBS includes.
#include <lbs.h>
#include <lbscommon.h>
#include <lbs/lbsprivacycontroller.h>

// LBS test includes.
#include "ctlbshybridcombinedstep_base.h"
#include <lbs/test/posserverwatch.h>

// Literals used
_LIT(KLbsHybridCombinedStep_Sequential02, "LbsHybridCombinedStep_Sequential02");

class CT_LbsHybridCombinedStep_Sequential02 :	public	CT_LbsHybridCombinedStep_Base, public MPosServerObserver, public MLbsPrivacyObserver
	{
public:
	~CT_LbsHybridCombinedStep_Sequential02();

	static CT_LbsHybridCombinedStep_Sequential02* New(CT_LbsHybridCombinedServer& aParent);
	virtual TVerdict doTestStepL();

	// MPosServerObserver
	void OnGetLastKnownPosition(TInt32 aErr, const TPositionInfoBase& aPosInfo);
	void OnNotifyPositionUpdate(TInt32 aErr, const TPositionInfoBase& aPosInfo);
	
	// From MLbsPrivacyObserver
	void ProcessNetworkLocationRequest(TUint aRequestId, const TLbsExternalRequestInfo& aRequestInfo, const TNotificationType& aNotificationType);
	void ProcessNetworkPositionUpdate(TUint aRequestId, const TPositionInfo& aPosInfo);
	void ProcessRequestComplete(TUint aRequestId, TInt aReason);
	
protected:
	CT_LbsHybridCombinedStep_Sequential02(CT_LbsHybridCombinedServer& aParent);
	void ConstructL();
	
private:
	enum TState
		{
		EInitializing,
		ERefLocReceived,
		EGpsLocReceived,
		EPrivacyCheckOk,
		ERequestComplete
		};

	TState iState;
	TBool iPositionUpdateRef;
    CLbsPrivacyController* iController;
    CNetProtocolProxy* iProxy;
	TLbsNetSessionId iSessionId;
	TLbsNetSessionId iSessionId2;
	};

#endif //__CT_LBS_HYBRIDCOMBINED_SEQUENTIAL02_H__

