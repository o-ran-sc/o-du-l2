#include "common_def.h"
#include "SN-FieldLengthUM.h"
#include "SN-FieldLengthAM.h"
#include "T-PollRetransmit.h"
#include "PollPDU.h"
#include "PollByte.h"
#include "UL-AM-RLC.h"
#include "T-Reassembly.h"
#include "T-StatusProhibit.h"
#include "DL-AM-RLC.h"
#include "RLC-Config.h"
#include "RLC-BearerConfig.h"
#include "BSR-Config.h"
#include "PUCCH-Resource.h"
#include "du_app_rlc_inf.h"

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

SnLenUm covertUmSnLenFromToRrcEnumToIntEnum(SN_FieldLengthUM_t snLen)
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
