// Copyright (c) 2001-2009 Objective Systems, Inc. (http://www.obj-sys.com) 
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available 
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//

/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).
 */
#ifndef SUPL_INIT_H
#define SUPL_INIT_H

#include <stdio.h>
#include <stdlib.h>
#include "rtkey.h"
#include "rtpersrc/asn1PerCppTypes.h"
#include "ASN1CBitStr.h"
#include "ULP-Components.h"

/**
 * Header file for ASN.1 module SUPL-INIT
 */
/**************************************************************/
/*                                                            */
/*  NotificationType                                          */
/*                                                            */
/**************************************************************/

#define TV_NotificationType	(TM_UNIV|TM_PRIM|10)

struct NotificationType {
   enum Root {
      noNotificationNoVerification = 0,
      notificationOnly = 1,
      notificationAndVerficationAllowedNA = 2,
      notificationAndVerficationDeniedNA = 3,
      privacyOverride = 4
   } ;
} ;

typedef OSUINT32 ASN1T_NotificationType;

IMPORT_C int asn1PE_NotificationType (OSCTXT* pctxt, ASN1T_NotificationType value);

IMPORT_C int asn1PD_NotificationType (OSCTXT* pctxt, ASN1T_NotificationType* pvalue);

IMPORT_C void asn1Print_NotificationType
   (const char* name, ASN1T_NotificationType* pvalue);

IMPORT_C const OSUTF8CHAR* ASN1T_NotificationType_ToString (OSINT32 value);

IMPORT_C int ASN1T_NotificationType_ToEnum (OSCTXT* pctxt,
   const OSUTF8CHAR* value, ASN1T_NotificationType* pvalue);

/**************************************************************/
/*                                                            */
/*  EncodingType                                              */
/*                                                            */
/**************************************************************/

#define TV_EncodingType	(TM_UNIV|TM_PRIM|10)

struct EncodingType {
   enum Root {
      ucs2 = 0,
      gsmDefault = 1,
      utf8 = 2
   } ;
} ;

typedef OSUINT32 ASN1T_EncodingType;

IMPORT_C int asn1PE_EncodingType (OSCTXT* pctxt, ASN1T_EncodingType value);

IMPORT_C int asn1PD_EncodingType (OSCTXT* pctxt, ASN1T_EncodingType* pvalue);

IMPORT_C void asn1Print_EncodingType
   (const char* name, ASN1T_EncodingType* pvalue);

IMPORT_C const OSUTF8CHAR* ASN1T_EncodingType_ToString (OSINT32 value);

IMPORT_C int ASN1T_EncodingType_ToEnum (OSCTXT* pctxt,
   const OSUTF8CHAR* value, ASN1T_EncodingType* pvalue);

/**************************************************************/
/*                                                            */
/*  Notification_requestorId                                  */
/*                                                            */
/**************************************************************/

#define TV_Notification_requestorId	(TM_UNIV|TM_PRIM|4)

struct ASN1T_Notification_requestorId {
   OSUINT32 numocts;
   OSOCTET data[50];
   // ctors
   ASN1T_Notification_requestorId () : numocts(0) {}
   ASN1T_Notification_requestorId (OSUINT32 _numocts, const OSOCTET* _data) :
      numocts (_numocts) {
      OSCRTLMEMCPY (data, _data, OSRTMIN (numocts, sizeof(data)));
   }
   ASN1T_Notification_requestorId (const char* cstring) {
      if (strlen(cstring) > 0) {
         numocts = OSRTMIN ((OSCRTLSTRLEN(cstring)+1), sizeof(data));
         OSCRTLSTRNCPY ((char*)data, cstring, sizeof(data));
      }
      else numocts = 0;
   }
   // assignment operators
   ASN1T_Notification_requestorId& operator= (const char* cstring) {
      if (strlen(cstring) > 0) {
         numocts = OSRTMIN ((OSCRTLSTRLEN(cstring)+1), sizeof(data));
         OSCRTLSTRNCPY ((char*)data, cstring, sizeof(data));
      }
      else numocts = 0;
      return *this;
   }
} ;

IMPORT_C int asn1PE_Notification_requestorId (OSCTXT* pctxt, ASN1T_Notification_requestorId* pvalue);

IMPORT_C int asn1PD_Notification_requestorId (OSCTXT* pctxt, ASN1T_Notification_requestorId* pvalue);

IMPORT_C void asn1Print_Notification_requestorId
   (const char* name, ASN1T_Notification_requestorId* pvalue);

/**************************************************************/
/*                                                            */
/*  FormatIndicator                                           */
/*                                                            */
/**************************************************************/

#define TV_FormatIndicator	(TM_UNIV|TM_PRIM|10)

struct FormatIndicator {
   enum Root {
      logicalName = 0,
      e_mailAddress = 1,
      msisdn = 2,
      url = 3,
      sipUrl = 4,
      min = 5,
      mdn = 6
   } ;
} ;

typedef OSUINT32 ASN1T_FormatIndicator;

IMPORT_C int asn1PE_FormatIndicator (OSCTXT* pctxt, ASN1T_FormatIndicator value);

IMPORT_C int asn1PD_FormatIndicator (OSCTXT* pctxt, ASN1T_FormatIndicator* pvalue);

IMPORT_C void asn1Print_FormatIndicator
   (const char* name, ASN1T_FormatIndicator* pvalue);

IMPORT_C const OSUTF8CHAR* ASN1T_FormatIndicator_ToString (OSINT32 value);

IMPORT_C int ASN1T_FormatIndicator_ToEnum (OSCTXT* pctxt,
   const OSUTF8CHAR* value, ASN1T_FormatIndicator* pvalue);

/**************************************************************/
/*                                                            */
/*  Notification_clientName                                   */
/*                                                            */
/**************************************************************/

#define TV_Notification_clientName	(TM_UNIV|TM_PRIM|4)

struct ASN1T_Notification_clientName {
   OSUINT32 numocts;
   OSOCTET data[50];
   // ctors
   ASN1T_Notification_clientName () : numocts(0) {}
   ASN1T_Notification_clientName (OSUINT32 _numocts, const OSOCTET* _data) :
      numocts (_numocts) {
      OSCRTLMEMCPY (data, _data, OSRTMIN (numocts, sizeof(data)));
   }
   ASN1T_Notification_clientName (const char* cstring) {
      if (strlen(cstring) > 0) {
         numocts = OSRTMIN ((OSCRTLSTRLEN(cstring)+1), sizeof(data));
         OSCRTLSTRNCPY ((char*)data, cstring, sizeof(data));
      }
      else numocts = 0;
   }
   // assignment operators
   ASN1T_Notification_clientName& operator= (const char* cstring) {
      if (strlen(cstring) > 0) {
         numocts = OSRTMIN ((OSCRTLSTRLEN(cstring)+1), sizeof(data));
         OSCRTLSTRNCPY ((char*)data, cstring, sizeof(data));
      }
      else numocts = 0;
      return *this;
   }
} ;

IMPORT_C int asn1PE_Notification_clientName (OSCTXT* pctxt, ASN1T_Notification_clientName* pvalue);

IMPORT_C int asn1PD_Notification_clientName (OSCTXT* pctxt, ASN1T_Notification_clientName* pvalue);

IMPORT_C void asn1Print_Notification_clientName
   (const char* name, ASN1T_Notification_clientName* pvalue);

/**************************************************************/
/*                                                            */
/*  Notification                                              */
/*                                                            */
/**************************************************************/

#define TV_Notification	(TM_UNIV|TM_CONS|16)

struct ASN1T_Notification {
   struct {
      unsigned encodingTypePresent : 1;
      unsigned requestorIdPresent : 1;
      unsigned requestorIdTypePresent : 1;
      unsigned clientNamePresent : 1;
      unsigned clientNameTypePresent : 1;
   } m;
   ASN1T_NotificationType notificationType;
   ASN1T_EncodingType encodingType;
   ASN1T_Notification_requestorId requestorId;
   ASN1T_FormatIndicator requestorIdType;
   ASN1T_Notification_clientName clientName;
   ASN1T_FormatIndicator clientNameType;
   IMPORT_C ASN1T_Notification ();
} ;

IMPORT_C int asn1PE_Notification (OSCTXT* pctxt, ASN1T_Notification* pvalue);

IMPORT_C int asn1PD_Notification (OSCTXT* pctxt, ASN1T_Notification* pvalue);

IMPORT_C void asn1Print_Notification
   (const char* name, ASN1T_Notification* pvalue);

/**************************************************************/
/*                                                            */
/*  SLPMode                                                   */
/*                                                            */
/**************************************************************/

#define TV_SLPMode	(TM_UNIV|TM_PRIM|10)

struct SLPMode {
   enum Root {
      proxy = 0,
      nonProxy = 1
   } ;
} ;

typedef OSUINT32 ASN1T_SLPMode;

IMPORT_C int asn1PE_SLPMode (OSCTXT* pctxt, ASN1T_SLPMode value);

IMPORT_C int asn1PD_SLPMode (OSCTXT* pctxt, ASN1T_SLPMode* pvalue);

IMPORT_C void asn1Print_SLPMode
   (const char* name, ASN1T_SLPMode* pvalue);

IMPORT_C const OSUTF8CHAR* ASN1T_SLPMode_ToString (OSINT32 value);

IMPORT_C int ASN1T_SLPMode_ToEnum (OSCTXT* pctxt,
   const OSUTF8CHAR* value, ASN1T_SLPMode* pvalue);

/**************************************************************/
/*                                                            */
/*  MAC                                                       */
/*                                                            */
/**************************************************************/

#define TV_MAC	(TM_UNIV|TM_PRIM|3)

struct ASN1T_MAC {
   OSUINT32  numbits;
   OSOCTET data[8];
   // ctors
   ASN1T_MAC () : numbits(0) {}
   ASN1T_MAC (OSUINT32 _numbits, const OSOCTET* _data) :
      numbits(_numbits) {
      OSCRTLMEMCPY (data, _data, sizeof(data));
   }
} ;

IMPORT_C int asn1PE_MAC (OSCTXT* pctxt, ASN1T_MAC* pvalue);

IMPORT_C int asn1PD_MAC (OSCTXT* pctxt, ASN1T_MAC* pvalue);

IMPORT_C void asn1Print_MAC
   (const char* name, ASN1T_MAC* pvalue);

/**************************************************************/
/*                                                            */
/*  KeyIdentity                                               */
/*                                                            */
/**************************************************************/

#define TV_KeyIdentity	(TM_UNIV|TM_PRIM|3)

struct ASN1T_KeyIdentity {
   OSUINT32  numbits;
   OSOCTET data[16];
   // ctors
   ASN1T_KeyIdentity () : numbits(0) {}
   ASN1T_KeyIdentity (OSUINT32 _numbits, const OSOCTET* _data) :
      numbits(_numbits) {
      OSCRTLMEMCPY (data, _data, sizeof(data));
   }
} ;

IMPORT_C int asn1PE_KeyIdentity (OSCTXT* pctxt, ASN1T_KeyIdentity* pvalue);

IMPORT_C int asn1PD_KeyIdentity (OSCTXT* pctxt, ASN1T_KeyIdentity* pvalue);

IMPORT_C void asn1Print_KeyIdentity
   (const char* name, ASN1T_KeyIdentity* pvalue);

/**************************************************************/
/*                                                            */
/*  SUPLINIT                                                  */
/*                                                            */
/**************************************************************/

#define TV_SUPLINIT	(TM_UNIV|TM_CONS|16)

struct ASN1T_SUPLINIT {
   struct {
      unsigned notificationPresent : 1;
      unsigned sLPAddressPresent : 1;
      unsigned qoPPresent : 1;
      unsigned mACPresent : 1;
      unsigned keyIdentityPresent : 1;
   } m;
   ASN1T_PosMethod posMethod;
   ASN1T_Notification notification;
   ASN1T_SLPAddress sLPAddress;
   ASN1T_QoP qoP;
   ASN1T_SLPMode sLPMode;
   ASN1T_MAC mAC;
   ASN1T_KeyIdentity keyIdentity;
   IMPORT_C ASN1T_SUPLINIT ();
} ;

IMPORT_C int asn1PE_SUPLINIT (OSCTXT* pctxt, ASN1T_SUPLINIT* pvalue);

IMPORT_C int asn1PD_SUPLINIT (OSCTXT* pctxt, ASN1T_SUPLINIT* pvalue);

IMPORT_C void asn1Print_SUPLINIT
   (const char* name, ASN1T_SUPLINIT* pvalue);

IMPORT_C void asn1Free_SUPLINIT (OSCTXT *pctxt, ASN1T_SUPLINIT* pvalue);

#endif