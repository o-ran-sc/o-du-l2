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

/* This file contains message handling functionality for DU APP */
#include "common_def.h"
#include "lrg.h"
#include "legtp.h"
#include "lkw.h"
#include "kwu.h"
#include "lrg.x"
#include "lkw.x"
#include "kwu.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_cfg.h"
#include "du_app_rlc_inf.h"
#include "du_mgr.h"
#include "du_sctp.h"
#include "F1AP-PDU.h"
#include "du_f1ap_msg_hdl.h"
#include "du_ue_mgr.h"
#include "lsctp.h"
#include "legtp.h"
#include "lphy_stub.h"
#include "du_utils.h"

uint8_t rlcDlCfg = 0;
uint8_t numRlcDlSaps = 0;
uint8_t rlcUlCfg = 0;
uint8_t numRlcMacSaps = 0;
uint8_t macCfg = 0;
uint8_t macCfgInst = 0;

extern DuCfgParams duCfgParam;
extern uint8_t packRlcConfigReq(Pst *pst, RlcMngmt *cfg);
extern uint8_t cmPkLkwCntrlReq(Pst *pst, RlcMngmt *cfg);
extern uint8_t cmPkLrgCfgReq(Pst *pst, RgMngmt *cfg);
extern uint8_t BuildAndSendE2SetupReq();
extern uint8_t egtpHdlDatInd(EgtpMsg egtpMsg);
extern uint8_t BuildAndSendDUConfigUpdate();
extern uint16_t getTransId();
extern uint8_t cmPkLrgSchCfgReq(Pst * pst,RgMngmt * cfg);
uint8_t BuildAndSendRrcDeliveryReport(uint32_t gnbCuUeF1apId, uint32_t gnbDuUeF1apId, RrcDeliveryReport *rrcDelivery);

packMacCellCfgReq packMacCellCfgOpts[] =
{
   packMacCellCfg, /* packing for loosely coupled */
   MacProcCellCfgReq, /* packing for tightly coupled */
   packMacCellCfg, /* packing for light weight loosly coupled */
};

DuMacCellStartReq packMacCellStartReqOpts[] =
{
   packMacCellStartReq,   /* Loose coupling */
   MacProcCellStartReq,    /* TIght coupling */
   packMacCellStartReq    /* Light weight-loose coupling */
};

DuMacCellStopReq packMacCellStopReqOpts[] =
{
   packMacCellStopReq,   /* Loose coupling */
   MacProcCellStopReq,    /* TIght coupling */
   packMacCellStopReq    /* Light weight-loose coupling */
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
uint8_t duBuildRlcCfg(Inst inst)
{
   RlcMngmt   rlcMngmt;
   RlcGenCfg  *genCfg = NULLP;
   Pst pst;

   DU_SET_ZERO(&rlcMngmt, sizeof(RlcMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(rlcMngmt.t.cfg.s.gen);

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
   genCfg->lmPst.srcEnt    = ENTRLC;
   genCfg->lmPst.srcInst   = inst;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   genCfg->lmPst.pool      = RLC_POOL;
   genCfg->lmPst.selector  = ODU_SELECTOR_LC;

   /* Fill Header */
   rlcMngmt.hdr.msgType             = TCFG;
   rlcMngmt.hdr.msgLen              = 0;
   rlcMngmt.hdr.entId.ent           = ENTRLC;
   rlcMngmt.hdr.entId.inst          = (Inst)0;
   rlcMngmt.hdr.elmId.elmnt         = STGEN;
   rlcMngmt.hdr.seqNmb              = 0;
   rlcMngmt.hdr.version             = 0;
   rlcMngmt.hdr.transId             = 0;
   rlcMngmt.hdr.response.prior      = PRIOR0;
   rlcMngmt.hdr.response.route      = RTESPEC;
   rlcMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   rlcMngmt.hdr.response.mem.pool   = DU_POOL;
   rlcMngmt.hdr.response.selector   = ODU_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = ODU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRLC;
   pst.dstInst   = inst;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   DU_LOG("\nDU_APP : RLC Gen Cfg Req sent for inst %d", inst);

   /* Send the request to RLC */
   packRlcConfigReq(&pst, &rlcMngmt);

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
uint8_t duBuildRlcLsapCfg(Ent ent, Inst inst, uint8_t lsapInst)
{

   RlcMngmt   rlcMngmt;
   RlcSapCfg  *lSap = NULLP;
   Pst        pst;

   DU_SET_ZERO(&rlcMngmt, sizeof(RlcMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   rlcMngmt.hdr.msgType             = TCFG;
   rlcMngmt.hdr.entId.ent           = ENTRLC;
   rlcMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;

   rlcMngmt.hdr.response.mem.pool   = RLC_POOL;

   /* Fill Pst */
   pst.selector  = ODU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRLC;
   pst.dstProcId = DU_PROC;
   pst.dstInst   = inst;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;
   lSap   = &(rlcMngmt.t.cfg.s.sap);

   lSap->mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   lSap->mem.pool    = RLC_POOL;
   lSap->mem.spare   = 0;
   lSap->bndTmrIntvl = 10;
   lSap->priority    = PRIOR0;
   lSap->route       = RTESPEC;
   if (ent == ENTMAC)
   {
      lSap->procId      = DU_PROC;
      lSap->ent         = ENTMAC;
      lSap->inst        = lsapInst;
      lSap->sapId       = lsapInst;      /* SapId will be stored as suId in MAC */
      lSap->selector    = (inst == RLC_UL_INST) ? ODU_SELECTOR_LWLC : ODU_SELECTOR_TC;
      rlcMngmt.hdr.elmId.elmnt  = STRGUSAP;
      DU_LOG("\nDU_APP : RLC MAC Lower Sap Cfg Req sent for inst %d", inst);

   }
   else
   {
      lSap->procId    = DU_PROC;
      lSap->ent       = ENTRLC;
      lSap->inst      = (inst == RLC_UL_INST) ?
	 RLC_DL_INST : RLC_UL_INST;
      lSap->sapId       = 0;
      lSap->selector = ODU_SELECTOR_LC;
      rlcMngmt.hdr.elmId.elmnt  = STUDXSAP;
      DU_LOG("\nDU_APP : RLC DL/UL Lower Sap Cfg Req sent for inst %d", inst);
   }

   packRlcConfigReq(&pst, &rlcMngmt);
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
uint8_t duBuildRlcUsapCfg(uint8_t elemId, Ent ent, Inst inst)
{
   RlcMngmt   rlcMngmt;
   RlcSapCfg  *uSap = NULLP;
   Pst        pst;

   DU_SET_ZERO(&rlcMngmt, sizeof(RlcMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(rlcMngmt.t.cfg.s.sap);

   uSap->selector   = ODU_SELECTOR_LC;
   uSap->mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   uSap->mem.pool = RLC_POOL;
   uSap->mem.spare = 0;

   uSap->procId = DU_PROC;
   uSap->ent = ENTRLC;
   uSap->sapId = 0;

   uSap->inst = (inst == RLC_UL_INST) ?
      RLC_DL_INST : RLC_UL_INST;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   rlcMngmt.hdr.msgType             = TCFG;
   rlcMngmt.hdr.entId.ent           = ENTRLC;
   rlcMngmt.hdr.elmId.elmnt         = STUDXSAP;
   rlcMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;

   rlcMngmt.hdr.response.mem.pool   = RLC_POOL;

   /* Fill Pst */
   pst.selector  = ODU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRLC;
   pst.dstProcId = DU_PROC;
   pst.dstInst = inst;
   pst.srcProcId = DU_PROC;
   pst.region = duCb.init.region;

   DU_LOG("\nDU_APP : RLC Kwu Upper Sap Cfg Req sent for inst %d", inst);
   packRlcConfigReq(&pst, &rlcMngmt);

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
uint8_t duProcCfgComplete()
{
   uint8_t         ret = ROK;
   static uint16_t cellId = 0;
   uint16_t        idx;
   for(idx=0; idx< DEFAULT_CELLS; idx++)
   {
      DuCellCb *cell = NULLP;
      DU_ALLOC(cell, sizeof(DuCellCb))
	 if(cell == NULLP)
	 {
	    DU_LOG("\nDU_APP : Memory Allocation failed in duProcCfgComplete");
	    ret = RFAILED;
	 }
	 else
	 {
	    uint32_t nci;
	    uint8_t idx1; 
	    memset(cell, 0, sizeof(DuCellCb));
	    cell->cellId = ++cellId;
	    cell->cellInfo.nrEcgi.plmn.mcc[0] = PLMN_MCC0;
	    cell->cellInfo.nrEcgi.plmn.mcc[1] = PLMN_MCC1;
	    cell->cellInfo.nrEcgi.plmn.mcc[2] = PLMN_MCC2;
	    cell->cellInfo.nrEcgi.plmn.mnc[0] = PLMN_MNC0;
	    cell->cellInfo.nrEcgi.plmn.mnc[1] = PLMN_MNC1;
	    cell->cellInfo.nrEcgi.plmn.mnc[2] = PLMN_MNC2;
	    cell->cellInfo.nrEcgi.cellId = NR_CELL_ID;
	    cell->cellInfo.nrPci = NR_PCI; 
	    cell->cellInfo.fiveGsTac = DU_TAC;
	    for(idx1=0; idx1<MAX_PLMN; idx1++)
	    {
	       cell->cellInfo.plmn[idx1].mcc[0] = PLMN_MCC0;
	       cell->cellInfo.plmn[idx1].mcc[1] = PLMN_MCC1;
	       cell->cellInfo.plmn[idx1].mcc[2] = PLMN_MCC2;
	       cell->cellInfo.plmn[idx1].mnc[0] = PLMN_MNC0;
	       cell->cellInfo.plmn[idx1].mnc[1] = PLMN_MNC1;
	       cell->cellInfo.plmn[idx1].mnc[2] = PLMN_MNC2;
	    }
	    cell->cellInfo.maxUe = duCfgParam.maxUe;
	    cell->cellStatus = CELL_OUT_OF_SERVICE;
	    nci = (uint16_t)cell->cellInfo.nrEcgi.cellId;

	    duCb.cfgCellLst[nci-1] = cell;
	    duCb.numCfgCells++;
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
uint8_t duSendRlcUlCfg()
{
   uint8_t cellIdx; 

   duBuildRlcCfg((Inst)RLC_UL_INST);
   for(cellIdx = 0; cellIdx < DEFAULT_CELLS; cellIdx++)
   {
      duBuildRlcLsapCfg(ENTMAC, (Inst)RLC_UL_INST, cellIdx);
   }
   duBuildRlcLsapCfg(ENTRLC, (Inst)RLC_UL_INST, 0);

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
uint8_t duSendRlcDlCfg()
{
   uint8_t cellIdx; 

   duBuildRlcCfg((Inst)RLC_DL_INST);
   duBuildRlcUsapCfg(STUDXSAP, ENTRLC, (Inst)RLC_DL_INST);
   for(cellIdx = 0; cellIdx < DEFAULT_CELLS; cellIdx++)
   {
      duBuildRlcLsapCfg(ENTMAC, (Inst)RLC_DL_INST, cellIdx);
   }

   return ROK;
}
/**************************************************************************
 * @brief Function to handle Config Confirm from RLC
 *
 * @details
 *
 *      Function : DuHdlRlcCfgComplete 
 * 
 *      Functionality:
 *           Handles Gen Config Confirm from RLC
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  RlcMngmt *cfm, Unpacked primitive info received from RLC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t DuHdlRlcCfgComplete(Pst *pst, RlcMngmt *cfm)
{
   uint8_t ret = ROK;
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
 * @param[in]  RlcMngmt *cfm, Unpacked primitive info received from RLC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t duHdlRlcCntrlCfgComplete(Pst *pst, RlcMngmt *cntrl)
{
   uint8_t ret = ROK;

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
 * @param[in]  RlcMngmt *cfm, Unpacked primitive info received from RLC UL
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t duProcRlcUlCfgComplete(Pst *pst, RlcMngmt *cfm)
{
   uint8_t ret;

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
 * @param[in]  RlcMngmt *cfm, Unpacked primitive info received from RLC DL
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t duProcRlcDlCfgComplete(Pst *pst, RlcMngmt *cfm)
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
uint8_t duSendMacCfg()
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
uint8_t duBuildMacGenCfg()
{
   RgMngmt       rgMngmt;
   RgGenCfg      *genCfg=NULLP;
   Pst           pst;

   DU_SET_ZERO(&pst, sizeof(Pst));
   DU_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   genCfg   = &(rgMngmt.t.cfg.s.genCfg);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->mem.region = MAC_MEM_REGION;
   genCfg->mem.pool   = MAC_POOL;
   genCfg->tmrRes     = 10;
   genCfg->numRguSaps = 2;

   genCfg->lmPst.dstProcId = DU_PROC;
   genCfg->lmPst.srcProcId = DU_PROC;
   genCfg->lmPst.dstEnt    = ENTDUAPP;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTMAC;
   genCfg->lmPst.srcInst   = macCfgInst;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = MAC_MEM_REGION;
   genCfg->lmPst.pool      = MAC_POOL;
   genCfg->lmPst.selector  = ODU_SELECTOR_LC;

   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.msgLen              = 0;
   rgMngmt.hdr.entId.ent           = ENTMAC;
   rgMngmt.hdr.entId.inst          = (Inst)0;
   rgMngmt.hdr.elmId.elmnt         = STGEN;
   rgMngmt.hdr.seqNmb              = 0;
   rgMngmt.hdr.version             = 0;
   rgMngmt.hdr.transId             = 0;

   rgMngmt.hdr.response.prior      = PRIOR0;
   rgMngmt.hdr.response.route      = RTESPEC;
   rgMngmt.hdr.response.mem.region = MAC_MEM_REGION;
   rgMngmt.hdr.response.mem.pool   = MAC_POOL;
   rgMngmt.hdr.response.selector   = ODU_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = ODU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTMAC;
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
uint8_t duBuildMacUsapCfg(SpId sapId)
{
   RgMngmt     rgMngmt;
   RgUpSapCfg  *uSap = NULLP;
   Pst         pst;

   DU_SET_ZERO(&pst, sizeof(Pst));
   DU_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   uSap   = &(rgMngmt.t.cfg.s.rguSap);

   uSap->mem.region = MAC_MEM_REGION;
   uSap->mem.pool   = MAC_POOL;
   uSap->suId       = 0;
   uSap->spId       = sapId;
   uSap->procId     = DU_PROC;
   uSap->ent        = ENTRLC;
   uSap->inst       = sapId;
   uSap->prior      = PRIOR0;
   uSap->route      = RTESPEC;
   uSap->selector   = ODU_SELECTOR_LC ;

   /* fill header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTMAC;
   rgMngmt.hdr.entId.inst          = (Inst)0;
   rgMngmt.hdr.elmId.elmnt         = STRGUSAP;
   rgMngmt.hdr.response.mem.region = MAC_MEM_REGION;
   rgMngmt.hdr.response.mem.pool   = MAC_POOL;

   /* fill pst */
   pst.selector  = ODU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTMAC;
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
uint8_t duHdlMacCfgComplete(Pst *pst, RgMngmt *cfm)
{
   uint8_t ret = ROK;

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
uint8_t duBindUnbindRlcToMacSap(uint8_t inst, uint8_t action)
{
   RlcCntrl *cntrl = NULLP;
   RlcMngmt rlcMngmt;
   Pst      pst;

   TRC2(smBindKwToRguSap)

      DU_SET_ZERO(&rlcMngmt, sizeof(RlcMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   if (action == ABND)
   {
      DU_LOG("\nDU_APP : Cntrl Req to RLC inst %d to bind MAC sap", inst);
   }
   else
   {
      DU_LOG("\nDU_APP : Cntrl Req to RLC inst %d to unbind MAC sap", inst);
   }
   cntrl = &(rlcMngmt.t.cntrl);

   cntrl->action            =  action;
   cntrl->subAction         =  DU_ZERO_VAL;
   cntrl->s.sapCntrl.suId   =  macCfgInst;
   cntrl->s.sapCntrl.spId   =  inst;

   /* Fill header */
   rlcMngmt.hdr.msgType             = TCNTRL;
   rlcMngmt.hdr.entId.ent           = ENTRLC;
   rlcMngmt.hdr.entId.inst          = inst;
   rlcMngmt.hdr.elmId.elmnt         = 186; /* ambiguous defines in lkw.h and lrg.h so direct hardcoded*/
   rlcMngmt.hdr.response.mem.region = (inst == RLC_UL_INST) ?
      RLC_UL_MEM_REGION:RLC_DL_MEM_REGION;
   rlcMngmt.hdr.response.mem.pool   = RLC_POOL;

   /* Fill pst */
   pst.selector  = ODU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRLC;
   pst.dstProcId = DU_PROC;
   pst.dstInst   = inst;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   cmPkLkwCntrlReq(&pst, &rlcMngmt);

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

uint8_t duSctpNtfyHdl(Buffer *mBuf, CmInetSctpNotification *ntfy)
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
uint8_t duFillEgtpPst(Pst *pst, Event event)
{
   memset((uint8_t *)pst, 0, sizeof(Pst));
   pst->srcEnt = (Ent)ENTDUAPP;
   pst->srcInst = (Inst)DU_INST;
   pst->srcProcId = DU_PROC;
   pst->dstEnt = (Ent)ENTEGTP;
   pst->dstInst = (Inst)EGTP_INST;
   pst->dstProcId = pst->srcProcId;
   pst->event = event;
   pst->selector = ODU_SELECTOR_LC;
   pst->pool= DU_POOL;

   return ROK;
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

uint8_t duBuildEgtpCfgReq()
{
   Pst pst;
   EgtpConfig egtpCfg;

   DU_LOG("\nDU_APP : Sending EGTP config request");

   memset((uint8_t *)&egtpCfg, 0, sizeof(EgtpConfig));
   memcpy((uint8_t *)&egtpCfg, (uint8_t *)&duCfgParam.egtpParams, (PTR)sizeof(EgtpConfig));

   duFillEgtpPst(&pst, EVTCFGREQ);
   packEgtpCfgReq(&pst, egtpCfg);

   return ROK;
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
uint8_t duHdlEgtpCfgComplete(CmStatus cfm)
{
   uint8_t ret = ROK;

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

   return (ret);
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

uint8_t duSendEgtpSrvOpenReq()
{
   Pst pst;

   DU_LOG("\nDU_APP : Sending EGTP server open request");

   duFillEgtpPst(&pst, EVTSRVOPENREQ);
   packEgtpSrvOpenReq(&pst);

   return ROK;
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

uint8_t duHdlEgtpSrvOpenComplete(CmStatus cfm)
{
   uint8_t ret = ROK;

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

   return (ret);
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

uint8_t duSendEgtpTnlMgmtReq(uint8_t action, uint32_t lclTeid, uint32_t remTeid)
{
   Pst pst;
   EgtpTnlEvt tnlEvt;

   tnlEvt.action = action;
   tnlEvt.lclTeid = lclTeid;
   tnlEvt.remTeid = remTeid;

   DU_LOG("\nDU_APP : Sending EGTP tunnel management request");

   duFillEgtpPst(&pst, EVTTNLMGMTREQ);
   packEgtpTnlMgmtReq(&pst, tnlEvt);

   return ROK;
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
uint8_t duHdlEgtpTnlMgmtCfm(EgtpTnlEvt tnlEvtCfm)
{
   uint8_t ret = ROK;

   if(tnlEvtCfm.cfmStatus.status == LCM_PRIM_OK)
   {
      DU_LOG("\nDU_APP : Tunnel management confirm OK");
   }
   else
   {
      DU_LOG("\nDU_APP : Tunnel management failed");
      ret = RFAILED;
   }

   return (ret);
}

uint8_t duSendEgtpDatInd(Buffer *mBuf)
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
uint8_t duSendEgtpTestData()
{
   char data[30] = "This is EGTP data from DU";
   int datSize = 30;

   Buffer   *mBuf;

   if(ODU_GET_MSG_BUF(DU_APP_MEM_REGION, DU_POOL, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)data, datSize, mBuf) != ROK)
      {
	 DU_LOG("\nDU_APP : ODU_ADD_POST_MSG_MULT failed");
	 ODU_PUT_MSG_BUF(mBuf);
	 return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nDU_APP : Failed to allocate memory");
      return RFAILED;
   }

   /* filling IPv4 header */ 
   CmIpv4Hdr ipv4Hdr;
   MsgLen    mLen;

   mLen = 0;
   ODU_GET_MSG_LEN(mBuf, &mLen);

   memset((uint8_t *)&ipv4Hdr, 0, sizeof(CmIpv4Hdr));
   ipv4Hdr.length = CM_IPV4_HDRLEN + mLen;
   ipv4Hdr.hdrVer = 0x45;
   ipv4Hdr.proto = 1;
   ipv4Hdr.srcAddr = CM_INET_NTOH_UINT32(duCfgParam.egtpParams.localIp.ipV4Addr);
   ipv4Hdr.destAddr = CM_INET_NTOH_UINT32(duCfgParam.egtpParams.destIp.ipV4Addr);

   /* Packing IPv4 header into buffer */
   uint8_t          ret, cnt, idx;
   Data         revPkArray[CM_IPV4_HDRLEN];
   Data         pkArray[CM_IPV4_HDRLEN];

   /* initialize locals */
   cnt = 0;
   memset(revPkArray, 0, CM_IPV4_HDRLEN);
   memset(pkArray, 0, CM_IPV4_HDRLEN);

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
   ret = ODU_ADD_PRE_MSG_MULT(revPkArray, (MsgLen)cnt, mBuf);

   duSendEgtpDatInd(mBuf);

   return ROK;
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
uint8_t duSendSchCfg()
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
   cfg->genCfg.mem.region = MAC_MEM_REGION;
   cfg->genCfg.mem.pool = MAC_POOL;
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
   cfg->genCfg.lmPst.srcEnt    = ENTMAC;
   cfg->genCfg.lmPst.srcInst   = DEFAULT_CELLS + 1;
   cfg->genCfg.lmPst.prior     = PRIOR0;
   cfg->genCfg.lmPst.route     = RTESPEC;
   cfg->genCfg.lmPst.region    = MAC_MEM_REGION;
   cfg->genCfg.lmPst.pool      = MAC_POOL;
   cfg->genCfg.lmPst.selector  = ODU_SELECTOR_LC;

   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTMAC;
   rgMngmt.hdr.entId.inst          = DU_INST;
   rgMngmt.hdr.elmId.elmnt         = STSCHINST;
   rgMngmt.hdr.response.mem.region = MAC_MEM_REGION;
   rgMngmt.hdr.response.mem.pool   = MAC_POOL;

   /* Fill Pst */
   pst.selector  = ODU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTMAC;
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
uint8_t duLayerConfigComplete()
{
   uint8_t ret = ROK;

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

   return (ret); 
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
uint8_t duHdlSchCfgComplete(Pst *pst, RgMngmt *cfm)
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
 * @brief Sends Slot indication to EGTP
 *
 * @details
 *
 *    Function : duSendEgtpSlotInd
 *
 *    Functionality:
 *     Sends Slot indication to EGTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duSendEgtpSlotInd()
{
   Pst pst;

   duFillEgtpPst(&pst, EVTSLOTIND);
   packEgtpSlotInd(&pst);

   return ROK;

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
uint8_t duBuildAndSendMacCellCfg()
{
   Pst pst;
   DU_SET_ZERO(&pst, sizeof(Pst));
   MacCellCfg *duMacCellCfg = NULLP;

   DU_ALLOC_SHRABL_BUF(duMacCellCfg, sizeof(MacCellCfg));
   if(duMacCellCfg == NULLP)
   {
      return RFAILED;
   }

   /* store the address in the duCb so that we can free on confirm msg */
   duCb.duMacCellCfg = duMacCellCfg;

   /* copy the mac config structure from duCfgParams */
   memcpy(duMacCellCfg,&duCfgParam.macCellCfg,sizeof(MacCellCfg));

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_CELL_CONFIG_REQ);

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
uint8_t  duHandleMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm)
{
   uint8_t actvCellIdx  = 0;
   uint8_t ret          = ROK;

   if(macCellCfgCfm->rsp == ROK)  
   {
	for(actvCellIdx = 0 ; actvCellIdx <duCb.numActvCells ; actvCellIdx++)
	{
	    if(macCellCfgCfm->cellId == duCb.actvCellLst[actvCellIdx]->cellId)
	    {
	        duCb.duMacCellCfg = NULLP;
	        /* Build and send GNB-DU config update */
	        ret = BuildAndSendDUConfigUpdate();

                /* TODO: Trigger cell start req once cell up slot ind is received*/
	        /* Build and Send Cell Start Req to MAC */
	        ret = duBuildAndSendMacCellStartReq();

	    }  
	}
    }
    else
    {
	DU_LOG("\nMac cell cfg failed");
	ret = RFAILED;
    }
    return ret;
}

/*******************************************************************
 *
 * @brief Handles slot indication from MAC
 *
 * @details
 *
 *    Function : duHandleSlotInd
 *
 *    Functionality:
 *      Handles slot indication from MAC
 *
 * @params[in] Post structure pointer
 *             Slot Info pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duHandleSlotInd(Pst *pst, SlotIndInfo *slotInfo)
{

   DU_LOG("\nDU APP : Slot Indication received");

   if(slotInfo->cellId <=0 || slotInfo->cellId > MAX_NUM_CELL)
   {
      DU_LOG("\nDU APP : Invalid Cell Id %d", slotInfo->cellId);
   }
   if(!duCb.actvCellLst[slotInfo->cellId-1]->firstSlotIndRcvd)
   {
      duCb.actvCellLst[slotInfo->cellId-1]->firstSlotIndRcvd = true;
      if((duCb.actvCellLst[slotInfo->cellId-1] != NULL) && \
	    (duCb.actvCellLst[slotInfo->cellId-1]->cellStatus == \
	     ACTIVATION_IN_PROGRESS))
      {
	 DU_LOG("\nDU APP : 5G-NR Cell %d is UP", slotInfo->cellId);
	 duCb.actvCellLst[slotInfo->cellId-1]->cellStatus = ACTIVATED;
      }

   }

   /* TODO : Slot Indication to be moved out of EGTP_TEST when
    * data path is established */
#ifdef EGTP_TEST
   duSendEgtpSlotInd();    
#endif

   if((pst->selector == ODU_SELECTOR_LWLC) || (pst->selector == ODU_SELECTOR_TC)) 
      DU_FREE_SHRABL_BUF(MAC_MEM_REGION, pst->pool, slotInfo, sizeof(SlotIndInfo));

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends cell start request to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendMacCellStartReq
 *
 *    Functionality:
 *       Builds and sends cell start request to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duBuildAndSendMacCellStartReq()
{
   Pst pst;
   MacCellStartInfo *cellStartInfo = NULL;

   DU_LOG("\nDU APP : Building and Sending cell start request to MAC");

   /* Send Cell Start Request to MAC */
   DU_ALLOC_SHRABL_BUF(cellStartInfo, sizeof(MacCellStartInfo));
   if(!cellStartInfo)
   {
      DU_LOG("\nDU APP : Memory alloc failed while building cell start request");
      return RFAILED;
   }

   for(uint8_t id = 0; id < MAX_NUM_CELL; id++) 
   {
      if(duCb.actvCellLst[id])
      {
	 duCb.actvCellLst[id]->firstSlotIndRcvd = FALSE;
	 cellStartInfo->cellId = duCb.actvCellLst[id]->cellInfo.nrEcgi.cellId;

	 /* Fill Pst */
	 FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_CELL_START_REQ);

	 return (*packMacCellStartReqOpts[pst.selector])(&pst, cellStartInfo);
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends cell stop request to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendMacCellStopReq 
 *
 *    Functionality:
 *       Builds and sends cell stop request to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duBuildAndSendMacCellStopReq()
{
   Pst pst;
   MacCellStopInfo *cellStopInfo = NULL;

   DU_LOG("\nDU APP : Building and Sending cell stop request to MAC");

   /* Send Cell Stop Request to MAC */
   DU_ALLOC_SHRABL_BUF(cellStopInfo, sizeof(MacCellStopInfo));
   if(!cellStopInfo)
   {
      DU_LOG("\nDU APP : Memory alloc failed while building cell stop request");
      return RFAILED;
   }
   cellStopInfo->cellId = duCb.actvCellLst[0]->cellId;

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_CELL_STOP_REQ);

   return (*packMacCellStopReqOpts[pst.selector])(&pst, cellStopInfo);
}

/*******************************************************************
 *
 * @brief Handles stop indication from MAC
 *
 * @details
 *
 *    Function : duHandleStopInd
 *
 *    Functionality:
 *      Handles stop indication from MAC
 *
 * @params[in] Post structure pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duHandleStopInd(Pst *pst, MacCellStopInfo *cellStopId)
{
   if(cellStopId->cellId <=0 || cellStopId->cellId > MAX_NUM_CELL)
   {
      DU_LOG("\nDU APP : Invalid Cell Id %d", cellStopId->cellId);
   }
   if(duCb.actvCellLst[cellStopId->cellId-1] != NULL)
   {
      if(duCb.actvCellLst[cellStopId->cellId-1]->firstSlotIndRcvd)
      {
	 duCb.actvCellLst[cellStopId->cellId-1]->firstSlotIndRcvd = false;
	 if((duCb.actvCellLst[cellStopId->cellId-1]->cellStatus == \
		  ACTIVATED))
	 {
	    DU_LOG("\nDU APP : 5G-NR Cell %d is DOWN", cellStopId->cellId);
	    duCb.actvCellLst[cellStopId->cellId-1]->cellStatus = DELETION_IN_PROGRESS;
	 }
      }
   }
   if((pst->selector == ODU_SELECTOR_LWLC) || (pst->selector == ODU_SELECTOR_TC))
      DU_FREE_SHRABL_BUF(MAC_MEM_REGION, pst->pool, cellStopId, sizeof(MacCellStopInfo));

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles slot indication from MAC
 *
 * @details
 *
 *    Function : duHandleUlCcchInd
 *
 *    Functionality:
 *      Handles UL CCCH indication from MAC
 *
 * @params[in] Post structure pointer
 *             UL CCCH Ind pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duHandleUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo)
{

   DU_LOG("\nDU APP : UL CCCH Indication received");

   return (duProcUlCcchInd(ulCcchIndInfo));
}

/*******************************************************************
 *
 * @brief Process UL RRC Message from RLC
 *
 * @details
 *
 *    Function : DuProcRlcUlRrcMsgTrans
 *
 *    Functionality: Process UL RRC Message from RLC
 *
 * @params[in] Post structure
 *             UL RRC Message Info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuProcRlcUlRrcMsgTrans(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo)
{
   DuUeCb   ueCb;
   
   ueCb = duCb.actvCellLst[ulRrcMsgInfo->cellId -1]->ueCb[ulRrcMsgInfo->ueIdx -1];
   BuildAndSendULRRCMessageTransfer(ueCb, ulRrcMsgInfo->lcId, ulRrcMsgInfo->msgLen, \
      ulRrcMsgInfo->rrcMsg);

   DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulRrcMsgInfo->rrcMsg, ulRrcMsgInfo->msgLen);
   DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulRrcMsgInfo, sizeof(RlcUlRrcMsgInfo));
   return ROK;
}

/*******************************************************************
*
* @brief Process RRC delivery report from RLC
*
* @details
*
*    Function : DuProcRlcRrcDeliveryReport
*
*    Functionality: Process RRC delivery Message from RLC
*
* @params[in] Post structure
*             UL RRC Message Info
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t DuProcRlcRrcDeliveryReport(Pst *pst, RrcDeliveryReport *rrcDeliveryReport)
{
       DuUeCb   ueCb;
       uint8_t  ret = RFAILED;

       ueCb = duCb.actvCellLst[rrcDeliveryReport->cellId -1]->ueCb[rrcDeliveryReport->ueIdx -1];
       ret = BuildAndSendRrcDeliveryReport(ueCb.gnbCuUeF1apId, ueCb.gnbDuUeF1apId,rrcDeliveryReport);

       DU_FREE_SHRABL_BUF(pst->region, pst->pool, rrcDeliveryReport, sizeof(RrcDeliveryReport));
       return ret;
}


/**********************************************************************
  End of file
 **********************************************************************/
