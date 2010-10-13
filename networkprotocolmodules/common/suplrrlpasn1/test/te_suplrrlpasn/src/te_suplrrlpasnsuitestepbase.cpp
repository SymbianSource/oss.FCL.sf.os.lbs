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
//

/**
 @file te_suplrrlpasnsuitestepbase.cpp
 @internalTechnology
*/

#include "te_suplrrlpasnsuitestepbase.h"
#include "te_suplrrlpasnsuitedefs.h"

// Device driver constants

TVerdict CTe_SuplRrlpAsnSuiteStepBase::doTestStepPreambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all initialisation common to derived classes in here.
 * Make it being able to leave if there are any errors here as there's no point in
 * trying to run a test step if anything fails.
 * The leave will be picked up by the framework.
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTe_SuplRrlpAsnSuiteStepBase::doTestStepPostambleL()
/**
 * @return - TVerdict
 * Implementation of CTestStep base class virtual
 * It is used for doing all after test treatment common to derived classes in here.
 * Make it being able to leave
 * The leave will be picked up by the framework.
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}

CTe_SuplRrlpAsnSuiteStepBase::CTe_SuplRrlpAsnSuiteStepBase()
	{
	}


CTe_SuplRrlpAsnSuiteStepBase::~CTe_SuplRrlpAsnSuiteStepBase()
	{
	}

void CTe_SuplRrlpAsnSuiteStepBase::LeaveIfErrorL(TInt aErr)
	{
	if (aErr != KErrNone)
		{
		User::Leave(aErr);
		}
	}

// check received VERSION element
TInt CTe_SuplRrlpAsnSuiteStepBase::CheckVersionContentL(CSuplVersion& aVersion)
	{
	if (aVersion.iMaj != 1     ||
		aVersion.iMin != 0     ||
		aVersion.iServind != 0 )
		{
		return KErrGeneral;
		}

	return KErrNone;
	}

// check received SESSION ID element	
TInt CTe_SuplRrlpAsnSuiteStepBase::CheckSessionIdContentL(CSuplSessionId& aSessionId)
	{
	// SET SESSION ID
	if (aSessionId.iSetSessionId == NULL)
		return KErrGeneral;
	
	if (aSessionId.iSetSessionId->iSessionId != 12345)
		return KErrGeneral;
	
	if (aSessionId.iSetSessionId->iSetId == NULL)
		return KErrGeneral;
	
	if (aSessionId.iSetSessionId->iSetId->iSetIdType != ESuplSetIdTypeIPAddress)
		return KErrGeneral;
	
	if (aSessionId.iSetSessionId->iSetId->iIpAddress == NULL)
		return KErrGeneral;

	if (aSessionId.iSetSessionId->iSetId->iIpAddress->iIpAddressType != ESuplIpAddressTypeV4)
		return KErrGeneral;
	
	_LIT8(KExpectedSetIdIpAddress, "abcd");
	if (aSessionId.iSetSessionId->iSetId->iIpAddress->iIpAddress.Compare(KExpectedSetIdIpAddress) != 0)
		return KErrGeneral;
	
	// SLP SESSION ID
	if (aSessionId.iSlpSessionIdPresent && aSessionId.iSlpSessionId == NULL)
		return KErrGeneral;
	
	_LIT8(KExpectedSlpIdSessionId, "EFGH");
	if (aSessionId.iSlpSessionId->iSessionId.Compare(KExpectedSlpIdSessionId) != 0)
		return KErrGeneral;
	
	if (aSessionId.iSlpSessionId->iSlpAddress == NULL)
		return KErrGeneral;

	if (aSessionId.iSlpSessionId->iSlpAddress->iSlpAddressType != ESuplSlpAddressTypeIp)
		return KErrGeneral;
	
	if (aSessionId.iSlpSessionId->iSlpAddress->iIpAddress == NULL)
		return KErrGeneral;

	if (aSessionId.iSlpSessionId->iSlpAddress->iIpAddress->iIpAddressType != ESuplIpAddressTypeV4)
		return KErrGeneral;
	
	_LIT8(KExpectedSlpIdIpAddress, "VWXY");
	if (aSessionId.iSlpSessionId->iSlpAddress->iIpAddress->iIpAddress.Compare(KExpectedSlpIdIpAddress) != 0)
		return KErrGeneral;
	
	return KErrNone;
	}
	
// check received RRLP REFERENCE element
TInt CTe_SuplRrlpAsnSuiteStepBase::CheckRrlpReferenceContentL(TRrlpReference& aRrlpRef)
	{
	if (aRrlpRef.aRefNum != 5)
		{
		return KErrGeneral;
		}

	if (!aRrlpRef.aRel5EntendedRefPresent)
		{
		return KErrGeneral;
		}

	if (aRrlpRef.aRel5SmlcCode != 39)
		{
		return KErrGeneral;
		}

	if (aRrlpRef.aRel5TransactionId != 13337)
		{
		return KErrGeneral;
		}
	
	return KErrNone;	
	}

void CTe_SuplRrlpAsnSuiteStepBase::PopulateSessionId(CSuplSessionId* aSessionId, TBool aIsSuplStart)
	{
	aSessionId->iSetSessionId->iSessionId = 12345; // 0..65535;
	aSessionId->iSetSessionId->iSetId->iSetIdType = ESuplSetIdTypeIPAddress; // TSuplSetIdType
	aSessionId->iSetSessionId->iSetId->iIpAddress->iIpAddressType = ESuplIpAddressTypeV4; // TSuplIpAddressType
	_LIT8(KSetIpv4Address,"abcd");
	aSessionId->iSetSessionId->iSetId->iIpAddress->iIpAddress.Copy(KSetIpv4Address); // TBuf8<16>
	aSessionId->iSetSessionIdPresent=ETrue;
	
	// SLP SESSION ID
	if (!aIsSuplStart)
		{
		_LIT8(KSlpIdSessionId, "EFGH");
		aSessionId->iSlpSessionId->iSessionId.Copy(KSlpIdSessionId);
		aSessionId->iSlpSessionId->iSlpAddress->iSlpAddressType = ESuplSlpAddressTypeIp; 
		aSessionId->iSlpSessionId->iSlpAddress->iIpAddress->iIpAddressType = ESuplIpAddressTypeV4;
		_LIT8(KSlpIdIpAddress, "VWXY");
		aSessionId->iSlpSessionId->iSlpAddress->iIpAddress->iIpAddress.Copy(KSlpIdSessionId);
		}
	}

