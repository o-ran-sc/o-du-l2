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
  
        Name:    CKW RRC Control Interface
    
        Type:    C include file
  
        Desc:    This file Contains the Data structures for and prototypes
                  CKW Interface
 
        File:    ckw.x
  
*********************************************************************21*/

#ifndef __CKW_X__
#define __CKW_X__

#ifdef __cplusplus
EXTERN "C" {
#endif  /*for extern "C"*/

/** @file ckw.x
    @brief CKW Interface File (ckw.x)
*/
 
/***********************************************************************
                          CKW typedefs and data structures
 ***********************************************************************/

/** @brief
   Logical Channel Information */
typedef struct ckwLChInfo
{
   U8                   lChId;               /*!< Logical Channel ID.
                                                  The allowed values are [1..10] */
   U8                   type;                /*!< Logical Channel Type - 
                                                  BCCH/PCCH/CCCH/DTCH/DCCH */
}CkwLChInfo;

/** @brief
   Un-acknowledge Mode Information */
typedef struct ckwUmInfo
{
   struct   _umDl
   {
      U8                snLen;               /*!< Sequence Number length in bits. Allowed values are 6 and 12*/
   }dl;                                      /*!< Downlink */
   struct   _umUl
   {
      U8                snLen;               /*!< Sequence Number length in bits.*/
                                             /*!< Allowed values are 6 and 12*/
      U8                reOrdTmr;            /*!< T_reordering Timer in msec */
   }ul;                                      /*!< Uplink */
}CkwUmInfo;

/** @brief
   Acknowledge Mode Information */
typedef struct ckwAmInfo
{
   struct   _amDl
   {
      U8                snLen;               /*!< Sequence Number length in
                                               bits. Allowed values are 12 and 18 */
      U16               pollRetxTmr;         /*!< T_poll_retransmit Timer in msec */
      S16               pollPdu;             /*!< Used to trigger a poll for every pollPdu.*/
                                             /*!< This field indicates the number of PDUs */
      /* Patch ckw_x_001.main_5 */
                                             /* !<For Infinite, RLC expects value 0xFFFF */
      S32               pollByte;            /*!< Poll_Byte in bytes. */
                                             /*!<For Infinite RLC expects value 0xFFFFFFFF */
      U8                maxRetx;             /*!< Max_Retx_Threshold */
   }dl;                                      /*!< Down link */
   struct   _amUl
   {
      U8                snLen;               /*!< Sequence Number length in
                                               bits. Allowed values are 12 and 18 */
      U16               staProhTmr;          /*!< T_status_prohibit Timer in msec*/
      U8                reOrdTmr;            /*!< T_reordering Timer in msec*/
   }ul;                                      /*!< Up link */
}CkwAmInfo;

/** @brief
   Transparent Mode Information */
typedef union ckwTmInfo
{
   struct   _tmDl
   {
      U32               buffSize;            /*!< Buffer Size in bytes */
   }dl;                                      /*!< Down link */
}CkwTmInfo;

/** @brief
   Entity Configuration Information */
typedef struct ckwEntCfgInfo
{
   uint8_t              cfgType;             /*!< Type of Configuration - 
                                             ADD/MODIFY/DELETE/RE-ESTABLISH/
                                             DELTEUE */
   uint8_t              rbId;                /* RB ID. The allowed
                                                Values are [0..255] */
   uint8_t              rbType;              /* Type of the RB - can be SRB or DRB */
   uint8_t              qci;                 /*!< qCI value Associated with that RB */
   CkwLChInfo           lCh[CM_LTE_MAX_LOGCH_PER_RB];  /*!< Logical channel
                                                        information
                                                In case of AM mode: 
                                                idx = 0 implies DL and
                                                idx = 1 implies UL
                                                In case of TM/UM idx 0 is only
                                                applicable */
   CmLteRlcMode         entMode;             /*!< Entity mode: AM, UM or TM */
   uint8_t              dir;                 /*!< Direction: It can be either
                                               UL / DL for TM mode 
                                               UL / DL or BOTH for UM mode 
                                               and for AM its always for both UL 
                                               and DL. */
   /* Supported by SPLIT Architecture */
   uint16_t       discardTmr;  /*!< Discard timer. @n The value for this parameter is 
                                expected in milli seconds. @n Value of infinity is also
                                allowed and -1 must be passed for the same.  */
   SpId                 rguSapId;
   /* Supported by SPLIT Architecture ends */
   union
   {
      CkwUmInfo         umInfo;              /*!< Un-acknowledged Mode Information */
      CkwAmInfo         amInfo;              /*!< Acknowledged Mode Information */
      CkwTmInfo         tmInfo;              /*!< Transparent Mode Information */
   }m;
}CkwEntCfgInfo;

/** @brief
   Configuration Information from RRC to RLC */
typedef struct ckwCfgInfo
{
   uint32_t           transId;             /*!< Transaction Id */
   uint8_t            ueId;                /*!< UE ID */
   uint16_t           cellId;              /*!< Cell ID */
   uint8_t            numEnt;              /*!< Number of entities to
                                               configure */
   CkwEntCfgInfo        entCfg[CKW_MAX_ENT_CFG];  /*!< Array of Entities to be
                                              configure */
}CkwCfgInfo;

/** @brief
   Entity Configuration Confirmation Information */
typedef struct ckwEntCfgCfmInfo
{
   uint8_t              rbId;                /*!< rb ID */
   uint8_t              rbType;              /*!< RB type - can be SRB or DRB */
   CmStatus             status;              /*!< Status of the confirmation */
}CkwEntCfgCfmInfo;

/** @brief
   Configuration Confirmation Informatin RLC to RRC */
typedef struct ckwCfgCfmInfo
{
   uint32_t             transId;             /*!< Transaction Id */
   uint8_t              ueId;                /*!< UE ID */
   uint16_t             cellId;              /*!< Cell ID */
   uint8_t              numEnt;              /*!< Number of entities configured */
   CkwEntCfgCfmInfo     entCfgCfm[CKW_MAX_ENT_CFG];   /*!< Array of Entity cfg
                                                         confiramations */
}CkwCfgCfmInfo;

/** @brief
     UE Id Information */
typedef struct ckwUeInfo
{
   CmLteRnti            ueId;               /*!< UE ID */
   CmLteCellId          cellId;             /*!< Cell ID */
}CkwUeInfo;
/* Control primitives towards LTE RRC */

/* Pointer functiones for Pack/Unpack */
//typedef S16 (*CkwCfgReq)     ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));
typedef S16 (*CkwCfgReq)     ARGS((Pst *pst, CkwCfgInfo *cfgInfo));
typedef S16 (*CkwCfgCfm)     ARGS((Pst *pst, SuId suId, CkwCfgCfmInfo *cfmInfo));

typedef S16 (*CkwUeIdChgReq) ARGS((Pst *pst, SpId spId, U32 transId, 
                                   CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));
typedef S16 (*CkwUeIdChgCfm) ARGS((Pst *pst, SuId suId, U32 transId, 
                                   CkwUeInfo *ueInfo, CmStatus status));

typedef S16 (*CkwBndReq)     ARGS((Pst *pst, SuId suId, SpId spId));
typedef S16 (*CkwBndCfm)     ARGS((Pst *pst, SuId suId, U8 status));
typedef S16 (*CkwUbndReq)    ARGS((Pst *pst, SpId spId, Reason reason));

/** 
 * @details The primitive binds the RLC service user entity to the RLC product.
 * The RLC service user must bind each CKW SAP individually before sending any
 * other request. RLC acknowledges the receipt of the bind request by sending
 * a KwUiCkwBndCfm primitive.

 * @param[in] pst       -  For the bind request, the event field in the Pst
 *                         structure is set to EVTCKWBNDREQ
 * @param[in] suId      -  The ID of the service user SAP associated with the
 *                         SAP in the RLC layer. The SSAP in the RLC layer is
 *                         specified by the spId parameter.\n
 *                         Allowable values: 0 to 32767.
 * @param[in] spId      -  The ID of the service provider SAP in the RLC layer.\n
 *                         Allowable values: 0 to 32767.
 * @return ROK
 */
EXTERN S16 KwUiCkwBndReq ARGS((Pst *pst, SuId suId, SpId spId));

/** 
 * @details RLC invokes the bind confirm primitive to acknowledge the receipt
 * of the KwUiCkwBndReq primitive from the service user. The status field
 * indicates whether the bind request succeeded or failed. The bind request
 * may fail due to invalid parameters in the bind request from the
 * service user (for example, an invalid spId).

 * @param[in] pst       -  For the bind confirm, the event field in the Pst
 *                         structure is set to EVTCKWBNDCFM
 * @param[in] suId      -  The ID of the service user SAP specified in the
 *                         bind request. Allowable
values: 0 to 32767.
 * @param[in] status    -  Indicates the status of the bind request.\n
CM_BND_OK: Bind request succeeded\n
CM_BND_NOK: Error encountered during the processing of the bind request.
 * @return ROK
 */
EXTERN S16 KwUiCkwBndCfm ARGS((Pst *pst, SuId suId, U8 status));

/** 
 * @details The RLC service user initiates this primitive for performing
 * an unbind operation.This primitive brings the link down if it is already up,
 * and releases all the resources associated for this SAP at RLC.

 * @param[in] pst          -  For the unbind request, the event field in the Pst
 *                            structure is set to EVTCKWUBNDREQ
 * @param[in] spId         -  The ID of the service provider SAP in the RLC layer
 *                            that is to be disassociated from its service
 *                            user SAP.\n
 *                            Allowable values: 0 to 32767.
 * @param[in] reason       -  Reason for the unbind.\n
 *                            Allowable value: CKW_UBND_REQ.
 * @return ROK
 */
EXTERN S16 KwUiCkwUbndReq ARGS((Pst *pst, SpId spId, Reason reason));

/* Layer interface Control Primitives */
/**
 *@details This primitive is used by RRC to configure the multiple RLC entities. RRC
 sends this primitive for the control of the RLC entity.

 * The different configuration allowed by RRC to RLC are:\n
   CKW_CFG_ADD          -  Configures the new RLC entity\n
   CKW_CFG_MODIFY       -  Modifies the RLC entity\n
   CKW_CFG_DELETE       -  Deletes the RLC entity\n
   CKW_CFG_REESTABLISH  -  Re-establilsh the RLC entity\n
   CKW_CFG_DELETE_UE    -  Deletes the RLC entities per UE\n


 * @param[in] pst       -  Point to the pst structure
 * @param[in] transId   -  Transaction ID. This field will uniquily identifies
 *                         transaction between RRC and RLC.
 * @param[in] cfgInfo   -  This structure contains the configuration information
 * @return ROK
 */
//EXTERN S16 KwUiCkwCfgReq ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));
EXTERN S16 KwUiCkwCfgReq ARGS((Pst *pst, CkwCfgInfo *cfgInfo));
EXTERN S16 RlcDuappProcUeCreateReq ARGS((Pst *pst, CkwCfgInfo *cfgInfo));

/**
 *@details This primitive is used by RLC to confirm the configuration requested
 by RRC.

 * The different values for status are:\n
 * CKW_CFG_CFM_OK    -  Success\n
 * CKW_CFG_CFM_NOK   -  Failure\n

 * The different reasons for the failure are:\n
 * CKW_CFG_REAS_TYPE_MIS      -  Logical channel type mismatch with mode\n
 * CKW_CFG_REAS_ENT_UNKWN     -  Reconfigure request for an unknown RLC entity\n

 * @param[in] pst       -  Point to the pst structure.
 * @param[in] transId   -  Transaction ID. This field will uniquily identifies
 * transaction between RRC and RLC.
 * @param[in] cfmInfo   -  This structure contains the configuration confirm
 * information.
 * @return ROK
 */
S16 KwUiCkwCfgCfm ARGS((Pst *pst, SuId suId, CkwCfgCfmInfo *cfmInfo));

/**
 *@details This primitive is used by RRC to change the UeId for the existing UE
           context.

 * @param[in] pst       -  Point to the pst structure
 * @param[in] spId      -  The ID of the service provider SAP in the RLC layer. 
 * @param[in] transId   -  Transaction ID. This field uniquily identifies
 *                         transaction between RRC and RLC.
 * @param[in] ueInfo    -  Old UE Id Info for which the change request has come. 
 * @param[in] newUeInfo -  New UE Id Info for existing UE context. 
 * @return ROK
 */
EXTERN S16 KwUiCkwUeIdChgReq ARGS((Pst *pst, SpId spId, U32 transId,
                                   CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));

/**
 *@details This primitive is used by RLC to confirm the UeId change requested
 by RRC.

 * The different values for status are:\n
 * CKW_CFG_CFM_OK    -  Success\n
 * CKW_CFG_CFM_NOK   -  Failure\n

 * The different reasons for the failure are:\n
 * CKW_CFG_REAS_UE_CREAT_FAIL -  UE Cb update failure \n
 * CKW_CFG_REAS_UE_UNKWN      -  Request received for an unknown UE Id 

 * @param[in] pst       -  Point to the pst structure.
 * @param[in] suId      -  The ID of the service user SAP associated with RLC
 * @param[in] transId   -  Transaction ID. This field uniquily identifies
 *                         transaction between RRC and RLC.
 * @param[in] ueInfo    -  UE Identity which is changed with the new Id for the same
 *                         context.
 * @param[in] CmStatus  -  Confirmation status with appropriate reason.
 * information.
 * @return ROK
 */
EXTERN S16 KwUiCkwUeIdChgCfm  ARGS((Pst *pst, SuId suId, U32 transId, 
                                   CkwUeInfo *ueInfo, CmStatus status));
/* RRC Extern declarations */
#ifdef NH
EXTERN S16 NhLiCkwBndReq ARGS((
         Pst *pst,
         SuId suId,
         SpId spId));

EXTERN S16 NhLiCkwUbndReq ARGS((
         Pst *pst,
         SpId spId,
         Reason reason));

EXTERN S16 NhLiCkwBndCfm ARGS((Pst *pst,
         SuId suId,
         U8 status));

EXTERN S16 NhLiCkwCfgReq ARGS((
         Pst *pst,
         SpId spId,
         CkwCfgInfo *cfgInfo));

EXTERN S16 NhLiCkwCfgCfm ARGS((Pst *pst,
         SuId suId,
         CkwCfgCfmInfo* cfmInfo));

EXTERN S16 NhLiCkwUeIdChgReq ARGS((Pst *pst, 
         SpId spId, U32 transId,
         CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));

EXTERN S16 NhLiCkwUeIdChgCfm  ARGS((Pst *pst, 
         SuId suId, U32 transId, 
         CkwUeInfo *ueInfo, CmStatus status));

#endif /* NH */

/* Dummy RRC Interface primitives */
#ifdef DM
EXTERN S16 DmUiCkwBndReq ARGS((
         Pst *pst,
         SuId suId,
         SpId spId));

EXTERN S16 DmUiCkwUbndReq ARGS((
         Pst *pst,
         SpId spId,
         Reason reason));

EXTERN S16 DmUiCkwBndCfm ARGS((Pst *pst,
         SuId suId,
         U8 status));

EXTERN S16 DmUiCkwCfgReq ARGS((
         Pst *pst,
         SpId spId,
         CkwCfgInfo *cfgInfo));

EXTERN S16 DmUiCkwCfgCfm ARGS((Pst *pst,
         SuId suId,
         CkwCfgCfmInfo* cfmInfo));

EXTERN S16 DmUiCkwUeIdChgReq ARGS((Pst *pst, 
         SpId spId, U32 transId,
         CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));

EXTERN S16 DmUiCkwUeIdChgCfm  ARGS((Pst *pst, 
         SuId suId, U32 transId, 
         CkwUeInfo *ueInfo, CmStatus status));

#endif /* DM */

/*****************************************************************************
 *                      PACK/UNPACK FUNCTIONS
 ****************************************************************************/
#ifdef LCCKW
/* Packing functions */
EXTERN S16 cmPkCkwTmInfo ARGS ((
CkwTmInfo      *param,
Buffer         *mBuf
));

EXTERN S16 cmPkCkwUmInfo ARGS ((
CkwEntCfgInfo  *param,
Buffer         *mBuf
));

EXTERN S16 cmPkCkwAmInfo ARGS ((
CkwAmInfo      *param,
Buffer         *mBuf
));

EXTERN S16 cmPkCkwLChInfo ARGS ((
CkwLChInfo     *param,
Buffer         *mBuf
));

EXTERN S16 cmPkCkwEntCfgInfo ARGS ((
CkwEntCfgInfo  *param,
Buffer         *mBuf
));

EXTERN S16 cmPkCkwCfgInfo ARGS ((
CkwCfgInfo     *param,
Pst            *pst,
Buffer         *mBuf
));

EXTERN S16 cmPkCkwEntCfgCfmInfo ARGS ((
CkwEntCfgCfmInfo  *param,
Buffer            *mBuf
));

EXTERN S16 cmPkCkwCfgCfmInfo ARGS ((
CkwCfgCfmInfo  *param,
Pst            *pst,
Buffer         *mBuf
));

EXTERN S16 cmPkCkwUeInfo ARGS ((
CkwUeInfo *param,
Buffer    *mBuf
));

EXTERN S16 cmPkCkwBndReq ARGS ((
Pst *pst,
SuId suId,
SpId spId
));

EXTERN S16 cmPkCkwUbndReq ARGS ((
Pst *pst,
SpId spId,
Reason reason
));

EXTERN S16 cmPkCkwBndCfm ARGS ((
Pst *pst,
SuId suId,
U8 status
));

EXTERN S16 packUeCreateReq ARGS ((
Pst               *pst,
CkwCfgInfo        *cfgInfo
));

EXTERN S16 cmPkCkwCfgCfm ARGS ((
Pst               *pst,
SuId              suId,
CkwCfgCfmInfo     *cfgCfmInfo
));

EXTERN S16 cmPkCkwUeIdChgReq ARGS ((
Pst                *pst, 
SpId               spId, 
U32                transId,
CkwUeInfo          *ueInfo,
CkwUeInfo          *newUeInfo
));

EXTERN S16 cmPkCkwUeIdChgCfm  ARGS ((
Pst                *pst, 
SuId               suId, 
U32                transId, 
CkwUeInfo          *ueInfo, 
CmStatus           status
));

/* Unpacking functions */
EXTERN S16 cmUnpkCkwTmInfo ARGS ((
CkwTmInfo      *param,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwUmInfo ARGS ((
CkwEntCfgInfo  *param,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwAmInfo ARGS ((
CkwAmInfo      *param,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwLInfo ARGS ((
CkwLChInfo     *param,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwCfgCfmInfo ARGS ((
CkwCfgCfmInfo  *param,
Pst            *pst,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwBndReq ARGS ((
CkwBndReq      func,
Pst            *pst,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwLChInfo ARGS ((
CkwLChInfo     *param,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwEntCfgInfo ARGS ((
CkwEntCfgInfo  *param,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwCfgInfo ARGS ((
CkwCfgInfo     *param,
Pst            *pst,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwUeInfo ARGS ((
CkwUeInfo      *param,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwCfgCfm ARGS ((
CkwCfgCfm         func,
Pst               *pst,
Buffer            *mBuf
));

EXTERN S16 cmUnpkCkwUbndReq ARGS ((
CkwUbndReq     func,
Pst            *pst,
Buffer         *mBuf
));

EXTERN S16 cmUnpkCkwBndCfm ARGS ((
CkwBndCfm      func,
Pst            *pst,
Buffer         *mBuf
));

EXTERN S16 unpackUeCreateReq ARGS ((
CkwCfgReq         func,
Pst               *pst,
Buffer            *mBuf
));

EXTERN S16 cmUnpkCkwEntCfgCfmInfo ARGS ((
CkwEntCfgCfmInfo  *param,
Buffer            *mBuf
));

EXTERN S16 cmUnpkCkwUeIdChgReq ARGS ((
CkwUeIdChgReq      func,
Pst                *pst, 
Buffer             *mBuf
));

EXTERN S16 cmUnpkCkwUeIdChgCfm  ARGS ((
CkwUeIdChgCfm      func,
Pst                *pst, 
Buffer             *mBuf
));
#endif /* LCCKW */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CKW_X__ */


  
/********************************************************************30**
         End of file
**********************************************************************/
