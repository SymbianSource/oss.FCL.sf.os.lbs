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
 * @file asn1CEvtHndlr.h 
 * C event handler structure. The ASN1CEventHandler type is a structured type  
 * that can be used to define event handlers by the user.
 */
/**
 * @defgroup Asn1CEventHandler Asn1CEventHandler 
 * Asn1CEventHandler is a structure typed used for user-defined event handlers.
 * @ingroup cppruntime
 * @{
 */

/**
@file
@internalTechnology
*/

#ifndef _ASN1CEVTHNDLR_H_
#define _ASN1CEVTHNDLR_H_

#include <stdio.h>
#include "rtsrc/asn1type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This is a function pointer for a callback function which is invoked 
 * from within a decode function when an element of a SEQUENCE, SET, 
 * SEQUENCE OF, SET OF, or CHOICE construct is parsed.
 *
 * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
 *                       element as defined in the ASN.1 defination. For
 *                       SEQUENCE OF or SET OF, this is set to the name
 *                       "element".
 * @param index        For SEQUENCE, SET, or CHOICE, this is not used and is
 *                       set to the value
 *                       -1. For SEQUENCE OF or SET OF, this contains the
 *                       zero-based index of the element in the conceptual
 *                       array associated with the construct.
 * @return             - none
 */
typedef void (*rtxStartElement) (const char* name, int index) ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when parsing is complete on an element of a 
 * SEQUENCE, SET, SEQUENCE OF, SET OF, or CHOICE construct.
 *
 * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
 *                       element as defined in the ASN.1 defination. For
 *                       SEQUENCE OF or SET OF, this is set to the name
 *                       "element".
 * @param index        For SEQUENCE, SET, or CHOICE, this is not used and is
 *                       set to the value
 *                       -1. For SEQUENCE OF or SET OF, this contains the
 *                       zero-based index of the element in the conceptual
 *                       array associated with the construct.
 * @return             - none
 */
typedef void (*rtxEndElement) (const char* name, int index) ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of the BOOLEAN ASN.1 type is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxBoolValue) (OSBOOL value);

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of the INTERGER ASN.1 type is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxIntValue) (OSINT32 value);

/**
 * This is a function pointer for a callback function which is invoked 
 * from within a decode function when a value of the INTEGER ASN.1 type 
 * is parsed. In this case, constraints on the integer value forced the 
 * use of unsigned integer C type to represent the value.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxUIntValue) (OSUINT32 value);

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of the 64-bit INTERGER ASN.1 type 
 * is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxInt64Value) (OSINT64 value);

/**
 * This is a function pointer for a callback function which is invoked 
 * from within a decode function when a value of the 64-bit INTEGER ASN.1 type 
 * is parsed. 
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxUInt64Value) (OSUINT64 value);

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of the BIT STRING ASN.1 type is 
 * parsed.
 *
 * @param numbits      - Number of bits in the parsed value. 
 * @param data         - Pointer to a byte array that contains the bit 
 *                         string data.
 * @return             - none
 */ 
typedef void (*rtxBitStrValue) (OSUINT32 numbits, const OSOCTET* data);

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of one of the OCTET STRING ASN.1 type
 * is parsed.
 *
 * @param numocts      Number of octets in the parsed value.
 * @param data         Pointer to byte array containing the octet string
 *                       data.
 * @return             - none
 */
typedef void (*rtxOctStrValue) (OSUINT32 numocts, const OSOCTET* data) ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of one of the 8-bit ASN.1 character 
 * string types is parsed.
 *
 * @param value        Null terminated character string value.
 * @return             - none
 */
typedef void (*rtxCharStrValue) (const char* value) ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of one of the 16-bit ASN.1 character 
 * string types is parsed.
 *
 * This is used for the ASN.1 BmpString type.
 *
 * @param nchars       Number of characters in the parsed value.
 * @param data         Pointer to an array containing 16-bit values.
 *                       These are represented using unsigned short integer
 *                       values.
 * @return             - none
 */
typedef void (*rtxCharStrValue16Bit) (OSUINT32 nchars, OSUNICHAR* data) ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of one of the 32-bit ASN.1 characer 
 * string types is parsed.
 *
 * This is used for the ASN.1 UniversalString type.
 *
 * @param nchars       Number of characters in the parsed value.
 * @param data         Pointer to an array containing 32-bit values.
 *                       Each 32-bit integer value is a universal character.
 * @return             - none
 */
typedef void (*rtxCharStrValue32Bit) (OSUINT32 nchars, OS32BITCHAR* data) ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of the NULL ASN.1 type is parsed.
 *
 * @param             - none
 * @return             - none
 */
typedef void (*rtxNullValue) () ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function whn a value the OBJECT IDENTIFIER ASN.1 type is 
 * parsed.
 *
 * @param numSubIds    Number of subidentifiers in the object identifier.
 * @param pSubIds      Pointer to array containing the subidentifier values.
 * @return             -none
 */
typedef void (*rtxOidValue) (OSUINT32 numSubIds, OSUINT32* pSubIds) ;

 /**
  * This is a function pointer for a callback function which is invoked from 
  * within a decode function when a value the REAL ASN.1 type is parsed.
  *
  * @param value        Parsed value.
  * @return             - none
  */
typedef void (*rtxRealValue) (double value) ;

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when a value of the ENUMERATED ASN.1 type is 
 * parsed.
 *
 * @param value        - Parsed enumerated value
 * @param text         - Textual value of the enumerated identifier
 * @return             - none
 */
typedef void (*rtxEnumValue) (OSUINT32 value, const OSUTF8CHAR* text);

/**
 * This is a function pointer for a callback function which is invoked from 
 * within a decode function when an ASN.1 open type is parsed.
 *
 * @param numocts      Number of octets in the parsed value.
 * @param data         Pointer to byet array contain in tencoded ASN.1
 *                       value.
 * @return             - none
 */
typedef void (*rtxOpenTypeValue) (OSUINT32 numocts, const OSOCTET* data) ;

/**
 * This is a basic C based event handler structure, which can be used
 * to define user-defined event handlers.
 */
typedef struct Asn1NamedCEventHandler {
   rtxStartElement      startElement;
   rtxEndElement        endElement;
   rtxBoolValue         boolValue;
   rtxIntValue          intValue;
   rtxUIntValue         uIntValue;
   rtxInt64Value        int64Value;
   rtxUInt64Value       uInt64Value;
   rtxBitStrValue       bitStrValue;
   rtxOctStrValue       octStrValue;
   rtxCharStrValue      charStrValue;
   rtxCharStrValue16Bit charStrValue16Bit;
   rtxCharStrValue32Bit charStrValue32Bit;
   rtxNullValue         nullValue;
   rtxOidValue          oidValue;
   rtxRealValue         realValue;
   rtxEnumValue         enumValue;
   rtxOpenTypeValue     openTypeValue;
} Asn1NamedCEventHandler;

/**
 * This function is called to add a new event handler to the context event 
 * handler list.
 *
 * @param pCtxt       Context to which event handler has to be added.
 * @param pHandler    Pointer to the event handler structure.
 * @return            none
 */

EXTERNRT void rtAddEventHandler 
(OSCTXT* pCtxt, Asn1NamedCEventHandler* pHandler);

/**
 * This function is called to remove an event handler from the context event 
 * handler list.  Note that it does not delete the event handler object.
 *
 * @param pCtxt       Context from which event handler has to be removed.
 * @param pHandler    Pointer to event handler structure.
 * @return            none
 */
EXTERNRT void rtRemoveEventHandler 
(OSCTXT* pCtxt, Asn1NamedCEventHandler* pHandler);

/**
 * The following functions are invoked from within the generated 
 * code to call the various user-defined event handler methods ..
 */
EXTERNRT void rtInvokeStartElement 
(OSCTXT* pCtxt, const char* name, int index);

EXTERNRT void rtInvokeEndElement 
(OSCTXT* pCtxt, const char* name, int index);

EXTERNRT void rtInvokeBoolValue (OSCTXT* pCtxt, OSBOOL value);

EXTERNRT void rtInvokeIntValue (OSCTXT* pCtxt, OSINT32 value);

EXTERNRT void rtInvokeUIntValue (OSCTXT* pCtxt, OSUINT32 value);

EXTERNRT void rtInvokeInt64Value (OSCTXT* pCtxt, OSINT64 value);

EXTERNRT void rtInvokeUInt64Value (OSCTXT* pCtxt, OSUINT64 value);

EXTERNRT void rtInvokeBitStrValue 
(OSCTXT* pCtxt, OSUINT32 numbits, const OSOCTET* data);

EXTERNRT void rtInvokeOctStrValue 
(OSCTXT* pCtxt, OSUINT32 numocts, const OSOCTET* data);

EXTERNRT void rtInvokeCharStrValue (OSCTXT* pCtxt, const char* value);

EXTERNRT void rtInvokeCharStr16BitValue 
(OSCTXT* pCtxt, OSUINT32 nchars, OSUNICHAR* data);

EXTERNRT void rtInvokeCharStr32BitValue 
(OSCTXT* pCtxt, OSUINT32 nchars, OS32BITCHAR* data);

EXTERNRT void rtInvokeNullValue (OSCTXT* pCtxt);

EXTERNRT void rtInvokeOidValue 
(OSCTXT* pCtxt, OSUINT32 numSubIds, OSUINT32* pSubIds);

EXTERNRT void rtInvokeRealValue (OSCTXT* pCtxt, double value);

EXTERNRT void rtInvokeEnumValue 
(OSCTXT* pCtxt, OSUINT32 value, const OSUTF8CHAR* text);

EXTERNRT void rtInvokeOpenTypeValue 
(OSCTXT* pCtxt, OSUINT32 numocts, const OSOCTET* data);


#ifdef __cplusplus
}
#endif

#endif 

/**
 * @}
 */
