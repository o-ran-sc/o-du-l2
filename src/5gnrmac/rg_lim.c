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
  
     File:     rg_lim.c 
  
**********************************************************************/

/** @file rg_lim.c.
@brief It has APIs exposed by Lower Interface Modulue of MAC. It acts as an 
Interface handler for lower interface APIs.
*/
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=182;
static int RLOG_MODULE_ID=4096;
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"       /* common tokens */
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* customisable defines and macros for MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "du_log.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"    /* PRG interface typedefs */
#include "du_app_mac_inf.h"
#include "rg.x"            /* typedefs for MAC */

#include "mac_upr_inf_api.h"
#include "mac.h"

/* local externs */
#ifdef UNUSED_FUNC
PRIVATE S16  rgLIMValidateSap ARGS((Inst inst,SuId suId));
#endif
PRIVATE Void rgLIMUtlFreeDatIndEvnt ARGS((TfuDatIndInfo *datInd,
                                          Bool error));

/* function pointers for packing slot ind from mac to sch */
//S16 packMacSchSlotInd(Pst *pst, SlotIndInfo *slotInd);

MacSchSlotIndFunc macSchSlotIndOpts[] =
{
   packMacSchSlotInd,  
   macSchSlotInd,                                             
   packMacSchSlotInd
}; 

#ifdef RG_UNUSED
PRIVATE Void rgLIMUtlFreeDatReqEvnt ARGS((TfuDatReqInfo *datReq,
                                          Bool error));
#endif
/* forward references */

/**
 * @brief This API is invoked to send TFU SAP bind request to PHY.
 *
 * @details
 *
 *     Function : rgLIMTfuBndReq
 *      
 *      This API is invoked to send TFU SAP bind request to PHY. It fills in 
 *      the Pst structure, spId and suId values and invokes bind request
 *      primitive at TFU.
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  SuId            suId 
 *  @param[in]  SpId            spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgLIMTfuBndReq
(
Inst    inst,
SuId    suId, 
SpId    spId
)
#else
PUBLIC S16 rgLIMTfuBndReq(inst,suId, spId)
Inst    inst;
SuId    suId; 
SpId    spId;
#endif
{
   S16         ret;
   RgLowSapCb  *tfuSap;
   Pst         pst;

   TRC2(rgLIMTfuBndReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb[inst].tfuSap);
   (Void)cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuBndReq (&pst, suId, spId)) != ROK)
   {
      RLOG0(L_ERROR,"Call to RgLiTfuBndReq() failed");
   }
   RETVALUE(ret);
}  /* rgLIMTfuBndReq */


/**
 * @brief This API is invoked to send TFU SAP unbind request to PHY.
 *
 * @details
 *
 *     Function : rgLIMTfuBndReq
 *      
 *      This API is invoked to send TFU SAP unbind request to PHY. It fills in 
 *      the Pst structure and spId value and invokes unbind request
 *      primitive at TFU.
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  SpId            spId
 *  @param[in]  Reason          reason 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgLIMTfuUbndReq
(
Inst    inst,
SpId    spId, 
Reason  reason
)
#else
PUBLIC S16 rgLIMTfuUbndReq(inst,spId, reason)
Inst    inst;
SpId    spId; 
Reason  reason;
#endif
{
   S16         ret;
   RgLowSapCb  *tfuSap;
   Pst         pst;

   TRC2(rgLIMTfuUbndReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb[inst].tfuSap);
   cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuUbndReq (&pst, tfuSap->sapCfg.spId, reason)) != ROK)
   {
      RLOG0(L_ERROR,"Call to RgLiTfuUbndReq() failed");
   }
   RETVALUE(ret);

}  /* rgLIMTfuUbndReq */


/**
 * @brief Bind confirm API for TFU SAP 
 *
 * @details
 *
 *     Function : RgLiTfuBndCfm
 *      
 *      This API is invoked by PHY to confirm TFU SAP bind. 
 *     
 *           
 *  @param[in]  Pst   *pst 
 *  @param[in]  SuId  suId 
 *  @param[in]  U8    status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuBndCfm 
(
Pst     *pst,
SuId    suId, 
U8      status
)
#else
PUBLIC S16 RgLiTfuBndCfm(pst, suId, status)
Pst     *pst; 
SuId    suId; 
U8      status;
#endif
{
   Inst inst;
   S16 ret;
   RgLowSapCb  *tfuSap;

   TRC3(RgLiTfuBndCfm);


   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /* Lets validate suId first */
   /* CA_Change */
   tfuSap = &(rgCb[inst].tfuSap);

   if (suId != tfuSap->sapCfg.suId)
   {
      RLOG2(L_ERROR,"Incorrect SuId. Configured (%d) Recieved (%d)",
            tfuSap->sapCfg.suId, suId);
      RETVALUE(RFAILED);
   }
   ret = rgLMMBndCfm (pst, suId, status);
   RETVALUE(ret);
}  /* RgLiTfuBndCfm */

 /** @brief This function Validates the SAP information received along with the
  * primitive from the lower layer. 
  * Function:
  *   Validates SAP information.
 *  @param[in]  Inst        inst
  * @param  suId The SAP Id
  * @return 
  *   -# ROK
  *   -# RFAILED
  */
#ifdef UNUSED_FUNC
#ifdef ANSI
PRIVATE S16 rgLIMValidateSap
(
 Inst    inst,
 SuId    suId
)
#else
PRIVATE S16 rgLIMValidateSap(inst,suId)
 Inst    inst;
 SuId    suId;
#endif
{
   RgLowSapCb  *tfuSap;

   TRC2(rgLIMValidateSap)

   tfuSap = &(rgCb[inst].tfuSap);

   /* First lets check the suId */
   if( suId != tfuSap->sapCfg.suId)
   {
      RLOG2(L_ERROR,"Incorrect SuId. Configured (%d) Recieved (%d)",
            tfuSap->sapCfg.suId, suId);
      RETVALUE(RFAILED);
   }
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RLOG1(L_ERROR,"Lower SAP not enabled SuId (%d)",
            tfuSap->sapCfg.suId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of rgLIMValidateSap */
#endif
/** @brief This function frees up the TfuDatIndInfo structure
 *
 * @details
 *
 *     Function: rgLIMUtlFreeDatIndEvnt 
 *       - Function frees up the TfuDatIndInfo structure, in case of error it shall
 *       free up the buffer's present in the datIndLst.
 *
 *         Processing steps:
 * @param  [in] TfuDatIndInfo *datInd
 * @param  [in] Bool          *error
 * @return 
 */
#ifdef ANSI
PRIVATE Void rgLIMUtlFreeDatIndEvnt 
(
 TfuDatIndInfo *datInd,
 Bool          error
 )
#else
PRIVATE Void rgLIMUtlFreeDatIndEvnt(datInd, error)
 TfuDatIndInfo *datInd;
 Bool          error;
#endif
{

   TfuDatInfo     *datInfo;
   CmLList        *node;

   TRC2(rgLIMUtlFreeDatIndEvnt);
   /* Steps of freeing up the TfuDatInd.
    * 1. loop through the datIndLst and free up all the buffers.
    * 2. free up the whole event
    */
   if ((error == TRUE) && (datInd->datIndLst.count > 0))
   {
      node =  datInd->datIndLst.first;
      while (node)
      {
         datInfo = (TfuDatInfo*)node->node;
         RG_FREE_MSG(datInfo->mBuf);
         node = node->next;
      }
   }
   RG_FREE_MEM(datInd);
   RETVOID;
} /* end of rgLIMUtlFreeDatIndEvnt*/

/**
 * @brief Downlink data indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuDatInd
 *      
 *      This API is invoked by PHY to send data indication to MAC on 
 *      recieving data from UEs.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDatIndInfo    *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuDatInd
(
Pst                *pst, 
SuId               suId, 
TfuDatIndInfo    *datInd
)
#else
PUBLIC S16 RgLiTfuDatInd(pst, suId, datInd)
Pst                *pst; 
SuId               suId; 
TfuDatIndInfo    *datInd;
#endif
{
   Inst             inst;
   S16              ret;
   VOLATILE U32     startTime=0;

   TRC3(RgLiTfuDatInd);

  // printf("5GTF:: DatindRcvd\n");

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /*starting Task*/
   SStartTask(&startTime, PID_MAC_TFU_DATIND);

#ifndef NO_ERRCLS 
   if ((ret = rgLIMValidateSap (inst,suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,datInd->cellId,"SAP Validation failed");
      rgLIMUtlFreeDatIndEvnt(datInd, TRUE);
      RETVALUE(ret);
   }
#endif
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   rgCb[inst].tfuSap.sapSts.numPduRcvd += 
                    datInd->datIndLst.count;
   ret = rgTOMDatInd(inst,datInd);
   /* Fix: sriky memory corruption precautions */
   /* Free up the memory for the request structure */
   if (ret == ROK)
   {
      rgLIMUtlFreeDatIndEvnt(datInd, FALSE);
   }
   else
   {
      rgLIMUtlFreeDatIndEvnt(datInd, TRUE);
   }

   /*stoping Task*/
   SStopTask(startTime, PID_MAC_TFU_DATIND);

   RETVALUE(ret);
}  /* RgLiTfuDatInd*/

#ifdef RG_UNUSED
/** @brief This function frees up the TfuDatReqInfo structure.
 *
 * @details
 *
 *     Function: rgLIMUtlFreeDatReqEvnt
 *       - Function frees up the TfuDatReqInfo structure, in case of error it shall
 *       free up the buffer's present in the PDUs list.
 *
 *         Processing steps:
 * @param  [in] TfuDatReqInfo *datReq
 * @param  [in] Bool          *error
 * @return 
 */
#ifdef ANSI
PRIVATE Void rgLIMUtlFreeDatReqEvnt
(
 TfuDatReqInfo *datReq,
 Bool          error
 )
#else
PRIVATE Void rgLIMUtlFreeDatReqEvnt(datReq, error)
 TfuDatReqInfo *datReq;
 Bool          error;
#endif
{

   TfuDatReqPduInfo *datInfo;
   CmLList          *node;
   U8               i;

   TRC2(rgLIMUtlFreeDatReqEvnt);
   /* Steps of freeing up the TfuDatReq.
    * 1. Free the bch buffer.
    * 2. loop through the pdus list and free up all the buffers.
    * 3. free up the whole event
    */
   if (error)
   {
      if (datReq->bchDat.pres == PRSNT_NODEF)
      {
         RG_FREE_MSG(datReq->bchDat.val);
      }
      if (datReq->pdus.count > 0)
      {
         node =  datReq->pdus.first;
         while (node)
         {
            datInfo = (TfuDatReqPduInfo*)node->node;
            for (i=0; i<datInfo->nmbOfTBs; i++)
            {
               if (datInfo->mBuf[i] != NULLP)
               {
                  RG_FREE_MSG(datInfo->mBuf[i]);
               }
            }
            node = node->next;
         }
      }
   }
   RG_FREE_MEM(datReq);
   RETVOID;
} /* end of rgLIMUtlFreeDatReqEvnt*/
#endif
/**
 * @brief This API is invoked to send Data to PHY.
 *
 * @details
 *
 *     Function : rgLIMTfuDatReq
 *      
 *      This API is invoked to send Data to PHY. It 
 *      fills in the Pst structure, spId value and invokes Data
 *      request primitive at TFU.
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  TfuDatReqInfo *datReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgLIMTfuDatReq 
(
Inst          inst,
TfuDatReqInfo *datReq
)
#else
PUBLIC S16 rgLIMTfuDatReq(inst,datReq)
Inst          inst;
TfuDatReqInfo *datReq;
#endif
{
   S16         ret;
   RgLowSapCb  *tfuSap;

   TRC2(rgLIMTfuDatReq)

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb[inst].tfuSap);

#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,datReq->cellId,"Lower SAP not bound (%d)",
            tfuSap->sapSta.sapState);
#ifdef RG_UNUSED
      /* This case will never be hit if sap is not bound then we dont get TTI */
      rgLIMUtlFreeDatReqEvnt(datReq, TRUE);
#endif
      RETVALUE(RFAILED);
   }
#endif

   tfuSap->sapSts.numPduTxmit += datReq->pdus.count; 

   /* Using existing pst - for optimization */
   if((ret = RgLiTfuDatReq (&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId, 
                            datReq)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,datReq->cellId,"Call to RgLiTfuDatReq() failed");
   }
   RETVALUE(ret);
}  /* rgLIMTfuDatReq*/

#ifdef L2_OPTMZ
/**
 * @brief This API is invoked to send Data to PHY.
 *
 * @details
 *
 *     Function : rgLIMTfuDelDatReq
 *      
 *      This API is invoked to send Data to PHY. It 
 *      fills in the Pst structure, spId value and invokes Data
 *      request primitive at TFU.
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  TfuDelDatReqInfo *datReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgLIMTfuDelDatReq 
(
Inst          inst,
TfuDelDatReqInfo *delDatReq
)
#else
PUBLIC S16 rgLIMTfuDatReq(inst,delDatReq)
Inst          inst;
TfuDelDatReqInfo *delDatReq;
#endif
{
   S16         ret;
   RgLowSapCb  *tfuSap;

   TRC2(rgLIMTfuDelDatReq)

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb[inst].tfuSap);

#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,delDatReq->cellId,"Lower SAP not bound (%d)",
            tfuSap->sapSta.sapState);
      RETVALUE(RFAILED);
   }
#endif

   if((ret = RgLiTfuDelDatReq (&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId, 
                            delDatReq)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,delDatReq->cellId,"Call to RgLiTfuDelDatReq() failed");
   }
   RETVALUE(ret);
}  /* rgLIMTfuDatReq*/
#endif /*L2_OPTMZ */

void fapiMacConfigRsp()
{
   /* TODO : Processing of conig response from PHY */

   /* Send cell config cfm to DU APP */
   MacSendCellCfgCfm(RSP_OK);
}

/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : sendSlotIndMacToSch
 * 
 *      This API is invoked by MAC to send slot ind to scheduler.
 *           
 *  @param[in]  SlotIndInfo    *slotInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
int sendSlotIndMacToSch(SlotIndInfo *slotInd)
{
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;
   pst.srcProcId = 0;
   pst.dstProcId = 0;
   pst.srcEnt = ENTRG;
   pst.dstEnt = ENTRG;
   pst.srcInst = 0;
   pst.dstInst = 1;
   pst.event = EVENT_SLOT_IND_TO_SCH;
   pst.region = 0;
   pst.pool =  0;
   pst.selector = MAC_SELECTOR_TC;

   return(*macSchSlotIndOpts[pst.selector])(&pst,slotInd);
}


/*******************************************************************
 *
 * @brief Send slot indication to DU APP
 *
 * @details
 *
 *    Function : sendSlotIndMacToDuApp
 *
 *    Functionality:
 *       Send slot indication to DU APP
 *
 * @params[in] Slot indication info 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendSlotIndMacToDuApp(SlotIndInfo *slotInd)
{
   Pst pst;
   SlotInfo  *slotInfo;
  
   /* Send Slot Indication to DU APP */
   MAC_ALLOC(slotInfo, sizeof(SlotInfo));
   if(!slotInfo)
   {
      DU_LOG("\nMAC : Slot Indication memory allocation failed");
      return RFAILED;
   }

   slotInfo->cellId = macCb.macCell->cellId;
   slotInfo->sfn = slotInd->sfn;
   slotInfo->slot = slotInd->slot;

   /* Fill Pst */
   pst.selector  = DU_MAC_LWLC;
   pst.srcEnt    = ENTRG;
   pst.dstEnt    = ENTDUAPP;
   pst.dstInst   = 0;
   pst.srcInst   = macCb.macInst;
   pst.dstProcId = rgCb[pst.srcInst].rgInit.procId;
   pst.srcProcId = rgCb[pst.srcInst].rgInit.procId;
   pst.region = MAC_MEM_REGION;
   pst.pool = MAC_POOL;
   pst.event = EVENT_MAC_SLOT_IND;
   pst.route = 0;
   pst.prior = 0;
   pst.intfVer = 0;
 
   return MacDuAppSlotInd(&pst, slotInfo);
  
}

/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : fapiMacSlotInd 
 *      
 *      This API is invoked by PHY to indicate TTI indication to MAC for a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @param[in]  SlotIndInfo    *slotInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
PUBLIC S16 fapiMacSlotInd 
(
Pst                 *pst, 
SlotIndInfo         *slotInd
)
{
   S16              ret;
   VOLATILE U32     startTime=0;
   Inst             inst;

   DU_LOG("\nMAC : Slot Indication received");
   
   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /*starting Task*/
   SStartTask(&startTime, PID_MAC_TTI_IND);

   /* send slot indication to scheduler */
   ret = sendSlotIndMacToSch(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : Sending of slot ind msg from MAC to SCH failed");
      RETVALUE(ret);
   }

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = macProcessSlotInd(inst,*slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : macProcessSlotInd failed");
      RETVALUE(ret);
   }

   /* send slot indication to du app */
   ret = sendSlotIndMacToDuApp(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC :Sending of slot ind msg from MAC to DU APP failed");
      RETVALUE(ret);
   }

   /*stoping Task*/
   SStopTask(startTime, PID_MAC_TTI_IND);

   RETVALUE(ret);
}  /* fapiMacSlotInd */

#if defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD)
 /**
 * @brief Transmission of non-rt indication from CL.
 *
 * @details
 *
 *     Function : RgLiTfuNonRtInd 
 *      
 *      This API is invoked by CL to indicate non-rt processing indication to MAC for a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuNonRtInd
(
Pst                 *pst,
SuId                suId
)
#else
PUBLIC S16 RgLiTfuNonRtInd(pst, suId)
Pst                 *pst;
SuId                suId;
#endif
{
   TRC3(RgLiTfuNonRtInd);

#ifdef NO_ERRCLS
   if (rgLIMValidateSap (pst->dstInst - RG_INST_START, suId) != ROK)
   {
      RGDBGERRNEW(pst->dstInst - RG_INST_START, (rgPBuf(pst->dstInst - RG_INST_START),"RgLiTfuNonRtInd() SAP Validation failed.\n"));
      RETVALUE(RFAILED);
   }
#endif
   rgDHMFreeTbBufs(pst->dstInst - RG_INST_START);
   RETVALUE(ROK);
}  /* RgLiTfuNonRtInd */

#endif
/**********************************************************************
 
         End of file
**********************************************************************/
