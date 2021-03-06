/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef CLBSCONVERSIONPOSITIONERSUBSESSION_H_
#define CLBSCONVERSIONPOSITIONERSUBSESSION_H_


#include <e32property.h>
#include "tserverstartparams.h"
#include "csecureasbase.h"
#include "csecureserverbase.h"
#include "csecuresessionbase.h"
#include "csecuresessionsubsessionbase.h"
#include "csubsessionbase.h"
#include "messageutils.h"
#include "clbslocmonitorutils.h"

class CLbsLocMonitorServer;

/*
  Server side subsession of the Location Monitor server.
  This subsession handles client's requests for the
  location info conversion.
*/
class CLbsConversionPositionerSubsession : public CBase, 
                                           public MSubSessionImpl
   {
public:
   static CLbsConversionPositionerSubsession* NewL();
   
   ~CLbsConversionPositionerSubsession();
   
   // From MSubSessionImpl
   virtual void DispatchL(const RMessage2& aMessage);
   virtual void DispatchError(const RMessage2& aMessage, TInt aError);
   virtual void CreateSubSessionL(const RMessage2& aMessage, const CSecureServerBase* aServer);
   virtual void CloseSubSession();
   
   // via MSubSessionImpl::MRelease    
   void VirtualRelease(); 
     
protected:
   CLbsConversionPositionerSubsession();
   void ConstructL();

   //
   // action methods.
   //
   void GetPositionL(const RMessage2& aMessage);
   void CancelGetPositionL(const RMessage2& aMessage);
   void SetLastKnownPositionL(const RMessage2& aMessage);
    
private:

   CLbsLocMonitorServer* iLocMonitorServer;

   // Unique ID of this subsession
   CLbsLocMonitorUtils::sessionAndSubSessionId iSubsessionId;
   };

#endif /*CLBSCONVERSIONPOSITIONERSUBSESSION_H_*/
