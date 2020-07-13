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

/* This file contains all F1AP message handler related functionality */
#include<ProtocolIE-Field.h>
#include "ProtocolExtensionField.h"
#include "F1AP-PDU.h"
#include "Cells-to-be-Activated-List.h"
#include "DL-CCCH-Message.h"
#include "SRB-ToAddModList.h"
#include "SRB-ToAddMod.h"
#include "RRCSetup-IEs.h"
#include "RRCSetup.h"
#include "du_log.h"

#define ENCODE_FAIL -1
#define TRANS_ID 1
#define RRC_SIZE 1
#define ENC_BUF_MAX_LEN 100
#define SUL_BAND_COUNT 0
#define UL_SRBID        1
#define DL_SRBID        0
#define DU_ID           1
#define CU_ID           1
#define CRNTI           17017
#define CELL_INDEX      0
#define SRB1 1

/* allocate and zero out a static buffer */
#define CU_ALLOC(_datPtr, _size)                                \
{                                                               \
   S16 _ret;                                                    \
   _ret = SGetSBuf(CU_APP_MEM_REG, CU_POOL,                  \
                    (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      cmMemset((U8*)_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}

/* free a static buffer */
#define CU_FREE(_datPtr, _size)                                 \
   if(_datPtr)                                                  \
      SPutSBuf(CU_APP_MEM_REG, CU_POOL,                      \
         (Data *)_datPtr, _size);


typedef struct f1apDb
{
   OCTET_STRING_t duToCuContainer;
}F1apMsgDb;
  
F1apMsgDb f1apMsgDb;
void F1APMsgHdlr(Buffer *mBuf);

/**********************************************************************
         End of file
**********************************************************************/
