// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Example CTestStep derived implementation
// 
//

/**
 @file suplconnectionmanager1Step.cpp
 @internalTechnology
*/
#include "suplconnectionmanager3Step.h"
#include "Te_suplconnectionmanagerSuiteDefs.h"

Csuplconnectionmanager3Step::~Csuplconnectionmanager3Step()
/**
 * Destructor
 */
	{
	}

Csuplconnectionmanager3Step::Csuplconnectionmanager3Step() :
	iState(ESuplStart)
	{
	SetTestStepName(Ksuplconnectionmanager3Step);
	}

TVerdict Csuplconnectionmanager3Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Csuplconnectionmanager3Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		SetTestStepResult(EFail);
		Start();
		SetTestStepResult(EFail);
		iState = ESuplStart;
		Start(); // Lets run it twice
		}
	  return TestStepResult();
	}



TVerdict Csuplconnectionmanager3Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}


void Csuplconnectionmanager3Step::MessageRecieved(TDesC8& /*aBuffer*/) // Network side
	{
	switch (iState)
		{
		case ESuplResponse:
			{
			iState = ESuplPosInit;
			SendSuplResponse();
			break;
			}
		case ESuplPosAssData:
			{
			iState = ESuplPosAck;
			SendSuplPosAssData();
			break;
			}
		case ESuplPosRequest:
			{
			iState = ESuplPosLocationResponse;
			SendSupPosLocReq();
			break;
			}
		case ESuplEnd:
			{
			iState = EFinished;
			SendSuplEnd();
			break;
			}
		default:
			{
			ASSERT(EFalse);
			} // default
		}
	}

void Csuplconnectionmanager3Step::Connected(const TInetAddr& /*aAddr*/)
	{
	ASSERT(iState == ESuplStart);
	
	iState = ESuplResponse;
	SendSuplStart();
	}

void Csuplconnectionmanager3Step::ConnectionError(const TSuplConnectionError& /*aError*/)
	{
	}

void Csuplconnectionmanager3Step::MessageReceived(CSuplMessageBase* aSuplMessage) // From connection
	{
	switch (iState)
		{
		case ESuplPosInit:
			{
			iState = ESuplPosAssData;
			SendSuplPosInit();
			break;
			}
		case ESuplPosAck:
			{
			iState = ESuplPosRequest;
			SendSuplPosAck();
			break;
			}
		case ESuplPosLocationResponse:
			{
			iState = ESuplEnd;
			SendSupPosLocResp();
			break;
			}
		case EFinished:
			{
			SetTestStepResult(EPass);
			Stop();
			break;
			}
		default:
			{
			ASSERT(EFalse);
			break;
			}
		}
	
	delete aSuplMessage;
	}
