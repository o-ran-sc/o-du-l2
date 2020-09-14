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
  
     Desc:     C source code for INF Interface Module 
  
     File:     rg_sch_pt.c 
  
**********************************************************************/

/** @file rg_sch_pt.c
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
#include "cm_mblk.h"       /* memory management */
#include "cm_lte.h"        /* Common LTE Defines */
#include "tfu.h"           /* RGU Interface defines */
#ifdef LTE_L2_MEAS
#include "lrg.h"
#endif
#include "rg_sch_inf.h"           /* RGU Interface defines */

/* header/extern include files (.x) */
#include "crg.x"           /* CRG Interface includes */
#include "rgr.x"           /* RGR Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "cm_mblk.x"       /* memory management */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"           /* RGU Interface includes */
#ifdef LTE_L2_MEAS
#include "lrg.x"
#endif
#include "rg_sch_inf.x"           /* RGU Interface includes */

#define RG_MAX_SCH 1

PRIVATE CONSTANT CellRegReq RgSchMacCellRegReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacCellRegReq,
#else
   RgSchMacCellRegReq
#endif
};

PRIVATE CONSTANT DedBoUpdtReq RgMacSchDedBoUpdtReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchDedBoUpdtReq,
#else
   RgMacSchDedBoUpdtReq
#endif
};

PRIVATE CONSTANT CmnBoUpdtReq RgMacSchCmnBoUpdtReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchCmnBoUpdtReq,
#else
   RgMacSchCmnBoUpdtReq
#endif
};

PRIVATE CONSTANT SfRecpInd RgMacSchSfRecpIndMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchSfRecpInd
#else
      RgMacSchSfRecpInd,
#endif
};
/*Fix: start: Inform UE delete to scheduler*/
PRIVATE CONSTANT UeDelInd RgMacSchUeDelIndMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchUeDelInd
#else
      RgMacSchUeDelInd,
#endif
};
/*Fix: end: Inform UE delete to scheduler*/
PRIVATE CONSTANT SfAllocReq RgSchMacSfAllocReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacSfAllocReq,
#else
   RgSchMacSfAllocReq
#endif
};

PRIVATE CONSTANT RstHqEntReq RgSchMacRstHqEntReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacRstHqEntReq,
#else
   RgSchMacRstHqEntReq
#endif
};
PRIVATE CONSTANT RlsHqReq RgSchMacRlsHqReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacRlsHqReq,
#else
   RgSchMacRlsHqReq
#endif
};

PRIVATE CONSTANT RlsRntiReq RgSchMacRlsRntiReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacRlsRntiReq,
#else
   RgSchMacRlsRntiReq
#endif
};

PRIVATE CONSTANT LcgReg RgSchMacLcgRegReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacLcgRegReq,
#else
   RgSchMacLcgRegReq
#endif
};

#ifdef LTEMAC_SPS
PRIVATE CONSTANT SpsLcReg RgSchMacSpsLcRegReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacSpsLcRegReq,
#else
   RgSchMacSpsLcRegReq
#endif
};

PRIVATE CONSTANT UlSpsReset RgSchMacUlSpsResetMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacUlSpsReset,
#else
   RgSchMacUlSpsResetReq
#endif
};



PRIVATE CONSTANT SpsLcDereg RgSchMacSpsLcDeregReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacSpsLcDeregReq,
#else
   RgSchMacSpsLcDeregReq
#endif
};

PRIVATE CONSTANT SpsRelInd RgMacSchSpsRelIndMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchSpsRelInd,
#else
   RgMacSchSpsRelInd
#endif
};

#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
PRIVATE CONSTANT L2MeasReq RgSchMacL2MeasReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacL2MeasReq
#else
      RgSchMacL2MeasReq
#endif
};
/*Added for Rsys oam*/
PRIVATE CONSTANT L2MeasStopReq RgSchMacL2MeasStopReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacL2MeasStopReq
#else
      RgSchMacL2MeasStopReq
#endif
};

PRIVATE CONSTANT L2MeasSendReq RgSchMacL2MeasSendReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacL2MeasSendReq
#else
      RgSchMacL2MeasSendReq
#endif
};

PRIVATE CONSTANT L2MeasCfm RgMacSchL2MeasCfmMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchL2MeasCfm
#else
      RgMacSchL2MeasCfm
#endif
};
PRIVATE CONSTANT L2MeasStopCfm RgMacSchL2MeasStopCfmMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchL2MeasStopCfm
#else
      RgMacSchL2MeasStopCfm
#endif
};
#endif/*LTE_L2_MEAS*/
/**
 * @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
 *
 * @details
 *
 *     Function : RgSchMacCellReg
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDDatIndInfo  *  datInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacCellReg
(
 Pst* pst,
 RgInfCellReg*   regReq
 )
#else
PUBLIC S16 RgSchMacCellReg(pst, regReq)
   Pst* pst;
   RgInfCellReg*   regReq;
#endif
{

   TRC3(RgSchMacCellReg);

   RETVALUE((*RgSchMacCellRegReqMt[0])(pst, regReq));
}

/**
 * @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
 *
 * @details
 *
 *     Function : RgMacSchDedBoUpdt
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDDatIndInfo  *  datInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgMacSchDedBoUpdt
(
 Pst* pst,
 RgInfDedBoRpt*  boRpt
 )
#else
PUBLIC S16 RgMacSchDedBoUpdt(pst, boRpt)
   Pst* pst;
   RgInfDedBoRpt*  boRpt;
#endif
{

   TRC3(RgMacSchDedBoUpdt);

   RETVALUE((*RgMacSchDedBoUpdtReqMt[0])(pst, boRpt));
}

/**
 * @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
 *
 * @details
 *
 *     Function : RgMacSchCmnBoUpdt
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDDatIndInfo  *  datInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgMacSchCmnBoUpdt
(
 Pst* pst,
 RgInfCmnBoRpt*  boRpt
 )
#else
PUBLIC S16 RgMacSchCmnBoUpdt(pst, boRpt)
   Pst* pst;
   RgInfCmnBoRpt*  boRpt;
#endif
{

   TRC3(RgMacSchCmnBoUpdt);

   RETVALUE((*RgMacSchCmnBoUpdtReqMt[0])(pst, boRpt));
}

/*Fix: start: Inform UE delete to scheduler*/
/**
 * @brief This primitive is used to indicate deletion of UE
 *        at MAC to scheduler. 
 *
 * @details
 *
 *     Function : RgMacSchUeDel
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   RgInfSfDatInd*  ueDelInd 
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgMacSchUeDel
(
 Pst* pst,
 RgInfUeDelInd*  ueDelInd
 )
#else
PUBLIC S16 RgMacSchUeDel(pst, datInd)
   Pst* pst;
   RgInfUeDelInd*  ueDelInd;
#endif
{

   TRC3(RgMacSchUeDel);

   RETVALUE((*RgMacSchUeDelIndMt[0])(pst, ueDelInd));
}
/*Fix: end: Inform UE delete to scheduler*/
/**
 * @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
 *
 * @details
 *
 *     Function : RgMacSchSfRecp
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDDatIndInfo  *  datInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgMacSchSfRecp
(
 Pst* pst,
 RgInfSfDatInd*  datInd
 )
#else
PUBLIC S16 RgMacSchSfRecp(pst, datInd)
   Pst* pst;
   RgInfSfDatInd*  datInd;
#endif
{

   TRC3(RgMacSchSfRecp);

   RETVALUE((*RgMacSchSfRecpIndMt[0])(pst, datInd));
}

/**
 * @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
 *
 * @details
 *
 *     Function : RgSchMacSfAlloc
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDDatIndInfo  *  datInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacSfAlloc
(
 Pst* pst,
 RgInfSfAlloc*   resAllocReq
 )
#else
PUBLIC S16 RgSchMacSfAlloc(pst, resAllocReq)
   Pst* pst;
   RgInfSfAlloc*   resAllocReq;
#endif
{

   TRC3(RgSchMacSfAlloc);

   RETVALUE((*RgSchMacSfAllocReqMt[0])(pst, resAllocReq));
}

/**
 * @brief Trigger for resetting Harq Entity at MAC
 *
 * @details
 *
 *     Function : RgSchMacRstHqEnt
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   RgInfResetHqEnt *hqEntRstReq
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacRstHqEnt
(
 Pst*                 pst,    
 RgInfResetHqEnt*     hqEntInfo
 )
#else
PUBLIC S16 RgSchMacRstHqEnt(pst, hqEntInfo)
   Pst*                 pst;
   RgInfResetHqEnt*     hqEntInfo;
#endif
{

   TRC3(RgSchMacRstHqEnt)

      RETVALUE((*RgSchMacRstHqEntReqMt[0])(pst, hqEntInfo));
}

/**
 * @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
 *
 * @details
 *
 *     Function : RgSchMacRlsHq
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDDatIndInfo  *  datInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacRlsHq
(
 Pst* pst,
 RgInfRlsHqInfo* sfHqInfo
 )
#else
PUBLIC S16 RgSchMacRlsHq(pst, sfHqInfo)
   Pst* pst;
   RgInfRlsHqInfo* sfHqInfo;
#endif
{

   TRC3(RgSchMacRlsHq);

   RETVALUE((*RgSchMacRlsHqReqMt[0])(pst, sfHqInfo));
}

/**
 * @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
 *
 * @details
 *
 *     Function : RgSchMacRlsRnti
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDDatIndInfo  *  datInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacRlsRnti
(
 Pst* pst,
 RgInfRlsRnti*   rlsRnti
 )
#else
PUBLIC S16 RgSchMacRlsRnti(pst, rlsRnti)
   Pst* pst;
   RgInfRlsRnti*   rlsRnti;
#endif
{

   TRC3(RgSchMacRlsRnti);

   RETVALUE((*RgSchMacRlsRntiReqMt[0])(pst, rlsRnti));
}

/**
 * @brief Request from SCH to MAC to register the GBR LCG of a UE
 *
 * @details
 *
 *     Function : RgSchMacLcgReg
 *
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfLcgRegReq *lcgRegReq
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacLcgReg
(
 Pst*                pst,
 RgInfLcgRegReq      *lcgRegReq
 )
#else
PUBLIC S16 RgSchMacLcgReg(pst, lcgRegReq)
   Pst*                pst;
   RgInfLcgRegReq       *lcgRegReq;
#endif
{

   TRC3(RgSchMacLcgReg);

   RETVALUE((*RgSchMacLcgRegReqMt[0])(pst, lcgRegReq));
} /* end of RgSchMacLcgReg */

#ifdef LTEMAC_SPS
/**
 * @brief Data Indication from MAC to SCH to 
 * indicate the arrival of the data on SPS logical channels
 *
 * @details
 *
 *     Function : RgMacSchSpsRel
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   RgInfSpsRelInfo* relInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgMacSchSpsRel
(
 Pst*                pst,
 RgInfSpsRelInfo*    relInfo
 )
#else
PUBLIC S16 RgMacSchSpsRel(pst, relInfo)
   Pst*                pst;
   RgInfSpsRelInfo*    relInfo;
#endif
{

   TRC3(RgMacSchS);

   RETVALUE((*RgMacSchSpsRelIndMt[0])(pst, relInfo));
} /* end of RgMacSchSpsRel */

/**
 * @brief Request from SCH to MAC to register the SPS logical channels of a UE
 *
 * @details
 *
 *     Function : RgSchMacSpsLcReg
 *
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfSpsLcInfo *lcInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacSpsLcReg
(
 Pst*                pst,
 RgInfSpsLcInfo      *lcInfo
 )
#else
PUBLIC S16 RgSchMacSpsLcReg(pst, lcInfo)
   Pst*                pst;
   RgInfSpsLcInfo      *lcInfo;
#endif
{

   TRC3(RgSchMacSpsLcReg);

   RETVALUE((*RgSchMacSpsLcRegReqMt[0])(pst, lcInfo));
} /* end of RgSchMacSpsLcReg */



/**
 * @brief Request from SCH to MAC to reset UL SPS params
 *
 * @details
 *
 *     Function : RgSchMacUlSpsReset
 *
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfUlSpsReset *lcInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacUlSpsReset
(
 Pst*                pst,
 RgInfUlSpsReset      *ulSpsResetInfo
 )
#else
PUBLIC S16 RgSchMacUlSpsReset(pst, ulSpsResetInfo)
   Pst*                pst;
   RgInfUlSpsReset      *ulSpsResetInfo;
#endif
{

   TRC3(RgSchMacUlSpsReset);

   RETVALUE((*RgSchMacUlSpsResetMt[0])(pst, ulSpsResetInfo));
} /* end of RgSchMacUlSpsReset */



/**
 * @brief Request from SCH to MAC to deregister the SPS logical channels of a UE
 *
 * @details
 *
 *     Function : RgSchMacSpsLcDereg
 *
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfSpsLcInfo *lcInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacSpsLcDereg
(
 Pst*                pst,
 CmLteCellId         cellId,
 CmLteRnti           crnti
 )
#else
PUBLIC S16 RgSchMacSpsLcDereg(pst, cellId, crnti)
   Pst*                pst;
   CmLteCellId         cellId;
   CmLteRnti           crnti;
#endif
{

   TRC3(RgSchMacSpsLcDereg);

   RETVALUE((*RgSchMacSpsLcDeregReqMt[0])(pst, cellId, crnti));
} /* end of RgSchMacSpsLcDereg */

#endif /* LTEMAC_SPS */
#ifdef LTE_L2_MEAS
/**
 * @brief Request from SCH to MAC for L2 Measurement
 *
 * @details
 *
 *     Function : RgSchMacL2Meas
 *
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfL2MeasReq *measInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacL2Meas
(
 Pst*                pst,
 RgInfL2MeasReq      *measInfo
 )
#else
PUBLIC S16 RgSchMacL2Meas(pst, measInfo)
   Pst*                pst;
   RgInfL2MeasReq      *measInfo;
#endif
{

   TRC3(RgSchMacL2Meas);

   RETVALUE((*RgSchMacL2MeasReqMt[0])(pst, measInfo));
} /* end of RgSchMacL2Meas */

/**
 * @brief Request from SCH to MAC for Stopping L2 Measurement
 *
 * @details
 *
 *     Function : RgSchMacL2MeasStop
 *     
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfL2MeasReq *measInfo
 *  @return   S16
 *      -# ROK
 **/   
#ifdef ANSI
PUBLIC S16 RgSchMacL2MeasStop
(       
 Pst*                pst,
 RgInfL2MeasStopReq *measInfo
 )     
#else 
PUBLIC S16 RgSchMacL2MeasStop(pst, measInfo)
   Pst*                pst;
   RgInfL2MeasStopReq *measInfo;
#endif
{  

   TRC3(RgSchMacL2MeasStop);

   RETVALUE((*RgSchMacL2MeasStopReqMt[0])(pst, measInfo));
} /* end of RgSchMacL2Meas */

/**
 * @brief Request from SCH to MAC for Sending L2 Measurement
 *
 * @details
 *
 *     Function : RgSchMacL2MeasSend
 *     
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfL2MeasReq *measInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgSchMacL2MeasSend
(
 Pst*                pst,
 RgInfL2MeasSndReq *measInfo
 )
#else
PUBLIC S16 RgSchMacL2MeasSend(pst, measInfo)
   Pst*                pst;
   RgInfL2MeasSndReq *measInfo;
#endif
{

   TRC3(RgSchMacL2MeasSend);

   RETVALUE((*RgSchMacL2MeasSendReqMt[0])(pst, measInfo));
} /* end of RgSchMacL2MeasSend */

/**
 * @brief Request from MAC to SCH for L2 Measurement
 *
 * @details
 *
 *     Function : RgMacSchL2Meas
 *
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfL2MeasCfm *measInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgMacSchL2Meas
(
 Pst*                pst,
 RgInfL2MeasCfm      *measInfo
 )
#else
PUBLIC S16 RgMacSchL2Meas(pst, measInfo)
   Pst*                pst;
   RgInfL2MeasCfm      *measInfo;
#endif
{

   TRC3(RgMacSchL2Meas);

   RETVALUE((*RgMacSchL2MeasCfmMt[0])(pst, measInfo));
} /* end of RgSchMacL2Meas */
/**
 * @brief Request from MAC to SCH for L2 Measurement
 *  
 * @details
 *
 *     Function : RgMacSchL2MeasStop
 *
 *  @param[in]   Pst*           pst
 *  @param[in]   RgInfL2MeasCfm *measInfo
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
   PUBLIC S16 RgMacSchL2MeasStop
(
 Pst*                pst,   
 RgInfL2MeasCfm      *measInfo
 )
#else
PUBLIC S16 RgMacSchL2MeasStop(pst, measInfo)
   Pst*                pst;
   RgInfL2MeasCfm      *measInfo;
#endif
{

   TRC3(RgMacSchL2MeasStop);

   RETVALUE((*RgMacSchL2MeasStopCfmMt[0])(pst, measInfo));
} /* end of RgSchMacL2MeasStop*/
#endif/*LTE_L2_MEAS*/
/**********************************************************************

  End of file
 **********************************************************************/
