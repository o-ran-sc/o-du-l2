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

#define ENCODE_FAIL -1
#define TRANS_ID 1
#define RRC_SIZE 1
#define ENC_BUF_MAX_LEN 100
#define SUL_BAND_COUNT 0
#define UL_SRBID        1
#define DL_SRBID        0
#define DU_ID           1
#define RIC_ID           1
#define CRNTI           17017
#define CELL_INDEX      0


/* allocate and zero out a static buffer */
#define RIC_ALLOC(_datPtr, _size)                                \
{                                                               \
   uint8_t _ret;                                                    \
   _ret = SGetSBuf(RIC_APP_MEM_REG, RIC_POOL,                  \
                    (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      memset(_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}

/* free a static buffer */
#define RIC_FREE(_datPtr, _size)                                 \
   if(_datPtr)                                                  \
      SPutSBuf(RIC_APP_MEM_REG, RIC_POOL,                      \
         (Data *)_datPtr, _size);



void E2APMsgHdlr(Buffer *mBuf);
S16 BuildAndSendE2SetupReq();
S16 SendE2APMsg(Region , Pool );

/**********************************************************************
         End of file
**********************************************************************/
