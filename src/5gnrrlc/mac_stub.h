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

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
 
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "cm_tpt.h"
#include "cm_inet.h"
#include "du_log.h" 
#include "rgu.h"
#include "kw.h"

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_tpt.x"
#include "cm_inet.x"
#include "rgu.x"


#define DU_IP_V4_ADDR "10.0.2.20"
#define CU_IP_V4_ADDR "10.0.2.25"

S16 macStubBOStatus(Pst *pst, SpId spId, RlcMacBOStatus *boSta);
S16 macStubSendDlData(Pst *post, SpId spId, RlcMacData *dlData);
void macStubBuildUlData(Buffer *mBuf);

#endif /* __MAC_STUB_H__ */
#endif /* EGTP_TEST */

/**********************************************************************
         End of file
**********************************************************************/
