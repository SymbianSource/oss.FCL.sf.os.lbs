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
 @file suplrrlp0Step.h
 @internalTechnology
*/
#if (!defined __SUPLRRLP0_STEP_H__)
#define __SUPLRRLP0_STEP_H__
#include <test/testexecutestepbase.h>
#include "Te_suplrrlpSuiteStepBase.h"

class Csuplrrlp0Step : public CTe_suplrrlpSuiteStepBase
	{
public:
	Csuplrrlp0Step();
	~Csuplrrlp0Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
private:
	CActiveScheduler*	iSched;
	CRrlpTest*			iRrlpTest;
	};

_LIT(Ksuplrrlp0Step,"suplrrlp0Step");

#endif
