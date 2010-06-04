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
 @file GpsUtcModelParamsStep.h
*/
#if (!defined __GPSUTCMODELPARAMS_STEP_H__)
#define __GPSUTCMODELPARAMS_STEP_H__
#include <test/testexecutestepbase.h>
#include "Te_LbsAssistanceDataSuiteStepBase.h"

class CGpsUtcModelParamsStep : public CTe_LbsAssistanceDataSuiteStepBase
	{
public:
	CGpsUtcModelParamsStep(CTe_LbsAssistanceDataSuite& aOwningServer);
	~CGpsUtcModelParamsStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

// Please add/modify your class members here:
private:
	};

_LIT(KGpsUtcModelParamsStep,"GpsUtcModelParamsStep");

#endif
