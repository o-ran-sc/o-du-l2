/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [Radisys]                                             #
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
/* Utility definitions to be used in du app */
#include "common_def.h"
#include "du_app_mac_inf.h"
#include "du_utils.h"

/* Spec Ref-38.214-Table 5.1.2.1-1 */
uint8_t slivCfgIdxTable[MAX_SLIV_CONFIG_IDX][3] = {
/*S    L     SLIV*/
{ 0,   1,    0  },/* index0  */
{ 0,   2,    14 },/* index1  */
{ 0,   3,    28 },/* index2  */
{ 0,   4,    42 },/* index3  */
{ 0,   5,    56 },/* index4  */
{ 0,   6,    70 },/* index5  */
{ 0,   7,    84 },/* index6  */
{ 0,   8,    98 },/* index7  */
{ 0,   9,    97 },/* index8  */
{ 0,   10,   83 },/* index9  */
{ 0,   11,   69 },/* index10 */
{ 0,   12,   55 },/* index11 */
{ 0,   13,   41 },/* index12 */
{ 0,   14,   27 },/* index13 */
{ 1,   1,    1  },/* index14 */
{ 1,   2,    15 },/* index15 */
{ 1,   3,    29 },/* index16 */
{ 1,   4,    43 },/* index17 */
{ 1,   5,    57 },/* index18 */
{ 1,   6,    71 },/* index19 */
{ 1,   7,    85 },/* index20 */
{ 1,   8,    99 },/* index21 */
{ 1,   9,    96 },/* index22 */
{ 1,   10,   82 },/* index23 */
{ 1,   11,   68 },/* index24 */
{ 1,   12,   54 },/* index25 */
{ 1,   13,   40 },/* index26 */
{ 2,   1,    2  },/* index27 */
{ 2,   2,    16 },/* index28 */
{ 2,   3,    30 },/* index29 */
{ 2,   4,    44 },/* index30 */
{ 2,   5,    58 },/* index31 */
{ 2,   6,    72 },/* index32 */
{ 2,   7,    86 },/* index33 */
{ 2,   8,    100},/* index34 */
{ 2,   9,    95 },/* index35 */
{ 2,   10,   81 },/* index36 */
{ 2,   11,   67 },/* index37 */
{ 2,   12,   53 },/* index38 */
{ 3,   1,    3  },/* index39 */
{ 3,   2,    17 },/* index40 */
{ 3,   3,    31 },/* index41 */
{ 3,   4,    45 },/* index42 */
{ 3,   5,    59 },/* index43 */
{ 3,   6,    73 },/* index44 */
{ 3,   7,    87 },/* index45 */
{ 3,   8,    101},/* index46 */
{ 3,   9,    94 },/* index47 */
{ 3,   10,   80 },/* index48 */
{ 3,   11,   66 },/* index49 */
{ 4,   1,    4  },/* index50 */
{ 4,   2,    18 },/* index51 */
{ 4,   3,    32 },/* index52 */
{ 4,   4,    46 },/* index53 */
{ 4,   5,    60 },/* index54 */
{ 4,   6,    74 },/* index55 */
{ 4,   7,    88 },/* index56 */
{ 4,   8,    102},/* index57 */
{ 4,   9,    93 },/* index58 */
{ 4,   10,   79 },/* index59 */
{ 5,   1,    5  },/* index60 */
{ 5,   2,    19 },/* index61 */
{ 5,   3,    33 },/* index62 */
{ 5,   4,    47 },/* index63 */
{ 5,   5,    61 },/* index64 */
{ 5,   6,    75 },/* index65 */
{ 5,   7,    89 },/* index66 */
{ 5,   8,    103},/* index67 */
{ 5,   9,    92 },/* index68 */
{ 6,   1,    6  },/* index69 */
{ 6,   2,    20 },/* index70 */
{ 6,   3,    34 },/* index71 */
{ 6,   4,    48 },/* index72 */
{ 6,   5,    62 },/* index73 */
{ 6,   6,    76 },/* index74 */
{ 6,   7,    90 },/* index75 */
{ 6,   8,    104},/* index76 */
{ 7,   1,    7  },/* index77 */
{ 7,   2,    21 },/* index78 */
{ 7,   3,    35 },/* index79 */
{ 7,   4,    49 },/* index80 */
{ 7,   5,    63 },/* index81 */
{ 7,   6,    77 },/* index82 */
{ 7,   7,    91 },/* index83 */
{ 8,   1,    8  },/* index84 */
{ 8,   2,    22 },/* index85 */
{ 8,   3,    36 },/* index86 */
{ 8,   4,    50 },/* index87 */
{ 8,   5,    64 },/* index88 */
{ 8,   6,    78 },/* index89 */
{ 9,   1,    9  },/* index90 */
{ 9,   2,    23 },/* index91 */
{ 9,   3,    37 },/* index92 */
{ 9,   4,    51 },/* index93 */
{ 9,   5,    65 },/* index94 */
{ 10,  1,    10 },/* index95 */
{ 10,  2,    24 },/* index96 */
{ 10,  3,    38 },/* index97 */
{ 10,  4,    52 },/* index98 */
{ 11,  1,    11 },/* index99 */
{ 11,  2,    25 },/* index100*/
{ 11,  3,    39 },/* index101*/
{ 12,  1,    12 },/* index102*/
{ 12,  2,    26 },/* index103*/
{ 13,  1,    13 } /* index104*/
};

/*******************************************************************
 *
 * @brief Function to fill the start Symbol and Symbol Len from the 
 * sliv Config Idx Table
 *
 * @details
 *
 *    Function : fillStartSymbolAndLen
 *
 *    Functionality: Function to fill the start Symbol and Symbol Len 
 *                   from the sliv Config Idx Table
 *
 * @params[in] startSymbolIdx pointer,
 *             symbolLen pointer,
 *             sliv
 * @return void
 *
 * ****************************************************************/
void fillStartSymbolAndLen(uint8_t numRsrcAlloc, PdschConfig *pdschCfg, PuschCfg *puschCfg)
{
   uint8_t slivIdx, timeDomIdx;

   for(timeDomIdx = 0; timeDomIdx < numRsrcAlloc; timeDomIdx++)
   {
      for(slivIdx = 0; slivIdx < MAX_SLIV_CONFIG_IDX; slivIdx++)
      {
         if(pdschCfg)/* PDSCH Config */
	 {
            if(pdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbolAndLength ==\
               slivCfgIdxTable[slivIdx][2])
            {
               pdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbol  = slivCfgIdxTable[slivIdx][0];
               pdschCfg->timeDomRsrcAllociList[timeDomIdx].symbolLength = slivCfgIdxTable[slivIdx][1];
               break;
            }
         }
	 if(puschCfg)/* PUSCH Config */
	 {
            if(puschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbolAndLength ==\
               slivCfgIdxTable[slivIdx][2])
            {
               puschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbol  = slivCfgIdxTable[slivIdx][0];
               puschCfg->timeDomRsrcAllocList[timeDomIdx].symbolLength = slivCfgIdxTable[slivIdx][1];
               break;
            }

	 }
      }
   }
}

/**********************************************************************
End of file
**********************************************************************/

