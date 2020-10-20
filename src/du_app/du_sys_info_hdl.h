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

/* Macros */
#define ODU_VALUE_ZERO 0
#define ODU_VALUE_ONE 1
#define ODU_VALUE_TWO 2
#define ODU_VALUE_THREE 3
#define ODU_VALUE_FOUR 4
#define ODU_VALUE_FIVE 5
#define ODU_VALUE_SEVEN 7
#define ODU_VALUE_EIGHT 8
#define ODU_VALUE_NINE  9 
#define ODU_VALUE_TEN  10

/* Function declarations */
uint8_t BuildMibPdu();
uint8_t BuildMibMsg();
uint8_t BuildSib1Msg();
char encBuf[ENC_BUF_MAX_LEN];
DuCfgParams duCfgParam;

/**********************************************************************
         End of file
**********************************************************************/
