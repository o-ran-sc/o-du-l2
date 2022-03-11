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

RLC_BearerConfig__servedRadioBearer_PR covertRbTypeFromIntEnumToRrcEnum(RlcRbType rbType);
RLC_Config_PR covertRlcModeFromIntEnumToRrcEnum(RlcMode mode);
SN_FieldLengthAM_t covertAmSnLenFromIntEnumToRrcEnum(SnLenAm snLen);
SnLenAm covertAmSnLenFromRrcEnumToIntEnum(SN_FieldLengthAM_t snLen);
SN_FieldLengthUM_t covertUmSnLenFromIntEnumToRrcEnum(SnLenUm snLen);
SnLenUm covertUmSnLenFromRrcEnumToIntEnum(SN_FieldLengthUM_t snLen);
T_PollRetransmit_t covertPollRetxTmrValueToEnum(uint16_t pollRetxTmr);
uint16_t covertPollRetxTmrEnumToValue(uint8_t pollRetxTmrCfg);
PollPDU_t covertPollPduValueToEnum(int16_t pollPdu);
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

/**********************************************************************
  End of file
 **********************************************************************/
