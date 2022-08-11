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
#define DU_IP_V4_ADDR (char*[]){"<IP_of_first_DU_connecting_to_CU>", "<IP_of_second_DU_connecting_to_CU>"}
#define DU_SCTP_PORT (int[]){38472, 38473}

#define CU_IP_V4_ADDR "<IP_of_CU>"
#define CU_SCTP_PORT_TO_DU (int[]){38472, 38473}
#endif

#define DU_EGTP_PORT  (int[]){39001, 39002}
#define CU_EGTP_PORT (int[]){39003, 39004}
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
#define MAX_QOS_FLOWS 64 /* As per the spec 38.331, maxNrofQFIs = 64 */

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

#define SEARCH_DU_DB(_duIdx, _duId, _duDb){\
   _duDb = NULLP; \
   for(_duIdx=0; _duIdx < cuCb.numDu; _duIdx++)\
   {\
      if(cuCb.duInfo[_duIdx].duId == _duId)\
      {\
         _duDb =  (&cuCb.duInfo[_duIdx]);\
         break; \
     }\
   }\
}

#define SEARCH_CELL_DB(_cellIdx, _duDb, _nrCellId, _cellCb){\
   _cellCb = NULLP; \
   for(_cellIdx=0; _cellIdx < _duDb->numCells; _cellIdx++)\
   {\
      if(_duDb->cellCb[_cellIdx].nrCellId == _nrCellId)\
      {\
         _cellCb = (&(_duDb->cellCb[_cellIdx]));\
         break;\
      }\
   }\
}

typedef enum
{
   CELL_INACTIVE,
   CELL_ACTIVE,
   CELL_DELETION_IN_PROGRESS
}CellStatusInfo;

typedef enum
{
   UE_IDLE,
   UE_ATTACH_IN_PROGRESS,
   UE_ACTIVE,
   UE_HANDOVER_IN_PROGRESS
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
   CuEgtpParams     egtpParams;
   RrcVersion       rrcVersion;
}CuCfgParams;

typedef struct handoverInfo
{
   uint32_t sourceDuId;
   uint32_t targetDuId;
}HandoverInfo;

typedef struct dlAmCfg
{
   uint8_t        snLenDl;              /* Sequence Number length in bits. Allowed values are 12 and 18 */
   int8_t         reAssemTmr;           /* T_reassembling Timer in msec*/
   int16_t        statProhTmr;          /* T_status_prohibit Timer in msec*/
}DlAmCfg;

typedef struct ulAmCfg
{
   uint8_t      snLenUl;             /* Sequence Number length in bits. Allowed values are 12 and 18 */
   uint16_t     pollRetxTmr;         /* T_poll_retransmit Timer in msec */
   int16_t      pollPdu;             /* Used to trigger a poll for every pollPdu.*/
   int32_t      pollByte;            /* Poll_Byte in bytes. */
   uint8_t      maxRetxTh;           /* Max_Retx_Threshold */
}UlAmCfg;

typedef struct amBearerCfg
{
   UlAmCfg  ulAmCfg;
   DlAmCfg  dlAmCfg;
}AmBearerCfg;

typedef struct ulUmCfg
{
   uint8_t        snLenUlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */
}UlUmCfg;

typedef struct dlUmCfg
{
   uint8_t        snLenDlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */
   int8_t         reAssemTmr;            /* T_reassembling Timer in msec*/
}DlUmCfg;

typedef struct umBiDirBearerCfg
{
   UlUmCfg  ulUmCfg;
   DlUmCfg  dlUmCfg;
}UmBiDirBearerCfg;

typedef struct umUniDirUlBearerCfg
{
   UlUmCfg  ulUmCfg;
}UmUniDirUlBearerCfg;

typedef struct umUniDirDlBearerCfg
{
   DlUmCfg  dlUmCfg;
}UmUniDirDlBearerCfg;

typedef struct rlcLcCfg
{
   uint8_t rlcMode;
   union
   {   
      AmBearerCfg         amCfg;
      UmBiDirBearerCfg    umBiDirCfg;
      UmUniDirUlBearerCfg umUniDirUlCfg;
      UmUniDirDlBearerCfg umUniDirDlCfg;
   }u; 
}RlcLcCfg;

typedef struct macLcCfg
{
   uint8_t priority;
   uint8_t lcGroup;
   uint8_t schReqId;
   uint8_t pbr;        // prioritisedBitRate
   uint8_t bsd;        // bucketSizeDuration
}MacLcCfg;

typedef struct srbInfo
{
   uint8_t   srbId;
   uint8_t   lcId;
   RlcLcCfg  rlcLcCfg;
   MacLcCfg  macLcCfg;
   bool      cfgSentToUe;
}SrbInfo;

typedef struct qosInfo
{
   uint8_t nonDynFiveQI;
   uint16_t avgWindow;
   uint16_t maxBurstDataVol;
   uint8_t priorityLevel;
   uint8_t preemptionCapability;
   uint8_t preemptionVulnerability;
   uint8_t pduSessionId;
}QosInfo;

typedef struct flowsMapped
{
   uint8_t qosFlowId;
   QosInfo qos;
}FlowsMapped;

typedef struct TnlInfo
{
   uint8_t address[4];
   uint8_t teId[4];
}TnlInfo;

typedef struct drbInfo
{
   uint8_t drbId;
   uint8_t lcId;
   QosInfo qos;
   Snssai *snssai;
   uint8_t numFlowMap;
   FlowsMapped flowMapList[MAX_QOS_FLOWS];
   TnlInfo ulUpTnlInfo;
   TnlInfo dlUpTnlInfo;
   uint8_t rlcMode;
   RlcLcCfg  rlcLcCfg;
   MacLcCfg  macLcCfg;
   bool      cfgSentToUe;
}DrbInfo;

typedef struct cuCellCb CuCellCb;

typedef struct cuUeCb
{
   CuCellCb  *cellCb;
   uint32_t  crnti;
   uint8_t   gnbDuUeF1apId;
   uint8_t   gnbCuUeF1apId;
   uint8_t   numSrb;
   SrbInfo   srbList[MAX_NUM_SRB];
   uint8_t   numDrb;
   DrbInfo   drbList[MAX_NUM_DRB];
   F1apMsgDb f1apMsgDb;
   UeState   state;
   HandoverInfo hoInfo;
}CuUeCb;

struct cuCellCb
{
   uint32_t nrCellId;
   uint8_t  numUe;
   CuUeCb   *ueCb[MAX_NUM_UE];
   CellStatusInfo cellStatus;
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
