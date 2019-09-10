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
 
        File:    pj_ptui.c
  
*********************************************************************21*/

/** @file pj_ptui.c
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
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"

#include "pj_err.h"
#include "pj_env.h"        /* RLC environment options */

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
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */


#if !(defined(LCPJUICPJ)  && defined(NH))
#define PTPJCPJ
#endif

#if !(defined(LCPJUIPJU) && (defined(PX))) || !(defined(NH))
#define PTPJPJU
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

  
/*********************************************************************
 *             Forward Declartion for CPJ Porting Functions
 ********************************************************************/
#ifdef PTPJCPJ

EXTERN S16 PtUiCpjBndCfm ARGS((Pst * pst, SuId suId, U8 status));

EXTERN S16 PtUiCpjCfgCfm ARGS((Pst * pst, SuId suId, CpjCfgCfmInfo * cfgCfm));

EXTERN S16 PtUiCpjReEstCfm ARGS((Pst * pst, SuId suId, CpjReEstCfmInfo * reEstCfm));

EXTERN S16 PtUiCpjSecCfgCfm ARGS((Pst * pst, SuId suId, CpjSecCfgCfmInfo * secCfgCfm));

EXTERN S16 PtUiCpjUeIdChgCfm ARGS((Pst * pst, SuId suId, U32 transId, CpjUeInfo  * ueInfo, CmStatus status));

EXTERN S16 PtUiCpjCountCfm ARGS((Pst * pst, SuId suId, CpjCountCfmInfo * countCfm));

EXTERN S16 PtUiCpjSduStaCfm ARGS((Pst * pst, SuId suId, CpjSduStaCfmInfo * staCfm));
EXTERN S16 PtUiCpjKdfCfm ARGS((Pst* pst, SuId suId, CpjCfgTransId transId, CpjKdfCfmInfo *kdfCfmInf,
U8 status
));
EXTERN S16 PtUiCpjDatResumeCfm ARGS(( Pst *pst, SuId suId, CpjDatResumeCfmInfo *datResumeCfm));
#endif /* PTPJCPJ */

  
/*********************************************************************
 *             Forward Declartion for PJU Porting Functions
 ********************************************************************/
#ifdef PTPJPJU

EXTERN S16 PtUiPjuBndReq ARGS((Pst* pst, SpId spId, SuId suId));

EXTERN S16 PtUiPjuBndCfm ARGS((Pst* pst, SuId suId, U8 status));

EXTERN S16 PtUiPjuDatCfm ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, PjuDatCfmInfo * datCfm));

EXTERN S16 PtUiPjuDatInd ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, Buffer * mBuf));

EXTERN S16 PtUiPjuStaInd ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, PjuStaIndInfo * staInd, Buffer *mBuf));

EXTERN S16 PtUiPjuDatFwdInd ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, PjuDatFwdIndInfo * datFwdInd));
#endif /*--#ifdef PTPJPJU--*/

  
/*********************************************************************
 *                         Primitives for CPJ interface 
 ********************************************************************/
PRIVATE CONSTANT CpjBndCfm PjUiCpjBndCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjBndCfm,
#else
   PtUiCpjBndCfm,
#endif
#ifdef NH
   NhLiCpjBndCfm,
#else
   PtUiCpjBndCfm,
#endif
};


PRIVATE CONSTANT CpjCfgCfm PjUiCpjCfgCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjCfgCfm,
#else
   PtUiCpjCfgCfm,
#endif
#ifdef NH
   NhLiCpjCfgCfm,
#else
   PtUiCpjCfgCfm,
#endif
};


PRIVATE CONSTANT CpjReEstCfm PjUiCpjReEstCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjReEstCfm,
#else
   PtUiCpjReEstCfm,
#endif
#ifdef NH
   NhLiCpjReEstCfm,
#else
   PtUiCpjReEstCfm,
#endif
};


PRIVATE CONSTANT CpjSecCfgCfm PjUiCpjSecCfgCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjSecCfgCfm,
#else
   PtUiCpjSecCfgCfm,
#endif
#ifdef NH
   NhLiCpjSecCfgCfm,
#else
   PtUiCpjSecCfgCfm,
#endif
};


PRIVATE CONSTANT CpjUeIdChgCfm PjUiCpjUeIdChgCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjUeIdChgCfm,
#else
   PtUiCpjUeIdChgCfm,
#endif
#ifdef NH
   NhLiCpjUeIdChgCfm,
#else
   PtUiCpjUeIdChgCfm,
#endif
};


PRIVATE CONSTANT CpjCountCfm PjUiCpjCountCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjCountCfm,
#else
   PtUiCpjCountCfm,
#endif
#ifdef NH
   NhLiCpjCountCfm,
#else
   PtUiCpjCountCfm,
#endif
};


PRIVATE CONSTANT CpjSduStaCfm PjUiCpjSduStaCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjSduStaCfm,
#else
   PtUiCpjSduStaCfm,
#endif
#ifdef NH
   NhLiCpjSduStaCfm,
#else
   PtUiCpjSduStaCfm,
#endif
};

PRIVATE CONSTANT CpjDatResumeCfm PjUiCpjDatResumeCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjDatResumeCfm,
#else
   PtUiCpjDatResumeCfm,
#endif
#ifdef NH
   NhLiCpjDatResumeCfm,
#else
   PtUiCpjDatResumeCfm,
#endif
};

/** @brief This API is used to receive a Config confirm from PDCP to
 * Application.
 */
PRIVATE CONSTANT CpjKdfCfm PjUiCpjKdfCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjKdfCfm,
#else
   PtUiCpjKdfCfm,
#endif
#ifdef NH
   PtUiCpjKdfCfm
#else
   PtUiCpjKdfCfm
#endif

};

  
/****************************************************************************
 *                         CPJ Interface Mt functions
 ***************************************************************************/
#ifdef PJ
#ifdef ANSI
PUBLIC S16 PjUiCpjBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PjUiCpjBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PjUiCpjBndCfm)

   (*PjUiCpjBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjCfgCfm
(
Pst * pst,
SuId suId,
CpjCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PjUiCpjCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
CpjCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PjUiCpjCfgCfm)

   (*PjUiCpjCfgCfmMt[pst->selector])(pst, suId, cfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjReEstCfm
(
Pst * pst,
SuId suId,
CpjReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PjUiCpjReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
CpjReEstCfmInfo * reEstCfm;
#endif
{

   TRC3(PjUiCpjReEstCfm)

   (*PjUiCpjReEstCfmMt[pst->selector])(pst, suId, reEstCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjSecCfgCfm
(
Pst * pst,
SuId suId,
CpjSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PjUiCpjSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
CpjSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PjUiCpjSecCfgCfm)

   (*PjUiCpjSecCfgCfmMt[pst->selector])(pst, suId, secCfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
CpjUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PjUiCpjUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
CpjUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PjUiCpjUeIdChgCfm)

   (*PjUiCpjUeIdChgCfmMt[pst->selector])(pst, suId, transId, ueInfo, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjCountCfm
(
Pst * pst,
SuId suId,
CpjCountCfmInfo * countCfm
)
#else
PUBLIC S16 PjUiCpjCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
CpjCountCfmInfo * countCfm;
#endif
{

   TRC3(PjUiCpjCountCfm)

   (*PjUiCpjCountCfmMt[pst->selector])(pst, suId, countCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjSduStaCfm
(
Pst * pst,
SuId suId,
CpjSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PjUiCpjSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
CpjSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PjUiCpjSduStaCfm)

   (*PjUiCpjSduStaCfmMt[pst->selector])(pst, suId, staCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjDatResumeCfm
(
Pst                   *pst,
SuId                  suId,
CpjDatResumeCfmInfo   *datResCfm
)
#else
PUBLIC S16 PjUiCpjDatResumeCfm(pst, suId, datResCfm)
Pst                   *pst;
SuId                  suId;
CpjDatResumeCfmInfo   *datResCfm;
#endif
{

   TRC3(PjUiCpjDataResumeCfm)

   (*PjUiCpjDatResumeCfmMt[pst->selector])(pst, suId, datResCfm);

   RETVALUE(ROK);
}
/**
* @brief This API is used to receive a Bind confirm from PDCP to Application.
*
* @details
*
*     Function : PjUiCpjKdfCfm
*
*  @param[in]   pst - Post structure
*  @param[in]   suId - Service User Id
*  @param[in]   transId - Transaction Id
*  @param[in]   kdfCfmInf - Generated keys
*  @param[in]   status - status of KDF request
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PjUiCpjKdfCfm
(
Pst* pst,
SuId suId,
CpjCfgTransId transId,
CpjKdfCfmInfo *kdfCfmInf,
U8 status
)
#else
PUBLIC S16 PjUiCpjKdfCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CpjCfgTransId transId;
CpjKdfCfmInfo *kdfCfmInf;
U8 status;
#endif
{

   TRC3(PtUiCpjCfgCfm);

   (*PjUiCpjKdfCfmMt[pst->selector])(pst, suId, transId,kdfCfmInf, status);

   RETVALUE(ROK);
}
#endif /*--ifdef PJ--*/

  
/*************************************************************************
 *                         CPJ Porting Functions
 ************************************************************************/
#ifdef PTPJCPJ

#ifdef ANSI
PUBLIC S16 PtUiCpjBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiCpjBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiCpjBndCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjCfgCfm
(
Pst * pst,
SuId suId,
CpjCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PtUiCpjCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
CpjCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PtUiCpjCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjReEstCfm
(
Pst * pst,
SuId suId,
CpjReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PtUiCpjReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
CpjReEstCfmInfo *reEstCfm;
#endif
{

   TRC3(PtUiCpjReEstCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(reEstCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjSecCfgCfm
(
Pst * pst,
SuId suId,
CpjSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PtUiCpjSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
CpjSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PtUiCpjSecCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(secCfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
CpjUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PtUiCpjUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
CpjUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PtUiCpjUeIdChgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjCountCfm
(
Pst * pst,
SuId suId,
CpjCountCfmInfo * countCfm
)
#else
PUBLIC S16 PtUiCpjCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
CpjCountCfmInfo * countCfm;
#endif
{

   TRC3(PtUiCpjCountCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(countCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjSduStaCfm
(
Pst * pst,
SuId suId,
CpjSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PtUiCpjSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
CpjSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PtUiCpjSduStaCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staCfm);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PtUiCpjDatResumeCfm
(
Pst                   *pst,
SuId                  suId,
CpjDatResumeCfmInfo   *datResumeCfm
)
#else
PUBLIC S16 PtUiCpjDatResumeCfm(pst, suId, dataResCfm)
Pst                    *pst;
SuId                   suId;
CpjDataResumeCfmInfo   *datResumeCfm;
#endif
{

   TRC3(PtUiCpjDataResCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datResumeCfm);

   RETVALUE(ROK);

}

/**
* @brief This is a portable API for KDF function 
*
* @details
*
*     Function : PtUiCpjKdfCfm
*
*  @param[in]   pst - Post structure
*  @param[in]   suId - Service User Id
*  @param[in]   transId - Transaction Id
*  @param[in]   kdfCfmInf - Generated keys
*  @param[in]   status - status of KDF request
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiCpjKdfCfm
(
Pst* pst,
SuId suId,
CpjCfgTransId transId,
CpjKdfCfmInfo *kdfCfmInf,
U8 status
)
#else
PUBLIC S16 PtUiCpjKdfCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CpjCfgTransId transId;
CpjKdfCfmInfo *kdfCfmInf;
U8 status;
#endif
{

   TRC3(PtUiCpjKdfCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(kdfCfmInf);
   UNUSED(status);

   RETVALUE(ROK);
}
#endif /*--ifdef PTPJCPJ--*/

  
/*********************************************************************
 *                         Primitives for PJU interface 
 ********************************************************************/

PRIVATE CONSTANT PjuBndCfm PjUiPjuBndCfmMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuBndCfm,
#else
   PtUiPjuBndCfm,
#endif
#ifdef NH
   NhLiPjuBndCfm,
#else
   PtUiPjuBndCfm,
#endif
#ifdef PX
   PxLiPjuBndCfm,
#else
   PtUiPjuBndCfm,
#endif
};


PRIVATE CONSTANT PjuDatCfm PjUiPjuDatCfmMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatCfm,
#else
   PtUiPjuDatCfm,
#endif
#ifdef NH
   NhLiPjuDatCfm,
#else
   PtUiPjuDatCfm,
#endif
#ifdef PX
   PxLiPjuDatCfm,
#else
   PtUiPjuDatCfm,
#endif
};


PRIVATE CONSTANT PjuDatInd PjUiPjuDatIndMt[] =
{
#ifdef SS_RBUF
#ifndef TENB_ACC 
#ifdef LCPJUIPJU
   cmPkFpPjuDatInd,
#else
   PtUiPjuDatInd,
#endif
#endif
#else
#ifdef LCPJUIPJU
   cmPkPjuDatInd,
#else
   PtUiPjuDatInd,
#endif
#endif
#ifdef NH
   NhLiPjuDatInd,
#else
   PtUiPjuDatInd,
#endif
#ifdef PX
   PxLiPjuDatInd,
#else
   PtUiPjuDatInd,
#endif
};


PRIVATE CONSTANT PjuStaInd PjUiPjuStaIndMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuStaInd,
#else
   PtUiPjuStaInd,
#endif
#ifdef NH
   NhLiPjuStaInd,
#else
   PtUiPjuStaInd,
#endif
#ifdef PX
   PxLiPjuStaInd,
#else
   PtUiPjuStaInd,
#endif
};


PRIVATE CONSTANT PjuDatFwdInd PjUiPjuDatFwdIndMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatFwdInd,
#else
   PtUiPjuDatFwdInd,
#endif
#ifdef NH
   NhLiPjuDatFwdInd,
#else
   PtUiPjuDatFwdInd,
#endif
#ifdef PX
   PxLiPjuDatFwdInd,
#else
   PtUiPjuDatFwdInd,
#endif
};

  
/****************************************************************************
 *                         PJU Interface Mt functions
 ***************************************************************************/
#ifdef PJ

#ifdef ANSI
PUBLIC S16 PjUiPjuBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PjUiPjuBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PjUiPjuBndCfm)

   (*PjUiPjuBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuDatCfm
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatCfmInfo * datCfm
)
#else
PUBLIC S16 PjUiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatCfmInfo * datCfm;
#endif
{

   TRC3(PjUiPjuDatCfm)

   (*PjUiPjuDatCfmMt[pst->selector])(pst, suId, pdcpId, datCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuDatInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
Buffer * mBuf
)
#else
PUBLIC S16 PjUiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
Buffer * mBuf;
#endif
{

   TRC3(PjUiPjuDatInd)

   (*PjUiPjuDatIndMt[pst->selector])(pst, suId, pdcpId, mBuf);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuStaInd
(
Pst * pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuStaIndInfo * staInd,
Buffer    *mBuf
)
#else
PUBLIC S16 PjUiPjuStaInd(pst, suId, pdcpId, staInd, mBuf)
Pst * pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuStaIndInfo * staInd;
Buffer *mBuf;
#endif
{

   TRC3(PjUiPjuStaInd)

   (*PjUiPjuStaIndMt[pst->selector])(pst, suId, pdcpId, staInd, mBuf);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuDatFwdInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatFwdIndInfo * datFwdInd
)
#else
PUBLIC S16 PjUiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatFwdIndInfo * datFwdInd;
#endif
{

   TRC3(PjUiPjuDatFwdInd)

   (*PjUiPjuDatFwdIndMt[pst->selector])(pst, suId, pdcpId, datFwdInd);

   RETVALUE(ROK);

}
#endif /*--ifdef PJ--*/

  
/*************************************************************************
 *                         PJU Porting Functions
 ************************************************************************/
#ifdef PTPJPJU

#ifdef ANSI
PUBLIC S16 PtUiPjuBndReq
(
Pst* pst,
SpId spId,
SuId suId
)
#else
PUBLIC S16 PtUiPjuBndReq(pst, spId, suId)
Pst* pst;
SpId spId;
SuId suId;
#endif
{

   TRC3(PtUiPjuBndReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(suId);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiPjuBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiPjuBndCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuDatCfm
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatCfmInfo * datCfm
)
#else
PUBLIC S16 PtUiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatCfmInfo * datCfm;
#endif
{

   TRC3(PtUiPjuDatCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(datCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuDatInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
Buffer * mBuf
)
#else
PUBLIC S16 PtUiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
Buffer * mBuf;
#endif
{

   TRC3(PtUiPjuDatInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(mBuf);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuStaInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuStaIndInfo * staInd,
Buffer *mBuf
)
#else
PUBLIC S16 PtUiPjuStaInd(pst, suId, pdcpId, staInd, mBuf)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuStaIndInfo * staInd;
Buffer *mBuf;
#endif
{

   TRC3(PtUiPjuStaInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(staInd);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuDatFwdInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatFwdIndInfo * datFwdInd
)
#else
PUBLIC S16 PtUiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatFwdIndInfo * datFwdInd;
#endif
{

   TRC3(PtUiPjuDatFwdInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(datFwdInd);

   RETVALUE(ROK);

}
#endif /*--ifdef PTPJPJU--*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file
**********************************************************************/
