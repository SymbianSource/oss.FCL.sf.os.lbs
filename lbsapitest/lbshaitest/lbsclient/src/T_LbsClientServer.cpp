/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#include "t_lbsclientserver.h"
#include "t_rpositionerdata.h"
#include "t_rpositionserverdata.h"

_LIT(KT_RPositioner, "RPositioner");
_LIT(KT_RPositionServer, "RPositionServer");

/**
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 * @return - Instance of the test server
 */
CT_LbsClientServer* CT_LbsClientServer::NewL()
	{
	CT_LbsClientServer* server = new (ELeave) CT_LbsClientServer();
	CleanupStack::PushL(server);
	server->ConstructL();
	CleanupStack::Pop(server);
	return server;
	}

/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
LOCAL_C void MainL()
	{
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched = NULL;
	sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CT_LbsClientServer* server = NULL;

	// Create the CTestServer derived server
	TRAPD(err, server = CT_LbsClientServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}

	delete server;
	delete sched;
	}

/**
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 * @return - Standard Epoc error code on process exit
 */
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}

#if (defined TRAP_IGNORE)
	TRAP_IGNORE(MainL());
#else
	TRAPD(err,MainL());
#endif

	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

CDataWrapper* CT_LbsClientServer::CT_LbsBlock::CreateDataL(const TDesC& aData)
	{
	CDataWrapper* wrapper = NULL;

	if( KT_RPositioner() == aData )
		{
		wrapper = CT_RPositionerData::NewL();
		}
	else if (KT_RPositionServer() == aData)
		{
		wrapper = CT_RPositionServerData::NewL();
		}

	return wrapper;
	}
