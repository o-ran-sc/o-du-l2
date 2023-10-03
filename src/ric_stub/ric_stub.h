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

#ifndef __RIC_MGR_MAIN_H__
#define __RIC_MGR_MAIN_H__

#define RIC_ID 1
#define RIC_NAME "ORAN_OAM_RIC"

#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define RIC_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"

#ifndef O1_ENABLE
#define LOCAL_IP_RIC "192.168.130.80"

#define E2_SCTP_PORT 36421
#define NUM_E2_ASSOC 1 
#define REMOTE_IP_DU (char*[]){"192.168.130.81", "192.168.130.83"}
#endif

#define RRC_VER 0
#define EXT_RRC_VER 5
#define PLMN_MCC0 3
#define PLMN_MCC1 1
#define PLMN_MCC2 1
#define PLMN_MNC0 4
#define PLMN_MNC1 8
#define PLMN_MNC2 0

#define RIC_DU_NAME_LEN_MAX 30      /* Max length of RIC/DU name string */

#define RIC_APP_MEM_REG 1
#define RIC_POOL 1
#define MAX_RAN_FUNCTION 256        /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.2.2 : maxofRANfunctionID */
#define MAX_NUM_TRANSACTION 256     /* As per, O-RAN WG3 E2AP v3.0, section 9.2.33 */

#define MAX_RIC_ACTION  16          /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.1.1 : maxofRICActionID */
#define MAX_RIC_REQUEST 5           /* As per O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.7, max request is 65535. \
                                     * But for our internal testing purpose, keeping it to 5 for now */

/* allocate and zero out a static buffer */
#define RIC_ALLOC(_datPtr, _size)                                \
{                                                               \
   S16 _ret;                                                    \
   _ret = SGetSBuf(RIC_APP_MEM_REG, RIC_POOL,                  \
                     (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      memset(_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}
 
/* free a static buffer */
#define RIC_FREE(_datPtr, _size)                                 \
   SPutSBuf(RIC_APP_MEM_REG, RIC_POOL,                         \
         (Data *)_datPtr, _size);

#define SEARCH_DU_DB(_duIdx, _duId, _duDb){\
   _duDb = NULLP; \
   for(_duIdx=0; _duIdx < ricCb.numDu; _duIdx++)\
   {\
      if(ricCb.duInfo[_duIdx].duId == _duId)\
      {\
         _duDb =  (&ricCb.duInfo[_duIdx]);\
         break; \
     }\
   }\
}

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.26 */
typedef enum
{
   NG,
   XN,
   E1,
   F1,
   W1,
   S1,
   X2
}InterfaceType;

typedef struct
{
   uint16_t requestorId;
   uint16_t instanceId;
}RicRequestId;

typedef struct
{
   int16_t         actionId;
}ActionInfo;

typedef struct ricSubscription
{
   RicRequestId    requestId;
   uint8_t         numOfActions;
   ActionInfo      actionSequence[MAX_RIC_ACTION];
}RicSubscription;

typedef struct
{
   uint16_t  id;
   uint16_t  revisionCounter;
   uint8_t   numOfSubscription;
   RicSubscription subscriptionList[MAX_RIC_REQUEST];
}RanFunction;

typedef struct
{
   InterfaceType   interfaceType;
   uint64_t        componentId;
}E2NodeComponent;

typedef struct duDb
{
   uint32_t        duId;
   uint8_t         ricTransIdCounter;
   uint16_t        numOfRanFunction;
   RanFunction     ranFunction[MAX_RAN_FUNCTION];
   E2NodeComponent e2NodeComponent;
}DuDb;

typedef struct ricCfgParams
{
   uint32_t        ricId;
   char            ricName[RIC_DU_NAME_LEN_MAX];
   RicSctpParams   sctpParams;
   Plmn            plmn;
}RicCfgParams;

typedef struct cuGlobalCb
{
   RicCfgParams ricCfgParams;
   uint8_t      numDu;
   DuDb         duInfo[MAX_DU_SUPPORTED];
}RicGlobalCb;

RicGlobalCb ricCb;

typedef struct
{
   uint8_t numOfRanFunAccepted;
   RanFunction ranFunAcceptedList[MAX_RAN_FUNCTION];
   uint8_t numOfRanFuneRejected;
   RanFunction ranFunRejectedList[MAX_RAN_FUNCTION];
}RicTmpRanFunList;

typedef struct
{
   uint8_t numActionModifed;
   uint8_t actionModifiedList[MAX_RIC_ACTION];
   uint8_t numActionModFailed;
   uint8_t actionModFailedList[MAX_RIC_ACTION];
   uint8_t numActionRemoved;
   uint8_t actionRemovedList[MAX_RIC_ACTION];
}RicTmpActionList;

void readRicCfg();
void cuAppInmsgHdlr(Buffer *mBuf);
void sctpNtfyInd(CmInetSctpNotification *ntfy);

#endif

/**********************************************************************
         End of file
**********************************************************************/
