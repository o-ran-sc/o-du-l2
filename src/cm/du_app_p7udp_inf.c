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

#include "common_def.h"
#include "du_app_p7udp_inf.h"

/*******************************************************************
*
* @brief Packs and Sends NFAPI's UDP P7 Interface's Config 
*
* @details
*
*    Function : packDuNfapiP7UdpCfg
*
*    Functionality:
*       Packs and Sends NFAPI's UDP P7 Interface's Configuration 
*
*
* @params[in] Post structure pointer
*             NfapiP7UdpCfg pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuNfapiP7UdpCfg(Pst *pst,  NfapiP7UdpCfg *nfapiP7UdpCfg)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("ERROR  --> MAC : Memory allocation failed at packDuNfapiP7UdpCfg");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)nfapiP7UdpCfg, mBuf);
    }
    else
    {
       DU_LOG("ERROR  -->  MAC: Only LWLC supported for packDuNfapiP7UdpCfg");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks NFAPI's UDP P7 Interface's Configuration 
*
* @details
*
*    Function : unpackDuNfapiP7UdpCfg
*
*    Functionality:
*         Unpacks NFAPI's UDP P7 Interface's Configuration 
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuNfapiP7UdpCfg(DuNfapiP7UdpCfgFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       NfapiP7UdpCfg *nfapiP7UdpCfg = NULLP;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&nfapiP7UdpCfg, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, nfapiP7UdpCfg);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("ERROR  -->  DU APP : Only LWLC supported for UDP P7 Config Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/**********************************************************************
  End of file
 **********************************************************************/
