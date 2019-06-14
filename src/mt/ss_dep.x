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
 
     Name:     System Services -- implementation specific definitions
 
     Type:     C include file
 
     Desc:     This file conditionally includes implementation
               specific files.
 
     File:     ss_dep.x
 
*********************************************************************21*/


#ifndef __SSDEPX__
#define __SSDEPX__

#ifdef __cplusplus
extern "C" {
#endif

/*ss014.301 : 4GMX SSI specific includes*/
#ifdef SS_4GMX_LCORE
#else

/*ss011.301 : RMI SSI specific includes*/
#ifdef SS_RMIOS
#include "cm5.x"
#include "uc_ss.x"
#else

#ifdef SS_MT
#include "cm5.x"
#include "mt_ss.x"
#else /* not SS_MT */

#ifdef SS_PS
#include "cm5.x"
#include "ps.x"
#include "ps_ss.x"
#else /* not SS_PS */

#ifdef SS_VW
#include "vw_ss.x"
#include "cm5.x"
#else /* not SS_VW */

#ifdef NS
#include "ns_task.x"
#include "ns_timer.x"
#include "cm5.x"
#include "ns_gen.x"
#else /* not NS */

#ifdef SS_SK
#include "sk_ss.x"
#else /* not SS_SK */

#error "Specify an implementation"

#endif /* SS_SK */
#endif /* NS */
#endif /* SS_VW */
#endif /* SS_PS */
#endif /* SS_MT */
#endif /* SS_RMIOS*/
#endif /* SS_4GMX_LCORE*/


#ifdef __cplusplus
}
#endif

#endif  /*  __SSDEPX__  */



/********************************************************************30**

         End of file
**********************************************************************/
