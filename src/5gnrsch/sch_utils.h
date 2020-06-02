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

#include <stdint.h>

/* macros */
#define MAX_CORESET_INDEX      16
#define MAX_SEARCH_SPACE_INDEX 16
#define MAX_RACH_NUM_RB_IDX    16
#define MAX_PRACH_CONFIG_IDX   256
#define MAX_MU_PUSCH           4

#define SET_BITS(_startBit, _numBits, _byte)                                \
{                                                            \
   _byte = (((~((~0)<<_numBits))<<_startBit));               \
}

/* functions declarations */
void calculatePRB(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain);

extern int8_t coresetIdxTable[MAX_CORESET_INDEX][4];
extern int8_t searchSpaceIdxTable[MAX_SEARCH_SPACE_INDEX][4];

/**********************************************************************
         End of file
**********************************************************************/
