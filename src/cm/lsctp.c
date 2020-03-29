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

#include "du_mgr.h"
#include "du_sctp.h"


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
      SPkU16(ntfy->u.assocChange.assocId, mBuf);
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
      SUnpkU16(&(ntfy.u.assocChange.assocId), mBuf);
      SUnpkU16(&(ntfy.u.assocChange.state), mBuf);
   }

   RETVALUE((*func)(mBuf, &ntfy));
}

/**********************************************************************
         End of file
**********************************************************************/
