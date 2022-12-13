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
  
  Desc:     Structures, variables, and typedefs required by the LTE MAC-RRM
            Control (RGR) interface.

  File:     rgr.x 

**********************************************************************/

#ifndef __RGR_X__ 
#define __RGR_X__

#ifdef __cplusplus
extern "C" {
#endif
#define MAX_5GTF_SUBFRAME_INFO 10
/** 
  @file rgr.x 
  @brief Structure declarations and definitions for RGR interface.
  */
/** Group power formats */
typedef enum rgrGrpPwrFormat
{
   RGR_PWR_FORMAT_3, /**< Power Format 3 */
   RGR_PWR_FORMAT_3A /**< Power Format 3A */
} RgrGrpPwrFormat;

/** @name  LTE_TDD */
/** @{ */
#ifdef LTE_TDD
/** Indicates one of the two TDD ACK/NACK feedback modes */
typedef enum rgrTddAckNackMode
{
   /* rgr_x_001.main_10. Added changes of TFU_UPGRADE */
   RGR_TDD_ACKNACK_MODE_BUNDL, /**< TDD Ack/Nack Mode Bundle */
   RGR_TDD_ACKNACK_MODE_MULT   /**< TDD Ack/Nack Mode Multiplex */
} RgrTddAckNackMode;
#endif /* LTE_TDD */
/** @} */

/**  Periodicity of Downlink CQI Transmission per UE */
typedef enum rgrCqiPrdicity
{
   RGR_CQI_PRD_MS2 = 2,   /**< CQI Periodicity of 2ms */
   RGR_CQI_PRD_MS5 = 5,   /**< CQI Periodicity of 5ms */ 
   RGR_CQI_PRD_MS10 = 10, /**< CQI Periodicity of 10ms */
   RGR_CQI_PRD_MS20 = 20, /**< CQI Periodicity of 20ms */
   RGR_CQI_PRD_MS32 = 32, /**< CQI Periodicity of 32ms */
   RGR_CQI_PRD_MS40 = 40, /**< CQI Periodicity of 40ms */
   RGR_CQI_PRD_MS64 = 64, /**< CQI Periodicity of 64ms */
   RGR_CQI_PRD_MS80 = 80, /**< CQI Periodicity of 80ms */
   RGR_CQI_PRD_MS128 = 128, /**< CQI Periodicity of 128ms */
   RGR_CQI_PRD_MS160 = 160, /**< CQI Periodicity of 160ms */
   RGR_CQI_PRD_MS256 = 256, /**< CQI Periodicity of 256ms */
   RGR_CQI_PRD_MSOff /**< CQI Periodicity OFF */
} RgrCqiPrdicity;

/** 
 * Periodic CQI Transmission Modes */
typedef enum rgrPrdCqiMode
{
   RGR_PRD_CQI_MOD10,/**< Periodic CQI Mode 1-0 */
   RGR_PRD_CQI_MOD11,/**< Periodic CQI Mode 1-1 */ 
   RGR_PRD_CQI_MOD20,/**< Periodic CQI Mode 2-0 */
   RGR_PRD_CQI_MOD21 /**< Periodic CQI Mode 2-1 */
} RgrPrdCqiMode;

/** Different values for Power Aplha parameter. Divide it by 10 to get
 * the actual value of the enum that is 0, 0.4, 0.5, 0.6 etc...*/
typedef enum rgrPwrAlpha
{
   RGR_PWR_ALPHA0 = 0,   /**< Power Aplha Value 0*/
   RGR_PWR_ALPHA4 = 4,   /**< Power Aplha Value 0.4*/
   RGR_PWR_ALPHA5 = 5,   /**< Power Aplha Value 0.5*/
   RGR_PWR_ALPHA6 = 6,   /**< Power Aplha Value 0.6*/
   RGR_PWR_ALPHA7 = 7,   /**< Power Aplha Value 0.7*/
   RGR_PWR_ALPHA8 = 8,   /**< Power Aplha Value 0.8*/
   RGR_PWR_ALPHA9 = 9,   /**< Power Aplha Value 0.9*/
   RGR_PWR_ALPHAALL = 10 /**< Power Aplha Value 1*/
} RgrPwrAlpha;

/** Different Values for ACK/NACK Repetition Factor*/
typedef enum rgrAckNackRepFactor
{
   RGR_ACKNACK_REPFACT_N2 = 2,  /**< ACK/NACK Repetition Factor value 2 */
   RGR_ACKNACK_REPFACT_N4 = 4,  /**< ACK/NACK Repetition Factor value 4 */
   RGR_ACKNACK_REPFACT_N6 = 6   /**< ACK/NACK Repetition Factor value 6 */
} RgrAckNackRepFactor;

/* rgr_x_001.main_8 - Changes for MIMO feature addition */
/** @{ */
/** 
 * UE Transmission Modes state transition in case of recfg */
typedef enum rgrTxModeTrnstn
{
   RGR_TXMODE_RECFG_CMPLT = 0,
   RGR_TXMODE_RECFG_START
}RgrTxModeTrnstn;

/** @} */
/** 
 * UE Transmission Modes */
typedef enum rgrTxMode
{
   RGR_UE_TM_1 = 1,    /**< Transmission Mode 1 */
   RGR_UE_TM_2,        /**< Transmission Mode 2 */
   RGR_UE_TM_3,        /**< Transmission Mode 3 */
   RGR_UE_TM_4,        /**< Transmission Mode 4 */
   RGR_UE_TM_5,        /**< Transmission Mode 5 */
   RGR_UE_TM_6,        /**< Transmission Mode 6 */
   RGR_UE_TM_7,        /**< Transmission Mode 7 */
   RGR_UE_TM_8         /**< Transmission Mode 8 */
#ifdef LTE_ADV
   ,        
   RGR_UE_TM_9         /**< Transmission Mode 9 */
#endif
} RgrTxMode;
/** 
 * Random access system frame number */
typedef enum rgrRaSfn
{
   RGR_SFN_EVEN, /**< Even Sub Frame */
   RGR_SFN_ANY,  /**< Any Sub Frame */
/** @name LTE_TDD */
/** @{ */
#ifdef LTE_TDD 
   RGR_SFN_ODD, /**< Odd Sub Frame */
#endif /*LTE_TDD */
/** @} */
   RGR_SFN_NA /**< Sub Frame Not Available */
} RgrRaSfn;
/** 
 * Configuration period per Cell for SRS */
typedef enum rgrSrsCfgPrd
{
   RGR_SRS_CFG_PRD_1 = 1, /**< Sounding Reference signal periodicity 1ms */
   RGR_SRS_CFG_PRD_2 = 2, /**< Sounding Reference signal periodicity 2ms */
   RGR_SRS_CFG_PRD_5 = 5, /**< Sounding Reference signal periodicity 5ms */
   RGR_SRS_CFG_PRD_10 = 10, /**< Sounding Reference signal periodicity 10ms */
   RGR_SRS_CFG_PRD_INF  /**< Sounding Reference signal periodicity Infinite  */
} RgrSrsCfgPrd;

/** 
 * SRS Bandwidth Configuration per cell for SRS */
typedef enum rgrSrsBwCfg
{
   RGR_SRS_BWCFG_0 = 0, /**< Sounding Reference Signal BW CFG 0 */ 
   RGR_SRS_BWCFG_1 = 1, /**< Sounding Reference Signal BW CFG 1 */ 
   RGR_SRS_BWCFG_2 = 2, /**< Sounding Reference Signal BW CFG 2 */ 
   RGR_SRS_BWCFG_3 = 3, /**< Sounding Reference Signal BW CFG 3 */ 
   RGR_SRS_BWCFG_4 = 4, /**< Sounding Reference Signal BW CFG 4 */ 
   RGR_SRS_BWCFG_5 = 5, /**< Sounding Reference Signal BW CFG 5 */ 
   RGR_SRS_BWCFG_6 = 6, /**< Sounding Reference Signal BW CFG 6 */
   RGR_SRS_BWCFG_7 = 7  /**< Sounding Reference Signal BW CFG 7 */
} RgrSrsBwCfg;


/** 
 * Ng values for PHICH For more details refer to 36.211 Sec 6.9*/
typedef enum rgrPhichNg
{
   RGR_NG_ONESIXTH, /**< PHICH Ng Values 1/6 */
   RGR_NG_HALF,     /**< PHICH Ng Values 1/2 */
   RGR_NG_ONE,      /**< PHICH Ng Values 1 */
   RGR_NG_TWO       /**< PHICH Ng Values 2 */
} RgrPhichNg;
/** 
 * Aperiodic CQI Transmission Modes */
typedef enum rgrAprdCqiMode
{
   RGR_APRD_CQI_MOD12,  /**< Aperiodic CQI Mode 1-2 */
   RGR_APRD_CQI_MOD20,  /**< Aperiodic CQI Mode 2-0 */
   RGR_APRD_CQI_MOD22,  /**< Aperiodic CQI Mode 2-2 */
   RGR_APRD_CQI_MOD30, /**< Aperiodic CQI Mode 3-0 */
   RGR_APRD_CQI_MOD31  /**< Aperiodic CQI Mode 3-1 */
} RgrAprdCqiMode;
/*rgr_x_001.main_9 - Added support for SPS*/
/**
* Number of empty transmissions for Implicit Release */
/*rgr_x_001.main_11 MOD added comments*/
typedef enum rgrSpsImplRelCnt
{
   RGR_SPS_E2 = 2,  /**< SPS Implicit release count 2 */
   RGR_SPS_E3 = 3,  /**< SPS Implicit release count 3 */
   RGR_SPS_E4 = 4,  /**< SPS Implicit release count 4 */
   RGR_SPS_E8 = 8   /**< SPS Implicit release count 8 */
} RgrSpsImplRelCnt;

/** 
 * TODO: Check if needed this way
 * SPS Periodicty values */
/*rgr_x_001.main_11 MOD added comments*/
typedef enum rgrSpsPrd
{
   RGR_SPS_PRD_10SF = 10,  /**< SPS peridicity 10 */
   RGR_SPS_PRD_20SF = 20,  /**< SPS peridicity 20 */
   RGR_SPS_PRD_32SF = 32,  /**< SPS peridicity 32 */
   RGR_SPS_PRD_40SF = 40,  /**< SPS peridicity 40 */
   RGR_SPS_PRD_64SF = 64,  /**< SPS peridicity 64 */
   RGR_SPS_PRD_80SF = 80,  /**< SPS peridicity 80 */
   RGR_SPS_PRD_128SF = 128,  /**< SPS peridicity 128 */
   RGR_SPS_PRD_160SF = 160,  /**< SPS peridicity 160 */
   RGR_SPS_PRD_320SF = 320,  /**< SPS peridicity 320 */
   RGR_SPS_PRD_640SF = 640,  /**< SPS peridicity 640 */
   RGR_SPS_PRD_INVALID       /**< SPS peridicity invalid */  
} RgrSpsPrd;



/* rgr_x_001.main_5:ADD-Added for SI Enhancement. */

/**@name RGR_SI_SCH */
/**@{ */
#ifdef RGR_SI_SCH
/**
 * SIs Periodicity */ 
typedef enum _rgrSiPerd
{
   RGR_SI_PERD_8   = 8,    /**< SI Periodicity 8 RF */
   RGR_SI_PERD_16  = 16,   /**< SI Periodicity 16 RF */
   RGR_SI_PERD_32  = 32,   /**< SI Periodicity 32 RF */
   RGR_SI_PERD_64  = 64,   /**< SI Periodicity 64 RF */
   RGR_SI_PERD_128 = 128,  /**< SI Periodicity 128 RF */
   RGR_SI_PERD_256 = 256,  /**< SI Periodicity 256 RF */
   RGR_SI_PERD_512 = 512   /**< SI Periodicity 512 RF */
} RgrSiPeriodicity;

/*rgr_x_001.main_11 ccpu00115364 ADD changed uint8_t to enum for modPrd*/
/*modification period = (modificationPeriodCoeff * defaultPagingCycle)%m*/
/*where modificationPeriodCoeff={2,4,8,16} defaultPagingCycle={32,64,128,256}*/
/**
  @brief Modification period Periodicity */ 
typedef enum _rgrModPerd
{
   RGR_MOD_PERD_64   = 64,    /**< modification period 64 RF */
   RGR_MOD_PERD_128  = 128,   /**< modification period 128 RF */
   RGR_MOD_PERD_256  = 256,   /**< modification period 256 RF */
   RGR_MOD_PERD_512  = 512,   /**< modification period 512 RF */
   RGR_MOD_PERD_1024 = 1024  /**< modification period 1024 RF */
} RgrModPeriodicity;

/** SI Configuration Type */
typedef enum  rgrSiCfgType
{
   RGR_SI_CFG_TYPE_MIB = 0,   /**< SI CFG Type MIB */
   RGR_SI_CFG_TYPE_SIB1,      /**< SI CFG TYPE SIB1 */
   RGR_SI_CFG_TYPE_SI,         /**< SI CFG TYPE SI */
   RGR_SI_CFG_TYPE_SIB1_PWS,    /**< SI CFG TYPE SIB1 PWS */
   RGR_SI_CFG_TYPE_SIB8_CDMA,      /**< SI CFG TYPE SIB8 */
   RGR_SI_STOP
#ifdef EMTC_ENABLE
   ,
   RGR_SI_CFG_EMTC_TYPE_SIB1_BR,
   RGR_SI_CFG_EMTC_TYPE_SIB1_BR_PER,
   RGR_SI_CFG_EMTC_TYPE_SI,
   RGR_SI_CFG_EMTC_TYPE_SI_PER
#endif
} RgrSiCfgType;
#endif /*RGR_SI_SCH*/
/**@} */

/*rgr_x_001.main_11 ADD added changes for DRX*/
/**
 * @brief DRX Timer Period */ 
typedef enum _rgrDrxTmrPerd
{
   RGR_DRX_PRD_0PSF    = 0, /**< DRX timer period 0 PDCCH sf */
   RGR_DRX_PRD_1PSF    = 1, /**< DRX timer period 1 PDCCH sf */
   RGR_DRX_PRD_2PSF    = 2, /**< DRX timer period 2 PDCCH sf */
   RGR_DRX_PRD_3PSF    = 3, /**< DRX timer period 3 PDCCH sf */
   RGR_DRX_PRD_4PSF    = 4, /**< DRX timer period 4 PDCCH sf */
   RGR_DRX_PRD_5PSF    = 5, /**< DRX timer period 5 PDCCH sf */
   RGR_DRX_PRD_6PSF    = 6, /**< DRX timer period 6 PDCCH sf */
   RGR_DRX_PRD_8PSF    = 8, /**< DRX timer period 8 PDCCH sf */
   RGR_DRX_PRD_10PSF   = 10, /**< DRX timer period 10 PDCCH sf */
   RGR_DRX_PRD_16PSF   = 16, /**< DRX timer period 16 PDCCH sf */
   RGR_DRX_PRD_20PSF   = 20, /**< DRX timer period 20 PDCCH sf */
   RGR_DRX_PRD_24PSF   = 24, /**< DRX timer period 24 PDCCH sf */
   RGR_DRX_PRD_30PSF   = 30, /**< DRX timer period 30 PDCCH sf */
   RGR_DRX_PRD_33PSF   = 33, /**< DRX timer period 33 PDCCH sf */
   RGR_DRX_PRD_40PSF   = 40, /**< DRX timer period 40 PDCCH sf */
   RGR_DRX_PRD_50PSF   = 50, /**< DRX timer period 50 PDCCH sf */
   RGR_DRX_PRD_60PSF   = 60, /**< DRX timer period 60 PDCCH sf */
   RGR_DRX_PRD_64PSF   = 64, /**< DRX timer period 64 PDCCH sf */
   RGR_DRX_PRD_80PSF   = 80, /**< DRX timer period 80 PDCCH sf */
   RGR_DRX_PRD_96PSF   = 96, /**< DRX timer period 96 PDCCH sf */
   RGR_DRX_PRD_100PSF  = 100, /**< DRX timer period 100 PDCCH sf */
   RGR_DRX_PRD_112PSF  = 112, /**< DRX timer period 112 PDCCH sf */
   RGR_DRX_PRD_128PSF  = 128, /**< DRX timer period 128 PDCCH sf */
   RGR_DRX_PRD_160PSF  = 160, /**< DRX timer period 160 PDCCH sf */
   RGR_DRX_PRD_200PSF  = 200, /**< DRX timer period 200 PDCCH sf */
   RGR_DRX_PRD_300PSF  = 300, /**< DRX timer period 300 PDCCH sf */
   RGR_DRX_PRD_320PSF  = 320, /**< DRX timer period 320 PDCCH sf */
   RGR_DRX_PRD_400PSF  = 400, /**< DRX timer period 300 PDCCH sf */
   RGR_DRX_PRD_500PSF  = 500, /**< DRX timer period 500 PDCCH sf */
   RGR_DRX_PRD_600PSF  = 600, /**< DRX timer period 600 PDCCH sf */
   RGR_DRX_PRD_750PSF  = 750, /**< DRX timer period 750 PDCCH sf */
   RGR_DRX_PRD_800PSF  = 800, /**< DRX timer period 800 PDCCH sf */
   RGR_DRX_PRD_1000PSF = 1000, /**< DRX timer period 1000 PDCCH sf */
   RGR_DRX_PRD_1200PSF = 1200, /**< DRX timer period 1200 PDCCH sf */
   RGR_DRX_PRD_1280PSF = 1280, /**< DRX timer period 1280 PDCCH sf */
   RGR_DRX_PRD_1600PSF = 1600, /**< DRX timer period 1600 PDCCH sf */
   RGR_DRX_PRD_1920PSF = 1920, /**< DRX timer period 1920 PDCCH sf */
   RGR_DRX_PRD_2560PSF = 2560 /**< DRX timer period 2560 PDCCH sf */
}RgrDrxTmrPrd;


/*rgr_x_001.main_11 ADD added changes for DRX*/
/**
 * @brief DRX Cycle Period */ 
typedef enum _rgrDrxCyclePerd
{
   RGR_DRX_PRD_2SF     = 2,  /**< DRX cycle period 2 sf */ 
   RGR_DRX_PRD_5SF     = 5,  /**< DRX cycle period 5 sf */
   RGR_DRX_PRD_8SF     = 8,  /**< DRX cycle period 8 sf */
   RGR_DRX_PRD_10SF    = 10, /**< DRX cycle period 10 sf */
   RGR_DRX_PRD_16SF    = 16, /**< DRX cycle period 16 sf */
   RGR_DRX_PRD_20SF    = 20, /**< DRX cycle period 20 sf */
   RGR_DRX_PRD_32SF    = 32, /**< DRX cycle period 32 sf */
   RGR_DRX_PRD_40SF    = 40, /**< DRX cycle period 40 sf */
   RGR_DRX_PRD_64SF    = 64, /**< DRX cycle period 64 sf */
   RGR_DRX_PRD_80SF    = 80, /**< DRX cycle period 80 sf */
   RGR_DRX_PRD_128SF   = 128, /**< DRX cycle period 128 sf */
   RGR_DRX_PRD_160SF   = 160, /**< DRX cycle period 160 sf */
   RGR_DRX_PRD_256SF   = 256, /**< DRX cycle period 256 sf */
   RGR_DRX_PRD_320SF   = 320, /**< DRX cycle period 320 sf */
   RGR_DRX_PRD_512SF   = 512, /**< DRX cycle period 512 sf */
   RGR_DRX_PRD_640SF   = 640, /**< DRX cycle period 640 sf */
   RGR_DRX_PRD_1024SF  = 1024, /**< DRX cycle period 1024 sf */
   RGR_DRX_PRD_1280SF  = 1280, /**< DRX cycle period 1280 sf */
   RGR_DRX_PRD_2048SF  = 2048, /**< DRX cycle period 2048 sf */
   RGR_DRX_PRD_2560SF  = 2560 /**< DRX cycle period 2560 sf */
}RgrDrxCyclePrd;

/*rgr_x_001.main_11 ADD added changes for DRX*/
/**
 * @brief DRX Short Cycle Timer values */ 
typedef enum _rgrDrxShortCycleTmr
{
   RGR_DRX_SHRTCYCLE_MIN = 1,  /**< min DRX short cycle timer val */
   RGR_DRX_SHRTCYCLE_MAX = 16  /**< max DRX short cycle timer val */
}RgrDrxShrtCycleTmr;

/*rgr_x_001.main_11 ADD added changes for R9*/
/**@name LTEMAC_R9 */
/**@{ */
#ifdef LTEMAC_R9
/**
 * @brief DRX CQI Mask */ 
typedef enum _rgrDrxCqiMask
{
   RGR_DRX_SETUP = 0  /**< DRX CQI mask value */
}RgrDrxCqiMask;
#endif
/**@} */
      
/*rgr_x_001.main_11 ADD added changes for CQI management*/
/** 
  @brief Enumerated P_A Values */
typedef enum 
{
   RGRUE_DLPWRCNTRL_PA_DB_6 = 0,
   RGRUE_DLPWRCNTRL_PA_DB_4DOT77,
   RGRUE_DLPWRCNTRL_PA_DB_3,
   RGRUE_DLPWRCNTRL_PA_DB_1DOT77,
   RGRUE_DLPWRCNTRL_PA_DB0,
   RGRUE_DLPWRCNTRL_PA_DB1,
   RGRUE_DLPWRCNTRL_PA_DB2,
   RGRUE_DLPWRCNTRL_PA_DB3
} RgrUeDlPwrCntrlPaCfg;

/*f1b_Sprint3*/
typedef enum rgrSchFrmt1b3TypEnum
{
   RG_SCH_UCI_FORMAT1A_1B,
   RG_SCH_UCI_FORMAT1B_CS,
   RG_SCH_UCI_FORMAT3,
   RG_SCH_UCI_FORMAT_NON_CA
}RgrSchFrmt1b3TypEnum;
/*f1b_Sprint3*/

/** @brief Transaction ID between MAC and RRM */
typedef struct rgrCfgTransId
{
   uint8_t trans[RGR_CFG_TRANSID_SIZE]; /*!< RRM Transaction ID */
} RgrCfgTransId;

/** @brief  Downlink HARQ configuration per Cell */
typedef struct rgrDlHqCfg
{
   uint8_t  maxDlHqTx;            /*!< Maximum number of DL HARQ Transmissions.
                                  Minimum value is 1, maximum can be defined
                                  by the user */    
   uint8_t  maxMsg4HqTx;          /*!< Maximum msg4(Random Access) HARQ Transmissions
                                  Minimum value is 1, Maximum can be defined by
                                  the user */
} RgrDlHqCfg;

/** @brief Range of RNTIs managed by MAC */
typedef struct rgrRntiCfg
{
   CmLteRnti startRnti;      /*!< Start RNTI for the range managed by MAC */
   uint16_t       size;           /*!< Indicates contiguous range of RNTI managed by
                                  MAC */
} RgrRntiCfg;

/** @brief Downlink common channel code rate configuration per cell */
typedef struct rgrDlCmnCodeRateCfg
{
   uint16_t bcchPchRaCodeRate;    /*!< BCCH on DLSCH, PCH and RARsp coding rate.
                              * This defines the actual number of bits per 1024
                              * physical layer bits  */
   uint16_t pdcchCodeRate;        /*!< PDCCH code rate defines actual number of bits
                              * per 1024 physical layer bits. This is used to
                              * calculate aggregation level for PDCCH meant 
                              * for broadcasting RNTIs  */
   uint8_t  ccchCqi;              /*!< Default CQI to be used for Msg4 in case where 
                              * no CQI is available for the UE. ccchCqi ranges
                              * from 1 to 15.*/
} RgrDlCmnCodeRateCfg;

/** @brief Control Format Indicator (CFI) configuration per cell */
typedef struct rgrCfiCfg
{
   uint8_t cfi;                   /*!< CFI for PDCCH: a value in set {1,2,3} */
} RgrCfiCfg;

/** @brief PUSCH sub-band configuration per cell */
typedef struct rgrPuschSubBandCfg
{
  uint8_t subbandStart;           /*!< Sub-band start */
  uint8_t numSubbands;            /*!< Number of equal sized sub-bands */
  uint8_t size;                   /*!< Size of a sub-band */ 
  uint8_t dmrs[RGR_MAX_SUBBANDS]; /*!< DMRS information per sub-band */
} RgrPuschSubBandCfg;

/** @brief Uplink common channel code rate configuration per cell */
typedef struct rgrUlCmnCodeRateCfg
{
   uint8_t ccchCqi;              /*!< CCCH CQI index, also used as default
                             *  initial CQI for UEs */
} RgrUlCmnCodeRateCfg;

/* rgr_x_001.main_1: Removing unwanted srMcs Configuration structure */

/** @brief Target Uplink CQI to achieve through group power control configured per cell */
typedef struct rgrUlTrgCqiCfg
{
   uint8_t trgCqi;                /*!< Target UL CQI to be achieved through power 
                               control.Range is defined is between 1 to 15 */
} RgrUlTrgCqiCfg;
/** 
  @brief Bandwidth configuration per cell */
typedef struct rgrBwCfg
{
   uint8_t dlTotalBw;            /*!< Total Dowlink Bandwidth */
   uint8_t ulTotalBw;            /*!< Total Uplink Bandwidth  */
} RgrBwCfg;

/** 
  @brief PHICH configuration per cell */
typedef struct rgrPhichCfg
{
   RgrPhichNg ngEnum;        /*!< Ng value for PHICH */
   Bool       isDurExtend;   /*!< PHICH Duration: TRUE-extended/FALSE-normal */
} RgrPhichCfg;

/** 
  @brief PUCCH configuration per cell */
typedef struct rgrPucchCfg
{
   uint8_t  resourceSize;       /*!< PUCCH resource-size or N^(2)_RB (in RBs) */
   uint16_t n1PucchAn;          /*!< N^(1)_PUCCH */
   uint8_t  deltaShift;         /*!< Delta Shift for PUCCH: a value in set {1,2,3} */ 
   uint8_t  cyclicShift;        /*!< Cyclic Shift for PUCCH (N^(1)_CS): a value in
                             range [0-7] */ 
   uint8_t  maxPucchRb;         /*!< The max number of RBs for PUCCH. This will be
                                used to limit the max CFI value when dynamic
                                CFI feature is enabled. If there is no 
                                limitation on the max PUCCH RBs, this variable
                                should be set to 0 */                             
} RgrPucchCfg;
/** 
  @brief SRS configuration per cell */
typedef struct rgrSrsCfg
{
   /*ccpu00130768 - ADD - SRS CFG Present flag to enable/disable cell specific SRS*/
   Bool         isSrsCfgSetup;  /*!< cell specific SRS CFG enable/disable flag */
   RgrSrsCfgPrd srsCfgPrdEnum; /*!< SRS configuration period (in subframes).*/
   RgrSrsBwCfg  srsBwEnum;     /*!< SRS Bandwidth configuration per cell.
                                 Range - [0-7] */
   uint8_t           srsSubFrameCfg;/*!< SRS subframe configuration index per cell.
                                 Range - [0-15] */
} RgrSrsCfg;
/** 
  @brief RACH configuration per cell */
typedef struct rgrRachCfg
{
   uint8_t  preambleFormat;        /*!< RACH Preamble format: a value in set {0,1,2,3} */
   uint8_t  raWinSize;             /*!< RA Window size */
  /** @brief Ocassion at Which Random Access Is Expected */
   struct raOccasionS
   {
      uint8_t       size;         /*!< Number of subframe numbers */
      RgrRaSfn sfnEnum;      /*!< System Frame Number */
      uint8_t       subFrameNum[RGR_MAX_SUBFRAME_NUM]; /*!< Subframe numbers */
   } raOccasion;             /*!< Random access occasions */
   uint8_t  maxMsg3Tx;            /*!< Maximum number of message 3 transmissions */
   uint8_t  numRaPreamble;        /*!< Number of RA Preambles */
   uint8_t  sizeRaPreambleGrpA;   /*!< Size of RA Preamble in Group A */
   uint16_t msgSizeGrpA;          /*!< MESSAGE_SIZE_GROUP_A */ 
   uint8_t  prachResource;        /*!< N^RA_PRB: PRACH resource for random access */
/**@name RGR_V1 */
/**@{ */
#ifdef RGR_V1
   /* rgr_x_001.main_7: [ccpu00112372] Added contention resolution timer */
   uint8_t  contResTmr;           /*!< Contention resolution timer */
#endif
/**@} */
} RgrRachCfg;

/**
  @brief SI Configuration per cell 
*/
typedef struct rgrSiCfg
{
   uint8_t  siWinSize;  /*!< SI window size */
   uint8_t  retxCnt;    /*!< Retransmission count */
   /* rgr_x_001.main_5-ADD-Added for SI Enhancement. */
/**@name RGR_SI_SCH */
/**@{ */
#ifdef RGR_SI_SCH
/*rgr_x_001.main_11 ccpu00115364 MOD changed uint8_t to enum for modPrd*/
   RgrModPeriodicity  modPrd;     /*!< Modificiation Period for SI */  
   uint8_t  numSi;      /*!<Number of SIs, SI Id starts from 1 */
   RgrSiPeriodicity siPeriodicity[RGR_MAX_NUM_SI]; /*!<Periodicities of SIs */
   uint16_t              minPeriodicity;  /*!< Minimum Periodicity Configured */
#endif/*RGR_SI_SCH*/
/**@} */
} RgrSiCfg;

/**
 * @brief TPC RNTI Range */
typedef struct rgrTpcRntiCfg
{
   CmLteRnti startTpcRnti;   /*!< Start RNTI for TPC */
   uint16_t       size;           /*!< Indicates contiguous range of RNTI */
} RgrTpcRntiCfg;

/* rgr_x_001.main_6 : documentation update. */
/**
 * @brief Cell-specific power configuration */
typedef struct rgrUlPwrCfg
{
   S8            p0NominalPusch;    /*!< P0_NOMINAL_PUSCH Currently this is
                                         unused parameter */
   RgrPwrAlpha   alpha;             /*!< Aplha, 3-bit cell-specific parameter
                                        Currently this is unused parameter*/
   S8            p0NominalPucch;    /*!< P0_NOMINAL_PUCCH 
                                        Currently this is unused parameter */
   S8            deltaPreambleMsg3; /*!< Delta_PREAMBLE_MSG3
                                       Currently this is unused parameter */
   RgrTpcRntiCfg pucchPwrFmt3;
   RgrTpcRntiCfg pucchPwrFmt3a;
   RgrTpcRntiCfg puschPwrFmt3;
   RgrTpcRntiCfg puschPwrFmt3a;
} RgrUlPwrCfg;

/**
 * @brief Cell-specific hopping configuration */
typedef struct rgrPuschCfg
{
   uint8_t       numSubBands; /*!< Number of sub-bands 
                            Currently this is unused parameter */
   Bool     isIntraHop;  /*!< Hopping mode inter/intra subframe
                             Currently this is unused parameter */
   uint8_t       hopOffst;    /*!< Hopping offset {0 ... 98} 
                              Currently this is unused parameter*/
} RgrPuschCfg;

/**
 * @brief Number of bits in Code Book for different transmission modes */
typedef struct rgrCodeBookRstCfg
{
   Bool   pres;          /*!< Code Book restriction present ?*/
   uint32_t    pmiBitMap[2];  /*!< Array for number of Bits for ports and TX Mode*/
} RgrCodeBookRstCfg;
/**
 *  @brief Range of PDCCH Order Preamble Set managed by MAC */
typedef struct rgrPreambleSetCfg
{
   Bool    pres;      /*!< Indicates if other configuration fields are valid */
   uint8_t      start;     /*!< Start Preamble ID for the range managed by MAC */
   uint8_t      size;      /*!< Indicates contiguous range of premables managaed by
                        MAC */
} RgrPreambleSetCfg; 

/** 
  @brief Logical channel configuration information for common channels */
typedef struct rgrCmnLchCfg
{
   CmLteLcId     lcId;       /*!< Logical channel ID */
   CmLteLcType   lcType;     /*!< Identifies the Logical channel type.lcType
                               can take the following values:
                               CM_LTE_LCH_BCCH
                               CM_LTE_LCH_PCCH
                               CM_LTE_LCH_CCCH
                               CM_LTE_LCH_DCCH
                               CM_LTE_LCH_DTCH */  
   uint8_t            dir;        /*!< Indicates Direction. Direction can take following 
                                  values:
                                  RGR_DIR_TX
                                  RGR_DIR_RX
                                  RGR_DIR_TX_RX */  
   CmLteTrchType dlTrchType; /*!< Indicates type of DL transport channel:
                                   Validated only for BCCH at MAC. DL Transport
                                   channel type can take following values:
                                   CM_LTE_TRCH_BCH 
                                   CM_LTE_TRCH_PCH
                                   CM_LTE_TRCH_DL_SCH  */ 
   CmLteTrchType  ulTrchType; /*!< Indicates type of UL transport channel:
                                   Validated only for CCCH at MAC 
                                   UL Transport channel type can take following values:
                                   CM_LTE_TRCH_RACH
                                   CM_LTE_TRCH_UL_SCH */  
} RgrCmnLchCfg;

/** 
  @brief RGR configuration for DLFS scheduler */ 
typedef struct rgrDlfsCfg
{
  uint8_t isDlFreqSel;            /*!< Indicates if resource allocation is frequency
                                  selective or not */
  uint8_t thresholdCqi;           /*!< This value is used by the DL frequency 
                               selective   scheduler. This is the threshold
                               value below which the sub-band is not
                               considered for allocation for a frequency
                               selective cell. This value is utilized only if
                               DL frequency selective scheduler is configured 
                               Range is defined from 1 to 15 */
} RgrDlfsCfg;

/* LTE_ADV_FLAG_REMOVED_START */

/**
 * @brief LTE Adv feature status */
typedef enum _rgrFeature
{
   RGR_ABS  = 1 << 0, /**< ABS Present */
   RGR_SFR  = 1 << 1, /**< SFR Present */
   RGR_DSFR = 1 << 2, /**< DSFR Present */
   RGR_RE   = 1 << 3  /**< RE Present */
#ifdef LTE_ADV
   ,
   RGR_SCELLRELEASE = 1 << 4,/**< Trigger Scell Release*/
   RGR_SCELLADD     = 1 << 5,/**< Trigger Scell ADD*/
   RGR_SCELLACT     = 1 << 6,/**< Trigger Scell Activate*/
   RGR_SCELLDEACT   = 1 << 7 /**< Trigger Scell Deactivate*/
#endif
} RgrFeature;

/**
 * @brief LTE Adv feature is enable/disable */
typedef enum _rgrFeaturestatus
{
   RGR_DISABLE  = 0, /**< Feature disabled */
   RGR_ENABLE   = 1 /**< Feature Enabled */ 
} RgrFeatureStatus;

/**
 * @brief Pattern type for ABS */
typedef enum _rgrAbsPatternType
{
   RGR_ABS_MUTE     = 1 << 0, /**< Cell will Mute in configured ABS pattern */
   RGR_ABS_TRANSMIT = 1 << 1  /**< Cell will Transmit in configured ABS pattern */
} RgrAbsPatternType;

/**
  @brief RGR RB range for SFR */
typedef struct rgrSfrRbRange
{
   uint8_t   startRb;  /*<! Start RB for cell edge user */
   uint8_t   endRb;    /*<! End RB for cell edge user */
} RgrSfrRbRange;

#ifdef TFU_UPGRADE
/**
  @brief RGR Power threshold for SFR */
typedef struct rgrPwrThreshold
{
   RgrUeDlPwrCntrlPaCfg            pLow;  /*<! Power level for cell center UE */
   RgrUeDlPwrCntrlPaCfg            pHigh; /*<! Power level for cell edge UE */
} RgrPwrThreshold;
#endif

/**
  @brief RGR configuration for SFR feature */
typedef struct rgrSfrConfig
{
   RgrFeatureStatus   status; /*!< Indicate feature is enabled or disabled */
   RgrSfrRbRange      cellEdgeRbRange; /*<! Range of RBS for cell edge UEs */
#ifdef TFU_UPGRADE
   RgrPwrThreshold    pwrThreshold;    /*<! Power threshold for cell edge and cell center UE */
#endif   
} RgrSfrConfig;

/** This structure holds DSFR Config params **/
typedef struct rgrDsfrConfig
{
   RgrFeatureStatus   status; /*!< Indicate feature is enabled or disabled */
} RgrDsfrConfig;

/**
  @brief RGR configuration for LTE Advanced feature */
typedef struct rgrAbsConfig
{
   RgrFeatureStatus status; /*!< Indicate feature is enabled or disabled */            
   uint32_t  absPatternType; /*!< Indicate it as Mute and/or Transmit type */
   uint8_t   absPattern[RGR_ABS_PATTERN_LEN]; /*!< ABS pattern */
   uint32_t  absLoadPeriodicity; /*!< ABS Load Ind periodicity in msec */ 
} RgrAbsConfig;

/**
  @brief RGR configuration for LTE Advanced feature */
typedef struct rgrLteAdvancedCellConfig
{
   uint32_t                        pres;   /*!< To indicate presence of feature config */
   RgrAbsConfig               absCfg; /*!< Configuration of ABS feature */
   RgrSfrConfig               sfrCfg; /*!< Configuration of SFR feature */
   RgrDsfrConfig              dsfrCfg;/*!< Configuration of DSFR feature */
} RgrLteAdvancedCellConfig;
/* LTE_ADV_FLAG_REMOVED_END */

/** @name LTE_TDD */
/** @{ */
#ifdef LTE_TDD
/** @brief PRACH resource information for TDD */
typedef struct rgrTddPrachInfo
{
   uint8_t        freqIdx;          /*!< Frequency Index */
   RgrRaSfn  sfn;              /*!< Even/Odd/All Radio Frames */
   uint8_t        halfFrm;          /*!< First/Second Half Frame */
   uint8_t        ulStartSfIdx;     /*!< Uplink Start Subframe Index;
                                    RGR_TDD_SPL_UL_IDX must be used
                                    to configure special subframes */
} RgrTddPrachInfo;

/** @brief Set of PRACH information for TDD */
typedef struct rgrTddPrachRscInfo
{
   uint8_t               numRsc;           /*!< Number of PRACH resources*/
   RgrTddPrachInfo  prachInfo[RGR_TDD_MAX_FREQ_RSRC];     /*!< PRACH information */
} RgrTddPrachRscInfo;
#endif /* LTE_TDD */
/** @} */

/** @brief set of PF Scheduler Parameters */ 
typedef struct rgrEnbPfs
{
   uint8_t           tptCoeffi;    /*!< Downlink Throughput Coeffiecient
                                          Range 0 -10 */
   uint8_t           fairCoeffi;   /*!< Downlink Fairness Coeffiecient
                                         Range 0 -10 */
   uint32_t          qciWgt[RGR_MAX_NUM_QCI];   /*!< Downlink Qci Weights */
} RgrEnbPfs;

/** 
  @brief OPEN/CSG/HCSG Access Mode CELL 
 */
typedef enum rgrCellAccsMode
{
   RGR_CELL_ACCS_OPEN = 1,   /*!< Open Access Mode */
   RGR_CELL_ACCS_CLOSED, /*!< Closed Mode */
   RGR_CELL_ACCS_HYBRID  /*!< Hybrid Mode */
} RgrCellAccsMode;

/**
@brief Control Command Type
*/
typedef enum rgrCellCntrlCmdType
{
   RGR_CNTRL_CMD_RACH_OVRLD=1,
   RGR_CNTRL_CMD_CPU_OVRLD
}RgrCellCntrlCmdType;

/** 
  @brief Control Commands to alter CELL performance  
 */
typedef struct rgrCellCntrlCmdCfg
{
   RgrCellCntrlCmdType cmdType;
   /** @brief Cntrl Cmd Description */
   union rgrCellCmdDesc 
   {
      /** @brief RACH OverLoad Cntrl Cmd Description */
      struct rachOvrLd
      {
         Bool backOffEnb; /*!< backoff RACH during overlaod */
         uint8_t   backOffVal; /*!< backoff value during overload */
      }rachOvrLd;

      /** @brief CPU OverLoad Cntrl Cmd Description */
      struct cpuOvrLd
      {
         uint8_t instruction;  /*!< CPU Over Load Cntrl Instruction */
      }cpuOvrLd;
   }cmdDesc;
} RgrCellCntrlCmdCfg;


/**
  * @brief
  * eNB level Scheduler Configurations
  * along with other PFS config Parameters
  */
typedef struct macSchedGnbCfg
{
   uint8_t         numTxAntPorts;    /*!< Number of Tx antenna ports */
   uint8_t         ulSchdType;     /*!< Indicates which UL scheduler to use, range
                               * is 0..(number of schedulers - 1) */
   uint8_t         dlSchdType;     /*!< Indicates which DL scheduler to use, range
                               * is 0..(number of schedulers - 1) */
   uint8_t         numCells;       /*!< Max number of cells */
   uint8_t         maxUlUePerTti;  /*!< Max number of UE in UL per TTI */
   uint8_t         maxDlUePerTti;  /*!< Max number of UE in DL per TTI */
}MacSchedGnbCfg;
/**
  * @brief
  * eNB level Scheduler Configurations
  * along with other PFS config Parameters
  */
typedef struct rgrSchedEnbCfg
{
   uint8_t         numTxAntPorts;    /*!< Number of Tx antenna ports */
   uint8_t         dlSchdType;     /*!< Indicates which DL scheduler to use, range
                               * is 0..(number of schedulers - 1) */
   union       rgrDlSchInfoS
   {
      RgrEnbPfs     dlPfs;      /*!< Information related to DL PFS 
                                  Scheduler */
   } dlSchInfo;
   uint8_t         ulSchdType;     /*!< Indicates which UL scheduler to use, range
                                is 0..(number of schedulers - 1) */
   union       rgrUlSchInfoS
   {
      RgrEnbPfs     ulPfs;      /*!< This structure holds the parameters   
                                  to be configured for PFS scheduler.
                                  These values are utilized only when
                                  if PFS scheduler is being configured  
                                  There is no range defined for the  
                                  values, However the product of all
                                  three priorities must not exceed the
                                  range of uint32_t */
   } ulSchInfo;
   RgrCellAccsMode     accsMode;       /*!< Cell Access Mode */ 
#ifdef RG_5GTF
   Bool                isDynTddEnbld;  /*!< Dynamic TDD config */
#endif
} RgrSchedEnbCfg;

/*rgr_x_001.main_9 - Added support for SPS*/
/*rgr_x_001.main_11 MOD added comments for doxygen*/
/** @name LTEMAC_SPS */
/** @{ */
/** 
  @brief DL SPS configuration parameters per UE 
TODO: Check if this is to be added to re-configuration as well
 */
/* SPS_DEV */
typedef struct rgrSpsCellCfg
{
   uint8_t        maxSpsDlBw; /*!< BW used of SPS Scheduling */ 
                                                            
   uint16_t       maxSpsUePerDlSf; /*!< Maximum DL SPS UEs that can be 
                                                           scheduled in a TTI */
   uint16_t       maxSpsUePerUlSf; /*!< Maximum UL SPS UEs that can be scheduled
                                in a TTI */
} RgrSpsCellCfg;

/** 
  @brief OPEN/CSG/HCSG Access Mode Configuration parameters for CELL 
 */
typedef struct rgrCellCsgParamCfg
{
   uint8_t   minDlResNonCsg;  /*!< Min PDSCH Resources for Non-CSG Members */
   uint8_t   minUlResNonCsg;  /*!< Min PUSCH Resources for Non-CSG Members */
} RgrCellCsgParamCfg;

/** @brief LAA Cell Configuration */
typedef struct rgrLteUCfg
{
   Bool    isLaaCell;     /*<! To distinguish between LAA and LTE-U cell*/
}RgrLteUCfg;
#ifdef EMTC_ENABLE
/* emtc */
typedef struct rgrEmtcSiSchedInfo
{
   uint8_t   emtcSiNarrowBand; /*!< narrowband assigned to SI */
   uint16_t  emtcSiTbs;         /*!< tbs value for SI */
}RgrEmtcSiSchedInfo;

typedef struct rgrEmtcPrmbleMap
{
     uint8_t  firstPreamble;       /*!< for each CE mode, starting preamble */
     uint8_t  lastPreamble;        /*!< for each CE mode, starting preamble */
}RgrEmtcPrmbleMap;

typedef struct rgrEmtcRachCElevelInfoLst
{
   uint8_t                emtcRarHopping;         /*!< by defualt off */
   uint16_t               raEmtcWinSize;          /*!< RA Window size */
   uint16_t               raEmtcContResTmr;       /*!< Contension Resolution */
   uint16_t               emtcPreambleTransMax;   /*!< PreambleTransMax  */
   RgrEmtcPrmbleMap  emtcPreambleMap;           /*!< preamble mapping Info */ 
}RgrEmtcRachCElevelInfoLst;

typedef struct rgrEmtcRachCfg
{
   uint8_t emtcCeLvlSupported;
   RgrEmtcRachCElevelInfoLst ceLevelInfo[RGR_MAX_CE_LEVEL]; /* for all CE levels 0,1,2,3*/
}RgrEmtcRachCfg;

typedef struct rgrEmtcPdschCfg
{
   uint16_t emtcMaxRepCeModeA;   /*!< max repetition for CE ModeA */ 
   uint16_t emtcMaxRepCeModeB;   /*!< max repetition for CE ModeB */ 
}RgrEmtcPdschCfg;

typedef struct rgrEmtcPuschCfg
{
   uint16_t emtcMaxRepCeModeA;  /*!< max repetition for CE ModeA */ 
   uint16_t emtcMaxRepCeModeB;  /*!< max repetition for CE ModeB */
   uint8_t  emtcHoppingOffset;  /*!< Hopping offset */
}RgrEmtcPuschCfg;

typedef struct rgrEmtcPrachCEParamLst
{
   uint8_t   emtcPrachCfgIdx;         /*!< Prach config index */
   uint8_t   emtcPrachFreqOffset;     /*!< Prach Frequency Offset */
   uint16_t  emtcPrachStartSubFrame;  /*!< Starting sub frame */
   uint8_t   emtcMaxPremAttemptCE;    /*!< max preamble attempt  CE */
   uint8_t   emtcNumRepPerPreambleAtt;/*!< num of repetition per preamble attempt*/
   uint8_t   emtcNumMpdcchNBtoMonitor;/*!< num of Mpddch NB to monitor */
   uint8_t   emtcMpdcchNBtoMonitor[RGR_MAX_NUM_MPDCCH_MONITOR];/*!<  Mpddch NB to monitor */
   uint16_t  emtcMpdcchNumRep;        /*!< num of Mpddch Number of repetition */
   uint8_t   emtcPrachHoppingCfg;     /*!< num of Prach Hopping config */
}RgrEmtcPrachCEParamLst;

typedef struct rgrEmtcPrachCfg
{
   uint8_t           emtcMpdcchStartSFCssRaFdd;  /*!< mdpcch start SubFrame Ra type2 */
   uint8_t           emtcPrachHopingOffset;      /*!< prach hopping offset value */
   uint8_t           emtcInitialCElevel;         /*!< Initial CE level to start with */
   RgrEmtcPrachCEParamLst emtcPrachCEparmLst[RGR_MAX_CE_LEVEL];
}RgrEmtcPrachCfg;


/*Changes by Simran*/
typedef struct rgrFddDownlinkOrTddSubframeBitmapLC
{
   uint8_t sfnPtnChoice;
   union
   {
      uint16_t ptn10;
      uint32_t ptn40[2];
   }u;
}RgrFddDownlinkOrTddSubframeBitmapLC;


typedef struct rgrEmtcSiCfg
{
   Bool             siHoppingEnable;      /*!< SI Hopping enabled or not */
   RgrModPeriodicity  modPrd;     /*!< Modificiation Period for SI */  
   uint8_t               siWinSizeBr;          /*!< SI window size */
   uint8_t               sib1Repetition;       /*!< sib1-br repetition */
   uint8_t               siRepetition;         /*!< SI repetition pattern
                                               everyRF,every2ndRF,every4thRF,every8thRF*/
   uint16_t              startSymbolLc;
   /*Changes by Simran*/
   RgrFddDownlinkOrTddSubframeBitmapLC    fddDLOrTddSfBitmapLC;
   //uint16_t              fddDlOrTddSfBitmapBR; /*!< 10 bit value for sending SI*/
   RgrEmtcSiSchedInfo schdInfo[RGR_MAX_NUM_SI];
   uint8_t               numSi;      /*!<Number of SIs, SI Id starts from 1 */
   RgrSiPeriodicity siPeriodicity[RGR_MAX_NUM_SI]; /*!<Periodicities of SIs */
} RgrEmtcSiCfg;

typedef struct rgrEmtcPucchCfg
{
   uint16_t       emtcN1pucchAnInfoLst[RGR_MAX_CE_LEVEL];  /*!< Max CE level is 4 */
   uint8_t        emtcPucchNumRepCEMsg4Lvl0;           /*!< update the level 0 */
   uint8_t        emtcPucchNumRepCEMsg4Lvl1;           /*!< update the level 1 */
   uint8_t        emtcPucchNumRepCEMsg4Lvl2;           /*!< update the level 2 */
   uint8_t        emtcPucchNumRepCEMsg4Lvl3;           /*!< update the level 3 */
}RgrEmtcPucchCfg;

typedef struct rgrEmtcRntiCfg
{
   CmLteRnti rntiCeModeAStart;      /*!< EMTC Start RNTI for the range managed by MAC */
   CmLteRnti rntiCeModeARange;      /*!< EMTC Range of RNTI for the CEMODE A */
   uint16_t       rntiCeModeBStart;      /*!< EMTC Start RNTI for the range managed by MAC */
   CmLteRnti rntiCeModeBRange;      /*!< EMTC Range of RNTI for the CEMODE B */
   uint16_t       size;           /*!< Indicates contiguous range of RNTI managed by EMTC
                                  MAC */
} RgrEmtcRntiCfg;

typedef struct rgrEmtcCellCfg
{
   uint16_t               pci;            /*!< Physical Cell ID */
   uint32_t               emtcT300Tmr;    /*!< T300 timer as per Rel13 */
   uint32_t               emtcT301Tmr;    /*!< T301 timer as per Rel13 */
   RgrEmtcSiCfg      emtcSiCfg;      /*!< SI configuration */
   RgrEmtcRachCfg    emtcRachCfg;    /*!< Rach config as per Rel13*/
   RgrEmtcPdschCfg   emtcPdschCfg;   /*!< Pdsch config as per Rel13*/
   RgrEmtcPuschCfg   emtcPuschCfg;   /*!< Pusch config as per Rel13*/
   RgrEmtcPrachCfg   emtcPrachCfg;   /*!< Prach config as per Rel13*/
   RgrEmtcPucchCfg   emtcPucchCfg;   /*!< Pucch config as per Rel13*/
   RgrEmtcRntiCfg    emtcMacRnti;
   uint8_t                emtcPdschNbIdx;
   uint8_t                emtcMpdcchNbIdx;
   uint8_t                emtcPuschNbIdx;
}RgrEmtcCellCfg;

#endif

#ifdef RG_5GTF
typedef enum rgSchSfType
{
   RG_SCH_SF_DLCNTRL_DLDATA = 0,
   RG_SCH_SF_DLCNTRL_DLDATA_ULCNTRL,
   RG_SCH_SF_DLCNTRL_ULDATA,
   RG_SCH_SF_DLCNTRL_ULDATA_ULCNTRL
}RgSchSfType;

typedef struct rgr5gtfCellCfg
{
   RgSchSfType     dynConfig[MAX_5GTF_SUBFRAME_INFO];
   uint8_t              uePerGrp;
   uint8_t              ueGrpPerTti;
   uint8_t              numUes;
   uint8_t              numOfCC;
   uint8_t              bwPerCC;
   uint8_t              cfi;
}Rgr5gtfCellCfg;
#endif

/** @brief This enum defines dl ul transmission periodicity as per spec 38.331
 * servingCellConfigCommon */
typedef enum rgrDlUlTxPrdcty
{
   RGR_DLULTXPRDCTY_MS0DOT5    = 0,  /*!<Periodicity of 0.5 ms*/
   RGR_DLULTXPRDCTY_MS0DOT625, /*!<Periodicity of 0.625 ms*/
   RGR_DLULTXPRDCTY_MS1,       /*!<Periodicity of 1 ms */
   RGR_DLULTXPRDCTY_MS1DOT25,  /*!<Periodicity of 1.25 ms */
   RGR_DLULTXPRDCTY_MS2,       /*!<Periodicity of 2 ms */
   RGR_DLULTXPRDCTY_MS2DOT5,   /*!<Periodicity of 2.5 ms */
   RGR_DLULTXPRDCTY_MS5,       /*!<Periodicity of 5 ms */
   RGR_DLULTXPRDCTY_MS10       /*!<Periodicity of 10 ms */
}RgrDlUlTxPrdcty;

#if 0
typedef struct rgrSlotCfg
{
Bool duplexMode;  /*!< FDD:0, TDD:1 */
uint32_t  slotFrmt[RGR_MAX_SLOTS_IN_10MS]; /*!< Least significant 2 bits indicates:
                                        00-DL, 01-UL, 10-Flexi, 11-Mixed
                                        Rest 28 bits indicates 14 symbols
                                        types(DL/UL/Flexi) 2 bits for
                                        each symbol*/
RgrDlUlTxPrdcty  dlUlTxPrdcty;       /*!< dl-ul-Transmission periodicity
                                          as per 38.331 in servingCellConfigCommon*/

}RgrSlotCfg;
#endif
/** @} */
/** @brief Cell Configuration at RRM */
typedef struct rgrCellCfg
{

#if 0
   CmLteCellId         cellId;         /*!< Cell ID */
   uint16_t                 pci;            /*!< Physical Cell ID */
   uint8_t                  maxMsg3PerUlSlot; /*!< Maximum MSG3 that may be scheduled
                                            per uplink slot */
   uint8_t                  maxUePerUlSlot;   /*!<Maximum UE scheduled per UL slot */
   uint8_t                  maxUePerDlSlot;   /*!<Maximum UE scheduled per DL in a TTI */
   Bool                isCpUlExtend;   /*!< Cyclic prefix: TRUE-extended, 
                                         FALSE-normal for UL */
   Bool                isCpDlExtend;   /*!< Cyclic prefix: TRUE-extended, 
                                         FALSE-normal for DL*/
   RgrDlHqCfg          dlHqCfg;        /*!< HARQ related configuration */ 
   RgrCfiCfg           cfiCfg;         /*!< CFI for PDCCH */
   RgrUlTrgCqiCfg      trgUlCqi;       /*!< Target UL CQI */
   RgrDlCmnCodeRateCfg dlCmnCodeRate;  /*!< Coding rate for common DL channels: 
                                         Expressed in multiples of 1024 */
   RgrUlCmnCodeRateCfg ulCmnCodeRate;  /*!< Coding rate for common UL channels: 
                                         Expressed as index into CQI table */
   RgrBwCfg            bwCfg;          /*!< Bandwidth configuration */
   RgrUlPwrCfg         pwrCfg;         /*!< Cell-specific power configuration */
   RgrPuschCfg         puschCfg;       /*!< Cell-specific hopping configuration */
   RgrPreambleSetCfg   macPreambleSet; /*!< Range of PDCCH Order Preamble IDs
                                         to be managed by MAC */

   uint16_t                bcchTxPwrOffset; /*!< Tx Pwr Offset for BCCH tx on PDSCH.
                                         Offset to the reference signal 
                                         power. Value: 0 -> 10000, 
                                         representing -6 dB to 4 dB in 0.001
                                         dB steps */                                    
   uint16_t                pcchTxPwrOffset; /*!< Tx Pwr Offset for PCCH tx.
                                         Offset to the reference signal 
                                         power. Value: 0 -> 10000, 
                                         representing -6 dB to 4 dB in 0.001
                                         dB steps */                                    
   uint16_t                rarTxPwrOffset; /*!< Tx Pwr Offset for RAR tx.
                                        Offset to the reference signal 
                                        power. Value: 0 -> 10000, 
                                        representing -6 dB to 4 dB in 0.001
                                        dB steps */          
   uint8_t                   nrMu; /*!<Indicates the number of Slot for a radio frame*/
   uint8_t                   tbScalingField;/*!< TB Scaling Factor used while
                                         calucating TBS for P-RNTI, or
                                         RA-RNTI*/
   RgrSlotCfg        slotCfg;         /*!< Slot config as per 38.211, sec
                                              11.1 and
                                              38.331
                                              tdd-UL-DL-configcommon */
#if 0 //TODO: uncomment after study
   RgrDataDmrsTypAPosEnum dmrsTypAPos;      /*!< Position of (first) DL DM-RS is
                                              included in NR-PBCH payload. value is 2 or 3 */

   RgrSsPbchBlockCfg  ssPbchCfg;  /*!< SS PBCH Block configuration */
   RgrFreqRangeType  freqRangeType;       /*!<Cell Frequency Range type (<=
                                            3Ghz, 3 < x <= 6Ghz, > 6Ghz)*/
   RgrType0PdcchCSSCfg type0PdcchCSSCfg; /*!< Type 0 CSS Config params */
   RgrFreqInfoDlCfg      freqInfoDlCfg;    /*!< DL Frequency information
                                             config  */
   RgrSulCellCfg         sulCellCfg;   /*!< SUL Cell config */
   RgrUlCfgCmn         ulCmnCfg;  /*!< initial UL Bwp and Ul freq
                                    information */
#endif
   Bool                initDlBwpPres; /*!< intial common
                                        DL BWP is present or not */
   RgrBwpDlCmn      initDlBwp;    /*!<Initial Dl BWP*/
   RgrAddlBwpCfg    addlBwpCfg; /*!<Additional BWP Configuration apart from
                    Initial  BWP*/

   RgrCellNsCfgInfo   nsCfg;   /*!< NSI Cfg */

   RgrCellPfsCfgInfo   pfsCfg;   /*!< Pfs Config */
   Bool   isRateMatchSsPbchEnbld; /*!<PDSCH rate match enabled for SS PBCH */
   Bool   isRateMatchCsetEnbld; /*!<PDSCH rate match enabled */
#endif
#if 1
   CmLteCellId         cellId;         /*!< Cell ID */
   Inst                macInst;        /*!< MAC instance that is serving the cell */
   /**@name RGR_V1 */
   /**@{ */
#ifdef RGR_V1
   /* rgr_x_001.main_7: [ccpu00112789] Added configuration for maximum number
      of  MSG3s */
   uint8_t                  maxMsg3PerUlSf; /*!< Maximum MSG3 that may be scheduled
                                         per uplink subframe */
#endif /* RGR_V1 */
   /** @} */
   uint8_t                  maxUePerUlSf;   /*!< Maximum UEs that may be scheduled
                                         per uplink subframe. Currently this is
                                         unused parameter */
   uint8_t                  maxUePerDlSf;   /*!< Maximum UE to be considered for DL 
                                         scheduling in a TTI.Currently this is
                                         unused parameter */
   /*[ccpu00138609]-ADD- Max limit for Msg4/DL CCCH Ues */
   uint8_t                  maxCcchPerDlSf; /*!< Max num of Msg4/DL CCCH SDU UEs that
                                         can be scheduled per TTI. It cannot 
                                         exceed max UE per Dl sf. If set as
                                         0, this will be a don't care 
                                         parameter */ 
   uint8_t                  maxUlBwPerUe;   /*!< Maximum number of RBs that can be  
                                         allocated to an UE in an UL subframe
                                         Maximum value is defined by 
                                         RG_SCH_CMN_MAX_UL_BW_PER_UE in   
                                         rg_env.h. This can be modified as
                                         per need basis */
   uint8_t                  maxDlBwPerUe;   /*!< Maximum number of RBs that can be    
                                         allocated to an UE in an DL subframe  
                                         Maximum value is defined by     
                                         RG_SCH_CMN_MAX_DL_BW_PER_UE in
                                         rg_env.h. This can be modified as 
                                         per need basis */
   uint8_t                  maxDlRetxBw;    /*!< Maximum number of RBs that can be     
                                         allocated for retransmission in DL
                                         Maximum value is defined by 
                                         RG_SCH_CMN_MAX_DL_RETX_BW in   
                                         rg_env.h. This can be modified as   
                                         per need basis */
   uint8_t                  maxDlUeNewTxPerTti; /*!< Maximum number of UEs that can  
                                             be scheduled for a new DL    
                                             transmission in a TTI. Value should
                                             be configured by the user as per  
                                             specific needs */ 
   uint8_t                  maxUlUeNewTxPerTti;  /*!< Maximum number of UEs that can 
                                              be scheduled for a new UL 
                                              transmission in a TTI. Value should 
                                              be configured by the user as per  
                                              specific needs */
   Bool                isCpUlExtend;   /*!< Cyclic prefix: TRUE-extended, 
                                         FALSE-normal for UL */
   Bool                isCpDlExtend;   /*!< Cyclic prefix: TRUE-extended, 
                                         FALSE-normal for DL*/
   Bool                cellModSchm;    /*!< TRUE indicates 64QAM
                                         allowed while FALSE indicates 64QAM 
                                         is not allowed. Currently this is unused 
                                         parameter */
   S8                  pMax;           /*!< To limit the Cell Uplink 
                                         transmission power */

   uint8_t                  dlfsSchdType;   /*!< Indicates which DLFS scheduler to use, range
                                        * is 0..(number of schedulers - 1) */
   RgrDlHqCfg          dlHqCfg;        /*!< HARQ related configuration */ 
   RgrRntiCfg          macRnti;        /*!< Range of RNTIs to be managed by MAC */   
   RgrCfiCfg           cfiCfg;         /*!< CFI for PDCCH */
   RgrUlTrgCqiCfg      trgUlCqi;       /*!< Target UL CQI */
   RgrDlCmnCodeRateCfg dlCmnCodeRate;  /*!< Coding rate for common DL channels: 
                                         Expressed in multiples of 1024 */
   RgrPuschSubBandCfg  puschSubBand;   /*!< UL sub-band information */
   RgrUlCmnCodeRateCfg ulCmnCodeRate;  /*!< Coding rate for common UL channels: 
                                         Expressed as index into CQI table */
   RgrDlfsCfg          dlfsCfg;        /*!< Configuration for DLFS scheduler */
   RgrBwCfg            bwCfg;          /*!< Bandwidth configuration */
   RgrPhichCfg         phichCfg;       /*!< PHICH configuration information */
   RgrPucchCfg         pucchCfg;       /*!< PUCCH configuration information */
   RgrSrsCfg           srsCfg;         /*!< SRS configuration information. Currently 
                                         this is unused */ 
   RgrRachCfg          rachCfg;        /*!< RACH configuration */
   RgrSiCfg            siCfg;          /*!< SI configuration */
   RgrUlPwrCfg         pwrCfg;         /*!< Cell-specific power configuration */
   RgrPuschCfg         puschCfg;       /*!< Cell-specific hopping configuration */
   RgrPreambleSetCfg   macPreambleSet; /*!< Range of PDCCH Order Preamble IDs
                                         to be managed by MAC */
   uint8_t                  numCmnLcs;      /*!< Number of common logical channels*/
   RgrCmnLchCfg        cmnLcCfg[RGR_MAX_CMN_LC_PER_CELL];  /*!< Configuration for 
                                                             common logical channels */
   RgrCellCsgParamCfg  csgParamCfg;    /* Cell-specific configuration for CSG */
   /** @name LTE_TDD */
   /** @{ */
#ifdef LTE_TDD
   uint8_t                  ulDlCfgIdx;     /*!< UL-DL configuration index*/
   uint8_t                  spclSfCfgIdx;   /*!< Special Subframe configuration index*/
   RgrTddPrachRscInfo  prachRscInfo;   /*!< PRACH information */
#endif /* LTE_TDD */
   /** @} */
   /* rgr_x_001.main_3: Added TTI indication from MAC to RGR user */
   /* rgr_x_001.main_4: Added 0 as valid value to shut off RGR TICKs. */
   /** @name RGR_RRM_TICK */
   /** @{ */
   uint8_t                  rrmTtiIndPrd;   /*!< Periodicity of TTI indication from
                                         MAC towards RGR user. Range [0-255]. A
                                         value of 1 means one tick per System
                                         Frame and 2 means one tick per 2 System
                                         Frame, and so on.
                                         A value of 0 implies no ticks. */
   /** @} */
   /*rgr_x_001.main_9 - Added support for SPS*/
   /** @name LTEMAC_SPS */
   /** @{ */
   RgrSpsCellCfg    spsCfg;  /* Cell-specific configuration for DL SPS */
   /* LTE_ADV_FLAG_REMOVED_START */
   RgrLteAdvancedCellConfig  rgrLteAdvCfg; /*!< RGR Configuration of LTE Adv */
   /* LTE_ADV_FLAG_REMOVED_END */
#ifdef AIRSPAN
   uint8_t                dlCqiOverrideFloor;
   uint8_t                dlCqiOverrideCeil;
   uint8_t                ulCqiOverrideFloor;
   uint8_t                ulCqiOverrideCeil;
#endif  
   /** @} */
   uint16_t t300TmrVal;               /*!< t300Timer value configured in Frames */

   /* ccpu00132314-ADD-Tx power offsets for Common PDSCH transmissions */                                   
   uint16_t                bcchTxPwrOffset; /*!< Tx Pwr Offset for BCCH tx on PDSCH.
                                         Offset to the reference signal 
                                         power. Value: 0 -> 10000, 
                                         representing -6 dB to 4 dB in 0.001
                                         dB steps */                                    
   uint16_t                pcchTxPwrOffset; /*!< Tx Pwr Offset for PCCH tx.
                                         Offset to the reference signal 
                                         power. Value: 0 -> 10000, 
                                         representing -6 dB to 4 dB in 0.001
                                         dB steps */                                    
   uint16_t                rarTxPwrOffset; /*!< Tx Pwr Offset for RAR tx.
                                        Offset to the reference signal 
                                        power. Value: 0 -> 10000, 
                                        representing -6 dB to 4 dB in 0.001
                                        dB steps */          
   /* ccpu00138898 - Added Tx pwr offset for PHICH Tx*/
   uint16_t                phichTxPwrOffset; /*!< Tx Pwr Offset for PHICH tx.
                                          Offset to the reference signal 
                                          power. Value: 0 -> 10000, 
                                          representing -6 dB to 4 dB in 0.001
                                          dB steps */                                    
   Bool               isDynCfiEnb;   /*!< To indicate whether Dynamic CFI is enabled 
                                       or not */ 
   Bool               isAutoCfgModeEnb;   /*!< To indicate whether AutoCfg Mode
                                            change is enabled or not */ 
   RgrUeDlPwrCntrlPaCfg msg4pAVal;      /*!< Default value (Enum) of PA that is 
                                          used by Scheduler for msg4 */        
   RgrLteUCfg    lteUCfg;               /*!< Flag to identify LAA or LTE-U*/
#ifdef LTE_ADV
   Bool          isPucchFormat3Sptd;    /*!< Flag for Format 3 Support */
#endif
#ifdef EMTC_ENABLE
   Bool               emtcEnable;
   RgrEmtcCellCfg     emtcCellCfg;   
#endif
#ifdef RG_5GTF
   Rgr5gtfCellCfg     Cell5gtfCfg;
#endif
#endif
} RgrCellCfg;
/** 
  @brief Downlink Aperiodic CQI reporting related configuration per UE */
typedef struct rgrUeAprdDlCqiCfg
{
   Bool                pres;          /*!< Indicates presence of aperiodic 
                                           DL CQI configuration */
   RgrAprdCqiMode     aprdModeEnum;   /*!< Aperiodic CQI reporting mode */
   /* These two fields are only valid for Pcell*/
#ifdef LTE_ADV
   uint8_t                 triggerSet1;    /*!< Trigger set one*/
   uint8_t                 triggerSet2;    /*!< Trigger set two*/
#endif
} RgrUeAprdDlCqiCfg;

/* rgr_x_001.main_10. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
/** 
  @brief Downlink Periodic CQI reporting related configuration per UE */
typedef struct rgrUePrdDlCqiCfg 
{
   Bool                pres;          /*!< Indicates presence of periodic 
                                           DL CQI configuration. */
   RgrPrdCqiMode       prdModeEnum;   /*!< Peiodic CQI reporting mode. */
   RgrCqiPrdicity      prdicityEnum;  /*!< Periodicity values for CQI. 
                                           Currently, this is unused parameter. */
   uint8_t                  subframeOffst; /*!< Subframe offset. 
                                           Currently, this is unused parameter. */
   S8                  cqiOffst;      /*!< Delta^cqi_offset: (actual_value*10).
                                           Currently, this is unused parameter. */ 
   uint8_t                  k;             /*!< k value: range [1-4] */
   uint16_t                 cqiPmiCfgIdx;  /*!< CQI-PMI configuration index. */
} RgrUePrdDlCqiCfg;

#else /* TFU_UPGRADE */

/**
* @brief Periodic CQI Setup configuration parameters information 
*/
/* Reference: 36.313: CQI-ReportPeriodic */
typedef struct rgrUeDlPCqiSetup
{
      uint16_t  cqiPResIdx;  /*!< cqi-PUCCH-ResourceIndex (0.. 1185) */
      uint16_t  cqiPCfgIdx;  /*!< cqi-pmi-ConfigIndex (0..1023) */
      uint8_t   cqiRepType;  /*!< Wideband CQI = 1  Subband CQI =2 */
      uint8_t   k;           /*!< Ref: 36.213 [23, 7.2.2] (1..4). 
                             Valid only for Subband CQI */
      uint8_t   riEna;       /*!< Rand Indicator is Enabled TRUE(1) FALSE(0) */
      uint16_t  riCfgIdx;    /*!< ri-ConfigIndex    (0..1023)  */
      Bool sANCQI;      /*!< simultaneousAckNackAndCQI TRUE(1) FALSE(0) */
      RgrPrdCqiMode prdModeEnum;   /*!< Peiodic CQI reporting mode */
}RgrUeDlPCqiSetup;


/**
* @brief Periodic CQI/PMI/RI configuration parameters information 
*/
typedef struct  rgrUeDlPCqiCfg
{
   uint8_t                 type;               /*!< Setup(1) or Release(0) */    
   RgrUeDlPCqiSetup   cqiSetup;           /*!< Periodic CQI Setup */
} RgrUePrdDlCqiCfg;



/*rgr_x_001.main_11 MOD added comments*/
/** 
* @ brief Different values for UL SRS BW information 
*/
typedef enum rgrUlSrsBwInfo
{
   RGR_ULSRS_BW_0 = 0, /**< UL SRS BW info 0 */  
   RGR_ULSRS_BW_1 = 1, /**< UL SRS BW info 1 */  
   RGR_ULSRS_BW_2 = 2, /**< UL SRS BW info 2 */
   RGR_ULSRS_BW_3 = 3  /**< UL SRS BW info 3 */
} RgrUlSrsBwInfo;


/*rgr_x_001.main_11 MOD added comments*/
/** 
* @brief Different values for UL SRS Hoping BW information 
*/
typedef enum rgrUlSrsHoBwInfo
{
   RGR_ULSRS_HOP_BW_0 = 0,  /**< UL SRS Hopping BW info 0 */ 
   RGR_ULSRS_HOP_BW_1 = 1,  /**< UL SRS Hopping BW info 1 */
   RGR_ULSRS_HOP_BW_2 = 2,  /**< UL SRS Hopping BW info 2 */
   RGR_ULSRS_HOP_BW_3 = 3   /**< UL SRS Hopping BW info 3 */
} RgrUlSrsHoBwInfo;

/*rgr_x_001.main_11 MOD added comments*/
/** 
* @brief Different values for UL SRS Cyclic Shift information 
*/
typedef enum rgrUlSrsCycShiftInfo
{
   RGR_ULSRS_CYSHIFT_0 = 0,   /**< UL SRS Cyclic shift info 0 */
   RGR_ULSRS_CYSHIFT_1 = 1,   /**< UL SRS Cyclic shift info 1 */
   RGR_ULSRS_CYSHIFT_2 = 2,   /**< UL SRS Cyclic shift info 2 */
   RGR_ULSRS_CYSHIFT_3 = 3,   /**< UL SRS Cyclic shift info 3 */
   RGR_ULSRS_CYSHIFT_4 = 4,   /**< UL SRS Cyclic shift info 4 */
   RGR_ULSRS_CYSHIFT_5 = 5,   /**< UL SRS Cyclic shift info 5 */
   RGR_ULSRS_CYSHIFT_6 = 6,   /**< UL SRS Cyclic shift info 6 */
   RGR_ULSRS_CYSHIFT_7 = 7   /**< UL SRS Cyclic shift info 7 */
} RgrUlSrsCycShiftInfo;


/*rgr_x_001.main_11 MOD added comments*/
/**
* @brief SRS configuration setup parameters information. 
   Reference 36.313 SoundingRS-UL-Config
*/
typedef struct rgrUeUlSrsSetupCfg 
{
   uint16_t               srsCfgIdx;         /*!< SRS Configuration Index ISRS   
                                            Ref:  36.213: Table 8.2-1; Range: 0-636*/
   RgrUlSrsBwInfo    srsBw;        /*!< SRS Bandwidth */
   RgrUlSrsHoBwInfo  srsHopBw;     /*!< SRS Hoping Bandwidth */
   RgrUlSrsCycShiftInfo   cycShift;     /*!< Cyclic Shift */  
   Bool              duration;     /*!< Single(0) Infinite(1) */ 
/*rgr_x_001.main_11 MOD added comments for doxygen*/
   Bool              sANSrs;       /*!< Simultaneous ACK/NACK and SRS. Note:
                                     This param is specified as a UE specific
                                     parameter though 3GPP TS36.331 specifies
                                     this as a cell-specific parameter. RRM
                                     should configure this parameter with the
                                     same value for all the UEs configured for
                                     the same  cell. */
/* rgr_x_001.main_13 - DEL - Removed the redeclaration of sANSrs and added the proper comment termination  above  */
   uint8_t                txComb;       /*!< Tranmission Comb: 0..1 */
   uint8_t                fDomPosi;     /*!< Frequency Domain Position */
}RgrUeUlSrsSetupCfg;


/*rgr_x_001.main_11 MOD added comments*/
/**
 *@brief Dsr Trans maximum  
*/
typedef enum rgrUeDsrTransMax 
{
   RGR_DSR_TXMAX_4=4,  /**< Dsr Trans maximum 4 */
   RGR_DSR_TXMAX_16=16, /**< Dsr Trans maximum 16 */
   RGR_DSR_TXMAX_32=32, /**< Dsr Trans maximum 32 */
   RGR_DSR_TXMAX_64=64 /**< Dsr Trans maximum 64 */
}RgrUeDsrTransMax; 


/**
* @brief SR Setup configuration parameters information 
*/
typedef struct rgrUeSrSetupCfg
{
   uint16_t              srResIdx;       /*!< Range: 0-2047; Reference: SchedulingRequestConfig  */
   uint8_t               srCfgIdx;       /*!< Range: 0 -155; Reference: SchedulingRequestConfig */
   /*ccpu00131601:DEL - dTMax will not be required by scheduler */
}RgrUeSrSetupCfg;

/**
* @brief SR configuration parameters information 
*/
typedef struct rgrUeSrCfg
{
   Bool             type;            /*!< Release(0)/Setup(1) */
   RgrUeSrSetupCfg  srSetup;         /*!< SR Setup Configuration */
}RgrUeSrCfg;

/** @brief SRS configuration parameters information.  
  Reference 36.313 SoundingRS-UL-Config 
*/
typedef struct  rgrUeUlSrsCfg
{
   uint8_t                      type;      /*!< Release=0 Setup =1 */
   RgrUeUlSrsSetupCfg      srsSetup;  /*!< SRS Setup Configuration */

}RgrUeUlSrsCfg;

#endif /*TFU_UPGRADE */

#ifdef LTE_ADV/* Sprint 3*/
typedef struct rgrUePucchFormat3Cfg
{
   uint8_t  sCellAckN3ResAntP0Count;
   uint8_t  sCellAckN3ResAntP1Count;
   uint16_t sCellAckN3ResAntP0[4];
   uint16_t sCellAckN3ResAntP1[4];
}RgrUePucchFormat3Cfg;
typedef struct rgrUePucchFormat1BCSCfg
{
   uint8_t  sCellAckN1ResTb1Count; /* !< num of N1 res for TB1 */
   uint8_t  sCellAckN1ResTb2Count; /* !< num of N1 res for TB2 */
   uint16_t sCellAckN1ResTb1[4];   /*!< TB1 N1 resources */
   uint16_t sCellAckN1ResTb2[4];   /* !< TB2 N1 resources */
}RgrUePucchFormat1BCSCfg;
typedef struct rgrUeSCellAckPucchCfg
{
   RgrSchFrmt1b3TypEnum pucchFormatType;       /* !< 1B Channel selection or format 3*/
   union
   {
      RgrUePucchFormat1BCSCfg format1Bcs;
      RgrUePucchFormat3Cfg    format3;
   }u;
}RgrUeSCellAckPucchCfg;
#endif
/* rgr_x_001.main_10. Added changes of TFU_UPGRADE 
 This structure was earlier included under MIMO flag. But it was not part 
 of any structure. Now after TFU_UPGRADE inclusion this shall be used for 
 for PUSCH Reception Request. */
/**
* @brief PUSCH dedicated configuration parameters information.  
*/
typedef struct rgrUePuschDedCfg 
{
   Bool   pres;       /*! Prsent TRUE(1)/FALSE(0) */ 
   uint8_t     bACKIdx;    /*! betaOffset-ACK-Index (0..15) */
   uint8_t     bRIIdx;     /*! betaOffset-RI-Index (0..15) */  
   uint8_t     bCQIIdx;    /*! betaOffset-CQI-Index (0..15) */
}RgrUePuschDedCfg;
/** 
* @brief Downlink CQI reporting related configuration per UE
*/
typedef struct rgrUeDlCqiCfg
{
   RgrUeAprdDlCqiCfg aprdCqiCfg;  /*!< Aperiodic CQI-related information */
   RgrUePrdDlCqiCfg  prdCqiCfg;   /*!< Periodic CQI-related configuration */
} RgrUeDlCqiCfg;
/**
* @brief Measurement Gap configuration for UE 
*/
typedef struct rgrUeMeasGapCfg
{
   Bool isMesGapEnabled;    /*!< Is Measuremnet Gap enabled or disabled */
   uint8_t   gapPrd;             /*!< Gap period 40ms/80ms */
   uint8_t   gapOffst;           /*!< Gap offset - Vaue is 0 to 1*/
} RgrUeMeasGapCfg;
/**
 @brief DRX Long Cycle Offset */
typedef struct rgrDrxLongCycleOffst
{
   uint16_t      longDrxCycle;   /*!< DRX Long Cycle value in subframes*/
   uint16_t      drxStartOffst;  /*!< DRX Long Cycle offset value in subframes*/ 
} RgrDrxLongCycleOffst;

/**
 *  @brief DRX Short Cycle Offset */
typedef struct rgrDrxShortDrx
{
   Bool    pres;             /*!< Short cycle is configured or not */
   uint16_t     shortDrxCycle;    /*!< DRX Short Cycle value in sub-frames*/
   uint8_t      drxShortCycleTmr; /*!< Value in multiples of Short DRX Cycles*/
} RgrDrxShortDrx;

/**
 * @brief DRX configuration for UE */
typedef struct rgrUeDrxCfg
{
   Bool                  isDrxEnabled;      /*!< To indicate if DRX enabled or
                                              not, this can be used in reconfiguration
                                             to release/stop the DRX (TRUE = Enabled)*/
/*rgr_x_001.main_11 ADD added changes for R9*/
/** @name LTEMAC_R9 */
/** @{ */
#ifdef LTEMAC_R9
   TknS32                cqiMask;          /*!< To indicate if cqi-Mask is setup
                                                by higher layers. Currently supports
                                                only a enum SETUP*/
#endif
/** @} */
   uint16_t                    drxOnDurTmr;       /*!< DRX On-duration Timer value in
                                             PDCCH subframes */
   uint16_t                   drxInactvTmr;      /*!< DRX Inactivity Timer value in
                                              PDCCH subframes */
   uint16_t                    drxRetxTmr;        /*!< DRX Retransmission Timer value in PDCCH
                                              subframes */
   RgrDrxLongCycleOffst  drxLongCycleOffst; /*!< DRX Long cycle and offset, values in subframes */
   RgrDrxShortDrx        drxShortDrx;       /*!< DRX Short cycle value and offset */
#ifdef EMTC_ENABLE
   uint16_t                   emtcDrxUlRetxTmr;  /*Rel13 Drx Ul Retx Timer */
   Bool                  isEmtcUe;
   Bool                  drxOnDurTmrR13Pres;
   Bool                  drxRetxTmrR13Pres;
#endif
} RgrUeDrxCfg;

/**
 * @brief UE capability Configuration */
typedef struct rgrUeCapCfg
{
   uint8_t   pwrClass;        /*!< Power class per UE */
   Bool intraSfFeqHop;   /*!< Intra subframe frequency hopping for PUSCH */
   Bool resAloocType1;   /*!< Resource allocation type 1 for PDSCH */
   Bool simCqiAckNack;   /*!< Simultaneous CQI and ACK/NACK on PUCCH */
   Bool txAntSel;        /*!< TRUE if UE capable of doing TX Antenna selection */
/** @} */
} RgrUeCapCfg;

/**
 *  @brief UE ACK/NACK configuration */
typedef struct rgrUeAckNackRepCfg
{
   Bool                 isAckNackEnabled;  /*!< Is ACK/NACK enabled? This
                                             variable can be used in reconfiguration
                                             also to stop/release the ACK/NACK
                                             Repetition */
   uint16_t                  pucchAckNackRep;   /*!< n1PUCCH-AN-Rep */
   RgrAckNackRepFactor  ackNackRepFactor;  /*!< ACK/NACK Repetition factor */
} RgrUeAckNackRepCfg;

/** 
  @brief Transmission mode configuration per UE */
typedef struct rgrUeTxModeCfg
{
   Bool            pres;           /*!< Indicates presence of transmission mode for UE */
   RgrTxModeTrnstn tmTrnstnState;  /*!< State of Transmission Mode transition */
/* rgr_x_001.main_9 - Added support for UE Reconfiguration */
   RgrTxMode       txModeEnum;     /*!< UE transmission mode */
} RgrUeTxModeCfg;
/** 
  @brief Uplink HARQ configuration per UE */
typedef struct rgrUeUlHqCfg
{
   uint8_t maxUlHqTx;           /*!< Maximum number of UL HARQ transmissions */
   uint8_t deltaHqOffst;        /*!< Delta HARQ offset 
                                Currently this is unused parameter */
} RgrUeUlHqCfg;
/** 
  @brief Group power configuration per UE for PUCCH and PUSCH group power control */
typedef struct rgrUeGrpPwrCfg
{
   Bool            pres;            /*!< Indicates presence of UE PUCCH/PUSCH group power configuration */  
   CmLteRnti       tpcRnti;         /*!< TPC PUCCH/PUSCH RNTI for UE */
   uint8_t              idx;             /*!< Index for format 3/3A */
} RgrUeGrpPwrCfg;
/** 
  @brief Uplink power configuration per UE */
typedef struct rgrUeUlPwrCfg
{
   RgrUeGrpPwrCfg uePuschPwr;        /*!< PUSCH group power configuration per UE */
   RgrUeGrpPwrCfg uePucchPwr;        /*!< PUCCH group power configuration per UE */
   Bool           isAccumulated;     /*!< To indicate if accumulation is enabled */
   Bool           isDeltaMCSEnabled; /*!< To indicate Delta MCS Enabled */
   S8             p0UePusch;         /*!< P_0UE_PUSCH*/
   S8             p0UePucch;         /*!< P_0_PUCCH*/
   uint8_t             pSRSOffset;        /*!< P_SRS_OFFSET 
                                          Currently this is unused parameter */
   uint8_t             trgCqi;            /*!< CQI to aim for during PUSCH power
                                      *  control. Zero indicates absence, where
                                      *  cell-wide trgCqi is used */
} RgrUeUlPwrCfg;
/** 
  @brief Downlink/Uplink QoS configuration per UE */
typedef struct rgrUeQosCfg
{
   Bool ambrPres;   /*!< Indicates presence of AMBR */  
   uint32_t  dlAmbr;     /*!< DL AMBR value for UE (bytes/sec): Optional */
   uint32_t  ueBr;       /*!< UL Byte Rate value for UE (bytes/sec): Optional */
} RgrUeQosCfg;
/** 
  @brief Time Alignment timer configuration per UE */
typedef struct rgrUeTaTmrCfg
{
   Bool       pres;          /*!< rgr_x_001.main_7: Pres=NOTPRSNT indicates taTmr INFINITY */
   uint16_t        taTmr;         /*!< Timer configuration (in subframes) */
} RgrUeTaTmrCfg;
/** @name RGR_V1 */
/** @{ */
#ifdef RGR_V1
/* rgr_x_001.main_7: [ccpu00112398] Added periodicBSR-Timer and 
   retxBSR-Timer */
/** 
  @brief BSR timer configuration per UE */
typedef struct rgrUeBsrTmrCfg
{
   Bool      isPrdBsrTmrPres; /*!< Indicates if periodic BSR timer is present
                              */
   uint16_t       prdBsrTmr;       /*!< periodicBSR-Timer configuration
                                   (in subframes): Value 0xFFFF indicates
                                   'Infinity' */
   uint16_t       retxBsrTmr;      /*!< retxBSR-Timer configuration (in subframes)
                                   : Mandatory parameter */
} RgrUeBsrTmrCfg;
#endif
/** @{ */

/*rgr_x_001.main_9 - Added support for SPS*/
/** 
  @brief DL SPS configuration parameters per UE */
typedef struct rgrUeSpsDlCfg
{
   Bool        isDlSpsEnabled;   /*!< Bool indicating if DL SPS is enabled */
   uint8_t          numSpsHqProc;      /*!< Number of SPS harq Proc: Value in set
                                   [1..8] */
   uint8_t          numPucchVal;      /*!< Count for configured PUCCH values */
   uint32_t         n1PucchVal[4];    /*!< Array of n1Pucch values */
   RgrSpsPrd   dlSpsPrdctyEnum;  /*!< Periodicity for DL SPS */
   uint16_t         explicitRelCnt;   /*!< Number of SPS ocassions with BO = 0 after 
                               which SPS is released */
} RgrUeSpsDlCfg;

typedef struct rgrUlSpsLcInfo
{
   Bool  isSpsEnabled;
   uint8_t    lcId;
}RgrUlSpsLcInfo;

/** 
  @brief UL SPS configuration parameters per UE */
typedef struct rgrUeSpsUlCfg
{
   Bool              isUlSpsEnabled;   /*!< Bool indicating if UL SPS is 
                                            enabled */
   RgrSpsImplRelCnt  implicitRelCnt;   /*!< Number of SPS ocassions after which
                                            implicit release happens */
#ifdef LTE_TDD
   Bool              twoIntervalCfg;   /*!< Bool indicating if two interval
                                            config is enabled */
#endif
   Bool              pwrCfgPres;       /*!< Indicates if Power related 
                                            configuration is present */
   struct 
   {
      S8             p0NominalPuschVal;/*!< Value in range [-126...24] */ 
      S8             p0UePuschVal;     /*!< Value in range [-8....7] */
   } pwrCfg;
   RgrSpsPrd         ulSpsPrdctyEnum;  /*!< Periodicity for UL SPS */
   uint8_t                lcCnt;            /*!< Number of logical channels */
   RgrUlSpsLcInfo    spsLcInfo[RGR_MAX_SPS_LC];/*!< Array of SPS logical channels -
                                               All these are assumed to be 
                                               mapped onto SPS lcg with ID=1 */

   Bool              isLcSRMaskEnab ;  /*!< Logical Channel SR Mask Enable Flag*/


} RgrUeSpsUlCfg;

/** 
  @brief SPS configuration parameters per UE */
typedef struct rgrUeSpsCfg
{
   CmLteRnti      spsRnti;          /*!< SPS-RNTI value */ 
   RgrUeSpsDlCfg  dlSpsCfg;         /*!< DL SPS configuration information */  
   RgrUeSpsUlCfg  ulSpsCfg;         /*!< UL SPS configuration information */  
   
} RgrUeSpsCfg;
/** @brief Transmit Antenna selection types 
*/
typedef enum rgrUeTxAntSelType
{
   RGR_UE_TX_ANT_OPENLOOP,
   RGR_UE_TX_ANT_CLOSEDLOOP
} RgrUeTxAntSelType;
/**
  @brief UE Transmit Antenna selection related configuration */
typedef struct rgrUeTxAntSelCfg
{
   Bool               pres;    /*!< Configuration present */
   RgrUeTxAntSelType  selType; /*!< Transmit Antenna selection type */
} RgrUeTxAntSelCfg;
/** @} */

/*rgr_x_001.main_11 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/* DL Power control related structures */

/** 
  @brief PUSH n CQI Reporting related configuration for an UE*/
typedef struct rgrUeCqiReptCfg
{
   uint8_t   numColltdCqiRept;      /*!< Number of CQI reports to be sent in PUSH n
                                 Reporting */
}RgrUeCqiReptCfg;

/** 
  @brief CQI for subband number subBandIdx */
typedef struct rgrSubBandCqiInfo
{
   uint8_t cqi[2];     /*!< Subband CQI for two codewords */
   uint8_t subBandIdx; /*!< Index of the subband starting from 0, 
                       in ascending order of frequency */
} RgrSubBandCqiInfo;

/*rgr_x_001.main_11 ADD added changes for CQI management*/
/** 
  @brief A CQI Report used in PUSH n Reporting*/
typedef struct rgrUeCqiRept
{
   uint8_t                  cqi[2];    /*!< Wideband CQI Value for two codewords*/
   uint8_t                  cqiMode;   /*!< Reporting mode by which CQI was reported */
   RgrSubBandCqiInfo   sbCqiInfo[RGR_MAX_DL_CQI_SUBBAND];
   uint8_t                  numSubBand;/*!< Number of Subbands for which CQI is 
                                       being reported */
} RgrUeCqiRept;

/*rgr_x_001.main_11 ADD added changes for CQI management*/
/** 
  @brief Collated CQI reports */
typedef struct RgrUeCqiInfo
{
   RgrUeCqiRept cqiRept[RGR_CQIRPTS_MAXN];    /*!< CQI reports */
   uint8_t numCqiRept;       /*!< Number of CQI reports present */
} RgrUeCqiInfo;

/*rgr_x_001.main_11 ADD added changes for CQI management*/
/**
  @brief Status Indication structure passed in RgUiRgrStaInd primitive */
typedef struct rgrStaIndInfo
{
   CmLteCellId       cellId;       /*!< Cell ID */
   CmLteRnti         crnti;        /*!< UE identifier UE ID: CRNTI */
   RgrUeCqiInfo      ueCqiInfo;        /*!< CQI reports*/
}RgrStaIndInfo;
#endif

/* LTE_ADV_FLAG_REMOVED_START */
/**
  @brief LOAD INF Indication structure passed in RgUiRgrLoadInfInd primitive */
typedef struct rgrLoadInfIndInfo
{
   CmLteCellId       cellId;             /*!< Cell ID   */
   uint16_t               bw;                 /*!< Bandwidth */
   uint32_t               type;
   union
   {
      TknStrOSXL     rntpInfo;           /*!< RNTP Info */
      uint32_t            absLoadInfo[RGR_ABS_PATTERN_LEN]; 
   } u;
}RgrLoadInfIndInfo;
/* LTE_ADV_FLAG_REMOVED_END */

#ifdef TFU_UPGRADE 
/*rgr_x_001.main_11 ADD added changes for CQI management*/
/**
  @brief Struct for P_A configuration per UE */
typedef struct rgrUepACfg
{
   /* PA value in db */
   RgrUeDlPwrCntrlPaCfg   pA;   /*!< P_A Value which as used in equation pa =\
                                   delta_PowerOffset + P_A
                                   Ref: RRC 36.331, 6.3.2, PDSCH-Config*/
   Bool    pAPrsnt;            /*!< Indicates if pA has valid information */
}RgrUepACfg;

/*rgr_x_001.main_11 ADD added changes for CQI management*/
typedef struct rgrUePdschDedCfg
{
   RgrUepACfg         uepACfg;    /*!< P_A Configuration,
                                     Ref: RRC 36.331, 6.3.2, PDSCH-Config*/
}RgrUePdschDedCfg;

#endif

/* LTE_ADV_FLAG_REMOVED_START */
/* @brief UE Configuration for LTE Adv feature */
typedef struct rgrLteAdvancedUeConfig
{
   uint32_t      pres;
   Bool     isUeCellEdge;  /*! Flag to indicate UE is cell edge or cell center */
   Bool     isAbsUe;       /*! Flag to indicate ABS UE or Not */
} RgrLteAdvancedUeConfig;
/* LTE_ADV_FLAG_REMOVED_END */

typedef enum RgrAccessStratumRls
{
	RGR_REL_8,
	RGR_REL_9,
	RGR_REL_10,
	RGR_REL_11,
	RGR_REL_12,
	RGR_REL_SPARE_4,
	RGR_REL_SPARE_3,
	RGR_REL_SPARE_2,
	RGR_REL_SPARE_1,
} RgrAccessStratumRls;

#ifdef EMTC_ENABLE
#define RGR_MAX_EPDCCH_SET 2
typedef struct rgrExtaddgrp2
{
   Bool pres;
   uint8_t  csiNumRep;// MAPPING
   uint8_t  mpddchPdschHop;
   uint8_t  mpdcchStartUESSFDD;// MAPPING
   uint16_t  mpdcchNumRep;// MAPPING
   uint32_t  mpddchNB;//1.. maxAvailNarrowBands-r13
}RgrExtaddgrp2;

typedef struct rgrRbAssignment
{
 uint8_t numPRBpairs; // MAPPING
 uint8_t rbAssignment[5];
}RgrRbAssignment;
typedef  struct rgrEpdcchAddModLst
{
   uint8_t setConfigId;
   uint8_t   transmissionType;
   RgrRbAssignment   resBlkAssignment;
   uint32_t dmrsScrambSeq;
   uint32_t pucchResStartoffset;
   TknUInt32   pdschRemapQLcfgId;
   TknUInt8  mpdcchNumPRBpair; // MAPPING
   RgrExtaddgrp2 extaddgrp2;
}RgrEpdcchAddModLst;

typedef struct rgrSubFrmPatCfg
{
   Bool pres;
   uint8_t measSfPatFDD[5];
}RgrSubFrmPatCfg;
typedef struct rgrEpdcchConfigRel11
{  
   Bool pres;
   RgrSubFrmPatCfg sfPtn;
   TknUInt32 startSymbolr11;
   RgrEpdcchAddModLst  epdcchAddModLst[RGR_MAX_EPDCCH_SET];
}RgrEpdcchConfigRel11;

typedef struct rgrUeEmtcRecfg
{
   Bool isHdFddEnbld; /*!< Half Duplex FDD is configured: TRUE=1/FALSE=0 */
}RgrUeEmtcRecfg;
typedef struct rgrPucchRepCfgRel13
{
	Bool isPucchRepPres;
	uint8_t modeANumPucchRepFormat1;
	uint8_t modeANumPucchRepFormat2;

}RgrPucchRepCfgRel13;

typedef struct rgrUeEmtcCfg
{
   Bool isHdFddEnbld; /*!< Half Duplex FDD is configured: TRUE=1/FALSE=0 */
   Bool pres;
   RgrEpdcchConfigRel11 emtcEpdcchCfg;
   RgrPucchRepCfgRel13 emtcPucchRepCfg;
   uint8_t                  pdschReptLevModeA;
}RgrUeEmtcCfg;
#endif

#ifdef RG_5GTF
typedef struct rgrUe5gtfCfg
{
   uint8_t              grpId;
   uint8_t              BeamId;
   uint8_t              numCC;
   uint8_t              mcs;
   uint8_t              maxPrb;
}RgrUe5gtfCfg;
#endif


/** 
  @brief UE configuration */
typedef struct rgrUeCfg
{
   CmLteCellId        cellId;           /*!< Cell ID */
   CmLteRnti          crnti;            /*!< UE ID: CRNTI */ 
   RgrUeTxModeCfg     txMode;           /*!< UE Transmission mode: Optional */
   RgrUeDlCqiCfg      ueDlCqiCfg;       /*!< UE DL CQI configuration */
   RgrUeUlHqCfg       ueUlHqCfg;        /*!< UE-related UL HARQ configuration */
   RgrUeUlPwrCfg      ueUlPwrCfg;       /*!< UE UL power configuration: Optional */
   RgrUeQosCfg        ueQosCfg;         /*!< UE-related Dl/UL QoS configuration: AMBR */
   RgrUeTaTmrCfg      ueTaTmrCfg;       /*!< UE TA timer configuration: Optional */
/** @name RGR_V1 */
/** @{ */
#ifdef RGR_V1
   /* rgr_x_001.main_7: [ccpu00112398] Added periodicBSR-Timer and 
   retxBSR-Timer */
   RgrUeBsrTmrCfg     ueBsrTmrCfg;      /*!< UE BSR timer configuration: 
                                             Mandatory */
#endif
/** @} */
   CmLteUeCategory    ueCatEnum;        /*!< UE category */
/*rgr_x_001.main_11 ADD added changes for DRX*/
/**@{ */
   RgrUeDrxCfg        ueDrxCfg;         /*!< UE-specific DRX configuration */
/** @} */
   Bool               isTtiBundlEnabled;/*!< TtiBundling Enabled/Disabled for UE */
   RgrUeAckNackRepCfg ueAckNackCfg;     /*!< ACK/NACK configuration for UE */
   RgrUeMeasGapCfg    ueMesGapCfg;      /*!< Measurement Gap configuration for UE */
   RgrUeCapCfg        ueCapCfg;         /*!< UE Capabilty reconfiguration */
   RgrCodeBookRstCfg  ueCodeBookRstCfg; /*!< Number of bits in code book for
                                            transmission modes */
   TknUInt8              dedPreambleId;    /*!< If present, then mapping exists at
                                          RGR user with CRNTI */
/** @name LTE_TDD */
/** @{ */
#ifdef LTE_TDD
   RgrTddAckNackMode   ackNackModeEnum;  /*!< ACK/NACK Mode Bundling or
                                           Multiplexing */
#endif /* LTE_TDD */
/** @} */
   /*rgr_x_001.main_9 - Added support for SPS*/
/** @name LTEMAC_SPS */
/** @{ */
   RgrUeSpsCfg        ueSpsCfg;          /*!< SPS related configuration
                                           parameters for UE */
/** @} */
/* rgr_x_001.main_10. Added changes of TFU_UPGRADE */
/** @name TFU_UPGRADE */
/** @{ */
#ifdef TFU_UPGRADE
   /* Periodic CQI, SRS, SR  and HD-FDD Configuration  */
   RgrUeUlSrsCfg     srsCfg;       /*!< SRS  configuration information */
   RgrUeSrCfg        srCfg;        /*!< SR configuration information */
#endif 
/** @} */
/*rgr_x_001.main_11 ADD added changes for HDFDD*/
/** @name LTEMAC_HDFDD */
/** @{ */
#ifdef LTEMAC_HDFDD
   Bool              isHdFddEnbld;        /*!< Half Duplex FDD is configured: TRUE=1/FALSE=0 */
#endif /* LTEMAC_HDFDD */
/** @} */
/* rgr_x_001.main_8 - Changes for MIMO feature addition */
/* rgr_x_001.main_9 - Removed dependency on MIMO compile-time flag */

   /* rgr_x_001.main_10. Added changes of TFU_UPGRADE */
   RgrUePuschDedCfg  puschDedCfg;           /*!< PUSCH -Configuration that is dedicated. Refer 
                                             to 36.331 for more information */
   RgrUeTxAntSelCfg    ulTxAntSel;          /*!< UL Transmit antenna selection configuration */
   
/** @name RGR_CQI_REPT */
/** @{ */
/* rgr_x_001.main_11 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   RgrUeCqiReptCfg    ueCqiReptCfg;    /*!< PUSH n CQI Reporting
                                         configuration */
#endif
/** @} */
#ifdef TFU_UPGRADE 
   RgrUePdschDedCfg   uePdschDedCfg; /*!< PDSCH related dedicated configuration per UE */
#endif
   /* LTE_ADV_FLAG_REMOVED_START */
   RgrLteAdvancedUeConfig ueLteAdvCfg; /*!< LTE Adv configuration per UE */
   /* LTE_ADV_FLAG_REMOVED_END */
   RgrAccessStratumRls accessStratumRls; /*!< UE Access Stratum Release */
   uint8_t        csgMmbrSta;     /* CSG Membership status, refer RgrUeCsgMbrStatus */
#ifdef EMTC_ENABLE
   RgrUeEmtcCfg emtcUeCfg;
#endif
#ifdef RG_5GTF
   RgrUe5gtfCfg ue5gtfCfg;
#endif
} RgrUeCfg;
/** 
  @brief QCI, GBR, and MBR configuration for dedicated logical channels */
typedef struct rgrLchQosCfg
{
   uint8_t qci;                   /*!< QCI for the logical channel. 
                                  Valid Range:[0-255] (Actual QCI - 1). */
   uint32_t gbr;                  /*!< GBR value for a logical channel (bytes/sec). */
   uint32_t mbr;                  /*!< MBR value for a logical channel (bytes/sec). */
} RgrLchQosCfg;
/*rgr_x_001.main_9 - Added support for SPS*/
/**    
  @brief SPS related configuration for logical channels */
typedef struct rgrLchSpsCfg
{
   Bool isSpsEnabled;        /*!< Bool indicating if SPS is enabled for
                                  the service */
} RgrLchSpsCfg;
/** 
  @brief Logical channel configuration information for downlink logical channels */
typedef struct rgrDlLchCfg 
{
   CmLteTrchType dlTrchType; /*!< Indicates type of DL transport channel:
                                   Validated only for BCCH at MAC 
                                   DL Transport channel type can take following values:
                                   CM_LTE_TRCH_BCH 
                                   CM_LTE_TRCH_PCH
                                   CM_LTE_TRCH_DL_SCH*/
   RgrLchQosCfg   dlQos;      /*!< DL QoS parameters: Only for dedicated channels */
   /*rgr_x_001.main_9 - Added support for SPS*/
   RgrLchSpsCfg   dlSpsCfg;   /*!< SPS configuration for DL logical channel */
   uint8_t rlcReorderTmr;          /*!< RLC reordering timer required for LAA*/
} RgrDlLchCfg;

/** 
  @brief Logical channel configuration information for uplink logical channels */
typedef struct rgrUlLchCfg
{
  CmLteLcId   lcId;    /*!< LC ID for uplink logical channel*/
  uint8_t          qci;     /*!< QCI associated with LC ID */
} RgrUlLchCfg;
/** 
  @brief Logical channel group configuration information for uplink logical channels */
typedef struct rgrUlLcgCfg
{
   uint8_t             lcgId;      /*!< Logical channel group ID */
/*rgr_x_001.main_11 ADD added changes for L2 measurements*/
#ifdef LTE_L2_MEAS
   uint8_t             numLch;      /*!< Number of LC's for this group in Uplink */
   RgrUlLchCfg    lchUlCfg[RGR_MAX_LC_PER_LCG]; /*!< Logical Channel details for
                                                 this LCG*/
#endif /*LTE_L2_MEAS */
   uint32_t            gbr;      /*!< Commulative UL GBR of all LC mapping to this LCG */
   uint32_t            mbr;      /*!< Commulative UL MBR of all LC mapping to this LCG */
} RgrUlLcgCfg;

/** 
  @brief Logical channel Uplink configuration information for dedicated channels */
typedef struct rgrUlLchQciCfg
{
   CmLteLcId lcId;           /*!< Logical channel ID */
   uint8_t        qci;            /*!< Qci */
   uint8_t        lcgId;          /*!< Logical channel group ID */
}RgrUlLchQciCfg;

/** 
  @brief Logical channel configuration information for dedicated channels */
typedef struct rgrLchCfg
{
   CmLteCellId   cellId;     /*!< Cell ID */
   CmLteRnti     crnti;      /*!< CRNTI for DTCH and DCCH */
   CmLteLcId     lcId;       /*!< Logical channel ID */
   CmLteLcType   lcType;     /*!< Identifies the Logical channel type.lcType
                               can take the following values:
                                      CM_LTE_LCH_BCCH
                                      CM_LTE_LCH_PCCH
                                      CM_LTE_LCH_CCCH
                                      CM_LTE_LCH_DCCH
                                      CM_LTE_LCH_DTCH */  
   RgrDlLchCfg   dlInfo;     /*!< Downlink logical channel configuration information */
   RgrUlLchQciCfg    ulLchQciInfo;  /*!< Uplink logical channel configuration information */
   uint8_t             lcgId;      /*!< Logical channel group ID */
} RgrLchCfg;

/** @brief Set of parameters for logical channelgroup Configuration */
typedef struct rgrLcgCfg
{
   CmLteCellId   cellId;     /*!< Cell ID */
   CmLteRnti     crnti;      /*!< CRNTI for DTCH and DCCH */
   RgrUlLcgCfg   ulInfo;     /*!< Uplink logical channel configuration information */
} RgrLcgCfg;


/** @brief Basic configuration structure at RRM */
typedef struct rgrCfg
{
   uint8_t cfgType;                /*!< Indicates configuration type */
   union                      /*!< cfgType is selector */ 
   { 
      RgrCellCfg      cellCfg;   /*!< Cell configuration */
      RgrUeCfg        ueCfg;     /*!< UE configuration */ 
      RgrLchCfg       lchCfg;    /*!< Dedicated logical channel configuration */
      RgrLcgCfg       lcgCfg;    /*!< Dedicated logical channel Group configuration */
      RgrSchedEnbCfg  schedEnbCfg; /*!< EnodeB Sched Configurations */ //TODO:remove this
      MacSchedGnbCfg  schedGnbCfg; /*!< gNB Sched Configurations */
   } u;
} RgrCfg;

/** 
 * @brief Activation time information */
typedef struct rgrActvTime
{
   Bool            pres;      /*!< Indicates the presence of activation time */
   CmLteTimingInfo actvTime;  /*!< Activation time information */ 
} RgrActvTime;


/** @brief Cell reconfiguration structure at RRM */
typedef struct rgrCellRecfg
{
   CmLteCellId         cellId;        /*!< Cell ID */
   uint32_t                 recfgTypes;    /*!< Bitmask indicating reconfiguration types */
   RgrActvTime         recfgActvTime; /*!< Activation Time for cell reconfiguration */
   RgrDlHqCfg          dlHqRecfg;     /*!< DL HARQ related reconfiguration */ 
   RgrCfiCfg           cfiRecfg;      /*!< CFI reconfiguration for PDCCH */
/* rgr_x_001.main_1: Removing unwanted srMcs Configuration structure */
   RgrUlTrgCqiCfg      trgUlCqi;      /*!< Target UL CQI */
   RgrDlCmnCodeRateCfg dlCmnCodeRate; /*!< Coding rate for common DL channels: 
                                           Expressed in multiples of 1024 */
   RgrPuschSubBandCfg  puschSubBand;  /*!< UL sub-band information */
   RgrUlCmnCodeRateCfg ulCmnCodeRate; /*!< Coding rate for common UL channels: 
                                         Expressed in multiples of 1024 */
   RgrPucchCfg         pucchRecfg;    /*!< PUCCH configuration information */
   RgrSrsCfg           srsRecfg;      /*!< SRS configuration information */ 
   RgrRachCfg          rachRecfg;     /*!< RACH configuration */
   RgrDlfsCfg          dlfsRecfg;     /*!< Reconfiguration for DLFS scheduler */
   /* rgr_x_001.main_5-ADD-Added for SI Enhancement. */
/** @name RGR_SI_SCH */
/** @{ */
#ifdef RGR_SI_SCH
   RgrSiCfg            siReCfg;       /*!<SI Re-Configuration structure */
#endif
   uint16_t                 t300TmrVal;    /*!< t300Timer value configured in Frames */
   /* LTE_ADV_FLAG_REMOVED_START */
   RgrLteAdvancedCellConfig rgrLteAdvCfg; /*!< RGR Configuration of LTE Adv for a cell */
   /* LTE_ADV_FLAG_REMOVED_END */
   Bool               isDynCfiEnb;   /*!< To indicate whether Dynamic CFI is enabled 
                                          or not */ 
   Bool               isAutoCfgModeEnb;   /*!< To indicate whether AutoCfg mode is enabled 
                                          or not */ 
   RgrCellCsgParamCfg  csgParamCfg;   /*!< Cell-specific configuration for CSG */
   RgrCellCntrlCmdCfg  cntrlCmdCfg;      /*!< control cmds to alter this cell's performance */ 
/** @} */
#ifdef EMTC_ENABLE
   Bool               emtcEnable;
   RgrEmtcCellCfg     emtcCellReCfg;   
#endif
} RgrCellRecfg;

#ifdef LTE_ADV
/**
 * @brief Structure to store SCell Index for activation
 *
 * @details
 *    - sCellIdx     : sCell Index in in list of Secondary cells
 */
typedef struct rgrSCellActDeactInfo 
{
   uint8_t          sCellIdx;    /*!< sCell Index to be activated */
}RgrSCellActDeactInfo;

/**
 * @brief Structure to store details for activation/deactivation event
 *
 * @details
 *    - crnti              : Ue identifier used to get UE control block
 *    - numOfSCells        : Number of SCells to be Activated/Deactivated
 *    - sCellActDeactInfo  : Array of SCellsIdx
 */
typedef struct rgrSCellActDeactEvnt 
{
   CmLteRnti          crnti;       /*!< UE ID: To fetch UeCb in a cell*/
   uint8_t                 numOfSCells;    /* !<No of SCells to be Activated */
   RgrSCellActDeactInfo    sCellActDeactInfo[RGR_MAX_SCELL_PER_UE]; 
                      /* !<SCH SCell Activation Deactivation Information */
} RgrSCellActDeactEvnt;


/**
 * @brief Structure to store details for SCell UL PC Config
 *
 * @details
 *    - isAccumulated      : If TRUE Accumulation is ocnfigured
 *    - isDeltaMCSEnabled  : If TRUE DELTA MCS is enabled
 *    - p0UePusch          : P0 Nominal value
 *    - pSRSOffset         : SRS OFFSET value for SCELL
 */
typedef struct rgrUeUlPwrDedSCellCfg
{
   Bool           isAccumulated;     /*!< To indicate if accumulation is enabled */
   Bool           isDeltaMCSEnabled; /*!< To indicate Delta MCS Enabled */
   S8             p0UePusch;         /*!< P_0UE_PUSCH*/
   uint8_t             pSRSOffset;        /*!< P_SRS_OFFSET 
                                       Currently this is unused parameter */

} RgrUeUlPwrDedSCellCfg;


/** @brief UE Downlink secondary cell config params */
typedef struct rgrUeSecCellCfg 
{
  uint8_t                   sCellIdx;
  uint16_t                  sCellId;         /*!< This will be secondary cellId */
  TknUInt32               sCellDeActTmr;     /*!< for Activating the sCell for UE */
  RgrUeDlCqiCfg        ueSCellDlCqiCfg; /*!< Secondary cell dedicated configuration */
#ifdef TFU_UPGRADE 
  RgrUePdschDedCfg     uePdschDedCfg;   /*!< Secondary cell dedicated configuration */
#endif
  RgrUeTxModeCfg       txMode;          /*!< UE transmission mode in Secondary
                                          cell*/
  Bool                 isUlCaEnabled;  /*!<If TRUE ULCA is also enabled along with DL CA for 
                                           this SCELL */
  RgrUeUlPwrDedSCellCfg ueSCellUlDedPwrCfg; /*!< UE UL DED ULPC Cfg */
}RgrUeSecCellCfg;

/** @brief Configuration information of Secondary cells for a UE*/
typedef struct rgrUeSecCellInfo 
{
  Bool                useExtBSRSizes; /*!< If TRUE used R10 Extended BSR Size Table*/
  uint8_t                  numSCells;
  RgrUeSecCellCfg     ueSCellDedCfg[RGR_MAX_SCELL_PER_UE]; /*!< Secondary cell configuration per
UE */
}RgrUeSecCellInfo;

/** @brief Configuration of every SCell for a UE*/
typedef struct rgrUeDlSecCellRelInfo 
{
  uint8_t                   sCellIdx;  /*!< This will be secondary cell Idx */
  uint16_t                  sCellId;   /*!< This will be secondary cellId */
}RgrUeDlSecCellRelInfo;

/** @brief Configuration for SCell Release for a UE*/
typedef struct rgrUeSecCellRelInfo 
{
  uint8_t                   numSCells; /*!< This will be the number of secondary cells*/
  RgrUeDlSecCellRelInfo   ueSCellRelDedCfg[RGR_MAX_SCELL_PER_UE]; /*!< Secondary cell configuration per
UE */
}RgrUeSecCellRelInfo;
      
#endif /* LTE_ADV */
/** 
  @brief UE reconfiguration information */
typedef struct rgrUeRecfg
{
   CmLteCellId        cellId;            /*!< Cell ID */
   CmLteRnti          oldCrnti;          /*!< Old UE ID */
   CmLteRnti          newCrnti;          /*!< New UE ID: This value must match
                                         'oldCrnti', if no CRNTI change during reconfiguration */ 
   /*rgr_x_001.main_11 : changing RecfgTypes to uint32_t 
    * as all 16 bits are exhausted*/
   /* LTE_ADV_FLAG_REMOVED_START */
   /* KW fix for LTE_ADV */
   uint32_t                ueRecfgTypes;      /*!< Bitmask indicating UE reconfiguration items */
   /* LTE_ADV_FLAG_REMOVED_END */
   RgrUeTxModeCfg     txMode;            /*!< UE transmission mode */
   RgrUeAprdDlCqiCfg  aprdDlCqiRecfg;    /*!< Aperiodic CQI-related information */
#ifndef TFU_UPGRADE
   RgrUePrdDlCqiCfg   prdDlCqiRecfg;     /*!< Periodic CQI-related configuration */
#endif
   RgrUeUlHqCfg       ueUlHqRecfg;       /*!< UE UL HARQ information */
   RgrUeQosCfg        ueQosRecfg;        /*!< UE-related Dl/UL QoS configuration: AMBR */
   RgrUeTaTmrCfg      ueTaTmrRecfg;      /*!< UE TA timer information */
/** @name RGR_V1 */
/** @{ */   
#ifdef RGR_V1
   /* rgr_x_001.main_7: [ccpu00112398] Added periodicBSR-Timer and 
   retxBSR-Timer */
   RgrUeBsrTmrCfg     ueBsrTmrRecfg;     /*!< UE BSR timer reconfiguration */
#endif
/** @} */
   RgrUeUlPwrCfg      ueUlPwrRecfg;      /*!< UE UL power configuration */
/*rgr_x_001.main_11 ADD added changes for DRX*/
/**@{ */
   RgrUeDrxCfg        ueDrxRecfg;        /*!< UE-specific DRX configuration 
                                              Currently this is unused */
   /*rgr_x_001.main_12 - REM - isTtiBundlEnabled redundant field*/
/** @} */
   Bool               isTtiBundlEnabled; /*!< TTI Bundling Enabled/Disabled for UE 
                                              Currently this is unused */
   RgrUeAckNackRepCfg ueAckNackRecfg;    /*!< ACK/NACK configuration for UE */
   RgrUeCapCfg        ueCapRecfg;        /*!< UE Capabilty reconfiguration*/
   RgrUeMeasGapCfg    ueMeasGapRecfg;    /*!< Measurement Gap configuration for UE */
   RgrCodeBookRstCfg  ueCodeBookRstRecfg;/*!< Number of bits in code book for
                                             transmission modes */
/* rgr_x_001.main_9 - Added support for UE Reconfiguration */
   CmLteUeCategory     ueCatEnum;           /*!< UE category */ 
   RgrUeTxAntSelCfg    ulTxAntSel;          /*!< UL Transmit antenna selection configuration */
/** @} */
   /*rgr_x_001.main_9 - Added support for SPS*/
/** @name LTEMAC_SPS */
/** @{ */   
   RgrUeSpsCfg        ueSpsRecfg;        /*!< UE SPS reconfiguration */
/** @} */
/* rgr_x_001.main_10. Added changes of TFU_UPGRADE */
/** @name TFU_UPGRADE */
/** @{ */   
#ifdef TFU_UPGRADE

   /* Periodic CQI, SRS, SR  and HD-FDD Reconfiguration  */

   RgrUePrdDlCqiCfg  cqiCfg;       /*!< Periodic CQI PMI RI reconfiguration information */
   RgrUeUlSrsCfg     srsCfg;       /*!< SRS  reconfiguration information */
   RgrUeSrCfg        srCfg;        /*!< SR reconfiguration information */
#endif
/** @} */
/*rgr_x_001.main_11 ADD added changes for HDFDD*/
/** @name LTEMAC_HDFDD */
/** @{ */   
#ifdef LTEMAC_HDFDD
   Bool              isHdFddEnbld;        /*!< Half Duplex FDD is configured: TRUE=1/FALSE=0 */
#endif /* LTEMAC_HDFDD */
/** @} */
/* rgr_x_001.main_10. Added changes of TFU_UPGRADE */
   RgrUePuschDedCfg  puschDedCfg;   /*!< PUSCH Configuration that is dedicated.
                                    Refer to 36.331 for more information */
/** @name RGR_CQI_REPT */
/** @{ */   
/* rgr_x_001.main_11 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   RgrUeCqiReptCfg    ueCqiReptCfg;   /*!< PUSH n CQI Reporting configuration */
#endif
/** @} */
#ifdef TFU_UPGRADE 
   RgrUePdschDedCfg   uePdschDedCfg; /*!< PDSCH related dedicated configuration per UE */
#endif
   /* LTE_ADV_FLAG_REMOVED_START */
   RgrLteAdvancedUeConfig ueLteAdvCfg; /*!< LTE Adv configuration per UE */
   /* LTE_ADV_FLAG_REMOVED_END */

#ifdef LTE_ADV
   RgrUeSecCellInfo     ueSCellCfgInfo;/*!< Secondary cell dedicated informaton
                                         per UE */
   RgrUeSCellAckPucchCfg        sCellAckN1ResCfg; /*!< N1ResCfg for SCell ack feedback */
   uint8_t                 simulAckNackCQIFormat3;
#endif /* LTE_ADV */
   RgrAccessStratumRls    accessStratumRls; /*!< UE Access Stratum Release */
   uint8_t        csgMmbrSta;     /* CSG Membership status, refer RgrUeCsgMbrStatus */
#ifdef EMTC_ENABLE
   RgrUeEmtcRecfg emtcUeRecfg;
#endif
} RgrUeRecfg;
/** 
  @brief Logical channel reconfiguration information for dedicated channels only */
typedef struct rgrLchRecfg
{
   CmLteCellId cellId;       /*!< Cell ID */
   CmLteRnti   crnti;        /*!< CRNTI for DTCH and DCCH */
   CmLteLcId   lcId;         /*!< Logical channel ID */
   /*rgr_x_001.main_9 - Added support for SPS*/
   uint8_t          recfgTypes;   /*!< Reconfiguration type for DL LC */
   
/** @brief Reconfiguration Parameters during the DownLink */
struct dlRecfgS 
   {
      RgrLchQosCfg dlQos;    /*!< DL QoS parameters */
      /*rgr_x_001.main_9 - Added support for SPS*/
      RgrLchSpsCfg dlSpsRecfg; /*!< SPS re-configuration for DL logical channel */
   } dlRecfg;                /*!< Downlink logical channel reconfiguration information */
   
   RgrUlLchQciCfg ulLchQciInfo;
   uint8_t             lcgId;      /*!< Logical channel group ID */
} RgrLchRecfg;

/** @brief Set of parameters Corresponding To Logical channel group Reconfiguration */
typedef struct rgrLcgRecfg 
{
   CmLteCellId cellId;       /*!< Cell ID */
   CmLteRnti   crnti;        /*!< CRNTI for DTCH and DCCH */
   /** @brief Uplink Reconfiguration Parameters for logical channel Groups */   
   struct ullcgRecfgS
   {
      uint8_t             lcgId;     /*!< Logical channel group ID */
#ifdef RG_UNUSED
      uint8_t             numLch;      /*!< Number of LC's for this group in Uplink */
      RgrUlLchCfg    lchUlCfg[RGR_MAX_LC_PER_LCG]; /*!< Logical Channel details for
                                                        this LCG*/
#endif /*LTE_L2_MEAS */
      uint32_t            gbr;     /*!< Commulative UL GBR of all LC mapping to this LCG */
      uint32_t            mbr;     /*!< Commulative UL MBR of all LC mapping to this LCG */
   } ulRecfg;
}RgrLcgRecfg;

/** @brief Basic reconfiguration structure at RRM */
typedef struct rgrRecfg
{
   uint8_t recfgType;             /*!< Indicates reconfiguration type */
   union                     /*!< Reconfiguration type is selector */
   {
      RgrCellRecfg  cellRecfg; /*!< Cell reconfiguration */
      RgrUeRecfg    ueRecfg;    /*!< UE reconfiguration information */ 
      RgrLchRecfg   lchRecfg;   /*!< Logical channel reconfiguration information */
      RgrLcgRecfg   lcgRecfg;   /*!< Logical group reconfiguration information */
   } u;
} RgrRecfg;
/** 
  @brief Basic Delete information for MAC */
typedef struct rgrDel
{
   uint8_t delType;               /*!< Indicates configuration item to be deleted */
   /** @brief Indicates The Cell/UE/Logical Channel Group to be deleted  */ 
   union rgrDelU
   {
      /** @brief Delete The Cell ID */
      struct rgrCellDelS 
      {
         CmLteCellId cellId; /*!< Cell ID */
      } cellDel;             /*!< Cell Delete information */

      /** @brief Delete The Ue From The Cell */
      struct rgrUeDelS 
      {
         CmLteCellId cellId; /*!< Cell ID */
         CmLteRnti   crnti;  /*!< UE ID: CRNTI */
      } ueDel;               /*!< UE Delete information */
      
       /** @brief Delete The Logical Channels */
      struct rgrLchDelS
      {
         CmLteCellId cellId; /*!< Cell ID */
         CmLteRnti   crnti;  /*!< CRNTI for DTCH and DCCH */
         CmLteLcId   lcId;   /*!< Logical channel ID */
         uint8_t          lcgId;  /*!< Logical channel group ID */
      } lchDel;              /*!< Logical channel delete information */
      
      /** @brief Delete The Logical channel groups */
      struct rgrLcgDelS
      {
         CmLteCellId cellId; /*!< Cell ID */
         CmLteRnti   crnti;  /*!< CRNTI for DTCH and DCCH */
         uint8_t          lcgId;  /*!< Logical channel group ID */
      } lcgDel;
#ifdef LTE_ADV
      /** @brief Delete Secondary cell for a UE */
      struct rgrUeScellRel
      {
         CmLteCellId          cellId;           /*!< Cell ID */
         CmLteRnti            crnti;            /*!< UE ID: CRNTI */
         uint32_t                  ueDelTypes;       /*!< Bitmask indicating UE reconfiguration items */
         RgrUeSecCellRelInfo  ueSCellRelCfgInfo;/*!< Secondary cell dedicated informaton*/
      } ueScellRel;
#endif   
   } u;                      /*!< Union of Cell/UE/Lch delete information */
} RgrDel;
/** 
  @brief UE RESET info for MAC */
typedef struct rgrRst
{
   CmLteCellId cellId; /*!< Cell ID */
   CmLteRnti   crnti;  /*!< UE ID: CRNTI tobe RESET */
} RgrRst;

typedef enum rgrSonCfgType
{
   RGR_SON_PRB_CFG
}RgrSonCfgType;

/* Pa enum -6, -4.77, -3, -1.77, 0, 1, 2, 3 dB and -INF */
typedef enum rgrPaVal
{
   RGR_PA_DB_NEG_6,
   RGR_PA_DB_NEG_4_77,
   RGR_PA_DB_NEG_3,
   RGR_PA_DB_NEG_1_77,
   RGR_PA_DB_0,
   RGR_PA_DB_1,
   RGR_PA_DB_2,
   RGR_PA_DB_3,
   RGR_PA_NEGTIVE_INF
}RgrPaVal;

/* Pa level enum High, Normal, Low */
typedef enum rgrPaLevel
{
   RGR_PA_LVL_NORMAL,
   RGR_PA_LVL_LOW,
   RGR_PA_LVL_HIGH
}RgrPaLevel;


/** 
  @brief PRB configuration received from SON for MAC */

typedef struct rgrPrbCfg
{
   uint8_t                isSonIcicEnable;
   uint8_t                numCellEdgeUEs;
   uint8_t                numCellCentreUEs;
   CmLteRnti         cellEdgeUe[RG_SCH_MAX_UE];      /*!< List of cell Edge UE's */
   CmLteRnti         cellCentreUe[RG_SCH_MAX_UE];   /*!< List of cell centre UE's */
   RgrPaLevel        paLevelPerPrb[RGR_SCH_MAX_PA_PER_PRB]; /*!< Mapping of PRB to Pa Values */
}RgrPrbCfg; 

/** 
  @brief configuration received from SON for MAC */
typedef struct rgrSonCfg
{
   RgrSonCfgType   cfgType;
   union
   {
      RgrPrbCfg   prbCfg;
   }u; 
}RgrSonCfg;

 /** @brief Basic RGR configuration/reconfiguration info at RRM */
typedef struct rgrCfgReqInfo
{

   uint8_t  action;               /*!< Determines configuration/reconfiguration */
   union                     /*!< Action is selector */
   {
      RgrCfg   cfgInfo;      /*!< Configuration information at RRM */
      RgrRecfg recfgInfo;    /*!< Reconfiguration information at RRM */
      RgrDel   delInfo;      /*!< Deletion related information */
      RgrRst   rstInfo;      /*!< UE information to be REST */
      RgrSonCfg   sonCfg;    /*!< PA and PRB configuration received from SON*/
#ifdef LTE_ADV
      RgrSCellActDeactEvnt  sCellActDeactEvnt;  /*!< SCell Activation Information */
#endif /* LTE_ADV */
   } u;
} RgrCfgReqInfo;

/* rgr_x_001.main_3: Added TTI indication from MAC to RGR user */
/** @name RGR_RRM_TICK */
/** @{ */
/** @brief This structure contains information that is passed as part of the TTI
 * indication sent from PHY to MAC.
 */
typedef struct rgrTtiIndInfo
{
   CmLteCellId       cellId;       /*!< Cell ID */
   uint16_t               hSfn;          /*!< Hyper System Frame Number */
   uint16_t               sfn;          /*!< System Frame Number */
} RgrTtiIndInfo;
/** @} */

#define RGR_UESTA_MAC_CRNTI_CE_RECVD   0x01
#define RGR_UESTA_MAC_CRNTI_CE_RECVD_IN_SPS_ACTIVE   0x02
/**
  @brief Status Indication structure passed in RgUiRgrUeStaInd primitive */
typedef struct rgrUeStaIndInfo
{
   CmLteCellId       cellId;       /*!< Cell ID */
   CmLteRnti         crnti;        /*!< UE identifier UE ID: CRNTI */
   uint8_t                status;       /*!< Status */
}RgrUeStaIndInfo;
/** @} */

/* 
   Function Prototypes 
 */

/** @brief Request from RRM to MAC to bind the interface SAPs .
 *
 * @details This Primitive is used to bind The SAPs between RRM and the MAC.
 * The API validates the Sap Ids , contents of the pst Structure .
 *
 * @param[in] pst  Pointer To the Post Structure.
 * @param[in] suId SAP Id of the Service User.
 * @param[in] spId SAP Id of the Service Provider
 * @return ROK/RFAILED
 */

typedef S16 (*RgrBndReq) ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId));

/* rgr_x_001.main_3: Added TTI indication from MAC to RGR user */
/** @name RGR_RRM_TICK */
/** @{ */

/** @brief Indication from MAC to a RGR User about the Transmit Time Interval. 
 *
 * @details This Primitive is used to indicate RRM after every TTI.
 * API validates the  post Structure and TTI Timing .
 *
 * @param[in] pst    Pointer To the Post Structure.
 * @param[in] suId   SAP Id of the Service User.
 * @param[in] ttiInd Pointer To a structure containing additional Information that is passed 
 * as a part of TTI Indication. 
 * @return ROK/RFAILED
 */

typedef S16 (*RgrTtiInd) ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrTtiIndInfo        *ttiInd));
/** @} */

/** @brief Confirmation from MAC to RRM for the bind request for the interface SAPs.
 *
 * @details This Primitive is used To confirm the binding between the MAC and The RRM.
 *
 * @param[in] pst    Pointer To the Post Structure.
 * @param[in] suId   SAP Id of the Service User.
 * @param[in] status Binding Status.
 * @return ROK/RFAILED
 */
typedef S16 (*RgrBndCfm) ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status));

/** @brief Request from RRM to MAC to unbind the interface SAPs.
 *
 * @details This Primitive is used to unbind MAC and The RRM.It validates on the SAP If its already Bound 
 * and unbinds the SAP.
 *
 * @param[in] pst    Pointer to the Post Structure.
 * @param[in] spId   SAP Id of the Service Provider.
 * @param[in] reason Cause for unbinding.
 * @return ROK/RFAILED
 */
typedef S16 (*RgrUbndReq) ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason));

/** @brief Configuration request from RRM to MAC for configuring Cell/UE/LC. 
 *
 * @details This API is used to configure a scheduler for a UE/Cell/Logical Channels 
 *
 * @param[in] pst        Pointer to the Post Structure.
 * @param[in] spId       SAP Id of the Service Provider.
 * @param[in] transId    Reason for unbinding.
 * @param[in] cfgReqInfo A Pointer to Configuration/Reconfiguration Structure. 
 * @return ROK/RFAILED
 */
typedef S16 (*RgrCfgReq) ARGS((
   Pst*                 pst,
   RgrCfgTransId        transId,
   RgrCfgReqInfo *      cfgReqInfo));

S16 cmPkRgrCfgCfm ARGS((
   Pst*                 pst,
   RgrCfgTransId        transId,
   uint8_t                   status
));

/** @brief Configuration confirm from MAC to RRM. 
 * 
 * @details This API confirms the RGR User about the status of the Configuration.
 * 
 * @param[in] pst      Pointer to the Post Structure.
 * @param[in] spId     SAP Id of the Service User.
 * @param[in] transId  Transaction Id for the transaction between RRM and MAC
 * @param[in] status   Configuration confirmation status Information.
 * @return ROK/RFAILED
 */
typedef S16 (*RgrCfgCfm) ARGS((
   Pst*                 pst,
   RgrCfgTransId        transId,
   uint8_t                   status));
/* rgr_x_001.main_5-ADD-Added for SI Enhancement. */

/* rgr_x_001.main_11 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
typedef S16 (*RgrStaInd) ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrStaIndInfo        *staInd));
#endif

/* LTE_ADV_FLAG_REMOVED_START */
typedef S16 (*RgrLoadInfInd) ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrLoadInfIndInfo    *loadInfInd));
/* LTE_ADV_FLAG_REMOVED_END */

typedef S16 (*RgrUeStaInd) ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrUeStaIndInfo        *staInd));
#ifdef RG

/* rgr_x_001.main_3: Added TTI indication from MAC to RGR user */
/** @name RGR_RRM_TICK */
/** @{ */
/** @brief TTI Indication from Scheduler to RRM.  
* 
*  @details TTI Indication from MAC to RGR User.
*
* @param[in] pst    Pointer to a post structure.
* @param[in] suId   SAP Id of the Service User.
* @param[in] ttiInd Parameters containing the information on TTI Indication.
* @return ROK/RFAILED
*/
S16 RgUiRgrTtiInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrTtiIndInfo        *ttiInd
));
#endif
/** @} */
/*rgr_x_001.main_9 - Added support for SPS*/
S16 cmPkRgrSpsCellCfg ARGS((
   RgrSpsCellCfg *param,
   Buffer *mBuf
));
S16 cmUnpkRgrSpsDlCellCfg ARGS((
   RgrSpsCellCfg *param,
   Buffer *mBuf
));
S16 cmPkRgrUeSpsDlCfg ARGS((
   RgrUeSpsDlCfg *param,
   Buffer *mBuf
));
S16 cmUnpkRgrUeSpsDlCfg ARGS((
   RgrUeSpsDlCfg *param,
   Buffer *mBuf
));
S16 cmPkRgrUeSpsUlCfg ARGS((
   RgrUeSpsUlCfg *param,
   Buffer *mBuf
));
S16 cmUnpkRgrUeSpsUlCfg ARGS((
   RgrUeSpsUlCfg *param,
   Buffer *mBuf
));
S16 cmPkRgrUeSpsCfg ARGS((
   RgrUeSpsCfg *param,
   Buffer *mBuf
));
S16 cmUnpkRgrUeSpsCfg ARGS((
   RgrUeSpsCfg *param,
   Buffer *mBuf
));
S16 cmPkRgrLchSpsCfg ARGS((
   RgrLchSpsCfg *param,
   Buffer *mBuf
));
S16 cmUnpkRgrLchSpsCfg ARGS((
   RgrLchSpsCfg *param,
   Buffer *mBuf
));

/*rgr_x_001.main_11 ADD added changes for L2 measurements*/
S16 cmPkRgrUlLchCfg ARGS((
   RgrUlLchCfg *param,
   Buffer *mBuf
));
S16 cmUnpkRgrUlLchCfg ARGS((
  RgrUlLchCfg *param,
  Buffer *mBuf
));
S16 cmPkRgrUlLchQciCfg ARGS((
   RgrUlLchQciCfg *param,
   Buffer *mBuf
));
S16 cmUnpkRgrUlLchQciCfg ARGS((
  RgrUlLchQciCfg *param,
  Buffer *mBuf
));


/*rgr_x_001.main_11 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
S16 cmPkRgrUeCqiReptCfg ARGS((
RgrUeCqiReptCfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeCqiReptCfg ARGS((
RgrUeCqiReptCfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrSubBandCqiInfo ARGS((
RgrSubBandCqiInfo *param,
Buffer *mBuf
));

S16 cmPkRgrStaInd ARGS((
Pst* pst,
SuId suId,
RgrStaIndInfo* staInd
));

S16 cmUnpkRgrStaInd ARGS((
RgrStaInd func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkRgrStaIndInfo ARGS((
RgrStaIndInfo *param,
Buffer *mBuf
));

S16 cmUnpkRgrStaIndInfo ARGS((
RgrStaIndInfo *param,
Buffer *mBuf
));

S16 cmPkRgrUeCqiInfo ARGS((
RgrUeCqiInfo *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeCqiInfo ARGS((
RgrUeCqiInfo *param,
Buffer *mBuf
));

S16 cmPkRgrUeCqiRept ARGS((
RgrUeCqiRept *param,
Buffer *mBuf
));

S16 cmPkRgrSubBandCqiInfo ARGS((
RgrSubBandCqiInfo *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeCqiRept ARGS((
RgrUeCqiRept *param,
Buffer *mBuf
));
#endif

/* LTE_ADV_FLAG_REMOVED_START */
S16 cmPkRgrLoadInfInd ARGS((
Pst* pst,
SuId suId,
RgrLoadInfIndInfo* loadInfInd
));

S16 cmUnpkRgrLoadInfInd ARGS((
RgrLoadInfInd func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkRgrLoadInfIndInfo ARGS((
RgrLoadInfIndInfo *param,
Buffer *mBuf
));

S16 cmUnpkRgrLoadInfIndInfo ARGS((
RgrLoadInfIndInfo *param,
Pst *pst, /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
Buffer *mBuf
));
/* LTE_ADV_FLAG_REMOVED_END */

#ifdef TFU_UPGRADE 
S16 cmPkRgrUePdschDedCfg ARGS((
RgrUePdschDedCfg  *param,
Buffer *mBuf
));

S16 cmUnpkRgrUePdschDedCfg ARGS((
RgrUePdschDedCfg *param,
Buffer *mBuf
));

S16 cmPkRgrUepACfg ARGS((
RgrUepACfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrUepACfg ARGS((
RgrUepACfg *param,
Buffer *mBuf
));
#endif

#ifdef LTE_ADV
S16 cmPkRgrUeSecCellInfo ARGS((
RgrUeSecCellInfo *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeSecCellInfo ARGS((
RgrUeSecCellInfo *param,
Buffer *mBuf
));

S16 cmPkRgrUeDlSecCellRelInfo ARGS((
RgrUeDlSecCellRelInfo *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeDlSecCellRelInfo ARGS((
RgrUeDlSecCellRelInfo *param,
Buffer *mBuf
));

S16 cmPkRgrUeSecCellRelInfo ARGS((
RgrUeSecCellRelInfo *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeSecCellRelInfo ARGS((
RgrUeSecCellRelInfo *param,
Buffer *mBuf
));

/* Sprint 3 */
S16 cmPkRgrUeSCellAckPucchCfg ARGS((
RgrUeSCellAckPucchCfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeSCellAckPucchCfg ARGS((
RgrUeSCellAckPucchCfg *param,
Buffer *mBuf
));
#endif /* LTE_ADV */
#ifdef RG

/** @brief Configuration Confirm from MAC to RRM.  
 *
 * @details In this API crnti, preambleId, and
 * maskId are returned to RRM if request does not contain crnti (For Handover purpose)
 *
 *  @param[in] pst     A pointer to post Structure.
 *  @param[in] transId MAC to RRM User transaction Id. 
 *  @param[in] status  Status indication from the MAC.  
 *  @return ROK/RFAILED
 */
S16 RgUiRgrCfgCfm ARGS((
   Pst*                 pst,
   RgrCfgTransId        transId,
   uint8_t                   status
));

/** @name RGR_CQI_REPT */
/** @{ */
/* rgr_x_001.main_11 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/** @brief Sta Indication from Scheduler to RRM 
 *
 * @details This primitive is used to send status indication from scheduler
 *          to RRM. 
 * 
 * @param[in] pst     Pointer to a post structure
 * @param[in] suId    Service User SAP Id
 * @param[in] staInd  Status Indication .
 * @return ROK/RFAILED
 */
S16 RgUiRgrStaInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrStaIndInfo        *staInd
));
#endif
S16 RgUiRgrUeStaInd ARGS((
Pst             *pst,
SuId            suId,
RgrUeStaIndInfo *ueStaInd
));


/** @} */
/** @{ */
/* LTE_ADV_FLAG_REMOVED_START */
/** @brief LoadInf Indication from Scheduler to RRM
 *
 * @details This primitive is used to send LOAD INF indication from scheduler
 *          to RRM.
 *
 * @param[in] pst         Pointer to a post structure
 * @param[in] suId        Service User SAP Id
 * @param[in] loadInfInd  LOAD INF Indication .
 * @return ROK/RFAILED
 */
S16 RgUiRgrLoadInfInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrLoadInfIndInfo    *loadInfInd
));
/* LTE_ADV_FLAG_REMOVED_END */
/** @} */

#endif

#ifdef NX
/** @brief Request from RRM to MAC to bind the interface SAPs.
 *
 *  @details This API validats the SAP Id and binds the interface SAPs  
 *
 *  @param[in] pst    Pointer to a post structure
 *  @param[in] suId   Service User SAP Id
 *  @param[in] spId   Service Provider SAP Id
 *  @return ROK/RFAILED  
 */
S16 NxLiRgrBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));

/* rgr_x_001.main_3: Added TTI indication from MAC to RGR user */
/** @name RGR_RRM_TICK */
/** @{ */
/** @brief TTI indication from scheduler to RRM. 
*
* @details This primitive handles TTI Indication. It essentially validates
* the post structure and Transmit Time timing .
*
* @param[in] Pst*     pst          Pointer To a post Structure
* @param[in] SuId     suId         Service user SAP Id
* @param[in] RgrTtiIndInfo* ttiInd Information passed as a part of TTI indication from PHY to MAC. 
* @return ROK/RFAILED 
*/
S16 NxLiRgrTtiInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrTtiIndInfo        *ttiInd
));
/** @} */

/** @brief Confirmation from MAC to RRM for the bind request for the interface SAPs.
 *
 * @details This Primitive handles the call for bind confirmation.
 * 
 * @param[in] Pst*  pst    A pointer to post structure.
 * @param[in] SuId  suId   Service User SAP Id.
 * @param[in] uint8_t    status An information on status confirmation.
 * @return S16
 */
S16 NxLiRgrBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));

/** @brief Request from RRM to MAC to unbind the interface SAPs 
 *
 * @details This primitive unbinds the interface SAPs.It validates if the SAP is really bound.
 * 
 * @param[in] Pst*      pst    A pointer to post structure. 
 * @param[in] SpId      spId   Service Provider SAP Id.
 * @param[in] Reason    reason A cause for unbinding the SAPs.
 * @return S16
 */
S16 NxLiRgrUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/** @brief Configuration request from RRM to MAC for 
 * configuring Cell/UE/LC 
 *
 * @details The RRM configures the Cell/UE/LC using this primitive.
 *
 * @param[in]   Pst*  pst      A pointer to post structure.
 * @param[in]   SpId  spId     Service Provider SAP Id.
 * @param[in]   RgrCfgTransId  transId Transaction Id between RRM and MAC. 
 * @param[in]   RgrCfgReqInfo* cfgReqInfo A structure containing the configuration information.
 * @return   S16 
 */
S16 NxLiRgrCfgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   RgrCfgTransId        transId,
   RgrCfgReqInfo *      cfgReqInfo
));

/** @brief Configuration confirm from MAC to RRM 
 *
 * @details This primitive confirms the RRM about the configuration reception request.
 * 
 * @param[in] Pst*      pst    A pointer to post structure. 
 * @param[in] SuId      suId   Service Provider SAP Id.
 * @param[in] RgrCfgTransId transId RRM to MAC transaction Id. 
 * @return S16
 */
S16 NxLiRgrCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrCfgTransId        transId,
   uint8_t                   status
));

/** @name RGR_CQI_REPT */
/** @{ */
/* rgr_x_001.main_11 ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/** @brief Sta Indication from Scheduler to RRM 
 *
 * @details This primitive is used to send status indication
 *          from scheduler to RRM. 
 * 
 * @param[in] pst     Pointer to a post structure
 * @param[in] suId    Service User SAP Id
 * @param[in] staInd  Status Indication .
*/
S16 NxLiRgrStaInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrStaIndInfo        *staInd
));
#endif
/** @} */
/** @{ */
/* LTE_ADV_FLAG_REMOVED_START */
/** @brief LOAD INF Indication from Scheduler to RRM
 *
 * @details This primitive is used to send loadInf indication
 *          from scheduler to RRM.
 *
 * @param[in] pst         Pointer to a post structure
 * @param[in] suId        Service User SAP Id
 * @param[in] loadInfInd  LOAD INF Indication .
*/
S16 NxLiRgrLoadInfInd ARGS((
   Pst*                  pst,
   SuId                  suId,
   RgrLoadInfIndInfo     *loadInfInd
));
/* LTE_ADV_FLAG_REMOVED_END */
/** @} */

#endif
#if defined(LCRGR)
/* rgr_x_001.main_3: Added TTI indication from MAC to RGR user */
/** @name RGR_RRM_TICK */
/** @{ */
/** @brief Pack function for TTI indication from scheduler to RRM */
S16 cmPkRgrTtiInd ARGS((
         Pst*                 pst,
         SuId                 suId,
         RgrTtiIndInfo        *ttiInd
         ));
S16 cmPkRgrTtiIndInfo ARGS((
         RgrTtiIndInfo  *ttiInd,
         Buffer         *mBuf));
/** @brief Unpack function for TTI indication from scheduler to RRM */
S16 cmUnpkRgrTtiInd   ARGS((
         RgrTtiInd            func,
         Pst*                 pst,
         Buffer               *mBuf
         ));
S16 cmUnpkRgrTtiIndInfo ARGS((
         RgrTtiIndInfo *param,
         Buffer        *mBuf
         ));
/** @} */
S16 cmPkRgrCfgReq ARGS((
   Pst*                 pst,
   RgrCfgTransId        transId,
   RgrCfgReqInfo *      cfgReqInfo
));

S16 cmUnpkRgrCfgReq ARGS((
   RgrCfgReq            func,
   Pst*                 pst,
   Buffer               *mBuf
));

S16 cmUnpkRgrCfgCfm ARGS((
   RgrCfgCfm            func,
   Pst*                 pst,
   Buffer               *mBuf
));

S16 cmPkRgrCfgTransId ARGS((
   RgrCfgTransId        *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCfgTransId ARGS((
   RgrCfgTransId        *param,
   Buffer               *mBuf
));
S16 cmPkRgrDlHqCfg ARGS((
   RgrDlHqCfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrDlHqCfg ARGS((
   RgrDlHqCfg           *param,
   Buffer               *mBuf
));
S16 cmPkRgrRntiCfg ARGS((
   RgrRntiCfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrRntiCfg ARGS((
   RgrRntiCfg           *param,
   Buffer               *mBuf
));
S16 cmPkRgrDlCmnCodeRateCfg ARGS((
   RgrDlCmnCodeRateCfg  *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrDlCmnCodeRateCfg ARGS((
   RgrDlCmnCodeRateCfg  *param,
   Buffer               *mBuf
));
S16 cmPkRgrCfiCfg ARGS((
   RgrCfiCfg            *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCfiCfg ARGS((
   RgrCfiCfg            *param,
   Buffer               *mBuf
));
S16 cmPkRgrPuschSubBandCfg ARGS((
   RgrPuschSubBandCfg   *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrPuschSubBandCfg ARGS((
   RgrPuschSubBandCfg   *param,
   Buffer               *mBuf
));
S16 cmPkRgrUlCmnCodeRateCfg ARGS((
   RgrUlCmnCodeRateCfg  *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUlCmnCodeRateCfg ARGS((
   RgrUlCmnCodeRateCfg  *param,
   Buffer               *mBuf
));
S16 cmPkRgrUlTrgCqiCfg ARGS((
   RgrUlTrgCqiCfg       *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUlTrgCqiCfg ARGS((
   RgrUlTrgCqiCfg       *param,
   Buffer               *mBuf
));
S16 cmPkRgrBwCfg ARGS((
   RgrBwCfg             *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrBwCfg ARGS((
   RgrBwCfg             *param,
   Buffer               *mBuf
));
S16 cmPkRgrPhichCfg ARGS((
   RgrPhichCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrPhichCfg ARGS((
   RgrPhichCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrPucchCfg ARGS((
   RgrPucchCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrPucchCfg ARGS((
   RgrPucchCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrSrsCfg ARGS((
   RgrSrsCfg            *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrSrsCfg ARGS((
   RgrSrsCfg            *param,
   Buffer               *mBuf
));
S16 cmPkRgrRachCfg ARGS((
   RgrRachCfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrRachCfg ARGS((
   RgrRachCfg           *param,
   Buffer               *mBuf
));
S16 cmPkRgrSiCfg ARGS((
   RgrSiCfg             *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrSiCfg ARGS((
   RgrSiCfg             *param,
   Buffer               *mBuf
));
S16 cmPkRgrTpcRntiCfg ARGS((
   RgrTpcRntiCfg        *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrTpcRntiCfg ARGS((
   RgrTpcRntiCfg        *param,
   Buffer               *mBuf
));
S16 cmPkRgrUlPwrCfg ARGS((
   RgrUlPwrCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUlPwrCfg ARGS((
   RgrUlPwrCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrPuschCfg ARGS((
   RgrPuschCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrPuschCfg ARGS((
   RgrPuschCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrCodeBookRstCfg ARGS((
   RgrCodeBookRstCfg    *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCodeBookRstCfg ARGS((
   RgrCodeBookRstCfg    *param,
   Buffer               *mBuf
));
S16 cmPkRgrPreambleSetCfg ARGS((
   RgrPreambleSetCfg    *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrPreambleSetCfg ARGS((
   RgrPreambleSetCfg    *param,
   Buffer               *mBuf
));
S16 cmPkRgrCmnLchCfg ARGS((
   RgrCmnLchCfg         *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCmnLchCfg ARGS((
   RgrCmnLchCfg         *param,
   Buffer               *mBuf
));
S16 cmPkRgrDlfsCfg ARGS((
   RgrDlfsCfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrDlfsCfg ARGS((
   RgrDlfsCfg           *param,
   Buffer               *mBuf
));


/** @name LTE_TDD */
/** @{ */
#ifdef LTE_TDD
S16 cmPkRgrTddPrachInfo ARGS((
   RgrTddPrachInfo      *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrTddPrachInfo ARGS((
   RgrTddPrachInfo      *param,
   Buffer               *mBuf
));
S16 cmPkRgrTddPrachRscInfo ARGS((
   RgrTddPrachRscInfo   *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrTddPrachRscInfo ARGS((
   RgrTddPrachRscInfo   *param,
   Buffer               *mBuf
));
#endif /* LTE_TDD*/
/** @} */
S16 cmPkRgrEnbPfs ARGS((
   RgrEnbPfs         *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrEnbPfs ARGS((
   RgrEnbPfs         *param,
   Buffer               *mBuf
));
S16 cmPkRgrCellCfg ARGS((
   RgrCellCfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCellCfg ARGS((
   RgrCellCfg           *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeAprdDlCqiCfg ARGS((
   RgrUeAprdDlCqiCfg    *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeAprdDlCqiCfg ARGS((
   RgrUeAprdDlCqiCfg    *param,
   Buffer               *mBuf
));
S16 cmPkRgrSchedGnbCfg ARGS((
   RgrSchedEnbCfg       *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrSchedGnbCfg ARGS((
   RgrSchedEnbCfg       *param,
   Buffer               *mBuf
));
S16 cmPkRgrUePrdDlCqiCfg ARGS((
   RgrUePrdDlCqiCfg     *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUePrdDlCqiCfg ARGS((
   RgrUePrdDlCqiCfg     *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeDlCqiCfg ARGS((
   RgrUeDlCqiCfg        *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeDlCqiCfg ARGS((
   RgrUeDlCqiCfg        *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeMeasGapCfg ARGS((
   RgrUeMeasGapCfg      *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeMeasGapCfg ARGS((
   RgrUeMeasGapCfg      *param,
   Buffer               *mBuf
));
/*rgr_x_001.main_11 ADD added changes for DRX*/
S16 cmPkRgrDrxLongCycleOffst ARGS((
   RgrDrxLongCycleOffst *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrDrxLongCycleOffst ARGS((
   RgrDrxLongCycleOffst *param,
   Buffer               *mBuf
));
S16 cmPkRgrDrxShortDrx ARGS((
   RgrDrxShortDrx       *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrDrxShortDrx ARGS((
   RgrDrxShortDrx       *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeDrxCfg ARGS((
   RgrUeDrxCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeDrxCfg ARGS((
   RgrUeDrxCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeCapCfg ARGS((
   RgrUeCapCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeCapCfg ARGS((
   RgrUeCapCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeAckNackRepCfg ARGS((
   RgrUeAckNackRepCfg   *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeAckNackRepCfg ARGS((
   RgrUeAckNackRepCfg   *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeTxModeCfg ARGS((
   RgrUeTxModeCfg       *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeTxModeCfg ARGS((
   RgrUeTxModeCfg       *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeUlHqCfg ARGS((
   RgrUeUlHqCfg         *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeUlHqCfg ARGS((
   RgrUeUlHqCfg         *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeGrpPwrCfg ARGS((
   RgrUeGrpPwrCfg       *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeGrpPwrCfg ARGS((
   RgrUeGrpPwrCfg       *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeUlPwrCfg ARGS((
   RgrUeUlPwrCfg        *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeUlPwrCfg ARGS((
   RgrUeUlPwrCfg        *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeQosCfg ARGS((
   RgrUeQosCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeQosCfg ARGS((
   RgrUeQosCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeTaTmrCfg ARGS((
   RgrUeTaTmrCfg        *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeTaTmrCfg ARGS((
   RgrUeTaTmrCfg        *param,
   Buffer               *mBuf
));
/** @name RGR_V1 */
/** @{ */
#ifdef RGR_V1
/* rgr_x_001.main_7: [ccpu00112398] Added periodicBSR-Timer and 
   retxBSR-Timer */
S16 cmPkRgrUeBsrTmrCfg ARGS((
   RgrUeBsrTmrCfg       *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeBsrTmrCfg ARGS((
   RgrUeBsrTmrCfg       *param,
   Buffer               *mBuf
));
#endif
/** @} */
S16 cmPkRgrUeCfg ARGS((
   RgrUeCfg             *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeCfg ARGS((
   RgrUeCfg             *param,
   Buffer               *mBuf
));
S16 cmPkRgrLchQosCfg ARGS((
   RgrLchQosCfg         *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrLchQosCfg ARGS((
   RgrLchQosCfg         *param,
   Buffer               *mBuf
));
S16 cmPkRgrDlLchCfg ARGS((
   RgrDlLchCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrDlLchCfg ARGS((
   RgrDlLchCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrUlLcgCfg ARGS((
   RgrUlLcgCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUlLcgCfg ARGS((
   RgrUlLcgCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrLchCfg ARGS((
   RgrLchCfg            *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrLchCfg ARGS((
   RgrLchCfg            *param,
   Buffer               *mBuf
));
S16 cmPkRgrLcgCfg ARGS((
   RgrLcgCfg            *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrLcgCfg ARGS((
   RgrLcgCfg            *param,
   Buffer               *mBuf
));
S16 cmPkRgrCfg ARGS((
   RgrCfg               *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCfg ARGS((
   RgrCfg               *param,
   Buffer               *mBuf
));
S16 cmPkRgrActvTime ARGS((
   RgrActvTime          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrActvTime ARGS((
   RgrActvTime          *param,
   Buffer               *mBuf
));
S16 cmPkRgrCellRecfg ARGS((
   RgrCellRecfg         *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCellRecfg ARGS((
   RgrCellRecfg         *param,
   Buffer               *mBuf
));
S16 cmPkRgrUeRecfg ARGS((
   RgrUeRecfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeRecfg ARGS((
   RgrUeRecfg           *param,
   Buffer               *mBuf
));
S16 cmPkRgrLchRecfg ARGS((
   RgrLchRecfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrLchRecfg ARGS((
   RgrLchRecfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrLcgRecfg ARGS((
   RgrLcgRecfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrLcgRecfg ARGS((
   RgrLcgRecfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgrRecfg ARGS((
   RgrRecfg             *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrRecfg ARGS((
   RgrRecfg             *param,
   Buffer               *mBuf
));
S16 cmPkRgrDel ARGS((
   RgrDel               *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrDel ARGS((
   RgrDel               *param,
   Buffer               *mBuf
));
S16 cmPkRgrRst ARGS((
   RgrRst               *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrRst ARGS((
   RgrRst               *param,
   Buffer               *mBuf
));

S16 cmPkRgrSonCfg   ARGS((
RgrSonCfg *param,
Buffer *mBuf
));
S16 cmUnpkRgrSonCfg   ARGS((
RgrSonCfg   *param,
Buffer      *mBuf
));
S16 cmPkRgrSonPrbCfg   ARGS((
RgrPrbCfg *param,
Buffer *mBuf
));
S16 cmUnpkRgrSonPrbCfg   ARGS((
RgrPrbCfg   *param,
Buffer      *mBuf
));
S16 cmPkRgrCfgReqInfo ARGS((
   RgrCfgReqInfo        *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrCfgReqInfo ARGS((
   RgrCfgReqInfo        *param,
   Buffer               *mBuf
));

#ifdef LTE_ADV 
S16 cmUnPkRgrSCellActDeactEvnt ARGS((
   RgrSCellActDeactEvnt *param,
   Buffer *mBuf
));

S16 cmPkRgrSCellActDeactEvnt ARGS((
   RgrSCellActDeactEvnt *param,
   Buffer *mBuf
));
#endif /* LTE_ADV */

S16 cmPkRgrUeTxAntSelCfg ARGS((
   RgrUeTxAntSelCfg     *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUeTxAntSelCfg ARGS((
   RgrUeTxAntSelCfg     *param,
   Buffer               *mBuf
));
S16 cmPkRgrUePuschDedCfg ARGS((
   RgrUePuschDedCfg     *param,
   Buffer               *mBuf
));
S16 cmUnpkRgrUePuschDedCfg ARGS((
   RgrUePuschDedCfg     *param,
   Buffer               *mBuf
));

#ifdef TFU_UPGRADE
S16 cmPkRgrUeDlPCqiSetup ARGS
((
RgrUeDlPCqiSetup *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeDlPCqiSetup ARGS
((
RgrUeDlPCqiSetup *param,
Buffer *mBuf
));

S16 cmPkRgrUeUlSrsSetupCfg ARGS
((
RgrUeUlSrsSetupCfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeUlSrsSetupCfg ARGS
((
RgrUeUlSrsSetupCfg *param,
Buffer *mBuf
));

S16 cmPkRgrUeSrSetupCfg ARGS
((
RgrUeSrSetupCfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeSrSetupCfg ARGS
((
RgrUeSrSetupCfg *param,
Buffer *mBuf
));

 S16 cmPkRgrUeSrCfg ARGS
((
RgrUeSrCfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeSrCfg ARGS
((
RgrUeSrCfg *param,
Buffer *mBuf
));

S16 cmPkRgrUeUlSrsCfg ARGS
((
RgrUeUlSrsCfg *param,
Buffer *mBuf
));

S16 cmUnpkRgrUeUlSrsCfg ARGS
((
RgrUeUlSrsCfg *param,
Buffer *mBuf
));

#endif
#endif

#ifdef DM
/** @brief Request from RRM to MAC to bind the interface SAPs */
S16 DmUiRgrBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/** @brief Confirmation from MAC to RRM for the bind/unbind 
 * request for the interface SAPs */
S16 DmUiRgrBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/** @brief Request from RRM to MAC to unbind the interface SAPs */
S16 DmUiRgrUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/** @brief Configuration request from RRM to MAC for 
 * configuring Cell/UE/LC */
S16 DmUiRgrCfgReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   RgrCfgTransId        transId,
   RgrCfgReqInfo *      cfgReqInfo
));
/** @brief Configuration confirm from MAC to RRM */
S16 DmUiRgrCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrCfgTransId        transId,
   uint8_t                   status
));
#endif
/** @brief Sta Indication from Scheduler to RRM 
 *
 * @details This primitive is used to send status indication
 *          from scheduler to RRM. 
 * 
 * @param[in] pst     Pointer to a post structure
 * @param[in] suId    Service User SAP Id
 * @param[in] staInd  Status Indication .
*/
S16 NxLiRgrUeStaInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgrUeStaIndInfo      *ueStaInd
));

S16 cmPkRgrUeStaInd ARGS
((
Pst* pst,
SuId suId,
RgrUeStaIndInfo* ueStaInd
));

S16 cmUnpkRgrUeStaInd ARGS
((
RgrUeStaInd  func,
Pst *pst,
Buffer *mBuf
));

S16 cmPkRgrUeStaIndInfo ARGS
((
RgrUeStaIndInfo *param,
Buffer *mBuf
));


S16 cmUnpkRgrUeStaIndInfo ARGS
((
RgrUeStaIndInfo *param,
Buffer *mBuf
));


/* LTE_ADV_FLAG_REMOVED_START */
S16 cmPkRgrLteAdvancedUeConfig ARGS((
            RgrLteAdvancedUeConfig *param,
            Buffer *mBuf
            ));

S16 cmUnpkRgrLteAdvancedUeConfig ARGS((
            RgrLteAdvancedUeConfig *param,
            Buffer *mBuf
            ));

S16 cmPkRgrAbsConfig ARGS((
            RgrAbsConfig *param,
            Buffer *mBuf
            ));

S16 cmUnpkRgrAbsConfig ARGS((
            RgrAbsConfig *param,
            Buffer *mBuf
            ));

S16 cmPkRgrSfrConfig ARGS((
            RgrSfrConfig *param,
            Buffer *mBuf
            ));

S16 cmUnpkRgrSfrConfig ARGS((
            RgrSfrConfig *param,
            Buffer *mBuf
            ));

S16 cmPkRgrCellLteAdvancedFeatureCfg ARGS((
            RgrLteAdvancedCellConfig *param,
            Buffer *mBuf
            ));

S16 cmUnpkRgrCellLteAdvancedFeatureCfg ARGS((
            RgrLteAdvancedCellConfig *param,
            Buffer *mBuf
            ));

S16 cmPkRgrDsfrConfig ARGS((
         RgrDsfrConfig *param,
         Buffer *mBuf
         ));

S16 cmUnpkRgrDsfrConfig ARGS((
         RgrDsfrConfig *param,
         Buffer *mBuf
         ));
/* LTE_ADV_FLAG_REMOVED_END */

S16 cmPkRgrCellCsgParamCfg ARGS((
RgrCellCsgParamCfg *param,
Buffer *mBuf
));
S16 cmUnpkRgrCellCsgParamCfg ARGS((
RgrCellCsgParamCfg *param,
Buffer *mBuf
));
S16 cmPkRgrCellCntrlCmdCfg ARGS((
RgrCellCntrlCmdCfg *param,
Buffer *mBuf
));
S16 cmUnpkRgrCellCntrlCmdCfg ARGS((
RgrCellCntrlCmdCfg *param,
Buffer *mBuf
));

#ifdef __cplusplus
}
#endif
#endif /* __RGR_X__*/

/**********************************************************************

         End of file
**********************************************************************/
