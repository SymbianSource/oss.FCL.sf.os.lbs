/**
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file suplconnectionmanager2Step.h
 @internalTechnology
*/
#if (!defined __SUPLCONNECTIONMANAGER2_STEP_H__)
#define __SUPLCONNECTIONMANAGER2_STEP_H__
#include <test/testexecutestepbase.h>
#include "Te_suplconnectionmanagerSuiteStepBase.h"

class Csuplconnectionmanager2Step : public CTe_suplconnectionmanagerSuiteStepBase
	{
public:
	Csuplconnectionmanager2Step();
	~Csuplconnectionmanager2Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

// Please add/modify your class members here:
	void MessageRecieved(TDesC8& aBuffer);
	
	void Connected(const TInetAddr& aAddr);
	void ConnectionError(const TSuplConnectionError& aError);
	void MessageReceived(CSuplMessageBase* aSuplMessage);
private:
	enum TState
		{
		ESuplStart,
		ESuplResponse,
		ESuplPosInit,
		ESuplPosAssData,
		ESuplPosAck,
		ESuplPosRequest,
		ESuplPosLocationResponse,
		ESuplEnd,
		EFinished
		};

	TState iState;
	};

_LIT(Ksuplconnectionmanager2Step,"suplconnectionmanager2Step");

#endif