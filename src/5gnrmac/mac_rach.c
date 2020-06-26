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
#include <stdbool.h>
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "tfu.h"           /* RGU Interface includes */
#include "lrg.h"

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"           /* RGU Interface includes */
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac.h"
#include "du_log.h"

/* Function pointer for sending rach ind from MAC to SCH */
MacSchRachIndFunc macSchRachIndOpts[]=
{
   packMacSchRachInd,
   macSchRachInd,
   packMacSchRachInd
};

/*******************************************************************
 *
 * @brief Sends RACH indication to SCH
 *
 * @details
 *
 *    Function : sendRachIndMacToSch
 *
 *    Functionality:
 *     Sends RACH indication to SCH
 *
 * @params[in] RACH indication info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendRachIndMacToSch(RachIndInfo *rachInd)
{
   Pst pst;
    
   fillMacToSchPst(&pst);
   pst.event = EVENT_RACH_IND_TO_SCH;
    
   return(*macSchRachIndOpts[pst.selector])(&pst, rachInd); 
}

/*******************************************************************
 *
 * @brief Processes RACH indication from PHY
 *
 * @details
 *
 *    Function : fapiMacRachInd
 *
 *    Functionality:
 *      Processes RACH indication from PHY
 *
 * @params[in] Post structure
 *             Rach indication message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/ 
uint16_t fapiMacRachInd(Pst *pst, RachInd *rachInd)
{
   uint8_t      pduIdx;
   uint8_t      preambleIdx;
   RachIndInfo  rachIndInfo;

   DU_LOG("\nMAC : Received RACH indication");
   /* Considering one pdu and one preamble */
   pduIdx = 0;
   preambleIdx = 0;

   rachIndInfo.cellId = rachInd->rachPdu[pduIdx].pci;
   /* TODO : A.ocate unique crnti for each ue */
   rachIndInfo.crnti = 100;
   rachIndInfo.timingInfo.sfn = rachInd->timingInfo.sfn;
   rachIndInfo.timingInfo.slot = rachInd->timingInfo.slot;
   rachIndInfo.slotIdx = rachInd->rachPdu[pduIdx].slotIdx;
   rachIndInfo.symbolIdx = rachInd->rachPdu[pduIdx].symbolIdx;
   rachIndInfo.freqIdx = rachInd->rachPdu[pduIdx].freqIdx;
   rachIndInfo.preambleIdx = \
      rachInd->rachPdu[pduIdx].preamInfo[preambleIdx].preamIdx;
   rachIndInfo.timingAdv = \
      rachInd->rachPdu[pduIdx].preamInfo[preambleIdx].timingAdv;

   /* storing the value in macRaCb */
   createMacRaCb(rachIndInfo.cellId, rachIndInfo.crnti);

   return(sendRachIndMacToSch(&rachIndInfo));
}

/* spec-38.211 Table 6.3.3.1-7 */
uint8_t UnrestrictedSetNcsTable[MAX_ZERO_CORR_CFG_IDX] = 
{0, 2, 4, 6, 8, 10, 12, 13, 15, 17, 19, 23, 27, 34, 46, 69};

int MacProcUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo)
{
   if(ulSchedInfo != NULLP)
	{
      MacUlSlot *currUlSlot = 
	   &macCb.macCell->ulSlot[ulSchedInfo->slotIndInfo.slot % MAX_SLOT_SUPPORTED];
      memcpy(&currUlSlot->ulInfo, ulSchedInfo, sizeof(UlSchedInfo)); 
   }
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/

