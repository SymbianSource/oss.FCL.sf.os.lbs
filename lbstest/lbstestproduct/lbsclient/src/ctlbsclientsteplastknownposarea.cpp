/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* @file ctlbsclientstep_notifyposupdate.cpp
* This is the class implementation for the Last Known Position Area Tests
*
*/

 
#include "ctlbsclientsteplastknownposarea.h"

#include <lbs.h>
#include <lbssatellite.h>

#include <lbs/test/tlbsutils.h>
#include <lbs/test/ctlbsasyncwaiter.h>

#include <etel3rdparty.h>
#include <simtsy.h>


// constant definitions
_LIT(KLbsClientStepLastKnownPosArea, "LbsClientStepLastKnownPosArea");
const TInt KSimTsyTransitionDelay = 4000000; // Currently it seems to take the SimTsy 3 seconds to change cell


/**
 * Destructor
 */
CT_LbsClientStep_LastKnownPosArea::~CT_LbsClientStep_LastKnownPosArea()
	{
	}


/**
 * Constructor
 */
CT_LbsClientStep_LastKnownPosArea::CT_LbsClientStep_LastKnownPosArea(CT_LbsClientServer& aParent) : CT_LbsClientStep(aParent)
	{
	SetTestStepName(KLbsClientStep_LastKnownPosArea);
	}


/**
Static Constructor
*/
CT_LbsClientStep_LastKnownPosArea* CT_LbsClientStep_LastKnownPosArea::New(CT_LbsClientServer& aParent)
	{
	// Note the lack of ELeave.
	// This means that having insufficient memory will return NULL;
	CT_LbsClientStep_LastKnownPosArea* testStep = new CT_LbsClientStep_LastKnownPosArea(aParent);
	if (testStep)
		{
		TInt err = KErrNone;
		TRAP(err, testStep->ConstructL());
		if (err)
			{
			delete testStep;
			testStep = NULL;
			}
		}
	return testStep;
	}


void CT_LbsClientStep_LastKnownPosArea::ConstructL()
	{
	}

//from MT_NotifyPosUpdateObserver:
/**
 * 	Callback - called when NotifyPositionUpdate request completes
 */

void CT_LbsClientStep_LastKnownPosArea::NotifyPositionUpdateCallback(TRequestStatus& aStatus)
	{
	TInt err = aStatus.Int();
	if (KErrCancel != err)	
		{
		SetTestStepResult(EFail); // the request always gets cancelled (see test 0122)
		}
	CActiveScheduler::Stop();
	}

/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
TVerdict CT_LbsClientStep_LastKnownPosArea::doTestStepL()
	{
	// Test step used to test the LBS Client Notify Position Area update API.
	INFO_PRINTF1(_L("&gt;&gt;CT_LbsClientStep_LastKnownPosArea::doTestStepL()"));

	if (TestStepResult()==EPass)
		{
		// Connect to self locate server.	
		TInt err = iServer.Connect();
		User::LeaveIfError(err);
		CleanupClosePushL(iServer);
		RPointerArray<TAny>& posInfoArr = iParent.iSharedData->iCurrentPosInfoArr;
		T_LbsUtils utils;
		utils.ResetAndDestroy_PosInfoArr(posInfoArr);
		TPositionModuleId modId;
		modId = utils.GetAGpsModuleIdL(iServer);
		User::LeaveIfError(iPositioner.Open(iServer, modId));
		CleanupClosePushL(iPositioner);
		
		// Carry out actions.
		TInt testCaseId;
		if (GetIntFromConfig(ConfigSection(), KTestCaseId, testCaseId))
			{
			switch (testCaseId)
				{
				// Test case LBS-Client-LastKnownPosArea-0001
				// Ensure correct match levels are returned
				case 1:
					{
					// Empty Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);

					TPositionInfo posInfo2;
					TPositionAreaExtendedInfo matchLevel;
					TPositionAreaExtendedInfo expectedMatchLevel;

					TPositionInfo* posInfo = new(ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);

					// Check that last known position is completed correctly with no known area and nothing in cache.
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);

					// no position and database returns KErrUnknown in these circumstances.
					if ( KErrUnknown != err)
						{
						ERR_PRINTF2(_L("Expected KErrUnknown, incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}

					//1. Move to cell 234.15.1911.36464
					err = RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//2. NPUD
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//3. LastKnown Pos Area
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaCity, ETrue, ETrue, ETrue, ETrue);
					//4. Move to Cell 234.15.1911.64762
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//5. LastKnown Pos Area
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaRegion, ETrue, ETrue, ETrue, EFalse);
					//6. 234.15.2020.57354
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//7. Lastknown Pos Area
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaCountry, ETrue, ETrue, EFalse, EFalse);
					//8. Move to Cell 234.30.4321.52345
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//9. Lastknown Pos Area
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaCountry, ETrue, EFalse, EFalse, EFalse);
					//10. Move to Cell 262.06.5323.64523
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//11. Lastknown Pos Area
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaUnknown, EFalse, EFalse, EFalse, EFalse);
					//12. Clear Database
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);
					break;
					}
				// Test case LBS-Client-LastKnownPosArea-0002
				// Ensure that the database is wiped out properly
				case 2:
					{
					TPositionInfo* posInfo = new(ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);

					TInt err = KErrNone;
					//1. Move to cell 234.15.1911.36464
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//2. NPUD
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//3. LastKnown Pos Area
					TPositionInfo posInfo2;
					TPositionAreaExtendedInfo matchLevel, expectedMatchLevel;
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaCity, ETrue, ETrue, ETrue, ETrue);
					//4. Clear Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);

					//5. LastKnown Pos Area
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNotFound != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					// No Validation, if request fails paramters are not valid
					//Validate(*posInfo, posInfo2, matchLevel, EFalse, EFalse, EFalse, EFalse);
					break;
					}
				// Test case LBS-Client-LastKnownPosArea-0003
				// Verify that the last position is returned if current network position is unavailable
				case 3:
					{
					TPositionInfo* posInfo = new(ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);
					TInt err = KErrNone;
					
					// The SIM config file used by this test contains an
					// invalid global cell-id (LAC is zero). A transition to
					// a new valid cell is not provoked by this test. Therefore
					// the Location Monitor never gets to know a valid cell in this test.

					// Request a position so that the position goes into the database
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}

					// Since the Location Monitor never received a valid value of
					// GCI, the call to GetLastKnownPositionArea should return 
					// the last stored position (posInfo) together with area information
					// that shows the "rough area accuracy" to be TPositionAreaInfo::EAreaUnknown.
					TPositionInfo posInfo2;
					TPositionAreaExtendedInfo matchLevel, expectedMatchLevel;
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
						
					// Verify that the position received in the call to GLKPA is the last known position
					// due to the NPUD and also that the accuracy is set to TPositionAreaInfo::EAreaUnknown
					// (in this case the rest of boolean values passed in Validate() are irrelevant).
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaUnknown, EFalse, EFalse, EFalse, EFalse);
					
					//7. Clear Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);
					break;
					}
				// Test case LBS-Client-LastKnownPosArea-0004
				// verify that the last position is returned if requested by two clients sequentially
				case 4:
					{
					RPositioner positioner2;
					User::LeaveIfError(positioner2.Open(iServer, modId));
					CleanupClosePushL(positioner2);
					TPositionInfo* posInfo = new(ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);
					TInt err = KErrNone;
					//1. Move to cell 234.15.1911.36464
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//2. NPUD
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//3. LastKnown Pos Area from client 1
					TPositionInfo posInfo2;
					TPositionAreaExtendedInfo matchLevel, expectedMatchLevel;
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaCity, ETrue, ETrue, ETrue, ETrue);
					//4. LastKnown Pos Area from client 2
					TPositionInfo posInfo3;
					TRequestStatus status;
					TPositionAreaExtendedInfo matchLevel2;
					positioner2.GetLastKnownPositionArea(posInfo3,matchLevel2,status);
					User::WaitForRequest(status);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo3, matchLevel, TPositionAreaInfo::EAreaCity, ETrue, ETrue, ETrue, ETrue);
					//5. Clear Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);
					CleanupStack::PopAndDestroy(&positioner2);
					break;
					}
				// Test case LBS-Client-LastKnownPosArea-0005
				// Verify that Last Known Position area can be cancelled
				case 5:
					{
					TPositionInfo* posInfo = new(ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);
					TInt err = KErrNone;
					//1. Move to cell 234.15.1911.36464
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//2. NPUD
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//3. LastKnown Pos Area from client 1
					RPositioner positioner2;
					User::LeaveIfError(positioner2.Open(iServer, modId));
					CleanupClosePushL(positioner2);
					TPositionInfo posInfo2;
					TRequestStatus status;
					TPositionAreaExtendedInfo matchLevel;
					positioner2.GetLastKnownPositionArea(posInfo2, matchLevel, status);
					//4. Cancel LastKnown Pos Area
					err = positioner2.CancelRequest(RPositioner::EReqGetLastKnownPositionArea);
					User::WaitForRequest(status);
					if(err != KErrNone && err != KErrNotFound)
						{
						ERR_PRINTF2(_L("Incorrect Cancel error: %d"), err);
						SetTestStepResult(EFail);
						}
					//5. Clear Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);
					if((err == KErrNone && status.Int() != KErrCancel) || 
							(err == KErrNotFound && status.Int() != KErrNone))
						{
						ERR_PRINTF2(_L("Cancel cancel inconsistency, was: %d"), status.Int());
						SetTestStepResult(EFail);
						}
					CleanupStack::PopAndDestroy(&positioner2);
					break;
					}
				// Test case LBS-Client-LastKnownPosArea-0006
				// Verify that clear last known position area database can be cancelled
				case 6:
					{
					TPositionInfo* posInfo = new(ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);
					TInt err = KErrNone;
					//1. Move to cell 234.15.1911.36464
					RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//2. NPUD
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//3. LastKnown Pos Area
					TPositionInfo posInfo2;
					TPositionAreaExtendedInfo matchLevel, expectedMatchLevel;
					err = DoLastKnownPosAreaL(posInfo2, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo2, matchLevel, TPositionAreaInfo::EAreaCity, ETrue, ETrue, ETrue, ETrue);
					//4. Clear Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					//5. Cancel Clear Database
					iServer.CancelRequest(RPositionServer::EReqEmptyLastKnownPositionStore);
					User::WaitForRequest(emptyStatus);
					if(emptyStatus.Int() != KErrNone && emptyStatus.Int() != KErrCancel)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), emptyStatus.Int());
						SetTestStepResult(EFail);
						}
					//6. Clear Database
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);
					break;
					}
				// Test case LBS-Client-LastKnownPosArea-0007
				// Simultaneous NPUD and GLNPA
				case 7:
					{
					TPositionInfo* posInfo = new (ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);
					TPositionInfo* posInfo2 = new (ELeave) TPositionInfo();
					posInfoArr.Append(posInfo2);
					TInt err = KErrNone;
					//1. Move to cell 234.15.1911.36464
					RProperty::Set(KUidPSSimTsyCategory,
							KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//2. NPUD
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//3. NPUD (with no waiting for completion) 
					TPositionInfo posInfo3;
					TRequestStatus status;
					iPositioner.NotifyPositionUpdate(*posInfo2, status);
					//4. LastKnown Pos Area 
					TPositionAreaExtendedInfo matchLevel, expectedMatchLevel;
					err = DoLastKnownPosAreaL(posInfo3, matchLevel);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					Validate(*posInfo, posInfo3, matchLevel, TPositionAreaInfo::EAreaCity, ETrue, ETrue,
							ETrue, ETrue);
					//Now wait for the NPUD we did earlier
					User::WaitForRequest(status);
					//5. Clear Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);
					break;
					}
				// Test case LBS-Client-LastKnownPosArea-0008
				// Simultaneous NPUD and GLNPA with cancels
				case 8:
					{
					TPositionInfo* posInfo = new (ELeave) TPositionInfo();
					posInfoArr.Append(posInfo);
					TPositionInfo* posInfo2 = new (ELeave) TPositionInfo();
					posInfoArr.Append(posInfo2);
					TInt err = KErrNone;
					//1. Move to cell 234.15.1911.36464
					RProperty::Set(KUidPSSimTsyCategory,
							KPSSimTsyTimersReduceTime, KReduceSimTsyTimers);
					User::After(KSimTsyTransitionDelay);
					//2. NPUD
					err = DoNotifyUpdateL(*posInfo);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//3. NPUD (with no waiting for completion)
					TPositionInfo posInfo3;
					TRequestStatus status2, status3;
					iPositioner.NotifyPositionUpdate(*posInfo2, status2);
					TPositionAreaExtendedInfo matchLevel, expectedMatchLevel;
					//4. LastKnown Pos Area 
					iPositioner.GetLastKnownPositionArea(posInfo3, matchLevel, status3);
					if (KErrNone != err)
						{
						ERR_PRINTF2(_L("Incorrect err %d returned"), err);
						SetTestStepResult(EFail);
						}
					//5. Cancel NPUD
					iPositioner.CancelRequest(RPositioner::EReqNotifyPositionUpdate);
					//6. Cancel LastKnown Pos Area
					iPositioner.CancelRequest(RPositioner::EReqGetLastKnownPositionArea);
					//Validate(*posInfo, posInfo3, matchLevel, ETrue, ETrue,
					//		ETrue, ETrue);
					//Now wait for the NPUD&GLKPA we did earlier
					User::WaitForRequest(status2);
					User::WaitForRequest(status3);
					//7. Clear Database
					TRequestStatus emptyStatus;
					iServer.EmptyLastKnownPositionStore(emptyStatus);
					User::WaitForRequest(emptyStatus);
					break;
					}
				default:
					User::Panic(KLbsClientStepLastKnownPosArea, KErrUnknown);					
    		    }
			}
		// All done, clean up.
		CleanupStack::PopAndDestroy(&iPositioner);
		CleanupStack::PopAndDestroy(&iServer);		
		}
	INFO_PRINTF1(_L("&lt;&lt;CT_LbsClientStep_LastKnownPosArea::doTestStepL()"));
	return TestStepResult();
	}


void CT_LbsClientStep_LastKnownPosArea::Validate(TPositionInfo& aExpectedPosition, TPositionInfo& aActualPosition, 
		TPositionAreaExtendedInfo& aActualArea, TPositionAreaInfo::TArea aArea, TBool aMccExpected, TBool aMncExpected, TBool aLacExpected, TBool aCidExpected)
	{
	TPositionAreaExtendedInfo expectedArea;
	
	if (aArea != TPositionAreaInfo::EAreaUnknown)
		{
		expectedArea.SetArea(aArea);
		expectedArea.SetMobileCountryCodeMatch(aMccExpected);
		expectedArea.SetMobileNetworkCodeMatch(aMncExpected);
		expectedArea.SetLocationAreaCodeMatch(aLacExpected);
		expectedArea.SetCellIdMatch(aCidExpected);	
		VerifyMatchLevel(expectedArea, aActualArea);	
		}
	else
		{
		if (aActualArea.Area() != TPositionAreaInfo::EAreaUnknown)
			{
			ERR_PRINTF1(_L("Incorrect area reported when EAreaUnknown was expected"));	
			}
		}

	T_LbsUtils utils;
	TBool equality = utils.Compare_PosInfo(aExpectedPosition, aActualPosition);
	if(!equality)
		{
		ERR_PRINTF1(_L("Position does not match"));
		SetTestStepResult(EFail);
		}
	}


TInt CT_LbsClientStep_LastKnownPosArea::VerifyMatchLevel(TPositionAreaExtendedInfo& aExpected, TPositionAreaExtendedInfo& aActual)
	{
	TInt err = KErrNone;
	
	if (aExpected.Area() != aActual.Area())
		{
		ERR_PRINTF1(_L("Incorrect Area accuracy level returned"));
		ERR_PRINTF2(_L("Expected: Area:%d"), aExpected.Area());
		ERR_PRINTF2(_L("Actual: Area:%d"), aActual.Area());
		SetTestStepResult(EFail);
		err = KErrGeneral;
		}
	
	if (!(aExpected.MobileCountryCodeMatch() == aActual.MobileCountryCodeMatch() &&
			aExpected.MobileNetworkCodeMatch() == aActual.MobileNetworkCodeMatch() &&
			aExpected.LocationAreaCodeMatch() == aActual.LocationAreaCodeMatch() &&
			aExpected.CellIdMatch() == aActual.CellIdMatch()))
		{
		ERR_PRINTF1(_L("Incorrect Match level"));
		ERR_PRINTF5(_L("Expected: MCC:%d, MNC:%d, LAC:%d, CID:%d"), 
				aExpected.MobileCountryCodeMatch(), 
				aExpected.MobileNetworkCodeMatch(),
				aExpected.LocationAreaCodeMatch(),
				aExpected.CellIdMatch());
		ERR_PRINTF5(_L("Actual: MCC:%d, MNC:%d, LAC:%d, CID:%d"), 
				aActual.MobileCountryCodeMatch(), 
				aActual.MobileNetworkCodeMatch(),
				aActual.LocationAreaCodeMatch(),
				aActual.CellIdMatch());
		SetTestStepResult(EFail);
		err = KErrGeneral;
		}
	return err;
	}

TInt CT_LbsClientStep_LastKnownPosArea::DoLastKnownPosAreaL(TPositionInfoBase& aPosInfo, TPositionAreaExtendedInfo& aMatchLevel)
{
	// Use wrapper active object for the async call below.
	CT_LbsAsyncWaiter*	waiter = CT_LbsAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	User::LeaveIfError(iPositioner.SetRequestor(CRequestor::ERequestorService,
												CRequestor::EFormatApplication,
												_L("LbsClient LKPA Tests")));
	iPositioner.GetLastKnownPositionArea(aPosInfo, aMatchLevel, waiter->iStatus);
	// Wait for and process the result.
	waiter->StartAndWait();
	TInt err = waiter->Result();
	if (KErrNone == err)
		{
		INFO_PRINTF1(_L("Last Known Pos Area position obtained successfully"));
		}
	else
		{
		INFO_PRINTF2(_L("Position update failed with error %d"), err);
		}
	CleanupStack::PopAndDestroy(waiter);
	return err;
}
