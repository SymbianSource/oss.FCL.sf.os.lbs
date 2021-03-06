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
//

/**
 @file te_agpsmanagerstep21.cpp
*/
 
#include "te_agpsmanagerstep21.h"
#include "te_agpsmanagersuitedefs.h"
#include <lbs/lbslocdatasourceclasstypes.h>


CTe_AgpsManagerStep21::~CTe_AgpsManagerStep21()
	{
	}

CTe_AgpsManagerStep21::CTe_AgpsManagerStep21()
	{
	SetTestStepName(KAgpsManagerStep21);
	}

TVerdict CTe_AgpsManagerStep21::doTestStepPreambleL()
	{
	CTe_AgpsManagerStepBase::doTestStepPreambleL();
	return TestStepResult();
	}

//
// Boundary value tests for positioning Method in Location Server requests.
// The number of positioning methods requested by the LS cannot be 9 (Manager panics).
//
TVerdict CTe_AgpsManagerStep21::doTestStepL()
	{
 	if (TestStepResult()==EPass)
 		{
		TLbsNetPosRequestMethodInt methods;
		TLbsNetPosMethodInt posMethods[9];
   		posMethods[0].SetPosMethod(KLbsPositioningMeansGps,
   								   TPositionModuleInfo::ETechnologyTerminal|TPositionModuleInfo::ETechnologyAssisted);
   		posMethods[1].SetPosMethod(KLbsPositioningMeansGps,
   								   TPositionModuleInfo::ETechnologyNetwork|TPositionModuleInfo::ETechnologyAssisted);
   		// Add 17 more...
   		for (TUint index = 2; index < 10; index++ )
   		{
   			posMethods[index].SetPosMethod(KLbsPositioningMeansGps,
   						   TPositionModuleInfo::ETechnologyNetwork|TPositionModuleInfo::ETechnologyAssisted);
   		}
   								   			   
   		__ASSERT_ALWAYS (KErrNone == methods.SetPosMethods(posMethods, 9), User::Invariant());
		
		// Configure H/W Capabilities as Terminal Based and Terminal Assisted only
		__ASSERT_ALWAYS(KErrNone == iCapabilitiesProperty.Set(KManagerTestSuiteUid, CTe_AgpsManagerStepBase::ETestCapabilitiesKey,
															  TPositionModuleInfoExtended::EDeviceGpsModeSimultaneousTATB),
															  User::Invariant());
		
		//Inject a location request from LS for Autonomous method		
		iLsToManagerRequestInjector->RequestPositionUpdate(methods);
		iGpsModuleObserver->WaitForCallBack();
		}
	  return TestStepResult();
	}

TVerdict CTe_AgpsManagerStep21::doTestStepPostambleL()
	{
	CTe_AgpsManagerStepBase::doTestStepPostambleL();
	return TestStepResult();
	}
