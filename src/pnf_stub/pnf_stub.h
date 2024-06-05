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

#ifndef __PNF_MAIN_H
#define __PNF_MAIN_H

#define PNF_NAME_LEN_MAX 30
#define PNF_ID 1
#define PNF_NAME "ORAN_OAM_PNF"

#define LOCAL_IP_PNF "192.168.130.79"

#define PNF_P5_SCTP_PORT 7701
#define NUM_PNF_P5_ASSOC 1 
#define REMOTE_IP_DU (char*[]){"192.168.130.81", "192.168.130.83"}

/*P7 UDP Teansport Cfg Details*/
#define PNF_P7_UDP_PORT 9876
#define VNF_P7_UDP_PORT 6789

#define PNF_APP_MEM_REG 1
#define PNF_POOL 1

typedef enum
{
   PNF_IDLE,
   PNF_CONFIGURED,
   PNF_RUNNING
}PnfState;

typedef struct pnfCfgParams
{
   PnfState          pnfState;
   uint32_t          pnfId;
   char              pnfName[PNF_NAME_LEN_MAX];
   PnfP5SctpParams   pnfP5SctpParams;
   PnfP7UdpParams    pnfP7UdpParams;
}PnfCfgParams;

typedef struct pnfGlobalCb
{
   PnfCfgParams pnfCfgParams;
   uint8_t      numDu;
   //DuDb         duInfo[MAX_DU_SUPPORTED]; /*TODO: VNF Database can be added*/
}PnfGlobalCb;

extern PnfGlobalCb pnfCb;
uint8_t p5MsgHandlerAtPnf(Buffer *mBuf);
void nfapiFillP5Hdr(Buffer *mBuf);
void nfapiFillMsgHdr(Buffer *mBuf, uint8_t phyId, uint16_t msgId, uint32_t msgLen);
void nFapiExtractP5Hdr(nFapi_p5_hdr *p5Hdr, Buffer *mBuf);
void nFapiExtractMsgHdr(nFapi_msg_header *msgHdr, Buffer *mBuf);
#endif
