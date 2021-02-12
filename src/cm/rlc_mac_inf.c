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
/* This file define APIs at RLC-MAC interface */
#include "common_def.h"
#include "rlc_mac_inf.h"

/**
 * @brief Primitive invoked from RLC to MAC to 
 * inform the BO report for dedicated channels
 *
 * @details
 *
 *     Function : packBOStatus,
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   RlcMacBOStatus  *  staRsp
 *  @return   S16
 *      -# ROK
 **/
uint8_t packRlcBoStatus(Pst* pst, RlcBoStatus  *boStatus)
{
   Buffer *mBuf = NULLP;
 
   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nERROR  -->  RLC: Memory allocation failed at packBOStatus");
      return RFAILED;
   }

   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      /* light weight loose coupling */
      CMCHKPK(oduPackPointer,(PTR) boStatus, mBuf);
   }
   else
   {
      /* light coupling */
      return RFAILED;
   }
 
   return ODU_POST_TASK(pst,mBuf);
}

/**
 * @brief Primitive invoked from RLC to MAC to 
 * inform the BO report for dedicated channels
 *
 * @details
 *
 *     Function : unpackBOStatus
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   RlcMacBOStatus *  staRsp
 *  @return   S16
 *      -# ROK
 **/
uint8_t unpackRlcBoStatus(RlcMacBoStatusFunc func, Pst *pst, Buffer *mBuf)
{
   RlcBoStatus *boSta;
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
       /* Light weight loose coupling */
       CMCHKUNPK(oduUnpackPointer,(PTR *) &boSta, mBuf);
   }
   else
   {
      /* Light Coupling */
      return RFAILED;
   }
   ODU_PUT_MSG_BUF(mBuf);
   return (*func)(pst, boSta);
}

/**
 * @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
 *
 * @details
 *
 *     Function : packSchedRep
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RlcMacSchedRep*  schRep
 *  @return   S16
 *      -# ROK
 **/
uint8_t packRlcSchedResultRpt(Pst* pst, RlcSchedResultRpt *schRep)
{
   Buffer *mBuf = NULLP;
 
   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed at packSchResultRep");
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKPK(oduPackPointer,(PTR) schRep, mBuf);
   }
   else
   {
      return RFAILED;
   }
 
   return ODU_POST_TASK(pst,mBuf);
}

/**
 * @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
 *
 * @details
 *
 *     Function : unpackSchResultRpt
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RguDStaIndInfo  *  staInd
 *  @return   S16
 *      -# ROK
 **/
uint8_t unpackSchedResultRpt(RlcMacSchedResultRptFunc func, Pst *pst, Buffer *mBuf)
{
   RlcSchedResultRpt *schRep;
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(oduUnpackPointer,(PTR *) &schRep, mBuf);
   }
   else
   {
      return RFAILED;
   }
   ODU_PUT_MSG_BUF(mBuf);
   return (*func)(pst, schRep);
}

/*******************************************************************
 *
 * @brief Pack and send DL Data from RLC to MAC
 *
 * @details
 *
 *    Function : packRlcDlData
 *
 *    Functionality: Pack and send DL Data from RLC to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcDlData(Pst* pst, RlcData *dlData)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nERROR  -->  RLC : Memory allocation failed at packRlcDlData");
      return RFAILED;
   }
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
       CMCHKPK(oduPackPointer,(PTR) dlData, mBuf);
   }
   else
   {
      return RFAILED;
   }
   return ODU_POST_TASK(pst,mBuf);

}

/*******************************************************************
 *
 * @brief unpack RLC DL data
 *
 * @details
 *
 *    Function : unpackRlcDlData
 *
 *    Functionality: unpack RLC DL data
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcDlData(RlcMacDlDataFunc func, Pst *pst, Buffer *mBuf)
{
   RlcData *dlData;

   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(oduUnpackPointer, (PTR *) &dlData, mBuf);
   }
   else
   {
      return RFAILED;
   }
   ODU_PUT_MSG_BUF(mBuf);
   return((*func)(pst, dlData));
}

/*******************************************************************
 *
 * @brief Pack and send UL data from MAC to RLC
 *
 * @details
 *
 *    Function : packRlcUlData
 *
 *    Functionality: Pack and send UL data from MAC to RLC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcUlData(Pst* pst, RlcData *ulData)
{
   Buffer *mBuf = NULLP;
 
   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed at packRlcUlData");
      return RFAILED;
   }

   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKPK(oduPackPointer,(PTR)ulData, mBuf);
   }
   else
   {
      return RFAILED;
   }
 
   pst->event = (Event)EVENT_UL_DATA_TO_RLC;
   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpack RLC UL data
 *
 * @details
 *
 *    Function : unpackRlcUlData
 *
 *    Functionality: Unpack RLC UL data
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcUlData(RlcMacUlDataFunc func, Pst *pst, Buffer *mBuf)
{
   RlcData *ulData;
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(oduUnpackPointer,(PTR *) &ulData, mBuf);
   }
   else 
   {
      return RFAILED;
   }
   ODU_PUT_MSG_BUF(mBuf);
   return (*func)(pst, ulData);
}

/**********************************************************************
         End of file
**********************************************************************/
