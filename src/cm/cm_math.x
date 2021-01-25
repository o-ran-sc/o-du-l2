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

     Name:     common math library

     Type:     C include file

     Desc:     Extern defintions

     File:     cm_math.x

*********************************************************************21*/

#ifndef __CMMATHX__
#define __CMMATHX__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SS_FLOAT

/* environment dependent include files */
#include <math.h>

/* Math library function prototypes */
uint32_t cmAbs ARGS((F64 val));
F64 cmPow ARGS((F64 x, F64 y));
F64 cmFloor ARGS((F64 x));
F64 cmLog ARGS((F64 x));
F64 cmLog10 ARGS((F64 x));

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CMMATHX__ */ 


/********************************************************************30**

         End of file
**********************************************************************/
