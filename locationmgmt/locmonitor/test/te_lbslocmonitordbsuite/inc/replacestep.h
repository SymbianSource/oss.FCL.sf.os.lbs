/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __REPLACE_STEP_H__
#define __REPLACE_STEP_H__
#include <test/TestExecuteStepBase.h>
#include "te_lbslocationmonitorsuitestepbase.h"

class CReplaceStep : public CTe_LbsLocationMonitorSuiteStepBase
	{
public:
	CReplaceStep();
	~CReplaceStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	};

_LIT(KReplaceStep,"ReplaceStep");

#endif
