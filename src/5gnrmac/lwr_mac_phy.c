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

#ifdef FAPI_DECODER
#define PORT 8080
#define SERVER_IP "127.0.0.1"  // Change this to the server's IP address
#endif


#ifdef INTEL_WLS_MEM
CmLListCp wlsBlockToFreeList[WLS_MEM_FREE_PRD];
#endif

uint8_t rgClHndlCfgReq ARGS((void *msg));
void l1ProcessFapiRequest ARGS((uint8_t msgType, uint32_t msgLen, void *msg));

#ifdef FAPI_DECODER
/*******************************************************************
 *
 * @brief print const fapi_api_queue_elem_t element list
 *
 * @details
 *
 *    Function : print_queue_elem
 *
 *    Functionality: print const fapi_api_queue_elem_t element list
 *
 * @params[in] const fapi_api_queue_elem_t *elem
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void print_queue_elem(const fapi_api_queue_elem_t *elem) 
{
   if (elem == NULL) {
      printf("Element is NULL\n");
      return;
   }

   printf("Queue Element:\n");
   printf("  msg_type: %u\n", elem->msg_type);
   printf("  num_message_in_block: %u\n", elem->num_message_in_block);
   printf("  msg_len: %u\n", elem->msg_len);
   printf("  align_offset: %u\n", elem->align_offset);
   printf("  time_stamp: %llu\n", (unsigned long long)elem->time_stamp);
   printf("  p_next: %p\n", (void*)elem->p_next);
   printf("  p_tx_data_elm_list: %p\n", (void*)elem->p_tx_data_elm_list);
}
/*******************************************************************
 *
 * @brief calculate fapi msg size with the help of fapi msg id
 *
 * @details
 *
 *    Function : calculateFapiMsgSize
 *
 *    Functionality: calculate fapi msg size with the help of fapi 
 *            msg id
 *
 * @params[in] uint8_t msgType, uint16_t *msgSzie
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void calculateFapiMsgSize(uint8_t msgType, uint16_t *msgSzie)
{
   switch(msgType)
   {
      case FAPI_PARAM_REQUEST:
         *msgSzie= sizeof(fapi_param_req_t);
         break;
      case FAPI_PARAM_RESPONSE:
         *msgSzie=sizeof(fapi_param_resp_t);
         break;
      case FAPI_CONFIG_REQUEST:
         *msgSzie=sizeof(fapi_config_req_t);
         break;
      case FAPI_CONFIG_RESPONSE:
         *msgSzie=sizeof(fapi_config_resp_t);
         break;
      case FAPI_START_REQUEST:
         *msgSzie=sizeof(fapi_start_req_t);
         break;
      case FAPI_STOP_REQUEST:
         *msgSzie=sizeof(fapi_stop_req_t);
         break;
      case FAPI_STOP_INDICATION:
         *msgSzie=sizeof(fapi_stop_ind_t);
         break;
      case FAPI_ERROR_INDICATION:
         *msgSzie=sizeof(fapi_error_ind_t);
         break;
      case FAPI_DL_TTI_REQUEST:
         *msgSzie=sizeof(fapi_dl_tti_req_t);
         break;
      case FAPI_UL_TTI_REQUEST:
         *msgSzie=sizeof(fapi_ul_tti_req_t);
         break;
      case FAPI_SLOT_INDICATION:
         *msgSzie=sizeof(fapi_slot_ind_t);
         break;
      case FAPI_UL_DCI_REQUEST:
         *msgSzie=sizeof(fapi_ul_dci_req_t);
         break;
      case FAPI_TX_DATA_REQUEST:
         *msgSzie=sizeof(fapi_tx_data_req_t);
         break;
      case FAPI_RX_DATA_INDICATION:
         *msgSzie=sizeof(fapi_rx_data_indication_t);
         break;
      case FAPI_CRC_INDICATION:
         *msgSzie=sizeof(fapi_crc_ind_t);
         break;
      case FAPI_UCI_INDICATION:
         *msgSzie=sizeof(fapi_uci_indication_t);
         break;
      case FAPI_SRS_INDICATION:
         *msgSzie=sizeof(fapi_srs_indication_t);
         break;
      case FAPI_RACH_INDICATION:
         *msgSzie=sizeof(fapi_rach_indication_t);
         break;
   }
}

/*******************************************************************
 *
 * @brief serialize_list fapi_api_queue_elem_t list
 *
 * @details
 *
 *    Function : serialize_queue_elem 
 *
 *    Functionality: serialize_list fapi_api_queue_elem_t list
 *             and store in buffer
 *
 * @params[in] const p_fapi_api_queue_elem_t elem, uint8_t *buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
size_t serialize_queue_elem(const p_fapi_api_queue_elem_t elem, uint8_t *buffer) 
{
   size_t offset = 0;

   // Serializep_fapi_api_queue_elem_t 
   memcpy(buffer + offset, elem, sizeof(p_fapi_api_queue_elem_t));
   offset += sizeof(p_fapi_api_queue_elem_t);

   switch (elem->msg_type) {
      case FAPI_VENDOR_MSG_HEADER_IND: // fapi_msg_header_t
         memcpy(buffer + offset, (fapi_msg_header_t *)(elem + 1), sizeof(fapi_msg_header_t));
         offset += sizeof(fapi_msg_header_t);
         break;
      case FAPI_CONFIG_REQUEST: // fapi_config_req_t
         memcpy(buffer + offset, (fapi_config_req_t*)(elem + 1), sizeof(fapi_config_req_t));
         offset += sizeof(fapi_config_req_t);
         break;
      case FAPI_VENDOR_MESSAGE: // fapi_vendor_msg_t
         memcpy(buffer + offset, (fapi_vendor_msg_t*)(elem + 1), sizeof(fapi_vendor_msg_t));
         offset += sizeof(fapi_vendor_msg_t);
         break;
      default:
         fprintf(stderr, "Unknown message type: %u\n", elem->msg_type);
         return 0; // Indicate an error
   }
   return offset;
}

/*******************************************************************
 *
 * @brief create and serialize_list fapi_api_queue_elem_t list
 *
 * @details
 *
 *    Function : create_and_serialize_list
 *
 *    Functionality: create and serialize_list fapi_api_queue_elem_t 
 *            list
 *
 * @params[in] uint8_t *buffer, p_fapi_api_queue_elem_t headerElem
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
size_t create_and_serialize_list(uint8_t *buffer, p_fapi_api_queue_elem_t headerElem) 
{
   size_t total_size = 0;
   size_t elem_size;

   // Serialize the linked list
   p_fapi_api_queue_elem_t current = headerElem;
   while (current != NULL) {
      elem_size = serialize_queue_elem(current, buffer + total_size);
      if (elem_size == 0) {
         return 0; // Error during serialization
      }
      total_size += elem_size;
      current = current->p_next;
   }

   return total_size;
}

/*******************************************************************
 *
 * @brief send p_fapi_api_queue_elem_t  to fapi decoder
 *
 * @details
 *
 *    Function : sendFapiQueueElemToFapiDecoder
 *
 *    Functionality: send p_fapi_api_queue_elem_t  to fapi decoder
 *
 * @params[in] p_fapi_api_queue_elem_t headerElem
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendFapiQueueElemToFapiDecoder(p_fapi_api_queue_elem_t headerElem) 
{
   int sockfd;
   struct sockaddr_in servaddr;
   size_t buffer_size = 65536; // Define your desired buffer size
   uint8_t *buffer = malloc(buffer_size);

   // Create socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      perror("Socket creation failed");
      exit(EXIT_FAILURE);
   }

   // Setup server address
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
   servaddr.sin_port = htons(PORT);

   // Connect to server
   if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
      perror("Connection failed");
      close(sockfd);
      exit(EXIT_FAILURE);
   }

   // Create and serialize the linked list
   size_t data_size = create_and_serialize_list(buffer, headerElem);
   if (data_size == 0) {
      fprintf(stderr, "Failed to create and serialize list\n");
      close(sockfd);
      exit(EXIT_FAILURE);
   }

   // Send data to server
   if (send(sockfd, buffer, data_size, 0) != data_size) {
      perror("Send failed");
      close(sockfd);
      exit(EXIT_FAILURE);
   }

   printf("Data sent successfully\n");

   // Close socket
   close(sockfd);
   return 0;
}
#endif

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
   }
#else
   p_fapi_api_queue_elem_t nextMsg = NULLP;

#ifdef FAPI_DECODER
   uint8_t msgType=0;
   uint16_t msgSize =0;
   currMsg = (p_fapi_api_queue_elem_t)msg;
   while(currMsg)
   {
      nextMsg = currMsg->p_next;
      msgLen = currMsg->msg_len + sizeof(fapi_api_queue_elem_t);
      if((currMsg->msg_type != FAPI_VENDOR_MSG_HEADER_IND) && (currMsg->msg_type != FAPI_VENDOR_MESSAGE))
      {
          calculateFapiMsgSize(currMsg->msg_type, &msgSize);
      }
      currMsg = nextMsg;
   }
   currMsg = (p_fapi_api_queue_elem_t)msg;
   
   if(msgSize != 0)
   {
      sendFapiQueueElemToFapiDecoder(currMsg);
   }
#endif
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
