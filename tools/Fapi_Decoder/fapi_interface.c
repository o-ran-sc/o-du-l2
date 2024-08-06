#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "../../src/intel_fapi/fapi_interface.h"
#include "../../src/intel_fapi/fapi_vendor_extension.h"
#include "../../src/intel_fapi/nfapi_interface.h"
#include "../../src/intel_fapi/nr5g_fapi_common_types.h"
#include "../../src/intel_fapi/nr5g_fapi_internal.h"

void decode_fapi_srs_pdu(fapi_srs_pdu_t* srsPdu);
void decode_fapi_precoding_bmform(const fapi_precoding_bmform_t* precodingBfForm);

void decode_fapi_msg_header(fapi_msg_header_t* header) 
{
   printf("\n");
   printf("num_msg = %u\t", header->num_msg);
   printf("handle = %u\t", header->handle);
}

void decode_fapi_msg(fapi_msg_t* msg) 
{
   printf("\n");
   printf("msg_id = %u\t", msg->msg_id);
   printf("length = %u\t", msg->length);
   printf("pad = %u\t", msg->pad);
}

void decode_fapi_tl(fapi_tl_t* tl) 
{
   printf("\n");
   printf("tag = %u\t", tl->tag);
   printf("length = %u\t", tl->length);
}


void decode_fapi_uint8_tlv(fapi_uint8_tlv_t* tlv) 
{
   printf("\n");
   decode_fapi_tl(&tlv->tl);
   printf("value = %u\t", tlv->value);
   printf("rsv[0] = %u\t", tlv->rsv[0]);
   printf("rsv[1] = %u\t", tlv->rsv[1]);
   printf("rsv[2] = %u\t", tlv->rsv[2]);
}

void decode_fapi_uint8_ptr_tlv(fapi_uint8_ptr_tlv_t* tlv) 
{
   printf("\n");
   decode_fapi_tl(&tlv->tl);
   if (tlv->value != NULL) 
   {
      printf("value = %d", tlv->value);
      printf("\t");
   } else 
   {
      printf("value is NULL\t");
   }
}

void decode_fapi_uint16_tlv(fapi_uint16_tlv_t* tlv) 
{
   printf("\n");
   decode_fapi_tl(&tlv->tl);
   printf("value = %u\t", tlv->value);
   printf("rsv[0] = %u\t", tlv->rsv[0]);
   printf("rsv[1] = %u\t", tlv->rsv[1]);
}
void decode_fapi_int16_tlv(fapi_int16_tlv_t* tlv) 
{
   printf("\n");
   decode_fapi_tl(&tlv->tl);
   printf("value = %d\t", tlv->value);
   printf("rsv[0] = %u\t", tlv->rsv[0]);
   printf("rsv[1] = %u\t", tlv->rsv[1]);
}

void decode_fapi_uint32_tlv(fapi_uint32_tlv_t * tlv) 
{
   printf("\n");
   decode_fapi_tl(&tlv->tl);
   printf("value = %u ", tlv->value);
}

void decode_fapi_config_tlv(fapi_config_tlv_t* config) 
{
   printf("\n");
   printf("tag = %u\t", config->tag);
   printf("length = %u\t", config->length);
   printf("value = %u\t", config->value);
}

void decode_fapi_config_num_tlv(fapi_config_num_tlv_t* config_num) 
{
   printf("\n");
   decode_fapi_tl(&config_num->tl);
   for (size_t i = 0; i < FAPI_NUMEROLOGIES; i++) {
      printf("value[%zu] = %u\t", i, config_num->value[i]);
   }
   printf("rsv = %u\t", config_num->rsv);
}

void decode_fapi_param_req(fapi_param_req_t* param_req) 
{
   // Assuming fapi_msg_t header has its own check function
   decode_fapi_msg(&param_req->header);
}

void decode_fapi_cell_parms(fapi_cell_parms_t* cell_parms) 
{
   decode_fapi_uint16_tlv(&cell_parms->releaseCapability);
   decode_fapi_uint16_tlv(&cell_parms->phyState);
   decode_fapi_uint8_tlv(&cell_parms->skipBlankDlConfig);
   decode_fapi_uint8_tlv(&cell_parms->skipBlankUlConfig);
   decode_fapi_uint16_tlv(&cell_parms->numTlvsToReport);
   for (size_t i = 0; i < FAPI_MAX_NUM_TLVS_CONFIG; i++) 
   {
      decode_fapi_config_tlv(&cell_parms->tlvStatus[i]);
   }
}

void decode_fapi_carrier_parms(fapi_carrier_parms_t* carrier_parms) 
{
   decode_fapi_uint8_tlv(&carrier_parms->cyclicPrefix);
   decode_fapi_uint8_tlv(&carrier_parms->supportedSubcarrierSpacingDl);
   decode_fapi_uint16_tlv(&carrier_parms->supportedBandwidthDl);
   decode_fapi_uint8_tlv(&carrier_parms->supportedSubcarrierSpecingsUl);
   decode_fapi_uint16_tlv(&carrier_parms->supportedBandwidthUl);
}

void decode_fapi_pdcch_parms(fapi_pdcch_parms_t* pdcch_parms) 
{
   decode_fapi_uint8_tlv(&pdcch_parms->cceMappingType);
   decode_fapi_uint8_tlv(&pdcch_parms->coresetOutsideFirst3OfdmSymsOfSlot);
   decode_fapi_uint8_tlv(&pdcch_parms->precoderGranularityCoreset);
   decode_fapi_uint8_tlv(&pdcch_parms->pdcchMuMimo);
   decode_fapi_uint8_tlv(&pdcch_parms->pdcchPrecoderCycling);
   decode_fapi_uint8_tlv(&pdcch_parms->maxPdcchsPerSlot);
}

void decode_fapi_pucch_parms(fapi_pucch_parms_t* pucch_parms) 
{
   decode_fapi_uint8_tlv(&pucch_parms->pucchFormats);
   decode_fapi_uint8_tlv(&pucch_parms->maxPucchsPerSlot);
}

void decode_fapi_pdsch_parms(fapi_pdsch_parms_t* pdsch_parms) 
{
   decode_fapi_uint8_tlv(&pdsch_parms->pdschMappingType);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschAllocationTypes);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschVrbToPrbMapping);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschCbg);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschDmrsConfigTypes);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschDmrsMaxLength);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschDmrsAdditionalPos);
   decode_fapi_uint8_tlv(&pdsch_parms->maxPdschsTBsPerSlot);
   decode_fapi_uint8_tlv(&pdsch_parms->maxNumberMimoLayersPdsch);
   decode_fapi_uint8_tlv(&pdsch_parms->supportedMaxModulationOrderDl);
   decode_fapi_uint8_tlv(&pdsch_parms->maxMuMimoUsersDl);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschDataInDmrsSymbols);
   decode_fapi_uint8_tlv(&pdsch_parms->premptionSupport);
   decode_fapi_uint8_tlv(&pdsch_parms->pdschNonSlotSupport);
}

void decode_fapi_pusch_parms(fapi_pusch_parms_t* pusch_parms) 
{
   decode_fapi_uint8_tlv(&pusch_parms->uciMuxUlschInPusch);
   decode_fapi_uint8_tlv(&pusch_parms->uciOnlyPusch);
   decode_fapi_uint8_tlv(&pusch_parms->puschFrequencyHopping);
   decode_fapi_uint8_tlv(&pusch_parms->puschDmrsConfigTypes);
   decode_fapi_uint8_tlv(&pusch_parms->puschDmrsMaxLen);
   decode_fapi_uint8_tlv(&pusch_parms->puschDmrsAdditionalPos);
   decode_fapi_uint8_tlv(&pusch_parms->puschCbg);
   decode_fapi_uint8_tlv(&pusch_parms->puschMappingType);
   decode_fapi_uint8_tlv(&pusch_parms->puschAllocationTypes);
   decode_fapi_uint8_tlv(&pusch_parms->puschVrbToPrbMapping);
   decode_fapi_uint8_tlv(&pusch_parms->puschMaxPtrsPorts);
   decode_fapi_uint8_tlv(&pusch_parms->maxPduschsTBsPerSlot);
   decode_fapi_uint8_tlv(&pusch_parms->maxNumberMimoLayersNonCbPusch);
   decode_fapi_uint8_tlv(&pusch_parms->supportedModulationOrderUl);
   decode_fapi_uint8_tlv(&pusch_parms->maxMuMimoUsersUl);
   decode_fapi_uint8_tlv(&pusch_parms->dftsOfdmSupport);
   decode_fapi_uint8_tlv(&pusch_parms->puschAggregationFactor);
}

void decode_fapi_prach_parms(fapi_prach_parms_t* prach_parms) 
{
   decode_fapi_uint8_tlv(&prach_parms->prachLongFormats);
   decode_fapi_uint16_tlv(&prach_parms->prachShortFormats);
   decode_fapi_uint8_tlv(&prach_parms->prachRestrictedSets);
   decode_fapi_uint8_tlv(&prach_parms->maxPrachFdOccasionsInASlot);
}
void decode_fapi_meas_parms(fapi_meas_parms_t* meas_parms) 
{
   decode_fapi_uint8_tlv(&meas_parms->rssiMeasurementSupport);
}

void decode_fapi_params(fapi_params_t* params) 
{
   decode_fapi_cell_parms(&params->cell_parms);
   decode_fapi_carrier_parms(&params->carr_parms);
   decode_fapi_pdcch_parms(&params->pdcch_parms);
   decode_fapi_pucch_parms(&params->pucch_parms);
   decode_fapi_pdsch_parms(&params->pdsch_parms);
   decode_fapi_pusch_parms(&params->pusch_parms);
   decode_fapi_prach_parms(&params->prach_parms);
   decode_fapi_meas_parms(&params->meas_parms);
}

void decode_fapi_param_resp(fapi_param_resp_t* param_resp) 
{
   printf("\n");
   decode_fapi_msg(&param_resp->header);
   printf("error_code = %u\t", param_resp->error_code);
   printf("number_of_tlvs = %u\t", param_resp->number_of_tlvs);
   for (size_t i = 0; i < FAPI_MAX_NUM_TLVS_PARAMS; i++) 
   {
      decode_fapi_uint16_tlv(&param_resp->tlvs[i]);
   }
}

void decode_fapi_carrier_config(fapi_carrier_config_t* carrier_config) 
{
   decode_fapi_uint16_tlv(&carrier_config->dlBandwidth);
   decode_fapi_uint32_tlv(&carrier_config->dlFrequency);
   decode_fapi_config_num_tlv(&carrier_config->dlk0);
   decode_fapi_config_num_tlv(&carrier_config->dlGridSize);
   decode_fapi_uint16_tlv(&carrier_config->numTxAnt);
   decode_fapi_uint16_tlv(&carrier_config->uplinkBandwidth);
   decode_fapi_uint32_tlv(&carrier_config->uplinkFrequency);
   decode_fapi_config_num_tlv(&carrier_config->ulk0);
   decode_fapi_config_num_tlv(&carrier_config->ulGridSize);
   decode_fapi_uint16_tlv(&carrier_config->numRxAnt);
   decode_fapi_uint8_tlv(&carrier_config->frequencyShift7p5KHz);
}

void decode_fapi_cell_config(fapi_cell_config_t* cell_config) 
{
   decode_fapi_uint8_tlv(&cell_config->phyCellId);
   decode_fapi_uint8_tlv(&cell_config->frameDuplexType);
}

void decode_fapi_ssb_config(fapi_ssb_config_t* ssb_config) 
{
   decode_fapi_uint32_tlv(&ssb_config->ssPbchPower);
   decode_fapi_uint8_tlv(&ssb_config->bchPayload);
   decode_fapi_uint8_tlv(&ssb_config->scsCommon);
}

void decode_fapi_prachFdOccasion(fapi_prachFdOccasion_t* prachFdOccasion) 
{
   decode_fapi_uint16_tlv(&prachFdOccasion->prachRootSequenceIndex);
   decode_fapi_uint8_tlv(&prachFdOccasion->numRootSequences);
   decode_fapi_uint16_tlv(&prachFdOccasion->k1);
   decode_fapi_uint8_tlv(&prachFdOccasion->prachZeroCorrConf);
   decode_fapi_uint16_tlv(&prachFdOccasion->numUnusedRootSequences);
   for (size_t i = 0; i < FAPI_MAX_NUM_UNUSED_ROOT_SEQUENCES; i++) {
      decode_fapi_uint16_tlv(&prachFdOccasion->unusedRootSequences[i]);
   }
}

void decode_fapi_prach_configuration(fapi_prach_configuration_t* prachConfig) 
{
   decode_fapi_uint8_tlv(&prachConfig->prachSequenceLength);
   decode_fapi_uint8_tlv(&prachConfig->prachSubCSpacing);
   decode_fapi_uint8_tlv(&prachConfig->restrictedSetConfig);
   decode_fapi_uint8_tlv(&prachConfig->numPrachFdOccasions);
   decode_fapi_uint8_tlv(&prachConfig->prachConfigIndex);
   for (size_t i = 0; i < FAPI_MAX_NUM_PRACH_FD_OCCASIONS; i++) {
      decode_fapi_prachFdOccasion(&prachConfig->prachFdOccasion[i]);
   }
   decode_fapi_uint8_tlv(&prachConfig->ssbPerRach);
   decode_fapi_uint8_tlv(&prachConfig->prachMultipleCarriersInABand);
}

void decode_fapi_ssb_table(fapi_ssb_table_t* ssbTable) 
{
   decode_fapi_uint16_tlv(&ssbTable->ssbOffsetPointA);
   decode_fapi_uint8_tlv(&ssbTable->betaPss);
   decode_fapi_uint8_tlv(&ssbTable->ssbPeriod);
   decode_fapi_uint8_tlv(&ssbTable->ssbSubCarrierOffset);
   decode_fapi_uint32_tlv(&ssbTable->mib);
   for (size_t i = 0; i < 2; i++) {
      decode_fapi_uint32_tlv(&ssbTable->ssbMask[i]);
   }
   for (size_t i = 0; i < 64; i++) {
      decode_fapi_uint8_tlv(&ssbTable->beamId[i]);
   }
   decode_fapi_uint8_tlv(&ssbTable->ssPbchMultipleCarriersInABand);
   decode_fapi_uint8_tlv(&ssbTable->multipleCellsSsPbchInACarrier);
}

void decode_fapi_slotconfig(fapi_slotconfig_t* slotConfig) 
{
   for (size_t i = 0; i < FAPI_MAX_NUM_OF_SYMBOLS_PER_SLOT; i++) {
      decode_fapi_uint8_tlv(&slotConfig->slotConfig[i]);
   }
}

void decode_fapi_tdd_table(fapi_tdd_table_t* tddTable) 
{
   decode_fapi_uint8_tlv(&tddTable->tddPeriod);
   for (size_t i = 0; i < FAPI_MAX_TDD_PERIODICITY; i++) {
      decode_fapi_slotconfig(&tddTable->slotConfig[i]);
   }
}

void decode_fapi_meas_config(fapi_meas_config_t* measConfig) 
{
   decode_fapi_uint8_tlv(&measConfig->rssiMeasurement);
}

void decode_fapi_dig_beam_weight(fapi_dig_beam_weight_t* digBeamWeight) 
{
   printf("\n");
   printf("digBeamWeightRe = %d\t", digBeamWeight->digBeamWeightRe);
   printf("digBeamWeightIm = %d\t", digBeamWeight->digBeamWeightIm);
}

void decode_fapi_dig_beam_config(fapi_dig_beam_config_t* digBeamConfig) 
{
   printf("\n");
   printf("beamIdx = %u\t", digBeamConfig->beamIdx);
   for (size_t i = 0; i < FAPI_MAX_NUMBER_TX_RUS; i++) {
      decode_fapi_dig_beam_weight(&digBeamConfig->digBeamWeight[i]);
   }
}

void decode_fapi_beamforming_table(fapi_beamforming_table_t* beamformingTable) 
{
   printf("\n");
   printf("numDigBeams = %u\t", beamformingTable->numDigBeams);
   printf("numTxRus = %u\t", beamformingTable->numTxRus);
   for (size_t i = 0; i < FAPI_MAX_NUMBER_OF_BEAMS; i++) {
      decode_fapi_dig_beam_config(&beamformingTable->digBeam[i]);
   }
}

void decode_fapi_precoderWeight_t(fapi_precoderWeight_t* precoder)
{

   printf("\n");
   printf("preCoderWeightRe = %d\t", precoder->preCoderWeightRe);
   printf("preCoderWeightIm = %d\t", precoder->preCoderWeightIm);

}
void decode_fapi_precoder_weight_t(fapi_precoder_weight_t* precoderWeight) 
{
   for (size_t i = 0; i < FAPI_MAX_NUM_ANT_PORTS; i++) {
      decode_fapi_precoderWeight_t(&precoderWeight->precoder_weight[i]);
   }
}

void decode_fapi_precoding_table(fapi_precoding_table_t* precodingTable) 
{
   printf("\n");
   printf("pmIdx = %u\t", precodingTable->pmIdx);
   printf("numLayers = %u\t", precodingTable->numLayers);
   printf("numAntPorts = %u\t", precodingTable->numAntPorts);
   for (size_t i = 0; i < FAPI_MAX_NUM_LAYERS; i++) {
      decode_fapi_precoder_weight_t(&precodingTable->precoderWeight[i]);
   }
}

void decode_fapi_config(fapi_config_t* config) 
{
   decode_fapi_carrier_config(&config->carrierConfig);
   decode_fapi_cell_config(&config->cellConfig);
   decode_fapi_ssb_config(&config->ssbConfig);
   decode_fapi_prach_configuration(&config->prachConfig);
   decode_fapi_ssb_table(&config->ssbTable);
   decode_fapi_tdd_table(&config->tddTable);
   decode_fapi_meas_config(&config->measConfig);
   decode_fapi_beamforming_table(&config->beamformingTable);
   decode_fapi_precoding_table(&config->precodingTable);
}

void handleCfgReqtlv(fapi_uint32_tlv_t* tlv, fapi_config_t* config) 
{
   printf("\n");
   printf("\ttl.tag = %u\t", tlv->tl.tag);
   switch (tlv->tl.tag) {
      // Carrier Configuration Tags
      case FAPI_DL_BANDWIDTH_TAG:
         config->carrierConfig.dlBandwidth.value = (uint16_t)tlv->value;
         break;

      case FAPI_UPLINK_BANDWIDTH_TAG:
         config->carrierConfig.uplinkBandwidth.value = (uint16_t)tlv->value;
         break;

      case FAPI_DL_FREQUENCY_TAG:
         config->carrierConfig.dlFrequency.value = (uint32_t)tlv->value;
         break;

      case FAPI_UPLINK_FREQUENCY_TAG:
         config->carrierConfig.uplinkFrequency.value = (uint32_t)tlv->value;
         break;

      case FAPI_DL_K0_TAG:
         config->carrierConfig.dlk0.value[0] = (uint16_t)tlv->value;
         break;

      case FAPI_UL_K0_TAG:
         config->carrierConfig.ulk0.value[0] = (uint16_t)tlv->value;
         break;

      case FAPI_DL_GRIDSIZE_TAG:
         config->carrierConfig.dlGridSize.value[0] = (uint8_t)tlv->value;
         break;

      case FAPI_UL_GRID_SIZE_TAG:
         config->carrierConfig.ulGridSize.value[0] = (uint8_t)tlv->value;
         break;

      case FAPI_NUM_TX_ANT_TAG:
         config->carrierConfig.numTxAnt.value = (uint8_t)tlv->value;
         break;

      case FAPI_NUM_RX_ANT_TAG:
         config->carrierConfig.numRxAnt.value = (uint8_t)tlv->value;
         break;

      case FAPI_FREQUENCY_SHIFT_7P5_KHZ_TAG:
         config->carrierConfig.frequencyShift7p5KHz.value = (uint8_t)tlv->value;
         break;

         // Cell Configuration Tags
      case FAPI_PHY_CELL_ID_TAG:
         config->cellConfig.phyCellId.value = (uint16_t)tlv->value;
         break;

      case FAPI_FRAME_DUPLEX_TYPE_TAG:
         config->cellConfig.frameDuplexType.value = (uint8_t)tlv->value;
         break;

         // SSB Configuration Tags
      case FAPI_SS_PBCH_POWER_TAG:
         config->ssbConfig.ssPbchPower.value = (int8_t)tlv->value;
         break;

      case FAPI_BCH_PAYLOAD_TAG:
         config->ssbConfig.bchPayload.value = (uint32_t)tlv->value;
         break;

      case FAPI_SCS_COMMON_TAG:
         config->ssbConfig.scsCommon.value = (uint16_t)tlv->value;
         break;

         // PRACH Configuration Tags
      case FAPI_PRACH_SEQUENCE_LENGTH_TAG:
         config->prachConfig.prachSequenceLength.value = (uint8_t)tlv->value;
         break;

      case FAPI_PRACH_SUBC_SPACING_TAG:
         config->prachConfig.prachSubCSpacing.value = (uint8_t)tlv->value;
         break;

      case FAPI_RESTRICTED_SET_CONFIG_TAG:
         config->prachConfig.restrictedSetConfig.value = (uint8_t)tlv->value;
         break;

      case FAPI_NUM_PRACH_FD_OCCASIONS_TAG:
         config->prachConfig.numPrachFdOccasions.value = (uint8_t)tlv->value;
         break;

      case FAPI_PRACH_ROOT_SEQUENCE_INDEX_TAG:
         config->prachConfig.prachFdOccasion[0].prachRootSequenceIndex.value = (uint16_t)tlv->value;
         break;

      case FAPI_NUM_ROOT_SEQUENCES_TAG:
         config->prachConfig.prachFdOccasion[0].numRootSequences.value = (uint16_t)tlv->value;
         break;

      case FAPI_K1_TAG:
         config->prachConfig.prachFdOccasion[0].k1.value = (uint8_t)tlv->value;
         break;

      case FAPI_PRACH_ZERO_CORR_CONF_TAG:
         config->prachConfig.prachFdOccasion[0].prachZeroCorrConf.value = (uint8_t)tlv->value;
         break;

      case FAPI_NUM_UNUSED_ROOT_SEQUENCES_TAG:
         config->prachConfig.prachFdOccasion[0].numUnusedRootSequences.value = (uint8_t)tlv->value;
         break;

      case FAPI_UNUSED_ROOT_SEQUENCES_TAG:
         {
            for(uint8_t idx =0; idx<config->prachConfig.prachFdOccasion[0].numUnusedRootSequences.value; idx++)
               config->prachConfig.prachFdOccasion[0].unusedRootSequences[idx].value = (uint8_t)tlv->value;
            break;
         }

      case FAPI_SSB_PER_RACH_TAG:
         config->prachConfig.ssbPerRach.value = (uint8_t)tlv->value;
         break;

      case FAPI_PRACH_MULTIPLE_CARRIERS_IN_A_BAND_TAG:
         config->prachConfig.prachMultipleCarriersInABand.value = (uint8_t)tlv->value;
         break;

         // SSB Table Tags
      case FAPI_SSB_OFFSET_POINT_A_TAG:
         config->ssbTable.ssbOffsetPointA.value = (uint32_t)tlv->value;
         break;

      case FAPI_BETA_PSS_TAG:
         config->ssbTable.betaPss.value = (uint8_t)tlv->value;
         break;

      case FAPI_SSB_PERIOD_TAG:
         config->ssbTable.ssbPeriod.value = (uint8_t)tlv->value;
         break;

      case FAPI_SSB_SUBCARRIER_OFFSET_TAG:
         config->ssbTable.ssbSubCarrierOffset.value = (uint16_t)tlv->value;
         break;

      case FAPI_MIB_TAG:
         config->ssbTable.mib.value = (uint32_t)tlv->value;
         break;

      case FAPI_SSB_MASK_TAG:
         memcpy(config->ssbTable.ssbMask, &tlv->value, sizeof(config->ssbTable.ssbMask));
         break;

      case FAPI_BEAM_ID_TAG:
         memcpy(config->ssbTable.beamId, &tlv->value, sizeof(config->ssbTable.beamId));
         break;

      case FAPI_SS_PBCH_MULTIPLE_CARRIERS_IN_A_BAND_TAG:
         config->ssbTable.ssPbchMultipleCarriersInABand.value = (uint8_t)tlv->value;
         break;

      case FAPI_MULTIPLE_CELLS_SS_PBCH_IN_A_CARRIER_TAG:
         config->ssbTable.multipleCellsSsPbchInACarrier.value = (uint8_t)tlv->value;
         break;

         // TDD Table Tags
      case FAPI_TDD_PERIOD_TAG:
         config->tddTable.tddPeriod.value = (uint8_t)tlv->value;
         break;

      case FAPI_SLOT_CONFIG_TAG:
         memcpy(config->tddTable.slotConfig, &tlv->value, sizeof(config->tddTable.slotConfig));
         break;

         // Measurement Configuration Tags
      case FAPI_RSSI_MEASUREMENT_TAG:
         config->measConfig.rssiMeasurement.value = (uint8_t)tlv->value;
         break;

      default:
         printf("Unsupported TLV tag: %u\t", tlv->tl.tag);
         break;
   }
}


void decode_fapi_config_req(fapi_config_req_t* configReq) 
{
   printf("\n");
   fapi_config_t config;

   decode_fapi_msg(&configReq->header);
   printf("number_of_tlvs = %u\t", configReq->number_of_tlvs);
   for (size_t i = 0; i < configReq->number_of_tlvs; i++) {
      decode_fapi_uint32_tlv(&configReq->tlvs[i]);
      handleCfgReqtlv(&configReq->tlvs[i], &config);
   }
   decode_fapi_config(&config);    
}

void decode_fapi_config_resp(fapi_config_resp_t* configResp) 
{
   printf("\n");
   decode_fapi_msg(&configResp->header);
   printf("error_code = %u\t", configResp->error_code);
   printf("number_of_invalid_tlvs = %u\t", configResp->number_of_invalid_tlvs);
   printf("number_of_inv_tlvs_idle_only = %u\t", configResp->number_of_inv_tlvs_idle_only);
   printf("number_of_inv_tlvs_running_only = %u\t", configResp->number_of_inv_tlvs_running_only);
   printf("number_of_missing_tlvs = %u\t", configResp->number_of_missing_tlvs);
   for (size_t i = 0; i < 4 * FAPI_MAX_NUM_TLVS_CONFIG; i++) {
      decode_fapi_uint16_tlv(&configResp->tlvs[i]);
   }
}

void decode_fapi_start_req(fapi_start_req_t* startReq) 
{
   decode_fapi_msg(&startReq->header);
}

void decode_fapi_stop_req(fapi_stop_req_t* stopReq) 
{
   decode_fapi_msg(&stopReq->header);
}

void decode_fapi_stop_ind(fapi_stop_ind_t* stopInd) 
{
   decode_fapi_msg(&stopInd->header);
}

void decode_fapi_error_ind(fapi_error_ind_t* errorInd) 
{
   printf("\n");
   decode_fapi_msg(&errorInd->header);
   printf("sfn = %u\t", errorInd->sfn);
   printf("slot = %u\t", errorInd->slot);
   printf("message_id = %u\t", errorInd->message_id);
   printf("error_code = %u\t", errorInd->error_code);
}

void decode_fapi_slot_ind(fapi_slot_ind_t* slotInd) 
{
   printf("\n");
   decode_fapi_msg(&slotInd->header);
   printf("sfn = %u\t", slotInd->sfn);
   printf("slot = %u\t", slotInd->slot);
}

void decode_fapi_bmi(const fapi_bmi_t* bmi) 
{
   printf("\n");
   printf("beamidx = %u\t", bmi->beamidx);
}

void decode_fapi_pmi_bfi(const fapi_pmi_bfi_t* pmiBfi) 
{
   printf("\n");
   printf("pmIdx = %u\t", pmiBfi->pmIdx);
   for (size_t i = 0; i < FAPI_MAX_NUM_DIGBFINTERFACES; i++) {
      decode_fapi_bmi(&pmiBfi->beamIdx[i]);
   }
}

void decode_fapi_dl_dci(fapi_dl_dci_t* dlDci) 
{
   printf("\n");
   printf("rnti = %u\t", dlDci->rnti);
   printf("scramblingId = %u\t", dlDci->scramblingId);
   printf("scramblingRnti = %u\t", dlDci->scramblingRnti);
   printf("cceIndex = %u\t", dlDci->cceIndex);
   printf("aggregationLevel = %u\t", dlDci->aggregationLevel);
   decode_fapi_precoding_bmform(&dlDci->pc_and_bform);
   printf("beta_pdcch_1_0 = %u\t", dlDci->beta_pdcch_1_0);
   printf("powerControlOffsetSS = %u\t", dlDci->powerControlOffsetSS);
   printf("payloadSizeBits = %u\t", dlDci->payloadSizeBits);
   for (size_t i = 0; i < FAPI_DCI_PAYLOAD_BYTE_LEN; i++) {
      printf("payload[%zu] = %u\t", i, dlDci->payload[i]);
   }
}

void decode_fapi_dl_pdcch_pdu(fapi_dl_pdcch_pdu_t* dlPdcchPdu) 
{
   printf("\n");
   printf("bwpSize = %u\t", dlPdcchPdu->bwpSize);
   printf("bwpStart = %u\t", dlPdcchPdu->bwpStart);
   printf("subCarrierSpacing = %u\t", dlPdcchPdu->subCarrierSpacing);
   printf("cyclicPrefix = %u\t", dlPdcchPdu->cyclicPrefix);
   printf("startSymbolIndex = %u\t", dlPdcchPdu->startSymbolIndex);
   printf("durationSymbols = %u\t", dlPdcchPdu->durationSymbols);
   printf("freqDomainResource = ");
   for (size_t i = 0; i < sizeof(dlPdcchPdu->freqDomainResource); i++) {
      printf("%u ", dlPdcchPdu->freqDomainResource[i]);
   }
   printf("\t");
   printf("cceRegMappingType = %u\t", dlPdcchPdu->cceRegMappingType);
   printf("regBundleSize = %u\t", dlPdcchPdu->regBundleSize);
   printf("interleaverSize = %u\t", dlPdcchPdu->interleaverSize);
   printf("coreSetType = %u\t", dlPdcchPdu->coreSetType);
   printf("shiftIndex = %u\t", dlPdcchPdu->shiftIndex);
   printf("precoderGranularity = %u\t", dlPdcchPdu->precoderGranularity);
   printf("numDlDci = %u\t", dlPdcchPdu->numDlDci);
   for (size_t i = 0; i < dlPdcchPdu->numDlDci; i++) {
      decode_fapi_dl_dci(&dlPdcchPdu->dlDci[i]);
   }
}

void decode_fapi_codeword_pdu(fapi_codeword_pdu_t* codewordPdu) 
{
   printf("\n");
   printf("targetCodeRate = %u\t", codewordPdu->targetCodeRate);
   printf("qamModOrder = %u\t", codewordPdu->qamModOrder);
   printf("mcsIndex = %u\t", codewordPdu->mcsIndex);
   printf("mcsTable = %u\t", codewordPdu->mcsTable);
   printf("rvIndex = %u\t", codewordPdu->rvIndex);
   printf("tbSize = %u\t", codewordPdu->tbSize);
}

void decode_fapi_precoding_bmform(const fapi_precoding_bmform_t* precodingBfForm) 
{
   printf("\n");
   printf("numPrgs = %u\t", precodingBfForm->numPrgs);
   printf("prgSize = %u\t", precodingBfForm->prgSize);
   printf("digBfInterfaces = %u\t", precodingBfForm->digBfInterfaces);
   for (size_t i = 0; i < precodingBfForm->numPrgs; i++) 
   {
      decode_fapi_pmi_bfi(&precodingBfForm->pmi_bfi[i]);
   }
}

void decode_fapi_dl_pdsch_pdu(fapi_dl_pdsch_pdu_t* dlPdschPdu) 
{
   printf("\n");
   printf("pduBitMap = %u\t", dlPdschPdu->pduBitMap);
   printf("rnti = %u\t", dlPdschPdu->rnti);
   printf("pdu_index = %u\t", dlPdschPdu->pdu_index);
   printf("bwpSize = %u\t", dlPdschPdu->bwpSize);
   printf("bwpStart = %u\t", dlPdschPdu->bwpStart);
   printf("subCarrierSpacing = %u\t", dlPdschPdu->subCarrierSpacing);
   printf("cyclicPrefix = %u\t", dlPdschPdu->cyclicPrefix);
   printf("nrOfCodeWords = %u\t", dlPdschPdu->nrOfCodeWords);

   for (size_t i = 0; i < dlPdschPdu->nrOfCodeWords; i++) {
      printf("cwInfo[%zu]:\t", i);
      decode_fapi_codeword_pdu(&dlPdschPdu->cwInfo[i]);
   }

   printf("dataScramblingId = %u\t", dlPdschPdu->dataScramblingId);
   printf("nrOfLayers = %u\t", dlPdschPdu->nrOfLayers);
   printf("transmissionScheme = %u\t", dlPdschPdu->transmissionScheme);
   printf("refPoint = %u\t", dlPdschPdu->refPoint);
   printf("dmrsConfigType = %u\t", dlPdschPdu->dmrsConfigType);
   printf("dlDmrsSymbPos = %u\t", dlPdschPdu->dlDmrsSymbPos);
   printf("scid = %u\t", dlPdschPdu->scid);
   printf("numDmrsCdmGrpsNoData = %u\t", dlPdschPdu->numDmrsCdmGrpsNoData);
   printf("resourceAlloc = %u\t", dlPdschPdu->resourceAlloc);
   printf("dlDmrsScramblingId = %u\t", dlPdschPdu->dlDmrsScramblingId);
   printf("dmrsPorts = %u\t", dlPdschPdu->dmrsPorts);
   printf("rbStart = %u\t", dlPdschPdu->rbStart);
   printf("rbSize = %u\t", dlPdschPdu->rbSize);

   printf("rbBitmap = ");
   for (size_t i = 0; i < sizeof(dlPdschPdu->rbBitmap); i++) {
      printf("%u ", dlPdschPdu->rbBitmap[i]);
   }

   printf("vrbToPrbMapping = %u\t", dlPdschPdu->vrbToPrbMapping);
   printf("startSymbIndex = %u\t", dlPdschPdu->startSymbIndex);
   printf("nrOfSymbols = %u\t", dlPdschPdu->nrOfSymbols);
   printf("ptrsPortIndex = %u\t", dlPdschPdu->ptrsPortIndex);
   printf("ptrsTimeDensity = %u\t", dlPdschPdu->ptrsTimeDensity);
   printf("ptrsFreqDensity = %u\t", dlPdschPdu->ptrsFreqDensity);
   printf("ptrsReOffset = %u\t", dlPdschPdu->ptrsReOffset);
   printf("nEpreRatioOfPdschToPtrs = %u\t", dlPdschPdu->nEpreRatioOfPdschToPtrs);

   decode_fapi_precoding_bmform(&dlPdschPdu->preCodingAndBeamforming);

   printf("powerControlOffset = %u\t", dlPdschPdu->powerControlOffset);
   printf("powerControlOffsetSS = %u\t", dlPdschPdu->powerControlOffsetSS);
   printf("isLastCbPresent = %u\t", dlPdschPdu->isLastCbPresent);
   printf("isInlineTbCrc = %u\t", dlPdschPdu->isInlineTbCrc);
   printf("dlTbCrc = %u\t", dlPdschPdu->dlTbCrc);
   printf("mappingType = %u\t", dlPdschPdu->mappingType);
   printf("nrOfDmrsSymbols = %u\t", dlPdschPdu->nrOfDmrsSymbols);
   printf("dmrsAddPos = %u\t", dlPdschPdu->dmrsAddPos);
}

void decode_fapi_phy_mib_pdu(fapi_phy_mib_pdu_t* phyMibPdu) 
{
   printf("\n");
   printf("dmrsTypeAPosition = %u\t", phyMibPdu->dmrsTypeAPosition);
   printf("pdcchConfigSib1 = %u\t", phyMibPdu->pdcchConfigSib1);
   printf("cellBarred = %u\t", phyMibPdu->cellBarred);
   printf("intraFreqReselection = %u\t", phyMibPdu->intraFreqReselection);
}

void decode_fapi_bch_payload(fapi_bch_payload_t* bchPayload) 
{
   printf("\n");
   printf("bchPayload.bchPayload = %u\t", bchPayload->bchPayload);
   printf("bchPayload.phyMibPdu:\t");
   decode_fapi_phy_mib_pdu(&bchPayload->phyMibPdu);
}

void decode_fapi_dl_ssb_pdu(fapi_dl_ssb_pdu_t* dlSsbPdu) 
{
   printf("\n");
   printf("physCellId = %u\t", dlSsbPdu->physCellId);
   printf("betaPss = %u\t", dlSsbPdu->betaPss);
   printf("ssbBlockIndex = %u\t", dlSsbPdu->ssbBlockIndex);
   printf("ssbSubCarrierOffset = %u\t", dlSsbPdu->ssbSubCarrierOffset);
   printf("bchPayloadFlag = %u\t", dlSsbPdu->bchPayloadFlag);
   printf("ssbOffsetPointA = %u\t", dlSsbPdu->ssbOffsetPointA);
   printf("bchPayload:\t");
   decode_fapi_bch_payload(&dlSsbPdu->bchPayload);
   printf("preCodingAndBeamforming:\t");
   decode_fapi_precoding_bmform(&dlSsbPdu->preCodingAndBeamforming);
}

void decode_fapi_dl_csi_rs_pdu(const fapi_dl_csi_rs_pdu_t *pdu) {

   printf("\n");
   printf("bwpSize = %u\t", pdu->bwpSize);
   printf("bwpStart = %u\t", pdu->bwpStart);
   printf("subCarrierSpacing = %u\t", pdu->subCarrierSpacing);
   printf("cyclicPrefix = %u\t", pdu->cyclicPrefix);
   printf("startRb = %u\t", pdu->startRb);
   printf("nrOfRbs = %u\t", pdu->nrOfRbs);
   printf("csiType = %u\t", pdu->csiType);
   printf("row = %u\t", pdu->row);
   printf("freqDomain = %u\t", pdu->freqDomain);
   printf("symbL0 = %u\t", pdu->symbL0);
   printf("symbL1 = %u\t", pdu->symbL1);
   printf("cdmType = %u\t", pdu->cdmType);
   printf("freqDensity = %u\t", pdu->freqDensity);
   printf("scramId = %u\t", pdu->scramId);
   printf("powerControlOffset = %u\t", pdu->powerControlOffset);
   printf("powerControlOffsetSs = %u\t", pdu->powerControlOffsetSs);
   decode_fapi_precoding_bmform(&pdu->preCodingAndBeamforming);

}

void decode_fapi_dl_tti_req_pdu(fapi_dl_tti_req_pdu_t* dlTtiReqPdu) 
{
   printf("\n");
   printf("pduType = %u\t", dlTtiReqPdu->pduType);
   printf("pduSize = %u\t", dlTtiReqPdu->pduSize);

   switch (dlTtiReqPdu->pduType) {
      case 0: // Example case for pdcch_pdu
         printf("pdcch_pdu:\t");
         decode_fapi_dl_pdcch_pdu(&dlTtiReqPdu->pdu.pdcch_pdu);
         break;
      case 1: // Example case for pdsch_pdu
         printf("pdsch_pdu:\t");
         decode_fapi_dl_pdsch_pdu(&dlTtiReqPdu->pdu.pdsch_pdu);
         break;
      case 2: // Example case for csi_rs_pdu
         printf("csi_rs_pdu:\t");
         decode_fapi_dl_csi_rs_pdu(&dlTtiReqPdu->pdu.csi_rs_pdu);
         break;
      case 3: // Example case for ssb_pdu
         printf("ssb_pdu:\t");
         decode_fapi_dl_ssb_pdu(&dlTtiReqPdu->pdu.ssb_pdu);
         break;
      default:
         printf("Unknown PDU type\t");
         break;
   }
}

void decode_fapi_ue_info(fapi_ue_info_t* ueInfo) 
{
   printf("\n");
   printf("nUe = %u\t", ueInfo->nUe);
   printf("pduIdx = ");
   for (size_t i = 0; i < sizeof(ueInfo->pduIdx); i++) {
      printf("%u ", ueInfo->pduIdx[i]);
   }
}

void decode_fapi_dl_tti_req(fapi_dl_tti_req_t* dlTtiReq) 
{
   printf("\n");
   printf("sfn = %u\t", dlTtiReq->sfn);
   printf("slot = %u\t", dlTtiReq->slot);
   printf("nPdus = %u\t", dlTtiReq->nPdus);
   printf("nGroup = %u\t", dlTtiReq->nGroup);

   printf("pdus:\t");
   for (size_t i = 0; i < dlTtiReq->nPdus; i++) {
      printf("PDUs[%zu]:\t", i);
      decode_fapi_dl_tti_req_pdu(&dlTtiReq->pdus[i]);
   }

   printf("ue_grp_info:\t");
   for (size_t i = 0; i < dlTtiReq->nGroup; i++) {
      printf("UE Group Info[%zu]:\t", i);
      decode_fapi_ue_info(&dlTtiReq->ue_grp_info[i]);
   }
}

void decode_fapi_pusch_data(fapi_pusch_data_t* puschData) 
{
   printf("\n");
   printf("rvIndex = %u\t", puschData->rvIndex);
   printf("harqProcessId = %u\t", puschData->harqProcessId);
   printf("newDataIndicator = %u\t", puschData->newDataIndicator);
   printf("tbSize = %u\t", puschData->tbSize);
   printf("numCb = %u\t", puschData->numCb);
   printf("cbPresentAndPosition = ");
   for (size_t i = 0; i < sizeof(puschData->cbPresentAndPosition); i++) {
      printf("%u ", puschData->cbPresentAndPosition[i]);
   }
}

void decode_fapi_pusch_uci(fapi_pusch_uci_t* puschUci) 
{
   printf("\n");
   printf("harqAckBitLength = %u\t", puschUci->harqAckBitLength);
   printf("csiPart1BitLength = %u\t", puschUci->csiPart1BitLength);
   printf("csiPart2BitLength = %u\t", puschUci->csiPart2BitLength);
   printf("alphaScaling = %u\t", puschUci->alphaScaling);
   printf("betaOffsetHarqAck = %u\t", puschUci->betaOffsetHarqAck);
   printf("betaOffsetCsi1 = %u\t", puschUci->betaOffsetCsi1);
   printf("betaOffsetCsi2 = %u\t", puschUci->betaOffsetCsi2);
}

void decode_fapi_ptrs_info(fapi_ptrs_info_t* ptrsInfo) 
{
   printf("\n");
   printf("ptrsPortIndex = %u\t", ptrsInfo->ptrsPortIndex);
   printf("ptrsDmrsPort = %u\t", ptrsInfo->ptrsDmrsPort);
   printf("ptrsReOffset = %u\t", ptrsInfo->ptrsReOffset);
}

void decode_fapi_pusch_ptrs(fapi_pusch_ptrs_t* puschPtrs) 
{
   printf("\n");
   printf("numPtrsPorts = %u\t", puschPtrs->numPtrsPorts);
   printf("ptrsTimeDensity = %u\t", puschPtrs->ptrsTimeDensity);
   printf("ptrsFreqDensity = %u\t", puschPtrs->ptrsFreqDensity);
   printf("ulPtrsPower = %u\t", puschPtrs->ulPtrsPower);

   printf("ptrsInfo:\t");
   for (uint8_t i = 0; i < puschPtrs->numPtrsPorts; i++) {
      printf("PTRS Info[%d]:\t", i);
      decode_fapi_ptrs_info(&puschPtrs->ptrsInfo[i]);
   }
}
void decode_fapi_dfts_ofdm(fapi_dfts_ofdm_t* dftsOfdm) 
{
   printf("\n");
   printf("lowPaprSequenceNumber = %u\t", dftsOfdm->lowPaprSequenceNumber);
   printf("lowPaprGroupNumber = %u\t", dftsOfdm->lowPaprGroupNumber);
   printf("ulPtrsSampleDensity = %u\t", dftsOfdm->ulPtrsSampleDensity);
   printf("ulPtrsTimeDensityTransformPrecoding = %u\t", dftsOfdm->ulPtrsTimeDensityTransformPrecoding);
}

void decode_fapi_rx_bfi(fapi_rx_bfi_t* rxBfi) 
{
   printf("\n");
   printf("beamIdx:\t");
   for (size_t i = 0; i < FAPI_MAX_NUM_DIGBFINTERFACES; i++) {
      printf("BeamIdx[%zu]:\t", i);
      decode_fapi_bmi(&rxBfi->beamIdx[i]);
   }
}

void decode_fapi_ul_rx_bmform_pdu(fapi_ul_rx_bmform_pdu_t* ulRxBmformPdu) 
{
   printf("\n");
   printf("numPrgs = %u\t", ulRxBmformPdu->numPrgs);
   printf("prgSize = %u\t", ulRxBmformPdu->prgSize);
   printf("digBfInterface = %u\t", ulRxBmformPdu->digBfInterface);

   printf("pad = [");
   for (size_t i = 0; i <3; i++) {
      printf("%u ", ulRxBmformPdu->pad[i]);
   }
   printf("rx_bfi:\t");
   for (size_t i = 0; i < ulRxBmformPdu->numPrgs; i++) {
      printf("RX BFI[%zu]:\t", i);
      decode_fapi_rx_bfi(&ulRxBmformPdu->rx_bfi[i]);
   }
}

void decode_fapi_ul_prach_pdu(fapi_ul_prach_pdu_t ulPrachPdu) 
{
   printf("\n");
   printf("physCellId = %u\t", ulPrachPdu.physCellId);
   printf("numPrachOcas = %u\t", ulPrachPdu.numPrachOcas);
   printf("prachFormat = %u\t", ulPrachPdu.prachFormat);
   printf("numRa = %u\t", ulPrachPdu.numRa);
   printf("prachStartSymbol = %u\t", ulPrachPdu.prachStartSymbol);
   printf("numCs = %u\t", ulPrachPdu.numCs);
   printf("beamforming:\t");
   decode_fapi_ul_rx_bmform_pdu(&ulPrachPdu.beamforming);
}

void decode_fapi_ul_pusch_pdu(fapi_ul_pusch_pdu_t* ulPuschPdu) 
{
   printf("\n");
   printf("pduBitMap = %u\t", ulPuschPdu->pduBitMap);
   printf("rnti = %u\t", ulPuschPdu->rnti);
   printf("handle = %u\t", ulPuschPdu->handle);
   printf("bwpSize = %u\t", ulPuschPdu->bwpSize);
   printf("bwpStart = %u\t", ulPuschPdu->bwpStart);
   printf("subCarrierSpacing = %u\t", ulPuschPdu->subCarrierSpacing);
   printf("cyclicPrefix = %u\t", ulPuschPdu->cyclicPrefix);
   printf("mcsIndex = %u\t", ulPuschPdu->mcsIndex);
   printf("mcsTable = %u\t", ulPuschPdu->mcsTable);
   printf("targetCodeRate = %u\t", ulPuschPdu->targetCodeRate);
   printf("qamModOrder = %u\t", ulPuschPdu->qamModOrder);
   printf("transformPrecoding = %u\t", ulPuschPdu->transformPrecoding);
   printf("dataScramblingId = %u\t", ulPuschPdu->dataScramblingId);
   printf("nrOfLayers = %u\t", ulPuschPdu->nrOfLayers);
   printf("dmrsConfigType = %u\t", ulPuschPdu->dmrsConfigType);
   printf("ulDmrsSymbPos = %u\t", ulPuschPdu->ulDmrsSymbPos);
   printf("ulDmrsScramblingId = %u\t", ulPuschPdu->ulDmrsScramblingId);
   printf("scid = %u\t", ulPuschPdu->scid);
   printf("numDmrsCdmGrpsNoData = %u\t", ulPuschPdu->numDmrsCdmGrpsNoData);
   printf("dmrsPorts = %u\t", ulPuschPdu->dmrsPorts);
   printf("nTpPuschId = %u\t", ulPuschPdu->nTpPuschId);
   printf("tpPi2Bpsk = %u\t", ulPuschPdu->tpPi2Bpsk);
   printf("rbBitmap = [");
   for (size_t i = 0; i < sizeof(ulPuschPdu->rbBitmap); i++) {
      printf("%u ", ulPuschPdu->rbBitmap[i]);
   }
   printf("]\t");
   printf("rbStart = %u\t", ulPuschPdu->rbStart);
   printf("rbSize = %u\t", ulPuschPdu->rbSize);
   printf("vrbToPrbMapping = %u\t", ulPuschPdu->vrbToPrbMapping);
   printf("frequencyHopping = %u\t", ulPuschPdu->frequencyHopping);
   printf("txDirectCurrentLocation = %u\t", ulPuschPdu->txDirectCurrentLocation);
   printf("resourceAlloc = %u\t", ulPuschPdu->resourceAlloc);
   printf("uplinkFrequencyShift7p5khz = %u\t", ulPuschPdu->uplinkFrequencyShift7p5khz);
   printf("startSymbIndex = %u\t", ulPuschPdu->startSymbIndex);
   printf("nrOfSymbols = %u\t", ulPuschPdu->nrOfSymbols);
   printf("mappingType = %u\t", ulPuschPdu->mappingType);
   printf("nrOfDmrsSymbols = %u\t", ulPuschPdu->nrOfDmrsSymbols);
   printf("dmrsAddPos = %u\t", ulPuschPdu->dmrsAddPos);

   // Check nested structures
   printf("puschData:\t");
   decode_fapi_pusch_data(&ulPuschPdu->puschData);
   printf("puschUci:\t");
   decode_fapi_pusch_uci(&ulPuschPdu->puschUci);
   printf("puschPtrs:\t");
   decode_fapi_pusch_ptrs(&ulPuschPdu->puschPtrs);
   printf("dftsOfdm:\t");
   decode_fapi_dfts_ofdm(&ulPuschPdu->dftsOfdm);
   printf("beamforming:\t");
   decode_fapi_ul_rx_bmform_pdu(&ulPuschPdu->beamforming);
}

void decode_fapi_ul_pucch_pdu(fapi_ul_pucch_pdu_t* ulPucchPdu) 
{
   printf("\n");
   printf("rnti = %u\t", ulPucchPdu->rnti);

   printf("pad1 = [");
   for (size_t i = 0; i < sizeof(ulPucchPdu->pad1); i++) {
      printf("%u ", ulPucchPdu->pad1[i]);
   }
   printf("]\t");

   printf("handle = %u\t", ulPucchPdu->handle);
   printf("bwpSize = %u\t", ulPucchPdu->bwpSize);
   printf("bwpStart = %u\t", ulPucchPdu->bwpStart);
   printf("subCarrierSpacing = %u\t", ulPucchPdu->subCarrierSpacing);
   printf("cyclicPrefix = %u\t", ulPucchPdu->cyclicPrefix);
   printf("formatType = %u\t", ulPucchPdu->formatType);
   printf("multiSlotTxIndicator = %u\t", ulPucchPdu->multiSlotTxIndicator);
   printf("pi2Bpsk = %u\t", ulPucchPdu->pi2Bpsk);
   printf("pad2 = %u\t", ulPucchPdu->pad2);
   printf("prbStart = %u\t", ulPucchPdu->prbStart);
   printf("prbSize = %u\t", ulPucchPdu->prbSize);
   printf("startSymbolIndex = %u\t", ulPucchPdu->startSymbolIndex);
   printf("nrOfSymbols = %u\t", ulPucchPdu->nrOfSymbols);
   printf("freqHopFlag = %u\t", ulPucchPdu->freqHopFlag);
   printf("groupHopFlag = %u\t", ulPucchPdu->groupHopFlag);
   printf("sequenceHopFlag = %u\t", ulPucchPdu->sequenceHopFlag);
   printf("pad3 = %u\t", ulPucchPdu->pad3);
   printf("secondHopPrb = %u\t", ulPucchPdu->secondHopPrb);
   printf("hoppingId = %u\t", ulPucchPdu->hoppingId);
   printf("initialCyclicShift = %u\t", ulPucchPdu->initialCyclicShift);
   printf("dataScramblingId = %u\t", ulPucchPdu->dataScramblingId);
   printf("timeDomainOccIdx = %u\t", ulPucchPdu->timeDomainOccIdx);
   printf("preDftOccIdx = %u\t", ulPucchPdu->preDftOccIdx);
   printf("preDftOccLen = %u\t", ulPucchPdu->preDftOccLen);
   printf("addDmrsFlag = %u\t", ulPucchPdu->addDmrsFlag);
   printf("dmrsScramblingId = %u\t", ulPucchPdu->dmrsScramblingId);
   printf("dmrsCyclicShift = %u\t", ulPucchPdu->dmrsCyclicShift);
   printf("srFlag = %u\t", ulPucchPdu->srFlag);
   printf("bitLenHarq = %u\t", ulPucchPdu->bitLenHarq);

   printf("pad4 = [");
   for (size_t i = 0; i < sizeof(ulPucchPdu->pad4); i++) {
      printf("%u ", ulPucchPdu->pad4[i]);
   }
   printf("]\t");

   printf("bitLenCsiPart1 = %u\t", ulPucchPdu->bitLenCsiPart1);
   printf("bitLenCsiPart2 = %u\t", ulPucchPdu->bitLenCsiPart2);

   // Check nested structure
   printf("beamforming:\t");
   decode_fapi_ul_rx_bmform_pdu(&ulPucchPdu->beamforming);
}
// Function to print the structure members

void decode_fapi_ul_srs_pdu(fapi_ul_srs_pdu_t* ulSrsPdu) 
{
   printf("\n");
   printf("rnti = %u\t", ulSrsPdu->rnti);
   printf("handle = %u\t", ulSrsPdu->handle);
   printf("bwpSize = %u\t", ulSrsPdu->bwpSize);
   printf("bwpStart = %u\t", ulSrsPdu->bwpStart);
   printf("subCarrierSpacing = %u\t", ulSrsPdu->subCarrierSpacing);
   printf("cyclicPrefix = %u\t", ulSrsPdu->cyclicPrefix);
   printf("numAntPorts = %u\t", ulSrsPdu->numAntPorts);
   printf("numSymbols = %u\t", ulSrsPdu->numSymbols);
   printf("numRepetitions = %u\t", ulSrsPdu->numRepetitions);
   printf("timeStartPosition = %u\t", ulSrsPdu->timeStartPosition);
   printf("configIndex = %u\t", ulSrsPdu->configIndex);
   printf("bandwidthIndex = %u\t", ulSrsPdu->bandwidthIndex);
   printf("sequenceId = %u\t", ulSrsPdu->sequenceId);
   printf("combSize = %u\t", ulSrsPdu->combSize);
   printf("combOffset = %u\t", ulSrsPdu->combOffset);
   printf("cyclicShift = %u\t", ulSrsPdu->cyclicShift);
   printf("frequencyPosition = %u\t", ulSrsPdu->frequencyPosition);
   printf("frequencyShift = %u\t", ulSrsPdu->frequencyShift);
   printf("frequencyHopping = %u\t", ulSrsPdu->frequencyHopping);
   printf("groupOrSequenceHopping = %u\t", ulSrsPdu->groupOrSequenceHopping);
   printf("resourceType = %u\t", ulSrsPdu->resourceType);
   printf("tSrs = %u\t", ulSrsPdu->tSrs);
   printf("tOffset = %u\t", ulSrsPdu->tOffset);
   decode_fapi_ul_rx_bmform_pdu(&ulSrsPdu->beamforming);
}

void decode_fapi_ul_tti_req_pdu(fapi_ul_tti_req_pdu_t* ulTtiReqPdu) 
{
   printf("\n");
   printf("pduType = %u\t", ulTtiReqPdu->pduType);
   printf("pduSize = %u\t", ulTtiReqPdu->pduSize);

   // Check each PDU type
   switch (ulTtiReqPdu->pduType) {
      case 0: // PRACH
         printf("PRACH PDU:\t");
         decode_fapi_ul_prach_pdu(ulTtiReqPdu->pdu.prach_pdu);
         break;
      case 1: // PUSCH
         printf("PUSCH PDU:\t");
         decode_fapi_ul_pusch_pdu(&ulTtiReqPdu->pdu.pusch_pdu);
         break;
      case 2: // PUCCH
         printf("PUCCH PDU:\t");
         decode_fapi_ul_pucch_pdu(&ulTtiReqPdu->pdu.pucch_pdu);
         break;
      case 3: // SRS
         printf("SRS PDU:\t");
         decode_fapi_ul_srs_pdu(&ulTtiReqPdu->pdu.srs_pdu);
         break;
      default:
         printf("ERROR: Unknown PDU type %u\t", ulTtiReqPdu->pduType);
   }
}

void decode_fapi_ul_tti_req(fapi_ul_tti_req_t* ulTtiReq) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("sfn = %u\t", ulTtiReq->sfn);
   printf("slot = %u\t", ulTtiReq->slot);
   printf("nPdus = %u\t", ulTtiReq->nPdus);
   printf("rachPresent = %u\t", ulTtiReq->rachPresent);
   printf("nUlsch = %u\t", ulTtiReq->nUlsch);
   printf("nUlcch = %u\t", ulTtiReq->nUlcch);
   printf("nGroup = %u\t", ulTtiReq->nGroup);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(ulTtiReq->pad); i++) {
      printf("%u ", ulTtiReq->pad[i]);
   }
   printf("]\t");

   printf("PDUs:\t");
   for (size_t i = 0; i < ulTtiReq->nPdus; i++) {
      printf("PDU[%zu]:\t", i);
      decode_fapi_ul_tti_req_pdu(&ulTtiReq->pdus[i]);
   }

   printf("UE Group Info:\t");
   for (size_t i = 0; i < ulTtiReq->nGroup; i++) {
      printf("UE Group Info[%zu]:\t", i);
      decode_fapi_ue_info(&ulTtiReq->ueGrpInfo[i]);
   }
}

void decode_fapi_dci_pdu(fapi_dci_pdu_t* dciPdu) 
{
   printf("\n");
   printf("pduType = %u\t", dciPdu->pduType);
   printf("pduSize = %u\t", dciPdu->pduSize);
   printf("pdcchPduConfig:\t");
   decode_fapi_dl_pdcch_pdu(&dciPdu->pdcchPduConfig);
}

void decode_fapi_ul_dci_req(fapi_ul_dci_req_t* ulDciReq) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("sfn = %u\t", ulDciReq->sfn);
   printf("slot = %u\t", ulDciReq->slot);
   printf("numPdus = %u\t", ulDciReq->numPdus);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(ulDciReq->pad); i++) {
      printf("%u ", ulDciReq->pad[i]);
   }
   printf("]\t");

   printf("PDUs:\t");
   for (size_t i = 0; i < ulDciReq->numPdus; i++) {
      printf("PDU[%zu]:\t", i);
      decode_fapi_dci_pdu(&ulDciReq->pdus[i]);
   }
}

void decode_fapi_tx_pdu_desc(fapi_tx_pdu_desc_t* txPduDesc) 
{
   printf("\n");
   printf("pdu_length = %u\t", txPduDesc->pdu_length);
   printf("pdu_index = %u\t", txPduDesc->pdu_index);
   printf("num_tlvs = %u\t", txPduDesc->num_tlvs);

   printf("TLVs:\t");
   for (size_t i = 0; i < txPduDesc->num_tlvs; i++) {
      printf("TLV[%zu]: { /* Details omitted */ }\t", i);
   }
}

void decode_fapi_tx_data_req(fapi_tx_data_req_t* txDataReq) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("sfn = %u\t", txDataReq->sfn);
   printf("slot = %u\t", txDataReq->slot);
   printf("num_pdus = %u\t", txDataReq->num_pdus);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(txDataReq->pad); i++) {
      printf("%u ", txDataReq->pad[i]);
   }
   printf("]\t");

   printf("PDU Descriptions:\t");
   for (size_t i = 0; i < txDataReq->num_pdus; i++) {
      printf("PDU Desc[%zu]:\t", i);
      decode_fapi_tx_pdu_desc(&txDataReq->pdu_desc[i]);
   }
}

void decode_fapi_pdu_ind_info(fapi_pdu_ind_info_t* pduIndInfo) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("handle = %u\t", pduIndInfo->handle);
   printf("rnti = %u\t", pduIndInfo->rnti);
   printf("harqId = %u\t", pduIndInfo->harqId);
   printf("ul_cqi = %u\t", pduIndInfo->ul_cqi);
   printf("timingAdvance = %u\t", pduIndInfo->timingAdvance);
   printf("rssi = %u\t", pduIndInfo->rssi);
   printf("pdu_length = %u\t", pduIndInfo->pdu_length);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(pduIndInfo->pad); i++) {
      printf("%u ", pduIndInfo->pad[i]);
   }
   printf("]\t");

   // Note: pduData is a void pointer; you need to know its type to print or inspect its content
   printf("pduData = %p\t", pduIndInfo->pduData);
}

void decode_fapi_rx_data_indication(fapi_rx_data_indication_t* rxDataInd) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("header = { /* Details omitted */ }\t");
   printf("sfn = %u\t", rxDataInd->sfn);
   printf("slot = %u\t", rxDataInd->slot);
   printf("numPdus = %u\t", rxDataInd->numPdus);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(rxDataInd->pad); i++) {
      printf("%u ", rxDataInd->pad[i]);
   }
   printf("]\t");

   printf("PDU Indication Info:\t");
   for (size_t i = 0; i < rxDataInd->numPdus; i++) {
      printf("PDU Ind[%zu]:\t", i);
      decode_fapi_pdu_ind_info(&rxDataInd->pdus[i]);
   }
}

void decode_fapi_crc_ind_info(fapi_crc_ind_info_t* crcIndInfo) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("handle = %u\t", crcIndInfo->handle);
   printf("rnti = %u\t", crcIndInfo->rnti);
   printf("harqId = %u\t", crcIndInfo->harqId);
   printf("tbCrcStatus = %u\t", crcIndInfo->tbCrcStatus);
   printf("ul_cqi = %u\t", crcIndInfo->ul_cqi);

   printf("pad = %u\t", crcIndInfo->pad);

   printf("numCb = %u\t", crcIndInfo->numCb);
   printf("timingAdvance = %u\t", crcIndInfo->timingAdvance);
   printf("rssi = %u\t", crcIndInfo->rssi);

   printf("cbCrcStatus:\t");
   for (size_t i = 0; i < sizeof(crcIndInfo->cbCrcStatus); i++) {
      printf("cbCrcStatus[%zu] = %u\t", i, crcIndInfo->cbCrcStatus[i]);
   }
}

void decode_fapi_crc_ind(fapi_crc_ind_t* crcInd) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("header = { /* Details omitted */ }\t");
   printf("sfn = %u\t", crcInd->sfn);
   printf("slot = %u\t", crcInd->slot);
   printf("numCrcs = %u\t", crcInd->numCrcs);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(crcInd->pad); i++) {
      printf("%u ", crcInd->pad[i]);
   }
   printf("]\t");

   printf("CRC Info:\t");
   for (size_t i = 0; i < crcInd->numCrcs; i++) {
      printf("CRC[%zu]:\t", i);
      decode_fapi_crc_ind_info(&crcInd->crc[i]);
   }
}

void decode_fapi_harq_info(fapi_harq_info_t* harqInfo) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("harqCrc = %u\t", harqInfo->harqCrc);

   printf("pad = %u\t", harqInfo->pad);

   printf("harqBitLen = %u\t", harqInfo->harqBitLen);
   printf("harqPayload: ");
   for (size_t i = 0; i < sizeof(harqInfo->harqPayload); i++) {
      printf("%u ", harqInfo->harqPayload[i]);
   }
}

void decode_fapi_csi_p1_info(fapi_csi_p1_info_t* csiP1Info) 
{

   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("csiPart1Crc = %u\t", csiP1Info->csiPart1Crc);
   printf("pad = %u\t", csiP1Info->pad);
   printf("csiPart1BitLen = %u\t", csiP1Info->csiPart1BitLen);
   printf("csiPart1Payload: ");
   for (size_t i = 0; i < sizeof(csiP1Info->csiPart1Payload); i++) {
      printf("%u ", csiP1Info->csiPart1Payload[i]);
   }
}

void decode_fapi_csi_p2_info(fapi_csi_p2_info_t* csiP2Info) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("csiPart2Crc = %u\t", csiP2Info->csiPart2Crc);
   printf("pad = %u\t", csiP2Info->pad);
   printf("csiPart2BitLen = %u\t", csiP2Info->csiPart2BitLen);
   printf("csiPart2Payload: ");
   for (size_t i = 0; i < sizeof(csiP2Info->csiPart2Payload); i++) {
      printf("%u ", csiP2Info->csiPart2Payload[i]);
   }
}

void decode_fapi_uci_o_pusch(fapi_uci_o_pusch_t* uciO_pusch) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("handle = %u\t", uciO_pusch->handle);
   printf("pduBitmap = %u\t", uciO_pusch->pduBitmap);
   printf("ul_cqi = %u\t", uciO_pusch->ul_cqi);
   printf("rnti = %u\t", uciO_pusch->rnti);
   printf("timingAdvance = %u\t", uciO_pusch->timingAdvance);
   printf("rssi = %u\t", uciO_pusch->rssi);

   // Print only if indicated by pduBitmap
   if (uciO_pusch->pduBitmap & 0x01) { // Example check for HARQ info
      printf("HARQ Info:\t");
      decode_fapi_harq_info(&uciO_pusch->harqInfo);
   }

   if (uciO_pusch->pduBitmap & 0x02) { // Example check for CSI Part 1 info
      printf("CSI Part 1 Info:\t");
      decode_fapi_csi_p1_info(&uciO_pusch->csiPart1info);
   }

   if (uciO_pusch->pduBitmap & 0x04) { // Example check for CSI Part 2 info
      printf("CSI Part 2 Info:\t");
      decode_fapi_csi_p2_info(&uciO_pusch->csiPart2info);
   }
}

void decode_fapi_sr_f0f1_info(fapi_sr_f0f1_info_t* srF0F1Info) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("srIndication = %u\t", srF0F1Info->srIndication);
   printf("srConfidenceLevel = %u\t", srF0F1Info->srConfidenceLevel);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(srF0F1Info->pad); i++) {
      printf("%u ", srF0F1Info->pad[i]);
   }
   printf("]\t");
}

void decode_fapi_harq_f0f1_info(fapi_harq_f0f1_info_t* harqF0F1Info) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("numHarq = %u\t", harqF0F1Info->numHarq);
   printf("harqConfidenceLevel = %u\t", harqF0F1Info->harqConfidenceLevel);
   printf("harqValue: ");
   for (size_t i = 0; i < sizeof(harqF0F1Info->harqValue); i++) {
      printf("%u ", harqF0F1Info->harqValue[i]);
   }
}

void decode_fapi_sr_f2f3f4_info(fapi_sr_f2f3f4_info_t* srF2F3F4Info) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("srBitlen = %u\t", srF2F3F4Info->srBitlen);
   printf("srPayload: ");
   for (size_t i = 0; i < sizeof(srF2F3F4Info->srPayload); i++) {
      printf("%u ", srF2F3F4Info->srPayload[i]);
   }
}

void decode_fapi_harq_f2f3f4_info(fapi_harq_f2f3f4_info_t* harqF2F3F4Info) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("harqCrc = %u\t", harqF2F3F4Info->harqCrc);
   printf("pad = %u\t", harqF2F3F4Info->pad);
   printf("harqBitLen = %u\t", harqF2F3F4Info->harqBitLen);
   printf("harqPayload: ");
   for (size_t i = 0; i < sizeof(harqF2F3F4Info->harqPayload); i++) {
      printf("%u ", harqF2F3F4Info->harqPayload[i]);
   }
}

void decode_fapi_uci_o_pucch_f2f3f4(fapi_uci_o_pucch_f2f3f4_t* uciO_pucch_f2f3f4) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("handle = %u\t", uciO_pucch_f2f3f4->handle);
   printf("pduBitmap = %u\t", uciO_pucch_f2f3f4->pduBitmap);
   printf("pucchFormat = %u\t", uciO_pucch_f2f3f4->pucchFormat);
   printf("ul_cqi = %u\t", uciO_pucch_f2f3f4->ul_cqi);
   printf("pad = %u\t", uciO_pucch_f2f3f4->pad);
   printf("rnti = %u\t", uciO_pucch_f2f3f4->rnti);
   printf("timingAdvance = %u\t", uciO_pucch_f2f3f4->timingAdvance);
   printf("rssi = %u\t", uciO_pucch_f2f3f4->rssi);
   printf("num_uci_bits = %u\t", uciO_pucch_f2f3f4->num_uci_bits);
   printf("uciBits: ");
   for (size_t i = 0; i < sizeof(uciO_pucch_f2f3f4->uciBits); i++) {
      printf("%u ", uciO_pucch_f2f3f4->uciBits[i]);
   }
}

void decode_fapi_uci_o_pucch_f0f1(fapi_uci_o_pucch_f0f1_t* uciO_pucch_f0f1) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("handle = %u\t", uciO_pucch_f0f1->handle);
   printf("pduBitmap = %u\t", uciO_pucch_f0f1->pduBitmap);
   printf("pucchFormat = %u\t", uciO_pucch_f0f1->pucchFormat);
   printf("ul_cqi = %u\t", uciO_pucch_f0f1->ul_cqi);
   printf("pad = %u\t", uciO_pucch_f0f1->pad);

   printf("rnti = %u\t", uciO_pucch_f0f1->rnti);
   printf("timingAdvance = %u\t", uciO_pucch_f0f1->timingAdvance);
   printf("rssi = %u\t", uciO_pucch_f0f1->rssi);

   printf("pad1 = [");
   for (size_t i = 0; i < sizeof(uciO_pucch_f0f1->pad1); i++) {
      printf("%u ", uciO_pucch_f0f1->pad1[i]);
   }
   printf("]\t");

   printf("srInfo: \t");
   decode_fapi_sr_f0f1_info(&uciO_pucch_f0f1->srInfo);

   printf("harqInfo: \t");
   decode_fapi_harq_f0f1_info(&uciO_pucch_f0f1->harqInfo);
}

void decode_fapi_uci_pdu_info(fapi_uci_pdu_info_t* uciPduInfo) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("pduType = %u\t", uciPduInfo->pduType);
   printf("pduSize = %u\t", uciPduInfo->pduSize);

   switch (uciPduInfo->pduType) {
      case 0: // Assuming 0 for uciPusch
         printf("uciPusch: \t");
         decode_fapi_uci_o_pusch(&uciPduInfo->uci.uciPusch);
         break;
      case 1: // Assuming 1 for uciPucchF0F1
         printf("uciPucchF0F1: \t");
         decode_fapi_uci_o_pucch_f0f1(&uciPduInfo->uci.uciPucchF0F1);
         break;
      case 2: // Assuming 2 for uciPucchF2F3F4
         printf("uciPucchF2F3F4: \t");
         decode_fapi_uci_o_pucch_f2f3f4(&uciPduInfo->uci.uciPucchF2F3F4);
         break;
      default:
         printf("Unknown pduType = %u\t", uciPduInfo->pduType);
         break;
   }
}

void decode_fapi_uci_indication(fapi_uci_indication_t* uciIndication) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("sfn = %u\t", uciIndication->sfn);
   printf("slot = %u\t", uciIndication->slot);
   printf("numUcis = %u\t", uciIndication->numUcis);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(uciIndication->pad); i++) {
      printf("%u ", uciIndication->pad[i]);
   }
   printf("]\t");

   for (size_t i = 0; i < uciIndication->numUcis; i++) {
      printf("uciPdu[%zu]: \t", i);
      decode_fapi_uci_pdu_info(&uciIndication->uciPdu[i]);
   }
}

void decode_fapi_symb_snr(fapi_symb_snr_t* symbSnr) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("numRbs = %u\t", symbSnr->numRbs);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(symbSnr->pad); i++) {
      printf("%u ", symbSnr->pad[i]);
   }
   printf("]\t");

   printf("rbSNR: ");
   for (size_t i = 0; i < sizeof(symbSnr->rbSNR); i++) {
      printf("%u ", symbSnr->rbSNR[i]);
   }
}

void decode_fapi_srs_pdu(fapi_srs_pdu_t* srsPdu) 
{
   printf("\n");
   printf("header = { /* Details omitted */ }\t");
   printf("handle = %u\t", srsPdu->handle);
   printf("rnti = %u\t", srsPdu->rnti);
   printf("timingAdvance = %u\t", srsPdu->timingAdvance);
   printf("numSymbols = %u\t", srsPdu->numSymbols);
   printf("wideBandSnr = %u\t", srsPdu->wideBandSnr);
   printf("numReportedSymbols = %u\t", srsPdu->numReportedSymbols);
   printf("pad = %u\t", srsPdu->pad);
   for (size_t i = 0; i < srsPdu->numReportedSymbols; i++) {
      printf("symbSnr[%zu]: \t", i);
      decode_fapi_symb_snr(&srsPdu->symbSnr[i]);
   }
}

void decode_fapi_srs_indication(fapi_srs_indication_t* srsIndication) 
{
   printf("\n");
   printf("sfn = %u\t", srsIndication->sfn);
   printf("slot = %u\t", srsIndication->slot);
   printf("numPdus = %u\t", srsIndication->numPdus);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(srsIndication->pad); i++) {
      printf("%u ", srsIndication->pad[i]);
   }
   printf("]\t");

   for (size_t i = 0; i < srsIndication->numPdus; i++) {
      printf("srsPdus[%zu]: \t", i);
      decode_fapi_srs_pdu(&srsIndication->srsPdus[i]);
   }
}

void decode_fapi_preamble_info(fapi_preamble_info_t* preambleInfo) 
{
   printf("\n");
   printf("preambleIndex = %u\t", preambleInfo->preambleIndex);
   printf("pad = %u\t", preambleInfo->pad);
   printf("timingAdvance = %u\t", preambleInfo->timingAdvance);
   printf("preamblePwr = %u\t", preambleInfo->preamblePwr);
}

void decode_fapi_rach_pdu(fapi_rach_pdu_t* rachPdu) 
{
   printf("\n");
   printf("phyCellId = %u\t", rachPdu->phyCellId);
   printf("symbolIndex = %u\t", rachPdu->symbolIndex);
   printf("slotIndex = %u\t", rachPdu->slotIndex);
   printf("freqIndex = %u\t", rachPdu->freqIndex);
   printf("avgRssi = %u\t", rachPdu->avgRssi);
   printf("avgSnr = %u\t", rachPdu->avgSnr);
   printf("numPreamble = %u\t", rachPdu->numPreamble);

   for (size_t i = 0; i < rachPdu->numPreamble; i++) {
      printf("preambleInfo[%zu]: \t", i);
      decode_fapi_preamble_info(&rachPdu->preambleInfo[i]);
   }
}

void decode_fapi_rach_indication(fapi_rach_indication_t* rachIndication) 
{
   printf("\n");
   printf("sfn = %u\t", rachIndication->sfn);
   printf("slot = %u\t", rachIndication->slot);
   printf("numPdus = %u\t", rachIndication->numPdus);

   printf("pad = [");
   for (size_t i = 0; i < sizeof(rachIndication->pad); i++) {
      printf("%u ", rachIndication->pad[i]);
   }
   printf("]\t");

   for (size_t i = 0; i < rachIndication->numPdus; i++) {
      printf("rachPdu[%zu]: \t", i);
      decode_fapi_rach_pdu(&rachIndication->rachPdu[i]);
   }
}
