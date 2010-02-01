// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file CT_LbsStep_SetBehaviourModeOriginal.h
// This contains the header file for Set Autonomous Mode Test Step
// 
//

/**
 @file
 @internalTechnology
 @test
*/

#ifndef __CT_LBS_STEP_SET_BEHAVIOUR_ORIGINAL_MODE_H__
#define __CT_LBS_STEP_SET_BEHAVIOUR_ORIGINAL_MODE_H__

#include "ctlbsstep.h"
#include "ctlbsserver.h"

_LIT(KLbsStep_SetBehaviourModeOriginal, "LbsStep_SetBehaviourModeOriginal"); 

class CT_LbsStep_SetBehaviourModeOriginal: public CT_LbsStep
	{
public:
	static CT_LbsStep_SetBehaviourModeOriginal* New(CT_LbsServer& aParent);
	TVerdict doTestStepL();

private:
	CT_LbsStep_SetBehaviourModeOriginal(CT_LbsServer& aParent);
	
	TBool SetAndVerifyBehaviourModeOriginalL();
	
	};

#endif // __CT_LBS_STEP_SET_BEHAVIOUR_ORIGINAL_MODE_H__