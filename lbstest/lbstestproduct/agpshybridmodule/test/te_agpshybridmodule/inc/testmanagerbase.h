// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TESTMANAGERBASE_H
#define TESTMANAGERBASE_H

#include <lbs/lbslocdatasourcegpsbase.h>
#include <lbs/test/ctlbsagpshandler.h>

const TUid	KAGpsHybridModuleUid	= {0x1028228E};

//************************************************************************************************************
//   CTestManagerBase
//************************************************************************************************************
class CTestManagerBase : public CTimer, public MLbsLocationSourceGpsObserver, MT_ResponseObserver 
	{
public:
	~CTestManagerBase();
	
	//from MLbsLocationSourceGpsObserver
	virtual void UpdateLocation(TInt aStatus, const TPositionInfoBase* aPosInfoArray[], TInt aNumItems, const TTime& aTargetTime);
  	virtual void UpdateDeviceStatus(TPositionModuleStatus::TDeviceStatus aDeviceStatus);
  	virtual void UpdateDataQualityStatus(TPositionModuleStatus::TDataQualityStatus aDataQuality);
	virtual TInt GetAssistanceDataItem(TLbsAssistanceDataItem aItem, RDataReaderRootBase& aDataRoot, TTime& aTimeStamp);
	virtual TInt GetAssistanceDataItemTimeStamp(TLbsAssistanceDataItem aItem, TTime& aTimeStamp);
	virtual void RequestAssistanceData(TLbsAsistanceDataGroup aDataItemMask);
  	virtual void Shutdown();

	// from MT_ResponseObserver
	virtual void ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse);

protected:
	CTestManagerBase();
	void ConstructL();
	
	void CompleteSelf(TInt aReason = KErrNone);
	
	void RequestLocation();
	void CheckLocationUpdateReceived();
	
protected:
	TBool 	iShutdown;
	TBool	iInitDone;
	TBool	iUpdateReceived;
	TInt	iRequestCounter;
	TBool	iExpectNanPosition;

	TTime 						iTargetTime;
	TLbsLocRequestQuality 		iQuality;
	TLbsGpsOptionsArray 		iGpsOptions;
	
	CLbsLocationSourceGpsBase* iModule;

	// Communication channel between A-Gps module and Test Harness
	CT_LbsAGpsHandler* iAGpsHandler;
	};


#endif	//	TESTMANAGERBASE_H
