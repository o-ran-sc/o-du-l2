/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* Contains definitions for MAC CL modules */
#ifndef __LWR_MAC_H__
#define __LWR_MAC_H__

#ifdef INTEL_WLS_MEM
#define LWR_MAC_ALLOC(_datPtr, _size)   WLS_MEM_ALLOC(_datPtr, _size);
#else                                     
#define LWR_MAC_ALLOC(_datPtr, _size)   MAC_ALLOC(_datPtr, _size);
#endif                                    

#ifdef INTEL_WLS_MEM
#define LWR_MAC_FREE(_datPtr, _size) WLS_MEM_FREE(_datPtr, _size);
#else
#define LWR_MAC_FREE(_datPtr, _size) MAC_FREE(_datPtr, _size);
#endif

typedef enum
{
   PHY_STATE_IDLE,
   PHY_STATE_CONFIGURED,
   PHY_STATE_RUNNING,
   MAX_STATE   
}PhyState;

/* Events in Lower Mac */
typedef enum{
#ifdef INTEL_TIMER_MODE
   UL_IQ_SAMPLE,
#endif
   PARAM_REQUEST,
   PARAM_RESPONSE,
   CONFIG_REQUEST,
   CONFIG_RESPONSE,
   START_REQUEST,
   STOP_REQUEST,
   MAX_EVENT
}EventState;

typedef struct cellCb
{
   uint16_t     cellId;
   uint16_t     phyCellId;
   PhyState     state;
}LwrMacCellCb;

typedef struct lwrMacGlobalCb
{
   Region          region;
   Pool            pool;
   bool            clCfgDone;   /* CL configuration done */
   uint8_t         phySlotIndCntr;
   LwrMacCellCb    cellCb[MAX_NUM_CELL];   /* List of Cells configured */
   uint8_t         numCell;  /* Number of Cells configured */
   PhyState        phyState;    /* State of PHY */
   EventState      event;       /* State of Event */
}LwrMacCb;

typedef enum
{
   FAPI_RELEASE_15
}ReleaseCapab;

typedef enum
{
   NOT_SUPPORTED,
   SUPPORTED
}ParamSupport;

typedef enum
{
   NORMAL_CYCLIC_PREFIX_MASK,
   EXTENDED_CYCLIC_PREFIX_MASK
}CyclicPrefix;

typedef enum 
{
   SPACING_15_KHZ,
   SPACING_30_KHZ,
   SPACING_60_KHZ,
   SPACING_120_KHZ
}SubCarrierSpacing;

typedef enum 
{
   BW_5MHZ,
   BW_10MHZ,
   BW_15MHZ,
   BW_20MHZ,
   BW_40MHZ,
   BW_50MHZ,
   BW_60MHZ,
   BW_70MHZ,
   BW_80MHZ,
   BW_90MHZ,
   BW_100MHZ,
   BW_200MHZ,
   BW_400MHZ
}SupportedBandwidth;

typedef enum
{
   CCE_MAPPING_INTERLEAVED_MASK,
   CCE_MAPPING_NONINTERLVD_MASK
}CCEMappingType;

typedef enum
{
   FORMAT_0,
   FORMAT_1,
   FORMAT_2,
   FORMAT_3,
   FORMAT_4
}Formats;

typedef enum
{
   MAPPING_TYPE_A,        
   MAPPING_TYPE_B,
}MappingType;

typedef enum
{
   ALLOCATION_TYPE_0,            
   ALLOCATION_TYPE_1,
}AllocationType;

typedef enum
{
   VRB_TO_PRB_MAP_NON_INTLV,
   VRB_TO_PRB_MAP_INTLVD
}VrbToPrbMap;

typedef enum
{  
   DMRS_CONFIG_TYPE_1,     
   DMRS_CONFIG_TYPE_2
}DmrsConfigType;

typedef enum
{  
   DMRS_MAX_LENGTH_1,     
   DMRS_MAX_LENGTH_2
}DmrMaxLen;

typedef enum
{
   DMRS_ADDITIONAL_POS_0,         
   DMRS_ADDITIONAL_POS_1,         
   DMRS_ADDITIONAL_POS_2,         
   DMRS_ADDITIONAL_POS_3        
}DmrsPos;

typedef enum
{
   MOD_QPSK,
   MOD_16QAM,
   MOD_64QAM,
   MOD_256QAM
}ModulationOrder;

typedef enum 
{
   AGG_FACTOR_1,
   AGG_FACTOR_2,
   AGG_FACTOR_4,
   AGG_FACTOR_8
}AggregationFactor;

typedef enum
{
   SF_FORMAT_A1,
   SF_FORMAT_A2,
   SF_FORMAT_A3,
   SF_FORMAT_B1,
   SF_FORMAT_B2,
   SF_FORMAT_B3,
   SF_FORMAT_B4,
   SF_FORMAT_C0,
   SF_FORMAT_C2
}ShortFormat;

typedef enum 
{
   PRACH_FD_OCC_IN_A_SLOT_1   = 1,
   PRACH_FD_OCC_IN_A_SLOT_2   = 2,
   PRACH_FD_OCC_IN_A_SLOT_4   = 4,
   PRACH_FD_OCC_IN_A_SLOT_8   = 8
}FdOccPerSlot;

typedef enum
{
   RSSI_REPORT_DBM,
   RSSI_REPORT_DBFS
}RssiMeasurement;

typedef struct clCellParam
{   
   ReleaseCapab          releaseCapability;                    /* Release Capability */  
   PhyState              ParamPhystate;
   ParamSupport          skipBlankDlConfig;
   ParamSupport          skipBlankUlConfig;
   ParamSupport          numTlvsToReport;
   CyclicPrefix          cyclicPrefix;                
   SubCarrierSpacing     supportedSubcarrierSpacingDl;
   SupportedBandwidth    supportedBandwidthDl;         
   SubCarrierSpacing     supportedSubcarrierSpacingsUl;
   SupportedBandwidth    supportedBandwidthUl;
   CCEMappingType        cceMappingType;
   ParamSupport          coresetOutsideFirst3OfdmSymsOfSlot;
   ParamSupport          precoderGranularityCoreset;
   ParamSupport          pdcchMuMimo;
   ParamSupport          pdcchPrecoderCycling;
   uint8_t               maxPdcchsPerSlot;
   Formats               pucchFormats;
   uint8_t               maxPucchsPerSlot;   
   MappingType           pdschMappingType;
   AllocationType        pdschAllocationTypes;
   VrbToPrbMap           pdschVrbToPrbMapping;
   ParamSupport          pdschCbg;
   DmrsConfigType        pdschDmrsConfigTypes;
   DmrMaxLen             pdschDmrsMaxLength;
   DmrsPos               pdschDmrsAdditionalPos;
   uint8_t               maxPdschsTBsPerSlot;
   uint8_t               maxNumberMimoLayersPdsch;
   ModulationOrder       supportedMaxModulationOrderDl;
   uint8_t               maxMuMimoUsersDl;
   ParamSupport          pdschDataInDmrsSymbols;
   ParamSupport          premptionSupport;
   ParamSupport          pdschNonSlotSupport;
   ParamSupport          uciMuxUlschInPusch;
   ParamSupport          uciOnlyPusch;
   ParamSupport          puschFrequencyHopping;
   DmrsConfigType        puschDmrsConfigTypes;
   DmrMaxLen             puschDmrsMaxLength;
   DmrsPos               puschDmrsAdditionalPos;
   ParamSupport          puschCbg;
   MappingType           puschMappingType;
   AllocationType        puschAllocationTypes;
   VrbToPrbMap           puschVrbToPrbMapping;
   uint8_t               puschMaxPtrsPorts;
   uint8_t               maxPduschsTBsPerSlot;
   uint8_t               maxNumberMimoLayersNonCbPusch;
   ModulationOrder       supportedModulationOrderUl;
   uint8_t               maxMuMimoUsersUl;
   ParamSupport          dftsOfdmSupport;
   AggregationFactor     puschAggregationFactor;
   Formats                prachLongFormats;
   ShortFormat           prachShortFormats;
   ParamSupport          prachRestrictedSets;
   FdOccPerSlot          maxPrachFdOccasionsInASlot;
   RssiMeasurement       rssiMeasurementSupport;
}ClCellParam;

LwrMacCb lwrMacCb; 
LwrMacCellCb * lwrMacGetCellCb ARGS((uint16_t cellId));
uint32_t reverseBits(uint32_t num, uint8_t numBits);
void fillDlDciPayload(uint8_t *buf, uint8_t *bytePos, uint8_t *bitPos,\
      uint32_t val, uint8_t valSize);

#endif

/**********************************************************************
  End of file
 **********************************************************************/
