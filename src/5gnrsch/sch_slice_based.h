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

// #define SCH_MULTI_THREAD      /* Enable the multi-thread intra-slice scheduling feature */
// #define SLICE_BASED_DEBUG_LOG    /* Enable the debug log */
#define NUM_SLICE 2     /* Define the number of slices for multi thread feature */

typedef enum
{
   FLAT,
   HIERARCHY
}SchAlgoMethod;

typedef enum
{
   RR, /* Round Robin */
   WFQ /* Weight Fair Queue */
}SchAlgorithm;

/*Following structures to keep record and estimations of PRB allocated for each
 * LC taking into consideration the RRM policies*/
typedef struct schSliceBasedLcInfo
{
   uint8_t  lcId;     /*LCID for which BO are getting recorded*/
   SchUeCb *ueCb;    /* To store which UE this LC belongs to */
   uint16_t priorLevel; /* Priority Level which is associated with this LC */
   float_t weight; /*  Weight (0 ~ 1) which is used for WFQ algorithm, could be calculated */
   uint32_t reqBO;    /*Size of the BO requested/to be allocated for this LC*/
   uint32_t allocBO;  /*TBS/BO Size which is actually allocated*/
   uint8_t  allocPRB; /*PRB count which is allocated based on RRM policy/FreePRB*/
}SchSliceBasedLcInfo;

typedef struct schSliceBasedLcCb
{
   /* TODO: For Multiple RRMPolicies, Make DedicatedLcInfo as array/Double Pointer 
    * and have separate DedLCInfo for each RRMPolcyMemberList */
   /* Dedicated LC List will be allocated, if any available */
   CmLListCp dedLcList;	/*Contain LCInfo per RRMPolicy*/
   CmLListCp defLcList; /*Linklist of LC assoc with Default S-NSSAI(s)*/
   /* SharedPRB number can be used by any LC.
    * Need to calculate in every Slot based on PRB availability*/
   uint16_t sharedNumPrb;
}SchSliceBasedLcCb;

typedef struct schSliceBasedHqProcCb
{
   SchSliceBasedLcCb lcCb; 
}SchSliceBasedHqProcCb;

typedef struct schSliceBasedHqCb
{
   CmLListCp  ulRetxHqList;
   CmLListCp  dlRetxHqList;
}SchSliceBasedHqCb;

typedef struct schSliceBasedUeCb
{
   SchSliceBasedHqCb   hqRetxCb;
   float_t prbWeight; /* prbWeight (0 ~ 1) is used for calculate the number of PRB within this TTI */
   float_t weight; /* Weight (0 ~ 1) which is used for WFQ algorithm */
   bool isTxPayloadLenAdded;
}SchSliceBasedUeCb;

/* Store the information of slices */
typedef struct schSliceBasedSliceCb
{
   Snssai  snssai;
   /* Linked list to store SchSliceBasedLcInfo of each UE which is associated with this slice */
   CmLListCp lcInfoList[MAX_NUM_UE]; 
   uint16_t dedicatedPrb;  /* As per 3GPP 28.541 Sec: 4.3.36 */
   uint16_t prioritizedPrb;   /* As per 3GPP 28.541 Sec: 4.3.36 */
   uint16_t sharedPrb;  /* As per 3GPP 28.541 Sec: 4.3.36 */
   uint16_t allocatedPrb; /* Store the allocated PRB per slice within 1 TTI */
   SchRrmPolicyRatio rrmPolicyRatioInfo;
   SchAlgoMethod algoMethod; /* Specify the scheduling method (0: flat, 1: hierarchy) */
   SchAlgorithm algorithm; /* Specify the scheduling algorithm (0: Round Robin, 1: Weight Fair Queue) */
}SchSliceBasedSliceCb;

/* Parameter for DL multi-thread intra-slice scheduling */
typedef struct schSliceBasedDlThreadArg   
{
   uint8_t *triggerFlag;
   SchCellCb *cell;
   SlotTimingInfo *pdcchTime;
   uint8_t *pdschNumSymbols;
   uint16_t *totalRemainingPrb;
   uint16_t *maxFreePRB;
   SchSliceBasedSliceCb *sliceCb;
   CmLListCp *ueDlNewTransmission;
   uint16_t ueId;
}SchSliceBasedDlThreadArg;

/* Multi-thread feature: Parameter for UL multi-thread intra-slice scheduling */
typedef struct schSliceBasedUlThreadArg
{
   SchCellCb *cell;
   SlotTimingInfo puschTime;
   uint8_t puschNumSymbols;
   uint16_t *totalRemainingPrb;
   uint16_t maxFreePRB;
   SchSliceBasedSliceCb *sliceCb;
   uint8_t ueId;
}SchSliceBasedUlThreadArg;

typedef struct schSliceBasedCellCb
{
   CmLListCp     ueToBeScheduled;           /* Linked list to store UEs pending to be scheduled */
   CmLListCp     sliceCbList;               /* Linked list to store slice control block with priority, the last node */

   /* For experiment */
   bool isTimerStart;
   uint16_t slot_ind_count;
   uint16_t timer_sec;
   uint16_t algoDelay;

   /* Multi-thread feature: For thread creating */
   SchSliceBasedDlThreadArg *threadArg[NUM_SLICE];
   pthread_t intraSliceThread[NUM_SLICE];
   
}SchSliceBasedCellCb;

uint8_t schSliceBasedAddUeToSchedule(SchCellCb *cellCb, uint16_t ueIdToAdd);
void SchSliceBasedSliceCfgReq(SchCellCb *cellCb);
void SchSliceBasedSliceRecfgReq(SchCellCb *cellCb, SchRrmPolicyOfSlice *rrmPolicy);
void schSliceBasedAllApisInit(SchAllApis *allSliceBasedApi);

/* Utility function for Slice-Based Scheduling */
uint8_t schSliceBasedFillLcInfoToSliceCb(CmLListCp *sliceCbList, SchUeCb *ueCb);
uint16_t schSliceBasedCalculatePriorLevel(uint16_t fiveQi);
void schSliceBasedSortLcByPriorLevel(CmLListCp *lcInfoList, float_t totalPriorLevel);
void schSliceBasedSortUeByWeight(SchCellCb *cellCb, CmLListCp *ueList, float_t totalWeight);
uint8_t schSliceBasedUpdateLcListReqBo(CmLListCp *lcInfoList, SchUeCb *ueCb, Direction dir);
void schSliceBasedUpdateGrantSizeForBoRpt(CmLListCp *lcLL, DlMsgSchInfo *dlMsgAlloc,\
                                    BsrInfo *bsrInfo, uint32_t *accumalatedBOSize, bool isDedicated);
uint32_t calculateEstimateTBSize(uint32_t reqBO, uint16_t mcsIdx, uint8_t numSymbols,\
                                   uint16_t maxPRB, uint16_t *estPrb);                                  

/* DL Slice-Based Main Function */
bool schSliceBasedDlScheduling(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchDlHqProcCb **hqP);
uint8_t schSliceBasedDlIntraSliceScheduling(SchCellCb *cellCb, SlotTimingInfo pdcchTime, uint8_t pdschNumSymbols, \
                                             uint16_t maxFreePRB, \
                                             uint16_t *totalRemainingPrb, SchSliceBasedSliceCb *sliceCb, uint8_t ueId);
void *schSliceBasedDlIntraSliceThreadScheduling(void *threadArg);
uint8_t schSliceBasedDlFinalScheduling(SchCellCb *cellCb, SlotTimingInfo pdschTime, SlotTimingInfo pdcchTime, \
                                       SlotTimingInfo pucchTime, uint8_t pdschStartSymbol, uint8_t pdschNumSymbols, 
                                       bool isRetx, SchDlHqProcCb **ueNewHarqList, \
                                       uint16_t remainingPrb, uint16_t startPrb, uint8_t ueId);

/* UL Slice-Based Main Function (To be finished) */
bool schSliceBasedUlScheduling(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchUlHqProcCb **hqP);
uint8_t schSliceBasedUlIntraSliceScheduling(SchCellCb *cellCb, SlotTimingInfo puschTime, uint8_t puschNumSymbols, \
                                            uint16_t *totalRemainingPrb, uint16_t maxFreePRB, \
                                            SchSliceBasedSliceCb *sliceCb, uint8_t ueId);
void *schSliceBasedUlIntraSliceThreadScheduling(void *threadArg);
uint8_t schSliceBasedUlFinalScheduling(SchCellCb *cellCb, SlotTimingInfo puschTime, SlotTimingInfo dciTime, \
                  uint8_t puschStartSymbol, uint8_t puschNumSymbols, uint8_t ueId, \
                  bool isRetx, SchUlHqProcCb **hqP, uint16_t remainingPrb, uint16_t startPrb);

/* Scheduling Algorithm */
void schSliceBasedRoundRobinAlgo(SchCellCb *cellCb, uint8_t ueId, CmLListCp *lcInfoList, uint8_t numSymbols, \
                                 uint16_t *availablePrb, SchAlgoMethod algoMethod, bool *srRcvd);
void schSliceBasedWeightedFairQueueAlgo(SchCellCb *cellCb, uint8_t ueId, CmLListCp *lcInfoList, \
                                 uint8_t numSymbols, uint16_t *availablePrb, SchAlgoMethod algoMethod, bool *srRcvd);

/* Scheduling Algorithm for Logical Channel Level */
void schSliceBasedRoundRobinAlgoforLc(CmLListCp *lcInfoList, uint8_t numSymbols, uint16_t *availablePrb, \
                                       bool *isTxPayloadLenAdded, bool *srRcvd);
void schSliceBasedWeightedFairQueueAlgoforLc(CmLListCp *lcInfoList, uint8_t numSymbols, uint16_t *availablePrb, \
                                       bool *isTxPayloadLenAdded, bool *srRcvd);
void SliceBasedPrbAllocDefaultSlice(CmLListCp *lcInfoList, uint16_t mcsIdx, uint8_t numSymbols, \
                                          uint16_t *availablePrb, bool *isTxPayloadLenAdded, bool *srRcvd);

/* For experiment */
void setRrmPolicyWithTimer(SchCellCb *cell);
/**********************************************************************
    End of file
 *********************************************************************/

