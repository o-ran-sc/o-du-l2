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

#define MAX_NUM_TMR_PER_STATS_GRP 1

#define EVENT_STATISTICS_TMR 1

bool schChkTmr(PTR cb, int16_t tmrEvnt);
void schStartTmr(SchCb *gCb, PTR cb, int16_t tmrEvnt, uint16_t timerValue);
void schStopTmr(SchCb *gCb, PTR cb, uint8_t tmrType);

/**********************************************************************
  End of file
 **********************************************************************/

