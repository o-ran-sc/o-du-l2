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

/* This file is the entry point for UDP P7 */

#include "common_def.h"

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : udpP7ActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during UDP P7
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
uint8_t udpP7ActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   return ROK;
}

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : udpP7ActvTsk 
 * 
 *      Functionality:
 *           Primitives invoked by UDP P7 users/providers through
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
uint8_t udpP7ActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;

   switch(pst->srcEnt)
   {
       case ENTDUAPP:
       {
          if(pst->event == EVENT_NFAPI_P7_UDP_CFG)
          {
             unpackDuNfapiP7UdpCfg(NfapiProcP7UdpCfg, pst, mBuf);
          }
          else
          {
             DU_LOG("\nERROR  -> NFAPI_VNF: UDP Act task received Incorrect event:%d",\
                                                                  pst->event);
          }
          break;
       }
       default:
       {
          DU_LOG("\nERROR  --> NFAPI_VNF: UDP Act task received from wrong Entity:%d",\
                                                                  pst->srcEnt);
          ret = RFAILED;
       }
   }
   
   ODU_EXIT_TASK();
   return ret;
}

/**************************************************************************
 * @brief Handles the NFAPI_UDP_P7 Configuration received from DUAPP 
 *
 * @details
 *
 *      Function : udpP7ActvTsk 
 * 
 *      Functionality: UDP P7 Configuration recevied from DUAPP has to be stored
 *      in Database of NFAPI which will be used during UDP Socket Establishment
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t NfapiProcP7UdpCfg(Pst *pst, NfapiP7UdpCfg *nfapiP7UdpCfg)
{
   uint8_t ret = ROK;
   
   DU_LOG("INFO --> NFAPI_VNF: Received UDP P7 Configuration from DUAPP");
   if(nfapiP7UdpCfg)
   {
      if(nfapiP7UdpCfg->ipv4P7VnfPres)
      {
         vnfDb.p7TransInfo.srcIpv4Address = nfapiP7UdpCfg->ip4P7VnfAddr;
         vnfDb.p7TransInfo.srcIpv4Port = nfapiP7UdpCfg->p7VnfPort;
         vnfDb.p7TransInfo.ipNetAddr = CM_INET_NTOH_UINT32(vnfDb.p7TransInfo.srcIpv4Address):
      }

      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsModificationReq, sizeof(MacStatsModificationReq));
   }
   else
   {
      DU_LOG("\nINFO  -->  MAC : Received NfapiP7UdpCfg is NULL");
      ret =  RFAILED;
   }
   return ret;
}
