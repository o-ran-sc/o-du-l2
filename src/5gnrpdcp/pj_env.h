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

/********************************************************************20**
  
        Name:     LTE PDCP file 
    
        Type:     H include file
  
        Desc:     This file contain the hash definations for PDCP 
 
        File:     pj_env.h

*********************************************************************21*/
/** @file pj_env.h
@brief PDCP Hash definitions
*/

#ifndef __PJENVT2KH__
#define __PJENVT2KH__

#define PJ_MAX_PDCP_INSTANCES            2 
#define PJ_FWD_MAX_SDU_CNT            1 

#define PJ_ALLOC_BUF(gCb,_buf)                                 \
{                                                          \
   if (SGetMsg(gCb->init.region, gCb->init.pool, (Buffer **)&_buf) != ROK) \
   {                                                       \
      (_buf) = NULLP;                                      \
   }                                                       \
}

#define PJ_CPY_MSG(_cb,_x, _y, _ret)                           \
   _ret =(SAddMsgRef((_x), PJ_GET_MEM_REGION(_cb), PJ_GET_MEM_POOL(_cb), (_y)))                                                                                         

#define PJ_SND_CFM(_cb,_pjRbCb, _txEnt, _cause)                \
{                                                          \
   PjuDatCfmInfo *_datCfm;                                 \
   CmLtePdcpId *_pdcpId;                                   \
   PjPjuSapCb  *_pjuSap;                                   \
   if (_pjRbCb->rbType == PJ_SRB)                          \
      _pjuSap   = &(_cb->u.dlCb->pjuSap[PJ_SRB_SAP]);                  \
   else                                                    \
      _pjuSap   = &(_cb->u.dlCb->pjuSap[PJ_DRB_SAP]);                  \
                                                           \
   PJ_ALLOC(_cb, _datCfm, sizeof(PjuDatCfmInfo));              \
   PJ_ALLOC(_cb,_pdcpId, sizeof(CmLtePdcpId));                \
   if ( _datCfm != NULLP && _pdcpId != NULLP )             \
   {                                                       \
      _datCfm->cfmSta[_datCfm->numSdus].sduId = _txEnt->sduId;\
      _datCfm->cfmSta[_datCfm->numSdus].status = _cause;   \
      _datCfm->numSdus++;                                  \
      _pdcpId->rbId   = _pjRbCb->rbId;                     \
      _pdcpId->rbType = _pjRbCb->rbType;                   \
      _pdcpId->ueId   = _pjRbCb->ueCb->key.ueId;           \
      _pdcpId->cellId = _pjRbCb->ueCb->key.cellId;         \
      PjUiPjuDatCfm(&(_pjuSap->pst), _pjuSap->suId, _pdcpId, _datCfm);\
   }                                                       \
   pjDbmDelTxEnt(_cb, &(pjRbCb->dlCb.txBuf), _txEnt->count);   \
}

#define PJ_ADD_PRE_MSG(_pdu, _hdr, _numBytes, _ret)        \
{                                                          \
   Data _liByte;                                           \
   U16  _bytes;                                            \
   _bytes = 0;                                             \
   while (_numBytes > _bytes )                             \
   {                                                       \
      _liByte = (U8) (_hdr >> (8*_bytes));                 \
      _ret = SAddPreMsg((Data )_liByte, _pdu);             \
      if ( _ret != ROK )                                   \
      {                                                    \
         break;                                            \
      }                                                    \
      _bytes++;                                            \
   }                                                       \
}
#endif /* __PJENVT2KH__ */
/********************************************************************30**
  
         End of file
**********************************************************************/
