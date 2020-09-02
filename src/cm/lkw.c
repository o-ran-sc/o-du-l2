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
  
        Name:     LTE-RLC Layer 
    
        Type:     C source file
  
        Desc:     C Source code for common packing and unpacking functions
                  for RLC Layer Manager Interfaces.  It contains the 
                  following function definitions

                     -- packRlcConfigReq
                     -- cmPkLkwCntrlReq
                     -- cmPkLkwCntrlCfm 
                     -- packRlcConfigCfm
                     -- cmPkLkwStaInd

                     -- unpackRlcConfigReq 
                     -- cmUnpkLkwCntrlReq
                     -- cmUnpkLkwCntrlCfm
                     -- unpackRlcConfigCfm 
                     -- cmUnpkLkwStaInd
 
        File:     lkw.c

*********************************************************************21*/
 
  
/* header include files (.h) */
#include "common_def.h"
#include "lkw.h"                /* Layer manager */

/* header/extern include files (.x) */

#include "lkw.x"                /* layer manager */

#ifdef LCLKW

Txt ptNmb[LKW_PART_NUM_STR_LEN];         /* Part number */

  
/* forward references */

PRIVATE S16 cmPkKwMngmt ARGS ((KwMngmt *param ,Event eventType, 
                               Buffer *mBuf));
PRIVATE S16 cmPkRlcCfg ARGS ((KwMngmt *param ,Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmPkKwCntrl ARGS ((KwMngmt *cntrl, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmPkKwUsta ARGS ((KwMngmt *usta, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmPkKwSsta ARGS ((KwMngmt *ssta, Elmnt etmnt, Event eventType,
                              Buffer *mBuf));
PRIVATE S16 cmPkKwSts ARGS ((KwMngmt *sts, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmPkGenSts ARGS ((KwGenSts *gen, Buffer *mBuf));
PRIVATE S16 cmPkCkwSapSts ARGS ((KwCkwCntSts *rcSap, Buffer *mBuf));
PRIVATE S16 cmPkKwuSapSts ARGS ((KwKwuSapSts *ruSap, Buffer *mBuf));
/* lkw_c_001.main_2 removed support of RGU sap statistics as it is reflecting
 * in general statstics.
 * */
PRIVATE S16 cmPkKwTrc ARGS ((KwMngmt *trc, Buffer *mBuf));

PRIVATE S16 cmUnpkKwMngmt ARGS ((KwMngmt *param, Event eventType,
                                 Buffer * mBuf));
PRIVATE S16 cmUnpkRlcCfg ARGS ((KwMngmt *cfg, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkKwCntrl ARGS ((KwMngmt *param, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkKwUsta ARGS ((KwMngmt *usta, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkKwSsta ARGS ((KwMngmt *ssta, Elmnt elmnt, Event eventType,
                                Buffer *mBuf));
PRIVATE S16 cmUnpkKwSts ARGS ((KwMngmt *sts, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkGenSts ARGS ((KwGenSts *gen, Buffer *mBuf));
PRIVATE S16 cmUnpkCkwSapSts ARGS ((KwCkwCntSts *rcSap, Buffer *mBuf));
PRIVATE S16 cmUnpkKwuSapSts ARGS ((KwKwuSapSts *ruSap, Buffer *mBuf));
/* lkw_c_001.main_2 removed support of RGU sap statistics as it is reflecting
 * in general statstics.
 * */
PRIVATE S16 cmUnpkKwTrc ARGS ((KwMngmt *trc, Buffer *mBuf));


PRIVATE S16 cmPkCkwSapSta ARGS ((KwCkwCntSapSta *rcSap, Buffer *mBuf));
PRIVATE S16 cmUnpkCkwSapSta ARGS ((KwCkwCntSapSta *rcSap, Buffer *mBuf));

PRIVATE S16 cmPkKwuSapSta ARGS ((KwKwuSapSta *rlSap, Buffer *mBuf));
PRIVATE S16 cmUnpkKwuSapSta ARGS ((KwKwuSapSta *rlSap, Buffer *mBuf));

PRIVATE S16 cmPkRguSapSta ARGS ((KwRguSapSta *mkSap, Buffer *mBuf));
PRIVATE S16 cmUnpkRguSapSta ARGS ((KwRguSapSta *mkSap, Buffer *mBuf));

/**
 * @brief
     Pack Config Request
 *
 * @param[in] pst  : post structure 
 * @param[in] cfg  : RLC LM structure 
 *  @return  S16
 *      -# Success : ROK 
 *      -# Failure : RFAILED 
 */

#ifdef ANSI
PUBLIC S16 packRlcConfigReq
(
Pst *pst,                    /* post structure */
KwMngmt *cfg                 /* RLC LM structure */
)
#else
PUBLIC S16 packRlcConfigReq(pst, cfg)
Pst *pst;                    /* post structure */
KwMngmt *cfg;                /* RLC LM structure */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(packRlcConfigReq)

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_ADD_RES, ELKW001, (ErrVal) ret1,
             "SGetMsg failed ...!\n");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   ret1 = cmPkKwMngmt(cfg, LKW_EVT_CFG_REQ, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_ADD_RES, ELKW002, (ErrVal) ret1,
               "cmPkKwMngmt failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_CFG_REQ;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

}  /* end of packRlcConfigReq */

/**
 * @brief
     Unpack Config Request
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
 
#ifdef ANSI
PUBLIC S16 unpackRlcConfigReq
(
RlcConfigReq func,              /* primitive to call */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 unpackRlcConfigReq(func, pst, mBuf)
RlcConfigReq func;              /* primitive to call */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   S16 ret1;                 /* Return value */
   KwMngmt cfg;              /* RLC LM structure */

   TRC3(unpackRlcConfigReq)

   cmMemset((U8 *) &cfg, (U8) 0, (S16) sizeof(KwMngmt));
   ret1 = cmUnpkKwMngmt(&cfg, LKW_EVT_CFG_REQ, mBuf);

    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
      /*MBUF_FIXX*/
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW003, (ErrVal) ret1,
               "cmUnpkKwMngmt failure ......!");
      
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
       return RFAILED;
    }

   SPutMsg(mBuf);
   (Void) (*func)(pst, &cfg);

   return ROK;

} /* end of unpackRlcConfigReq */



/**
 * @brief
     Pack Control Request
 *
 * @param[in] pst  : post structure
 * @param[in] cntrl  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmPkLkwCntrlReq
(
Pst *pst,                    /* post structure */
KwMngmt *cntrl               /* RLC LM structure */  
)
#else
PUBLIC S16 cmPkLkwCntrlReq(pst,cntrl)
Pst *pst;                    /* post structure */
KwMngmt *cntrl;              /* RLC LM structure */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(cmPkLkwCntrlReq)

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW004, (ErrVal) ret1,
               "SGetMsg failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   ret1 = cmPkKwMngmt(cntrl, LKW_EVT_CNTRL_REQ, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW005, (ErrVal) ret1,
               "cmPkKwMngmt failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   pst->event = (Event) LKW_EVT_CNTRL_REQ;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

}  /* end of cmPkLkwCntrlReq */

/**
 * @brief
     Unpack Control Request
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmUnpkLkwCntrlReq
(
LkwCntrlReq func,            /* primitive to call */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwCntrlReq(func, pst, mBuf)
LkwCntrlReq func;            /* primitive to call */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   S16 ret1;                 /* return value */
   KwMngmt cfm;              /* Confirmation */

   TRC3(cmUnpkLkwCntrlReq)

   ret1 = cmUnpkKwMngmt(&cfm, LKW_EVT_CNTRL_REQ, mBuf);

    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
      /*MBUF_FIXX*/
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW006, (ErrVal) ret1,
               "cmUnpkKwMngmt failure ......!");
     
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
       return RFAILED;
    }

   SPutMsg(mBuf);
   RETVALUE((*func)(pst, &cfm));

} /* end of cmUnpkLkwCntrlReq */

/**
 * @brief
     Pack Control Confirm
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmPkLkwCntrlCfm
(
Pst *pst,                    /* post structure */
KwMngmt *cfm                 /* RLC LM structure */
)
#else
PUBLIC S16 cmPkLkwCntrlCfm(pst, cfm)
Pst *pst;                    /* post structure */
KwMngmt *cfm;                /* RLC LM structure */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(cmPkLkwCntrlCfm)

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW007, (ErrVal) ret1,
               "SGetMsg failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   ret1 = cmPkKwMngmt(cfm, LKW_EVT_CNTRL_CFM, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW008, (ErrVal) ret1,
               "Packing failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_CNTRL_CFM;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

}  /* end of cmPkLkwCntrlCfm */


/**
 * @brief
     Unpack Control confirm
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmUnpkLkwCntrlCfm
(
LkwCntrlCfm func,            /* primitive to call */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwCntrlCfm(func, pst, mBuf)
LkwCntrlCfm func;            /* primitive to call */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   KwMngmt cfm;              /* RLC LM structure */
   S16 ret1;                 /* return value */

   TRC3(cmUnpkLkwCntrlCfm)

   ret1 = cmUnpkKwMngmt(&cfm, LKW_EVT_CNTRL_CFM, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW009, (ErrVal) ret1,
               "cmUnpkKwMngmt failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
      return RFAILED;
   }
      
   SPutMsg(mBuf); 

   RETVALUE((*func)(pst, &cfm));

} /* end of cmUnpkLkwCntrlCfm */

/**
 * @brief
     Unpack statistics Confirm
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmUnpkLkwStsCfm
(
LkwStsCfm func,              /* primitive to call */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwStsCfm(func, pst, mBuf)
LkwStsCfm func;              /* primitive to call */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   KwMngmt cfm;              /* RLC LM structure */
   S16 ret1;                 /* return value */

   TRC3(cmUnpkLkwStsCfm)

   ret1 = cmUnpkKwMngmt(&cfm, LKW_EVT_STS_CFM, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW010, (ErrVal) ret1,
               "SPutMsg failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
      return RFAILED;
   }
     
   SPutMsg(mBuf);

   RETVALUE((*func)(pst, 0, &cfm));

} /* end of cmUnpkLkwStsCfm */

/**
 * @brief
     Pack Config Confirm 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */


#ifdef ANSI
PUBLIC S16 packRlcConfigCfm
(
Pst *pst,                    /* post structure */
KwMngmt *cfm                 /* RLC LM structure */
)
#else
PUBLIC S16 packRlcConfigCfm(pst,cfm)
Pst *pst;                    /* post structure */
KwMngmt *cfm;                /* RLC LM structure */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(packRlcConfigCfm)

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW011, (ErrVal) ret1,
               "SGetMsg failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   ret1 = cmPkKwMngmt(cfm, LKW_EVT_CFG_CFM, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW012, (ErrVal) ret1,
               "SPutMsg failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_CFG_CFM;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

}  /* end of packRlcConfigCfm */


/**
 * @brief
     Unpack Config Confirm 
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 unpackRlcConfigCfm
(
RlcConfigCfm func,              /* primitive to call */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 unpackRlcConfigCfm(func, pst, mBuf)
RlcConfigCfm func;              /* primitive to call */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   S16 ret1;                 /* return value */
   KwMngmt cfm;              /* RLC LM structure */

   TRC3(unpackRlcConfigCfm)

   ret1 = cmUnpkKwMngmt(&cfm, LKW_EVT_CFG_CFM, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW013, (ErrVal) ret1,
               "SPutMsg failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
      return RFAILED;
   }

   SPutMsg(mBuf);
   RETVALUE((*func)(pst, &cfm));

} /* end of unpackRlcConfigCfm */



/**
 * @brief
     Pack Status Indication 
 *
 * @param[in] pst  : post structure
 * @param[in] usta  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmPkLkwStaInd
(
Pst *pst,                    /* post structure */
KwMngmt *usta                /* RLC LM structure */
)
#else
PUBLIC S16 cmPkLkwStaInd(pst,usta)
Pst *pst;                    /* post structure */
KwMngmt *usta;               /* RLC LM structure */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(cmPkLkwStaInd)

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW014, (ErrVal) ret1,
               "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   ret1 = cmPkKwMngmt(usta, LKW_EVT_STA_IND, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW015, (ErrVal) ret1,
               "cmPkKwMngmt failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   pst->event = (Event) LKW_EVT_STA_IND;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

}  /* end of cmPkLkwStaInd */


/**
 * @brief
     Unpack Unsolicited Status indication 
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmUnpkLkwStaInd
(
LkwStaInd func,             /* primitive pointer */
Pst *pst,                   /* post structure */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwStaInd(func, pst, mBuf)
LkwStaInd func;             /* primitive pointer */
Pst *pst;                   /* post structure */
Buffer *mBuf;               /* message buffer */
#endif
{
   KwMngmt usta;            /* RLC LM structure */
   S16 ret1;                /* return value */

   TRC3(cmUnpkLkwStaInd)
   
   ret1 = cmUnpkKwMngmt(&usta, LKW_EVT_STA_IND, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW016, (ErrVal) ret1,
               "cmUnpkKwMngmt failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
      return RFAILED;
   }

   SPutMsg(mBuf);

   RETVALUE((*func)(pst, &usta));
} /* end of cmUnpkLsnStaInd */

/**
 * @brief
     Pack Status Request
 *
 * @param[in] pst  : post structure
 * @param[in] sta : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmPkLkwStaReq
(
Pst *pst,                    /* post structure */
KwMngmt *sta                 /* status */
)
#else
PUBLIC S16 cmPkLkwStaReq(pst,sta)
Pst *pst;                    /* post structure */
KwMngmt *sta;                /* status */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(cmPkLkwStaReq)

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW017, (ErrVal) ret1,
               "SGetMsg failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }
 
   ret1 = cmPkKwMngmt(sta, LKW_EVT_STA_REQ, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW018, (ErrVal) ret1,
               "SGetMsg() failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_STA_REQ;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

}  /* end of cmPkLkwStaReq */

/**
 * @brief
     Unpack status confirmation 
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmUnpkLkwStaCfm
(
LkwStaReq func,              /* primitive pointer */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwStaCfm(func, pst, mBuf)
LkwStaReq func;              /* primitive pointer */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   KwMngmt cfm;              /* RLC LM structure */
   S16 ret1;                 /* return value */

   TRC3(cmUnpkLkwStaReq)

   ret1 = cmUnpkKwMngmt(&cfm, LKW_EVT_STA_CFM, mBuf);

    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW019, (ErrVal) ret1,
               "Unpacking failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
       return RFAILED;
    }

   SPutMsg(mBuf);
  
   RETVALUE((*func)(pst, &cfm));

} /* end of function cmUnpkLkwStaCfm */

/**
 * @brief
     Unpack Status request 
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmUnpkLkwStsReq
(
LkwStsReq func,              /* primitive pointer */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwStsReq(func, pst, mBuf)
LkwStsReq func;              /* primitive pointer */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   KwMngmt sts;              /* RLC LM structure */
   S16 ret1;                 /* return value */
   Action action;            /* Action */

   TRC3(cmUnpkLkwStsReq)

   CMCHKUNPK(SUnpkS16, &action, mBuf);

   ret1 = cmUnpkKwMngmt(&sts, LKW_EVT_STS_REQ, mBuf);

    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
       /*MBUF_FIXX*/
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW020, (ErrVal) ret1,
               "Unpacking failure ......!");
      
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
       return RFAILED;
    }

   SPutMsg(mBuf);

   RETVALUE((*func)(pst, action, &sts));

} /* end of function cmUnpkLkwStsReq */


/**
 * @brief
     Unpack ssta 
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmUnpkLkwStaReq
(
LkwStaReq func,              /* primitive pointer */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwStaReq(func, pst, mBuf)
LkwStaReq func;              /* primitive pointer */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   KwMngmt sta;              /* RLC LM structure */
   S16 ret1;                 /* return value */

   TRC3(cmUnpkLkwStaReq)

   ret1 = cmUnpkKwMngmt(&sta, LKW_EVT_STA_REQ, mBuf);

    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
      /*MBUF_FIXX*/
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW021, (ErrVal) ret1,
               "Unpacking failure ......!");
      
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
       return RFAILED;
    }

   SPutMsg(mBuf);

   RETVALUE((*func)(pst, &sta));

} /* end of function cmUnpkLkwStaReq */


/**
 * @brief
     Unpack Trace Indication 
 *
 * @param[in] func  : primitive to call
 * @param[in] pst   : post structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 cmUnpkLkwTrcInd
(
LkwTrcInd func,              /* primitive to call */
Pst *pst,                    /* post structure */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 cmUnpkLkwTrcInd(func, pst, mBuf)
LkwTrcInd func;              /* primitive to call */
Pst *pst;                    /* post structure */
Buffer *mBuf;                /* message buffer */
#endif
{
   S16 ret1;                 /* return value */
   KwMngmt trc;              /* RLC LM structure */

   TRC3(cmUnpkLkwTrcInd)

   ret1 = cmUnpkKwMngmt(&trc, LKW_EVT_TRC_IND, mBuf);

    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW022, (ErrVal) ret1,
               "Unpacking failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_DEBUG  */
       return RFAILED;
    }

   (Void) (*func)(pst, &trc, mBuf);

   return ROK;

} /* end of cmUnpkLkwTrcInd */


/**
 * @brief
     Pack Config Request
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PRIVATE S16 cmPkCkwSapSta 
( 
KwCkwCntSapSta *rcSap,         /* SAP statistics */      
Buffer *mBuf                 /* Message buffer */
)
#else
PRIVATE S16 cmPkCkwSapSta(rcSap, mBuf)
KwCkwCntSapSta *rcSap;         /* SAP statistics */
Buffer *mBuf;                /* Message buffer */
#endif
{
   TRC3(cmPkCkwSapSta)

   CMCHKPK(cmPkState, rcSap->state, mBuf);
   CMCHKPK(SPkS16, rcSap->spId, mBuf);   

   return ROK;

} /* end of cmPkCkwSapSta */

/**
 * @brief
     Unpack CKW sap Sta 
 *
 * @param[out] rcSap: RLC LM structure 
 * @param[in] buffer: Buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkCkwSapSta 
(
KwCkwCntSapSta *rcSap,         /* SAP statistics */ 
Buffer *mBuf                 /* Message buffer */
)
#else
PRIVATE S16 cmUnpkCkwSapSta(rcSap, mBuf)
KwCkwCntSapSta *rcSap;         /* SAP statistics */ 
Buffer *mBuf;                /* Message buffer */
#endif
{
   TRC3(cmUnpkCkwSapSta)

   CMCHKUNPK(SUnpkS16, &rcSap->spId, mBuf);
   CMCHKUNPK(cmUnpkState, &rcSap->state, mBuf);
  
   return ROK;
} /* end of function cmUnpkCkwSapSta */

/**
 * @brief
     Pack Config Request
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkKwuSapSta
(
KwKwuSapSta *rlSap,           /* SAP statistics */       
Buffer *mBuf                 /* Message buffer */
)
#else
PRIVATE S16 cmPkKwuSapSta(rlSap, mBuf)
KwKwuSapSta *rlSap;           /* SAP statistics */
Buffer *mBuf;                /* Message buffer */
#endif
{
   TRC3(cmPkKwuSapSta)

   CMCHKPK(cmPkState, rlSap->state, mBuf);
   CMCHKPK(SPkS16, rlSap->spId, mBuf);

   return ROK;

} /* end of cmPkKwuSapSta */

/**
 * @brief
     Unpack Kwu Sap Sta 
 *
 * @param[out] rlSap  : KwRLSapSta structure 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwuSapSta
(
KwKwuSapSta *rlSap,           /* SAP statistics */                   
Buffer *mBuf                 /* Message buffer */
)
#else
PRIVATE S16 cmUnpkKwuSapSta(rlSap, mBuf)
KwKwuSapSta *rlSap;           /* SAP statistics */
Buffer *mBuf;                /* Message buffer */
#endif
{
   TRC3(cmUnpkKwuSapSta)

   CMCHKUNPK(SUnpkS16, &rlSap->spId, mBuf);
   CMCHKUNPK(cmUnpkState, &rlSap->state, mBuf);

   return ROK;
 
} /* end of function cmUnpkKwuSapSta */

/**
 * @brief
     Pack 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkRguSapSta
(
KwRguSapSta *mkSap,           /* SAP statistics */             
Buffer *mBuf                 /* Message buffer */
)
#else
PRIVATE S16 cmPkRguSapSta(mkSap, mBuf)
KwRguSapSta *mkSap;           /* SAP statistics */
Buffer *mBuf;                /* Message buffer */
#endif
{
   TRC3(cmPkRguSapSta)

   CMCHKPK(cmPkState, mkSap->state, mBuf);
   CMCHKPK(cmPkSuId, mkSap->suId, mBuf);

   return ROK;

} /* end of cmPkRguSapSta */

/**
 * @brief
     Unpack Rgu Sap Sta 
 *
 * @param[out] mkSap: KwRguSapSta structure 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkRguSapSta
(
KwRguSapSta *mkSap,           /* SAP statistics */              
Buffer *mBuf                 /* Message buffer */
)
#else
PRIVATE S16 cmUnpkRguSapSta(mkSap, mBuf)
KwRguSapSta *mkSap;           /* SAP statistics */
Buffer *mBuf;                /* Message buffer */
#endif
{
   TRC3(cmUnpkRguSapSta)

   CMCHKUNPK(cmUnpkSuId, &mkSap->suId, mBuf);
   CMCHKUNPK(cmUnpkState, &mkSap->state, mBuf);

   return ROK;

} /* end of function cmUnpkRguSapSta */

/**
 * @brief
     Pack 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmPkLkwStaCfm
(
Pst *pst,                    /* post structure */
KwMngmt *cfm                 /* solicited status confirm */
)
#else
PUBLIC S16 cmPkLkwStaCfm(pst, cfm)
Pst *pst;                    /* post structure */
KwMngmt *cfm;                /* solicited status confirm */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(cmPkLkwStaCfm);

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW023, (ErrVal) ret1,
               "SGetMsg() failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   ret1 = cmPkKwMngmt(cfm, LKW_EVT_STA_CFM, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW024, (ErrVal) ret1,
               "packing failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_STA_CFM;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;
} /* end of cmPkLkwStaCfm */


/**
 * @brief
     Pack 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmPkLkwTrcInd
(
Pst *pst,                    /* post structure */
KwMngmt *trc,                /* trace */
Buffer *trcBuf                 /* trace buffer */
)
#else
PUBLIC S16 cmPkLkwTrcInd(pst, trc, trcBuf)
Pst *pst;                     /* post structure */
KwMngmt *trc;                 /* trace */
Buffer *trcBuf;               /* trace buffer */
#endif
{
   S16      ret1 = ROK;       /* return value */
   Buffer   *mBuf = NULLP;    /* Buffer to trace */

   TRC3(cmPkLkwTrcInd);

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW025, (ErrVal) ret1,
               "SGetMsg() failure ......!");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   if (trcBuf)
   {
      ret1 = SCatMsg(mBuf, trcBuf, M1M2);
      if (ret1 != ROK)
      {
         SPutMsg(mBuf);
         SPutMsg(trcBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         if(ret1 != ROK)
         {
            LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW026, (ErrVal) ret1,
                  "SGetMsg() failure ......!");
         }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         return RFAILED;
      }

      SPutMsg(trcBuf);
   }

   ret1 = cmPkKwMngmt(trc, LKW_EVT_TRC_IND, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW027, (ErrVal) ret1,
               "packing failure ......!");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      /* log error */
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_TRC_IND;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

} /* end of cmPkLkwTrcInd */


/**
 * @brief
     Pack 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmPkLkwStsReq
(
Pst *pst,                    /* post structure */
Action action,               /* action */
KwMngmt *sts                 /* statistics request */
)
#else
PUBLIC S16 cmPkLkwStsReq(pst, action, sts)
Pst *pst;                    /* post structure */
Action action;               /* action */
KwMngmt *sts;                /* statistics request */
#endif
{

   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(cmPkLkwStsReq)

   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW028, (ErrVal) ret1,
               "SGetMsg() failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   ret1 = cmPkKwMngmt(sts, LKW_EVT_STS_REQ, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW029, (ErrVal) ret1,
               "Packing failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   CMCHKPK(SPkS16, action, mBuf); 
   pst->event = (Event) LKW_EVT_STS_REQ;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;

} /* end of cmPkLkwStsReq */


/**
 * @brief
     Pack 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 cmPkLkwStsCfm
(
Pst *pst,                    /* post structure */
Action action,               /* action */
KwMngmt *cfm                 /* statistics confirmation */
)
#else
PUBLIC S16 cmPkLkwStsCfm(pst, action, cfm)
Pst *pst;                    /* post structure */
Action action;               /* action */
KwMngmt *cfm;                /* statistics confirmation */
#endif
{
   Buffer *mBuf;             /* message buffer */
   S16 ret1;                 /* return value */

   TRC3(cmPkLkwStsCfm);
   /* lkw_c_001.main_2 removed warning */
   UNUSED(action);
   ret1 = SGetMsg(pst->region, pst->pool, &mBuf);
   if (ret1 != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW030, (ErrVal) ret1,
               "SGetMsg() failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   ret1 = cmPkKwMngmt(cfm, LKW_EVT_STS_CFM, mBuf);

   if(ret1 != ROK)
   {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          LKWLOGERROR(pst, ERRCLS_INT_PAR, ELKW031, (ErrVal) ret1,
               "Packing  failure ......!");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_STS_CFM;   /* event */
   SPstTsk(pst, mBuf);

   return ROK;
} /* end of cmPkLkwStsCfm */


/**
 * @brief
     Pack  
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PRIVATE S16 cmPkKwMngmt
(
KwMngmt *param,              /* Managment */             
Event eventType,             /* Event type */
Buffer *mBuf                 /* Message Buffer */
)
#else
PRIVATE S16 cmPkKwMngmt(param ,eventType, mBuf)
KwMngmt *param;              /* Managment */
Event eventType;             /* Event type */
Buffer *mBuf;                /* Message Buffer */
#endif
{
    S16 ret1;                /* return value */

    TRC3(cmPkKwMngmt)

    switch( eventType )
    {
       case LKW_EVT_CFG_REQ:
          ret1 = cmPkRlcCfg(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_CNTRL_REQ:
         ret1 = cmPkKwCntrl(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_STA_IND:
          ret1 = cmPkKwUsta(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_STA_REQ:
       case LKW_EVT_STA_CFM:
          ret1 = cmPkKwSsta(param, param->hdr.elmId.elmnt , eventType, mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_STS_REQ:
       case LKW_EVT_STS_CFM:
          ret1 = cmPkKwSts(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;
       case LKW_EVT_TRC_IND:
          ret1 = cmPkKwTrc(param, mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;
       case LKW_EVT_CNTRL_CFM:
       case LKW_EVT_CFG_CFM:
          break;
    }

   CMCHKPK(cmPkCmStatus, &param->cfm, mBuf);
   CMCHKPK(cmPkHeader, &param->hdr, mBuf);

   return ROK;
} /* end of function cmPkKwMngmt */


/**
 * @brief
     Pack 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkRlcCfg
(
KwMngmt *cfg,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkRlcCfg(cfg, elmnt, mBuf)
KwMngmt *cfg;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmPkRlcCfg)

    switch( elmnt )
    {
      case STGEN:
      {
         CMCHKPK(oduUnpackUInt16, cfg->t.cfg.s.gen.maxRguSaps, mBuf);
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.gen.rlcMode, mBuf);
         CMCHKPK(oduUnpackUInt32, cfg->t.cfg.s.gen.timeRes, mBuf);
         CMCHKPK(oduUnpackUInt16, cfg->t.cfg.s.gen.maxUdxSaps, mBuf);
         CMCHKPK(oduUnpackUInt16, cfg->t.cfg.s.gen.maxKwuSaps, mBuf);
         CMCHKPK(oduUnpackUInt32, cfg->t.cfg.s.gen.maxUe, mBuf);
         CMCHKPK(cmPkPst, &cfg->t.cfg.s.gen.lmPst,mBuf);

         break;
      }
      case STCKWSAP:
      case STKWUSAP:
      case STUDXSAP:
      case STRGUSAP:
      {
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.sap.route,mBuf);
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.sap.priority,mBuf);
         CMCHKPK(oduUnpackUInt16, cfg->t.cfg.s.sap.bndTmrIntvl, mBuf);
         CMCHKPK(SPkS16, cfg->t.cfg.s.sap.sapId, mBuf);
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.sap.inst, mBuf);
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.sap.ent, mBuf);
         CMCHKPK(oduUnpackUInt16, cfg->t.cfg.s.sap.procId, mBuf);
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.sap.mem.pool, mBuf);
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.sap.mem.region, mBuf);
         CMCHKPK(oduUnpackUInt8, cfg->t.cfg.s.sap.selector, mBuf);
         break;
      }
       default:
          return RFAILED;
    }
    return ROK;
} /*end of function cmPkRlcCfg*/


/**
 * @brief
     Pack kw control request
 *
 * @param[in] cntrl  : LM management structure
 * @param[in] elmnt  : element 
 * @param[in] mBuf   : Buffer 
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkKwCntrl
(
KwMngmt *cntrl,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkKwCntrl(cntrl, elmnt, mBuf)
KwMngmt *cntrl;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
    U8 subAction;            /* sub action */     

    TRC3(cmPkKwCntrl)

    switch( elmnt )
    {
      case STGEN:
         {
            subAction = cntrl->t.cntrl.subAction;
            switch(subAction)
            {
               case SAUSTA:
                  break;
               case SADBG:
                  {
                      CMCHKPK(oduUnpackUInt32, cntrl->t.cntrl.s.dbgCntrl.dbgMask,
                                                           mBuf);
                  }
                  break;
               case SATRC:
                  {
                     CMCHKPK(SPkS16, cntrl->t.cntrl.s.trcCntrl.trcLen, 
                                                           mBuf);
                     CMCHKPK(oduUnpackUInt8, cntrl->t.cntrl.s.trcCntrl.trcMask,
                                                           mBuf);
                  }
                  break;
#ifdef SS_DIAG
               case SALOG:
                  {
                      CMCHKPK(oduUnpackUInt32, cntrl->t.cntrl.s.logMask,  mBuf);
                  }
                  break;
#endif
               default:
                  break;
            }
            break;
         }
      case STRGUSAP:
      case STUDXSAP:
         CMCHKPK(SPkS16, cntrl->t.cntrl.s.sapCntrl.spId, mBuf);
         CMCHKPK(SPkS16, cntrl->t.cntrl.s.sapCntrl.suId, mBuf);
         break;
      default:
         break;
   }

   CMCHKPK(oduUnpackUInt8, cntrl->t.cntrl.action, mBuf);
   CMCHKPK(oduUnpackUInt8, cntrl->t.cntrl.subAction, mBuf);
   CMCHKPK(cmPkDateTime, &cntrl->t.cntrl.dt, mBuf);

   return ROK;
} /*end of function cmPkKwCntrl */

/**
 * @brief
     Pack unsolicit status  
 *
 * @param[in] usta  : RLC LM structure 
 * @param[in] elmnt : element
 * @param[in] mBuf  : Buffer 
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkKwUsta
(
KwMngmt *usta,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkKwUsta(usta, elmnt, mBuf)
KwMngmt *usta;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmPkKwUsta)

   /* lkw_c_001.main_2 removed warning */
   UNUSED(elmnt);
   /* lkw_c_001.main_2 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   CMCHKPK(oduUnpackUInt8, usta->t.usta.qci, mBuf);
#endif
   CMCHKPK(oduUnpackUInt32, usta->t.usta.ueId, mBuf);
   CMCHKPK(SPkS16, usta->t.usta.suId, mBuf);
   CMCHKPK(cmPkCmAlarm, &usta->t.usta.alarm,mBuf);
   CMCHKPK(cmPkDateTime, &usta->t.usta.dt, mBuf);

   return ROK;

} /* end of cmPkKwUsta */


/**
 * @brief
     Pack 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkKwSsta
(
KwMngmt *ssta,
Elmnt elmnt,
Event eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkKwSsta(ssta, elmnt, eventType, mBuf)
KwMngmt *ssta;
Elmnt elmnt;
Event eventType;
Buffer *mBuf;
#endif
{
   S16 ret1;
   TRC3(cmPkKwSsta)

   ret1 = RFAILED;
   switch(elmnt)
   {
      case STSID:
         ret1 = ROK;
         if(eventType == LKW_EVT_STA_CFM)
         {
            CMCHKPK(cmPkSystemId, &ssta->t.ssta.s.sysId,mBuf);
         }
         break;
      case STCKWSAP:
         ret1 = cmPkCkwSapSta(&ssta->t.ssta.s.ckwSap, mBuf);
         break;
      case STKWUSAP:
         ret1 = cmPkKwuSapSta(&ssta->t.ssta.s.kwuSap, mBuf);
         break;
      case STRGUSAP:
      case STUDXSAP:
         ret1 = cmPkRguSapSta(&ssta->t.ssta.s.rguSap, mBuf);
         break;
      default:
         ret1 = RFAILED;
        break;
   }

   if(ret1 != ROK)
   {
      return RFAILED;
   }

   CMCHKPK(cmPkDateTime, &ssta->t.ssta.dt, mBuf);

   return ROK;

} /* end of cmPkKwSsta */

/**
 * @brief
     Pack Trace Indication
 *
 * @param[in] trc  : RLC LM structure 
 * @param[in] mBuf : Buffer 
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkKwTrc
(
KwMngmt *trc,                /* trace */                 
Buffer *mBuf                 /* data buffer */
)
#else
PRIVATE S16 cmPkKwTrc(trc, mBuf)
KwMngmt *trc;                /* trace */
Buffer *mBuf;                /* data buffer */
#endif
{
   TRC3(cmPkKwTrc)

   CMCHKPK(oduUnpackUInt16, trc->t.trc.event, mBuf); 
   CMCHKPK(cmPkDateTime, &trc->t.trc.dt, mBuf);

   return ROK;
} /* end of cmPkKwTrc */


/**
 * @brief
     Pack Statistics request 
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

#ifdef ANSI
PRIVATE S16 cmPkKwSts
(
KwMngmt *sts,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkKwSts(sts, elmnt, mBuf)
KwMngmt *sts;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1;
   TRC3(cmPkKwSts)

   ret1 = RFAILED;
   switch(elmnt)
   {
      case STGEN:
         ret1 = cmPkGenSts(&sts->t.sts.s.gen, mBuf); 
         break;
      case STCKWSAP:
         ret1 = cmPkCkwSapSts(&sts->t.sts.s.ckwSap, mBuf);
         break;
      case STKWUSAP:
         ret1 = cmPkKwuSapSts(&sts->t.sts.s.kwuSap, mBuf);
         break;
/* lkw_c_001.main_2 removed support of RGU sap statistics as it is reflecting
 * in general statstics.
 * */
      default:
         ret1 = RFAILED;
        break;
   } /* end of switch */

   if(ret1 != ROK)
   {
      return RFAILED;
   }

   CMCHKPK(cmPkDateTime, &sts->t.sts.dt, mBuf);

   return ROK;
} /* end of cmPkKwSts */



/**
 * @brief
     Pack general statistics 
 *
 * @param[in] gen  : KwGenSts structure 
 * @param[in] mBuf : Buffer 
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkGenSts
(
KwGenSts *gen,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkGenSts(gen, mBuf)
KwGenSts *gen;
Buffer *mBuf;
#endif
{
   TRC3(cmPkGenSts)

   CMCHKPK(oduUnpackUInt32, gen->numSduDisc, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->numOfRb, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->protTimeOut, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->errorPdusRecv, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->unexpPdusRecv, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->bytesSent, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->bytesRecv, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->pdusRetx, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->pdusSent, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->pdusRecv, mBuf);
   CMCHKPK(oduUnpackUInt32, gen->numUe, mBuf);

   return ROK;

} /* end of cmPkGenSts */

/**
 * @brief
     Pack CKW statistics
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkCkwSapSts
(
KwCkwCntSts *rcSap,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkCkwSapSts(rcSap, mBuf)
KwCkwCntSts *rcSap;
Buffer *mBuf;
#endif
{
   TRC3(cmPkCkwSapSts)

   CMCHKPK(oduUnpackUInt32, rcSap->statMsgs, mBuf);

   return ROK;
} /* end of cmPkCkwSapSts */

/**
 * @brief
     Pack Config Request
 *
 * @param[in] pst  : post structure
 * @param[in] cfg  : RLC LM structure
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmPkKwuSapSts
(
KwKwuSapSts *ruSap,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkKwuSapSts(ruSap, mBuf)
KwKwuSapSts *ruSap;
Buffer *mBuf;
#endif
{
   TRC3(cmPkCkwSapSts)

   CMCHKPK(oduUnpackUInt32, ruSap->sduTx, mBuf);
   CMCHKPK(oduUnpackUInt32, ruSap->sduRx, mBuf);
   /* lkw_c_001.main_2 changed from suId to spId */
   CMCHKPK(cmPkSuId, ruSap->spId, mBuf);

   return ROK;
} /* end of cmPkCkwSapSts */

/* lkw_c_001.main_2 removed support of RGU sap statistics as it is reflecting
 * in general statstics.
 * */

/**
 * @brief
     Unpack RLC Layer Management structure 
 *
 * @param[in] param  : RLC LM structure
 * @param[in] eventType   : event type 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwMngmt
(
KwMngmt *param,
Event eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkKwMngmt(param ,eventType, mBuf)
KwMngmt *param;
Event eventType;
Buffer *mBuf;
#endif
{
    S16 ret1;
    TRC3(cmUnpkKwMngmt)

   cmMemset((U8 *) param, (U8) 0, (S16) sizeof(KwMngmt));

   CMCHKUNPK(cmUnpkHeader, &param->hdr, mBuf);
   CMCHKUNPK(cmUnpkCmStatus, &param->cfm, mBuf);

    switch( eventType )
    {
       case  LKW_EVT_CFG_REQ:
          ret1 = cmUnpkRlcCfg(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;
       case LKW_EVT_CNTRL_REQ:
          ret1 = cmUnpkKwCntrl(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_STA_IND:
          ret1 = cmUnpkKwUsta(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_STA_REQ:
       case LKW_EVT_STA_CFM:
          ret1 = cmUnpkKwSsta(param, param->hdr.elmId.elmnt, eventType,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_STS_REQ:
       case LKW_EVT_STS_CFM:
          
          ret1 = cmUnpkKwSts(param, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;
       case LKW_EVT_TRC_IND:
          ret1 = cmUnpkKwTrc(param, mBuf);
          if (ret1 != ROK)
          {
             return RFAILED;
          }
          break;

       case LKW_EVT_CNTRL_CFM:
       case LKW_EVT_CFG_CFM:
       break;

       default:
          break;
    }
    return ROK;
} /*end of function cmUnpkKwMngmt*/

/**
 * @brief
     Unpack Kw configuration structure 
 *
 * @param[out] cfg  : RLC LM structure 
 * @param[in] elmnt  : element 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkRlcCfg
(
KwMngmt *cfg,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkRlcCfg(cfg ,elmnt, mBuf)
KwMngmt *cfg;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkRlcCfg)

   switch(elmnt)
   {
      case STGEN:
      {
         CMCHKUNPK(cmUnpkPst, &cfg->t.cfg.s.gen.lmPst,mBuf);
         CMCHKUNPK(oduPackUInt32, &cfg->t.cfg.s.gen.maxUe, mBuf);
         CMCHKUNPK(oduPackUInt16, &cfg->t.cfg.s.gen.maxKwuSaps, mBuf);
         CMCHKUNPK(oduPackUInt16, &cfg->t.cfg.s.gen.maxUdxSaps, mBuf);
         CMCHKUNPK(oduPackUInt32, &cfg->t.cfg.s.gen.timeRes, mBuf);
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.gen.rlcMode, mBuf);
         CMCHKUNPK(oduPackUInt16, &cfg->t.cfg.s.gen.maxRguSaps, mBuf);
      }
      break;

      case STCKWSAP:
      case STKWUSAP:
      case STRGUSAP:
      case STUDXSAP:
      {
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.sap.selector, mBuf);
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.sap.mem.region, mBuf);
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.sap.mem.pool, mBuf);
         CMCHKUNPK(oduPackUInt16, &cfg->t.cfg.s.sap.procId, mBuf);
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.sap.ent, mBuf);
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.sap.inst, mBuf);
         CMCHKUNPK(SUnpkS16, &cfg->t.cfg.s.sap.sapId, mBuf);
         CMCHKUNPK(oduPackUInt16, &cfg->t.cfg.s.sap.bndTmrIntvl, mBuf);
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.sap.priority,mBuf);
         CMCHKUNPK(oduPackUInt8, &cfg->t.cfg.s.sap.route,mBuf);
      }
      break;
      default:
        return RFAILED;
   } /* end of switch */     

   return ROK;
} /* end of cmUnpkRlcCfg */

/**
 * @brief
     Unpack Control Structure 
 *
 * @param[in] cfm   : RLC LM structure 
 * @param[in] elmnt : element 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwCntrl
(
KwMngmt *cfm,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkKwCntrl(cfm, elmnt, mBuf)
KwMngmt *cfm;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   U8 subAction;
   KwDbgCntrl *dbgCntrl;
   KwTrcCntrl *trcCntrl;

   TRC3(cmUnpkKwCntrl);
   
   /* lkw_c_001.main_2 removed warning */
   UNUSED(elmnt);

   CMCHKUNPK(cmUnpkDateTime, &cfm->t.cntrl.dt, mBuf);
   CMCHKUNPK(oduPackUInt8, &cfm->t.cntrl.subAction, mBuf);
   CMCHKUNPK(oduPackUInt8, &cfm->t.cntrl.action, mBuf);

   subAction = cfm->t.cntrl.subAction;

   switch(cfm->hdr.elmId.elmnt)
   {
      case STGEN:
         {
            switch(subAction)
            {
               case SAUSTA:
                  break;
               case SADBG:
                  {
                     dbgCntrl = &cfm->t.cntrl.s.dbgCntrl;
                     CMCHKUNPK(oduPackUInt32, &dbgCntrl->dbgMask, mBuf);
                  }
                  break;
               case SATRC:
                  {
                    trcCntrl = &cfm->t.cntrl.s.trcCntrl; 
                    CMCHKUNPK(oduPackUInt8, &trcCntrl->trcMask, mBuf);
                    CMCHKUNPK(SUnpkS16,&trcCntrl->trcLen, mBuf);
                  }
                  break;
#ifdef SS_DIAG
               case SALOG:
                  {
                      CMCHKUNPK(oduPackUInt32, &cfm->t.cntrl.s.logMask, mBuf);
                  }
                  break;
#endif
               default:
                  break;
            }
            break;
         }
      case STRGUSAP:
      case STUDXSAP:
           CMCHKUNPK(SUnpkS16, &cfm->t.cntrl.s.sapCntrl.suId, mBuf);
           CMCHKUNPK(SUnpkS16, &cfm->t.cntrl.s.sapCntrl.spId, mBuf);
           break;
      default:
           break;
   }

   return ROK;

} /* end of cmUnpkKwCntrl */


/**
 * @brief
     Unpack USta 
 *
 * @param[in] usta  : RLC LM Structure
 * @param[in] elmnt : element
 * @param[out] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwUsta
(
KwMngmt *usta,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkKwUsta(usta ,elmnt, mBuf)
KwMngmt *usta;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkKwUsta);

   /* lkw_c_001.main_2 removed warning */
   UNUSED(elmnt);

   CMCHKUNPK(cmUnpkDateTime, &usta->t.usta.dt, mBuf);
   CMCHKUNPK(cmUnpkCmAlarm, &usta->t.usta.alarm,mBuf);
   CMCHKUNPK(SUnpkS16, &usta->t.usta.suId, mBuf);
   CMCHKUNPK(oduPackUInt32, &usta->t.usta.ueId, mBuf);
   /* lkw_c_001.main_2 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   CMCHKUNPK(oduPackUInt8, &usta->t.usta.qci, mBuf);
#endif
   return ROK;
}

/**
 * @brief
     Unpack SSta 
 *
 * @param[out] ssta: RLC LM structure 
 * @param[in] elmnt  : element 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwSsta
(
KwMngmt *ssta,
Elmnt elmnt,
Event eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkKwSsta(ssta ,elmnt, eventType, mBuf)
KwMngmt *ssta;
Elmnt elmnt;
Event eventType;
Buffer *mBuf;
#endif
{
   S16 ret1;           /* return value */
   TRC3(cmUnpkKwSsta);

   CMCHKUNPK(cmUnpkDateTime, &ssta->t.ssta.dt, mBuf);
   switch(elmnt)
   {
      case STSID:
         ret1 = ROK;
         if(eventType == LKW_EVT_STA_CFM)
         {
            cmMemset((U8 *) ptNmb, (U8) 0, LKW_PART_NUM_STR_LEN);
            ssta->t.ssta.s.sysId.ptNmb = ptNmb;
            CMCHKUNPK(cmUnpkSystemId, &ssta->t.ssta.s.sysId,mBuf);
         }
         break;
      case STCKWSAP:
         ret1 = cmUnpkCkwSapSta(&ssta->t.ssta.s.ckwSap, mBuf);
         break;
      case STKWUSAP:
         ret1 = cmUnpkKwuSapSta(&ssta->t.ssta.s.kwuSap, mBuf);
         break;
      case STRGUSAP:
      case STUDXSAP:
         ret1 = cmUnpkRguSapSta(&ssta->t.ssta.s.rguSap, mBuf);
         break;
      default:
         ret1 = RFAILED;
        break;
   }

   if(ret1 != ROK)
   {
      return RFAILED;
   }

   return ROK;
}


/**
 * @brief
     Unpack trace Indication 
 *
 * @param[out] trc   : RLC LM Structure 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwTrc
(
KwMngmt *trc,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkKwTrc(trc, mBuf)
KwMngmt *trc;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkKwTrc);

   CMCHKUNPK(cmUnpkDateTime, &trc->t.trc.dt, mBuf);
   CMCHKUNPK(oduPackUInt16, &trc->t.trc.event, mBuf);

   return ROK;
} /* end of cmUnpkKwTrc */


/**
 * @brief
     Unpack Statistics 
 *
 * @param[out] sts  : RLC LM structure 
 * @param[in] elmnt : element 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwSts
(
KwMngmt *sts,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkKwSts(sts ,elmnt, mBuf)
KwMngmt *sts;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1;           /* return value */
   TRC3(cmUnpkKwSts);

   CMCHKUNPK(cmUnpkDateTime, &sts->t.sts.dt, mBuf);

   switch(elmnt)
   {
      case STGEN:
         ret1 = cmUnpkGenSts(&sts->t.sts.s.gen, mBuf);
         break;
      case STCKWSAP:
         ret1 = cmUnpkCkwSapSts(&sts->t.sts.s.ckwSap, mBuf);
         break;
      case STKWUSAP:
         ret1 = cmUnpkKwuSapSts(&sts->t.sts.s.kwuSap, mBuf);
         break;
/* lkw_c_001.main_2 removed support of RGU sap statistics as it is reflecting
 * in general statstics.
 * */
      default:
         ret1 = RFAILED;
        break;
   } /* end of switch */

   if(ret1 != ROK)
   {
      return RFAILED;
   }

   return ROK;
} /* end of cmUnpkKwSts */


/**
 * @brief
     Unpack General statistics 
 *
 * @param[out] gen  : KwGenSts structure 
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkGenSts
(
KwGenSts *gen,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkGenSts(gen, mBuf)
KwGenSts *gen;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkGenSts);

   CMCHKUNPK(oduPackUInt32, &gen->numUe, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->pdusRecv, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->pdusSent, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->pdusRetx, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->bytesRecv, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->bytesSent, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->unexpPdusRecv, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->errorPdusRecv, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->protTimeOut, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->numOfRb, mBuf);
   CMCHKUNPK(oduPackUInt32, &gen->numSduDisc, mBuf);

   return ROK;
} /* end of cmUnpkGenSts */

/**
 * @brief
     Unpack Ckw Sap Statistics 
 *
 * @param[out] rcSap  : KwCkwCntSts structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkCkwSapSts
(
KwCkwCntSts *rcSap,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkCkwSapSts(rcSap ,mBuf)
KwCkwCntSts *rcSap;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkCkwSapSts);

   CMCHKUNPK(oduPackUInt32, &rcSap->statMsgs, mBuf); 

   return ROK;

} /* end of cmUnpkCkwSapSts */

/**
 * @brief
     Unpack Kwu Sap statistics
 *
 * @param[out]       : KwKwuSapSts structure
 * @param[in] mBuf  : message buffer
 *  @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 cmUnpkKwuSapSts
(
KwKwuSapSts *ruSap,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkKwuSapSts(ruSap, mBuf)
KwKwuSapSts *ruSap;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkKwuSapSts)

   /* lkw_c_001.main_2 changed from suId to spId */
   CMCHKUNPK(cmUnpkSuId, &ruSap->spId, mBuf);
   CMCHKUNPK(oduPackUInt32, &ruSap->sduRx, mBuf);
   CMCHKUNPK(oduPackUInt32, &ruSap->sduTx, mBuf);

   return ROK;

} /* end of cmUnpkKwuSapSts */

/* lkw_c_001.main_2 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 cmPkLkwL2MeasReq
(
Pst * pst,
KwL2MeasReqEvt *measReqEvt
)
#else
PUBLIC S16 cmPkLkwL2MeasReq(pst, measReqEvt)
Pst * pst;
KwL2MeasReqEvt *measReqEvt;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLkwL2MeasReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)measReqEvt, sizeof(KwL2MeasReqEvt));
      return RFAILED;
   }       
   if (cmPkKwL2MeasReqInfo(&measReqEvt->measReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)measReqEvt, sizeof(KwL2MeasReqEvt));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (oduUnpackUInt16(measReqEvt->measPeriod, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)measReqEvt, sizeof(KwL2MeasReqEvt));
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (oduUnpackUInt32(measReqEvt->transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)measReqEvt, sizeof(KwL2MeasReqEvt));
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) LKW_EVT_L2MEAS_REQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkKwL2MeasReqInfo
(
KwL2MeasReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkKwL2MeasReqInfo(param, mBuf)
KwL2MeasReqInfo *param;
Buffer *mBuf;
#endif
{
   U8 idx;
   U16 idx1;

   TRC3(cmPkKwL2MeasReqInfo)

   if((param->measType & LKW_L2MEAS_DL_IP) ||
         (param->measType & LKW_L2MEAS_UL_IP))
   {
      for(idx1 = 0;idx1 < param->val.ipThMeas.numUes;idx1++)
      {

         CMCHKPK(cmPkLteCellId, param->val.ipThMeas.ueInfoLst[idx1].cellId, mBuf);
         CMCHKPK(cmPkLteRnti,   param->val.ipThMeas.ueInfoLst[idx1].ueId, mBuf);
         for(idx = 0; idx < param->val.ipThMeas.ueInfoLst[idx1].numQci; idx++)
         {
            CMCHKPK(oduUnpackUInt8, param->val.ipThMeas.ueInfoLst[idx1].qci[idx], mBuf);
         }
         CMCHKPK(oduUnpackUInt8, param->val.ipThMeas.ueInfoLst[idx1].numQci, mBuf);
      }
      CMCHKPK(oduUnpackUInt16, param->val.ipThMeas.numUes, mBuf);
      CMCHKPK(oduUnpackUInt8, param->measType, mBuf);
      return ROK;
   }
   else
   {
      for(idx = 0; idx < param->val.nonIpThMeas.numQci; idx++)
      {
         CMCHKPK(oduUnpackUInt8, param->val.nonIpThMeas.qci[idx], mBuf);
      }
      CMCHKPK(oduUnpackUInt8, param->val.nonIpThMeas.numQci, mBuf);
      CMCHKPK(oduUnpackUInt16, param->val.nonIpThMeas.numSamples, mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->measType, mBuf);
   return ROK;
}

#ifdef ANSI
PUBLIC S16 cmPkLkwL2MeasStopReq
(
Pst *pst,
U8  measType
)
#else
PUBLIC S16 cmPkLkwL2MeasStopReq(pst, measType)
Pst *pst;
U8 measType;
#endif
{
   Buffer *mBuf = NULLP;

   TRC3(cmPkLkwL2MeasStopReq)
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   CMCHKPK(oduUnpackUInt8, measType, mBuf);
   pst->event = (Event) LKW_EVT_L2MEAS_STOP_REQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkLkwL2MeasSendReq
(
Pst *pst,
U8  measType
)
#else
PUBLIC S16 cmPkLkwL2MeasSendReq(pst, measType)
Pst *pst;
U8 measType;
#endif
{
   Buffer *mBuf = NULLP;

   TRC3(cmPkLkwL2MeasSendReq)
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   CMCHKPK(oduUnpackUInt8, measType, mBuf);
   pst->event = (Event) LKW_EVT_L2MEAS_SEND_REQ;
   RETVALUE(SPstTsk(pst,mBuf));
}
#ifdef TENB_SPLIT_ARCH
#ifdef ANSI
PUBLIC S16 cmUnpkLkwL2MeasReq
(
LkwL2MeasReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLkwL2MeasReq(func, pst, mBuf)
LkwL2MeasReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   KwL2MeasReqEvt measReqEvt;
   
   TRC3(cmUnpkLkwL2MeasReq)

   if (oduPackUInt32(&measReqEvt.transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt16(&measReqEvt.measPeriod, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkKwL2MeasReqInfo(&measReqEvt.measReq, mBuf) != ROK) 
   {
      /*SPutSBuf(pst->region, pst->pool, (Data *)measReqEvt, sizeof(KwL2MeasReqEvt));*/
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
           __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
           (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, &measReqEvt));
}
#else
#ifdef ANSI
PUBLIC S16 cmUnpkLkwL2MeasReq
(
LkwL2MeasReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLkwL2MeasReq(func, pst, mBuf)
LkwL2MeasReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret;
   KwL2MeasReqEvt *measReqEvt;

   TRC3(cmUnpkLkwL2MeasReq)


   if((ret = SGetSBuf(pst->region, pst->pool, (Data **)&measReqEvt,\
               sizeof(KwL2MeasReqEvt))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret);
   }

   if (oduPackUInt32(&measReqEvt->transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (oduPackUInt16(&measReqEvt->measPeriod, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmUnpkKwL2MeasReqInfo(&measReqEvt->measReq, mBuf) != ROK) 
   {
      SPutSBuf(pst->region, pst->pool, (Data *)measReqEvt, sizeof(KwL2MeasReqEvt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, measReqEvt));
}
#endif

#ifdef ANSI
PUBLIC S16 cmUnpkKwL2MeasReqInfo
(
KwL2MeasReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkKwL2MeasReqInfo(param, mBuf)
KwL2MeasReqInfo *param;
Buffer *mBuf;
#endif
{
   U8 idx;
   U16 idx1; 

   TRC3(cmUnpkKwL2MeasReqInfo)

   CMCHKUNPK(oduPackUInt8, &param->measType, mBuf);

   if((param->measType & LKW_L2MEAS_DL_IP) ||
         (param->measType & LKW_L2MEAS_UL_IP))
   {
      CMCHKUNPK(oduPackUInt16, &param->val.ipThMeas.numUes, mBuf);
      for(idx1 = 0;idx1 < param->val.ipThMeas.numUes;idx1++)
      {
         CMCHKUNPK(oduPackUInt8, &param->val.ipThMeas.ueInfoLst[idx1].numQci, mBuf);
         for(idx = param->val.ipThMeas.ueInfoLst[idx1].numQci; idx > 0; idx--)
         {
            CMCHKUNPK(oduPackUInt8, &param->val.ipThMeas.ueInfoLst[idx1].\
                  qci[idx - 1], mBuf);
         }
         CMCHKUNPK(cmUnpkLteRnti, &param->val.ipThMeas.ueInfoLst[idx1].\
               ueId, mBuf);
         CMCHKUNPK(cmUnpkLteCellId, &param->val.ipThMeas.\
               ueInfoLst[idx1].cellId, mBuf);
      }
      return ROK;
   }
   else
   {
      CMCHKUNPK(oduPackUInt16, &param->val.nonIpThMeas.numSamples, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->val.nonIpThMeas.numQci, mBuf);
      for(idx = param->val.nonIpThMeas.numQci; idx > 0; idx--)
      {
         CMCHKUNPK(oduPackUInt8, &param->val.nonIpThMeas.qci[idx - 1], mBuf);
      }
   }
   return ROK;
}

#ifdef ANSI
PUBLIC S16 cmUnpkLkwL2MeasStopReq
(
LkwL2MeasStopReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLkwL2MeasStopReq(func, pst, mBuf)
LkwL2MeasStopReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   U8 measType;
   TRC3(cmUnpkLkwL2MeasReq)

   CMCHKUNPK(oduPackUInt8, &measType, mBuf);
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, measType));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLkwL2MeasSendReq
(
LkwL2MeasSendReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLkwL2MeasSendReq(func, pst, mBuf)
LkwL2MeasSendReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   U8    measType;

   TRC3(cmUnpkLkwL2MeasSendReq)

   CMCHKUNPK(oduPackUInt8, &measType, mBuf); 
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, measType));
}

#ifdef ANSI
PUBLIC S16 cmPkLkwL2MeasCfm
(
Pst * pst,
KwL2MeasCfmEvt *measCfmEvt
)
#else
PUBLIC S16 cmPkLkwL2MeasCfm(pst, measCfmEvt)
Pst * pst;
KwL2MeasCfmEvt *measCfmEvt;
#endif
{
   Buffer *mBuf = NULLP;
   U8 idx;
   U8 idx1;

   TRC3(cmPkLkwL2MeasCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)measCfmEvt, sizeof(KwL2MeasCfmEvt));
      return RFAILED;
   }

   if((measCfmEvt->measType & LKW_L2MEAS_ACT_UE) || \
         (measCfmEvt->measType & LKW_L2MEAS_UU_LOSS) || \
         (measCfmEvt->measType & LKW_L2MEAS_DL_DISC) ||  \
         (measCfmEvt->measType & LKW_L2MEAS_DL_DELAY)) 
   {
      for(idx = 0; idx < measCfmEvt->val.nonIpThMeas.numCfm; idx++)
      {
         if (cmPkKwL2MeasCfmInfo(&measCfmEvt->val.nonIpThMeas.measCfm[idx], \
                  mBuf, measCfmEvt->measType) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
            SPutMsg(mBuf);
            return RFAILED;
         }
      }

      if (oduUnpackUInt16(measCfmEvt->val.nonIpThMeas.numCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else
   {
      for(idx1 =0 ;idx1< measCfmEvt->val.ipThMeas.numUes;idx1++)
      {

         for(idx = 0; idx < measCfmEvt->val.ipThMeas.ueInfoLst[idx1].numCfm; idx++)
         {
            if (cmPkKwL2MeasCfmInfo(&measCfmEvt->val.ipThMeas.ueInfoLst[idx1].\
                     measCfm[idx], mBuf, measCfmEvt->measType) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
               SPutMsg(mBuf);
               return RFAILED;
            }
         }

         if (oduUnpackUInt16(measCfmEvt->val.ipThMeas.ueInfoLst[idx1].numCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
            SPutMsg(mBuf);
            return RFAILED;
         }

         CMCHKPK(cmPkLteCellId, measCfmEvt->val.ipThMeas.ueInfoLst[idx1].cellId, mBuf);
         CMCHKPK(cmPkLteRnti,   measCfmEvt->val.ipThMeas.ueInfoLst[idx1].ueId, mBuf);
      }
      oduUnpackUInt16(measCfmEvt->val.ipThMeas.numUes, mBuf);
   }

   CMCHKPK(oduUnpackUInt8, measCfmEvt->measType, mBuf);
   CMCHKPK(cmPkCmStatus, &measCfmEvt->status, mBuf);
   if (oduUnpackUInt32(measCfmEvt->transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   pst->event = (Event) LKW_EVT_L2MEAS_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkKwL2MeasCfmInfo
(
KwL2MeasCfmInfo *param,
Buffer *mBuf,
U8 measType
)
#else
PUBLIC S16 cmPkKwL2MeasCfmInfo(param, mBuf, measType)
KwL2MeasCfmInfo *param;
Buffer *mBuf;
U8 measType;
#endif
{
   TRC3(cmPkKwL2MeasCfmInfo)

   if((measType & LKW_L2MEAS_DL_IP) ||
         (measType & LKW_L2MEAS_UL_IP))
   {
      CMCHKPK(SPkF32, param->val.ipThrput.ulIpThPut, mBuf);
      CMCHKPK(SPkF32, param->val.ipThrput.dlIpThPut, mBuf);
      CMCHKPK(oduUnpackUInt8, param->qci, mBuf);
      return ROK;
   }
   else  
   {
      CMCHKPK(oduUnpackUInt32, param->val.nonIpThrput.dlDiscRate, mBuf);
      CMCHKPK(oduUnpackUInt32, param->val.nonIpThrput.uuLoss, mBuf);
      CMCHKPK(oduUnpackUInt32, param->val.nonIpThrput.numActUe, mBuf);
      CMCHKPK(oduUnpackUInt32, param->val.nonIpThrput.dlSduDelay, mBuf);
      CMCHKPK(oduUnpackUInt8,  param->qci, mBuf);
   } 
   return ROK;
}

#ifdef ANSI
PUBLIC S16 cmUnpkLkwL2MeasCfm
(
LkwL2MeasCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLkwL2MeasCfm(func, pst, mBuf)
LkwL2MeasCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   U8 idx;
   U16 idx1;
   KwL2MeasCfmEvt measCfmEvt;

   TRC3(cmUnpkLkwL2MeasCfm)

   cmMemset((U8 *)&measCfmEvt, 0 , sizeof(KwL2MeasCfmEvt));

   if (oduPackUInt32(&measCfmEvt.transId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   CMCHKUNPK(cmUnpkCmStatus, &measCfmEvt.status, mBuf);
   CMCHKUNPK(oduPackUInt8, &measCfmEvt.measType, mBuf);  

   if(measCfmEvt.measType & LKW_L2MEAS_DL_IP || measCfmEvt.measType & LKW_L2MEAS_UL_IP)
   { 
      CMCHKUNPK(oduPackUInt16, &measCfmEvt.val.ipThMeas.numUes, mBuf);
      for(idx1 = 0;idx1<measCfmEvt.val.ipThMeas.numUes;idx1++)
      {
         CMCHKUNPK(cmUnpkLteRnti, &measCfmEvt.val.ipThMeas.\
               ueInfoLst[idx1].ueId, mBuf);
         CMCHKUNPK(cmUnpkLteCellId, &measCfmEvt.val.ipThMeas.\
               ueInfoLst[idx1].cellId, mBuf);
         if (oduPackUInt16(&measCfmEvt.val.ipThMeas.ueInfoLst[idx1].\
                  numCfm, mBuf) != ROK) {
            SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
            return RFAILED;
         }
         for(idx = measCfmEvt.val.ipThMeas.ueInfoLst[idx1].numCfm; idx > 0; idx--)
         {
            if (cmUnpkKwL2MeasCfmInfo(&measCfmEvt.val.ipThMeas.ueInfoLst[idx1].\
                     measCfm[idx - 1], mBuf, measCfmEvt.measType) != ROK) {
               SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
               return RFAILED;
            }
         }
      }
   }
   else
   {
      if (oduPackUInt16(&measCfmEvt.val.nonIpThMeas.numCfm, mBuf) != ROK) {
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
         return RFAILED;
      }
      for(idx = measCfmEvt.val.nonIpThMeas.numCfm; idx > 0; idx--)
      {
         if (cmUnpkKwL2MeasCfmInfo(&measCfmEvt.val.nonIpThMeas.measCfm[idx - 1], \
                  mBuf, measCfmEvt.measType) != ROK) {
            SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
            return RFAILED;
         }
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, &measCfmEvt));
}

#ifdef ANSI
PUBLIC S16 cmUnpkKwL2MeasCfmInfo
(
KwL2MeasCfmInfo *param,
Buffer *mBuf,
U8 measType
)
#else
PUBLIC S16 cmUnpkKwL2MeasCfmInfo(param, mBuf, measType)
KwL2MeasCfmInfo *param;
Buffer *mBuf;
U8 measType;
#endif
{
   TRC3(cmUnpkKwL2MeasCfmInfo)

   if((measType & LKW_L2MEAS_DL_IP) ||
         (measType & LKW_L2MEAS_UL_IP))
   {
      CMCHKUNPK(oduPackUInt8,  &param->qci, mBuf);
      CMCHKUNPK(SUnpkF32, &param->val.ipThrput.dlIpThPut, mBuf);
      CMCHKUNPK(SUnpkF32, &param->val.ipThrput.ulIpThPut, mBuf);
   }
   else

   {
      CMCHKUNPK(oduPackUInt8,  &param->qci, mBuf);
      CMCHKUNPK(oduPackUInt32, &param->val.nonIpThrput.dlSduDelay, mBuf);
      CMCHKUNPK(oduPackUInt32, &param->val.nonIpThrput.numActUe, mBuf);
      CMCHKUNPK(oduPackUInt32, &param->val.nonIpThrput.uuLoss, mBuf);
      CMCHKUNPK(oduPackUInt32, &param->val.nonIpThrput.dlDiscRate, mBuf);
   }
   return ROK;
}


#ifdef ANSI
PUBLIC S16 cmPkLkwL2MeasStopCfm
(
Pst * pst,
U8 measType,
U8 status
)
#else
PUBLIC S16 cmPkLkwL2MeasStopCfm(pst, measType,status)
Pst * pst;
U8  measType;
U8  status
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLkwL2MeasStopCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ERRLKW, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   CMCHKPK(oduUnpackUInt8, status, mBuf);
   CMCHKPK(oduUnpackUInt8, measType, mBuf);
   pst->event = (Event) LKW_EVT_L2MEAS_STOP_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLkwL2MeasStopCfm
(
LkwL2MeasStopCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLkwL2MeasStopCfm(func, pst, mBuf)
LkwL2MeasStopCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   U8 measType;
   U8 status;
   TRC3(cmUnpkLkwL2MeasCfm)
   CMCHKUNPK(oduPackUInt8,  &measType, mBuf);
   CMCHKUNPK(oduPackUInt8,  &status, mBuf);
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, measType,status));
}
#endif /* LTE_L2_MEAS */
#endif /* LCLKW */

  
/**********************************************************************
         End of file
 **********************************************************************/
