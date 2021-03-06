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
// This contains the header file for MT-LR GPS mode options Tests
// 
//

#ifndef __CT_LBS_MTLR_RESET_ASSISTANCE_H__
#define __CT_LBS_MTLR_RESET_ASSISTANCE_H__

// LBS includes.
#include <lbs.h>
#include <lbscommon.h>
#include <lbs/lbsprivacycontroller.h>
#include <lbs/lbsadmin.h>

// LBS test includes.
#include "ctlbshybridmtlrstep.h"
#include <lbs/test/lbsnetprotocolproxy.h>
#include <lbs/test/transmitserverwatch.h>

// Literals used
_LIT(KLbsMTLRResetAssistance, "LbsMTLRResetAssistance");

class CT_LbsMTLRResetAssistance :	public	CT_LbsHybridMTLRStep, public MLbsPrivacyObserver
	{
public:
	~CT_LbsMTLRResetAssistance();

	static CT_LbsMTLRResetAssistance* New(CT_LbsHybridMTLRServer& aParent);
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();

	
protected:
	CT_LbsMTLRResetAssistance(CT_LbsHybridMTLRServer& aParent);
	void ConstructL();
	
protected:
// from MLbsPrivacyObserver
void ProcessNetworkLocationRequest(TUint aRequestId, const TLbsExternalRequestInfo& aRequestInfo, const TNotificationType& aNotificationType);
void ProcessNetworkPositionUpdate(TUint aRequestId, const TPositionInfo& aPosInfo);
void ProcessRequestComplete(TUint aRequestId, TInt aReason);	

private:
	TLbsNetPosRequestMethod RequestNetworkMethod(TBool aWarmUp);
	void DecideWhatNetworkReceives();
	TBool SendResetAssistanceDataL();

private:
	enum TChosenNetworkMethod
		{
		ENetworkMethodInvalid,
		ETerminalBasedNetworkMethod,
		ETerminalBasedTerminalAssistedNetworkMethod,
		ETerminalAssistedNetworkMethod,
		ETerminalAssistedTerminalBasedNetworkMethod,
		EAutonomousNetworkMethod,
		ENetworkMethodNotSet
		};
	enum TPlannedPositionOriginator
		{
		EPositionOriginatorUnkown,
		EPositionOriginatorModule,
		EPositionOriginatorNetwork
		};
	enum TNetworkStep
		{
		EInitial,
		EBeforeSession,
		EBeginningOfSession,
		EAfterAssistanceData,
		EAfterMeasurements,
		EAfterPosition,
		EAfterSessionClose
		};
	
	TLbsNetSessionId iSessionId;
	CNetProtocolProxy* iProxy;	
	
	/** Used so the network requests the method specified in the ini file */
	TChosenNetworkMethod iNetworkMethod;
	/** Used so the network proxy knows if it should return the positions 
	 	or should let the module calculate it in hybrid case */
	TPlannedPositionOriginator iPlannedPositionOriginator;
	/** Used so the network proxy knows what to expect */
	TBool iNetworkExpectsMeasurements;
	TBool iNetworkExpectsPositions;
	/* If network requests a mode that the module does not support it will be rejected */
	TBool iNetRequestRejected;
	
	enum TState
	{
	EInitializing,
	EPrivacyCheckOk,
	ERefLocReceived,
	EGpsLocReceived,
	ERequestComplete
	};
	
	TState iState;
	CLbsPrivacyController* iController;
	
	/** So we know what to expect when network doesn't specify mode */
	CLbsAdmin::TGpsMode iAdminGpsMode;
	
	TNetworkStep iStepForSendingReset;
	TBool iWarmUpModule;
	TLbsAsistanceDataGroup iResetMask;
	};

#endif //__CT_LBS_MTLR_RESET_ASSISTANCE_H__
