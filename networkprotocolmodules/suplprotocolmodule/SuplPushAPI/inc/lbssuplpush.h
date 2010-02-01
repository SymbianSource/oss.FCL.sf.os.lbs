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
// Location Based Services SUPL Push API
// 
//

/**
 @file
 @publishedPartner
 @deprecated
*/
#ifndef SUPL_PUSH_H
#define SUPL_PUSH_H


//-------------------------------------------------------------------------------
#include <e32base.h>
#include <lbs/lbssuplpushcommon.h>

//-------------------------------------------------------------------------------
class CLbsSuplPushImpl;


//-------------------------------------------------------------------------------
/**
The MLbsSuplPushObserver class provides a call-back type of interface that must be
used together with the CLbsSuplPush class. A class using the CLbsSuplPush interface must 
derive from MLbsSuplPushObserver and implement appropriate virtual methods in order
to receive notifications about request completions.

@publishedPartner
@deprecated
@see CLbsSuplPush
*/
class MLbsSuplPushObserver
	{
public:
	IMPORT_C virtual TVersion Version() const;

	/**
	Receive notification that the SUPL Init message has been sent to the LBS sub-system.
	The call-back is invoked immediately after delivery of the SUPL INIT request and 
	does not provide any information about it's outcome, e.g. conflict control results,
	host validation results, connection results etc.
	
	@param aChannel  [In] The channel the call-back is related to.
	@param aReqId    [In] An Id of the request the call-back is related to.
	@param aError    [In] KErrNone if successful, KErrTimeout if it was not possible to deliver
	                      the request before the timeout period, KErrArgument if the structure 
	                      or content of the SUPL INIT message was incorrect. 
	                      Any system wide error code otherwise.
	@param aReserved [In] Reserved for future use.
	
	@see CLbsSuplPush::SuplInit
	*/
	virtual void OnSuplInitComplete(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, TInt aError, TInt aReserved) = 0;

protected:			
	/**
	Should not be used. Provides for future expansion of the observer interface. 
	Currently not implemented.
	*/  
	IMPORT_C virtual TAny* ExtendedInterface(TInt aFunctionNumber, TAny* aPtr1, TAny* aPtr2);
	};


//-------------------------------------------------------------------------------
/**
The CLbsSuplPush class provides an interface to send SUPL INIT Push messages into
the LBS sub-system. Normally SUPL INIT messages arrive through SMS or WAP push 
and, if accepted, result in the opening of a a TCP/IP connection to the SLP (SUPL Server).

Although it is possible for multiple threads to open a channel of the same type 
(e.g. SMS or WAP), only one thread can actively use the channel at any time. A system 
requiring concurrent access to a channel must provide its own access control mechanism.

Unless documented explicitly, no assumptions should be made about the order and/or 
content of notifications generated by the LBS sub-system through this or 
other interfaces.

@publishedPartner
@deprecated
@see MLbsSuplPushObserver
*/
NONSHARABLE_CLASS(CLbsSuplPush) : public CBase
	{
public:
	IMPORT_C static CLbsSuplPush* NewL(TLbsSuplPushChannel aChannel, MLbsSuplPushObserver& aObserver);
	virtual ~CLbsSuplPush();
	
public:
	IMPORT_C TInt SuplInit(TLbsSuplPushRequestId& aReqId, const TDesC8& aMsg, TInt aReserved);
	
private:
	CLbsSuplPush();
	void ConstructL(TLbsSuplPushChannel aChannel, MLbsSuplPushObserver& aObserver);
	
	//** Prohibit copy constructor */
	CLbsSuplPush(const CLbsSuplPush&);
	//** Prohibit assigment operator */
	CLbsSuplPush& operator= (const CLbsSuplPush&);

private:
	/** CLbsSuplPushImpl* Impl is the internal handle to the Implementation */
	CLbsSuplPushImpl* iImpl;
	};

#endif //SUPL_PUSH_H
