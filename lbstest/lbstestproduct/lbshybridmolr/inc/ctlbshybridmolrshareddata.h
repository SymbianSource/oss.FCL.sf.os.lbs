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
// @file ctlbshybridMOLRshareddata.h
// 
//


#ifndef __T_LBS_HYBRID_MOLR_SHARED_DATA_H__
#define __T_LBS_HYBRID_MOLR_SHARED_DATA_H__

// system includes

// Lbs includes
#include "ctlbsshareddata.h"
#include <lbs/test/tlbsutils.h>

class CT_LbsHybridMOLRSharedData : public CT_LbsSharedData
{
public:
	static CT_LbsHybridMOLRSharedData* NewL();
	~CT_LbsHybridMOLRSharedData();

protected:
	CT_LbsHybridMOLRSharedData();
	void ConstructL();

private:
//	RPointerArray<TLbsModuleUpdateItem>& iUpdateArr;
};

#endif // __T_LBS_HYBRID_MOLR_SHARED_DATA_H__
