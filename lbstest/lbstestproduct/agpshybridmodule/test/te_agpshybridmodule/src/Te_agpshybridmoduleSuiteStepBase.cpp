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
// This is the class implementation for the agpshybridmodule suite Test Server
// 
//

#include "Te_agpshybridmoduleSuiteStepBase.h"

TVerdict CTe_agpshybridmoduleSuiteStepBase::doTestStepPreambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all initialisation common to derived classes in here.
 * Make it being able to leave if there are any errors here as there's no point in
 * trying to run a test step if anything fails.
 * The leave will be picked up by the framework.
 */
	{
	// process some common pre setting to test steps then set SetTestStepResult to EFail or Epass.
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTe_agpshybridmoduleSuiteStepBase::doTestStepPostambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all after test treatment common to derived classes in here.
 * Make it being able to leave
 * The leave will be picked up by the framework.
 */
	{
	// process some common post setting to test steps then set SetTestStepResult to EFail or Epass.
	//SetTestStepResult(EPass);  // or EFail
	return TestStepResult();
	}

CTe_agpshybridmoduleSuiteStepBase::~CTe_agpshybridmoduleSuiteStepBase()
	{
	delete iRootScheduler;
	}

CTe_agpshybridmoduleSuiteStepBase::CTe_agpshybridmoduleSuiteStepBase()
	{
	iRootScheduler = new CActiveScheduler;
	CActiveScheduler::Install(iRootScheduler);
	}
