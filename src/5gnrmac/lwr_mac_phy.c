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

/* This file contains APIs to send/receive messages from PHY */

#include "common_def.h"
#include "lrg.h"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "mac_utils.h"
#include "lwr_mac_utils.h"
#include "lwr_mac.h"
#ifdef INTEL_FAPI
#include "nr5g_fapi_internal.h"
#include "fapi_vendor_extension.h"
#endif
#ifdef INTEL_WLS_MEM
#include "wls_lib.h"
#endif

#ifdef INTEL_WLS_MEM
CmLListCp wlsBlockToFreeList[WLS_MEM_FREE_PRD];
#endif

uint8_t rgClHndlCfgReq ARGS((void *msg));
void l1ProcessFapiRequest ARGS((uint8_t msgType, uint32_t msgLen, void *msg));

#ifdef INTEL_WLS_MEM

/*******************************************************************
 *
 * @brief Sends request to start wls receiver thread
 *
 * @details
 *
 *    Function : LwrMacStartWlsRcvr
 *
 *    Functionality:
 *      Sends request to start wls receiver thread
 *
 * @params[in] 
 * @return void
 *
 * ****************************************************************/
void LwrMacStartWlsRcvr()
{
   Pst pst;
   Buffer *mBuf;

   DU_LOG("\nINFO  -->  LWR MAC : Requesting to start WLS receiver thread");

   /* Filling post */
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_LWR_MAC_TO_LWR_MAC(pst, EVT_START_WLS_RCVR);

   if (ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  LWR MAC : Memory allocation failed for LwrMacStartWlsRcvr");
      return;
   }

   ODU_POST_TASK(&pst, mBuf);
}

/*******************************************************************
 *
 * @brief Enqueues memory blocks for use by L1
 *
 * @details
 *
 *    Function : LwrMacEnqueueWlsBlock
 *
 *    Functionality:
 *      Enqueues memory blocks for use by L1
 *
 * @params[in] 
 * @return void
 *
 * ****************************************************************/
void LwrMacEnqueueWlsBlock()
{
   void *memPtr;
   void *wlsHdlr = NULLP;

   WLS_MEM_ALLOC(memPtr, LWR_MAC_WLS_BUF_SIZE);

   if(memPtr) 
   {
      mtGetWlsHdl(&wlsHdlr);
      /* allocate blocks for UL transmittion */
      while(WLS_EnqueueBlock(wlsHdlr, WLS_VA2PA(wlsHdlr, memPtr)))
      {
	 WLS_MEM_ALLOC(memPtr, LWR_MAC_WLS_BUF_SIZE);
	 if(!memPtr)
	    break;
      }

      // free not enqueued block
      if(memPtr)
      {
	 WLS_MEM_FREE(memPtr, LWR_MAC_WLS_BUF_SIZE);
      }
   }
}/* LwrMacEnqueueWlsBlock */

/*******************************************************************
 *
 * @brief Add memory block (to be freed later) to list
 *
 * @details
 *
 *    Function : addWlsBlockToFree 
 *
 *    Functionality:
 *       Add memory block (to be freed later) to list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void addWlsBlockToFree(void *msg, uint32_t msgLen, uint8_t idx)
{
   CmLList         *node = NULLP;
   WlsBlockToFree  *block = NULLP;
   MAC_ALLOC(block, sizeof(WlsBlockToFree));
   if(block)
   {
      MAC_ALLOC(node, sizeof(CmLList));
      if(node)
      {
	 block->ptr = msg;
	 block->size = msgLen;

	 node->node = (PTR)block;
	 cmLListAdd2Tail(&wlsBlockToFreeList[idx], node);
      }
   }
}/* addWlsBlockToFree */


/*******************************************************************
 *
 * @brief Free DL Memory blocks stored in list
 *
 * @details
 *
 *    Function : freeWlsBlockList
 *
 *    Functionality: Free DL Memory blocks stored in list
 *
 * @params[in] Array index to be freed
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeWlsBlockList(uint8_t idx)
{
   CmLList         *node;
   WlsBlockToFree  *block;
   if(wlsBlockToFreeList[idx].count)
   {
      CM_LLIST_FIRST_NODE(&wlsBlockToFreeList[idx], node);
      while(node)
      {
	 block = (WlsBlockToFree *)node->node;
	 cmLListDelFrm(&wlsBlockToFreeList[idx], node);
	 WLS_MEM_FREE(block->ptr, block->size);
	 MAC_FREE(block, sizeof(WlsBlockToFree));
	 MAC_FREE(node, sizeof(CmLList));
	 node = NULL;
	 CM_LLIST_FIRST_NODE(&wlsBlockToFreeList[idx], node);
      }
   }
}

#ifdef OAI_TESTING 
#if 0
//I will remove this in next gerrit
void hexdump1(void *data, uint32_t size) 
{
        DU_LOG("\nPBORLA size %u\n", size);
    const unsigned char *byte = (const unsigned char *)data;
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", byte[i]);
        if ((i + 1) % 16 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}
#endif
uint8_t* packConfigReq(p_fapi_api_queue_elem_t cfgReqElem, uint32_t *len) 
{
    uint8_t *mBuf = NULL;
    uint8_t *out = NULL;
    uint32_t msgLen = 0;
    fapi_config_req_t *configReq = NULL;
    uint16_t totalTlv = 0;

    LWR_MAC_ALLOC(mBuf, sizeof(fapi_config_req_t));
    if (mBuf == NULL) 
    {
        DU_LOG("Memory allocation failed");
        return NULL;
    }

    out = mBuf;
    memset(mBuf, 0, sizeof(fapi_config_req_t));

    configReq = (fapi_config_req_t *)(cfgReqElem + 1);

    CMCHKPKLEN(oduPackPostUInt8, configReq->header.numMsg, &out, &msgLen);
    CMCHKPKLEN(oduPackPostUInt8, configReq->header.opaque, &out, &msgLen);
    CMCHKPKLEN(oduPackPostUInt16, configReq->header.msg_id, &out, &msgLen);
    CMCHKPKLEN(oduPackPostUInt32, configReq->header.length, &out, &msgLen);

    totalTlv = configReq->number_of_tlvs;
    uint8_t randmTlvCnt=  25;//OAI does not verify the TLV number
    CMCHKPKLEN(oduPackPostUInt8, randmTlvCnt, &out, &msgLen);

    for(uint16_t idx=0;idx<totalTlv;idx++)
    {
	    fapi_uint32_tlv_t tlv =configReq->tlvs[idx];
	
	    CMCHKPKLEN(oduPackPostUInt16, tlv.tl.tag, &out, &msgLen);
	    CMCHKPKLEN(oduPackPostUInt16, tlv.tl.length, &out, &msgLen);
	    switch(reverseBytes16(tlv.tl.length))
	    {
		    case 1:
			    {
				    uint8_t val=tlv.value;
				    CMCHKPKLEN(oduPackPostUInt8, val, &out, &msgLen);
				    CMCHKPKLEN(oduPackPostUInt8, 0, &out, &msgLen);
				    CMCHKPKLEN(oduPackPostUInt8, 0, &out, &msgLen);
				    CMCHKPKLEN(oduPackPostUInt8, 0, &out, &msgLen);
				    break;
			    }
		    case 2:
			    {
				    uint16_t val=tlv.value;
				    CMCHKPKLEN(oduPackPostUInt16, val, &out, &msgLen);
				    CMCHKPKLEN(oduPackPostUInt16, 0, &out, &msgLen);
				    break;
			    }
		    case 4:
			    {
				    uint32_t val=tlv.value;
				    CMCHKPKLEN(oduPackPostUInt32, val, &out, &msgLen);
				    break;
			    }


	    }
    }

    *len = msgLen;
    *((uint32_t *)(mBuf + 4)) = reverseBytes32(msgLen);
    return mBuf;
}

uint8_t *packStartReq(p_fapi_api_queue_elem_t startReqElem, uint32_t *len)
{
    uint8_t *mBuf = NULL;
    uint16_t totalLen = 0;
    uint8_t *out = NULL;
    fapi_start_req_t *startReq = NULL;

    
    LWR_MAC_ALLOC(mBuf, sizeof(fapi_start_req_t));
    if (mBuf == NULL) 
    {
        DU_LOG("Memory allocation failed");
        return NULL;
    }

    out = mBuf;  

    memset(mBuf, 0, sizeof(fapi_start_req_t));

    startReq = (fapi_start_req_t *)(startReqElem + 1);  

    
    CMCHKPKLEN(oduPackPostUInt8, startReq->header.numMsg, &out, &totalLen);
    CMCHKPKLEN(oduPackPostUInt8, startReq->header.opaque, &out, &totalLen);
    CMCHKPKLEN(oduPackPostUInt16, startReq->header.msg_id, &out, &totalLen);
    CMCHKPKLEN(oduPackPostUInt32, startReq->header.length, &out, &totalLen);

    *len = totalLen;  
    return mBuf;  
}

uint8_t * packHeaderbuffer(p_fapi_api_queue_elem_t  headerElem, uint32_t *len)
{

   uint8_t *mBuf = NULLP;
   Pst pst;
   uint16_t totalLen = 0;
   fapi_msg_header_t *msgHeader = NULLP;

   LWR_MAC_ALLOC(mBuf, ( sizeof(fapi_msg_header_t)));
   
   msgHeader= (fapi_msg_header_t*)(headerElem+ 1);
   CMCHKPKLEN(oduPackPostUInt8, msgHeader->num_msg, &mBuf, &totalLen);
   CMCHKPKLEN(oduPackPostUInt8, msgHeader->handle, &mBuf, &totalLen);

   DU_LOG("\n PBORLA %s %u",__func__, totalLen);
   *len=totalLen;
   return mBuf;
}

uint8_t *processAndSendMsg(p_fapi_api_queue_elem_t currMsg, uint32_t *msgLen)
{
    uint8_t *mBuf = NULL;
    uint32_t len = 0;

    switch (currMsg->msg_type)
    {
        case FAPI_CONFIG_REQUEST:
            mBuf = packConfigReq(currMsg, &len);
            break;
        case FAPI_START_REQUEST:
            mBuf = packStartReq(currMsg, &len);
            break;
#if 0
        case FAPI_DL_TTI_REQUEST:
            mBuf = buildAndSendDlTtiReq(currMsg, &len);
            hexdump1(mBuf, len);  // Debugging: View the buffer contents
            break;
        case FAPI_UL_TTI_REQUEST:
            mBuf = buildAndSendUlTtiReq(currMsg, &len);
            break;
        case FAPI_UL_DCI_REQUEST:
            mBuf = buildAndSendUlDciReqBuffer(currMsg, &len);
            break;
        case FAPI_TX_DATA_REQUEST:
            mBuf = buildAndSendTxDataReqBuffer(currMsg, &len);
            break;
#endif
        default:
            mBuf = packHeaderbuffer(currMsg, &len);
            break;
    }

    if (mBuf == NULL)
    {
        DU_LOG("\nERROR  -->  LWR MAC : Failed to build message buffer for msg_type %d", currMsg->msg_type);
        return NULL;
    }

    *msgLen = len;
    return mBuf;
}

#endif

/*******************************************************************
 *
 * @brief Receives msg from L1 
 *
 * @details
 *
 *    Function :LwrMacRecvPhyMsg 
 *
 *    Functionality:
 *      Receives L1 Msg and enqueues memort for UL msg
 *
 * @params[in] 
 * @return 
 * ****************************************************************/
void LwrMacRecvPhyMsg()
{
#ifdef INTEL_FAPI
   uint32_t numMsgToGet;   /* Number of Memory blocks to get */
   void     *wlsHdlr;       /* WLS handler */
   uint64_t l1Msg;         /* Message received */
   void     *l1MsgPtr;
   uint32_t msgSize;
   uint16_t msgType;
   uint16_t flag = 0;
   p_fapi_api_queue_elem_t currElem  = NULLP;

   mtGetWlsHdl(&wlsHdlr);
   if(WLS_Ready(wlsHdlr) == 0) 
   {
      while(true)
      {
	 numMsgToGet = WLS_Wait(wlsHdlr);
	 if(numMsgToGet == 0)
	 {
	    continue;
	 }

	 while(numMsgToGet--)
	 {
	    currElem = NULLP;
	    l1Msg = (uint64_t)NULLP;
	    l1MsgPtr = NULLP;
	    l1Msg = WLS_Get(wlsHdlr, &msgSize, &msgType, &flag);
	    if(l1Msg)
	    {

	       l1MsgPtr = WLS_PA2VA(wlsHdlr, l1Msg); 
	       currElem = (p_fapi_api_queue_elem_t) l1MsgPtr;
	       if(currElem->msg_type != FAPI_VENDOR_MSG_HEADER_IND)
	       {
             procPhyMessages(currElem->msg_type, 0, (void *)(currElem + 1));
	       }
	       WLS_MEM_FREE(currElem, LWR_MAC_WLS_BUF_SIZE);
	    }
	 }
	 LwrMacEnqueueWlsBlock();
      }
   }
#endif
} /* LwrMacRecvPhyMsg */

#endif /* INTEL_WLS_MEM */

/*******************************************************************
 *
 * @brief Send FAPI messages to Intel PHY/Phy stub
 *
 * @details
 *
 *    Function : LwrMacSendToL1
 *
 *    Functionality: Send FAPI messages to Intel PHY/Phy stub
 *
 * @params[in] Message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t LwrMacSendToL1(void *msg)
{
   uint8_t ret = ROK;
#ifdef INTEL_FAPI
   uint32_t msgLen =0;
   p_fapi_api_queue_elem_t currMsg = NULLP;

#ifdef CALL_FLOW_DEBUG_LOG   
   char message[100];

   currMsg = (p_fapi_api_queue_elem_t)msg;
   while(currMsg)
   {
      switch(currMsg->msg_type)
      {
         case FAPI_PARAM_REQUEST:
            strcpy(message, "FAPI_PARAM_REQUEST");
            break;
         case FAPI_CONFIG_REQUEST:
            strcpy(message, "FAPI_CONFIG_REQUEST");
            break;
         case FAPI_START_REQUEST:
            strcpy(message, "FAPI_START_REQUEST");
            break;
         case FAPI_DL_TTI_REQUEST:
            strcpy(message, "FAPI_DL_TTI_REQUEST");
            break;
         case FAPI_TX_DATA_REQUEST:
            strcpy(message, "FAPI_TX_DATA_REQUEST");
            break;
         case FAPI_UL_TTI_REQUEST:
            strcpy(message, "FAPI_UL_TTI_REQUEST");
            break;
         case FAPI_STOP_REQUEST:
            strcpy(message, "FAPI_STOP_REQUEST");
            break;
         case FAPI_UL_DCI_REQUEST:
            strcpy(message, "FAPI_UL_DCI_REQUEST");
            break;
         default:
            strcpy(message, "INVALID_MSG");
            break;
      }
      DU_LOG("\nCall Flow: ENTLWRMAC -> PHY : %s\n",message);
      currMsg = currMsg->p_next;
   }
#endif


#ifdef INTEL_WLS_MEM
   void * wlsHdlr = NULLP;

   mtGetWlsHdl(&wlsHdlr);
   if(msg)
   {
#ifndef OAI_TESTING 
      currMsg = (p_fapi_api_queue_elem_t)msg;
      msgLen = currMsg->msg_len + sizeof(fapi_api_queue_elem_t);
      addWlsBlockToFree(currMsg, msgLen, (lwrMacCb.phySlotIndCntr-1));
      if(currMsg->p_next == NULLP)
      {
	 DU_LOG("\nERROR  -->  LWR MAC : There cannot be only one block to send");
	 return RFAILED;
      }

      /* Sending first block */
      ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, currMsg), msgLen, currMsg->msg_type, WLS_SG_FIRST);
      if(ret != 0)
      {
	 DU_LOG("\nERROR  -->  LWR MAC : Failure in sending message to PHY");
	 return RFAILED;
      }
      currMsg = currMsg->p_next;

      while(currMsg)
      {
	 /* Sending the next msg */
	 msgLen = currMsg->msg_len + sizeof(fapi_api_queue_elem_t);
	 addWlsBlockToFree(currMsg, msgLen, (lwrMacCb.phySlotIndCntr-1));
	 if(currMsg->p_next != NULLP)
	 {
	    ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, currMsg), msgLen, currMsg->msg_type, WLS_SG_NEXT);
	    if(ret != 0)
	    {
	       DU_LOG("\nERROR  -->  LWR MAC : Failure in sending message to PHY");
	       return RFAILED;
	    }
	    currMsg = currMsg->p_next;
	 }
	 else
	 {
	    /* Sending last msg */
	    ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, currMsg), msgLen, currMsg->msg_type, WLS_SG_LAST);
	    if(ret != 0)
	    {
	       DU_LOG("\nERROR  -->  LWR MAC : Failure in sending message to PHY");
	       return RFAILED;
	    }
	    currMsg = NULLP;
	 }
      }
#else
      uint8_t *mBuf = NULL;
      currMsg = (p_fapi_api_queue_elem_t)msg;
      msgLen = currMsg->msg_len;

      mBuf = processAndSendMsg(currMsg, &msgLen);
      if (mBuf == NULL) {
	      return RFAILED;
      }

      ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, mBuf), msgLen, currMsg->msg_type, WLS_SG_FIRST);
      if (ret != 0) {
	      return RFAILED;
      }

      currMsg = currMsg->p_next;
      while (currMsg)
      {
	      msgLen = currMsg->msg_len;
	      mBuf = processAndSendMsg(currMsg, &msgLen);
	      if (mBuf == NULL) {
		      return RFAILED;
	      }

	      ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, mBuf), msgLen, currMsg->msg_type, (currMsg->p_next == NULLP) ? WLS_SG_LAST : WLS_SG_NEXT);
	      if (ret != 0) {
		      return RFAILED;
	      }

	      currMsg = currMsg->p_next;
      }
	
      currMsg = (p_fapi_api_queue_elem_t)msg;
      while(currMsg)
      {
	      p_fapi_api_queue_elem_t nextMsg = currMsg->p_next;
	      msgLen= currMsg->msg_len + sizeof(fapi_api_queue_elem_t);
	      LWR_MAC_FREE(currMsg, msgLen);
	      currMsg = nextMsg;
      }

#endif
   }
#else
   p_fapi_api_queue_elem_t nextMsg = NULLP;

   /* FAPI header and vendor specific msgs are freed here. Only 
    * the main FAPI messages are sent to phy stub */
   currMsg = (p_fapi_api_queue_elem_t)msg;
   while(currMsg)
   {
	   nextMsg = currMsg->p_next;
	   msgLen = currMsg->msg_len + sizeof(fapi_api_queue_elem_t);
	   if((currMsg->msg_type != FAPI_VENDOR_MSG_HEADER_IND) && \
			   (currMsg->msg_type != FAPI_VENDOR_MESSAGE))
	   {
		   l1ProcessFapiRequest(currMsg->msg_type, msgLen, currMsg);
	   }
	   else
	   {
		   LWR_MAC_FREE(currMsg, msgLen);   
	   }
	   currMsg = nextMsg;
   }
#endif
#endif
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
