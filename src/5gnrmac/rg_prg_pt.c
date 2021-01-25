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
  
     Desc:     C source code for MAC to MAC (PRG) interface 
  
     File:     rg_prg_pt.c 
  
**********************************************************************/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"           /* layer manager for MAC */
#include "crg.h"           /* CRG interface includes*/
#include "rgu.h"           /* RGU interface includes*/
#include "tfu.h"           /* TFU interface includes*/
#include "rg_sch_inf.h"    /* SCH interface includes*/
#include "rg_prg.h"        /* PRG interface includes*/
#include "rg_env.h"        /* MAC Enviroments includes */
#include "rg.h"            /* MAC includes*/
#include "rg_err.h"        /* MAC error includes*/

/* header/extern include files (.x) */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* layer management typedefs for MAC */
#include "rg_prg.x"        /* PRG interface typedefs*/
#include "du_app_mac_inf.h"
#include "rg.x"            /* typedefs for MAC */

#ifdef LTE_ADV
#include "rg_pom_scell.x"

/* Matrix for Ue SCell Config Req*/
static const RgPrgUeSCellCfgReq RgPrgPMacSMacUeSCellCfgReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellCfgReq,
#else
   RgPrgPMacSMacUeSCellCfgReq
#endif
};

/**
* @brief Ue SCell config Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeSCellCfg
*
*  @param[in]   Pst         *pst
*  @param[in]   RgPrgUeSCellCfgInfo *ueSCellCfgInfo;
*  @return   S16
*      -# ROK
**/
S16 RgPrgPMacSMacUeSCellCfg(Pst *pst, RgPrgUeSCellCfgInfo *ueSCellCfgInfo)
{

   return ((*RgPrgPMacSMacUeSCellCfgReqMt[0])(pst, ueSCellCfgInfo));
}

/* Matrix for config confirm from SMac to Pmac*/
static const RgSMacPMacCfgCfm RgPrgSMacPMacCfgCfmMt[RG_PRG_MAX] =
{
#ifdef LCPRG
   cmPkPrgSMacPMacCfgCfm,
#else
   RgPrgSMacPMacCfgCfm
#endif
};

/**
* @brief Config confirm from SMac to PMac for Ue Cell Config Req 
*
* @details
*
*     Function :RgPrgSMacPMacCfg
*
*  @param[in]   Pst              *pst
*  @param[in]   RgPrgCfgCfmInfo  *cfmCfm
*  @return   S16
*      -# ROK
**/
S16 RgPrgSMacPMacCfg(Pst *pst,RgPrgCfgCfmInfo *cfgCfm)
{

   return ((*RgPrgSMacPMacCfgCfmMt[0])(pst, cfgCfm));
}

/* Matrix for Ue SCell delete req/Ue Id change req from PMac to SMac */
static const RgUeSCellDelReq RgPrgPMacSMacUeSCellDelReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
   cmPkPrgPMacSMacUeSCellDelReq,
#else
   RgPrgPMacSMacUeSCellDelReq
#endif
};

/**
* @brief Ue SCell delete Req/Ue Id change Req from PMac to SMac
*
* @details
*
*     Function : RgPrgPMacSMacUeSCellDel 
*
*  @param[in]   Pst                 *pst
*  @param[in]   RgPrgUeSCellDelInfo *ueSCellDelInfo;
*  @return   S16
*      -# ROK
**/
S16 RgPrgPMacSMacUeSCellDel(Pst *pst,RgPrgUeSCellDelInfo *ueSCellDelInfo)
{

   return ((*RgPrgPMacSMacUeSCellDelReqMt[0])(pst, ueSCellDelInfo));
}



/* Matrix for Lch ReConfig Req*/
static const RgPrgUeScellModLchReq RgPrgPMacSMacUeScellModLchReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellLchModReq,
#else
   RgPrgPMacSMacUeSCellLchModReq
#endif
};


/**
* @brief Ue Lch Reconfig Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeScellLchMod
*
*  @param[in]   Pst                      *pst
*  @param[in]   RgPrgUeSCellLchModInfo   *lchCfgInfo
*  @return   S16
*      -# ROK
**/
S16 RgPrgPMacSMacUeScellLchMod(Pst *pst,RgPrgUeSCellLchModInfo *lchCfgInfo)
{

   return ((*RgPrgPMacSMacUeScellModLchReqMt[0])(pst, lchCfgInfo));
}


/* Matrix for delete Lch Req*/
static const RgPrgUeScellDelLchReq RgPrgPMacSMacUeScellDelLchReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellLchDelReq,
#else
   RgPrgPMacSMacUeSCellLchDelReq
#endif
};


/**
* @brief Delete Lch Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeScellLchDel
*
*  @param[in]   Pst                      *pst
*  @param[in]   RgPrgUeSCellLchDelInfo   *delLcCb
*  @return   S16
*      -# ROK
**/
S16 RgPrgPMacSMacUeScellLchDel(Pst *pst,RgPrgUeSCellLchDelInfo *delLcCb)
{

   return ((*RgPrgPMacSMacUeScellDelLchReqMt[0])(pst, delLcCb));
}


/* Matrix for Lch Config Req*/
static const RgPrgUeScellAddLchReq RgPrgPMacSMacUeScellAddLchReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellLchAddReq,
#else
   RgPrgPMacSMacUeSCellLchAddReq
#endif
};


/**
* @brief Ue Lch config Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeScellLchAdd
*
*  @param[in]   Pst                      *pst
*  @param[in]   RgPrgUeSCellLchAddInfo   *lchCfgInfo
*  @return   S16
*      -# ROK
**/
S16 RgPrgPMacSMacUeScellLchAdd(Pst *pst,RgPrgUeSCellLchAddInfo *lchCfgInfo)
{

   return ((*RgPrgPMacSMacUeScellAddLchReqMt[0])(pst, lchCfgInfo));
}

#endif /* LTE_ADV */

/**********************************************************************
 
         End of file
**********************************************************************/
