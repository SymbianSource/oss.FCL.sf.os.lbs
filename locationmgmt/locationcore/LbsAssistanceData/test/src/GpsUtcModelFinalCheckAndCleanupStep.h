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
*
*/



/**
 @file GpsUtcModelFinalCheckAndCleanupStep.h
*/
#if (!defined __GPSUTCMODELFINALCHECKANDCLEANUP_STEP_H__)
#define __GPSUTCMODELFINALCHECKANDCLEANUP_STEP_H__
#include <test/testexecutestepbase.h>
#include "Te_LbsAssistanceDataSuiteStepBase.h"

class CGpsUtcModelFinalCheckAndCleanupStep : public CTe_LbsAssistanceDataSuiteStepBase
	{
public:
	CGpsUtcModelFinalCheckAndCleanupStep(CTe_LbsAssistanceDataSuite& aOwningServer);
	~CGpsUtcModelFinalCheckAndCleanupStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

	};

_LIT(KGpsUtcModelFinalCheckAndCleanupStep,"GpsUtcModelFinalCheckAndCleanupStep");

#endif
