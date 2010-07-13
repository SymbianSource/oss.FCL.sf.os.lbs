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
#ifndef SUPL_POS_INIT_H
#define SUPL_POS_INIT_H

#include <stdio.h>
#include <stdlib.h>
#include "rtkey.h"
#include "rtpersrc/asn1PerCppTypes.h"
#include "ASN1CSeqOfList.h"
#include "SUPL-POS.h"
#include "SUPL-START.h"
#include "ULP-Components.h"

/**
 * Header file for ASN.1 module SUPL-POS-INIT
 */
/**************************************************************/
/*                                                            */
/*  SatelliteInfoElement                                      */
/*                                                            */
/**************************************************************/

#define TV_SatelliteInfoElement	(TM_UNIV|TM_CONS|16)

struct ASN1T_SatelliteInfoElement {
   OSUINT8 satId;
   OSUINT8 iODE;
   IMPORT_C ASN1T_SatelliteInfoElement ();
} ;

IMPORT_C int asn1PE_SatelliteInfoElement (OSCTXT* pctxt, ASN1T_SatelliteInfoElement* pvalue);

IMPORT_C int asn1PD_SatelliteInfoElement (OSCTXT* pctxt, ASN1T_SatelliteInfoElement* pvalue);

IMPORT_C void asn1Print_SatelliteInfoElement
   (const char* name, ASN1T_SatelliteInfoElement* pvalue);

/**************************************************************/
/*                                                            */
/*  SatelliteInfo                                             */
/*                                                            */
/**************************************************************/

#define TV_SatelliteInfo	(TM_UNIV|TM_CONS|16)

/* List of ASN1T_SatelliteInfoElement */
typedef ASN1TSeqOfList ASN1T_SatelliteInfo;

class ASN1C_SatelliteInfo :
public ASN1CSeqOfList
{
protected:
   ASN1T_SatelliteInfo& msgData;
public:
   IMPORT_C ASN1C_SatelliteInfo (ASN1T_SatelliteInfo& data);
   IMPORT_C ASN1C_SatelliteInfo (OSRTMessageBufferIF& msgBuf, ASN1T_SatelliteInfo& data);
   ASN1C_SatelliteInfo (ASN1CType& ccobj, ASN1T_SatelliteInfo& data);
   IMPORT_C ASN1C_SatelliteInfo (OSRTContext &context, ASN1T_SatelliteInfo& data);
   void Append (ASN1T_SatelliteInfoElement* elem);
   ASN1T_SatelliteInfoElement* NewElement ();
} ;

IMPORT_C int asn1PE_SatelliteInfo (OSCTXT* pctxt, ASN1T_SatelliteInfo* pvalue);

IMPORT_C int asn1PD_SatelliteInfo (OSCTXT* pctxt, ASN1T_SatelliteInfo* pvalue);

IMPORT_C void asn1Print_SatelliteInfo
   (const char* name, ASN1T_SatelliteInfo* pvalue);

IMPORT_C void asn1Free_SatelliteInfo (OSCTXT *pctxt, ASN1T_SatelliteInfo* pvalue);

/**************************************************************/
/*                                                            */
/*  NavigationModel                                           */
/*                                                            */
/**************************************************************/

#define TV_NavigationModel	(TM_UNIV|TM_CONS|16)

struct ASN1T_NavigationModel {
   struct {
      unsigned satInfoPresent : 1;
   } m;
   OSUINT16 gpsWeek;
   OSUINT8 gpsToe;
   OSUINT8 nSAT;
   OSUINT8 toeLimit;
   ASN1T_SatelliteInfo satInfo;
   IMPORT_C ASN1T_NavigationModel ();
} ;

IMPORT_C int asn1PE_NavigationModel (OSCTXT* pctxt, ASN1T_NavigationModel* pvalue);

IMPORT_C int asn1PD_NavigationModel (OSCTXT* pctxt, ASN1T_NavigationModel* pvalue);

IMPORT_C void asn1Print_NavigationModel
   (const char* name, ASN1T_NavigationModel* pvalue);

IMPORT_C void asn1Free_NavigationModel (OSCTXT *pctxt, ASN1T_NavigationModel* pvalue);

/**************************************************************/
/*                                                            */
/*  RequestedAssistData                                       */
/*                                                            */
/**************************************************************/

#define TV_RequestedAssistData	(TM_UNIV|TM_CONS|16)

struct ASN1T_RequestedAssistData {
   struct {
      unsigned navigationModelDataPresent : 1;
   } m;
   OSBOOL almanacRequested;
   OSBOOL utcModelRequested;
   OSBOOL ionosphericModelRequested;
   OSBOOL dgpsCorrectionsRequested;
   OSBOOL referenceLocationRequested;
   OSBOOL referenceTimeRequested;
   OSBOOL acquisitionAssistanceRequested;
   OSBOOL realTimeIntegrityRequested;
   OSBOOL navigationModelRequested;
   ASN1T_NavigationModel navigationModelData;
   IMPORT_C ASN1T_RequestedAssistData ();
} ;

IMPORT_C int asn1PE_RequestedAssistData (OSCTXT* pctxt, ASN1T_RequestedAssistData* pvalue);

IMPORT_C int asn1PD_RequestedAssistData (OSCTXT* pctxt, ASN1T_RequestedAssistData* pvalue);

IMPORT_C void asn1Print_RequestedAssistData
   (const char* name, ASN1T_RequestedAssistData* pvalue);

IMPORT_C void asn1Free_RequestedAssistData (OSCTXT *pctxt, ASN1T_RequestedAssistData* pvalue);

/**************************************************************/
/*                                                            */
/*  SUPLPOSINIT                                               */
/*                                                            */
/**************************************************************/

#define TV_SUPLPOSINIT	(TM_UNIV|TM_CONS|16)

struct ASN1T_SUPLPOSINIT {
   struct {
      unsigned requestedAssistDataPresent : 1;
      unsigned positionPresent : 1;
      unsigned sUPLPOSPresent : 1;
      unsigned verPresent : 1;
   } m;
   ASN1T_SETCapabilities sETCapabilities;
   ASN1T_RequestedAssistData requestedAssistData;
   ASN1T_LocationId locationId;
   ASN1T_Position position;
   ASN1T_SUPLPOS sUPLPOS;
   ASN1T_Ver ver;
   IMPORT_C ASN1T_SUPLPOSINIT ();
} ;

IMPORT_C int asn1PE_SUPLPOSINIT (OSCTXT* pctxt, ASN1T_SUPLPOSINIT* pvalue);

IMPORT_C int asn1PD_SUPLPOSINIT (OSCTXT* pctxt, ASN1T_SUPLPOSINIT* pvalue);

IMPORT_C void asn1Print_SUPLPOSINIT
   (const char* name, ASN1T_SUPLPOSINIT* pvalue);

IMPORT_C void asn1Free_SUPLPOSINIT (OSCTXT *pctxt, ASN1T_SUPLPOSINIT* pvalue);

#endif