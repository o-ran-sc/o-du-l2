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
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "cm_mblk.h"        /* Common LTE Defines */
#include "tfu.h"           /* RGU Interface defines */
#ifdef FAPI
#include "fapi.h"
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
#include "cm_mblk.x"        /* Common LTE Defines */
#include "tfu.x"           /* RGU Interface includes */
#include "du_log.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "lwr_mac_upr_inf.h"

#ifdef FAPI
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
   pst->selector = MAC_SELECTOR_TC;
}

/*******************************************************************
 *
 * @brief Processes Slot Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : handleSlotInd
 *
 *    Functionality:
 *     Processes Slot Indication from PHY and sends to MAC
 *
 * @params[in] fapi_slot_ind_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
U16 handleSlotInd(fapi_slot_ind_t *fapiSlotInd)
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
 * @brief Processes Rach Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : handleRachInd
 *
 *    Functionality:
 *         Processes Rach Indication from PHY and sends to MAC
 *
 * @params[in] fapi_rach_indication_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t handleRachInd(fapi_rach_indication_t  *fapiRachInd)
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
      rachPdu->pci = fapiRachInd->rachPdu[pduIdx].physCellId;
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

#endif /* FAPI */

void handlePhyMessages(uint16_t msgType, uint32_t msgSize, void *msg)
{
#ifdef FAPI
   /* extract the header */
   fapi_msg_t *header;
   header = (fapi_msg_t *)msg;

   switch(header->message_type_id)
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
            DU_LOG("\nLOWER MAC: PHY has moved to running state");
            clGlobalCp.phyState = PHY_STATE_RUNNING;
         }

         fapi_slot_ind_t *slotInd;
         slotInd  = (fapi_slot_ind_t *)msg;
         handleSlotInd(slotInd);
         break;
      }
      case FAPI_ERROR_INDICATION:
      {
         break;
      }
      case FAPI_RX_DATA_INDICATION:
      {
         break;
      }  
      case FAPI_CRC_INDICATION:
      {
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
         if((handleRachInd(rachInd)) == ROK)
         {
            if(ricIndFlag)
            {
               BuildAndSendRicIndication();
            }
         }
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
