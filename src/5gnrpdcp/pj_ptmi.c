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

        Name:     PDCP Layer Management interface

        Type:     C source file

        Desc:     This file contains source code for RLC layer management
                  interface primitives. It includes the definition of the
                  following functions and their portable function
                  definitions.
                     -- PjMiLpjCfgCfm
                     -- PjMiLpjCntrlCfm
                     -- PjMiLpjStaInd


        File:     pj_ptmi.c

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
#include "lpj.h"           /* LPJ defines */


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
#include "lpj.x"           /* LPJ */


/* local defines */
#ifndef LCPJMILPJ
#define PTPJLPJ
#endif

#ifndef SM
#define PTPJLPJ
#endif


#ifdef PTPJLPJ

EXTERN S16 PtMiLpjCfgCfm ARGS((Pst * pst, PjMngmt * cfgCfm));

EXTERN S16 PtMiLpjCntrlCfm ARGS((Pst * pst, PjMngmt * cntlCfm));

EXTERN S16 PtMiLpjStaInd ARGS((Pst * pst, PjMngmt * staInd));

EXTERN S16 PtMiLpjStaCfm ARGS((Pst * pst, PjMngmt * staCfm));

EXTERN S16 PtMiLpjStsCfm ARGS((Pst * pst, Action actn, PjMngmt * stsCfm));

EXTERN S16 PtMiLpjTrcInd ARGS((Pst * pst, PjMngmt * trc, Buffer * mBuf));

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 PtMiLpjL2MeasCfm   ARGS((Pst *pst, PjL2MeasCfmEvt *measEvt));
PRIVATE S16 PtMiLpjL2MeasStopCfm   ARGS((Pst *pst, U16 measType,U8 status));
#endif /*  LTE_L2_MEAS */

#endif /*--#ifdef PTPJLPJ--*/

/*********************************************************************
 *             Primitives for LPJ interface 
 ********************************************************************/
PRIVATE CONSTANT LpjCfgCfm PjMiLpjCfgCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjCfgCfm,
#else
   PtMiLpjCfgCfm,
#endif
#ifdef SM
   SmMiLpjCfgCfm,
#else
   PtMiLpjCfgCfm,
#endif
};


PRIVATE CONSTANT LpjCntrlCfm PjMiLpjCntrlCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjCntrlCfm,
#else
   PtMiLpjCntrlCfm,
#endif
#ifdef SM
   SmMiLpjCntrlCfm,
#else
   PtMiLpjCntrlCfm,
#endif
};


PRIVATE CONSTANT LpjStaInd PjMiLpjStaIndMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjStaInd,
#else
   PtMiLpjStaInd,
#endif
#ifdef SM
   SmMiLpjStaInd,
#else
   PtMiLpjStaInd,
#endif
};


PRIVATE CONSTANT LpjStaCfm PjMiLpjStaCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjStaCfm,
#else
   PtMiLpjStaCfm,
#endif
#ifdef SM
   SmMiLpjStaCfm,
#else
   PtMiLpjStaCfm,
#endif
};


PRIVATE CONSTANT LpjStsCfm PjMiLpjStsCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjStsCfm,
#else
   PtMiLpjStsCfm,
#endif
#ifdef SM
   SmMiLpjStsCfm,
#else
   PtMiLpjStsCfm,
#endif
};


PRIVATE CONSTANT LpjTrcInd PjMiLpjTrcIndMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjTrcInd,
#else
   PtMiLpjTrcInd,
#endif
#ifdef SM
   SmMiLpjTrcInd,
#else
   PtMiLpjTrcInd,
#endif
};

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
PRIVATE CONSTANT LpjL2MeasCfm PjMiLpjL2MeasCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjL2MeasCfm,
#else
   PtMiLpjL2MeasCfm,
#endif
#ifdef SM
   SmMiLpjL2MeasCfm,
#else
   PtMiLpjL2MeasCfm,
#endif
};
PRIVATE CONSTANT LpjL2MeasStopCfm PjMiLpjL2MeasStopCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjL2MeasStopCfm,
#else
   PtMiLpjL2MeasStopCfm,
#endif
#ifdef SM
   SmMiLpjL2MeasStopCfm,
#else
   PtMiLpjL2MeasStopCfm,
#endif
};
#endif /*  LTE_L2_MEAS */

/****************************************************************************
 *                         LPJ Interface Mt functions
 ***************************************************************************/
#ifdef ANSI
PUBLIC S16 PjMiLpjCfgCfm
(
Pst * pst,
PjMngmt * cfgCfm
)
#else
PUBLIC S16 PjMiLpjCfgCfm(pst, cfgCfm)
Pst * pst;
PjMngmt * cfgCfm;
#endif
{

   TRC3(PjMiLpjCfgCfm)

   (*PjMiLpjCfgCfmMt[pst->selector])(pst, cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjCntrlCfm
(
Pst * pst,
PjMngmt * cntlCfm
)
#else
PUBLIC S16 PjMiLpjCntrlCfm(pst, cntlCfm)
Pst * pst;
PjMngmt * cntlCfm;
#endif
{

   TRC3(PjMiLpjCntrlCfm)

   (*PjMiLpjCntrlCfmMt[pst->selector])(pst, cntlCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjStaInd
(
Pst * pst,
PjMngmt * staInd
)
#else
PUBLIC S16 PjMiLpjStaInd(pst, staInd)
Pst * pst;
PjMngmt * staInd;
#endif
{

   TRC3(PjMiLpjStaInd)

   (*PjMiLpjStaIndMt[pst->selector])(pst, staInd);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjStaCfm
(
Pst * pst,
PjMngmt * staCfm
)
#else
PUBLIC S16 PjMiLpjStaCfm(pst, staCfm)
Pst * pst;
PjMngmt * staCfm;
#endif
{

   TRC3(PjMiLpjStaCfm)

   (*PjMiLpjStaCfmMt[pst->selector])(pst, staCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjStsCfm
(
Pst * pst,
Action actn,
PjMngmt * stsCfm
)
#else
PUBLIC S16 PjMiLpjStsCfm(pst, actn, stsCfm)
Pst * pst;
Action actn;
PjMngmt * stsCfm;
#endif
{

   TRC3(PjMiLpjStsCfm)

   (*PjMiLpjStsCfmMt[pst->selector])(pst, actn, stsCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjTrcInd
(
Pst * pst,
PjMngmt * trc,
Buffer * mBuf
)
#else
PUBLIC S16 PjMiLpjTrcInd(pst, trc, mBuf)
Pst * pst;
PjMngmt * trc;
Buffer * mBuf;
#endif
{

   TRC3(PjMiLpjTrcInd)

   (*PjMiLpjTrcIndMt[pst->selector])(pst, trc, mBuf);

   RETVALUE(ROK);

}

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasCfm
(
Pst * pst,
PjL2MeasCfmEvt *measEvt
)
#else
PUBLIC S16 PjMiLpjL2MeasCfm(pst, trc, mBuf)
Pst * pst;
PjL2MeasCfmEvt *measEvt;
#endif
{

   TRC3(PjMiLpjL2MeasCfm)

   (*PjMiLpjL2MeasCfmMt[pst->selector])(pst, measEvt);

   RETVALUE(ROK);

}
#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasStopCfm
(  
Pst *pst,
U16  measType,
U8  status
)
#else
PUBLIC S16 PjMiLpjL2MeasStopCfm(pst, measType,status)
Pst *pst;
U16  measType;
U8  status;
#endif
{

   TRC3(PjMiLpjL2MeasStopCfm)

   (*PjMiLpjL2MeasStopCfmMt[pst->selector])(pst, measType,status);

   RETVALUE(ROK);

}
#endif /*  LTE_L2_MEAS */


#ifdef PTPJLPJ


#ifdef ANSI
PUBLIC S16 PtMiLpjCfgCfm
(
Pst * pst,
PjMngmt * cfgCfm
)
#else
PUBLIC S16 PtMiLpjCfgCfm(pst, cfgCfm)
Pst * pst;
PjMngmt * cfgCfm;
#endif
{

   TRC3(PtMiLpjCfgCfm)

   UNUSED(pst);
   UNUSED(cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjCntrlCfm
(
Pst * pst,
PjMngmt * cntlCfm
)
#else
PUBLIC S16 PtMiLpjCntrlCfm(pst, cntlCfm)
Pst * pst;
PjMngmt * cntlCfm;
#endif
{

   TRC3(PtMiLpjCntrlCfm)

   UNUSED(pst);
   UNUSED(cntlCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjStaInd
(
Pst * pst,
PjMngmt * staInd
)
#else
PUBLIC S16 PtMiLpjStaInd(pst, staInd)
Pst * pst;
PjMngmt * staInd;
#endif
{

   TRC3(PtMiLpjStaInd)

   UNUSED(pst);
   UNUSED(staInd);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjStaCfm
(
Pst * pst,
PjMngmt * staCfm
)
#else
PUBLIC S16 PtMiLpjStaCfm(pst, staCfm)
Pst * pst;
PjMngmt * staCfm;
#endif
{

   TRC3(PtMiLpjStaCfm)

   UNUSED(pst);
   UNUSED(staCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjStsCfm
(
Pst * pst,
Action actn,
PjMngmt * stsCfm
)
#else
PUBLIC S16 PtMiLpjStsCfm(pst, actn, stsCfm)
Pst * pst;
Action actn;
PjMngmt * stsCfm;
#endif
{

   TRC3(PtMiLpjStsCfm)

   UNUSED(pst);
   UNUSED(actn);
   UNUSED(stsCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjTrcInd
(
Pst * pst,
PjMngmt * trc,
Buffer * mBuf
)
#else
PUBLIC S16 PtMiLpjTrcInd(pst, trc, mBuf)
Pst * pst;
PjMngmt * trc;
Buffer * mBuf;
#endif
{

   TRC3(PtMiLpjTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(mBuf);

   RETVALUE(ROK);

}

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 PtMiLpjL2MeasCfm
(
Pst * pst,
PjL2MeasCfmEvt * measEvt
)
#else
PUBLIC S16 PtMiLpjL2MeasCfm(pst, measEvt)
Pst * pst;
PjL2MeasCfmEvt * measEvt;
#endif
{

   TRC3(PtMiLpjL2MeasCfm)

   UNUSED(pst);
   UNUSED(measEvt);

   RETVALUE(ROK);

}
#ifdef ANSI
PRIVATE S16 PtMiLpjL2MeasStopCfm
(
Pst * pst,
U16 measType,
U8 status
)
#else
PRIVATE S16 PtMiLpjL2MeasStopCfm(pst, measType,status)
Pst * pst;
U16  measType;
U8  status;
#endif
{

   TRC3(PtMiLpjL2MeasStopCfm)

   UNUSED(pst);
   UNUSED(measType);
   UNUSED(status);

   RETVALUE(ROK);

}
#endif /*  LTE_L2_MEAS */
#endif /*--ifdef PTPJLPJ--*/

/********************************************************************30**

         End of file
**********************************************************************/
