/**
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file cancelverifylocationrequeststep.h
*/
#if (!defined __CANCELVERIFYLOCATIONREQUEST_STEP_H__)
#define __CANCELVERIFYLOCATIONREQUEST_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_lbsnetworkprivacysuitestepbase.h"
#include <lbs/epos_mposverificationobserver.h>

_LIT(KCancelVerifyLocationRequestStep,"CancelVerifyLocationRequestStep");

class CCancelVerifyLocationRequestStep : public CTe_LbsNetworkPrivacySuiteStepBase,
                                         public MPosVerificationObserver
	{
public:
	CCancelVerifyLocationRequestStep();
	~CCancelVerifyLocationRequestStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
   	// from MLbsNetChannelObserver
	void ProcessNetChannelMessage(RLbsNetChannel::TLbsNetChannelId aChannelId, const TLbsNetInternalMsgBase& aMessage);
	
	// from MPosVerificationObserver
	void HandleVerifyComplete(TInt aRequestId,TInt aCompletionCode);
  	};
#endif //cancelverifylocationrequeststep.h
