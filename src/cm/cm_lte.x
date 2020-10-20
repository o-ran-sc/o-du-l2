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
extern "C" {
#endif  /*__cplusplus*/

/** @file cm_lte.x
    @brief CM_LTE Interface File (cm_lte.x)
*/
 
/* definitions for Common LTE */

/** @brief Radio Bearer ID */
typedef uint8_t    CmLteRbId;

/** @brief Cell ID */
typedef uint16_t   CmLteCellId;

/** @brief RNTI */
typedef uint16_t   CmLteRnti;

/** @brief Mode Type TM/UM/AM */
typedef uint8_t    CmLteRlcMode;

/** @brief Logical Channel ID */
typedef uint8_t    CmLteLcId;         

/** @brief Logical Channel Type */
typedef uint8_t    CmLteLcType;         

/** @brief Transport Channel Type */
typedef uint8_t    CmLteTrchType;         

/** @brief Contention Resolution ID */
typedef uint8_t CmLteContResId[6];

/** @brief RLC ID */
typedef struct cmLteRlcId
{
  CmLteRbId     rbId;   /*!< Radio Bearer ID */
  uint8_t            rbType; /*!< RB Type */
  CmLteRnti     ueId;   /*!< UE ID */
  CmLteCellId   cellId; /*!< Cell ID */
}CmLteRlcId;

/** @brief LTE Timing Info */
typedef struct cmLteTimingInfo
{
#if 0
   uint16_t hSfn;                 /*!< Hyper System Frame Number */
#endif
   uint16_t sfn;                  /*!< System Frame Number */
   uint16_t slot;             /*!< Subframe number */
} CmLteTimingInfo;

/** @brief PDCP ID */
typedef struct cmLtePdcpId
{
   CmLteCellId   cellId;      /*!< Cell ID */
   CmLteRnti     ueId;        /*!< UE ID */
   CmLteRbId     rbId;        /*!< PDCP Instance ID */
   uint8_t            rbType;      /*!< RB type */
} CmLtePdcpId;

/* Defining structures for Memory Information for L2-MEAS */
typedef struct cmLtePoolInfo
{
   uint32_t   poolSize;
   uint32_t   totAvailable;
   uint32_t   crntUsed;
   uint32_t   maxUsed;
} CmLtePoolInfo;

typedef struct cmLteRegionInfo
{
   uint8_t   regionType; /* 0-SSI 1-Shared */
   uint8_t   regionId;
   uint8_t   numPools;
   uint8_t   isGenMemInfoUpdated;
   CmLtePoolInfo poolInfo[MAX_POOL_SIZE];
}CmLteRegionInfo;

typedef struct cmLteMemInfo
{
   uint8_t           numRegions;
   uint8_t           idx;
   CmLteRegionInfo   regInfo[MAX_REGION_SIZE];
}CmLteMemInfo;

/** @brief CPU Utilization INFO */
typedef struct cmLteCpuUtilInfo
{
   uint32_t      avgCpuUtil; /*!< average cpu utilization */
   uint32_t      maxCpuUtil; /*!< Max cpu utilization */
}CmLteCpuUtilInfo;

/** @brief CPU INFO */
typedef struct cmLteCpuInfo
{
   uint8_t       numCores;        /*!< number of cores*/
   CmLteCpuUtilInfo cpuUtil[CM_MAX_CPU_CORES]; /*!< cpu measurement info*/
}CmLteCpuInfo;
/** @brief CPU Utilization INFO */
typedef struct cmCpuUtilStatsInfo
{
   uint32_t      numSamples;
   uint32_t      maxCpuUtil; /*!< Max cpu utilization */
   uint32_t      totCpuUtil; /*!< Total cpu utilization */
}CmCpuUtilStatsInfo;

/** @brief CPU INFO */
typedef struct cmCpuStatsInfo
{
   uint8_t       numCores;        /*!< number of cores*/
   CmCpuUtilStatsInfo cpuUtil[CM_MAX_CPU_CORES]; /*!< cpu measurement info*/
}CmCpuStatsInfo;




/** @brief Counter Statistics */
typedef uint32_t  CntrSts;

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

S16 cmUpdateSsiMemInfo(CmLteMemInfo *mInfo);

S16 cmFillMemUtilizationMeas(CmLteMemInfo *memoryInfo,CmLteMemInfo *memInfo);

S16 cmClearMemUtilizationCounter(CmLteMemInfo *memInfo);
S16 UpdateSocMemInfo(uint8_t area , CmLteMemInfo *mInfo);

S16 cmFillCpuUtilizationMeas(CmLteCpuInfo *cpuMeasInfo,CmCpuStatsInfo *cpuInfo);

S16 cmClearCpuUtilizationCounter(CmCpuStatsInfo *cpuInfo);
Void UpdateSocCpuInfo(CmCpuStatsInfo *cpuInfo,uint8_t Idx);

S16 SGetRegPoolInfo(uint8_t* numRegion, uint8_t* numPool);

/* Packing Functions */
S16 cmPkLteRlcId ARGS ((
CmLteRlcId *param,
Buffer *mBuf
));

S16 cmPkLteTimingInfo ARGS ((
CmLteTimingInfo *param,
Buffer *mBuf
));

S16 cmPkLtePdcpId ARGS ((
CmLtePdcpId *param,
Buffer *mBuf
));

/* Unpack Function */
S16 cmUnpkLteRlcId ARGS ((
CmLteRlcId *param,
Buffer *mBuf
));

S16 cmUnpkLteTimingInfo ARGS ((
CmLteTimingInfo *param,
Buffer *mBuf
));

S16 cmUnpkLtePdcpId ARGS ((
CmLtePdcpId *param,
Buffer *mBuf
));

#ifdef TENB_TTI_PERF

typedef struct cmTtiProc 
{
	uint32_t totTtiProcessingTime;
	uint32_t numOfTti;
	uint32_t maxTtiProcessingTime;
	uint32_t ttiStretchCount;
	uint32_t ttiThresholdExceedCount;
	/* this count is added to compare phy reported tti stretch and calculated tti stretch */
	uint32_t phyReptTtiStretchCount;
}CmTtiProc;

CmTtiProc ttiProc;

Void cmUpdateTtiCounters(uint32_t ttiProcessingTime);
Void cmResetTtiCounters(Void); 
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CM_LTE_X__ */

/********************************************************************30**
  
         End of file
**********************************************************************/
