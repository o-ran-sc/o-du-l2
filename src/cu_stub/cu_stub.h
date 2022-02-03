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

#define CU_ID 1
#define CU_NAME "ORAN_OAM_CU"

#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define CU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"

#ifndef O1_ENABLE
#define DU_IP_V4_ADDR (char*[]){"192.168.130.81", "192.168.130.83"}
#define DU_SCTP_PORT (int[]){38472, 38473}

#define CU_IP_V4_ADDR "192.168.130.82"
#define CU_SCTP_PORT_TO_DU (int[]){38472, 38473}
#endif

#define DU_EGTP_PORT 39001
#define CU_EGTP_PORT 39002
#define RRC_VER 0
#define EXT_RRC_VER 5
#define PLMN_MCC0 3
#define PLMN_MCC1 1
#define PLMN_MCC2 1
#define PLMN_MNC0 4
#define PLMN_MNC1 8
#define PLMN_MNC2 0

#define CU_DU_NAME_LEN_MAX 30      /* Max length of CU/DU name string */

#define CU_APP_MEM_REG 1
#define CU_POOL 1

#define MAX_NUM_OF_SLICE 1024 /* As per the spec 38.473, maxnoofSliceItems = 1024*/

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

typedef enum
{
   ATTACH_IN_PROGRESS,
   ACTIVE,
   HANDOVER_IN_PROGRESS,
   IDLE
}UeState;

typedef struct RrcVersion
{
  uint8_t    rrcVer;     /* Latest RRC Version */
  uint32_t   extRrcVer;  /* Latest RRC version extended */
}RrcVersion;

typedef struct cuCfgParams
{
   uint32_t         cuId;
   char             cuName[CU_DU_NAME_LEN_MAX];
   CuSctpParams     sctpParams;
   Plmn             plmn;
   EgtpParams       egtpParams;
   RrcVersion       rrcVersion;
}CuCfgParams;

typedef struct cuCellCb CuCellCb;

typedef struct cuUeCb
{
   CuCellCb  *cellCb;
   uint32_t  crnti;
   uint8_t   gnbDuUeF1apId;
   uint8_t   gnbCuUeF1apId;
   F1apMsgDb f1apMsgDb;
   UeState   state;
}CuUeCb;

struct cuCellCb
{
   uint32_t nrCellId;
   uint8_t  numUe;
   CuUeCb   *ueCb[MAX_NUM_UE];
};

typedef struct duDb
{
   uint32_t duId;
   char     duName[CU_DU_NAME_LEN_MAX];
   uint8_t  numCells;
   CuCellCb cellCb[MAX_NUM_CELL];  
   uint8_t  numUe;
   CuUeCb   ueCb[MAX_NUM_CELL * MAX_NUM_UE];
}DuDb;

typedef struct cuGlobalCb
{
   CuCfgParams cuCfgParams;
   uint8_t     numSnssaiSupported;
   Snssai      *snssaiList[MAX_NUM_OF_SLICE];
   uint8_t     numDu;
   DuDb        duInfo[MAX_DU_SUPPORTED];
   uint32_t    gnbCuUeF1apIdGenerator;   /* Generating CU UE F1AP ID */
}CuGlobalCb;

CuGlobalCb cuCb;

void readCuCfg();
void cuAppInmsgHdlr(Buffer *mBuf);
void sctpNtfyInd(CmInetSctpNotification *ntfy);
uint8_t egtpInitReq();
void *cuConsoleHandler(void *);

#endif

/**********************************************************************
         End of file
**********************************************************************/
