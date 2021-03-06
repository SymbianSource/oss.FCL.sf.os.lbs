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
 @file cpluginstep1.h
*/
#if (!defined __PLUGIN_STEP1_H__)
#define __PLUGIN_STEP1_H__
#include <test/testexecutestepbase.h>
#include "te_testprotocolmodulesuitestepbase.h"

class CPluginStep1 : public CTe_testprotocolmoduleSuiteStepBase
	{
public:
	CPluginStep1();
	~CPluginStep1();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

// Please add/modify your class members here:
private:
	};

_LIT(KPluginStep1,"PluginStep1");

#endif
