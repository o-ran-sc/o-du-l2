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

/* This file contains message handling functionality for DU cell management */

#include "du_sctp.h"
#include "f1ap_msg_hdl.h"

U8 rlcDlCfg = 0;
U8 numRlcDlSaps = 0;
U8 rlcUlCfg = 0;
U8 numRlcMacSaps = 0;
U8 macCfg = 0;
U8 macCfgInst = 0;

extern S16 cmPkLkwCfgReq(Pst *pst, KwMngmt *cfg);
extern S16 cmPkLkwCntrlReq(Pst *pst, KwMngmt *cfg);
extern S16 cmPkLrgCfgReq(Pst *pst, RgMngmt *cfg);

/**************************************************************************
 * @brief Function to fill configs required by RLC
 *
 * @details
 *
 *      Function : duBuildRlcCfg 
 * 
 *      Functionality:
 *           Initiates general Configs towards RLC 
 *     
 * @param[in] Inst  Specifies if RLC UL or RLC DL instance 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duBuildRlcCfg(Inst inst)
{
   KwMngmt   kwMngmt;
   KwGenCfg  *genCfg = NULLP;
   Pst pst;

   DU_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(kwMngmt.t.cfg.s.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->maxUe       = duCfgParam.maxUe;
   genCfg->maxKwuSaps  = 2;
   genCfg->maxUdxSaps  = 1; 
   genCfg->rlcMode     = (inst == RLC_UL_INST) ?
      LKW_RLC_MODE_UL : LKW_RLC_MODE_DL;
   genCfg->timeRes     = 1; 
   genCfg->maxRguSaps  = DEFAULT_CELLS;

   /*----------- Fill lmPst
    * Parameters ---------*/
   genCfg->lmPst.dstProcId = DU_PROC;
   genCfg->lmPst.srcProcId = DU_PROC;
   genCfg->lmPst.dstEnt    = ENTDUAPP;
   genCfg->lmPst.dstInst   = DU_INST;
   genCfg->lmPst.srcEnt    = ENTKW;
   genCfg->lmPst.srcInst   = inst;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   genCfg->lmPst.pool      = RLC_POOL;
   genCfg->lmPst.selector  = DU_SELECTOR_LC;

    /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.msgLen              = 0;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = (Inst)0;
   kwMngmt.hdr.elmId.elmnt         = STGEN;
   kwMngmt.hdr.seqNmb              = 0;
   kwMngmt.hdr.version             = 0;
   kwMngmt.hdr.transId             = 0;
   kwMngmt.hdr.response.prior      = PRIOR0;
   kwMngmt.hdr.response.route      = RTESPEC;
   kwMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   kwMngmt.hdr.response.mem.pool   = DU_POOL;
   kwMngmt.hdr.response.selector   = DU_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstInst   = inst;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   printf("\nRLC Gen Cfg Req sent for inst %d", inst);

   /* Send the request to RLC */
   cmPkLkwCfgReq(&pst, &kwMngmt);

   return ROK;
}

/**************************************************************************
 * @brief Function to fill configs required by RLC
 *
 * @details
 *
 *      Function : duBuildRlcLsapCfg 
 * 
 *      Functionality:
 *           Initiates general Configs towards RLC 
 *     
 * @param[in] Inst  Specifies if RLC UL or RLC DL instance 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duBuildRlcLsapCfg(Ent ent, Inst inst, U8 lsapInst)
{

   KwMngmt    kwMngmt;
   KwSapCfg   *lSap = NULLP;
   Pst        pst;

   DU_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;

   kwMngmt.hdr.response.mem.pool   = RLC_POOL;

   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = DU_PROC;
   pst.dstInst   = inst;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;
   lSap   = &(kwMngmt.t.cfg.s.sap);

   lSap->mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   lSap->mem.pool    = RLC_POOL;
   lSap->mem.spare   = 0;
   lSap->bndTmrIntvl = 10;
   lSap->priority    = PRIOR0;
   lSap->route       = RTESPEC;
   if (ent == ENTRG)
   {
      lSap->procId      = DU_PROC;
      lSap->ent         = ENTRG;
      lSap->inst        = lsapInst;
      lSap->sapId       = lsapInst;      /* SapId will be stored as suId in MAC */
      lSap->selector    = (inst == RLC_UL_INST) ? DU_SELECTOR_LWLC : DU_SELECTOR_TC;
      kwMngmt.hdr.elmId.elmnt  = STRGUSAP;
      printf("\nRLC MAC Lower Sap Cfg Req sent for inst %d", inst);

   }
   else
   {
      lSap->procId    = DU_PROC;
      lSap->ent       = ENTKW;
      lSap->inst      = (inst == RLC_UL_INST) ?
         RLC_DL_INST : RLC_UL_INST;
      lSap->sapId       = 0;
      lSap->selector = DU_SELECTOR_LC;
      kwMngmt.hdr.elmId.elmnt  = STUDXSAP;
      printf("\nRLC DL/UL Lower Sap Cfg Req sent for inst %d", inst);
   }

   cmPkLkwCfgReq(&pst, &kwMngmt);
   return ROK;
}

/**************************************************************************
 * @brief Function to fill configs required by RLC
 *
 * @details
 *
 *      Function : duBuildRlcUsapCfg 
 * 
 *      Functionality:
 *           Initiates general Configs towards RLC 
 *     
 * @param[in] Inst  Specifies if RLC UL or RLC DL instance 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duBuildRlcUsapCfg(U8 elemId, Ent ent, Inst inst)
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst        pst;

   DU_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);

   uSap->selector   = DU_SELECTOR_LC;
   uSap->mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   uSap->mem.pool = RLC_POOL;
   uSap->mem.spare = 0;

   uSap->procId = DU_PROC;
   uSap->ent = ENTKW;
   uSap->sapId = 0;

   uSap->inst = (inst == RLC_UL_INST) ?
      RLC_DL_INST : RLC_UL_INST;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.elmId.elmnt         = STUDXSAP;
   kwMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;

   kwMngmt.hdr.response.mem.pool   = RLC_POOL;

   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = DU_PROC;
   pst.dstInst = inst;
   pst.srcProcId = DU_PROC;
   pst.region = duCb.init.region;

   printf("\nRLC Kwu Upper Sap Cfg Req sent for inst %d", inst);
   cmPkLkwCfgReq(&pst, &kwMngmt);

   return ROK;
}
/**************************************************************************
 * @brief Function to invoke DU Layer Configs
 *
 * @details
 *
 *      Function : duSendRlcUlCfg 
 * 
 *      Functionality:
 *           Initiates Configs towards layers of DU
 *     
 * @param[in]  void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duSendRlcUlCfg()
{
   U8 cellIdx; 

   duBuildRlcCfg((Inst)RLC_UL_INST);
   for(cellIdx = 0; cellIdx < DEFAULT_CELLS; cellIdx++)
   {
      duBuildRlcLsapCfg(ENTRG, (Inst)RLC_UL_INST, cellIdx);
   }
   duBuildRlcLsapCfg(ENTKW, (Inst)RLC_UL_INST, 0);
   
   return ROK;
}

/**************************************************************************
 * @brief Function to invoke DU Layer Configs
 *
 * @details
 *
 *      Function : duSendRlcDlCfg 
 * 
 *      Functionality:
 *           Initiates Configs towards layers of DU
 *     
 * @param[in]  void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duSendRlcDlCfg()
{
   U8 cellIdx; 

   duBuildRlcCfg((Inst)RLC_DL_INST);
   duBuildRlcUsapCfg(STUDXSAP, ENTKW, (Inst)RLC_DL_INST);
   for(cellIdx = 0; cellIdx < DEFAULT_CELLS; cellIdx++)
   {
      duBuildRlcLsapCfg(ENTRG, (Inst)RLC_DL_INST, cellIdx);
   }
   
   return ROK;
}
/**************************************************************************
 * @brief Function to handle Config Confirm from RLC
 *
 * @details
 *
 *      Function : duHdlRlcCfgComplete 
 * 
 *      Functionality:
 *           Handles Gen Config Confirm from RLC
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  KwMngmt *cfm, Unpacked primitive info received from RLC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duHdlRlcCfgComplete(Pst *pst, KwMngmt *cfm)
{
   S16 ret = ROK;
   if (pst->srcInst == RLC_UL_INST)
   {
      ret = duProcRlcUlCfgComplete(pst, cfm);
   }
   else
   {
      ret = duProcRlcDlCfgComplete(pst, cfm);
   }
   return ret;
}

/**************************************************************************
 * @brief Function to handle Control Config Confirm from RLC
 *
 * @details
 *
 *      Function : duHdlRlcCntrlCfgComplete 
 * 
 *      Functionality:
 *           Handles Control Config Confirm from RLC
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  KwMngmt *cfm, Unpacked primitive info received from RLC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duHdlRlcCntrlCfgComplete(Pst *pst, KwMngmt *cntrl)
{
   S16 ret = ROK;

   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      switch (cntrl->hdr.elmId.elmnt)
      {
         case  STRGUSAP:
            {
               if (pst->srcInst == RLC_DL_INST)
               {
                  printf("\nBIND OF RLC DL TO MAC (RGU) SAP SUCCESSFUL");
                  macCfgInst++;
                  if(macCfgInst < DEFAULT_CELLS)
                  {
                     macCfgInst = 0;
                     duBindUnbindRlcToMacSap((Inst) RLC_DL_INST, ABND);
                  }
                  else
                  {
                     duBindUnbindRlcToMacSap((Inst) RLC_UL_INST, ABND);
                  }
               }
               else
               {
                  printf("\nBIND OF RLC UL TO MAC (RGU) SAP SUCCESSFUL");
                  macCfgInst++;
                  if(macCfgInst < DEFAULT_CELLS)
                  {
                     duBindUnbindRlcToMacSap((Inst) RLC_UL_INST, ABND);
                  }
                  else
                  {
                     duSctpStartReq();
                  }

                  break;
               }
            }

      }
   }
   return ret;
}
/**************************************************************************
 * @brief Function to handle Config Confirm from RLC UL
 *
 * @details
 *
 *      Function : duHdlRlcUlCfgComplete 
 * 
 *      Functionality:
 *           Handles Config Confirm from RLC UL
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  KwMngmt *cfm, Unpacked primitive info received from RLC UL
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duProcRlcUlCfgComplete(Pst *pst, KwMngmt *cfm)
{
   S16 ret;

   printf("\nRLC UL Cfg Status %d", cfm->cfm.status);
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch(cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            {
               rlcUlCfg |= RLC_GEN_CFG;
               break;
            }
         case STRGUSAP:
            {
               numRlcMacSaps++;
               if(numRlcMacSaps == DEFAULT_CELLS)
               {
                  rlcUlCfg |= RLC_MAC_SAP_CFG;
                  numRlcMacSaps = 0;
               }
               break;
            }
         case STUDXSAP:
            {
               rlcUlCfg |= RLC_UDX_SAP_CFG;
               break;

            }
         default:
            break;
      }
      printf("\n RLC UL Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
      if(rlcUlCfg == DU_RLC_UL_CONFIGURED)
      {
          rlcUlCfg = 0;
          numRlcMacSaps = 0;
          //Start configuration of RLC DL
          duSendRlcDlCfg();

      }
   }
   else
   {
      printf("\nConfig confirm NOK from RLC UL");
      ret = RFAILED;
   }
   return ret;
}

/**************************************************************************
 * @brief Function to handle Config Confirm from RLC DL
 *
 * @details
 *
 *      Function : duHdlRlcDlCfgComplete 
 * 
 *      Functionality:
 *           Handles Config Confirm from RLC DL
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  KwMngmt *cfm, Unpacked primitive info received from RLC DL
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duProcRlcDlCfgComplete(Pst *pst, KwMngmt *cfm)
{
   printf("\nRLC DL Cfg Status %d", cfm->cfm.status);
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch(cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            {
               rlcDlCfg |= RLC_GEN_CFG;
               break;
            }
         case STRGUSAP:
            {
               numRlcMacSaps++;
               if(numRlcMacSaps == DEFAULT_CELLS)
               {
                  rlcDlCfg |= RLC_MAC_SAP_CFG;
                  numRlcMacSaps = 0;
               }
               break;
            }
         case STUDXSAP:
            {
               rlcDlCfg |= RLC_UDX_SAP_CFG;
               break;

            }
         default:
            break;

      }
      printf("\n RLC DL Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
      if(rlcDlCfg == DU_RLC_DL_CONFIGURED)
      {
          rlcDlCfg = 0;
          //Start configuration of MAC
          duSendMacCfg();

      }
   }
   else
   {
      printf("\nConfig confirm NOK from RLC DL");
   }
   return ROK;
}

/**************************************************************************
 * @brief Function to send configs to MAC
 *
 * @details
 *
 *      Function : duSendMacCfg 
 * 
 *      Functionality:
 *           Initiates Configs towards MAC layer
 *     
 * @param[in]  void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duSendMacCfg()
{
   duBuildMacGenCfg();
   duBuildMacUsapCfg(RLC_UL_INST);
   duBuildMacUsapCfg(RLC_DL_INST);

   return ROK;
}

/**************************************************************************
 * @brief Function to fill gen config required by MAC
 *
 * @details
 *
 *      Function : duBuildMacGenCfg 
 * 
 *      Functionality:
 *           Initiates general Configs towards MAC
 *     
 * @param[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duBuildMacGenCfg()
{
   RgMngmt       rgMngmt;
   RgGenCfg      *genCfg=NULLP;
   Pst           pst;

   DU_SET_ZERO(&pst, sizeof(Pst));
   DU_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   genCfg   = &(rgMngmt.t.cfg.s.genCfg);
   
   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->mem.region = RG_MEM_REGION;
   genCfg->mem.pool   = RG_POOL;
   genCfg->tmrRes     = 10;
   genCfg->numRguSaps = 2;

   genCfg->lmPst.dstProcId = DU_PROC;
   genCfg->lmPst.srcProcId = DU_PROC;
   genCfg->lmPst.dstEnt    = ENTDUAPP;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTRG;
   genCfg->lmPst.srcInst   = macCfgInst;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = RG_MEM_REGION;
   genCfg->lmPst.pool      = RG_POOL;
   genCfg->lmPst.selector  = DU_SELECTOR_LC;

    /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.msgLen              = 0;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = (Inst)0;
   rgMngmt.hdr.elmId.elmnt         = STGEN;
   rgMngmt.hdr.seqNmb              = 0;
   rgMngmt.hdr.version             = 0;
   rgMngmt.hdr.transId             = 0;

   rgMngmt.hdr.response.prior      = PRIOR0;
   rgMngmt.hdr.response.route      = RTESPEC;
   rgMngmt.hdr.response.mem.region = RG_MEM_REGION;
   rgMngmt.hdr.response.mem.pool   = RG_POOL;
   rgMngmt.hdr.response.selector   = DU_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRG;
   pst.dstInst   = macCfgInst;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region = duCb.init.region;

   printf("\nMAC Gen Cfg Req sent");

   /* Send the request to MAC */
   cmPkLrgCfgReq(&pst, &rgMngmt);

   return ROK;
}

/**************************************************************************
 * @brief Function to fill USAP config required by MAC
 *
 * @details
 *
 *      Function : duBuildMacUsapCfg 
 * 
 *      Functionality:
 *           Initiates USAP Configs towards MAC
 *     
 * @param[in] SpId  Specifies if RLC UL or RLC DL instance 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duBuildMacUsapCfg(SpId sapId)
{
   RgMngmt     rgMngmt;
   RgUpSapCfg  *uSap = NULLP;
   Pst         pst;

   DU_SET_ZERO(&pst, sizeof(Pst));
   DU_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   uSap   = &(rgMngmt.t.cfg.s.rguSap);

   uSap->mem.region = RG_MEM_REGION;
   uSap->mem.pool   = RG_POOL;
   uSap->suId       = 0;
   uSap->spId       = sapId;
   uSap->procId     = DU_PROC;
   uSap->ent        = ENTKW;
   uSap->inst       = sapId;
   uSap->prior      = PRIOR0;
   uSap->route      = RTESPEC;
   uSap->selector   = DU_SELECTOR_LC ;

   /* fill header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = (Inst)0;
   rgMngmt.hdr.elmId.elmnt         = STRGUSAP;
   rgMngmt.hdr.response.mem.region = RG_MEM_REGION;
   rgMngmt.hdr.response.mem.pool   = RG_POOL;
   
   /* fill pst */
   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRG;
   pst.dstInst   = macCfgInst;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   printf("\nMAC Rgu USap Cfg Req sent");

   /* Send the request to MAC */
   cmPkLrgCfgReq(&pst, &rgMngmt);

   return ROK;
}

/**************************************************************************
 * @brief Function to handle Config Confirm from MAC
 *
 * @details
 *
 *      Function : duHdlMacCfgComplete 
 * 
 *      Functionality:
 *           Handles Gen Config Confirm from MAC
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  RgMngmt *cfm, Unpacked primitive info received from MAC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duHdlMacCfgComplete(Pst *pst, RgMngmt *cfm)
{
   S16 ret = ROK;

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            {
               macCfg |= MAC_GEN_CFG;
               break;
            }
         case STRGUSAP:
            {
               macCfg |= MAC_SAP_CFG;
               numRlcMacSaps++;
               break;
            }
         default:
            break;
      }
      printf("\n MAC Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
      if(macCfg == MAC_CONFIGURED && numRlcMacSaps == MAX_MAC_SAP)
      {
         macCfg = 0;
         printf("\n Completed sending Configs");
         macCfgInst = 0;
         duBindUnbindRlcToMacSap(RLC_DL_INST, ABND);
      }

   }
   else
   {
      printf("\nConfig confirm NOK from MAC");
      ret = RFAILED;
   }
   return ret;
}

/**************************************************************************
 * @brief Function to bind/unbind RLC to MAC SAP
 *
 * @details
 *
 *      Function : duBindUnbindRlcToMacSap 
 * 
 *      Functionality:
 *           Initiates Bind/Unbind from RLC to MAC
 *     
 * @param[in] Inst   Specifies if RLC UL or RLC DL instance 
 * @param[in] action Specifies if action is bind or unbind
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duBindUnbindRlcToMacSap(U8 inst, U8 action)
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;

   TRC2(smBindKwToRguSap)

   DU_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   if (action == ABND)
   {
      printf("\nCntrl Req to RLC inst %d to bind MAC sap", inst);
   }
   else
   {
      printf("\nCntrl Req to RLC inst %d to unbind MAC sap", inst);
   }
   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  action;
   cntrl->subAction         =  DU_ZERO_VAL;
   cntrl->s.sapCntrl.suId   =  macCfgInst;
   cntrl->s.sapCntrl.spId   =  inst;

   /* Fill header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = inst;
   kwMngmt.hdr.elmId.elmnt         = 186; /* ambiguous defines in lkw.h and lrg.h so direct hardcoded*/
   kwMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   kwMngmt.hdr.response.mem.pool   = RLC_POOL;

   /* Fill pst */
   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = DU_PROC;
   pst.dstInst   = inst;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   cmPkLkwCntrlReq(&pst, &kwMngmt);

   return ROK;
}
/*******************************************************************
 *
 * @brief  Function to start SCTP
 *
 * @details
 *
 *    Function : duSctpStartReq
 *
 *    Functionality:
 *       Function to start SCTP
 *
 * @params[in] 
 * @return void
 *
 * ****************************************************************/
S16 duSctpStartReq()
{
   Pst pst;
   Buffer *mBuf;

   cmMemset((U8 *)&(pst), 0, sizeof(Pst));
   pst.srcEnt = (Ent)ENTDUAPP;
   pst.srcInst = (Inst)DU_INST;
   pst.srcProcId = DU_PROC;
   pst.dstEnt = (Ent)ENTSCTP;
   pst.dstInst = (Inst)SCTP_INST;
   pst.dstProcId = pst.srcProcId;
   pst.event = EVTSCTPSTRT;
   pst.selector = DU_SELECTOR_LC;
   pst.pool= DU_POOL;
   if(SGetMsg(DFLT_REGION, DU_POOL, &mBuf) != ROK)
   {
      printf("\nMemory allocation failed in duReadCfg");
      return RFAILED;
   }
   SPstTsk(&pst, mBuf);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Handles SCTP notifications
 *
 * @details
 *
 *    Function : duSctpNtfyHdl
 *
 *    Functionality:
 *         Handles SCTP notification
 *
 * @params[in] Message Buffer
 *             SCTP notification
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 duSctpNtfyHdl(Buffer *mBuf, CmInetSctpNotification *ntfy)
{
   char *finalBuf;
   int i,j;

   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE:
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               {
                  printf("\nSCTP communication UP");
                  duCb.sctpStatus = TRUE;
                  //Setup F1-C
                  if(!duCb.f1Status)
                  {
                     /* Build and send F1 Setup response */
	                  Buffer *f1SetupReq;
                     MsgLen  copyCnt;

                     BuildF1SetupReq();
                     /* Reversing the encoded string */
                     if(SGetSBuf(1, 1, (Data **)&finalBuf, (Size)encBufSize) != ROK)
                     {
                        printf("Memory allocation failed");
                        RETVALUE(RFAILED);
                     }
                     for(i = 0, j = encBufSize-1; i<encBufSize; i++, j--)
                     {
                        finalBuf[j] = encBuf[i];
                     }

                     if(SGetMsg(1, 1, &f1SetupReq) == ROK)
                     {
                        if(SCpyFixMsg((Data *)finalBuf, f1SetupReq, 0, encBufSize, &copyCnt) == ROK)
                        {
                           printf("\nSending F1 setup request");
                           SPrntMsg(f1SetupReq, 0,0);
                           if(sctpOutMsgSend(f1SetupReq) != ROK)
                           {
                              printf("\nFailed Sending");
                           }
                        }
                     }
                  }
                  else
                  {
                  }
                  break;
               }
         }
         break;
   }
   RETVALUE(ROK);
}

/**********************************************************************
         End of file
**********************************************************************/
