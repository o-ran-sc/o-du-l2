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

/* This file contains definitions of F1AP conversion functions */

#include "RLC-BearerConfig.h"
#include "RLC-Config.h"
#include "SN-FieldLengthAM.h"
#include "SN-FieldLengthUM.h"
#include "T-PollRetransmit.h"
#include "PollPDU.h"
#include "PollByte.h"
#include "T-Reassembly.h"
#include "T-StatusProhibit.h"
#include "PUCCH-Resource.h"
#include "PagingDRX.h"
#include "PCCH-Config.h"
#include "ServingCellConfigCommonSIB.h"
#include "ServingCellConfigCommon.h"
#include "CFRA.h"
#include "RACH-ConfigCommon.h"
#include "du_app_rlc_inf.h"
#include "LongDRXCycleLength.h"
#include "ShortDRXCycleLength.h"
#include "DRX-ConfigRrc.h"

RLC_BearerConfig__servedRadioBearer_PR covertRbTypeFromIntEnumToRrcEnum(RlcRbType rbType);
RLC_Config_PR covertRlcModeFromIntEnumToRrcEnum(RlcMode mode);
SN_FieldLengthAM_t covertAmSnLenFromIntEnumToRrcEnum(SnLenAm snLen);
SnLenAm covertAmSnLenFromRrcEnumToIntEnum(SN_FieldLengthAM_t snLen);
SN_FieldLengthUM_t covertUmSnLenFromIntEnumToRrcEnum(SnLenUm snLen);
SnLenUm covertUmSnLenFromRrcEnumToIntEnum(SN_FieldLengthUM_t snLen);
T_PollRetransmit_t covertPollRetxTmrValueToEnum(uint16_t pollRetxTmr);
uint16_t covertPollRetxTmrEnumToValue(uint8_t pollRetxTmrCfg);
PollPDU_t covertPollPduValueToEnum(int32_t pollPdu);
int32_t covertPollPduEnumToValue(uint8_t pollPduCfg);
PollByte_t covertPollByteValueToEnum(int32_t pollByte);
int32_t covertPollByteEnumToValue(uint16_t pollBytesCfg);
uint8_t covertMaxRetxValueToEnum(uint8_t maxRetx);
uint8_t covertMaxRetxEnumToValue(uint8_t maxRetxCfg);
T_Reassembly_t convertReasmblTmrValueToEnum(int8_t reasmblTmr);
int8_t convertReasmblTmrEnumToValue(uint8_t reAsmblTmrCfg);
T_StatusProhibit_t convertProhibitTmrValueToEnum(int16_t prohibitTmr);
int16_t convertProhibitTmrEnumToValue(T_StatusProhibit_t prohibitTmr);
PUCCH_Resource__format_PR covertPucchFormatIntEnumToRrcEnum(uint8_t pucchFormat);
long convertBsrPeriodicTmrValueToEnum(int16_t periodicTmr);
long convertBsrRetxTmrValueToEnum(uint16_t retxTmr);
long convertLcSrDelayTmrValueToEnum(uint16_t delayTimer);

e_PagingDRX convertPagingCycleValueToEnum(uint16_t pagingDrxCycle);
uint16_t convertPagingCycleEnumToValue(e_PagingDRX pagingDrx);
uint8_t convertNsEnumToValue(e_PCCH_Config__ns Ns);
e_PCCH_Config__ns convertNsValueToEnum(uint8_t numPO);

e_ServingCellConfigCommonSIB__ssb_PeriodicityServingCell convertSsbPeriodicityValueToEnumForSib(uint16_t ssbPeriodicity);
e_ServingCellConfigCommon__ssb_periodicityServingCell convertSsbPeriodicityValueToEnum(uint16_t ssbPeriodicity);
e_CFRA__occasions__ssb_perRACH_Occasion convertCFRASsbPerRachOccasionValueToEnum(uint8_t ssbOccPerRach);
e_ServingCellConfigCommon__dmrs_TypeA_Position convertDmrsTypeAPosValueToEnum(uint8_t dmrsTypeAPos);
e_RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one convertCbPreamblePerSsbValueToEnum(uint8_t numCbPreamblePerSsb);

enum DRX_ConfigRrc__drx_LongCycleStartOffset_PR convertLongDrxCycleLengthValueToEnum(uint16_t longDrxCycleLength);
e_DRX_ConfigRrc__shortDRX__drx_ShortCycle convertShortDrxCycleLengthValueToEnum(uint16_t shortDrxCycleLength);
e_DRX_ConfigRrc__drx_onDurationTimer__milliSeconds convertOnDurationTimerMilliSecondsValueToEnum(uint16_t ondurationTime);
e_DRX_ConfigRrc__drx_RetransmissionTimerUL convertDrxRetransmissionTimerUlValueToEnum(uint16_t retransmissionTimerUL);
e_DRX_ConfigRrc__drx_RetransmissionTimerDL convertDrxRetransmissionTimerDlValueToEnum(uint16_t retransmissionTimerDL);
e_DRX_ConfigRrc__drx_InactivityTimer convertDrxInactivityTimerValueToEnum(uint16_t inactivityTimer);
uint16_t convertShortDrxCycleLengthEnumToValue(e_ShortDRXCycleLength shortDrxCycleLength);
uint16_t convertLongDrxCycleLengthEnumToValue(e_LongDRXCycleLength longDrxCycleLength);
uint16_t convertDrxInactivityTimerEnumToValue(e_DRX_ConfigRrc__drx_InactivityTimer inactivityTimer);
uint16_t convertDrxRetransmissionTimerDlEnumToValue(e_DRX_ConfigRrc__drx_RetransmissionTimerDL retransmissionTimerDL);
uint16_t convertDrxRetransmissionTimerUlEnumToValue(e_DRX_ConfigRrc__drx_RetransmissionTimerUL retransmissionTimerUL);
uint16_t convertOnDurationTimerMilliSecondsEnumToValue(e_DRX_ConfigRrc__drx_onDurationTimer__milliSeconds ondurationTime);
/**********************************************************************
  End of file
 **********************************************************************/
