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

void deleteMacRaCb(uint16_t cellIdx, MacUeCb *ueCb);
uint8_t MacProcSchUeCfgRsp(Pst *pst, SchUeCfgRsp *cfgRsp);
uint8_t MacProcSchUeRecfgRsp(Pst *pst, SchUeRecfgRsp *reCfgRsp);
uint8_t MacProcSchUeDeleteRsp(Pst *pst, SchUeDeleteRsp *schUeDelRsp);

/**********************************************************************
  End of file
 **********************************************************************/
