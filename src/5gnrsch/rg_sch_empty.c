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
#include "common_def.h"
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
Void rgSCHCmnSpsInit()
{

   return;
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
S16 rgSCHCmnSpsUeCfg(RgSchCellCb *cell,RgSchUeCb  *ue,RgrUeCfg *ueCfg,RgSchErrInfo *err)
{

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
S16 rgSCHCmnSpsUeRecfg(RgSchCellCb *cell,RgSchUeCb *ue,RgrUeRecfg *ueRecfg,RgSchErrInfo *err)
{

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
Void rgSCHCmnSpsUeDel(RgSchCellCb *cell,RgSchUeCb  *ue)
{

   return;
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
S16 rgSCHCmnSpsCellCfg(RgSchCellCb *cell,RgrCellCfg *cellCfg,RgSchErrInfo *err)
{

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
Void rgSCHCmnSpsCellDel(RgSchCellCb *cell)
{

   return;
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
S16 rgSCHCmnSpsDlLcCfg(RgSchCellCb  *cell,RgSchUeCb  *ue,RgSchDlLcCb  *dlLc,RgrLchCfg  *lcCfg,RgSchErrInfo  *err)
{

   return ROK;
}

Void rgSchCmnDlSfHqDel 
ARGS((
RgSchUeCb    *ue,
RgSchCellCb  *cell
))
{
   return;
}

S16 rgSCHCmnSpsDlLcRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchRecfg        *lcRecfg,
RgSchErrInfo       *err
))
{
   
   return ROK;
}

Void rgSCHCmnSpsUlLcgDel ARGS(( 
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg
))
{
   return;
}

Void rgSCHCmnSpsDlLcDel ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc
))
{
   return;
}

S16 rgSCHCmnSpsUlTti ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
))
{

   return ROK;
}

Void rgSCHCmnSpsDlDedBoUpd ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *dlLc
))
{

   return;
}

Void rgSCHCmnSpsDlProcAddToRetx ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
))
{

   return;
}

Void rgSCHCmnSpsDlCqiIndHndlr ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      timingInfo
))
{

   return;
}

S16 rgSCHCmnSpsUlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue
))
{

   return ROK;
}

S16 rgSCHCmnSpsBsrRpt ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg
))
{

   return ROK;
}

Void rgSCHCmnSpsPhrInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
))
{

   return;
}

Void rgSCHCmnSpsDlRelPdcchFbk ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
Bool                  isAck
))
{

   return;
}

Void rgSCHCmnSpsDlProcAck ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
))
{

   return;
}

Void rgSCHCmnSpsDlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
))
{

   return;
}

Void rgSCHCmnSpsUlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
))
{

   return;
}

Void rgSCHCmnSpsUlProcRelInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isExplRel
))
{

   return;
}

Void rgSCHCmnSpsUlProcActInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
uint16_t                 spsSduSize
))
{

   return;
}

S16 rgSCHCmnSpsUlProcCrcInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      crcTime
))
{

   return ROK;
}

S16 rgSCHCmnSpsUlProcDtxInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      dtxTime
))
{

   return ROK;
}

Void rgSCHCmnSpsDlUpdDlSfAllocWithSps ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf
))
{

   return;
}





/**********************************************************************
 
         End of file
**********************************************************************/
