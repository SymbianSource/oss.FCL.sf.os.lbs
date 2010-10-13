/**
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file sockethandler.h
 @internalTechnology
*/
#ifndef SOCKETHANDLER_H_
#define SOCKETHANDLER_H_

#include <es_sock.h>
#include <in_sock.h>

class MSocketHandler
	{
public:
	enum TWhat
		{
		EWhatConnection,
		EWhatSocket
		};

	virtual void Connected(TInt aWhat) = 0;
	virtual void MessageRecieved(TDesC8& aBuffer) = 0;
	};

class CSocketListener : public CActive
	{
public:
	static CSocketListener* NewL(RSocket& aSocket, MSocketHandler& aObserver);
	~CSocketListener();
private:
	// Constructors
	void ConstructL();
	CSocketListener(RSocket& aSocket, MSocketHandler& aObserver);
	
	// CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
private:
	// Socket
	RSocket iSocket;
	
	// Recv buffer
	RBuf8 iRxBuffer;
	TSockXfrLength iRxLength;
	
	// Observer
	MSocketHandler& iObserver;
	};

class CSocketHandler : public CActive
	{
public:
	// Constructors
	static CSocketHandler* NewL(MSocketHandler& aObserver);
	~CSocketHandler();
	
	// CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
	// Methods
	void StartL();
	void SendMessageL(TDesC8& aBuffer);
protected:
private:
	// Constructors
	CSocketHandler(MSocketHandler& aObserver);
	void ConstructL();
	
private:
	enum TState
		{
			ENone,
			EConnectingConnection,
			EConnecting,
			EIdle,
			ESending,
			EListening
		};

	// Connection
	RConnection iConnection;
	
	// Socket Server
	RSocketServ iSocketServ;
	
	// Sockets
	RSocket iListen, iAccept;
	
	// Address
	TInetAddr iAddr;
		
	// Send data
	RBuf8 iTxBuffer;
	
	// Observer
	MSocketHandler& iObserver;
	
		// State
	TState iState;

	// Listener
	CSocketListener* iListener;
	};

#endif /*SOCKETHANDLER_H_*/
