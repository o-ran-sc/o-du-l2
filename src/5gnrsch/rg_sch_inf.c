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
  
     Desc:     C source code for packing/unpacking of INF interface
               primitives.
  
     File:     rg_sch_inf.c 
  
**********************************************************************/

/* header include files (.h) */
#include "common_def.h"
#include "tfu.h"           /* RGU defines */
#ifdef LTE_L2_MEAS
#include "lrg.h"
#endif
#include "rg_sch_inf.h"    /* RGU Interface defines */

/* header/extern include files (.x) */
#include "tfu.x"           /* RGU defines */
#ifdef LTE_L2_MEAS
#include "lrg.x"
#endif
#include "rg_sch_inf.x"    /* RGU Interface includes */

#ifdef LCSCH
/*Fix: start: Inform UE delete to scheduler*/
/**
* @brief This primitive is used to indicate to scheduler
*        that UE has been deleted at MAC.
*
* @details
*
*     Function : cmPkMacSchUeDelInd
*
*  @param[in]   Pst*  pst
*  @param[in]   RgInfUeDelInd*  ueDelInd
*  @return   S16
*      -# ROK
**/
S16 cmPkMacSchUeDelInd(Pst* pst,RgInfUeDelInd*  ueDelInd)
{
   Buffer *mBuf = NULLP;
   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   
#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)ueDelInd, sizeof(RgInfUeDelInd), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFUEDELIND;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief This primitive is used to indicate to scheduler
*        that UE has been deleted at MAC.
*
*
* @details
*
*     Function : cmUnpkMacSchUeDelInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkMacSchUeDelInd(UeDelInd func,Pst *pst,Buffer *mBuf)
{
   RgInfUeDelInd ueDelInd;
   
   if(SRemPreMsgMult((Data *)&ueDelInd, sizeof(RgInfUeDelInd), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);

   return ((*func)(pst, &ueDelInd));
}
/*Fix: end: Inform UE delete to scheduler*/

/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmPkMacSchDedBoUpdtReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmPkMacSchDedBoUpdtReq(Pst* pst,RgInfDedBoRpt* boRpt)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)boRpt, sizeof(RgInfDedBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFDEDBOUPDTREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmUnpkMacSchDedBoUpdtReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkMacSchDedBoUpdtReq(DedBoUpdtReq func,Pst *pst,Buffer *mBuf)
{
   RgInfDedBoRpt boRpt;
   
   if(SRemPreMsgMult((Data *)&boRpt, sizeof(RgInfDedBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, &boRpt));
}
/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmPkMacSchCmnBoUpdtReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmPkMacSchCmnBoUpdtReq(Pst* pst,RgInfCmnBoRpt* boRpt)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)boRpt, sizeof(RgInfCmnBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFCMNBOUPDTREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmUnpkMacSchCmnBoUpdtReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkMacSchCmnBoUpdtReq(CmnBoUpdtReq func,Pst *pst,Buffer *mBuf)
{
   RgInfCmnBoRpt boRpt;
   
   if(SRemPreMsgMult((Data *)&boRpt, sizeof(RgInfCmnBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, &boRpt));
}
/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmPkMacSchSfRecpInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmPkMacSchSfRecpInd(Pst* pst,RgInfSfDatInd*  datInd)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(oduPackPointer((PTR)datInd, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFSFRECPIND;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmUnpkMacSchSfRecpInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkMacSchSfRecpInd(SfRecpInd func,Pst *pst,Buffer *mBuf)
{
   RgInfSfDatInd* datInd;
   
   if(oduUnpackPointer((PTR *)&datInd, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   /* Call cmFreeMem(datInd) in scheduler */
   return ((*func)(pst, datInd));
}

#ifdef LTEMAC_SPS
/**
* @brief Primitive from MAC to SCH to indicate release of UL SPS for a UE
*
* @details
*
*     Function : cmPkMacSchSpsRelInd
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfSpsRelInfo*     relInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkMacSchSpsRelInd(Pst* pst,RgInfSpsRelInfo* relInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(oduPackPointer((PTR)relInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFSPSRELIND;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkMacSchSpsRelInd */

/**
* @brief Primitive from MAC to SCH to indicate release of UL SPS for a UE
*
* @details
*
*     Function : cmUnpkMacSchSpsRelInd
*
*  @param[in]    SpsRelInd            func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkMacSchSpsRelInd(SpsRelInd func,Pst *pst,Buffer *mBuf)
{
   RgInfSpsRelInfo *relInfo;
   
   if(oduUnpackPointer((PTR *)&relInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, relInfo));
} /* end of cmUnpkMacSchSpsRelInd */
#endif /* LTEMAC_SPS */

#endif
#ifdef LCRG
/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmPkSchMacSfAllocReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacSfAllocReq(Pst* pst,RgInfSfAlloc* resAllocReq)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(oduPackPointer((PTR)resAllocReq, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFSFALLOCREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmUnpkSchMacSfAllocReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacSfAllocReq(SfAllocReq func,Pst *pst,Buffer *mBuf)
{
   RgInfSfAlloc* resAllocReq;
   
   if(oduUnpackPointer((PTR *)&resAllocReq, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, resAllocReq));
}
/**
* @brief Request from  SCH To MAC for harq entity reset
*
* @details
*
*     Function : cmPkSchMacRstHqEntReq
*
*  @param[in]   Pst*  pst
*  @param[in]   RgInfResetHqEnt *hqEntInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacRstHqEntReq(Pst*  pst,RgInfResetHqEnt* hqEntInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(oduPackPointer((PTR)hqEntInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFHQENTRESET;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief Request from SCH to MAC for resetting the harqentity
*
* @details
*
*     Function : cmUnpkSchMacRstHqEntReq
*
*  @param[in]   Pst*  pst
*  @param[in]   RgInfResetHqEnt *hqEntInfo
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacRstHqEntReq(RstHqEntReq func,Pst *pst,Buffer *mBuf)
{
   RgInfResetHqEnt* hqEntRstInfo;
   
   if(oduUnpackPointer((PTR *)&hqEntRstInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, hqEntRstInfo));
}

/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmPkSchMacRlsHqReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacRlsHqReq(Pst* pst,RgInfRlsHqInfo* sfHqInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(oduPackPointer((PTR)sfHqInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFRLSHQREQ;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmUnpkSchMacRlsHqReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacRlsHqReq(RlsHqReq func,Pst *pst,Buffer *mBuf)
{
   RgInfRlsHqInfo* sfHqInfo;
   
   if(oduUnpackPointer((PTR *)&sfHqInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, sfHqInfo));
}
/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmPkSchMacRlsRntiReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacRlsRntiReq(Pst* pst,RgInfRlsRnti* rlsRnti)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)rlsRnti, sizeof(RgInfRlsRnti), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFRLSRNTIREQ;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmUnpkSchMacRlsRntiReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacRlsRntiReq(RlsRntiReq func,Pst *pst,Buffer *mBuf)
{
   RgInfRlsRnti rlsRnti;
   
   if(SRemPreMsgMult((Data *)&rlsRnti, sizeof(RgInfRlsRnti), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, &rlsRnti));
}

/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmPkSchMacCellRegReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacCellRegReq(Pst* pst,RgInfCellReg* regReq)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)regReq, sizeof(RgInfCellReg), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFCELLREGREQ;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief Request from RLC to MAC for forwarding SDUs on 
 * dedicated channel for transmission.
*
* @details
*
*     Function : cmUnpkSchMacCellRegReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RguDDatReqInfo  *  datReq
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacCellRegReq(CellRegReq func,Pst *pst,Buffer *mBuf)
{
   RgInfCellReg regReq;
   
   if(SRemPreMsgMult((Data *)&regReq, sizeof(RgInfCellReg), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, &regReq));
}

/**
* @brief Primitive from SCH to MAC to register GBR LCG per UE
*
* @details
*
*     Function : cmPkSchMacLcgRegReq
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfLcgRegReq       *lcgRegReq
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacLcgRegReq(Pst* pst,RgInfLcgRegReq *lcgRegReq)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(SAddPstMsgMult((Data *)lcgRegReq, sizeof(RgInfLcgRegReq), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFLCGREG;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkSchMacLcgRegReq */

/**
* @brief Primitive from SCH to MAC to register GBR LCG
*
* @details
*
*     Function : cmUnpkSchMacLcgRegReq
*
*  @param[in]    LcgReg               func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacLcgRegReq(LcgReg func,Pst *pst,Buffer *mBuf)
{
   RgInfLcgRegReq       *lcgRegReq;
   
   if(oduUnpackPointer((PTR *)&lcgRegReq, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, lcgRegReq));
} /* end of cmUnpkSchMacLcgRegReq */

#ifdef LTEMAC_SPS

/**
* @brief Primitive from SCH to MAC to register the set of SPS LCs per UE
*
* @details
*
*     Function : cmPkSchMacSpsLcRegReq
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfSpsLcInfo       *lcInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacSpsLcRegReq(Pst*  pst,RgInfSpsLcInfo *lcInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(oduPackPointer((PTR)lcInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFSPSLCREG;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkSchMacSpsLcRegReq */

/**
* @brief Primitive from SCH to MAC to reset SPS Params for the UE
*
* @details
*
*     Function : cmPkSchMacUlSpsResetReq
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfUlSpsReset       *ulSpsResetInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacUlSpsResetReq(Pst*  pst,RgInfUlSpsReset  *ulSpsResetInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(oduPackPointer((PTR)ulSpsResetInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFSPSRESET;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkSchMacUlSpsResetReq */

/**
* @brief Primitive from SCH to MAC to register the set of SPS LCs per UE
*
* @details
*
*     Function : cmUnpkSchMacSpsLcRegReq
*
*  @param[in]    SpsLcReg             func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacSpsLcRegReq(SpsLcReg func,Pst *pst,Buffer *mBuf)
{
   RgInfSpsLcInfo       *lcInfo;
   
   if(oduUnpackPointer((PTR *)&lcInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, lcInfo));
} /* end of cmUnpkSchMacSpsLcRegReq */



/**
* @brief Primitive from SCH to MAC to reset UL SPS params
*
* @details
*
*     Function : cmUnpkSchMacUlSpsResetReq
*
*  @param[in]    UlSpsReset             func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacUlSpsResetReq(UlSpsReset func,Pst *pst,Buffer  *mBuf)
{
   RgInfUlSpsReset       *ulSpsResetInfo;
   
   if(oduUnpackPointer((PTR *)&ulSpsResetInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, ulSpsResetInfo));
} /* end of cmUnpkSchMacUlSpsResetReq */



/**
* @brief Primitive from SCH to MAC to deregister the set of SPS LCs per UE
*
* @details
*
*     Function : cmPkSchMacSpsLcDeregReq
*
*  @param[in]   Pst*                 pst
*  @param[in]   CmLteCellId          cellId,
*  @param[in]   CmLteRnti            crnti
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacSpsLcDeregReq(Pst* pst,CmLteCellId cellId,CmLteRnti  crnti)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   CMCHKPK(cmPkLteCellId, cellId, mBuf);
   CMCHKPK(cmPkLteRnti, crnti, mBuf);

   pst->event = (Event) EVTINFSPSLCDEREG;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkSchMacSpsLcDeregReq */

/**
* @brief Primitive from SCH to MAC to deregister the set of SPS LCs per UE
*
* @details
*
*     Function : cmUnpkSchMacSpsLcDeregReq
*
*  @param[in]    SpsLcDereg           func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacSpsLcDeregReq(SpsLcDereg func,Pst  *pst, Buffer *mBuf)
{
   CmLteCellId  cellId;
   CmLteRnti    crnti;
   
   CMCHKUNPK(cmUnpkLteRnti, &crnti, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &cellId, mBuf);

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, cellId, crnti));
} /* end of cmUnpkSchMacSpsLcDeregReq */

#endif /* LTEMAC_SPS */
#ifdef LTE_L2_MEAS

/**
* @brief Primitive from SCH to MAC for L2 Measurement
*
* @details
*
*     Function : cmPkSchMacL2MeasReq
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfSpsLcInfo       *lcInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacL2MeasReq(Pst* pst,RgInfL2MeasReq  *measInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }
   if(SAddPstMsgMult((Data *)measInfo, sizeof(RgInfL2MeasReq), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFL2MEASREQ;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkSchMacL2MeasReq */

/**
* @brief Primitive from SCH to MAC for L2 Stop Measurement
*
* @details
*
*     Function : cmPkSchMacL2MeasStopReq
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfSpsLcInfo       *lcInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacL2MeasStopReq(Pst*  pst,RgInfL2MeasStopReq *measInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
       return RFAILED;
  }
   if(SAddPstMsgMult((Data *)measInfo, sizeof(RgInfL2MeasStopReq), mBuf) != ROK)
   {
      ODU_PUT_MSG_BUF(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFL2MEASSTOPREQ;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkSchMacL2MeasStopReq */
/**
* @brief Primitive from SCH to MAC for L2 Measurement
*         Send Request
* @details
*
*     Function : cmPkSchMacL2MeasSendReq
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfSpsLcInfo       *lcInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkSchMacL2MeasSendReq(Pst* pst,RgInfL2MeasSndReq *measInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
       return RFAILED;
  }

   if(SAddPstMsgMult((Data *)measInfo, sizeof(RgInfL2MeasSndReq), mBuf) != ROK)
   {
      ODU_PUT_MSG_BUF(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFL2MEASSENDREQ;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkSchMacL2MeasSendReq */

/**
* @brief Primitive from SCH to MAC for L2 Measurement request
*
* @details
*
*     Function : cmUnpkSchMacL2MeasReq
*
*  @param[in]    L2MeasReg            func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacL2MeasReq(L2MeasReq func,Pst *pst,Buffer *mBuf)
{
   RgInfL2MeasReq      measInfo;
   
   if(SRemPreMsgMult((Data *)&measInfo, sizeof(RgInfL2MeasReq), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, &measInfo));
} /* end of cmUnpkSchMacL2MeasReq */

/**
* @brief Primitive from SCH to MAC for L2 Measurement Stop request
*
* @details
*
*     Function : cmUnpkSchMacL2MeasStopReq
*
*  @param[in]    L2MeasReg            func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacL2MeasStopReq(L2MeasStopReq func,Pst *pst,Buffer *mBuf)
{
   RgInfL2MeasStopReq measInfo;

   if(SRemPreMsgMult((Data *)&measInfo, sizeof(RgInfL2MeasStopReq), mBuf) != ROK)
   {
      ODU_PUT_MSG_BUF(mBuf);
      return RFAILED;
   }

   ODU_PUT_MSG_BUF(mBuf);
   return ((*func)(pst, &measInfo));
} /* end of cmUnpkSchMacL2MeasReq */
/**
* @brief Primitive from SCH to MAC for L2 Measurement request
*
* @details
*
*     Function : cmUnpkSchMacL2MeasReq
*
*  @param[in]    L2MeasReg            func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkSchMacL2MeasSendReq(L2MeasSendReq  func,Pst *pst,Buffer *mBuf)
{
   RgInfL2MeasSndReq measInfo;

 if(SRemPreMsgMult((Data *)&measInfo, sizeof(RgInfL2MeasSndReq), mBuf) != ROK)  
   {
      ODU_PUT_MSG_BUF(mBuf);
      return RFAILED;
   }

   ODU_PUT_MSG_BUF(mBuf);
   return ((*func)(pst, &measInfo));
} /* end of cmUnpkSchMacL2MeasSendReq*/

/**
* @brief Primitive from MAC to SCH for L2 Measurement
*
* @details
*
*     Function : cmPkMacSchL2MeasCfm
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfL2MeasCfm       *measInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkMacSchL2MeasCfm(Pst*  pst,RgInfL2MeasCfm *measCfm)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(SAddPstMsgMult((Data *)measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFL2MEASCFM;
   return (SPstTsk(pst,mBuf));
} /* end of cmPkMacSchL2MeasCfm */


/**
* @brief Primitive from MAC to SCH for L2 Measurement
*        stop cfm
* @details
*
*     Function : cmPkMacSchL2MeasStopCfm
*
*  @param[in]   Pst*                 pst
*  @param[in]   RgInfL2MeasCfm       *measInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkMacSchL2MeasStopCfm(Pst*  pst,RgInfL2MeasCfm  *measCfm)
{
   Buffer *mBuf = NULLP;

 if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) {
      return RFAILED;
   }

   if(SAddPstMsgMult((Data *)measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      ODU_PUT_MSG_BUF(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTINFL2MEASSTOPCFM;
   return (SPstTsk(pst,mBuf));
}/*cmPkMacSchL2MeasStopCfm*/
/**
* @brief Primitive from MAC to SCH for L2 Measurement Cfm
*
* @details
*
*     Function : cmUnpkSchMacL2MeasReq
*
*  @param[in]    L2MeasCfm            func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkMacSchL2MeasCfm(L2MeasCfm func,Pst *pst,Buffer *mBuf)
{
   RgInfL2MeasCfm      measCfm;
   
   if(SRemPreMsgMult((Data *)&measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGSCHINF_FREE_MSG(mBuf);
   return ((*func)(pst, &measCfm));
} /* end of cmUnpkMacSchL2MeasCfm */

/**
* @brief Primitive from MAC to SCH for L2 Measurement Stop  Cfm
*
* @details
*
*     Function : cmUnpkMacSchL2MeasStopCfm
*
*  @param[in]    L2MeasCfm            func
*  @param[in]    Pst*                 pst
*  @param[in]    Buffer               *mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkMacSchL2MeasStopCfm(L2MeasCfm func,Pst *pst,Buffer  *mBuf)
{
   RgInfL2MeasCfm      measCfm;

   if(SRemPreMsgMult((Data *)&measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      ODU_PUT_MSG_BUF(mBuf);
      return RFAILED;
   }

   ODU_PUT_MSG_BUF(mBuf);
   return ((*func)(pst, &measCfm));
} /* end of cmUnpkMacSchL2MeasStopCfm */

#endif/* LTE_L2_MEAS */

#endif


/**********************************************************************
 
         End of file
**********************************************************************/
