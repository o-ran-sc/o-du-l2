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

/* header include files -- defines (.h)  */
#include <stdbool.h>
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
#include "cm_lte.h"        /* common tokens */
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "lrg.h"
#include "crg.h"
#include "rg.h"
#include "du_log.h"
#include "lwr_mac.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "rgu.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "lrg.x"
#include "crg.x"
#include "rg_prg.x"
#include "du_app_mac_inf.h"
#include "mac_upr_inf_api.h"
#include "rg.x"
#include "lwr_mac_fsm.h"
#include "mac.h"

/* This file contains message handling functionality for MAC */

extern MacCb  macCb;

extern void sendToLowerMac(uint16_t msgType, uint32_t msgLen, void *msg);
uint16_t buildMacPdu(RlcMacData *dlData);

/* Function pointer for sending crc ind from MAC to SCH */
MacSchCrcIndFunc macSchCrcIndOpts[]=
{
   packMacSchCrcInd,
   macSchCrcInd,
   packMacSchCrcInd
};

/* Function pointer for sending DL RLC BO Info from MAC to SCH */
MacSchDlRlcBoInfoFunc macSchDlRlcBoInfoOpts[]=
{
   packMacSchDlRlcBoInfo,
   macSchDlRlcBoInfo,
   packMacSchDlRlcBoInfo
};

/*******************************************************************
 *
 * @brief Sends DL BO Info to SCH
 *
 * @details
 *
 *    Function : sendDlRlcBoInfoMacToSch
 *
 *    Functionality:
 *       Sends DL BO Info to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ****************************************************************/
int sendDlRlcBoInfoMacToSch(DlRlcBOInfo *dlBoInfo)
{
   Pst pst;
 
   fillMacToSchPst(&pst);
   pst.event = EVENT_DL_RLC_BO_INFO_TO_SCH;
 
   return(*macSchDlRlcBoInfoOpts[pst.selector])(&pst, dlBoInfo);
}

/*******************************************************************
 *
 * @brief Sends CRC Indication to SCH
 *
 * @details
 *
 *    Function : sendCrcIndMacToSch 
 *
 *    Functionality:
 *       Sends CRC Indication to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
  ****************************************************************/
int sendCrcIndMacToSch(CrcIndInfo *crcInd)
{
   Pst pst;
 
   fillMacToSchPst(&pst);
   pst.event = EVENT_CRC_IND_TO_SCH;
 
   return(*macSchCrcIndOpts[pst.selector])(&pst, crcInd);
}
 
/*******************************************************************
 *
 * @brief Processes CRC Indication from PHY
 *
 * @details
 *
 *    Function : fapiMacCrcInd
 *
 *    Functionality:
 *       Processes CRC Indication from PHY
 *
 * @params[in] Post Structure Pointer
 *             Crc Indication Pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t fapiMacCrcInd(Pst *pst, CrcInd *crcInd)
{
   CrcIndInfo   crcIndInfo;
 
   DU_LOG("\nMAC : Received CRC indication");

   /* Considering one pdu and one preamble */ 
   crcIndInfo.cellId = macCb.macCell->cellId;;
   crcIndInfo.crnti = crcInd->crcInfo[0].rnti;
   crcIndInfo.timingInfo.sfn = crcInd->timingInfo.sfn;
   crcIndInfo.timingInfo.slot = crcInd->timingInfo.slot;
   crcIndInfo.numCrcInd = crcInd->crcInfo[0].numCb;
   crcIndInfo.crcInd[0] = crcInd->crcInfo[0].cbCrcStatus[0];
 
   return(sendCrcIndMacToSch(&crcIndInfo));
}
 
/*******************************************************************
 *
 * @brief Process Rx Data Ind at MAC
 *
 * @details
 *
 *    Function : fapiMacRxDataInd
 *
 *    Functionality:
 *       Process Rx Data Ind at MAC
 *
 * @params[in] Post structure
 *             Rx Data Indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t fapiMacRxDataInd(Pst *pst, RxDataInd *rxDataInd)
{
   uint16_t pduIdx;
 
   DU_LOG("\nMAC : Received Rx Data indication");
  
   /* TODO : compare the handle received in RxDataInd with handle send in PUSCH
	 * PDU, which is stored in raCb */

   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      unpackRxData(&rxDataInd->pdus[pduIdx]);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes DL data from RLC
 *
 * @details
 *
 *    Function : MacRlcProcDlData 
 *
 *    Functionality:
 *      Processes DL data from RLC
 *
 * @params[in] Post structure
 *             DL data
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacRlcProcDlData(Pst* pst, SpId spId, RlcMacData *dlData)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes BO status from RLC
 *
 * @details
 *
 *    Function : MacRlcProcBOStatus
 *
 *    Functionality:
 *      Processes BO status from RLC
 *
 * @params[in] Post structure
 *             BO status
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacRlcProcBOStatus(Pst* pst, SpId spId, RlcMacBOStatus*      boStatus)
{
   return ROK;
}


/*******************************************************************
 *
 * @brief Handles cell start reuqest from DU APP
 *
 * @details
 *
 *    Function : MacHdlCellStartReq
 *
 *    Functionality:
 *      Handles cell start reuqest from DU APP
 *
 * @params[in] Post structure pointer
 *             Cell Start Request info pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacHdlCellStartReq(Pst *pst, MacCellStartInfo  *cellStartInfo)
{
   DU_LOG("\nMAC : Handling cell start request");
   sendToLowerMac(START_REQUEST, 0, cellStartInfo);

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellStartInfo, \
	   sizeof(MacCellStartInfo));

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles cell stop request from DU APP
 *
 * @details
 *
 *    Function : MacHdlCellStartReq
 *
 *    Functionality:
 *        Handles cell stop request from DU APP
 *
 * @params[in] Post structure pointer
 *             Mac Cell stop information
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacHdlCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo)
{
   #ifdef FAPI
   DU_LOG("\nMAC : Sending cell stop request to Lower Mac");
   sendToLowerMac(FAPI_STOP_REQUEST, 0, cellStopInfo);
   #endif

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellStopInfo, \
	   sizeof(MacCellStopInfo));
 
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles DL CCCH Ind from DU APP
 *
 * @details
 *
 *    Function : MacHdlDlCcchInd 
 *
 *    Functionality:
 *      Handles DL CCCH Ind from DU APP
 *
 * @params[in] Post structure pointer
 *             DL CCCH Ind pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacHdlDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo)
{
   DlRlcBOInfo  dlBoInfo;

   DU_LOG("\nMAC : Handling DL CCCH IND");
   
   /* TODO : Fill DL RLC Buffer status info */
   dlBoInfo.cellId = dlCcchIndInfo->cellId;
   dlBoInfo.crnti = dlCcchIndInfo->crnti;
   dlBoInfo.numLc = 0;
   
   if(dlCcchIndInfo->msgType == RRC_SETUP)
   {
      dlBoInfo.numLc++;
      dlBoInfo.boInfo[dlBoInfo.numLc].lcId = 0;    // SRB 0 for msg4
      dlBoInfo.boInfo[dlBoInfo.numLc].dataVolume = \
      strlen((const char*)dlCcchIndInfo->dlCcchMsg);

      /* storing Msg4 Pdu in raCb */
      if(macCb.macCell->macRaCb[0].crnti == dlCcchIndInfo->crnti)
      {
         macCb.macCell->macRaCb[0].msg4PduLen = strlen((const char*)dlCcchIndInfo\
         ->dlCcchMsg);
         MAC_ALLOC(macCb.macCell->macRaCb[0].msg4Pdu, macCb.macCell->macRaCb[0]\
         .msg4PduLen);
         if(macCb.macCell->macRaCb[0].msg4Pdu)
         {
            memcpy(macCb.macCell->macRaCb[0].msg4Pdu, dlCcchIndInfo->dlCcchMsg,\
            macCb.macCell->macRaCb[0].msg4PduLen);
         }
      }
   }
   sendDlRlcBoInfoMacToSch(&dlBoInfo);
   
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, dlCcchIndInfo->dlCcchMsg, \
      strlen((const char*)dlCcchIndInfo->dlCcchMsg));
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, dlCcchIndInfo, sizeof(DlCcchIndInfo));
   return ROK;

}

/*******************************************************************
 *
 * @brief Sends UL CCCH Ind to DU APP
 *
 * @details
 *
 *    Function : macSendUlCcchInd
 *
 *    Functionality:
 *        MAC sends UL CCCH Ind to DU APP
 *
 * @params[in] Post structure pointer
 *            
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t macSendUlCcchInd(uint8_t *rrcContainer, uint16_t cellId, uint16_t crnti)
{
   Pst pst;
	uint8_t ret = ROK;
   UlCcchIndInfo *ulCcchIndInfo = NULLP;

   MAC_ALLOC_SHRABL_BUF(ulCcchIndInfo, sizeof(UlCcchIndInfo));
   if(!ulCcchIndInfo)
   {
		DU_LOG("\nMAC: Memory failed in macSendUlCcchInd");
		return RFAILED;
	}

	ulCcchIndInfo->cellId = cellId;
	ulCcchIndInfo->crnti  = crnti;
	ulCcchIndInfo->ulCcchMsg = rrcContainer;

	/* Fill Pst */
	pst.selector  = DU_MAC_LWLC;
	pst.srcEnt    = ENTRG;
	pst.dstEnt    = ENTDUAPP;
	pst.dstInst   = 0;
	pst.srcInst   = macCb.macInst;
	pst.dstProcId = rgCb[pst.srcInst].rgInit.procId;
	pst.srcProcId = rgCb[pst.srcInst].rgInit.procId;
	pst.region    = MAC_MEM_REGION;
	pst.pool      = MAC_POOL;
	pst.event     = EVENT_MAC_UL_CCCH_IND;
	pst.route     = 0;
	pst.prior     = 0;
	pst.intfVer   = 0;

	if(MacDuAppUlCcchInd(&pst, ulCcchIndInfo) != ROK)
	{
		DU_LOG("\nMAC: Failed to send UL CCCH Ind to DU APP");
		MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo->ulCcchMsg,
				strlen((const char*)ulCcchIndInfo->ulCcchMsg));
		MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo, sizeof(UlCcchIndInfo));
		ret = RFAILED;
	}
	return ret;
}

/*******************************************************************
 *
 * @brief Handles Ue Create Request from DU APP
 *
 * @details
 *
 *    Function : MacHdlUeCreateReq
 *
 *    Functionality:
 *      Handles Ue create Request from DU APP
 *
 * @params[in] Post structure pointer
 *             DuUeCb pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacHdlUeCreateReq(Pst *pst, DuUeCb *ueCfg)
{
   /* TODO : Create MAC UE Context.
	          Copy Tx Pdu from raCb
				 Delete RaCb
				 Send MacUeContext to SCH */
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(DuUeCb));
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/

