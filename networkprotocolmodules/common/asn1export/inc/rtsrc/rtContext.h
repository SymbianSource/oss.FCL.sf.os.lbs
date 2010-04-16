// Copyright (c) 2001-2009 Objective Systems, Inc. (http://www.obj-sys.com) 
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
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
 * @file rtContext.h
 * ASN.1 run-time context definitions.
 */

/**
@file
@internalTechnology
*/

#ifndef _RTCONTEXT_H_
#define _RTCONTEXT_H_

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxSList.h"
#include "rtxsrc/rtxStack.h"
#include "rtsrc/rtExternDefs.h"

/**
 * @addtogroup rtxCtxt
 * @{
 */
/* ASN.1 size constraint structure */

typedef struct _Asn1SizeCnst {
   OSBOOL       extended;
   OSUINT32     lower;
   OSUINT32     upper;
   struct _Asn1SizeCnst* next;
} Asn1SizeCnst;

/* Flag mask constant values */

#define ASN1DYNCTXT     0x8000  /* set if context is dynamic            */
#define ASN1INDEFLEN    0x4000  /* set if message is indefinite length  */
#define ASN1TRACE       0x2000  /* enable PER bit tracing               */
#define ASN1LASTEOC     0x1000  /* indicates last parsed item was EOC   */
#define ASN1FASTCOPY    0x0800  /* turns on the "fast copy" mode        */
#define ASN1CONSTAG     0x0400  /* form of last parsed tag              */
#define ASN1CANXER      0x0200  /* canonical XER                        */
#define ASN1OPENTYPE    0x0080  /* item is an open type field           */
#define ASN1BOOLTRUE1   0x0040  /* boolean true in BER is 01, not FF    */

/* ASN.1 encode/decode context block structure */

typedef struct {
   OSFreeCtxtAppInfoPtr  pFreeFunc; /* Free function pointer            */
   OSResetCtxtAppInfoPtr pResetFunc;/* Reset function pointer           */
   OSRTSList    fieldList;      /* PER field list                       */
   Asn1SizeCnst* pSizeConstraint;  /* Size constraint list              */
   const char*  pCharSet;       /* String of permitted characters       */
   OSRTStack    nameStack;      /* Element name stack                   */
   OSRTSList    evtHndlrList;   /* Event handler object list            */
   void*        errHndlrCB;     /* Error handler callback function      */
} OSASN1CtxtInfo;

#define ACINFO(pctxt)           ((OSASN1CtxtInfo*)(pctxt)->pASN1Info)
#define ASN1BUFCUR(cp)          (cp)->buffer.data[(cp)->buffer.byteIndex]
#define ASN1BUFPTR(cp)          &(cp)->buffer.data[(cp)->buffer.byteIndex]
#define ASN1BUF_INDEFLEN(cp)    (((cp)->flags&ASN1INDEFLEN)!=0)
#define ASN1BUF_PTR(cp)         ASN1BUFPTR(cp)

#ifdef __cplusplus
extern "C" {
#endif
/**
 * This function initializes a context using a run-time key.  This form 
 * is required for evaluation and limited distribution software.  The 
 * compiler will generate a macro for rtInitContext in the rtkey.h 
 * file that will invoke this function with the generated run-time key.
 *
 * @param pctxt        The pointer to the context structure variable to be
 *                       initialized.
 * @param key          Key data generated by ASN1C compiler.
 * @param keylen       Key data field length.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtInitContextUsingKey 
(OSCTXT* pctxt, const OSOCTET* key, size_t keylen);

/**
 * This function initializes an OSCTXT block. It makes sure that if the block
 * was not previously initialized, that all key working parameters are set to
 * their correct initial state values (i.e. declared within a function as a
 * normal working variable).  A user must call this function or 
 * rtNewContext before making any other run-time library calls.
 *
 * @param pctxt  Pointer to context structure variable to be initialized.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
#ifndef rtInitContext
EXTERNRT int rtInitContext (OSCTXT* pctxt);
#endif

/**
 * This function initializes an OSCTXT block using an existing context 
 * as a basis.  This provides improved performance over initializing a 
 * context from scratch because the overhead of creating a memory heap 
 * and doing license checking is eliminated.  The new context shares 
 * the memory heap of the existing context.
 *
 * @param pctxt  Pointer to context structure variable to be initialized.
 * @param psrc   Source context to use fo initialization.
 * @return       Completion status of operation:
 *                 - 0 (ASN_OK) = success,
 *                 - negative return value is error.
 */
EXTERNRT int rtInitSubContext (OSCTXT* pctxt, OSCTXT* psrc);

/**
 * This function allocates a new OSCTXT block and initializes it. Although
 * the block is allocated from the standard heap, it should not be freed using
 * free. The rtFreeContext function should be used because this frees items
 * allocated within the block before freeing the block itself.
 *
 * This is the preferred way of setting up a new encode or decode context
 * because it ensures the block is properly initialized before using it. If a
 * context variable is declared on the stack, the user must first remember to
 * invoke the rtInitContext function on it. This can be a source of errors.
 * This function can be called directly when setting up BER context or it will
 * be invoked from within the pu_newContext call for PER.
 *
 * @return             Newly allocated context if successful, 
 *                       NULL if failure.
 */
#ifndef rtNewContext
EXTERNRT OSCTXT* rtNewContext (void);
#endif

/**
 * This function allocates a new OSCTXT block and initializes it. Although
 * the block is allocated from the standard heap, it should not be freed using
 * free. The rtFreeContext function should be used because this frees items
 * allocated within the block before freeing the block itself.
 *
 * This is the preferred way of setting up a new encode or decode context
 * because it ensures the block is properly initialized before using it. If a
 * context variable is declared on the stack, the user must first remember to
 * invoke the rtInitContext function on it. This can be a source of errors.
 * This function can be called directly when setting up BER context or it will
 * be invoked from within the pu_newContext call for PER.
 *
 * @param key          Key data generated by ASN1C compiler.
 * @param keylen       Key data field length.
 * @return             Newly allocated context if successful, 
 *                       NULL if failure.
 */
EXTERNRT OSCTXT* rtNewContextUsingKey (const OSOCTET* key, size_t keylen);

/**
 * This function frees all dynamic memory associated with a context. This
 * includes all memory inside the block (in particular, the list of memory
 * blocks used by the rtMem functions) as well as the block itself if allocated
 * with the rtNewContext function
 *
 * @param pctxt        A pointer to a context structure.
 */
EXTERNRT void rtFreeContext (OSCTXT* pctxt);

/**
 * This function is deprecated, use ::rtSetFastCopy.
 */ 
EXTERNRT void rtSetCopyValues (OSCTXT* pctxt, OSBOOL value);

/**
 * This function sets the ASN1FASTCOPY flag in the specified context. This flag
 * has effect only if decoding is being performed. If this flag is set then
 * some decoded data (BIT STRINGs, OCTET STRINGs or OPEN TYPEs) will not
 * be copied.  Instead, a pointer to the location of the data in the decode 
 * buffer will be stored in the generated structure.  This may improve 
 * decoding performance, but should be used carefully. If the decode 
 * buffer is destroyed, all uncopied data will be lost. Use only if you 
 * are sure the decode buffer will be available when data in the decoded 
 * structure is processed. By default this flag is not set.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Boolean value of the flag to set.
 */
EXTERNRT void rtSetFastCopy (OSCTXT* pctxt, OSBOOL value);

/** 
 * @} rtxCtxt
 */

/* Internal function defs */
void rtErrASN1Init ();
EXTERNRT int rtCtxtInitASN1Info (OSCTXT* pctxt);
#ifdef __cplusplus
}
#endif

#endif
