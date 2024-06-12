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

#define CALC_TIME_USEC_FROM_SFNSLOT(_frameInfo) (_frameInfo.sfn * 10000) + (_frameInfo.slot * PER_TTI_TIME_USEC)

#define CALC_NEXT_SFN_SLOT(_frameInfo)\
{ \
  if((_frameInfo.slot + 1) >= NUM_SLOTS_PER_SUBFRAME) \
  {                                                  \
     _frameInfo.sfn++;                               \
  }                                                  \
  _frameInfo.slot = (_frameInfo.slot + 1) % NUM_SLOTS_PER_SUBFRAME; \
  if(_frameInfo.sfn >= 1024)                                  \
  {                                                           \
     _frameInfo.sfn = _frameInfo.sfn % 1024;                  \
  }                                                           \
}

#define EXTRACT_SFN_SLOT_FROM_TIME(_t, _frameInfo)  \
{                                                   \
   unsigned int time;                                      \
   time = _t / PER_TTI_TIME_USEC;                   \
   _frameInfo.sfn = time / NUM_SLOTS_PER_SUBFRAME;  \
   _frameInfo.slot = time % NUM_SLOTS_PER_SUBFRAME; \
}

/*P7 UDP Teansport Cfg Details*/
#define PNF_P7_UDP_PORT 9876
#define VNF_P7_UDP_PORT 6789

#define PNF_APP_MEM_REG 1
#define PNF_POOL 1

#define NUMEROLOGY 0

uint32_t PER_TTI_TIME_USEC;
uint8_t  NUM_SLOTS_PER_SUBFRAME;

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

typedef struct pnfSlotInfo
{
   uint16_t sfn;
   uint8_t slot;
}PnfSlotInfo;

typedef struct pnfP7Info
{
   uint32_t t_ref_ns;
}PnfP7Info;

typedef struct pnfGlobalCb
{
   PnfCfgParams pnfCfgParams;
   PnfSlotInfo  pnfSlotInfo;
   PnfP7Info    pnfP7Info;
   uint8_t      numDu;
   //DuDb         duInfo[MAX_DU_SUPPORTED]; /*TODO: VNF Database can be added*/
}PnfGlobalCb;

PnfGlobalCb pnfCb;
uint8_t p5MsgHandlerAtPnf(Buffer *mBuf);
void nfapiFillP5Hdr(Buffer *mBuf);
void nfapiFillMsgHdr(Buffer *mBuf, uint8_t phyId, uint16_t msgId, uint32_t msgLen);
void nFapiExtractP5Hdr(nFapi_p5_hdr *p5Hdr, Buffer *mBuf);
void nFapiExtractMsgHdr(nFapi_msg_header *msgHdr, Buffer *mBuf);
void nfapiFillP7Hdr(Buffer *mBuf,uint32_t totSduLen, uint32_t byteOffset, uint32_t time);
void nFapiExtractP7Hdr(nFapi_p7_hdr *p7Hdr, Buffer *mBuf);
void convertIpStringToUInt8(const char *ip_str, uint8_t ip_address[]); 
void fillTlvOfArrayOfUint8(Buffer *mBuf, uint16_t tag, uint16_t length, uint8_t *value);
void fillTlvOfSizeUint8(Buffer *mBuf, uint16_t tag, uint16_t length, uint8_t value);
void fillTlvOfSizeUint16(Buffer *mBuf, uint16_t tag, uint16_t length, uint16_t value);
void fillTlvOfSizeUint32(Buffer *mBuf, uint16_t tag, uint16_t length, uint32_t value);

#endif
