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
#include "lrg.x"           /* layer management typedefs for MAC */
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_utils.h"

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
uint8_t unpackRxData(uint16_t cellId, SlotIndInfo slotInfo, RxDataIndPdu *rxDataIndPdu)
{
   uint8_t   lcId;        /* LC ID of a sub pdu */
   uint8_t   fBit = 0;    /* Value of F Bit in MAC sub-header */
   uint8_t   idx = 0;     /* Iterator for received PDU */
   uint16_t  length;      /* Length of payload in a sub-PDU */ 
   uint8_t   *pdu;        /* Payload in sub-PDU */
   uint16_t  pduLen;      /* Length of undecoded PDU */
   uint8_t   *rxDataPdu;  /* Received PDU in Rx Data Ind */
   uint16_t  cellIdx;     /* Cell Index */

   GET_CELL_IDX(cellId, cellIdx);
   pduLen = rxDataIndPdu->pduLength;
   rxDataPdu = rxDataIndPdu->pduData;

   while(pduLen > 0)
   {
      /* MSB in 1st octet is Reserved bit. Hence not decoding it. 
	 2nd MSB in 1st octet is R/F bit depending upon type of payload */
      fBit = (1 << 7) & rxDataPdu[idx];

      /* LC id is the 6 LSB in 1st octet */
      lcId = (~((~0) << 6)) & rxDataPdu[idx];

      /* Since 1st byte is now decoded, reducing pduLen by 1 and
	 incrementing index by 1
       */
      pduLen--;
      idx++;

      switch(lcId)
      {
	 case MAC_LCID_CCCH :
	    {
	       /* for UL CCCH,fixed length of MAC SDU */
	       length = 6;

	       /*  Allocating sharable memory to send ul ccch msg to du app*/
	       MAC_ALLOC_SHRABL_BUF(pdu, length);
	       if(!pdu)
	       {
		  DU_LOG("\nMAC : UL CCCH PDU memory allocation failed");
		  return RFAILED;
	       }  
	       memcpy(pdu, &rxDataPdu[idx], length);
	       pduLen -= length;
	       idx = idx + length;

	       /* store msg3 pdu in macRaCb for CRI value */
	       memcpy(macCb.macCell[cellIdx]->macRaCb[0].msg3Pdu, pdu, length);

	       /* Send UL-CCCH Indication to DU APP */
	       macSendUlCcchInd(pdu, cellId, rxDataIndPdu->rnti); 
	       break;
	    }

	 case MAC_LCID_MIN ... MAC_LCID_MAX :
	    {
	       DU_LOG("\nMAC : PDU received for LC ID %d", lcId);

	       length = rxDataPdu[idx];
	       if(fBit)
	       {
		  idx++;
		  length = (length << 8) & rxDataPdu[idx];
	       }

	       /*  Copying the payload to send to RLC */
	       MAC_ALLOC_SHRABL_BUF(pdu, length);
	       if(!pdu)
	       {
		  DU_LOG("\nMAC : Memory allocation failed while demuxing Rx Data PDU");
		  return RFAILED;
	       }
               idx++;
	       memcpy(pdu, &rxDataPdu[idx], length);
	       pduLen -= length;
	       idx = idx + length;
	       
	       /* Send UL Data to RLC */
	       macFillAndSendUlData(cellId, rxDataIndPdu->rnti, slotInfo, lcId, pdu, length);

	       break;
	    }
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
