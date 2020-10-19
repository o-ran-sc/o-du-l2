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


/*********************************************************************
 *             Primitives for LKW interface 
 ********************************************************************/
/* Configuration confirmation primitive */

PRIVATE RlcConfigCfm rlcMiRlcConfigCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   packRlcConfigCfm,            /* 0 - loosely coupled - fc */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiRlcConfigCfm,            /* 1 - tightly coupled layer management*/
#endif /* SM */
};

/* control confirmation primitives */

PRIVATE LkwCntrlCfm kwMiLkwCntrlCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwCntrlCfm,          /* 0 - loosely coupled - fc */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiLkwCntrlCfm,          /* 1 - tightly coupled layer management*/
#endif /* SM */
};

/* Status Indication primitive */

PRIVATE LkwStaInd kwMiLkwStaIndMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwStaInd,            /* 0 - loosely coupled  */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwStaInd,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* Status confirm primitive */

PRIVATE LkwStaCfm kwMiLkwStaCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwStaCfm,            /* 0 - loosely coupled  */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwStaCfm,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* Statistics confirm primitive */

PRIVATE LkwStsCfm kwMiLkwStsCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   cmPkLkwStsCfm,            /* 0 - loosely coupled  */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiLkwStsCfm,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* Trace indication primitive */

PRIVATE LkwTrcInd kwMiLkwTrcIndMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   cmPkLkwTrcInd,            /* 0 - loosely coupled  */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwTrcInd,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
PRIVATE CONSTANT LkwL2MeasCfm rlcMiLkwL2MeasCfmMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasCfm,
#endif
#ifdef SM
   SmMiLkwL2MeasCfm,
#endif
};
PRIVATE CONSTANT LkwL2MeasStopCfm RlcMiLkwL2MeasStopCfmMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasStopCfm,
#endif
#ifdef SM
   SmMiLkwL2MeasStopCfm,
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
uint8_t  measType,
uint8_t  status
)
#else
S16 RlcMiLkwL2MeasStopCfm(pst, measType,status)
Pst *pst;
uint8_t  measType;
uint8_t  status;
#endif
{

   TRC3(RlcMiLkwL2MeasStopCfm)

   (*RlcMiLkwL2MeasStopCfmMt[pst->selector])(pst, measType,status);

   return ROK;

}
#endif /*  LTE_L2_MEAS */

/********************************************************************30**

         End of file
**********************************************************************/
