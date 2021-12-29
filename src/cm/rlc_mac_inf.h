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
/* This file containes define for RLC-MAC interface */

/* RLC instances */
#define RLC_UL_INST  0
#define RLC_DL_INST  1

/* Event */
#define EVENT_BO_STATUS_TO_MAC   15
#define EVENT_SCHED_RESULT_TO_RLC  16 
#define EVENT_DL_DATA_TO_MAC     17
#define EVENT_UL_DATA_TO_RLC     18

/* MACRO defines */
#define MAX_NUM_PDU     16

/* Buffer occupancy status information */
typedef struct rlcBoStatus
{
   uint16_t   cellId;     /*!< CELL ID */
   uint16_t   ueId;       /*!< UE ID */
   bool       commCh;     /*!< Common or Dedicated Channel */
   uint8_t    lcId;       /*!< Logical channel ID */
   uint32_t   bo;         /*!< Buffer occupancy reported by RLC */
}RlcBoStatus;

/* Scheduled logical channel info */
typedef struct rlcLcSchInfo
{
   bool      commCh;      /* Common or dedicated channel */
   uint8_t   lcId;        /*!< Logical channel ID */
   uint32_t  bufSize;  /*!< Total buffer size in bytes scheduled by MAC */
}RlcLcSchInfo;

/* Schedule result report */
typedef struct rlcSchedResultRpt
{
   //Slot info
   uint16_t       cellId;       /*!< CELL ID */
   uint16_t       rnti;         /*!< Temporary CRNTI */
   uint8_t        numLc;        /*!< Number of logical channels scheduled */
   SlotTimingInfo    slotInfo;     /*!< Timing info */
   RlcLcSchInfo   lcSch[MAX_NUM_LC];  /*!< Scheduled info of logical channels */
}RlcSchedResultRpt;

/* DL/UL data transfer */
typedef struct rlcPduInfo
{
   bool       commCh;   /*!<Common or Dedicated Channel */
   uint8_t    lcId;     /*!< Logical channel ID */
   uint16_t   pduLen;   /*!< PDU Length */
   uint8_t    *pduBuf;  /*!< RLC PDU buffer */
}RlcPduInfo;
 
typedef struct rlcData
{
   uint16_t      cellId;       /*!< CELL ID */
   uint16_t      rnti;         /*!< Temporary CRNTI */
   SlotTimingInfo   slotInfo;     /*!< Timing info */
   uint8_t       numPdu;       /*!< Number of RLC PDUs */
   RlcPduInfo    pduInfo[MAX_NUM_PDU];
   uint8_t       numLc;
   RlcBoStatus   boStatus[MAX_NUM_LC];     /*!< Buffer occupancy */
}RlcData;

/* Function pointers */
typedef uint8_t (*RlcMacBoStatusFunc)(Pst* pst, RlcBoStatus  *boStatus);
typedef uint8_t (*RlcMacSchedResultRptFunc)(Pst *pst, RlcSchedResultRpt *schRpt);
typedef uint8_t (*RlcMacDlDataFunc)(Pst* pst, RlcData *dlData);
typedef uint8_t (*RlcMacUlDataFunc)(Pst* pst, RlcData *ulData);

/* Function declarations */
uint8_t packRlcBoStatus(Pst* pst, RlcBoStatus*  boStatus);
uint8_t unpackRlcBoStatus(RlcMacBoStatusFunc func, Pst* pst, Buffer *mBuf);
uint8_t MacProcRlcBoStatus(Pst* pst, RlcBoStatus* boStatus);
uint8_t packRlcSchedResultRpt(Pst* pst, RlcSchedResultRpt *schRpt);
uint8_t unpackSchedResultRpt(RlcMacSchedResultRptFunc func, Pst *pst, Buffer *mBuf);
uint8_t RlcProcSchedResultRpt(Pst *pst, RlcSchedResultRpt *schRpt);
uint8_t packRlcDlData(Pst* pst, RlcData *datReq);
uint8_t unpackRlcDlData(RlcMacDlDataFunc func, Pst *pst, Buffer *mBuf);
uint8_t MacProcRlcDlData(Pst* pst, RlcData *dlData);
uint8_t packRlcUlData(Pst* pst, RlcData *ulData);
uint8_t unpackRlcUlData(RlcMacUlDataFunc func, Pst *pst, Buffer *mBuf);
uint8_t RlcProcUlData(Pst* pst, RlcData *ulData);

/**********************************************************************
          End of file
**********************************************************************/

