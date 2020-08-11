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

/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"
#ifdef INTEL_FAPI
#include "fapi.h"
#endif

/* header/extern include files (.x) */
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"

#ifdef INTEL_FAPI
/* Function pointer for slot indication from lower mac to mac */
packSlotIndMsg packSlotIndOpts[] =
{
   packLcSlotInd,  /* packing for loosely coupled */
   fapiMacSlotInd, /* packing for tightly coupled */
   packLwlcSlotInd /* packing for light weight loosly coupled */
};

/* Function pointer for rach indication from lower mac to mac */ 
packRachIndMsg sendRachIndOpts[] =
{
   packRachInd,
   fapiMacRachInd,
   packRachInd
};

/* Function pointer for crc indication from lower mac to mac */
packCrcIndMsg sendCrcIndOpts[] =
{
   packCrcInd,
   fapiMacCrcInd,
   packCrcInd
};

/* Function pointer for Rx Data indication from lower mac to mac */
packRxDataIndMsg sendRxDataIndOpts[] =
{
   packRxDataInd,
   fapiMacRxDataInd,
   packRxDataInd
};

/* Function pointer for stop indication from lower mac to mac */ 
packStopIndMsg sendStopIndOpts[] =
{
   packStopInd,
   fapiMacStopInd,
   packStopInd
};
/*******************************************************************
 *
 * @brief Fills post structure
 *
 * @details
 *
 *    Function : fillLwrMacToMacPst
 *
 *    Functionality:
 *     Fills post structure used to send message from lower MAC
 *     to MAC
 *
 * @params[in] Pst pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillLwrMacToMacPst(Pst *pst)
{
   pst->srcProcId = 0;
   pst->dstProcId = 0;
   pst->srcEnt = ENTTF;
   pst->dstEnt = ENTRG;
   pst->srcInst = 0;
   pst->dstInst = 0;
   pst->region = 0;
   pst->pool =  0; 
   pst->selector = ODU_SELECTOR_TC;
}

/*******************************************************************
 *
 * @brief Processes Slot Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procSlotInd
 *
 *    Functionality:
 *     Processes Slot Indication from PHY and sends to MAC
 *
 * @params[in] fapi_slot_ind_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
U16 procSlotInd(fapi_slot_ind_t *fapiSlotInd)
{
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;
   uint16_t ret;
   SlotIndInfo slotInd;

   fillLwrMacToMacPst(&pst);
   pst.event = EVENT_SLOT_IND_TO_MAC;

   slotInd.sfn = fapiSlotInd->sfn;
   slotInd.slot = fapiSlotInd->slot;

   ret = (*packSlotIndOpts[pst.selector])(&pst, &slotInd);

#ifdef INTEL_WLS
   slotIndIdx++;
   if(slotIndIdx > WLS_MEM_FREE_PRD)
   {
      slotIndIdx = 1;
   }
   freeWlsBlockList(slotIndIdx - 1);
#endif

   return ret;
}

/*******************************************************************
 *
 * @brief Handles stop indication recived from PHY
 *
 * @details
 *
 *    Function : procStopInd
 *
 *    Functionality:
 *         Handles Stop Indication received from PHY
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procStopInd()
{
   uint8_t ret;
   Pst pst;

   clGlobalCp.phyState = PHY_STATE_CONFIGURED;
   DU_LOG("\nLWR_MAC: PHY has moved to configured state");

   fillLwrMacToMacPst(&pst);
   pst.event = EVENT_STOP_IND_TO_MAC;

   ret = (*sendStopIndOpts[pst.selector])(&pst);
   return ret;
}
/*******************************************************************
 *
 * @brief Processes Rach Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procRachInd
 *
 *    Functionality:
 *         Processes Rach Indication from PHY and sends to MAC
 *
 * @params[in] fapi_rach_indication_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procRachInd(fapi_rach_indication_t  *fapiRachInd)
{
   Pst          pst;
   uint8_t      pduIdx;
   uint8_t      prmbleIdx;
   RachPduInfo  *rachPdu;
   RachInd      rachInd;

   rachInd.timingInfo.sfn = fapiRachInd->sfn;
   rachInd.timingInfo.slot = fapiRachInd->slot;
   rachInd.numPdu = fapiRachInd->numPdus;
   for(pduIdx=0; pduIdx < rachInd.numPdu; pduIdx++)
   {
      rachPdu = &rachInd.rachPdu[pduIdx];
      rachPdu->pci = fapiRachInd->rachPdu[pduIdx].phyCellId;
      rachPdu->symbolIdx = fapiRachInd->rachPdu[pduIdx].symbolIndex;
      rachPdu->slotIdx = fapiRachInd->rachPdu[pduIdx].slotIndex;
      rachPdu->freqIdx = fapiRachInd->rachPdu[pduIdx].freqIndex;
      rachPdu->numPream = fapiRachInd->rachPdu[pduIdx].numPreamble; 
      for(prmbleIdx=0; prmbleIdx<rachPdu->numPream; prmbleIdx++)
      {
	 rachPdu->preamInfo[prmbleIdx].preamIdx = \
	    fapiRachInd->rachPdu[pduIdx].preambleInfo[prmbleIdx].preambleIndex;
	 rachPdu->preamInfo[prmbleIdx].timingAdv = \
	    fapiRachInd->rachPdu[pduIdx].preambleInfo[prmbleIdx].timingAdvance;
      }
   }
   fillLwrMacToMacPst(&pst);
   pst.event = EVENT_RACH_IND_TO_MAC;

   (*sendRachIndOpts[pst.selector])(&pst, &rachInd);
   return ROK;

}/* handleRachInd */

/*******************************************************************
 *
 * @brief Handles CRC indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procCrcInd
 *
 *    Functionality:
 *      Handles CRC indication from PHY and sends to MAC
 *
 * @params[in] fapi_crc_ind_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procCrcInd(fapi_crc_ind_t  *fapiCrcInd)
{
   Pst          pst;
   uint8_t      crcInfoIdx;
   uint8_t      crcStatusIdx;
   CrcInfo      *crcIndInfo;
   CrcInd       crcInd;

   crcInd.timingInfo.sfn = fapiCrcInd->sfn;
   crcInd.timingInfo.slot = fapiCrcInd->slot;
   crcInd.numCrc = fapiCrcInd->numCrcs;

   for(crcInfoIdx = 0; crcInfoIdx < crcInd.numCrc; crcInfoIdx++)
   {
      crcIndInfo = &crcInd.crcInfo[crcInfoIdx];
      crcIndInfo->handle      = fapiCrcInd->crc[crcInfoIdx].handle;
      crcIndInfo->rnti        = fapiCrcInd->crc[crcInfoIdx].rnti;
      crcIndInfo->harqId      = fapiCrcInd->crc[crcInfoIdx].harqId;
      crcIndInfo->tbCrcStatus = fapiCrcInd->crc[crcInfoIdx].tbCrcStatus;
      crcIndInfo->numCb       = fapiCrcInd->crc[crcInfoIdx].numCb;
      for(crcStatusIdx = 0; crcStatusIdx < crcIndInfo->numCb; crcStatusIdx++)
      {
	 crcIndInfo->cbCrcStatus[crcStatusIdx] = \
	    fapiCrcInd->crc[crcInfoIdx].cbCrcStatus[crcStatusIdx];
      }
      crcIndInfo->ul_cqi  = fapiCrcInd->crc[crcInfoIdx].ul_cqi;
      crcIndInfo->timingAdvance = fapiCrcInd->crc[crcInfoIdx].timingAdvance;
      crcIndInfo->rssi = fapiCrcInd->crc[crcInfoIdx].rssi;
   }

   fillLwrMacToMacPst(&pst);
   pst.event = EVENT_CRC_IND_TO_MAC;

   (*sendCrcIndOpts[pst.selector])(&pst, &crcInd);
   return ROK;

} /* handleCrcInd */

/*******************************************************************
 *
 * @brief Handles Rx Data indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procRxDataInd
 *
 *    Functionality:
 *      Handles Rx Data indication from PHY and sends to MAC
 *
 * @params[in] fapi_rx_data_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procRxDataInd(fapi_rx_data_indication_t  *fapiRxDataInd)
{
   Pst           pst;
   uint8_t       pduIdx;
   RxDataInd     rxDataInd;
   RxDataIndPdu  *pdu;   

   rxDataInd.timingInfo.sfn = fapiRxDataInd->sfn; 
   rxDataInd.timingInfo.slot = fapiRxDataInd->slot;
   rxDataInd.numPdus = fapiRxDataInd->numPdus;

   for(pduIdx = 0; pduIdx < rxDataInd.numPdus; pduIdx++)
   {
      pdu = &rxDataInd.pdus[pduIdx];
      pdu->handle = fapiRxDataInd->pdus[pduIdx].handle;
      pdu->rnti = fapiRxDataInd->pdus[pduIdx].rnti;
      pdu->harqId = fapiRxDataInd->pdus[pduIdx].harqId;
      pdu->pduLength = fapiRxDataInd->pdus[pduIdx].pdu_length;
      pdu->ul_cqi = fapiRxDataInd->pdus[pduIdx].ul_cqi;
      pdu->timingAdvance = fapiRxDataInd->pdus[pduIdx].timingAdvance;
      pdu->rssi = fapiRxDataInd->pdus[pduIdx].rssi;

      MAC_ALLOC(pdu->pduData, pdu->pduLength);
      memcpy(pdu->pduData, fapiRxDataInd->pdus[pduIdx].pduData, pdu->pduLength);
   }

   fillLwrMacToMacPst(&pst);
   pst.event = EVENT_RX_DATA_IND_TO_MAC;

   (*sendRxDataIndOpts[pst.selector])(&pst, &rxDataInd);
   return ROK;
}

#endif /* FAPI */

void procPhyMessages(uint16_t msgType, uint32_t msgSize, void *msg)
{
#ifdef INTEL_FAPI
   /* extract the header */
   fapi_msg_t *header;
   header = (fapi_msg_t *)msg;

   switch(header->msg_id)
   {
      case FAPI_PARAM_RESPONSE:
      case FAPI_CONFIG_RESPONSE:
	 {
	    sendToLowerMac(msgType, msgSize, msg);
	    break;
	 }
      case FAPI_SLOT_INDICATION:
	 {
	    if(clGlobalCp.phyState == PHY_STATE_CONFIGURED)
	    {
	       DU_LOG("\nLWR_MAC: PHY has moved to running state");
	       clGlobalCp.phyState = PHY_STATE_RUNNING;
	    }

	    fapi_slot_ind_t *slotInd;
	    slotInd  = (fapi_slot_ind_t *)msg;
	    procSlotInd(slotInd);
	    break;
	 }
      case FAPI_ERROR_INDICATION:
	 {
	    break;
	 }
      case FAPI_RX_DATA_INDICATION:
	 {
	    fapi_rx_data_indication_t *rxDataInd;
	    rxDataInd = (fapi_rx_data_indication_t *)msg;
	    procRxDataInd(rxDataInd);
	    break;
	 }  
      case FAPI_CRC_INDICATION:
	 {
	    fapi_crc_ind_t  *crcInd;
	    crcInd = (fapi_crc_ind_t *)msg;
	    procCrcInd(crcInd);
	    break;
	 }  
      case FAPI_UCI_INDICATION:
	 {
	    break;
	 }
      case FAPI_SRS_INDICATION:
	 {
	    break;
	 }  
      case FAPI_RACH_INDICATION:
	 {
	    fapi_rach_indication_t  *rachInd;
	    rachInd = (fapi_rach_indication_t *)msg;
	    procRachInd(rachInd);
	    break;
	 }
      case FAPI_STOP_INDICATION:
	 {
	    DU_LOG("\nLWR_MAC: Handling Stop Indication");
	    procStopInd();
	    break;
	 }  
   }
#ifdef INTEL_WLS
   WLS_MEM_FREE(msg, LWR_MAC_WLS_BUF_SIZE); 
#endif
#endif
}

/**********************************************************************
  End of file
 **********************************************************************/
