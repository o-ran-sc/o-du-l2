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
 
     Name:     SCH layer
  
     Type:     C source file
  
     Desc:     C source code file for SPS related functions which are
               currently dummy
  
     File:     rg_sch_sps.c 
  
**********************************************************************/
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rgm.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"

/**
 *@brief Initializes the SPS related global data structures
 *
 *@details
 *     Function : rgSCHCmnSpsInit
 *
 *  @param[in] Void
 * 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsInit
(
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsInit ()
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsInit);

   RETVOID;
}

/**
 *@brief Initializes UE with SPS info
 *
 *@details
 *     Function : rgSCHCmnSpsUeCfg 
 *
 *  @param[in] Void
 * 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUeCfg
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUeCfg (cell, ue, ueCfg, err)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUeCfg);

   return ROK;
}

/**
 *@brief Re-Initializes UE with SPS info
 *
 *@details
 *     Function : rgSCHCmnSpsUeReCfg 
 *
 *  @param[in] Void
 * 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUeRecfg
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUeRecfg (cell, ue, ueCfg, err)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUeRecfg);

   return ROK;
}

/**
 *@brief Clears the UE of SPS info
 *
 *@details
 *     Function : rgSCHCmnSpsUeDel
 *
 *  @param[in] Void
 * 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUeDel
(
RgSchCellCb        *cell,
RgSchUeCb          *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUeDel (cell, ue)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUeDel);
   RETVOID;
}

/**
 *@brief Validates cell config for SPS
 *
 *@details
 *     Function : rgSCHCmnSpsCellCfg
 *
 *  @param[in] Void
 * 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsCellCfg
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsCellCfg(cell, cellCfg, err)
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsCellCfg);
   return ROK;
}

/**
 *@brief Deletes cell config for SPS
 *
 *@details
 *     Function : rgSCHCmnSpsCellDel
 *
 *  @param[in] Void
 * 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsCellDel
(
RgSchCellCb        *cell
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsCellDel (cell)
RgSchCellCb        *cell;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsCellDel);
   RETVOID;
}

/**
 *@brief Configures DL LC for SPS
 *
 *@details
 *     Function :rgSCHCmnSpsDlLcCfg 
 *
 *  @param[in] Void
 * 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsDlLcCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchCfg          *lcCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsDlLcCfg(cell, ue, dlLc, lcCfg, err)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *dlLc;
RgrLchCfg          *lcCfg;
RgSchErrInfo       *err
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsDlLcCfg);
   return ROK;
}

Void rgSchCmnDlSfHqDel 
ARGS((
RgSchUeCb    *ue,
RgSchCellCb  *cell
))
{
   TRC2(rgSchCmnDlSfHqDel);
   RETVOID;
}

S16 rgSCHCmnSpsDlLcRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchRecfg        *lcRecfg,
RgSchErrInfo       *err
))
{
   
   TRC2(rgSCHCmnSpsDlLcRecfg);
   return ROK;
}

Void rgSCHCmnSpsUlLcgDel ARGS(( 
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg
))
{
   TRC2(rgSCHCmnSpsUlLcgDel);
   RETVOID;
}

Void rgSCHCmnSpsDlLcDel ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc
))
{
   TRC2(rgSCHCmnSpsUlLcgDel);
   RETVOID;
}

S16 rgSCHCmnSpsUlTti ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
))
{

   TRC2(rgSCHCmnSpsUlTti);
   return ROK;
}

Void rgSCHCmnSpsDlDedBoUpd ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *dlLc
))
{

   TRC2(rgSCHCmnSpsDlDedBoUpd);
   RETVOID;
}

Void rgSCHCmnSpsDlProcAddToRetx ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
))
{

   TRC2(rgSCHCmnSpsDlProcAddToRetx);
   RETVOID;
}

Void rgSCHCmnSpsDlCqiIndHndlr ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      timingInfo
))
{

   TRC2(rgSCHCmnSpsDlCqiIndHndlr);
   RETVOID;
}

S16 rgSCHCmnSpsUlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue
))
{

   TRC2(rgSCHCmnSpsUlCqiInd);
   return ROK;
}

S16 rgSCHCmnSpsBsrRpt ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg
))
{

   TRC2(rgSCHCmnSpsBsrRpt);
   return ROK;
}

Void rgSCHCmnSpsPhrInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
))
{

   TRC2(rgSCHCmnSpsPhrInd);
   RETVOID;
}

Void rgSCHCmnSpsDlRelPdcchFbk ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
Bool                  isAck
))
{

   TRC2(rgSCHCmnSpsDlRelPdcchFbk);
   RETVOID;
}

Void rgSCHCmnSpsDlProcAck ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
))
{

   TRC2(rgSCHCmnSpsDlProcAck);
   RETVOID;
}

Void rgSCHCmnSpsDlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
))
{

   TRC2(rgSCHCmnSpsDlUeReset);
   RETVOID;
}

Void rgSCHCmnSpsUlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
))
{

   TRC2(rgSCHCmnSpsUlUeReset);
   RETVOID;
}

Void rgSCHCmnSpsUlProcRelInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isExplRel
))
{

   TRC2(rgSCHCmnSpsUlProcRelInd);
   RETVOID;
}

Void rgSCHCmnSpsUlProcActInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U16                 spsSduSize
))
{

   TRC2(rgSCHCmnSpsUlProcActInd);
   RETVOID;
}

S16 rgSCHCmnSpsUlProcCrcInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      crcTime
))
{

   TRC2(rgSCHCmnSpsUlProcActInd);
   return ROK;
}

S16 rgSCHCmnSpsUlProcDtxInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      dtxTime
))
{

   TRC2(rgSCHCmnSpsUlProcDtxInd);
   return ROK;
}

Void rgSCHCmnSpsDlUpdDlSfAllocWithSps ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf
))
{

   TRC2(rgSCHCmnSpsUlProcDtxInd);
   RETVOID;
}





/**********************************************************************
 
         End of file
**********************************************************************/
