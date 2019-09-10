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
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for PDCP
 
        File:     pj_ul.h
  
*********************************************************************21*/
/** @file pj_ul.h
@brief PDCP Hash definitions
*/

#ifndef __PJ_ULH__
#define __PJ_ULH__
#define PJ_MAX_NUM_OF_BITS 4096

#define PJ_DBM_FETCH_UL_RBCB(_rbId, _rbType, _ueCb, _rbCb)     \
{                                                           \
   _rbCb = (_rbType==CM_LTE_SRB)?_ueCb->srbCb[_rbId]:_ueCb->drbCb[_rbId];\
}

/* HO_FIX: Changed the success value to ROK as RRC expects
           ROK or RFAILED */
#define  PJ_FILL_UL_SDU_CFM_INFO(_cb,_cfmInfo,_tempDlStaInfo,_ueCb)                    \
{                                                                 \
   U8 _rbCnt;                                                     \
   U8 _numRb = 0;                                                 \
   PjUlHoCfmInfo *_hoCfmInfo;                                       \
   CpjSduStaInfo  *_staInfo;                                      \
   Bool _rbPres;                                                  \
   _cfmInfo->ueId    = _ueCb->key.ueId;                           \
   _cfmInfo->cellId  = _ueCb->key.cellId;                         \
   _cfmInfo->transId = _ueCb->hoInfo->transId;                    \
   for(_rbCnt = 0; _rbCnt < PJ_MAX_DRB_PER_UE && _numRb < CPJ_MAX_DRB; _rbCnt++)          \
   {                                                              \
       _rbPres     = FALSE;                                       \
      _hoCfmInfo   =  &_ueCb->hoInfo->hoCfmInfo[_rbCnt];          \
      _staInfo     =  &_cfmInfo->sduStaInfo[_numRb];              \
      if(_hoCfmInfo->pres == TRUE)                                \
      {                                                           \
      _rbPres = TRUE;                                             \
      _staInfo->rbId  =  _hoCfmInfo->rbId;                        \
      _staInfo->dir   |= _hoCfmInfo->dir;                         \
      _staInfo->ulSduStaInfo.numBits   =  _hoCfmInfo->numBits; \
      _staInfo->ulSduStaInfo.ulBitMap  =  _hoCfmInfo->ulBitMap;\
      _staInfo->ulSduStaInfo.count     =  _hoCfmInfo->count;   \
      _hoCfmInfo->ulBitMap          =  NULLP;                  \
      }                                                        \
      if(tempDlStaInfo[_rbCnt].dlSduStaInfo.hoPres == TRUE)  \
      {                                                                \
       _rbPres = TRUE;                                                 \
       _staInfo->rbId  =  _tempDlStaInfo[_rbCnt].rbId;        \
       _staInfo->dir   |= _tempDlStaInfo[_rbCnt].dir;                         \
       _staInfo->dlSduStaInfo.count = _tempDlStaInfo[_rbCnt].dlSduStaInfo.count; \
      }                                                        \
      if(_rbPres != TRUE)                                      \
      {                                                        \
         continue;                                             \
      }                                                        \
      else                                                     \
      {                                                        \
        _numRb++;                                              \
      }                                                        \
  }                                                              \
   _cfmInfo->numRb = _numRb;                                   \
   _cfmInfo->status = ROK;                                     \
   _cfmInfo->reason = CPJ_CFG_REAS_NONE;                          \
   PJ_FREE(_cb,_ueCb->hoInfo->hoCfmInfo,                          \
              (PJ_MAX_DRB_PER_UE * sizeof(PjUlHoCfmInfo)));       \
   PJ_FREE(_cb,_ueCb->hoInfo, sizeof(PjUlHoInfo));                \
}

#define PJ_DBM_DELETE_UL_RBCB(_cb, _rbCb)                           \
{                                                           \
   /* Free memory for rbCb */                               \
   PJ_FREE(_cb, _rbCb, sizeof(PjUlRbCb));                            \
   PJ_LMM_RB_STS_DEC(_cb);                                     \
}

#endif /* __PJH__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
