// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file te_suplrrlpasnsuiteserver.cpp
 @internalTechnology
*/

#include "te_suplrrlpasnsuiteserver.h"
#include "suplrrlpasn0step.h"
#include "suplrrlpasn1step.h"
#include "suplrrlpasn2step.h"
#include "suplrrlpasn3step.h"
#include "suplrrlpasn4step.h"
#include "suplrrlpasn5step.h"
#include "suplrrlpasn6step.h"
#include "suplrrlpasn7step.h"
#include "suplrrlpasn8step.h"
#include "suplrrlpasn9step.h"
#include "suplrrlpasn10step.h"
#include "suplrrlpasn11step.h"
#include "suplrrlpasn12step.h"


_LIT(KServerName, "te_suplrrlpasnsuite");

CTe_SuplRrlpAsnSuite* CTe_SuplRrlpAsnSuite::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTe_SuplRrlpAsnSuite * server = new (ELeave) CTe_SuplRrlpAsnSuite();
	CleanupStack::PushL(server);
	server->ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

CTe_SuplRrlpAsnSuite::CTe_SuplRrlpAsnSuite()
	{
	}

CTe_SuplRrlpAsnSuite::~CTe_SuplRrlpAsnSuite()
	{
	}

void CTe_SuplRrlpAsnSuite::ConstructL(const TDesC& aName)
	{
	CTestServer::ConstructL(aName);
	
	}

CTestStep* CTe_SuplRrlpAsnSuite::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Secure and non-secure variants
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;
	if(aStepName == KSuplRrlpAsn0Step)
		{
		testStep = new CSuplRrlpAsn0Step();
		}

	if(aStepName == KSuplRrlpAsn1Step)
		{
		testStep = new CSuplRrlpAsn1Step();
		}
	if(aStepName == KSuplRrlpAsn2Step)
		{
		testStep = new CSuplRrlpAsn2Step();
		}
	if(aStepName == KSuplRrlpAsn3Step)
		{
		testStep = new CSuplRrlpAsn3Step();
		}
	if(aStepName == KSuplRrlpAsn4Step)
		{
		testStep = new CSuplRrlpAsn4Step();
		}
	if(aStepName == KSuplRrlpAsn5Step)
		{
		testStep = new CSuplRrlpAsn5Step();
		}
	if(aStepName == KSuplRrlpAsn6Step)
		{
		testStep = new CSuplRrlpAsn6Step();
		}
	if(aStepName == KSuplRrlpAsn7Step)
		{
		testStep = new CSuplRrlpAsn7Step();
		}
	if(aStepName == KSuplRrlpAsn8Step)
		{
		testStep = new CSuplRrlpAsn8Step();
		}
	if(aStepName == KSuplRrlpAsn9Step)
		{
		testStep = new CSuplRrlpAsn9Step();
		}
	if(aStepName == KSuplRrlpAsn10Step)
		{
		testStep = new CSuplRrlpAsn10Step();
		}
	if(aStepName == KSuplRrlpAsn11Step)
		{
		testStep = new CSuplRrlpAsn11Step();
		}
	if(aStepName == KSuplRrlpAsn12Step)
		{
		testStep = new CSuplRrlpAsn12Step();
		}

	return testStep;
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
	CTe_SuplRrlpAsnSuite* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTe_SuplRrlpAsnSuite::NewL());
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
