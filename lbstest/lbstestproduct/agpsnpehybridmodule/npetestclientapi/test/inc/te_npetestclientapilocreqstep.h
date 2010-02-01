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


#ifndef TE_NPETESTCLIENTAPILOCREQSTEP_H
#define TE_NPETESTCLIENTAPILOCREQSTEP_H

#include <test/testexecutestepbase.h>

#include "te_npetestclientapistepbase.h"
#include "te_testnpeintegmodule.h"


//************************************************************************************************************
//   CTe_LocReqTestNpiModule
//************************************************************************************************************

class CTe_LocReqTestNpiModule : public CTe_TestNpeIntegModule 
	{
public:
	static CTe_LocReqTestNpiModule* NewL();
	~CTe_LocReqTestNpiModule();

	void RunL();

	// MT_ResponseObserver
	void ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse);
	
protected:
    CTe_LocReqTestNpiModule();
	void ConstructL();
	
private:
	TBool iMultiUpdateOnDone;
    TBool iAssistDataOnDone;
	};


//************************************************************************************************************
//   CTe_NpeTestClientApiLocReqStep
//************************************************************************************************************
class CTe_NpeTestClientApiLocReqStep : public CTe_NpeTestClientApiStepBase
	{
public:
    CTe_NpeTestClientApiLocReqStep();
	~CTe_NpeTestClientApiLocReqStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
    CTe_LocReqTestNpiModule* iTestManager;
	};

_LIT(KNpeTestClientApiLocReqStep,"NpeTestClientApiLocReqStep");

#endif	//	TE_NPETESTCLIENTAPILOCREQSTEP_H
