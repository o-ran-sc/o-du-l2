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

        Name:     RLC Layer Management interface

        Type:     C source file

        Desc:     This file contains source code for RLC layer management
                  interface primitives. It includes the definition of the
                  following functions and their portable function
                  definitions.
                     -- PjMiLpjCfgCfm
                     -- PjMiLpjCntrlCfm
                     -- PjMiLpjStaInd


        File:     kw_ptmi.c

*********************************************************************21*/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */

#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"

/* local defines */
#define MAXKWMI 2               /* max. layer management interfaces */

#ifndef LCKWMILKW
#define PTKWLKW
#endif


#ifndef SM
#define PTKWLKW
#endif



#ifdef PTKWLKW
/* portable functions */

PRIVATE S16 PtMiRlcConfigCfm    ARGS((Pst *pst, RlcMngmt *cfm));
PRIVATE S16 PtMiLkwCntrlCfm  ARGS((Pst *pst, RlcMngmt *cfm));
PRIVATE S16 PtMiLkwStaInd    ARGS((Pst *pst, RlcMngmt *usta));

PRIVATE S16 PtMiLkwStaCfm    ARGS((Pst *pst, RlcMngmt *cfm));
PRIVATE S16 PtMiLkwStsCfm    ARGS((Pst *pst, Action action,
                                RlcMngmt *cfm));
PRIVATE S16 PtMiLkwTrcInd    ARGS((Pst *pst, RlcMngmt *trc,
                                Buffer *mBuf));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
PRIVATE S16 PtMiLkwL2MeasCfm   ARGS((Pst *pst, RlcL2MeasCfmEvt *measEvt));
PRIVATE S16 PtMiLkwL2MeasStopCfm   ARGS((Pst *pst, U8 measType,U8 status));
#endif /*  LTE_L2_MEAS */
#endif /* PTKWLKW */


/*********************************************************************
 *             Primitives for LKW interface 
 ********************************************************************/
/* Configuration confirmation primitive */

PRIVATE RlcConfigCfm rlcMiRlcConfigCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   packRlcConfigCfm,            /* 0 - loosely coupled - fc */
#else
   PtMiRlcConfigCfm,            /* 0 - tightly coupled portable */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiRlcConfigCfm,            /* 1 - tightly coupled layer management*/
#else
   PtMiRlcConfigCfm,            /* 1 - tightly coupled portable */
#endif /* SM */
};

/* control confirmation primitives */

PRIVATE LkwCntrlCfm kwMiLkwCntrlCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwCntrlCfm,          /* 0 - loosely coupled - fc */
#else
   PtMiLkwCntrlCfm,          /* 0 - tightly coupled portable */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiLkwCntrlCfm,          /* 1 - tightly coupled layer management*/
#else
   PtMiLkwCntrlCfm,          /* 1 - tightly coupled portable */
#endif /* SM */
};

/* Status Indication primitive */

PRIVATE LkwStaInd kwMiLkwStaIndMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwStaInd,            /* 0 - loosely coupled  */
#else
   PtMiLkwStaInd,            /* 0 - tightly coupled, portable */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwStaInd,            /* 1 - tightly coupled, layer management */
#else
   PtMiLkwStaInd,            /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* Status confirm primitive */

PRIVATE LkwStaCfm kwMiLkwStaCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwStaCfm,            /* 0 - loosely coupled  */
#else
   PtMiLkwStaCfm,            /* 0 - tightly coupled, portable */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwStaCfm,            /* 1 - tightly coupled, layer management */
#else
   PtMiLkwStaCfm,            /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* Statistics confirm primitive */

PRIVATE LkwStsCfm kwMiLkwStsCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   cmPkLkwStsCfm,            /* 0 - loosely coupled  */
#else
   PtMiLkwStsCfm,            /* 0 - tightly coupled, portable */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiLkwStsCfm,            /* 1 - tightly coupled, layer management */
#else
   PtMiLkwStsCfm,            /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* Trace indication primitive */

PRIVATE LkwTrcInd kwMiLkwTrcIndMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   cmPkLkwTrcInd,            /* 0 - loosely coupled  */
#else
   PtMiLkwTrcInd,            /* 0 - tightly coupled, portable */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwTrcInd,            /* 1 - tightly coupled, layer management */
#else
   PtMiLkwTrcInd,            /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
PRIVATE CONSTANT LkwL2MeasCfm rlcMiLkwL2MeasCfmMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasCfm,
#else
   PtMiLkwL2MeasCfm,
#endif
#ifdef SM
   SmMiLkwL2MeasCfm,
#else
   PtMiLkwL2MeasCfm,
#endif
};
PRIVATE CONSTANT LkwL2MeasStopCfm RlcMiLkwL2MeasStopCfmMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasStopCfm,
#else
   PtMiLkwL2MeasStopCfm,
#endif
#ifdef SM
   SmMiLkwL2MeasStopCfm,
#else
   PtMiLkwL2MeasStopCfm,
#endif
};
#endif /*  LTE_L2_MEAS */
  
/****************************************************************************
 *                         LKW Interface Mt functions
 ***************************************************************************/
/**
   @brief
   This function is called by the RlcMiRlcConfigReq function for responding
   to configuration requests.The cfm field in the RlcMngmt  structure contains
 the response value.

   - This function calls the mapping matrix for sending the configuration
     confirmation.
   - The actual function called depends on the coupling at the LKW interface.
   - For a loosely coupled interface, a common packing function is called.
   - The packing function packs the parameter in a message buffer and posts
     the message to the target task.
   - For a tightly coupled interface, the actual function called depends on
     the layer manager API provided.

*/
#ifdef ANSI
S16 RlcMiRlcConfigCfm
(
Pst        *pst,                /* post structure */
RlcMngmt    *cfm                 /* Layer Management structure */
)
#else
S16 RlcMiRlcConfigCfm(pst, cfm)
Pst        *pst;                /* post structure */
RlcMngmt    *cfm;                /* Layer Management structure */
#endif
{
   TRC3(RlcMiRlcConfigCfm);

   /* jump to specific primitive depending on configured selector */
   (*rlcMiRlcConfigCfmMt[pst->selector])(pst, cfm);
   
   return ROK;
}


/**
   @brief
   This function is called by the RlcMiLkwCntrlReq function to send a control confirm to the layer management module.

   - This function calls the mapping matrix for sending the control confirmation.
   - Actual function called depends on the coupling of the LKW interface.
   - For a loosely coupled interface, a common packing function is called.
   - The packing function packs the parameter in a message buffer and posts the
     message to the target task.
   - For a tightly coupled interface, the actual function called depends on the
     layer manager API provided.

*/
#ifdef ANSI
S16 RlcMiLkwCntrlCfm
(
Pst *pst,                    /* post structure */
RlcMngmt *cfm                 /* configure */
)
#else
S16 RlcMiLkwCntrlCfm(pst, cfm)
Pst *pst;                    /* post structure */
RlcMngmt *cfm;                /* confirm */
#endif
{
   TRC3(RlcMiLkwCntrlCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwCntrlCfmMt[pst->selector])(pst, cfm);

   return ROK;

} /* end of RlcMiLkwCntrlCfm */

/**
   @brief
   Description:
   - This function can be used by RLC to send unsolicited status information
     to the layer manager, when the unsolicited status flag is enabled by the
     layer manager through a previous control request.

   - This function calls the mapping matrix for sending the unsolicited status
     indication.The actual function called depends on the coupling of the
     LKW interface.

   - For a loosely coupled interface, a common packing function is called. The
     packing function packs the parameter in a message buffer and posts the
     message to the target task.

   - For a tightly coupled interface, the actual function called depends on
     the layer manager API provided.
*/
#ifdef ANSI
S16 RlcMiLkwStaInd
(
Pst     *pst,                /* post structure */
RlcMngmt *usta                /* unsolicited status */
)
#else
S16 RlcMiLkwStaInd(pst, usta)
Pst     *pst;                /* post structure */
RlcMngmt *usta;               /* unsolicited status */
#endif
{
   TRC3(RlcMiLkwStaInd);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStaIndMt[pst->selector])(pst, usta);

   return (ROK);
} /* end of RlcMiLkwStaInd */


/**
   @brief
   - This function is called by the RlcMiLkwStaReq function to send
      the requested status information to the layer manager.

   - This function calls the mapping matrix for sending the status
      confirmation. The actual function called depends on the coupling
      of the LKW interface.

   - For a loosely coupled interface, a common packing function is called.
      The packing function packs the parameter in a message buffer and
      posts the message to the target task.

   - For a tightly coupled interface, the actual function called depends
      on the layer manager API provided.

*/
#ifdef ANSI
S16 RlcMiLkwStaCfm
(
Pst *pst,                    /* post structure */
RlcMngmt *cfm                 /* solicited status confirmation */
)
#else
S16 RlcMiLkwStaCfm(pst, cfm)
Pst *pst;                    /* post structure */
RlcMngmt *cfm;                /* solicited status confirmation */
#endif
{
   TRC3(RlcMiLkwStaCfm);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStaCfmMt[pst->selector])(pst, cfm);

   return ROK;

} /* end of RlcMiLkwStaCfm */


/**
   @brief
   - This function is called by the RlcMiLkwStsReq function for responding
      to statistics requests.

   - This function calls the mapping matrix for sending the statistics
      confirmation. The actual function called depends on the coupling
      of the LKW interface.

   - For a loosely coupled interface, a common packing function is called.
      The packing function packs the parameter in a message buffer and
      posts the message to the target task.

   - For a tightly coupled interface, the actual function called depends
      on the layer manager API provided.

*/
#ifdef ANSI
S16 RlcMiLkwStsCfm
(
Pst *pst,                    /* post structure */
Action action,               /* action */
RlcMngmt *cfm                 /* statistics confirmation */
)
#else
S16 RlcMiLkwStsCfm(pst, action, cfm)
Pst *pst;                    /* post structure */
Action action;               /* action */
RlcMngmt *cfm;                /* statistics confirmation */
#endif
{
   TRC3(RlcMiLkwStsCfm);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStsCfmMt[pst->selector])(pst, action, cfm);

   return ROK;

} /* end of RlcMiLkwStsCfm */

/**
   @brief
   - This function can be used by RLC module to send unsolicited trace
      indications to the layer manager, when tracing is enabled by the
      layer manager through a previous control request.

   - This function calls the mapping matrix for sending the trace indication.
      The actual function called depends on the coupling of the LKW interface.

   - For a loosely coupled interface, a common packing function is called.
      The packing function packs the parameter in a message buffer and posts
      the message to the target task.

   - For a tightly coupled interface, the actual function called depends on
      the layer manager API provided.

*/
#ifdef ANSI
S16 RlcMiLkwTrcInd
(
Pst *pst,                    /* post structure */
RlcMngmt *trc,                /* trace indication */
Buffer *mBuf                 /* message buffer */
)
#else
S16 RlcMiLkwTrcInd(pst, trc, mBuf)
Pst *pst;                    /* post structure */
RlcMngmt *trc;                /* trace indication */
Buffer *mBuf;                /* message buffer */
#endif
{
   TRC3(RlcMiLkwTrcInd);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwTrcIndMt[pst->selector])(pst, trc, mBuf);

   return ROK;

} /* end of RlcMiLkwTrcInd */


/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
S16 RlcMiLkwL2MeasCfm
(
Pst * pst,
RlcL2MeasCfmEvt *measEvt
)
#else
S16 RlcMiLkwL2MeasCfm(pst, measEvt)
Pst * pst;
RlcL2MeasCfmEvt *measEvt;
#endif
{

   TRC3(RlcMiLkwL2MeasCfm)

   (*rlcMiLkwL2MeasCfmMt[pst->selector])(pst, measEvt);

   return ROK;

}
#ifdef ANSI
S16 RlcMiLkwL2MeasStopCfm
(  
Pst *pst,
U8  measType,
U8  status
)
#else
S16 RlcMiLkwL2MeasStopCfm(pst, measType,status)
Pst *pst;
U8  measType;
U8  status;
#endif
{

   TRC3(RlcMiLkwL2MeasStopCfm)

   (*RlcMiLkwL2MeasStopCfmMt[pst->selector])(pst, measType,status);

   return ROK;

}
#endif /*  LTE_L2_MEAS */
#ifdef PTKWLKW


/*************************************************************************
 *                         Porting Functions
 ************************************************************************/
/*
 *
 *       Fun:   configuration  Confirm
 *
 *       Desc:  This function is used to confirm the receipt of configuration
 *              request from layer management.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  kw_ptmi.c
 *
 */

#ifdef ANSI
S16 PtMiRlcConfigCfm
(
Pst *pst,                    /* post structure */
RlcMngmt *cfm                 /* Layer Management structure */
)
#else
S16 PtMiRlcConfigCfm(pst, cfm)
Pst *pst;                    /* post structure */
RlcMngmt *cfm;                /* Layer Management structure */
#endif
{
   TRC3(PtMiRlcConfigCfm)

   UNUSED(pst);
   UNUSED(cfm);

   TRC2(PtMiRlcConfigCfm() : function is not implemented)

   return ROK;
} /* end of PtMiRlcConfigCfm */

/*
 *
 *       Fun:   Control Confirmation
 *
 *       Desc:  This function is the portable version of used to
 *              confirm the receipt of configuration request from
 *              layer management.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  kw_ptmi.c
 *
 */

#ifdef ANSI
PRIVATE S16 PtMiLkwCntrlCfm
(
Pst *pst,                 /* Post structure */
RlcMngmt *cfm              /* Layer Management structure */
)
#else
PRIVATE S16 PtMiLkwCntrlCfm(pst, cfm)
Pst *pst;                 /* Post structure */
RlcMngmt *cfm;             /* Layer Management structure */
#endif
{
   TRC3(PtMiLkwCntrlCfm);

   UNUSED(pst);
   UNUSED(cfm);

   TRC2(PtMiLkwCntrlCfm() :  function is not implemented)

   return ROK;
} /* end of PtMiLkwCntrlCfm */


/*
 *
 *       Fun:   unsolicited status indication
 *
 *       Desc:  This function is the portable version  used to
 *              send the status indication to the layer manager
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  kw_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLkwStaInd
(
Pst *pst,                    /* post structure */
RlcMngmt *usta                /* unsolicited status */
)
#else
PRIVATE S16 PtMiLkwStaInd(pst, usta)
Pst *pst;                    /* post structure */
RlcMngmt *usta;               /* unsolicited status */
#endif
{
   TRC3(PtMiLkwStaInd)

   UNUSED(pst);
   UNUSED(usta);

   TRC2(PtMiLkwStaInd() : function is not implemented)

   return ROK;
} /* end of PtMiLkwStaInd */


/*
 *
 *       Fun:   portable function for solicited status confirmation
 *
 *       Desc:  This function is the portable version  used to
 *              send the status confirmation to the layer manager
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  kw_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLkwStaCfm
(
Pst *pst,                    /* post structure */
RlcMngmt *cfm                 /* solicited status confirmation */
)
#else
PRIVATE S16 PtMiLkwStaCfm(pst, cfm)
Pst *pst;                    /* post structure */
RlcMngmt *cfm;                /* solicited status confirmation */
#endif
{
   TRC3(PtMiLkwStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   return ROK;
} /* end of PtMiLkwStaCfm */


/*
 *
 *       Fun:   portable function for statistics confirmation
 *
 *       Desc:  This function is the portable version  used to
 *              send the statistics confirmation to the layer manager
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  kw_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLkwStsCfm
(
Pst *pst,                    /* post structure */
Action action,               /* action */
RlcMngmt *cfm                 /* statistics confirmation */
)
#else
PRIVATE S16 PtMiLkwStsCfm(pst, action, cfm)
Pst *pst;                    /* post structure */
Action action;               /* action */
RlcMngmt *cfm;                /* statistics confirmation */
#endif
{
   TRC3(PtMiLkwStsCfm)

   UNUSED(pst);
   UNUSED(action);
   UNUSED(cfm);

   return ROK;
} /* end of PtMiLkwStsCfm */


/*
 *
 *       Fun:   portable function for trace indication
 *
 *       Desc:  This function is the portable version  used to
 *              send trace indication to the layer manager
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  kw_ptmi.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtMiLkwTrcInd
(
Pst *pst,                    /* post structure */
RlcMngmt *trc,                /* trace indication */
Buffer *mBuf                 /* message buffer */
)
#else
PRIVATE S16 PtMiLkwTrcInd(pst, trc, mBuf)
Pst *pst;                    /* post structure */
RlcMngmt *trc;                /* trace indication */
Buffer *mBuf;                /* message buffer */
#endif
{
   TRC3(PtMiLkwTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(mBuf);

   return ROK;
} /* end of PtMiLkwTrcInd */

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PRIVATE S16 PtMiLkwL2MeasCfm
(
Pst * pst,
RlcL2MeasCfmEvt * measEvt
)
#else
PRIVATE S16 PtMiLkwL2MeasCfm(pst, measEvt)
Pst * pst;
RlcL2MeasCfmEvt * measEvt;
#endif
{

   TRC3(PtMiLkwL2MeasCfm)

   UNUSED(pst);
   UNUSED(measEvt);

   return ROK;

}
#ifdef ANSI
PRIVATE S16 PtMiLkwL2MeasStopCfm
(
Pst * pst,
U8 measType,
U8 status
)
#else
PRIVATE S16 PtMiLkwL2MeasStopCfm(pst, measType,status)
Pst * pst;
U8  measType;
U8  status;
#endif
{

   TRC3(PtMiLkwL2MeasStopCfm)

   UNUSED(pst);
   UNUSED(measType);
   UNUSED(status);

   return ROK;

}
#endif /*  LTE_L2_MEAS */
#endif /* PTKWLKW */


/********************************************************************30**

         End of file
**********************************************************************/
