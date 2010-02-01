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
 @file AssistanceDataStep.h
*/
#ifndef ASSISTANCEDATASTEP_H
#define ASSISTANCEDATASTEP_H

#include <test/testexecutestepbase.h>

#include "Te_agpshybridmoduleSuiteStepBase.h"
#include "testmanagerbase.h"


//************************************************************************************************************
//   CTestManagerAssistDataStep
//************************************************************************************************************
class CTestManagerAssistDataStep : public CTestManagerBase
	{
public:
	static CTestManagerAssistDataStep* NewL();
	~CTestManagerAssistDataStep();

	void RunL();

	// MT_ResponseObserver
	void ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse);
	
protected:
	CTestManagerAssistDataStep();
	void ConstructL();

private:
	TBool iAssistDataOnDone;
	
	// assistance data reader/builder... set to ref time, so we can verify, copy data provider...
	};


//************************************************************************************************************
//   CAssistanceDataStep
//************************************************************************************************************
class CAssistanceDataStep : public CTe_agpshybridmoduleSuiteStepBase
	{
public:
	CAssistanceDataStep();
	~CAssistanceDataStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CTestManagerAssistDataStep* iTestManager;
	};

_LIT(KAssistanceDataStep,"AssistanceDataStep");

#endif	// ASSISTANCEDATASTEP_H
