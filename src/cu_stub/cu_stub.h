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

#ifndef __CU_MGR_MAIN_H__
#define __CU_MGR_MAIN_H__

#define MAX_IPV6_LEN 16
#define CU_DU_NAME_LEN_MAX 30      /* Max length of CU/DU name string */

#define CU_APP_MEM_REG 1
#define CU_POOL 1
#define MAX_DU_PORT 2
#define DU_PORT 38472
#define MAX_NUM_OF_SLICE 1024 /* As per the spec 38.473, maxnoofSliceItems = 1024*/

/*VALID Tunnel ID*/
#define MIN_TEID 1   /*[Spec 29.281,Sec 5.1]: All Zero TEIDs are never assigned for setting up GTP-U Tunnel*/
#define MAX_TEID MAX_NUM_DRB * MAX_NUM_UE /*[Spec 29.281]: Max limit is not mentioned but as per GTP-U Header Format, TEID occupies 4 octets */
/* allocate and zero out a static buffer */

#define CU_ALLOC(_datPtr, _size)                             \
{                                                            \
   S16 _ret;                                                 \
   _ret = SGetSBuf(CU_APP_MEM_REG, CU_POOL,                  \
                     (Data **)&_datPtr, _size);              \
   if(_ret == ROK)                                           \
      memset(_datPtr, 0, _size);                      \
   else                                                      \
      _datPtr = NULLP;                                       \
}
 
/* free a static buffer */
#define CU_FREE(_datPtr, _size)                              \
   if(_datPtr)                                               \
   SPutSBuf(CU_APP_MEM_REG, CU_POOL,                         \
         (Data *)_datPtr, _size);

typedef struct ipAddr
{
 Bool      ipV4Pres;
 uint32_t  ipV4Addr;
 Bool      ipV6Pres;
 uint8_t   ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct RrcVersion
{
  uint8_t    rrcVer;     /* Latest RRC Version */
  uint32_t   extRrcVer;  /* Latest RRC version extended */
}RrcVersion;

typedef struct egtpParams
{
   SctpIpAddr  localIp;
   uint16_t    localPort;
   SctpIpAddr  destIp;
   uint16_t    destPort;
   uint32_t    currTunnelId;
   uint32_t    minTunnelId;
   uint32_t    maxTunnelId;
}EgtpParams;

typedef struct CuSctpParams
{
   SctpIpAddr  duIpAddr;
   uint16_t    duPort;
   SctpIpAddr  cuIpAddr;
   uint16_t    cuPort;
}CuSctpParams;

typedef struct cuCfgParams
{
   uint32_t         cuId;
   char             cuName[CU_DU_NAME_LEN_MAX];
   CuSctpParams     sctpParams;
   Plmn             plmn;
   EgtpParams       egtpParams;
   RrcVersion       rrcVersion;
   uint8_t          numSnssaiSupported;
   Snssai           *snssaiList[MAX_NUM_OF_SLICE];
}CuCfgParams;
CuCfgParams cuCfgParams; //global variable to hold all configs

void readCuCfg();
void cuAppInmsgHdlr(Buffer *mBuf);
void sctpNtfyInd(CmInetSctpNotification *ntfy);
uint8_t egtpInitReq();
void *cuConsoleHandler(void *);

#endif

/**********************************************************************
         End of file
**********************************************************************/
