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

#define MAX_IPV6_LEN 16
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

typedef struct ipAddr
{
 bool ipV4Pres;
 uint32_t  ipV4Addr;
 bool ipV6Pres;
 uint8_t   ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct RicSctpParams
{
   SctpIpAddr  duIpAddr;
   uint16_t         duPort;
   SctpIpAddr  ricIpAddr;
   uint16_t         ricPort;
}RicSctpParams;

typedef struct ricCfgParams
{
   uint32_t             ricId;
   char            ricName[RIC_DU_NAME_LEN_MAX];
   RicSctpParams   sctpParams;
   Plmn            plmn;
}RicCfgParams;
RicCfgParams ricCfgParams; //global variable to hold all configs

void readRicCfg();
void cuAppInmsgHdlr(Buffer *mBuf);
void sctpNtfyInd(CmInetSctpNotification *ntfy);

#endif

/**********************************************************************
         End of file
**********************************************************************/
