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
  
     File:     rg_tom.c 
  
**********************************************************************/

/** @file rg_tom.c 
@brief This module does processing related to handling of lower interface APIs 
invoked by PHY towards MAC
*/
/* header include files -- defines (.h) */
#include "common_def.h"
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "rg_env.h"            /* defines and macros for MAC */
#include "rg_err.h"            /* defines and macros for MAC */
#include "rgm.h"           /* layer management typedefs for MAC */

/* header/extern include files (.x) */
#include "crg.x"           /* CRG interface typedefs */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"        /* PRG interface typedefs */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg.x"            /* typedefs for MAC */
#ifdef MAC_RLC_UL_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif

/* ADD Changes for Downlink UE Timing Optimization */
#ifndef LTEMAC_DLUE_TMGOPTMZ 
static S16 rgTOMUtlProcDlSf ARGS(( RgDlSf *dlSf, RgCellCb   *cellCb,
                                    RgErrInfo  *err));
#else
S16 rgTOMUtlProcDlSf ARGS((RgDlSf *dlSf, RgCellCb *cellCb,
                                  RgErrInfo  *err));
#endif
static S16 rgTOMProcCrntiCEInDatInd ARGS((
RgMacPdu          *pdu,
RgUeCb            *prevUeCb,
RgCellCb          *cellCb,
TfuDatInfo        *datInfo,
RgInfCeInfo       *ceInfo,
uint16_t          slot
));

static S16 rgTOMProcCCCHSduInDatInd ARGS((
RgMacPdu          *pdu,
RgUeCb            *prevUeCb,
RgCellCb          *cellCb,
TfuDatInfo        *datInfo,
RgInfCeInfo       *ceInfo,
uint16_t           slot
));

S16 rgHndlFlowCntrl
(
RgCellCb       *cell,
RgInfSfAlloc        *sfInfo
);

S16 RgUiRguFlowCntrlInd(Pst* pst, SuId suId, RguFlowCntrlInd *flowCntrlInd);
#ifdef EMTC_ENABLE
S16 rgEmtcHndl(RgCellCb *cell,RgInfSfAlloc  *sfInfo);  
S16 rgTOMEmtcUtlFillDatReqPdus(TfuDatReqInfo *datInfo, RgDlSf *dlSf, RgCellCb *cell, RgErrInfo *err);  
Void rgTOMEmtcRlsSf(RgDlSf *dlSf);  
#endif
#ifdef LTE_L2_MEAS
static Void rgTOML2MCompileActiveLCs ARGS
((
 RgCellCb      *cellCb, 
 RgUeCb        *ueCb,
 RgMacPdu      *pdu,
 RgInfCeInfo   *ceInfo 
 ));
static S16 rgTOMUtlL2MStoreBufSz ARGS
((
 RgUeCb      *ueCb,
 RgInfCeInfo *ceInfo
 ));

static S16 rgTomUtlPrepareL2MUlThrpInfo ARGS
((
   RgCellCb *cellCb,
   RgUeCb  *ueCb,
   RgRguDedDatInd  *dDatInd
));


/* The below table takes lower values of BSR Range for a BSR value
     This is to ensure that outstanding can be decrease to zero upon reception of
     TB, which is not guaranteed if higher Range values are used */
   /* Note: taking value 10 for BSR index 1 */
#ifndef MAC_5GTF_UPDATE
static uint32_t rgLwrBsrTbl[64] = {
   0, 10, 10, 12, 14, 17, 19, 22, 26,
   31, 36, 42, 49, 57, 67, 78, 91,
   107, 125, 146, 171, 200, 234, 274, 321,
   376, 440, 515, 603, 706, 826, 967, 1132,
   1326, 1552, 1817, 2127, 2490, 2915, 3413, 3995,
   4677, 5476, 6411, 7505, 8787, 10287, 12043, 14099,
   16507, 19325, 22624, 26487, 31009, 36304, 42502, 49759,
   58255, 68201, 79846, 93479, 109439, 128125, 150000
};
#else

static uint32_t rgLwrBsrTbl[64] = {
0,10,13,16,19,23,29,35,43,53,65,80,98,120,147,181,223,274,337,414,
509,625,769,945,1162,1429,1757,2161,2657,3267,4017,4940,6074,7469,
9185,11294,13888,17077,20999,25822,31752,39045,48012,59039,72598,
89272,109774,134986,165989,204111,250990,308634,379519,466683,
573866,705666,867737,1067031,1312097,1613447,1984009,2439678,
3000000};

#endif

#endif

/* local defines */
#define RG_TOM_INF_ALLOC(_pdu, _size, _dataPtr, _ret) {\
   _ret = cmGetMem((Ptr)&_pdu->memCp, _size, (Ptr *)&_dataPtr); \
}

/* global variables */
uint32_t rgUlrate_tfu;
#ifdef EMTC_ENABLE
uint32_t grgUlrate_tfu;
#endif

/** @brief This function fills the PDSCH data of a downlink subframe 
 *
 * @details
 *
 *     Function: rgTOMUtlFillDatReqPdus 
 *
 *         Processing steps:
 *         - Fill BCCH on DLSCH data using datInfo
 *         - Fill PCCH on DLSCH data using datInfo
 *         - Fill Dedicated data on DLSCH data using datInfo
 *         - Fill RA RSP data using datInfo
 *
 * @param  [out] TfuDatReqInfo *datInfo 
 * @param  [in]  RgDlSf     *dlSf
 * @param  [in]  RgCellCb   *cellCb
 * @param  [out] RgErrInfo *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgTOMUtlFillDatReqPdus (TfuDatReqInfo *datInfo,RgDlSf *dlSf,RgCellCb *cellCb, RgErrInfo *err)
{
   S16              ret;
   TfuDatReqPduInfo *datReq=NULLP;
   /* Moving node declaration to limited scope for optimization */
   RgDlHqProcCb     *hqCb;
   uint8_t               idx;
   Inst             inst = cellCb->macInst - RG_INST_START;


   /* first lets send the BCCH data down to PHY */
   if (dlSf->bcch.tb != NULLP)
   {
      if ((ret = rgGetEventMem(inst,(Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
		  &(datInfo->memCp))) != ROK)
      {
	 err->errCause = RGERR_TOM_MEM_EXHAUST;
	 DU_LOG("\nERROR  -->  MAC : Memory Exhaustion ");
	 return (ret);
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
#ifndef L2_OPTMZ      
      datReq->mBuf[1] = 0;
#else 
      datReq->tbInfo[0].lchInfo[0].mBuf[0]=NULLP;
#endif
#endif
      datReq->rnti                   =  RG_SI_RNTI;
      datReq->dciInfo                =  dlSf->bcch.pdcch.dci;
      /* Note: SCpyMsgMsg is not done since free of unsent buffer 
       * has been taken care through cell delete by invoking rgTomRlsSf
       * during shutdown */
      datReq->nmbOfTBs               =  1;
#ifndef L2_OPTMZ
      datReq->mBuf[0]                =  dlSf->bcch.tb;
#else
      SFndLenMsg((Buffer *)dlSf->bcch.tb, &(datReq->tbInfo[0].tbSize));
      datReq->tbInfo[0].tbPres             = TRUE;
      datReq->tbInfo[0].lchInfo[0].mBuf[0] = dlSf->bcch.tb;
      datReq->tbInfo[0].numLch             = 1;
      datReq->tbInfo[0].lchInfo[0].numPdu  = 1;

#ifdef TFU_ALLOC_EVENT_NO_INIT
      datReq->tbInfo[1].tbPres             = FALSE;
      datReq->tbInfo[1].lchInfo[0].mBuf[0] = NULLP;
#endif
#endif
      cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
      datReq->lnk.node = (PTR)datReq;
#ifdef TFU_UPGRADE      
      /* ccpu00132314-ADD- Adding txPower offset for the PDSCH transmission */
      datReq->txPwrOffset            = dlSf->bcch.txPwrOffset;
#endif      
      /* Setting the pointer to NULL post transmission */
      dlSf->bcch.tb = NULLP;
   }
   /* Fill PCCH data */
   if (dlSf->pcch.tb != NULLP)
   {
      if ((ret = rgGetEventMem(inst,(Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
		  &(datInfo->memCp))) != ROK)
      {
	 err->errCause = RGERR_TOM_MEM_EXHAUST;
	 DU_LOG("\nERROR  -->  MAC : Memory Exhaustion CRNTI:%d",datReq->rnti);
	 return (ret);
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
#ifndef L2_OPTMZ      
      datReq->mBuf[1] = 0;
#endif     
#endif     
      datReq->rnti                   =  RG_P_RNTI;
      datReq->dciInfo                = dlSf->pcch.pdcch.dci;
      datReq->nmbOfTBs               =  1;
#ifndef L2_OPTMZ
      datReq->mBuf[0]                =  dlSf->pcch.tb;
#else
      SFndLenMsg((Buffer *)dlSf->pcch.tb, &datReq->tbInfo[0].tbSize);
      datReq->tbInfo[0].tbPres             = TRUE;
      datReq->tbInfo[0].lchInfo[0].mBuf[0] =  dlSf->pcch.tb;
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datReq->tbInfo[1].tbPres             = FALSE;
      datReq->tbInfo[1].lchInfo[0].mBuf[0] = NULLP;
#endif
      datReq->tbInfo[0].numLch             = 1;
      datReq->tbInfo[0].lchInfo[0].numPdu  = 1;
#endif
      cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
      datReq->lnk.node = (PTR)datReq;
#ifdef TFU_UPGRADE      
      /* ccpu00132314-ADD- Adding txPower offset for the PDSCH transmission */
      datReq->txPwrOffset            = dlSf->pcch.txPwrOffset;
#endif      
      dlSf->pcch.tb = NULLP;
   }

   for(idx=0; idx < dlSf->numRaRsp; idx++)
   {
      if ((ret = rgGetEventMem(inst,(Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
		  &(datInfo->memCp))) != ROK)
      {
	 err->errCause = RGERR_TOM_MEM_EXHAUST;
	 DU_LOG("\nERROR  -->  MAC : Memory Exhaustion CRNTI:%d",
	       datReq->rnti);
	 return (ret);
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
#ifndef L2_OPTMZ      
      datReq->mBuf[1] = 0;
#endif    
#endif    
      datReq->rnti                   =  dlSf->raRsp[idx].pdcch.rnti;
      datReq->dciInfo                = dlSf->raRsp[idx].pdcch.dci;
      datReq->nmbOfTBs               =  1;
#ifndef L2_OPTMZ
      datReq->mBuf[0]                =  dlSf->raRsp[idx].rar;
#else
      SFndLenMsg((Buffer *)dlSf->raRsp[idx].rar, &datReq->tbInfo[0].tbSize);
      datReq->tbInfo[0].tbPres             = TRUE;
      datReq->tbInfo[0].lchInfo[0].mBuf[0] =  dlSf->raRsp[idx].rar;
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datReq->tbInfo[1].lchInfo[0].mBuf[0] = NULLP;
      datReq->tbInfo[1].tbPres             = FALSE;
#endif
      datReq->tbInfo[0].numLch             = 1;
      datReq->tbInfo[0].lchInfo[0].numPdu  = 1;
      //   prc_trace_format_string(0x40,3,"UE Id=(%d) tbSz=(%d)",datReq->rnti, datReq->tbInfo[0].tbSize);
#endif
      cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
      datReq->lnk.node = (PTR)datReq;
#ifdef TFU_UPGRADE      
      /* ccpu00132314-ADD- Adding txPower offset for the PDSCH transmission */
      datReq->txPwrOffset            = dlSf->raRsp[idx].txPwrOffset;
#endif
      dlSf->raRsp[idx].rar = NULLP;
   }

   /* Fill Dedicated UE data */
   if (dlSf->tbs.count != 0) 
   {
      CmLList          *node;
      while (dlSf->tbs.first)
      {
	 node = dlSf->tbs.first;
	 hqCb = (RgDlHqProcCb*)node->node;
	 if ((ret = rgDHMSndDatReq (cellCb, dlSf, datInfo, hqCb, err)) != ROK)
	 {
	    DU_LOG("\nERROR  -->  MAC : DHM unable to fill DATA request");
	    err->errType = RGERR_TOM_TTIIND;
	    continue;
	 }
      } /* end of while */
   } 

   return ROK;
} /* end of rgTOMUtlFillDatReqPdus*/ 

/** @brief This function does all the processing related to a single downlink
 * subframe.
 *
 * @details 
 *
 *     Function: rgTOMUtlProcDlSf
 *
 *         Processing steps:
 *         - collate control data for all UEs and send to PHY 
 *         - collate data buffers for all UEs and send to PHY 
 *
 * @param  [in] RgDlSf     *dlSf
 * @param  [in] RgCellCb   *cellCb
 * @param  [out] RgErrInfo *err
 * @return S16
 */
/* ADD Changes for Downlink UE Timing Optimization */
#ifndef LTEMAC_DLUE_TMGOPTMZ 
static S16 rgTOMUtlProcDlSf(RgDlSf *dlSf,RgCellCb *cellCb,RgErrInfo *err)
#else
S16 rgTOMUtlProcDlSf(RgDlSf *dlSf,RgCellCb *cellCb,RgErrInfo *err)
#endif
{
   S16               ret;
   TfuDatReqInfo     *datInfo;
   Inst              inst = cellCb->macInst - RG_INST_START;


   /* Fill Data Request Info from scheduler to PHY */   
   if ((ret = rgAllocEventMem(inst,(Ptr *)&datInfo, 
	       sizeof(TfuDatReqInfo))) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Unable to Allocate TfuDatReqInfo");
      return (ret);
   }
   else
   {
      cmLListInit(&datInfo->pdus);
#ifdef LTE_TDD
      RGADDTOCRNTTIME(dlSf->schdTime, datInfo->timingInfo, TFU_DELTA);
#else
      RGADDTOCRNTTIME(dlSf->schdTime, datInfo->timingInfo, TFU_DLDATA_DLDELTA);
#endif
      datInfo->cellId = cellCb->cellId;
      if((0 == (datInfo->timingInfo.sfn % 30)) && (0 == datInfo->timingInfo.slot))
      {
	 //DU_LOG("5GTF_CHECK rgTOMUtlProcDlSf dat (%d : %d)\n", datInfo->timingInfo.sfn, datInfo->timingInfo.slot);
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datInfo->bchDat.pres = 0;
#endif

      /* Fill BCH data */
      if (dlSf->bch.tb != NULLP)
      {
	 datInfo->bchDat.pres = PRSNT_NODEF;
	 datInfo->bchDat.val  = dlSf->bch.tb;
	 dlSf->bch.tb = NULLP;
      }
#ifdef EMTC_ENABLE
      /* Fill the DLSCH PDUs of BCCH, PCCH and Dedicated Channels */
      if ((ret = rgTOMEmtcUtlFillDatReqPdus(datInfo, dlSf, cellCb, err)) != ROK)
      {
	 RG_FREE_MEM(datInfo);
	 return (ret);
      }
#endif 
      /* Fill the DLSCH PDUs of BCCH, PCCH and Dedicated Channels */
      if ((ret = rgTOMUtlFillDatReqPdus(datInfo, dlSf, cellCb, err)) != ROK)
      {
	 DU_LOG("\nERROR  -->  MAC : Unable to send data for cell");
	 RG_FREE_MEM(datInfo);
	 return (ret);
      }
      if((datInfo->pdus.count) || (datInfo->bchDat.pres == TRUE))
      {
	 /* sending the data to Phy */
	 //if (rgLIMTfuDatReq(inst,datInfo) != ROK)
	 {
	    DU_LOG("\nERROR  -->  MAC : Unable to send data info for cell");               
	 }
      }
      else
      {
	 /* Nothing to send: free the allocated datInfo */
	 RG_FREE_MEM(datInfo);
      }
   }
   return ROK;
} /* end of */

uint32_t  rgMacGT;

/** @brief This function allocates the RgMacPdu that will be populated by DEMUX
 * with the SubHeaders list and the values of the Control elements.
 *
 * @details
 *
 *     Function: rgTOMUtlAllocPduEvnt
 *
 *         Processing steps:
 *  @param[in]  Inst        inst
 * @param  [out] RgMacPdu   **pdu
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgTOMUtlAllocPduEvnt (Inst inst,RgMacPdu **pdu)
{

   Mem               evntMem;
   RgUstaDgn         dgn;      /* Alarm diagnostics structure */
   volatile uint32_t startTime=0;


   evntMem.region = rgCb[inst].rgInit.region;
   evntMem.pool   = rgCb[inst].rgInit.pool;

   /*starting Task*/
   SStartTask(&startTime, PID_TOMUTL_CMALLCEVT);

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */

   if (cmAllocEvnt (sizeof (RgMacPdu), RG_BLKSZ, &evntMem, (Ptr*)pdu) != ROK)
   {
      rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
	    LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      DU_LOG("\nERROR  -->  MAC : Allocation of DUX event failed");
      return RFAILED;
   }

   /*stoping Task*/
   SStopTask(startTime, PID_TOMUTL_CMALLCEVT);

   return ROK;
} /* end of */ 

/** @brief This function frees up the RgMacPdu structure that has been
 * populated by demux.
 *
 * @details
 *
 *     Function: rgTOMUtlFreePduEvnt
 *       - Function frees up the RgMacPdu structure, in case of error it shall
 *       free up the buffer's present in the different sdu.
 *
 *         Processing steps:
 * @param  [in]  Inst        inst
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] Bool       *error
 * @return 
 */
static Void rgTOMUtlFreePduEvnt( RgMacPdu *pdu,Bool  error)
{

   RgMacSdu       *sdu;
   CmLList        *node;

   /* Steps of freeing up the PDU.
    * 1. loop through the subHdrLst and free up all the buffers.
    * 2. free up the whole event
    */
   if ((error == TRUE) && (pdu->sduLst.count > 0))
   {
      node =  pdu->sduLst.first;
      while (node)
      {
	 sdu = (RgMacSdu*)node->node;
	 RG_FREE_MSG(sdu->mBuf);
	 node = node->next;
      }
   }
   RG_FREE_MEM(pdu);
   return;
} /* end of rgTOMUtlFreePduEvnt */ 

/** @brief This function allocates the RgMacPdu that will be populated by DEMUX
 * with the SubHeaders list and the values of the Control elements.
 *
 * @details
 *
 *     Function: rgTOMInfAllocPduEvnt
 *
 *         Processing steps:
 * @param  [in] Inst        inst
 * @param  [out] RgMacPdu   **pdu
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgTOMInfAllocPduEvnt (Inst inst,RgInfSfDatInd **sfInfo)
{

   Mem               evntMem;
   RgUstaDgn         dgn;      /* Alarm diagnostics structure */
   volatile uint32_t      startTime=0;


   evntMem.region = rgCb[inst].rgInit.region;
   evntMem.pool   = rgCb[inst].rgInit.pool;

   /*starting Task*/
   SStartTask(&startTime, PID_TOMINF_CMALLCEVT);

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   if (cmAllocEvnt (sizeof (RgInfSfDatInd), RG_BLKSZ, &evntMem, (Ptr*)sfInfo) != ROK)
   {
      rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
	    LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      DU_LOG("\nERROR  -->  MAC : Allocation failed");
      return RFAILED;
   }

   /*stoping Task*/
   SStopTask(startTime, PID_TOMINF_CMALLCEVT);

   return ROK;
} /* end of */ 

/** @brief This function frees up the RgMacPdu structure that has been
 * populated by demux.
 *
 * @details
 *
 *     Function: rgTOMInfFreePduEvnt
 *       - Function frees up the RgMacPdu structure, in case of error it shall
 *       free up the buffer's present in the different sdu.
 *
 *         Processing steps:
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] Bool       *error
 * @return 
 */
static Void rgTOMInfFreePduEvnt(RgInfSfDatInd *sfInfo)
{

   RG_FREE_MEM(sfInfo);
   return;
} /* end of rgTOMUtlFreePduEvnt */

#ifdef LTE_L2_MEAS

/** @brief This function performs the preparation of information needed to set
 * L2M Scheduled UL Throughput Information for a particular UE.
 *
 * @details
 *
 *     Function: rgTomUtlPrepareL2MUlThrpInfo
 *      This function performs the preparation of information needed to set
 *       L2M Scheduled UL Throughput Information for a particular UE.
 *
 *
 *         Processing steps:
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb       *ueCb
 * @param  [out] RgRguDedDatInd *dDatInd
 * @return 
 */
static S16 rgTomUtlPrepareL2MUlThrpInfo(RgCellCb *cellCb,RgUeCb *ueCb,RgRguDedDatInd *dDatInd)
{
   uint8_t lcId;
   uint8_t lcgId;
   uint8_t loop;

   dDatInd->burstInd = RGU_L2M_UL_BURST_END;
   for(loop=0;loop<dDatInd->numLch;loop++)
   {
      lcId=dDatInd->lchData[loop].lcId;
      if (lcId)
      {
	 lcgId = ueCb->ul.lcCb[lcId - 1].lcgId;
	 if(ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs > 0)
	 {
	    dDatInd->burstInd = RGU_L2M_UL_BURST_START;
	    break;
	 }
      }
   }

   return ROK;
}

#endif

/** @brief This function is called by rgTOMDatInd. This function invokes the
 * scheduler with the information of the received Data and any Control Elements
 * if present. Also it generates Data indications towards the higher layers.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Retrieves the RaCb with the rnti provided, if it doesnt exist
 *         return failure. 
 *         - If UE exists then update the Schduler with any MAC CEs if present. 
 *         - Invoke RAM module to do Msg3 related processing rgRAMProcMsg3
 *         - Loop through the SDU subheaders and invoke either a common data
 *         indication (rgUIMSndCmnDatInd) or dedicated data indication
 *         (rgUIMSndDedDatInd) towards the higher layers. 
 *
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] CmLteRnti  rnti
 * @param  [in] RgMacPdu   *pdu
 * @param  [out] uint32_t       *lcgBytes
 *  
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
RgUeCb  *glblueCb4;
RgUeCb  *glblueCb5;

#ifdef LTEMAC_SPS
   static S16 rgTOMUtlProcMsg
(
 RgCellCb      *cellCb, 
 RgUeCb        *ueCb,
 RgMacPdu      *pdu,
 Bool          isSpsRnti,
 Bool          *spsToBeActvtd,
 uint16_t      *sduSize,
 uint16_t      slot,
 uint32_t      *lcgBytes
 )
#else /* LTEMAC_SPS */
   static S16 rgTOMUtlProcMsg
(
 RgCellCb      *cellCb, 
 RgUeCb        *ueCb,
 RgMacPdu      *pdu,
 uint16_t      slot,
 uint32_t      *lcgBytes
 )
#endif
{
   Inst              inst = cellCb->macInst - RG_INST_START;
   S16               ret;
   RgRguCmnDatInd    *cDatInd;
   RgRguDedDatInd    *dDatInd;
   CmLList           *node;
   RgMacSdu          *sdu;
   MsgLen            ccchSz;
   MsgLen            cpySz;
#ifdef LTEMAC_SPS
   Pst               schPst1;  
   //   RgInfSpsRelInfo   relInfo;
#endif

#ifdef LTE_L2_MEAS
   uint8_t                idx1;
   uint8_t                idx2;
   RgUlSf                 *ulSf;
   uint16_t               totalBytesRcvd = 0;
   uint16_t               sduLen[RGU_MAX_LC] = {0};
   uint8_t                qciVal[RGU_MAX_LC] = {0};
   uint8_t                numPrb = 0;

#endif
   uint8_t                lcgId;
   MsgLen                 bufSz;

   /* Moved outside of LTE_L2_MEAS
    *          scope as this pointer will now be used to 
    *          check for valid Logical Channel ID
    */
   RgUlLcCb          *ulLcCb;

   cDatInd  = NULLP;
   dDatInd  = NULLP;
#ifdef LTE_L2_MEAS
   ulSf   = NULLP;
   idx1   = 0;
   idx2   = 0;
#endif
#ifdef SS_RBUF 
   Void *elem = NULLP;
#endif

   ulLcCb = NULLP;


#ifndef LTE_L2_MEAS      
   UNUSED(slot);
#endif

   if(pdu->sduLst.first)
   {
      sdu = (RgMacSdu*)(pdu->sduLst.first->node);
      glblueCb4 = ueCb;
      if ((sdu->lcId == RG_CCCH_LCID))
      {
	 /* code for common channel dat indications */
	 if ((ret = rgAllocShrablSBuf (inst,(Data**)&cDatInd, sizeof(RgRguCmnDatInd))) != ROK)
	 {
	    return RFAILED;
	 }
	 cDatInd->cellId   = cellCb->cellId;
	 cDatInd->rnti     = ueCb->ueId;
	 /* rg001.101: Corrected lcId value for common data indication */
	 cDatInd->lcId     = cellCb->ulCcchId;
	 cDatInd->pdu      = sdu->mBuf;
	 SFndLenMsg (sdu->mBuf, &ccchSz);
	 /* Fix : syed Contention resolution ID copy should consider only
	  * 6 bytes of information from sdu->mBuf. Incase of CCCH sdu for reest
	  * message/psuedo reest message, ccchSz can go beyond 6 and can corrupt 
	  * other fields of ueCb. */
	 if (ccchSz >= RG_CRES_LEN)
	 {
	    SCpyMsgFix (sdu->mBuf, (MsgLen)0, RG_CRES_LEN, ueCb->contResId.resId,
		  &cpySz);
	 }
#ifdef XEON_SPECIFIC_CHANGES
	 CM_LOG_DEBUG(CM_LOG_ID_MAC, "CCCH SDU of size(%d) received for UE(%d) CRES[0x%x 0x%x 0x%x 0x%x 0x%x 0x%x] Time[%d %d]\n",  ((S16)ccchSz), ueCb->ueId,ueCb->contResId.resId[0], ueCb->contResId.resId[1], ueCb->contResId.resId[2], ueCb->contResId.resId[3], ueCb->contResId.resId[4], ueCb->contResId.resId[5], cellCb->crntTime.sfn,  cellCb->crntTime.slot);
#endif
	 sdu->mBuf = NULLP;
	 rgUIMSndCmnDatInd(inst,cellCb->rguUlSap,cDatInd);
	 return ROK;
      } /* end of common channel processing */
#ifndef SS_RBUF 
      if ((ret = rgAllocShrablSBuf (inst,(Data**)&dDatInd, sizeof(RgRguDedDatInd))) != ROK)
      {
	 return RFAILED;
      }
#else
      glblueCb5 = ueCb;
      elem = SRngGetWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
      if (NULLP == elem)
      { 
	 return RFAILED;
      }
      dDatInd = (RgRguDedDatInd *)elem;
      memset(dDatInd, 0x00, sizeof(RgRguDedDatInd));
#endif
      dDatInd->cellId   = cellCb->cellId;
      dDatInd->rnti     = ueCb->ueId;
      dDatInd->numLch   = 0;
   }
#ifdef LTE_L2_MEAS
   ulSf = &cellCb->ulSf[(slot % RG_NUM_SUB_FRAMES)];
   if(ulSf->ueUlAllocInfo != NULLP)
   {
      for(idx1 = 0; idx1 < ulSf->numUe; idx1++)
      {
	 if(ulSf->ueUlAllocInfo[idx1].rnti == ueCb->ueId)
	 {
	    numPrb = ulSf->ueUlAllocInfo[idx1].numPrb;
	    break;
	 }
      }
   }
#endif
   node =  pdu->sduLst.first;
   while (node)
   {
      sdu = (RgMacSdu*)node->node;

      ulLcCb = rgDBMGetUlDedLcCb (ueCb, sdu->lcId);

      if(ulLcCb == NULLP)
      {
	 DU_LOG("\nERROR  -->  MAC : Unconfigured LCID:%d CRNTI:%d"
	       ,sdu->lcId,ueCb->ueId);
	 /* ccpu00128443: Fix for memory leak */
	 /* Fix : syed Neccessary to set sdu->mBuf = NULLP */
	 RG_FREE_MSG(sdu->mBuf);         
	 node = node->next;
	 continue;
      }
#ifdef RLC_STA_PROC_IN_MAC/* RLC Status PDU Processing */
      {
	 S16 rlcProcDlStatusPdu(Pst       *udxPst,SuId      suId,
	       CmLteCellId cellId,CmLteRnti rnti,CmLteLcId lcId,Buffer *rlcSdu);

	 if(ROK == rlcProcDlStatusPdu(&(cellCb->rguDlSap->sapCfg.sapPst),
		  cellCb->rguDlSap->sapCfg.suId,
		  cellCb->cellId,ueCb->ueId,sdu->lcId,sdu->mBuf))
	 {
	    RG_FREE_MSG(sdu->mBuf);	      
	    node = node->next;
	    continue;
	 }
      }
#endif

      /* ccpu00116477- Fixed the rgUIMSndDedDatInd condition when we receive 11 sdus in the 
       * list we are losing 11th sdu and sending the first 10 sdus again which
       * is causing the duplicate packets and eNB crashing due to access
       * of the freed memory */
      if (dDatInd->numLch >= RGU_MAX_LC)
      {
	 if ((ret = rgUIMSndDedDatInd(inst,ueCb->rguUlSap,dDatInd)) != ROK)
	 {
	    DU_LOG("\nERROR  -->  MAC : Failed to send datIndication to RGU CRNTI:%d",ueCb->ueId);
	    return (ret);
	 }
#ifndef SS_RBUF
	 if ((ret = rgAllocShrablSBuf (inst,(Data**)&dDatInd, sizeof(RgRguDedDatInd))) != ROK)
	 {
	    return RFAILED;
	 }
#else
	 elem = SRngGetWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
	 if (NULLP == elem)
	 { 
	    return RFAILED;
	 }
	 dDatInd = (RgRguDedDatInd *)elem;
	 memset(dDatInd, 0x00, sizeof(RgRguDedDatInd));
#endif
	 dDatInd->cellId   = cellCb->cellId;
	 dDatInd->rnti     = ueCb->ueId;
	 dDatInd->numLch   = 0;
      }
      dDatInd->lchData[dDatInd->numLch].lcId = sdu->lcId;
      dDatInd->lchData[dDatInd->numLch].pdu.mBuf[dDatInd->lchData[dDatInd->numLch].pdu.numPdu] = sdu->mBuf;
      dDatInd->lchData[dDatInd->numLch].pdu.numPdu++;
      lcgId = ulLcCb->lcgId;
      SFndLenMsg(sdu->mBuf, &bufSz);
#ifdef LTE_L2_MEAS
      if(ulLcCb->measOn)
      {
	 ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs -= bufSz; 
      }
#endif
      //if ((lcgBytes != NULLP) && (ueCb->ul.lcgArr[lcgId].isGbr == TRUE))
      if (lcgBytes != NULLP)
      {
	 lcgBytes[lcgId] += bufSz;
      }
      sdu->mBuf = NULLP;
      dDatInd->numLch++;
#ifdef LTEMAC_SPS
      /* Check if data has come on SPS LC */
      /* KWORK_FIX: Modified the index from lcId to lcId-1 for handling lcId 10 properly */
      if (ueCb->ul.spsLcId[sdu->lcId-1] == TRUE)
      {
	 ueCb->ul.spsDatRcvd++;
      }

      if(isSpsRnti)
      {
	 /* Data rcvd on CRNTI*/
	 /* Retrieve the LCG ID of the LCID*/
	 /* SPS LCG has data whose size > SID Size */
	 /* Activate SPS if data recvd on SPS LCID and size > SID Packet Size */
	 if((ueCb->ul.spsLcId[sdu->lcId-1] == TRUE) &&
	       (sdu->len > RG_SPS_SID_PACKET_SIZE))
	 {
	    *spsToBeActvtd = TRUE;
	    *sduSize = sdu->len;
	 }
      }

#endif  

#ifdef LTE_L2_MEAS
      if(cellCb->qciArray[ulLcCb->qci].mask == TRUE)
      {
	 sduLen[ulLcCb->qci] = sdu->len;
	 totalBytesRcvd += sdu->len;
	 qciVal[ulLcCb->qci] = ulLcCb->qci;
      }
#endif
      node = node->next;
   } /* end of while for SubHeaders */
#ifdef LTE_L2_MEAS
   for(idx2 = 0; idx2 < RGU_MAX_LC; idx2++)
   {
      if((cellCb->qciArray[qciVal[idx2]].mask == TRUE) &&
	    totalBytesRcvd > 0)
      {
	 cellCb->qciArray[qciVal[idx2]].prbCount += 
	    ((numPrb * sduLen[idx2]) / totalBytesRcvd);
      }

      /* RRM_RBC_X */
      if(totalBytesRcvd > 0 && qciVal[idx2] > 0)
      {
	 RG_UPD_GBR_PRB(cellCb, qciVal[idx2], ((numPrb * sduLen[idx2])/totalBytesRcvd));
      }
      /* RRM_RBC_Y */
   }
#endif
   /*Added for explicit release - start*/
#ifdef LTEMAC_SPS

   if(isSpsRnti && dDatInd && dDatInd->numLch)
   {
      if(ueCb->ul.spsDatRcvd != 0)
      {
	 ueCb->ul.explRelCntr = 0;
	 ueCb->ul.spsDatRcvd = 0;
      }
      else
      {
	 ueCb->ul.explRelCntr++;
	 if (ueCb->ul.explRelCntr == ueCb->ul.explRelCnt)
	 {
	    ueCb->ul.explRelCntr = 0;
	    /* Indicate scheduler for explicit release */
	    memset(&schPst1, 0, sizeof(Pst));
	    rgGetPstToInst(&schPst1,inst, cellCb->schInstMap.schInst);
	    //TODO: commented for compilation without SCH 
#if 0
	    relInfo.cellSapId = cellCb->schInstMap.cellSapId;
	    relInfo.cRnti = ueCb->ueId;
	    relInfo.isExplRel = TRUE;
	    /* Release indicator is called now through the matrix in the function below */
	    //TODO: commented for compilation without SCH RgMacSchSpsRel( &schPst1, &relInfo );
#endif
	    ueCb->ul.implRelCntr = 0;
	 }
      }
   } 
   else
   {
      /* SPS_FIX */
      if(ueCb->ul.spsDatRcvd != 0)
      {
	 //ueCb->ul.implRelCntr = 0;
	 ueCb->ul.explRelCntr = 0;
	 ueCb->ul.spsDatRcvd = 0;
      }
   }
#endif
   /*Added for explicit release - end */

   if((dDatInd) && (dDatInd->numLch))
   {
#ifdef LTE_L2_MEAS
      rgTomUtlPrepareL2MUlThrpInfo(cellCb, ueCb,dDatInd);

      RG_CALC_TTI_CNT(cellCb, dDatInd->ttiCnt); 
#endif
      if ((ret = rgUIMSndDedDatInd(inst,ueCb->rguUlSap,dDatInd)) != ROK)
      {
	 DU_LOG("\nERROR  -->  MAC : Failed to send datIndication to RGU CRNTI:%d",ueCb->ueId);
	 return (ret);
      }
   }
#ifndef SS_RBUF
   else if((dDatInd) && (0 == dDatInd->numLch))
   {
      /* Free the memory allocated for dDatInd if we 
       * have no valid LCH PDU to send to RLC.*/
      rgFreeSharableSBuf(inst,(Data **)&dDatInd,sizeof(RgRguDedDatInd)); 
   }
#endif
   return ROK;
} /* end of */ 

/** @brief This function frees up the RgMacPdu structure that has been
 * populated by demux.
 *
 * @details
 *
 *     Function: rgTOMUtlInsSchInfo
 *       - Function frees up the RgMacPdu structure, in case of error it shall
 *       free up the buffer's present in the different sdu.
 *
 *         Processing steps:
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] Bool       *error
 * @return 
 */
#ifdef LTEMAC_SPS
   static S16 rgTOMUtlInsSchInfo
(
 RgMacPdu *pdu,
 RgInfSfDatInd *sfInfo,
 RgInfCeInfo   *ceInfo,
 CmLteRnti      rnti,
 Bool           spsToBeActvtd,
 uint16_t       sduSize,
 uint32_t       *lcgBytes
 )
#else
   static S16 rgTOMUtlInsSchInfo
(
 RgMacPdu *pdu,
 RgInfSfDatInd *sfInfo,
 RgInfCeInfo *ceInfo,
 CmLteRnti   rnti,
 uint32_t    *lcgBytes
 )
#endif
{
   S16            ret;
   RgInfUeDatInd *ueInfo;
   uint32_t       lcgId = 0;
   uint32_t       idx = 0;


   RG_TOM_INF_ALLOC(sfInfo, sizeof(RgInfUeDatInd), ueInfo, ret);

   if(ROK != ret)
   {
      return RFAILED;
   }

   ueInfo->rnti = rnti; 

   ueInfo->ceInfo = *ceInfo;
   ueInfo->ueLstEnt.node = (PTR)ueInfo;
   for (lcgId = 1, idx = 0; lcgId < RGINF_MAX_LCG_PER_UE; lcgId++)
   {
      if (lcgBytes[lcgId] != 0)
      {
	 /* Only GBR bytes */
	 ueInfo->lcgInfo[idx].lcgId     = lcgId;
	 ueInfo->lcgInfo[idx++].bytesRcvd = lcgBytes[lcgId];
	 lcgBytes[lcgId] = 0;
      }
   }
   cmLListAdd2Tail(&sfInfo->ueLst, &ueInfo->ueLstEnt);
   return ROK;
} /* end of rgTOMUtlInsSchInfo */

#include <stdlib.h>
/**
 * @brief Handler for processing data indication recieved from PHY for UEs.
 *
 * @details
 *
 *     Function: rgTOMDatInd
 *
 *     Handler for processing data indication recieved from PHY for UEs.
 *
 *     Invoked by: RgLiTfuDatInd of LIM 
 *
 *     Processing Steps: 
 *     For each DataInfo recieved
 *      - Validate the information received and retrieve cellCb 
 *        Validate cellId, rnti 
 *      - Call De-Mux module to decode the data rgDUXDemuxData
 *      - If  received a CRNTI control element 
 *          - Check if a CCCH SDU is present, if it is return failure
 *          - Check for the existence of UE, if its isnt present return failure.
 *          - Delegate the remaining processing to rgTOMUtlProcMsg3 which
 *          primarily informs the scheduler about the data received and
 *          generates Data indications towards the higher layer.
 *      - If only CCCH SDU is present
 *        - Invoke rgTOMUtlProcMsg3 for further processing.
 *      - If its a non-Msg3 PDU i.e. received outside of a RA procedure
 *        - Retrieve the UeCB 
 *        - Validate that the received PDU contains only configured Logical
 *        Channels.
 *        - Invoke rgTOMUtlProcDatPdu for further processing. It informs the
 *        scheduler with the information of the received Data and generates
 *        DatIndications towards the higher layers. 
 *           
 * @param  [in] Inst        inst
 *  @param[in]  TfuDatIndInfo *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgTOMDatInd(Inst inst, TfuDatIndInfo *datInd)
{
   S16               ret = ROK;
   RgErrInfo         err;
   RgUeCb            *ueCb;
   RgUeCb            *prevUeCb = NULLP;
   RgCellCb          *cellCb;
   RgMacPdu          *pdu;
   RgInfSfDatInd     *sfInfo;
   RgInfCeInfo       ceInfo; 
   Pst               schPst;
   CmLList           *node;
   TfuDatInfo        *datInfo;
   RgLowSapCb        *tfuSap;
   uint16_t          slot;
#ifdef LTEMAC_SPS
   Bool              isSpsRnti=FALSE;
   Pst               schPst1;  
   // RgInfSpsRelInfo   relInfo;
   Bool              spsToBeActvtd = FALSE;
   uint16_t          sduSize = 0;
#endif
   uint32_t          lcgBytes[RGINF_MAX_LCG_PER_UE];


#ifdef STUB_TTI_HANDLING_5GTF 
   node =  datInd->datIndLst.first;
   for (;node; node=node->next)
   {
      datInfo = (TfuDatInfo*)node->node;
      {
	 MsgLen len;
	 SFndLenMsg(datInfo->mBuf, &len);
	 rgUlrate_tfu += len;
	 if (rgUlrate_tfu > 100000)
	 {
	    DU_LOG("\nINFO  -->  MAC : rgTOMDatInd datInfo->mBuf len =%d rgUlrate_tfu=%d",len,rgUlrate_tfu);
	    rgUlrate_tfu = 0;
	 }
      }
   }
   return(RFAILED);
#endif      

   memset(&lcgBytes, 0, sizeof(lcgBytes));

   tfuSap = &(rgCb[inst].tfuSap);
   ueCb = NULLP;
   cellCb = rgCb[inst].cell;
   if((cellCb == NULLP) ||
	 (cellCb->cellId != datInd->cellId))   
   {

      DU_LOG("\nERROR  -->  MAC : Unable to get the cellCb for cell");
      return RFAILED;
   }
   /* Avoiding memset as all the fields are getting initialized further */

   if (rgTOMInfAllocPduEvnt (inst,&sfInfo) != ROK)
   {
      err.errType = RGERR_TOM_DATIND;
      DU_LOG("\nERROR  -->  MAC : Unable to Allocate PDU for DUX cell");
      node =  datInd->datIndLst.first;
      return RFAILED;
   }
   cmLListInit(&sfInfo->ueLst);
   sfInfo->cellId = datInd->cellId;
   sfInfo->timingInfo = datInd->timingInfo;
   slot = datInd->timingInfo.slot;

   node =  datInd->datIndLst.first;
   for (;node; node=node->next)
   {
      datInfo = (TfuDatInfo*)node->node;
      {
	 //uint32_t ulrate_tfu;
	 MsgLen len;
	 SFndLenMsg(datInfo->mBuf, &len);
#ifdef STUB_TTI_HANDLING_5GTF         
	 //  DU_LOG(":rgTOMDatInd datInfo->mBuf len =%d",len);
#endif
	 rgUlrate_tfu += len;
#ifdef EMTC_ENABLE
	 grgUlrate_tfu += len;
#endif
      }
#ifdef STUB_TTI_HANDLING_5GTF         
      rgLIMUtlFreeDatIndEvnt(datInd,TRUE);
#endif
      /* We shall call De-Mux to process the received buffer. We shall try and find
       * out the RaCb based on the following - 
       * 1. If the incoming PDU contained a CCCH SDU i.e. this is message 3.
       * 2. If the incoming PDU contained a CRNTI control element, i.e. we should
       * have a ueCb also for this 
       */
      /* Lets allocate the event that needs to be passed to DUX */
      if (rgTOMUtlAllocPduEvnt (inst,&pdu) != ROK)
      {
	 err.errType = RGERR_TOM_DATIND;
	 DU_LOG("\nERROR  -->  MAC : Unable to Allocate PDU for DUX cell");
	 rgTOMInfFreePduEvnt (sfInfo);
	 return RFAILED;
      }

      if ((ret = rgDUXDemuxData (inst,pdu, &ceInfo, 
		  &datInfo->mBuf, &err)) != ROK)
      {
	 //exit(1);
	 /* Fix: sriky memory corruption precautions */
	 rgTOMUtlFreePduEvnt (pdu, TRUE);
	 err.errType = RGERR_TOM_DATIND;
	 DU_LOG("\nERROR  -->  MAC : DUX processing failed");
	 tfuSap->sapSts.numPduDrop++;
	 continue; 
      }
      /* It could be that a non-msg3 pdu contains a CRNTI Control element. We
       * should check for CRNTI CE and if it exists the UECb must exist, also an
       * if the CRNTI in the CE and the one with which the message came in are
       * different we shall look for an raCb as well. 
       */

      if (ceInfo.bitMask & RG_CCCH_SDU_PRSNT)
      {
	 ret = rgTOMProcCCCHSduInDatInd(pdu, prevUeCb, \
	       cellCb, datInfo, &ceInfo, slot);
	 if (ret == RFAILED)
	 {
	    rgTOMUtlFreePduEvnt (pdu, TRUE);
	    err.errType = RGERR_TOM_DATIND;
	    tfuSap->sapSts.numPduDrop++;
	    continue; 
	 }
      } /* end of Msg3 processing */

      else if (ceInfo.bitMask & RG_CRNTI_CE_PRSNT)
      {
	 ret = rgTOMProcCrntiCEInDatInd(pdu, prevUeCb, \
	       cellCb, datInfo, &ceInfo, slot);
	 if (ret == RFAILED)
	 {
	    rgTOMUtlFreePduEvnt (pdu, TRUE);
	    err.errType = RGERR_TOM_DATIND;
	    tfuSap->sapSts.numPduDrop++;
	    continue; 
	 }

      } /* end of CRNTI based message */
      else
      {
	 ueCb = rgDBMGetUeCb (cellCb, datInfo->rnti);
	 if (ueCb == NULLP)
	 {
#ifdef LTEMAC_SPS
	    /* Try getting the UE using SPS-RNTI. */
	    ueCb = rgDBMGetSpsUeCb (cellCb, datInfo->rnti);
	    if (ueCb != NULLP)
	    {
	       isSpsRnti = TRUE;
	       /* Increment implrelCntr for an empty transmission */
	       if (pdu->sduLst.count == 0)
	       {
		  ueCb->ul.implRelCntr++;
		  if (ueCb->ul.implRelCntr == ueCb->ul.implRelCnt)
		  {
		     /* Indicate scheduler for implicit release */
		     memset(&schPst1, 0, sizeof(Pst));
		     rgGetPstToInst(&schPst1,inst, cellCb->schInstMap.schInst);

		     ueCb->ul.implRelCntr = 0;
		     ueCb->ul.explRelCntr = 0;
#if 0                     
		     relInfo.cellSapId = cellCb->schInstMap.cellSapId;
		     relInfo.cRnti = ueCb->ueId;
		     relInfo.isExplRel= FALSE;
		     //TODO: commented for compilation without SCH RgMacSchSpsRel(&schPst1, &relInfo);
#endif  
		  }
	       }
	       else
	       {
		  /* Reset the implrelCntr */
		  ueCb->ul.implRelCntr = 0;
	       }
	    }
	    else
#endif 
	    {
	       /* Perform failure if ueCb is still NULLP */
	       rgTOMUtlFreePduEvnt (pdu, TRUE);
	       err.errType = RGERR_TOM_DATIND;
	       DU_LOG("\nERROR  -->  MAC : RNTI:%d Unable to get the UE CB", 
		     datInfo->rnti);
	       tfuSap->sapSts.numPduDrop++;
	       continue;
	    }
	 }
#ifdef LTE_L2_MEAS         
	 rgTOMUtlL2MStoreBufSz(ueCb, &ceInfo);
	 rgTOML2MCompileActiveLCs (cellCb, ueCb, pdu, &ceInfo);
#endif
#ifdef LTEMAC_SPS
	 if ((ret = rgTOMUtlProcMsg(cellCb, ueCb, pdu, isSpsRnti,&spsToBeActvtd,&sduSize, slot, (uint32_t *)&lcgBytes)) != ROK)
#else
	    if ((ret = rgTOMUtlProcMsg (cellCb, ueCb, pdu, slot, (uint32_t *)&lcgBytes)) != ROK)
#endif /* LTEMAC_SPS */
	    {
	       rgTOMUtlFreePduEvnt (pdu, TRUE);
	       err.errType = RGERR_TOM_DATIND;
	       DU_LOG("\nERROR  -->  MAC : Unable to handle Data Indication CRNTI:%d",ueCb->ueId);
	       tfuSap->sapSts.numPduDrop++;
	       continue;
	    }
      }


#ifdef LTEMAC_SPS
      if(rgTOMUtlInsSchInfo(pdu, sfInfo, &ceInfo, datInfo->rnti,spsToBeActvtd,sduSize, (uint32_t *)&lcgBytes) != ROK)
#else
	 if(rgTOMUtlInsSchInfo(pdu, sfInfo, &ceInfo, datInfo->rnti, (uint32_t *)&lcgBytes) != ROK)
#endif

	 {
	    rgTOMInfFreePduEvnt (sfInfo);
	    rgTOMUtlFreePduEvnt (pdu, FALSE);
	    return RFAILED;
	 }
      /* free up the PDU memory */
      rgTOMUtlFreePduEvnt (pdu, FALSE);
   }
   /* Free the allocated memory for ueUlAllocInfo here */
#ifdef LTE_L2_MEAS
   if(cellCb->ulSf[(slot % RG_NUM_SUB_FRAMES)].ueUlAllocInfo != NULLP)
   {
      /*ccpu00117052 - MOD - Passing double for proper NULLP
	assignment */
      rgFreeSBuf(inst,(Data **)&(cellCb->ulSf[(slot % RG_NUM_SUB_FRAMES)].ueUlAllocInfo), 
	    ((cellCb->ulSf[(slot % RG_NUM_SUB_FRAMES)].numUe) * sizeof(RgUeUlAlloc)));
   }
#endif
   /* RRM_RBC_X */
   /* Update PRB used for all GBR QCIs to scheduler */
   memcpy( &sfInfo->qcisUlPrbCnt[0],
	 &cellCb->qcisUlPrbCnt[0],
	 (RGM_MAX_QCI_REPORTS * sizeof(uint32_t)));
   /* clear the cellCb ul prb value */
   memset(&cellCb->qcisUlPrbCnt[0], 0, 
	 (RGM_MAX_QCI_REPORTS * sizeof(uint32_t)));

   /* RRM_RBC_Y */

   rgGetPstToInst(&schPst, inst,cellCb->schInstMap.schInst);
   sfInfo->cellSapId = cellCb->schInstMap.cellSapId;
   //TODO: commented for compilation without SCH RgMacSchSfRecp(&schPst, sfInfo);
   return ROK;
}  /* rgTOMDatInd */

/**
 * @brief Function handles allocation for common channels i.e. BCCH-BCH,
 * BCCH-DLSCH, PCCH-DLSCH.
 *
 * @details
 *
 *     Function : rgHndlCmnChnl
 *     
 *     This function is invoked from RgSchMacSfAllocReq. This function handles
 *     allocations made for common channels like BCCH and PCCH. 
 *
 *     Processing steps:
 *     1. If BCCH on BCH has been scheduled, send StatusIndication on RGU.
 *     2. If PCCH is scheduled, send StatusIndication on RGU.
 *     3. If BCCH on DLSCH has been scheduled and sndStatInd is TRUE, send
 *     StatusIndication on RGU, else copy the bcch buffer onto the downlink
 *     subframe. 
 *     
 *           
 *  @param[in] RgCellCb          *cell,
 *  @param[in] CmLteTimingInfo   timingInfo,
 *  @param[in] RgInfCmnLcInfo    *cmnLcInfo,
 *  @param[in/out] RgErrInfo     *err,
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
   static S16 rgHndlCmnChnl
(
 RgCellCb            *cell,
 CmLteTimingInfo     timingInfo,
 RgInfCmnLcInfo      *cmnLcInfo,
 RgErrInfo           *err
 )
{
#if (ERRCLASS & ERRCLS_DEBUG)
   RgPcchLcCb           *pcch;
#endif
#ifndef RGR_SI_SCH
   RgBcchDlschLcCb      *bcch;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgBcchBchLcCb        *bch;
#endif
#endif/*RGR_SI_SCH*/
   RguCStaIndInfo       *staInd;
   RgDlSf               *dlSf;
   Inst                 inst = cell->macInst - RG_INST_START;


   dlSf = &cell->subFrms[(timingInfo.slot % RG_NUM_SUB_FRAMES)];

   if(cmnLcInfo->bitMask & RGINF_BCH_INFO)
   {
#ifndef RGR_SI_SCH
#if (ERRCLASS & ERRCLS_DEBUG) 
      if(NULLP == (bch = rgDBMGetBcchOnBch(cell)))
      {
	 return RFAILED;
      }
      if(cmnLcInfo->bchInfo.lcId != bch->lcId)
      {
	 return RFAILED;
      }
#endif

      if (rgAllocShrablSBuf (inst,(Data**)&staInd, sizeof(RguCStaIndInfo)) != ROK)
      {
	 err->errCause = RGERR_TOM_MEM_EXHAUST;
	 return RFAILED;
      }
      staInd->cellId = cell->cellId;
      staInd->rnti   = RG_INVALID_RNTI;
      staInd->lcId   = cmnLcInfo->bchInfo.lcId;
      staInd->transId = (timingInfo.sfn << 8) | (timingInfo.slot);
      /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      dlSf->remDatReqCnt++;
#endif
      if (rgUIMSndCmnStaInd (inst,cell->rguDlSap,staInd) != ROK)
      {
	 return RFAILED;
      }
#else
      /*Store the received BCH Data in the scheduled subframe*/
      dlSf->bch.tb = cmnLcInfo->bchInfo.pdu;
#endif/*RGR_SI_SCH*/
   }

   if(cmnLcInfo->bitMask & RGINF_PCCH_INFO)
   {
#if (ERRCLASS & ERRCLS_DEBUG) 
      if(NULLP == (pcch = rgDBMGetPcch(cell)))
      {
	 return RFAILED;
      }
      if(cmnLcInfo->pcchInfo.lcId != pcch->lcId)
      {
	 return RFAILED;
      }
#endif

      dlSf->pcch.pdcch.rnti = 
	 cmnLcInfo->pcchInfo.rnti;
      dlSf->pcch.pdcch.dci = 
	 cmnLcInfo->pcchInfo.dciInfo;
#ifdef TFU_UPGRADE               
      /* ccpu00132314-ADD-Fill the tx Pwr offset from scheduler */         
      dlSf->pcch.txPwrOffset = cmnLcInfo->pcchInfo.txPwrOffset;         
#endif
      if (rgAllocShrablSBuf (inst,(Data**)&staInd, sizeof(RguCStaIndInfo)) != ROK)
      {
	 err->errCause = RGERR_TOM_MEM_EXHAUST;
	 return RFAILED;
      }
      staInd->cellId = cell->cellId;
      staInd->rnti   = RG_INVALID_RNTI;
      staInd->lcId   = cmnLcInfo->pcchInfo.lcId;
      staInd->transId = (timingInfo.sfn << 8) | (timingInfo.slot);
      /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      dlSf->remDatReqCnt++;
#endif
      /* for consolidated CmnStaInd calling below function from function 
       * rgHndlSchedUe once CmnStaInd prepared for all UEs
       */
      if (rgUIMSndCmnStaInd (inst,cell->rguDlSap,staInd) != ROK)
      {
	 return RFAILED;
      }
   }

   if(cmnLcInfo->bitMask & RGINF_BCCH_INFO)
   {
      dlSf->bcch.pdcch.rnti = 
	 cmnLcInfo->bcchInfo.rnti;
      dlSf->bcch.pdcch.dci = 
	 cmnLcInfo->bcchInfo.dciInfo;
#ifdef TFU_UPGRADE               
      /* ccpu00132314-ADD-Fill the tx Pwr offset from scheduler */         
      dlSf->bcch.txPwrOffset = cmnLcInfo->bcchInfo.txPwrOffset;         
#endif      
#ifndef RGR_SI_SCH
      if(NULLP == 
	    (bcch=rgDBMGetBcchOnDlsch(cell,cmnLcInfo->bcchInfo.lcId)))
      {
	 return RFAILED;
      }
      if(TRUE == cmnLcInfo->bcchInfo.sndStatInd)
      {
	 RG_FREE_MSG(bcch->tb);
	 if (rgAllocShrablSBuf (inst,(Data**)&staInd, 
		  sizeof(RguCStaIndInfo)) != ROK)
	 {
	    err->errCause = RGERR_TOM_MEM_EXHAUST;
	    return RFAILED;
	 }
	 staInd->cellId = cell->cellId;
	 staInd->rnti   = RG_INVALID_RNTI;
	 staInd->lcId   = cmnLcInfo->bcchInfo.lcId;
	 staInd->transId = (timingInfo.sfn << 8) | (timingInfo.slot);
	 /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
	 dlSf->remDatReqCnt++;
#endif
	 if (rgUIMSndCmnStaInd (inst,cell->rguDlSap,staInd) != ROK)
	 {
	    return RFAILED;
	 }
      }
      else
      {
	 SCpyMsgMsg(bcch->tb, RG_GET_MEM_REGION(rgCb[inst]),
	       RG_GET_MEM_POOL(rgCb[inst]), &dlSf->bcch.tb);
      }
#else
      /*Store the received BCCH Data in the scheduled subframe*/
      dlSf->bcch.tb = cmnLcInfo->bcchInfo.pdu;
#endif/*RGR_SI_SCH*/
   }

   return ROK;
} /* end of rgHndlCmnChnl */

/**
 * @brief Function for handling allocations for dedicated channels for a
 * subframe.
 *
 * @details
 *
 *     Function : rgHndlSchdUe
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of dedicated channels for a subframe. Invoked by the function
 *     RgSchMacSfAllocReq.
 *
 *     Processing steps :
 *     1. Loops through the list of UE's scheduled looking for the corresponding
 *     ueCb/raCb. 
 *     2. Finds the corresponding HARQ process.
 *     3. Invokes the DHM function to issue StatusIndications on RGU.
 *
 *           
 *  @param[in] RgCellCb            *cell,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RgInfUeInfo         *ueInfo
 *  @param[in/out] RgErrInfo       *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
   static S16 rgHndlSchdUe
(
 RgCellCb            *cell,
 CmLteTimingInfo     timingInfo,
 RgInfUeInfo         *ueInfo,
 RgErrInfo           *err
 )
{


   if(NULLP == ueInfo->allocInfo)
   {
      return RFAILED;
   }

   rgDHMSndConsolidatedStaInd(cell, ueInfo, timingInfo, err);

   return ROK;
} /* end of rgHndlSchdUe */

#ifdef LTE_L2_MEAS
/**
 * @brief Function for handling Uplink allocations for Ue for a
 * subframe.
 *
 * @details
 *
 *     Function : rgHndlUlUeInfo
 *     
 *  @param[in] RgCellCb            *cell,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RgInfUlUeInfo       *ueInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
   static S16 rgHndlUlUeInfo
(
 RgCellCb            *cell,
 CmLteTimingInfo     timingInfo,
 RgInfUlUeInfo       *ueInfo
 )
{
   Inst           inst = cell->macInst - RG_INST_START;
   uint8_t        idx;
   RgUlSf         *ulSf;
   S16            ret;


   ulSf = &cell->ulSf[(timingInfo.slot % RGSCH_NUM_SUB_FRAMES)];

   /* rg003.301-MOD- Corrected the purifier memory leak */
   if (ulSf->numUe != ueInfo->numUes)
   {
      if (ulSf->ueUlAllocInfo)
      {
	 rgFreeSBuf(inst,(Data **)&(ulSf->ueUlAllocInfo),
	       ulSf->numUe * sizeof(RgUeUlAlloc));
      }
   }
#ifdef XEON_SPECIFIC_CHANGES
   CM_MEAS_TIME((cell->crntTime.slot % RGSCH_NUM_SUB_FRAMES), CM_DBG_MAC_TTI_IND, CM_DBG_MEAS_FREE);
   CM_ADD_INFO((cell->crntTime.slot % RGSCH_NUM_SUB_FRAMES),  CM_DBG_MEAS_FREE, ulSf->numUe);
#endif
   ulSf->numUe         = ueInfo->numUes;
   if((ulSf->ueUlAllocInfo == NULLP) && (ueInfo->numUes > 0))
   {
      /* Allocate memory for ulAllocInfo */
      if((ret = rgAllocSBuf(inst,(Data**)&(cell->ulSf[(timingInfo.slot % RGSCH_NUM_SUB_FRAMES)].
		     ueUlAllocInfo), ueInfo->numUes *  sizeof(RgUeUlAlloc))) != ROK)
      {
	 return (ret);
      }
   }
#ifdef XEON_SPECIFIC_CHANGES
   CM_MEAS_TIME((cell->crntTime.slot % RGSCH_NUM_SUB_FRAMES), CM_DBG_MAC_TTI_IND, CM_DBG_MEAS_ALLOC);
   CM_ADD_INFO((cell->crntTime.slot % RGSCH_NUM_SUB_FRAMES), CM_DBG_MEAS_ALLOC, ueInfo->numUes);
#endif
   if (ulSf->ueUlAllocInfo != NULLP)
   {
      for(idx = 0; idx < ueInfo->numUes; idx++)
      {
	 ulSf->ueUlAllocInfo[idx].rnti   = ueInfo->ulAllocInfo[idx].rnti;
	 ulSf->ueUlAllocInfo[idx].numPrb = ueInfo->ulAllocInfo[idx].numPrb;
      }
   }
   RGCPYTIMEINFO(timingInfo, ulSf->schdTime);
   return ROK;
} /* end of rgHndlUlUeInfo */
#endif
/**
 * @brief Function for handling RaResp request received from scheduler to MAC
 *
 * @details
 *
 *     Function : rgTOMRlsSf
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of random access responses for a subframe.
 *     This shall invoke RAM to create ueCbs for all the rapIds allocated and 
 *     shall invoke MUX to create RAR PDUs for raRntis allocated.
 *     
 *           
 *  @param[in] Inst        inst
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RaRespInfo          *rarInfo
 *  @return  S16
 *      -# ROK 
 **/
Void rgTOMRlsSf(Inst inst,RgDlSf *dlSf)
{
   uint8_t          idx;


   if(dlSf->txDone == FALSE)
   {
      DU_LOG("\nERROR  -->  MAC : SUBFRAME Not pushed to the PHY");

      if (dlSf->bch.tb != NULLP)
      {
	 RG_FREE_MSG(dlSf->bch.tb);
      }
      if (dlSf->bcch.tb != NULLP)
      {
	 RG_FREE_MSG(dlSf->bcch.tb);
      }
      if (dlSf->pcch.tb != NULLP)
      {
	 RG_FREE_MSG(dlSf->pcch.tb);
      }
#ifdef EMTC_ENABLE
      rgTOMEmtcRlsSf(dlSf);
#endif
      for(idx=0; idx < dlSf->numRaRsp; idx++)
      {
	 RG_FREE_MSG(dlSf->raRsp[idx].rar);
      }
   }
   /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   dlSf->remDatReqCnt = 0;
   /* Fix[ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */
   dlSf->statIndDone = FALSE;
#endif
   if (dlSf->tbs.count)
   {
      uint8_t      i;
      CmLList      *node;
      RgDlHqProcCb *hqP;
      DU_LOG("\nERROR  -->  MAC : Error Stale TBs in Subframes TBS list\n");
      node = dlSf->tbs.first;
      while(node)
      {
	 hqP = (RgDlHqProcCb*)node->node;
	 node = node->next;
	 if (hqP)
	 {
	    for(i=0;i< RG_MAX_TB_PER_UE;i++)
	    {
	       if (hqP->tbInfo[i].sfLnkInfo[dlSf->schdTime.slot % 2].sf == dlSf)
	       {
		  cmLListDelFrm(&dlSf->tbs, &(hqP->tbInfo[i].sfLnkInfo[dlSf->schdTime.slot % 2].sfLnk));
		  hqP->tbInfo[i].sfLnkInfo[dlSf->schdTime.slot % 2].sfLnk.node = NULLP;
		  DU_LOG("\nERROR  -->  MAC : rgTOMRlsSf:: hqP %p \n", (Void *)hqP);
	       }
	       hqP->tbInfo[i].sfLnkInfo[dlSf->schdTime.slot % 2].sf = NULLP;
	    }
	 }
      }
   }
   /*arjun: check if dlSf laaTb list has to be freed???*/
   cmLListInit(&dlSf->tbs);
   dlSf->txDone = FALSE;
   dlSf->numRaRsp = 0;
   return;
}

/**
 * @brief Function is called by the scheduler once it has completed the
 * allocation for the subframe. 
 *
 * @details
 *
 *     Function : rgHndlFlowCntrl
 *     This function should fill and send Flow control 
 *     indication to RLC
 *
 *           
 *  @param[in] Pst                 *cell       
 *  @param[in] RgInfSfAlloc        *sfInfo   Carries all the allocation
 *  information.
 *  @return  S16
 *      -# ROK 
 **/
S16 rgHndlFlowCntrl(RgCellCb *cell,RgInfSfAlloc *sfInfo)
{
   RguFlowCntrlInd  *flowCntrlInd;
   Pst              *pst;
   uint32_t         ueIdx;
   uint32_t         lcIdx;

   pst = &cell->rguDlSap->sapCfg.sapPst;
   /* flowCntrlInd is alloced in cell init time and will be re-used throughout */
   flowCntrlInd = cell->flowCntrlInd;
   flowCntrlInd->cellId = sfInfo->cellId;
   flowCntrlInd->numUes = sfInfo->flowCntrlInfo.numUes; 

   for (ueIdx = 0; ueIdx < sfInfo->flowCntrlInfo.numUes; ueIdx++)
   {
      flowCntrlInd->ueFlowCntrlInfo[ueIdx].ueId = sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].ueId;
      flowCntrlInd->ueFlowCntrlInfo[ueIdx].numLcs = sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].numLcs;

      for (lcIdx = 0; lcIdx < RGINF_MAX_NUM_DED_LC; lcIdx++)
      {
	 flowCntrlInd->ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].lcId =
	    sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].lcId;
	 flowCntrlInd->ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].pktAdmitCnt = 
	    sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].pktAdmitCnt;

	 flowCntrlInd->ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].maxBo4FlowCtrl = 
	    sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].maxBo4FlowCtrl;
      }
   }
   RgUiRguFlowCntrlInd(pst, cell->rguDlSap->sapCfg.spId,flowCntrlInd); /* TODO: Rishi confirm if the suId and pst pointer is correct */
   return ROK;
}
/**
 * @brief Function is called by the scheduler once it has completed the
 * allocation for the subframe. 
 *
 * @details
 *
 *     Function : RgSchMacSfAllocReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of for a subframe. The sfInfo carries all the allocation
 *     details for the common channels, RA responses and dedicated channel
 *     allocations. 
 *
 *     Processing steps :
 *     1. Reset the information present in the downlink subframe that is
 *     scheduled.
 *     2. Handle common channel allocations
 *     3. Handle RA Response allocations
 *     4. Handle dedicated channel allocations.
 *           
 *  @param[in] Pst                 *pst       
 *  @param[in] RgInfSfAlloc        *sfInfo   Carries all the allocation
 *  information.
 *  @return  S16
 *      -# ROK 
 **/
S16 RgSchMacSfAllocReq(Pst *pst,RgInfSfAlloc *sfInfo)
{
   RgCellCb       *cell;
   RgDlSf         *dlSf;
   RgErrInfo      err;
   volatile uint32_t   startTime=0;
   Inst           inst;


   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /*starting Task*/
   SStartTask(&startTime, PID_MAC_SF_ALLOC_REQ);

   if(NULLP == sfInfo)
   {
      return RFAILED;
   }

   if((cell = rgCb[inst].cell) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : No cellCb found with cell");
      return RFAILED;
   }

   dlSf = &cell->subFrms[(sfInfo->timingInfo.slot % RG_NUM_SUB_FRAMES)];

   rgTOMRlsSf(inst,dlSf);
   dlSf->schdTime = sfInfo->timingInfo;

#ifdef LTE_ADV
   rgLaaInitTbInfoLst(cell);
#endif

   /* Fix : syed Ignore Failure Returns and continue processing.
    * Incomplete processing results in state sync loss between MAC-SCH. */
#ifdef EMTC_ENABLE
   if(TRUE == cell->emtcEnable)
   {
      rgEmtcHndl(cell, sfInfo);
   }
#endif
   rgHndlCmnChnl(cell, sfInfo->timingInfo, &sfInfo->cmnLcInfo, &err);

   rgHndlRaResp(cell, sfInfo->timingInfo, &sfInfo->rarInfo, &err);

#ifdef LTE_ADV
#ifdef XEON_SPECIFIC_CHANGES
   CM_MEAS_TIME((cell->crntTime.slot % RG_NUM_SUB_FRAMES), CM_DBG_MAC_TTI_IND, CM_DBG_MAC_B4_UE_SCHD);
#endif
   rgHndlSchdUe(cell, sfInfo->timingInfo, &sfInfo->ueInfo, &err);
#ifdef XEON_SPECIFIC_CHANGES
   CM_MEAS_TIME((cell->crntTime.slot % RG_NUM_SUB_FRAMES), CM_DBG_MAC_TTI_IND, CM_DBG_MAC_UE_SCHD);
#endif
   rgLaaChkAndReqTbs(dlSf,cell, inst);

#else
   rgHndlSchdUe(cell, sfInfo->timingInfo, &sfInfo->ueInfo, &err);
#endif

#ifdef LTE_L2_MEAS
   if(rgHndlUlUeInfo(cell, sfInfo->ulUeInfo.timingInfo, 
	    &sfInfo->ulUeInfo) != ROK)
   {
      return RFAILED;
   }
#endif
#ifdef XEON_SPECIFIC_CHANGES
   CM_MEAS_TIME((cell->crntTime.slot % RG_NUM_SUB_FRAMES), CM_DBG_MAC_TTI_IND, CM_DBG_MAC_MEAS);
#endif
   /* Fix[ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */


   /* Added the handling for pushing down
    * TFU Data request in the retransmission only scenario.*/ 
#ifdef LTEMAC_DLUE_TMGOPTMZ
   dlSf->statIndDone = TRUE;
   /* Fix [ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */
   if(!(dlSf->txDone) && 
#ifdef LTE_ADV
	 (TRUE == rgLaaChkAllRxTbs(dlSf)) && 
#endif
	 (0 == dlSf->remDatReqCnt) && (dlSf->statIndDone) && 
	 (RG_TIMEINFO_SAME(cell->crntTime, dlSf->schdTime)))
   {
      /*This is the case of rettransmission, so no need
       * to wait for TTI Ind to push TFU Data Request. Send
       * it right away.*/
      if (ROK != rgTOMUtlProcDlSf (dlSf, cell, &err))
      {
	 DU_LOG("\nERROR  -->  MAC : Unable to process downlink subframe for cell");
	 err.errType = RGERR_ROM_DEDDATREQ;
      }
      /* Mark this frame as sent */
      dlSf->txDone = TRUE;
   }
#endif
   if (sfInfo->flowCntrlInfo.numUes > 0)
   {
      rgHndlFlowCntrl(cell,sfInfo);
   }
   /*stoping Task*/
   SStopTask(startTime, PID_MAC_SF_ALLOC_REQ);
   return ROK;
} /* end of RgSchMacSfAllocReq */
/**
 * @brief Handler for processing data indication recieved from PHY for UEs.
 *
 * @details
 *
 *     Function: rgTOMProcCrntiCEInDatInd
 *
 *     Handler for processing data indication recieved from PHY for UEs.
 *
 *     Invoked by: RgLiTfuDatInd of LIM 
 *
 *     Processing Steps: 
 *     For each DataInfo recieved
 *      - If  received a CRNTI control element 
 *          - Check if a CCCH SDU is present, if it is return failure
 *          - Check for the existence of UE, if its isnt present return failure.
 *          - Delegate the remaining processing to rgTOMUtlProcMsg3 which
 *          primarily informs the scheduler about the data received and
 *          generates Data indications towards the higher layer.
 *           
 *  @param      RgMacPdu          *pdu,
 *  @param      RgUeCb            *prevUeCb,
 *  @param      RgCellCb          *cellCb,
 *  @param      TfuDatInfo        *datInfo,
 *  @param      RgInfCeInfo       *ceInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
   static S16 rgTOMProcCrntiCEInDatInd
(
 RgMacPdu          *pdu,
 RgUeCb            *prevUeCb,
 RgCellCb          *cellCb,
 TfuDatInfo        *datInfo,
 RgInfCeInfo       *ceInfo,
 uint16_t               slot
 )
{
   RgUeCb *ueCb = NULLP;
   Inst   inst  = cellCb->macInst - RG_INST_START;


#ifdef LTEMAC_SPS
   Bool spsToBeActvtd;
   uint16_t  sduSize;
#endif


#ifndef LTE_L2_MEAS      
   UNUSED(slot);
#endif

   ueCb = rgDBMGetUeCbFromRachLst (cellCb, datInfo->rnti);

   if (ueCb == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : RNTI:%d Received MSG3 with CRNTI,unable to find ueCb", ceInfo->ces.cRnti);
      return RFAILED;
   }

   prevUeCb = rgDBMGetUeCb (cellCb, ceInfo->ces.cRnti);
   if (prevUeCb == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : RNTI:%d Received MSG3 with CRNTI,unable to find ueCb", ceInfo->ces.cRnti);
      return RFAILED;
   }
   DU_LOG("\nERROR  -->  MAC : CRNTI CE(%d) received through tmpCrnti(%d)",
	 ceInfo->ces.cRnti, datInfo->rnti);
   rgDBMDelUeCbFromRachLst(cellCb, ueCb);
   rgRAMFreeUeCb(inst,ueCb);
   ueCb = prevUeCb;
#ifdef LTEMAC_SPS
   if ((rgTOMUtlProcMsg(cellCb, ueCb, pdu, FALSE,&spsToBeActvtd,&sduSize, slot, NULLP)) != ROK)
#else
      if ((rgTOMUtlProcMsg (cellCb, ueCb, pdu, slot, NULLP)) != ROK)
#endif /* LTEMAC_SPS */
      {
	 DU_LOG("\nERROR  -->  MAC : RNTI:%d Processing for MSG3 failed",datInfo->rnti);
	 return RFAILED;
      }
   return ROK;
}
/**
 * @brief Handler for processing data indication recieved from PHY for UEs.
 *
 * @details
 *
 *     Function: rgTOMProcCCCHSduInDatInd
 *
 *     Handler for processing data indication recieved from PHY for UEs.
 *
 *     Invoked by: RgLiTfuDatInd of LIM 
 *
 *     Processing Steps: 
 *     For each DataInfo recieved
 *      - If only CCCH SDU is present
 *        - Invoke rgTOMUtlProcMsg3 for further processing.
 *      - If its a non-Msg3 PDU i.e. received outside of a RA procedure
 *        - Retrieve the UeCB 
 *        - Validate that the received PDU contains only configured Logical
 *        Channels.
 *        - Invoke rgTOMUtlProcDatPdu for further processing. It informs the
 *        scheduler with the information of the received Data and generates
 *        DatIndications towards the higher layers. 
 *           
 *  @param  TfuDatIndInfo *datInd
 *  @param  RgMacPdu          *pdu,
 *  @param  RgUeCb            *prevUeCb,
 *  @param  RgCellCb          *cellCb,
 *  @param  TfuDatInfo        *datInfo,
 *  @param  RgInfCeInfo       *ceInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
   static S16 rgTOMProcCCCHSduInDatInd
(
 RgMacPdu          *pdu,
 RgUeCb            *prevUeCb,
 RgCellCb          *cellCb,
 TfuDatInfo        *datInfo,
 RgInfCeInfo       *ceInfo,
 uint16_t               slot 
 )
{
   RgUeCb *ueCb = NULLP;
   Inst   inst  = cellCb->macInst - RG_INST_START;

#ifdef LTEMAC_SPS
   Bool spsToBeActvtd;
   uint16_t  sduSize;
#endif



#ifndef LTE_L2_MEAS      
   UNUSED(slot);
#endif

   if (ceInfo->bitMask & RG_CRNTI_CE_PRSNT)
   {
      DU_LOG("\nERROR  -->  MAC : CRNTI:%d Received MSG3 with CCCH",ceInfo->ces.cRnti);
      return RFAILED;
   }

   ueCb = rgDBMGetUeCbFromRachLst (cellCb, datInfo->rnti);

   if (ueCb == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : RNTI:%d Processing for MSG3 failed", datInfo->rnti);
      return RFAILED;
   }
   /* Fix: syed Drop any duplicate Msg3(CCCH Sdu) */
   if (ueCb->dl.hqEnt.numHqProcs)
   {
      /* HqE is already initialized by a previuos Msg3 */ 
      DU_LOG("\nERROR  -->  MAC : RNTI:%d Processing for MSG3 failed. Duplicate "
	    "MSG3 received. Dropping", datInfo->rnti);
      return RFAILED;
   }

   if(rgDHMHqEntInit(inst,&ueCb->dl.hqEnt,
	    cellCb->maxDlHqProcPerUe) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RNTI:%d Harq Initialization failed ", 
	    datInfo->rnti);
      return RFAILED;
   }
   DU_LOG("\nDEBUG  -->  MAC : CCCH SDU received through tmpCrnti(%d)",datInfo->rnti);
#ifdef LTEMAC_SPS
   if ((rgTOMUtlProcMsg(cellCb, ueCb, pdu, FALSE,&spsToBeActvtd,&sduSize, slot, NULLP)) != ROK)
#else
      if ((rgTOMUtlProcMsg (cellCb, ueCb, pdu, slot, NULLP)) != ROK)
#endif /* LTEMAC_SPS */
      {
	 DU_LOG("\nERROR  -->  MAC : RNTI:%d Processing for MSG3 failed", 
	       datInfo->rnti);
	 return RFAILED;
      }
   return ROK;
}

#ifdef LTE_L2_MEAS

/** @brief This function captures the BSR value from Control Element
 *  Info structure and updates the effective Buffer size into the 
 *  corresponding LCG ID. 
 *
 * @details 
 *
 *     Function: rgTOMUtlL2MStoreBufSz
 *
 *         Processing steps:
 *         - update/append the Data structure based on BSR type 
 *
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] RgInfCeInfo *ceInfo
 * @return S16
 */

static S16 rgTOMUtlL2MStoreBufSz( RgUeCb *ueCb, RgInfCeInfo *ceInfo )
{
   uint8_t lcgId;
   uint8_t bsr;

   if(ceInfo->bitMask & RG_TRUNC_BSR_CE_PRSNT)
   {
      lcgId = ((ceInfo->ces.bsr.truncBsr >> 6) & 0x03);
      bsr = ceInfo->ces.bsr.truncBsr & 0x3F;
      ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs = rgLwrBsrTbl[bsr];
   }
   else if(ceInfo->bitMask & RG_SHORT_BSR_CE_PRSNT)
   {
      lcgId = ((ceInfo->ces.bsr.shortBsr >> 6) & 0x03);
      bsr = ceInfo->ces.bsr.shortBsr & 0x3F;
      ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs = rgLwrBsrTbl[bsr];

   }
   else if(ceInfo->bitMask & RG_LONG_BSR_CE_PRSNT)
   {
      ueCb->ul.lcgArr[0].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs1];
      ueCb->ul.lcgArr[1].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs2];
      ueCb->ul.lcgArr[2].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs3];
      ueCb->ul.lcgArr[3].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs4];
   }
   return ROK;
} /* end of rgTOMUtlL2MStoreBufSz*/

/** @brief : Compiles list of LCs received in UL data for DTCH RBs
 *
 * @details
 *
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] CmLteRnti  rnti
 * @param  [in] RgMacPdu   *pdu
 * @param 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static Void rgTOML2MCompileActiveLCs(RgCellCb *cellCb,RgUeCb *ueCb,RgMacPdu *pdu,RgInfCeInfo *ceInfo)
{
   CmLList           *node;
   RgMacSdu          *sdu;
   RgUlLcCb          *ulLcCb;


   node =  pdu->sduLst.first;
   while (node)
   {
      sdu = (RgMacSdu*)node->node;

      if ((ulLcCb = rgDBMGetUlDedLcCb(ueCb, sdu->lcId)), ulLcCb != NULLP)
      {
	 if (ulLcCb->lcgId != 0)
	 {
	    ceInfo->bitMask |= RG_ACTIVE_LC_PRSNT;
	    ceInfo->ulActLCs[ulLcCb->lcId - 1] = TRUE;
	 }
      }
      node = node->next;
   }

} /* end of */ 



#endif
/**********************************************************************

  End of file
 **********************************************************************/
