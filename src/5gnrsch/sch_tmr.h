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

#define MAX_TOTAL_PRB_USAGE_TMR 1

#define EVENT_DL_TOTAL_PRB_USAGE_TMR 1
#define EVENT_UL_TOTAL_PRB_USAGE_TMR 2

bool schChkTmr(PTR cb, int16_t tmrEvnt);
void schStartTmr(SchCb *gCb, PTR cb, int16_t tmrEvnt, uint8_t timerValue);
void schStopTmr(SchCb *gCb, PTR cb, uint8_t tmrType);

/**********************************************************************
  End of file
 **********************************************************************/

