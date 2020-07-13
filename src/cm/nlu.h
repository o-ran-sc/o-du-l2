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

/********************************************************************20**

     Name:     ESON layer management

     Type:     

     Desc:     This file contains the 
               

     File:     nlu.h

*********************************************************************21*/
#ifndef __NLUH__
#define __NLUH__

#define EVTNLUBNDREQ                           1
#define EVTNLUBNDCFM                           2
#define EVTNLUNHBRREQ                          3
#define EVTNLUPCIMODINDCFM                     4
#define EVTNLUPERCELLSRCHRSP                   5
#define EVTNLUUEMEASRPTIND                     6
#define EVTNLUUECQIRPTIND                      7
#define EVTNLUUEIND                            8

#define NLU_MAX_NGH_CELL_INFO                  16
#define NLU_MAX_REPORTS                        8
#define NLU_MAX_NGH_NGH_CELL_INFO              16
#define NLU_MAX_NGH_ENB_CFG                    16
/* Periodic REM for TPM */
#define NLU_REM_MAX_CELL_SEARCH                16                
/* Periodic REM for TPM End */
#define LWR_LTEU_MAX_EARFCN   10
#endif /* __NLUH__ */
/********************************************************************30**

           End of file
**********************************************************************/
