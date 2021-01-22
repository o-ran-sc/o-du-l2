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
/* header include files -- defines (.h) */
#include "common_def.h"
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* customisable defines and macros for MAC */
#include "rg.h"            /* defines and macros for MAC */

/* header/extern include files (.x) */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"    /* PRG interface typedefs */
#include "rg.x"            /* typedefs for MAC */

/* local externs */
#ifdef UNUSED_FUNC
static S16  rgLIMValidateSap ARGS((Inst inst,SuId suId));
#endif
static Void rgLIMUtlFreeDatIndEvnt ARGS((TfuDatIndInfo *datInd,
                                          Bool error));
#ifdef RG_UNUSED
static Void rgLIMUtlFreeDatReqEvnt ARGS((TfuDatReqInfo *datReq,
                                          Bool error));
#endif
/* forward references */

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
 *  @param[in]  uint8_t    status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuBndCfm 
(
Pst     *pst,
SuId    suId, 
uint8_t status
)
{
   Inst inst;
   S16 ret;
   RgLowSapCb  *tfuSap;



   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /* Lets validate suId first */
   /* CA_Change */
   tfuSap = &(rgCb[inst].tfuSap);

   if (suId != tfuSap->sapCfg.suId)
   {
      DU_LOG("\nERROR  -->  MAC : Incorrect SuId. Configured (%d) Recieved (%d)",
            tfuSap->sapCfg.suId, suId);
      return RFAILED;
   }
   ret = rgLMMBndCfm (pst, suId, status);
   return (ret);
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
static S16 rgLIMValidateSap(Inst  inst,SuId suId)
{
   RgLowSapCb  *tfuSap;


   tfuSap = &(rgCb[inst].tfuSap);

   /* First lets check the suId */
   if( suId != tfuSap->sapCfg.suId)
   {
      DU_LOG("\nERROR  -->  MAC : Incorrect SuId. Configured (%d) Recieved (%d)",
            tfuSap->sapCfg.suId, suId);
      return RFAILED;
   }
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      DU_LOG("\nERROR  -->  MAC : Lower SAP not enabled SuId (%d)",
            tfuSap->sapCfg.suId);
      return RFAILED;
   }
   return ROK;
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
static Void rgLIMUtlFreeDatIndEvnt(TfuDatIndInfo *datInd,Bool error)
{

   TfuDatInfo     *datInfo;
   CmLList        *node;

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
   return;
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
S16 RgLiTfuDatInd
(
Pst                *pst, 
SuId               suId, 
TfuDatIndInfo    *datInd
)
{
   Inst             inst;
   S16              ret;
   volatile uint32_t     startTime=0;


  // DU_LOG("5GTF:: DatindRcvd\n");

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /*starting Task*/
   SStartTask(&startTime, PID_MAC_TFU_DATIND);

#ifndef NO_ERRCLS 
   if ((ret = rgLIMValidateSap (inst,suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : SAP Validation failed");
      rgLIMUtlFreeDatIndEvnt(datInd, TRUE);
      return (ret);
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

   return (ret);
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
static Void rgLIMUtlFreeDatReqEvnt(TfuDatReqInfo *datReq,Bool  error)
{

   TfuDatReqPduInfo *datInfo;
   CmLList          *node;
   uint8_t          i;

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
   return;
} /* end of rgLIMUtlFreeDatReqEvnt*/
#endif

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
S16 rgLIMTfuDelDatReq(Inst inst,TfuDelDatReqInfo *delDatReq)
{
   S16         ret;
   RgLowSapCb  *tfuSap;


   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb[inst].tfuSap);

#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      DU_LOG("\nERROR  -->  MAC : Lower SAP not bound (%d)",
            tfuSap->sapSta.sapState);
      return RFAILED;
   }
#endif

   if((ret = RgLiTfuDelDatReq (&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId, 
                            delDatReq)) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Call to RgLiTfuDelDatReq() failed");
   }
   return (ret);
}  /* rgLIMTfuDatReq*/
#endif /*L2_OPTMZ */

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
S16 RgLiTfuNonRtInd(Pst  *pst,SuId  suId)
{

#ifdef NO_ERRCLS
   if (rgLIMValidateSap (pst->dstInst - RG_INST_START, suId) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RgLiTfuNonRtInd() SAP Validation failed.\n");
      return RFAILED;
   }
#endif
   rgDHMFreeTbBufs(pst->dstInst - RG_INST_START);
   return ROK;
}  /* RgLiTfuNonRtInd */

#endif
/**********************************************************************
 
         End of file
**********************************************************************/
