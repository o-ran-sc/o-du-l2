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

/**
 * @brief function to unpack Slot ind message from MAC
 *        to scheduler with loose coupling
 *
 * @details
 *
 *     Function : unpackMacSchSlotInd
 *
 *
 *  @param[in]  Pst *pst, the post structure
 *  @param[in]  Buffer *mBuf, the message buffer
 *  @return  S16
 *      -# ROK
 **/
uint8_t unpackMacSchSlotInd(MacSchSlotIndFunc func, Pst *pst, Buffer  *mBuf)
{
   /* TODO */
   return ROK;
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
uint8_t packMacSchAddUeConfigReq(Pst *pst, SchUeCfg  *ueCfg)
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
uint8_t packMacSchModUeConfigReq(Pst *pst, SchUeCfg  *ueCfg)
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
uint8_t packMacSchCellDeleteReq(Pst *pst,  SchCellDelete *schCellDelete)
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
 *    Function : packMacSchSliceReCfgReq 
 *
 *    Functionality:
 *       Pack and Send Slice ReCfg request from MAC to SCH
 *
 * @params[in] Pst *pst, SchSliceCfgReq *cfgReq 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSchSliceReCfgReq(Pst *pst, SchSliceCfgReq *cfgReq)
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
 *     Function : packSchSliceReCfgRsp 
 *     
 *     
 *  @param[in]  Pst *pst, SchSliceCfgRsp *cfgRsp
 *  @return  S16 - ROK
 **/
uint8_t packSchSliceReCfgRsp(Pst *pst, SchSliceCfgRsp *cfgRsp)
{
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/

