// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file ctlbsstep_installscheduler.cpp
// This is the class implementation for the Install Active Scheduler Test Step
// 
//

/**
 @file
 @internalTechnology
 @test
*/

#include "ctlbsstepinstallscheduler.h"

CT_LbsStep_InstallScheduler::CT_LbsStep_InstallScheduler(CActiveScheduler& aScheduler) :
	iScheduler(aScheduler)
	{
	SetTestStepName(KLbsStep_InstallScheduler);
	}

/**
Static Constructor
*/
CT_LbsStep_InstallScheduler* CT_LbsStep_InstallScheduler::New(CActiveScheduler& aScheduler)
	{
	return new CT_LbsStep_InstallScheduler(aScheduler);
	// Note the lack of ELeave.
	// This means that having insufficient memory will return NULL;
	}

/**
@pre 	No Active Scheduler installed in the worker test thread
@return Test verdict
@post	Test server's Active Scheduler member installed in worker thread
*/
TVerdict CT_LbsStep_InstallScheduler::doTestStepL()
	{
	if( CActiveScheduler::Current() )
		{
		return TestStepResult();
		}
	CActiveScheduler::Install(&iScheduler);

	return TestStepResult();
	}

