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
// @file CT_LbsClientServer.cpp
// This is the class implementation for the LBS Client Test Server
// 
//

// User includes
#include "ctlbsclientserver.h"

// System includes
#include <e32std.h>
#include <rsshared.h>

// Header files of LBS Client API Test Steps
#include "ctlbsstepconfighybridmodule.h"
#include "ctlbsclientstepnotifyposupdate.h"
#include "ctlbsclientstepcellbasednpud.h"
#include "ctlbsclientsteplastknownpos.h"
#include "ctlbsclientstepmultireq.h"
#include "ctlbsclientsteptracking.h"
#include "ctlbsclientstepupdateoptions.h"

/**
  NewL()
  Constructs a CT_LbsClientServer object.
  Uses two phase construction and leaves nothing on the CleanupStack.
  @internalTechnology
  @param  none
  @return Created object of type CT_LbsClientServer
  @pre    None
  @post   None
*/
CT_LbsClientServer* CT_LbsClientServer::NewL()
    {
    CT_LbsClientServer*server = new(ELeave) CT_LbsClientServer();
    CleanupStack::PushL(server);
    server->ConstructL(KSfLbsClientTestServer);
    CleanupStack::Pop(server);
    return server;
    }

/**
  MainL()
  Description :		This is the main function which installs the 
					active scheduler and creates an object of the Email server
  @internalTechnology
  @param  none
  @return none
  @pre    None
  @post   None
*/
LOCAL_C void MainL()
	{
	CActiveScheduler* sched = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(sched);
	CActiveScheduler::Install(sched);

	// this registers the server with the active scheduler and calls SetActive
	CT_LbsClientServer* server = CT_LbsClientServer::NewL();

	// signal to the client that we are ready by
	// rendevousing process
	RProcess::Rendezvous(KErrNone);

	// run the active scheduler
	sched->Start();

	// clean up
	delete server;
	
	CleanupStack::PopAndDestroy(sched);
	}




/**
  E32Main()
  Description :	It is the entry point 
  @internalTechnology
  @param  none
  @return Returns the error code
  @pre    None
  @post   None
*/
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}

	TRAPD(err, MainL());

	delete cleanup;
	__UHEAP_MARKEND;

	return err;
	}


CT_LbsClientServer::~CT_LbsClientServer()
	{
	// DELETE EVERYTHIGN THAT IS ALLOCATED IN THE ConstructL(!)
	delete iSharedData;
	iSharedData = NULL;
	}
	
	
/**
  Function : CT_LbsClientServer
  Description : Constructor
  @internalTechnology
  @param :
  @return : N/A
  @precondition : none
  @postcondition : none
*/
CT_LbsClientServer::CT_LbsClientServer()
	{
	}

/**
  Function : ConstructL
  Description : 
  @internalTechnology
  @param :
  @return : N/A
  @precondition : none
  @postcondition : none
*/
void CT_LbsClientServer::ConstructL(const TDesC& aName)
	{
	CT_LbsServer::ConstructL(aName);
	
	iSharedData = CT_LbsSharedData::NewL();
	}

	
/**
  Function : CreateTestStep
  Description : Creates a test step based on the step name read from the script file
  @internalTechnology
  @param : aStepName   The step name which needs to be created
  @return : Created object of type CTestStep
  @precondition : none
  @postcondition : none
*/
CTestStep* CT_LbsClientServer::CreateTestStep(const TDesC& aStepName)
	{
	// NULL if insufficient memory. This suits the API.

	// Test case test steps, each test step supports one or more test cases.
	if(aStepName == KLbsStep_ConfigHybridModule)	
		{
		return CT_LbsStep_ConfigHybridModule::New(*this);
		}	

	if(aStepName == KLbsClientStep_NotifyPosUpdate)	
		{
		return CT_LbsClientStep_NotifyPosUpdate::New(*this);
		}
		
	if(aStepName == KLbsClientStep_CellBasedNotifyPosUpdate)	
		{
		return CT_LbsClientStep_CellBasedNotifyPosUpdate::New(*this);
		}		

	if(aStepName == KLbsClientStep_LastKnownPos)	
		{
		return CT_LbsClientStep_LastKnownPos::New(*this);
		}
	if(aStepName == KLbsClientStep_MultiReq)	
		{
		return CT_LbsClientStep_MultiReq::New(*this);
		}
	if (aStepName == KLbsClientStep_Tracking)
		{
		return CT_LbsClientStep_Tracking::New(*this);
		}
	if (aStepName == KLbsClientStep_UpdateOptions)
		{
		return CT_LbsClientStep_UpdateOptions::New(*this);
		}
	// Let base class handle any common test steps - will return NULL if test step is not supported.
	return CT_LbsServer::CreateTestStep(aStepName);
	}
