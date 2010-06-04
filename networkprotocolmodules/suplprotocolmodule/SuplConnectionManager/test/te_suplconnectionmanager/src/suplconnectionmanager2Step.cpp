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
 @file suplconnectionmanager2Step.cpp
 @internalTechnology
*/
#include "suplconnectionmanager2Step.h"
#include "Te_suplconnectionmanagerSuiteDefs.h"

Csuplconnectionmanager2Step::~Csuplconnectionmanager2Step()
/**
 * Destructor
 */
	{
	}

Csuplconnectionmanager2Step::Csuplconnectionmanager2Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ksuplconnectionmanager2Step);
	}

TVerdict Csuplconnectionmanager2Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Csuplconnectionmanager2Step::doTestStepL()
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
		}
	  return TestStepResult();
	}



TVerdict Csuplconnectionmanager2Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}


void Csuplconnectionmanager2Step::MessageRecieved(TDesC8& /*aBuffer*/) // Network side
	{
	// Dont do anything, as this is a timeout test
	}

void Csuplconnectionmanager2Step::Connected(const TInetAddr& /*aAddr*/)
	{
	ASSERT(iState == ESuplStart);
	
	iState = ESuplResponse;
	SendSuplStart();
	}

void Csuplconnectionmanager2Step::ConnectionError(const TSuplConnectionError& aError)
	{
	if (aError == MSuplConnectionManagerObserver::EConnectionLost)
		{
		SetTestStepResult(EPass);
		}
	
	Stop();
	}

void Csuplconnectionmanager2Step::MessageReceived(CSuplMessageBase* aSuplMessage) // From connection
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
