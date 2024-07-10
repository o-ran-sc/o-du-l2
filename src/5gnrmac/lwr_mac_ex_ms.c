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

/* This file is the entry point for Lower MAC */

#include "common_def.h"
#include <arpa/inet.h>
#include "mac_utils.h"
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#ifdef INTEL_FAPI
#include "nr5g_fapi_internal.h"
#include "fapi_vendor_extension.h"
#endif
#if !(defined(NFAPI_ENABLED) || defined (INTEL_WLS_MEM))
#include "lwr_mac_phy_stub_inf.h"
#endif
#ifdef NFAPI_ENABLED
#include "lwr_mac_sctp_inf.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "nfapi_udp_p7.h"
#endif

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : lwrMacActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during Lower MAC's 
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]  Ent entity, the entity ID of this task.     
 * @param[in]  Inst inst, the instance ID of this task.
 * @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 * @param[in]  Reason reason.
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
uint8_t lwrMacActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   return ROK;
}

#ifdef CALL_FLOW_DEBUG_LOG
/**************************************************************************
* @brief function prints src, dest, msg info about all the msgs received 
*
* @details
*
*     Function : callFlowlwrMacActvTsk 
*
*     Functionality:
*          function prints src, dest, msg info about all the msgs received
*
* @param[in]  Pst *pst
*
* @return void 
***************************************************************************/

void callFlowlwrMacActvTsk(Pst *pst)
{
   char sourceTask[50];
   char destTask[50]="ENTLWRMAC";
   char message[100];

   switch(pst->srcEnt)
   {
      case ENTLWRMAC:
         {
            strcpy(sourceTask,"ENTLWRMAC");
            switch(pst->event)
            {
#ifdef INTEL_WLS_MEM
               case EVT_START_WLS_RCVR:
                  {
                     strcpy(message,"EVT_START_WLS_RCVR");
                     break;
                  }
#endif
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }
            break;
         }

#if !(defined(NFAPI_ENABLED) || defined (INTEL_WLS_MEM))
      case ENTPHYSTUB:
         {
            strcpy(sourceTask,"PHY");
            switch(pst->event)
            {
               case EVT_PHY_STUB_SLOT_IND:
                  {
                     strcpy(message,"EVT_PHY_STUB_SLOT_IND");
                     break;
                  }

               case EVT_PHY_STUB_STOP_IND:
                 {
                    strcpy(message,"EVT_PHY_STUB_STOP_IND");
                    break;
                 }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }
            break;
         }
#endif
     case ENTSCTP:
     {
         strcpy(sourceTask,"SCTP");
         switch(pst->event)
         {
#ifdef NFAPI_ENABLED
             case EVENT_PNF_DATA:
               {
                  strcpy(message,"EVENT_PNF_DATA");
                  break;
               }
#endif
             default:
               {
                  strcpy(message,"Invalid Event");
                  break;
               }
         }
         break;
      }

      default:
         {
            strcpy(sourceTask,"Invalid Source Entity Id");
         }
   }
   DU_LOG("Call Flow: %s -> %s : %s\n", sourceTask, destTask, message);
}
#endif

#ifdef NFAPI_ENABLED
/**************************************************************************
* @brief process param response received from PNF_STUB, fill p7TransInfo 
* parameter and send the param reponse to lwr mac for furthur processing
*
* @details
*
*     Function : sendParamRspToLowerMacFsm 
*
*     Functionality:
*           process param response received from PNF_STUB, fill p7TransInfo
*     parameter and send the param reponse to lwr mac for furthur processing
*
* @param[in]  Param response buffer
* @return ROK     - success
*         RFAILED - failure
***************************************************************************/

uint8_t sendParamRspToLowerMacFsm(Buffer *mBuf)
{
   uint8_t pnfAdd[4];
   uint8_t errorCode = 1,idx=0,numtlv=0;
   uint16_t port=0, tag =0, len=0;
   CmInetIpAddr vnfIp;
   char newIp[INET_ADDRSTRLEN];
   memset(newIp, 0, INET_ADDRSTRLEN);
   
   DU_LOG("INFO  -->  LWR MAC: Extracting information from param resp message");
   //Upacking error code and number of tlv as per : 5G nFAPI Specification,
   //section 3.2.2 PARAM.response, Table 3-14
   CMCHKPK(oduUnpackUInt8, &(errorCode), mBuf);
   if(errorCode != 0)
   {
      DU_LOG("ERROR  -->  LWR MAC: Param rsp errorCode is %d", errorCode);
      return RFAILED;
   }

   CMCHKPK(oduUnpackUInt8, &(numtlv), mBuf);
   
   //unpacking P7 PNF Address Ipv4 tlv mentioned in table 3-15
   CMCHKPK(oduUnpackUInt16, &(tag), mBuf);
   CMCHKPK(oduUnpackUInt16, &(len), mBuf);
   for(idx=0; idx<4; idx++)
   {
      CMCHKPK(oduUnpackUInt8, &(pnfAdd[idx]), mBuf);
   }
   
   //unpacking P7 PNF Port tlv mentioned in table 3-15
   CMCHKPK(oduUnpackUInt16, &(tag), mBuf);
   CMCHKPK(oduUnpackUInt16, &(len), mBuf);
   CMCHKPK(oduUnpackUInt16, &(port), mBuf);
   if (inet_ntop(AF_INET, pnfAdd, newIp, INET_ADDRSTRLEN) == NULL) 
   {
      DU_LOG("ERROR  -->  LWR MAC: failed to convert the pnf ip");
      return RFAILED;
   }
   cmInetAddr((S8*)newIp, &vnfIp);

   //storing the information in vnf db
   vnfDb.p7TransInfo.destIpv4Address = vnfIp;
   vnfDb.p7TransInfo.destIpv4Port = port;
   vnfDb.p7TransInfo.destIpNetAddr.address = CM_INET_NTOH_UINT32(vnfDb.p7TransInfo.destIpv4Address);
   vnfDb.p7TransInfo.destIpNetAddr.port = vnfDb.p7TransInfo.destIpv4Port;
   DU_LOG("INFO  -->  LWR MAC: Sending param response message body buffer to lower mac");
   sendEventToLowerMacFsm(PARAM_RESPONSE, 0, mBuf); 
   return ROK;
}
#endif

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : lwrMacActvTsk 
 * 
 *      Functionality:
 *           Primitives invoked by Lower MAC's users/providers through
 *           a loosely coupled interface arrive here by means of 
 *           SSI's message handling. This API is registered with
 *           SSI during the Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t lwrMacActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;

#ifdef CALL_FLOW_DEBUG_LOG
   callFlowlwrMacActvTsk(pst);
#endif
   
   switch(pst->srcEnt)
   {
      case ENTLWRMAC:
         {
            switch(pst->event)
            {
#ifdef INTEL_WLS_MEM
               case EVT_START_WLS_RCVR:
                  {
                     ODU_PUT_MSG_BUF(mBuf);

                     /* Allocate memory for intial UL transmission */
                     LwrMacEnqueueWlsBlock();

                     /* Start thread to receive from L1 */
                     DU_LOG("INFO  -->  LWR MAC: Starting WLS receiver thread");
                     LwrMacRecvPhyMsg();
                     break;
                  }
#endif
               default:
                  {
                     ODU_PUT_MSG_BUF(mBuf);
                     DU_LOG("ERROR  -->  LWR MAC: Invalid event %d received", pst->event);
                     ret = RFAILED;
                  }
            }
            break;
         }

#if !(defined(NFAPI_ENABLED) || defined (INTEL_WLS_MEM))
      case ENTPHYSTUB:
         {
            switch(pst->event)
            {
               case EVT_PHY_STUB_SLOT_IND:
                  {
                     fapi_slot_ind_t *slotIndMsg;

                     CMCHKUNPK(oduUnpackPointer, (PTR *)&slotIndMsg, mBuf);
                     ODU_PUT_MSG_BUF(mBuf);

                     procPhyMessages(slotIndMsg->header.msg_id, sizeof(fapi_slot_ind_t), (void*)slotIndMsg);
                     MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotIndMsg, sizeof(fapi_slot_ind_t));
                     break;
                  }

               case EVT_PHY_STUB_STOP_IND:
                 {
                    fapi_stop_ind_t *stopIndMsg;
                    CMCHKUNPK(oduUnpackPointer, (PTR *)&stopIndMsg, mBuf);
                    ODU_PUT_MSG_BUF(mBuf);

                    procPhyMessages(stopIndMsg->header.msg_id, sizeof(fapi_stop_ind_t), (void*)stopIndMsg);
                    MAC_FREE_SHRABL_BUF(pst->region, pst->pool, stopIndMsg, sizeof(fapi_stop_ind_t));
                    break;
                 }
               default:
                  {
                     DU_LOG("ERROR  -->  LWR_MAC: Invalid event %d received from PHY STUB", pst->event);
                  }
            }
            break;
         }
#endif
     case ENTSCTP:
       {
          switch(pst->event)
           {
#ifdef NFAPI_ENABLED
             case EVENT_PNF_DATA:
               {
                   uint8_t retVal = RFAILED;

                   nFapi_p5_hdr     p5Hdr;
                   nFapi_msg_header msgHdr;
                   EventState phyEvent;
                   NfapiPnfEvent nfapiPnfEvent;
            
                   nFapiExtractP5Hdr(&p5Hdr, mBuf);
                   nFapiExtractMsgHdr(&msgHdr, mBuf);
                   
                   if(msgHdr.sRU_termination_type != NFAPI_P5_P7_SRU_TYPE)
                   {
                      ODU_PUT_MSG_BUF(mBuf);
                      DU_LOG("ERROR  --> NFAPI_VNF: Incorrect SRU Termination Type:%d",\
                            msgHdr.sRU_termination_type);
                      return RFAILED;
                   }
                   if(convertNfapiP5TagValToMsgId(msgHdr.msg_id, &nfapiPnfEvent, &phyEvent)!=ROK)
                   {
                      ODU_PUT_MSG_BUF(mBuf);
                      DU_LOG("ERROR  --> NFAPI_VNF: Incorrect NFAPI MsgID received:%d",\
                            msgHdr.msg_id);
                      return RFAILED;
                   }
                   if(nfapiPnfEvent!= PNF_MAX_EVENT)
                   {
                      sendEventToNfapiVnfFsm(nfapiPnfEvent, &p5Hdr, &msgHdr, mBuf);
                   }
                   else if(phyEvent != MAX_EVENT)
                   {
                      if(phyEvent == PARAM_RESPONSE)
                      {
                         if(sendParamRspToLowerMacFsm(mBuf) != ROK)
                         {
                            ODU_PUT_MSG_BUF(mBuf);
                            DU_LOG("ERROR  --> NFAPI_VNF: Failed to process param response");
                            return RFAILED;
                         }
                           
                      }
                      else
                      {
                         retVal = sendEventToLowerMacFsm(phyEvent, msgHdr.length, mBuf);
                         if(phyEvent == START_RESPONSE && retVal == ROK)
                         {
                            DU_LOG("DEBUG  --> NFAPI_VNF: Opening UDP Socket");
                            nfapiP7UdpOpenReq(); 
                         }
                      }
                   }
                   ODU_PUT_MSG_BUF(mBuf);
                   break;
               }
#endif
             default:
                {
                    DU_LOG("ERROR  -->  LWR_MAC: Invalid event %d received from SCTP", pst->event);
                }
           }
           break;
       }
      default:
         {
            ODU_PUT_MSG_BUF(mBuf);
            DU_LOG("ERROR  -->  LWR MAC: Message from invalid source entity %d", pst->srcEnt);
            ret = RFAILED;
         }
   }
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
