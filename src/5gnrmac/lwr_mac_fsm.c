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
#include "lwr_mac_fsm.h"

EXTERN void sendToPhy ARGS((U16 msgType, U32 msgLen, void *msg));

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
  printf("\n Error Indication Event[%d] received in state [%d]", clGlobalCp.event, clGlobalCp.phyState);
  RETVALUE(ROK);
}

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
PUBLIC void fillMsgHeader(fapi_msg_t *hdr, U16 msgType, U16 msgLen)
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
PUBLIC void fillTlvs(fapi_uint16_tlv_t *tlv, U16 tag, U16 length, U16 value, U16 *msgLen)
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
PUBLIC void fillCyclicPrefix(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillSubcarrierSpaceDl(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillBandwidthDl(U16 value, ClCellParam **cellPtr)
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

PUBLIC void fillSubcarrierSpaceUl(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillBandwidthUl(U16 value, ClCellParam **cellPtr)
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

PUBLIC void fillCCEmaping(U8 value,  ClCellParam **cellPtr)
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

PUBLIC void fillPucchFormat(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPdschMappingType(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPdschAllocationType(U8 value, ClCellParam **cellPtr)
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
PUBLIC void fillPrbMappingType(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPdschDmrsConfigType(U8 value, ClCellParam **cellPtr)
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
PUBLIC void fillPdschDmrsLength(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPdschDmrsAddPos(U8 value, ClCellParam **cellPtr)
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
PUBLIC void fillModulationOrderDl(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPuschDmrsConfig(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPuschDmrsLength(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPuschDmrsAddPos(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPuschMappingType(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPuschAllocationType(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPuschPrbMappingType(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillModulationOrderUl(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPuschAggregationFactor(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPrachLongFormat(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillPrachShortFormat(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillFdOccasions(U8 value, ClCellParam **cellPtr)
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

PUBLIC void fillRssiMeas(U8 value, ClCellParam **cellPtr)
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

S16 getParamValue(fapi_uint16_tlv_t *tlv, U16 type)
{
    U16 valueLen;
    void *temp, *posPtr;
    valueLen = tlv->tl.length;
    posPtr   = &tlv->tl.tag;
    posPtr   += sizeof(tlv->tl.tag);
    posPtr   += sizeof(tlv->tl.length);
    /*TO DO: malloc to SSI memory */
    if(type == FAPI_UINT_8)
    {
       temp = (U8 *)malloc(valueLen * sizeof(U8));
       memcpy(temp, posPtr, valueLen);
       return(*(U8 *)temp);
    }
    else if(type == FAPI_UINT_16)
    {
       temp = (U16 *)malloc(valueLen * sizeof(U16));
       memcpy(temp, posPtr, valueLen);
       return(*(U16 *)temp);
    }
    else if(type == FAPI_UINT_32)
    {
       temp = (U32 *)malloc(valueLen * sizeof(U32));
       memcpy(temp, posPtr, valueLen);
       return(*(U32 *)temp);
    }
    else
    {
      printf("\n Value Extraction failed" );
      RETVALUE(RFAILED);
    }
}
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
   /* startGuardTimer(); */
   fapi_param_req_t *paramReq = (fapi_param_req_t *)msg;
   if(SGetSBuf(0, 0, (Data **)&paramReq, sizeof(fapi_param_req_t)) != ROK)
   {
      printf("\nFailed to allocate memory for Param Request");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }
   else
   {
      fillMsgHeader(&paramReq->header, FAPI_PARAM_REQUEST, 0);
      sendToPhy(paramReq->header.message_type_id, sizeof(fapi_param_req_t), (void *)paramReq);
      RETVALUE(ROK);
   }
}

 /*******************************************************************
  *
  * @brief Sends FAPI Param rsp to MAC via PHY
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

S16 lwr_mac_handleParamRspEvt(fapi_param_resp_t *paramRsp)
{
  /* stopGuardTimer(); */
   U16 index;
   U32 encodedVal;
   ClCellParam *cellParam = NULLP;
   if(paramRsp == NULLP)
   {
      printf("\n Param Request for State [%d] is unsuccessfull", clGlobalCp.phyState);
      RETVALUE(RFAILED);
   }
   printf("\n Received EVENT[%d] at STATE[%d]", clGlobalCp.event, clGlobalCp.phyState);
   SPutSBuf(0, 0, (Data *)paramRsp, paramRsp->header.length);
   if(SGetSBuf(0, 0, (Data **)&cellParam, sizeof(ClCellParam)) != ROK)
   {
      printf("\nFailed to allocate memory for new cell");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   printf("\n Filling TLVS into MAC API");
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
               cellParam->maxMuMimoUsersDl         = getParamValue(&paramRsp->tlvs[index], FAPI_UINT_8);
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
            printf("\n Invalid value for TLV[%d]", paramRsp->tlvs[index].tl.tag);
	    break;
      }
    }
  }
  else
  {
    printf("\n Error Indication Evnt received in state[%d] event[%d]", clGlobalCp.phyState, clGlobalCp.event);
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
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
   RETVALUE(ROK);
}

S16 lwr_mac_handleConfigRspEvt(fapi_config_resp_t *configRsp)
{
   RETVALUE(ROK);
}

S16 lwr_mac_handleStartReqEvt(void *msg)
{
  /*fapi_slot_ind_t *slotInd;
  slotInd->header.message_type_id = FAPI_SLOT_INDICATION;
  slotInd->sfn = 0;
  slotInd->slot = 1;
  phyToMac(slotInd->header.message_type_id, sizeof(fapi_slot_ind_t), (void *)slotInd);
  */
  RETVALUE(ROK);
}

S16 lwr_mac_handleStopReqEvt(void *msg)
{
   /* stop TX and RX operation return PHy to configured State
      send stop.indication to l2/l3 */
   RETVALUE(ROK);
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
       lwr_mac_handleInvalidEvt
   },
   {
       /* PHY_STATE_CONFIGURED */
       lwr_mac_handleParamReqEvt,
       lwr_mac_handleParamRspEvt,
       lwr_mac_handleConfigReqEvt,
       lwr_mac_handleConfigRspEvt,
       lwr_mac_handleStartReqEvt,
       lwr_mac_handleInvalidEvt
   },
   {
       /* PHY_STATE_RUNNING */
       lwr_mac_handleInvalidEvt,
       lwr_mac_handleInvalidEvt,
       lwr_mac_handleConfigReqEvt,
       lwr_mac_handleConfigRspEvt,
       lwr_mac_handleInvalidEvt,
       lwr_mac_handleStopReqEvt
   }
};

/*******************************************************************
 *
 * @brief Sends message to Lower Mac Fsm Event Handler
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
S16 sendToLowerMac(U16 msgType, U32 msgLen, void *msg)
{
   clGlobalCp.event = msgType;
   fapiEvtHdlr[clGlobalCp.phyState][msgType](msg);
}

/**********************************************************************
         End of file
**********************************************************************/
