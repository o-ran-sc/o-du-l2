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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_mux.c 
  
**********************************************************************/

/** @file rg_mux.c
@brief MAC Multiplexing API.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=229;
static int RLOG_MODULE_ID=4096;

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_lte.h"        /* Common LTE Defines */

#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* TFU Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg_err.h"        /* MAC error defines */
#include "rg.h"            /* MAC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* memory management */
#include "cm_lte.x"        /* Common LTE Definitions */

#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* CRG Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "rg_sch_inf.x"    /* SCH Interface includes */
#include "rg_prg.x"        /* PRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "du_mgr_mac_inf.h"
#include "rg.x"            /* MAC includes */
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"            /* MAC includes */
/* local defines */
#ifndef T2K_MEM_LEAK_DBG
EXTERN  S16 ssGetDBufOfSize(Region region,Size size,Buffer **dBuf);
#else
char* file = __FILE__;
U32 line = __LINE__;
#endif

/* local typedefs */

/* global variables */
U32 rgDlrate_rgu;

/* local externs */

PRIVATE Void rgMUXGet20bitRarGrnt ARGS((U8 ulBw,
                                        RgInfRarUlGrnt *msg3Grnt,
                                        U8 *grnt));
EXTERN U16 rgMUXCalcRiv ARGS((U8 bw,
                                U8 rbStart,
                                U8 numRb));
 
#ifndef MS_MBUF_CORRUPTION
#define MS_BUF_ADD_ALLOC_CALLER()
#endif
/* forward references */

#define RG_PACK_SHDR_FIXD_SZ(_subHdr, _lcId, _mBuf, _ret) {\
   _subHdr.shLen = RG_FIXDSZ_CE_SHDR_LEN;\
   _subHdr.shData[0] = (0x3F & _lcId);\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_CE(_ce, _len, _ceBuf, _ret) {\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult((U8 *)(&(_ce)), _len, _ceBuf);\
}

#define RG_MUX_CALC_LEN(_len,_lenBytes,_elmTotLen) {\
   U8 _hdrLen;\
   _lenBytes    = (_len <= 255) ? 1 : 2;\
   _hdrLen      = _lenBytes + RG_SDU_SHDR_LEN;\
   _elmTotLen   = _hdrLen + _len;\
}

#define RG_PACK_VAR_SZ_CE_SDU_SHDR(_subHdr, _lcId, _len,_mBuf, _ret) {\
   _ret = ROK;\
   if(_len <= 255)\
   {\
      _subHdr.shData[0] = (0x3F & _lcId);\
      _subHdr.shLen = 2;\
      _subHdr.shData[1] = (0xFF & _len);\
      _subHdr.shData[2] = 0;\
   }\
   else\
   {\
   _subHdr.shData[0] = (0x7F & ((0x40) | _lcId));\
      _subHdr.shLen = 3;\
      _subHdr.shData[1] = (0xFF & (_len >> 8));\
      _subHdr.shData[2] = (0xFF & _len);\
   }\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_PAD_SHDR(_mBuf, _ret) {\
      _ret = SAddPreMsg(0x3F, _mBuf);\
}

#define RG_PACK_RAR_SHDR(_byte, _mBuf, _ret) {\
   _ret = SAddPstMsg(_byte, _mBuf);\
}


/**
 * @brief Function to add ces along with subhdrs.
 * This function packs first CE sub-hdr and then CE in ceBuf pointer 
 *
 * @details
 *
 *     Function : rgMUXAddCes
 *     
 *  @param[in]  Inst        inst
 *  @param[in] RgBldPduInfo   *pdu
 *  @param[in] Buffer        *ceBuf 
 *  @param[in] RgErrInfo     *err 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddCes
(
Inst           inst,
RgBldPduInfo   *pdu,
Buffer         *ceBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddCes(inst,pdu, ceShdrBuf, ceBuf, err)
Inst           inst;
RgBldPduInfo   *pdu;
Buffer         *ceBuf;
RgErrInfo      *err;
#endif
{
   S16            ret;
   RgMUXSubHdr    subHdr;

   TRC2(rgMUXAddCes)

   if (NULLP != pdu->contResId)
   {
      if(pdu->schdTbSz >= RG_CRES_ELM_LEN)
      {
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_CRES_LCID_IDX, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
            RLOG0(L_ERROR, "Muxing of Contention Resolution CE sub-header is failed");
            RETVALUE(RFAILED);
         }

         RG_PACK_CE(pdu->contResId->resId[0], RG_CRES_LEN, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CE_FAIL;
            RLOG0(L_ERROR, "Muxing of Contention Resolution CE is failed")
            RETVALUE(RFAILED);
         }
         pdu->schdTbSz -= RG_CRES_ELM_LEN;
      }
   }
   if (TRUE == pdu->ta.pres)
   {
      if(pdu->schdTbSz >= RG_TA_ELM_LEN)
      {
         U8 taVal; /* Moving from outer scope to available scope */
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_TA_LCID_IDX, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
            RLOG0(L_ERROR, "Muxing of TA CE sub-hdr is failed")
            RETVALUE(RFAILED);
         }

         taVal = pdu->ta.val;
         RG_PACK_CE(taVal, RG_TA_LEN, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CE_FAIL;
            RLOG0(L_ERROR, "Muxing of TA CE is failed")
            RETVALUE(RFAILED);
         }
         pdu->schdTbSz -= RG_TA_ELM_LEN;
         RLOG1(L_DEBUG,"TA muxed by MAC: %u", pdu->ta.val);
      }
   }
#ifdef LTE_ADV
   if(TRUE == pdu->sCellActCe.pres)
   {
      if(pdu->schdTbSz >= RG_SCELL_CE_ELM_LEN)
      {
         /* Adding the subheader for ACT CE */
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_SCELL_LCID_IDX, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
            RLOG0(L_ERROR, "Muxing of SCELL Activation CE sub-hdr is failed")
            RETVALUE(RFAILED);
         }

         /* Adding the ACT CE */
         RG_PACK_CE(pdu->sCellActCe.val, RG_SCELL_ACT_CE_LEN, ceBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_CE_FAIL;
            RLOG0(L_ERROR, "Muxing of SCELL Activation CE is failed")
            RETVALUE(RFAILED);
         }
         pdu->schdTbSz -= RG_SCELL_CE_ELM_LEN;

      }
   }
#endif
  
  /*LcId is not yet decided in 5G-NR spec for MAC CEs Hence, not writing code
   * new MAC CEs. */

   RETVALUE(ROK);
} /* rgMUXAddCes */

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXInsSdu
 *     
 *  @param[in]       Inst        inst
 *  @param[in]       MsgLen      *schdTbSz
 *  @param[in]       U8          lcId
 *  @param[in]       Buffer      *sdu
 *  @param[out]      Buffer      *sduBuf 
 *  @param[out]      RgErrInfo   *err 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXInsSdu
(
Inst           inst,
MsgLen         *schdTbSz,
U8             lcId,
Buffer         *sdu,
Buffer         *sduBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXInsSdu(inst,schdTbSz, lcId, sdu, sduBuf, err)
Inst           inst;
MsgLen         *schdTbSz;
U8             lcId;
Buffer         *sdu;
Buffer         *sduBuf;
RgErrInfo      *err;
#endif
{
   S16            ret;
   MsgLen         msgLen = 0;
   U8             lenBytes;
   MsgLen         elmTotLen;

   TRC2(rgMUXInsSdu)
   SFndLenMsg(sdu, &msgLen);

   RG_MUX_CALC_LEN(msgLen,lenBytes,elmTotLen);
   
   if (lcId == 3)
   {
     rgDlrate_rgu += msgLen; 
   }
   if (*schdTbSz >= elmTotLen)
   {
      RgMUXSubHdr    subHdr;
      RG_PACK_VAR_SZ_CE_SDU_SHDR(subHdr, lcId, msgLen,sduBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDUHDR_FAIL;
         RLOG1(L_ERROR, "RGERR_MUX_BLD_SDUHDR_FAIL for LCID:%d",lcId);
         RETVALUE(RFAILED);
      }

#ifndef L2_OPTMZ /* no need to pack as passing not muxing all LCs PDUs to 1*/
      RG_PACK_SDU(sduBuf, sdu, ret);
#else
  //UDAY
      ret = ROK;
      UNUSED(sduBuf);
#endif

      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDU_FAIL;
         RLOG1(L_ERROR, "RGERR_MUX_BLD_SDU_FAIL for LCID:%d",lcId);
         RETVALUE(RFAILED);
      }

      *schdTbSz -= elmTotLen;
   }
   else
   {
      /* This Sub-PDU can not be accodmodated at all */
      RLOG4(L_ERROR, "Failed lcId %u, elmTotLen %d lenBytes %d LCID:%d",
               lcId, ((S16)elmTotLen), lenBytes,lcId);
      RLOG3(L_ERROR, "msglen %d schdTbSz %d LCID:%d",
               ((S16)msgLen), ((S16)*schdTbSz),lcId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddPadd
 *     
 *  @param[in]  Inst        inst
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef L2_OPTMZ
U32 padSize = 0;
#endif
#ifdef ANSI
PUBLIC S16 rgMUXAddPadd
(
Inst           inst,
MsgLen         *schdTbSz,
Buffer         *sduBuf,
Bool           isRar,
RgErrInfo      *err
)
#else
PUBLIC S16 rgMUXAddPadd(inst,schdTbSz, sduBuf, isRar, err)
Inst           inst;
MsgLen         *schdTbSz;
Buffer         *sduBuf;
Bool           isRar;
RgErrInfo      *err;
#endif
{
   S16     ret = ROK;
   Buffer         *padBuf = NULLP;
   RgMUXSubHdr    subHdr;
   TRC2(rgMUXAddPadd)

#ifdef L2_OPTMZ
   padSize = 0;
#endif
   if(*schdTbSz)
   {
#ifndef L2_OPTMZ
      if(FALSE == isRar)
#else
      if((FALSE == isRar) && (NULL != sHdrBuf))
#endif

      {
         RG_PACK_SHDR_FIXD_SZ(subHdr, RG_PAD_LCID_IDX, sduBuf, ret);  

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
            RLOG0(L_ERROR, "RGERR_MUX_BLD_PADHDR_FAIL");
            RETVALUE(RFAILED);
         }

         *schdTbSz -= 1;
      }

      if (*schdTbSz)
      {
         if (*schdTbSz <= RG_MAX_PAD_ARR_SZ)
         {
#ifndef L2_OPTMZ
            RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
#else
            if(sduBuf)
            {
               RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
            }
            else
               padSize += *schdTbSz;
#endif
            if(ret != ROK)
            {
               err->errCause = RGERR_MUX_BLD_PAD_FAIL;
               RLOG0(L_ERROR, "RGERR_MUX_BLD_PAD_FAIL");
               RETVALUE(RFAILED);
            }
            *schdTbSz = 0;
         }
         else
         {
            while (*schdTbSz)
            {
               if (*schdTbSz > RG_MAX_PAD_ARR_SZ)
               {
#ifndef L2_OPTMZ
                  RG_PACK_PAD(padBuf,RG_MAX_PAD_ARR_SZ,sduBuf);
#else
                  if(sduBuf)
                  {
                     RG_PACK_PAD(padBuf,RG_MAX_PAD_ARR_SZ,sduBuf);
                  }
                  else
                     padSize += RG_MAX_PAD_ARR_SZ;
#endif

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
                     RLOG0(L_ERROR, "RGERR_MUX_BLD_PAD_FAIL");
                     RETVALUE(RFAILED);
                  }

                  *schdTbSz -= RG_MAX_PAD_ARR_SZ;
               }
               else
               {
#ifndef L2_OPTMZ
                  RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
#else
                  if(sduBuf)
                  {
                     RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
                  }
                  else
                     padSize += *schdTbSz;
#endif

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
                     RLOG0(L_ERROR, "RGERR_MUX_BLD_PAD_FAIL");
                     RETVALUE(RFAILED);

                  }
                  *schdTbSz = 0;
               }
            }
         }
      }
   }

   RETVALUE(ROK);
} /* rgMUXAddPadd */

#ifndef L2_OPTMZ
/**
 * @brief Function to add SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddSdus
 *     
 *  @param[in]  Inst        inst
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddSdus
(
Inst           inst,
RgBldPduInfo   *pdu,
Buffer         *sduBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddSdus(inst,pdu, sduBuf, err)
Inst           inst;
RgBldPduInfo   *pdu;
Buffer         *sduBuf;
RgErrInfo      *err;
#endif
{
   RgRguDDatReqPerUe *dDatReq;
   RgRguCmnDatReq    *cDatReq;

   TRC2(rgMUXAddSdus)

   switch(pdu->reqType)
   {
      case EVTRGUCDATREQ:
         cDatReq = (RgRguCmnDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != cDatReq)
         {
            if(rgMUXInsSdu(inst,&pdu->schdTbSz, 
                     RG_CCCH_LCID, cDatReq->pdu, sduBuf, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            RG_FREE_MSG(cDatReq->pdu);
         }
         break;

      case EVTRGUDDATREQ:
         dDatReq = (RgRguDDatReqPerUe *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != dDatReq)
         {
            if(pdu->tbIndex == 1)
            {
               U16 idx1, idx2;
               /* Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[0];

               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst,&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sduBuf, err) != ROK)
                        {
                           RLOG1(L_ERROR, "FAILED for LCID:%d",datReqTb->lchData[idx1].lcId);
                           RETVALUE(RFAILED);
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
            else if(pdu->tbIndex == 2)
            {
               U16 idx1, idx2;
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[1];
               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst,&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sduBuf, err) != ROK)
                        {
                           RLOG2(L_ERROR, "FAILED TB Size %d LCID:%d",
                                    ((S16)pdu->schdTbSz),datReqTb->lchData[idx1].lcId);
                           RETVALUE(RFAILED);
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
         }
         break;

      case EVTTFUTTIIND:
         break;
      default:
         break;
   } /* End of switch(reqType) */
   if(rgMUXAddPadd(inst,&pdu->schdTbSz, sduBuf, FALSE, err) != ROK)
   {
      RLOG1(L_ERROR, "FAILED for TB Size:%d",(S16)pdu->schdTbSz);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Function to create MAC PDU from RLC SDUs and control elements, if any. 
 *
 * @details
 *
 *     Function : rgMUXBldPdu
 *     
 *     -# This function shall be invoked by Downlink Harq Module as soon as a
 *        Data request is received from RLC for a UE along with its stored 
 *        control elements to create a MAC PDU.
 *     -# It shall create subheaders for the control elements (timing advance
 *        and contention resolution ID) and pack sub-header before each CE,
 *        if given, and then shall run through all the logical channels and
 *        create subheader for each of the SDUs given on that logical channel
 *        and pack corresponding sub-header before the each SDU 
 *     -# It shall invoke rgMUXPadPdu if the total length of the created 
 *        buffer is less than the scheduled TB size. 
 *     
 *           
 *  @param[in]  Inst           *inst
 *  @param[in]  RgBldPduInfo   *bldPdu
 *  @param[in]  Buffer         **txPdu
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldPdu
(
Inst           inst,
RgBldPduInfo   *pdu,
Buffer         **txPdu,
RgErrInfo      *err
)
#else
PUBLIC S16 rgMUXBldPdu(inst, pdu, txPdu, err)
Inst           inst;
RgBldPduInfo   *pdu;
Buffer         **txPdu;
RgErrInfo      *err;
#endif
{
   Buffer         *mBuf = NULLP;

   TRC2(rgMUXBldPdu)

 
   if (rgGetMsg(inst, &mBuf) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG1(L_FATAL, "Memory allocation failed during MUXing of MAC TB: MacInst %d", inst);
      RETVALUE(RFAILED);
   }

   if(rgMUXAddCes(inst, pdu, mBuf, err) != ROK)
   {
      RG_FREE_MSG(mBuf);
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG1(L_ERROR, "Failed to Multiplex MAC CEs: MacInst %d", inst);
      RETVALUE(RFAILED);
   }

   if(rgMUXAddSdus(inst, pdu, mBuf, err) != ROK)
   {
      RG_FREE_MSG(mBuf);
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG1(L_ERROR, "FAILED to Multiplex MAC SDU: MacInst %d", inst);
      RETVALUE(RFAILED);
   }

   *txPdu = mBuf;

   RETVALUE(ROK);

}  /* rgMUXBldPdu */

#else /* else of ifndef L2_OPTMZ */

/**
 * @brief Function to add SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddSdus
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddSdus
(
Inst                inst,
RgBldPduInfo        *pdu,
Buffer              *sHdrBuf,
RgTfuDatReqTbInfo   *tb,
RgErrInfo           *err
)
#else
PRIVATE S16 rgMUXAddSdus(pdu, sHdrBuf, tb, err)
Inst                inst;
RgBldPduInfo        *pdu;
Buffer              *sHdrBuf;
RgTfuDatReqTbInfo   *tb;
RgErrInfo           *err;
#endif
{
   RgRguDDatReqPerUe  *dDatReq;
   RgRguCmnDatReq     *cDatReq;
   U32 lchIdx, pduIdx;

   TRC2(rgMUXAddSdus)

   switch(pdu->reqType)
   {
      case EVTRGUCDATREQ:
         cDatReq = (RgRguCmnDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != cDatReq)
         {
            if(rgMUXInsSdu(inst, &pdu->schdTbSz, 
                     RG_CCCH_LCID, cDatReq->pdu, 
                     sHdrBuf, NULLP, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            /* L2 Optimization for mUe/Tti: RLC pdu mbuf pointer will be passed
             * to CL it is stored in DlHqProc->TbInfo and it will be used in
             * case of harq retransmission. Store CCCH data at 0th index of
             * lch array*/
            tb->lchInfo[tb->numLch].mBuf[(tb->lchInfo[tb->numLch].numPdu)]\
               = cDatReq->pdu;
            tb->lchInfo[tb->numLch].numPdu++;
            tb->numLch++;
           RLOG3(L_INFO,"MSG4 is muxed  numLch=%ld numPdu=%ld tbaddr =%p", tb->numLch,tb->lchInfo[tb->numLch-1].numPdu, (U32)tb);
         }
         break;

      case EVTRGUDDATREQ:
         dDatReq = (RgRguDDatReqPerUe *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != dDatReq)
         {
            if(pdu->tbIndex == 1)
            {
               U16 idx1, idx2;
               /* Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[0];
             
               tb->numLch = lchIdx = 0;

               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  tb->lchInfo[lchIdx].numPdu = pduIdx = 0;

                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst, &pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, NULLP, err) != ROK)
                        {
                           RGDBGERRNEW(inst,(rgPBuf(inst), "FAILED\n"));
                           RETVALUE(RFAILED);
                        }

                        /* L2 Optimization for mUe/Tti:Increment numPdu by 1
                         * Store pdu buffer in tb to send it to CL/PHY. Increment
                         * numPdu by 1*/
                        tb->lchInfo[lchIdx].mBuf[pduIdx] = datReqTb->lchData[idx1].pdu.mBuf[idx2];

#ifdef L2_OPTMZ
                        if(datReqTb->lchData[idx1].freeBuff == FALSE)
                        {/* Not incrementing refCnt for UM Mode. */
                           tb->lchInfo[lchIdx].mBuf[pduIdx]->refCnt++;
                        }
#endif
//UDAY 
                        if(NULL != datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont)
                        {
                             Buffer *tmp;
                             tmp = datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont;
                             if(NULL == tmp->b_rptr)
                             {
                                RLOG0(L_INFO,"11111Its Null here only ");
                             }
                        }
                        else
                        {
                            RLOG0(L_INFO,"222222Its Null here only \n");
                        }
                        pduIdx++;
                        //tb->lchInfo[tb->numLch].numPdu++;
                        //tb->numLch++;

                    }
                  }
                  
                  if(pduIdx)
                  {
                    tb->lchInfo[lchIdx].numPdu = pduIdx;
                    /* If Bearer is UM then MBUF to be free by MAC layer */
                    tb->lchInfo[lchIdx].freeBuff = datReqTb->lchData[idx1].freeBuff;
                    lchIdx++;
                  }
               }
               tb->numLch = lchIdx;
            }
            else if(pdu->tbIndex == 2)
            {
               U16 idx1, idx2;
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[1];
               tb->numLch = lchIdx = 0;
         //      prc_trace_format_string(0x40,3,": AddSdus: numOfLch=%d numOfPdu=%d, schdSz=%d", datReqTb->nmbLch, datReqTb->lchData[0].pdu.numPdu, pdu->schdTbSz);
              for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                 tb->lchInfo[lchIdx].numPdu = pduIdx = 0;
                 for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(inst, &pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, NULLP, err) != ROK)
                        {
                           RGDBGERRNEW(inst,(rgPBuf(inst), "FAILED TB Size %d\n",
                                    ((S16)pdu->schdTbSz)));
                           RETVALUE(RFAILED);
                        }
                         /* L2 Optimization for mUe/Tti:Increment numPdu by 1
                         * Store pdu buffer in tb to send it to CL/PHY. Increment
                         * numPdu by 1*/
                        tb->lchInfo[lchIdx].mBuf[pduIdx] = datReqTb->lchData[idx1].pdu.mBuf[idx2];
#ifdef L2_OPTMZ
                        if(datReqTb->lchData[idx1].freeBuff == FALSE)
                        {/* Not incrementing refCnt for UM Mode. */
                           tb->lchInfo[lchIdx].mBuf[pduIdx]->refCnt++;
                        }
#endif
                        if(NULL != datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont)
                        {
                             Buffer *tmp;
                             tmp = datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont;
                             if(NULL == tmp->b_rptr)
                             {
                                RLOG0(L_INFO,"2212121Its Null here only \n");
                             }
                        }
                        else
                        {
                            RLOG0(L_INFO,"343343433ts Null here only \n");
                        }
                        pduIdx++;
                       // tb->lchInfo[tb->numLch].numPdu++;
                       // tb->numLch++;

                     }
                  }

                  if(pduIdx)
                  {
                    tb->lchInfo[lchIdx].numPdu = pduIdx;
                    /* If Bearer is UM then MBUF to be free by MAC layer */
                    tb->lchInfo[lchIdx].freeBuff = datReqTb->lchData[idx1].freeBuff;
                    lchIdx++;
                  }
               }

               tb->numLch = lchIdx;
           }
         }
         break;

      case EVTTFUTTIIND:
         break;
      default:
         break;
   } /* End of switch(reqType) */

   
   if(rgMUXAddPadd(inst, &pdu->schdTbSz, sduBuf, NULLP, FALSE, err) != ROK)
   {
      //RGDBGERRNEW((rgPBuf, "FAILED"));
      RETVALUE(RFAILED);
   }
   tb->padSize = padSize;

   RETVALUE(ROK);
}

/**
 * @brief Function to create MAC PDU from RLC SDUs and control elements, if any. 
 *
 * @details
 *
 *     Function : rgMUXBldPdu
 *     -# This function shall be invoked by Downlink Harq Module as soon as a
 *        Data request is received from RLC for a UE along with its stored 
 *        control elements to create a MAC PDU.
 *     -# It shall create subheaders for the control elements (timing advance
 *        and contention resolution ID), if given, and then shall run through
 *        all the logical channels and create subheader for each of the SDUs
 *        given on that logical channel.
 *     -# L2 Optimization for mUe/Tti: Avoiding muxing to reduce overhead of 
 *           additional Mbuf allocation memory related operation.
       -# MAC header, MAC CEs, MAC PDUs and MAC padding are stored in pre-
            allocated mBufs. These pointers will not be freed by CL    
 *     -# It shall invoke rgMUXPadPdu if the total length of the created 
 *        buffer is less than the scheduled TB size.
 *     -# At successfull operation of this function tb->macHdr, will have
 *        complete MAC Header. tb->macCes will have MAC CEs if any. tb->
 *        lchInfo[idx].mBuf[idx] will have MAC SDU per LCH per TB per UE 
 *     
 *           
 *  @param[in]  RgBldPduInfo   *bldPdu
 *  @param[out] RgTbInfo       *tb
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldPdu
(
Inst               inst,
RgBldPduInfo       *pdu,
RgTfuDatReqTbInfo  *tb,
RgErrInfo          *err
)
#else
PUBLIC S16 rgMUXBldPdu(inst, pdu, tb, err)
Inst               inst;
RgBldPduInfo       *pdu;
RgTfuDatReqTbInfo  *tb;
RgErrInfo          *err;
#endif
{
   Buffer         *mBuf1; /* MAC hearder */
   Buffer         *mBuf2; /* MAC CEs */
   //U32            lchIdx, pduIdx;

   TRC2(rgMUXBldPdu)
   
  /* Reseting macHdr and macCes pointers */
  if(tb->macHdr)
   SResetMBuf(tb->macHdr);
   if(tb->macCes)
   SResetMBuf(tb->macCes);
   
   mBuf1 = tb->macHdr; /* MAC hearder */
   mBuf2 = tb->macCes; /* MAC CEs */
   tb->tbSize  = pdu->schdTbSz;

   if(rgMUXAddCes(inst, pdu, mBuf1, mBuf2, err) != ROK)
   {
      /* Reset rPtr and wPtr to the base of data buffer(db_base)*/
      RLOG0(L_INFO,"rgMUXBldPdu: rgMUXAddCes is Failed \n");
      RG_FREE_TB(tb);
      err->errType = RGERR_MUX_BLD_PDU;
      //RGDBGERRNEW((rgPBuf, "FAILED"));
      RETVALUE(RFAILED);
   }
   if(rgMUXAddSdus(inst, pdu, mBuf1, tb, err) != ROK)
   {
      /*TODO:MP Reset rPtr and wPtr to the base of data buffer(db_base)
       * Reset numLch and numPdu to zero and set MAC SDU buf to NULLP */
      RLOG0(L_INFO, "rgMUXBldPdu: rgMUXAddSdus is Failed \n");
      RG_FREE_TB(tb);

      err->errType = RGERR_MUX_BLD_PDU;
      //RGDBGERRNEW((rgPBuf, "FAILED"));
      RETVALUE(RFAILED);
   }
// UDAY 
//      SPrntMsg(tb->macHdr, 0, 0);
//   prc_trace_format_string(0x40,3,": padSize=%ld", tb->padSize);

   tb->tbPres = TRUE;
   RETVALUE(ROK);

}  /* rgMUXBldPdu */

#endif /* end of L2_OPTMZ */

/**
 * @brief Function to create RAR PDU. 
 *
 * @details
 *
 *     Function : rgMUXBldRarPdu
 *                This function is used to build RAR PDUs and is being 
 *                invoked by the scheduler.
 *     
 *  @param[out]RgCellCb       *cellCb
 *  @param[in] RgRaRspAlloc   *bldPdu
 *  @param[in] Buffer         **txPdu 
 *  @param[out] RgErrInfo     *err
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldRarPdu
(
RgCellCb        *cell,
RgInfRaRntiInfo *alloc,
Buffer          **txPdu,
RgErrInfo       *err
)
#else
PUBLIC S16 rgMUXBldRarPdu(cell, alloc, txPdu, err)
RgCellCb        *cell;
RgInfRaRntiInfo *alloc;
Buffer          **txPdu;
RgErrInfo       *err;
#endif
{
   Buffer      *datBuf = NULLP;
   S16         ret; 
   U8          data[RG_RAR_ELEM_LEN];
   U8          hdrByte;
   MsgLen      schdTbSz;
   U8          idx;
   Inst        inst = cell->macInst - RG_INST_START;

   TRC2(rgMUXBldRarPdu)

   schdTbSz = alloc->schdTbSz;
   /* RAR PDU Requirements */
   /*
   1. SubHeader - R/T/RAPID. //5GNR, changed E to R
   2. TA ( if applicable)
   3. Ul Grant:
         a. Hopping Flag - 1 Bit.
         b. Fixed Size RB Assignment. - 10 Bits.
         c. Truncated Modulation and coding scheme - 4 Bits.
         d. TPC command for scheduled PUSCH. - 3 Bits.
         e. UL Delay - 1 Bit.
         f. CQI Request - 1 Bit.
   4. Temporary Crnti.
   */

   /* Initialize the error type */
   err->errType = RGERR_MUX_BLD_RAR_PDU;
  
   if ((ret = rgGetMsg(inst,&datBuf)) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "FAILED to getMsg");
      RETVALUE(RFAILED);
   }

   if (TRUE == alloc->backOffInd.pres)
   {
      /*Set T Bit , NO E bit Required */
      hdrByte = 0x00;
      /* Add the bi */
      hdrByte |= (0x0F & (alloc->backOffInd.val));

      /* Add the header */
      RG_PACK_RAR_SHDR(hdrByte, datBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_BI_FAIL;
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"RGERR_MUX_BLD_BI_FAIL");
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
      schdTbSz--;
   }

   for (idx=0; idx < (alloc->numCrnti) && 
         (schdTbSz >= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN); idx++)
   {
      /* Add the tBit */
      hdrByte = 0x40; 
      /* Add the rapId */
      hdrByte |= (0x3F & (alloc->crntiInfo[idx].rapId));

         /* Add the header */
         RG_PACK_RAR_SHDR(hdrByte, datBuf, ret);
         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_RAPIDHDR_FAIL;
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"RGERR_MUX_BLD_RAPIDHDR_FAIL");
            RG_FREE_MSG(datBuf);
            RETVALUE(RFAILED);
         }

      /* Prepare the data */
      data[0]  =  0x7F & ((alloc->crntiInfo[idx].ta.val) >> 4);
      data[1] = 0;
      data[2] = 0;
      data[3] = 0;
      {    
         rgMUXGet20bitRarGrnt(cell->bwCfg.ulTotalBw, &(alloc->crntiInfo[idx].grnt), &data[1]);
      }
      data[1] |=  ((U8)((alloc->crntiInfo[idx].ta.val) << 4));
      data[4]  =  (alloc->crntiInfo[idx].tmpCrnti) >> 8;
      data[5]  =  (U8) (alloc->crntiInfo[idx].tmpCrnti);

      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
         		"Rar,Rapid=%d, Temp CRNTI:%d", 
                alloc->crntiInfo[idx].rapId,
                alloc->crntiInfo[idx].tmpCrnti);
     MS_BUF_ADD_ALLOC_CALLER();
      if(SAddPstMsgMult(&data[0], RG_RAR_ELEM_LEN, datBuf) != ROK)
      {
         err->errCause = RGERR_MUX_BLD_RAPID_FAIL;
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"RGERR_MUX_BLD_RAPID_FAIL");
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
      schdTbSz -= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN;
   }

   if(rgMUXAddPadd(inst,&schdTbSz, datBuf, TRUE, err) != ROK)
   {
      RG_FREE_MSG(datBuf);
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"FAILED to mux add padding");
      RETVALUE(RFAILED);
   }

   *txPdu = datBuf;

   RETVALUE(ROK);
} /* rgMUXBldRarPdu */

/***********************************************************
 *
 *     Func : rgMUXGet20bitRarGrnt
 *
 *     Desc : This function fills up the 20-bit grant
 *            for RA response.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgMUXGet20bitRarGrnt
(
U8             ulBw,
RgInfRarUlGrnt *msg3Grnt,
U8             *grnt
)
#else
PRIVATE Void rgMUXGet20bitRarGrnt(ulBw, msg3Grnt, grnt)
U8             ulBw;
RgInfRarUlGrnt *msg3Grnt;
U8             *grnt;
#endif
{
   U16       riv = rgMUXCalcRiv(ulBw, msg3Grnt->rbStart, msg3Grnt->numRb);

   TRC2(rgMUXGet20bitRarGrnt);

   grnt[2]  = msg3Grnt->cqiBit;   /* cqi bit is 0, output from sched */
   grnt[2] |= (msg3Grnt->delayBit << 1);
   grnt[2] |= (msg3Grnt->tpc << 2);
   grnt[2] |= (msg3Grnt->iMcsCrnt << 5);

   grnt[1]  = (msg3Grnt->iMcsCrnt >> 3);
   /* Forcing right shift to insert 0 as the LSB: 
    * since this is assumed in the computation */
   grnt[1] |= (U8)((riv << 1) & 0xFE);

   grnt[0]  = (U8)((riv >> 7) & 0x07);
   grnt[0] |= ((msg3Grnt->hop & 0x01) << 3);

   RETVOID;
} /* rgMUXGet20bitRarGrnt */

/***********************************************************
 *
 *     Func : rgMUXCalcRiv
 *
 *     Desc : This function calculates RIV.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC U16 rgMUXCalcRiv
(
U8           bw,
U8           rbStart,
U8           numRb
)
#else
PUBLIC U16 rgMUXCalcRiv(bw, rbStart, numRb)
U8           bw;
U8           rbStart;
U8           numRb;
#endif
{
   U8           numRbMinus1 = numRb - 1;
   U16          riv;

   TRC2(rgMUXCalcRiv);

   if (numRbMinus1 <= bw/2)
   {
      riv = bw * numRbMinus1 + rbStart;
   }
   else
   {
      riv = bw * (bw - numRbMinus1) + (bw - rbStart - 1);
   }
   RETVALUE(riv);
} /* rgMUXCalcRiv */



/**********************************************************************
 
         End of file
**********************************************************************/
