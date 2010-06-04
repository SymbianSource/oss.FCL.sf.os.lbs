/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  The locationengine API header file
*
*/


#ifndef CTEST_LOCATIONENGINE_API_IMPL_H
#define CTEST_LOCATIONENGINE_API_IMPL_H

#include <e32base.h>
#include <lbs.h>

// LBS test includes
#include <lbs/test/ctlbstimerutils.h>
#include "locationengine_api.h"
#include "ctestharnesshandler.h"


class CNpeApiImpl : public CBase, MT_LbsTimerUtilsObserver, MT_RequestObserver
    {
public:
    static CNpeApiImpl& NpeApiImpl();
    static void InstantiateL(MNpeObserver* aObserver);
    static void Cleanup();
    
public:
    // API Version
    void Version(TVersion& aApi);

    // Misc. interfaces
    void SetGpsOptions(TNpeGpsMode aGpsMode, TBool aGpsTimeRelationReq);
    void AdvisePowerMode(TNpePowerMode aPowerMode);

    // Position interface
    void PositionRequest(const TTime& aTargetTime, TInt aMaxFixTime, TInt aHorUncertainty, TInt aVerUncertainty);
    void CancelPositionRequest(void);

    // Assistance data delivery interface
    void DeliverAssistanceData(TLbsAsistanceDataGroup aDataGroup, RLbsAssistanceDataReaderSet& aDataReaderSet);
    void DeleteAssistanceData(TNpeAdTypes aData);

    // MT_AGpsModuleTimerObserver function.
    void HandleTimerL(TInt aTimerId, const TTime& aTargetTime);

    // MT_RequestObserver
    TInt ProcessRequestUpdateInit(const TDesC& aConfigFileName, const TDesC& aConfigSection);
    void ProcessRequestTimeOut(const TTimeIntervalMicroSeconds& aTimeOut);
    void ProcessRequestError(TInt aError);
    TInt ProcessRequestModuleOptions(TLbsHybridModuleOptions aModuleMode, TBool aValue);
    void ProcessRequestForcedUpdate();
    void ProcessImmediateMeasurements(TInt aImmediateMeasurements);

    void SetClientUsage(TNpeClientInfo aClientInfo);

private:
    CNpeApiImpl(MNpeObserver* aObservers);
    void ConstructL();
    void DataCleanup();

	// Helper function to return position info data.
	void UpdateLocation();
	void ReturnNanPosition();
	void ReturnNanMeas();
    void DumpUpdateToRefFile(const TPositionInfoBase* aPosInfoArray[], TInt aNumItems);
    void ClearAssistanceData();
    void PublishGpsMode(const TNpeGpsMode aGpsMode);

	// Populates update with dummy data.
	void PopulateUpdateArrayL();

	// Module options functions.
	void SetOption(TLbsHybridModuleOptions aModuleOption, TBool aValue);
	TBool GetOption(TLbsHybridModuleOptions aModuleOption);

	
private:
    //CNpeApiPrivate  *iApiImplementation;
	MNpeObserver* iNpeObserver;
	TUint32 iModuleOptions;
	TNpeGpsMode iGpsMode;

	/** Position update timer used to handle position update requests from manager. */
	CT_LbsTimerUtils* iPosUpdateTimer;
	
	/** Used to cause the module to delay returning the actual position, to reflect a more
		realistic behaviour when receiving assistance data and returning positions. */
	CT_LbsTimerUtils* iDelayUpdateTimer;


	/** Interval variables required by the multi-update module option.
	*/
	TTimeIntervalMicroSeconds iIntervalDelay;
	TInt iExtraUpdatesCtr;


	/** Communication channel between A-Gps module and Test Harness.
	*/
	CT_TestHarnessHandler* iTHHandler;

	RPointerArray<TLbsModuleUpdateItem> iUpdateArr;
	TInt iUpdateArrIndex;
	
	TTimeIntervalMicroSeconds iTimeOut;
	TInt iError;
	TInt iImmediateMeasurements;
	
	/** Target time. */
	TTime iTargetTime;

	/** Flag used to determine if a assistance data request is required.
	
	Set to ETrue if assistance data is required.
	Set to EFalse if assistance is not required.
	*/
	TBool iAssistanceDataOn;

	/** Flag used to determine if assistance has been requested.

	Set to ETrue if assistance data has already been requested.
	Set to EFalse if assistance data has not been requested.
	*/
	TBool iAssistanceDataRequested;
	

	/** Flag used to determine if a assistance data is already available.
	
	Set to ETrue if it is available.
	Set to EFalse if assistance data has not been already received.
	*/
	TBool iAssistanceDataAvailable;
	
	/** Request time, used to record the time the request for assistance was made, to
		allow validation of assistance data time stamp.
	*/
	//TTime iAssistanceDataRequestedTime;
	
	TBool iGPSModeNotSupported;
	
	/** List of the assistance data that we have requested but not yet received */
	TLbsAsistanceDataGroup iOutstandingAssistanceData;
 
	};

#endif /* CTEST_LOCATIONENGINE_API_IMPL_H */
