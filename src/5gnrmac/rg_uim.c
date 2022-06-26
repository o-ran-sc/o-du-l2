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
  
     File:     rg_uim.c 
  
**********************************************************************/

/** @file rg_uim.c.
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/


/* header include files -- defines (.h) */

/* header/extern include files (.x) */
/* header include files (.h) */
#include "common_def.h"
#include "crg.h"           /* CRG defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "tfu.h"
#include "rgu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"        /* customizable defines and macros for MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "rg_err.h"        /* RG error defines */
#include "mac_utils.h"

/* header/extern include files (.x) */

#include "crg.x"           /* CRG types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "tfu.x"
#include "rgu.x"
#include "rg_sch_inf.x"
#include "rg_prg.x"        /* PRG interface typedefs*/
#include "du_app_mac_inf.h"
#include "rg.x"            /* typedefs for MAC */

#include "ss_rbuf.h"
#include "ss_rbuf.x"

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

uint8_t rgRguDlSap;
uint8_t rgRguUlSap;
/**
 * @brief Handler for Bind request.
 *
 * @details
 *
 *     Function : RgUiRguBndReq
 *     
 *     This function handles the bind request from MAC Service User.
 *     
 *           
 *  @param[in]  Pst  *pst
 *  @param[in]  SuId suId
 *  @param[in]  SpId spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRguBndReq
(
Pst  *pst,
SuId suId,
SpId spId
)
{
   Inst      inst; 
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgCb[inst].rgInit.region;
   tmpPst.pool        = rgCb[inst].rgInit.pool;
   tmpPst.srcProcId   = rgCb[inst].rgInit.procId;
   tmpPst.srcEnt      = rgCb[inst].rgInit.ent;
   tmpPst.srcInst     = rgCb[inst].rgInit.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   if(spId == rgCb[inst].rguSap[spId].sapCfg.spId)
   {
      /* Check the state of the SAP */
      switch (rgCb[inst].rguSap[spId].sapSta.sapState)
      {
         case LRG_NOT_CFG: /* SAP Not configured */
            rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(inst,LCM_CATEGORY_INTERFACE,LRG_NOT_CFG,
                  LCM_CAUSE_INV_SAP, &dgn);
            DU_LOG("\nDEBUG  -->  MAC : SAP Not Configured");
            ret = RgUiRguBndCfm(&tmpPst, suId, CM_BND_NOK);
            break;
         case LRG_UNBND: /* SAP is not bound */
            DU_LOG("\nDEBUG  -->  MAC : SAP Not yet bound");
            rgCb[inst].rguSap[spId].sapSta.sapState = LRG_BND;
            rgCb[inst].rguSap[spId].sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            /*T2K - Passing spId as it is required to access the SAP CB*/
            ret = rgUIMRguBndCfm(inst,spId, CM_BND_OK);
            /* Indicate to Layer manager */ 
            rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(inst,LCM_CATEGORY_INTERFACE,LRG_EVENT_RGUSAP_ENB,
                  LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            DU_LOG("\nDEBUG  -->  MAC : SAP already bound");
            /*T2K - Passing spId as it is required to access the SAP CB*/
            ret = rgUIMRguBndCfm(inst,spId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            DU_LOG("\nERROR  -->  MAC : Invalid SAP State:RgUiRguBndReq failed\n");
#endif
            /*T2K - Passing spId as it is required to access the SAP CB*/
            ret = rgUIMRguBndCfm(inst,spId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  MAC : Invalid SAP Id:RgUiRguBndReq failed\n");
#endif
      /*T2K - Passing spId as it is required to access the SAP CB*/
      ret = rgUIMRguBndCfm(inst,spId, CM_BND_NOK);
   }
   return (ret);
}  /* RgUiRguBndReq */


/**
 * @brief Handler for Unbind request.
 *
 * @details
 *
 *     Function : RgUiRguUbndReq
 *     
 *     This function handles the unbind request from MAC Service User.
 *     
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SpId   spId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRguUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
{
   Inst      inst;

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /* SAP Id validation */
   if (spId == rgCb[inst].rguSap[spId].sapCfg.spId)
   {
      switch(rgCb[inst].rguSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            DU_LOG("\nDEBUG  -->  MAC : SAP already bound");
            /* setting SAP state to UN BOUND */
            rgCb[inst].rguSap[spId].sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
     DU_LOG("\nERROR  -->  MAC : Invalid SAP State:%d RgUiRguUbndReq failed",
                  rgCb[inst].rguSap[spId].sapSta.sapState);
       
#endif
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
     DU_LOG("\nERROR  -->  MAC : Invalid SAP Id:RgUiRguUbndReq failed\n");
#endif
      return RFAILED;
   }
   return ROK;
}  /* RgUiRguUbndReq */
/**
 * @brief API for sending bind confirm from MAC to RLC
 *
 * @details
 *
 *     Function: rgUIMRguBndCfm
 *     
 *     This API is invoked to send bind confirm from MAC to RLC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     bind confirm API towards RLC.
 *           
 *  @param[in] Inst        inst
 *  @param[in]  SuId          suId
 *  @param[in]  uint8_t            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgUIMRguBndCfm(Inst inst,SpId spId,uint8_t status)
{
   S16  ret = ROK;
   
   ret = RgUiRguBndCfm(&rgCb[inst].rguSap[spId].sapCfg.sapPst, 
                      rgCb[inst].rguSap[spId].sapCfg.suId, status);
   if (ret != ROK)
   {
      
      DU_LOG("\nERROR  -->  MAC : RgUiRguBndCfm Failed ");
      return (ret);
   }
   return (ret);
}  /* rgUIMRguBndCfm*/

#ifdef LTE_L2_MEAS

/**
 * @brief Handler for L2M MeasReq from RGU
 *
 * @details
 *
 *     Function :RgUiRguL2MUlThrpMeasReq 
 *     
 *     This function validates SAP and invokes ROM for further processing
 *           
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguL2MUlThrpMeasReqInfo  *measReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRguL2MUlThrpMeasReq 
(
Pst             *pst,
SpId            spId,
RguL2MUlThrpMeasReqInfo  *measReq
)
{
   Inst  inst;

   S16   ret = ROK;

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
#ifndef NO_ERRCLS
   if (measReq == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Input Response Buffer is NULL");
      return RFAILED;
   }

   if (spId == rgCb[inst].rguSap[spId].sapCfg.spId)
   {
      switch (rgCb[inst].rguSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            DU_LOG("\nERROR  -->  MAC : SAP is already bound");
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            DU_LOG("\nERROR  -->  MAC : Invalid SAP State:%d RgUiRguL2MUlThrpMeasReq failed",
                  rgCb[inst].rguSap[spId].sapSta.sapState);
#endif
            return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  MAC : Invalid SAP Id:%d RgUiRguL2MUlThrpMeasReq failed",spId);
#endif
      return RFAILED;
   }
#endif

   ret = rgROML2MUlThrpMeasReq(inst,measReq);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Processing Of Meas Request Failed");
   }

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, (Data *)measReq,sizeof(RguL2MUlThrpMeasReqInfo));
   return (ret);
}  /* RgUiRguL2MUlThrpMeasReq */
#endif

/**
 * @brief Handler for sending staInd to dedicated logical channels of a UE 
 *
 * @details
 *
 *     Function : rgUIMSndDedStaInd
 *     
 *     This function fills SAP and Pst information to send the staInd to
 *     a UE.
 *     
 *           
 *  @param[in] Inst        inst
 *  @param[in] RgUpSapCb  *rguSap 
 *  @param[in]  RgRguDedStaInd  *staInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgUIMSndDedStaInd
(
Inst         inst,
RgUpSapCb    *rguSap,
RgRguDedStaInd  *staInd
)
{
   S16  ret = ROK;
   
   DU_LOG("\nINFO  -->  MAC : rgUIMSndDedStaInd(): staInd = %p;\n", (void *)staInd);
   
   ret = RgUiRguDStaInd(&(rguSap->sapCfg.sapPst), rguSap->sapCfg.suId, 
         staInd);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RgUiRguDStaInd Failed");
      return (ret);
   }
   return (ret);
}  /* rgUIMSndDedStaInd */


/**
 * @brief Handler for sending staInd to a common logical channel.
 *
 * @details
 *
 *     Function : rgUIMSndCmnStaInd
 *     
 *     This function fills SAP and Pst information to send the staInd to
 *     a common logical channel.
 *     
 *           
 *  @param[in] Inst        inst
 *  @param[in] RgUpSapCb  *rguSap 
 *  @param[in]  RgRguCmnStaInd  *staInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgUIMSndCmnStaInd
(
Inst            inst,
RgUpSapCb    *rguDlSap,
RgRguCmnStaInd  *staInd
)
{
   S16  ret = ROK;

   ret = RgUiRguCStaInd(&(rguDlSap->sapCfg.sapPst), rguDlSap->sapCfg.suId, 
         staInd);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RgUiRguCStaInd Failed");
      return (ret);
   }
   return (ret);
}  /* rgUIMSndCmnStaInd */


/**
 * @brief Handler for sending datInd to dedicated logical channels of a UE 
 *
 * @details
 *
 *     Function : rgUIMSndDedDatInd
 *     
 *     This function fills SAP and Pst information to send the datInd to
 *     a UE.
 *     
 *           
 *  @param[in] Inst        inst
 *  @param[in] RgUpSapCb  *rguUlSap 
 *  @param[in]  RgRguDedDatInd  *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgUIMSndDedDatInd
(
Inst         inst,
RgUpSapCb    *rguUlSap,
RgRguDedDatInd  *datInd
)
{
   S16  ret = ROK;

   rguUlSap->sapSts.numPduTxmit += datInd->numLch;
#ifndef SS_RBUF
   ret = RgUiRguDDatInd(&(rguUlSap->sapCfg.sapPst), rguUlSap->sapCfg.suId, 
         datInd);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RgUiRguDdatInd Failed");
      return (ret);
   }
#else
   SRngIncrWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
   SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].pktRate++;
#endif
   return (ret);
}  /* rgUIMSndDedDatInd */


/**
 * @brief Handler for sending datInd to a common logical channel.
 *
 * @details
 *
 *     Function : rgUIMSndCmnDatInd
 *     
 *     This function fills SAP and Pst information to send the datInd to
 *     a common logical channel.
 *     
 *           
 *  @param[in] Inst        inst
 *  @param[in] RgUpSapCb  *rguSap 
 *  @param[in]  RgRguCmnDatInd  *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgUIMSndCmnDatInd(Inst inst,RgUpSapCb *rguUlSap,RgRguCmnDatInd  *datInd)
{
   S16  ret = ROK;

   DU_LOG("\nDEBUG  -->  MAC : rgUIMSndCmnDatInd(): staInd = %p;\n", (void *)datInd);

   rguUlSap->sapSts.numPduTxmit++;

   DU_LOG("\nDEBUG  -->  MAC : rgUIMSndCmnDatInd suId = %d\n", rguUlSap->sapCfg.suId);   
   ret = RgUiRguCDatInd(&(rguUlSap->sapCfg.sapPst), rguUlSap->sapCfg.suId, 
         datInd);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RgUiRguCDatInd Failed");
      return (ret);
   }
   return (ret);
}  /* rgUIMSndCmnDatInd */

/**

 * @brief API for bind request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgBndReq
 *     
 *     This API is invoked by RRC towards MAC to bind CRG SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to RRC.
 *
 *           
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiCrgBndReq(Pst   *pst, SuId  suId,SpId  spId)
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   RgUstaDgn dgn;      /* Alarm diagnostics structure */
   Inst      inst;

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgCb[inst].rgInit.region;
   tmpPst.pool        = rgCb[inst].rgInit.pool;
   tmpPst.srcProcId   = rgCb[inst].rgInit.procId;
   tmpPst.srcEnt      = rgCb[inst].rgInit.ent;
   tmpPst.srcInst     = rgCb[inst].rgInit.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;


   if(spId == rgCb[inst].crgSap.sapCfg.spId)
   {
      /* Check the state of the SAP */
      switch (rgCb[inst].crgSap.sapSta.sapState)
      {
         case LRG_NOT_CFG: /* SAP Not configured */
            
            rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(inst,LCM_CATEGORY_INTERFACE,LRG_NOT_CFG,
                  LCM_CAUSE_INV_SAP, &dgn);
            DU_LOG("\nDEBUG  -->  MAC : SAP Not Configured");
            ret = RgUiCrgBndCfm(&tmpPst, suId, CM_BND_NOK);
            break;
         case LRG_UNBND: /* SAP is not bound */
            DU_LOG("\nERROR  -->  MAC : SAP Not yet bound");
            
            rgCb[inst].crgSap.sapSta.sapState = LRG_BND;
            rgCb[inst].crgSap.sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgUIMCrgBndCfm(inst,suId, CM_BND_OK);
            /* Indicate to Layer manager */
            rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(inst,LCM_CATEGORY_INTERFACE,LRG_EVENT_CRGSAP_ENB,
                  LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            DU_LOG("\nDEBUG  -->  MAC : SAP is already bound");
            
            ret = rgUIMCrgBndCfm(inst,suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            DU_LOG("\nERROR  -->  MAC : Invalid SAP State:%d RgUiCrgBndReq failed",
                  rgCb[inst].crgSap.sapSta.sapState);
#endif
            ret = rgUIMCrgBndCfm(inst,suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  MAC : Invalid SAP Id:%d RgUiCrgBndReq failed",
           rgCb[inst].crgSap.sapCfg.spId);
#endif
      ret = rgUIMCrgBndCfm(inst,suId, CM_BND_NOK);
   }
   return (ret);
}  /* RgUiCrgBndReq */


/**
 * @brief API for unbind request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgUbndReq
 *     
 *     This API is invoked by RRC towards MAC to unbind CRG SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to RRC.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiCrgUbndReq(Pst *pst,SpId spId,Reason reason)
{
   Inst      inst;

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /* SAP Id validation */
   if (spId == rgCb[inst].crgSap.sapCfg.spId)
   {
      switch(rgCb[inst].crgSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            /* setting SAP state to UN BOUND */
            DU_LOG("\nDEBUG  -->  MAC : SAP is already bound");
            
            rgCb[inst].crgSap.sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)
            DU_LOG("\nERROR  -->  MAC : Invalid SAP State:%d RgUiCrgUbndReq failed",
                  rgCb[inst].crgSap.sapSta.sapState);
#endif
            return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  MAC : Invalid SAP Id:%d RgUiCrgUbndReq failed",
            rgCb[inst].crgSap.sapCfg.spId);
#endif
      return RFAILED;
   }
   return ROK;
}  /* RgUiCrgUbndReq */

/**
 * @brief API for sending bind confirm from MAC to RRC
 *
 * @details
 *
 *     Function: rgUIMRgrBndCfm
 *     
 *     This API is invoked to send bind confirm from MAC to RRC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     bind confirm API towards RRC.
 *           
 *  @param[in] Inst        inst
 *  @param[in]  SuId          suId
 *  @param[in]  uint8_t            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgUIMCrgBndCfm(Inst  inst,SuId suId,uint8_t status)
{

   if(RgUiCrgBndCfm(&(rgCb[inst].crgSap.sapCfg.sapPst), rgCb[inst].crgSap.sapCfg.suId, status) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RgUiCrgBndCfm Failed ");
      return RFAILED;
   }

   return ROK;
}  /* rgUIMCrgBndCfm*/

/**
 * @brief API for sending configuration confirm from MAC to RRC
 *
 * @details
 *
 *     Function: rgUIMCrgCfgCfm
 *     
 *     This API is invoked to send configuration confirm from MAC to RRC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     config confirm API towards RRC.
 *           
 *  @param[in] Inst        inst
 *  @param[in]  CrgCfgTransId transId
 *  @param[in]  uint8_t            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgUIMCrgCfgCfm
(
Inst      inst,
CrgCfgTransId transId,
uint8_t       status
)
{
   S16  ret = ROK;
   uint8_t   prntTrans[CRG_CFG_TRANSID_SIZE+1];

   memcpy(prntTrans, transId.trans, CRG_CFG_TRANSID_SIZE);
   prntTrans[CRG_CFG_TRANSID_SIZE] = '\0';


   ret = RgUiCrgCfgCfm(&(rgCb[inst].crgSap.sapCfg.sapPst), rgCb[inst].crgSap.sapCfg.suId, transId, status);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : RgUiCrgCfgCfm Failed ");
      return (ret);
   }

   return (ret);
}  /* rgUIMCrgCfgCfm */

/**********************************************************************
 
         End of file
**********************************************************************/
