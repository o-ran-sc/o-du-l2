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
#ifdef ANSI
S16 cmPkMacSchUeDelInd
(
Pst* pst,
RgInfUeDelInd*  ueDelInd
)
#else
S16 cmPkMacSchUeDelInd(pst, ueDelInd)
Pst* pst;
RgInfUeDelInd*  ueDelInd;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkMacSchUeDelInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   
#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)ueDelInd, sizeof(RgInfUeDelInd), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFUEDELIND;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkMacSchUeDelInd
(
UeDelInd func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkMacSchUeDelInd(func, pst, mBuf)
UeDelInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfUeDelInd ueDelInd;
   
   TRC2(cmUnpkMacSchUeDelInd)

   if(SRemPreMsgMult((Data *)&ueDelInd, sizeof(RgInfUeDelInd), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);

   RETVALUE((*func)(pst, &ueDelInd));
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
#ifdef ANSI
S16 cmPkMacSchDedBoUpdtReq
(
Pst* pst,
RgInfDedBoRpt* boRpt
)
#else
S16 cmPkMacSchDedBoUpdtReq(pst, boRpt)
Pst* pst;
RgInfDedBoRpt* boRpt;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkMacSchDedBoUpdtReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)boRpt, sizeof(RgInfDedBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFDEDBOUPDTREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkMacSchDedBoUpdtReq
(
DedBoUpdtReq func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkMacSchDedBoUpdtReq(func, pst, mBuf)
DedBoUpdtReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfDedBoRpt boRpt;
   
   TRC2(cmUnpkMacSchDedBoUpdtReq)

   if(SRemPreMsgMult((Data *)&boRpt, sizeof(RgInfDedBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &boRpt));
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
#ifdef ANSI
S16 cmPkMacSchCmnBoUpdtReq
(
Pst* pst,
RgInfCmnBoRpt* boRpt
)
#else
S16 cmPkMacSchCmnBoUpdtReq(pst, boRpt)
Pst* pst;
RgInfCmnBoRpt* boRpt;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkMacSchCmnBoUpdtReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)boRpt, sizeof(RgInfCmnBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFCMNBOUPDTREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkMacSchCmnBoUpdtReq
(
CmnBoUpdtReq func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkMacSchCmnBoUpdtReq(func, pst, mBuf)
CmnBoUpdtReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfCmnBoRpt boRpt;
   
   TRC2(cmUnpkMacSchCmnBoUpdtReq)

   if(SRemPreMsgMult((Data *)&boRpt, sizeof(RgInfCmnBoRpt), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &boRpt));
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
#ifdef ANSI
S16 cmPkMacSchSfRecpInd
(
Pst* pst,
RgInfSfDatInd*  datInd
)
#else
S16 cmPkMacSchSfRecpInd(pst, datInd)
Pst* pst;
RgInfSfDatInd*  datInd;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkMacSchSfRecpInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(cmPkPtr((PTR)datInd, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFSFRECPIND;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkMacSchSfRecpInd
(
SfRecpInd func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkMacSchSfRecpInd(func, pst, mBuf)
SfRecpInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfSfDatInd* datInd;
   
   TRC2(cmUnpkMacSchCmnBoUpdtReq)

   if(cmUnpkPtr((PTR *)&datInd, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   /* Call cmFreeMem(datInd) in scheduler */
   RETVALUE((*func)(pst, datInd));
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
#ifdef ANSI
S16 cmPkMacSchSpsRelInd
(
Pst*                 pst,
RgInfSpsRelInfo*     relInfo
)
#else
S16 cmPkMacSchSpsRelInd(pst, relInfo)
Pst*                 pst;
RgInfSpsRelInfo*     relInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkMacSchSpsRelInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(cmPkPtr((PTR)relInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFSPSRELIND;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkMacSchSpsRelInd
(
SpsRelInd      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkMacSchSpsRelInd(func, pst, mBuf)
SpsRelInd      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfSpsRelInfo *relInfo;
   
   TRC2(cmUnpkMacSchSpsRelInd)

   if(cmUnpkPtr((PTR *)&relInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, relInfo));
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
#ifdef ANSI
S16 cmPkSchMacSfAllocReq
(
Pst* pst,
RgInfSfAlloc*   resAllocReq
)
#else
S16 cmPkSchMacSfAllocReq(pst, resAllocReq)
Pst* pst;
RgInfSfAlloc*   resAllocReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkSchMacSfAllocReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(cmPkPtr((PTR)resAllocReq, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFSFALLOCREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacSfAllocReq
(
SfAllocReq func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkSchMacSfAllocReq(func, pst, mBuf)
SfAllocReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfSfAlloc* resAllocReq;
   
   TRC2(cmUnpkSchMacSfAllocReq)

   if(cmUnpkPtr((PTR *)&resAllocReq, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, resAllocReq));
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
#ifdef ANSI
S16 cmPkSchMacRstHqEntReq
(
Pst*                 pst,
RgInfResetHqEnt*     hqEntInfo
)
#else
S16 cmPkSchMacRstHqEntReq(pst, sfHqInfo)
Pst*                 pst,    
RgInfResetHqEnt*     hqEntInfo
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkSchMacRstHqEntReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(cmPkPtr((PTR)hqEntInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFHQENTRESET;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacRstHqEntReq
(
RstHqEntReq func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkSchMacRstHqEntReq(func, pst, mBuf)
RstHqEntReq func,
Pst *pst,
Buffer *mBuf
#endif
{
   RgInfResetHqEnt* hqEntRstInfo;
   
   TRC2(cmUnpkSchMacRstHqEntReq)

   if(cmUnpkPtr((PTR *)&hqEntRstInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, hqEntRstInfo));
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
#ifdef ANSI
S16 cmPkSchMacRlsHqReq
(
Pst* pst,
RgInfRlsHqInfo* sfHqInfo
)
#else
S16 cmPkSchMacRlsHqReq(pst, sfHqInfo)
Pst* pst;
RgInfRlsHqInfo* sfHqInfo;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkSchMacRlsHqReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(cmPkPtr((PTR)sfHqInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFRLSHQREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacRlsHqReq
(
RlsHqReq func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkSchMacRlsHqReq(func, pst, mBuf)
RlsHqReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfRlsHqInfo* sfHqInfo;
   
   TRC2(cmUnpkSchMacRlsHqReq)

   if(cmUnpkPtr((PTR *)&sfHqInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, sfHqInfo));
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
#ifdef ANSI
S16 cmPkSchMacRlsRntiReq
(
Pst* pst,
RgInfRlsRnti* rlsRnti
)
#else
S16 cmPkSchMacRlsRntiReq(pst, rlsRnti)
Pst* pst;
RgInfRlsRnti* rlsRnti;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkSchMacRlsRntiReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)rlsRnti, sizeof(RgInfRlsRnti), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFRLSRNTIREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacRlsRntiReq
(
RlsRntiReq func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkSchMacRlsRntiReq(func, pst, mBuf)
RlsRntiReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfRlsRnti rlsRnti;
   
   TRC2(cmUnpkSchMacRlsRntiReq)

   if(SRemPreMsgMult((Data *)&rlsRnti, sizeof(RgInfRlsRnti), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &rlsRnti));
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
#ifdef ANSI
S16 cmPkSchMacCellRegReq
(
Pst* pst,
RgInfCellReg* regReq
)
#else
S16 cmPkSchMacCellRegReq(pst, regReq)
Pst* pst;
RgInfCellReg* regReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC2(cmPkSchMacCellRegReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
   if(SAddPstMsgMult((Data *)regReq, sizeof(RgInfCellReg), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFCELLREGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacCellRegReq
(
CellRegReq func,
Pst *pst,
Buffer *mBuf
)
#else
S16 cmUnpkSchMacCellRegReq(func, pst, mBuf)
CellRegReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   RgInfCellReg regReq;
   
   TRC2(cmUnpkSchMacCellRegReq)

   if(SRemPreMsgMult((Data *)&regReq, sizeof(RgInfCellReg), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &regReq));
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
#ifdef ANSI
S16 cmPkSchMacLcgRegReq
(
Pst*                 pst,
RgInfLcgRegReq       *lcgRegReq
)
#else
S16 cmPkSchMacLcgRegReq(pst, lcgRegReq)
Pst*                 pst;
RgInfLcgRegReq       *lcgRegReq;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkSchMacLcgRegReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(SAddPstMsgMult((Data *)lcgRegReq, sizeof(RgInfLcgRegReq), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFLCGREG;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacLcgRegReq
(
LcgReg         func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkSchMacLcgRegReq(func, pst, mBuf)
LcgReg         func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfLcgRegReq       *lcgRegReq;
   
   TRC2(cmUnpkSchMacLcgRegReq)

   if(cmUnpkPtr((PTR *)&lcgRegReq, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, lcgRegReq));
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
#ifdef ANSI
S16 cmPkSchMacSpsLcRegReq
(
Pst*                 pst,
RgInfSpsLcInfo       *lcInfo
)
#else
S16 cmPkSchMacSpsLcRegReq(pst, lcInfo)
Pst*                 pst;
RgInfSpsLcInfo       *lcInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkSchMacSpsLcRegReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(cmPkPtr((PTR)lcInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFSPSLCREG;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmPkSchMacUlSpsResetReq
(
Pst*                 pst,
RgInfUlSpsReset      *ulSpsResetInfo
)
#else
S16 cmPkSchMacUlSpsResetReq(pst, ulSpsResetInfo)
Pst*                 pst;
RgInfUlSpsReset       *ulSpsResetInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkSchMacUlSpsResetReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(cmPkPtr((PTR)ulSpsResetInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFSPSRESET;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacSpsLcRegReq
(
SpsLcReg       func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkSchMacSpsLcRegReq(func, pst, mBuf)
SpsLcReg       func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfSpsLcInfo       *lcInfo;
   
   TRC2(cmUnpkSchMacSpsLcRegReq)

   if(cmUnpkPtr((PTR *)&lcInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, lcInfo));
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
#ifdef ANSI
S16 cmUnpkSchMacUlSpsResetReq
(
UlSpsReset       func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkSchMacUlSpsResetReq(func, pst, mBuf)
UlSpsReset       func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfUlSpsReset       *ulSpsResetInfo;
   
   TRC2(cmUnpkSchMacUlSpsResetReq)

   if(cmUnpkPtr((PTR *)&ulSpsResetInfo, mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, ulSpsResetInfo));
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
#ifdef ANSI
S16 cmPkSchMacSpsLcDeregReq
(
Pst*                 pst,
CmLteCellId          cellId,
CmLteRnti            crnti
)
#else
S16 cmPkSchMacSpsLcDeregReq(pst, cellId, crnti)
Pst*                 pst;
CmLteCellId          cellId;
CmLteRnti            crnti;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkSchMacSpsLcDeregReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   CMCHKPK(cmPkLteCellId, cellId, mBuf);
   CMCHKPK(cmPkLteRnti, crnti, mBuf);

   pst->event = (Event) EVTINFSPSLCDEREG;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacSpsLcDeregReq
(
SpsLcDereg     func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkSchMacSpsLcDeregReq(func, pst, mBuf)
SpsLcDereg     func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   CmLteCellId  cellId;
   CmLteRnti    crnti;
   
   TRC2(cmUnpkSchMacSpsLcDeregReq)

   CMCHKUNPK(cmUnpkLteRnti, &crnti, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &cellId, mBuf);

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, cellId, crnti));
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
#ifdef ANSI
S16 cmPkSchMacL2MeasReq
(
Pst*                 pst,
RgInfL2MeasReq      *measInfo
)
#else
S16 cmPkSchMacL2MeasReq(pst, measInfo)
Pst*                 pst;
RgInfL2MeasReq      *measInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkSchMacL2MeasReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }
   if(SAddPstMsgMult((Data *)measInfo, sizeof(RgInfL2MeasReq), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFL2MEASREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmPkSchMacL2MeasStopReq
(
Pst*                 pst,
RgInfL2MeasStopReq *measInfo
)
#else
S16 cmPkSchMacL2MeasStopReq(pst, measInfo)
Pst*                 pst;
RgInfL2MeasStopReq *measInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkSchMacL2MeasStopReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
       RETVALUE(RFAILED);
  }
   if(SAddPstMsgMult((Data *)measInfo, sizeof(RgInfL2MeasStopReq), mBuf) != ROK)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFL2MEASSTOPREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmPkSchMacL2MeasSendReq
(
Pst*                 pst,
RgInfL2MeasSndReq *measInfo
)
#else
S16 cmPkSchMacL2MeasSendReq(pst, measInfo)
Pst*                 pst;
RgInfL2MeasSndReq *measInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkSchMacL2MeasSendReq)
 if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
       RETVALUE(RFAILED);
  }

   if(SAddPstMsgMult((Data *)measInfo, sizeof(RgInfL2MeasSndReq), mBuf) != ROK)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFL2MEASSENDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkSchMacL2MeasReq
(
L2MeasReq      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkSchMacL2MeasReq(func, pst, mBuf)
L2MeasReq      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfL2MeasReq      measInfo;
   
   TRC2(cmUnpkSchMacL2MeasReq)

   if(SRemPreMsgMult((Data *)&measInfo, sizeof(RgInfL2MeasReq), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &measInfo));
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
#ifdef ANSI
S16 cmUnpkSchMacL2MeasStopReq
(
L2MeasStopReq func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkSchMacL2MeasStopReq(func, pst, mBuf)
L2MeasStopReq      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfL2MeasStopReq measInfo;

   TRC2(cmUnpkSchMacL2MeasStopReq)
   if(SRemPreMsgMult((Data *)&measInfo, sizeof(RgInfL2MeasStopReq), mBuf) != ROK)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   SPutMsg(mBuf);
   RETVALUE((*func)(pst, &measInfo));
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
#ifdef ANSI
S16 cmUnpkSchMacL2MeasSendReq
(
L2MeasSendReq      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkSchMacL2MeasSendReq(func, pst, mBuf)
L2MeasSendReq func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfL2MeasSndReq measInfo;

   TRC2(cmUnpkSchMacL2MeasSendReq)
 if(SRemPreMsgMult((Data *)&measInfo, sizeof(RgInfL2MeasSndReq), mBuf) != ROK)  
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   SPutMsg(mBuf);
   RETVALUE((*func)(pst, &measInfo));
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
#ifdef ANSI
S16 cmPkMacSchL2MeasCfm
(
Pst*                 pst,
RgInfL2MeasCfm       *measCfm
)
#else
S16 cmPkMacSchL2MeasCfm(pst, measCfm)
Pst*                 pst;
RgInfL2MeasCfm       *measCfm;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkMacSchL2MeasCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(SAddPstMsgMult((Data *)measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFL2MEASCFM;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmPkMacSchL2MeasStopCfm
(
Pst*                 pst,
RgInfL2MeasCfm       *measCfm
)
#else
S16 cmPkMacSchL2MeasStopCfm(pst, measCfm)
Pst*                 pst;
RgInfL2MeasCfm       *measCfm;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkMacSchL2MeasStopCfm)
 if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      RETVALUE(RFAILED);
   }

   if(SAddPstMsgMult((Data *)measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTINFL2MEASSTOPCFM;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
S16 cmUnpkMacSchL2MeasCfm
(
L2MeasCfm      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkMacSchL2MeasCfm(func, pst, mBuf)
L2MeasCfm      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   RgInfL2MeasCfm      measCfm;
   
   TRC2(cmUnpkMacSchL2MeasCfm)

   if(SRemPreMsgMult((Data *)&measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      RGSCHINF_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGSCHINF_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &measCfm));
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
#ifdef ANSI
S16 cmUnpkMacSchL2MeasStopCfm
(
L2MeasCfm      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkMacSchL2MeasStopCfm(func, pst, mBuf)
L2MeasCfm      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
RgInfL2MeasCfm      measCfm;

   TRC2(cmUnpkMacSchL2MeasStopCfm)

   if(SRemPreMsgMult((Data *)&measCfm, sizeof(RgInfL2MeasCfm), mBuf) != ROK)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   SPutMsg(mBuf);
   RETVALUE((*func)(pst, &measCfm));
} /* end of cmUnpkMacSchL2MeasStopCfm */

#endif/* LTE_L2_MEAS */

#endif


/**********************************************************************
 
         End of file
**********************************************************************/
