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
// incoming SUPL POS with RRLP ASSISTANCE DATA payload
// 
//

/**
 @file suplrrlpasn3step.cpp
 @internalTechnology
*/
#include "suplrrlpasn3step.h"
#include "te_suplrrlpasnsuitedefs.h"

// LBS
#include <lbs/lbsnetcommon.h>
#include <lbs/lbsassistancedatabase.h>
#include <lbspositioninfo.h>
#include <lbs/lbsgpsmeasurement.h>

// items under test
#include "suplmessagebase.h"
#include "suplasn1decoder.h"
#include "suplmessagecommon.h"
#include "suplstart.h"
#include "suplresponse.h"
#include "suplposinit.h"
#include "suplpos.h"
#include "suplpospayload.h"
#include "suplend.h"
#include "rrlpassistancedata.h"
#include "rrlpassistancedataack.h"
#include "rrlpmeasureposrequest.h"
#include "rrlpmeasureposresponse.h"
#include "rrlpprotocolerror.h"


CSuplRrlpAsn3Step::~CSuplRrlpAsn3Step()
/**
 * Destructor
 */
	{
	}

CSuplRrlpAsn3Step::CSuplRrlpAsn3Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KSuplRrlpAsn3Step);
	}

TVerdict CSuplRrlpAsn3Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iSpoofServer = CSuplSpoofServer::NewL();
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict CSuplRrlpAsn3Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	  if (TestStepResult()==EPass)
		{
		TRAPD(err, TestReceiveSuplPosRrlpAssistanceDataL());
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			}
		else
			{
			SetTestStepResult(EPass);
			}
		}
	  return TestStepResult();
	}

TVerdict CSuplRrlpAsn3Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	delete iSpoofServer;
	return TestStepResult();
	}


void CSuplRrlpAsn3Step::TestReceiveSuplPosRrlpAssistanceDataL()
	{
	TInt err = KErrNone;

	// Receive a SUPL POS with an RRLP ASSISTANCE DATA payload
	TPtr8 decodePtr(receiveBuffer.Des());
	err = iSpoofServer->GenerateSuplPosRrlpAssistanceDataL(decodePtr);
	LeaveIfErrorL(err);
	
	// Create an ASN1 Decoder object...
	CSuplAsn1Decoder* suplAsn1Decoder = CSuplAsn1Decoder::NewL();
	CleanupStack::PushL(suplAsn1Decoder);

	// ...and decode the received message
	TPtrC8 receivePtr(decodePtr);
	CSuplMessageBase* receivedMessage = suplAsn1Decoder->DecodeL(&receivePtr, err);
	CleanupStack::PushL(receivedMessage);

	// extract the version
	CSuplVersion* version = CSuplVersion::NewL();
	CleanupStack::PushL(version);
	err = receivedMessage->GetVersion(*version);
	LeaveIfErrorL(err);
	err = CheckVersionContentL(*version);
	LeaveIfErrorL(err);
	
	// extract the Session ID
	CSuplSessionId* sessionId = CSuplSessionId::NewL();
	CleanupStack::PushL(sessionId);
	err = receivedMessage->GetSessionId(*sessionId);
	LeaveIfErrorL(err);

	err = CheckSessionIdContentL(*sessionId);
	LeaveIfErrorL(err);
	
	// message type
	CSuplMessageBase::TSuplMessageType type = receivedMessage->MessageType();

	// handle message type specific stuff
	if (type == CSuplMessageBase::ESuplPos)
		{
		CSuplPos* pos = static_cast <CSuplPos*> (receivedMessage);
		
		// VELOCITY component
		if (pos->VelocityPresent())
			{
			// extract velocity.
			}
		
		// POSITIONING PAYLOAD
		CSuplPosPayload* posPayload = pos->PosPayload();
		CleanupStack::PushL(posPayload);
		if (posPayload->MessageType() == CSuplPosPayload::ERrlpAssistanceData)
			{
			CRrlpAssistanceData* assistData = static_cast <CRrlpAssistanceData*>(posPayload);
			
			TRrlpReference reference;
			err = assistData->GetReference(reference);
			LeaveIfErrorL(err);
			
			err = CheckRrlpReferenceContentL(reference);
			LeaveIfErrorL(err);
			
			if (assistData->AssistanceDataPresent())
				{
				TLbsAsistanceDataGroup dataMask;
				RLbsAssistanceDataBuilderSet builderSet;
				CleanupClosePushL(builderSet);
				builderSet.OpenL();
				err = assistData->BuildAssistanceData(dataMask, builderSet);
				LeaveIfErrorL(err);
				
				err = CheckAssistanceContentL(dataMask, builderSet);
				LeaveIfErrorL(err);

				CleanupStack::PopAndDestroy(&builderSet);
				}
			}
		else
			{
			// unexpected payload received
			err = KErrGeneral;
			LeaveIfErrorL(err);
			}
		CleanupStack::PopAndDestroy(posPayload);
		}
	else
		{
		// unexpected message received
		err = KErrGeneral;
		LeaveIfErrorL(err);
		}
	
	CleanupStack::PopAndDestroy(sessionId);
	CleanupStack::PopAndDestroy(version);		
	CleanupStack::PopAndDestroy(receivedMessage);	
	CleanupStack::PopAndDestroy(suplAsn1Decoder);
	}
	


// check received supl pos rrlp assistance data content
TInt CSuplRrlpAsn3Step::CheckAssistanceContentL(TLbsAsistanceDataGroup& aDataMask, RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	if (aDataMask != (  EAssistanceDataReferenceTime
					  |	EAssistanceDataReferenceLocation
					  |	EAssistanceDataNavigationModel
					  |	EAssistanceDataIonosphericModel
					  |	EAssistanceDataPositioningGpsUtcModel
					  |	EAssistanceDataAlmanac
					  |	EAssistanceDataAquisitionAssistance
					  |	EAssistanceDataBadSatList ) )
		{
		return KErrGeneral;
		}
		
		// reference time
	if (KErrNone != CheckReferenceTimeContentL(aAssistanceDataBuilderSet))
		{
		return KErrGeneral;
		}

		// reference location
	if (KErrNone != CheckReferenceLocationContentL(aAssistanceDataBuilderSet))
		{
		return KErrGeneral;
		}
		
		// navigation model
	if (KErrNone != CheckNavigationModelContentL(aAssistanceDataBuilderSet))
		{
		return KErrGeneral;
		}

		// ionospheric model
	if (KErrNone != CheckIonosphericModelContentL(aAssistanceDataBuilderSet))
		{
		return KErrGeneral;
		}
	
		// utc model
	if (KErrNone != CheckUtcModelContentL(aAssistanceDataBuilderSet))
		{
		return KErrGeneral;
		}
	
		// almanac
	if (KErrNone != CheckAlmanacContentL(aAssistanceDataBuilderSet))
		{
		return KErrGeneral;
		}
		
		// bad satellite list
	if (KErrNone != CheckBadSatListContentL(aAssistanceDataBuilderSet))
		{
		return KErrGeneral;
		}

	return KErrNone;
	}


TInt CSuplRrlpAsn3Step::CheckReferenceTimeContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RUEPositioningGpsReferenceTimeBuilder* refTimeBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(refTimeBuilderPtr))
		{
		if (refTimeBuilderPtr->IsDataAvailable())
			{
			// Create a Ref Time Reader with data from the Builder
			RUEPositioningGpsReferenceTimeReader refTimeReader;
			CleanupClosePushL(refTimeReader);
			refTimeReader.OpenL();
			refTimeReader.DataBuffer() = refTimeBuilderPtr->DataBuffer();

			TUint tempUint;
			TBool tempBool;

			refTimeReader.GetField(TUEPositioningGpsReferenceTime::EGpsWeek, tempUint);
			if (tempUint != 701)
				{
				return KErrGeneral;
				}
				
			// GPS TOW is translated to ms from the RRLP 0.08s resolution.
			refTimeReader.GetField(TUEPositioningGpsReferenceTime::EGpsTow1Msec, tempUint);
			if (tempUint != 2602880)
				{
				return KErrGeneral;
				}

			// tow assist array.
			if(refTimeReader.FieldExists(TUEPositioningGpsReferenceTime::EGpsTowAssistArray))
				{
				RGpsTowAssistArrayReader towAssistArrayReader;
				refTimeReader.GetArrayReader(TUEPositioningGpsReferenceTime::EGpsTowAssistArray, towAssistArrayReader);
				
				// for each element in the array... (only 1)
					{
					RGpsTowAssistReader towAssistElementReader;
					towAssistArrayReader.GetFieldReader(0,towAssistElementReader);

					towAssistElementReader.GetField(TGpsTowAssist::ESatID, tempUint);
					if (tempUint != 38)
						{
						return KErrGeneral;
						}

					towAssistElementReader.GetField(TGpsTowAssist::ETlmMessage, tempUint);
					if (tempUint != 7098)
						{
						return KErrGeneral;
						}

					towAssistElementReader.GetField(TGpsTowAssist::EAlert, tempBool);
					if (tempBool != EFalse)
						{
						return KErrGeneral;
						}

					towAssistElementReader.GetField(TGpsTowAssist::EAntiSpoof, tempBool);
					if (tempBool != EFalse)
						{
						return KErrGeneral;
						}
					}
				}
			
			CleanupStack::PopAndDestroy(&refTimeReader);
			}
		else
			{
			return KErrGeneral;
			}
		}
	else
		{
		return KErrGeneral;
		}
	return KErrNone;
	}


TInt CSuplRrlpAsn3Step::CheckReferenceLocationContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RReferenceLocationBuilder* refLocBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(refLocBuilderPtr))
		{
		if (refLocBuilderPtr->IsDataAvailable())
			{
			// Create a Ref Loc Reader with data from the Builder
			RReferenceLocationReader refLocReader;
			CleanupClosePushL(refLocReader);
			refLocReader.OpenL();
			refLocReader.DataBuffer() = refLocBuilderPtr->DataBuffer();
			
			// compare against expected values
			if(refLocReader.FieldExists(TReferenceLocation::EEllipsoidPointAltitudeEllipsoide))
				{
				REllipsoidPointAltitudeEllipsoideReader ellipsoidReader;
				refLocReader.GetFieldReader(TReferenceLocation::EEllipsoidPointAltitudeEllipsoide, ellipsoidReader);
				TUint tempUint;
				TInt  tempInt;

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::ELatitudeSign, tempInt);
				if (tempInt != TEllipsoidPointAltitudeEllipsoide::ENorth)
					{
					return KErrGeneral;
					}

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::ELatitude, tempUint);
				if (tempUint != 0x360b60)
					{
					return KErrGeneral;
					}

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::ELongitude, tempInt);
				if (tempInt != 0x6c16c1)
					{
					return KErrGeneral;
					}

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::EAltitudeDirection, tempInt);
				if (tempInt != TEllipsoidPointAltitudeEllipsoide::EHeight)
					{
					return KErrGeneral;
					}
				
				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::EAltitude, tempUint);
				if (tempUint != 0x23ab)
					{
					return KErrGeneral;
					}

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::EUncertaintySemiMajor, tempUint);
				if (tempUint != 0x12)
					{
					return KErrGeneral;
					}

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::EUncertaintySemiMinor, tempUint);
				if (tempUint != 0x34)
					{
					return KErrGeneral;
					}
					
				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::EOrientationMajorAxis, tempUint);
				if (tempUint != 0x56)
					{
					return KErrGeneral;
					}

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::EUncertaintyAltitude, tempUint);
				if (tempUint != 0x78)
					{
					return KErrGeneral;
					}

				ellipsoidReader.GetField(TEllipsoidPointAltitudeEllipsoide::EConfidence, tempUint);
				if (tempUint != 0x9a)
					{
					return KErrGeneral;
					}
				
				CleanupStack::PopAndDestroy(&refLocReader);
				}
			else
				{
				// ellipsoid point field not present.
				return KErrGeneral;
				}
			}
		else
			{
			// reflocbuilder data not available
			return KErrGeneral;
			}
		}
	else
		{
		// can't get the data builder
		return KErrGeneral;
		}

	return KErrNone;
	}



TInt CSuplRrlpAsn3Step::CheckNavigationModelContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RUEPositioningGpsNavigationModelBuilder* navModelBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(navModelBuilderPtr))
		{
		if (navModelBuilderPtr->IsDataAvailable())
			{
			// Create a Nav Model Reader with data from the Builder
			RUEPositioningGpsNavigationModelReader navModelReader;
			CleanupClosePushL(navModelReader);
			navModelReader.OpenL();
			navModelReader.DataBuffer() = navModelBuilderPtr->DataBuffer();

			TUint tempUint;
	
			// nav model list
			if(navModelReader.FieldExists(TUEPositioningGpsNavigationModel::ENavigationModelSatelliteInfoArray))
				{
				RNavigationModelSatInfoArrayReader satInfoArrayReader;
				navModelReader.GetArrayReader(TUEPositioningGpsNavigationModel::ENavigationModelSatelliteInfoArray, satInfoArrayReader);
				
				// for each sat info element.. (only 1)
					{
					RNavigationModelSatInfoReader satInfoReader;
					satInfoArrayReader.GetFieldReader(0, satInfoReader);
					
					/** ESatId */
					satInfoReader.GetField(TNavigationModelSatInfo::ESatId, tempUint);
					if (tempUint != 2)
						{
						return KErrGeneral;
						}

					/** ESatelliteStatus */
					TSatelliteStatus satStatus;
					satInfoReader.GetField(TNavigationModelSatInfo::ESatelliteStatus, satStatus);
					if (satStatus != EEsNNU)
						{
						return KErrGeneral;
						}

					/** EEphemerisParameter */
					REphemerisParameterReader ephemerisReader;
					satInfoReader.GetFieldReader(TNavigationModelSatInfo::EEphemerisParameter, ephemerisReader);
					
					ephemerisReader.GetField(TEphemerisParameter::ECodeOnL2, tempUint);
					if (tempUint != 2)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EUraIndex, tempUint);
					if (tempUint != 5)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ESatHealth, tempUint);
					if (tempUint != 36)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EIodc, tempUint);
					if (tempUint != 832)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EL2Pflag, tempUint);
					if (tempUint != 0)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ETGD, tempUint);
					if ((TInt)tempUint != -35)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EToc, tempUint);
					if (tempUint != 9280)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EAf2, tempUint);
					if (tempUint != 75)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EAf1, tempUint);
					if ((TInt)tempUint != -4531)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EAf0, tempUint);
					if ((TInt)tempUint != -2076907)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ECrs, tempUint);
					if ((TInt)tempUint != -10073)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EDeltaN, tempUint);
					if ((TInt)tempUint != -28891)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EM0, tempUint);
					if ((TInt)tempUint != -2147472068)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ECuc, tempUint);
					if ((TInt)tempUint != -25419)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EE, tempUint);
					if (tempUint != 1472)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ECus, tempUint);
					if ((TInt)tempUint != -23000)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EASqrt, tempUint);
					if (tempUint != 27781)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EToe, tempUint);
					if (tempUint != 13342)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EFitInterval, tempUint);
					if (tempUint != 0)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EAodo, tempUint);
					if (tempUint != 18)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ECic, tempUint);
					if ((TInt)tempUint != -13052)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EOmega0, tempUint);
					if ((TInt)tempUint != -2147457393)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ECis, tempUint);
					if ((TInt)tempUint != -15593)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EI0, tempUint);
					if ((TInt)tempUint != -2147460618)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::ECrc, tempUint);
					if ((TInt)tempUint != -10196)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EOmega, tempUint);
					if ((TInt)tempUint != -2147451776)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EOmegaDot, tempUint);
					if ((TInt)tempUint != -8373169)
						{
						return KErrGeneral;
						}
					ephemerisReader.GetField(TEphemerisParameter::EIDot, tempUint);
					if ((TInt)tempUint != -1681)
						{
						return KErrGeneral;
						}
					}
				}
			else
				{
				// sat info array not present
				return KErrGeneral;
				}
			CleanupStack::PopAndDestroy(&navModelReader);
			}
		else
			{
			// no nav model data available
			return KErrGeneral;
			}
		}
	else
		{
		// failed to get nav model builder pointer.
		return KErrGeneral;
		}
	return KErrNone;
	}



TInt CSuplRrlpAsn3Step::CheckIonosphericModelContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RUEPositioningGpsIonosphericModelBuilder* ionModelBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(ionModelBuilderPtr))
		{
		if (ionModelBuilderPtr->IsDataAvailable())
			{
			// Create a Nav Model Reader with data from the Builder
			RUEPositioningGpsIonosphericModelReader ionModelReader;
			CleanupClosePushL(ionModelReader);
			ionModelReader.OpenL();
			ionModelReader.DataBuffer() = ionModelBuilderPtr->DataBuffer();

			TUint tempUint;

			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EAlfa0, tempUint);
			if ((TInt)tempUint != 30)
				{
				return KErrGeneral;
				}
			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EAlfa1, tempUint);
			if ((TInt)tempUint != -35)
				{
				return KErrGeneral;
				}
			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EAlfa2, tempUint);
			if ((TInt)tempUint != -37)
				{
				return KErrGeneral;
				}
			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EAlfa3, tempUint);
			if ((TInt)tempUint != -78)
				{
				return KErrGeneral;
				}
			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EBeta0, tempUint);
			if ((TInt)tempUint != -124)
				{
				return KErrGeneral;
				}			
			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EBeta1, tempUint);
			if ((TInt)tempUint != 117)
				{
				return KErrGeneral;
				}
			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EBeta2, tempUint);
			if ((TInt)tempUint != -36)
				{
				return KErrGeneral;
				}
			ionModelReader.GetField(TUEPositioningGpsIonosphericModel::EBeta3, tempUint);
			if ((TInt)tempUint != -47)
				{
				return KErrGeneral;
				}

			CleanupStack::PopAndDestroy(&ionModelReader);
			}
		else
			{
			// no ion data available
			return KErrGeneral;
			}
		}
	else
		{
		// failed to get ion model builder pointer.
		return KErrGeneral;
		}
	return KErrNone;
	}
	
TInt CSuplRrlpAsn3Step::CheckUtcModelContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RUEPositioningGpsUtcModelBuilder* utcModelBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(utcModelBuilderPtr))
		{
		if (utcModelBuilderPtr->IsDataAvailable())
			{
			// Create a Nav Model Reader with data from the Builder
			RUEPositioningGpsUtcModelReader utcModelReader;
			CleanupClosePushL(utcModelReader);
			utcModelReader.OpenL();
			utcModelReader.DataBuffer() = utcModelBuilderPtr->DataBuffer();

			TUint tempUint;

			utcModelReader.GetField(TUEPositioningGpsUtcModel::EA1, tempUint);
			if ((TInt)tempUint != -8369181)
				{
				return KErrGeneral;
				}
			utcModelReader.GetField(TUEPositioningGpsUtcModel::EA0, tempUint);
			if ((TInt)tempUint != -2147452026)
				{
				return KErrGeneral;
				}
			utcModelReader.GetField(TUEPositioningGpsUtcModel::ETot, tempUint);
			if ((TInt)tempUint != 164)
				{
				return KErrGeneral;
				}
			utcModelReader.GetField(TUEPositioningGpsUtcModel::EWnt, tempUint);
			if ((TInt)tempUint != 12)
				{
				return KErrGeneral;
				}
			utcModelReader.GetField(TUEPositioningGpsUtcModel::EDeltatLS, tempUint);
			if ((TInt)tempUint != 59)
				{
				return KErrGeneral;
				}
			utcModelReader.GetField(TUEPositioningGpsUtcModel::EWnlsf, tempUint);
			if ((TInt)tempUint != 64)
				{
				return KErrGeneral;
				}
			utcModelReader.GetField(TUEPositioningGpsUtcModel::EDn, tempUint);
			if ((TInt)tempUint != -18)
				{
				return KErrGeneral;
				}
			utcModelReader.GetField(TUEPositioningGpsUtcModel::EDeltatLSF, tempUint);	
			if ((TInt)tempUint != 19)
				{
				return KErrGeneral;
				}

			CleanupStack::PopAndDestroy(&utcModelReader);
			}
		else
			{
			// no utc model data available
			return KErrGeneral;
			}
		}
	else
		{
		// failed to get utc model builder pointer.
		return KErrGeneral;
		}
	return KErrNone;
	}
	
TInt CSuplRrlpAsn3Step::CheckAlmanacContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RUEPositioningGpsAlmanacBuilder* almanacBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(almanacBuilderPtr))
		{
		if (almanacBuilderPtr->IsDataAvailable())
			{
			// Create a Nav Model Reader with data from the Builder
			RUEPositioningGpsAlmanacReader almanacReader;
			CleanupClosePushL(almanacReader);
			almanacReader.OpenL();
			almanacReader.DataBuffer() = almanacBuilderPtr->DataBuffer();

			TUint tempUint;
			
			almanacReader.GetField(TUEPositioningGpsAlmanac::EWnA, tempUint);
			if (tempUint != 54)
				{
				return KErrGeneral;
				}
			
			// almanac list
			if(almanacReader.FieldExists(TUEPositioningGpsAlmanac::EAlmanacInfoSatArray))
				{
				RAlmanacSatInfoArrayReader almanacSatInfoArrayReader;
				almanacReader.GetArrayReader(TUEPositioningGpsAlmanac::EAlmanacInfoSatArray, almanacSatInfoArrayReader);
				
				// for each sat info element.. (only 1)
					{
					RAlmanacSatInfoReader almanacSatInfoReader;
					almanacSatInfoArrayReader.GetFieldReader(0, almanacSatInfoReader);
					
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EDataID, tempUint);
					//if (tempUint != )	// data field not present in RRLP message
					//	{
					//	return KErrGeneral;
					//	}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::ESatID, tempUint);
					if (tempUint != 15)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EE, tempUint);
					if (tempUint != 27607)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EToa, tempUint);
					if (tempUint != 112)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EDeltaI, tempUint);
					if ((TInt)tempUint != -25295)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EOmegaDot, tempUint);
					if ((TInt)tempUint != -29112)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::ESatHealth, tempUint);
					if ((TInt)tempUint != 12)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EASqrt, tempUint);
					if ((TInt)tempUint != 28982)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EOmega0, tempUint);
					if ((TInt)tempUint != -8385564)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EM0, tempUint);
					if ((TInt)tempUint != -8363342)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EOmega, tempUint);
					if ((TInt)tempUint != -8360795)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EAf0, tempUint);
					if (tempUint != 925)
						{
						return KErrGeneral;
						}
					almanacSatInfoReader.GetField(TAlmanacSatInfo::EAf1, tempUint);
					if (tempUint != 42)
						{
						return KErrGeneral;
						}
					}
				}
			else
				{
				// almanac sat info array does not exist
				return KErrGeneral;
				}
			CleanupStack::PopAndDestroy(&almanacReader);
			}
		else
			{
			// no nav model data available
			return KErrGeneral;
			}
		}
	else
		{
		// failed to get nav model builder pointer.
		return KErrGeneral;
		}
	return KErrNone;
	}
	
TInt CSuplRrlpAsn3Step::CheckAcquisitionAssistanceContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RUEPositioningGpsAcquisitionAssistanceBuilder* acquiAssistBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(acquiAssistBuilderPtr))
		{
		if (acquiAssistBuilderPtr->IsDataAvailable())
			{
			// Create a Nav Model Reader with data from the Builder
			RUEPositioningGpsAcquisitionAssistanceReader acquiAssistReader;
			CleanupClosePushL(acquiAssistReader);
			acquiAssistReader.OpenL();
			acquiAssistReader.DataBuffer() = acquiAssistBuilderPtr->DataBuffer();

			TUint tempUint;
			TInt  tempInt;
			
			// EGpsReferenceTime
			acquiAssistReader.GetField(TUEPositioningGpsAcquisitionAssistance::EGpsReferenceTime, tempUint);
			if (tempUint != 11360)
				{
				return KErrGeneral;
				}
			
			// EUtranGpsReferenceTime not provided by RRLP
			
			// ESatelliteInformationArray
			if(acquiAssistReader.FieldExists(TUEPositioningGpsAcquisitionAssistance::ESatelliteInformationArray))
				{
				RAcquisitionSatInfoArrayReader aquisitionSatInfoArrayReader;
				acquiAssistReader.GetArrayReader(TUEPositioningGpsAcquisitionAssistance::ESatelliteInformationArray, aquisitionSatInfoArrayReader);

				// for each sat info element.. (only 1)
					{
					RAcquisitionSatInfoReader aquisitionSatInfoReader;
					aquisitionSatInfoArrayReader.GetFieldReader(0, aquisitionSatInfoReader);
					
					aquisitionSatInfoReader.GetField(TAcquisitionSatInfo::ESatID, tempUint);
					if (tempUint != 7)
						{
						return KErrGeneral;
						}
					aquisitionSatInfoReader.GetField(TAcquisitionSatInfo::EDoppler0thOrder, tempInt);
					if (tempUint != -485)
						{
						return KErrGeneral;
						}
					RExtraDopplerInfoReader extraDopplerInfoReader;
					aquisitionSatInfoReader.GetFieldReader(TAcquisitionSatInfo::EExtraDopplerInfo, extraDopplerInfoReader);
					extraDopplerInfoReader.GetField(TExtraDopplerInfo::EDoppler1stOrder, tempInt);
					if (tempUint != 40)
						{
						return KErrGeneral;
						}
					TDopplerUncertainty dopplerUncertainty;
					extraDopplerInfoReader.GetField(TExtraDopplerInfo::EDopplerUncertainty, dopplerUncertainty);
					if (dopplerUncertainty != 5)
						{
						return KErrGeneral;
						}
					aquisitionSatInfoReader.GetField(TAcquisitionSatInfo::ECodePhase, tempUint);
					if (tempUint != 11)
						{
						return KErrGeneral;
						}
					aquisitionSatInfoReader.GetField(TAcquisitionSatInfo::EIntegerCodePhase, tempUint);
					if (tempUint != 1)
						{
						return KErrGeneral;
						}
					aquisitionSatInfoReader.GetField(TAcquisitionSatInfo::EGpsBitNumber, tempUint);
					if (tempUint != 2)
						{
						return KErrGeneral;
						}
					TCodePhaseSearchWindow codePhaseWindow;
					aquisitionSatInfoReader.GetField(TAcquisitionSatInfo::ECodePhaseSearchWindow, codePhaseWindow);
					if (tempUint != 2)
						{
						return KErrGeneral;
						}
					RTAzimuthAndElevationReader azimuthAndElevationReader;
					aquisitionSatInfoReader.GetFieldReader(TAcquisitionSatInfo::EAzimuthAndElevation, azimuthAndElevationReader);
					azimuthAndElevationReader.GetField(TAzimuthAndElevation::EAzimuth, tempUint);
					if (tempUint != 11)
						{
						return KErrGeneral;
						}
					azimuthAndElevationReader.GetField(TAzimuthAndElevation::EElevation, tempUint);
					if (tempUint != 5)
						{
						return KErrGeneral;
						}
					}
				}
			CleanupStack::PopAndDestroy(&acquiAssistReader);
			}
		else
			{
			// no nav model data available
			return KErrGeneral;
			}
		}
	else
		{
		// failed to get nav model builder pointer.
		return KErrGeneral;
		}
	return KErrNone;
	}


TInt CSuplRrlpAsn3Step::CheckBadSatListContentL(RLbsAssistanceDataBuilderSet& aAssistanceDataBuilderSet)
	{
	RBadSatListBuilder* badSatListBuilderPtr;
	if (KErrNone == aAssistanceDataBuilderSet.GetDataBuilder(badSatListBuilderPtr))
		{
		if (badSatListBuilderPtr->IsDataAvailable())
			{
			// Create a Nav Model Reader with data from the Builder
			RBadSatListReader badSatListReader;
			CleanupClosePushL(badSatListReader);
			badSatListReader.OpenL();
			badSatListReader.DataBuffer() = badSatListBuilderPtr->DataBuffer();
			
			if(badSatListReader.FieldExists(TBadSatList::EBadSatIdArray))
				{
				RBadSatArrayReader badSatArrayReader;
				badSatListReader.GetArrayReader(TBadSatList::EBadSatIdArray, badSatArrayReader);

				TUint tempUint;
				badSatArrayReader.GetElement(0, tempUint);
				if (tempUint != 35)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(1, tempUint);
				if (tempUint != 17)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(2, tempUint);
				if (tempUint != 50)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(3, tempUint);
				if (tempUint != 24)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(4, tempUint);
				if (tempUint != 51)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(5, tempUint);
				if (tempUint != 4)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(6, tempUint);
				if (tempUint != 4)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(7, tempUint);
				if (tempUint != 53)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(8, tempUint);
				if (tempUint != 37)
					{
					return KErrGeneral;
					}
				badSatArrayReader.GetElement(9, tempUint);
				if (tempUint != 62)
					{
					return KErrGeneral;
					}
				}
			CleanupStack::PopAndDestroy(&badSatListReader);
			}
		else
			{
			// no nav model data available
			return KErrGeneral;
			}
		}
	else
		{
		// failed to get nav model builder pointer.
		return KErrGeneral;
		}
	return KErrNone;
	}

