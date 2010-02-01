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
 @file suplrrlpasn12step.h
 @internalTechnology
*/
#if (!defined __SUPLRRLPASN12_STEP_H__)
#define __SUPLRRLPASN12_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_suplrrlpasnsuitestepbase.h"

// LBS
#include <lbs/lbsnetcommon.h>
#include <lbs/lbsloccommon.h>
#include <lbs/lbsassistancedatabase.h>
#include <lbspositioninfo.h>
#include <lbs/lbsgpsmeasurement.h>


class CSuplRrlpAsn12Step : public CTe_SuplRrlpAsnSuiteStepBase
	{
public:
	CSuplRrlpAsn12Step();
	~CSuplRrlpAsn12Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
private:
	void TestReceiveSuplInitL();
	TInt CheckPosMethodContentL(TLbsNetPosRequestMethod& aMethod);
	TInt CheckNotificationContentL(TLbsNetPosRequestPrivacy& aPrivacy);
	TInt CheckExternalRequestInfoContentL(TLbsExternalRequestInfo& aRequestInfo);
	TInt CheckSlpAddressContentL(CSuplSlpAddress& aAddress);
	TInt CheckQopL(TLbsNetPosRequestQuality& aQuality);
	};

_LIT(KSuplRrlpAsn12Step,"suplrrlpasn12step");

#endif
