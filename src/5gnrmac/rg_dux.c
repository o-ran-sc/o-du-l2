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
  
     File:    rg_dux.c 
  
**********************************************************************/

/** @file rg_dux.c
@brief This module handles de-multiplexing of the data recieved at MAC.
*/

/* header include files -- defines (.h) */
#include "common_def.h"
  
#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* CRG Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */

#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* CRG Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "rg_sch_inf.x"    /* SCH Interface includes */
#include "rg_prg.x"        /* PRG interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC includes */

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

#define RG_DUX_ALLOC(_pdu, _size, _dataPtr, _ret) {\
   _ret = cmGetMem((Ptr)&_pdu->memCp, _size, (Ptr *)&_dataPtr); \
}

#define RG_INIT_SDU(_sdu, _lcId, _len) {\
   (_sdu)->lcId = (_lcId); \
   (_sdu)->len = (_len); \
   (_sdu)->mBuf = NULLP; \
   (_sdu)->sduLstEnt.next = NULLP; \
   (_sdu)->sduLstEnt.prev = NULLP; \
   (_sdu)->sduLstEnt.node = (PTR)(_sdu); \
}

#define RG_EXT_BS(_bsr, _bs1, _bs2, _bs3, _bs4)  {\
   _bs1 = _bsr[0] >> 2; \
   _bs2 = (((_bsr[0] & 0x3) << 4) | (_bsr[1] >> 4)); \
   _bs3 = (((_bsr[1] & 0x0F) << 2) | (_bsr[2] >> 6)); \
   _bs4 = _bsr[2] & 0x3F; \
}

#define RG_UNPACK_LONG_BSR(_bsr, _mBuf, _ret) {\
   _ret = SRemPreMsgMult((_bsr), 3, (_mBuf)); \
}

#define RG_UNPACK_SHORT_BSR(_bsr, _mBuf, _ret) {\
   _ret = oduPackUInt8((_bsr), (_mBuf)); \
}

#define RG_UNPACK_TRUNC_BSR(_bsr, _mBuf, _ret) {\
   _ret = oduPackUInt8((_bsr), (_mBuf)); \
}

#define RG_UNPACK_PHR(_phr, _mBuf, _ret) {\
   _ret = oduPackUInt8((_phr), (_mBuf)); \
}

#define RG_UNPACK_CRNTI(_rnti, _mBuf, _ret) {\
   Data _unpkArray[2];\
   *_rnti = 0;\
   _ret = SRemPreMsgMult(_unpkArray, (MsgLen) 2, _mBuf);\
   if (_ret == ROK)\
   {\
      *_rnti = (uint16_t) PutHiByte(*_rnti, (uint8_t) _unpkArray[0]);\
      *_rnti = (uint16_t) PutLoByte(*_rnti, (uint8_t) _unpkArray[1]);\
   }\
}

/* For EXT PHR DEMUX */
#define RG_UNPACK_EXT_PHR_CI(_ci, _mBuf, _ret) {\
   _ret = oduPackUInt8((_ci), (_mBuf)); \
}

#define RG_UNPACK_EXT_PHR(_extPhr, _mBuf, _ret) {\
   _ret = oduPackUInt8((_extPhr), (_mBuf)); \
}



/**
 * @brief Handles the insertion of SDU in to PDU.
 *
 * @details
 *
 *     Function: rgDUXInsSdu
 *     
 *     This API handles the insertion of SDU in to PDU.
 *     
 *     Processing Steps: 
 *      - Append the sdu to the sduLst of pdu.
 *
 *  @param[in]  Inst        inst
 *  @param[out] *pdu
 *  @param[in]  lcId
 *  @param[in]  sduLen
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgDUXInsSdu(Inst inst, RgMacPdu *pdu, RgMacSdu **sdu, uint8_t lcId, uint16_t sduLen, RgErrInfo *err)
{
   S16         ret;
   RgMacSdu    *sduAloc = NULLP;


   RG_DUX_ALLOC(pdu, sizeof(RgMacSdu), sduAloc, ret);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Allocation of RgSubHdr failed for LCID:%d",lcId);
      err->errCause = RGERR_DUX_MEM_EXHAUST;
      return RFAILED;
   }
   *sdu = sduAloc; 
   RG_INIT_SDU(sduAloc, lcId, sduLen);
   cmLListAdd2Tail(&pdu->sduLst, &sduAloc->sduLstEnt);
   return ROK;
}

/**
 * @brief Handles extracting the CE sub headers from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtSubHdr
 *     
 *     This API handles extracting the  sub headers from the MAC PDU.
 *     
 *     Processing Steps: 
 *      - Extract the each sub header.
 *
 *  @param[in]  Inst        inst
 *  @param[out]  *pdu
 *  @param[in]  *mBuf
 *  @param[out] *lcId
 *  @param[out] *len
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgDUXExtSubHdr(Inst inst, RgMacPdu *pdu, Buffer *mBuf, uint8_t *lcId, uint16_t *len, RgErrInfo *err)
{
   uint8_t             byte;
   uint8_t             fmt=0;
 

   *len = 0;   
   if(oduPackUInt8(&byte,mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : oduPackUInt8 failed");
      err->errCause = RGERR_DUX_UNPACK_FAILURE;
      return RFAILED;
   }
   /* Extract the lcid */
   RG_EXT_LCID(*lcId, byte);

   /*note: RG_EXT_PHR_LCID currently not considered */
   if(*lcId <= RG_DEDLC_MAX_LCID)
   {  /* variable size MAC Sub PDU */
      RG_EXT_FORMT_BIT(fmt,byte);
      if(oduPackUInt8(&byte, mBuf) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : oduPackUInt8 failed");
         err->errCause = RGERR_DUX_UNPACK_FAILURE;
         return RFAILED;
      }
      *len = byte;
      if(fmt)
      {
         if(oduPackUInt8(&byte,mBuf) != ROK)
         {
            DU_LOG("\nERROR  -->  MAC : oduPackUInt8 failed");
            err->errCause = RGERR_DUX_UNPACK_FAILURE;
            return RFAILED;
         }
         *len = (*len << 8) | byte;
      }
   }
   return ROK;
} /* rgDUXExtSubHdr */

/**
 * @brief Handles extracting the CEs from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtCe
 *     
 *     This API handles extracting the CEs from the MAC PDU.
 *     
 *     Processing Steps: 
 *      - Based on the ce sub header extract the ce.
 *
 *  @param[in]  Inst        inst
 *  @param[in,out]  *pdu
 *  @param[out] *ceInfo
 *  @param[in]  *mBuf
 *  @param[in] lcId
 *  @param[in] subPduLen
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgDUXExtCe(Inst inst, RgMacPdu *pdu, RgInfCeInfo *ceInfo, Buffer *mBuf, uint8_t lcId, uint16_t subPduLen,\
RgErrInfo *err)
{
   S16            ret;


   switch(lcId)
   {
      case RG_EXT_PHR_LCID:
         {
            uint8_t Ci;
            uint8_t sCellIdx;
            uint8_t extPhrOctet;
            uint8_t extPhrPCmax;
            RgInfExtPhrCEInfo *extPhr;

            RG_UNPACK_EXT_PHR_CI(&Ci,mBuf,ret);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : Unpacking of EXT PHR failed LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               return RFAILED;
            }

            /* Not handling Type 2 PHR report as simultaneous PUSCH/PUCCH
               is not supported as of now */
            extPhr = &ceInfo->ces.extPhr;
            extPhr->numServCells = 0;

            /* Setting first BIT as PCELL field even though reserved is always
               reported by UE */
            Ci |= 0x1;
            for (sCellIdx = 0; (Ci && sCellIdx < CM_LTE_MAX_CELLS); sCellIdx++)
            {
               if (Ci & 0x1)
               {
                  extPhr->servCellPhr[extPhr->numServCells].sCellIdx = sCellIdx;
                  RG_UNPACK_EXT_PHR(&extPhrOctet,mBuf,ret);
                  if(ret != ROK)
                  {
                     DU_LOG("\nERROR  -->  MAC : Unpacking of EXT PHR failed LCID:%d",lcId);
                     err->errCause = RGERR_DUX_UNPACK_FAILURE;
                     return RFAILED;
                  }

                  /* extPhrOctet: Bits : 7 6 5 4 3 2 1 0
                   *                     P V x x x x x x
                   *                         <6x Bit phr>
                   */
                  /* P : P Back off applied or not */
                  extPhr->servCellPhr[extPhr->numServCells].phr = (extPhrOctet & 0x3F);
                  extPhr->servCellPhr[extPhr->numServCells].pBackOff = 
                     ((extPhrOctet >> 7) & 0x01);

                  /* V: Virtual PCMAX or Real Tx PCMAX */
                  if (extPhrOctet & 0x40)
                  {
                     extPhr->servCellPhr[extPhr->numServCells].pCmax = RG_REF_PCMAX;
                  }
                  else
                  {
                     RG_UNPACK_EXT_PHR(&extPhrPCmax,mBuf,ret);
                     if(ret != ROK)
                     {
                        DU_LOG("\nERROR  -->  MAC : Unpacking of EXT PHR failed LCID:%d",lcId);
                        err->errCause = RGERR_DUX_UNPACK_FAILURE;
                        return RFAILED;
                     }
                     extPhr->servCellPhr[extPhr->numServCells].pCmax = (extPhrPCmax & 0x3F);
                  }
                  extPhr->numServCells++;
               }
               Ci >>= 1;
            }

            ceInfo->bitMask |= RG_EXT_PHR_CE_PRSNT;
         }
         break;

      case RG_PHR_LCID:
         {
            RG_UNPACK_PHR(&ceInfo->ces.phr,mBuf,ret);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : Unpacking of PHR failed LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               return RFAILED;
            }
            ceInfo->bitMask |= RG_PHR_CE_PRSNT;
         }
         break;
      case RG_TRUNC_BSR_LCID:
         {
            RG_UNPACK_TRUNC_BSR(&ceInfo->ces.bsr.truncBsr,mBuf,ret);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : Unpacking of Trunc BSR failed LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               return RFAILED;
            }
            ceInfo->bitMask |= RG_TRUNC_BSR_CE_PRSNT;
         }
         break;
      case RG_SHORT_BSR_LCID:
         {
            RG_UNPACK_SHORT_BSR(&ceInfo->ces.bsr.shortBsr,mBuf,ret);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : Unpacking of Short BSR failed LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               return RFAILED;
            }
            ceInfo->bitMask |= RG_SHORT_BSR_CE_PRSNT;
         }
         break;
      case RG_LONG_BSR_LCID:
         {
            uint8_t longBsr[3] = {0}; /* KW_FIXX */
            RG_UNPACK_LONG_BSR(longBsr,mBuf,ret);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : Unpacking of Long BSR failed LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               return RFAILED;
            }
            RG_EXT_BS(longBsr, 
                  ceInfo->ces.bsr.longBsr.bs1, 
                  ceInfo->ces.bsr.longBsr.bs2, 
                  ceInfo->ces.bsr.longBsr.bs3, 
                  ceInfo->ces.bsr.longBsr.bs4);
            ceInfo->bitMask |= RG_LONG_BSR_CE_PRSNT;
         }
         break;
      case RG_CRNTI_LCID:
         {
            RG_UNPACK_CRNTI(&ceInfo->ces.cRnti,mBuf,ret);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : Unpacking of C-RNTI failed LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               return RFAILED;
            }
            ceInfo->bitMask |= RG_CRNTI_CE_PRSNT;
         }
         break;
      default:
         DU_LOG("\nERROR  -->  MAC : Invalid LCID:%u received",lcId); 
         err->errCause = RGERR_DUX_INV_LCID_RX;
         return RFAILED;
   }
   return ROK;
} /* rgDUXExtCe  */


/**
 * @brief Handles extracting the SDU from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtSdu
 *     
 *     This API handles extracting the SDU corresponding to a logical channel.
 *     
 *     Processing Steps: 
 *      - Based on the length stored in the sub header extract the SDU.
 *
 *  @param[in]  Inst        inst
 *  @param[in,out]  *pdu
 *  @param[out] *ceInfo
 *  @param[in]  *mBuf
 *  @param[in] lcId
 *  @param[in] subPduLen
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgDUXExtSdu(Inst inst, RgMacPdu *pdu, RgInfCeInfo *ceInfo, Buffer **mBuf, uint8_t lcId, uint16_t subPduLen,\
RgErrInfo *err)
{
   S16         ret;
   Buffer      *tmpBuf1;
   Buffer      *tmpBuf2 = NULLP;
   RgMacSdu    *sdu;


   if(lcId == RG_CCCH_LCID)
   {
      ceInfo->bitMask |= RG_CCCH_SDU_PRSNT;
   }

   if(rgDUXInsSdu(inst,pdu, &sdu,lcId, subPduLen, err) != ROK)
   {
      RG_FREE_MSG(*mBuf);
      return RFAILED;
   }

   tmpBuf1 = *mBuf;
   {
      ret = SSegMsg(tmpBuf1,subPduLen,&tmpBuf2);
      if((ret != ROK) && (!((ret == ROKDNA) )))
      {
         RG_FREE_MSG(tmpBuf1);
         DU_LOG("\nERROR  -->  MAC : SSegMsg failed");
         err->errCause = RGERR_DUX_RLC_PDU_CREAT_FAIL;
         return RFAILED;
      }
      sdu->mBuf = tmpBuf1;
      *mBuf = tmpBuf2;
   }
   return ROK;
}   /* rgDUXExtSdu */

/**
 * @brief Handles de-multiplexing of the data recieved at MAC.
 *
 * @details
 *
 *     Function: rgDUXDemuxData
 *     
 *     This API handles de-multiplexing of the data recieved at MAC.
 *     
 *     Invoked by: rgTOMTfuDatInd of TOM 
 *     
 *     Processing Steps: 
 *      - De-multiplex the mBuf
 *
 *  @param[in]  Inst        inst
 *  @param[in]  *pdu 
 *  @param[in]  *mBuf 
 *  @param[out] *err 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgDUXDemuxData(Inst inst, RgMacPdu  *pdu, RgInfCeInfo *ceInfo, Buffer **mBuf, RgErrInfo *err)
{
   uint8_t     lcId;
   uint16_t    subPduLen;
   MsgLen      len;  

   ceInfo->bitMask = 0x0000;

   /* Initialize the sdu list */
   cmLListInit(&pdu->sduLst);

   if(*mBuf == NULLP) 
   {
      DU_LOG("\nERROR  -->  MAC : Null Buffer Recevived");
      return RFAILED;
   }
   do
   {
      /* UL Message format  order : 
           PduSubHdr+SubPDU,PduSubHdr+SubPDU,...CeSubHdr+Ce,CeSubPdu+Ce,...,PADSubHdr+PAD */
      /* Extract the Sub headers */
      if(rgDUXExtSubHdr(inst,pdu, *mBuf, &lcId, 
               &subPduLen, err) != ROK)
      {
         RG_FREE_MSG(*mBuf);	      
         DU_LOG("\nERROR  -->  MAC : Failed to extract pad sub headers");
         return RFAILED;
      }
      if(lcId == RG_PAD_LCID)
      { /*at end of MAC PDU,  Padding started */ 
         RG_FREE_MSG(*mBuf);	      
         return ROK;
      }
      if(lcId <= RG_DEDLC_MAX_LCID)
      {
         /* Extract the sdus */
         if(rgDUXExtSdu(inst,pdu,ceInfo, mBuf,lcId,subPduLen, err) != ROK)
         {
            /* Fix : syed rgDUXExtSdu would have segmented mBuf and hence
             * will be responsible for freeing mBuf */
            *mBuf = NULLP;
            DU_LOG("\nERROR  -->  MAC : failed to Extract the sdus");
            return RFAILED;
         }
         if(*mBuf == NULLP) /* if message read completes then return */
         {
            return ROK;
         }
      }
      else
      {
         /* Extract the ces */
         if(rgDUXExtCe(inst,pdu,ceInfo,*mBuf, lcId,subPduLen, err) != ROK)
         {
            RG_FREE_MSG(*mBuf);	      
            DU_LOG("\nERROR  -->  MAC : failed to Extract the ces");
            return RFAILED;
         }
      }
      if(SFndLenMsg(*mBuf,&len) != ROK)
      {
         RG_FREE_MSG(*mBuf);
         DU_LOG("\nERROR  -->  MAC : mBuf length check failed");
         err->errCause = RGERR_DUX_UNPACK_FAILURE;
         return RFAILED;
      }
   }while(len);

   RG_FREE_MSG(*mBuf);
   return ROK;
}  /* rgDUXDemuxData */

/**********************************************************************
 
         End of file
**********************************************************************/
