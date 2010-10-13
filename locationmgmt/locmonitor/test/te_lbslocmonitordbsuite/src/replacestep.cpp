/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#include "replacestep.h"
#include "te_lbslocationmonitorsuitedefs.h"

#include "rlbslocmonitordb.h"

#include <Lbs.h> 


CReplaceStep::~CReplaceStep()
/**
 * Destructor
 */
	{

	}

CReplaceStep::CReplaceStep()
/**
 * Constructor
 */
	{
	SetTestStepName(KReplaceStep);
	}

TVerdict CReplaceStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CTe_LbsLocationMonitorSuiteStepBase::doTestStepPreambleL();
	return TestStepResult();
	}


TVerdict CReplaceStep::doTestStepL()
/**
 * @return - TVerdict code
 * This test check that KErrArgument is returned when calling the Get function for the 
 * KLbsSpecialFeatureMaximumRequestDelay entry with an out of range delay value.
 */
	{
	__UHEAP_MARK;
	  if (TestStepResult()==EPass)
		{
		RLbsLocMonitorDb locMonitorDb = RLbsLocMonitorDb();
		locMonitorDb.OpenL();
		CleanupClosePushL(locMonitorDb);
		CTe_LbsLocationMonitorSuiteStepBase::InitWaitersL();
		
		// First Reset DB to initial state
		locMonitorDb.ClearDatabase();
		
		// Add first cell position

		TPosition dummyPos1;
		dummyPos1.SetCoordinate(10.10, 1.01, 11.11);
		dummyPos1.SetHorizontalAccuracy(100);
		dummyPos1.SetVerticalAccuracy(1000);

		INFO_PRINTF1(_L("Saving first position"));
		TRequestStatus status;
		TLbsLocMonitorAreaInfoGci in1;
		in1.iMcc = 120;
		in1.iMnc = 101;
		in1.iLac = 2000;
		in1.iCid = 3000;
		RPointerArray<TLbsLocMonitorAreaInfoBase> in1Array;
		in1Array.Append(&in1);
		locMonitorDb.SavePosition(dummyPos1,in1Array,iWaiter->iStatus);
		iWaiter->StartAndWait();
		
		// Check first cell position was correctly stored
		INFO_PRINTF1(_L("Retrieving first position"));
		TLbsLocMonitorAreaInfoGci out1;
		out1.iMcc = 120;
		out1.iMnc = 101;
		out1.iLac = 2000;
		out1.iCid = 3000;
		RPointerArray<TLbsLocMonitorAreaInfoBase> outArray;
		outArray.Append(&out1);
		TPosition outPosition1;
		TPositionAreaExtendedInfo matchLevel; 
		locMonitorDb.GetPosition(outPosition1,outArray,matchLevel,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		if(!matchLevel.CellIdMatch())
			{
			ERR_PRINTF2(_L("Read Match Level: %d, Expected 4"), matchLevel.Area());
			SetTestStepResult(EFail);
			}
		ComparePositions(dummyPos1,outPosition1);
	
		// Add second cell position
		TPosition dummyPos2;
		dummyPos2.SetCoordinate(20.20, 2.02, 22.22);
		dummyPos2.SetHorizontalAccuracy(200);
		dummyPos2.SetVerticalAccuracy(2000);		
		INFO_PRINTF1(_L("Saving second position"));
		locMonitorDb.SavePosition(dummyPos2,in1Array,iWaiter->iStatus);
		iWaiter->StartAndWait();
		in1Array.Reset();
		
		// Check check second cell position has replaced first
		TLbsLocMonitorAreaInfoGci out2;
		out2.iMcc = 120;
		out2.iMnc = 101;
		out2.iLac = 2000;
		out2.iCid = 3000;
		INFO_PRINTF1(_L("Retrieving second position"));
		TPosition outPosition2;
		outArray.Append(&out2);
		locMonitorDb.GetPosition(outPosition2,outArray,matchLevel,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		if(!matchLevel.CellIdMatch())
			{
			ERR_PRINTF2(_L("Read Match Level: %d, Expected 4"), matchLevel.Area());
			SetTestStepResult(EFail);
			}
		ComparePositions(dummyPos2,outPosition2);
		
		// Reset DB to initial state again
		locMonitorDb.ClearDatabase();
		
		CleanupStack::PopAndDestroy(&locMonitorDb);
		DeleteWaiters();
		}
	  __UHEAP_MARKEND;
	  return TestStepResult();
	}



TVerdict CReplaceStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 */
	{

	return TestStepResult();
	}
