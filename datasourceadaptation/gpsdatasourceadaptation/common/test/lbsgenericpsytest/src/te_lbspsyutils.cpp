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
// Class used by psy test suites to do common tasks such as restarting LBS.
// 
//

#include <centralrepository.h>

#include "te_lbspsyutils.h"
#include "lbssystemcontroller.h"
#include "lbsrootcenrepdefs.h"

EXPORT_C CTe_SystemStarter::CTe_SystemStarter(TUid aServerId)
/** Constructor.
*/
	{
	iServerId = aServerId;
	}

EXPORT_C CTe_SystemStarter::~CTe_SystemStarter()
/** Destructor.
*/
	{
	}

EXPORT_C void CTe_SystemStarter::RestartLbs_RootOnlyL(TBuf8<KMaxFileName>& aFirstExe)
	{
	//Shutdown the whole LBS and restart with only the root process.
	RLbsSystemController lbsSysController;
	lbsSysController.OpenL(iServerId);
	CleanupClosePushL(lbsSysController);	
	User::LeaveIfError(lbsSysController.RequestCompleteCloseDown());
	
	CRepository* rep = CRepository::NewL(TUid::Uid(KLbsCenRepUidValue));
	CleanupStack::PushL(rep);
	
	TInt err = rep->Get(KLbsRootProcessNameFirstKey, aFirstExe);
	if(err==KErrNone)
		{
		User::LeaveIfError(rep->Delete(KLbsRootProcessNameFirstKey));
		}
	else if(err==KErrNotFound)
		{
		aFirstExe = KNullDesC8;
		}
	else
		{
		User::Leave(err);
		}
	CleanupStack::PopAndDestroy(rep);
	
	User::LeaveIfError(lbsSysController.RequestSystemStartup());
	
	CleanupStack::PopAndDestroy(&lbsSysController);  
	}

EXPORT_C void CTe_SystemStarter::RestartLbs_NormalL(TBuf8<KMaxFileName>aFirstExe)
	{
	//We need to shutdown the whole LBS (including the root process) so any subsequent tests are able to start 
	//the root process with new settings if necessary.
	//This is done for the device build.
	RLbsSystemController lbsSysController;
	lbsSysController.OpenL(iServerId);
	CleanupClosePushL(lbsSysController);	
	User::LeaveIfError(lbsSysController.RequestCompleteCloseDown());
	CleanupStack::PopAndDestroy(&lbsSysController);
			
	if(aFirstExe.Length())
		{
		CRepository* rep = CRepository::NewL(TUid::Uid(KLbsCenRepUidValue));
		rep->Set(KLbsRootProcessNameFirstKey, aFirstExe);
		delete rep;
		}
	}
