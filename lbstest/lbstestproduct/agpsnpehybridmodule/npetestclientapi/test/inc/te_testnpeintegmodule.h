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

#ifndef TE_TESTNPEINTEGMODULE_H
#define TE_TESTNPEINTEGMODULE_H

#include <lbs/test/ctlbsagpshandler.h>
#include "locationengine_api.h"

//************************************************************************************************************
//   CTestManagerBase
//************************************************************************************************************
class CTe_TestNpeIntegModule : public CTimer, public MNpeObserver, MT_ResponseObserver 
	{
public:
	~CTe_TestNpeIntegModule();
	
	//from MNpeObserver
    void PositionUpdate(TInt aStatus, const TPositionInfoBase *aPositionDataArray[], TInt aNumItems, const TTime &aTargetTime);
    void AssistanceDataRequest(TLbsAssistanceDataGroup aAssistanceData);
    void GpsStatus(TNpeGpsStatus aStatus);
	void GpsQualityStatus(TNpeGpsQualityStatus aQualityStatus);

	// from MT_ResponseObserver
	virtual void ProcessAGpsResponseMessage(const TT_LbsAGpsResponseMsg::TModuleResponseType aResponse);

	
protected:
	CTe_TestNpeIntegModule();
	void ConstructL();
	
	void CompleteSelf(TInt aReason = KErrNone);
	
	void RequestLocation();
	void CheckLocationUpdateReceived();
	
    void Shutdown();
protected:
	TBool 	iShutdown;
	TBool	iInitDone;
	TBool	iUpdateReceived;
	TInt	iRequestCounter;
	TBool	iExpectNanPosition;

	TTime 						iTargetTime;
	TLbsLocRequestQuality 		iQuality;
	TLbsGpsOptionsArray 		iGpsOptions;
	TNpeGpsMode            iGpsMode;
	
	CNpeApi* iModule;

	// Communication channel between A-Gps module and Test Harness
	CT_LbsAGpsHandler* iAGpsHandler;
	};


#endif	//	TE_TESTNPEINTEGMODULE_H
