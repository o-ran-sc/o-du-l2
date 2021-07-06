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

#ifdef RGM_LWLC


/**
* @brief Request from RRM to MAC to bind the interface saps
*
* @details
*
*     Function : cmPkLwLcRgmBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmPkLwLcRgmBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }

   if (SPkS16(spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGMBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RRM to MAC to bind the interface saps
*
* @details
*
*     Function : cmUnpkLwLcRgmBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLwLcRgmBndReq
(
RgmBndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   SpId spId;
   S16  ret;
   

   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SUnpkS16(&spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   ret = ((*func)(pst, suId, spId));

   SPutMsg(mBuf);

   return (ret);
}

/**
* @brief Request from RRM to MAC to Unbind the interface saps
*
* @details
*
*     Function : cmPkLwLcRgmUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmPkLwLcRgmUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   if (SPkS16(reason, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGMUBNDREQ;
   return (SPstTsk(pst,mBuf));
}

/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmPkLwLcRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkLwLcRgmBndCfm
(
Pst* pst,
SuId suId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (oduUnpackUInt8(status, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

  pst->event = (Event) EVTRGMBNDCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmUnpkLwLcRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLwLcRgmBndCfm
(
RgmBndCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   uint8_t status;
   

   if (oduPackUInt8(&status, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, status));
}


/**
* @brief Configure the PRB Report Preparation Start/Stop from RRM to MAC 
*
* @details
*
*     Function : cmPkLwLcRgmCfgPrbRprt
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RgmPrbRprtCfg  *  prbRprtCfg
*  @return   S16
*      -# ROK
**/
S16 cmPkLwLcRgmCfgPrbRprt
(
Pst* pst,
SpId spId,
RgmPrbRprtCfg  * prbRprtCfg
)
{
   Buffer *mBuf = NULLP;
   uint32_t len = sizeof(RgmPrbRprtCfg);

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtCfg, sizeof(RgmPrbRprtCfg));
      return RFAILED;
   }

    if(oduPackPointer((PTR)prbRprtCfg, mBuf) != ROK)
    {
       SPutMsg(mBuf); 
       SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data*)prbRprtCfg, len);
       return RFAILED;
    }

   if (SPkS16(spId, mBuf) != ROK) 
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtCfg, sizeof(RgmPrbRprtCfg));
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGMCFGPRBRPRT;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Configure the PRB Report Preparation Start/Stop from RRM to MAC 
*
* @details
*
*     Function : cmUnpkLwLcRgmCfgPrbRprt
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RgmPrbRprtCfg  *  prbRprtCfg
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLwLcRgmCfgPrbRprt
(
RgmCfgPrbRprtFptr func,
Pst *pst,
Buffer *mBuf
)
{
   S16 ret;
   SpId spId;
   RgmPrbRprtCfg *prbRprtCfg;
   

   if (SUnpkS16(&spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (oduUnpackPointer((PTR *)&prbRprtCfg, mBuf) != ROK)
   {
      SPutMsg(mBuf);
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtCfg, sizeof(RgmPrbRprtCfg));
      return RFAILED;
   }

   ret =  ((*func)(pst, spId, prbRprtCfg));

   SPutMsg(mBuf);
   return (ret);
}


/**
* @brief PRB Usage Report Indication  from MAC to RRM 
*
* @details
*
*     Function : cmPkLwLcRgmPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmPrbRprtInd  *  prbRprtInd
*  @return   S16
*      -# ROK
**/
S16 cmPkLwLcRgmPrbRprtInd
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

   if(oduPackPointer((PTR)prbRprtInd, mBuf) != ROK)
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

   pst->event = (Event) EVTRGMCFGPRBRPRT;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief PRB Usage Report Indication from MAC to RRM 
*
* @details
*
*     Function : cmUnpkLwLcRgmPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmPrbRprtInd  *  prbRprtInd
*  @return   S16
*      -# ROK
**/
S16 cmUnpkLwLcRgmPrbRprtInd
(
RgmPrbRprtIndFptr func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   RgmPrbRprtInd *prbRprtInd;
   S16 ret;
   

   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (oduUnpackPointer((PTR *)&prbRprtInd, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtInd, sizeof(RgmPrbRprtInd));
      return RFAILED;
   }


   ret = ((*func)(pst, suId, prbRprtInd));

   SPutMsg(mBuf);

   return (ret);
}
#endif


/**
* @brief Request from RRM to MAC to bind the interface saps
*
* @details
*
*     Function : cmPkRgmBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmPkRgmBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }

   if (SPkS16(spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGMBNDREQ;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Request from RRM to MAC to bind the interface saps
*
* @details
*
*     Function : cmUnpkRgmBndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   SpId  spId
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRgmBndReq
(
RgmBndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   SpId spId;
   S16  ret;
   

   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SUnpkS16(&spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   ret = ((*func)(pst, suId, spId));

   SPutMsg(mBuf);

   return (ret);
}

/**
* @brief Request from RRM to MAC to Unbind the interface saps
*
* @details
*
*     Function : cmPkRgmUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmPkRgmUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }
   if (SPkS16(reason, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SPkS16(spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   pst->event = (Event) EVTRGMUBNDREQ;
   return (SPstTsk(pst,mBuf));
}

#if 1 

/**
* @brief Request from RRM to MAC to Unbind the interface saps
*
* @details
*
*     Function : cmUnpkRgmUbndReq
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   Reason  reason
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRgmUbndReq
(
RgmUbndReq func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   Reason reason;
   

   if (SUnpkS16(&spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }
   if (SUnpkS16(&reason, mBuf) != ROK)
   {
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, spId, reason));
}
#endif

/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmPkRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmPkRgmBndCfm
(
Pst* pst,
SuId suId,
uint8_t status
)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      return RFAILED;
   }

   if (SPkS16(suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (oduUnpackUInt8(status, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

  pst->event = (Event) EVTRGMBNDCFM;
   return (SPstTsk(pst,mBuf));
}


/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : cmUnpkRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   uint8_t  status
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRgmBndCfm
(
RgmBndCfm func,
Pst *pst,
Buffer *mBuf
)
{
   SuId suId;
   uint8_t status;
   

   if (oduPackUInt8(&status, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (SUnpkS16(&suId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }
   SPutMsg(mBuf);
   return ((*func)(pst, suId, status));
}




/**
* @brief Configure the PRB Report Preparation Start/Stop from RRM to MAC 
*
* @details
*
*     Function : cmPkCfgPrbRprt
*
*  @param[in]   RgmPrbRprtCfg  *prbRprtCfg
*  @param[in]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmPkCfgPrbRprt
(
RgmPrbRprtCfg  * prbRprtCfg,
Buffer *mBuf
)
{
   CMCHKPK(oduUnpackUInt16, prbRprtCfg->usPrbAvgPeriodicty, mBuf);
   CMCHKPK(oduUnpackUInt8, prbRprtCfg->bConfigType, mBuf);
   CMCHKPK(oduUnpackUInt8, prbRprtCfg->bCellId, mBuf);
   return ROK;
}
/**
* @brief Configure the PRB Report Preparation Start/Stop from RRM to MAC 
*
* @details
*
*     Function : cmUnPkCfgPrbRprt
*
*  @param[in]   RgmPrbRprtCfg  *prbRprtCfg
*  @param[in]   Buffer *mBuf 
*  @return   S16
*      -# ROK
**/
S16 cmUnPkCfgPrbRprt
(
RgmPrbRprtCfg  * prbRprtCfg,
Buffer *mBuf
)
{
   CMCHKUNPK(oduPackUInt8, &prbRprtCfg->bCellId, mBuf);
   CMCHKUNPK(oduPackUInt8, &prbRprtCfg->bConfigType, mBuf);
   CMCHKUNPK(oduPackUInt16, &prbRprtCfg->usPrbAvgPeriodicty, mBuf);
   return ROK;
}




/**
* @brief Configure the PRB Report Preparation Start/Stop from RRM to MAC 
*
* @details
*
*     Function : cmPkRgmCfgPrbRprt
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RgmPrbRprtCfg  *prbRprtCfg
*  @return   S16
*      -# ROK
**/
S16 cmPkRgmCfgPrbRprt
(
Pst* pst,
SpId spId,
RgmPrbRprtCfg  * prbRprtCfg
)
{
   Buffer *mBuf = NULLP;
   uint32_t len = sizeof(RgmPrbRprtCfg);

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtCfg, sizeof(RgmPrbRprtCfg));
      return RFAILED;
   }

    if(cmPkCfgPrbRprt(prbRprtCfg, mBuf) != ROK)
    {
       SPutMsg(mBuf); 
       SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data*)prbRprtCfg, len);
       return RFAILED;
    }

   if (SPkS16(spId, mBuf) != ROK) 
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtCfg, sizeof(RgmPrbRprtCfg));
      SPutMsg(mBuf);
      return RFAILED;
   }

   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data *)prbRprtCfg, sizeof(RgmPrbRprtCfg));

   pst->event = (Event) EVTRGMCFGPRBRPRT;
   return (SPstTsk(pst,mBuf));
}



/**
* @brief Configure the PRB Report Preparation Start/Stop from RRM to MAC 
*
* @details
*
*     Function : cmUnpkRgmCfgPrbRprt
*
*  @param[in]   Pst*  pst
*  @param[in]   SpId  spId
*  @param[in]   RgmPrbRprtCfg  *  prbRprtCfg
*  @return   S16
*      -# ROK
**/
S16 cmUnpkRgmCfgPrbRprt
(
RgmCfgPrbRprtFptr func,
Pst *pst,
Buffer *mBuf
)
{
   SpId spId;
   RgmPrbRprtCfg *prbRprtCfg;

   if ((SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,pst->region, pst->pool, (Data **)&prbRprtCfg, sizeof(RgmPrbRprtCfg))) != ROK)
      {
         SPutMsg(mBuf);
         return RFAILED;
      }

   if (SUnpkS16(&spId, mBuf) != ROK) 
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   if (cmUnPkCfgPrbRprt(prbRprtCfg, mBuf) != ROK)
   {
      SPutMsg(mBuf);
      return RFAILED;
   }

   SPutMsg(mBuf);
   return ((*func)(pst, spId, prbRprtCfg));
}

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
