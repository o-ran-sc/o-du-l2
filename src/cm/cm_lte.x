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

/********************************************************************20**
  
        Name:    Common LTE
    
        Type:    C include file
  
        Desc:    This file Contains the Data structures for Common LTE
 
        File:    cm_lte.x

        Sid:      cm_lte.x@@/main/2 - Fri Nov 13 14:09:17 2009
  
        Prg:     chakrapani
  
*********************************************************************21*/

#ifndef __CM_LTE_X__
#define __CM_LTE_X__

#include <cm_lte.h>
/* Packing Defines */
#define cmPkLteRbId              oduUnpackUInt8
#define cmPkLteRnti              oduUnpackUInt16
#define cmPkLteCellId            oduUnpackUInt16
#define cmPkLteRlcMode           oduUnpackUInt8
#define cmPkLteLcId              oduUnpackUInt8
#define cmPkLteLcType            oduUnpackUInt8
#define cmPkLteAggrLvl           oduUnpackUInt32

/* Unpacking Defines */
#define cmUnpkLteRbId            oduPackUInt8
#define cmUnpkLteRnti            oduPackUInt16
#define cmUnpkLteCellId          oduPackUInt16
#define cmUnpkLteRlcMode         oduPackUInt8
#define cmUnpkLteLcId            oduPackUInt8
#define cmUnpkLteLcType          oduPackUInt8
#define cmUnpkLteAggrLvl         oduPackUInt32

#define MAX_POOL_SIZE        6      /*!< Maximum pool size */
#define MAX_REGION_SIZE      5      /*!< Maximum Region size */
#ifdef PACK_STRUCT
#define CM_PACK_STRUCT __attribute__((packed))
#else
#define CM_PACK_STRUCT
#endif




#ifdef __cplusplus
EXTERN "C" {
#endif  /*__cplusplus*/

/** @file cm_lte.x
    @brief CM_LTE Interface File (cm_lte.x)
*/
 
/* definitions for Common LTE */

/** @brief Radio Bearer ID */
typedef U8    CmLteRbId;

/** @brief Cell ID */
typedef U16   CmLteCellId;

/** @brief RNTI */
typedef U16   CmLteRnti;

/** @brief Mode Type TM/UM/AM */
typedef U8    CmLteRlcMode;

/** @brief Logical Channel ID */
typedef U8    CmLteLcId;         

/** @brief Logical Channel Type */
typedef U8    CmLteLcType;         

/** @brief Transport Channel Type */
typedef U8    CmLteTrchType;         

/** @brief Contention Resolution ID */
typedef U8 CmLteContResId[6];

/** @brief RLC ID */
typedef struct cmLteRlcId
{
  CmLteRbId     rbId;   /*!< Radio Bearer ID */
  U8            rbType; /*!< RB Type */
  CmLteRnti     ueId;   /*!< UE ID */
  CmLteCellId   cellId; /*!< Cell ID */
}CmLteRlcId;

/** @brief LTE Timing Info */
typedef struct cmLteTimingInfo
{
#if 0
   U16 hSfn;                 /*!< Hyper System Frame Number */
#endif
   U16 sfn;                  /*!< System Frame Number */
   U16 slot;             /*!< Subframe number */
} CmLteTimingInfo;

/** @brief PDCP ID */
typedef struct cmLtePdcpId
{
   CmLteCellId   cellId;      /*!< Cell ID */
   CmLteRnti     ueId;        /*!< UE ID */
   CmLteRbId     rbId;        /*!< PDCP Instance ID */
   U8            rbType;      /*!< RB type */
} CmLtePdcpId;

/* Defining structures for Memory Information for L2-MEAS */
typedef struct cmLtePoolInfo
{
   U32   poolSize;
   U32   totAvailable;
   U32   crntUsed;
   U32   maxUsed;
} CmLtePoolInfo;

typedef struct cmLteRegionInfo
{
   U8   regionType; /* 0-SSI 1-Shared */
   U8   regionId;
   U8   numPools;
   U8   isGenMemInfoUpdated;
   CmLtePoolInfo poolInfo[MAX_POOL_SIZE];
}CmLteRegionInfo;

typedef struct cmLteMemInfo
{
   U8           numRegions;
   U8           idx;
   CmLteRegionInfo   regInfo[MAX_REGION_SIZE];
}CmLteMemInfo;

/** @brief CPU Utilization INFO */
typedef struct cmLteCpuUtilInfo
{
   U32      avgCpuUtil; /*!< average cpu utilization */
   U32      maxCpuUtil; /*!< Max cpu utilization */
}CmLteCpuUtilInfo;

/** @brief CPU INFO */
typedef struct cmLteCpuInfo
{
   U8       numCores;        /*!< number of cores*/
   CmLteCpuUtilInfo cpuUtil[CM_MAX_CPU_CORES]; /*!< cpu measurement info*/
}CmLteCpuInfo;
/** @brief CPU Utilization INFO */
typedef struct cmCpuUtilStatsInfo
{
   U32      numSamples;
   U32      maxCpuUtil; /*!< Max cpu utilization */
   U32      totCpuUtil; /*!< Total cpu utilization */
}CmCpuUtilStatsInfo;

/** @brief CPU INFO */
typedef struct cmCpuStatsInfo
{
   U8       numCores;        /*!< number of cores*/
   CmCpuUtilStatsInfo cpuUtil[CM_MAX_CPU_CORES]; /*!< cpu measurement info*/
}CmCpuStatsInfo;




/** @brief Counter Statistics */
typedef U32  CntrSts;

/** @brief Aggregation Level */
typedef enum cmLteAggrLvl
{
   CM_LTE_AGGR_LVL2 = 2,  /*!< Aggregation level 2 */
   CM_LTE_AGGR_LVL4 = 4,  /*!< Aggregation level 4 */
   CM_LTE_AGGR_LVL8 = 8,  /*!< Aggregation level 8 */
   CM_LTE_AGGR_LVL16= 16   /*!< Aggregation level 16 */
} CmLteAggrLvl;

/** @brief UE Category */
typedef enum cmLteUeCategory
{
   CM_LTE_UE_CAT_1 = 1,
   CM_LTE_UE_CAT_2,
   CM_LTE_UE_CAT_3,
   CM_LTE_UE_CAT_4,
   CM_LTE_UE_CAT_5,
   CM_LTE_UE_CAT_6,
   CM_LTE_UE_CAT_7, /* RRC-REL10-Upgrade */
   CM_LTE_UE_CAT_8
} CmLteUeCategory;

/****************************************************************************
 *                         PACK/UNPACK Functions
 ***************************************************************************/

EXTERN S16 cmUpdateSsiMemInfo(CmLteMemInfo *mInfo);

EXTERN S16 cmFillMemUtilizationMeas(CmLteMemInfo *memoryInfo,CmLteMemInfo *memInfo);

EXTERN S16 cmClearMemUtilizationCounter(CmLteMemInfo *memInfo);
EXTERN S16 UpdateSocMemInfo(U8 area , CmLteMemInfo *mInfo);

EXTERN S16 cmFillCpuUtilizationMeas(CmLteCpuInfo *cpuMeasInfo,CmCpuStatsInfo *cpuInfo);

EXTERN S16 cmClearCpuUtilizationCounter(CmCpuStatsInfo *cpuInfo);
EXTERN Void UpdateSocCpuInfo(CmCpuStatsInfo *cpuInfo,U8 Idx);

EXTERN S16 SGetRegPoolInfo(U8* numRegion, U8* numPool);

/* Packing Functions */
EXTERN S16 cmPkLteRlcId ARGS ((
CmLteRlcId *param,
Buffer *mBuf
));

EXTERN S16 cmPkLteTimingInfo ARGS ((
CmLteTimingInfo *param,
Buffer *mBuf
));

EXTERN S16 cmPkLtePdcpId ARGS ((
CmLtePdcpId *param,
Buffer *mBuf
));

/* Unpack Function */
EXTERN S16 cmUnpkLteRlcId ARGS ((
CmLteRlcId *param,
Buffer *mBuf
));

EXTERN S16 cmUnpkLteTimingInfo ARGS ((
CmLteTimingInfo *param,
Buffer *mBuf
));

EXTERN S16 cmUnpkLtePdcpId ARGS ((
CmLtePdcpId *param,
Buffer *mBuf
));

#ifdef TENB_TTI_PERF

typedef struct cmTtiProc 
{
	U32 totTtiProcessingTime;
	U32 numOfTti;
	U32 maxTtiProcessingTime;
	U32 ttiStretchCount;
	U32 ttiThresholdExceedCount;
	/* this count is added to compare phy reported tti stretch and calculated tti stretch */
	U32 phyReptTtiStretchCount;
}CmTtiProc;

EXTERN CmTtiProc ttiProc;

EXTERN Void cmUpdateTtiCounters(U32 ttiProcessingTime);
EXTERN Void cmResetTtiCounters(Void); 
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CM_LTE_X__ */

/********************************************************************30**
  
         End of file
**********************************************************************/
