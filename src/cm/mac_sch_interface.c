/* header include files (.h) */
#include "common_def.h"
#include "tfu.h"           /* RGU Interface defines */
#include "lrg.h"

//#include "fapi.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* RGU Interface includes */
#include "lrg.x"
#include "du_app_mac_inf.h"
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
int packMacSchSlotInd(Pst *pst, SlotIndInfo *slotInd)
{
   Buffer *mBuf = NULLP;
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* pack SFN and slot value */
   CMCHKPK(SPkU16,slotInd->sfn, mBuf);
   CMCHKPK(SPkU16,slotInd->slot, mBuf);

   RETVALUE(SPstTsk(pst,mBuf));
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
int packMacSchRachInd(Pst *pst, RachIndInfo *rachInd)
{
   if((pst->selector == MAC_SELECTOR_LC) || (pst->selector == MAC_SELECTOR_LWLC))
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
int packMacSchCrcInd(Pst *pst, CrcIndInfo *crcInd)
{
   if((pst->selector == MAC_SELECTOR_LC) || (pst->selector == MAC_SELECTOR_LWLC))
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
uint8_t packMacSchDlRlcBoInfo(Pst *pst, DlRlcBOInfo *dlBoInfo)
{
   if((pst->selector == MAC_SELECTOR_LC) || (pst->selector == MAC_SELECTOR_LWLC))
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
int packSchMacDlAlloc(Pst *pst, DlSchedInfo  *dlSchedInfo)
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
int packSchMacUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo)
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
int packSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg)
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
int packSchCellCfgCfm(Pst *pst, SchCellCfgCfm *schCellCfgCfm)
{
	return ROK;
}


/**********************************************************************
  End of file
 **********************************************************************/

