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
 @file suplconnectionmanager0Step.h
 @internalTechnology
*/
#if (!defined __SUPLCONNECTIONMANAGER0_STEP_H__)
#define __SUPLCONNECTIONMANAGER0_STEP_H__
#include <test/testexecutestepbase.h>
#include "Te_suplconnectionmanagerSuiteStepBase.h"


class Csuplconnectionmanager0Step : public CTe_suplconnectionmanagerSuiteStepBase
	{
public:
	Csuplconnectionmanager0Step();
	~Csuplconnectionmanager0Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

// Please add/modify your class members here:
	void MessageRecieved(TDesC8& aBuffer);
	
	void Connected(const TInetAddr& aAddr);
	void ConnectionError(const TSuplConnectionError& aError);
	void MessageReceived(CSuplMessageBase* aSuplMessage);
protected:	
	
private:
	};

_LIT(Ksuplconnectionmanager0Step,"suplconnectionmanager0Step");

#endif
