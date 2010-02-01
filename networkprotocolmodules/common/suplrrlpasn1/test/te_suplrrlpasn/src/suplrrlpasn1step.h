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
 @file suplrrlpasn1step.h
 @internalTechnology
*/
#if (!defined __SUPLRRLPASN1_STEP_H__)
#define __SUPLRRLPASN1_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_suplrrlpasnsuitestepbase.h"

class TLbsNetPosRequestMethod;

class CSuplRrlpAsn1Step : public CTe_SuplRrlpAsnSuiteStepBase
	{
public:
	CSuplRrlpAsn1Step();
	~CSuplRrlpAsn1Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
private:
	void TestReceiveSuplResponseL();
	TInt CheckPosMethodContentL(TLbsNetPosRequestMethod& aMethod);
	TInt CheckSlpAddressContentL(CSuplSlpAddress& aAddress);
	};

_LIT(KSuplRrlpAsn1Step,"suplrrlpasn1step");

#endif
