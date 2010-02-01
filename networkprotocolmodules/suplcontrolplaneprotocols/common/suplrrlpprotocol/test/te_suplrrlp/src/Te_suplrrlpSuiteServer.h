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
 @file Te_suplrrlpSuiteServer.h
 @internalTechnology
*/

#if (!defined __TE_SUPLRRLP_SERVER_H__)
#define __TE_SUPLRRLP_SERVER_H__
#include <test/testexecuteserverbase.h>

#include "rrlptest.h"


class CTe_suplrrlpSuite : public CTestServer
	{
public:
	static CTe_suplrrlpSuite* NewL();
	~CTe_suplrrlpSuite();
	
	// Base class pure virtual override
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);

private:	
	CTe_suplrrlpSuite();
	void ConstructL(const TDesC& aName);
	};

#endif
