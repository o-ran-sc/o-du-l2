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

/* UL RRC Message from RLC to DU APP */
typedef struct ulRrcMsgInfo
{
   uint16_t   cellId;       /* Cell Id */
   uint16_t   ueIdx;        /* UE Index */
   uint8_t    lcId;         /* Logical channel Id */
   uint16_t   msgLen;       /* RRC message length (in bytes) */
   uint8_t    *rrcMsg;      /* RRC Message (UL-DCCH Message) */
}RlcUlRrcMsgInfo;

/* Function pointer */

typedef uint8_t (*RlcUlRrcMsgToDuFunc) ARGS((
   Pst           *pst,
   RlcUlRrcMsgInfo *ulRrcMsgInfo));

uint8_t packRlcUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t DuProcRlcUlRrcMsgTrans(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t unpackRlcUlRrcMsgToDu(RlcUlRrcMsgToDuFunc func, Pst *pst, Buffer *mBuf);

/**********************************************************************
         End of file
**********************************************************************/
