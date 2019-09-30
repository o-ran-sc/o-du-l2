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

/* This file contains handler for incoming F1AP message and sending of F1AP response */

#include "cu_mgr_main.h"
#include "cu_sctp.h"

/*******************************************************************
 *
 * @brief Handles received F1AP message and sends back response  
 *
 * @details
 *
 *    Function : cuAppInmsgHdlr
 *
 *    Functionality:
 *         - Decodes received F1AP control message
 *         - Prepares response message, encodes and sends to SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void cuAppInmsgHdlr(char *recvBuf, U8 len)
{
      char *respBuf;
      U8   respLen = 0;

   /* TODO : 
      call ASN decoder for incoming message
      Check message type and fills its response
      Call ASN encoder for response 
      Send Message to SCTP */ 
  
      sctpOutMsgSend(respBuf, respLen);

} /* End of cuAppInmsgHdlr */

/**********************************************************************
         End of file
**********************************************************************/
