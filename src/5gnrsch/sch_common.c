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

/************************************************************************

Name:     5G NR SCH layer

Type:     C source file

Desc:     C source code for Entry point fucntions

File:     sch_common.c

 **********************************************************************/

/** @file sch_common.c
  @brief This module performs common scheduling
 */
#include "stdbool.h"
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "tfu.h"
#include "lrg.h"

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"

extern SchCb schCb[SCH_MAX_INST];
extern uint16_t prachCfgIdxTable[MAX_PRACH_CONFIG_IDX][8];
extern uint16_t numRbForPrachTable[MAX_RACH_NUM_RB_IDX][5];

SchMacUlSchInfoFunc schMacUlSchInfoOpts[] =
{
	packSchMacUlSchInfo,
	MacProcUlSchInfo,
	packSchMacUlSchInfo
};

/**
 * @brief Time domain allocation for SSB
 *
 * @details
 *
 *     Function : ssbDlTdAlloc 
 *     
 *     This function handles common scheduling for DL
 *     
 *  @param[in]  schCellCb *cell, cell cb
 *  @param[in]  DlBrdcstAlloc *dlBrdcstAlloc, DL brdcst allocation
 *  @return  void
 **/
void ssbDlTdAlloc(uint8_t scs, uint8_t *ssbStartSymb)
{
   uint8_t n;
	/* Determine value of "n" based on Section 4.1 of 3GPP TS 38.213 */
	switch(scs)
	{
		case SCH_SCS_15KHZ:
			{
			   uint8_t symbIdx=0;
			   n = 2;/* n = 0, 1 for SCS = 15KHz */
				for(uint8_t idx=0; idx<n; idx++)
				{
               /* start symbol determined using {2, 8} + 14n */
					ssbStartSymb[symbIdx++] = 2 + SCH_SYMBOL_PER_SLOT*idx;
					ssbStartSymb[symbIdx++] = 8 + SCH_SYMBOL_PER_SLOT*idx;
				}
			}
			break;
		default:
			DU_LOG("\nSCS %d is currently not supported", scs);
	}
}

/**
 * @brief common resource allocation for SSB
 *
 * @details
 *
 *     Function : schCmnDlAlloc
 *     
 *     This function handles common scheduling for DL
 *     
 *  @param[in]  SchCellCb *cell, cell cb
 *  @param[in]  DlBrdcstAlloc *dlBrdcstAlloc, DL brdcst allocation
 *  @return  void
 **/
uint8_t schCmnDlAlloc(SchCellCb *cell, DlBrdcstAlloc *dlBrdcstAlloc)
{
	/* schedule SSB */
	uint8_t scs, ssbStartPrb, ssbStartSymb, idx;
	uint8_t ssbStartSymbArr[SCH_MAX_SSB_BEAM];
	SchDlAlloc *dlAlloc;
	SsbInfo ssbInfo;

	dlAlloc = cell->dlAlloc[cell->slotInfo.slot];
	if(dlBrdcstAlloc->ssbTrans)
	{
		scs = cell->cellCfg.ssbSchCfg.scsCommon;
		ssbStartPrb = \
		   (cell->cellCfg.ssbSchCfg.ssbOffsetPointA)/SCH_NUM_SC_PRB;

		memset(ssbStartSymbArr, 0, SCH_MAX_SSB_BEAM);
		ssbDlTdAlloc(scs, ssbStartSymbArr);
		ssbStartSymb = ssbStartSymbArr[dlBrdcstAlloc->ssbIdxSupported-1]; /*since we are supporting only 1 ssb beam */
		/* Assign interface structure */
		for(idx=0; idx<dlBrdcstAlloc->ssbIdxSupported; idx++)
		{
			ssbInfo.ssbIdx = idx;
			ssbInfo.fdAlloc.ssbStartPrbIdx = ssbStartPrb;
			ssbInfo.fdAlloc.ssbPrbDuration = SCH_SSB_PRB_DURATION;
			ssbInfo.tdAlloc.ssbStartSymbIdx = ssbStartSymb;
			ssbInfo.tdAlloc.ssbSymbolDuration = SCH_SSB_SYMB_DURATION;
			dlBrdcstAlloc->ssbInfo[idx] = ssbInfo;
			dlAlloc->ssbInfo[idx] = ssbInfo;

		}

		dlAlloc->ssbPres = true;
		dlAlloc->ssbIdxSupported = dlBrdcstAlloc->ssbIdxSupported;
		for(idx=ssbStartSymb; idx<ssbStartSymb+SCH_SSB_SYMB_DURATION; idx++)
		{
			dlAlloc->assignedPrb[idx] = SCH_SSB_PRB_DURATION + 1; /* +1 for kSsb */
		}

	}

	/* SIB1 allocation */
	if(dlBrdcstAlloc->sib1Trans)
	{
	   memcpy(&dlBrdcstAlloc->sib1Alloc.sib1PdcchCfg, &cell->cellCfg.sib1SchCfg.sib1PdcchCfg, sizeof(Sib1PdcchCfg)); 
	   memcpy(&dlBrdcstAlloc->sib1Alloc.sib1PdschCfg, &cell->cellCfg.sib1SchCfg.sib1PdschCfg, sizeof(Sib1PdschCfg)); 
	}
	return ROK;
}

/*******************************************************************
 *
 * @brief Handles sending UL scheduler info to MAC 
 *
 * @details
 *
 *    Function : sendUlSchInfoToMac
 *
 *    Functionality:
 *     Sends UL Sch info to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendUlSchInfoToMac(UlSchInfo *ulSchInfo, Inst inst)
{
	Pst pst;

   memset(&pst, 0, sizeof(Pst));
   SCH_FILL_RSP_PST(pst, inst);
	pst.event = EVENT_UL_SCH_INFO;

	return(*schMacUlSchInfoOpts[pst.selector])(&pst, ulSchInfo);

}
/**
 * @brief resource allocation for PRACH
 *
 * @details
 *
 *     Function : schPrachResAlloc
 *     
 *     This function handles PRACH allocation
 *     
 *  @param[in]  SchCellCb *cell, cell cb
 *  @param[in]  UlSchInfo *ulSchInfo, UL scheduling info
 *  @return  void
 **/
int schPrachResAlloc(SchCellCb *cell, UlSchInfo *ulSchInfo)
{
   uint8_t  numPrachRb;
	uint8_t  numRa;
	uint8_t  freqStart;
	uint16_t sfn;
	uint16_t slot;
	uint8_t  prachCfgIdx = 0;
	uint8_t  prachFormat = 0;
	uint8_t  x = 0;
	uint8_t  y = 0;
	uint8_t  prachSubframe = 0;
	uint8_t  prachStartSymbol = 0;
	uint8_t  prachOcas = 0;
	uint8_t  dataType = 0;
	uint8_t  idx = 0;
	SchUlAlloc *ulAlloc = NULLP;

	sfn  = cell->slotInfo.sfn;
	slot = cell->slotInfo.slot; 
	ulAlloc = cell->ulAlloc[cell->slotInfo.slot];
	prachCfgIdx = cell->cellCfg.schRachCfg.prachCfgIdx;

    /* derive the prachCfgIdx table paramters */
	x                = prachCfgIdxTable[prachCfgIdx][1];
	y                = prachCfgIdxTable[prachCfgIdx][2];
	prachSubframe    = prachCfgIdxTable[prachCfgIdx][3];
	
	if((sfn%x) != y)
	{
	   /* prach occasion does not lie in this SFN */
		DU_LOG("\nPRACH ocassion doesn't lie in this SFN");
	   return RFAILED;
	}
	/* check for subFrame number */
	if ((1 << slot) & prachSubframe)
	{
      /* prach ocassion present in this subframe */

		prachFormat      = prachCfgIdxTable[prachCfgIdx][0];
		prachStartSymbol = prachCfgIdxTable[prachCfgIdx][4];
		prachOcas        = prachCfgIdxTable[prachCfgIdx][6];

		/* freq domain resource determination for RACH*/
		freqStart = cell->cellCfg.schRachCfg.msg1FreqStart;
		/* numRa determined as 𝑛 belonging {0,1,.., M − 1}, 
		 * where M is given by msg1Fdm */
		numRa = (cell->cellCfg.schRachCfg.msg1Fdm - 1);
		for(idx=0; idx<MAX_RACH_NUM_RB_IDX; idx++)
		{
         if(numRbForPrachTable[idx][0] == cell->cellCfg.schRachCfg.rootSeqIdx)
			   break;
		}
		numPrachRb = numRbForPrachTable[idx][3];
		/* Considering first slot in the frame for PRACH */
		idx = 0;
		ulAlloc->assignedPrb[idx] = freqStart+numPrachRb;
	}

	/* Fill UL SCH Info */
	ulSchInfo->cellId = cell->cellId;
	ulSchInfo->slotIndInfo.sfn = sfn;
   ulSchInfo->slotIndInfo.slot = slot;
	ulSchInfo->dataType = dataType | SCH_DATATYPE_PRACH;
	/* prach info */
	ulSchInfo->prachSchInfo.numPrachOcas   = prachOcas;
	ulSchInfo->prachSchInfo.prachFormat    = prachFormat;
	ulSchInfo->prachSchInfo.numRa          = numRa;
	ulSchInfo->prachSchInfo.prachStartSymb = prachStartSymbol;

	return ROK;
}

/**
 * @brief resource allocation for UL
 *
 * @details
 *
 *     Function : schUlResAlloc
 *     
 *     This function handles UL Resource allocation
 *     
 *  @param[in]  SchCellCb *cell, cellCb
 *  @return  void
 **/
int schUlResAlloc(SchCellCb *cell, Inst schInst)
{
   int ret = ROK;
   UlSchInfo ulSchInfo;
   /* Schedule resources for PRACH */
	schPrachResAlloc(cell, &ulSchInfo);
	//send msg to MAC
   ret = sendUlSchInfoToMac(&ulSchInfo, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nSending UL Sch info from SCH to MAC failed");
   }
    
	return ret;
}
/**********************************************************************
  End of file
 **********************************************************************/

