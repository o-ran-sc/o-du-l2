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
#include "mac_stub.h"

uint8_t rlcDatSn = 0;
uint64_t ulDatNum = 0;

uint8_t macStubBOStatus(Pst *pst, SpId spId, RlcMacBOStatus *boSta)
{
  Pst rspPst;
  RlcMacSchedRepInfo *schRep;

  DU_LOG("\nMAC : Received BO status from RLC");

//  SGetSBuf(pst->region, pst->pool, (Data **)&rspPst, sizeof(Pst));

  rspPst.selector  = ODU_SELECTOR_TC;
  rspPst.srcEnt    = pst->dstEnt;
  rspPst.dstEnt    = pst->srcEnt;
  rspPst.dstInst   = pst->srcInst;
  rspPst.dstProcId = pst->srcProcId;
  rspPst.srcProcId = pst->dstProcId;
  rspPst.region    = pst->region; 
  rspPst.pool    = pst->pool;

  /* Filling Scheduling Report */
  RLC_SHRABL_STATIC_BUF_ALLOC(pst->region, pst->pool, schRep, sizeof(RlcMacSchedRepInfo));
   
  schRep->cellId = boSta->cellId;
  schRep->rnti   = boSta->rnti;
  schRep->nmbLch = 1;
  schRep->lchSta[0].commCh = boSta->commCh;
  schRep->lchSta[0].lchStaInd.lcId = boSta->lcId;
  schRep->lchSta[0].lchStaInd.totBufSize = boSta->bo + 5; /* Extra buffer space including RLC and MAC Header size */

  DU_LOG("\nMAC : Sending scheduling report to RLC");
  
  RlcMacProcSchedRep(&rspPst, 1, schRep);

  return ROK;

}

uint8_t macStubSendDlData(Pst *pst, SpId spId, RlcMacData *dlData)
{
   U32 availmem;

   Pst rspPst;
   Buffer *mBuf;
   RlcMacData *ulData;

   DU_LOG("\nMAC_STUB : Received DL data from RLC to be sent to PHY"); 

   ODU_PUT_MSG_BUF(dlData->pduInfo[0].pduBuf);
   dlData->pduInfo[0].pduBuf = NULL;

#if 0
   RLC_FREE_SHRABL_BUF(pst->region, pst->pool,
                        dlData, sizeof(RlcMacData));
{
   RLC_ALLOC_SHRABL_BUF(pst->region, pst->pool,
                          ulData, sizeof(RlcMacData));

   SRegInfoShow(2, &availmem);
   cmMemcpy((U8 *)ulData, (U8 *)dlData, sizeof(RlcMacData));
#endif
   
   ulData = dlData;
   ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf);
   macStubBuildUlData(mBuf);
   ulData->pduInfo[0].pduBuf = mBuf;

   /* Fill response post */
   rspPst.selector  = ODU_SELECTOR_TC;
   rspPst.srcEnt    = pst->dstEnt;
   rspPst.dstEnt    = pst->srcEnt;
   rspPst.dstInst   = pst->srcInst;
   rspPst.dstProcId = pst->srcProcId;
   rspPst.srcProcId = pst->dstProcId;
   rspPst.region    = pst->region;
   rspPst.pool      = pst->pool;

   ulDatNum++;
   DU_LOG("\nMAC_STUB : UL data number %d", ulDatNum);
   RlcProcUlData(&rspPst, 1, ulData);
   return ROK;
}

void macStubBuildUlData(Buffer *mBuf)
{
   char data[30] = "This is EGTP data from DU";
   int datSize = 30;
   U32 availmem;

   ODU_ADD_POST_MSG_MULT((Data *)data, datSize, mBuf);

   /* filling IPv4 header */
   CmIpv4Hdr ipv4Hdr;
   MsgLen    mLen;
   U32 ipv4_du, ipv4_cu;
   
   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);

   mLen = 0;
   SFndLenMsg(mBuf, &mLen);
 
   cmMemset((U8 *)&ipv4Hdr, 0, sizeof(CmIpv4Hdr));
   ipv4Hdr.length = CM_IPV4_HDRLEN + mLen;
   ipv4Hdr.hdrVer = 0x45;
   ipv4Hdr.proto = 1;
   ipv4Hdr.srcAddr = CM_INET_NTOH_U32(ipv4_du);
   ipv4Hdr.destAddr = CM_INET_NTOH_U32(ipv4_cu);
  
   /* Packing IPv4 header into buffer */
   S16          ret, cnt, idx;
   Data         revPkArray[CM_IPV4_HDRLEN];
   Data         pkArray[CM_IPV4_HDRLEN];
  
   /* initialize locals */
   cnt = 0;
   cmMemset(revPkArray, 0, CM_IPV4_HDRLEN);
   cmMemset(pkArray, 0, CM_IPV4_HDRLEN);
 
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
#if 0
   SRegInfoShow(0, &availmem);
   SRegInfoShow(1, &availmem);
   SRegInfoShow(2, &availmem);
   SRegInfoShow(3, &availmem);
   SRegInfoShow(4, &availmem);
#endif
   //rlcDatSn++;
   //
   if(rlcDatSn++ >15 )
      rlcDatSn = 0;

} /* macStubBuildUlData */

#endif /* EGTP_TEST */

/**********************************************************************
         End of file
**********************************************************************/
