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

#ifdef EGTP_TEST
#include "common_def.h"

#define DU_IP_V4_ADDR "192.168.130.81"
#define CU_IP_V4_ADDR "192.168.130.82"

uint8_t rlcDatSn = 0;

void macStubBuildUlData(Buffer *mBuf)
{
   char data[30] = "This is EGTP data from DU";
   int datSize = 30;

   ODU_ADD_POST_MSG_MULT((Data *)data, datSize, mBuf);

   /* filling IPv4 header */
   CmIpv4Hdr ipv4Hdr;
   MsgLen    mLen;
   uint32_t ipv4_du, ipv4_cu;
   
   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);

   mLen = 0;
   SFndLenMsg(mBuf, &mLen);
 
   memset(&ipv4Hdr, 0, sizeof(CmIpv4Hdr));
   ipv4Hdr.length = CM_IPV4_HDRLEN + mLen;
   ipv4Hdr.hdrVer = 0x45;
   ipv4Hdr.proto = 1;
   ipv4Hdr.srcAddr = CM_INET_NTOH_UINT32(ipv4_du);
   ipv4Hdr.destAddr = CM_INET_NTOH_UINT32(ipv4_cu);
  
   /* Packing IPv4 header into buffer */
   S16          ret, cnt, idx;
   Data         revPkArray[CM_IPV4_HDRLEN];
   Data         pkArray[CM_IPV4_HDRLEN];
  
   /* initialize locals */
   cnt = 0;
   memset(revPkArray, 0, CM_IPV4_HDRLEN);
   memset(pkArray, 0, CM_IPV4_HDRLEN);
 
   /* Pack Header Version */
   pkArray[cnt++] = ipv4Hdr.hdrVer;
  
   /* Pack TOS */
   pkArray[cnt++] = ipv4Hdr.tos;
  
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.length);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.length);
    
   /* Pack Id */
   pkArray[cnt++] = (Data) GetHiByte(ipv4Hdr.id);
   pkArray[cnt++] = (Data) GetLoByte(ipv4Hdr.id);
    
   /* Pack Offset */
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.off);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.off);
   
   /* Pack TTL */
   pkArray[cnt++] = ipv4Hdr.ttl;
    
   /* Pack Protocol */
   pkArray[cnt++] = ipv4Hdr.proto;
    
   /* Pack Checksum */
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.chkSum);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.chkSum);
    
   /* Pack Source Address */
   pkArray[cnt++] = (Data)GetHiByte(GetHiWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetHiWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetHiByte(GetLoWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetLoWord(ipv4Hdr.srcAddr));
    
   /* Pack Destination Address */
   pkArray[cnt++] = (Data)GetHiByte(GetHiWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetHiWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetHiByte(GetLoWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetLoWord(ipv4Hdr.destAddr));
    
   for (idx = 0;  idx < CM_IPV4_HDRLEN;  idx++)
        revPkArray[idx] = pkArray[CM_IPV4_HDRLEN - idx -1];
    
   /* this function automatically reverses revPkArray */
   ret = ODU_ADD_PRE_MSG_MULT(revPkArray, (MsgLen)cnt, mBuf);

   ODU_ADD_PRE_MSG_MULT((Data *)&rlcDatSn, sizeof(uint8_t), mBuf);

   if(rlcDatSn++ >15 )
      rlcDatSn = 0;

} /* macStubBuildUlData */

#endif /* EGTP_TEST */

/**********************************************************************
         End of file
**********************************************************************/
