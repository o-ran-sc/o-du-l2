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

/* This file contains DU APP and SCTP interface functions */

#include "du_sctp.h"

/*******************************************************************
 *
 * @brief Packs SCTP configuration 
 *
 * @details
 *
 *    Function : cmPkSctpAssocReq
 *
 *    Functionality:
 *       Packs SCTP configuration
 *
 * @params[in] Notification 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 cmPkSctpAssocReq(Pst *pst, F1SctpParams sctpCfgParam)
{
   Buffer *mBuf;

   if(SGetMsg(DFLT_REGION, DU_POOL, &mBuf) != ROK)
   {
      printf("\nDU_APP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }
  
   if(sctpCfgParam.duIpAddr.ipV4Pres)
   {
      SPkU32(sctpCfgParam.duIpAddr.ipV4Addr, mBuf);
   }
   cmPkBool(sctpCfgParam.duIpAddr.ipV4Pres, mBuf);
   SPkU16(sctpCfgParam.duPort, mBuf);

   if(sctpCfgParam.cuIpAddr.ipV4Pres)
   {
      SPkU32(sctpCfgParam.cuIpAddr.ipV4Addr, mBuf);
   }
   cmPkBool(sctpCfgParam.cuIpAddr.ipV4Pres, mBuf);
   SPkU16(sctpCfgParam.cuPort, mBuf);

   SPstTsk(pst, mBuf);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Unpacks SCTP configuration 
 *
 * @details
 *
 *    Function : cmUnpkSctpAssocReq
 *
 *    Functionality:
 *       Unpacks SCTP configuration
 *
 * @params[in] Notification 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 cmUnpkSctpAssocReq(Buffer *mBuf) 
{
   cmMemset((U8*)&sctpCfg, 0, sizeof(F1SctpParams));  

   SUnpkU16(&(sctpCfg.cuPort), mBuf);
   cmUnpkBool(&(sctpCfg.cuIpAddr.ipV4Pres), mBuf);
   if(sctpCfg.cuIpAddr.ipV4Pres)
   {
      SUnpkU32(&(sctpCfg.cuIpAddr.ipV4Addr), mBuf);
   }
   
   SUnpkU16(&(sctpCfg.duPort), mBuf);
   cmUnpkBool(&(sctpCfg.duIpAddr.ipV4Pres), mBuf);
   if(sctpCfg.duIpAddr.ipV4Pres)
   {
      SUnpkU32(&(sctpCfg.duIpAddr.ipV4Addr), mBuf);
   }

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Packs SCTP notification 
 *
 * @details
 *
 *    Function : cmPkSctpNtfy
 *
 *    Functionality:
 *       Packs SCTP notification
 *
 * @params[in] Notification 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 cmPkSctpNtfy(Pst *pst, CmInetSctpNotification *ntfy)
{
   Buffer *mBuf;

   if(SGetMsg(DU_APP_MEM_REGION, DU_POOL, &mBuf) != ROK)
   {
      printf("\nSCTP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }

   if(ntfy->header.nType == CM_INET_SCTP_ASSOC_CHANGE)
   {
      SPkU16(ntfy->u.assocChange.state, mBuf);
   }
   SPkU16(ntfy->header.nType, mBuf);

   if (SPstTsk(pst, mBuf) != ROK)
   {
      printf("\nSCTP : SPstTsk failed while sending SCTP notification");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Unpacks SCTP notification 
 *
 * @details
 *
 *    Function : cmUnpkSctpNtfy 
 *
 *    Functionality:
 *       Unpacks SCTP notification 
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 cmUnpkSctpNtfy(SctpNtfy func, Pst *pst, Buffer *mBuf)
{
   CmInetSctpNotification ntfy;
   cmMemset((U8*)&ntfy, 0, sizeof(CmInetSctpNotification));

   SUnpkU16(&(ntfy.header.nType), mBuf);
   if(ntfy.header.nType == CM_INET_SCTP_ASSOC_CHANGE)
   {
      SUnpkU16(&(ntfy.u.assocChange.state), mBuf);
   }

   RETVALUE((*func)(mBuf, &ntfy));
}

/**********************************************************************
         End of file
**********************************************************************/
