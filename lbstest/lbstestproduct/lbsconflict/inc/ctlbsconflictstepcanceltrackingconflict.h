// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file ctlbsconflictstepcanceltrackingconflict.h
// This contains the header file for Location Data Source Tests
// 
//

#ifndef __CT_LBS_CONFLICT_STEP_CANCELTRACKINGCONFLICT_H__
#define __CT_LBS_CONFLICT_STEP_CANCELTRACKINGCONFLICT_H__

// LBS includes.
#include <lbs.h>
#include <lbscommon.h>
#include <lbs/lbsnetcommon.h>
//#include <lbs/lbsprivacycontroller.h>

// LBS test includes.
#include "ctlbsconflictstep.h"
#include "ctlbsconflictserver.h"
#include "ctlbsdoposupdate.h"
#include "ctlbsdox3p.h"
//#include "ctlbsassdatamoduledata.h"

// Literals used
_LIT(KLbsConflictStep_canceltrackingconflict, "LbsConflictStep_canceltrackingconflict");

// Callbacks flags.
//const TLbsCallbackFlags KLbsCallback_Got_NotifyUpdate			= 0x1000;
//const TLbsCallbackFlags KLbsCallback_Got_X3P					= 0x4000;
//const TLbsCallbackFlags KLbsCallback_Got_ModuleConflictAnswer	= 0x2000;


class CT_LbsConflictStep_canceltrackingconflict :	public	CT_LbsConflictStep,
								public	MT_LbsDoPosUpdateObserver,
								public	MT_LbsDoX3PObserver
//								public	MT_LbsConflictModuleDataBusObserver,
//								public	MLbsPrivacyObserver
	{
public:
	~CT_LbsConflictStep_canceltrackingconflict();

	static CT_LbsConflictStep_canceltrackingconflict* New(CT_LbsConflictServer& aParent);
	virtual TVerdict doTestStepL();

	// From CT_LbsNetSimStep via CT_LbsConflictStep
	void Connected();
	void Disconnected();
	void NotifyRegisterLcsMoLr(const TDesC& aData);
	void NotifyReleaseLcsMoLr(TInt aReason);
	void NotifyMeasurementControlLocation(const TPositionInfo& aPosition, const RLbsAssistanceDataBuilderSet& aData, const TLbsNetPosRequestQuality& aQuality);
	void NotifyReleaseLcsLocationNotification(const CLbsNetworkProtocolBase::TLbsPrivacyResponse& aResult);
	void NotifyFacilityLcsMoLrResult(TInt aReason, const TPositionInfo& aPosition);
	void NotifyMeasurementReportLocation(const TPositionInfo& aPosition);
	void NotifyMeasurementReportRequestMoreAssistanceData(const TLbsAssistanceDataGroup& aFilter);
	void NotifyMeasurementReportControlFailure(TInt aReason);

	// From MT_LbsDoPosUpdateObserver - Position notify update callback.
	void MT_LbsDoPosUpdateCallback(TRequestStatus& aStatus);
	// put this in base class maybe !! no keep here

	void MT_LbsDoX3PCallback(TInt aTransmitId, TRequestStatus& aStatus);

	// From MT_LbsDoPosUpdateObserver - Module out data bus updated.
//	void HandleDataBusUpdateL(const TModuleDataOut& aModuleDataOut);

	// From MLbsPrivacyObserver.
//	void ProcessNetworkLocationRequest(TUint aRequestId, const TLbsExternalRequestInfo& aRequestInfo, const TNotificationType& aNotificationType);
//	void ProcessNetworkPositionUpdate(TUint aRequestId, const TPositionInfo& aPosInfo);
//	void ProcessRequestComplete(TUint aRequestId, TInt aReason);

private:
	CT_LbsConflictStep_canceltrackingconflict(CT_LbsConflictServer& aParent);
	void ConstructL();		

	void SetExpectedSeq();
	void VerifyPosInfos();

	enum {	EReqPosUpdate = CT_LbsConflictStep::ELast,
			EReqX3P,
			EReqPosUpdate2};

	
	/** Current test case being executed.
	*/
	TInt iTestCaseId;

	/** Active object wrapper to allow MO-LRs.
	*/
	// move to base class, no keep here
	CT_LbsDoPosUpdate* iDoPosUpdate;

	CT_LbsDoX3P* iDoX3P;
	// so each test writes it's own netsim func it needs, an doposupdate + dox3p



	/** Test module data bus monitor, to allow the module to return test status to
		the test step.
	*/
//	CT_LbsConflictModuleDataBusMonitor* iModuleDataBusMonitor;

	/** Privacy controller, required to preform a MT-LR.
	*/
//	CLbsPrivacyController* iPrivacyController;

	/** MT-LR request id, used to verify the request ids passed to MT-LR callbacks.
	*/	
//	TUint iMtlrRequestId;

	/** Used to monitor the number of assistance data events we receviced during a test.
	*/
//	TUint iAssEventCount;
			
			
	/** Indicates cancel in operation.
	*/
	TBool iCancelSubState;
	
	/** Indivcates expecting self locate request to completed with KErrPositionHighPriorityReceive
	*/
	TBool iKErrHighPriorityRecExpected;
	
	/** Indivcates expecting self locate request to completed with KErrServerBusy
	*/
	TBool iKErrServerBusyExpected;
	
	TBool iDoX3PPushGetMC;
	
	TBool iKDoASecondNPUD;
	
	TBool iX3PPush;
	
	TBool iDoAX3PTimer;
	
	TBool iHadLastCallBack;
	};


// Helper function, to copy position infos.
//TInt CopyPositionTypes(TPositionInfoBase& aTo, const TPositionInfoBase& aFrom);
//TInt SupportedType(const TUint32& aType, const TInt& aSize);

#endif //__CT_LBS_CONFLICT_STEP_CANCELTRACKINGCONFLICT_H__
