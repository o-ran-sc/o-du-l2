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
 
/* This file contains all SCTP related functionality */
 
#ifndef __PNF_STUB_P7_UDP_H__
#define __PNF_STUB_P7_UDP_H__

typedef struct pnfP7UdpParams
{
   uint32_t     srcIpv4P7Addr;  /*PNF P7*/
   uint16_t     srcIpv4Port;          
   uint32_t     destIpv4P7Addr; /*VNF P7*/
   uint16_t     destIpv4Port;          
}PnfP7UdpParams;

typedef struct pnfP7UdpGlobalCb
{
   CmInetAddr   srcAddr;        /*PNF IP Address*/
   CmInetAddr   destAddr;       /*VNF IP Address*/
   CmInetFd     sockFd;         /* Socket file descriptor */
   uint16_t     gCntMsg; 
}PnfP7UdpGlobalCb;

PnfP7UdpGlobalCb pnfP7Cb;

uint8_t pnfP7UdpActvInit();
uint8_t pnfP7UdpCfgReq();
void    pnfP7UdpHdlRecvMsg(Buffer *buf);
S16 pnfP7UdpSendMsg(Buffer *mBuf);

#endif

