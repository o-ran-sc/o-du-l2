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

#ifndef __ODU_COMMON_CODEC_H__
#define __ODU_COMMON_CODEC_H__

#define ENC_BUF_MAX_LEN 400
#define ENCODE_FAIL -1

char encBuf[ENC_BUF_MAX_LEN];
int  encBufSize;

int PrepFinalEncBuf(const void *buffer, size_t size, void *encodedBuf);
uint8_t buildPlmnId(Plmn plmn, OCTET_STRING_t *octe);
uint8_t fillBitString(BIT_STRING_t *id, uint8_t unusedBits, uint8_t byteSize, uint8_t val);
uint8_t bitStringToInt(BIT_STRING_t *bitString, uint16_t *val);

#endif

/**********************************************************************
         End of file
**********************************************************************/
