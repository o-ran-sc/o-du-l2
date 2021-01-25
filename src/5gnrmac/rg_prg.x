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
            Two MAC instances(PRG = MAC to MAC).

  File:     rg_prg.x 

**********************************************************************/
/** 
  @file rg_Prg.x 
  @brief Structure declarations and definitions for MAC-MAC (PRG) interface.
  */

#ifndef __RGPRG_X__ 
#define __RGPRG_X__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef L2_OPTMZ
typedef TfuDatReqTbInfo    RgPrgDatReqTbInfo;
#endif

/** 
  * @brief Typedef for uplink Dedicated Logical channel group Id.
  */
typedef uint8_t                 RgPrgLteLcgId;

/** 
  * @brief Structure to hold uplink Dedicated Logical channel info.
  *
  * @details
  *    - lcId      : Logical channel ID at MAC
  *    - lcgId     : Logical channel group ID at MAC
  *    - qci       : QCI for this logical channel control block
  *    - measOn    : TRUE if Timing Info needs to be fetched for scheduled UL
  *                  IP throughput else FALSE
  */
typedef struct rgPrgUlLcInfo
{
   CmLteLcId     lcId;    /*!< Logical channel ID */
   RgPrgLteLcgId lcgId;    /*!< Logical channel group */
#ifdef LTE_L2_MEAS
   uint8_t       qci;     /*!< QCI for this logical channel control block */

   Bool          measOn;   /*!< TRUE if Timing Info needs to be fetched for
                             Scheduled UL IP throughput else FALSE */

#endif
}RgPrgUlLcInfo;

/** 
  * @brief Structure to hold logical channel group information
  *
  * @Details
  *    - lcgId   : Logical channel group ID at MAC
  *    - lcCount : Logical channel count
  *    - isGbr   : Indicate if LCG is GBR LCG
  */
typedef struct rgPrgLcgInfo
{
   /* Right now not keeping associated logical channels, searching for
    * associated channels needed only during config */
   RgPrgLteLcgId    lcgId;   /*!< Group ID */
   uint8_t          lcCount; /*!< Lc count */
   Bool             isGbr;   /*!< Indicate if LCG is GBR LCG */
}RgPrgLcgInfo;

/** 
  * @brief Structure to hold downlink Dedicated Logical channel info.
  * 
  * @Details
  *   - lcId   : Logical channel ID at MAC
  */
typedef struct rgPrgDlLcInfo
{
   CmLteLcId            lcId;    /*!< Logical channel ID */
}RgPrgDlLcInfo;

#ifdef LTE_ADV
/** 
  * @brief Structure to hold Config req from PMAC to SMAC.
  * 
  * @details
  *   - ueId        : UE identifier
  *   - cellId      : Cell ID
  *   - maxUlHqRetx : Maximum number of harq retx
  *   - ulLcInfo    : Dedicated Uplink logical channel info
  *   - lcgInfo     :  Logical channel groups
  *   - dlLcInfo    : Dedicated Downlink logical channels in UE
  *   - txMode      : UE Transmission mode Cfg
  *   - rguDlSapId  : Sap id for associated with RLC DL
  *   - rguUlSapId  : Sap id for associated with RLC UL
  */
typedef struct rgPrgUeSCellCfgInfo
{
   CmLteRnti            ueId;            /*!< UE identifier */
   CmLteCellId          cellId;          /*!< Cell ID */

   uint8_t              maxUlHqRetx;     /*!< Maximum number of harq
                                          * re-transmissions */ 
   RgPrgUlLcInfo        ulLcInfo[RG_MAX_LC_PER_UE];  /*!< Dedicated Uplink 
                                                logical channel information */
   RgPrgLcgInfo         lcgInfo[RG_MAX_LCG_PER_UE]; /*!< Logical channel 
                                                 groups */
   RgPrgDlLcInfo        dlLcInfo[RG_MAX_LC_PER_UE];  /*!< Dedicated Downlink 
                                                 logical channels in UE */

   CrgTxMode            txMode;          /*!< UE Transmission mode Cfg */

   SuId                 rguDlSapId;      /* !< Sap id for associated 
                                     with RLC DL */
   SuId                 rguUlSapId;      /* !< Sap id for associated 
                                      with RLC UL */
}RgPrgUeSCellCfgInfo;

/** 
  * @brief Structure to hold config confirm info from PMAC to SMAC.
  * 
  * @details
  *   - ueId    : UE identifier
  *   - sCellId : SCell ID
  *   - status  : status (OK/NOK)
  */
typedef struct rgPrgCfgCfmInfo
{
   CmLteRnti       ueId;    /*!< UE identifier */
   CmLteCellId     sCellId;  /*!< SCell ID */
   uint8_t         status;  /*!< Status: OK/NOK */
   uint8_t         event;   /*!< type of event */ 
}RgPrgCfgCfmInfo;

/** 
  * @brief  Structure to hold Ue delete Req/Ue Id change Info from PMAC to SMAC.
  *
  * @details
  *    - ueId     : UE identifier
  *    - sCellId  : sCell ID
  *    - newRnti  : new RNTI changed as part of reestablishment
  **/
typedef struct rgPrgUeSCellDelInfo
{
   CmLteRnti       ueId;     /*!< UE identifier */
   CmLteCellId     sCellId;  /*!< sCell ID */
   CmLteRnti       newRnti;  /*!< new RNTI changed as part of reestablishment */
}RgPrgUeSCellDelInfo;

/** 
  * @brief Structure to hold LC Config req from PMAC to SMAC.
  * 
  * @details
  *   - cellId      : Cell ID
  *   - crnti       : CRNTI for DTCH and DCCH
  *   - lcId        : Logical Channel Id
  *   - lcgId       : Logical channel group Id
  */
typedef struct rgPrgUeSCellLchModInfo
{
   CmLteCellId cellId;       /*!< Cell ID */
   CmLteRnti   crnti;        /*!< CRNTI for DTCH and DCCH */
   CmLteLcId   lcId;         /*!< Logical channel ID */

   struct ulLchRecfgS
   {
      uint8_t  lcgId;  /*!< Logical channel group ID */
   } ulLchRecfg;                /*!< Uplink logical channel reconfiguration
                               information */
}RgPrgUeSCellLchModInfo;

/** 
  * @brief Structure to hold LC Config req for deletion from PMAC to SMAC.
  * 
  * @details
  *   - cellId      : Cell ID
  *   - crnti       : CRNTI for DTCH and DCCH
  *   - lcId        : Logical Channel Id
  *   - dir         : Indicates Direction
  */
typedef struct rgPrgUeSCellLchDelInfo
{
   CmLteCellId cellId;       /*!< Cell ID */
   CmLteRnti   crnti;        /*!< CRNTI for DTCH and DCCH */
   CmLteLcId   lcId;         /*!< Logical channel ID */
   uint8_t     dir;          /*!< Indicates Direction. Direction can take following
                            values:<br>
                            PRG_DIR_TX<br>
                            PRG_DIR_RX<br>
                            PRG_DIR_TX_RX
                            */
}RgPrgUeSCellLchDelInfo;

/** 
  * @brief Logical channel configuration information for uplink logical channels. 
  *
  * @details
  *    -ulTrchType      : Indicates type of UL transport channel:
                                  Validated only for BCCH at MAC. 
                                  UL Transport channel type can take following values:<br>
                                   CM_LTE_TRCH_RACH<br>
                                   CM_LTE_TRCH_UL_SCH   
   *   -lcgId;          :  Logical channel group ID 
   */
typedef struct rgPrgUlLchCfg
{
   uint8_t             ulTrchType; /*!< Indicates type of UL transport channel:
                                   Validated only for BCCH at MAC. 
                                   UL Transport channel type can take following values:<br>
                                   CM_LTE_TRCH_RACH<br>
                                   CM_LTE_TRCH_UL_SCH */  
   uint8_t             lcgId;      /*!< Logical channel group ID */
} RgPrgUlLchCfg;


/** 
   *  @brief Logical channel configuration information for downlink logical channels. 
   *
   *  @details             
   *     -dlTrchType    :  Indicates type of DL transport channel:
                                   Validated only for BCCH at MAC. DL Transport
                                   channel type can take following values:<br>
                                   CM_LTE_TRCH_BCH<br> 
                                   CM_LTE_TRCH_PCH<br>
                                   CM_LTE_TRCH_DL_SCH  
*/
typedef struct rgPrgDlLchCfg 
{
   uint8_t             dlTrchType; /*!< Indicates type of DL transport channel:
                                   Validated only for BCCH at MAC. DL Transport
                                   channel type can take following values:<br>
                                   CM_LTE_TRCH_BCH<br> 
                                   CM_LTE_TRCH_PCH<br>
                                   CM_LTE_TRCH_DL_SCH  */
} RgPrgDlLchCfg;
  /**  @brief Logical channel configuration information for common and dedicated channels. 
   *
   *   @details
   *       cellId      :  Cell ID 
   *       crnti       :  CRNTI for DTCH and DCCH 
   *       lcId        :  Logical channel ID 
   *       lcType      :  Identifies the Logical channel type. lcType can take the following values:<br>                                            
                                      CM_LTE_LCH_BCCH<br>
                                      CM_LTE_LCH_PCCH<br>
                                      CM_LTE_LCH_CCCH<br>
                                      CM_LTE_LCH_DCCH<br>
                                      CM_LTE_LCH_DTCH   
                            
    *      dir         :  Indicates Direction. Direction can take following 
                                  values:<br>
                                  PRG_DIR_TX<br>
                                  PRG_DIR_RX<br>
                                  PRG_DIR_TX_RX 

    *      dlInfo      :   Downlink logical channel configuration info 
    *      ulInfo      :   Uplink logical channel configuration info 

    *      qci;        :            QCI for the logical channel.
                                  Valid Range:[0-255] (Actual QCI - 1). 
*/
typedef struct rgPrgUeSCellLchAddInfo
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
                            
   uint8_t        dir;        /*!< Indicates Direction. Direction can take following 
                                  values:<br>
                                  PRG_DIR_TX<br>
                                  PRG_DIR_RX<br>
                                  PRG_DIR_TX_RX */  

   RgPrgDlLchCfg   dlInfo;     /*!< Downlink logical channel configuration info */
   RgPrgUlLchCfg   ulInfo;     /*!< Uplink logical channel configuration info */
#ifdef LTE_L2_MEAS
   uint8_t qci;                   /*!< QCI for the logical channel.
                                  Valid Range:[0-255] (Actual QCI - 1). */

#endif /* LTE_L2_MEAS */
} RgPrgUeSCellLchAddInfo;


/* 
   Function Prototypes 
 */
/** @brief Request from PMAC to SMAC to add Ue Scell config. */
S16 RgPrgPMacSMacUeSCellCfg ARGS
   ((
     Pst         *pst,    
     RgPrgUeSCellCfgInfo *ueSCellCb
    ));

/** @brief Request from PMAC to SMAC to add Ue Scell config. */
S16 RgPrgPMacSMacUeSCellCfgReq ARGS
((
 Pst         *pst,
 RgPrgUeSCellCfgInfo *ueSCellCb
));

/** @brief Function ptr for Request from PMAC to SMAC to add Ue Scell config.*/
typedef S16 (*RgPrgUeSCellCfgReq) ARGS ((
         Pst*                 pst,
         RgPrgUeSCellCfgInfo          *ueSCellCb
         ));

/** @brief Config confirm of Ue SCell config Req from SMAC to PMAC */
S16 RgPrgSMacPMacCfg ARGS ((
         Pst              *pst,
         RgPrgCfgCfmInfo  *cfgCfm
         ));

/** @brief Config confirm from SMAC to PMAC */
S16 RgPrgSMacPMacCfgCfm ARGS ((
         Pst             *pst,    
         RgPrgCfgCfmInfo *cfgCfm
         ));

/** @brief Function ptr for Config confirm from SMAC to PMAC */
typedef S16 (*RgSMacPMacCfgCfm) ARGS((
         Pst*                 pst,
         RgPrgCfgCfmInfo      *cfgCfm
         ));

/** 
 * @brief Ue SCell cfg delete Req from PMac to SMac*/
S16 RgPrgPMacSMacUeSCellDel ARGS ((
         Pst                  *pst,
         RgPrgUeSCellDelInfo  *sCellUedelReq
         ));

/** 
 * @brief Ue SCell cfg delete Req from PMac to SMac*/
S16 RgPrgPMacSMacUeSCellDelReq ARGS ((
         Pst                 *pst,    
         RgPrgUeSCellDelInfo *sCellUedelReq
         ));

/** 
 * @brief Function ptr to Ue SCell cfg delete Req from PMac to SMac*/
typedef S16 (*RgUeSCellDelReq) ARGS((
         Pst                   *pst,
         RgPrgUeSCellDelInfo   *sCellUedelReq
         ));

#ifdef LCPRG
/** 
 * @brief Ue SCell Cfg Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkPrgPMacSMacUeSCellCfgReq ARGS ((
         Pst                          *pst,    
         RgPrgUeSCellCfgInfo          *ueSCellCb
         ));

S16 cmUnpkPrgPMacSMacUeSCellCfgReq ARGS ((
         RgPrgUeSCellCfgReq   func,
         Pst                  *pst,
         Buffer               *mBuf
         ));

/** 
 * @brief Ue SCell Add Cfg cfm from SMac to PMac.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkPrgSMacPMacCfgCfm ARGS((
         Pst                  *pst,    
         RgPrgCfgCfmInfo      *cfgCfm
         ));


S16 cmUnpkPrgSMacPMacCfgCfm ARGS((
         RgSMacPMacCfgCfm    func,
         Pst                 *pst,
         Buffer              *mBuf
         ));
/** 
 * @brief SCell Ue Delete Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkPrgPMacSMacUeSCellDelReq ARGS((
         Pst                  *pst,    
         RgPrgUeSCellDelInfo  *sCellUeDelInfo
         ));


S16 cmUnpkPrgPMacSMacUeSCellDelReq ARGS((
         RgUeSCellDelReq    func,
         Pst                *pst,
         Buffer             *mBuf
         ));


/** 
 * @brief Ue Lch recfg Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkPrgPMacSMacUeSCellLchModReq ARGS((
         Pst                       *pst,    
         RgPrgUeSCellLchModInfo    *lchCfgCb
         ));

S16 cmUnpkPrgPMacSMacUeSCellLchModReq ARGS((
         RgPrgUeScellModLchReq       func,
         Pst                         *pst,
         Buffer                      *mBuf
         ));

/** 
 * @brief SCell Ue Delete Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkPrgPMacSMacUeSCellLchDelReq ARGS((
         Pst                          *pst,    
         RgPrgUeSCellLchDelInfo       *delLcCb
         ));


S16 cmUnpkPrgPMacSMacUeSCellLchDelReq ARGS((
         RgPrgUeScellDelLchReq     func,
         Pst                       *pst,
         Buffer                    *mBuf
         ));

/** 
 * @brief Ue Lch cfg Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling. 
 */
S16 cmPkPrgPMacSMacUeSCellLchAddReq ARGS ((
         Pst                       *pst,    
         RgPrgUeSCellLchAddInfo    *lchCfgCb
         ));

S16 cmUnpkPrgPMacSMacUeSCellLchAddReq ARGS ((
         RgPrgUeScellAddLchReq       func,
         Pst                         *pst,
         Buffer                      *mBuf
         ));


#endif /* LCPRG */

/** @brief Request from PMAC to SMAC to add Lch Reconfig. */
S16 RgPrgPMacSMacUeScellLchMod ARGS
   ((
     Pst                      *pst,    
     RgPrgUeSCellLchModInfo   *lchCfgCb
    ));

/** @brief Request from PMAC to SMAC to add Lch Reconfig. */

/** @brief Function ptr for Request from PMAC to SMAC to add Lch Reconfig.*/
typedef S16 (*RgPrgUeScellModLchReq) ARGS((
         Pst*                        pst,
         RgPrgUeSCellLchModInfo      *lchCfgCb
         ));

/** @brief Request from PMAC to SMAC to delete Lch . */
S16 RgPrgPMacSMacUeScellLchDel ARGS
   ((
     Pst                       *pst,    
     RgPrgUeSCellLchDelInfo    *delLcCb
    ));

/** @brief Request from PMAC to SMAC to delete Lch. */

/** @brief Function ptr for Request from PMAC to SMAC to delete Lch.*/
typedef S16 (*RgPrgUeScellDelLchReq) ARGS((
         Pst*                      pst,
         RgPrgUeSCellLchDelInfo    *delLcCb
         ));


/** @brief Request from PMAC to SMAC to add Lch config. */
S16 RgPrgPMacSMacUeScellLchAdd ARGS
   ((
     Pst                      *pst,    
     RgPrgUeSCellLchAddInfo   *lchCfgCb
    ));

/** @brief Request from PMAC to SMAC to add Lch config. */

/** @brief Function ptr for Request from PMAC to SMAC to add Lch config.*/
typedef S16 (*RgPrgUeScellAddLchReq) ARGS
         ((Pst                        *pst,
         RgPrgUeSCellLchAddInfo      *lchCfgCb
         ));
#endif /* LTE_ADV */
#ifdef __cplusplus
}
#endif
#endif /* __RGPRG_X__*/

/**********************************************************************
  
         End of file:     rg_prg.x
**********************************************************************/
