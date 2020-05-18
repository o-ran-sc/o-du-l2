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

/* This file contains ASN codec for MIB and SIB1 msgs */

#include "du_mgr.h"
#include "du_log.h"
#include "BCCH-BCH-Message.h"
#include "MIB.h"
#include "PLMN-IdentityInfo.h"
#include "PLMN-IdentitY.h"
#include "SIB-TypeInfo.h"
#include "SchedulingInfo.h"
#include "SI-SchedulingInfo.h"
#include "ConnEstFailureControl.h"
#include "FrequencyInfoDL-SIB.h"
#include "SearchSpace.h"
#include "SCS-SpecificCarrier.h"
#include "NR-MultiBandInfo.h"
#include "PDCCH-ConfigCommon.h"
#include "PDSCH-TimeDomainResourceAllocation.h"
#include "PDSCH-TimeDomainResourceAllocationList.h"
#include "PDSCH-ConfigCommon.h"
#include "BWP-DownlinkCommon.h"
#include "DownlinkConfigCommonSIB.h"
#include "RACH-ConfigCommon.h"
#include "PUSCH-TimeDomainResourceAllocation.h"
#include "PUSCH-TimeDomainResourceAllocationList.h"
#include "PUSCH-ConfigCommon.h"
#include "PUCCH-ConfigCommon.h"
#include "UplinkConfigCommonSIB.h"
#include "TDD-UL-DL-ConfigCommon.h"
#include "ServingCellConfigCommonSIB.h"
#include "MCC.h"
#include "SIB1.h"
#include "du_sys_info_hdl.h"

extern char encBuf[ENC_BUF_MAX_LEN];

extern DuCfgParams duCfgParam;

/*******************************************************************
 *
 * @brief Builds MIB
 *
 * @details
 *
 *    Function : BuildMib
 *
 *    Functionality: Building MIB
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildMib(MIB_t *mib)
{
   mib->systemFrameNumber.size = sizeof(uint8_t);
	DU_ALLOC(mib->systemFrameNumber.buf, 
			mib->systemFrameNumber.size);
	if(!(mib->systemFrameNumber.buf))
	{
		DU_LOG("DU APP: MIB msg memory allocation failure");
		return RFAILED;
	}

	*(mib->systemFrameNumber.buf) =
		duCfgParam.mibParams.sysFrmNum;
	mib->systemFrameNumber.bits_unused = ODU_VALUE_TWO;

	mib->subCarrierSpacingCommon =\
		duCfgParam.mibParams.subCarrierSpacingCommon;
	mib->ssb_SubcarrierOffset =\
	   duCfgParam.mibParams.ssb_SubcarrierOffset;
	mib->dmrs_TypeA_Position =\
		duCfgParam.mibParams.dmrs_TypeA_Position;
	mib->pdcch_ConfigSIB1.controlResourceSetZero = \
	   duCfgParam.mibParams.controlResourceSetZero;
	mib->pdcch_ConfigSIB1.searchSpaceZero = \
	   duCfgParam.mibParams.searchSpaceZero;
	mib->cellBarred = duCfgParam.mibParams.cellBarred;
	mib->intraFreqReselection =
		duCfgParam.mibParams.intraFreqReselection;
	mib->spare.size = sizeof(uint8_t);
	DU_ALLOC(mib->spare.buf,sizeof(uint8_t));
	if(!mib->spare.buf)
	{
		DU_LOG("DU APP: MIB msg memory allocation failure");
		return RFAILED;
	}
	*(mib->spare.buf) = SPARE;
	mib->spare.bits_unused = ODU_VALUE_SEVEN;
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds MIB PDU for broadcast
 *
 * @details
 *
 *    Function : BuildMibPdu
 *
 *    Functionality: Building MIB PDU for system broadcast
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildMibPdu()
{
   BCCH_BCH_Message_t *bcchMsg;
	asn_enc_rval_t encRetVal;        /* Encoder return value */

   DU_ALLOC(bcchMsg, sizeof(BCCH_BCH_Message_t));
   if(!bcchMsg)
   {
      DU_LOG("\nMemory allocation failure in BuildMibPdu");
      return RFAILED;
   }

   bcchMsg->message.present = BCCH_BCH_MessageType_PR_mib;
   DU_ALLOC(bcchMsg->message.choice.mib, sizeof(MIB_t));
   if(!bcchMsg->message.choice.mib)
   {
      DU_LOG("\nMemory allocation failure in BuildMibPdu");
      return RFAILED;
   }
   BuildMib(bcchMsg->message.choice.mib);
	xer_fprint(stdout, &asn_DEF_BCCH_BCH_Message, bcchMsg);
	cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
	encBufSize = 0;
	encRetVal = aper_encode(&asn_DEF_BCCH_BCH_Message, 0,
				   bcchMsg, PrepFinalEncBuf, encBuf);
	printf("\nencbufSize:%d\n", encBufSize);
	if(encRetVal.encoded	== -1) 
	{   
		DU_LOG("\nDU APP: Could not encode BCCH BCH Message Type structure(at %s)\n", 
				encRetVal.failed_type?\
				encRetVal.failed_type->name
				:"unknown");
		return RFAILED;
	}  

	/* Print encoded buffer */
	for(int i=0; i< encBufSize; i++)
	{
		printf("%x\t",encBuf[i]);
	} 
	printf("\n");

	/* Free allocated memory */
	DU_FREE(bcchMsg->message.choice.mib->systemFrameNumber.buf,
			bcchMsg->message.choice.mib->systemFrameNumber.size);
	DU_FREE(bcchMsg->message.choice.mib->spare.buf, 
         bcchMsg->message.choice.mib->spare.size);
   DU_FREE(bcchMsg->message.choice.mib, sizeof(MIB_t));
   DU_FREE(bcchMsg, sizeof(BCCH_BCH_MessageType_t));

   return ROK;

}
/*******************************************************************
 *
 * @brief Builds MIB message in Served Cell Info
 *
 * @details
 *
 *    Function : BuildMibMsg
 *
 *    Functionality: Building MIB message in Served Cell Info
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildMibMsg()
{
	MIB_t          *mibMsg;
	asn_enc_rval_t encRetVal;        /* Encoder return value */

	DU_ALLOC(mibMsg, sizeof(MIB_t));
	if(!mibMsg)
	{
		DU_LOG("DU APP: MIB msg memory allocation failure");
		return RFAILED;
	}
	BuildMib(mibMsg);

	xer_fprint(stdout, &asn_DEF_MIB, mibMsg);
	cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
	encBufSize = 0;
	encRetVal = aper_encode(&asn_DEF_MIB, 0,
				   mibMsg, PrepFinalEncBuf, encBuf);
	printf("\nencbufSize:%d\n", encBufSize);
	if(encRetVal.encoded	== -1) 
	{   
		DU_LOG("\nDU APP: Could not encode MIB structure(at %s)\n", 
				encRetVal.failed_type?\
				encRetVal.failed_type->name
				:"unknown");
		return RFAILED;
	}  

	/* Print encoded buffer */
	for(int i=0; i< encBufSize; i++)
	{
		printf("%x\t",encBuf[i]);
	} 
	printf("\n");

	/* Free allocated memory */
	DU_FREE(mibMsg->systemFrameNumber.buf,
			mibMsg->systemFrameNumber.size);
	DU_FREE(mibMsg->spare.buf, mibMsg->spare.size);
	DU_FREE(mibMsg, sizeof(MIB_t));

   return ROK;

}

/*******************************************************************
 *
 * @brief Builds CellIdentity  within SIB1 message
 *
 * @details
 *
 *    Function : BuildCellIdentity
 *
 *    Functionality: Building RAN area code in SIB1 message 
 *
 * @params[in] RAN_AreaCode_t **ranAreaCode 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildCellIdentity(CellIdentity_t  *cellIdentity)
{
   cellIdentity->size = ODU_VALUE_FIVE*sizeof(uint8_t);
	cellIdentity->bits_unused = ODU_VALUE_FOUR;

	DU_ALLOC(cellIdentity->buf, cellIdentity->size);
	if(!cellIdentity->buf)
	{   
		DU_LOG("DU APP: CellIdentity memory allocation failure");
		return RFAILED;
	}   
   *cellIdentity->buf = duCfgParam.sib1Params.cellIdentity;

   return ROK;
}
/*******************************************************************
 *
 * @brief Builds RANAC within SIB1 message
 *
 * @details
 *
 *    Function : BuildRanac
 *
 *    Functionality: Building RAN area code in SIB1 message 
 *
 * @params[in] RAN_AreaCode_t **ranAreaCode 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildRanac(RAN_AreaCode_t **ranAreaCode)
{
   RAN_AreaCode_t *ranac;
   DU_ALLOC(ranac, sizeof(RAN_AreaCode_t));
	if(!ranac)
	{   
		DU_LOG("DU APP: RANAC memory allocation failure");
		return RFAILED;
	}   
   *ranac = duCfgParam.sib1Params.ranac;
	*ranAreaCode = ranac;
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds TAC within SIB1 message
 *
 * @details
 *
 *    Function : BuildTac
 *
 *    Functionality: Building Tracking area code in SIB1 message 
 *
 * @params[in] TrackingAreaCode_t **tac 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildTac(TrackingAreaCode_t **trackAreaCode)
{
   TrackingAreaCode_t *tac;
  

   DU_ALLOC(tac, sizeof(TrackingAreaCode_t));
	if(!tac)
	{   
		DU_LOG("DU APP: TAC memory allocation failure");
		return RFAILED;
	}   

	tac->size = ODU_VALUE_THREE*sizeof(uint8_t);
	DU_ALLOC(tac->buf, tac->size);
	if(!tac->buf)
	{   
		DU_LOG("DU APP: TAC memory allocation failure");
		return RFAILED;
	}   
	*(tac->buf) = duCfgParam.sib1Params.tac;
	tac->bits_unused = ODU_VALUE_ZERO;
	*trackAreaCode = tac;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PLMN list within SIB1 message
 *
 * @details
 *
 *    Function : BuildPlmnList
 *
 *    Functionality: Building PLMN list in SIB1 message 
 *
 * @params[in] CellAccessRelatedInfo_t *cellAccessInfo 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildPlmnList(CellAccessRelatedInfo_t *cellAccessInfo)
{
   U8                 idx, idx1, idx2;
	U8                 elementCnt;
	TrackingAreaCode_t **tac;
	RAN_AreaCode_t     **ranac;
	CellIdentity_t     *cellIdentity;
	struct PLMN_IdentityInfo__plmn_IdentityList
		*plmnIdInfo;

	DU_ALLOC(cellAccessInfo->plmn_IdentityList.list.array,
			cellAccessInfo->plmn_IdentityList.list.size);
	if(!cellAccessInfo->plmn_IdentityList.list.array)
	{   
		DU_LOG("DU APP: BuildPlmnList memory allocation failure");
		return RFAILED;
	}   
   elementCnt = cellAccessInfo->plmn_IdentityList.list.count; 
	for(idx=0; idx<elementCnt; idx++)
	{   
		DU_ALLOC(cellAccessInfo->plmn_IdentityList.list.array[idx],
				sizeof(PLMN_IdentityInfo_t));
		if(!cellAccessInfo->plmn_IdentityList.list.array[idx])
		{
			DU_LOG("DU APP: BuildPlmnList memory allocation failure");
			return RFAILED;
		}
	}

	idx = 0;
	/* PLMN list */
	elementCnt = ODU_VALUE_ONE;

	plmnIdInfo = &cellAccessInfo->plmn_IdentityList.list.array[idx]->\
					 plmn_IdentityList;
	plmnIdInfo->list.count = elementCnt;
	plmnIdInfo->list.size  = elementCnt * sizeof(PLMN_IdentitY_t);
	DU_ALLOC(plmnIdInfo->list.array, plmnIdInfo->list.size);
	if(!plmnIdInfo->list.array)
	{
		DU_LOG("DU APP: BuildPlmnList memory allocation failure");
		return RFAILED;
	}

	for(idx1=0; idx1<elementCnt; idx1++)
	{
		DU_ALLOC(plmnIdInfo->list.array[idx1],
				sizeof(PLMN_IdentitY_t));
		if(!(plmnIdInfo->list.array[idx1]))
		{
			DU_LOG("DU APP: BuildPlmnList memory allocation failure");
			return RFAILED;
		}
	}
	idx1 = 0;
	DU_ALLOC(plmnIdInfo->list.array[idx1]->mcc,
			sizeof(MCC_t));
	if(!plmnIdInfo->list.array[idx1]->mcc)
	{
		DU_LOG("DU APP: BuildPlmnList memory allocation failure");
		return RFAILED;
	}

   elementCnt = ODU_VALUE_THREE;
	plmnIdInfo->list.array[idx1]->mcc->list.count =	elementCnt;
	plmnIdInfo->list.array[idx1]->mcc->list.size =\
		elementCnt * sizeof(MCC_MNC_Digit_t);
	DU_ALLOC(plmnIdInfo->list.array[idx1]->mcc->list.array,
			plmnIdInfo->list.array[idx1]->mcc->list.size)
	if(!(plmnIdInfo->list.array[idx1]->mcc->list.array))
	{
		DU_LOG("DU APP: BuildPlmnList memory allocation failure");
		return RFAILED;
	}
	for(idx2=0; idx2<elementCnt; idx2++)
	{
		DU_ALLOC(plmnIdInfo->list.array[idx1]->mcc->list.array[idx2],
				sizeof(MCC_MNC_Digit_t));
		if(!plmnIdInfo->list.array[idx1]->mcc->list.array[idx2])
		{
			DU_LOG("DU APP: BuildPlmnList memory allocation failure");
			return RFAILED;
		}
		*(plmnIdInfo->list.array[idx1]->mcc->list.array[idx2])=\
			duCfgParam.sib1Params.plmn.mcc[idx2];
	}
	idx2 = 0;
	plmnIdInfo->list.array[idx1]->mnc.list.count = elementCnt;
	plmnIdInfo->list.array[idx1]->mnc.list.size =\
		elementCnt * sizeof(MCC_MNC_Digit_t);
	DU_ALLOC(plmnIdInfo->list.array[idx1]->mnc.list.array,
			plmnIdInfo->list.array[idx1]->mnc.list.size);
	if(!plmnIdInfo->list.array[idx1]->mnc.list.array)
	{
		DU_LOG("DU APP: BuildPlmnList memory allocation failure");
		return RFAILED;
	}
	for(idx2=0; idx2<elementCnt; idx2++)
	{
		DU_ALLOC(plmnIdInfo->list.array[idx1]->mnc.list.array[idx2],
				sizeof(MCC_MNC_Digit_t));
		if(!plmnIdInfo->list.array[idx1]->mnc.list.array[idx2])
		{
			DU_LOG("DU APP: BuildPlmnList memory allocation failure");
			return RFAILED;
		}
		*(plmnIdInfo->list.array[idx1]->mnc.list.array[idx2])=\
			duCfgParam.sib1Params.plmn.mnc[idx2];
	}

   /* Tracking Area Code */
   tac = &cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode;
	BuildTac(tac);

   /* RANAC */
	ranac = &cellAccessInfo->plmn_IdentityList.list.array[idx]->ranac;
	BuildRanac(ranac);

   /* CellIdentity */
   cellIdentity =\
	&cellAccessInfo->plmn_IdentityList.list.array[idx]->cellIdentity;
	BuildCellIdentity(cellIdentity);

	/* cellReservedForOperatorUse */
	cellAccessInfo->plmn_IdentityList.list.array[idx]->\
		cellReservedForOperatorUse = \
		duCfgParam.sib1Params.cellResvdForOpUse;

 
   return ROK;
}

/*******************************************************************
 *
 * @brief  Build SIB Map Info List
 *
 * @details
 *
 *    Function : BuildSibMapInfoList
 *
 *    Functionality:
 *       Build SIB MapInfo List
 *
 * @params[in] SIB_Mapping_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildSibMapInfoList(SIB_Mapping_t *sibMapInfo)
{
   uint8_t   itr;         /* List iterator */
   uint8_t   elementCnt;  /* Number of elements in list */
   SIB_TypeInfo_t *sibTypeInfo;  /* Scheduling info */

   /* SI scheduling info list */
   DU_ALLOC(sibMapInfo->list.array, sibMapInfo->list.size);
   if(!sibMapInfo->list.array)
   {
      DU_LOG("DU APP: BuildSibMapInfoList memory allocation failure");
      return RFAILED;
   }

   elementCnt = sibMapInfo->list.count;
   for(itr = 0; itr < elementCnt; itr++)
   {
      DU_ALLOC(sibMapInfo->list.array[itr], sizeof(SIB_TypeInfo_t));
      if(!sibMapInfo->list.array[itr])
      {
         DU_LOG("DU APP: BuildSibMapInfoList memory allocation failure");
         return RFAILED;
      }
   }
 
   itr = 0;
   sibTypeInfo = sibMapInfo->list.array[itr];
   sibTypeInfo->type = duCfgParam.sib1Params.siSchedInfo.sibType;
   DU_ALLOC(sibTypeInfo->valueTag, sizeof(long));
   if(!sibTypeInfo->valueTag)
   {
      DU_LOG("DU APP: BuildSibMapInfoList memory allocation failure");
      return RFAILED;
   }

   *sibTypeInfo->valueTag = duCfgParam.sib1Params.siSchedInfo.sibValTag;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds SI Scheduling info list within SIB1 message 
 *
 * @details
 *
 *    Function : BuildSiSchedInfoList 
 *
 *    Functionality: 
 *        Building SI Scheduling info list in SIB1 message
 *
 * @params[in] struct SI_SchedulingInfo__schedulingInfoList *si_SchedulingInfo
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildSiSchedInfoList(struct SI_SchedulingInfo__schedulingInfoList *si_SchedulingInfoList)
{
   uint8_t   itr;         /* List iterator */
   uint8_t   elementCnt;  /* Number of elements in list */
   SchedulingInfo_t *schedInfo;  /* Scheduling info */

   /* SI scheduling info list */
   DU_ALLOC(si_SchedulingInfoList->list.array, si_SchedulingInfoList->list.size);
   if(!si_SchedulingInfoList->list.array)
   {
      DU_LOG("DU APP: BuildSiSchedInfoList memory allocation failure");
      return RFAILED;
   }
   
   elementCnt = si_SchedulingInfoList->list.count;
   for(itr = 0; itr < elementCnt; itr++)
   {
      DU_ALLOC(si_SchedulingInfoList->list.array[itr], sizeof(struct SchedulingInfo));
      if(!si_SchedulingInfoList->list.array[itr])
      {
         DU_LOG("DU APP: BuildSiSchedInfoList memory allocation failure");
         return RFAILED;
      }
   }

   itr = 0;
   schedInfo = si_SchedulingInfoList->list.array[itr];
   schedInfo->si_BroadcastStatus = duCfgParam.sib1Params.siSchedInfo.broadcastSta;
   schedInfo->si_Periodicity = duCfgParam.sib1Params.siSchedInfo.preiodicity;

   elementCnt = ODU_VALUE_ONE; 
   schedInfo->sib_MappingInfo.list.count = elementCnt;
   schedInfo->sib_MappingInfo.list.size = elementCnt * sizeof(SIB_TypeInfo_t);
   
   BuildSibMapInfoList(&schedInfo->sib_MappingInfo);

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds SCS Specific carrier list for DL 
 *
 * @details
 *
 *    Function : BuildScsSpecificCarrierListDl
 *
 *    Functionality:
 *       Builds Subcarrier specific carrier list for downlink
 *
 * @params[in] FrequencyInfoDL_SIB__scs_SpecificCarrierList pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildScsSpecificCarrierListDl
(
struct FrequencyInfoDL_SIB__scs_SpecificCarrierList *scsCarrierList
)
{
   uint8_t idx;
   ScsSpecCarrier   duScsSpecCarrier;

   duScsSpecCarrier = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.dlScsCarrier;

   DU_ALLOC(scsCarrierList->list.array, scsCarrierList->list.size);
   if(!scsCarrierList->list.array)
   {
      DU_LOG("\nDU APP : SCS Specific Carrier list memory allocation failed"); 
      return RFAILED;
   }

   for(idx = 0; idx < scsCarrierList->list.count; idx++)
   {
      DU_ALLOC(scsCarrierList->list.array[idx], sizeof(SCS_SpecificCarrier_t));
      if(!scsCarrierList->list.array[idx])
      {
         DU_LOG("\nDU APP : SCS Specific Carrier list memory allocation failed");
         return RFAILED;
      }
   }

   idx = 0;
   scsCarrierList->list.array[idx]->offsetToCarrier = duScsSpecCarrier.scsOffset; 
   scsCarrierList->list.array[idx]->subcarrierSpacing = duScsSpecCarrier.scs;
   scsCarrierList->list.array[idx]->carrierBandwidth = duScsSpecCarrier.scsBw;

   return ROK;
} /* End of BuildScsSpecificCarrierListDl */

/*******************************************************************
 *
 * @brief Builds common search space list 
 *
 * @details
 *
 *    Function : BuildCommonSerachSpaceList
 *
 *    Functionality:
 *      Builds common search space list
 *
 * @params[in] PDCCH_ConfigCommon__commonSearchSpaceList pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildCommonSerachSpaceList
(
struct PDCCH_ConfigCommon__commonSearchSpaceList *searchSpclist
)
{
   uint8_t idx;
   SearchSpace_t    *searchSpace;
   PdcchCfgCommon   duPdcchCfg;

   duPdcchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pdcchCfg;

   DU_ALLOC(searchSpclist->list.array, searchSpclist->list.size)
   if(!searchSpclist->list.array)
   {
      DU_LOG("\nDU APP : Common search space list memory alloc failed");
      return RFAILED;
   }

   for(idx = 0; idx < searchSpclist->list.count; idx++)
   {
      DU_ALLOC(searchSpclist->list.array[idx], sizeof(SearchSpace_t));
      if(!searchSpclist->list.array[idx])
      {
         DU_LOG("\nDU APP : Common search space list memory alloc failed");
         return RFAILED;
      }
   }

   idx = 0;
   searchSpace = searchSpclist->list.array[idx];

   /* Search Space id */
   searchSpace->searchSpaceId = duPdcchCfg.searchSpcId;

   /* Control Resource Set Id */
   DU_ALLOC(searchSpace->controlResourceSetId, sizeof(ControlResourceSetId_t));
   if(!searchSpace->controlResourceSetId)
   {
      DU_LOG("\nDU APP : Common search space list memory alloc failed");
      return RFAILED;
   }
   *searchSpace->controlResourceSetId = duPdcchCfg.ctrlRsrcSetId;

   /* Monitoring Slot periodicity and offset */
   DU_ALLOC(searchSpace->monitoringSlotPeriodicityAndOffset,\
      sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
   if(!searchSpace->monitoringSlotPeriodicityAndOffset)
   {
      DU_LOG("\nDU APP : Common search space list memory alloc failed");
      return RFAILED;
   }
   searchSpace->monitoringSlotPeriodicityAndOffset->present = \
      duPdcchCfg.monitorSlotPrdAndOffPresent;
   switch(searchSpace->monitoringSlotPeriodicityAndOffset->present)
   {
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1:
      {
         //searchSpace->monitoringSlotPeriodicityAndOffset->choice.s11 = \
            duPdcchCfg.monitorSlotPrdAndOff;
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl2:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl4:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl5:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl8:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl10:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl16:
      {
         //TODO;
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl20:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl40:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl80:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl160:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl320:
      {
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl640:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1280:
      {
         //TODO
         break;
      }
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl2560:
      {
         //TODO
         break;
      }
      default:
      {
         DU_LOG("\nDU APP : Invalid value:Montoring slot periodicity and offset");
         return RFAILED;
      }
   }
 
   /* Monitoring Symbols Within Slot */
   DU_ALLOC(searchSpace->monitoringSymbolsWithinSlot,\
      sizeof(BIT_STRING_t));
   if(!searchSpace->monitoringSymbolsWithinSlot)
   {
       DU_LOG("\nDU APP : Common search space list memory alloc failed");
       return RFAILED;
   }
   searchSpace->monitoringSymbolsWithinSlot->size = 2;
   DU_ALLOC(searchSpace->monitoringSymbolsWithinSlot->buf, \
      searchSpace->monitoringSymbolsWithinSlot->size * sizeof(uint8_t));
   if(!searchSpace->monitoringSymbolsWithinSlot->buf)
   {  
      DU_LOG("\nDU APP : Common search space list memory alloc failed");
      return RFAILED;
   }
   searchSpace->monitoringSymbolsWithinSlot->buf[0] = duPdcchCfg.monitorSymbolsInSlot[0];
   searchSpace->monitoringSymbolsWithinSlot->buf[1] = duPdcchCfg.monitorSymbolsInSlot[1];
   searchSpace->monitoringSymbolsWithinSlot->bits_unused = 2;

   /* Number of candidates per aggregation level */
   DU_ALLOC(searchSpace->nrofCandidates,\
      sizeof(struct SearchSpace__nrofCandidates));
   if(!searchSpace->nrofCandidates)
   {
      DU_LOG("\nDU APP : Common search space list memory alloc failed");
      return RFAILED;
   }
   searchSpace->nrofCandidates->aggregationLevel1 = duPdcchCfg.numCandAggLvl1;
   searchSpace->nrofCandidates->aggregationLevel2 = duPdcchCfg.numCandAggLvl2;
   searchSpace->nrofCandidates->aggregationLevel4 = duPdcchCfg.numCandAggLvl4;
   searchSpace->nrofCandidates->aggregationLevel8 = duPdcchCfg.numCandAggLvl8;
   searchSpace->nrofCandidates->aggregationLevel16 = duPdcchCfg.numCandAggLvl16;

   /* Search Space type and  DCI Format */
   DU_ALLOC(searchSpace->searchSpaceType,\
      sizeof( struct SearchSpace__searchSpaceType));
   if(!searchSpace->searchSpaceType)
   {
      DU_LOG("\nDU APP : Common search space list memory alloc failed");
      return RFAILED;
   }

   searchSpace->searchSpaceType->present = duPdcchCfg.searchSpcType; 
   switch(searchSpace->searchSpaceType->present)
   {
      case SearchSpace__searchSpaceType_PR_NOTHING:
      {
        //TODO
        break;
      }
      case SearchSpace__searchSpaceType_PR_common:
      {
         DU_ALLOC(searchSpace->searchSpaceType->choice.common,\
            sizeof(struct SearchSpace__searchSpaceType__common));
         if(!searchSpace->searchSpaceType->choice.common)
         {
            DU_LOG("\nDU APP : Common search space list memory alloc failed");
            return RFAILED;
         }

         DU_ALLOC(searchSpace->searchSpaceType->choice.common->\
            dci_Format0_0_AndFormat1_0, sizeof(struct \
            SearchSpace__searchSpaceType__common__dci_Format0_0_AndFormat1_0));
         if(!searchSpace->searchSpaceType->choice.common->dci_Format0_0_AndFormat1_0)
         {
            DU_LOG("\nDU APP : Common search space list memory alloc failed");
            return RFAILED;
         }
         break;
      }
      case SearchSpace__searchSpaceType_PR_ue_Specific:
      {
         break;
      }
      default:
      {
         DU_LOG("\nDU_APP: Invalid Search Space type");
         return RFAILED;
      }

   }

   return ROK;
}/* BuildCommonSerachSpaceList */

/*******************************************************************
 *
 * @brief Builds PDCCH configuration common 
 *
 * @details
 *
 *    Function : BuildPdcchCfgCommon
 *
 *    Functionality:
 *      Builds PDCCH configuration common
 *
 * @params[in] BWP_DownlinkCommon__pdcch_ConfigCommon pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildPdcchCfgCommon(struct BWP_DownlinkCommon__pdcch_ConfigCommon *pdcchCfg)
{
   uint8_t   elementCnt;
   PdcchCfgCommon   duPdcchCfg;
   PDCCH_ConfigCommon_t  *pdcchSetup;

   duPdcchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pdcchCfg;

   pdcchCfg->present = duPdcchCfg.present;

   switch(pdcchCfg->present)
   {
      case BWP_DownlinkCommon__pdcch_ConfigCommon_PR_NOTHING:
      {
         //TODO
         break;
      }
      case BWP_DownlinkCommon__pdcch_ConfigCommon_PR_release:
      {
         //TODO
         break;
      }
      case BWP_DownlinkCommon__pdcch_ConfigCommon_PR_setup:
      {
         DU_ALLOC(pdcchCfg->choice.setup, sizeof(PDCCH_ConfigCommon_t));
         if(!pdcchCfg->choice.setup)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         pdcchSetup = pdcchCfg->choice.setup;
         
         /* Control Resource Set Zero */
         DU_ALLOC(pdcchSetup->controlResourceSetZero, 
            sizeof(ControlResourceSetZero_t)); 
         if(!pdcchSetup->controlResourceSetZero)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         *pdcchSetup->controlResourceSetZero = duPdcchCfg.ctrlRsrcSetZero;
         
         /* Search space zero */
         DU_ALLOC(pdcchSetup->searchSpaceZero, sizeof(SearchSpaceZero_t));
         if(!pdcchSetup->searchSpaceZero)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         *pdcchSetup->searchSpaceZero = duPdcchCfg.searchSpcZero;
        
         /* Common Search Space List */
         DU_ALLOC(pdcchSetup->commonSearchSpaceList,\
            sizeof(struct PDCCH_ConfigCommon__commonSearchSpaceList));
         if(!pdcchSetup->commonSearchSpaceList)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         elementCnt = ODU_VALUE_ONE;
         pdcchSetup->commonSearchSpaceList->list.count = elementCnt;
         pdcchSetup->commonSearchSpaceList->list.size = \
            elementCnt * sizeof(SearchSpace_t);
         BuildCommonSerachSpaceList(pdcchSetup->commonSearchSpaceList);

         /* Search Space for SIB1 */
         DU_ALLOC(pdcchSetup->searchSpaceSIB1, sizeof(SearchSpaceId_t));
         if(!pdcchSetup->searchSpaceSIB1)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         *pdcchSetup->searchSpaceSIB1 = duPdcchCfg.searchSpcSib1;

         /* Serach Space for Paging */
         DU_ALLOC(pdcchSetup->pagingSearchSpace, sizeof(SearchSpaceId_t));
         if(!pdcchSetup->pagingSearchSpace)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }  
         *pdcchSetup->pagingSearchSpace = duPdcchCfg.pagingSearchSpc;

         /* Search space for Random Access */
         DU_ALLOC(pdcchSetup->ra_SearchSpace, sizeof(SearchSpaceId_t));
         if(!pdcchSetup->ra_SearchSpace)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }             
         *pdcchSetup->ra_SearchSpace = duPdcchCfg.raSearchSpc;

         break;
      }
      default :
      {
         DU_LOG("\nDU APP : Invalid PDCCH Config type");
         return RFAILED;
      }
   }

   return ROK;
} /* BuildPdcchCfgCommon */

/*******************************************************************
 *
 * @brief Builds PDSCH configuration common 
 *
 * @details
 *
 *    Function : BuildPdschCfgCommon
 *
 *    Functionality:
 *       Builds PDSCH configuration common
 *
 * @params[in] BWP_DownlinkCommon__pdsch_ConfigCommon pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildPdschCfgCommon(struct BWP_DownlinkCommon__pdsch_ConfigCommon *pdschCfg)
{
   uint8_t   idx;
   uint8_t   elementCnt;
   PdschCfgCommon   duPdschCfg;
   PDSCH_ConfigCommon_t  *pdschSetup;
   PDSCH_TimeDomainResourceAllocation_t  *timeDomRsrcAllocInfo;
 
   duPdschCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pdschCfg;
  
   pdschCfg->present = duPdschCfg.present;

   switch(pdschCfg->present)
   {
      case BWP_DownlinkCommon__pdsch_ConfigCommon_PR_NOTHING:
      {
         //TODO
         break;
      }
      case BWP_DownlinkCommon__pdsch_ConfigCommon_PR_release:
      {
         //TODO
         break;
      }
      case BWP_DownlinkCommon__pdsch_ConfigCommon_PR_setup:
      {
         DU_ALLOC(pdschCfg->choice.setup, sizeof(PDSCH_ConfigCommon_t));
         if(!pdschCfg->choice.setup)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         pdschSetup = pdschCfg->choice.setup; 
         
         /* Time Domain Allocation List */
         DU_ALLOC(pdschSetup->pdsch_TimeDomainAllocationList, \
            sizeof(PDSCH_TimeDomainResourceAllocationList_t));
         if(!pdschSetup->pdsch_TimeDomainAllocationList)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         elementCnt = ODU_VALUE_ONE;
         pdschSetup->pdsch_TimeDomainAllocationList->list.count = elementCnt;
         pdschSetup->pdsch_TimeDomainAllocationList->list.size = \
            elementCnt * sizeof(PDSCH_TimeDomainResourceAllocation_t);

         DU_ALLOC(pdschSetup->pdsch_TimeDomainAllocationList->list.array,\
            pdschSetup->pdsch_TimeDomainAllocationList->list.size);
         if(!pdschSetup->pdsch_TimeDomainAllocationList->list.array)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
          
         for(idx=0; idx<elementCnt; idx++)
         {
            DU_ALLOC(pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx],\
               sizeof(PDSCH_TimeDomainResourceAllocation_t));
            if(!pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx])
            {
               DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
               return RFAILED;
            }
         }

         idx = 0;
         timeDomRsrcAllocInfo = pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx];

         /* K0 */
         DU_ALLOC(timeDomRsrcAllocInfo->k0, sizeof(long));
         if(!timeDomRsrcAllocInfo->k0)
         {
            DU_LOG("\nDU APP : PDCCH Config memory alloc failed");
            return RFAILED;
         }
         *timeDomRsrcAllocInfo->k0 = duPdschCfg.k0;
  
         timeDomRsrcAllocInfo->mappingType = duPdschCfg.mapType;
         timeDomRsrcAllocInfo->startSymbolAndLength = duPdschCfg.sliv;

         break;
      }
      default:
      {
         DU_LOG("\nDU APP: Invalid PDSCH Configuration type");
         return RFAILED;
      }
   }

   return ROK;
}/* BuildPdschCfgCommon */

/*******************************************************************
 *
 * @brief Builds DL BWP common 
 *
 * @details
 *
 *    Function : BuildBwpDlCommon
 *
 *    Functionality:
 *      Builds DL BWP common
 *
 * @params[in] BWP_DownlinkCommon_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildBwpDlCommon(BWP_DownlinkCommon_t *bwp)
{
   DlCfgCommon duDlCfg;
 
   duDlCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg;

   bwp->genericParameters.locationAndBandwidth = duDlCfg.locAndBw;
   bwp->genericParameters.subcarrierSpacing = duDlCfg.dlScsCarrier.scs;

   /* PDCCH Config Common */
   DU_ALLOC(bwp->pdcch_ConfigCommon, \
      sizeof(struct BWP_DownlinkCommon__pdcch_ConfigCommon));
   if(!bwp->pdcch_ConfigCommon)
   {
      DU_LOG("\nDU APP : DL BWP memory allocation failed");
      return RFAILED;
   }
   BuildPdcchCfgCommon(bwp->pdcch_ConfigCommon);

   /* PDSCH Config Common */
   DU_ALLOC(bwp->pdsch_ConfigCommon, \
      sizeof(struct BWP_DownlinkCommon__pdsch_ConfigCommon));
   if(!bwp->pdsch_ConfigCommon)
   {
      DU_LOG("\nDU APP : DL BWP memory allocation failed");
      return RFAILED;
   }
   BuildPdschCfgCommon(bwp->pdsch_ConfigCommon);

   return ROK;
} /* BuildBwpDlCommon */

/*******************************************************************
 *
 * @brief Build BCCH configuration 
 *
 * @details
 *
 *    Function : BuildBcchConfig
 *
 *    Functionality:
 *      Build BCCH configuration
 *
 * @params[in] BCCH_Config_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildBcchConfig(BCCH_Config_t *bcchCfg)
{
   BcchCfg   duBcchCfg;

   duBcchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.bcchCfg;

   bcchCfg->modificationPeriodCoeff = duBcchCfg.modPrdCoeff;
   
   return ROK;
}/* BuildBcchConfig */

/*******************************************************************
 *
 * @brief Builds PCCH configuration 
 *
 * @details
 *
 *    Function : BuildPcchConfig
 *
 *    Functionality:
 *        Builds PCCH configuration
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildPcchConfig(PCCH_Config_t *pcchCfg)
{
   PcchCfg   duPcchCfg;
  
   duPcchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg;
 
   pcchCfg->defaultPagingCycle = duPcchCfg.dfltPagingCycle;
   pcchCfg->nAndPagingFrameOffset.present =  duPcchCfg.nAndPagingFrmOffPresent;
   switch(pcchCfg->nAndPagingFrameOffset.present)
   {
      case PCCH_Config__nAndPagingFrameOffset_PR_NOTHING:
      {
         //TODO
         break;
      }
      case PCCH_Config__nAndPagingFrameOffset_PR_oneT:
      {
         break;
      }
      case PCCH_Config__nAndPagingFrameOffset_PR_halfT:
      {
         //TODO
         break;
      }
      case PCCH_Config__nAndPagingFrameOffset_PR_quarterT:
      {
         //TODO
         break;
      }
      case PCCH_Config__nAndPagingFrameOffset_PR_oneEighthT:
      {
         //TODO
         break;
      }
      case PCCH_Config__nAndPagingFrameOffset_PR_oneSixteenthT:
      {
         //TODO
         break;
      }
      default:
      {
         DU_LOG("\nDU APP : Invalid nAndPagingFrameOffset configuration");
         return RFAILED;
      }
   }
   pcchCfg->ns = duPcchCfg.numPagingOcc;

   return ROK;
}/* BuildPcchConfig */

/*******************************************************************
 *
 * @brief Builds Frequency Info for Downlink 
 *
 * @details
 *
 *    Function : BuildFreqInfoDl
 *
 *    Functionality:
 *     Builds Frequency Info for Downlink
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildFreqInfoDl(FrequencyInfoDL_SIB_t *frequencyInfoDL)
{
   uint8_t idx;
   uint8_t elementCnt;
   DlCfgCommon  dlCfg;
   NR_MultiBandInfo_t  *multiBandInfo;

   dlCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg;

   /* NR Multi-Band Info */
   elementCnt = ODU_VALUE_ONE;   
   frequencyInfoDL->frequencyBandList.list.count = elementCnt;  
   frequencyInfoDL->frequencyBandList.list.size = \
     elementCnt * sizeof(NR_MultiBandInfo_t);

   DU_ALLOC(frequencyInfoDL->frequencyBandList.list.array, \
      frequencyInfoDL->frequencyBandList.list.size);
   if(!frequencyInfoDL->frequencyBandList.list.array)
   {
      DU_LOG("\nDU APP : SIB1 DL Configuration memory allocation failed");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      DU_ALLOC(frequencyInfoDL->frequencyBandList.list.array[idx],\
         sizeof(NR_MultiBandInfo_t));
      if(!frequencyInfoDL->frequencyBandList.list.array[idx])
      {
         DU_LOG("\nDU APP : SIB1 DL Configuration memory allocation failed");
         return RFAILED;
      }
   }

   idx = 0;
   multiBandInfo = frequencyInfoDL->frequencyBandList.list.array[idx];

   /* Frequency band indicator */
   DU_ALLOC(multiBandInfo->freqBandIndicatorNR, sizeof(FreqBandIndicatorNR_t));
   if(!multiBandInfo->freqBandIndicatorNR)
   {
      DU_LOG("\nDU APP : SIB1 DL Configuration memory allocation failed");
      return RFAILED;
   }
   *multiBandInfo->freqBandIndicatorNR = dlCfg.freqBandInd;

   /* Offset to Point A */
   frequencyInfoDL->offsetToPointA = dlCfg.offsetToPointA;

   /* Subcarrier Spacing specifc carrier */
   elementCnt = ODU_VALUE_ONE;
   frequencyInfoDL->scs_SpecificCarrierList.list.count = elementCnt;
   frequencyInfoDL->scs_SpecificCarrierList.list.size = \
      elementCnt * sizeof(SCS_SpecificCarrier_t);
   BuildScsSpecificCarrierListDl(&frequencyInfoDL->scs_SpecificCarrierList);

   return ROK;
}/* BuildFreqInfoDl */

/*******************************************************************
 *
 * @brief Builds DL configuration common for SIB
 *
 * @details
 *
 *    Function : BuildDlCfgCommSib
 *
 *    Functionality:
 *     Builds DL configuration common for SIB
 *
 * @params[in] DownlinkConfigCommonSIB_t pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildDlCfgCommSib(DownlinkConfigCommonSIB_t *dlCfg)
{
   /* DL frequency info */
   BuildFreqInfoDl(&dlCfg->frequencyInfoDL);
   
   /* BWP Downlink Common */
   BuildBwpDlCommon(&dlCfg->initialDownlinkBWP);

   /* BCCH Config */
   BuildBcchConfig(&dlCfg->bcch_Config);

   /* PCCH Config */
   BuildPcchConfig(&dlCfg->pcch_Config);

   return ROK;
} /* BuildDlCfgCommSib */

/*******************************************************************
 *
 * @brief Builds SCS specific carrier list for UL 
 *
 * @details
 *
 *    Function : BuildScsSpecificCarrierListUl
 *
 *    Functionality:
 *       Builds SCS specific carrier list for UL
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildScsSpecificCarrierListUl
(
struct FrequencyInfoUL_SIB__scs_SpecificCarrierList *scsCarrierList
)
{
   uint8_t idx;
   ScsSpecCarrier   duScsSpecCarrier;
 
   duScsSpecCarrier = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.ulScsCarrier;
 
   DU_ALLOC(scsCarrierList->list.array, scsCarrierList->list.size);
   if(!scsCarrierList->list.array)
   {
       DU_LOG("\nDU APP : SCS Specific Carrier list memory allocation failed");
       return RFAILED;
   }
 
   for(idx = 0; idx < scsCarrierList->list.count; idx++)
   {
      DU_ALLOC(scsCarrierList->list.array[idx], sizeof(SCS_SpecificCarrier_t));
      if(!scsCarrierList->list.array[idx])
      {
         DU_LOG("\nDU APP : SCS Specific Carrier list memory allocation failed");
         return RFAILED;
      }
   }
   idx = 0;
   scsCarrierList->list.array[idx]->offsetToCarrier = duScsSpecCarrier.scsOffset;
   scsCarrierList->list.array[idx]->subcarrierSpacing = duScsSpecCarrier.scs;
   scsCarrierList->list.array[idx]->carrierBandwidth = duScsSpecCarrier.scsBw;
 
   return ROK;
} /* End of BuildScsSpecificCarrierListUl */

/*******************************************************************
 *
 * @brief Builds frequency infor for UL 
 *
 * @details
 *
 *    Function : BuildFreqInfoUl
 *
 *    Functionality:
 *       Builds frequency infor for UL
 *
 * @params[in] FrequencyInfoUL_SIB_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildFreqInfoUl(FrequencyInfoUL_SIB_t *frequencyInfoUL)
{
   uint8_t elementCnt;
   UlCfgCommon  ulCfg;

   ulCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg;
     
   /* Subcarrier Spacing specifc carrier */
   elementCnt = ODU_VALUE_ONE;
   frequencyInfoUL->scs_SpecificCarrierList.list.count = elementCnt;
   frequencyInfoUL->scs_SpecificCarrierList.list.size = \
     elementCnt * sizeof(SCS_SpecificCarrier_t);
   BuildScsSpecificCarrierListUl(&frequencyInfoUL->scs_SpecificCarrierList);

   /* P-MAX */
   DU_ALLOC(frequencyInfoUL->p_Max, sizeof(P_Max_t));
   if(!frequencyInfoUL->p_Max)
   {
      DU_LOG("\nDU APP : UL Frequency Infoo  memory allocation failure");
      return RFAILED;
   }
   *frequencyInfoUL->p_Max = ulCfg.pMax;

   return ROK;
}/* BuildFreqInfoUl */

/*******************************************************************
 *
 * @brief Builds RACH configuration common 
 *
 * @details
 *
 *    Function : BuildRachCfgCommon
 *
 *    Functionality:
 *      Builds RACH configuration common
 *
 * @params[in] BWP_UplinkCommon__rach_ConfigCommon pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16  BuildRachCfgCommon(struct BWP_UplinkCommon__rach_ConfigCommon *rachCfg)
{
   RachCfgCommon    duRachCfg;
   RACH_ConfigCommon_t  *setup;

   duRachCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.rachCfg;

   rachCfg->present = duRachCfg.present;
   switch(rachCfg->present)
   {
      case BWP_UplinkCommon__rach_ConfigCommon_PR_NOTHING:
      {
         //TODO
         break;
      }
      case BWP_UplinkCommon__rach_ConfigCommon_PR_release:
      {
         //TODO
         break;
      }
      case BWP_UplinkCommon__rach_ConfigCommon_PR_setup:
      {
         DU_ALLOC(rachCfg->choice.setup, sizeof(RACH_ConfigCommon_t)); 
         if(!rachCfg->choice.setup)
         {
            DU_LOG("\nDU APP : Rach Config memory alloc failed");
            return RFAILED;
         }
         setup = rachCfg->choice.setup;

         /* Generic RACH Configuration */
         setup->rach_ConfigGeneric.prach_ConfigurationIndex = duRachCfg.prachCfgIdx;
         setup->rach_ConfigGeneric.msg1_FDM = duRachCfg.msg1Fdm;
         setup->rach_ConfigGeneric.msg1_FrequencyStart = duRachCfg.msg1FreqStart;
         setup->rach_ConfigGeneric.zeroCorrelationZoneConfig = duRachCfg.zeroCorrZoneCfg;
         setup->rach_ConfigGeneric.preambleReceivedTargetPower = duRachCfg.preambleRcvdTgtPwr; 
         setup->rach_ConfigGeneric.preambleTransMax = duRachCfg.preambleTransMax;
         setup->rach_ConfigGeneric.powerRampingStep = duRachCfg.pwrRampingStep;
         setup->rach_ConfigGeneric.ra_ResponseWindow = duRachCfg.raRspWindow;

         /* Total number of RA preambles */
         DU_ALLOC(setup->totalNumberOfRA_Preambles, sizeof(long));
         if(!setup->totalNumberOfRA_Preambles)
         {
            DU_LOG("\nDU APP : Rach Config memory alloc failed");
            return RFAILED;
         }
         *setup->totalNumberOfRA_Preambles = duRachCfg.numRaPreamble;

         /* SSB per RACH occassion and CB Preambles per SSB */
         DU_ALLOC(setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB,\
          sizeof(struct RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB));
         if(!setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB)
         {
            DU_LOG("\nDU APP : Rach Config memory alloc failed");
            return RFAILED;
         }
         setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB->present = \
            duRachCfg.ssbPerRachOccPresent;
         switch(setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB->present)
         {
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_NOTHING:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_oneEighth:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_oneFourth:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_oneHalf:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_one:
            {
               setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB->choice.one =\
                  duRachCfg.numSsbPerRachOcc;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_two:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_four:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_eight:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_sixteen:
            {
               //TODO
               break;
            }
            default:
            {
               DU_LOG("\nDU APP: Invalid value for \
                  ssb_PerRach_OccassionAndCB_PreamblesPerSSB");
               return RFAILED;
            }
         }

         /* RA Contention Resolution Timer */
         setup->ra_ContentionResolutionTimer = duRachCfg.contResTimer;

         /* RSRP Threshold SSB */
         DU_ALLOC(setup->rsrp_ThresholdSSB, sizeof(RSRP_Range_t));
         if(!setup->rsrp_ThresholdSSB)
         {
            DU_LOG("\nDU APP : Rach Config memory alloc failed");
            return RFAILED;
         }
         *setup->rsrp_ThresholdSSB = duRachCfg.rsrpThreshSsb;

         /* Root Sequence index */
         setup->prach_RootSequenceIndex.present = duRachCfg.rootSeqIdxPresent;
         switch(setup->prach_RootSequenceIndex.present)
         {
            case RACH_ConfigCommon__prach_RootSequenceIndex_PR_NOTHING:
            {
               //TODO
               break;
            }
            case RACH_ConfigCommon__prach_RootSequenceIndex_PR_l839:
            {
               //TODO
               break;;
            }
            case RACH_ConfigCommon__prach_RootSequenceIndex_PR_l139:
            {
               setup->prach_RootSequenceIndex.choice.l139 = duRachCfg.rootSeqIdx;
               break;
            }
            default:
            {
               DU_LOG("\nDU APP: Inavlid PRACH root sequence index type");
               return RFAILED;
            }
         }
         
         /* Msg 1 Subcarrier spacing */
         DU_ALLOC(setup->msg1_SubcarrierSpacing, sizeof(SubcarrierSpacing_t));
         if(!setup->msg1_SubcarrierSpacing)
         {
            DU_LOG("\nDU APP : Rach Config memory alloc failed");
            return RFAILED;
         }
         *setup->msg1_SubcarrierSpacing = duRachCfg.msg1Scs;

         /* Restricted Set Config */
         setup->restrictedSetConfig = duRachCfg.restrictedSetCfg;

         break;
      }
      default:
      {
         DU_LOG("\nDU APP : Invalid RACH Config type ");
         return RFAILED;
      }
   }
   return ROK;
}/* BuildRachCfgCommon */

/*******************************************************************
 *
 * @brief Builds PUSCH configuration for common 
 *
 * @details
 *
 *    Function : BuildPuschCfgCommon
 *
 *    Functionality:
 *      Builds PUSCH configuration for common
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildPuschCfgCommon(struct BWP_UplinkCommon__pusch_ConfigCommon *puschCfg)
{
   uint8_t           idx;
   uint8_t           elementCnt;   
   PuschCfgCommon    duPuschCfg;
   PUSCH_ConfigCommon_t *setup;
   PUSCH_TimeDomainResourceAllocation_t  *timeDomRsrcAllocInfo;
  
   duPuschCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.puschCfg;
 
   puschCfg->present = duPuschCfg.present;
   switch(puschCfg->present)
   {
      case BWP_UplinkCommon__pusch_ConfigCommon_PR_NOTHING:
      {
         //TODO
         break;
      }
      case BWP_UplinkCommon__pusch_ConfigCommon_PR_release:
      {
         //TODO
         break;
      }
      case BWP_UplinkCommon__pusch_ConfigCommon_PR_setup:
      {
         DU_ALLOC(puschCfg->choice.setup, sizeof(PUSCH_ConfigCommon_t));
         if(!puschCfg->choice.setup)
         {
            DU_LOG("\nDU APP : PUSCH Config memory alloc failed");
            return RFAILED;
         }
         setup = puschCfg->choice.setup;

         /* Time Domain Resource Allocation List */
         DU_ALLOC(setup->pusch_TimeDomainAllocationList,\
            sizeof(PUSCH_TimeDomainResourceAllocationList_t)); 
         if(!setup->pusch_TimeDomainAllocationList)
         {
            DU_LOG("\nDU APP : PUSCH Config memory alloc failed");
            return RFAILED;
         }
         elementCnt = ODU_VALUE_ONE;
         setup->pusch_TimeDomainAllocationList->list.count = elementCnt;
         setup->pusch_TimeDomainAllocationList->list.size = \
            elementCnt * sizeof(PUSCH_TimeDomainResourceAllocation_t);

         DU_ALLOC(setup->pusch_TimeDomainAllocationList->list.array, \
            setup->pusch_TimeDomainAllocationList->list.size);
         if(!setup->pusch_TimeDomainAllocationList->list.array)
         {
            DU_LOG("\nDU APP : PUSCH Config memory alloc failed");
            return RFAILED;
         }

         for(idx=0; idx<elementCnt; idx++)
         {
            DU_ALLOC(setup->pusch_TimeDomainAllocationList->list.array[idx],\
               sizeof(PUSCH_TimeDomainResourceAllocation_t));
            if(!setup->pusch_TimeDomainAllocationList->list.array[idx])
            {
               DU_LOG("\nDU APP : PUSCH Config memory alloc failed");
               return RFAILED;
            }
         }

         idx = 0;
         timeDomRsrcAllocInfo = setup->pusch_TimeDomainAllocationList->list.array[idx];

         /* K2 */
         DU_ALLOC(timeDomRsrcAllocInfo->k2, sizeof(long));
         if(!timeDomRsrcAllocInfo->k2)
         {
            DU_LOG("\nDU APP : PUSCH Config memory alloc failed");
            return RFAILED;
         }
         *timeDomRsrcAllocInfo->k2 = duPuschCfg.k2;

         timeDomRsrcAllocInfo->mappingType = duPuschCfg.mapType;
         timeDomRsrcAllocInfo->startSymbolAndLength = duPuschCfg.sliv;
         
         /* Msg3 Delta Preamble */
         DU_ALLOC(setup->msg3_DeltaPreamble, sizeof(long));
         if(!setup->msg3_DeltaPreamble)
         {
            DU_LOG("\nDU APP : PUSCH Config memory alloc failed");
            return RFAILED;
         }
         *setup->msg3_DeltaPreamble = duPuschCfg.msg3DeltaPreamble;

         /* P0 Nominal with grnat */
         DU_ALLOC(setup->p0_NominalWithGrant, sizeof(long));
         if(!setup->p0_NominalWithGrant)
         {
            DU_LOG("\nDU APP : PUSCH Config memory alloc failed");
            return RFAILED;
         }
         *setup->p0_NominalWithGrant = duPuschCfg.p0NominalWithGrant;

         break;
      }
      default:
      {
         DU_LOG("\nDU APP : Invalid PUSCH configuration type ");
         return RFAILED;
      }
   }

   return ROK;
}/* BuildPuschCfgCommon */

/*******************************************************************
 *
 * @brief  Builds PUCCH configuration common
 *
 * @details
 *
 *    Function : BuildPucchCfgCommon
 *
 *    Functionality:
 *      Builds PUCCH configuration common
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildPucchCfgCommon( struct BWP_UplinkCommon__pucch_ConfigCommon *pucchCfg)
{
   PucchCfgCommon    duPucchCfg;
   PUCCH_ConfigCommon_t *setup;

   duPucchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.pucchCfg;
 
   pucchCfg->present = duPucchCfg.present;
   switch(pucchCfg->present)
   {
      case BWP_UplinkCommon__pucch_ConfigCommon_PR_NOTHING:
      {
         //TODO
         break;
      }
      case BWP_UplinkCommon__pucch_ConfigCommon_PR_release:
      {
         //TODO
         break;
      }
      case BWP_UplinkCommon__pucch_ConfigCommon_PR_setup:
      {
         DU_ALLOC(pucchCfg->choice.setup, sizeof(PUCCH_ConfigCommon_t));
         if(!pucchCfg->choice.setup)
         {
            DU_LOG("\nDU APP : PUCCH Config memory alloc failed");
            return RFAILED;
         }
         setup = pucchCfg->choice.setup;
         
         /* Resource Common */
         DU_ALLOC(setup->pucch_ResourceCommon, sizeof(long));
         if(!setup->pucch_ResourceCommon)
         {
            DU_LOG("\nDU APP : PUCCH Config memory alloc failed");
            return RFAILED;
         }
         *setup->pucch_ResourceCommon = duPucchCfg.rsrcComm;

         /* Group hopping */
         setup->pucch_GroupHopping = duPucchCfg.grpHop;

         /* P0 nominal */
         DU_ALLOC(setup->p0_nominal, sizeof(long));
         if(!setup->p0_nominal)
         {
           DU_LOG("\nDU APP : PUCCH Config memory alloc failed");
           return RFAILED;
         }
         *setup->p0_nominal = duPucchCfg.p0Nominal;

         break;
      }
      default:
      {
         DU_LOG("\nDU APP : Invalid PUCCH Config type");
         return RFAILED;
      }
   }
   return ROK;
}/* BuildPucchCfgCommon */

/*******************************************************************
 *
 * @brief Builds UL BWP Common 
 *
 * @details
 *
 *    Function : BuildBwpUlCommon
 *
 *    Functionality:
 *      Builds UL BWP Common 
 *
 * @params[in] BWP_UplinkCommon_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildBwpUlCommon(BWP_UplinkCommon_t *bwp)
{
   UlCfgCommon duUlCfg;
 
   duUlCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg;
 
   bwp->genericParameters.locationAndBandwidth = duUlCfg.locAndBw;
   bwp->genericParameters.subcarrierSpacing = duUlCfg.ulScsCarrier.scs;
 
   /* RACH Config Common */
   DU_ALLOC(bwp->rach_ConfigCommon,\
      sizeof(struct BWP_UplinkCommon__rach_ConfigCommon));
   if(!bwp->rach_ConfigCommon)
   {
      DU_LOG("\nDU APP : UL BWP memory allocation failed");
      return RFAILED;
   }
   BuildRachCfgCommon(bwp->rach_ConfigCommon);

   /* PUSCH Config Common */
   DU_ALLOC(bwp->pusch_ConfigCommon,\
      sizeof(struct BWP_UplinkCommon__pusch_ConfigCommon));
   if(!bwp->pusch_ConfigCommon)
   {
      DU_LOG("\nDU APP : UL BWP memory allocation failed");
      return RFAILED;
   } 
   BuildPuschCfgCommon(bwp->pusch_ConfigCommon);

   /* PUCCH Config Common */
   DU_ALLOC(bwp->pucch_ConfigCommon,\
      sizeof(struct BWP_UplinkCommon__pucch_ConfigCommon));
   if(!bwp->pucch_ConfigCommon)
   {
      DU_LOG("\nDU APP : UL BWP memory allocation failed");
      return RFAILED;
   } 
   BuildPucchCfgCommon(bwp->pucch_ConfigCommon);

   return ROK;
}/* BuildBwpUlCommon */

/*******************************************************************
 *
 * @brief Builds UL config common for SIB 
 *
 * @details
 *
 *    Function : BuildUlCfgCommSib
 *
 *    Functionality:
 *      Builds UL config common for SIB
 *
 * @params[in] UplinkConfigCommonSIB_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildUlCfgCommSib(UplinkConfigCommonSIB_t *ulCfg)
{
   /* UL frequency info */
   BuildFreqInfoUl(&ulCfg->frequencyInfoUL);

   /* BWP Uplink Common */
   BuildBwpUlCommon(&ulCfg->initialUplinkBWP);

   /* Time Alignment timer */
   ulCfg->timeAlignmentTimerCommon = \
     duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.timeAlignTimerComm;

   return ROK;
}/* BuildUlCfgCommSib */

/*******************************************************************
 *
 * @brief Builds TDD UL DL configuration common 
 *
 * @details
 *
 *    Function : BuildTddUlDlCfgComm
 *
 *    Functionality:
 *      Builds TDD UL DL configuration common
 *
 * @params[in] TDD_UL_DL_ConfigCommon_t pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildTddUlDlCfgComm(TDD_UL_DL_ConfigCommon_t *tddCfg)
{
   TddUlDlCfgCommon duTddCfg;

   duTddCfg = duCfgParam.sib1Params.srvCellCfgCommSib.tddCfg;

   /* Reference Subcarrier spacing */
   tddCfg->referenceSubcarrierSpacing = duTddCfg.refScs;

   /* Pattern 1*/
   tddCfg->pattern1.dl_UL_TransmissionPeriodicity = duTddCfg.txPrd;
   tddCfg->pattern1.nrofDownlinkSlots = duTddCfg.numDlSlots;
   tddCfg->pattern1.nrofDownlinkSymbols = duTddCfg.numDlSymbols;
   tddCfg->pattern1.nrofUplinkSlots = duTddCfg.numUlSlots;
   tddCfg->pattern1.nrofUplinkSymbols = duTddCfg.numUlSymbols;
   
   return ROK;

}/* BuildTddUlDlCfgComm */
 
/*******************************************************************
 * 
 * @brief Builds Serving cell config common in SIB1 message 
 * 
 * @details
 * 
 *     Function : BuildServCellCfgCommonSib
 * 
 *    Functionality: 
 *       Building Serving cell config common in SIB1 message
 *
 * @params[in] ServingCellConfigCommonSIB_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildServCellCfgCommonSib(ServingCellConfigCommonSIB_t *srvCellCfg)
{
   SrvCellCfgCommSib    duSrvCellCfg;
   BIT_STRING_t         *ssbPosInBurst;

   duSrvCellCfg = duCfgParam.sib1Params.srvCellCfgCommSib;

   /* SSB Position in Burst */
   ssbPosInBurst = &srvCellCfg->ssb_PositionsInBurst.inOneGroup;
   ssbPosInBurst->size = 1;
   DU_ALLOC(ssbPosInBurst->buf, ssbPosInBurst->size * sizeof(uint8_t));
   if(!ssbPosInBurst->buf)
   {
      DU_LOG("\nDU APP : Serving cell config common  memory allocation failure");
      return RFAILED;
   } 
   ssbPosInBurst->buf[0] = duSrvCellCfg.ssbPosInBurst;  
   ssbPosInBurst->bits_unused = 0;

   srvCellCfg->ssb_PeriodicityServingCell = duSrvCellCfg.ssbPrdServingCell;
   srvCellCfg->ss_PBCH_BlockPower = duSrvCellCfg.ssPbchBlockPwr;

   /* Downlink config common */
   BuildDlCfgCommSib(&srvCellCfg->downlinkConfigCommon);

   /* Uplink Config Comm */
   DU_ALLOC(srvCellCfg->uplinkConfigCommon, sizeof(UplinkConfigCommonSIB_t));
   if(!srvCellCfg->uplinkConfigCommon)
   {
      DU_LOG("\nDU APP : Serving cell config common  memory allocation failure");
      return RFAILED;
   }
   BuildUlCfgCommSib(srvCellCfg->uplinkConfigCommon);

   /* TDD UL DL Config Comm */
   DU_ALLOC(srvCellCfg->tdd_UL_DL_ConfigurationCommon, sizeof(TDD_UL_DL_ConfigCommon_t));
   if(!srvCellCfg->tdd_UL_DL_ConfigurationCommon)
   {
      DU_LOG("\nDU APP : Serving cell config common  memory allocation failure");
      return RFAILED;
   }
   BuildTddUlDlCfgComm(srvCellCfg->tdd_UL_DL_ConfigurationCommon);

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds SIB message in Served Cell Info
 *
 * @details
 *
 *    Function : BuildSib1Msg
 *
 *    Functionality: Building SIB message in Served Cell Info
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int BuildSib1Msg()
{
	SIB1_t                   *sib1Msg;
	CellAccessRelatedInfo_t  *cellAccessInfo;
	U8                       elementCnt;
	U8                       idx, idx1, idx2;
	asn_enc_rval_t           encRetVal;
	TrackingAreaCode_t       *tac;
	RAN_AreaCode_t           *ranac;
	CellIdentity_t           *cellIdentity;
   SIB_Mapping_t            *sibMapInfo;
   ServingCellConfigCommonSIB_t     *srvCellCfg;
   DownlinkConfigCommonSIB_t        *dlCfg;
   UplinkConfigCommonSIB_t          *ulCfg;
   FrequencyInfoDL_SIB_t            *frequencyInfoDL;
   FrequencyInfoUL_SIB_t            *frequencyInfoUL;
   BWP_DownlinkCommon_t             *bwpDl;
   BWP_UplinkCommon_t               *bwpUl;
   PDCCH_ConfigCommon_t             *pdcchSetup;
   SearchSpace_t                    *pdcchSearchSpace;
   PDSCH_ConfigCommon_t             *pdschSetup;
   RACH_ConfigCommon_t              *rachSetup;
   PUSCH_ConfigCommon_t             *puschSetup;
   PUCCH_ConfigCommon_t             *pucchSetup;
   struct PLMN_IdentityInfo__plmn_IdentityList     *plmnIdInfo;
   struct SI_SchedulingInfo__schedulingInfoList    *si_SchedulingInfoList;
   struct BWP_DownlinkCommon__pdcch_ConfigCommon   *pdcchCfg;
   struct BWP_DownlinkCommon__pdsch_ConfigCommon   *pdschCfg;
   struct BWP_UplinkCommon__rach_ConfigCommon      *rachCfg;
   struct BWP_UplinkCommon__pusch_ConfigCommon     *puschCfg;
   struct BWP_UplinkCommon__pucch_ConfigCommon     *pucchCfg;
   struct FrequencyInfoDL_SIB__scs_SpecificCarrierList *scsCarrierList;
   struct FrequencyInfoUL_SIB__scs_SpecificCarrierList *scsCarrierListUl;
   struct PDCCH_ConfigCommon__commonSearchSpaceList *pdcchSearchSpclist;
  

	DU_ALLOC(sib1Msg, sizeof(SIB1_t));
	if(!sib1Msg)
	{   
		DU_LOG("DU APP: SIB1 msg memory allocation failure");
		return RFAILED;
	}   
   
	elementCnt = ODU_VALUE_ONE;

   /* PLMN list */
	cellAccessInfo = &sib1Msg->cellAccessRelatedInfo;
	cellAccessInfo->plmn_IdentityList.list.count =
		elementCnt;
	cellAccessInfo->plmn_IdentityList.list.size
		= elementCnt * sizeof(PLMN_IdentityInfo_t);

   BuildPlmnList(cellAccessInfo);

   /* Connection Establish Failure Control */
   DU_ALLOC(sib1Msg->connEstFailureControl, sizeof(ConnEstFailureControl_t));
   if(!sib1Msg->connEstFailureControl)
   {
      DU_LOG("DU APP: sib1Msg->connEstFailureControl memory allocation failure");
      return RFAILED;
   }
   sib1Msg->connEstFailureControl->connEstFailCount =\
      duCfgParam.sib1Params.connEstFailCnt;
   sib1Msg->connEstFailureControl->connEstFailOffsetValidity =\
      duCfgParam.sib1Params.connEstFailOffValidity;

   /* SI Scheduling Info */
   DU_ALLOC(sib1Msg->si_SchedulingInfo, sizeof(SI_SchedulingInfo_t));
   if(!sib1Msg->si_SchedulingInfo)
   {
      DU_LOG("DU APP: sib1Msg->si_SchedulingInfo memory allocation failure");
      return RFAILED;
   } 
   elementCnt = ODU_VALUE_ONE;
   sib1Msg->si_SchedulingInfo->schedulingInfoList.list.count = elementCnt;
   sib1Msg->si_SchedulingInfo->schedulingInfoList.list.size = elementCnt * sizeof(struct SchedulingInfo);
   BuildSiSchedInfoList(&sib1Msg->si_SchedulingInfo->schedulingInfoList);
   sib1Msg->si_SchedulingInfo->si_WindowLength = duCfgParam.sib1Params.siSchedInfo.winLen;

   /* Serving Cell Config Common */
   DU_ALLOC(sib1Msg->servingCellConfigCommon, sizeof(ServingCellConfigCommonSIB_t));
   if(!sib1Msg->servingCellConfigCommon)
   {
      DU_LOG("DU APP: sib1Msg->servingCellConfigCommon memory allocation failure");
      return RFAILED;
   }
   BuildServCellCfgCommonSib(sib1Msg->servingCellConfigCommon);


   xer_fprint(stdout, &asn_DEF_SIB1, sib1Msg);

	/* Encode the F1SetupRequest type as APER */
	cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
	encBufSize = 0;
	encRetVal = aper_encode(&asn_DEF_SIB1, 0, sib1Msg, PrepFinalEncBuf,\
			encBuf);
	printf("\nencbufSize: %d\n", encBufSize);
	if(encRetVal.encoded == -1)
	{
		DU_LOG("\nDU APP : Could not encode SIB1 structure (at %s)\n",\
				encRetVal.failed_type ?
				encRetVal.failed_type->name :
				"unknown");
		return RFAILED;
	}
	for(int i=0; i< encBufSize; i++)
	{
		printf("%x\t",encBuf[i]);
	}
	printf("\n");

	/* Free allocated memeory */

   /* Free serving cell config common */
   srvCellCfg = sib1Msg->servingCellConfigCommon;

   /* Free downling config common */
   dlCfg = &srvCellCfg->downlinkConfigCommon;

   /* Free DL frequency info */
   frequencyInfoDL = &dlCfg->frequencyInfoDL;

   for(idx = 0; idx < frequencyInfoDL->frequencyBandList.list.count; idx++)
   {
      DU_FREE(frequencyInfoDL->frequencyBandList.list.array[idx]->\
         freqBandIndicatorNR, sizeof(FreqBandIndicatorNR_t));
      DU_FREE(frequencyInfoDL->frequencyBandList.list.array[idx],\
            sizeof(NR_MultiBandInfo_t));
   }
   DU_FREE(frequencyInfoDL->frequencyBandList.list.array, \
      frequencyInfoDL->frequencyBandList.list.size);

   scsCarrierList = &frequencyInfoDL->scs_SpecificCarrierList;
   for(idx = 0; idx < scsCarrierList->list.count; idx++)
   {
      DU_FREE(scsCarrierList->list.array[idx], sizeof(SCS_SpecificCarrier_t));
   }
   DU_FREE(scsCarrierList->list.array, scsCarrierList->list.size);

   /* Free BWP DL common */
   bwpDl = &dlCfg->initialDownlinkBWP;
  
   /* Free PDCCH config common */
   pdcchCfg = bwpDl->pdcch_ConfigCommon;
   pdcchSetup = pdcchCfg->choice.setup;

   DU_FREE(pdcchSetup->controlResourceSetZero, sizeof(ControlResourceSetZero_t));
   DU_FREE(pdcchSetup->searchSpaceZero, sizeof(SearchSpaceZero_t));

   pdcchSearchSpclist = pdcchSetup->commonSearchSpaceList;
   for(idx = 0; idx < pdcchSearchSpclist->list.count; idx++)
   {
      pdcchSearchSpace = pdcchSearchSpclist->list.array[idx];

      DU_FREE(pdcchSearchSpace->controlResourceSetId, \
         sizeof(ControlResourceSetId_t));
      DU_FREE(pdcchSearchSpace->monitoringSlotPeriodicityAndOffset,\
         sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
      DU_FREE(pdcchSearchSpace->monitoringSymbolsWithinSlot->buf, \
         pdcchSearchSpace->monitoringSymbolsWithinSlot->size * sizeof(uint8_t));
      DU_FREE(pdcchSearchSpace->monitoringSymbolsWithinSlot,\
         sizeof(BIT_STRING_t));
      DU_FREE(pdcchSearchSpace->nrofCandidates,\
         sizeof(struct SearchSpace__nrofCandidates));
      DU_FREE(pdcchSearchSpace->searchSpaceType->choice.common->\
         dci_Format0_0_AndFormat1_0, sizeof(struct \
         SearchSpace__searchSpaceType__common__dci_Format0_0_AndFormat1_0));
      DU_FREE(pdcchSearchSpace->searchSpaceType->choice.common,\
         sizeof(struct SearchSpace__searchSpaceType__common));
      DU_FREE(pdcchSearchSpace->searchSpaceType,\
         sizeof( struct SearchSpace__searchSpaceType));

      DU_FREE(pdcchSearchSpclist->list.array[idx], sizeof(SearchSpace_t));
   }
   DU_FREE(pdcchSearchSpclist->list.array, pdcchSearchSpclist->list.size);

   DU_FREE(pdcchSetup->commonSearchSpaceList,\
      sizeof(struct PDCCH_ConfigCommon__commonSearchSpaceList));
   DU_FREE(pdcchSetup->searchSpaceSIB1, sizeof(SearchSpaceId_t));
   DU_FREE(pdcchSetup->pagingSearchSpace, sizeof(SearchSpaceId_t));
   DU_FREE(pdcchSetup->ra_SearchSpace, sizeof(SearchSpaceId_t));

   DU_FREE(pdcchCfg->choice.setup, sizeof(PDCCH_ConfigCommon_t));
   DU_FREE(bwpDl->pdcch_ConfigCommon, \
      sizeof(struct BWP_DownlinkCommon__pdcch_ConfigCommon));

   /* Free PDSCH Config Common */
   pdschCfg = bwpDl->pdsch_ConfigCommon;
   pdschSetup = pdschCfg->choice.setup;

   for(idx=0; idx<pdschSetup->pdsch_TimeDomainAllocationList->list.count; idx++)
   {
      DU_FREE(pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx]->k0, \
         sizeof(long));
      DU_FREE(pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx],\
         sizeof(PDSCH_TimeDomainResourceAllocation_t));
   }
   DU_FREE(pdschSetup->pdsch_TimeDomainAllocationList->list.array,\
      pdschSetup->pdsch_TimeDomainAllocationList->list.size);
   DU_FREE(pdschSetup->pdsch_TimeDomainAllocationList, \
      sizeof(PDSCH_TimeDomainResourceAllocationList_t));
   DU_FREE(pdschCfg->choice.setup, sizeof(PDSCH_ConfigCommon_t));
   DU_FREE(bwpDl->pdsch_ConfigCommon, \
      sizeof(struct BWP_DownlinkCommon__pdsch_ConfigCommon));

   /* Free Uplink Config Common */
   ulCfg = srvCellCfg->uplinkConfigCommon;

   /* Free UL Frequency Info */
   frequencyInfoUL = &ulCfg->frequencyInfoUL;
   scsCarrierListUl = &frequencyInfoUL->scs_SpecificCarrierList;
   for(idx = 0; idx < scsCarrierListUl->list.count; idx++)
   {
      DU_FREE(scsCarrierListUl->list.array[idx], sizeof(SCS_SpecificCarrier_t));
   }
   DU_FREE(scsCarrierListUl->list.array, scsCarrierList->list.size);
   DU_FREE(frequencyInfoUL->p_Max, sizeof(P_Max_t));

   /* Free BWP Uplink common */
   bwpUl = &ulCfg->initialUplinkBWP;

   /* Free RACH config Common */
   rachCfg = bwpUl->rach_ConfigCommon;
   rachSetup = rachCfg->choice.setup;
   DU_FREE(rachSetup->totalNumberOfRA_Preambles, sizeof(long));
   DU_FREE(rachSetup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB,\
      sizeof(struct RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB));
   DU_FREE(rachSetup->rsrp_ThresholdSSB, sizeof(RSRP_Range_t));
   DU_FREE(rachSetup->msg1_SubcarrierSpacing, sizeof(SubcarrierSpacing_t));
   DU_FREE(rachCfg->choice.setup, sizeof(RACH_ConfigCommon_t));
   DU_FREE(bwpUl->rach_ConfigCommon, \
      sizeof(struct BWP_UplinkCommon__rach_ConfigCommon));

   /* Free PUSCH Config Common */
   puschCfg = bwpUl->pusch_ConfigCommon;
   puschSetup = puschCfg->choice.setup;
   for(idx=0; idx<puschSetup->pusch_TimeDomainAllocationList->list.count; idx++)
   {
      DU_FREE(puschSetup->pusch_TimeDomainAllocationList->list.array[idx]->k2,\
         sizeof(long));
      DU_FREE(puschSetup->pusch_TimeDomainAllocationList->list.array[idx],\
         sizeof(PUSCH_TimeDomainResourceAllocation_t));
   }
   DU_FREE(puschSetup->pusch_TimeDomainAllocationList->list.array, \
      puschSetup->pusch_TimeDomainAllocationList->list.size);
   DU_FREE(puschSetup->pusch_TimeDomainAllocationList,\
      sizeof(PUSCH_TimeDomainResourceAllocationList_t));
   DU_FREE(puschSetup->msg3_DeltaPreamble, sizeof(long));
   DU_FREE(puschSetup->p0_NominalWithGrant, sizeof(long));
   DU_FREE(puschCfg->choice.setup, sizeof(PUSCH_ConfigCommon_t));
   DU_FREE(bwpUl->pusch_ConfigCommon,\
      sizeof(struct BWP_UplinkCommon__pusch_ConfigCommon));


   /* Free PUCCH Config Common */
   pucchCfg = bwpUl->pucch_ConfigCommon;
   pucchSetup = pucchCfg->choice.setup;
   DU_FREE(pucchSetup->pucch_ResourceCommon, sizeof(long));
   DU_FREE(pucchSetup->p0_nominal, sizeof(long));
   DU_FREE(pucchCfg->choice.setup, sizeof(PUCCH_ConfigCommon_t));
   DU_FREE(bwpUl->pucch_ConfigCommon,\
      sizeof(struct BWP_UplinkCommon__pucch_ConfigCommon));

   DU_FREE(srvCellCfg->uplinkConfigCommon, sizeof(UplinkConfigCommonSIB_t));

   DU_FREE(srvCellCfg->ssb_PositionsInBurst.inOneGroup.buf, \
      srvCellCfg->ssb_PositionsInBurst.inOneGroup.size * sizeof(uint8_t));
   DU_FREE(sib1Msg->servingCellConfigCommon, sizeof(ServingCellConfigCommonSIB_t));

	/* Free PLMN List */

	for(idx=0; idx<cellAccessInfo->plmn_IdentityList.list.count; idx++)
	{
		plmnIdInfo = &cellAccessInfo->plmn_IdentityList.list.array[idx]->\
						 plmn_IdentityList;
		for(idx1=0; idx1<plmnIdInfo->list.count; idx1++)
		{
			for(idx2=0; idx2<plmnIdInfo->list.array[idx1]->mnc.list.count; idx2++)
			{
				DU_FREE(plmnIdInfo->list.array[idx1]->mcc->list.array[idx2],
						sizeof(MCC_MNC_Digit_t));
				DU_FREE(plmnIdInfo->list.array[idx1]->mnc.list.array[idx2],
						sizeof(MCC_MNC_Digit_t));
			}
			DU_FREE(plmnIdInfo->list.array[idx1]->mcc->list.array,
					plmnIdInfo->list.array[idx1]->mcc->list.size);
			DU_FREE(plmnIdInfo->list.array[idx1]->mnc.list.array,
					plmnIdInfo->list.array[idx1]->mnc.list.size);
			DU_FREE(plmnIdInfo->list.array[idx1]->mcc,
					sizeof(MCC_t));
			DU_FREE(plmnIdInfo->list.array[idx1],
					sizeof(PLMN_IdentitY_t));
		}
		DU_FREE(cellAccessInfo->plmn_IdentityList.list.array[idx],
				sizeof(PLMN_IdentityInfo_t));
		cellIdentity =\
						  &cellAccessInfo->plmn_IdentityList.list.array[idx]->cellIdentity;
		DU_FREE(cellIdentity->buf, cellIdentity->size);

		ranac = cellAccessInfo->plmn_IdentityList.list.array[idx]->ranac;
		DU_FREE(ranac, sizeof(RAN_AreaCode_t));

		tac = cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode;
		DU_FREE(tac->buf, tac->size);
		DU_FREE(tac, sizeof(TrackingAreaCode_t));
	}
	DU_FREE(cellAccessInfo->plmn_IdentityList.list.array,
			cellAccessInfo->plmn_IdentityList.list.size);

   si_SchedulingInfoList = &sib1Msg->si_SchedulingInfo->schedulingInfoList;
   for(idx = 0; idx < si_SchedulingInfoList->list.count; idx++)
   {
      sibMapInfo = &si_SchedulingInfoList->list.array[idx]->sib_MappingInfo;
      for(idx1 = 0; idx1 < sibMapInfo->list.count; idx1++)
      {
         DU_FREE(sibMapInfo->list.array[idx1]->valueTag, sizeof(long));
         DU_FREE(sibMapInfo->list.array[idx1], sizeof(SIB_TypeInfo_t));
      }
      DU_FREE(sibMapInfo->list.array, sibMapInfo->list.size);
      DU_FREE(si_SchedulingInfoList->list.array[idx], sizeof(struct SchedulingInfo));
   }
   DU_FREE(si_SchedulingInfoList->list.array, si_SchedulingInfoList->list.size);
   DU_FREE(sib1Msg->si_SchedulingInfo, sizeof(SI_SchedulingInfo_t));
   DU_FREE(sib1Msg->connEstFailureControl, sizeof(ConnEstFailureControl_t));

	DU_FREE(sib1Msg, sizeof(SIB1_t));
   return ROK;

}

/**********************************************************************
  End of file
 **********************************************************************/
