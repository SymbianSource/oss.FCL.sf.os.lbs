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
 @file LbsRootApiTest9Step.h
*/
#if (!defined __LBSROOTAPITEST9_STEP_H__)
#define __LBSROOTAPITEST9_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_lbsrootapitestsuiteStepBase.h"

class CLbsRootApiTest9Step : public CTe_LbsRootApiTestSuiteStepBase
	{
public:
	CLbsRootApiTest9Step();
	~CLbsRootApiTest9Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	void CheckProcessesAreUp();
	};

_LIT(KLbsRootApiTest9Step,"LbsRootApiTest9Step");
#endif // __LBSROOTAPITEST9_STEP_H__
