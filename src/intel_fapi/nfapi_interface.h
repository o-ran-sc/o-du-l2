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

/*Reference: SCF225_5G_NFAPI_SPECIFICATION, v225.2.1, Issue Date: 23 Nov 2021*/

#ifndef _NFAPI_INTERFACE_H
#define _NFAPI_INTERFACE_H

#include "fapi_interface.h"
/*Table 2-7 Dedicated NFAPI message IDs*/

/*P5 Messages*/
#define NFAPI_PNF_PARAM_REQ    0x0100
#define NFAPI_PNF_PARAM_RESP   0x0101
#define NFAPI_PNF_CONFIG_REQ   0x0102
#define NFAPI_PNF_CONFIG_RESP  0x0103
#define NFAPI_PNF_START_REQ    0x0104
#define NFAPI_PNF_START_RESP   0x0105
#define NFAPI_PNF_STOP_REQ     0x0106
#define NFAPI_PNF_STOP_RESP    0x0107
#define NFAPI_START_RESPONSE         0x0108
#define NFAPI_PNF_READY_IND    0x0109

/*Reserved for P5 Dedicated NFAPI Messages 0x010A - 0x017F */

/*P7 messages*/
#define NFAPI_DL_NODE_SYNC     0x0180
#define NFAPI_UL_NODE_SYNC     0x0181
#define NFAPI_TIMING_INFO      0x0182

/*Reserved for P7 Dedicated NFAPI Messages 0x0183 - 0x01ff*/
/*RESERVED for Vendor Extension messages 0x0300 - 0x03ff*/

/*Table 3-16 nFAPI TLVs included in PARAM.response and 
 *Table 3-19 nFAPI TLVs included in CONFIG.request*/
//PNF and VNF Parameters
#define NFAPI_P7_VNF_ADD_IPV4              0x0100
#define NFAPI_P7_VNF_ADD_IPV6              0x0101
#define NFAPI_P7_VNF_PORT                  0x0102
#define NFAPI_P7_PNF_ADD_IPV4              0x0103
#define NFAPI_P7_PNF_ADD_IPV6              0x0104
#define NFAPI_P7_PNF_PORT                  0x0105
//TTI Related Parameters
#define NFAPI_DL_TTI_TIMING_OFFSET         0x0106
#define NFAPI_UL_TTI_TIMING_OFFSET         0x0107
#define NFAPI_UL_DCI_TIMING_OFFSET         0x0108
#define NFAPI_TX_DATA_TIMING_OFFSET        0x0109
//Timing Related Parameters
#define NFAPI_TIMING_WINDOW                0x011E
#define NFAPI_TIMING_INFO_MODE             0x011F
#define NFAPI_TIMING_INFO_PERIOD           0x0120
//P7 Transport Related Parameters
#define NFAPI_P7_IP_FRAGMENTATION_ALLOWED  0x0121
#define NFAPI_P7_TRANSPORT                 0x0122
#define NFAPI_P7_PNF_ETHERNET_ADD          0x0123
#define NFAPI_P7_VNF_ETHERNET_ADD          0x0124
//CPRI Related Parameters
#define NFAPI_ECPRI_MSG_TYPE               0x0125
#define NFAPI_ECPRI_PHY_TRANSPORT_ID       0x0126

#define NFAPI_5G_FAPI_MSG_BODY             0x0F00

/*Table 3-2*/
#define NFAPI_PNF_PARAM_GENERAL            0x1000 

/*As per Table 3-15 and Table 3-17*/
#define MAX_NUM_TLV_IN_PARAM_RESP          19
/*As per Table 3-19*/
#define MAX_NUM_TLV_IN_CONFIG_REQ          9

typedef enum {
   PNF_STATE_IDLE = 0,
   PNF_STATE_CONFIGURED,
   PNF_STATE_RUNNING
}nFapi_pnf_state;

/*Table 3-4 and Table 3-7*/
typedef enum {
   NFAPI_MSG_OK = 0,
   NFAPI_MSG_INVALID_STATE,
   NFAPI_MSG_INVALID_CONFIG
}nFapi_error_code;

/*Table 3-21: "Timing info mode"*/
typedef enum {
   NFAPI_TIMING_INFO_MODE_DISABLE = 0,
   NFAPI_TIMING_INFO_MODE_PERIODIC,
   NFAPI_TIMING_INFO_MODE_APERIODIC,
   NFAPI_TIMING_INFO_MODE_PERIODIC_APERIODIC
}nFapi_timing_info_mode;


/*Table 2-3*/
typedef struct nFapi_p5_hdr_t{
   uint16_t seg_len;       /*Segment Length*/
   uint8_t  more_segNum;   /*Combination of MORE(1 bit = MSB) and SEGMENT NUMBER(7 bits)*/
   uint8_t  seq_num;       /*Sequence Number*/
   uint32_t timeStamp;     /*Transit TimeStamp*/
}nFapi_p5_hdr;

/*Table 2-5*/
typedef struct nFapi_p7_hdr_t{
   uint16_t seq_num;       /*Sequence Number*/
   uint16_t pad;           /*Padding for Byte alignment*/
   uint32_t tot_SDU_len;   /*Total SDU length*/
   uint32_t byteOffset;    /*Byte Offset: Offset specifying the start of a segment*/
   uint32_t timeStamp;     /*Transit TimeStamp*/
}nFapi_p7_hdr;

/*Table 2-4*/
typedef struct nFapi_msg_header_t{
   uint8_t  sRU_termination_type;  /*RU termination Type: 0x01 = P5/P7 msgs*/
   uint8_t  phy_id;                /*PHY instance: [1..”Maximum Number Of PHYs]*/
   uint16_t msg_id;                /*msg ID of FAPI or NFAPI msg*/
   uint32_t length;                /*len of msg body in octets excluding msg hdr.*/
}nFapi_msg_header;

typedef struct {
   uint16_t tag;
   uint16_t length;
   void     *value;
}nFapi_tlv;

//Section 2.1.1 P5 PNF Procedures
/*Table 3-1*/
typedef struct nFapi_pnf_ready_ind_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint32_t          versionInfo;
}nFapi_pnf_ready_ind;

typedef struct nFapi_pnf_param_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
}nFapi_pnf_param_req;

/*Table 3-3*/
typedef struct nFapi_pnf_param_general_t{
   uint8_t   nFapi_sync_mode;
   uint8_t   loc_mode;
   uint8_t   *loc_coordinates; /*Spec 38.455, 'NG-RANAccessPointPosition'*/
   uint16_t  max_num_phy;
   uint8_t   oui[3];
   uint16_t  numRfInstances;
   uint16_t  numDfeInstances;
}nFapi_pnf_param_general;

/*Table 3-2*/
typedef struct nFapi_pnf_param_resp_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   nFapi_error_code  errCode;
   uint8_t           numTLVs;
   nFapi_tlv         tlvs[1]; /*As per Spec only 1 TLV is defined (nFapi_pnf_param_general)*/
}nFapi_pnf_param_resp;

/*Table 3-5*/
typedef struct nFapi_pnf_config_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           numTLVs;
}nFapi_pnf_config_req;

/*Table 3-6*/
typedef struct nFapi_pnf_config_resp_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   nFapi_error_code  errCode;
}nFapi_pnf_config_resp;

typedef struct nFapi_pnf_start_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
}nFapi_pnf_start_req;

/*Table 3-9*/
typedef struct nFapi_pnf_start_resp_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   nFapi_error_code  errCode;
}nFapi_pnf_start_resp;

typedef struct nFapi_pnf_stop_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
}nFapi_pnf_stop_req;

/*Table 3-10*/
typedef struct nFapi_pnf_stop_resp_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   nFapi_error_code  errCode;
}nFapi_pnf_stop_resp;

//Section 2.1.2 P5 PHY Procedures
typedef struct nFapi_param_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           *fapiMsgBody; /*SCF222 v222.10.03, Table 3-5*/ 
}nFapi_param_req;

/*Table 3-14*/
typedef struct nFapi_param_resp_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   nFapi_error_code  errCode;
   uint8_t           numTLVs;
   nFapi_tlv         tlvs[MAX_NUM_TLV_IN_PARAM_RESP];
}nFapi_param_resp;

/*Table 3-18*/
typedef struct nFapi_config_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           numTLVs;
   nFapi_tlv         tlvs[MAX_NUM_TLV_IN_CONFIG_REQ];
}nFapi_config_req;

typedef struct nFapi_config_resp_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           *fapiMsgBody; /*SCF222 v222.10.03, Table 3-27*/ 
}nFapi_config_resp;

typedef struct nFapi_start_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           *fapiMsgBody; /*SCF222 v222.10.03, Sec 3.3.4.1*/
}nFapi_start_req;

/*Table 3-12*/
typedef struct nFapi_start_resp_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   nFapi_error_code  errCode;
}nFapi_start_resp;

typedef struct nFapi_error_ind_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           *fapiMsgBody; /*SCF222 v222.10.03, Sec 3.3.6.1*/
}nFapi_error_ind;

typedef struct nFapi_stop_req_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           *fapiMsgBody; /*SCF222 v222.10.03, Sec 3.3.5.1*/
}nFapi_stop_req;

typedef struct nFapi_stop_ind_t{
   nFapi_p5_hdr      nFapiP5Hdr;
   nFapi_msg_header  nFapiMsghdr;
   uint8_t           *fapiMsgBody; /*SCF222 v222.10.03, Sec 3.3.5.2*/
}nFapi_stop_ind;

//P7 Procedures
/*Table 4-1*/
typedef struct nFapi_dl_node_sync_info_t{
   uint32_t t1;   /*In micro-sec, timeDiff between [VNF's SFN 0/0] and [DL NodeSYnc Tx time]*/
   int32_t  delta_sfnSlot;
   uint8_t  scs;
}nFapi_dl_node_sync_info;

typedef struct nFapi_dl_node_sync_t{
   nFapi_p7_hdr             nFapiP7Hdr;
   nFapi_msg_header         nFapiMsghdr;
   nFapi_dl_node_sync_info  dlNodeSyncInfo;
}nFapi_dl_node_sync;

/*Table 4-2*/
typedef struct nFapi_ul_node_sync_info_t{
   uint32_t t1;   /*In micro-sec, timeDiff between [VNF's SFN 0/0] and [DL NodeSYnc Tx time]*/
   uint32_t t2;   /*In micro-sec, timeDiff between [PNF's SFN 0/0] and [DL NodeSYnc Rx time]*/
   uint32_t t3;   /*In micro-sec, timeDiff between [PNF's SFN 0/0] and [UL NodeSYnc Tx time]*/
}nFapi_ul_node_sync_info;

typedef struct nFapi_ul_node_sync_t{
   nFapi_p7_hdr             nFapiP7Hdr;
   nFapi_msg_header         nFapiMsghdr;
   nFapi_ul_node_sync_info  ulNodeSyncInfo;
}nFapi_ul_node_sync;

/*Table 4-3*/
typedef struct nFapi_timing_parameter_t{
   uint16_t lastSFN;
   uint16_t lastSlot;
   uint32_t time_elapsed; /*In ms, Time since the last timing Info*/
   uint32_t dl_tti_jitter;  /*Units: micro-sec(ms)*/
   uint32_t tx_data_jitter; /*Units: ms*/
   uint32_t ul_tti_jitter;  /*Units: ms*/
   uint32_t ul_dci_jitter;  /*Units: ms*/
   int32_t  dl_tti_latest_delay; /*Units: ms*/
   int32_t  tx_data_latest_delay;
   int32_t  ul_tti_latest_delay; /*Units: ms*/
   int32_t  ul_dci_latest_delay; /*Units: ms*/
   int32_t  dl_tti_early_arr; /*Units: ms*/
   int32_t  tx_data_early_arr; /*Units: ms*/
   int32_t  ul_tti_early_arr; /*Units: ms*/
   int32_t  ul_dci_early_arr; /*Units: ms*/
   uint8_t  subCarrierSpacing;
}nFapi_timing_parameter;

typedef struct nFapi_timing_info_t{
   nFapi_p7_hdr             nFapiP7Hdr;
   nFapi_msg_header         nFapiMsghdr;
   nFapi_timing_parameter   timingParam;
}nFapi_timing_info;

/*SCF222 v222.10.03, Table 3-54*/
typedef struct fapi_dl_tti_req_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint8_t nPdus;
   uint8_t nGroup;
   uint8_t pad[2];
   fapi_dl_tti_req_pdu_t pdus[FAPI_MAX_PDUS_PER_SLOT]; // 5G FAPI Table 3-35
   fapi_ue_info_t ue_grp_info[FAPI_MAX_NUMBER_OF_GROUPS_PER_TTI];
}fapi_dl_tti_req_msg_body;

typedef struct nFapi_dl_tti_req_t{
   nFapi_p7_hdr              nFapiP7Hdr;
   nFapi_msg_header          nFapiMsghdr;
   fapi_dl_tti_req_msg_body  fapiMsgBody;
}nFapi_dl_tti_req;

typedef struct nFapi_dl_tti_resp_t{
   nFapi_p7_hdr       nFapiP7Hdr;
   nFapi_msg_header   nFapiMsghdr;
   uint8_t            *fapiMsgBody; /*TODO: In fapi_interface.h, dl_tti_resp not present*/
}nFapi_dl_tti_resp;

typedef struct fapi_ul_tti_req_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint8_t nPdus;
   uint8_t rachPresent;
   uint8_t nUlsch;
   uint8_t nUlcch;
   uint8_t nGroup;
   uint8_t pad[3];
   fapi_ul_tti_req_pdu_t pdus[FAPI_MAX_NUMBER_UL_PDUS_PER_TTI];    // 5G FAPI Table 3-44
   fapi_ue_info_t ueGrpInfo[FAPI_MAX_NUMBER_OF_GROUPS_PER_TTI];
}fapi_ul_tti_req_msg_body;

typedef struct nFapi_ul_tti_req_t{
   nFapi_p7_hdr              nFapiP7Hdr;
   nFapi_msg_header          nFapiMsghdr;
   fapi_ul_tti_req_msg_body  fapiMsgBody;
}nFapi_ul_tti_req;

typedef struct fapi_ul_dci_req_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint8_t numPdus;
   uint8_t pad[3];
   fapi_dci_pdu_t pdus[FAPI_MAX_NUMBER_UCI_PDUS_PER_SLOT]; // 5G FAPI Table 3-54
}fapi_ul_dci_req_msg_body;

typedef struct nFapi_ul_dci_req_t{
   nFapi_p7_hdr              nFapiP7Hdr;
   nFapi_msg_header          nFapiMsghdr;
   fapi_ul_dci_req_msg_body  fapiMsgBody;
}nFapi_ul_dci_req;

typedef struct fapi_tx_data_req_msg_body_t{ 
   uint16_t sfn;
   uint16_t slot;
   uint16_t num_pdus;
   uint8_t pad[2];
   fapi_tx_pdu_desc_t pdu_desc[FAPI_MAX_NUMBER_DL_PDUS_PER_TTI];   // 5G FAPI Table 3-58
}fapi_tx_data_req_msg_body;

typedef struct nFapi_tx_data_req_t{
   nFapi_p7_hdr               nFapiP7Hdr;
   nFapi_msg_header           nFapiMsghdr;
   fapi_tx_data_req_msg_body  fapiMsgBody;
}nFapi_tx_data_req;

typedef struct fapi_rx_data_ind_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint16_t numPdus;
   uint8_t pad[2];
   fapi_pdu_ind_info_t pdus[FAPI_MAX_NUMBER_OF_ULSCH_PDUS_PER_SLOT];   // 5G FAPI Table 3-61
}fapi_rx_data_ind_msg_body;

typedef struct nFapi_rx_data_ind_t{
   nFapi_p7_hdr               nFapiP7Hdr;
   nFapi_msg_header           nFapiMsghdr;
   fapi_rx_data_ind_msg_body  fapiMsgBody;
}nFapi_rx_data_ind;

typedef struct fapi_crc_ind_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint16_t numCrcs;
   uint8_t pad[2];
   fapi_crc_ind_info_t crc[FAPI_MAX_NUMBER_OF_CRCS_PER_SLOT];  // 5G FAPI Table 3-62
}fapi_crc_ind_msg_body;

typedef struct nFapi_crc_ind_t{
   nFapi_p7_hdr           nFapiP7Hdr;
   nFapi_msg_header       nFapiMsghdr;
   fapi_crc_ind_msg_body  fapiMsgBody;
}nFapi_crc_ind;

typedef struct fapi_uci_ind_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint16_t numUcis;       // 5G FAPI Table 3-63
   uint8_t pad[2];
   fapi_uci_pdu_info_t uciPdu[FAPI_MAX_NUMBER_UCI_PDUS_PER_SLOT];
}fapi_uci_ind_msg_body;

typedef struct nFapi_uci_ind_t{
   nFapi_p7_hdr           nFapiP7Hdr;
   nFapi_msg_header       nFapiMsghdr;
   fapi_uci_ind_msg_body  fapiMsgBody;
}nFapi_uci_ind;

typedef struct fapi_srs_ind_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint8_t numPdus;
   uint8_t pad[3];
   fapi_srs_pdu_t srsPdus[FAPI_MAX_NUMBER_SRS_PDUS_PER_SLOT];  // 5G FAPI Table 3-73
}fapi_srs_ind_msg_body;

typedef struct nFapi_srs_ind_t{
   nFapi_p7_hdr           nFapiP7Hdr;
   nFapi_msg_header       nFapiMsghdr;
   fapi_srs_ind_msg_body  fapiMsgBody;
}nFapi_srs_ind;

typedef struct fapi_rach_ind_msg_body_t{
   uint16_t sfn;
   uint16_t slot;
   uint8_t numPdus;
   uint8_t pad[3];
   fapi_rach_pdu_t rachPdu[FAPI_MAX_NUMBER_RACH_PDUS_PER_SLOT];    // 5G FAPI Table 3-74
}fapi_rach_ind_msg_body;

typedef struct nFapi_rach_ind_t{
   nFapi_p7_hdr           nFapiP7Hdr;
   nFapi_msg_header       nFapiMsghdr;
   fapi_rach_ind_msg_body  fapiMsgBody;
}nFapi_rach_ind;

#endif
