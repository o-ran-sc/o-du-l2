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
  
     Desc:     Token Structures, required by two or more layers
   
     File:     cm_tkns.x
  
*********************************************************************21*/

#ifndef __CMTKNSX
#define __CMTKNSX

#ifdef __cplusplus
extern "C" {
#endif


/* typedefs */

/* Token Present */
typedef TknUInt8 TknPres;
/* Do not remove TknBool definition. If cm_gprs.x is used then include this
 * file before cm_gprs.x */
typedef TknUInt8 TknBool;

typedef struct tknS16        
{
   uint8_t   pres;                     /* present flag */
   uint8_t   spare1;                   /* for alignment */
   S16  val;                      /* value */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
} TknS16;

/* Token TknStr8 */
typedef struct tknStr8            /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint8_t   val[8];                  /* string value - 4 byte alligned */
} TknStr8;

/* Token TknStr16 */
typedef struct tknStr16            /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint8_t   val[16];                  /* string value - 4 byte alligned */
} TknStr16;

typedef struct tknStrOSXL
{
   uint8_t   pres;                    /* present flag */
   uint8_t   spare1;                  /* spare */
   uint16_t   len;                    /* length */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                  /* for 64 bit alignment */
#endif
   uint8_t   *val;                    /* string value - 4 byte alligned */
} TknStrOSXL;

/* Token TknBStr32 : length field indicates number of Bits present */
typedef TknStr4 TknBStr32;

/* Token TknStrBSXL : Len field indicates number of bits present */
typedef TknStrOSXL TknStrBSXL;


/*****************************************************
 Token structure for BMP Character Strings
******************************************************/

/* Token TknStrBMP4 */
typedef struct tknStrBMP4         /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint16_t   val[4];                  /* string value - 4 byte alligned */
} TknStrBMP4;

/* Token TknStrBMPXL */
typedef struct tknStrBMPXL
{
   uint8_t   pres;                    /* present flag */
   uint8_t   spare1;                  /* spare */
   uint16_t   len;                    /* length */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                  /* for 64 bit alignment */
#endif
   uint16_t   *val;                   /* string value - 4 byte alligned */
} TknStrBMPXL;


/*****************************************************
 Token structure for UNIVERSAL Character Strings
******************************************************/

/* Token TknStrUNI4 */
typedef struct tknStrUNI4         /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint32_t   val[4];                  /* string value - 4 byte alligned */
} TknStrUNI4;

typedef struct tknStrUNIXL       /* Universal String */
{
   uint8_t   pres;                    /* present flag */
   uint8_t   spare1;                  /* spare */
   uint16_t  len;                     /* length */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                  /* for 64 bit alignment */
#endif
   uint32_t  *val;                    /* string value- 4 byte alligned */ 
} TknStrUNIXL;

/* common packing functions */
S16 cmPkTknPres      ARGS((TknPres      *tknPres,       Buffer *mBuf));
S16 cmPkTknS16       ARGS((TknS16       *tknS16,        Buffer *mBuf));
S16 cmPkTknBStr32    ARGS((TknBStr32    *tknbStr32,     Buffer *mBuf));
S16 cmPkTknStr8      ARGS((TknStr8      *tknStr8,       Buffer *mBuf));
S16 cmPkTknStr16     ARGS((TknStr16     *tknStr16,      Buffer *mBuf));
S16 cmPkTknStrOSXL   ARGS((TknStrOSXL   *tknStrOSXL,    Buffer *mBuf));
S16 cmPkTknStrBSXL   ARGS((TknStrBSXL   *tknStrBSXL,    Buffer *mBuf));
S16 cmPkTknStrBMP4   ARGS((TknStrBMP4   *tknStrBMP4,    Buffer *mBuf));
S16 cmPkTknStrBMPXL  ARGS((TknStrBMPXL  *tknStrBMPXL,   Buffer *mBuf));
S16 cmPkTknStrUNI4   ARGS((TknStrUNI4   *tknStrUNI4,    Buffer *mBuf));
S16 cmPkTknStrUNIXL  ARGS((TknStrUNIXL  *tknStrUNIXL,   Buffer *mBuf));



/* common unpacking functions */
S16 cmUnpkTknPres      ARGS((TknPres      *tknPres,       Buffer *mBuf));
S16 cmUnpkTknS16       ARGS((TknS16       *tknS16,        Buffer *mBuf));
S16 cmUnpkTknBStr32    ARGS((TknBStr32    *tknBStr32,     Buffer *mBuf));
S16 cmUnpkTknStr8      ARGS((TknStr8      *tknStr8,       Buffer *mBuf));
S16 cmUnpkTknStr16     ARGS((TknStr16     *tknStr16,      Buffer *mBuf));
S16 cmUnpkTknStrOSXL   ARGS((TknStrOSXL   *tknStrOSXL,    Buffer *mBuf,
                                       Ptr ptr));
S16 cmUnpkTknStrBSXL   ARGS((TknStrBSXL   *tknStrBSXL,    Ptr ptr, 
                                       Buffer *mBuf));
S16 cmUnpkTknStrBMP4   ARGS((TknStrBMP4   *tknStrBMP4,    Buffer *mBuf));
S16 cmUnpkTknStrBMPXL  ARGS((TknStrBMPXL  *tknStrBMPXL,   Buffer *mBuf,
                                       Ptr ptr));
S16 cmUnpkTknStrUNI4   ARGS((TknStrUNI4   *tknStrUNI4,    Buffer *mBuf));
S16 cmUnpkTknStrUNIXL  ARGS((TknStrUNIXL  *tknStrUNIXL,   Buffer *mBuf,
                                       Ptr ptr));


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CMTKNSX */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
