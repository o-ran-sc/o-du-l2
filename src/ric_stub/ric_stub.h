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

typedef struct duDb
{
   uint32_t duId;
   Bool     ricSubscribedToDu;
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

void readRicCfg();
void cuAppInmsgHdlr(Buffer *mBuf);
void sctpNtfyInd(CmInetSctpNotification *ntfy);

#endif

/**********************************************************************
         End of file
**********************************************************************/
