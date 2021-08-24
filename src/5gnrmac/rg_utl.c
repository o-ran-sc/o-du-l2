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
  
     File:     rg_utl.c
  
**********************************************************************/

/** @file rg_utl.c
@brief This file implements utility functions for LTE MAC
*/


/* header include files (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"    /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */
#include  "mac_utils.h"

#include "rg_prg.h"        /* PRG(MAC-MAC) Interface includes */
#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "cm5.x"           /* Timer */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"    /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */
#include "rg_prg.x"        /* PRG(MAC-MAC) Interface includes */

#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC includes */

/* local defines */
#define RG_NON_MIMO_IDX 0

/* local typedefs */
 
/* local externs */
 
/* forward references */
static S16 rgUtlHndlCrntiChng ARGS((
         Inst            inst,
         RgCellCb        *cell,
         CmLteRnti       rnti,
         CmLteRnti       newRnti
         ));
static Void rgUtlHndlCrntiRls ARGS((
         RgCellCb        *cell,
         RgInfRlsRnti    *rlsRnti
         ));

S16 rgDelUeFrmAllSCell ARGS((
         RgCellCb       *cell,
         RgUeCb         *ue
         ));

#ifdef LTE_ADV
static S16 rgUtlSndCrntiChngReq2AllSMacs ARGS((
         RgCellCb        *cell,
         CmLteRnti       rnti,
         CmLteRnti       newRnti
         ));
#endif
/***********************************************************
 *
 *     Func : rgAllocShrablSBuf
 *
 *     Desc : Utility Function to Allocate static buffer which is 
 *            sharable among different layers if YYYY flag is enabled. 
 *            else it allocates from normal static region
 *            Memory allocated is assumed contiguous.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 rgAllocShrablSBuf
(
Inst    inst,
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      return RFAILED;
   }

   /* allocate buffer */
   MAC_ALLOC_SHRABL_BUF(pData, size);
   if(pData == NULLP)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb[inst].rgInit.region;
     dgn.u.mem.pool = rgCb[inst].rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      DU_LOG("\nERROR  -->  MAC : Unable to Allocate Buffer");
      return RFAILED;
   }

#ifndef ALIGN_64BIT
   DU_LOG("\nDEBUG  -->  MAC : MAC_ALLOC(Region (%d), Pool (%d), Size (%ld)), Data (0x%p))\n",
             rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, size, *pData);
#else
   DU_LOG("\nDEBUG  -->  MAC : MAC_ALLOC(Region (%d), Pool (%d), Size (%d)), Data (0x%p))\n",
             rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, size, *pData);
#endif

   /* zero out the allocated memory */
   memset(*pData, 0x00, size);

   return ROK;

} /* end of rgAllocSBuf */


/***********************************************************
 *
 *     Func : rgAllocSBuf
 *
 *     Desc : Utility Function to Allocate static buffer. 
 *            Memory allocated is assumed contiguous.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 rgAllocSBuf
(
Inst    inst,
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      return RFAILED;
   }

   /* allocate buffer */
#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   MAC_ALLOC(pData, size);
   if(pData == NULLP)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb[inst].rgInit.region;
     dgn.u.mem.pool = rgCb[inst].rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      DU_LOG("\nERROR  -->  MAC : Unable to Allocate Buffer");
      return RFAILED;
   }

   /* zero out the allocated memory */
   memset(*pData, 0x00, size);

   return ROK;

} /* end of rgAllocSBuf */

/*
*
*       Fun:   rgFreeSharableSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   void
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
Void rgFreeSharableSBuf
(
Inst inst,
Data **data,         /* address of pointer to data */
Size size            /* size */
)
{

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      return;
   }

   /* Deallocate buffer */
   MAC_FREE_SHRABL_BUF(rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, *data, size);
   
   if (data != NULLP)
   {
      return;
   }

   *data = NULLP;

   return;

} /* end of rgFreeSharableBuf */



/*
*
*       Fun:   rgFreeSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   void
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
Void rgFreeSBuf
(
Inst inst,
Data **data,         /* address of pointer to data */
Size size            /* size */
)
{

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      return;
   }


#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_CALLER();
#endif /* */
   /* Deallocate buffer */
   MAC_FREE(data, size);

   if (data != NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : rgFreeSBuf failed.\n");
      return;
   }

   *data = NULLP;

   return;

} /* end of rgFreeSBuf */


/***********************************************************
 *
 *     Func : rgGetMsg
 *
 *     Desc : Utility Function to Allocate message buffer. 
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 rgGetMsg
(
Inst    inst,
Buffer  **mBuf            /* Message Buffer pointer be returned */
)
{
   S16         ret;

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   ret = ODU_GET_MSG_BUF(RG_GET_MEM_REGION(rgCb[inst]), RG_GET_MEM_POOL(rgCb[inst]), mBuf);

   if (ROK != ret) 
   {
      /* Moving diagnostics structure to limited scope for optimization */
      RgUstaDgn   dgn;      /* Alarm diagnostics structure */

      rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM);

      /*  Send an alarm to Layer Manager */
      rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      DU_LOG("\nERROR  -->  MAC : Unable to Allocate Buffer");
      return RFAILED;
   }

   return ROK;

} /* end of rgGetMsg */


/***********************************************************
 *
 *     Func : rgFillDgnParams
 *
 *     Desc : Utility Function to Fill Diagonostic params. 
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
Void rgFillDgnParams
(
Inst        inst,
RgUstaDgn   *dgn,
uint8_t     dgnType
)
{

   switch(dgnType)
   {
      case LRG_USTA_DGNVAL_MEM:
         dgn->type = (uint8_t) LRG_USTA_DGNVAL_MEM;
         dgn->u.mem.region  = rgCb[inst].rgInit.region;
         dgn->u.mem.pool    = rgCb[inst].rgInit.pool;
      break;

      default:
      break;
   }

   return;
} /* end of rgFillDgnParams */


/***********************************************************
 *
 *     Func : rgUpdtRguDedSts
 *
 *     Desc : Utility Function to update rgu sap statistics for dedicated
 *            DatReqs.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
Void rgUpdtRguDedSts
(
Inst           inst,
RgUpSapCb      *rguDlSap,
uint8_t        stsType,   /* Statistics type to update */
RgRguDedDatReq *datReq    /* DatReq pointer */
)
{
   uint8_t idx1,idx2;
   uint32_t idx;

   switch(stsType)
   {
      case RG_RGU_SDU_RCVD:
      for(idx = 0; idx < datReq->nmbOfUeGrantPerTti; idx++)
      {
         RguDDatReqPerUe *datReqPerUe = &datReq->datReq[idx];
         for (idx1 = 0; idx1 < datReqPerUe->nmbOfTbs; idx1++) 
         {
            for(idx2 = 0; idx2 < datReqPerUe->datReqTb[idx1].nmbLch; idx2++) 
            {
               rguDlSap->sapSts.numPduRcvd +=
                  datReqPerUe->datReqTb[idx1].lchData[idx2].pdu.numPdu;
            }
         }
      }

         break;
      case RG_RGU_SDU_DROP:
      for(idx = 0; idx < datReq->nmbOfUeGrantPerTti; idx++)
      {
         RguDDatReqPerUe *datReqPerUe = &datReq->datReq[idx];
         for (idx1 = 0; idx1 < datReqPerUe->nmbOfTbs; idx1++) 
         {
            for(idx2 = 0; idx2 < datReqPerUe->datReqTb[idx1].nmbLch; idx2++) 
            {
               rguDlSap->sapSts.numPduRcvd +=
                  datReqPerUe->datReqTb[idx1].lchData[idx2].pdu.numPdu;
               rguDlSap->sapSts.numPduDrop +=
                  datReqPerUe->datReqTb[idx1].lchData[idx2].pdu.numPdu;
            }
         }
      }

         break;
   }
   
   return;
} /* rgUpdtRguDedSts */


/***********************************************************
 *
 *     Func : rgUpdtRguCmnSts
 *
 *     Desc : Utility Function to update rgu sap statistics for common
 *            DatReqs.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
Void rgUpdtRguCmnSts
(
Inst           inst,
RgUpSapCb     *rguDlSap,
uint8_t       stsType   /* Statistics type to update */
)
{

   switch(stsType)
   {
      case RG_RGU_SDU_RCVD:
         rguDlSap->sapSts.numPduRcvd ++;
         break;
      case RG_RGU_SDU_DROP:
         rguDlSap->sapSts.numPduRcvd ++;
         rguDlSap->sapSts.numPduDrop ++;
         break;
   }
   
   return;
} /* rgUpdtRguCmnSts */


/***********************************************************
 *
 *     Func : rgUpdtCellCnt
 *
 *     Desc : Utility Function to update cell count. It gives number of active
 *     cells
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called only after cell is added/deleted 
 *     from the globlal hashlist
 *
 *     File : rg_utl.c
 *
 **********************************************************/
Void rgUpdtCellCnt(Inst inst,uint8_t updtType)
{

   switch (updtType)
   {
      case RG_CFG_ADD:
         rgCb[inst].genSts.numCellCfg++;
         break;
      case RG_CFG_DEL:
         rgCb[inst].genSts.numCellCfg--;
         break;
      default:
         break;
   }

   return;
} /* rgUpdtCellCnt */


/***********************************************************
 *
 *     Func : rgUpdtUeCnt
 *
 *     Desc : Utility Function to update ue count. It gives number of active
 *     Ues.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called only after ue is added/deleted 
 *     from the globlal hashlist
 *
 *     File : rg_utl.c
 *
 **********************************************************/
Void rgUpdtUeCnt(Inst inst,uint8_t updtType)
{
   switch (updtType)
   {
      case RG_CFG_ADD:
         rgCb[inst].genSts.numUeCfg++;
         break;
      case RG_CFG_DEL:
         rgCb[inst].genSts.numUeCfg--;
         break;
      default:
         break;
   }
   return;
} /* rgUpdtUeCnt */

/*
*
*       Fun:   rgAllocEventMem
*
*       Desc:  This function allocates event memory 
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
S16 rgAllocEventMem
(
Inst     inst,
Ptr       *memPtr,
Size      memSize
)
{
   Mem               sMem;
   volatile uint32_t startTime=0;


   sMem.region = rgCb[inst].rgInit.region;
   sMem.pool = rgCb[inst].rgInit.pool;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (memSize<= 0)
   {
      DU_LOG("\nERROR  -->  MAC : rgAllocEventMem(): memSize invalid\n");
      return  (RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */


   /*starting Task*/
   SStartTask(&startTime, PID_MACUTL_CMALLCEVT);

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
#ifdef TFU_ALLOC_EVENT_NO_INIT
   if(ROK != cmAllocEvntNoInit(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
#else
   if(ROK != cmAllocEvnt(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
#endif /* */
   {
      DU_LOG("\nERROR  -->  MAC : cmAllocEvnt Failed"); 
      return RFAILED;
   }

   /*stoping Task*/
   SStopTask(startTime, PID_MACUTL_CMALLCEVT);

   return ROK;
} /* end of rgAllocEventMem*/

/*
*
*       Fun:   rgGetEventMem
*
*       Desc:  This function allocates event memory 
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
S16 rgGetEventMem
(
Inst      inst,
Ptr       *ptr,
Size      len,
Ptr       memCp
)
{
   S16   ret;

#ifdef TFU_ALLOC_EVENT_NO_INIT
   ret = cmGetMemNoInit(memCp, len, (Ptr *)ptr);
#else
   ret = cmGetMem(memCp, len, (Ptr *)ptr);
#endif /* */
   return (ret);
} /* end of rgGetEventMem*/

/***********************************************************
 *
 *     Func : rgGetPstToInst
 *
 *     Desc : Utility Function to get the pst structure to post a message to
 *     scheduler instance
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called while sending a msg from 
 *     MAC to a scheduler instance
 *
 *     File : rg_utl.c
 *
 **********************************************************/
Void rgGetPstToInst
(
Pst           *pst,
Inst          srcInst,
Inst          dstInst
)
{

   pst->srcEnt = rgCb[srcInst].rgInit.ent; 
   pst->srcInst = rgCb[srcInst].rgInit.inst;
   pst->srcProcId = rgCb[srcInst].rgInit.procId;
   pst->region = rgCb[srcInst].rgInit.region;
   pst->pool = rgCb[srcInst].rgInit.pool;

   pst->dstProcId = rgCb[dstInst].rgInit.procId;
   pst->dstEnt = rgCb[dstInst].rgInit.ent; 
   pst->dstInst = dstInst;
   pst->selector = 0;
   pst->prior     = PRIOR0;
   pst->intfVer   = 0;
   pst->route   = RTESPEC;

   return; 
} /* end of rgGetPstToInst */

/***********************************************************
 *
 *     Func : RgSchMacLcgRegReq
 *
 *     Desc : Utility Function to register the set of GBR LCG.
 *        Invoked at the time of LCG configuration/Reconfiguration at Schedular.
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given in lcInfo            
 *           - Store the if LCG is GBR or not.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time of LCG 
 *     configuration/Reconfiguration at Schedular. 
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 RgSchMacLcgRegReq(Pst *pst,RgInfLcgRegReq *lcgRegReq)
{
   Inst       inst;
   RgCellCb   *cell = NULLP;
   RgUeCb     *ue;

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP) ||
      (cell->cellId != lcgRegReq->cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : Cell does not exist ");
      return RFAILED;
   }

   if ((ue = rgDBMGetUeCb(cell, lcgRegReq->crnti)) == NULLP)
   {
     DU_LOG("\nERROR  -->  MAC : CRNTI:%d does not exist", 
		         lcgRegReq->crnti);
      return RFAILED;
   }
   ue->ul.lcgArr[lcgRegReq->lcgId].isGbr = lcgRegReq->isGbr;

   return ROK; 
} /* end of RgSchMacLcgRegReq */

#ifdef LTEMAC_SPS

/***********************************************************
 *
 *     Func : RgSchMacUlSpsResetReq
 *
 *     Desc : Utility Function to reset SPS params for a UE
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti 
 *           - reset implRelCnt and explRelCnt
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 RgSchMacUlSpsResetReq(Pst *pst,RgInfUlSpsReset *ulSpsResetInfo)
{
   Inst       inst;
   RgCellCb   *cell = NULLP;
   RgUeCb     *ue;

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP)||
      (cell->cellId != ulSpsResetInfo->cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : Cell does not exist ");
      return RFAILED;
   }

   if ((ue = rgDBMGetUeCb(cell, ulSpsResetInfo->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : CRNTI:%d does not exist", 
		      ulSpsResetInfo->crnti);
      return RFAILED;
   }

   ue->ul.implRelCntr = 0;
   ue->ul.explRelCntr = 0;

   return ROK; 
} /* end of RgSchMacUlSpsResetReq */




/***********************************************************
 *
 *     Func : RgSchMacSpsLcRegReq
 *
 *     Desc : Utility Function to register the set of uplink SPS logical
 *        channels for a SPS UE.
 *        Invoked at the time of activation of a UE for UL-SPS.
 *        Whenever there is data on these LCs MAC shall inform scheduler
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given in lcInfo            
 *           - Store the sps-rnti and set the bits corresponding to the
 *             logical channel ids in ueUlCb->spsLcMask.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time UL SPS is activated 
 *     for a UE at scheduler
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 RgSchMacSpsLcRegReq(Pst *pst,RgInfSpsLcInfo *lcInfo)
{
   Inst       inst;
   RgCellCb   *cell= NULLP;
   RgUeCb     *ue;
   uint8_t    idx;

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP) ||
      (cell->cellId != lcInfo->cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : Cell does not exist ");
      return RFAILED;
   }

   if ((ue = rgDBMGetUeCb(cell, lcInfo->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : CRNTI:%d does not exist", 
		      lcInfo->crnti);
      return RFAILED;
   }

   /* Store the sps-rnti and SPS LC information in the UE */ 
   ue->spsRnti = lcInfo->spsRnti;
   for (idx=0; idx < lcInfo->spsLcCnt; idx++)
   {
      /* KWORK_FIX: Modified the index from lcId to lcId-1 for handling lcId 10 properly */
      ue->ul.spsLcId[(lcInfo->spsLcId[idx])-1] = TRUE;
   }
   ue->ul.implRelCnt = lcInfo->implRelCnt;
   ue->ul.explRelCnt = ue->ul.implRelCnt + 1; /*(lcInfo->implRelCnt * lcInfo->spsPrd);*/

   /* Insert the UE into SPS UE List */
   if (rgDBMInsSpsUeCb(cell, ue) == RFAILED)
   {
      DU_LOG("\nERROR  -->  MAC : Ue insertion into SPS list failed SPS CRNTI:%d", ue->spsRnti);
      return RFAILED;
   } 

   return ROK; 
} /* end of RgSchMacSpsLcRegReq */


/***********************************************************
 *
 *     Func : RgSchMacSpsLcDeregReq
 *
 *     Desc : Utility Function to deregister the set of uplink SPS 
 *        logical channels for a UE.
 *        Invoked at the time of release of UL-SPS for an activated UE.
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given
 *           - Reset the bits corresponding to the logical channel ids in
 *             ueUlCb->spsLcMask.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time UL SPS is released 
 *     for a UE at scheduler
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 RgSchMacSpsLcDeregReq(Pst  *pst,CmLteCellId cellId,CmLteRnti crnti)
{
   Inst       inst;
   RgCellCb   *cell = NULLP;
   RgUeCb      *ue;

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP) ||
      (cell->cellId != cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : Cell does not exist ");
      return RFAILED;
   }

   if ((ue = rgDBMGetUeCb(cell, crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : CRNTI:%d Ue does not exist", crnti);
      return RFAILED;
   }

   /* No need to reset the SPS LC Ids as they will not be looked at*/

   /* Delete UE from the SPS UE List */
   rgDBMDelSpsUeCb(cell, ue);
   
   return ROK; 
} /* end of RgSchMacSpsLcDeregReq */

#endif /* LTEMAC_SPS */

/**
 * @brief Function for handling CRNTI change request 
 * received from scheduler to MAC.
 *
 * @details
 *
 *     Function : rgUtlHndlCrntiChng
 *     
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *     
 *           
 *  @param[in] Inst        inst
 *  @param[in] RgCellCb      *cell,
 *  @param[in] CmLteRnti     rnti,
 *  @param[in] CmLteRnti     newRnti
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
static S16 rgUtlHndlCrntiChng 
(
Inst            inst,
RgCellCb        *cell,
CmLteRnti       rnti,
CmLteRnti       newRnti
)
{
   RgUeCb         *ue = NULLP;
   RgUeCb         *newUe = NULLP;

   ue = rgDBMGetUeCb(cell, rnti);
   newUe = rgDBMGetUeCbFromRachLst(cell, newRnti);
   if ((ue == NULLP) || (newUe == NULLP))
   {
      DU_LOG("\nERROR  -->  MAC : RNTI:%d Failed to get UECB[%lu:%lu] or NEW RNTI:%d", 
               rnti, ((PTR)ue), ((PTR)newUe), newRnti);
      return RFAILED;
   }
#ifdef XEON_SPECIFIC_CHANGES
   DU_LOG("\nDEBUG  -->  MAC : MAC:UE[%d] id changed to %d\n", rnti, newRnti);
#endif
   rgDBMDelUeCb(cell, ue);

   ue->ueId = newRnti;

   memcpy(&(ue->contResId), &(newUe->contResId), 
		   sizeof(newUe->contResId));
   /* Fix : syed MSG4 might be RETXing need to store the
    * HARQ context. */
   rgDHMFreeUe(inst,&ue->dl.hqEnt);
   ue->dl.hqEnt = newUe->dl.hqEnt;
       
   rgDBMInsUeCb(cell, ue);

   rgDBMDelUeCbFromRachLst(cell, newUe);
   rgFreeSBuf(inst,(Data **)&newUe, sizeof(*newUe));

   return ROK;
} /* end of rgUtlHndlCrntiChng */

#ifdef LTE_ADV
/**
 * @brief Function for handling UE  release for SCELL
 * triggered from SCH to MAC.
 *
 * @details
 *
 *     Function : rgDelUeFrmAllSCell
 *     
 *        - This Function should be invoked by PCell of UE
 *        - Remove the UE context from SCELL corresponding to rnti.
 *           
 *  @param[in] Inst      *macInst,
 *  @param[in] RgUeCb    *ue
 *  @return  ROK is SUCCESS 
 **/
S16 rgDelUeFrmAllSCell(RgCellCb *cell,RgUeCb *ue)
{
   Inst        inst     = cell->macInst - RG_INST_START;
   uint8_t     idx      = 0;
   Inst        sCellInstIdx;
   Pst         dstInstPst;
   RgPrgUeSCellDelInfo ueSCellDelInfo;

   /* To Delete the SCells if exisits for that UE */
   for(idx = 0; idx < RG_MAX_SCELL_PER_UE ; idx++)
   {
      if(TRUE == ue->sCelInfo[idx].isSCellAdded)
      {
         sCellInstIdx = ue->sCelInfo[idx].macInst - RG_INST_START;

         rgGetPstToInst(&dstInstPst, inst, sCellInstIdx);
         ueSCellDelInfo.ueId = ue->ueId;
         ueSCellDelInfo.sCellId = ue->sCelInfo[idx].sCellId;

         /* Filling same ueId in newRnti so that SMAC will check if newRnti
          *and old UeId is same that means its a UeSCell delete request*/
         ueSCellDelInfo.newRnti = ue->ueId;

         RgPrgPMacSMacUeSCellDel(&dstInstPst, &ueSCellDelInfo);
         ue->sCelInfo[idx].isSCellAdded = FALSE;
      } /* loop of if */
   } /* loop of for */

   return ROK;
} /* rgDelUeFrmAllSCell */

/**
 * @brief Function to validate AddSCellCfg.
 *
 * @details
 *
 *     Function : rgUtlVltdAddSCellCfg
 *     
 *           
 *  @param[in] ueSCellCb   secondary cell CB for validation
 *  @param[in] cell        cell control block
 *  @param[in] inst        instance number to fetch rgCb instance
 *  @return  S16
 *      -# ROK 
 **/
S16 rgUtlVltdAddSCellCfg(RgPrgUeSCellCfgInfo *ueSCellCb,RgCellCb *cell,Inst inst)
{
  S16 ret = ROK; 
  
  /* To Validate the CellID presence */
  if((cell == NULLP) ||
        (cell->cellId != ueSCellCb->cellId))
  {
     DU_LOG("\nERROR  -->  MAC : [%d]Sec Cell does not exit %d\n",
              ueSCellCb->ueId, ueSCellCb->cellId);
     ret = RFAILED;
  }
#ifdef TENB_MULT_CELL_SUPPRT
   if((ueSCellCb->rguDlSapId > rgCb[inst].numRguSaps) ||
      (ueSCellCb->rguUlSapId > rgCb[inst].numRguSaps))
   {
      DU_LOG("\nERROR  -->  MAC : Invald Sap Id: DL %d UL %d for ueId %d failed\n",
               ueSCellCb->rguDlSapId,
               ueSCellCb->rguUlSapId,
               ueSCellCb->cellId);
     ret = RFAILED;
   }
#endif
   return (ret);
} /* rgUtlVltdAddSCellCfg */

/**
 * @brief Function to build CrntiChangeReq and send to all SMACs.
 *
 * @details
 *
 *     Function : rgUtlSndCrntiChngReq2AllSMacs
 *     
 *        - This Function should be invoked by PCell of UE
 *        - It sends RgPrgPMacSMacUeSCellDelReq to all SMACs with newRnti sent
 *          by SCH. SMAC will check if newRnti is not equal to old UeId then it
 *          do only UeId change else it will delete the UeScell context
 *           
 *  @param[in] cell    Cell CB to get Ue control block
 *  @param[in] rnti    Ue Identifier used to fill in UeId Change req
 *  @param[in] newRnti UE new identifier, to be used in UeId Change req
 *  @return  S16
 *      -# ROK 
 **/
static S16 rgUtlSndCrntiChngReq2AllSMacs
(
RgCellCb        *cell,
CmLteRnti       rnti,
CmLteRnti       newRnti
)
{
   Inst                inst = cell->macInst - RG_INST_START;
   Inst                sCellInstIdx;
   Pst                 dstInstPst;
   RgPrgUeSCellDelInfo ueIdChngReq;
   RgUeCb              *ue;
   uint8_t                  idx;
#ifdef L2_OPTMZ
TfuDelDatReqInfo delDatReq;
#endif

   /* use newRnti to get UeCb in PMac because rnti is already changed in PMac*/
   ue = rgDBMGetUeCb(cell, newRnti);
   if (ue == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]RNTI:Failed to get ueCb \
               newRnti=%d\n", rnti, newRnti);
      return RFAILED;
   }
   /* For all added SCells, prepare and send ueIdChngReq */
   for(idx = 0; idx < RG_MAX_SCELL_PER_UE ; idx++)
   {
      if(TRUE == ue->sCelInfo[idx].isSCellAdded)
      {
         sCellInstIdx = ue->sCelInfo[idx].macInst - RG_INST_START;

         rgGetPstToInst(&dstInstPst, inst, sCellInstIdx);
         /* fill old rnti*/
         ueIdChngReq.ueId = rnti;
         ueIdChngReq.sCellId = ue->sCelInfo[idx].sCellId;
         
         /* Filling newRnti so that SMAC can check if old ueId and new UeId
          *(newRnti) is different then its a UeId change request from PMAC.
          * RgPrgPMacSMacUeSCellDelReq is being reused for UeId change req
          * from PMAC to SMAC*/
         ueIdChngReq.newRnti = newRnti;
         
         /* Re-using UeSCellDelReq API for UeId change*/
         RgPrgPMacSMacUeSCellDel(&dstInstPst, &ueIdChngReq);
#ifdef L2_OPTMZ
      /* Sending delDatReq to CL to clear the Pdus for old UeId present in CL*/
      delDatReq.cellId = ueIdChngReq.sCellId;
      delDatReq.ueId = ueIdChngReq.ueId;
      rgLIMTfuDelDatReq(sCellInstIdx, &delDatReq);
#endif

      } /* loop of if */
   } /* loop of for */

   return ROK;
} /* rgUtlSndCrntiChngReq2AllSMacs */

#endif /* LTE_ADV */

/**
 * @brief Function for handling CRNTI Context release 
 * triggered from SCH to MAC.
 *
 * @details
 *
 *     Function : rgUtlHndlCrntiRls
 *     
 *        - Remove the UE context from MAC corresponding to rnti.
 *     
 *           
 *  @param[in] RgCellCb      *cell,
 *  @param[in] CmLteRnti     rnti
 *  @return  Void 
 **/
static Void rgUtlHndlCrntiRls(RgCellCb *cell,RgInfRlsRnti *rlsRnti)
{
   Inst           inst = cell->macInst - RG_INST_START;
   RgUeCb        *ue = NULLP;
#ifdef LTEMAC_SPS
   RgUeCb         *spsUeCb = NULLP;
#endif

   if ((ue = rgDBMGetUeCb(cell, rlsRnti->rnti)) == NULLP)
   {
      /* Check in RachLst */
      if((ue=rgDBMGetUeCbFromRachLst (cell, rlsRnti->rnti)) != NULLP)
      {
         /* Delete Ue from the UE list */
         rgDBMDelUeCbFromRachLst(cell, ue);

         /* Free Ue */
         rgRAMFreeUeCb(inst,ue);
      }
      else
      {
          DU_LOG("\nERROR  -->  MAC : RNTI:%d No ueCb found in RachLst",rlsRnti->rnti);
      }
   }
   else
   {
#ifdef LTE_ADV
      if(FALSE == rlsRnti->isUeSCellDel)
      {
         rgDelUeFrmAllSCell(cell, ue);
      }
#endif /* LTE_ADV */

      /* Delete Ue from the UE list */
      rgDBMDelUeCb(cell, ue);
#ifdef LTEMAC_SPS
      spsUeCb = rgDBMGetSpsUeCb (cell, ue->spsRnti);
      if (spsUeCb)
      {
         rgDBMDelSpsUeCb(cell, spsUeCb);
      }
#endif

      /* Free Ue */
      rgCFGFreeUeCb(cell, ue);	   
      /* MS_REMOVE : syed Check in RachLst */
      {
         if((ue=rgDBMGetUeCbFromRachLst (cell, rlsRnti->rnti)) != NULLP)
         {
	     DU_LOG("\nERROR  -->  MAC : RNTI:%d STALE UE is still present", rlsRnti->rnti);         
         }
      }
   }

   return;
} /* end of rgUtlHndlCrntiRls */

/**
 * @brief Function for handling RaResp request received from scheduler to MAC.
 *
 * @details
 *
 *     Function : RgSchMacRlsRntiReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of random access responses for a subframe.
 *     This shall invoke RAM to create ueCbs for all the rapIds allocated and 
 *     shall invoke MUX to create RAR PDUs for raRntis allocated.
 *     
 *           
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RaRespInfo          *rarInfo
 *  @return  S16
 *      -# ROK 
 **/
S16 RgSchMacRlsRntiReq(Pst *pst,RgInfRlsRnti *rlsRnti)
{
//   Pst            schPst;
//   RgInfUeDelInd  ueDelInd;
   Inst           macInst;
   RgCellCb       *cell;
#ifdef L2_OPTMZ
TfuDelDatReqInfo delDatReq;
#endif

   RG_IS_INST_VALID(pst->dstInst);
   macInst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[macInst].cell;

   if(NULLP == rlsRnti)
   {
      return RFAILED;
   }

   if((cell == NULLP) ||
      (cell->cellId != rlsRnti->cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : No cellCb found with cellId for RNTI:%d", 
		         rlsRnti->rnti);
      return RFAILED;
   }
   /* Fix : syed Clearing UE context when SCH indicates to do so
    * UE DEL from CRG interface is now dummy. */
   if (rlsRnti->ueIdChng)
   {
      /* Fix : syed ueId change as part of reestablishment.
       * Now SCH to trigger this. CRG ueRecfg for ueId change 
       * is dummy */	   
      if (rgUtlHndlCrntiChng(macInst,cell, rlsRnti->rnti, rlsRnti->newRnti) != ROK)	   
      {
	  DU_LOG("\nERROR  -->  MAC : CRNTI change failed for RNTI:%d new RNTI:%d",
                   rlsRnti->rnti,rlsRnti->newRnti);
         return RFAILED;
      }

#ifdef LTE_ADV
      /*PMAC_Reest: Prepare CrntiChngReq and then send to all SMACs to change
       *rnti in all Scells
       */
      if(rgUtlSndCrntiChngReq2AllSMacs(cell, rlsRnti->rnti, rlsRnti->newRnti) != ROK)
      {
         /* TODO: do we need to send DelInd to SCH in failure case*/ 
         return RFAILED;
      }
#endif
#ifdef L2_OPTMZ
      /* Sending delDatReq to CL to clear the Pdus for old UeId present in CL*/
      delDatReq.cellId = cell->cellId;
      delDatReq.ueId = rlsRnti->rnti;
      rgLIMTfuDelDatReq(macInst, &delDatReq);
#endif
   }
   else
   {
      rgUtlHndlCrntiRls(cell, rlsRnti);
   }
   /* Fix : syed Send delete confirmation to SCH */
   /* Send RgMacSchUeDelInd to SCH only if it is Rnti release to PMAC.
    * Basically dont send DelInd to SCH incase of Ue SCell Del*/
#ifdef LTE_ADV
   if(FALSE == rlsRnti->isUeSCellDel)
#endif
   {
      //TODO: commented for compilation without SCH 
#if 0
      ueDelInd.cellSapId  = cell->schInstMap.cellSapId;
      ueDelInd.cellId  = rlsRnti->cellId;
      ueDelInd.rnti    = rlsRnti->rnti; 
      rgGetPstToInst(&schPst,macInst, cell->schInstMap.schInst);
      RgMacSchUeDel(&schPst, &ueDelInd);
#endif
   }

   return ROK;
} /* end of RgSchMacRlsRntiReq */

#ifdef L2_OPTMZ
Bool RgUtlIsTbMuxed(TfuDatReqTbInfo *tb)
{
   MsgLen len = 0;
   SFndLenMsg(tb->macHdr, &len);
   return (len?TRUE : FALSE);
}
#endif

/**********************************************************************
 
         End of file
**********************************************************************/
