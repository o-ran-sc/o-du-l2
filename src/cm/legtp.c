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
uint8_t packEgtpCfmStatus(CmStatus cfm, Buffer *mBuf)
{
   oduUnpackUInt16(cfm.status, mBuf);
   oduUnpackUInt16(cfm.reason, mBuf);
   
   return ROK;
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

uint8_t unpackEgtpCfmStatus(CmStatus *cfm, Buffer *mBuf)
{
   oduPackUInt16(&(cfm->reason), mBuf);
   oduPackUInt16(&(cfm->status), mBuf);

   return ROK;
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
uint8_t packEgtpCfgReq(Pst *pst, EgtpConfig egtpCfg)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to allocate memory");
      return RFAILED;
   }
   if(egtpCfg.localIp.ipV4Pres)
   {
      oduUnpackUInt32(egtpCfg.localIp.ipV4Addr, mBuf);
   }
   oduPackBool(egtpCfg.localIp.ipV4Pres, mBuf);
   oduUnpackUInt16(egtpCfg.localPort, mBuf);

   if(egtpCfg.destIp.ipV4Pres)
   {
      oduUnpackUInt32(egtpCfg.destIp.ipV4Addr, mBuf);
   }
   oduPackBool(egtpCfg.destIp.ipV4Pres, mBuf);
   oduUnpackUInt16(egtpCfg.destPort, mBuf);

   oduUnpackUInt32(egtpCfg.minTunnelId, mBuf);
   oduUnpackUInt32(egtpCfg.maxTunnelId, mBuf);

   ODU_POST_TASK(pst, mBuf);

   return ROK;
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
uint8_t unpackEgtpCfgReq(EgtpCfgReq func, Pst *pst, Buffer *mBuf)
{
   EgtpConfig egtpCfg;

   memset(&egtpCfg, 0, sizeof(EgtpConfig));

   oduPackUInt32(&(egtpCfg.maxTunnelId), mBuf);
   oduPackUInt32(&(egtpCfg.minTunnelId), mBuf);
  
   oduPackUInt16(&(egtpCfg.destPort), mBuf);
   oduUnpackBool(&(egtpCfg.destIp.ipV4Pres), mBuf);
   if(egtpCfg.destIp.ipV4Pres)
   {  
      oduPackUInt32(&(egtpCfg.destIp.ipV4Addr), mBuf);
   }

   oduPackUInt16(&(egtpCfg.localPort), mBuf);
   oduUnpackBool(&(egtpCfg.localIp.ipV4Pres), mBuf);
   if(egtpCfg.localIp.ipV4Pres)
   {
      oduPackUInt32(&(egtpCfg.localIp.ipV4Addr), mBuf);
   }
   
   ODU_PUT_MSG_BUF(mBuf);
 
   return ((*func)(pst, egtpCfg));
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

uint8_t packEgtpCfgCfm(Pst *pst, CmStatus cfm)
{
   Buffer *mBuf;
  
   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to allocate memory");
      return RFAILED;
   }
 
   packEgtpCfmStatus(cfm, mBuf); 
   ODU_POST_TASK(pst, mBuf); 
   return ROK; 
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
 
uint8_t unpackEgtpCfgCfm(EgtpCfgCfm func, Buffer *mBuf)
{
   CmStatus cfm;
   
   memset(&cfm, 0, sizeof(CmStatus));
   unpackEgtpCfmStatus(&cfm, mBuf);

   return ((*func)(cfm));
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
uint8_t packEgtpSrvOpenReq(Pst *pst)
{
   Buffer *mBuf;
 
   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to allocate memory");
      return RFAILED;
   }
  
   ODU_POST_TASK(pst, mBuf);
   return ROK;
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
uint8_t unpackEgtpSrvOpenReq(EgtpSrvOpenReq func, Pst *pst, Buffer *mBuf)
{
    return ((*func)(pst));
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
uint8_t packEgtpSrvOpenCfm(Pst *pst, CmStatus cfm)
{
   Buffer *mBuf;
 
   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to allocate memory");
      return RFAILED;
   }
 
   packEgtpCfmStatus(cfm, mBuf); 
   ODU_POST_TASK(pst, mBuf);
   return ROK;

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
uint8_t unpackEgtpSrvOpenCfm(EgtpSrvOpenCfm func, Buffer *mBuf)
{
   CmStatus cfm;
    
   memset(&cfm, 0, sizeof(CmStatus));
   unpackEgtpCfmStatus(&cfm, mBuf);

   return ((*func)(cfm));
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
uint8_t packEgtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to allocate memory");
      return RFAILED;
   }

   oduUnpackUInt8(tnlEvt.action, mBuf);
   oduUnpackUInt32(tnlEvt.lclTeid, mBuf);
   oduUnpackUInt32(tnlEvt.remTeid, mBuf);

   ODU_POST_TASK(pst, mBuf);
   return ROK;
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
uint8_t unpackEgtpTnlMgmtReq(EgtpTnlMgmtReq func, Pst *pst, Buffer *mBuf)
{
   EgtpTnlEvt tnlEvt;

   memset(&tnlEvt, 0, sizeof(EgtpTnlEvt));

   oduPackUInt32(&(tnlEvt.remTeid), mBuf);
   oduPackUInt32(&(tnlEvt.lclTeid), mBuf);
   oduPackUInt8(&(tnlEvt.action), mBuf);

   return ((* func)(pst, tnlEvt));

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

uint8_t packEgtpTnlMgmtCfm(Pst *pst, EgtpTnlEvt tnlEvt)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to allocate memory");
      return RFAILED;
   }
    
   oduUnpackUInt8(tnlEvt.action, mBuf);
   oduUnpackUInt32(tnlEvt.lclTeid, mBuf);
   oduUnpackUInt32(tnlEvt.remTeid, mBuf);
   
   packEgtpCfmStatus(tnlEvt.cfmStatus, mBuf);
    
   ODU_POST_TASK(pst, mBuf);
   return ROK;

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
uint8_t unpackEgtpTnlMgmtCfm(EgtpTnlMgmtCfm func, Buffer *mBuf)
{
   EgtpTnlEvt tnlEvt;
 
   memset(&tnlEvt, 0, sizeof(EgtpTnlEvt));

   unpackEgtpCfmStatus(&(tnlEvt.cfmStatus), mBuf); 
   oduPackUInt32(&(tnlEvt.remTeid), mBuf);
   oduPackUInt32(&(tnlEvt.lclTeid), mBuf);
   oduPackUInt8(&(tnlEvt.action), mBuf);
 
   return ((* func)(tnlEvt));
 
}

/*******************************************************************
 *
 * @brief Packs Polling requesting and sends to EGTP
 *
 * @details
 *
 *    Function : packEgtpStartPollingReq
 *
 *    Functionality:
 *       Packs polling request and sends to EGTP
 *
 * @params[in] Post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
uint8_t packEgtpStartPollingReq(Pst *pst)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to allocate memory");
      return RFAILED;
   }

   ODU_POST_TASK(pst, mBuf);
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
