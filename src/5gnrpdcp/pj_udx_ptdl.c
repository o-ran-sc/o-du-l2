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
  
        Name:    LTE-PDCP Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the upper interface of LTE-PDCP
 
        File:    pj_udx_ptdl.c
  
*********************************************************************21*/

/** @file pj_udx_ptdl.c
@brief PDCP Upper Interface
*/


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
#include "cpj.h"           /* UDX defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"

#include "pj_err.h"
#include "pj_env.h"        /* RLC environment options */
#include "pj_udx.h"

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
#include "cpj.x"           /* UDX defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#include "pj.h"
#include "pj.x"
#include "pj_udx.x"


#if !(defined(LCPJULUDX)  && defined(PJ))
#define PTPJUDX
#endif


#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

  
/*********************************************************************
 *             Forward Declartion for UDX Porting Functions
 ********************************************************************/
#ifdef PTPJUDX

EXTERN S16 PtDlUdxBndCfm ARGS((Pst * pst, SuId suId, U8 status));

EXTERN S16 PtDlUdxCfgCfm ARGS((Pst * pst, SuId suId, UdxCfgCfmInfo * cfgCfm));

EXTERN S16 PtDlUdxReEstCfm ARGS((Pst * pst, SuId suId, UdxReEstCfmInfo * reEstCfm));

EXTERN S16 PtDlUdxSecCfgCfm ARGS((Pst * pst, SuId suId, UdxSecCfgCfmInfo * secCfgCfm));

EXTERN S16 PtDlUdxUeIdChgCfm ARGS((Pst * pst, SuId suId, U32 transId, UdxUeInfo  * ueInfo, CmStatus status));

EXTERN S16 PtDlUdxCountCfm ARGS((Pst * pst, SuId suId, UdxCountCfmInfo * countCfm));

EXTERN S16 PtDlUdxSduStaCfm ARGS((Pst * pst, SuId suId, UdxSduStaCfmInfo * staCfm));

EXTERN S16 PtDlUdxDatResumeCfm ARGS((Pst * pst, SuId suId, UdxDatResumeCfmInfo * datResume));

EXTERN S16 PtDlUdxDatForwardCfm ARGS((Pst * pst, SuId suId, UdxDatFwdCfmInfo * datFwdCfm));
#endif /* PTPJUDX */

  
/*********************************************************************
 *             Forward Declartion for PJU Porting Functions
 ********************************************************************/

  
/*********************************************************************
 *                         Primitives for UDX interface 
 ********************************************************************/
PRIVATE CONSTANT UdxBndCfm PjDlUdxBndCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxBndCfm,
#else
   PtDlUdxBndCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxBndCfm,
#else
   PtDlUdxBndCfm,
#endif
#ifdef PJ
   PjUlUdxBndCfm,
#else
   PtDlUdxBndCfm,
#endif
};


PRIVATE CONSTANT UdxCfgCfm PjDlUdxCfgCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxCfgCfm,
#else
   PtDlUdxCfgCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxCfgCfm,
#else
   PtDlUdxCfgCfm,
#endif
#ifdef PJ
   PjUlUdxCfgCfm,
#else
   PtDlUdxCfgCfm,
#endif
};


PRIVATE CONSTANT UdxReEstCfm PjDlUdxReEstCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxReEstCfm,
#else
   PtDlUdxReEstCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxReEstCfm,
#else
   PtDlUdxReEstCfm,
#endif
#ifdef PJ
   PjUlUdxReEstCfm,
#else
   PtDlUdxReEstCfm,
#endif
};


PRIVATE CONSTANT UdxSecCfgCfm PjDlUdxSecCfgCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxSecCfgCfm,
#else
   PtDlUdxSecCfgCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxSecCfgCfm,
#else
   PtDlUdxSecCfgCfm,
#endif
#ifdef PJ
   PjUlUdxSecCfgCfm,
#else
   PtDlUdxSecCfgCfm,
#endif
};


PRIVATE CONSTANT UdxUeIdChgCfm PjDlUdxUeIdChgCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxUeIdChgCfm,
#else
   PtDlUdxUeIdChgCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxUeIdChgCfm,
#else
   PtDlUdxUeIdChgCfm,
#endif
#ifdef PJ
   PjUlUdxUeIdChgCfm,
#else
   PtDlUdxUeIdChgCfm,
#endif
};


PRIVATE CONSTANT UdxCountCfm PjDlUdxCountCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxCountCfm,
#else
   PtDlUdxCountCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxCountCfm,
#else
   PtDlUdxCountCfm,
#endif
#ifdef PJ
   PjUlUdxCountCfm,
#else
   PtDlUdxCountCfm,
#endif
};


PRIVATE CONSTANT UdxSduStaCfm PjDlUdxSduStaCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxSduStaCfm,
#else
   PtDlUdxSduStaCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxSduStaCfm,
#else
   PtDlUdxSduStaCfm,
#endif
#ifdef PJ
   PjUlUdxSduStaCfm,
#else
   PtDlUdxSduStaCfm,
#endif
};

PRIVATE CONSTANT UdxDatResumeCfm PjDlUdxDatResumeCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxDatResumeCfm,
#else
   PtDlUdxDatResumeCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxDatResumeCfm,
#else
   PtDlUdxDatResumeCfm,
#endif
#ifdef PJ
   PjUlUdxDatResumeCfm,
#else
   PtDlUdxDatResumeCfm,
#endif
};

PRIVATE CONSTANT UdxDatForwardCfm PjDlUdxDatFwdCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkPjUdxDatForwardCfm,
#else
   PtDlUdxDatForwardCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkPjUdxDatForwardCfm,
#else
   PtDlUdxDatForwardCfm,
#endif
#ifdef PJ
   PjUlUdxDatForwardCfm,
#else
   PtDlUdxDatForwardCfm,
#endif
};

  
/****************************************************************************
 *                         UDX Interface Mt functions
 ***************************************************************************/
#ifdef PJ
#ifdef ANSI
PUBLIC S16 PjDlUdxBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PjDlUdxBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PjDlUdxBndCfm)

   (*PjDlUdxBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxCfgCfm
(
Pst * pst,
SuId suId,
UdxCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PjDlUdxCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
UdxCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PjDlUdxCfgCfm)

   (*PjDlUdxCfgCfmMt[pst->selector])(pst, suId, cfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxReEstCfm
(
Pst * pst,
SuId suId,
UdxReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PjDlUdxReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
UdxReEstCfmInfo * reEstCfm;
#endif
{

   TRC3(PjDlUdxReEstCfm)

   (*PjDlUdxReEstCfmMt[pst->selector])(pst, suId, reEstCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxSecCfgCfm
(
Pst * pst,
SuId suId,
UdxSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PjDlUdxSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
UdxSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PjDlUdxSecCfgCfm)

   (*PjDlUdxSecCfgCfmMt[pst->selector])(pst, suId, secCfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PjDlUdxUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
UdxUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PjDlUdxUeIdChgCfm)

   (*PjDlUdxUeIdChgCfmMt[pst->selector])(pst, suId, transId, ueInfo, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxCountCfm
(
Pst * pst,
SuId suId,
UdxCountCfmInfo * countCfm
)
#else
PUBLIC S16 PjDlUdxCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
UdxCountCfmInfo * countCfm;
#endif
{

   TRC3(PjDlUdxCountCfm)

   (*PjDlUdxCountCfmMt[pst->selector])(pst, suId, countCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxSduStaCfm
(
Pst * pst,
SuId suId,
UdxSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PjDlUdxSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PjDlUdxSduStaCfm)

   (*PjDlUdxSduStaCfmMt[pst->selector])(pst, suId, staCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxDatResumeCfm
(
Pst * pst,
SuId suId,
UdxDatResumeCfmInfo * staCfm
)
#else
PUBLIC S16 PjDlUdxDatResumeCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxDatResumeCfmInfo * staCfm;
#endif
{

   TRC3(PjDlUdxDatResumeCfm)

   (*PjDlUdxDatResumeCfmMt[pst->selector])(pst, suId, staCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxDatFwdCfm
(
Pst * pst,
SuId  suId,
UdxDatFwdCfmInfo  * datFwdCfm
)
#else
PUBLIC S16 PjDlUdxDatFwdCfm(pst, suId, datFwdCfm)
Pst * pst;
SuId  suId;
UdxDatFwdCfmInfo  * datFwdCfm;
#endif
{

   TRC3(PjDlUdxDatFwdCfm)

   (*PjDlUdxDatFwdCfmMt[pst->selector])(pst, suId, datFwdCfm);

   RETVALUE(ROK);
}
#endif
  
/*************************************************************************
 *                         UDX Porting Functions
 ************************************************************************/
#ifdef PTPJUDX

#ifdef ANSI
PUBLIC S16 PtDlUdxBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtDlUdxBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtDlUdxBndCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxCfgCfm
(
Pst * pst,
SuId suId,
UdxCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PtDlUdxCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
UdxCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PtDlUdxCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxReEstCfm
(
Pst * pst,
SuId suId,
UdxReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PtDlUdxReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
UdxReEstCfmInfo *reEstCfm;
#endif
{

   TRC3(PtDlUdxReEstCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(reEstCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxSecCfgCfm
(
Pst * pst,
SuId suId,
UdxSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PtDlUdxSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
UdxSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PtDlUdxSecCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(secCfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PtDlUdxUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
UdxUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PtDlUdxUeIdChgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxCountCfm
(
Pst * pst,
SuId suId,
UdxCountCfmInfo * countCfm
)
#else
PUBLIC S16 PtDlUdxCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
UdxCountCfmInfo * countCfm;
#endif
{

   TRC3(PtDlUdxCountCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(countCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxSduStaCfm
(
Pst * pst,
SuId suId,
UdxSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PtDlUdxSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PtDlUdxSduStaCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staCfm);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PtDlUdxDatResumeCfm
(
Pst * pst,
SuId suId,
UdxDatResumeCfmInfo * staCfm
)
#else
PUBLIC S16 PtDlUdxDatResumeCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxDatResumeCfmInfo * staCfm;
#endif
{

   TRC3(PtDlUdxDatResumeCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staCfm);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PtDlUdxDatForwardCfm
(
 Pst               *pst,
 SuId              suId,
 UdxDatFwdCfmInfo  *datFwdCfm
 )
#else
PUBLIC S16 PtDlUdxDatForwardCfm(pst, suId, datFwdCfm)
   Pst               *pst;
   SuId              suId;
   UdxDatFwdCfmInfo  *datFwdCfm;
#endif
{

   TRC3(PtDlUdxDatForwardCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datFwdCfm); 

   RETVALUE(ROK);
}
#endif /*--ifdef PTPJUDX--*/


#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file
**********************************************************************/
