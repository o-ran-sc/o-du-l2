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

     Name:     common library functions
  
     Type:     C source file
  
     Desc:     Common functions that are implemented in
               both a portable and a performance-efficient manner. These
               functions are selected based on the operating system.

     File:     cm_lib.c

*********************************************************************21*/

/***********************************************************************
 * This file provides memory and string library functions implemented in
 * one of two ways:
 *
 *     1. portable: does not assume library support for any of the functions
 *                  needed.
 *
 *     2. using environment specific: this uses existing library
 *        functions and is therefore, for an environment where such
 *        functionality is available and , a more desirable choice!
 *
 *  The following functions are available in this file:
 *
 *       1. cmMemcpy: copies specified number of octets from one memory
 *          location to another.
 *       2. cmMemcmp: compares specified number of octets at one memory
 *          locaton with the same number from another.
 *       3. cmMemset: sets specified number of octets starting at given
 *          memory location with the given value.
 *       4. cmStrCmp: compares two strings, until the '\0' character is
 *          encountered.
 *       5. cmStrncmp: compares two strings, until \0 is encountered or
 *          until the specified length is exceeded.
 *       6. cmStrlen: finds length of a string until the \0 character.
 *
 *  The following flags are used in this file:
 *
 *     1. DONT_USE_SYS_LIB: If defined, this will compile in the portable
 *        versions of each function in this file. This overrides all other
 *        flags defined in this file.
 *
 *  For the SOLARIS environment:
 *
 *     The functionality defined in this file is generally available as library
 *     functions in the native operating systems' libraries. We have provided
 *     the functionality using the SOLARIS libraries, in this file.
 *
 *     If you want to take advantage of these library functions, all you need
 *     to do is the following:
 *
 *     1. Do not turn on the DONT_USE_SYS_LIB flag.
 *     2. Turn on the SUNOS51 flag.
 *
 *     This, in turn, turns on the following flags
 *
 *        1. MEMCPY_AVAIL  : maps cmMemcpy to C library function memcpy
 *        2. MEMCMP_AVAIL  : maps cmMemcmp to C library function memcmp
 *        3. MEMSET_AVAIL  : maps cmMemset to C library function memset
 *        4. STRCMP_AVAIL  : maps cmStrcmp to Strings library function strcmp
 *        5. STRNCMP_AVAIL : maps cmStrncmp to Strings library function strncmp
 *        5. STRLEN_AVAIL  : maps cmStrlen to Strings library function strlen
 *
 *  For an environment different from SOLARIS:
 *        
 *    You will need to modify this file to do more or less the same stuff
 *    as has been done for Solaris. i.e.
 *
 *     1. create a section inside the #ifndef DONT_USE_SYS_LIB section
 *        similar to the #ifdef SUNOS51 section that:
 *             1. includes system header files.
 *             2. defines MEMCPY_AVAIL etc. as needed.
 *     2. modify code inside functions to make use of the system library
 *        primitive.
 *     3. communicate your changes to Trillium so they can be incorporated
 *        in the next release of this file
 *
 *  To add a new library primitive to this file:
 *     1. it should be implemented in both a portable and environment specific
 *        manner.
 *     2. the portable implementation will not be the default
 *     3. the portable version and the environment specif versions must be
 *        enclosed in #ifdef XXXXXX_AVAIL
 *                     <environment specific implementation>
 *                    #else
 *                     <portable implementation>
 *                    #endif
 *     4. It must be communicated to Trillium so it will be included in the
 *        next release of the file.
 *     5. Trillium engineering must have all changes approved by engineering
 *        management.
 *     6. Trillium reserves the right to not incorporate any changes submitted
 *        by customers, if not approved by Trillium engineering management.
 *     7. Trillium reserves the right to modify code submitted by customers to
 *        enhance this file.
 ************************************************************************/

/*cm_lib_c_001.main_13 - Moved env files inclusion to the top*/
#include "envopt.h"        /* environment options */
#include "envind.h"        /* environment options */
#include "envdep.h"        /* environment options */

#ifndef DONT_USE_SYS_LIB

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MEMCPY_AVAIL   1
#define MEMCMP_AVAIL   1
#define MEMSET_AVAIL   1
#define STRCMP_AVAIL   1
#define STRNCMP_AVAIL  1
#define STRLEN_AVAIL   1

#else  /* DONT_USE_SYS_LIB */
#define MEMCPY_AVAIL   0
#define MEMCMP_AVAIL   0
#define MEMSET_AVAIL   0
#define STRCMP_AVAIL   0
#define STRNCMP_AVAIL  0
#define STRLEN_AVAIL   0
#endif /* not DONT_USE_SYS_LIB */


 
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

/* header/extern include files (.x) */
 
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_lib.x"        /* prototypes of primitives in this file */

#if (ERRCLASS & ERRCLS_DEBUG)

#define ECMLIBBASE     0
#define ECMLIB001      ECMLIBBASE + 1
#define ECMLIB002      ECMLIBBASE + 2
#define ECMLIB003      ECMLIBBASE + 3
#define ECMLIB004      ECMLIBBASE + 4
#define ECMLIB005      ECMLIBBASE + 5
#define ECMLIB006      ECMLIBBASE + 6
#define ECMLIB007      ECMLIBBASE + 7

#define CMLIBERR(_eCode, _eVal, _eDesc) \
    SLogError ((Ent) 0, (Inst)0, (ProcId)0, __FILE__, __LINE__, \
               (ErrCls)ERRCLS_DEBUG, (ErrCode)_eCode, (ErrVal) _eVal, \
               (Txt *) _eDesc)
#else
#define CMLIBERR(_eCode, _eVal, _eDesc)
#endif

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
Data *startPtr128;
Size regMemSize;
#endif

/*
*
*       Fun:   cmMemcpy
*
*       Desc:  common primitive to copy a contiguous string of bytes
*              optimized for when memcpy is available. It uses memcpy
*              when available. Otherwise, copies in a 'for' loop.
*
*              sets "len" memory locations starting from "tgt" to the values
*              of corresponding memory locations starting from "src".
*
*       Ret:   pointer to target string
*
*       Notes: None
*
*       File:  cm_lib.c
*
*/
uint8_t *cmMemcpy(uint8_t *tgt,const uint8_t *src,PTR len)
{
   /*cm_lib_c_001.main_14 : Fix for TRACE5 feature crash due to missing TRC MACRO*/

#ifdef MS_MBUF_CORRUPTION /* checking for valid memory address */
if ((tgt > startPtr128) && (tgt < (startPtr128+regMemSize)))
{
   if ((*((uint32_t *)(tgt + 4)) == 0xDEADDEAD) || (*((uint32_t *)(tgt + 24)) == 0xDEADDEAD) ||
      (*((uint32_t *)(tgt + 44)) == 0xDEADDEAD) || (*((uint32_t *)(tgt + 80)) == 0xDEADDEAD))
   {
      Data *crashPtr=NULLP;
      *crashPtr = 9;
   }
}
if ((src > startPtr128) && (src < (startPtr128+regMemSize)))
{
   if ((*((uint32_t *)(src + 4)) == 0xDEADDEAD) || (*((uint32_t *)(src + 24)) == 0xDEADDEAD) ||
      (*((uint32_t *)(src + 44)) == 0xDEADDEAD) || (*((uint32_t *)(src + 80)) == 0xDEADDEAD))
   {
      Data *crashPtr=NULLP;
      *crashPtr = 9;
   }
}
#endif 
#if (MEMCPY_AVAIL) /* memcpy is available */
   return ( memcpy(tgt, src, len));
#else
   while (len--)
      *tgt++ = *src++;

   return (tgt);
#endif /* MEMCPY_AVAIL */

} /* end of cmMemcpy */


/*
*
*       Fun:   cmMemcmp
*
*       Desc:  common primitive to compare a contiguous string of bytes
*              optimized for when memcmp is available. It uses memcmp
*              when available. Otherwise, compares in a 'for' loop.
*
*       Ret:    < 0 => s1 < s2
*               > 0 => s1 > s2
*               = 0 => s1 = s2
*
*       Notes: None
*
*       File:  cm_lib.c
*
*/
S16 cmMemcmp(const uint8_t *s1,const uint8_t *s2,PTR len)
{
   /*cm_lib_c_001.main_14 : Fix for TRACE5 feature crash due to missing TRC MACRO*/
#if MEMCMP_AVAIL /* memcmp is available */
   return ((S16) memcmp((const Void *)s1, (const Void *)s2, (size_t)len));
#else  /* MEMCMP_AVAIL: memcmp is not available */
   while (len--)
   {
      if (*s1 ^ *s2)
         return ((S16) (*s1 - *s2));
      s1++;
      s2++;
   }
   return (0);
#endif /* MEMCMP_AVAIL */
} /* end of cmMemcmp */


/*
*
*       Fun:   cmMemset
*
*       Desc:  common primitive to set a contiguous string of bytes
*              with a specified value optimized for when memset is available.
*              It uses memset when available. Otherwise, uses a 'for' loop.
*
*              sets "len" memory locations starting from "str" to the value
*              "val".
* 
*       Ret:   pointer to string
*
*       Notes: None
*
*       File:  cm_lib.c
*
*/
uint8_t *cmMemset(uint8_t *str,uint8_t val,PTR len)
{
   /*cm_lib_c_001.main_14 : Fix for TRACE5 feature crash due to missing TRC MACRO*/
#if MS_MBUF_CORRUPTION /* checking for valid memory address */
if ((str > startPtr128) && (str < (startPtr128+regMemSize)))
{
   if ((*((uint32_t *)(str + 4)) == 0xDEADDEAD) || (*((uint32_t *)(str + 24)) == 0xDEADDEAD) ||
      (*((uint32_t *)(str + 44)) == 0xDEADDEAD) || (*((uint32_t *)(str + 80)) == 0xDEADDEAD))
   {
      Data *crashPtr=NULLP;
      *crashPtr = 9;
   }
}
#endif
#if (MEMSET_AVAIL) /* memset is available */
   if (val==0)
   {  
      bzero((void *)str,(size_t)len);
   }
   else
   {
      memset(str,val, len);
   }
#else  /* MEMSET_AVAIL: memset is not available */
   while (len --)
      *str++ = val;

#endif /* MEMSET_AVAIL */
   return (str);
} /* end of cmMemset */


/*
*
*       Fun:   cmStrcmp
*
*       Desc:  common primitive to compare a contiguous string of characters
*              terminated by the '\0' character.
*
*              when strcmp is available, it uses that. otherwise, it
*              compares the strings using a for loop.
*
*              The following is the "strcmp" description from the SunOS 5.4
*              man-page. cmStrcmp follows this.
*
*             strcmp() compares two strings byte-by-byte, according to the
*             ordering  of  your  machine's  character  set.  The function
*             returns an integer greater than, equal to, or less  than  0,
*             if the string pointed to by s1 is greater than, equal to, or
*             less than the string pointed to by s2 respectively. The sign
*             of  a non-zero return value is determined by the sign of the
*             difference between the values of the  first  pair  of  bytes
*             that  differ in the strings being compared.
*
*             Bytes following a null byte are not compared.
*
*
*       Ret:    < 0 => s1 < s2
*               > 0 => s1 > s2
*               = 0 => s1 = s2
*
*       Notes: None
*
*       File:  cm_lib.c
*
*/
S16 cmStrcmp(const uint8_t *s1,const uint8_t *s2)
{
   /*cm_lib_c_001.main_14 : Fix for TRACE5 feature crash due to missing TRC MACRO*/
#if (STRCMP_AVAIL)
   return (strcmp((const char *)s1, (const char *)s2));
#else   /* STRCMP_AVAIL */
  
   while (*s1 && *s2)
   {
      if (*s1 ^ *s2)
         return (*s1 - *s2);
      s1++;
      s2++;
   }
   return (0);
#endif      /* strcmp is not available */

} /* end of cmStrcmp */



/*
*
*       Fun:   cmStrncmp
*
*       Desc:  common primitive to compare a contiguous string of characters
*              terminated by the '\0' character.
*              
*              when strncmp is available, it uses that. otherwise, it
*              compares the strings using a for loop.
*              
*              The following is the "strncmp" description from the SunOS 5.4
*              man-page. cmStrncmp follows this.
*
*              strcmp() compares two strings byte-by-byte, according to the
*              ordering  of  your  machine's  character  set.  The function
*              returns an integer greater than, equal to, or less  than  0,
*              if the string pointed to by s1 is greater than, equal to, or
*              less than the string pointed to by s2 respectively. The sign
*              of  a non-zero return value is determined by the sign of the
*              difference between the values of the  first  pair  of  bytes
*              that  differ in the strings being compared.  strncmp() makes
*              the same comparison but looks  at  a  maximum  of  n  bytes.
*              Bytes following a null byte are not compared.
*
*       Ret:    < 0 => s1 < s2
*               > 0 => s1 > s2
*               = 0 => s1 = s2
*
*       Notes: None
*
*       File:  cm_lib.c
*
*/
S16 cmStrncmp
(
const uint8_t  *s1,
const uint8_t  *s2,
MsgLen       len /* cm_lib_c_001.main_12: Changing from S16 to MsgLen.*/
)
{
   /*cm_lib_c_001.main_14 : Fix for TRACE5 feature crash due to missing TRC MACRO*/
#if (STRNCMP_AVAIL)
   return (strncmp((const char *)s1, (const char *)s2, (size_t) len));
#else   /* STRNCMP_AVAIL */
  
   while (*s1 && *s2 && len--)
   {
      if (*s1 ^ *s2)
         return (*s1 - *s2);
      s1++;
      s2++;
   }
   return (0);
#endif   /* strncmp is not available */
} /* end of cmStrncmp */


/*
*
*       Fun:   cmStrlen
*
*       Desc:  common primitive to compute the length of a NULL terminated
*              string.
*              
*              when strlen is available, it uses that. otherwise, it
*              inspects the string using a for loop.
*              
*              The following is the "strlen" description from the SunOS 5.4
*              man-page. cmStrlen follows this.
*
*              strlen() returns the number of bytes in s, not including the
*              terminating null character.
*
*       Ret:   length of string
*
*       Notes: None
*
*       File:  cm_lib.c
*
*/
MsgLen cmStrlen(const uint8_t *s)
{
#if (STRLEN_AVAIL)
   /*cm_lib_c_001.main_15 : Fix for warning due to mixed declation*/
   return ((MsgLen)strlen((const char *)s));
#else   /* STRLEN_AVAIL */
   MsgLen i;
  
   /*cm_lib_c_001.main_15 : Fix for warning due to mixed declation*/

   for (i = 0; *s; i++, s++);
   return (i);
#endif   /* strlen is not available */
} /* end of cmStrlen */

/**********************************************************************
         End of file
**********************************************************************/
