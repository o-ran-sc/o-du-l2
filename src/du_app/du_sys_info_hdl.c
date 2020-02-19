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

#include "du_sys_info_hdl.h"
#include "MIB.h"

extern char encBuf[ENC_BUF_MAX_LEN];

extern DuCfgParams duCfgParam;

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
 * @params[in] GNB_DU_System_Information *gnbDuSysInfo
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildMibMsg()
{
	MIB_t          *mibMsg;
	asn_enc_rval_t encRetVal;        /* Encoder return value */

	DU_ALLOC(mibMsg, sizeof(MIB_t));
	if(!mibMsg)
	{
		DU_LOG("F1AP: MIB msg memory allocation failure");
		return RFAILED;
	}
	mibMsg->systemFrameNumber.size = sizeof(uint8_t);
	DU_ALLOC(mibMsg->systemFrameNumber.buf, 
			mibMsg->systemFrameNumber.size);
	*(mibMsg->systemFrameNumber.buf) =
		duCfgParam.mibParams.sysFrmNum;
	mibMsg->systemFrameNumber.bits_unused = 2;

	mibMsg->subCarrierSpacingCommon =\ 
		duCfgParam.mibParams.subCarrierSpacingCommon;
	mibMsg->ssb_SubcarrierOffset =\
	   duCfgParam.mibParams.ssb_SubcarrierOffset;
	mibMsg->dmrs_TypeA_Position =\
		duCfgParam.mibParams.dmrs_TypeA_Position;
	mibMsg->pdcch_ConfigSIB1.controlResourceSetZero = \
	   duCfgParam.mibParams.controlResourceSetZero;
	mibMsg->pdcch_ConfigSIB1.searchSpaceZero = \
	   duCfgParam.mibParams.searchSpaceZero;
	mibMsg->cellBarred = duCfgParam.mibParams.cellBarred;
	mibMsg->intraFreqReselection =
		duCfgParam.mibParams.intraFreqReselection;
	mibMsg->spare.size = sizeof(uint8_t);
	DU_ALLOC(mibMsg->spare.buf,sizeof(uint8_t));
	*(mibMsg->spare.buf) = SPARE;
	mibMsg->spare.bits_unused = 7;

	xer_fprint(stdout, &asn_DEF_MIB, mibMsg);
	cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
	encBufSize = 0;
	encRetVal = aper_encode(&asn_DEF_MIB, 0,
				   mibMsg, PrepFinalEncBuf, encBuf);
	printf("\nencbufSize:%d\n", encBufSize);
	if(encRetVal.encoded	== -1) 
	{   
		DU_LOG("\nF1AP: Could not encode MIB structure(at %s)\n", 
				encRetVal.failed_type?\
				encRetVal.failed_type->name
				:"unknown");
		return RFAILED;
	}   
   return ROK;

}

/**********************************************************************
  End of file
 **********************************************************************/
