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

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"           /* Layer manager interface includes*/
#include "crg.h"           /* CRG interface includes*/
#include "rgu.h"           /* RGU interface includes*/
#include "tfu.h"           /* TFU interface includes */
#include "rg_sch_inf.h"    /* SCH interface includes */
#include "rg_prg.h"       /* PRG (MAC-MAC) interface includes*/
#include "rg_env.h"       /* MAC environmental includes*/
#include "rg.h"           /* MAC includes*/
#include "rg_err.h"       /* MAC error includes*/
#include "du_log.h"
#include "lwr_mac_fsm.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* CRG interface includes */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"        /* PRG (MAC-MAC) Interface typedefs */
#include "du_app_mac_inf.h"
#include "mac.h"
#include "rg.x"            /* typedefs for MAC */
#include "lwr_mac_phy.h"
#include "common_def.h"
#include "math.h"

#define MIB_SFN_BITMASK 0xFC
#define PDCCH_PDU_TYPE 0
#define PDSCH_PDU_TYPE 1
#define SSB_PDU_TYPE 3
#define PRACH_PDU_TYPE 0
#define PUSCH_PDU_TYPE 1
#define PDU_PRESENT 1
#define SET_MSG_LEN(x, size) x += size

extern void fapiMacConfigRsp();
extern uint8_t UnrestrictedSetNcsTable[MAX_ZERO_CORR_CFG_IDX];

/* Global variables */
uint8_t slotIndIdx;
uint16_t sendTxDataReq(SlotIndInfo currTimingInfo, DlSchedInfo *dlInfo);

void lwrMacInit()
{
#ifdef INTEL_WLS
   uint8_t  idx;

   /* Initializing WLS free mem list */
   slotIndIdx = 1;
   for(idx = 0; idx < WLS_MEM_FREE_PRD; idx++)
   {
      cmLListInit(&wlsBlockToFreeList[idx]);
   }
#endif
}

 /*******************************************************************
  *
  * @brief Handles Invalid Request Event
  *
  * @details
  *
  *    Function : lwr_mac_handleInvalidEvt
  *
  *    Functionality:
  *         - Displays the PHY state when the invalid event occurs
  *
  * @params[in]
  * @return ROK     - success
  *         RFAILED - failure
  *
  * ****************************************************************/
S16 lwr_mac_handleInvalidEvt(void *msg)
{
   printf("\nLWR_MAC: Error Indication Event[%d] received in state [%d]", clGlobalCp.event, clGlobalCp.phyState);
   RETVALUE(ROK);
}

#ifdef FAPI
/*******************************************************************
  *
  * @brief Fills FAPI message header
  *
  * @details
  *
  *    Function : fillMsgHeader
  *
  *    Functionality:
  *         -Fills FAPI message header
  *
  * @params[in] Pointer to header
  *             Number of messages
  *             Messae Type
  *             Length of message
  * @return void
  *
  * ****************************************************************/
PUBLIC void fillMsgHeader(fapi_msg_t *hdr, uint16_t msgType, uint16_t msgLen)
{
   hdr->message_type_id = msgType;
   hdr->length = msgLen;
}

/*******************************************************************
  *
  * @brief Fills FAPI Config Request message header
  *
  * @details
  *
  *    Function : fillTlvs
  *
  *    Functionality:
  *         -Fills FAPI Config Request message header
  *
  * @params[in] Pointer to TLV
  *             Tag
  *             Length
  *             Value
  *             MsgLen
  * @return void
  *
  * ****************************************************************/
PUBLIC void fillTlvs(fapi_uint16_tlv_t *tlv, uint16_t tag, uint16_t length,
uint16_t value, uint32_t *msgLen)
{
   tlv->tl.tag    = tag;
   tlv->tl.length = length;
   tlv->value     = value;
   *msgLen        = *msgLen + sizeof(tag) + sizeof(length) + length;
}
 /*******************************************************************
  *
  * @brief fills the cyclic prefix by comparing the bitmask
  *
  * @details
  *
  *    Function : fillCyclicPrefix
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's cyclic prefix.
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ********************************************************************/
PUBLIC void fillCyclicPrefix(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_NORMAL_CYCLIC_PREFIX_MASK) == FAPI_NORMAL_CYCLIC_PREFIX_MASK)
   {
      (*cellPtr)->cyclicPrefix   = NORMAL_CYCLIC_PREFIX_MASK;
   }
   else if((value & FAPI_EXTENDED_CYCLIC_PREFIX_MASK) == FAPI_EXTENDED_CYCLIC_PREFIX_MASK)
   {
      (*cellPtr)->cyclicPrefix   = EXTENDED_CYCLIC_PREFIX_MASK;
   }
   else
   {
      (*cellPtr)->cyclicPrefix = INVALID_VALUE;
   }
}

 /*******************************************************************
  *
  * @brief fills the subcarrier spacing of Downlink by comparing the bitmask
  *
  * @details
  *
  *    Function : fillSubcarrierSpaceDl
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's subcarrier spacing in DL
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  * ****************************************************************/

PUBLIC void fillSubcarrierSpaceDl(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_15KHZ_MASK) == FAPI_15KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingDl = SPACING_15_KHZ;
   }
   else if((value & FAPI_30KHZ_MASK) == FAPI_30KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingDl = SPACING_30_KHZ;
   }
   else if((value & FAPI_60KHZ_MASK) == FAPI_60KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingDl = SPACING_60_KHZ;
   }
   else if((value & FAPI_120KHZ_MASK) == FAPI_120KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingDl = SPACING_120_KHZ;
   }
   else
   {
      (*cellPtr)->supportedSubcarrierSpacingDl = INVALID_VALUE;
   }
}

 /*******************************************************************
  *
  * @brief fills the downlink bandwidth by comparing the bitmask
  *
  * @details
  *
  *    Function : fillBandwidthDl
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *         -fills the cellPtr's DL Bandwidth
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  * ****************************************************************/

PUBLIC void fillBandwidthDl(uint16_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_5MHZ_BW_MASK) == FAPI_5MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_5MHZ;
   }
   else if((value & FAPI_10MHZ_BW_MASK) == FAPI_10MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_10MHZ;
   }
   else if((value & FAPI_15MHZ_BW_MASK) == FAPI_15MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_15MHZ;
   }
   else if((value & FAPI_20MHZ_BW_MASK) == FAPI_20MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_20MHZ;
   }
   else if((value & FAPI_40MHZ_BW_MASK) == FAPI_40MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_40MHZ;
   }
   else if((value & FAPI_50MHZ_BW_MASK) == FAPI_50MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_50MHZ;
   }
   else if((value & FAPI_60MHZ_BW_MASK) == FAPI_60MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_60MHZ;
   }
   else if((value & FAPI_70MHZ_BW_MASK) == FAPI_70MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_70MHZ;
   }
   else if((value & FAPI_80MHZ_BW_MASK) == FAPI_80MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_80MHZ;
   }
   else if((value & FAPI_90MHZ_BW_MASK) == FAPI_90MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_90MHZ;
   }
   else if((value & FAPI_100MHZ_BW_MASK) == FAPI_100MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_100MHZ;
   }
   else if((value & FAPI_200MHZ_BW_MASK) == FAPI_200MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_200MHZ;
   }
   else if((value & FAPI_400MHZ_BW_MASK) == FAPI_400MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthDl = BW_400MHZ;
   }
   else
   {
      (*cellPtr)->supportedBandwidthDl = INVALID_VALUE;
   }
}

 /*******************************************************************
  *
  * @brief fills the subcarrier spacing of Uplink by comparing the bitmask
  *
  * @details
  *
  *    Function : fillSubcarrierSpaceUl
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *         -fills cellPtr's subcarrier spacing in UL
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  * ****************************************************************/

PUBLIC void fillSubcarrierSpaceUl(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_15KHZ_MASK) == FAPI_15KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingsUl = SPACING_15_KHZ;
   }
   else if((value & FAPI_30KHZ_MASK) == FAPI_30KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingsUl = SPACING_30_KHZ;
   }
   else if((value & FAPI_60KHZ_MASK) == FAPI_60KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingsUl = SPACING_60_KHZ;
   }
   else if((value & FAPI_120KHZ_MASK) == FAPI_120KHZ_MASK)
   {
      (*cellPtr)->supportedSubcarrierSpacingsUl = SPACING_120_KHZ;
   }
   else
   {
      (*cellPtr)->supportedSubcarrierSpacingsUl = INVALID_VALUE;
   }
}

 /*******************************************************************
  *
  * @brief fills the uplink bandwidth by comparing the bitmask
  *
  * @details
  *
  *    Function : fillBandwidthUl
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's UL Bandwidth
  *
  *
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  *
  * ****************************************************************/

PUBLIC void fillBandwidthUl(uint16_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_5MHZ_BW_MASK) == FAPI_5MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_5MHZ;
   }
   else if((value & FAPI_10MHZ_BW_MASK) == FAPI_10MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_10MHZ;
   }
   else if((value & FAPI_15MHZ_BW_MASK) == FAPI_15MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_15MHZ;
   }
   else if((value & FAPI_20MHZ_BW_MASK) == FAPI_20MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_20MHZ;
   }
   else if((value & FAPI_40MHZ_BW_MASK) == FAPI_40MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_40MHZ;
   }
   else if((value & FAPI_50MHZ_BW_MASK) == FAPI_50MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_50MHZ;
   }
   else if((value & FAPI_60MHZ_BW_MASK) == FAPI_60MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_60MHZ;
   }
   else if((value & FAPI_70MHZ_BW_MASK) == FAPI_70MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_70MHZ;
   }
   else if((value & FAPI_80MHZ_BW_MASK) == FAPI_80MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_80MHZ;
   }
   else if((value & FAPI_90MHZ_BW_MASK) == FAPI_90MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_90MHZ;
   }
   else if((value & FAPI_100MHZ_BW_MASK) == FAPI_100MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_100MHZ;
   }
   else if((value & FAPI_200MHZ_BW_MASK) == FAPI_200MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_200MHZ;
   }
   else if((value & FAPI_400MHZ_BW_MASK) == FAPI_400MHZ_BW_MASK)
   {
      (*cellPtr)->supportedBandwidthUl = BW_400MHZ;
   }
   else
   {
      (*cellPtr)->supportedBandwidthUl = INVALID_VALUE;
   }
}
 /*******************************************************************
  *
  * @brief fills the CCE maping by comparing the bitmask
  *
  * @details
  *
  *    Function : fillCCEmaping
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's CCE Mapping Type
  *
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  * ****************************************************************/

PUBLIC void fillCCEmaping(uint8_t value,  ClCellParam **cellPtr)
{
   if ((value & FAPI_CCE_MAPPING_INTERLEAVED_MASK) == FAPI_CCE_MAPPING_INTERLEAVED_MASK)
   {
      (*cellPtr)->cceMappingType = CCE_MAPPING_INTERLEAVED_MASK;
   }
   else if((value & FAPI_CCE_MAPPING_INTERLEAVED_MASK) == FAPI_CCE_MAPPING_NONINTERLVD_MASK)
   {
      (*cellPtr)->cceMappingType = CCE_MAPPING_NONINTERLVD_MASK;
   }
   else
   {
      (*cellPtr)->cceMappingType = INVALID_VALUE;
   }
}

 /*******************************************************************
  *
  * @brief fills the PUCCH format by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPucchFormat
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's pucch format
  *
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  * ****************************************************************/

PUBLIC void fillPucchFormat(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_FORMAT_0_MASK) == FAPI_FORMAT_0_MASK)
   {
      (*cellPtr)->pucchFormats    = FORMAT_0;
   }
   else if((value & FAPI_FORMAT_1_MASK) == FAPI_FORMAT_1_MASK)
   {
      (*cellPtr)->pucchFormats    = FORMAT_1;
   }
   else if((value & FAPI_FORMAT_2_MASK) == FAPI_FORMAT_2_MASK)
   {
      (*cellPtr)->pucchFormats    = FORMAT_2;
   }
   else if((value & FAPI_FORMAT_3_MASK) == FAPI_FORMAT_3_MASK)
   {
      (*cellPtr)->pucchFormats    = FORMAT_3;
   }
   else if((value & FAPI_FORMAT_4_MASK) == FAPI_FORMAT_4_MASK)
   {
      (*cellPtr)->pucchFormats    = FORMAT_4;
   }
   else
   {
      (*cellPtr)->pucchFormats    = INVALID_VALUE;
   }
}

 /*******************************************************************
  *
  * @brief fills the PDSCH Mapping Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPdschMappingType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PDSCH MappingType
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  * ****************************************************************/

PUBLIC void fillPdschMappingType(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PDSCH_MAPPING_TYPE_A_MASK) == FAPI_PDSCH_MAPPING_TYPE_A_MASK)
   {
      (*cellPtr)->pdschMappingType = MAPPING_TYPE_A;
   }
   else if((value & FAPI_PDSCH_MAPPING_TYPE_B_MASK) == FAPI_PDSCH_MAPPING_TYPE_B_MASK)
   {
      (*cellPtr)->pdschMappingType = MAPPING_TYPE_B;
   }
   else
   {
      (*cellPtr)->pdschMappingType = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PDSCH Allocation Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPdschAllocationType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PDSCH AllocationType
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  * ****************************************************************/

PUBLIC void fillPdschAllocationType(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PDSCH_ALLOC_TYPE_0_MASK) == FAPI_PDSCH_ALLOC_TYPE_0_MASK)
   {
      (*cellPtr)->pdschAllocationTypes = ALLOCATION_TYPE_0;
   }
   else if((value & FAPI_PDSCH_ALLOC_TYPE_1_MASK) == FAPI_PDSCH_ALLOC_TYPE_1_MASK)
   {
      (*cellPtr)->pdschAllocationTypes = ALLOCATION_TYPE_1;
   }
   else
   {
      (*cellPtr)->pdschAllocationTypes = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PDSCH PRB Mapping Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPrbMappingType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PRB Mapping Type
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/
PUBLIC void fillPrbMappingType(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PDSCH_VRB_TO_PRB_MAP_NON_INTLV_MASK) == FAPI_PDSCH_VRB_TO_PRB_MAP_NON_INTLV_MASK)
   {
      (*cellPtr)->pdschVrbToPrbMapping = VRB_TO_PRB_MAP_NON_INTLV;
   }
   else if((value & FAPI_PDSCH_VRB_TO_PRB_MAP_INTLVD_MASK) == FAPI_PDSCH_VRB_TO_PRB_MAP_INTLVD_MASK)
   {
      (*cellPtr)->pdschVrbToPrbMapping = VRB_TO_PRB_MAP_INTLVD;
   }
   else
   {
      (*cellPtr)->pdschVrbToPrbMapping = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PDSCH DmrsConfig Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPdschDmrsConfigType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's DmrsConfig Type
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPdschDmrsConfigType(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PDSCH_DMRS_CONFIG_TYPE_1_MASK) == FAPI_PDSCH_DMRS_CONFIG_TYPE_1_MASK)
   {
      (*cellPtr)->pdschDmrsConfigTypes = DMRS_CONFIG_TYPE_1;
   }
   else if((value & FAPI_PDSCH_DMRS_CONFIG_TYPE_2_MASK) == FAPI_PDSCH_DMRS_CONFIG_TYPE_2_MASK)
   {
      (*cellPtr)->pdschDmrsConfigTypes = DMRS_CONFIG_TYPE_2;
   }
   else
   {
      (*cellPtr)->pdschDmrsConfigTypes = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PDSCH DmrsLength by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPdschDmrsLength
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PdschDmrsLength
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/
PUBLIC void fillPdschDmrsLength(uint8_t value, ClCellParam **cellPtr)
{
   if(value == FAPI_PDSCH_DMRS_MAX_LENGTH_1)
   {
      (*cellPtr)->pdschDmrsMaxLength = DMRS_MAX_LENGTH_1;
   }
   else if(value == FAPI_PDSCH_DMRS_MAX_LENGTH_2)
   {
      (*cellPtr)->pdschDmrsMaxLength = DMRS_MAX_LENGTH_2;
   }
   else
   {
      (*cellPtr)->pdschDmrsMaxLength = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PDSCH Dmrs Additional Pos by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPdschDmrsAddPos
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's Pdsch DmrsAddPos
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPdschDmrsAddPos(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_DMRS_ADDITIONAL_POS_0_MASK) == FAPI_DMRS_ADDITIONAL_POS_0_MASK)
   {
      (*cellPtr)->pdschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_0;
   }
   else if((value & FAPI_DMRS_ADDITIONAL_POS_1_MASK) == FAPI_DMRS_ADDITIONAL_POS_1_MASK)
   {
      (*cellPtr)->pdschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_1;
   }
   else if((value & FAPI_DMRS_ADDITIONAL_POS_2_MASK) == FAPI_DMRS_ADDITIONAL_POS_2_MASK)
   {
      (*cellPtr)->pdschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_2;
   }
   else if((value & FAPI_DMRS_ADDITIONAL_POS_3_MASK) == FAPI_DMRS_ADDITIONAL_POS_3_MASK)
   {
      (*cellPtr)->pdschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_3;
   }
   else
   {
      (*cellPtr)->pdschDmrsAdditionalPos = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the Modulation Order in DL by comparing the bitmask
  *
  * @details
  *
  *    Function : fillModulationOrderDl
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's ModulationOrder in DL.
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/
PUBLIC void fillModulationOrderDl(uint8_t value, ClCellParam **cellPtr)
{
   if(value == 0 )
   {
      (*cellPtr)->supportedMaxModulationOrderDl = MOD_QPSK;
   }
   else if(value == 1)
   {
      (*cellPtr)->supportedMaxModulationOrderDl = MOD_16QAM;
   }
   else if(value == 2)
   {
      (*cellPtr)->supportedMaxModulationOrderDl = MOD_64QAM;
   }
   else if(value == 3)
   {
      (*cellPtr)->supportedMaxModulationOrderDl = MOD_256QAM;
   }
   else
   {
      (*cellPtr)->supportedMaxModulationOrderDl = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PUSCH DmrsConfig Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPuschDmrsConfigType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PUSCH DmrsConfigType
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPuschDmrsConfig(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PUSCH_DMRS_CONFIG_TYPE_1_MASK) == FAPI_PUSCH_DMRS_CONFIG_TYPE_1_MASK)
   {
      (*cellPtr)->puschDmrsConfigTypes = DMRS_CONFIG_TYPE_1;
   }
   else if((value & FAPI_PUSCH_DMRS_CONFIG_TYPE_2_MASK) == FAPI_PUSCH_DMRS_CONFIG_TYPE_2_MASK)
   {
      (*cellPtr)->puschDmrsConfigTypes = DMRS_CONFIG_TYPE_2;
   }
   else
   {
      (*cellPtr)->puschDmrsConfigTypes = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PUSCH DmrsLength by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPuschDmrsLength
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PUSCH DmrsLength
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPuschDmrsLength(uint8_t value, ClCellParam **cellPtr)
{
   if(value  == FAPI_PUSCH_DMRS_MAX_LENGTH_1)
   {
      (*cellPtr)->puschDmrsMaxLength = DMRS_MAX_LENGTH_1;
   }
   else if(value  == FAPI_PUSCH_DMRS_MAX_LENGTH_2)
   {
      (*cellPtr)->puschDmrsMaxLength = DMRS_MAX_LENGTH_2;
   }
   else
   {
      (*cellPtr)->puschDmrsMaxLength = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PUSCH Dmrs Additional position by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPuschDmrsAddPos
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PUSCH DmrsAddPos
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPuschDmrsAddPos(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_DMRS_ADDITIONAL_POS_0_MASK) == FAPI_DMRS_ADDITIONAL_POS_0_MASK)
   {
      (*cellPtr)->puschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_0;
   }
   else if((value & FAPI_DMRS_ADDITIONAL_POS_1_MASK) == FAPI_DMRS_ADDITIONAL_POS_1_MASK)
   {
      (*cellPtr)->puschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_1;
   }
   else if((value & FAPI_DMRS_ADDITIONAL_POS_2_MASK) == FAPI_DMRS_ADDITIONAL_POS_2_MASK)
   {
      (*cellPtr)->puschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_2;
   }
   else if((value & FAPI_DMRS_ADDITIONAL_POS_3_MASK) == FAPI_DMRS_ADDITIONAL_POS_3_MASK)
   {
      (*cellPtr)->puschDmrsAdditionalPos = DMRS_ADDITIONAL_POS_3;
   }
   else
   {
      (*cellPtr)->puschDmrsAdditionalPos = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PUSCH Mapping Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPuschMappingType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PUSCH MappingType
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPuschMappingType(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PUSCH_MAPPING_TYPE_A_MASK) == FAPI_PUSCH_MAPPING_TYPE_A_MASK)
   {
      (*cellPtr)->puschMappingType = MAPPING_TYPE_A;
   }
   else if((value & FAPI_PUSCH_MAPPING_TYPE_B_MASK) == FAPI_PUSCH_MAPPING_TYPE_B_MASK)
   {
      (*cellPtr)->puschMappingType = MAPPING_TYPE_B;
   }
   else
   {
      (*cellPtr)->puschMappingType = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PUSCH Allocation Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPuschAllocationType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PUSCH AllocationType
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPuschAllocationType(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PUSCH_ALLOC_TYPE_0_MASK) == FAPI_PUSCH_ALLOC_TYPE_0_MASK)
   {
      (*cellPtr)->puschAllocationTypes = ALLOCATION_TYPE_0;
   }
   else if((value & FAPI_PUSCH_ALLOC_TYPE_0_MASK) == FAPI_PUSCH_ALLOC_TYPE_0_MASK)
   {
      (*cellPtr)->puschAllocationTypes = ALLOCATION_TYPE_1;
   }
   else
   {
      (*cellPtr)->puschAllocationTypes = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PUSCH PRB Mapping Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPuschPrbMappingType
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PUSCH PRB MApping Type
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPuschPrbMappingType(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PUSCH_VRB_TO_PRB_MAP_NON_INTLV_MASK) == FAPI_PUSCH_VRB_TO_PRB_MAP_NON_INTLV_MASK)
   {
      (*cellPtr)->puschVrbToPrbMapping = VRB_TO_PRB_MAP_NON_INTLV;
   }
   else if((value & FAPI_PUSCH_VRB_TO_PRB_MAP_INTLVD_MASK) == FAPI_PUSCH_VRB_TO_PRB_MAP_INTLVD_MASK)
   {
      (*cellPtr)->puschVrbToPrbMapping = VRB_TO_PRB_MAP_INTLVD;
   }
   else
   {
      (*cellPtr)->puschVrbToPrbMapping = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the Modulation Order in Ul by comparing the bitmask
  *
  * @details
  *
  *    Function : fillModulationOrderUl
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's Modualtsion Order in UL.
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillModulationOrderUl(uint8_t value, ClCellParam **cellPtr)
{
   if(value == 0)
   {
      (*cellPtr)->supportedModulationOrderUl = MOD_QPSK;
   }
   else if(value == 1)
   {
      (*cellPtr)->supportedModulationOrderUl = MOD_16QAM;
   }
   else if(value == 2)
   {
      (*cellPtr)->supportedModulationOrderUl = MOD_64QAM;
   }
   else if(value == 3)
   {
      (*cellPtr)->supportedModulationOrderUl = MOD_256QAM;
   }
   else
   {
      (*cellPtr)->supportedModulationOrderUl = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PUSCH Aggregation Factor by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPuschAggregationFactor
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PUSCH Aggregation Factor
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPuschAggregationFactor(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_FORMAT_0_MASK) == FAPI_FORMAT_0_MASK)
   {
      (*cellPtr)->puschAggregationFactor    = AGG_FACTOR_1;
   }
   else if((value & FAPI_FORMAT_1_MASK) == FAPI_FORMAT_1_MASK)
   {
      (*cellPtr)->puschAggregationFactor    = AGG_FACTOR_2;
   }
   else if((value & FAPI_FORMAT_2_MASK) == FAPI_FORMAT_2_MASK)
   {
      (*cellPtr)->puschAggregationFactor    = AGG_FACTOR_4;
   }
   else if((value & FAPI_FORMAT_3_MASK) == FAPI_FORMAT_3_MASK)
   {
      (*cellPtr)->puschAggregationFactor    = AGG_FACTOR_8;
   }
   else
   {
      (*cellPtr)->puschAggregationFactor    = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PRACH Long Format by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPrachLongFormat
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PRACH Long Format
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPrachLongFormat(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PRACH_LF_FORMAT_0_MASK) == FAPI_PRACH_LF_FORMAT_0_MASK)
   {
      (*cellPtr)->prachLongFormats    = FORMAT_0;
   }
   else if((value & FAPI_PRACH_LF_FORMAT_1_MASK) == FAPI_PRACH_LF_FORMAT_1_MASK)
   {
      (*cellPtr)->prachLongFormats    = FORMAT_1;
   }
   else if((value & FAPI_PRACH_LF_FORMAT_2_MASK) == FAPI_PRACH_LF_FORMAT_2_MASK)
   {
      (*cellPtr)->prachLongFormats    = FORMAT_2;
   }
   else if((value & FAPI_PRACH_LF_FORMAT_3_MASK) == FAPI_PRACH_LF_FORMAT_3_MASK)
   {
      (*cellPtr)->prachLongFormats    = FORMAT_3;
   }
   else
   {
      (*cellPtr)->prachLongFormats    = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the PRACH Short Format by comparing the bitmask
  *
  * @details
  *
  *    Function : fillPrachShortFormat
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's PRACH ShortFormat
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillPrachShortFormat(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_PRACH_SF_FORMAT_A1_MASK) == FAPI_PRACH_SF_FORMAT_A1_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_A1;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_A2_MASK) == FAPI_PRACH_SF_FORMAT_A2_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_A2;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_A3_MASK) == FAPI_PRACH_SF_FORMAT_A3_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_A3;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_B1_MASK) == FAPI_PRACH_SF_FORMAT_B1_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_B1;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_B2_MASK) == FAPI_PRACH_SF_FORMAT_B2_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_B2;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_B3_MASK) == FAPI_PRACH_SF_FORMAT_B3_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_B3;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_B4_MASK) == FAPI_PRACH_SF_FORMAT_B4_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_B4;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_C0_MASK) == FAPI_PRACH_SF_FORMAT_C0_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_C0;
   }
   else if((value & FAPI_PRACH_SF_FORMAT_C2_MASK) == FAPI_PRACH_SF_FORMAT_C2_MASK)
   {
      (*cellPtr)->prachShortFormats    = SF_FORMAT_C2;
   }
   else
   {
      (*cellPtr)->prachShortFormats    = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the Fd Occasions Type by comparing the bitmask
  *
  * @details
  *
  *    Function : fillFdOccasions
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's Fd Occasions
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillFdOccasions(uint8_t value, ClCellParam **cellPtr)
{
   if(value == 0)
   {
      (*cellPtr)->maxPrachFdOccasionsInASlot = PRACH_FD_OCC_IN_A_SLOT_1;
   }
   else if(value == 1)
   {
      (*cellPtr)->maxPrachFdOccasionsInASlot = PRACH_FD_OCC_IN_A_SLOT_2;
   }
   else if(value == 3)
   {
      (*cellPtr)->maxPrachFdOccasionsInASlot = PRACH_FD_OCC_IN_A_SLOT_4;
   }
   else if(value == 4)
   {
      (*cellPtr)->maxPrachFdOccasionsInASlot = PRACH_FD_OCC_IN_A_SLOT_8;
   }
   else
   {
      (*cellPtr)->maxPrachFdOccasionsInASlot = INVALID_VALUE;
   }
}

/*******************************************************************
  *
  * @brief fills the RSSI Measurement by comparing the bitmask
  *
  * @details
  *
  *    Function : fillRssiMeas
  *
  *    Functionality:
  *         -checks the value with the bitmask and
  *          fills the cellPtr's RSSI Measurement report
  *
  * @params[in] Pointer to ClCellParam
  *             Value to be compared
  * @return void
  *
  ******************************************************************/

PUBLIC void fillRssiMeas(uint8_t value, ClCellParam **cellPtr)
{
   if((value & FAPI_RSSI_REPORT_IN_DBM_MASK) == FAPI_RSSI_REPORT_IN_DBM_MASK)
   {
      (*cellPtr)->rssiMeasurementSupport    = RSSI_REPORT_DBM;
   }
   else if((value & FAPI_RSSI_REPORT_IN_DBFS_MASK) == FAPI_RSSI_REPORT_IN_DBFS_MASK)
   {
      (*cellPtr)->rssiMeasurementSupport    = RSSI_REPORT_DBFS;
   }
   else
   {
      (*cellPtr)->rssiMeasurementSupport    = INVALID_VALUE;
   }
}

 /*******************************************************************
  *
  * @brief Returns the TLVs value
  *
  * @details
  *
  *    Function : getParamValue
  *
  *    Functionality:
  *         -return TLVs value
  *
  * @params[in]
  * @return ROK     - temp
  *         RFAILED - failure
  *
  * ****************************************************************/

uint32_t getParamValue(fapi_uint16_tlv_t *tlv, uint16_t type)
{
   //uint16_t valueLen;
   void *posPtr;
   //valueLen = tlv->tl.length;
   posPtr   = &tlv->tl.tag;
   posPtr   += sizeof(tlv->tl.tag);
   posPtr   += sizeof(tlv->tl.length);
   /*TO DO: malloc to SSI memory */
   if(type == FAPI_UINT_8)
   {
      //temp = (uint8_t *)malloc(valueLen * sizeof(U8));
      //memcpy(temp, posPtr, valueLen);
      return(*(uint8_t *)posPtr);
   }
   else if(type == FAPI_UINT_16)
   {
      return(*(uint16_t *)posPtr);
   }
   else if(type == FAPI_UINT_32)
   {
      return(*(uint32_t *)posPtr);
   }
   else
   {
     DU_LOG("\nLWR_MAC: Value Extraction failed" );
     return RFAILED;
   }
}
#endif /* FAPI */
 /*******************************************************************
  *
  * @brief Sends FAPI Param req to PHY
  *
  * @details
  *
  *    Function : lwr_mac_handleParamReqEvt
  *
  *    Functionality:
  *         -Sends FAPI Param req to PHY
  *
  * @params[in]
  * @return ROK     - success
  *         RFAILED - failure
  *
  * ****************************************************************/

S16 lwr_mac_handleParamReqEvt(void *msg)
{
#ifdef FAPI
   /* startGuardTimer(); */
   uint32_t msgLen = 0;             //Length of message Body
   fapi_param_req_t *paramReq = NULL;

   LWR_MAC_ALLOC(paramReq, sizeof(fapi_param_req_t));
   if(paramReq != NULL)
   {
      fillMsgHeader(&paramReq->header, FAPI_PARAM_REQUEST, msgLen);

      DU_LOG("\nLWR_MAC: Sending Param Request to Phy");
      LwrMacSendToPhy(paramReq->header.message_type_id, \
         sizeof(fapi_param_req_t), (void *)paramReq);
   }
   else
   {
      DU_LOG("\nLWR_MAC: Failed to allocate memory for Param Request");
      return RFAILED;
   }
#endif
   return ROK;
}

 /*******************************************************************
  *
  * @brief Sends FAPI Param Response to MAC via PHY
  *
  * @details
  *
  *    Function : lwr_mac_handleParamRspEvt
  *
  *    Functionality:
  *         -Sends FAPI Param rsp to MAC via PHY
  *
  * @params[in]
  * @return ROK     - success
  *         RFAILED - failure
  *
  * ****************************************************************/

S16 lwr_mac_handleParamRspEvt(void *msg)
{
#ifdef FAPI
  /* stopGuardTimer(); */
   uint8_t index;
   uint32_t encodedVal;
   fapi_param_resp_t *paramRsp;
   ClCellParam *cellParam = NULLP;

   paramRsp = (fapi_param_resp_t *)msg;
   DU_LOG("\nLWR_MAC: Received EVENT[%d] at STATE[%d]", clGlobalCp.event, clGlobalCp.phyState);

   if(paramRsp != NULLP)
   {
      MAC_ALLOC(cellParam, sizeof(ClCellParam));
      if(cellParam != NULLP)
      {
         DU_LOG("\n LWR_MAC: Filling TLVS into MAC API");
         if(paramRsp->error_code == MSG_OK)
         {
            for(index = 0; index < paramRsp->number_of_tlvs; index++)
            {
               switch(paramRsp->tlvs[index].tl.tag)
               {
                  case FAPI_RELEASE_CAPABILITY_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_16);
                     if(encodedVal != RFAILED && (encodedVal & RELEASE_15) == RELEASE_15)
                     {
                        cellParam->releaseCapability = RELEASE_15;
                     }
                     break;

                  case FAPI_PHY_STATE_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != clGlobalCp.phyState)
                     {
                        printf("\n PhyState mismatch [%d][%d]", clGlobalCp.phyState, clGlobalCp.event);
                        RETVALUE(RFAILED);
                     }
                     break;

                  case FAPI_SKIP_BLANK_DL_CONFIG_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->skipBlankDlConfig = SUPPORTED;
                     }
                     else
                     {
                        cellParam->skipBlankDlConfig = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_SKIP_BLANK_UL_CONFIG_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->skipBlankUlConfig = SUPPORTED;
                     }
                     else
                     {
                        cellParam->skipBlankUlConfig = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_NUM_CONFIG_TLVS_TO_REPORT_TYPE_TAG:
                     cellParam->numTlvsToReport = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_16);
                     break;

                  case FAPI_CYCLIC_PREFIX_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillCyclicPrefix(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_SUPPORTED_SUBCARRIER_SPACING_DL_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillSubcarrierSpaceDl(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_SUPPORTED_BANDWIDTH_DL_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_16);
                     if(encodedVal != RFAILED)
                     {
                        fillBandwidthDl(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_SUPPORTED_SUBCARRIER_SPACING_UL_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillSubcarrierSpaceUl(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_SUPPORTED_BANDWIDTH_UL_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_16);
                     if(encodedVal != RFAILED)
                     {
                        fillBandwidthUl(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_CCE_MAPPING_TYPE_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillCCEmaping(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_CORESET_OUTSIDE_FIRST_3_OFDM_SYMS_OF_SLOT_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->coresetOutsideFirst3OfdmSymsOfSlot = SUPPORTED;
                     }
                     else
                     {
                        cellParam->coresetOutsideFirst3OfdmSymsOfSlot = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_PRECODER_GRANULARITY_CORESET_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->precoderGranularityCoreset = SUPPORTED;
                     }
                     else
                     {
                        cellParam->precoderGranularityCoreset = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_PDCCH_MU_MIMO_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->pdcchMuMimo = SUPPORTED;
                     }
                     else
                     {
                        cellParam->pdcchMuMimo = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_PDCCH_PRECODER_CYCLING_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->pdcchPrecoderCycling = SUPPORTED;
                     }
                     else
                     {
                        cellParam->pdcchPrecoderCycling = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_MAX_PDCCHS_PER_SLOT_TAG:
                     cellParam->maxPdcchsPerSlot = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     break;

                  case FAPI_PUCCH_FORMATS_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillPucchFormat(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_MAX_PUCCHS_PER_SLOT_TAG:
                   cellParam->maxPucchsPerSlot   = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     break;

                  case FAPI_PDSCH_MAPPING_TYPE_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillPdschMappingType(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_PDSCH_ALLOCATION_TYPES_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillPdschAllocationType(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_PDSCH_VRB_TO_PRB_MAPPING_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillPrbMappingType(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_PDSCH_CBG_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->pdschCbg = SUPPORTED;
                     }
                     else
                     {
                        cellParam->pdschCbg = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_PDSCH_DMRS_CONFIG_TYPES_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillPdschDmrsConfigType(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_PDSCH_DMRS_MAX_LENGTH_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillPdschDmrsLength(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_PDSCH_DMRS_ADDITIONAL_POS_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillPdschDmrsAddPos(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_MAX_PDSCHS_TBS_PER_SLOT_TAG:
                     cellParam->maxPdschsTBsPerSlot = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     break;

                  case FAPI_MAX_NUMBER_MIMO_LAYERS_PDSCH_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal < FAPI_MAX_NUMBERMIMO_LAYERS_PDSCH)
                     {
                        cellParam->maxNumberMimoLayersPdsch   = encodedVal;
                     }
                     break;

                  case FAPI_SUPPORTED_MAX_MODULATION_ORDER_DL_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED)
                     {
                        fillModulationOrderDl(encodedVal, &cellParam);
                     }
                     break;

                  case FAPI_MAX_MU_MIMO_USERS_DL_TAG:
                     cellParam->maxMuMimoUsersDl         = \
							   getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     break;

                  case FAPI_PDSCH_DATA_IN_DMRS_SYMBOLS_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->pdschDataInDmrsSymbols = SUPPORTED;
                     }
                     else
                     {
                        cellParam->pdschDataInDmrsSymbols = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_PREMPTIONSUPPORT_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->premptionSupport = SUPPORTED;
                     }
                     else
                     {
                        cellParam->premptionSupport = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_PDSCH_NON_SLOT_SUPPORT_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->pdschNonSlotSupport = SUPPORTED;
                     }
                     else
                     {
                        cellParam->pdschNonSlotSupport = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_UCI_MUX_ULSCH_IN_PUSCH_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->uciMuxUlschInPusch = SUPPORTED;
                     }
                     else
                     {
                        cellParam->uciMuxUlschInPusch = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_UCI_ONLY_PUSCH_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->uciOnlyPusch = SUPPORTED;
                     }
                     else
                     {
                        cellParam->uciOnlyPusch = NOT_SUPPORTED;
                     }
                     break;

                  case FAPI_PUSCH_FREQUENCY_HOPPING_TAG:
                     encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                     if(encodedVal != RFAILED && encodedVal != 0)
                     {
                        cellParam->puschFrequencyHopping = SUPPORTED;
                     }
                     else
                     {
                        cellParam->puschFrequencyHopping = NOT_SUPPORTED;
                     }
                     break;

                 case FAPI_PUSCH_DMRS_CONFIG_TYPES_TAG:
                    encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                    if(encodedVal != RFAILED)
                    {
                       fillPuschDmrsConfig(encodedVal, &cellParam);
                    }
                    break;

                 case FAPI_PUSCH_DMRS_MAX_LEN_TAG:
                    encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                    if(encodedVal != RFAILED)
                    {
                       fillPuschDmrsLength(encodedVal, &cellParam);
                    }
                    break;

                 case FAPI_PUSCH_DMRS_ADDITIONAL_POS_TAG:
                    encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                    if(encodedVal != RFAILED)
                    {
                       fillPuschDmrsAddPos(encodedVal, &cellParam);
                    }
                    break;

                 case FAPI_PUSCH_CBG_TAG:
                    encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                    if(encodedVal != RFAILED && encodedVal != 0)
                    {
                       cellParam->puschCbg = SUPPORTED;
                    }
                    else
                    {
                       cellParam->puschCbg = NOT_SUPPORTED;
                    }
                    break;

                case FAPI_PUSCH_MAPPING_TYPE_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED)
                   {
                      fillPuschMappingType(encodedVal, &cellParam);
                   }
                   break;

                case FAPI_PUSCH_ALLOCATION_TYPES_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED)
                   {
                      fillPuschAllocationType(encodedVal, &cellParam);
                   }
                   break;

                case FAPI_PUSCH_VRB_TO_PRB_MAPPING_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED)
                   {
                      fillPuschPrbMappingType(encodedVal, &cellParam);
                   }
                   break;

                case FAPI_PUSCH_MAX_PTRS_PORTS_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED && encodedVal < FAPI_PUSCH_MAX_PTRS_PORTS_UB)
                   {
                      cellParam->puschMaxPtrsPorts = encodedVal;
                   }
                   break;

                case FAPI_MAX_PDUSCHS_TBS_PER_SLOT_TAG:
                   cellParam->maxPduschsTBsPerSlot = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   break;

                case FAPI_MAX_NUMBER_MIMO_LAYERS_NON_CB_PUSCH_TAG:
                   cellParam->maxNumberMimoLayersNonCbPusch = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   break;

                case FAPI_SUPPORTED_MODULATION_ORDER_UL_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED)
                   {
                      fillModulationOrderUl(encodedVal, &cellParam);
                   }
                   break;

                case FAPI_MAX_MU_MIMO_USERS_UL_TAG:
                   cellParam->maxMuMimoUsersUl = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   break;

                case FAPI_DFTS_OFDM_SUPPORT_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED && encodedVal != 0)
                   {
                      cellParam->dftsOfdmSupport = SUPPORTED;
                   }
                   else
                   {
                      cellParam->dftsOfdmSupport = NOT_SUPPORTED;
                   }
                   break;

                case FAPI_PUSCH_AGGREGATION_FACTOR_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED)
                   {
                      fillPuschAggregationFactor(encodedVal, &cellParam);
                   }
                   break;

                case FAPI_PRACH_LONG_FORMATS_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED)
                   {
                      fillPrachLongFormat(encodedVal, &cellParam);
                   }
                   break;

                case FAPI_PRACH_SHORT_FORMATS_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED)
                   {
                      fillPrachShortFormat(encodedVal, &cellParam);
                   }
                   break;

                case FAPI_PRACH_RESTRICTED_SETS_TAG:
                   encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                   if(encodedVal != RFAILED && encodedVal != 0)
                   {
                      cellParam->prachRestrictedSets = SUPPORTED;
                   }
                   else
                   {
                      cellParam->prachRestrictedSets = NOT_SUPPORTED;
                   }
                   break;

               case FAPI_MAX_PRACH_FD_OCCASIONS_IN_A_SLOT_TAG:
                  encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                  if(encodedVal != RFAILED)
                  {
                     fillFdOccasions(encodedVal, &cellParam);
                  }
                  break;

               case FAPI_RSSI_MEASUREMENT_SUPPORT_TAG:
                  encodedVal = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
                  if(encodedVal != RFAILED)
                  {
                     fillRssiMeas(encodedVal, &cellParam);
                  }
                  break;
               default:
               //printf("\n Invalid value for TLV[%x] at index[%d]", paramRsp->tlvs[index].tl.tag, index);
               break;
              }
           }
           MAC_FREE(cellParam, sizeof(ClCellParam));
           sendToLowerMac(FAPI_CONFIG_REQUEST, 0, (void *)NULL);
           return ROK;
         }
         else
         {
            DU_LOG("\n LWR_MAC: Invalid error code %d", paramRsp->error_code);
            return RFAILED;
         }
     }
     else
     {
        DU_LOG("\nLWR_MAC: Failed to allocate memory for cell param");
        return RFAILED;
     }
   }
   else
   {
       DU_LOG("\nLWR_MAC:  Param Response received from PHY is NULL");
       return RFAILED;
   }
#else
   return ROK;
#endif
}

 /*******************************************************************
  *
  * @brief Sends FAPI Config req to PHY
  *
  * @details
  *
  *    Function : lwr_mac_handleConfigReqEvt
  *
  *    Functionality:
  *         -Sends FAPI Config Req to PHY
  *
  * @params[in]
  * @return ROK     - success
  *         RFAILED - failure
  *
  * ****************************************************************/

S16 lwr_mac_handleConfigReqEvt(void *msg)
{
#ifdef FAPI
   Inst inst = 0;
   uint8_t idx = 0;
   uint8_t index = 0;
   uint32_t msgLen = 0;
   uint32_t configReqSize;
   RgCellCb  *cellParams;
   MacCellCfg macCfgParams;
   fapi_config_req_t *configReq;

   DU_LOG("\nLWR_MAC: Received EVENT[%d] at STATE[%d]", clGlobalCp.event, \
      clGlobalCp.phyState);

   cellParams = rgCb[inst].cell;
   macCfgParams = cellParams->macCellCfg;

   configReqSize = sizeof(fapi_config_req_t);
   LWR_MAC_ALLOC(configReq, configReqSize);
   if(configReq != NULL)
   {
      msgLen = sizeof(macCfgParams.numTlv);
      configReq->number_of_tlvs = macCfgParams.numTlv;

      if(macCfgParams.dlCarrCfg.pres)
      {
         fillTlvs(&configReq->tlvs[index++], FAPI_DL_BANDWIDTH_TAG,           \
            sizeof(uint16_t), macCfgParams.dlCarrCfg.bw, &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_DL_FREQUENCY_TAG,           \
            sizeof(uint32_t), macCfgParams.dlCarrCfg.freq, &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_DL_K0_TAG,                  \
            sizeof(uint16_t), macCfgParams.dlCarrCfg.k0[0], &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_DL_GRIDSIZE_TAG,            \
            sizeof(uint16_t), macCfgParams.dlCarrCfg.gridSize[0], &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_NUM_TX_ANT_TAG,             \
            sizeof(uint16_t), macCfgParams.dlCarrCfg.numAnt, &msgLen);
      }
      if(macCfgParams.ulCarrCfg.pres)
      {
         fillTlvs(&configReq->tlvs[index++], FAPI_UPLINK_BANDWIDTH_TAG,       \
            sizeof(uint16_t), macCfgParams.ulCarrCfg.bw, &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_UPLINK_FREQUENCY_TAG,       \
            sizeof(uint32_t), macCfgParams.ulCarrCfg.freq, &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_UL_K0_TAG,                  \
            sizeof(uint16_t), macCfgParams.ulCarrCfg.k0[0], &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_UL_GRID_SIZE_TAG,           \
            sizeof(uint16_t), macCfgParams.ulCarrCfg.gridSize[0], &msgLen);
         fillTlvs(&configReq->tlvs[index++], FAPI_NUM_RX_ANT_TAG,             \
            sizeof(uint16_t), macCfgParams.ulCarrCfg.numAnt, &msgLen);
      }
      fillTlvs(&configReq->tlvs[index++], FAPI_FREQUENCY_SHIFT_7P5_KHZ_TAG,   \
         sizeof(uint8_t), macCfgParams.freqShft, &msgLen);

      /* fill cell config */
      fillTlvs(&configReq->tlvs[index++], FAPI_PHY_CELL_ID_TAG,               \
         sizeof(uint8_t), macCfgParams.phyCellId, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_FRAME_DUPLEX_TYPE_TAG,         \
         sizeof(uint8_t), macCfgParams.dupType, &msgLen);

      /* fill SSB configuration */
      fillTlvs(&configReq->tlvs[index++], FAPI_SS_PBCH_POWER_TAG,             \
         sizeof(uint32_t), macCfgParams.ssbCfg.ssbPbchPwr, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_BCH_PAYLOAD_TAG,               \
         sizeof(uint8_t), macCfgParams.ssbCfg.bchPayloadFlag, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_SCS_COMMON_TAG,                \
         sizeof(uint8_t), macCfgParams.ssbCfg.scsCmn, &msgLen);

      /* fill PRACH configuration */
      fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_SEQUENCE_LENGTH_TAG,     \
         sizeof(uint8_t), macCfgParams.prachCfg.prachSeqLen, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_SUBC_SPACING_TAG,        \
         sizeof(uint8_t), macCfgParams.prachCfg.prachSubcSpacing, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_RESTRICTED_SET_CONFIG_TAG,     \
         sizeof(uint8_t), macCfgParams.prachCfg.prachRstSetCfg, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_NUM_PRACH_FD_OCCASIONS_TAG,
         sizeof(uint8_t), macCfgParams.prachCfg.msg1Fdm, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_ROOT_SEQUENCE_INDEX_TAG, \
         sizeof(uint16_t), macCfgParams.prachCfg.fdm[0].rootSeqIdx, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_NUM_ROOT_SEQUENCES_TAG,        \
         sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].numRootSeq, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_K1_TAG,                        \
         sizeof(uint16_t), macCfgParams.prachCfg.fdm[0].k1, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_ZERO_CORR_CONF_TAG ,     \
         sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].zeroCorrZoneCfg, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_NUM_UNUSED_ROOT_SEQUENCES_TAG, \
         sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].numUnusedRootSeq, &msgLen);
      if(macCfgParams.prachCfg.fdm[0].numUnusedRootSeq)
      {
         for(idx = 0; idx < macCfgParams.prachCfg.fdm[0].numUnusedRootSeq; idx++)
            fillTlvs(&configReq->tlvs[index++], FAPI_UNUSED_ROOT_SEQUENCES_TAG,   \
               sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].unsuedRootSeq[idx], \
               &msgLen);
      }
      else
      {
         macCfgParams.prachCfg.fdm[0].unsuedRootSeq = NULL;
      }

      fillTlvs(&configReq->tlvs[index++], FAPI_SSB_PER_RACH_TAG,              \
         sizeof(uint8_t), macCfgParams.prachCfg.ssbPerRach, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_MULTIPLE_CARRIERS_IN_A_BAND_TAG,  \
         sizeof(uint8_t), macCfgParams.prachCfg.prachMultCarrBand, &msgLen);

      /* fill SSB table */
      fillTlvs(&configReq->tlvs[index++], FAPI_SSB_OFFSET_POINT_A_TAG,        \
         sizeof(uint16_t), macCfgParams.ssbCfg.ssbOffsetPointA, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_BETA_PSS_TAG,                  \
         sizeof(uint8_t),  macCfgParams.ssbCfg.betaPss, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_SSB_PERIOD_TAG,                \
         sizeof(uint8_t),  macCfgParams.ssbCfg.ssbPeriod, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_SSB_SUBCARRIER_OFFSET_TAG,     \
         sizeof(uint8_t),  macCfgParams.ssbCfg.ssbScOffset, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_MIB_TAG ,                      \
         sizeof(uint32_t), macCfgParams.ssbCfg.mibPdu[0], &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_SSB_MASK_TAG,                  \
         sizeof(uint32_t), macCfgParams.ssbCfg.ssbMask[0], &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_BEAM_ID_TAG,                   \
         sizeof(uint8_t),  macCfgParams.ssbCfg.beamId[0], &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_SS_PBCH_MULTIPLE_CARRIERS_IN_A_BAND_TAG, \
         sizeof(uint8_t), macCfgParams.ssbCfg.multCarrBand, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_MULTIPLE_CELLS_SS_PBCH_IN_A_CARRIER_TAG, \
         sizeof(uint8_t), macCfgParams.ssbCfg.multCellCarr, &msgLen);

      /* fill TDD table */
      fillTlvs(&configReq->tlvs[index++], FAPI_TDD_PERIOD_TAG,                \
         sizeof(uint8_t), macCfgParams.tddCfg.tddPeriod, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_SLOT_CONFIG_TAG,               \
         sizeof(uint8_t), macCfgParams.tddCfg.slotCfg[0][0], &msgLen);

      /* fill measurement config */
      fillTlvs(&configReq->tlvs[index++], FAPI_RSSI_MESUREMENT_TAG,           \
         sizeof(uint8_t), macCfgParams.rssiUnit, &msgLen);

      fillMsgHeader(&configReq->header, FAPI_CONFIG_REQUEST, msgLen);

      DU_LOG("\nLWR_MAC: Sending Config Request to Phy");
      LwrMacSendToPhy(configReq->header.message_type_id, configReqSize, (void *)configReq);
   }
   else
   {
      DU_LOG("\nLWR_MAC: Failed to allocate memory for config Request");
      return RFAILED;
   }
#endif

   return ROK;
} /* lwr_mac_handleConfigReqEvt */

/*******************************************************************
 *
 * @brief Processes config response from phy
 *
 * @details
 *
 *    Function : lwr_mac_handleConfigRspEvt
 *
 *    Functionality:
 *          Processes config response from phy
 *
 * @params[in] FAPI message pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 lwr_mac_handleConfigRspEvt(void *msg)
{
#ifdef FAPI
   fapi_config_resp_t *configRsp;
   configRsp = (fapi_config_resp_t *)msg;

   DU_LOG("\nLWR_MAC: Received EVENT[%d] at STATE[%d]", clGlobalCp.event, \
      clGlobalCp.phyState);

   if(configRsp != NULL)
   {
      if(configRsp->error_code == MSG_OK)
      {
         DU_LOG("\nLWR_MAC: PHY has moved to Configured state \n");
         clGlobalCp.phyState = PHY_STATE_CONFIGURED;
         /* TODO : 
          * Store config response into an intermediate struture and send to MAC
          * Support LC and LWLC for sending config rsp to MAC 
          */
         fapiMacConfigRsp();
      }
      else
      {
         DU_LOG("\n LWR_MAC: Invalid error code %d", configRsp->error_code);
         return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nLWR_MAC: Config Response received from PHY is NULL");
      return RFAILED;
   }
#endif

   return ROK;
} /* lwr_mac_handleConfigRspEvt */

/*******************************************************************
 *
 * @brief Build and send start request to phy
 *
 * @details
 *
 *    Function : lwr_mac_handleStartReqEvt
 *
 *    Functionality:
 *       Build and send start request to phy
 *
 * @params[in] FAPI message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 lwr_mac_handleStartReqEvt(void *msg)
{
#ifdef FAPI
   uint32_t msgLen = 0;
   fapi_start_req_t *startReq;

   LWR_MAC_ALLOC(startReq, sizeof(fapi_start_req_t));
   if(startReq != NULL)
   {
      fillMsgHeader(&startReq->header, FAPI_START_REQUEST, msgLen);

      DU_LOG("\nLWR_MAC: Sending Start Request to PHY");
      LwrMacSendToPhy(startReq->header.message_type_id, sizeof(fapi_start_req_t),\
         (void *)startReq);
   }
   else
   {
      DU_LOG("\nLWR_MAC: Failed to allocate memory for Start Request");
      return RFAILED;
   }
#endif
   return ROK;
} /* lwr_mac_handleStartReqEvt */

 /*******************************************************************
  *
  * @brief Sends FAPI Stop Req to PHY
  *
  * @details
  *
  *    Function : lwr_mac_handleStopReqEvt
  *
  *    Functionality:
  *         -Sends FAPI Stop Req to PHY
  *
  * @params[in]
  * @return ROK     - success
  *         RFAILED - failure
  *
  ********************************************************************/

S16 lwr_mac_handleStopReqEvt(void *msg)
{
#ifdef FAPI
   uint32_t msgLen = 0;
   fapi_stop_req_t *stopReq = NULLP;
   LWR_MAC_ALLOC(stopReq, sizeof(fapi_stop_req_t));
   if(stopReq != NULLP)
   {
      fillMsgHeader(&stopReq->header, FAPI_STOP_REQUEST, msgLen);
      DU_LOG("\nLOWER MAC: Sending Stop Request to PHY");
      LwrMacSendToPhy(stopReq->header.message_type_id, sizeof(fapi_stop_req_t), (void *)stopReq);
   }
   else
   {
      DU_LOG("\nLOWER MAC: Failed to allocate memory for Stop Request");
      return RFAILED;
   }
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Modifes the received mibPdu to uint32 bit
 *        and stores it in MacCellCfg
 *
 * @details
 *
 *    Function : setMibPdu
 *
 *    Functionality:
 *         -Sets the MibPdu
 *
 * @params[in] Pointer to mibPdu
 *             pointer to modified value
 ******************************************************************/

PUBLIC void setMibPdu(uint8_t *mibPdu, uint32_t *val, uint16_t sfn)
{
   *mibPdu |= (((uint8_t)(sfn >> 2)) & MIB_SFN_BITMASK);
   *val = (mibPdu[0] << 24 | mibPdu[1] << 16 | mibPdu[2] << 8);
    DU_LOG("\nLWR_MAC: MIB PDU %x", *val);
}

#ifdef FAPI
/*******************************************************************
 *
 * @brief fills SSB PDU required for DL TTI info in MAC
 *
 * @details
 *
 *    Function : fillSsbPdu
 *
 *    Functionality:
 *         -Fills the SSB PDU info
 *          stored in MAC
 *
 * @params[in] Pointer to FAPI DL TTI Req
 *             Pointer to RgCellCb
 *             Pointer to msgLen of DL TTI Info
 * @return ROK
 *
 ******************************************************************/

S16 fillSsbPdu(fapi_dl_tti_req_pdu_t *dlTtiReqPdu, MacCellCfg *macCellCfg,
   MacDlSlot *currDlSlot, uint32_t *msgLen, uint8_t ssbIdxCount, uint16_t sfn)
{
   uint32_t mibPayload = 0;
   if(dlTtiReqPdu != NULL)
   {
      dlTtiReqPdu->pduType = SSB_PDU_TYPE;     /* SSB PDU */
      dlTtiReqPdu->u.ssb_pdu.physCellId = macCellCfg->phyCellId;
      dlTtiReqPdu->u.ssb_pdu.betaPss = macCellCfg->ssbCfg.betaPss;
      dlTtiReqPdu->u.ssb_pdu.ssbBlockIndex = currDlSlot->dlInfo.brdcstAlloc.ssbInfo[ssbIdxCount].ssbIdx;
      dlTtiReqPdu->u.ssb_pdu.ssbSubCarrierOffset = macCellCfg->ssbCfg.ssbScOffset;
      /* ssbOfPdufstA to be filled in ssbCfg */
      dlTtiReqPdu->u.ssb_pdu.ssbOffsetPointA = macCellCfg->ssbCfg.ssbOffsetPointA;
      dlTtiReqPdu->u.ssb_pdu.bchPayloadFlag = macCellCfg->ssbCfg.bchPayloadFlag;
      /* Bit manipulation for SFN */
      setMibPdu(macCellCfg->ssbCfg.mibPdu, &mibPayload, sfn);
      dlTtiReqPdu->u.ssb_pdu.bchPayload.v.bchPayload = mibPayload;
      dlTtiReqPdu->u.ssb_pdu.preCodingAndBeamforming.numPrgs = 0;
      dlTtiReqPdu->u.ssb_pdu.preCodingAndBeamforming.prgSize = 0;
      dlTtiReqPdu->u.ssb_pdu.preCodingAndBeamforming.digBfInterfaces = 0;
      dlTtiReqPdu->u.ssb_pdu.preCodingAndBeamforming.pmi_bfi[0].pmIdx = 0;
      dlTtiReqPdu->u.ssb_pdu.preCodingAndBeamforming. \
         pmi_bfi[0].beamIdx[0].beamidx = macCellCfg->ssbCfg.beamId[0];
      dlTtiReqPdu->pduSize = sizeof(fapi_dl_ssb_pdu_t);  /* Size of SSB PDU */
      SET_MSG_LEN(*msgLen, (sizeof(dlTtiReqPdu->pduType) + \
         sizeof(dlTtiReqPdu->pduSize) + sizeof(fapi_dl_ssb_pdu_t)));
      return ROK;
    }
    else
    {
       return RFAILED;
    }
}

/*******************************************************************
 *
 * @brief fills Dl DCI PDU required for DL TTI info in MAC
 *
 * @details
 *
 *    Function : fillSib1DlDciPdu
 *
 *    Functionality:
 *         -Fills the Dl DCI PDU
 *
 * @params[in] Pointer to fapi_dl_dci_t
 *             Pointer to PdcchCfg
 * @return ROK
 *
 ******************************************************************/

void fillSib1DlDciPdu(fapi_dl_dci_t *dlDciPtr, PdcchCfg *sib1PdcchInfo)
{
   if(dlDciPtr != NULLP)
   {
      uint8_t numBytes;
      uint8_t bytePos;
      uint8_t bitPos;
      
      uint16_t coreset0Size;
      uint16_t rbStart;
      uint16_t rbLen;
      uint32_t freqDomResAssign;
      uint32_t timeDomResAssign;
      uint8_t  VRB2PRBMap;
      uint32_t modNCodScheme;
      uint8_t  redundancyVer;
      uint32_t sysInfoInd;
      uint32_t reserved;

      /* Size(in bits) of each field in DCI format 0_1 
       * as mentioned in spec 38.214 */
      uint8_t freqDomResAssignSize;
      uint8_t timeDomResAssignSize = 4;
      uint8_t VRB2PRBMapSize       = 1;
      uint8_t modNCodSchemeSize    = 5;
      uint8_t redundancyVerSize    = 2;
      uint8_t sysInfoIndSize       = 1;
      uint8_t reservedSize         = 15;

      dlDciPtr->rnti = sib1PdcchInfo->dci.rnti;
      dlDciPtr->scramblingId = sib1PdcchInfo->dci.scramblingId;    
      dlDciPtr->scramblingRnti = sib1PdcchInfo->dci.scramblingRnti;
      dlDciPtr->cceIndex = sib1PdcchInfo->dci.cceIndex;
      dlDciPtr->aggregationLevel = sib1PdcchInfo->dci.aggregLevel;
      dlDciPtr->pc_and_bform.numPrgs = sib1PdcchInfo->dci.beamPdcchInfo.numPrgs;
      dlDciPtr->pc_and_bform.prgSize = sib1PdcchInfo->dci.beamPdcchInfo.prgSize;
      dlDciPtr->pc_and_bform.digBfInterfaces = sib1PdcchInfo->dci.beamPdcchInfo.digBfInterfaces;
      dlDciPtr->pc_and_bform.pmi_bfi[0].pmIdx = sib1PdcchInfo->dci.beamPdcchInfo.prg[0].pmIdx;
      dlDciPtr->pc_and_bform.pmi_bfi[0].beamIdx[0].beamidx = sib1PdcchInfo->dci.beamPdcchInfo.prg[0].beamIdx[0];
      dlDciPtr->beta_pdcch_1_0 = sib1PdcchInfo->dci.txPdcchPower.powerValue;           
      dlDciPtr->powerControlOfssetSS = sib1PdcchInfo->dci.txPdcchPower.powerControlOffsetSS;

      /* Calculating freq domain resource allocation field value and size
       * coreset0Size = Size of coreset 0
       * RBStart = Starting Virtual Rsource block
       * RBLen = length of contiguously allocted RBs
       * Spec 38.214 Sec 5.1.2.2.2
       */
      coreset0Size= sib1PdcchInfo->coreset0Cfg.coreSet0Size;
      rbStart = 0;              /* For SIB1 */
      //rbStart = sib1PdcchInfo->dci.pdschCfg->freqAlloc.rbStart;
      rbLen = sib1PdcchInfo->dci.pdschCfg->freqAlloc.rbSize;

      if((rbLen >=1) && (rbLen <= coreset0Size - rbStart))
      {
         if((rbLen - 1) <= floor(coreset0Size / 2))
            freqDomResAssign = (coreset0Size * (rbLen-1)) + rbStart;
         else
            freqDomResAssign = (coreset0Size * (coreset0Size - rbLen + 1)) \
               + (coreset0Size - 1 - rbStart);

         freqDomResAssignSize = ceil(log2(coreset0Size * (coreset0Size + 1) / 2));
      }

      /* Fetching DCI field values */
      timeDomResAssign = sib1PdcchInfo->dci.pdschCfg->timeAlloc.
                         rowIndex -1;
      VRB2PRBMap       = sib1PdcchInfo->dci.pdschCfg->freqAlloc.\
                         vrbPrbMapping;
      modNCodScheme    = sib1PdcchInfo->dci.pdschCfg->codeword[0].mcsIndex;
      redundancyVer    = sib1PdcchInfo->dci.pdschCfg->codeword[0].rvIndex;
      sysInfoInd       = 0;           /* 0 for SIB1; 1 for SI messages */
      reserved         = 0;

      /* Reversing bits in each DCI field */
      freqDomResAssign = reverseBits(freqDomResAssign, freqDomResAssignSize);
      timeDomResAssign = reverseBits(timeDomResAssign, timeDomResAssignSize);
      VRB2PRBMap       = reverseBits(VRB2PRBMap, VRB2PRBMapSize);
      modNCodScheme    = reverseBits(modNCodScheme, modNCodSchemeSize);
      redundancyVer    = reverseBits(redundancyVer, redundancyVerSize);
      sysInfoInd       = reverseBits(sysInfoInd, sysInfoIndSize);

     /* Calulating total number of bytes in buffer */
     dlDciPtr->payloadSizeBits = freqDomResAssignSize + timeDomResAssignSize\
              + VRB2PRBMapSize + modNCodSchemeSize + redundancyVerSize\
              + sysInfoIndSize + reservedSize;

     numBytes = dlDciPtr->payloadSizeBits / 8;
     if(dlDciPtr->payloadSizeBits % 8)
        numBytes += 1;

     if(numBytes > DCI_PAYLOAD_BYTE_LEN)
     {
        DU_LOG("\nLWR_MAC : Total bytes for DCI is more than expected");
        return;
     }

     /* Initialize buffer */
     for(bytePos = 0; bytePos < numBytes; bytePos++)
        dlDciPtr->payload[bytePos] = 0;

     bytePos = numBytes - 1;
     bitPos = 0;

     /* Packing DCI format fields */
     fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
        freqDomResAssign, freqDomResAssignSize);
     fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
        timeDomResAssign, timeDomResAssignSize);
     fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
        VRB2PRBMap, VRB2PRBMapSize);
     fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
        modNCodScheme, modNCodSchemeSize);
     fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
        redundancyVer, redundancyVerSize);
     fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
        sysInfoInd, sysInfoIndSize);
     fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
        reserved, reservedSize);

   }
} /* fillSib1DlDciPdu */

/*******************************************************************
 *
 * @brief fills Dl DCI PDU required for DL TTI info in MAC
 *
 * @details
 *
 *    Function : fillRarDlDciPdu
 *
 *    Functionality:
 *         -Fills the Dl DCI PDU
 *
 * @params[in] Pointer to fapi_dl_dci_t
 *             Pointer to PdcchCfg
 * @return ROK
 *
 ******************************************************************/

void fillRarDlDciPdu(fapi_dl_dci_t *dlDciPtr, PdcchCfg *rarPdcchInfo)
{
   if(dlDciPtr != NULLP)
   {
      uint8_t numBytes;
      uint8_t bytePos;
      uint8_t bitPos;

      uint16_t coreset0Size;
      uint16_t rbStart;
      uint16_t rbLen;
      uint32_t freqDomResAssign;
      uint8_t timeDomResAssign;
      uint8_t  VRB2PRBMap;
      uint8_t modNCodScheme;
      uint8_t tbScaling;
      uint32_t reserved;

      /* Size(in bits) of each field in DCI format 1_0 */
      uint8_t freqDomResAssignSize;
      uint8_t timeDomResAssignSize = 4;
      uint8_t VRB2PRBMapSize       = 1;
      uint8_t modNCodSchemeSize    = 5;
      uint8_t tbScalingSize        = 2;
      uint8_t reservedSize         = 16;

      dlDciPtr->rnti = rarPdcchInfo->dci.rnti;
      dlDciPtr->scramblingId = rarPdcchInfo->dci.scramblingId;    
      dlDciPtr->scramblingRnti = rarPdcchInfo->dci.scramblingRnti;
      dlDciPtr->cceIndex = rarPdcchInfo->dci.cceIndex;
      dlDciPtr->aggregationLevel = rarPdcchInfo->dci.aggregLevel;
      dlDciPtr->pc_and_bform.numPrgs = rarPdcchInfo->dci.beamPdcchInfo.numPrgs;
      dlDciPtr->pc_and_bform.prgSize = rarPdcchInfo->dci.beamPdcchInfo.prgSize;
      dlDciPtr->pc_and_bform.digBfInterfaces = rarPdcchInfo->dci.beamPdcchInfo.digBfInterfaces;
      dlDciPtr->pc_and_bform.pmi_bfi[0].pmIdx = rarPdcchInfo->dci.beamPdcchInfo.prg[0].pmIdx;
      dlDciPtr->pc_and_bform.pmi_bfi[0].beamIdx[0].beamidx = rarPdcchInfo->dci.beamPdcchInfo.prg[0].beamIdx[0];
      dlDciPtr->beta_pdcch_1_0 = rarPdcchInfo->dci.txPdcchPower.powerValue;           
      dlDciPtr->powerControlOfssetSS = rarPdcchInfo->dci.txPdcchPower.powerControlOffsetSS;

      /* Calculating freq domain resource allocation field value and size
       * coreset0Size = Size of coreset 0
       * RBStart = Starting Virtual Rsource block
       * RBLen = length of contiguously allocted RBs
       * Spec 38.214 Sec 5.1.2.2.2
       */

      /* TODO: Fill values of coreset0Size, rbStart and rbLen */
      coreset0Size= rarPdcchInfo->coreset0Cfg.coreSet0Size;
      rbStart = 0;              /* For SIB1 */
      //rbStart = rarPdcchInfo->dci.pdschCfg->freqAlloc.rbStart;
      rbLen = rarPdcchInfo->dci.pdschCfg->freqAlloc.rbSize;

      if((rbLen >=1) && (rbLen <= coreset0Size - rbStart))
      {
         if((rbLen - 1) <= floor(coreset0Size / 2))
            freqDomResAssign = (coreset0Size * (rbLen-1)) + rbStart;
         else
            freqDomResAssign = (coreset0Size * (coreset0Size - rbLen + 1)) \
                               + (coreset0Size - 1 - rbStart);

         freqDomResAssignSize = ceil(log2(coreset0Size * (coreset0Size + 1) / 2));
      }

      /* Fetching DCI field values */
      timeDomResAssign = rarPdcchInfo->dci.pdschCfg->timeAlloc.rowIndex -1;
      VRB2PRBMap       = rarPdcchInfo->dci.pdschCfg->freqAlloc.vrbPrbMapping;
      modNCodScheme    = rarPdcchInfo->dci.pdschCfg->codeword[0].mcsIndex;
      tbScaling        = 0; /* configured to 0 scaling */
      reserved         = 0;

      /* Reversing bits in each DCI field */
      freqDomResAssign = reverseBits(freqDomResAssign, freqDomResAssignSize);
      timeDomResAssign = reverseBits(timeDomResAssign, timeDomResAssignSize);
      VRB2PRBMap       = reverseBits(VRB2PRBMap, VRB2PRBMapSize);
      modNCodScheme    = reverseBits(modNCodScheme, modNCodSchemeSize);
      tbScaling        = reverseBits(tbScaling, tbScalingSize); 

      /* Calulating total number of bytes in buffer */
      dlDciPtr->payloadSizeBits = freqDomResAssignSize + timeDomResAssignSize\
                                  + VRB2PRBMapSize + modNCodSchemeSize + tbScalingSize + reservedSize;

      numBytes = dlDciPtr->payloadSizeBits / 8;
      if(dlDciPtr->payloadSizeBits % 8)
         numBytes += 1;

      if(numBytes > DCI_PAYLOAD_BYTE_LEN)
      {
         DU_LOG("\nLWR_MAC : Total bytes for DCI is more than expected");
         return;
      }

      /* Initialize buffer */
      for(bytePos = 0; bytePos < numBytes; bytePos++)
         dlDciPtr->payload[bytePos] = 0;

      bytePos = numBytes - 1;
      bitPos = 0;

      /* Packing DCI format fields */
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            freqDomResAssign, freqDomResAssignSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            timeDomResAssign, timeDomResAssignSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            VRB2PRBMap, VRB2PRBMapSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            modNCodScheme, modNCodSchemeSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            tbScaling, tbScalingSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            reserved, reservedSize);
   }
} /* fillRarDlDciPdu */

/*******************************************************************
 *
 * @brief fills msg4 Dl DCI PDU required for DL TTI info in MAC
 *
 * @details
 *
 *    Function : fillMsg4DlDciPdu
 *
 *    Functionality:
 *         -Fills the Msg4 Dl DCI PDU
 *
 * @params[in] Pointer to fapi_dl_dci_t
 *             Pointer to PdcchCfg
 * @return ROK
 *
 ******************************************************************/
void fillMsg4DlDciPdu(fapi_dl_dci_t *dlDciPtr, PdcchCfg *msg4PdcchInfo,\
Msg4Info *msg4Info)
{
   if(dlDciPtr != NULLP)
   {
      uint8_t numBytes;
      uint8_t bytePos;
      uint8_t bitPos;

      uint16_t coreset0Size = 0;
      uint16_t rbStart = 0;
      uint16_t rbLen = 0;
      uint8_t  dciFormatId;
      uint32_t freqDomResAssign;
      uint8_t  timeDomResAssign;
      uint8_t  VRB2PRBMap;
      uint8_t  modNCodScheme;
      uint8_t  ndi = 0;
      uint8_t  redundancyVer = 0;
      uint8_t  harqProcessNum = 0;
      uint8_t  dlAssignmentIdx = 0;
      uint8_t  pucchTpc = 0;
      uint8_t  pucchResoInd = 0;
      uint8_t  harqFeedbackInd = 0;

      /* Size(in bits) of each field in DCI format 1_0 */
      uint8_t dciFormatIdSize    = 1;
      uint8_t freqDomResAssignSize;
      uint8_t timeDomResAssignSize = 4;
      uint8_t VRB2PRBMapSize       = 1;
      uint8_t modNCodSchemeSize    = 5;
      uint8_t ndiSize              = 1;
      uint8_t redundancyVerSize    = 2;
      uint8_t harqProcessNumSize   = 4;
      uint8_t dlAssignmentIdxSize  = 2;
      uint8_t pucchTpcSize         = 2;
      uint8_t pucchResoIndSize     = 3;
      uint8_t harqFeedbackIndSize  = 3;

      dlDciPtr->rnti = msg4PdcchInfo->dci.rnti;
      dlDciPtr->scramblingId = msg4PdcchInfo->dci.scramblingId;    
      dlDciPtr->scramblingRnti = msg4PdcchInfo->dci.scramblingRnti;
      dlDciPtr->cceIndex = msg4PdcchInfo->dci.cceIndex;
      dlDciPtr->aggregationLevel = msg4PdcchInfo->dci.aggregLevel;
      dlDciPtr->pc_and_bform.numPrgs = msg4PdcchInfo->dci.beamPdcchInfo.numPrgs;
      dlDciPtr->pc_and_bform.prgSize = msg4PdcchInfo->dci.beamPdcchInfo.prgSize;
      dlDciPtr->pc_and_bform.digBfInterfaces = msg4PdcchInfo->dci.beamPdcchInfo.digBfInterfaces;
      dlDciPtr->pc_and_bform.pmi_bfi[0].pmIdx = msg4PdcchInfo->dci.beamPdcchInfo.prg[0].pmIdx;
      dlDciPtr->pc_and_bform.pmi_bfi[0].beamIdx[0].beamidx = msg4PdcchInfo->dci.beamPdcchInfo.prg[0].beamIdx[0];
      dlDciPtr->beta_pdcch_1_0 = msg4PdcchInfo->dci.txPdcchPower.powerValue;           
      dlDciPtr->powerControlOfssetSS = msg4PdcchInfo->dci.txPdcchPower.powerControlOffsetSS;

      /* Calculating freq domain resource allocation field value and size
       * coreset0Size = Size of coreset 0
       * RBStart = Starting Virtual Rsource block
       * RBLen = length of contiguously allocted RBs
       * Spec 38.214 Sec 5.1.2.2.2
       */

      /* TODO: Fill values of coreset0Size, rbStart and rbLen */
      coreset0Size = msg4PdcchInfo->coreset0Cfg.coreSet0Size;
      //rbStart = msg4PdcchInfo->dci.pdschCfg->freqAlloc.rbStart;
      rbLen = msg4PdcchInfo->dci.pdschCfg->freqAlloc.rbSize;

      if((rbLen >=1) && (rbLen <= coreset0Size - rbStart))
      {
         if((rbLen - 1) <= floor(coreset0Size / 2))
            freqDomResAssign = (coreset0Size * (rbLen-1)) + rbStart;
         else
            freqDomResAssign = (coreset0Size * (coreset0Size - rbLen + 1)) \
                               + (coreset0Size - 1 - rbStart);

         freqDomResAssignSize = ceil(log2(coreset0Size * (coreset0Size + 1) / 2));
      }

      /* Fetching DCI field values */
      dciFormatId      = msg4Info->dciFormatId; /* DCI indentifier for DL */
      timeDomResAssign = msg4PdcchInfo->dci.pdschCfg->timeAlloc.rowIndex -1;
      VRB2PRBMap       = msg4PdcchInfo->dci.pdschCfg->freqAlloc.vrbPrbMapping;
      modNCodScheme    = msg4PdcchInfo->dci.pdschCfg->codeword[0].mcsIndex;
      ndi              = msg4Info->ndi;      
      redundancyVer    = msg4PdcchInfo->dci.pdschCfg->codeword[0].rvIndex;
      harqProcessNum   = msg4Info->harqProcNum; 
      dlAssignmentIdx  = msg4Info->dlAssignIdx;
      pucchTpc         = msg4Info->pucchTpc;
      pucchResoInd     = msg4Info->pucchResInd;
      harqFeedbackInd  = msg4Info->harqFeedbackInd;

      /* Reversing bits in each DCI field */
      dciFormatId      = reverseBits(dciFormatId, dciFormatIdSize);
      freqDomResAssign = reverseBits(freqDomResAssign, freqDomResAssignSize);
      timeDomResAssign = reverseBits(timeDomResAssign, timeDomResAssignSize);
      VRB2PRBMap       = reverseBits(VRB2PRBMap, VRB2PRBMapSize);
      modNCodScheme    = reverseBits(modNCodScheme, modNCodSchemeSize);
      ndi              = reverseBits(ndi, ndiSize);
      redundancyVer    = reverseBits(redundancyVer, redundancyVerSize);
      harqProcessNum   = reverseBits(harqProcessNum, harqProcessNumSize);
      dlAssignmentIdx  = reverseBits(dlAssignmentIdx , dlAssignmentIdxSize);
      pucchTpc         = reverseBits(pucchTpc, pucchTpcSize);
      pucchResoInd     = reverseBits(pucchResoInd, pucchResoIndSize);
      harqFeedbackInd  = reverseBits(harqFeedbackInd, harqFeedbackIndSize);


      /* Calulating total number of bytes in buffer */
      dlDciPtr->payloadSizeBits = (dciFormatIdSize + freqDomResAssignSize\
      + timeDomResAssignSize + VRB2PRBMapSize + modNCodSchemeSize\
      + ndiSize + redundancyVerSize + harqProcessNumSize + dlAssignmentIdxSize\
      + pucchTpcSize + pucchResoIndSize + harqFeedbackIndSize);

      numBytes = dlDciPtr->payloadSizeBits / 8;
      if(dlDciPtr->payloadSizeBits % 8)
         numBytes += 1;

      if(numBytes > DCI_PAYLOAD_BYTE_LEN)
      {
         DU_LOG("\nLWR_MAC : Total bytes for DCI is more than expected");
         return;
      }

      /* Initialize buffer */
      for(bytePos = 0; bytePos < numBytes; bytePos++)
         dlDciPtr->payload[bytePos] = 0;

      bytePos = numBytes - 1;
      bitPos = 0;

      /* Packing DCI format fields */
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            dciFormatId, dciFormatIdSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            freqDomResAssign, freqDomResAssignSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            timeDomResAssign, timeDomResAssignSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            VRB2PRBMap, VRB2PRBMapSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            modNCodScheme, modNCodSchemeSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            ndi, ndiSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            redundancyVer, redundancyVerSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            redundancyVer, redundancyVerSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            harqProcessNum, harqProcessNumSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            dlAssignmentIdx, dlAssignmentIdxSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            pucchTpc, pucchTpcSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            pucchResoInd, pucchResoIndSize);
      fillDlDciPayload(dlDciPtr->payload, &bytePos, &bitPos,\
            harqFeedbackInd, harqFeedbackIndSize);
   }
} /* fillMsg4DlDciPdu */

/*******************************************************************
 *
 * @brief fills PDCCH PDU required for DL TTI info in MAC
 *
 * @details
 *
 *    Function : fillPdcchPdu
 *
 *    Functionality:
 *         -Fills the Pdcch PDU info
 *          stored in MAC
 *
 * @params[in] Pointer to FAPI DL TTI Req
 *             Pointer to PdcchCfg
 *             Pointer to msgLen of DL TTI Info
 * @return ROK
 *
 ******************************************************************/
S16 fillPdcchPdu(fapi_dl_tti_req_pdu_t *dlTtiReqPdu, DlSchedInfo *dlInfo, uint32_t *msgLen, RntiType rntiType)
{
   if(dlTtiReqPdu != NULLP)
   {
      PdcchCfg *pdcchInfo = NULLP;
      BwpCfg *bwp = NULLP;

      if(rntiType == SI_RNTI_TYPE)
      {
         pdcchInfo = &dlInfo->brdcstAlloc.sib1Alloc.sib1PdcchCfg;
         bwp = &dlInfo->brdcstAlloc.sib1Alloc.bwp;
         fillSib1DlDciPdu(dlTtiReqPdu->u.pdcch_pdu.dlDci, pdcchInfo);
      }
      else if(rntiType == RA_RNTI_TYPE)
      {
         pdcchInfo = &dlInfo->rarAlloc->rarPdcchCfg;
         bwp = &dlInfo->rarAlloc->bwp;
         fillRarDlDciPdu(dlTtiReqPdu->u.pdcch_pdu.dlDci, pdcchInfo);
      }
      else if(rntiType == TC_RNTI_TYPE)
      {
         pdcchInfo = &dlInfo->msg4Alloc->msg4PdcchCfg;
         bwp = &dlInfo->msg4Alloc->bwp;
         fillMsg4DlDciPdu(dlTtiReqPdu->u.pdcch_pdu.dlDci, pdcchInfo,\
            &dlInfo->msg4Alloc->msg4Info);
      }
      else
      {
         DU_LOG("\nLWR_MAC: Failed filling PDCCH Pdu");
         return RFAILED;;
      }
      dlTtiReqPdu->pduType = PDCCH_PDU_TYPE;
      dlTtiReqPdu->u.pdcch_pdu.bwpSize = bwp->BWPSize;
      dlTtiReqPdu->u.pdcch_pdu.bwpPart = bwp->BWPStart;
      dlTtiReqPdu->u.pdcch_pdu.subCarrierSpacing = bwp->subcarrierSpacing; 
      dlTtiReqPdu->u.pdcch_pdu.cyclicPrefix = bwp->cyclicPrefix; 
      dlTtiReqPdu->u.pdcch_pdu.startSymbolIndex = pdcchInfo->coreset0Cfg.startSymbolIndex;
      dlTtiReqPdu->u.pdcch_pdu.durationSymbols = pdcchInfo->coreset0Cfg.durationSymbols;
      memcpy(dlTtiReqPdu->u.pdcch_pdu.freqDomainResource, pdcchInfo->coreset0Cfg.freqDomainResource, 6);
      dlTtiReqPdu->u.pdcch_pdu.cceRegMappingType = pdcchInfo->coreset0Cfg.cceRegMappingType;
      dlTtiReqPdu->u.pdcch_pdu.regBundleSize = pdcchInfo->coreset0Cfg.regBundleSize;
      dlTtiReqPdu->u.pdcch_pdu.interleaverSize = pdcchInfo->coreset0Cfg.interleaverSize;
      dlTtiReqPdu->u.pdcch_pdu.coreSetSize = pdcchInfo->coreset0Cfg.coreSetType;
      dlTtiReqPdu->u.pdcch_pdu.shiftIndex =  pdcchInfo->coreset0Cfg.shiftIndex;
      dlTtiReqPdu->u.pdcch_pdu.precoderGranularity = pdcchInfo->coreset0Cfg.precoderGranularity;
      dlTtiReqPdu->u.pdcch_pdu.numDlDci = pdcchInfo->numDlDci;
      dlTtiReqPdu->u.pdcch_pdu.dlDci = (fapi_dl_dci_t *)(dlTtiReqPdu + \
         (sizeof(fapi_dl_tti_req_pdu_t) - sizeof(dlTtiReqPdu->u)) + \
         (sizeof(fapi_dl_pdcch_pdu_t) - sizeof(fapi_dl_dci_t*)));

      /* Calculating PDU length. Considering only one dl dci pdu for now */
      dlTtiReqPdu->pduSize = sizeof(fapi_dl_pdcch_pdu_t) + sizeof(fapi_dl_dci_t);
      SET_MSG_LEN(*msgLen, (sizeof(dlTtiReqPdu->pduType) + \
         sizeof(dlTtiReqPdu->pduSize) + dlTtiReqPdu->pduSize));

    }

    return ROK;
}

/*******************************************************************
 *
 * @brief fills PDSCH PDU required for DL TTI info in MAC
 *
 * @details
 *
 *    Function : fillPdschPdu
 *
 *    Functionality:
 *         -Fills the Pdsch PDU info
 *          stored in MAC
 *
 * @params[in] Pointer to FAPI DL TTI Req
 *             Pointer to PdschCfg
 *             Pointer to msgLen of DL TTI Info
 * @return ROK
 *
 ******************************************************************/

void fillPdschPdu(fapi_dl_tti_req_pdu_t *dlTtiReqPdu, PdschCfg *pdschInfo,
   BwpCfg bwp,uint32_t *msgLen, uint16_t pduIndex)
{
    uint8_t idx;

    if(dlTtiReqPdu != NULLP)
    {
       dlTtiReqPdu->pduType = PDSCH_PDU_TYPE;
       dlTtiReqPdu->u.pdsch_pdu.pduBitMap = pdschInfo->pduBitmap;
       dlTtiReqPdu->u.pdsch_pdu.rnti = pdschInfo->rnti;         
       dlTtiReqPdu->u.pdsch_pdu.pduIndex = pduIndex;
       dlTtiReqPdu->u.pdsch_pdu.bwpSize = bwp.BWPSize;       
       dlTtiReqPdu->u.pdsch_pdu.bwpStart = bwp.BWPStart;
       dlTtiReqPdu->u.pdsch_pdu.subCarrierSpacing = bwp.subcarrierSpacing;
       dlTtiReqPdu->u.pdsch_pdu.cyclicPrefix = bwp.cyclicPrefix;
       dlTtiReqPdu->u.pdsch_pdu.nrOfCodeWords = pdschInfo->numCodewords;
       for(idx = 0; idx < MAX_CODEWORDS ; idx++)
       { 
          dlTtiReqPdu->u.pdsch_pdu.cwInfo[idx].targetCodeRate = pdschInfo->codeword[idx].targetCodeRate;
          dlTtiReqPdu->u.pdsch_pdu.cwInfo[idx].qamModOrder = pdschInfo->codeword[idx].qamModOrder;
          dlTtiReqPdu->u.pdsch_pdu.cwInfo[idx].mcsIndex = pdschInfo->codeword[idx].mcsIndex;
          dlTtiReqPdu->u.pdsch_pdu.cwInfo[idx].mcsTable = pdschInfo->codeword[idx].mcsTable;
          dlTtiReqPdu->u.pdsch_pdu.cwInfo[idx].rvIndex = pdschInfo->codeword[idx].rvIndex;
          dlTtiReqPdu->u.pdsch_pdu.cwInfo[idx].tbSize = pdschInfo->codeword[idx].tbSize;
       }
       dlTtiReqPdu->u.pdsch_pdu.dataScramblingId = pdschInfo->dataScramblingId;       
       dlTtiReqPdu->u.pdsch_pdu.nrOfLayers = pdschInfo->numLayers;
       dlTtiReqPdu->u.pdsch_pdu.transmissionScheme = pdschInfo->transmissionScheme;
       dlTtiReqPdu->u.pdsch_pdu.refPoint = pdschInfo->refPoint;
       dlTtiReqPdu->u.pdsch_pdu.dlDmrsSymbPos = pdschInfo->dmrs.dlDmrsSymbPos;
       dlTtiReqPdu->u.pdsch_pdu.dmrsConfigType = pdschInfo->dmrs.dmrsConfigType;
       dlTtiReqPdu->u.pdsch_pdu.dlDmrsScramblingId = pdschInfo->dmrs.dlDmrsScramblingId;
       dlTtiReqPdu->u.pdsch_pdu.scid = pdschInfo->dmrs.scid;
       dlTtiReqPdu->u.pdsch_pdu.numDmrsCdmGrpsNoData = pdschInfo->dmrs.numDmrsCdmGrpsNoData;
       dlTtiReqPdu->u.pdsch_pdu.dmrsPorts = pdschInfo->dmrs.dmrsPorts;
       dlTtiReqPdu->u.pdsch_pdu.resourceAlloc = pdschInfo->freqAlloc.resourceAlloc;
       /* since we are using type-1, hence rbBitmap excluded */
       dlTtiReqPdu->u.pdsch_pdu.rbStart = pdschInfo->freqAlloc.rbStart;
       dlTtiReqPdu->u.pdsch_pdu.rbSize = pdschInfo->freqAlloc.rbSize;
       dlTtiReqPdu->u.pdsch_pdu.vrbToPrbMapping = pdschInfo->freqAlloc.vrbPrbMapping;
       dlTtiReqPdu->u.pdsch_pdu.startSymbIndex = pdschInfo->timeAlloc.startSymbolIndex;
       dlTtiReqPdu->u.pdsch_pdu.nrOfSymbols = pdschInfo->timeAlloc.numSymbols;
       dlTtiReqPdu->u.pdsch_pdu.preCodingAndBeamforming.numPrgs = pdschInfo->beamPdschInfo.numPrgs;
       dlTtiReqPdu->u.pdsch_pdu.preCodingAndBeamforming.prgSize = pdschInfo->beamPdschInfo.prgSize;
       dlTtiReqPdu->u.pdsch_pdu.preCodingAndBeamforming.digBfInterfaces = pdschInfo->beamPdschInfo.digBfInterfaces;
       dlTtiReqPdu->u.pdsch_pdu.preCodingAndBeamforming.pmi_bfi[0]. \
          pmIdx = pdschInfo->beamPdschInfo.prg[0].pmIdx;
       dlTtiReqPdu->u.pdsch_pdu.preCodingAndBeamforming.pmi_bfi[0]. \
          beamIdx[0].beamidx = pdschInfo->beamPdschInfo.prg[0].beamIdx[0];
       dlTtiReqPdu->u.pdsch_pdu.powerControlOffset = pdschInfo->txPdschPower.powerControlOffset;  
       dlTtiReqPdu->u.pdsch_pdu.powerControlOffsetSS = pdschInfo->txPdschPower.powerControlOffsetSS;
       dlTtiReqPdu->pduSize = sizeof(fapi_dl_pdsch_pdu_t);

       SET_MSG_LEN(*msgLen, (sizeof(dlTtiReqPdu->pduType) + \
          sizeof(dlTtiReqPdu->pduSize) + sizeof(fapi_dl_pdsch_pdu_t)));

    }

}

/***********************************************************************
 *
 * @brief calculates the total size to be allocated for DL TTI Req
 *
 * @details
 *
 *    Function : calcDlTtiReqPduCount
 *
 *    Functionality:
 *         -calculates the total pdu count to be allocated for DL TTI Req
 *
 * @params[in]    DlBrdcstAlloc *cellBroadcastInfo
 * @return count
 *
 * ********************************************************************/
uint8_t calcDlTtiReqPduCount(DlSchedInfo *dlInfo)
{
   uint8_t count = 0;
   uint8_t idx = 0;

   if(dlInfo->isBroadcastPres)
   {
      if(dlInfo->brdcstAlloc.ssbTrans)
      {
         for(idx = 0; idx < dlInfo->brdcstAlloc.ssbIdxSupported; idx++)
         {
            count++;
         }
      }
      if(dlInfo->brdcstAlloc.sib1Trans)
      {
         count += 2;
      }
   }
   if(dlInfo->rarAlloc != NULLP)
   {
      count += 2;
   }
   if(dlInfo->msg4Alloc != NULLP)
   {
      count += 2;
   }

   return count;
}

/***********************************************************************
 *
 * @brief calculates the total size to be allocated for DL TTI Req
 *
 * @details
 *
 *    Function : calcTxDataReqPduCount
 *
 *    Functionality:
 *         -calculates the total pdu count to be allocated for DL TTI Req
 *
 * @params[in]    DlBrdcstAlloc *cellBroadcastInfo
 * @return count
 *
 * ********************************************************************/
uint8_t calcTxDataReqPduCount(DlSchedInfo *dlInfo)
{
   uint8_t count = 0;

   if(dlInfo->isBroadcastPres && dlInfo->brdcstAlloc.sib1Trans)
   {
      count++;
   }
   if(dlInfo->rarAlloc != NULLP)
   {
      count++;
   }
   if(dlInfo->msg4Alloc != NULLP)
   {
      count++;
   }

   return count;
}
/***********************************************************************
 *
 * @brief fills the SIB1 TX-DATA request message
 *
 * @details
 *
 *    Function : fillSib1TxDataReq
 *
 *    Functionality:
 *         - fills the SIB1 TX-DATA request message
 *
 * @params[in]    fapi_tx_pdu_desc_t *pduDesc
 * @params[in]    macCellCfg consist of SIB1 pdu
 * @params[in]    uint32_t *msgLen
 * @params[in]    uint16_t pduIndex
 * @return ROK
 *
 * ********************************************************************/
uint8_t fillSib1TxDataReq(fapi_tx_pdu_desc_t *pduDesc,MacCellCfg *macCellCfg,
   uint32_t *msgLen, uint16_t pduIndex)
{
   uint32_t pduLen = 0;
   uint32_t *sib1TxdataValue = NULLP;

   pduDesc[pduIndex].pduIndex = pduIndex;
   pduDesc[pduIndex].numTlvs = 1;

   /* fill the TLV */
   /* as of now, memory is allocated from SSI, later WLS memory needs to be taken */
   pduDesc[pduIndex].tlvs[0].tl.tag = 1; /* pointer to be sent */
   pduDesc[pduIndex].tlvs[0].tl.length = macCellCfg->sib1Cfg.sib1PduLen;
   LWR_MAC_ALLOC(sib1TxdataValue,macCellCfg->sib1Cfg.sib1PduLen);
   if(sib1TxdataValue == NULLP)
   {
      return RFAILED;
   }
   memcpy(sib1TxdataValue,macCellCfg->sib1Cfg.sib1Pdu,
      macCellCfg->sib1Cfg.sib1PduLen);
   pduDesc[pduIndex].tlvs[0].value = sib1TxdataValue;

   /* The total length of the PDU description and   PDU data */
   pduLen += 8; /* size of PDU length 2 bytes, PDU index 2 bytes, numTLV 4 bytes */
   pduLen += sizeof(fapi_uint32_tlv_t); /* only 1 TLV is present */
   pduDesc[pduIndex].pduLength = pduLen; 
   msgLen += pduLen;

#ifndef INTEL_WLS   
   MAC_FREE(sib1TxdataValue,macCellCfg->sib1Cfg.sib1PduLen);
#endif

   return ROK;
}

/***********************************************************************
 *
 * @brief fills the RAR TX-DATA request message
 *
 * @details
 *
 *    Function : fillRarTxDataReq
 *
 *    Functionality:
 *         - fills the RAR TX-DATA request message
 *
 * @params[in]    fapi_tx_pdu_desc_t *pduDesc
 * @params[in]    RarInfo *rarInfo
 * @params[in]    uint32_t *msgLen
 * @params[in]    uint16_t pduIndex
 * @return ROK
 *
 * ********************************************************************/
uint8_t fillRarTxDataReq(fapi_tx_pdu_desc_t *pduDesc, RarInfo *rarInfo,
   uint32_t *msgLen, uint16_t pduIndex)
{
   uint32_t pduLen = 0;
   uint32_t *rarTxdataValue = NULLP;

   pduDesc[pduIndex].pduIndex = pduIndex;
   pduDesc[pduIndex].numTlvs = 1;

   /* fill the TLV */
   /* as of now, memory is allocated from SSI, later WLS memory needs to be taken */
   pduDesc[pduIndex].tlvs[0].tl.tag = 1; /* pointer to be sent */
   pduDesc[pduIndex].tlvs[0].tl.length = rarInfo->rarPduLen;
   LWR_MAC_ALLOC(rarTxdataValue,rarInfo->rarPduLen);
   if(rarTxdataValue == NULLP)
   {
      return RFAILED;
   }
   memcpy(rarTxdataValue,rarInfo->rarPdu,rarInfo->rarPduLen);
   pduDesc[pduIndex].tlvs[0].value = (uint32_t)rarTxdataValue;

   /* The total length of the PDU description and   PDU data */
   pduLen += 8; /* size of PDU length 2 bytes, PDU index 2 bytes, numTLV 4 bytes */
   pduLen += sizeof(fapi_uint32_tlv_t); /* only 1 TLV is present */
   pduDesc[pduIndex].pduLength = pduLen; 
   msgLen += pduLen;

/* TODO: The pointer value which was stored, needs to be free-ed at PHY *
 * But since we did not implement WLS, this has to be done here
 */
#ifndef INTEL_WLS   
   MAC_FREE(rarTxdataValue,rarInfo->rarPduLen);
#endif

   return ROK;
}

/***********************************************************************
 *
 * @brief fills the Msg4 TX-DATA request message
 *
 * @details
 *
 *    Function : fillMsg4TxDataReq
 *
 *    Functionality:
 *         - fills the Msg4 TX-DATA request message
 *
 * @params[in]    fapi_tx_pdu_desc_t *pduDesc
 * @params[in]    Msg4Info *msg4Info
 * @params[in]    uint32_t *msgLen
 * @params[in]    uint16_t pduIndex
 * @return ROK
 *
 * ********************************************************************/
uint8_t fillMsg4TxDataReq(fapi_tx_pdu_desc_t *pduDesc, Msg4Info *msg4Info,
   uint32_t *msgLen, uint16_t pduIndex)
{
   uint32_t pduLen = 0;
   uint32_t *msg4TxDataValue = NULLP;

   pduDesc[pduIndex].pduIndex = pduIndex;
   pduDesc[pduIndex].numTlvs = 1;
   
   /* fill the TLV */
   /* as of now, memory is allocated from SSI, later WLS memory needs to be taken */
   pduDesc[pduIndex].tlvs[0].tl.tag = 1; /* pointer to be sent */
   pduDesc[pduIndex].tlvs[0].tl.length = msg4Info->msg4PduLen;
   LWR_MAC_ALLOC(msg4TxDataValue, msg4Info->msg4PduLen);
   if(msg4TxDataValue == NULLP)
   {
      return RFAILED;
   }
   memcpy(msg4TxDataValue, msg4Info->msg4Pdu, msg4Info->msg4PduLen);
   pduDesc[pduIndex].tlvs[0].value = (uint32_t)msg4TxDataValue;

   /* The total length of the PDU description and PDU data */
   pduLen += 8; /* size of PDU length 2 bytes, PDU index 2 bytes, numTLV 4 bytes */
   pduLen += sizeof(fapi_uint32_tlv_t); /* only 1 TLV is present */
   pduDesc[pduIndex].pduLength = pduLen; 
   msgLen += pduLen;

   /* TODO: The pointer value which was stored, needs to be free-ed at PHY *
    * But since we did not implement WLS, this has to be done here
    */
   #ifndef INTEL_WLS   
      MAC_FREE(msg4TxDataValue, msg4Info->msg4PduLen);
   #endif

   return ROK;
}

#endif /* FAPI */
/*******************************************************************
 *
 * @brief Sends DL TTI Request to PHY
 *
 * @details
 *
 *    Function : handleDlTtiReq
 *
 *    Functionality:
 *         -Sends FAPI DL TTI req to PHY
 *
 * @params[in]    timing info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t handleDlTtiReq(SlotIndInfo currTimingInfo)
{
#ifdef FAPI
   uint8_t idx;
   uint8_t nPdu = 0;
   uint8_t numPduEncoded = 0;
   uint16_t pduIndex = 0;
   uint32_t msgLen = 0;
   uint32_t dlTtiReqMsgSize = 0;

   fapi_dl_tti_req_t *dlTtiReq = NULLP;
   SlotIndInfo dlTtiReqTimingInfo;

   RgCellCb  *cellCbParams = NULLP;
   MacDlSlot *currDlSlot = NULLP;
   MacCellCfg macCellCfg;
   memset(&macCellCfg, 0, sizeof(MacCellCfg));
   Inst inst = 0;
   RntiType rntiType;
   
   if(clGlobalCp.phyState == PHY_STATE_RUNNING)
   {
      /* consider phy delay */
      ADD_DELTA_TO_TIME(currTimingInfo,dlTtiReqTimingInfo,PHY_DELTA);

      cellCbParams = rgCb[inst].cell;
		macCellCfg = cellCbParams->macCellCfg;

		currDlSlot = &macCb.macCell->dlSlot[dlTtiReqTimingInfo.slot]; 
		nPdu = calcDlTtiReqPduCount(&currDlSlot->dlInfo);
		dlTtiReqMsgSize = sizeof(fapi_dl_tti_req_t) + (nPdu * \
				sizeof(fapi_dl_tti_req_pdu_t));
		if(nPdu > 0)
		{
			if(currDlSlot->dlInfo.isBroadcastPres)
			{
				if(currDlSlot->dlInfo.brdcstAlloc.sib1Trans)
				{
					dlTtiReqMsgSize += sizeof(fapi_dl_dci_t);
				}
			}
			if(currDlSlot->dlInfo.rarAlloc != NULLP)
			{
				dlTtiReqMsgSize += sizeof(fapi_dl_dci_t);
			}
			if(currDlSlot->dlInfo.msg4Alloc != NULLP)
			{
				dlTtiReqMsgSize += sizeof(fapi_dl_dci_t);
			}
		}
		LWR_MAC_ALLOC(dlTtiReq, dlTtiReqMsgSize);
		if(dlTtiReq != NULLP)
		{
			memset(dlTtiReq, 0, dlTtiReqMsgSize);
			dlTtiReq->sfn  = currTimingInfo.sfn;
			dlTtiReq->slot = currTimingInfo.slot;
			dlTtiReq->nPdus = calcDlTtiReqPduCount(&currDlSlot->dlInfo);  /* get total Pdus */
			nPdu = dlTtiReq->nPdus;
			dlTtiReq->nGroup = 0;

			if(dlTtiReq->nPdus > 0)
			{
				dlTtiReq->pdus = (fapi_dl_tti_req_pdu_t*)(dlTtiReq + \
						(sizeof(fapi_dl_tti_req_t) - sizeof(fapi_dl_tti_req_pdu_t*)));
				if(!dlTtiReq->pdus)
				{
					DU_LOG("\nLWR_MAC: Memory allocation failed");
					return RFAILED;
				}

				if(currDlSlot->dlInfo.isBroadcastPres)
				{
					if(currDlSlot->dlInfo.brdcstAlloc.ssbTrans)
					{
						if(dlTtiReq->pdus != NULLP)
						{
							for(idx = 0; idx < currDlSlot->dlInfo.brdcstAlloc.ssbIdxSupported; idx++)
							{
								fillSsbPdu(&dlTtiReq->pdus[numPduEncoded], &macCellCfg,\
										currDlSlot, &msgLen, idx, dlTtiReq->sfn);
								numPduEncoded++;
							}
						}
						printf("\033[1;31m");
						DU_LOG("\nLWR_MAC: MIB sent..");
						printf("\033[0m");
					}
					if(currDlSlot->dlInfo.brdcstAlloc.sib1Trans)
					{
						/* Filling SIB1 param */
						if(numPduEncoded != nPdu)
						{
							rntiType = SI_RNTI_TYPE;
							fillPdcchPdu(&dlTtiReq->pdus[numPduEncoded],&currDlSlot->dlInfo,\
									&msgLen, rntiType);
							numPduEncoded++;
							fillPdschPdu(&dlTtiReq->pdus[numPduEncoded],
									&currDlSlot->dlInfo.brdcstAlloc.sib1Alloc.sib1PdschCfg,
									currDlSlot->dlInfo.brdcstAlloc.sib1Alloc.bwp,
									&msgLen, pduIndex);
							pduIndex++;
							numPduEncoded++;
						}
						printf("\033[1;34m");
						DU_LOG("\nLWR_MAC: SIB1 sent...");
						printf("\033[0m");
					}
				}
				if(currDlSlot->dlInfo.rarAlloc != NULLP)
				{
					/* Filling RAR param */
					rntiType = RA_RNTI_TYPE;
					fillPdcchPdu(&dlTtiReq->pdus[numPduEncoded], \
							&currDlSlot->dlInfo, &msgLen, rntiType);
					numPduEncoded++;
					fillPdschPdu(&dlTtiReq->pdus[numPduEncoded],
							&currDlSlot->dlInfo.rarAlloc->rarPdschCfg,
							currDlSlot->dlInfo.rarAlloc->bwp,
							&msgLen, pduIndex);
					numPduEncoded++;
					pduIndex++;

					printf("\033[1;32m");
					DU_LOG("\nLWR_MAC: RAR sent...");
					printf("\033[0m");
				}
				if(currDlSlot->dlInfo.msg4Alloc != NULLP)
				{
					/* Filling Msg4 param */
					rntiType = TC_RNTI_TYPE;
					fillPdcchPdu(&dlTtiReq->pdus[numPduEncoded], \
							&currDlSlot->dlInfo, &msgLen, rntiType);
					numPduEncoded++;
					fillPdschPdu(&dlTtiReq->pdus[numPduEncoded],
							&currDlSlot->dlInfo.msg4Alloc->msg4PdschCfg,
							currDlSlot->dlInfo.msg4Alloc->bwp,
							&msgLen, pduIndex);
					numPduEncoded++;
					pduIndex++;

					printf("\033[1;32m");
					DU_LOG("\nLWR_MAC: MSG4 sent...");
					printf("\033[0m");
				}
				msgLen += sizeof(fapi_dl_tti_req_t) - sizeof(fapi_msg_t);
				fillMsgHeader(&dlTtiReq->header, FAPI_DL_TTI_REQUEST, msgLen);
				LwrMacSendToPhy(dlTtiReq->header.message_type_id, dlTtiReqMsgSize, \
						(void *)dlTtiReq);

				/* send Tx-DATA req message */
				sendTxDataReq(currTimingInfo, &currDlSlot->dlInfo);
			}
			else
			{
				msgLen = sizeof(fapi_dl_tti_req_t) - sizeof(fapi_msg_t);
				fillMsgHeader(&dlTtiReq->header, FAPI_DL_TTI_REQUEST, msgLen);
				LwrMacSendToPhy(dlTtiReq->header.message_type_id, dlTtiReqMsgSize, (void *)dlTtiReq);
			}
			return ROK;
		}
		else
		{
			DU_LOG("\nLWR_MAC: Failed to allocate memory for DL TTI Request");
			return RFAILED;
		}
	}
   else
   {
       lwr_mac_handleInvalidEvt(&currTimingInfo);
       return RFAILED;
   }
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Sends TX data Request to PHY
 *
 * @details
 *
 *    Function : sendTxDataReq
 *
 *    Functionality:
 *         -Sends FAPI TX data req to PHY
 *
 * @params[in]    timing info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t sendTxDataReq(SlotIndInfo currTimingInfo, DlSchedInfo *dlInfo)
{
#ifdef FAPI
   uint8_t nPdu = 0;
   uint32_t msgLen = 0;
   uint16_t pduIndex = 0;
   uint32_t txDataReqMsgSize = 0;
   fapi_tx_data_req_t *txDataReq = NULLP;
   Inst inst = 0;

   /* send TX_Data request message */
   nPdu = calcTxDataReqPduCount(dlInfo);
   if(nPdu > 0)
   {
      txDataReqMsgSize = sizeof(fapi_tx_data_req_t) + \
         (nPdu * sizeof(fapi_tx_pdu_desc_t));
      if(dlInfo->brdcstAlloc.sib1Trans)
      {
         txDataReqMsgSize += rgCb[inst].cell->macCellCfg.sib1Cfg.sib1PduLen;
      }
      if(dlInfo->rarAlloc != NULLP)
      {
         txDataReqMsgSize += dlInfo->rarAlloc->rarInfo.rarPduLen;
      }
      if(dlInfo->msg4Alloc != NULLP)
      {
         txDataReqMsgSize += dlInfo->msg4Alloc->msg4Info.msg4PduLen;
      }

      LWR_MAC_ALLOC(txDataReq, txDataReqMsgSize);
      if(txDataReq == NULLP)
      {
         DU_LOG("\nLWR_MAC: Failed to allocate memory for TX data Request");
         return RFAILED;
      }

      memset(txDataReq, 0, txDataReqMsgSize);
		txDataReq->sfn  = currTimingInfo.sfn;
		txDataReq->slot = currTimingInfo.slot;
      txDataReq->pduDesc = (fapi_tx_pdu_desc_t *)(txDataReq + \
         (sizeof(fapi_tx_data_req_t) - sizeof(fapi_tx_pdu_desc_t *)));

      if(dlInfo->brdcstAlloc.sib1Trans)
      {
         fillSib1TxDataReq(txDataReq->pduDesc,
               &rgCb[inst].cell->macCellCfg, &msgLen, pduIndex);
         pduIndex++;
         txDataReq->numPdus++;
      }
      if(dlInfo->rarAlloc != NULLP)
      {
         fillRarTxDataReq(txDataReq->pduDesc, &dlInfo->rarAlloc->rarInfo, &msgLen, pduIndex);
         pduIndex++;
         txDataReq->numPdus++;

         MAC_FREE(dlInfo->rarAlloc,sizeof(RarAlloc));
         dlInfo->rarAlloc = NULLP;
      }
      if(dlInfo->msg4Alloc != NULLP)
      {
         fillMsg4TxDataReq(txDataReq->pduDesc, &dlInfo->msg4Alloc->\
             msg4Info, &msgLen, pduIndex);
         pduIndex++;
         txDataReq->numPdus++;

         MAC_FREE(dlInfo->msg4Alloc,sizeof(Msg4Alloc));
         dlInfo->msg4Alloc = NULLP;
      }
      msgLen += sizeof(fapi_tx_data_req_t) - sizeof(fapi_msg_t);
      fillMsgHeader(&txDataReq->header, FAPI_TX_DATA_REQUEST, msgLen);
      LwrMacSendToPhy(txDataReq->header.message_type_id, txDataReqMsgSize, \
         (void *)txDataReq);
   }
#endif
   return ROK;
}

/***********************************************************************
 *
 * @brief calculates the total size to be allocated for UL TTI Req
 *
 * @details
 *
 *    Function : getnPdus
 *
 *    Functionality:
 *         -calculates the total pdu count to be allocated for UL TTI Req
 *
 * @params[in] Pointer to fapi Ul TTI Req
 *             Pointer to CurrUlSlot
 * @return count
 * ********************************************************************/
#ifdef FAPI
uint8_t getnPdus(fapi_ul_tti_req_t *ulTtiReq, MacUlSlot *currUlSlot)
{
   uint8_t pduCount = 0;

   if(currUlSlot != NULLP)
   {
      if(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_PRACH)
      {
         pduCount++;
         if(ulTtiReq)
            ulTtiReq->rachPresent = PDU_PRESENT;
      }
		if(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_PUSCH)
		{
		   pduCount++;
         if(ulTtiReq)
            ulTtiReq->nUlsch++;
		}
		if(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_PUSCH_UCI)
		{
		   pduCount++;
         if(ulTtiReq)
            ulTtiReq->nUlsch = PDU_PRESENT;
      }
      if(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_UCI)
      {
         pduCount++;
         if(ulTtiReq)
            ulTtiReq->nUlcch = PDU_PRESENT;
      }
      if(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_SRS)
      {
         pduCount++;
      }
   }
   return pduCount;
}
#endif

/***********************************************************************
 *
 * @brief Set the value of zero correlation config in PRACH PDU
 *
 * @details
 *
 *    Function : setNumCs
 *
 *    Functionality:
 *         -Set the value of zero correlation config in PRACH PDU
 *
 * @params[in] Pointer to zero correlation config
 *             Pointer to MacCellCfg
 * ********************************************************************/

void setNumCs(uint8_t *numCs, MacCellCfg *macCellCfg)
{
#ifdef FAPI
   uint8_t idx;
   if(macCellCfg != NULLP)
   {
      idx = macCellCfg->prachCfg.fdm[0].zeroCorrZoneCfg; 
      *numCs = UnrestrictedSetNcsTable[idx];
   }
#endif
}

/***********************************************************************
 *
 * @brief Fills the PRACH PDU in UL TTI Request
 *
 * @details
 *
 *    Function : fillPrachPdu
 *
 *    Functionality:
 *         -Fills the PRACH PDU in UL TTI Request
 *
 * @params[in] Pointer to Prach Pdu
 *             Pointer to CurrUlSlot
 *             Pointer to macCellCfg
 *             Pointer to msgLen
 * ********************************************************************/

#ifdef FAPI
void fillPrachPdu(fapi_ul_tti_req_pdu_t *ulTtiReqPdu, MacCellCfg *macCellCfg, MacUlSlot *currUlSlot, uint32_t *msgLen)
{
   if(ulTtiReqPdu != NULLP)
   {
      ulTtiReqPdu->pduType = PRACH_PDU_TYPE; 
      ulTtiReqPdu->u.prach_pdu.physCellId = macCellCfg->phyCellId;
      ulTtiReqPdu->u.prach_pdu.numPrachOcas = currUlSlot->ulCellInfo.prachSchInfo.numPrachOcas;
      ulTtiReqPdu->u.prach_pdu.prachFormat = \
      currUlSlot->ulCellInfo.prachSchInfo.prachFormat;
      ulTtiReqPdu->u.prach_pdu.numRa = currUlSlot->ulCellInfo.prachSchInfo.numRa;
      ulTtiReqPdu->u.prach_pdu.prachStartSymbol = \
      currUlSlot->ulCellInfo.prachSchInfo.prachStartSymb;
      setNumCs(&ulTtiReqPdu->u.prach_pdu.numCs, macCellCfg);
      ulTtiReqPdu->u.prach_pdu.beamforming.numPrgs = 0;
      ulTtiReqPdu->u.prach_pdu.beamforming.prgSize = 0;
      ulTtiReqPdu->u.prach_pdu.beamforming.digBfInterfaces = 0;
      ulTtiReqPdu->u.prach_pdu.beamforming.pmi_bfi[0].pmIdx = 0;
      ulTtiReqPdu->u.prach_pdu.beamforming.pmi_bfi[0].beamIdx[0].beamidx = 0;
      ulTtiReqPdu->pduSize = sizeof(fapi_ul_prach_pdu_t); 

      SET_MSG_LEN(*msgLen, (sizeof(ulTtiReqPdu->pduType) + \
         sizeof(ulTtiReqPdu->pduSize) + sizeof(fapi_ul_prach_pdu_t)));
   }
}

void fillPuschPdu(fapi_ul_tti_req_pdu_t *ulTtiReqPdu, MacCellCfg *macCellCfg, MacUlSlot *currUlSlot, uint32_t *msgLen)
{
   if(ulTtiReqPdu != NULLP)
   {
      ulTtiReqPdu->pduType = PUSCH_PDU_TYPE;
      ulTtiReqPdu->u.pusch_pdu.pduBitMap = 1;
      ulTtiReqPdu->u.pusch_pdu.rnti = currUlSlot->ulCellInfo.crnti;
		/* TODO : Fill handle in raCb when scheduling pusch and access here */
      ulTtiReqPdu->u.pusch_pdu.handle = 100;
      ulTtiReqPdu->u.pusch_pdu.bwpSize = macCellCfg->initialUlBwp.bwp.numPrb;
      ulTtiReqPdu->u.pusch_pdu.bwpStart = macCellCfg->initialUlBwp.bwp.firstPrb;
      ulTtiReqPdu->u.pusch_pdu.subCarrierSpacing = \
		   macCellCfg->initialUlBwp.bwp.scs;
      ulTtiReqPdu->u.pusch_pdu.cyclicPrefix = \
		   macCellCfg->initialUlBwp.bwp.cyclicPrefix;
      ulTtiReqPdu->u.pusch_pdu.targetCodeRate = 308;
      ulTtiReqPdu->u.pusch_pdu.qamModOrder = 2;
      ulTtiReqPdu->u.pusch_pdu.mcsIndex = \
		   currUlSlot->ulCellInfo.schPuschInfo.tbInfo.mcs;
      ulTtiReqPdu->u.pusch_pdu.mcsTable = 0;
      ulTtiReqPdu->u.pusch_pdu.transformPrecoding = 1;
      ulTtiReqPdu->u.pusch_pdu.dataScramblingId = currUlSlot->ulCellInfo.cellId;
      ulTtiReqPdu->u.pusch_pdu.nrOfLayers = 1;
      ulTtiReqPdu->u.pusch_pdu.ulDmrsSymbPos = 4;
      ulTtiReqPdu->u.pusch_pdu.dmrsConfigType = 0;
      ulTtiReqPdu->u.pusch_pdu.ulDmrsScramblingId = currUlSlot->ulCellInfo.cellId;
      ulTtiReqPdu->u.pusch_pdu.scid = 0;
      ulTtiReqPdu->u.pusch_pdu.numDmrsCdmGrpsNoData = 1;
      ulTtiReqPdu->u.pusch_pdu.dmrsPorts = 0;
      ulTtiReqPdu->u.pusch_pdu.resourceAlloc = \
		   currUlSlot->ulCellInfo.schPuschInfo.resAllocType;
      ulTtiReqPdu->u.pusch_pdu.rbStart = \
		   currUlSlot->ulCellInfo.schPuschInfo.fdAlloc.startPrb;
      ulTtiReqPdu->u.pusch_pdu.rbSize = \
		   currUlSlot->ulCellInfo.schPuschInfo.fdAlloc.numPrb;
      ulTtiReqPdu->u.pusch_pdu.vrbToPrbMapping = 0;
      ulTtiReqPdu->u.pusch_pdu.frequencyHopping = 0;
      ulTtiReqPdu->u.pusch_pdu.txDirectCurrentLocation = 0;
      ulTtiReqPdu->u.pusch_pdu.uplinkFrequencyShift7p5khz = 0;
      ulTtiReqPdu->u.pusch_pdu.startSymbIndex = \
		   currUlSlot->ulCellInfo.schPuschInfo.tdAlloc.startSymb;
      ulTtiReqPdu->u.pusch_pdu.nrOfSymbols = \
		   currUlSlot->ulCellInfo.schPuschInfo.tdAlloc.numSymb;
      ulTtiReqPdu->u.pusch_pdu.puschData.rvIndex = \
		   currUlSlot->ulCellInfo.schPuschInfo.tbInfo.rv;
      ulTtiReqPdu->u.pusch_pdu.puschData.harqProcessId = \
		   currUlSlot->ulCellInfo.schPuschInfo.harqProcId;
      ulTtiReqPdu->u.pusch_pdu.puschData.newDataIndicator = \
		   currUlSlot->ulCellInfo.schPuschInfo.tbInfo.ndi;
      ulTtiReqPdu->u.pusch_pdu.puschData.tbSize = \
		   currUlSlot->ulCellInfo.schPuschInfo.tbInfo.tbSize;
		/* numCb is 0 for new transmission */
      ulTtiReqPdu->u.pusch_pdu.puschData.numCb = 0;

      ulTtiReqPdu->pduSize = sizeof(fapi_ul_pusch_pdu_t);
       
      SET_MSG_LEN(*msgLen, (sizeof(ulTtiReqPdu->pduType) + \
         sizeof(ulTtiReqPdu->pduSize) + sizeof(fapi_ul_pusch_pdu_t)));
   }
}
#endif

/*******************************************************************
 *
 * @brief Sends UL TTI Request to PHY
 *
 * @details
 *
 *    Function : handleUlTtiReq
 *
 *    Functionality:
 *         -Sends FAPI Param req to PHY
 *
 * @params[in]  Pointer to CmLteTimingInfo
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint16_t handleUlTtiReq(SlotIndInfo currTimingInfo)
{
#ifdef FAPI
   uint8_t    pduIdx = -1;
   uint8_t    numPdu = 0;
   uint32_t   msgLen = 0;
   uint32_t   msgSize = 0;

   fapi_ul_tti_req_t *ulTtiReq = NULLP;
   SlotIndInfo ulTtiReqTimingInfo;

   RgCellCb  *cellCbParams = NULLP;
   MacUlSlot *currUlSlot = NULLP;
   MacCellCfg macCellCfg;
   Inst inst = 0;

   if(clGlobalCp.phyState == PHY_STATE_RUNNING)
	{
		cellCbParams = rgCb[inst].cell;
		macCellCfg = cellCbParams->macCellCfg;

		/* add PHY delta */
		ADD_DELTA_TO_TIME(currTimingInfo,ulTtiReqTimingInfo,PHY_DELTA);

		currUlSlot = &macCb.macCell->ulSlot[ulTtiReqTimingInfo.slot % MAX_SLOT_SUPPORTED];
		numPdu = getnPdus(NULL, currUlSlot);
		msgSize = sizeof(fapi_ul_tti_req_t) + (numPdu*sizeof(fapi_ul_tti_req_pdu_t));
		LWR_MAC_ALLOC(ulTtiReq, msgSize);

		if(ulTtiReq != NULLP)
		{
			memset(ulTtiReq, 0, msgSize);
			ulTtiReq->sfn  = currTimingInfo.sfn;
			ulTtiReq->slot = currTimingInfo.slot;
			ulTtiReq->nPdus = getnPdus(ulTtiReq, currUlSlot);
			ulTtiReq->nGroup = 0;
			if(ulTtiReq->nPdus > 0)
			{
				ulTtiReq->pdus = (fapi_ul_tti_req_pdu_t *)(ulTtiReq + \
						(sizeof(fapi_ul_tti_req_t) - sizeof(fapi_ul_tti_req_pdu_t*)));
				/* Fill Prach Pdu */
				if(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_PRACH)
				{
					pduIdx++;
					fillPrachPdu(&ulTtiReq->pdus[pduIdx], &macCellCfg, currUlSlot, &msgLen);
				}

				/* Fill PUSCH PDU */
				if(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_PUSCH)
				{
					pduIdx++;
					fillPuschPdu(&ulTtiReq->pdus[pduIdx], &macCellCfg, currUlSlot, &msgLen);
				}

				if((currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_PRACH) || \
						(currUlSlot->ulCellInfo.dataType & SCH_DATATYPE_PUSCH))
				{
					msgLen += (sizeof(fapi_ul_tti_req_t) - sizeof(fapi_msg_t));
					fillMsgHeader(&ulTtiReq->header, FAPI_UL_TTI_REQUEST, msgLen);

					DU_LOG("\nLWR_MAC: Sending UL TTI Request");
					LwrMacSendToPhy(ulTtiReq->header.message_type_id, msgSize, (void *)ulTtiReq);
				}   
			} 
			else
			{
				msgLen = sizeof(fapi_ul_tti_req_t) - sizeof(fapi_msg_t);
				fillMsgHeader(&ulTtiReq->header, FAPI_UL_TTI_REQUEST, msgLen);

				DU_LOG("\nLWR_MAC: Sending UL TTI Request");
				LwrMacSendToPhy(ulTtiReq->header.message_type_id, msgSize, (void *)ulTtiReq);
			}
			memset(&currUlSlot, 0, sizeof(MacUlSlot));
			return ROK;
		}
		else
		{
			DU_LOG("\nLWR_MAC: Failed to allocate memory for UL TTI Request");
			memset(&currUlSlot, 0, sizeof(MacUlSlot));
			return RFAILED;
		}
	}
   else
   {
       lwr_mac_handleInvalidEvt(&currTimingInfo);
   }
#endif
   return ROK;
}

lwrMacFsmHdlr fapiEvtHdlr[MAX_STATE][MAX_EVENT] =
{
   {
      /* PHY_STATE_IDLE */
       lwr_mac_handleParamReqEvt,
       lwr_mac_handleParamRspEvt,
       lwr_mac_handleConfigReqEvt,
       lwr_mac_handleConfigRspEvt,
       lwr_mac_handleInvalidEvt,
       lwr_mac_handleInvalidEvt,
   },
   {
       /* PHY_STATE_CONFIGURED */
       lwr_mac_handleParamReqEvt,
       lwr_mac_handleParamRspEvt,
       lwr_mac_handleConfigReqEvt,
       lwr_mac_handleConfigRspEvt,
       lwr_mac_handleStartReqEvt,
       lwr_mac_handleInvalidEvt,
   },
   {
       /* PHY_STATE_RUNNING */
       lwr_mac_handleInvalidEvt,
       lwr_mac_handleInvalidEvt,
       lwr_mac_handleConfigReqEvt,
       lwr_mac_handleConfigRspEvt,
       lwr_mac_handleInvalidEvt,
       lwr_mac_handleStopReqEvt,
   }
};

/*******************************************************************
 *
 * @brief Sends message to LWR_MAC Fsm Event Handler
 *
 * @details
 *
 *    Function : sendToLowerMac
 *
 *    Functionality:
 *         -Sends message to LowerMac
 *
 * @params[in] Message Type
 *             Message Length
 *             Messaga Pointer
 *
 * @return void
 *
******************************************************************/
void sendToLowerMac(uint16_t msgType, uint32_t msgLen, void *msg)
{
   clGlobalCp.event = msgType;
   fapiEvtHdlr[clGlobalCp.phyState][clGlobalCp.event](msg);
}
/**********************************************************************
         End of file
**********************************************************************/
