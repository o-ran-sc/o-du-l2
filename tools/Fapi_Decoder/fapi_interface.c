#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "queue_elem.h"
#include <stdbool.h>
#include "/root/pborla/29July/l2/src/intel_fapi/fapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/fapi_vendor_extension.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nfapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nr5g_fapi_common_types.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nr5g_fapi_internal.h"

void decode_fapi_srs_pdu(fapi_srs_pdu_t* srsPdu);
void decode_fapi_msg_header(fapi_msg_header_t* header) {
    printf("DEBUG: num_msg = %u\n", header->num_msg);
    printf("DEBUG: handle = %u\n", header->handle);
}

void decode_fapi_msg(fapi_msg_t* msg) {
#ifdef OAI_TESTING
    printf("DEBUG: numMsg = %u\n", msg->numMsg);
    printf("DEBUG: opaque = %u\n", msg->opaque);
#endif
    printf("DEBUG: msg_id = %u\n", msg->msg_id);
    printf("DEBUG: length = %u\n", msg->length);
#ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", msg->pad);
#endif
}

void decode_fapi_tl(fapi_tl_t* tl) {
    printf("DEBUG: tag = %u\n", tl->tag);
    printf("DEBUG: length = %u\n", tl->length);
}


void decode_fapi_uint8_tlv(fapi_uint8_tlv_t* tlv) {
    printf("DEBUG: tl.tag = %u\n", tlv->tl.tag);
    printf("DEBUG: tl.length = %u\n", tlv->tl.length);
    printf("DEBUG: value = %u\n", tlv->value);
    printf("DEBUG: rsv[0] = %u\n", tlv->rsv[0]);
    printf("DEBUG: rsv[1] = %u\n", tlv->rsv[1]);
    printf("DEBUG: rsv[2] = %u\n", tlv->rsv[2]);
}

void decode_fapi_uint8_ptr_tlv(fapi_uint8_ptr_tlv_t* tlv) {
    printf("DEBUG: tl.tag = %u\n", tlv->tl.tag);
    printf("DEBUG: tl.length = %u\n", tlv->tl.length);

    if (tlv->value != NULL) {
        printf("DEBUG: value = ");
        for (size_t i = 0; i < 4; i++) { // Assuming 32-bit value, hence 4 bytes
            printf("%02X ", tlv->value[i]);
        }
        printf("\n");
    } else {
        printf("DEBUG: value is NULL\n");
    }
}

void decode_fapi_uint16_tlv(fapi_uint16_tlv_t* tlv) {
    printf("DEBUG: tl.tag = %u\n", tlv->tl.tag);
    printf("DEBUG: tl.length = %u\n", tlv->tl.length);
    printf("DEBUG: value = %u\n", tlv->value);
    printf("DEBUG: rsv[0] = %u\n", tlv->rsv[0]);
    printf("DEBUG: rsv[1] = %u\n", tlv->rsv[1]);
}
void decode_fapi_int16_tlv(fapi_int16_tlv_t* tlv) {
    printf("DEBUG: tl.tag = %u\n", tlv->tl.tag);
    printf("DEBUG: tl.length = %u\n", tlv->tl.length);
    printf("DEBUG: value = %d\n", tlv->value);
    printf("DEBUG: rsv[0] = %u\n", tlv->rsv[0]);
    printf("DEBUG: rsv[1] = %u\n", tlv->rsv[1]);
}

void decode_fapi_uint32_tlv(fapi_uint32_tlv_t * tlv) {
    printf("\nDEBUG: tl.tag = %u ", tlv->tl.tag);
    printf("DEBUG: tl.length = %u", tlv->tl.length);
    printf("DEBUG: value = %u ", tlv->value);
}

void decode_fapi_config_tlv(fapi_config_tlv_t* config) {
    printf("DEBUG: tag = %u\n", config->tag);
    printf("DEBUG: length = %u\n", config->length);
    printf("DEBUG: value = %u\n", config->value);
}

void decode_fapi_config_num_tlv(fapi_config_num_tlv_t* config_num) {
    printf("DEBUG: tl.tag = %u\n", config_num->tl.tag);
    printf("DEBUG: tl.length = %u\n", config_num->tl.length);
    for (size_t i = 0; i < FAPI_NUMEROLOGIES; i++) {
        printf("DEBUG: value[%zu] = %u\n", i, config_num->value[i]);
    }
    printf("DEBUG: rsv = %u\n", config_num->rsv);
}

void decode_fapi_param_req(fapi_param_req_t* param_req) {
    // Assuming fapi_msg_t header has its own check function
    decode_fapi_msg(&param_req->header);
}

void decode_fapi_cell_parms(fapi_cell_parms_t* cell_parms) {
    decode_fapi_uint16_tlv(&cell_parms->releaseCapability);
    decode_fapi_uint16_tlv(&cell_parms->phyState);
    decode_fapi_uint8_tlv(&cell_parms->skipBlankDlConfig);
    decode_fapi_uint8_tlv(&cell_parms->skipBlankUlConfig);
    decode_fapi_uint16_tlv(&cell_parms->numTlvsToReport);
    for (size_t i = 0; i < FAPI_MAX_NUM_TLVS_CONFIG; i++) {
        printf("DEBUG: tlvStatus[%zu].tag = %u\n", i, cell_parms->tlvStatus[i].tag);
        printf("DEBUG: tlvStatus[%zu].length = %u\n", i, cell_parms->tlvStatus[i].length);
    }
}

void decode_fapi_carrier_parms(fapi_carrier_parms_t* carrier_parms) {
    decode_fapi_uint8_tlv(&carrier_parms->cyclicPrefix);
    decode_fapi_uint8_tlv(&carrier_parms->supportedSubcarrierSpacingDl);
    decode_fapi_uint16_tlv(&carrier_parms->supportedBandwidthDl);
    decode_fapi_uint8_tlv(&carrier_parms->supportedSubcarrierSpecingsUl);
    decode_fapi_uint16_tlv(&carrier_parms->supportedBandwidthUl);
}

void decode_fapi_pdcch_parms(fapi_pdcch_parms_t* pdcch_parms) {
    decode_fapi_uint8_tlv(&pdcch_parms->cceMappingType);
    decode_fapi_uint8_tlv(&pdcch_parms->coresetOutsideFirst3OfdmSymsOfSlot);
    decode_fapi_uint8_tlv(&pdcch_parms->precoderGranularityCoreset);
    decode_fapi_uint8_tlv(&pdcch_parms->pdcchMuMimo);
    decode_fapi_uint8_tlv(&pdcch_parms->pdcchPrecoderCycling);
    decode_fapi_uint8_tlv(&pdcch_parms->maxPdcchsPerSlot);
}

void decode_fapi_pucch_parms(fapi_pucch_parms_t* pucch_parms) {
    decode_fapi_uint8_tlv(&pucch_parms->pucchFormats);
    decode_fapi_uint8_tlv(&pucch_parms->maxPucchsPerSlot);
}

void decode_fapi_pdsch_parms(fapi_pdsch_parms_t* pdsch_parms) {
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

void decode_fapi_pusch_parms(fapi_pusch_parms_t* pusch_parms) {
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

void decode_fapi_prach_parms(fapi_prach_parms_t* prach_parms) {
    decode_fapi_uint8_tlv(&prach_parms->prachLongFormats);
    decode_fapi_uint16_tlv(&prach_parms->prachShortFormats);
    decode_fapi_uint8_tlv(&prach_parms->prachRestrictedSets);
    decode_fapi_uint8_tlv(&prach_parms->maxPrachFdOccasionsInASlot);
}
void decode_fapi_meas_parms(fapi_meas_parms_t* meas_parms) {
    decode_fapi_uint8_tlv(&meas_parms->rssiMeasurementSupport);
}

void decode_fapi_params(fapi_params_t* params) {
    decode_fapi_cell_parms(&params->cell_parms);
    decode_fapi_carrier_parms(&params->carr_parms);
    decode_fapi_pdcch_parms(&params->pdcch_parms);
    decode_fapi_pucch_parms(&params->pucch_parms);
    decode_fapi_pdsch_parms(&params->pdsch_parms);
    decode_fapi_pusch_parms(&params->pusch_parms);
    decode_fapi_prach_parms(&params->prach_parms);
    decode_fapi_meas_parms(&params->meas_parms);
}

void decode_fapi_param_resp(fapi_param_resp_t* param_resp) {
    decode_fapi_msg(&param_resp->header);
    printf("DEBUG: error_code = %u\n", param_resp->error_code);
    printf("DEBUG: number_of_tlvs = %u\n", param_resp->number_of_tlvs);
    for (size_t i = 0; i < FAPI_MAX_NUM_TLVS_PARAMS; i++) {
        printf("DEBUG: tlvs[%zu].tl.tag = %u\n", i, param_resp->tlvs[i].tl.tag);
        printf("DEBUG: tlvs[%zu].tl.length = %u\n", i, param_resp->tlvs[i].tl.length);
    }
}

void decode_fapi_carrier_config(fapi_carrier_config_t* carrier_config) {
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

void decode_fapi_cell_config(fapi_cell_config_t* cell_config) {
    decode_fapi_uint8_tlv(&cell_config->phyCellId);
    decode_fapi_uint8_tlv(&cell_config->frameDuplexType);
}

void decode_fapi_ssb_config(fapi_ssb_config_t* ssb_config) {
    decode_fapi_uint32_tlv(&ssb_config->ssPbchPower);
    decode_fapi_uint8_tlv(&ssb_config->bchPayload);
    decode_fapi_uint8_tlv(&ssb_config->scsCommon);
}

void decode_fapi_prachFdOccasion(fapi_prachFdOccasion_t* prachFdOccasion) {
    decode_fapi_uint16_tlv(&prachFdOccasion->prachRootSequenceIndex);
    decode_fapi_uint8_tlv(&prachFdOccasion->numRootSequences);
    decode_fapi_uint16_tlv(&prachFdOccasion->k1);
    decode_fapi_uint8_tlv(&prachFdOccasion->prachZeroCorrConf);
    decode_fapi_uint16_tlv(&prachFdOccasion->numUnusedRootSequences);
    for (size_t i = 0; i < FAPI_MAX_NUM_UNUSED_ROOT_SEQUENCES; i++) {
        decode_fapi_uint16_tlv(&prachFdOccasion->unusedRootSequences[i]);
    }
}

void decode_fapi_prach_configuration(fapi_prach_configuration_t* prachConfig) {
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

void decode_fapi_ssb_table(fapi_ssb_table_t* ssbTable) {
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

void decode_fapi_slotconfig(fapi_slotconfig_t* slotConfig) {
    for (size_t i = 0; i < FAPI_MAX_NUM_OF_SYMBOLS_PER_SLOT; i++) {
        decode_fapi_uint8_tlv(&slotConfig->slotConfig[i]);
    }
}

void decode_fapi_tdd_table(fapi_tdd_table_t* tddTable) {
    decode_fapi_uint8_tlv(&tddTable->tddPeriod);
    for (size_t i = 0; i < FAPI_MAX_TDD_PERIODICITY; i++) {
        decode_fapi_slotconfig(&tddTable->slotConfig[i]);
    }
}

void decode_fapi_meas_config(fapi_meas_config_t* measConfig) {
    decode_fapi_uint8_tlv(&measConfig->rssiMeasurement);
}

void decode_fapi_dig_beam_weight(fapi_dig_beam_weight_t* digBeamWeight) {
    printf("DEBUG: digBeamWeightRe = %d\n", digBeamWeight->digBeamWeightRe);
    printf("DEBUG: digBeamWeightIm = %d\n", digBeamWeight->digBeamWeightIm);
}

void decode_fapi_dig_beam_config(fapi_dig_beam_config_t* digBeamConfig) {
    printf("DEBUG: beamIdx = %u\n", digBeamConfig->beamIdx);
    for (size_t i = 0; i < FAPI_MAX_NUMBER_TX_RUS; i++) {
        decode_fapi_dig_beam_weight(&digBeamConfig->digBeamWeight[i]);
    }
}

void decode_fapi_beamforming_table(fapi_beamforming_table_t* beamformingTable) {
    printf("DEBUG: numDigBeams = %u\n", beamformingTable->numDigBeams);
    printf("DEBUG: numTxRus = %u\n", beamformingTable->numTxRus);
    for (size_t i = 0; i < FAPI_MAX_NUMBER_OF_BEAMS; i++) {
        decode_fapi_dig_beam_config(&beamformingTable->digBeam[i]);
    }
}

void decode_fapi_precoderWeight_t(fapi_precoderWeight_t* precoder)
{

  printf("DEBUG: PrecoderRe = %u\n", precoder->preCoderWeightRe);
  printf("DEBUG: PrecoderIm = %u\n", precoder->preCoderWeightIm);
	
}
void decode_fapi_precoder_weight_t(fapi_precoder_weight_t* precoderWeight) {
    for (size_t i = 0; i < FAPI_MAX_NUM_ANT_PORTS; i++) {
        decode_fapi_precoderWeight_t(&precoderWeight->precoder_weight[i]);
    }
}

void decode_fapi_precoding_table(fapi_precoding_table_t* precodingTable) {
    printf("DEBUG: pmIdx = %u\n", precodingTable->pmIdx);
    printf("DEBUG: numLayers = %u\n", precodingTable->numLayers);
    printf("DEBUG: numAntPorts = %u\n", precodingTable->numAntPorts);
    for (size_t i = 0; i < FAPI_MAX_NUM_LAYERS; i++) {
        decode_fapi_precoder_weight_t(&precodingTable->precoderWeight[i]);
    }
}

void decode_fapi_config(fapi_config_t* config) {
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

void handleCfgReqtlv(fapi_uint32_tlv_t* tlv, fapi_config_t* config) {
    printf("\nDEBUG: tl.tag = %u\n", tlv->tl.tag);
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
            printf("DEBUG: Unsupported TLV tag: %u\n", tlv->tl.tag);
            break;
    }
}


void decode_fapi_config_req(fapi_config_req_t* configReq) 
{
    fapi_config_t config;
    
    decode_fapi_msg(&configReq->header);
    printf("DEBUG: number_of_tlvs = %u\n", configReq->number_of_tlvs);
    for (size_t i = 0; i < configReq->number_of_tlvs; i++) {
        decode_fapi_uint32_tlv(&configReq->tlvs[i]);
        handleCfgReqtlv(&configReq->tlvs[i], &config);
    }
    decode_fapi_config(&config);    
}

void decode_fapi_config_resp(fapi_config_resp_t* configResp) {
    decode_fapi_msg(&configResp->header);
    printf("DEBUG: error_code = %u\n", configResp->error_code);
    printf("DEBUG: number_of_invalid_tlvs = %u\n", configResp->number_of_invalid_tlvs);
    printf("DEBUG: number_of_inv_tlvs_idle_only = %u\n", configResp->number_of_inv_tlvs_idle_only);
    printf("DEBUG: number_of_inv_tlvs_running_only = %u\n", configResp->number_of_inv_tlvs_running_only);
    printf("DEBUG: number_of_missing_tlvs = %u\n", configResp->number_of_missing_tlvs);
    for (size_t i = 0; i < 4 * FAPI_MAX_NUM_TLVS_CONFIG; i++) {
        decode_fapi_uint16_tlv(&configResp->tlvs[i]);
    }
}

void decode_fapi_start_req(fapi_start_req_t* startReq) {
    decode_fapi_msg(&startReq->header);
}

void decode_fapi_stop_req(fapi_stop_req_t* stopReq) {
    decode_fapi_msg(&stopReq->header);
}

void decode_fapi_stop_ind(fapi_stop_ind_t* stopInd) {
    decode_fapi_msg(&stopInd->header);
}

void decode_fapi_error_ind(fapi_error_ind_t* errorInd) {
    decode_fapi_msg(&errorInd->header);
    printf("DEBUG: sfn = %u\n", errorInd->sfn);
    printf("DEBUG: slot = %u\n", errorInd->slot);
    printf("DEBUG: message_id = %u\n", errorInd->message_id);
    printf("DEBUG: error_code = %u\n", errorInd->error_code);
}

void decode_fapi_slot_ind(fapi_slot_ind_t* slotInd) {
    decode_fapi_msg(&slotInd->header);
    printf("DEBUG: sfn = %u\n", slotInd->sfn);
    printf("DEBUG: slot = %u\n", slotInd->slot);
}

void decode_fapi_bmi(fapi_bmi_t* bmi) {
    printf("DEBUG: beamidx = %u\n", bmi->beamidx);
}

void decode_fapi_pmi_bfi(fapi_pmi_bfi_t* pmiBfi) {
    printf("DEBUG: pmIdx = %u\n", pmiBfi->pmIdx);
    for (size_t i = 0; i < FAPI_MAX_NUM_DIGBFINTERFACES; i++) {
        decode_fapi_bmi(&pmiBfi->beamIdx[i]);
    }
}

#if 0
void decode_fapi_precoding_bmform(fapi_precoding_bmform_t precodingBmform) {
    printf("DEBUG: numPrgs = %u\n", precodingBmform.numPrgs);
    printf("DEBUG: prgSize = %u\n", precodingBmform.prgSize);
    printf("DEBUG: digBfInterfaces = %u\n", precodingBmform.digBfInterfaces);
    for (size_t i = 0; i < FAPI_MAX_NUM_PRGS_PER_TTI; i++) {
        decode_fapi_pmi_bfi(precodingBmform.pmi_bfi[i]);
    }
}

#endif

void decode_fapi_dl_dci(fapi_dl_dci_t* dlDci) {
    printf("DEBUG: rnti = %u\n", dlDci->rnti);
    printf("DEBUG: scramblingId = %u\n", dlDci->scramblingId);
    printf("DEBUG: scramblingRnti = %u\n", dlDci->scramblingRnti);
    printf("DEBUG: cceIndex = %u\n", dlDci->cceIndex);
    printf("DEBUG: aggregationLevel = %u\n", dlDci->aggregationLevel);
    decode_fapi_precoding_bmform(&dlDci->pc_and_bform);
    printf("DEBUG: beta_pdcch_1_0 = %u\n", dlDci->beta_pdcch_1_0);
    printf("DEBUG: powerControlOffsetSS = %u\n", dlDci->powerControlOffsetSS);
    printf("DEBUG: payloadSizeBits = %u\n", dlDci->payloadSizeBits);
    for (size_t i = 0; i < FAPI_DCI_PAYLOAD_BYTE_LEN; i++) {
        printf("DEBUG: payload[%zu] = %u\n", i, dlDci->payload[i]);
    }
}

void decode_fapi_dl_pdcch_pdu(fapi_dl_pdcch_pdu_t* dlPdcchPdu) {
    printf("DEBUG: bwpSize = %u\n", dlPdcchPdu->bwpSize);
    printf("DEBUG: bwpStart = %u\n", dlPdcchPdu->bwpStart);
    printf("DEBUG: subCarrierSpacing = %u\n", dlPdcchPdu->subCarrierSpacing);
    printf("DEBUG: cyclicPrefix = %u\n", dlPdcchPdu->cyclicPrefix);
    printf("DEBUG: startSymbolIndex = %u\n", dlPdcchPdu->startSymbolIndex);
    printf("DEBUG: durationSymbols = %u\n", dlPdcchPdu->durationSymbols);
    printf("DEBUG: freqDomainResource = ");
    for (size_t i = 0; i < sizeof(dlPdcchPdu->freqDomainResource); i++) {
        printf("%u ", dlPdcchPdu->freqDomainResource[i]);
    }
    printf("\n");
    printf("DEBUG: cceRegMappingType = %u\n", dlPdcchPdu->cceRegMappingType);
    printf("DEBUG: regBundleSize = %u\n", dlPdcchPdu->regBundleSize);
    printf("DEBUG: interleaverSize = %u\n", dlPdcchPdu->interleaverSize);
    printf("DEBUG: coreSetType = %u\n", dlPdcchPdu->coreSetType);
    printf("DEBUG: shiftIndex = %u\n", dlPdcchPdu->shiftIndex);
    printf("DEBUG: precoderGranularity = %u\n", dlPdcchPdu->precoderGranularity);
    printf("DEBUG: numDlDci = %u\n", dlPdcchPdu->numDlDci);
    for (size_t i = 0; i < dlPdcchPdu->numDlDci; i++) {
        decode_fapi_dl_dci(&dlPdcchPdu->dlDci[i]);
    }
}

void decode_fapi_codeword_pdu(fapi_codeword_pdu_t* codewordPdu) {
    printf("DEBUG: targetCodeRate = %u\n", codewordPdu->targetCodeRate);
    printf("DEBUG: qamModOrder = %u\n", codewordPdu->qamModOrder);
    printf("DEBUG: mcsIndex = %u\n", codewordPdu->mcsIndex);
    printf("DEBUG: mcsTable = %u\n", codewordPdu->mcsTable);
    printf("DEBUG: rvIndex = %u\n", codewordPdu->rvIndex);
    printf("DEBUG: tbSize = %u\n", codewordPdu->tbSize);
}

void decode_fapi_precoding_bmform(fapi_precoding_bmform_t* precodingBfForm) {
    printf("DEBUG: numPrgs = %u\n", precodingBfForm->numPrgs);
    printf("DEBUG: prgSize = %u\n", precodingBfForm->prgSize);
    printf("DEBUG: digBfInterfaces = %u\n", precodingBfForm->digBfInterfaces);
    for (size_t i = 0; i < FAPI_MAX_NUM_PRGS_PER_TTI; i++) {
        printf("DEBUG: pmi_bfi[%zu] pmIdx = %u\n", i, precodingBfForm->pmi_bfi[i].pmIdx);
        for (size_t j = 0; j < FAPI_MAX_NUM_DIGBFINTERFACES; j++) {
            printf("DEBUG: pmi_bfi[%zu] beamIdx[%zu] beamidx = %u\n", i, j, precodingBfForm->pmi_bfi[i].beamIdx[j].beamidx);
        }
    }
}

void decode_fapi_dl_pdsch_pdu(fapi_dl_pdsch_pdu_t* dlPdschPdu) {
    printf("DEBUG: pduBitMap = %u\n", dlPdschPdu->pduBitMap);
    printf("DEBUG: rnti = %u\n", dlPdschPdu->rnti);
    printf("DEBUG: pdu_index = %u\n", dlPdschPdu->pdu_index);
    printf("DEBUG: bwpSize = %u\n", dlPdschPdu->bwpSize);
    printf("DEBUG: bwpStart = %u\n", dlPdschPdu->bwpStart);
    printf("DEBUG: subCarrierSpacing = %u\n", dlPdschPdu->subCarrierSpacing);
    printf("DEBUG: cyclicPrefix = %u\n", dlPdschPdu->cyclicPrefix);
    printf("DEBUG: nrOfCodeWords = %u\n", dlPdschPdu->nrOfCodeWords);
    
    for (size_t i = 0; i < dlPdschPdu->nrOfCodeWords; i++) {
        printf("DEBUG: cwInfo[%zu]:\n", i);
        decode_fapi_codeword_pdu(&dlPdschPdu->cwInfo[i]);
    }
    
    printf("DEBUG: dataScramblingId = %u\n", dlPdschPdu->dataScramblingId);
    printf("DEBUG: nrOfLayers = %u\n", dlPdschPdu->nrOfLayers);
    printf("DEBUG: transmissionScheme = %u\n", dlPdschPdu->transmissionScheme);
    printf("DEBUG: refPoint = %u\n", dlPdschPdu->refPoint);
    printf("DEBUG: dmrsConfigType = %u\n", dlPdschPdu->dmrsConfigType);
    printf("DEBUG: dlDmrsSymbPos = %u\n", dlPdschPdu->dlDmrsSymbPos);
    printf("DEBUG: scid = %u\n", dlPdschPdu->scid);
    printf("DEBUG: numDmrsCdmGrpsNoData = %u\n", dlPdschPdu->numDmrsCdmGrpsNoData);
    printf("DEBUG: resourceAlloc = %u\n", dlPdschPdu->resourceAlloc);
    printf("DEBUG: dlDmrsScramblingId = %u\n", dlPdschPdu->dlDmrsScramblingId);
    printf("DEBUG: dmrsPorts = %u\n", dlPdschPdu->dmrsPorts);
    printf("DEBUG: rbStart = %u\n", dlPdschPdu->rbStart);
    printf("DEBUG: rbSize = %u\n", dlPdschPdu->rbSize);
    
    printf("DEBUG: rbBitmap = ");
    for (size_t i = 0; i < sizeof(dlPdschPdu->rbBitmap); i++) {
        printf("%u ", dlPdschPdu->rbBitmap[i]);
    }
    printf("\n");
    
    printf("DEBUG: vrbToPrbMapping = %u\n", dlPdschPdu->vrbToPrbMapping);
    printf("DEBUG: startSymbIndex = %u\n", dlPdschPdu->startSymbIndex);
    printf("DEBUG: nrOfSymbols = %u\n", dlPdschPdu->nrOfSymbols);
    printf("DEBUG: ptrsPortIndex = %u\n", dlPdschPdu->ptrsPortIndex);
    printf("DEBUG: ptrsTimeDensity = %u\n", dlPdschPdu->ptrsTimeDensity);
    printf("DEBUG: ptrsFreqDensity = %u\n", dlPdschPdu->ptrsFreqDensity);
    printf("DEBUG: ptrsReOffset = %u\n", dlPdschPdu->ptrsReOffset);
    printf("DEBUG: nEpreRatioOfPdschToPtrs = %u\n", dlPdschPdu->nEpreRatioOfPdschToPtrs);
    
    decode_fapi_precoding_bmform(&dlPdschPdu->preCodingAndBeamforming);
    
    printf("DEBUG: powerControlOffset = %u\n", dlPdschPdu->powerControlOffset);
    printf("DEBUG: powerControlOffsetSS = %u\n", dlPdschPdu->powerControlOffsetSS);
    printf("DEBUG: isLastCbPresent = %u\n", dlPdschPdu->isLastCbPresent);
    printf("DEBUG: isInlineTbCrc = %u\n", dlPdschPdu->isInlineTbCrc);
    printf("DEBUG: dlTbCrc = %u\n", dlPdschPdu->dlTbCrc);
    printf("DEBUG: mappingType = %u\n", dlPdschPdu->mappingType);
    printf("DEBUG: nrOfDmrsSymbols = %u\n", dlPdschPdu->nrOfDmrsSymbols);
    printf("DEBUG: dmrsAddPos = %u\n", dlPdschPdu->dmrsAddPos);
}

void decode_fapi_phy_mib_pdu(fapi_phy_mib_pdu_t* phyMibPdu) {
    printf("DEBUG: dmrsTypeAPosition = %u\n", phyMibPdu->dmrsTypeAPosition);
    printf("DEBUG: pdcchConfigSib1 = %u\n", phyMibPdu->pdcchConfigSib1);
    printf("DEBUG: cellBarred = %u\n", phyMibPdu->cellBarred);
    printf("DEBUG: intraFreqReselection = %u\n", phyMibPdu->intraFreqReselection);
}

void decode_fapi_bch_payload(fapi_bch_payload_t* bchPayload) {
    printf("DEBUG: bchPayload.bchPayload = %u\n", bchPayload->bchPayload);
    printf("DEBUG: bchPayload.phyMibPdu:\n");
    decode_fapi_phy_mib_pdu(&bchPayload->phyMibPdu);
}

void decode_fapi_dl_ssb_pdu(fapi_dl_ssb_pdu_t* dlSsbPdu) {
    printf("DEBUG: physCellId = %u\n", dlSsbPdu->physCellId);
    printf("DEBUG: betaPss = %u\n", dlSsbPdu->betaPss);
    printf("DEBUG: ssbBlockIndex = %u\n", dlSsbPdu->ssbBlockIndex);
    printf("DEBUG: ssbSubCarrierOffset = %u\n", dlSsbPdu->ssbSubCarrierOffset);
    printf("DEBUG: bchPayloadFlag = %u\n", dlSsbPdu->bchPayloadFlag);
    printf("DEBUG: ssbOffsetPointA = %u\n", dlSsbPdu->ssbOffsetPointA);
    printf("DEBUG: bchPayload:\n");
    decode_fapi_bch_payload(&dlSsbPdu->bchPayload);
    printf("DEBUG: preCodingAndBeamforming:\n");
    decode_fapi_precoding_bmform(&dlSsbPdu->preCodingAndBeamforming);
}

void decode_fapi_dl_csi_rs_pdu(const fapi_dl_csi_rs_pdu_t *pdu) {

     printf("DEBUG: bwpSize = %u\n", pdu->bwpSize);

     printf("DEBUG: bwpStart = %u\n", pdu->bwpStart);

     printf("DEBUG: subCarrierSpacing = %u\n", pdu->subCarrierSpacing);

     printf("DEBUG: cyclicPrefix = %u\n", pdu->cyclicPrefix);

     printf("DEBUG: startRb = %u\n", pdu->startRb);

     printf("DEBUG: nrOfRbs = %u\n", pdu->nrOfRbs);

     printf("DEBUG: csiType = %u\n", pdu->csiType);

     printf("DEBUG: row = %u\n", pdu->row);

     printf("DEBUG: freqDomain = %u\n", pdu->freqDomain);

     printf("DEBUG: symbL0 = %u\n", pdu->symbL0);

     printf("DEBUG: symbL1 = %u\n", pdu->symbL1);

     printf("DEBUG: cdmType = %u\n", pdu->cdmType);

     printf("DEBUG: freqDensity = %u\n", pdu->freqDensity);

     printf("DEBUG: scramId = %u\n", pdu->scramId);

     printf("DEBUG: powerControlOffset = %u\n", pdu->powerControlOffset);

     printf("DEBUG: powerControlOffsetSs = %u\n", pdu->powerControlOffsetSs);
   decode_fapi_precoding_bmform(&pdu->preCodingAndBeamforming);

 }

void decode_fapi_dl_tti_req_pdu(fapi_dl_tti_req_pdu_t* dlTtiReqPdu) {
    printf("DEBUG: pduType = %u\n", dlTtiReqPdu->pduType);
    printf("DEBUG: pduSize = %u\n", dlTtiReqPdu->pduSize);
    
    switch (dlTtiReqPdu->pduType) {
        case 0: // Example case for pdcch_pdu
            printf("DEBUG: pdcch_pdu:\n");
            decode_fapi_dl_pdcch_pdu(&dlTtiReqPdu->pdu.pdcch_pdu);
            break;
        case 1: // Example case for pdsch_pdu
            printf("DEBUG: pdsch_pdu:\n");
            decode_fapi_dl_pdsch_pdu(&dlTtiReqPdu->pdu.pdsch_pdu);
            break;
        case 2: // Example case for csi_rs_pdu
            printf("DEBUG: csi_rs_pdu:\n");
            decode_fapi_dl_csi_rs_pdu(&dlTtiReqPdu->pdu.csi_rs_pdu);
            break;
        case 3: // Example case for ssb_pdu
            printf("DEBUG: ssb_pdu:\n");
            decode_fapi_dl_ssb_pdu(&dlTtiReqPdu->pdu.ssb_pdu);
            break;
        default:
            printf("DEBUG: Unknown PDU type\n");
            break;
    }
}

void decode_fapi_ue_info(fapi_ue_info_t* ueInfo) {
    printf("DEBUG: nUe = %u\n", ueInfo->nUe);
    printf("DEBUG: pduIdx = ");
    for (size_t i = 0; i < sizeof(ueInfo->pduIdx); i++) {
        printf("%u ", ueInfo->pduIdx[i]);
    }
    printf("\n");
}

void decode_fapi_dl_tti_req(fapi_dl_tti_req_t* dlTtiReq) {
    printf("DEBUG: sfn = %u\n", dlTtiReq->sfn);
    printf("DEBUG: slot = %u\n", dlTtiReq->slot);
    printf("DEBUG: nPdus = %u\n", dlTtiReq->nPdus);
    printf("DEBUG: nGroup = %u\n", dlTtiReq->nGroup);

    printf("DEBUG: pdus:\n");
    for (size_t i = 0; i < dlTtiReq->nPdus; i++) {
        printf("DEBUG: PDUs[%zu]:\n", i);
        decode_fapi_dl_tti_req_pdu(&dlTtiReq->pdus[i]);
    }

    printf("DEBUG: ue_grp_info:\n");
    for (size_t i = 0; i < dlTtiReq->nGroup; i++) {
        printf("DEBUG: UE Group Info[%zu]:\n", i);
        decode_fapi_ue_info(&dlTtiReq->ue_grp_info[i]);
    }
}

void decode_fapi_pusch_data(fapi_pusch_data_t* puschData) {
    printf("DEBUG: rvIndex = %u\n", puschData->rvIndex);
    printf("DEBUG: harqProcessId = %u\n", puschData->harqProcessId);
    printf("DEBUG: newDataIndicator = %u\n", puschData->newDataIndicator);
    printf("DEBUG: tbSize = %u\n", puschData->tbSize);
    printf("DEBUG: numCb = %u\n", puschData->numCb);
    printf("DEBUG: cbPresentAndPosition = ");
    for (size_t i = 0; i < sizeof(puschData->cbPresentAndPosition); i++) {
        printf("%u ", puschData->cbPresentAndPosition[i]);
    }
    printf("\n");
}

void decode_fapi_pusch_uci(fapi_pusch_uci_t* puschUci) {
    printf("DEBUG: harqAckBitLength = %u\n", puschUci->harqAckBitLength);
    printf("DEBUG: csiPart1BitLength = %u\n", puschUci->csiPart1BitLength);
    printf("DEBUG: csiPart2BitLength = %u\n", puschUci->csiPart2BitLength);
    printf("DEBUG: alphaScaling = %u\n", puschUci->alphaScaling);
    printf("DEBUG: betaOffsetHarqAck = %u\n", puschUci->betaOffsetHarqAck);
    printf("DEBUG: betaOffsetCsi1 = %u\n", puschUci->betaOffsetCsi1);
    printf("DEBUG: betaOffsetCsi2 = %u\n", puschUci->betaOffsetCsi2);
}

void decode_fapi_ptrs_info(fapi_ptrs_info_t* ptrsInfo) {
    printf("DEBUG: ptrsPortIndex = %u\n", ptrsInfo->ptrsPortIndex);
    printf("DEBUG: ptrsDmrsPort = %u\n", ptrsInfo->ptrsDmrsPort);
    printf("DEBUG: ptrsReOffset = %u\n", ptrsInfo->ptrsReOffset);
}

void decode_fapi_pusch_ptrs(fapi_pusch_ptrs_t* puschPtrs) {
    printf("DEBUG: numPtrsPorts = %u\n", puschPtrs->numPtrsPorts);
    printf("DEBUG: ptrsTimeDensity = %u\n", puschPtrs->ptrsTimeDensity);
    printf("DEBUG: ptrsFreqDensity = %u\n", puschPtrs->ptrsFreqDensity);
    printf("DEBUG: ulPtrsPower = %u\n", puschPtrs->ulPtrsPower);

    printf("DEBUG: ptrsInfo:\n");
    for (size_t i = 0; i < puschPtrs->numPtrsPorts; i++) {
        printf("DEBUG: PTRS Info[%zu]:\n", i);
        decode_fapi_ptrs_info(&puschPtrs->ptrsInfo[i]);
    }
}
void decode_fapi_dfts_ofdm(fapi_dfts_ofdm_t* dftsOfdm) {
    printf("DEBUG: lowPaprSequenceNumber = %u\n", dftsOfdm->lowPaprSequenceNumber);
    printf("DEBUG: lowPaprGroupNumber = %u\n", dftsOfdm->lowPaprGroupNumber);
    printf("DEBUG: ulPtrsSampleDensity = %u\n", dftsOfdm->ulPtrsSampleDensity);
    printf("DEBUG: ulPtrsTimeDensityTransformPrecoding = %u\n", dftsOfdm->ulPtrsTimeDensityTransformPrecoding);
}

void decode_fapi_rx_bfi(fapi_rx_bfi_t* rxBfi) {
    printf("DEBUG: beamIdx:\n");
    for (size_t i = 0; i < FAPI_MAX_NUM_DIGBFINTERFACES; i++) {
        printf("DEBUG: BeamIdx[%zu]:\n", i);
        decode_fapi_bmi(&rxBfi->beamIdx[i]);
    }
}

void decode_fapi_ul_rx_bmform_pdu(fapi_ul_rx_bmform_pdu_t* ulRxBmformPdu) {
    printf("DEBUG: numPrgs = %u\n", ulRxBmformPdu->numPrgs);
    printf("DEBUG: prgSize = %u\n", ulRxBmformPdu->prgSize);
    printf("DEBUG: digBfInterface = %u\n", ulRxBmformPdu->digBfInterface);

    printf("DEBUG: rx_bfi:\n");
    for (size_t i = 0; i < ulRxBmformPdu->numPrgs; i++) {
        printf("DEBUG: RX BFI[%zu]:\n", i);
        decode_fapi_rx_bfi(&ulRxBmformPdu->rx_bfi[i]);
    }
}

void decode_fapi_ul_prach_pdu(fapi_ul_prach_pdu_t ulPrachPdu) {
    printf("DEBUG: physCellId = %u\n", ulPrachPdu.physCellId);
    printf("DEBUG: numPrachOcas = %u\n", ulPrachPdu.numPrachOcas);
    printf("DEBUG: prachFormat = %u\n", ulPrachPdu.prachFormat);
    printf("DEBUG: numRa = %u\n", ulPrachPdu.numRa);
    printf("DEBUG: prachStartSymbol = %u\n", ulPrachPdu.prachStartSymbol);
    printf("DEBUG: numCs = %u\n", ulPrachPdu.numCs);
    printf("DEBUG: beamforming:\n");
    decode_fapi_ul_rx_bmform_pdu(&ulPrachPdu.beamforming);
}

void decode_fapi_ul_pusch_pdu(fapi_ul_pusch_pdu_t* ulPuschPdu) {
    printf("DEBUG: pduBitMap = %u\n", ulPuschPdu->pduBitMap);
    printf("DEBUG: rnti = %u\n", ulPuschPdu->rnti);
    printf("DEBUG: handle = %u\n", ulPuschPdu->handle);
    printf("DEBUG: bwpSize = %u\n", ulPuschPdu->bwpSize);
    printf("DEBUG: bwpStart = %u\n", ulPuschPdu->bwpStart);
    printf("DEBUG: subCarrierSpacing = %u\n", ulPuschPdu->subCarrierSpacing);
    printf("DEBUG: cyclicPrefix = %u\n", ulPuschPdu->cyclicPrefix);
    printf("DEBUG: mcsIndex = %u\n", ulPuschPdu->mcsIndex);
    printf("DEBUG: mcsTable = %u\n", ulPuschPdu->mcsTable);
    printf("DEBUG: targetCodeRate = %u\n", ulPuschPdu->targetCodeRate);
    printf("DEBUG: qamModOrder = %u\n", ulPuschPdu->qamModOrder);
    printf("DEBUG: transformPrecoding = %u\n", ulPuschPdu->transformPrecoding);
    printf("DEBUG: dataScramblingId = %u\n", ulPuschPdu->dataScramblingId);
    printf("DEBUG: nrOfLayers = %u\n", ulPuschPdu->nrOfLayers);
    printf("DEBUG: dmrsConfigType = %u\n", ulPuschPdu->dmrsConfigType);
    printf("DEBUG: ulDmrsSymbPos = %u\n", ulPuschPdu->ulDmrsSymbPos);
    printf("DEBUG: ulDmrsScramblingId = %u\n", ulPuschPdu->ulDmrsScramblingId);
    printf("DEBUG: scid = %u\n", ulPuschPdu->scid);
    printf("DEBUG: numDmrsCdmGrpsNoData = %u\n", ulPuschPdu->numDmrsCdmGrpsNoData);
    printf("DEBUG: dmrsPorts = %u\n", ulPuschPdu->dmrsPorts);
    printf("DEBUG: nTpPuschId = %u\n", ulPuschPdu->nTpPuschId);
    printf("DEBUG: tpPi2Bpsk = %u\n", ulPuschPdu->tpPi2Bpsk);
    printf("DEBUG: rbBitmap = [");
    for (size_t i = 0; i < sizeof(ulPuschPdu->rbBitmap); i++) {
        printf("%u ", ulPuschPdu->rbBitmap[i]);
    }
    printf("]\n");
    printf("DEBUG: rbStart = %u\n", ulPuschPdu->rbStart);
    printf("DEBUG: rbSize = %u\n", ulPuschPdu->rbSize);
    printf("DEBUG: vrbToPrbMapping = %u\n", ulPuschPdu->vrbToPrbMapping);
    printf("DEBUG: frequencyHopping = %u\n", ulPuschPdu->frequencyHopping);
    printf("DEBUG: txDirectCurrentLocation = %u\n", ulPuschPdu->txDirectCurrentLocation);
    printf("DEBUG: resourceAlloc = %u\n", ulPuschPdu->resourceAlloc);
    printf("DEBUG: uplinkFrequencyShift7p5khz = %u\n", ulPuschPdu->uplinkFrequencyShift7p5khz);
    printf("DEBUG: startSymbIndex = %u\n", ulPuschPdu->startSymbIndex);
    printf("DEBUG: nrOfSymbols = %u\n", ulPuschPdu->nrOfSymbols);
    printf("DEBUG: mappingType = %u\n", ulPuschPdu->mappingType);
    printf("DEBUG: nrOfDmrsSymbols = %u\n", ulPuschPdu->nrOfDmrsSymbols);
    printf("DEBUG: dmrsAddPos = %u\n", ulPuschPdu->dmrsAddPos);

    // Check nested structures
    printf("DEBUG: puschData:\n");
    decode_fapi_pusch_data(&ulPuschPdu->puschData);
    printf("DEBUG: puschUci:\n");
    decode_fapi_pusch_uci(&ulPuschPdu->puschUci);
    printf("DEBUG: puschPtrs:\n");
    decode_fapi_pusch_ptrs(&ulPuschPdu->puschPtrs);
    printf("DEBUG: dftsOfdm:\n");
    decode_fapi_dfts_ofdm(&ulPuschPdu->dftsOfdm);
    printf("DEBUG: beamforming:\n");
    decode_fapi_ul_rx_bmform_pdu(&ulPuschPdu->beamforming);
}
void decode_fapi_ul_pucch_pdu(fapi_ul_pucch_pdu_t* ulPucchPdu) {
    printf("DEBUG: rnti = %u\n", ulPucchPdu->rnti);

    #ifndef OAI_TESTING
    printf("DEBUG: pad1 = [");
    for (size_t i = 0; i < sizeof(ulPucchPdu->pad1); i++) {
        printf("%u ", ulPucchPdu->pad1[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: handle = %u\n", ulPucchPdu->handle);
    printf("DEBUG: bwpSize = %u\n", ulPucchPdu->bwpSize);
    printf("DEBUG: bwpStart = %u\n", ulPucchPdu->bwpStart);
    printf("DEBUG: subCarrierSpacing = %u\n", ulPucchPdu->subCarrierSpacing);
    printf("DEBUG: cyclicPrefix = %u\n", ulPucchPdu->cyclicPrefix);
    printf("DEBUG: formatType = %u\n", ulPucchPdu->formatType);
    printf("DEBUG: multiSlotTxIndicator = %u\n", ulPucchPdu->multiSlotTxIndicator);
    printf("DEBUG: pi2Bpsk = %u\n", ulPucchPdu->pi2Bpsk);

    #ifndef OAI_TESTING
    printf("DEBUG: pad2 = %u\n", ulPucchPdu->pad2);
    #endif

    printf("DEBUG: prbStart = %u\n", ulPucchPdu->prbStart);
    printf("DEBUG: prbSize = %u\n", ulPucchPdu->prbSize);
    printf("DEBUG: startSymbolIndex = %u\n", ulPucchPdu->startSymbolIndex);
    printf("DEBUG: nrOfSymbols = %u\n", ulPucchPdu->nrOfSymbols);
    printf("DEBUG: freqHopFlag = %u\n", ulPucchPdu->freqHopFlag);
    printf("DEBUG: groupHopFlag = %u\n", ulPucchPdu->groupHopFlag);
    printf("DEBUG: sequenceHopFlag = %u\n", ulPucchPdu->sequenceHopFlag);

    #ifndef OAI_TESTING
    printf("DEBUG: pad3 = %u\n", ulPucchPdu->pad3);
    #endif

    printf("DEBUG: secondHopPrb = %u\n", ulPucchPdu->secondHopPrb);
    printf("DEBUG: hoppingId = %u\n", ulPucchPdu->hoppingId);
    printf("DEBUG: initialCyclicShift = %u\n", ulPucchPdu->initialCyclicShift);
    printf("DEBUG: dataScramblingId = %u\n", ulPucchPdu->dataScramblingId);
    printf("DEBUG: timeDomainOccIdx = %u\n", ulPucchPdu->timeDomainOccIdx);
    printf("DEBUG: preDftOccIdx = %u\n", ulPucchPdu->preDftOccIdx);
    printf("DEBUG: preDftOccLen = %u\n", ulPucchPdu->preDftOccLen);
    printf("DEBUG: addDmrsFlag = %u\n", ulPucchPdu->addDmrsFlag);
    printf("DEBUG: dmrsScramblingId = %u\n", ulPucchPdu->dmrsScramblingId);
    printf("DEBUG: dmrsCyclicShift = %u\n", ulPucchPdu->dmrsCyclicShift);
    printf("DEBUG: srFlag = %u\n", ulPucchPdu->srFlag);
    printf("DEBUG: bitLenHarq = %u\n", ulPucchPdu->bitLenHarq);

    #ifndef OAI_TESTING
    printf("DEBUG: pad4 = [");
    for (size_t i = 0; i < sizeof(ulPucchPdu->pad4); i++) {
        printf("%u ", ulPucchPdu->pad4[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: bitLenCsiPart1 = %u\n", ulPucchPdu->bitLenCsiPart1);
    printf("DEBUG: bitLenCsiPart2 = %u\n", ulPucchPdu->bitLenCsiPart2);

    // Check nested structure
    printf("DEBUG: beamforming:\n");
    decode_fapi_ul_rx_bmform_pdu(&ulPucchPdu->beamforming);
}
// Function to print the structure members

 void decode_fapi_ul_srs_pdu(fapi_ul_srs_pdu_t* ulSrsPdu) {

     printf("DEBUG: rnti = %u\n", ulSrsPdu->rnti);

     printf("DEBUG: handle = %u\n", ulSrsPdu->handle);

     printf("DEBUG: bwpSize = %u\n", ulSrsPdu->bwpSize);

     printf("DEBUG: bwpStart = %u\n", ulSrsPdu->bwpStart);

     printf("DEBUG: subCarrierSpacing = %u\n", ulSrsPdu->subCarrierSpacing);

     printf("DEBUG: cyclicPrefix = %u\n", ulSrsPdu->cyclicPrefix);

     printf("DEBUG: numAntPorts = %u\n", ulSrsPdu->numAntPorts);

     printf("DEBUG: numSymbols = %u\n", ulSrsPdu->numSymbols);

     printf("DEBUG: numRepetitions = %u\n", ulSrsPdu->numRepetitions);

     printf("DEBUG: timeStartPosition = %u\n", ulSrsPdu->timeStartPosition);

     printf("DEBUG: configIndex = %u\n", ulSrsPdu->configIndex);

     printf("DEBUG: bandwidthIndex = %u\n", ulSrsPdu->bandwidthIndex);

     printf("DEBUG: sequenceId = %u\n", ulSrsPdu->sequenceId);

     printf("DEBUG: combSize = %u\n", ulSrsPdu->combSize);

     printf("DEBUG: combOffset = %u\n", ulSrsPdu->combOffset);

     printf("DEBUG: cyclicShift = %u\n", ulSrsPdu->cyclicShift);

     printf("DEBUG: frequencyPosition = %u\n", ulSrsPdu->frequencyPosition);

     printf("DEBUG: frequencyShift = %u\n", ulSrsPdu->frequencyShift);

     printf("DEBUG: frequencyHopping = %u\n", ulSrsPdu->frequencyHopping);

     printf("DEBUG: groupOrSequenceHopping = %u\n", ulSrsPdu->groupOrSequenceHopping);

     printf("DEBUG: resourceType = %u\n", ulSrsPdu->resourceType);

     printf("DEBUG: tSrs = %u\n", ulSrsPdu->tSrs);

     printf("DEBUG: tOffset = %u\n", ulSrsPdu->tOffset);

     // Add print statements for beamforming if needed

 }
void decode_fapi_ul_tti_req_pdu(fapi_ul_tti_req_pdu_t* ulTtiReqPdu) {
    printf("DEBUG: pduType = %u\n", ulTtiReqPdu->pduType);
    printf("DEBUG: pduSize = %u\n", ulTtiReqPdu->pduSize);

    // Check each PDU type
    switch (ulTtiReqPdu->pduType) {
        case 0: // PRACH
            printf("DEBUG: PRACH PDU:\n");
            decode_fapi_ul_prach_pdu(ulTtiReqPdu->pdu.prach_pdu);
            break;
        case 1: // PUSCH
            printf("DEBUG: PUSCH PDU:\n");
            decode_fapi_ul_pusch_pdu(&ulTtiReqPdu->pdu.pusch_pdu);
            break;
        case 2: // PUCCH
            printf("DEBUG: PUCCH PDU:\n");
            decode_fapi_ul_pucch_pdu(&ulTtiReqPdu->pdu.pucch_pdu);
            break;
        case 3: // SRS
            printf("DEBUG: SRS PDU:\n");
            decode_fapi_ul_srs_pdu(&ulTtiReqPdu->pdu.srs_pdu);
            break;
        default:
            printf("ERROR: Unknown PDU type %u\n", ulTtiReqPdu->pduType);
    }
}

void decode_fapi_ul_tti_req(fapi_ul_tti_req_t* ulTtiReq) {
    printf("DEBUG: header = { /* Details omitted */ }\n");
    printf("DEBUG: sfn = %u\n", ulTtiReq->sfn);
    printf("DEBUG: slot = %u\n", ulTtiReq->slot);
    printf("DEBUG: nPdus = %u\n", ulTtiReq->nPdus);
    printf("DEBUG: rachPresent = %u\n", ulTtiReq->rachPresent);
    printf("DEBUG: nUlsch = %u\n", ulTtiReq->nUlsch);
    printf("DEBUG: nUlcch = %u\n", ulTtiReq->nUlcch);
    printf("DEBUG: nGroup = %u\n", ulTtiReq->nGroup);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(ulTtiReq->pad); i++) {
        printf("%u ", ulTtiReq->pad[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: PDUs:\n");
    for (size_t i = 0; i < ulTtiReq->nPdus; i++) {
        printf("DEBUG: PDU[%zu]:\n", i);
        decode_fapi_ul_tti_req_pdu(&ulTtiReq->pdus[i]);
    }

    printf("DEBUG: UE Group Info:\n");
    for (size_t i = 0; i < ulTtiReq->nGroup; i++) {
        printf("DEBUG: UE Group Info[%zu]:\n", i);
        decode_fapi_ue_info(&ulTtiReq->ueGrpInfo[i]);
    }
}

void decode_fapi_dci_pdu(fapi_dci_pdu_t* dciPdu) {
    printf("DEBUG: pduType = %u\n", dciPdu->pduType);
    printf("DEBUG: pduSize = %u\n", dciPdu->pduSize);
    printf("DEBUG: pdcchPduConfig:\n");
    decode_fapi_dl_pdcch_pdu(&dciPdu->pdcchPduConfig);
}

void decode_fapi_ul_dci_req(fapi_ul_dci_req_t* ulDciReq) {
    printf("DEBUG: header = { /* Details omitted */ }\n");
    printf("DEBUG: sfn = %u\n", ulDciReq->sfn);
    printf("DEBUG: slot = %u\n", ulDciReq->slot);
    printf("DEBUG: numPdus = %u\n", ulDciReq->numPdus);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(ulDciReq->pad); i++) {
        printf("%u ", ulDciReq->pad[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: PDUs:\n");
    for (size_t i = 0; i < ulDciReq->numPdus; i++) {
        printf("DEBUG: PDU[%zu]:\n", i);
        decode_fapi_dci_pdu(&ulDciReq->pdus[i]);
    }
}

void decode_fapi_tx_pdu_desc(fapi_tx_pdu_desc_t* txPduDesc) {
    printf("DEBUG: pdu_length = %u\n", txPduDesc->pdu_length);
    printf("DEBUG: pdu_index = %u\n", txPduDesc->pdu_index);
    printf("DEBUG: num_tlvs = %u\n", txPduDesc->num_tlvs);

    printf("DEBUG: TLVs:\n");
    for (size_t i = 0; i < txPduDesc->num_tlvs; i++) {
        printf("DEBUG: TLV[%zu]: { /* Details omitted */ }\n", i);
    }
}

void decode_fapi_tx_data_req(fapi_tx_data_req_t* txDataReq) {
    printf("DEBUG: header = { /* Details omitted */ }\n");
    printf("DEBUG: sfn = %u\n", txDataReq->sfn);
    printf("DEBUG: slot = %u\n", txDataReq->slot);
    printf("DEBUG: num_pdus = %u\n", txDataReq->num_pdus);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(txDataReq->pad); i++) {
        printf("%u ", txDataReq->pad[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: PDU Descriptions:\n");
    for (size_t i = 0; i < txDataReq->num_pdus; i++) {
        printf("DEBUG: PDU Desc[%zu]:\n", i);
        decode_fapi_tx_pdu_desc(&txDataReq->pdu_desc[i]);
    }
}

void decode_fapi_pdu_ind_info(fapi_pdu_ind_info_t* pduIndInfo) {
    printf("DEBUG: handle = %u\n", pduIndInfo->handle);
    printf("DEBUG: rnti = %u\n", pduIndInfo->rnti);
    printf("DEBUG: harqId = %u\n", pduIndInfo->harqId);
    printf("DEBUG: ul_cqi = %u\n", pduIndInfo->ul_cqi);
    printf("DEBUG: timingAdvance = %u\n", pduIndInfo->timingAdvance);
    printf("DEBUG: rssi = %u\n", pduIndInfo->rssi);
    printf("DEBUG: pdu_length = %u\n", pduIndInfo->pdu_length);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(pduIndInfo->pad); i++) {
        printf("%u ", pduIndInfo->pad[i]);
    }
    printf("]\n");
    #endif

    // Note: pduData is a void pointer; you need to know its type to print or inspect its content
    printf("DEBUG: pduData = %p\n", pduIndInfo->pduData);
}

void decode_fapi_rx_data_indication(fapi_rx_data_indication_t* rxDataInd) {
    printf("DEBUG: header = { /* Details omitted */ }\n");
    printf("DEBUG: sfn = %u\n", rxDataInd->sfn);
    printf("DEBUG: slot = %u\n", rxDataInd->slot);
    printf("DEBUG: numPdus = %u\n", rxDataInd->numPdus);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(rxDataInd->pad); i++) {
        printf("%u ", rxDataInd->pad[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: PDU Indication Info:\n");
    for (size_t i = 0; i < rxDataInd->numPdus; i++) {
        printf("DEBUG: PDU Ind[%zu]:\n", i);
        decode_fapi_pdu_ind_info(&rxDataInd->pdus[i]);
    }
}

void decode_fapi_crc_ind_info(fapi_crc_ind_info_t* crcIndInfo) {
    printf("DEBUG: handle = %u\n", crcIndInfo->handle);
    printf("DEBUG: rnti = %u\n", crcIndInfo->rnti);
    printf("DEBUG: harqId = %u\n", crcIndInfo->harqId);
    printf("DEBUG: tbCrcStatus = %u\n", crcIndInfo->tbCrcStatus);
    printf("DEBUG: ul_cqi = %u\n", crcIndInfo->ul_cqi);
    
    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", crcIndInfo->pad);
    #endif

    printf("DEBUG: numCb = %u\n", crcIndInfo->numCb);
    printf("DEBUG: timingAdvance = %u\n", crcIndInfo->timingAdvance);
    printf("DEBUG: rssi = %u\n", crcIndInfo->rssi);
    
    printf("DEBUG: cbCrcStatus:\n");
    for (size_t i = 0; i < sizeof(crcIndInfo->cbCrcStatus); i++) {
        printf("DEBUG: cbCrcStatus[%zu] = %u\n", i, crcIndInfo->cbCrcStatus[i]);
    }
}

void decode_fapi_crc_ind(fapi_crc_ind_t* crcInd) {
    printf("DEBUG: header = { /* Details omitted */ }\n");
    printf("DEBUG: sfn = %u\n", crcInd->sfn);
    printf("DEBUG: slot = %u\n", crcInd->slot);
    printf("DEBUG: numCrcs = %u\n", crcInd->numCrcs);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(crcInd->pad); i++) {
        printf("%u ", crcInd->pad[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: CRC Info:\n");
    for (size_t i = 0; i < crcInd->numCrcs; i++) {
        printf("DEBUG: CRC[%zu]:\n", i);
        decode_fapi_crc_ind_info(&crcInd->crc[i]);
    }
}

void decode_fapi_harq_info(fapi_harq_info_t* harqInfo) {
    printf("DEBUG: harqCrc = %u\n", harqInfo->harqCrc);
    
    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", harqInfo->pad);
    #endif

    printf("DEBUG: harqBitLen = %u\n", harqInfo->harqBitLen);
    printf("DEBUG: harqPayload: ");
    for (size_t i = 0; i < sizeof(harqInfo->harqPayload); i++) {
        printf("%u ", harqInfo->harqPayload[i]);
    }
    printf("\n");
}

void decode_fapi_csi_p1_info(fapi_csi_p1_info_t* csiP1Info) {
    printf("DEBUG: csiPart1Crc = %u\n", csiP1Info->csiPart1Crc);
    
    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", csiP1Info->pad);
    #endif

    printf("DEBUG: csiPart1BitLen = %u\n", csiP1Info->csiPart1BitLen);
    printf("DEBUG: csiPart1Payload: ");
    for (size_t i = 0; i < sizeof(csiP1Info->csiPart1Payload); i++) {
        printf("%u ", csiP1Info->csiPart1Payload[i]);
    }
    printf("\n");
}

void decode_fapi_csi_p2_info(fapi_csi_p2_info_t* csiP2Info) {
    printf("DEBUG: csiPart2Crc = %u\n", csiP2Info->csiPart2Crc);
    
    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", csiP2Info->pad);
    #endif

    printf("DEBUG: csiPart2BitLen = %u\n", csiP2Info->csiPart2BitLen);
    printf("DEBUG: csiPart2Payload: ");
    for (size_t i = 0; i < sizeof(csiP2Info->csiPart2Payload); i++) {
        printf("%u ", csiP2Info->csiPart2Payload[i]);
    }
    printf("\n");
}

void decode_fapi_uci_o_pusch(fapi_uci_o_pusch_t* uciO_pusch) {
    printf("DEBUG: handle = %u\n", uciO_pusch->handle);
    printf("DEBUG: pduBitmap = %u\n", uciO_pusch->pduBitmap);
    printf("DEBUG: ul_cqi = %u\n", uciO_pusch->ul_cqi);
    printf("DEBUG: rnti = %u\n", uciO_pusch->rnti);
    printf("DEBUG: timingAdvance = %u\n", uciO_pusch->timingAdvance);
    printf("DEBUG: rssi = %u\n", uciO_pusch->rssi);

    // Print only if indicated by pduBitmap
    if (uciO_pusch->pduBitmap & 0x01) { // Example check for HARQ info
        printf("DEBUG: HARQ Info:\n");
        decode_fapi_harq_info(&uciO_pusch->harqInfo);
    }

    if (uciO_pusch->pduBitmap & 0x02) { // Example check for CSI Part 1 info
        printf("DEBUG: CSI Part 1 Info:\n");
        decode_fapi_csi_p1_info(&uciO_pusch->csiPart1info);
    }

    if (uciO_pusch->pduBitmap & 0x04) { // Example check for CSI Part 2 info
        printf("DEBUG: CSI Part 2 Info:\n");
        decode_fapi_csi_p2_info(&uciO_pusch->csiPart2info);
    }
}

void decode_fapi_sr_f0f1_info(fapi_sr_f0f1_info_t* srF0F1Info) {
    printf("DEBUG: srIndication = %u\n", srF0F1Info->srIndication);
    printf("DEBUG: srConfidenceLevel = %u\n", srF0F1Info->srConfidenceLevel);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(srF0F1Info->pad); i++) {
        printf("%u ", srF0F1Info->pad[i]);
    }
    printf("]\n");
    #endif
}

void decode_fapi_harq_f0f1_info(fapi_harq_f0f1_info_t* harqF0F1Info) {
    printf("DEBUG: numHarq = %u\n", harqF0F1Info->numHarq);
    printf("DEBUG: harqConfidenceLevel = %u\n", harqF0F1Info->harqConfidenceLevel);
    printf("DEBUG: harqValue: ");
    for (size_t i = 0; i < sizeof(harqF0F1Info->harqValue); i++) {
        printf("%u ", harqF0F1Info->harqValue[i]);
    }
    printf("\n");
}

void decode_fapi_sr_f2f3f4_info(fapi_sr_f2f3f4_info_t* srF2F3F4Info) {
    printf("DEBUG: srBitlen = %u\n", srF2F3F4Info->srBitlen);
    printf("DEBUG: srPayload: ");
    for (size_t i = 0; i < sizeof(srF2F3F4Info->srPayload); i++) {
        printf("%u ", srF2F3F4Info->srPayload[i]);
    }
    printf("\n");
}

void decode_fapi_harq_f2f3f4_info(fapi_harq_f2f3f4_info_t* harqF2F3F4Info) {
    printf("DEBUG: harqCrc = %u\n", harqF2F3F4Info->harqCrc);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", harqF2F3F4Info->pad);
    #endif

    printf("DEBUG: harqBitLen = %u\n", harqF2F3F4Info->harqBitLen);
    printf("DEBUG: harqPayload: ");
    for (size_t i = 0; i < sizeof(harqF2F3F4Info->harqPayload); i++) {
        printf("%u ", harqF2F3F4Info->harqPayload[i]);
    }
    printf("\n");
}

void decode_fapi_uci_o_pucch_f2f3f4(fapi_uci_o_pucch_f2f3f4_t* uciO_pucch_f2f3f4) {
    printf("DEBUG: handle = %u\n", uciO_pucch_f2f3f4->handle);
    printf("DEBUG: pduBitmap = %u\n", uciO_pucch_f2f3f4->pduBitmap);
    printf("DEBUG: pucchFormat = %u\n", uciO_pucch_f2f3f4->pucchFormat);
    printf("DEBUG: ul_cqi = %u\n", uciO_pucch_f2f3f4->ul_cqi);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", uciO_pucch_f2f3f4->pad);
    #endif

    printf("DEBUG: rnti = %u\n", uciO_pucch_f2f3f4->rnti);
    printf("DEBUG: timingAdvance = %u\n", uciO_pucch_f2f3f4->timingAdvance);
    printf("DEBUG: rssi = %u\n", uciO_pucch_f2f3f4->rssi);
    printf("DEBUG: num_uci_bits = %u\n", uciO_pucch_f2f3f4->num_uci_bits);
    printf("DEBUG: uciBits: ");
    for (size_t i = 0; i < sizeof(uciO_pucch_f2f3f4->uciBits); i++) {
        printf("%u ", uciO_pucch_f2f3f4->uciBits[i]);
    }
    printf("\n");
}

void decode_fapi_uci_o_pucch_f0f1(fapi_uci_o_pucch_f0f1_t* uciO_pucch_f0f1) {
    printf("DEBUG: handle = %u\n", uciO_pucch_f0f1->handle);
    printf("DEBUG: pduBitmap = %u\n", uciO_pucch_f0f1->pduBitmap);
    printf("DEBUG: pucchFormat = %u\n", uciO_pucch_f0f1->pucchFormat);
    printf("DEBUG: ul_cqi = %u\n", uciO_pucch_f0f1->ul_cqi);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", uciO_pucch_f0f1->pad);
    #endif

    printf("DEBUG: rnti = %u\n", uciO_pucch_f0f1->rnti);
    printf("DEBUG: timingAdvance = %u\n", uciO_pucch_f0f1->timingAdvance);
    printf("DEBUG: rssi = %u\n", uciO_pucch_f0f1->rssi);

    #ifndef OAI_TESTING
    printf("DEBUG: pad1 = [");
    for (size_t i = 0; i < sizeof(uciO_pucch_f0f1->pad1); i++) {
        printf("%u ", uciO_pucch_f0f1->pad1[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: srInfo: \n");
    decode_fapi_sr_f0f1_info(&uciO_pucch_f0f1->srInfo);

    printf("DEBUG: harqInfo: \n");
    decode_fapi_harq_f0f1_info(&uciO_pucch_f0f1->harqInfo);
}

void decode_fapi_uci_pdu_info(fapi_uci_pdu_info_t* uciPduInfo) {
    printf("DEBUG: pduType = %u\n", uciPduInfo->pduType);
    printf("DEBUG: pduSize = %u\n", uciPduInfo->pduSize);

    switch (uciPduInfo->pduType) {
        case 0: // Assuming 0 for uciPusch
            printf("DEBUG: uciPusch: \n");
            decode_fapi_uci_o_pusch(&uciPduInfo->uci.uciPusch);
            break;
        case 1: // Assuming 1 for uciPucchF0F1
            printf("DEBUG: uciPucchF0F1: \n");
            decode_fapi_uci_o_pucch_f0f1(&uciPduInfo->uci.uciPucchF0F1);
            break;
        case 2: // Assuming 2 for uciPucchF2F3F4
            printf("DEBUG: uciPucchF2F3F4: \n");
            decode_fapi_uci_o_pucch_f2f3f4(&uciPduInfo->uci.uciPucchF2F3F4);
            break;
        default:
            printf("DEBUG: Unknown pduType = %u\n", uciPduInfo->pduType);
            break;
    }
}

void decode_fapi_uci_indication(fapi_uci_indication_t* uciIndication) {
    printf("DEBUG: sfn = %u\n", uciIndication->sfn);
    printf("DEBUG: slot = %u\n", uciIndication->slot);
    printf("DEBUG: numUcis = %u\n", uciIndication->numUcis);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(uciIndication->pad); i++) {
        printf("%u ", uciIndication->pad[i]);
    }
    printf("]\n");
    #endif

    for (size_t i = 0; i < uciIndication->numUcis; i++) {
        printf("DEBUG: uciPdu[%zu]: \n", i);
        decode_fapi_uci_pdu_info(&uciIndication->uciPdu[i]);
    }
}

void decode_fapi_symb_snr(fapi_symb_snr_t* symbSnr) {
    printf("DEBUG: numRbs = %u\n", symbSnr->numRbs);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(symbSnr->pad); i++) {
        printf("%u ", symbSnr->pad[i]);
    }
    printf("]\n");
    #endif

    printf("DEBUG: rbSNR: ");
    for (size_t i = 0; i < sizeof(symbSnr->rbSNR); i++) {
        printf("%u ", symbSnr->rbSNR[i]);
    }
    printf("\n");
}

void decode_fapi_srs_pdu(fapi_srs_pdu_t* srsPdu) {
    printf("DEBUG: handle = %u\n", srsPdu->handle);
    printf("DEBUG: rnti = %u\n", srsPdu->rnti);
    printf("DEBUG: timingAdvance = %u\n", srsPdu->timingAdvance);
    printf("DEBUG: numSymbols = %u\n", srsPdu->numSymbols);
    printf("DEBUG: wideBandSnr = %u\n", srsPdu->wideBandSnr);
    printf("DEBUG: numReportedSymbols = %u\n", srsPdu->numReportedSymbols);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", srsPdu->pad);
    #endif

    for (size_t i = 0; i < srsPdu->numReportedSymbols; i++) {
        printf("DEBUG: symbSnr[%zu]: \n", i);
        decode_fapi_symb_snr(&srsPdu->symbSnr[i]);
    }
}

void decode_fapi_srs_indication(fapi_srs_indication_t* srsIndication) {
    printf("DEBUG: sfn = %u\n", srsIndication->sfn);
    printf("DEBUG: slot = %u\n", srsIndication->slot);
    printf("DEBUG: numPdus = %u\n", srsIndication->numPdus);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(srsIndication->pad); i++) {
        printf("%u ", srsIndication->pad[i]);
    }
    printf("]\n");
    #endif

    for (size_t i = 0; i < srsIndication->numPdus; i++) {
        printf("DEBUG: srsPdus[%zu]: \n", i);
        decode_fapi_srs_pdu(&srsIndication->srsPdus[i]);
    }
}

void decode_fapi_preamble_info(fapi_preamble_info_t* preambleInfo) {
    printf("DEBUG: preambleIndex = %u\n", preambleInfo->preambleIndex);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = %u\n", preambleInfo->pad);
    #endif

    printf("DEBUG: timingAdvance = %u\n", preambleInfo->timingAdvance);
    printf("DEBUG: preamblePwr = %u\n", preambleInfo->preamblePwr);
}

void decode_fapi_rach_pdu(fapi_rach_pdu_t* rachPdu) {
    printf("DEBUG: phyCellId = %u\n", rachPdu->phyCellId);
    printf("DEBUG: symbolIndex = %u\n", rachPdu->symbolIndex);
    printf("DEBUG: slotIndex = %u\n", rachPdu->slotIndex);
    printf("DEBUG: freqIndex = %u\n", rachPdu->freqIndex);
    printf("DEBUG: avgRssi = %u\n", rachPdu->avgRssi);
    printf("DEBUG: avgSnr = %u\n", rachPdu->avgSnr);
    printf("DEBUG: numPreamble = %u\n", rachPdu->numPreamble);

    for (size_t i = 0; i < rachPdu->numPreamble; i++) {
        printf("DEBUG: preambleInfo[%zu]: \n", i);
        decode_fapi_preamble_info(&rachPdu->preambleInfo[i]);
    }
}

void decode_fapi_rach_indication(fapi_rach_indication_t* rachIndication) {
    printf("DEBUG: sfn = %u\n", rachIndication->sfn);
    printf("DEBUG: slot = %u\n", rachIndication->slot);
    printf("DEBUG: numPdus = %u\n", rachIndication->numPdus);

    #ifndef OAI_TESTING
    printf("DEBUG: pad = [");
    for (size_t i = 0; i < sizeof(rachIndication->pad); i++) {
        printf("%u ", rachIndication->pad[i]);
    }
    printf("]\n");
    #endif

    for (size_t i = 0; i < rachIndication->numPdus; i++) {
        printf("DEBUG: rachPdu[%zu]: \n", i);
        decode_fapi_rach_pdu(&rachIndication->rachPdu[i]);
    }
}







