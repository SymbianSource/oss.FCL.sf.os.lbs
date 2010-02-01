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
//

#ifndef TE_NPETESTCLIENTAPISTEPBASE_H
#define TE_NPETESTCLIENTAPISTEPBASE_H

#include <test/testexecutestepbase.h>

class CTe_NpeTestClientApiStepBase : public CTestStep
	{
public:
	virtual ~CTe_NpeTestClientApiStepBase();
	CTe_NpeTestClientApiStepBase();
	virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();

protected:
	HBufC8*				iReadData;
	HBufC8*				iWriteData;
	CActiveScheduler* 	iRootScheduler;
	};

#endif	//	TE_NPETESTCLIENTAPISTEPBASE_H
