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

#ifndef _NFAPI_COMMON_H
#define _NFAPI_COMMON_H

#include "nfapi_vnf_fsm.h"

#define NFAPI_P5_PHY_ID    0
#define NFAPI_P5_TIMESTAMP 0 /*As per SCF225,Table 2-3:Set to zero on Tx and ignored on Rx */
#define NFAPI_P5_MORE         0
#define NFAPI_P5_SEG_NUM      0
#define NFAPI_P5_P7_SRU_TYPE  0x01 /*SCF225,Table 2-4: 0x01 for P5/P7 messages.*/

#define NFAPI_MORE_SEG_NUM(_moreField, _segStart) ((_moreField << 7) | (_segStart & 0x07))

/*Common Functions*/
void nfapiFillP5Hdr(Buffer *mBuf);
void nfapiFillMsgHdr(Buffer *mBuf, uint8_t phyId, uint16_t msgId, uint32_t msglen);

void nFapiExtractP5Hdr(nFapi_p5_hdr *p5Hdr, Buffer *mBuf);
void nFapiExtractMsgHdr(nFapi_msg_header *msgHdr, Buffer *mBuf);

NfapiPnfEvent convertNfapiP5TagValToMsgId(uint16_t tagVal);
#endif
