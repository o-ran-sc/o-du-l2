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
 
    Name:   MAC layer
 
    Type:   H include file
 
    Desc:   Defines required by LTE-MAC
 
    File:   rg.h
 
**********************************************************************/
 
/** @file rg.h
@brief This file contains definitons for Mac.
*/

 
#ifndef __RGH__
#define __RGH__



#ifdef L2_OPTMZ
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "ss_strm.h"
#include "ss_strm.x"
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x" 

#define RG_MAC_HDR_SIZE 250
#define RG_MAC_CE_SIZE  250
uint32_t MacPtrAddress;
uint32_t macHeader[2];

#define RG_ADD_DBuf(_dBuf,_size,_mBuf)\
{ \
  SsMsgInfo *mInfo = NULLP; \
  mInfo = (SsMsgInfo *)_mBuf->b_rptr; \
  ssGetDBufOfSize(mInfo->region,_size, &_dBuf); \
  SUpdMsg(_mBuf, _dBuf,0);\
  _dBuf->b_wptr = _dBuf->b_rptr =  (_dBuf->b_datap->db_base + 5);\
  MacPtrAddress = (uint32_t)_dBuf->b_wptr;\
}
#endif



#define RGLAYERNAME     "MAC"
#define RG_MAX_TB_PER_UE 2
#define RG_MAX_BCCH_DLSCH 2
#define RG_INVALID_RNTI   0x0000

#define RG_MAX_NUM_DED_LC 10       /* maximum dedicated logical channels in a UE */
#define RG_MAX_NUM_CMN_LC 5       /* maximum number of common logical 
                                     channels in a cell */
#define RG_MAX_LCG_PER_UE 4
#define RG_CON_RES_ID_SZ  6
#define RG_MAX_RA_RNTI   60

/* Added support for SPS*/
#ifdef LTEMAC_SPS
#define RG_ADDL_SPS_OCC_FOR_EXP_REL 2

#define RG_SPS_SID_PACKET_SIZE 10
#define RG_SPS_LCG_ID  1

#endif

/* Well known RNTIS */
#define RG_SI_RNTI        0xffff
#define RG_P_RNTI         0xfffe

#define RG_MAX_UPPERSAP               3
#define RG_MAX_LOWERSAP               1 
#define RG_TQ_SIZE                    10   /* Timing Queue Size */
#define RG_MAX_TIMER     RG_MAX_LOWERSAP   /* MAX number of MAC timers */
#define RG_NMB_CELL_HASHBIN           10   /* Number of Hash Bins for cell hash
                                              table */
#define RG_NMB_UE_HASHBIN             10   /* Number of Hash Bins for UE hash
                                              table */        
#define RG_BNDREQ_TMR    1    /* Bind Request timer event value */
#define RG_MAX_BNDRETRY  2    /* Max number of Bind Retries for TFU SAP */


#define RG_UE_TQ_SIZE    10   /* Timing Queue Size */

#define RG_INVALID_SCHD_TYPE 255
#define RG_MAX_UE_PER_CELL 0xFFFF /*!< Maximum number of UEs per cell */
#ifdef EIGHT_UE_PER_TTI_CHANGES
#define RG_MAX_UE_BIN_PER_CELL 1024 /*!< Maximum number of UE bins per cell */
#else
#define RG_MAX_UE_BIN_PER_CELL 128 /*!< Maximum number of UE bins per cell */
#endif
#define RG_MIN_DL_BW       6    /*!< Minimum Downlink bandwidth in RBs */
#define RG_MAX_DL_BW       110  /*!< Maximum Downlink bandwidth in RBs */
#define RG_MIN_UL_BW       6    /*!< Minimum Uplink bandwidth in RBs */
#define RG_MAX_UL_BW       110  /*!< Maximum Uplink bandwidth in RBs */
#define RG_MIN_CFI_VAL     1    /*!< Minimum value for CFI */
#define RG_MAX_CFI_VAL     3    /*!< Maximum value for CFI */
#define RG_QM_BPSK         2    /*!< Qm value for BPSK */
#define RG_QM_QPSK         4    /*!< Qm value for QPSK */
#define RG_QM_64QAM        6    /*!< Qm value for 64QAM */
#define RG_MIN_SRS_SFCFG_IDX 0  /*!< Minimum value for SRS subframe 
                                     configurtion index */
#define RG_MAX_SRS_SFCFG_IDX 15  /*!< Maximum value for SRS subframe 
                                     configurtion index */
#define RG_MAX_SRS_TX_OFFSET 8    /*!< Maximum number of SRS transmission 
                                    offsets per cell */
#define RG_MIN_MAC_RNTI      10   /*!< Minimum value of RNTI to be managed by 
                                    MAC */

/* HARQ related MACROs */
#define RG_NUM_DL_HQ_PROC 8
#define RG_NUM_UL_HQ_PROC 8
#define RG_MIN_HQ_TX 1

/* Group power related MACROs */
#define RG_MAX_GRP_PWR_FMT3_IDX  15   /*!< Maximum index value for group power format 3 */
#define RG_MAX_GRP_PWR_FMT3A_IDX 31   /*!< Maximum index value for group power format 3A */

/* MACROs to indicate cell specific config for cell to be active */
#define RG_BCCH_BCH_CFG_DONE      (1<<0)
#define RG_BCCH_DLSCH_CFG1_DONE    (1<<1)
#define RG_BCCH_DLSCH_CFG2_DONE    (1<<2)
#define RG_PCCH_CFG_DONE          (1<<3)
#define RG_UL_CCCH_CFG_DONE       (1<<4)
#define RG_DL_CCCH_CFG_DONE       (1<<5)
#define RG_SCHD_CFG_DONE          (1<<6)
#define RG_CELL_ACTIVE (RG_BCCH_BCH_CFG_DONE | RG_BCCH_DLSCH_CFG1_DONE |  RG_BCCH_DLSCH_CFG2_DONE | RG_PCCH_CFG_DONE | RG_UL_CCCH_CFG_DONE | RG_DL_CCCH_CFG_DONE)
/* Logical channel realated MACROs */
#define RG_INVALID_LCG_ID  255
#define RG_INVALID_LC_ID   255
#define RG_BCCH_BCH_IDX   0
#define RG_BCCH_DLSCH_IDX 1
#define RG_PCCH_IDX 2

/* constants for MIN & MAX qci*/
#define RG_QCI_MIN 1
#define RG_QCI_MAX 9

/* PUCCH related macros */
#define RG_PUCCH_MAXVAL_CS    7   /*!< Maximum value for cyclic shift of PUCCH */
#define RG_PUCCH_MINVAL_DS    1   /*!< Mininmum value for delta shift of PUCCH */
#define RG_PUCCH_MAXVAL_DS    3   /*!< Maximum value for delta shift of PUCCH */

/* DUX related macros */
#define RG_LCID_MASK 0x3F

#define RG_LCID_LEN 0x6
#define RG_CCCH_LCID 0x00
#define RG_DEDLC_MIN_LCID 0x01
#define RG_DEDLC_MAX_LCID 0x0A
#define RG_RES_MIN_LCID 0x0B
#define RG_RES_MAX_LCID 0x18
#define RG_EXT_PHR_LCID 0x19
#define RG_PHR_LCID 0x1A
#define RG_CRNTI_LCID 0X1B
#define RG_TRUNC_BSR_LCID 0X1C
#define RG_SHORT_BSR_LCID 0X1D
#define RG_LONG_BSR_LCID  0X1E
#define RG_PAD_LCID 0x3F
#define RG_MAX_EXTN_PAD_SUBHDRS 0x02
#define RG_REF_PCMAX  0xFF

#define RG_CCCH_SDU_PRSNT     (1<<0)
#define RG_CRNTI_CE_PRSNT     (1<<1)
#define RG_PHR_CE_PRSNT       (1<<2)
#define RG_TRUNC_BSR_CE_PRSNT (1<<3)
#define RG_SHORT_BSR_CE_PRSNT (1<<4)
#define RG_LONG_BSR_CE_PRSNT  (1<<5)
/* L2_COUNTERS */
#define RG_ACTIVE_LC_PRSNT  (1<<6)

#ifdef LTEMAC_SPS
#define RG_UL_SPS_ACT_PRSNT (1<<7)
#endif
#define RG_EXT_PHR_CE_PRSNT (1<<8)

/* LOGICAL CHANNEL */
#define RG_MAX_LC_PER_UE      10
/* Maximum number of common logical channel control blocks */
#define RG_MAX_CMN_LC_CB      3
#define RG_MAX_BCCH           2

#ifdef LTE_ADV
#define RG_MAX_SCELL_PER_UE   7  /*!< MAX SCell can be Added per Cell */
#endif /* LTE_ADV */

#define RG_OPTM_NUM_DED_LC    3

/* Random access related MACROs */
#define RG_MAX_RA_PREAMBLE_FMT 3 /*!< Maximun value of Random access preamble 
                                      format */
#define RG_MAX_RA_WINSIZE    10  /*!< Maximum size of Random access response 
                                      window in subframes */
#define RG_MIN_RA_WINSIZE    2   /*!< Minimum size of Random access response 
                                      window in subframes */
#define RG_MIN_NUM_RA_PREAMBLE 4 /*!< Minimum number of Random access 
                                      preambles */
#define RG_MAX_NUM_RA_PREAMBLE 64 /*!< Maximim number of Random access 
                                      preambles */
#define RG_NUM_RA_RB     6

#define RG_MAX_RA_RSP_ALLOC    4 /*!< Maximum number of Random access
                                      allocations */
#define RG_CRG_CFG 1          /* CRG configuration element */
#define RG_RGR_CFG 2          /* RGR configuration element */

#define RG_NUM_ITBS         27
#define RG_MAX_NUM_RB       110

#define RG_MAX_NUM_PAD_HDRS 2

/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
#define RG_MAX_LYR_PERCW    2

#ifdef LTE_TDD
#define RG_MAX_DL_HARQ_NUM   15
#else
#define RG_MAX_DL_HARQ_NUM   8 
#endif

/* Free shared memory, received through LWLC */
#define MAC_FREE_MEM(_region, _pool, _datPtr, _size)         \
       MAC_FREE(_datPtr, _size);      \

#define RG_LCG_ISCFGD(lcg) ((lcg)->lcgId != RG_INVALID_LCG_ID)
/* Corrected the check for dlCcchId */
#define RG_DLCCCH_ISCFGD(cell) ((cell)->dlCcchId != RG_INVALID_LC_ID)
#define RG_ULCCCH_ISCFGD(cell) ((cell)->ulCcchId != RG_INVALID_LC_ID)
/* After merging from 2.1 to 2.2 */
#define RG_CALC_SF_DIFF(_time1, _time2)\
   (_time1.sfn*RG_NUM_SUB_FRAMES_5G+_time1.slot) < (_time2.sfn*RG_NUM_SUB_FRAMES_5G+_time2.slot)?\
     ((_time1.sfn+RG_MAX_SFN)*RG_NUM_SUB_FRAMES_5G+_time1.slot) -\
       (_time2.sfn*RG_NUM_SUB_FRAMES_5G+_time2.slot) : \
     (_time1.sfn*RG_NUM_SUB_FRAMES_5G+_time1.slot) - (_time2.sfn*RG_NUM_SUB_FRAMES_5G+_time2.slot)
/*LTE_L2_MEAS_PHASE2*/
#define RG_CALC_SFN_SF_DIFF(_time1,_sfnCycle, _time2)\
(((_time1.sfn+RG_MAX_SFN * _sfnCycle)*RG_NUM_SUB_FRAMES_5G) + _time1.slot -\
(_time2.sfn*RG_NUM_SUB_FRAMES_5G + _time2.slot))

#define RG_EXT_LCID(_lcId, _byte) {\
      (_lcId) = (_byte) & RG_LCID_MASK; \
}
#define RG_EXT_FORMT_BIT(_fmtBit, _byte) {\
      (_fmtBit) = ((_byte) >> RG_LCID_LEN) & 0x01; \
}

#define RG_EXT_EXTN_BIT(_extnBit, _byte) {\
      (_extnBit) = ((_byte) >> RG_LCID_LEN) & 0x01; \
}

#ifndef MS_MBUF_CORRUPTION
#define MS_BUF_ADD_ALLOC_CALLER()
#endif

#define RG_PACK_PAD(_padBuf,_size,_sduBuf) { \
  Buffer *sduEnd = NULLP; \
  SsMsgInfo *mInfo = NULLP; \
  mInfo = (SsMsgInfo *)_sduBuf->b_rptr; \
  ssGetDBufOfSize(mInfo->region,_size, &_padBuf); \
  if (_padBuf == NULLP) \
  { \
    DU_LOG("\nERROR  -->  MAC : RGERR_MUX_BLD_CEHDR_FAIL");\
    return RFAILED;\
  } \
  if (mInfo->endptr == NULLP) { \
    sduEnd = _sduBuf; \
    }\
  else \
  {  \
    sduEnd = mInfo->endptr; \
  } \
  sduEnd->b_cont = _padBuf; \
  padBuf->b_wptr += _size; \
  mInfo = (SsMsgInfo *)_sduBuf->b_rptr; \
  mInfo->endptr = _padBuf; \
  mInfo->len += _size; \
}

#define RG_PACK_SDU(_sdusBuf, _sduBuf, _ret) {\
   _ret = SCatMsg(_sdusBuf, _sduBuf, M1M2);\
}

#define RG_PACK_LAST_SDU_SHDR(_subHdr, _lcId, _mBuf, _ret) {\
   _ret = ROK;\
   _subHdr.shLen = 1;\
   _subHdr.shData[0] = (0x1F & _lcId);\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RGADDTOCRNTTIME(crntTime, toFill, incr)          \
   if ((crntTime.slot + incr) > (RG_NUM_SUB_FRAMES_5G - 1))   \
      toFill.sfn = (crntTime.sfn + 1);      \
   else                                                  \
      toFill.sfn = crntTime.sfn;                              \
   toFill.slot = (crntTime.slot + incr) % RG_NUM_SUB_FRAMES_5G; \
   if (toFill.sfn >= RG_MAX_SFN) \
   { \
      toFill.sfn%=RG_MAX_SFN; \
   } \

#define RGSUBFRMCRNTTIME(crntTime, toFill, dcr)    \
{                                                  \
   if (crntTime.sfn == 0)                          \
{                                                  \
   if ((crntTime.slot - (dcr)) < 0)              \
   {                                               \
      toFill.sfn = RG_MAX_SFN - 1;                 \
   }                                               \
   else                                            \
   {                                               \
   toFill.sfn = crntTime.sfn;                      \
   }                                               \
}                                                  \
else                                               \
{                                                  \
   if ((crntTime.slot - (dcr)) < 0)              \
   {                                               \
   toFill.sfn = crntTime.sfn - 1;                  \
   }                                               \
   else                                            \
   toFill.sfn = crntTime.sfn;                      \
}                                                  \
toFill.slot = (RG_NUM_SUB_FRAMES_5G + crntTime.slot - (dcr)) % (RG_NUM_SUB_FRAMES_5G);                                   \
}

#define RGCPYTIMEINFO(src, dst)  \
   dst.sfn    = src.sfn;     \
   dst.slot   = src.slot;
#define RG_TIMEINFO_SAME(x, y) ((x.sfn == y.sfn) && (x.slot == y.slot))


#define rgPBuf(_inst)  rgCb[_inst].rgInit.prntBuf

/* Debug Prints for MAC */
#ifdef DEBUGP
#define RGDBGERRNEW(_inst,_args)          \
                  DBGP(&rgCb[_inst].rgInit, RGLAYERNAME, DBGMASK_ERR, _args)
#define RGDBGINFONEW(_inst,_args)         \
                  DBGP(&rgCb[_inst].rgInit, RGLAYERNAME, DBGMASK_INFO, _args)
#else
#define RGDBGERRNEW(_inst,_args) 
#define RGDBGINFONEW(_inst,_args)
#endif /* #ifdef DEBUGP */
#define RGDBGPRM(_inst,_args) 
#define RGDBGERR(_inst,_args) 
#define RGDBGINFO(_inst,_args)

#ifdef ERRCLS_KW
#define RG_NULL_CHECK(_inst, _ptr )     \
   if((_ptr) == NULLP)  \
   {                                               \
      DU_LOG("\nERROR  -->  MAC : Null Pointer detected");\
      SExit();\
   }
#define RG_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)     \
   if((_idxVal) >= (sizeof(_array)/sizeof(_array[0]))) \
   {                                               \
      DU_LOG("\nERROR  -->  MAC : Array Bound Check Failed");\
      SExit();\
   }
#else
#define RG_NULL_CHECK(_inst, _ptr )     
#define RG_ARRAY_BOUND_CHECK(_inst, _array, _idxVal)     
#endif

/* Macro to free the message buffer and initialize it to zero */
/***********************************************************
 *
 *     Name : RG_FREE_MSG
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
#define RG_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      SPutMsg((_buf)); \
      _buf = NULLP; \
   } \
}

#define RG_FREE_MEM(_mem)\
{\
   if (NULLP != (_mem)) \
   { \
      cmFreeMem((_mem)); \
      _mem = NULLP; \
   } \
}

#ifdef L2_OPTMZ
#define RG_FREE_TB(_tb)\
{\
   uint32_t lchIdx, pduIdx;\
   SResetMBuf(_tb->macHdr);\
   SResetMBuf(_tb->macCes);\
   _tb->tbPres = FALSE;\
   _tb->tbSize = 0;\
   _tb->padSize = 0;\
   for(lchIdx = 0; lchIdx < _tb->numLch; lchIdx++) \
   {\
      for(pduIdx = 0; pduIdx < _tb->lchInfo[lchIdx].numPdu; pduIdx++)\
      {\
          if(_tb->lchInfo[lchIdx].mBuf[pduIdx] != NULL)\
          {\
            SPutMsg(_tb->lchInfo[lchIdx].mBuf[pduIdx]);\
            _tb->lchInfo[lchIdx].freeBuff = FALSE;\
          }\
          _tb->lchInfo[lchIdx].mBuf[pduIdx] = NULL;\
      }\
      _tb->lchInfo[lchIdx].numPdu = 0;\
   }\
   _tb->numLch = 0;\
}

#define RG_FREE_SAVED_TB(_tb)\
{\
   uint32_t lchIdx, pduIdx;\
   RG_FREE_MSG(_tb->macHdr);\
   RG_FREE_MSG(_tb->macCes);\
   _tb->tbPres = FALSE;\
   _tb->tbSize = 0;\
   _tb->padSize = 0;\
   for(lchIdx = 0; lchIdx < 10; lchIdx++) \
   {\
      for(pduIdx = 0; pduIdx < 4; pduIdx++)\
      {\
          if(_tb->lchInfo[lchIdx].freeBuff == TRUE)\
          {\
            SPutMsg(_tb->lchInfo[lchIdx].mBuf[pduIdx]);\
            _tb->lchInfo[lchIdx].freeBuff = FALSE;\
          }\
          _tb->lchInfo[lchIdx].mBuf[pduIdx] = NULL;\
      }\
      _tb->lchInfo[lchIdx].numPdu = 0;\
   }\
   _tb->numLch = 0;\
}

#endif

/***********************************************************
 *
 *     Name : RG_DROP_RGUDDATREQ_MBUF
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
#define RG_DROP_RGUDDATREQ_MBUF(_datReq)\
{\
   uint32_t idx5,idx6,idx7;\
   for (idx5=0; idx5 < _datReq.nmbOfTbs; idx5++)\
   {\
      for (idx6=0; idx6 < _datReq.datReqTb[idx5].nmbLch; idx6++)\
      {\
         for (idx7=0; \
               idx7 < _datReq.datReqTb[idx5].lchData[idx6].pdu.numPdu; \
               idx7++)\
         {\
            RG_FREE_MSG(_datReq.datReqTb[idx5].\
                  lchData[idx6].pdu.mBuf[idx7]);\
         }\
      }\
   }\
}

/***********************************************************
 *
 *     Name : RG_DROP_RGUCDATREQ_MBUF
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
#define RG_DROP_RGUCDATREQ_MBUF(_datReq)\
{\
   if (_datReq != NULLP)\
   {\
      RG_FREE_MSG(_datReq->pdu);\
   }\
}

/* RRM_SP1_START */
#define RG_UPD_GBR_PRB(_cellCb, _qci, _prbUsed) {\
   if(_qci <= RG_MAX_QCI_REPORTS)\
   {\
      _cellCb->qcisUlPrbCnt[_qci-1] += _prbUsed;\
   }\
}
/* RRM_SP1_END */

/* Macros for memory region and pool determination */
#define RG_GET_MEM_REGION(rgCb)  (rgCb.rgInit.region)
#define RG_GET_MEM_POOL(rgCb)    (rgCb.rgInit.pool)


/* MUX related macros */
#define RG_RAR_SHDR_LEN                1
#define RG_RAR_ELEM_LEN                6
#define RG_MAX_SDU_SUB_HDR_LEN         3
#define RG_MAX_PAD_ARR_SZ              4096 /* Changing from 400 to 4096 */
#define RG_PAD_BYTE                    0x00

#define RG_HDR_TYPE_CRES                1
#define RG_HDR_TYPE_TA                  2
#ifdef LTE_ADV
#define RG_HDR_TYPE_SCELL_ACT           3
#endif

#define RG_SDU_SHDR_LEN 1
#define RG_FIXDSZ_CE_SHDR_LEN 1
#define RG_PAD_SHDR_LEN 1
#define RG_CRES_LEN    6
#define RG_TA_LEN      1
#ifdef LTE_ADV
#define RG_SCELL_ACT_CE_LEN 1
#define RG_SCELL_CE_ELM_LEN (RG_FIXDSZ_CE_SHDR_LEN+RG_SCELL_ACT_CE_LEN)
#endif
#define RG_CRES_ELM_LEN (RG_FIXDSZ_CE_SHDR_LEN+RG_CRES_LEN)
#define RG_TA_ELM_LEN   (RG_FIXDSZ_CE_SHDR_LEN+RG_TA_LEN)


/* Values of below macros not yet defined in 5G-NR hence using LTE values till
 * They are defined
 */
#define RG_CRES_LCID_IDX               0x1C
#define RG_TA_LCID_IDX                 0x1D
#ifdef LTE_ADV
#define RG_SCELL_LCID_IDX              0x1B
#endif
#define RG_PAD_LCID_IDX               0x3F

/* Structure member offset computation macro    */
#define OffsetOf(type, field)                                                 \
         (PTR) (&(((type *) NULLP)->field))

#define RG_MAX_SUBFRAMES_IN_SFN        9
#define RG_MAX_SFN                     1024
#ifdef RGAC_5GTF
#define RG_NUM_SUB_FRAMES              50
#else
#define RG_NUM_SUB_FRAMES              10
#endif
#define RG_NUM_SUB_FRAMES_5G           50
/* RRM_SP1_START */
#define RG_MAX_QCI_REPORTS         4
/* RRM_SP1_END */

#ifdef LTE_L2_MEAS
#define RG_NUM_UL_SUB_FRAMES              16
#define RG_MAX_QCI_VALUE                  10

#ifdef EIGHT_UE_PER_TTI_CHANGES 
/* Tuned according to TDD Cfg Mode2 and 2UE/TTI.
 *  * Need to tune if NumUE/TTI is increased */
#define RG_MAX_DFRD_FREE_BUFS             64 /* 64 - 8UE/TTI */
#define RG_MAX_FREE_BUFS_PERTTI           16 /* 16 - 8UE/TTI */
#endif

#define RG_CALC_SF_DIFF(_time1, _time2)\
      (_time1.sfn*RG_NUM_SUB_FRAMES_5G+_time1.slot) < (_time2.sfn*RG_NUM_SUB_FRAMES_5G+_time2.slot)?\
     ((_time1.sfn+RG_MAX_SFN)*RG_NUM_SUB_FRAMES_5G+_time1.slot) -\
       (_time2.sfn*RG_NUM_SUB_FRAMES_5G+_time2.slot) : \
     (_time1.sfn*RG_NUM_SUB_FRAMES_5G+_time1.slot) - (_time2.sfn*RG_NUM_SUB_FRAMES_5G+_time2.slot)

#define RG_TTI_CYCLE_INVLD                0xFFFFFFFF     
#define RG_CALC_TTI_CNT(_cellCb, _ttiCnt)\
     _ttiCnt = (RG_NUM_SUB_FRAMES_5G * (_cellCb->crntTime.sfn + (_cellCb->ttiCycle * 1024)) )+\
               _cellCb->crntTime.slot;
#endif /* LTE_L2_MEAS */

/* Tuned according to TDD Cfg Mode2 and 2UE/TTI.
 * Need to tune if NumUE/TTI is increased */
#define RG_MAX_DFRD_FREE_BUFS             32 /* 16- 2 UE per TTI 32 - 4UE/TTI */
#define RG_MAX_FREE_BUFS_PERTTI           8 /* 4 - 2 Ue per TTI, 8 - 4UE/TTI */


/* Define for the block size for memory allocation */
/* RG_BLKSZ changed from 2048 to 1500*/
#define RG_BLKSZ                       1500

/* Defines for RGU Statistics types */
#define RG_RGU_SDU_DROP 1
#define RG_RGU_SDU_RCVD 2

/* MACROS for General Statistics */
#define RG_CFG_ADD      1
#define RG_CFG_DEL      2

#define RG_HQ_FDB_IND_CB_TYPE_HQ_ENT      1
#define RG_HQ_FDB_IND_CB_TYPE_RA_CB       2

/* MACRO for validating the mac instance id */
#define RG_IS_INST_VALID(_inst)\
{\
   if(_inst >= RG_MAX_INST)\
   {\
      return RFAILED;\
   }\
}

/* 
 * Removed unused hash-define which defines the
 * index for releasing the subframe.
 */

/* Value used to set nDmrs in uplink grant if nDmrs is not applicable */
#define RG_INVALID_NDMRS  10

#define RG_SEND_TRC_IND(_inst,_mBuf, _event) rgLMMTrcInd(_inst,_mBuf, _event)

/* Note: Any changes to these enums should reflect to */
/** @details Enums for special argument
 *
*/
typedef enum
{
   RG_DIAG_NA
} RgDiagSplArg;
 
#ifdef SS_DIAG 
#define RG_DIAG_LVL0(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                        \
{                                                                                                \
   if(rgCb[_inst].rgInit.logMask & SS_DIAG_LVL0)                                                         \
   {                                                                                             \
      ssDiagFix(_tknId, _splArgEnum, ENTMAC, rgCb[_inst].rgInit.inst, SS_DIAG_LVL0, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                             \
}

/** @details Macro definition for LTE-MAC error logs
 *  
*/
#define RG_DIAG_LVL1(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)            \
{                                                                 \
   if(rgCb[_inst].rgInit.logMask & SS_DIAG_LVL1)                             \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTMAC, rgCb[_inst].rgInit.inst, SS_DIAG_LVL1, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for LTE-MAC critical logs
 *  
*/
#define RG_DIAG_LVL2(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                              \
   if(rgCb[_inst].rgInit.logMask & SS_DIAG_LVL2)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTMAC, rgCb[_inst].rgInit.inst, SS_DIAG_LVL2, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for LTE-MAC logs 
 *  
*/
#define RG_DIAG_LVL3(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(rgCb[_inst].rgInit.logMask & SS_DIAG_LVL3)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTMAC, rgCb[_inst].rgInit.inst, SS_DIAG_LVL3, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for LTE-MAC logs
 *  
*/
#define RG_DIAG_LVL4(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(rgCb[_inst].rgInit.logMask & SS_DIAG_LVL4)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTMAC, rgCb[_inst].rgInit.inst, SS_DIAG_LVL4, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}
  
#else

#define RG_DIAG_LVL0(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                        \
{                                                                                                \
}

/** @details Macro definition for LTE-MAC error logs
 *  
*/
#define RG_DIAG_LVL1(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)            \
{                                                                 \
}

/** @details Macro definition for LTE-MAC critical logs
 *  
*/
#define RG_DIAG_LVL2(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                              \
}

/** @details Macro definition for LTE-MAC logs 
 *  
*/
#define RG_DIAG_LVL3(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
}

/** @details Macro definition for LTE-MAC logs
 *  
*/
#define RG_DIAG_LVL4(_inst,_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
}
#endif
#endif /* __RGH__ */

/**********************************************************************
         End of file
**********************************************************************/
