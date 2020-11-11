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

/**********************************************************************
     Name:    Lower layer interface - TFU

     Type:    C header file

     Desc:    Structures, variables and typedefs required by 
              TFU interface

     File:    tfu.x

*********************************************************************21*/

/**
  @file tfu.x
  @brief Structure declarations and definitions for TFU interface.
  */


#ifndef __TFUX__
#define __TFUX__
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
                          typedefs and data structures
 ***********************************************************************/
/** @enum TfuDciFormat
 * This Enum has values for the DCI format types.
 */
typedef enum
{
   TFU_DCI_FORMAT_0,       /*!< DCI format 0 */
   TFU_DCI_FORMAT_1,       /*!< DCI format 1 */
   TFU_DCI_FORMAT_1A,      /*!< DCI format 1A */
   TFU_DCI_FORMAT_1B,      /*!< DCI format 1B */
   TFU_DCI_FORMAT_1C,      /*!< DCI format 1C */
   TFU_DCI_FORMAT_1D,      /*!< DCI format 1D */
   TFU_DCI_FORMAT_2,       /*!< DCI format 2 */
   TFU_DCI_FORMAT_2A,      /*!< DCI format 2A */
   TFU_DCI_FORMAT_3,       /*!< DCI format 3 */
   TFU_DCI_FORMAT_3A       /*!< DCI format 3A */
#ifdef TFU_5GTF
   ,TFU_DCI_FORMAT_A1,      /*!< 5GTF: DCI format A1 */
   TFU_DCI_FORMAT_A2,       /*!< 5GTF: DCI format A2 */
   TFU_DCI_FORMAT_B1,       /*!< 5GTF: DCI format B1 */
   TFU_DCI_FORMAT_B2        /*!< 5GTF: DCI format B2 */
#endif
#ifdef EMTC_ENABLE
   ,TFU_DCI_FORMAT_6_0A,    /*!< DCI format 6-0 A */
   TFU_DCI_FORMAT_6_0B,    /*!< DCI format 6-0 B */
   TFU_DCI_FORMAT_6_1A,    /*!< DCI format 6-1 A */
   TFU_DCI_FORMAT_6_1B,    /*!< DCI format 6-1 B */
   TFU_DCI_FORMAT_6_2      /*!< DCI format 6-2 */
#endif   
      /*tfu_x_001.main_6 - Added support for SPS*/
#ifdef LTEMAC_SPS
   ,TFU_DCI_FORMAT_INVALID  /*!< Invalid DCI format value */
#endif
} TfuDciFormat;

/** @enum TfuModScheme
 * This enum contains the values for the modulation schemes possible. 
 */
typedef enum
{
   TFU_MOD_BPSK  = 1,
   TFU_MOD_QPSK  = 2,
   TFU_MOD_16QAM = 4,
   TFU_MOD_64QAM = 6
} TfuModScheme;


/** @enum TfuDlCqiPucchMode
 * This enum contains values for the CQI reporting modes. 
 */
typedef enum
{
   TFU_PUCCH_CQI_MODE10,
   TFU_PUCCH_CQI_MODE11,
   TFU_PUCCH_CQI_MODE20,
   TFU_PUCCH_CQI_MODE21
} TfuDlCqiPucchMode;

/** @enum TfuRptType
 * This enum contains values for Type of UEs CQI reporting. 
 */
typedef enum
{
   TFU_RPT_CQI,
   TFU_RPT_RI
} TfuRptType;


#ifdef PHY_ERROR_LOGING
typedef struct _rgSchUlAllocCntr RgSchUlAllocCntr;

struct _rgSchUlAllocCntr
{
   uint8_t  mcs;
   uint16_t numOfRb;
   uint16_t rbStart;
   Bool testStart;
   Bool enaLog;
   uint16_t  logTime;
   uint32_t  crcOk;
   uint32_t  crcErr;
   uint32_t  numUlPackets;
   uint32_t  numPrach;
   uint32_t  taZero;
};

RgSchUlAllocCntr rgSchUlAllocCntr;

#endif
//Chandan
#ifdef DLHQ_STATS
typedef struct rgSchDlHqStats
{
   uint16_t   sfn;
   uint8_t    sf;
   uint8_t    ack;
   uint8_t    cellId;
   uint8_t    ulDai;
   uint8_t    dlDai;
   uint8_t    fdbkIdx;
   uint8_t    ue;
   uint8_t    ack0;
   uint8_t    ack1;
   uint8_t    ack2;
   uint8_t    ack3;
   uint8_t    ack4;
   uint8_t    ack5;
   uint8_t    ack6;
   uint8_t    ack7;
}RgSchDlHqStats;
typedef struct ysMsDlHqStats
{
   uint16_t   sfn;
   uint8_t    sf;
   uint8_t    mode; /*1 = PUCCH 2= PUSCH*/
   uint8_t    M;
   uint8_t    o0; /*For PUCCH n1PUCCH*/
   uint8_t    o1; /*For PUCCH Invalid*/
   uint8_t    o2;
   uint8_t    o3;
   uint8_t    ack0;
   uint8_t    ack1;
   uint8_t    ack2;
   uint8_t    ack3;
   uint8_t    ack4;
   uint8_t    ack5;
   uint8_t    ack6;
   uint8_t    ack7;
}YsMsDlHqStats;
#define MAX_STATS_CNT
#endif

#ifdef TFU_5GTF
/* Considers case where RIV is <= 324 for DCI format A1 */
typedef struct tfuRbAssignA1Val324
{
   uint8_t         hqProcId;   /*!< HARQ process ID */
   uint8_t		     mcs;		  /*!< 4 bits for MCS */
   Bool		  ndi;		  /*!< 1 bit for new data indicator */
} TfuRbAssignA1Val324;

/* Considers case where RIV is <= 324 for DCI format B1 */
typedef struct tfuRbAssignB1Val324
{
   uint8_t         hqProcId;   /*!< HARQ process ID */
   uint8_t		     mcs;		  /*!< 4 bits for MCS */
   Bool		  ndi;		  /*!< 1 bit for new data indicator */
   uint8_t         RV;         /*!< 2 bits Redundancy version */
   uint8_t         bmiHqAckNack; /* BMI: Bit Mapping Index for Harq-Ack
                                Refer sec 8.5 of 213 spec */ 
} TfuRbAssignB1Val324;

/* Considers case where RIV is == 326 */
typedef struct tfuRbAssignVal326
{
   uint8_t          FreqBandIdx;  /* nRACH: Frequency band index Value:0 -> 7 */
   Bool        OCCInd;       /* nOCC: OCC indicator Value:0 -> 1 */
   uint8_t          CyclicShiftInd; /* nNcs: Cyclic shift indicator Value:0 -> 3 */
} TfuRbAssignVal326;


/* DCI format A1: UL xPDCCH */
typedef struct tfuDciFormatA1Info
{
   Bool        formatType;    /* Format Descriminator 00: A1, 01:A2 */
   uint8_t          xPUSCHRange;  /* 0 : the stopping of xPUSCH is the 12th symbol, 
                                                    1 : the stopping of xPUSCH is the 13th symbol,
                                                    2 : the stopping of xPUSCH is the final (14th) symbol */
   uint8_t          xPUSCH_TxTiming; /* Transmission timing of xPUSCH Value: 0->7 
                                   l E {0,1,...,7}*/
   uint16_t         RBAssign;        /*!< same as phy i/f RBAssign: RIV as defined in V5G.213
                                     Value <= 324: this field assigns morethan zero RB.
                                     Value ==325: this format assigns zero RB.
                                     Value ==326: this format assigns zero RB and used 
                                     for Randon Access procedure initiated by xPDCCH order. */
   uint8_t          beamIndex;       /*!< transmit beam index: Value: 0->7 */
   uint8_t          symbIdx;         /*!< OFDM symbol index for the xPDCCH: Value: 0->1 */
   uint8_t          rv;              /*!< HARQ redundancy version,  Value: 0 -> 3 */

   /* Based on RBAssign/RIV value*/
   union
   {
      TfuRbAssignA1Val324 rbAssignA1Val324;
      TfuRbAssignVal326   rbAssignVal326;
   }u;

   uint8_t          CSI_BSI_BRI_Req; /* CSI / BSI / BRI request Value:0 -> 7 */
   uint8_t          CSIRS_BRRS_TxTiming; /* Transmission timing of CSI-RS / BRRS, Value:0 -> 3
                                        m E {0,1,2,3} */
#ifdef FIVEGTF_PHY_API
   uint16_t         rbgStart;
   uint16_t         rbgSize;
#endif

   /* ToDo: following 4 fields can be made as union with an indicator type*/
   uint8_t          CSIRS_BRRS_SymbIdx; /* Indication of OFDM symbol index for CSI-RS / BRRS allocations
                                                         Value:0 -> 3 */
   uint8_t          CSI_BRRS_Indicator; /* If this DCI format allocates either of 
                                      CSI-RS or BRRS transmission Value:0 -> 3 */
   uint8_t          CSIRS_BRRS_ProcInd; /* Process indicator : Value:0 -> 3 */
   uint8_t          numBSI_Reports;  /* Number of BSI reports Value:0 -> 3 */

   Bool        uciOnxPUSCH;   /* "UCI on xPUSCH w/o xUL-SCH data" indicator. Value:0 -> 1 */
   Bool        beamSwitch;    /* beam switch indication. Value:0 -> 1 */

   uint8_t          SRS_Config; /* Indication of SRS configurations Value:0 -> 3
                              00 : {No SRS request}, 01 : {Config. #0},
                              10 : {Config. #1}, 11 : {Config. #2} */
   Bool        SRS_Symbol;  /* If SRS is requested, 0: SRS transmission on the 13th OFDM symbol
                              and 1: SRS transmission on the 14th OFDM symbol */   

   uint8_t          REMapIdx_DMRS_PCRS_numLayers; /* RE Mapping index, Ki for DMRS/PCRS and number of layers
                                                Value: 0 -> 7 
                                                0: 1 Layer, k0 = 0, 1: 1 Layer, k0 = 1
                                                2: 1 Layer, k0 = 2, 3: 1 Layer, k0 = 3
                                                4: 2 Layers, k0 = 0 and k1 = 1
                                                5: 2 Layers, k0 = 2 and k1 = 3
                                                6,7: Reserved */
   Bool        SCID;         /* nSCID is applied for both DMRS in subframe n and
   	                          CSI-RS in subframe n+m Value: 0 -> 1 */
   uint8_t          PMI;          /* Precoding matrix indicator Value:0 -> 7 */
#ifdef FIVEGTF_PHY_API
   uint8_t          AntPorts_numLayers;
   uint8_t          TransmissionScheme;
#endif
   Bool	      UL_PCRS;      /* UL dual PCRS 	Value: 0 -> 1 */
   uint8_t          tpcCmd;       /*!< 2 bits for TPC command for xPUSCH, Values: 0->3  */													  
} TfuDciFormatA1Info;

/* DCI format A2: UL xPDCCH */
typedef TfuDciFormatA1Info TfuDciFormatA2Info;

/* DCI format B1: DL xPDCCH */
typedef struct tfuDciFormatB1Info
{
   Bool        formatType;    /* Format Descriminator 2: B1, 3:B2 */
   uint8_t          xPDSCHRange;  /* MSB (starting of xPDSCH including DMRS symbol) :
                                0 is the second symbol, 1 is the third symbol.
                                LSB (stopping of xPDSCH): 0 is the 12th symbol,
                                1 is the 14th symbol */
   uint16_t         RBAssign;        /*!< same as phy i/f RBAssign: RIV as defined in V5G.213
                                     Value <= 324: this field assigns morethan zero RB.
                                     Value ==325: this format assigns zero RB.
                                     Value ==326: this format assigns zero RB and used 
                                     for Randon Access procedure initiated by xPDCCH order. */
   uint8_t          beamIndex;       /*!< transmit beam index: Value: 0->7 */
   uint8_t          symbIdx;         /*!< OFDM symbol index for the xPDCCH: Value: 0->1 */
   /* Based on RBAssign/RIV value*/
   union
   {
      TfuRbAssignB1Val324 rbAssignB1Val324;
      TfuRbAssignVal326   rbAssignVal326;
   }u;

   uint8_t          CSI_BSI_BRI_Req; /* CSI / BSI / BRI request Value:0 -> 7 */
   uint8_t          CSIRS_BRRS_TxTiming; /* Transmission timing of CSI-RS / BRRS, Value:0 -> 3
                                        m E {0,1,2,3} */
   /* ToDo: following 4 fields can be made as union with an indicator type*/
   uint8_t          CSIRS_BRRS_SymbIdx; /* Indication of OFDM symbol index for CSI-RS / BRRS allocations
                                                         Value:0 -> 3 */
   uint8_t          CSI_BRRS_Indicator; /* If this DCI format allocates either of 
                                      CSI-RS or BRRS transmission Value:0 -> 3 */
   uint8_t          CSIRS_BRRS_ProcInd; /* Process indicator : Value:0 -> 3 */
   uint8_t          numBSI_Reports;  /* Number of BSI reports Value:0 -> 3 */

   Bool        xPUCCH_TxTiming;   /*  field indicates transmission time offset value k.{0, 1, ., 7} */
   uint8_t          freqResIdx_xPUCCH; /* Frequency resource index of xPUCCH for UCI report */
   Bool        beamSwitch;    /* beam switch indication. Value:0 -> 1 */

   uint8_t          SRS_Config; /* Indication of SRS configurations Value:0 -> 3
                              00 : {No SRS request}, 01 : {Config. #0},
                              10 : {Config. #1}, 11 : {Config. #2} */
   Bool        SRS_Symbol;  /* If SRS is requested, 0: SRS transmission on the 13th OFDM symbol
                              and 1: SRS transmission on the 14th OFDM symbol */   

   uint8_t          AntPorts_numLayers; 

   Bool        SCID;         /* nSCID is applied for both DMRS in subframe n and
   	                          CSI-RS in subframe n+m Value: 0 -> 1 */
   uint8_t          tpcCmd;       /*!< 2 bits for TPC command for xPUCCH, Values: 0->3  */													  
   Bool	      DL_PCRS;      /* DL PCRS 	Value: 0 -> 3 
                                00 : {No PCRS }, 01 : {PCRS on AP 60},
                                10 : {PCRS on AP 61}, 11 : {PCRS on AP 60 and 61}*/

} TfuDciFormatB1Info;

/* DCI format B2: DL xPDCCH */
typedef TfuDciFormatB1Info TfuDciFormatB2Info;
#endif  /* TFU_5GTF */

/** @brief This structure contains the information needed to convey DCI format 0.
 */
typedef struct tfuDciFormat0Info
{
   Bool          isSrGrant;    /*!< This is to aid a counter which prints num successful SR Grants SR_RACH_STATS */
   Bool        isMsg4PdcchWithCrnti; /* This is to aid rgNumMsg4PdcchWithCrnti counter which counts
                                       num of PDCCH scrambled with CRNTI for CRNTI Based contention resolution */
   Bool        hoppingEnbld;  /*!< TRUE, if hopping is enabled */
   uint8_t          rbStart;       /*!< Starting RB */
   uint8_t          numRb;         /*!< Number of RBs */
#ifdef TFU_UPGRADE
   uint32_t         riv;           /*!< RIV as defined in 213 
                               section 7.1.6.3 - used only for
                               SPS release */
#endif 
   uint8_t          hoppingBits;   /*!< Hopping bits as mentioned in 213 section 8.4 */
   uint8_t          mcs;           /*!< 5 bits for MCS and RV */
   Bool        ndi;           /*!< 1 bit for new data indicator */
   uint8_t          tpcCmd;        /*!< 2 bits for TPC command */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t          ulIdx;         /*!< UL index applicable only for TDD */
   uint8_t          dai;           /*!< The downlink assignement index */
#endif
/** @} */
   uint8_t          nDmrs;         /*!< 3 bits for DMRS cyclic shift */
   uint8_t          cqiReq;        /*!< 1 bit for CQI non-CA while value 00, 01, 10 and 11 for CA */
   uint8_t          numCqiBit;     /*!< Number of CsiReq Bit(s) Values : 1 or 2(CA)*/
   uint8_t          txAntenna;     /*!< Antenna selection - 213 section 8.7 */

   /* tfu_x_001.main_4: Adding hqProcId */
   uint8_t          hqProcId;    /*!< HARQ process ID */
} TfuDciFormat0Info;

/** @enum TfuAllocType
 * This enum contains the values for the allocation types.
 */
typedef enum
{
   TFU_ALLOC_TYPE_MAP= 1,
   TFU_ALLOC_TYPE_RIV 
} TfuAllocType;

/** @brief This Structure contains either a resource allocation bit map OR a
 * RIV Value as defined in 213 section 7.1.6.
 */
typedef struct tfuAllocMapOrRiv
{
   TfuAllocType      type;   /*!< Allocation type RIV or resAllocMap */
   union
   {

      uint32_t                 riv;   /*!< RIV as defined in 213 section 7.1.6.3. */
      /*tfu_x_001.main_8 - ADD - TFU_RESMAP_CHANGE support */
#ifndef TFU_RESMAP_CHANGE
      uint8_t                  resAllocMap[TFU_MAX_ALLOC_BYTES]; /*!< Resource 
                                      allocation bitmap. LSB aligns with the
                                      LSB of the allocation bits.*/
#else /* TFU_RESMAP_CHANGE */
      TfuAllocMap         allocMap;   /*!< Allocation Map. */
#endif
   }u;
} TfuAllocMapOrRiv;

/** @enum TfuAllocSubType
 * This enum contains the values for the allocation types.
 */
typedef enum
{
   TFU_ALLOC_TYPE_0= 1,
   TFU_ALLOC_TYPE_1 
} TfuAllocSubType;


#ifdef LTEMAC_SPS
/** @brief This Structure contains either a resource allocation bitmap for
 * for resource allocation type 0/1. 
 * TODO: This structure can be made part of all DCI format having alloction
 * type 0/1.
 */
typedef struct tfuAllocMap
{
   TfuAllocSubType           type;       /*!< Allocation type: 0/1 */
  union
  {
     struct 
     {
        uint32_t               allocMask;  /*!< Allocation Mask for RBGs */
     } allocType0;
     struct
     {
        uint8_t                rbgSubset;  /*!< RBG Subset number: Value in set
                                           {0,1,2,3} */
        uint8_t                shift;      /*!< Shift value for RBG mask: Value in 
                                           set {0,1} */
        uint32_t               allocMask;  /*!< Allocation bitmask */
     } allocType1;
  } u; 
} TfuAllocMap;
#endif

/** @brief This structure contains only the allocation information, that is part
 * of DCI format 1. 
 * @details Allocation information also contains parameters necessary
 * for Physical layer to process Downlink data. This structure accompanies the
 * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
 * @sa TfuDciFormat1Info
 */
typedef struct tfuDciFormat1AllocInfo
{
   Bool        isAllocType0;  /*!< TRUE, if allocation type is 0 */
   uint8_t          resAllocMap[TFU_MAX_ALLOC_BYTES];   /*!< Resource allocation bitmap.
                                                LSB aligns with the LSB of the
                                                allocation bits.*/
   uint8_t          harqProcId;    /*!< HARQ process ID */
   Bool        ndi;           /*!< 1 bit for new data indicator */
   uint8_t          mcs;           /*!< 5 bits for MCS */
   uint8_t          rv;            /*!< Redundancy version */
} TfuDciFormat1AllocInfo;

/** @brief This structure contains the information needed to convey DCI format 1.
 * @details Allocation information is separated from the other control
 * information carried in this format. This separation is needed as Data must
 * also carry some control information, essentially, allocation information
 * along with it, in order to aid physical layer processing of the data. 
 */
typedef struct tfuDciFormat1Info
{
   TfuDciFormat1AllocInfo  allocInfo;     /*!< Allocation Information to map data on PDSCH */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t                      dai;           /*!< The downlink assignement index */
#endif
/** @} */
   uint8_t                      tpcCmd;        /*!< 2 bits for TPC command */
} TfuDciFormat1Info;

/** @brief This structure contains the TB level DCI signalling
  * parameters in case of DCI formats 2 and 2A */
typedef struct tfuDciFormatTbInfo
{
   Bool                   ndi; /*!< New data indicator */
   uint8_t                     rv;  /*!< Redundancy version Indicator */
   uint8_t                     mcs; /*!< 5 bits for MCS */
}TfuDciFormatTbInfo;
/** @} */
/** @brief This structure contains only the allocation information, that is part
  * of DCI format 2A. 
  * @details Allocation information also contains parameters necessary
  * for Physical layer to process Downlink data. This structure accompanies the
  * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
  * @sa TfuDciFormat2AInfo
  */
typedef struct tfuDciFormat2AAllocInfo
{
   Bool        isAllocType0;  /*!< TRUE, if allocation type is 0 */
      /*tfu_x_001.main_8 - ADD - TFU_RESMAP_CHANGE support */
#ifndef TFU_RESMAP_CHANGE
   uint8_t          resAllocMap[TFU_MAX_ALLOC_BYTES];   /*!< Resource allocation bitmap.
                                                LSB aligns with the LSB of the
                                                allocation bits.*/
#else
   TfuAllocMap allocMap;   /*!< Allocation Map */
#endif /* TFU_RESMAP_CHANGE */
   uint8_t          harqProcId;    /*!< HARQ process ID */
   TfuDciFormatTbInfo tbInfo[2]; /*!< DCI Format 2/2A info per TB */
   uint8_t          precoding;     /*!< Precoding information bit */
   Bool        transSwap;     /*!< Transport block to codeword swap flag */
} TfuDciFormat2AAllocInfo;


/** @brief This structure contains the information carried by DCI format 2A. 
 * @details It carries the allocation information and other control information.
 */
typedef struct tfuDciFormat2AInfo
{
   TfuDciFormat2AAllocInfo  allocInfo;     /*!< Allocation Information to map data on PDSCH */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t                       dai;           /*!< The downlink assignement index */
#endif
/** @} */
   uint8_t                       tpcCmd;        /*!< 2 bits for TPC command */
} TfuDciFormat2AInfo;

/** @brief This structure contains only the allocation information, that is part
  * of DCI format 2. 
  * @details Allocation information also contains parameters necessary
  * for Physical layer to process Downlink data. This structure accompanies the
  * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
  * @sa TfuDciFormat2Info
  */
typedef struct tfuDciFormat2AllocInfo
{
   Bool                   isAllocType0;  /*!< TRUE, if allocation type is 0 */
#ifndef TFU_RESMAP_CHANGE
   uint8_t                     resAllocMap[TFU_MAX_ALLOC_BYTES]; /*!< Resource 
                                              allocation bitmap. LSB aligns
                                              with the LSB of the allocation
                                              bits.*/
#else
   TfuAllocMap             allocMap;   /*!< Allocation Map.*/
#endif
/** @} */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t                     dai;           /*!< The downlink assignement index */
#endif
/** @} */
   Bool                   transSwap;     /*!< Transport block to codeword swap
                                              flag */
   uint8_t                     precoding;     /*!< Precoding information bit */
   uint8_t                     harqProcId;    /*!< HARQ process ID */
   TfuDciFormatTbInfo     tbInfo[2];     /*!< DCI Format 2/2A info per TB */
} TfuDciFormat2AllocInfo;


 /** @brief This structure contains the information carried by DCI format 2.
 * @details Allocation information is separated from the other control
 * information carried in this format. This separation is needed as Data must
 * also carry some control information, essentially, allocation information
 * along with it, in order to aid physical layer processing of the data. 
  */
typedef struct tfuDciFormat2Info
{
   TfuDciFormat2AllocInfo  allocInfo;     /*!< Allocation Information to map 
                                               data on PDSCH */
   uint8_t                      tpcCmd;        /*!< 2 bits for TPC command */
/** @{ */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t                      dai;            /*!< The downlink assignment index */
#endif
/** @} */
/** @} */
} TfuDciFormat2Info;

 /** @brief This structure contains the information carried in DCI format 3.
  */
typedef struct tfuDciFormat3Info
{
   uint8_t          tpcCmd[TFU_MAX_2BIT_TPC]; /*!< 2 bits for TPC command. 
                                           LSB corresponds to the 1st bit of the 1st
                                          byte.*/
   uint8_t          isPucch;                  /*!< 1 if the TPC meant for PUCCH, 0 if the TPC meant for PUSCH */                                          

} TfuDciFormat3Info;

 /** @brief This structure contains the information carried by DCI format 3A.
  */
typedef struct tfuDciFormat3AInfo
{
   uint8_t          tpcCmd[TFU_MAX_1BIT_TPC]; /*!< 1 bit for TPC command. 
                                           LSB corresponds to the 1st bit of the
                                          1st byte.*/
   uint8_t          isPucch;                  /*!< 1 if the TPC meant for PUCCH, 0 if the TPC meant for PUSCH */                                          
} TfuDciFormat3AInfo;



 /** @brief This structure contains only the allocation information that is part
  * of DCI format 1D. 
  * @details Allocation information also contains parameters necessary
  * for Physical layer to process Downlink data. This structure accompanies the
  * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
  * @sa TfuDciFormat1dInfo
  */
typedef struct tfuDciFormat1dAllocInfo
{
   Bool              isLocal;       /*!< TRUE, if localised VRB */
   TknUInt8             nGap2;         /*!< 1 indicates Ngap=Ngap,1 and 2 indicates Ngap,2 */
   TfuAllocMapOrRiv  alloc;         /*!< Allocation represented as a bit-map or RIV */   
   uint8_t                mcs;           /*!< 5 bits for MCS */
   uint8_t                rv;            /*!< Redundancy version */
   uint8_t                tPmi;          /*!< PMI */
} TfuDciFormat1dAllocInfo;

 /** @brief This structure contains the information carried by DCI format 1D.
 * @details Allocation information is separated from the other control
 * information carried in this format. This separation is needed as Data must
 * also carry some control information, essentially, allocation information
 * along with it, in order to aid physical layer processing of the data. 
  */
typedef struct tfuDciFormat1dInfo
{
   TfuDciFormat1dAllocInfo allocInfo;     /*!< Allocation information */
   uint8_t                      harqProcId;    /*!< HARQ process ID */
   Bool                    ndi;           /*!< 1 bit for new data indicator */
   uint8_t                      tpcCmd;        /*!< 2 bits for TPC command */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t                      dai;           /*!< The downlink assignement index */
#endif
/** @} */
   uint8_t                      dlPwrOffset;   /*!< Downlink power offset */
} TfuDciFormat1dInfo;


 /** @brief This structure contains only the allocation information, that is part
  * of DCI format 1C. 
  * @details Allocation information also contains parameters necessary
  * for Physical layer to process Downlink data. This structure accompanies the
  * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
  * @sa TfuDciFormat1CInfo
  */
typedef struct tfuDciFormat1cInfo
{
   TknUInt8             nGap2;         /*!< 1 indicates Ngap=Ngap,1 and 2 indicates Ngap,2 */
   TfuAllocMapOrRiv  alloc;         /*!< Allocation represented as a bit-map or RIV */   
   uint8_t                iTbs;          /*!< 5 bits for I-tbs */
} TfuDciFormat1cInfo;


 /** @brief This structure contains only the allocation information, that is part
  * of DCI format 1 B. 
  * @details Allocation information also contains parameters necessary
  * for Physical layer to process Downlink data. This structure accompanies the
  * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
  * @sa TfuDciFormat1BInfo
  */
typedef struct tfuDciFormat1bAllocInfo
{
   Bool              isLocal;       /*!< TRUE, if localised VRB */
   TknUInt8             nGap2;         /*!< 1 indicates Ngap=Ngap,1 and 2 indicates Ngap,2 */
   TfuAllocMapOrRiv  alloc;         /*!< Allocation represented as a bit-map or RIV */
   uint8_t                mcs;           /*!< 5 bits for MCS */
   uint8_t                rv;            /*!< Redundancy version */
   uint8_t                harqProcId;    /*!< HARQ process ID */
   Bool              ndi;           /*!< 1 bit for new data indicator */
   uint8_t                tPmi;          /*!< PMI */
   Bool              pmiCfm;        /*!< PMI confirmation bit */
} TfuDciFormat1bAllocInfo;


 /** @brief This structure contains the information that is carried in DCI
  * format 1A for a PDCCH order. 
  */
typedef struct tfuPdcchOrderInfo
{
   uint8_t          preambleIdx;    /*!< Dedicated preamble index */
   uint8_t          prachMaskIdx;   /*!< PRACH Mask index used to determine the
                                    subframe to be used for RACH */
} TfuPdcchOrderInfo;

 /** @brief This structure contains only the allocation information that is part
  * of DCI format 1A. 
  * @details Allocation information also contains parameters necessary
  * for Physical layer to process Downlink data. This structure accompanies the
  * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
  * @sa TfuDciFormat1AInfo
  */
typedef struct tfuDciFormat1aAllocInfo
{
   Bool              isLocal;       /*!< TRUE, if localised VRB */
   TknUInt8             nGap2;         /*!< 1 indicates Ngap=Ngap,1 and 2 indicates Ngap,2 */
   TfuAllocMapOrRiv  alloc;         /*!< Allocation carries RIV or a bitmap */
   uint8_t                mcs;           /*!< 5 bits for MCS */
   uint8_t                rv;            /*!< Redundancy version */
   TknUInt8             harqProcId;    /*!< HARQ process ID. Reserved
                                         if RA-RNTI, P-RNTI, SI-RNTI is used */
   Bool              ndi;           /*!< 1 bit for new data indicator */
} TfuDciFormat1aAllocInfo;

 /** @brief This structure contains the information carried in DCI format 1A
  * when it is NOT used for a PDCCH order. 
  */
typedef struct tfudciformat1aPdsch
{
   TfuDciFormat1aAllocInfo allocInfo;     /*!< Allocation information. */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   TknUInt8                   dai;   /*!< The downlink assignement index. Reserved
                                   if RA-RNTI, P-RNTI, SI-RNTI is used. */
#endif
/** @} */
   uint8_t                      tpcCmd;        /*!< 2 bits for TPC command. */
   Bool                 isTBMsg4; /*!< This field is added to help counting num of Msg4 Tx failures */
} Tfudciformat1aPdsch;
/*tfu_x_001.main_6 - Added for SPS support*/
/** @brief This structure contains the information needed to convey DCI format 1A.
 * @details Format can possibly carry PDSCH allocation or information needed for
 * a PDCCH order, used to initiate a RACH procedure in cases where UE looses
 * synchronization with eNodeB.
 */
typedef struct tfuDciFormat1aInfo
{
   /*tfu_x_001.main_6- Added for SPS support*/
   Bool                 isPdcchOrder;
   union
   {
      TfuPdcchOrderInfo       pdcchOrder;    /*!< PDDCH order information */
      Tfudciformat1aPdsch     pdschInfo;     /*!< PDSCH allocation information */
      /*tfu_x_001.main_6 - Added for SPS support*/
   }t;
} TfuDciFormat1aInfo;

/** @brief This structure contains the information needed to convey DCI format 1A.
 * @details Allocation information is separated from the other control
 * information carried in this format. This separation is needed as Data must
 * also carry some control information, essentially, allocation information
 * along with it, in order to aid physical layer processing of the data. 
 */
typedef struct tfuDciFormat1bInfo
{
   TfuDciFormat1bAllocInfo allocInfo;     /*!< Allocation information */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t          dai;                       /*!< The downlink assignement index */
#endif
/** @} */
   uint8_t                      tpcCmd;        /*!< 2 bits for TPC command */
} TfuDciFormat1bInfo;
#ifdef EMTC_ENABLE
/** @brief This structure contains the information needed to convey DCI format 6-0A.
 */
typedef struct tfuDciFormat60aInfo
{
   Bool              hoppingEnbld;  /*!< TRUE, if hopping is enabled */
   uint32_t               riv;           /*!<9 bits, type2 allocation, RIV as defined in 213 section 7.1.6.3. */
   uint8_t                mcs;           /*!< 4 bits for MCS and RV */
   uint8_t                rep;           /*!< 2 bits, repetion number */
   uint8_t                hqProcId;    /*!< HARQ process ID */
   Bool              ndi;           /*!< 1 bit for new data indicator */
   uint8_t                rv;            /*!< 2 bits - Redundancy version */
   uint8_t                tpcCmd;        /*!< 2 bits for TPC command */
#ifdef TFU_TDD
/* TODO_Mavericks: Check if DAI is required for FDD also */
   uint8_t               ulIdx;         /*!< UL index applicable only for TDD */
   uint8_t               dai;           /*!< The downlink assignement index */
#endif
   Bool            cqiReq;        /*!< 1 bit for CQI */
   Bool            isSrs;         /*!< 1 bit for SRS */
   uint8_t              dciRep;        /*!< 2 bits for DCI Subframe repetition */      
} TfuDciFormat60aInfo;
/** @brief This structure contains only the allocation information, that is part
 * of DCI format 6-1A. 
 * @details Allocation information also contains parameters necessary
 * for Physical layer to process Downlink data. This structure accompanies the
 * Data that is sent from MAC to PHY in the TfUiTfuDatReq primitive.
 * @sa TfuDciFormat61AInfo
 */
typedef struct tfuDciFormat61AllocInfo
{
   uint32_t         riv;           /*!<9 bits, type2 allocation, RIV as defined in 213 section 7.1.6.3. */
/* TODO_EMTC: Add start RB, number of RB, Narrow band if required */
   uint8_t          harqProcId;    /*!< HARQ process ID */
   Bool        ndi;           /*!< 1 bit for new data indicator */
   uint8_t          mcs;           /*!< 4 bits for MCS and RV */
   uint8_t          rv;            /*!< 2 bits - Redundancy version */
	uint32_t 		   scramblerInitValue; /*!< Scrambler init(Cinit) value as per 36.211 */
} TfuDciFormat61AllocInfo;

 /** @brief This structure contains the information carried in DCI format 6-1A
  * when it is NOT used for a PDCCH order. 
  */
typedef struct tfudciformat61aPdsch
{
   TfuDciFormat61AllocInfo allocInfo;   /* Allocation info for 6-1A */
   Bool                hoppingEnbld;  /*!< TRUE, if hopping is enabled */
   uint8_t                  rep;           /*!< 2 bits, repetion number */
   uint8_t                  tpcCmd;        /*!< 2 bits for TPC command */
#ifdef TFU_TDD
/* TODO_Maveri: Check if DAI is required for FDD also */
   uint8_t                  dai;           /*!< The downlink assignement index */
#endif
   uint8_t                  antPortAndScrId; /*!< 2 bits - Antenna port(s) and scrambling identity  */
   Bool                isSrs;           /*!< 1 bit for SRS */
   uint8_t                  tPmi;            /*!< 2 0r 4 bits:  TPMI information for precoding  */
   uint8_t                  pmiCfm;          /*!<  1 bit, PMI confirmation for precoding  */
   uint8_t                  harqAckOffst;    /*!< 2 bits, HARQ-ACK resource offset */ 
   uint8_t                  dciRep;          /*!< 2 bits for DCI Subframe repetition */   
   Bool                isTBMsg4;        /*!< This field is added to help counting num of Msg4 Tx failures */
} Tfudciformat61aPdsch;
typedef struct tfuPdcchOrderInfoEmtc
{
   uint32_t         riv;             /*!<9 bits, type2 allocation, RIV as defined in 213 section 7.1.6.3. */
   uint8_t          preambleIdx;    /*!< Dedicated preamble index */
   uint8_t          prachMaskIdx;   /*!< PRACH Mask index used to determine the
                                    subframe to be used for RACH */
   uint8_t          ceLevel;        /*!< 2 bits, Starting CE level */
} TfuPdcchOrderInfoEmtc;

/** @brief This structure contains the information needed to convey DCI format 6-1A.
 * @details Format can possibly carry PDSCH allocation or information needed for
 * a PDCCH order, used to initiate a RACH procedure in cases where UE looses
 * synchronization with eNodeB.
 */
typedef struct tfuDciFormat61aInfo
{
   Bool                 isPdcchOrder;
   union
   {
      TfuPdcchOrderInfoEmtc   pdcchOrder;    /*!< PDDCH order information */
      Tfudciformat61aPdsch     pdschInfo;     /*!< PDSCH allocation information */
      /*tfu_x_001.main_6 - Added for SPS support*/
   }t;
} TfuDciFormat61aInfo;


typedef struct tfuDciFormatForEmtcBcch
{
   uint8_t  mcs;
   uint16_t numOfRb;
   uint16_t rbStart;
}TfuDciFormatForEmtcBcch;

typedef struct tfuDirectIndication
{
   uint8_t  directInd; /*!< 8 bit direct Indication Information*/
}TfuDirectIndication;

typedef struct tfuDciFormat62AllocInfo
{
  uint8_t riv;                         /*!< Narrow Band Index*/ 
  uint8_t mcs;                         /*!<MCS */
  uint32_t         scramblerInitValue; /*!< Scrambler init(Cinit) value as per 36.211 */
  uint8_t          rv;                  /*!<Redundancy version */ 
}TfuDciFormat62AllocInfo;

typedef struct tfudciformat62Pdsch
{
    TfuDciFormat62AllocInfo format62AllocInfo;
    uint8_t repNum;                       /*!< 3 bits, repetion number */
    uint8_t dciSubRepNum;                 /*!< 2 bits for DCI Subframe repetition */
     

}Tfudciformat62Pdsch;

typedef struct tfuDciFormat62Info
{
   Bool                 isPaging;   
   union
   {
      TfuDirectIndication   directIndication;    /*!< Direct Indication information */
      Tfudciformat62Pdsch   pdschInfo;           /*!< Paging information */
   }t;
} TfuDciFormat62Info;

#endif /* EMTC_ENABLE */




/** @brief This structure contains the information needed to convey the possible DCI
 * formats. The following are the supported formats: 
 * -# Format 0    - used for PUSCH scheduling
 * -# Format 1    - used for PDSCH scheduling
 * -# Format 1A   - used for compact scheduling of PDSCH or RA procedure
 *                   intitiated by a PDCCH order.
 * -# Format 1B   - used for compact scheduling of PDSCH with precoding
 *                   information.
 * -# Format 1C   - used for very compact scheduling of PDSCH.
 * -# Format 1D   - used for compact scheduling for PDSCH with precoding
 *                   information with power offset.
 * -# Format 2    - used for PDSCH scheduling
 * -# Format 2A   - used for PDSCH scheduling
 * -# Format 3    - used for power control 
 * -# Format 3A   - used for power control 
 */
typedef struct tfuDciInfo
{
   TfuDciFormat       dciFormat;       /*!< DCI format */
   union
   {
      TfuDciFormat0Info  format0Info;  /*!< Format 0 information */
      TfuDciFormat1Info  format1Info;  /*!< Format 1 information */
      TfuDciFormat1aInfo format1aInfo; /*!< Format 1A information */
      TfuDciFormat1bInfo format1bInfo; /*!< Format 1B information */
      TfuDciFormat1cInfo format1cInfo; /*!< Format 1C information */
      TfuDciFormat1dInfo format1dInfo; /*!< Format 1D information */
      TfuDciFormat2Info  format2Info;  /*!< Format 2 information */
      TfuDciFormat2AInfo format2AInfo; /*!< Format 2A information */
      TfuDciFormat3Info  format3Info;  /*!< Format 3 information */
      TfuDciFormat3AInfo format3AInfo; /*!< Format 3A information */
#ifdef EMTC_ENABLE
      TfuDciFormat60aInfo format60aInfo; /*!< Format 60a Information */
      TfuDciFormat61aInfo format61aInfo; /*!< Format 61a Information */
      TfuDciFormat62Info  format62Info;  /*!< Formra 62  Information */
#endif /* EMTC_ENABLE */
#ifdef TFU_5GTF
	  TfuDciFormatA1Info  formatA1Info;  /*!< 5GTF: Format A1 information */
	  TfuDciFormatA2Info  formatA2Info;  /*!< 5GTF: Format A1 information */
	  TfuDciFormatB1Info  formatB1Info;  /*!< 5GTF: Format B1 information */
	  TfuDciFormatB2Info  formatB2Info;  /*!< 5GTF: Format B2 information */
#endif
   } u;
} TfuDciInfo;

/** @brief This structure contains the information needed to convey the start and size
 * of the sub-band in the CQI report. 
 */
typedef struct tfuSubbandInfo
{
   uint8_t          rbStart;       /*!< Starting RB */
   uint8_t          numRb;         /*!< Number of RBs */
} TfuSubbandInfo;

/** @brief This structure is used to convey the sub-band CQI reported. 
 */
typedef struct tfuSubbandDlCqiInfo
{
   TfuSubbandInfo         subband;             /*!< Sub-band information */
   uint8_t                     cqiIdx[TFU_MAX_TB];  /*!< Sub-band CQI index per CW */
} TfuSubbandDlCqiInfo;
/** @} */


/** @brief This structure is used to convey the subbannd CQI reported. 
 */
typedef struct tfuSubbandCqiInfo
{
   TfuSubbandInfo subband;    /*!< Sub-band information */
   uint8_t             cqiIdx;     /*!< CQI index */
} TfuSubbandCqiInfo;
/** @brief This structure conveys the CCE information.
 */
typedef struct tfuPdcchCceInfo
{
   uint8_t             cceIdx;   /*!< CCE index */
   CmLteAggrLvl   aggrLvl;  /*!< Aggregation level */
} TfuPdcchCceInfo;

 /** @brief This structure maps to the CQI mode 10. The report can either
  * convey a Rank index or a wide-band CQI. 
  */
typedef struct tfuCqiPucchMode10
{
   TfuRptType       type;   /*!< Either RI or CQI reported */
   union
   {
      uint8_t        ri;    /*!< One RI assuming transmission on S sub-bands */
      uint8_t        cqi;   /*!< Wide-band CQI assuming transmission on S sub-bands */
   }u;
} TfuCqiPucchMode10;


 /** @brief This structure contains a wide-band CQI, PMI, and optionally a
  * wide-band differential CQI. 
  */
typedef struct tfuCqiMode11Cqi
{
   uint8_t       cqi;           /*!< Wide-band CQI */
   uint8_t       pmi;           /*!< PMI */
   TknUInt8    wideDiffCqi;   /*!< Wide-band differential CQI */
} TfuCqiMode11Cqi;

 /** @brief This structure maps to the CQI mode 11. The report can either
  * convey a Rank index or a CQI report. 
  */
typedef struct tfuCqiPucchMode11
{
   TfuRptType       type;   /*!< Either RI or CQI reported */
   union
   {
      uint8_t                ri;    /*!< One RI assuming transmission on S sub-bands */
      TfuCqiMode11Cqi   cqi;   /*!< CQI assuming transmission on S sub-bands */
   }u;
} TfuCqiPucchMode11;

 /** @brief This structure contains the sub-band CQI for mode 2-0. 
  */
typedef struct tfuCqiMode20SubCqi
{
   uint8_t      cqi;      /*!< CQI value */
   uint8_t       l;       /*!< L bit sub-band label */
} TfuCqiMode20SubCqi;

 /** @brief This structure contains Mode 20 CQI report. It can either be a
  * wide-band CQI or a sub-band CQI. 
  */
typedef struct tfuCqiMode20Cqi
{
   Bool     isWideband;             /*!< TRUE, if wide-band CQI is reported */
   union
   {
      uint8_t                 wideCqi;   /*!< Single Wide-band CQI */
      TfuCqiMode20SubCqi subCqi;    /*!< Sub-band CQI */
   }u;
} TfuCqiMode20Cqi;


 /** @brief This structure maps to CQI mode 20. The report either contains a
  * Rank Index or a CQI report. 
  */
typedef struct tfuCqiPucchMode20
{
   TfuRptType       type;   /*!< Either RI or CQI reported */
   union
   {
      uint8_t                ri;   /*!< One RI assuming transmission on S sub-bands */
      TfuCqiMode20Cqi   cqi;  /*!< CQI assuming transmission on S sub-bands */
   }u;
} TfuCqiPucchMode20;

 /** @brief This structure contains the sub-band CQI report for mode 2-1. 
  * It carries a CQI, 'L' bit sub-band label, and optionally a differential CQI.
  */
typedef struct tfuCqiMode21SubCqi
{
   uint8_t       cqi;     /*!< CQI */
   uint8_t       l;       /*!< L bit sub-band label */
   TknUInt8    diffCqi; /*!< Optional Differential CQI */
} TfuCqiMode21SubCqi;


 /** @brief This structure contains the wide-band CQI report for mode 2-1.
  * It carries a wide-band CQI, PMI and optionally a differential CQI. 
  */
typedef struct tfuCqiMode21WideCqi
{
   uint8_t       cqi;        /*!< CQI */
   uint8_t       pmi;        /*!< PMI */
   TknUInt8    diffCqi;    /*!< Optional Differential CQI */
} TfuCqiMode21WideCqi;


 /** @brief This structure conveys CQI report for mode 2-1. The reported value
  * can either be wide-band or sub-band. 
  */
typedef struct tfuCqiMode21Cqi
{
   Bool     isWideband;             /*!< TRUE, if wide-band CQI is reported. */
   union
   {
      TfuCqiMode21WideCqi wideCqi;  /*!< Single Wide-band CQI */
      TfuCqiMode21SubCqi  subCqi;   /*!< Sub-band CQI. */
   }u;
} TfuCqiMode21Cqi;


 /** @brief This structure maps to the CQI reporting mode 2-1. The report either
  * conveys a Rank Indicator or a CQI report. 
  */
typedef struct tfuCqiPucchMode21
{
   TfuRptType       type;   /*!< Either RI or CQI reported */
   union
   {
      uint8_t                ri;   /*!< One RI assuming transmission on S sub-bands */
      TfuCqiMode21Cqi   cqi;  /*!< CQI assuming transmission on S sub-bands */
   }u;
} TfuCqiPucchMode21;


/** @brief This structure conveys the Downlink CQI reported on PUCCH.
 */
typedef struct tfuDlCqiPucch
{
   uint8_t                cellIdx;
   TfuDlCqiPucchMode mode;          /*!< PUSCH CQI mode */
   union
   {
      TfuCqiPucchMode10 mode10Info; /*!< Mode 1-0 information */
      TfuCqiPucchMode11 mode11Info; /*!< Mode 1-1 information */
      TfuCqiPucchMode20 mode20Info; /*!< Mode 2-0 information */
      TfuCqiPucchMode21 mode21Info; /*!< Mode 2-1 information */
   }u;
} TfuDlCqiPucch;


 /** @brief This structure carries sub-band information and PMI, in the CQI
  * reporting format 1-2. 
  */
typedef struct tfuSubbandMode12
{
   uint8_t             pmi;             /*!< PMI for this sub-band */
   TfuSubbandInfo subBand;   /*!< Sub-band information start and length */
} TfuSubbandMode12;

/** @brief This structure conveys information present in CQI reporting mode 1-2 received
 * over PUSCH. 
 */
typedef struct tfuCqiPuschMode12
{
   uint8_t               numSubband;                     /*!< Number of sub-bands */
   uint8_t               cqiIdx[TFU_MAX_TB];             /*!< Single Wide-band CQI per
                                                     codeword */
   TfuSubbandMode12 subbandArr[TFU_MAX_DL_SUBBAND]; /*!< Sub-band information array */
} TfuCqiPuschMode12;


/** @brief This structure conveys information present in CQI reporting mode 2-0 received
 * over PUSCH. This mode is for UE selected sub-band feedback. 
 */
typedef struct tfuCqiPuschMode20
{
   uint8_t             cqiIdx;                         /*!< CQI index transmission
                                                    over M sub-bands */
   uint8_t             wideBandCqi;                    /*!< Wide-band CQI index, that is,
                                                    transmission over S sub-bands */
   uint8_t             numSubband;                     /*!< Number of sub-bands */
   TfuSubbandInfo subbandArr[TFU_MAX_DL_SUBBAND]; /*!< Sub-band information array */
} TfuCqiPuschMode20;

/** @brief This structure conveys information present in CQI reporting mode 2-2 received
 * over PUSCH.
 */
typedef struct tfuCqiPuschMode22
{
   uint8_t                cqi[TFU_MAX_TB];                 /*!< CQI index over M preferred
                                                         Sub-bands */
   uint8_t                pmi;                             /*!< PMI preferred PMI over M
                                                         sub-bands */
   uint8_t                wideBandCqi[TFU_MAX_TB];         /*!< Wide-band CQI */
   uint8_t                wideBandPmi;                     /*!< PMI */
   uint8_t                numSubband;                      /*!< Number of sub-bands, that is, M mentioned in 213, each of size k */
   TfuSubbandInfo    subbandArr[TFU_MAX_DL_SUBBAND];  /*!< Sub-band information array */
} TfuCqiPuschMode22;


/** @brief This structure holds CQI report information in mode 3-0
 */
typedef struct tfuSubbandMode30
{
   uint8_t             cqi;       /*!< CQI for this sub-band */
   TfuSubbandInfo subBand;   /*!< Sub-band information start and length. */
} TfuSubbandMode30;


/** @brief This structure conveys information present in CQI reporting mode 3-0
 * received over PUSCH.
 */
typedef struct tfuCqiPuschMode30
{
   uint8_t                wideBandCqi;                     /*!< Wide-band CQI */
   uint8_t                numSubband;                      /*!< Number of sub-bands */
   TfuSubbandMode30  subbandArr[TFU_MAX_DL_SUBBAND];  /*!< Sub-band information array */
} TfuCqiPuschMode30;

/** @brief This structure holds CQI report information in mode 3-1
 */
typedef struct tfuSubbandMode31
{
   uint8_t             cqi[TFU_MAX_TB];  /*!< CQI for this sub-band */
   TfuSubbandInfo subBand;          /*!< Sub-band information start and length */
} TfuSubbandMode31;


/** @brief This structure conveys information present in CQI reporting mode 3-1
 * received over PUSCH.
 */
typedef struct tfuCqiPuschMode31
{
   uint8_t                     pmi;                      /*!< PMI */
   uint8_t                     wideBandCqi[TFU_MAX_TB];  /*!< Wide-band CQI per CW */
   uint8_t                     numSubband;               /*!< Number of sub-bands */
   TfuSubbandDlCqiInfo    subbandCqiArr[TFU_MAX_DL_SUBBAND];  /*!< Array of 
                                                         sub-bands and CQI for
                                                         each of them */
} TfuCqiPuschMode31;

/** @enum TfuDlCqiPuschMode
 * This enum contains values for the CQI reporting modes. 
 */
typedef enum
{
   TFU_PUSCH_CQI_MODE_12,
   TFU_PUSCH_CQI_MODE_20,
   TFU_PUSCH_CQI_MODE_22,
   TFU_PUSCH_CQI_MODE_30,
   TFU_PUSCH_CQI_MODE_31
} TfuDlCqiPuschMode;


/** @brief This structure conveys Downlink CQI information (mode and corresponding
 * CQI) received over PUSCH. 
 */
typedef struct tfuDlCqiPusch
{
   uint8_t                cellIdx;
   TfuDlCqiPuschMode mode;         /*!< PUSCH CQI mode */
   TknUInt8                  ri;            /*!< Rank Indicator for TM 3,4 */
   union
   {
      TfuCqiPuschMode12 mode12Info; /*!< Mode 1-2 information */
      TfuCqiPuschMode20 mode20Info; /*!< Mode 2-0 information */
      TfuCqiPuschMode22 mode22Info; /*!< Mode 2-2 information */
      TfuCqiPuschMode30 mode30Info; /*!< Mode 3-0 information */
      TfuCqiPuschMode31 mode31Info; /*!< Mode 3-1 information */
   }u;
} TfuDlCqiPusch;

/** @brief This structure conveys explicitly the PMI information in case
 *  of PUSCH feedback usage in scheduling. Applicable for TM 4, 5, 6. 
 */
typedef struct tfuDlCqiPuschInfo
{
   TfuDlCqiPuschMode      mode;          /*!< PUSCH CQI mode */
   union
   {
      TfuCqiPuschMode12   mode12Info;    /*!< Mode 1-2 information */
      TfuCqiPuschMode22   mode22Info;    /*!< Mode 2-2 information */
      TfuCqiPuschMode31   mode31Info;    /*!< Mode 3-1 information */
   }u;
} TfuDlCqiPuschInfo;

#ifdef TFU_TDD
/** @enum TfuAckNackMode
 * This enum contains values for modes of HARQ feedback in TDD
 */
typedef enum
{
   TFU_ACK_NACK_BUNDLING,
   TFU_ACK_NACK_MULTIPLEXING,
   TFU_ACK_NACK_SPECIAL_BUNDLING,
   TFU_ACK_NACK_CHANNEL_SELECTION =TFU_ACK_NACK_SPECIAL_BUNDLING,
   TFU_ACK_NACK_FORMAT_3
} TfuAckNackMode;

#else /* else of TFU_TDD */
/** @enum TfuAckNackMode
 * This enum contains values for modes of HARQ feedback in FDD
 */
typedef enum
{
   TFU_UCI_FORMAT_1A_1B,
   TFU_UCI_FORMAT_1B_CS,
   TFU_UCI_FORMAT_3
} TfuAckNackMode;
#endif

#ifdef TFU_UPGRADE

/** @brief This structure conveys the Uplink SRS indication of an UE.
 * @details Uplink SRS from each UE is received by the physical layer and this
 * information is provided to the scheduler.
 */
typedef struct tfuSrsRpt
{
   CmLList                lnk;           /*!< Link to the list of rerports */
   CmLteRnti              ueId;          /*!< CRNTI */
   uint16_t                    dopEst;        /*!< Doppler estimation [0-255] */
   uint16_t                    ta;            /*!< Timing advance for the UE */
   uint8_t                     numRbs;        /*!< Number of RBs to be reported for this UE */
   uint8_t                     rbStart;       /*!< Starting RB to be reported */
   uint8_t                     snr[TFU_MAX_UL_RB]; /*!< snr for each RB */
   Bool                   wideCqiPres;   /*!< Flag to indicate Wide-band CQI */
   uint8_t                     wideCqi;       /*!< Wide-band CQI for an UL channel */
} TfuSrsRpt;


/** @brief This structure conveys the Uplink SRS indication.
 * @details Uplink SRS is received by the physical layer and this
 * information is provided to the scheduler. 
 * This data structure clubs together the UL SRS indications of all the UEs 
 * in the subframe. 
 */
typedef struct tfuSrsIndInfo
{
   CmMemListCp            memCp;         /*!< Memory control point */
   CmLteCellId            cellId;        /*!< Cell ID */
   CmLteTimingInfo        timingInfo;    /*!< Timing information */
   CmLListCp              srsRpt;        /*!< List of SRS indications - TfuSrsRpt */
} TfuSrsIndInfo;

/** @brief This structure contains the Raw CQI information, that is, the CQI report
 * as transmitted by the UE. 
 * @details These bits are interpreted by MAC to derive the following: 
 * -# Wide-band CQI
 * -# Sub-band CQI
 * -# PMI
 * -# RI
 */
typedef struct tfuRawCqiRpt
{
   CmLList                lnk;           /*!< Link to the list of reports */
   CmLteRnti              crnti;         /*!< CRNTI of the UE */
   uint8_t                     numBits;       /*!< Number of valid CQI bits */
#ifdef TFU_5GTF
   uint32_t                    uciPayload;    /*!< Raw UCI bit payload */
#else
   uint8_t                     cqiConfBitMask;
   uint8_t                     cqiBits[TFU_MAX_CQI_BYTES]; /*!< An array of bits
                                              corresponding to the Raw CQI report */
   uint8_t                     ri[CM_LTE_MAX_CELLS]; /*!< Array of bits comprising of a Rank */                                              
#endif
} TfuRawCqiRpt;

/** @brief This structure contains the Raw CQI reports received for the
 * transmitting UEs in a single subframe. 
 * @details Raw CQI corresponds to the actual bits transmitted by the UE, whose
 * interpretation is upto the MAC layer. 
 */
typedef struct tfuRawCqiIndInfo
{
   CmMemListCp            memCp;       /*!< Memory control point */
   CmLteCellId            cellId;      /*!< Cell ID */
   CmLteTimingInfo        timingInfo;  /*!< Timing information */
   CmLListCp              rawCqiRpt;   /*!< List of HQ+CQI+RI indications - TfuRawUciRpt */
} TfuRawCqiIndInfo;
/* 5GTF TODO: Change Interface Structure and API name accordingly */


typedef enum
{
   TFU_PUCCH_HARQ,
   TFU_PUCCH_SR,
   TFU_PUCCH_SRS,
   TFU_PUCCH_CQI,
   TFU_PUCCH_HARQ_SR,
   TFU_PUCCH_HARQ_SRS,
   TFU_PUCCH_HARQ_CQI,
   TFU_PUCCH_HARQ_SR_SRS,
   TFU_PUCCH_HARQ_SR_CQI,
   TFU_PUCCH_SR_SRS,
   TFU_PUCCH_SR_CQI,
   /*tfu_x_001.main_8 - ADD - New Pucch RR Info Combination support */
   /* ccpu00116923 - New Reception Request types for CQI and SRS with SR */
   TFU_PUCCH_HARQ_SR_CQI_SRS,
   TFU_PUCCH_SR_CQI_SRS
#ifdef TFU_5GTF
   ,
   /* 5GTF TODO: Order of encoding should be as per 212 5.2.3.6 */
   TFU_XPUCCH_UCI_INFO,   /*!< HQ+CQI+RI */
   TFU_PUCCH_BSI_BRI
#endif
} TfuUePucchRecpType;

/** @brief This structure is sent from scheduler to PHY in order to request the
 * reception of HARQ information on PUCCH.
 */
typedef struct tfuUePucchHqRecpInfo
{
   TfuAckNackMode         hqFdbkMode;    /*!< Feedback mode in TDD */
   uint8_t                     hqSz;          /*!< Size of feedback in bits */
   uint8_t                     pucchResCnt;   /*!< Number of ACK/NACK responses
                                              received in this subframe [0-4] */
   uint16_t                    hqRes[TFU_MAX_HQ_RES]; /*!< (n^1PUCCH(0-3))
                                              values [0-2047] */
   uint8_t                     a;             /*!< A value to interpret Hq feedback */ 
} TfuUePucchHqRecpInfo;

/** @brief This structure is sent from scheduler to PHY in order to request the
 * reception of SR information on PUCCH.
 */
typedef struct tfuUePucchSrRecpInfo
{
   uint16_t                    n1PucchIdx;    /*!< (n^1PUCCH) value [0-2047] */
} TfuUePucchSrRecpInfo;
 
/** @brief This structure is sent from scheduler to PHY in order to request the
 * reception of CQI information on PUCCH.
 */
typedef struct tfuUePucchCqiRecpInfo
{
   uint16_t                    n2PucchIdx;    /*!< (n^2PUCCH) value[0-1185] */
   uint8_t                     cqiPmiSz;      /*!< DL CQI/PMI size in bits [0-255] */
} TfuUePucchCqiRecpInfo;

/** Different values for UL SRS BW information */
typedef enum tfuUlSrsBwInfo
{
   TFU_ULSRS_BW_0 = 0,   
   TFU_ULSRS_BW_1 = 1,   
   TFU_ULSRS_BW_2 = 2,   
   TFU_ULSRS_BW_3 = 3   
} TfuUlSrsBwInfo;


/** Different values for UL SRS Hoping BW information */
typedef enum tfuUlSrsHoBwInfo
{
   TFU_ULSRS_HOP_BW_0 = 0,   
   TFU_ULSRS_HOP_BW_1 = 1,   
   TFU_ULSRS_HOP_BW_2 = 2,   
   TFU_ULSRS_HOP_BW_3 = 3   
} TfuUlSrsHoBwInfo;

/** Different values for UL SRS Cyclic Shift information */
typedef enum tfuUlSrsCycShiftInfo
{
   TFU_ULSRS_CYSHIFT_0 = 0,   
   TFU_ULSRS_CYSHIFT_1 = 1,   
   TFU_ULSRS_CYSHIFT_2 = 2,   
   TFU_ULSRS_CYSHIFT_3 = 3,   
   TFU_ULSRS_CYSHIFT_4 = 4,   
   TFU_ULSRS_CYSHIFT_5 = 5,   
   TFU_ULSRS_CYSHIFT_6 = 6,   
   TFU_ULSRS_CYSHIFT_7 = 7   
} TfuUlSrsCycShiftInfo;

 
/** @brief This structure is sent from scheduler to PHY in order to request the
 * reception of SRS information on PUCCH.
 */
typedef struct tfuUePucchSrsRecpInfo
{
   TfuUlSrsBwInfo  srsBw;         /*!< UE specific SRS Bandwidth (B,srs)
                                              parameter. Given by RRC[0-3] */
   uint8_t                     nRrc;          /*!< Frequency Domain position given by
                                              RRC per UE.[0-23]*/
   TfuUlSrsHoBwInfo  srsHopBw;      /*!< SRS Hopping BW given by RRC per
                                              UE (b,Hop)[0-3] */
   uint8_t                     transComb;     /*!< Frequency location given by RRC
                                              per UE (k,TC) [0-1] */
   uint16_t                    srsCfgIdx;     /*!< (Isrs) - defines SRS periodicity
                                              and subframe offset,given by RRC
                                              [0-1023] */
   TfuUlSrsCycShiftInfo   srsCyclicShft; /*!< (n^cs,srs) given by RRC [0-7] */
   /*Need to add uint8_t AntPort for Rel10 when SRS is supported */
} TfuUePucchSrsRecpInfo;

#ifdef TFU_5GTF
/** @brief This structure is sent from scheduler to PHY in order to request the
 * reception of BSI BRI information on PUCCH.
 */
typedef struct tfuUePucchBsiBriRecpInfo
{
   uint8_t                     puschFlag;  /*!< UCI is carried in xPUSCH or not
                                           0 -> UCI is in xPUCCH, 
                                           1 -> UCI is in xPUSCH */
   uint8_t                     pucchIndex; /*!< Frequency resource index of xPUCCH 
                                           for UCI report. [0-15] */
   uint8_t                     SCID;       /*!< SCID : 0 or 1 */
   uint8_t                     bsiRpt;     /*!< total num of BSI report[0,1,2,4] */
   uint8_t                     briRpt;     /*!< total num of BRI report[0,1,2,4] */
} TfuUePucchBsiBriRecpInfo;

/** @brief This structure is sent from scheduler to PHY in order to request the
 * reception of UCI (CQI, HQ, RI) information on PUCCH.
 */
typedef struct tfuUePucchUciRecpInfo
{
   uint8_t                     numBits;       /*!< 1-22, 1-Only HQ, 5-Only CQI/RI, 6- both HQ CQI/RI */
   uint8_t                     pucchIndex;    /*!< 0-15, PUCCH Freq Res Idx */
   uint8_t                     SCID;          /*!< SCID : 0 or 1 */
} TfuUePucchUciRecpInfo;
#endif

/** @brief This structure is sent from scheduler to PHY in order to request the
  * reception of UCI on PUCCH. This may contain the following:
  * -# HARQ
  * -# SR
  * -# SRS
  * -# CQI
  * -# HARQ+SR
  * -# HARQ+SRS
  * -# HARQ+CQI
  * -# HARQ+SR+SRS
  * -# HARQ+SR+CQI
  * -# SR+SRS
  * -# SR+CQI  
  */
typedef struct tfuUePucchRecpReq
{
   TfuUePucchRecpType     uciInfo;       /*!< Bitmask for the combination of UCI
                                              to be received */
   TfuUePucchCqiRecpInfo  cqiInfo;       /*!< Info needed to receive CQI. Valid
                                              if CQI is included in uciInfo */
   TfuUePucchSrRecpInfo   srInfo;        /*!< Info needed to receive SR. Valid
                                              if SR is included in uciInfo */
   TfuUePucchHqRecpInfo   hqInfo;        /*!< Info needed to receive Harq-Valid
                                              if HQ is included in uciInfo */
   TfuUePucchSrsRecpInfo  srsInfo;       /*!< Info needed to receive SRS. Valid
                                              if SRS is included in uciInfo */
#ifdef TFU_5GTF
   TfuUePucchBsiBriRecpInfo  bsiBriInfo; /*!< Info needed to receive BSI_BRI. Valid
                                              if BSI_BRI is included in uciInfo */
   TfuUePucchUciRecpInfo     uciPduInfo; /*!< Info needed to receive HQ/CQI/RI.*/
#endif                                              
#ifdef EMTC_ENABLE
	uint16_t							format1aRepNumber;
	uint16_t							format2aRepNumber;
	uint32_t							catMScramblerInitVal;
#endif
} TfuUePucchRecpReq;

typedef enum
{
   TFU_PUSCH_DATA,
   TFU_PUSCH_DATA_SRS,
   TFU_PUSCH_DATA_HARQ,
   TFU_PUSCH_DATA_HARQ_SRS,
   TFU_PUSCH_DATA_CQI,
   TFU_PUSCH_DATA_CQI_SRS,
   TFU_PUSCH_DATA_CQI_HARQ,
   TFU_PUSCH_DATA_CQI_HARQ_SRS
#ifdef TFU_5GTF
   /* Message types possible on xPUCCH and xPUSCH : UL Data, UCIs: HARQ-ACK, SR, BSI, BRI, CQI/PMI/RI etc  */
   /* for now only TFU_PUSCH_DATA can be considered */ 
#endif
} TfuUePuschRecpType;

 /** @brief This structure is sent from scheduler to PHY in order to request the
  * reception of UEs data on ULSCH.
  */
typedef struct tfuUeUlSchRecpInfo
{
   uint16_t                    size;         /*!< Length of the Data in bytes*/
   /* ToDo : rbStart and numRb is kept for legacy, need to remove later */
   uint8_t                     rbStart;      /*!< Start of Resource Block of 
                                             allocation */
   uint8_t                     numRb;        /*!< Number of RBs allocated */
   TfuModScheme           modType;      /*!< Modulation order */
   uint8_t                     nDmrs;        /*!< 3 bits for DMRS cyclic shift */
   Bool                   hoppingEnbld; /*!< TRUE, if hopping is enabled */
   uint8_t                     hoppingBits;  /*!< Hopping bits as mentioned in 
                                             213 8.4 */
   Bool                   isRtx;        /*!< TRUE, if this is a retx */
   Bool                   ndi;          /*!< New Data Indication */   
   uint8_t                     rv;           /*!< Redundancy version */
   uint8_t                     harqProcId;   /*!< HARQ process ID */
   uint8_t                     txMode;       /*!< UE's UL transmission mode: 
                                             0 = SISO/SIMO, 1 = MIMO */
   uint8_t                     crntTxNb;     /*!< Current Harq Tx Cntr of this TB.
                                             Valid if frequency hopping is 
                                             enabled */
   Bool                   nSrs;         /*!< Indicates if RBs for this TB
                                             overlap with SRS configuration. 
                                             FALSE = No overlap,TRUE=overlap */
   uint8_t                     mcs;          /*!< MCS */
#ifdef EMTC_ENABLE
   uint32_t 			  repetitionNumber;
   uint32_t    		  scramblerInitValue;
#endif
#ifdef TFU_5GTF
   uint8_t                     rbgStart; /* The starting resource block group for
                                       this xPUSCH allocation.*/
   uint8_t                     numRbg; /* The number of resource block groups
                                       allocated to this ULSCH grant.*/
   uint8_t                     xPUSCHRange; /* 0 : the stopping of xPUSCH is the 12th symbol,
                                                                    1 : the stopping of xPUSCH is the 13th symbol
                                                                    2 : the stopping of xPUSCH is the final (14th) symbol */
   uint8_t                     nAntPortLayer;/* 0:1 Layer, port 40 1:1 Layer, port 41 2:1 Layer, port 42
                                           3:1 Layer, port 43 4:2 Layers, ports {40, 41} 
                                           5:2 Layers, ports {42, 43} */
   uint8_t                     SCID;        /*SCID : 0 or 1 */
   uint8_t                     PMI;         /* Precoding matrix indicator, see V5G.211 section Table 5.3.3A.2-1 */
   Bool                   uciWoTBFlag;      /* 0: UCI without transmit block
                                               1: with transmit block */
   uint8_t                     beamIndex;   /* Receiving beam index Value: 0->7 */
#endif
} TfuUeUlSchRecpInfo;

/** @brief This structure is sent from scheduler to PHY in order to request the
  * reception of CQI_RI information on PUSCH.
  */
typedef struct tfuUePuschCqiRecpInfo
{
   uint8_t                     reportType;      /*! Type of CSI report.
                                            0 = periodic report.
                                            1 = aperiodic report */
   uint8_t                     cCNum;           /*! The number of CC in the aperiodic report.
                                              Value: 1->5 */
   uint8_t                     cqiPmiSzR1[CM_LTE_MAX_CELLS];   /*!< Size of DL CQI/PMI in bits for
                                             rank 1 report[0-255] */
   uint8_t                     cqiPmiSzRn1[CM_LTE_MAX_CELLS];  /*!< Size of DL CQI/PMI in bits for 
                                             rank > 1 report[0-255] */
   TknUInt8                  riSz[CM_LTE_MAX_CELLS];         /*!< Size of RI in bits[1-2]. Value will
                                         be 0 (present flag-false) in frames
                                         with no RI Occasion*/
   uint8_t                     cqiBetaOff;    /*!< Beta offset for CQI[0-15]. Given  
                                             by RRC */
   uint8_t                     riBetaOff;     /*!< Beta offset for RI[0-15]. Given 
                                             by RRC */
} TfuUePuschCqiRecpInfo;

/** @brief This structure is sent from scheduler to PHY in order to request the
  * reception of HARQ information on PUSCH.
  */
typedef struct tfuUePuschHqRecpInfo
{
   uint8_t                     hqSz;         /*!< Number of feedback bits: 
                                         FDD- 1 or 2 TDD- 1 to 4*/
   uint8_t                     hqBetaOff;     /*!< Beta offset for Harq[0-15]. Given
                                             by RRC */
#ifdef TFU_TDD
   TfuAckNackMode         hqFdbkMode;   /*!< Feedback mode in TDD */
   uint8_t                     nBundled;     /*!< nBundled value intended for PHY */
   uint8_t                     ulDai;        /*!< Ul Dai */
#endif
} TfuUePuschHqRecpInfo;

typedef TfuUePucchSrsRecpInfo TfuUePuschSrsRecpInfo;
 /** @brief This structure is sent from scheduler to PHY in order to request the
  * reception of UEs data on PUSCH. This may contain the following: 
  * -# Data
  * -# Data + SRS
  * -# Data + HARQ
  * -# Data + HARQ + SRS
  * -# Data + CQI/RI
  * -# Data + CQI/RI + SRS
  * -# Data + CQI/RI + HARQ
  * -# Data + CQI/RI + HARQ + SRS
  */
typedef struct tfuUePuschRecpReq
{ 
   TfuUePuschRecpType     rcpInfo;      /*!< Bitmask for the combination of
                                             information to be received */ 
   TfuUeUlSchRecpInfo     ulSchInfo;    /*!< Info needed to receive data pdu
                                             on PUSCH. Valid if data is included
                                             in rcpInfo */
   TfuUePuschCqiRecpInfo  cqiRiInfo;    /*!< Info needed to receive CQI/RI on
                                             PUSCH. Valid if CQI is included
                                             in rcpInfo */
   TfuUePuschHqRecpInfo   hqInfo;       /*!< Info needed to receive Harq on
                                             PUSCH. Valid if HARQ is included
                                             in rcpInfo */
   TfuUePuschSrsRecpInfo  srsInfo;      /*!< Info needed to receive SRS. Valid
                                             if SRS is included in rcpInfo */
   TknUInt8                  initialNSrs;  /*!< Not present in case of only data
                                             [0/1]. 0 = last OFDM symbol is 
                                             not punctured. 1 = last OFDM
                                             symbol is punctured */
   TknUInt8                  initialNumRbs; /*!< Number of RBs for initial
                                              transmission of this TB. Not  
                                              present in case of only data */
} TfuUePuschRecpReq;

/** @enum TfuRecpReqType
 * This enum contains values for types of Reception Request.
 */
typedef enum
{
   TFU_RECP_REQ_PUCCH,
   TFU_RECP_REQ_PUSCH,
   TFU_RECP_REQ_INVLD
} TfuRecpReqType;

/** @brief This structure is a Per UE reception request for either PUCCH or PUSCH
 * data. This contains information needed by PHY to decode the data sent by the
 * UE. 
 */
typedef struct tfuUeRecpReqInfo
{
   CmLList                lnk;           /*!< Link of Linked List */
   CmLteRnti              rnti;          /*!< RNTI of the scheduled UE */
#ifdef TFU_5GTF   
   uint8_t                     groupId;       /*!< UE Group ID */
#endif
   TfuRecpReqType         type;          /*!< Type indicating PUCCH or PUSCH */
   union 
   {
      TfuUePucchRecpReq   pucchRecpReq;  /*!< Reception request for PUCCH */
      TfuUePuschRecpReq   puschRecpReq;  /*!< Reception request for PUSCH */
   }t;
} TfuUeRecpReqInfo;

#else /* TFU_UPGRADE */

/** @enum TfuUciType
 * This enum has values for the various types of UCI information present.
 */
typedef enum
{
   TFU_UCI_SR,       /*!< Only a scheduling request */
   TFU_UCI_HARQ,     /*!< Only HARQ-ACK feedback */
   TFU_UCI_CQI,      /*!< Only CQI (periodic) report */
   TFU_UCI_HARQ_CQI, /*!< HARQ and CQI together */
   TFU_UCI_HARQ_SR   /*!< HARQ and SR together */
} TfuUciType;

/** @enum TfuHqRecpReqType
 * This enum contains values for types of HARQ Reception Request.
 */
typedef enum
{
   TFU_HQ_RECP_REQ_NORMAL,
   TFU_HQ_RECP_REQ_N1PUCCH
} TfuHqRecpReqType;



 /** @brief This structure is sent from Scheduler to PHY in order to request the
  * reception of an UE's data on PUCCH. This may contain the following: 
  * -# HARQ-ACK 
  * -# Scheduling Request (SR)
  * -# HARQ-ACK and SR
  * -# CQI
  * -# CQI and HARQ-ACK
  */
typedef struct tfuUePucchRecpReq
{
    
   TfuHqRecpReqType   hqType;   /*!< Type of HARQ Reception Request */
   union
   {
/** @name TFU_TDD */
/** @{ */
#ifndef TFU_TDD
      uint16_t    nCce;            /*!< Number of first CCE used for transmission of
                                the corresponding DCI assignment. Used to decode
                                HARQ-ACK. */
#else
      uint16_t             nCce[TFU_MAX_M];   /*!< nCCE is an array in the case of TDD
                                           HARQ ACK multiplexing. This value is
                                           needed for the calculation of 
                                           (n^1 PUCCH,i) for i = {0,1,2,3} */
#endif
/** @} */
      uint16_t                 n1Pucch;  /*!< n1 PUCCH for ACK NACK repetition. 
                                              This is the resource that UE must
                                              transmit the HARQ Feedback except
                                              the first one. */
   }t;
  TfuUciType      type;      /*!< Type of Uplink control information - value
                                must be one of the following: 
                                 -# TFU_UCI_SR
                                 -# TFU_UCI_HARQ
                                 -# TFU_UCI_CQI
                                 -# TFU_UCI_HARQ_CQI
                                 -# TFU_UCI_HARQ_SR
                                */
/** @name TFU_TDD  */
/** @{ */
#ifdef TFU_TDD
   uint8_t             multCnt;       /*!< Number of ACK/NACK resources present */
   uint8_t             M;             /*!< Number of elements in the set K - tb.
                                   10.1-1 213 */
   uint8_t             m[TFU_MAX_M];  /*!< Values utilized for TDD HARQ-ACK bundling
                                   mode */
   uint8_t             p[TFU_MAX_M];  /*!< "p" from 10.1 in 213 */
#endif
/** @} */
} TfuUePucchRecpReq;


/** @enum TfuRecpReqType
 * This enum contains values for types of Reception Request.
 */
typedef enum
{
   TFU_RECP_REQ_PUCCH,
   TFU_RECP_REQ_MSG3,
   TFU_RECP_REQ_PUSCH
} TfuRecpReqType;


 /** @brief This structure is sent from scheduler to PHY in order to request the
  * reception of UEs data sent as MSG3 for Random access.
  */
typedef struct tfuUeMsg3RecpReq
{  
   Bool                 hoppingEnbld; /*!< TRUE, if hopping is enabled */
   uint8_t                   rbStart;      /*!< Start of Resource Block of allocation */
   uint8_t                   numRb;        /*!< Number of RBs allocated */
   uint8_t                   mcs;          /*!< 3 bits for MCS */
   Bool                 expCqi;       /*!< TRUE, if CQI report along
                                        with PUSCH data is expected */
   Bool                 ulDelay;      /*!< UL Delay bit 6.2 of 213 */
   /* tfu_x_001.main_2: Presently not used, it is useful when
    * Non-adaptive retransmission is implemented */
   Bool                 isRtx;        /*!< TRUE, if this is a Msg3 retransmission */
   /*ccpu00128993 - ADD - fix for msg3 softcombining bug*/
   uint8_t                   harqProcId;   /*!< HARQ process ID */
   Bool                 ndi;          /*!< 1 bit for new data indicator */
   uint8_t                   rv;           /*!< Redundancy version */
   uint8_t                   nDmrs;        /*!< 3 bits for DMRS cyclic shift */   
   /* tfu_x_001.main_2:107387:To include the size and ModOrder in DataRecp Request */
   uint16_t                  size;         /*!< Length of the Data in bytes */
   TfuModScheme         modType;      /*!< Modulation order */
} TfuUeMsg3RecpReq;

 /** @brief This structure is sent from scheduler to PHY in order to request the
  * reception of UEs data on PUSCH. This may contain the following: 
  * -# Data
  * -# Data + CQI and RI
  * -# Data + CQI and HARQ
  */
typedef struct tfuUePuschRecpReq
{  
   Bool                 hoppingEnbld; /*!< TRUE, if hopping is enabled */
   uint8_t                   hoppingBits;  /*!< Hopping bits as mentioned in 213 8.4 */
   uint8_t                   rbStart;      /*!< Start of Resource Block of allocation */
   uint8_t                   numRb;        /*!< Number of RBs allocated */
   uint8_t                   mcs;          /*!< 5 bits for MCS and RV */
   uint8_t                   harqProcId;   /*!< HARQ process ID */
   Bool                 ndi;          /*!< 1 bit for new data indicator */
   Bool                 isRtx;        /*!< TRUE, if this is a retransmission */
   uint8_t                   rv;           /*!< Redundancy version */
   uint8_t                   nDmrs;        /*!< 3 bits for DMRS cyclic shift */
   Bool                 expCqi;       /*!< TRUE, if CQI report along
                                        with PUSCH data is expected */
   Bool                 expHarq;      /*!< TRUE, if HARQ feedback along
                                        with PUSCH data is expected */
   TfuDlCqiPuschMode    mode;         /*!< CQI reporting mode configured. Note
                                        this value is valid, only if expCqi is
                                        TRUE. */
   /* tfu_x_001.main_2:107387:To include the size and ModOrder in DataRecp Req.*/
   uint16_t                  size;         /*!< Length of the Data in bytes*/
   TfuModScheme         modType;      /*!< Modulation order */
} TfuUePuschRecpReq;

/** @brief This structure is a Per UE reception request for either PUCCH or PUSCH
 * data. This contains information needed by PHY to decode the data sent by the
 * UE. 
 */
typedef struct tfuUeRecpReqInfo
{
   CmLList         lnk;                /*!< Link of Linked List */
   CmLteRnti       rnti;               /*!< RNTI of the scheduled UE */
   TfuRecpReqType  type;               /*!< Type indicating PUCCH or MSG3 or PUSCH */
   union 
   {
      TfuUePucchRecpReq    pucchRecpReq;  /*!< Reception request for PUCCH Data */
      TfuUeMsg3RecpReq     msg3RecpReq;   /*!< Reception request for PUCCH Data */
      TfuUePuschRecpReq    puschRecpReq;  /*!< Reception request for PUSCH Data */
   }t;
} TfuUeRecpReqInfo;

#endif /* TFU_UPGRADE */

/** @brief This structure contains CQI information received over PUCCH or PUSCH.
 */
typedef union tfuDlCqiInfo
{
   TfuDlCqiPucch          pucchCqi;      /*!< PUCCH CQI information */
   struct {
      uint8_t                  numOfCells;
      TfuDlCqiPusch       puschCqi[CM_LTE_MAX_CELLS]; /*!< PUSCH CQI information */
   }pusch;
} TfuDlCqiInfo;

/** @brief This structure is sent from Scheduler to PHY. This includes parameters
 * needed by PHY to decode the following: 
 * -# DATA on PUSCH
 * -# HARQ Feedback on PUCCH
 * -# CQI Report 
 * -# RI Report
 * -# SR Indications
 * This structure carries the reception information for all the scheduled UEs.
 */
typedef struct tfuRecpReqInfo
{
   CmMemListCp       memCp;         /*!< Memory control point */
   CmLteCellId       cellId;        /*!< Cell ID */
   CmLteTimingInfo   timingInfo;    /*!< Timing information */
   CmLListCp         ueRecpReqLst;  /*!< Linked List of reception requests
                                      TfuUeRecpReqInfo */
#ifdef EMTC_ENABLE
   CmLListCp         emtcUeRecpReqLst;  /*!< Linked List of reception requests
                                      TfuUeRecpReqInfo */
#endif
   /*tfu_x_001.main_8 - ADD - New Pucch RR Info Combination support */
   /*ccpu00116923 - ADD - SRS present support*/
#ifdef TFU_UPGRADE   
   Bool                srsPres;       /*!< SRS present information (For Cell
                                        Specific SRS notification)
                                      0: No SRS in this subframe
                                      1: SRS present in this subframe */
#endif
} TfuRecpReqInfo;

 /** @brief This structure contains the PDCCH to be sent on PHY, that is, DCI
  * formats. This information is associated with an RNTI. 
  */
 typedef struct tfuPdcchInfo
{
   CmLList           lnk;             /*!< Link of Linked List */
   CmLteRnti         rnti;            /*!< RNTI of the allocation */
#ifdef LTEMAC_SPS
   Bool              isSpsRnti;       /*!< TRUE if rnti is SPS RNTI */
   CmLteRnti         crnti;           /*!< crnti in case rnti is SPS RNTI */
#endif
   uint16_t               nCce;            /*!< CCE index */
   CmLteAggrLvl      aggrLvl;         /*!< Aggregation level possible values are
                                        {1, 2, ...,8}. */
   uint8_t                dciNumOfBits;    /*!< The size of DCI in bits */
   TfuDciInfo        dci;             /*!< PDCCH DCI format */
#ifdef EMTC_ENABLE
	uint8_t 					distributedAlloc;
	uint8_t						localizedAntPortIndex;
	uint16_t					dmrs_txpowerControl; 
	uint8_t						nRBxm;
	uint8_t 					startRB;
	uint32_t					scramblerInit;
	uint32_t					demodRSInitValue;
#endif
#ifdef TFU_5GTF
   uint8_t               sectorId;        /* Sector index of the 5GNB. Value: 0->2 */
   uint8_t               sccIdx;          /* Serving Cell index in the given sector. Value: 0->7 */
   uint8_t               grpId;           /* Ue group Id */
#endif
} TfuPdcchInfo;

/** @brief This structure contains one PHICH information. In order to locate the
 * resources to be used for PHICH, the following information is provided along
 * with the feedback:
 * -# rbStart
 * -# nDmrs
 * -# iPhich
 * -# txPower
 * */
typedef struct tfuPhichInfo
{
   CmLList           lnk;          /*!< Link of Linked List */
   uint8_t                rbStart;      /*!< Starting RB */
   uint8_t                nDmrs;        /*!< DMRS offset index */
   Bool              isAck;        /*!< TRUE, if ACK, else NACK */
   Bool              isForMsg3;
#ifdef TFU_TDD
   uint8_t                     iPhich;        /*!< Needed to Calculate PHICH
                                           location. For TDD only */
#endif
/* tfu_x_001.main_8. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   uint16_t                    txPower;        /*!< Offset to the ref. signal power */
#endif
} TfuPhichInfo;

/** @brief This structure is sent from Scheduler to PHY. This includes all the
 * control information to be sent to the UE. 
 * -# PDCCH Physical Downlink Control Channel
 * -# PHICH Physical HARQ Indicator Channel
 * -# PCFICH Physical Control Format Indicator Channel
 */
typedef struct tfuCntrlReqInfo
{
   CmMemListCp       memCp;         /*!< Memory control point */
   CmLteCellId       cellId;        /*!< Cell ID */
   CmLteTimingInfo   ulTiming;      /*!< Timing information for UL PDCCHs and PHICH */
   CmLteTimingInfo   dlTiming;      /*!< Timing information for DL PDCCHs and CFI */
   uint8_t                cfi;           /*!< CFI value that goes on the PCFICH control channel */
   uint32_t               numDlActvUes;  /*!< Active Ues reqrd for rgu dyna delta*/
   CmLListCp         ulPdcchLst;    /*!< List of Uplink PDCCHs (DCI format 0) for the TfuPdcchInfo subframe */
   CmLListCp         dlPdcchLst;    /*!< List of Downlink PDCCHs for the TfuPdcchInfo subframe */
   CmLListCp         phichLst;      /*!< List of PHICHs for the TfuPhichInfo subframe */
#ifdef LTEMAC_SPS /* SPS_NEW_CHGS */
   Bool              isSPSOcc;      /*!< TRUE if this is an SPS Occasion */
#endif
#ifdef EMTC_ENABLE
   CmLListCp         ulMpdcchLst;    /*!< List of Uplink PDCCHs (DCI format 0) for the TfuPdcchInfo subframe */
   CmLListCp         dlMpdcchLst;    /*!< List of Downlink PDCCHs for the TfuPdcchInfo subframe */
#endif

} TfuCntrlReqInfo;

 /** @brief This structure carries the control information that is needed
  * by the Physical layer to process the data to be sent on PDSCH. 
  */
typedef struct tfuPdschDciInfo
{   
   TfuDciFormat               format;            /*!< Selector for the union*/   
   union
   {
      TfuDciFormat1AllocInfo  format1AllocInfo;  /*!< Format 1 allocation information.*/
      TfuDciFormat1aAllocInfo format1aAllocInfo; /*!< Format 1A allocation information.*/
      TfuDciFormat1bAllocInfo format1bAllocInfo; /*!< Format 1B allocation information.*/
      TfuDciFormat1cInfo      format1cAllocInfo; /*!< Format 1C allocation information.*/
      TfuDciFormat1dAllocInfo format1dAllocInfo; /*!< Format 1D allocation information.*/
      TfuDciFormat2AllocInfo  format2AllocInfo;  /*!< Format 2A allocation information.*/
      TfuDciFormat2AAllocInfo format2AAllocInfo; /*!< Format 2AA allocation information.*/
#ifdef EMTC_ENABLE
      TfuDciFormat61AllocInfo format61AllocInfo;  /*!< Format 6-1A allocation information.*/
      TfuDciFormat62AllocInfo format62AllocInfo;  /*!< Formtat 6-2 allocation information */
#endif
#ifdef TFU_5GTF
	   TfuDciFormatA1Info      formatA1Info;  /*!< 5GTF: Format A1 information */
	   TfuDciFormatA2Info      formatA2Info;  /*!< 5GTF: Format A1 information */
	   TfuDciFormatB1Info      formatB1Info;  /*!< 5GTF: Format B1 information */
	   TfuDciFormatB2Info      formatB2Info;  /*!< 5GTF: Format B2 information */
#endif
   } u;
} TfuPdschDciInfo;

/** 
 * UE Transmission Modes */
typedef enum tfuTxMode
{
   TFU_UE_TM_1 = 1,    /** Transmission Mode 1 */
   TFU_UE_TM_2,        /** Transmission Mode 2 */
   TFU_UE_TM_3,        /** Transmission Mode 3 */
   TFU_UE_TM_4,        /** Transmission Mode 4 */
   TFU_UE_TM_5,        /** Transmission Mode 5 */
   TFU_UE_TM_6,        /** Transmission Mode 6 */
   TFU_UE_TM_7         /** Transmission Mode 7 */
} TfuTxMode;

/* tfu_x_001.main_8. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
 /** @brief This structure carries the Beam Forming Vector Information
   * needed by the Physical layer to process the data to be sent on PDSCH. 
   */
 typedef struct tfuBfVectorInfo
 {
   uint8_t             sbIndex;    /** Subband Index */
   uint8_t             numPhyAnt;  /** Number of Physical Antennas */
   uint16_t            bfValue[TFU_MAX_PHY_ANTENNA];  /** Beam forming vector element for 
                                      physical antenna #i real 8 bits followed by imaginary 8 bits */ 
 }TfuBfVectorInfo;
 #endif

#ifdef L2_OPTMZ
/**
 * @brief DatReq Information of a logical channel.
 */
typedef struct tfuDatReqLchInfo
{
   Bool              freeBuff; /* !< If TRUE, the buffer is to be freed by MAC */
   uint32_t               numPdu;  /*!< No of PDUs */
   Buffer           *mBuf[TFU_MAX_PDU]; /*!<SIncMsgRef in MAC. 
                                           Freed by CL as well*/
}TfuDatReqLchInfo;

/** @brief This structure contains a single MAC TB Headers and RLC PDU
 *  @details It contains preallocated MAC Header,MAC CEs,logical channel info
 *   RLC PDU and pre-allocated padding 
 */  
typedef struct tfuDatReqTbInfo
{
   Bool             tbPres;
   MsgLen           tbSize;
   Buffer           *macHdr;  /*!< Pre allocated. CL shall not free this*/
   Buffer           *macCes;  /*!< Pre-allocated. CL shall not free this */
   uint32_t               padSize;
   uint32_t               numLch;  /*!< No of logical channels associated to TB*/ 
   TfuDatReqLchInfo  lchInfo[TFU_MAX_LC]; /*!< Array of logical channels */
}TfuDatReqTbInfo;
#endif

 /** @brief This structure contains a single MAC TB.
  * @details Data is accompanied with control information pertaining to the
  * allocation and what is required by physical layer for its processing. 
  */
typedef struct tfuDatReqPduInfo
{
   CmLList              lnk;                 /*!< Link of Linked List */
   CmLteRnti            rnti;                /*!< RNTI of the UE */
   TfuPdschDciInfo      dciInfo;             /*!< Carries the control data
                                                  needed for processing PDSCH data */
   uint8_t                   nmbOfTBs;            /*!< Number of TBs, in cases where
                                                  one TB is disabled */
#ifdef FIVEGTF_PHY_API
   uint16_t                  crcResult;
#endif
#ifndef L2_OPTMZ
   Buffer               *mBuf[TFU_MAX_TB];   /*!< MAC PDUs buffer */
#else
   TfuDatReqTbInfo      tbInfo[TFU_MAX_TB];  /*!< Array of TfuDatReqTbInfo*/
#endif
   Bool                 puschRptUsd;         /*!< If PUSCH Rpt used for scheduling */
   TfuDlCqiPuschInfo    puschPmiInfo;        /*!< Explicitly inform PHY of the
                                                  PMI details for the entire 
                                                  DL BW */
   TfuTxMode            transMode;           /*!< Transmission Mode of this UE */                        
   TknUInt32               doa;                 /*!< DOA indicator for this UE */
   Bool                 isTApres;
#ifdef TFU_UPGRADE
   /*tfu_x_001.main_8 - ADD - Detailed comments for fields */
   uint8_t                   numLayers;           /*!< Number of Layers. Note: Field
                                               specified at Interface. Value
                                               not set from LTE MAC */
   uint8_t                   deltaPowOffIdx;      /*!< Delta power offset. Note:
                                               Field specified at Interface. Value
                                              not set from LTE MAC */
   uint8_t                   numBfPrbPerSb;       /*!< Number of PRBs that are
                                               treated as one Subband. Note:
                                               Field specified at interface.
                                               Value not set from LTE MAC */
   uint8_t                   numBfVectors;        /*!< Number of Beam Forming
                                               vectors. Note: Field specified at
                                              Interface. Value not set from LTE
                                              MAC */
   TfuBfVectorInfo      bfVector[TFU_MAX_DL_SUBBAND]; /*!< BeamForming vector
                                                        information. Note: Field
                                                       specified at Interface.
                                                       Value not set from LTE
                                                       MAC */
   uint16_t                  txPwrOffset;          /*!< Offset to the reference 
                                                  signal power. Value 0->10000,
                                                  representing -6 dB to 4 dB in
                                                  0.001 dB steps */      
/* LTE_ADV_FLAG_REMOVED_START */
	uint8_t                   pa;                  /*!< ratio of PDSCH EPRE to cell-specific RS 
	                                              EPRE among PDSCH REs */
#endif
   uint8_t isEnbSFR;             /*to tell if SFR is enabled*/
} TfuDatReqPduInfo;


/** @brief This structure contains the Data PDUs. 
 * @details This structure contains Data meant to be sent on both PBCH and
 * PDSCH. Control information necessary for processing of the data is sent along
 * with the Data.
 */
typedef struct tfuDatReqInfo
{
   CmMemListCp          memCp;         /*!< Memory control point */
   CmLteCellId          cellId;        /*!< Cell ID */
   CmLteTimingInfo      timingInfo;    /*!< Timing information */
   TknBuf               bchDat;        /*!< BCCH mapped onto BCH Buffer, if
                                            present */
#ifdef EMTC_ENABLE
   TknBuf               emtcBcchDat;  /*!< BCCH mapped onto BCH Buffer, if
                                            present */
#endif
   CmLListCp            pdus;          /*!< Linked List of TfuDatReqPduInfo PDUs */
} TfuDatReqInfo;

 /** @brief This structure contains the Data PDU and the RNTI of the UE.
  */
typedef struct tfuDatInfo
{
   CmLList        lnk;    /*!< Link of Linked List */
   CmLteRnti      rnti;   /*!< RNTI of the UE */
#ifdef FIVEGTF_PHY_API
   TfuPdcchInfo   *grantInfo; /*!< UL Grant Info */
#endif
   Buffer         *mBuf;  /*!< Data PDU */
} TfuDatInfo;

/** @brief This structure contains the Data PDUs received at Physical layer.
 * @details All the PDUs received by Physical layer in one subframe are clubbed
 * together into a single data structure and sent to MAC for processing. 
 */
typedef struct tfuDatIndInfo
{
   CmMemListCp          memCp;         /*!< Memory control point */
   CmLteCellId          cellId;        /*!< Cell ID */
   CmLteTimingInfo      timingInfo;    /*!< Timing information */
   CmLListCp            datIndLst;     /*!< List of data PDUs - TfuDatInfo */
#ifdef TFU_5GTF
   uint8_t               sectorId;        /* Sector index of the 5GNB. Value: 0->2 */
   uint8_t               sccIdx;          /* Serving Cell index in the given sector. Value: 0->7 */
#endif
} TfuDatIndInfo;

 /** @brief This structure contains the SR indication for an UE, identified by
  * the given RNTI. The structure also contains a PUCCH Delta power, that is
  * calculated by the physical layer, and is used by the scheduler for Power
  * control of the UE. 
  */
typedef struct tfuSrInfo
{
   CmLList        lnk;             /*!< Link of Linked List */
   CmLteRnti      rnti;            /*!< UEs RNTI */
} TfuSrInfo;

 /** @brief This structure contains all the Scheduling Requests (SR) indications
  * received in a subframe. 
  */
typedef struct tfuSrIndInfo
{
   CmMemListCp          memCp;      /*!< Memory control point */
   CmLteCellId          cellId;     /*!< Cell ID */
   CmLteTimingInfo      timingInfo; /*!< Timing information */
   CmLListCp            srLst;      /*!< List of SR indications - TfuSrInfo */
} TfuSrIndInfo;

/* tfu_x_001.main_8. Added changes of TFU_UPGRADE */
//#ifdef TFU_UPGRADE
/** @enum TfuHqFdbk
 * This enum contains the possible values for the HARQ feedback.
 */
typedef enum
{  
   TFU_HQ_ACK=1,
   TFU_HQ_NACK,
   TFU_HQ_ACK_OR_NACK,
   TFU_HQ_DTX,
   TFU_HQ_ACK_OR_DTX,
   TFU_HQ_NACK_OR_DTX,
   TFU_HQ_ACK_OR_NACK_OR_DTX,
   TFU_HQ_INVALID = 255  
} TfuHqFdbk;
//#endif

 /** @brief This structure contains the HARQ feedback for a single UE. 
  * @details RNTI is present to identify the UE. 
  * For FDD, a single feedback is present. 
  * For TDD HARQ multiplexing, a list of HARQ feedbacks is present.
  */
/* tfu_x_001.main_8. Added changes of TFU_UPGRADE */
typedef struct tfuHqInfo
{
   CmLList                lnk;           /*!< Link of Linked List */
   CmLteRnti              rnti;          /*!< RNTI of the UE */
   TfuAckNackMode         hqFdbkMode;    /*!< Feedback mode */  
   uint8_t                     noOfTbs;       /*!< Number of TBs */
#ifdef TFU_TDD
   uint8_t                     M;             /*!< Number of feedbacks multiplexed */
#endif /* TFU_TDD */
   TfuHqFdbk              isAck[TFU_MAX_HARQ_FDBKS];  /*!< Harq feedbacks */ 
   Bool                   isPusch;       /*!< Indicates if this HARQ feedback is
                                              received on PUSCH */
                                         /* Note: isPusch is not a field
                                          * specified by Femto Forum. */
} TfuHqInfo;

 /** @brief This structure contains a list of HARQ feedbacks for a number of UEs.
  * @details All the HARQ feedback received in the subframe are clubbed together
  * in this data structure and given to the scheduler for processing. 
  */
typedef struct tfuHqIndInfo
{
   CmMemListCp          memCp;         /*!< Memory control point */
   CmLteCellId          cellId;        /*!< Cell ID */
   CmLteTimingInfo      timingInfo;    /*!< Timing information */
   CmLListCp            hqIndLst;      /*!< List of HARQ feedback - TfuHqInfo */
} TfuHqIndInfo;


/** @brief This structure conveys the Uplink CQI information for a single sub-band. 
 */
typedef struct tfuUlCqiInfo
{
   TfuSubbandCqiInfo subbandCqi; /*!< CQI associated with a sub-band */
} TfuUlCqiInfo;

 /** @brief This structure contains the UL CQI estimated by the physical layer. 
  * @details The report can be wide-band CQI or sub-band CQI or both. 
  */
typedef struct tfuUlCqiRpt
{
   CmLList           lnk;                              /*!< Link of Linked List */
   CmLteRnti         rnti;                             /*!< RNTI */
   Bool              isTxPort0;                        /*!< TRUE, if TX port is
                                                         0, else TX port is 1  */
   uint8_t                wideCqi;                          /*!< Wide-band CQI*/
   uint8_t                numSubband;                       /*!< Number of sub-bands */
   TfuUlCqiInfo      ulCqiInfoArr[TFU_MAX_UL_SUBBAND]; /*!< UL CQI information array */
} TfuUlCqiRpt;

/** @brief This structure conveys the Uplink CQI information.
 * @details Uplink CQI is calculated by the physical layer, and this
 * information is provided to the scheduler. 
 * This data structure clubs together the UL CQI estimates for all the UEs that
 * are scheduled in the subframe for uplink transmission. 
 */
typedef struct tfuUlCqiIndInfo
{
   CmMemListCp       memCp;         /*!< Memory control point */
   CmLteCellId       cellId;        /*!< Cell ID */
   CmLteTimingInfo   timingInfo;    /*!< Timing information */
   CmLListCp         ulCqiRpt;      /*!< List of UL CQI reports - TfuUlCqiRpt */
} TfuUlCqiIndInfo;

 /** @brief This structure contains the DOA estimated by the physical layer. 
  * @details DOA for a given UE. 
  */
typedef struct tfuDoaRpt
{
   CmLList                lnk;           /*!< Link of Linked List */
   CmLteRnti              rnti;          /*!< RNTI */
   uint32_t                    doa;           /*!< DOA for rnti */
} TfuDoaRpt;

/** @brief This structure conveys the DOA (Direction Of Arrival) indicator.
 * @details DOA is calculated by the physical layer, and this
 * information is provided to the scheduler. 
 * This data structure clubs together the DOAs for all the UEs
 * calculated by PHY in this subframe. 
 */
typedef struct tfuDoaIndInfo
{
   CmMemListCp            memCp;         /*!< Memory control point */
   CmLteCellId            cellId;        /*!< Cell ID */
   CmLListCp              doaRpt;        /*!< List of DOA reports - TfuDoaRpt */
} TfuDoaIndInfo;
/** @} */
 /** @brief This structure contains the DL CQI report for a single UE. 
  * @details The report can either be described on PUCCH or PUSCH, a flag
  * conveys this information. 
  */
typedef struct tfuDlCqiRpt
{
   CmLList           lnk;         /*!< Link of Linked List */
   CmLteRnti         rnti;        /*!< RNTI */
   Bool              isPucchInfo; /*!< Indicates whether PUCCH or PUSCH information */
   TfuDlCqiInfo      dlCqiInfo;   /*!< DL CQI information */
} TfuDlCqiRpt;

/** @brief This structure contains a list of Downlink CQI reports transmitted by
 * UEs. 
 * @details This structure clubs together DL CQI reports for a number of UEs. 
 */
typedef struct tfuDlCqiIndInfo
{
   CmMemListCp       memCp;            /*!< Memory control point */
   CmLteCellId       cellId;           /*!< Cell ID */
   CmLteTimingInfo   timingInfo;       /*!< Timing information */
   CmLListCp         dlCqiRptsLst;     /*!< List of DL CQI reports TfuDlCqiRpt */
} TfuDlCqiIndInfo;

 /** @brief This structure contains the CRC indication for a single
  * UE. 
  */
typedef struct tfuCrcInfo
{
   CmLList           lnk;         /*!< Link of Linked List */
   CmLteRnti         rnti;        /*!< RNTI of the UE */
   Bool              isFailure;   /*!< TRUE, if CRC check fails */
   TknUInt8             rv;          /*!< Redundancy version provided by PHY */
   Bool              isDtx;       /*! If SNR < 0 , set to TRUE */
} TfuCrcInfo;

/** @brief This structure contains information that is passed as a part of the
 * CRC Indication from PHY to MAC.
 * @details This structure clubs together all the CRC indications for
 * a single subframe and conveys this information to the scheduler. 
 */
typedef struct tfuCrcIndInfo
{
   CmMemListCp       memCp;         /*!< Memory control point */
   CmLteCellId       cellId;        /*!< Cell ID */
   CmLteTimingInfo   timingInfo;    /*!< Timing information */
   CmLListCp         crcLst;        /*!< List of CRC indications - TfuCrcInfo */
#ifdef TFU_5GTF
   uint8_t               sectorId;        /* Sector index of the 5GNB. Value: 0->2 */
   uint8_t               sccIdx;          /* Serving Cell index in the given sector. Value: 0->7 */
#endif
} TfuCrcIndInfo;

 /** @brief This structure contains the timing advance information for a single
  * UE. 
  */
 typedef struct tfuTimingAdvInfo
{
   CmLList           lnk;         /*!< Link of Linked List */
   CmLteRnti         rnti;        /*!< RNTI */
   uint8_t                timingAdv;   /*!< Value of the Timing advance */
} TfuTimingAdvInfo;


/** @brief This structure contains information that is passed as part of the Timing
 * Advance indication from PHY to MAC.
 * @details This structure clubs together timing advances for a number of UEs.
 * 
 */
typedef struct tfuTimingAdvIndInfo
{
   CmMemListCp       memCp;         /*!< Memory control point */
   CmLteCellId       cellId;        /*!< Cell ID */
   CmLteTimingInfo   timingInfo;    /*!< Timing information */
   CmLListCp         timingAdvLst;  /*!< List of timing advances - TfuTimingAdvInfo */
} TfuTimingAdvIndInfo;

/** @brief This structure contains information that is passed as part of the TTI
 * indication sent from CL to MAC and SCH.
 */
typedef struct tfuTtiCellInfo
{
   CmLteCellId      cellId;       /*!< Cell ID */
   CmLteTimingInfo  timingInfo;   /*!< SFN, SF for each cell */
   Bool             isDummyTti;   /*!< Flag to indicate dummy TTI */                       
   /* 4UE_TTI_DELTA */             
   uint8_t               schTickDelta; /*!< Dynamic Delta for SCH. Value will vary
                                       according to num of active UEs */                
   uint8_t               dlBlankSf;    /*!< TODO */  
   uint8_t               ulBlankSf;    /*!< TODO */        

} TfuTtiCellInfo;

/** @brief This structure contains information that is passed as part of the TTI
 * indication sent from CL to MAC and SCH.
 */
typedef struct tfuTtiIndInfo
{
   uint8_t             numCells;                /*!< Num of Cells */
   TfuTtiCellInfo cells[CM_LTE_MAX_CELLS]; /*!< Array of Cell timing info */ 

} TfuTtiIndInfo;

/** @brief This structure contains the information for a single Random Access Request.
 * @details The information present in this structure is for a single preamble.
 * Associated with each preamble are fields that the physical layer calculates
 * based on the reception of the Random Access Request. These are the following:
 * -# Timing Advance
 * -# TPC
 * -# CQI (optional)
 */
typedef struct tfuRaReqInfo
{
   uint8_t           rapId;     /*!< ID of the preamble choosen by the UE for Random Acess.*/
   uint16_t          ta;        /*!< Amount of Timing advance needed by the UE for it
                             to be uplink synchronized, this is calculated by Physical layer. */
   uint8_t           tpc;       /*!< Transmit power control command that the physical
                            layer calculates from the RA request received.*/ 
   Bool         cqiPres;   /*!< TRUE, if CQI present. */
   uint8_t           cqiIdx;    /*!< This is the Channel Quality Index of the UL channel estimated by the
                              physical layer. This aids MAC in the scheduling of this UE. */

} TfuRaReqInfo;

 /** @brief This structure contains RACH request information for a single RA-RNTI.
  */
typedef struct tfuRachInfo
{
   CmLteRnti       raRnti;       /*!< RA-RNTI */
   uint8_t              numRaReqInfo; /*!< Number of RA requests */
#ifdef PRACH_5GTF
   uint8_t              nPreambleFormat;
   uint8_t              nPRACHConfiguration;
   uint8_t              nRootSeq;
   uint8_t              nCyclicShift;
   uint8_t              nf1Value;
   uint8_t              nPRACHSymbIndex;
   uint16_t             nPwrFactor;
#endif
   TfuRaReqInfo    *raReqInfoArr; /*!< RA requests */
} TfuRachInfo;

/** @brief This structure contains the list of Random Access Requests received in a single TTI.
 * @details Random Access Request is associated with a RA-RNTI, which remains
 * common for all the random access requests within a subframe. This structure
 * thus carries the RA-RNTI and a list of Random Access Request information.
 * This information consists of the following: 
 * -# preamble-id - this corresponds to the Index of the Preamble choosen by the
 * UE.
 * -# Timing Advance - this corresponds to the timing adjustment that the UE
 * needs to be uplink synchronized. This value is calculated by physical layer. 
 * -# tpc - this is the power control command that the physical layer calculates
 * from the received RA request. This value is transmitted in the RA Response
 * sent out by MAC.
 * -# CQI - this is the Channel Quality Index of the UL channel estimated by the
 * physical layer. This aids MAC in the scheduling of this UE.
 */
typedef struct tfuRaReqIndInfo
{
   CmMemListCp     memCp;         /*!< Memory control point */
   CmLteCellId     cellId;        /*!< Cell ID */
   CmLteTimingInfo timingInfo;    /*!< Timing information */
   uint8_t              nmbOfRaRnti;   /*!< Number of RA-RNTIs present for FDD this
                                    must be 1 for TDD, it can be a max of 6.  */
   TfuRachInfo     *rachInfoArr;   /*!< Array of
                                         RACH information per
                                         RA-RNTI */
} TfuRaReqIndInfo;

/** @brief This structure contains PUCCH Delta power for a single UE.
 */
typedef struct tfuPucchDeltaPwr
{
   CmLList         lnk;           /*!< Link of Linked List */
   CmLteRnti       rnti;          /*!< Memory control */
   S8              pucchDeltaPwr; /*!< PUCCH delta power to be sent by PHY. */
} TfuPucchDeltaPwr;

/** @brief This structure contains information that is passed as part of the
 * PUCCH Delta power indication from PHY to SCH.
 * @details This structure clubs together PUCCH Delta power values for a number
 * of UEs.
 */
typedef struct tfuPucchDeltaPwrIndInfo
{
   CmMemListCp       memCp;         /*!< Memory control point */
   CmLteCellId       cellId;        /*!< Cell ID */
   CmLteTimingInfo   timingInfo;    /*!< Timing information */
   CmLListCp         pucchDeltaPwrLst;  /*!< List of PUCCH Delta power
                                        TfuPucchDeltaPwr */
} TfuPucchDeltaPwrIndInfo; /* TODO renamed this from Del -> Delta. not changed tfu.c */

 /** @brief LAA:This structure contains the cell Id and subframe information for
  * which the transmission failed. 
  */
typedef struct tfuErrIndInfo
{
   CmLteCellId          cellId;        /*!< Cell ID */
   CmLteTimingInfo      timingInfo;    /*!< Timing information */
} TfuErrIndInfo;

#ifdef L2_OPTMZ
/** @brief :This structure contains the cell Id and UeId to inform CL
  * to delete datReq if it exists in CL but UeId got changed or similar other
  * scenarios.These scenarios are possible only when L2_OPTMZ flag is enabled.
  */
typedef struct tfuDelDatReqInfo
{
   CmLteCellId          cellId;        /*!< Cell ID */
   CmLteRnti            ueId;         /*!< CRNTI of the UE */
} TfuDelDatReqInfo;
#endif

#ifdef __cplusplus
}
#endif
#endif /* __TFUX__ */

/********************************************************************30**

         End of file
**********************************************************************/
