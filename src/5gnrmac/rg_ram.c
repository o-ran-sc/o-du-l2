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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_ram.c
  
**********************************************************************/

/** @file rg_ram.c
@brief This file has APIs to handle the random access procedure functionality.
*/

/* header include files (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"    /* SCH Interface includes */
#include "rg_prg.x"        /* PRG Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC includes */

/* local defines */

/* local typedefs */
 
/* forward references */

/***********************************************************
 *
 *     Func : rgRAMFreeUeCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
Void rgRAMFreeUeCb(Inst  inst,RgUeCb *ue)
{
   rgDHMFreeUe(inst,&ue->dl.hqEnt);

   /*ccpu00117052 - MOD - Passing double pointer for proper NULLP 
                          assignment */
   /* De-allocate the Ue */
   rgFreeSBuf(inst,(Data **)&ue, sizeof(*ue));

   /* Stack Crash problem for TRACE5 changes. Added the return below */
   return;

}  /* rgRAMFreeUeCb */

/**
 * @brief Handler for Random Access control block creation.
 *
 * @details
 *
 *     Function : rgRAMCreateUeCb
 *                Creates a raCb and gives the same to scheduler for its updation.
 *     
 *
 *  @param[in]       RgCellCb       *cell 
 *  @param[in]       CmLteRnti      tmpCrnti 
 *  @param[out]      RgErrInfo      *err
 *  @return  RgUeCb*
 **/
RgUeCb* rgRAMCreateUeCb(RgCellCb *cell,CmLteRnti  tmpCrnti,Bool insert,RgErrInfo *err)
{
   Inst       inst = cell->macInst - RG_INST_START;
   RgUeCb    *ueCb = NULLP;

   DU_LOG("\nINFO  -->  MAC : CREATE UECB FOR CRNTI:%d",
             tmpCrnti);
   /* Allocate the Ue control block */
   if (rgAllocSBuf(inst,(Data **)&ueCb, sizeof(*ueCb)) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation FAILED for CRNTI:%d",tmpCrnti);
      err->errCause = RGERR_RAM_MEM_EXHAUST;
      return (NULLP);
   }

   /* Inititialize Ue control block */
   ueCb->ueId = tmpCrnti;

   /* Initialize the lists of the UE */
   rgDBMInitUe(ueCb); 
   
   if(insert == TRUE)
   {
      /* MS_FIX : Remove stale UEs if present */
      RgUeCb *staleUe = NULLP;
      /* Insert the created raCb into raCb list of cell */
      ueCb->rachLstEnt.next = NULLP;
      ueCb->rachLstEnt.prev = NULLP;
      ueCb->rachLstEnt.node = (PTR)(ueCb);
      /* MS_FIX : Remove stale UEs if present */
      staleUe = rgDBMGetUeCbFromRachLst (cell, tmpCrnti);
      if (staleUe)
      {
         rgDBMDelUeCbFromRachLst(cell, staleUe);
         rgRAMFreeUeCb(inst,staleUe);
      }
      rgDBMInsUeCbInRachLst(cell, ueCb);
   }

   return (ueCb);
}  /* rgRAMCreateUeCb */

/**
 * @brief Function for handling cell delete.
 *
 * @details
 *
 *     Function : rgRAMFreeCell
 *     
 *     This function shall be invoked whenever a cell needs to be deleted.
 *     This shall remove raCbs and raReqs stored in cell.
 *     
 *           
 *  @param[in,out] RgCellCb  *cell
 *  @return  S16
 *      -# ROK 
 **/
S16 rgRAMFreeCell(RgCellCb *cell)
{
   Inst    inst = cell->macInst - RG_INST_START;
   RgUeCb  *ueCb;

   /* Free CURRENT CRG cfg list */
   while ((ueCb = rgDBMGetNextUeCbFromRachLst(cell, NULLP)) != NULLP)
   {
      rgDBMDelUeCbFromRachLst(cell, ueCb);
      rgRAMFreeUeCb(inst,ueCb);
   }

   return ROK; 
} /* rgRAMFreeCell */
/**
 * @brief Function for handling RA response scheduled for a subframe.
 *
 * @details
 *
 *     Function : rgHndlRaResp
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of random access responses for a subframe RgSchMacSfAllocReq.
 *
 *     Processing steps :
 *
 *     This shall invoke RAM to create ueCbs for all the rapIds allocated and 
 *     shall invoke MUX to create RAR PDUs for raRntis allocated.
 *     
 *           
 *  @param[in] RgCellCb          *cell,
 *  @param[in] CmLteTimingInfo   timingInfo,
 *  @param[in] RgInfRarInfo      *rarInfo
 *  @param[in/out] RgErrInfo     *err
 *  @return  S16
 *      -# ROK 
 **/
S16 rgHndlRaResp(RgCellCb *cell,CmLteTimingInfo timingInfo,RgInfRarInfo *rarInfo,RgErrInfo *err)
{
   uint8_t  idx1,idx2;
   Buffer   *rarPdu;
   RgDlSf   *dlSf;
   uint8_t  idx;

   if(NULLP == rarInfo->raRntiInfo)
   {
      return RFAILED;
   }

   idx = (timingInfo.slot % RG_NUM_SUB_FRAMES);
   dlSf = &cell->subFrms[idx];

   /* Create RAR PDUs for all the allocated RA-RNTIs */
   for(idx1 = 0; idx1 < rarInfo->numRaRntis; idx1++)
   {
      if(ROK == (rgMUXBldRarPdu(cell, 
                     &rarInfo->raRntiInfo[idx1], &rarPdu, err)))
      {
         /* Create RaCbs for all the rapIds allocated */
         for(idx2 = 0; idx2 < rarInfo->raRntiInfo[idx1].numCrnti; idx2++)
         {
            if(FALSE == rarInfo->raRntiInfo[idx1].crntiInfo[idx2].isContFree)
            {
               if(rgRAMCreateUeCb(cell,
                  rarInfo->raRntiInfo[idx1].crntiInfo[idx2].tmpCrnti, 
                  TRUE, err) == NULLP)
               {
                  return RFAILED;
               }
            }
         }
         /* Store the created RAR PDU */
         dlSf->raRsp[dlSf->numRaRsp].pdcch.rnti = 
            rarInfo->raRntiInfo[idx1].raRnti;

         dlSf->raRsp[dlSf->numRaRsp].pdcch.dci = 
            rarInfo->raRntiInfo[idx1].dciInfo;

         dlSf->raRsp[dlSf->numRaRsp].rar = rarPdu;
         /* ccpu00132314-ADD-Adding txPower offset for the PDSCH transmission */
         dlSf->raRsp[dlSf->numRaRsp].txPwrOffset =
               rarInfo->txPwrOffset;

         dlSf->numRaRsp++;
      }
      else
      {
         DU_LOG("\nERROR  -->  MAC : RARNTI:%d Creation of RAR"
                  "PDU for failed", rarInfo->raRntiInfo[idx1].raRnti);
         continue;
      }
   } /* end of raRntis loop */
   return ROK;
} /* end of rgHndlRaResp */

/**********************************************************************
 
         End of file
**********************************************************************/
