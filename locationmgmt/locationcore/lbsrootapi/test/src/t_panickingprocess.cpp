/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
@file
@internalTechnology
@released
*/
#include <e32base.h>
#include <e32test.h>

#include "lbsrootapi.h"
#include "t_panickingprocess.h"





GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);

    User::SetJustInTime(EFalse);

	CTrapCleanup* cleanup = CTrapCleanup::New();
	if (!cleanup)
		{
		return KErrNoMemory;
		}
		
    CTLbsPanickingProcess* panickingProcess = 0;
    TRAPD(err, panickingProcess = CTLbsPanickingProcess::NewL());

	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		scheduler->Start();
		}

    User::SetJustInTime(ETrue);
    
	if (KErrNone == err)
		{
		delete panickingProcess;
		}
	
	delete cleanup;
	delete scheduler;
	
	__UHEAP_MARKEND;
    
	return err;
    }


CTLbsPanickingProcess::CTLbsPanickingProcess() 
: CActive(EPriorityStandard)
    {
	
    }
CTLbsPanickingProcess::~CTLbsPanickingProcess()
    {
    Cancel();
    }
    
CTLbsPanickingProcess* CTLbsPanickingProcess::NewL()
    {
    CTLbsPanickingProcess* self = new(ELeave) CTLbsPanickingProcess();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return(self);
    }
    
void CTLbsPanickingProcess::ConstructL()
    {
    CActiveScheduler::Add(this);
        
	// Does nothing but panic
	User::Invariant();
    }

    
void CTLbsPanickingProcess::RunL()
    {
    }
    
void CTLbsPanickingProcess::DoCancel()
    {
    }
