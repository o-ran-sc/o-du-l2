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
  
     Desc:     C source code for Upper Interface Module 
  
     File:     rg_ptui.c 
  
**********************************************************************/

/** @file rg_ptui.c
@brief This file contains the definitions for Upper Interface(RGR/CRG/RGU) 
       primitives that are invoked from MAC to its service users.
       Portable functions corresponding to these primitives are also defined.
*/
/* header include files (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgr.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "rgm.h"           /* RGM Interface defines*/

/* header/extern include files (.x) */
#include "crg.x"           /* CRG Interface includes */
#include "rgr.x"           /* RGR Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "rgm.x"           /* RGM Interface includes*/
#include "ss_rbuf.h"
#include "ss_rbuf.x"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !(defined(LCRGUIRGU) && defined(LWLCRGUIRGU)  && defined(KW))
#define PTRGUIRGU
#endif

#if !(defined(LCRGUICRG)  && defined(NH))
#define PTRGUICRG
#endif

#if !(defined(LCRGUIRGR)  && defined(NX))
#define PTRGUIRGR
#endif

#if !(defined(LCRGUIRGM)  && defined(RM))
#define PTRGUIRGM
#endif

/* MAX Number of Service Users of RG */
#define RG_MAX_RGU_USR   3

/* MAX Number of Service Users of RG */
#define RG_MAX_CRG_USR   2

/* MAX Number of Service Users of RG */
#define RG_MAX_RGR_USR   2

/* MAX Number of Service Users of RG */
#define RG_MAX_RGM_USR   2
#ifdef PTRGUIRGU
/** @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps */
EXTERN S16 PtUiRguBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels*/
EXTERN S16 PtUiRguCDatInd ARGS((Pst* pst, SuId suId, RguCDatIndInfo  * datInd));
/** @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels*/
EXTERN S16 PtUiRguDDatInd ARGS((Pst* pst, SuId suId, RguDDatIndInfo  * datInd));
/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels. */
EXTERN S16 PtUiRguCStaInd ARGS((Pst* pst, SuId suId, RguCStaIndInfo  * staInd));
/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels. */
EXTERN S16 PtUiRguDStaInd ARGS((Pst* pst, SuId suId, RguDStaIndInfo  * staInd));
#ifdef LTE_L2_MEAS
/**@brief HARQ Status Indication from MAC to RLC */
EXTERN S16 PtUiRguHqStaInd ARGS((Pst* pst,SuId suId,
         RguHarqStatusInd *harqStatusInd));
#endif
/**@brief PDB FLow Control Indication from MAC to RLC */
EXTERN S16 PtUiRguFlowCntrlInd ARGS((Pst* pst,SuId suId,
         RguFlowCntrlInd *flowCntrlInd));
#endif /*--#ifdef PTRGUIRGU--*/
S16 RgUiRguFlowCntrlInd(Pst* pst, SuId suId, RguFlowCntrlInd *flowCntrlInd);
#ifdef LTE_L2_MEAS
#ifdef MAC_RLC_HARQ_STA_RBUF
S16 RgUiRguHqStaIndRbuf ARGS((Pst* pst,SuId suId,RguHarqStatusInd *harqStatusInd));
#endif 
#endif

#if defined(MAC_RLC_UL_RBUF) && !defined(SS_RBUF)
PRIVATE S16  RgUiRguDDatIndRbuf ARGS((RguDDatIndInfo  *datInd));
#endif
EXTERN Void rgFreeSharableSBuf ARGS((Data **data, Size size));

#ifdef RGR_CQI_REPT 
EXTERN S16 PtUiRgrStaInd ARGS(( Pst* pst, SuId suId, RgrStaIndInfo *staInd));
#endif
/* LTE_ADV_FLAG_REMOVED_START */
EXTERN S16 PtUiRgrLoadInfInd ARGS(( Pst* pst, SuId suId, RgrLoadInfIndInfo *loadInfInd));
/* LTE_ADV_FLAG_REMOVED_END */
EXTERN S16 PtUiRgrUeStaInd ARGS
((
Pst* pst, 
SuId suId, 
RgrUeStaIndInfo *staInd
));
#ifdef PTRGUICRG
/** @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps */
EXTERN S16 PtUiCrgBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief Configuration Confirm from MAC to RRC */
EXTERN S16 PtUiCrgCfgCfm ARGS((Pst* pst, SuId suId, CrgCfgTransId transId, U8 status));
#endif /*--#ifdef PTRGUICRG--*/

#ifdef PTRGUIRGR
/** @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps */
EXTERN S16 PtUiRgrBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief Configuration Confirm from MAC to RRM */
EXTERN S16 PtUiRgrCfgCfm ARGS((Pst* pst, SuId suId, RgrCfgTransId transId, U8 status));
EXTERN S16 PtUiRgrTtiInd ARGS((Pst* pst, SuId suId, RgrTtiIndInfo *ttiInd));
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
EXTERN S16 PtUiRgrSiCfgCfm ARGS((Pst* pst, SuId suId, RgrCfgTransId transId, U8 status));
EXTERN S16 PtUiRgrWarningSiCfgCfm ARGS((Pst* pst, SuId suId, 
                                RgrCfgTransId transId, U8 siId, U8 status));
#endif/*RGR_SI_SCH*/
#endif /*--#ifdef PTRGUIRGR--*/

#ifdef PTRGUIRGM
S16 PtUiRgmPrbRprtInd ARGS((Pst* pst, SuId suId, RgmPrbRprtInd *prbRprtInd));
S16 PtUiRgmBndCfm ARGS((Pst* pst, SuId suId, U8 status));
S16 PtUiRgmTransModeInd  ARGS((Pst* pst, SuId suId, RgmTransModeInd *transModeInd));
#endif
S16 RgUiRgmSendPrbRprtInd ARGS((Pst* pst, SuId suId, RgmPrbRprtInd *prbRprtInd));
S16 RgUiRgmChangeTransModeInd ARGS((Pst* pst, SuId suId, RgmTransModeInd *transModeInd));

S16 RgUiRguFlowCntrlInd ARGS((Pst* pst, SuId suId, RguFlowCntrlInd *flowCntrlInd));
/* Added for sending TTI tick to RRM */
/** @brief TTI indication from MAC to RRM */
PRIVATE CONSTANT RgrTtiInd RgUiRgrTtiIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrTtiInd,
#else
   PtUiRgrTtiInd,
#endif
#ifdef NX
   NxLiRgrTtiInd,
#else
   PtUiRgrTtiInd,
#endif
};


/** @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps */
PRIVATE CONSTANT RgrBndCfm RgUiRgrBndCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrBndCfm,
#else
   PtUiRgrBndCfm,
#endif
#ifdef NX
   NxLiRgrBndCfm,
#else
   PtUiRgrBndCfm,
#endif
};

/** @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps  via RGM interface*/
PRIVATE CONSTANT RgmBndCfm RgUiRgmBndCfmMt[RG_MAX_RGM_USR] =
{
#ifdef RGM_LC
   cmPkRgmBndCfm,
#else
   PtUiRgmBndCfm,
#endif
#ifdef RM
   RmLiRgmBndCfm, /*To be added by RRM*/
#else
   PtUiRgmBndCfm,
#endif
};


/** @brief Configuration Confirm from MAC to RRM */
PRIVATE CONSTANT RgrCfgCfm RgUiRgrCfgCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrCfgCfm,
#else
   PtUiRgrCfgCfm,
#endif
#ifdef NX
   NxLiRgrCfgCfm,
#endif
};

/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/** @brief SI Configuration Confirm from MAC to RRM */
PRIVATE CONSTANT RgrSiCfgCfm RgUiRgrSiCfgCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrSiCfgCfm,
#else
   PtUiRgrSiCfgCfm,
#endif
#ifdef NX
   NxLiRgrSiCfgCfm,
#else
   PtUiRgrSiCfgCfm,
#endif
};


/** @brief Warning SI Configuration Confirm from MAC to RRM */
PRIVATE CONSTANT RgrWarningSiCfgCfm RgUiRgrWarningSiCfgCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrWarningSiCfgCfm,
#else
   PtUiRgrWarningSiCfgCfm,
#endif
#ifdef NX
   NxLiRgrWarningSiCfgCfm,
#else
   PtUiRgrWarningSiCfgCfm,
#endif
};

#endif/*RGR_SI_SCH */
/** @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps */
PRIVATE CONSTANT CrgBndCfm RgUiCrgBndCfmMt[RG_MAX_CRG_USR] =
{
#ifdef LCRGUICRG
   cmPkCrgBndCfm,
#else
   PtUiCrgBndCfm,
#endif
#ifdef NH
   NhLiCrgBndCfm,
#else
   PtUiCrgBndCfm,
#endif
};

/** @brief Configuration Confirm from MAC to RRC */
PRIVATE CONSTANT CrgCfgCfm RgUiCrgCfgCfmMt[RG_MAX_CRG_USR] =
{
#ifdef LCRGUICRG
   cmPkCrgCfgCfm,
#else
   PtUiCrgCfgCfm,
#endif
#ifdef NH
   NhLiCrgCfgCfm,
#else
   PtUiCrgCfgCfm,
#endif
};


/** @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps */
PRIVATE CONSTANT RguBndCfm RgUiRguBndCfmMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguBndCfm,
#else
   PtUiRguBndCfm,
#endif
#ifdef KW
   RlcLiRguBndCfm,
#else
   PtUiRguBndCfm,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguBndCfm,
#else
   PtUiRguBndCfm,
#endif
};

/** @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels*/
PRIVATE CONSTANT RguCDatInd RgUiRguCDatIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
//   cmPkRguCDatInd, /* TODO : Call packRcvdUlData instead */
#else
   PtUiRguCDatInd,
#endif
#ifdef KW
   //RlcProcCommLcUlData,
#else
   PtUiRguCDatInd,
#endif
#ifdef LWLCRGUIRGU
//   cmPkRguCDatInd, /* TODO:Call packRcvdUlData instead */
#else
   PtUiRguCDatInd,
#endif
};

/** @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels*/
PRIVATE CONSTANT RguDDatInd RgUiRguDDatIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
//   cmPkRguDDatInd,  /* TODO: Call packRcvdUlData instead */
#else
   PtUiRguDDatInd,
#endif
#ifdef KW
   //RlcProcDedLcUlData,
#else
   PtUiRguDDatInd,
#endif
#ifdef LWLCRGUIRGU
//   cmPkRguDDatInd, /* TODO: Call packRcvdUlData instead */
#else
   PtUiRguDDatInd,
#endif
};

/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels. */
PRIVATE CONSTANT RguCStaInd RgUiRguCStaIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   //cmPkRguCStaInd, /* TODO: Call packSchedRep instead of cmPkRguCStaInd */
#else
   PtUiRguCStaInd,
#endif
#ifdef KW
   //RlcMacProcSchedRep,
#else
   PtUiRguCStaInd,
#endif
#ifdef LWLCRGUIRGU
   //cmPkRguCStaInd, /* TODO: Call packSchedRep instead of cmPkRguCStaInd */
#else
   PtUiRguCStaInd,
#endif
};

/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels. */
PRIVATE CONSTANT RguDStaInd RgUiRguDStaIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   //cmPkRguDStaInd, /* TODO: Call packSchedRep instead of cmPkRguDStaInd */
#else
   PtUiRguDStaInd,
#endif
#ifdef KW
   //RlcMacProcSchedRep,
#else
   PtUiRguDStaInd,
#endif
#ifdef LWLCRGUIRGU
   //cmPkRguDStaInd, /* TODO: Call packSchedRep instead of cmPkRguDStaInd */
#else
   PtUiRguDStaInd,
#endif
};

#ifdef LTE_L2_MEAS
/** @brief HARQ Status Indication from MAC to RLC  
 * as a response to the DdatReq primitive from RLC.
 * Informs RLC of the successful transmission of TB's 
 * (ACK/NACK) along with Mapping Info. */
PRIVATE CONSTANT RguHqStaInd RgUiRguHqStaIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguHqStaInd,
#else
   PtUiRguHqStaInd,
#endif
#ifdef KW
   RlcLiRguHqStaInd,
#else
   PtUiRguHqStaInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguHqStaInd,
#else
   PtUiRguHqStaInd,
#endif
};
#endif /* LTE_L2_MEAS */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/** @brief TTI indication from MAC to RRM */
PRIVATE CONSTANT RgrStaInd RgUiRgrStaIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrStaInd,
#else
   PtUiRgrStaInd,
#endif
#ifdef NX
   NxLiRgrStaInd,
#else
   PtUiRgrStaInd,
#endif
};
#endif /* RGR_CQI_REPT */

/* LTE_ADV_FLAG_REMOVED_START */
/** @brief LOAD INF indication from MAC to RRM */
PRIVATE CONSTANT RgrLoadInfInd RgUiRgrLoadInfIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrLoadInfInd,
#else
   PtUiRgrLoadInfInd,
#endif
#ifdef NX
   NxLiRgrLoadInfInd,
#else
   PtUiRgrLoadInfInd,
#endif
};
/* LTE_ADV_FLAG_REMOVED_END */

/** @brief TTI indication from MAC to RRM */
PRIVATE CONSTANT RgrUeStaInd RgUiRgrUeStaIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrUeStaInd,
#else
   PtUiRgrUeStaInd,
#endif
#ifdef NX
   NxLiRgrUeStaInd,
#else
   PtUiRgrUeStaInd,
#endif
};

/** @brief Average PRB usage indication from MAC to RRM */
PRIVATE CONSTANT RgmPrbRprtIndFptr RgUiRgmSendPrbRprtIndMt[RG_MAX_RGM_USR] =
{
#ifdef RGM_LC
   cmPkRgmPrbRprtInd,
#else
   PtUiRgmPrbRprtInd,
#endif
#ifdef RM
   RmLiRgmPrbRprtInd,
#else
   PtUiRgmPrbRprtInd,
#endif
};


/** @brief Transmission Mode Change indication from MAC to RRM */
PRIVATE CONSTANT RgmTransModeIndFptr RgUiRgmChangeTransModeIndMt[RG_MAX_RGM_USR] =
{
#ifdef RGM_LC
   cmPkRgmTransModeInd,
#else
   PtUiRgmTransModeInd,
#endif
#ifdef RM
   RmLiRgmTransModeInd,
#else
   PtUiRgmTransModeInd,
#endif
};

PRIVATE CONSTANT RguFlowCntrlIndInfo RgUiRguFlowCntrlIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguFlowCntrlInd,
#else
   PtUiRguFlowCntrlInd,
#endif
#ifdef KW
   RlcLiRguFlowCntrlInd,
#else
   PtUiRguFlowCntrlInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguFlowCntrlInd,
#else
   PtUiRguFlowCntrlInd,
#endif
};
#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef RG

/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : RgUiRgrBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgrBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 RgUiRgrBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   return ((*RgUiRgrBndCfmMt[pst->selector])(pst, suId, status));

}
/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps via RGM interface
*
* @details
*
*     Function : RgUiRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgmBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 RgUiRgmBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   return ((*RgUiRgmBndCfmMt[pst->selector])(pst, suId, status));

}



/* Added for sending TTI tick to RRM */

/**
* @brief TTI indication from MAC to RGR user.
*
* @details
*
*     Function : RgUiRgrTtiInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrTtiIndInfo  ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgrTtiInd
(
Pst* pst,
SuId suId,
RgrTtiIndInfo  *ttiInd
)
#else
S16 RgUiRgrTtiInd(pst, suId, ttiInd)
Pst* pst;
SuId suId;
RgrTtiIndInfo *ttiInd;
#endif
{

   return ((*RgUiRgrTtiIndMt[pst->selector])(pst, suId, ttiInd));

}



/**
* @brief Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : RgUiRgrCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgrCfgCfm
(
Pst* pst,
RgrCfgTransId transId,
U8 status
)
#else
S16 RgUiRgrCfgCfm(pst, transId, status)
Pst* pst;
RgrCfgTransId transId;
U8 status;
#endif
{

   return ((*RgUiRgrCfgCfmMt[pst->selector])(pst, transId, status));

}


/**
* @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : RgUiCrgBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiCrgBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 RgUiCrgBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   return ((*RgUiCrgBndCfmMt[pst->selector])(pst, suId, status));

}



/**
* @brief Configuration Confirm from MAC to RRC
*
* @details
*
*     Function : RgUiCrgCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiCrgCfgCfm
(
Pst* pst,
SuId suId,
CrgCfgTransId transId,
U8 status
)
#else
S16 RgUiCrgCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CrgCfgTransId transId;
U8 status;
#endif
{

   return ((*RgUiCrgCfgCfmMt[pst->selector])(pst, suId, transId, status));

}


/**
* @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : RgUiRguBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRguBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 RgUiRguBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   return ((*RgUiRguBndCfmMt[pst->selector])(pst, suId, status));

}

int macDDatIndSnt;
int macCDatIndSnt;


/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels
*
* @details
*
*     Function : RgUiRguCDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRguCDatInd
(
Pst* pst,
SuId suId,
RguCDatIndInfo  * datInd
)
#else
S16 RgUiRguCDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguCDatIndInfo  * datInd;
#endif
{
   macCDatIndSnt++;

   return ((*RgUiRguCDatIndMt[pst->selector])(pst, suId, datInd));

}


/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgUiRguDDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRguDDatInd
(
Pst* pst,
SuId suId,
RguDDatIndInfo  * datInd
)
#else
S16 RgUiRguDDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguDDatIndInfo  * datInd;
#endif
{
   macDDatIndSnt++;

#if defined(MAC_RLC_UL_RBUF) && !defined(SS_RBUF)
   return (RgUiRguDDatIndRbuf(datInd));
#else 
   return ((*RgUiRguDDatIndMt[pst->selector])(pst, suId, datInd));
#endif    
}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels.
*
* @details
*
*     Function : RgUiRguCStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRguCStaInd
(
Pst* pst,
SuId suId,
RguCStaIndInfo  * staInd
)
#else
S16 RgUiRguCStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguCStaIndInfo  * staInd;
#endif
{

   return ((*RgUiRguCStaIndMt[pst->selector])(pst, suId, staInd));

}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
*
* @details
*
*     Function : RgUiRguDStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRguDStaInd
(
Pst* pst,
SuId suId,
RguDStaIndInfo  * staInd
)
#else
S16 RgUiRguDStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguDStaIndInfo  * staInd;
#endif
{

   return ((*RgUiRguDStaIndMt[pst->selector])(pst, suId, staInd));

}
#ifdef LTE_L2_MEAS
/* TODO: Function header */
#ifdef ANSI
S16 RgUiRguHqStaInd
(
Pst* pst,
SuId suId,
RguHarqStatusInd *harqStatusInd
)
#else
S16 RgUiRguHqStaInd(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *harqStatusInd;
#endif
{

#if defined(SPLIT_RLC_DL_TASK) && defined(MAC_RLC_HARQ_STA_RBUF)
   S16 ret=ROK;
   ret = RgUiRguHqStaIndRbuf(pst,suId,harqStatusInd);
   return (ret);
#else 
   return ((*RgUiRguHqStaIndMt[pst->selector])(pst, suId, harqStatusInd));
#endif 

} /* RgUiRguHqStaInd */
#endif /* LTE_L2_MEAS */

#ifdef ANSI
S16 RgUiRguFlowCntrlInd
(
Pst* pst,
SuId suId,
RguFlowCntrlInd *flowCntrlInd
)
#else
S16 RgUiRguFlowCntrlInd(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguFlowCntrlInd *flowCntrlInd;
#endif
{
  return ((*RgUiRguFlowCntrlIndMt[pst->selector])(pst, suId, flowCntrlInd));
}
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
* @brief SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : RgUiRgrSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgrSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 status
)
#else
S16 RgUiRgrSiCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8 status;
#endif
{

   return ((*RgUiRgrSiCfgCfmMt[pst->selector])(pst, suId, transId, status));

}

/**
* @brief Warning SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : RgUiRgrWarningSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8   siId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgrWarningSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8   siId,
U8 status
)
#else
S16 RgUiRgrWarningSiCfgCfm(pst, suId, transId, siId,status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8  siId;
U8 status;
#endif
{
   return ((*RgUiRgrWarningSiCfgCfmMt[pst->selector])
         (pst, suId, transId, siId, status));
}

#endif/*RGR_SI_SCH*/


/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/**
* @brief STA indication from MAC to RGR user.
*
* @details
*
*     Function : RgUiRgrStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrStaIndInfo  *staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgrStaInd
(
Pst* pst,
SuId suId,
RgrStaIndInfo  *staInd
)
#else
S16 RgUiRgrStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RgrStaIndInfo *staInd;
#endif
{

   return ((*RgUiRgrStaIndMt[pst->selector])(pst, suId, staInd));

}
#endif /* End of RGR_CQI_REPT */

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief LOAD INF indication from MAC to RGR user.
 *
 * @details
 *
 *     Function : RgUiRgrLoadInfInd
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RgrLoadInfIndInfo  *loadInfInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
S16 RgUiRgrLoadInfInd
(
 Pst* pst,
 SuId suId,
 RgrLoadInfIndInfo  *loadInfInd
 )
#else
S16 RgUiRgrLoadInfInd(pst, suId, loadInfInd)
   Pst* pst;
   SuId suId;
   RgrLoadInfIndInfo *loadInfInd;
#endif
{

   return ((*RgUiRgrLoadInfIndMt[pst->selector])(pst, suId, loadInfInd));

}
/* LTE_ADV_FLAG_REMOVED_END */

/**
* @brief UESTA indication from MAC to RGR user.
*
* @details
*
*     Function : RgUiRgrUeStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrStaIndInfo  *staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgrUeStaInd
(
Pst             *pst,
SuId            suId,
RgrUeStaIndInfo *ueStaInd
)
#else
S16 RgUiRgrUeStaInd(pst, suId, ueStaInd)
Pst             *pst;
SuId            suId;
RgrUeStaIndInfo *ueStaInd;
#endif
{

   return ((*RgUiRgrUeStaIndMt[pst->selector])(pst, suId, ueStaInd));

}
#endif /*--ifdef RG--*/

#ifdef PTRGUICRG

/**
* @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : PtUiCrgBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiCrgBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 PtUiCrgBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   return ROK;

}



/**
* @brief Configuration Confirm from MAC to RRC
*
* @details
*
*     Function : PtUiCrgCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiCrgCfgCfm
(
Pst* pst,
SuId suId,
CrgCfgTransId transId,
U8 status
)
#else
S16 PtUiCrgCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CrgCfgTransId transId;
U8 status;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(status);

   return ROK;

}
#endif /*--ifdef PTRGUICRG--*/

#ifdef PTRGUIRGU

/**
* @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : PtUiRguBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRguBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 PtUiRguBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   return ROK;

}



/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels
*
* @details
*
*     Function : PtUiRguCDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRguCDatInd
(
Pst* pst,
SuId suId,
RguCDatIndInfo  * datInd
)
#else
S16 PtUiRguCDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguCDatIndInfo  * datInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datInd);

   return ROK;

}



/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : PtUiRguDDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRguDDatInd
(
Pst* pst,
SuId suId,
RguDDatIndInfo  * datInd
)
#else
S16 PtUiRguDDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguDDatIndInfo  * datInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datInd);

   return ROK;

}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels.
*
* @details
*
*     Function : PtUiRguCStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRguCStaInd
(
Pst* pst,
SuId suId,
RguCStaIndInfo  * staInd
)
#else
S16 PtUiRguCStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguCStaIndInfo  * staInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

   return ROK;

}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
*
* @details
*
*     Function : PtUiRguDStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRguDStaInd
(
Pst* pst,
SuId suId,
RguDStaIndInfo  * staInd
)
#else
S16 PtUiRguDStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguDStaIndInfo  * staInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

   return ROK;

}
#ifdef LTE_L2_MEAS
/* TODO: Function Header */
#ifdef ANSI
S16 PtUiRguHqStaInd
(
Pst* pst,
SuId suId,
RguHarqStatusInd *harqStatusInd
)
#else
S16 PtUiRguHqStaInd(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *harqStatusInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(harqStatusInd);

   return ROK;

}
#endif /* LTE_L2_MEAS */

#ifdef ANSI
S16 PtUiRguFlowCntrlInd
(
Pst* pst,
SuId suId,
RguFlowCntrlInd *flowCntrlInd
)
#else
S16 PtUiRguFlowCntrlInd(pst, suId, flowCntrlInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *flowCntrlInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(flowCntrlInd);

   return ROK;

}

#endif /*--ifdef PTRGUIRGU--*/

#ifdef PTRGUIRGR

/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : PtUiRgrBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgrBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 PtUiRgrBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   return ROK;

}

/* Added for sending TTI tick to RRM */

/**
* @brief TTI indication from MAC to RGR user.
*
* @details
*
*     Function : PtUiRgrTtiInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrTtiIndInfo  *ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgrTtiInd
(
Pst* pst,
SuId suId,
RgrTtiIndInfo *ttiInd
)
#else
S16 PtUiRgrTtiInd(pst, suId, ttiInd)
Pst* pst;
SuId suId;
RgrTtiIndInfo *ttiInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(ttiInd);

   return ROK;

}



/**
* @brief Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : PtUiRgrCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgrCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 status
)
#else
S16 PtUiRgrCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8 status;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(status);

   return ROK;

}

/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
* @brief SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : PtUiRgrSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgrSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 status
)
#else
S16 PtUiRgrSiCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8 status;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(status);

   return ROK;

}

/**
* @brief Warning SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : PtUiRgrWarningSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8 siId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgrWarningSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 siId,
U8 status
)
#else
S16 PtUiRgrWarningSiCfgCfm(pst, suId, transId, siId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8  siId;
U8 status;
#endif
{
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(siId);
   UNUSED(status);

   return ROK;
}
#endif/* RGR_SI_SCH */


/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/**
* @brief STA indication from MAC to RGR user.
*
* @details
*
*     Function : PtUiRgrStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrStaIndInfo  *staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgrStaInd
(
Pst* pst,
SuId suId,
RgrStaIndInfo  *staInd
)
#else
S16 PtUiRgrStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RgrStaIndInfo *staInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

   return ROK;

}
#endif /* End of RGR_CQI_REPT */
/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief LOAD INF indication from MAC to RGR user.
 *
 * @details
 *
 *     Function : PtUiRgrLoadInfInd
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RgrLoadInfIndInfo  *loadInfInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
S16 PtUiRgrLoadInfInd
(
 Pst* pst,
 SuId suId,
 RgrLoadInfIndInfo  *loadInfInd
 )
#else
S16 PtUiRgrLoadInfInd(pst, suId, loadInfInd)
   Pst* pst;
   SuId suId;
   RgrLoadInfIndInfo *loadInfInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(loadInfInd);

   return ROK;

}
/* LTE_ADV_FLAG_REMOVED_END */

/**
* @brief STA indication from MAC to RGR user.
*
* @details
*
*     Function : PtUiRgrUeStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrUeStaIndInfo  *ueStaInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgrUeStaInd
(
Pst* pst,
SuId suId,
RgrUeStaIndInfo  *ueStaInd
)
#else
S16 PtUiRgrUeStaInd(pst, suId, ueStaInd)
Pst* pst;
SuId suId;
RgrUeStaIndInfo *ueStaInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(ueStaInd);

   return ROK;

}
#endif /*--ifdef PTRGUIRGR--*/


#ifdef PTRGUIRGM
/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps via RGM interface
*
* @details
*
*     Function : PtUiRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgmBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
S16 PtUiRgmBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   return ROK;

}

/**
* @brief Average PRB indication from MAC to RRM
*
* @details
*
*     Function : PtUiRgmPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrUeStaIndInfo  *ueStaInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgmPrbRprtInd
(
Pst* pst,
SuId suId,
RgmPrbRprtInd *prbRprtInd
)
#else
S16 PtUiRgmPrbRprtInd(pst, suId, prbRprtInd)
Pst* pst;
SuId suId;
RgmPrbRprtInd *prbRprtInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(prbRprtInd);

   return ROK;

}

/**
* @brief Transmission Mode Change indication from MAC to RRM
*
* @details
*
*     Function : PtUiRgmTransModeInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmTransModeInd *transModeInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 PtUiRgmTransModeInd
(
Pst* pst,
SuId suId,
RgmTransModeInd *transModeInd
)
#else
S16 PtUiRgmTransModeInd(pst, suId, transModeInd)
Pst* pst;
SuId suId;
RgmTransModeInd *transModeInd;
#endif
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transModeInd);

   return ROK;

}
#endif

/**
* @brief Average PRB indication from MAC to RRM
*
* @details
*
*     Function : RgUiRgmSendPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmPrbRprtInd prbRprtInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgmSendPrbRprtInd
(
Pst* pst,
SuId suId,
RgmPrbRprtInd *prbRprtInd
)
#else
S16 RgUiRgmSendPrbRprtInd(pst, suId, prbRprtInd)
Pst* pst;
SuId suId;
RgmPrbRprtInd *prbRprtInd;
#endif

{
   return ((*RgUiRgmSendPrbRprtIndMt[pst->selector])(pst, suId, prbRprtInd));
}

/**
* @brief Transmission Mode change indication from MAC to RRM
*
* @details
*
*     Function : RgUiRgmChangeTransModeInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmTransModeInd transModeInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
S16 RgUiRgmChangeTransModeInd
(
Pst* pst,
SuId suId,
RgmTransModeInd *transModeInd
)
#else
S16 RgUiRgmChangeTransModeInd(pst, suId, transModeInd)
Pst* pst;
SuId suId;
RgmTransModeInd *transModeInd;
#endif

{
   return ((*RgUiRgmChangeTransModeIndMt[pst->selector])(pst, suId, transModeInd));
}

#if defined(MAC_RLC_UL_RBUF) && !defined(SS_RBUF)
PRIVATE S16 RgUiRguDDatIndRbuf(RguDDatIndInfo  *datInd)
{
      S16  ret = ROK;
      Void *elem = NULLP;
      RguDedDatInd1 *rguDatIndl = NULLP;
      elem = SRngGetWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
      rguDatIndl = (RguDedDatInd1 *) elem;
      if (NULLP == elem)
      {
         SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].pktDrop++;
         U32 i,j;
         for(i = 0; i< datInd->numLch; i++)
         {
            for(j = 0; j < datInd->lchData[i].pdu.numPdu; j++)
            {
               if(datInd->lchData[i].pdu.mBuf[j])
               {
                  SPutMsg(datInd->lchData[i].pdu.mBuf[j]);
               }
            }
         }
         rgFreeSharableSBuf((Data **)&datInd,sizeof(RguDDatIndInfo));
         ret = RFAILED;
      }
      else
      {
         rguDatIndl->msg = datInd;
         SRngIncrWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
         SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].pktRate++;
      }
  return (ret);
}
#endif
#ifdef RLC_MAC_DAT_REQ_RBUF
#ifdef ANSI
S16 rgDlDatReqBatchProc
(
Void
)
#else
S16 rgDlDatReqBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */

   U8 rngBufDeqIndx = 0;
   PRIVATE Pst rgDDatRbfuPst ={1,1,ENTMAC,0,ENTRLC,1,PRIOR0,RTESPEC,EVTRGUDDATREQ,0,0,2,0};
   PRIVATE Pst rgCDatRbfuPst ={1,1,ENTMAC,0,ENTRLC,1,PRIOR0,RTESPEC,EVTRGUCDATREQ,0,0,2,0};
   Void *elmIndx = NULLP;
   RguInfoRingElem *datReqRing=NULLP;
   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
   while(NULLP != elmIndx)
   {
      datReqRing= (RguInfoRingElem *)elmIndx;
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].nPktProc++;;//Number of pkt processed in tti
      if(datReqRing->msg)
      {
         if(datReqRing->event == EVTRGUDDATREQ)
         {
         RgUiRguDDatReq(&rgDDatRbfuPst, datReqRing->spId, (RguDDatReqInfo *) datReqRing->msg);
         }
         else
         { 
          //  printf("CSHP:*** Received CDatReq in MAC Ring  \n");
         RgUiRguCDatReq(&rgCDatRbfuPst, datReqRing->spId, (RguCDatReqInfo *) datReqRing->msg); 
         }
      }
      else
      {
	      printf(" Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->read,
              SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->write);
	      /* Due to the cache issue we are verifying the mbuf pointer again and sending it to rlc if avilable*/
              if(datReqRing->msg)
	      {
		      if(datReqRing->event == EVTRGUDDATREQ)
			      RgUiRguDDatReq(&rgDDatRbfuPst, datReqRing->spId, (RguDDatReqInfo *) datReqRing->msg);
		      else 
			      RgUiRguCDatReq(&rgCDatRbfuPst, datReqRing->spId, (RguCDatReqInfo *) datReqRing->msg); 
	      }else 
	      {
		      printf(" still Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->read,
		      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->write);
	      } 
      }
      datReqRing->msg=NULLP;
      SRngIncrRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
      datReqRing->event=0; 
      elmIndx = NULLP;
      datReqRing= NULLP;
      rngBufDeqIndx++;

      if(rngBufDeqIndx >= SS_RNG_MAX_DLRLC_TO_DLMAC_DAT_REQ_DQ_CNT)
        break;

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ)) == NULLP)
      break;
   }
 
   return ROK;
}
#endif 

#ifdef RLC_MAC_STA_RSP_RBUF
#ifdef ANSI
S16 rgDlStaRspBatchProc
(
Void
)
#else
S16 rgDlStaRspBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */

   U8 rngBufDeqIndx = 0;
   PRIVATE Pst rgDStaRbfuPst ={1,1,ENTMAC,0,ENTRLC,1,PRIOR0,RTESPEC,EVTRGUDSTARSP,0,0,2,0};
   PRIVATE Pst rgCStaRbfuPst ={1,1,ENTMAC,0,ENTRLC,1,PRIOR0,RTESPEC,EVTRGUCSTARSP,0,0,2,0};
   Void *elmIndx = NULLP;
   RguInfoRingElem *staRspRing=NULLP;

   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
   while(NULLP != elmIndx)
   {
      staRspRing= (RguInfoRingElem *)elmIndx;
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].nPktProc++;;//Number of pkt processed in tti
      
      if(staRspRing->msg!= NULLP)
      {
         if( staRspRing->event == EVTRGUDSTARSP)
         {
         RgUiRguDStaRsp(&rgDStaRbfuPst, staRspRing->spId, (RguDStaRspInfo *) staRspRing->msg);
         }
         else
         {
         RgUiRguCStaRsp(&rgCStaRbfuPst, staRspRing->spId, (RguCStaRspInfo *) staRspRing->msg);
         } 
      }
      else
      {
	      printf(" Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->read,
			      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->write);
	      /* Due to the cache issue we are verifying the mbuf pointer again and sending it to rlc if avilable*/
              if(staRspRing->msg!= NULLP)
	      {
		      if( staRspRing->event == EVTRGUDSTARSP)
			      RgUiRguDStaRsp(&rgDStaRbfuPst, staRspRing->spId, (RguDStaRspInfo *) staRspRing->msg);
		      else  
			      RgUiRguCStaRsp(&rgCStaRbfuPst, staRspRing->spId, (RguCStaRspInfo *) staRspRing->msg);
	      }else 
	      {
		      printf(" still Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->read,
				      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->write);
	      } 
      }
      staRspRing->msg=NULLP;
      SRngIncrRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
      staRspRing->event =0;    
      elmIndx = NULLP;
      staRspRing= NULLP;

      rngBufDeqIndx++;

      if(rngBufDeqIndx >= SS_RNG_MAX_DLRLC_TO_DLMAC_STA_RSP_DQ_CNT)
        break;

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC)) == NULLP)
      break;
   }
 
   return ROK;
}
#endif 
#ifdef LTE_L2_MEAS
#ifdef MAC_RLC_HARQ_STA_RBUF
#ifdef ANSI
S16 RgUiRguHqStaIndRbuf 
(
Pst* pst,
SuId suId,
RguHarqStatusInd *harqStatusInd
)
#else
S16 RgUiRguHqStaIndRbuf(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *harqStatusInd;
#endif
{
   S16  ret = ROK;
   Void *elem = NULLP;
   RguHarqStaInd *harqStaInd = NULLP;
   elem = SRngGetWIndx(SS_RNG_BUF_MAC_HARQ);
   harqStaInd = (RguHarqStaInd*) elem;
   if (NULLP == elem )
   {
      SsRngInfoTbl[SS_RNG_BUF_MAC_HARQ].pktDrop++;
      ret = RFAILED;
   }
   else
   {
      memcpy(&(harqStaInd->hqStatusInd), harqStatusInd, sizeof(RguHarqStatusInd));
      memcpy(&(harqStaInd->pst), pst, sizeof(Pst));
      SRngIncrWIndx(SS_RNG_BUF_MAC_HARQ);
      SsRngInfoTbl[SS_RNG_BUF_MAC_HARQ].pktRate++;
   }
   return (ret);
}
#endif
#endif


/**********************************************************************
 
         End of file
**********************************************************************/
