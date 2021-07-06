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
  
     Desc:     C source code for pack/unpack of LRG interface primitives. 
  
     File:     lrg.c 
  
**********************************************************************/

/** @file lrg.c
@brief This file contains the packing/unpacking code for the LRG interface 
       primitives.
*/

/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"           /* LRG Interface Defines */
/* header/extern include files (.x) */
#include "lrg.x"           /* LRG Interface includes */

#if (defined(LCLRG))


/**
* @brief This API is used to send a 
Configuration Request from LM to MAC.
*
* @details
*
*     Function: cmPkLrgCfgReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfg
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgCfgReq
(
Pst * pst,
RgMngmt * cfg
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG001, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cfg, EVTLRGCFGREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG002, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   pst->event = (Event) EVTLRGCFGREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Configuration Request from LM to MAC.
*
* @details
*
*     Function: cmUnpkLrgCfgReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfg
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgCfgReq
(
LrgCfgReq func,
Pst *pst,
Buffer *mBuf
)
{
   /* lrg_c_001.main_2: Changed Pointer across Interface */
   RgMngmt cfg;
   
   
   if (cmUnpkRgMngmt(pst, &cfg, EVTLRGCFGREQ, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG003, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cfg));
}


/**
* @brief This API is used to send a 
Configuration Request from LM to SCH.
*
* @details
*
*     Function: cmPkLrgSchCfgReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfg
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchCfgReq
(
Pst * pst,
RgMngmt * cfg
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG004, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cfg, EVTMACSCHGENCFGREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG005, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Configuration Request from LM to SCH.
*
* @details
*
*     Function: cmUnpkLrgSchCfgReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfg
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchCfgReq
(
LrgSchCfgReq func,
Pst *pst,
Buffer *mBuf
)
{
   /* lrg_c_001.main_2: Changed Pointer across Interface */
   RgMngmt cfg;
   

   if (cmUnpkRgMngmt(pst, &cfg, EVTMACSCHGENCFGREQ, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG006, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cfg));
}


/**
* @brief This API is used to send a 
Configuration Confirm from MAC to LM.
*
* @details
*
*     Function: cmPkLrgCfgCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
uint8_t cmPkLrgCfgCfm
(
Pst * pst,
RgMngmt * cfm
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG007, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cfm, EVTLRGCFGCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG008, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGCFGCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Configuration Confirm from MAC to LM.
*
* @details
*
*     Function: cmUnpkLrgCfgCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgCfgCfm
(
LrgCfgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cfm;
   

   if (cmUnpkRgMngmt(pst, &cfm, EVTLRGCFGCFM, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG009, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cfm));
}


/**
* @brief This API is used to send a 
Configuration Confirm from SCH to LM.
*
* @details
*
*     Function: cmPkLrgSchCfgCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfg
*  @return   S16
*      -# ROK
**/
uint8_t cmPkLrgSchCfgCfm
(
Pst * pst,
RgMngmt * cfg
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG010, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cfg, EVTMACSCHGENCFGCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG011, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTMACSCHGENCFGCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Configuration Confirm from SCH to LM.
*
* @details
*
*     Function: cmUnpkLrgSchCfgCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfg
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchCfgCfm
(
LrgSchCfgCfm func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cfg;
   
   if (cmUnpkRgMngmt(pst, &cfg, EVTMACSCHGENCFGCFM, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG012, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cfg));
}


/**
* @brief This API is used to send a 
Statistics Request from LM to MAC.
*
* @details
*
*     Function: cmPkLrgStsReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  sts
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgStsReq
(
Pst * pst,
RgMngmt * sts
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG013, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, sts, EVTLRGSTSREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG014, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGSTSREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Statistics Request from LM to MAC.
*
* @details
*
*     Function: cmUnpkLrgStsReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  sts
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgStsReq
(
LrgStsReq func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt sts;
   

   if (cmUnpkRgMngmt(pst, &sts, EVTLRGSTSREQ, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG015, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &sts));
}


/**
* @brief This API is used to send a 
Statistics Confirm from MAC to LM.
*
* @details
*
*     Function: cmPkLrgStsCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgStsCfm
(
Pst * pst,
RgMngmt * cfm
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG016, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cfm, EVTLRGSTSCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG017, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGSTSCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Statistics Confirm from MAC to LM.
*
* @details
*
*     Function: cmUnpkLrgStsCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgStsCfm
(
LrgStsCfm func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cfm;
   

   if (cmUnpkRgMngmt(pst, &cfm, EVTLRGSTSCFM, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG018, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cfm));
}


/**
* @brief This API is used to send a 
Status Request from LM to MAC.
*
* @details
*
*     Function: cmPkLrgStaReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  sta
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgStaReq
(
Pst * pst,
RgMngmt * sta
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG019, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmPkRgMngmt(pst, sta, EVTLRGSSTAREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG020, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   
   pst->event = (Event) EVTLRGSSTAREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Status Request from LM to MAC.
*
* @details
*
*     Function: cmUnpkLrgStaReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  sta
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgStaReq
(
LrgStaReq func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt sta;
   
   
   memset(&sta, 0, sizeof(RgMngmt));
   if (cmUnpkRgMngmt(pst, &sta, EVTLRGSSTAREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG021, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &sta));
}


/**
* @brief This API is used to send a 
Status Confirm from MAC to LM.
*
* @details
*
*     Function: cmPkLrgStaCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgStaCfm
(
Pst * pst,
RgMngmt * cfm
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG022, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (cmPkRgMngmt(pst, cfm, EVTLRGSSTACFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG023, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   
   pst->event = (Event) EVTLRGSSTACFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Status Confirm from MAC to LM.
*
* @details
*
*     Function: cmUnpkLrgStaCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgStaCfm
(
LrgStaCfm func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cfm;
   
  
   memset(&cfm, 0, sizeof(RgMngmt));
   if (cmUnpkRgMngmt(pst, &cfm, EVTLRGSSTACFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG024, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cfm));
}


/**
* @brief This API is used to send a 
Status Indication from MAC to LM.
*
* @details
*
*     Function: cmPkLrgStaInd
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  usta
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgStaInd
(
Pst * pst,
RgMngmt * usta
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG025, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, usta, EVTLRGUSTAIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG026, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGUSTAIND;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Status Indication from MAC to LM.
*
* @details
*
*     Function: cmUnpkLrgStaInd
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  usta
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgStaInd
(
LrgStaInd func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt usta;
   

   if (cmUnpkRgMngmt(pst, &usta, EVTLRGUSTAIND, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG027, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &usta));
}


/**
* @brief This API is used to send a 
Status Indication from SCH to LM.
*
* @details
*
*     Function: cmPkLrgSchStaInd
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  sta
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchStaInd
(
Pst * pst,
RgMngmt * sta
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG028, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, sta, EVTLRGSCHSTAIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG029, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGSCHSTAIND;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Status Indication from SCH to LM.
*
* @details
*
*     Function: cmUnpkLrgSchStaInd
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  sta
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchStaInd
(
LrgSchStaInd func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt sta;
   

   if (cmUnpkRgMngmt(pst, &sta, EVTLRGSCHSTAIND, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG030, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &sta));
}


/**
* @brief This API is used to send a 
Control Request from LM to MAC.
*
* @details
*
*     Function: cmPkLrgCntrlReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cntrl
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG031, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cntrl, EVTLRGCNTRLREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG032, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGCNTRLREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Control Request from LM to MAC.
*
* @details
*
*     Function: cmUnpkLrgCntrlReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cntrl
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgCntrlReq
(
LrgCntrlReq func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cntrl;
   

   if (cmUnpkRgMngmt(pst, &cntrl, EVTLRGCNTRLREQ, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG033, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cntrl));
}


/**
* @brief This API is used to send a 
Control Request from LM to SCH.
*
* @details
*
*     Function: cmPkLrgSchCntrlReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cntrl
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG034, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cntrl, EVTLRGSCHCNTRLREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG035, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGSCHCNTRLREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Control Request from LM to SCH.
*
* @details
*
*     Function: cmUnpkLrgSchCntrlReq
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cntrl
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchCntrlReq
(
LrgSchCntrlReq func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cntrl;
   

   if (cmUnpkRgMngmt(pst, &cntrl, EVTLRGSCHCNTRLREQ, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG036, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cntrl));
}


/**
* @brief This API is used to send a 
Control Confirm from MAC to LM.
*
* @details
*
*     Function: cmPkLrgCntrlCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgCntrlCfm
(
Pst * pst,
RgMngmt * cfm
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG037, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cfm, EVTLRGCNTRLCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG038, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGCNTRLCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Control Confirm from MAC to LM.
*
* @details
*
*     Function: cmUnpkLrgCntrlCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cfm
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgCntrlCfm
(
LrgCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cfm;
   

   if (cmUnpkRgMngmt(pst, &cfm, EVTLRGCNTRLCFM, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG039, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cfm));
}


/**
* @brief This API is used to send a 
Control Confirm from SCH to LM.
*
* @details
*
*     Function: cmPkLrgSchCntrlCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cntrl
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchCntrlCfm
(
Pst * pst,
RgMngmt * cntrl
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG040, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      if (cmPkRgMngmt(pst, cntrl, EVTLRGSCHCNTRLCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG041, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   
   pst->event = (Event) EVTLRGSCHCNTRLCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Control Confirm from SCH to LM.
*
* @details
*
*     Function: cmUnpkLrgSchCntrlCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  cntrl
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchCntrlCfm
(
LrgSchCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt cntrl;
   

   if (cmUnpkRgMngmt(pst, &cntrl, EVTLRGSCHCNTRLCFM, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG042, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &cntrl));
}


/**
* @brief This API is used to send a 
Trace Indication from MAC to LM.
*
* @details
*
*     Function: cmPkLrgTrcInd
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  trc
*  @param[in]   Buffer *  trcBuf
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgTrcInd
(
Pst * pst,
RgMngmt * trc,
Buffer * trcBuf
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG043, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(trcBuf);
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      MsgLen msgLen;
      if (SFndLenMsg(trcBuf, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG044, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(trcBuf);
         return RFAILED;
      }
      if (SCatMsg(mBuf, trcBuf, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG045, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(trcBuf);
         return RFAILED;
      }
      SPutMsg(trcBuf);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);
   }
   if (cmPkRgMngmt(pst, trc, EVTLRGTRCIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG046, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(trcBuf);
      SPutMsg(mBuf);
      return RFAILED;
   }
   
   pst->event = (Event) EVTLRGTRCIND;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief This API is used to send a 
Trace Indication from MAC to LM.
*
* @details
*
*     Function: cmUnpkLrgTrcInd
*
*  @param[in]   Pst *  pst
*  @param[in]   RgMngmt *  trc
*  @param[in]   Buffer *  trcBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgTrcInd
(
LrgTrcInd func,
Pst *pst,
Buffer *mBuf
)
{
   RgMngmt trc;
  /* lrg_c_001.main_3 - MODIFY -  Modified trcBuf to have it initialized to NULLP */
   Buffer *trcBuf = NULLP;
   
   
   if (cmUnpkRgMngmt(pst, &trc, EVTLRGTRCIND, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELRG047, (ErrVal)0, "Packing failed");
#endif
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         return RFAILED;
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &trcBuf) != ROK)
         return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &trc, trcBuf));
}


/***********************************************************
*
*     Func : cmPkRgGenCfg
*
*
*     Desc :   This structure holds configuration parameters for MAC General Configuration.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgGenCfg
(
RgGenCfg *param,
Buffer *mBuf
)
{


#ifdef LTE_ADV
   CMCHKPK(oduUnpackUInt8, param->isSCellActDeactAlgoEnable, mBuf);
   CMCHKPK(oduUnpackUInt8, param->forceCntrlSrbBoOnPCel, mBuf);
#endif
   CMCHKPK(oduUnpackUInt8, param->startCellId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->numRguSaps, mBuf);
   CMCHKPK(oduUnpackUInt8, param->tmrRes, mBuf);
   CMCHKPK(cmPkMemoryId, &param->mem, mBuf);
   CMCHKPK(cmPkPst, &param->lmPst, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgGenCfg
*
*
*     Desc :   This structure holds configuration parameters for MAC General Configuration.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgGenCfg
(
RgGenCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkPst, &param->lmPst, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &param->mem, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->tmrRes, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numRguSaps, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->startCellId, mBuf);
#ifdef LTE_ADV
   CMCHKUNPK(oduPackUInt8, &param->forceCntrlSrbBoOnPCel, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->isSCellActDeactAlgoEnable, mBuf);
#endif
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgUpSapCfg
*
*
*     Desc :   This structure holds configuration parameters for MAC Upper SAP Configuration.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgUpSapCfg
(
RgUpSapCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(SPkS16, param->suId, mBuf);
   CMCHKPK(SPkS16, param->spId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->route, mBuf);
   CMCHKPK(oduUnpackUInt8, param->inst, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ent, mBuf);
   CMCHKPK(oduUnpackUInt16, param->procId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->prior, mBuf);
   CMCHKPK(cmPkMemoryId, &param->mem, mBuf);
   CMCHKPK(oduUnpackUInt8, param->selector, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgUpSapCfg
*
*
*     Desc :   This structure holds configuration parameters for MAC Upper SAP Configuration.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgUpSapCfg
(
RgUpSapCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->selector, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &param->mem, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->prior, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->procId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ent, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->inst, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->route, mBuf);
   CMCHKUNPK(SUnpkS16, &param->spId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->suId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgLowSapCfg
*
*
*     Desc :   This structure holds configuration parameters for MAC Lower SAP Configuration.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgLowSapCfg
(
RgLowSapCfg *param,
Buffer *mBuf
)
{


   CMCHKPK(cmPkTmrCfg, &param->bndTmr, mBuf);
   CMCHKPK(SPkS16, param->suId, mBuf);
   CMCHKPK(SPkS16, param->spId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->route, mBuf);
   CMCHKPK(oduUnpackUInt8, param->inst, mBuf);
   CMCHKPK(oduUnpackUInt8, param->ent, mBuf);
   CMCHKPK(oduUnpackUInt16, param->procId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->prior, mBuf);
   CMCHKPK(cmPkMemoryId, &param->mem, mBuf);
   CMCHKPK(oduUnpackUInt8, param->selector, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgLowSapCfg
*
*
*     Desc :   This structure holds configuration parameters for MAC Lower SAP Configuration.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgLowSapCfg
(
RgLowSapCfg *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->selector, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &param->mem, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->prior, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->procId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->ent, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->inst, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->route, mBuf);
   CMCHKUNPK(SUnpkS16, &param->spId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->suId, mBuf);
   CMCHKUNPK(cmUnpkTmrCfg, &param->bndTmr, mBuf);
   return ROK;
}

#ifdef MAC_SCH_STATS


/***********************************************************
*
*     Func : cmPkRgAckNack
*
*
*     Desc :   Ack and Nack statistics
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgAckNack
(
RgAckNack *param,
Buffer *mBuf
)
{


   CMCHKPK(oduUnpackUInt16, param->numOfAcks, mBuf);
   CMCHKPK(oduUnpackUInt16, param->numOfNacks, mBuf);
   CMCHKPK(oduUnpackUInt8, param->mcs, mBuf);

   return ROK;
} /* cmPkRgAckNack */


/***********************************************************
*
*     Func : cmPkRgSchNackAckStats
*
*
*     Desc :   
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSchNackAckStats
(
RgSchNackAckStats *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=14; i >= 0; i--) {
      CMCHKPK(cmPkRgAckNack, &param->ulCqiStat[i], mBuf);
   }

   for (i=14; i >= 0; i--) {
      CMCHKPK(cmPkRgAckNack, &param->dlCqiStat[i], mBuf);
   }

   return ROK;
}


/***********************************************************
*
*     Func : cmPkRgHqNumRetx
*
*
*     Desc :   Harq Retransmission statistics
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgHqNumRetx
(
RgSchHqNumRetx *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, param->totalTx, mBuf);
   CMCHKPK(oduUnpackUInt16, param->numOfHQ_4, mBuf);
   CMCHKPK(oduUnpackUInt16, param->numOfHQ_3, mBuf);
   CMCHKPK(oduUnpackUInt16, param->numOfHQ_2, mBuf);
   CMCHKPK(oduUnpackUInt16, param->numOfHQ_1, mBuf);
   CMCHKPK(oduUnpackUInt8, param->mcs, mBuf);

   return ROK;
} /* cmPkRgHqNumRetx */


/***********************************************************
*
*     Func : cmPkRgSchHqRetxStats
*
*
*     Desc :   
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSchHqRetxStats
(
RgSchHqRetxStats *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=14; i >= 0; i--) {
      CMCHKPK(cmPkRgHqNumRetx, &param->ulCqiStat[i], mBuf);
   }

   for (i=14; i >= 0; i--) {
      CMCHKPK(cmPkRgHqNumRetx, &param->dlCqiStat[i], mBuf);
   }

   return ROK;
} /* cmPkRgSchHqRetxStats */

/* unpcaking functions */

/***********************************************************
*
*     Func : cmUnpkRgAckNack
*
*
*     Desc :   Ack and Nack statistics
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgAckNack
(
RgAckNack *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->mcs, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numOfNacks, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numOfAcks, mBuf);

   return ROK;
} /* cmUnkRgAckNack */


/***********************************************************
*
*     Func : cmUnpkRgSchNackAckStats
*
*
*     Desc :   
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSchNackAckStats
(
RgSchNackAckStats *param,
Buffer *mBuf
)
{
   S32 i;

   for (i=0; i <= 14; i++) {
      CMCHKUNPK(cmUnpkRgAckNack, &param->dlCqiStat[i], mBuf);
   }

   for (i=0; i <= 14; i++) {
      CMCHKUNPK(cmUnpkRgAckNack, &param->ulCqiStat[i], mBuf);
   }

   return ROK;
} /* cmUnpkRgSchNackAckStats */


/***********************************************************
*
*     Func : cmUnpkRgHqNumRetx
*
*
*     Desc :   Harq Retransmission statistics
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgHqNumRetx
(
RgSchHqNumRetx *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->mcs, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numOfHQ_1, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numOfHQ_2, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numOfHQ_3, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numOfHQ_4, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->totalTx, mBuf);

   return ROK;
} /* cmUnpkRgHqNumRetx */


/***********************************************************
*
*     Func : cmUnpkRgSchHqRetxStats
*
*
*     Desc :   
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSchHqRetxStats
(
RgSchHqRetxStats *param,
Buffer *mBuf
)
{
   S32 i;

   for (i=0; i <= 14; i++) {
      CMCHKUNPK(cmUnpkRgHqNumRetx, &param->dlCqiStat[i], mBuf);
   }

   for (i=0; i <= 14; i++) {
      CMCHKUNPK(cmUnpkRgHqNumRetx, &param->ulCqiStat[i], mBuf);
   }

   return ROK;
} /* cmUnpkRgSchHqRetxStats */

#endif /* MAC_SCH_STATS */


/***********************************************************
*
*     Func : cmPkRgGenSts
*
*
*     Desc :   This structure holds General Statistical information of MAC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgGenSts
(
RgGenSts *param,
Buffer *mBuf
)
{

#ifdef MAC_SCH_STATS
   CMCHKPK(cmPkRgSchHqRetxStats, &param->hqRetxStats, mBuf);
   CMCHKPK(cmPkRgSchNackAckStats, &param->nackAckStats, mBuf);
#endif /* MAC_SCH_STATS */
   CMCHKPK(oduUnpackUInt16, param->numCellCfg, mBuf);
   CMCHKPK(oduUnpackUInt32, param->numUeCfg, mBuf);
   CMCHKPK(oduUnpackUInt32, param->numHarqFail, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgGenSts
*
*
*     Desc :   This structure holds General Statistical information of MAC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgGenSts
(
RgGenSts *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt32, &param->numHarqFail, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->numUeCfg, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numCellCfg, mBuf);
#ifdef MAC_SCH_STATS
   CMCHKUNPK(cmUnpkRgSchNackAckStats, &param->nackAckStats, mBuf);
   CMCHKUNPK(cmUnpkRgSchHqRetxStats, &param->hqRetxStats, mBuf);
#endif /* MAC_SCH_STATS */
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgSapSts
*
*
*     Desc :   This structure holds Statistical information of a SAP in MAC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSapSts
(
RgSapSts *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, param->numPduDrop, mBuf);
   CMCHKPK(oduUnpackUInt32, param->numPduTxmit, mBuf);
   CMCHKPK(oduUnpackUInt32, param->numPduRcvd, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgSapSts
*
*
*     Desc :   This structure holds Statistical information of a SAP in MAC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSapSts
(
RgSapSts *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt32, &param->numPduRcvd, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->numPduTxmit, mBuf);
   CMCHKUNPK(oduPackUInt32, &param->numPduDrop, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgSchInstCfg
*
*
*     Desc : Scheduler Configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSchInstCfg
(
RgSchInstCfg *param,
Buffer *mBuf
)
{

   S32 i;

   for (i=param->numSaps-1; i >= 0; i--) {
      CMCHKPK(cmPkRgLowSapCfg, &param->tfuSap[i], mBuf);
   }
   for (i=param->numSaps-1; i >= 0; i--) {
      CMCHKPK(cmPkRgUpSapCfg, &param->rgrSap[i], mBuf);
   }
   for (i=param->numSaps-1; i >= 0; i--) {
      CMCHKPK(cmPkRgUpSapCfg, &param->rgmSap[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, param->numSaps, mBuf);
   CMCHKPK(cmPkRgGenCfg, &param->genCfg, mBuf);
   CMCHKPK(oduUnpackUInt8, param->instId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgSchInstCfg
*
*
*     Desc : Scheduler Configuration
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSchInstCfg
(
RgSchInstCfg *param,
Buffer *mBuf
)
{

   S32 i;

   CMCHKUNPK(oduPackUInt8, &param->instId, mBuf);
   CMCHKUNPK(cmUnpkRgGenCfg, &param->genCfg, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numSaps, mBuf);
   for (i=0; i<param->numSaps; i++) {
      CMCHKUNPK(cmUnpkRgUpSapCfg, &param->rgmSap[i], mBuf);
   }
   for (i=0; i<param->numSaps; i++) {
      CMCHKUNPK(cmUnpkRgUpSapCfg, &param->rgrSap[i], mBuf);
   }
   for (i=0; i<param->numSaps; i++) {
      CMCHKUNPK(cmUnpkRgLowSapCfg, &param->tfuSap[i], mBuf);
   }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgCfg
*
*
*     Desc :   This structure holds Configuration parameters for MAC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgCfg
(
RgCfg *param,
S16 elmnt,
Buffer *mBuf
)
{

      switch(elmnt) {
         case STSCHINST:
            CMCHKPK(cmPkRgSchInstCfg, &param->s.schInstCfg, mBuf);
            break;
         case STTFUSAP:
            CMCHKPK(cmPkRgLowSapCfg, &param->s.tfuSap, mBuf);
            break;
         case STCRGSAP:
            CMCHKPK(cmPkRgUpSapCfg, &param->s.crgSap, mBuf);
            break;
         case STRGUSAP:
            CMCHKPK(cmPkRgUpSapCfg, &param->s.rguSap, mBuf);
            break;
         case STGEN:
            CMCHKPK(cmPkRgGenCfg, &param->s.genCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgCfg
*
*
*     Desc :   This structure holds Configuration parameters for MAC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgCfg
(
RgCfg *param,
S16 elmnt,
Buffer *mBuf
)
{

      switch(elmnt) {
         case STGEN:
            CMCHKUNPK(cmUnpkRgGenCfg, &param->s.genCfg, mBuf);
            break;
         case STRGUSAP:
            CMCHKUNPK(cmUnpkRgUpSapCfg, &param->s.rguSap, mBuf);
            break;
         case STCRGSAP:
            CMCHKUNPK(cmUnpkRgUpSapCfg, &param->s.crgSap, mBuf);
            break;
         case STTFUSAP:
            CMCHKUNPK(cmUnpkRgLowSapCfg, &param->s.tfuSap, mBuf);
            break;
         case STSCHINST:
            CMCHKUNPK(cmUnpkRgSchInstCfg, &param->s.schInstCfg, mBuf);
            break;
         default :
            return RFAILED;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgSapSta
*
*
*     Desc :   This structure holds a SAP's status information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSapSta
(
RgSapSta *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->sapState, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgSapSta
*
*
*     Desc :   This structure holds a SAP's status information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSapSta
(
RgSapSta *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt8, &param->sapState, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgSts
*
*
*     Desc :   This structure holds MAC's Statistical information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSts
(
RgSts *param,
S16 elmnt,
Buffer *mBuf
)
{

      switch(elmnt) {
         case STTFUSAP:
            CMCHKPK(cmPkRgSapSts, &param->s.tfuSts, mBuf);
            break;
         case STRGRSAP:
            CMCHKPK(cmPkRgSapSts, &param->s.rgrSts, mBuf);
            break;
         case STCRGSAP:
            CMCHKPK(cmPkRgSapSts, &param->s.crgSts, mBuf);
            break;
         case STRGUSAP:
            CMCHKPK(cmPkRgSapSts, &param->s.rguSts, mBuf);
            break;
         case STGEN:
            CMCHKPK(cmPkRgGenSts, &param->s.genSts, mBuf);
            break;
         default :
            break;
      }
   CMCHKPK(cmPkAction, param->action, mBuf);
   CMCHKPK(oduUnpackUInt8, param->sapInst, mBuf);
   CMCHKPK(cmPkDateTime, &param->dt, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgSts
*
*
*     Desc :   This structure holds MAC's Statistical information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSts
(
RgSts *param,
S16 elmnt,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkDateTime, &param->dt, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->sapInst, mBuf);
   CMCHKUNPK(cmUnpkAction, &param->action, mBuf);
      switch(elmnt) {
         case STGEN:
            CMCHKUNPK(cmUnpkRgGenSts, &param->s.genSts, mBuf);
            break;
         case STRGUSAP:
            CMCHKUNPK(cmUnpkRgSapSts, &param->s.rguSts, mBuf);
            break;
         case STCRGSAP:
            CMCHKUNPK(cmUnpkRgSapSts, &param->s.crgSts, mBuf);
            break;
         case STRGRSAP:
            CMCHKUNPK(cmUnpkRgSapSts, &param->s.rgrSts, mBuf);
            break;
         case STTFUSAP:
            CMCHKUNPK(cmUnpkRgSapSts, &param->s.tfuSts, mBuf);
            break;
         default :
            break;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgSsta
*
*
*     Desc :   This structure holds MAC's Solicited Status information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSsta
(
Pst *pst,
RgSsta *param,
S16 elmnt,
#ifdef LRG_V1
uint8_t eventType,
#endif
Buffer *mBuf
)
{

      switch(elmnt) {
         case STTFUSAP:
            CMCHKPK(cmPkRgSapSta, &param->s.tfuSapSta, mBuf);
            break;
         case STRGRSAP:
            CMCHKPK(cmPkRgSapSta, &param->s.rgrSapSta, mBuf);
            break;
         case STCRGSAP:
            CMCHKPK(cmPkRgSapSta, &param->s.crgSapSta, mBuf);
            break;
         case STRGUSAP:
            CMCHKPK(cmPkRgSapSta, &param->s.rguSapSta, mBuf);
            break;
            /*ccpu00118255 - ADD - Check for eventType before Pack */
         case STGEN:
#ifdef LRG_V1 
            if (eventType == EVTLRGSSTACFM)
            {
               CMCHKPK(cmPkSystemId, &param->s.sysId, mBuf);
               if (param->s.sysId.ptNmb != NULLP)
               {
                  SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, 
                        (Data *)param->s.sysId.ptNmb, LRG_MAX_PT_NUM_SIZE);
               }
            }
#else /*LRG_V1 not defined */
            if (param->s.sysId.ptNmb != NULLP)
            {
                 CMCHKPK(cmPkSystemId, &param->s.sysId, mBuf);
                 SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)param->s.sysId.ptNmb, LRG_MAX_PT_NUM_SIZE);
            }

#endif /*end of LRG_V1 */
            break;
         default :
            break;
      }
   CMCHKPK(oduUnpackUInt8, param->sapInst, mBuf);
   CMCHKPK(cmPkDateTime, &param->dt, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgSsta
*
*
*     Desc :   This structure holds MAC's Solicited Status information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSsta
(
Pst    *pst,
RgSsta *param,
S16 elmnt,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkDateTime, &param->dt, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->sapInst, mBuf);
      switch(elmnt) {
         case STGEN:
            /*ccpu00118255 - ADD - Check for eventType before Unpack */
            if (pst->event == EVTLRGSSTACFM)
            {
               /* After Merging */
               if((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&param->s.sysId.ptNmb,
                           LRG_MAX_PT_NUM_SIZE)) != ROK){
#if (ERRCLASS & ERRCLS_ADD_RES)
                  SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                        (ErrVal)ELRG048, (ErrVal)0, "Packing failed");
#endif
                  SPutMsg(mBuf);
                  return RFAILED;
               }
               /* KWORK_FIX: Moved the memset under the NULL check */
               if (param->s.sysId.ptNmb != NULLP)
               {
                  memset(param->s.sysId.ptNmb, 0, LRG_MAX_PT_NUM_SIZE);
                  CMCHKUNPK(cmUnpkSystemId, &param->s.sysId, mBuf);
               }
            }
            break;
         case STRGUSAP:
            CMCHKUNPK(cmUnpkRgSapSta, &param->s.rguSapSta, mBuf);
            break;
         case STCRGSAP:
            CMCHKUNPK(cmUnpkRgSapSta, &param->s.crgSapSta, mBuf);
            break;
         case STRGRSAP:
            CMCHKUNPK(cmUnpkRgSapSta, &param->s.rgrSapSta, mBuf);
            break;
         case STTFUSAP:
            CMCHKUNPK(cmUnpkRgSapSta, &param->s.tfuSapSta, mBuf);
            break;
         default :
            break;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgUstaDgn
*
*
*     Desc :   Alarm diagnostics structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgUstaDgn
(
RgUstaDgn *param,
Buffer *mBuf
)
{

      switch(param->type) {
         case LRG_USTA_DGNVAL_MEM:
            CMCHKPK(cmPkMemoryId, &param->u.mem, mBuf);
            break;
         default :
            break;
      }
   CMCHKPK(oduUnpackUInt8, param->type, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgUstaDgn
*
*
*     Desc :   Alarm diagnostics structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgUstaDgn
(
RgUstaDgn *param,
Buffer *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->type, mBuf);
   switch(param->type) 
   {
      case LRG_USTA_DGNVAL_MEM:
	 CMCHKUNPK(cmUnpkMemoryId, &param->u.mem, mBuf);
	 break;
      default :
	 break;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgUsta
*
*
*     Desc :   This structure holds MAC's Unsolicited Status information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgUsta
(
RgUsta *param,
Buffer *mBuf
)
{

   CMCHKPK(cmPkRgUstaDgn, &param->dgn, mBuf);
   CMCHKPK(cmPkCmAlarm, &param->cmAlarm, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgUsta
*
*
*     Desc :   This structure holds MAC's Unsolicited Status information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgUsta
(
RgUsta *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkCmAlarm, &param->cmAlarm, mBuf);
   CMCHKUNPK(cmUnpkRgUstaDgn, &param->dgn, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgTrc
*
*
*     Desc :   This structure holds MAC's Trace Indication information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgTrc
(
RgTrc *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->evnt, mBuf);
   CMCHKPK(cmPkDateTime, &param->dt, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgTrc
*
*
*     Desc :   This structure holds MAC's Trace Indication information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgTrc
(
RgTrc *param,
Buffer *mBuf
)
{

   CMCHKUNPK(cmUnpkDateTime, &param->dt, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->evnt, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgDbgCntrl
*
*
*     Desc :   This structure holds MAC's Debug Control information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgDbgCntrl
(
RgDbgCntrl *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt32, param->dbgMask, mBuf);
   return ROK;
}

#ifdef PHY_ERROR_LOGING
/***********************************************************
*
*     Func : cmPkRgSchUlAllocCntrl
*
*
*     Desc :   This structure holds MAC's Debug Control information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSchUlAllocCntrl
(
RgSchUlAllocCntrl *param,
Buffer *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->mcs, mBuf);
   CMCHKPK(oduUnpackUInt16, param->numOfRb, mBuf);
   CMCHKPK(oduUnpackUInt16, param->rbStart, mBuf);
   CMCHKPK(oduUnpackUInt8, param->testStart, mBuf);
   CMCHKPK(oduUnpackUInt8, param->enaLog, mBuf);
   CMCHKPK(oduUnpackUInt16, param->logTime, mBuf);

   return ROK;
}

/***********************************************************
*
*     Func : cmUnpkRgSchUlAllocCntrl
*
*
*     Desc :   This structure holds MAC's Scheduler Configuration for Ul Allocation.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSchUlAllocCntrl
(
RgSchUlAllocCntrl *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt16, &param->logTime, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->enaLog, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->testStart, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->rbStart, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->numOfRb, mBuf);   
   CMCHKUNPK(oduPackUInt8, &param->mcs, mBuf);

   return ROK;
}


#endif

/***********************************************************
*
*     Func : cmUnpkRgDbgCntrl
*
*
*     Desc :   This structure holds MAC's Debug Control information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgDbgCntrl
(
RgDbgCntrl *param,
Buffer *mBuf
)
{

   CMCHKUNPK(oduPackUInt32, &param->dbgMask, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgSapCntrl
*
*
*     Desc :   This structure holds MAC's SAP Control information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgSapCntrl
(
RgSapCntrl *param,
Buffer *mBuf
)
{

   CMCHKPK(SPkS16, param->spId, mBuf);
   CMCHKPK(SPkS16, param->suId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgSapCntrl
*
*
*     Desc :   This structure holds MAC's SAP Control information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgSapCntrl
(
RgSapCntrl *param,
Buffer *mBuf
)
{

   CMCHKUNPK(SUnpkS16, &param->suId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->spId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgCntrl
*
*
*     Desc :   This structure holds MAC's Control information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgCntrl
(
RgCntrl *param,
S16 elmnt,
Buffer *mBuf
)
{


      switch(elmnt) {
         case STTFUSAP:
         case STRGUSAP:
         case STRGRSAP:
         case STCRGSAP:
            switch(param->subAction) {
               default :
                  CMCHKPK(cmPkRgSapCntrl, &param->s.rgSapCntrl, mBuf);
                  break;
            }
         break;
         case STGEN:
            switch(param->subAction) {
               case SATRC:
                  CMCHKPK(SPkS16, param->s.trcLen, mBuf);
                  break;
               case SADBG:
                  CMCHKPK(cmPkRgDbgCntrl, &param->s.rgDbgCntrl, mBuf);
                  break;
               case SAUSTA:
                  break;
#ifdef SS_DIAG
               case SALOG:
                  CMCHKPK(oduUnpackUInt32,  param->s.logMask, mBuf);
                  break;
#endif
#ifdef PHY_ERROR_LOGING
               case SAELMNT:
             CMCHKPK(cmPkRgSchUlAllocCntrl, &param->s.rgSchUlAllocCntrl, mBuf);
		  break;
#endif			   
               default :
                  break;
            }
            break;
         default :
            break;
      }
   CMCHKPK(oduUnpackUInt8, param->subAction, mBuf);
   CMCHKPK(oduUnpackUInt8, param->action, mBuf);
   CMCHKPK(cmPkDateTime, &param->dt, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgCntrl
*
*
*     Desc :   This structure holds MAC's Control information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgCntrl
(
RgCntrl *param,
S16 elmnt,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkDateTime, &param->dt, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->action, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->subAction, mBuf);
   switch(elmnt) {
      case STGEN:
	 switch(param->subAction) {
	    case SADBG:
	       CMCHKUNPK(cmUnpkRgDbgCntrl, &param->s.rgDbgCntrl, mBuf);
	       break;
	    case SATRC:
	       CMCHKUNPK(SUnpkS16, &param->s.trcLen, mBuf);
	       break;
	    case SAUSTA:
	       break;
#ifdef SS_DIAG
	    case SALOG:
	       CMCHKUNPK(oduPackUInt32, &param->s.logMask, mBuf);
	       break;
#endif
#ifdef PHY_ERROR_LOGING
	    case SAELMNT:
	       CMCHKUNPK(cmUnpkRgSchUlAllocCntrl, &param->s.rgSchUlAllocCntrl, mBuf);
	       break;
#endif			   
	    default :
	       break;
	 }
	 break;
      case STTFUSAP:
      case STRGUSAP:
      case STRGRSAP:
      case STCRGSAP:
	 switch(param->subAction) {
	    default :
	       CMCHKUNPK(cmUnpkRgSapCntrl, &param->s.rgSapCntrl, mBuf);
	       break;
	 }
	 break;
      default :
	 break;
      }
   return ROK;
}



/***********************************************************
*
*     Func : cmPkRgMngmt
*
*
*     Desc :   This structure holds MAC's Configuration and Control Management Information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmPkRgMngmt
(
Pst *pst,
RgMngmt *param,
uint8_t eventType,
Buffer *mBuf
)
{


      switch(eventType) {
         case EVTLRGCNTRLREQ:
         case  EVTLRGCNTRLCFM:
         case EVTLRGSCHCNTRLREQ:
         case  EVTLRGSCHCNTRLCFM:
            if(cmPkRgCntrl(&param->t.cntrl, param->hdr.elmId.elmnt, mBuf) != ROK)
               return RFAILED;
            break;
         case EVTLRGTRCIND:
            CMCHKPK(cmPkRgTrc, &param->t.trc, mBuf);
            break;
         case EVTLRGUSTAIND:
         case EVTLRGSCHSTAIND:
            CMCHKPK(cmPkRgUsta, &param->t.usta, mBuf);
            break;
         case EVTLRGSSTAREQ:
         case  EVTLRGSSTACFM:
            /*ccpu00118255 - ADD - eventType param */
#ifdef LRG_V1
            if(cmPkRgSsta(pst, &param->t.ssta, param->hdr.elmId.elmnt,
                     eventType, mBuf) != ROK)
               return RFAILED;
#else /* LRG_V1 is not defined */
            if(cmPkRgSsta(pst, &param->t.ssta, param->hdr.elmId.elmnt, mBuf) != ROK)
               return RFAILED;
#endif /* end of LRG_V1*/            
            break;
         case EVTLRGSTSREQ:
         case  EVTLRGSTSCFM:
            if(cmPkRgSts(&param->t.sts, param->hdr.elmId.elmnt, mBuf)!= ROK)
               return RFAILED;
            break;
         case EVTLRGCFGREQ:
         case  EVTLRGCFGCFM:
            if(cmPkRgCfg(&param->t.cfg, param->hdr.elmId.elmnt, mBuf) != ROK)
               return RFAILED;
            break;
         case EVTMACSCHGENCFGREQ:
         case  EVTMACSCHGENCFGCFM:
            if(cmPkRgCfg(&param->t.cfg, param->hdr.elmId.elmnt, mBuf) != ROK)
               return RFAILED;
            break;
         default :
            return RFAILED;
      }
   CMCHKPK(cmPkCmStatus, &param->cfm, mBuf);
   CMCHKPK(cmPkHeader, &param->hdr, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkRgMngmt
*
*
*     Desc :   This structure holds MAC's Configuration and Control Management Information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 cmUnpkRgMngmt
(
Pst *pst,
RgMngmt *param,
uint8_t eventType,
Buffer *mBuf
)
{


   CMCHKUNPK(cmUnpkHeader, &param->hdr, mBuf);
   CMCHKUNPK(cmUnpkCmStatus, &param->cfm, mBuf);
   switch(eventType) {
      case EVTLRGCFGREQ:
      case  EVTLRGCFGCFM:
      case EVTMACSCHGENCFGREQ:
      case  EVTMACSCHGENCFGCFM:
	 if(cmUnpkRgCfg(&param->t.cfg, param->hdr.elmId.elmnt, mBuf) != ROK)
	    return RFAILED;
	 break;
      case EVTLRGSTSREQ:
      case  EVTLRGSTSCFM:
	 if(cmUnpkRgSts(&param->t.sts, param->hdr.elmId.elmnt, mBuf) != ROK)
	    return RFAILED;
	 break;
      case EVTLRGSSTAREQ:
      case  EVTLRGSSTACFM:
	 if(cmUnpkRgSsta(pst, &param->t.ssta, param->hdr.elmId.elmnt, mBuf) != ROK)
	    return RFAILED;
	 break;
      case EVTLRGUSTAIND:
      case EVTLRGSCHSTAIND:
	 CMCHKUNPK(cmUnpkRgUsta, &param->t.usta, mBuf);
	 break;
      case EVTLRGTRCIND:
	 CMCHKUNPK(cmUnpkRgTrc, &param->t.trc, mBuf);
	 break;
      case EVTLRGCNTRLREQ:
      case  EVTLRGCNTRLCFM:
      case EVTLRGSCHCNTRLREQ:
      case  EVTLRGSCHCNTRLCFM:
	 if(cmUnpkRgCntrl(&param->t.cntrl, param->hdr.elmId.elmnt, mBuf) != ROK)
	    return RFAILED;
	 break;
      default :
	 return RFAILED;
      }
   return ROK;
}

/* lrg_c_001.main_3 - ADD -  Added the functions pertaining to LTE_L2_MEAS  */
#ifdef LTE_L2_MEAS

/**
* @brief This API is used to pack 
 LrgNmbActvUeQCI elements
*
* @details
*
*     Function: cmPkNmbActvUeQciReq
*
*  @param[in]   LrgNmbActvUeQCI
*  @param[in]   Buffer   *mBuf
*  @return   S16
*      -# ROK
**/
static S16  cmPkNmbActvUeQciReq
(
LrgNmbActvUeQCI   *nmbActvUeQci, 
Buffer        *mBuf
)
{
   uint8_t      idx;

   
   for(idx = 0; idx < nmbActvUeQci->numQci; idx++)
   {
       CMCHKPK(oduUnpackUInt8, nmbActvUeQci->qci[idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, nmbActvUeQci->numQci, mBuf);
   CMCHKPK(oduUnpackUInt8, nmbActvUeQci->sampPrd, mBuf);
   return ROK;
}

/**
* @brief This API is used to pack 
 LrgAvgPrbQci elements
*
* @details
*
*     Function: cmPkAvgPrbQciReq
*
*  @param[in]   LrgNmbActvUeQciReq  *avgPrbQciReq
*  @param[in]   Buffer   *mBuf
*  @return   S16
*      -# ROK
**/
static S16  cmPkAvgPrbQciReq
(
LrgAvgPrbQCI  *avgPrbQciReq,
Buffer        *mBuf
)
{
   uint8_t      idx;

   for(idx = 0; idx < avgPrbQciReq->numQci; idx++)
   {
       CMCHKPK(oduUnpackUInt8, avgPrbQciReq->qci[idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, avgPrbQciReq->numQci, mBuf);
   return ROK;

}

/**
* @brief This API is used to send a 
L2 Measurement Request from LM to MAC.
*
* @details
*
*     Function: cmPkLrgSchL2MeasReq
*
*  @param[in]   Pst *  pst
*  @param[in]   LrgSchMeasReqInfo  *  measInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchL2MeasReq
(
Pst * pst,
LrgSchMeasReqInfo  *measInfo
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG049, (ErrVal)0, "SGetMsg failed");
#endif
      return RFAILED;
   }
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
      CMCHKPK(cmPkNmbActvUeQciReq, &measInfo->nmbActvUeQciDl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
      CMCHKPK(cmPkNmbActvUeQciReq, &measInfo->nmbActvUeQciUl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
      CMCHKPK(cmPkAvgPrbQciReq, &measInfo->avgPrbQciDl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      CMCHKPK(cmPkAvgPrbQciReq, &measInfo->avgPrbQciUl, mBuf);
   }

   CMCHKPK(cmPkLteCellId, measInfo->cellId, mBuf);
   CMCHKPK(oduUnpackUInt32, measInfo->timePrd, mBuf);
   CMCHKPK(oduUnpackUInt16, measInfo->measType, mBuf);
   CMCHKPK(cmPkHeader, &measInfo->hdr, mBuf);
   pst->event = (Event) EVTLRGSCHL2MEASREQ;
   return (SPstTsk(pst,mBuf));
}
/**
* @brief This API is used to stop a 
L2 Measurement Request from LM to MAC.
*
* @details
*
*     Function: cmPkLrgSchL2MeasStopReq
*
*  @param[in]   Pst *  pst
*  @param[in]   LrgSchMeasReqInfo  *  measInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchL2MeasStopReq
(
Pst * pst,
LrgSchMeasStopReqInfo *measInfo
)
{
   Buffer *mBuf = NULLP;

 if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG049, (ErrVal)0, "SGetMsg failed");
#endif
      return RFAILED;
   }
   CMCHKPK(cmPkLteCellId, measInfo->cellId, mBuf);
   /*CMCHKPK(oduUnpackUInt16, measInfo->timePrd, mBuf);*/
   CMCHKPK(oduUnpackUInt16, measInfo->measType, mBuf);
   CMCHKPK(cmPkHeader, &measInfo->hdr, mBuf);
   pst->event = (Event) EVTLRGSCHL2MEASSTOPREQ;
   return (SPstTsk(pst,mBuf));
}/*cmPkLrgSchL2MeasStopReq*/

/**
* @brief This API is used to send a 
L2 Measurement Request from LM to MAC.
*
* @details
*
*     Function: cmPkLrgSchL2MeasSendReq
*
*  @param[in]   Pst *  pst
*  @param[in]   LrgSchMeasReqInfo  *  measInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchL2MeasSendReq
(
Pst * pst,
LrgSchMeasSndReqInfo *measInfo
)
{
   Buffer *mBuf = NULLP;
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG049, (ErrVal)0, "SGetMsg failed");
#endif
      return RFAILED;
   }
   CMCHKPK(cmPkLteCellId, measInfo->cellId, mBuf);
   CMCHKPK(oduUnpackUInt32, measInfo->timePrd, mBuf);
   CMCHKPK(oduUnpackUInt16, measInfo->measType, mBuf);
   CMCHKPK(cmPkHeader, &measInfo->hdr, mBuf);
   pst->event = (Event) EVTLRGSCHL2MEASSENDREQ;
   return (SPstTsk(pst,mBuf));
}/*cmPkLrgSchL2MeasSendReq*/

/**
* @brief This API is used to unpack AvgPrbQciReq
*
* @details
*
*     Function: cmUnpkNmbActvUeQciReq
*
*  @param[in]   LrgNmbActvUeQCI     *param
*  @param[in]   Buffer * mBuf
*  @return   S16
*      -# ROK
**/
static S16 cmUnpkNmbActvUeQciReq
(
LrgNmbActvUeQCI  *param,
Buffer           *mBuf
)
{
   uint8_t     idx;

   CMCHKUNPK(oduPackUInt8, &param->sampPrd, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->numQci, mBuf);
   for(idx = param->numQci; idx > 0; idx--)
   {
      CMCHKUNPK(oduPackUInt8, &param->qci[idx - 1], mBuf);
   }

   return ROK;
}

/**
* @brief This API is used to unpack AvgPrbQciReq
*
* @details
*
*     Function: cmUnpkAvgPrbQciReq
*
*  @param[in]   LrgAvgPrbQCI     *param
*  @param[in]   Buffer * mBuf
*  @return   S16
*      -# ROK
**/
static S16 cmUnpkAvgPrbQciReq
(
LrgAvgPrbQCI     *param,
Buffer           *mBuf
)
{
   uint8_t     idx;


   CMCHKUNPK(oduPackUInt8, &param->numQci, mBuf);
   for(idx = param->numQci; idx > 0; idx--)
   {
      CMCHKUNPK(oduPackUInt8, &param->qci[idx - 1], mBuf);
   }
   return ROK;

}

/**
* @brief This API is used to send a 
Measurement  Request from LM to SCH.
*
* @details
*
*     Function: cmUnpkLrgSchL2MeasReq
*
*  @param[in]   LrgSchMeasReq func
*  @param[in]   Pst *  pst
*  @param[in]   Buffer * mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchL2MeasReq
(
LrgSchL2MeasReq func,
Pst *pst,
Buffer *mBuf
)
{
   LrgSchMeasReqInfo  measInfo;


   CMCHKUNPK(cmUnpkHeader, &measInfo.hdr, mBuf);
   CMCHKUNPK(oduPackUInt16, &measInfo.measType, mBuf);
   CMCHKUNPK(oduPackUInt32, &measInfo.timePrd, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &measInfo.cellId, mBuf);
   if(measInfo.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      CMCHKUNPK(cmUnpkAvgPrbQciReq, &measInfo.avgPrbQciUl, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
      CMCHKUNPK(cmUnpkAvgPrbQciReq, &measInfo.avgPrbQciDl, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
      CMCHKUNPK(cmUnpkNmbActvUeQciReq, &measInfo.nmbActvUeQciUl, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
      CMCHKUNPK(cmUnpkNmbActvUeQciReq, &measInfo.nmbActvUeQciDl, mBuf);
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &measInfo));
}

/**
* @brief This API is used to stop  a 
Measurement  Request from LM to SCH.
*
* @details
*
*     Function: cmUnpkLrgSchL2MeasStopReq
*
*  @param[in]   LrgSchMeasStopReq func
*  @param[in]   Pst *  pst
*  @param[in]   Buffer * mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchL2MeasStopReq
(
LrgSchL2MeasStopReq func,
Pst *pst,
Buffer *mBuf
)
{
   LrgSchMeasStopReqInfo measInfo;

   CMCHKUNPK(cmUnpkHeader, &measInfo.hdr, mBuf);
   CMCHKUNPK(oduPackUInt16, &measInfo.measType, mBuf);
   /*CMCHKUNPK(oduPackUInt16, &measInfo.timePrd, mBuf);*/
   CMCHKUNPK(cmUnpkLteCellId, &measInfo.cellId, mBuf);
   SPutMsg(mBuf);
   return ((*func)(pst, &measInfo));
}/*cmUnpkLrgSchL2MeasStopReq*/

/**
* @brief This API is used to send a 
Measurement  Request from LM to SCH.
*
* @details
*
*     Function: cmUnpkLrgSchL2MeasSendReq
*
*  @param[in]   LrgSchMeasSendReq func
*  @param[in]   Pst *  pst
*  @param[in]   Buffer * mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchL2MeasSendReq
(
LrgSchL2MeasSendReq func,
Pst *pst,
Buffer *mBuf
)
{
   LrgSchMeasSndReqInfo measInfo;
   CMCHKUNPK(cmUnpkHeader, &measInfo.hdr, mBuf);
   CMCHKUNPK(oduPackUInt16, &measInfo.measType, mBuf);
   CMCHKUNPK(oduPackUInt32, &measInfo.timePrd, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &measInfo.cellId, mBuf);
   SPutMsg(mBuf);
   return ((*func)(pst, &measInfo));
}/*cmUnpkLrgSchL2MeasSendReq*/

/**
* @brief This API is used to stop a 
L2 Measurement confirm from MAC to LM
*
* @details
*
*     Function: cmPkLrgSchL2MeasStopCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   LrgSchMeasCfmInfo  *  measInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchL2MeasStopCfm
(
Pst * pst,
LrgSchMeasCfmInfo  *measInfo
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG050, (ErrVal)0, "SGetMsg failed");
#endif
      return RFAILED;
   }
   CMCHKPK(cmPkLteCellId, measInfo->cellId, mBuf);
   CMCHKPK(cmPkCmStatus, &measInfo->cfm, mBuf);
   CMCHKPK(oduUnpackUInt16, measInfo->measType, mBuf);
   CMCHKPK(cmPkHeader, &measInfo->hdr, mBuf);
   pst->event = (Event) EVTLRGSCHL2MEASSTOPCFM;
   return (SPstTsk(pst,mBuf));
}/*cmPkLrgSchL2MeasStopCfm*/
/**
* @brief This API is used to Send a 
Measurement  Confirm from SCH to LM.
*
* @details
*
*     Function: cmUnpkLrgL2SchMeasCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   Buffer * mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchL2MeasStopCfm
(
LrgSchL2MeasStopCfm func,
Pst *pst,
Buffer *mBuf
)
{
   LrgSchMeasCfmInfo  measInfo;


   CMCHKUNPK(cmUnpkHeader, &measInfo.hdr, mBuf);
   CMCHKUNPK(oduPackUInt16, &measInfo.measType, mBuf);
   CMCHKUNPK(cmUnpkCmStatus, &measInfo.cfm, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &measInfo.cellId, mBuf);

   SPutMsg(mBuf);
   return ((*func)(pst, &measInfo));
}/*cmUnpkLrgSchL2MeasStopCfm*/

/**
* @brief This API is used to unpack LrgNumActvUeQCICfm structure
*
* @details
*
*     Function: cmPkNumUeQciCfm 
*
*  @param[in]   LrgNumActvUeQCICfm *  param
*  @param[in]   Buffer *  mBuf
*  @return   S16
*      -# ROK
**/
static S16 cmPkNumUeQciCfm
(
LrgNumActvUeQCICfm  *param,
Buffer              *mBuf
)
{
   uint8_t         idx;

   
   for(idx = 0; idx < param->numQci; idx++)
   {
/*LRG : Review Tag*/
     CMCHKPK(oduUnpackUInt8, param->numActvUeQci[idx].qciValue, mBuf);
     CMCHKPK(oduUnpackUInt8, param->numActvUeQci[idx].numActvUeQci, mBuf);
/*LRG : Review Tag*/
   }
   CMCHKPK(oduUnpackUInt8, param->numQci, mBuf);
   return ROK;
}

/**
* @brief This API is used to unpack LrgAvgPrbQCICfm structure
*
* @details
*
*     Function: cmPkAvgPrbQciCfm
*
*  @param[in]   LrgAvgPrbQCICfm *  param
*  @param[in]   Buffer *  mBuf
*  @return   S16
*      -# ROK
**/
static S16 cmPkAvgPrbQciCfm
(
LrgAvgPrbQCICfm    *param,
Buffer             *mBuf
)
{
   uint8_t         idx;

   for(idx = 0; idx < param->numQci; idx++)
   {
/*LRG : Review Tag*/
      CMCHKPK(oduUnpackUInt8, param->prbPercQci[idx].qciValue, mBuf);
      CMCHKPK(oduUnpackUInt8, param->prbPercQci[idx].prbPercQci, mBuf);
/*LRG : Review Tag*/
   } 
   CMCHKPK(oduUnpackUInt8, param->numQci, mBuf);
   return ROK;
}

/**
* @brief This API is used to unpack raPreamblesCfm structure
*
* @details
*
*     Function: cmPkRaPrmbsCfm
*
*  @param[in]   LrgRaPreamblesCfm *  param
*  @param[in]   Buffer *  mBuf
*  @return   S16
*      -# ROK
**/
static S16 cmPkRaPrmbsCfm
(
LrgRaPreamblesCfm  *param,
Buffer             *mBuf
)
{

   CMCHKPK(oduUnpackUInt16, param->randSelPreHighRange, mBuf);
   CMCHKPK(oduUnpackUInt16, param->randSelPreLowRange, mBuf);
   CMCHKPK(oduUnpackUInt16, param->dedPreambles, mBuf);
   return ROK;
}

/**
* @brief This API is used to unpack avgPrbCfm structure
*
* @details
*
*     Function: cmPkAvgPrbCfm
*
*  @param[in]   LrgAvgPrbCfm *  param
*  @param[in]   Buffer *  mBuf
*  @return   S16
*      -# ROK
**/
static S16 cmPkAvgPrbCfm 
(
LrgAvgPrbCfm  *param,
Buffer        *mBuf
)
{

   CMCHKPK(oduUnpackUInt8, param->prbPerc, mBuf);
   return ROK;
}

/**
* @brief This API is used to send a 
L2 Measurement confirm from MAC to LM
*
* @details
*
*     Function: cmPkLrgSchL2MeasCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   LrgSchMeasCfmInfo  *  measInfo
*  @return   S16
*      -# ROK
**/
S16 cmPkLrgSchL2MeasCfm
(
Pst * pst,
LrgSchMeasCfmInfo  *measInfo
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELRG050, (ErrVal)0, "SGetMsg failed");
#endif
      return RFAILED;
   }
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
     CMCHKPK(cmPkNumUeQciCfm, &measInfo->numUeQciDlCfm, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
     CMCHKPK(cmPkNumUeQciCfm, &measInfo->numUeQciUlCfm, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_RA_PREAMBLE)
   {
     CMCHKPK(cmPkRaPrmbsCfm, &measInfo->raPrmbsCfm, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
     CMCHKPK(cmPkAvgPrbQciCfm, &measInfo->avgPrbQciDlCfm, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
     CMCHKPK(cmPkAvgPrbQciCfm, &measInfo->avgPrbQciUlCfm, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_DL)
   {
     CMCHKPK(cmPkAvgPrbCfm, &measInfo->avgPrbDl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_UL)
   {
     CMCHKPK(cmPkAvgPrbCfm, &measInfo->avgPrbUl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_TB_TRANS_DL_COUNT)
   {
      CMCHKPK(oduUnpackUInt32, measInfo->tbTransDlTotalCnt, mBuf);
   }   
   if(measInfo->measType & LRG_L2MEAS_TB_TRANS_DL_FAULTY_COUNT)
   {
      CMCHKPK(oduUnpackUInt32, measInfo->tbTransDlFaulty, mBuf);
   }   
   if(measInfo->measType & LRG_L2MEAS_TB_TRANS_UL_COUNT)
   {
      CMCHKPK(oduUnpackUInt32, measInfo->tbTransUlTotalCnt, mBuf);
   }   
   if(measInfo->measType & LRG_L2MEAS_TB_TRANS_UL_FAULTY_COUNT)
   {
      CMCHKPK(oduUnpackUInt32, measInfo->tbTransUlFaulty, mBuf);
   }   
   CMCHKPK(cmPkLteCellId, measInfo->cellId, mBuf);
   CMCHKPK(cmPkCmStatus, &measInfo->cfm, mBuf);
   CMCHKPK(oduUnpackUInt16, measInfo->measType, mBuf);
   CMCHKPK(cmPkHeader, &measInfo->hdr, mBuf);
   pst->event = (Event) EVTLRGSCHL2MEASCFM;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief This API is used to unpack LrgNumActvUeQCICfm
*
* @details
*
*     Function: cmUnpkNumUeQciCfm
*
*  @param[in]   LrgNumActvUeQCICfm *param
*  @param[in]   Buffer * mBuf
*  @return   Void
*      -# ROK
**/
static S16 cmUnpkNumUeQciCfm
(
LrgNumActvUeQCICfm *param,
Buffer          *mBuf
)
{
   uint8_t          idx;

   CMCHKUNPK(oduPackUInt8, &param->numQci, mBuf);
   for(idx = param->numQci; idx > 0; idx--)
   {
/*LRG : Review Tag*/
       CMCHKUNPK(oduPackUInt8, &param->numActvUeQci[idx - 1].numActvUeQci, mBuf);
       CMCHKUNPK(oduPackUInt8, &param->numActvUeQci[idx - 1].qciValue, mBuf);
/*LRG : Review Tag*/
   }
   return ROK;
}

/**
* @brief This API is used to unpack LrgAvgPrbQCICfm
*
* @details
*
*     Function: cmUnpkAvgPrbQciCfm
*
*  @param[in]   LrgAvgPrbQCICfm *param
*  @param[in]   Buffer * mBuf
*  @return  Void 
*      -# ROK
**/
static S16 cmUnpkAvgPrbQciCfm
(
LrgAvgPrbQCICfm *param,
Buffer          *mBuf
)
{
   uint8_t          idx;


   CMCHKUNPK(oduPackUInt8, &param->numQci, mBuf);
   for(idx = param->numQci; idx > 0; idx--)
   {
/*LRG : Review Tag*/
      CMCHKUNPK(oduPackUInt8, &param->prbPercQci[idx - 1].prbPercQci, mBuf);
      CMCHKUNPK(oduPackUInt8, &param->prbPercQci[idx - 1].qciValue, mBuf);
/*LRG : Review Tag*/
   }
   return ROK;
}

/**
* @brief This API is used to unpack LrgRaPreamblesCfm
*
* @details
*
*     Function: cmUnpkRaPrmbsCfm
*
*  @param[in]   LrgRaPreamblesCfm *param
*  @param[in]   Buffer * mBuf
*  @return   Void
*      -# ROK
**/
static S16 cmUnpkRaPrmbsCfm
(
LrgRaPreamblesCfm *param,
Buffer         *mBuf
)
{


   CMCHKUNPK(oduPackUInt16, &param->dedPreambles, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->randSelPreLowRange, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->randSelPreHighRange, mBuf);
   return ROK;
}

/**
* @brief This API is used to unpack avgPrbCfm
*
* @details
*
*     Function: cmUnpkAvgPrbCfm
*
*  @param[in]   LrgAvgPrbCfm  *param
*  @param[in]   Buffer * mBuf
*  @return   Void
*      -# ROK
**/
static S16 cmUnpkAvgPrbCfm
(
LrgAvgPrbCfm   *param, 
Buffer         *mBuf
)
{


   CMCHKUNPK(oduPackUInt8, &param->prbPerc, mBuf);
   return ROK;
}

/**
* @brief This API is used to send a 
Measurement  Confirm from LM to SCH.
*
* @details
*
*     Function: cmUnpkLrgL2SchMeasCfm
*
*  @param[in]   Pst *  pst
*  @param[in]   Buffer * mBuf
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLrgSchL2MeasCfm
(
LrgSchL2MeasCfm func,
Pst *pst,
Buffer *mBuf
)
{
   LrgSchMeasCfmInfo  measInfo;


   CMCHKUNPK(cmUnpkHeader, &measInfo.hdr, mBuf);
   CMCHKUNPK(oduPackUInt16, &measInfo.measType, mBuf);
   CMCHKUNPK(cmUnpkCmStatus, &measInfo.cfm, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &measInfo.cellId, mBuf);
   if(measInfo.measType & LRG_L2MEAS_TB_TRANS_UL_FAULTY_COUNT)
   {
      CMCHKPK(oduPackUInt32, &measInfo.tbTransUlFaulty, mBuf);
   }    
   if(measInfo.measType & LRG_L2MEAS_TB_TRANS_UL_COUNT)
   {
      CMCHKPK(oduPackUInt32, &measInfo.tbTransUlTotalCnt, mBuf);
   }   
   if(measInfo.measType & LRG_L2MEAS_TB_TRANS_DL_FAULTY_COUNT)
   {
      CMCHKPK(oduPackUInt32, &measInfo.tbTransDlFaulty, mBuf);
   }   
   if(measInfo.measType & LRG_L2MEAS_TB_TRANS_DL_COUNT)
   {
      CMCHKPK(oduPackUInt32, &measInfo.tbTransDlTotalCnt, mBuf);
   }   
   if(measInfo.measType & LRG_L2MEAS_AVG_PRB_UL)
   {
     CMCHKUNPK(cmUnpkAvgPrbCfm, &measInfo.avgPrbUl, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_AVG_PRB_DL)
   {
     CMCHKUNPK(cmUnpkAvgPrbCfm, &measInfo.avgPrbDl, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
     CMCHKUNPK(cmUnpkAvgPrbQciCfm, &measInfo.avgPrbQciUlCfm, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
     CMCHKUNPK(cmUnpkAvgPrbQciCfm, &measInfo.avgPrbQciDlCfm, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_RA_PREAMBLE)
   {
     CMCHKUNPK(cmUnpkRaPrmbsCfm, &measInfo.raPrmbsCfm, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
     CMCHKUNPK(cmUnpkNumUeQciCfm, &measInfo.numUeQciUlCfm, mBuf);
   }
   if(measInfo.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
     CMCHKUNPK(cmUnpkNumUeQciCfm, &measInfo.numUeQciDlCfm, mBuf);
   }
   SPutMsg(mBuf);
   return ((*func)(pst, &measInfo));
}
#endif
#endif


/**********************************************************************
         End of file
 **********************************************************************/
