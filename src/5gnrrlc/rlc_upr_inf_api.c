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
#include "common_def.h"
#include "du_app_rlc_inf.h"

/* Function pointer array for UL RRC Msg Transfer */
RlcUlRrcMsgToDuFunc rlcSendUlRrcMsgToDuOpts[] = 
{
   packRlcUlRrcMsgToDu,       /* 0 - Loosely coupled */
   DuProcRlcUlRrcMsgTrans,    /* 1 - Tightly coupled */
   packRlcUlRrcMsgToDu        /* 2 - Light weight loosely coupled */
};

RlcRrcDeliveryReportToDuFunc rlcSendRrcDeliveryReportToDuOpts[]=
{
   packRrcDeliveryReportToDu,          /* 0 - Loosely coupled */
   DuProcRlcRrcDeliveryReport,    /* 1 - Tightly coupled */
   packRrcDeliveryReportToDu           /* 2 - Light weight loosely coupled */
};

/* Function pointer array for DL RRC Msg Rsp Transfer */
RlcDlRrcMsgRspToDuFunc rlcSendDlRrcMsgRspToDuOpts[] = 
{
   packRlcDlRrcMsgRspToDu,       /* 0 - Loosely coupled */
   DuProcRlcDlRrcMsgRsp,         /* 1 - Tightly coupled */
   packRlcDlRrcMsgRspToDu        /* 2 - Light weight loosely coupled */
};

/* Function pointer array for UL RRC Msg Transfer */
RlcUlUserDataToDuFunc rlcSendUlUserDataToDuOpts[] =
{
   packRlcUlUserDataToDu,       /* 0 - Loosely coupled */
   DuProcRlcUlUserDataTrans,    /* 1 - Tightly coupled */
   packRlcUlUserDataToDu        /* 2 - Light weight loosely coupled */
};

RlcDuUeDeleteRsp rlcUeDeleteRspOpts[] =
{
   packRlcDuUeDeleteRsp,     /* 0 - loosely coupled */
   DuProcRlcUeDeleteRsp,     /* 1 - tightly coupled */
   packRlcDuUeDeleteRsp     /* 2 - LWLC loosely coupled */
};

RlcSlicePmToDuFunc rlcSlicePmOpts[] =
{
   packRlcDuSlicePm,          /* 0 - loosely coupled */
   DuProcRlcSliceMetrics,     /* 1 - tightly coupled */
   packRlcDuSlicePm           /* 2 - LWLC loosely coupled */
};
/*******************************************************************
 *
 * @brief Sends UL RRC Message Info to DU APP
 *
 * @details
 *
 *    Function : rlcSendUlRrcMsgToDu
 *
 *    Functionality:  Sends UL RRC Message Info to DU APP
 *
 * @params[in] Pst structure
 *             UL RRC Msg Info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcSendUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo)
{
   return (*rlcSendUlRrcMsgToDuOpts[pst->selector])(pst, ulRrcMsgInfo);
}

/*******************************************************************
*
* @brief Sends RRC delivery Message Info to DU APP
*
* @details
*
*    Function : rlcSendRrcDeliveryReportToDu
*
*    Functionality:  Sends RRC delivery Message Info to DU APP
*
* @params[in] Pst structure
*             RRC delivery Msg Info
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t rlcSendRrcDeliveryReportToDu(Pst *pst, RrcDeliveryReport *rrcDelivery) 
{
   return (*rlcSendRrcDeliveryReportToDuOpts[pst->selector])(pst, rrcDelivery);
}
/*******************************************************************
 *
 * @brief Sends DL RRC Message Status to DU APP
 *
 * @details
 *
 *    Function : rlcSendDlRrcMsgRspToDu
 *
 *    Functionality:  Sends DL RRC Message Status to DU APP
 *
 * @params[in] Pst structure
 *             DL RRC Msg Info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcSendDlRrcMsgRspToDu(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsg)
{
   return (*rlcSendDlRrcMsgRspToDuOpts[pst->selector])(pst, dlRrcMsg);
}

/*******************************************************************
 *
 * @brief Sends UL User data Info to DU APP
 *
 * @details
 *
 *    Function : rlcSendUlUserDataToDu
 *
 *    Functionality:  Sends UL User Data Info to DU APP
 *
 * @params[in] Pst structure
 *             UL user data Info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcSendUlUserDataToDu(Pst *pst, RlcUlUserDatInfo *ulUserData)
{
   return (*rlcSendUlUserDataToDuOpts[pst->selector])(pst, ulUserData);
}

/*******************************************************************
 *
 * @brief Sends Ue Delete Rsp to DU APP
 *
 * @details
 *
 *    Function : rlcSendUeDeleteRspToDu 
 *
 *    Functionality:  Sends Ue Delete Rsp to DU APP
 *
 * @params[in] Pst *pst, RlcUeDeleteRsp *ueDeleteRsp 
 *             
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcSendUeDeleteRspToDu(Pst *pst, RlcUeDeleteRsp *ueDeleteRsp)
{
    return (*rlcUeDeleteRspOpts[pst->selector])(pst, ueDeleteRsp);
}

/*******************************************************************
 *
 * @brief Sends Slice Performance Metrics to DU APP
 *
 * @details
 *
 *    Function : rlcSendSlicePmToDu 
 *
 *    Functionality:  Sends Performace Metrics per slice together to DU APP
 *
 * @params[in] Pst *pst, SlicePmList *sliceStats 
 *             
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcSendSlicePmToDu(Pst *pst, SlicePmList *sliceStats)
{
    return (*rlcSlicePmOpts[pst->selector])(pst, sliceStats);
}
/**********************************************************************
         End of file
**********************************************************************/
