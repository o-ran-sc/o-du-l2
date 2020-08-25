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
/* header include files (.h) */
#include "common_def.h"
#include "rgu.h"           /* RGU interface includes*/
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

/* header/extern include files (.x) */
#include "rgu.x"           
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "kw_err.h"
#include "kw.x"
#include "rlc_mac_inf.h"
#include "rlc.h"

/*******************************************************************
 *
 * @brief Reverse fixed buffer
 *
 * @details
 *
 *    Function : reverseFixBuf
 *
 *    Functionality: Reverse fixed buffer
 *
 * @params[in] Fixed buffer to be reversed
 *             Buffer length
 * @return void
 *
 * ****************************************************************/
void reverseFixBuf(uint8_t *buf, uint16_t len)
{
   uint8_t idx, revIdx;
   uint8_t temp;

   for(idx = 0, revIdx = len-1; idx < len/2; idx++, revIdx--)
   {
      temp = buf[idx];
      buf[idx] = buf[revIdx];
      buf[revIdx] = temp;
   }
   return;
}

/**
 * @brief Handler to process PDU received from MAC
 *
 * @details
 *    This function receives the PDU from MAC.
 *    seggregates common and dedicated logical channel
 *    PDU and call respective handler.
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] datInd  Data Indication Information
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 *
*/
uint8_t  RlcMacProcUlData(Pst *pst, RlcData *ulData)
{
   uint8_t         idx;
   uint8_t         lcId;                    /* Logical Channel */
   uint8_t         numDLch = 0;             /* Number of dedicated logical channel */
   uint16_t        copyLen;                 /* Number of bytes copied */
   SuId            suId = 0;
   bool            dLchPduPres;             /* PDU received on dedicated logical channel */
   RguLchDatInd    dLchData[RGU_MAX_LC];    /* PDU info on dedicated logical channel */
   RguDDatIndInfo  *dLchUlDat;              /* UL data on dedicated logical channel */
   RguCDatIndInfo  *cLchUlDat;              /* UL data on common logical channel */

   /* Initializing dedicated logical channel Database */
   for(idx = 0; idx < RGU_MAX_LC; idx++)
   {
      dLchData[idx].lcId = idx;
      dLchData[idx].pdu.numPdu = 0;
   }

   dLchPduPres = FALSE;

   /* Seggregate PDUs received on common and dedicated channels
    * and call common channel's handler */
   for(idx = 0; idx< ulData->numPdu; idx++)
   {
      if(ulData->pduInfo[idx].commCh)
      {
	 KW_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_UL, RLC_POOL, cLchUlDat, sizeof(RguCDatIndInfo));
	 cmMemset((U8*)cLchUlDat, (U8)0, sizeof(RguCDatIndInfo));
	 cLchUlDat->cellId = ulData->cellId;
	 GET_UE_IDX(ulData->rnti, cLchUlDat->rnti);
	 cLchUlDat->lcId   = ulData->pduInfo[idx].lcId;

         /* Copy fixed buffer to message */
	 if(SGetMsg(RLC_MEM_REGION_UL, RLC_POOL, &cLchUlDat->pdu) != ROK)
	 {
	    DU_LOG("\nRLC : Memory allocation failed at RlcMacProcUlData");
	    return RFAILED;
	 }
         reverseFixBuf(ulData->pduInfo[idx].pduBuf, ulData->pduInfo[idx].pduLen);
	 SCpyFixMsg(ulData->pduInfo[idx].pduBuf, cLchUlDat->pdu, 0, \
	    ulData->pduInfo[idx].pduLen, (MsgLen *)&copyLen);

	 KwLiRguCDatInd(pst, suId, cLchUlDat);
      }
      else
      {
	 if(!dLchPduPres)
	 {
	    KW_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_UL, RLC_POOL, dLchUlDat, sizeof(RguDDatIndInfo));
	    dLchPduPres = TRUE;
	 }

         /* Copy fixed buffer to message */
	 lcId = ulData->pduInfo[idx].lcId;
	 if(SGetMsg(RLC_MEM_REGION_UL, RLC_POOL, \
	    &dLchData[lcId].pdu.mBuf[dLchData[lcId].pdu.numPdu]) != ROK)
	 {
	    DU_LOG("\nRLC : Memory allocation failed at RlcMacProcUlData");
	    return RFAILED;
	 }
         reverseFixBuf(ulData->pduInfo[idx].pduBuf, ulData->pduInfo[idx].pduLen);
	 SCpyFixMsg(ulData->pduInfo[idx].pduBuf, dLchData[lcId].pdu.mBuf[dLchData[lcId].pdu.numPdu],\
	    0, ulData->pduInfo[idx].pduLen, (MsgLen *)&copyLen);

	 dLchData[lcId].pdu.numPdu++;
      }

      /* Free fixed buffer memory */
      KW_FREE_SHRABL_BUF(pst->region, pst->pool, ulData->pduInfo[idx].pduBuf, \
         ulData->pduInfo[idx].pduLen);
   }
   /* If any PDU received on dedicated logical channel, copy into RguDDatIndInfo
    * and call its handler */
   if(dLchPduPres)
   {
      dLchUlDat->cellId = ulData->cellId;
      GET_UE_IDX(ulData->rnti, dLchUlDat->rnti);
      for(idx = 0; idx < RGU_MAX_LC; idx++)
      {
	 if(dLchData[idx].pdu.numPdu)
	 {
	    cmMemcpy((U8 *)&dLchUlDat->lchData[numDLch], (U8 *)&dLchData[idx], sizeof(RguLchDatInd));
	    numDLch++;
	 }
      }
      dLchUlDat->numLch = numDLch;
      KwLiRguDDatInd(pst, suId, dLchUlDat);
   }
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      KW_FREE_SHRABL_BUF(pst->region, pst->pool, ulData, sizeof(RlcData));
   }
   return ROK;

}/* End of RlcMacProcUlData */

/*******************************************************************
 *
 * @brief Handler for extracting common and dedicated channel
 *      Scheduling result report.
 *
 * @details
 *
 *    Function : RlcMacProcSchResultRep
 *
 *    Functionality:
 *     Handler for extracting common and dedicated channel
 *      Scheduling result report
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcMacProcSchResultRep(Pst *pst, RlcSchResultRep *schRep)
{
   uint8_t idx;                     /* Iterator */
   uint8_t nmbDLch = 0;                 /* Number of dedicated logical channles */
   SuId suId = 0;
   RguCStaIndInfo   *cLchSchInfo;    /* Common logical channel scheduling result */
   RguDStaIndInfo   *dLchSchInfo;  /* Dedicated logical channel scheduling result */

   DU_LOG("\nRLC : Received scheduling report from MAC");
   for(idx=0; idx < schRep->numLc; idx++)
   {
      /* If it is common channel, fill status indication information
       * and trigger the handler for each common lch separately */
      if(schRep->lcSch[idx].commCh)
      {
	 KW_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_DL, RLC_POOL, cLchSchInfo, sizeof(RguCStaIndInfo));
	 cmMemset((U8*)cLchSchInfo, (U8)0, sizeof(RguCStaIndInfo));

	 cLchSchInfo->cellId  = schRep->cellId;
	 cLchSchInfo->lcId    = schRep->lcSch[idx].lcId;
	 cLchSchInfo->transId = schRep->slotInfo.sfn;
	 cLchSchInfo->transId = (cLchSchInfo->transId << 16) | schRep->slotInfo.slot;
	 GET_UE_IDX(schRep->rnti, cLchSchInfo->rnti);
	 KwLiRguCStaInd(pst, suId, cLchSchInfo);
      }
      else
      {
	 /* Fill status info structure if at least one dedicated channel
	  * scheduling report is received */
	 if(nmbDLch == 0)
	 {
	    KW_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_DL, RLC_POOL, dLchSchInfo, sizeof(RguDStaIndInfo));
	    dLchSchInfo->cellId = schRep->cellId;
	    dLchSchInfo->nmbOfUeGrantPerTti = 1;
	    GET_UE_IDX(schRep->rnti, dLchSchInfo->staInd[0].rnti);
	    dLchSchInfo->staInd[0].transId = schRep->slotInfo.sfn;
	    dLchSchInfo->staInd[0].transId = (dLchSchInfo->staInd[0].transId << 16) | schRep->slotInfo.slot; 
	    dLchSchInfo->staInd[0].nmbOfTbs = 1;
	    //dLchSchInfo->staInd[0].fillCrlPdu = /* TODO : Check the value needed to be filled */
	 }

	 /* Fill logical channel scheduling info */
	 dLchSchInfo->staInd[0].staIndTb[0].lchStaInd[nmbDLch].lcId = schRep->lcSch[idx].lcId;
	 dLchSchInfo->staInd[0].staIndTb[0].lchStaInd[nmbDLch].totBufSize = schRep->lcSch[idx].bufSize;
	 nmbDLch++;

      }

   }

   /* Calling handler for all dedicated channels scheduling*/
   if(nmbDLch)
   {
      dLchSchInfo->staInd[0].staIndTb[0].nmbLch = nmbDLch;
      KwLiRguDStaInd(pst, suId, dLchSchInfo);
   }

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      KW_FREE_SHRABL_BUF(pst->region, pst->pool, schRep, sizeof(RlcSchResultRep));
   }
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
