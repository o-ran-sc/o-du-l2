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

/* Defines APIs exchanged between du_app and UDP P7 module */
#ifndef __DUAPP_P7UDP_INF_H__
#define __DUAPP_P7UDP_INF_H__

#define EVENT_NFAPI_P7_UDP_CFG 1

/********************* Global Variable ********************/

typedef struct nfapiP7UdpCfg{
   bool      ipv4P7VnfPres;
   uint32_t  ipv4P7VnfAddr;
   bool      ipv6P7VnfPres;
   uint32_t  ipv6P7VnfAddr;
   uint16_t  p7VnfPort;

}NfapiP7UdpCfg;

typedef uint8_t (* DuNfapiP7UdpCfgFunc) ARGS((
      Pst *pst,
      NfapiP7UdpCfg *nfapiP7UdpCfg));


/******************** FUNCTION DECLARATIONS ********************************/
uint8_t packDuNfapiP7UdpCfg(Pst *pst, NfapiP7UdpCfg *nfapiP7UdpCfg);
uint8_t NfapiProcP7UdpCfg(Pst *pst, NfapiP7UdpCfg *nfapiP7UdpCfg);
uint8_t unpackDuNfapiP7UdpCfg(DuNfapiP7UdpCfgFunc func, Pst *pst, Buffer *mBuf);

#endif

/**********************************************************************
  End of file
 **********************************************************************/
