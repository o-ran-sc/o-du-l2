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
uint8_t unpackRxData(uint16_t cellId, SlotTimingInfo slotInfo, RxDataIndPdu *rxDataIndPdu)
{
   uint8_t   ueIdx = 0;       /* Iterator for UE list */
   uint8_t   lcId = 0;        /* LC ID of a sub pdu */
   uint8_t   fBit = 0;        /* Value of F Bit in MAC sub-header */
   uint8_t   idx = 0;         /* Iterator for received PDU */
   uint16_t  length = 0;      /* Length of payload in a sub-PDU */ 
   uint8_t   *pdu = NULLP;    /* Payload in sub-PDU */
   uint16_t  pduLen = 0;      /* Length of undecoded PDU */
   uint8_t   *rxDataPdu = NULLP;  /* Received PDU in Rx Data Ind */
   uint16_t  cellIdx = 0;     /* Cell Index */
   uint8_t   ret =ROK;

   GET_CELL_IDX(cellId, cellIdx);
   pduLen = rxDataIndPdu->pduLength;
   rxDataPdu = rxDataIndPdu->pduData;
   GET_UE_IDX(rxDataIndPdu->rnti, ueIdx);
   ueIdx = ueIdx -1;

   while(pduLen > 0)
   {
      /* MSB in 1st octet is Reserved bit. Hence not decoding it. 
	 2nd MSB in 1st octet is R/F bit depending upon type of payload */
      fBit = (1 << 7) & rxDataPdu[idx];

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
		  DU_LOG("\nERROR  -->  MAC : UL CCCH PDU memory allocation failed");
		  return RFAILED;
	       }  
	       idx++;
	       memcpy(pdu, &rxDataPdu[idx], length);
	       pduLen -= length;
	       idx = idx + length;

	       /* store msg3 pdu in macRaCb for CRI value */
	       memcpy(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg3Pdu, pdu, length);

	       /* Send UL-CCCH Indication to DU APP */
	       ret = macProcUlCcchInd(macCb.macCell[cellIdx]->cellId, rxDataIndPdu->rnti, length, pdu);
	       break;
	    }

	 case MAC_LCID_MIN ... MAC_LCID_MAX :
	    {
	       DU_LOG("\nINFO   -->  MAC : PDU received for LC ID %d", lcId);
	       pduLen--;
	       idx++;

	       length = rxDataPdu[idx];
	       if(fBit)
	       {
	          pduLen--;
		  idx++;
		  length = (length << 8) & rxDataPdu[idx];
	       }

	       /*  Copying the payload to send to RLC */
	       MAC_ALLOC_SHRABL_BUF(pdu, length);
	       if(!pdu)
	       {
		  DU_LOG("\nERROR  -->  MAC : Memory allocation failed while demuxing Rx Data PDU");
		  return RFAILED;
	       }
	       pduLen--;
               idx++;
	       memcpy(pdu, &rxDataPdu[idx], length);
	       pduLen -= length;
	       idx = idx + length;

	       /* Delete RA cb once RRC setup complete received */
	       if(macCb.macCell[cellIdx]->macRaCb[ueIdx].crnti == rxDataIndPdu->rnti)
	       {
	          MAC_FREE(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu, \
		          macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
             MAC_FREE(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, \
                      macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize - TX_PAYLOAD_HDR_LEN);
	          memset(&macCb.macCell[cellIdx]->macRaCb[ueIdx], 0, sizeof(MacRaCbInfo));
	       }

	       /* Send UL Data to RLC */
	       ret = macProcUlData(cellId, rxDataIndPdu->rnti, slotInfo, lcId, length, pdu);

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
	    {
	       uint8_t  lcgId         = 0;
	       uint8_t  bufferSizeIdx = 0;
	       uint8_t  crnti         = 0;
	       uint32_t bufferSize    = 0;

	       pduLen--;

	       idx++;
	       crnti = rxDataIndPdu->rnti;
	       /* 5 LSB bits in pdu represent buffer size */
	       bufferSizeIdx = (~((~0) << 5)) & rxDataPdu[idx];
	       /* first 3 MSB bits in pdu represent LCGID */
	       lcgId = (rxDataPdu[idx]) >> 5;
	       /* determine actual number of bytes requested */
	       bufferSize = shortBsrBytesTable[bufferSizeIdx];
	       ret = macProcShortBsr(macCb.macCell[cellIdx]->cellId, crnti, lcgId, bufferSize);
	       pduLen--;
	       idx++;
	
	       break;
	    }

	 case MAC_LCID_LONG_BSR :
	    break;

	 case MAC_LCID_PADDING :
	    break;
	 
	 default:
	    {
	       DU_LOG("\nERROR  -->  MAC : Invalid LC Id %d", lcId);
	       return RFAILED;
	    }
      } /* End of switch */

      if(lcId == MAC_LCID_PADDING)
      {
	 break;
      }
   } /* End of While */

   return ret;
} /* End of unpackRxData */

/**********************************************************************
  End of file
 **********************************************************************/
