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

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "gen.x"           /* General */
#include "ssi.h"
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "asn_codecs.h"

#define ENC_BUF_MAX_LEN 200
#define ENCODE_FAIL -1

typedef struct PlmnIdentity 
{
   U8 mcc[3];
   U8 mnc[3];
}Plmn;

S16 BuildPlmnId(Plmn plmn, OCTET_STRING_t *octe);
S16 FillBitString(BIT_STRING_t *id, U8 unusedBits, U8 byteSize, U8 val);

char encBuf[ENC_BUF_MAX_LEN];
int  encBufSize;


/*******************************************************************
 *
 * @brief Writes the encoded chunks into a buffer
 *
 * @details
 *
 *    Function : PrepFinalEncBuf
 *
 *    Functionality:Fills the encoded buffer
 *
 * @params[in] void *buffer,initial encoded data
 * @params[in] size_t size,size of buffer
 * @params[in] void *encodedBuf,final buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
static int PrepFinalEncBuf(const void *buffer, size_t size, void *encodedBuf)
{
   memcpy(encodedBuf + encBufSize, buffer, size);
   encBufSize += size;
   return 0;
} /* PrepFinalEncBuf */

#endif

/**********************************************************************
         End of file
**********************************************************************/
