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
#define EVENT_SCH_RESULT_TO_RLC  16 
#define EVENT_DL_DATA_TO_MAC     17
#define EVENT_UL_DATA_TO_RLC     18

/* MACRO defines */
#define MAX_NUM_LC      11
#define MAX_NUM_PDU     16

/* Buffer occupancy status information */
typedef struct rlcBOStatus
{
   uint16_t   cellId;     /*!< CELL ID */
   uint16_t   ueIdx;      /*!< UE ID */
   bool       commCh;     /*!< Common or Dedicated Channel */
   uint8_t    lcId;       /*!< Logical channel ID */
   uint16_t   bo;         /*!< Buffer occupancy reported by RLC */
}RlcBOStatus;

/* Scheduled logical channel info */
typedef struct rlcMacLcSchInfo
{
   bool      commCh;      /* Common or dedicated channel */
   uint8_t   lcId;        /*!< Logical channel ID */
   uint16_t  bufSize;  /*!< Total buffer size in bytes scheduled by MAC */
}RlcLcSchInfo;

/* Schedule result report */
typedef struct rlcMacSchResultRpt
{
   //Slot info
   uint16_t       cellId;       /*!< CELL ID */
   uint16_t       rnti;         /*!< Temporary CRNTI */
   uint8_t        numLc;        /*!< Number of logical channels scheduled */
   SlotIndInfo    slotInfo;     /*!< Timing info */
   RlcLcSchInfo   lcSch[MAX_NUM_LC];  /*!< Scheduled info of logical channels */
}RlcSchResultRpt;

/* DL/UL data transfer */
typedef struct rlcPduInfo
{
   bool       commCh;   /*!<Common or Dedicated Channel */
   uint8_t    lcId;     /*!< Logical channel ID */
   uint16_t   pduLen;   /*!< PDU Length */
   uint8_t    *pduBuf;  /*!< RLC PDU buffer */
}RlcPduInfo;
 
typedef struct rlcMacData
{
   // add slot info
   uint16_t     cellId;       /*!< CELL ID */
   uint16_t     rnti;         /*!< Temporary CRNTI */
   SlotIndInfo  slotInfo;     /*!< Timing info */
   uint8_t      numPdu;       /*!< Number of RLC PDUs */
   RlcPduInfo   pduInfo[MAX_NUM_PDU];
}RlcData;


typedef uint8_t (*RlcMacBoStatusFunc)(Pst* pst, RlcBOStatus  *boStatus);
uint8_t packRlcBOStatus(Pst* pst, RlcBOStatus*  boStatus);
uint8_t unpackRlcBOStatus(RlcMacBoStatusFunc func, Pst* pst, Buffer *mBuf);
uint8_t MacProcRlcBOStatus(Pst* pst, RlcBOStatus* boStatus);

typedef uint8_t (*RlcMacSchResultRptFunc)(Pst *pst, RlcSchResultRpt *schRpt);
uint8_t packRlcSchResultRpt(Pst* pst, RlcSchResultRpt *schRpt);
uint8_t unpackSchResultRpt(RlcMacSchResultRptFunc func, Pst *pst, Buffer *mBuf);
uint8_t RlcMacProcSchResultRpt(Pst *pst, RlcSchResultRpt *schRpt);

typedef uint8_t (*RlcMacDlDataFunc)(Pst* pst, RlcData *dlData);
uint8_t packRlcDlData(Pst* pst, RlcData *datReq);
uint8_t unpackRlcDlData(RlcMacDlDataFunc func, Pst *pst, Buffer *mBuf);
uint8_t MacProcRlcDlData(Pst* pst, RlcData *dlData);

typedef uint8_t (*RlcMacUlDataFunc)(Pst* pst, RlcData *ulData);
uint8_t packRlcUlData(Pst* pst, RlcData *ulData);
uint8_t unpackRlcUlData(RlcMacUlDataFunc func, Pst *pst, Buffer *mBuf);
uint8_t RlcMacProcUlData(Pst* pst, RlcData *ulData);

/**********************************************************************
          End of file
**********************************************************************/

