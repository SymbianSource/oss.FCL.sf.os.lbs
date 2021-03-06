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
// This contains the header file for Extended protocol module MTLR Cancel
// 
//

#ifndef __CT_LBS_CANCEL_BEFORE_REQ_H__
#define __CT_LBS_CANCEL_BEFORE_REQ_H__

// LBS includes.
#include <lbs.h>
#include <lbscommon.h>
#include <lbs/lbsnetcommon.h>
#include <lbs/lbsprivacycontroller.h>

// LBS test includes.
#include "ctlbsextendedmtlrstep.h"

// Literals used
_LIT(KLbsCancelBeforeReq,"KLbsCancelBeforeReq");

class CT_LbsCancelBeforeReq: public CT_LbsExtendedMTLRStep, public MLbsPrivacyObserver
{
public:
    static CT_LbsCancelBeforeReq* New(CT_LbsExtendedMTLRServer& aParent);
    enum TVerdict doTestStepL();
    ~CT_LbsCancelBeforeReq();

protected:
	// from MLbsPrivacyObserver
	void ProcessNetworkLocationRequest(TUint aRequestId, const TLbsExternalRequestInfo& aRequestInfo, const TNotificationType& aNotificationType);
	void ProcessNetworkPositionUpdate(TUint aRequestId, const TPositionInfo& aPosInfo);
	void ProcessRequestComplete(TUint aRequestId, TInt aReason);
	
protected:
    CT_LbsCancelBeforeReq(CT_LbsExtendedMTLRServer& aParent);
    void ConstructL();
private:
	void InitiateCancelMTLR(TUint aRequestId);
private:
	enum TState
		{
		EInitializing,
		EPrivacyCheckOk,
		ERefLocReceived,
		EGpsLocReceived,
		ERequestComplete
		};
	
	TState iState;
	TLbsNetSessionId iSessionId;
	CLbsPrivacyController* iController;
	CNetProtocolProxy* iProxy;
};

#endif //__CT_LBS_CANCEL_BEFORE_REQ_H__
