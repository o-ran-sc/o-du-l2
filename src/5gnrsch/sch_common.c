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

extern SchCb schCb[SCH_MAX_INST];

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
int schCmnDlAlloc(SchCellCb *cell, DlBrdcstAlloc *dlBrdcstAlloc)
{
	/* schedule SSB */
	uint8_t scs, ssbStartPrb, ssbStartSymb, idx;
	uint8_t ssbStartSymbArr[SCH_MAX_SSB_BEAM];
	SchDlAlloc *dlAlloc;
	SsbInfo ssbInfo;

	dlAlloc = cell->dlAlloc[cell->slotInfo.slot];
	if(dlBrdcstAlloc->ssbTrans)
	{
		scs = cell->cellCfg.scsCommon;
		ssbStartPrb = \
		   ((cell->cellCfg.ssbSubcOffset)-(cell->cellCfg.ssbOffsetPointA))/SCH_NUM_SC_PRB;

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
			dlAlloc->assignedPrb[idx] = SCH_SSB_PRB_DURATION; 
		}

	}
	return ROK;
}


/**********************************************************************
  End of file
 **********************************************************************/

