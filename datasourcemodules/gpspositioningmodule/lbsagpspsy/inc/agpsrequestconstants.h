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
 @InternalComponent
*/

// Pre-defined technology types for common positioning modes.
const TPositionModuleInfo::TTechnologyType KTerminalAssistedMode = 
		(TPositionModuleInfo::ETechnologyNetwork |
		 TPositionModuleInfo::ETechnologyAssisted);
		 
const TPositionModuleInfo::TTechnologyType KTerminalBasedMode = 
		(TPositionModuleInfo::ETechnologyTerminal |
		 TPositionModuleInfo::ETechnologyAssisted);
		 
const TPositionModuleInfo::TTechnologyType KAutonomousMode = 
		(TPositionModuleInfo::ETechnologyTerminal);
