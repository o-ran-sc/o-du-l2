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

/* Defines APIs exchanged between du_app and cl module of RLC */
#ifndef __RLC_INF_H__
#define __RLC_INF_H__

/* Events */
#define EVENT_RLC_UE_CREATE_REQ  210
#define EVENT_RLC_UE_CREATE_RSP 211    /*!< Config Confirm */
#define EVENT_UL_RRC_MSG_TRANS_TO_DU  212
#define EVENT_DL_RRC_MSG_TRANS_TO_RLC 213
#define EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU 214
#define EVENT_RLC_UE_RECONFIG_REQ  215
#define EVENT_RLC_UE_RECONFIG_RSP  216
#define EVENT_DL_RRC_MSG_RSP_TO_DU 217
#define EVENT_DL_USER_DATA_TRANS_TO_DU 218
#define EVENT_UL_USER_DATA_TRANS_TO_DU 219

#define RB_ID_SRB 0
#define RB_ID_DRB 1

#define RB_TYPE_SRB 0        /* Signalling Radio Bearer */
#define RB_TYPE_DRB 1        /* Data Radio Bearer */

/* RLC Direction Config */
#define RLC_CFG_DIR_UL        1     /*!< Uplink direction */
#define RLC_CFG_DIR_DL        2     /*!< Downlink direction */
#define RLC_CFG_DIR_BOTH      3     /*!< Both Downlink and Uplink Direction*/


/* Logical Channel Type */
#define LCH_BCCH      1      /*!< BCCH Logical Channel */
#define LCH_PCCH      2      /*!< PCCH Logical Channel */
#define LCH_CCCH      3      /*!< CCCH Logical Channel */
#define LCH_DTCH      4      /*!< DTCH Logical Channel */
#define LCH_DCCH      5      /*!< DCCH Logical Channel */

typedef enum
{
   TRANSMISSION_IN_PROGRESS,
   TRANSMISSION_COMPLETE,
   TRANSMISSION_FAILED
}DlMsgState;

typedef enum
{
   RLC_CFG_REAS_NONE            ,  /*!< No Failure */
   RLC_CFG_REAS_RB_CREAT_FAIL   ,  /*!< RB CB creation Failure */
   RLC_CFG_REAS_UE_CREAT_FAIL   ,  /*!< UE CB creation Failure */
   RLC_CFG_REAS_CELL_CREAT_FAIL ,  /*!< CELL CB creation Failure */
   RLC_CFG_REAS_RB_PRSNT        ,  /*!< RB CB already present */
   RLC_CFG_REAS_LCHTYPE_MIS     ,  /*!< Logical channel type mismatch */
   RLC_CFG_REAS_RB_UNKWN        ,  /*!< RB Cb unknown */
   RLC_CFG_REAS_UE_UNKWN        ,  /*!< UE Cb unknown */
   RLC_CFG_REAS_CELL_UNKWN      ,  /*!< Cell Cb unknown */
   RLC_CFG_REAS_RB_DEL_FAIL     ,  /*!< RB Cb Deletion Failure */
   RLC_CFG_REAS_UE_DEL_FAIL     ,  /*!< UE Cb Deletion Failure */
   RLC_CFG_REAS_RB_REEST_FAIL   ,  /*!< RB Cb Re establish Failure */
   RLC_CFG_REAS_RB_MODE_MIS     ,  /*!< RB Cb Mode mismatch */
   RLC_CFG_REAS_REEST_FAIL      ,  /*!< RB Cb Re-establishment Failed */
   RLC_CFG_REAS_RECFG_FAIL      ,  /*!< RB Cb Re-configuration Failed */
   RLC_CFG_REAS_INVALID_CFG     ,  /*!< Invalid Configuration Type */
   RLC_CFG_REAS_SAME_UEID       ,  /*!< UE Ids match in ChngUeId Request */
   RLC_CFG_REAS_CELL_DEL_FAIL   ,  /*!< Cell Cb Deletion Failure */
   RLC_CFG_REAS_INVALID_LCHID   ,  /*!< Invalid LChId */
   RLC_CFG_REAS_INVALID_DIR     ,  /*!< Invalid Direction */
   RLC_CFG_REAS_UE_EXISTS       ,  /*!< UE ID already exists */
   RLC_CFG_REAS_INVALID_SNLEN   ,  /*!< Invalid SN Length */
   RLC_CFG_REAS_SNLEN_MIS       ,  /*!< SN Length mismatch */
   RLC_CFG_REAS_INVALID_RGUSAP     /*!< Invalid RGU SAP ID */
}FailureReason;

typedef enum
{
   RLC_AM,                    //Acknowledged Mode
   RLC_UM_BI_DIRECTIONAL,     //UnAcknowledged Mode
   RLC_UM_UNI_DIRECTIONAL_UL,
   RLC_UM_UNI_DIRECTIONAL_DL
}RlcMode;

typedef enum
{
   AM_SIZE_12 = 1,
   AM_SIZE_18
}SnLenAm;

typedef enum
{
   UM_SIZE_6 = 1,
   UM_SIZE_12
}SnLenUm;

typedef enum
{
   POLL_RETX_TMR_5MS,
   POLL_RETX_TMR_10MS,
   POLL_RETX_TMR_15MS,
   POLL_RETX_TMR_20MS,
   POLL_RETX_TMR_25MS,
   POLL_RETX_TMR_30MS,
   POLL_RETX_TMR_35MS,
   POLL_RETX_TMR_40MS,
   POLL_RETX_TMR_45MS,
   POLL_RETX_TMR_50MS,
   POLL_RETX_TMR_55MS,
   POLL_RETX_TMR_60MS,
   POLL_RETX_TMR_65MS,
   POLL_RETX_TMR_70MS,
   POLL_RETX_TMR_75MS,
   POLL_RETX_TMR_80MS,
   POLL_RETX_TMR_85MS,
   POLL_RETX_TMR_90MS,
   POLL_RETX_TMR_95MS,
   POLL_RETX_TMR_100MS,
   POLL_RETX_TMR_105MS,
   POLL_RETX_TMR_110MS,
   POLL_RETX_TMR_115MS,
   POLL_RETX_TMR_120MS,
   POLL_RETX_TMR_125MS,
   POLL_RETX_TMR_130MS,
   POLL_RETX_TMR_135MS,
   POLL_RETX_TMR_140MS,
   POLL_RETX_TMR_145MS,
   POLL_RETX_TMR_150MS,
   POLL_RETX_TMR_155MS,
   POLL_RETX_TMR_160MS,
   POLL_RETX_TMR_165MS,
   POLL_RETX_TMR_170MS,
   POLL_RETX_TMR_175MS,
   POLL_RETX_TMR_180MS,
   POLL_RETX_TMR_185MS,
   POLL_RETX_TMR_190MS,
   POLL_RETX_TMR_195MS,
   POLL_RETX_TMR_200MS,
   POLL_RETX_TMR_205MS,
   POLL_RETX_TMR_210MS,
   POLL_RETX_TMR_215MS,
   POLL_RETX_TMR_220MS,
   POLL_RETX_TMR_225MS,
   POLL_RETX_TMR_230MS,
   POLL_RETX_TMR_235MS,
   POLL_RETX_TMR_240MS,
   POLL_RETX_TMR_245MS,
   POLL_RETX_TMR_250MS,
   POLL_RETX_TMR_300MS,
   POLL_RETX_TMR_350MS,
   POLL_RETX_TMR_400MS,
   POLL_RETX_TMR_450MS,
   POLL_RETX_TMR_500MS,
   POLL_RETX_TMR_800MS,
   POLL_RETX_TMR_1000MS,
   POLL_RETX_TMR_2000MS,
   POLL_RETX_TMR_4000MS,
   POLL_RETX_TMR_SPARE5,
   POLL_RETX_TMR_SPARE4,
   POLL_RETX_TMR_SPARE3,
   POLL_RETX_TMR_SPARE2,
   POLL_RETX_TMR_SPARE1

}TpollReTxTmr;

typedef enum
{
   POLL_PDU_TMR_4MS,
   POLL_PDU_TMR_8MS,
   POLL_PDU_TMR_16MS,
   POLL_PDU_TMR_32MS,
   POLL_PDU_TMR_64MS,
   POLL_PDU_TMR_128MS,
   POLL_PDU_TMR_256MS,
   POLL_PDU_TMR_512MS,
   POLL_PDU_TMR_1024MS,
   POLL_PDU_TMR_2048MS,
   POLL_PDU_TMR_4096MS,
   POLL_PDU_TMR_6144MS,
   POLL_PDU_TMR_8192MS,
   POLL_PDU_TMR_12288MS,
   POLL_PDU_TMR_16384MS,
   POLL_PDU_TMR_20480MS,
   POLL_PDU_TMR_24576MS,
   POLL_PDU_TMR_28672MS,
   POLL_PDU_TMR_32768MS,
   POLL_PDU_TMR_40960MS,
   POLL_PDU_TMR_49152MS,
   POLL_PDU_TMR_57344MS,
   POLL_PDU_TMR_65536MS,
   POLL_PDU_TMR_INFINITY,
   POLL_PDU_TMR_SPARE_8,
   POLL_PDU_TMR_SPARE_7,
   POLL_PDU_TMR_SPARE_6,
   POLL_PDU_TMR_SPARE_5,
   POLL_PDU_TMR_SPARE_4,
   POLL_PDU_TMR_SPARE_3,
   POLL_PDU_TMR_SPARE_2,
   POLL_PDU_TMR_SPARE_1

}PollPdu;

typedef enum
{
   POLL_BYTES_1KB,
   POLL_BYTES_2KB,
   POLL_BYTES_5KB,
   POLL_BYTES_8KB,
   POLL_BYTES_10KB,
   POLL_BYTES_15KB,
   POLL_BYTES_25KB,
   POLL_BYTES_50KB,
   POLL_BYTES_75KB,
   POLL_BYTES_100KB,
   POLL_BYTES_125KB,
   POLL_BYTES_250KB,
   POLL_BYTES_375KB,
   POLL_BYTES_500KB,
   POLL_BYTES_750KB,
   POLL_BYTES_1000KB,
   POLL_BYTES_1250KB,
   POLL_BYTES_1500KB,
   POLL_BYTES_2000KB,
   POLL_BYTES_3000KB,
   POLL_BYTES_4000KB,
   POLL_BYTES_4500KB,
   POLL_BYTES_5000KB,
   POLL_BYTES_5500KB,
   POLL_BYTES_6000KB,
   POLL_BYTES_6500KB,
   POLL_BYTES_7000KB,
   POLL_BYTES_7500KB,
   POLL_BYTES_8MB,
   POLL_BYTES_9MB,
   POLL_BYTES_10MB,
   POLL_BYTES_11MB,
   POLL_BYTES_12MB,
   POLL_BYTES_13MB,
   POLL_BYTES_14MB,
   POLL_BYTES_15MB,
   POLL_BYTES_16MB,
   POLL_BYTES_17MB,
   POLL_BYTES_18MB,
   POLL_BYTES_20MB,
   POLL_BYTES_25MB,
   POLL_BYTES_30MB,
   POLL_BYTES_40MB,
   POLL_BYTES_INFINITY,
   POLL_BYTES_SPARE_20,
   POLL_BYTES_SPARE_19,
   POLL_BYTES_SPARE_18,
   POLL_BYTES_SPARE_17,
   POLL_BYTES_SPARE_16,
   POLL_BYTES_SPARE_15,
   POLL_BYTES_SPARE_14,
   POLL_BYTES_SPARE_13,
   POLL_BYTES_SPARE_12,
   POLL_BYTES_SPARE_11,
   POLL_BYTES_SPARE_10,
   POLL_BYTES_SPARE_9,
   POLL_BYTES_SPARE_8,
   POLL_BYTES_SPARE_7,
   POLL_BYTES_SPARE_6,
   POLL_BYTES_SPARE_5,
   POLL_BYTES_SPARE_4,
   POLL_BYTES_SPARE_3,
   POLL_BYTES_SPARE_2,
   POLL_BYTES_SPARE_1

}PollBytes;

typedef enum
{
   RETX_TH_1,
   RETX_TH_2,
   RETX_TH_3,
   RETX_TH_4,
   RETX_TH_6,
   RETX_TH_8,
   RETX_TH_16,
   RETX_TH_32

}MaxRetxTh;


typedef enum
{
   RLC_DU_APP_RSP_NOK,
   RLC_DU_APP_RSP_OK
}RlcRsp;

typedef struct ulAmCfg
{
   SnLenAm        snLenUl;              /* Sequence Number length in bits. Allowed values are 12 and 18 */
   int8_t         reAssemTmr;           /* T_reassembling Timer in msec*/
   int16_t        statProhTmr;          /* T_status_prohibit Timer in msec*/

}UlAmCfg;

typedef struct dlAmCfg
{
   SnLenAm        snLenDl;             /* Sequence Number length in bits. Allowed values are 12 and 18 */
   TpollReTxTmr   pollRetxTmr;         /* T_poll_retransmit Timer in msec */
   PollPdu        pollPdu;             /* Used to trigger a poll for every pollPdu.*/
   PollBytes      pollByte;            /* Poll_Byte in bytes. */
   MaxRetxTh      maxRetxTh;           /* Max_Retx_Threshold */
 
}DlAmCfg;

typedef struct dlUmCfg
{
   SnLenUm        snLenDlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */

}DlUmCfg;

typedef struct ulUmCfg
{
   SnLenUm        snLenUlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */
   int8_t         reAssemTmr;            /* T_reassembling Timer in msec*/

}UlUmCfg;

typedef struct amBearerCfg
{
   UlAmCfg  ulAmCfg;
   DlAmCfg  dlAmCfg;
}AmBearerCfg;

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

/* Spec Ref: 38.331, 6.3.2 RLC-BearerConfig */
typedef struct rlcBearerCfg
{
   ConfigType   configType;
   uint8_t      rbId;
   uint8_t      rbType;
   uint8_t      lcId;
   uint8_t      lcType;
   RlcMode      rlcMode;
   union
   {
      AmBearerCfg         *amCfg;
      UmBiDirBearerCfg    *umBiDirCfg;
      UmUniDirUlBearerCfg *umUniDirUlCfg;
      UmUniDirDlBearerCfg *umUniDirDlCfg;
   }u;
}RlcBearerCfg;

typedef struct rlcUeCfg
{
   uint16_t     cellId;
   uint8_t      ueIdx;
   uint8_t      numLcs;
   RlcBearerCfg rlcLcCfg[MAX_NUM_LC];
   UeCfgState rlcUeCfgState; /* InActive / Completed */
}RlcUeCfg;

typedef struct rlcUeCfgRsp
{
   uint16_t       cellId;
   uint16_t       ueIdx;
   RlcRsp         result;
   FailureReason  reason;
}RlcUeCfgRsp;

/* UL RRC Message from RLC to DU APP */
typedef struct ulRrcMsgInfo
{
   uint16_t   cellId;       /* Cell Id */
   uint16_t   ueIdx;        /* UE Index */
   uint8_t    lcId;         /* Logical channel Id */
   uint16_t   msgLen;       /* RRC message length (in bytes) */
   uint8_t    *rrcMsg;      /* RRC Message (UL-DCCH Message) */
}RlcUlRrcMsgInfo;

/* UL User Data from RLC to DU APP */
typedef struct ulUserDatInfo
{
   uint16_t   cellId;       /* Cell Id */
   uint16_t   ueIdx;        /* UE Index */
   uint8_t    rbId;
   uint16_t   msgLen;       /* User data length (in bytes) */
   uint8_t    *userData;    /* User data (UL-DTCH Message) */
}RlcUlUserDatInfo;

/* DL RRC Message from DU APP to RLC */
typedef struct dlRrcMsgInfo
{
   uint16_t   cellId;         /* Cell Id */
   uint16_t   ueIdx;          /* UE index */
   uint8_t    rbType;         /* Radio Bearer Type */
   uint8_t    rbId;           /* Radio Bearer Id */
   uint8_t    lcType;         /* Logical channel type */
   uint8_t    lcId;           /* Logical channel Id */
   bool       execDup;        /* Execution duplication enabled */
   bool       deliveryStaRpt;  /* Reporting of delivery status by RLC to DU-APP enabled */
   uint16_t   msgLen;         /* RRC Message length */
   uint8_t    *rrcMsg;        /* RRC Message (DL-DCCH Message) */
}RlcDlRrcMsgInfo;

/* DL RRC Message Rsp From RLC to DU APP */
typedef struct dlRrcMsgRsp
{
   uint16_t   cellId;         /* Cell Id */
   uint16_t   crnti;          /* UE index */
   DlMsgState state;         /* Dl RRC Msg State */
}RlcDlRrcMsgRsp;

/* RRC delivery message from RLC to DU APP */
typedef struct rrcDeliveryStatus
{
   uint16_t  deliveryStatus;
   uint16_t  triggeringMessage;
}RrcDeliveryStatus;

typedef struct rrcDeliveryReportInfo
{
   uint16_t  cellId;
   uint16_t  ueIdx;
   uint8_t   srbId;
   RrcDeliveryStatus  rrcDeliveryStatus;
}RrcDeliveryReport;

/* Function Pointers */
/* UE create Request from DU APP to RLC*/
typedef uint8_t (*DuRlcUeCreateReq) ARGS((
   Pst           *pst,
   RlcUeCfg      *ueCfg ));

/* UE Cfg Response from RLC to DU APP*/
typedef uint8_t (*RlcDuUeCfgRsp) ARGS((
   Pst          *pst,
   RlcUeCfgRsp  *ueCfgRsp));

/* UL RRC Message from RLC to DU APP */
typedef uint8_t (*RlcUlRrcMsgToDuFunc) ARGS((
   Pst           *pst,
   RlcUlRrcMsgInfo *ulRrcMsgInfo));

/* DL RRC Message from DU APP to RLC */
typedef uint8_t (*DuDlRrcMsgToRlcFunc) ARGS((
   Pst           *pst,
   RlcDlRrcMsgInfo *dlRrcMsgInfo));

/* RRC delivery message from RLC to DU APP */
typedef uint8_t (*RlcRrcDeliveryReportToDuFunc) ARGS((
   Pst           *pst,
   RrcDeliveryReport *rrcDeliveryReport));

typedef uint8_t (*DuRlcUeReconfigReq) ARGS((
   Pst           *pst,
   RlcUeCfg      *ueCfg ));

/* DL RRC Message from RLC to DU APP */
typedef uint8_t (*RlcDlRrcMsgRspToDuFunc) ARGS((
   Pst           *pst,
   RlcDlRrcMsgRsp *dlRrcMsgRsp));

/* UL User data from RLC to DU APP */
typedef uint8_t (*RlcUlUserDataToDuFunc) ARGS((
   Pst           *pst,
   RlcUlUserDatInfo *ulUserData));

/* Pack/Unpack function declarations */
uint8_t packDuRlcUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t unpackRlcUeCreateReq(DuRlcUeCreateReq func, Pst *pst, Buffer *mBuf);
uint8_t packRlcDuUeCfgRsp(Pst *pst, RlcUeCfgRsp *ueCfgRsp);
uint8_t unpackRlcUeCfgRsp(RlcDuUeCfgRsp func, Pst *pst, Buffer *mBuf);
uint8_t packRlcUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t unpackRlcUlRrcMsgToDu(RlcUlRrcMsgToDuFunc func, Pst *pst, Buffer *mBuf);
uint8_t packDlRrcMsgToRlc(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo);
uint8_t unpackDlRrcMsgToRlc(DuDlRrcMsgToRlcFunc func, Pst *pst, Buffer *mBuf);
uint8_t packRrcDeliveryReportToDu(Pst *pst, RrcDeliveryReport *rrcDeliveryReport);
uint8_t unpackRrcDeliveryReportToDu(RlcRrcDeliveryReportToDuFunc func,Pst *pst, Buffer *mBuf);
uint8_t packDuRlcUeReconfigReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t unpackRlcUeReconfigReq(DuRlcUeReconfigReq func, Pst *pst, Buffer *mBuf);
uint8_t packRlcDlRrcMsgRspToDu(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsgRsp);
uint8_t unpackRlcDlRrcMsgRspToDu(RlcDlRrcMsgRspToDuFunc func, Pst *pst, Buffer *mBuf);
uint8_t packRlcUlUserDataToDu(Pst *pst, RlcUlUserDatInfo *ulUserData);
uint8_t unpackRlcUlUserDataToDu(RlcUlUserDataToDuFunc func, Pst *pst, Buffer *mBuf);

/* Event Handler function declarations */
uint8_t RlcProcUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t DuProcRlcUeCfgRsp(Pst *pst, RlcUeCfgRsp *cfgRsp);
uint8_t DuProcRlcUlRrcMsgTrans(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t RlcProcDlRrcMsgTransfer(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo);
uint8_t DuProcRlcRrcDeliveryReport(Pst *pst, RrcDeliveryReport *rrcDeliveryReport);
uint8_t RlcProcUeReconfigReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t DuProcRlcDlRrcMsgRsp(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsg);
uint8_t DuProcRlcUlUserDataTrans(Pst *pst, RlcUlUserDatInfo *ulUserData);

#endif /* RLC_INF_H */

/**********************************************************************
         End of file
**********************************************************************/
