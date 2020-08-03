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

#include <stdbool.h>

/* Macro for Ue Context */
#define MAX_NUM_LOGICAL_CHANNELS 11
#define EVENT_RLC_UE_CREATE_REQ      209
#define RB_ID 1

/* RLC MODE */
#define RLC_MODE_AM 0                   //Acknowledged Mode
#define RLC_MODE_UM_BI_DIRECTIONAL 1    //UnAcknowledged Mode
#define RLC_MODE_UM_UNI_DIRECTIONAL_UL 2
#define RLC_MODE_UM_UNI_DIRECTIONAL_DL 3

/* Logical Channel Type */
#define LCH_BCCH      1      /*!< BCCH Logical Channel */
#define LCH_PCCH      2      /*!< PCCH Logical Channel */
#define LCH_CCCH      3      /*!< CCCH Logical Channel */
#define LCH_DTCH      4      /*!< DTCH Logical Channel */
#define LCH_DCCH      5      /*!< DCCH Logical Channel */


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
   MS_5,
   MS_10,
   MS_15,
   MS_20,
   MS_25,
   MS_30,
   MS_35,
   MS_40,
   MS_45,
   MS_50,
   MS_55,
   MS_60,
   MS_65,
   MS_70,
   MS_75,
   MS_80,
   MS_85,
   MS_90,
   MS_95,
   MS_100,
   MS_105,
   MS_110,
   MS_115,
   MS_120,
   MS_125,
   MS_130,
   MS_135,
   MS_140,
   MS_145,
   MS_150,
   MS_155,
   MS_160,
   MS_165,
   MS_170,
   MS_175,
   MS_180,
   MS_185,
   MS_190,
   MS_195,
   MS_200,
   MS_205,
   MS_210,
   MS_215,
   MS_220,
   MS_225,
   MS_230,
   MS_235,
   MS_240,
   MS_245,
   MS_250,
   MS_300,
   MS_350,
   MS_400,
   MS_450,
   MS_500,
   MS_800,
   MS_1000,
   MS_2000,
   MS_4000,
   RETX_SPARE5,
   RETX_SPARE4,
   RETX_SPARE3,
   RETX_SPARE2,
   RETX_SPARE1

}TpollReTxTmr;

typedef enum
{
   P_4,
   P_8,
   P_16,
   P_32,
   P_64,
   P_128,
   P_256,
   P_512,
   P_1024,
   P_2048,
   P_4096,
   P_6144,
   P_8192,
   P_12288,
   P_16384,
   P_20480,
   P_24576,
   P_28672,
   P_32768,
   P_40960,
   P_49152,
   P_57344,
   P_65536,
   P_INFINITY,
   P_SPARE_8,
   P_SPARE_7,
   P_SPARE_6,
   P_SPARE_5,
   P_SPARE_4,
   P_SPARE_3,
   P_SPARE_2,
   P_SPARE_1

}PollPdu;

typedef enum
{
   KB_1,
   KB_2,
   KB_5,
   KB_8,
   KB_10,
   KB_15,
   KB_25,
   KB_50,
   KB_75,
   KB_100,
   KB_125,
   KB_250,
   KB_375,
   KB_500,
   KB_750,
   KB_1000,
   KB_1250,
   KB_1500,
   KB_2000,
   KB_3000,
   KB_4000,
   KB_4500,
   KB_5000,
   KB_5500,
   KB_6000,
   KB_6500,
   KB_7000,
   KB_7500,
   MB_8,
   MB_9,
   MB_10,
   MB_11,
   MB_12,
   MB_13,
   MB_14,
   MB_15,
   MB_16,
   MB_17,
   MB_18,
   MB_20,
   MB_25,
   MB_30,
   MB_40,
   BYTES_INFINITY,
   BYTES_SPARE_20,
   BYTES_SPARE_19,
   BYTES_SPARE_18,
   BYTES_SPARE_17,
   BYTES_SPARE_16,
   BYTES_SPARE_15,
   BYTES_SPARE_14,
   BYTES_SPARE_13,
   BYTES_SPARE_12,
   BYTES_SPARE_11,
   BYTES_SPARE_10,
   BYTES_SPARE_9,
   BYTES_SPARE_8,
   BYTES_SPARE_7,
   BYTES_SPARE_6,
   BYTES_SPARE_5,
   BYTES_SPARE_4,
   BYTES_SPARE_3,
   BYTES_SPARE_2,
   BYTES_SPARE_1

}PollBytes;

typedef enum
{
   TH_1,
   TH_2,
   TH_3,
   TH_4,
   TH_6,
   TH_8,
   TH_16,
   TH_32

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

typedef struct ulAm
{
   SnLenAm        snLenUl;              /* Sequence Number length in bits. Allowed values are 12 and 18 */
   TReAssembleTmr reAssemTmr;           /* T_reassembling Timer in msec*/
   TStatProhTmr   statProhTmr;          /* T_status_prohibit Timer in msec*/

}UlAm;

typedef struct dlAm
{
   SnLenAm        snLenDl;             /* Sequence Number length in bits. Allowed values are 12 and 18 */
   TpollReTxTmr   pollRetxTmr;         /* T_poll_retransmit Timer in msec */
   PollPdu        pollPdu;             /* Used to trigger a poll for every pollPdu.*/
   PollBytes      pollByte;            /* Poll_Byte in bytes. */
   MaxRetxTh      maxRetxTh;           /* Max_Retx_Threshold */
 
}DlAm;

typedef struct dlUm
{
   SnLenUm        snLenDlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */

}DlUm;

typedef struct ulUm
{
   SnLenUm        snLenUlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */
   TReAssembleTmr reAssemTmr;            /* T_reassembling Timer in msec*/

}UlUm;

typedef struct amInfo
{
   UlAm  ulAm;
   DlAm  dlAm;
}AmInfo;

typedef struct umBiDir
{
   UlUm  ulUm;
   DlUm  dlUm;
}UmBiDir;

typedef struct umUniDirUl
{
   UlUm  ulUm;
}UmUniDirUl;

typedef struct umUniDirDl
{
   DlUm  dlUm;
}UmUniDirDl;

/* Spec Ref: 38.331, 6.3.2 RLC-BearerConfig */
typedef struct rlcBearerCfg
{
   uint8_t       lcId;
   uint8_t       rlcMode;
   AmInfo        amInfo;
   UmBiDir       umBiDirInfo;
   UmUniDirUl    umUniDirUlInfo;
   UmUniDirDl    umUniDirDlInfo;
}RlcBearerCfg;

typedef struct rlcUeCfg
{
   uint16_t     cellId;
   uint8_t      ueIdx;
   uint8_t      numLcs;
   RlcBearerCfg rlcBearerCfg[MAX_NUM_LOGICAL_CHANNELS];
}RlcUeCfg;

/* UE create Request from DU APP to RLC*/
typedef uint8_t (*DuRlcUlUeCreateReq) ARGS((
   Pst           *pst,
   RlcUeCfg      *ueCfg ));

extern uint8_t packDuRlcUlUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);
extern uint8_t unpackRlcUlUeCreateReq(DuRlcUlUeCreateReq func, Pst *pst, Buffer *mBuf);
extern uint8_t RlcUlHdlUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);

#endif /* RLC_INF_H */

/**********************************************************************
         End of file
**********************************************************************/
