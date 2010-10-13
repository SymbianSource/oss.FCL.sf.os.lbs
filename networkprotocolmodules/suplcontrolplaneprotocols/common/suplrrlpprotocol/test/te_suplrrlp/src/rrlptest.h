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
// teststeps.cpp
// 
//

#ifndef _TESTSTEPS_H
#define _TESTSTEPS_H

#include <lbs/lbsnetcommon.h>
#include <lbs/lbsassistancedatabuilderset.h>

#include "rrlpassistancedata.h"
#include "suplrrlpstatemachine.h"


// Base class
class CRrlpTest : public CActive,
			  	  public MSuplPositioningProtocolFsmObserver,
			  	  public MLbsCallbackTimerObserver
{
public:
	enum TMessages
		{
		ESendAssistanceData,
		ESendAssistanceDataFinal,
		EWaitingAssistanceDataAck,
		EWaitingProcessAssistanceData,
		ESendMeasurementRequest,
		ESendMeasurementRequestWithAssistanceData,
		EWaitingProcessPositionRequest,
		ESendLocationResponse,
		EWaitingLocationResponse,
		ESendProtocolError,
		EWaitingProtocolError,
		EDoTimeDelay,
		EFinished
		};
	
	static CRrlpTest* NewL();
	virtual ~CRrlpTest();

	// CActive
	void RunL();
	void DoCancel();
	
	// Methods
	void GoL();
	void SetSequence(TInt* aSequence, TInt aCount);
protected:
	// MSuplPositioningProtocolFsmObserver
	void ProcessAssistanceData(const TLbsAsistanceDataGroup& aGroupMask, TInt aReason);
	void ProcessPositioningRequest(const TLbsNetPosRequestQuality& aQuality, const TLbsNetPosRequestMethod& aPosMethod);
	void PositioningPayloadToNetwork(const CSuplPosPayload* aPositioningPayload);
	void PositioningProtocolError(const TInt& aError);	
	virtual void PositioningSessionEnded();
	
	
	
	// MLbsCallbackTimerObserver
	virtual void OnTimerEventL(TInt aTimerId);
	virtual TInt OnTimerError(TInt aTimerId, TInt aError);
protected:
	CSuplRrlpFsm*	iSuplRrlpFsm;
	CLbsCallbackTimer* iTimer;
	RLbsAssistanceDataBuilderSet iBuilderSet;
	
	// Message sequence
	TInt				iNextMessage;
	CArrayFixFlat<TInt>	iMessageSequence;
private:
	// Methods
	CRrlpTest();
	void ConstructL();

	void Complete();
	TInt NextMessage();
	TInt PeekMessage();
};


#endif
