/**
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file ProcessLaunchTest0Step.h
 @internalTechnology
*/
#if (!defined __PROCESSLAUNCHTEST0_STEP_H__)
#define __PROCESSLAUNCHTEST0_STEP_H__
#include <test/TestExecuteStepBase.h>
#include "Te_ProcessLaunchTestSuiteStepBase.h"

class CProcessLaunchTest0Step : public CTe_ProcessLaunchTestSuiteStepBase
	{
public:
	CProcessLaunchTest0Step();
	~CProcessLaunchTest0Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

// Please add/modify your class members here:
private:
	};

_LIT(KProcessLaunchTest0Step,"ProcessLaunchTest0Step");

#endif
