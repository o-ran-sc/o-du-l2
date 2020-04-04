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

#include "du_mgr.h"
#include "du_sctp.h"
#include "du_f1ap_msg_hdl.h"
#include "lsctp.h"
#include "legtp.h"
#include "lphy_stub.h"

U8 rlcDlCfg = 0;
U8 numRlcDlSaps = 0;
U8 rlcUlCfg = 0;
U8 numRlcMacSaps = 0;
U8 macCfg = 0;
U8 macCfgInst = 0;

extern DuCfgParams duCfgParam;
extern S16 cmPkLkwCfgReq(Pst *pst, KwMngmt *cfg);
extern S16 cmPkLkwCntrlReq(Pst *pst, KwMngmt *cfg);
extern S16 cmPkLrgCfgReq(Pst *pst, RgMngmt *cfg);

packMacCellCfgReq packMacCellCfgOpts[] =
{
   packMacCellCfg, /* packing for loosely coupled */
   MacHdlCellCfgReq, /* packing for tightly coupled */
   packMacCellCfg, /* packing for light weight loosly coupled */
};

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

   DU_LOG("\nDU_APP : RLC Gen Cfg Req sent for inst %d", inst);

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
      DU_LOG("\nDU_APP : RLC MAC Lower Sap Cfg Req sent for inst %d", inst);

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
      DU_LOG("\nDU_APP : RLC DL/UL Lower Sap Cfg Req sent for inst %d", inst);
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

   DU_LOG("\nDU_APP : RLC Kwu Upper Sap Cfg Req sent for inst %d", inst);
   cmPkLkwCfgReq(&pst, &kwMngmt);

   return ROK;
}

/**************************************************************************
 * @brief Function to populate internal DS of DU APP
 *
 * @details
 *
 *      Function : duProcCfgComplete
 * 
 *      Functionality:
 *           Populates internal data structures of DU APP after 
 *           receiving configurations.
 *     
 * @param[in]  void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duProcCfgComplete()
{
   S16 ret = ROK;
   static U16 cellId = 0;
   U16 idx;
   for(idx=0; idx< DEFAULT_CELLS; idx++)
   {
      DuCellCb *cell;
      DU_ALLOC(cell, sizeof(DuCellCb))
      if(cell == NULLP)
      {
         DU_LOG("\nDU_APP : Memory Allocation failed in duProcCfgComplete");
         ret = RFAILED;
      }
      else
      {
         U32 nci;
         U8 idx;
         
         memset(cell, 0, sizeof(DuCellCb));
         cell->cellId = cellId++;
         cell->cellInfo.nrEcgi.plmn.mcc[0] = PLMN_MCC0;
         cell->cellInfo.nrEcgi.plmn.mcc[1] = PLMN_MCC1;
         cell->cellInfo.nrEcgi.plmn.mcc[2] = PLMN_MCC2;
         cell->cellInfo.nrEcgi.plmn.mnc[0] = PLMN_MNC0;
         cell->cellInfo.nrEcgi.plmn.mnc[1] = PLMN_MNC1;
         cell->cellInfo.nrEcgi.plmn.mnc[2] = PLMN_MNC2;
         cell->cellInfo.nrEcgi.cellId = NR_CELL_ID;
         cell->cellInfo.nrPci = NR_PCI; 
         cell->cellInfo.fiveGsTac = DU_TAC;
         for(idx=0; idx<MAX_PLMN; idx++)
         {
            cell->cellInfo.plmn[idx].mcc[0] = PLMN_MCC0;
            cell->cellInfo.plmn[idx].mcc[1] = PLMN_MCC1;
            cell->cellInfo.plmn[idx].mcc[2] = PLMN_MCC2;
            cell->cellInfo.plmn[idx].mnc[0] = PLMN_MNC0;
            cell->cellInfo.plmn[idx].mnc[1] = PLMN_MNC1;
            cell->cellInfo.plmn[idx].mnc[2] = PLMN_MNC2;
			}
         cell->cellInfo.maxUe = duCfgParam.maxUe;
         cell->cellStatus = CELL_OUT_OF_SERVICE;
         nci = (U16)cell->cellInfo.nrEcgi.cellId;
         
         duCb.cfgCellLst[nci-1] = cell;
      }
   }
   if(ret != RFAILED)
   {
      //Start layer configs
      ret = duSendRlcUlCfg();
   }
   return ret;
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
                  DU_LOG("\nDU_APP : BIND OF RLC DL TO MAC (RGU) SAP SUCCESSFUL");
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
                  DU_LOG("\nDU_APP : BIND OF RLC UL TO MAC (RGU) SAP SUCCESSFUL");
                  macCfgInst++;
                  if(macCfgInst < DEFAULT_CELLS)
                  {
                     duBindUnbindRlcToMacSap((Inst) RLC_UL_INST, ABND);
                  }
                  else
                  {
                     duSendSchCfg();
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

   DU_LOG("\nDU_APP : RLC UL Cfg Status %d", cfm->cfm.status);
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
      DU_LOG("\nDU_APP : RLC UL Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
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
      DU_LOG("\nDU_APP : Config confirm NOK from RLC UL");
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
   DU_LOG("\nDU_APP : RLC DL Cfg Status %d", cfm->cfm.status);
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
      DU_LOG("\nDU_APP : RLC DL Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
      if(rlcDlCfg == DU_RLC_DL_CONFIGURED)
      {
          rlcDlCfg = 0;
          //Start configuration of MAC
          duSendMacCfg();

      }
   }
   else
   {
      DU_LOG("\nDU_APP : Config confirm NOK from RLC DL");
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

   DU_LOG("\nDU_APP : MAC Gen Cfg Req sent");

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

   DU_LOG("\nDU_APP : MAC Rgu USap Cfg Req sent");

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
      DU_LOG("\nDU_APP : MAC Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
      if(macCfg == MAC_CONFIGURED && numRlcMacSaps == MAX_MAC_SAP)
      {
         macCfg = 0;
         DU_LOG("\nDU_APP : Completed sending Configs");
         macCfgInst = 0;
         duBindUnbindRlcToMacSap(RLC_DL_INST, ABND);
      }

   }
   else
   {
      DU_LOG("\nDU_APP : Config confirm NOK from MAC");
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
      DU_LOG("\nDU_APP : Cntrl Req to RLC inst %d to bind MAC sap", inst);
   }
   else
   {
      DU_LOG("\nDU_APP : Cntrl Req to RLC inst %d to unbind MAC sap", inst);
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
   if(f1Params.assocId == ntfy->u.assocChange.assocId)
   {
      if(BuildAndSendF1SetupReq() != ROK)
      {
         return RFAILED;
      }
   }
   else if(ricParams.assocId == ntfy->u.assocChange.assocId)
   {
      if(BuildAndSendE2SetupReq() != ROK)
      {
         return RFAILED;
      }
   }
   else
	{
      DU_LOG("\nDU_APP : Invalid assocId %d received", ntfy->u.assocChange.assocId);
		return RFAILED;
	}
   return ROK;
}

/*******************************************************************
 *
 * @brief  Fills Pst struct for ENTEGTP
 *
 * @details
 *
 *    Function : duFillEgtpPst
 *
 *    Functionality:
 *       Fills Pst struct for ENTEGTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duFillEgtpPst(Pst *pst, Event event)
{
   cmMemset((U8 *)pst, 0, sizeof(Pst));
   pst->srcEnt = (Ent)ENTDUAPP;
   pst->srcInst = (Inst)DU_INST;
   pst->srcProcId = DU_PROC;
   pst->dstEnt = (Ent)ENTEGTP;
   pst->dstInst = (Inst)EGTP_INST;
   pst->dstProcId = pst->srcProcId;
   pst->event = event;
   pst->selector = DU_SELECTOR_LC;
   pst->pool= DU_POOL;
  
   RETVALUE(ROK);
}


/*******************************************************************
 *
 * @brief  Function to configure EGTP
 *
 * @details
 *
 *    Function : duBuildEgtpCfgReq
 *
 *    Functionality:
 *       Function to configure EGTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 duBuildEgtpCfgReq()
{
    Pst pst;
    EgtpConfig egtpCfg;
    
    DU_LOG("\nDU_APP : Sending EGTP config request");

    cmMemset((U8 *)&egtpCfg, 0, sizeof(EgtpConfig));
    cmMemcpy((U8 *)&egtpCfg, (U8 *)&duCfgParam.egtpParams, (PTR)sizeof(EgtpConfig));
    
    duFillEgtpPst(&pst, EVTCFGREQ);
    cmPkEgtpCfgReq(&pst, egtpCfg);
 
    RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief  Function to configure EGTP
 *
 * @details
 *
 *    Function : duBuildEgtpCfgReq
 *
 *    Functionality:
 *       Function to configure EGTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duHdlEgtpCfgComplete(CmStatus cfm)
{
   S16 ret = ROK;

   if(cfm.status == LCM_PRIM_OK)
   {
      DU_LOG("\nDU_APP : EGTP configuraton complete");
#ifdef EGTP_TEST
      duSendEgtpSrvOpenReq();
#endif
   }
   else
   {
      DU_LOG("\nDU_APP : EGTP configuraton failed");
      ret = RFAILED;
   }

   RETVALUE(ret);
}

/*******************************************************************
 *
 * @brief  Sends server open request to EGTP
 *
 * @details
 *
 *    Function : duSendEgtpSrvOpenReq
 *
 *    Functionality:
 *       Sends server open request to EGTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
 
S16 duSendEgtpSrvOpenReq()
{
   Pst pst;

   DU_LOG("\nDU_APP : Sending EGTP server open request");

   duFillEgtpPst(&pst, EVTSRVOPENREQ);
   cmPkEgtpSrvOpenReq(&pst);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Handles server open confirmation
 *
 * @details
 *
 *    Function : duHdlEgtpSrvOpenComplete
 *
 *    Functionality:
 *        Handles server open confirmation
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

S16 duHdlEgtpSrvOpenComplete(CmStatus cfm)
{
    S16 ret = ROK;
 
    if(cfm.status == LCM_PRIM_OK)
    {
       DU_LOG("\nDU_APP : EGTP server opened successfully");
#ifdef EGTP_TEST
       duSendEgtpTnlMgmtReq(EGTP_TNL_MGMT_ADD, EGTP_LCL_TEID, EGTP_REM_TEID);
#endif
    }
    else
    {
       DU_LOG("\nDU_APP : EGTP server opening failed");
       ret = RFAILED;
    }
 
    RETVALUE(ret);
}

/*******************************************************************
 *
 * @brief Sends tunnel management request
 *
 * @details
 *
 *    Function : duSendEgtpTnlMgmtReq 
 *
 *    Functionality:
 *        Builds and sends tunnel management request to EGTP
 *
 * @params[in] Action
 *             Local tunnel endpoint id
 *             Remote tunnel endpoint id 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 duSendEgtpTnlMgmtReq(U8 action, U32 lclTeid, U32 remTeid)
{
   Pst pst;
   EgtpTnlEvt tnlEvt;

   tnlEvt.action = action;
   tnlEvt.lclTeid = lclTeid;
   tnlEvt.remTeid = remTeid;
    
   DU_LOG("\nDU_APP : Sending EGTP tunnel management request");
    
   duFillEgtpPst(&pst, EVTTNLMGMTREQ);
   cmPkEgtpTnlMgmtReq(&pst, tnlEvt);
    
   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Handles Tunnel management confirm 
 *
 * @details
 *
 *    Function : duHdlEgtpTnlMgmtCfm
 *
 *    Functionality:
 *      Handles tunnel management confirm received from Egtp
 *
 * @params[in] Tunnel Event  
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duHdlEgtpTnlMgmtCfm(EgtpTnlEvt tnlEvtCfm)
{
   S16 ret = ROK;

   if(tnlEvtCfm.cfmStatus.status == LCM_PRIM_OK)
   {
      DU_LOG("\nDU_APP : Tunnel management confirm OK");

#ifdef EGTP_TEST
      duSendUeCreateReqToRlc();

      duSendEgtpTestData();
      
      /* For testing purpose. TTI thread should actually be in L1 */
      duStartTtiThread();
#endif      
   }
   else
   {
      DU_LOG("\nDU_APP : Tunnel management failed");
      ret = RFAILED;
   }

   RETVALUE(ret);
}

S16 duSendEgtpDatInd(Buffer *mBuf)
{
   EgtpMsg  egtpMsg;

   /* Fill EGTP header */
   egtpMsg.msgHdr.msgType = EGTPU_MSG_GPDU;
   egtpMsg.msgHdr.nPdu.pres = FALSE;
   egtpMsg.msgHdr.seqNum.pres = FALSE;
   egtpMsg.msgHdr.extHdr.udpPort.pres = FALSE;
   egtpMsg.msgHdr.extHdr.pdcpNmb.pres = FALSE;
   egtpMsg.msgHdr.teId = 1;
   egtpMsg.msg = mBuf;
   
   egtpHdlDatInd(egtpMsg);

   return ROK;

}

#ifdef EGTP_TEST
/*******************************************************************
 *
 * @brief Simulate UL Data for intial test
 *
 * @details
 *
 *    Function : duSendEgtpTestData
 *
 *    Functionality:
 *      Simulate UL data for initial test
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duSendEgtpTestData()
{
   char data[30] = "This is EGTP data from DU";
   int datSize = 30;

   Buffer   *mBuf;

   if(SGetMsg(DU_APP_MEM_REGION, DU_POOL, &mBuf) == ROK)
   {
      if(SAddPstMsgMult((Data *)data, datSize, mBuf) != ROK)
      {
         DU_LOG("\nDU_APP : SAddPstMsgMult failed");
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      DU_LOG("\nDU_APP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   /* filling IPv4 header */ 
   CmIpv4Hdr ipv4Hdr;
   MsgLen    mLen;

   mLen = 0;
   SFndLenMsg(mBuf, &mLen);

   cmMemset((U8 *)&ipv4Hdr, 0, sizeof(CmIpv4Hdr));
   ipv4Hdr.length = CM_IPV4_HDRLEN + mLen;
   ipv4Hdr.hdrVer = 0x45;
   ipv4Hdr.proto = 1;
   ipv4Hdr.srcAddr = CM_INET_NTOH_U32(duCfgParam.egtpParams.localIp.ipV4Addr);
   ipv4Hdr.destAddr = CM_INET_NTOH_U32(duCfgParam.egtpParams.destIp.ipV4Addr);

   /* Packing IPv4 header into buffer */
   S16          ret, cnt, idx;
   Data         revPkArray[CM_IPV4_HDRLEN];
   Data         pkArray[CM_IPV4_HDRLEN];
 
   /* initialize locals */
   cnt = 0;
   cmMemset(revPkArray, 0, CM_IPV4_HDRLEN);
   cmMemset(pkArray, 0, CM_IPV4_HDRLEN);

   /* Pack Header Version */
   pkArray[cnt++] = ipv4Hdr.hdrVer;

   /* Pack TOS */
   pkArray[cnt++] = ipv4Hdr.tos;

   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.length);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.length);

   /* Pack Id */
   pkArray[cnt++] = (Data) GetHiByte(ipv4Hdr.id);
   pkArray[cnt++] = (Data) GetLoByte(ipv4Hdr.id);

   /* Pack Offset */
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.off);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.off);

   /* Pack TTL */
   pkArray[cnt++] = ipv4Hdr.ttl;

   /* Pack Protocol */
   pkArray[cnt++] = ipv4Hdr.proto;

   /* Pack Checksum */
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.chkSum);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.chkSum);

   /* Pack Source Address */
   pkArray[cnt++] = (Data)GetHiByte(GetHiWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetHiWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetHiByte(GetLoWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetLoWord(ipv4Hdr.srcAddr));

   /* Pack Destination Address */
   pkArray[cnt++] = (Data)GetHiByte(GetHiWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetHiWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetHiByte(GetLoWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetLoWord(ipv4Hdr.destAddr));

   for (idx = 0;  idx < CM_IPV4_HDRLEN;  idx++)
      revPkArray[idx] = pkArray[CM_IPV4_HDRLEN - idx -1];

   /* this function automatically reverses revPkArray */
   ret = SAddPreMsgMult(revPkArray, (MsgLen)cnt, mBuf);
    
   duSendEgtpDatInd(mBuf);
 
   RETVALUE(ROK);
}
#endif /* EGTP_TEST */


/**************************************************************************
 * @brief Function to send configs to SCH
 *
 * @details
 *
 *      Function : duSendSchCfg 
 * 
 *      Functionality:
 *           Sends general config to Scheduler via MAC layer
 *     
 * @param[in]  void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duSendSchCfg()
{
   RgMngmt       rgMngmt;
   RgSchInstCfg  *cfg = NULLP;
   Pst           pst;

   DU_SET_ZERO(&pst, sizeof(Pst));
   DU_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   cfg = &(rgMngmt.t.cfg.s.schInstCfg);

   /* Filling of Instance Id */
   cfg->instId = DEFAULT_CELLS + 1;
   /* Filling of Gen config */
   cfg->genCfg.mem.region = RG_MEM_REGION;
   cfg->genCfg.mem.pool = RG_POOL;
   cfg->genCfg.tmrRes = 10;

#ifdef LTE_ADV
   cfg->genCfg.forceCntrlSrbBoOnPCel = FALSE;
   cfg->genCfg.isSCellActDeactAlgoEnable = TRUE;
#endif/*LTE_ADV*/
   cfg->genCfg.startCellId     = 1;
   cfg->genCfg.lmPst.dstProcId = DU_PROC;
   cfg->genCfg.lmPst.srcProcId = DU_PROC;
   cfg->genCfg.lmPst.dstEnt    = ENTDUAPP;
   cfg->genCfg.lmPst.dstInst   = DU_INST;
   cfg->genCfg.lmPst.srcEnt    = ENTRG;
   cfg->genCfg.lmPst.srcInst   = DEFAULT_CELLS + 1;
   cfg->genCfg.lmPst.prior     = PRIOR0;
   cfg->genCfg.lmPst.route     = RTESPEC;
   cfg->genCfg.lmPst.region    = RG_MEM_REGION;
   cfg->genCfg.lmPst.pool      = RG_POOL;
   cfg->genCfg.lmPst.selector  = DU_SELECTOR_LC;

   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = DU_INST;
   rgMngmt.hdr.elmId.elmnt         = STSCHINST;
   rgMngmt.hdr.response.mem.region = RG_MEM_REGION;
   rgMngmt.hdr.response.mem.pool   = RG_POOL;

   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.srcInst   = DU_INST;
   pst.dstInst   = 0;
   pst.region    = duCb.init.region;
   pst.event    = (Event) EVTMACSCHGENCFGREQ;

   DU_LOG("\nDU_APP : MAC Sch Cfg sent");

   /* Send the request to MAC */
   cmPkLrgSchCfgReq(&pst, &rgMngmt);

   return ROK;
}


/**************************************************************************
 * @brief Function to configure SCTP params and 
 *  responsible for F1 and E2 interfaces
 *
 * @details
 *
 *      Function : duLayerConfigComplete
 * 
 *      Functionality:
 *           Configures SCTP Params and responsible for handling
 *           F1 and E2 interface.
 *     
 * @param[in]  void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duLayerConfigComplete()
{
   S16 ret = ROK;

   DU_LOG("\nDU_APP : Configuring all Layer is complete");

   if((ret = duSctpCfgReq(duCfgParam.sctpParams)) != ROK)
   {
      DU_LOG("\nDU_APP : Failed configuring Sctp Params");
      ret = RFAILED;
   }
   if((ret = duSctpAssocReq(F1_INTERFACE)) != ROK)
   {
      DU_LOG("\nDU_APP : Failed to send AssocReq F1");
      ret = RFAILED;
   }
   if((ret = duSctpAssocReq(E2_INTERFACE)) != ROK)
   {
      DU_LOG("\nDU_APP : Failed to send AssocReq E2");
      ret = RFAILED;
   }

   RETVALUE(ret); 
} 

/**************************************************************************
 * @brief Function to handle  SCH Config Confirm from MAC
 *
 * @details
 *
 *      Function : duHdlSchCfgComplete 
 * 
 *      Functionality:
 *           Handles Scheduler Gen Config Confirm from MAC
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  RgMngmt *cfm, Unpacked primitive info received from MAC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duHdlSchCfgComplete(Pst *pst, RgMngmt *cfm)
{
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STSCHINST:
            {
               DU_LOG("\nDU_APP : Received SCH CFG CFM at DU APP");
               break;
            }
         default:
            break;
      }
   }
   duLayerConfigComplete();
   duBuildEgtpCfgReq();
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles TTI indication 
 *
 * @details
 *
 *    Function : duSendEgtpTTIInd
 *
 *    Functionality:
 *     Handles TTI indication received from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duSendEgtpTTIInd()
{
   Pst pst;

   duFillEgtpPst(&pst, EVTTTIIND);
   cmPkEgtpTTIInd(&pst);
   
   RETVALUE(ROK);
   
}

/**************************************************************************
 * @brief Function to fill and send MacCellconfig
 *
 * @details
 *
 *      Function : duBuildAndSendMacCellCfg 
 * 
 *      Functionality:
 *           Initiates MAC Configs towards MAC
 *     
 * @param[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duBuildAndSendMacCellCfg()
{
   Pst pst;
   DU_SET_ZERO(&pst, sizeof(Pst));
   MacCellCfg *duMacCellCfg = NULLP;

   DU_ALLOC(duMacCellCfg, sizeof(MacCellCfg));
   if(duMacCellCfg == NULLP)
   {
      return RFAILED;
   }

   /* store the address in the duCb so that we can free on confirm msg */
   duCb.duMacCellCfg = duMacCellCfg;

   /* copy the mac config structure from duCfgParams */
   memcpy(duMacCellCfg,&duCfgParam.macCellCfg,sizeof(MacCellCfg));

   duMacCellCfg->transId = getTransId(); /* transaction ID */
   
   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRG;
   pst.dstInst   = 0;
   pst.srcInst   = 0;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region = duCb.init.region;
   pst.event = EVENT_MAC_CELL_CONFIG_REQ;

   /* Send MAC cell config to MAC */
   return (*packMacCellCfgOpts[pst.selector])(&pst, duMacCellCfg);
}

/**************************************************************************
 * @brief Function to Handle MAC cell config confirm
 *
 * @details
 *
 *      Function : duHandleMacCellCfgCfm 
 * 
 *      Functionality:
 *           Initiates general Configs towards MAC
 *     
 * @param[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 duHandleMacCellCfgCfm(MacCellCfgCfm *macCellCfgCfm)
{
   S16 ret = ROK;

   if(macCellCfgCfm->transId == duCb.duMacCellCfg->transId)
   {
      /* free the memory allocated during sending macCellCfg request */
      DU_FREE(duCb.duMacCellCfg,sizeof(MacCellCfg));
      duCb.duMacCellCfg = NULLP;
      DU_LOG("\nDU-APP : MAC CELL config confirm recieved\n");


      /* Build and send GNB-DU config update */
      ret = BuildAndSendDUConfigUpdate();
   }
   else
   {
      /* transaction ID missmatch */
      DU_LOG("\n transaction ID mismatch in macCellCfg");
      ret = RFAILED;
   }

   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
