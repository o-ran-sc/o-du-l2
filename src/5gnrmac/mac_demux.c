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

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"           /* Layer manager interface includes*/
#include "crg.h"           /* CRG interface includes*/
#include "rgu.h"           /* RGU interface includes*/
#include "tfu.h"           /* TFU interface includes */
#include "rg_sch_inf.h"    /* SCH interface includes */
#include "rg_prg.h"       /* PRG (MAC-MAC) interface includes*/
#include "rg_env.h"       /* MAC environmental includes*/
#include "rg.h"           /* MAC includes*/
#include "rg_err.h"       /* MAC error includes*/
#include "du_log.h"

/* header/extern include files (.x) */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* CRG interface includes */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"        /* PRG (MAC-MAC) Interface typedefs */
#include "du_app_mac_inf.h"
#include "mac.h"
#include "rg.x"            /* typedefs for MAC */

/*******************************************************************
 *
 * @brief De-mux of MAC-Sub PDUs from Rx Data Ind Pdu
 *
 * @detail
 *
 *    Function : unpackRxData
 *
 *    Functionality:
 *     De-mux of MAC-Sub PDUs from Rx Data Ind Pdu
 *
 * @params[in] Pointer to PDU received
 *             PDU length
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
int unpackRxData(RxDataIndPdu *rxDataIndPdu)
{
   uint8_t   lcId;
   uint8_t   idx = 0;
   uint16_t  length;
   uint8_t   *pdu;
   uint16_t  pduLen;
   uint8_t   *rxDataPdu;

   pduLen = rxDataIndPdu->pduLength;
   rxDataPdu = rxDataIndPdu->pduData;

   while(pduLen > 0)
   {
      /* LC id is the 6 LSB in 1st octet */
      lcId = (~((~0) << 6)) & rxDataPdu[idx];

      switch(lcId)
      {
         case MAC_LCID_CCCH :
         {
			   pduLen--;

			    /* for UL CCCH,fixed length of MAC SDU */
			   length = 6;
            
            /*  Allocating sharable memory to send ul ccch msg to du app*/
            MAC_ALLOC_SHRABL_BUF(pdu, length);
            if(!pdu)
            {
               DU_LOG("\nMAC : UL CCCH PDU memory allocation failed");
               return RFAILED;
            }  
            idx++;
            memcpy(pdu, &rxDataPdu[idx], length);
            pduLen -= length;
            idx = idx + length;

            /* store msg3 pdu in macRaCb for CRI value */
            memcpy(&macCb.macCell->macRaCb[0].msg3Pdu, pdu, length);

            /* Send UL-CCCH Indication to DU APP */
				macSendUlCcchInd(pdu, macCb.macCell->cellId, rxDataIndPdu->rnti); 
            break;
         }
         
         case MAC_DEDLC_MIN_LCID ... MAC_DEDLC_MAX_LCID :
            break;

         case MAC_LCID_RESERVED_MIN ... MAC_LCID_RESERVED_MAX :
            break;

         case MAC_LCID_CCCH_48BIT :
            break;

         case MAC_LCID_BIT_RATE_QUERY :
            break;

         case MAC_LCID_MULT_PHR_FOUR_OCT :
            break;

         case MAC_LCID_CFG_GRANT_CFM :
            break;

         case MAC_LCID_MULT_PHR_ONE_OCT:
            break;

         case MAC_LCID_SINGLE_PHR :
            break;

         case MAC_LCID_CRNTI :
            break;

         case MAC_LCID_SHORT_TRUNC_BSR :
            break;

         case MAC_LCID_LONG_TRUNC_BSR :
            break;

         case MAC_LCID_SHORT_BSR :
            break;

         case MAC_LCID_LONG_BSR :
            break;
         
         case MAC_LCID_PADDING :
         {
            break;
         }

         default:
         {
            DU_LOG("\nMAC : Invalid LC Id %d", lcId);
            return RFAILED;
         }
      } /* End of switch */

		if(lcId == MAC_LCID_PADDING)
		   break;

   } /* End of While */

   return ROK;
} /* End of unpackRxData */

/**********************************************************************
         End of file
**********************************************************************/
