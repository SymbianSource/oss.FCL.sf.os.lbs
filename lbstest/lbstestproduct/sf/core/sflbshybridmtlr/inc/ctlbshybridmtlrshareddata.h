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
// @file ctlbshybridmtlrshareddata.h
// 
//


#ifndef __T_LBS_HYBRID_MTLR_SHARED_DATA_H__
#define __T_LBS_HYBRID_MTLR_SHARED_DATA_H__

// system includes

// Lbs includes
#include "ctlbsshareddata.h"
#include "tlbsutils.h"

class CT_LbsHybridMTLRSharedData : public CT_LbsSharedData
{
public:
	static CT_LbsHybridMTLRSharedData* NewL();
	~CT_LbsHybridMTLRSharedData();

protected:
	CT_LbsHybridMTLRSharedData();
	void ConstructL();

private:
//	RPointerArray<TLbsModuleUpdateItem>& iUpdateArr;
};

#endif // __T_LBS_HYBRID_MTLR_SHARED_DATA_H__
