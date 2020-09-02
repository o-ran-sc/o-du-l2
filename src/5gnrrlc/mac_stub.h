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

#ifdef EGTP_TEST

#ifndef __MAC_STUB_H__
#define __MAC_STUB_H__

#include "du_log.h" 
#include "rgu.h"
#include "kw.h"

#include "rgu.x"


#define DU_IP_V4_ADDR "192.168.130.81"
#define CU_IP_V4_ADDR "192.168.130.82"

uint8_t macStubBOStatus(Pst *pst, SpId spId, RlcMacBOStatus *boSta);
uint8_t macStubSendDlData(Pst *post, SpId spId, RlcMacData *dlData);
void macStubBuildUlData(Buffer *mBuf);

#endif /* __MAC_STUB_H__ */
#endif /* EGTP_TEST */

/**********************************************************************
         End of file
**********************************************************************/
