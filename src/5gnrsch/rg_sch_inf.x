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

  Name:     LTE-MAC layer 
  
  Type:     C Include File 
  
  Desc:     Structures, variables, and typedefs required by the interface between
            MAC and Scheduler.

  File:     rg_sch_inf.x 

**********************************************************************/
/** 
  @file rg_sch_inf.x 
  @brief Structure declarations and definitions for MAC-SCH interface.
  */

#ifndef __GKSCH_X__ 
#define __GKSCH_X__

#ifdef __cplusplus
extern "C" {
#endif
#ifdef EMTC_ENABLE
/** 
 * @brief This structure contains the uplink grant information that is sent in
 response to the random access request from the UE for CEmodeA.
 */
typedef struct rgInfEmtcCEmodeARarUlGrt
{
   uint8_t msg3PUSCHNbIdx;        /*!< Indicates the Msg3 PUSCH narrowband index. */          
   uint8_t rbStart;               /*!< Start Resource block of allocation. */
   uint8_t numRb;                 /*!< Number of resource blocks allocated. */
   uint8_t msg3PUSCHNumRep;       /*!< Number of repetitions for Msg3 PUSCH. */
   uint8_t iMcsCrnt;              /*!< Current MCS index of the grant. */
   uint8_t tpc;                   /*!< TPC command for the uplink grant. */
   uint8_t cqiBit;                /*!< Indicates the CQI is enabled or not. */
   uint8_t delayBit;              /*!< Indicates the Delay bit. */
   uint8_t msg34MPDCCHNbIdx;      /*!< Indicates Msg3/4 MPDCCH narrowband index. */          
} RgInfEmtcCEmodeARarUlGrnt;

/** 
 * @brief This structure contains the uplink grant information that is sent in
 response to the random access request from the UE for CEmodeB.
 */
typedef struct rgInfEmtcCEmodeBRarUlGrnt
{
   uint8_t msg3PUSCHNbIdx;        /*!< Indicates the Msg3 PUSCH narrowband index. */          
   uint8_t rbStart;               /*!< Start Resource block of allocation. */
   uint8_t numRb;                 /*!< Number of resource blocks allocated. */
   uint8_t msg3PUSCHNumRep;       /*!< Number of repetitions for Msg3 PUSCH. */
   uint8_t iTbsCrnt;              /*!< Current TBS index of the grant. */
   uint8_t msg34MPDCCHNbIdx;      /*!< Indicates Msg3/4 MPDCCH narrowband index. */          
}RgInfEmtcCEmodeBRarUlGrnt;

typedef enum rgEmtcCEmodeType
{
   RG_EMTC_CEMODE_A,
   RG_EMTC_CEMODE_B,
   RG_EMTC_CEMODE_INV
}RgEmtcCEmodeType;

/** 
 * @brief This structure contains the uplink grant information that is sent in
 response to the random access request from the UE for EMTC.
 */
typedef struct rgInfEmtcRarUlGrnt 
{
   uint8_t rgEmtcCEmodeType;
   union
   {
      RgInfEmtcCEmodeARarUlGrnt  emtcCEmodeARarUlGrnt;
      RgInfEmtcCEmodeBRarUlGrnt  emtcCEmodeBRarUlGrnt;
   }u;
}RgInfEmtcRarUlGrnt;

#endif /* EMTC_ENABLE */
/**
 * @brief This structure contains common channel buffer occupancy report information.
 */
typedef struct rgInfCmnBoRpt
{
   S16          cellSapId;
   CmLteCellId  cellId;         /*!< Identifies the cell. CellId value must be within the set of configured cell IDs. */
   CmLteLcId    lcId;           /*!< Identifies the logical channel. lcId value range is defined in
                                     Section 6.2.1 in 36.321 specification. */
   CmLteLcType  lcType;         /*!< Identifies the Logical channel type.lcType can take the following values:
                                      CM_LTE_LCH_BCCH
                                      CM_LTE_LCH_PCCH
                                      CM_LTE_LCH_CCCH
                                      CM_LTE_LCH_DCCH
                                      CM_LTE_LCH_DTCH */
   S32 bo;                      /*!< Buffer occupancy reported by RLC in bytes. */
#ifdef EMTC_ENABLE
   uint8_t         emtcDIReason;   /*!< Reason for DI message to send. */
   uint8_t         pnb;            /*!< Pagging narrowBand on which Ue perfom paging reception*/
#endif
   union                        /*!< lcType in the primitive is the union selector. */
   {
      CmLteTimingInfo timeToTx; /*!< Timing info for the BO, applicable for BCCH and PCCH. */
      CmLteRnti       rnti;     /*!< Temporary C-RNTI, only for CCCH. RNTI range is specified 
                                     in Section 7.1 in 36.321 specification. */
   } u;
} RgInfCmnBoRpt;
/**
 * @brief This structure contains dedicated channel buffer occupancy report 
 *  information.
 */
typedef struct rgInfDedBoRpt
{
   S16          cellSapId;  /*!< Identifies the cell SAP. cellSapId value must be within the set of configured cell SAP IDs. */
   CmLteCellId  cellId;     /*!< Identifies the cell. CellId value must be within the set of configured cellIds. */
   CmLteRnti    rnti;       /*!< Identifies the UE. RNTI value range is specified in Section 7.1 in 25.321 specification. */
   CmLteLcId    lcId;       /*!< Identifies the logical channel. lcId value range is defined in Section 6.2.1 in 36.321 specification. */
   S32          bo;         /*!< Number of bytes reported as Buffer occupancy by RLC. */
#ifdef CCPU_OPT
   Bool         staPduPrsnt;   /*!< Is status PDU present reported by RLC. */
   uint16_t          estRlcHdrSz;/*!< Estimated hader size reported by RLC */
#endif
   uint32_t          staPduBo;   /*!< Number of bytes reported as Buffer occupancy for status PDU by RLC. 
                                 This is already included in BO.*/
  uint32_t          oldestSduArrTime; /*!< Oldest SDU Arrival Time from Upper Layer */	
  Bool         setMaxUlPrio; /*!< set when Pollbit is set from RLC in PDU */
  Bool         setMaxDlPrio; /*!< Set when there is a status PDU in the DL*/			 
} RgInfDedBoRpt;

/*Fix: start: Indicate UE deletion from MAC to Scheduler*/
/**
 * @brief This structure contains the RNTI which is deleted at MAC 
 *  information.
 */
typedef struct rgInfUeDelInd
{
   S16          cellSapId;  /*!< Identifies the cell SAP. cellSapId value must be within the set of configured cell SAP IDs. */
   CmLteCellId  cellId;     /*!< Identifies the cell. CellId value must be within the set of configured cellIds. */
   CmLteRnti    rnti;       /*!< Identifies the UE. RNTI value range is specified in Section 7.1 in 25.321 specification. */
} RgInfUeDelInd;

/*Fix: end: Indicate UE deletion from MAC to Scheduler*/



/** 
 * @brief This structure contains the uplink grant information that is sent in
          response to the random access request from the UE.
 */
typedef struct rgInfRarUlGrnt
{
#ifndef MAC_5GTF_UPDATE
   uint8_t hop;        /*!< Indicates the hopping flag. */     
#else
   uint8_t xPuschRange; /*!< xPUSCH range */
#endif
   uint8_t rbStart;    /*!< Start Resource block of allocation. */
   uint8_t numRb;      /*!< Number of resource blocks allocated. */
   uint8_t tpc;        /*!< TPC command for the uplink grant. */
   uint8_t iMcsCrnt;   /*!< Current MCS index of the grant. */
   uint8_t delayBit;   /*!< Indicates the Delay bit. */
#ifndef MAC_5GTF_UPDATE
   uint8_t cqiBit;     /*!< Indicates the CQI is enabled or not. */
#else
   uint8_t numBsiRep;   /*!< Number of BSI reports. */
   uint8_t bsiBetaOffIdx; /*!< Index of BSI beta offset used in Msg3 */
   uint8_t pcrs;         /*!< UL dual PCRS */
#endif
} RgInfRarUlGrnt;

/** 
 * @brief This structure carries timing adjustment, uplink grant information for the specific temporary C-RNTI.
 */
typedef struct rgInfCrntiInfo
{
    CmLteRnti       tmpCrnti;    /*!< Temporary C-RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */     
    uint8_t              rapId;       /*!< rapId identifies the index of the Random Access Preamble. rapId ranges from 0 to 63.*/ 
    TknUInt16          ta;          /*!< Timing Adjustment. Timing Adjustment Value range is defined in Section 6.1.3.5 in 36.321 specification. */ 
    RgInfRarUlGrnt  grnt;        /*!< Uplink Grant to go in RAR. */ 
    Bool            isContFree; /*!< Indicates whether the procedure is contention-free or not. */
#ifdef EMTC_ENABLE
    RgInfEmtcRarUlGrnt  emtcGrnt; /*!< Uplink grant for EMTC UE to go in RAR. */
#endif
}RgInfCrntiInfo;

/** 
 * @brief This structure carries information about downlink control format, scheduled TB size,
          backoff indicator value, and the set of Random Access Responses within this RA-RNTI. 
*/
typedef struct rgInfRaRntiInfo
{
   uint16_t             raRnti;     /*!< RA-RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */
   TfuPdschDciInfo dciInfo;    /*!< PDCCH allocated for RA-RNTI. For more information
                                    refer to <i>TFU Interface Service Definition</i> (p/n 1100091). */
   uint32_t             schdTbSz;   /*!< Scheduled TB size. schdTbSz value range is defined in Section 7.1.7.2.1 in 36.213
                                    specification. */
   TknUInt8           backOffInd; /*!< Indicates the Backoff Indicator value. backOffInd value range
                                    is defined in Section 7.2 in 36.321 specification. */
   uint8_t              numCrnti;   /*!< Number of valid RARs in the array. */
   RgInfCrntiInfo  *crntiInfo; /*!< RAR information. */
}RgInfRaRntiInfo;
  
/**
 * @brief This structure contains information about the RA-RNTIs for which
 *  random access responses are generated. 
 */
typedef struct rgInfRarInfo
{
 uint8_t              numRaRntis;    /*!< Indicates the number of valid RA-RNTIs present. */
 RgInfRaRntiInfo *raRntiInfo;   /*!< Contains allocation information per RA-RNTI.  */
 uint16_t             txPwrOffset;   /*!< PDSCH tx power offset for RAR transmission */ 
}RgInfRarInfo;

/**
 *  @brief Logical channel allocation information. 
 */
typedef struct rgInfLcDatInfo
{
   CmLList  lchLstEnt;   /*!< Is not used when part of resource allocation. */
   uint8_t       lcId;        /*!< Identifies the logical channel. lcId value range is defined
                              in Section 6.2.1 in 36.321 specification. */
   uint32_t      numBytes;    /*!< Number of bytes allocated/received to logical channel. */
} RgInfLcDatInfo; 

/**
 * @brief This structure contains control element information received from a 
 *  particular UE.Extended PHR information
 */
typedef struct rgInfExtPhrSCellInfo
{
   uint8_t    sCellIdx;      /*!< SCELL index for which PHR is reported */
   uint8_t    phr;  /*!< PHR value. PHR value is defined in Section 6.1.3.6a in 36.321 specification. */
   uint8_t    pCmax; /*!< PCMAX value. PCMAX value is defined in Table 6.1.3.6a-1 in 36.321 specification. */
   uint8_t    pBackOff; /*!< If UE applied back off due to P-MPRc in 36.321 specification. */
}RgInfExtPhrSCellInfo;
typedef struct rgInfExtPhrCEInfo
{
   uint8_t    type2Phr;  /*!< PHR value. PHR value is defined in Section 6.1.3.6a in 36.321 specification. */
   uint8_t    type2PCMax; /*!< PCMAX value. PCMAX value is defined in Table 6.1.3.6a-1 in 36.321 specification. */
   uint8_t    numServCells; /*!< Number of serving cells for which PHR is reported */
   RgInfExtPhrSCellInfo servCellPhr[CM_LTE_MAX_CELLS];
}RgInfExtPhrCEInfo;  /*!< EXT PHR value. EXT PHR value is defined in Section 6.1.3.6a in 36.321 R10 specification. */

/**
 * @brief This structure contains control element information received from a 
 *  particular UE.
 */
typedef struct rgInfCeInfo
{
   uint16_t         bitMask;   /*!< Bitmask for the MAC Control elements present. */
   struct                                                  
   {                                                       
      uint16_t   cRnti;        /*!< C-RNTI value. RNTI range is specified in Section 7.1 in 36.321 specification. */
      uint8_t    phr;          /*!< PHR value. PHR value is defined in Section 6.1.3.6 in 36.321 specification. */
      union                                                
      {                                                    
         uint8_t    truncBsr;  /*!< Truncated BSR value. BSR value is defined in 
                               Section 6.1.3.1 in 36.321 specification. */
         uint8_t    shortBsr;  /*!< Short BSR  value.BSR value is defined in 
                               Section 6.1.3.1 in 36.321 specification. */
         struct                                            
         {                                                 
            uint8_t    bs1;   /*!< Buffer size 1 of long BSR. */ 
            uint8_t    bs2;   /*!< Buffer size 2 of long BSR. */ 
            uint8_t    bs3;   /*!< Buffer size 3 of long BSR. */ 
            uint8_t    bs4;   /*!< Buffer size 4 of long BSR. */ 
         }longBsr;       /*!< BSR value is defined in Section 6.1.3.1 in 36.321 specification. */ 
      }bsr;   
      RgInfExtPhrCEInfo extPhr;  /*!< EXT PHR value. EXT PHR value is defined in Section 6.1.3.6a in 36.321 R10 specification. */
#ifdef MAC_5GTF_UPDATE
 struct
      {
        uint8_t bar;         /*!< Beam Adjusment Request */
        uint8_t numBsiFields; /*!< Number of BSI fields UE sent in BSI Feedback */
        struct 
        {
           uint16_t bi;         /*!< BSI Feedback : 9 bits Beam Index */
           uint8_t brsrp;       /*!< BSI Feedback :7 bits BRSRP */
        }bsiFdbk[4];
      }beam;
#endif
   } ces;
      /* L2_COUNTERS */
#ifdef LTE_L2_MEAS
   uint8_t ulActLCs[RGINF_MAX_NUM_DED_LC];
                        /*!< List of LCs for which Data is received in UL */
#endif


#ifdef LTEMAC_SPS
   uint16_t spsSduSize;
#endif

} RgInfCeInfo;

typedef struct rgInfLcgInfo
{
   uint8_t    lcgId;        /*!< LCGID. GBR DRB LCG */
   uint32_t   bytesRcvd;    /*!< Contains bytes received for LCG */
}RgInfLcgInfo;

/** 
 * @brief This structure contains control element information received from a 
 *  particular UE identified by the RNTI.
 */
typedef struct rgInfUeDatInd
{
   CmLteRnti   rnti;      /*!< RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */
   CmLList     ueLstEnt;  /*!< UE linked list entry. */
   RgInfCeInfo ceInfo;    /*!< Contains the control elements received from the UE. */
   RgInfLcgInfo lcgInfo[RGINF_MAX_LCG_PER_UE - 1];  /*!< Contains the bytes received per DRB LCG. */
}RgInfUeDatInd;

/** 
 * @brief This structure carries the control element information received for a
          set of UEs along with timing information. 
 */
typedef struct rgInfSfDatInd
{
   CmMemListCp       memCp;      /*!< Memory control point. */
   CmLListCp         ueLst;      /*!< Pointer to the UE linked list. */
   /* RRM_SP1_START */
   uint32_t               qcisUlPrbCnt[RGINF_MAX_GBR_QCI_REPORTS];/*!< UL gbr LC's PRB count*/
   /* RRM_SP1_END */
   S16               cellSapId;  /*!< Cell SAP Identifier. CellSAP ID value must be within the set of configured cell SAP IDs. */
   CmLteCellId       cellId;     /*!< Identifies the cell. CellId must be within the set of configured cell IDs. */
   CmLteTimingInfo   timingInfo; /*!< Contains information about SFN and subframe. SFN ranges from 0 to 1023 whereas subframe ranges from 0 to 9. */
}RgInfSfDatInd;

/** 
 * @brief This structure carries the information about scheduled logical 
 *  channels within this transport block along with timing adjustment 
 *  information. 
 */
typedef struct rgInfUeTbInfo
{
   Bool                 disTb;       /*!< Currently, not used, but is 
                                          applicable in MIMO case. */
   Bool                 isReTx;      /*!< Indicates the TB transmission type. */
   TknUInt8                ta;          /*!< Timing Adjustment. */ 
#ifdef LTE_ADV
  TknUInt8                 sCellActCe;   /* !< SCell Act values and whether
                                          scheduled or not */
#endif

   /* Changed as a result of CR timer implementation*/
   uint8_t contResCe; /*!< Indicating presence of Contention Resolution CE across MAC-SCH
                   interface to
                  * identify CCCH SDU transmissions which need to
                  * be done without the
                  * contention resolution CE.*/

   uint8_t                   numSchLch;   /*!< Indicates the number of logical 
                                          channels scheduled. */    
   uint32_t                  schdTbSz; 
   RgInfLcDatInfo       schdDat[RGINF_MAX_NUM_DED_LC]; /*!< Contains 
                                          information about scheduled logical
                                          channel. */
} RgInfUeTbInfo;

/**
 @brief This structure carries the information reagarding secondary MAC like its
 instance Id and HARQ process's Id
 */
typedef struct rgLaaTbReqInfo
{
   uint8_t      sMacInstId;
   uint8_t      sCellHqPId;
   uint8_t      tbId;
   uint16_t     hqPStamp;
}RgLaaTbReqInfo;

/** 
 *  @brief This structure carries the UE-specific Resource allocation 
 *   information like RNTI, downlink control format, HARQ process Identifier, 
 *   scheduled TB size, and Transport Block information. 
 */
typedef struct rgInfUeAlloc
{
   CmLteRnti        rnti;      /*!< RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */ 

/* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteRnti            pdcchRnti;   /*!< RNTI used for PDCCH scrambling */
#endif



   TfuPdschDciInfo  dciInfo;   /*!< Contains Downlink Control Information. For more information, 
                                    refer to <i>TFU Interface Service Definition</i> (p/n 1100091). */
   uint8_t               hqProcId;  /*!< HARQ process Identifier. hqProcId ranges between 1 and 8 for FDD mode and 1 and 15 for TDD mode */
   S8                  tbStrtIdx;
   TknUInt32              doa;
   TfuTxMode           txMode;
   Bool                puschRptUsd;/*!< True, if Precoding Information in PDCCH has to be
                                   in-accordance with the latest PUSCH report */
   TfuDlCqiPuschInfo   puschPmiInfo;/*!< PUSCH report details for explicit PMI
                                       * information to PHY during a PDSCH */


   uint8_t               nmbOfTBs;  /*!< Indicates the number of TBs. Currently this is set to 1. */
   RgInfUeTbInfo    tbInfo[RGINF_MAX_TB_PER_UE]; /*!< Contains TB identifier. */
   /* LTE_ADV_FLAG_REMOVED_START */
#ifdef TFU_UPGRADE
   uint8_t                  pa;          /*!<  DL Power control paramter P_A
                                        configured by higher layers
                                        Ref: RRC 36.331, 6.3.2, PDSCH-Config */
#endif
   uint8_t   isEnbSFR;     /*To check if SFR is enabled*/
   /* LTE_ADV_FLAG_REMOVED_END */
#ifdef LTE_ADV
   Bool                 fillCtrlPdu; /*!< Based upon this flag RLC will fill RLC Control PDU 
                                          In a tti if P-cell is present then control PDU 
                                          should be kept in P-cell otherwise S-cell*/
#endif
   RgLaaTbReqInfo   tbReqInfo;      /*!< LAA: TB information for the TBs which
                                     need to be fetched from the SCell*/
}RgInfUeAlloc;

/** 
 * @brief This structure carries the Resource allocation information for the 
 *  set of scheduled UEs. 
 */
typedef struct rgInfUeInfo
{
   uint8_t               numUes;    /*!< Number of UEs allocated. */
   RgInfUeAlloc     *allocInfo; /*!< Allocation information per UE. */
}RgInfUeInfo;

/** 
 * @brief This structure contains the RNTI and downlink control format 
 *  information for the scheduled BCCH logical channel.  
 */
typedef struct rgInfBcchInfo
{
   CmLteRnti            rnti;       /*!< RNTI range is specified in Section 7.1 in 36.321 specification. */ 
   TfuPdschDciInfo      dciInfo;    /*!< Downlink Control Information. For more information,
                                         refer to <i>TFU Interface Service Definition</i> (p/n 1100091).*/
/* Modified for SI Enhancement*/
#ifndef RGR_SI_SCH
   CmLteLcId            lcId;       /*!< Logical Channel Identifier.lcId value range is defined in 
                                         Section 6.2.1 in 36.321 specification. */
   Bool                 sndStatInd; /*!< Currently this is set to 1 for fresh transmission of BCCH data. */
#else
   Buffer               *pdu;       /*!< PDU being specified for BCCH. */
#endif
   uint16_t                  txPwrOffset;   /*!< PDSCH tx power offset for BCCH 
                                            transmission */ 
}RgInfBcchInfo;

/** 
 * @brief This structure contains the RNTI and the downlink control information for the scheduled PCCH logical channel. 
 */
typedef struct rgInfPcchInfo
{
   CmLteRnti            rnti;      /*!< RNTI range is specified in Section 7.1 in 36.321 specification. */ 
   TfuPdschDciInfo      dciInfo;   /*!< Downlink Control Information.
                                        For more information, refer to <i>TFU Interface Service Definition</i> (p/n 1100091). */
   CmLteLcId            lcId;      /*!< Logical Channel Identifier. lcId value range is defined in
                                        Section 6.2.1 in 36.321 specification. */
                                        
   uint16_t                  txPwrOffset;   /*!< PDSCH tx power offset for PCCH 
                                            transmission */ 
}RgInfPcchInfo;

/** 
 * @brief This structure contains the scheduled logical channel information 
 *  mapped onto the BCH transport channel.
 */
typedef struct rgInfBchInfo
{
/* Modified for SI Enhancement*/
#ifndef RGR_SI_SCH
   CmLteLcId       lcId;    /*!< Logical Channel Identifier. lcId value range is defined in Section 6.2.1 in 36.321 specification. */
#else
   Buffer          *pdu;    /*!< PDU being specified for BCH */
#endif
}RgInfBchInfo;

/** 
 * @brief This structure contains the scheduling information of BCH, PCCH, and 
 *  BCCH channel information.
 */
typedef struct rgInfCmnLcInfo
{
   uint16_t               bitMask;   /*!< Bitmask representing all the common channels present. */
   RgInfBchInfo      bchInfo;   /*!< BCH channel Information. */
   RgInfPcchInfo     pcchInfo;  /*!< Paging Logical Channel Information. */
   RgInfBcchInfo     bcchInfo;  /*!< Broadcast Logical Channel Information. */
}RgInfCmnLcInfo;

#ifdef EMTC_ENABLE
/** 
 * @brief This structure contains the scheduled logical channel information 
 *  mapped onto the BCH transport channel.
 */
typedef struct rgInfEmtcBcchInfo
{
   CmLteRnti         rnti;    /*!< RNTI range is specified in Section 7.1 in 36.321 specification. */ 
   TfuPdschDciInfo   dciInfo; /*!< Downlink Control Information. For more information */
   Buffer            *pdu;    /*!< PDU being specified for BCH */
}RgInfEmtcBcchInfo;

/** 
 * @brief This structure contains the scheduling information of BCH, PCCH, and 
 *  BCCH channel information.
 */
typedef struct rgInfEmtcCmnLcInfo
{
   uint16_t               bitMask;   /*!< Bitmask representing all the common channels present. */
   RgInfBchInfo      bchInfo;   /*!< BCH channel Information. */
   RgInfPcchInfo     pcchInfo;  /*!< Paging Logical Channel Information. */
   RgInfEmtcBcchInfo emtcBcchInfo;  /*!< Broadcast Logical Channel Information. */
}RgInfEmtcCmnLcInfo;
#endif

#ifdef LTE_L2_MEAS
/**
 * @brief UE-specific allocation information needed for measurements. 
 * */
typedef struct rgInfUeUlAlloc
{
   CmLteRnti            rnti;        /*!< UE ID */ 
   uint8_t                   numPrb;      /*!< Number of total PRB's allocated for this UE */
} RgInfUeUlAlloc;

/**
 * @brief Allocation information of all UEs in this subframe. 
 * */
typedef struct rgInfUlUeInfo
{
   uint8_t                   numUes;      /*!< Number of UE's*/
   CmLteTimingInfo      timingInfo;  /*!< Uplink timing information */
   RgInfUeUlAlloc       *ulAllocInfo;/*!< Uplink Allocations information */
}RgInfUlUeInfo;
#endif /*LTE_L2_MEAS */

/**
*@brief this structure contains the lcId on which flow control need to be performed and the number of packets allowed for admission */
typedef struct rgInfLcInfo
{
   CmLteLcId   lcId;        /*!< lcId for flow control*/
   uint32_t         pktAdmitCnt; /*!<Pkts allowed for the LC */
   uint32_t         maxBo4FlowCtrl; /*!< MAX BO of Queued SDUs at RLC beyond which Flow is Controlled */
}RgInfLcFlowCntrlInfo;


/** 
 * @brief This structure contains the flow control information per UE */
typedef struct rgInfUeFlowCntrlInfo 
{
   CmLteRnti            ueId;  /*!<ueId for flow control*/
   uint32_t                  numLcs;/*!<numLcs in the UE*/
   RgInfLcFlowCntrlInfo lcInfo[RGINF_MAX_NUM_DED_LC]; /*!<LC list*/
}RgInfUeFlowCntrlInfo;  
/**
* @brief This structure contains the flow control information to be sent to MAC */
typedef struct rgInfFlowCntrlInfo
{
   uint32_t                   numUes;/*!<Num UEs for flow control */
   RgInfUeFlowCntrlInfo  ueFlowCntrlInfo[RGINF_MAX_NUM_UE_PER_TTI]; /*!<Ue Info for flow control */
}RgInfFlowCntrlInfo;
#ifdef EMTC_ENABLE
typedef struct rgInfEmtcInfo
{
   RgInfEmtcCmnLcInfo  cmnLcInfo;     /*!< Contains Information about commmon channels. */
   RgInfRarInfo        rarInfo;       /*!< Contains Information about EMTC Random Access Responses. */
   RgInfUeInfo         ueInfo;        /*!< Contains UE specific allocation
                                            information for EMTC UEs. */
}RgInfEmtcInfo;
#endif
/** 
 * @brief This structure contains the scheduling information of common channels, random access responses and UE specific allocations along with timing information.
 */
typedef struct rgInfSfAlloc
{
   CmLteCellId         cellId;        /*!< Identifies the cell. CellId value should be within the set of configured
                                         cell IDs. */
   CmLteTimingInfo     timingInfo;    /*!< Contains Information about SFN and subframe. SFN ranges from 0 to 1023 where as subframe is from 0 to 9. */
   RgInfCmnLcInfo      cmnLcInfo;     /*!< Contains Information about commmon channels. */
#ifdef EMTC_ENABLE
   RgInfEmtcInfo       emtcInfo;
#endif
   RgInfRarInfo        rarInfo;       /*!< Contains Information about Random Access Responses. */
   RgInfUeInfo         ueInfo;        /*!< Contains UE specific allocation information. */
#ifdef LTE_L2_MEAS
   RgInfUlUeInfo       ulUeInfo;    /*!< Uplink allocation information needed at MAC */
#endif /* LTE_L2_MEAS */
   RgInfFlowCntrlInfo  flowCntrlInfo; /*!<Flow Control information */
}RgInfSfAlloc;

/**
* @brief This structure contains regarding the ue for
*  resettng the harqentiry of a scell
*/
typedef struct rgInfResetHqEnt
{
   CmLteCellId cellId;        /*!< Identifies the sec cell for which harq entity reset
                                   has to be done*/
   CmLteRnti   crnti;          /*!< RNTI for which harq entity needs to be 
                                            reset */
}RgInfResetHqEnt;

/** 
 * @brief This structure contains the UE specific HARQ and Transport Block 
 *  information to be released.  
 */
typedef struct rgInfUeHqInfo
{
   CmLteRnti   rnti;                        /*!< RNTI which uniquely identifies the UE. RNTI value range is 
                                                 specified in Section 7.1 in 25.321 */
   uint8_t          hqProcId;                    /*!< HARQ Process Identifier.
                                                 hqProcId ranges between 1 and 8 for FDD mode and 1 and 15 for TDD mode. */
   uint8_t          numOfTBs;                    /*!< Identifies the number of TBs. Currently this is set to 1. */
   uint8_t          tbId[RGINF_MAX_TB_PER_UE];   /*!< Indicates the TB Identifier. */
   /* MS_WORKAROUND: to increase Harq Fail Counter .
       The status field is required for tracking the number of harq faliures at MAC.
       As this is already present under L2_MEAS flag, we have replaced it with platform flag */
   uint8_t                   status[RGINF_MAX_TB_PER_UE];      /*!< Indicates HARQ ACK or NACK */ 
   uint8_t          rlsOperationType;            /*!< Decides action to be taken in MAC
                                                 0x00 RGINF_RLS_HQ_NO_ACTION - Free Hq Procs and TBs
                                                 0x01 RGINF_RLS_HQ_SAVE_TB - Free Hq Proc but save the TBs
                                                 0x02 RGINF_RLS_HQ_DEL_TB - Del already saved TBs */
   uint16_t         saveId;                     /*!< Uniquely identify an particular HqP save */                                              
} RgInfUeHqInfo;

/** 
 * @brief This structure contains HARQ process information and buffer 
 *  information for a set of UEs to be released in a cell identified by the 
 *  cell ID. 
 */
typedef struct rgInfRlsHqInfo
{
   CmLteCellId   cellId;           /*!< Identifies Cell. CellId should be within
                                        the set of configured cell IDs. */
   uint8_t            numUes;           /*!< Indicates number of UEs present. */
   RgInfUeHqInfo *ueHqInfo;        /*!< Contains UE specific HARQ information. */
}RgInfRlsHqInfo;

/** 
 * @brief This structure contains the released RNTI in a particular cell 
 *  identified by a cellId.
 */
typedef struct rgInfRlsRnti
{
   CmLteCellId   cellId;           /*!< cellId uniquely identifies the
                                        cell. cellId value should be within the set of configured cell IDs. */
   CmLteRnti     rnti;             /*!< RNTI which uniquely identifies the UE. 
                                        RNTI range is specified in Section 7.1 in 25.321 specification. */
   /* Fix : syed ueId change as part of reestablishment.
    * Now SCH to trigger this. CRG ueRecfg for ueId change 
    * is dummy */	   
   Bool          ueIdChng;         /*!< Indicates if there is a ueId change as part
                                        of reestablishment */
   CmLteRnti     newRnti;          /*!< new RNTI changed as part of reestablishment */
#ifdef LTE_ADV
   Bool          isUeSCellDel;         /*!< TRUE means UeSCellDel*/
#endif
}RgInfRlsRnti;

/** 
 * @brief This structure contains the cell information along with the 
 *  maximum number of downlink HARQ processes configured per UE in the cell.
 */
typedef struct rgInfCellReg
{
   CmLteCellId cellId;            /*!< Identifies the cell. cellId value should be within the set of configured cell IDs. */
   S16         cellSapId;         /*!< Identifies the cell SAP. cellSapId value should be within the set of configured cell SAP IDs. */
   uint8_t          maxDlHqProcPerUe;  /*!< Indicates Maximum number of downlink HARQ processes per UE. */
}RgInfCellReg;

#ifdef LTE_L2_MEAS
/**
 * @brief struture to hold measurement request for Uplink Average PRB usage per
 * QCI
 * */
typedef struct rgInfPrbReq
{
   uint8_t                   numQci;      /*!< Number fo QCI's in Request */
   uint8_t                   qci[LRG_MAX_QCI_PER_REQ];  /*!< QCI for which PRB has
                                          to be measured */
} RgInfPrbReq;


typedef struct prbUsage
{
   uint8_t                   qciValue;   /*!< Qci Value */
   uint32_t                  prbUsage;   /*!<Prb Usage for this Qci */
}PrbUsage;
/**
 * @brief struture to hold measurement confirm for Uplink Average PRB usage per
 * QCI
 * */
typedef struct rgInfPrbCfm
{
   uint8_t                   numQci;      /*!< Number fo QCI's in Request */
   PrbUsage             prbUsage[LRG_MAX_QCI_PER_REQ];  /*!< Average PRB usage
                                          per QCI */
} RgInfPrbCfm;

/**
 * @brief Structure used by Scheduler to send the measurement request to MAC 
 * */
typedef struct RgInfL2MeasReq
{
   uint32_t                  transId;     /*!< TransId to uniquely identify request */
   uint16_t                  measType;    /*!< For action type Avegare PRB usage 
                                          in Uplink */
   uint16_t                  timePrd;     /*!< Time Period for which measurement
                                          is done */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
   union {
      RgInfPrbReq       prbReq;      /*!< Avgerage PRB usage per QCI*/
   } t;
} RgInfL2MeasReq;

/**
 * @brief Structure used by Scheduler to send the measurement stop request to MAC 
 * */
typedef struct RgInfL2MeasStopReq
{
   uint32_t                  transId;     /*!< TransId to uniquely identify request */
   uint8_t                   measType;    /*!< For action type Avegare PRB usage 
                                          in Uplink */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
} RgInfL2MeasStopReq;


/**
 * @brief Structure used by Scheduler to send the measurement send request to MAC 
 * */
typedef struct RgInfL2MeasSndReq
{
   uint32_t                  transId;     /*!< TransId to uniquely identify request */
   uint8_t                   measType;    /*!< For action type Avegare PRB usage 
                                          in Uplink */
   uint16_t                  timePrd;     /*!< Time Period for which measurement
                                          is done */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
} RgInfL2MeasSndReq;

/**
 * @brief Structure used MAC to send the confirm back to scheduler 
 * */
typedef struct rgInfL2MeasCfm
{
   uint16_t                  transId;     /*!< transId to map to request */
   uint8_t                   measType;    /*!< Action for which measurement done */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
   CmStatus             cfm;        /*!< Confirmation possible Values when measType
                                  is invalid status -> NOK and reason -> INVALID */
   union {
      RgInfPrbCfm       prbCfm;      /*!< Avgerage PRB usage per QCI*/
   } u;
} RgInfL2MeasCfm;

/**
 * @brief This API is invoked from l2 Measurements module at scheduler.
 * When Scheduler receives a measurement request from stack manager for Average 
 * PRB usage Per QCI in Uplink, Scheduler invokes this API towards LTE MAC 
 * for the calculations. LTE MAC utilizes the uplink allocations information provided 
 * by scheduler for every subframe and data indications received for this calculation.
 * This API carries a transId to uniquely identify the confirm received for 
 * this request from LTEMAC.
 * */
S16 RgSchMacL2MeasReq ARGS((
   Pst*                 pst,
   RgInfL2MeasReq*      l2MeasReq
));
/**
 * @brief This API is invoked from l2 Measurements module at scheduler.
 * When Scheduler receives a measurement send request from stack manager, 
 * it sends L2 measurement to layer manager.
 * */

S16 RgSchMacL2MeasSendReq ARGS((
   Pst*                 pst,
   RgInfL2MeasSndReq*      l2MeasReq
));
/**
 * @brief This API is invoked from l2 Measurements module at scheduler.
 * When Scheduler receives a measurement stop request from stack manager,
 * it stops L2 Measurement 
 */
S16 RgSchMacL2MeasStopReq ARGS((
   Pst*                 pst,
   RgInfL2MeasStopReq*      l2MeasReq
));

/**
 * @brief This API is invoked from L2 Measurement module at LTE MAC. When LTE MAC
 * completes calculation of Avergae PRB usage per QCI in Uplink for a given time 
 * period, It invokes this API to indicate result back to scheduler. LTE  
 * MAC sends the same transId received in measurement request from Scheduler.
 * */
S16 RgMacSchL2MeasCfm ARGS((
   Pst*                 pst,
   RgInfL2MeasCfm*      l2MeasCfm
));
/**
 * @brief This API is invoked from L2 Measurement module at LTE MAC. When LTE MAC
 * stops L2 measurement, it sends L2 measurement cfm.
 * */

S16 RgMacSchL2MeasStopCfm ARGS((
   Pst*                 pst,
   RgInfL2MeasCfm*      l2MeasCfm
));

S16 RgMacSchL2MeasStop ARGS
((
Pst*                pst,   
RgInfL2MeasCfm      *measInfo
));
#endif /* LTE_L2_MEAS */

typedef struct rgInfLcgRegReq
{
   CmLteCellId          cellId;      /*!< Cell Identifier */
   CmLteRnti            crnti;       /*!< RNTI which uniquely identifies the UE
                                          RNTI range is specified in Section 
                                          7.1 in 25.321 Specification. */
   uint8_t                   lcgId;
   Bool                 isGbr;       /* Indicate if the LCG is Gbr */
} RgInfLcgRegReq;


/* Added support for SPS*/
#ifdef LTEMAC_SPS
/** 
 * @brief This structure contains the logical channel information related to
 *  uplink SPS for a particular UE identified by the CRNTI.
 */
typedef struct rgInfSpsLcInfo
{
   CmLteCellId          cellId;      /*!< Cell Identifier */
   CmLteRnti            crnti;       /*!< RNTI which uniquely identifies the UE
                                          RNTI range is specified in Section 
                                          7.1 in 25.321 Specification. */
   CmLteRnti            spsRnti;     /*!< SPS RNTI. RNTI range is specified in
                                          Section 7.1 in 25.321 Specification. */
   uint8_t                   spsLcCnt;    /*!< identifies the number of SPS
                                          configured logical channels */ 
   uint8_t                   spsLcId[RGINF_MAX_NUM_DED_LC]; /*!< Logical Channel
                                          Identifier. lcId value range is 
                                          defined in Section 6.2.1
                                          in 36.321 Specification. */
   uint8_t                   implRelCnt;  /*!< "implicitRelAfter" vallue */  
   uint16_t                  spsPrd;      /*!< SPS periodicity of the UE */
} RgInfSpsLcInfo;

/** 
 * @brief This structure contains UL SPS param Reset related to
 *  uplink SPS for a particular UE identified by the CRNTI.
 */
typedef struct rgInfUlSpsReset
{
   CmLteCellId          cellId;      /*!< Cell Identifier */
   CmLteRnti            crnti;       /*!< RNTI which uniquely identifies the UE
                                          RNTI range is specified in Section 
                                          7.1 in 25.321 Specification. */
} RgInfUlSpsReset;


/** 
 * @brief This structure contains the information to release UL SPS for a UE */
typedef struct rgInfSpsRelInfo
{
   S16                  cellSapId;   /*!< identifies the cell SAP. Value range
                                          should be within the set of 
                                          configured cell SAP(s).*/
   CmLteRnti            cRnti;       /*!< RNTI of the UE */
   Bool                 isExplRel;    /*!< TRUE if explicit release needs to be
                                          sent to UE */
} RgInfSpsRelInfo;
#endif /* LTEMAC_SPS */

/* 
 * Function Prototypes 
 */
/** 
 * @brief Request from Scheduler to MAC to register a cell. 
 * @details This primitive is used for light-weight loose coupling.
 */
S16 cmPkSchMacCellRegReq ARGS((
   Pst*                 pst,    
   RgInfCellReg*        regReq  
));

typedef S16 (*CellRegReq) ARGS((
   Pst*                 pst,
   RgInfCellReg*        regReq
));

/** 
 * @brief Request from Scheduler to MAC to register a cell. 
 * @details The scheduler invokes this primitive after the scheduler cell
 *  configuration is completed. Before calling this primitive, the scheduler
 *  creates a mapping of the cell which is uniquely idetified by cell ID and 
 *  scheduler instance that is serving the cell.
 */
S16 RgSchMacCellRegReq ARGS((
   Pst*                 pst,
   RgInfCellReg*        regReq
));
/** 
 * @brief Request from Scheduler to MAC to register a cell. 
 * @details This primitive is used for light-weight loose coupling.
 */
S16 cmUnpkSchMacCellRegReq ARGS((
   CellRegReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from MAC to scheduler to update dedicated BO.
 * @details This primitive is used for light-weight loose coupling.
 */
S16 cmPkMacSchDedBoUpdtReq ARGS((
   Pst*                 pst,    
   RgInfDedBoRpt*       boRpt  
));

typedef S16 (*DedBoUpdtReq) ARGS((
   Pst*                 pst,
   RgInfDedBoRpt*       boRpt
));

/** 
 * @brief Request from MAC to scheduler to update dedicated BO.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives status response from the RLC for dedicated channels. Scheduler
 *  takes the buffer occupancy information into consideration while taking
 *  scheduling decisons. 
 */
S16 RgMacSchDedBoUpdtReq ARGS((
   Pst*                 pst,
   RgInfDedBoRpt*       boRpt
));
/** 
 * @brief Request from MAC to scheduler to update dedicated BO.
 * @details This primitive is used for light-weight loose coupling.
 */
S16 cmUnpkMacSchDedBoUpdtReq ARGS((
   DedBoUpdtReq         func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from MAC to scheduler to update common channel BO.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkMacSchCmnBoUpdtReq ARGS((
   Pst*                 pst,    
   RgInfCmnBoRpt*       boRpt  
));

typedef S16 (*CmnBoUpdtReq) ARGS((
   Pst*                 pst,
   RgInfCmnBoRpt*       boRpt
));

/** 
 * @brief Request from MAC to scheduler to update common channel BO.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives status response from the RLC for common channels. Scheduler
 *  takes the buffer occupancy information into consideration while taking
 *  scheduling decisons. 
 */
S16 RgMacSchCmnBoUpdtReq ARGS((
   Pst*                 pst,
   RgInfCmnBoRpt*       boRpt
));
/** 
 * @brief Request from MAC to scheduler to update common channel BO.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmUnpkMacSchCmnBoUpdtReq ARGS((
   CmnBoUpdtReq         func,
   Pst*                 pst,
   Buffer               *mBuf
));

/*Fix: start:Indicate UE deletion at MAC to scheduler*/
/** 
 * @brief UE delete indication from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkMacSchUeDelInd ARGS((
   Pst*                 pst,    
   RgInfUeDelInd*       ueDelInd
));

typedef S16 (*UeDelInd) ARGS((
   Pst*                 pst,
   RgInfUeDelInd*       ueDelInd
));

/** 
 * @brief UE deletion indication from MAC to scheduler.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives UE delete Request. As the UE is now deleted at MAC, it should 
 *  not be scheduled.
 */
S16 RgMacSchUeDelInd ARGS((
   Pst*                 pst,
   RgInfUeDelInd*       ueDelInd
));
/** 
 * @brief UE delete Indication Request from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmUnpkMacSchUeDelInd ARGS((
   UeDelInd            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/*Fix: end:Indicate UE deletion at MAC to scheduler*/
/** 
 * @brief Data Indication Request from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkMacSchSfRecpInd ARGS((
   Pst*                 pst,    
   RgInfSfDatInd*       datInd
));

typedef S16 (*SfRecpInd) ARGS((
   Pst*                 pst,
   RgInfSfDatInd*       datInd
));

/** 
 * @brief Data Indication Request from MAC to scheduler.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives MAC Control Elements from the data received from the UE. Scheduler
 *  takes these control elements into consideration while taking scheduling 
 *  decisons for the uplink.
 */
S16 RgMacSchSfRecpInd ARGS((
   Pst*                 pst,
   RgInfSfDatInd*       datInd
));
/** 
 * @brief Data Indication Request from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmUnpkMacSchSfRecpInd ARGS((
   SfRecpInd            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Resource Allocation Request from Scheduler to MAC.
 * @details This primitive is used for light-weight loose coupling.
 */ 
S16 cmPkSchMacSfAllocReq ARGS((
   Pst*                 pst,    
   RgInfSfAlloc*        resAllocReq
));

typedef S16 (*SfAllocReq) ARGS((
   Pst*                 pst,
   RgInfSfAlloc*        resAllocReq
));

/** 
 * @brief Resource Allocation Request from Scheduler to MAC.
 * @details Scheduler invokes this primitive for every TTI towards MAC to  
 *  inform the scheduling decisions taken for uplink grants, common channels
 *  and list of UEs to be scheduling during this TTI.
 */
S16 RgSchMacSfAllocReq ARGS((
   Pst*                 pst,
   RgInfSfAlloc*        resAllocReq
));
/** 
 * @brief Resource Allocation Request from Scheduler to MAC.
 * @details This primitive is used for light-weight loose coupling.
 */ 
S16 cmUnpkSchMacSfAllocReq ARGS((
   SfAllocReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from Scheduler to release HARQ processes at MAC.
 * @details This primitive is used for light-weight loose coupling.
 */
S16 cmPkSchMacRlsHqReq ARGS((
   Pst*                 pst,    
   RgInfRlsHqInfo*      sfHqInfo
));

typedef S16 (*RlsHqReq) ARGS((
   Pst*                 pst,
   RgInfRlsHqInfo*      sfHqInfo
));

/** 
 * @brief Request from Scheduler to release HARQ processes at MAC.
 * @details  Scheduler calls this primitive to send the list of UEs for  
 *  which the HARQ buffers are released to MAC. The Scheduler invokes this
 *  primitive when a positive acknowledgement is received for the TB transmitted
 *  or a TB is retransmitted for the allowed maximum number of retransmissions. 
 */
S16 RgSchMacRlsHqReq ARGS((
   Pst*                 pst,
   RgInfRlsHqInfo*      sfHqInfo
));
/** 
 * @brief Request from Scheduler to release HARQ processes at MAC.
 * @details This primitive is used for light-weight loose coupling.
 */

S16 cmUnpkSchMacRlsHqReq ARGS((
   RlsHqReq             func,
   Pst*                 pst,
   Buffer               *mBuf
));

/** 
 * @brief Request from Scheduler to reset HARQ Entity at MAC.
 * @details This primitive is used for light-weight loose coupling.
 */
S16 cmPkSchMacRstHqEntReq ARGS((
   Pst*                 pst,    
   RgInfResetHqEnt*     hqEntInfo
));

typedef S16 (*RstHqEntReq) ARGS((
   Pst*                 pst,    
   RgInfResetHqEnt*     hqEntInfo
));

/** 
 * @brief Request from Scheduler to reset HARQ entity at MAC for a scell of an ue.
 * This is triggered upon deactivation of a scell
 */
S16 RgSchMacRstHqEntReq ARGS((
   Pst*                 pst,
   RgInfResetHqEnt*     hqEntInfo
));
/** 
 * @brief Request from Scheduler to release HARQ processes at MAC.
 * @details This primitive is used for light-weight loose coupling.
 */

S16 cmUnpkSchMacRstHqEntReq ARGS((
   RstHqEntReq          func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from Scheduler to release RNTI at MAC.
 * @details  This primitive is used for light-weight loose coupling.
 */
S16 cmPkSchMacRlsRntiReq ARGS((
   Pst*                 pst,    
   RgInfRlsRnti*        rlsRnti
));

typedef S16 (*RlsRntiReq) ARGS((
   Pst*                 pst,
   RgInfRlsRnti*        rlsRnti
));

/** 
 * @brief Request from Scheduler to release RNTI at MAC.
 * @details The Scheduler calls this primitive to send the list of RNTIs for 
 *  which the RRC Connection is being rejected.
 */
S16 RgSchMacRlsRntiReq ARGS((
   Pst*                 pst,
   RgInfRlsRnti*        rlsRnt
));
/** 
 * @brief Request from Scheduler to release RNTI at MAC.
 * @details  This primitive is used for light-weight loose coupling.
 */
S16 cmUnpkSchMacRlsRntiReq ARGS((
   RlsRntiReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* Added support for SPS*/

typedef S16 (*LcgReg) ARGS((
   Pst*                 pst,    
   RgInfLcgRegReq       *lcgRegReq    
));

S16 cmPkSchMacLcgRegReq ARGS((
   Pst*                 pst,
   RgInfLcgRegReq       *lcgRegReq  
));

S16  RgSchMacLcgRegReq ARGS((Pst *pst, RgInfLcgRegReq *lcgRegReq));

S16 cmUnpkSchMacLcgRegReq ARGS((
   LcgReg          func,
   Pst             *pst,
   Buffer          *mBuf
));

S16  RgSchMacLcgReg ARGS((Pst* pst, RgInfLcgRegReq *lcgRegReq));

#ifdef LTEMAC_SPS
/** 
 * @brief Primitive from Scheduler to MAC to register the logical channels of
 * a SPS UE
 * @details  This primitive is used for light-weight loose coupling.
 */
S16 cmPkSchMacSpsLcRegReq ARGS((
   Pst*                 pst,
   RgInfSpsLcInfo       *lcInfo    
));

typedef S16 (*SpsLcReg) ARGS((
   Pst*                 pst,    
   RgInfSpsLcInfo       *lcInfo    
));

/** 
 * @brief Request from Scheduler to register the SPS related logical channels.
 * @details  Scheduler calls this primitive to send the list of logical channels
 *  that belong to the SPS logical channel group.   
 */
S16  RgSchMacSpsLcRegReq ARGS((Pst *pst, RgInfSpsLcInfo *lcInfo));

S16 cmUnpkSchMacSpsLcRegReq ARGS((
   SpsLcReg        func,
   Pst             *pst,
   Buffer          *mBuf
));


/** 
 * @brief Primitive from Scheduler to MAC to Reset UL SPS related Params
 * @details  This primitive is used for light-weight loose coupling.
 */
S16 cmPkSchMacUlSpsResetReq ARGS((
   Pst*                 pst,
   RgInfUlSpsReset       *ulSpsResetInfo    
));

typedef S16 (*UlSpsReset) ARGS((
   Pst*                 pst,    
   RgInfUlSpsReset       *ulSpsResetInfo    
));

/** 
 * @brief Request from Scheduler to reset UL SPS Params
 * @details  Scheduler calls this primitive to reset implicit and explicit
 * release counters for the UE
 */
S16  RgSchMacUlSpsResetReq ARGS((Pst *pst, RgInfUlSpsReset *ulSpsResetInfo));

S16 cmUnpkSchMacUlSpsResetReq ARGS((
   UlSpsReset        func,
   Pst             *pst,
   Buffer          *mBuf
));



/** 
 * @brief Primitive from Scheduler to MAC to deregister the logical channels of
 * a SPS UE
 * @details  This primitive is used for light-weight loose coupling.
 */
S16 cmPkSchMacSpsLcDeregReq ARGS((
   Pst*                 pst,    
   CmLteCellId          cellId,
   CmLteRnti            crnti
));

typedef S16 (*SpsLcDereg) ARGS((
   Pst*                 pst,    
   CmLteCellId          cellId,
   CmLteRnti            crnti
));

/** 
 * @brief Request from Scheduler to deregister the SPS related logical channels.
 * @details  Scheduler calls this primitive to send the deregistration request
 * for a UE once SPS is released for it
 */
S16  RgSchMacSpsLcDeregReq ARGS((Pst *pst, CmLteCellId cellId, CmLteRnti
    crnti));

S16 cmUnpkSchMacSpsLcDeregReq ARGS((
   SpsLcDereg           func,
   Pst*                 pst,
   Buffer               *mBuf
));

/** 
 * @brief Primitive from MAC to Scheduler to indicate release of UL SPS for a UE
 * @details  This primitive is used for light-weight loose coupling.
 */
S16 cmPkMacSchSpsRelInd ARGS((
   Pst*                 pst,    
   RgInfSpsRelInfo*     relInfo
));

typedef S16 (*SpsRelInd) ARGS((
   Pst*                 pst,    
   RgInfSpsRelInfo*     relInfo
));

/** 
 * @brief Indication from  MAC to Scheduler to release UL SPS for a UE 
 * @details  MAC calls this primitive to inform the scheduler that UL SPS needs
 * to be released for a UE
 */
S16  RgMacSchSpsRelInd ARGS((Pst *pst, RgInfSpsRelInfo *relInfo));

S16 cmUnpkMacSchSpsRelInd ARGS((
   SpsRelInd            func,
   Pst*                 pst,
   Buffer               *mBuf
));

#endif
#ifdef LTE_L2_MEAS
S16 cmPkMacSchL2MeasCfm ARGS((
Pst*                 pst,
RgInfL2MeasCfm       *measCfm
));

S16 cmPkMacSchL2MeasStopCfm ARGS((
Pst*                 pst,
RgInfL2MeasCfm       *measCfm
));


S16 cmPkSchMacL2MeasReq ARGS((
   Pst*                 pst,
   RgInfL2MeasReq      *measInfo
));

S16 cmPkSchMacL2MeasStopReq ARGS((
   Pst*                 pst,
   RgInfL2MeasStopReq *measInfo
));

S16 cmPkSchMacL2MeasSendReq ARGS((
   Pst*                 pst,
   RgInfL2MeasSndReq *measInfo
));
typedef S16 (*L2MeasReq) ARGS((
   Pst*                 pst,    
   RgInfL2MeasReq       *measInfo
));

typedef S16 (*L2MeasStopReq) ARGS((
   Pst*                 pst,
   RgInfL2MeasStopReq *measInfo
));

typedef S16 (*L2MeasSendReq) ARGS((
   Pst*                 pst,
   RgInfL2MeasSndReq *measInfo
));
typedef S16 (*L2MeasStopCfm) ARGS((
   Pst                *pst,
   RgInfL2MeasCfm     *measCfm
));

typedef S16 (*L2MeasCfm) ARGS((
   Pst                *pst, 
   RgInfL2MeasCfm     *measCfm
));
S16 cmUnpkMacSchL2MeasCfm ARGS
((
L2MeasCfm      func,
Pst            *pst,
Buffer         *mBuf
));
S16 cmUnpkSchMacL2MeasReq ARGS((
   L2MeasReq      func,
   Pst            *pst,
   Buffer         *mBuf
));
S16 cmUnpkSchMacL2MeasSendReq ARGS((
   L2MeasSendReq      func,
   Pst            *pst,
   Buffer         *mBuf
));
S16 cmUnpkSchMacL2MeasStopReq ARGS((
   L2MeasStopReq      func,
   Pst            *pst,
   Buffer         *mBuf
));

S16 cmUnpkMacSchL2MeasStopCfm ARGS((
   L2MeasCfm      func,
   Pst            *pst,
   Buffer         *mBuf
));
#endif
S16  RgSchMacRlsRnti ARGS((Pst* pst, RgInfRlsRnti* rlsRnti));
S16  RgSchMacRlsHq ARGS((Pst* pst, RgInfRlsHqInfo* sfHqInfo));
S16  RgSchMacSfAlloc ARGS((Pst* pst, RgInfSfAlloc* resAllocReq));
S16  RgSchMacRstHqEnt ARGS((Pst* pst, RgInfResetHqEnt* hqEntInfo));
S16  RgMacSchSfRecp ARGS((Pst* pst, RgInfSfDatInd* datInd));
S16  RgMacSchCmnBoUpdt ARGS(( Pst* pst, RgInfCmnBoRpt* boRpt));
S16 RgMacSchDedBoUpdt ARGS(( Pst* pst, RgInfDedBoRpt* boRpt));
S16  RgSchMacCellReg ARGS((Pst* pst,RgInfCellReg* regReq));
#ifdef LTE_L2_MEAS
S16 RgSchMacL2Meas ARGS((Pst* pst, RgInfL2MeasReq* l2MeasReq));
S16 RgMacSchL2Meas ARGS((Pst* pst, RgInfL2MeasCfm* l2MeasCfm));
S16 RgSchMacL2MeasStop ARGS((Pst* pst, RgInfL2MeasStopReq *measInfo));
S16 RgSchMacL2MeasSend ARGS((Pst* pst, RgInfL2MeasSndReq *measInfo));
#endif  /* LTE_L2_MEAS */
/* Added support for SPS*/
#ifdef LTEMAC_SPS
S16 RgSchMacSpsLcReg ARGS((Pst *pst, RgInfSpsLcInfo *lcInfo));
S16 RgSchMacUlSpsReset ARGS((Pst *pst, RgInfUlSpsReset *lcInfo));
S16 RgSchMacSpsLcDereg ARGS((Pst *pst, CmLteCellId cellId, CmLteRnti
    crnti));
S16 RgMacSchSpsRel ARGS((Pst *pst, RgInfSpsRelInfo* relInfo));
#endif
S16 RgMacSchUeDel ARGS((Pst* pst, RgInfUeDelInd*  ueDelInd));
#ifdef __cplusplus
}
#endif
#endif /* __GKSCH_X__*/

/**********************************************************************
  
         End of file
**********************************************************************/
