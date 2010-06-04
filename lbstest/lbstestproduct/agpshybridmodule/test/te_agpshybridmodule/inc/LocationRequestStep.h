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
 @file LocationRequestStep.h
*/
#ifndef LOCATIONREQUESTSTEP_H
#define LOCATIONREQUESTSTEP_H

#include <test/testexecutestepbase.h>

#include "Te_agpshybridmoduleSuiteStepBase.h"
#include "testmanagerbase.h"


//************************************************************************************************************
//   CTestManagerLocReqStep
//************************************************************************************************************
class CTestManagerLocReqStep : public CTestManagerBase
	{
public:
	static CTestManagerLocReqStep* NewL();
	~CTestManagerLocReqStep();

	void RunL();

	// MT_ResponseObserver
	void ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse);
	
protected:
	CTestManagerLocReqStep();
	void ConstructL();
	
private:
	TBool iMultiUpdateOnDone;
	};


//************************************************************************************************************
//   CLocationRequestStep
//************************************************************************************************************
class CLocationRequestStep : public CTe_agpshybridmoduleSuiteStepBase
	{
public:
	CLocationRequestStep();
	~CLocationRequestStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CTestManagerLocReqStep* iTestManager;
	};

_LIT(KLocationRequestStep,"LocationRequestStep");

#endif	//	LOCATIONREQUESTSTEP_H
