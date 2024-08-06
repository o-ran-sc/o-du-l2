
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "/root/pborla/29July/l2/src/intel_fapi/fapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/fapi_vendor_extension.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nfapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nr5g_fapi_common_types.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nr5g_fapi_internal.h"

void decode_fapiSupport_t(fapiSupport_t support) 
{
   printf("\n");
   switch (support) 
   {
      case FAPI_NOT_SUPPORTED:
         printf("FAPI_NOT_SUPPORTED\t");
         break;
      case FAPI_SUPPORTED:
         printf("FAPI_SUPPORTED\t");
         break;
      default:
         printf("Unknown fapiSupport_t value\t");
   }
}

void decode_fapiTlvStatus_t(fapiTlvStatus_t status) 
{
   printf("\n");
   switch (status) 
   {
      case FAPI_IDLE_STATE_ONLY_OPTIONAL:
         printf("FAPI_IDLE_STATE_ONLY_OPTIONAL\t");
         break;
      case FAPI_IDLE_STATE_ONLY_MANDATORY:
         printf("FAPI_IDLE_STATE_ONLY_MANDATORY\t");
         break;
      case FAPI_IDLE_AND_CONFIGURED_STATES_OPTIONAL:
         printf("FAPI_IDLE_AND_CONFIGURED_STATES_OPTIONAL\t");
         break;
      case FAPI_IDLE_STATE_MANDATORY_CONFIGURED_STATE_OPTIONAL:
         printf("FAPI_IDLE_STATE_MANDATORY_CONFIGURED_STATE_OPTIONAL\t");
         break;
      case FAPI_IDLE_CONFIGURED_AND_RUNNING_STATES_OPTIONAL:
         printf("FAPI_IDLE_CONFIGURED_AND_RUNNING_STATES_OPTIONAL\t");
         break;
      case FAPI_IDLE_STATE_MANDATORY_CONFIGURED_AND_RUNNING_STATES_OPTIONAL:
         printf("FAPI_IDLE_STATE_MANDATORY_CONFIGURED_AND_RUNNING_STATES_OPTIONAL\t");
         break;
      default:
         printf("Unknown fapiTlvStatus_t value\t");
   }
}

void decode_fapiModOrder_t(fapiModOrder_t modOrder) 
{
   printf("\n");
   switch (modOrder) 
   {
      case FAPI_QPSK:
         printf("FAPI_QPSK\t");
         break;
      case FAPI_16QAM:
         printf("FAPI_16QAM\t");
         break;
      case FAPI_64QAM:
         printf("FAPI_64QAM\t");
         break;
      case FAPI_256QAM:
         printf("FAPI_256QAM\t");
         break;
      default:
         printf("Unknown fapiModOrder_t value\t");
   }
}

void decode_fapiSsbPerRach_t(fapiSsbPerRach_t ssbPerRach)
{
   printf("\n");
   switch (ssbPerRach) 
   {
      case FAPI_SSB_PER_RACH_1_OVER_8:
         printf("FAPI_SSB_PER_RACH_1_OVER_8\t");
         break;
      case FAPI_SSB_PER_RACH_1_OVER_4:
         printf("FAPI_SSB_PER_RACH_1_OVER_4\t");
         break;
      case FAPI_SSB_PER_RACH_1_OVER_2:
         printf("FAPI_SSB_PER_RACH_1_OVER_2\t");
         break;
      case FAPI_SSB_PER_RACH_1:
         printf("FAPI_SSB_PER_RACH_1\t");
         break;
      case FAPI_SSB_PER_RACH_2:
         printf("FAPI_SSB_PER_RACH_2\t");
         break;
      case FAPI_SSB_PER_RACH_4:
         printf("FAPI_SSB_PER_RACH_4\t");
         break;
      case FAPI_SSB_PER_RACH_8:
         printf("FAPI_SSB_PER_RACH_8\t");
         break;
      case FAPI_SSB_PER_RACH_16:
         printf("FAPI_SSB_PER_RACH_16\t");
         break;
      default:
         printf("Unknown fapiSsbPerRach_t value\t");
   }
}
