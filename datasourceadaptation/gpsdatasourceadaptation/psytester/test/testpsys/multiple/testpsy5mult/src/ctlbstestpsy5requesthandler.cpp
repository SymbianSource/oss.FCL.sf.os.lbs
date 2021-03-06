// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#include <lbspositioninfo.h>
#include <lbssatellite.h>
#include "ctlbstestPsy5RequestHandler.h"
#include "ctlbs_testPsy5Constants.h"

// ---------------------------------------------------------
// CT_LbsTestPsy4RequestHandler::InstanceL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//  
CT_LbsTestPsy5RequestHandler* CT_LbsTestPsy5RequestHandler::InstanceL()
	{   
    CT_LbsTestPsy5RequestHandler* self;
	if (Dll::Tls())
		{
		self = reinterpret_cast<CT_LbsTestPsy5RequestHandler*> (Dll::Tls());
		if(self->iReferenceCounter >= 10)
			{
            self->iReferenceCounter++;
			User::Leave(KErrGeneral);
			}
		}
    else
        {
		self = new (ELeave) CT_LbsTestPsy5RequestHandler;
        CleanupStack::PushL(self);
        self->ConstructL();
        CleanupStack::Pop(); // self
		Dll::SetTls(reinterpret_cast<TAny*>(self));
        }

	self->iReferenceCounter++;
	return self;
    }

// ---------------------------------------------------------
// ::Release
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CT_LbsTestPsy5RequestHandler::Release()
	{
	if (--iReferenceCounter == 0)
		{
        Dll::SetTls(NULL);
		delete this;
		}
	}

// C++ default constructor can NOT contain any code, that
// might leave.
//
CT_LbsTestPsy5RequestHandler::CT_LbsTestPsy5RequestHandler() :
	CT_LbsTestPsyBase(NULL) 
    {
	
	}

// Destructor
CT_LbsTestPsy5RequestHandler::~CT_LbsTestPsy5RequestHandler()
    {

	}



