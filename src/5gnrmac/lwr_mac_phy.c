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
#include "lwr_mac_phy.h"
#include "lwr_mac_fsm.h"
#include "mac_utils.h"

#ifdef INTEL_WLS
#include "wls_lib.h"
#endif

EXTERN uint8_t rgClHndlCfgReq ARGS((void *msg));
EXTERN void l1ProcessFapiRequest ARGS((uint8_t msgType, uint32_t msgLen, void *msg));

#ifdef INTEL_WLS

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
   void *wlsHdlr;

   WLS_MEM_ALLOC(memPtr, LWR_MAC_WLS_BUF_SIZE);

   if(memPtr) 
   {
      wlsHdlr = mtGetWlsHdl();

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
 * @brief Enqueue N number of blocks
 *
 * @details
 *
 *    Function : enqueueNBlocks
 *
 *    Functionality:
 *      Enqueue N number of memory blocks
 *
 * @params[in] Number of blocks
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t enqueueNBlocks(uint32_t numBlocks)
{
   void    *memPtr;
   void    *wlsHdlr;       /* WLS handler */

   wlsHdlr = mtGetWlsHdl();   
   while(numBlocks)
   {
      numBlocks--;

      memPtr = (void *)NULL;
      WLS_MEM_ALLOC(memPtr, LWR_MAC_WLS_BUF_SIZE);
      if(memPtr)
      {
	 WLS_EnqueueBlock(wlsHdlr, WLS_VA2PA(wlsHdlr, memPtr));
      }
   }
   return ROK;
}/* enqueueNBlocks */

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
   CmLList         *node;
   WlsBlockToFree  *block;
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
   uint32_t numL1Msg;   /* Number of L1 messaes received */
   uint32_t numToGet;   /* Number of Memory blocks to get */
   void     *wlsHdlr;       /* WLS handler */
   uint64_t l1Msg;         /* Message received */
   void     *l1MsgPtr;
   uint32_t msgSize;
   uint16_t msgType;
   uint16_t flag;

   wlsHdlr = mtGetWlsHdl();
   if(WLS_Ready(wlsHdlr))
   {
      numToGet = WLS_Wait(wlsHdlr);

      numL1Msg = numToGet;

      while(numToGet)
      {
	 l1Msg = (uint64_t) NULL;
	 l1Msg = WLS_Get(wlsHdlr, &msgSize, &msgType, &flag);
	 if(l1Msg)
	 {
	    l1MsgPtr = WLS_PA2VA(wlsHdlr, l1Msg); 
	    handlePhyMessages(msgType, msgSize, l1MsgPtr);
	 }
	 numToGet--;
      }

      if(numL1Msg)
      {
	 enqueueNBlocks(numL1Msg);
      }

   }
} /* LwrMacRecvPhyMsg */

#endif /* INTEL_WLS */

/*******************************************************************
 * 
 *  @brief Sends message to PHY
 * 
 *  @details
 * 
 *    Function : LwrMacSendToPhy
 *    Functionality:
 *         -Sends message to PHY
 * 
 *  @params[in] Message Type
 *              Message Length
 *              Messaga Pointer
 * 
 *  @return void
 * 
 * *****************************************************************/

uint16_t LwrMacSendToPhy(uint8_t msgType, uint32_t msgLen, void *msg)
{
#ifdef INTEL_WLS
   int ret;
   unsigned long long pMsg;

   pMsg = WLS_VA2PA(mtGetWlsHdl(), msg);
   ret = WLS_Put(mtGetWlsHdl(), pMsg, msgLen, msgType, 0);

   if(ret != 0)
   {
      printf("\nFailure in sending message to PHY");
      WLS_MEM_FREE(msg, msgLen);	
      return RFAILED;
   }
   else
   {
      addWlsBlockToFree(msg, msgLen, (slotIndIdx-1));
   }
#else
   l1ProcessFapiRequest(msgType, msgLen, msg);
#endif
   return ROK;
} /* LwrMacSendToPhy */

/**********************************************************************
  End of file
 **********************************************************************/
