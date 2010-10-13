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
 @file suplrrlpasn5step.h
 @internalTechnology
*/
#if (!defined __SUPLRRLPASN5_STEP_H__)
#define __SUPLRRLPASN5_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_suplrrlpasnsuitestepbase.h"
#include <lbspositioninfo.h>
#include <lbs/lbsnetcommon.h>
#include <lbs/lbsassistancedatabase.h>
#include <lbs/lbsgpsmeasurement.h>

class CSuplRrlpAsn5Step : public CTe_SuplRrlpAsnSuiteStepBase
	{
public:
	CSuplRrlpAsn5Step();
	~CSuplRrlpAsn5Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
private:
	void TestReceiveSuplPosRrlpMeasurePositionRequestL();
	TInt CheckPositionInstructContentL(TLbsNetPosRequestQuality& aQuality, TLbsNetPosRequestMethod& aMethod);
	};

_LIT(KSuplRrlpAsn5Step,"suplrrlpasn5step");

#endif
