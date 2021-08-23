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


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "lwr_mac.h"
#ifdef INTEL_FAPI
#include "fapi.h"
#include "fapi_vendor_extension.h"
#endif
#ifdef INTEL_WLS_MEM
#include "wls_lib.h"
#endif
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "mac_utils.h"

#define MIB_SFN_BITMASK 0xFC
#define PDCCH_PDU_TYPE 0
#define PDSCH_PDU_TYPE 1
#define SSB_PDU_TYPE 3
#define PRACH_PDU_TYPE 0
#define PUSCH_PDU_TYPE 1
#define PUCCH_PDU_TYPE 2
#define PDU_PRESENT 1
#define SET_MSG_LEN(x, size) x += size

/* Global variables */
LwrMacCb lwrMacCb;

uint8_t UnrestrictedSetNcsTable[MAX_ZERO_CORR_CFG_IDX];
void fapiMacConfigRsp(uint16_t cellId);
uint16_t sendTxDataReq(SlotTimingInfo currTimingInfo, DlSchedInfo *dlInfo, p_fapi_api_queue_elem_t prevElem);
uint16_t fillUlTtiReq(SlotTimingInfo currTimingInfo, p_fapi_api_queue_elem_t prevElem);
uint16_t fillUlDciReq(SlotTimingInfo currTimingInfo, p_fapi_api_queue_elem_t prevElem);
uint8_t lwr_mac_procStopReqEvt(SlotTimingInfo slotInfo, p_fapi_api_queue_elem_t  prevElem);

void lwrMacLayerInit(Region region, Pool pool)
{
#ifdef INTEL_WLS_MEM
   uint8_t idx;
#endif

   memset(&lwrMacCb, 0, sizeof(LwrMacCb));
   lwrMacCb.region = region;
   lwrMacCb.pool = pool;
   lwrMacCb.clCfgDone = TRUE;
   lwrMacCb.numCell = 0;
   lwrMacCb.phyState = PHY_STATE_IDLE;

#ifdef INTEL_WLS_MEM
   /* Initializing WLS free mem list */
   lwrMacCb.phySlotIndCntr = 1;
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
 *    Function : lwr_mac_procInvalidEvt
 *
 *    Functionality:
 *         - Displays the PHY state when the invalid event occurs
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t lwr_mac_procInvalidEvt(void *msg)
{
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : INVALID_EVENT\n");
#endif
   DU_LOG("\nERROR  -->  LWR_MAC: Error Indication Event[%d] received in state [%d]", lwrMacCb.event, lwrMacCb.phyState);
   return ROK;
}

#ifdef INTEL_FAPI
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
void fillMsgHeader(fapi_msg_t *hdr, uint16_t msgType, uint16_t msgLen)
{
   memset(hdr, 0, sizeof(fapi_msg_t));
   hdr->msg_id = msgType;
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
void fillTlvs(fapi_uint32_tlv_t *tlv, uint16_t tag, uint16_t length,
      uint32_t value, uint32_t *msgLen)
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
void fillCyclicPrefix(uint8_t value, ClCellParam **cellPtr)
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

void fillSubcarrierSpaceDl(uint8_t value, ClCellParam **cellPtr)
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

void fillBandwidthDl(uint16_t value, ClCellParam **cellPtr)
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

void fillSubcarrierSpaceUl(uint8_t value, ClCellParam **cellPtr)
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

void fillBandwidthUl(uint16_t value, ClCellParam **cellPtr)
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

void fillCCEmaping(uint8_t value,  ClCellParam **cellPtr)
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

void fillPucchFormat(uint8_t value, ClCellParam **cellPtr)
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

void fillPdschMappingType(uint8_t value, ClCellParam **cellPtr)
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

void fillPdschAllocationType(uint8_t value, ClCellParam **cellPtr)
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
void fillPrbMappingType(uint8_t value, ClCellParam **cellPtr)
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

void fillPdschDmrsConfigType(uint8_t value, ClCellParam **cellPtr)
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
void fillPdschDmrsLength(uint8_t value, ClCellParam **cellPtr)
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

void fillPdschDmrsAddPos(uint8_t value, ClCellParam **cellPtr)
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
void fillModulationOrderDl(uint8_t value, ClCellParam **cellPtr)
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

void fillPuschDmrsConfig(uint8_t value, ClCellParam **cellPtr)
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

void fillPuschDmrsLength(uint8_t value, ClCellParam **cellPtr)
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

void fillPuschDmrsAddPos(uint8_t value, ClCellParam **cellPtr)
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

void fillPuschMappingType(uint8_t value, ClCellParam **cellPtr)
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

void fillPuschAllocationType(uint8_t value, ClCellParam **cellPtr)
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

void fillPuschPrbMappingType(uint8_t value, ClCellParam **cellPtr)
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

void fillModulationOrderUl(uint8_t value, ClCellParam **cellPtr)
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

void fillPuschAggregationFactor(uint8_t value, ClCellParam **cellPtr)
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

void fillPrachLongFormat(uint8_t value, ClCellParam **cellPtr)
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

void fillPrachShortFormat(uint8_t value, ClCellParam **cellPtr)
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

void fillFdOccasions(uint8_t value, ClCellParam **cellPtr)
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

void fillRssiMeas(uint8_t value, ClCellParam **cellPtr)
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
   void *posPtr;
   posPtr   = &tlv->tl.tag;
   posPtr   += sizeof(tlv->tl.tag);
   posPtr   += sizeof(tlv->tl.length);
   /*TO DO: malloc to SSI memory */
   if(type == FAPI_UINT_8)
   {
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
      DU_LOG("\nERROR  -->  LWR_MAC: Value Extraction failed" );
      return RFAILED;
   }
}
#endif /* FAPI */

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
void setMibPdu(uint8_t *mibPdu, uint32_t *val, uint16_t sfn)
{
   *mibPdu |= (((uint8_t)(sfn << 2)) & MIB_SFN_BITMASK);
   *val = (mibPdu[0] << 24 | mibPdu[1] << 16 | mibPdu[2] << 8);
   DU_LOG("\nDEBUG  -->  LWR_MAC: MIB PDU %x", *val);
}

/*******************************************************************
 *
 * @brief Sends FAPI Param req to PHY
 *
 * @details
 *
 *    Function : lwr_mac_procParamReqEvt
 *
 *    Functionality:
 *         -Sends FAPI Param req to PHY
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t lwr_mac_procParamReqEvt(void *msg)
{
#ifdef INTEL_FAPI
#ifdef CALL_FLOW_DEBUG_LOG 
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : PARAM_REQ\n");
#endif

   /* startGuardTimer(); */
   fapi_param_req_t         *paramReq = NULL;
   fapi_msg_header_t        *msgHeader;
   p_fapi_api_queue_elem_t  paramReqElem;
   p_fapi_api_queue_elem_t  headerElem;

   LWR_MAC_ALLOC(paramReqElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_param_req_t)));
   if(paramReq != NULL)
   {
      FILL_FAPI_LIST_ELEM(paramReqElem, NULLP, FAPI_PARAM_REQUEST, 1, \
         sizeof(fapi_tx_data_req_t));
      paramReq = (fapi_param_req_t *)(paramReqElem +1);
      memset(paramReq, 0, sizeof(fapi_param_req_t));
      fillMsgHeader(&paramReq->header, FAPI_PARAM_REQUEST, sizeof(fapi_param_req_t));

      /* Fill message header */
      LWR_MAC_ALLOC(headerElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_msg_header_t)));
      if(!headerElem)
      {
         DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for param req header");
         LWR_MAC_FREE(paramReqElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_param_req_t)));
         return RFAILED;
      }
      FILL_FAPI_LIST_ELEM(headerElem, paramReqElem, FAPI_VENDOR_MSG_HEADER_IND, 1, \
         sizeof(fapi_msg_header_t));
      msgHeader = (fapi_msg_header_t *)(headerElem + 1);
      msgHeader->num_msg = 1;
      msgHeader->handle = 0;

      DU_LOG("\nDEBUG  -->  LWR_MAC: Sending Param Request to Phy");
      LwrMacSendToL1(headerElem);
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Failed to allocate memory for Param Request");
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
 *    Function : lwr_mac_procParamRspEvt
 *
 *    Functionality:
 *         -Sends FAPI Param rsp to MAC via PHY
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t lwr_mac_procParamRspEvt(void *msg)
{
#ifdef INTEL_FAPI
   /* stopGuardTimer(); */
   uint8_t index;
   uint32_t encodedVal;
   fapi_param_resp_t *paramRsp;
   ClCellParam *cellParam = NULLP;

   paramRsp = (fapi_param_resp_t *)msg;
   DU_LOG("\nINFO  -->  LWR_MAC: Received EVENT[%d] at STATE[%d]", lwrMacCb.event, lwrMacCb.phyState);

   if(paramRsp != NULLP)
   {
      MAC_ALLOC(cellParam, sizeof(ClCellParam));
      if(cellParam != NULLP)
      {
	 DU_LOG("\nDEBUG  -->  LWR_MAC: Filling TLVS into MAC API");
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
		     if(encodedVal != RFAILED && encodedVal != lwrMacCb.phyState)
		     {
			DU_LOG("\nERROR  -->  PhyState mismatch [%d][%d]", lwrMacCb.phyState, lwrMacCb.event);
			return RFAILED;
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
		     //DU_LOG("\nERROR  -->   Invalid value for TLV[%x] at index[%d]", paramRsp->tlvs[index].tl.tag, index);
		     break;
	       }
	    }
	    MAC_FREE(cellParam, sizeof(ClCellParam));
	    sendToLowerMac(FAPI_CONFIG_REQUEST, 0, (void *)NULL);
	    return ROK;
	 }
	 else
	 {
	    DU_LOG("\nERROR  -->   LWR_MAC: Invalid error code %d", paramRsp->error_code);
	    return RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Failed to allocate memory for cell param");
	 return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC:  Param Response received from PHY is NULL");
      return RFAILED;
   }
#else
   return ROK;
#endif
}

#ifdef INTEL_TIMER_MODE
uint8_t lwr_mac_procIqSamplesReqEvt(void *msg)
{
   void * wlsHdlr = NULLP;
   fapi_msg_header_t *msgHeader;
   fapi_vendor_ext_iq_samples_req_t *iqSampleReq;
   p_fapi_api_queue_elem_t  headerElem;
   p_fapi_api_queue_elem_t  iqSampleElem;
   char filename[100] = "/root/intel/FlexRAN/testcase/ul/mu0_20mhz/2/uliq00_prach_tst2.bin"; 

   uint8_t buffer[] ={0,0,0,0,0,2,11,0,212,93,40,0,20,137,38,0,20,0,20,0,0,8,0,8,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,1,0,0,0,0,0,0,1,0,2,0,0,0,0,0,0,0,1,0};

   size_t bufferSize = sizeof(buffer) / sizeof(buffer[0]);

   /* Fill IQ sample req */
   mtGetWlsHdl(&wlsHdlr);
   //iqSampleElem = (p_fapi_api_queue_elem_t)WLS_Alloc(wlsHdlr, \
      (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_ext_iq_samples_req_t))); 
   LWR_MAC_ALLOC(iqSampleElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_ext_iq_samples_req_t)));
   if(!iqSampleElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for IQ sample req");
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(iqSampleElem, NULLP, FAPI_VENDOR_EXT_UL_IQ_SAMPLES, 1, \
      sizeof(fapi_vendor_ext_iq_samples_req_t));

   iqSampleReq = (fapi_vendor_ext_iq_samples_req_t *)(iqSampleElem + 1);
   memset(iqSampleReq, 0, sizeof(fapi_vendor_ext_iq_samples_req_t));
   fillMsgHeader(&iqSampleReq->header, FAPI_VENDOR_EXT_UL_IQ_SAMPLES, \
      sizeof(fapi_vendor_ext_iq_samples_req_t));

   iqSampleReq->iq_samples_info.carrNum = 0;
   iqSampleReq->iq_samples_info.numSubframes = 40;
   iqSampleReq->iq_samples_info.nIsRadioMode = 0;
   iqSampleReq->iq_samples_info.timerModeFreqDomain = 0;
   iqSampleReq->iq_samples_info.phaseCompensationEnable = 0;
   iqSampleReq->iq_samples_info.startFrameNum = 0;
   iqSampleReq->iq_samples_info.startSlotNum = 0;
   iqSampleReq->iq_samples_info.startSymNum = 0;
   strncpy(iqSampleReq->iq_samples_info.filename_in_ul_iq[0], filename, 100);
   memcpy(iqSampleReq->iq_samples_info.buffer, buffer, bufferSize);

   /* TODO : Fill remaining parameters */

   /* Fill message header */
   LWR_MAC_ALLOC(headerElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_msg_header_t)));
   if(!headerElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for FAPI header in lwr_mac_procIqSamplesReqEvt");
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(headerElem, iqSampleElem, FAPI_VENDOR_MSG_HEADER_IND, 1, \
     sizeof(fapi_msg_header_t));
   msgHeader = (fapi_msg_header_t *)(headerElem + 1);
   msgHeader->num_msg = 1; 
   msgHeader->handle = 0;

   DU_LOG("\nINFO   -->  LWR_MAC: Sending IQ Sample request to Phy");
   LwrMacSendToL1(headerElem);
   return ROK;
}
#endif

/*******************************************************************
 *
 * @brief Sends FAPI Config req to PHY
 *
 * @details
 *
 *    Function : lwr_mac_procConfigReqEvt
 *
 *    Functionality:
 *         -Sends FAPI Config Req to PHY
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t lwr_mac_procConfigReqEvt(void *msg)
{
#ifdef INTEL_FAPI
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : CONFIG_REQ\n");
#endif
#ifdef NR_TDD
   uint8_t slotIdx = 0; 
   uint8_t symbolIdx =0;
#endif   
   uint8_t index = 0;
   uint16_t *cellId =NULLP;
   uint16_t cellIdx =0;
   uint32_t msgLen = 0;
   uint32_t mib = 0;
   MacCellCfg macCfgParams;
   fapi_vendor_msg_t *vendorMsg;
   fapi_config_req_t *configReq;
   fapi_msg_header_t *msgHeader;
   p_fapi_api_queue_elem_t  headerElem;
   p_fapi_api_queue_elem_t  vendorMsgQElem;
   p_fapi_api_queue_elem_t  cfgReqQElem;

   DU_LOG("\nINFO  -->  LWR_MAC: Received EVENT[%d] at STATE[%d]", lwrMacCb.event, \
	 lwrMacCb.phyState);

   cellId = (uint16_t *)msg;
   GET_CELL_IDX(*cellId, cellIdx);
   macCfgParams = macCb.macCell[cellIdx]->macCellCfg;

   /* Fill Cell Configuration in lwrMacCb */
   memset(&lwrMacCb.cellCb[lwrMacCb.numCell], 0, sizeof(LwrMacCellCb));
   lwrMacCb.cellCb[lwrMacCb.numCell].cellId = macCfgParams.cellId;
   lwrMacCb.cellCb[lwrMacCb.numCell].phyCellId = macCfgParams.phyCellId; 
   lwrMacCb.numCell++;

   /* Allocte And fill Vendor msg */
   LWR_MAC_ALLOC(vendorMsgQElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));  
   if(!vendorMsgQElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for vendor msg in config req");
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(vendorMsgQElem, NULLP, FAPI_VENDOR_MESSAGE, 1, sizeof(fapi_vendor_msg_t)); 
   vendorMsg = (fapi_vendor_msg_t *)(vendorMsgQElem + 1);
   fillMsgHeader(&vendorMsg->header, FAPI_VENDOR_MESSAGE, sizeof(fapi_vendor_msg_t));
   vendorMsg->config_req_vendor.hopping_id = 0;
   vendorMsg->config_req_vendor.carrier_aggregation_level = 0;
   vendorMsg->config_req_vendor.group_hop_flag = 0;
   vendorMsg->config_req_vendor.sequence_hop_flag = 0;
   vendorMsg->start_req_vendor.sfn = 0;
   vendorMsg->start_req_vendor.slot = 0;
   vendorMsg->start_req_vendor.mode = 4;
#ifdef DEBUG_MODE
   vendorMsg->start_req_vendor.count = 0;
   vendorMsg->start_req_vendor.period = 1;
#endif
   /* Fill FAPI config req */
   LWR_MAC_ALLOC(cfgReqQElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_config_req_t)));
   if(!cfgReqQElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for config req");
      LWR_MAC_ALLOC(vendorMsgQElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(cfgReqQElem, vendorMsgQElem, FAPI_CONFIG_REQUEST, 1, \
      sizeof(fapi_config_req_t));

   configReq = (fapi_config_req_t *)(cfgReqQElem + 1);
   memset(configReq, 0, sizeof(fapi_config_req_t));
   fillMsgHeader(&configReq->header, FAPI_CONFIG_REQUEST, sizeof(fapi_config_req_t));
   configReq->number_of_tlvs = 25;
   msgLen = sizeof(configReq->number_of_tlvs);

   if(macCfgParams.dlCarrCfg.pres)
   {
      fillTlvs(&configReq->tlvs[index++], FAPI_DL_BANDWIDTH_TAG,           \
         sizeof(uint32_t), macCfgParams.dlCarrCfg.bw, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_DL_FREQUENCY_TAG,           \
         sizeof(uint32_t), macCfgParams.dlCarrCfg.freq, &msgLen);
      /* Due to bug in Intel FT code, commenting TLVs that are are not 
       * needed to avoid error. Must be uncommented when FT bug is fixed */
      //fillTlvs(&configReq->tlvs[index++], FAPI_DL_K0_TAG,                  \
         sizeof(uint16_t), macCfgParams.dlCarrCfg.k0[0], &msgLen);
      //fillTlvs(&configReq->tlvs[index++], FAPI_DL_GRIDSIZE_TAG,            \
         sizeof(uint16_t), macCfgParams.dlCarrCfg.gridSize[0], &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_NUM_TX_ANT_TAG,             \
         sizeof(uint16_t), macCfgParams.dlCarrCfg.numAnt, &msgLen);
   }
   if(macCfgParams.ulCarrCfg.pres)
   {
      fillTlvs(&configReq->tlvs[index++], FAPI_UPLINK_BANDWIDTH_TAG,       \
	    sizeof(uint32_t), macCfgParams.ulCarrCfg.bw, &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_UPLINK_FREQUENCY_TAG,       \
	    sizeof(uint32_t), macCfgParams.ulCarrCfg.freq, &msgLen);
      //fillTlvs(&configReq->tlvs[index++], FAPI_UL_K0_TAG,                  \
      sizeof(uint16_t), macCfgParams.ulCarrCfg.k0[0], &msgLen);
      //fillTlvs(&configReq->tlvs[index++], FAPI_UL_GRID_SIZE_TAG,           \
      sizeof(uint16_t), macCfgParams.ulCarrCfg.gridSize[0], &msgLen);
      fillTlvs(&configReq->tlvs[index++], FAPI_NUM_RX_ANT_TAG,             \
	    sizeof(uint16_t), macCfgParams.ulCarrCfg.numAnt, &msgLen);
   }
   //fillTlvs(&configReq->tlvs[index++], FAPI_FREQUENCY_SHIFT_7P5_KHZ_TAG,   \
   sizeof(uint8_t), macCfgParams.freqShft, &msgLen);

   /* fill cell config */
   fillTlvs(&configReq->tlvs[index++], FAPI_PHY_CELL_ID_TAG,               \
	 sizeof(uint8_t), macCfgParams.phyCellId, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_FRAME_DUPLEX_TYPE_TAG,         \
	 sizeof(uint8_t), macCfgParams.dupType, &msgLen);

   /* fill SSB configuration */
   fillTlvs(&configReq->tlvs[index++], FAPI_SS_PBCH_POWER_TAG,             \
	 sizeof(uint32_t), macCfgParams.ssbCfg.ssbPbchPwr, &msgLen);
   //fillTlvs(&configReq->tlvs[index++], FAPI_BCH_PAYLOAD_TAG,               \
   sizeof(uint8_t), macCfgParams.ssbCfg.bchPayloadFlag, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_SCS_COMMON_TAG,                \
	 sizeof(uint8_t), macCfgParams.ssbCfg.scsCmn, &msgLen);

   /* fill PRACH configuration */
   //fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_SEQUENCE_LENGTH_TAG,     \
   sizeof(uint8_t), macCfgParams.prachCfg.prachSeqLen, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_SUBC_SPACING_TAG,        \
	 sizeof(uint8_t), macCfgParams.prachCfg.prachSubcSpacing, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_RESTRICTED_SET_CONFIG_TAG,     \
	 sizeof(uint8_t), macCfgParams.prachCfg.prachRstSetCfg, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_NUM_PRACH_FD_OCCASIONS_TAG,
	 sizeof(uint8_t), macCfgParams.prachCfg.msg1Fdm, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_CONFIG_INDEX_TAG,
	 sizeof(uint8_t), macCfgParams.prachCfg.prachCfgIdx, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_ROOT_SEQUENCE_INDEX_TAG, \
	 sizeof(uint16_t), macCfgParams.prachCfg.fdm[0].rootSeqIdx, &msgLen);
   //fillTlvs(&configReq->tlvs[index++], FAPI_NUM_ROOT_SEQUENCES_TAG,        \
   sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].numRootSeq, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_K1_TAG,                        \
	 sizeof(uint16_t), macCfgParams.prachCfg.fdm[0].k1, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_ZERO_CORR_CONF_TAG ,     \
	 sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].zeroCorrZoneCfg, &msgLen);
   //fillTlvs(&configReq->tlvs[index++], FAPI_NUM_UNUSED_ROOT_SEQUENCES_TAG, \
   sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].numUnusedRootSeq, &msgLen);
   /* if(macCfgParams.prachCfg.fdm[0].numUnusedRootSeq)
      {
      for(idx = 0; idx < macCfgParams.prachCfg.fdm[0].numUnusedRootSeq; idx++)
      fillTlvs(&configReq->tlvs[index++], FAPI_UNUSED_ROOT_SEQUENCES_TAG,   \
      sizeof(uint8_t), macCfgParams.prachCfg.fdm[0].unsuedRootSeq[idx], \
      &msgLen);
      }
      else
      {
      macCfgParams.prachCfg.fdm[0].unsuedRootSeq = NULL;
      }*/

   fillTlvs(&configReq->tlvs[index++], FAPI_SSB_PER_RACH_TAG,              \
	 sizeof(uint8_t), macCfgParams.prachCfg.ssbPerRach, &msgLen);
   //fillTlvs(&configReq->tlvs[index++], FAPI_PRACH_MULTIPLE_CARRIERS_IN_A_BAND_TAG,  \
   sizeof(uint8_t), macCfgParams.prachCfg.prachMultCarrBand, &msgLen);

   /* fill SSB table */
   fillTlvs(&configReq->tlvs[index++], FAPI_SSB_OFFSET_POINT_A_TAG,        \
	 sizeof(uint16_t), macCfgParams.ssbCfg.ssbOffsetPointA, &msgLen);
   //fillTlvs(&configReq->tlvs[index++], FAPI_BETA_PSS_TAG,                  \
   sizeof(uint8_t),  macCfgParams.ssbCfg.betaPss, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_SSB_PERIOD_TAG,                \
	 sizeof(uint8_t),  macCfgParams.ssbCfg.ssbPeriod, &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_SSB_SUBCARRIER_OFFSET_TAG,     \
	 sizeof(uint8_t),  macCfgParams.ssbCfg.ssbScOffset, &msgLen);

   setMibPdu(macCfgParams.ssbCfg.mibPdu, &mib, 0);
   fillTlvs(&configReq->tlvs[index++], FAPI_MIB_TAG ,                      \
	 sizeof(uint32_t), mib, &msgLen);

   fillTlvs(&configReq->tlvs[index++], FAPI_SSB_MASK_TAG,                  \
	 sizeof(uint32_t), macCfgParams.ssbCfg.ssbMask[0], &msgLen);
   fillTlvs(&configReq->tlvs[index++], FAPI_BEAM_ID_TAG,                   \
	 sizeof(uint8_t),  macCfgParams.ssbCfg.beamId[0], &msgLen);
   //fillTlvs(&configReq->tlvs[index++], FAPI_SS_PBCH_MULTIPLE_CARRIERS_IN_A_BAND_TAG, \
   sizeof(uint8_t), macCfgParams.ssbCfg.multCarrBand, &msgLen);
   //fillTlvs(&configReq->tlvs[index++], FAPI_MULTIPLE_CELLS_SS_PBCH_IN_A_CARRIER_TAG, \
   sizeof(uint8_t), macCfgParams.ssbCfg.multCellCarr, &msgLen);

#ifdef NR_TDD
   /* fill TDD table */
   fillTlvs(&configReq->tlvs[index++], FAPI_TDD_PERIOD_TAG,                \
   sizeof(uint8_t), macCfgParams.tddCfg.tddPeriod, &msgLen);
   for(slotIdx =0 ;slotIdx< MAX_TDD_PERIODICITY_SLOTS; slotIdx++) 
   {
      for(symbolIdx = 0; symbolIdx< MAX_SYMB_PER_SLOT; symbolIdx++)
      {
	 fillTlvs(&configReq->tlvs[index++], FAPI_SLOT_CONFIG_TAG,               \
	       sizeof(uint8_t), macCfgParams.tddCfg.slotCfg[slotIdx][symbolIdx], &msgLen);
      }
   }
#endif   
   
   /* fill measurement config */
   //fillTlvs(&configReq->tlvs[index++], FAPI_RSSI_MEASUREMENT_TAG,          \
   sizeof(uint8_t), macCfgParams.rssiUnit, &msgLen);

   /* fill DMRS Type A Pos */
   fillTlvs(&configReq->tlvs[index++], FAPI_DMRS_TYPE_A_POS_TAG,           \
	 sizeof(uint8_t), macCfgParams.dmrsTypeAPos, &msgLen);

   /* Fill message header */
   LWR_MAC_ALLOC(headerElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_msg_header_t)));
   if(!headerElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for vendor msg in config req");
      LWR_MAC_ALLOC(cfgReqQElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_config_req_t)));
      LWR_MAC_ALLOC(vendorMsgQElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(headerElem, cfgReqQElem, FAPI_VENDOR_MSG_HEADER_IND, 1, \
	 sizeof(fapi_msg_header_t));
   msgHeader = (fapi_msg_header_t *)(headerElem + 1);
   msgHeader->num_msg = 2; /* Config req msg and vendor specific msg */
   msgHeader->handle = 0;

   DU_LOG("\nDEBUG  -->  LWR_MAC: Sending Config Request to Phy");
   LwrMacSendToL1(headerElem);
#endif

   return ROK;
} /* lwr_mac_handleConfigReqEvt */

/*******************************************************************
 *
 * @brief Processes config response from phy
 *
 * @details
 *
 *    Function : lwr_mac_procConfigRspEvt
 *
 *    Functionality:
 *          Processes config response from phy
 *
 * @params[in] FAPI message pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t lwr_mac_procConfigRspEvt(void *msg)
{
#ifdef INTEL_FAPI
   fapi_config_resp_t *configRsp;
   configRsp = (fapi_config_resp_t *)msg;

   DU_LOG("\nINFO  -->  LWR_MAC: Received EVENT[%d] at STATE[%d]", lwrMacCb.event, \
	 lwrMacCb.phyState);

   if(configRsp != NULL)
   {
      if(configRsp->error_code == MSG_OK)
      {
	 DU_LOG("\nDEBUG  -->  LWR_MAC: PHY has moved to Configured state \n");
	 lwrMacCb.phyState = PHY_STATE_CONFIGURED;
	 lwrMacCb.cellCb[0].state = PHY_STATE_CONFIGURED;
	 /* TODO : 
	  * Store config response into an intermediate struture and send to MAC
	  * Support LC and LWLC for sending config rsp to MAC 
	  */
	 fapiMacConfigRsp(lwrMacCb.cellCb[0].cellId);
      }
      else
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Invalid error code %d", configRsp->error_code);
	 return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Config Response received from PHY is NULL");
      return RFAILED;
   }
#endif

   return ROK;
} /* lwr_mac_procConfigRspEvt */

/*******************************************************************
 *
 * @brief Build and send start request to phy
 *
 * @details
 *
 *    Function : lwr_mac_procStartReqEvt
 *
 *    Functionality:
 *       Build and send start request to phy
 *
 * @params[in] FAPI message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t lwr_mac_procStartReqEvt(void *msg)
{
#ifdef INTEL_FAPI
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : START_REQ\n");
#endif
   fapi_msg_header_t *msgHeader;
   fapi_start_req_t *startReq;
   fapi_vendor_msg_t *vendorMsg;
   p_fapi_api_queue_elem_t  headerElem;
   p_fapi_api_queue_elem_t  startReqElem;
   p_fapi_api_queue_elem_t  vendorMsgElem;

   /* Allocte And fill Vendor msg */
   LWR_MAC_ALLOC(vendorMsgElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));
   if(!vendorMsgElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for vendor msg in start req");
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(vendorMsgElem, NULLP, FAPI_VENDOR_MESSAGE, 1, sizeof(fapi_vendor_msg_t));
   vendorMsg = (fapi_vendor_msg_t *)(vendorMsgElem + 1);
   fillMsgHeader(&vendorMsg->header, FAPI_VENDOR_MESSAGE, sizeof(fapi_vendor_msg_t));
   vendorMsg->start_req_vendor.sfn = 0;
   vendorMsg->start_req_vendor.slot = 0;
   vendorMsg->start_req_vendor.mode = 4; /* for Radio mode */
#ifdef DEBUG_MODE
   vendorMsg->start_req_vendor.count = 0;
   vendorMsg->start_req_vendor.period = 1;
#endif

   /* Fill FAPI config req */
   LWR_MAC_ALLOC(startReqElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_start_req_t)));
   if(!startReqElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for start req");
      LWR_MAC_ALLOC(vendorMsgElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(startReqElem, vendorMsgElem, FAPI_START_REQUEST, 1, \
      sizeof(fapi_start_req_t));

   startReq = (fapi_start_req_t *)(startReqElem + 1);
   memset(startReq, 0, sizeof(fapi_start_req_t));
   fillMsgHeader(&startReq->header, FAPI_START_REQUEST, sizeof(fapi_start_req_t));

   /* Fill message header */
   LWR_MAC_ALLOC(headerElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_msg_header_t)));
   if(!headerElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for vendor msg in config req");
      LWR_MAC_ALLOC(startReqElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_start_req_t)));
      LWR_MAC_ALLOC(vendorMsgElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(headerElem, startReqElem, FAPI_VENDOR_MSG_HEADER_IND, 1, \
      sizeof(fapi_msg_header_t));
   msgHeader = (fapi_msg_header_t *)(headerElem + 1);
   msgHeader->num_msg = 2; /* Start req msg and vendor specific msg */
   msgHeader->handle = 0;

   /* Send to PHY */
   DU_LOG("\nDEBUG  -->  LWR_MAC: Sending Start Request to Phy");
   LwrMacSendToL1(headerElem);
#endif
   return ROK;
} /* lwr_mac_procStartReqEvt */

/*******************************************************************
 *
 * @brief Sends FAPI Stop Req to PHY
 *
 * @details
 *
 *    Function : lwr_mac_procStopReqEvt
 *
 *    Functionality:
 *         -Sends FAPI Stop Req to PHY
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 ********************************************************************/

uint8_t lwr_mac_procStopReqEvt(SlotTimingInfo slotInfo, p_fapi_api_queue_elem_t  prevElem)
{
#ifdef INTEL_FAPI
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : STOP_REQ\n");
#endif

   fapi_stop_req_t   *stopReq;
   fapi_vendor_msg_t *vendorMsg;
   p_fapi_api_queue_elem_t  stopReqElem;
   p_fapi_api_queue_elem_t  vendorMsgElem;

   /* Allocte And fill Vendor msg */
   LWR_MAC_ALLOC(vendorMsgElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));
   if(!vendorMsgElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for vendor msg in stop req");
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(vendorMsgElem, NULLP, FAPI_VENDOR_MESSAGE, 1, sizeof(fapi_vendor_msg_t));
   vendorMsg = (fapi_vendor_msg_t *)(vendorMsgElem + 1);
   fillMsgHeader(&vendorMsg->header, FAPI_VENDOR_MESSAGE, sizeof(fapi_vendor_msg_t));
   vendorMsg->stop_req_vendor.sfn = slotInfo.sfn;
   vendorMsg->stop_req_vendor.slot = slotInfo.slot;

   /* Fill FAPI stop req */
   LWR_MAC_ALLOC(stopReqElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_stop_req_t)));
   if(!stopReqElem)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for stop req");
      LWR_MAC_FREE(vendorMsgElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_vendor_msg_t)));
      return RFAILED;
   }
   FILL_FAPI_LIST_ELEM(stopReqElem, vendorMsgElem, FAPI_STOP_REQUEST, 1, \
      sizeof(fapi_stop_req_t));
   stopReq = (fapi_stop_req_t *)(stopReqElem + 1);
   memset(stopReq, 0, sizeof(fapi_stop_req_t));
   fillMsgHeader(&stopReq->header, FAPI_STOP_REQUEST, sizeof(fapi_stop_req_t));

   /* Send to PHY */
   DU_LOG("\nINFO  -->  LWR_MAC: Sending Stop Request to Phy");
   prevElem->p_next = stopReqElem;

#endif
   return ROK;
}

#ifdef INTEL_FAPI
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

uint8_t fillSsbPdu(fapi_dl_tti_req_pdu_t *dlTtiReqPdu, MacCellCfg *macCellCfg,
      MacDlSlot *currDlSlot, uint8_t ssbIdxCount, uint16_t sfn)
{
   uint32_t mibPayload = 0;
   if(dlTtiReqPdu != NULL)
   {
      dlTtiReqPdu->pduType = SSB_PDU_TYPE;     /* SSB PDU */
      dlTtiReqPdu->pdu.ssb_pdu.physCellId = macCellCfg->phyCellId;
      dlTtiReqPdu->pdu.ssb_pdu.betaPss = macCellCfg->ssbCfg.betaPss;
      dlTtiReqPdu->pdu.ssb_pdu.ssbBlockIndex = currDlSlot->dlInfo.brdcstAlloc.ssbInfo[ssbIdxCount].ssbIdx;
      dlTtiReqPdu->pdu.ssb_pdu.ssbSubCarrierOffset = macCellCfg->ssbCfg.ssbScOffset;
      /* ssbOfPdufstA to be filled in ssbCfg */
      dlTtiReqPdu->pdu.ssb_pdu.ssbOffsetPointA = macCellCfg->ssbCfg.ssbOffsetPointA;
      dlTtiReqPdu->pdu.ssb_pdu.bchPayloadFlag = macCellCfg->ssbCfg.bchPayloadFlag;
      /* Bit manipulation for SFN */
      setMibPdu(macCellCfg->ssbCfg.mibPdu, &mibPayload, sfn);
      dlTtiReqPdu->pdu.ssb_pdu.bchPayload.bchPayload = mibPayload;
      dlTtiReqPdu->pdu.ssb_pdu.preCodingAndBeamforming.numPrgs = 0;
      dlTtiReqPdu->pdu.ssb_pdu.preCodingAndBeamforming.prgSize = 0;
      dlTtiReqPdu->pdu.ssb_pdu.preCodingAndBeamforming.digBfInterfaces = 0;
      dlTtiReqPdu->pdu.ssb_pdu.preCodingAndBeamforming.pmi_bfi[0].pmIdx = 0;
      dlTtiReqPdu->pdu.ssb_pdu.preCodingAndBeamforming. \
	 pmi_bfi[0].beamIdx[0].beamidx = macCellCfg->ssbCfg.beamId[0];
      dlTtiReqPdu->pduSize = sizeof(fapi_dl_ssb_pdu_t);  /* Size of SSB PDU */

      return ROK;
   }
   return RFAILED;
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
      uint8_t numBytes=0;
      uint8_t bytePos=0;
      uint8_t bitPos=0;

      uint16_t coreset0Size=0;
      uint16_t rbStart=0;
      uint16_t rbLen=0;
      uint32_t freqDomResAssign=0;
      uint32_t timeDomResAssign=0;
      uint8_t  VRB2PRBMap=0;
      uint32_t modNCodScheme=0;
      uint8_t  redundancyVer=0;
      uint32_t sysInfoInd=0;
      uint32_t reserved=0;

      /* Size(in bits) of each field in DCI format 0_1 
       * as mentioned in spec 38.214 */
      uint8_t freqDomResAssignSize = 0;
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
      dlDciPtr->powerControlOffsetSS = sib1PdcchInfo->dci.txPdcchPower.powerControlOffsetSS;

      /* Calculating freq domain resource allocation field value and size
       * coreset0Size = Size of coreset 0
       * RBStart = Starting Virtual Rsource block
       * RBLen = length of contiguously allocted RBs
       * Spec 38.214 Sec 5.1.2.2.2
       */
      coreset0Size= sib1PdcchInfo->coresetCfg.coreSetSize;
      rbStart = 0;              /* For SIB1 */
      //rbStart = sib1PdcchInfo->dci.pdschCfg->pdschFreqAlloc.freqAlloc.startPrb;
      rbLen = sib1PdcchInfo->dci.pdschCfg->pdschFreqAlloc.freqAlloc.numPrb;

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
      timeDomResAssign = sib1PdcchInfo->dci.pdschCfg->pdschTimeAlloc.
	 rowIndex -1;
      VRB2PRBMap       = sib1PdcchInfo->dci.pdschCfg->pdschFreqAlloc.\
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

      if(numBytes > FAPI_DCI_PAYLOAD_BYTE_LEN)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC : Total bytes for DCI is more than expected");
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
      uint8_t numBytes =0;
      uint8_t bytePos =0;
      uint8_t bitPos =0;

      uint16_t coreset0Size =0;
      uint16_t rbStart =0;
      uint16_t rbLen =0;
      uint32_t freqDomResAssign =0;
      uint8_t timeDomResAssign =0;
      uint8_t  VRB2PRBMap =0;
      uint8_t modNCodScheme =0;
      uint8_t tbScaling =0;
      uint32_t reserved =0;

      /* Size(in bits) of each field in DCI format 1_0 */
      uint8_t freqDomResAssignSize = 0;
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
      dlDciPtr->powerControlOffsetSS = rarPdcchInfo->dci.txPdcchPower.powerControlOffsetSS;

      /* Calculating freq domain resource allocation field value and size
       * coreset0Size = Size of coreset 0
       * RBStart = Starting Virtual Rsource block
       * RBLen = length of contiguously allocted RBs
       * Spec 38.214 Sec 5.1.2.2.2
       */

      /* TODO: Fill values of coreset0Size, rbStart and rbLen */
      coreset0Size= rarPdcchInfo->coresetCfg.coreSetSize;
      rbStart = 0;              /* For SIB1 */
      //rbStart = rarPdcchInfo->dci.pdschCfg->freqAlloc.rbStart;
      rbLen = rarPdcchInfo->dci.pdschCfg->pdschFreqAlloc.freqAlloc.numPrb;

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
      timeDomResAssign = rarPdcchInfo->dci.pdschCfg->pdschTimeAlloc.rowIndex -1;
      VRB2PRBMap       = rarPdcchInfo->dci.pdschCfg->pdschFreqAlloc.vrbPrbMapping;
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

      if(numBytes > FAPI_DCI_PAYLOAD_BYTE_LEN)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC : Total bytes for DCI is more than expected");
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
 * @brief fills DL DCI PDU required for DL TTI info in MAC
 *
 * @details
 *
 *    Function : fillDlMsgDlDciPdu
 *
 *    Functionality:
 *         -Fills the Dl DCI PDU  
 *
 * @params[in] Pointer to fapi_dl_dci_t
 *             Pointer to PdcchCfg
 * @return ROK
 *
 ******************************************************************/
void fillDlMsgDlDciPdu(fapi_dl_dci_t *dlDciPtr, PdcchCfg *pdcchInfo,\
      DlMsgInfo *dlMsgInfo)
{
   if(dlDciPtr != NULLP)
   {
      uint8_t numBytes;
      uint8_t bytePos;
      uint8_t bitPos;

      uint16_t coresetSize = 0;
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
      uint8_t freqDomResAssignSize = 0;
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

      dlDciPtr->rnti = pdcchInfo->dci.rnti;
      dlDciPtr->scramblingId = pdcchInfo->dci.scramblingId;
      dlDciPtr->scramblingRnti = pdcchInfo->dci.scramblingRnti;
      dlDciPtr->cceIndex = pdcchInfo->dci.cceIndex;
      dlDciPtr->aggregationLevel = pdcchInfo->dci.aggregLevel;
      dlDciPtr->pc_and_bform.numPrgs = pdcchInfo->dci.beamPdcchInfo.numPrgs;
      dlDciPtr->pc_and_bform.prgSize = pdcchInfo->dci.beamPdcchInfo.prgSize;
      dlDciPtr->pc_and_bform.digBfInterfaces = pdcchInfo->dci.beamPdcchInfo.digBfInterfaces;
      dlDciPtr->pc_and_bform.pmi_bfi[0].pmIdx = pdcchInfo->dci.beamPdcchInfo.prg[0].pmIdx;
      dlDciPtr->pc_and_bform.pmi_bfi[0].beamIdx[0].beamidx = pdcchInfo->dci.beamPdcchInfo.prg[0].beamIdx[0];
      dlDciPtr->beta_pdcch_1_0 = pdcchInfo->dci.txPdcchPower.powerValue;
      dlDciPtr->powerControlOffsetSS = pdcchInfo->dci.txPdcchPower.powerControlOffsetSS;

      /* Calculating freq domain resource allocation field value and size
       * coreset0Size = Size of coreset 0
       * RBStart = Starting Virtual Rsource block
       * RBLen = length of contiguously allocted RBs
       * Spec 38.214 Sec 5.1.2.2.2
       */
      coresetSize = pdcchInfo->coresetCfg.coreSetSize;
      rbStart = pdcchInfo->dci.pdschCfg->pdschFreqAlloc.freqAlloc.startPrb;
      rbLen = pdcchInfo->dci.pdschCfg->pdschFreqAlloc.freqAlloc.numPrb;

      if((rbLen >=1) && (rbLen <= coresetSize - rbStart))
      {
         if((rbLen - 1) <= floor(coresetSize / 2))
            freqDomResAssign = (coresetSize * (rbLen-1)) + rbStart;
         else
            freqDomResAssign = (coresetSize * (coresetSize - rbLen + 1)) \
                               + (coresetSize - 1 - rbStart);

         freqDomResAssignSize = ceil(log2(coresetSize * (coresetSize + 1) / 2));
      }

      /* Fetching DCI field values */
      dciFormatId      = dlMsgInfo->dciFormatId;     /* Always set to 1 for DL */
      timeDomResAssign = pdcchInfo->dci.pdschCfg->pdschTimeAlloc.rowIndex -1;
      VRB2PRBMap       = pdcchInfo->dci.pdschCfg->pdschFreqAlloc.vrbPrbMapping;
      modNCodScheme    = pdcchInfo->dci.pdschCfg->codeword[0].mcsIndex;
      ndi              = dlMsgInfo->ndi;
      redundancyVer    = pdcchInfo->dci.pdschCfg->codeword[0].rvIndex;
      harqProcessNum   = dlMsgInfo->harqProcNum;
      dlAssignmentIdx  = dlMsgInfo->dlAssignIdx;
      pucchTpc         = dlMsgInfo->pucchTpc;
      pucchResoInd     = dlMsgInfo->pucchResInd;
      harqFeedbackInd  = dlMsgInfo->harqFeedbackInd;

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

      if(numBytes > FAPI_DCI_PAYLOAD_BYTE_LEN)
      {
         DU_LOG("\nERROR  -->  LWR_MAC : Total bytes for DCI is more than expected");
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
}

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
 * @return ROK
 *
 ******************************************************************/
uint8_t fillPdcchPdu(fapi_dl_tti_req_pdu_t *dlTtiReqPdu, DlSchedInfo *dlInfo, \
      RntiType rntiType, uint8_t coreSetType)
{
   if(dlTtiReqPdu != NULLP)
   {
      PdcchCfg *pdcchInfo = NULLP;
      BwpCfg *bwp = NULLP;

      memset(&dlTtiReqPdu->pdu.pdcch_pdu, 0, sizeof(fapi_dl_pdcch_pdu_t));
      if(rntiType == SI_RNTI_TYPE)
      {
	 pdcchInfo = &dlInfo->brdcstAlloc.sib1Alloc.sib1PdcchCfg;
	 bwp = &dlInfo->brdcstAlloc.sib1Alloc.bwp;
	 fillSib1DlDciPdu(dlTtiReqPdu->pdu.pdcch_pdu.dlDci, pdcchInfo);
      }
      else if(rntiType == RA_RNTI_TYPE)
      {
	 pdcchInfo = &dlInfo->rarAlloc->rarPdcchCfg;
	 bwp = &dlInfo->rarAlloc->bwp;
	 fillRarDlDciPdu(dlTtiReqPdu->pdu.pdcch_pdu.dlDci, pdcchInfo);
      }
      else if(rntiType == TC_RNTI_TYPE || rntiType == C_RNTI_TYPE)
      {
         pdcchInfo = &dlInfo->dlMsgAlloc->dlMsgPdcchCfg;
	 bwp = &dlInfo->dlMsgAlloc->bwp;
         fillDlMsgDlDciPdu(dlTtiReqPdu->pdu.pdcch_pdu.dlDci, pdcchInfo,\
	    &dlInfo->dlMsgAlloc->dlMsgInfo);
      }
      else
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Failed filling PDCCH Pdu");
	 return RFAILED;
      }
      dlTtiReqPdu->pduType = PDCCH_PDU_TYPE;
      dlTtiReqPdu->pdu.pdcch_pdu.bwpSize = bwp->freqAlloc.numPrb;
      dlTtiReqPdu->pdu.pdcch_pdu.bwpStart = bwp->freqAlloc.startPrb;
      dlTtiReqPdu->pdu.pdcch_pdu.subCarrierSpacing = bwp->subcarrierSpacing; 
      dlTtiReqPdu->pdu.pdcch_pdu.cyclicPrefix = bwp->cyclicPrefix; 
      dlTtiReqPdu->pdu.pdcch_pdu.startSymbolIndex = pdcchInfo->coresetCfg.startSymbolIndex;
      dlTtiReqPdu->pdu.pdcch_pdu.durationSymbols = pdcchInfo->coresetCfg.durationSymbols;
      memcpy(dlTtiReqPdu->pdu.pdcch_pdu.freqDomainResource, pdcchInfo->coresetCfg.freqDomainResource, 6);
      dlTtiReqPdu->pdu.pdcch_pdu.cceRegMappingType = pdcchInfo->coresetCfg.cceRegMappingType;
      dlTtiReqPdu->pdu.pdcch_pdu.regBundleSize = pdcchInfo->coresetCfg.regBundleSize;
      dlTtiReqPdu->pdu.pdcch_pdu.interleaverSize = pdcchInfo->coresetCfg.interleaverSize;
      dlTtiReqPdu->pdu.pdcch_pdu.shiftIndex =  pdcchInfo->coresetCfg.shiftIndex;
      dlTtiReqPdu->pdu.pdcch_pdu.precoderGranularity = pdcchInfo->coresetCfg.precoderGranularity;
      dlTtiReqPdu->pdu.pdcch_pdu.numDlDci = pdcchInfo->numDlDci;
      dlTtiReqPdu->pdu.pdcch_pdu.coreSetType = coreSetType;
		     
      /* Calculating PDU length. Considering only one dl dci pdu for now */
      dlTtiReqPdu->pduSize = sizeof(fapi_dl_pdcch_pdu_t);
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
      BwpCfg bwp, uint16_t pduIndex)
{
   uint8_t idx;

   if(dlTtiReqPdu != NULLP)
   {
      dlTtiReqPdu->pduType = PDSCH_PDU_TYPE;
      memset(&dlTtiReqPdu->pdu.pdsch_pdu, 0, sizeof(fapi_dl_pdsch_pdu_t));
      dlTtiReqPdu->pdu.pdsch_pdu.pduBitMap = pdschInfo->pduBitmap;
      dlTtiReqPdu->pdu.pdsch_pdu.rnti = pdschInfo->rnti;         
      dlTtiReqPdu->pdu.pdsch_pdu.pdu_index = pduIndex;
      dlTtiReqPdu->pdu.pdsch_pdu.bwpSize = bwp.freqAlloc.numPrb;       
      dlTtiReqPdu->pdu.pdsch_pdu.bwpStart = bwp.freqAlloc.startPrb;
      dlTtiReqPdu->pdu.pdsch_pdu.subCarrierSpacing = bwp.subcarrierSpacing;
      dlTtiReqPdu->pdu.pdsch_pdu.cyclicPrefix = bwp.cyclicPrefix;
      dlTtiReqPdu->pdu.pdsch_pdu.nrOfCodeWords = pdschInfo->numCodewords;
      for(idx = 0; idx < MAX_CODEWORDS ; idx++)
      { 
	 dlTtiReqPdu->pdu.pdsch_pdu.cwInfo[idx].targetCodeRate = pdschInfo->codeword[idx].targetCodeRate;
	 dlTtiReqPdu->pdu.pdsch_pdu.cwInfo[idx].qamModOrder = pdschInfo->codeword[idx].qamModOrder;
	 dlTtiReqPdu->pdu.pdsch_pdu.cwInfo[idx].mcsIndex = pdschInfo->codeword[idx].mcsIndex;
	 dlTtiReqPdu->pdu.pdsch_pdu.cwInfo[idx].mcsTable = pdschInfo->codeword[idx].mcsTable;
	 dlTtiReqPdu->pdu.pdsch_pdu.cwInfo[idx].rvIndex = pdschInfo->codeword[idx].rvIndex;
	 dlTtiReqPdu->pdu.pdsch_pdu.cwInfo[idx].tbSize = pdschInfo->codeword[idx].tbSize;
      }
      dlTtiReqPdu->pdu.pdsch_pdu.dataScramblingId = pdschInfo->dataScramblingId;       
      dlTtiReqPdu->pdu.pdsch_pdu.nrOfLayers = pdschInfo->numLayers;
      dlTtiReqPdu->pdu.pdsch_pdu.transmissionScheme = pdschInfo->transmissionScheme;
      dlTtiReqPdu->pdu.pdsch_pdu.refPoint = pdschInfo->refPoint;
      dlTtiReqPdu->pdu.pdsch_pdu.dlDmrsSymbPos = pdschInfo->dmrs.dlDmrsSymbPos;
      dlTtiReqPdu->pdu.pdsch_pdu.dmrsConfigType = pdschInfo->dmrs.dmrsConfigType;
      dlTtiReqPdu->pdu.pdsch_pdu.dlDmrsScramblingId = pdschInfo->dmrs.dlDmrsScramblingId;
      dlTtiReqPdu->pdu.pdsch_pdu.scid = pdschInfo->dmrs.scid;
      dlTtiReqPdu->pdu.pdsch_pdu.numDmrsCdmGrpsNoData = pdschInfo->dmrs.numDmrsCdmGrpsNoData;
      dlTtiReqPdu->pdu.pdsch_pdu.dmrsPorts = pdschInfo->dmrs.dmrsPorts;
      dlTtiReqPdu->pdu.pdsch_pdu.resourceAlloc = pdschInfo->pdschFreqAlloc.resourceAllocType;
      /* since we are using type-1, hence rbBitmap excluded */
      dlTtiReqPdu->pdu.pdsch_pdu.rbStart = pdschInfo->pdschFreqAlloc.freqAlloc.startPrb;
      dlTtiReqPdu->pdu.pdsch_pdu.rbSize = pdschInfo->pdschFreqAlloc.freqAlloc.numPrb;
      dlTtiReqPdu->pdu.pdsch_pdu.vrbToPrbMapping = pdschInfo->pdschFreqAlloc.vrbPrbMapping;
      dlTtiReqPdu->pdu.pdsch_pdu.startSymbIndex = pdschInfo->pdschTimeAlloc.timeAlloc.startSymb;
      dlTtiReqPdu->pdu.pdsch_pdu.nrOfSymbols = pdschInfo->pdschTimeAlloc.timeAlloc.numSymb;
      dlTtiReqPdu->pdu.pdsch_pdu.preCodingAndBeamforming.numPrgs = pdschInfo->beamPdschInfo.numPrgs;
      dlTtiReqPdu->pdu.pdsch_pdu.preCodingAndBeamforming.prgSize = pdschInfo->beamPdschInfo.prgSize;
      dlTtiReqPdu->pdu.pdsch_pdu.preCodingAndBeamforming.digBfInterfaces = pdschInfo->beamPdschInfo.digBfInterfaces;
      dlTtiReqPdu->pdu.pdsch_pdu.preCodingAndBeamforming.pmi_bfi[0]. \
	 pmIdx = pdschInfo->beamPdschInfo.prg[0].pmIdx;
      dlTtiReqPdu->pdu.pdsch_pdu.preCodingAndBeamforming.pmi_bfi[0]. \
	 beamIdx[0].beamidx = pdschInfo->beamPdschInfo.prg[0].beamIdx[0];
      dlTtiReqPdu->pdu.pdsch_pdu.powerControlOffset = pdschInfo->txPdschPower.powerControlOffset;  
      dlTtiReqPdu->pdu.pdsch_pdu.powerControlOffsetSS = pdschInfo->txPdschPower.powerControlOffsetSS;
      dlTtiReqPdu->pdu.pdsch_pdu.mappingType =   pdschInfo->dmrs.mappingType;
      dlTtiReqPdu->pdu.pdsch_pdu.nrOfDmrsSymbols = pdschInfo->dmrs.nrOfDmrsSymbols;
      dlTtiReqPdu->pdu.pdsch_pdu.dmrsAddPos = pdschInfo->dmrs.dmrsAddPos;

      dlTtiReqPdu->pduSize = sizeof(fapi_dl_pdsch_pdu_t);
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
	    /* SSB PDU is filled */
	    count++;
	 }
      }
      if(dlInfo->brdcstAlloc.sib1Trans)
      {
	 /* PDCCH and PDSCH PDU is filled */
	 count += 2;
      }
   }
   if(dlInfo->rarAlloc != NULLP)
   {
      /* PDCCH and PDSCH PDU is filled */
      count += 2;
   }
   if(dlInfo->dlMsgAlloc != NULLP)
   {
      /* PDCCH and PDSCH PDU is filled */
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
   if(dlInfo->dlMsgAlloc != NULLP)
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
uint8_t fillSib1TxDataReq(fapi_tx_pdu_desc_t *pduDesc, uint16_t pduIndex, MacCellCfg *macCellCfg,
      PdschCfg pdschCfg)
{
   uint32_t payloadSize = 0;
   uint8_t *sib1Payload = NULLP;
   fapi_api_queue_elem_t *payloadElem = NULLP;
#ifdef INTEL_WLS_MEM
   void * wlsHdlr = NULLP;
#endif

   pduDesc[pduIndex].pdu_index = pduIndex;
   pduDesc[pduIndex].num_tlvs = 1;

   /* fill the TLV */
   payloadSize = pdschCfg.codeword[0].tbSize;
   pduDesc[pduIndex].tlvs[0].tl.tag = ((payloadSize & 0xff0000) >> 8) | FAPI_TX_DATA_PTR_TO_PAYLOAD_64;
   pduDesc[pduIndex].tlvs[0].tl.length = (payloadSize & 0x0000ffff);
   LWR_MAC_ALLOC(sib1Payload, payloadSize);
   if(sib1Payload == NULLP)
   {
      return RFAILED;
   }
   payloadElem = (fapi_api_queue_elem_t *)sib1Payload;
   FILL_FAPI_LIST_ELEM(payloadElem, NULLP, FAPI_VENDOR_MSG_PHY_ZBC_BLOCK_REQ, 1, \
      macCellCfg->sib1Cfg.sib1PduLen);
   memcpy(sib1Payload + TX_PAYLOAD_HDR_LEN, macCellCfg->sib1Cfg.sib1Pdu, macCellCfg->sib1Cfg.sib1PduLen);

#ifdef INTEL_WLS_MEM
   mtGetWlsHdl(&wlsHdlr);
   pduDesc[pduIndex].tlvs[0].value = WLS_VA2PA(wlsHdlr, sib1Payload);
#else
   pduDesc[pduIndex].tlvs[0].value = sib1Payload;
#endif
   pduDesc[pduIndex].pdu_length = payloadSize; 

#ifdef INTEL_WLS_MEM   
   addWlsBlockToFree(sib1Payload, payloadSize, (lwrMacCb.phySlotIndCntr-1));
#else
   LWR_MAC_FREE(sib1Payload, payloadSize);
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
uint8_t fillRarTxDataReq(fapi_tx_pdu_desc_t *pduDesc, uint16_t pduIndex, RarInfo *rarInfo, PdschCfg pdschCfg)
{
   uint16_t payloadSize;
   uint8_t  *rarPayload = NULLP;
   fapi_api_queue_elem_t *payloadElem = NULLP;
#ifdef INTEL_WLS_MEM
   void * wlsHdlr = NULLP;
#endif

   pduDesc[pduIndex].pdu_index = pduIndex;
   pduDesc[pduIndex].num_tlvs = 1;

   /* fill the TLV */
   payloadSize = pdschCfg.codeword[0].tbSize;
   pduDesc[pduIndex].tlvs[0].tl.tag = FAPI_TX_DATA_PTR_TO_PAYLOAD_64;
   pduDesc[pduIndex].tlvs[0].tl.length = payloadSize;
   LWR_MAC_ALLOC(rarPayload, payloadSize);
   if(rarPayload == NULLP)
   {
      return RFAILED;
   }
   payloadElem = (fapi_api_queue_elem_t *)rarPayload;
   FILL_FAPI_LIST_ELEM(payloadElem, NULLP, FAPI_VENDOR_MSG_PHY_ZBC_BLOCK_REQ, 1, rarInfo->rarPduLen);
   memcpy(rarPayload + TX_PAYLOAD_HDR_LEN, rarInfo->rarPdu, rarInfo->rarPduLen);

#ifdef INTEL_WLS_MEM
   mtGetWlsHdl(&wlsHdlr);
   pduDesc[pduIndex].tlvs[0].value = WLS_VA2PA(wlsHdlr, rarPayload);
#else
   pduDesc[pduIndex].tlvs[0].value = rarPayload;
#endif
   pduDesc[pduIndex].pdu_length = payloadSize;

#ifdef INTEL_WLS_MEM
   addWlsBlockToFree(rarPayload, payloadSize, (lwrMacCb.phySlotIndCntr-1));
#else
   LWR_MAC_FREE(rarPayload, payloadSize);
#endif
   return ROK;
}

/***********************************************************************
 *
 * @brief fills the DL dedicated Msg TX-DATA request message
 *
 * @details
 *
 *    Function : fillDlMsgTxDataReq
 *
 *    Functionality:
 *         - fills the Dl Dedicated Msg TX-DATA request message
 *
 * @params[in]    fapi_tx_pdu_desc_t *pduDesc
 * @params[in]    DlMsgInfo *dlMsgInfo
 * @params[in]    uint32_t *msgLen
 * @params[in]    uint16_t pduIndex
 * @return ROK
 *
 * ********************************************************************/
uint8_t fillDlMsgTxDataReq(fapi_tx_pdu_desc_t *pduDesc, uint16_t pduIndex, DlMsgInfo *dlMsgInfo, PdschCfg pdschCfg)
{
   uint16_t payloadSize;
   uint8_t  *dlMsgPayload = NULLP;
   fapi_api_queue_elem_t *payloadElem = NULLP;
#ifdef INTEL_WLS_MEM
   void * wlsHdlr = NULLP;
#endif

   pduDesc[pduIndex].pdu_index = pduIndex;
   pduDesc[pduIndex].num_tlvs = 1;

   /* fill the TLV */
   payloadSize = pdschCfg.codeword[0].tbSize;
   pduDesc[pduIndex].tlvs[0].tl.tag = FAPI_TX_DATA_PTR_TO_PAYLOAD_64;
   pduDesc[pduIndex].tlvs[0].tl.length = payloadSize;
   LWR_MAC_ALLOC(dlMsgPayload, payloadSize);
   if(dlMsgPayload == NULLP)
   {
      return RFAILED;
   }
   payloadElem = (fapi_api_queue_elem_t *)dlMsgPayload;
   FILL_FAPI_LIST_ELEM(payloadElem, NULLP, FAPI_VENDOR_MSG_PHY_ZBC_BLOCK_REQ, 1, dlMsgInfo->dlMsgPduLen);
   memcpy(dlMsgPayload + TX_PAYLOAD_HDR_LEN, dlMsgInfo->dlMsgPdu, dlMsgInfo->dlMsgPduLen);

#ifdef INTEL_WLS_MEM
   mtGetWlsHdl(&wlsHdlr);
   pduDesc[pduIndex].tlvs[0].value = WLS_VA2PA(wlsHdlr, dlMsgPayload);
#else
   pduDesc[pduIndex].tlvs[0].value = dlMsgPayload;
#endif
   pduDesc[pduIndex].pdu_length = payloadSize;

#ifdef INTEL_WLS_MEM
   addWlsBlockToFree(dlMsgPayload, payloadSize, (lwrMacCb.phySlotIndCntr-1));
#else
   LWR_MAC_FREE(dlMsgPayload, payloadSize);
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
 *    Function : fillDlTtiReq
 *
 *    Functionality:
 *         -Sends FAPI DL TTI req to PHY
 *
 * @params[in]    timing info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t fillDlTtiReq(SlotTimingInfo currTimingInfo)
{
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : DL_TTI_REQUEST\n");
#endif

#ifdef INTEL_FAPI
   uint8_t idx =0;
   uint8_t nPdu = 0;
   uint8_t numPduEncoded = 0;
   uint16_t cellIdx =0;
   uint16_t pduIndex = 0;

   SlotTimingInfo dlTtiReqTimingInfo;
   MacDlSlot *currDlSlot = NULLP;
   MacCellCfg macCellCfg;
   RntiType rntiType;
   fapi_dl_tti_req_t *dlTtiReq = NULLP;
   fapi_msg_header_t *msgHeader = NULLP;
   p_fapi_api_queue_elem_t dlTtiElem;
   p_fapi_api_queue_elem_t headerElem;
   p_fapi_api_queue_elem_t prevElem;

   if(lwrMacCb.phyState == PHY_STATE_RUNNING)
   {
      GET_CELL_IDX(currTimingInfo.cellId, cellIdx);
      /* consider phy delay */
      ADD_DELTA_TO_TIME(currTimingInfo,dlTtiReqTimingInfo,PHY_DELTA_DL);
      dlTtiReqTimingInfo.cellId = currTimingInfo.cellId;

      macCellCfg = macCb.macCell[cellIdx]->macCellCfg;

      currDlSlot = &macCb.macCell[cellIdx]->dlSlot[dlTtiReqTimingInfo.slot]; 

      LWR_MAC_ALLOC(dlTtiElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_dl_tti_req_t)));
      if(dlTtiElem)
      {
         FILL_FAPI_LIST_ELEM(dlTtiElem, NULLP, FAPI_DL_TTI_REQUEST, 1, \
               sizeof(fapi_dl_tti_req_t));

         /* Fill message header */
         LWR_MAC_ALLOC(headerElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_msg_header_t)));
         if(!headerElem)
         {
            DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed for header in DL TTI req");
            LWR_MAC_FREE(dlTtiElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_dl_tti_req_t)));
            return RFAILED;
         }
         FILL_FAPI_LIST_ELEM(headerElem, dlTtiElem, FAPI_VENDOR_MSG_HEADER_IND, 1, \
               sizeof(fapi_msg_header_t));
         msgHeader = (fapi_msg_header_t *)(headerElem + 1);
         msgHeader->num_msg = 1;
         msgHeader->handle = 0;

         /* Fill Dl TTI Request */
         dlTtiReq = (fapi_dl_tti_req_t *)(dlTtiElem +1);
         memset(dlTtiReq, 0, sizeof(fapi_dl_tti_req_t));
         fillMsgHeader(&dlTtiReq->header, FAPI_DL_TTI_REQUEST, sizeof(fapi_dl_tti_req_t));

         dlTtiReq->sfn  = dlTtiReqTimingInfo.sfn;
         dlTtiReq->slot = dlTtiReqTimingInfo.slot;
         dlTtiReq->nPdus = calcDlTtiReqPduCount(&currDlSlot->dlInfo);  /* get total Pdus */
         nPdu = dlTtiReq->nPdus;
         dlTtiReq->nGroup = 0;
         if(dlTtiReq->nPdus > 0)
         {
            if(currDlSlot->dlInfo.isBroadcastPres)
            {
               if(currDlSlot->dlInfo.brdcstAlloc.ssbTrans)
               {
                  if(dlTtiReq->pdus != NULLP)
                  {
                     for(idx = 0; idx < currDlSlot->dlInfo.brdcstAlloc.ssbIdxSupported; idx++)
                     {
                        fillSsbPdu(&dlTtiReq->pdus[numPduEncoded], &macCellCfg,\
                              currDlSlot, idx, dlTtiReq->sfn);
                        numPduEncoded++;
                     }
                  }
                  DU_LOG("\033[1;31m");
                  DU_LOG("\nDEBUG  -->  LWR_MAC: MIB sent..");
                  DU_LOG("\033[0m");
               }
               if(currDlSlot->dlInfo.brdcstAlloc.sib1Trans)
               {
                  /* Filling SIB1 param */
                  if(numPduEncoded != nPdu)
                  {
                     rntiType = SI_RNTI_TYPE;
                     fillPdcchPdu(&dlTtiReq->pdus[numPduEncoded],&currDlSlot->dlInfo,\
                           rntiType, CORESET_TYPE0);
                     numPduEncoded++;
                     fillPdschPdu(&dlTtiReq->pdus[numPduEncoded],
                           &currDlSlot->dlInfo.brdcstAlloc.sib1Alloc.sib1PdschCfg,
                           currDlSlot->dlInfo.brdcstAlloc.sib1Alloc.bwp,
                           pduIndex);
                     dlTtiReq->ue_grp_info[dlTtiReq->nGroup].pduIdx[pduIndex] = pduIndex;
                     pduIndex++;
                     numPduEncoded++;
                  }

                  DU_LOG("\033[1;34m");
                  DU_LOG("\nDEBUG  -->  LWR_MAC: SIB1 sent...");
                  DU_LOG("\033[0m");
               }
            }
            if(currDlSlot->dlInfo.rarAlloc != NULLP)
            {
               /* Filling RAR param */
               rntiType = RA_RNTI_TYPE;
               fillPdcchPdu(&dlTtiReq->pdus[numPduEncoded], \
                     &currDlSlot->dlInfo, rntiType, CORESET_TYPE0);
               numPduEncoded++;
               fillPdschPdu(&dlTtiReq->pdus[numPduEncoded],
                     &currDlSlot->dlInfo.rarAlloc->rarPdschCfg,
                     currDlSlot->dlInfo.rarAlloc->bwp,
                     pduIndex);
               numPduEncoded++;
               pduIndex++;

               DU_LOG("\033[1;32m");
               DU_LOG("\nDEBUG  -->  LWR_MAC: RAR sent...");
               DU_LOG("\033[0m");
            }
            if(currDlSlot->dlInfo.dlMsgAlloc != NULLP)
            {
               if(currDlSlot->dlInfo.dlMsgAlloc->dlMsgInfo.dlMsgPdu != NULLP)
               {
                  /* Filling Msg4 param */
                  DU_LOG("\033[1;32m");
                  if(currDlSlot->dlInfo.dlMsgAlloc->dlMsgInfo.isMsg4Pdu)
                  {
                     rntiType = TC_RNTI_TYPE;
                     fillPdcchPdu(&dlTtiReq->pdus[numPduEncoded], \
                           &currDlSlot->dlInfo, rntiType, CORESET_TYPE0);
                     DU_LOG("\nDEBUG  -->  LWR_MAC: MSG4 sent...");
                  }
                  else
                  { 
                     /* Filling other DL msg params */
                     rntiType = C_RNTI_TYPE;
                     fillPdcchPdu(&dlTtiReq->pdus[numPduEncoded], \
                           &currDlSlot->dlInfo, rntiType, CORESET_TYPE1);
                     DU_LOG("\nDEBUG  -->  LWR_MAC: DL MSG sent...");
                  }
                  DU_LOG("\033[0m");

                  numPduEncoded++;
                  fillPdschPdu(&dlTtiReq->pdus[numPduEncoded],
                        &currDlSlot->dlInfo.dlMsgAlloc->dlMsgPdschCfg,
                        currDlSlot->dlInfo.dlMsgAlloc->bwp,
                        pduIndex);
                  numPduEncoded++;
                  pduIndex++;
               }
               else
               {
                  MAC_FREE(currDlSlot->dlInfo.dlMsgAlloc, sizeof(DlMsgAlloc));
                  currDlSlot->dlInfo.dlMsgAlloc = NULLP;
               }
            }

            dlTtiReq->ue_grp_info[dlTtiReq->nGroup].nUe = MAX_NUM_UE_PER_TTI;
            dlTtiReq->nGroup++;

#ifdef ODU_SLOT_IND_DEBUG_LOG	    
            DU_LOG("\nDEBUG  -->  LWR_MAC: Sending DL TTI Request");
#endif	    

            /* Intel L1 expects UL_TTI.request following DL_TTI.request */
            fillUlTtiReq(currTimingInfo, dlTtiElem);
            msgHeader->num_msg++;

            /* Intel L1 expects UL_DCI.request following DL_TTI.request */
            fillUlDciReq(currTimingInfo, dlTtiElem->p_next);
            msgHeader->num_msg++;

            /* send Tx-DATA req message */
            sendTxDataReq(dlTtiReqTimingInfo, &currDlSlot->dlInfo, dlTtiElem->p_next->p_next);
            if(dlTtiElem->p_next->p_next->p_next)
            {
               msgHeader->num_msg++;
               prevElem = dlTtiElem->p_next->p_next->p_next;
            }
            else
               prevElem = dlTtiElem->p_next->p_next;
         }
         else
         {
#ifdef ODU_SLOT_IND_DEBUG_LOG	    
            DU_LOG("\nDEBUG  -->  LWR_MAC: Sending DL TTI Request");
#endif	    

            /* Intel L1 expects UL_TTI.request following DL_TTI.request */
            fillUlTtiReq(currTimingInfo, dlTtiElem);
            msgHeader->num_msg++;

            /* Intel L1 expects UL_DCI.request following DL_TTI.request */
            fillUlDciReq(currTimingInfo, dlTtiElem->p_next);
            msgHeader->num_msg++;

            prevElem = dlTtiElem->p_next->p_next;
         }

         if(macCb.macCell[cellIdx]->state == CELL_TO_BE_STOPPED)
         {
            /* Intel L1 expects UL_DCI.request following DL_TTI.request */
            lwr_mac_procStopReqEvt(currTimingInfo, prevElem);
            msgHeader->num_msg++;
            macCb.macCell[cellIdx]->state = CELL_STOP_IN_PROGRESS;
         }
         LwrMacSendToL1(headerElem);
         memset(currDlSlot, 0, sizeof(MacDlSlot));
         return ROK;
      }
      else
      {
         DU_LOG("\nERROR  -->  LWR_MAC: Failed to allocate memory for DL TTI Request");
         memset(currDlSlot, 0, sizeof(MacDlSlot));
         return RFAILED;
      }
   }
   else
   {
      lwr_mac_procInvalidEvt(&currTimingInfo);
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
uint16_t sendTxDataReq(SlotTimingInfo currTimingInfo, DlSchedInfo *dlInfo, p_fapi_api_queue_elem_t prevElem)
{
#ifdef INTEL_FAPI
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : TX_DATA_REQ\n");
#endif

   uint8_t nPdu = 0;
   uint16_t cellIdx;
   uint16_t pduIndex = 0;
   fapi_tx_data_req_t       *txDataReq =NULLP;
   p_fapi_api_queue_elem_t  txDataElem = 0;

   GET_CELL_IDX(currTimingInfo.cellId, cellIdx);

   /* send TX_Data request message */
   nPdu = calcTxDataReqPduCount(dlInfo);
   if(nPdu > 0)
   {
      LWR_MAC_ALLOC(txDataElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_tx_data_req_t)));
      if(txDataElem == NULLP)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Failed to allocate memory for TX data Request");
	 return RFAILED;
      }

      FILL_FAPI_LIST_ELEM(txDataElem, NULLP, FAPI_TX_DATA_REQUEST, 1, \
          sizeof(fapi_tx_data_req_t));
      txDataReq = (fapi_tx_data_req_t *)(txDataElem +1);
      memset(txDataReq, 0, sizeof(fapi_tx_data_req_t));
      fillMsgHeader(&txDataReq->header, FAPI_TX_DATA_REQUEST, sizeof(fapi_tx_data_req_t));

      txDataReq->sfn  = currTimingInfo.sfn;
      txDataReq->slot = currTimingInfo.slot;
      if(dlInfo->brdcstAlloc.sib1Trans)
      {
	 fillSib1TxDataReq(txDataReq->pdu_desc, pduIndex, &macCb.macCell[cellIdx]->macCellCfg, \
	    dlInfo->brdcstAlloc.sib1Alloc.sib1PdschCfg);
	 pduIndex++;
	 txDataReq->num_pdus++;
      }
      if(dlInfo->rarAlloc != NULLP)
      {
	 fillRarTxDataReq(txDataReq->pdu_desc, pduIndex, &dlInfo->rarAlloc->rarInfo,\
	    dlInfo->rarAlloc->rarPdschCfg);
	 pduIndex++;
	 txDataReq->num_pdus++;

	 MAC_FREE(dlInfo->rarAlloc,sizeof(RarAlloc));
	 dlInfo->rarAlloc = NULLP;
      }
      if(dlInfo->dlMsgAlloc != NULLP)
      {
         fillDlMsgTxDataReq(txDataReq->pdu_desc, pduIndex, &dlInfo->dlMsgAlloc->dlMsgInfo,\
	    dlInfo->dlMsgAlloc->dlMsgPdschCfg);
         pduIndex++;
         txDataReq->num_pdus++;

         MAC_FREE(dlInfo->dlMsgAlloc->dlMsgInfo.dlMsgPdu,\
            dlInfo->dlMsgAlloc->dlMsgInfo.dlMsgPduLen);
         dlInfo->dlMsgAlloc->dlMsgInfo.dlMsgPdu = NULLP;
         MAC_FREE(dlInfo->dlMsgAlloc, sizeof(DlMsgAlloc));
         dlInfo->dlMsgAlloc = NULLP;
      }

      /* Fill message header */
      DU_LOG("\nDEBUG  -->  LWR_MAC: Sending TX DATA Request");
      prevElem->p_next = txDataElem;
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
#ifdef INTEL_FAPI
uint8_t getnPdus(fapi_ul_tti_req_t *ulTtiReq, MacUlSlot *currUlSlot)
{
   uint8_t pduCount = 0;

   if(ulTtiReq && currUlSlot)
   {
      if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_PRACH)
      {
	 pduCount++;
	 ulTtiReq->rachPresent++;
      }
      if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_PUSCH)
      {
	 pduCount++;
	 ulTtiReq->nUlsch++;
      }
      if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_PUSCH_UCI)
      {
	 pduCount++;
	 ulTtiReq->nUlsch++;
      }
      if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_UCI)
      {
	 pduCount++;
	 ulTtiReq->nUlcch++;
      }
      if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_SRS)
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

void setNumCs(uint16_t *numCs, MacCellCfg *macCellCfg)
{
#ifdef INTEL_FAPI
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

#ifdef INTEL_FAPI
void fillPrachPdu(fapi_ul_tti_req_pdu_t *ulTtiReqPdu, MacCellCfg *macCellCfg, MacUlSlot *currUlSlot)
{
   if(ulTtiReqPdu != NULLP)
   {
      ulTtiReqPdu->pduType = PRACH_PDU_TYPE; 
      ulTtiReqPdu->pdu.prach_pdu.physCellId = macCellCfg->phyCellId;
      ulTtiReqPdu->pdu.prach_pdu.numPrachOcas = \
         currUlSlot->ulInfo.prachSchInfo.numPrachOcas;
      ulTtiReqPdu->pdu.prach_pdu.prachFormat = \
	 currUlSlot->ulInfo.prachSchInfo.prachFormat;
      ulTtiReqPdu->pdu.prach_pdu.numRa = currUlSlot->ulInfo.prachSchInfo.numRa;
      ulTtiReqPdu->pdu.prach_pdu.prachStartSymbol = \
	 currUlSlot->ulInfo.prachSchInfo.prachStartSymb;
      setNumCs(&ulTtiReqPdu->pdu.prach_pdu.numCs, macCellCfg);
      ulTtiReqPdu->pdu.prach_pdu.beamforming.numPrgs = 0;
      ulTtiReqPdu->pdu.prach_pdu.beamforming.prgSize = 0;
      ulTtiReqPdu->pdu.prach_pdu.beamforming.digBfInterface = 0;
      ulTtiReqPdu->pdu.prach_pdu.beamforming.rx_bfi[0].beamIdx[0].beamidx = 0;
      ulTtiReqPdu->pduSize = sizeof(fapi_ul_prach_pdu_t); 
   }
}

/*******************************************************************
 *
 * @brief Filling PUSCH PDU in UL TTI Request
 *
 * @details
 *
 *    Function : fillPuschPdu
 *
 *    Functionality: Filling PUSCH PDU in UL TTI Request
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillPuschPdu(fapi_ul_tti_req_pdu_t *ulTtiReqPdu, MacCellCfg *macCellCfg, MacUlSlot *currUlSlot)
{
   if(ulTtiReqPdu != NULLP)
   {
      ulTtiReqPdu->pduType = PUSCH_PDU_TYPE;
      memset(&ulTtiReqPdu->pdu.pusch_pdu, 0, sizeof(fapi_ul_pusch_pdu_t));
      ulTtiReqPdu->pdu.pusch_pdu.pduBitMap = 1;
      ulTtiReqPdu->pdu.pusch_pdu.rnti = currUlSlot->ulInfo.crnti;
      /* TODO : Fill handle in raCb when scheduling pusch and access here */
      ulTtiReqPdu->pdu.pusch_pdu.handle = 100;
      ulTtiReqPdu->pdu.pusch_pdu.bwpSize = macCellCfg->initialUlBwp.bwp.numPrb;
      ulTtiReqPdu->pdu.pusch_pdu.bwpStart = macCellCfg->initialUlBwp.bwp.firstPrb;
      ulTtiReqPdu->pdu.pusch_pdu.subCarrierSpacing = \
         macCellCfg->initialUlBwp.bwp.scs;
      ulTtiReqPdu->pdu.pusch_pdu.cyclicPrefix = \
         macCellCfg->initialUlBwp.bwp.cyclicPrefix;
      ulTtiReqPdu->pdu.pusch_pdu.targetCodeRate = 308;
      ulTtiReqPdu->pdu.pusch_pdu.qamModOrder = currUlSlot->ulInfo.schPuschInfo.tbInfo.qamOrder;
      ulTtiReqPdu->pdu.pusch_pdu.mcsIndex = currUlSlot->ulInfo.schPuschInfo.tbInfo.mcs;
      ulTtiReqPdu->pdu.pusch_pdu.mcsTable = currUlSlot->ulInfo.schPuschInfo.tbInfo.mcsTable;
      ulTtiReqPdu->pdu.pusch_pdu.transformPrecoding = 1;
      ulTtiReqPdu->pdu.pusch_pdu.dataScramblingId = currUlSlot->ulInfo.cellId;
      ulTtiReqPdu->pdu.pusch_pdu.nrOfLayers = 1;
      ulTtiReqPdu->pdu.pusch_pdu.ulDmrsSymbPos = 4;
      ulTtiReqPdu->pdu.pusch_pdu.dmrsConfigType = 0;
      ulTtiReqPdu->pdu.pusch_pdu.ulDmrsScramblingId = currUlSlot->ulInfo.cellId;
      ulTtiReqPdu->pdu.pusch_pdu.scid = 0;
      ulTtiReqPdu->pdu.pusch_pdu.numDmrsCdmGrpsNoData = 1;
      ulTtiReqPdu->pdu.pusch_pdu.dmrsPorts = 0;
      ulTtiReqPdu->pdu.pusch_pdu.resourceAlloc = \
	 currUlSlot->ulInfo.schPuschInfo.resAllocType;
      ulTtiReqPdu->pdu.pusch_pdu.rbStart = \
         currUlSlot->ulInfo.schPuschInfo.fdAlloc.startPrb;
      ulTtiReqPdu->pdu.pusch_pdu.rbSize = \
	 currUlSlot->ulInfo.schPuschInfo.fdAlloc.numPrb;
      ulTtiReqPdu->pdu.pusch_pdu.vrbToPrbMapping = 0;
      ulTtiReqPdu->pdu.pusch_pdu.frequencyHopping = 0;
      ulTtiReqPdu->pdu.pusch_pdu.txDirectCurrentLocation = 0;
      ulTtiReqPdu->pdu.pusch_pdu.uplinkFrequencyShift7p5khz = 0;
      ulTtiReqPdu->pdu.pusch_pdu.startSymbIndex = \
         currUlSlot->ulInfo.schPuschInfo.tdAlloc.startSymb;
      ulTtiReqPdu->pdu.pusch_pdu.nrOfSymbols = \
         currUlSlot->ulInfo.schPuschInfo.tdAlloc.numSymb;
      ulTtiReqPdu->pdu.pusch_pdu.mappingType = \
         currUlSlot->ulInfo.schPuschInfo.dmrsMappingType;
      ulTtiReqPdu->pdu.pusch_pdu.nrOfDmrsSymbols = \
         currUlSlot->ulInfo.schPuschInfo.nrOfDmrsSymbols;
      ulTtiReqPdu->pdu.pusch_pdu.dmrsAddPos = \
         currUlSlot->ulInfo.schPuschInfo.dmrsAddPos;
      ulTtiReqPdu->pdu.pusch_pdu.puschData.rvIndex = \
         currUlSlot->ulInfo.schPuschInfo.tbInfo.rv;
      ulTtiReqPdu->pdu.pusch_pdu.puschData.harqProcessId = \
         currUlSlot->ulInfo.schPuschInfo.harqProcId;
      ulTtiReqPdu->pdu.pusch_pdu.puschData.newDataIndicator = \
         currUlSlot->ulInfo.schPuschInfo.tbInfo.ndi;
      ulTtiReqPdu->pdu.pusch_pdu.puschData.tbSize = \
         currUlSlot->ulInfo.schPuschInfo.tbInfo.tbSize;
      /* numCb is 0 for new transmission */
      ulTtiReqPdu->pdu.pusch_pdu.puschData.numCb = 0;

      ulTtiReqPdu->pduSize = sizeof(fapi_ul_pusch_pdu_t);
   }
}

/*******************************************************************
 *
 * @brief Fill PUCCH PDU in Ul TTI Request
 *
 * @details
 *
 *    Function : fillPucchPdu
 *
 *    Functionality: Fill PUCCH PDU in Ul TTI Request
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillPucchPdu(fapi_ul_tti_req_pdu_t *ulTtiReqPdu, MacCellCfg *macCellCfg,\
      MacUlSlot *currUlSlot)
{
   if(ulTtiReqPdu != NULLP)
   {
      ulTtiReqPdu->pduType                  = PUCCH_PDU_TYPE;
      memset(&ulTtiReqPdu->pdu.pucch_pdu, 0, sizeof(fapi_ul_pucch_pdu_t));
      ulTtiReqPdu->pdu.pucch_pdu.rnti         = currUlSlot->ulInfo.schPucchInfo.rnti;
      /* TODO : Fill handle in raCb when scheduling pucch and access here */
      ulTtiReqPdu->pdu.pucch_pdu.handle       = 100;
      ulTtiReqPdu->pdu.pucch_pdu.bwpSize      = macCellCfg->initialUlBwp.bwp.numPrb;
      ulTtiReqPdu->pdu.pucch_pdu.bwpStart     = macCellCfg->initialUlBwp.bwp.firstPrb;
      ulTtiReqPdu->pdu.pucch_pdu.subCarrierSpacing = macCellCfg->initialUlBwp.bwp.scs;
      ulTtiReqPdu->pdu.pucch_pdu.cyclicPrefix = macCellCfg->initialUlBwp.bwp.cyclicPrefix;
      ulTtiReqPdu->pdu.pucch_pdu.formatType   = currUlSlot->ulInfo.schPucchInfo.pucchFormat; /* Supporting PUCCH Format 0 */
      ulTtiReqPdu->pdu.pucch_pdu.multiSlotTxIndicator = 0; /* No Multi Slot transmission */
      
      ulTtiReqPdu->pdu.pucch_pdu.prbStart     = currUlSlot->ulInfo.schPucchInfo.fdAlloc.startPrb;
      ulTtiReqPdu->pdu.pucch_pdu.prbSize      = currUlSlot->ulInfo.schPucchInfo.fdAlloc.numPrb;
      ulTtiReqPdu->pdu.pucch_pdu.startSymbolIndex = currUlSlot->ulInfo.schPucchInfo.tdAlloc.startSymb;
      ulTtiReqPdu->pdu.pucch_pdu.nrOfSymbols  = currUlSlot->ulInfo.schPucchInfo.tdAlloc.numSymb;
      ulTtiReqPdu->pdu.pucch_pdu.freqHopFlag  = currUlSlot->ulInfo.schPucchInfo.intraFreqHop;
      ulTtiReqPdu->pdu.pucch_pdu.secondHopPrb = currUlSlot->ulInfo.schPucchInfo.secondPrbHop;
      ulTtiReqPdu->pdu.pucch_pdu.groupHopFlag = 0;     
      ulTtiReqPdu->pdu.pucch_pdu.sequenceHopFlag = 0;
      ulTtiReqPdu->pdu.pucch_pdu.hoppingId    = 0;

      ulTtiReqPdu->pdu.pucch_pdu.initialCyclicShift = currUlSlot->ulInfo.schPucchInfo.initialCyclicShift;

      ulTtiReqPdu->pdu.pucch_pdu.dataScramblingId = 0; /* Valid for Format 2, 3, 4 */
      ulTtiReqPdu->pdu.pucch_pdu.timeDomainOccIdx = currUlSlot->ulInfo.schPucchInfo.timeDomOCC; 
      ulTtiReqPdu->pdu.pucch_pdu.preDftOccIdx = currUlSlot->ulInfo.schPucchInfo.occIdx; /* Valid for Format 4 only */
      ulTtiReqPdu->pdu.pucch_pdu.preDftOccLen = currUlSlot->ulInfo.schPucchInfo.occLen; /* Valid for Format 4 only */
      ulTtiReqPdu->pdu.pucch_pdu.pi2Bpsk = currUlSlot->ulInfo.schPucchInfo.cmnFormatCfg.pi2BPSK;
      ulTtiReqPdu->pdu.pucch_pdu.addDmrsFlag = currUlSlot->ulInfo.schPucchInfo.cmnFormatCfg.addDmrs;/* Valid for Format 3, 4 only */
      ulTtiReqPdu->pdu.pucch_pdu.dmrsScramblingId = 0; /* Valid for Format 2 */
      ulTtiReqPdu->pdu.pucch_pdu.dmrsCyclicShift  = 0; /* Valid for Format 4 */
      ulTtiReqPdu->pdu.pucch_pdu.srFlag           = currUlSlot->ulInfo.schPucchInfo.srFlag;
      ulTtiReqPdu->pdu.pucch_pdu.bitLenHarq       = currUlSlot->ulInfo.schPucchInfo.numHarqBits;
      ulTtiReqPdu->pdu.pucch_pdu.bitLenCsiPart1   = 0; /* Valid for Format 2, 3, 4 */
      ulTtiReqPdu->pdu.pucch_pdu.bitLenCsiPart2   = 0; /* Valid for Format 2, 3, 4 */
      ulTtiReqPdu->pdu.pucch_pdu.beamforming.numPrgs = 0; /* Not Supported */
      ulTtiReqPdu->pdu.pucch_pdu.beamforming.prgSize = 0;
      ulTtiReqPdu->pdu.pucch_pdu.beamforming.digBfInterface = 0;
      ulTtiReqPdu->pdu.pucch_pdu.beamforming.rx_bfi[0].beamIdx[0].beamidx = 0;

      ulTtiReqPdu->pduSize = sizeof(fapi_ul_pucch_pdu_t);
   }
}

#endif

/*******************************************************************
 *
 * @brief Sends UL TTI Request to PHY
 *
 * @details
 *
 *    Function : fillUlTtiReq
 *
 *    Functionality:
 *         -Sends FAPI Param req to PHY
 *
 * @params[in]  Pointer to CmLteTimingInfo
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint16_t fillUlTtiReq(SlotTimingInfo currTimingInfo, p_fapi_api_queue_elem_t prevElem)
{
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : UL_TTI_REQUEST\n");
#endif

#ifdef INTEL_FAPI
   uint16_t   cellIdx =0;
   uint8_t    pduIdx = -1;
   SlotTimingInfo ulTtiReqTimingInfo;
   MacUlSlot *currUlSlot = NULLP;
   MacCellCfg macCellCfg;
   fapi_ul_tti_req_t *ulTtiReq = NULLP;
   p_fapi_api_queue_elem_t ulTtiElem;

   if(lwrMacCb.phyState == PHY_STATE_RUNNING)
   {
      GET_CELL_IDX(currTimingInfo.cellId, cellIdx);
      macCellCfg = macCb.macCell[cellIdx]->macCellCfg;

      /* add PHY delta */
      ADD_DELTA_TO_TIME(currTimingInfo,ulTtiReqTimingInfo,PHY_DELTA_UL);
      currUlSlot = &macCb.macCell[cellIdx]->ulSlot[ulTtiReqTimingInfo.slot % MAX_SLOTS];

      LWR_MAC_ALLOC(ulTtiElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_ul_tti_req_t)));
      if(ulTtiElem)
      {
         FILL_FAPI_LIST_ELEM(ulTtiElem, NULLP, FAPI_UL_TTI_REQUEST, 1, \
	    sizeof(fapi_ul_tti_req_t));
	 ulTtiReq = (fapi_ul_tti_req_t *)(ulTtiElem +1);
	 memset(ulTtiReq, 0, sizeof(fapi_ul_tti_req_t));
	 fillMsgHeader(&ulTtiReq->header, FAPI_UL_TTI_REQUEST, sizeof(fapi_ul_tti_req_t));
	 ulTtiReq->sfn  = ulTtiReqTimingInfo.sfn;
	 ulTtiReq->slot = ulTtiReqTimingInfo.slot;
	 ulTtiReq->nPdus = getnPdus(ulTtiReq, currUlSlot);
	 ulTtiReq->nGroup = 0;
	 if(ulTtiReq->nPdus > 0)
	 {
	    /* Fill Prach Pdu */
	    if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_PRACH)
	    {
	       pduIdx++;
	       fillPrachPdu(&ulTtiReq->pdus[pduIdx], &macCellCfg, currUlSlot);
	    }

	    /* Fill PUSCH PDU */
	    if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_PUSCH)
	    {
	       pduIdx++;
	       fillPuschPdu(&ulTtiReq->pdus[pduIdx], &macCellCfg, currUlSlot);
	    }
	    /* Fill PUCCH PDU */
	    if(currUlSlot->ulInfo.dataType & SCH_DATATYPE_UCI)
	    {
	       pduIdx++;
	       fillPucchPdu(&ulTtiReq->pdus[pduIdx], &macCellCfg, currUlSlot);
	    }
	 } 

#ifdef ODU_SLOT_IND_DEBUG_LOG
         DU_LOG("\nDEBUG  -->  LWR_MAC: Sending UL TTI Request");
#endif
	 prevElem->p_next = ulTtiElem;

	 memset(currUlSlot, 0, sizeof(MacUlSlot));
	 return ROK;
      }
      else
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Failed to allocate memory for UL TTI Request");
	 memset(currUlSlot, 0, sizeof(MacUlSlot));
	 return RFAILED;
      }
   }
   else
   {
      lwr_mac_procInvalidEvt(&currTimingInfo);
   }
#endif
   return ROK;
}

#ifdef INTEL_FAPI
/*******************************************************************
 *
 * @brief fills bsr Ul DCI PDU required for UL DCI Request to PHY
 *
 * @details
 *
 *    Function : fillUlDciPdu
 *
 *    Functionality:
 *         -Fills the Ul DCI PDU, spec Ref:38.212, Table 7.3.1-1
 *
 * @params[in] Pointer to fapi_dl_dci_t
 *             Pointer to DciInfo
 * @return ROK
 *
 ******************************************************************/
void fillUlDciPdu(fapi_dl_dci_t *ulDciPtr, DciInfo *schDciInfo)
{
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTLWRMAC : UL_DCI_REQUEST\n");
#endif
   if(ulDciPtr != NULLP)
   {
      uint8_t numBytes =0;
      uint8_t bytePos =0;
      uint8_t bitPos =0;

      uint8_t  coreset1Size = 0;
      uint16_t rbStart = 0;
      uint16_t rbLen = 0;
      uint8_t  dciFormatId = 0;
      uint32_t freqDomResAssign =0;
      uint8_t  timeDomResAssign =0;
      uint8_t  freqHopFlag =0;
      uint8_t  modNCodScheme =0;
      uint8_t  ndi =0;
      uint8_t  redundancyVer = 0;
      uint8_t  harqProcessNum = 0;
      uint8_t  puschTpc = 0;
      uint8_t  ul_SlInd = 0;

      /* Size(in bits) of each field in DCI format 0_0 */
      uint8_t dciFormatIdSize      = 1;
      uint8_t freqDomResAssignSize = 0;
      uint8_t timeDomResAssignSize = 4;
      uint8_t freqHopFlagSize      = 1;
      uint8_t modNCodSchemeSize    = 5;
      uint8_t ndiSize              = 1;
      uint8_t redundancyVerSize    = 2;
      uint8_t harqProcessNumSize   = 4;
      uint8_t puschTpcSize         = 2;
      uint8_t ul_SlIndSize         = 1;

      ulDciPtr->rnti                          = schDciInfo->dciInfo.rnti;
      ulDciPtr->scramblingId                  = schDciInfo->dciInfo.scramblingId;    
      ulDciPtr->scramblingRnti                = schDciInfo->dciInfo.scramblingRnti;
      ulDciPtr->cceIndex                      = schDciInfo->dciInfo.cceIndex;
      ulDciPtr->aggregationLevel              = schDciInfo->dciInfo.aggregLevel;
      ulDciPtr->pc_and_bform.numPrgs          = schDciInfo->dciInfo.beamPdcchInfo.numPrgs;
      ulDciPtr->pc_and_bform.prgSize          = schDciInfo->dciInfo.beamPdcchInfo.prgSize;
      ulDciPtr->pc_and_bform.digBfInterfaces  = schDciInfo->dciInfo.beamPdcchInfo.digBfInterfaces;
      ulDciPtr->pc_and_bform.pmi_bfi[0].pmIdx = schDciInfo->dciInfo.beamPdcchInfo.prg[0].pmIdx;
      ulDciPtr->pc_and_bform.pmi_bfi[0].beamIdx[0].beamidx = schDciInfo->dciInfo.beamPdcchInfo.prg[0].beamIdx[0];
      ulDciPtr->beta_pdcch_1_0                = schDciInfo->dciInfo.txPdcchPower.powerValue;           
      ulDciPtr->powerControlOffsetSS          = schDciInfo->dciInfo.txPdcchPower.powerControlOffsetSS;

      /* Calculating freq domain resource allocation field value and size
       * coreset1Size = Size of coreset 1
       * RBStart = Starting Virtual Rsource block
       * RBLen = length of contiguously allocted RBs
       * Spec 38.214 Sec 5.1.2.2.2
       */
      if(schDciInfo->formatType == FORMAT0_0)
      {
         coreset1Size = schDciInfo->coresetCfg.coreSetSize;
         rbLen = schDciInfo->format.format0_0.freqAlloc.numPrb;
         rbStart = schDciInfo->format.format0_0.freqAlloc.startPrb;

         if((rbLen >=1) && (rbLen <= coreset1Size - rbStart))
         {
            if((rbLen - 1) <= floor(coreset1Size / 2))
               freqDomResAssign = (coreset1Size * (rbLen-1)) + rbStart;
            else
               freqDomResAssign = (coreset1Size * (coreset1Size - rbLen + 1)) \
                                  + (coreset1Size - 1 - rbStart);

            freqDomResAssignSize = ceil(log2(coreset1Size * (coreset1Size + 1) / 2));
         }
         /* Fetching DCI field values */
         dciFormatId      = schDciInfo->formatType; /* DCI indentifier for UL DCI */
         timeDomResAssign = schDciInfo->format.format0_0.rowIndex;
         freqHopFlag      = schDciInfo->format.format0_0.freqHopFlag; 
         modNCodScheme    = schDciInfo->format.format0_0.mcs;
         ndi              = schDciInfo->format.format0_0.ndi; 
         redundancyVer    = schDciInfo->format.format0_0.rv;
         harqProcessNum   = schDciInfo->format.format0_0.harqProcId; 
         puschTpc         = schDciInfo->format.format0_0.tpcCmd;
         ul_SlInd         = schDciInfo->format.format0_0.sUlCfgd;
     
         /* Reversing bits in each DCI field */
         dciFormatId      = reverseBits(dciFormatId, dciFormatIdSize);
         freqDomResAssign = reverseBits(freqDomResAssign, freqDomResAssignSize);
         timeDomResAssign = reverseBits(timeDomResAssign, timeDomResAssignSize);
         modNCodScheme    = reverseBits(modNCodScheme, modNCodSchemeSize);
         redundancyVer    = reverseBits(redundancyVer, redundancyVerSize);
         harqProcessNum   = reverseBits(harqProcessNum, harqProcessNumSize);
         puschTpc         = reverseBits(puschTpc, puschTpcSize);
         ul_SlInd         = reverseBits(ul_SlInd, ul_SlIndSize);
      }
      /* Calulating total number of bytes in buffer */
      ulDciPtr->payloadSizeBits = (dciFormatIdSize + freqDomResAssignSize\
      + timeDomResAssignSize + freqHopFlagSize + modNCodSchemeSize + ndi \
      + redundancyVerSize + harqProcessNumSize + puschTpcSize + ul_SlIndSize);

      numBytes = ulDciPtr->payloadSizeBits / 8;
      if(ulDciPtr->payloadSizeBits % 8)
         numBytes += 1;

      if(numBytes > FAPI_DCI_PAYLOAD_BYTE_LEN)
      {
         DU_LOG("\nERROR  -->  LWR_MAC : Total bytes for DCI is more than expected");
         return;
      }

      /* Initialize buffer */
      for(bytePos = 0; bytePos < numBytes; bytePos++)
         ulDciPtr->payload[bytePos] = 0;

      bytePos = numBytes - 1;
      bitPos = 0;

      /* Packing DCI format fields */
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            dciFormatId, dciFormatIdSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            freqDomResAssign, freqDomResAssignSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            timeDomResAssign, timeDomResAssignSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            freqHopFlag, freqHopFlagSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            modNCodScheme, modNCodSchemeSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            ndi, ndiSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            redundancyVer, redundancyVerSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            harqProcessNum, harqProcessNumSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            puschTpc, puschTpcSize);
      fillDlDciPayload(ulDciPtr->payload, &bytePos, &bitPos,\
            ul_SlInd, ul_SlIndSize);
   }
} /* fillUlDciPdu */

/*******************************************************************
 *
 * @brief fills PDCCH PDU required for UL DCI REQ to PHY
 *
 * @details
 *
 *    Function : fillUlDciPdcchPdu
 *
 *    Functionality:
 *         -Fills the Pdcch PDU info
 *
 * @params[in] Pointer to FAPI DL TTI Req
 *             Pointer to PdcchCfg
 * @return ROK
 *
 ******************************************************************/
uint8_t fillUlDciPdcchPdu(fapi_dci_pdu_t *ulDciReqPdu, DlSchedInfo *dlInfo, uint8_t coreSetType)
{
   if(ulDciReqPdu != NULLP)
   {
      memset(&ulDciReqPdu->pdcchPduConfig, 0, sizeof(fapi_dl_pdcch_pdu_t));
      fillUlDciPdu(ulDciReqPdu->pdcchPduConfig.dlDci, dlInfo->ulGrant);
      ulDciReqPdu->pduType                          = PDCCH_PDU_TYPE;
      ulDciReqPdu->pdcchPduConfig.bwpSize           = dlInfo->ulGrant->bwpCfg.freqAlloc.numPrb;
      ulDciReqPdu->pdcchPduConfig.bwpStart          = dlInfo->ulGrant->bwpCfg.freqAlloc.startPrb;
      ulDciReqPdu->pdcchPduConfig.subCarrierSpacing = dlInfo->ulGrant->bwpCfg.subcarrierSpacing; 
      ulDciReqPdu->pdcchPduConfig.cyclicPrefix      = dlInfo->ulGrant->bwpCfg.cyclicPrefix; 
      ulDciReqPdu->pdcchPduConfig.startSymbolIndex  = dlInfo->ulGrant->coresetCfg.startSymbolIndex;
      ulDciReqPdu->pdcchPduConfig.durationSymbols   = dlInfo->ulGrant->coresetCfg.durationSymbols;
      memcpy(ulDciReqPdu->pdcchPduConfig.freqDomainResource, dlInfo->ulGrant->coresetCfg.freqDomainResource, 6);
      ulDciReqPdu->pdcchPduConfig.cceRegMappingType = dlInfo->ulGrant->coresetCfg.cceRegMappingType;
      ulDciReqPdu->pdcchPduConfig.regBundleSize     = dlInfo->ulGrant->coresetCfg.regBundleSize;
      ulDciReqPdu->pdcchPduConfig.interleaverSize   = dlInfo->ulGrant->coresetCfg.interleaverSize;
      ulDciReqPdu->pdcchPduConfig.shiftIndex        = dlInfo->ulGrant->coresetCfg.shiftIndex;
      ulDciReqPdu->pdcchPduConfig.precoderGranularity = dlInfo->ulGrant->coresetCfg.precoderGranularity;
      ulDciReqPdu->pdcchPduConfig.numDlDci          = 1;
      ulDciReqPdu->pdcchPduConfig.coreSetType       = coreSetType;

      /* Calculating PDU length. Considering only one Ul dci pdu for now */
      ulDciReqPdu->pduSize = sizeof(fapi_dl_pdcch_pdu_t);
   }
   return ROK;
}
#endif
/*******************************************************************
 *
 * @brief Sends UL DCI Request to PHY
 *
 * @details
 *
 *    Function : fillUlDciReq
 *
 *    Functionality:
 *         -Sends FAPI Ul Dci req to PHY
 *
 * @params[in]  Pointer to CmLteTimingInfo
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint16_t fillUlDciReq(SlotTimingInfo currTimingInfo, p_fapi_api_queue_elem_t prevElem)
{
#ifdef INTEL_FAPI
   uint8_t      cellIdx =0;
   uint8_t      numPduEncoded = 0;
   SlotTimingInfo  ulDciReqTimingInfo ={0};
   MacDlSlot    *currDlSlot = NULLP;
   fapi_ul_dci_req_t        *ulDciReq =NULLP;
   p_fapi_api_queue_elem_t  ulDciElem;

   if(lwrMacCb.phyState == PHY_STATE_RUNNING)
   {
      GET_CELL_IDX(currTimingInfo.cellId, cellIdx);
      memcpy(&ulDciReqTimingInfo, &currTimingInfo, sizeof(SlotTimingInfo));
      currDlSlot = &macCb.macCell[cellIdx]->dlSlot[ulDciReqTimingInfo.slot % MAX_SLOTS];

         LWR_MAC_ALLOC(ulDciElem, (sizeof(fapi_api_queue_elem_t) + sizeof(fapi_ul_dci_req_t)));
         if(ulDciElem)
	 {
	    FILL_FAPI_LIST_ELEM(ulDciElem, NULLP, FAPI_UL_DCI_REQUEST, 1, \
	       sizeof(fapi_ul_dci_req_t));
	    ulDciReq = (fapi_ul_dci_req_t *)(ulDciElem +1);
	    memset(ulDciReq, 0, sizeof(fapi_ul_dci_req_t));
	    fillMsgHeader(&ulDciReq->header, FAPI_UL_DCI_REQUEST, sizeof(fapi_ul_dci_req_t));

            ulDciReq->sfn  = ulDciReqTimingInfo.sfn;
            ulDciReq->slot = ulDciReqTimingInfo.slot;
          if(currDlSlot->dlInfo.ulGrant != NULLP)
          {
            ulDciReq->numPdus = 1;  // No. of PDCCH PDUs
            if(ulDciReq->numPdus > 0)
            {
               /* Fill PDCCH configuration Pdu */
               fillUlDciPdcchPdu(&ulDciReq->pdus[numPduEncoded], &currDlSlot->dlInfo, CORESET_TYPE1);
               numPduEncoded++;
	       /* free UL GRANT at SCH */
	       MAC_FREE(currDlSlot->dlInfo.ulGrant, sizeof(DciInfo));
	       currDlSlot->dlInfo.ulGrant = NULLP;
            }
#ifdef ODU_SLOT_IND_DEBUG_LOG
	       DU_LOG("\nDEBUG  -->  LWR_MAC: Sending UL DCI Request");
#endif
	 }
               prevElem->p_next = ulDciElem;
      }
   }
   else
   {
       lwr_mac_procInvalidEvt(&currTimingInfo);
   }
#endif
   return ROK;
}

lwrMacFsmHdlr fapiEvtHdlr[MAX_STATE][MAX_EVENT] =
{
   {
      /* PHY_STATE_IDLE */
#ifdef INTEL_TIMER_MODE 
      lwr_mac_procIqSamplesReqEvt,
#endif
      lwr_mac_procParamReqEvt,
      lwr_mac_procParamRspEvt,
      lwr_mac_procConfigReqEvt,
      lwr_mac_procConfigRspEvt,
      lwr_mac_procInvalidEvt,
      lwr_mac_procInvalidEvt,
   },
   {
      /* PHY_STATE_CONFIGURED */
#ifdef INTEL_TIMER_MODE
      lwr_mac_procInvalidEvt,
#endif
      lwr_mac_procParamReqEvt,
      lwr_mac_procParamRspEvt,
      lwr_mac_procConfigReqEvt,
      lwr_mac_procConfigRspEvt,
      lwr_mac_procStartReqEvt,
      lwr_mac_procInvalidEvt,
   },
   {
      /* PHY_STATE_RUNNING */
#ifdef INTEL_TIMER_MODE
      lwr_mac_procInvalidEvt,
#endif
      lwr_mac_procInvalidEvt,
      lwr_mac_procInvalidEvt,
      lwr_mac_procConfigReqEvt,
      lwr_mac_procConfigRspEvt,
      lwr_mac_procInvalidEvt,
      lwr_mac_procInvalidEvt,
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
   lwrMacCb.event = msgType;
   fapiEvtHdlr[lwrMacCb.phyState][lwrMacCb.event](msg);
}
/**********************************************************************
  End of file
 **********************************************************************/
