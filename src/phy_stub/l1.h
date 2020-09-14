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

#define MAX_SLOT_VALUE   9
#define MAX_SFN_VALUE    1023
#define NR_PCI            1
#define SLOT_DELAY       3

uint16_t sfnValue = 0;
uint16_t slotValue = 0;
bool     rachIndSent = false;
bool     msg3Sent = false;
bool     msg5ShortBsrSent = false;
bool     msg5Sent = false;

typedef enum
{
   MSG_TYPE_MSG3,
   MSG_TYPE_SHORT_BSR,
   MSG_TYPE_MSG5
}MsgType;

void phyToMac ARGS((uint16_t msgType, uint32_t msgLen,void *msg));
#ifdef INTEL_FAPI
void fillTlvs ARGS((fapi_uint16_tlv_t *tlv, uint16_t tag, uint16_t
	 length, uint16_t value, uint32_t *msgLen));
void fillMsgHeader ARGS((fapi_msg_t *hdr, uint16_t msgType, uint16_t msgLen));
#endif
void procPhyMessages(uint16_t msgType, uint32_t msgSize, void *msg);

/**********************************************************************
  End of file
 **********************************************************************/
