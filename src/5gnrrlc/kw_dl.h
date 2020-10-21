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
  
        Name:     LTE RLC file for downlink and non real time tasks 
    
        Type:     C include file
  
        Desc:     This file contains helper macros for RLC downlink
                  and non real time task
 
        File:     kw_dl.h
  
*********************************************************************21*/
/** @file kw_dl.h
@brief RLC Downlink helper macros 
*/

#ifndef __KW_DLH__
#define __KW_DLH__
#define RLC_DL_INST 1
#define RLC_MAX_RNG_DEQ_INDX 8

/* Below are the status used within RLC */
#define RLC_SDU_OPAT_NOUPDATE 0
#define RLC_SDU_OPAT_UPD_TX 1
#define RLC_SDU_OPAT_UPD_RETX   2

/* Below are the status which needs to be updated to MAC */
#define RLC_RGU_SDU_OPAT_NOUPDATE 0
#define RLC_RGU_SDU_OPAT_UPDATE   1
#define RLC_RGU_SDU_OPAT_BREACH   2

/* PDB Values for various QCI bearers : This value is considered 
   after taking into account a 20ms delay at the network */
#define RLC_PDB_VALUE_FOR_QCI1    80
#define RLC_PDB_VALUE_FOR_QCI2    130
#define RLC_PDB_VALUE_FOR_QCI3    30
#define RLC_PDB_VALUE_FOR_QCI4    280
#define RLC_PDB_VALUE_FOR_QCI5    80
#define RLC_PDB_VALUE_FOR_QCI6    280
#define RLC_PDB_VALUE_FOR_QCI7    80
#define RLC_PDB_VALUE_FOR_QCI8    280
#define RLC_PDB_VALUE_FOR_QCI9    280

#define RLC_DATA_BITMASK          0x80 /* Data bitmask is used to fill D/C bit for data*/
/**
 * @def RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB
 *
 *    This macro is used to check if DL IP throughput measurement is ON
 *    or off for the passed rb
 *
 *    Returns TRUE (non-zero) if measurement is ON else FALSE (zero)
 *
 * @param[in] _gCb     RLC DL Cb
 * @param[in] _rbCb    RLC downlink control block
 *
*/ 
#ifdef LTE_L2_MEAS
#define RLC_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci]))

#define RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
      (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_IP) && \
      (_rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_IP))

#define RLC_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_DELAY))

#define RLC_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_UU_LOSS))
#define RLC_UPD_PDCP_L2_DLDELAY_STS(_rlcCb, _rlcRbCb, _delay)                     \
{                                                                              \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                               \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DELAY))    \
   {                                                                           \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DELAY]->dlPjSduDelay.sduDelay += _delay; \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DELAY]->dlPjSduDelay.numSdus++; \
   }                                                                    \
}

#define RLC_UPD_L2_UU_LOSS_PKTS(_rlcCb, _rlcRbCb, _val)                         \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_UU_LOSS]->uuLoss.dLoss += _val;     \
   }                                                              \
}
#define RLC_UPD_L2_UU_LOSS_POS_PKTS(_rlcCb,_rlcRbCb,_val)                         \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_UU_LOSS]->uuLoss.posPkts += _val;     \
   }                                                              \
}
/* Discard new changes starts */
#define RLC_UPD_L2_DL_DISC_SDU_STS(_rlcCb,_rlcRbCb)                         \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DISC))              \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DISC]->dlDisc.discSdus++;     \
   }                                                              \
}                                              

#define RLC_UPD_L2_DL_TOT_SDU_STS(_rlcCb,_rlcRbCb)                             \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DISC]->dlDisc.totSdus++;      \
   }                                                              \
}

#define RLC_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_rlcCb, _rlcRbCb)                 \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                    \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      U32 idx1;                                                    \
      for (idx1 = 0; idx1 < LKW_MAX_L2MEAS; idx1++)                  \
      {                                                           \
         if(_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.measType & LKW_L2MEAS_DL_DISC)                 \
         {                                                        \
            if(_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci].totDrbsPerQci > 0) \
            {                                                                                               \
               _rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci].totDrbsPerQci--;   \
               if (_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci].totDrbsPerQci == 0) \
               {                                                                                                 \
                  _rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.qci[(_rlcRbCb)->qci] = 0;                      \
                  memset(&_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci], 0,  \
                     sizeof(_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci]));            \
               }                                                                                                 \
            }                                                                                                    \
         }                                                                                                       \
      }                                                                                                          \
   }                                                                                                             \
}
#else
#define RLC_UPD_PDCP_L2_DLDELAY_STS(_rlcCb, _rlcRbCb, _delay)
#define RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define RLC_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define RLC_UPD_L2_DL_DISC_SDU_STS(_rlcCb,_rlcRbCb)
#define RLC_UPD_L2_DL_TOT_SDU_STS(_rlcCb, _rlcRbCb)
#define RLC_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_rlcCb, _rlcRbCb)
#endif


#endif /* __KW_DLH__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
