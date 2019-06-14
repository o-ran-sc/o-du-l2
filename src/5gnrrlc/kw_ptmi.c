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
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */

#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
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

PRIVATE S16 PtMiLkwCfgCfm    ARGS((Pst *pst, KwMngmt *cfm));
PRIVATE S16 PtMiLkwCntrlCfm  ARGS((Pst *pst, KwMngmt *cfm));
PRIVATE S16 PtMiLkwStaInd    ARGS((Pst *pst, KwMngmt *usta));

PRIVATE S16 PtMiLkwStaCfm    ARGS((Pst *pst, KwMngmt *cfm));
PRIVATE S16 PtMiLkwStsCfm    ARGS((Pst *pst, Action action,
                                KwMngmt *cfm));
PRIVATE S16 PtMiLkwTrcInd    ARGS((Pst *pst, KwMngmt *trc,
                                Buffer *mBuf));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
PRIVATE S16 PtMiLkwL2MeasCfm   ARGS((Pst *pst, KwL2MeasCfmEvt *measEvt));
PRIVATE S16 PtMiLkwL2MeasStopCfm   ARGS((Pst *pst, U8 measType,U8 status));
#endif /*  LTE_L2_MEAS */
#endif /* PTKWLKW */


/*********************************************************************
 *             Primitives for LKW interface 
 ********************************************************************/
/* Configuration confirmation primitive */

PRIVATE LkwCfgCfm kwMiLkwCfgCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   cmPkLkwCfgCfm,            /* 0 - loosely coupled - fc */
#else
   PtMiLkwCfgCfm,            /* 0 - tightly coupled portable */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiLkwCfgCfm,            /* 1 - tightly coupled layer management*/
#else
   PtMiLkwCfgCfm,            /* 1 - tightly coupled portable */
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
PRIVATE CONSTANT LkwL2MeasCfm KwMiLkwL2MeasCfmMt[] =
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
PRIVATE CONSTANT LkwL2MeasStopCfm KwMiLkwL2MeasStopCfmMt[] =
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
   This function is called by the KwMiLkwCfgReq function for responding
   to configuration requests.The cfm field in the KwMngmt  structure contains
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
PUBLIC S16 KwMiLkwCfgCfm
(
Pst        *pst,                /* post structure */
KwMngmt    *cfm                 /* Layer Management structure */
)
#else
PUBLIC S16 KwMiLkwCfgCfm(pst, cfm)
Pst        *pst;                /* post structure */
KwMngmt    *cfm;                /* Layer Management structure */
#endif
{
   TRC3(KwMiLkwCfgCfm);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwCfgCfmMt[pst->selector])(pst, cfm);
   
   RETVALUE(ROK);
}


/**
   @brief
   This function is called by the KwMiLkwCntrlReq function to send a control confirm to the layer management module.

   - This function calls the mapping matrix for sending the control confirmation.
   - Actual function called depends on the coupling of the LKW interface.
   - For a loosely coupled interface, a common packing function is called.
   - The packing function packs the parameter in a message buffer and posts the
     message to the target task.
   - For a tightly coupled interface, the actual function called depends on the
     layer manager API provided.

*/
#ifdef ANSI
PUBLIC S16 KwMiLkwCntrlCfm
(
Pst *pst,                    /* post structure */
KwMngmt *cfm                 /* configure */
)
#else
PUBLIC S16 KwMiLkwCntrlCfm(pst, cfm)
Pst *pst;                    /* post structure */
KwMngmt *cfm;                /* confirm */
#endif
{
   TRC3(KwMiLkwCntrlCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwCntrlCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

} /* end of KwMiLkwCntrlCfm */

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
PUBLIC S16 KwMiLkwStaInd
(
Pst     *pst,                /* post structure */
KwMngmt *usta                /* unsolicited status */
)
#else
PUBLIC S16 KwMiLkwStaInd(pst, usta)
Pst     *pst;                /* post structure */
KwMngmt *usta;               /* unsolicited status */
#endif
{
   TRC3(KwMiLkwStaInd);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStaIndMt[pst->selector])(pst, usta);

   RETVALUE(ROK);
} /* end of KwMiLkwStaInd */


/**
   @brief
   - This function is called by the KwMiLkwStaReq function to send
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
PUBLIC S16 KwMiLkwStaCfm
(
Pst *pst,                    /* post structure */
KwMngmt *cfm                 /* solicited status confirmation */
)
#else
PUBLIC S16 KwMiLkwStaCfm(pst, cfm)
Pst *pst;                    /* post structure */
KwMngmt *cfm;                /* solicited status confirmation */
#endif
{
   TRC3(KwMiLkwStaCfm);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStaCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

} /* end of KwMiLkwStaCfm */


/**
   @brief
   - This function is called by the KwMiLkwStsReq function for responding
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
PUBLIC S16 KwMiLkwStsCfm
(
Pst *pst,                    /* post structure */
Action action,               /* action */
KwMngmt *cfm                 /* statistics confirmation */
)
#else
PUBLIC S16 KwMiLkwStsCfm(pst, action, cfm)
Pst *pst;                    /* post structure */
Action action;               /* action */
KwMngmt *cfm;                /* statistics confirmation */
#endif
{
   TRC3(KwMiLkwStsCfm);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStsCfmMt[pst->selector])(pst, action, cfm);

   RETVALUE(ROK);

} /* end of KwMiLkwStsCfm */

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
PUBLIC S16 KwMiLkwTrcInd
(
Pst *pst,                    /* post structure */
KwMngmt *trc,                /* trace indication */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 KwMiLkwTrcInd(pst, trc, mBuf)
Pst *pst;                    /* post structure */
KwMngmt *trc;                /* trace indication */
Buffer *mBuf;                /* message buffer */
#endif
{
   TRC3(KwMiLkwTrcInd);

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwTrcIndMt[pst->selector])(pst, trc, mBuf);

   RETVALUE(ROK);

} /* end of KwMiLkwTrcInd */


/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 KwMiLkwL2MeasCfm
(
Pst * pst,
KwL2MeasCfmEvt *measEvt
)
#else
PUBLIC S16 KwMiLkwL2MeasCfm(pst, measEvt)
Pst * pst;
KwL2MeasCfmEvt *measEvt;
#endif
{

   TRC3(KwMiLkwL2MeasCfm)

   (*KwMiLkwL2MeasCfmMt[pst->selector])(pst, measEvt);

   RETVALUE(ROK);

}
#ifdef ANSI
PUBLIC S16 KwMiLkwL2MeasStopCfm
(  
Pst *pst,
U8  measType,
U8  status
)
#else
PUBLIC S16 KwMiLkwL2MeasStopCfm(pst, measType,status)
Pst *pst;
U8  measType;
U8  status;
#endif
{

   TRC3(KwMiLkwL2MeasStopCfm)

   (*KwMiLkwL2MeasStopCfmMt[pst->selector])(pst, measType,status);

   RETVALUE(ROK);

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
PUBLIC S16 PtMiLkwCfgCfm
(
Pst *pst,                    /* post structure */
KwMngmt *cfm                 /* Layer Management structure */
)
#else
PUBLIC S16 PtMiLkwCfgCfm(pst, cfm)
Pst *pst;                    /* post structure */
KwMngmt *cfm;                /* Layer Management structure */
#endif
{
   TRC3(PtMiLkwCfgCfm)

   UNUSED(pst);
   UNUSED(cfm);

   TRC2(PtMiLkwCfgCfm() : function is not implemented)

   RETVALUE(ROK);
} /* end of PtMiLkwCfgCfm */

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
KwMngmt *cfm              /* Layer Management structure */
)
#else
PRIVATE S16 PtMiLkwCntrlCfm(pst, cfm)
Pst *pst;                 /* Post structure */
KwMngmt *cfm;             /* Layer Management structure */
#endif
{
   TRC3(PtMiLkwCntrlCfm);

   UNUSED(pst);
   UNUSED(cfm);

   TRC2(PtMiLkwCntrlCfm() :  function is not implemented)

   RETVALUE(ROK);
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
KwMngmt *usta                /* unsolicited status */
)
#else
PRIVATE S16 PtMiLkwStaInd(pst, usta)
Pst *pst;                    /* post structure */
KwMngmt *usta;               /* unsolicited status */
#endif
{
   TRC3(PtMiLkwStaInd)

   UNUSED(pst);
   UNUSED(usta);

   TRC2(PtMiLkwStaInd() : function is not implemented)

   RETVALUE(ROK);
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
KwMngmt *cfm                 /* solicited status confirmation */
)
#else
PRIVATE S16 PtMiLkwStaCfm(pst, cfm)
Pst *pst;                    /* post structure */
KwMngmt *cfm;                /* solicited status confirmation */
#endif
{
   TRC3(PtMiLkwStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
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
KwMngmt *cfm                 /* statistics confirmation */
)
#else
PRIVATE S16 PtMiLkwStsCfm(pst, action, cfm)
Pst *pst;                    /* post structure */
Action action;               /* action */
KwMngmt *cfm;                /* statistics confirmation */
#endif
{
   TRC3(PtMiLkwStsCfm)

   UNUSED(pst);
   UNUSED(action);
   UNUSED(cfm);

   RETVALUE(ROK);
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
KwMngmt *trc,                /* trace indication */
Buffer *mBuf                 /* message buffer */
)
#else
PRIVATE S16 PtMiLkwTrcInd(pst, trc, mBuf)
Pst *pst;                    /* post structure */
KwMngmt *trc;                /* trace indication */
Buffer *mBuf;                /* message buffer */
#endif
{
   TRC3(PtMiLkwTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(mBuf);

   RETVALUE(ROK);
} /* end of PtMiLkwTrcInd */

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PRIVATE S16 PtMiLkwL2MeasCfm
(
Pst * pst,
KwL2MeasCfmEvt * measEvt
)
#else
PRIVATE S16 PtMiLkwL2MeasCfm(pst, measEvt)
Pst * pst;
KwL2MeasCfmEvt * measEvt;
#endif
{

   TRC3(PtMiLkwL2MeasCfm)

   UNUSED(pst);
   UNUSED(measEvt);

   RETVALUE(ROK);

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

   RETVALUE(ROK);

}
#endif /*  LTE_L2_MEAS */
#endif /* PTKWLKW */


/********************************************************************30**

         End of file
**********************************************************************/
