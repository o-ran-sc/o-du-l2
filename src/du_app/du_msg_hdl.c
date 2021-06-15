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
#include "du_utils.h"
#include "du_cell_mgr.h" 

#ifdef O1_ENABLE

#include "AlarmInterface.h"
#include "ConfigInterface.h"

#endif 

uint8_t rlcDlCfg = 0;
uint8_t numRlcDlSaps = 0;
uint8_t rlcUlCfg = 0;
uint8_t numRlcMacSaps = 0;
uint8_t macCfg = 0;
uint8_t macCfgInst = 0;

DuCfgParams duCfgParam;
uint8_t packRlcConfigReq(Pst *pst, RlcMngmt *cfg);
uint8_t cmPkLkwCntrlReq(Pst *pst, RlcMngmt *cfg);
uint8_t cmPkLrgCfgReq(Pst *pst, RgMngmt *cfg);
uint8_t BuildAndSendE2SetupReq();
uint8_t egtpHdlDatInd(EgtpMsg egtpMsg);
uint8_t BuildAndSendDUConfigUpdate();
uint16_t getTransId();
uint8_t cmPkLrgSchCfgReq(Pst * pst,RgMngmt * cfg);
uint8_t sendCellDeleteReqToMac(uint16_t cellId);

packMacCellCfgReq packMacCellCfgOpts[] =
{
   packMacCellCfg, /* packing for loosely coupled */
   MacProcCellCfgReq, /* packing for tightly coupled */
   packMacCellCfg, /* packing for light weight loosly coupled */
};

DuMacCellStart packMacCellStartOpts[] =
{
   packMacCellStart,   /* Loose coupling */
   MacProcCellStart,   /* TIght coupling */
   packMacCellStart    /* Light weight-loose coupling */
};

DuMacCellStop packMacCellStopOpts[] =
{
   packMacCellStop,   /* Loose coupling */
   MacProcCellStop,   /* TIght coupling */
   packMacCellStop    /* Light weight-loose coupling */
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

   DU_LOG("\nDEBUG   -->  DU_APP : RLC Gen Cfg Req sent for inst %d", inst);

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
      DU_LOG("\nDEBUG   -->  DU_APP : RLC MAC Lower Sap Cfg Req sent for inst %d", inst);

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
      DU_LOG("\nDEBUG   -->  DU_APP : RLC DL/UL Lower Sap Cfg Req sent for inst %d", inst);
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

   DU_LOG("\nDEBUG   -->  DU_APP : RLC Kwu Upper Sap Cfg Req sent for inst %d", inst);
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
   uint16_t        cellId = 0;
   uint16_t        idx;
   for(idx=0; idx< DEFAULT_CELLS; idx++)
   {
      DuCellCb *cell = NULLP;
      DU_ALLOC(cell, sizeof(DuCellCb));
      if(cell == NULLP)
      {
         DU_LOG("\nERROR  -->  DU_APP : Memory Allocation failed in duProcCfgComplete");
         ret = RFAILED;
      }
      else
      {
         uint8_t idx1=0; 
         memset(cell, 0, sizeof(DuCellCb));
         cell->cellId = ++cellId;
         memset(&cell->cellInfo.nrEcgi.plmn, 0, sizeof(Plmn));
         cell->cellInfo.nrEcgi.plmn.mcc[0] = PLMN_MCC0;
         cell->cellInfo.nrEcgi.plmn.mcc[1] = PLMN_MCC1;
         cell->cellInfo.nrEcgi.plmn.mcc[2] = PLMN_MCC2;
         cell->cellInfo.nrEcgi.plmn.mnc[0] = PLMN_MNC0;
         cell->cellInfo.nrEcgi.plmn.mnc[1] = PLMN_MNC1;
         cell->cellInfo.nrEcgi.cellId = cell->cellId;
         cell->cellInfo.nrPci = NR_PCI; 
         cell->cellInfo.fiveGsTac = DU_TAC;
         memset(&cell->cellInfo.plmn[idx1], 0, sizeof(Plmn));
         for(idx1=0; idx1<MAX_PLMN; idx1++)
         {
            cell->cellInfo.plmn[idx1].mcc[0] = PLMN_MCC0;
            cell->cellInfo.plmn[idx1].mcc[1] = PLMN_MCC1;
            cell->cellInfo.plmn[idx1].mcc[2] = PLMN_MCC2;
            cell->cellInfo.plmn[idx1].mnc[0] = PLMN_MNC0;
            cell->cellInfo.plmn[idx1].mnc[1] = PLMN_MNC1;
         }
         cell->cellInfo.maxUe = duCfgParam.maxUe;
         cell->cellStatus = CELL_OUT_OF_SERVICE;
         gCellStatus = CELL_DOWN;
         cell->gnbDuUeF1apIdGenerator = 0;
         duCb.cfgCellLst[duCb.numCfgCells] = cell;
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
		  DU_LOG("\nDEBUG   -->  DU_APP : BIND OF RLC DL TO MAC (RGU) SAP SUCCESSFUL");
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
		  DU_LOG("\nDEBUG   -->  DU_APP : BIND OF RLC UL TO MAC (RGU) SAP SUCCESSFUL");
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

   DU_LOG("\nDEBUG   -->  DU_APP : RLC UL Cfg Status %d", cfm->cfm.status);
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
      DU_LOG("\nDEBUG   -->  DU_APP : RLC UL Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
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
      DU_LOG("\nERROR  -->  DU_APP : Config confirm NOK from RLC UL");
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
   DU_LOG("\nDEBUG   -->  DU_APP : RLC DL Cfg Status %d", cfm->cfm.status);
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
      DU_LOG("\nDEBUG   -->  DU_APP : RLC DL Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
      if(rlcDlCfg == DU_RLC_DL_CONFIGURED)
      {
	 rlcDlCfg = 0;
	 //Start configuration of MAC
	 duSendMacCfg();

      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP : Config confirm NOK from RLC DL");
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

   DU_LOG("\nDEBUG   -->  DU_APP : MAC Gen Cfg Req sent");

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

   DU_LOG("\nDEBUG  -->  DU_APP : MAC Rgu USap Cfg Req sent");

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
      DU_LOG("\nDEBUG   -->  DU_APP : MAC Cfg Cfm received for the element %d ",cfm->hdr.elmId.elmnt);
      if(macCfg == MAC_CONFIGURED && numRlcMacSaps == MAX_MAC_SAP)
      {
	 macCfg = 0;
	 DU_LOG("\nDEBUG   -->  DU_APP : Completed sending Configs");
	 macCfgInst = 0;
	 duBindUnbindRlcToMacSap(RLC_DL_INST, ABND);
      }

   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP : Config confirm NOK from MAC");
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
   RlcCntrl  *cntrl = NULLP;
   RlcMngmt  rlcMngmt;
   Pst      pst;


   DU_SET_ZERO(&rlcMngmt, sizeof(RlcMngmt));
   DU_SET_ZERO(&pst, sizeof(Pst));

   if (action == ABND)
   {
      DU_LOG("\nDEBUG   -->  DU_APP : Cntrl Req to RLC inst %d to bind MAC sap", inst);
   }
   else
   {
      DU_LOG("\nDEBUG   -->  DU_APP : Cntrl Req to RLC inst %d to unbind MAC sap", inst);
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
      DU_LOG("\nERROR  -->  DU_APP : Invalid assocId %d received", ntfy->u.assocChange.assocId);
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
   memset(pst, 0, sizeof(Pst));
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

   DU_LOG("\nDEBUG   -->  DU_APP : Sending EGTP config request");

   memset(&egtpCfg, 0, sizeof(EgtpConfig));
   memcpy(&egtpCfg, &duCfgParam.egtpParams, sizeof(EgtpConfig));

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
      DU_LOG("\nDEBUG   -->  DU_APP : EGTP configuraton complete");
#ifdef EGTP_TEST
      duSendEgtpSrvOpenReq();
#endif
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP : EGTP configuraton failed");
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

   DU_LOG("\nDEBUG   -->  DU_APP : Sending EGTP server open request");

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
      DU_LOG("\nDEBUG   -->  DU_APP : EGTP server opened successfully");
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP : EGTP server opening failed");
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

uint8_t duSendEgtpTnlMgmtReq(uint8_t action, uint32_t teIdTobeMod, GtpTnlCfg *ueCbTnlCfg)
{
   uint8_t ret =ROK;
   Pst pst;
   EgtpTnlEvt tnlEvt;

   DU_LOG("\nDEBUG   -->  DU_APP : Sending EGTP tunnel management request for teId [%d]", ueCbTnlCfg->teId);

   /* ADD/MOD/DEL per tunnel */
   tnlEvt.action = action;
   tnlEvt.remTeid = ueCbTnlCfg->teId;
   if(action != EGTP_TNL_MGMT_ADD)
   {
      tnlEvt.lclTeid = teIdTobeMod;
   }
   else
   {
      tnlEvt.lclTeid = ueCbTnlCfg->teId;
   }
   duFillEgtpPst(&pst, EVTTNLMGMTREQ);
   ret = egtpTnlMgmtReq(&pst, tnlEvt);
   return ret;
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
      DU_LOG("\nDEBUG  -->  DU_APP: Tunnel management confirm OK");
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP: Tunnel management failed");
      ret = RFAILED;
   }

   return (ret);
}

/*******************************************************************
 *
 * @brief Sends UL user data over to EGTP
 *
 * @details
 *
 *    Function : duSendEgtpDatInd
 *
 *    Functionality: Sends UL user data over to EGTP
 *
 * @params[in] UL data buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
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

   DU_LOG("\nDEBUG   -->  DU_APP : MAC Sch Cfg sent");

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

   DU_LOG("\nINFO   -->  DU_APP : Configuring all Layer is complete");

   if((ret = duSctpCfgReq(duCfgParam.sctpParams)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed configuring Sctp Params");
      ret = RFAILED;
   }
   if((ret = duSctpAssocReq(F1_INTERFACE)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to send AssocReq F1");
      ret = RFAILED;
   }
   if((ret = duSctpAssocReq(E2_INTERFACE)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to send AssocReq E2");
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
	       DU_LOG("\nDEBUG   -->  DU_APP : Received SCH CFG CFM at DU APP");
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
 * @param[in]cell id
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t duBuildAndSendMacCellCfg(uint16_t cellId)
{
   Pst pst;
   MacCellCfg *duMacCellCfg = NULLP;

   DU_ALLOC_SHRABL_BUF(duMacCellCfg, sizeof(MacCellCfg));
   if(duMacCellCfg == NULLP)
   {
      return RFAILED;
   }

   /* store the address in the duCellCb so that we can free on confirm msg */
   duCb.actvCellLst[cellId-1]->duMacCellCfg = duMacCellCfg;

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
   uint8_t  actvCellIdx  = 0;
   uint8_t  ret          = ROK;

   for(actvCellIdx = 0; actvCellIdx < MAX_NUM_CELL; actvCellIdx++)
   {
      if(macCellCfgCfm->cellId == duCb.actvCellLst[actvCellIdx]->cellId)
      {
         duCb.actvCellLst[actvCellIdx]->duMacCellCfg = NULLP;
      }
   }
   if(macCellCfgCfm->rsp == ROK)
   {
      /* Build and send GNB-DU config update */
      ret = BuildAndSendDUConfigUpdate(SERV_CELL_TO_MODIFY);

      /* Build and Send Cell Start Req to MAC */
      ret = duBuildAndSendMacCellStart();
   }
   else
   {
      /* TODO : Action to be taken if cell configuration fails. 
       * Should CU be informed? */

      DU_LOG("\nERROR  -->  DU_APP : Mac cell cfg failed");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds and sends cell start request to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendMacCellStart
 *
 *    Functionality:
 *       Builds and sends cell start request to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duBuildAndSendMacCellStart()
{
   Pst pst;
   OduCellId *cellId = NULL;

   DU_LOG("\nINFO   -->  DU APP : Building and Sending cell start request to MAC");

   /* Send Cell Start Request to MAC */
   DU_ALLOC_SHRABL_BUF(cellId, sizeof(OduCellId));
   if(!cellId)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory alloc failed while building cell start request");
      return RFAILED;
   }

   for(uint8_t id = 0; id < duCb.numActvCells; id++) 
   {
      if(duCb.actvCellLst[id])
      {
	 cellId->cellId = duCb.actvCellLst[id]->cellId;

	 /* Fill Pst */
	 FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_CELL_START);

	 return (*packMacCellStartOpts[pst.selector])(&pst, cellId);
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
 *    Function : duBuildAndSendMacCellStop 
 *
 *    Functionality:
 *       Builds and sends cell stop request to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duBuildAndSendMacCellStop(uint16_t cellId)
{
   Pst pst;
   uint16_t cellIdx=0;
   OduCellId *oduCellId = NULL;
   
   DU_LOG("\nINFO   -->  DU APP : Building and Sending cell stop request to MAC");

   GET_CELL_IDX(cellId, cellIdx);
   if(duCb.actvCellLst[cellIdx] != NULLP)
   {
      /* Send Cell Stop Request to MAC */
      DU_ALLOC_SHRABL_BUF(oduCellId, sizeof(OduCellId));
      if(!oduCellId)
      {
         DU_LOG("\nERROR  -->  DU APP : duBuildAndSendMacCellStop():  Memory allocation failed ");
         return RFAILED;
      }
      memset(oduCellId, 0, sizeof(OduCellId));
      oduCellId->cellId = duCb.actvCellLst[cellIdx]->cellId;

      /* Fill Pst */
      FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_CELL_STOP);

      return (*packMacCellStopOpts[pst.selector])(&pst, oduCellId);
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : duBuildAndSendMacCellStop(): cellId[%d] doesnot exists", cellId);
      return RFAILED;
   }
   return ROK;
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
uint8_t duHandleStopInd(Pst *pst, OduCellId *cellId)
{
   DuCellCb *cellCb = NULLP;

   if(cellId->cellId <=0 || cellId->cellId > MAX_NUM_CELL)
   {
      DU_LOG("\nERROR  -->  DU APP : Invalid Cell Id %d in duHandleStopInd()", cellId->cellId);
   }

   if(duGetCellCb(cellId->cellId, &cellCb) != ROK)
      return RFAILED;

   if((cellCb->cellStatus == ACTIVATED) || (cellCb->cellStatus == DELETION_IN_PROGRESS))
   {
      DU_LOG("\nINFO   -->  DU APP : 5G-NR Cell %d is DOWN", cellId->cellId);
      if(sendCellDeleteReqToMac(cellId->cellId) == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : duHandleStopInd(): Failed to send Cell delete req to MAC for\
               cellId[%d]", cellId->cellId);
         return RFAILED;
      }


#ifdef O1_ENABLE
      DU_LOG("\nINFO   -->  DU APP : Raise cell down alarm for cell id=%d", cellId->cellId);
      raiseCellAlrm(CELL_DOWN_ALARM_ID, cellId->cellId);
      setCellOpState(cellId->cellId, DISABLED, INACTIVE);
#endif
   }

   if((pst->selector == ODU_SELECTOR_LWLC) || (pst->selector == ODU_SELECTOR_TC))
      DU_FREE_SHRABL_BUF(MAC_MEM_REGION, pst->pool, cellId, sizeof(OduCellId));

   cellCb->cellStatus = CELL_OUT_OF_SERVICE; //TODO: cell status must be set to OOS after all UE and cell cleanup which is not
                                             //supported now

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

   DU_LOG("\nINFO  -->  DU APP : UL CCCH Indication received");

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
   DuCellCb *cellCb = NULLP;
   DuUeCb   ueCb ={0};
  
   if(duGetCellCb(ulRrcMsgInfo->cellId, &cellCb) != ROK)
      return RFAILED;
   if(ulRrcMsgInfo->ueIdx > 0)
   {
   ueCb = cellCb->ueCb[ulRrcMsgInfo->ueIdx -1];


   BuildAndSendULRRCMessageTransfer(ueCb, ulRrcMsgInfo->lcId, ulRrcMsgInfo->msgLen, \
      ulRrcMsgInfo->rrcMsg);

   DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulRrcMsgInfo->rrcMsg, ulRrcMsgInfo->msgLen);
   DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulRrcMsgInfo, sizeof(RlcUlRrcMsgInfo));
   }
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
   DuCellCb *cellCb = NULLP;
   DuUeCb   ueCb;
   uint8_t  ret = RFAILED;

   if(duGetCellCb(rrcDeliveryReport->cellId, &cellCb) != ROK)
      return RFAILED;
   
   ueCb = cellCb->ueCb[rrcDeliveryReport->ueIdx -1];
   ret = BuildAndSendRrcDeliveryReport(ueCb.gnbCuUeF1apId, ueCb.gnbDuUeF1apId,rrcDeliveryReport);

   DU_FREE_SHRABL_BUF(pst->region, pst->pool, rrcDeliveryReport, sizeof(RrcDeliveryReport));
   return ret;
}

/*******************************************************************
 *
 * @brief Process UL user data from RLC
 *
 * @details
 *
 *    Function : DuProcRlcUlUserDataTrans
 *
 *    Functionality: Process UL user data from RLC
 *
 * @params[in] Post structure
 *             UL user data
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuProcRlcUlUserDataTrans(Pst *pst, RlcUlUserDatInfo *ulUserData)
{
   uint8_t  rbIdx;
   EgtpMsg  egtpMsg;
   Buffer   *mBuf;

   DU_LOG("\nDEBUG  -->  DU APP : Received UL user data");

   /* Fill EGTP header */
   egtpMsg.msgHdr.msgType = EGTPU_MSG_GPDU;
   egtpMsg.msgHdr.nPdu.pres = FALSE;
   egtpMsg.msgHdr.seqNum.pres = FALSE;
   egtpMsg.msgHdr.extHdr.udpPort.pres = FALSE;
   egtpMsg.msgHdr.extHdr.pdcpNmb.pres = FALSE;

   /* Fetch EGTP tunnel info */
   for(rbIdx = 0; rbIdx < duCb.numDrb; rbIdx++)
   {
      if((duCb.upTnlCfg[rbIdx] != NULLP) && (duCb.upTnlCfg[rbIdx]->drbId == ulUserData->rbId))
      {
	 if(duCb.upTnlCfg[rbIdx]->tnlCfg1)
	 {
            egtpMsg.msgHdr.teId = duCb.upTnlCfg[rbIdx]->tnlCfg1->teId; /*As we are supporting only 1 tunnel per DRB*/
	    break;
	 }
      }
   }

   if (ODU_GET_MSG_BUF(DU_APP_MEM_REGION, DU_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocated buffer memory in DuProcRlcUlUserDataTrans");
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulUserData->userData, ulUserData->msgLen);
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulUserData, sizeof(RlcUlUserDatInfo));
      return RFAILED;
   }
   oduCpyFixBufToMsg(ulUserData->userData, mBuf, ulUserData->msgLen);
   ODU_PRINT_MSG(mBuf, 0, 0);
   egtpMsg.msg = mBuf;
   egtpHdlDatInd(egtpMsg);

   DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulUserData->userData, ulUserData->msgLen);
   DU_FREE_SHRABL_BUF(pst->region, pst->pool, ulUserData, sizeof(RlcUlUserDatInfo));
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
