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
#define CU_NAME "ORAN OAM CU"

#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define CU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"

#ifndef O1_ENABLE
#define LOCAL_IP_CU "192.168.130.82"

#define F1_SCTP_PORT 38472  /* As per the spec 38.472, the registered port number for F1AP is 38472 */
#define NUM_F1_ASSOC 1  
#define REMOTE_IP_DU (char*[]){"192.168.130.81", "192.168.130.83"}

#define XN_SCTP_PORT 38422 /* As per 3GPP TS 38.422, The SCTP Destination Port number value assigned by IANA to be used for XnAP is 38422 */
#define NUM_XN_ASSOC 0
#define REMOTE_IP_CU (char*[]){"192.168.130.84"}
#endif

#define LOCAL_NODE_TYPE SERVER
#define F1_EGTP_PORT 2152 /* As per the spec 29.281, the registered port number for GTP-U is 2152 */
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

#ifdef NR_DRX
/* Set this parameter true of false as per the need to enable or disable drx of
 * a particular UE */
#define DRX_TO_BE_RELEASE true 
#endif

typedef enum
{
  Xn_Based_Inter_CU_HO = 1,
  Inter_DU_HO
}HandoverType;

typedef enum
{
   XN_SETUP_REQ,
   XN_SETUP_RSP,
   HO_REQ,
   HO_REQ_ACK,
   UE_CTXT_REL
}XnEventType;

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
   HandoverType HOType;
   uint32_t srcNodeId;   /* If Inter_DU HO, this is Source DU ID. In case of Inter CU HO, this is Source CU ID */
   uint32_t tgtNodeId;   /* If Inter_DU HO, this is Taregt DU ID. In case of Inter CU HO, this is Target CU ID */
   uint32_t tgtCellId;   /* Cell Id in target node to which UE is to be handed over */
   uint8_t  cuUeF1apIdSrc;  /* Used for Inter-CU HO. CU UE F1AP ID of UE in source CU */
   uint8_t  cuUeF1apIdTgt;  /* Used for Inter-CU HO. CU UE F1AP ID of UE in target CU */
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

#ifdef NR_DRX
/* The following list of structures is taken from the DRX-Config section of specification 33.331. */
typedef struct drxOnDurationTimer
{
   bool  onDurationTimerValInMs;
   union
   {
      uint8_t  subMilliSeconds;
      uint16_t milliSeconds;
   }onDurationtimerValue;
}DrxOnDurationTimer;

typedef struct drxLongCycleStartOffset
{
   uint16_t drxLongCycleStartOffsetChoice;
   uint16_t drxLongCycleStartOffsetVal;
}DrxLongCycleStartOffset;

typedef struct shortDrx
{
   uint16_t   drxShortCycle;
   uint8_t    drxShortCycleTimer;
}ShortDrx;

typedef struct drxCfg
{
   DrxOnDurationTimer       drxOnDurationTimer;
   uint16_t                 drxInactivityTimer;
   uint8_t                  drxHarqRttTimerDl;
   uint8_t                  drxHarqRttTimerUl;
   uint16_t                 drxRetransmissionTimerDl;
   uint16_t                 drxRetransmissionTimerUl;
   DrxLongCycleStartOffset  drxLongCycleStartOffset;
   bool                     shortDrxPres;
   ShortDrx                 shortDrx;
   uint8_t                  drxSlotOffset;
}DrxCfg;
#endif

typedef struct cuUeCb
{
   CuCellCb  *cellCb;
   uint32_t  crnti;
   uint8_t   gnbDuUeF1apId;
   uint8_t   gnbCuUeF1apId;
   uint16_t  pdcpSn;
   uint8_t   numSrb;
   SrbInfo   srbList[MAX_NUM_SRB];
   uint8_t   numDrb;
   DrbInfo   drbList[MAX_NUM_DRB];
   F1apMsgDb f1apMsgDb;
   UeState   state;
#ifdef NR_DRX
   bool      drxCfgPresent;
   DrxCfg    drxCfg;
#endif
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
   CuUeCb   *tempUeCtxtInHo;
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
