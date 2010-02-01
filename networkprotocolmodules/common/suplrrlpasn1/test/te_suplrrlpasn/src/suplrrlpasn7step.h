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
 @file suplrrlpasn7step.h
 @internalTechnology
*/
#if (!defined __SUPLRRLPASN7_STEP_H__)
#define __SUPLRRLPASN7_STEP_H__
#include <test/testexecutestepbase.h>
#include "te_suplrrlpasnsuitestepbase.h"
#include "rrlpprotocolerror.h"


class CSuplRrlpAsn7Step : public CTe_SuplRrlpAsnSuiteStepBase
	{
public:
	CSuplRrlpAsn7Step();
	~CSuplRrlpAsn7Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
private:
	void TestReceiveSuplPosRrlpProtocolErrorL();
	TInt CheckRrlpErrorL(TRrlpErrorCode& aErrorCause);
	};

_LIT(KSuplRrlpAsn7Step,"suplrrlpasn7step");

#endif
