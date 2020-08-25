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
uint8_t packRlcBOStatus(Pst* pst, RlcBOStatus  *boStatus)
{
   Buffer *mBuf = NULLP;
 
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nRLC: Memory allocation failed at packBOStatus");
      return RFAILED;
   }

   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      /* light weight loose coupling */
      CMCHKPK(cmPkPtr,(PTR) boStatus, mBuf);
   }
   else
   {
      /* light coupling */
      CMCHKPK(SPkU16, boStatus->bo, mBuf);
      CMCHKPK(SPkU8, boStatus->lcId, mBuf);
      CMCHKPK(SPkU8, (uint8_t)boStatus->commCh, mBuf);
      CMCHKPK(SPkU16, boStatus->ueIdx, mBuf);
      CMCHKPK(SPkU16, boStatus->cellId, mBuf)

      SPutStaticBuffer(pst->region, pst->pool, (Data *)boStatus, sizeof(RlcBOStatus), 0); 
      boStatus = NULLP;
   }
 
   return SPstTsk(pst,mBuf);
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
uint8_t unpackRlcBOStatus(RlcMacBoStatusFunc func, Pst *pst, Buffer *mBuf)
{
   RlcBOStatus *boSta;
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
       /* Light weight loose coupling */
       CMCHKUNPK(cmUnpkPtr,(PTR *) &boSta, mBuf);
   }
   else
   {
      /* Light Coupling */
      RlcBOStatus boStatus;
      boSta = &boStatus;
      memset(boSta, 0, sizeof(RlcBOStatus));

      CMCHKUNPK(SUnpkU16, &boSta->cellId, mBuf);
      CMCHKUNPK(SUnpkU16, &boSta->ueIdx, mBuf);
      CMCHKUNPK(SUnpkU8, (uint8_t *)&boSta->commCh, mBuf);
      CMCHKUNPK(SUnpkU8, &boSta->lcId, mBuf);
      CMCHKUNPK(SUnpkU16, &boSta->bo, mBuf);
   }
   SPutMsg(mBuf);
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
uint8_t packRlcSchResultRpt(Pst* pst, RlcSchResultRpt *schRep)
{
   uint8_t   lcIdx;
   Buffer *mBuf = NULLP;
 
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nMAC: Memory allocation failed at packSchResultRep");
      return RFAILED;
   }
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR) schRep, mBuf);
   }
   else
   {
      for(lcIdx = (schRep->numLc-1); lcIdx >= 0; lcIdx--)
      {
         CMCHKPK(SPkU16, schRep->lcSch[lcIdx].bufSize, mBuf);
         CMCHKPK(SPkU8, schRep->lcSch[lcIdx].lcId, mBuf);
         CMCHKPK(SPkU8, (uint8_t)schRep->lcSch[lcIdx].commCh, mBuf);
      }
      CMCHKPK(SPkU8, schRep->numLc, mBuf);
      CMCHKPK(SPkU16, schRep->rnti, mBuf);
      CMCHKPK(SPkU16, schRep->cellId, mBuf);
      //CMCHKPK(cmPkLteTimingInfo, &param->timeToTx, mBuf);

      SPutStaticBuffer(pst->region, pst->pool, (Data *)schRep, \
         sizeof(RlcSchResultRpt), 0);
      schRep = NULLP;
   }
 
   return SPstTsk(pst,mBuf);
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
uint8_t unpackSchResultRpt(RlcMacSchResultRptFunc func, Pst *pst, Buffer *mBuf)
{
   uint8_t   lcIdx;
   RlcSchResultRpt *schRep;
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &schRep, mBuf);
   }
   else
   {
      RlcSchResultRpt schReport;
      schRep = &schReport;
      memset(schRep, 0, sizeof(RlcSchResultRpt));

      //CMCHKUNPK(cmUnpkLteTimingInfo, &param->timeToTx, mBuf);
      CMCHKUNPK(SUnpkU16, &schRep->cellId, mBuf);
      CMCHKUNPK(SUnpkU16, &schRep->rnti, mBuf);
      CMCHKUNPK(SUnpkU8, &schRep->numLc, mBuf);
      for(lcIdx = 0; lcIdx < schRep->numLc; lcIdx++)
      {
         CMCHKUNPK(SUnpkU8, (uint8_t *)&schRep->lcSch[lcIdx].commCh, mBuf);
         CMCHKUNPK(SUnpkU8, &schRep->lcSch[lcIdx].lcId, mBuf);
         CMCHKUNPK(SUnpkU16, &schRep->lcSch[lcIdx].bufSize, mBuf);
      }
   }
   SPutMsg(mBuf);
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
   uint8_t   pduIdx;
   uint16_t  bufIdx;
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nRLC: Memory allocation failed at packRlcDlData");
      return RFAILED;
   }
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
       CMCHKPK(cmPkPtr,(PTR) dlData, mBuf);
   }
   else
   {
      for (pduIdx = dlData->numPdu-1; pduIdx >= 0; pduIdx--)
      {
         for(bufIdx = dlData->pduInfo[pduIdx].pduLen-1; bufIdx >=0; bufIdx++)
         {
            CMCHKPK(SPkU8, dlData->pduInfo[pduIdx].pduBuf[bufIdx], mBuf);
         }
         CMCHKPK(SPkU16, dlData->pduInfo[pduIdx].pduLen, mBuf);
         CMCHKPK(SPkU8, dlData->pduInfo[pduIdx].lcId, mBuf);
         CMCHKPK(SPkU8, (uint8_t)dlData->pduInfo[pduIdx].commCh, mBuf);
      }
      CMCHKPK(SPkU8, dlData->numPdu, mBuf);
      CMCHKPK(SPkU16, dlData->rnti, mBuf);
      CMCHKPK(SPkU16, dlData->cellId, mBuf);
      // pack slot info
      
      SPutStaticBuffer(pst->region, pst->pool, (Data *)dlData, \
         sizeof(RlcData), 0);
      dlData = NULLP;
   }
   return SPstTsk(pst,mBuf);

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
   uint8_t   pduIdx;
   uint16_t  bufIdx;
   RlcData *dlData;

   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &dlData, mBuf);
   }
   else
   {
      RlcData dlDataInfo;
      dlData = &dlDataInfo;
      memset(dlData, 0, sizeof(RlcData));
         
      // unpack slot info
      CMCHKUNPK(SUnpkU16, &dlData->cellId, mBuf);
      CMCHKUNPK(SUnpkU16, &dlData->rnti, mBuf);
      CMCHKUNPK(SUnpkU8, &dlData->numPdu, mBuf);
      for (pduIdx = 0; pduIdx < dlData->numPdu; pduIdx++)
      {
         CMCHKUNPK(SUnpkU8, (uint8_t *)&dlData->pduInfo[pduIdx].commCh, mBuf);
         CMCHKUNPK(SUnpkU8, &dlData->pduInfo[pduIdx].lcId, mBuf);
         CMCHKUNPK(SUnpkU16, &dlData->pduInfo[pduIdx].pduLen, mBuf);
         for(bufIdx = 0; bufIdx < dlData->pduInfo[pduIdx].pduLen; bufIdx++)
         {
            CMCHKUNPK(SUnpkU8, &dlData->pduInfo[pduIdx].pduBuf[bufIdx], mBuf);  
         }
      }

   }
   SPutMsg(mBuf);
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
   uint8_t   pduIdx;
   uint16_t  bufIdx;
   Buffer *mBuf = NULLP;
 
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      DU_LOG("\nMAC : Memory allocation failed at packRlcUlData");
      return RFAILED;
   }

   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKPK(cmPkPtr,(PTR)ulData, mBuf);
   }
   else
   {
      for (pduIdx = ulData->numPdu-1; pduIdx >= 0; pduIdx--)
      {
         for(bufIdx = ulData->pduInfo[pduIdx].pduLen-1; bufIdx >=0; bufIdx++)
         {
            CMCHKPK(SPkU8, ulData->pduInfo[pduIdx].pduBuf[bufIdx], mBuf);
         }
         CMCHKPK(SPkU16, ulData->pduInfo[pduIdx].pduLen, mBuf);
         CMCHKPK(SPkU8, ulData->pduInfo[pduIdx].lcId, mBuf);
         CMCHKPK(SPkU8, (uint8_t)ulData->pduInfo[pduIdx].commCh, mBuf);
      }
      CMCHKPK(SPkU8, ulData->numPdu, mBuf);
      CMCHKPK(SPkU16, ulData->rnti, mBuf);
      CMCHKPK(SPkU16, ulData->cellId, mBuf);
      // pack slot info
      
      SPutStaticBuffer(pst->region, pst->pool, (Data *)ulData, \
        sizeof(RlcData), 0);
      ulData = NULLP;
   }
 
   pst->event = (Event)EVENT_UL_DATA_TO_RLC;
   return SPstTsk(pst,mBuf);
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
   uint8_t pduIdx;
   uint16_t bufIdx;
   RlcData *ulData;
 
   if (pst->selector == ODU_SELECTOR_LWLC)
   {
      CMCHKUNPK(cmUnpkPtr,(PTR *) &ulData, mBuf);
   }
   else 
   {
      RlcData ulDataInfo;
      ulData = &ulDataInfo;
      memset(ulData, 0, sizeof(RlcData));

      // unpack slot info
      CMCHKUNPK(SUnpkU16, &ulData->cellId, mBuf);
      CMCHKUNPK(SUnpkU16, &ulData->rnti, mBuf);
      CMCHKUNPK(SUnpkU8, &ulData->numPdu, mBuf);
      for (pduIdx = 0; pduIdx < ulData->numPdu; pduIdx++)
      {
         CMCHKUNPK(SUnpkU8, (uint8_t *)&ulData->pduInfo[pduIdx].commCh, mBuf);
         CMCHKUNPK(SUnpkU8, &ulData->pduInfo[pduIdx].lcId, mBuf);
         CMCHKUNPK(SUnpkU16, &ulData->pduInfo[pduIdx].pduLen, mBuf);
         for(bufIdx = 0; bufIdx < ulData->pduInfo[pduIdx].pduLen; bufIdx++)
         {
            CMCHKUNPK(SUnpkU8, &ulData->pduInfo[pduIdx].pduBuf[bufIdx], mBuf);
         }
      }
   }
   SPutMsg(mBuf);
   return (*func)(pst, ulData);
}

/**********************************************************************
         End of file
**********************************************************************/
