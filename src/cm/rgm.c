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
  
     Desc:     C source code for packing/unpacking of RGM interface
               primitives.
  
     File:     rgm.c 
  
**********************************************************************/

/** @file rgm.c
@brief This file contains the packing/unpacking code for the RGM interface 
       primitives.
*/

/* header include files (.h) */
#include "common_def.h"
#include "rgm.h"           /* RGM Interface defines */
/* header/extern include files (.x) */
#include "rgm.x"           /* RGM Interface includes */

/* RRM_SP1_START */
/**
* @brief Packing of PRB Usage Report for qci for sending 
*        to RRM from MAC 
*
* @details
*
*     Function : cmPkRgmPrbQciRpt 
*
*  @param[in]   RgmPrbRptPerQci * qciPrbRprt 
*  @param[out]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmPkRgmPrbQciRpt 
(
RgmPrbRptPerQci *qciPrbRprt,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt8, qciPrbRprt->bQci, mBuf);
   CMCHKPK(oduUnpackUInt8, qciPrbRprt->bAvgPrbUlUsage, mBuf);
   CMCHKPK(oduUnpackUInt8, qciPrbRprt->bAvgPrbDlUsage, mBuf);

   return ROK;
}

/**
* @brief Unpacking of PRB Usage Report for qci received from MAC 
*
* @details
*
*     Function : cmUnpkRgmPrbQciRpt 
*
*  @param[out]   RgmPrbRptPerQci * qciPrbRprt 
*  @param[in]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRgmPrbQciRpt 
(
RgmPrbRptPerQci *qciPrbRprt,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8, &qciPrbRprt->bAvgPrbDlUsage, mBuf);
   CMCHKUNPK(oduPackUInt8, &qciPrbRprt->bAvgPrbUlUsage, mBuf);
   CMCHKUNPK(oduPackUInt8, &qciPrbRprt->bQci, mBuf);

   return ROK;
}
/* RRM_SP1_END */

/**
* @brief PRB Usage Report Indication  from MAC to RRM 
*
* @details
*
*     Function : cmPkPrbRprtInd
*
*  @param[in]   RgmPrbRprtInd  *  prbRprtInd
*  @param[in]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmPkPrbRprtInd
(
RgmPrbRprtInd  * prbRprtInd,
Buffer *mBuf
)
{
   S32 idx = 0;

   
   /* RRM_SP1_START */
   for(idx = RGM_MAX_QCI_REPORTS-1; idx >= 0; idx--)
   {
      CMCHKPK(cmPkRgmPrbQciRpt, &prbRprtInd->stQciPrbRpts[idx], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, prbRprtInd->bPrbUsageMask, mBuf);
   CMCHKPK(oduUnpackUInt8, prbRprtInd->bCellId, mBuf);
   /* RRM_SP1_END */
   return ROK;
}

/**
* @brief PRB Usage Report Indication  from MAC to RRM 
*
* @details
*
*     Function : cmUnpkPrbRprtInd
*
*  @param[in]   RgmPrbRprtInd  *  prbRprtInd
*  @param[in]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmUnpkPrbRprtInd
(
RgmPrbRprtInd  * prbRprtInd,
Buffer *mBuf
)
{
   uint32_t idx = 0;


   /* RRM_SP1_START */
   CMCHKUNPK(oduPackUInt8, &prbRprtInd->bCellId, mBuf);
   CMCHKUNPK(oduPackUInt8, &prbRprtInd->bPrbUsageMask, mBuf);
   for(idx = 0; idx < RGM_MAX_QCI_REPORTS; idx++)
   {
      CMCHKUNPK(cmUnpkRgmPrbQciRpt, &prbRprtInd->stQciPrbRpts[idx], mBuf);
   }

   /* RRM_SP1_END */
   return ROK;
}



/**
* @brief PRB Usage Report Indication  from MAC to RRM 
*
* @details
*
*     Function : cmPkRgmPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmPrbRprtInd  *  prbRprtInd
*  @return   S16
*      -# ROK
**/
S16 cmPkRgmPrbRprtInd
(
Pst* pst,
SuId suId,
RgmPrbRprtInd  * prbRprtInd
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtInd, sizeof(RgmPrbRprtInd));
      return RFAILED;
   }

   if(cmPkPrbRprtInd(prbRprtInd, mBuf) != ROK)
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtInd, sizeof(RgmPrbRprtInd));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK) 
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtInd, sizeof(RgmPrbRprtInd));
      SPutMsg(mBuf);
      return RFAILED;
   }

   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtInd, sizeof(RgmPrbRprtInd));

   pst->event = (Event) EVTRGMPRBRPRTIND;
   return (SPstTsk(pst,mBuf));
}
/**
* @brief PRB Usage Report Indication from MAC to RRM 
*
* @details
*
*     Function : cmUnpkRgmPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmPrbRprtInd  *  prbRprtInd
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRgmPrbRprtInd
(
RgmPrbRprtIndFptr func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgmPrbRprtInd prbRprtInd;
   

   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (cmUnpkPrbRprtInd(&prbRprtInd, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   SPutMsg(mBuf);
   return ((*func)(pst, suId, &prbRprtInd));
}

/**
* @brief Transmission Mode Change Indication  from MAC to RRM 
*
* @details
*
*     Function : cmPkTransModeInd
*
*  @param[in]   RgmTransModeInd *transModeInd
*  @param[in]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmPkTransModeInd
(
RgmTransModeInd *transModeInd,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt32, transModeInd->eMode, mBuf);
   CMCHKPK(oduUnpackUInt16, transModeInd->usCrnti, mBuf);
   CMCHKPK(oduUnpackUInt8, transModeInd->bCellId, mBuf);
   return ROK;
}

/**
* @brief  Transmission Mode Change Indication from MAC to RRM 
*
* @details
*
*     Function : cmUnpkTransModeInd
*
*  @param[in]   RgmTransModeInd *transModeInd 
*  @param[in]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmUnpkTransModeInd
(
RgmTransModeInd *transModeInd,
Buffer *mBuf
)
{
   uint32_t tmpModeEnum;
   CMCHKUNPK(oduPackUInt8, &transModeInd->bCellId, mBuf);
   CMCHKUNPK(oduPackUInt16, &transModeInd->usCrnti, mBuf);
   CMCHKUNPK(oduPackUInt32, (uint32_t *)&tmpModeEnum, mBuf);
   transModeInd->eMode = (RgmTxnMode)tmpModeEnum;
   return ROK;
}
/**
* @brief Transmission Mode Change Indication  from MAC to RRM 
*
* @details
*
*     Function : cmPkRgmTransModeInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmTransModeInd *transModeInd 
*  @return   S16
*      -# ROK
**/

S16 cmPkRgmTransModeInd 
(
Pst* pst,
SuId suId,
RgmTransModeInd *transModeInd
)
{
   Buffer *mBuf = NULLP;


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)transModeInd, sizeof(RgmTransModeInd));
      return RFAILED;
   }

   if(cmPkTransModeInd(transModeInd, mBuf) != ROK)
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)transModeInd, sizeof(RgmTransModeInd));
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK) 
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)transModeInd, sizeof(RgmTransModeInd));
      SPutMsg(mBuf);
      return RFAILED;
   }

   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)transModeInd, sizeof(RgmTransModeInd));

   pst->event = (Event) EVTRGMTRANSMODEIND;
   return (SPstTsk(pst,mBuf));
}
/**
* @brief Transmission Mode Change Indication  from MAC to RRM 
*
* @details
*
*     Function : cmUnpkRgmTransModeInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmTransModeInd *transModeInd 
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRgmTransModeInd
(
RgmTransModeIndFptr func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgmTransModeInd transModeInd;
   

   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (cmUnpkTransModeInd(&transModeInd, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   SPutMsg(mBuf);
   return ((*func)(pst, suId, &transModeInd));
}
/**********************************************************************
 
         End of file
 
**********************************************************************/
