#include "common_def.h"
#ifdef INTEL_FAPI
#include "nr5g_fapi_internal.h"
#include "fapi_vendor_extension.h"
#endif
#ifdef INTEL_WLS_MEM
#include "wls_lib.h"
#endif
#ifdef INTEL_API
#include "common_mac_phy_api.h"
#include "nr5g_mac_phy_api.h"
#endif
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "utilities.h"

 /** @ingroup group_source_api_p5_fapi2phy_proc
 *
 *  @param[in]  nSubcCommon  Sub carrier spacing
 *  @param[in]  nDLBandwidth/ nULBandwidth  Sub carrier spacing
 *
 *
 *  @return     IAPI FFT size for DL UL
 *
 *  @description
 *  This function converts FAPI *TddPeriod* to IAPI *nTddPeriod*
 *  structure based on *subCarrierSpacingCommon - 3*.
 *
**/
uint16_t nr5g_fapi_calc_fft_size(
    uint8_t sub_carrier_common,
    uint16_t bw)
{
    if (FAPI_SUBCARRIER_SPACING_15 == sub_carrier_common) {
        if (FAPI_BANDWIDTH_5_MHZ == bw)
            return FAPI_FFT_SIZE_512;
        else if (FAPI_BANDWIDTH_10_MHZ == bw)
            return FAPI_FFT_SIZE_1024;
        else if ((FAPI_BANDWIDTH_15_MHZ == bw) || (FAPI_BANDWIDTH_20_MHZ == bw))
            return FAPI_FFT_SIZE_2048;
        else if ((FAPI_BANDWIDTH_25_MHZ == bw) || (FAPI_BANDWIDTH_30_MHZ == bw)
            || (FAPI_BANDWIDTH_40_MHZ == bw) || (FAPI_BANDWIDTH_50_MHZ == bw))
            return FAPI_FFT_SIZE_4096;
    } else if (FAPI_SUBCARRIER_SPACING_30 == sub_carrier_common) {
        if ((FAPI_BANDWIDTH_5_MHZ == bw) || (FAPI_BANDWIDTH_10_MHZ == bw))
            return FAPI_FFT_SIZE_512;
        else if ((FAPI_BANDWIDTH_15_MHZ == bw) || (FAPI_BANDWIDTH_20_MHZ == bw))
            return FAPI_FFT_SIZE_1024;
        else if ((FAPI_BANDWIDTH_25_MHZ == bw) || (FAPI_BANDWIDTH_30_MHZ == bw)
            || (FAPI_BANDWIDTH_40_MHZ == bw) || (FAPI_BANDWIDTH_50_MHZ == bw))
            return FAPI_FFT_SIZE_2048;
        else if ((FAPI_BANDWIDTH_60_MHZ == bw) || (FAPI_BANDWIDTH_70_MHZ == bw)
            || (FAPI_BANDWIDTH_80_MHZ == bw) || ((FAPI_BANDWIDTH_90_MHZ == bw))
            || (FAPI_BANDWIDTH_100_MHZ == bw))
            return FAPI_FFT_SIZE_4096;
    } else if (FAPI_SUBCARRIER_SPACING_60 == sub_carrier_common) {
        if ((FAPI_BANDWIDTH_10_MHZ == bw) || (FAPI_BANDWIDTH_15_MHZ == bw) ||
            (FAPI_BANDWIDTH_20_MHZ == bw) || (FAPI_BANDWIDTH_25_MHZ == bw))
            return FAPI_FFT_SIZE_512;
        else if ((FAPI_BANDWIDTH_30_MHZ == bw) || (FAPI_BANDWIDTH_40_MHZ == bw)
            || (FAPI_BANDWIDTH_50_MHZ == bw))
            return FAPI_FFT_SIZE_1024;
        else if ((FAPI_BANDWIDTH_60_MHZ == bw) || (FAPI_BANDWIDTH_70_MHZ == bw)
            || (FAPI_BANDWIDTH_80_MHZ == bw) || (FAPI_BANDWIDTH_90_MHZ == bw) ||
            (FAPI_BANDWIDTH_100_MHZ == bw))
            return FAPI_FFT_SIZE_2048;
        else if (FAPI_BANDWIDTH_200_MHZ == bw)
            return FAPI_FFT_SIZE_4096;
    } else if (FAPI_SUBCARRIER_SPACING_120 == sub_carrier_common) {
        if (FAPI_BANDWIDTH_50_MHZ == bw)
            return FAPI_FFT_SIZE_512;
        else if (FAPI_BANDWIDTH_100_MHZ == bw)
            return FAPI_FFT_SIZE_1024;
        else if (FAPI_BANDWIDTH_200_MHZ == bw)
            return FAPI_FFT_SIZE_2048;
        else if (FAPI_BANDWIDTH_400_MHZ == bw)
            return FAPI_FFT_SIZE_4096;
    } else {
    }

    return 0;
}

void setUlSlotInfo(uint16_t sfn, uint16_t slot, UlSlotInfo *ulSlotInfo)
{
   memset(ulSlotInfo, 0, sizeof(UlSlotInfo));
   ulSlotInfo->sfn = sfn;
   ulSlotInfo->slot = slot;
}

UlSlotInfo *getUlSlotInfo(uint16_t sfn, uint16_t slot)
{
   if((gUlSlotInfo[sfn][slot].sfn == sfn) &&
      (gUlSlotInfo[sfn][slot].slot == slot))
   {
      return &gUlSlotInfo[sfn][slot];
   }
   return NULLP;
}

PucchInfo *fetchPucchInfo(uint16_t ueId, UlSlotInfo *ulSlotInfo)
{
   uint8_t idx=0;
   for(idx=0; idx < ulSlotInfo->numPucch; idx++)
   {
      if(ulSlotInfo->pucchInfo[idx].handle == ueId)
         return &ulSlotInfo->pucchInfo[idx];
   }
   return NULLP;
}

PuschInfo *fetchPuschInfo(uint16_t ueId, UlSlotInfo *ulSlotInfo)
{
   uint8_t idx=0;
   for(idx=0; idx < ulSlotInfo->numPusch; idx++)
   {
      if(ulSlotInfo->puschInfo[idx].handle == ueId)
         return &ulSlotInfo->puschInfo[idx];
   }
   return NULLP;
}

