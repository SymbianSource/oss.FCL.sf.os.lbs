// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file sockethandler.cpp
 @internalTechnology
*/
#include "sockethandler.h"

CSocketListener* CSocketListener::NewL(RSocket& aSocket, MSocketHandler& aObserver)
	{
	CSocketListener* self = new(ELeave) CSocketListener(aSocket, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	
	return self;
	}

void CSocketListener::ConstructL()
	{
	// Allocate buffers
	iRxBuffer.CreateL(8192);
	
	iSocket.RecvOneOrMore(iRxBuffer, 0, iStatus, iRxLength);
	SetActive();
	}

CSocketListener::CSocketListener(RSocket& aSocket, MSocketHandler& aObserver) :
	CActive(EPriorityStandard), iSocket(aSocket), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

CSocketListener::~CSocketListener()
	{
	Cancel();
	iRxBuffer.Close();
	}

void CSocketListener::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	TInt length = iRxLength();
	iObserver.MessageRecieved(iRxBuffer);
	
	iRxLength = 0;
	iRxBuffer.Zero();
	iSocket.RecvOneOrMore(iRxBuffer, 0, iStatus, iRxLength);
	SetActive();
	}

void CSocketListener::DoCancel()
	{
	iSocket.CancelAll();
	}

TInt CSocketListener::RunError(TInt aError)
	{
	ASSERT(EFalse);
	
	return aError;
	}



CSocketHandler* CSocketHandler::NewL(MSocketHandler& aObserver)
	{
	CSocketHandler* self = new(ELeave) CSocketHandler(aObserver);
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	
	return self;
	}

CSocketHandler::CSocketHandler(MSocketHandler& aObserver) :
	CActive(EPriorityStandard), iAddr(INET_ADDR(127,0,0,1), 7275),
	iObserver(aObserver), iState(ENone)
	{
	CActiveScheduler::Add(this);
	}

CSocketHandler::~CSocketHandler() 
	{
	iState = ENone;
	delete iListener;
	iConnection.Close();	
	iSocketServ.Close();
	iTxBuffer.Close();
	}

void CSocketHandler::ConstructL()
	{
	// Allocate buffers
	iTxBuffer.CreateL(8192);
	}

void CSocketHandler::StartL()
	{
	if (iState == ENone)
		{
		// Connect the socket server
		User::LeaveIfError(iSocketServ.Connect());
		
		// Connect the connection
		User::LeaveIfError(iConnection.Open(iSocketServ));
		iConnection.Start(iStatus);
		
		iState = EConnectingConnection;
		SetActive();
		}
	else
		{
		iObserver.Connected(MSocketHandler::EWhatConnection);
		}
	}

void CSocketHandler::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	switch (iState)
		{
		case EConnectingConnection:
			{
			// Connect the socket
			User::LeaveIfError(iListen.Open(iSocketServ, KAfInet, KSockStream, KProtocolInetTcp, iConnection));
			User::LeaveIfError(iAccept.Open(iSocketServ));
			User::LeaveIfError(iListen.Bind(iAddr));
			User::LeaveIfError(iListen.Listen(1));
			iListen.Accept(iAccept, iStatus);
			iState = EListening;
			
			iObserver.Connected(MSocketHandler::EWhatConnection);
			
			SetActive();
			break;
			}
		case EListening:
			{
			iListener = CSocketListener::NewL(iAccept, iObserver);
			
			iObserver.Connected(MSocketHandler::EWhatSocket);
			
			iState = EIdle;
			break;
			}
		case ESending:
			{
			iState = EIdle;
			break;
			}
		}
	}

void CSocketHandler::DoCancel()
	{
	}

TInt CSocketHandler::RunError(TInt aError)
	{
	ASSERT(ETrue);
	
	return aError;
	}

void CSocketHandler::SendMessageL(TDesC8& aBuffer)
	{
	iTxBuffer.Copy(aBuffer);
	
	iAccept.Send(iTxBuffer, 0, iStatus);
	iState = ESending;
	SetActive();
	}
