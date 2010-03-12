/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



#include "clbslocmonitordbengine.h"
#include "lbslocmonitordbenginedefs.h"
#include "lbsdevloggermacros.h"



CLbsLocMonitorDbEngine* CLbsLocMonitorDbEngine::NewL()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::NewL");
	CLbsLocMonitorDbEngine* self = new(ELeave)CLbsLocMonitorDbEngine();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


void CLbsLocMonitorDbEngine::ConstructL()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::ConstructL");
	InitDbL();
	iPeriodic = CPeriodic::NewL(EPriorityStandard);
	iPeriodic->Start(KInitialPeriod, KFlushPeriod, TCallBack(FlushTimerCallback, this));
	}		


void CLbsLocMonitorDbEngine::InitDbL()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::InitDbL");
	iCount = 0;
	iIsLastValid = EFalse;
	TInt error = iDatabase.Open(KSecureLocMonDB);
	if(KErrNotFound == error)
		{		
		RSqlSecurityPolicy securityPolicy;
		User::LeaveIfError(securityPolicy.Create(TSecurityPolicy(TSecurityPolicy::EAlwaysPass)));
		CleanupClosePushL(securityPolicy);
		User::LeaveIfError(securityPolicy.SetDbPolicy(RSqlSecurityPolicy::EWritePolicy, TSecurityPolicy(TSecurityPolicy::EAlwaysPass)));
		#ifdef LBS_LOCMONITORDB_TEST
		User::LeaveIfError(iDatabase.Create(KSecureLocMonDB));
		#else
		User::LeaveIfError(iDatabase.Create(KSecureLocMonDB, securityPolicy));
		#endif
		CleanupStack::PopAndDestroy(&securityPolicy);
		iDBInitialised = ETrue;
		User::LeaveIfError(iDatabase.Exec(KCreateTable));
		User::LeaveIfError(iDatabase.Exec(KCreateIndex4));
		User::LeaveIfError(iDatabase.Exec(KCreateIndex3));
		}
	else
		{
		User::LeaveIfError(error);
		iDBInitialised = ETrue;
		}
	User::LeaveIfError(iDatabase.Exec(KCreateTempTable));
	User::LeaveIfError(iDatabase.Exec(KCreateTempIndex4));
	}


CLbsLocMonitorDbEngine::CLbsLocMonitorDbEngine():
CActive(EPriorityStandard),
iDBInitialised(EFalse)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::CLbsLocMonitorDbEngine");
	CActiveScheduler::Add(this);
	}


CLbsLocMonitorDbEngine::~CLbsLocMonitorDbEngine()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::~CLbsLocMonitorDbEngine");
	Cancel();
	if(iPeriodic)
		{
		iPeriodic->Cancel();
		delete iPeriodic;
		}
	if(iDBInitialised)
		{
		iSqlSaveStatement.Close();
		if(iIsLastValid)
			{
			Insert(ETrue);
			}

		Flush(ETrue);
		
		iDatabase.Close();
		}
	}


TInt CLbsLocMonitorDbEngine::SavePosition(TUint aMcc, TUint aMnc, TUint aLac, TUint aCid, const TPosition& aPosition, TRequestStatus& aStatus)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::SavePosition");
	if(aMcc > KMaxTInt || aMnc > KMaxTInt || aLac > KMaxTInt || aCid > KMaxTInt)
		{
		return KErrArgument;
		}		
	iClientStatus = &aStatus;
	*iClientStatus = KRequestPending;
	// If the cache does not contain a cell, this cell goes in the cache
	if (!iIsLastValid)
		{
		iLastMcc = aMcc;
		iLastMnc = aMnc;
		iLastLac = aLac;
		iLastCid = aCid;
		iLastTime.UniversalTime();
		iLastPosition = aPosition;
		iIsLastValid = ETrue;
		User::RequestComplete(iClientStatus, KErrNone);
		return KErrNone;
		} 
	// If this cell is the same as the the cache, update the cache's position and timestamp
	
	else if(CacheMatchLevel(aMcc, aMnc, aLac, aCid).CellIdMatch())
		{
		iLastPosition = aPosition;
		iLastTime.UniversalTime();
		User::RequestComplete(iClientStatus, KErrNone);
		return KErrNone;
		}
	// If this cell is different from the cache, send the cache to db server then put this cell in cache
	else
		{
		TInt result = Insert(EFalse);
		if(result >= KErrNone)
			{
			iLastMcc = aMcc;
			iLastMnc = aMnc;
			iLastLac = aLac;
			iLastCid = aCid;
			iLastTime.UniversalTime();
			iLastPosition = aPosition;
			iIsLastValid = ETrue;
			}
		return result;
		}
	}


TInt CLbsLocMonitorDbEngine::Insert(TBool aShutdown)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::Insert");
	TInt error = KErrNone;
	TPckg<TPosition> positionDes( iLastPosition );
	error = iSqlSaveStatement.Prepare(iDatabase, KUpsertRow);
	if(error == KErrNone)
		{
		TInt indexMcc = iSqlSaveStatement.ParameterIndex(KMcc);
		TInt indexMnc = iSqlSaveStatement.ParameterIndex(KMnc);
		TInt indexLac = iSqlSaveStatement.ParameterIndex(KLac);
		TInt indexCid = iSqlSaveStatement.ParameterIndex(KCid);
		TInt indexStamp = iSqlSaveStatement.ParameterIndex(KStamp);
		TInt indexData = iSqlSaveStatement.ParameterIndex(KData);
		
		iSqlSaveStatement.BindInt(indexMcc, iLastMcc);
		iSqlSaveStatement.BindInt(indexMnc, iLastMnc);
		iSqlSaveStatement.BindInt(indexLac, iLastLac);
		iSqlSaveStatement.BindInt(indexCid, iLastCid);
		iSqlSaveStatement.BindInt64(indexStamp, iLastTime.Int64());
		iSqlSaveStatement.BindBinary(indexData, positionDes);
		
		// Shutdown inserts are synchronous, regular inserts asynchronous
		if(aShutdown)
			{
			iSqlSaveStatement.Exec();
			iSqlSaveStatement.Close();
			}
		else
			{
			// Statement is closed in RunL, once asynchrnous insert has taken place
			iStatus = KRequestPending;
			iSqlSaveStatement.Exec(iStatus);
			SetActive();
			}
		}
	return error;
	}


TInt CLbsLocMonitorDbEngine::GetPosition(TUint aMcc, TUint aMnc, TUint aLac, TUint aCid, TPosition& aPosition, TPositionAreaExtendedInfo& aMatchingAreaInfo, TRequestStatus& aStatus)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::GetPosition");
	if(aMcc > KMaxTInt || aMnc > KMaxTInt || aLac > KMaxTInt || aCid > KMaxTInt)
		{
		return KErrArgument;
		}
		
	TInt result = KErrNone;
	// Clear result
	aMatchingAreaInfo.SetMobileCountryCodeMatch(EFalse);
	aMatchingAreaInfo.SetMobileNetworkCodeMatch(EFalse);
	aMatchingAreaInfo.SetLocationAreaCodeMatch(EFalse);
	aMatchingAreaInfo.SetCellIdMatch(EFalse);
	aMatchingAreaInfo.SetArea(TPositionAreaExtendedInfo::EAreaUnknown);
	TPositionAreaExtendedInfo cacheMatch = CacheMatchLevel(aMcc, aMnc, aLac, aCid);
	if(cacheMatch.CellIdMatch() || Select(aPosition, TPtrC(KSelectTempRow4), aMcc, aMnc, aLac, aCid) || Select(aPosition, TPtrC(KSelectRow4), aMcc, aMnc, aLac, aCid))
		{		
		if(cacheMatch.CellIdMatch())
			{
			aPosition = iLastPosition;
			}
		aMatchingAreaInfo.SetMobileCountryCodeMatch(ETrue);
		aMatchingAreaInfo.SetMobileNetworkCodeMatch(ETrue);
		aMatchingAreaInfo.SetLocationAreaCodeMatch(ETrue);
		aMatchingAreaInfo.SetCellIdMatch(ETrue);
		aMatchingAreaInfo.SetArea(TPositionAreaExtendedInfo::EAreaCity);
		}
	else if(cacheMatch.LocationAreaCodeMatch() || Select(aPosition, TPtrC(KSelectTempRow3), aMcc, aMnc, aLac) || Select(aPosition, TPtrC(KSelectRow3), aMcc, aMnc, aLac))
		{
		if(cacheMatch.LocationAreaCodeMatch())
			{
			aPosition = iLastPosition;
			}
		aMatchingAreaInfo.SetMobileCountryCodeMatch(ETrue);
		aMatchingAreaInfo.SetMobileNetworkCodeMatch(ETrue);
		aMatchingAreaInfo.SetLocationAreaCodeMatch(ETrue);
		aMatchingAreaInfo.SetArea(TPositionAreaExtendedInfo::EAreaRegion);
		}
	else if(cacheMatch.MobileNetworkCodeMatch() || Select(aPosition, TPtrC(KSelectTempRow2), aMcc, aMnc) || Select(aPosition, TPtrC(KSelectRow2), aMcc, aMnc))
		{
		if(cacheMatch.MobileNetworkCodeMatch())
			{
			aPosition = iLastPosition;
			}
		aMatchingAreaInfo.SetMobileCountryCodeMatch(ETrue);
		aMatchingAreaInfo.SetMobileNetworkCodeMatch(ETrue);
		aMatchingAreaInfo.SetArea(TPositionAreaExtendedInfo::EAreaCountry);
		}
	else if(cacheMatch.MobileCountryCodeMatch() || Select(aPosition, TPtrC(KSelectTempRow1), aMcc) || Select(aPosition, TPtrC(KSelectRow1), aMcc))
		{
		if(cacheMatch.MobileCountryCodeMatch())
			{
			aPosition = iLastPosition;
			}
		aMatchingAreaInfo.SetMobileCountryCodeMatch(ETrue);
		aMatchingAreaInfo.SetArea(TPositionAreaExtendedInfo::EAreaCountry);
		}
	else if(iIsLastValid || Select(aPosition, TPtrC(KSelectTempRowLatest)) || Select(aPosition, TPtrC(KSelectRowLatest)) || iIsLastValid)
		{
		if(iIsLastValid)
			{
			aPosition = iLastPosition;
			}
		}
	else
		{
		result = KErrNotFound;
		}
	TRequestStatus* status = &aStatus;
	*status = KRequestPending;
	User::RequestComplete(status, result);
	return result;
	}


TInt CLbsLocMonitorDbEngine::GetPosition(TPosition& aPosition, TRequestStatus& aStatus)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::GetPosition");
	TInt result = KErrUnknown;
	// Before going to db server, check if we can get cell position from cache
	if(iIsLastValid)
		{
		aPosition = iLastPosition;
		result = KErrNone;
		}
	else if(Select(aPosition, TPtrC(KSelectRowLatest)) || Select(aPosition, TPtrC(KSelectRowLatest)))
		{
		result = KErrNone;
		}
	TRequestStatus* status = &aStatus;
	*status = KRequestPending;
	User::RequestComplete(status, result);
	return result;
	}

TInt CLbsLocMonitorDbEngine::ClearDatabase()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::ClearDatabase");	
	iIsLastValid = EFalse;
	iDatabase.Exec(KClear);
	iDatabase.Close();
	TInt del = iDatabase.Delete(KSecureLocMonDB);
	TRAPD(init, InitDbL());
    return (KErrNone != del) ? del: init;
	}


TBool CLbsLocMonitorDbEngine::Select(TPosition& aPosition, TPtrC aQuery, TInt aMcc, TInt aMnc, TInt aLac, TInt aCid)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::Select");
	TBool found = EFalse;
	TPckg<TPosition> positionDes( aPosition );
	RSqlStatement sqlStatement;
	sqlStatement.Prepare(iDatabase, aQuery);	
	if(aMcc > KErrNotFound)
		{
		TInt indexMcc = sqlStatement.ParameterIndex(KMcc);
		sqlStatement.BindInt(indexMcc, aMcc);
		}
	if(aMnc > KErrNotFound)
		{
		TInt indexMnc = sqlStatement.ParameterIndex(KMnc);
		sqlStatement.BindInt(indexMnc, aMnc);
		}
	if(aLac > KErrNotFound)
		{
		TInt indexLac = sqlStatement.ParameterIndex(KLac);
		sqlStatement.BindInt(indexLac, aLac);
		}
	if(aCid > KErrNotFound)
		{
		TInt indexCid = sqlStatement.ParameterIndex(KCid);
		sqlStatement.BindInt(indexCid, aCid);
		}
		
	TInt columnIndex = sqlStatement.ColumnIndex(KDataColumn);
	if(sqlStatement.Next() == KSqlAtRow)
		{
		sqlStatement.ColumnBinary(columnIndex, positionDes);
		found = ETrue;
		}
	sqlStatement.Close();
	return found;
	}


void CLbsLocMonitorDbEngine::CheckFlush()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::CheckFlush");
	iCount++;
	if(iCount > KTempFlushSize)
		{
		Flush(EFalse);		
		}
	}


void CLbsLocMonitorDbEngine::Flush(TBool aShutdown)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::Flush");
	// Only do this if an insert is not currently in progress
	if(!IsActive())
		{	
		iCount = 0;
		// Move all data from temp table to permanent table
		iDatabase.Exec(KBegin);
		iDatabase.Exec(KCopy);	
		iDatabase.Exec(KClear);
		if((!aShutdown) && (DbSize() > KMaxDbSize))
			{
			// Delete 2000 oldest records
			iDatabase.Exec(KPrune);
			}
		iDatabase.Exec(KCommit);
		}
	}


TInt CLbsLocMonitorDbEngine::FlushTimerCallback(TAny* aPtr)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::FlushTimerCallback");
	CLbsLocMonitorDbEngine* DbEngine = static_cast<CLbsLocMonitorDbEngine*>(aPtr);
	if (DbEngine)
		{
		DbEngine->Flush(EFalse);
		}
	return KErrNone;
	}


// Returns number of rows in db
TInt CLbsLocMonitorDbEngine::DbSize()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::DbSize");
	TInt result = KErrNotFound;
	RSqlStatement sqlStatement;
	sqlStatement.Prepare(iDatabase, KCount);
	TInt columnIndex = sqlStatement.ColumnIndex(KCountColumn);
	if(sqlStatement.Next() == KSqlAtRow)
		{
		result = sqlStatement.ColumnInt(columnIndex);
		}
	sqlStatement.Close();
	return result;
	}


TPositionAreaExtendedInfo CLbsLocMonitorDbEngine::CacheMatchLevel(TInt aMcc, TInt aMnc, TInt aLac, TInt aCid)
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::CacheMatchLevel");
	TPositionAreaExtendedInfo areaInfo;

	if(iIsLastValid)
		{
		if((aMcc == iLastMcc))
			{
			if(aMnc == iLastMnc)
				{
				if(aLac == iLastLac)
					{
					if(aCid == iLastCid)
						{
						areaInfo.SetCellIdMatch(ETrue);
						}
					areaInfo.SetLocationAreaCodeMatch(ETrue);
					}
				areaInfo.SetMobileNetworkCodeMatch(ETrue);
				}
			areaInfo.SetMobileCountryCodeMatch(ETrue);
			}
		}

	return areaInfo;
	}



void CLbsLocMonitorDbEngine::RunL()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::RunL");
	User::RequestComplete(iClientStatus, KErrNone);
	iSqlSaveStatement.Close();
	CheckFlush();
	}


void CLbsLocMonitorDbEngine::DoCancel()
	{
	LBSLOG(ELogP1,"->CLbsLocMonitorDbEngine::DoCancel");
	*iClientStatus = KRequestPending;
	User::RequestComplete(iClientStatus, KErrCancel);
	}		
