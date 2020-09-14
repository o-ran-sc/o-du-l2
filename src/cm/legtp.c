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
 
/* This file contains DU APP and EGTP interface functions */
#include "common_def.h"
#include "legtp.h"

/*******************************************************************
 *
 * @brief Packs EGTP confirm status
 *
 * @details
 *
 *    Function : packEgtpCfmStatus
 *
 *    Functionality:
 *       Packs EGTP confirm status
 *
 * @params[in] Confirm status
 *             Message buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 packEgtpCfmStatus(CmStatus cfm, Buffer *mBuf)
{
   SPkU16(cfm.status, mBuf);
   SPkU16(cfm.reason, mBuf);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Unpacks EGTP confirm status
 *
 * @details
 *
 *    Function : unpackEgtpCfmStatus
 *
 *    Functionality:
 *       Packs EGTP confirm status
 *
 * @params[in] Confirm status
 *             Message buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

S16 unpackEgtpCfmStatus(CmStatus *cfm, Buffer *mBuf)
{
   SUnpkU16(&(cfm->reason), mBuf);
   SUnpkU16(&(cfm->status), mBuf);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Packs EGTP configuration 
 *
 * @details
 *
 *    Function : packEgtpCfgReq 
 *
 *    Functionality:
 *       Packs EGTP configuration
 *
 * @params[in] Post structure
 *             EGTP configuration 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 packEgtpCfgReq(Pst *pst, EgtpConfig egtpCfg)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      printf("\nDU_APP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }
   if(egtpCfg.localIp.ipV4Pres)
   {
      SPkU32(egtpCfg.localIp.ipV4Addr, mBuf);
   }
   cmPkBool(egtpCfg.localIp.ipV4Pres, mBuf);
   SPkU16(egtpCfg.localPort, mBuf);

   if(egtpCfg.destIp.ipV4Pres)
   {
      SPkU32(egtpCfg.destIp.ipV4Addr, mBuf);
   }
   cmPkBool(egtpCfg.destIp.ipV4Pres, mBuf);
   SPkU16(egtpCfg.destPort, mBuf);

   SPkU32(egtpCfg.minTunnelId, mBuf);
   SPkU32(egtpCfg.maxTunnelId, mBuf);

   SPstTsk(pst, mBuf);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Unpacks EGTP configuration
 *
 * @details
 *
 *    Function : unpackEgtpCfgReq
 *
 *    Functionality:
 *       Unpacks EGTP configuration
 *
 * @params[in] Configuration Request handler
 *             Post structure
 *             Message Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 unpackEgtpCfgReq(EgtpCfgReq func, Pst *pst, Buffer *mBuf)
{
   EgtpConfig egtpCfg;

   cmMemset((U8 *)&egtpCfg, 0, sizeof(EgtpConfig));

   SUnpkU32(&(egtpCfg.maxTunnelId), mBuf);
   SUnpkU32(&(egtpCfg.minTunnelId), mBuf);

   SUnpkU16(&(egtpCfg.destPort), mBuf);
   cmUnpkBool(&(egtpCfg.destIp.ipV4Pres), mBuf);
   if(egtpCfg.destIp.ipV4Pres)
   {  
      SUnpkU32(&(egtpCfg.destIp.ipV4Addr), mBuf);
   }

   SUnpkU16(&(egtpCfg.localPort), mBuf);
   cmUnpkBool(&(egtpCfg.localIp.ipV4Pres), mBuf);
   if(egtpCfg.localIp.ipV4Pres)
   {
      SUnpkU32(&(egtpCfg.localIp.ipV4Addr), mBuf);
   }

   SPutMsg(mBuf);

   RETVALUE((*func)(pst, egtpCfg));
}


/*******************************************************************
 *
 * @brief Packs EGTP configuration results
 *
 * @details
 *
 *    Function : packEgtpCfgCfm
 *
 *    Functionality:
 *       Packs EGTP configuration result
 *
 * @params[in] Post structure
 *             Status confirm 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

S16 packEgtpCfgCfm(Pst *pst, CmStatus cfm)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      printf("\nEGTP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   packEgtpCfmStatus(cfm, mBuf); 
   SPstTsk(pst, mBuf); 
   RETVALUE(ROK); 
}

/*******************************************************************
 *
 * @brief Unpacks EGTP configuration results
 *
 * @details
 *
 *    Function : unpackEgtpCfgCfm
 *
 *    Functionality:
 *       Unpacks EGTP configuration result
 *
 * @params[in] Config Cfm Hanlder
 *             Message buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

S16 unpackEgtpCfgCfm(EgtpCfgCfm func, Buffer *mBuf)
{
   CmStatus cfm;

   cmMemset((U8 *)&cfm, 0, sizeof(CmStatus));
   unpackEgtpCfmStatus(&cfm, mBuf);

   RETVALUE((*func)(cfm));
}

/*******************************************************************
 *
 * @brief Packs EGTP server open request
 *
 * @details
 *
 *    Function : packEgtpSrvOpenReq
 *
 *    Functionality:
 *       Packs EGTP server open request
 *
 * @params[in] Post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/ 
S16 packEgtpSrvOpenReq(Pst *pst)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      printf("\nDU_APP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   SPstTsk(pst, mBuf);
   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Unpacks EGTP server open req
 *
 * @details
 *
 *    Function : unpackEgtpSrvOpenReq
 *
 *    Functionality:
 *       Unpacks EGTP server open req
 *
 * @params[in] Hanlder function pointer
 *             Message buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 unpackEgtpSrvOpenReq(EgtpSrvOpenReq func, Pst *pst, Buffer *mBuf)
{
   RETVALUE((*func)(pst));
}


/*******************************************************************
 *
 * @brief Packs EGTP server open confirm
 *
 * @details
 *
 *    Function : packEgtpSrvOpenCfm
 *
 *    Functionality:
 *       Packs EGTP server open confirm
 *
 * @params[in] Post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
S16 packEgtpSrvOpenCfm(Pst *pst, CmStatus cfm)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      printf("\nEGTP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   packEgtpCfmStatus(cfm, mBuf); 
   SPstTsk(pst, mBuf);
   RETVALUE(ROK);

}

/*******************************************************************
 *
 * @brief Unpacks EGTP server open confirm
 *
 * @details
 *
 *    Function : unpackEgtpSrvOpenCfm
 *
 *    Functionality:
 *       Unpacks EGTP server open confirm
 *
 * @params[in] Hanlder function pointer
 *             Message buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
S16 unpackEgtpSrvOpenCfm(EgtpSrvOpenCfm func, Buffer *mBuf)
{
   CmStatus cfm;

   cmMemset((U8 *)&cfm, 0, sizeof(CmStatus));
   unpackEgtpCfmStatus(&cfm, mBuf);

   RETVALUE((*func)(cfm));
}

/*******************************************************************
 *
 * @brief Packs EGTP tunnel management request
 *
 * @details
 *
 *    Function : packEgtpTnlMgmtReq
 *
 *    Functionality:
 *       Packs EGTP tunnel management request
 *
 * @params[in] Post structure
 *             Tunnel action
 *             Local tunnel endpoint id
 *             Remote tunnel endpoint id
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
S16 packEgtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      printf("\nDU_APP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   SPkU8(tnlEvt.action, mBuf);
   SPkU32(tnlEvt.lclTeid, mBuf);
   SPkU32(tnlEvt.remTeid, mBuf);

   SPstTsk(pst, mBuf);
   RETVALUE(ROK);
}


/*******************************************************************
 *
 * @brief Unpacks EGTP tunnel management request
 *
 * @details
 *
 *    Function : unpackEgtpTnlMgmtReq
 *
 *    Functionality:
 *       Unpacks EGTP tunnel management request
 *
 * @params[in] Hanlder function pointer
 *             Post structure
 *             Message buffer
 * @return ROK     - success
 *         RFAILED - failure
 * 
 * *******************************************************************/
S16 unpackEgtpTnlMgmtReq(EgtpTnlMgmtReq func, Pst *pst, Buffer *mBuf)
{
   EgtpTnlEvt tnlEvt;

   cmMemset((U8 *)&tnlEvt, 0, sizeof(EgtpTnlEvt));

   SUnpkU32(&(tnlEvt.remTeid), mBuf);
   SUnpkU32(&(tnlEvt.lclTeid), mBuf);
   SUnpkU8(&(tnlEvt.action), mBuf);

   RETVALUE((* func)(pst, tnlEvt));

}

/*******************************************************************
 *
 * @brief Packs EGTP tunnel management confirm
 *
 * @details
 *
 *    Function : packEgtpTnlMgmtCfm
 *
 *    Functionality:
 *      Packs EGTP tunnel management cfm
 *
 * @params[in] Post structure
 *             Tunnel Event structure
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ********************************************************************/

S16 packEgtpTnlMgmtCfm(Pst *pst, EgtpTnlEvt tnlEvt)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      printf("\nEGTP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   SPkU8(tnlEvt.action, mBuf);
   SPkU32(tnlEvt.lclTeid, mBuf);
   SPkU32(tnlEvt.remTeid, mBuf);

   packEgtpCfmStatus(tnlEvt.cfmStatus, mBuf);

   SPstTsk(pst, mBuf);
   RETVALUE(ROK);

}

/*******************************************************************
 *
 * @brief Unpacks EGTP tunnel management confirm
 *
 * @details
 *
 *    Function : unpackEgtpTnlMgmtCfm
 *
 *    Functionality:
 *       Unpacks EGTP tunnel management confirm
 *
 * @params[in] Hanlder function pointer
 *             Post structure
 *             Message buffer
 * @return ROK     - success
 *         RFAILED - failure
 * 
 * *******************************************************************/
S16 unpackEgtpTnlMgmtCfm(EgtpTnlMgmtCfm func, Buffer *mBuf)
{
   EgtpTnlEvt tnlEvt;

   cmMemset((U8 *)&tnlEvt, 0, sizeof(EgtpTnlEvt));

   unpackEgtpCfmStatus(&(tnlEvt.cfmStatus), mBuf); 
   SUnpkU32(&(tnlEvt.remTeid), mBuf);
   SUnpkU32(&(tnlEvt.lclTeid), mBuf);
   SUnpkU8(&(tnlEvt.action), mBuf);

   RETVALUE((* func)(tnlEvt));

}

/*******************************************************************
 *
 * @brief Packs Slot indication and sends to EGTP
 *
 * @details
 *
 *    Function : packEgtpSlotInd
 *
 *    Functionality:
 *       Packs slot indication and sends tp EGTP
 *
 * @params[in] Post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
S16 packEgtpSlotInd(Pst *pst)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      printf("\nDU_APP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   SPstTsk(pst, mBuf);
   RETVALUE(ROK);

}

/*******************************************************************
 *
 * @brief Unpacks slot indication
 *
 * @details
 *
 *    Function : unpackEgtpSlotInd
 *
 *    Functionality:
 *       Unpacks slot indication
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 unpackEgtpSlotInd(EgtpSlotInd func, Pst *pst, Buffer *mBuf)
{
   RETVALUE((*func)());
}
