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
//
	
#include "e32base.h"
#include "csecureasbase.h"
#include "tprocessstartparams.h"
#include "claunchedprocess.h"
#include "lbsdevloggermacros.h"

// E32Main for AGPS manager

TInt E32Main()
	{
	LBSLOG_BEGIN();
	LBSLOG(ELogP3, "AGPS Manager Start()\n");
	TInt r = KErrNone;
	// do we need to process any startline params?
	if(User::CommandLineLength()>0)
		{
		// yes, so assume these are TProcessStartParams that we understand
		TProcessStartParams params;
		TPtr ptr(reinterpret_cast<TText*>(&params), 0, sizeof(TProcessStartParams)/sizeof(TText16));
		User::CommandLine(ptr);
		r = CLaunchedProcess::CompleteProcessLaunch(params);
		}
	else
		{
		// no - so somebody and this should ONLY be the test team!! - has passed
		// us a null command line, presumably by directly calling RProcess::Create()
		// NB in this case you have no way of determining directly that the process
		// creation has failed, or if the process is ready to start processing events!
		r = CLaunchedProcess::CompleteProcessLaunch();
		}
	LBSLOG(ELogP3, "AGPS Manager Stop()\n");
	return r;
	}
