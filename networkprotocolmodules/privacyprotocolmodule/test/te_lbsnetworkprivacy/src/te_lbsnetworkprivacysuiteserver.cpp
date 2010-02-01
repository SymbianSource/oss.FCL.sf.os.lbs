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

/**
 @file te_lbsnetworkprivacysuiteserver.cpp
*/

#include "te_lbsnetworkprivacysuiteserver.h"
#include "notifylocationrequeststep.h"
#include "verifylocationrequeststep.h"
#include "cancelverifylocationrequeststep.h"
#include "verificationtimeoutstep.h"
#include "multipleverifylocationrequeststep.h"
#include "suplnotifylocationrequeststep.h"


_LIT(KServerName,"te_networkprivacysuite");
 
 
 CTe_LbsNetworkPrivacySuite* CTe_LbsNetworkPrivacySuite::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTe_LbsNetworkPrivacySuite* server = new (ELeave) CTe_LbsNetworkPrivacySuite();
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
	CTe_LbsNetworkPrivacySuite* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTe_LbsNetworkPrivacySuite::NewL());
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


CTestStep* CTe_LbsNetworkPrivacySuite::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Secure and non-secure variants
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;
                            
              if(aStepName == KNotifyLocationRequestStep)
                testStep = new CNotifyLocationRequestStep();
              else if(aStepName == KVerifyLocationRequestStep)
                testStep = new CVerifyLocationRequestStep();
              else if(aStepName == KCancelVerifyLocationRequestStep)
                testStep = new CCancelVerifyLocationRequestStep();
	          else if(aStepName == KVerificationTimeoutStep)
                testStep = new CNotifyVerificationTimeoutStep();  
	          else if(aStepName == KMultipleVerifyLocationRequestStep)
                testStep = new CMultipleVerifyLocationRequestStep();  
	          else if(aStepName == KSuplNotifyLocationRequestStep)
                testStep = new CSuplNotifyLocationRequestStep();  
	 
	
	return testStep;
	}
