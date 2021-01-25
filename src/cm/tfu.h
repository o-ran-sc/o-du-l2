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

/**********************************************************************
     Name:    Lower layer interface - TFU

     Type:    C header file

     Desc:    Constants needed for TFU interface

     File:    tfu.h

*********************************************************************21*/

/**
  @file tfu.h
  @brief Defines for TFU interface.
  */

#ifndef __TFU_H__
#define __TFU_H__

/* Define for the block size for memory allocation */
/** @name TFU_TDD */
/** @{ */
#define TFU_MAX_HQ_RES        4     /*!< n^1 Pucch resources for
                                                    HARQ feedback */

#ifdef TFU_5GTF
#define TFU_RIV_324        324     /*!<  RV value 324 */
#define TFU_RIV_325        325     /*!<  RV value 325 */
#define TFU_RIV_326        326     /*!<  RV value 326 */
#endif

#ifdef TFU_TDD
#define TFU_BLKSZ                 2048  /*!< Block size for memory allocations. */
#else
/* tfu_h_001.main_3  Changing from 4096 to 1500 to align with max bucket-max 
 * packet sizes - optimization*/
#define TFU_BLKSZ                 1500  /*!< Block size for memory allocations. */
#endif
/* tfu_h_001.main_6. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
#define TFU_MAX_UL_RB             110   /*!< Maximum number of RBs in UL */
#define TFU_MAX_CQI_BYTES         9     /*!< Maximum number of CQI Bytes length */
#define TFU_MAX_PHY_ANTENNA       4     /*!<  Maximum number of Physical Antennas */
#endif /* TFU_UPGRADE */

#define TFU_MAX_RAPID_IN_SUBFRAME 64    /*!< Maximum number of Random access preambes in a subframe. */
#define TFU_MAX_UL_SUBBAND        110   /*!< Maxiumum number of Uplink Subbands.*/
#define TFU_MAX_DL_SUBBAND        110   /*!< Maxiumum number of Downlink Subbands.*/
#define TFU_MAX_TB                2    /*!< Maximum number of Transport blocks per UE. */
#ifdef L2_OPTMZ
#define TFU_MAX_PDU               8    /*!< This No is mapped to RGU_MAX_PDU*/
#define TFU_MAX_LC                10    /*!< This No is mapped to RGU_MAX_LC*/
#endif
#define TFU_MAX_M                 4    /*!< Maximum value of "M" for HARQ Feedback multiplexing. */
#define TFU_MAX_2BIT_TPC          16   /*!< Maximum number of 2 bit TPC commands. */
#define TFU_MAX_1BIT_TPC          32   /*!< Maximum number of 1 bit TPC commands. */
/*tfu_h_001.main_5 - Added support for SPS*/
/* Tunable parameter */
#define TFU_MAX_MEMBLK_SIZE       1500 /* tfu_h_001.main_3 Changing from 2048 to 1500 
                                          for alignment and optimization */

#define TFU_UBNDREQ_MNGMT         1    /*!< TFU sap unbind reason */

#ifdef LTE_ADV
#define TFU_MAX_FDD_HARQ_FDBKS       10 
#define TFU_MAX_HARQ_FDBKS       20 /*For Rel10 UE
                                     Considering 10 bit requirement of PUCCH Format 3*/
#else
#define TFU_MAX_HARQ_FDBKS       TFU_MAX_TB /*For Rel8/Rel9 UE*/
#endif/*LTE_ADV*/

/* selector(coupling) values */
#define TFU_MAX_ALLOC_BYTES     4
/* pack unpack error code */
#define ETFUXXX                 0
#define ERRTFU                  0
/* tfu_h_001.main_2 - ccpu00110457 DTX Changes Start */
#define TFU_HQFDB_NACK          FALSE
#define TFU_HQFDB_ACK           TRUE
/* tfu_h_001.main_6. Added changes of TFU_UPGRADE */
#define TFU_HQFDB_DTX           4
#define TFU_HQFDB_INVALID       0xFF

#define TFU_PERIODIC_CQI_TYPE  0
#define TFU_APERIODIC_CQI_TYPE 1

#define TFU_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      SPutMsg((_buf)); \
      _buf = NULLP; \
   } \
}

#define TFU_FREE_MEM(_mem)\
{\
   if (NULLP != (_mem)) \
   { \
      cmFreeMem((_mem)); \
      _mem = NULLP; \
   } \
}

#endif /* __TFU_H__ */

/********************************************************************30**

         End of file
**********************************************************************/
