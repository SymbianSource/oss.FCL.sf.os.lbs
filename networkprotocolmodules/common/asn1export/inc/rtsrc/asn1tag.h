// Copyright (c) 2001-2009 Objective Systems, Inc. (http://www.obj-sys.com) 
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Eclipse Public License v1.0"
// which accompanies this distribution, and is available 
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Objective Systems, Inc - initial contribution.
//
// Contributors:
//
// Description:
// 

/** 
 * @file asn1tag.h 
 * Constants and structures used to define ASN.1 tag values.
 */

/**
@file
@internalTechnology
*/

#ifndef _ASN1TAG_H_
#define _ASN1TAG_H_

#include "rtxsrc/osSysTypes.h"

/* Set this #define to use an internal tag type of 32 bits instead      */
/* of 16 bits.  This will allow ID codes up to 2 ^ 29 (536870912)       */ 
/* instead of the default representation of 2 ^ 13 (8192).              */
#ifndef _16BIT
#define ASN1C_EXPANDED_TAGS
#endif

/* Tagging Value and Mask Constants */

/* tag class value constants */

#define TV_UNIV         0       /* universal                    */
#define TV_APPL         1       /* application-wide             */
#define TV_CTXT         2       /* context-specific             */
#define TV_PRIV         3       /* private-use                  */

/* tag form value constants */

#define TV_PRIM         0       /* primitive                    */
#define TV_CONS         1       /* constructor                  */

/* tag mask values - these can be logically or'd together to    */
/* form tags compatible with the ASNTAG structure..             */

#ifdef ASN1C_EXPANDED_TAGS

#define TM_UNIV         0x00000000  /* universal class          */
#define TM_APPL         0x40000000  /* application-wide class   */
#define TM_CTXT         0x80000000  /* context-specific class   */
#define TM_PRIV         0xC0000000  /* private-use class        */

#define TM_PRIM         0x00000000  /* primitive form           */
#define TM_CONS         0x20000000  /* constructor form         */
#define TM_IDCODE       0x1FFFFFFF  /* ID code mask             */

#define ASN_K_BADTAG    0xFFFFFFFF  /* invalid tag code         */
#define ASN_K_NOTAG     0xFFFFFFFF  /* no tag input parameter   */

#else

#define TM_UNIV         0x0000  /* universal class              */
#define TM_APPL         0x4000  /* application-wide class       */
#define TM_CTXT         0x8000  /* context-specific class       */
#define TM_PRIV         0xC000  /* private-use class            */

#define TM_PRIM         0x0000  /* primitive form               */
#define TM_CONS         0x2000  /* constructor form             */
#define TM_IDCODE       0x1FFF  /* ID code mask                 */
                                                                
#define ASN_K_BADTAG    0xFFFF  /* invalid tag code             */
#define ASN_K_NOTAG     0xFFFF  /* no tag input parameter       */

#endif

#define TM_CLASS        0xC0    /* class mask                   */
#define TM_FORM         0x20    /* form mask                    */
#define TM_CLASS_FORM   0xE0    /* class/form mask              */
#define TM_B_IDCODE     0x1F    /* id code mask (byte)          */

/* universal built-in type ID code value constants */

#define ASN_ID_EOC      0       /* end of contents              */
#define ASN_ID_BOOL     1       /* boolean                      */
#define ASN_ID_INT      2       /* integer                      */
#define ASN_ID_BITSTR   3       /* bit string                   */
#define ASN_ID_OCTSTR   4       /* byte (octet) string          */
#define ASN_ID_NULL     5       /* null                         */
#define ASN_ID_OBJID    6       /* object ID                    */
#define ASN_ID_OBJDSC   7       /* object descriptor            */
#define ASN_ID_EXTERN   8       /* external type                */
#define ASN_ID_REAL     9       /* real                         */
#define ASN_ID_ENUM     10      /* enumerated value             */
#define ASN_ID_EPDV     11      /* EmbeddedPDV type             */
#define ASN_ID_RELOID   13      /* relative object ID           */
#define ASN_ID_SEQ      16      /* sequence, sequence of        */
#define ASN_ID_SET      17      /* set, set of                  */

#define ASN_SEQ_TAG     0x30    /* SEQUENCE universal tag byte  */
#define ASN_SET_TAG     0x31    /* SET universal tag byte       */

/* Restricted character string type ID's */

#define ASN_ID_UTF8String       12
#define ASN_ID_NumericString    18
#define ASN_ID_PrintableString  19
#define ASN_ID_TeletexString    20
#define ASN_ID_T61String        ASN_ID_TeletexString
#define ASN_ID_VideotexString   21
#define ASN_ID_IA5String        22
#define ASN_ID_UTCTime          23
#define ASN_ID_GeneralTime      24
#define ASN_ID_GraphicString    25
#define ASN_ID_VisibleString    26
#define ASN_ID_GeneralString    27
#define ASN_ID_UniversalString  28
#define ASN_ID_BMPString        30

#ifdef ASN1C_EXPANDED_TAGS
typedef OSUINT32        ASN1TAG;
#define ASN1TAG_LSHIFT  24
#else
typedef OSUINT16        ASN1TAG;
#define ASN1TAG_LSHIFT  8
#endif

typedef enum { ASN1IMPL, ASN1EXPL } ASN1TagType;

#endif
