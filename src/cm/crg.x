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
  
  Desc:     Structures, variables and typedefs required by the LTE MAC-RRC
            Control (CRG) interface.

  File:     crg.x 

**********************************************************************/

#ifndef __CRG_X__
#define __CRG_X__

/** 
  @file crg.x 
  @brief Structure declarations and definitions for CRG interface.
  */

#ifdef __cplusplus
extern "C" {
#endif

/* Enumertaions for CRG */

/* Data Structures for CRG */

/** 
  @brief Transaction ID between MAC and RRC. */
typedef struct crgCfgTransId
{
   uint8_t trans[CRG_CFG_TRANSID_SIZE]; /*!< RRC transaction ID */
} CrgCfgTransId;

/** 
  @brief Bandwidth configuration per cell. */
typedef struct crgBwCfg
{
   uint8_t dlTotalBw;            /*!< Total Dowlink Bandwidth */
   uint8_t ulTotalBw;            /*!< Total Uplink Bandwidth  */
} CrgBwCfg;

/** 
  @brief RACH configuration per cell. */
typedef struct crgRachCfg
{
   uint8_t  maxMsg3Tx;            /*!< Maximum number of message 3 transmissions */
} CrgRachCfg;

#ifdef LTE_ADV
/** @brief UE secondary cell config params to MAC*/
typedef struct crgUeSCellInfo
{
  uint16_t                  sCellId;       /*!< This will be secondary cellId */
  Inst                 macInst;       /*!< MAC instance for SCell */
  SuId                 rguUlSapId;    /*!< MAC-RLC UL Sap ID */ 
  SuId                 rguDlSapId;    /*!< MAC-RLC DL SAP ID */
}CrgUeSCellInfo;


/** @brief Configuration information of Secondary cells for a UE*/
typedef struct crgUeSecCellInfo 
{
  Bool                 isSCellCfgPres; /*!<  Bool indicates whether configuration
                                   is present */
  uint8_t                   numSCells;                        /*!< Number of SCells
                                                          */
  CrgUeSCellInfo       ueSCellCfg[CRG_MAX_SCELL_PER_UE]; /*!< Secondary cell 
                                                           configuration per UE */
}CrgUeSecCellInfo;

#endif /* LTE_ADV */

/** 
  @brief Cell configuration. */
typedef struct crgCellCfg
{
   CmLteCellId   cellId;       /*!< Cell ID */
   CrgRachCfg    rachCfg;      /*!< RACH Configuration */
   CrgBwCfg      bwCfg;        /*!< Bandwidth Configuration */
#ifdef TENB_MULT_CELL_SUPPRT
   SpId          rguUlSapId;   /*!< RLC UL SAP Id for common channels */
   SpId          rguDlSapId;   /*!< RLC DL SAP Id for common channels */
#endif
#ifdef EMTC_ENABLE
   uint8_t            emtcEnable;
#endif
} CrgCellCfg;

/** 
  @brief Uplink HARQ configuration per UE. */
typedef struct crgUeUlHqCfg
{
   uint8_t maxUlHqTx;           /*!< Maximum number of UL HARQ transmissions */
} CrgUeUlHqCfg;

/* crg_x_001.main_3 - Changes for MIMO feature addition */
/* crg_x_001.main_4 - Changes for MIMO feature addition */
/** 
 * @enum UE Transmission Modes */
typedef enum crgTxMode
{
   CRG_UE_TM_1 = 1,    /** Transmission Mode 1 */
   CRG_UE_TM_2,        /** Transmission Mode 2 */
   CRG_UE_TM_3,        /** Transmission Mode 3 */
   CRG_UE_TM_4,        /** Transmission Mode 4 */
   CRG_UE_TM_5,        /** Transmission Mode 5 */
   CRG_UE_TM_6,        /** Transmission Mode 6 */
   CRG_UE_TM_7         /** Transmission Mode 7 */
} CrgTxMode;

/** 
  @brief UE Transmission mode configuration */
typedef struct crgTxModeCfg
{
   Bool      pres;  /*!< TRUE if Transmission mode is being CFG/RECFG ed */
   CrgTxMode tm;    /*!< Transmission mode */
}CrgTxModeCfg;
/** 
  @brief UE configuration. */
typedef struct crgUeCfg
{
   CmLteCellId    cellId;      /*!< Cell ID */
   CmLteRnti      crnti;       /*!< UE ID: CRNTI */ 
   CrgUeUlHqCfg   ueUlHqCfg;   /*!< UE related UL HARQ configuration */
   CrgTxModeCfg   txMode;      /*!< UE Transmission mode Cfg */
#ifdef TENB_MULT_CELL_SUPPRT
   SpId           rguUlSapId;   /*!< RLC UL SAP Id for common channels */
   SpId           rguDlSapId;   /*!< RLC DL SAP Id for common channels */
#endif
} CrgUeCfg;

/** 
  @brief Logical channel configuration information for downlink logical channels. */
typedef struct crgDlLchCfg 
{
   uint8_t             dlTrchType; /*!< Indicates type of DL transport channel:
                                   Validated only for BCCH at MAC. DL Transport
                                   channel type can take following values:<br>
                                   CM_LTE_TRCH_BCH<br> 
                                   CM_LTE_TRCH_PCH<br>
                                   CM_LTE_TRCH_DL_SCH  */
} CrgDlLchCfg;

/** 
  @brief Logical channel configuration information for uplink logical channels. */
typedef struct crgUlLchCfg
{
   uint8_t             ulTrchType; /*!< Indicates type of UL transport channel:
                                   Validated only for BCCH at MAC. 
                                   UL Transport channel type can take following values:<br>
                                   CM_LTE_TRCH_RACH<br>
                                   CM_LTE_TRCH_UL_SCH */  
   uint8_t             lcgId;      /*!< Logical channel group ID */
} CrgUlLchCfg;

/* crg_x_001.main_2: Documentation update */
/** 
  @brief Logical channel configuration information for common and dedicated channels. */
typedef struct crgLchCfg
{
   CmLteCellId   cellId;     /*!< Cell ID */
   CmLteRnti     crnti;      /*!< CRNTI for DTCH and DCCH */
   CmLteLcId     lcId;       /*!< Logical channel ID */
   CmLteLcType   lcType;     /*!< Identifies the Logical channel type. lcType can take the following values:<br>                                            
                                      CM_LTE_LCH_BCCH<br>
                                      CM_LTE_LCH_PCCH<br>
                                      CM_LTE_LCH_CCCH<br>
                                      CM_LTE_LCH_DCCH<br>
                                      CM_LTE_LCH_DTCH */  
                            
   uint8_t            dir;        /*!< Indicates Direction. Direction can take following 
                                  values:<br>
                                  CRG_DIR_TX<br>
                                  CRG_DIR_RX<br>
                                  CRG_DIR_TX_RX */  

   CrgDlLchCfg   dlInfo;     /*!< Downlink logical channel configuration info */
   CrgUlLchCfg   ulInfo;     /*!< Uplink logical channel configuration info */
   /* crg_x_001.main_5 - ADD - Members corresponding to LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
   uint8_t qci;                   /*!< QCI for the logical channel.
                                  Valid Range:[0-255] (Actual QCI - 1). */

#endif /* LTE_L2_MEAS */
} CrgLchCfg;

/** 
  @brief Activation time information. */
typedef struct crgActvTime
{
   Bool            pres;      /*!< Indicates the presence of activation time */
   CmLteTimingInfo actvTime;  /*!< Activation time information */ 
} CrgActvTime;


/** 
  @brief Cell reconfiguration information. */
typedef struct crgCellRecfg
{
   CmLteCellId     cellId;         /*!< Cell ID */
   CrgRachCfg      rachRecfg;      /*!< RACH Configuration */
} CrgCellRecfg;

/** 
  @brief UE reconfiguration information. */
typedef struct crgUeRecfg
{
   CmLteCellId       cellId;          /*!< Cell ID */
   CmLteRnti         oldCrnti;        /*!< Old UE ID */
   CmLteRnti         newCrnti;        /*!< New UE ID: This value must match
                                        'oldCrnti',if no CRNTI change during
                                         reconfiguration */
   CrgUeUlHqCfg      ueUlHqRecfg;     /*!< UE UL HARQ information */
   CrgTxModeCfg      txMode;          /*!< UE Transmission mode ReCfg */
#ifdef LTE_ADV
   CrgUeSecCellInfo crgSCellCfg;    /*!< Secondary cell dedicated informaton
                                             per UE */
#endif
} CrgUeRecfg;

/** 
  @brief Logical channel reconfiguration information for dedicated channels only. */
typedef struct crgLchRecfg
{
   CmLteCellId cellId;       /*!< Cell ID */
   CmLteRnti   crnti;        /*!< CRNTI for DTCH and DCCH */
   CmLteLcId   lcId;         /*!< Logical channel ID */
   
   struct ulRecfgS 
   {
      uint8_t             lcgId;  /*!< Logical channel group ID */
   } ulRecfg;                /*!< Uplink logical channel reconfiguration information */

} CrgLchRecfg;

/** 
  @brief Basic reconfiguration information for MAC. */
typedef struct crgRecfg
{
   uint8_t recfgType;                /*!< Indicates reconfiguration type */
   union crgRecfgU
   {
      CrgCellRecfg  cellRecfg;  /*!< Cell Reconfig information */
      CrgUeRecfg    ueRecfg;    /*!< UE Reconfig information */ 
      CrgLchRecfg   lchRecfg;   /*!< Logical channel reconfiguration information */
   } u;                         /*!< Union of Cell/UE/Lch reconfiguration information
                                  */
} CrgRecfg;

/** 
  @brief Basic Delete information for MAC. */
typedef struct crgDel
{
   uint8_t delType;               /*!< Indicates configuration item to be deleted */
   union crgDelU
   {
      struct cellDelS 
      {
         CmLteCellId cellId; /*!< Cell ID */
      } cellDel;             /*!< Cell Delete information */

      struct ueDelS 
      {
         CmLteCellId cellId; /*!< Cell ID */
         CmLteRnti   crnti;  /*!< UE ID: CRNTI */
      } ueDel;               /*!< UE Delete information */
      
      struct lchDelS
      {
         CmLteCellId cellId; /*!< Cell ID */
         CmLteRnti   crnti;  /*!< CRNTI for DTCH and DCCH */
         CmLteLcId   lcId;   /*!< Logical channel ID */
         uint8_t          dir;    /*!< Indicates Direction. Direction can take following 
                                  values:<br>
                                  CRG_DIR_TX<br>
                                  CRG_DIR_RX<br>
                                  CRG_DIR_TX_RX */ 
      } lchDel;              /*!< Logical channel delete information */
   } u;                      /*!< Union of Cell/UE/Lch delete information */
} CrgDel;

/** 
  @brief UE RESET information for MAC. */
typedef struct crgRst
{
   CmLteCellId cellId; /*!< Cell ID */
   CmLteRnti   crnti;  /*!< UE ID: CRNTI tobe RESET */
} CrgRst;

/* 
   Function Prototypes 
 */
/** @brief Request from RRC to MAC to bind the interface SAPs. */
typedef S16 (*CrgBndReq) ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId));
/** @brief Confirmation from MAC to RRC for the bind/unbind 
 * request for the interface SAPs. */
typedef S16 (*CrgBndCfm) ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status));
/** @brief Request from RRC to MAC to unbind the interface SAPs. */
typedef S16 (*CrgUbndReq) ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason));
/** @brief Configuration Confirm from MAC to RRC. */
typedef S16 (*CrgCfgCfm) ARGS((
   Pst*                 pst,
   SuId                 suId,
   CrgCfgTransId        transId,
   uint8_t                   status));

/** @brief Request from RRC to MAC to bind the interface SAPs. */
S16 RgUiCrgBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/** @brief Confirmation from MAC to RRC for the bind/unbind 
 * request for the interface SAPs. */
S16 RgUiCrgBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/** @brief Request from RRC to MAC to Unbind the interface SAPs. */
S16 RgUiCrgUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/** @brief Configuration Confirm from MAC to RRC. */
S16 RgUiCrgCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CrgCfgTransId        transId,
   uint8_t                   status
));

#ifdef NH
/** @brief Request from RRC to MAC to bind the interface SAPs. */
S16 NhLiCrgBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/** @brief Confirmation from MAC to RRC for the bind/unbind 
 * request for the interface SAPs. */
S16 NhLiCrgBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/** @brief Request from RRC to MAC to Unbind the interface SAPs. */
S16 NhLiCrgUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/** @brief Configuration Confirm from MAC to RRC. */
S16 NhLiCrgCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CrgCfgTransId        transId,
   uint8_t                   status
));
#endif

#ifdef LCCRG
/** @brief Request from RRC to MAC to bind the interface SAPs. */
S16 cmPkCrgBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/** @brief Request from RRC to MAC to bind the interface SAPs. */
S16 cmUnpkCrgBndReq ARGS((
   CrgBndReq            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** @brief Confirmation from MAC to RRC for the bind/unbind 
 * request for the interface SAPs. */
S16 cmPkCrgBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/** @brief Confirmation from MAC to RRC for the bind/unbind 
 * request for the interface SAPs. */
S16 cmUnpkCrgBndCfm ARGS((
   CrgBndCfm            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** @brief Request from RRC to MAC to unbind the interface SAPs. */
S16 cmPkCrgUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
/** @brief Request from RRC to MAC to unbind the interface SAPs. */
S16 cmUnpkCrgUbndReq ARGS((
   CrgUbndReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** @brief Configuration Confirm from MAC to RRC. */
S16 cmPkCrgCfgCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   CrgCfgTransId        transId,
   uint8_t                   status
));
/** @brief Configuration Confirm from MAC to RRC. */
S16 cmUnpkCrgCfgCfm ARGS((
   CrgCfgCfm            func,
   Pst*                 pst,
   Buffer               *mBuf
));
S16 cmPkCrgCfgTransId ARGS((
   CrgCfgTransId        *param,
   Buffer               *mBuf
));
S16 cmUnpkCrgCfgTransId ARGS((
   CrgCfgTransId        *param,
   Buffer               *mBuf
));
S16 cmPkCrgBwCfg ARGS((
   CrgBwCfg             *param,
   Buffer               *mBuf
));
S16 cmUnpkCrgBwCfg ARGS((
   CrgBwCfg             *param,
   Buffer               *mBuf
));
S16 cmPkCrgRachCfg ARGS((
   CrgRachCfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkCrgRachCfg ARGS((
   CrgRachCfg           *param,
   Buffer               *mBuf
));
S16 cmPkCrgUeUlHqCfg ARGS((
   CrgUeUlHqCfg         *param,
   Buffer               *mBuf
));
S16 cmUnpkCrgUeUlHqCfg ARGS((
   CrgUeUlHqCfg         *param,
   Buffer               *mBuf
));

#ifdef LTE_ADV
S16 cmPkCrgUeSecCellInfo ARGS((
CrgUeSecCellInfo *param,
Buffer *mBuf
));

S16 cmUnpkCrgUeSecCellInfo ARGS((
CrgUeSecCellInfo *param,
Buffer *mBuf
));
#endif /* LTE_ADV */

S16 cmPkCrgActvTime ARGS((
   CrgActvTime          *param,
   Buffer               *mBuf
));
S16 cmUnpkCrgActvTime ARGS((
   CrgActvTime          *param,
   Buffer               *mBuf
));
#endif

#ifdef DM
/** @brief Request from RRC to MAC to bind the interface SAPs. */
S16 DmUiCrgBndReq ARGS((
   Pst*                 pst,
   SuId                 suId,
   SpId                 spId
));
/** @brief Confirmation from MAC to RRC for the bind/unbind 
 * request for the interface SAPs. */
S16 DmUiCrgBndCfm ARGS((
   Pst*                 pst,
   SuId                 suId,
   uint8_t                   status
));
/** @brief Request from RRC to MAC to unbind the interface SAPs. */
S16 DmUiCrgUbndReq ARGS((
   Pst*                 pst,
   SpId                 spId,
   Reason               reason
));
#endif

#ifdef __cplusplus
}
#endif
#endif /* __CRG_X__ */

/**********************************************************************
  
         End of file
**********************************************************************/
