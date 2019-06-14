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
 
     Desc:     This file conditionally includes various implementation
               specific files.
 
     File:     ss_dep.h

*********************************************************************21*/


#ifndef __SSDEPH__
#define __SSDEPH__

#ifdef __cplusplus
extern "C" {
#endif

/*ss014.301 : 4GMX SSI specific includes*/
#ifdef SS_4GMX_LCORE
#include "rb_include.h"
#include <stdio.h>
#include <string.h>
#else
/*ss011.301 : RMI SSI specific includes*/
#ifdef SS_RMIOS
#include <stdio.h>
#include "uc_ss.h"
#include "string.h"
#include "byteorder.h"
#include "cm5.h"
#else
#ifdef SS_MT
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include "cm5.h"
#include "mt_ss.h"
  /* ss003.13: addition */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#else /* not SS_MT */

#ifdef SS_PS
#include <psos.h>
#include "cm5.h"
#include "ps.h"
#include "ps_ss.h"
  /* ss009.13: addition */
#include <string.h>
#else /* not SS_PS */

#ifdef SS_VW
  /* ss002.13: addition */
#include <string.h>
#include "vw_ss.h"
#include <sys/times.h>
#include <ioLib.h>
#include <sockLib.h>
#include <selectLib.h>
#include <hostLib.h>
#include "cm5.h"
#else /* not SS_VW */

#ifdef NS
#ifdef FAR
#undef FAR
#endif
#ifdef NEAR
#undef NEAR
#endif
#ifdef NU 
#include <stdio.h>
#include "windows.h"
#include "winbase.h"
#endif /* NU */

#ifdef NK
#include "ntddk.h"
#endif /* NK */
#include "ns_err.h"
#include "ns_task.h"
#include "ns_timer.h"
#include "ns_gen.h"
#else /* not NS */

#ifdef SS_SK
#include "sk_ss.h"
#else /* not SS_SK */

#error "Specify an implementation"

#endif /* SS_SK */
#endif /* NS */
#endif /* SS_VW */
#endif /* SS_PS */
#endif /* SS_MT */
#endif /* SS_RMIOS */
#endif /* SS_4GMX_LCORE */


#ifdef __cplusplus
}
#endif

#endif  /*  __SSDEPH__  */


  
/********************************************************************30**

         End of file
**********************************************************************/
