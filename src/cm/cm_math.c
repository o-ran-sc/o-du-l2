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

     Name:     common MATH library functions
  
     Type:     C source file
  
     Desc:     Common functions that are implemented in
               a portable manner. These functions are selected 
               based on the operating system.

     File:     cm_math.c

*********************************************************************21*/


/* header include files (.h) */
#include "envopt.h"             /* environment options */
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */
#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */

#include "cm_math.h"
#include <stdlib.h>
#include <math.h>

#ifdef SS_FLOAT
/*
*
*       Fun:   cmAbs
*
*       Desc:  common primitive for an absolute value of an integer
*
*       Ret:   absolute value of the operand 
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
uint32_t cmAbs(F64  val)
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   return ((uint32_t) abs((int) val));/* G++ */
#endif
} /* end of cmAbs */
#endif /* SS_FLOAT */


#ifdef SS_FLOAT
/*
*
*       Fun:   cmPow
*
*       Desc:  common primitive for computing the power of x raised to y.
*
*       Ret:   value of x raised to the power of y.
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
F64 cmPow(F64 x,F64 y)
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   return ((F64) pow((F64)x, (F64)y));
#endif
} /* end of cmPow */
#endif /* SS_FLOAT */

#ifdef SS_FLOAT
/*
*
*       Fun:   cmFloor
*
*       Desc:  common primitive for computing the largest integral value 
*              not greater than x.
*
*       Ret:   floor value of x 
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
F64 cmFloor(F64  x)
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   return ((F64) floor((F64)x));
#endif

} /* end of cmFloor */
#endif /* SS_FLOAT */

#ifdef SS_FLOAT
/*
*
*       Fun:   cmLog
*
*       Desc:  common primitive for computing the natural logrithm of x.
*
*       Ret:   loge(x)
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
F64 cmLog(F64 x)
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   return ((F64) log((F64)x));
#endif

} /* end of cmLog */
#endif /* SS_FLOAT */

#ifdef SS_FLOAT
/*
*
*       Fun:   cmLog10
*
*       Desc:  common primitive for computing the base 10 logrithm of x.
*
*       Ret:   log10(x)
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
F64 cmLog10(F64 x)
{
#if (defined(SUNOS)|| defined(WIN32)|| defined(WIN2K)|| defined (SS_LINUX))
   return ((F64) log10((F64)x));
#endif
} /* end of cmLog10 */
#endif /* SS_FLOAT */

/*
*
*       Fun:   cmLog
*
*       Desc:  common primitive for computing the base 2 logrithm of x.
*
*       Ret:   log2(x)
*
*       Notes: None
*
*       File:  cm_math.c
*
*/
float cmLog2(uint32_t x)
{
   return ((float) log2(x));
}

/**********************************************************************
         End of file
**********************************************************************/
