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

/* This file contains F1AP message handler functions */
#include "common_def.h"
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "odu_common_codec.h"
#include "cu_stub_sctp.h"
#include "cu_f1ap_msg_hdl.h"
/*header files needed for Ue Context Setup */
#include<ProtocolIE-Field.h>
#include "ProtocolExtensionField.h"
#include "F1AP-PDU.h"
#include "ProtocolExtensionContainer.h"
#include "CellGroupConfigRrc.h"
#include "MAC-CellGroupConfig.h"
#include "SchedulingRequestConfig.h"
#include "SchedulingRequestToAddMod.h"
#include "BSR-Config.h"
#include "TAG-Config.h"
#include "TAG.h"
#include "PHR-Config.h"
#include "RLC-Config.h"
#include "UL-AM-RLC.h"
#include "DL-AM-RLC.h"
#include "LogicalChannelConfig.h"
#include "RLC-BearerConfig.h"
#include "PhysicalCellGroupConfig.h"
#include "SpCellConfig.h"
#include "ServingCellConfig.h"
#include "ControlResourceSet.h"
#include "SearchSpace.h"
#include "PDCCH-Config.h"
#include "PDSCH-TimeDomainResourceAllocation.h"
#include "PDSCH-TimeDomainResourceAllocationList.h"
#include "DMRS-DownlinkConfig.h"
#include "PDSCH-Config.h"
#include "BWP-DownlinkDedicated.h"
#include "PUSCH-TimeDomainResourceAllocation.h"
#include "PUSCH-TimeDomainResourceAllocationList.h"
#include "DMRS-UplinkConfig.h"
#include "PUSCH-Config.h"
#include "SRS-ResourceId.h"
#include "SRS-Resource.h"
#include "SRS-ResourceSet.h"
#include "SRS-Config.h"
#include "BWP-UplinkDedicated.h"
#include "PUSCH-ServingCellConfig.h"
#include "UplinkConfig.h"
#include "PDSCH-ServingCellConfig.h"

/* MACRO for CUtoDURRCInformation */
#define CELL_GRP_ID 1
#define FREQ_DOM_RSRC_SIZE 6
#define CORESET0_END_PRB   48
#define CORESET1_NUM_PRB   24
#define PDCCH_CTRL_RSRC_SET_ONE_ID  1
#define PDCCH_CTRL_RSRC_SET_ONE_DURATION 2  /* Duration for control resource set id i */
#define PDCCH_CTRL_RSRC_SET_ONE_PRECOD_GRANULARITY 1   /* Precoded granularity */
#define PDCCH_SRCH_SPC_TWO_ID 2
#define NR_PCI 1
#define SCRAMBLING_ID  NR_PCI
#define DMRS_ADDITIONAL_POS  0          /* DMRS Additional poistion */
#define PDCCH_SYMBOL_WITHIN_SLOT 128     /* Symbol within Slot Value */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL1_CANDIDATE 7   /* Num of candidate at aggregation level 1 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL2_CANDIDATE 7   /* Num of candidate at aggregation level 2 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL4_CANDIDATE 4   /* Num of candidate at aggregation level 4 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL8_CANDIDATE 2   /* Num of candidate at aggregation level 8 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL16_CANDIDATE 1  /* Num of candidate at aggregation level 16 */
#define PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT  0  /* format 0-0 and 1-0 */
#define PDCCH_SERACH_SPACE_DCI_FORMAT 0

/* MACRO Ddefine for PDSCH Configuration */
#define PDSCH_K0  0
#define PDSCH_RES_ALLOC_TYPE       1          /* Resource allocation type */
#define PDSCH_MAX_CODEWORD_SCH_BY_DCI 0       /* Max num of codewords scheduled by DCI */
#define PDSCH_RBG_SIZE   0                    /* 0: config1 */
#define PDSCH_NUM_HARQ_PROC 5
#define PDSCH_MAPPING_TYPE_A 0
#define PDSCH_MAPPING_TYPE_B 1

/* MACRO Define for PUSCH Configuration */
#define PUSCH_K2  3

#define PUSCH_MSG3_DELTA_PREAMBLE 0
#define PUSCH_P0_NOMINAL_WITH_GRANT -70
#define PUSCH_TRANSFORM_PRECODER    1      /* 1: Disabled */
#define PUSCH_MAX_MIMO_LAYERS       1
#define PUSCH_PROCESS_TYPE2_ENABLED false
#define PUSCH_MAPPING_TYPE_A        0
#define PUSCH_MAPPING_TYPE_B        1

/* MACRO defines for SRC config */
#define SRS_RSRC_ID  1
#define SRS_RSET_ID  1
#define SRS_COMB_OFFSET_N2   0
#define SRS_CYCLIC_SHIFT_N2  0
#define SRS_FREQ_DOM_POS     0
#define SRS_FREQ_DOM_SHIFT   0
#define C_SRS 0
#define B_SRS 0
#define B_HOP 0
#define SRS_SEQ_ID 0
#define APERIODIC_SRS_RESRC_TRIGGER 1 

/* Macro definitions for DUtoCuRrcContainer */
#define CELL_GRP_ID 1
#define SCH_REQ_ID  0
#define SR_PROHIBIT_TMR 5
#define SR_TRANS_MAX 2
#define PERIODIC_BSR_TMR 2
#define RETX_BSR_TMR     5
#define SR_DELAY_TMR     6
#define TAG_ID 0
#define TIME_ALIGNMENT_TMR 7
#define PHR_PERIODIC_TMR 7
#define PHR_PROHIBHIT_TMR 0
#define PHR_PWR_FACTOR_CHANGE 3
#define PHR_MODE_OTHER_CG 0
#define RLC_LCID 1 
#define SRB_ID_1 1
#define SN_FIELD_LEN 0
#define T_POLL_RETRANSMIT 8 
#define POLL_PDU 0
#define POLL_BYTE 43
#define MAX_RETX_THRESHOLD 5
#define T_REASSEMBLY 8
#define T_STATUS_PROHIBHIT 7
#define MAC_LC_PRIORITY 1
#define PRIORTISIED_BIT_RATE 15
#define BUCKET_SIZE_DURATION 5
#define LC_GRP 0
#define P_NR_FR1 0
#define PDSCH_HARQ_ACK_CODEBOOK 1
#define SERV_CELL_IDX 0
#define RLM_SYNC_OUT_SYNC_THRESHOLD 0
#define ACTIVE_DL_BWP_ID 0
#define ACTIVE_UL_BWP_ID 0
#define SCRAMBLING_ID  NR_PCI
#define DMRS_ADDITIONAL_POS  0          /* DMRS Additional poistion */
#define RES_ALLOC_TYPE       1          /* Resource allocation type */

/*******************************************************************
*
* @brief Sends F1 msg over SCTP
*
* @details
*
*    Function : SendF1APMsg
*
*    Functionality: Sends F1 msg over SCTP
*
* @params[in] Region region
*             Pool pool
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
S16 SendF1APMsg(Region region, Pool pool)
{
   Buffer *mBuf = NULLP;

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         ODU_PRINT_MSG(mBuf, 0,0);
 
         if(sctpSend(mBuf) != ROK)
         {
            DU_LOG("\nF1AP : SCTP Send failed");
            ODU_PUT_MSG_BUF(mBuf);
            return RFAILED;
         }
      }
      else
      {
         DU_LOG("\nF1AP : ODU_ADD_POST_MSG_MULT failed");
         ODU_PUT_MSG_BUF(mBuf);
         return RFAILED;
      }
      ODU_PUT_MSG_BUF(mBuf);
   }
   else
   {
      DU_LOG("\nF1AP : Failed to allocate memory");
      return RFAILED;
   }

   return ROK;
} /* SendF1APMsg */

/*******************************************************************
 *
 * @brief Builds NRCell ID 
 *
 * @details
 *
 *    Function : BuildNrCellId
 *
 *    Functionality: Building the NR Cell ID
 *
 * @params[in] BIT_STRING_t *nrcell
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildNrCellId(BIT_STRING_t *nrcell)
{
   uint8_t tmp;
   for (tmp = 0 ; tmp < nrcell->size-1; tmp++)
   {
      nrcell->buf[tmp] = 0;
   }
   nrcell->buf[4]   = 16; 
   nrcell->bits_unused = 4;
   nrcell->size = 5 * sizeof(uint8_t);
   return ROK;
}

/********************************************************************
 *
 * @brief Builds and sends the F1SetupResponse
 *
 * @details
 *
 *    Function : BuildAndSendF1SetupRsp
 *
 *    Functionality: Constructs the F1SetupResponse message and sends
 *                   it back to the DU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildAndSendF1SetupRsp()
{
   uint8_t    idx,ieIdx;
   uint8_t    elementCnt,cellCnt;
   F1AP_PDU_t         *f1apMsg = NULL;
   F1SetupResponse_t  *f1SetupRsp;
   GNB_CU_Name_t      *cuName;
   Cells_to_be_Activated_List_t *cellToActivate;
   RRC_Version_t      *rrcVer;
   asn_enc_rval_t     encRetVal; 
   DU_LOG("\nF1AP : Building F1 Setup Response\n");

   /* Allocate the memory for F1SetupRequest_t */
   CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t)); 
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }
   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;  
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_F1Setup;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = \
						      SuccessfulOutcome__value_PR_F1SetupResponse;
   f1SetupRsp = &f1apMsg->choice.successfulOutcome->value.choice.F1SetupResponse;

   elementCnt = 4;
   f1SetupRsp->protocolIEs.list.count = elementCnt;
   f1SetupRsp->protocolIEs.list.size = elementCnt*sizeof(F1SetupResponseIEs_t *);

   CU_ALLOC(f1SetupRsp->protocolIEs.list.array, \
	 elementCnt * sizeof(F1SetupResponseIEs_t *));
   if(f1SetupRsp->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1ResponseIEs failed");
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(f1SetupRsp->protocolIEs.list.array[idx], \
	    sizeof(F1SetupResponseIEs_t)); 
      if(f1SetupRsp->protocolIEs.list.array[idx] == NULLP)
      {  
	 CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	       elementCnt * sizeof(F1SetupResponseIEs_t *));
	 CU_FREE(f1apMsg->choice.successfulOutcome, \
	       sizeof(SuccessfulOutcome_t));
	 CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
	 return RFAILED;
      }    
   }

   /*TransactionID*/
   idx = 0;
   f1SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
							    F1SetupResponseIEs__value_PR_TransactionID;
   f1SetupRsp->protocolIEs.list.array[idx]->value.choice.TransactionID =\
									TRANS_ID;

   /*CU Name*/
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_gNB_CU_Name;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
							    F1SetupResponseIEs__value_PR_GNB_CU_Name;
   cuName = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_Name;
   cuName->size = sizeof(cuCfgParams.cuName);

   CU_ALLOC(cuName->buf, sizeof(cuName->size)); 
   if(cuName->buf == NULLP)
   {
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx],\
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome,\
	    sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   strcpy((char*)cuName->buf, (char*)cuCfgParams.cuName);

   /*Cells to be activated list*/
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = \
						 ProtocolIE_ID_id_Cells_to_be_Activated_List ;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
							    F1SetupResponseIEs__value_PR_Cells_to_be_Activated_List;
   cellToActivate = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.\
		    Cells_to_be_Activated_List;
   cellCnt=1;
   cellToActivate->list.count = cellCnt;
   cellToActivate->list.size = \
			       cellCnt*sizeof(struct Cells_to_be_Activated_List_ItemIEs  *);
   CU_ALLOC(cellToActivate->list.array,\
	 sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
   if(cellToActivate->list.array == NULLP)
   {
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx],\
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
   {
      CU_ALLOC(cellToActivate->list.array[ieIdx],sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      if(cellToActivate->list.array[ieIdx] == NULLP)
      {
	 CU_FREE(cellToActivate->list.array,\
	       sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
	 CU_FREE(cuName->buf, sizeof(cuName->size));
	 for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
	 {
	    CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
		  sizeof(F1SetupResponseIEs_t));
	 }
	 CU_FREE(f1SetupRsp->protocolIEs.list.array, \
	       elementCnt * sizeof(F1SetupResponseIEs_t *));
	 CU_FREE(f1apMsg->choice.successfulOutcome, \
	       sizeof(SuccessfulOutcome_t));
	 CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
	 return RFAILED;
      }
   }
   cellToActivate->list.array[0]->id = \
				       ProtocolIE_ID_id_Cells_to_be_Activated_List_Item;
   cellToActivate->list.array[0]->criticality = Criticality_ignore;
   cellToActivate->list.array[0]->value.present = \
						  Cells_to_be_Activated_List_ItemIEs__value_PR_Cells_to_be_Activated_List_Item;
   cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
      nRCGI.pLMN_Identity.size = 3*sizeof(uint8_t);
   CU_ALLOC(cellToActivate->list.array[0]->\
	 value.choice.Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf,\
	 3*sizeof(uint8_t));
   if(cellToActivate->list.array[0]->value.choice.\
	 Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf == NULLP)
   {

      for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
      {
	 CU_FREE(cellToActivate->list.array[ieIdx],\
	       sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
	    sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
	    sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   buildPlmnId(cuCfgParams.plmn , cellToActivate->list.array[0]->value.choice.\
	 Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf);
   cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
      nRCGI.nRCellIdentity.size = 5;
   CU_ALLOC(cellToActivate->list.array[0]->value.choice.\
	 Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf,\
	 5*sizeof(uint8_t));
   if(cellToActivate->list.array[0]->value.choice.\
	 Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf == NULLP)
   {
      CU_FREE(cellToActivate->list.array[0]->\
	    value.choice.Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf,\
	    3*sizeof(uint8_t));
      for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
      {
	 CU_FREE(cellToActivate->list.array[ieIdx],\
	       sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
	    sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
	    sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   BuildNrCellId(&(cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity));
   /* RRC Version */
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = \
						 ProtocolIE_ID_id_GNB_CU_RRC_Version;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
							    F1SetupResponseIEs__value_PR_RRC_Version;
   rrcVer = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.RRC_Version;
   rrcVer->latest_RRC_Version.size = RRC_SIZE; 

   CU_ALLOC(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
   if(rrcVer->latest_RRC_Version.buf == NULLP)
   {  
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; idx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   /* Need to check RRC Version */
   rrcVer->latest_RRC_Version.buf[0] = cuCfgParams.rrcVersion.rrcVer; 
   rrcVer->latest_RRC_Version.bits_unused = 5; //TODO: pick from cuCfgParam. If not present, add it
   CU_ALLOC(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
   if(rrcVer->iE_Extensions == NULLP)
   {
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.count = 1;
   rrcVer->iE_Extensions->list.size = sizeof(struct RRC_Version_ExtIEs *);
   CU_ALLOC(rrcVer->iE_Extensions->list.array,\
	 sizeof(struct RRC_Version_ExtIEs *));
   if(rrcVer->iE_Extensions->list.array == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions,\
	    sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   CU_ALLOC(rrcVer->iE_Extensions->list.array[0],\
	 sizeof(struct RRC_Version_ExtIEs));
   if(rrcVer->iE_Extensions->list.array[0] == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array,\
	    sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
	    sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.array[0]->id = \
					      ProtocolIE_ID_id_latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->criticality = Criticality_reject;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.present = \
								  RRC_Version_ExtIEs__extensionValue_PR_Latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.size = 3*sizeof(uint8_t);
   CU_ALLOC(rrcVer->iE_Extensions->list.\
	 array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf,\
	 3*sizeof(uint8_t));
   if(rrcVer->iE_Extensions->list.\
	 array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array[0],\
	    sizeof(struct RRC_Version_ExtIEs));
      CU_FREE(rrcVer->iE_Extensions->list.array,\
	    sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
	    sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[0] = 0;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[1] = 5;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[2] = 15;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
   CU_FREE(cuName->buf, sizeof(cuName->size));
   for(idx=0; idx<elementCnt; idx++)
   {
      CU_FREE(f1SetupRsp->protocolIEs.list.array[idx], sizeof(F1SetupResponseIEs_t));
   }             
   CU_FREE(f1SetupRsp->protocolIEs.list.array, elementCnt * sizeof(F1SetupResponseIEs_t *));
   CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));

   /* Check encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG("\nF1AP : Could not encode F1SetupResponse structure (at %s)\n",\
	    encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;   
   } 
   else 
   {
      DU_LOG("\nF1AP : Created APER encoded buffer for F1SetupResponse\n");
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x",encBuf[i]);
      } 
   }

   /* Sending msg */
   if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
   {
      DU_LOG("\nF1AP : Sending F1 Setup Response failed");      
      return RFAILED;
   }

   return ROK;
}/* End of BuildAndSendF1SetupRsp */

/*******************************************************************
 *
 * @brief Builds and sends the DUUpdateAcknowledge
 *
 * @details
 *
 *    Function : BuildAndSendDUUpdateAck
 *
 *    Functionality: Constructs the DU Update Acknowledge message and sends
 *                   it to the DU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildAndSendDUUpdateAck()
{
   uint8_t   idx;
   uint8_t   elementCnt;
   F1AP_PDU_t *f1apMsg = NULL;
   GNBDUConfigurationUpdateAcknowledge_t *gNBDuCfgAck;
   asn_enc_rval_t enRetVal; /* Encoder return value */

   DU_LOG("\nF1AP : Building GNB-DU Config Update Ack\n");

   /* Allocate the memory for F1SetupRequest_t */
   CU_ALLOC(f1apMsg, (Size)sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }

   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_gNBDUConfigurationUpdate;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge;
   gNBDuCfgAck = &f1apMsg->choice.successfulOutcome->value.choice.GNBDUConfigurationUpdateAcknowledge;

   elementCnt = 1;
   gNBDuCfgAck->protocolIEs.list.count = elementCnt;
   gNBDuCfgAck->protocolIEs.list.size = elementCnt*sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t);

   /* Initialize the F1Setup members */
   CU_ALLOC(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   if(gNBDuCfgAck->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for DuUpdateAcknowledgeIEs failed");
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(gNBDuCfgAck->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
      if(gNBDuCfgAck->protocolIEs.list.array[idx] == NULLP)
      {
	 CU_FREE(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
	 CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
	 CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
	 return RFAILED;
      }
   }

   /*TransactionID*/ 
   idx = 0;
   gNBDuCfgAck->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID ;
   gNBDuCfgAck->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   gNBDuCfgAck->protocolIEs.list.array[idx]->value.present = GNBDUConfigurationUpdateAcknowledgeIEs__value_PR_TransactionID;
   gNBDuCfgAck->protocolIEs.list.array[idx]->value.choice.TransactionID = TRANS_ID;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   enRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   for(idx=0; idx<elementCnt; idx++)
   {
      CU_FREE(gNBDuCfgAck->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
   }
   CU_FREE(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));

   /* Checking encode results */
   if(enRetVal.encoded == ENCODE_FAIL) 
   {
      DU_LOG("\nF1AP : Could not encode DUConfigUpdateAcknowledge structure (at %s)",enRetVal.failed_type ? enRetVal.failed_type->name : "unknown");
      return RFAILED; 
   } 
   else 
   {
      DU_LOG("\nF1AP : Created APER encoded buffer for DuConfigUpdateAcknowledge\n");  
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x",encBuf[i]);
      } 
   }

   /* Sending msg */
   if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
   {
      DU_LOG("\nF1AP : Sending GNB-DU Config Update Ack failed");
      return RFAILED;
   }

   return ROK;

}/* End of BuildAndSendDUUpdateAck*/
/*******************************************************************
*
* @brief deallocating the memory of  F1reset msg
*
* @details
*
*    Function : FreeF1ResetReq
*
*    Functionality :
*         - freeing memory of F1reset request msg
*
* @params[in]
* @return void
*
*
* ****************************************************************/
void FreeF1ResetReq(F1AP_PDU_t *f1apMsg)
{
    uint8_t idx = 0;
    Reset_t *f1ResetMsg = NULLP;

    if(f1apMsg)
    {
       if(f1apMsg->choice.initiatingMessage)
       {
          f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;
          if(f1ResetMsg->protocolIEs.list.array)
          {
             for(idx=0 ;idx < f1ResetMsg->protocolIEs.list.count ; idx++)
             {
                 if(f1ResetMsg->protocolIEs.list.array[idx])
                 {
                    CU_FREE(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
                 }
             }
             CU_FREE(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
          }
          CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
       }
       CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
    }
}
/*******************************************************************
*
* @brief build ansld ans send f1reset msg 
*
* @details
*
*    Function : BuildAndSendF1ResetReq
*
*    Functionality: build and send f1reset msg 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildAndSendF1ResetReq()
{
    uint8_t          elementCnt=0;
    uint8_t          idx=0;
    uint8_t          ret= RFAILED;
    Reset_t          *f1ResetMsg = NULLP;
    F1AP_PDU_t       *f1apMsg = NULLP;
    asn_enc_rval_t   encRetVal;
    DU_LOG("\nF1AP : Building F1 Reset request \n");
    do
    {
       CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
       if(f1apMsg == NULLP)
       {
          DU_LOG("\nF1AP : Memory allocation for the BuildAndSendF1ResetReq's F1AP-PDU failed"); 
	  break;
       }
       f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
       CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
       if(f1apMsg->choice.initiatingMessage == NULLP)
       {
          DU_LOG("\nF1AP : Memory allocation for BuildAndSendF1ResetReq failed");
          break;
       }
       f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_Reset;
       f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
       f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_Reset;

       f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

       elementCnt = 3;
       f1ResetMsg->protocolIEs.list.count = elementCnt;
       f1ResetMsg->protocolIEs.list.size = elementCnt * sizeof(ResetIEs_t *);

       /* Initialize the F1Reset members */
       CU_ALLOC(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
       if(f1ResetMsg->protocolIEs.list.array == NULLP)
       {
          DU_LOG("\nF1AP : Memory allocation failed for BuildAndSendF1ResetReq");
	  break;
       }
       for(idx=0; idx<elementCnt; idx++)
       {
           CU_ALLOC(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
           if(f1ResetMsg->protocolIEs.list.array[idx] == NULLP)
           {
	      DU_LOG("\nF1AP : Memory allocation failed for BuildAndSendF1ResetReq msg array");
              break;
           }
       }

       /*TransactionID*/
       idx=0;
       f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
       f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_reject;
       f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_TransactionID;
       f1ResetMsg->protocolIEs.list.array[idx]->value.choice.TransactionID = 1;

       /*Cause*/
       idx++;
       f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_Cause;
       f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
       f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_Cause;
       f1ResetMsg->protocolIEs.list.array[idx]->value.choice.Cause.present = Cause_PR_radioNetwork;
       f1ResetMsg->protocolIEs.list.array[idx]->value.choice.Cause.choice.radioNetwork = CauseRadioNetwork_action_desirable_for_radio_reasons;

       /*Reset Type*/
       idx++;
       f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_ResetType;
       f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_reject;
       f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_ResetType;
       f1ResetMsg->protocolIEs.list.array[idx]->value.choice.ResetType.present = ResetType_PR_f1_Interface;
       f1ResetMsg->protocolIEs.list.array[idx]->value.choice.ResetType.choice.f1_Interface = ResetAll_reset_all;

       xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

       /* Encode the F1SetupRequest type as APER */
       memset(encBuf, 0, ENC_BUF_MAX_LEN);
       encBufSize = 0;
       encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
                   encBuf);

       /* Encode results */
       if(encRetVal.encoded == ENCODE_FAIL)
       {
          DU_LOG("\nF1AP : Could not encode F1Reset structure (at %s)\n",\
                      encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
          break;
       }
       else
       {
          DU_LOG("\nF1AP : Created APER encoded buffer for F1Reset\n");
          for(idx=0; idx< encBufSize; idx++)
          {
             printf("%x",encBuf[idx]);
          }
       }

       if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
       {
          DU_LOG("\nF1AP : Sending F1 Reset request failed");
          break;
       }

       ret = ROK;
       break;
    }while(true);

    FreeF1ResetReq(f1apMsg);
    return ret;
 }

/*******************************************************************
 *
 * @brief Fills Radio Bearer Config 
 *
 * @details
 *
 *    Function : fillSrbCfg
 *
 *    Functionality: Fills Radio Bearer Config
 *
 * @params[in] SRB_ToAddModList *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillSrbCfg(uint8_t srbId, SRB_ToAddModList_t *bearerCfg)
{
   uint8_t elementCnt;
   uint8_t idx, ieId;
   if(bearerCfg != NULLP)
   {
      elementCnt = 1;
      bearerCfg->list.count = elementCnt;
      bearerCfg->list.size =\
           elementCnt * sizeof(SRB_ToAddMod_t *);
      CU_ALLOC(bearerCfg->list.array, bearerCfg->list.size);
      if(bearerCfg->list.array != NULLP)
      {
         for(idx = 0; idx < elementCnt; idx++)
         {
            CU_ALLOC(bearerCfg->list.array[idx], sizeof(SRB_ToAddMod_t));
            if(bearerCfg->list.array[idx] == NULLP)
            {
               for(ieId = 0; ieId < idx; ieId++)
               {
                  CU_FREE(bearerCfg->list.array[ieId], sizeof(SRB_ToAddMod_t));
               }
               CU_FREE(bearerCfg->list.array, bearerCfg->list.size);
               return RFAILED;
            }
         }
      }
      else
      {
         return RFAILED;
      }
      idx = 0;
      bearerCfg->list.array[idx]->srb_Identity = srbId;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Master CellGroup Info 
 *
 * @details
 *
 *    Function : fillMasterCellGroup
 *
 *    Functionality: Fills Master Cell Group IE
 *
 * @params[in] RRCSetup_IEs_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillMasterCellGroup(OCTET_STRING_t *masterCellGroup)
{
   uint8_t ret = ROK;
   masterCellGroup->buf = NULLP;
   if(f1apMsgDb.duToCuContainer.buf)
   {
      masterCellGroup->size = f1apMsgDb.duToCuContainer.size;
      CU_ALLOC(masterCellGroup->buf, masterCellGroup->size);
      if(masterCellGroup->buf != NULLP)
      {
	 memcpy(masterCellGroup->buf, f1apMsgDb.duToCuContainer.buf,\
	       masterCellGroup->size);
      }
      else
      {
	 ret = RFAILED;
      }
   }
   else
   {
      ret =  RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills RRC setup IE 
 *
 * @details
 *
 *    Function : fillRRCSetupIE
 *
 *    Functionality: Fills RRC Setup IE
 *
 * @params[in] RRCSetup_IEs_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillRRCSetupIE(RRCSetup_IEs_t *rrcSetupIE)
{
   uint8_t ret = ROK;
   if(rrcSetupIE)
   {
      CU_ALLOC(rrcSetupIE->radioBearerConfig.srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      if(rrcSetupIE->radioBearerConfig.srb_ToAddModList != NULLP)
      {
	 ret = fillSrbCfg(SRB1, rrcSetupIE->radioBearerConfig.srb_ToAddModList);
      }		
      if(ret == ROK)
      {
	 ret = fillMasterCellGroup(&rrcSetupIE->masterCellGroup);
      }
      else
      {
	 CU_FREE(rrcSetupIE->radioBearerConfig.srb_ToAddModList, sizeof(SRB_ToAddModList_t));
	 ret = RFAILED;
      }
   }
   return ret;
}
/*******************************************************************
 *
 * @brief Fills DL DCCCH Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillDlCcchRrcMsg
 *
 *    Functionality: Fills DL DCCCH Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCContainer_t *rrcContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlCcchRrcMsg(RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;
   uint16_t idx2;
   DL_CCCH_Message_t dl_CCCH_Msg;
   asn_enc_rval_t        encRetVal;

   if(rrcContainer != NULLP)
   {
      dl_CCCH_Msg.message.present = DL_CCCH_MessageType_PR_c1;

      CU_ALLOC(dl_CCCH_Msg.message.choice.c1 , sizeof(DL_CCCH_MessageType_t));
      if(dl_CCCH_Msg.message.choice.c1 != NULLP)
      {
	 dl_CCCH_Msg.message.choice.c1->present = DL_CCCH_MessageType__c1_PR_rrcSetup;
	 CU_ALLOC(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup, sizeof(RRCSetup_t));
	 if(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup != NULLP)
	 {
	    dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->rrc_TransactionIdentifier = 0;
	    dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->criticalExtensions.\
	       present = RRCSetup__criticalExtensions_PR_rrcSetup;
	    /* Fill RRC Setup IE */
	    CU_ALLOC(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->\
		  criticalExtensions.choice.rrcSetup, sizeof(RRCSetup_IEs_t));
	    if(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->\
		  criticalExtensions.choice.rrcSetup != NULLP)
	    {
	       ret = fillRRCSetupIE(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->\
		     criticalExtensions.choice.rrcSetup);

	       if(ret == ROK)
	       {
		  /* encode DL-CCCH message into RRC Container */
		  xer_fprint(stdout, &asn_DEF_DL_CCCH_MessageType, &dl_CCCH_Msg);
		  memset(encBuf, 0, ENC_BUF_MAX_LEN);
		  encBufSize = 0;
		  encRetVal = aper_encode(&asn_DEF_DL_CCCH_MessageType, 0, &dl_CCCH_Msg, PrepFinalEncBuf, encBuf);
		  /* Encode results */
		  if(encRetVal.encoded == ENCODE_FAIL)
		  {
		     DU_LOG( "\n F1AP : Could not encode RRCContainer for DL-CCCH Msg(at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		     return RFAILED;
		  }
		  else
		  {
		     DU_LOG("\n F1AP : Created APER encoded buffer for RRCContainer for DL-CCCH Msg\n");
		     for(int i = 0; i< encBufSize; i++)
		     {
			printf("%x",encBuf[i]);
		     }
		     rrcContainer->size = encBufSize;
		     CU_ALLOC(rrcContainer->buf, rrcContainer->size);
		     if(rrcContainer->buf != NULLP)
		     {
			memset(rrcContainer->buf, 0, encBufSize);
			for(idx2 = 0; idx2 < encBufSize; idx2++)
			{
			   rrcContainer->buf[idx2] =	encBuf[idx2];
			}
		     }
		  }
	       }
	       else
	       {
		  ret = RFAILED;
	       }
	    }
	    else
	    {
	       DU_LOG("\nF1AP: Memory Alloc failed for RRC Setup Msg at fillDlCcchRrcMsg()");
	       ret = RFAILED;
	    }
	 }
	 else
	 {
	    DU_LOG("\nF1AP: Memory Alloc failed for RRC Msg at fillDlCcchRrcMsg()");
	    ret = RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nF1AP: Memory Alloc failed for DL Ccch Msg choice at fillDlCcchRrcMsg()");
	 ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nF1AP: RRC Container is NULLP at fillDlCcchRrcMsg()");
      ret = RFAILED;
   }
}

uint8_t fillQosFlowsToAdd(struct SDAP_Config__mappedQoS_FlowsToAdd *qosFlow)
{
   uint8_t idx, ied, elementCnt;

   elementCnt = 1;
   qosFlow->list.count = elementCnt;
   qosFlow->list.size  = elementCnt * sizeof(QFI_t *);
   CU_ALLOC(qosFlow->list.array, qosFlow->list.size);
   if(qosFlow->list.array != NULLP)
   {
      for(idx = 0; idx < elementCnt; idx++)
      {
         CU_ALLOC(qosFlow->list.array[idx], sizeof(QFI_t));
         if(qosFlow->list.array[idx] == NULLP)
         {
            for(ied = 0; ied < idx; ied++)
            {
               CU_FREE(qosFlow->list.array[idx], sizeof(QFI_t));
            }
            CU_FREE(qosFlow->list.array, qosFlow->list.size);
            return RFAILED;
         }
      }
   }
   idx = 0;
   *qosFlow->list.array[idx] = 9;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills CN Assoc for Drb to Add/Mod List
 *
 * @details
 *
 *    Function : fillCnAssoc
 *
 *    Functionality: Fills CN Assoc for Drb to Add/Mod List
 *
 * @params[in] struct DRB_ToAddMod__cnAssociation *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillCnAssoc(struct DRB_ToAddMod__cnAssociation *cnAssoc)
{
   uint8_t ret = ROK;

   cnAssoc->present = DRB_ToAddMod__cnAssociation_PR_sdap_Config;
   if(cnAssoc->present == DRB_ToAddMod__cnAssociation_PR_eps_BearerIdentity)
   {
      cnAssoc->choice.eps_BearerIdentity = 5;
   }
   if(cnAssoc->present == DRB_ToAddMod__cnAssociation_PR_sdap_Config)
   {
      CU_ALLOC(cnAssoc->choice.sdap_Config, sizeof(SDAP_Config_t));
      if(cnAssoc->choice.sdap_Config)
      {
         cnAssoc->choice.sdap_Config->pdu_Session = 5;
         cnAssoc->choice.sdap_Config->sdap_HeaderDL = 0;
         cnAssoc->choice.sdap_Config->sdap_HeaderUL = 0;
         cnAssoc->choice.sdap_Config->defaultDRB = true;
         cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd = NULLP;
         cnAssoc->choice.sdap_Config->mappedQoS_FlowsToRelease = NULLP;
         CU_ALLOC(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd, \
            sizeof(struct SDAP_Config__mappedQoS_FlowsToAdd));
         if(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd)
         {
            ret = fillQosFlowsToAdd(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd);
         }
         else
         {
            DU_LOG("\nF1AP: Memory alloc failed at mappedQoS_FlowsToAdd in fillCnAssoc()");
            CU_FREE(cnAssoc->choice.sdap_Config, sizeof(SDAP_Config_t));
            ret = RFAILED;
         }
      }
      else
      {
         DU_LOG("\nF1AP: Mem alloc failed at fillCnAssoc()");
         ret = RFAILED;
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills Radio Bearer Config for Drb 
 *
 * @details
 *
 *    Function : fillDrbCfg
 *
 *    Functionality: Fills Radio Bearer Config for Drb
 *
 * @params[in] drbId, DRB_ToAddModList *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillDrbCfg(uint8_t drbId, DRB_ToAddModList_t *drbCfg)
{
   uint8_t idx, ied, ret, elementCnt;
   
   ret = ROK;
   if(drbCfg != NULLP)
   {
      elementCnt = 1;
      drbCfg->list.count = elementCnt;
      drbCfg->list.size =\
           elementCnt * sizeof(DRB_ToAddMod_t *);
      CU_ALLOC(drbCfg->list.array, drbCfg->list.size);
      if(drbCfg->list.array != NULLP)
      {
         for(idx = 0; idx < elementCnt; idx++)
         {
            CU_ALLOC(drbCfg->list.array[idx], sizeof(DRB_ToAddMod_t));
            if(drbCfg->list.array[idx] == NULLP)
            {
               for(ied = 0; ied < idx; ied++)
               {
                  CU_FREE(drbCfg->list.array[idx], sizeof(DRB_ToAddMod_t));
               }
               CU_FREE(drbCfg->list.array, drbCfg->list.size);
               return RFAILED;
            }
         }
      }
      else
      {
         return RFAILED;
      }
      idx = 0;
      /* CN ASSOCIATION */
      CU_ALLOC(drbCfg->list.array[idx]->cnAssociation, sizeof(struct DRB_ToAddMod__cnAssociation));
      if(drbCfg->list.array[idx]->cnAssociation)
      {
         ret = fillCnAssoc(drbCfg->list.array[idx]->cnAssociation);
      }
      /* DRB */
      drbCfg->list.array[idx]->drb_Identity = drbId;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills RRC Reconfig Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillRrcReconfigIE
 *
 *    Functionality: Fills RRC Reconfig Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCReconfiguration_IEs_t* rrcReconfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillRrcReconfigIE(RRCReconfiguration_IEs_t *rrcReconfigMsg)
{
   uint8_t ret = ROK;
   CU_ALLOC(rrcReconfigMsg->radioBearerConfig, sizeof(RadioBearerConfig_t));
   if(rrcReconfigMsg->radioBearerConfig)
   {
      CU_ALLOC(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      if(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList != NULLP)
      {
         ret = fillSrbCfg(SRB2, rrcReconfigMsg->radioBearerConfig->srb_ToAddModList);
	 
      }
      if(ret == ROK)
      {
         CU_ALLOC(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
         if(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList != NULLP)
	 {
            ret = fillDrbCfg(DRB1, rrcReconfigMsg->radioBearerConfig->drb_ToAddModList);
	    if(ret == RFAILED)
	    {
               DU_LOG("\n F1AP: Failed to fill DrbCfg at fillRrcReconfigIE()");
	       CU_FREE(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
               CU_FREE(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
	    }
	 }
      }
      else
      {
         DU_LOG("\n F1AP: memory Alloc failed at fillRrcReconfigIE()");
	 CU_FREE(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      }
   }

   return ret;
}
/*******************************************************************
 *
 * @brief Fills DL DCCH Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillDlDcchRrcMsg
 *
 *    Functionality: Fills DL DCCH Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCContainer_t *rrcContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlDcchRrcMsg(RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;
   uint16_t idx2;
   DL_DCCH_Message_t dl_DCCH_Msg;
   memset(&dl_DCCH_Msg, 0, sizeof(DL_DCCH_Message_t));
   asn_enc_rval_t        encRetVal;

   if(rrcContainer != NULLP)
   {
      dl_DCCH_Msg.message.present = DL_DCCH_MessageType_PR_c1;

      CU_ALLOC(dl_DCCH_Msg.message.choice.c1 , sizeof(DL_DCCH_MessageType_t));
      if(dl_DCCH_Msg.message.choice.c1 != NULLP)
      {
	 dl_DCCH_Msg.message.choice.c1->present = DL_DCCH_MessageType__c1_PR_rrcReconfiguration;
	 CU_ALLOC(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration, sizeof(RRCReconfiguration_t));
	 if(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration != NULLP)
	 {
	    dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->rrc_TransactionIdentifier = 0;
	    dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->criticalExtensions.\
	       present = RRCReconfiguration__criticalExtensions_PR_rrcReconfiguration;
	    /* Fill RRC Reconfig IE */
	    CU_ALLOC(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->\
		  criticalExtensions.choice.rrcReconfiguration, sizeof(RRCReconfiguration_IEs_t));
	    if(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->\
		  criticalExtensions.choice.rrcReconfiguration != NULLP)
	    {
	       ret = fillRrcReconfigIE(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->\
		     criticalExtensions.choice.rrcReconfiguration);

	       if(ret == ROK)
	       {
		  /* encode DL-DCCH message into RRC Container */
		  xer_fprint(stdout, &asn_DEF_DL_DCCH_MessageType, &dl_DCCH_Msg);
		  memset(encBuf, 0, ENC_BUF_MAX_LEN);
		  encBufSize = 0;
		  encRetVal = aper_encode(&asn_DEF_DL_DCCH_MessageType, 0, &dl_DCCH_Msg, PrepFinalEncBuf, encBuf);
		  /* Encode results */
		  if(encRetVal.encoded == ENCODE_FAIL)
		  {
		     DU_LOG( "\n F1AP : Could not encode RRCContainer for DL-DCCH Msg (at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		     return RFAILED;
		  }
		  else
		  {
		     DU_LOG("\n F1AP : Created APER encoded buffer for RRCContainer for DL-DCCH Msg\n");
		     for(int i = 0; i< encBufSize; i++)
		     {
			printf("%x",encBuf[i]);
		     }
		     rrcContainer->size = encBufSize;
		     CU_ALLOC(rrcContainer->buf, rrcContainer->size);
		     if(rrcContainer->buf != NULLP)
		     {
			memset(rrcContainer->buf, 0, encBufSize);
			for(idx2 = 0; idx2 < encBufSize; idx2++)
			{
			   rrcContainer->buf[idx2] =	encBuf[idx2];
			}
		     }
		  }
	       }
	       else
	       {
	          DU_LOG("\nF1AP: Failed to fill RrcReconfig IE at fillDlDcchRrcMsg()");
	       }
	    }
	    else
	    {
	       DU_LOG("\nF1AP: Memory Alloc failed for RRC Reconfig at fillDlDcchRrcMsg()");
	       ret = RFAILED;
	    }
	 }
	 else
	 {
	    DU_LOG("\nF1AP: Memory Alloc failed for RRC Msg at fillDlDcchRrcMsg()");
	    ret = RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nF1AP: Memory Alloc failed for DL Dcch Msg choice at fillDlDcchRrcMsg()");
	 ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nF1AP: RRC Container is NULLP at fillDlDcchRrcMsg()");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds RRC Container IE required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : BuildDLRRCContainer
 *
 *    Functionality: Builds RRC Container IE required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t	BuildDLRRCContainer(uint8_t rrcMsgType, RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;

   if(rrcMsgType == RRC_SETUP)
   { 
      ret = fillDlCcchRrcMsg(rrcContainer);
      if(ret == RFAILED)
         DU_LOG("\n F1AP: Failed to fill DL-CCCH Msg at BuildDLRRCContainer()");
   }
   else if(rrcMsgType == RRC_RECONFIG)
   {
      ret = fillDlDcchRrcMsg(rrcContainer);
      if(ret == RFAILED)
         DU_LOG("\n F1AP: Failed at fill DL-DCCH Msg at BuildDLRRCContainer()");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds and sends the DLRRCMessageTransfer 
 *
 * @details
 *
 *    Function : BuildAndSendDLRRCMessageTransfer
 *
 *    Functionality: Constructs the DL RRC Message Transfer and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendDLRRCMessageTransfer(uint8_t rrcMsgType)
{
   uint8_t   elementCnt = 0;
   uint8_t  ieId;
   uint8_t  idx;
   F1AP_PDU_t  *f1apMsg = NULLP;
   DLRRCMessageTransfer_t  *dlRRCMsg = NULLP;
   asn_enc_rval_t   encRetVal;        /* Encoder return value */

   DU_LOG("\n F1AP : Building DL RRC Message Transfer Message\n");

   CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }

   f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
   CU_ALLOC(f1apMsg->choice.initiatingMessage,
	 sizeof(InitiatingMessage_t));
   if(f1apMsg->choice.initiatingMessage == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for  F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_DLRRCMessageTransfer;
   f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
   f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_DLRRCMessageTransfer;
   dlRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;

   elementCnt = 4;
   dlRRCMsg->protocolIEs.list.count = elementCnt;
   dlRRCMsg->protocolIEs.list.size = elementCnt * sizeof(DLRRCMessageTransferIEs_t *);

   /* Initialize the F1Setup members */
   CU_ALLOC(dlRRCMsg->protocolIEs.list.array, dlRRCMsg->protocolIEs.list.size);
   if(dlRRCMsg->protocolIEs.list.array == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for DL RRC MessageTransferIEs failed");
      CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(dlRRCMsg->protocolIEs.list.array[idx], sizeof(DLRRCMessageTransferIEs_t));
      if(dlRRCMsg->protocolIEs.list.array[idx] == NULLP)
      {
	 for(ieId=0; ieId<idx; ieId++)
	 {
	    CU_FREE(dlRRCMsg->protocolIEs.list.array[ieId],\
		  sizeof(DLRRCMessageTransferIEs_t));
	 }
	 CU_FREE(dlRRCMsg->protocolIEs.list.array,\
	       dlRRCMsg->protocolIEs.list.size);
	 CU_FREE(f1apMsg->choice.initiatingMessage,\
	       sizeof(InitiatingMessage_t));
	 CU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
	 return RFAILED;
      }
   }

   /* GNB CU UE F1AP ID */
   idx = 0;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  =   Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
							  DLRRCMessageTransferIEs__value_PR_GNB_CU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = CU_ID;

   /* GNB DU UE F1AP ID */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = \
						ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  =   Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
							  DLRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = DU_ID;

   /* SRBID */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_SRBID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  =   Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
							  DLRRCMessageTransferIEs__value_PR_SRBID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.SRBID = SRB1;

   /* RRCContainer */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_RRCContainer;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality   = Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
							  DLRRCMessageTransferIEs__value_PR_RRCContainer;
   BuildDLRRCContainer(rrcMsgType, &dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer);

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as APER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	 encBuf);
   /* Encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG( "\n F1AP : Could not encode DL RRC Message Transfer structure (at %s)\n",\
	    encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;
   }
   else
   {
      DU_LOG("\n F1AP : Created APER encoded buffer for DL RRC Message transfer\n");
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x",encBuf[i]);
      }
   }

   /* Sending  msg  */
   if(SendF1APMsg(CU_APP_MEM_REG,CU_POOL)  !=  ROK)
   {
      DU_LOG("\n F1AP : Sending  DL RRC Message Transfer Failed");
      return RFAILED;
   }

   return ROK;
}/* End of BuildAndSendDLRRCMessageTransfer */

/*******************************************************************
 *
 * @brief Function to set the Dl RRC Msg Type
 *
 * @details
 *
 *    Function : setDlRRCMsgType
 *
 *    Functionality: Constructs the UE Setup Response and sends
 *                   it to the DU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t setDlRRCMsgType()
{
   uint8_t rrcMsgType = 0;
   switch(f1apMsgDb.dlRrcMsgCount)
   {
      case RRC_SETUP:
        rrcMsgType = RRC_SETUP;
        break;
      case RRC_SECURITY_MODE_COMMAND:
        rrcMsgType = RRC_SECURITY_MODE_COMMAND;
        break;
      case RRC_RECONFIG:
        rrcMsgType = RRC_RECONFIG;
        break;
      default:
        break;
   }
   return rrcMsgType;   
}

/*******************************************************************
 *
 * @brief Function to build Initial UL RRC Message
 *
 * @details
 *
 *    Function : procInitULRRCMsg
 *
 *    Functionality: Function to build Initial UL RRC Message
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procInitULRRCMsg(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx, rrcMsgType;
   uint8_t ret =ROK;
   InitialULRRCMessageTransfer_t *initULRRCMsg = NULLP;
   DU_LOG("\n filling the required values in DB in procInitULRRCMsg");

   initULRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.InitialULRRCMessageTransfer;

   for(idx=0; idx < initULRRCMsg->protocolIEs.list.count; idx++)
   {
      switch(initULRRCMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    break;
	 case ProtocolIE_ID_id_NRCGI:
	    break;
	 case ProtocolIE_ID_id_C_RNTI:
	    break;
	 case ProtocolIE_ID_id_RRCContainer:
	    break;
	 case ProtocolIE_ID_id_DUtoCURRCContainer:
	    {
	       if((initULRRCMsg->protocolIEs.list.array[idx]->value.choice\
			.DUtoCURRCContainer.size > 0) && (initULRRCMsg->protocolIEs\
			   .list.array[idx]->value.choice.DUtoCURRCContainer.buf != NULLP))
	       {
		  DU_LOG("\n Received Du to Cu RRC Container ");
		  f1apMsgDb.duToCuContainer.size = initULRRCMsg->protocolIEs\
						   .list.array[idx]->value.choice.DUtoCURRCContainer.size;
		  CU_ALLOC(f1apMsgDb.duToCuContainer.buf, \
			f1apMsgDb.duToCuContainer.size);
		  if(f1apMsgDb.duToCuContainer.buf != NULLP)
		  { 
		     memcpy(f1apMsgDb.duToCuContainer.buf, initULRRCMsg->protocolIEs\
			   .list.array[idx]->value.choice.DUtoCURRCContainer.buf, f1apMsgDb\
			   .duToCuContainer.size);
		  }
	       }
	       else
	       {
		  DU_LOG("\n Failed to receive Du to Cu RRC Container ");
		  ret = RFAILED;
	       }
	       break;
	    }
	 default:
	    DU_LOG("\n Invalid Event %ld", initULRRCMsg->protocolIEs.list.array[idx]->id);
	    break;
      }
   }
   if(ret == ROK)
   {
      f1apMsgDb.dlRrcMsgCount++;
      rrcMsgType = setDlRRCMsgType();
      ret = BuildAndSendDLRRCMessageTransfer(rrcMsgType);
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds Nrcgi 
 *
 * @details
 *
 *    Function : BuildNrcgi
 *
 *    Functionality: Building the PLMN ID and NR Cell id
 *
 * @params[in] NRCGI_t *nrcgi
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildNrcgi(NRCGI_t *nrcgi)
{
   uint8_t ret;
   uint8_t unused = 4;
   uint8_t byteSize = 5;
   uint8_t val = 16;
   /* Allocate Buffer Memory */
   nrcgi->pLMN_Identity.size = 3 * sizeof(uint8_t);
   CU_ALLOC(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   if(nrcgi->pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   ret = buildPlmnId(cuCfgParams.plmn , nrcgi->pLMN_Identity.buf);

   if(ret != ROK)
   {
      return RFAILED;
   }
   nrcgi->nRCellIdentity.size = byteSize * sizeof(uint8_t);
   CU_ALLOC(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
   if(nrcgi->nRCellIdentity.buf == NULLP)
   {
      return RFAILED;
   }
   ret = fillBitString(&nrcgi->nRCellIdentity, unused, byteSize, val);
   if(ret != ROK)
   {
      return RFAILED;
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Special cell list for UE Setup Request 
 *
 * @details
 *
 *    Function : BuildSplCellList
 *
 *    Functionality: Constructs the Special Cell list for UESetReq
 *
 * @params[in] SCell_ToBeSetup_List_t *spCellLst
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSplCellList(SCell_ToBeSetup_List_t *spCellLst)
{
   uint8_t  cellCnt;
   uint8_t  idx;
   uint8_t  ret;
   cellCnt = 1;
   spCellLst->list.count = cellCnt;
   spCellLst->list.size = cellCnt * sizeof(SCell_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(spCellLst->list.array,spCellLst->list.size);
   if(spCellLst->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<cellCnt; idx++)
   {
      CU_ALLOC(spCellLst->list.array[idx],sizeof(SCell_ToBeSetup_ItemIEs_t));
      if(spCellLst->list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   idx = 0;
   spCellLst->list.array[idx]->id = ProtocolIE_ID_id_SCell_ToBeSetup_Item;
   spCellLst->list.array[idx]->criticality = Criticality_ignore;
   spCellLst->list.array[idx]->value.present =\
					      SCell_ToBeSetup_ItemIEs__value_PR_SCell_ToBeSetup_Item;
   /* Special Cell ID -NRCGI */
   ret = BuildNrcgi(&spCellLst->list.array[idx]->value.choice.SCell_ToBeSetup_Item.sCell_ID);
   if(ret != ROK)
   {
      return RFAILED;
   }
   /*Special Cell Index*/
   spCellLst->list.array[idx]->value.choice.SCell_ToBeSetup_Item.sCellIndex = 1;
   return ROK;	
}/* End of BuildSplCellList*/

/*******************************************************************
 *
 * @brief Builds SRBS to be setup 
 *
 * @details
 *
 *    Function : BuildSRBSetup
 *
 *    Functionality: Constructs the SRB's for UESetReq
 *
 * @params[in] SRBs_ToBeSetup_List_t *srbSet
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSRBSetup(SRBs_ToBeSetup_List_t *srbSet)
{
   uint8_t idx;
   uint8_t srbCnt;
   srbCnt = 1;
   srbSet->list.count = srbCnt;
   srbSet->list.size = srbCnt*sizeof(SRBs_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(srbSet->list.array,srbSet->list.size);
   if(srbSet->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<srbCnt; idx++)
   {
      CU_ALLOC(srbSet->list.array[idx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
      if(srbSet->list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   idx = 0;
   srbSet->list.array[idx]->id = ProtocolIE_ID_id_SRBs_ToBeSetup_Item;
   srbSet->list.array[idx]->criticality = Criticality_ignore;
   srbSet->list.array[idx]->value.present = \
      SRBs_ToBeSetup_ItemIEs__value_PR_SRBs_ToBeSetup_Item;
   srbSet->list.array[idx]->value.choice.SRBs_ToBeSetup_Item.sRBID = 2;
   return ROK;
}/* End of BuildSRBSetup*/

/*******************************************************************
 *
 * @brief Builds QOS Info for DRB Setum Item 
 *
 * @details
 *
 *    Function : BuildQOSInfo
 *
 *    Functionality: Constructs the QOS Info for DRB Setup Item
 *
 * @params[in] QoSInformation_t *qosinfo
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildQOSInfo(QoSFlowLevelQoSParameters_t *drbQos)
{
   /* NonDynamic5QIDescriptor */
   drbQos->qoS_Characteristics.present = QoS_Characteristics_PR_non_Dynamic_5QI;
   CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI == NULLP)
   {
      return RFAILED;
   }
   /*FiveQI*/
   drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI = 0;
   /*AveragingWindow*/
   CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
	 sizeof(AveragingWindow_t));
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow == \
	 NULLP)
   {
      return RFAILED;
   }
   *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow) = 0;
   /*MaxDataBurstVolume*/
   CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
	 sizeof(MaxDataBurstVolume_t));
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume == \
	 NULLP)
   {
      return RFAILED;
   }
   *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume) = 0;

   /*nRGRAN Allocation Retention Priority*/
   drbQos->nGRANallocationRetentionPriority.priorityLevel = PriorityLevel_highest;
   drbQos->nGRANallocationRetentionPriority.pre_emptionCapability = Pre_emptionCapability_may_trigger_pre_emption;
   drbQos->nGRANallocationRetentionPriority.pre_emptionVulnerability = Pre_emptionVulnerability_not_pre_emptable;

   /* TO DO: GBR_QoSFlowInformation */
   return ROK;
}/*End of BuildQOSInfo*/

/*******************************************************************
 *
 * @brief Builds SNSSAI  
 *
 * @details
 *
 *    Function : BuildSNSSAI
 *
 *    Functionality: Constructs the SNSSAI For DRB list
 *
 * @params[in] SNSSAI_t *snssai
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSNSSAI(SNSSAI_t *snssai)
{
   /*SNSSAI*/
   /*ssT*/
   snssai->sST.size = sizeof(uint8_t);
   CU_ALLOC(snssai->sST.buf,snssai->sST.size);
   if(snssai->sST.buf == NULLP)
   {
      return RFAILED;
   }
   snssai->sST.buf[0] = 3;
   /*sD*/
   CU_ALLOC(snssai->sD,sizeof(OCTET_STRING_t));
   if(snssai->sD == NULLP)
   {
      return RFAILED;
   }
   snssai->sD->size = 3*sizeof(uint8_t);
   CU_ALLOC(snssai->sD->buf,snssai->sD->size);
   if(snssai->sD->buf == NULLP)
   {
      return RFAILED;
   }
   snssai->sD->buf[0] = 3;
   snssai->sD->buf[1] = 6;
   snssai->sD->buf[2] = 9;
   return ROK;
}/*End of BuildSNSSAI*/

/*******************************************************************
 *
 * @brief Builds the flow map.  
 *
 * @details
 *
 *    Function : BuildFlowsMap
 *
 *    Functionality: Constructs the flowmap For DRB list
 *
 * @params[in] Flows_Mapped_To_DRB_List_t *flowMap
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildFlowsMap(Flows_Mapped_To_DRB_List_t *flowMap)
{
   uint8_t  ret;
   uint8_t  idx;
   uint8_t  flowCnt;
   flowCnt = 1;
   flowMap->list.count = flowCnt;
   flowMap->list.size = flowCnt * sizeof(Flows_Mapped_To_DRB_Item_t *);
   CU_ALLOC(flowMap->list.array,flowMap->list.size);
   if(flowMap->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<flowCnt; idx++)
   {
      CU_ALLOC(flowMap->list.array[idx],sizeof(Flows_Mapped_To_DRB_Item_t));
      if(flowMap->list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   idx = 0;
   flowMap->list.array[idx]->qoSFlowIdentifier = 0;
   ret = BuildQOSInfo(&flowMap->list.array[idx]->qoSFlowLevelQoSParameters);
   if(ret != ROK)
   {
      return RFAILED;
   }
   return ROK;
}/*End of BuildFlowsMap*/

/*******************************************************************
 *
 * @brief Builds the Uplink Tunnel Info  
 *
 * @details
 *
 *    Function : BuildULTnlInfo
 *
 *    Functionality: Constructs the UL TnlInfo For DRB list
 *
 * @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildULTnlInfo(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
   uint8_t idx;
   uint8_t ulCnt;
   ulCnt = 1;
   ulInfo->list.count = ulCnt;
   ulInfo->list.size = ulCnt * sizeof(ULUPTNLInformation_ToBeSetup_Item_t *);
   CU_ALLOC(ulInfo->list.array,ulInfo->list.size);
   if(ulInfo->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<ulCnt; idx++)
   {
      CU_ALLOC(ulInfo->list.array[idx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
      if(ulInfo->list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   idx = 0;
   ulInfo->list.array[idx]->uLUPTNLInformation.present = \
							 UPTransportLayerInformation_PR_gTPTunnel;
   /*GTP TUNNEL*/
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel,\
	 sizeof(GTPTunnel_t));
   if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel == NULLP)
   {
      return RFAILED;
   }
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.size	= 4*sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
	 transportLayerAddress.buf,ulInfo->list.array[idx]->\
	 uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
   if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
	 transportLayerAddress.buf == NULLP)
   {
      return RFAILED;
   }
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[0] = 4;
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[1] = 4;
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[2] = 4;
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[3] = 5;
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.bits_unused = 0;
   /*GTP TEID*/
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size\
      = 4 * sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
	 gTP_TEID.buf,ulInfo->list.array[idx]->uLUPTNLInformation.choice.\
	 gTPTunnel->gTP_TEID.size);
   if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
	 == NULLP)
   {
      return RFAILED;
   }
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      gTP_TEID.buf[0] = 11;
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      gTP_TEID.buf[1] = 0;
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      gTP_TEID.buf[2] = 0;
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      gTP_TEID.buf[3] = 1;

   return ROK;
}/*End of BuildULTnlInfo*/

/*******************************************************************
 *
 * @brief Builds DRBS to be setup 
 *
 * @details
 *
 *    Function : BuildDRBSetup
 *
 *    Functionality: Constructs the DRB's for UESetReq
 *
 * @params[in] DRBs_ToBeSetup_List_t *drbSet
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDRBSetup(DRBs_ToBeSetup_List_t *drbSet)
{
   uint8_t BuildQOSInforet;
   uint8_t BuildSNSSAIret;
   uint8_t BuildFlowsMapret;
   uint8_t BuildULTnlInforet;
   uint8_t idx;
   uint8_t drbCnt;
   DRBs_ToBeSetup_Item_t *drbSetItem;
   drbCnt = 1;
   drbSet->list.count = drbCnt;
   drbSet->list.size = drbCnt*sizeof(DRBs_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(drbSet->list.array,drbSet->list.size);
   if(drbSet->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<drbCnt; idx++)
   {
      CU_ALLOC(drbSet->list.array[idx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
      if(drbSet->list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   idx = 0;
   drbSet->list.array[idx]->id = ProtocolIE_ID_id_DRBs_ToBeSetup_Item;
   drbSet->list.array[idx]->criticality = Criticality_ignore;
   drbSet->list.array[idx]->value.present = \
					    DRBs_ToBeSetup_ItemIEs__value_PR_DRBs_ToBeSetup_Item;
   drbSetItem = &drbSet->list.array[idx]->value.choice.DRBs_ToBeSetup_Item;	
   /*dRBID*/
   drbSetItem->dRBID = 1;	
   /*qoSInformation*/
   drbSetItem->qoSInformation.present = QoSInformation_PR_choice_extension;
   CU_ALLOC(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
   if(drbSetItem->qoSInformation.choice.choice_extension == NULLP)
   {	
      return RFAILED;
   }
   drbSetItem->qoSInformation.choice.choice_extension->id = \
							    ProtocolIE_ID_id_DRB_Information;
   drbSetItem->qoSInformation.choice.choice_extension->criticality = \
								     Criticality_ignore;
   drbSetItem->qoSInformation.choice.choice_extension->value.present = \
								       QoSInformation_ExtIEs__value_PR_DRB_Information;
  BuildQOSInforet =  BuildQOSInfo(&drbSetItem->qoSInformation.choice.\
	 choice_extension->value.choice.DRB_Information.dRB_QoS);
   if(BuildQOSInforet != ROK)
   {
      return RFAILED;
   }
   /*SNSSAI*/
   BuildSNSSAIret = BuildSNSSAI(&drbSetItem->qoSInformation.choice.\
	 choice_extension->value.choice.DRB_Information.sNSSAI);
   if(BuildSNSSAIret != ROK)
   {	
      return RFAILED;
   }
   /*Flows mapped to DRB List*/
   BuildFlowsMapret = BuildFlowsMap(&drbSetItem->qoSInformation.choice.\
	 choice_extension->value.choice.DRB_Information.flows_Mapped_To_DRB_List);
   if(BuildFlowsMapret != ROK)
   {
      return RFAILED;
   }
   /*ULUPTNLInformation To Be Setup List*/
   BuildULTnlInforet = BuildULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
   if(BuildULTnlInforet != ROK)
   {
      return RFAILED;
   }
   /*RLCMode*/
   drbSetItem->rLCMode = RLCMode_rlc_um_bidirectional;

   /*UL Configuration*/
   CU_ALLOC(drbSetItem->uLConfiguration,sizeof(ULConfiguration_t));
   if(drbSetItem->uLConfiguration == NULLP)
   {
      return RFAILED;
   }
   drbSetItem->uLConfiguration->uLUEConfiguration = ULUEConfiguration_no_data;
   return ROK;
}/* End of BuildDRBSetup*/

/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeNrcgi
 *
 *    Functionality: Deallocating memory for function BuildNrcgi
 *
 * @params[in] NRCGI_t *nrcgi
 *
 * @return void
 *
 *******************************************************************/
void FreeNrcgi(NRCGI_t *nrcgi)
{
   if(nrcgi->pLMN_Identity.buf != NULLP)
   {
      if(nrcgi->nRCellIdentity.buf != NULLP)
      {
	 CU_FREE(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
      }
      CU_FREE(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   }
}
/*******************************************************************
 *
 * @brief  Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeSplCellList
 *
 *    Functionality: Deallocating memory for function BuildSplCellList
 *
 * @params[in] SCell_ToBeSetup_List_t *spCellLst
 *
 * @return void
 *      
 *
 * *****************************************************************/
void FreeSplCellList(SCell_ToBeSetup_List_t *spCellLst)
{
   uint8_t  cellidx;
   if(spCellLst->list.array != NULLP)
   {
      for(cellidx=0; cellidx<spCellLst->list.count; cellidx++)
      {
	 if(cellidx==0&&spCellLst->list.array[cellidx]!=NULLP)
	 {
	    FreeNrcgi(&spCellLst->list.array[cellidx]->value.choice.SCell_ToBeSetup_Item.sCell_ID);
	 }
	 if(spCellLst->list.array[cellidx]!=NULLP)
	 {
	    CU_FREE(spCellLst->list.array[cellidx],sizeof(SCell_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(spCellLst->list.array,spCellLst->list.size);
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeSRBSetup
 *
 *    Functionality: Deallocating memory for function BuildSRBSetup
 *
 * @params[in] SRBs_ToBeSetup_List_t *srbSet
 *
 * @return void
 *        
 *
 * ******************************************************************/
void FreeSRBSetup(SRBs_ToBeSetup_List_t *srbSet)
{
   uint8_t srbidx;
   if(srbSet->list.array != NULLP)
   {
      for(srbidx=0; srbidx<srbSet->list.count; srbidx++)
      {
	 if(srbSet->list.array[srbidx]!=NULLP)
	 {
	    CU_FREE(srbSet->list.array[srbidx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(srbSet->list.array,srbSet->list.size);
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeQOSInfo
 *
 *    Functionality:  Deallocating memory for function BuildQOSInfo
 *
 * @params[in] QoSFlowLevelQoSParameters_t *drbQos
 *
 * @return void
 *          
 * ****************************************************************/
void FreeQOSInfo(QoSFlowLevelQoSParameters_t *drbQos)
{
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI != NULLP)
   {
      if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
      {
	 if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
	 {
	    CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
		  sizeof(MaxDataBurstVolume_t));
	 }
	 CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
	       sizeof(AveragingWindow_t));
      }
      CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,\
	    sizeof(NonDynamic5QIDescriptor_t));
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeULTnlInfo
 *
 *    Functionality:  Deallocating memory for function BuildULTnlInfo
 *
 * @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
 *
 * @return void
 *         

 * ****************************************************************/
void FreeULTnlInfo(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
   uint8_t ulidx=0;
   if(ulInfo->list.array != NULLP)
   {
      for(ulidx=0; ulidx<ulInfo->list.count; ulidx++)
      {
	 if(ulidx==0&&ulInfo->list.array[ulidx]!=NULLP)
	 {
	    if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel!=NULLP)
	    {
	       if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
		     transportLayerAddress.buf != NULLP)
	       {
		  if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
			!=NULLP)
		  {
		     CU_ALLOC(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
			   gTP_TEID.buf,ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.\
			   gTPTunnel->gTP_TEID.size);
		  }
		  CU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
			transportLayerAddress.buf,ulInfo->list.array[ulidx]->\
			uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
	       }
	       CU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel,\
		     sizeof(GTPTunnel_t));
	    }
	 }
	 if(ulInfo->list.array[ulidx]!=NULLP)
	 {
	    CU_FREE(ulInfo->list.array[ulidx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
	 }
      }
      CU_FREE(ulInfo->list.array,ulInfo->list.size);
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory for BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeDRBSetup
 *
 *    Functionality:  Deallocating memory for BuildDRBSetup
 *
 * @params[in] DRBs_ToBeSetup_List_t *drbSet
 *
 * @return void
 *
 * ****************************************************************/
void FreeDRBSetup(DRBs_ToBeSetup_List_t *drbSet)
{
   DRBs_ToBeSetup_Item_t *drbSetItem;
   uint8_t  flowidx;
   uint8_t  drbidx;
   if(drbSet->list.array == NULLP)
   {
      for(drbidx=0; drbidx<drbSet->list.count; drbidx++)
      {
	 if(drbidx==0&&drbSet->list.array[drbidx] != NULLP)
	 {
	    drbSetItem =&drbSet->list.array[drbidx]->value.choice.DRBs_ToBeSetup_Item;
	    if(drbSetItem->qoSInformation.choice.choice_extension != NULLP)
	    {
	       if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
		     qoS_Characteristics.choice.non_Dynamic_5QI !=NULLP)
	       {
		  if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
		  {
		     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			   qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
		     {
			if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf!=NULLP)
			{
			   if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD!=NULLP)
			   {
			      if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf!=NULLP)
			      {
				 if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
				       flows_Mapped_To_DRB_List.list.array != NULLP)
				 {
				    for(flowidx=0;flowidx<drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
					  flows_Mapped_To_DRB_List.list.count; flowidx++)
				    {
				       if(flowidx==0&&drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
					     DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]!=NULLP)
				       {
					  if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						qoS_Characteristics.choice.non_Dynamic_5QI!=NULLP)
					  {
					     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						   DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						   qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
					     {
						if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						      DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						      qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
						{	
						   FreeULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
						   CU_FREE(drbSetItem->uLConfiguration,sizeof(ULConfiguration_t));

						   CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
							 DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
							 qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
							 sizeof(MaxDataBurstVolume_t));	  
						}
						CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						      DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						      qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
					     }
					     CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						   DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						   qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
					  }
				       }
				       if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
					     DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]!=NULLP)
				       {
					  CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx],sizeof(Flows_Mapped_To_DRB_Item_t));
				       }
				    }
				    CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
					  flows_Mapped_To_DRB_List.list.array,drbSetItem->qoSInformation.choice.choice_extension->value.\
					  choice.DRB_Information.flows_Mapped_To_DRB_List.list.size);
				 }
				 CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf,\
				       drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->size);
			      }
			      CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD,\
				    sizeof(OCTET_STRING_t));
			   }
			   CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf,\
				 drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.size);
			}
			CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			      qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
		     }
		     CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			   qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		  }
		  CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			qoS_Characteristics.choice.non_Dynamic_5QI, sizeof(NonDynamic5QIDescriptor_t));
	       }
	       CU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
	    }
	 }
	 if(drbSet->list.array[drbidx]!=NULLP)
	 {
	    CU_FREE(drbSet->list.array[drbidx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(drbSet->list.array,drbSet->list.size);
   }
}


/*******************************************************************
 *
 * @brief Free the UE Setup Request
 *
 * @details
 *
 *    Function : FreeUeContextSetupReq
 *
 *    Functionality: Deallocate the memory of BuildUESetReq
 *
 * @params[in]  F1AP_PDU_t  *f1apMsg
 *
 * @return void
 *
 *
 * ****************************************************************/
void FreeUeContextSetupReq(F1AP_PDU_t  *f1apMsg)
{
   uint8_t idx, ieId;
   UEContextSetupRequest_t  *ueSetReq = NULLP;

   if(f1apMsg != NULLP)
   {
      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
	 ueSetReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;
	 if(ueSetReq->protocolIEs.list.array != NULLP)
	 {
	    for(idx = 0; idx < ueSetReq->protocolIEs.list.count; idx++)
	    {
	       if(ueSetReq->protocolIEs.list.array[idx])
	       {
	          switch(ueSetReq->protocolIEs.list.array[idx]->id)
		  {
                     case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_SpCell_ID:
	                FreeNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI);
		        break;
		     case ProtocolIE_ID_id_ServCellIndex:
		        break;
		     case ProtocolIE_ID_id_SpCellULConfigured:
		        break;
		     case ProtocolIE_ID_id_CUtoDURRCInformation:
		        FreeCuToDuInfo(&ueSetReq->protocolIEs.list.array[idx]->value.choice.CUtoDURRCInformation);
		        break;
		     case ProtocolIE_ID_id_SCell_ToBeSetup_List:
	                FreeSplCellList(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SCell_ToBeSetup_List);
			break;
		     case ProtocolIE_ID_id_SRBs_ToBeSetup_List:
		        FreeSRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List);
		        break;
                     case ProtocolIE_ID_id_DRBs_ToBeSetup_List:
		        FreeDRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List);
			break;
                     case ProtocolIE_ID_id_RRCContainer:
		        if(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf != NULLP)
			{
			  CU_ALLOC(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf, \
			  ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
			}
			break;
		     default:
		        printf("\nF1AP: Invalid event type %ld", ueSetReq->protocolIEs.list.array[idx]->id);
	          }
	       }
	       break;
	    }
	    for(ieId=0; ieId<idx; ieId++)
	    {
	       if(ueSetReq->protocolIEs.list.array[ieId] != NULLP)
	       {
	          CU_FREE(ueSetReq->protocolIEs.list.array[ieId],sizeof(UEContextSetupRequestIEs_t));
	       }
	    }
	    CU_FREE(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);
	 }
	 CU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/**Filling cell group info **/
/*******************************************************************
 *
 * @brief Build Control resource set to add/modify list 
 *
 * @details
 *
 *    Function : BuildControlRSetToAddModList
 *
 *    Functionality: Build Control resource set to add/modify list
 *
 * @params[in] 
 * struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildControlRSetToAddModList
(
struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList
)
{
   uint8_t idx;
   uint8_t elementCnt;
   uint8_t numBytes, bitsUnused;
   struct ControlResourceSet *controlRSet;
   uint8_t freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   uint8_t coreset0EndPrb, coreset1StartPrb, coreset1NumPrb;

   elementCnt = 1;
   controlRSetList->list.count = elementCnt;
   controlRSetList->list.size = \
       elementCnt * sizeof(struct ControlResourceSet *);

   controlRSetList->list.array = NULLP;
   CU_ALLOC(controlRSetList->list.array, controlRSetList->list.size);
   if(!controlRSetList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      controlRSetList->list.array[idx] = NULLP;
      CU_ALLOC(controlRSetList->list.array[idx], sizeof(struct ControlResourceSet));
      if(!controlRSetList->list.array[idx])
      {
         DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
         return RFAILED;
      }
   }

   idx=0;
   controlRSet = controlRSetList->list.array[idx];
   controlRSet->controlResourceSetId = PDCCH_CTRL_RSRC_SET_ONE_ID;

   /* Values harcoded according to our design:
    * size 6 bytes
    * 3 LSBs unsued
    * Bit string stored ff0000000000
    */
   numBytes = 6;
   bitsUnused = 3;
   controlRSet->frequencyDomainResources.size = numBytes * sizeof(uint8_t);
   controlRSet->frequencyDomainResources.buf = NULLP;
	CU_ALLOC(controlRSet->frequencyDomainResources.buf, \
	   controlRSet->frequencyDomainResources.size);
	if(!controlRSet->frequencyDomainResources.buf)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
		return RFAILED;
   }

   memset(controlRSet->frequencyDomainResources.buf, 0, FREQ_DOM_RSRC_SIZE);
   coreset0EndPrb = CORESET0_END_PRB;
   coreset1StartPrb = coreset0EndPrb + 6;
   coreset1NumPrb = CORESET1_NUM_PRB;
   /* calculate the PRBs */
   freqDomRscAllocType0(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
   memcpy(controlRSet->frequencyDomainResources.buf, freqDomainResource, FREQ_DOM_RSRC_SIZE);
   controlRSet->frequencyDomainResources.bits_unused = bitsUnused;

   controlRSet->duration = PDCCH_CTRL_RSRC_SET_ONE_DURATION;
   controlRSet->cce_REG_MappingType.present = \
      ControlResourceSet__cce_REG_MappingType_PR_nonInterleaved;

   controlRSet->precoderGranularity = PDCCH_CTRL_RSRC_SET_ONE_PRECOD_GRANULARITY;
   controlRSet->tci_StatesPDCCH_ToAddList = NULLP;
	controlRSet->tci_StatesPDCCH_ToReleaseList = NULLP;
	controlRSet->tci_PresentInDCI = NULLP;
#if 0
   uint8_t tciStateIdx;

   CU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList, \
      sizeof(struct ControlResourceSet__tci_StatesPDCCH_ToAddList));
   if(!controlRset->tci_StatesPDCCH_ToAddList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   elementCnt = 1;
   controlRset->tci_StatesPDCCH_ToAddList->list.count = elementCnt;
   controlRset->tci_StatesPDCCH_ToAddList->list.size = elementCnt * sizeof(TCI_StateId_t *);
   CU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array, \
      controlRset->tci_StatesPDCCH_ToAddList->list.size)
   if(!controlRset->tci_StatesPDCCH_ToAddList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }
   
   for(tciStateIdx = 0; tciStateIdx <elementCntl; tciStateIdx++)
   {
      CU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx], sizeof(TCI_StateId_t));
      if(!controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx])
      {
         DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
         return RFAILED;
      }
   }

   tciStateIdx = 0;
   /* TODO */
   *(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx]);

   CU_ALLOC(controlRset->tci_PresentInDCI, sizeof(long));
   if(!controlRset->tci_PresentInDCI)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }
   /* TODO */
   *(controlRset->tci_PresentInDCI);
#endif

   controlRSet->pdcch_DMRS_ScramblingID = NULLP;
   CU_ALLOC(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
   if(!controlRSet->pdcch_DMRS_ScramblingID)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }
   *(controlRSet->pdcch_DMRS_ScramblingID) = SCRAMBLING_ID;

   return ROK;
} /* End BuildControlRSetToAddModList */

/*******************************************************************
 *
 * @brief Build search space to add/modify list
 *
 * @details
 *
 *    Function : BuildSearchSpcToAddModList
 *
 *    Functionality: Build search space to add/modify list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSearchSpcToAddModList
(
struct PDCCH_Config__searchSpacesToAddModList *searchSpcList
)
{
   uint8_t idx;
	uint8_t numBytes;
	uint8_t byteIdx;
	uint8_t bitsUnused;
   uint8_t elementCnt;
   struct SearchSpace *searchSpc;

   elementCnt = 1;
   searchSpcList->list.count = elementCnt;
   searchSpcList->list.size = elementCnt * sizeof(struct SearchSpace *);

	searchSpcList->list.array = NULLP;
   CU_ALLOC(searchSpcList->list.array, searchSpcList->list.size);
   if(!searchSpcList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
	   searchSpcList->list.array[idx] = NULLP;
      CU_ALLOC(searchSpcList->list.array[idx], sizeof(struct SearchSpace));
      if(!searchSpcList->list.array[idx])
      {
         DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }
   }

   idx = 0;
   searchSpc = searchSpcList->list.array[idx];

   searchSpc->searchSpaceId = PDCCH_SRCH_SPC_TWO_ID;

   searchSpc->controlResourceSetId = NULLP;
   CU_ALLOC(searchSpc->controlResourceSetId, sizeof(ControlResourceSetId_t));
   if(!searchSpc->controlResourceSetId)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }
   *(searchSpc->controlResourceSetId) = PDCCH_CTRL_RSRC_SET_ONE_ID;

   searchSpc->monitoringSlotPeriodicityAndOffset = NULLP;
   CU_ALLOC(searchSpc->monitoringSlotPeriodicityAndOffset, \
      sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
   if(!searchSpc->monitoringSlotPeriodicityAndOffset)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
		return RFAILED;
	}
   searchSpc->monitoringSlotPeriodicityAndOffset->present = \
	   SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1;
  
   searchSpc->duration = NULLP;
   searchSpc->monitoringSymbolsWithinSlot = NULLP;
	CU_ALLOC(searchSpc->monitoringSymbolsWithinSlot, sizeof(BIT_STRING_t));
	if(!searchSpc->monitoringSymbolsWithinSlot)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
		return RFAILED;
	}
   
	/* Values taken from reference logs :
	 * size 2 bytes
	 * 2 LSBs unsued
	 * Bit string stores 8000
	 */
   numBytes = 2;
	bitsUnused = 2;

   searchSpc->monitoringSymbolsWithinSlot->size = numBytes * sizeof(uint8_t);
	searchSpc->monitoringSymbolsWithinSlot->buf = NULLP;
	CU_ALLOC(searchSpc->monitoringSymbolsWithinSlot->buf, \
	   searchSpc->monitoringSymbolsWithinSlot->size);
	if(!searchSpc->monitoringSymbolsWithinSlot->buf)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
		return RFAILED;
	}

	byteIdx = 0;
	searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = \
	 PDCCH_SYMBOL_WITHIN_SLOT /* setting MSB to 128 i.e. 0x80 */;
	searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = 0;
	searchSpc->monitoringSymbolsWithinSlot->bits_unused = bitsUnused;

   searchSpc->nrofCandidates = NULLP;
   CU_ALLOC(searchSpc->nrofCandidates, sizeof(struct SearchSpace__nrofCandidates));
   if(!searchSpc->nrofCandidates)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   searchSpc->nrofCandidates->aggregationLevel1 = \
	   PDCCH_SRCH_SPC_TWO_AGG_LVL1_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel2 = \
	   PDCCH_SRCH_SPC_TWO_AGG_LVL2_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel4 = \
	   PDCCH_SRCH_SPC_TWO_AGG_LVL4_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel8 = \
	   PDCCH_SRCH_SPC_TWO_AGG_LVL8_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel16 = \
	   PDCCH_SRCH_SPC_TWO_AGG_LVL16_CANDIDATE;

   searchSpc->searchSpaceType = NULLP;
   CU_ALLOC(searchSpc->searchSpaceType, sizeof(struct SearchSpace__searchSpaceType));
   if(!searchSpc->searchSpaceType)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }
   
   searchSpc->searchSpaceType->present = SearchSpace__searchSpaceType_PR_ue_Specific;

   searchSpc->searchSpaceType->choice.ue_Specific = NULLP;
   CU_ALLOC(searchSpc->searchSpaceType->choice.ue_Specific, \
      sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
   if(!searchSpc->searchSpaceType->choice.ue_Specific)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }  
   searchSpc->searchSpaceType->choice.ue_Specific->dci_Formats = \
	   PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT;

   return ROK;
}/* End BuildSearchSpcToAddModList */

/*******************************************************************
 *
 * @brief Builds BWP DL dedicated PDCCH config
 *
 * @details
 *
 *    Function : BuildBWPDlDedPdcchCfg
 *
 *    Functionality: Builds BWP DL dedicated PDCCH config
 *
 * @params[in] struct PDCCH_Config *pdcchCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPDlDedPdcchCfg(struct PDCCH_Config *pdcchCfg)
{
   pdcchCfg->controlResourceSetToAddModList = NULLP;
   CU_ALLOC(pdcchCfg->controlResourceSetToAddModList, \
      sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
   if(!pdcchCfg->controlResourceSetToAddModList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
      return RFAILED;
   }

	if(BuildControlRSetToAddModList(pdcchCfg->controlResourceSetToAddModList) != ROK)
	{
	   return RFAILED;
	}
  
   pdcchCfg->controlResourceSetToReleaseList = NULLP;

   pdcchCfg->searchSpacesToAddModList = NULLP;
	CU_ALLOC(pdcchCfg->searchSpacesToAddModList, \
	   sizeof(struct PDCCH_Config__searchSpacesToAddModList));
	if(!pdcchCfg->searchSpacesToAddModList)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
		return RFAILED;
	}
  
	if(BuildSearchSpcToAddModList(pdcchCfg->searchSpacesToAddModList) != ROK)
	{
	   return RFAILED;
	}
   
   pdcchCfg->searchSpacesToReleaseList = NULLP;
	pdcchCfg->downlinkPreemption = NULLP;
	pdcchCfg->tpc_PUSCH = NULLP;
	pdcchCfg->tpc_PUCCH = NULLP;
	pdcchCfg->tpc_SRS = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds DMRS DL PDSCH Mapping type A
 *
 * @details
 *
 *    Function : BuildDMRSDLPdschMapTypeA
 *
 *    Functionality: Builds DMRS DL PDSCH Mapping type A
 *
 * @params[in]
 * struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDMRSDLPdschMapTypeA
(
struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg
)
{
   dmrsDlCfg->present = PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA_PR_setup;
	dmrsDlCfg->choice.setup = NULLP;
   CU_ALLOC(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
   if(!dmrsDlCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
        
	dmrsDlCfg->choice.setup->dmrs_Type = NULLP;
	dmrsDlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   CU_ALLOC(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsDlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSDLPdschMapTypeA");
      return RFAILED;
   }
   *(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS;
         
	dmrsDlCfg->choice.setup->maxLength = NULLP;
	dmrsDlCfg->choice.setup->scramblingID0 = NULLP;
	dmrsDlCfg->choice.setup->scramblingID1 = NULLP;
	dmrsDlCfg->choice.setup->phaseTrackingRS = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds TCI states to add/modify list
 *
 * @details
 *
 *    Function : BuildTCIStatesToAddModList
 *
 *    Functionality:Builds TCI states to add/modify list
 *
 * @params[in] 
 * struct PDSCH_Config__tci_StatesToAddModList *tciStatesList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildTCIStatesToAddModList(struct PDSCH_Config__tci_StatesToAddModList *tciStatesList)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH time domain allocation list
 *
 * @details
 *
 *    Function : BuildPdschTimeDomAllocList
 *
 *    Functionality: Builds PDSCH time domain allocation list
 *
 * @params[in] 
 * struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPdschTimeDomAllocList
(
struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
)
{
   uint8_t idx;
   uint8_t elementCnt;
	struct PDSCH_TimeDomainResourceAllocation *timeDomAlloc;

   timeDomAllocList->present = \
	   PDSCH_Config__pdsch_TimeDomainAllocationList_PR_setup;

   timeDomAllocList->choice.setup = NULLP;
   CU_ALLOC(timeDomAllocList->choice.setup, \
	   sizeof(struct PDSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
		return RFAILED;
	}
         
	elementCnt = 1;
	timeDomAllocList->choice.setup->list.count = elementCnt;
	timeDomAllocList->choice.setup->list.size = \
	   elementCnt * sizeof(struct PDSCH_TimeDomainResourceAllocation *);

	timeDomAllocList->choice.setup->list.array = NULLP;
	CU_ALLOC(timeDomAllocList->choice.setup->list.array, \
	   timeDomAllocList->choice.setup->list.size);
	if(!timeDomAllocList->choice.setup->list.array)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
		return RFAILED;
	}

	for(idx = 0; idx < elementCnt; idx++)
	{
	   timeDomAllocList->choice.setup->list.array[idx] = NULLP;
	   CU_ALLOC(timeDomAllocList->choice.setup->list.array[idx], \
		   sizeof(struct PDSCH_TimeDomainResourceAllocation));
		if(!timeDomAllocList->choice.setup->list.array[idx])
		{
		   DU_LOG("\nF1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
			return RFAILED;
		}
	}

	idx = 0;
	timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];

   timeDomAlloc->k0 = NULLP;
   timeDomAlloc->mappingType = PDSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = 53;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH PRB Bundling type
 *
 * @details
 *
 *    Function : BuildPdschPrbBundlingType
 *
 *    Functionality: Builds PDSCH PRB Bundling type
 *
 * @params[in] 
 * struct PDSCH_Config__prb_BundlingType *prbBndlType
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPdschPrbBundlingType
(
struct PDSCH_Config__prb_BundlingType *prbBndlType
)
{
   prbBndlType->present = PDSCH_Config__prb_BundlingType_PR_staticBundling;

   prbBndlType->choice.staticBundling = NULLP;
	CU_ALLOC(prbBndlType->choice.staticBundling, \
	   sizeof(struct PDSCH_Config__prb_BundlingType__staticBundling));
	if(!prbBndlType->choice.staticBundling)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildPdschPrbBundlingType");
		return RFAILED;
	}
	prbBndlType->choice.staticBundling->bundleSize = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP DL dedicated PDSCH config 
 *
 * @details
 *
 *    Function : BuildBWPDlDedPdschCfg
 *
 *    Functionality: Builds BWP DL dedicated PDSCH config
 *
 * @params[in] struct PDSCH_Config *pdschCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPDlDedPdschCfg(struct PDSCH_Config *pdschCfg)
{
   pdschCfg->dataScramblingIdentityPDSCH = NULLP;

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA = NULLP;
   CU_ALLOC(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, \
      sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
   if(!pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }

   if(BuildDMRSDLPdschMapTypeA(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeB = NULLP;
   pdschCfg->tci_StatesToAddModList = NULLP;
   pdschCfg->tci_StatesToReleaseList = NULLP;
	pdschCfg->vrb_ToPRB_Interleaver = NULLP;
#if 0
   CU_ALLOC(pdschCfg->tci_StatesToAddModList, sizeof(struct PDSCH_Config__tci_StatesToAddModList));
   if(!pdschCfg->tci_StatesToAddModList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   if(BuildTCIStatesToAddModList(pdschCfg->tci_StatesToAddModList) != ROK)
   {
      return RFAILED;
   }
#endif

	pdschCfg->resourceAllocation = RES_ALLOC_TYPE;

   pdschCfg->pdsch_TimeDomainAllocationList = NULLP;
	CU_ALLOC(pdschCfg->pdsch_TimeDomainAllocationList, \
	   sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
   if(!pdschCfg->pdsch_TimeDomainAllocationList)
	{
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
		return RFAILED;
	}
	if(BuildPdschTimeDomAllocList(pdschCfg->pdsch_TimeDomainAllocationList) != ROK)
	{
	   return RFAILED;
	}
   pdschCfg->pdsch_AggregationFactor = NULLP;
	pdschCfg->rateMatchPatternToAddModList = NULLP;
	pdschCfg->rateMatchPatternToReleaseList = NULLP;
	pdschCfg->rateMatchPatternGroup1 = NULLP;
	pdschCfg->rateMatchPatternGroup2 = NULLP;
	pdschCfg->rbg_Size = PDSCH_RBG_SIZE;
	pdschCfg->mcs_Table = NULLP;

	pdschCfg->maxNrofCodeWordsScheduledByDCI = NULLP;
	CU_ALLOC(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
	if(!pdschCfg->maxNrofCodeWordsScheduledByDCI)
	{
	   DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
		return RFAILED;
	}
	*(pdschCfg->maxNrofCodeWordsScheduledByDCI) = PDSCH_MAX_CODEWORD_SCH_BY_DCI;
	
	if(BuildPdschPrbBundlingType(&pdschCfg->prb_BundlingType) != ROK)
	{
	   return RFAILED;
	}

	pdschCfg->zp_CSI_RS_ResourceToAddModList = NULLP;
	pdschCfg->zp_CSI_RS_ResourceToReleaseList = NULLP;
	pdschCfg->aperiodic_ZP_CSI_RS_ResourceSetsToAddModList = NULLP;
	pdschCfg->aperiodic_ZP_CSI_RS_ResourceSetsToReleaseList = NULLP;
	pdschCfg->sp_ZP_CSI_RS_ResourceSetsToAddModList = NULLP;
	pdschCfg->sp_ZP_CSI_RS_ResourceSetsToReleaseList = NULLP;
   pdschCfg->p_ZP_CSI_RS_ResourceSet = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds intitial DL BWP
 * @details
 *
 *    Function : BuildInitialDlBWP 
 *
 *    Functionality: Builds intitial DL BWP in spCellCfgDed
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildInitialDlBWP(BWP_DownlinkDedicated_t *dlBwp)
{
   dlBwp->pdcch_Config = NULLP;
   CU_ALLOC(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
	if(!dlBwp->pdcch_Config)
	{
	   DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
	}
	dlBwp->pdcch_Config->present = BWP_DownlinkDedicated__pdcch_Config_PR_setup; 

   dlBwp->pdcch_Config->choice.setup = NULLP;
	CU_ALLOC(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
   if(!dlBwp->pdcch_Config->choice.setup)
   {
	   DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
		return RFAILED;
	}
   if(BuildBWPDlDedPdcchCfg(dlBwp->pdcch_Config->choice.setup) != ROK)
	{
	   return RFAILED;
	}

   dlBwp->pdsch_Config = NULLP;
	CU_ALLOC(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
	if(!dlBwp->pdsch_Config)
	{
	   DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
		return RFAILED;
	}
	dlBwp->pdsch_Config->present = BWP_DownlinkDedicated__pdsch_Config_PR_setup;

   dlBwp->pdsch_Config->choice.setup = NULLP;
   CU_ALLOC(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
   if(!dlBwp->pdsch_Config->choice.setup)
   {
      DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
		return RFAILED;
	}

   if(BuildBWPDlDedPdschCfg(dlBwp->pdsch_Config->choice.setup) != ROK)
	{
	   return RFAILED;
	}

   dlBwp->sps_Config = NULLP;
   dlBwp->radioLinkMonitoringConfig = NULLP; 
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds DMRS UL Pusch Mapping type A
 *
 * @details
 *
 *    Function : BuildDMRSULPuschMapTypeA
 *
 *    Functionality: Builds DMRS UL Pusch Mapping type A
 *
 * @params[in] 
 *    struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDMRSULPuschMapTypeA
(
struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg
)
{
   dmrsUlCfg->present = PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup;
   dmrsUlCfg->choice.setup= NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup, sizeof(DMRS_UplinkConfig_t));
   if(!dmrsUlCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
         
   dmrsUlCfg->choice.setup->dmrs_Type = NULLP;
	dmrsUlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   *(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS; 

	dmrsUlCfg->choice.setup->phaseTrackingRS = NULLP;
	dmrsUlCfg->choice.setup->maxLength = NULLP;
   dmrsUlCfg->choice.setup->transformPrecodingDisabled = NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled, \
      sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }

   dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0 = NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
      sizeof(long));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   *(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0) = SCRAMBLING_ID;

	dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID1 = NULLP;
	dmrsUlCfg->choice.setup->transformPrecodingEnabled = NULLP;
   return ROK;
}

/*******************************************************************
 *
 * @brief Build PUSCH time domain allocation list
 *
 * @details
 *
 *    Function : BuildPuschTimeDomAllocList
 *
 *    Functionality: Build PUSCH time domain allocation list
 *
 * @params[in] 
 * struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPuschTimeDomAllocList
(
struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList
)
{
   uint8_t idx;
   uint8_t elementCnt;
   PUSCH_TimeDomainResourceAllocation_t  *timeDomAlloc;

   timeDomAllocList->present = PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup;
   timeDomAllocList->choice.setup = NULLP;
   CU_ALLOC(timeDomAllocList->choice.setup, \
      sizeof(struct PUSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }
         
   elementCnt = 1;
   timeDomAllocList->choice.setup->list.count = elementCnt;
   timeDomAllocList->choice.setup->list.size = \
      elementCnt * sizeof(PUSCH_TimeDomainResourceAllocation_t *);
	timeDomAllocList->choice.setup->list.array = NULLP;
   CU_ALLOC(timeDomAllocList->choice.setup->list.array, \
      timeDomAllocList->choice.setup->list.size);
   if(!timeDomAllocList->choice.setup->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
	   timeDomAllocList->choice.setup->list.array[idx] = NULLP;
      CU_ALLOC(timeDomAllocList->choice.setup->list.array[idx],\
         sizeof(PUSCH_TimeDomainResourceAllocation_t));
      if(!timeDomAllocList->choice.setup->list.array[idx])
      {
         DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
         return RFAILED;
      }
   }

   idx = 0;
   timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
   CU_ALLOC(timeDomAlloc->k2, sizeof(long));
   if(!timeDomAlloc->k2)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }
   *(timeDomAlloc->k2) = PUSCH_K2;
   timeDomAlloc->mappingType = PUSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = 27; 
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP UL dedicated PUSCH Config
 *
 * @details
 *
 *    Function : BuildBWPUlDedPuschCfg
 *
 *    Functionality:
 *      Builds BWP UL dedicated PUSCH Config
 *
 * @params[in] : PUSCH_Config_t *puschCfg
 *    
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPUlDedPuschCfg(PUSCH_Config_t *puschCfg)
{
   puschCfg->dataScramblingIdentityPUSCH = NULLP;
   CU_ALLOC(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
   if(!puschCfg->dataScramblingIdentityPUSCH)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   *(puschCfg->dataScramblingIdentityPUSCH) = SCRAMBLING_ID;

   puschCfg->txConfig = NULLP;
	puschCfg->dmrs_UplinkForPUSCH_MappingTypeA = NULLP;
   CU_ALLOC(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, \
      sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
   if(!puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildDMRSULPuschMapTypeA(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   puschCfg->dmrs_UplinkForPUSCH_MappingTypeB = NULLP;
	puschCfg->pusch_PowerControl = NULLP;
	puschCfg->frequencyHopping = NULLP;
	puschCfg->frequencyHoppingOffsetLists = NULLP;
   puschCfg->resourceAllocation = RES_ALLOC_TYPE;

   puschCfg->pusch_TimeDomainAllocationList = NULLP;
   CU_ALLOC(puschCfg->pusch_TimeDomainAllocationList, \
      sizeof(struct PUSCH_Config__pusch_TimeDomainAllocationList));
   if(!puschCfg->pusch_TimeDomainAllocationList)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildPuschTimeDomAllocList(puschCfg->pusch_TimeDomainAllocationList) != ROK)
   {
      return RFAILED;
   }

   puschCfg->pusch_AggregationFactor = NULLP;
	puschCfg->mcs_Table = NULLP;
	puschCfg->mcs_TableTransformPrecoder = NULLP;
   puschCfg->transformPrecoder = NULLP;
   CU_ALLOC(puschCfg->transformPrecoder, sizeof(long));
   if(!puschCfg->transformPrecoder)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   *(puschCfg->transformPrecoder) = PUSCH_TRANSFORM_PRECODER;

	puschCfg->codebookSubset = NULLP;
	puschCfg->maxRank = NULLP;
	puschCfg->rbg_Size = NULLP;
	puschCfg->uci_OnPUSCH = NULLP;
	puschCfg->tp_pi2BPSK = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills SRS resource to add/modify list 
 *
 * @details
 *
 *    Function : BuildSrsRsrcAddModList
 *
 *    Functionality: Fills SRS resource to add/modify list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSrsRsrcAddModList(struct SRS_Config__srs_ResourceToAddModList *resourceList)
{
   uint8_t   elementCnt;
   uint8_t   rsrcIdx;

   elementCnt = 1;
   resourceList->list.count = elementCnt;
   resourceList->list.size = elementCnt * sizeof(SRS_Resource_t *);
   resourceList->list.array = NULLP;
   CU_ALLOC(resourceList->list.array, resourceList->list.size);
   if(!resourceList->list.array)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }
 
   for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
   {
      CU_ALLOC(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
      if(!resourceList->list.array[rsrcIdx])
      {
         DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
         return RFAILED;
      }
   }

   rsrcIdx = 0;
   resourceList->list.array[rsrcIdx]->srs_ResourceId = SRS_RSRC_ID;
   resourceList->list.array[rsrcIdx]->nrofSRS_Ports = SRS_Resource__nrofSRS_Ports_port1;
   resourceList->list.array[rsrcIdx]->transmissionComb.present = SRS_Resource__transmissionComb_PR_n2;

   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2 = NULLP;
   CU_ALLOC(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2, \
      sizeof(struct SRS_Resource__transmissionComb__n2));
   if(!resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->combOffset_n2\
	   = SRS_COMB_OFFSET_N2;
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->cyclicShift_n2\
	   = SRS_CYCLIC_SHIFT_N2;

   resourceList->list.array[rsrcIdx]->resourceMapping.startPosition = \
	   0;
   resourceList->list.array[rsrcIdx]->resourceMapping.nrofSymbols =  \
      SRS_Resource__resourceMapping__nrofSymbols_n1;
   resourceList->list.array[rsrcIdx]->resourceMapping.repetitionFactor = \
      SRS_Resource__resourceMapping__repetitionFactor_n1;

   resourceList->list.array[rsrcIdx]->freqDomainPosition = SRS_FREQ_DOM_POS;
   resourceList->list.array[rsrcIdx]->freqDomainShift = SRS_FREQ_DOM_SHIFT;
   resourceList->list.array[rsrcIdx]->freqHopping.c_SRS = C_SRS;
   resourceList->list.array[rsrcIdx]->freqHopping.b_SRS = B_SRS;
   resourceList->list.array[rsrcIdx]->freqHopping.b_hop = B_HOP;
   resourceList->list.array[rsrcIdx]->groupOrSequenceHopping = \
      SRS_Resource__groupOrSequenceHopping_neither;

   /* Setting resource type to aperiodic for intergration purposes */
   resourceList->list.array[rsrcIdx]->resourceType.present = \
	   SRS_Resource__resourceType_PR_aperiodic;
   resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic = NULLP;
   CU_ALLOC(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,
      sizeof(struct SRS_Resource__resourceType__aperiodic));
   if(!resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }
   resourceList->list.array[rsrcIdx]->sequenceId = SRS_SEQ_ID;

   return ROK;
}

/*******************************************************************
 *
 * @brief Build SRS resource set Add/mod list
 *
 * @details
 *
 *    Function : BuildSrsRsrcSetAddModList
 *
 *    Functionality: Build SRS resource set Add/mod list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSrsRsrcSetAddModList
(
struct SRS_Config__srs_ResourceSetToAddModList *rsrcSetList
)
{
   uint8_t  elementCnt;
   uint8_t  rSetIdx;
	uint8_t  rsrcIdx;
	struct SRS_ResourceSet__srs_ResourceIdList *rsrcIdList;

   elementCnt = 1;
   rsrcSetList->list.count = elementCnt;
   rsrcSetList->list.size = elementCnt * sizeof(SRS_ResourceSet_t *);
   rsrcSetList->list.array = NULLP;
   CU_ALLOC(rsrcSetList->list.array, rsrcSetList->list.size);
   if(!rsrcSetList->list.array)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
   {
      CU_ALLOC(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
      if(!rsrcSetList->list.array[rSetIdx])
      {
         DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
         return RFAILED;
      }
   }

   rSetIdx = 0;
   rsrcSetList->list.array[rSetIdx]->srs_ResourceSetId = SRS_RSET_ID;

	/* Fill Resource Id list in resource set */
	rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList = NULLP;
	CU_ALLOC(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
	   sizeof(struct SRS_ResourceSet__srs_ResourceIdList));
	if(!rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList)
	{
	   DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
		return RFAILED;
	}

	elementCnt = 1;
	rsrcIdList = rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList;
	rsrcIdList->list.count = elementCnt;
	rsrcIdList->list.size = elementCnt * sizeof(SRS_ResourceId_t *);
	rsrcIdList->list.array = NULLP;
	CU_ALLOC(rsrcIdList->list.array, rsrcIdList->list.size);
	if(!rsrcIdList->list.array)
	{
	   DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
		return RFAILED;
	}

	for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count; rsrcIdx++)
	{
	   CU_ALLOC(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
		if(!rsrcIdList->list.array[rsrcIdx])
		{
		   DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
			return RFAILED;
		}
	}

	rsrcIdx = 0;
	*rsrcIdList->list.array[rsrcIdx] = SRS_RSRC_ID;

	/* Fill resource type */
   rsrcSetList->list.array[rSetIdx]->resourceType.present = \
      SRS_ResourceSet__resourceType_PR_aperiodic;

   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic = NULLP;
   CU_ALLOC(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
      sizeof(struct SRS_ResourceSet__resourceType__aperiodic));
   if(!rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic)
   {
	   DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
		return RFAILED;
	}
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->aperiodicSRS_ResourceTrigger \
	   = APERIODIC_SRS_RESRC_TRIGGER;

	/* TODO : Fill values for below IEs as expected by Viavi */
 	rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->csi_RS = NULLP;
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->slotOffset = NULLP;
  

   rsrcSetList->list.array[rSetIdx]->usage = SRS_ResourceSet__usage_codebook;
	rsrcSetList->list.array[rSetIdx]->alpha = NULLP;
	rsrcSetList->list.array[rSetIdx]->p0 = NULLP;
	rsrcSetList->list.array[rSetIdx]->pathlossReferenceRS = NULLP;
	rsrcSetList->list.array[rSetIdx]->srs_PowerControlAdjustmentStates = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP UL dedicated SRS Config
 *
 * @details
 *
 *    Function : BuildBWPUlDedSrsCfg
 *
 *    Functionality: Builds BWP UL dedicated SRS Config
 *
 * @params[in] SRS Config 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPUlDedSrsCfg(SRS_Config_t *srsCfg)
{
   srsCfg->srs_ResourceSetToReleaseList = NULLP;
   srsCfg->srs_ResourceSetToAddModList = NULLP;
   CU_ALLOC(srsCfg->srs_ResourceSetToAddModList, \
      sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
   if(!srsCfg->srs_ResourceSetToAddModList)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedSrsCfg");
      return RFAILED;
   }
   if(BuildSrsRsrcSetAddModList(srsCfg->srs_ResourceSetToAddModList) != ROK)
   {
      return RFAILED;
   }

   srsCfg->srs_ResourceToReleaseList = NULLP;

   /* Resource to Add/Modify list */
   srsCfg->srs_ResourceToAddModList = NULLP;
   CU_ALLOC(srsCfg->srs_ResourceToAddModList, \
      sizeof(struct SRS_Config__srs_ResourceToAddModList));
   if(!srsCfg->srs_ResourceToAddModList)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedSrsCfg");
      return RFAILED;
   }

   if(BuildSrsRsrcAddModList(srsCfg->srs_ResourceToAddModList) != ROK)
   {
      return RFAILED;
   }

   srsCfg->tpc_Accumulation = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds inital UL BWP
 *
 * @details
 *
 *    Function : BuildInitialUlBWP
 *
 *    Functionality: Builds initial UL BWP
 *
 * @params[in] BWP_UplinkDedicated_t *ulBwp
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildInitialUlBWP(BWP_UplinkDedicated_t *ulBwp)
{
   ulBwp->pucch_Config = NULLP;

   /* Fill BWP UL dedicated PUSCH config */
	ulBwp->pusch_Config = NULLP;
   CU_ALLOC(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));
   if(!ulBwp->pusch_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }
   
   ulBwp->pusch_Config->present = BWP_UplinkDedicated__pusch_Config_PR_setup;
   ulBwp->pusch_Config->choice.setup = NULLP;
   CU_ALLOC(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
   if(!ulBwp->pusch_Config->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedPuschCfg(ulBwp->pusch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

	ulBwp->configuredGrantConfig = NULLP;

   /* Fill BPW UL dedicated SRS config */
	ulBwp->srs_Config = NULLP;
   CU_ALLOC(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
   if(!ulBwp->srs_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->srs_Config->present = BWP_UplinkDedicated__srs_Config_PR_setup;
   ulBwp->srs_Config->choice.setup = NULLP;
   CU_ALLOC(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
   if(!ulBwp->srs_Config->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedSrsCfg(ulBwp->srs_Config->choice.setup) != ROK)
   {
      return RFAILED;   
   }

	ulBwp->beamFailureRecoveryConfig = NULLP;
   
   return ROK;
}

/*******************************************************************
*
* @brief Builds Pusch Serving cell Config
*
* @details
*
*    Function : BuildPuschSrvCellCfg
*
*    Functionality: Builds Pusch Serving cell Config
*
* @params[in] struct UplinkConfig__pusch_ServingCellConfig *puschCfg
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildPuschSrvCellCfg(struct UplinkConfig__pusch_ServingCellConfig *puschCfg)
{
   puschCfg->present = UplinkConfig__pusch_ServingCellConfig_PR_setup;
   puschCfg->choice.setup = NULLP;
   CU_ALLOC(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
   if(!puschCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
	puschCfg->choice.setup->rateMatching = NULLP;
	puschCfg->choice.setup->xOverhead = NULLP;
   puschCfg->choice.setup->ext1 = NULLP;
   CU_ALLOC(puschCfg->choice.setup->ext1, sizeof(struct PUSCH_ServingCellConfig__ext1));
   if(!puschCfg->choice.setup->ext1)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->ext1->maxMIMO_Layers = NULLP;
   CU_ALLOC(puschCfg->choice.setup->ext1->maxMIMO_Layers, sizeof(long));
   if(!puschCfg->choice.setup->ext1->maxMIMO_Layers)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }
   *(puschCfg->choice.setup->ext1->maxMIMO_Layers) = PUSCH_MAX_MIMO_LAYERS;

   puschCfg->choice.setup->ext1->processingType2Enabled= NULLP;
   CU_ALLOC(puschCfg->choice.setup->ext1->processingType2Enabled,sizeof(BOOLEAN_t));
   if(!puschCfg->choice.setup->ext1->processingType2Enabled)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }
   *(puschCfg->choice.setup->ext1->processingType2Enabled) = PUSCH_PROCESS_TYPE2_ENABLED;
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds UL config
 * @details
 *
 *    Function : BuildUlCfg 
 *
 *    Functionality: Builds UL config in spCellCfgDed
 *
 * @params[in] UplinkConfig_t *ulCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildUlCfg(UplinkConfig_t *ulCfg)
{
   ulCfg->initialUplinkBWP = NULLP;
   CU_ALLOC(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
	if(!ulCfg->initialUplinkBWP)
	{
	   DU_LOG("\nF1AP : Memory Allocation failed in BuildUlCfg");
		return RFAILED;
	}

   if(BuildInitialUlBWP(ulCfg->initialUplinkBWP) != ROK)
	{
	   return RFAILED;
	}

   ulCfg->uplinkBWP_ToReleaseList = NULLP;
	ulCfg->uplinkBWP_ToAddModList = NULLP;
	ulCfg->firstActiveUplinkBWP_Id = NULLP;
   CU_ALLOC(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
   if(!ulCfg->firstActiveUplinkBWP_Id)
   {
      DU_LOG("\nF1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }
   *(ulCfg->firstActiveUplinkBWP_Id) = ACTIVE_UL_BWP_ID;

   ulCfg->pusch_ServingCellConfig = NULLP;
   CU_ALLOC(ulCfg->pusch_ServingCellConfig, \
      sizeof(struct UplinkConfig__pusch_ServingCellConfig));
   if(!ulCfg->pusch_ServingCellConfig)
   {
      DU_LOG("\nF1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }

   if(BuildPuschSrvCellCfg(ulCfg->pusch_ServingCellConfig) != ROK)
   {
      return RFAILED;
   }
   
	ulCfg->carrierSwitching = NULLP;
	ulCfg->ext1 = NULLP;
	return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH serving cell config
 * @details
 *
 *    Function : BuildPdschSrvCellCfg
 *
 *    Functionality: Builds PDSCH serving cell config in spCellCfgDed
 *
 * @params[in] struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPdschSrvCellCfg(struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg)
{
   pdschCfg->present =  ServingCellConfig__pdsch_ServingCellConfig_PR_setup;
   pdschCfg->choice.setup = NULLP;
   CU_ALLOC(pdschCfg->choice.setup, sizeof( struct PDSCH_ServingCellConfig));
   if(!pdschCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }

   pdschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
	pdschCfg->choice.setup->xOverhead = NULLP;
   pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH = NULLP;
   CU_ALLOC(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH, sizeof(long));
   if(!pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }
   *(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH)= PDSCH_NUM_HARQ_PROC;
	pdschCfg->choice.setup->pucch_Cell = NULLP;
	pdschCfg->choice.setup->ext1 = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds CSI Meas config
 * @details
 *
 *    Function : BuildCsiMeasCfg 
 *
 *    Functionality: Builds CSI Meas config in spCellCfgDed
 *
 * @params[in] struct ServingCellConfig__csi_MeasConfig *csiMeasCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildCsiMeasCfg(struct ServingCellConfig__csi_MeasConfig *csiMeasCfg)
{

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Spcell config dedicated
 * @details
 *
 *    Function : BuildSpCellCfgDed
 *
 *    Functionality: Builds sp cell config dedicated in spCellCfg
 *
 * @params[in] ServingCellConfig_t srvCellCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSpCellCfgDed(ServingCellConfig_t *srvCellCfg)
{
   srvCellCfg->tdd_UL_DL_ConfigurationDedicated = NULLP;

   srvCellCfg->initialDownlinkBWP = NULLP;
   CU_ALLOC(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
	if(!srvCellCfg->initialDownlinkBWP)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
		return RFAILED;
	}

   if(BuildInitialDlBWP(srvCellCfg->initialDownlinkBWP) != ROK)
	{
		DU_LOG("\nF1AP : BuildInitialDlBWP failed");
		return RFAILED;
	}
   srvCellCfg->downlinkBWP_ToReleaseList = NULLP;
	srvCellCfg->downlinkBWP_ToAddModList = NULLP;

   srvCellCfg->firstActiveDownlinkBWP_Id = NULLP;
	CU_ALLOC(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));
	if(!srvCellCfg->firstActiveDownlinkBWP_Id)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
		return RFAILED;
	}
	*(srvCellCfg->firstActiveDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;

	srvCellCfg->bwp_InactivityTimer = NULLP;

   srvCellCfg->defaultDownlinkBWP_Id = NULLP;
	CU_ALLOC(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));
	if(!srvCellCfg->defaultDownlinkBWP_Id)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
		return RFAILED;
	}
	*(srvCellCfg->defaultDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;

   srvCellCfg->uplinkConfig = NULLP;
   CU_ALLOC(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));
	if(!srvCellCfg->uplinkConfig)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
		return RFAILED;
	}

   if(BuildUlCfg(srvCellCfg->uplinkConfig) != ROK)
	{
		DU_LOG("\nF1AP : BuildUlCfg failed");
		return RFAILED;
	}
	srvCellCfg->supplementaryUplink = NULLP;
	srvCellCfg->pdcch_ServingCellConfig = NULLP;

   srvCellCfg->pdsch_ServingCellConfig = NULLP;
	CU_ALLOC(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct	ServingCellConfig__pdsch_ServingCellConfig));
	if(!srvCellCfg->pdsch_ServingCellConfig)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
		return RFAILED;
	}

   if(BuildPdschSrvCellCfg(srvCellCfg->pdsch_ServingCellConfig) != ROK)
	{
		DU_LOG("\nF1AP : BuildPdschSrvCellCfg failed");
		return RFAILED;
	}

   srvCellCfg->csi_MeasConfig = NULLP;
#if 0
	CU_ALLOC(srvCellCfg->csi_MeasConfig, sizeof(struct	ServingCellConfig__csi_MeasConfig))
	if(!srvCellCfg->csi_MeasConfig)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
		return RFAILED;
	}

   if(BuildCsiMeasCfg(srvCellCfg->csi_MeasConfig) != ROK)
	{
		DU_LOG("\nF1AP : BuildCsiMeasCfg failed");
		return RFAILED;
	}
#endif
   srvCellCfg->sCellDeactivationTimer = NULLP;
	srvCellCfg->crossCarrierSchedulingConfig = NULLP;
	srvCellCfg->tag_Id = TAG_ID;
   srvCellCfg->dummy = NULLP;
   srvCellCfg->pathlossReferenceLinking = NULLP;
   srvCellCfg->servingCellMO = NULLP;
   srvCellCfg->ext1 = NULLP;

	return ROK;
}
/*******************************************************************
 *
 * @brief Builds Spcell config 
 *
 * @details
 *
 *    Function : BuildSpCellCfg 
 *
 *    Functionality: Builds sp cell config in DuToCuRrcContainer
 *
 * @params[in] SpCellConfig_t spCellCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSpCellCfg(SpCellConfig_t *spCellCfg)
{

   spCellCfg->servCellIndex = NULLP;
	CU_ALLOC(spCellCfg->servCellIndex, sizeof(long));
	if(!spCellCfg->servCellIndex)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfg");
		return RFAILED;
	}
   *(spCellCfg->servCellIndex) = SERV_CELL_IDX;

   spCellCfg->reconfigurationWithSync = NULLP;
	spCellCfg->rlf_TimersAndConstants = NULLP;
   spCellCfg->rlmInSyncOutOfSyncThreshold = NULLP;
	CU_ALLOC(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));
	if(!spCellCfg->rlmInSyncOutOfSyncThreshold)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfg");
		return RFAILED;
	}
   *(spCellCfg->rlmInSyncOutOfSyncThreshold) = RLM_SYNC_OUT_SYNC_THRESHOLD;

   spCellCfg->spCellConfigDedicated = NULLP;
   CU_ALLOC(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
	if(!spCellCfg->spCellConfigDedicated)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfg");
		return RFAILED;
	}
   if(BuildSpCellCfgDed(spCellCfg->spCellConfigDedicated) != ROK)
	{
		DU_LOG("\nF1AP : BuildSpCellCfgDed failed");
		return RFAILED;
	}
	return ROK;
}
/*******************************************************************
*
* @brief Builds Phy cell group config 
*
* @details
*
*    Function : BuildPhyCellGrpCfg 
*
*    Functionality: Builds Phy cell group config in DuToCuRrcContainer
*
* @params[in] PhysicalCellGroupConfig_t *phyCellGrpCfg 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildPhyCellGrpCfg(PhysicalCellGroupConfig_t *phyCellGrpCfg)
{
   phyCellGrpCfg->harq_ACK_SpatialBundlingPUCCH = NULLP;
	phyCellGrpCfg->harq_ACK_SpatialBundlingPUSCH = NULLP;

   phyCellGrpCfg->p_NR_FR1 = NULLP;
	CU_ALLOC(phyCellGrpCfg->p_NR_FR1, sizeof(long));
	if(!phyCellGrpCfg->p_NR_FR1)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildPhyCellGrpCfg");
		return RFAILED;
	}
   *(phyCellGrpCfg->p_NR_FR1)             = P_NR_FR1;
	phyCellGrpCfg->pdsch_HARQ_ACK_Codebook = PDSCH_HARQ_ACK_CODEBOOK;
	phyCellGrpCfg->tpc_SRS_RNTI = NULLP;
	phyCellGrpCfg->tpc_PUCCH_RNTI = NULLP;
	phyCellGrpCfg->tpc_PUSCH_RNTI = NULLP;
	phyCellGrpCfg->sp_CSI_RNTI = NULLP;
	phyCellGrpCfg->cs_RNTI = NULLP;
	phyCellGrpCfg->ext1 = NULLP;
	phyCellGrpCfg->ext2 = NULLP;

	return ROK;
}

/*******************************************************************
*
* @brief Builds tag config 
*
* @details
*
*    Function : BuildTagConfig 
*
*    Functionality: Builds tag config in MacCellGroupConfig
*
* @params[in] TAG_Config *tag_Config
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildTagConfig(struct TAG_Config *tagConfig)
{
	struct TAG_Config__tag_ToAddModList *tagList;
   uint8_t                     idx, elementCnt;

   tagConfig->tag_ToReleaseList = NULLP;
   tagConfig->tag_ToAddModList = NULLP;
   CU_ALLOC(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
	if(!tagConfig->tag_ToAddModList)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildTagConfig");
		return RFAILED;
	}

	elementCnt = 1; //ODU_VALUE_ONE;
	tagList = tagConfig->tag_ToAddModList;
	tagList->list.count = elementCnt;
	tagList->list.size  =  elementCnt * sizeof(struct TAG *);

   tagList->list.array = NULLP;
	CU_ALLOC(tagList->list.array, tagList->list.size);
	if(!tagList->list.array)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildTagConfig");
		return RFAILED;
	}

	for(idx=0; idx<tagList->list.count; idx++)
	{
	   tagList->list.array[idx] = NULLP;
		CU_ALLOC(tagList->list.array[idx], sizeof(struct TAG));
		if(!tagList->list.array[idx])
		{
			DU_LOG("\nF1AP : Memory allocation failure in BuildTagConfig");
			return RFAILED;
		}
	}

	idx = 0;
	tagList->list.array[idx]->tag_Id = TAG_ID;
	tagList->list.array[idx]->timeAlignmentTimer = TIME_ALIGNMENT_TMR;

   return ROK;
}

/*******************************************************************
*
* @brief Builds PHR Config 
*
* @details
*
*    Function : BuildPhrConfig
*
*    Functionality: Builds phrConfig in MacCellGroupConfig
*
* @params[in] PHR Config *
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildPhrConfig(struct MAC_CellGroupConfig__phr_Config *phrConfig)
{

   phrConfig->present = MAC_CellGroupConfig__phr_Config_PR_setup;
	phrConfig->choice.setup = NULLP;
	CU_ALLOC(phrConfig->choice.setup, sizeof(struct PHR_Config));
	if(!phrConfig->choice.setup)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildPhrConfig");
		return RFAILED;
	}

	phrConfig->choice.setup->phr_PeriodicTimer        = PHR_PERIODIC_TMR;
	phrConfig->choice.setup->phr_ProhibitTimer        = PHR_PROHIBHIT_TMR;
   phrConfig->choice.setup->phr_Tx_PowerFactorChange = PHR_PWR_FACTOR_CHANGE;
	phrConfig->choice.setup->multiplePHR              = false;
	phrConfig->choice.setup->dummy                    = false;
	phrConfig->choice.setup->phr_Type2OtherCell       = false;
	phrConfig->choice.setup->phr_ModeOtherCG          = PHR_MODE_OTHER_CG;

	return ROK;
}

/*******************************************************************
*
* @brief Builds BSR Config 
*
* @details
*
*    Function : BuildBsrConfig
*
*    Functionality: Builds BuildBsrConfig in MacCellGroupConfig
*
* @params[in] BSR_Config *bsrConfig
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildBsrConfig(struct BSR_Config *bsrConfig)
{
   bsrConfig->periodicBSR_Timer = PERIODIC_BSR_TMR;
	bsrConfig->retxBSR_Timer     = RETX_BSR_TMR;
	bsrConfig->logicalChannelSR_DelayTimer = NULLP;

   return ROK;
}

/*******************************************************************
*
* @brief Builds scheduling request config 
*
* @details
*
*    Function : BuildSchedulingReqConfig 
*
*    Functionality: Builds BuildSchedulingReqConfig in MacCellGroupConfig
*
* @params[in] SchedulingRequestConfig *schedulingRequestConfig
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildSchedulingReqConfig(struct SchedulingRequestConfig *schedulingRequestConfig)
{
	struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList;
   uint8_t                     idx, elementCnt;

   schedulingRequestConfig->schedulingRequestToAddModList = NULLP;
	CU_ALLOC(schedulingRequestConfig->schedulingRequestToAddModList,
			sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));
	if(!schedulingRequestConfig->schedulingRequestToAddModList)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
		return RFAILED;
	}

	elementCnt = 1; //ODU_VALUE_ONE;
	schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
	schReqList->list.count = elementCnt;
   schReqList->list.size  = elementCnt * sizeof(struct SchedulingRequestToAddMod *);

   schReqList->list.array = NULLP;
   CU_ALLOC(schReqList->list.array, schReqList->list.size);
	if(!schReqList->list.array)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
		return RFAILED;
	}

   for(idx=0;idx<schReqList->list.count; idx++)
	{
	   schReqList->list.array[idx] = NULLP;
		CU_ALLOC(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
		if(!schReqList->list.array[idx])
		{
			DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
			return RFAILED;
		}
	}

	idx = 0;
	schReqList->list.array[idx]->schedulingRequestId = SCH_REQ_ID;

   schReqList->list.array[idx]->sr_ProhibitTimer = NULLP;
	CU_ALLOC(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
	if(!schReqList->list.array[idx]->sr_ProhibitTimer)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
		return RFAILED;
	}
   *(schReqList->list.array[idx]->sr_ProhibitTimer) = SR_PROHIBIT_TMR;
	schReqList->list.array[idx]->sr_TransMax = SR_TRANS_MAX;
	schedulingRequestConfig->schedulingRequestToReleaseList = NULLP;

	return ROK;
}
/*******************************************************************
*
* @brief Builds Mac cell group config 
*
* @details
*
*    Function : BuildMacCellGrpCfg 
*
*    Functionality: Builds Mac cell group config in DuToCuRrcContainer
*
* @params[in] MAC_CellGroupConfig_t *macCellGrpCfg
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildMacCellGrpCfg(MAC_CellGroupConfig_t *macCellGrpCfg)
{
   macCellGrpCfg->drx_Config = NULLP;
   macCellGrpCfg->schedulingRequestConfig = NULLP;
	CU_ALLOC(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
	if(!macCellGrpCfg->schedulingRequestConfig)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
		return RFAILED;
	}

	if(BuildSchedulingReqConfig(macCellGrpCfg->schedulingRequestConfig) != ROK)
	{
	   DU_LOG("\nF1AP : BuildSchedulingReqConfig failed");
		return RFAILED;
	}

   macCellGrpCfg->bsr_Config = NULLP;
   CU_ALLOC(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
	if(!macCellGrpCfg->bsr_Config)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
		return RFAILED;
	}

   if(BuildBsrConfig(macCellGrpCfg->bsr_Config) != ROK)
	{
      DU_LOG("\nF1AP : BuildBsrConfig failed");
	   return RFAILED;
	}

   macCellGrpCfg->tag_Config = NULLP;
   CU_ALLOC(macCellGrpCfg->tag_Config, sizeof(struct TAG_Config));
	if(!macCellGrpCfg->tag_Config)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
		return RFAILED;
	}

   if(BuildTagConfig(macCellGrpCfg->tag_Config) != ROK)
	{
	   DU_LOG("\nF1AP : BuildTagConfig failed");
	   return RFAILED;
   }

   macCellGrpCfg->phr_Config = NULLP;
   CU_ALLOC(macCellGrpCfg->phr_Config, sizeof(struct MAC_CellGroupConfig__phr_Config));
	if(!macCellGrpCfg->phr_Config)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
		return RFAILED;
	}

   if(BuildPhrConfig(macCellGrpCfg->phr_Config) != ROK)
	{
	   DU_LOG("\nF1AP : BuildPhrConfig failed");
	   return RFAILED;
   }

   macCellGrpCfg->skipUplinkTxDynamic = false;
	macCellGrpCfg->ext1 = NULLP;

	return ROK;
}
 /*******************************************************************
 *
 * @brief Frees memeory allocated for SearchSpcToAddModList
 *
 * @details
 *
 *    Function : FreeSearchSpcToAddModList
 *
 *    Functionality: Deallocating memory of SearchSpcToAddModList
 *
 * @params[in] struct PDCCH_Config__searchSpacesToAddModList *searchSpcList
 *
 * @return void
 *
 4221 * ****************************************************************/
void FreeSearchSpcToAddModList(struct PDCCH_Config__searchSpacesToAddModList *searchSpcList)
{
    uint8_t idx1=0;
    uint8_t idx2=0;
    struct  SearchSpace *searchSpc=NULLP;

    if(searchSpcList->list.array)
	 {
	    if(searchSpcList->list.array[idx2])
	    {
          searchSpc = searchSpcList->list.array[idx2];
	       if(searchSpc->controlResourceSetId)
	       {
	          if(searchSpc->monitoringSlotPeriodicityAndOffset)
	          {
	             if(searchSpc->monitoringSymbolsWithinSlot)
	             {
	                if(searchSpc->monitoringSymbolsWithinSlot->buf)
	                {
	                   if(searchSpc->nrofCandidates)
	                   {
	                      if(searchSpc->searchSpaceType)
	                      {
	                         CU_FREE(searchSpc->searchSpaceType->choice.ue_Specific,\
	                         sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
	                      	 CU_FREE(searchSpc->searchSpaceType, sizeof(struct
									 SearchSpace__searchSpaceType));
	                      }
	                      CU_FREE(searchSpc->nrofCandidates,
	                      sizeof(struct SearchSpace__nrofCandidates));
	                   }
	                   CU_FREE(searchSpc->monitoringSymbolsWithinSlot->buf, \
	                   searchSpc->monitoringSymbolsWithinSlot->size);
	                }
	                CU_FREE(searchSpc->monitoringSymbolsWithinSlot,
	                sizeof(BIT_STRING_t));
	             }
	             CU_FREE(searchSpc->monitoringSlotPeriodicityAndOffset, \
	             sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
	          }
	          CU_FREE(searchSpc->controlResourceSetId,
	          sizeof(ControlResourceSetId_t));
		     }
		  }
		  for(idx1 = 0; idx1 < searchSpcList->list.count; idx1++)
	     {
	         CU_FREE(searchSpcList->list.array[idx1],
				sizeof(struct SearchSpace));
		  }
		  CU_FREE(searchSpcList->list.array,searchSpcList->list.size);
	 }
}
 /*******************************************************************
 *
 * @brief Frees memory allocated for PdschTimeDomAllocList
 *
 * @details
 *
 *    Function : FreePdschTimeDomAllocList
 *
 *    Functionality: Deallocating memory of PdschTimeDomAllocList
 *
 * @params[in] struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return void
 *
 4221 * ****************************************************************/
void FreePdschTimeDomAllocList( struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList)
{
   uint8_t idx1=0;
	
	if(timeDomAllocList->choice.setup)
   {
      if(timeDomAllocList->choice.setup->list.array)
      {
          for(idx1 = 0; idx1 <timeDomAllocList->choice.setup->list.count ; idx1++)
          {
             CU_FREE(timeDomAllocList->choice.setup->list.array[idx1],
             sizeof(struct PDSCH_TimeDomainResourceAllocation));
          }
          CU_FREE(timeDomAllocList->choice.setup->list.array, \
          timeDomAllocList->choice.setup->list.size);
      }
      CU_FREE(timeDomAllocList->choice.setup,\
      sizeof(struct PDSCH_TimeDomainResourceAllocationList));
   }
}
 /*******************************************************************
 *
 * @brief Frees memory allocated for PuschTimeDomAllocList
 *
 *@details
 *
 *    Function : FreePuschTimeDomAllocList
 *
 *    Functionality: Deallocating memory of PuschTimeDomAllocList
 *
 * @params[in] PUSCH_Config_t *puschCfg
 *
 * @return void
 *
 ***********************************************************************/
void FreePuschTimeDomAllocList(PUSCH_Config_t *puschCfg)
{
    uint8_t idx1=0;
	 uint8_t idx2=0;
    struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList_t=NULLP;

    if(puschCfg->pusch_TimeDomainAllocationList)
    {
       timeDomAllocList_t=puschCfg->pusch_TimeDomainAllocationList;
       if(timeDomAllocList_t->choice.setup)
       {
          if(timeDomAllocList_t->choice.setup->list.array)
          {
             CU_FREE(timeDomAllocList_t->choice.setup->list.array[idx2]->k2, sizeof(long));
             for(idx1 = 0; idx1<timeDomAllocList_t->choice.setup->list.count; idx1++)
             {
                CU_FREE(timeDomAllocList_t->choice.setup->list.array[idx1],\
				    sizeof(PUSCH_TimeDomainResourceAllocation_t));
				 }
				 CU_FREE(timeDomAllocList_t->choice.setup->list.array, \
             timeDomAllocList_t->choice.setup->list.size);
          }
          CU_FREE(timeDomAllocList_t->choice.setup, \
	       sizeof(struct PUSCH_TimeDomainResourceAllocationList));
       }
		 CU_FREE(puschCfg->transformPrecoder, sizeof(long));
       CU_FREE(puschCfg->pusch_TimeDomainAllocationList, \
		 sizeof(struct PUSCH_Config__pusch_TimeDomainAllocationList));
    }

}
 /*******************************************************************
 *
 * @brief Frees memory allocated for InitialUlBWP
 *
 * @details
 *
 *    Function : FreeInitialUlBWP
 *
 *    Functionality: Deallocating memory of InitialUlBWP
 *
 * @params[in] BWP_UplinkDedicated_t *ulBwp
 *
 * @return void
 *
 * ****************************************************************/
void FreeInitialUlBWP(BWP_UplinkDedicated_t *ulBwp)
{
    uint8_t  rSetIdx, rsrcIdx;
    SRS_Config_t   *srsCfg = NULLP;
    PUSCH_Config_t *puschCfg = NULLP;
    struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg = NULLP;
	 struct SRS_Config__srs_ResourceSetToAddModList *rsrcSetList = NULLP;
	 struct SRS_ResourceSet__srs_ResourceIdList *rsrcIdList = NULLP;
	 struct SRS_Config__srs_ResourceToAddModList *resourceList = NULLP;

	 if(ulBwp->pusch_Config)
    {
       if(ulBwp->pusch_Config->choice.setup)
       {
          puschCfg=ulBwp->pusch_Config->choice.setup;
          if(puschCfg->dataScramblingIdentityPUSCH)
          {
             if(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
             {
                FreePuschTimeDomAllocList(puschCfg);
                dmrsUlCfg=puschCfg->dmrs_UplinkForPUSCH_MappingTypeA;
					 if(dmrsUlCfg->choice.setup)
                {
                   if(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
                   {
                      if(dmrsUlCfg->choice.setup->transformPrecodingDisabled)
                      {
                         CU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
								 sizeof(long));
								 CU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled,
                         sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
							 }
							 CU_FREE(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition,
							 sizeof(long));
						 }
						 CU_FREE(dmrsUlCfg->choice.setup,sizeof(DMRS_UplinkConfig_t));
                }
					 CU_FREE(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, \
					 sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
				 }
             CU_FREE(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
          }
          CU_FREE(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
	    }
		 CU_FREE(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));

		 /* Free SRS-Config */
		 if(ulBwp->srs_Config)
		 {
		    if(ulBwp->srs_Config->choice.setup)
			 {
			    srsCfg = ulBwp->srs_Config->choice.setup;

				 /* Free Resource Set to add/mod list */
			    if(srsCfg->srs_ResourceSetToAddModList)
				 {
				    rsrcSetList = srsCfg->srs_ResourceSetToAddModList;
					 if(rsrcSetList->list.array)
					 {
					    rSetIdx = 0;

						 /* Free SRS resource Id list in this SRS resource set */
						 if(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList)
						 {
						    rsrcIdList = rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList;

						    if(rsrcIdList->list.array)
							 {
							    for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count; rsrcIdx++)
								 {
								    CU_FREE(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
							    }
							    CU_FREE(rsrcIdList->list.array, rsrcIdList->list.size);
							 }
						    CU_FREE(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
							    sizeof(struct SRS_ResourceSet__srs_ResourceIdList));
						 }

						 /* Free resource type info for this SRS resource set */
						 CU_FREE(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
						    sizeof(struct SRS_ResourceSet__resourceType__aperiodic));

						 /* Free memory for each resource set */
					    for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
						 {
						    CU_FREE(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
						 }
					    CU_FREE(rsrcSetList->list.array, rsrcSetList->list.size); 
					 }
				    CU_FREE(srsCfg->srs_ResourceSetToAddModList, \
					    sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
				 }

				 /* Free resource to add/modd list */
				 if(srsCfg->srs_ResourceToAddModList)
				 {
				    resourceList = srsCfg->srs_ResourceToAddModList;
					 if(resourceList->list.array)
					 {
					    rsrcIdx = 0;
						 CU_FREE(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2,\
						    sizeof(struct SRS_Resource__transmissionComb__n2));
					    CU_FREE(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,\
						    sizeof(struct SRS_Resource__resourceType__aperiodic));

					    for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
						 {
						    CU_FREE(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
                   }
					    CU_FREE(resourceList->list.array, resourceList->list.size);
					 }
				    CU_FREE(srsCfg->srs_ResourceToAddModList, \
					    sizeof(struct SRS_Config__srs_ResourceToAddModList));
				 }

			    CU_FREE(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
			 }
		    CU_FREE(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
       }
	 }
}	
 /*******************************************************************
 *
 * @brief Frees memory allocated for initialUplinkBWP
 *
 * @details
 *
 *    Function : FreeinitialUplinkBWP
 *
 *    Functionality: Deallocating memory of initialUplinkBWP
 *
 * @params[in] UplinkConfig_t *ulCfg
 *
 * @return void
 *         
 *
 * ****************************************************************/
void FreeinitialUplinkBWP(UplinkConfig_t *ulCfg)
{
   BWP_UplinkDedicated_t *ulBwp=NULLP; 
   struct UplinkConfig__pusch_ServingCellConfig *puschCfg=NULLP;
   
	if(ulCfg->initialUplinkBWP)
	{
	   ulBwp=ulCfg->initialUplinkBWP;
	   if(ulCfg->firstActiveUplinkBWP_Id)
	   {
	      if(ulCfg->pusch_ServingCellConfig)
	      {
	         puschCfg=ulCfg->pusch_ServingCellConfig;
	         if(puschCfg->choice.setup)
	         {
	            if(puschCfg->choice.setup->ext1)
	            {
	                CU_FREE(puschCfg->choice.setup->ext1->\
						 processingType2Enabled,sizeof(BOOLEAN_t));
	                CU_FREE(puschCfg->choice.setup->ext1->\
						 maxMIMO_Layers,sizeof(long));
						 CU_FREE(puschCfg->choice.setup->ext1, \
						 sizeof(struct PUSCH_ServingCellConfig__ext1));
					}
	            CU_FREE(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
	         }
	         CU_FREE(ulCfg->pusch_ServingCellConfig, sizeof(struct UplinkConfig__pusch_ServingCellConfig));
	      }
	      CU_FREE(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
	   }
	   FreeInitialUlBWP(ulBwp);
	   CU_FREE(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
	}
}
 /*******************************************************************
 *
 * @brief Frees emmory allocated for BWPDlDedPdschCfg
 *
 * @details
 *
 *    Function : FreeBWPDlDedPdschCfg
 *
 *    Functionality: Deallocating memory of BWPDlDedPdschCfg
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return void
 *
 *
 * ****************************************************************/
void FreeBWPDlDedPdschCfg(BWP_DownlinkDedicated_t *dlBwp)
{
   struct PDSCH_Config *pdschCfg=NULLP;
	struct PDSCH_Config__prb_BundlingType *prbBndlType=NULLP;
   struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList=NULLP;
   struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg=NULLP;

	if(dlBwp->pdsch_Config->choice.setup)
   {
      pdschCfg=dlBwp->pdsch_Config->choice.setup;
      if(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
      {
          if(pdschCfg->pdsch_TimeDomainAllocationList)
          {
             timeDomAllocList=pdschCfg->pdsch_TimeDomainAllocationList;
             if(pdschCfg->maxNrofCodeWordsScheduledByDCI)
             {
                prbBndlType=&pdschCfg->prb_BundlingType;
                CU_FREE(prbBndlType->choice.staticBundling,\
                sizeof(struct PDSCH_Config__prb_BundlingType__staticBundling));
                CU_FREE(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
             }
             FreePdschTimeDomAllocList(timeDomAllocList);
             CU_FREE(pdschCfg->pdsch_TimeDomainAllocationList, \
             sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
          }
          dmrsDlCfg=pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA;
          if(dmrsDlCfg->choice.setup)
          {
             CU_FREE(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition,
             sizeof(long));
             CU_FREE(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
          }
          CU_FREE(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, \
          sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
       }
       CU_FREE(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
    }
}
 /*******************************************************************
 *
 * @brief Frees emmory allocated for BWPDlDedPdcchCfg
 *
 * @details
 *
 *    Function : FreeBWPDlDedPdcchCfg
 *
 *    Functionality: Deallocating memory of BWPDlDedPdcchCfg
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return void
 *         
 *
 * ****************************************************************/
void FreeBWPDlDedPdcchCfg(BWP_DownlinkDedicated_t *dlBwp)
{
   uint8_t idx1=0;
	uint8_t idx2=0;
	struct PDCCH_Config *pdcchCfg=NULLP;
   struct ControlResourceSet *controlRSet=NULLP;
	struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList=NULLP;

	if(dlBwp->pdcch_Config->choice.setup)
   {
      pdcchCfg=dlBwp->pdcch_Config->choice.setup;
      if(pdcchCfg->controlResourceSetToAddModList)
      {
         controlRSetList = pdcchCfg->controlResourceSetToAddModList;
         if(controlRSetList->list.array)
         {
            controlRSet = controlRSetList->list.array[idx2];
            if(controlRSet)
            {
               if(controlRSet->frequencyDomainResources.buf)
               {
                  if(controlRSet->pdcch_DMRS_ScramblingID)
                  {
                     if(pdcchCfg->searchSpacesToAddModList)
                     {
                         FreeSearchSpcToAddModList(pdcchCfg->searchSpacesToAddModList);
                         CU_FREE(pdcchCfg->searchSpacesToAddModList, \
                         sizeof(struct PDCCH_Config__searchSpacesToAddModList));
                     }
                     CU_FREE(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
                  }
						CU_FREE(controlRSet->frequencyDomainResources.buf, \
						controlRSet->frequencyDomainResources.size);
					}
				}
            for(idx1 = 0; idx1 <controlRSetList->list.count; idx1++)
            {
               CU_FREE(controlRSetList->list.array[idx1], sizeof(struct ControlResourceSet));
            }
				CU_FREE(controlRSetList->list.array, controlRSetList->list.size);
			}
			CU_FREE(pdcchCfg->controlResourceSetToAddModList, \
			sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
		}
		CU_FREE(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
	}
}
/*******************************************************************
*
* @brief Builds RLC Config
*
* @details
*
*    Function : BuildRlcConfig
*
*    Functionality: Builds RLC Config in BuildRlcBearerToAddModList 
*
* @params[in] RLC_Config *rlcConfig
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildRlcConfig(struct RLC_Config *rlcConfig)
{

   rlcConfig->present = RLC_Config_PR_am;

   rlcConfig->choice.am = NULLP;
   CU_ALLOC(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
	if(!rlcConfig->choice.am)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildRlcConfig");
		return RFAILED;
	}

   /* UL */
	rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength = NULLP;
   CU_ALLOC(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
	if(!rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildRlcConfig");
		return RFAILED;
	}
   *(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength) = SN_FIELD_LEN;
	rlcConfig->choice.am->ul_AM_RLC.t_PollRetransmit  = T_POLL_RETRANSMIT;
	rlcConfig->choice.am->ul_AM_RLC.pollPDU           = POLL_PDU;
	rlcConfig->choice.am->ul_AM_RLC.pollByte          = POLL_BYTE;
	rlcConfig->choice.am->ul_AM_RLC.maxRetxThreshold  = MAX_RETX_THRESHOLD;

   /* DL */
   rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength = NULLP;
   CU_ALLOC(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
	if(!rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildRlcConfig");
		return RFAILED;
	}
	*(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength) = SN_FIELD_LEN;
   rlcConfig->choice.am->dl_AM_RLC.t_Reassembly      = T_REASSEMBLY;
	rlcConfig->choice.am->dl_AM_RLC.t_StatusProhibit  = T_STATUS_PROHIBHIT;
	
   return ROK;
}

/*******************************************************************
*
* @brief Builds MAC LC Config
*
* @details
*
*    Function : BuildMacLCConfig 
*
*    Functionality: Builds MAC LC Config in BuildRlcBearerToAddModList 
*
* @params[in] struct LogicalChannelConfig macLcConfig
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildMacLCConfig(struct LogicalChannelConfig *macLcConfig)
{

   macLcConfig->ul_SpecificParameters = NULLP;
   CU_ALLOC(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
	if(!macLcConfig->ul_SpecificParameters)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildMacLCConfig");
		return RFAILED;
	}

   macLcConfig->ul_SpecificParameters->priority = MAC_LC_PRIORITY;
	macLcConfig->ul_SpecificParameters->prioritisedBitRate =	PRIORTISIED_BIT_RATE;
	macLcConfig->ul_SpecificParameters->bucketSizeDuration =	BUCKET_SIZE_DURATION;
   macLcConfig->ul_SpecificParameters->allowedServingCells = NULLP;
	macLcConfig->ul_SpecificParameters->allowedSCS_List = NULLP;
	macLcConfig->ul_SpecificParameters->maxPUSCH_Duration = NULLP;
   macLcConfig->ul_SpecificParameters->configuredGrantType1Allowed = NULLP;

   macLcConfig->ul_SpecificParameters->logicalChannelGroup = NULLP;
   CU_ALLOC(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
   if(!macLcConfig->ul_SpecificParameters->logicalChannelGroup)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildMacLCConfig");
		return RFAILED;
	}
   *(macLcConfig->ul_SpecificParameters->logicalChannelGroup) = LC_GRP;

   macLcConfig->ul_SpecificParameters->schedulingRequestID = NULLP;
	CU_ALLOC(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
	if(!macLcConfig->ul_SpecificParameters->schedulingRequestID)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildMacLCConfig");
		return RFAILED;
	}
   *(macLcConfig->ul_SpecificParameters->schedulingRequestID) = SCH_REQ_ID;

	macLcConfig->ul_SpecificParameters->logicalChannelSR_Mask = false;
	macLcConfig->ul_SpecificParameters->logicalChannelSR_DelayTimerApplied = false;
	macLcConfig->ul_SpecificParameters->bitRateQueryProhibitTimer = NULLP;

   return ROK;
}
/*******************************************************************
*
* @brief Builds RLC Bearer to Add/Mod list
*
* @details
*
*    Function :BuildRlcBearerToAddModList 
*
*    Functionality: Builds RLC Bearer to Add/Mod list in DuToCuRrcContainer
*
* @params[in] rlc_BearerToAddModList
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildRlcBearerToAddModList(struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList)
{
   uint8_t                     idx, elementCnt;

   elementCnt = 1;
   rlcBearerList->list.count = elementCnt;
	rlcBearerList->list.size  = elementCnt * sizeof(struct RLC_BearerConfig *);

   rlcBearerList->list.array = NULLP;
	CU_ALLOC(rlcBearerList->list.array, rlcBearerList->list.size);
	if(!rlcBearerList->list.array)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
		return RFAILED;
	}

   for(idx=0; idx<rlcBearerList->list.count; idx++)
	{
	   rlcBearerList->list.array[idx] = NULLP;
		CU_ALLOC(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
		if(!rlcBearerList->list.array[idx])
		{
			DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
			return RFAILED;
		}
	}

   idx = 0;
	rlcBearerList->list.array[idx]->logicalChannelIdentity = RLC_LCID;

	CU_ALLOC(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
	if(!rlcBearerList->list.array[idx]->servedRadioBearer)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
		return RFAILED;
	}

	rlcBearerList->list.array[idx]->servedRadioBearer->present = RLC_BearerConfig__servedRadioBearer_PR_srb_Identity;
   rlcBearerList->list.array[idx]->servedRadioBearer->choice.srb_Identity = SRB_ID_1;

   rlcBearerList->list.array[idx]->reestablishRLC = NULLP;
   rlcBearerList->list.array[idx]->rlc_Config = NULLP;
   CU_ALLOC(rlcBearerList->list.array[idx]->rlc_Config, sizeof(struct RLC_Config));
	if(!rlcBearerList->list.array[idx]->rlc_Config)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
		return RFAILED;
	}

   if(BuildRlcConfig(rlcBearerList->list.array[idx]->rlc_Config) != ROK)
	{
		DU_LOG("\nF1AP : BuildRlcConfig failed");
		return RFAILED;
	}

   rlcBearerList->list.array[idx]->mac_LogicalChannelConfig = NULLP;
   CU_ALLOC(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
	if(!rlcBearerList->list.array[idx]->mac_LogicalChannelConfig)
	{
		DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
		return RFAILED;
	}

	if(BuildMacLCConfig(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig) != ROK)
	{
		DU_LOG("\nF1AP : BuildMacLCConfig failed");
		return RFAILED;
	}

	return ROK;
}

/*******************************************************************
 *
 * @brief Free memory allocated for CellGroupConfig 
 *
 * @details
 *
 *    Function : FreeMemCellGrpCfg
 *
 *    Functionality: Deallocating memory of CellGroupConfig
 *
 * @params[in] pointer to CellGroupConfigRrc_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t FreeMemCellGrpCfg(CellGroupConfigRrc_t *cellGrpCfg)
{
   uint8_t idx=0;
   SpCellConfig_t *spCellCfg=NULLP;
   ServingCellConfig_t *srvCellCfg=NULLP;
   BWP_DownlinkDedicated_t *dlBwp=NULLP;
   MAC_CellGroupConfig_t *macCellGrpCfg=NULLP;
   PhysicalCellGroupConfig_t *phyCellGrpCfg=NULLP;
   struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList=NULLP;
   struct RLC_Config *rlcConfig=NULLP;
   struct LogicalChannelConfig *macLcConfig=NULLP;
   struct SchedulingRequestConfig *schedulingRequestConfig=NULLP;
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList=NULLP;
   struct TAG_Config *tagConfig=NULLP;
   struct TAG_Config__tag_ToAddModList *tagList=NULLP;
   struct MAC_CellGroupConfig__phr_Config *phrConfig=NULLP;
   struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg=NULLP;

   rlcBearerList = cellGrpCfg->rlc_BearerToAddModList;
   if(rlcBearerList)
   {
      if(rlcBearerList->list.array)
      {
	 for(idx=0; idx<rlcBearerList->list.count; idx++)
	 {
	    if(rlcBearerList->list.array[idx])
	    {  
	       rlcConfig   = rlcBearerList->list.array[idx]->rlc_Config;
	       macLcConfig = rlcBearerList->list.array[idx]->mac_LogicalChannelConfig;
	       if(rlcConfig)
	       {
		  if(rlcConfig->choice.am)
		  {
		     CU_FREE(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
		     CU_FREE(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
		     CU_FREE(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
		  }	
		  CU_FREE(rlcConfig, sizeof(struct RLC_Config));
	       }
	       CU_FREE(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
	       if(macLcConfig)
	       {
		  if(macLcConfig->ul_SpecificParameters)
		  {
		     CU_FREE(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
		     CU_FREE(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
		     CU_FREE(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
		  }
		  CU_FREE(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
	       }
	       CU_FREE(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
	    }	
	 }
	 CU_FREE(rlcBearerList->list.array, rlcBearerList->list.size);
      }
      CU_FREE(cellGrpCfg->rlc_BearerToAddModList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
   }

   macCellGrpCfg = cellGrpCfg->mac_CellGroupConfig;
   if(macCellGrpCfg)
   {
      schedulingRequestConfig = macCellGrpCfg->schedulingRequestConfig; 
      if(schedulingRequestConfig)
      {
	 schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
	 if(schReqList)
	 {
	    if(schReqList->list.array)
	    {
	       for(idx=0;idx<schReqList->list.count; idx++)
	       {
		  if(schReqList->list.array[idx])
		  {
		     CU_FREE(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
		     CU_FREE(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
		  }
	       }
	       CU_FREE(schReqList->list.array, schReqList->list.size);
	    }
	    CU_FREE(schedulingRequestConfig->schedulingRequestToAddModList,\
		  sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));    }
	    CU_FREE(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
      }
      if(macCellGrpCfg->bsr_Config)
      {
	 CU_FREE(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
      }
      tagConfig = macCellGrpCfg->tag_Config;
      if(tagConfig)
      {
	 tagList = tagConfig->tag_ToAddModList;
	 if(tagList)
	 {
	    if(tagList->list.array)
	    {
	       for(idx=0; idx<tagList->list.count; idx++)
	       {
		  CU_FREE(tagList->list.array[idx], sizeof(struct TAG));
	       }
	       CU_FREE(tagList->list.array, tagList->list.size);
	    }
	    CU_FREE(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
	 }
	 CU_FREE(tagConfig, sizeof(struct TAG_Config));
      }

      phrConfig = macCellGrpCfg->phr_Config;
      if(phrConfig)
      {
	 CU_FREE(phrConfig->choice.setup, sizeof(struct PHR_Config));
	 CU_FREE(phrConfig, sizeof(struct MAC_CellGroupConfig__phr_Config));
      }

      CU_FREE(macCellGrpCfg, sizeof(MAC_CellGroupConfig_t));
   }

   phyCellGrpCfg = cellGrpCfg->physicalCellGroupConfig;
   if(phyCellGrpCfg)
   {
      CU_FREE(phyCellGrpCfg->p_NR_FR1, sizeof(long));
      CU_FREE(phyCellGrpCfg, sizeof(PhysicalCellGroupConfig_t));
   }

   spCellCfg = cellGrpCfg->spCellConfig;
   if(spCellCfg)
   {
      if(spCellCfg->servCellIndex)
      {
	 if(spCellCfg->rlmInSyncOutOfSyncThreshold)
	 {
	    if(spCellCfg->spCellConfigDedicated)
	    {
	       srvCellCfg = spCellCfg->spCellConfigDedicated;
	       if(srvCellCfg->initialDownlinkBWP)
	       {
		  dlBwp = srvCellCfg->initialDownlinkBWP;
		  if(srvCellCfg->firstActiveDownlinkBWP_Id)
		  {
		     if(srvCellCfg->defaultDownlinkBWP_Id)
		     {
			if(srvCellCfg->uplinkConfig)
			{
			   if(srvCellCfg->pdsch_ServingCellConfig)
			   {
			      pdschCfg= srvCellCfg->pdsch_ServingCellConfig;
			      if(pdschCfg->choice.setup)
			      {
				 CU_FREE(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH,sizeof(long));
				 CU_FREE(pdschCfg->choice.setup, sizeof( struct PDSCH_ServingCellConfig));
			      }
			      CU_FREE(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct
				       ServingCellConfig__pdsch_ServingCellConfig));
			   }
			   FreeinitialUplinkBWP(srvCellCfg->uplinkConfig);
			   CU_FREE(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));	
			}
			CU_FREE(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));
		     }
		     CU_FREE(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));
		  }
		  if(dlBwp->pdcch_Config)
		  {
		     if(dlBwp->pdsch_Config)
		     {
			FreeBWPDlDedPdschCfg(dlBwp);
			CU_FREE(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
		     }
		     FreeBWPDlDedPdcchCfg(dlBwp);
		     CU_FREE(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
		  }
		  CU_FREE(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
	       }
	       CU_FREE(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
	    }
	    CU_FREE(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));
	 }
	 CU_FREE(spCellCfg->servCellIndex, sizeof(long));
      }
      CU_FREE(spCellCfg,sizeof(SpCellConfig_t));
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills CellGroupConfig 
 *
 * @details
 *
 *    Function : fillCellGrpCfg
 *
 *    Functionality: Fills CellGroupConfig
 *
 * @params[in] pointer to CellGroupConfigRrc_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillCellGrpCfg(CellGroupConfig_t *cellGrp)
{
   uint8_t               ret = RFAILED;
   CellGroupConfigRrc_t  cellGrpCfg;
   asn_enc_rval_t        encRetVal;

   while(true)
   {
      cellGrpCfg.cellGroupId = CELL_GRP_ID;

      cellGrpCfg.rlc_BearerToAddModList = NULLP;
      CU_ALLOC(cellGrpCfg.rlc_BearerToAddModList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
      if(!cellGrpCfg.rlc_BearerToAddModList)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in CellGrpConfig");
	 break;
      }
      if(BuildRlcBearerToAddModList(cellGrpCfg.rlc_BearerToAddModList) != ROK)
      {
	 DU_LOG("\nF1AP : fillCellGrpCfg failed");
	 break;
      }

      cellGrpCfg.rlc_BearerToReleaseList = NULLP;
      cellGrpCfg.mac_CellGroupConfig = NULLP;
      CU_ALLOC(cellGrpCfg.mac_CellGroupConfig, sizeof(MAC_CellGroupConfig_t));
      if(!cellGrpCfg.mac_CellGroupConfig)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in fillCellGrpCfg");
	 break;
      }
      if(BuildMacCellGrpCfg(cellGrpCfg.mac_CellGroupConfig) != ROK)
      {
	 DU_LOG("\nF1AP : BuildMacCellGrpCfg failed");
	 break;
      }


      cellGrpCfg.physicalCellGroupConfig = NULLP;
      CU_ALLOC(cellGrpCfg.physicalCellGroupConfig, sizeof(PhysicalCellGroupConfig_t));
      if(!cellGrpCfg.physicalCellGroupConfig)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildDuToCuRrcContainer");
	 break;
      }
      if(BuildPhyCellGrpCfg(cellGrpCfg.physicalCellGroupConfig) != ROK)
      {
	 DU_LOG("\nF1AP : BuildPhyCellGrpCfg failed");
	 break;
      }

      cellGrpCfg.spCellConfig = NULLP;
      CU_ALLOC(cellGrpCfg.spCellConfig, sizeof(SpCellConfig_t));
      if(!cellGrpCfg.spCellConfig)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildDuToCuRrcContainer");
	 break;
      }
      if(BuildSpCellCfg(cellGrpCfg.spCellConfig) != ROK)
      {
	 DU_LOG("\nF1AP : BuildSpCellCfg failed");
	 break;
      }

      cellGrpCfg.sCellToAddModList = NULLP;
      cellGrpCfg.sCellToReleaseList = NULLP;
      cellGrpCfg.ext1 = NULLP;

      /* encode cellGrpCfg into duToCuRrcContainer */
      xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, &cellGrpCfg);
      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_CellGroupConfigRrc, 0, &cellGrpCfg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG( "\n F1AP : Could not encode DuToCuRrcContainer (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\n F1AP : Created APER encoded buffer for DuToCuRrcContainer\n");
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
      }

      cellGrp->size = encBufSize;
      CU_ALLOC(cellGrp->buf, cellGrp->size);
      if(!cellGrp->buf)
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildDuToCuRrcContainer");
	 break;
      }
      memcpy(cellGrp->buf, encBuf, cellGrp->size);
      ret = ROK;
      break;
   }
   FreeMemCellGrpCfg(&cellGrpCfg);
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills CuToDuContainer 
 *
 * @details
 *
 *    Function : fillCuToDuContainer
 *
 *    Functionality: Fills CuToDuContainer
 *
 * @params[in] pointer to CUtoDURRCInformation_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillCuToDuContainer(CUtoDURRCInformation_t *rrcMsg)
{
   uint8_t elementCnt = 0;
   uint8_t ret = ROK;
   uint8_t idx, idx2, rrcBufLen;

   elementCnt = 1;
   CU_ALLOC(rrcMsg->iE_Extensions, sizeof(ProtocolExtensionContainer_4624P16_t));
   if(rrcMsg->iE_Extensions)
   {
      rrcMsg->iE_Extensions->list.count = elementCnt;
      rrcMsg->iE_Extensions->list.size = elementCnt * sizeof(CUtoDURRCInformation_ExtIEs_t);

      /* Initialize the CUtoDURRCInformation_ExtIEs */
      CU_ALLOC(rrcMsg->iE_Extensions->list.array, rrcMsg->iE_Extensions->list.size);

      if(rrcMsg->iE_Extensions->list.array == NULLP)
      {
         DU_LOG(" F1AP : Memory allocation for CUtoDURRCInformation_ExtIEs failed");
         ret = RFAILED;
      }

      for(idx=0; idx<elementCnt; idx++)
      {
         CU_ALLOC(rrcMsg->iE_Extensions->list.array[idx], sizeof(CUtoDURRCInformation_ExtIEs_t));
         if(rrcMsg->iE_Extensions->list.array[idx] == NULLP)
         {
            ret = RFAILED;
         }
      }
      idx = 0;
      /* Filling CellGroupConfig_t IE */
      rrcMsg->iE_Extensions->list.array[idx]->id = ProtocolIE_ID_id_CellGroupConfig;
      rrcMsg->iE_Extensions->list.array[idx]->criticality = Criticality_ignore; 
      rrcMsg->iE_Extensions->list.array[idx]->extensionValue.present  =\
      CUtoDURRCInformation_ExtIEs__extensionValue_PR_CellGroupConfig;
      ret = fillCellGrpCfg(&rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig);
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Free CuToDuContainer 
 *
 * @details
 *
 *    Function : FreeCuToDuInfo
 *
 *    Functionality: Free CuToDuContainer
 *
 * @params[in] pointer to CUtoDURRCInformation_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

void FreeCuToDuInfo(CUtoDURRCInformation_t *rrcMsg)
{
   uint8_t idx, idx2;

   if(rrcMsg->iE_Extensions)
   {
      if(rrcMsg->iE_Extensions->list.array)
      {
         for(idx= 0; idx < rrcMsg->iE_Extensions->list.count; idx++)
         {
            if(rrcMsg->iE_Extensions->list.array[idx])
	    {
	       switch(rrcMsg->iE_Extensions->list.array[idx]->id)
	       {
                  case ProtocolIE_ID_id_CellGroupConfig:
                     if(rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig.buf != NULLP)
		     {
                        CU_FREE(rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig.buf,\
			rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig.size);

		     }
                  break;
                  default:
		     DU_LOG("\nF1AP:Invalid Event type %ld at FreeCuToDuInfo()", \
		        rrcMsg->iE_Extensions->list.array[idx]->id);
		     break;
	       }
	    }
	    break;
         }
	 for(idx2 = 0; idx2 < idx; idx2++)
	 {
            CU_FREE(rrcMsg->iE_Extensions->list.array[idx2], sizeof(CUtoDURRCInformation_ExtIEs_t));
	 }
         CU_FREE(rrcMsg->iE_Extensions->list.array, rrcMsg->iE_Extensions->list.size);

      }

      CU_FREE(rrcMsg->iE_Extensions, sizeof(ProtocolExtensionContainer_4624P16_t));
   }
}
/*******************************************************************
 *
 * @brief Builds and sends the UE Setup Request 
 *
 * @details
 *
 *    Function : BuildAndSendUeContextSetupReq
 *
 *    Functionality: Constructs the UE Setup Request and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendUeContextSetupReq(uint8_t cuUeF1apId, uint8_t duUeF1apId, \
   uint16_t rrcContLen, uint8_t *rrcContainer)
{
   uint8_t   Nrcgiret;
   uint8_t   SplCellListret;
   uint8_t   SrbSetupret;
   uint8_t   elementCnt;
   uint8_t   idx;
   uint8_t   idx1;
   F1AP_PDU_t      	*f1apMsg = NULLP;
   UEContextSetupRequest_t *ueSetReq = NULLP;
   asn_enc_rval_t encRetVal;        /* Encoder return value */
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));
   uint8_t ret= RFAILED;
   uint8_t ret1;
   while(true)
   {
      DU_LOG("\n F1AP : Building UE Context Setup Request\n");

      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }

      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      CU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
	 break;
      }

      f1apMsg->choice.initiatingMessage->procedureCode = \
	 ProcedureCode_id_UEContextSetup;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = \
         InitiatingMessage__value_PR_UEContextSetupRequest;
      ueSetReq =
	 &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;

      elementCnt = 11;
      ueSetReq->protocolIEs.list.count = elementCnt;
      ueSetReq->protocolIEs.list.size = \
					elementCnt * sizeof(UEContextSetupRequestIEs_t *);

      /* Initialize the UESetup members */
      CU_ALLOC(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);

      if(ueSetReq->protocolIEs.list.array == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for UE Context SetupRequest failed");
	 break;
      }

      for(idx1=0; idx1<elementCnt; idx1++)
      {
	 CU_ALLOC(ueSetReq->protocolIEs.list.array[idx1],sizeof(UEContextSetupRequestIEs_t));
	 if(ueSetReq->protocolIEs.list.array[idx1] == NULLP)
	 {
	    break;
	 }
      }

      idx = 0;

      /*GNB CU UE F1AP ID*/
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = cuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = duUeF1apId;

      /*Special Cell ID*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_SpCell_ID;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_NRCGI;
      Nrcgiret = BuildNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI);
      if(Nrcgiret != ROK)
      {
	 break;
      }

      /*Served Cell Index*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_ServCellIndex;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_ServCellIndex;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.ServCellIndex = \
									  CELL_INDEX;

      /*CellULConfigured*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_SpCellULConfigured;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_CellULConfigured;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.CellULConfigured = \
									     CellULConfigured_none;

      /*CUtoDURRCContainer*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_CUtoDURRCInformation;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_CUtoDURRCInformation;
      if(fillCuToDuContainer(&ueSetReq->protocolIEs.list.array[idx]->value.choice.CUtoDURRCInformation))
      {
         break;
      }
      
      /*Special Cells to be SetupList*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_SCell_ToBeSetup_List;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_SCell_ToBeSetup_List;
      SplCellListret = BuildSplCellList(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SCell_ToBeSetup_List);
      if(SplCellListret != ROK)
      {  
	 break;
      }
      /*SRBs To Be Setup List*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_SRBs_ToBeSetup_List;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_SRBs_ToBeSetup_List;
      SrbSetupret =	BuildSRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List);
      if(SrbSetupret != ROK)
      {        
	 break;
      }
      /*DRBs to Be Setup List*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_DRBs_ToBeSetup_List;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_DRBs_ToBeSetup_List;
      ret1 = BuildDRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List);
      if(ret1 != ROK)
      {	
	 break;
      }
      
      /* RRC Container */
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_RRCContainer;
      ueSetReq->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
         UEContextSetupRequestIEs__value_PR_RRCContainer;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size = rrcContLen;
      CU_ALLOC(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,
          ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
      if(!ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf)
      {
         DU_LOG(" F1AP : Memory allocation for BuildAndSendUeContextSetupReq failed");
         break;
      }
      memcpy(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf, \
         rrcContainer, ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size); 

      /* RRC delivery status request */
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id = \
         ProtocolIE_ID_id_RRCDeliveryStatusRequest;
      ueSetReq->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
         UEContextSetupRequestIEs__value_PR_RRCDeliveryStatusRequest;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCDeliveryStatusRequest = \
         RRCDeliveryStatusRequest_true;
      
      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	    encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG( "\n F1AP : Could not encode UE Context Setup Request structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\n F1AP : Created APER encoded buffer for UE Context Setup Request\n");
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
      }

      /* Sending  msg  */
      if(SendF1APMsg(CU_APP_MEM_REG,CU_POOL)	!=	ROK)
      {
	 DU_LOG("\n F1AP : Sending UE Context Setup Request Failed");
	 break;
      }
      ret = ROK;
      break;
   }
   FreeUeContextSetupReq(f1apMsg);

   return ret;
}/* End of BuildAndSendUeContextSetupReq*/

/****************************************************************
 * @brief Function to process Ul Rrc Msg received from DU 
 *
 * @details
 *
 *    Function : procUlRrcMsg
 *
 *    Functionality:
 *         - Function to process Ul Rrc Msg received from DU
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procUlRrcMsg(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx, ret, srbId, rrcMsgType;
   uint8_t cuUeF1apId, duUeF1apId;
   uint8_t *rrcContainer = NULLP;
   uint16_t rrcContLen;
   ULRRCMessageTransfer_t *ulRrcMsg = NULLP;

   ret = ROK;
   ulRrcMsg = &f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;

   for(idx=0; idx < ulRrcMsg->protocolIEs.list.count; idx++)
   {
      switch(ulRrcMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       cuUeF1apId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       duUeF1apId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_SRBID:
	    srbId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.SRBID;
	    break;
	 case ProtocolIE_ID_id_RRCContainer:
	    {
	       rrcContLen = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size;
	       CU_ALLOC(rrcContainer, rrcContLen)
	       if(!rrcContainer)
	       {
	          DU_LOG("\nCU_STUB: Failed to allocated memory in procUlRrcMsg");
	          return RFAILED;
	       }
	       memcpy(rrcContainer, ulRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,\
		     rrcContLen);
	       break;
	    }

	 default:
	    DU_LOG("\n Invalid Event %ld", ulRrcMsg->protocolIEs.list.array[idx]->id);
	    break;
      }
   }
   if(srbId == 1)
   {
      f1apMsgDb.dlRrcMsgCount++;
      rrcMsgType = setDlRRCMsgType();
      if(rrcMsgType == RRC_SECURITY_MODE_COMMAND)
      {
         ret = BuildAndSendUeContextSetupReq(cuUeF1apId, duUeF1apId, rrcContLen, rrcContainer);
      }
      if(rrcMsgType == RRC_RECONFIG)
      {
         ret = BuildAndSendDLRRCMessageTransfer(rrcMsgType);
      }
   }
   return ret;
}

/****************************************************************
 * @brief Build And Send F1ResetAck 
 *
 * @details
 *
 *    Function : FreeF1ResetAck
 *
 *    Functionality:
 *         - Build And Send  F1ResetRSP
 *
 *  @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeF1ResetAck(F1AP_PDU_t *f1apMsg)
{
    uint8_t idx;
    ResetAcknowledge_t *f1ResetAck;

    if(f1apMsg)
    {
       if(f1apMsg->choice.successfulOutcome)
       {
          f1ResetAck= &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

          if(f1ResetAck->protocolIEs.list.array)
          {
             for(idx=0; idx<f1ResetAck->protocolIEs.list.count ; idx++)
             {
                if(f1ResetAck->protocolIEs.list.array[idx])
                {
                   CU_FREE(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
                }
             }
             CU_FREE(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size );
          }
          CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
       }
       CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
    }
}

/****************************************************************
* @brief Build And Send F1ResetAck
*
* @details
*
*    Function : BuildAndSendF1ResetAck
*
*    Functionality:
*         - Build And Send  F1ResetRSP
*
*  @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t BuildAndSendF1ResetAck()
{
   uint8_t                idx = 0;
   uint8_t  	          elementCnt = 0;
   uint8_t                ret = RFAILED;
   F1AP_PDU_t             *f1apMsg = NULL;
   ResetAcknowledge_t     *f1ResetAck = NULLP;
   asn_enc_rval_t         encRetVal;
   DU_LOG("\nF1AP : Building F1 Reset Acknowledgment \n");

   do{
        /* Allocate the memory for F1ResetRequest_t */
        CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
        if(f1apMsg == NULLP)
        {
	   DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
	   break;
        }

        f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

        CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
        if(f1apMsg->choice.successfulOutcome == NULLP)
        {
 	   DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
	   break;
        }

        f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_Reset;
        f1apMsg->choice.successfulOutcome->criticality   = Criticality_reject;
        f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_ResetAcknowledge;
        f1ResetAck = &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

        elementCnt = 1;

        f1ResetAck->protocolIEs.list.count = elementCnt;
        f1ResetAck->protocolIEs.list.size = elementCnt*sizeof(ResetAcknowledgeIEs_t *);

        CU_ALLOC(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size ); 
        if(f1ResetAck->protocolIEs.list.array == NULLP) 
        {
 	   DU_LOG("\nF1AP : Memory allocation for F1ResetAckIEs failed");
	   break;
        }

        for(idx=0; idx<elementCnt; idx++)     
        {
	   CU_ALLOC(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
	   if(f1ResetAck->protocolIEs.list.array[idx] == NULLP)
	   {
	      break;
	   }
        }
        /*TransactionID*/
        idx = 0;
        f1ResetAck->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
        f1ResetAck->protocolIEs.list.array[idx]->criticality = Criticality_reject;
        f1ResetAck->protocolIEs.list.array[idx]->value.present = ResetAcknowledgeIEs__value_PR_TransactionID;
        f1ResetAck->protocolIEs.list.array[idx]->value.choice.TransactionID = TRANS_ID;

        xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

        /* Encode the F1SetupRequest type as UPER */
        memset(encBuf, 0, ENC_BUF_MAX_LEN);
        encBufSize = 0;
        encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

        /* Check encode results */
        if(encRetVal.encoded == ENCODE_FAIL)
        {
	   DU_LOG("\nF1AP : Could not encode F1ResetAck structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	   break;
        }
        else
        {
	   DU_LOG("\nF1AP : Created APER encoded buffer for F1ResetAck \n");
	   for(int i=0; i< encBufSize; i++)
	   {
	      printf("%x",encBuf[i]);
	   }
        }
        /* Sending msg */
        if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
        {
	   DU_LOG("\nF1AP : Sending F1 Reset Response failed");
	   break;
        }

        ret = ROK;
        break;
   }while(true);
   
   FreeF1ResetAck(f1apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Handles received F1AP message and sends back response  
 *
 * @details
 *
 *    Function : F1APMsgHdlr
 *
 *    Functionality:
 *         - Decodes received F1AP control message
 *         - Prepares response message, encodes and sends to SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void F1APMsgHdlr(Buffer *mBuf)
{
   int i;
   char *recvBuf;
   MsgLen copyCnt;
   MsgLen recvBufLen;
   F1AP_PDU_t *f1apMsg = NULLP;
   asn_dec_rval_t rval; /* Decoder return value */
   F1AP_PDU_t f1apasnmsg ;

   DU_LOG("\nF1AP : Received F1AP message buffer");
   ODU_PRINT_MSG(mBuf, 0,0);

   /* Copy mBuf into char array to decode it */
   ODU_GET_MSG_LEN(mBuf, &recvBufLen);
   CU_ALLOC(recvBuf, (Size)recvBufLen);

   if(recvBuf == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation failed");
      return;
   }
   if(ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nF1AP : Failed while copying %d", copyCnt);
      return;
   }

   printf("\nF1AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
      printf("%x",recvBuf[i]);
   }

   /* Decoding flat buffer into F1AP messsage */
   f1apMsg = &f1apasnmsg;
   memset(f1apMsg, 0, sizeof(F1AP_PDU_t));

   rval = aper_decode(0, &asn_DEF_F1AP_PDU, (void **)&f1apMsg, recvBuf, recvBufLen, 0, 0);
   CU_FREE(recvBuf, (Size)recvBufLen);

   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nF1AP : ASN decode failed");
      return;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   switch(f1apMsg->present)
   {
      case F1AP_PDU_PR_initiatingMessage:
      {
         switch(f1apMsg->choice.initiatingMessage->value.present)
         {
            case InitiatingMessage__value_PR_Reset:
	    {
	        DU_LOG("\nF1AP : F1 reset request received ");
	        BuildAndSendF1ResetAck();
	        break;
	    }

	    case InitiatingMessage__value_PR_F1SetupRequest:
            {
               DU_LOG("\nF1AP : F1 setup request received");
               BuildAndSendF1SetupRsp();
	       break;
            }

            case InitiatingMessage__value_PR_GNBDUConfigurationUpdate:
            {
               DU_LOG("\nF1AP : GNB-DU config update received");
               BuildAndSendDUUpdateAck();
	       DU_LOG("\nF1AP : Sending F1 reset request");
	       BuildAndSendF1ResetReq();
               break;
            }
            case InitiatingMessage__value_PR_InitialULRRCMessageTransfer:
            {
               DU_LOG("\nF1AP : Received InitialULRRCMessageTransfer");
               procInitULRRCMsg(f1apMsg);
               break;
            }
	    case InitiatingMessage__value_PR_ULRRCMessageTransfer:
	    {
	       DU_LOG("\nF1AP : Received ULRRCMessageTransfer");
	       procUlRrcMsg(f1apMsg);
	       break;
	    }
	    
	    case InitiatingMessage__value_PR_RRCDeliveryReport:
	    {
	       DU_LOG("\nF1AP : Received RRC delivery report");
	        break;
	    }
            default:
            {
               DU_LOG("\nF1AP : Invalid type of intiating message [%d]",f1apMsg->choice.initiatingMessage->value.present);
               return;
            }
         }/* End of switch(initiatingMessage) */
         break;
      }
      
      case F1AP_PDU_PR_successfulOutcome:
      {
         switch(f1apMsg->choice.successfulOutcome->value.present)
	 {
	    case SuccessfulOutcome__value_PR_ResetAcknowledge:
            {
	       DU_LOG("\nF1Reset Acknowledgement is received successfully ");
	       break;
	    }
            case SuccessfulOutcome__value_PR_UEContextSetupResponse:
	    {
               DU_LOG("\nF1AP : UE ContextSetupResponse received");
               break;
	    }
            default:
            {
               DU_LOG("\nF1AP : Invalid type of successful outcome message [%d]",\
	          f1apMsg->choice.successfulOutcome->value.present);
               return;
            }
	 }/* End of switch(successfulOutcome) */
	 break;
      } 
      default:
	 {
	    DU_LOG("\nF1AP : Invalid type of f1apMsg->present [%d]",f1apMsg->present);
	    return;
	 }
   }/* End of switch(f1apMsg->present) */

} /* End of F1APMsgHdlr */

/**********************************************************************
  End of file
 **********************************************************************/
