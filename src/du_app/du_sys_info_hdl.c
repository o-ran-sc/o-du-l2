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
#include "common_def.h"
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "legtp.h"
#include "du_app_mac_inf.h"
#include "du_cfg.h"
#include "du_utils.h"
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
#include "odu_common_codec.h"
#include "du_sys_info_hdl.h"

void FreeSib1Msg(SIB1_t *sib1Msg);
uint8_t FreqInfoUlret = RFAILED;
uint8_t RachCfgCommonret = RFAILED;
uint8_t PuschCfgCommonret = RFAILED;
uint8_t UlCfgCommSibret = RFAILED;
uint8_t FreqInfoDlret = RFAILED;
uint8_t DlCfgCommSibret = RFAILED;
uint8_t CommonSerachSpaceListret = RFAILED;

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
uint8_t BuildMib(MIB_t *mib)
{
   mib->systemFrameNumber.size = sizeof(uint8_t);
   DU_ALLOC(mib->systemFrameNumber.buf, mib->systemFrameNumber.size);
   if(!(mib->systemFrameNumber.buf))
   {
      DU_LOG("\nERROR  -->  DU APP: MIB msg memory allocation failure");
      return RFAILED;
   }

   *(mib->systemFrameNumber.buf) =  duCfgParam.mibParams.sysFrmNum;
   mib->systemFrameNumber.bits_unused = ODU_VALUE_TWO;

   mib->subCarrierSpacingCommon = duCfgParam.mibParams.subCarrierSpacingCommon;
   mib->ssb_SubcarrierOffset = duCfgParam.mibParams.ssb_SubcarrierOffset;
   mib->dmrs_TypeA_Position = duCfgParam.mibParams.dmrs_TypeA_Position;
   mib->pdcch_ConfigSIB1.controlResourceSetZero = duCfgParam.mibParams.controlResourceSetZero;
   mib->pdcch_ConfigSIB1.searchSpaceZero = duCfgParam.mibParams.searchSpaceZero;
   mib->cellBarred = duCfgParam.mibParams.cellBarred;
   mib->intraFreqReselection = duCfgParam.mibParams.intraFreqReselection;
   mib->spare.size = sizeof(uint8_t);
   DU_ALLOC(mib->spare.buf, mib->spare.size);
   if(!mib->spare.buf)
   {
      DU_LOG("\nERROR  -->  DU APP: MIB msg memory allocation failure");
      return RFAILED;
   }
   *(mib->spare.buf) = SPARE;
   mib->spare.bits_unused = ODU_VALUE_SEVEN;
   return ROK;
}
/*******************************************************************
 *
 * @brief Free MIB PDU for broadcast
 *
 * @details
 *
 *    Function : FreeMibPdu
 *
 *    Functionality: Deallocating MIB PDU for system broadcast
 *
 * @param[in]: BCCH_BCH_Message_t *bcchMsg
 *
 * @return void         
 *
 * ****************************************************************/
void FreeMibPdu(BCCH_BCH_Message_t *bcchMsg)
{
   if(!bcchMsg)
   {
      if(!bcchMsg->message.choice.mib)
      {
	 if(!(bcchMsg->message.choice.mib->systemFrameNumber.buf))
	 {
	    if(!bcchMsg->message.choice.mib->spare.buf)
	    {
	       DU_FREE(bcchMsg->message.choice.mib->spare.buf,\
		     bcchMsg->message.choice.mib->spare.size);
	    }
	    DU_FREE(bcchMsg->message.choice.mib->systemFrameNumber.buf,
		  bcchMsg->message.choice.mib->systemFrameNumber.size);
	 }
	 DU_FREE(bcchMsg->message.choice.mib, sizeof(MIB_t));
      }
      DU_FREE(bcchMsg, sizeof(BCCH_BCH_Message_t));
   }
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
uint8_t BuildMibPdu()
{
   uint8_t ret = RFAILED;
   BCCH_BCH_Message_t *bcchMsg;
   asn_enc_rval_t encRetVal;        /* Encoder return value */
   while(true)
   {
      DU_ALLOC(bcchMsg, sizeof(BCCH_BCH_Message_t));
      if(!bcchMsg)
      {
	 DU_LOG("\nERROR  -->  Memory allocation failure in BuildMibPdu");
	 break;
      }

      bcchMsg->message.present = BCCH_BCH_MessageType_PR_mib;
      DU_ALLOC(bcchMsg->message.choice.mib, sizeof(MIB_t));
      if(!bcchMsg->message.choice.mib)
      {
	 DU_LOG("\nERROR  -->  Memory allocation failure in BuildMibPdu");
	 break;
      }
      if(BuildMib(bcchMsg->message.choice.mib) != ROK)
      {
	 break;
      }
      xer_fprint(stdout, &asn_DEF_BCCH_BCH_Message, bcchMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_BCCH_BCH_Message, 0,
	    bcchMsg, PrepFinalEncBuf, encBuf);
      printf("\nencbufSize:%d\n", encBufSize);
      if(encRetVal.encoded == -1) 
      {   
	 DU_LOG("\nERROR  -->  DU APP: Could not encode BCCH BCH Message Type structure(at %s)\n", 
	       encRetVal.failed_type?\
	       encRetVal.failed_type->name
	       :"unknown");
	 break;
      }    

      /* Print encoded buffer */
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x\t",encBuf[i]);
      } 
      printf("\n");

      ret = ROK;
      break;
   }
   /* Free allocated memory */
   FreeMibPdu(bcchMsg);

   return ret;

}
/*******************************************************************
 *
 * @brief free MIB message in Served Cell Info
 *
 * @details
 *
 *    Function : FreeMibMsg
 *
 *    Functionality: deallocating MIB message in Served Cell Info
 *
 * @param[in]   MIB_t *mibMsg
 *
 * @return void
 *        
 *
 * ****************************************************************/
void FreeMibMsg( MIB_t *mibMsg)
{
   if(!mibMsg)
   {
      if(!(mibMsg->systemFrameNumber.buf))
      {   
	 if(!mibMsg->spare.buf)
	 {
	    DU_FREE(mibMsg->spare.buf, mibMsg->spare.size);
	 }
	 DU_FREE(mibMsg->systemFrameNumber.buf,
	       mibMsg->systemFrameNumber.size);
      }
      DU_FREE(mibMsg, sizeof(MIB_t));
   }
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
uint8_t BuildMibMsg()
{
   MIB_t          *mibMsg;
   asn_enc_rval_t encRetVal;        /* Encoder return value */
   uint8_t ret = RFAILED;
   while(true)
   {
      DU_ALLOC(mibMsg, sizeof(MIB_t));
      if(!mibMsg)
      {
	 DU_LOG("\nERROR  -->  DU APP: MIB msg memory allocation failure");
	 return RFAILED;
      }
      if(BuildMib(mibMsg) != ROK)
      {
	 break;
      }
      xer_fprint(stdout, &asn_DEF_MIB, mibMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_MIB, 0,
	    mibMsg, PrepFinalEncBuf, encBuf);
      printf("\nencbufSize:%d\n", encBufSize);
      if(encRetVal.encoded	== -1) 
      {   
	 DU_LOG("\nERROR  -->  DU APP: Could not encode MIB structure(at %s)\n", 
	       encRetVal.failed_type?\
	       encRetVal.failed_type->name
	       :"unknown");
	 break;
      }  

      /* Print encoded buffer */
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x\t",encBuf[i]);
      } 
      printf("\n");

      /* Free allocated memory */
      ret = ROK;
      break;
   }

   FreeMibMsg(mibMsg);

   return ret;

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
uint8_t BuildCellIdentity(CellIdentity_t  *cellIdentity)
{
   cellIdentity->size = ODU_VALUE_FIVE*sizeof(uint8_t);
   cellIdentity->bits_unused = ODU_VALUE_FOUR;

   DU_ALLOC(cellIdentity->buf, cellIdentity->size);
   if(!cellIdentity->buf)
   {   
      DU_LOG("\nERROR  -->  DU APP: CellIdentity memory allocation failure");
      return RFAILED;
   }
   memset(cellIdentity->buf, 0,  cellIdentity->size);
   cellIdentity->buf[4] = duCfgParam.sib1Params.cellIdentity;

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
uint8_t BuildRanac(RAN_AreaCode_t **ranAreaCode)
{
   RAN_AreaCode_t *ranac;
   DU_ALLOC(ranac, sizeof(RAN_AreaCode_t));
   if(!ranac)
   {   
      DU_LOG("\nERROR  -->  DU APP: RANAC memory allocation failure");
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
uint8_t BuildTac(TrackingAreaCode_t **trackAreaCode)
{
   TrackingAreaCode_t *tac;


   DU_ALLOC(tac, sizeof(TrackingAreaCode_t));
   if(!tac)
   {   
      DU_LOG("\nERROR  -->  DU APP: TAC memory allocation failure");
      return RFAILED;
   }   

   tac->size = ODU_VALUE_THREE*sizeof(uint8_t);
   DU_ALLOC(tac->buf, tac->size);
   if(!tac->buf)
   {   
      DU_LOG("\nERROR  -->  DU APP: TAC memory allocation failure");
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
uint8_t BuildPlmnList(CellAccessRelatedInfo_t *cellAccessInfo)
{
   uint8_t            idx;
   uint8_t            idx1;
   uint8_t            idx2;
   uint8_t            elementCnt;
   TrackingAreaCode_t **tac;
   RAN_AreaCode_t     **ranac;
   CellIdentity_t     *cellIdentity;
   uint8_t            ret;	
   struct PLMN_IdentityInfo__plmn_IdentityList
      *plmnIdInfo;

   DU_ALLOC(cellAccessInfo->plmn_IdentityList.list.array,
	 cellAccessInfo->plmn_IdentityList.list.size);
   if(!cellAccessInfo->plmn_IdentityList.list.array)
   {   
      DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
      return RFAILED;
   }   
   elementCnt = cellAccessInfo->plmn_IdentityList.list.count; 
   for(idx=0; idx<elementCnt; idx++)
   {   
      DU_ALLOC(cellAccessInfo->plmn_IdentityList.list.array[idx],
	    sizeof(PLMN_IdentityInfo_t));
      if(!cellAccessInfo->plmn_IdentityList.list.array[idx])
      {
	 DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
	 return RFAILED;
      }
   }

   idx = 0;
   /* PLMN list */
   elementCnt = ODU_VALUE_ONE;

   plmnIdInfo = &cellAccessInfo->plmn_IdentityList.list.array[idx]->\
		plmn_IdentityList;
   plmnIdInfo->list.count = elementCnt;
   plmnIdInfo->list.size  = elementCnt * sizeof(PLMN_IdentitY_t *);
   DU_ALLOC(plmnIdInfo->list.array, plmnIdInfo->list.size);
   if(!plmnIdInfo->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
      return RFAILED;
   }

   for(idx1=0; idx1<elementCnt; idx1++)
   {
      DU_ALLOC(plmnIdInfo->list.array[idx1],
	    sizeof(PLMN_IdentitY_t));
      if(!(plmnIdInfo->list.array[idx1]))
      {
	 DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
	 return RFAILED;
      }
   }
   idx1 = 0;
   DU_ALLOC(plmnIdInfo->list.array[idx1]->mcc,
	 sizeof(MCC_t));
   if(!plmnIdInfo->list.array[idx1]->mcc)
   {
      DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
      return RFAILED;
   }

   elementCnt = ODU_VALUE_THREE;
   plmnIdInfo->list.array[idx1]->mcc->list.count = elementCnt;
   plmnIdInfo->list.array[idx1]->mcc->list.size =\
   elementCnt * sizeof(MCC_MNC_Digit_t *);
   DU_ALLOC(plmnIdInfo->list.array[idx1]->mcc->list.array,
	 plmnIdInfo->list.array[idx1]->mcc->list.size)
      if(!(plmnIdInfo->list.array[idx1]->mcc->list.array))
      {
	 DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
	 return RFAILED;
      }
   for(idx2=0; idx2<elementCnt; idx2++)
   {
      DU_ALLOC(plmnIdInfo->list.array[idx1]->mcc->list.array[idx2],
	    sizeof(MCC_MNC_Digit_t));
      if(!plmnIdInfo->list.array[idx1]->mcc->list.array[idx2])
      {
	 DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
	 return RFAILED;
      }
      *(plmnIdInfo->list.array[idx1]->mcc->list.array[idx2])=\
      duCfgParam.sib1Params.plmn.mcc[idx2];
   }
   idx2 = 0;
   plmnIdInfo->list.array[idx1]->mnc.list.count = elementCnt;
   plmnIdInfo->list.array[idx1]->mnc.list.size =\
   elementCnt * sizeof(MCC_MNC_Digit_t *);
   DU_ALLOC(plmnIdInfo->list.array[idx1]->mnc.list.array,\
   plmnIdInfo->list.array[idx1]->mnc.list.size);
   if(!plmnIdInfo->list.array[idx1]->mnc.list.array)
   {
      DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
      return RFAILED;
   }
   for(idx2=0; idx2<elementCnt; idx2++)
   {
      DU_ALLOC(plmnIdInfo->list.array[idx1]->mnc.list.array[idx2],
	    sizeof(MCC_MNC_Digit_t));
      if(!plmnIdInfo->list.array[idx1]->mnc.list.array[idx2])
      {
	 DU_LOG("\nERROR  -->  DU APP: BuildPlmnList memory allocation failure");
	 return RFAILED;
      }
      *(plmnIdInfo->list.array[idx1]->mnc.list.array[idx2])=\
      duCfgParam.sib1Params.plmn.mnc[idx2];
   }

   /* Tracking Area Code */
   tac = &cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode;
   ret = BuildTac(tac);
   if(ret != ROK)
   {
      return RFAILED;
   }

   /* RANAC */
   ranac = &cellAccessInfo->plmn_IdentityList.list.array[idx]->ranac;
   ret=BuildRanac(ranac);
   if(ret != ROK)
   {
      return RFAILED;
   }

   /* CellIdentity */
   cellIdentity = &cellAccessInfo->plmn_IdentityList.list.array[idx]->cellIdentity;
   ret=BuildCellIdentity(cellIdentity);
   if(ret != ROK)
   {
      return RFAILED;
   }

   /* cellReservedForOperatorUse */
   cellAccessInfo->plmn_IdentityList.list.array[idx]->\
   cellReservedForOperatorUse = duCfgParam.sib1Params.cellResvdForOpUse;


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
uint8_t BuildSibMapInfoList(SIB_Mapping_t *sibMapInfo)
{
   uint8_t   itr;         /* List iterator */
   uint8_t   elementCnt;  /* Number of elements in list */
   SIB_TypeInfo_t *sibTypeInfo;  /* Scheduling info */

   /* SI scheduling info list */
   DU_ALLOC(sibMapInfo->list.array, sibMapInfo->list.size);
   if(!sibMapInfo->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP: BuildSibMapInfoList memory allocation failure");
      return RFAILED;
   }

   elementCnt = sibMapInfo->list.count;
   for(itr = 0; itr < elementCnt; itr++)
   {
      DU_ALLOC(sibMapInfo->list.array[itr], sizeof(SIB_TypeInfo_t));
      if(!sibMapInfo->list.array[itr])
      {
	 DU_LOG("\nERROR  -->  DU APP: BuildSibMapInfoList memory allocation failure");
	 return RFAILED;
      }
   }

   itr = 0;
   sibTypeInfo = sibMapInfo->list.array[itr];
   sibTypeInfo->type = duCfgParam.sib1Params.siSchedInfo.sibType;
   DU_ALLOC(sibTypeInfo->valueTag, sizeof(long));
   if(!sibTypeInfo->valueTag)
   {
      DU_LOG("\nERROR  -->  DU APP: BuildSibMapInfoList memory allocation failure");
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
uint8_t BuildSiSchedInfoList(struct SI_SchedulingInfo__schedulingInfoList *si_SchedulingInfoList)
{
   uint8_t   itr;         /* List iterator */
   uint8_t   elementCnt;  /* Number of elements in list */
   uint8_t   ret;
   SchedulingInfo_t *schedInfo;  /* Scheduling info */

   /* SI scheduling info list */
   DU_ALLOC(si_SchedulingInfoList->list.array, si_SchedulingInfoList->list.size);
   if(!si_SchedulingInfoList->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP: BuildSiSchedInfoList memory allocation failure");
      return RFAILED;
   }

   elementCnt = si_SchedulingInfoList->list.count;
   for(itr = 0; itr < elementCnt; itr++)
   {
      DU_ALLOC(si_SchedulingInfoList->list.array[itr], sizeof(struct SchedulingInfo));
      if(!si_SchedulingInfoList->list.array[itr])
      {
	 DU_LOG("\nERROR  -->  DU APP: BuildSiSchedInfoList memory allocation failure");
	 return RFAILED;
      }
   }

   itr = 0;
   schedInfo = si_SchedulingInfoList->list.array[itr];
   schedInfo->si_BroadcastStatus = duCfgParam.sib1Params.siSchedInfo.broadcastSta;
   schedInfo->si_Periodicity = duCfgParam.sib1Params.siSchedInfo.preiodicity;

   elementCnt = ODU_VALUE_ONE; 
   schedInfo->sib_MappingInfo.list.count = elementCnt;
   schedInfo->sib_MappingInfo.list.size = elementCnt * sizeof(SIB_TypeInfo_t *);

   ret = BuildSibMapInfoList(&schedInfo->sib_MappingInfo);
   if(ret != ROK)
   {
      return RFAILED;
   }

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
uint8_t BuildScsSpecificCarrierListDl( struct FrequencyInfoDL_SIB__scs_SpecificCarrierList *scsCarrierList)
{
   uint8_t idx;
   ScsSpecCarrier   duScsSpecCarrier;

   duScsSpecCarrier = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.dlScsCarrier;

   DU_ALLOC(scsCarrierList->list.array, scsCarrierList->list.size);
   if(!scsCarrierList->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : SCS Specific Carrier list memory allocation failed"); 
      return RFAILED;
   }

   for(idx = 0; idx < scsCarrierList->list.count; idx++)
   {
      DU_ALLOC(scsCarrierList->list.array[idx], sizeof(SCS_SpecificCarrier_t));
      if(!scsCarrierList->list.array[idx])
      {
	 DU_LOG("\nERROR  -->  DU APP : SCS Specific Carrier list memory allocation failed");
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
uint8_t BuildCommonSerachSpaceList( struct PDCCH_ConfigCommon__commonSearchSpaceList *searchSpclist)
{
   uint8_t idx;
   SearchSpace_t    *searchSpace;
   PdcchCfgCommon   duPdcchCfg;

   duPdcchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pdcchCfg;

   DU_ALLOC(searchSpclist->list.array, searchSpclist->list.size)
   if(!searchSpclist->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
      return RFAILED;
   }

   for(idx = 0; idx < searchSpclist->list.count; idx++)
   {
      DU_ALLOC(searchSpclist->list.array[idx], sizeof(SearchSpace_t));
      if(!searchSpclist->list.array[idx])
      {
	 DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
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
      DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
      return RFAILED;
   }
   *searchSpace->controlResourceSetId = duPdcchCfg.ctrlRsrcSetId;

   /* Monitoring Slot periodicity and offset */
   DU_ALLOC(searchSpace->monitoringSlotPeriodicityAndOffset,\
   sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
   if(!searchSpace->monitoringSlotPeriodicityAndOffset)
   {
      DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
      return RFAILED;
   }
   searchSpace->monitoringSlotPeriodicityAndOffset->present = \
   duPdcchCfg.monitorSlotPrdAndOffPresent;
   switch(searchSpace->monitoringSlotPeriodicityAndOffset->present)
   {
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1:
      {
         //searchSpace->monitoringSlotPeriodicityAndOffset->choice.s11 =  duPdcchCfg.monitorSlotPrdAndOff;
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
        //TODO
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
	 DU_LOG("\nERROR  -->  DU APP : Invalid value:Montoring slot periodicity and offset");
	 return RFAILED;
      }
   }

   /* Monitoring Symbols Within Slot */
   DU_ALLOC(searchSpace->monitoringSymbolsWithinSlot,\
   sizeof(BIT_STRING_t));
   if(!searchSpace->monitoringSymbolsWithinSlot)
   {
      DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
      return RFAILED;
   }
   searchSpace->monitoringSymbolsWithinSlot->size = 2*sizeof(uint8_t);
   DU_ALLOC(searchSpace->monitoringSymbolsWithinSlot->buf, \
   searchSpace->monitoringSymbolsWithinSlot->size );
   if(!searchSpace->monitoringSymbolsWithinSlot->buf)
   {  
      DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
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
      DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
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
      DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
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
         DU_ALLOC(searchSpace->searchSpaceType->choice.common, sizeof(struct SearchSpace__searchSpaceType__common));
	 if(!searchSpace->searchSpaceType->choice.common)
	 {
	    DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
	    return RFAILED;
	 }

	 DU_ALLOC(searchSpace->searchSpaceType->choice.common->\
	 dci_Format0_0_AndFormat1_0, sizeof(struct \
	 SearchSpace__searchSpaceType__common__dci_Format0_0_AndFormat1_0));
	 if(!searchSpace->searchSpaceType->choice.common->dci_Format0_0_AndFormat1_0)
	 {
	    DU_LOG("\nERROR  -->  DU APP : Common search space list memory alloc failed");
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
	 DU_LOG("\nERROR  -->  DU_APP: Invalid Search Space type");
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
uint8_t BuildPdcchCfgCommon(struct BWP_DownlinkCommon__pdcch_ConfigCommon *pdcchCfg)
{
   uint8_t   ret;
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
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 pdcchSetup = pdcchCfg->choice.setup;

	 /* Control Resource Set Zero */
	 DU_ALLOC(pdcchSetup->controlResourceSetZero, sizeof(ControlResourceSetZero_t)); 
	 if(!pdcchSetup->controlResourceSetZero)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 *pdcchSetup->controlResourceSetZero = duPdcchCfg.ctrlRsrcSetZero;

	 /* Search space zero */
	 DU_ALLOC(pdcchSetup->searchSpaceZero, sizeof(SearchSpaceZero_t));
	 if(!pdcchSetup->searchSpaceZero)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 *pdcchSetup->searchSpaceZero = duPdcchCfg.searchSpcZero;

	 /* Common Search Space List */
	 DU_ALLOC(pdcchSetup->commonSearchSpaceList,\
		  sizeof(struct PDCCH_ConfigCommon__commonSearchSpaceList));
	 if(!pdcchSetup->commonSearchSpaceList)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 elementCnt = ODU_VALUE_ONE;
	 pdcchSetup->commonSearchSpaceList->list.count = elementCnt;
	 pdcchSetup->commonSearchSpaceList->list.size =  elementCnt * sizeof(SearchSpace_t *);
	 ret = BuildCommonSerachSpaceList(pdcchSetup->commonSearchSpaceList);
	 if(ret != ROK)
	 {
	    return RFAILED;
	 }
	 CommonSerachSpaceListret=ROK;

	 /* Search Space for SIB1 */
	 DU_ALLOC(pdcchSetup->searchSpaceSIB1, sizeof(SearchSpaceId_t));
	 if(!pdcchSetup->searchSpaceSIB1)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 *pdcchSetup->searchSpaceSIB1 = duPdcchCfg.searchSpcSib1;

	 /* Serach Space for Paging */
	 DU_ALLOC(pdcchSetup->pagingSearchSpace, sizeof(SearchSpaceId_t));
	 if(!pdcchSetup->pagingSearchSpace)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }  
	 *pdcchSetup->pagingSearchSpace = duPdcchCfg.pagingSearchSpc;

	 /* Search space for Random Access */
	 DU_ALLOC(pdcchSetup->ra_SearchSpace, sizeof(SearchSpaceId_t));
	 if(!pdcchSetup->ra_SearchSpace)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }             
	 *pdcchSetup->ra_SearchSpace = duPdcchCfg.raSearchSpc;

	 break;
      }
      default :
      {
	 DU_LOG("\nERROR  -->  DU APP : Invalid PDCCH Config type");
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
uint8_t BuildPdschCfgCommon(struct BWP_DownlinkCommon__pdsch_ConfigCommon *pdschCfg)
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
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 pdschSetup = pdschCfg->choice.setup; 

	 /* Time Domain Allocation List */
	 DU_ALLOC(pdschSetup->pdsch_TimeDomainAllocationList, \
		  sizeof(PDSCH_TimeDomainResourceAllocationList_t));
	 if(!pdschSetup->pdsch_TimeDomainAllocationList)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 elementCnt = duPdschCfg.numTimeDomRsrcAlloc;
	 pdschSetup->pdsch_TimeDomainAllocationList->list.count = elementCnt;
	 pdschSetup->pdsch_TimeDomainAllocationList->list.size =  elementCnt * sizeof(PDSCH_TimeDomainResourceAllocation_t *);

	 DU_ALLOC(pdschSetup->pdsch_TimeDomainAllocationList->list.array,\
	 pdschSetup->pdsch_TimeDomainAllocationList->list.size);
	 if(!pdschSetup->pdsch_TimeDomainAllocationList->list.array)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	    return RFAILED;
	 }

	 for(idx=0; idx<elementCnt; idx++)
	 {
	    DU_ALLOC(pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx],\
	    sizeof(PDSCH_TimeDomainResourceAllocation_t));
	    if(!pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx])
	    {
	       DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
	       return RFAILED;
	    }
	 }

    for(idx = 0; idx < pdschSetup->pdsch_TimeDomainAllocationList->list.count; idx++)
    {
       timeDomRsrcAllocInfo = pdschSetup->pdsch_TimeDomainAllocationList->list.array[idx];

       /* K0 */
       DU_ALLOC(timeDomRsrcAllocInfo->k0, sizeof(long));
       if(!timeDomRsrcAllocInfo->k0)
       {
          DU_LOG("\nERROR  -->  DU APP : PDCCH Config memory alloc failed");
          return RFAILED;
       }
       *timeDomRsrcAllocInfo->k0 = duPdschCfg.timeDomAlloc[idx].k0;

       timeDomRsrcAllocInfo->mappingType = duPdschCfg.timeDomAlloc[idx].mapType;
       timeDomRsrcAllocInfo->startSymbolAndLength = duPdschCfg.timeDomAlloc[idx].sliv;
    }
	 break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  DU APP: Invalid PDSCH Configuration type");
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
uint8_t BuildBwpDlCommon(BWP_DownlinkCommon_t *bwp)
{
   uint8_t ret;
   DlCfgCommon duDlCfg;

   duDlCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg;

   bwp->genericParameters.locationAndBandwidth = duDlCfg.locAndBw;
   bwp->genericParameters.subcarrierSpacing = duDlCfg.dlScsCarrier.scs;

   /* PDCCH Config Common */
   DU_ALLOC(bwp->pdcch_ConfigCommon, \
	 sizeof(struct BWP_DownlinkCommon__pdcch_ConfigCommon));
   if(!bwp->pdcch_ConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : DL BWP memory allocation failed");
      return RFAILED;
   }
   ret=BuildPdcchCfgCommon(bwp->pdcch_ConfigCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
   /* PDSCH Config Common */
   DU_ALLOC(bwp->pdsch_ConfigCommon, \
	 sizeof(struct BWP_DownlinkCommon__pdsch_ConfigCommon));
   if(!bwp->pdsch_ConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : DL BWP memory allocation failed");
      return RFAILED;
   }
   ret=BuildPdschCfgCommon(bwp->pdsch_ConfigCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
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

uint8_t BuildBcchConfig(BCCH_Config_t *bcchCfg)
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
uint8_t BuildPcchConfig(PCCH_Config_t *pcchCfg)
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
         //TODO
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
	 DU_LOG("\nERROR  -->  DU APP : Invalid nAndPagingFrameOffset configuration");
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

uint8_t BuildFreqInfoDl(FrequencyInfoDL_SIB_t *frequencyInfoDL)
{
   uint8_t ret;
   uint8_t idx;
   uint8_t elementCnt;
   DlCfgCommon  dlCfg;
   NR_MultiBandInfo_t  *multiBandInfo;

   dlCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg;

   /* NR Multi-Band Info */
   elementCnt = ODU_VALUE_ONE;   
   frequencyInfoDL->frequencyBandList.list.count = elementCnt;  
   frequencyInfoDL->frequencyBandList.list.size = \
   elementCnt * sizeof(NR_MultiBandInfo_t *);

   DU_ALLOC(frequencyInfoDL->frequencyBandList.list.array, \
   frequencyInfoDL->frequencyBandList.list.size);
   if(!frequencyInfoDL->frequencyBandList.list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : SIB1 DL Configuration memory allocation failed");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      DU_ALLOC(frequencyInfoDL->frequencyBandList.list.array[idx],\
      sizeof(NR_MultiBandInfo_t));
      if(!frequencyInfoDL->frequencyBandList.list.array[idx])
      {
	 DU_LOG("\nERROR  -->  DU APP : SIB1 DL Configuration memory allocation failed");
	 return RFAILED;
      }
   }

   idx = 0;
   multiBandInfo = frequencyInfoDL->frequencyBandList.list.array[idx];

   /* Frequency band indicator */
   DU_ALLOC(multiBandInfo->freqBandIndicatorNR, sizeof(FreqBandIndicatorNR_t));
   if(!multiBandInfo->freqBandIndicatorNR)
   {
      DU_LOG("\nERROR  -->  DU APP : SIB1 DL Configuration memory allocation failed");
      return RFAILED;
   }
   *multiBandInfo->freqBandIndicatorNR = dlCfg.freqBandInd;

   /* Offset to Point A */
   frequencyInfoDL->offsetToPointA = dlCfg.offsetToPointA;

   /* Subcarrier Spacing specifc carrier */
   elementCnt = ODU_VALUE_ONE;
   frequencyInfoDL->scs_SpecificCarrierList.list.count = elementCnt;
   frequencyInfoDL->scs_SpecificCarrierList.list.size = \
   elementCnt * sizeof(SCS_SpecificCarrier_t *);
   ret= BuildScsSpecificCarrierListDl(&frequencyInfoDL->scs_SpecificCarrierList);

   if(ret != ROK)
   {
      return RFAILED;
   }
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

uint8_t BuildDlCfgCommSib(DownlinkConfigCommonSIB_t *dlCfg)
{
   /* DL frequency info */
   uint8_t ret;
   ret = BuildFreqInfoDl(&dlCfg->frequencyInfoDL);
   if(ret != ROK)
   {
      return RFAILED;
   }
   FreqInfoDlret=ROK;
   /* BWP Downlink Common */
   ret = BuildBwpDlCommon(&dlCfg->initialDownlinkBWP);
   if(ret != ROK)
   {
      return RFAILED;
   }
   /* BCCH Config */
   ret = BuildBcchConfig(&dlCfg->bcch_Config);
   if(ret != ROK)
   {
      return RFAILED;
   }
   /* PCCH Config */
   ret = BuildPcchConfig(&dlCfg->pcch_Config);
   if(ret != ROK)
   {
      return RFAILED;
   }
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

uint8_t BuildScsSpecificCarrierListUl
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
      DU_LOG("\nERROR  -->  DU APP : SCS Specific Carrier list memory allocation failed");
      return RFAILED;
   }

   for(idx = 0; idx < scsCarrierList->list.count; idx++)
   {
      DU_ALLOC(scsCarrierList->list.array[idx], sizeof(SCS_SpecificCarrier_t));
      if(!scsCarrierList->list.array[idx])
      {
	 DU_LOG("\nERROR  -->  DU APP : SCS Specific Carrier list memory allocation failed");
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

uint8_t BuildFreqInfoUl(FrequencyInfoUL_SIB_t *frequencyInfoUL)
{
   uint8_t      elementCnt;
   UlCfgCommon  ulCfg;
   uint8_t      ret;
   ulCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg;

   /* Subcarrier Spacing specifc carrier */
   elementCnt = ODU_VALUE_ONE;
   frequencyInfoUL->scs_SpecificCarrierList.list.count = elementCnt;
   frequencyInfoUL->scs_SpecificCarrierList.list.size = \
   elementCnt * sizeof(SCS_SpecificCarrier_t *);
   ret=BuildScsSpecificCarrierListUl(&frequencyInfoUL->scs_SpecificCarrierList);
   if(ret != ROK)
   {
      return RFAILED;
   }
   /* P-MAX */
   DU_ALLOC(frequencyInfoUL->p_Max, sizeof(P_Max_t));
   if(!frequencyInfoUL->p_Max)
   {
      DU_LOG("\nERROR  -->  DU APP : UL Frequency Infoo  memory allocation failure");
      return RFAILED;
   }
   *frequencyInfoUL->p_Max = ulCfg.pMax;
   FreqInfoUlret=ROK;
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

uint8_t  BuildRachCfgCommon(struct BWP_UplinkCommon__rach_ConfigCommon *rachCfg)
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
	    DU_LOG("\nERROR  -->  DU APP : Rach Config memory alloc failed");
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
	    DU_LOG("\nERROR  -->  DU APP : Rach Config memory alloc failed");
	    return RFAILED;
	 }
	 *setup->totalNumberOfRA_Preambles = duRachCfg.numRaPreamble;

	 /* SSB per RACH occassion and CB Preambles per SSB */
	 DU_ALLOC(setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB,\
	 sizeof(struct RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB));
	 if(!setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB)
	 {
	    DU_LOG("\nERROR  -->  DU APP : Rach Config memory alloc failed");
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
	       DU_LOG("\nERROR  -->  DU APP: Invalid value for \
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
	    DU_LOG("\nERROR  -->  DU APP : Rach Config memory alloc failed");
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
	       break;
	    }
	    case RACH_ConfigCommon__prach_RootSequenceIndex_PR_l139:
	    {
	       setup->prach_RootSequenceIndex.choice.l139 = duRachCfg.rootSeqIdx;
	       break;
	    }
	    default:
	    {
	       DU_LOG("\nERROR  -->  DU APP: Inavlid PRACH root sequence index type");
	       return RFAILED;
	    }
	 }

	 /* Msg 1 Subcarrier spacing */
	 DU_ALLOC(setup->msg1_SubcarrierSpacing, sizeof(SubcarrierSpacing_t));
	 if(!setup->msg1_SubcarrierSpacing)
	 {
	    DU_LOG("\nERROR  -->  DU APP : Rach Config memory alloc failed");
	    return RFAILED;
	 }
	 *setup->msg1_SubcarrierSpacing = duRachCfg.msg1Scs;

	 /* Restricted Set Config */
	 setup->restrictedSetConfig = duRachCfg.restrictedSetCfg;

	 break;
      }
      default:
      {
	 DU_LOG("\nERROR  -->  DU APP : Invalid RACH Config type ");
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

uint8_t BuildPuschCfgCommon(struct BWP_UplinkCommon__pusch_ConfigCommon *puschCfg)
{
   uint8_t           idx;
   uint8_t           elementCnt;   
   PuschCfgCommon    duPuschCfg;
   PUSCH_ConfigCommon_t *setup;
   PUSCH_TimeDomainResourceAllocation_t  *timeDomRsrcAllocInfo;

   duPuschCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.puschCfg;

   puschCfg->present = duPuschCfg.puschCfgPresent;
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
	    DU_LOG("\nERROR  -->  DU APP : PUSCH Config memory alloc failed");
	    return RFAILED;
	 }
	 setup = puschCfg->choice.setup;

	 /* Time Domain Resource Allocation List */
	 DU_ALLOC(setup->pusch_TimeDomainAllocationList,\
	 sizeof(PUSCH_TimeDomainResourceAllocationList_t)); 
	 if(!setup->pusch_TimeDomainAllocationList)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PUSCH Config memory alloc failed");
	    return RFAILED;
	 }
	 elementCnt = duPuschCfg.numTimeDomRsrcAlloc;
	 setup->pusch_TimeDomainAllocationList->list.count = elementCnt;
	 setup->pusch_TimeDomainAllocationList->list.size = \
	 elementCnt * sizeof(PUSCH_TimeDomainResourceAllocation_t *);

	 DU_ALLOC(setup->pusch_TimeDomainAllocationList->list.array, \
	 setup->pusch_TimeDomainAllocationList->list.size);
	 if(!setup->pusch_TimeDomainAllocationList->list.array)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PUSCH Config memory alloc failed");
	    return RFAILED;
	 }

	 for(idx=0; idx<elementCnt; idx++)
	 {
	    DU_ALLOC(setup->pusch_TimeDomainAllocationList->list.array[idx],\
	    sizeof(PUSCH_TimeDomainResourceAllocation_t));
	    if(!setup->pusch_TimeDomainAllocationList->list.array[idx])
	    {
	       DU_LOG("\nERROR  -->  DU APP : PUSCH Config memory alloc failed");
	       return RFAILED;
	    }
	 }

	 for(idx = 0; idx < elementCnt; idx++)
    {
       timeDomRsrcAllocInfo = setup->pusch_TimeDomainAllocationList->list.array[idx];

       /* K2 */
       DU_ALLOC(timeDomRsrcAllocInfo->k2, sizeof(long));
       if(!timeDomRsrcAllocInfo->k2)
       {
          DU_LOG("\nERROR  -->  DU APP : PUSCH Config memory alloc failed");
          return RFAILED;
       }
       *timeDomRsrcAllocInfo->k2 = duPuschCfg.timeDomAllocList[idx].k2;
       timeDomRsrcAllocInfo->mappingType = duPuschCfg.timeDomAllocList[idx].mapType;
       timeDomRsrcAllocInfo->startSymbolAndLength = duPuschCfg.timeDomAllocList[idx].sliv;
    }
	 /* Msg3 Delta Preamble */
	 DU_ALLOC(setup->msg3_DeltaPreamble, sizeof(long));
	 if(!setup->msg3_DeltaPreamble)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PUSCH Config memory alloc failed");
	    return RFAILED;
	 }
	 *setup->msg3_DeltaPreamble = duPuschCfg.msg3DeltaPreamble;

	 /* P0 Nominal with grnat */
	 DU_ALLOC(setup->p0_NominalWithGrant, sizeof(long));
	 if(!setup->p0_NominalWithGrant)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PUSCH Config memory alloc failed");
	    return RFAILED;
	 }
	 *setup->p0_NominalWithGrant = duPuschCfg.p0NominalWithGrant;

	 break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  DU APP : Invalid PUSCH configuration type ");
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

uint8_t BuildPucchCfgCommon( struct BWP_UplinkCommon__pucch_ConfigCommon *pucchCfg)
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
	    DU_LOG("\nERROR  -->  DU APP : PUCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 setup = pucchCfg->choice.setup;

	 /* Resource Common */
	 DU_ALLOC(setup->pucch_ResourceCommon, sizeof(long));
	 if(!setup->pucch_ResourceCommon)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PUCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 *setup->pucch_ResourceCommon = duPucchCfg.rsrcComm;

	 /* Group hopping */
	 setup->pucch_GroupHopping = duPucchCfg.grpHop;

	 /* P0 nominal */
	 DU_ALLOC(setup->p0_nominal, sizeof(long));
	 if(!setup->p0_nominal)
	 {
	    DU_LOG("\nERROR  -->  DU APP : PUCCH Config memory alloc failed");
	    return RFAILED;
	 }
	 *setup->p0_nominal = duPucchCfg.p0Nominal;

	 break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  DU APP : Invalid PUCCH Config type");
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

uint8_t BuildBwpUlCommon(BWP_UplinkCommon_t *bwp)
{
   uint8_t     ret;
   UlCfgCommon duUlCfg;

   duUlCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg;

   bwp->genericParameters.locationAndBandwidth = duUlCfg.locAndBw;
   bwp->genericParameters.subcarrierSpacing = duUlCfg.ulScsCarrier.scs;

   /* RACH Config Common */
   DU_ALLOC(bwp->rach_ConfigCommon,\
   sizeof(struct BWP_UplinkCommon__rach_ConfigCommon));
   if(!bwp->rach_ConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : UL BWP memory allocation failed");
      return RFAILED;
   }
   ret = BuildRachCfgCommon(bwp->rach_ConfigCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
   RachCfgCommonret=ROK;

   /* PUSCH Config Common */
   DU_ALLOC(bwp->pusch_ConfigCommon,\
   sizeof(struct BWP_UplinkCommon__pusch_ConfigCommon));
   if(!bwp->pusch_ConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : UL BWP memory allocation failed");
      return RFAILED;
   } 
   ret = BuildPuschCfgCommon(bwp->pusch_ConfigCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
   PuschCfgCommonret = ROK;

   /* PUCCH Config Common */
   DU_ALLOC(bwp->pucch_ConfigCommon,\
   sizeof(struct BWP_UplinkCommon__pucch_ConfigCommon));
   if(!bwp->pucch_ConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : UL BWP memory allocation failed");
      return RFAILED;
   } 
   ret = BuildPucchCfgCommon(bwp->pucch_ConfigCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
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

uint8_t BuildUlCfgCommSib(UplinkConfigCommonSIB_t *ulCfg)
{
   uint8_t ret;
   /* UL frequency info */
   ret = BuildFreqInfoUl(&ulCfg->frequencyInfoUL);
   if(ret != ROK)
   {
      return RFAILED;
   }
   /* BWP Uplink Common */
   ret = BuildBwpUlCommon(&ulCfg->initialUplinkBWP);
   if(ret != ROK)
   {
      return RFAILED;
   }
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

uint8_t BuildTddUlDlCfgComm(TDD_UL_DL_ConfigCommon_t *tddCfg)
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
uint8_t BuildServCellCfgCommonSib(ServingCellConfigCommonSIB_t *srvCellCfg)
{
   SrvCellCfgCommSib    duSrvCellCfg;
   BIT_STRING_t         *ssbPosInBurst;
   uint8_t              ret;
   duSrvCellCfg = duCfgParam.sib1Params.srvCellCfgCommSib;

   /* SSB Position in Burst */
   ssbPosInBurst = &srvCellCfg->ssb_PositionsInBurst.inOneGroup;
   ssbPosInBurst->size = 1;
   DU_ALLOC(ssbPosInBurst->buf, ssbPosInBurst->size * sizeof(uint8_t));
   if(!ssbPosInBurst->buf)
   {
      DU_LOG("\nERROR  -->  DU APP : Serving cell config common  memory allocation failure");
      return RFAILED;
   } 
   ssbPosInBurst->buf[0] = duSrvCellCfg.ssbPosInBurst;  
   ssbPosInBurst->bits_unused = 0;

   switch(duSrvCellCfg.ssbPrdServingCell)
   {
      case SSB_PERIODICITY_5MS:
	 srvCellCfg->ssb_PeriodicityServingCell = \
         ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms5;
	 break;
      case SSB_PERIODICITY_10MS:
	 srvCellCfg->ssb_PeriodicityServingCell = \
	 ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms10;
	 break;
      case SSB_PERIODICITY_20MS:
	 srvCellCfg->ssb_PeriodicityServingCell = \
	 ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms20;
	 break;
      case SSB_PERIODICITY_40MS:
	 srvCellCfg->ssb_PeriodicityServingCell = \
	 ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms40;
	 break;
      case SSB_PERIODICITY_80MS:
	 srvCellCfg->ssb_PeriodicityServingCell = \
	 ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms80;
	 break;
      case SSB_PERIODICITY_160MS:
	 srvCellCfg->ssb_PeriodicityServingCell = \
	 ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms160;
         break;
   }

   srvCellCfg->ss_PBCH_BlockPower = duSrvCellCfg.ssPbchBlockPwr;

   /* Downlink config common */
   ret = BuildDlCfgCommSib(&srvCellCfg->downlinkConfigCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
   DlCfgCommSibret = ROK;

   /* Uplink Config Comm */
   DU_ALLOC(srvCellCfg->uplinkConfigCommon, sizeof(UplinkConfigCommonSIB_t));
   if(!srvCellCfg->uplinkConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : Serving cell config common  memory allocation failure");
      return RFAILED;
   }
   ret = BuildUlCfgCommSib(srvCellCfg->uplinkConfigCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
   UlCfgCommSibret=ROK;

   /* TDD UL DL Config Comm */
   DU_ALLOC(srvCellCfg->tdd_UL_DL_ConfigurationCommon, sizeof(TDD_UL_DL_ConfigCommon_t));
   if(!srvCellCfg->tdd_UL_DL_ConfigurationCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : Serving cell config common  memory allocation failure");
      return RFAILED;
   }
   ret = BuildTddUlDlCfgComm(srvCellCfg->tdd_UL_DL_ConfigurationCommon);
   if(ret != ROK)
   {
      return RFAILED;
   }
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
uint8_t BuildSib1Msg()
{
   SIB1_t                   *sib1Msg;
   CellAccessRelatedInfo_t  *cellAccessInfo;
   uint8_t                  elementCnt;
   uint8_t                  ret1;
   asn_enc_rval_t           encRetVal; 
   uint8_t                  ret = RFAILED;

   do
   {
      DU_ALLOC(sib1Msg, sizeof(SIB1_t));
      if(!sib1Msg)
      {   
	 DU_LOG("\nERROR  -->  DU APP: SIB1 msg memory allocation failure");
	 break;
      }   

      elementCnt = ODU_VALUE_ONE;

      /* PLMN list */
      cellAccessInfo = &sib1Msg->cellAccessRelatedInfo;
      cellAccessInfo->plmn_IdentityList.list.count = elementCnt;
      cellAccessInfo->plmn_IdentityList.list.size = elementCnt * sizeof(PLMN_IdentityInfo_t *);

      ret1 =  BuildPlmnList(cellAccessInfo);
      if(ret1 != ROK)
      {
	 break;
      }
      /* Connection Establish Failure Control */
      DU_ALLOC(sib1Msg->connEstFailureControl, sizeof(ConnEstFailureControl_t));
      if(!sib1Msg->connEstFailureControl)
      {
	 DU_LOG("\nERROR  -->  DU APP: sib1Msg->connEstFailureControl memory allocation failure");
	 break;
      }
      sib1Msg->connEstFailureControl->connEstFailCount =\
      duCfgParam.sib1Params.connEstFailCnt;
      sib1Msg->connEstFailureControl->connEstFailOffsetValidity =\
      duCfgParam.sib1Params.connEstFailOffValidity;

      /* SI Scheduling Info */
      DU_ALLOC(sib1Msg->si_SchedulingInfo, sizeof(SI_SchedulingInfo_t));
      if(!sib1Msg->si_SchedulingInfo)
      {
	 DU_LOG("\nERROR  -->  DU APP: sib1Msg->si_SchedulingInfo memory allocation failure");
	 break;
      } 
      elementCnt = ODU_VALUE_ONE;
      sib1Msg->si_SchedulingInfo->schedulingInfoList.list.count = elementCnt;
      sib1Msg->si_SchedulingInfo->schedulingInfoList.list.size = elementCnt *
	 sizeof(struct SchedulingInfo *);
      ret1 = BuildSiSchedInfoList(&sib1Msg->si_SchedulingInfo->schedulingInfoList);
      if(ret1 != ROK)
      {
	 break;
      }
      sib1Msg->si_SchedulingInfo->si_WindowLength = duCfgParam.sib1Params.siSchedInfo.winLen;

      /* Serving Cell Config Common */
      DU_ALLOC(sib1Msg->servingCellConfigCommon, sizeof(ServingCellConfigCommonSIB_t));
      if(!sib1Msg->servingCellConfigCommon)
      {
	 DU_LOG("\nERROR  -->  DU APP: sib1Msg->servingCellConfigCommon memory allocation failure");
	 break;
      }
      ret1 =  BuildServCellCfgCommonSib(sib1Msg->servingCellConfigCommon);
      if(ret1 != ROK)
      {
	 break;
      }

      xer_fprint(stdout, &asn_DEF_SIB1, sib1Msg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_SIB1, 0, sib1Msg, PrepFinalEncBuf,\
	    encBuf);
      printf("\nencbufSize: %d\n", encBufSize);
      if(encRetVal.encoded == -1)
      {
	 DU_LOG("\nERROR  -->  DU APP : Could not encode SIB1 structure (at %s)\n",\
	       encRetVal.failed_type ?
	       encRetVal.failed_type->name :
	       "unknown");
	 break;
      }
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x\t",encBuf[i]);
      }
      printf("\n");

      ret = ROK;
      break; 
   }while(true);

   FreeSib1Msg(sib1Msg);

   return ret;


}
/*******************************************************************
 *
 * @brief      :  deallocating the memory of BuildSib1Msg 
 *
 * @details
 *
 *     Function : FreeFreqInfoDl
 *
 *    Functionality: Freeing memory of BuildFreqInfoDl 
 *
 * @params[in] : FrequencyInfoDL_SIB_t *frequencyInfoDL
 * @return     :  void
 *
 *******************************************************************/
void FreeFreqInfoDl(FrequencyInfoDL_SIB_t *frequencyInfoDL)
{
   uint8_t idx=0;
   uint8_t idx1=0;

   /* Free DL frequency info */
   if(!frequencyInfoDL->frequencyBandList.list.array)
   {
      /*Free Frequency band indicator */
      if(!frequencyInfoDL->frequencyBandList.list.array[idx])
      {
	 if(!frequencyInfoDL->frequencyBandList.list.array[idx]->\
	       freqBandIndicatorNR)
	 { 
	    if(!frequencyInfoDL->scs_SpecificCarrierList.list.array)
	    {
	       for(idx1 = 0;idx1<frequencyInfoDL->scs_SpecificCarrierList.list.count;idx1++)
	       {
		  if(!frequencyInfoDL->scs_SpecificCarrierList.list.array[idx1])
		  {
		     DU_FREE(frequencyInfoDL->scs_SpecificCarrierList.list.\
			   array[idx1], sizeof(SCS_SpecificCarrier_t));
		  }
	       }
	       DU_FREE(frequencyInfoDL->scs_SpecificCarrierList.list.array,
		     frequencyInfoDL->scs_SpecificCarrierList.list.size);
	    }
	    DU_FREE(frequencyInfoDL->frequencyBandList.list.\
		  array[idx]->freqBandIndicatorNR, sizeof(FreqBandIndicatorNR_t));
	 }
      }
      for(idx = 0; idx <frequencyInfoDL->frequencyBandList.list.count; idx++)
      {
	 if(!frequencyInfoDL->frequencyBandList.list.array[idx])
	 { 
	    DU_FREE(frequencyInfoDL->frequencyBandList.list.array[idx],\
		  sizeof(NR_MultiBandInfo_t));
	 }
      }
      DU_FREE(frequencyInfoDL->frequencyBandList.list.array,\
	    frequencyInfoDL->frequencyBandList.list.size);
   }
}
/*******************************************************************
 *
 * @brief  :  deallocating the memory of BuildSib1Msg
 *
 * @details
 *
 *     Function :  FreeCommonSerachSpaceList
 *
 *     Functionality:  deallocating the memory of BuildCommonSerachSpaceList
 *
 * @params[in] :struct PDCCH_ConfigCommon__commonSearchSpaceList  *searchSpclist
 * @return  : void
 *
 *******************************************************************/
void FreeCommonSerachSpaceList( struct PDCCH_ConfigCommon__commonSearchSpaceList
      *searchSpclist)
{
   uint8_t idx=0;
   SearchSpace_t *searchSpace= NULLP;

   if(!searchSpclist->list.array)
   {
      if( searchSpclist->list.array[idx] != NULLP)
      {
	 searchSpace= searchSpclist->list.array[idx];
	 if(!searchSpace->controlResourceSetId)
	 {
	    if(!searchSpace->monitoringSlotPeriodicityAndOffset)
	    {
	       if(!searchSpace->monitoringSymbolsWithinSlot)
	       {
		  if(!searchSpace->monitoringSymbolsWithinSlot->buf)
		  {
		     if(!searchSpace->nrofCandidates)
		     {
			if(!searchSpace->searchSpaceType)
			{
			   switch(searchSpace->searchSpaceType->present)
			   {
			      case SearchSpace__searchSpaceType_PR_NOTHING:
				 break;
			      case SearchSpace__searchSpaceType_PR_common:
		              {    
				 if(!searchSpace->searchSpaceType->choice.common)
				 {
				    if(!searchSpace->searchSpaceType->choice.\
					  common->dci_Format0_0_AndFormat1_0)
				    {
				       DU_FREE(searchSpace->searchSpaceType->choice.\
					     common->dci_Format0_0_AndFormat1_0,sizeof(struct\
						SearchSpace__searchSpaceType__common__dci_Format0_0_AndFormat1_0));
				    }
				    DU_FREE(searchSpace->searchSpaceType->choice.common,\
					  sizeof(struct SearchSpace__searchSpaceType__common));
				 }
				 break;
			      }
			      case SearchSpace__searchSpaceType_PR_ue_Specific:
				 break;
			      default:
				 break;
			   }
			   DU_FREE(searchSpace->searchSpaceType,\
				 sizeof( struct SearchSpace__searchSpaceType));
			}
			DU_FREE(searchSpace->nrofCandidates,\
			      sizeof(struct SearchSpace__nrofCandidates));
		     }
		     DU_FREE(searchSpace->monitoringSymbolsWithinSlot->buf,\
			   searchSpace->monitoringSymbolsWithinSlot->size);
		  }
		  DU_FREE(searchSpace->monitoringSymbolsWithinSlot,\
			sizeof(BIT_STRING_t));
	       }
	       DU_FREE(searchSpace->monitoringSlotPeriodicityAndOffset,\
		     sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
	    }
	    DU_FREE(searchSpace->controlResourceSetId,sizeof(ControlResourceSetId_t));
	 }
      }  
      for(idx = 0; idx < searchSpclist->list.count; idx++)
      {  
	 if(!searchSpclist->list.array[idx])
	 { 
	    DU_FREE(searchSpclist->list.array[idx], sizeof(SearchSpace_t));
	 }
      }
      DU_FREE(searchSpclist->list.array, searchSpclist->list.size)
   }
}
/*******************************************************************
 *
 * @brief : deallocating the memory of BuildSib1Msg
 *
 * @details
 *
 *     Function : FreeBwpDlCommon
 *
 *     Functionality :Deallocating memory of BuildBwpDlCommon
 *
 * @params[in] : BWP_DownlinkCommon_t *bwp
 *
 * @return      : void
 *
 *******************************************************************/
void FreeBwpDlCommon(BWP_DownlinkCommon_t *bwp)
{
   uint8_t idx =0;
   struct BWP_DownlinkCommon__pdsch_ConfigCommon *pdschCfg=bwp->pdsch_ConfigCommon;
   struct BWP_DownlinkCommon__pdcch_ConfigCommon *pdcchCfg=bwp->pdcch_ConfigCommon;
   pdcchCfg->present=duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pdcchCfg.present; 
   pdschCfg->present=duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pdschCfg.present;

   if(!bwp->pdcch_ConfigCommon)
   {
      if(!bwp->pdsch_ConfigCommon)
      {
	 switch( pdschCfg->present)
	 {
	    case BWP_DownlinkCommon__pdsch_ConfigCommon_PR_NOTHING:
	    {
	       //TODO
	       break;
	    }
	    case  BWP_DownlinkCommon__pdsch_ConfigCommon_PR_release:
	    { 
	       //TODO
	       break;
	    }
	    case BWP_DownlinkCommon__pdsch_ConfigCommon_PR_setup:
	    {
	       if(!pdschCfg->choice.setup)
	       {
		  if(!pdschCfg->choice.setup->pdsch_TimeDomainAllocationList)
		  {
		     if(!pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.array)
		     {
			if(pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.array[idx]!= NULLP)
			{
			   if(!pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.array[idx]->k0)
			   {
			      DU_FREE(pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.array[idx]->k0,\
				    sizeof(long)); 
			   }
			}
			for(idx=0; idx<pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.count ; idx++)
			{
			   if(pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.array[idx]!=
				 NULLP)
			   {
			      DU_FREE(pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.array[idx],\
				    sizeof(PDSCH_TimeDomainResourceAllocation_t));
			   }
			}
			DU_FREE(pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.array,\
			      pdschCfg->choice.setup->pdsch_TimeDomainAllocationList->list.size);
		     }
		     DU_FREE(pdschCfg->choice.setup->pdsch_TimeDomainAllocationList,\
			   sizeof(PDSCH_TimeDomainResourceAllocationList_t));
		  }
		  DU_FREE(pdschCfg->choice.setup,
			sizeof(PDSCH_ConfigCommon_t));
	       }
	    }
	    default:
	       break;
	 }
	 DU_FREE(bwp->pdsch_ConfigCommon,\
	       sizeof(struct BWP_DownlinkCommon__pdsch_ConfigCommon));
      }

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
	    if(!pdcchCfg->choice.setup)
	    {
	       /* Control Resource Set Zero */
	       if(! pdcchCfg->choice.setup->controlResourceSetZero)
	       {
		  /* Search space zero */
		  if(! pdcchCfg->choice.setup->searchSpaceZero)
		  {
		     /* Common Search Space List */
		     if(! pdcchCfg->choice.setup->commonSearchSpaceList)
		     {
			if(CommonSerachSpaceListret==ROK)
			{
			   if(!pdcchCfg->choice.setup->searchSpaceSIB1)
			   {
			      if(!pdcchCfg->choice.setup->pagingSearchSpace)
			      {
				 if(!pdcchCfg->choice.setup->ra_SearchSpace)
				 {
				    DU_FREE(pdcchCfg->choice.setup->ra_SearchSpace,
					  sizeof(SearchSpaceId_t));
				 }
				 DU_FREE( pdcchCfg->choice.setup->pagingSearchSpace,
				       sizeof(SearchSpaceId_t));
			      }
			      DU_FREE( pdcchCfg->choice.setup->searchSpaceSIB1,
				    sizeof(SearchSpaceId_t));
			   }
			}

			FreeCommonSerachSpaceList(pdcchCfg->choice.setup->commonSearchSpaceList);

			DU_FREE( pdcchCfg->choice.setup->commonSearchSpaceList,\
			      sizeof(struct
				 PDCCH_ConfigCommon__commonSearchSpaceList));
		     }
		     DU_FREE( pdcchCfg->choice.setup->searchSpaceZero,
			   sizeof(SearchSpaceZero_t));
		  }
		  DU_FREE( pdcchCfg->choice.setup->controlResourceSetZero,
			sizeof(ControlResourceSetZero_t));
	       }
	       DU_FREE(pdcchCfg->choice.setup,
		     sizeof(PDCCH_ConfigCommon_t));
	    }
	    break;
	 }
	 default:
	    break;
      }
      DU_FREE(bwp->pdcch_ConfigCommon,sizeof(struct BWP_DownlinkCommon__pdcch_ConfigCommon));
   }
}
/*******************************************************************
 *
 * @brief  deallocating the memory of BuildSib1Msg
 *
 * @details
 *
 *     Function: FreeUlCfgCommSib
 *
 *     Functionality deallocating the memory of BuildUlCfgCommSib
 *
 * @params[in] : UplinkConfigCommonSIB_t *ulCfg
 *
 * @return : void
 *
 *******************************************************************/
void FreeUlCfgCommSib(UplinkConfigCommonSIB_t *ulCfg)
{
   uint8_t                 idx=0;
   RACH_ConfigCommon_t    *setup;
   BWP_UplinkCommon_t     *bwp=&ulCfg->initialUplinkBWP;
   struct FrequencyInfoUL_SIB__scs_SpecificCarrierList *scsCarrierList;
   struct BWP_UplinkCommon__rach_ConfigCommon  *rachCfg;
   struct BWP_UplinkCommon__pucch_ConfigCommon *pucchCfg;
   struct BWP_UplinkCommon__pusch_ConfigCommon *puschCfg;
   puschCfg=ulCfg->initialUplinkBWP.pusch_ConfigCommon;
   rachCfg =bwp->rach_ConfigCommon;
   scsCarrierList    = &ulCfg->frequencyInfoUL.scs_SpecificCarrierList;

   if(!scsCarrierList->list.array)
   {
      if(!ulCfg->frequencyInfoUL.p_Max)
      {
	 if(FreqInfoUlret == ROK)
	 {
	    if(!bwp->rach_ConfigCommon)
	    {
	       if(RachCfgCommonret== ROK)
	       {
		  if(!bwp->pusch_ConfigCommon)
		  {

		     if(PuschCfgCommonret==ROK)
		     {
			if(!bwp->pucch_ConfigCommon)
			{
			   pucchCfg=bwp->pucch_ConfigCommon;
			   switch(pucchCfg->present)
			   {
			      case BWP_UplinkCommon__pucch_ConfigCommon_PR_NOTHING:
				 break;
			      case BWP_UplinkCommon__pucch_ConfigCommon_PR_release:
				 break;
			      case BWP_UplinkCommon__pucch_ConfigCommon_PR_setup:
			      {
				 if(!pucchCfg->choice.setup)
				 {

				    if(!pucchCfg->choice.setup->pucch_ResourceCommon)
				    {
				       if(!pucchCfg->choice.setup->p0_nominal)
				       {
					  DU_FREE(pucchCfg->choice.setup->p0_nominal,
						sizeof(long));
				       }
				       DU_FREE(pucchCfg->choice.setup->pucch_ResourceCommon,
					     sizeof(long));
				    }
				    DU_FREE(pucchCfg->choice.setup,
					  sizeof(PUCCH_ConfigCommon_t));
				 }
			      }
			      default:
				 break;
			   }
			   DU_FREE(bwp->pucch_ConfigCommon,\
		           sizeof(struct BWP_UplinkCommon__pucch_ConfigCommon));
			}
		     }
		     switch(puschCfg->present)
		     {
			case BWP_UplinkCommon__pusch_ConfigCommon_PR_NOTHING:
			   break;
			case BWP_UplinkCommon__pusch_ConfigCommon_PR_release:
			   break;
			case BWP_UplinkCommon__pusch_ConfigCommon_PR_setup:
			{
			   if(!puschCfg->choice.setup)
			   {

			      if(! puschCfg->choice.setup->pusch_TimeDomainAllocationList)
			      {
				 if(! puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.array)
				 {
				    if(!puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.array[idx])
				    {
				       if(!puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.array[idx]->k2)
				       {
					  if(! puschCfg->choice.setup->msg3_DeltaPreamble)
					  {
					     if(!puschCfg->choice.setup->p0_NominalWithGrant)
					     {
						DU_FREE(puschCfg->choice.setup->p0_NominalWithGrant,
						      sizeof(long));
					     }
					     DU_FREE(puschCfg->choice.setup->msg3_DeltaPreamble,
						   sizeof(long));
					  }
					  DU_FREE(puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.\
						array[idx]->k2, sizeof(long));
				       }
				    }
				    for(idx=0;
					  idx<puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.count;
					  idx++)
				    {
				       if(!puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.array[idx])
				       {
					  DU_FREE(puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.array[idx],\
						sizeof(PUSCH_TimeDomainResourceAllocation_t));
				       }
				    }
				    DU_FREE(puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.array,\
					  puschCfg->choice.setup->pusch_TimeDomainAllocationList->list.size);
				 }
				 DU_FREE(puschCfg->choice.setup->pusch_TimeDomainAllocationList,\
				       sizeof(PUSCH_TimeDomainResourceAllocationList_t));
			      }
			      DU_FREE(puschCfg->choice.setup,
				    sizeof(PUSCH_ConfigCommon_t));
			   }
			}
			default :
			   break;
		     }

		     DU_FREE(bwp->pusch_ConfigCommon,\
			   sizeof(struct BWP_UplinkCommon__pusch_ConfigCommon));
		  }
	       }
	       switch(rachCfg->present)
	       {
		  case BWP_UplinkCommon__rach_ConfigCommon_PR_NOTHING:
		     break;
		  case BWP_UplinkCommon__rach_ConfigCommon_PR_release:
		     break;
		  case BWP_UplinkCommon__rach_ConfigCommon_PR_setup:
		  {
		     if(!rachCfg->choice.setup)
		     {
			setup = rachCfg->choice.setup;
			if(!setup->totalNumberOfRA_Preambles)
			{
			   if(!setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB)
			   {
			      if(!setup->rsrp_ThresholdSSB)
			      {
				 if(!setup->msg1_SubcarrierSpacing)
				 {
				    DU_FREE(setup->msg1_SubcarrierSpacing,
					  sizeof(SubcarrierSpacing_t));
				 }
				 DU_FREE(setup->rsrp_ThresholdSSB,
				       sizeof(RSRP_Range_t));
			      }
			      DU_FREE(setup->ssb_perRACH_OccasionAndCB_PreamblesPerSSB,\
				    sizeof(struct
				       RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB));
			   }
			   DU_FREE(setup->totalNumberOfRA_Preambles,
				 sizeof(long));
			}

			DU_FREE(rachCfg->choice.setup,
			      sizeof(RACH_ConfigCommon_t));
		     }
		  }
		  default:
		     break;
	       }
	       DU_FREE(bwp->rach_ConfigCommon,\
	       sizeof(struct BWP_UplinkCommon__rach_ConfigCommon));
	    }
	 }
	 DU_FREE(ulCfg->frequencyInfoUL.p_Max, sizeof(P_Max_t));
      }    
      for(idx = 0; idx < scsCarrierList->list.count; idx++)
      {
	 if(!scsCarrierList->list.array[idx])
	 {
	    DU_FREE(scsCarrierList->list.array[idx],
		  sizeof(SCS_SpecificCarrier_t));
	 }
      }
      DU_FREE(scsCarrierList->list.array, scsCarrierList->list.size);
   }
}
/*******************************************************************
 *
 * @brief : Deallocating memory of  SIB1 message
 *
 * @details
 *
 *   Function : FreeServCellCfgCommonSib
 *
 *   Functionality: Deallocating memory of BuildServCellCfgCommonSib
 *
 * @params[in] : ServingCellConfigCommonSIB_t *srvCellCfg
 * @return     : void
 *
 *******************************************************************/
void FreeServCellCfgCommonSib(ServingCellConfigCommonSIB_t *srvCellCfg)
{
   BIT_STRING_t         *ssbPosInBurst;
   ssbPosInBurst = &srvCellCfg->ssb_PositionsInBurst.inOneGroup;
   DownlinkConfigCommonSIB_t *dlCfg=&srvCellCfg->downlinkConfigCommon;

   if(!ssbPosInBurst->buf)
   {
      /* Free DL frequency info */
      if(FreqInfoDlret == ROK)
      {
	 if(DlCfgCommSibret == ROK)
	 {
	    /* Uplink Config Comm */
	    if(!srvCellCfg->uplinkConfigCommon)
	    {
	       if(UlCfgCommSibret==ROK)
	       {
		  /* TDD UL DL Config Comm */
		  if(!srvCellCfg->tdd_UL_DL_ConfigurationCommon)
		  {
		     DU_FREE(srvCellCfg->tdd_UL_DL_ConfigurationCommon,
			   sizeof(TDD_UL_DL_ConfigCommon_t));
		  }
	       }
	       FreeUlCfgCommSib(srvCellCfg->uplinkConfigCommon);
	       DU_FREE(srvCellCfg->uplinkConfigCommon,
		     sizeof(UplinkConfigCommonSIB_t));  
	    }
	 }
	 FreeBwpDlCommon(&dlCfg->initialDownlinkBWP);
      }
      FreeFreqInfoDl(&dlCfg->frequencyInfoDL);
      DU_FREE(ssbPosInBurst->buf, ssbPosInBurst->size * sizeof(uint8_t));
   }
}
/*******************************************************************
 *
 * @brief   : Deallcating memory of the function BuildSib1Msg
 *
 * @details
 *
 *     Function :  FreeSib1Msg
 *
 *    Functionality: Deallcating memory of the function BuildSib1Msg
 *
 * @params[in] ServingCellConfigCommonSIB_t pointer
 * @return   void
 *
 *******************************************************************/
void FreeSib1Msg(SIB1_t *sib1Msg)
{
   uint8_t idx=0;
   uint8_t idx1=0;
   uint8_t idx2=0; 
   CellIdentity_t           *cellIdentity = NULLP;
   CellAccessRelatedInfo_t  *cellAccessInfo ;
   struct PLMN_IdentityInfo__plmn_IdentityList     *plmnIdInfo;


   if(sib1Msg != NULLP)
   {
      cellAccessInfo = &sib1Msg->cellAccessRelatedInfo;
      if(cellAccessInfo->plmn_IdentityList.list.array !=NULLP)
      {
	 if(cellAccessInfo->plmn_IdentityList.list.array[idx]!=NULLP)
	 {
	    plmnIdInfo =
	       &cellAccessInfo->plmn_IdentityList.list.array[idx]->plmn_IdentityList;

	    if(plmnIdInfo->list.array !=NULLP)
	    {
	       if(!plmnIdInfo->list.array[idx])
	       {
		  if(!plmnIdInfo->list.array[idx]->mcc)
		  {
		     if(!(plmnIdInfo->list.array[idx]->mcc->list.array))
		     {
			if(!plmnIdInfo->list.array[idx]->mnc.list.array)
			{	      
			   /*Free Tracking Area Code */
			   if(!cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode)
			   {	    
			      /*Free RANAC */
			      if(!cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode->buf)
			      {	   
				 /* Free CellIdentity */
				 if(!cellAccessInfo->plmn_IdentityList.list.array[idx]->ranac)
				 {
				    cellIdentity
				       =&cellAccessInfo->plmn_IdentityList.\
				       list.array[idx]->cellIdentity;
				    if(!cellIdentity->buf)
				    {
				       /*Free Connection Establish Failure Control */
				       if(!sib1Msg->connEstFailureControl)
				       {
					  /*Free Serving Cell Config Common */
					  if(!sib1Msg->si_SchedulingInfo)
					  {
					     /* Free Serving Cell Config Common* */
					     if(!sib1Msg->servingCellConfigCommon)
					     {
						/*Free BuildServCellCfgCommonSib*/
						FreeServCellCfgCommonSib(\
						      sib1Msg->servingCellConfigCommon);

						DU_FREE(sib1Msg->servingCellConfigCommon,
						      sizeof(ServingCellConfigCommonSIB_t));
					     }

					     DU_FREE(sib1Msg->si_SchedulingInfo,
						   sizeof(SI_SchedulingInfo_t));
					  }

					  DU_FREE(sib1Msg->connEstFailureControl,
						sizeof(ConnEstFailureControl_t));
				       }
				       DU_FREE(cellIdentity->buf,cellIdentity->size);

				    }
				    DU_FREE(cellAccessInfo->plmn_IdentityList.list.array[idx]->ranac, sizeof(RAN_AreaCode_t)); 

				 }
				 DU_FREE(cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode->buf,\
				       cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode->size);

			      }
			      DU_FREE(cellAccessInfo->plmn_IdentityList.list.array[idx]->trackingAreaCode\
				    , sizeof(TrackingAreaCode_t));
			   }

			   for(idx2=0; idx2<plmnIdInfo->list.array[idx1]->mnc.list.count; idx2++)
			   {
			      if(!plmnIdInfo->list.array[idx2]->mnc.list.array[idx2])
			      {
				 DU_FREE(plmnIdInfo->list.array[idx2]->mnc.list.array[idx2],
				       sizeof(MCC_MNC_Digit_t));
			      } 
			   }
			   DU_FREE(plmnIdInfo->list.array[idx]->mnc.list.array,
				 plmnIdInfo->list.array[idx1]->mnc.list.size);
			}

			for(idx1=0; idx1<plmnIdInfo->list.array[idx]->mcc->list.count; idx1++)
			{
			   if(plmnIdInfo->list.array[idx]->mcc->list.array[idx1]!=NULLP)
			   {
			      DU_FREE(plmnIdInfo->list.array[idx]->mcc->list.array[idx1],\
				    sizeof(MCC_MNC_Digit_t));
			   }

			}
			DU_FREE(plmnIdInfo->list.array[idx]->mcc->list.array,\
			      plmnIdInfo->list.array[idx]->mcc->list.size)
		     }
		     DU_FREE(plmnIdInfo->list.array[idx]->mcc,sizeof(MCC_t));
		  }
	       }
	       for(idx1=0; idx1<plmnIdInfo->list.count; idx1++)
	       {
		  if(!(plmnIdInfo->list.array[idx1]))
		  {
		     DU_FREE(plmnIdInfo->list.array[idx1],
			   sizeof(PLMN_IdentitY_t));
		  }
	       }
	       DU_FREE(plmnIdInfo->list.array, plmnIdInfo->list.size);
	    }
	 }
	 for(idx=0; idx<cellAccessInfo->plmn_IdentityList.list.count; idx++)
	 {
	    if(cellAccessInfo->plmn_IdentityList.list.array[idx]!=NULLP)
	    { 
	       DU_FREE(cellAccessInfo->plmn_IdentityList.list.array[idx],
		     sizeof(PLMN_IdentityInfo_t));
	    }
	 }
	 DU_FREE(cellAccessInfo->plmn_IdentityList.list.array,
	       cellAccessInfo->plmn_IdentityList.list.size);
      }
      DU_FREE(sib1Msg, sizeof(SIB1_t)); 
   }

}

/**********************************************************************
  End of file
 **********************************************************************/
