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

/* This file contains all E2AP message handler related functionality */

#define ENCODE_FAIL -1
#define TRANS_ID 1
#define RRC_SIZE 1
#define ENC_BUF_MAX_LEN 100
#define SUL_BAND_COUNT  0
#define UL_SRBID        1
#define DL_SRBID        0
#define DU_ID           1
#define RIC_ID          1
#define CRNTI           17017
#define CELL_INDEX      0

#define RIC_STYLE_TYPE  1

/* if config type is CONFIG_ADD then 
 * for action Id = n, RIC_ACTION_GRANULARITY_PERIOD = 100+ n*100 
 * else config type is CONFIG_MOD then
 * for action Id = n, RIC_ACTION_GRANULARITY_PERIOD =  100 +50(n+1) */ 
#define RIC_ACTION_GRANULARITY_PERIOD(_configType, _actionId)   \
   ((_configType == CONFIG_ADD) ?  (100 + 100 * _actionId ) : ( 100 + 50 *( _actionId +1))) 

/* allocate and zero out a static buffer */
#define RIC_ALLOC(_datPtr, _size)                                \
{                                                               \
   uint8_t _ret;                                                    \
   _ret = SGetSBuf(RIC_APP_MEM_REG, RIC_POOL,                  \
                    (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      memset(_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}

/* free a static buffer */
#define RIC_FREE(_datPtr, _size)                                 \
   if(_datPtr)                                                  \
      SPutSBuf(RIC_APP_MEM_REG, RIC_POOL,                      \
         (Data *)_datPtr, _size);




void E2APMsgHdlr(uint32_t *duId, Buffer *mBuf);
uint8_t BuildAndSendE2SetupRsp(DuDb *duDb, uint8_t transId, E2NodeConfigList e2NodeList);
uint8_t BuildAndSendRicSubscriptionReq(DuDb *duDb);
uint8_t SendE2APMsg(Region region, Pool pool, uint32_t duId);
uint8_t BuildAndSendRicServiceQuery(DuDb *duDb);
uint8_t BuildAndSendE2NodeConfigUpdateFailure(uint32_t duId, uint8_t transId, uint8_t causeInfo, uint8_t causeReason);
uint8_t fillE2NodeConfigAck(PTR e2NodeCfg, uint8_t procedureCode, E2NodeComponent *componentInfo, bool isSuccessful);
E2NodeComponent *fetchE2NodeComponentInfo(DuDb *duDb, InterfaceType interfaceType,CmLList **e2ComponentNode);
uint8_t handleE2NodeComponentAction(DuDb *duDb, PTR e2NodeCfg, uint8_t protocolId, E2NodeConfigItem *storeCfg);
uint8_t BuildAndSendE2NodeConfigUpdateAck(DuDb *duDb, uint8_t transId,  E2NodeConfigList *e2NodeList);
uint8_t BuildAndSendConnectionUpdate(uint32_t duId);
uint8_t BuildAndSendE2ConnectionUpdate(uint32_t duId, E2Connection connectionInfo);
uint8_t BuildAndSendRicSubscriptionDeleteRequest(uint32_t duId, RicSubscription *ricSubsDb);
void BuildRicSubsModificationReq(DuDb *duDb, RicSubscription *ricSubsInfo);

/**********************************************************************
         End of file
**********************************************************************/
