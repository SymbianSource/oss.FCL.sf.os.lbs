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
 @file suplconnectionmanager0Step.cpp
 @internalTechnology
*/
#include "suplconnectionmanager0Step.h"
#include "Te_suplconnectionmanagerSuiteDefs.h"
#include "socketwriter.h"
#include <lbs/lbshostsettingsclasstypes.h>
#include "suplstart.h"


Csuplconnectionmanager0Step::~Csuplconnectionmanager0Step()
/**
 * Destructor
 */
	{
	}

Csuplconnectionmanager0Step::Csuplconnectionmanager0Step()
	{
	SetTestStepName(Ksuplconnectionmanager0Step);
	}

TVerdict Csuplconnectionmanager0Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Csuplconnectionmanager0Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		Start();
		}
	  return TestStepResult();
	}



TVerdict Csuplconnectionmanager0Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}


void Csuplconnectionmanager0Step::MessageRecieved(TDesC8& /*aBuffer*/)
	{
	SendSuplEnd();
	}

void Csuplconnectionmanager0Step::Connected(const TInetAddr& /*aAddr*/)
	{
	SendSuplStart();
	}

void Csuplconnectionmanager0Step::ConnectionError(const TSuplConnectionError& /*aError*/)
	{
	}

void Csuplconnectionmanager0Step::MessageReceived(CSuplMessageBase* aSuplMessage)
	{
	delete aSuplMessage;
	SetTestStepResult(EPass);
	Stop();
	}

