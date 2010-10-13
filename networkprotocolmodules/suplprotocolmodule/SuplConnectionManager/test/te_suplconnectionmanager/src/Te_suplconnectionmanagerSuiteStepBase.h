/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file Te_suplconnectionmanagerSuiteStepBase.h
 @internalTechnology
*/

#if (!defined __TE_SUPLCONNECTIONMANAGER_STEP_BASE__)
#define __TE_SUPLCONNECTIONMANAGER_STEP_BASE__
#include <test/testexecutestepbase.h>
// Please add your include here if you have 
#include "sockethandler.h"
#include "suplconnectionmanager.h"
#include "suplstart.h"
#include "suplresponse.h"
#include "suplposinit.h"
#include "suplpos.h"
#include "rrlpassistancedataack.h"



/****************************************************************************
* The reason to have a new step base is that it is very much possible
* that the all individual test steps have project related common variables 
* and members 
* and this is the place to define these common variable and members.
* 
****************************************************************************/
class CTe_suplconnectionmanagerSuiteStepBase : public CTestStep, 
											   public MSocketHandler,
											   public MSuplConnectionManagerObserver,
											   public CActive
	{
public:
	virtual ~CTe_suplconnectionmanagerSuiteStepBase();
	CTe_suplconnectionmanagerSuiteStepBase();
	virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();

	// CActive
	void RunL();
	TInt RunError(TInt aError);
	void DoCancel();
	
	// MSuplConnectionManagerObserver
	void Connected(const TInetAddr& aAddr) = 0;
	void ConnectionError(const TSuplConnectionError& aError) = 0;
	void MessageReceived(CSuplMessageBase* aSuplMessage) = 0;
	
	// MSocketHandler
	void Connected(TInt aWhat);
	virtual void MessageRecieved(TDesC8& aBuffer) = 0;

	// Other
	void Start();
	void Stop();
	void SendSuplStart();
	void SendSuplEnd();
	void SendSuplResponse();
	void SendSuplPosInit();
	void SendSuplPosAssData();
	void SendSuplPosAck();
	void SendSupPosLocReq();
	void SendSupPosLocResp();
private:
	void BuildSuplStartL(CSuplStart*& aMessage);
	void PopulateSessionId(CSuplSessionId* aSessionId, TBool aIsSuplStart);
	void BuildSuplPosInitL(CSuplPosInit*& aMessage);
	void BuildSuplPosAckL(CSuplPos*& aMessage);
	void BuildSuplPosRespL(CSuplPos*& aMessage);

private:
	CSocketHandler* iSocketHandler;
	RBuf iTxBuffer;
	
	enum TState
		{
		EIdle,
		EConnect,
		ESendSuplStart,
		ESendSuplEnd,
		ESendSuplResponse,
		ESendSuplPosInit,
		ESendSuplPosAssData,
		ESendSuplPosAck,
		ESendSuplLocReq,
		ESendSuplLocResp,
		};

	TState iState;
	CSuplConnectionManager* iConnectionManager;
	};

#endif
