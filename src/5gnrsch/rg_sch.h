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

/**********************************************************************
 
    Name:   RRC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE-MAC
 
    File:   rg_sch.h
 
**********************************************************************/
 
#ifndef __RGSCH__
#define __RGSCH__

#define RGSCHLAYERNAME     "MAC Scheduler"
#ifdef RG_5GTF
#define MAX_5GTF_CELL          8
#define MAX_5GTF_GROUP         1 
#define MAX_5GTF_BEAMS         4
#define MAX_5GTF_SUBFRAME_INFO 10
#define MAX_5GTF_VRBG_SIZE     4
#define MAX_5GTF_PRBS          100
#define MAX_5GTF_MCS           15
#define MAX_5GTF_VRBG          25
#define MAX_5GTF_TB_SIZE       66392
/* Total number of UEs scheduled per cell in a subframe */
#define MAX_5GTF_UE_SCH        4
#define MAX_5GTF_DCIA1B1_SIZE  60
#define MAX_5GTF_XPUSCH_RANGE  2
#endif
/* This variables must moved into cell context finally. Since this requires replacing
 *  all macros in the code with these cell specific variables, this will taken as part of 
 *  delta cleanup activity in MAC. Untill then, since we only support one cell, this should be fine
 */
#ifdef RG_SCH_DYNDLDELTA
#define RGSCH_UPDATE_DELTA(_inst, delta)\
{\
rgSchCb[_inst].rgSchDlDelta    = RG_ENV_DL_DELTA + delta;\
rgSchCb[_inst].rgSchCmnDlDelta = RG_ENV_SCH_CMN_DL_DELTA + delta;\
rgSchCb[_inst].rgSchUlDelta    = TFU_ENV_HQFBKIND_ULDELTA + delta;\
}
#else
#define RGSCH_UPDATE_DELTA(_inst, delta)
#endif

#ifdef RG_SCH_DYNDLDELTA
/* 4UE_TTI_DELTA */
#ifdef CA_PHY_BRDCM_61765
#define RG_DL_DELTA rgSchCb[1].rgSchDlDelta
#define RG_SCH_CMN_DL_DELTA (rgSchCb[1].rgSchCmnDlDelta)
#define TFU_HQFBKIND_ULDELTA (rgSchCb[1].rgSchUlDelta)
#else
#define RG_DL_DELTA rgSchCb[0].rgSchDlDelta
#define RG_SCH_CMN_DL_DELTA (rgSchCb[0].rgSchCmnDlDelta)
#define TFU_HQFBKIND_ULDELTA (rgSchCb[0].rgSchUlDelta)
#endif
#else
#ifdef  SPLIT_RLC_DL_TASK
#define RG_DL_DELTA (RG_ENV_DL_DELTA + 1 )
#define RG_SCH_CMN_DL_DELTA (RG_ENV_SCH_CMN_DL_DELTA + 1)
#define TFU_HQFBKIND_ULDELTA (TFU_ENV_HQFBKIND_ULDELTA + 1)
#else 
#define RG_DL_DELTA RG_ENV_DL_DELTA 
#define RG_SCH_CMN_DL_DELTA RG_ENV_SCH_CMN_DL_DELTA 
#define TFU_HQFBKIND_ULDELTA TFU_ENV_HQFBKIND_ULDELTA 
#endif
#endif

/* Tunable parameter */
#define RGSCH_MAX_NUM_DED_LC 10       /* maximum dedicated logical channels in a UE */
#define RGSCH_MAX_NUM_CMN_LC 5        /* maximum number of common logical 
                                         channels in a cell */
#define RG_SCH_LCG0 0
#define RG_SCH_QCI5 5
#define RGSCH_MAX_LCG_PER_UE 4
#define RGSCH_CON_RES_ID_SZ  6
#ifdef LTE_TDD
#define RGSCH_MAX_RA_RNTI   60
#else
#define RGSCH_MAX_RA_RNTI   10
#define RGSCH_RAREQ_ARRAY_SIZE (2*RGSCH_MAX_RA_RNTI)
#endif

#define RGSCH_MAX_RAPID 64 /* XXX: For RACH Module */
/* Below two temps added by nagaraja S */
#define RG_MEAS_GAPPRD_40 40
#define RG_MEAS_GAPPRD_80 80
/* Added support for SPS*/

/* LTEMAC_SPS changes */
#define RG_SCH_NUM_RATYPE1_SUBSETS    4
#define RG_SCH_NUM_RATYPE2_32BIT_MASK 4
#define RG_SCH_NUM_RATYPE1_32BIT_MASK RG_SCH_NUM_RATYPE1_SUBSETS

#define RGSCH_SPS_ULBW_MASK_LEN                    4
#define RGSCH_SPS_MG_MASK_LEN                      10
/* MAX number of feedbacks in case of LTE_TDD */
#ifdef LTEMAC_SPS
#ifdef LTE_TDD
#define RGSCH_TDD_MAX_FDBK 4
#endif /* LTE_TDD */
/* DL SPS States of UE */
#define RGSCH_DL_SPS_IDLE                    0x00
#define RGSCH_DL_SPS_ACTVTN_PENDING          0x01
#define RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM    0x02
#define RGSCH_DL_SPS_ACTV                    0x03
#define RGSCH_DL_SPS_RELEASE_TRIGGERED       0x04
#define RGSCH_DL_SPS_REACTVTN_PENDING        0x05  

/* UL SPS States of UE */
#define RGSCH_SPS_IDLE              0x00
#define RGSCH_SPS_TO_BE_ACTV        0x01
#define RGSCH_SPS_TO_BE_REACTV      0x02
#define RGSCH_SPS_TO_BE_REL         0x03
#define RGSCH_SPS_ACTV_PDCCH_SENT   0x04
#define RGSCH_SPS_REL_SENT          0x05
#define RGSCH_SPS_ACTV              0x06

#define RG_SCH_SPS_LCG_ID 1
#define RG_SCH_MAX_N1PUCCH_VAL 2047
#define RGSCH_INVALID_SUBFRAME 0xFF
#define RGSCH_INVALID_SFN 0xFFFF
#endif

#define RG_SCH_MAX_NUM_N1PUCCH_PER_UE 4
#define RG_SCH_MAX_NUM_N3PUCCH_PER_UE 4
#define RGSCH_MAX_DL_HQ_PROC 8

/* This is the delta that MAC works on Schedule's DELTA should be a function of
 * this.
 */

/* Well known RNTIS */
#define RGSCH_SI_RNTI        0xffff
#define RGSCH_P_RNTI         0xfffe

#define RGSCH_MAX_UPPERSAP               3
#define RGSCH_MAX_LOWERSAP               1 
#define RGSCH_TQ_SIZE                    100   /* Timing Queue Size */ 
#define RGSCH_MAX_TIMER     RGSCH_MAX_LOWERSAP   /* MAX number of MAC timers */
#define RGSCH_NMB_CELL_HASHBIN           10   /* Number of Hash Bins for cell hash
                                              table */
#define RGSCH_NMB_UE_HASHBIN             10   /* Number of Hash Bins for UE hash
                                              table */        
#define RGSCH_BNDREQ_TMR    1    /* Bind Request timer event value */
#define RGSCH_MAX_BNDRETRY  2    /* Max number of Bind Retries for TFU SAP */


#define RGSCH_UE_TQ_SIZE    10   /* Timing Queue Size */

#define RGSCH_INVALID_SCHD_TYPE 255
#define RGSCH_MAX_UE_PER_CELL 0xFFFF /*!< Maximum number of UEs per cell */
#define RGSCH_MAX_UE_BIN_PER_CELL 128 /*!< Maximum number of UE bins per cell */
#define RGSCH_MIN_DL_BW       6    /*!< Minimum Downlink bandwidth in RBs */
#define RGSCH_MAX_DL_BW       110  /*!< Maximum Downlink bandwidth in RBs */
#define RGSCH_MIN_UL_BW       6    /*!< Minimum Uplink bandwidth in RBs */
#define RGSCH_MAX_UL_BW       110  /*!< Maximum Uplink bandwidth in RBs */
#define RGSCH_NUM_SC_IN_RB     12  /*!< Number of sub carriers in a RB */
#define RGSCH_NUM_PBCH_SYM      4  /*!< Number of PBCH symbols in subframe0.Section 6.6,36.211 */
#define RGSCH_NUM_PSS_SSS_SYM   2  /*!< Number of primary and secondary Synchronization symbols in                                        subframe 0 and subframe 5 */
#define RGSCH_NUM_CELL_RS_ONE_ANT_PORT  8   /*!< Number of cell specific reference symbols in a Subframe */
#define RGSCH_NUM_CELL_RS_TWO_ANT_PORT  16  /*!< Number of cell specific reference symbols in a Subframe */
#define RGSCH_NUM_CELL_RS_FOUR_ANT_PORT  24   /*!< Number of cell specific reference symbols in a Subframe */
#define RGSCH_TOT_NUM_SYM_NORCP 168 /*!< Total number of symbols in a RB in case of Normal CP */
#define RGSCH_TOT_NUM_SYM_EXTCP 144 /*!< Total number of symbols in a RB in case of Extended CP */

#define RGSCH_MAX_NUM_CSRS_ONE_SYMBOL_RB  4
#define RGSCH_NUM_ANT_PORT_FOUR 4
#define RGSCH_MIN_NUM_ANT_PORT 1
#define RGSCH_NUM_REGS_1ST_SYM 2
#define RGSCH_NUM_REGS_2ND_SYM_FOUR_ANT_PORT 2
#define RGSCH_NUM_REGS_2ND_SYM_1OR2_ANT_PORT 3
#define RGSCH_NUM_REGS_3RD_SYM 3
#define RGSCH_NUM_REGS_4TH_SYM_NOR_CP 3
#define RGSCH_NUM_REGS_4TH_SYM_EXT_CP 2

#define RGSCH_MIN_CFI_VAL     1    /*!< Minimum value for CFI */
#define RGSCH_MAX_CFI_VAL     3    /*!< Maximum value for CFI */
#define RGSCH_QM_BPSK         2    /*!< Qm value for BPSK */
#define RGSCH_QM_QPSK         4    /*!< Qm value for QPSK */
#define RGSCH_QM_64QAM        6    /*!< Qm value for 64QAM */
#define RGSCH_MIN_SRS_SFCFG_IDX 0  /*!< Minimum value for SRS subframe 
                                     configurtion index */
#define RGSCH_MAX_SRS_SFCFG_IDX 15  /*!< Maximum value for SRS subframe 
                                     configurtion index */
#define RGSCH_MAX_SRS_TX_OFFSET 8    /*!< Maximum number of SRS transmission 
                                    offsets per cell */
#ifdef LTE_TDD
#define RGSCH_MIN_MAC_RNTI      61   /*!< Minimum value of RNTI to be managed by 
                                    MAC */
#else
#define RGSCH_MIN_MAC_RNTI      11   /*!< Minimum value of RNTI to be managed by 
                                    MAC */
#endif

#define RG_SCH_DL_MAX_ITBS 13
#define RG_SCH_UL_MAX_ITBS 13

#define RG_TIME_DIFF(_currTime,_prevTime)                  \
   (_currTime < _prevTime ? ((0xffffffff - _prevTime) + _currTime ): (_currTime - _prevTime))

#define RGSCH_TYPE_SC1    RGR_SCH_TYPE_SC1 
#define RGSCH_TYPE_PFS    RGR_SCH_TYPE_PFS
#define RGSCH_TYPE_RR     RGR_SCH_TYPE_RR
#define RGSCH_TYPE_MAXCI  RGR_SCH_TYPE_MAXCI 
/* HARQ related MACROs */
#ifdef TFU_TDD
#define RGSCH_NUM_DL_HQ_PROC 15
#else
#define RGSCH_NUM_DL_HQ_PROC 8
#endif
#define RGSCH_NUM_UL_HQ_PROC 8
#define RGSCH_MIN_HQ_TX 1

/* Group power related MACROs */
#define RGSCH_MAX_GRP_PWR_FMT3_IDX  15   /*!< Maximum index value for group power format 3 */
#define RGSCH_MAX_GRP_PWR_FMT3A_IDX 31   /*!< Maximum index value for group power format 3A */

/* MACROs to indicate cell specific config for cell to be active */
#define RGSCH_BCCH_BCH_CFG      (1<<0)
#define RGSCH_BCCH_DLSCH_CFG1    (1<<1)
#define RGSCH_BCCH_DLSCH_CFG2    (1<<2)
#define RGSCH_PCCH_CFG          (1<<3)
#define RGSCH_UL_CCCH_CFG       (1<<4)
#define RGSCH_DL_CCCH_CFG       (1<<5)
#define RGSCH_SCHD_CFG          (1<<6)

#define RGSCH_CELL_ACTIVE_CFG (RGSCH_BCCH_BCH_CFG | RGSCH_BCCH_DLSCH_CFG1 | RGSCH_BCCH_DLSCH_CFG2 | RGSCH_PCCH_CFG | RGSCH_UL_CCCH_CFG | RGSCH_DL_CCCH_CFG)

/* Logical channel realated MACROs */
#define RGSCH_INVALID_LCG_ID  255
#define RGSCH_INVALID_LC_ID   255
#define RGSCH_BCCH_BCH_IDX   0
#define RGSCH_BCCH_DLSCH_IDX1 1
#define RGSCH_BCCH_DLSCH_IDX2 2
#define RGSCH_PCCH_IDX 3

/* PUCCH related macros */
#define RGSCH_PUCCH_MAXVAL_CS    7   /*!< Maximum value for cyclic shift of PUCCH */
#define RGSCH_PUCCH_MINVAL_DS    1   /*!< Mininmum value for delta shift of PUCCH */
#define RGSCH_PUCCH_MAXVAL_DS    3   /*!< Maximum value for delta shift of PUCCH */

/* DUX related macros */
#define RGSCH_LCID_MASK 0x1F
#define RGSCH_LCID_LEN 0x5
#define RGSCH_CCCH_LCID 0x00
#define RGSCH_DEDLC_MIN_LCID 0x01
#define RGSCH_DEDLC_MAX_LCID 0x0A
#define RGSCH_RES_MIN_LCID 0x0B
#define RGSCH_RES_MAX_LCID 0x19
#define RGSCH_PHR_LCID 0x1A
#define RGSCH_CRNTI_LCID 0X1B
#define RGSCH_TRUNC_BSR_LCID 0X1C
#define RGSCH_SHORT_BSR_LCID 0X1D
#define RGSCH_LONG_BSR_LCID  0X1E
#define RGSCH_PAD_LCID 0x1F
/* Fix: If only TA is scheduled, use some dummy LCID */
#define RG_TA_LCID 0x20
#define RGSCH_MAX_EXTN_PAD_SUBHDRS 0x02

#define RGSCH_CCCH_SDU_PRSNT     (1<<0)
#define RGSCH_CRNTI_CE_PRSNT     (1<<1)
#define RGSCH_PHR_CE_PRSNT       (1<<2)
#ifndef MAC_5GTF_UPDATE
#define RGSCH_TRUNC_BSR_CE_PRSNT (1<<3)
#define RGSCH_SHORT_BSR_CE_PRSNT (1<<4)
#define RGSCH_LONG_BSR_CE_PRSNT  (1<<5)
#else
#define RGSCH_BSR_CE_PRSNT       (1<<5)
#endif
/* L2_COUNTERS */
#define RGSCH_ACTIVE_LC_PRSNT  (1<<6)
#ifdef LTEMAC_SPS
#define RGSCH_UL_SPS_ACT_PRSENT (1<<7)
#endif
#define RGSCH_EXT_PHR_CE_PRSNT (1<<8)

/* LOGICAL CHANNEL */
#define RGSCH_MAX_LC_PER_UE   10
/* Maximum number of common logical channel control blocks */
#define RGSCH_MAX_CMN_LC_CB   4

/* Random access related MACROs */
#define RGSCH_MAX_RA_PREAMBLE_FMT 3 /*!< Maximun value of Random access preamble 
                                      format */
#define RGSCH_MAX_RA_WINSIZE    10  /*!< Maximum size of Random access response 
                                      window in subframes */
#define RGSCH_MIN_RA_WINSIZE    2   /*!< Minimum size of Random access response 
                                      window in subframes */
#define RGSCH_MIN_NUM_RA_PREAMBLE 4 /*!< Minimum number of Random access 
                                      preambles */
#define RGSCH_MAX_NUM_RA_PREAMBLE 64 /*!< Maximim number of Random access 
                                      preambles */
#define RGSCH_NUM_RA_RB     6
                                      
#define RGSCH_MAX_UL_RB     110     /*!< MAX Uplink RBs */

#define RGSCH_MAX_RA_RSP_ALLOC    4 /*!< Maximum number of Random access
                                      allocations */

#define RGSCH_MAX_RA_RNTI_PER_SUBFRM   6
#define RGSCH_MAX_TDD_RA_RSP_ALLOC     6 
#define RGSCH_MAX_TDD_RA_PREAMBLE_FMT  4
#define RGSCH_MAX_TDD_UL_DL_CFG        7
#define RGSCH_MAX_TDD_SPL_SUBFRM_CFG   9
#define RGSCH_INVALID_INFO             0xff

#define RGSCH_RGR_CFG 1          /* RGR configuration element */

/*CA_SPECIFIC MACROS*/
/*CA_DEV_DS_FIX*/
#define RGSCH_INVALID_CELL_IDX 255
#define RGSCH_PCELL_INDEX 0
#define RG_SCH_CELLINDEX(_cell) (uint8_t)((_cell->cellId >= rgSchCb[_cell->instIdx].genCfg.startCellId) ?\
      (_cell->cellId - rgSchCb[_cell->instIdx].genCfg.startCellId)&(CM_LTE_MAX_CELLS-1): 0)
#define RG_SCH_GET_SCELL_INDEX(_ueCb, _cell) _ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(_cell)]
/* RACHO */
#define RGSCH_MIN_MSG3_GRNT_SZ 7  /*!< Minimum msg3 grant size in bytes */

#define RGSCH_NUM_ITBS         27
#define RGSCH_MAX_NUM_RB       110
/* Max number of layers per CW */
#define RGSCH_MAX_NUM_LYR_PERCW   2

/* New hash define for TA. Refer to 36.213, 4.2.3*/
#define RGSCH_NO_TA_RQD  31  

/* ccpu00133680: Expcted alloc per LC at the end of scheduling
 * 2 bytes for minimum RLC Header+ 1 byte for minimal MAC header
 * + 1 byte for minimal RLC SDU*/
#define RGSCH_MIN_ALLOC_PER_LC 4
#define RGSCH_MAX_REFRESH_GRPSZ   4 /*! Max number of UEs refreshed per subframe */
#define RGSCH_MAX_REFRESH_OFFSET 16 /*! Max number of subframes used for UE refresh */

#define RGSCH_NUM_PDB_SEV_LEVELS 4 /*Number of Severity Levels the PDB can be classified as */
#define RG_SCH_MAX_PDB_TIME      280 /*Maximum PDB time */
#define RGSCH_LCG_ISCFGD(lcg) ((lcg)->lcgId != RGSCH_INVALID_LCG_ID)
/* Corrected the check for dlCcchId */
#define RGSCH_DLCCCH_ISCFGD(cell) ((cell)->dlCcchId != RGSCH_INVALID_LC_ID)
#define RGSCH_ULCCCH_ISCFGD(cell) ((cell)->ulCcchId != RGSCH_INVALID_LC_ID)

#ifdef EMTC_ENABLE
/* Note: In RGSCH_CALC_SF_DIFF, _time1 should be the latest */
#define RGSCH_CALC_SF_DIFF_EMTC(_time1, _time2)\
   (_time1.hSfn*10240+_time1.sfn*10+_time1.subframe) < (_time2.hSfn*10240+_time2.sfn*10+_time2.subframe)?\
     ((_time1.hSfn+RGSCH_MAX_SFN)*10240+_time1.sfn*10+_time1.subframe) -\
       (_time2.hSfn*10240+_time2.sfn*10+_time2.subframe) : \
     (_time1.hSfn*10240+_time1.sfn*10+_time1.subframe) - (_time2.hSfn*10240+_time2.sfn*10+_time2.subframe)\

/*Addef for L2Meas*/
/*LTE_L2_MEAS_PHASE2*/
/*#define RGSCH_CALC_SFN_SF_DIFF(_time1,_sfnCycle, _time2)\
(((_time1.sfn+RGSCH_MAX_SFN * _sfnCycle)*10) + _time1.subframe -\
(_time2.sfn*10 + _time2.subframe))*/

#define RG_SCH_ADD_TO_CRNT_TIME_EMTC(crntTime, toFill, incr)          \
   if ((crntTime.slot + (incr)) >= RGSCH_NUM_SUB_FRAMES)   \
      toFill.sfn = (crntTime.sfn + \
            (crntTime.slot + (incr)) / RGSCH_NUM_SUB_FRAMES); \
   else                                                  \
      toFill.sfn = crntTime.sfn;                              \
   toFill.subframe = (crntTime.slot + (incr)) % RGSCH_NUM_SUB_FRAMES; \
   if (toFill.sfn >= RGSCH_MAX_SFN) \
   { \
      toFill.hSfn = (crntTime.hSfn + 1) % RGSCH_MAX_SFN; \
      toFill.sfn = toFill.sfn % RGSCH_MAX_SFN; \
   }  \
   else  \
   {  \
      toFill.hSfn = crntTime.hSfn; \
   } 

#define RGSCHDECRFRMCRNTTIME_EMTC(_crntDl, _prevDl, decr)    \
do \
{                                                  \
   S32  _subframe;\
   _subframe = _crntDl.hSfn*10240 + _crntDl.sfn * RGSCH_NUM_SUB_FRAMES + _crntDl.subframe; \
   _subframe = _subframe - decr; \
   if(_subframe < 0) \
   { \
      _subframe = (RGSCH_MAX_SFN * RGSCH_MAX_SFN * RGSCH_NUM_SUB_FRAMES) + _subframe; \
   } \
   _prevDl.hSfn = _subframe / (10240); \
   _subframe = _subframe % 10240; \
   _prevDl.sfn = _subframe / RGSCH_NUM_SUB_FRAMES; \
   _prevDl.subframe = _subframe % RGSCH_NUM_SUB_FRAMES; \
} while(0)

/* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper output 
 * if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() as it is 
 * serving the purpose */

#define RGSCHCPYTIMEINFO_EMTC(src, dst)  \
   dst.hSfn        = src.hSfn;     \
   dst.sfn        = src.sfn;     \
   dst.subframe   = src.subframe;

#define RGSCH_SUBFRAME_INDEX(x) ( ( ( ((x).hSfn * RGSCH_MAX_SFN )+ (x).sfn) * RGSCH_NUM_SUB_FRAMES ) + (x).subframe )

#define RGSCH_TIMEINFO_SAME_EMTC(x, y) (((x).sfn == (y).sfn) && ((x).subframe == (y).subframe))

/* Added support for SPS*/
#ifdef LTEMAC_SPS
/*Compares time difference and returns 0 if same, - 1 if x < y, 1 if x > y
  Special check for RGSCH_MAX_SFN is to take care of the wrap around case */
#define RGSCH_TIMEINFO_CMP_EMTC(_x, _y, _ret)\
{\
   if (RGSCH_TIMEINFO_SAME_EMTC(_x, _y))\
   {*_ret = 0;}\
   else if ((_x).hSfn > (_y).hSfn)\
   {if ((((_x).hSfn - (_y).hSfn)) == (RGSCH_MAX_SFN -1)) *_ret = -1; else *_ret = 1; }\
   else if ((_x).hSfn < (_y).hSfn)\
   {if ((((_y).hSfn - (_x).hSfn)) == (RGSCH_MAX_SFN -1)) *_ret = 1; else *_ret = -1; }\
   else if ((_x).sfn > (_y).sfn)\
   {*_ret = 1;}\
   else if ((_x).sfn < (_y).sfn)\
   {*_ret = -1; }\
   else if ((_x).sfn == (_y).sfn)\
   {\
      if ((_x).subframe > (_y).subframe)\
      { *_ret = 1; }\
      else\
      {*_ret = -1; }\
   }\
   else\
   { *_ret = -1; }\
}
#endif

#define RGSCH_INCR_SUB_FRAME_EMTC(x,y) do { \
   if ((x.subframe += y) > 9) {\
      x.sfn += (x.subframe/10); x.subframe = (x.subframe%10);\
      if (x.sfn  >= RGSCH_MAX_SFN) \
      { \
         x.hSfn=(x.hSfn + 1)%RGSCH_MAX_SFN; \
         x.sfn %= RGSCH_MAX_SFN; \
      } \
   }\
}while(0)

#else

#define RGSCH_SUBFRAME_INDEX(x) ( ( ((x).sfn) * RGSCH_NUM_SUB_FRAMES_5G ) + (x).slot )
#endif
/* Note: In RGSCH_CALC_SF_DIFF, _time1 should be the latest */
#define RGSCH_CALC_SF_DIFF(_time1, _time2)\
   (_time1.sfn*RGSCH_NUM_SUB_FRAMES_5G+_time1.slot) < (_time2.sfn*RGSCH_NUM_SUB_FRAMES_5G +_time2.slot)?\
     (_time1.sfn*RGSCH_NUM_SUB_FRAMES_5G+_time1.slot) -\
       (_time2.sfn*RGSCH_NUM_SUB_FRAMES_5G+_time2.slot) : \
     (_time1.sfn*RGSCH_NUM_SUB_FRAMES_5G+_time1.slot) - (_time2.sfn*RGSCH_NUM_SUB_FRAMES_5G +_time2.slot)\

/*Addef for L2Meas*/
/*LTE_L2_MEAS_PHASE2*/
#define RGSCH_CALC_SFN_SF_DIFF(_time1,_sfnCycle, _time2)\
(((_time1.sfn+RGSCH_MAX_SFN * _sfnCycle)*RGSCH_NUM_SUB_FRAMES_5G) + _time1.slot -\
(_time2.sfn*RGSCH_NUM_SUB_FRAMES_5G + _time2.slot))

#define RG_SCH_ADD_TO_CRNT_TIME(crntTime, toFill, incr)          \
   if ((crntTime.slot + (incr)) >= RGSCH_NUM_SUB_FRAMES_5G)   \
      toFill.sfn = (crntTime.sfn + \
            (crntTime.slot + (incr)) / RGSCH_NUM_SUB_FRAMES_5G); \
   else                                                  \
      toFill.sfn = crntTime.sfn;                              \
   toFill.slot = (crntTime.slot + (incr)) % RGSCH_NUM_SUB_FRAMES_5G; \
   if (toFill.sfn >= RGSCH_MAX_SFN) \
   { \
      toFill.sfn = toFill.sfn % RGSCH_MAX_SFN; \
   }  

#define RGSCHDECRFRMCRNTTIME(_crntDl, _prevDl, decr)    \
do \
{                                                  \
   S32  _subframe;\
   _subframe = _crntDl.sfn * RGSCH_NUM_SUB_FRAMES_5G + _crntDl.slot; \
   _subframe = _subframe - decr; \
   if(_subframe < 0) \
   { \
      _subframe = (RGSCH_MAX_SFN * RGSCH_MAX_SFN * RGSCH_NUM_SUB_FRAMES_5G) + _subframe; \
   } \
   _subframe = _subframe % RGSCH_MAX_SUBFRM_5G; \
   _prevDl.sfn = _subframe / RGSCH_NUM_SUB_FRAMES_5G; \
   _prevDl.slot = _subframe % RGSCH_NUM_SUB_FRAMES_5G; \
} while(0)

/* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper output 
 * if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() as it is 
 * serving the purpose */

#define RGSCHCPYTIMEINFO(src, dst)  \
   dst.sfn        = src.sfn;     \
   dst.slot       = src.slot; \


#define RGSCH_TIMEINFO_SAME(x, y) (((x).sfn == (y).sfn) && ((x).slot == (y).slot))

/* Added support for SPS*/
#ifdef LTEMAC_SPS
/*Compares time difference and returns 0 if same, - 1 if x < y, 1 if x > y
  Special check for RGSCH_MAX_SFN is to take care of the wrap around case */
#define RGSCH_TIMEINFO_CMP(_x, _y, _ret)\
{\
   if (RGSCH_TIMEINFO_SAME(_x, _y))\
   {*_ret = 0;}\
   else if ((_x).sfn > (_y).sfn)\
   {*_ret = 1;}\
   else if ((_x).sfn < (_y).sfn)\
   {*_ret = -1; }\
   else if ((_x).sfn == (_y).sfn)\
   {\
      if ((_x).slot > (_y).slot)\
      { *_ret = 1; }\
      else\
      {*_ret = -1; }\
   }\
   else\
   { *_ret = -1; }\
}

#define RGSCH_INCR_SUB_FRAME(x,y) do { \
   if ((x.slot += y) > (RGSCH_NUM_SUB_FRAMES_5G - 1)) {\
      x.sfn += (x.slot/RGSCH_NUM_SUB_FRAMES_5G); x.slot = (x.slot%RGSCH_NUM_SUB_FRAMES_5G);\
      if (x.sfn  >= RGSCH_MAX_SFN) \
      { \
         x.sfn %= RGSCH_MAX_SFN; \
      } \
   }\
}while(0)



#endif /* EMTC_ENABLE */

/* RACHO : TRUE if rapId is a ded preamble */
#define RGSCH_IS_DEDPRM(cell, rapId) ((rapId) >= (cell->rachCfg.numRaPreamble))

#define rgSchPBuf(inst)  rgSchCb[inst].rgSchInit.prntBuf

/* Debug Prints for MAC */
#ifdef DEBUGP
#define RGSCHDBGERRNEW(inst, _args)          \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_ERR, _args)
#define RGSCHDBGINFONEW(inst, _args)         \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_INFO, _args)
#define RGSCHDBGPRM(inst, _args)   UNUSED(inst);
#define RGSCHDBGERR(inst, _args)  \
                  DBGP(&rgSchCb[inst].rgSchInit, RGSCHLAYERNAME, DBGMASK_ERR, _args)
#define RGSCHDBGINFO(inst, _args)  UNUSED(inst); 
#else
#define RGSCHDBGERRNEW(inst, _args) 
#define RGSCHDBGINFONEW(inst, _args)
#define RGSCHDBGPRM(inst, _args)
#define RGSCHDBGERR(inst, _args)
#define RGSCHDBGINFO(inst, _args)
#endif  /* #ifdef DEBUGP */

#define RGSCH_IS_GBR_BEARER(cfgdGbr) ((cfgdGbr) != 0)

#ifdef ERRCLS_KW
#define RGSCH_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)     \
   if((_idxVal) >= (sizeof(_array)/sizeof(_array[0]))) \
   {                                               \
      RGSCHDBGERRNEW((_inst), (rgSchPBuf(_inst), "Array Bound Check Failed"));\
      SExit();\
   }\

#define RGSCH_NULL_CHECK(_inst, _ptr )     \
   if((_ptr) == NULLP)  \
   {                                               \
      RGSCHDBGERRNEW((_inst), (rgSchPBuf(_inst), "Null Pointer detected"));\
      SExit();\
   }
#define RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(_inst, _array, _idxVal)     \
   if(((_idxVal) >= (sizeof(_array)/sizeof(_array[0])))|| (_idxVal < 0)) \
   {                                               \
      RGSCHDBGERRNEW((_inst), (rgSchPBuf(_inst), "Array Bound Check Failed"));\
      SExit();\
   }
   
#define RGSCH_PFS_AMBR_ARRAY_BOUND_CHECK(_inst, _pfsCell, _qId, _ue ) \
{\
   if (_ue->csgMmbrSta == TRUE)\
   {\
      RGSCH_ARRAY_BOUND_CHECK(_inst, _pfsCell->txQueues.prioAmbrLst, _qId);\
   }\
   else\
   {\
      RGSCH_ARRAY_BOUND_CHECK(_inst, _pfsCell->txQueues.normAmbrLst, _qId);\
   }\
}
#else
#define RGSCH_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)  
#define RGSCH_NULL_CHECK( _inst, _ptr )     
#define RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(_inst, _array, _idxVal)
#define RGSCH_PFS_AMBR_ARRAY_BOUND_CHECK(_inst, _pfsCell, _qId, _ue )
#endif

/* Macro to free the message buffer and initialize it to zero */
/***********************************************************
 *
 *     Name : RGSCH_FREE_MSG
 *
 *     Desc : Macro to free the message buffer and initialize it to zero
 *            
 *     Input  : mBuf - message buffer pointer to be retunrned
 *
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/
#define RGSCH_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      ODU_PUT_MSG_BUF((_buf)); \
      _buf = NULLP; \
   } \
}

#define RGSCH_FREE_MEM(_mem)\
{\
   if (NULLP != (_mem)) \
   { \
      cmFreeMem((Ptr)(_mem)); \
      _mem = NULLP; \
   } \
}
/***********************************************************
 *
 *     Name : RGSCH_DROP_RGUDDATREQ_MBUF
 *
 *     Desc : Macro to free the message buffers and initialize them to zero
 *            
 *     Input  : _datreq - Dedicated Data Request pointer which has mBufs
 *              to be freed
 *     
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/

#define RGSCH_DROP_RGUDDATREQ_MBUF(_datReq)\
{\
   uint32_t idx1,idx2,idx;\
   if (_datReq != NULLP)\
   {\
      for (idx=0; idx < _datReq->numLch; idx++)\
      {\
         for (idx1=0; idx1 < RGU_MAX_PDUSET; idx1++)\
         {\
            for (idx2=0; idx2 < _datReq->lchData[idx].pdu[idx1].numPdu; idx2++)\
            {\
               RGSCH_FREE_MSG(_datReq->lchData[idx].pdu[idx1].mBuf[idx2]);\
            }\
         }\
      }\
   }\
}
/***********************************************************
 *
 *     Name : RGSCH_DROP_RGUCDATREQ_MBUF
 *
 *     Desc : Macro to free the message buffers and initialize them to zero
 *            
 *     Input  : _datreq - Common Data Request pointer which has mBufs
 *              to be freed
 *     
 *     Output : None.
 *
 *     Notes: None
 *
 **********************************************************/
#define RGSCH_DROP_RGUCDATREQ_MBUF(_datReq)\
{\
   uint32_t idx1;\
   if (_datReq != NULLP)\
   {\
      for (idx1 = 0; idx1 < RGU_MAX_PDUSET; idx1++)\
      {\
         if (_datReq->pdu != NULLP)\
         {\
            RGSCH_FREE_MSG(_datReq->pdu[idx1]);\
         }\
      }\
   }\
}


/* Macros for memory region and pool determination */
#define RGSCH_GET_MEM_REGION(rgCb)  (rgCb.rgInit.region)
#define RGSCH_GET_MEM_POOL(rgCb)    (rgCb.rgInit.pool)

#ifdef LTE_TDD
#define RGSCH_UPD_HQAN_FDBKTIME(_tbInfo, _dlSf, _timingInfo) \
do \
{  \
   (_tbInfo)->m = 0;   \
   (_tbInfo)->fdbkTime.sfn = (_timingInfo.sfn + \
         _dlSf->dlFdbkInfo.sfnOffset) % RGSCH_MAX_SFN; \
   (_tbInfo)->fdbkTime.slot = _dlSf->dlFdbkInfo.slot; \
   (_tbInfo)->timingInfo = _timingInfo; \
} while(0) 

#define RGSCH_UPD_ANINFO_WITH_HQ(_anInfo, _tbInfo) \
do \
{  \
   _anInfo->sfn = (_tbInfo)->fdbkTime.sfn; \
   _anInfo->slot = (_tbInfo)->fdbkTime.slot; \
   _anInfo->latestMIdx = (_tbInfo)->m; \
} while(0) 
/* Support for iPhich=1 for TDD*/

#define RGSCH_UPD_PHICH(_ulDlCfgIdx, _subframe, _hqProc) \
   if(_ulDlCfgIdx == 0 && ((_subframe == 4) ||( _subframe == 9)))\
   {\
      _hqProc->iPhich = 1;\
   }\
   else\
   {\
      _hqProc->iPhich = 0;\
   }
#endif


/* MUX related macros */
#define RGSCH_HDR_TYPE_CRES                1
#define RGSCH_HDR_TYPE_TA                  2

#define RGSCH_SDU_SHDR_LEN 1
#define RGSCH_CE_SHDR_LEN 1
#define RGSCH_CRES_LEN    6
#define RGSCH_TA_LEN      1
#define RGSCH_CRES_ELM_LEN RGSCH_CE_SHDR_LEN+RGSCH_CRES_LEN
#define RGSCH_TA_ELM_LEN RGSCH_CE_SHDR_LEN+RGSCH_TA_LEN

#define RGSCH_CRES_LCID_IDX               0x1C
#define RGSCH_TA_LCID_IDX                 0x1D

#define RGSCH_MAX_SUBFRAMES_IN_SFN        9
#define RGSCH_MAX_SFN                     1024
#define RGSCH_NUM_SUB_FRAMES              10

#define RGSCH_NUM_SUB_FRAMES_5G           50

#ifdef LTE_TDD
#define RGSCH_ULCTRL_RECP_DIST            7
#else
#define RGSCH_ULCTRL_RECP_DIST            4
#endif
/* Definig the Macro for the Size SF alloc Info Structure
 * The Value is defines the number of subframes the Alloc Info is maintained
 * The SF alloc Info is used to send the scheduled Allocation Info 
 * to MAC from Scheduler
 */
#define RGSCH_SF_ALLOC_SIZE                4

/* Defining new MACRO for DL subframes */
#define RGSCH_NUM_DL_slotS 20
/* Define for the block size for memory allocation */
#define RGSCH_BLKSZ                       2048

/* Defines for RGU Statistics types */
#define RGSCH_RGU_SDU_DROP 1
#define RGSCH_RGU_SDU_RCVD 2

/* MACROS for General Statistics */
#define RGSCH_CFG_ADD      1
#define RGSCH_CFG_DEL      2

#define RGSCH_HQ_FDB_IND_CB_TYPE_HQ_ENT      1
#define RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB       2

/* The value of RGSCH_TDD_MAX_P_VAL is 4, since we need the table for 
 * P+1 value the below Macro is defined and used 
 */
#define RGSCH_TDD_MAX_P_PLUS_ONE_VAL      5
#define RGSCH_TDD_MAX_FREQ_RSRC  6

/* 
 * RG_TFU_HQ_IND_DELTA is replaced with
 * TFU_HQFBKIND_ULDELTA and moved into envopt.h.
 */

/* Macro for selecting the subframe index to be deleted */

#define RGSCH_RLS_SF_IDX   (RG_SCH_CMN_HARQ_INTERVAL + TFU_HQFBKIND_ULDELTA)

/* Macro for max random access responses */
#define RGSCH_MAX_RA_RSP_ALLOC   4

/* Value used to set nDmrs in uplink grant if nDmrs is not applicable */
#define RGSCH_INVALID_NDMRS  10

/* comcodsepa : start */
#define RGSCH_MIN(x, y) ((x) <= (y) ? (x) : (y))
#define RGSCH_MAX(x, y) ((x) >= (y) ? (x) : (y))
#define RGSCH_CEIL(x, y) (((x) + (y)-1) / (y))
#define RGSCH_FLOOR(x, y) ((x) / (y)) 
#define RGSCH_CEILING(x) (((x) - (uint8_t)(x)) ? (uint8_t)(x+1) : (uint8_t)x)  
#define RGSCH_DIV_ROUND(x, y) (((x) + (y)/2) / (y))

#define RGSCH_RARSP_WAIT_PERIOD 3 
#define RGSCH_INCR_FRAME(x) x = ((x) + 1) & 0x03ff

#define RG_SCH_TDD_DL_SUBFRAME     1
#define RG_SCH_TDD_UL_SUBFRAME     2
#define RG_SCH_TDD_SPL_SUBFRAME    3

#define RG_SCH_MAX_DAI_IDX         4
#define RG_SCH_DAI_MAX_BIT_VAL     3
/* Since DAI bit value is stored and its range is 0-3 in SCH, 
 * So Invalid is Set to 0xFF */
/* ccpu00139414 */
#define RG_SCH_INVALID_DAI_VAL     0x00
/* Valid Range is 0-8, So Invalid is Set to 99 */
#define RG_SCH_INVALID_M_VAL       0xFF

/* Below macro used to get the DAI value (1 -4) for the total DL transmission
 * made
 */
#define RG_SCH_GET_DAI_VALUE(dai) ((((dai) - 1) & RG_SCH_DAI_MAX_BIT_VAL) + 1)
/* below Macro indicates the max number of DL transmissions that can be made in
 * a single FeedBak period
 */
#define RG_SCH_MAX_NUM_EXPECTED_ACKS      9
/* The below macro indicates the max number of feedback report 
 */
#define RG_SCH_NUM_FDBK_VALUE             3

#define RG_SCH_ULIDX_LSB           1
#define RG_SCH_ULIDX_MSB           2
#define RG_SCH_ULIDX_BOTH          3

#ifdef MAC_5GTF_UPDATE
#define RGSCH_ONE_RAR_SIZE          8
#else
#define RGSCH_ONE_RAR_SIZE          7
#endif

#define RGSCH_MSG4_HDRSIZE          1
#define RGSCH_CONT_RESID_SIZE       7
/* CR timer change*/
#ifdef RGR_V1
#define RGSCH_CCCH_SDU_HDRSIZE      1
#define RGSCH_CONTRES_EXP      0xffff
#endif
/* Corrected allocation for common channels */
#define RGSCH_ONE_BIHDR_SIZE        1                                             

#define RGSCH_TA_SIZE               2
#ifdef LTE_ADV
#define RGSCH_SCELL_ACT_CE_SIZE     2
#define RGSCH_INVALID_PUCCH3_RES    550
#endif

#define RGSCH_GET_RAR_BYTES(x)   ((x) * RGSCH_ONE_RAR_SIZE)

#define RG_SCH_CMN_MEAS_GAPPRD40 40
#define RG_SCH_CMN_MEAS_GAPPRD80 80
#define RG_MEAS_GAPPRD_40           40
#define RG_MEAS_GAPPRD_80           80
#define RG_MAX_NUM_DLSF             10 /* Maximum number of Subframes */
#define RG_MEASGAP_INACTIVE         0x01
#define RG_ACKNAKREP_INACTIVE       0x02
#define RG_PDCCHODR_INACTIVE        0x04
#define RG_DRX_INACTIVE             0x08    /*UE is DRX inactive */

#define RG_SCH_DRX_UL               0      /*UE is DRX active in uplink*/
#define RG_SCH_DRX_DL               1     /*UE is DRX active in downlink*/
/*Fix:Inform UE delete to scheduler*/
#define RG_MACUEDEL_INACTIVE        0x10  
/* Fix : syed set UE inactive for scheduling if it is not completely 
 * initialized */
#define RG_HQENT_INACTIVE           0x20
/* Timer events */
#define RG_SCH_TMR_ACKNACK_REP      1  /* Timer event for ACK NACK Rep */
#define RG_SCH_TMR_MEASGAP          2  /* Timer event for Measurement gap */
#define RG_SCH_TMR_UL_ACKNACK       3  /* Timer event for Ul Inactivity due to ack-nack */
#define RG_SCH_TMR_DL_ACKNACK       4  /* Timer event for Dl Inactivity due to ack-nack */
#define RG_SCH_TMR_UL_MEASGAP       5  /* Timer event for Ul Inactivity due to Measurement gap */
#define RG_SCH_TMR_DL_MEASGAP       6  /* Timer event for Dl Inactivity due to Measurement gap */
#define RG_SCH_TMR_TA               7  /* Timer event for Timing Alignment */
/* Added periodic BSR timer */
#ifndef RGR_V1
#define RG_SCH_TMR_BSR              8  /* Timer event for Periodic BSR */
#else
#define RG_SCH_TMR_BSR              8  /* Timer event for Periodic BSR */
#endif

#define RG_SCH_TMR_TXMODE_TRNSTN    9  /* Timer event for TX Mode Transition */
#ifdef LTE_ADV
#define RG_SCH_TMR_SCELL_DEACT      10  /* Timer event for Secondary Cell Deactivation */
#define RG_SCH_TMR_SCELL_ACT_DELAY  11  /* Timer event for Secondary Cell Activation Delay */
#endif

#define RG_SCH_TXMODE_TRANS_TIMER   1000  /* Timer length for Tx Mode Transition
                                             Completion */
                                                         
#define RG_SCH_TMR_DLINACTV         2  /* Timer event for DL Inactivity */
#define RG_SCH_MEAS_GAP_LEN         6  /* Timer event for measurement GAP */

#define RGSCH_SCELL_DEACT_TMR_INFINITY_VAL 3840 /* In millisecond, thrice the maximum 
                                                   value of deactivation timer */                                                         

#define RG_SCH_DRX_DL_DELTA     (RG_DL_DELTA) /*!< look at timer queue 
                                                           for DL
                                                           RG_SCH_DRX_DL_DELTA 
                                                           in advance */

/* Introduced UL control timing delta in FDD */
#define RG_SCH_DRX_UL_DELTA     (TFU_ULCNTRL_DLDELTA)
/*Assigning RG_SCH_DRX_MAX_DELTA to the higher delta of uplink delta and
 * downlink delta.*/
#define RG_SCH_DRX_MAX_DELTA ((RG_SCH_DRX_DL_DELTA < RG_SCH_DRX_UL_DELTA)?\
RG_SCH_DRX_UL_DELTA:RG_SCH_DRX_DL_DELTA)

#define RG_SCH_NO_DELTA 0  /*Consider no delta */
/** @brief The lenght of array we maintain to track DRX Cycles.
 * @details
 * The periodicity of LONG DRX Cycle has a maximum value of 2560. The ideal and
 * the fastest way would be to have an array of this size. However having an
 * Array this large would make the cellCb a huge structure and may have
 * performance implications as well. 
 * Hence the defined size and lower values are a multiple of the larger sizes. 
 * A distance based approach is used to get to the correct value. 
 * @sa RgSchDRXCellCb.
 * For decreased processing hit this value maybe increased to 
 * [320, 640, 1024, 2560]
 */
#define RG_SCH_MAX_DRXQ_SIZE 256        /*!< Length of the DRQ queue maintained 
                                       at the cell level. */
#define RG_SCH_MIN_HARQ_RTT       8      /*!< Minimum round trip time for Harq
                                              feedback*/
#ifdef EMTC_ENABLE
#define RG_SCH_MIN_UL_HARQ_RTT       4      /*!< Minimum round trip time for UlHarq feedback */
#endif

/* MASKs for tracking DRX activity */

#define DRX_UE_INACTIVE          0xFFFFFFFF
#define DRX_INVALID              0xFFFF
#define DRX_TMR_EXPRD            -1 

#define RG_SCH_DRX_SR_BITMASK        0x00000001  /*DRX SR Bitmask*/
#define RG_SCH_DRX_RA_BITMASK        0x00000002  /*DRX RA Bitmask*/
#define RG_SCH_DRX_ONDUR_BITMASK     0x00000004  /*DRX on-duration bitmask */
#define RG_SCH_DRX_INACTVTMR_BITMASK 0x00000008  /*DRX inactive timer bitmask */
/* DLHQ Bitmask should always be the last */
#define RG_SCH_DRX_DLHQ_BITMASK      0x00000010  /*DRX DL harq bitmask*/

#ifdef EMTC_ENABLE
#define RG_SCH_DRX_ULHQ_BITMASK    (RG_SCH_DRX_DLHQ_BITMASK  << 8)
#endif

#ifdef LTE_ADV
#define RG_SCH_MAX_SCELL             (CM_LTE_MAX_CELLS - 1) /*!< Max number of sec cells per ue. 
                                                              -1 done for Primary cell */
#define RG_SCH_ACTIVATION_COUNT      10 /*!< Maximum count for SCELL Activation */
#endif
                                                   

/*if any bit in the mask is 0, ue is active */
#define RG_SCH_DRX_DL_IS_UE_ACTIVE(drxCb)\
   (drxCb->drxDlInactvMask ^ DRX_UE_INACTIVE) 

/*if any bit in the mask is 0, ue is active */
#define RG_SCH_DRX_UL_IS_UE_ACTIVE(drxCb)\
   (drxCb->drxUlInactvMask ^ DRX_UE_INACTIVE)     
/*if UE is in DL DRX on-duration */                                                     
#define RG_SCH_DRX_DL_IS_UE_ONDUR_INACTIVE(drxCb) \
   (drxCb->drxDlInactvMask & RG_SCH_DRX_ONDUR_BITMASK)
/*if UE is in UL DRX onduration */
#define RG_SCH_DRX_UL_IS_UE_ONDUR_INACTIVE(drxCb) \
   (drxCb->drxUlInactvMask & RG_SCH_DRX_ONDUR_BITMASK)
/*if ue is in DL DRX Inactive timer period */
#define RG_SCH_DRX_DL_IS_UE_INACTVTMR_INACTIVE(drxCb) \
   (drxCb->drxDlInactvMask & RG_SCH_DRX_INACTVTMR_BITMASK)

/*if ue is in UL DRX Inactive timer period */
#define RG_SCH_DRX_UL_IS_UE_INACTVTMR_INACTIVE(drxCb) \
   (drxCb->drxUlInactvMask & RG_SCH_DRX_INACTVTMR_BITMASK)
/*get DRX cell */
#define RG_SCH_DRX_GET_CELL(_cell) ((_cell)->drxCb) 
/*get DRX UE */
#define RG_SCH_DRX_GET_UE(_ue) ((_ue)->drxCb) 
/*get DRX DL HARQ */
#define RG_SCH_DRX_GET_DL_HQ(_hqProc) &((_hqProc)->drxCb)



#ifdef LTE_TDD

#define RGSCH_MAX_SFCFG    2   /* refer to rgSchDrxDlSfTddcfg in rg_sch.h to 
                                  understand why this is 2 */
#define RGSCH_MAX_TDD_CFG  7   
                                                     
#define RGSCH_NUM_SFRAMES 10   /* number of subframes in a RF */
  
#endif /* LTE_TDD */                                                    

#ifndef LTE_TDD                                                         
/* Introduced UL control timing delta in FDD */
/* Number of subframes in advance UL control (DCI/PHICH) should be sent from SCH */
#define TFU_ULCNTRL_DLDELTA  (RG_SCH_CMN_HARQ_INTERVAL - TFU_CRCIND_ULDELTA)
#endif/*ndef LTE_TDD*/                                                         

/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/* SI Re-configuration related bit masks */
#define RGSCH_SI_DFLT            0     /* Default */
#define RGSCH_SI_SICFG_UPD   (1<<0) /* SI CFG Updated */
#define RGSCH_SI_MIB_UPD   (1<<1) /* MIB CFG Updated */
#define RGSCH_SI_SIB1_UPD   (1<<2) /* SIB1 CFG Updated */
#define RGSCH_SI_SI_UPD           (1<<3) /* SI PDU CFG Updated */
#define RGSCH_SI_SIB1_PWS_UPD   (1<<4) /* SIB1 PWS CFG Updated */
#ifdef EMTC_ENABLE
#define RGSCH_SI_EMTC_TYPE_SIB1_BR_UPD   (1<<5) /* EMTC SIB1 BR CFG Updated */
#define RGSCH_SI_EMTC_TYPE_SI_UPD   (1<<6) /* EMTC SI PDU CFG Updated */
#endif
/*SI Scheduling Specific */
#define RGSCH_MIB_PERIODICITY        4 /*!< MIB Periodicity */ 
#define RGSCH_SIB1_PERIODICITY       8 /*!< SIB1 Periodicity */
#define RGSCH_SIB1_RPT_PERIODICITY   2 /*!< SIB1 Repeat Periodicity */
#define RGSCH_MIB_TX_SF_NUM          0 /*!< MIB TX SF Number */
#define RGSCH_SIB1_TX_SF_NUM         5 /*!< SIB1 TX SF Number */
/*rg009.lpr-ccpu00116647 -  Added siId validation for lower limit */
#define RGSCH_SI_SIID_LOWER_LMT      1 /*!< Least usabel siId number */
#define RGR_MAX_NUM_WARNING_SI       3 /*!< Max no. of Warning SI*/
#define RGSCHCHKNUPDSIPDU(_CRNTINFO,_NEWINFO,_NEWPDU,_BITMSK, _BITFLG) \
do\
{\
      if(NULLP == _CRNTINFO)\
      {\
         _CRNTINFO = _NEWPDU; \
      }\
      else\
      {\
         if(NULLP != _NEWINFO)\
         {\
            RGSCH_FREE_MSG(_NEWINFO);\
            _NEWINFO = NULLP;\
         }\
         _NEWINFO = _NEWPDU;\
         _BITMSK |= _BITFLG;\
      }\
}while(0)

#define RGSCH_SET_SI_INFO(_CRNTPTR,_NEWPTR)\
do\
{\
     if(NULLP != _CRNTPTR)\
        RGSCH_FREE_MSG(_CRNTPTR);\
     _CRNTPTR = _NEWPTR;\
     _NEWPTR = NULLP;\
}while(0)
#endif /*RGR_SI_SCH */
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/
 /* Array Based List size to store next active Periodic CQI, SRS and SR 
    tranmission instances at cell Cb  */

 /* Periodic CQI  max periodicity =   160 Ref: 36.213 (Table 7.2.2-1A For FDD 
                                        and 1C for TDD) */
 /* SRS      max periodicity =   320 Ref: 36.213 (Table 8.2-1) */
 /* SR       max periodicity =   80  Ref: 36.213 (Table 10.1-5 ) */
 /* Modifying Queue Size from 320 to 321. 320 is a possible periodicity for RI
  * and SRS. If it is 320, then while moving to next occassion it is being added
  * same queue. If more than one node is available in the queue then same UE's 
  * node is adding to the same queue and processed in same TTI. This is happening
  * in infinite loop. Hence modifying it to 321, which is not a possible periodicity.
  */
/* Changing this back to 320 as it was causing 
 * problem in TDD attach and stability
 */
#define  RG_SCH_PCQI_SRS_SR_TRINS_SIZE 320 
                                           

#ifdef LTE_TDD

/* Ref 36.213 Table 8.2-2:  */
#define  RG_SCH_ISRS_MAX_SUPP       644  
/* Note: <= 10 ms periodicity is not supported */ 
#define  RG_SCH_ISRS_MIN_SUPP        10 

/* Ref 36.213 Table 7.2.2-1C */

#define RG_SCH_ICQI_MAX_SUPP     315 
#define RG_SCH_ICQI_MIN_SUPP    0 

#else
/* Ref 36.213 Table 8.2-1:  */
#define  RG_SCH_ISRS_MAX_SUPP       636  
/* Note: <= 10 ms periodicity is not supported */ 
#define  RG_SCH_ISRS_MIN_SUPP        0 


#define RG_SCH_ICQI_MAX_SUPP    541 
#define RG_SCH_ICQI_MIN_SUPP    0 

#define RG_SCH_ICQI_RESV_FDD  317

#endif

/*Refer Table 7.2.1-5: for max BW configuration M=6*/
#define RG_SCH_MAX_NUM_UE_SEL_SUBBANDS 6

/*Refer Table 7.2.1-5: for max BW configuration k =4, ceil(110/4) = 28*/
#define RG_SCH_MAX_TOT_NUM_SUBBANDS 28

/* 36.213 Table 7.2.2-1B:  */
#define RG_SCH_IRI_MAX_SUPP   965
#define RG_SCH_IRI_MIN_SUPP   0

/*K value in CQI table min and max values*/
#define RG_SCH_CQI_K_MAX    4
#define RG_SCH_CQI_K_MIN    1

#define RG_SCH_PUCCH_RES_MAX_SUPP  1185 /* TODO: Need to check value */

#define RG_SCH_IRI_MAX_SUPP   965 


#define RG_SCH_ISR_MAX_SUPP 154
#define RG_SCH_ISR_MIN_SUPP 0

#define RG_SCH_SRS_FREQDOM_POS_MIN    0
#define RG_SCH_SRS_FREQDOM_POS_MAX    23

#define RG_SCH_SRS_TXCOMB_MIN    0
#define RG_SCH_SRS_TXCOMB_MAX    1

#define RG_SCH_SR_RES_IDX    2047   /* TODO: Need to check */
/*Reference: 36.213 Table:7.2.2-1A */
#define RG_SCH_CQIPMI_CFGIDX_MAX_FDD  10

/* Reference: 36.213 Table:7.2.2-1C */
#define RG_SCH_CQIPMI_CFGIDX_MAX_TDD  7


/* Note: RI table is same for FDD and TDD */
/*Reference: 36.213 Table:7.2.2-1B */
#define RG_SCH_RI_CFGIDX_MAX  6


/*Reference: 36.213 Table:7.2.2-2 */
#define RG_SCH_BW_SUBSZ_BWPARTS_MAX   5

/* Reference : 36.213 Table 8.2-1 */
#define RG_SCH_SRS_ISRS_INDX_MAX_FDD  8

/* Reference : 36.213 Table 8.2-2 */
#define RG_SCH_SRS_ISRS_INDX_MAX_TDD  7

/* Reference : 36.213 Table 10.1-5 */
/* Note: SR is same table for TDD and FDD */
#define RG_SCH_ISR_INDX_MAX  5


/* This use used to mark as invalid index value */
#define RG_SCH_INVALID_IDX 0xffff

/*ccpu00116923 - ADD - SRS present support*/
#define RGSCH_CELLSP_SRS_SF_CONFIGS 16

/*Used for Periodic CQI. */
#define RG_SCH_GET_CQI_J_VAL(_dlBw, _j) \
{\
 if(_dlBw >= 64)\
   {\
   _j = 4;\
   }\
 else if(_dlBw >= 27)\
   {\
   _j = 3;\
   }\
 else if(_dlBw >= 11 )\
   {\
   _j = 2;\
   }\
 else\
   {\
   _j = 1;\
   }\
}

#define RG_SCH_GET_CQI_K_VAL(_dlBw,  _k) \
{\
 if(_dlBw >= 64)\
   {\
   _k = 8;\
   }\
 else if(_dlBw >= 27)\
   {\
   _k = 6;\
   }\
 else if(_dlBw >= 11 )\
   {\
   _k = 4;\
   }\
 else\
   {\
   _k = 4;\
   }\
}


#define RG_SCH_GET_SBCQI_M_K_VAL(_dlBw, _m, _k) \
{\
 if(_dlBw >= 64)\
   {\
   _m = 6;\
   _k = 4;\
   }\
 else if(_dlBw >= 27)\
   {\
   _m = 5;\
   _k = 3;\
   }\
 else if(_dlBw >= 11 )\
   {\
   _m = 3;\
   _k = 2;\
   }\
 else\
   {\
   _m= 1;\
   _k = 2;\
   }\
}


/* To Get the Idx to pCqiSrsSrLst in RgSchCellCb*/
#define RG_SCH_GET_IDX_PCQISRSSR(_time, _indexId)\
{\
   (_indexId) = (_time.sfn)* RGSCH_NUM_SUB_FRAMES_5G + (_time.slot); \
   (_indexId) = (_indexId)%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;\
} 

#define RG_SCH_GET_UE_CELL_CQI_CB(_ueCb, _cell)  &((_ueCb)->cellInfo\
      [_ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(_cell)]]->cqiCb)

/* To Get UE Next Active Periodic RI Idx to the table pCqiSrsSrLst */
/* Caller should check the periodic RI is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_RI(_ueCb, _cell, _riIdx)\
{\
   (_riIdx) = (_ueCb)->cellInfo[_ueCb->cellIdToCellIdxMap\
               [RG_SCH_CELLINDEX(_cell)]].cqiCb.nRiTrIdx;\
}


/* To Get UE Next Active SR Idx to the table pCqiSrsSrLst */
/* Caller should check the periodic SR is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_SR(_ueCb, _srIdx)\
{\
   (_srIdx) = (_ueCb)->srCb..nSrTrIdx;\
}

/* To Get UE Next Active SRS Idx to the table pCqiSrsSrLst */
/* Caller should check the SRS is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_SRS(_ueCb, _ssrIdx)\
{\
   (_srsIdx) = (_ueCb)->srsCb..nSrsTrIdx;\
}



/* To Get UE Next Active Periodic CQI Idx to the table pCqiSrsSrLst */
/* Caller should check the periodic CQI is RG_SCH_INVALID_IDX or not */ 
#define RG_SCH_GET_IDX_PCQI(_ueCb, _cell, _pCqiIdx)\
{\
   (_pCqiIdx) = (_ueCb)->cellInfo[_ueCb->cellIdToCellIdxMap\
               [RG_SCH_CELLINDEX(_cell)]].cqiCb.nCqiTrIdx;\
}

/**
 * @def RG_SCH_PARSE_MULTI_PMI
 *
 *    This macro is used to parse RAW PMI 
 *  
 *
 * @param[in] _bitLen       bit length
 * @param[in] _totPmiBitLen total PMI bit length
 * @param[in] _ueCb         ue control block
 * @param[in] _revArray     reverse array
 * @param[out] _psMode12    Period mode 12
 * @param[out] _sbOffst     subband offset
 *
 */
#define  RG_SCH_PARSE_MULTI_PMI(_bitLen, _totPmiBitLen, _psMode12,\
                                   _acqiCb, _revArray,_sbOffst )\
{\
   uint8_t _loop =0;\
   for(_loop =0; _loop<(_totPmiBitLen/_bitLen); _loop++)\
   {\
       (_psMode12)->subbandArr[_loop].pmi = (uint8_t)rgSCHUtlParse(_revArray,\
               _sbOffst,(uint8_t)(_sbOffst+_bitLen), (uint8_t)TFU_MAX_CQI_BYTES);\
       _sbOffst+=_bitLen;\
       (_psMode12)->subbandArr[_loop].subBand.numRb = (_acqiCb).k;\
       (_psMode12)->subbandArr[_loop].subBand.rbStart = \
                                   (uint8_t) ((_acqiCb).k * (_loop));\
   }\
}

/*  Removed the MACRO RG_SCH_GET_PERIODICITY_TBL
 *  The function rgSCHUtlGetPcqiSrsSrRiTbl will do the same task as this 
 *  MACRO*/

#endif 




#ifdef LTEMAC_HDFDD
/* Half Duplex Specific defines */
/* Number of subframes information managed */
#define RG_SCH_HDFDD_NUMSFINFO   20
/* Subframe States */
#define RG_SCH_HDFDD_NOSCHD       0x00
#define RG_SCH_HDFDD_DLDATA       0x01
#define RG_SCH_HDFDD_DLCNTRL      0x02
#define RG_SCH_HDFDD_UL           0x04

#define RG_SCH_HDFDD_INVSFN       (RGSCH_MAX_SFN + 100) /* Add some arbitrary number to make it invalide */

#define RG_SCH_HDFDD_GRDTIM_DUR  1
#define RG_SCH_HDFDD_DELTA   10

/* To get the BCH is present or not at subframe */
#define RG_SCH_BCCH_TRUE_FALSE( _time, _bchTrue)\
{\
  _bchTrue = FALSE;\
  /* Call the API is provided by SI module */ \
}

/* Mark the subframe */
#define RG_SCH_HDFDD_MARKSTATE(_ueCb, _state, _sfn, _sfi)\
{\
   (_ueCb)->hdFddCb->subfrm[(_sfi)].subFrmDir = _state;\
   (_ueCb)->hdFddCb->subfrm[(_sfi)].sfn = _sfn;\
}

/* validate the ueCb and mark */
#define RG_SCH_HDFDD_VLDTANDMARK(_ueCb, _state, _sfn, _sfi)\
{\
   if( (_ueCb) != NULLP && (_ueCb)->hdFddCb)\
   {\
      RG_SCH_HDFDD_MARKSTATE(_ueCb, _state, _sfn, _sfi);\
   }\
}\

/* Get SFN and SFI from tti numbers */
#define RG_SCH_HDFDD_GETPTI(_time) ((((_time).sfn * RGSCH_NUM_SUB_FRAMES + \
                 (_time).subframe) + RG_SCH_HDFDD_DELTA) % RG_SCH_PCQI_SRS_SR_TRINS_SIZE)

/* Get SFI and SFN from given time and subframe offset */
#define RG_SCH_HDFDD_GETSFN(_sfn, _time, _offset)\
                     rgSCHHdFddGetSfn(&(_sfn), (_time), (_offset))

/* Get SFI and SFN from given time and subframe offset */
#define RG_SCH_HDFDD_GETSFI(_sfi, _time, _offset)\
          (_sfi) = (((_time).sfn * RGSCH_NUM_SUB_FRAMES) + \
                  ((_time).subframe + _offset))% RG_SCH_HDFDD_NUMSFINFO

/*If UE is HDFDD enabled */
#define RG_SCH_HDFDD_UE_ENBLD(_ue) ((_ue)->hdFddEnbld)
/*If HDFDD UE is scheduled */
#define RG_SCH_HDFDD_ISCMN_SCHED(_dlSf) ((_dlSf)->bch.tbSize || \
                    (_dlSf)->bcch.tbSize || (_dlSf)->pcch.tbSize)

#define RG_SCH_HDFDD_ROLLSFN(_sfCount, _sfn)\
do{\
   if (_sfCount < -RGSCH_NUM_SUB_FRAMES) \
   {\
      _sfn = (_sfn + (_sfCount/RGSCH_NUM_SUB_FRAMES)) & (RGSCH_MAX_SFN - 1);\
   } \
}while(0);

#endif /* LTEMAC_HDFDD */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT

/* Macro to check if expected CQI report collation has been done */
#define RG_SCH_CQIR_IS_TIMTOSEND_CQIREPT(_ue)\
     ((_ue)->schCqiInfo.cqiCount >= \
     (_ue)->cqiReptCfgInfo.numColltdCqiRept )

/* Macro to check if PUSH N CQI Reporting is still active */
#define RG_SCH_CQIR_IS_PUSHNCQI_ENBLE(_ue)\
   ((_ue)->cqiReptCfgInfo.numColltdCqiRept != 0)
#endif /* ifdef RGR_CQI_REPT */

#ifdef TFU_UPGRADE
/* Macro to check if Pa has valid value */
#define RG_SCH_IS_PAPRSNT(_ue,_cell) (_ue->cellInfo[_ue->cellIdToCellIdxMap\
               [RG_SCH_CELLINDEX(_cell)]]->pA.pres == TRUE)
#endif

/* Macro to check if Pa has been configured by RRM */
#define RG_SCH_UE_CFG_ISPAPRSNT(_info) ((_info).pAPrsnt == TRUE)

/* Macro to get absolute difference between two INT */
#define RG_ABSLT_DIFF(x, y) ((x > y)? (x-y):(y-x)) 

/* MIB transmission Count for DL TB counter */
#ifdef LTE_L2_MEAS
#define RG_SCH_MIB_CNT       4
#endif 

#define RG_SCH_PFS_FLW_CTRL_TRIG_TIME 64 

/* Interval between Transmission and feedback */
#define RG_SCH_CMN_HARQ_INTERVAL  4   
#define RG_SCH_MAX_MPHICH         3
#define RG_SCH_CMN_MAX_CFI        4

#define RGSCH_GET_SPS_SF_CFI(_dlTotalBw, _cfi)\
{\
   if(_dlTotalBw <= 10)\
   {                              \
      _cfi= 2;                    \
   }                              \
   else                           \
   {                              \
      _cfi = RGSCH_MIN(2, _cfi);  \
   }                              \
}      

#ifdef TFU_UPGRADE
#define RG_UPD_ACQI_TRIG_WT(_ue, _cell,_isAck)\
{\
   RgSchUeCellInfo *sCellInfo = RG_SCH_CMN_GET_SCELL_INFO(_ue,_cell);\
   if(sCellInfo->acqiCb.aCqiCfg.pres)\
   {\
      rgSCHUtlUpdACqiTrigWt(_ue, sCellInfo, _isAck);\
   }\
}
#else
#define RG_UPD_ACQI_TRIG_WT(_ue, _cell, _isAck){}
#endif

#define RG_SCH_MAX_HQP_SHIFT_Q_SZ         128   /*!< Length of the HqP Shift Q sizes maintained to
                                                  maintain which HqPs shall be tried on LAA SCELL
                                                  and which HqPs shall be move to PCELL */
#define RG_SCH_HQP_TIME_ON_PCELL 32 /*!< Time in milliseconds to be allowed
                                      for transmission of TB on PCell*/
/* SR_RACH_STATS */
uint32_t rgNumPrachRecvd;       /* Num of Rach Req received including dedicated preambles */
uint32_t rgNumRarSched;         /* Num of RARs sent */
uint32_t rgNumBI;               /* Num of BackOff Ind sent */
uint32_t rgNumMsg3CrcPassed;    /* Num of CRC success for Msg3 */
uint32_t rgNumMsg3CrcFailed ;    /* Num of CRC fail for Msg 3 */
uint32_t rgNumMsg3FailMaxRetx ;  /* Num of Msg3 fail after Max Retx attempts */
uint32_t rgNumMsg4Ack ;          /* Num of Acks for Msg4 Tx */
uint32_t rgNumMsg4Nack ; 
       /* Num of Nacks for Msg4 Tx */
uint32_t rgNumMsg4FailMaxRetx ;  /* Num of Msg4 Tx failed after Max Retx attempts */
uint32_t rgNumSrRecvd;          /* Num of Sched Req received */
uint32_t rgNumSrGrant;          /* Num of Sched Req Grants sent */
uint32_t rgNumMsg3CrntiCE;      /* Num of Msg 3 CRNTI CE received */
uint32_t rgNumDedPream ;         /* Num of Dedicated Preambles recvd */
uint32_t rgNumMsg3CCCHSdu;      /* Num of Msg 3 CCCH Sdus recvd */
uint32_t rgNumCCCHSduCrntiNotFound ;  /*UE Ctx not found for CCCH SDU Msg 3 */
uint32_t rgNumCrntiCeCrntiNotFound ;  /*UE Ctx not found for CRNTI CE Msg 3 */
uint32_t rgNumMsg4WithCCCHSdu ;       /* Num of Msg4 with CCCH Sdu */
uint32_t rgNumMsg4WoCCCHSdu ;         /* Num of Msg4 without CCCH Sdu */
uint32_t rgNumMsg4Dtx ;               /* Num of DTX received for Msg 4 */
uint32_t rgNumMsg3AckSent ;           /* Num of PHICH Ack sent for Msg 3 */
uint32_t rgNumMsg3NackSent ;          /* Num of PHICH Nack sent for Msg 3 */
uint32_t rgNumMsg4PdcchWithCrnti ;    /* Num of PDCCH for CRNTI based contention resolution */
uint32_t rgNumRarFailDuetoRntiExhaustion ; /* Num of RACH Failures due to RNTI pool exhaution */
uint32_t rgNumTAModified ;            /* Num of times TA received is different from prev value */
uint32_t rgNumTASent ;               /* Num of TA Command sent */
uint32_t rgNumMsg4ToBeTx ;           /* Num of times MSG4 that should be sent */
uint32_t rgNumMsg4Txed ;             /* Num of MSG4 actually sent *//* ysNumMsg4ToBeTx -ysNumMsg4Txed == Failed MSG4 TX */
uint32_t rgNumMsg3DtxRcvd;         /* CRC Fail with SINR < 0 */

uint32_t rgNumDedPreamUECtxtFound;
#endif /* __RGSCH__ */

/**********************************************************************
         End of file
**********************************************************************/
