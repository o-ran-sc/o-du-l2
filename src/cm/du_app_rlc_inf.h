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
#define EVENT_RLC_UL_UE_CREATE_REQ  210
#define EVENT_RLC_UL_UE_CREATE_RSP 211    /*!< Config Confirm */
#define EVENT_UL_RRC_MSG_TRANS_TO_DU  212
#define EVENT_DL_RRC_MSG_TRANS_TO_RLC 213
#define EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU 214

/* Macro for Ue Context */
#define MAX_NUM_LOGICAL_CHANNELS 11

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
   AM_SIZE_12,
   AM_SIZE_18
}SnLenAm;

typedef enum
{
   UM_SIZE_6,
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
   RE_ASM_0MS,
   RE_ASM_5MS,
   RE_ASM_10MS,
   RE_ASM_15MS,
   RE_ASM_20MS,
   RE_ASM_25MS,
   RE_ASM_30MS,
   RE_ASM_35MS,
   RE_ASM_40MS,
   RE_ASM_45MS,
   RE_ASM_50MS,
   RE_ASM_55MS,
   RE_ASM_60MS,
   RE_ASM_65MS,
   RE_ASM_70MS,
   RE_ASM_75MS,
   RE_ASM_80MS,
   RE_ASM_85MS,
   RE_ASM_90MS,
   RE_ASM_95MS,
   RE_ASM_100MS,
   RE_ASM_105MS,
   RE_ASM_110MS,
   RE_ASM_115MS,
   RE_ASM_120MS,
   RE_ASM_125MS,
   RE_ASM_130MS,
   RE_ASM_135MS,
   RE_ASM_140MS,
   RE_ASM_145MS,
   RE_ASM_150MS,
   RE_ASM_155MS,
   RE_ASM_160MS,
   RE_ASM_165MS,
   RE_ASM_170MS,
   RE_ASM_175MS,
   RE_ASM_180MS,
   RE_ASM_185MS,
   RE_ASM_190MS,
   RE_ASM_195MS,
   RE_ASM_200MS,
   RE_ASM_SPARE1

}TReAssembleTmr;

typedef enum
{
   PROH_0MS,
   PROH_5MS,
   PROH_10MS,
   PROH_15MS,
   PROH_20MS,
   PROH_25MS,
   PROH_30MS,
   PROH_35MS,
   PROH_40MS,
   PROH_45MS,
   PROH_50MS,
   PROH_55MS,
   PROH_60MS,
   PROH_65MS,
   PROH_70MS,
   PROH_75MS,
   PROH_80MS,
   PROH_85MS,
   PROH_90MS,
   PROH_95MS,
   PROH_100MS,
   PROH_105MS,
   PROH_110MS,
   PROH_115MS,
   PROH_120MS,
   PROH_125MS,
   PROH_130MS,
   PROH_135MS,
   PROH_140MS,
   PROH_145MS,
   PROH_150MS,
   PROH_155MS,
   PROH_160MS,
   PROH_165MS,
   PROH_170MS,
   PROH_175MS,
   PROH_180MS,
   PROH_185MS,
   PROH_190MS,
   PROH_195MS,
   PROH_200MS,
   PROH_205MS,
   PROH_210MS,
   PROH_215MS,
   PROH_220MS,
   PROH_225MS,
   PROH_230MS,
   PROH_235MS,
   PROH_240MS,
   PROH_245MS,
   PROH_250MS,
   PROH_300MS,
   PROH_350MS,
   PROH_400MS,
   PROH_450MS,
   PROH_500MS,
   PROH_800MS,
   PROH_1000MS,
   PROH_1200MS,
   PROH_1600MS,
   PROH_2000MS,
   PROH_2400MS,
   PROH_SPARE2,
   PROH_SPARE1

}TStatProhTmr;

typedef enum
{
   RLC_DU_APP_RSP_NOK,
   RLC_DU_APP_RSP_OK
}RlcRsp;

typedef struct ulAmCfg
{
   SnLenAm        snLenUl;              /* Sequence Number length in bits. Allowed values are 12 and 18 */
   TReAssembleTmr reAssemTmr;           /* T_reassembling Timer in msec*/
   TStatProhTmr   statProhTmr;          /* T_status_prohibit Timer in msec*/

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
   TReAssembleTmr reAssemTmr;            /* T_reassembling Timer in msec*/

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
   uint8_t      rbId;
   uint8_t      rbType;
   uint8_t      lcId;
   uint8_t      lcType;
   RlcMode      rlcMode;
   union
   {
      AmBearerCfg         amCfg;
      UmBiDirBearerCfg    umBiDirCfg;
      UmUniDirUlBearerCfg umUniDirUlCfg;
      UmUniDirDlBearerCfg umUniDirDlCfg;
   }u;
}RlcBearerCfg;

typedef struct rlcUeCfg
{
   uint16_t     cellId;
   uint8_t      ueIdx;
   uint8_t      numLcs;
   RlcBearerCfg rlcBearerCfg[MAX_NUM_LOGICAL_CHANNELS];
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

/* RRC delivery message from RLC to DU APP */
typedef struct rrcDeliveryStatusinfo
{
   uint16_t  deliveryStatus;
   uint16_t  triggeringMessage;
}RrcDeliveryStatusInfo;

typedef struct rrcDeliveryReportInfo
{
   uint16_t  cellId;
   uint16_t  ueIdx;
   uint8_t   srbId;
   RrcDeliveryStatusInfo  rrcDeliveryStatus;
}RrcDeliveryReportInfo;

/* Function Pointers */
/* UE create Request from DU APP to RLC*/
typedef uint8_t (*DuRlcUlUeCreateReq) ARGS((
   Pst           *pst,
   RlcUeCfg      *ueCfg ));

/* UE create Response from RLC to DU APP*/
typedef uint8_t (*RlcUlDuUeCreateRsp) ARGS((
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
typedef uint8_t (*RlcRrcDeliveryReportMsgToDuFunc) ARGS((
   Pst           *pst,
   RrcDeliveryReportInfo *rrcDeliveryMsg));

/* Pack/Unpack function declarations */
uint8_t packDuRlcUlUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t unpackRlcUlUeCreateReq(DuRlcUlUeCreateReq func, Pst *pst, Buffer *mBuf);
uint8_t packRlcUlDuUeCreateRsp(Pst *pst, RlcUeCfgRsp *ueCfgRsp);
uint8_t unpackRlcUlUeCreateRsp(RlcUlDuUeCreateRsp func, Pst *pst, Buffer *mBuf);
uint8_t packRlcUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t unpackRlcUlRrcMsgToDu(RlcUlRrcMsgToDuFunc func, Pst *pst, Buffer *mBuf);
uint8_t packDlRrcMsgToRlc(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo);
uint8_t unpackDlRrcMsgToRlc(DuDlRrcMsgToRlcFunc func, Pst *pst, Buffer *mBuf);
uint8_t packRrcDeliveryReportMsgToDu(Pst *pst, RrcDeliveryReportInfo *rrcDeliveryMsg);
uint8_t unpackRrcDeliveryReportMsgToDu(RlcRrcDeliveryReportMsgToDuFunc func,Pst *pst, Buffer *mBuf);

/* Event Handler function declarations */
uint8_t RlcUlProcUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t DuProcRlcUlUeCreateRsp(Pst *pst, RlcUeCfgRsp *cfgRsp);
uint8_t DuProcRlcUlRrcMsgTrans(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t RlcProcDlRrcMsgTransfer(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo);
uint8_t DuProcRlcRrcDeliveryMsgTrans(Pst *pst, RrcDeliveryReportInfo *rrcDeliveryMsgInfo);
#endif /* RLC_INF_H */

/**********************************************************************
         End of file
**********************************************************************/
