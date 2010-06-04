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
 @file te_suplrrlpasnsuitestepbase.h
 @internalTechnology
*/

#if (!defined __TE_SUPLRRLPASN_STEP_BASE__)
#define __TE_SUPLRRLPASN_STEP_BASE__
#include <test/testexecutestepbase.h>

#include "suplspoofserver.h"
#include "suplmessagecommon.h"
#include "rrlpmessagebase.h"

#define KMaxMessageLength 1024

/****************************************************************************
* The reason to have a new step base is that it is very much possible
* that the all individual test steps have project related common variables 
* and members 
* and this is the place to define these common variable and members.
* 
****************************************************************************/
class CTe_SuplRrlpAsnSuiteStepBase : public CTestStep
	{
public:
	virtual ~CTe_SuplRrlpAsnSuiteStepBase();
	CTe_SuplRrlpAsnSuiteStepBase();
	virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();

protected:
	void LeaveIfErrorL(TInt aErr);
	TInt CheckVersionContentL(CSuplVersion& aVersion);
	TInt CheckSessionIdContentL(CSuplSessionId& aSessionId);
	TInt CheckRrlpReferenceContentL(TRrlpReference& aRrlpRef);
	void PopulateSessionId(CSuplSessionId* sessionId, TBool aIsSuplStart = EFalse);

protected:
	// common test step variables here
	CSuplSpoofServer* iSpoofServer;

	// encode and receive buffers buffers
	TBufC8<KMaxMessageLength> encodeBuffer;
	TBufC8<KMaxMessageLength> receiveBuffer;
	};

#endif
