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

/* This file contains all E2AP message handler related functionality */

typedef enum
{
   RIC_INITIATED,
   E2_NODE_INITIATED
}E2ProcedureDirection;

typedef struct e2apDb
{
   uint16_t     ricId;
   uint8_t      transIdCounter;
   uint8_t      *plmn;
   uint32_t     ricReqId;
   uint32_t     ricInstanceId;
   uint32_t     ranFuncId;
   uint8_t     *ricEventTrigger;
   uint32_t     ricActionId;
   uint32_t     ricActionType;
}E2apDb;

uint8_t SendE2APMsg(Region region, Pool pool, char *encBuf, int encBufSize);
/**********************************************************************
  End of file
 **********************************************************************/
