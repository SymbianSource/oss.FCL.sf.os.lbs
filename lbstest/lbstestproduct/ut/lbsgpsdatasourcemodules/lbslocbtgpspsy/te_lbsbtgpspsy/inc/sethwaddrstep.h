/**
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/



/**
 @file sethwaddrstep.h
 @internalTechnology
*/
#ifndef __SETHWADDR_STEP_H__
#define __SETHWADDR_STEP_H__
#include <test/TestExecuteStepBase.h>
#include "Te_LbsBtGpsPsyStepBase.h"
#include <lbs.h>
#include <lbsCommon.h>



class CSetHwAddrStep : public CTe_LbsBtGpsPsyStepBase
	{
public:
	CSetHwAddrStep();
	~CSetHwAddrStep();
	virtual TVerdict doTestStepL();

// Please add/modify your class members here:
private:


	};

_LIT(KSetHwAddrStep,"SetHwAddrStep");

#endif
