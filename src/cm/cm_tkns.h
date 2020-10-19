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
  
     Name:     general layer
  
     Type:     C include file
  
     Desc:     defines , required by cm_tkns.c and cm_tkns.h
   
     File:     cm_tkns.h
  
*********************************************************************21*/

#ifndef __CMTKNSH
#define __CMTKNSH


/* defines */
#define cmPkTknPres(x, mBuf)       cmPkTknUInt8(x, mBuf)

#define cmUnpkTknPres(x, mBuf)     cmUnpkTknUInt8(x, mBuf)


/* packing and unpacking for token bit strings */

#define CMPKTKNBSTR(tknStr, mBuf)                   \
{                                                  \
   Cntr i;                                         \
   Cntr len;                                       \
                                                   \
   if(tknStr->pres)                                \
   {                                               \
      if (tknStr->len % 8)                         \
         len = (tknStr->len/8 ) + 1;               \
      else                                         \
         len = (tknStr->len/8 );                   \
                                                   \
      /* Value */                                  \
      for (i = 0; i < (S16) len; i++)              \
      {                                            \
         CMCHKPK(oduUnpackUInt8, tknStr->val[i], mBuf);     \
      }                                            \
                                                   \
      /* Length */                                 \
      CMCHKPK(oduUnpackUInt8, tknStr->len, mBuf);           \
   }                                               \
                                                   \
   /* Token Header */                              \
   CMCHKPK(oduUnpackUInt8, tknStr->pres, mBuf);             \
}

#define CMUNPKTKNBSTR(tknStr, mBuf)                                 \
{                                                                  \
   Cntr i;                                                         \
   Cntr len;                                                       \
                                                                   \
   /* Token Header */                                              \
   CMCHKUNPK(oduPackUInt8, &tknStr->pres, mBuf);                        \
                                                                   \
   if(tknStr->pres)                                                \
   {                                                               \
     /* Length */                                                  \
      CMCHKUNPK(oduPackUInt8, &tknStr->len, mBuf);                      \
                                                                   \
      if (tknStr->len % 8)                                         \
         len = (tknStr->len/8 ) + 1;                               \
      else                                                         \
         len = (tknStr->len/8 );                                   \
                                                                   \
      /* Value */                                                  \
      for (i = 1; i <= (S16) len; i++)                             \
      {                                                            \
         CMCHKUNPK(oduPackUInt8, &tknStr->val[len - i], mBuf);          \
      }                                                            \
   }                                                               \
                                                                   \
}

#endif /* __CMTKNSH */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
