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

#include "dbstep.h"
#include "te_lbslocationmonitorsuitedefs.h"

#include "rlbslocmonitordb.h" 

#include "ctasyncwaiter.h"


CDBStep::~CDBStep()
/**
 * Destructor
 */
	{

	}

CDBStep::CDBStep()
/**
 * Constructor
 */
	{
	SetTestStepName(KDBStep);
	}

TVerdict CDBStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	CTe_LbsLocationMonitorSuiteStepBase::doTestStepPreambleL();

	iDummyPosition.SetCoordinate(10.33, 54.22, 10.01);
	iDummyPosition.SetHorizontalAccuracy(100);
	iDummyPosition.SetVerticalAccuracy(1000);
	return TestStepResult();
	}


TVerdict CDBStep::doTestStepL()
/**
 * @return - TVerdict code
 * This test check that KErrArgument is returned when calling the Get function for the 
 * KLbsSpecialFeatureMaximumRequestDelay entry with an out of range delay value.
 */
	{
	__UHEAP_MARK;
	  if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("Init locMonitorDb"));
		RLbsLocMonitorDb locMonitorDb = RLbsLocMonitorDb();
		locMonitorDb.OpenL();
		INFO_PRINTF1(_L("Init Waiters"));
		CTe_LbsLocationMonitorSuiteStepBase::InitWaitersL();
		CleanupClosePushL(locMonitorDb);
		
		// First Reset DB to initial state
		INFO_PRINTF1(_L("First Reset DB to initial state"));
		locMonitorDb.ClearDatabase();
		
		INFO_PRINTF1(_L("Adding 10000 records"));
		TTime addStart;
		addStart.UniversalTime();
		for(TInt i = 1; i <= 1000; i++)
			{
			//TRequestStatus status;
			TLbsLocMonitorAreaInfoGci in;
			in.iMcc = i;
			in.iMnc = i+1000;
			in.iLac = i+2000;
			in.iCid = i+3000;
			RPointerArray<TLbsLocMonitorAreaInfoBase> inArray;
			inArray.Append(&in);
			locMonitorDb.SavePosition(iDummyPosition,inArray,iWaiter->iStatus);
			iWaiter->StartAndWait();
			inArray.Reset();
			}
		
		TTime addEnd;
		addEnd.UniversalTime();
		TTimeIntervalMicroSeconds interval = addEnd.MicroSecondsFrom(addStart);
		
		//INFO_PRINTF2(_L("Start Time %d"), addStart.Int64());
		//INFO_PRINTF2(_L("End Time %d "), addEnd.Int64());
		INFO_PRINTF3(_L("%d Adding 10000 records took %d Microseconds"), 0, interval.Int64());
		
		
		//
		// Read
		//
		INFO_PRINTF1(_L("Reading 4 records"));
		TTime readStart;
		readStart.UniversalTime();
		
		// 4 fixed values against previously created db
		TLbsLocMonitorAreaInfoGci out;
		out.iMcc = 51;
		out.iMnc = 1051;
		out.iLac = 2051;
		out.iCid = 3051;
		TPosition outPosition;
		RPointerArray<TLbsLocMonitorAreaInfoBase> outArray;
		outArray.Append(&out);
		TPositionAreaExtendedInfo matchLevel1; 
		locMonitorDb.GetPosition(outPosition,outArray,matchLevel1,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		if(!matchLevel1.CellIdMatch())
			{
			ERR_PRINTF1(_L("Cell Match not found"));
			SetTestStepResult(EFail);
			}
		ComparePositions(iDummyPosition,outPosition);
		
		out.iMcc = 82;
		out.iMnc = 1082;
		out.iLac = 2082;
		out.iCid = 5454;
		outArray.Append(&out);
		TPositionAreaExtendedInfo matchLevel2; 
		locMonitorDb.GetPosition(outPosition,outArray,matchLevel2,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		if(!matchLevel2.LocationAreaCodeMatch())
			{
			ERR_PRINTF1(_L("LAC Match not found"));
			SetTestStepResult(EFail);
			}
		ComparePositions(iDummyPosition,outPosition);
		
		out.iMcc = 423;
		out.iMnc = 43535445;
		out.iLac = 4442342;
		out.iCid = 92922;
		outArray.Append(&out);
		TPositionAreaExtendedInfo matchLevel3; 
		locMonitorDb.GetPosition(outPosition,outArray,matchLevel3,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		if(!matchLevel3.MobileCountryCodeMatch())
			{
			ERR_PRINTF1(_L("MCC match not found"));
			SetTestStepResult(EFail);
			}
		ComparePositions(iDummyPosition,outPosition);

		out.iMcc = 34000023;
		out.iMnc = 4353545;
		out.iLac = 4534;
		out.iCid = 5454;
		outArray.Append(&out);
		TPositionAreaExtendedInfo matchLevel4;  
		locMonitorDb.GetPosition(outPosition,outArray,matchLevel4,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		if(matchLevel4.MobileCountryCodeMatch())
			{
			ERR_PRINTF1(_L("Spurious match found"));
			SetTestStepResult(EFail);
			}
		ComparePositions(iDummyPosition,outPosition);
		
		TTime readEnd;
		readEnd.UniversalTime();
		TTimeIntervalMicroSeconds interval2 = readEnd.MicroSecondsFrom(readStart);
		

		//INFO_PRINTF2(_L("Start Time %d"), readStart.Int64());
		//INFO_PRINTF2(_L("End Time %d "), readEnd.Int64());
		INFO_PRINTF3(_L("%d Reading 4 records took %d Microseconds"), 0, interval2.Int64());
		if(interval2.Int64() > 2000000)
			{
			ERR_PRINTF2(_L("Retrieving 4 records took: %d microseonds, Expected less than 2000000"), interval2.Int64());
			}
		
		INFO_PRINTF1(_L("Performing a read with a second client"));
		RLbsLocMonitorDb locMonitorDb2 = RLbsLocMonitorDb();
		locMonitorDb2.OpenL();
		CleanupClosePushL(locMonitorDb2);
		out.iMcc = 34000023;
		out.iMnc = 4353545;
		out.iLac = 4534;
		out.iCid = 5454;
		outArray.Append(&out);
		TPositionAreaExtendedInfo matchLevel5;  
		locMonitorDb2.GetPosition(outPosition,outArray,matchLevel5,iWaiter->iStatus);
		iWaiter->StartAndWait();
		outArray.Reset();
		if(matchLevel5.MobileCountryCodeMatch())
			{
			ERR_PRINTF1(_L("Spurious match found"));
			SetTestStepResult(EFail);
			}
		ComparePositions(iDummyPosition,outPosition);
		CleanupStack::PopAndDestroy(&locMonitorDb2);
		locMonitorDb.ClearDatabase();
		CleanupStack::PopAndDestroy(&locMonitorDb);
		DeleteWaiters();
		}
	__UHEAP_MARKEND;
    return TestStepResult();
	}


TVerdict CDBStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 */
	{

	return TestStepResult();
	}
