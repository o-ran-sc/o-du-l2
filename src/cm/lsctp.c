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
#include "common_def.h"
#include "lsctp.h"

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

uint8_t cmPkSctpNtfy(Pst *pst, CmInetSctpNotification *ntfy)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP : Failed to allocate memory");
      return RFAILED;
   }

   if(ntfy->header.nType == CM_INET_SCTP_ASSOC_CHANGE)
   {
      oduUnpackUInt16(ntfy->u.assocChange.state, mBuf);
      oduUnpackUInt32(ntfy->u.assocChange.assocId, mBuf);
   }
   oduUnpackUInt16(ntfy->header.nType, mBuf);

   if (ODU_POST_TASK(pst, mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP : ODU_POST_TASK failed while sending SCTP notification");
      return RFAILED;
   }

   return ROK;
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

uint8_t cmUnpkSctpNtfy(SctpNtfy func, Pst *pst, Buffer *mBuf)
{
   CmInetSctpNotification ntfy;
   memset(&ntfy, 0, sizeof(CmInetSctpNotification));

   oduPackUInt16(&(ntfy.header.nType), mBuf);
   if(ntfy.header.nType == CM_INET_SCTP_ASSOC_CHANGE)
   {
      oduPackUInt32(&(ntfy.u.assocChange.assocId), mBuf);
      oduPackUInt16(&(ntfy.u.assocChange.state), mBuf);
   }

   return ((*func)(mBuf, &ntfy));
}

/**********************************************************************
         End of file
**********************************************************************/
