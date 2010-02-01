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
 @file TCallStep.cpp
 @internalTechnology
*/
#include "TCallStep.h"
#include "Te_LbsNetProtocolProxyUnitTestSuiteDefs.h"

CTCallStep::~CTCallStep()
/**
 * Destructor
 */
	{
	}

CTCallStep::CTCallStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTCallStep);
	}

TVerdict CTCallStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Test Step Preamble in Class CTCallStep"));
	CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPreambleL();
	if (TestStepResult()!=EPass)
	   return   TestStepResult();
	// process some pre setting to this test step then set SetTestStepResult to EFail or Epass.
	SetTestStepResult(EPass);
	return TestStepResult();
	}


LOCAL_C void Populate(TLbsNetSessionId& aSessionId)
	{
	aSessionId.SetSessionOwner(TUid::Uid(0x8008));
	aSessionId.SetSessionNum(0x3030);	
	}
	
LOCAL_C void Populate(TLbsNetPosRequestPrivacy& aPrivacy)
	{
	aPrivacy.SetRequestAdvice(TLbsNetPosRequestPrivacy::ERequestAdviceNotify);
	aPrivacy.SetRequestAction(TLbsNetPosRequestPrivacy::ERequestActionAllow);
	}
	
LOCAL_C void Populate(TLbsExternalRequestInfo& aRequestInfo)
	{
	aRequestInfo.SetRequesterId(_L8("TCallStepRequestor"));
	aRequestInfo.SetClientName(_L8("TCallStepClient"));
	aRequestInfo.SetClientExternalId(_L8("TCallStepExternalId"));
	}
	
LOCAL_C void Populate(TLbsNetPosRequestQuality& aQuality)
	{
	aQuality.SetMaxFixTime(102030405);
	aQuality.SetMaxFixAge(3322110011);
	aQuality.SetMinHorizontalAccuracy(179.535);
	aQuality.SetMinVerticalAccuracy(22.3344);
	}

LOCAL_C void Populate(TLbsNetPosRequestMethod& aMethod)
	{
	TLbsNetPosMethod method1;
	TLbsNetPosMethod method2;
	method1.SetPosMethod(TUid::Uid(0x9999), 0x01);
	method2.SetPosMethod(TUid::Uid(0x2222), 0x04);
	const TLbsNetPosMethod posMethods [] =
		{
		method1,method2
		};
	
	aMethod.SetPosMethods(posMethods, 2);
	}

LOCAL_C void Populate(RLbsAssistanceDataBuilderSet& aData)
	{
	aData.OpenL();

	// Access the Gps Reference Time which is the only data we support in this simple provider.
	RUEPositioningGpsReferenceTimeBuilder* refTimeBuilder = NULL;

	aData.GetDataBuilder(refTimeBuilder);
	if (refTimeBuilder)
		{
		const TUint KAssistanceData_RefTime_Week = 12;
		const TUint KAssistanceData_RefTime_Tow1MSec = 567;
		// Fill out the reference time items we support - note the builder set is already open.
		refTimeBuilder->SetField(TUEPositioningGpsReferenceTime::EGpsWeek, KAssistanceData_RefTime_Week);
		refTimeBuilder->SetField(TUEPositioningGpsReferenceTime::EGpsTow1Msec, KAssistanceData_RefTime_Tow1MSec);

		// Time stamp the data.
		TTime timeStamp;
		timeStamp.UniversalTime();

		refTimeBuilder->SetTimeStamp(timeStamp);
		}
	}

LOCAL_C void Populate(TPositionInfo& aPositionInfo)
	{
	TCoordinate coordinate(3.14159265359, 2.71828183);
	TLocality locality(coordinate, 99.99);
	TTime time(101010101);
	TPosition position(locality, time);
	aPositionInfo.SetPosition(position);
	}

LOCAL_C void Populate(TLbsNetPosCapabilities& aNetPosCapabilities)
	{
	const TUid posProtocols [] = 
		{
		TUid::Uid(0x01234),TUid::Uid(0x02345),TUid::Uid(0x3456),TUid::Uid(0x4567),TUid::Uid(0x5678)
		};
	aNetPosCapabilities.SetPosProtocols(posProtocols, 5);
	
	TLbsNetPosMethod method1;
	TLbsNetPosMethod method2;
	method1.SetPosMethod(TUid::Uid(0x9999), 0x01);
	method2.SetPosMethod(TUid::Uid(0x2222), 0x04);
	const TLbsNetPosMethod posMethods [] =
		{
		method1,method2
		};
	aNetPosCapabilities.SetPosMethods(posMethods,2);
	}

TVerdict CTCallStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TLbsNetSessionId sessionId;
		TBool emergency = ETrue;
		TLbsNetPosRequestPrivacy privacy;
		TLbsExternalRequestInfo requestInfo;
		MLbsNetworkProtocolObserver::TLbsNetProtocolService service = MLbsNetworkProtocolObserver::EServiceMobileTerminated;
		TLbsNetPosRequestQuality quality;
		TLbsNetPosRequestMethod method;
		TInt reason = KErrNotSupported;
		TLbsAsistanceDataGroup dataRequestMask1 = EAssistanceDataBadSatList;
		RLbsAssistanceDataBuilderSet data;
		TPositionInfo positionInfo;
		TLbsNetPosCapabilities netPosCapabilities;
		MLbsNetworkProtocolObserver::TLbsNetProtocolServiceMask activeServiceMask = 0x0100;
		Populate(sessionId);
		Populate(privacy);
		Populate(requestInfo);
		Populate(quality);
		Populate(method);
		Populate(data);
		Populate(positionInfo);
		Populate(netPosCapabilities);

		iProxy->CallL(ENetMsgProcessPrivacyRequest, &sessionId, &emergency, &privacy, &requestInfo);
		iProxy->CallL(ENetMsgProcessLocationRequest, &sessionId, &emergency, &service, &quality, &method);
		iProxy->CallL(ENetMsgProcessSessionComplete, &sessionId, &reason);
		iProxy->CallL(ENetMsgProcessAssistanceData, &dataRequestMask1, &data, &reason);
		iProxy->CallL(ENetMsgProcessLocationUpdate, &sessionId, &positionInfo);
		iProxy->CallL(ENetMsgGetCurrentCapabilitiesRequest, &netPosCapabilities);
		iProxy->CallL(ENetMsgProcessStatusUpdate, &activeServiceMask);

//		Extended interface functino not used
//		iProxy->CallL(ENetMsgActionExtendedInterface, NULL);

		data.Close();

		SetTestStepResult(EPass);
		}
	  return TestStepResult();
	}



TVerdict CTCallStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Please delete this line or modify it. I am in Test Step Postamble in Class CTCallStep"));
	// process something post setting to the test step
	// uncomment the following line if you have common post setting to all the test steps in there
	// CTe_LbsNetProtocolProxyUnitTestSuiteStepBase::doTestStepPostambleL();
	// uncomment the following line if you have post process or remove the following line if no post process
	// SetTestStepResult(EPass);		// or EFail
	return TestStepResult();
	}
