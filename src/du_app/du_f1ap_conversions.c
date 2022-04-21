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

/* This file contains functions that maps values received in F1AP message with
 * its corresponding values used in DU and vice-versa */

#include "common_def.h"
#include "BSR-Config.h"
#include "du_f1ap_conversions.h"

/************************************************************************
 *
 * @brief Coverts Internal Enum value to RRC code enum for RB type 
 *
 * @details
 *
 *    Function : covertRbTypeFromIntEnumToRrcEnum
 *
 *    Functionality: Coverts Internal Enum value to RRC code enum for RB type 
 *
 * @params[in] Internal Enum value for RB type
 * @return RRC codec enum value for RB type
 *
 * **********************************************************************/
RLC_BearerConfig__servedRadioBearer_PR covertRbTypeFromIntEnumToRrcEnum(RlcRbType rbType)
{
   switch(rbType)
   {
      case RB_TYPE_SRB:
         return RLC_BearerConfig__servedRadioBearer_PR_srb_Identity;
      case RB_TYPE_DRB:
         return RLC_BearerConfig__servedRadioBearer_PR_drb_Identity;
      default:
         return RLC_BearerConfig__servedRadioBearer_PR_NOTHING;
   }
}

/************************************************************************
 *
 * @brief Coverts internal enum value to RRC code enum for RLC mode
 *
 * @details
 *
 *    Function : covertRlcModeFromIntEnumToRrcEnum
 *
 *    Functionality: Coverts internal enum value to RRC code enum for RLC mode
 *
 * @params[in] Internal enum value for RLC mode
 * @return RRC codec enum value for RLC mode
 *
 * **********************************************************************/
RLC_Config_PR covertRlcModeFromIntEnumToRrcEnum(RlcMode mode)
{
   switch(mode)
   {
      case RLC_AM:
         return RLC_Config_PR_am;
      case RLC_UM_BI_DIRECTIONAL:
         return RLC_Config_PR_um_Bi_Directional;
      case RLC_UM_UNI_DIRECTIONAL_UL:
         return RLC_Config_PR_um_Uni_Directional_UL;
      case RLC_UM_UNI_DIRECTIONAL_DL:
         return RLC_Config_PR_um_Uni_Directional_DL;
      default:
         return RLC_Config_PR_NOTHING;
   }
}

/************************************************************************
 *
 * @brief Converts Internal enum value to RRC code enum value for AM mode 
 * SN Length
 *
 * @details
 *
 *    Function : covertAmSnLenFromIntEnumToRrcEnum 
 *
 *    Functionality: Converts Internal enum value to RRC code enum value for AM
 *    mode SN Length
 *
 * @params[in] Internal Enum value 
 * @return RRC Enum value
 *
 * **********************************************************************/
SN_FieldLengthAM_t covertAmSnLenFromIntEnumToRrcEnum(SnLenAm snLen)
{
   switch(snLen)
   {
      case AM_SIZE_12:
         return SN_FieldLengthAM_size12;
      case AM_SIZE_18:
         return SN_FieldLengthAM_size18; 
      default:
         return SN_FieldLengthAM_size12;
   }
}

/************************************************************************
 *
 * @brief Coverts RRC ASN enum value to Internal Enum value 
 *
 * @details
 *
 *    Function : covertAmSnLenFromRrcEnumToIntEnum
 *
 *    Functionality: Coverts RRC ASN enum value to Internal Enum value for
 *    AM mode SN length
 *
 * @params[in] RRC ASN Enum value
 * @return Internal Enum value
 *
 * **********************************************************************/
SnLenAm covertAmSnLenFromRrcEnumToIntEnum(SN_FieldLengthAM_t snLen)
{
   switch(snLen)
   {
      case SN_FieldLengthAM_size12:
         return AM_SIZE_12;
      case SN_FieldLengthAM_size18:
         return AM_SIZE_18;
      default:
         return AM_SIZE_12;
   }
}

/************************************************************************
 *
 * @brief Coverts Internal Enum value to RRC ASN enum value
 *
 * @details
 *
 *    Function : covertUmSnLenFromIntEnumToRrcEnum
 *
 *    Functionality: Coverts Internal ennum value to  RRC ASN enum value
 *    for UM mode SN length
 *
 * @params[in] Internal enum value 
 * @return RRC ASN value
 *
 * **********************************************************************/
SN_FieldLengthUM_t covertUmSnLenFromIntEnumToRrcEnum(SnLenUm snLen)
{
   switch(snLen)
   {
      case UM_SIZE_6:
         return SN_FieldLengthUM_size6;
      case UM_SIZE_12:
         return SN_FieldLengthUM_size12;
      default:
         return SN_FieldLengthUM_size12;
   }
}

/************************************************************************
 *
 * @brief Converts RRC ASN enum value to Internal enum value
 *
 * @details
 *
 *    Function : covertUmSnLenFromRrcEnumToIntEnum 
 *
 *    Functionality: Converts RRC ASN enum value to Internal enum value
 *    for UM mode SN length
 *
 * @params[in] RRC ASN enum value 
 * @return Internal enum value
 *
 * **********************************************************************/
SnLenUm covertUmSnLenFromRrcEnumToIntEnum(SN_FieldLengthUM_t snLen)
{
   switch(snLen)
   {
      case SN_FieldLengthUM_size6:
         return UM_SIZE_6;
      case SN_FieldLengthUM_size12:
         return UM_SIZE_12;
      default:
         return UM_SIZE_12;
   }
}

/************************************************************************
 *
 * @brief Converts poll retransmission timer value to its corresponding enum 
 *
 * @details
 *
 *    Function : covertPollRetxTmrValueToEnum
 *
 *    Functionality: Converts poll retransmission timer value to its
 *    corresponding enum
 *
 * @params[in] Poll retransmission timer value
 * @return Poll retransmission timer enum
 *
 * **********************************************************************/
T_PollRetransmit_t covertPollRetxTmrValueToEnum(uint16_t pollRetxTmr)
{
   /* All values of poll retx timer are at interval of 5ms starting from 5ms(Enum value 0) upto 250ms
    * Hence converting the value to enum by dividing it to 5 and subtracting 1.
    */
   if((pollRetxTmr <= 250) && ((pollRetxTmr%5) == 0))
      return ((pollRetxTmr/5) -1);
   else
   {
      switch(pollRetxTmr)
      {
         case 300:
            return T_PollRetransmit_ms300;
         case 350:
            return T_PollRetransmit_ms350;
         case 400:
            return T_PollRetransmit_ms400;
         case 450:
            return T_PollRetransmit_ms450;
         case 500:
            return T_PollRetransmit_ms500;
         case 800:
            return T_PollRetransmit_ms800;
         default:
            return T_PollRetransmit_spare5;
      }
   }
}

/************************************************************************
 *
 * @brief Converts enum values into actual value of Poll retransmit timer
 *
 * @details
 *
 *    Function : covertPollRetxTmrEnumToValue
 *
 *    Functionality: Converts enum values into actual value of poll 
 *    retransmit timer
 *
 * @params[in] Enum value of pollPdu
 * @return Actual value of pollPdu
 *
 * **********************************************************************/

uint16_t covertPollRetxTmrEnumToValue(uint8_t pollRetxTmrCfg)
{
   uint16_t pollRetxTmr;

   /* All values of poll retx timer are at interval of 5ms.
    * This is valid upto 250ms
    * Hence converting the enum value to actual value by multiplying it to 5
    */
   if(pollRetxTmrCfg <= T_PollRetransmit_ms250)
      pollRetxTmr = (pollRetxTmrCfg + 1) * 5;
   else
   {
      switch(pollRetxTmrCfg)
      {
         case T_PollRetransmit_ms300:
            pollRetxTmr = 300;
            break;
         case T_PollRetransmit_ms350:
            pollRetxTmr = 350;
            break;
         case T_PollRetransmit_ms400:
            pollRetxTmr = 400;
            break;
         case T_PollRetransmit_ms450:
            pollRetxTmr = 450;
            break;
         case T_PollRetransmit_ms500:
            pollRetxTmr = 500;
            break;
         case T_PollRetransmit_ms800:
            pollRetxTmr = 800;
            break;
         default:
            DU_LOG("\nERROR  -->  F1AP: Invalid value of Poll Retransmit timer");
            pollRetxTmr = 0;
      }
   }
   return pollRetxTmr; 
}

/************************************************************************
 *
 * @brief Coverts the value of Poll pdu to its corresponding enum 
 *
 * @details
 *
 *    Function : covertPollPduValueToEnum
 *
 *    Functionality: Coverts the value of Poll pdu to its corresponding enum
 *
 * @params[in] Poll PDU value
 * @return Poll PDU enum
 *
 * **********************************************************************/
PollPDU_t covertPollPduValueToEnum(int32_t pollPdu)
{
   switch(pollPdu)
   {
      case 4:
         return PollPDU_p4;
      case 8:
         return PollPDU_p8;
      case 16:
         return PollPDU_p16;
      case 32:
         return PollPDU_p32;
      case 64:
         return PollPDU_p64;
      case 128:
         return PollPDU_p128;
      case 256:
         return PollPDU_p256;
      case 512:
         return PollPDU_p512;
      case 1024:
         return PollPDU_p1024;
      case 2048:
         return PollPDU_p2048;
      case 4096:
         return PollPDU_p4096;
      case 6144:
         return PollPDU_p6144;
      case 8192:
         return PollPDU_p8192;
      case 12288:
         return PollPDU_p12288;
      case 16384:
         return PollPDU_p16384;
      case 20480:
         return PollPDU_p20480;
      case 24576:
         return PollPDU_p24576;
      case 28672:
         return PollPDU_p28672;
      case 32768:
         return PollPDU_p32768;
      case 40960:
         return PollPDU_p40960;
      case 49152:
         return PollPDU_p49152;
      case 57344:
         return PollPDU_p57344;
      case 65536:
         return PollPDU_p65536;
      case -1:
         return PollPDU_infinity;
      default:
         return PollPDU_spare8;
   }
   return PollPDU_spare8;
}

/*******************************************************************
 *
 * @brief Converts enum values into actual value of PollPdu
 *
 * @details
 *
 *    Function : covertPollPduEnumToValue
 *
 *    Functionality: Converts enum values into actual value of PollPdu
 *
 * @params[in] Enum value of pollPdu
 * @return Actual value of pollPdu
 *
 * ****************************************************************/
int32_t covertPollPduEnumToValue(uint8_t pollPduCfg)
{
   int32_t pollPdu;
   switch(pollPduCfg)
   {
      case PollPDU_p4:
         pollPdu = 4;
         break;
      case PollPDU_p8:
         pollPdu = 8;
         break;
      case PollPDU_p16:
         pollPdu = 16;
         break;
      case PollPDU_p32:
         pollPdu = 32;
         break;
      case PollPDU_p64:
         pollPdu = 64;
         break;
      case PollPDU_p128:
         pollPdu = 128;
         break;
      case PollPDU_p256:
         pollPdu = 256;
         break;
      case PollPDU_p512:
         pollPdu = 512;
         break;
      case PollPDU_p1024:
         pollPdu = 1024;
         break;
      case PollPDU_p2048:
         pollPdu = 2048;
         break;
      case PollPDU_p4096:
         pollPdu = 4096;
         break;
      case PollPDU_p6144:
         pollPdu = 6144;
         break;
      case PollPDU_p8192:
         pollPdu = 8192;
         break;
      case PollPDU_p12288:
         pollPdu = 12288;
         break;
      case PollPDU_p16384:
         pollPdu = 16384;
         break;
      case PollPDU_p20480:
         pollPdu = 20480;
         break;
      case PollPDU_p24576:
         pollPdu = 24576;
         break;
      case PollPDU_p28672:
         pollPdu = 28672;
         break;
      case PollPDU_p32768:
         pollPdu = 32768;
         break;
      case PollPDU_p40960:
         pollPdu = 40960;
         break;
      case PollPDU_p49152:
         pollPdu = 49152;
         break;
      case PollPDU_p57344:
         pollPdu = 57344;
         break;
      case PollPDU_p65536:
         pollPdu = 65536;
         break;
      case PollPDU_infinity:
         pollPdu = -1;
         break;
      default:
         DU_LOG("\nERROR  -->  F1AP: Invalid value of poll pdu");
         pollPdu = 0;
         break;
   }
   return pollPdu;
}

/************************************************************************
 *
 * @brief Converts Poll byte value to its corresponding enum 
 *
 * @details
 *
 *    Function : covertPollByteValueToEnum
 *
 *    Functionality: Converts Poll byte value to its corresponding enum
 *
 * @params[in] Poll Byte value
 * @return Poll Byte enum
 *
 * **********************************************************************/
PollByte_t covertPollByteValueToEnum(int32_t pollByte)
{
   switch(pollByte)
   {
      case 1000:
         return PollByte_kB1;
      case 2000:
         return PollByte_kB2;
      case 5000:
         return PollByte_kB5;
      case 8000:
         return PollByte_kB8;
      case 10000:
         return PollByte_kB10;
      case 15000:
         return PollByte_kB15;
      case 25000:
         return PollByte_kB25;
      case 50000:
         return PollByte_kB50;
      case 75000:
         return PollByte_kB75;
      case 100000:
         return PollByte_kB100;
      case 125000:
         return PollByte_kB125;
      case 250000:
         return PollByte_kB250;
      case 375000:
         return PollByte_kB375;
      case 500000:
         return PollByte_kB500;
      case 750000:
         return PollByte_kB750;
      case 1000000:
         return PollByte_kB1000;
      case 1250000:
         return PollByte_kB1250;
      case 1500000:
         return PollByte_kB1500;
      case 2000000:
         return PollByte_kB2000;
      case 3000000:
         return PollByte_kB3000;
      case 4000000:
         return PollByte_kB4000;
      case 4500000:
         return PollByte_kB4500;
      case 5000000:
         return PollByte_kB5000;
      case 5500000:
         return PollByte_kB5500;
      case 6000000:
         return PollByte_kB6000;
      case 6500000:
         return PollByte_kB6500;
      case 7000000:
         return PollByte_kB7000;
      case 7500000:
         return PollByte_kB7500;
      case 8000000:
         return PollByte_mB8;
      case 9000000:
         return PollByte_mB9;
      case 10000000:
         return PollByte_mB10;
      case 11000000:
         return PollByte_mB11;
      case 12000000:
         return PollByte_mB12;
      case 13000000:
         return PollByte_mB13;
      case 14000000:
         return PollByte_mB14;
      case 15000000:
         return PollByte_mB15;
      case 16000000:
         return PollByte_mB16;
      case 17000000:
         return PollByte_mB17;
      case 18000000:
         return PollByte_mB18;
      case 20000000:
         return PollByte_mB20;
      case 25000000:
         return PollByte_mB25;
      case 30000000:
         return PollByte_mB30;
      case 40000000:
         return PollByte_mB40;
      case -1:
         return PollByte_infinity; 
      default:
         return PollByte_spare20;
   }
}

/*******************************************************************
 *
 * @brief Converts enum values into actual value of poll bytes
 *
 * @details
 *
 *    Function : covertPollByteEnumToValue
 *
 *    Functionality: Converts enum values into actual value of pollBytes
 *
 * @params[in] Enum value
 * @return Actual value
 *
 * ****************************************************************/
int32_t covertPollByteEnumToValue(uint16_t pollBytesCfg)
{
   int32_t pollBytes;
   switch(pollBytesCfg)
   {
      case PollByte_kB1:
         pollBytes = 1000;
         break;
      case PollByte_kB2:
         pollBytes = 2000;
         break;
      case PollByte_kB5:
         pollBytes = 5000;
         break;
      case PollByte_kB8:
         pollBytes = 8000;
         break;
      case PollByte_kB10:
         pollBytes = 10000;
         break;
      case PollByte_kB15:
         pollBytes = 15000;
         break;
      case PollByte_kB25:
         pollBytes = 25000;
         break;
      case PollByte_kB50:
         pollBytes = 50000;
         break;
      case PollByte_kB75:
         pollBytes = 75000;
         break;
      case PollByte_kB100:
         pollBytes = 100000;
         break;
      case PollByte_kB125:
         pollBytes = 125000;
         break;
      case PollByte_kB250:
         pollBytes = 250000;
         break;
      case PollByte_kB375:
         pollBytes = 375000;
         break;
      case PollByte_kB500:
         pollBytes = 500000;
         break;
      case PollByte_kB750:
         pollBytes = 750000;
         break;
      case PollByte_kB1000:
         pollBytes = 1000000;
         break;
      case PollByte_kB1250:
         pollBytes = 1250000;
         break;
      case PollByte_kB1500:
         pollBytes = 1500000;
         break;
      case PollByte_kB2000:
         pollBytes = 2000000;
         break;
      case PollByte_kB3000:
         pollBytes = 3000000;
         break;
      case PollByte_kB4000:
         pollBytes = 4000000;
         break;
      case PollByte_kB4500:
         pollBytes = 4500000;
         break;
      case PollByte_kB5000:
         pollBytes = 5000000;
         break;
      case PollByte_kB5500:
         pollBytes = 5500000;
         break;
      case PollByte_kB6000:
         pollBytes = 6000000;
         break;
      case PollByte_kB6500:
         pollBytes = 6500000;
         break;
      case PollByte_kB7000:
         pollBytes = 7000000;
         break;
      case PollByte_kB7500:
         pollBytes = 7500000;
         break;
      case PollByte_mB8:
         pollBytes = 8000000;
         break;
      case PollByte_mB9:
         pollBytes = 9000000;
         break;
      case PollByte_mB10:
         pollBytes = 10000000;
         break;
      case PollByte_mB11:
         pollBytes = 11000000;
         break;
      case PollByte_mB12:
         pollBytes = 12000000;
         break;
      case PollByte_mB13:
         pollBytes = 13000000;
         break;
      case PollByte_mB14:
         pollBytes = 14000000;
         break;
      case PollByte_mB15:
         pollBytes = 15000000;
         break;
      case PollByte_mB16:
         pollBytes = 16000000;
         break;
      case PollByte_mB17:
         pollBytes = 17000000;
         break;
      case PollByte_mB18:
         pollBytes = 18000000;
         break;
      case PollByte_mB20:
         pollBytes = 20000000;
         break;
      case PollByte_mB25:
         pollBytes = 25000000;
         break;
      case PollByte_mB30:
         pollBytes = 30000000;
         break;
      case PollByte_mB40:
         pollBytes = 40000000;
         break;
      case PollByte_infinity:
         pollBytes = -1;
         break;
      default:
         DU_LOG("\nERROR  -->  F1AP: Invalid value of poll bytes");
         pollBytes = 0;
   }
   return pollBytes;
}

/************************************************************************
 *
 * @brief Converts Max retranmission value to enum 
 *
 * @details
 *
 *    Function : covertMaxRetxValueToEnum
 *
 *    Functionality: Converts Max retranmission value to enum
 *
 * @params[in] Max retransmission value
 * @return Max retransmission enum
 *
 * **********************************************************************/
uint8_t covertMaxRetxValueToEnum(uint8_t maxRetx)
{
   switch(maxRetx)
   {
      case 1:
         return UL_AM_RLC__maxRetxThreshold_t1;
      case 2:
         return UL_AM_RLC__maxRetxThreshold_t2;
      case 3:
         return UL_AM_RLC__maxRetxThreshold_t3;
      case 4:
         return UL_AM_RLC__maxRetxThreshold_t4;
      case 6:
         return UL_AM_RLC__maxRetxThreshold_t6;
      case 8:
         return UL_AM_RLC__maxRetxThreshold_t8;
      case 16:
         return UL_AM_RLC__maxRetxThreshold_t16;
      case 32:
         return UL_AM_RLC__maxRetxThreshold_t32;
   }
   return UL_AM_RLC__maxRetxThreshold_t8;
}

/*******************************************************************
 *
 * @brief Converts enum values into actual value of maxRetx
 *
 * @details
 *
 *    Function : covertMaxRetxEnumToValue
 *
 *    Functionality: Converts enum values into actual value of maxRetx
 *
 * @params[in] Enum value
 * @return Actual value
 *
 * ****************************************************************/
uint8_t covertMaxRetxEnumToValue(uint8_t maxRetxCfg)
{
   uint8_t maxRetx;
   switch(maxRetxCfg)
   {
      case UL_AM_RLC__maxRetxThreshold_t1:
         maxRetx = 1;
         break;
      case UL_AM_RLC__maxRetxThreshold_t2:
         maxRetx = 2;
         break;
      case UL_AM_RLC__maxRetxThreshold_t3:
         maxRetx = 3;
         break;
      case UL_AM_RLC__maxRetxThreshold_t4:
         maxRetx = 4;
         break;
      case UL_AM_RLC__maxRetxThreshold_t6:
         maxRetx = 6;
         break;
      case UL_AM_RLC__maxRetxThreshold_t8:
         maxRetx = 8;
         break;
      case UL_AM_RLC__maxRetxThreshold_t16:
         maxRetx = 16;
         break;
      case UL_AM_RLC__maxRetxThreshold_t32:
         maxRetx = 32;
         break;
      default:
         DU_LOG("\nERROR  -->  F1AP: Invalid configuration for Max retransmission threshold");
         maxRetx = 0;
   }
   return maxRetx;
}

/************************************************************************
 *
 * @brief Converts reassembly timer value to its ASN enum 
 *
 * @details
 *
 *    Function : convertReasmblTmrValueToEnum
 *
 *    Functionality: Converts reassembly timer value to its ASN enum
 *
 * @params[in] Reassembly timer value
 * @return Reassembly timer enum
 *
 * **********************************************************************/
T_Reassembly_t convertReasmblTmrValueToEnum(int8_t reasmblTmr)
{
   if(reasmblTmr == 0)
      return T_Reassembly_ms0;
   else if((reasmblTmr >=5 && reasmblTmr <= 100) && ((reasmblTmr % 5) == 0))
      return (reasmblTmr/5);
   else if((reasmblTmr <= 200) && ((reasmblTmr % 10) == 0))
      return ((reasmblTmr/10) +10);
   else 
      return T_Reassembly_spare1;
}

/*******************************************************************
 * @brief Converts enum values into actual value of reassembly timer
 *
 * @details
 *
 *    Function : convertReasmblTmrEnumToValue
 *
 *    Functionality: Converts enum values into actual value of reassembly 
 *    timer
 *
 * @params[in] Enum value of reassembly timer
 * @return Actual value of reassembly timer
 *
 * **********************************************************************/

int8_t convertReasmblTmrEnumToValue(uint8_t reAsmblTmrCfg)
{
   int8_t reAsmblTmr = 0;
   
   if(reAsmblTmrCfg == T_Reassembly_ms0)
   {
      reAsmblTmr = 0;
   }
   else if(reAsmblTmrCfg >= T_Reassembly_ms5 || reAsmblTmrCfg <= T_Reassembly_ms100)
   {
     /* All values of re assembly timer are at interval of 5ms.
      * This is valid upto 100ms
      * Hence converting the enum value to actual value by multiplying it to 5
      */
      reAsmblTmr = reAsmblTmrCfg * 5;
   }
   else if(reAsmblTmrCfg >= T_Reassembly_ms110 || reAsmblTmrCfg <= T_Reassembly_ms200)
   {
     /* All values of re assembly timer are at interval of 10ms.
      * This is valid upto 200ms
      * since enum value starts from 20 for 100ms, subtracting 10 and
      * converting the enum value to actual value by multiplying it to 10
      */
      reAsmblTmr = ((reAsmblTmrCfg-10) * 10);
   }
   else
   {
      DU_LOG("\nERROR  -->  F1AP : Invalid value of Re Assembly timer %d", reAsmblTmrCfg);
      reAsmblTmr = -1;
   }
   return reAsmblTmr; 
}

/************************************************************************
 *
 * @brief Converts Prohibit timer value to its ASN enum 
 *
 * @details
 *
 *    Function : convertProhibitTmrValueToEnum
 *
 *    Functionality: Converts Prohibit timer value to its ASN enum
 *
 * @params[in] Prohibit timer value
 * @return Prohibit timer enum
 *
 * **********************************************************************/
T_StatusProhibit_t convertProhibitTmrValueToEnum(int16_t prohibitTmr)
{
   if(prohibitTmr == 0)
      return T_StatusProhibit_ms0;
   else if((prohibitTmr >= 5) && (prohibitTmr <= 250) && ((prohibitTmr % 5) == 0))
      return prohibitTmr/5;
   else
   {
      switch(prohibitTmr)
      {
         case 300:
            return T_StatusProhibit_ms300;
         case 350:
            return T_StatusProhibit_ms350;
         case 400:
            return T_StatusProhibit_ms400;
         case 450:
            return T_StatusProhibit_ms450;
         case 500:
            return T_StatusProhibit_ms500;
         case 800:
            return T_StatusProhibit_ms800;
         case 1000:
            return T_StatusProhibit_ms1000;
         case 1200:
            return T_StatusProhibit_ms1200;
         case 1600:
            return T_StatusProhibit_ms1600;
         case 2000:
            return T_StatusProhibit_ms2000;
         case 2400:
            return T_StatusProhibit_ms2400;
         default:
            return T_StatusProhibit_spare2;
      }
   }
}

/************************************************************************
 *
 * @brief Converts Prohibit timer enum to its actual value
 *
 * @details
 *
 *    Function : convertProhibitTmrEnumToValue
 *
 *    Functionality: Converts Prohibit timer enum to its actual value
 *
 * @params[in] Prohibit timer enum
 * @return Prohibit timer value
 *
 * **********************************************************************/
int16_t convertProhibitTmrEnumToValue(T_StatusProhibit_t prohibitTmr)
{
   if(prohibitTmr == T_StatusProhibit_ms0)
      return 0;
   else if((prohibitTmr >= T_StatusProhibit_ms5) && (prohibitTmr <= T_StatusProhibit_ms250))
      return prohibitTmr*5;
   else
   {
      switch(prohibitTmr)
      {
         case T_StatusProhibit_ms300:
            return 300;
         case T_StatusProhibit_ms350:
            return 350;
         case T_StatusProhibit_ms400:
            return 400;
         case T_StatusProhibit_ms450:
            return 450;
         case T_StatusProhibit_ms500:
            return 500;
         case T_StatusProhibit_ms800:
            return 800;
         case T_StatusProhibit_ms1000:
            return 1000;
         case T_StatusProhibit_ms1200:
            return 1200;
         case T_StatusProhibit_ms1600:
            return 1600;
         case T_StatusProhibit_ms2000:
            return 2000;
         case T_StatusProhibit_ms2400:
            return 2400;
         default:
            return 35;
      }
   }
}

/************************************************************************
 *
 * @brief Converts internal enum value to RRC ASN enum value 
 *
 * @details
 *
 *    Function : covertPucchFormatIntEnumToRrcEnum
 *
 *    Functionality: Converts internal enum value to RRC ASN enum value
 *    for PUCCH format
 *
 * @params[in] PUCCH format value in  internal enum
 * @return PUCCH format value RRC ASN enum
 *
 * **********************************************************************/
PUCCH_Resource__format_PR covertPucchFormatIntEnumToRrcEnum(uint8_t pucchFormat)
{
   switch(pucchFormat)
   {
      case PUCCH_FORMAT_0:
         return PUCCH_Resource__format_PR_format0;
      case PUCCH_FORMAT_1:
         return PUCCH_Resource__format_PR_format1;
      case PUCCH_FORMAT_2:
         return PUCCH_Resource__format_PR_format2;
      case PUCCH_FORMAT_3:
         return PUCCH_Resource__format_PR_format3;
      case PUCCH_FORMAT_4:
         return PUCCH_Resource__format_PR_format4;
      default:
         return PUCCH_Resource__format_PR_NOTHING;
   }
   return PUCCH_Resource__format_PR_NOTHING;
}

/************************************************************************
 *
 * @brief Converts BSR Periodic timer value to its ASN enum value
 *
 * @details
 *
 *    Function : convertBsrPeriodicTmrValueToEnum
 *
 *    Functionality: Converts BSR Periodic timer timer value to its ASN enum value
 *
 * @params[in] BSR Periodic timer value
 * @return BSR Periodic timer ASN enum
 *
 * **********************************************************************/
long convertBsrPeriodicTmrValueToEnum(int16_t periodicTmr)
{
   switch(periodicTmr)
   {
      case 1:
         return BSR_Config__periodicBSR_Timer_sf1;
      case 5:
         return BSR_Config__periodicBSR_Timer_sf5;
      case 10:
         return BSR_Config__periodicBSR_Timer_sf10;
      case 16:
         return BSR_Config__periodicBSR_Timer_sf16;
      case 20:
         return BSR_Config__periodicBSR_Timer_sf20;
      case 32:
         return BSR_Config__periodicBSR_Timer_sf32;
      case 40:
         return BSR_Config__periodicBSR_Timer_sf40;
      case 64:
         return BSR_Config__periodicBSR_Timer_sf64;
      case 80:
         return BSR_Config__periodicBSR_Timer_sf80;
      case 128:
         return BSR_Config__periodicBSR_Timer_sf128;
      case 320:
         return BSR_Config__periodicBSR_Timer_sf320;
      case 640:
         return BSR_Config__periodicBSR_Timer_sf640;
      case 1280:
         return BSR_Config__periodicBSR_Timer_sf1280;
      case 2560:
         return BSR_Config__periodicBSR_Timer_sf2560;
      case -1:
         return BSR_Config__periodicBSR_Timer_infinity;
      default:
         return BSR_Config__periodicBSR_Timer_sf10;
   }
}

/************************************************************************
 *
 * @brief Converts BSR Retransmission timer value to its ASN enum value
 *
 * @details
 *
 *    Function : convertBsrRetxTmrValueToEnum
 *
 *    Functionality: Converts BSR Retransmission timer timer value to its ASN enum value
 *
 * @params[in] BSR Retransmission timer value
 * @return BSR Restransmission timer ASN enum
 *
 * **********************************************************************/
long convertBsrRetxTmrValueToEnum(uint16_t retxTmr)
{
   switch(retxTmr)
   {
      case 10:
         return BSR_Config__retxBSR_Timer_sf10;
      case 20:
         return BSR_Config__retxBSR_Timer_sf20;
      case 40:
         return BSR_Config__retxBSR_Timer_sf40;
      case 80:
         return BSR_Config__retxBSR_Timer_sf80;
      case 160:
         return BSR_Config__retxBSR_Timer_sf160;
      case 320:
         return BSR_Config__retxBSR_Timer_sf320;
      case 640:
         return BSR_Config__retxBSR_Timer_sf640;
      case 1280:
         return BSR_Config__retxBSR_Timer_sf1280;
      case 2560:
         return BSR_Config__retxBSR_Timer_sf2560;
      case 5120:
         return BSR_Config__retxBSR_Timer_sf5120;
      case 10240:
         return BSR_Config__retxBSR_Timer_sf10240;
      default:
         return BSR_Config__retxBSR_Timer_sf320;
   }
}

/************************************************************************
 *
 * @brief Converts LC SR Delay timer value to its ASN enum value
 *
 * @details
 *
 *    Function : convertLcSrDelayTmrValueToEnum
 *
 *    Functionality: Converts LC SR Delay timer value to its ASN enum value
 *
 * @params[in] LC SR Delay timer value
 * @return LC SR Delay timer ASN enum
 *
 * **********************************************************************/
long convertLcSrDelayTmrValueToEnum(uint16_t delayTimer)
{
   switch(delayTimer)
   {
      case 20:
         return BSR_Config__logicalChannelSR_DelayTimer_sf20;
      case 40:
         return BSR_Config__logicalChannelSR_DelayTimer_sf40;
      case 64:
         return BSR_Config__logicalChannelSR_DelayTimer_sf64;
      case 128:
         return BSR_Config__logicalChannelSR_DelayTimer_sf128;
      case 512:
         return BSR_Config__logicalChannelSR_DelayTimer_sf512;
      case 1024:
         return BSR_Config__logicalChannelSR_DelayTimer_sf1024;
      case 2560:
         return BSR_Config__logicalChannelSR_DelayTimer_sf2560;
      default:
         return BSR_Config__logicalChannelSR_DelayTimer_sf2560;
   }
}

/************************************************************************
 *
 * @brief Converts actual values into enum value of Paging DRX cycle
 *
 * @details
 *
 *    Function : convertPagingCycleValueToEnum
 *
 *    Functionality: As per Spec 38.331,
 *          Converts Actual values into enum value of Paging DRX cycle 
 *    
 *
 * @params[in] Actual value of PagingDRX
 * @return Enum value of DRX cycle
 *
 * **********************************************************************/
e_PagingDRX convertPagingCycleValueToEnum(uint16_t pagingDrxCycle)
{
   switch(pagingDrxCycle)
   {
      case 32:
            return PagingDRX_v32;
      case 64:
            return PagingDRX_v64;
      case 128:
            return PagingDRX_v128;
      case 256:
            return PagingDRX_v256;
      default:
            return PagingDRX_v32;
   }
}

/************************************************************************
 *
 * @brief Converts enum values into actual value of Paging DRX cycle
 *
 * @details
 *
 *    Function : convertPagingCycleEnumToValue
 *
 *    Functionality: As per Spec 38.331,
 *          Converts enum values into actual value of Paging DRX cycle 
 *    
 *
 * @params[in] Enum value of PagingDRX
 * @return Actual value of DRX cycle
 *    Note: Returning the MAX value in case of Incorrect Enum Value as DRX
 *    cycle is MIN of Various DRX cycles(Mentioned in Spec38.304)
 *
 * **********************************************************************/
uint16_t convertPagingCycleEnumToValue(e_PagingDRX pagingDrx)
{
   switch(pagingDrx)
   {
      case PagingDRX_v32:
            return 32;
      case PagingDRX_v64:
            return 64;
      case PagingDRX_v128:
            return 128;
      case PagingDRX_v256:
            return 256;
      default:
            return 256;
   }
}
/************************************************************************
 *
 * @brief Converts enum values into actual value of Num of Paging Ocassions
 *
 * @details
 *
 *    Function : convertPagingOccEnumToValue
 *
 *    Functionality: As per Spec 38.331,
 *          Converts enum values into actual value of Num of Paging Ocassion
 *    
 *
 * @params[in] Enum value of Ns
 * @return Actual value of Num of Paging Ocassions
 *
 * **********************************************************************/
uint8_t convertNsEnumToValue(e_PCCH_Config__ns Ns)
{
   switch(Ns)
   {
      case PCCH_Config__ns_four:
            return 4;
      case PCCH_Config__ns_two:
            return 2;
      case PCCH_Config__ns_one:
            return 1;
      default:
            return 1;
   }
}

/************************************************************************
 *
 * @brief Converts actual values into enum value of Num of Paging Ocassions
 *
 * @details
 *
 *    Function : convertNsValueToEnum
 *
 *    Functionality: As per Spec 38.331,
 *          Converts actual values into enum value of Num of Paging Ocassions
 *    
 *
 * @params[in] Actual value of Num of Paging Ocassions
 * @return  Enum value of Ns
 *
 * **********************************************************************/
e_PCCH_Config__ns convertNsValueToEnum(uint8_t numPO)
{
   switch(numPO)
   {
      case 1:
            return PCCH_Config__ns_one;
      case 2:
            return PCCH_Config__ns_two;
      case 4:
            return PCCH_Config__ns_four;
      default:
            return PCCH_Config__ns_one;
   }
}

/************************************************************************
 *
 * @brief Converts actual values into enum value of SSB Periodicity in SIB
 *
 * @details
 *
 *    Function : convertSsbPeriodicityValueToEnumForSib
 *
 *    Functionality: 
 *          Converts actual values into enum value of SSB Periodicity in SIB
 *    
 *
 * @params[in] Actual value of SSB Periodicity
 * @return  Enum value of SSB Periodicity
 *
 * **********************************************************************/
e_ServingCellConfigCommonSIB__ssb_PeriodicityServingCell convertSsbPeriodicityValueToEnumForSib(uint16_t ssbPeriodicity)
{
   switch(ssbPeriodicity)
   {
      case 5:
         return ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms5;
      case 10:
         return ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms10;
      case 20:
         return ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms20;
      case 40:
         return ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms40;
      case 80:
         return ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms80;
      case 160:
         return ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms160;
      default:
         return ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms20;
   }
}

/************************************************************************
 *
 * @brief Converts actual values into enum value of SSB Periodicity
 *
 * @details
 *
 *    Function : convertSsbPeriodicityValueToEnum
 *
 *    Functionality: 
 *          Converts actual values into enum value of SSB Periodicity
 *    
 *
 * @params[in] Actual value of SSB Periodicity
 * @return  Enum value of SSB Periodicity
 *
 * **********************************************************************/
e_ServingCellConfigCommon__ssb_periodicityServingCell convertSsbPeriodicityValueToEnum(uint16_t ssbPeriodicity)
{
   switch(ssbPeriodicity)
   {
      case 5:
         return ServingCellConfigCommon__ssb_periodicityServingCell_ms5;
      case 10:
         return ServingCellConfigCommon__ssb_periodicityServingCell_ms10;
      case 20:
         return ServingCellConfigCommon__ssb_periodicityServingCell_ms20;
      case 40:
         return ServingCellConfigCommon__ssb_periodicityServingCell_ms40;
      case 80:
         return ServingCellConfigCommon__ssb_periodicityServingCell_ms80;
      case 160:
         return ServingCellConfigCommon__ssb_periodicityServingCell_ms160;
      default:
         return ServingCellConfigCommon__ssb_periodicityServingCell_ms20;
   }
}

/************************************************************************
 *
 * @brief Converts actual values into enum value of SSB per RACH occassion
 *        for CFRA
 *
 * @details
 *
 *    Function : convertCFRASsbPerRachOccasionValueToEnum
 *
 *    Functionality: 
 *          Converts actual values into enum value of SSB occasion per RACH 
 *          occassion for CFRA
 *
 * @params[in] Actual value of SSB Occasion per RACH
 * @return  Enum value
 *
 * **********************************************************************/
e_CFRA__occasions__ssb_perRACH_Occasion convertCFRASsbPerRachOccasionValueToEnum(uint8_t ssbOccPerRach)
{
   switch(ssbOccPerRach)
   {
      case 1:
         return CFRA__occasions__ssb_perRACH_Occasion_one;
      case 2:
         return CFRA__occasions__ssb_perRACH_Occasion_two;
      case 4:
         return CFRA__occasions__ssb_perRACH_Occasion_four;
      case 8:
         return CFRA__occasions__ssb_perRACH_Occasion_eight;
      case 16:
         return CFRA__occasions__ssb_perRACH_Occasion_sixteen;
      default:
         return CFRA__occasions__ssb_perRACH_Occasion_one;
   }
}

/************************************************************************
 *
 * @brief Converts actual values into enum value of CB Preamble per SSB
 *           for one SSB per RACH occassion
 *
 * @details
 *
 *    Function : convertCbPreamblePerSsbValueToEnum 
 *
 *    Functionality: 
 *          Converts actual values into enum value of CB Preamble per 
 *          SSB for one SSB per RACH occasion
 *
 * @params[in] Actual value of CB preambles per SSB
 * @return  Enum value
 *
 * **********************************************************************/
e_RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one convertCbPreamblePerSsbValueToEnum(uint8_t numCbPreamblePerSsb)
{
   switch(numCbPreamblePerSsb)
   {
      case 4:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n4;
      case 8:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n8;
      case 12:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n12;
      case 16:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n16;
      case 20:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n20;
      case 24:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n24;
      case 28:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n28;
      case 32:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n32;
      case 36:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n36;
      case 40:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n40;
      case 44:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n44;
      case 48:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n48;
      case 52:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n52;
      case 56:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n56;
      case 60:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n60;
      case 64:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n64;
      default:
         return RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n8;
   }
}

/************************************************************************
 *
 * @brief Converts actual values into enum value of DMRS Type A Position
 *        for CFRA
 *
 * @details
 *
 *    Function : convertDmrsTypeAPosValueToEnum
 *
 *    Functionality: 
 *          Converts actual values into enum value of DMRS Type A Position
 *
 * @params[in] Actual value of DMRS Type A Position
 * @return  Enum value
 *
 * **********************************************************************/
e_ServingCellConfigCommon__dmrs_TypeA_Position convertDmrsTypeAPosValueToEnum(uint8_t dmrsTypeAPos)
{
   switch(dmrsTypeAPos)
   {
      case 2:
         return ServingCellConfigCommon__dmrs_TypeA_Position_pos2;
      case 3:
         return ServingCellConfigCommon__dmrs_TypeA_Position_pos3;
      default:
         return ServingCellConfigCommon__dmrs_TypeA_Position_pos2;
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
