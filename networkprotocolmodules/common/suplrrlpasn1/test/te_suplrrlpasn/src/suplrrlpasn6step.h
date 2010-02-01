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
 @file suplrrlpasn6step.h
 @internalTechnology
*/
#if (!defined __SUPLRRLPASN6_STEP_H__)
#define __SUPLRRLPASN6_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_suplrrlpasnsuitestepbase.h"

class CSuplRrlpAsn6Step : public CTe_SuplRrlpAsnSuiteStepBase
	{
public:
	CSuplRrlpAsn6Step();
	~CSuplRrlpAsn6Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
private:
	void TestEncodeSuplPosRrlpMeasurePositionResponseL();
	void TestEncodeSuplPosRrlpMeasurePositionResponse2L();
	};

_LIT(KSuplRrlpAsn6Step,"suplrrlpasn6step");

#endif
