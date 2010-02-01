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


#ifndef LBS_LOCMONITORDBENGINE_H
#define LBS_LOCMONITORDBENGINE_H


//-------------------------------------------------------------------------------
#include <e32base.h>
#include <SqlDb.h> 
#include <LbsPositionInfo.h> 
#include <e32capability.h>
//TODO remove
#include "rlbslocmonitordb.h"

//-------------------------------------------------------------------------------
/** 
*/
class CLbsLocMonitorDbEngine : public CActive
	{

public:		
	static CLbsLocMonitorDbEngine* NewL();
	virtual ~CLbsLocMonitorDbEngine();
	TInt SavePosition(TUint aMcc, TUint aMnc, TUint aLac, 
			TUint aCid, const TPosition& aPosition, TRequestStatus& aStatus);
	TInt GetPosition(TUint aMcc, TUint aMnc, TUint aLac, 
			TUint aCid, TPosition& aPosition, TPositionAreaExtendedInfo& aMatchingAreaInfo, TRequestStatus& aStatus);
	TInt GetPosition(TPosition& aPosition, TRequestStatus& aStatus);
	TInt ClearDatabase();
	static TInt FlushTimerCallback(TAny* aPtr);


private:
	CLbsLocMonitorDbEngine();
	void ConstructL();
	void InitDbL();
	TBool Select(TPosition& aPosition, TPtrC aQuery, 
			TInt aMcc = KErrNotFound, TInt aMnc = KErrNotFound, 
			TInt aLac = KErrNotFound, TInt aCid = KErrNotFound);
	void CheckFlush();
	void Flush(TBool aShutdown);
	TPositionAreaExtendedInfo CacheMatchLevel(TInt aMcc, TInt aMnc, TInt aLac, TInt aCid);
	TInt Insert(TBool aShutdown);
	TInt DbSize();
	virtual void RunL();
	virtual void DoCancel();
	
private:
	RSqlDatabase iDatabase;
	CPeriodic* iPeriodic;
	TTimeIntervalMicroSeconds32 iFlushInterval;
	TInt iCount;
	RSqlStatement iSqlSaveStatement;
	TRequestStatus* iClientStatus;
	
	TInt iLastMcc;
	TInt iLastMnc;
	TInt iLastLac;
	TInt iLastCid;
	TPosition iLastPosition;
	TTime iLastTime;
	TBool iIsLastValid;
	};
	



#endif //LBS_LOCATIONMONITORDATABASE_H
