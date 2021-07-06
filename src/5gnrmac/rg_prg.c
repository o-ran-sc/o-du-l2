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
  
     Desc:     C source code for packing/unpacking of MAC to MAC interface
               primitives.
  
     File:     rg_prg.c 
  
**********************************************************************/
/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_prg.h"
#include "rg_env.h"
#include "rg.h"
#include "rg_err.h"

/* header/extern include files (.x) */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg_prg.x"           /* Prg(MAC-MAC)interface includes */
#include "du_app_mac_inf.h"
#include "rg.x"            /* typedefs for MAC */

#ifdef LTE_ADV
#ifdef LCPRG
/** 
 * @brief Ue SCell Cfg Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmPkPrgPMacSMacUeSCellCfgReq
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   RgPrgUeSCellCfgInfo   *ueSCellCfgInfo
 *  @return   S16
 *      -# ROK
 **/
S16 cmPkPrgPMacSMacUeSCellCfgReq(Pst  *pst,RgPrgUeSCellCfgInfo   *ueSCellCfgInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   
   if(SAddPstMsgMult((Data *)ueSCellCfgInfo, sizeof(RgPrgUeSCellCfgInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTPRGUESCELLCFGREQ;
   return (SPstTsk(pst,mBuf));
}

/** 
 * @brief Ue SCell Cfg Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmUnpkPrgPMacSMacUeSCellCfgReq
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   RgPrgUeSCellCfgInfo   *ueSCellCfgInfo
 *  @return   S16
 *      -# ROK
 **/
S16 cmUnpkPrgPMacSMacUeSCellCfgReq(RgPrgUeSCellCfgReq   func,Pst *pst,Buffer  *mBuf)
{
   RgPrgUeSCellCfgInfo    ueSCellCfgInfo;
   
   if(SRemPreMsgMult((Data *)&ueSCellCfgInfo, sizeof(RgPrgUeSCellCfgInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGPRG_FREE_MSG(mBuf);
   return ((*func)(pst, &ueSCellCfgInfo));
}

/** 
 * @brief Config confirm for Ue SCell config Req.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmPkPrgSMacPMacCfgCfm
 *
 *  @param[in]   Pst              *pst
 *  @param[in]   RgPrgCfgCfmInfo  *cfgCfm
 *  @return   S16
 *      -# ROK
 **/
S16 cmPkPrgSMacPMacCfgCfm(Pst *pst,RgPrgCfgCfmInfo   *cfgCfm)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   
   if(SAddPstMsgMult((Data *)cfgCfm, sizeof(RgPrgCfgCfmInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }
   switch(cfgCfm->event)
   {
      case EVTPRGUESCELLLCHMODCFM : /*cfm for Lch recfg */ 
      {
         pst->event = (Event) EVTPRGUESCELLLCHMODCFM;
      }
      break;
      case EVTPRGUESCELLCFGCFM : /*cfm for Adding Scell */ 
      {
         pst->event = (Event) EVTPRGUESCELLCFGCFM;
      }
      break;
      case EVTPRGUESCELLLCHDELCFM : /* cfm for deleting Lch */
      {
         pst->event = (Event) EVTPRGUESCELLLCHDELCFM;
      }
      break;
      case EVTPRGUESCELLLCHADDCFM: /* cfm for adding of LCh */
      {
         pst->event = (Event) EVTPRGUESCELLLCHADDCFM;
      }
      break;
   }
   return (SPstTsk(pst,mBuf));
}

/** 
 * @brief Config confirm for SCell addd Req.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmUnpkPrgSMacPMacCfgCfm
 *
 *  @param[in]   CfgCfm          fun
 *  @param[in]   Pst*            pst
 *  @param[in]   RgPrgCfgCfmInfo *cfgCfm
 *  @return   S16
 *      -# ROK
 **/
S16 cmUnpkPrgSMacPMacCfgCfm
(
RgSMacPMacCfgCfm    func,
Pst                 *pst,
Buffer              *mBuf
)
{
   RgPrgCfgCfmInfo   cfgCfm;
   
   if(SRemPreMsgMult((Data *)&cfgCfm, sizeof(RgPrgCfgCfmInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGPRG_FREE_MSG(mBuf);
   return ((*func)(pst, &cfgCfm));
}

/** 
 * @brief SCell Ue Delete Req/UeId change req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmPkPrgPMacSMacUeSCellDelReq
 *
 *  @param[in]   Pst                  *pst
 *  @param[in]   RgPrgUeSCellDelInfo  *ueSCellDelInfo
 *  @return   S16
 *      -# ROK
 **/
S16 cmPkPrgPMacSMacUeSCellDelReq(Pst *pst,RgPrgUeSCellDelInfo   *ueSCellDelInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   
   if(SAddPstMsgMult((Data *)ueSCellDelInfo, sizeof(RgPrgUeSCellDelInfo), mBuf)\
         != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTPRGUESCELLDELREQ;
   return (SPstTsk(pst,mBuf));
}

/** 
 * @brief Unpacking for SCell UE delete/Ue Id Change Req to SMAC.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmUnpkPrgPMacSMacUeSCellDelReq
 *
 *  @param[in]   CfgCfm        fun
 *  @param[in]   Pst*          pst
 *  @param[in]   Buffer        *mBuf
 *  @return   S16
 *      -# ROK
 **/
S16 cmUnpkPrgPMacSMacUeSCellDelReq
(
RgUeSCellDelReq    func,
Pst                *pst,
Buffer             *mBuf
)
{
  RgPrgUeSCellDelInfo ueSCellDelInfo;
   
   if(SRemPreMsgMult((Data *)&ueSCellDelInfo, sizeof(RgPrgUeSCellDelInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGPRG_FREE_MSG(mBuf);
   return ((*func)(pst, &ueSCellDelInfo));
}

/** 
 * @brief Ue Lch Addition  Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmPkPrgPMacSMacUeSCellLchAddReq
 *
 *  @param[in]   Pst*                      pst
 *  @param[in]   RgPrgUeSCellLchAddInfo   *lchCfgInfo
 *  @return   S16
 *      -# ROK
 **/
S16 cmPkPrgPMacSMacUeSCellLchAddReq(Pst *pst,RgPrgUeSCellLchAddInfo   *lchCfgInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   
   if(SAddPstMsgMult((Data *)lchCfgInfo, sizeof(RgPrgUeSCellLchAddInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTPRGUESCELLLCHADDREQ;
   return (SPstTsk(pst,mBuf));
}

/** 
 * @brief LCH Addition Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmUnpkPrgPMacSMacUeSCellLchAddReq
 *
 *  @param[in]   Pst*                     pst
 *  @param[in]   RgPrgUeSCellLchAddInfo   *lchCfgInfo
 *  @param[in]Buffer                      *mBuf
 *  @return   S16
 *      -# ROK
 **/
S16 cmUnpkPrgPMacSMacUeSCellLchAddReq
(
RgPrgUeSCellLchAddInfo       func,
Pst                          *pst,
Buffer                       *mBuf
)
{
   RgPrgLchRecfgInfo    lchCfgInfo;
   
   if(SRemPreMsgMult((Data *)&lchCfgInfo, sizeof(RgPrgUeSCellLchAddInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGPRG_FREE_MSG(mBuf);
   return ((*func)(pst, &lchCfgInfo));
}

/** 
 * @brief Ue SCell Delete Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmPkPrgPMacSMacUeSCellLchDelReq
 *
 *  @param[in]   Pst                       *pst
 *  @param[in]   RgPrgUeSCellLchDelInfo    *delLcCb
 *  @return   S16
 *      -# ROK
 **/
S16 cmPkPrgPMacSMacUeSCellLchDelReq(Pst *pst,RgPrgUeSCellLchDelInfo *delLcCb)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   
   if(SAddPstMsgMult((Data *)delLcCb, sizeof(RgPrgUeSCellLchDelInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTPRGUESCELLLCHDELREQ;
   return (SPstTsk(pst,mBuf));
}


/** 
 * @brief Ue Lch reCfg Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmPkPrgPMacSMacUeSCellLchModReq
 *
 *  @param[in]   Pst                      *pst
 *  @param[in]   RgPrgUeSCellLchModInfo   *lchCfgInfo
 *  @return   S16
 *      -# ROK
 **/
S16 cmPkPrgPMacSMacUeSCellLchModReq(Pst  *pst,RgPrgUeSCellLchModInfo  *lchCfgInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   
   if(SAddPstMsgMult((Data *)lchCfgInfo, sizeof(RgPrgUeSCellLchModInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTPRGUESCELLLCHMODREQ;
   return (SPstTsk(pst,mBuf));
}


/** 
 * @brief LCH Cfg Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmUnpkPrgPMacSMacUeSCellLchModReq
 *
 *  @param[in]   Pst*                pst
 *  @param[in]   RgPrgLchRecfgInfo   *lchCfgInfo
 *  @param[in]   Buffer              *mBuf
 *  @return   S16
 *      -# ROK
 **/
S16 cmUnpkPrgPMacSMacUeSCellLchModReq
(
RgPrgUeScellModLchReq   func,
Pst                     *pst,
Buffer                  *mBuf
)
{
   RgPrgUeSCellLchModInfo   lchCfgInfo;
   
   if(SRemPreMsgMult((Data *)&lchCfgInfo, sizeof(RgPrgUeSCellLchModInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGPRG_FREE_MSG(mBuf);
   return ((*func)(pst, &lchCfgInfo));
}


/** 
 * @brief LCH Del Req from PMac to SMac.
 * @details This primitive is used for light-weight loose coupling.
 *
 * @details
 *
 *     Function : cmUnpkPrgPMacSMacUeSCellLchDelReq
 *
 *  @param[in]   Pst*                    pst
 *  @param[in]   RgPrgUeScellDelLchReq   func;
 *  @param[in]   Buffer                  *mBuf;
 *  @return      S16
 *      -# ROK
 **/
S16 cmUnpkPrgPMacSMacUeSCellLchDelReq
(
RgPrgUeScellDelLchReq   func,
Pst                     *pst,
Buffer                  *mBuf
)
{
   RgPrgUeSCellLchDelInfo   lchCfgInfo;
   
   if(SRemPreMsgMult((Data *)&lchCfgInfo, sizeof(RgPrgUeSCellLchDelInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      return RFAILED;
   }

   RGPRG_FREE_MSG(mBuf);
   return ((*func)(pst, &lchCfgInfo));
}

#endif /*LCPRG*/ 
#endif /*LTE_ADV*/

/**********************************************************************
 
         End of file
**********************************************************************/
