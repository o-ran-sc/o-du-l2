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

/* This file contains all F1AP message handler related functionality */
#define ENCODE_FAIL -1
#define TRANS_ID 1
#define RRC_SIZE 1
#define SUL_BAND_COUNT 0
#define UL_SRBID        1
#define DL_SRBID        0
#define DU_ID           1
#define CU_ID           1
#define CRNTI           17017
#define CELL_INDEX      0
#define SRB0 0
#define SRB1 1
#define SRB2 2
#define DRB1 1
#define DRB2 2
#define DRB3 3
#define QCI  9
#define MAX_DRB_SET_UE_CONTEXT_SETUP_REQ 2  /*Number of DRBs to be added using UE CONTEXT SETUP procedure*/
#define MAX_DRB_SET_UE_CONTEXT_MOD_REQ 1     /*Number of DRBs to be added/setup via UE CONTEXT MODIFICATION procedure*/
#define MAX_DRB_MODIFIED_UE_MOD_REQ 1        /*Currently CU trigger Modification of 2DRBs*/
#define DRB_ID_TO_ADD_MOD MAX_DRB_SET_UE_CONTEXT_SETUP_REQ+1 /*The DRB ID: which will be added using Modification Procedure*/

/*Total num of DRBs added using UE COntext Setup and Modification procedures*/
#define MAX_NUM_DRB_ADDED_PER_UE (MAX_DRB_SET_UE_CONTEXT_SETUP_REQ + MAX_DRB_SET_UE_CONTEXT_MOD_REQ)

#define CU_UE_F1AP_ID 0
#define DU_UE_F1AP_ID 1
#define SP_CELL_ID     2
#define SERV_CELL_IDX 3
#define CU_TO_DU_RRC_INF 4
#define NUM_QOS_EXT 1

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
#define PUSCH_K2_CFG1  1
#define PUSCH_K2_CFG2  2
#define PUSCH_START_SYMBOL 3

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
#define FIVE_QI_VALUE9 9  /*spec 23.501, Table 5.7.4-1*/
#define FIVE_QI_VALUE8 8  /*spec 23.501, Table 5.7.4-1*/
#define PDU_SESSION_ID_1 1
#define PDU_SESSION_ID_2 2
#define INVALID_PDU_SESSION_ID -1

typedef enum
{
   RRC_SETUP=1,
   RRC_SETUP_COMPLETE,
   NAS_AUTHENTICATION_RSP,
   NAS_SECURITY_MODE_COMPLETE,
   RRC_SECURITY_MODE_COMPLETE,
   REGISTRATION_COMPLETE,
   UE_CONTEXT_SETUP_RSP,
   RRC_RECONFIG_COMPLETE,
}RrcMsgType;

/* Defines action to be performed via UE Context Modification request */
typedef enum
{
   MODIFY_UE,        /* Modify UE configuration at DU */
   RRC_RECONFIG_COMPLETE_IND, /* Indicate to DU that RRC Reconfiguration is complete */
   QUERY_CONFIG,     /* Query DU for its configurations */
   STOP_DATA_TX,      /* Stop data transmission from DU to a particular UE */
   RESTART_DATA_TX   /* Restart data transmission from DU to a particular UE */
}UeCtxtModAction;

typedef struct f1apDb
{
   uint8_t dlRrcMsgCount;
   OCTET_STRING_t duToCuContainer;
}F1apMsgDb;
  
void F1APMsgHdlr(uint32_t *destDuId, Buffer *mBuf);
uint8_t BuildAndSendUeContextModificationReq(uint32_t duId, void *ueCb, UeCtxtModAction action);
uint8_t BuildAndSendUeContextReleaseCommand(uint32_t duId, uint8_t cuUeF1apId, uint8_t duUeF1apId);
uint8_t BuildAndSendF1ResetReq();
uint8_t BuildAndSendPagingMsg(uint64_t gsTmsi, uint8_t duId);

/**********************************************************************
         End of file
**********************************************************************/
