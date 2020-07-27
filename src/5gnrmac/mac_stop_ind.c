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
/* header include files (.h) */
#include "common_def.h"
#include "tfu.h"           /* RGU Interface includes */
#include "lrg.h"
#include "tfu.x"           /* RGU Interface includes */
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac.h"
#include "du_log.h"

/*******************************************************************
 * @brief process Stop indication to MAC
 *
 * @details
 *
 *     Function : fapiMacStopInd 
 *      
 *  @param[in]  Pst            *pst
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 ******************************************************************/
PUBLIC S16 fapiMacStopInd(Pst *pst) 
{
   uint8_t ret = ROK;
   ret = sendStopIndMacToDuApp();
   return ret;
}
