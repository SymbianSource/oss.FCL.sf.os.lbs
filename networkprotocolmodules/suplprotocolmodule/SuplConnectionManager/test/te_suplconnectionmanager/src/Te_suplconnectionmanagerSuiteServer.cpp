// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Example file/test code to demonstrate how to develop a TestExecute Server
// Developers should take this project as a template and substitute their own
// for (WINS && !EKA2) versions will be xxxServer.Dll and require a thread to be started
// in the process of the client. The client initialises the server by calling the
// one and only ordinal.
// 
//

/**
 @file Te_suplconnectionmanagerSuiteServer.cpp
 @internalTechnology
*/

#include "Te_suplconnectionmanagerSuiteServer.h"
#include "ctlbsstepinstallscheduler.h"
#include "suplconnectionmanager0Step.h"
#include "suplconnectionmanager1Step.h"
#include "suplconnectionmanager2Step.h"
#include "suplconnectionmanager3Step.h"

_LIT(KServerName,"Te_suplconnectionmanagerSuite");
CTe_suplconnectionmanagerSuite* CTe_suplconnectionmanagerSuite::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTe_suplconnectionmanagerSuite * server = new (ELeave) CTe_suplconnectionmanagerSuite();
	CleanupStack::PushL(server);

	server->ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}


// Secure variants much simpler
// For EKA2, just an E32Main and a MainL()
LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().DataCaging(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTe_suplconnectionmanagerSuite* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTe_suplconnectionmanagerSuite::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}



GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	delete cleanup;
	__UHEAP_MARKEND;
	return err;
    }

void CTe_suplconnectionmanagerSuite::ConstructL(const TDesC& aName)
	{
	CTestServer::ConstructL(aName);

	iScheduler = new (ELeave) CActiveScheduler();
	}

CTe_suplconnectionmanagerSuite::~CTe_suplconnectionmanagerSuite()
	{
	delete iScheduler;
	}


CTestStep* CTe_suplconnectionmanagerSuite::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Secure and non-secure variants
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;
    if(aStepName == Ksuplconnectionmanager0Step)
    	testStep = new Csuplconnectionmanager0Step();
    else if(aStepName == Ksuplconnectionmanager1Step)
    	testStep = new Csuplconnectionmanager1Step();
    else if(aStepName == Ksuplconnectionmanager2Step)
    	testStep = new Csuplconnectionmanager2Step();
    else if(aStepName == Ksuplconnectionmanager3Step)
    	testStep = new Csuplconnectionmanager3Step();
    else if (aStepName == KLbsStep_InstallScheduler)
    	testStep = new CT_LbsStep_InstallScheduler(*iScheduler);
    
	return testStep;
	}
