// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef TE_AGPSHYBRIDMODULESERVER_H
#define TE_AGPSHYBRIDMODULESERVER_H

#include <test/testexecuteserverbase.h>

class CTe_agpshybridmoduleSuite : public CTestServer
	{
public:
	static CTe_agpshybridmoduleSuite* NewL();
	// Base class pure virtual override
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};

#endif // TE_AGPSHYBRIDMODULESERVER_H
