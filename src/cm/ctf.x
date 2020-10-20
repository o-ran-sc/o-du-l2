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

  Name:     LTE PHY layer
  
  Type:     C Include File 
  
  Desc:     Structures, variables, and typedefs required by the LTE PHY 
            and PHY-User Control (CTF) interface.

  File:     ctf.x 

**********************************************************************/

#ifndef __CTF_X__
#define __CTF_X__

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @file 
  * @brief CTF Interface File.
  */

/**
 * @brief
 * Configuration transaction identifier.
 */
/* Data Structures for CTF */

/** 
  @brief Transaction ID between CTF and User. */
typedef struct ctfCfgTransId {
   uint8_t trans[CTF_CFG_TRANSID_SIZE]; /*!< User transaction ID */
} CtfCfgTransId;


/* Enumerations for CTF */
/**
 * @enum ctfBandwidth
 * This element enumerates the Bandwidth values for PHY in terms of RBs. */
typedef enum {
   CTF_BW_RB_6,
   CTF_BW_RB_15,
   CTF_BW_RB_25,
   CTF_BW_RB_50,
   CTF_BW_RB_75,
   CTF_BW_RB_100   
} CtfBandwidth;

/**
 * @enum ctfDuplexMode
 * This element enumerates the duplex mode for the entire system at PHY. 
*/
typedef enum {
   CTF_DUPMODE_FDD=1,
   CTF_DUPMODE_TDD   
} CtfDuplexMode;

/**
 * @enum ctfSCSpacing
 * This element enumerates the sub-carrier spacing for configuration at PHY. 
*/
typedef enum {
   CTF_SC_SPACING_15KHZ,
   CTF_SC_SPACING_7DOT5KHZ   
} CtfSCSpacing;

/**
 * @enum ctfCPCfg
 * This element enumerates the cyclic prefix for use at PHY.
*/
typedef enum {
   CTF_CP_NORMAL,
   CTF_CP_EXTENDED   
} CtfCPCfg;

/**
 * @brief
 * This element enumerates the number of antenna ports for configuration at PHY.
*/
typedef enum {
   CTF_AP_CNT_1 = 1,
   CTF_AP_CNT_2 = 2,
   CTF_AP_CNT_4 = 4  
} CtfAntennaPortsCount;

/**
 * @enum ctfPhichDuration
 * This element enumerates the duration for PHICH configuration at PHY.
*/
typedef enum {
   CTF_PHICH_DUR_NORMAL,
   CTF_PHICH_DUR_EXTENDED   
} CtfPhichDuration;

/**
 * @enum ctfPhichResource
 * This element enumerates the resources for PHICH configuration at PHY.
*/
typedef enum {
   CTF_PHICH_RSRC_ONESIXTH,
   CTF_PHICH_RSRC_HALF,
   CTF_PHICH_RSRC_ONE,
   CTF_PHICH_RSRC_TWO   
} CtfPhichResource;

/**
 * @enum ctfPuschHoppingMode
 * This element enumerates the hopping mode for PUSCH configuration at PHY.
*/
typedef enum {
   CTF_PUSCH_HOPMODE_INTER_SUBFRM,
   CTF_PUSCH_HOPMODE_INTRAANDINTERSUBFRM   
} CtfPuschHoppingMode;

/**
 * @enum ctfPucchDeltaShift
 * This element enumerates the delta shift for PUCCH configuration at PHY.
*/
typedef enum {
   CTF_PUCCH_DELTASHIFT1,
   CTF_PUCCH_DELTASHIFT2,
   CTF_PUCCH_DELTASHIFT3   
} CtfPucchDeltaShift;

/**
 * @enum ctfPdschPaParam
 * This element enumerates the parameter PA for PDSCH configuration at PHY.
 * Each enum value maps to a PA value in dBs.
*/
typedef enum {
   CTF_PDSCH_PA_MINUS_6,
   CTF_PDSCH_PA_MINUS_4DOT77,
   CTF_PDSCH_PA_MINUS_3,
   CTF_PDSCH_PA_MINUS_1DOT77,
   CTF_PDSCH_PA_ZERO,
   CTF_PDSCH_PA_ONE,
   CTF_PDSCH_PA_TWO,
   CTF_PDSCH_PA_THREE   
} CtfPdschPaParam;

/**
 * @enum CtfCqiRptModeAperiodic
 * This element enumerates the Aperiodic CQI reporting configuration at PHY.
*/
typedef enum {
   CTF_CQI_RPT_MODE_APERIODIC_RM12, /*!< Corresponds to Mode 1-2 */
   CTF_CQI_RPT_MODE_APERIODIC_RM20, /*!< Corresponds to Mode 2-0 */
   CTF_CQI_RPT_MODE_APERIODIC_RM22, /*!< Corresponds to Mode 2-2 */
   CTF_CQI_RPT_MODE_APERIODIC_RM30, /*!< Corresponds to Mode 3-0 */
   CTF_CQI_RPT_MODE_APERIODIC_RM31  /*!< Corresponds to Mode 3-1 */
} CtfCqiRptModeAperiodic;

/**
 * @enum CtfUeTxAntSelectType 
 * This element enumerates the UE TX Antenna selection configuration at PHY.
*/
typedef enum {
   CTF_UE_TX_ANT_SELECT_CLOSEDLOOP, /*!< Closed loop selection */
   CTF_UE_TX_ANT_SELECT_OPENLOOP    /*!< Open loop selection */
} CtfUeTxAntSelectType;

/**
 * @enum CtfPucchRepFactor 
 * This element enumerates the repetition factors for PUCCH configuration.
*/
typedef enum {
   CTF_PUCCH_REP_FACTOR_N2,   /*!< Pucch repetition factor of 2 */
   CTF_PUCCH_REP_FACTOR_N4,   /*!< Pucch repetition factor of 4 */
   CTF_PUCCH_REP_FACTOR_N6    /*!< Pucch repetition factor of 6 */
} CtfPucchRepFactor;

#ifdef CTF_VER3
/* ctf_x_001.main_4:115549: */
 /**
  * @enum CtfTddAckNackFBMode
  * This element enumerates the ACK/NACK feedback mode for TDD mode of
  *  PUCCH configuration.
 */
 typedef enum {
    CTF_PUCCH_TDD_ACKNACK_FB_MODE_BUNDLING, /*!< Bundling option */
    CTF_PUCCH_TDD_ACKNACK_FB_MODE_MUXING    /*!< Multiplexing option */
 } CtfTddAckNackFBMode;
#endif /* CTF_VER3 */

/**
 * @enum CtfUeTxAntMode 
 * This element enumerates the UE Transmission mode.
*/
typedef enum {
   CTF_UE_TX_ANT_MODE_TM1, /*!< Transmission Mode 1 */
   CTF_UE_TX_ANT_MODE_TM2, /*!< Transmission Mode 2 */
   CTF_UE_TX_ANT_MODE_TM3, /*!< Transmission Mode 3 */
   CTF_UE_TX_ANT_MODE_TM4, /*!< Transmission Mode 4 */
   CTF_UE_TX_ANT_MODE_TM5, /*!< Transmission Mode 5 */
   CTF_UE_TX_ANT_MODE_TM6, /*!< Transmission Mode 6 */
   CTF_UE_TX_ANT_MODE_TM7 /*!< Transmission Mode 7 */
/* ctf_x_001.main_2: Added transmission mode 8*/
#ifdef CTF_VER1
   ,CTF_UE_TX_ANT_MODE_TM8  /*!< Transmission Mode 8 */
#endif
} CtfUeTxAntMode;
/* ctf_x_001.main_3:Added enum for CTF reconfiguration */
/**
 * @enum CtfReCfgType 
 * This element enumerates the CTF reconfiguration type
*/
typedef enum {
   CTF_MAJOR_RECFG,   /*!< CTF Major reconfiguration */
   CTF_MINOR_RECFG   /*!< CTF Minor reconfiguration */
} CtfReCfgType;

/**
 *  @brief
 *  UE ID information
 */
typedef struct ctfUeInfo {
   CmLteCellId    cellId;     /*!< Cell ID */
   CmLteRnti      ueId;       /*!< UE ID */
}CtfUeInfo;

/* Data Structures for CTF */
/**
 * @brief   Bandwidth Configuration.
 * @details This structure contains the uplink and downlink bandwidth 
 *          information for configuration at PHY.
*/
typedef struct ctfBwCfgInfo {
   Bool   pres;       /*!< Present field */
   CtfBandwidth   dlBw;       /*!< Downlink Bandwidth in RBs */
   CtfBandwidth   ulBw;       /*!< Uplink Bandwidth in RBs */
   uint8_t             eUtraBand;  /*!< EUTRA Operating Band - 36.104 (5.5)
                                   FDD:(1..14), TDD:(33..40) */
} CtfBwCfgInfo;

/**
 * @brief   Transmission Scheme Configuration.
 * @details This structure contains the transmission scheme related information.
*/
typedef struct ctfTxSchemeCfg {
   Bool pres; /*!< Present field */
   CtfDuplexMode  duplexMode; /*!< Duplexing Mode: TDD/FDD */
   CtfSCSpacing   scSpacing;  /*!< Sub-carrier spacing */
   CtfCPCfg       cycPfx;     /*!< Cyclic prefix */
} CtfTxSchemeCfg;

/**
 * @brief   Antenna Ports configuration.
 * @details This structure contains the antenna configuration information for 
 *          configuration at PHY.
*/
typedef struct ctfAntennaCfgInfo {
   CtfAntennaPortsCount   antPortsCnt; /*!< Count of antenna ports */
} CtfAntennaCfgInfo;

/**
 * @brief   PRACH configuration.
 * @details This structure contains the configuration information for PRACH at PHY.
 *          -# PRACH preamble sequences are generated by PHY using Zadoff-Chu 
 *             sequences.[Ref: 36.211, 5.7.2]
 *          -# PRACH preamble format is derived from PRACH Configuration Index.
 *             [Ref: 36.211, Table 5.7.1-2]
 *          -# PrachFreqOffset is the first physical resource block allocated to 
 *             the PRACH opportunity considered for preamble format 0, 1, 2 and 3.
 *             [Ref: 36.211, 5.7.1]
*/
typedef struct ctfPrachCfgInfo {
   Bool  pres;                   /*!< Indicates the presence of this info */
   uint16_t   rootSequenceIndex;      /*!< Range (0..837) */
   uint8_t    prachCfgIndex;          /*!< Prach Config Index (0..63) */
   uint8_t    zeroCorrelationZoneCfg; /*!< Used for preamble sequence generation
                                      (36.211, 5.7.2); FDD:0..15, TDD:0..6 */
   Bool  highSpeedFlag;          /*!< TRUE: Restricted set, 
                                      FALSE: Unrestricted Set */
   uint8_t    prachFreqOffset;        /*!< Range(0..94) */
} CtfPrachCfgInfo;

/**
 * @brief   PDSCH configuration.
 * @details This structure contains the PDSCH configuration information for 
 *          configuration at PHY.
*/
typedef struct ctfPdschCfgInfo {
   Bool           pres;       /*!< Indicates the presence of this info */
   S16             refSigPwr;  /*!< Provides downlink reference signal EPRE, 
                                   in (-60..50)dBm */
   uint8_t             p_b;        /*!< Range(0..3) [36.213, 5.2] */ 
} CtfPdschCfgInfo;

/**
 * @brief   Basic PUSCH configuration.
 * @details This structure contains the basic PUSCH configuration information for 
 *          configuration at PHY.
*/
typedef struct ctfPuschCfgBasic {
   uint8_t                   noOfsubBands;  /*!< Number of sub-bands, (1..4) */
   CtfPuschHoppingMode  hoppingMode;   /*!< Hopping Mode */
   uint8_t                   hoppingOffset; /*!< Hopping Offset(0..98) */
   Bool                 enable64QAM;   /*!< 64 QAM,TRUE: enabled, 
                                                   FALSE: disabled */
} CtfPuschCfgBasic;

/**
 * @brief   PUSCH Uplink Reference Signals configuration.
 * @details This structure contains the PUSCH configuration information for 
 *          uplink reference signals configuration at PHY.
*/
typedef struct ctfPuschUlRS {
   Bool  grpHopEnabled; /*!< Group hopping, TRUE: enabled, FALSE: disabled */
   Bool  seqHopEnabled; /*!< Sequence hopping, TRUE: enabled, FALSE: disabled */
   uint8_t    grpNum;        /*!< Group number (0..29) */
   uint8_t    cycShift;      /*!< Cyclic shift(0..7) */
} CtfPuschUlRS;

/**
 * @brief   PUSCH configuration.
 * @details This structure contains the information for PUSCH 
 *          configuration at PHY.
*/
typedef struct ctfPuschCfgInfo {
   Bool              pres;          /*!< Indicates the presence of PUSCH configuration info */
   CtfPuschCfgBasic  puschBasicCfg; /*!< PUSCH basic configuration */
   CtfPuschUlRS      puschUlRS;     /*!< PUSCH uplink RS */
} CtfPuschCfgInfo;

/**
 * @brief   PHICH configuration.
 * @details This structure contains the duration and resource information for
 *          PHICH configuration at PHY.
*/
typedef struct ctfPhichCfgInfo {
   Bool              pres;       /*!< Indicates the presence of PHICH 
                                      configuration info */
   CtfPhichDuration  duration;   /*!< PHICH duration, normal/extended, 
                                      [36.211, 6.9.3-1] */
   CtfPhichResource  resource;   /*!< PHICH Resource, Ng [36.211, 6.9] */
} CtfPhichCfgInfo;

/**
 * @brief   PUCCH configuration.
 * @details This structure contains the information for PUCCH
 *          configuration at PHY.
*/
typedef struct ctfPucchCfgInfo {
   Bool  pres; /*!< Indicates the presence of PUCCH configuration info */
   uint8_t    nRB;  /*!< Number of RBs that are available for use by 
                    PUCCH formats (0..98) */
   uint8_t    nCS;  /*!< Number of cyclic shifts used for PUCCH formats (0..7) */
   uint16_t   n1PUCCH; /*!< PUCCH resource for transmission of ACK/NACK (0..2047) */
   CtfPucchDeltaShift   deltaShift; /*!< PUCCH delta shift */
} CtfPucchCfgInfo;

/**
 * @brief   SRS uplink configuration.
 * @details This structure contains the information for setting-up/release
 *          of uplink SRS configuration at PHY.
*/
typedef struct ctfSrsUlCfgInfo {
   Bool              pres;       /*!< Indicates the presence of UL SRS info */
   uint8_t                srsCfgType; /*!< Setup/Release: The setup structure
                                      is valid ,only if srcCfgType is setup. */
   struct srsSetupS
   {
      uint8_t                srsBw;   /*!< SRS bandwidth config (0..7) */
      uint8_t                sfCfg;   /*!< SRS sub-frame config (0..15) */
      Bool              srsANSimultTx; /*!< Simultaneous transmisson 
                                            of SRS and ACK/NACK */
      Bool              srsMaxUpPts;   /*!< SRS MaxUpPTS: TRUE/FALSE, 
                                            This field is valid only for TDD */
   } srsSetup;
} CtfSrsUlCfgInfo;

/**
 * @brief   Subframe Configuration for TDD mode.
 * @details This structure contains the information for setting-up
 *          the subframe for TDD mode at PHY.
*/
typedef struct ctfTddSfCfgInfo {
   Bool  pres;             /*!< Indicates if this info is valid */
   uint8_t    sfAssignment;     /*!< UL-DL subframe assignment : (0..6)
                                36.211 Table 4.2-2 */
   uint8_t    spclSfPatterns;   /*!< Special subframe configuration : (0..8)
                                36.211 Table 4.2-1 */
} CtfTddSfCfgInfo;

/* ctf_x_001.main_4: Added support for vendor specific parameters */
#ifdef CTF_VER3
/**
 * @brief   Vendor Specific Parameter
 * @details This structure contains the Parameters which is spaciftc to 
 *     Vendor.
 */

typedef struct ctfVendorSpecific{
  uint16_t  buffLen ;  /*!<  Length of parameters in bytes */
  uint8_t  *paramBuffer ; /*!< parameter Buffer */
} CtfVendorSpecific;
#endif
/* Cell specific configuration parameters */

/**
 * @brief   Unlicensed Cell Configuration Information.
 * @details This structure contains the information for setting-up
 *          of an unlicensed SDL cell and its associated resources at PHY.
*/
typedef struct ctfUnLicnsdCellCfg {
   uint8_t            coExistMethod; /*!< Continuous/LTE-U/LAA */
   uint8_t            lteOnPeriod; /*!< Percentage of time SDL
                                   is active */
   uint8_t            ccaMethod;   /*!< Clear channel assesment method */
   Bool          adaptiveTx;  /*!< Indicates if the duty cycle is 
                                   adaptive or fixed */
   uint16_t           transPeriod; /*!< Defines the duty cycle */
   uint16_t           listenPrd;   /*!< Time duration in us for clear channel 
                                   assessment */
   S16           energyTh;    /*!< Energy threshold in dBFS */
   uint16_t           scanTimePrd; /*!< Time period for RF channel scan */
   uint16_t           activityTh;  /*!< Percentage of time for activity detection
                                   to select different EARFCN */
   S32           txPowerLimit; /*! Reference Signal power for Unlicensed Cell */
   uint8_t            numFreq;     /*!< Number of unlicensed channels */
   uint32_t           earfcn[CTF_LTEU_MAX_EARFCN]; /*!< Unlicensed channels */
} CtfUnLicnsdCfgInfo;

typedef struct ctfLaaScanInd {
   S32 status;
   uint32_t nEarfcn;
   uint32_t earfcn[CTF_LTEU_MAX_EARFCN];
} CtfLaaScanInd;

#ifdef EMTC_ENABLE
typedef struct ctfEmtcCElevelInfoLst {
   uint8_t   firstPreamble;       /*!< for each CE mode, starting preamble */
   uint8_t   lastPreamble;        /*!< for each CE mode, starting preamble */
   uint8_t   emtcNumRepPerPreambleAtt;/*!< num of repetition per preamble attempt*/
}CtfEmtcCElevelInfoLst;
#endif
 
/**
 * @brief   Cell Configuration Information.
 * @details This structure contains the information for setting-up
 *          of a cell and its associated resources at PHY.
*/
typedef struct ctfCellCfgInfo {
   CmLteCellId       cellId;     /*!< Cell ID */
   uint8_t                cellIdGrpId;/*!< Cell Identity Group ID (0..167) */
   SpId          macSapId;   /*!< MAC SAP Id for Tfu Interface*/
   SpId          schSapId;   /*!< Scheduler SAP Id for Tfu Interface */
   /*ctf_x_001.main_3: Adding the remaining parameters */
   Bool              physCellIdPres ;
   uint8_t                physCellId; /*!< Cell ID (0..2) */
   CtfBwCfgInfo      bwCfg;      /*!< Bandwidth configuration */
   CtfTxSchemeCfg    txCfg;      /*!< Basic transmission scheme 
                                      configuration [36.300, 5.1.1] */
   CtfAntennaCfgInfo antennaCfg; /*!< Antenna configuration */
   CtfPrachCfgInfo   prachCfg;   /*!< PRACH configuration */
   CtfPdschCfgInfo   pdschCfg;   /*!< PDSCH configuration */
   CtfPuschCfgInfo   puschCfg;   /*!< PUSCH configuration */
   CtfPhichCfgInfo   phichCfg;   /*!< PHICH configuration */
   CtfPucchCfgInfo   pucchCfg;   /*!< PUCCH configuration */
   CtfSrsUlCfgInfo   srsUlCfg;   /*!< SRS UL configuration, setup case */
   CtfTddSfCfgInfo   tddSfCfg;   /*!< SubFrame configuration for TDD only */
   
   /*ctf_x_001.main_3: Adding the remaining parameters */
   uint16_t               syncSigPowOs;              /*!< Synchronization signal power offset */
   Bool              syncSigPowOsPres;
   uint16_t               cfiPowOs;                  /*!< Cfi Power Offset */
   Bool              cfiPowOsPres;
   uint16_t               dciPowOs;                  /*!< Dci Power Offser */
   Bool              dciPowOsPres;
   uint16_t               extWinMargin;              /*!< Extraction Window Marin */
   Bool              extWinMarginPres;
   uint16_t               pucchNoiseGamma;           /*!< PUCCH Noise Estimation Gamma*/
   Bool              pucchNoiseGammaPres;
   uint16_t               prachPkRatio4;             /*!< PRACH Format4 Peak Ratio */
   Bool              prachPkRatio4Pres;
   uint16_t               prachPkRatio0;             /*!< PRACH Format0 Peak Ratio */
   Bool              prachPkRatio0Pres;
   uint16_t               srsDopEstFactor;           /*!< Doppler Estimation Compensation Power */
   Bool              srsDopEstFactorPres;
   uint16_t               puschProbDtxAck;           /*!< Portability Dtx Ack PUSCH */
   Bool              puschProbDtxAckPres;
   uint16_t               pucchProbDtxAck;           /*!< Portability Dtx Ack PUCCH Format1 */
   Bool              pucchProbDtxAckPres;

   uint16_t               txAntennaPorts ;           /*!< The number of cell specific transmit antenna ports */
   Bool              txAntennaPortsPres;
   uint16_t               rxAntennaPorts ;           /*!< The number of cell specific receive antenna ports */
   Bool              rxAntennaPortsPres ;

   uint16_t               phySyncMode;               /*!< The method used by the PHY to signal the 1ms subframe */
   Bool              phySyncModePres;
   uint16_t               dataRepMode;               /*!< The data report mode for the uplink data */
   Bool              dataRepModePres ;
   uint16_t               rachSrRepMode;             /*!< The reporting mode for RACH.indication and SR.indcation */
   Bool              rachSrRepModePres;
   /* PHY configuration parameters */
   uint16_t           opMode;
   uint32_t           counter;
   uint32_t           period;
   S16           priSigPwr;
   S16           secSigPwr;
   uint16_t           dlEarfcn;
   uint16_t           ulEarfcn;  
/*Start Fix for CR ccpu00123185 */ 
   S16           pilotSigPwr;
/*End Fix for CR ccpu00123185 */
   /* Added parameter for rgu dynamic delta*/
   uint8_t            numDlUePerTti;
   Bool          unLicnsdCfgPres; /*!< Flag to indicate unlicensed cell config */ 
   CtfUnLicnsdCfgInfo lteUCfg; /*!< Unlicensed carrier config */
#ifdef EMTC_ENABLE
	uint8_t  				catMenabled;
   uint8_t             mPdcchStart;
   CtfEmtcCElevelInfoLst ceLevelInfo[CTF_MAX_CE_LEVEL];
#endif
} CtfCellCfgInfo;

/**
 * @enum CtfCellStatus 
 * This element enumerates the Cell Status for configuration at PHY.
*/
typedef enum {
   CTF_CELL_DOWN,    /*!< Cell DOWN selection */
   CTF_CELL_UP       /*!< Cell UP selection */
} CtfCellStatus;

/**
 * @brief   Cell Re-configuration Information.
 * @details This structure contains the information for reconfiguration
 *          of a cell and its associated resources at PHY.
*/
typedef struct ctfCellRecfgInfo {
   CtfReCfgType      ctfReCfgType ;
   CmLteCellId       cellId;     /*!< Cell ID */
   /*ctf_x_001.main_3: Adding the remaining parameters */
   uint8_t                cellIdGrpId;/*!< Cell Identity Group ID (0..167) */
   Bool              physCellIdPres ;
   uint8_t                physCellId; /*!< Cell ID (0..2) */
   CtfBwCfgInfo      bwCfg;      /*!< Bandwidth configuration */
   CtfTxSchemeCfg    txCfg;      /*!< Basic transmission scheme 
                                      configuration [36.300, 5.1.1] */
   CtfAntennaCfgInfo antennaCfg; /*!< Antenna configuration */
   CmLteTimingInfo   actvnTime;  /*!< Activation time, only SFN is valid */
   CtfPrachCfgInfo   prachCfg;   /*!< PRACH configuration */
   CtfPdschCfgInfo   pdschCfg;   /*!< PDSCH configuration */
   CtfPuschCfgInfo   puschCfg;   /*!< PUSCH configuration */
   CtfPhichCfgInfo   phichCfg;   /*!< PHICH configuration */
   CtfPucchCfgInfo   pucchCfg;   /*!< PUCCH configuration */
   CtfSrsUlCfgInfo   srsUlCfg;   /*!< SRS UL configuration, setup case */
   CtfTddSfCfgInfo   tddSfCfg;   /*!< SubFrame configuration for TDD only */
   /*ctf_x_001.main_3: Adding the remaining parameters */
   uint16_t               syncSigPowOs;              /*!< Synchronization signal power offset */
   Bool              syncSigPowOsPres;
   uint16_t               cfiPowOs;                  /*!< Cfi Power Offset */
   Bool              cfiPowOsPres;
   uint16_t               dciPowOs;                  /*!< Dci Power Offser */
   Bool              dciPowOsPres;
   uint16_t               extWinMargin;              /*!< Extraction Window Marin */
   Bool              extWinMarginPres;
   uint16_t               pucchNoiseGamma;           /*!< PUCCH Noise Estimation Gamma*/
   Bool              pucchNoiseGammaPres;
   uint16_t               prachPkRatio4;             /*!< PRACH Format4 Peak Ratio */
   Bool              prachPkRatio4Pres;
   uint16_t               prachPkRatio0;             /*!< PRACH Format0 Peak Ratio */
   Bool              prachPkRatio0Pres;
   uint16_t               srsDopEstFactor;           /*!< Doppler Estimation Compensation Power */
   Bool              srsDopEstFactorPres;
   uint16_t               puschProbDtxAck;           /*!< Portability Dtx Ack PUSCH */
   Bool              puschProbDtxAckPres;
   uint16_t               pucchProbDtxAck;           /*!< Portability Dtx Ack PUCCH Format1 */
   Bool              pucchProbDtxAckPres;

   uint16_t               txAntennaPorts ;           /*!< The number of cell specific transmit antenna ports */
   Bool              txAntennaPortsPres;
   uint16_t               rxAntennaPorts ;           /*!< The number of cell specific receive antenna ports */
   Bool              rxAntennaPortsPres ;

   uint16_t               phySyncMode;               /*!< The method used by the PHY to signal the 1ms subframe */
   Bool              phySyncModePres;
   uint16_t               dataRepMode;               /*!< The data report mode for the uplink data */
   Bool              dataRepModePres ;
   uint16_t               rachSrRepMode;             /*!< The reporting mode for RACH.indication and SR.indcation */
   Bool              rachSrRepModePres;
/*Start Fix for CR ccpu00123185 */ 
   S16               pilotSigPwr;
   S16               priSigPwr;
   S16               secSigPwr;
/*End Fix for CR ccpu00123185 */ 
   CtfCellStatus     ctfCellStatus; 
#ifdef ENABLE_API_LOG
   uint8_t                enableAPILog;
#endif /* ENABLE_API_LOG */
} CtfCellRecfgInfo;

/* UE-specific/Dedicated config parameters */

/**
 * @brief   Dedicated PDSCH Configuration.
 * @details This structure contains the information for PDSCH configuration
 *          specific to an UE.
*/
typedef struct ctfDedPdschCfgInfo {
   Bool              pres; /*!< Indicates the presence of PDSCH configuration info */
   CtfPdschPaParam   pA;   /*!< Pa (in dB) */
} CtfDedPdschCfgInfo;

/**
 * @brief   Dedicated PUCCH Configuration.
 * @details This structure contains the information for setting-up/release 
 *          PUCCH configuration specific to an UE.
*/
typedef struct ctfDedPucchCfgInfo {
   Bool              pres;             /*!< Indicates the presence of PUCCH info */
   uint8_t                dedPucchCfgType;  /*!< Setup/Release */
   struct pucchSetupS
   {
#ifdef CTF_VER3
      /* ctf_x_001.main_4:115549: */
      Bool              repFactPres;   /*!< Indicates if repetition factor
                                             is configured or not */
#endif /* CTF_VER3 */
      CtfPucchRepFactor repFact;       /*!< ACK/NACK repetition factor */
      uint16_t               n1PUCCHRep;    /*!< PUCCH resource to repeat the 
                                            transmission of the corresponding 
                                            ACK/NACK responce [36.213, 10.1] */
   } pucchSetup;
#ifdef CTF_VER3
   /* ctf_x_001.main_4:115549: */
    Bool                 tddAckNackCfgPres; /*! Present flag for TDD 
                                                ACK/NACK config */
    CtfTddAckNackFBMode  tddAckNackFBMode; /*!< TDD ACK/NACK Feedback Mode 
                                                 (bundling, multiplexing) */
#endif /* CTF_VER3 */
} CtfDedPucchCfgInfo;

/**
 * @brief   Dedicated PUSCH Configuration.
 * @details This structure contains the information for PUCCH 
 *          configuration specific to an UE.
*/
typedef struct ctfDedPuschCfgInfo {
   Bool     pres;             /*!< Indicates the presence of PUSCH info */
   uint8_t       betaOffsetAckIdx; /*!< Beta offset ACK index, (0..15)
                                   [36.213, Table 8.6.3.-1] */
   uint8_t       betaOffsetRiIdx;  /*!< Beta offset RI index, (0..15)
                                   [36.213, Table 8.6.3.-2] */
   uint8_t       betaOffsetCqiIdx; /*!< Beta offset CQI index, (0..15)
                                   [36.213, Table 8.6.3.-3] */
} CtfDedPuschCfgInfo;


/* ctf_x_001.main_2: Added pmiRiRpt and cqiMask*/
#ifdef CTF_VER1
/**
 * @enum ctfPmiRiRptSetup
 * This element enumerates the CQI PMI Report setup configuration at PHY.
*/
typedef enum ctfPmiRiRptSetup
{
   CTF_CQI_PMI_RPT_SETUP  /*!< PMI Report setup */
} CtfPmiRiRptSetup;

/**
 * @struct ctfPmiRiRpt
 * This struct contains the  CQI PMI Report setup configuration at PHY.
*/
typedef struct ctfPmiRiRpt
{
   Bool              pres;           /*!< Indicates presence of Pmi Ri Rpt */
   CtfPmiRiRptSetup  pmiRiRptSetup;  /*!< PMI Report setup */
} CtfPmiRiRpt;

/**
 * @enum ctfCqiMaskSetup
 * This element enumerates the CQI reporting setup configuration at PHY.
*/
typedef enum ctfCqiMaskSetup
{
   CTF_CQI_RPT_SETUP  /*!< Cqi report setup */
} CtfCqiMaskSetup;

/**
 * @struct ctfCqiMask
 * This struct contains the CQI reporting setup configuration at PHY.
*/
typedef struct ctfCqiMask
{
   Bool             pres;     /*!< Presence field */
   CtfCqiMaskSetup  cqiMaskSetup;  /*!< Cqi Report Setup */
} CtfCqiMask;
#endif /* CTF_VER1 */

#ifndef TFU_UPGRADE
/* ctf_x_001.main_2: Moved aPeriodic enum to seperate structure*/
#ifdef CTF_VER2
/* Moved aPeriodic enum to seperate structure */
/**
 * @brief   APeriodic CQI Report Configuration.
 * @details This structure contains the information for Aperiodic CQI report
 *          related configuration specific to an UE.
*/
typedef struct ctfCqiReportModeAPeriodic {
    Bool pres;
    CtfCqiRptModeAperiodic aPeriodicRptMode; /*!<Aperiodic CQI report*/
}CtfCqiRptModeAprd;
#endif /* CTF_VER2 */

/**
 * @brief   Periodic CQI Report Configuration.
 * @details This structure contains the information for periodic CQI report
 *          related configuration specific to an UE.
*/
typedef struct ctfCqiRptModePeriodic {
#ifdef CTF_VER2
   Bool  pres;
#endif /* CTF_VER2 */
   uint8_t    cqiPeriodicCfgType;     /*!< Configuration type: Setup/Release  */
   struct cqiSetupS
   {
      uint16_t   cqiPUCCHRsrcIndx;    /*!< Range (0..1185) */
      uint16_t   cqiPmiCfgIndx;       /*!< Range (0..1023) */
      uint8_t    formatIndicator;     /*!< Type: Wideband/Subband */
      struct subbandCqiS
      {
         uint8_t    k;                /*!< Range (1..4) */
      } subbandCqi;              /*!< Valid only if formatIndicator is Subband */

      Bool  riCfgPres;           /*!< RI Configuration index is present or absent */
      uint16_t   riCfgIndx;           /*!< Range (0..1023) */
      Bool  simultaneousAckNack; /*!< Simultaneous ACK/NACK with CQI support
                                      TRUE: YES, FALSE: NO */
/* ctf_x_001.main_2: Added cqiMask*/
#ifdef CTF_VER1
   CtfCqiMask   cqiMask;         /*!< cqi mask */
#endif /* CTF_VER1 */
   } cqiSetup;                   /*!< Valid only if cqiPeriodicCfgType is Setup */
} CtfCqiRptModePeriodic;

/**
 * @brief   CQI Report Configuration.
 * @details This structure contains the information for CQI report
 *          related configuration specific to an UE.
*/
typedef struct ctfCqiReportCfgInfo {
   Bool  pres;                /*!< Indicates the presence of this info  */
/* ctf_x_001.main_2: period and aperiodic rpt both can be configured simultaneously*/
#ifdef CTF_VER2
   struct
   {
      CtfCqiRptModeAprd       aPeriodicRpt;
      CtfCqiRptModePeriodic   periodicRpt;
   } reportMode;
#else /* CTF_VER2 */
   uint8_t    reportingMode;       /*!< Reporting mode: Periodic/Aperiodic */
   union
   {
      CtfCqiRptModeAperiodic  aPeriodicRpt;
      CtfCqiRptModePeriodic   periodicRpt;
   } reportMode;
#endif /* CTF_VER2 */
/* ctf_x_001.main_2: Added PmiRiRpt*/
#ifdef CTF_VER1
   CtfPmiRiRpt  pmiRiRpt;        /*!< Pmi Ri Report */
#endif /* CTF_VER1 */
} CtfCqiReportCfgInfo;

/**
 * @brief   Dedicated Uplink SRS Configuration.
 * @details This structure contains the information for setting-up/release
 *          UL SRS configuration specific to an UE.
*/
typedef struct ctfDedSrsUlCfgInfo {
   Bool     pres;             /*!< Indicates the presence of SRS UL info */
   uint8_t       dedSrsUlCfgType;  /*!< Setup/Release */
   struct dedSrsSetupS
   {
      uint8_t       srsBw;         /*!< SRS bandwidth(0,1,2,3) */
      uint8_t       srsHopngBw;    /*!< SRS hopping bandwidth (0,1,2,3) */
      uint8_t       freqDmnPos;    /*!< Frequency domain position (0..23) */
      Bool     duration;      /*!< Duration, FALSE: single, 
                                             TRUE: indefinite */
      uint16_t      srsCfgIdx;     /*!< SRS config index (0..1023) */
      uint8_t       txComb;        /*!< transmission comb (0..1) */
      uint8_t       cyclicShift;   /*!< Cyclic shift (0..7) */
   } dedSrsSetup;
} CtfDedSrsUlCfgInfo;

/**
 * @brief   Dedicated SR Configuration
 * @details This structure contains the information for setting-up/release
 *          SR configuration specific to an UE at PHY.
*/
typedef struct ctfDedSRCfgInfo {
   Bool     pres;          /*!< Indicates the presence of SR info */
   uint8_t       dedSRCfgType;  /*!< Setup/Release */
   struct dedSrSetupS
   {
      uint16_t      srPUCCHRi;  /*!< SR PUCCH Resource Index, n(1)PUCCH,SRI 
                                (0..2047) */
      uint8_t       srCfgIdx;   /*!< SR config index, Isr (0..155) */
   } dedSrSetup;           /*!< Valid only if dedSRCfgType is setup */
} CtfDedSRCfgInfo;
#endif /* TFU_UPGRADE */

#ifdef CTF_VER2
/* ctf_x_001.main_2 : Added enum for Code Book Subset Restriction */
/**
 * @enum CtfCdBkSubsetRestnType
 * This element enumerates the Code Book Subset Restriction 
 * configuration at PHY for a UE.
*/
typedef enum ctfCdBkSubsetRestnType
{
   N2TX_ANT_TM3,     /*!< 2 Antenna Ports for transmissiom mode 3*/
   N4TX_ANT_TM3,     /*!< 4 Antenna Ports for transmissiom mode 3*/
   N2TX_ANT_TM4,     /*!< 2 Antenna Ports for transmissiom mode 4*/
   N4TX_ANT_TM4,     /*!< 4 Antenna Ports for transmissiom mode 4*/
   N2TX_ANT_TM5,     /*!< 2 Antenna Ports for transmissiom mode 5*/
   N4TX_ANT_TM5,     /*!< 4 Antenna Ports for transmissiom mode 5*/
   N2TX_ANT_TM6,     /*!< 2 Antenna Ports for transmissiom mode 6*/
   N4TX_ANT_TM6      /*!< 4 Antenna Ports for transmissiom mode 6*/
}CtfCdBkSubsetRestnType;

/* nh005.201_LPR2 : Added new structure for Code Book Subset Restriction */
/**
 * @struct  CtfDedAntCodeBkSubsetRes
 * @brief Code Book Subset Restriction Configuration.
 * @details This structure contains the information for 
 *          codeBookSubsetRestriction  configuration specific to 
 *          transmission mode of a UE Phy.
*/
typedef struct ctfDedAntCodeBookSubsetRestn
{
  CtfCdBkSubsetRestnType ctfCdBkRestType;
  union
  {
      uint8_t   n2TxAntTm3;     /*!< 2 Antenna Ports for transmissiom mode 3*/
      uint8_t   n4TxAntTm3;     /*!< 4 Antenna Ports for transmissiom mode 3*/
      uint8_t   n2TxAntTm4;     /*!< 2 Antenna Ports for transmissiom mode 4*/
      uint8_t   n4TxAntTm4[8];  /*!< 4 Antenna Ports for transmissiom mode 4*/
      uint8_t   n2TxAntTm5;     /*!< 2 Antenna Ports for transmissiom mode 5*/
      uint16_t  n4TxAntTm5;     /*!< 4 Antenna Ports for transmissiom mode 5*/
      uint8_t   n2TxAntTm6;     /*!< 2 Antenna Ports for transmissiom mode 6*/
      uint16_t  n4TxAntTm6;     /*!< 4 Antenna Ports for transmissiom mode 6*/
  }codeBookSubsetRestn;
}CtfDedAntCodeBkSubsetRes;
#endif /* CTF_VER2 */

/**
 * @brief   Dedicated Antenna Configuration.
 * @details This structure contains the information for antenna related
 *          configuration specific to an UE at PHY.
*/
typedef struct ctfDedAntCfgInfo {
   Bool              pres;   /*!< Indicates the presence of antenna configuration info */
   CtfUeTxAntMode    txMode; /*!< Transmission Mode: 36.213, 7.1 */
   struct ueTxAntSelectionS
   {
      uint8_t                      cfgType;       /*!< Setup/Release */
      CtfUeTxAntSelectType    txAntSelect;   /*!< Closed/Open, valid only if
                                                  cfgType is setup:36.213, 8.7 */
   } ueTxAntSelection;
#ifdef CTF_VER2
   /*ctf_x_001.main_2 : Added CodeBookSubsetRestriction Parameter */
   CtfDedAntCodeBkSubsetRes codeBookSubsetRes;
#endif /* CTF_VER2 */
} CtfDedAntCfgInfo;

/**
 * @brief   UE Category Configuration.
 * @details This structure contains the information for configuring
 *          the UE category at PHY.
*/
typedef struct ctfUeCatCfgInfo {
   Bool                 pres;       /*!< TRUE - indicates UE category is present */
   CmLteUeCategory      ueCategory; /*!< UE category */
} CtfUeCatCfgInfo;

#ifdef TENB_AS_SECURITY
typedef struct ctfHdr{
   CmLteCellId  cellId;
   CmLteRnti   ueId;
}CtfHdr;

/**
 * @brief   KeNB Information Configuration.
 * @details This structure contains the input information for
 *          deriving the AS security keys using HMAC-SHA256 KDF.
*/
typedef struct ctfKenbCfgInfo{
           uint8_t pres;
           uint8_t algoType;
           uint8_t secKey[CTF_SEC_KEY_LEN];
}CtfKenbCfgInfo;

#endif

/**
 * @brief   Dedicated Configuration Information.
 * @details This structure contains the information for configuration 
 *          of UE-specific physical channel resources at PHY.
*/
typedef struct ctfDedCfgInfo {
   CmLteCellId          cellId;     /*!< Cell ID */
   CmLteRnti            ueId;       /*!< UE ID */
   CtfUeCatCfgInfo      ueCatCfg;   /*!< UE Cateogry configuration */
   CtfDedPdschCfgInfo   pdschCfg;   /*!< PDSCH configuration */
   CtfDedPucchCfgInfo   pucchCfg;   /*!< PUCCH configuration */
   CtfDedPuschCfgInfo   puschCfg;   /*!< PUSCH configuration */
#ifndef TFU_UPGRADE
   CtfCqiReportCfgInfo  cqiRptCfg;  /*!< CQI report configuration */
   CtfDedSrsUlCfgInfo   srsUlCfg;   /*!< SRS UL configuration */
   CtfDedSRCfgInfo      dedSRCfg;   /*!< SR configuration */
#endif /* TFU_UPGRADE */
   CtfDedAntCfgInfo     antInfo;    /*!< Antenna configuration */
#ifdef EMTC_ENABLE
   uint8_t                   isEmtcUe;   /*!< UE type is EMTC or not */
#endif
} CtfDedCfgInfo;

/** @brief Each Secondary cell configuration Info for a UE*/
typedef struct ctfUeSecCellCfgInfo
{
  uint8_t                   sCellIdx; /*!< This will be secondary cell Idx */
  uint16_t                  sCellId; /*!< This will be secondary cellId */
}CtfUeSecCellCfgInfo;

/** @brief Secondary cell action for a UE*/
typedef enum ctfScellAction                                           
{
   CTF_SCELL_ADD,
   CTF_SCELL_MOD_COMMON,
   CTF_SCELL_MOD_DEDICATED,
   CTF_SCELL_RELEASE
}CtfScellAction;

/** @brief Secondary Cell Configuration for a UE*/
typedef struct ctfSecCellCfgInfo
{
  uint8_t                    numSCells;
  CtfScellAction        sCellAction;
  CtfUeSecCellCfgInfo   ueSCellDedCfg[CTF_MAX_SCELL_PER_UE];
}CtfSecCellCfgInfo;

/**
 * @brief   Dedicated Re-configuration Information.
 * @details This structure contains the information for re-configuration 
 *          of UE-specific physical channel resources at PHY.
*/
typedef struct ctfDedRecfgInfo {
   CmLteCellId          cellId;     /*!< Cell ID */
   CmLteRnti            ueId;       /*!< Old UE ID */
   /* ctf_x_001.main_4: ReEst Fix */
#ifdef CTF_VER3
   CmLteRnti            newUeId;       /*!< If ueId is not getting reconfigured,
                                            both ueId and newUeId shell hold same
                                            value. If ueId is getting reconfigured 
                                            then these two value should be different
                                            from each other */
  #else
     CmLteRnti            newUeId;       /*!< If ueId is not getting reconfigured,
                                            both ueId and newUeId shell hold same
                                            value. If ueId is getting reconfigured 
                                            then these two value should be different
                                            from each other */

											
#endif

   CtfUeCatCfgInfo      ueCatCfg;   /*!< UE Cateogry configuration */
   CtfDedPdschCfgInfo   pdschCfg;   /*!< PDSCH configuration */
   CtfDedPucchCfgInfo   pucchCfg;   /*!< PUCCH configuration */
   CtfDedPuschCfgInfo   puschCfg;   /*!< PUSCH configuration */
#ifndef TFU_UPGRADE
   CtfDedSrsUlCfgInfo   srsUlCfg;   /*!< SRS UL configuration */
   CtfDedSRCfgInfo      dedSRCfg;   /*!< SR configuration */
/*ctf_x_001.main_1*/ 
   CtfCqiReportCfgInfo  cqiRptCfg;  /*!< CQI report configuration */
#endif /* TFU_UPGRADE */
   CtfDedAntCfgInfo     antInfo;    /*!< Antenna configuration */
/*Amit */
   CtfSecCellCfgInfo    sCellInfo;
} CtfDedRecfgInfo;

/**
 * @brief   Cell Release Information.
 * @details This structure contains the information for release/removal 
 *          of a cell and its assocated resources at PHY.
*/
typedef struct ctfCellReleaseInfo {
   CmLteCellId cellId; /*!< Cell ID */
} CtfCellReleaseInfo;

/**
 * @brief   Dedicated Release Information
 * @details This structure contains the information for release/removal 
 *          of dedicated/UE-specific configuration at PHY.
*/
typedef struct ctfDedReleaseInfo {
   CmLteCellId cellId;  /*!< Cell ID */
   CmLteRnti   ueId;    /*!< UE ID (rnti) */
   CtfSecCellCfgInfo    sCellInfo;
} CtfDedReleaseInfo;

/**
 * @brief   Configuration Information.
 * @details This structure contains the information for setting-up 
 *          cell or UE-specific configuration at PHY.
*/
typedef struct ctfCfgInfo {
   uint8_t cfgElem; /*!< Configuration element : Cell/UE */
   union
   {
      CtfCellCfgInfo cellCfg; /*!< Cell configuration */
      CtfDedCfgInfo  dedCfg;  /*!< UE configuration */
   } u;
} CtfCfgInfo;

/**
 * @brief   Re-configuration Information.
 * @details This structure contains the information for re-configuring 
 *          cell or UE-specific configuration at PHY.
*/
typedef struct ctfReCfgInfo {
   uint8_t cfgElem; /*!< Configuration element: Cell/UE */
   union
   {
      CtfCellRecfgInfo  cellRecfg;  /*!< Cell reconfiguration */
      CtfDedRecfgInfo   dedRecfg;   /*!< UE reconfiguration */
   } u;
} CtfReCfgInfo;

/**
 * @brief   Release Configuration Information.
 * @details This structure contains the information for releasing 
 *          cell or UE-specific configuration at PHY.
*/
typedef struct ctfReleaseInfo {
   uint8_t cfgElem; /*!< Configuration element : Cell/UE */
   union
   {
      CtfCellReleaseInfo  cellRel;  /*!< Cell release */
      CtfDedReleaseInfo   dedRel;   /*!< UE release */
   } u;
} CtfReleaseInfo;



/**
 * @brief   Configuration Request Information.
 * @details This is the main structure in the configuration request primitive.
 *    -# This structure contains the configuration information as given by 
 *       the User to configure PHY layer for common/dedicated resources.
 *    -# It can contain Config/Reconfig/Release.
 *    -# The result of configuration is indicated in TfUiCtfCfgCfm. 
*/
typedef struct ctfCfgReqInfo {
   uint8_t    cfgType; /*!< Configuration type: Config/Reconfig/Release */
   union
   {
      CtfCfgInfo     cfg;     /*!< Cell/UE Configuration information */
      CtfReCfgInfo   reCfg;   /*!< Cell/UE Reconfiguration information */
      CtfReleaseInfo release; /*!< Cell/UE Release information */
   } u;
/* ctf_x_001.main_4: Added support for vendor specific parameters */
#ifdef CTF_VER3
   CtfVendorSpecific  vendorParams ; /*!< Vendor Specific Parameter */ 
#endif   
} CtfCfgReqInfo;

#ifdef TENB_AS_SECURITY
/********************************************************************************************************************************/
                              /*START OF KEY DERIVATION FUNTION (KDF) REQUEST STRUCTURES*/
/********************************************************************************************************************************/

/*AS key generation input information structure*/
typedef struct ctfAsKeyInfo{
           uint8_t intgAlgoType;                /*!SNOW3g or AES used in key derivation as well */
           uint8_t ciphAlgoType;                /*!SNOW3g or AES used in key derivation as well */
           uint8_t secKey[CTF_SEC_KEY_LEN]; /*!< eNodeB key received in initial context setup */
}CtfAsKeyInfo;

/*Horizontal keNB derivation input information structure*/
typedef struct ctfKenbStarInfo{
           uint16_t dlCarrFreq;             /*!< DL UARFCN*/ 
           uint16_t physCellId;             /*!< Physical cell ID*/
           uint8_t secKey[CTF_SEC_KEY_LEN]; /*!< Current eNodeB key at App used to derive keNB*. Horizontal KD */
}CtfKenbStarInfo;

/*Vertical keNB derivation input information structure*/
typedef struct ctfNhKeyInfo{
           uint16_t dlCarrFreq;             /*!< DL UARFCN*/ 
           uint16_t physCellId;             /*!< Physical cell ID*/
           uint8_t secKey[CTF_SEC_KEY_LEN]; /*!< NH key provided by MME used to derive keNB*. Vertical KD */
}CtfNhKeyInfo;

typedef struct ctfKdfReqInfo {
   uint8_t    keyDerType; /*!< Keyderivation type type: for AS security keys/keNB* from keNB/keNB* from NH key */
   union
   {
      CtfAsKeyInfo      kEnbInf;       /*!< eNodeB key for for deriving AS security keys */
      CtfKenbStarInfo   kEnbStarInf;   /*!< eNodeB key for for deriving eNodeB* Key (Horizontal key derivation ref: ts 33.401) */
      CtfNhKeyInfo      nhKeyInf;      /*!< NH key for for deriving eNodeB* Key (Verticall key derivation ref: ts 33.401) */
   } u;
} CtfKdfReqInfo;

/********************************************************************************************************************************/
                              /*END OF REQUEST STRUCTURES*/
/********************************************************************************************************************************/


/********************************************************************************************************************************/
                              /*START OF KEY DERIVATION FUNTION (KDF) CONFIRM STRUCTURES*/
/********************************************************************************************************************************/

/*AS key generation output information structure*/
typedef struct ctfAskeyCfmInfo{
          uint8_t intKey[CTF_SEC_KEY_LEN]; /*!< eNodeB key received in initial context setup */
          uint8_t upCiphKey[CTF_SEC_KEY_LEN]; /*!< eNodeB key received in initial context setup */
          uint8_t cpCiphKey[CTF_SEC_KEY_LEN]; /*!< eNodeB key received in initial context setup */
}CtfAskeyCfmInfo;

/*Horizontal keNB derivation output information structure*/
typedef struct ctfKenbStarCfmInfo{
           uint8_t secKey[CTF_SEC_KEY_LEN]; /*!< Current eNodeB key at App used to derive keNB*. Horizontal KD */
}CtfKenbStarCfmInfo;

/*Vertical keNB derivation output information structure*/
typedef struct ctfNhKeyCfmInfo{
           uint8_t secKey[CTF_SEC_KEY_LEN]; /*!< NH key provided by MME used to derive keNB*. Vertical KD */
}CtfNhKeyCfmInfo;

typedef struct ctfKdfCfmInfo {
   uint8_t    keyDerType; /*!< Keyderivation type type: for AS security keys/keNB* from keNB/keNB* from NH key */
   union
   {
      CtfAskeyCfmInfo      kEnbInf;     /*!< AS security keys */
      CtfKenbStarCfmInfo   kEnbStarInf;  /*!< eNodeB* Key (Horizontal key derivation ref: ts 33.401) */
      CtfNhKeyCfmInfo      nhKeyInf;     /*!< eNodeB* Key (Verticall key derivation ref: ts 33.401) */
   } u;
} CtfKdfCfmInfo;
#endif
/********************************************************************************************************************************/
                              /*START OF CNM REQUEST STRUCTURES*/
/********************************************************************************************************************************/


typedef struct ctfCnmVendorSpecificInfo{
	uint16_t        numOfVendorSpecifcParam;            /* Number of vendor specific parameters */
	uint32_t        vendorSpecifcParam[CTF_CNM_MAX_VENDOR_PARAMS]; /*List of vendor specific parameters */
}CtfCnmVendorSpecificInfo;



typedef struct ctfCnmCellSyncReq {
	uint16_t                      nbrEarfcn;            /*earfcn of the neighbour to be synced with*/ 				
	uint16_t                      nbrPCellId;           /*PCI of the neighbour to be synced with*/ 				
	uint8_t                       nbrTxAntCount;        /*Number of Anttennas of the neighbour to be synced with
	                                                 Possible values 1,2,4 */ 				
    uint8_t                       nbrCellCp;            /* Cyclic perifx type of the neighbout cell 
	                                                * 0 : Normal Cp, 1:Extended Cp */
    uint8_t                       nbrCellNRb;           /* Number of Resource blocks of the neighbour cell 
	                                                * possible values 6,15,25,50,75,100 */
    uint8_t                       nbrSpecSfCfg;         /* Special Subframe configuraiton value of the neighbour cell */ 	
    CtfCnmVendorSpecificInfo ctfCnmVendorSpecificInfo;
}CtfCnmCellSyncReq;


typedef struct ctfCnmInitSyncReq {
	uint8_t                   searchType;           /*0: search for all cells.
	                                             *1: search for strongest cell. 
												 *2: Search for specific cell. */
	uint8_t                    mibRequest;           /*Flag to indicate if MIB information 
	                      						of the searched cell should be 
	                      						indicated in cell search response message.*/
	uint16_t                   earfcn;               /*earfcn to the searched for */ 				
	uint8_t                    measBandWidth;        /*Number of Rb */					 
	uint16_t                   numOfPciList;         /*Number of Pci to be searched for */
	CtfCnmCellSyncReq     pciList[CTF_CNM_MAX_CELL_SEARCH];
}CtfCnmInitSyncReq;
/********************************************************************************************************************************/
                              /*END OF REQUEST STRUCTURES*/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
                              /*START OF CNM CONFIRM STRUCTURES*/
/********************************************************************************************************************************/

typedef struct ctfCnmMibInfo {
	uint16_t      sfn;               /* The radio frame the data was received on */
	uint8_t       numTxAntennas;     /* Number of TX antennas of the cell */
	uint8_t       phichDuration;     /* The PHICH duration of the cell 0-Normal 1-Extended*/
	uint8_t       phichResource;     /* 0=1/6, 1=1/2 , 2=1 ,3=2 */ 
	uint8_t       dlSystemBandWidht; /*Possible values 6,15,25,50,75,100 */

}CtfCnmMibInfo;


typedef struct ctfCnmInitSyncInfo {
	uint16_t           pci;          /* Physical cell Id of the Cell */
	uint8_t            rsrp;         /* RSRP of the measured cell */
	uint8_t            mibValidFlag; /* 1=MIB present 0=MIB not present */
	uint16_t           localSfn;     /* The radio frame data was received on the local cell */
	CtfCnmMibInfo ctfCnmMibInfo;/* MIB information */
    CtfCnmVendorSpecificInfo ctfCnmVendorSpecificInfo;
}CtfCnmInitSyncInfo;


typedef struct ctfCnmInitSyncRsp {
	uint8_t                 status;             /*RFAILURE or ROK */
	uint16_t                earfcn;             /*Frequency channel of the searched cell */
	uint8_t                 numOfNeighbourInfo; /* number of neighboure Cell search Info */
	CtfCnmInitSyncInfo ctfCnmInitSyncInfo[CTF_CNM_MAX_CELL_SEARCH];
}CtfCnmInitSyncRsp;

typedef struct ctfCnmCellSyncRsp {
	uint8_t                 status;             /*RFAILURE or ROK */
}CtfCnmCellSyncRsp;

typedef struct ctfCnmCellSyncInd {
	uint8_t                 status;             /*RFAILURE or ROK */
    CtfCnmVendorSpecificInfo ctfCnmVendorSpecificInfo;
}CtfCnmCellSyncInd;

/* Periodic REM for TPM */
typedef struct ctfPeriodicRemCellInfo {
   uint16_t                     pci;
   uint8_t                      rsrp;
   uint8_t                      dlBw;
   uint8_t                      numTx;
}CtfPeriodicRemCellInfo;

typedef struct ctfPeriodicRemCellSearchRsp {
   uint8_t    numCells;       /* Number of Cells */
   CtfPeriodicRemCellInfo   cellInfo[CTF_REM_MAX_CELL_SEARCH];
}CtfPeriodicRemCellSearchRsp;
/* Periodic REM for TPM End */

/********************************************************************************************************************************/
                              /*END OF CNM CONFIRM STRUCTURES*/
/********************************************************************************************************************************/
/* 
 * Function Prototypes 
*/
/* Request from User to PHY to bind the CTF interface SAP. */
typedef S16 (*CtfBndReq) ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId));

/* Confirmation from PHY to User for the bind request of the CTF interface SAP.
*/
typedef S16 (*CtfBndCfm) ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status));

/* Request from User to PHY to Unbind the CTF interface SAP. */
typedef S16 (*CtfUbndReq) ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason));

/* Configuration Request from User to PHY for cell or dedicated configuration. 
*/
typedef S16 (*CtfCfgReq) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfCfgReqInfo*       cfgReqInfo));

/* Configuration Confirm from PHY to User. */
typedef S16 (*CtfCfgCfm) ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   uint8_t                   status));

/* EnodeB stop indication from PHY to User. */
typedef S16 (*CtfEnbStopInd) ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId));

#ifdef TENB_AS_SECURITY
typedef S16 (*CtfKdfReq) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfKdfReqInfo*       cfgReqInfo));

/* Configuration Confirm from PHY to User. */
typedef S16 (*CtfKdfCfm) ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   CtfKdfCfmInfo*       kdfCfmInfo,
   uint8_t                   status));
#endif
/* CNM start */
typedef S16 (*CtfCnmInitSyncReqMsg) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId*        transId,
   CtfCnmInitSyncReq*    cnmInitSyncReq));

typedef S16 (*CtfCnmInitSyncRspMsg) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId*       transId,
   CtfCnmInitSyncRsp*   cnmInitSyncRsp));

typedef S16 (*CtfCnmCellSyncReqMsg) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId*        transId,
   CtfCnmCellSyncReq*   cnmCellSyncReq));

typedef S16 (*CtfCnmCellSyncRspMsg) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId*       transId,
   CtfCnmCellSyncRsp*   cnmCellSyncRsp));

typedef S16 (*CtfCnmCellSyncIndMsg) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId*       transId,
   CtfCnmCellSyncInd*   cnmCellSyncInd));

/*CNM end */

/* UE ID Change Request from User to PHY. */
typedef S16 (*CtfUeIdChgReq) ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CtfUeInfo *          newUeInfo));

/* UE ID Change Confirm from PHY to User. */
typedef S16 (*CtfUeIdChgCfm) ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CmStatus             status));

/** 
 * @brief Bind Request from PHY User to bind the CTF interface SAP.
 *
 * @details This primitiveis used to bind a user to the CTF interface.
 *          -# PHY layer registers this user, allocates a Service Access Point
 *          for this bind and records the identity of the service user.
 *          -# PHY issues a @ref TfUiCtfBndCfm to the service user after
 *          completing a successful bind.
 *
 * @param[in] pst  Pointer to the post structure. SSI uses pst for loosely 
 *                 coupled intertask communication.
 * @param[in] suId The service user SAP ID. The service user uses this
 *                 reference number to identify a SAP. The service user provides
 *                 this ID and the service provider retains it. The service 
 *                 provider provides this reference number in any further 
 *                 primitives that the service provider sends to the service 
 *                 user.
 * @param[in] spId The service provider SAP ID. The service provider uses this
 *                 reference number to identify its SAP. The service user 
 *                 provides this reference number in any further primitives 
 *                 that the service user sends to the service provider.
 *
 * @return S16
 *  -# ROK on success
 *  -# RFAILED on failure
 *
*/
S16 TfUiCtfBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));

/** 
 * @brief Bind Confirm from PHY to User.
 *
 * @details This primitive is used to report the status of processing a bind 
 *          request. If the service user does not receive a bind confirm 
 *          primitive, it must stop trying to bind after a limited number of 
 *          retries. The service provider must send a bind confirm upon 
 *          receiving a bind request primitive from the service user. These 
 *          status values are requested:
 *          - CM_BND_OK: This status is reported when the bind request is 
 *                       successful.
 *          - CM_BND_NOK: This status is reported when the bind request is unsuccessful.
 *
 * @param[in] pst  Pointer to the post structure. SSI uses pst for loosely 
 *                 coupled intertask communication.
 * @param[in] suId The service user SAP ID. The service user uses this 
 *                 reference number to identify its lower control block.
 * @param[in] status The allowed values of status are:
 *                   CM_BND_OK   1
 *                   CM_BND_NOK  2
 *
 * @return S16
 * -# ROK 
 * -# RFAILED 
 *
*/
S16 TfUiCtfBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));

/** 
 * @brief Unbind Request from PHY user.
 *
 * @details This primitive is used to unbind the user SAP. The SAP is not
 *          deallocated and hence a new bind request can be issued on this SAP
 *          (without reconfiguring the SAP in PHY).
 *
 * @param[in] pst Pointer to the post structure. SSI uses pst for loosely 
 *                coupled inter-task communication.
 * @param[in] spId The service provider SAP ID. The service provider uses this
 *                 this reference number to idenitfy the SAP.
 * @param[in] reason Defines the reason for the unbind.
 *
 * @return S16
 * -# ROK
 * -# RFAILED
 *
*/
S16 TfUiCtfUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));

/** 
 * @brief Configuration Request from User.
 *
 * @details This primitive is used by User for the following at PHY:
 *          - Configure cell-related parameters.
 *          - Reconfigure cell-related parameters.
 *          - Release a cell context.
 *          - Configure UE-specific information.
 *          - Reconfigure UE-specific information.
 *          - Release UE-specific configuration.
 * The cell-specific configuration is identified using a cell identifier.
 * The UE-specific configuration is identified using a cell identifier and 
 * an UE identifier.
 * The configuration request primitive is assocated with a transaction 
 * identifier to correlate the configuration confirm returned by PHY.
 *
 * @param[in] pst Pointer to the service user task configuration structure.
 * @param[in] spId The service provider SAP ID.
 * @param[in] transId The transaction identifier.
 * @param[in] cfgReqInfo Pointer to the PHY(Cell/UE) configuration information.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
 *
*/
S16 TfUiCtfCfgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfCfgReqInfo*       cfgReqInfo
));

/** 
 * @brief Configuration Confirm from PHY to user.
 *
 * @details This primitive is used to indicate to the service user the status of
 *          configuration done at PHY as a response to configuration request.
 *          The status field in this primitive carries the status of processing
 *          this primitive at PHY. A configuration is treated as success, only 
 *          if it is successful at PHY. The primitive is associated with a 
 *          transaction identifier that is received in the request.
 *
 * @param[in] pst Pointer to the post structure. SSI uses pst for loosely coupled
 *                inter-task communication.
 * @param[in] suId    The service user SAP ID.
 * @param[in] transId The transaction identifier associated with the 
 *                    configuration request.
 * @param[in] status The status of the configuration request processing at PHY.
 *                   -# CTF_CFG_CFM_OK 1
 *                   -# CTF_CFG_CFM_NOK 2
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 *
*/
S16 TfUiCtfCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   uint8_t                   status
));

/** 
 * @brief Configuration Request from User.
 *
 * @details This primitive is used by User for the following at PHY:
 * The cell-specific configuration is identified using a cell identifier.
 * The UE-specific configuration is identified using a cell identifier and 
 * an UE identifier.
 * The configuration request primitive is assocated with a transaction 
 * identifier to correlate the configuration confirm returned by PHY.
 *
 * @param[in] pst Pointer to the service user task configuration structure.
 * @param[in] spId The service provider SAP ID.
 * @param[in] transId The transaction identifier.
 * @param[in] cfgReqInfo Pointer to the PHY(Cell/UE) configuration information.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
 *
*/
S16 TfUiCtfCnmCellSyncReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId *      transId,
   CtfCnmCellSyncReq*   cfgReq
));

/** 
 * @brief Configuration Confirm from PHY to user.
 *
 * @details This primitive is used to indicate to the service user the status of
 *          configuration done at PHY as a response to configuration request.
 *          The status field in this primitive carries the status of processing
 *          this primitive at PHY. A configuration is treated as success, only 
 *          if it is successful at PHY. The primitive is associated with a 
 *          transaction identifier that is received in the request.
 *
 * @param[in] pst Pointer to the post structure. SSI uses pst for loosely coupled
 *                inter-task communication.
 * @param[in] suId    The service user SAP ID.
 * @param[in] transId The transaction identifier associated with the 
 *                    configuration request.
 * @param[in] status The status of the configuration request processing at PHY.
 *                   -# CTF_CFG_CFM_OK 1
 *                   -# CTF_CFG_CFM_NOK 2
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 *
*/
S16 TfUiCtfCnmCellSyncRsp ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId  *     transId,
   CtfCnmCellSyncRsp *  cnmCellSyncRsp
));

/** 
 * @brief Configuration Request from User.
 *
 * @details This primitive is used by User for the following at PHY:
 * The cell-specific configuration is identified using a cell identifier.
 * The UE-specific configuration is identified using a cell identifier and 
 * an UE identifier.
 * The configuration request primitive is assocated with a transaction 
 * identifier to correlate the configuration confirm returned by PHY.
 *
 * @param[in] pst Pointer to the service user task configuration structure.
 * @param[in] spId The service provider SAP ID.
 * @param[in] transId The transaction identifier.
 * @param[in] cfgReqInfo Pointer to the PHY(Cell/UE) configuration information.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
 *
*/
S16 TfUiCtfCnmInitSyncReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId *      transId,
   CtfCnmInitSyncReq*   cfgReq
));

/** 
 * @brief Configuration Confirm from PHY to user.
 *
 * @details This primitive is used to indicate to the service user the status of
 *          configuration done at PHY as a response to configuration request.
 *          The status field in this primitive carries the status of processing
 *          this primitive at PHY. A configuration is treated as success, only 
 *          if it is successful at PHY. The primitive is associated with a 
 *          transaction identifier that is received in the request.
 *
 * @param[in] pst Pointer to the post structure. SSI uses pst for loosely coupled
 *                inter-task communication.
 * @param[in] suId    The service user SAP ID.
 * @param[in] transId The transaction identifier associated with the 
 *                    configuration request.
 * @param[in] status The status of the configuration request processing at PHY.
 *                   -# CTF_CFG_CFM_OK 1
 *                   -# CTF_CFG_CFM_NOK 2
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 *
*/
S16 TfUiCtfCnmInitSyncRsp ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId  *     transId,
   CtfCnmInitSyncRsp *  cnmInitSyncRsp
));

/** 
 * @brief Configuration Confirm from PHY to user.
 *
 * @details This primitive is used to indicate to the service user the status of
 *          configuration done at PHY as a response to configuration request.
 *          The status field in this primitive carries the status of processing
 *          this primitive at PHY. A configuration is treated as success, only 
 *          if it is successful at PHY. The primitive is associated with a 
 *          transaction identifier that is received in the request.
 *
 * @param[in] pst Pointer to the post structure. SSI uses pst for loosely coupled
 *                inter-task communication.
 * @param[in] suId    The service user SAP ID.
 * @param[in] transId The transaction identifier associated with the 
 *                    configuration request.
 * @param[in] status The status of the configuration request processing at PHY.
 *                   -# CTF_CFG_CFM_OK 1
 *                   -# CTF_CFG_CFM_NOK 2
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 *
*/
S16 TfUiCtfCnmCellSyncInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId  *     transId,
   CtfCnmCellSyncInd *  cnmCellSyncInd
));

/** 
 * @brief UE ID Change Request from User to PHY.
 *
 * @details This primitive is used by User for reconfiguring the C-RNTI
 *          at PHY of an UE.
 *
 * @param[in] pst Pointer to the service user task configuration structure.
 * @param[in] spId The service provider SAP ID.
 * @param[in] transId The transaction identifier.
 * @param[in] ueInfo UE ID (C-RNTI) of the UE.
 * @param[in] newUeInfo New UE ID (C-RNTI) for the UE denoted by ueInfo.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
 *
*/
S16 TfUiCtfUeIdChgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CtfUeInfo *          newUeInfo
));

/** 
 * @brief UE ID Change Confirm from PHY to user.
 *
 * @details This primitive is used to indicate to the service user the status of
 *          configuration done at PHY as a response to configuration request.
 *          The status field in this primitive carries the status of processing
 *          this primitive at PHY. A configuration is treated as success, only 
 *          if it is successful at PHY. The primitive is associated with a 
 *          transaction identifier that is received in the request.
 *
 * @param[in] pst Pointer to the post structure. SSI uses pst for loosely coupled
 *                inter-task communication.
 * @param[in] suId    The service user SAP ID.
 * @param[in] transId The transaction identifier associated with the configuration
 *                    request.
 * @param[in] status The status of the configuration request processging at PHY.
 *                   -# CTF_CFG_CFM_OK 1
 *                   -# CTF_CFG_CFM_NOK 2
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 *
*/
/** UE ID Change Confirm from PHY to User. */
S16 TfUiCtfUeIdChgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CmStatus             status
));


#ifdef NH
/* Request from User to PHY to Bind the CTF interface SAPs. */
S16 NhLiCtfBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/* Confirmation from PHY to User for the Bind/Unbind 
 * Request for the CTF interface SAP. */
S16 NhLiCtfBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/* Request from User to PHY to Unbind the CTF interface SAP. */
S16 NhLiCtfUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/* Configuration Request from User to PHY for 
 * cell or dedicated configuration. */
S16 NhLiCtfCfgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfCfgReqInfo*       cfgReqInfo
));
/* Configuration Confirm from PHY to User. */
S16 NhLiCtfCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   uint8_t                   status 
));
/** UE ID Change Request from User to PHY. */
S16 NhLiCtfUeIdChgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CtfUeInfo *          newUeInfo
));
/** UE ID Change Confirm from PHY to user. */
S16 NhLiCtfUeIdChgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CmStatus             status
));
#endif /* NH */

#ifdef LCCTF
/* Request from User to PHY to Bind the CTF interface SAP. */
S16 cmPkCtfBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/* Request from User to PHY to Bind the CTF interface SAP. */
S16 cmUnpkCtfBndReq ARGS((
   CtfBndReq            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* Confirmation from PHY to user for the Bind/Unbind 
 * Request for the CTF interface SAP.
*/
S16 cmPkCtfBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/* Confirmation from PHY to User for the Bind/Unbind 
 * Request for the CTF interface SAP.
*/
S16 cmUnpkCtfBndCfm ARGS((
   CtfBndCfm            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* Request from User to PHY to Unbind the CTF interface SAP. */
S16 cmPkCtfUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/* Request from User to PHY to Unbind the interface SAPs. */
S16 cmUnpkCtfUbndReq ARGS((
   CtfUbndReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* Configuration Request from User to PHY for 
 * cell or dedicated configuration. 
*/
/* Cnm Start */
S16 cmPkCtfCnmCellSyncReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId  *     transId,
   CtfCnmCellSyncReq *  ctfCnmCellSyncreq 
));

S16 cmUnpkCnmCellSyncReq ARGS((
   CtfCnmCellSyncReqMsg func,
   Pst*                 pst,
   Buffer               *mBuf
));

S16 cmPkCtfCnmCellSyncRsp ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId  *     transId,
   CtfCnmCellSyncRsp *  ctfCnmCellSyncrsp 
));

S16 cmUnpkCnmCellSyncRsp ARGS((
   CtfCnmCellSyncRspMsg func,
   Pst*                 pst,
   Buffer               *mBuf
));


S16 cmPkCtfCnmCellSyncInd ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId  *     transId,
   CtfCnmCellSyncInd *  ctfCnmCellSyncind
));

S16 cmUnpkCnmCellSyncInd ARGS((
   CtfCnmCellSyncIndMsg func,
   Pst*                 pst,
   Buffer               *mBuf
));

S16 cmPkCtfCnmInitSyncReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId  *     transId,
   CtfCnmInitSyncReq *  ctfCnmInitSyncreq
));
S16 cmUnpkCnmInitSyncReq ARGS((
   CtfCnmInitSyncReqMsg func,
   Pst*                 pst,
   Buffer               *mBuf
));

S16 cmPkCtfCnmInitSyncRsp ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId  *     transId,
   CtfCnmInitSyncRsp *  ctfCnmInitSyncrsp
));

S16 cmUnpkCnmInitSyncRsp ARGS((
   CtfCnmInitSyncRspMsg func,
   Pst*                 pst,
   Buffer               *mBuf
));


/* Cnm End */
#ifdef TENB_AS_SECURITY
S16 cmPkCtfKdfReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfKdfReqInfo  *     kdfReqInfo
));
/* Configuration Request from User to PHY for 
 * cell or dedicated configuration.
*/
S16 cmUnpkCtfKdfReq ARGS((
   CtfKdfReq            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* Configuration Confirm from PHY to User. */
S16 cmPkCtfkdfCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   CtfKdfCfmInfo        *kdfCfmInfo,
   uint8_t                   status
));
/* Configuration Confirm from PHY to User. */
S16 cmUnpkCtfKdfCfm ARGS((
   CtfKdfCfm            func,
   Pst*                 pst,
   Buffer               *mBuf
));

#endif
S16 cmPkCtfCfgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfCfgReqInfo  *     cfgReqInfo
));
/* Configuration Request from User to PHY for 
 * cell or dedicated configuration.
*/
S16 cmUnpkCtfCfgReq ARGS((
   CtfCfgReq            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* Configuration Confirm from PHY to User. */
S16 cmPkCtfCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   uint8_t                   status
));
/* ENB-STOP Indication from PHY to User. */
S16 cmPkCtfEnbStopInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId
));
/* Configuration Confirm from PHY to User. */
S16 cmUnpkCtfCfgCfm ARGS((
   CtfCfgCfm            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* unpack ENB-STOP Indication from PHY to User. */
S16 cmUnpkCtfEnbStopInd ARGS((
   CtfEnbStopInd        func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** UE ID Change Request from User to PHY. */
S16 cmPkCtfUeIdChgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CtfUeInfo *          newUeInfo
));
/** UE ID Change Request from User to PHY. */
S16 cmUnpkCtfUeIdChgReq ARGS((
   CtfUeIdChgReq        func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** UE ID Change Confirm from PHY to User. */
S16 cmPkCtfUeIdChgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CmStatus             status
));
/** UE ID Change Confirm from PHY to User. */
S16 cmUnpkCtfUeIdChgCfm ARGS((
   CtfUeIdChgCfm        func,
   Pst*                 pst,
   Buffer               *mBuf
));

/* Periodic REM Scan changes */

typedef S16 (*CtfPerRemCellSrchRsp) ARGS((
   Pst*                          pst,
   SpId                          spId,
   CtfCfgTransId*                transId,
   CtfPeriodicRemCellSearchRsp*  sniffCellRsp));

S16 cmUnpkPerRemCellSrchRsp ARGS ((
   CtfPerRemCellSrchRsp  func,
   Pst*                  pst,
   Buffer                *mBuf
));

S16 cmUnpkCtfPerRemCellSrch ARGS ((
CtfPeriodicRemCellSearchRsp *param,
Buffer *mBuf
));

S16 cmUnpkCtfPerRemScanCellInfo ARGS ((
CtfPeriodicRemCellInfo *param,
Buffer *mBuf
));

S16 cmPkCtfPeriodicCellSearchRsp ARGS ((
CtfPeriodicRemCellSearchRsp *param,
Buffer *mBuf
));

S16 cmPkCtfPeriodicRemCellSearchRsp ARGS ((
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfPeriodicRemCellSearchRsp *CellCfgRsp
));

S16 cmPkPerRemScanCellInfo ARGS ((
CtfPeriodicRemCellInfo *param,
Buffer *mBuf
));

/* Periodic REM Scan changes End */
#endif /* LCCTF */

#ifdef DM
/* Request from User to PHY to Bind the CTF interface SAP. */
S16 DmUiCtfBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/* Confirmation from PHY to User for the Bind/Unbind 
 * Request for the CTF interface SAP.
*/
S16 DmUiCtfBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/* Request from User to PHY to Unbind the CTF interface SAP. */
S16 DmUiCtfUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/* Configuration Request from User to PHY for 
 * cell or dedicated configuration.
*/
S16 DmUiCtfCfgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfCfgReqInfo*       cfgReqInfo
));
/* Configuration Confirm from PHY to User. */
S16 DmUiCtfCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   uint8_t                   status
));
/** UE ID Change Request from User to PHY. */
S16 DmUiCtfUeIdChgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   CtfUeInfo *          newUeInfo
));
/** UE ID Change Confirm from PHY to user. */
S16 DmUiCtfUeIdChgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CtfCfgTransId        transId,
   CtfUeInfo *          ueInfo,
   uint8_t                   status
));
#endif /* DM */

/*****************************************************************************
 *                      PACK/UNPACK FUNCTIONS
 ****************************************************************************/
#ifdef LCCTF
S16 cmPkCtfCfgTransId ARGS((
   CtfCfgTransId        *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfCfgTransId ARGS((
   CtfCfgTransId        *param,
   Buffer               *mBuf
));
S16 cmPkCtfUeInfo ARGS((
   CtfUeInfo            *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfUeInfo ARGS((
   CtfUeInfo            *param,
   Buffer               *mBuf
));
S16 cmPkCtfBwCfgInfo ARGS((
   CtfBwCfgInfo         *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfBwCfgInfo ARGS((
   CtfBwCfgInfo         *param,
   Buffer               *mBuf
));
S16 cmPkCtfTxSchemeCfg ARGS((
   CtfTxSchemeCfg       *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfTxSchemeCfg ARGS((
   CtfTxSchemeCfg       *param,
   Buffer               *mBuf
));
S16 cmPkCtfAntennaCfgInfo ARGS((
   CtfAntennaCfgInfo    *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfAntennaCfgInfo ARGS((
   CtfAntennaCfgInfo    *param,
   Buffer               *mBuf
));
S16 cmPkCtfPrachCfgInfo ARGS((
   CtfPrachCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfPrachCfgInfo ARGS((
   CtfPrachCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfPdschCfgInfo ARGS((
   CtfPdschCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfPdschCfgInfo ARGS((
   CtfPdschCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfPuschCfgBasic ARGS((
   CtfPuschCfgBasic     *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfPuschCfgBasic ARGS((
   CtfPuschCfgBasic     *param,
   Buffer               *mBuf
));
S16 cmPkCtfPuschUlRS ARGS((
   CtfPuschUlRS         *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfPuschUlRS ARGS((
   CtfPuschUlRS         *param,
   Buffer               *mBuf
));
S16 cmPkCtfPuschCfgInfo ARGS((
   CtfPuschCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfPuschCfgInfo ARGS((
   CtfPuschCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfPhichCfgInfo ARGS((
   CtfPhichCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfPhichCfgInfo ARGS((
   CtfPhichCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfPucchCfgInfo ARGS((
   CtfPucchCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfPucchCfgInfo ARGS((
   CtfPucchCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfSrsUlCfgInfo ARGS((
   CtfSrsUlCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfSrsUlCfgInfo ARGS((
   CtfSrsUlCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfTddSfCfgInfo ARGS((
   CtfTddSfCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfTddSfCfgInfo ARGS((
   CtfTddSfCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfCellCfgInfo ARGS((
   CtfCellCfgInfo       *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfCellCfgInfo ARGS((
   CtfCellCfgInfo       *param,
   Buffer               *mBuf
));
S16 cmPkCtfCellRecfgInfo ARGS((
   CtfCellRecfgInfo     *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfCellRecfgInfo ARGS((
   CtfCellRecfgInfo     *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedPdschCfgInfo ARGS((
   CtfDedPdschCfgInfo   *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedPdschCfgInfo ARGS((
   CtfDedPdschCfgInfo   *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedPucchCfgInfo ARGS((
   CtfDedPucchCfgInfo   *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedPucchCfgInfo ARGS((
   CtfDedPucchCfgInfo   *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedPuschCfgInfo ARGS((
   CtfDedPuschCfgInfo   *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedPuschCfgInfo ARGS((
   CtfDedPuschCfgInfo   *param,
   Buffer               *mBuf
));
#ifndef TFU_UPGRADE
S16 cmPkCtfCqiRptModePeriodic ARGS((
   CtfCqiRptModePeriodic *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfCqiRptModePeriodic ARGS((
   CtfCqiRptModePeriodic *param,
   Buffer               *mBuf
));
S16 cmPkCtfCqiReportCfgInfo ARGS((
   CtfCqiReportCfgInfo  *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfCqiReportCfgInfo ARGS((
   CtfCqiReportCfgInfo  *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedSrsUlCfgInfo ARGS((
   CtfDedSrsUlCfgInfo   *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedSrsUlCfgInfo ARGS((
   CtfDedSrsUlCfgInfo   *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedSRCfgInfo ARGS((
   CtfDedSRCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedSRCfgInfo ARGS((
   CtfDedSRCfgInfo      *param,
   Buffer               *mBuf
));
#endif /* TFU_UPGRADE */
S16 cmPkCtfDedAntCfgInfo ARGS((
   CtfDedAntCfgInfo     *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedAntCfgInfo ARGS((
   CtfDedAntCfgInfo     *param,
   Buffer               *mBuf
));

S16 cmPkCtfUeSecCellCfgInfo ARGS((
CtfUeSecCellCfgInfo *param,
Buffer *mBuf
));

S16 cmPkCtfSecCellCfgInfo ARGS((
CtfSecCellCfgInfo *param,
Buffer *mBuf
));

S16 cmUnpkCtfUeSecCellCfgInfo ARGS((
CtfUeSecCellCfgInfo *param,
Buffer *mBuf
));

S16 cmUnpkCtfSecCellCfgInfo ARGS((
CtfSecCellCfgInfo *param,
Buffer *mBuf
));

S16 cmPkCtfUeCatCfgInfo ARGS((
   CtfUeCatCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfUeCatCfgInfo ARGS((
   CtfUeCatCfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedCfgInfo ARGS((
   CtfDedCfgInfo        *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedCfgInfo ARGS((
   CtfDedCfgInfo        *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedRecfgInfo ARGS((
   CtfDedRecfgInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedRecfgInfo ARGS((
   CtfDedRecfgInfo      *param,
   Buffer               *mBuf
));
S16 cmPkCtfCellReleaseInfo ARGS((
   CtfCellReleaseInfo   *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfCellReleaseInfo ARGS((
   CtfCellReleaseInfo   *param,
   Buffer               *mBuf
));
S16 cmPkCtfDedReleaseInfo ARGS((
   CtfDedReleaseInfo    *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfDedReleaseInfo ARGS((
   CtfDedReleaseInfo    *param,
   Buffer               *mBuf
));
S16 cmPkCtfCfgInfo ARGS((
   CtfCfgInfo           *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfCfgInfo ARGS((
   CtfCfgInfo           *param,
   Buffer               *mBuf
));
S16 cmPkCtfReCfgInfo ARGS((
   CtfReCfgInfo         *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfReCfgInfo ARGS((
   CtfReCfgInfo         *param,
   Buffer               *mBuf
));
S16 cmPkCtfReleaseInfo ARGS((
   CtfReleaseInfo       *param,
   Buffer               *mBuf
));
S16 cmUnpkCtfReleaseInfo ARGS((
   CtfReleaseInfo       *param,
   Buffer               *mBuf
));
S16 cmPkCtfCfgReqInfo ARGS((
   CtfCfgReqInfo        *param,
   Buffer               *mBuf
));
/* ctf_x_001.main_4: Added support for vendor specific parameters */
S16 cmUnpkCtfCfgReqInfo ARGS((
   Pst                  *pst,
   CtfCfgReqInfo        *param,
   Buffer               *mBuf
));

#ifdef CTF_VER3
S16 cmPkCtfVendorSpecParams ARGS((
   CtfVendorSpecific *param,
   Buffer *mBuf
));

S16 cmUnpkCtfVendorSpecParams ARGS((
    Pst*                 pst,
    CtfVendorSpecific *param,
    Buffer *mBuf
));
#endif /* CTF_VER3*/

/* Starts - Fix for CR ccpu00123185 */
S16 cmPkCtfCellPwrcfgInfo ARGS((

    CtfCellRecfgInfo *param,
    Buffer *mBuf
));

S16 cmPkCtfCellStop ARGS((
    CtfCellRecfgInfo *param,
    Buffer *mBuf
));

S16 cmUnpkCtfCellPwrcfgInfo ARGS((
    CtfCellRecfgInfo *param,
    Buffer *mBuf
));
S16 cmUnpkCtfCellStop ARGS((
    CtfCellRecfgInfo *param,
    Buffer *mBuf
));
/* Ends - Fix for CR ccpu00123185 */
#ifdef TENB_AS_SECURITY
S16 cmPkCtfNhKeyInfo ARGS(
(
CtfNhKeyInfo *param,
Buffer *mBuf
));
S16 cmPkCtfKenbStarInfo ARGS(
(
CtfKenbStarInfo *param,
Buffer *mBuf
));
S16 cmPkCtfAskeyInfo ARGS(
(
CtfAsKeyInfo *param,
Buffer *mBuf
));
S16 cmPkCtfKdfReqInfo ARGS(
(
CtfKdfReqInfo *param,
Buffer *mBuf
 ));
S16 cmPkCtfKdfReq ARGS(
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfKdfReqInfo* kdfReqInfo
));
S16 cmUnpkCtfNhKeyInfo ARGS(
(
CtfNhKeyInfo *param,
Buffer *mBuf
));
S16 cmUnpkCtfKenbStarInfo ARGS(
(
CtfKenbStarInfo *param,
Buffer *mBuf
));
S16 cmUnpkCtfAskeyInfo ARGS(
(
CtfAsKeyInfo *param,
Buffer *mBuf
));
S16 cmUnpkCtfKdfReqInfo ARGS(
(
CtfKdfReqInfo *param,
Buffer *mBuf
 ));
S16 cmUnpkCtfKdfReq ARGS(
(
CtfKdfReq func,
Pst *pst,
Buffer *mBuf
));
S16 cmPkCtfNhKeyCfmInfo ARGS(
(
CtfNhKeyCfmInfo *param,
Buffer *mBuf
));
S16 cmPkCtfKenbStarCfmInfo ARGS(
(
CtfKenbStarCfmInfo *param,
Buffer *mBuf
));
S16 cmPkCtfAskeyCfmInfo ARGS(
(
CtfAskeyCfmInfo *param,
Buffer *mBuf
));
S16 cmPkCtfKdfCfmInfo ARGS(
(
CtfKdfCfmInfo *param,
Buffer *mBuf
 ));
S16 cmPkCtfKdfCfm ARGS(
(
Pst* pst,
SuId suId,
CtfCfgTransId transId,
CtfKdfCfmInfo *kdfCfmInfo,
uint8_t status
));
S16 cmUnpkCtfNhKeyCfmInfo ARGS(
(
CtfNhKeyCfmInfo *param,
Buffer *mBuf
));
S16 cmUnpkCtfKenbStarCfmInfo ARGS(
(
CtfKenbStarCfmInfo *param,
Buffer *mBuf
));
S16 cmUnpkCtfAskeyCfmInfo ARGS(
(
CtfAskeyCfmInfo *param,
Buffer *mBuf
));
S16 cmUnpkCtfKdfCfmInfo ARGS(
(
CtfKdfCfmInfo *param,
Buffer *mBuf
));
S16 cmUnpkCtfKdfCfm ARGS(
(
CtfKdfCfm func,
Pst *pst,
Buffer *mBuf
));
#endif
#endif /* LCCTF */

S16 UrUiCtfHandleSnfMsgs ARGS((
Pst*                 pst,
Buffer*              mBuf
));

S16 cmPkCtfCnmVendorSpecificInfo ARGS((
CtfCnmVendorSpecificInfo *param,
Buffer *mBuf
));

S16 cmPkCtfCtfCnmCellSyncReq ARGS((
CtfCnmCellSyncReq *param,
Buffer *mBuf
));

S16 cmUnpkCtfCnmVendorSpecificInfo ARGS((
CtfCnmVendorSpecificInfo *param,
Buffer *mBuf
));

S16 cmUnpkCtfCtfCnmCellSyncReq ARGS((
CtfCnmCellSyncReq *param,
Buffer *mBuf
));

S16 cmPkCtfCnmCellSyncReq ARGS((
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmCellSyncReq * cfgReqInfo
));

S16 cmUnpkCtfCnmCellSyncReq ARGS((
CtfCnmCellSyncReqMsg func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkCtfCnmInitSyncReq ARGS((
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmInitSyncReq * cfgReqInfo
));

S16 cmUnpkCtfCnmInitSyncReq ARGS((
CtfCnmInitSyncReqMsg func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkCtfctfCnmMibInfo ARGS((
CtfCnmMibInfo *param,
Buffer *mBuf
));

S16 cmPkCtfctfCnmInitSyncInfo ARGS((
CtfCnmInitSyncInfo *param,
Buffer *mBuf
));

S16 cmPkCtfCnmInitSyncRsp ARGS((
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmInitSyncRsp * cfgRspInfo
));

S16 cmUnpkCtfctfCnmMibInfo ARGS((
CtfCnmMibInfo *param,
Buffer *mBuf
));

S16 cmUnpkctfCnmInitSyncInfo ARGS(( 
CtfCnmInitSyncInfo *param,
Buffer *mBuf
));

S16 cmUnpkCtfCnmInitSyncRspMsg ARGS((
CtfCnmInitSyncRsp *param,
Buffer *mBuf
));

S16 cmUnpkCtfCnmInitSyncRsp ARGS((
CtfCnmInitSyncRspMsg func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkCtfCnmCellSyncRsp ARGS((
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmCellSyncRsp * cfgRspInfo
));

S16 cmUnpkCtfCnmSyncRspMsg ARGS((
CtfCnmCellSyncRsp *param,
Buffer *mBuf
));

S16 cmUnpkCtfCnmCellSyncRsp ARGS((
CtfCnmCellSyncRspMsg func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkCtfCnmCellSyncInd ARGS(( 
Pst* pst,
SpId spId,
CtfCfgTransId *transId,
CtfCnmCellSyncInd * cfgIndInfo
));

S16 cmUnpkCtfCnmSyncIndMsg ARGS(( 
CtfCnmCellSyncInd *param,
Buffer *mBuf
));

S16 cmUnpkCtfCnmCellSyncInd ARGS((
CtfCnmCellSyncIndMsg func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkCtfcfgReqInfo ARGS((
CtfCnmInitSyncReq *param,
Buffer *mBuf
));

S16 cmUnpkCtfsyncReqInfo ARGS((
CtfCnmInitSyncReq *param,
Buffer *mBuf
));

S16 cmPkCtfcfgRspInfo ARGS((
CtfCnmInitSyncRsp *param,
Buffer *mBuf
));

S16 cmPkCtfCellSyncRsp ARGS((
CtfCnmCellSyncRsp *param,
Buffer *mBuf
));

S16 cmPkCtfcfgIndInfo ARGS((
CtfCnmCellSyncInd *param,
Buffer *mBuf
));

#ifdef __cplusplus
}
#endif
#endif /* __CTF_X__ */

/**********************************************************************
         End of file
 **********************************************************************/
