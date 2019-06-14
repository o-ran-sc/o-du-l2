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
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
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
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg_prg.x"           /* Prg(MAC-MAC)interface includes */
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
#ifdef ANSI
PUBLIC S16 cmPkPrgPMacSMacUeSCellCfgReq
(
Pst           *pst,
RgPrgUeSCellCfgInfo   *ueSCellCfgInfo
)
#else
PUBLIC S16 cmPkPrgPMacSMacUeSCellCfgReq(pst, ueSCellCfgInfo)
Pst            *pst;
RgPrgUeSCellCfgInfo    *ueSCellCfgInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkPrgPMacSMacUeSCellCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   
   if(SAddPstMsgMult((Data *)ueSCellCfgInfo, sizeof(RgPrgUeSCellCfgInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTPRGUESCELLCFGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellCfgReq
(
RgPrgUeSCellCfgReq   func,
Pst                  *pst,
Buffer               *mBuf
)
#else
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellCfgReq(func, pst, mBuf)
RgPrgUeSCellCfgReq   func;
Pst                  *pst;
Buffer               *mBuf;
#endif
{
   RgPrgUeSCellCfgInfo    ueSCellCfgInfo;
   
   TRC2(cmUnpkPrgPMacSMacUeSCellCfgReq)

   if(SRemPreMsgMult((Data *)&ueSCellCfgInfo, sizeof(RgPrgUeSCellCfgInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGPRG_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &ueSCellCfgInfo));
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
#ifdef ANSI
PUBLIC S16 cmPkPrgSMacPMacCfgCfm
(
Pst               *pst,
RgPrgCfgCfmInfo   *cfgCfm
)
#else
PUBLIC S16 cmPkPrgSMacPMacCfgCfm(pst, cfgCfm)
Pst                *pst;
RgPrgCfgCfmInfo    *cfgCfm;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkPrgSMacPMacCfgCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   
   if(SAddPstMsgMult((Data *)cfgCfm, sizeof(RgPrgCfgCfmInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
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
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkPrgSMacPMacCfgCfm
(
RgSMacPMacCfgCfm    func,
Pst                 *pst,
Buffer              *mBuf
)
#else
PUBLIC S16 cmUnpkPrgSMacPMacCfgCfm(func, pst, mBuf)
RgSMacPMacCfgCfm    func;
Pst                 *pst;
Buffer              *mBuf;
#endif
{
   RgPrgCfgCfmInfo   cfgCfm;
   
   TRC2(cmUnpkPrgSMacPMacCfgCfm)

   if(SRemPreMsgMult((Data *)&cfgCfm, sizeof(RgPrgCfgCfmInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGPRG_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &cfgCfm));
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
#ifdef ANSI
PUBLIC S16 cmPkPrgPMacSMacUeSCellDelReq
(
Pst                   *pst,
RgPrgUeSCellDelInfo   *ueSCellDelInfo
)
#else
PUBLIC S16 cmPkPrgPMacSMacUeSCellDelReq(pst, ueSCellDelInfo)
Pst                    *pst;
RgPrgUeSCellDelInfo    *ueSCellDelInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkPrgPMacSMacUeSCellDelReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   
   if(SAddPstMsgMult((Data *)ueSCellDelInfo, sizeof(RgPrgUeSCellDelInfo), mBuf)\
         != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTPRGUESCELLDELREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellDelReq
(
RgUeSCellDelReq    func,
Pst                *pst,
Buffer             *mBuf
)
#else
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellDelReq(func, pst, mBuf)
RgUeSCellDelReq  func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
  RgPrgUeSCellDelInfo ueSCellDelInfo;
   
   TRC2(cmUnpkPrgPMacSMacUeSCellDelReq)

   if(SRemPreMsgMult((Data *)&ueSCellDelInfo, sizeof(RgPrgUeSCellDelInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGPRG_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &ueSCellDelInfo));
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
#ifdef ANSI
PUBLIC S16 cmPkPrgPMacSMacUeSCellLchAddReq
(
Pst                      *pst,
RgPrgUeSCellLchAddInfo   *lchCfgInfo,
)
#else
PUBLIC S16 cmPkPrgPMacSMacUeSCellLchAddReq(pst, lchCfgInfo)
Pst                       *pst;
RgPrgUeSCellLchAddInfo    *lchCfgInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkPrgPMacSMacUeSCellLchAddReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   
   if(SAddPstMsgMult((Data *)lchCfgInfo, sizeof(RgPrgUeSCellLchAddInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTPRGUESCELLLCHADDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellLchAddReq
(
RgPrgUeSCellLchAddInfo       func,
Pst                          *pst,
Buffer                       *mBuf
)
#else
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellLchAddReq(func, pst, mBuf)
RgPrgUeSCellLchAddInfo       func;
Pst                          *pst;
Buffer                       *mBuf;
#endif
{
   RgPrgLchRecfgInfo    lchCfgInfo;
   
   TRC2(cmUnpkPrgPMacSMacUeSCellLchAddReq)

   if(SRemPreMsgMult((Data *)&lchCfgInfo, sizeof(RgPrgUeSCellLchAddInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGPRG_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &lchCfgInfo));
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
#ifdef ANSI
PUBLIC S16 cmPkPrgPMacSMacUeSCellLchDelReq
(
Pst                       *pst,
RgPrgUeSCellLchDelInfo    *delLcCb
)
#else
PUBLIC S16 cmPkPrgPMacSMacUeSCellLchDelReq(pst, delLcCb)
Pst                       *pst;
RgPrgUeSCellLchDelInfo    *delLcCb;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkPrgPMacSMacUeSCellLchDelReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   
   if(SAddPstMsgMult((Data *)delLcCb, sizeof(RgPrgUeSCellLchDelInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTPRGUESCELLLCHDELREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmPkPrgPMacSMacUeSCellLchModReq
(
Pst                      *pst,
RgPrgUeSCellLchModInfo   *lchCfgInfo
)
#else
PUBLIC S16 cmPkPrgPMacSMacUeSCellLchModReq(pst, lchCfgInfo)
Pst                       *pst;
RgPrgUeSCellLchModInfo    *lchCfgInfo;
#endif
{
   Buffer *mBuf = NULLP;

   TRC2(cmPkPrgPMacSMacUeSCellLchModReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }
   
   if(SAddPstMsgMult((Data *)lchCfgInfo, sizeof(RgPrgUeSCellLchModInfo),\
            mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTPRGUESCELLLCHMODREQ;
   RETVALUE(SPstTsk(pst,mBuf));
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
#ifdef ANSI
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellLchModReq
(
RgPrgUeScellModLchReq   func,
Pst                     *pst,
Buffer                  *mBuf
)
#else
PUBLIC S16 cmUnpkPrgPMacSMacAddLchCfgReq(func, pst, mBuf)
RgPrgUeScellModLchReq   func;
Pst                     *pst;
Buffer                  *mBuf;
#endif
{
   RgPrgUeSCellLchModInfo   lchCfgInfo;
   
   TRC2(cmUnpkPrgPMacSMacAddLchCfgReq)

   if(SRemPreMsgMult((Data *)&lchCfgInfo, sizeof(RgPrgUeSCellLchModInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGPRG_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &lchCfgInfo));
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
#ifdef ANSI
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellLchDelReq
(
RgPrgUeScellDelLchReq   func,
Pst                     *pst,
Buffer                  *mBuf
)
#else
PUBLIC S16 cmUnpkPrgPMacSMacUeSCellLchDelReq(func, pst, mBuf)
RgPrgUeScellDelLchReq   func;
Pst                     *pst;
Buffer                  *mBuf;
#endif
{
   RgPrgUeSCellLchDelInfo   lchCfgInfo;
   
   TRC2(cmUnpkPrgPMacSMacUeSCellLchDelReq)

   if(SRemPreMsgMult((Data *)&lchCfgInfo, sizeof(RgPrgUeSCellLchDelInfo), mBuf) != ROK)
   {
      RGPRG_FREE_MSG(mBuf);
      RETVALUE(RFAILED);
   }

   RGPRG_FREE_MSG(mBuf);
   RETVALUE((*func)(pst, &lchCfgInfo));
}

#endif /*LCPRG*/ 
#endif /*LTE_ADV*/

/**********************************************************************
 
         End of file
**********************************************************************/
