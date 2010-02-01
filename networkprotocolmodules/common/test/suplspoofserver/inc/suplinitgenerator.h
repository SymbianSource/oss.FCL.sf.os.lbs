// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file
 @internalTechnology
 
*/
#ifndef LBS_SUPL_INIT_GENERATOR_H
#define LBS_SUPL_INIT_GENERATOR_H

#include <e32base.h>

class ASN1T_ULP_PDU;
class ASN1C_ULP_PDU;
class ASN1T_PDU;
class ASN1C_PDU;
class ASN1PEREncodeBuffer;
class OSCTXT;

enum ESuplMessageType
	{
	ESuplInit,
	ESuplResponse,
	ESuplPosInit,
	ESuplPos,
	ESuplEnd,
	ESuplAuthReq,
	ESuplAuthResp
	};
	
/**
SUPL INIT Generator

Generates ASN1 encoded SUPL INIT messages.
Specically for Unit testing of SUPL Protocol Module MT-LR transactions.

@internalTechnology
*/
class CSuplInitGenerator : public CBase
	{
public:
	IMPORT_C static CSuplInitGenerator* NewL();
		
public:
	virtual ~CSuplInitGenerator();
	
public:

	// encoded supl init message generator
	IMPORT_C TInt GenerateSuplInitL(TInt aNum, TPtr8& aBufPtr);
	
	// test decoder
	IMPORT_C TInt DecodeL(const TPtrC8* aBuf);
	
private:
	/** Constructor */
	CSuplInitGenerator();
	
	/** second stage constructor */
	void ConstructL();
	
private:

	// SUPL content generators
	ASN1T_ULP_PDU* genSuplCommonContent(OSCTXT *pctxt);
	
	ASN1T_ULP_PDU* genSuplInitContent0(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent1(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent2(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent3(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent4(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent5(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent6(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent7(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent8(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent9(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent10(OSCTXT *pctxt);
	ASN1T_ULP_PDU* genSuplInitContent11(OSCTXT *pctxt);
	
	
private: 
	ASN1PEREncodeBuffer* iEncodeBuffer;
    ASN1T_ULP_PDU* iData;
    ASN1C_ULP_PDU* iUlpPdu;

	};

#endif // LBS_SUPL_INIT_GENERATOR_H

