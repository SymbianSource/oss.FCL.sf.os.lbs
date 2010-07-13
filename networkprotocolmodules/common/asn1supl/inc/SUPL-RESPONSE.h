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
#ifndef SUPL_RESPONSE_H
#define SUPL_RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include "rtkey.h"
#include "rtpersrc/asn1PerCppTypes.h"
#include "ASN1CBitStr.h"
#include "ULP-Components.h"

/**
 * Header file for ASN.1 module SUPL-RESPONSE
 */
/**************************************************************/
/*                                                            */
/*  SETAuthKey_shortKey                                       */
/*                                                            */
/**************************************************************/

#define TV_SETAuthKey_shortKey	(TM_UNIV|TM_PRIM|3)

struct ASN1T_SETAuthKey_shortKey {
   OSUINT32  numbits;
   OSOCTET data[16];
   // ctors
   ASN1T_SETAuthKey_shortKey () : numbits(0) {}
   ASN1T_SETAuthKey_shortKey (OSUINT32 _numbits, const OSOCTET* _data) :
      numbits(_numbits) {
      OSCRTLMEMCPY (data, _data, sizeof(data));
   }
} ;

IMPORT_C int asn1PE_SETAuthKey_shortKey (OSCTXT* pctxt, ASN1T_SETAuthKey_shortKey* pvalue);

IMPORT_C int asn1PD_SETAuthKey_shortKey (OSCTXT* pctxt, ASN1T_SETAuthKey_shortKey* pvalue);

IMPORT_C void asn1Print_SETAuthKey_shortKey
   (const char* name, ASN1T_SETAuthKey_shortKey* pvalue);

/**************************************************************/
/*                                                            */
/*  SETAuthKey_longKey                                        */
/*                                                            */
/**************************************************************/

#define TV_SETAuthKey_longKey	(TM_UNIV|TM_PRIM|3)

struct ASN1T_SETAuthKey_longKey {
   OSUINT32  numbits;
   OSOCTET data[32];
   // ctors
   ASN1T_SETAuthKey_longKey () : numbits(0) {}
   ASN1T_SETAuthKey_longKey (OSUINT32 _numbits, const OSOCTET* _data) :
      numbits(_numbits) {
      OSCRTLMEMCPY (data, _data, sizeof(data));
   }
} ;

IMPORT_C int asn1PE_SETAuthKey_longKey (OSCTXT* pctxt, ASN1T_SETAuthKey_longKey* pvalue);

IMPORT_C int asn1PD_SETAuthKey_longKey (OSCTXT* pctxt, ASN1T_SETAuthKey_longKey* pvalue);

IMPORT_C void asn1Print_SETAuthKey_longKey
   (const char* name, ASN1T_SETAuthKey_longKey* pvalue);

/**************************************************************/
/*                                                            */
/*  SETAuthKey                                                */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_SETAuthKey_shortKey           1
#define T_SETAuthKey_longKey            2
#define T_SETAuthKey_extElem1           3

struct ASN1T_SETAuthKey {
   int t;
   union {
      /* t = 1 */
      ASN1T_SETAuthKey_shortKey *shortKey;
      /* t = 2 */
      ASN1T_SETAuthKey_longKey *longKey;
      /* t = 3 */
   } u;
} ;

IMPORT_C int asn1PE_SETAuthKey (OSCTXT* pctxt, ASN1T_SETAuthKey* pvalue);

IMPORT_C int asn1PD_SETAuthKey (OSCTXT* pctxt, ASN1T_SETAuthKey* pvalue);

IMPORT_C void asn1Print_SETAuthKey
   (const char* name, ASN1T_SETAuthKey* pvalue);

IMPORT_C void asn1Free_SETAuthKey (OSCTXT *pctxt, ASN1T_SETAuthKey* pvalue);

/**************************************************************/
/*                                                            */
/*  KeyIdentity4                                              */
/*                                                            */
/**************************************************************/

#define TV_KeyIdentity4	(TM_UNIV|TM_PRIM|3)

struct ASN1T_KeyIdentity4 {
   OSUINT32  numbits;
   OSOCTET data[16];
   // ctors
   ASN1T_KeyIdentity4 () : numbits(0) {}
   ASN1T_KeyIdentity4 (OSUINT32 _numbits, const OSOCTET* _data) :
      numbits(_numbits) {
      OSCRTLMEMCPY (data, _data, sizeof(data));
   }
} ;

IMPORT_C int asn1PE_KeyIdentity4 (OSCTXT* pctxt, ASN1T_KeyIdentity4* pvalue);

IMPORT_C int asn1PD_KeyIdentity4 (OSCTXT* pctxt, ASN1T_KeyIdentity4* pvalue);

IMPORT_C void asn1Print_KeyIdentity4
   (const char* name, ASN1T_KeyIdentity4* pvalue);

/**************************************************************/
/*                                                            */
/*  SUPLRESPONSE                                              */
/*                                                            */
/**************************************************************/

#define TV_SUPLRESPONSE	(TM_UNIV|TM_CONS|16)

struct ASN1T_SUPLRESPONSE {
   struct {
      unsigned sLPAddressPresent : 1;
      unsigned sETAuthKeyPresent : 1;
      unsigned keyIdentity4Present : 1;
   } m;
   ASN1T_PosMethod posMethod;
   ASN1T_SLPAddress sLPAddress;
   ASN1T_SETAuthKey sETAuthKey;
   ASN1T_KeyIdentity4 keyIdentity4;
   IMPORT_C ASN1T_SUPLRESPONSE ();
} ;

IMPORT_C int asn1PE_SUPLRESPONSE (OSCTXT* pctxt, ASN1T_SUPLRESPONSE* pvalue);

IMPORT_C int asn1PD_SUPLRESPONSE (OSCTXT* pctxt, ASN1T_SUPLRESPONSE* pvalue);

IMPORT_C void asn1Print_SUPLRESPONSE
   (const char* name, ASN1T_SUPLRESPONSE* pvalue);

IMPORT_C void asn1Free_SUPLRESPONSE (OSCTXT *pctxt, ASN1T_SUPLRESPONSE* pvalue);

#endif