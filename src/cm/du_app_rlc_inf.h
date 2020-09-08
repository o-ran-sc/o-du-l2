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

#define EVENT_UL_RRC_MSG_TRANS_TO_DU  212

typedef struct ulRrcMsgInfo
{
   uint16_t   cellId;
   uint16_t   ueIdx;
   uint8_t    lcId;
   uint16_t   msgLen;
   uint8_t    *rrcMsg;
}RlcUlRrcMsgInfo;

/* Function pointer */
/* UL RRC Message transfer from RLC to DU APP*/
typedef uint8_t (*RlcUlRrcMsgToDuFunc) ARGS((
   Pst           *pst,
   RlcUlRrcMsgInfo *ulRrcMsgInfo));

uint8_t packRlcUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t DuProcRlcUlRrcMsgTrans(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t unpackRlcUlRrcMsgToDu(RlcUlRrcMsgToDuFunc func, Pst *pst, Buffer *mBuf);

/**********************************************************************
         End of file
**********************************************************************/
