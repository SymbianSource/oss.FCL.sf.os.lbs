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
 @file cx3pstep11.h
*/

#if (!defined __X3P_STEP11_H__)
#define __X3P_STEP11_H__

#include <test/testexecutestepbase.h>
#include "te_testprotocolmodulesuitestepbase.h"
#include "cgatewayinterface.h"

class CX3pStep11 : public CTe_testprotocolmoduleSuiteStepBase
	{
public:
	CX3pStep11();
	~CX3pStep11();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CGatewayInterface* iModule;
	
	};

_LIT(KX3pStep11,"X3pStep11");

#endif
