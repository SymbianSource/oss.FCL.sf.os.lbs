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
// This contains the header file for MOLR UE Assisted No GPS Update during time t
// 
//

#ifndef CTLBSHYBRIDUEASSISTEDMOLRNOGPSUPDATE_H
#define CTLBSHYBRIDUEASSISTEDMOLRNOGPSUPDATE_H

// LBS test includes.
#include "ctlbshybridmolrstep.h"
#include <lbs/test/posserverwatch.h>

// Literals used
_LIT(KLbsHybridUEAssistedMOLRNoGPSUpdate,"LbsHybridUEAssistedMOLRNoGPSUpdate");


class CT_LbsHybridUEAssistedMOLRNoGPSUpdate: public CT_LbsHybridMOLRStep, public MPosServerObserver
{
public:
    static CT_LbsHybridUEAssistedMOLRNoGPSUpdate* New(CT_LbsHybridMOLRServer& aParent);
    enum TVerdict doTestStepL();
    ~CT_LbsHybridUEAssistedMOLRNoGPSUpdate();

protected:
	// MPosServerObserver
	void OnGetLastKnownPosition(TInt32 aErr, const TPositionInfoBase& aPosInfo);
	void OnNotifyPositionUpdate(TInt32 aErr, const TPositionInfoBase& aPosInfo);
	
	
protected:
    CT_LbsHybridUEAssistedMOLRNoGPSUpdate(CT_LbsHybridMOLRServer& aParent);
    void ConstructL();

private:
	enum TState
		{
		EInitializing,
		ERefLocReceived
		};
	
	TState iState;
	TLbsNetSessionId iSessionId;
};

#endif //	CTLBSHYBRIDUEASSISTEDMOLRNOGPSUPDATE_H
