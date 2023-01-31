/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"

/* header/extern include files (.x) */
#include "lrg.x"
#include "mac_sch_interface.h"

/**
 * @brief function to send Slot ind message from MAC
 *        to scheduler with loose coupling 
 *
 * @details
 *
 *     Function : packMacSchSlotInd
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  *slotInd, the value of SFN and slot
 *  @return  S16
 *      -# ROK
 **/
uint8_t packMacSchSlotInd(Pst *pst, SlotTimingInfo *slotInd)
{
   Buffer *mBuf = NULLP;
   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
   {
      return RFAILED;
   }

   /* pack SFN and slot value */
   CMCHKPK(oduUnpackUInt16,slotInd->sfn, mBuf);
   CMCHKPK(oduUnpackUInt16,slotInd->slot, mBuf);

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Pack and Send Rach Ind from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchRachInd
 *
 *    Functionality:
 *      Pack and Send Rach Ind from MAC to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchRachInd(Pst *pst, RachIndInfo *rachInd)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send Crc Ind from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchCrcInd
 *
 *    Functionality:
 *       Pack and Send Crc Ind from MAC to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchCrcInd(Pst *pst, CrcIndInfo *crcInd)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send DL RLC BO Info from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchDlRlcBoInfo
 *
 *    Functionality:
 *       Pack and Send L RLC BO Info from MAC to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchDlRlcBoInfo(Pst *pst, DlRlcBoInfo *dlBoInfo)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief function to pack DL Broadcast allocation message 
 *        from MAC to SCH
 *
 * @details
 *
 *     Function : packSchMaccDlAlloc 
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  DlSchedInfo  *dlSchedInfo
 *  @return  S16
 *      -# ROK
 **/
uint8_t packSchMacDlAlloc(Pst *pst, DlSchedInfo  *dlSchedInfo)
{
   return ROK;
}

/**
 * @brief function to pack UL Sch Info  message 
 *        from SCH to MAC
 *
 * @details
 *
 *     Function : packSchMacUlSchInfo 
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  UlSchedInfo *ulSchedInfo
 *  @return  S16
 *      -# ROK
 **/
uint8_t packSchMacUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo)
{
   return ROK;
}

/**
 * @brief function to pack cell cfg from MAC to SCH
 *
 * @details
 *
 *     Function : packSchCellCfg 
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  *schCellCfg, SchCellCfg structure
 *  @return  S16
 *      -# ROK
 **/
uint8_t packSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg)
{
   return ROK;
}

/**
 * @brief function to pack cell cfg cfm from SCH to MAC
 *
 * @details
 *
 *     Function : packSchCellCfgCfm
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  *schCellCfgCfm, SchCellCfgCfm structure
 *  @return  S16
 *      -# ROK
 **/
uint8_t packSchCellCfgCfm(Pst *pst, SchCellCfgCfm *schCellCfgCfm)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send UE Config Request from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchAddUeConfigReq
 *
 *    Functionality:
 *       Pack and Send UE Create Request from MAC to SCH
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchAddUeConfigReq(Pst *pst, SchUeCfgReq  *ueCfg)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED; 
   }
   return ROK;
}


/*******************************************************************
 *
 * @brief Packs and send UE create response
 *
 * @details 
 *
 *    Function : packSchUeCfgRsp
 *
 *    Functionality:
 *      Packs and send UE create response
 *
 * @params[in] Post structure
 *             UE config response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packSchUeCfgRsp(Pst *pst, SchUeCfgRsp *cfgRsp)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Packs and send UE reconfig response
 *
 * @details 
 *
 *    Function : packSchUeRecfgRsp
 *
 *    Functionality:
 *      Packs and send UE reconfig response
 *
 * @params[in] Post structure
 *             UE re-config response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packSchUeRecfgRsp(Pst *pst, SchUeRecfgRsp *recfgRsp)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send BSR from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchBsr
 *
 *    Functionality:
 *       Pack and Send BSR from MAC to SCH
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchBsr(Pst *pst, UlBufferStatusRptInd *bsrInd)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send SR UCI Ind from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSrSchUciInd
 *
 *    Functionality:
 *       Pack and Send SR UCI Ind from MAC to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchSrUciInd(Pst *pst, SrUciIndInfo *uciInd)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send DL HARQ Ind from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchDlHarqInd
 *
 *    Functionality:
 *       Pack and Send DL HARQ Ind from MAC to SCH
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchDlHarqInd(Pst *pst, DlHarqInd *dlHarqInd)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}


/*******************************************************************
 *
 * @brief Pack and Send Modify UE Config Request from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchModUeConfigReq
 *
 *    Functionality:
 *       Pack and Send Modify UE Config Request from MAC to SCH
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchModUeConfigReq(Pst *pst, SchUeRecfgReq  *ueRecfg)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED; 
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send RACH resource Request from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchRachRsrcReq
 *
 *    Functionality:
 *       Pack and Send RACH resouece Request from MAC to SCH
 *
 * @params[in] Post structure
 *             RACH resource request
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchRachRsrcReq(Pst *pst, SchRachRsrcReq *schRachRsrcReq)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED; 
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send RACH resource Response from SCH to MAC
 *
 * @details
 *
 *    Function : packSchRachRsrcRsp
 *
 *    Functionality:
 *       Pack and Send RACH resource Response from SCH to MAC
 *
 * @params[in] Post structure
 *             RACH resource response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packSchRachRsrcRsp(Pst *pst, SchRachRsrcRsp *schRachRsrcRsp)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
   {  
      /* TODO */
   }
   else
   {  
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send RACH resource Release from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchRachRsrcRel
 *
 *    Functionality:
 *       Pack and Send RACH resouece Release from MAC to SCH
 *
 * @params[in] Post structure
 *             RACH resource release
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchRachRsrcRel(Pst *pst, SchRachRsrcRel *schRachRsrcRel)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
   {   
      /* TODO */
   }   
   else
   {   
      return RFAILED; 
   }   
   return ROK;
}

/*******************************************************************
*
* @brief Pack and Send UE Delete Request from MAC to SCH
*
* @details
*
*    Function : packMacSchUeDeleteReq 
*
*    Functionality:
*       Pack and Send  UE Delete Request from MAC to SCH
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packMacSchUeDeleteReq(Pst *pst,  SchUeDelete *schUeDel)
{
    if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
    {
       /* TODO */
    }
    else
    {
       return RFAILED;
    }
    return ROK;
}

/*******************************************************************
*
* @brief Packs and send UE delete response
*
* @details
*
*    Function : packSchUeDeleteRsp
*
*    Functionality:
*      Packs and send UE delete response
*
* @params[in] Post structure
*             UE delete response
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t packSchUeDeleteRsp(Pst *pst, SchUeDeleteRsp  *delRsp)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send Cell Delete Request from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchCellDeleteReq
 *
 *    Functionality:
 *       Pack and Send  Cell Delete Request from MAC to SCH
 *
 * @params[in] Pst *pst,  SchCellDelete *schCellDelete
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchCellDeleteReq(Pst *pst,  SchCellDeleteReq *schCellDelete)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Packs and send CELL delete response
 *
 * @details
 *
 *    Function : packSchCellDeleteRsp
 *
 *    Functionality:
 * * @params[in] Post structure, SchCellDeleteRsp schCellDeleteRsp
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *
 * ****************************************************************/

uint8_t packSchCellDeleteRsp(Pst *pst, SchCellDeleteRsp  *schCellDeleteRsp)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send Slice Cfg request from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchSliceCfgReq 
 *
 *    Functionality:
 *       Pack and Send Slice Cfg request from MAC to SCH
 *
 * @params[in] Pst *pst, SchSliceCfgReq *cfgReq 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchSliceCfgReq(Pst *pst, SchSliceCfgReq *cfgReq)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief function to pack slice cfg rsp from SCH to MAC
 *
 * @details
 *
 *     Function : packSchSliceCfgRsp 
 *     
 *     
 *  @param[in]  Pst *pst, SchSliceCfgRsp *cfgRsp
 *  @return  S16 - ROK
 **/
uint8_t packSchSliceCfgRsp(Pst *pst, SchSliceCfgRsp *cfgRsp)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send Slice ReCfg request from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchSliceRecfgReq 
 *
 *    Functionality:
 *       Pack and Send Slice ReCfg request from MAC to SCH
 *
 * @params[in] Pst *pst, SchSliceRecfgReq *cfgReq 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchSliceRecfgReq(Pst *pst, SchSliceRecfgReq *sliceRecfgReq)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief function to pack slice cfg rsp from SCH to MAC
 *
 * @details
 *
 *     Function : packSchSliceRecfgRsp 
 *     
 *     
 *  @param[in]  Pst *pst, SchSliceRecfgRsp *cfgRsp
 *  @return  S16 - ROK
 **/
uint8_t packSchSliceRecfgRsp(Pst *pst, SchSliceRecfgRsp *recfgRsp)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Pack and Send paging indication from MAC to SCH
 *
 * @details
 *
 *    Function : packMacSchPagingInd
 *
 *    Functionality:
 *       Pack and Send paging indication from MAC to SCH
 *
 * @params[in] Pst *pst,  SchPageInd *pageInd
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchPagingInd(Pst *pst,  SchPageInd *pageInd)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LC))
   {
      /* TODO */
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief function to pack DL Paging allocation message 
 *        from MAC to SCH
 *
 * @details
 *
 *     Function : packSchMacDlPageAlloc 
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  DlPageAlloc *dlPageAlloc
 *  @return  S16
 *      -# ROK
 **/
uint8_t packSchMacDlPageAlloc(Pst *pst, DlPageAlloc *dlPageAlloc)
{
   return ROK;
}

/**
 * @brief function to pack Harq process release message 
 *        from SCH to MAC
 *
 * @details
 *
 *     Function : packSchMacDlReleaseHarq 
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  SchRlsHqInfo *rlsHqInfo
 *  @return  S16
 *      -# ROK
 **/
uint8_t packSchMacDlReleaseHarq(Pst *pst, SchRlsHqInfo *rlsHqInfo)
{
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/

