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
 @file suplrrlpasn3step.h
 @internalTechnology
*/
#if (!defined __SUPLRRLPASN3_STEP_H__)
#define __SUPLRRLPASN3_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_suplrrlpasnsuitestepbase.h"
#include <lbs/lbsassistancedatabase.h>


class CSuplRrlpAsn3Step : public CTe_SuplRrlpAsnSuiteStepBase
	{
public:
	CSuplRrlpAsn3Step();
	~CSuplRrlpAsn3Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
private:
	void TestReceiveSuplPosRrlpAssistanceDataL();
	
	TInt CheckAssistanceContentL(TLbsAsistanceDataGroup& aDataMask, RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckReferenceTimeContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckReferenceLocationContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckNavigationModelContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckIonosphericModelContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckUtcModelContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckAlmanacContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckAcquisitionAssistanceContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);
	TInt CheckBadSatListContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet);

	};

_LIT(KSuplRrlpAsn3Step,"suplrrlpasn3step");

#endif
