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
#include "common_def.h"
#include "tfu.h"
#include "lrg.h"
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_tmr.h"
#include "sch_utils.h"
#ifdef NR_DRX
#include "sch_drx.h"
#endif
typedef struct schCellCb SchCellCb;
typedef struct schUeCb SchUeCb;
void schUlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchUlHqEnt *hqE);
void schUlHqAddToFreeList(SchUlHqProcCb *hqP);
/**
 * @brief UL Harq entity intialization
 *
 * @details
 *
 *     Function : schUlHqEntInit
 *      
 *      This function intialize UL Harq entity
 *           
 *  @param[in]  SchCellCb *cellCb, cell cb pointer
 *  @param[in]  SchUeCb *ueCb, ue cb pointer
 *  @return  
 *      -# void
 **/
void schUlHqEntInit(SchCellCb *cellCb, SchUeCb *ueCb)
{
   ueCb->ulHqEnt.numHqPrcs = SCH_MAX_NUM_UL_HQ_PROC;
   ueCb->ulHqEnt.maxHqTx  = cellCb->schHqCfg.maxUlDataHqTx;
   ueCb->ulHqEnt.cell = cellCb;
   ueCb->ulHqEnt.ue =ueCb;
   schUlHqEntReset(cellCb, ueCb, &ueCb->ulHqEnt);
}
/**
 * @brief UL Harq entity Reset
 *
 * @details
 *
 *     Function : schUlHqEntReset
 *      
 *      This function Reset UL Harq entity
 *           
 *  @param[in]  SchCellCb *cellCb, cell cb pointer
 *  @param[in]  SchUeCb *ueCb, ue cb pointer
 *  @param[in]  SchUlHqEnt *hqE, Ul Harq entity pointer
 *  @return  
 *      -# void
 **/
void schUlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchUlHqEnt *hqE)
{
   uint8_t count = 0;
   SchUlHqProcCb *hqP = NULL;
   cmLListInit(&hqE->free);
   cmLListInit(&hqE->inUse);
   for(count=0; count < hqE->numHqPrcs; count++)
   {
      hqP = &(hqE->procs[count]);
      hqP->procId = count;
      hqP->hqEnt = hqE;
      hqP->maxHqTxPerHqP = hqE->maxHqTx;
      hqP->ulHqEntLnk.node = (PTR)hqP;
      hqP->ulHqProcLink.node = (PTR)hqP;
      hqP->ulSlotLnk.node = (PTR)hqP;
      cellCb->api->SchInitUlHqProcCb(hqP);
      schUlHqAddToFreeList(hqP);
   }
}

/**
 * @brief Add hq process to free list of UL Harq entity
 *
 * @details
 *
 *     Function : schUlHqAddToFreeList
 *      
 *      This function adds hq process to free list of UL Harq entity
 *           
 *  @param[in]  SchUlHqProcCb *hqP, UL harq process pointer
 *  @return  
 *      -# void
 **/
void schUlHqAddToFreeList(SchUlHqProcCb *hqP)
{
   cmLListAdd2Tail(&(hqP->hqEnt->free), &hqP->ulHqEntLnk);
}
/**
 * @brief Delete hq process from free list of UL Harq entity
 *
 * @details
 *
 *     Function : schUlHqDeleteFromFreeList
 *      
 *      This function deletes hq process to free list of UL Harq entity
 *           
 *  @param[in]  SchUlHqProcCb *hqP, UL harq process pointer
 *  @return  
 *      -# void
 **/
void schUlHqDeleteFromFreeList(SchUlHqProcCb *hqP)
{
   if(hqP->hqEnt->free.count == 0)
   {
      DU_LOG("\n ERROR schUlHqDeleteFromInUseList no proc in in free\n");
   }
   cmLListDelFrm(&(hqP->hqEnt->free), &hqP->ulHqEntLnk);
}
/**
 * @brief Add hq process to in use list of UL Harq entity
 *
 * @details
 *
 *     Function : schUlHqAddToInUseList
 *      
 *      This function adds hq process to in use list of UL Harq entity
 *           
 *  @param[in]  SchUlHqProcCb *hqP, UL harq process pointer
 *  @return  
 *      -# void
 **/
void schUlHqAddToInUseList(SchUlHqProcCb *hqP)
{
   cmLListAdd2Tail(&(hqP->hqEnt->inUse), &hqP->ulHqEntLnk);
}
/**
 * @brief Delete hq process from in use list of UL Harq entity
 *
 * @details
 *
 *     Function : schUlHqDeleteFromInUseList
 *      
 *      This function deletes hq process to in use list of UL Harq entity
 *           
 *  @param[in]  SchUlHqProcCb *hqP, UL harq process pointer
 *  @return  
 *      -# void
 **/
void schUlHqDeleteFromInUseList(SchUlHqProcCb *hqP)
{
   if(hqP->hqEnt->inUse.count == 0)
   {
      DU_LOG("\n ERROR schUlHqDeleteFromInUseList no proc in in use\n");
   }
   cmLListDelFrm(&(hqP->hqEnt->inUse), &hqP->ulHqEntLnk);
}
/**
 * @brief Get available Harq process from Harq entity
 *
 * @details
 *
 *     Function : schUlGetAvlHqProcess
 *      
 *      This function fetches hq process from free list and puts in in use list
 *           
 *  @param[in]  SchCellCb *cellCb, cell cb pointer
 *  @param[in]  SchUeCb *ueCb, ue cb pointer
 *  @param[in]  SchUlHqProcCb **hqP, Address of UL harq process pointer
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
uint8_t schUlGetAvlHqProcess(SchCellCb *cellCb, SchUeCb *ueCb, SchUlHqProcCb **hqP)
{
   SchUlHqProcCb                  *tmp;
   if (ueCb->ulHqEnt.free.count == 0)
   {
      return RFAILED;
   }
   tmp = (SchUlHqProcCb*)(cmLListFirst(&(ueCb->ulHqEnt.free))->node);
   if (NULLP == tmp)
   {
      return RFAILED;
   }
   schUlHqDeleteFromFreeList(tmp);
   schUlHqAddToInUseList(tmp);
   *hqP = tmp;
   (*hqP)->maxHqTxPerHqP = ueCb->ulHqEnt.maxHqTx;
   return ROK;
}
/**
 * @brief Release Harq process from the UL Harq entity
 *
 * @details
 *
 *     Function : schUlReleaseHqProcess
 *      
 *      This function releases Harq process from UL Harq entity
 *           
 *  @param[in]  SchUlHqProcCb *hqP, UL harq process pointer
 *  @param[in]  Bool togNdi, indication to togle NDI bit
 *  @return  
 *      -# void
 **/
void schUlReleaseHqProcess(SchUlHqProcCb *hqP, Bool togNdi)
{
   SchCellCb  *cellCb = NULLP;
   if (togNdi == TRUE)
   {
      hqP->tbInfo.ndi ^= 1;
   }

   cellCb = hqP->hqEnt->cell;
   cellCb->api->SchFreeUlHqProcCb(hqP);
   
   schUlHqDeleteFromInUseList(hqP);
   schUlHqAddToFreeList(hqP);
}
/**
 * @brief Handles NACK for UL Harq process
 *
 * @details
 *
 *     Function : schUlHqProcessNack
 *      
 *      This function handle NACK for  UL Harq process
 *           
 *  @param[in]  SchUlHqProcCb *hqP, UL harq process pointer
 *  @return  
 *      -# void
 **/
void schUlHqProcessNack(SchUlHqProcCb *hqP)
{
   if (hqP->tbInfo.txCntr < hqP->maxHqTxPerHqP)
   {
       hqP->hqEnt->cell->api->SchAddToUlHqRetxList(hqP);
   }
   else
   {
      schUlReleaseHqProcess(hqP, TRUE);
   }
}
/**
 * @brief Handles ACK for UL Harq process
 *
 * @details
 *
 *     Function : schUlHqProcessAck
 *      
 *      This function handles ACK for UL Harq process
 *           
 *  @param[in]  SchUlHqProcCb *hqP, UL harq process pointer
 *  @return  
 *      -# void
 **/
void schUlHqProcessAck(SchUlHqProcCb *hqP)
{
   schUlReleaseHqProcess(hqP, TRUE);
}

/**
 * @brief Deletes HARQ Entity
 *
 * @details
 *
 *     Function : schUlHqEntDelete
 *      
 *      This function deletes HARQ entity and its member paremeters
 *           
 *  @param[in]  Pointer to UE
 *  @return  
 *      -# void
 **/
void schUlHqEntDelete(SchUeCb *ueCb)
{
  uint8_t count;
  SchUlHqProcCb *hqP;

  cmLListDeleteLList(&ueCb->ulHqEnt.free);
  cmLListDeleteLList(&ueCb->ulHqEnt.inUse);
  for(count=0; count < ueCb->ulHqEnt.numHqPrcs; count++)
  {
     hqP = &(ueCb->ulHqEnt.procs[count]);
     ueCb->cellCb->api->SchDeleteUlHqProcCb(hqP);
  }
  memset(&ueCb->ulHqEnt, 0, sizeof(SchUlHqEnt));
}

/**********************************************************************
  End of file
 **********************************************************************/
