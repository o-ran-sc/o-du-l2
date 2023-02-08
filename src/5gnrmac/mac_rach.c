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
#include "lrg.h"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_utils.h"

/* Function pointer for sending RACH resource response from MAC to DU APP */
MacDuRachRsrcRspFunc macDuRachRsrcRspOpts[] =
{
   packDuMacRachRsrcRsp,   /* packing for loosely coupled */
   DuProcMacRachRsrcRsp,   /* packing for tightly coupled */
   packDuMacRachRsrcRsp    /* packing for light weight loosly coupled */
};

/*******************************************************************
 *
 * @brief Sends RACH indication to SCH
 *
 * @details
 *
 *    Function : sendRachIndMacToSch
 *
 *    Functionality:
 *     Sends RACH indication to SCH
 *
 * @params[in] RACH indication info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendRachIndMacToSch(RachIndInfo *rachInd)
{
   Pst pst;

   FILL_PST_MAC_TO_SCH(pst, EVENT_RACH_IND_TO_SCH);
   return(SchMessageRouter(&pst, (void *)rachInd));
}

/*******************************************************************
 *
 * @brief Adds an entry in list of RA Cb and fills RACH ind info
 *
 * @details
 *
 *    Function : createMacRaCb
 *
 *    Functionality:
 *     Adds an entry in list of RA Cb and fills RACH ind info
 *
 * @params[in] Pointer to cellCB,
 *             Pointer to RACH Indication information
 * @return ROK - SUCCESS
 *         RFAILED - FAILURE
 *
 * ****************************************************************/
uint8_t createMacRaCb(MacCellCb *cellCb, RachIndInfo *rachIndInfo)
{
   int8_t ueIdx = -1;
   uint8_t  ssbIdx = 0;
   uint16_t crnti = 0;
   MacUeCb  *ueCb = NULLP;

   /* Search if a UE CB is already present to which the incoming preamble index was dedicated */
   for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++) 
   {
      /* Check if ueCb has a crnti alloted to it. If not, it means this ueIdx
       * has no UE context stored in it, hence searching for preamble index in
       * this ueCb is not required */
      if(cellCb->ueCb[ueIdx].crnti)
      {
         for(ssbIdx = 0; ssbIdx < cellCb->ueCb[ueIdx].cfraResource.numSsb; ssbIdx++)
         {
            if(cellCb->ueCb[ueIdx].cfraResource.ssbResource[ssbIdx].raPreambleIdx == rachIndInfo->preambleIdx)
            {
               ueCb = &cellCb->ueCb[ueIdx];
               break;
            }
         }
         if(ueCb)
            break;
      }
   }

   if(ueCb)
   {
      /* If UE context is already present as in case of handover, fetch CRNTI from
       * UE CB allocated during UE context creation */
      crnti = ueCb->crnti;
   }
   else
   {
      /* If UE context not present, assign CRNTI to this UE */
      ueIdx = getFreeBitFromUeBitMap(rachIndInfo->cellId);
      if(ueIdx == -1)
      {
         DU_LOG("\nERROR  -->  MAC : Failed to find free UE Idx in UE bit map of cell Id [%d]", rachIndInfo->cellId);
         return RFAILED;
      }

      /* Calculate CRNTI from UE Index */
      GET_CRNTI(crnti, ueIdx+1);

      /* Store in raCb */
      memset(&cellCb->macRaCb[ueIdx], 0, sizeof(MacRaCbInfo));
      cellCb->macRaCb[ueIdx].cellId = rachIndInfo->cellId;
      cellCb->macRaCb[ueIdx].crnti  = crnti;

     /* Initialize MSG4 HARQ PROC CB */
     cellCb->macRaCb[ueIdx].msg4HqInfo.procId = MAX_NUM_HARQ_PROC;

   }

   /* Store in Rach Indication message to be sent to SCH */
   rachIndInfo->crnti  = crnti;
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes RACH indication from PHY
 *
 * @details
 *
 *    Function : fapiMacRachInd
 *
 *    Functionality:
 *      Processes RACH indication from PHY
 *
 * @params[in] Post structure
 *             Rach indication message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/ 
uint8_t fapiMacRachInd(Pst *pst, RachInd *rachInd)
{
   uint8_t      ret = ROK;
   uint8_t      pduIdx = 0;
   uint8_t      preambleIdx = 0;
   uint16_t     cellIdx = 0;  
   RachIndInfo  *rachIndInfo = NULLP;
   MacCellCb    *cellCb = NULLP;

   DU_LOG("\nINFO  -->  MAC : Received RACH indication");
   /* Considering one pdu and one preamble */
   pduIdx = 0;
   preambleIdx = 0;

   /* Validate cell Id */
   GET_CELL_IDX(rachInd->cellId, cellIdx);
   if(macCb.macCell[cellIdx] && (macCb.macCell[cellIdx]->cellId == rachInd->cellId))
         cellCb = macCb.macCell[cellIdx];

   if(!cellCb)
   {
      DU_LOG("\nERROR  --> MAC : Invalid Cell ID [%d] received in RACH Indication", rachInd->cellId);
      return RFAILED;
   }

   MAC_ALLOC(rachIndInfo, sizeof(RachIndInfo));
   if(!rachIndInfo)
   {
      DU_LOG("\nERROR  --> MAC : Memory allocation failure in fapiMacRachInd");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, rachInd, sizeof(RachInd));
      return RFAILED;
   }

   rachIndInfo->cellId = rachInd->cellId;
   rachIndInfo->timingInfo.sfn = rachInd->timingInfo.sfn;
   rachIndInfo->timingInfo.slot = rachInd->timingInfo.slot;
   rachIndInfo->slotIdx = rachInd->rachPdu[pduIdx].slotIdx;
   rachIndInfo->symbolIdx = rachInd->rachPdu[pduIdx].symbolIdx;
   rachIndInfo->freqIdx = rachInd->rachPdu[pduIdx].freqIdx;
   rachIndInfo->preambleIdx = rachInd->rachPdu[pduIdx].preamInfo[preambleIdx].preamIdx;
   rachIndInfo->timingAdv = rachInd->rachPdu[pduIdx].preamInfo[preambleIdx].timingAdv;

   /* Store the value in macRaCb */
   if((ret = createMacRaCb(cellCb, rachIndInfo)) == ROK)
   {
      /* Send RACH Indication to SCH */
      ret = sendRachIndMacToSch(rachIndInfo);
   }

   /* Free sharable buffer used to send RACH Indication from lower MAC to MAC */
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, rachInd, sizeof(RachInd));

   return ret;
}

/*******************************************************************
 *
 * @brief Processes RACH Resource request from DU APP
 *
 * @details
 *
 *    Function : MacProcRachRsrcReq
 *
 *    Functionality: Processes RACH resource request from DU APP.
 *      Fills and sends RACH resource request towards SCH.
 *
 * @params[in] Post structure
 *             RACH resource request
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcRachRsrcReq(Pst *pst, MacRachRsrcReq *rachRsrcReq)
{
   uint8_t   ret = RFAILED;
   uint16_t  cellIdx = 0;
   Pst       schPst;
   MacCellCb *cellCb = NULLP;
   MacUeCb   *ueCb = NULLP;
   SchRachRsrcReq *schRachRsrcReq = NULLP;

   DU_LOG("\nINFO  -->  MAC : Recieved RACH Resource Request for Cell ID [%d] UE ID [%d]",\
         rachRsrcReq->cellId, rachRsrcReq->ueId);

   /* Fetch Cell Cb */
   GET_CELL_IDX(rachRsrcReq->cellId, cellIdx);
   if(macCb.macCell[cellIdx] && (macCb.macCell[cellIdx]->cellId == rachRsrcReq->cellId))
   {
      cellCb = macCb.macCell[cellIdx];

      /* Fetch UE Cb */
      if(cellCb->ueCb[rachRsrcReq->ueId-1].ueId == rachRsrcReq->ueId)
      {
         ueCb = &cellCb->ueCb[rachRsrcReq->ueId-1];
         /* Allocate memory to RACH resource request to be sent to SCH */
         MAC_ALLOC(schRachRsrcReq, sizeof(SchRachRsrcReq));
         if(schRachRsrcReq)
         {
            /* Fill SCH RACH resource request from information received from DU APP to MAC */
            schRachRsrcReq->cellId = rachRsrcReq->cellId;
            schRachRsrcReq->crnti = ueCb->crnti;
            schRachRsrcReq->numSsb = rachRsrcReq->numSsb;
            memcpy(schRachRsrcReq->ssbIdx, rachRsrcReq->ssbIdx, sizeof(schRachRsrcReq->ssbIdx));

            /* Send RACH resource request from MAC to SCH */
            FILL_PST_MAC_TO_SCH(schPst, EVENT_RACH_RESOURCE_REQUEST_TO_SCH);
            ret = SchMessageRouter(&schPst, (void *)schRachRsrcReq);
         }
         else
            DU_LOG("\nERROR  -->  MAC : Memory allocation failed for RACH resource request to SCH");
      }
      else
         DU_LOG("\nERROR  -->  MAC : UE ID [%d] not found", rachRsrcReq->ueId);
   }
   else
      DU_LOG("\nERROR  -->  MAC : Cell ID [%d] not found", rachRsrcReq->cellId);

   /* Free sharable buffer used to send RACH reource request from DU APP to MAC */
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, rachRsrcReq, sizeof(MacRachRsrcReq));
   return ret;
}

/*******************************************************************
 *
 * @brief Processes RACH Resource response from SCH
 *
 * @details
 *
 *    Function : MacProcSchRachRsrcRsp
 *
 *    Functionality: Processes RACH resource response from SCH
 *      Fills and sends RACH resource response towards DU APP
 *
 * @params[in] Post structure
 *             RACH resource response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcSchRachRsrcRsp(Pst *pst, SchRachRsrcRsp *schRachRsrcRsp)
{
   uint16_t cellIdx = 0;
   Pst rspPst;
   MacRachRsrcRsp *rachRsrcRsp = NULLP;
   MacCellCb *cellCb = NULLP;
   MacUeCb   *ueCb = NULLP;

   DU_LOG("\nINFO  -->  MAC : Received RACH resource response from SCH : Cell ID [%d] CRNTI [%d]", \
         schRachRsrcRsp->cellId, schRachRsrcRsp->crnti);

   /* Fill RACH resource response to send to DU APP */
   MAC_ALLOC_SHRABL_BUF(rachRsrcRsp, sizeof(MacRachRsrcRsp));
   if(!rachRsrcRsp)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed for RACH resource response");
      MAC_FREE(schRachRsrcRsp, sizeof(SchRachRsrcRsp));
      return RFAILED;
   }
   rachRsrcRsp->cellId = schRachRsrcRsp->cellId;
   GET_UE_ID(schRachRsrcRsp->crnti, rachRsrcRsp->ueId);
   rachRsrcRsp->result = MAC_DU_APP_RSP_OK;

   /* Fill Pst structure to send RACH resource response from MAC to DU APP */
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_RACH_RESOURCE_RSP);

   /* If negative response is received from SCH, send it further to DU APP */ 
   if(schRachRsrcRsp->result == RSP_NOK)
   {
      DU_LOG("\nINFO  -->  MAC : RACH Resource response from SCH : Result [FAILURE]");
      rachRsrcRsp->result = MAC_DU_APP_RSP_NOK;
   }
   else
   {
      DU_LOG("\nINFO  -->  MAC : RACH Resource response from SCH : Result [SUCCESS]");
      
      /* Fetch Cell Cb */
      GET_CELL_IDX(schRachRsrcRsp->cellId, cellIdx);
      if(macCb.macCell[cellIdx] && (macCb.macCell[cellIdx]->cellId == schRachRsrcRsp->cellId))
      {   
         cellCb = macCb.macCell[cellIdx];

         /* Fetch UE Cb */
         if(cellCb->ueCb[rachRsrcRsp->ueId-1].crnti == schRachRsrcRsp->crnti)
            ueCb = &cellCb->ueCb[rachRsrcRsp->ueId-1];
         else
         {
            DU_LOG("\nERROR  -->  MAC : CRNTI [%d] not found", schRachRsrcRsp->crnti);
            rachRsrcRsp->result = MAC_DU_APP_RSP_NOK;
         }   
      }   
      else
      {   
         DU_LOG("\nERROR  -->  MAC : Cell ID [%d] not found", schRachRsrcRsp->cellId);
         rachRsrcRsp->result = MAC_DU_APP_RSP_NOK;
      }   
   }

   /* Fill SSB RACH resource info if SCH has sent a positive response and 
    * processing of SCH RACH resource response at MAC has been successful so far */
   if(rachRsrcRsp->result == MAC_DU_APP_RSP_OK)
   {
      rachRsrcRsp->newCrnti = ueCb->crnti; 
      rachRsrcRsp->cfraResource.numSsb = schRachRsrcRsp->cfraResource.numSsb;
      memcpy(rachRsrcRsp->cfraResource.ssbResource, schRachRsrcRsp->cfraResource.ssbResource, \
         rachRsrcRsp->cfraResource.numSsb * sizeof(MacCfraSsbResource));

      /* Copy resources to UE CB in MAC */
      memcpy(&ueCb->cfraResource, &rachRsrcRsp->cfraResource, sizeof(MacCfraResource));
   }

   /* Free SCH RACH resource response */
   MAC_FREE(schRachRsrcRsp, sizeof(SchRachRsrcRsp));

   /* Send RACH resource response to DU APP */
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_RACH_RESOURCE_RSP);
   return (*macDuRachRsrcRspOpts[rspPst.selector])(&rspPst, rachRsrcRsp);
}


/*******************************************************************
 *
 * @brief Processes RACH Resource release from DU APP
 *
 * @details
 *
 *    Function : MacProcRachRsrcRel
 *
 *    Functionality: Processes RACH resource release from DU APP.
 *      Fills and sends RACH resource release towards SCH.
 *
 * @params[in] Post structure
 *             RACH resource release
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcRachRsrcRel(Pst *pst, MacRachRsrcRel *rachRsrcRel)
{
   uint8_t   ret = RFAILED;
   uint16_t  cellIdx = 0;
   Pst       schPst;
   MacCellCb *cellCb = NULLP;
   MacUeCb   *ueCb = NULLP;
   SchRachRsrcRel *schRachRsrcRel = NULLP;

   DU_LOG("\nINFO  -->  MAC : Recieved RACH Resource Release for Cell ID [%d] UE ID [%d]",\
         rachRsrcRel->cellId, rachRsrcRel->ueId);

   /* Fetch Cell Cb */
   GET_CELL_IDX(rachRsrcRel->cellId, cellIdx);
   if(macCb.macCell[cellIdx] && (macCb.macCell[cellIdx]->cellId == rachRsrcRel->cellId))
   {
      cellCb = macCb.macCell[cellIdx];

      /* Fetch UE Cb */
      if(cellCb->ueCb[rachRsrcRel->ueId-1].ueId == rachRsrcRel->ueId)
      {
         ueCb = &cellCb->ueCb[rachRsrcRel->ueId-1];
         /* Allocate memory to RACH resource release to be sent to SCH */
         MAC_ALLOC(schRachRsrcRel, sizeof(SchRachRsrcRel));
         if(schRachRsrcRel)
         {
            /* Fill SCH RACH resource release from information received from DU APP to MAC */
            memset(schRachRsrcRel, 0, sizeof(SchRachRsrcRel));
            schRachRsrcRel->cellId = rachRsrcRel->cellId;
            schRachRsrcRel->crnti = ueCb->crnti;
            memcpy(&schRachRsrcRel->cfraResource, &ueCb->cfraResource, sizeof(schRachRsrcRel->cfraResource));
            
            /* Release RACH resources at MAC */
            memset(&ueCb->cfraResource, 0, sizeof(MacCfraResource));

            /* Send RACH resource release from MAC to SCH */
            FILL_PST_MAC_TO_SCH(schPst, EVENT_RACH_RESOURCE_RELEASE_TO_SCH);
            ret = SchMessageRouter(&schPst, (void *)schRachRsrcRel);
         }
         else
            DU_LOG("\nERROR  -->  MAC : Memory allocation failed for RACH resource release to SCH");
      }
      else
         DU_LOG("\nERROR  -->  MAC : UE ID [%d] not found", rachRsrcRel->ueId);
   }
   else
      DU_LOG("\nERROR  -->  MAC : Cell ID [%d] not found", rachRsrcRel->cellId);

   /* Free sharable buffer used to send RACH reource release from DU APP to MAC */
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, rachRsrcRel, sizeof(MacRachRsrcRel));
   return ret;
}

/* spec-38.211 Table 6.3.3.1-7 */
uint8_t UnrestrictedSetNcsTable[MAX_ZERO_CORR_CFG_IDX] = 
{0, 2, 4, 6, 8, 10, 12, 13, 15, 17, 19, 23, 27, 34, 46, 69};

/*******************************************************************
 *
 * @brief Processes UL scheduling info from SCH
 *
 * @details
 *
 *    Function : MacProcUlSchInfo
 *
 *    Functionality: Processes UL scheduling info from SCH
 *
 * @params[in] Post structure
 *             UL scheduling info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo)
{
   uint16_t  cellIdx;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_UL_SCH_INFO\n");
#endif

   GET_CELL_IDX(ulSchedInfo->cellId, cellIdx);
   if(ulSchedInfo != NULLP)
   {
      MacUlSlot *currUlSlot = 
	 &macCb.macCell[cellIdx]->ulSlot[ulSchedInfo->slotIndInfo.slot % macCb.macCell[cellIdx]->numOfSlots];
      memcpy(&currUlSlot->ulInfo, ulSchedInfo, sizeof(UlSchedInfo)); 
   }
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/

