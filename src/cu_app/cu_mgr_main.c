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

/* This functions contains main() for cu_app */

#include "cu_mgr_main.h"
#include "cu_sctp.h"

/*******************************************************************
 *
 * @brief Main function of CU APP
 *
 * @details
 *
 *    Function : main
 *
 *    Functionality:
 *         - Reads CU related configurations
 *         - Initialize SCTP Parameters
 *         - Start SCTP receiver thread
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void main()
{
   printf("\nStarting cu_app");
   
   /* Read CU configurations */
   readCuCfg();

   /* Initializing SCTP global parameters */
   sctpActvInit(0, 0, 0, 0);
 
   /* Start CU-SCTP to listen on incoming connection */
   sctpStartReq(); 
}

/**********************************************************************
         End of file
**********************************************************************/
