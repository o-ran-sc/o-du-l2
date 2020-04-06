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
#include "SIB1.h"
#include "PLMN-IdentityInfo.h"
#include "PLMN-IdentitY.h"
#include "MCC.h"
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
	struct PLMN_IdentityInfo__plmn_IdentityList
		*plmnIdInfo;

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

	DU_FREE(sib1Msg, sizeof(SIB1_t));
   return ROK;

}

/**********************************************************************
  End of file
 **********************************************************************/
