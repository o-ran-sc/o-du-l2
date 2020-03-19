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

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=179;

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE */

#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"    /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg_prg.h"        /* PRG(MAC-MAC) Interface includes */
#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* Timer */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"    /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */
#include "rg_prg.x"        /* PRG(MAC-MAC) Interface includes */

#include "du_mgr_mac_inf.h"
#include "rg.x"            /* MAC includes */

/* local defines */
#define RG_NON_MIMO_IDX 0

/* local typedefs */
 
/* local externs */
 
/* forward references */
PRIVATE S16 rgUtlHndlCrntiChng ARGS((
         Inst            inst,
         RgCellCb        *cell,
         CmLteRnti       rnti,
         CmLteRnti       newRnti
         ));
PRIVATE Void rgUtlHndlCrntiRls ARGS((
         RgCellCb        *cell,
         RgInfRlsRnti    *rlsRnti
         ));

PUBLIC S16 rgDelUeFrmAllSCell ARGS((
         RgCellCb       *cell,
         RgUeCb         *ue
         ));

#ifdef LTE_ADV
PRIVATE S16 rgUtlSndCrntiChngReq2AllSMacs ARGS((
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
#ifdef ANSI
PUBLIC S16 rgAllocShrablSBuf
(
Inst    inst,
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
#else
PUBLIC S16 rgAllocShrablSBuf(inst,pData, size)
Inst    inst;
Data    **pData;            /* Pointer of the data to be returned */
Size    size;               /* size */
#endif
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC2(rgAllocShrablSBuf)

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      RETVALUE(RFAILED);
   }

   /* allocate buffer */
   if (SGetStaticBuffer(rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, pData, size, 0) != ROK)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb[inst].rgInit.region;
     dgn.u.mem.pool = rgCb[inst].rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(inst,ERRCLS_DEBUG, ERG028, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }

#ifndef ALIGN_64BIT
   RGDBGINFO(inst,(rgPBuf(inst), "SGetSBuf(Region (%d), Pool (%d), Size (%ld)), Data (0x%p))\n",
             rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, size, *pData));
#else
   RGDBGINFO(inst,(rgPBuf(inst), "SGetSBuf(Region (%d), Pool (%d), Size (%d)), Data (0x%p))\n",
             rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, size, *pData));
#endif

   /* zero out the allocated memory */
   cmMemset((U8 *)*pData, 0x00, size);

   RETVALUE(ROK);

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
#ifdef ANSI
PUBLIC S16 rgAllocSBuf
(
Inst    inst,
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
#else
PUBLIC S16 rgAllocSBuf(inst,pData, size)
Inst    inst;
Data    **pData;            /* Pointer of the data to be returned */
Size    size;               /* size */
#endif
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC2(rgAllocSBuf)

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      RETVALUE(RFAILED);
   }

   /* allocate buffer */
#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   if (SGetSBuf(rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, pData, size) != ROK)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb[inst].rgInit.region;
     dgn.u.mem.pool = rgCb[inst].rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(inst,ERRCLS_DEBUG, ERG028, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }

   /* zero out the allocated memory */
   cmMemset((U8 *)*pData, 0x00, size);

   RETVALUE(ROK);

} /* end of rgAllocSBuf */

/*
*
*       Fun:   rgFreeSharableSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   RETVOID
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgFreeSharableSBuf
(
Inst inst,
Data **data,         /* address of pointer to data */
Size size            /* size */
)
#else
PUBLIC Void rgFreeSharableSBuf(inst,data, size)
Inst inst;
Data **data;         /* address of pointer to data */
Size size;          /* size */
#endif
{

   S16 ret;

   TRC2(rgFreeSharableBuf)

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      RETVOID;
   }

   /* Deallocate buffer */
   ret = SPutStaticBuffer(rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, *data, size, SS_SHARABLE_MEMORY);

   if (ret != ROK)
   {
      RETVOID;
   }

   *data = NULLP;

   RETVOID;

} /* end of rgFreeSharableBuf */



/*
*
*       Fun:   rgFreeSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   RETVOID
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgFreeSBuf
(
Inst inst,
Data **data,         /* address of pointer to data */
Size size            /* size */
)
#else
PUBLIC Void rgFreeSBuf(inst,data, size)
Inst  inst;
Data **data;         /* address of pointer to data */
Size size;          /* size */
#endif
{

   S16 ret;

   TRC2(rgFreeSBuf)

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      RETVOID;
   }


#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_CALLER();
#endif /* */
   /* Deallocate buffer */
   ret = SPutSBuf(rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, *data, size);

   if (ret != ROK)
   {
      RGLOGERROR(inst,ERRCLS_DEBUG, ERG029, (ErrVal) 0, "rgFreeSBuf failed.\n");
      RETVOID;
   }

   *data = NULLP;

   RETVOID;

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
#ifdef ANSI
PUBLIC S16 rgGetMsg
(
Inst    inst,
Buffer  **mBuf            /* Message Buffer pointer be returned */
)
#else
PUBLIC S16 rgGetMsg(inst,mBuf)
Inst    inst;
Buffer  **mBuf;           /* Message Buffer pointer be returned */
#endif
{
   S16         ret;

   TRC2(rgGetMsg)

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   ret = SGetMsg(RG_GET_MEM_REGION(rgCb[inst]), RG_GET_MEM_POOL(rgCb[inst]), mBuf);

   if (ROK != ret) 
   {
      /* Moving diagnostics structure to limited scope for optimization */
      RgUstaDgn   dgn;      /* Alarm diagnostics structure */

      rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM);

      /*  Send an alarm to Layer Manager */
      rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(inst,ERRCLS_DEBUG, ERG030, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

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
#ifdef ANSI
PUBLIC Void rgFillDgnParams
(
Inst        inst,
RgUstaDgn   *dgn,
U8          dgnType
)
#else
PUBLIC Void rgFillDgnParams(inst,dgn, dgnType)
Inst        inst;
RgUstaDgn   *dgn;
U8          dgnType;
#endif
{

   TRC2(rgFillDgnParams)

   switch(dgnType)
   {
      case LRG_USTA_DGNVAL_MEM:
         dgn->type = (U8) LRG_USTA_DGNVAL_MEM;
         dgn->u.mem.region  = rgCb[inst].rgInit.region;
         dgn->u.mem.pool    = rgCb[inst].rgInit.pool;
      break;

      default:
      break;
   }

   RETVOID;
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
#ifdef ANSI
PUBLIC Void rgUpdtRguDedSts
(
Inst           inst,
RgUpSapCb     *rguDlSap,
U8             stsType,   /* Statistics type to update */
RgRguDedDatReq *datReq    /* DatReq pointer */
)
#else
PUBLIC Void rgUpdtRguDedSts(inst,rguDlSap,stsType, datReq)
Inst           inst;
RgUpSapCb     *rguDlSap;
U8             stsType;   /* Statistics type to update */
RgRguDedDatReq *datReq;   /* DatReq pointer */
#endif
{
   U8 idx1,idx2;
   U32 idx;


   TRC2(rgUpdtRguDedSts)


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
   
   RETVOID;
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
#ifdef ANSI
PUBLIC Void rgUpdtRguCmnSts
(
Inst           inst,
RgUpSapCb     *rguDlSap,
U8             stsType   /* Statistics type to update */
)
#else
PUBLIC Void rgUpdtRguCmnSts(inst,rguDlSap,stsType)
Inst           inst;
RgUpSapCb     *rguDlSap;
U8             stsType;   /* Statistics type to update */
#endif
{
   TRC2(rgUpdtRguCmnSts)



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
   
   RETVOID;
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
#ifdef ANSI
PUBLIC Void rgUpdtCellCnt
(
Inst inst,
U8 updtType
)
#else
PUBLIC Void rgUpdtCellCnt(inst,updtType)
Inst inst;
U8 updtType;
#endif
{
   TRC2(rgUpdtCellCnt);

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

   RETVOID;
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
#ifdef ANSI
PUBLIC Void rgUpdtUeCnt
(
Inst inst,
U8 updtType
)
#else
PUBLIC Void rgUpdtUeCnt (inst,updtType)
Inst inst;
U8 updtType;
#endif
{
   TRC2(rgUpdtUeCnt);

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
   RETVOID;
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
#ifdef ANSI
PUBLIC S16 rgAllocEventMem
(
Inst     inst,
Ptr       *memPtr,
Size      memSize
)
#else
PUBLIC S16 rgAllocEventMem(inst,memPtr, memSize)
Inst     inst;
Ptr       *memPtr;
Size      memSize;
#endif
{
   Mem              sMem;
   VOLATILE U32     startTime=0;

   TRC2(rgAllocEventMem)

   sMem.region = rgCb[inst].rgInit.region;
   sMem.pool = rgCb[inst].rgInit.pool;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (memSize<= 0)
   {
      RGLOGERROR(inst,ERRCLS_INT_PAR, ERG031, memSize,
                   "rgAllocEventMem(): memSize invalid\n");
      RETVALUE (RFAILED);
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
      RLOG0(L_ERROR,"cmAllocEvnt Failed"); 
      RETVALUE(RFAILED);
   }

   /*stoping Task*/
   SStopTask(startTime, PID_MACUTL_CMALLCEVT);

   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 rgGetEventMem
(
Inst      inst,
Ptr       *ptr,
Size      len,
Ptr       memCp
)
#else
PUBLIC S16 rgGetEventMem(inst,ptr, len, memCp)
Inst      inst;
Ptr       *ptr;
Size      len;
Ptr       memCp;
#endif
{
   S16   ret;

   TRC2(rgGetEventMem)
#ifdef TFU_ALLOC_EVENT_NO_INIT
   ret = cmGetMemNoInit(memCp, len, (Ptr *)ptr);
#else
   ret = cmGetMem(memCp, len, (Ptr *)ptr);
#endif /* */
   RETVALUE(ret);
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
#ifdef ANSI
PUBLIC Void rgGetPstToInst
(
Pst           *pst,
Inst          srcInst,
Inst          dstInst
)
#else
PUBLIC Void rgGetPstToInst (pst, srcInst, dstInst)
Pst           *pst;
Inst          srcInst;
Inst          dstInst;
#endif
{
   TRC2(rgGetPstToInst);

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

   RETVOID; 
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
#ifdef ANSI
PUBLIC S16 RgSchMacLcgRegReq
(
Pst            *pst,
RgInfLcgRegReq *lcgRegReq
)
#else
PUBLIC S16 RgSchMacLcgRegReq (pst, lcgRegReq)
Pst            *pst;
RgInfLcgRegReq *lcgRegReq;
#endif
{
   Inst       inst;
   RgCellCb   *cell = NULLP;
   RgUeCb     *ue;

   TRC2(RgSchMacLcgRegReq);

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP) ||
      (cell->cellId != lcgRegReq->cellId))
   {
      
      RLOG_ARG0(L_ERROR,DBG_CELLID,lcgRegReq->cellId,"Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, lcgRegReq->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cell->cellId,"CRNTI:%d does not exist", 
		         lcgRegReq->crnti);
      RETVALUE(RFAILED);
   }
   ue->ul.lcgArr[lcgRegReq->lcgId].isGbr = lcgRegReq->isGbr;

   RETVALUE(ROK); 
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
#ifdef ANSI
PUBLIC S16 RgSchMacUlSpsResetReq
(
Pst            *pst,
RgInfUlSpsReset *ulSpsResetInfo
)
#else
PUBLIC S16 RgSchMacUlSpsResetReq (pst, lcInfo)
Pst            *pst;
RgInfUlSpsReset *ulSpsResetInfo;
#endif
{
   Inst       inst;
   RgCellCb   *cell = NULLP;
   RgUeCb     *ue;

   TRC2(RgSchMacUlSpsResetReq);

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP)||
      (cell->cellId != ulSpsResetInfo->cellId))
   {
      
      RLOG_ARG0(L_ERROR, DBG_CELLID,ulSpsResetInfo->cellId,"Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, ulSpsResetInfo->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cell->cellId,"CRNTI:%d does not exist", 
		      ulSpsResetInfo->crnti);
      RETVALUE(RFAILED);
   }

   ue->ul.implRelCntr = 0;
   ue->ul.explRelCntr = 0;

   RETVALUE(ROK); 
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
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcRegReq
(
Pst            *pst,
RgInfSpsLcInfo *lcInfo
)
#else
PUBLIC S16 RgSchMacSpsLcRegReq (pst, lcInfo)
Pst            *pst;
RgInfSpsLcInfo *lcInfo;
#endif
{
   Inst       inst;
   RgCellCb   *cell= NULLP;
   RgUeCb     *ue;
   U8         idx;

   TRC2(RgSchMacSpsLcRegReq);

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP) ||
      (cell->cellId != lcInfo->cellId))
   {
      
      RLOG_ARG0(L_ERROR,DBG_CELLID,lcInfo->cellId, "Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, lcInfo->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cell->cellId,"CRNTI:%d does not exist", 
		      lcInfo->crnti);
      RETVALUE(RFAILED);
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
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
	            "Ue insertion into SPS list failed SPS CRNTI:%d", ue->spsRnti);
      RETVALUE(RFAILED);
   } 

   RETVALUE(ROK); 
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
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcDeregReq
(
Pst            *pst,
CmLteCellId    cellId,
CmLteRnti      crnti
)
#else
PUBLIC S16 RgSchMacSpsLcDeregReq (pst, cellId, crnti)
Pst            *pst;
CmLteCellId    cellId;
CmLteRnti      crnti;
#endif
{
   Inst       inst;
   RgCellCb   *cell = NULLP;
   RgUeCb      *ue;

   TRC2(RgSchMacSpsLcDeregReq);

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[inst].cell;
   /* Fetch the cell and then the UE */
   if((cell == NULLP) ||
      (cell->cellId != cellId))
   {
      
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId, "Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cellId,"CRNTI:%d Ue does not exist", crnti);
      RETVALUE(RFAILED);
   }

   /* No need to reset the SPS LC Ids as they will not be looked at*/

   /* Delete UE from the SPS UE List */
   rgDBMDelSpsUeCb(cell, ue);
   
   RETVALUE(ROK); 
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
#ifdef ANSI
PRIVATE S16 rgUtlHndlCrntiChng 
(
Inst            inst,
RgCellCb        *cell,
CmLteRnti       rnti,
CmLteRnti       newRnti
)
#else
PRIVATE S16 rgUtlHndlCrntiChng(inst,cell, rnti, newRnti)
Inst            inst;
RgCellCb        *cell;
CmLteRnti       rnti;
CmLteRnti       newRnti;
#endif
{
   RgUeCb         *ue = NULLP;
   RgUeCb         *newUe = NULLP;

   TRC3(rgUtlHndlCrntiChng)

   ue = rgDBMGetUeCb(cell, rnti);
   newUe = rgDBMGetUeCbFromRachLst(cell, newRnti);
   if ((ue == NULLP) || (newUe == NULLP))
   {
      RLOG_ARG4(L_ERROR,DBG_CELLID,cell->cellId,
         		"RNTI:%d Failed to get UECB[%lu:%lu] or NEW RNTI:%d", 
               rnti, ((PTR)ue), ((PTR)newUe), newRnti);
      RETVALUE(RFAILED);
   }
#ifdef XEON_SPECIFIC_CHANGES
   CM_LOG_DEBUG(CM_LOG_ID_MAC, "MAC:UE[%d] id changed to %d\n", rnti, newRnti);
#endif
   rgDBMDelUeCb(cell, ue);

   ue->ueId = newRnti;

   cmMemcpy((U8*)&(ue->contResId), (U8*)&(newUe->contResId), 
		   sizeof(newUe->contResId));
   /* Fix : syed MSG4 might be RETXing need to store the
    * HARQ context. */
   rgDHMFreeUe(inst,&ue->dl.hqEnt);
   ue->dl.hqEnt = newUe->dl.hqEnt;
       
   rgDBMInsUeCb(cell, ue);

   rgDBMDelUeCbFromRachLst(cell, newUe);
   rgFreeSBuf(inst,(Data **)&newUe, sizeof(*newUe));

   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 rgDelUeFrmAllSCell
(
RgCellCb      *cell,
RgUeCb        *ue
)
#else
PUBLIC S16 rgDelUeFrmAllSCell(cell, ue)
RgCellCb      *cell;
RgUeCb        *ue;
#endif
{
   Inst        inst     = cell->macInst - RG_INST_START;
   U8          idx      = 0;
   Inst        sCellInstIdx;
   Pst         dstInstPst;
   RgPrgUeSCellDelInfo ueSCellDelInfo;

   TRC2(rgDelUeFrmAllSCell)

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

   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 rgUtlVltdAddSCellCfg
(
 RgPrgUeSCellCfgInfo *ueSCellCb,
 RgCellCb    *cell,
 Inst        inst
)
#else
PUBLIC S16 rgUtlVltdAddSCellCfg(ueSCellCb, cell, inst)
 RgPrgUeSCellCfgInfo *ueSCellCb;
 RgCellCb    *cell;
 Inst        inst;
#endif
{
  S16 ret = ROK; 
  
  TRC3(rgUtlVltdAddSCellCfg)
  
     /* To Validate the CellID presence */
  if((cell == NULLP) ||
        (cell->cellId != ueSCellCb->cellId))
  {
     RGDBGERRNEW(inst, (rgPBuf(inst),
              "[%d]Sec Cell does not exit %d\n",
              ueSCellCb->ueId, ueSCellCb->cellId));
     ret = RFAILED;
  }
#ifdef TENB_MULT_CELL_SUPPRT
   if((ueSCellCb->rguDlSapId > rgCb[inst].numRguSaps) ||
      (ueSCellCb->rguUlSapId > rgCb[inst].numRguSaps))
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "Invald Sap Id: DL %d UL %d for ueId %d failed\n",
               ueSCellCb->rguDlSapId,
               ueSCellCb->rguUlSapId,
               ueSCellCb->cellId));
     ret = RFAILED;
   }
#endif
   RETVALUE(ret);
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
#ifdef ANSI
PRIVATE S16 rgUtlSndCrntiChngReq2AllSMacs
(
RgCellCb        *cell,
CmLteRnti       rnti,
CmLteRnti       newRnti
)
#else
PRIVATE S16 rgUtlSndCrntiChngReq2AllSMacs(cell, rnti, newRnti)
RgCellCb        *cell;
CmLteRnti       rnti;
CmLteRnti       newRnti;
#endif
{
   Inst                inst = cell->macInst - RG_INST_START;
   Inst                sCellInstIdx;
   Pst                 dstInstPst;
   RgPrgUeSCellDelInfo ueIdChngReq;
   RgUeCb              *ue;
   U8                  idx;
#ifdef L2_OPTMZ
TfuDelDatReqInfo delDatReq;
#endif

   TRC2(rgUtlSndCrntiChngReq2AllSMacs)

   /* use newRnti to get UeCb in PMac because rnti is already changed in PMac*/
   ue = rgDBMGetUeCb(cell, newRnti);
   if (ue == NULLP)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst),"[%d]RNTI:Failed to get ueCb \
               newRnti=%d\n", rnti, newRnti));
      RETVALUE(RFAILED);
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

   RETVALUE(ROK);
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
#ifdef ANSI
PRIVATE Void rgUtlHndlCrntiRls
(
RgCellCb        *cell,
RgInfRlsRnti    *rlsRnti
)
#else
PRIVATE Void rgUtlHndlCrntiRls(cell, rlsRnti)
RgCellCb        *cell;
CmLteRnti       *rlsRnti;
#endif
{
   Inst           inst = cell->macInst - RG_INST_START;
   RgUeCb        *ue = NULLP;
#ifdef LTEMAC_SPS
   RgUeCb         *spsUeCb = NULLP;
#endif

   TRC3(rgUtlHndlCrntiRls)

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
         RLOG_ARG1(L_WARNING,DBG_CELLID,cell->cellId,
                   "RNTI:%d No ueCb found in RachLst",rlsRnti->rnti);
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
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
	      	      	"RNTI:%d STALE UE is still present", rlsRnti->rnti);         
         }
      }
   }

   RETVOID;
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
#ifdef ANSI
PUBLIC S16 RgSchMacRlsRntiReq
(
Pst                 *pst,
RgInfRlsRnti        *rlsRnti
)
#else
PUBLIC S16 RgSchMacRlsRntiReq(pst, rlsRnti)
Pst                 *pst;
RgInfRlsRnti        *rlsRnti;
#endif
{
//   Pst            schPst;
//   RgInfUeDelInd  ueDelInd;
   Inst           macInst;
   RgCellCb       *cell;
#ifdef L2_OPTMZ
TfuDelDatReqInfo delDatReq;
#endif

   TRC3(RgSchMacRlsRntiReq)

   RG_IS_INST_VALID(pst->dstInst);
   macInst   = pst->dstInst - RG_INST_START;
   cell   = rgCb[macInst].cell;

   if(NULLP == rlsRnti)
   {
      RETVALUE(RFAILED);
   }

   if((cell == NULLP) ||
      (cell->cellId != rlsRnti->cellId))
   {
      
      RLOG_ARG1(L_ERROR,DBG_CELLID,rlsRnti->cellId,
               "No cellCb found with cellId for RNTI:%d", 
		         rlsRnti->rnti);
      RETVALUE(RFAILED);
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
         RLOG_ARG2(L_ERROR,DBG_CELLID,rlsRnti->cellId,
	                "CRNTI change failed for RNTI:%d new RNTI:%d",
                   rlsRnti->rnti,rlsRnti->newRnti);
         RETVALUE(RFAILED);
      }

#ifdef LTE_ADV
      /*PMAC_Reest: Prepare CrntiChngReq and then send to all SMACs to change
       *rnti in all Scells
       */
      if(rgUtlSndCrntiChngReq2AllSMacs(cell, rlsRnti->rnti, rlsRnti->newRnti) != ROK)
      {
         /* TODO: do we need to send DelInd to SCH in failure case*/ 
         RETVALUE(RFAILED);
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

   RETVALUE(ROK);
} /* end of RgSchMacRlsRntiReq */

#ifdef L2_OPTMZ
#ifdef ANSI
PUBLIC Bool RgUtlIsTbMuxed
(
 TfuDatReqTbInfo *tb
)
#else
PUBLIC Bool RgUtlIsTbMuxed()
   TfuDatReqTbInfo *tb
#endif
{
   MsgLen len = 0;
   SFndLenMsg(tb->macHdr, &len);
   RETVALUE(len?TRUE : FALSE);
}
#endif

/**********************************************************************
 
         End of file
**********************************************************************/
