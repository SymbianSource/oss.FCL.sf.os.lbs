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

#include "timestampstep.h"
#include "te_lbslocationmonitorsuitedefs.h"

#include "rlbslocmonitordb.h"

#include <Lbs.h> 


CTimeStampStep::~CTimeStampStep()
/**
 * Destructor
 */
	{

	}

CTimeStampStep::CTimeStampStep()
/**
 * Constructor
 */
	{
	SetTestStepName(KTimeStampStep);
	}

TVerdict CTimeStampStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CTe_LbsLocationMonitorSuiteStepBase::doTestStepPreambleL();
	return TestStepResult();
	}


TVerdict CTimeStampStep::doTestStepL()
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
		
		locMonitorDb.ClearDatabase();
		
		TPosition dummy1, dummy2;
		
		dummy1.SetCoordinate(10.33, 54.22, 10.01);
		dummy1.SetHorizontalAccuracy(100);
		dummy1.SetVerticalAccuracy(1000);
		
		dummy2.SetCoordinate(20.22, 74.11, 20.02);
		dummy2.SetHorizontalAccuracy(100);
		dummy2.SetVerticalAccuracy(1000);
		
		INFO_PRINTF1(_L("Saving 3 records, 1 microsecond apart"));
		TLbsLocMonitorAreaInfoGci in1;
		in1.iMcc = 1;
		in1.iMnc = 1;
		in1.iLac = 1;
		in1.iCid = 2;
		RPointerArray<TLbsLocMonitorAreaInfoBase> arrayIn1;
		arrayIn1.Append(&in1);
		locMonitorDb.SavePosition(dummy1,arrayIn1,iWaiter->iStatus);
		iWaiter->StartAndWait();
		arrayIn1.Reset();
		
		User::After(1);
		TLbsLocMonitorAreaInfoGci in2;
		in2.iMcc = 1;
		in2.iMnc = 1;
		in2.iLac = 1;
		in2.iCid = 3;
		RPointerArray<TLbsLocMonitorAreaInfoBase> arrayIn2;
		arrayIn2.Append(&in2);
		locMonitorDb.SavePosition(dummy2,arrayIn2,iWaiter->iStatus);
		iWaiter->StartAndWait();
		arrayIn2.Reset();
		
		User::After(1);
		TLbsLocMonitorAreaInfoGci in3;
		in3.iMcc = 1;
		in3.iMnc = 1;
		in3.iLac = 2;
		in3.iCid = 2;
		RPointerArray<TLbsLocMonitorAreaInfoBase> arrayIn3;
		arrayIn3.Append(&in3);
		locMonitorDb.SavePosition(dummy1,arrayIn3,iWaiter->iStatus);
		iWaiter->StartAndWait();
		arrayIn3.Reset();
		
		INFO_PRINTF1(_L("Doing a GetPosition that matches 1 & 2, Checking that record 2 is returned"));
		TLbsLocMonitorAreaInfoGci out;
		out.iMnc = 1;
		out.iMcc = 1;
		out.iLac = 1;
		out.iCid = 1;
		RPointerArray<TLbsLocMonitorAreaInfoBase> outArray;
		outArray.Append(&out);
		TPosition outPosition;
		TPositionAreaExtendedInfo matchLevel;
		locMonitorDb.GetPosition(outPosition,outArray,matchLevel,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		TEST(matchLevel.LocationAreaCodeMatch());
		ComparePositions(dummy2, outPosition);
		
		INFO_PRINTF1(_L("Doing a GetLastStoredPosition, Checking that record 3 is returned"));
		TPosition outPosition2;
		locMonitorDb.GetLastStoredPosition(outPosition2,iWaiter->iStatus);
		iWaiter->StartAndWait();
		ComparePositions(dummy1, outPosition2);
		
		
		//locMonitorDb.ClearDatabase();
		CleanupStack::PopAndDestroy(&locMonitorDb);
		DeleteWaiters();
		}
	  __UHEAP_MARKEND;
	  return TestStepResult();
	}



TVerdict CTimeStampStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 */
	{

	return TestStepResult();
	}
