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
#define KW_DL_INST 1
#define KW_MAX_RNG_DEQ_INDX 8

/* Below are the status used within RLC */
#define KW_SDU_OPAT_NOUPDATE 0
#define KW_SDU_OPAT_UPD_TX 1
#define KW_SDU_OPAT_UPD_RETX   2

/* Below are the status which needs to be updated to MAC */
#define KW_RGU_SDU_OPAT_NOUPDATE 0
#define KW_RGU_SDU_OPAT_UPDATE   1
#define KW_RGU_SDU_OPAT_BREACH   2

/* PDB Values for various QCI bearers : This value is considered 
   after taking into account a 20ms delay at the network */
#define KW_PDB_VALUE_FOR_QCI1    80
#define KW_PDB_VALUE_FOR_QCI2    130
#define KW_PDB_VALUE_FOR_QCI3    30
#define KW_PDB_VALUE_FOR_QCI4    280
#define KW_PDB_VALUE_FOR_QCI5    80
#define KW_PDB_VALUE_FOR_QCI6    280
#define KW_PDB_VALUE_FOR_QCI7    80
#define KW_PDB_VALUE_FOR_QCI8    280
#define KW_PDB_VALUE_FOR_QCI9    280

#define KW_DATA_BITMASK          0x80 /* Data bitmask is used to fill D/C bit for data*/
/**
 * @def KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB
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
#define KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci]))

#define KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
      (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_IP) && \
      (_rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_IP))

#define KW_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_DELAY))

#define KW_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_UU_LOSS))
#define KW_UPD_PDCP_L2_DLDELAY_STS(_kwCb, _kwRbCb, _delay)                     \
{                                                                              \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                               \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DELAY))    \
   {                                                                           \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DELAY]->dlPjSduDelay.sduDelay += _delay; \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DELAY]->dlPjSduDelay.numSdus++; \
   }                                                                    \
}

#define KW_UPD_L2_UU_LOSS_PKTS(_kwCb, _kwRbCb, _val)                         \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_UU_LOSS]->uuLoss.dLoss += _val;     \
   }                                                              \
}
#define KW_UPD_L2_UU_LOSS_POS_PKTS(_kwCb,_kwRbCb,_val)                         \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_UU_LOSS]->uuLoss.posPkts += _val;     \
   }                                                              \
}
/* Discard new changes starts */
#define KW_UPD_L2_DL_DISC_SDU_STS(_kwCb,_kwRbCb)                         \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DISC))              \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DISC]->dlDisc.discSdus++;     \
   }                                                              \
}                                              

#define KW_UPD_L2_DL_TOT_SDU_STS(_kwCb,_kwRbCb)                             \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DISC]->dlDisc.totSdus++;      \
   }                                                              \
}

#define KW_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_kwCb, _kwRbCb)                 \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                    \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      U32 idx1;                                                    \
      for (idx1 = 0; idx1 < LKW_MAX_L2MEAS; idx1++)                  \
      {                                                           \
         if(_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.measType & LKW_L2MEAS_DL_DISC)                 \
         {                                                        \
            if(_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci].totDrbsPerQci > 0) \
            {                                                                                               \
               _kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci].totDrbsPerQci--;   \
               if (_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci].totDrbsPerQci == 0) \
               {                                                                                                 \
                  _kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.qci[(_kwRbCb)->qci] = 0;                      \
                  cmMemset((U8 *)&_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci], 0,  \
                     sizeof(_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci]));            \
               }                                                                                                 \
            }                                                                                                    \
         }                                                                                                       \
      }                                                                                                          \
   }                                                                                                             \
}
#else
#define KW_UPD_PDCP_L2_DLDELAY_STS(_kwCb, _kwRbCb, _delay)
#define KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define KW_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define KW_UPD_L2_DL_DISC_SDU_STS(_kwCb,_kwRbCb)
#define KW_UPD_L2_DL_TOT_SDU_STS(_kwCb, _kwRbCb)
#define KW_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_kwCb, _kwRbCb)
#endif


#endif /* __KW_DLH__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
