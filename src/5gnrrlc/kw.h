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
  
        Name:     LTE RLC file 
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for RLC 
 
        File:     kw.h
  
*********************************************************************21*/
/** @file kw.h
@brief RLC Hash definitions
*/

#ifndef __KWH__
#define __KWH__
#include "rl_interface.h"
#include "rl_common.h"
#include "du_log.h"
 
#define RLCLAYERNAME   "LTE RLC"     /* Layer Name */

#define EKWxxx 1
#define EMG099 1
#define EMG102 2
#define EMG103 3
#define EMG104 4

/* RLC-SPLIT Activity */
#define RLC_ONE       1
#define RLC_BIT0      0x01
#define RLC_BIT1      0x02
#define RLC_BIT2      0x04
#define RLC_BIT3      0x08

#define RLC_2K_BYTE   2048

/* RLC RB flag bits */
#define RLC_RB_REESTABLISH_DL  RLC_BIT0
#define RLC_RB_REESTABLISH_UL  RLC_BIT1
#define RLC_RB_DELETE_DL       RLC_BIT2
#define RLC_RB_DELETE_UL       RLC_BIT3


#define RLC_MOD_1024           0x3FF  /* used for MOD 1024 */


 
/************************************************************************
 *                            SAP States
 ************************************************************************/

#define RLC_SAP_NOT_CFG        0     /*!< SAP Not Configured */
#define RLC_SAP_CFG            1     /*!< SAP Configured but not not bound */
#define RLC_SAP_BND            2     /*!< SAP Bound */
#define RLC_SAP_BINDING        3     /*!< SAP Bind initiated */
#define RLC_SAP_UBND           4     /*!< SAP Unbind */

#define RLC_MAX_SAP_BND_RETRY  3     /*!< Maximum SAP Bin Retries */

#define RLC_MAX_UE             0xffffffff  /*!< Maximum number of UEs. */

/* Maximum number of Saps */
#define RLC_MAX_UDXSAPS        1     /*!< Maximum number of UDX Saps */
#define RLC_MAX_KWUSAPS        2     /*!< Maximum number of KWU Saps. */
#define RLC_MAX_CKWSAPS        1     /*!< Maximum number of CKW Saps. */
/*MCELL changes*/
#define RLC_MAX_RGUSAPS        4//5     /*!< Maximum number of RGU Saps. */

#define RLC_MAX_RGUSAP_TMR     1     /*!< Maximum number of RGU SAP Timers. */

#define RLC_UI_RRC             0     /*!< Upper interface RRC sap Id. */
#define RLC_UI_PDCP            1     /*!< Upper interface PDCP sap Id. */

#ifdef LTE_L2_MEAS
/* TODO. This works for FDD only. For TDD the array dimension
 * should be changed according to the number of Harq Procs */
#define RLC_MAX_TB_PER_UE      64    /*!< Maximum number of tbCb for UE */
#define RLC_INVALID_TBID       RLC_MAX_TB_PER_UE
#endif
/*******************************************************************************
 *                              Memory related Defines 
 ******************************************************************************/
#ifdef MCCABE_COV
/* Allocate function */
#define RLC_ALLOC(_cb,_buf, _size)                                   \
{                                                                   \
   SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf,       \
                (Size) _size);                                     \
      cmMemset((U8 *)(_buf), 0, _size);                             \
}

#define RLC_RMV_SDU(_cb,_sduQ,_sdu)                    \
{                                                     \
   SPutMsg(_sdu->mBuf);                               \
   cmLListDelFrm(_sduQ,&_sdu->lstEnt);                \
   RLC_FREE_WC(_cb,_sdu, sizeof(RlcSdu));               \
}

#define RLC_FREE(_cb,_buf, _size)                          \
{                                                         \
      (Void) SPutSBuf(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size);                 \
      _buf = NULLP;                                       \
}

#define RLC_FREE_BUF(_buf)           \
{                                   \
      SPutMsg(_buf);                \
      _buf = NULLP;                 \
}

#else

#define RLC_FREE_SHRABL_BUF(_region, _pool,_buf, _size)    \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutStaticBuffer(_region, _pool,             \
            (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                       \
   }                                                      \
}

#define RLC_FREE_SHRABL_BUF_WC(_region, _pool,_buf, _size) \
{                                                         \
  (Void) SPutStaticBuffer(_region, _pool,                 \
        (Data *) _buf, (Size) _size, 0);                 \
  _buf = NULLP;                                       \
}

#define RLC_ALLOC_SHRABL_BUF_WC(_region, _pool,_buf, _size)           \
{                                                                    \
 SGetStaticBuffer(_region, _pool, (Data **)&_buf,                    \
                (Size) _size, 0);                                    \
}

#define RLC_ALLOC_SHRABL_BUF(_region, _pool,_buf, _size)              \
{                                                                    \
 if (SGetStaticBuffer(_region, _pool, (Data **)&_buf,                \
                (Size) _size, 0) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}
/* Allocate function */
#define RLC_ALLOC(_cb,_buf, _size)                                    \
{                                                                    \
 if (SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf,      \
                (Size) _size) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}

#define RLC_ALLOC_WC(_cb,_buf, _size)  \
           SGetSBuf(_cb->init.region, _cb->init.pool, (Data **)&_buf, (Size) _size)     

#define RLC_RMV_SDU(_cb,_sduQ,_sdu)              \
{                                               \
   if(_sdu->mBuf)                               \
   {                                            \
      SPutMsg(_sdu->mBuf);    \
   }                                            \
   cmLListDelFrm(_sduQ,&_sdu->lstEnt);          \
   RLC_FREE(_cb,_sdu, sizeof(RlcSdu));            \
}

#define RLC_FREE(_cb,_buf, _size)                          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutSBuf(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size);                 \
      _buf = NULLP;                                       \
   }                                                      \
}

#define RLC_FREE_BUF(_buf)           \
{                                   \
   if (_buf != NULLP)               \
   {                                \
      SPutMsg(_buf);                \
   }                                \
      _buf = NULLP;                 \
}
#endif




#define RLC_FREE_WC(_cb,_buf, _size)                       \
{                                                         \
      (Void) SPutSBuf(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size);                 \
      _buf = NULLP; /*assigning NULLP after free*/ \
}

/* kw002.201 Freeing from region of pst */
#define RLC_PST_FREE(_region, _pool, _buf, _size)          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutSBuf(_region, _pool,                     \
                      (Data *) _buf, (Size) _size);       \
      _buf = NULLP;                                       \
   }                                                      \
}

#ifdef XEON_SPECIFIC_CHANGES
#ifdef SS_LOCKLESS_MEMORY
#define RLC_SHRABL_STATIC_BUF_FREE(_region, _pool, _buf, _size)     \
{                                                                  \
   if (_buf != NULLP)                                              \
   {                                                               \
      (Void) SPutStaticBuffer(_region, _pool,                      \
                      (Data *) _buf, (Size) _size, 0);             \
      _buf = NULLP;                                                \
   }                                                               \
}

#define RLC_SHRABL_STATIC_BUF_ALLOC(_region, _pool, _buf, _size)     \
{                                                                        \
 SGetStaticBuffer(_region, _pool, (Data **)&_buf,      \
                (Size) _size, 0);                                        \
}

#else

#define RLC_SHRABL_STATIC_BUF_FREE(_region, _pool, _buf, _size)     \
{                                                                  \
   if (_buf != NULLP)                                              \
   {                                                               \
      (Void) SPutSBuf(_region, _pool,                      \
                      (Data *) _buf, (Size) _size);             \
      _buf = NULLP;                                                \
   }                                                               \
}

#define RLC_SHRABL_STATIC_BUF_ALLOC(_region, _pool, _buf, _size)     \
{                                                                        \
 SGetSBuf(_region, _pool, (Data **)&_buf,      \
                (Size) _size);                                        \
}
#endif /* SS_LOCKLESS_MEMORY */

#else 

#define RLC_SHRABL_STATIC_BUF_FREE(_region, _pool, _buf, _size)     \
{                                                                  \
   if (_buf != NULLP)                                              \
   {                                                               \
      (Void) SPutStaticBuffer(_region, _pool,                      \
                      (Data *) _buf, (Size) _size, 0);             \
      _buf = NULLP;                                                \
   }                                                               \
}

#define RLC_SHRABL_STATIC_BUF_ALLOC(_region, _pool, _buf, _size)     \
{                                                                        \
 SGetStaticBuffer(_region, _pool, (Data **)&_buf,      \
                (Size) _size, 0);                                        \
}
#endif

#define RLC_FREE_BUF_WC(_buf)    SPutMsg((_buf));

#define RLC_MEM_CPY(_dst, _src, _size)  cmMemcpy((U8*)_dst, (U8 *)_src, _size); 

#define RLC_MEM_ZERO(_buf, _size) cmMemset((U8 *)(_buf), 0, _size);

#define RLC_GET_MEM_REGION(_cb) (_cb->init.region)

#define RLC_GET_MEM_POOL(_cb) (_cb->init.pool)

#define RLC_GET_MEM_POOL_ADDRESS(_cb) (&_cb->init.pool)

/* Memset to value */
#define RLC_MEM_SET(_arg, _val, _size) cmMemset((U8 *)_arg, (U8)_val, _size); 

/* Alarms */
/* Send an alarm for sapId events */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define RLC_GETSDUIDX(_sduIdx) \
{\
   _sduIdx = (((_sduIdx)+1) % RLC_L2MEAS_MAX_OUTSTNGSDU);\
}
#define RLC_SEND_SAPID_ALARM(_cb,_sapId, _evnt, _cause) \
{ \
   rlcLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, _sapId, 0, 0); \
}
#define RLC_SEND_UEID_ALARM(_cb,_ueId, _qci, _evnt, _cause) \
{ \
   rlcLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, 0, _ueId, _qci); \
}
#else /* LTE_L2_MEAS */
#define RLC_SEND_SAPID_ALARM(_cb,_sapId, _evnt, _cause) \
{ \
   rlcLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, _sapId, 0); \
}
#define RLC_SEND_UEID_ALARM(_cb,_ueId, _evnt, _cause) \
{ \
   rlcLmmSendAlarm(_cb,LCM_CATEGORY_INTERFACE, _evnt, _cause, 0, _ueId); \
}
#endif /* LTE_L2_MEAS */

/*******************************************************************************
 *                              Common Defines 
 ******************************************************************************/

/* RLC Configuration parameters */
#define RLC_MAX_UL_LI                (2 * RLC_MAX_LI)
/*macro RLC_MAX_DL_LI is moved to kw_env.h file */
#define RLC_MAX_DAT                  RLC_MAXIMUM_DAT
/*macro RLC_MAX_PDU is moved to kw_env.h file */
#define RLC_MAX_RB_PER_CELL          10
#define RLC_MAX_SRB_PER_UE           3
#define RLC_MAX_DRB_PER_UE           32
#define RLC_MAX_LCH_PER_UE           12
#define RLC_MAX_LCH_PER_CELL         6
#define RLC_MAX_NUM_RB               24
#define RLC_MAX_UE                   0xffffffff 
#define RLC_UE_LIST_BUCKET_SIZE      128 
#define RLC_CELL_LIST_BUCKET_SIZE    10 
#define RLC_TRANS_ID_LST_BKT_SIZE    10 
#define RLC_MAX_RB                   32

/* Direction defines */
#define RLC_DIR_UL        1     /*!< Unlink direction */
#define RLC_DIR_DL        2     /*!< Downlink direction */
#define RLC_DIR_BOTH      3     /*!< Both Downlink and Unlink */

#define RLC_DEF_SEQ_NUM 0 /**< Sequence number to pick in case of duplicate
                              entries in hash list searches*/

/**
 * @def RLC_MIN
 *
 *    Macro to find the miniumum of two numbers
 *
 * @param[in] x    First number
 * @param[in] y    Second number
 *
*/
#define RLC_MIN(x,y) (x) < (y) ? (x) : (y)

/**
 * @def RLC_GET_RLCCB
 *
 *    Macro to the RLC instance
 *
 * @param[in] _inst    Instance Id
 *
*/
#define RLC_GET_RLCCB(_inst) rlcCb[_inst]                              

#define RLC_ADD_SDU            1     /*!< Add SDU. */
#define RLC_DEL_SDU            2     /*!< Delete SDU. */

#define RLC_CFM_NOK            0     /*!< Do not send DatCfm */
#define RLC_CFM_OK             1     /*!< Send DatCfm */

/* Set the unsolictated Status flag */
#define RLC_SET_USTA_FLAG(_rlcCb, _value) \
{ \
   _rlcCb->init.usta = _value; \
}

/* Macros to get the init parameters */
#define RLC_GET_DBG_MASK(_rlcCb) (_rlcCb->init.dbgMask)
#define RLC_GET_LMPST_MEM_POOL(_rlcCb) (_rlcCb->init.lmPst.pool)
#define RLC_GET_LMPST_MEM_REGION(_rlcCb) (_rlcCb->init.lmPst.region)

/* Macros for configuration module */
#define RLC_CFG_FILL_CFG_CFM(_entCfm, _rbId, _rbType, _status, _reason)  \
{                                                              \
   _entCfm->rbId  = _rbId;                                     \
   _entCfm->rbType = _rbType;                                  \
   _entCfm->status.status = _status;                           \
   _entCfm->status.reason = _reason;                           \
} 

/**
 * @def RLC_VALIDATE_UE_RBID
 *
 *    This macro validates whether the _rbId passed is valid or not. It checks
 *    if the _rbId is within the maximum value depending on the _rbType.
 *    Returns TRUE if valid else FALSE
 *
 * @param[in] _rbType    Type of the Radio Bearer; SRB or DRB
 * @param[in] _rbId      RB Id of the RB to be validated
 *
*/ 
#define RLC_VALIDATE_UE_RBID(_rbType, _rbId)                     \
       ((_rbType == CM_LTE_SRB && _rbId < RLC_MAX_SRB_PER_UE) || \
       (_rbType == CM_LTE_DRB && _rbId < RLC_MAX_DRB_PER_UE))

/*******************************************************************************
 *                              UIM Defines 
 ******************************************************************************/
#if (ERRCLASS & ERRCLS_INT_PAR)
#define RLC_VALDATE_SAP(_cb,_chkSpId, _sap, _ret)                             \
{                                                                            \
   if (_chkSpId != _sap->spId)                                               \
   {                                                                         \
      RLCLOGERROR(_cb,ERRCLS_DEBUG, EKWxxx, (ErrVal) RFAILED,                 \
            "Sap Id Validation Failed.");                                    \
      _ret = RFAILED;                                                        \
   }                                                                         \
   /* SAP state validation */                                                \
   if(_sap->state != RLC_SAP_BND)                                             \
   {                                                                         \
      RLCLOGERROR(_cb,ERRCLS_INT_PAR, EKWXXX, (ErrVal) RFAILED,               \
            "Sap State Invalid.");                                           \
      RLC_SEND_SAPID_ALARM(_cb,0, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_STATE); \
      _ret = RFAILED;                                                        \
   }                                                                         \
}
#else /* ERRCLASS & ERRCLS_INT_PAR */
#define RLC_VALDATE_SAP(_cb,_chkSpId, _sap, _ret)                             \
{                                                                            \
   /* SAP state validation */                                                \
   if(_sap->state != RLC_SAP_BND)                                             \
   {                                                                         \
      RLCLOGERROR(_cb,ERRCLS_INT_PAR, EKWXXX, (ErrVal) RFAILED,               \
            "Sap State Invalid.");                                           \
      RLC_SEND_SAPID_ALARM(_cb,0, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_STATE); \
      _ret = RFAILED;                                                        \
   }                                                                         \
}
#endif /* ERRCLASS & ERRCLS_INT_PAR */

/*******************************************************************************
 *                              Timer Defines 
 ******************************************************************************/
#define RLC_TMR_LEN                     10
#define RLC_MAX_UM_TMR                  1
#define RLC_MAX_AM_TMR                  3
#define RLC_EVT_UMUL_REORD_TMR          1
#define RLC_EVT_AMUL_REORD_TMR          2
#define RLC_EVT_AMUL_STA_PROH_TMR       3
#define RLC_EVT_AMDL_POLL_RETX_TMR      4
#define RLC_EVT_WAIT_BNDCFM             5
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define RLC_EVT_L2_TMR                  6
#endif /* LTE_L2_MEAS */

/*******************************************************************************
 *                              DBM Defines 
 ******************************************************************************/
/**
 * @def RLC_DBM_GET_RBCB_FROM_UECB
 *
 *    This macro makes _rbCb point to the RB in _ueCb based on the passed 
 *    _rbId and _rbType. _rbCb can point to NULLP
 *
 * @param[in] _rbId      RB Id of the RB to be fetched
 * @param[in] _rbType    Type of the RB, SRB or DRB
 * @param[in] _ueCb      Pointer to the UECb for which to get the RB
 * @param[out] _rbCb     Pointer to the found RbCb
 *
*/
#define RLC_DBM_GET_RBCB_FROM_UECB(_rbId, _rbType, _ueCb, _rbCb)            \
           (_rbCb) = ((_rbType) == CM_LTE_SRB) ? (_ueCb)->srbCb[(_rbId)] : \
                                                 (_ueCb)->drbCb[(_rbId)];     
/**
 * @def RLC_DBM_GET_CELL_RBCB
 *
 *    This macro makes _rbCb point to the RB in the _rbCbLst. 
 *    _rbCb can point to NULLP
 *
 * @param[in] _rbId         RB Id of the RB to be fetched
 * @param[in] _rbCbLst      Pointer to array of RBCbs
 * @param[out] _rbCb        Pointer to the found RbCb
 *
*/
#define RLC_DBM_GET_CELL_RBCB(_rbId, _rbCbLst, _rbCb) \
           (_rbCb) = (_rbCbLst)[(_rbId)]; 

/*******************************************************************************
 *                              UMM Defines 
 ******************************************************************************/
#define RLC_UMDL     rbCb->m.umDl 
#define RLC_UMUL     rbCb->m.umUl 

/* Sequence Number length defines */
#define RLC_UM_CFG_5BIT_SN_LEN      1 /**< UM 5-bit Sequence number length 
                                          in bytes*/   
#define RLC_UM_CFG_10BIT_SN_LEN     2 /**< UM 10-bit Sequence number length 
                                          in bytes*/
/* 5GNR */
/* Sequence Number length defines */
#define RLC_AM_CFG_12BIT_SN_LEN      1 /**< AM 12-bit Sequence number length 
                                          in bytes*/   
#define RLC_AM_CFG_18BIT_SN_LEN      2 /**< AM 18-bit Sequence number length 
                                          in bytes*/

/**
 * @def RLC_RMV_MAC_HDR_SZ
 *
 *    If PDU size is greater than 127, MAC header would be 3 bytes else 2 bytes
 *
 * @param[in,out] _pduSz        Size of the pdu 
 *
*/
#define RLC_RMV_MAC_HDR_SZ(_pduSz) (_pduSz) -= ((_pduSz) > 127) ? 3 : 2;

/**
 * @def RLC_UM_GET_VALUE
 *
 *    This macro is used to calculate the value of UM state variables used 
 *    in comparisons.  VR(UH) - UM Window Size is taken as the base modulus.
 *    Returns the modifed value
 *
 * @param[in] _val               Value of the state variable 
 * @param[in] _kwUmUl        Um Uplink control block
 *
*/ 
#define RLC_UM_GET_VALUE(_val,_kwUmUl)  \
     (((_val) - ((_kwUmUl).vrUh - (_kwUmUl).umWinSz)) & ((_kwUmUl).modBitMask))
     
/*******************************************************************************
 *                              AMM Defines 
 ******************************************************************************/ 
#define AMDL                           rbCb->m.amDl 
#define AMUL                           rbCb->m.amUl

/* PDU Types */
#define RLC_DATA_PDU  1
#define RLC_CNTRL_PDU 0

#define RLC_FI_FIRST_SEG                0x02
#define RLC_FI_LAST_SEG                 0x01
#define RLC_SI_FIRST_SEG                0x01
#define RLC_SI_LAST_SEG                 0x02
#define RLC_SI_MID_SEG                  0x03

#define RLC_POLL_SET                    0x40 /* 01000000 */
#define RLC_POLL_UNSET                  0xbf /* 10111111 */
#define RLC_AM_WIN_SZ                   512
#define RLC_MAX_NACK_CNT                100
/*RLC_MAX_CNTRL_FIELDS (Maximum size of Status Pdu) 
 *  = MAX_NACK_CNT * sizeof(NACK_SN,E1,E2,E3,soStart,soEnd, nackRange)
 * for 18 bit SN + Fixed Header*/
#define RLC_MAX_CNTRL_FIELDS            ((RLC_MAX_NACK_CNT * 8) + 3)  

/* Each LI(Length Indicator) holds approx 1+1/2 byte and some other fields thus keeping Header Size equal to twice of MAX LI */
/* 5GNR_RLC: Need to change value of HDRSZ as per number of PDUs going in one datReq */
#define RLC_MAX_HDRSZ                         5 
#define RLC_AM_PDU_FIXED_HDRSZ                2
#define RLC_AM_PDU_12BIT_SN_HDRSZ             2
#define RLC_AM_PDU_18BIT_SN_HDRSZ             3
#define RLC_AM_SEG_12BIT_SN_WITH_SO_HDRSZ     4
#define RLC_AM_SEG_18BIT_SN_WITH_SO_HDRSZ     5
#define RLC_AM_SEG_12BIT_SN_WITHOUT_SO_HDRSZ  2
#define RLC_AM_SEG_18BIT_SN_WITHOUT_SO_HDRSZ  3
#define RLC_EXTN_HDRSZ                  2
#define RLC_CNTRL_PDU_FIXED_HDRSZ       3
#define RLC_MAC_HDR_SZ2                 2
#define RLC_MAC_HDR_SZ3                 3
#define RLC_BYTE_LEN                    8
#define RLC_2BYTE_LEN                   16
#define RLC_E1_LEN                      1
#define RLC_NACK_E1E2_LEN               12
#define RLC_SO_LEN                      15
#define RLC_DC_LEN                      1
#define RLC_CPT_LEN                     3
#define RLC_RF_LEN                      1
#define RLC_P_LEN                       1
#define RLC_FI_LEN                      2
#define RLC_SI_LEN                      2
#define RLC_E_LEN                       1
#define RLC_SN_LEN                      10
#define RLC_SN_LEN_12BITS               12
#define RLC_SN_LEN_18BITS               18
#define RLC_LSF_LEN                     1
#define RLC_LI_LEN                      11
#define RLC_STA_PDU_R_BITS_ACKSN_12BITS 7  /* 5GNR : Num Reserved bits in STATUS PDU */
#define RLC_STA_PDU_R_BITS_ACKSN_18BITS 1
#define RLC_STA_PDU_R_BITS_NACKSN_12BITS 1
#define RLC_STA_PDU_R_BITS_NACKSN_18BITS 3
#define RLC_NACK_RANGE_LEN               8
#define RLC_SO_LEN_5GNR                  16

#define RLC_DC_POS                      0x80
#define RLC_DC_SHT                      7      
#define RLC_POLL_POS                    0x40 /* 5GNR */
#define RLC_POLL_SHT                    6    /* 5GNR */
#define RLC_SI_POS                      0x30 /* 5GNR */
#define RLC_SI_SHT                      4    /* 5GNR */
#define RLC_SN_POS_12BIT                0x0F
#define RLC_SN_POS_18BIT                0x03
#define RLC_AM_GET_WIN_SZ(_snLen)       ((RLC_AM_CFG_12BIT_SN_LEN == (_snLen)) ? (2048) : (131072)) /* 5GNR */
#define RLC_RCV_BUF_BIN_SIZE 512   /* Could be increased to avoid the search time.
                                      with the memory trade-off */
#define RLC_TX_BUF_BIN_SIZE 512   /* Could be increased to avoid the search time.
                                      with the memory trade-off */

#define RLC_SDU_LST                     1
#define RLC_SEG_LST                     2
#define RLC_RETX_LST                    3
#define RLC_ALL_BYTES_MISSING           0xffff

#define RLC_MAX_PDU_MAP                 30       /*!< Maximum PDU Map. */

#define RLC_LLIST_FIRST_SDU(lstCp, nod)          \
{                                               \
   CmLList *tmpNode;                            \
   /*CM_LLIST_FIRST_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP)*/                        \
   if((tmpNode=cmLListFirst(&lstCp)))            \
      nod = (RlcSdu *)tmpNode->node;             \
   else                                         \
      nod = NULLP;                              \
}                                                          

                                                           
#define RLC_LLIST_FIRST_SEG(lstCp, nod)         \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_FIRST_NODE(&(lstCp), tmpNode);*/     \
   /*if (tmpNode != NULLP)*/                       \
   if((tmpNode=cmLListFirst(&lstCp)))            \
      nod = (RlcSeg *)tmpNode->node;            \
   else                                        \
      nod = NULLP;                             \
}                                                          

                                                           
#define RLC_LLIST_FIRST_RETX(lstCp, nod)        \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_FIRST_NODE(&(lstCp), tmpNode);*/     \
   /*if (tmpNode != NULLP)*/                       \
   if((tmpNode=cmLListFirst(&lstCp)))            \
      nod = (RlcRetx *)tmpNode->node;           \
   else                                        \
      nod = NULLP;                             \
}

#define RLC_LLIST_NEXT_SDU(lstCp, nod)          \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_NEXT_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP)  */                     \
   if((tmpNode = cmLListNext(&lstCp)))          \
      nod = (RlcSdu *)tmpNode->node;            \
   else                                        \
      nod = NULLP;                             \
}                                              


#define RLC_LLIST_NEXT_SEG(lstCp, nod)          \
{                                              \
   CmLList *tmpNode;                           \
   (lstCp).crnt = &((nod)->lstEnt);            \
   /*CM_LLIST_NEXT_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP)*/                       \
   if((tmpNode = cmLListNext(&lstCp)))           \
      nod = (RlcSeg *)tmpNode->node;            \
   else                                        \
      nod = NULLP;                             \
}      

                                        
#define RLC_LLIST_NEXT_RETX(lstCp, nod)         \
{                                              \
   CmLList *tmpNode;                           \
   /*CM_LLIST_NEXT_NODE(&(lstCp), tmpNode);*/      \
   /*if (tmpNode != NULLP) */                      \
   if ((tmpNode = cmLListNext(&lstCp)))          \
      nod = (RlcRetx *)tmpNode->node;           \
   else                                        \
      nod = NULLP;                             \
}


#define RLC_LLIST_LAST_RETX(lstCp, nod)         \
{                                              \
   CmLList *tempNode = NULLP;                  \
   cmLListLast(&lstCp);                        \
   tempNode = cmLListCrnt(&lstCp);             \
   if (tempNode != NULLP)                      \
      nod = (RlcRetx *)tempNode->node;          \
   else                                        \
      nod = NULLP;                             \
}

#define RLC_LLIST_LAST_SEG(lstCp, nod)          \
{                                              \
   CmLList *tempNode = NULLP;                  \
   cmLListLast(&lstCp);                        \
   tempNode = cmLListCrnt(&lstCp);             \
   if (tempNode != NULLP)                      \
      nod = (RlcSeg *)tempNode->node;           \
   else                                        \
      nod = NULLP;                             \
}

#define RLC_LLIST_LAST_SDU(lstCp, nod)          \
{                                              \
   CmLList *tempNode = NULLP;                  \
   cmLListLast(&lstCp);                        \
   tempNode = cmLListCrnt(&lstCp);             \
   if (tempNode != NULLP)                      \
      nod = (RlcSdu *)tempNode->node;           \
   else                                        \
      nod = NULLP;                             \
}

#define CM_LLIST_INS_AFT_CRNT(lstCp, nod)      \
{                                              \
   CmLList *nodeToIns = &nod->lstEnt;\
   nodeToIns->node = (PTR) nod;\
   cmLListInsAfterCrnt(&lstCp, nodeToIns);     \
}

#define CM_LLIST_INS_BEF_CRNT(lstCp, nod)      \
{                                              \
   CmLList *nodeToIns = &nod->lstEnt;          \
   nodeToIns->node = (PTR) nod;                \
   cmLListInsCrnt(&lstCp, nodeToIns);          \
}

#define RLC_LLIST_DEL_RECBUF(_recBuf)                       \
{                                                          \
   RlcSeg  *_seg = NULLP;                                   \
   RLC_LLIST_FIRST_SEG(_recBuf->segLst, _seg);              \
   while (_seg)                                            \
   {                                                       \
      cmLListDelFrm(&_recBuf->segLst, &_seg->lstEnt);      \
      RLC_FREE(_seg, sizeof(RlcSeg));                        \
      RLC_LLIST_NEXT_SEG(_recBuf->segLst, _seg);            \
   }                                                       \
}

#define MODAMT(x, y, z,_snModMask)   \
{                         \
   y = (x - z) & _snModMask;   \
}                                                         

#define MODAMR(x, y, z , _snModMask)   \
{                         \
   y = (x - z) & (_snModMask);   \
}

/**
 * @def RLC_AM_IS_TRANS_WIN_STALLED
 *
 *    This macro is used to check if the AM transmit window is stalled or not.
 *    The tramist window is stalled when the distance between txNext and txNextAck
 *    is greater than equal to Window Size. Actually it should never be greater 
 *    than Window Size.
 *    Returns TRUE if the window is stalled else FALSE
 *
 * @param[in] _amDl     AM Downlink control block
 *
*/ 
#define RLC_AM_IS_TRANS_WIN_STALLED(_amDl)  \
     ((((_amDl)->txNext - (_amDl)->txNextAck) & _amDl->snModMask) >= (RLC_AM_GET_WIN_SZ(_amDl->snLen)))

#ifdef TENB_STATS
#define RLC_AM_TRANS_WIN_SIZE(_amDl)  \
     (((_amDl)->txNext - (_amDl)->txNextAck) & _amDl->snModMask)
#endif

#define RLC_AM_IS_POLL_BIT_SET(_amDl) \
  (AMDL.pollSn == ((AMDL.txNext - 1) & AMDL.snModMask))

#define RLC_FILL_CNTRL_INFO(cntrlInfo, _val, _len, _idx, _eb)\
{                                                           \
   cntrlInfo.val = _val;                                    \
   cntrlInfo.len = _len;                                    \
   cntrlInfo.idx = _idx;                                    \
   cntrlInfo.emtBits = _eb;                                 \
}
#define RLC_FILL_PREV_IDX(cntrlInfo, _e1Idx, _e1eb, _idx, _eb) \
{                                                                     \
  _e1Idx = cntrlInfo.e1Idx;                                           \
  _e1eb  = cntrlInfo.e1eb;                                            \
  _idx   = cntrlInfo.idx;                                             \
  _eb    = cntrlInfo.emtBits;                                         \
}

#define RLC_FILL_HDR_ARGS(hdrInfo, _val, _len)  \
{                                              \
   hdrInfo.val = _val;                         \
   hdrInfo.len = _len;                         \
}

/* kw003.201 - This macro provides the header size other than the */
/*             fixed header of 2 bytes for each AMD PDU or 4 bytes*/
/*             for an AM PDU segment                              */
#define RLC_AM_EXTN_HDRSZ(_numLi, _eHdrSz)       \
{                                               \
   if ((_numLi - 1) % 2)                        \
   {                                            \
      _eHdrSz = ((3 * (_numLi - 2)) >> 1) + 2;  \
   }                                            \
   else                                         \
   {                                            \
      _eHdrSz = (3 * (_numLi - 1)) >> 1;        \
   }                                            \
}

/* Update poll bit in the buffer */
#define RLC_UPD_POLL_BIT(_gCb, _retx, _poll)                \
{                                                          \
   U8 fHdr;                                                \
                                                           \
   if (_poll != _retx->amHdr.p)                            \
   {                                                       \
      /* Get the first byte of the buffer */               \
      SRemPreMsg((Data *)&fHdr, _retx->seg);               \
      if (_poll == TRUE)                                   \
      {                                                    \
         fHdr = fHdr | RLC_POLL_SET;                        \
      }                                                    \
      else                                                 \
      {                                                    \
         fHdr = fHdr & RLC_POLL_UNSET;                      \
      }                                                    \
      /* Concatenated updated hdr to the mBuf */           \
      SAddPreMsg ((Data)fHdr, _retx->seg);                 \
   }                                                       \
   /* Update poll bit in the hdrInfo */                    \
   _retx->amHdr.p = _poll;                                 \
}

#define RLC_AM_ELIMINATE_EXTN_HDR(_pduSz, _sduSz, _numLi)   \
{                                                          \
   if ( (_pduSz > _sduSz) && (_sduSz < 2048) )             \
   {                                                       \
      _pduSz -= (_numLi % 2) ? 1 : 2;                      \
   }                                                       \
}
/**
 * @def RLC_AM_CHK_SN_WITHIN_RECV_WINDOW
 *
 *    This macro is used to check if a Sequence Number falls within the AM
 *    reception window or not.
 *    The condition is VR(R) <= SN < VR(MR), which are subtracting the base
 *    modulus becomes 0 <= (SN - VR(R)) % SNLen < (VR(MR) - VR(R)) % SnLen
 *    NOTE: Careful with the parantheses
 *
 *    Returns TRUE if within the window; FALSE otherwise
 *
 * @param[in] _sn       The sequence number to be checked
 * @param[in] _amUl     AM Uplink control block
 *
*/
#define RLC_AM_CHK_SN_WITHIN_RECV_WINDOW(_sn, _amUl)          \
  ((((_sn) - (_amUl->rxNext)) & (_amUl->snModMask)) < (((_amUl->vrMr) - (_amUl->rxNext)) & (_amUl->snModMask))) 

#define RLC_POWER(x, y)  x << (y-1); 

#ifndef L2_OPTMZ
#define rlcCpyMsg(_cb,x, y) \
      (SAddMsgRef((x), RLC_GET_MEM_REGION(_cb), RLC_GET_MEM_POOL(_cb), (y)))
#else
/* L2 optimization for mUe/Tti: Removing dup buf*/
#define rlcCpyMsg(_cb,x, y) \
      (SIncMsgRef((x), RLC_GET_MEM_REGION(_cb), RLC_GET_MEM_POOL(_cb), (y)))
#endif

//      printf("Copy Msg %x \n",x);

/*******************************************************************************
 *                              Debugging Defines 
 ******************************************************************************/
#define RLC_DBG_SUB_MASK   DBGMASK_MI             /**< Use for sub-mask */
#define RLC_DBGMASK_DETAIL (RLC_DBG_SUB_MASK << 0) /**< Parameters, It will give
                                                      in depth info */
#define RLC_DBGMASK_BRIEF  (RLC_DBG_SUB_MASK << 1) /**< Info, It will give info at
                                                    entry and exit places along
                                                   with certain state changes */
#define RLC_DBGMASK_ERROR  (RLC_DBG_SUB_MASK << 2) /**< Error information */
#define RLC_DBGMASK_FATAL  (RLC_DBG_SUB_MASK << 3) /**< FATAL errors like memory
                                                    resource failure etc., */

#define RLC_DBG_MDL_MASK (RLC_DBG_SUB_MASK << 4)

#define RLC_DBGMASK_TM         (RLC_DBG_MDL_MASK << 0)    /**< TM */
#define RLC_DBGMASK_UM         (RLC_DBG_MDL_MASK << 1)    /**< UM */
#define RLC_DBGMASK_AM         (RLC_DBG_MDL_MASK << 2)    /**< AM */
#define RLC_DBGMASK_DL         (RLC_DBG_MDL_MASK << 3)    /**< DL */
#define RLC_DBGMASK_UL         (RLC_DBG_MDL_MASK << 4)    /**< UL */
#define RLC_DBGMASK_CFG        (RLC_DBG_MDL_MASK << 5)    /**< CFG */
#define RLC_DBGMASK_LMM        (RLC_DBG_MDL_MASK << 6)    /**< LMM */
#define RLC_DBGMASK_INF        (RLC_DBG_MDL_MASK << 7)    /**< UI, LI */
#define RLC_DBGMASK_DUT        (RLC_DBG_MDL_MASK << 8)    /**< DBM, UTL, TMR */
#define RLC_DBGMASK_MBUF_PRNT  (RLC_DBG_MDL_MASK << 9)    /**< MBUF, useful in
                                                             integrated 
                                                             testing */
#define RLC_DBGMASK_MEM_INFO   (RLC_DBG_MDL_MASK << 10)   /**< Print SSI memory
                                                             information*/
#define RLC_DBGMASK_UDX        (RLC_DBG_MDL_MASK << 11)   /**< UDX interface */

#ifdef DEBUGP
#define RLC_PRNT_BORDER                                   \
do                                                       \
{                                                        \
   RLC_PRNT((_kwPBuf, "\n==========================\n")); \
}while(0)

#define RLC_PRNT_HLINE(_cb,_pMsg)                                              \
{                                                                             \
   sprintf((_cb)->init.prntBuf, "[RLC_LAYER: %s:%d]::", __FILE__, __LINE__);  \
   SPrint((_cb)->init.prntBuf);                                               \
   RLC_PRNT_TSTAMP(_cb);                                                       \
   sprintf((_cb)->init.prntBuf, _pMsg);                                       \
   SPrint((_cb)->init.prntBuf);                                               \
}

#define RLC_PRNT(_cb,_prntbuf)  \
{                              \
   sprintf _prntbuf;           \
   SPrint(_cb->init.prntBuf);  \
}

#define RLC_PRINT_TO_BUFFER(_cb,...)                             \
{                                                               \
   snprintf((_cb)->init.prntBuf, PRNTSZE, "[%s]::", __func__);  \
   SPrint((_cb)->init.prntBuf);                                 \
   snprintf(_cb->init.prntBuf, PRNTSZE,__VA_ARGS__);            \
   SPrint(_cb->init.prntBuf);                                   \
}

#define RLC_PRNT_TSTAMP(_cb)                                   \
{                                                             \
   S8 _buf[60];                                               \
   DateTime dt;                                               \
   cmMemset((U8 *)(&dt), 0, sizeof(DateTime));                \
   SGetDateTime(&dt);                                         \
   sprintf(_buf, "date: %02d/%02d/%04d time: %02d:%02d:%02d", \
     (int)dt.month,(int)dt.day,(int)dt.year + 1900,           \
     (int)dt.hour,(int)dt.min,(int)dt.sec);                   \
   RLC_PRNT(_cb,(_cb->init.prntBuf,("[%s]", _buf)));           \
}

#define RLC_PRNT_MBUF(_cb,_mBufPtr)                          \
do                                                          \
{                                                           \
   if(_cb->init.dbgMask & (RLC_DBGMASK_MBUF_PRNT))           \
   {                                                        \
     RLC_PRNT_HLINE(_cb,("\nMessage Buffer Contents:\n"));   \
     SPrntMsg ((Buffer *)_mBufPtr, 0, 0);                   \
   }                                                        \
}while(0)

#define RLC_PRNT_MEMINFO(_cb)                                  \
do                                                            \
{                                                             \
   U32  _memInfo;                                             \
   if(_cb->init.dbgMask & (RLC_DBGMASK_MEM_INFO))              \
   {                                                          \
     RLC_PRNT_HLINE(_cb,("\nMemory Information:\n"));          \
     SRegInfoShow(0, &_memInfo);                              \
   }                                                          \
}while(0)

#define RLCDBGP_INTERNAL(_cb,_mask,...)           \
do                                               \
{                                                \
   if (!((_cb->init.dbgMask & _mask) ^ _mask))   \
   {                                             \
      RLC_PRINT_TO_BUFFER(_cb, __VA_ARGS__);      \
   }                                             \
}while(0)

#define RLCDBGP_ERROR(_cb, ...) \
   RLCDBGP_INTERNAL(_cb,(RLC_DBGMASK_ERROR | RLC_MODULE),__VA_ARGS__)

#define RLCDBGP_DETAIL(_cb, ...) \
   RLCDBGP_INTERNAL(_cb,(RLC_DBGMASK_DETAIL | RLC_MODULE),__VA_ARGS__)

#define RLCDBGP_BRIEF(_cb, ...) \
   RLCDBGP_INTERNAL(_cb,(RLC_DBGMASK_BRIEF | RLC_MODULE),__VA_ARGS__)   
   
#else  /* DEBUGP */ 
#define RLC_PRNT_HLINE(_cb,_pMsg)
#define RLC_PRNT(_cb,_prntbuf)
#define RLC_PRNT_TSTAMP(_cb)
#define RLC_PRNT_MBUF(_cb,_mBufPtr)
#define RLC_PRNT_MEMINFO(_cb)
#define RLCDBGP(_cb,_mask, _arg)
#define RLCDBGP_ERROR(_cb, ...) 
#define RLCDBGP_DETAIL(_cb, ...)
#define RLCDBGP_BRIEF(_cb, ...)
#endif /* DEBUGP */

/*******************************************************************************
 *                              LMM Defines 
 ******************************************************************************/
#define RLC_LMM_RB_STS_INC(_cb)    (_cb)->genSts.numOfRb++;

#define RLC_LMM_RB_STS_DEC(_cb)    (_cb)->genSts.numOfRb--;

#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
#define RLC_FILL_SAP_HELPER(_Sap, _cfg, _gCb)\
{\
   _Sap->pst.selector = _cfg->selector; \
   _Sap->pst.route = _cfg->route; \
   _Sap->pst.prior =  _cfg->priority; \
   _Sap->pst.region = _gCb->init.region;\
   _Sap->pst.pool = _gCb->init.pool;\
   _Sap->pst.dstProcId = _cfg->procId; \
   _Sap->pst.dstEnt = _cfg->ent; \
   _Sap->pst.dstInst = _cfg->inst; \
   _Sap->pst.srcProcId = _gCb->init.procId; \
   _Sap->pst.srcEnt = _gCb->init.ent; \
   _Sap->pst.srcInst = _gCb->init.inst; \
   _Sap->pst.event = EVTNONE; \
   _Sap->spId = _cfg->sapId; \
   _Sap->state = RLC_SAP_CFG; \
}
#else /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
#define RLC_FILL_SAP_HELPER(_Sap, _cfg, _gCb)\
{\
   _Sap->pst.selector = _cfg->selector; \
   _Sap->pst.route = _cfg->route; \
   _Sap->pst.prior =  _cfg->priority; \
   _Sap->pst.region = _cfg->mem.region;\
   _Sap->pst.pool = _cfg->mem.pool;\
   _Sap->pst.dstProcId = _cfg->procId;\
   _Sap->pst.dstEnt = _cfg->ent;\
   _Sap->pst.dstInst = _cfg->inst;\
   _Sap->pst.srcProcId = _gCb->init.procId;\
   _Sap->pst.srcEnt = _gCb->init.ent;\
   _Sap->pst.srcInst = _gCb->init.inst;\
   _Sap->pst.event = EVTNONE;\
   _Sap->spId = _cfg->sapId;\
   _Sap->state = RLC_SAP_CFG;\
}
#endif

/*******************************************************************************
 *                              UDX Defines 
 ******************************************************************************/
#define RLC_GET_DL_SAPCB(_cb, _rbCb) (_cb->u.dlCb->udxDlSap + _rbCb->udxSapId)
#define RLC_GET_UDX_SAP(_cb) (_cb->u.ulCb->udxUlSap)

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define RLC_L2_MAX_TIMERS        1
#define RLC_QCI_LIST_BUCKET_SIZE 10
#define RLC_TB_LIST_BUCKET_SIZE  10
#define RLC_MAX_L2MEAS_EVT       10
/* L2 Measurement index to be used in rbCb to store measData */                                       
#define RLC_L2MEAS_ACT_UE       0                                       
#define RLC_L2MEAS_DL_DELAY     1                                       
#define RLC_L2MEAS_DL_DISC      2
#define RLC_L2MEAS_UU_LOSS      3
#define RLC_L2MEAS_DL_IP        4
#define RLC_L2MEAS_UL_IP        5
#endif /* LTE_L2_MEAS */

#define RLC_RDWR_LOCK(_lockPtr)
#define RLC_RDWR_UNLOCK(_lockPtr)
#define RLC_TIME_DIFF(t1,t2)                  \
   (t1<t2 ? ((0xffffffff - t2) + t1 ): (t1 - t2)) 

#endif /* __KWH__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
