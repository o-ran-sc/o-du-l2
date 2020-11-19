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
 
     Name:     System Services -- packing
 
     Type:     C source file
 
     Desc:     Source code for System Services packing functions.
 
     File:     ss_pack.c
 
*********************************************************************21*/


/* header include files (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

#include "ss_err.h"        /* errors */
#include "ss_dep.h"        /* implementation-specific */
#include "ss_queue.h"      /* queues */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "ss_gen.h"        /* general */
#include "cm_mem.h"        /* memory management */



/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */

#include "ss_dep.x"        /* implementation-specific */
#include "ss_queue.x"      /* queues */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#include "ss_drvr.x"       /* driver tasks */
#ifdef SS_LOCKLESS_MEMORY
#include "cm_llist.x"
#include "cm_hash.x"
#include "cm_mem_wl.x"        /* common memory manager */
#else
#include "cm_mem.x"        /* common memory manager */
#endif /* SS_LOCKLESS_MEMORY */
#include "ss_gen.x"        /* general */


  
/*
*
*       Fun:   SPkS8
*
*       Desc:  This function packs a signed 8 bit value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/

S16 SPkS8
(
S8 val,                     /* value */
Buffer *mBuf                /* message buffer */
)
{
   S16 ret;                 /* return code */
   ret = SAddPreMsg((Data) val, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS242, (ErrVal)ret, "SAddPreMsg() failed");
#endif
   return (ret);
} /* end of SPkS8 */
  

  
/*
*
*       Fun:   oduUnpackUInt8
*
*       Desc:  This function packs an unsigned 8 bit value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 oduUnpackUInt8
(
uint8_t val,                     /* value */
Buffer *mBuf                /* message buffer */
)
{
   S16 ret;                 /* return code */
   ret = SAddPreMsg((Data) val, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS243, (ErrVal)ret, "SAddPreMsg() failed");
#endif
   return (ret);
} /* end of oduUnpackUInt8 */
  
/*
*
*       Fun:   SPkS16
*
*       Desc:  This function packs a signed 16 bit value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SPkS16
(
S16 val,                    /* value */
Buffer *mBuf                /* message buffer */
)
{
   Data pkArray[2];         /* array for packing */
   S16 ret;                 /* return code */

#ifndef FCSPKINT            /* backward compatibility, packing order */
   pkArray[0] = (Data) GetHiByte(val);
   pkArray[1] = (Data) GetLoByte(val);
#else                   /* forward compatibility, packing order */
   pkArray[1] = (Data) GetHiByte(val);
   pkArray[0] = (Data) GetLoByte(val);
#endif
   ret = SAddPreMsgMult(pkArray, (MsgLen) 2, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS244, (ErrVal)ret, "SAddPreMsgMult() failed");
#endif
   return (ret);
} /* end of SPkS16 */
  
  
/*
*
*       Fun:   oduUnpackUInt16
*
*       Desc:  This function packs an unsigned 16 bit value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 oduUnpackUInt16
(
uint16_t val,                    /* value */
Buffer *mBuf                /* message buffer */
)
{
   Data pkArray[2];         /* array for packing */
   S16 ret;                 /* return code */

#ifndef FCSPKINT            /* backward compatibility, packing order */
   pkArray[0] = (Data) GetHiByte(val);
   pkArray[1] = (Data) GetLoByte(val);
#else                       /* forward compatibility, packing order */
   pkArray[1] = (Data) GetHiByte(val);
   pkArray[0] = (Data) GetLoByte(val);
#endif
   ret = SAddPreMsgMult(pkArray, (MsgLen) 2, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS245, (ErrVal)ret, "SAddPreMsgMult() failed");
#endif
   return (ret);
} /* end of oduUnpackUInt16 */
  
  
/*
*
*       Fun:   SPkS32
*
*       Desc:  This function packs a signed 32 bit value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SPkS32
(
S32 val,                    /* value */
Buffer *mBuf                /* message buffer */
)
{
   uint16_t tmp;                 /* temporary value */
   Data pkArray[4];         /* packing array */
   S16 ret;                 /* return code */
  
#ifndef FCSPKINT        /* backward compatibility, packing order */
   tmp = (uint16_t) GetHiWord(val);
   pkArray[0] = (Data) GetHiByte(tmp);
   pkArray[1] = (Data) GetLoByte(tmp);
   tmp = (uint16_t) GetLoWord(val);
   pkArray[2] = (Data) GetHiByte(tmp);
   pkArray[3] = (Data) GetLoByte(tmp);
#else                   /* forward compatibility, packing order */
   tmp = (uint16_t) GetHiWord(val);
   pkArray[3] = (Data) GetHiByte(tmp);
   pkArray[2] = (Data) GetLoByte(tmp);
   tmp = (uint16_t) GetLoWord(val);
   pkArray[1] = (Data) GetHiByte(tmp);
   pkArray[0] = (Data) GetLoByte(tmp);
#endif
   ret = SAddPreMsgMult(pkArray, (MsgLen) 4, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS246, (ErrVal)ret, "SAddPreMsgMult() failed");
#endif
   return (ret);
} /* end of SPkS32 */
  
  
/*
*
*       Fun:   oduUnpackUInt32
*
*       Desc:  This function packs an unsigned 32 bit value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 oduUnpackUInt32
(
uint32_t val,                    /* value */
Buffer *mBuf                /* message buffer */
)
{
   uint16_t tmp;                 /* temporary value */
   Data pkArray[4];         /* packing array */
   S16 ret;                 /* return code */
  
#ifndef FCSPKINT        /* backward compatibility, packing order */
   tmp = (uint16_t) GetHiWord(val);
   pkArray[0] = (Data) GetHiByte(tmp);
   pkArray[1] = (Data) GetLoByte(tmp);
   tmp = (uint16_t) GetLoWord(val);
   pkArray[2] = (Data) GetHiByte(tmp);
   pkArray[3] = (Data) GetLoByte(tmp);
#else                   /* forward compatibility, packing order */
   tmp = (uint16_t) GetHiWord(val);
   pkArray[3] = (Data) GetHiByte(tmp);
   pkArray[2] = (Data) GetLoByte(tmp);
   tmp = (uint16_t) GetLoWord(val);
   pkArray[1] = (Data) GetHiByte(tmp);
   pkArray[0] = (Data) GetLoByte(tmp);
#endif
   ret = SAddPreMsgMult(pkArray, (MsgLen) 4, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS247, (ErrVal)ret, "SAddPreMsgMult() failed");
#endif
   return (ret);
} /* end of oduUnpackUInt32 */

/*ss038.103  1. Added Floating point support*/
#ifdef SS_FLOAT
/*
*
*       Fun:   SPkF32
*
*       Desc:  This function packs an 32 bit ieee-754 format float value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SPkF32
(
F32 val,                    /* value */
Buffer *mBuf                /* message buffer */
)
{
   S16  ret;                /* return code */
   S8   tmpArray[15];       /* temporary array */
#ifdef FCSPKINT             /* backward compatibility, packing order */
   S32  tIdx = 14;          /* counters */
   S32  kIdx = 0;           /* counters */
   S8   pkArray[15];        /* packing array */
#endif /*FCSPKINT*/
  
   sprintf(tmpArray, "%.7e", val);

#ifndef FCSPKINT            /* backward compatibility, packing order */
   ret = SAddPreMsgMult((Data*)tmpArray, (MsgLen) 15, mBuf);
#else
   while(tIdx >= 0)
   {
      pkArray[kIdx] = tmpArray[tIdx];
      tIdx--; kIdx++;
   }
   ret = SAddPreMsgMult((Data*)pkArray, (MsgLen) 15, mBuf);
#endif


#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS248, (ErrVal)ret, "SAddPreMsgMult() failed");
#endif
   return (ret);
} /* end of SPkF32 */

/*
*
*       Fun:   SPkF64
*
*       Desc:  This function packs an 64 bit ieee-754 format float value into a message.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SPkF64
(
F64 val,                    /* value */
Buffer *mBuf                /* message buffer */
)
{

   S16  ret;                /* return code */
   S8   tmpArray[25];       /* temporary array */
#ifdef FCSPKINT             /* backward compatibility, packing order */
   S32  tIdx = 24;          /* counters */
   S32  kIdx = 0;           /* counters */
   S8   pkArray[25];        /* packing array */
#endif /*FCSPKINT*/
  
   sprintf(tmpArray, "%.16le", val);

#ifndef FCSPKINT            /* backward compatibility, packing order */
   ret = SAddPreMsgMult((Data*)tmpArray, (MsgLen) 25, mBuf);
#else
   while(tIdx >= 0)
   {
      pkArray[kIdx] = tmpArray[tIdx];
      tIdx--; kIdx++;
   }
   ret = SAddPreMsgMult((Data*)pkArray, (MsgLen) 25, mBuf);
#endif


#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_ADD_RES, ESS249, (ErrVal)ret, "SAddPreMsgMult() failed");
#endif
   return (ret);

} /* end of SPkF64 */
#endif /* SS_FLOAT */
  
/*
*
*       Fun:   SUnpkS8
*
*       Desc:  This function unpacks a signed 8 bit value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SUnpkS8
(
S8 *val,                    /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{
   Data tmp;                /* temporary value */
   S16 ret;                 /* return code */

/* ss021.103 - Addition of data pointer check */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS250, ERRZERO, "SUnpkS8 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsg(&tmp, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS251, (ErrVal)ret, "SRemPreMsg() failed");
#endif
   *val = (S8) tmp;
   return (ret);
} /* end of SUnpkS8 */
  
  
/*
*
*       Fun:   oduPackUInt8
*
*       Desc:  This function unpacks an unsigned 8 bit value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 oduPackUInt8
(
uint8_t *val,                    /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{
   Data tmp;                /* temporary value */
   S16 ret;                 /* return code */

/* ss021.103 - Addition of data pointer check */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS252, ERRZERO, "oduPackUInt8 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsg(&tmp, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS253, (ErrVal)ret, "SRemPreMsg() failed");
#endif
   *val = (uint8_t) tmp;
   return (ret);
} /* end of oduPackUInt8 */
  
/*
*
*       Fun:   SUnpkS16
*
*       Desc:  This function unpacks a signed 16 bit value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SUnpkS16
(
S16 *val,                   /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{
   uint16_t tmp16;               /* temporary value */
   Data unpkArray[2];       /* unpacking array */
   S16 ret;                 /* return code */

/* ss021.103 - Addition of data pointer check */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS254, ERRZERO, "SUnpkS16 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsgMult(unpkArray, (MsgLen) 2, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS255, (ErrVal)ret, "SRemPreMsgMult() failed");
#endif
   tmp16 = 0; 
#ifndef FCSPKINT            /* backward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[1]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[0]);
#else                       /* forward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[0]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[1]);
#endif
   *val = (S16) tmp16;
   return (ret);
} /* end of SUnpkS16 */
  
  
/*
*
*       Fun:   oduPackUInt16
*
*       Desc:  This function unpacks an unsigned 16 bit value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 oduPackUInt16
(
uint16_t *val,                   /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{
   uint16_t tmp16;               /* temporary value */
   Data unpkArray[2];       /* unpacking array */
   S16 ret;                 /* return code */

/* ss021.103 - Addition of data pointer check */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS256, ERRZERO, "oduPackUInt16 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsgMult(unpkArray, (MsgLen) 2, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS257, (ErrVal)ret, "SRemPreMsgMult failed");
#endif
   tmp16 = 0; 
#ifndef FCSPKINT            /* backward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[1]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[0]);
#else                       /* forward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[0]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[1]);
#endif
   *val = tmp16;
   return (ret);
} /* end of oduPackUInt16 */
  
  
/*
*
*       Fun:   SUnpkS32
*
*       Desc:  This function unpacks a signed 32 bit value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SUnpkS32
(
S32 *val,                   /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{
   uint16_t tmp16;               /* temporary value */
   uint32_t tmp32;               /* temporary value */
   Data unpkArray[4];       /* unpacking array */
   S16 ret;                 /* return code */

/* ss021.103 - Addition of data pointer check */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS258, ERRZERO, "SUnpkS32 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsgMult(unpkArray, (MsgLen) 4, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS259, (ErrVal)ret, "SRemPreMsgMult() failed");
#endif
   tmp16 = 0;
   tmp32 = 0; 
#ifndef FCSPKINT            /* backward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[3]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[2]);
   tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[1]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[0]);
   tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
#else                       /* forward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[0]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[1]);
   tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[2]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[3]);
   tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
#endif
   *val = (S32) tmp32;
   return (ret);
} /* end of SUnpkS32 */
  
  
/*
*
*       Fun:   oduPackUInt32
*
*       Desc:  This function unpacks an unsigned 32 bit value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 oduPackUInt32
(
uint32_t *val,                   /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{
   uint16_t tmp16;               /* temporary value */
   uint32_t tmp32;               /* temporary value */
   Data unpkArray[4];       /* unpacking array */
#ifdef ERRCLASS
   S16 ret;                 /* return code */
#endif /* ERRCLASS */

/* ss021.103 - Addition of data pointer check */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS260, ERRZERO, "oduPackUInt32 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsgMult(unpkArray, (MsgLen) 4, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS261, (ErrVal)ret, "SRemPreMsgMult() failed");
#endif

   tmp16 = 0;
   tmp32 = 0; 
#ifndef FCSPKINT            /* backward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[3]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[2]);
   tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[1]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[0]);
   tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
#else                       /* forward compatibility, packing order */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[0]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[1]);
   tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[2]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[3]);
   tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
#endif
   *val = tmp32;
   return (ret);
} /* end of oduPackUInt32 */

/*ss038.103  1. Added Floating point support*/
#ifdef SS_FLOAT
/*
*
*       Fun:   SUnpkF32
*
*       Desc:  This function unpacks an 32 bit ieee-754 format float value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SUnpkF32
(
F32 *val,                   /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{
   S16 ret;                 /* return code */
   S8 tmpArray[15];         /* temporary array */
#ifndef FCSPKINT            /* backward compatibility, packing order */
   S32 tIdx = 14;           /* temparory array index */
   S32 kIdx = 0;            /* unpacking array index */
   S8 unpkArray[15];        /* unpacking array */
#endif /* FCSPKINT */

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS262, ERRZERO, "SUnpkF32 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsgMult((Data*)tmpArray, (MsgLen) 15, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS263, (ErrVal)ret, "SRemPreMsgMult() failed");
#endif

#ifndef FCSPKINT            /* backward compatibility, packing order */
   while(tIdx >= 0)
   {
      unpkArray[kIdx] = tmpArray[tIdx];
      tIdx--; kIdx++;
   }
   sscanf(unpkArray, "%f", val);
#else
   sscanf(tmpArray, "%f", val);
#endif

   return (ret);

} /* end of SUnpkF32 */

/*
*
*       Fun:   SUnpkF64
*
*       Desc:  This function unpacks an 64 bit ieee-754 format float value from a message.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  ss_pack.c
*
*/
  
S16 SUnpkF64
(
F64 *val,                   /* pointer to value */
Buffer *mBuf                /* message buffer */
)
{

   S16 ret;                 /* return code */
   S8 tmpArray[25];         /* temporary array */
#ifndef FCSPKINT            /* backward compatibility, packing order */
   S32 tIdx = 24;           /* temparory array index */
   S32 kIdx = 0;            /* unpacking array index */
   S8 unpkArray[25];        /* unpacking array */
#endif /* FCSPKINT */

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check data pointer */
   if (!val)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS264, ERRZERO, "SUnpkF64 : Null value");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = SRemPreMsgMult((Data*)tmpArray, (MsgLen) 25, mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SSLOGERROR(ERRCLS_DEBUG, ESS265, (ErrVal)ret, "SRemPreMsgMult() failed");
#endif

#ifndef FCSPKINT            /* backward compatibility, packing order */
   while(tIdx >= 0)
   {
      unpkArray[kIdx] = tmpArray[tIdx];
      tIdx--; kIdx++;
   }
   sscanf(unpkArray, "%lf", val);
#else
   sscanf(tmpArray, "%lf", val);
#endif

   return (ret);

} /* end of SUnpkF64 */
#endif /* SS_FLOAT */

/**********************************************************************
         End of file
 **********************************************************************/
