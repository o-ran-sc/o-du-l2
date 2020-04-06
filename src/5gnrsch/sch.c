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
 
     Name:     sch.c
  
     Type:     C source file
  
     Desc:     C source code for scheduler fucntions
  
     File:     sch.c
  
**********************************************************************/

/** @file sch.c
@brief This file implements the schedulers main access to MAC layer code.
*/
#include "stdbool.h"
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "du_log.h"

extern SchCb schCb[SCH_MAX_INST];
/* local defines */
SchCellCfgCfmFunc SchCellCfgCfmOpts[] = 
{
	packSchCellCfgCfm,     /* LC */
	MacProcSchCellCfgCfm,  /* TC */
	packSchCellCfgCfm      /* LWLC */
};

/**
 * @brief Task Initiation function. 
 *
 * @details
 *
 *     Function : schActvInit
 *     
 *     This function is supplied as one of parameters during MAC's 
 *     task registration. MAC will invoke this function once, after
 *     it creates and attaches this TAPA Task to a system task.
 *     
 *  @param[in]  Ent Entity, the entity ID of this task.     
 *  @param[in]  Inst Inst, the instance ID of this task.
 *  @param[in]  Region Region, the region ID registered for memory 
 *              usage of this task.
 *  @param[in]  Reason Reason.
 *  @return  int
 *      -# ROK
 **/
int schActvInit
(
Ent    entity,            /* entity */
Inst   instId,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
{
   Inst inst = (instId  - SCH_INST_START);

   /* Initialize the MAC TskInit structure to zero */
   cmMemset ((uint8_t *)&schCb[inst], 0, sizeof(schCb));

   /* Initialize the MAC TskInit with received values */
   schCb[inst].schInit.ent = entity;
   schCb[inst].schInit.inst = inst;
   schCb[inst].schInit.region = region;
   schCb[inst].schInit.pool = 0;
   schCb[inst].schInit.reason = reason;
   schCb[inst].schInit.cfgDone = FALSE;
   schCb[inst].schInit.acnt = FALSE;
   schCb[inst].schInit.usta = FALSE;
   schCb[inst].schInit.trc = FALSE;
   schCb[inst].schInit.procId = SFndProcId();

   RETVALUE(ROK);
} /* schActvInit */

/**
 * @brief Scheduler instance Configuration Handler. 
 *
 * @details
 *
 *     Function : SchInstCfg
 *     
 *     This function in called by HandleSchGenCfgReq(). It handles the
 *     general configurations of the scheduler instance. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  RgCfg *cfg, the Configuaration information 
 *  @return  U16
 *      -# LCM_REASON_NOT_APPL 
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
PUBLIC U16 SchInstCfg
(
RgCfg *cfg,            /* Configuaration information */
Inst  dInst
)
{
   uint16_t ret = LCM_REASON_NOT_APPL;
   Inst     inst = (dInst - SCH_INST_START);

   printf("\nEntered SchInstCfg()");
   /* Check if Instance Configuration is done already */
   if (schCb[inst].schInit.cfgDone == TRUE)
   {
      RETVALUE(LCM_REASON_INVALID_MSGTYPE);
   }
   /* Update the Pst structure for LM interface */
   cmMemcpy((U8 *)&schCb[inst].schInit.lmPst,
            (U8 *)&cfg->s.schInstCfg.genCfg.lmPst,
             sizeof(Pst));
   
   schCb[inst].schInit.inst = inst;
   schCb[inst].schInit.lmPst.srcProcId = schCb[inst].schInit.procId;
   schCb[inst].schInit.lmPst.srcEnt = schCb[inst].schInit.ent;
   schCb[inst].schInit.lmPst.srcInst = schCb[inst].schInit.inst +
   SCH_INST_START;
   schCb[inst].schInit.lmPst.event = EVTNONE;

   schCb[inst].schInit.region = cfg->s.schInstCfg.genCfg.mem.region;
   schCb[inst].schInit.pool = cfg->s.schInstCfg.genCfg.mem.pool;
   schCb[inst].genCfg.tmrRes = cfg->s.schInstCfg.genCfg.tmrRes;
#ifdef LTE_ADV
   schCb[inst].genCfg.forceCntrlSrbBoOnPCel =  cfg->s.schInstCfg.genCfg.forceCntrlSrbBoOnPCel;
   schCb[inst].genCfg.isSCellActDeactAlgoEnable =  cfg->s.schInstCfg.genCfg.isSCellActDeactAlgoEnable;
#endif
   schCb[inst].genCfg.startCellId    = cfg->s.schInstCfg.genCfg.startCellId;
#if 0
   /* Initialzie the timer queue */   
   cmMemset((U8 *)&schCb[inst].tmrTq, 0, sizeof(CmTqType)*RGSCH_TQ_SIZE);
   /* Initialize the timer control point */
   cmMemset((U8 *)&schCb[inst].tmrTqCp, 0, sizeof(CmTqCp));
   schCb[inst].tmrTqCp.tmrLen = RGSCH_TQ_SIZE;

   /* SS_MT_TMR needs to be enabled as schActvTmr needs instance information */
   /* Timer Registration request to SSI */
   if (SRegTmrMt(schCb[inst].schInit.ent, dInst,
      (S16)schCb[inst].genCfg.tmrRes, schActvTmr) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "SchInstCfg(): Failed to "
             "register timer.");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }   
#endif               
   /* Set Config done in TskInit */
   schCb[inst].schInit.cfgDone = TRUE;
   printf("\nScheduler gen config done");
   
   RETVALUE(ret);
}

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : HandleSchGenCfgReq
 *     
 *     This function handles the configuration
 *     request received at scheduler instance from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rgHdlGenCfg() or rgHdlSapCfg().
 *     -# Invokes RgMiLrgSchCfgCfm() to send back the confirmation to the LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
int HandleSchGenCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
{
   uint16_t       ret = LCM_PRIM_OK;
   uint16_t       reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;

   if(pst->dstInst < SCH_INST_START)
   {
      DU_LOG("\nInvalid inst ID");
      DU_LOG("\nHandleSchGenCfgReq(): "
                "pst->dstInst=%d SCH_INST_START=%d", pst->dstInst,SCH_INST_START); 
      RETVALUE(ROK);
   }
   printf("\nReceived scheduler gen config");
   /* Fill the post structure for sending the confirmation */
   SchFillCfmPst(pst, &cfmPst, cfg);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId =
      cfg->hdr.transId;
#endif

   cfm.hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STSCHINST:
         reason = SchInstCfg(&cfg->t.cfg,pst->dstInst );
         break;
      default:
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
         DU_LOG("\nInvalid Elmnt=%d", cfg->hdr.elmId.elmnt);
         break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm.cfm.status = ret;
   cfm.cfm.reason = reason;

   SchSendCfgCfm(&cfmPst, &cfm);
   /*   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(RgMngmt)); */
   
   RETVALUE(ROK);
}/*-- HandleSchGenCfgReq --*/

/**
 * @brief slot indication from MAC to SCH.
 *
 * @details
 *
 *     Function : macSchSlotInd 
 *      
 *      This API is invoked by PHY to indicate slot indication to Scheduler for
 *      a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SlotIndInfo    *slotInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
int macSchSlotInd 
(
Pst                 *pst, 
SlotIndInfo         *slotInd
)
{
   Inst  inst = pst->dstInst-SCH_INST_START;

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   schProcessSlotInd(slotInd, inst);

   RETVALUE(ROK);
}  /* macSchSlotInd */

/**
 * @brief inti cellCb based on cellCfg
 *
 * @details
 *
 *     Function : InitSchCellCb 
 *      
 *      This API is invoked after receiving schCellCfg
 *           
 *  @param[in]  schCellCb *cell
 *  @param[in]  SchCellCfg *schCellCfg
 *  @return  int
 *      -# ROK 
 *      -# RFAILED 
 **/
int InitSchCellCb(Inst inst, SchCellCfg *schCellCfg)
{
   SchCellCb *cell;
   SCH_ALLOC(cell, sizeof(SchCellCb));
	if(!cell)
	{
      DU_LOG("\nMemory allocation failed in InitSchCellCb");
		return RFAILED;
	}

	cell->cellId = schCellCfg->cellId; 
	cell->instIdx = inst;
	switch(schCellCfg->scsCommon)
	{
	   case SCH_SCS_15KHZ:
		{
			cell->numSlots = SCH_NUM_SLOTS;
		}
		break;
		default:
		   DU_LOG("\nSCS %d not supported", schCellCfg->scsCommon);
	}
  
   for(uint8_t idx=0; idx<SCH_NUM_SLOTS; idx++)
	{
		SchDlAlloc *schDlAlloc;
		SCH_ALLOC(schDlAlloc, sizeof(SchDlAlloc));
		if(!schDlAlloc)
		{
			DU_LOG("\nMemory allocation failed in InitSchCellCb");
			return RFAILED;
		}

		schDlAlloc->totalPrb = (schCellCfg->bandwidth *
				1000)/(schCellCfg->scsCommon)*SCH_NUM_SC_PRB;
		for(uint8_t itr=0; itr<MAX_SSB_IDX; itr++)
		{
			schDlAlloc->assignedPrb[itr] = 0;
		}
		for(uint8_t itr=0; itr<MAX_SSB_IDX; itr++)
		{
			memset(&schDlAlloc->ssbInfo[itr], 0, sizeof(SsbInfo));
		}

		cell->dlAlloc[idx] = schDlAlloc;
	}
	schCb[inst].cells[inst] = cell; //Sphoorthi TODO: check if this works

   DU_LOG("\nCell init completed for cellId:%d", cell->cellId);

   return ROK;   
}

/**
 * @brief cell config from MAC to SCH.
 *
 * @details
 *
 *     Function : macSchCellCfgReq
 *      
 *      This API is invoked by MAC to send cell config to SCH
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SchCellCfg     *schCellCfg
 *  @return  int
 *      -# ROK 
 *      -# RFAILED 
 **/
int SchHdlCellCfgReq
(
Pst                 *pst, 
SchCellCfg          *schCellCfg
)
{
   int ret = ROK;
   SchCellCb *cellCb;
	SchCellCfgCfm schCellCfgCfm;
	Pst rspPst;
	Inst inst = pst->dstInst-1; 

	InitSchCellCb(inst, schCellCfg);
	cellCb = schCb[inst].cells[inst];
   cellCb->macInst = pst->srcInst;
   memcpy(&cellCb->cellCfg, &schCellCfg, sizeof(SchCellCfg));

   SCH_FILL_RSP_PST(rspPst, inst);
	rspPst.event = EVENT_SCH_CELL_CFG_CFM;
	schCellCfgCfm.rsp = RSP_OK;
  
   ret = (*SchCellCfgCfmOpts[rspPst.selector])(&rspPst, &schCellCfgCfm);

   return ret;

}

/**********************************************************************
         End of file
**********************************************************************/
