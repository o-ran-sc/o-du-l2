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

#define SELF_IP "192.178.85.20"
#define SELF_PORT 6789
//#define UE_SIM_IP "192.168.122.1"
//#define UE_SIM_IP "192.178.85.30"
#define UE_SIM_IP "172.27.4.116"
#define UE_SIM_PORT 9876

#define FAPI_CL_INST_0 0
#define FAPI_CL_INST_1 1

#define MAX_BUF_LEN 110000
#define MAX_PDU_SIZE 16000

#define EVT_FAPI_CL_START_SLOT_IND 1
#define EVT_FAPI_CL_START_RECEIVER 2
#define EVT_RECP_REQUEST 4
#define EVT_DATA_REQUEST 12
#define EVT_CNTL_REQUEST 15

#define FILL_PARAM(_src, _dst, _idx) \
{                                    \
   _dst = _src;                      \
   _idx += sizeof(_dst);             \
}

#define GET_FAPI_L1API_HDR(_opq_id, _num_msgs, _idx, _ptr) \
{                                       \
_num_msgs = *(_ptr + _idx++);   \
_opq_id = *(_ptr + _idx++);      \
}

#define GET_FAPI_MSG_ID(_val, _idx, _ptr)\
{                               \
 _val = 0;               \
 _val = (uint16_t) PutHiByte(_val, (uint8_t) (*(_ptr + _idx++)));  \
 _val = (uint16_t) PutLoByte(_val, (uint8_t) (*(_ptr + _idx++)));  \
}
 
typedef enum
{
   UCI_PUSCH,
   UCI_PUCCH_F0_F1,   
   UCI_PUCCH_F2_F3_F4, 
}uciType;

typedef struct slotIndInfo
{
   bool      slotIndicationStarted;
   uint16_t  currSfn;
   uint16_t  currSlot;
}SlotIndInfo;

typedef struct socketInfo
{
   uint32_t  sockfd;
   struct    sockaddr_in srcAddr;
   struct    sockaddr_in dstAddr;
}SocketInfo;

typedef struct clCb
{
   bool      configured;
   SlotIndInfo slotIndInfo;
   SocketInfo  socket;
}ClCb;

ClCb clCb;

void configureFapiCl();
void* generateTicks();
void sendMsgToUeSim(uint8_t event, Data *msg, uint16_t msgLen);
void recvMsgFromUeSim();
void fapiClProcessUeSim2MacFapiMsg(Data *msg);
void fapiClProcessMac2UeSimFapiMsg(void *msg);

/**********************************************************************
  End of file
 **********************************************************************/
