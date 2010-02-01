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

#include "rrlptest.h"
#include "rrlpassistancedata.h"
#include "rrlpprotocolerror.h"
#include "rrlpmeasureposrequest.h"

#include <lbspositioninfo.h>

CRrlpTest* CRrlpTest::NewL()
	{
	CRrlpTest* self = new(ELeave) CRrlpTest();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}

CRrlpTest::CRrlpTest() :
	CActive(CActive::EPriorityStandard), iNextMessage(0), iMessageSequence(10)
	{
	}

void CRrlpTest::ConstructL()
	{
	CActiveScheduler::Add(this);
	iBuilderSet.OpenL();
	iSuplRrlpFsm = CSuplRrlpFsm::NewL(*this, iBuilderSet);
	iTimer = CLbsCallbackTimer::NewL(*this);
	}

CRrlpTest::~CRrlpTest()
	{
	delete iSuplRrlpFsm;
	delete iTimer;
	iBuilderSet.Close();
	}

void CRrlpTest::GoL()
	{
	Complete();
	}

void CRrlpTest::SetSequence(TInt* aSequence, TInt aCount)
	{
	iMessageSequence.AppendL(aSequence, aCount);
	}

void CRrlpTest::RunL()
	{
	TInt message = NextMessage(); 
	switch (message)
		{
		case ESendAssistanceData:
		case ESendAssistanceDataFinal:
			{
			CRrlpAssistanceData* assdata = CRrlpAssistanceData::NewL();
			if (message == ESendAssistanceData)
				{
				assdata->SetMoreAssDataToBeSent(ETrue);
				}
			else
				{
				assdata->SetMoreAssDataToBeSent(EFalse);
				}
			
			iSuplRrlpFsm->ProcessPositioningMessage(assdata);
			break;
			}
		case ESendMeasurementRequest:
		case ESendMeasurementRequestWithAssistanceData:
			{
			CRrlpMeasurePositionRequest* posreq = CRrlpMeasurePositionRequest::NewL();
			
			RLbsAssistanceDataBuilderSet assdata; 
			if (message == ESendMeasurementRequestWithAssistanceData)
				{
				assdata.OpenL();
				posreq->SetAssistanceDataBuilderSet(assdata);
				}
			
			iSuplRrlpFsm->ProcessPositioningMessage(posreq);

			if (message == ESendMeasurementRequestWithAssistanceData)
				{
				assdata.Close();
				}
			
			break;
			}
		case ESendLocationResponse:
			{
			TPositionInfo posInfo;
			iSuplRrlpFsm->LocationResp(KErrNone, posInfo);
			break;
			}
		case ESendProtocolError:
			{
			CRrlpProtocolError* protErr = CRrlpProtocolError::NewL(EFalse);
			
			iSuplRrlpFsm->ProcessPositioningMessage(protErr);

			break;
			}
		case EDoTimeDelay:
			{
			TTimeIntervalSeconds time(NextMessage());
			iTimer->EventAfter(time, 0);
			break;
			}
		case EFinished:
			{
			// Cancel the timer incase it is outstanding
			iTimer->Cancel();
			CActiveScheduler::Stop();
			break;
			}
		default:
			{
			User::Leave(KErrGeneral);
			}
		}
	}

void CRrlpTest::DoCancel()
	{
	}


void CRrlpTest::ProcessAssistanceData(const TLbsAsistanceDataGroup& /*aGroupMask*/, TInt /*aReason*/)

	{
	if (NextMessage() != EWaitingProcessAssistanceData)
		{
		User::Leave(KErrGeneral);
		}
	
	if (PeekMessage() != EWaitingProcessPositionRequest)
		{
		Complete();
		}
	}


void CRrlpTest::ProcessPositioningRequest(const TLbsNetPosRequestQuality& /*aQuality*/,
										   const TLbsNetPosRequestMethod& /*aPosMethod*/)
	{
	if (NextMessage() == EWaitingProcessPositionRequest)
		{
		Complete();
		}
	}

void CRrlpTest::PositioningPayloadToNetwork(const CSuplPosPayload* aPayload)
	{
	TInt message = (const_cast<CSuplPosPayload*>(aPayload))->MessageType(); 
	switch (message)
		{
		case CSuplPosPayload::ERrlpAssistanceDataAck:
			{
			// Check that is what we are waiting for
			if (NextMessage() == EWaitingAssistanceDataAck)
				{
				if (PeekMessage() != EWaitingProcessAssistanceData)
					{
					Complete();
					}
				}
			else
				{
				User::Leave(KErrGeneral);
				}
			break;
			}
		case CSuplPosPayload::ERrlpMeasurePositionResp:
			{
			// Check this is what we are waiting for
			if (NextMessage() == EWaitingLocationResponse)
				{
				Complete();
				}
			else
				{
				User::Leave(KErrGeneral);
				}
			break;
			}
		default:
			{
			User::Leave(KErrGeneral);
			}
		};
	
	delete aPayload;	
	}

void CRrlpTest::PositioningProtocolError(const TInt& aError)
	{
	if (NextMessage() == EWaitingProtocolError)
		{
		if (aError == KErrTimedOut)
			{
			// We have probally entered a delay on purpose so cancel the timer
			iTimer->Cancel();
			}
		Complete();
		}
	else
		{
		User::Leave(KErrGeneral);
		}
	}


void CRrlpTest::PositioningSessionEnded()
	{
		
	}

void CRrlpTest::Complete()
	{
	TRequestStatus* temp = &iStatus;
	User::RequestComplete(temp, KErrNone);
	SetActive();
	}

TInt CRrlpTest::NextMessage()
	{
	TInt ret = iMessageSequence.At(iNextMessage);
	++iNextMessage;
	return ret;
	}

TInt CRrlpTest::PeekMessage()
	{
	return iMessageSequence.At(iNextMessage);
	}

void CRrlpTest::OnTimerEventL(TInt /*aTimerId*/)
	{
	// This should never be called as the timer is used for delaying to trigger the state machines
	// timeouts.
	User::Leave(KErrGeneral);
	}


TInt CRrlpTest::OnTimerError(TInt /*aTimerId*/, TInt /*aError*/)
	{
	Complete();
	
	return 0;
	}
