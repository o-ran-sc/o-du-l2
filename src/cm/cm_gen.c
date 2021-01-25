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
  
     Name:     common body 6
  
     Type:     C source file
  
     Desc:     common functions used to pack and unpack primitives and 
               structures that are:

               - Common to all interfaces e.g. Addrs, etc.
                 These functions are always compiled.

               All functions pack/unpack assuming that the most significant 
               bit is towards the head of the buffer.

     File:     cm_gen.c
  
*********************************************************************21*/
  
   
/* header include files (.h) */
  
#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

#ifndef CMFILE_REORG_1
#include "cm_gen.h"        /* common pack/unpack functions */
#endif

#include "cm_err.h"        /* common error */

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */

#ifndef CMFILE_REORG_1
#include "cm_gen.x"        /* common pack/unpack functions */
#endif

  
/* local defines */
  
/* local typedefs */

/* local externs */
  
/* forward references */

/* functions in other modules */

/* public variable declarations */
uint16_t gTransId = 0;

/* private variable declarations */


/* 
 * common packing functions 
 */


/*
*
*       Fun:   cmPkDateTime
*
*       Desc:  This function packs the Date Time structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkDateTime
(
DateTime *dateTime,       /* date and time */
Buffer *mBuf              /* message buffer */
)
{

   /*-- cm_gen_c_001.main_36 - added for micro seconds --*/
#ifdef SS_DATETIME_USEC
   CMCHKPK(oduUnpackUInt32, dateTime->usec, mBuf);
#endif /*-- SS_DATETIME_USEC --*/
   CMCHKPK(oduUnpackUInt8, dateTime->tenths, mBuf);
   CMCHKPK(oduUnpackUInt8, dateTime->sec, mBuf); 
   CMCHKPK(oduUnpackUInt8, dateTime->min, mBuf); 
   CMCHKPK(oduUnpackUInt8, dateTime->hour, mBuf);
   CMCHKPK(oduUnpackUInt8, dateTime->year, mBuf);
   CMCHKPK(oduUnpackUInt8, dateTime->day, mBuf);
   CMCHKPK(oduUnpackUInt8, dateTime->month, mBuf);

   return ROK;
} /* end of cmPkDateTime */


/*
*
*       Fun:   cmPkDuration
*
*       Desc:  This function packs the Duration structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkDuration
(
Duration *duration,        /* duration */
Buffer   *mBuf             /* message buffer */
)
{

   CMCHKPK(oduUnpackUInt8, duration->tenths, mBuf);
   CMCHKPK(oduUnpackUInt8, duration->secs,   mBuf);
   CMCHKPK(oduUnpackUInt8, duration->mins,   mBuf);
   CMCHKPK(oduUnpackUInt8, duration->hours,  mBuf);
   CMCHKPK(oduUnpackUInt8, duration->days,   mBuf);

   return ROK;
} /* end of cmPkDuration */

/*
*
*       Fun:   oduPackPointer
*
*       Desc:  This function packs the pointer
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
 
S16 oduPackPointer
(
PTR      ptr,              /* pointer */
Buffer   *mBuf             /* message buffer */
)
{
   Data pkArray[PTRSIZE];   /* array for packing */
   S16 ret;                 /* return code */
   uint16_t tmp;                 /* temporary value */

#if (defined(ALPHA) || defined(BIT_64))
   uint32_t tmp32;
#endif

   
   ret = ROK;
   switch (PTRSIZE)
   {
      case 2:
#ifndef FCSPKINT            /* backward compatibility, packing order */
         pkArray[0] = (Data) GetHiByte(ptr);
         pkArray[1] = (Data) GetLoByte(ptr);
#else                       /* forward compatibility, packing order */
         pkArray[1] = (Data) GetHiByte(ptr);
         pkArray[0] = (Data) GetLoByte(ptr);
#endif
         ret = SAddPreMsgMult(pkArray, (MsgLen) 2, mBuf);
         break;
    
      case 4: 
#ifndef FCSPKINT        /* backward compatibility, packing order */
         tmp = (uint16_t) GetHiWord(ptr);
         pkArray[0] = (Data) GetHiByte(tmp);
         pkArray[1] = (Data) GetLoByte(tmp);
         tmp = (uint16_t) GetLoWord(ptr);
         pkArray[2] = (Data) GetHiByte(tmp);
         pkArray[3] = (Data) GetLoByte(tmp);
#else                   /* forward compatibility, packing order */
         tmp = (uint16_t) GetHiWord(ptr);
         pkArray[3] = (Data) GetHiByte(tmp);
         pkArray[2] = (Data) GetLoByte(tmp);
         tmp = (uint16_t) GetLoWord(ptr);
         pkArray[1] = (Data) GetHiByte(tmp);
         pkArray[0] = (Data) GetLoByte(tmp);
#endif
         ret = SAddPreMsgMult(pkArray, (MsgLen) 4, mBuf);
         break;

      case 8:
#if (defined(ALPHA) || defined(BIT_64))
#ifndef FCSPKINT        /* backward compatibility, packing order */
         tmp32 = (uint32_t) GetHi32Bit(ptr);
         tmp = (uint16_t) GetHiWord(tmp32);
         pkArray[0] = (Data) GetHiByte(tmp);
         pkArray[1] = (Data) GetLoByte(tmp);
         tmp = (uint16_t) GetLoWord(tmp32);
         pkArray[2] = (Data) GetHiByte(tmp);
         pkArray[3] = (Data) GetLoByte(tmp);
         tmp32 = (uint32_t) GetLo32Bit(ptr);
         tmp = (uint16_t) GetHiWord(tmp32);
         pkArray[4] = (Data) GetHiByte(tmp);
         pkArray[5] = (Data) GetLoByte(tmp);
         tmp = (uint16_t) GetLoWord(tmp32);
         pkArray[6] = (Data) GetHiByte(tmp);
         pkArray[7] = (Data) GetLoByte(tmp);
#else                   /* forward compatibility, packing order */
         tmp32 = (uint32_t) GetHi32Bit(ptr);
         tmp = (uint16_t) GetHiWord(tmp32);
         pkArray[7] = (Data) GetHiByte(tmp);
         pkArray[6] = (Data) GetLoByte(tmp);
         tmp = (uint16_t) GetLoWord(tmp32);
         pkArray[5] = (Data) GetHiByte(tmp);
         pkArray[4] = (Data) GetLoByte(tmp);
         tmp32 = (uint32_t) GetLo32Bit(ptr);
         tmp = (uint16_t) GetHiWord(tmp32);
         pkArray[3] = (Data) GetHiByte(tmp);
         pkArray[2] = (Data) GetLoByte(tmp);
         tmp = (uint16_t) GetLoWord(tmp32);
         pkArray[1] = (Data) GetHiByte(tmp);
         pkArray[0] = (Data) GetLoByte(tmp);
#endif
         ret = SAddPreMsgMult(pkArray, (MsgLen) 8, mBuf);
         break;
#endif
      default:
         /* no support for uint64_t */
         ret = RFAILED;
   }

   return (ret);
} /* end of oduPackPointer */


/*
*
*       Fun:   cmPkEntityId
*
*       Desc:  This function packs the EntityId structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkEntityId
(
EntityId *entityId,        /* entity id */
Buffer   *mBuf             /* message buffer */
)
{

   CMCHKPK(cmPkInst, entityId->inst, mBuf);
   CMCHKPK(cmPkEnt, entityId->ent, mBuf);

   return ROK;
} /* end of cmPkEntityId */


/*
*
*       Fun:   cmPkElmntId
*
*       Desc:  This function packs the ElmntId structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkElmntId
(
ElmntId *elmntId,         /* element id */
Buffer  *mBuf             /* message buffer */
)
{

   CMCHKPK(cmPkElmntInst3, elmntId->elmntInst3, mBuf); 
   CMCHKPK(cmPkElmntInst2, elmntId->elmntInst2, mBuf); 
   CMCHKPK(cmPkElmntInst1, elmntId->elmntInst1, mBuf); 
   CMCHKPK(cmPkElmnt, elmntId->elmnt, mBuf);

   return ROK;
} /* end of cmPkElmntId */


/*
*
*       Fun:   cmPkMemoryId
*
*       Desc:  This function packs the MemoryId structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkMemoryId
(
MemoryId *memoryId,        /* memoryId */
Buffer   *mBuf             /* message buffer */
)
{

   CMCHKPK(cmPkPool, memoryId->pool, mBuf); 
   CMCHKPK(cmPkRegion, memoryId->region, mBuf);

   return ROK;
} /* end of cmPkMemoryId */


/*
*
*       Fun:   cmPkSystemId
*
*       Desc:  This function packs the System Id structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkSystemId
(
SystemId *systemId,         /* system id */
Buffer   *mBuf              /* message buffer */
)
{
   Txt *p;                  /* part number string */


   for (p = systemId->ptNmb; *p; p++);

   for (; p != systemId->ptNmb; p--)
   {
      CMCHKPK(cmPkTxt, *p, mBuf);
   }
   CMCHKPK(cmPkTxt, *p, mBuf);
   
   CMCHKPK(SPkS16, systemId->bRev, mBuf);
   CMCHKPK(SPkS16, systemId->bVer, mBuf);
   CMCHKPK(SPkS16, systemId->mRev, mBuf);
   CMCHKPK(SPkS16, systemId->mVer, mBuf);

   return ROK;
} /* end of cmPkSystemId */



/*
 *
 *       Fun:   cmPkProtAddr
 *
 *       Desc:  This function will pack protocol address.
 *
 *       Ret:   ROK on success
 *              RFAILED on error
 *
 *       Notes: None
 *
         File:  cm_gen.c
 *
 */

S16 cmPkProtAddr
(
ProtAddr     *pAddr,     /* protocol address */
Buffer       *mBuf       /* buffer */
)
{
   uint8_t              j;                  /* Index */


#ifdef CM_ARI2
   CMCHKPK(oduUnpackUInt32, pAddr->autoSysId, mBuf);  
   CMCHKPK(oduPackBool, pAddr->autoSysIdPres, mBuf);  
#endif /* CM_ARI2 */

   for (j = pAddr->len; j; j--)
      CMCHKPK(oduUnpackUInt8, pAddr->address[j - 1], mBuf);  

   CMCHKPK(oduUnpackUInt8,  pAddr->preLen, mBuf);  
   CMCHKPK(oduUnpackUInt8,  pAddr->len, mBuf);  
   CMCHKPK(oduUnpackUInt16, pAddr->protType, mBuf);  
       
   return ROK;

} /* end of cmPkProtAddr */
   


/*
 *
 *       Fun:   cmPkProtAddrTbl
 *
 *       Desc:  This function will pack protocol addresses.
 *
 *       Ret:   ROK on success
 *              RFAILED on error
 *
 *       Notes: None
 *
         File:  cm_gen.c
 *
 */

S16 cmPkProtAddrTbl
(
ProtAddrTbl  *protAddr,      /* protocol address table */
Buffer       *mBuf          /* buffer */
)
{
   uint8_t              i;                  /* index */
   uint8_t              j;                  /* Index */
   ProtAddr        *pAddr;             /* protocol Address */


   if (protAddr->count > MAX_PROT_ADDRS)
      return RFAILED;

   for (i = protAddr->count; i; i--)
   {
       pAddr = &(protAddr->addr[i - 1]);

#ifdef CM_ARI2
       CMCHKPK(oduUnpackUInt32, pAddr->autoSysId, mBuf);  
       CMCHKPK(oduPackBool, pAddr->autoSysIdPres, mBuf);  
#endif /* CM_ARI2 */

       for (j = pAddr->len; j; j--)
          CMCHKPK(oduUnpackUInt8, pAddr->address[j - 1], mBuf);  
        
       CMCHKPK(oduUnpackUInt8, pAddr->preLen, mBuf);  
       CMCHKPK(oduUnpackUInt8,  pAddr->len, mBuf);  
       CMCHKPK(oduUnpackUInt16,  pAddr->protType, mBuf);  
       
   }
   CMCHKPK(oduUnpackUInt8, protAddr->count, mBuf);  

   return ROK;
} /* end of cmPkProtAddrTbl */
   

/*
*
*       Fun:   cmPkAddrs
*
*       Desc:  This function packs the address structure for a loosely 
*              coupled interface
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkAddrs
(
Addrs *addrs,           /* address */
Buffer *mBuf            /* message buffer */
)
{
   uint8_t i;                /* loop counter */


   if (addrs->length > ADRLEN)
      return RFAILED;

   for (i = addrs->length; i; i--)
   {
      CMCHKPK(oduUnpackUInt8, addrs->strg[i-1], mBuf);
   }

   CMCHKPK(oduUnpackUInt8, addrs->length, mBuf);

   return ROK;
} /* end of cmPkAddrs */


/*
*
*       Fun:   cmPkShrtAddrs
*
*       Desc:  This function packs the short address structure for a loosely 
*              coupled interface
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkShrtAddrs
(
ShrtAddrs *addrs,          /* address */
Buffer    *mBuf            /* message buffer */
)
{
   uint8_t i;                /* loop counter */


   if (addrs->length > SHRTADRLEN)
      return RFAILED;

   for (i = addrs->length; i; i--)
   {
      CMCHKPK(oduUnpackUInt8, addrs->strg[i-1], mBuf);
   }

   CMCHKPK(oduUnpackUInt8, addrs->length, mBuf);

   return ROK;
} /* end of cmPkShrtAddrs */


/*
*
*       Fun:   cmPkAddrMask
*
*       Desc:  This function address mask array.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkAddrMask
(
uint8_t *mask,             /* pointer to address mask array */
Buffer  *mBuf         /* message buffer */
)
{
   S16 i;             /* counter */


   /* pack address mask */
   for (i = (ADRLEN - 1); i >= 0; i--)
   {
      CMCHKPK(oduUnpackUInt8, mask[i], mBuf);
   }

   return ROK;
} /* end of cmPkAddrMask */


/*
*
*       Fun:   cmPkBndCfg
*
*       Desc:  This function packs the BndCfg structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkBndCfg
(
BndCfg *bndCfg,         /* bndCfg */
Buffer *mBuf            /* message buffer */
)
{
   Txt *p;              /* temporary */


   CMCHKPK(cmPkSelector, bndCfg->selector, mBuf);
   CMCHKPK(cmPkAddrs, &bndCfg->sapAdr, mBuf);
   CMCHKPK(cmPkRoute, bndCfg->route, mBuf);
   CMCHKPK(cmPkPrior, bndCfg->prior, mBuf);
   CMCHKPK(cmPkPool, bndCfg->pool, mBuf); 
   CMCHKPK(cmPkRegion, bndCfg->region, mBuf);
   CMCHKPK(cmPkInst, bndCfg->inst, mBuf);
   CMCHKPK(cmPkEnt, bndCfg->ent, mBuf);
   CMCHKPK(oduUnpackUInt8, bndCfg->wdw, mBuf);
   CMCHKPK(oduUnpackUInt8, bndCfg->flcTyp, mBuf);
   CMCHKPK(oduUnpackUInt8, bndCfg->bufOwnshp, mBuf);

   for (p = bndCfg->usrId; *p; p++);
   for (; p != bndCfg->usrId; p--);
   CMCHKPK(cmPkTxt, *p, mBuf);

   return ROK;
} /* end of cmPkBndCfg */


/*
*
*       Fun:   cmPkPst
*
*       Desc:  pack post structure 
*
*       Ret:   ROK
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
S16 cmPkPst
(
Pst *pst,
Buffer *mBuf
)
{
   
   CMCHKPK(cmPkEvent, pst->event, mBuf);
   CMCHKPK(cmPkInst, pst->srcInst, mBuf);
   CMCHKPK(cmPkEnt, pst->srcEnt, mBuf); 
   CMCHKPK(cmPkProcId, pst->srcProcId, mBuf);
   CMCHKPK(cmPkInst, pst->dstInst, mBuf); 
   CMCHKPK(cmPkEnt, pst->dstEnt, mBuf); 
   CMCHKPK(cmPkProcId, pst->dstProcId, mBuf);
   CMCHKPK(cmPkRoute, pst->route, mBuf);
   CMCHKPK(cmPkPrior, pst->prior, mBuf);
   CMCHKPK(cmPkPool, pst->pool, mBuf);
   CMCHKPK(cmPkRegion, pst->region, mBuf);
   CMCHKPK(cmPkSelector, pst->selector, mBuf);
#ifdef TDS_ROLL_UPGRADE_SUPPORT
   CMCHKPK(cmPkIntfVer, pst->intfVer, mBuf);
#endif

   return ROK;
} /* end of cmPkPst */

/*
 *
 *      Fun:    cmPkElmtHdr
 *
 *      Desc:   Pack element header
 *
 *      Ret:   ROK      - ok
 *
 *      Notes:  None
 *
        File:   cm_gen.c
 *
 */
  
S16 cmPkElmtHdr
(
ElmtHdr *m,                  /* element header */
Buffer  *mBuf                /* message buffer */
)
{
 
#if (LCAMT || ATM_BISUP)
      CMCHKPK(oduUnpackUInt16, m->compInd, mBuf);
#endif /* LCAMT || ATM_BISUP */

#if (LCAMT || ATM_BISUP)
   /* Pack action indicator field */
   if (m->pres)
   {
     CMCHKPK(oduUnpackUInt8, m->actnInd, mBuf);
   }
#endif

   /* Present */
   CMCHKPK(oduPackBool, m->pres, mBuf);

   return ROK;
} /* end of cmPkElmtHdr */


/*
 *
 *      Fun:   cmPkTknUInt8
 *
 *      Desc:  This function packs a token uint8_t
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknUInt8
(
TknUInt8  *tknUInt8,              /* token uint8_t */
Buffer *mBuf                /* message buffer */
)
{

   if (tknUInt8->pres)
   {
      /* Value */
      CMCHKPK(oduUnpackUInt8, tknUInt8->val, mBuf);
   }

   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknUInt8->pres, mBuf);

   return ROK;
} /* end of cmPkTknUInt8 */


/*
 *
 *      Fun:   cmPkTknS8
 *
 *      Desc:  This function packs a token S8
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknS8
(
TknS8  *tknS8,              /* token S8 */
Buffer *mBuf                /* message buffer */
)
{
 
   if (tknS8->pres)
   {
      /* Value */
      CMCHKPK(SPkS8, tknS8->val, mBuf);
   }
 
   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknS8->pres, mBuf);
 
   return ROK;
} /* end of cmPkTknS8 */


/*
 *
 *      Fun:   cmPkTknUInt16
 *
 *      Desc:  This function packs a token uint16_t
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknUInt16
(
TknUInt16 *tknUInt16,             /* token uint16_t */
Buffer *mBuf                /* message buffer */
)
{

   if (tknUInt16->pres)
   {
      /* Value */
      CMCHKPK(oduUnpackUInt16, tknUInt16->val, mBuf);
   }

   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknUInt16->pres, mBuf);

   return ROK;
} /* end of cmPkTknUInt16 */


/*
 *
 *      Fun:   cmPkTknUInt32
 *
 *      Desc:  This function packs a token uint32_t
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknUInt32
(
TknUInt32 *tknUInt32,             /* token uint32_t */
Buffer *mBuf                /* message buffer */
)
{

   if (tknUInt32->pres)
   {
      /* Value */
      CMCHKPK(oduUnpackUInt32, tknUInt32->val, mBuf);
   }

   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknUInt32->pres, mBuf);

   return ROK;
} /* end of cmPkTknUInt32 */


/*
 *
 *      Fun:   cmPkTknStr
 *
 *      Desc:  This function packs a token string - regular size
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStr
(
TknStr *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                    /* counter */


   if (tknStr->pres)
   {
      /* Value */
      for (i = 0; i < (S16) tknStr->len; i++)
      {
         CMCHKPK(oduUnpackUInt8, tknStr->val[i], mBuf);
      }

      /* Length */
      CMCHKPK(oduUnpackUInt8, tknStr->len, mBuf);
   }
 
   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknStr->pres, mBuf);

   return ROK;
} /* end of cmPkTknStr */


/*
 *
 *      Fun:   cmPkTknStrM
 *
 *      Desc:  This function packs a token string - medium size
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStrM
(
TknStrM *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                    /* counter */


   if(tknStr->pres)
   {
      /* Value */
      for (i = 0; i < (S16) tknStr->len; i++)
      {
         CMCHKPK(oduUnpackUInt8, tknStr->val[i], mBuf);
      }

      /* Length */
      CMCHKPK(oduUnpackUInt8, tknStr->len, mBuf);
   }
 
   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknStr->pres, mBuf);

   return ROK;
} /* end of cmPkTknStrM */


/*
 *
 *      Fun:   cmPkTknStrS
 *
 *      Desc:  This function packs a token string - small size
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStrS
(
TknStrS *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                    /* counter */


   if(tknStr->pres)
   {
      /* Value */
      for (i = 0; i < (S16) tknStr->len; i++)
      {
         CMCHKPK(oduUnpackUInt8, tknStr->val[i], mBuf);
      }

      /* Length */
      CMCHKPK(oduUnpackUInt8, tknStr->len, mBuf);
   }
 
   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknStr->pres, mBuf);

   return ROK;
} /* end of cmPkTknStrS */


/*
 *
 *      Fun:   cmPkTknStrE
 *
 *      Desc:  This function packs a token string - extended size
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStrE
(
TknStrE *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                    /* counter */


   if(tknStr->pres)
   {
      /* Value */
      for (i = 0; i < (S16) tknStr->len; i++)
      {
         CMCHKPK(oduUnpackUInt8, tknStr->val[i], mBuf);
      }

      /* Length */
      CMCHKPK(oduUnpackUInt8, tknStr->len, mBuf);
   }
 
   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknStr->pres, mBuf);

   return ROK;
} /* end of cmPkTknStrE */

#ifndef CMFILE_REORG_1


/*
*
*       Fun:   cmPkPnNodeId
*
*       Desc:  This function packs a PnNodeId structure into a buffer
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkPnNodeId
(
PnNodeId  *src,     /* source PNNI Node Id */
Buffer *mBuf        /* message buffer */
)
{
   S16 i;
   
   
   for (i = PN_NODEID_LEN - 1; i >= 0; i--)
   {
      CMCHKPK(oduUnpackUInt8, src->id[i], mBuf);
   }
   
   return ROK;
} /* cmPkPnNodeId */

#endif /* CMFILE_REORG_1 */


/*
 *
 *      Fun:   cmPkTknStr4
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStr4
(
TknStr4 *tknStr,             /* token string */
Buffer  *mBuf                /* message buffer */
)
{

   CMPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmPkTknStr4 */



/*
 *
 *      Fun:   cmPkTknStr12
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStr12
(
TknStr12 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmPkTknStr12 */


/*
 *
 *      Fun:   cmPkTknStr32
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStr32
(
TknStr32 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmPkTknStr32 */


/*
 *
 *      Fun:   cmPkTknStr64
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStr64
(
TknStr64 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmPkTknStr64 */


/*
 *
 *      Fun:   cmPkTknStr132
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStr132
(
TknStr132 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmPkTknStr132 */


/*
 *
 *      Fun:   cmPkTknStr256
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknStr256
(
TknStr256 *tknStr,             /* token string */
Buffer    *mBuf                /* message buffer */
)
{

   CMPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmPkTknStr256 */


/*
 *
 *      Fun:   cmPkTknOid
 *
 *      Desc:  This function packs a Object Identifier token
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknOid
(
TknOid   *tknOid,             /* Object Identifier token */
Buffer   *mBuf                /* message buffer */
)
{
   uint16_t    i;

 
   if (tknOid->pres == TRUE)
   {
      /* Pack the value */
      for (i = 0; i < (uint16_t)tknOid->len; i++)
      {
         /* cm_gen_c_001.main_33: changes for TknOid value from uint16_t to uint32_t
          * with compilation flag TKNOID_UINT16 */
#ifndef TKNOID_UINT16
         CMCHKPK(oduUnpackUInt32, tknOid->val[i], mBuf);
#else
         CMCHKPK(oduUnpackUInt16, tknOid->val[i], mBuf);
#endif  /* !TKNOID_UINT16 */
      }
      /* Pack the length */
      CMCHKPK(oduUnpackUInt8, tknOid->len, mBuf);
   }
   /* Pack the token header */
   CMCHKPK(oduUnpackUInt8, tknOid->pres, mBuf);

   return ROK;
} /* end of cmPkTknOid */


/*
*
*      Fun:   cmPkTknS32
*
*      Desc:  This function packs a token S32
*
*      Ret:   ROK      - ok
*
*      Notes: None
*
*      File:  cm_gen.c
*
*/
  
S16 cmPkTknS32
(
TknS32 *tknS32,             /* token S32 */
Buffer *mBuf                /* message buffer */
)
{

   if (tknS32->pres)
   {
      /* Value */
      CMCHKPK(SPkS32, tknS32->val, mBuf);
   }

   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknS32->pres, mBuf);

   return ROK;
} /* end of cmPkTknS32 */


/*
*
*       Fun:   cmPkHeader
*
*       Desc:  This function packs the header structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkHeader
(
Header *header,             /* header */
Buffer *mBuf                /* message buffer */
)
{

#ifdef LMINT3
   CMCHKPK(cmPkMemoryId, &header->response.mem, mBuf);
   CMCHKPK(cmPkRoute, header->response.route, mBuf);
   CMCHKPK(cmPkPriority, header->response.prior, mBuf);
   CMCHKPK(cmPkSelector, header->response.selector, mBuf);
   CMCHKPK(cmPkTranId, header->transId, mBuf);
#endif /* LMINT3 */
   CMCHKPK(cmPkElmntId, &header->elmId, mBuf);
   CMCHKPK(cmPkEntityId, &header->entId, mBuf);
   CMCHKPK(oduUnpackUInt16, header->seqNmb, mBuf);
   CMCHKPK(oduUnpackUInt8, header->version, mBuf);
   CMCHKPK(oduUnpackUInt8, header->msgType, mBuf);
   CMCHKPK(oduUnpackUInt16, header->msgLen, mBuf);

   return ROK;
} /* end of cmPkHeader */


/*
*
*       Fun:   cmPkCmStatus
*
*       Desc:  This function packs common management status structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkCmStatus
(
CmStatus *sta,              /* status structure */
Buffer *mBuf                /* message buffer */
)
{

   CMCHKPK(oduUnpackUInt16, sta->reason, mBuf);
   CMCHKPK(oduUnpackUInt16, sta->status, mBuf);

   return ROK;
} /* end of cmPkCmStatus */


/*
*
*       Fun:   cmPkCmAlarm
*
*       Desc:  This function packs common management alarm structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkCmAlarm
(
CmAlarm *alarm,            /* alarm structure */
Buffer  *mBuf              /* message buffer */
)
{

   CMCHKPK(oduUnpackUInt16, alarm->cause, mBuf);
   CMCHKPK(oduUnpackUInt16, alarm->event, mBuf);
   CMCHKPK(oduUnpackUInt16, alarm->category, mBuf);
   CMCHKPK(cmPkDateTime, &alarm->dt, mBuf);

   return ROK;
} /* end of cmPkCmAlarm */


/*
*
*       Fun:   cmPkSmCfg
*
*       Desc:  This function packs the stack manager structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkSmCfg
(
SmCfg *smCfg,           /* stack manager */
Buffer *mBuf            /* message buffer */
)
{

   CMCHKPK(cmPkSelector, smCfg->selector, mBuf); 
   CMCHKPK(cmPkRoute, smCfg->route, mBuf); 
   CMCHKPK(cmPkPrior, smCfg->prior, mBuf); 
   CMCHKPK(cmPkPool, smCfg->pool, mBuf); 
   CMCHKPK(cmPkRegion, smCfg->region, mBuf);
   CMCHKPK(cmPkInst, smCfg->inst, mBuf);
   CMCHKPK(cmPkEnt, smCfg->ent, mBuf);

   return ROK;
} /* end of cmPkSmCfg */


/*
*
*       Fun:   cmPkTmrCfg
*
*       Desc:  This function packs the timer configuration structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkTmrCfg
(
TmrCfg *tmrCfg,         /* timer configuration */
Buffer *mBuf            /* message buffer */
)
{

   CMCHKPK(oduUnpackUInt16, tmrCfg->val, mBuf);
   CMCHKPK(oduPackBool, tmrCfg->enb, mBuf);

   return ROK;
} /* end of cmPkTmrCfg */


/*
 *
 *      Fun:   cmPkTknBuf
 *
 *      Desc:  This function packs a token buffer
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmPkTknBuf
(
TknBuf *tknBuf,                /* token string */
Buffer    *mBuf                /* message buffer */
)
{

   if(tknBuf->pres)
   {
      MsgLen msgLen;

      SFndLenMsg(tknBuf->val, &msgLen);
      if( SCatMsg(mBuf, tknBuf->val, M2M1) != ROK )
      {
         return RFAILED;
      }
      cmPkMsgLen(msgLen, mBuf);
      SPutMsg(tknBuf->val);
   }
  
   /* Token Header */
   CMCHKPK(oduUnpackUInt8, tknBuf->pres, mBuf);

   return ROK;
} /* end of cmPkTknBuf */

#ifdef TDS_ROLL_UPGRADE_SUPPORT
/*
*
*       Fun:   cmPkIntf
*
*       Desc:  This function packs the interface information
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmPkIntf
(
CmIntf *intf,           /* interface info */
Buffer *mBuf            /* message buffer */
)
{

   CMCHKPK(cmPkIntfId,  intf->intfId,  mBuf); 
   CMCHKPK(cmPkIntfVer, intf->intfVer, mBuf); 

   return ROK;
} /* end of cmPkIntf */
#endif /* TDS_ROLL_UPGRADE_SUPPORT */


/*
 *   common unpacking functions 
 */


/*
*
*       Fun:   cmUnpkDateTime
*
*       Desc:  This function is used to Unpack Pack Date Time structure
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkDateTime
(
DateTime *dateTime,  /* date/time structure */
Buffer *mBuf         /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt8, &dateTime->month, mBuf); 
   CMCHKUNPK(oduPackUInt8, &dateTime->day, mBuf); 
   CMCHKUNPK(oduPackUInt8, &dateTime->year, mBuf); 
   CMCHKUNPK(oduPackUInt8, &dateTime->hour, mBuf); 
   CMCHKUNPK(oduPackUInt8, &dateTime->min, mBuf); 
   CMCHKUNPK(oduPackUInt8, &dateTime->sec, mBuf); 
   CMCHKUNPK(oduPackUInt8, &dateTime->tenths, mBuf);

   /*-- added for micro seconds --*/
#ifdef SS_DATETIME_USEC
   CMCHKUNPK(oduPackUInt32, &dateTime->usec, mBuf);
#endif /*-- SS_DATETIME_USEC --*/

   return ROK;
} /* end of cmUnpkDateTime */


/*
*
*       Fun:   cmUnpkDuration
*
*       Desc:  This function is used to Unpack Pack Duration structure
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkDuration
(
Duration *duration,  /* duration structure */
Buffer *mBuf         /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt8, &duration->days, mBuf); 
   CMCHKUNPK(oduPackUInt8, &duration->hours, mBuf); 
   CMCHKUNPK(oduPackUInt8, &duration->mins, mBuf); 
   CMCHKUNPK(oduPackUInt8, &duration->secs, mBuf); 
   CMCHKUNPK(oduPackUInt8, &duration->tenths, mBuf);

   return ROK;
} /* end of cmUnpkDuration */

/*
*
*       Fun:   oduUnpackPointer
*
*       Desc:  This function is used to Unpack Ptr type
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 oduUnpackPointer
(
PTR *ptr,  /* duration structure */
Buffer *mBuf    /* message buffer */
)
{
   uint16_t tmp16 =0;               /* temporary value */
   uint32_t tmp32 =0;               /* temporary value */
   Data unpkArray[PTRSIZE];        /* unpacking array */
   S16 ret =ROK;                 /* return code */

#if (defined(ALPHA) || defined(BIT_64))
   uint64_t tmp64 =0;
#endif
   

   switch (PTRSIZE)
   {
      case 2:
         ret = SRemPreMsgMult(unpkArray, (MsgLen) 2, mBuf);
         if (ret != ROK)
            return (ret);

         tmp16 = 0; 
#ifndef FCSPKINT            /* backward compatibility, packing order */
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[1]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[0]);
#else                       /* forward compatibility, packing order */
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[0]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[1]);
#endif
         *ptr = tmp16;
         break;

      case 4:
         ret = SRemPreMsgMult(unpkArray, (MsgLen) 4, mBuf);
         if (ret != ROK)
            return (ret);

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
         *ptr = tmp32;
         break;

      case 8:
#if(defined(ALPHA) || defined(BIT_64))
         ret = SRemPreMsgMult(unpkArray, (MsgLen) 8, mBuf);
         if (ret != ROK)
            return (ret);

         tmp16 = 0;
         tmp32 = 0; 
         tmp64 = 0;
#ifndef FCSPKINT            /* backward compatibility, packing order */
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[7]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[6]);
         tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[5]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[4]);
         tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
         tmp64 = (uint64_t) PutHi32Bit(tmp64, tmp32);
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[3]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[2]);
         tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[1]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[0]);
         tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
         tmp64 = (uint64_t) PutLo32Bit(tmp64, tmp32);
#else                       /* forward compatibility, packing order */
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[0]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[1]);
         tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[2]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[3]);
         tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
         tmp64 = (uint64_t) PutHi32Bit(tmp64, tmp32);
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[4]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[5]);
         tmp32 = (uint32_t) PutHiWord(tmp32, (uint16_t) tmp16);
         tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) unpkArray[6]);
         tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) unpkArray[7]);
         tmp32 = (uint32_t) PutLoWord(tmp32, (uint16_t) tmp16);
         tmp64 = (uint64_t) PutLo32Bit(tmp64, tmp32);
#endif
         *ptr = tmp64;
         break;
#endif
      default:
         /* no support for uint64_t */
         ret = RFAILED;
   }

   return (ret);
} /* end of oduUnpackPointer */


/*
*
*       Fun:   cmUnpkEntityId
*
*       Desc:  This function unpacks the EntityId structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkEntityId
(
EntityId *entityId,        /* entity id */
Buffer   *mBuf             /* message buffer */
)
{

   CMCHKUNPK(cmUnpkEnt, &entityId->ent, mBuf); 
   CMCHKUNPK(cmUnpkInst, &entityId->inst, mBuf);

   return ROK;
} /* end of cmUnpkEntityId */


/*
*
*       Fun:   cmUnpkElmntId
*
*       Desc:  This function unpacks the ElmntId structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkElmntId
(
ElmntId *elmntId,         /* element id */
Buffer  *mBuf             /* message buffer */
)
{

   CMCHKUNPK(cmUnpkElmnt, &elmntId->elmnt, mBuf); 
   CMCHKUNPK(cmUnpkElmntInst1, &elmntId->elmntInst1, mBuf); 
   CMCHKUNPK(cmUnpkElmntInst2, &elmntId->elmntInst2, mBuf); 
   CMCHKUNPK(cmUnpkElmntInst3, &elmntId->elmntInst3, mBuf);

   return ROK;
} /* end of cmUnpkElmntId */


/*
*
*       Fun:   cmUnpkMemoryId
*
*       Desc:  This function unpacks the MemoryId structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkMemoryId
(
MemoryId *memoryId,        /* memoryId */
Buffer   *mBuf             /* message buffer */
)
{

   CMCHKUNPK(cmUnpkRegion, &memoryId->region,   mBuf); 
   CMCHKUNPK(cmUnpkPool, &memoryId->pool, mBuf);

   return ROK;
} /* end of cmUnpkMemoryId */


/*
*
*       Fun:   cmUnpkSystemId
*
*       Desc:  This function packs the System Id structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkSystemId
(
SystemId *systemId,         /* system id */
Buffer   *mBuf              /* message buffer */
)
{
   S16 i;               /* loop counter */
   MsgLen len;
   

   CMCHKUNPK(SUnpkS16, &systemId->mVer, mBuf); 
   CMCHKUNPK(SUnpkS16, &systemId->mRev, mBuf); 
   CMCHKUNPK(SUnpkS16, &systemId->bVer, mBuf); 
   CMCHKUNPK(SUnpkS16, &systemId->bRev, mBuf);

   SFndLenMsg(mBuf, &len);
   
   for (i = 0; i < len; i++)
   {
     CMCHKUNPK(cmUnpkTxt, &systemId->ptNmb[i], mBuf);
   }
   
   return ROK;
} /* end of cmUnpkSystemId */



/*
 *
 *       Fun:   cmUnpkProtAddr   
 *
 *       Desc:  This function will unpack protocol address.
 *
 *       Ret:   ROK on success
 *              RFAILED on error
 *
 *       Notes: None
 *
         File:  cm_gen.c
 *
 */

S16 cmUnpkProtAddr
(
ProtAddr     *pAddr,         /* protocol address */
Buffer       *mBuf           /* buffer */
)
{
   uint8_t               j;                  /* Index */


   CMCHKUNPK(oduPackUInt16,  &(pAddr->protType), mBuf);  
   CMCHKUNPK(oduPackUInt8,   &(pAddr->len), mBuf);  
   CMCHKUNPK(oduPackUInt8,   &(pAddr->preLen), mBuf);  

   for (j =0; j <  pAddr->len; j++)
      CMCHKUNPK(oduPackUInt8, &pAddr->address[j], mBuf);  
   
#ifdef CM_ARI2
   CMCHKUNPK(oduUnpackBool,   &(pAddr->autoSysIdPres), mBuf);  
   CMCHKUNPK(oduPackUInt32,  &(pAddr->autoSysId), mBuf);  
#endif /* CM_ARI2 */
   return ROK;

} /* end of cmUnpkProtAddr */
   

/*
 *
 *       Fun:   cmUnpkProtAddrTbl
 *
 *       Desc:  This function will pack protocol addresses.
 *
 *       Ret:   ROK on success
 *              RFAILED on error
 *
 *       Notes: None
 *
         File:  cm_gen.c
 *
 */

S16 cmUnpkProtAddrTbl
(
ProtAddrTbl  *protAddr,      /* protocol address table */
Buffer       *mBuf           /* buffer */
)
{
   uint8_t               i;                  /* index */
   uint8_t               j;                  /* Index */
   ProtAddr         *pAddr;             /* Protocol Address */


   CMCHKUNPK(oduPackUInt8, &protAddr->count, mBuf);  
   for (i = 0;  i < protAddr->count; i++)
   {
       pAddr = &(protAddr->addr[i]);

       CMCHKUNPK(oduPackUInt16,  &(pAddr->protType), mBuf);  
       CMCHKUNPK(oduPackUInt8,  &(pAddr->len), mBuf);  
       CMCHKUNPK(oduPackUInt8, &(pAddr->preLen), mBuf);  

        
       for (j =0; j <  pAddr->len; j++)
          CMCHKUNPK(oduPackUInt8, &pAddr->address[j], mBuf);  

#ifdef CM_ARI2
       CMCHKUNPK(oduUnpackBool,   &(pAddr->autoSysIdPres), mBuf);
       CMCHKUNPK(oduPackUInt32,  &(pAddr->autoSysId), mBuf);
#endif /* CM_ARI2 */
   }
   return ROK;
} /* end of cmUnpkProtAddrTbl */
   

/*
*
*       Fun:   cmUnpkAddrs
*
*       Desc:  This function is used to unpack the Addrs structure
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkAddrs
(
Addrs *addrs,     /* address */
Buffer *mBuf      /* message buffer */
)
{
   uint8_t i;          /* loop counter */


   CMCHKUNPK(oduPackUInt8, &addrs->length, mBuf);

   if (addrs->length > ADRLEN)
      return RFAILED;
   
   for(i = 0; i < addrs->length; i++)
   {
      CMCHKUNPK(oduPackUInt8, &addrs->strg[i], mBuf);
   }

   return ROK;
} /* end of cmUnpkAddrs */

/*
*
*       Fun:   cmUnpkShrtAddrs
*
*       Desc:  This function is used to unpack the ShrtAddrs structure
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkShrtAddrs
(
ShrtAddrs *addrs,    /* address */
Buffer    *mBuf      /* message buffer */
)
{
   uint8_t i;          /* loop counter */


   CMCHKUNPK(oduPackUInt8, &addrs->length, mBuf);

   if (addrs->length > SHRTADRLEN)
      return RFAILED;
   
   for(i = 0; i < addrs->length; i++)
   {
      CMCHKUNPK(oduPackUInt8, &addrs->strg[i], mBuf);
   }
   return ROK;
} /* end of cmUnpkShrtAddrs */


/*
*
*       Fun:   cmUnpkAddrMask
*
*       Desc:  This unpacks address mask.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkAddrMask
(
uint8_t *mask,             /* pointer to address mask */
Buffer  *mBuf         /* message buffer */
)
{
   S16 i;             /* counter */


   /* unpack address mask */
   for (i = 0; i< ADRLEN; i++) 
   {
      CMCHKUNPK(oduPackUInt8, &mask[i], mBuf);
   }
   return ROK;
} /* end of cmUnpkAddrMask */


/*
*
*       Fun:   cmUnpkBndCfg
*
*       Desc:  This function unpacks the BndCfg structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkBndCfg
(
BndCfg *bndCfg,         /* bndCfg */
Buffer *mBuf            /* message buffer */
)
{
   S16 i;               /* loop counter */


   for (i = 0; bndCfg->usrId[i]; i++);
   CMCHKUNPK(cmUnpkTxt, bndCfg->usrId+i, mBuf);

   CMCHKUNPK(oduPackUInt8, &bndCfg->bufOwnshp, mBuf); 
   CMCHKUNPK(oduPackUInt8, &bndCfg->flcTyp, mBuf); 
   CMCHKUNPK(oduPackUInt8, &bndCfg->wdw, mBuf); 
   CMCHKUNPK(cmUnpkEnt, &bndCfg->ent, mBuf); 
   CMCHKUNPK(cmUnpkInst, &bndCfg->inst, mBuf); 
   CMCHKUNPK(cmUnpkRegion, &bndCfg->region, mBuf); 
   CMCHKUNPK(cmUnpkPool, &bndCfg->pool, mBuf); 
   CMCHKUNPK(cmUnpkPrior, &bndCfg->prior, mBuf); 
   CMCHKUNPK(cmUnpkRoute, &bndCfg->route, mBuf); 
   CMCHKUNPK(cmUnpkAddrs, &bndCfg->sapAdr, mBuf); 
   CMCHKUNPK(cmUnpkSelector, &bndCfg->selector, mBuf);

   return ROK;
} /* end of cmUnpkBndCfg */


/*
*
*       Fun:   cmUnpkPst
*
*       Desc:  pack post structure 
*
*       Ret:   ROK
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
S16 cmUnpkPst
(
Pst *pst,
Buffer *mBuf
)
{

#ifdef TDS_ROLL_UPGRADE_SUPPORT
   CMCHKUNPK(cmUnpkIntfVer, &pst->intfVer, mBuf);
#endif
   CMCHKUNPK(cmUnpkSelector, &pst->selector, mBuf); 
   CMCHKUNPK(cmUnpkRegion, &pst->region, mBuf); 
   CMCHKUNPK(cmUnpkPool, &pst->pool, mBuf); 
   CMCHKUNPK(cmUnpkPrior, &pst->prior, mBuf); 
   CMCHKUNPK(cmUnpkRoute, &pst->route, mBuf); 
   CMCHKUNPK(cmUnpkProcId, &pst->dstProcId, mBuf); 
   CMCHKUNPK(cmUnpkEnt, &pst->dstEnt, mBuf); 
   CMCHKUNPK(cmUnpkInst, &pst->dstInst, mBuf); 
   CMCHKUNPK(cmUnpkProcId, &pst->srcProcId, mBuf); 
   CMCHKUNPK(cmUnpkEnt, &pst->srcEnt, mBuf); 
   CMCHKUNPK(cmUnpkInst, &pst->srcInst, mBuf); 
   CMCHKUNPK(cmUnpkEvent, &pst->event, mBuf);

   return ROK;
} /* end of cmUnpkPst */

/*
*
*       Fun:    cmUnpkElmtHdr
*
*       Desc:   Unpack element header
*
*       Ret:   ROK      - ok
*
*       Notes:  None
*
*       File:   cm_gen.c
*
*/
  
S16 cmUnpkElmtHdr
(
ElmtHdr *m,                  /* element header */
Buffer  *mBuf                /* message buffer */
)
{
 
   /* Present */
   CMCHKUNPK(oduUnpackBool, &m->pres , mBuf);

#if (LCAMT || ATM_BISUP)
   /* Unpack action indicator field */
   if (m->pres)
   {
      CMCHKUNPK(oduPackUInt8, &m->actnInd, mBuf);
   }
#endif

#if (LCAMT || ATM_BISUP)
      CMCHKUNPK(oduPackUInt16, &m->compInd, mBuf);
#endif /* LCAMT || ATM_BISUP */

   return ROK;
} /* end of cmUnpkElmtHdr */


/*
*
*       Fun:   cmUnpkTknUInt8
*
*       Desc:  This function unpacks a token uint8_t
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknUInt8
(
TknUInt8 *tknUInt8,               /* token uint8_t */
Buffer *mBuf                /* message buffer */
)
{

   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknUInt8->pres, mBuf);

   if (tknUInt8->pres)
   {
      /* Value */
      CMCHKUNPK(oduPackUInt8, &tknUInt8->val, mBuf);
   }

   return ROK;
} /* end of cmUnpkTknUInt8 */


/*
*
*       Fun:   cmUnpkTknS8
*
*       Desc:  This function unpacks a token S8
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknS8
(
TknS8 *tknS8,               /* token S8 */
Buffer *mBuf                /* message buffer */
)
{
 
   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknS8->pres, mBuf);
 
   if (tknS8->pres)
   {
      /* Value */
      CMCHKUNPK(SUnpkS8, &tknS8->val, mBuf);
   }
 
   return ROK;
} /* end of cmUnpkTknS8 */


/*
*
*       Fun:   cmUnpkTknUInt16
*
*       Desc:  This function unpacks a token uint16_t
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknUInt16
(
TknUInt16 *tknUInt16,             /* token uint16_t */
Buffer *mBuf                /* message buffer */
)
{

   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknUInt16->pres, mBuf);

   if (tknUInt16->pres)
   {
      /* Value */
      CMCHKUNPK(oduPackUInt16, &tknUInt16->val, mBuf);
   }

   return ROK;
} /* end of cmUnpkTknUInt16 */


/*
*
*       Fun:   cmUnpkTknUInt32
*
*       Desc:  This function unpacks a token uint32_t
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknUInt32
(
TknUInt32 *tknUInt32,             /* token uint32_t */
Buffer *mBuf                /* message buffer */
)
{

   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknUInt32->pres, mBuf);

   if (tknUInt32->pres)
   {
      /* Value */
      CMCHKUNPK(oduPackUInt32, &tknUInt32->val, mBuf);
   }

   return ROK;
} /* end of cmUnpkTknUInt32 */

/*
*
*       Fun:   cmUnpkTknStr
*
*       Desc:  This function unpacks a token string - regular size
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknStr
(
TknStr *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                     /* counter */


   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknStr->pres, mBuf);

   if (tknStr->pres)
   {
      /* Length */
      CMCHKUNPK(oduPackUInt8, &tknStr->len, mBuf);

      /* Value */
      for (i = (tknStr->len - 1); i >= 0; i--)
      {
         CMCHKUNPK(oduPackUInt8, &tknStr->val[i], mBuf);
      }
   }
   else
      tknStr->len = 0;

   return ROK;
} /* end of cmUnpkTknStr */

/*
*
*       Fun:   cmUnpkTknStrM
*
*       Desc:  This function unpacks a token string - medium size
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknStrM
(
TknStrM *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                     /* counter */


   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknStr->pres, mBuf);

   if (tknStr->pres)
   {
      /* Length */
      CMCHKUNPK(oduPackUInt8, &tknStr->len, mBuf);

      /* Value */
      for (i = (tknStr->len - 1); i >= 0; i--)
      {
         CMCHKUNPK(oduPackUInt8, &tknStr->val[i], mBuf);
      }
   }
   else
      tknStr->len = 0;

   return ROK;
} /* end of cmUnpkTknStrM */


/*
*
*       Fun:   cmUnpkTknStrS
*
*       Desc:  This function unpacks a token string - small size
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknStrS
(
TknStrS *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                     /* counter */


   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknStr->pres, mBuf);

   if (tknStr->pres)
   {
      /* Length */
      CMCHKUNPK(oduPackUInt8, &tknStr->len, mBuf);

      /* Value */
      for (i = (tknStr->len - 1); i >= 0; i--)
      {
         CMCHKUNPK(oduPackUInt8, &tknStr->val[i], mBuf);
      }
   }
   else
      tknStr->len = 0;

   return ROK;
} /* end of cmUnpkTknStrS */


/*
*
*       Fun:   cmUnpkTknStrE
*
*       Desc:  This function unpacks a token string - extended size
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknStrE
(
TknStrE *tknStr,             /* token string */
Buffer *mBuf                /* message buffer */
)
{
   Cntr i;                     /* counter */


   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknStr->pres, mBuf);

   if (tknStr->pres)
   {
      /* Length */
      CMCHKUNPK(oduPackUInt8, &tknStr->len, mBuf);

      /* Value */
      for (i = (tknStr->len - 1); i >= 0; i--)
      {
         CMCHKUNPK(oduPackUInt8, &tknStr->val[i], mBuf);
      }
   }
   else
      tknStr->len = 0;

   return ROK;
} /* end of cmUnpkTknStrE */

#ifndef CMFILE_REORG_1


/*
*
*       Fun:   cmUnpkPnNodeId
*
*       Desc:  This function unpacks a PnNodeId structure from a buffer
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkPnNodeId
(
PnNodeId  *dst,     /* source PNNI Node Id */
Buffer *mBuf        /* message buffer */
)
{
   S16 i;
   
   
   for (i = 0; i < PN_NODEID_LEN; i++)
   {
      CMCHKUNPK(oduPackUInt8, &dst->id[i], mBuf);
   }
   
   return ROK;
} /* cmUnpkPnNodeId */

#endif /* CMFILE_REORG_1 */


/*
 *
 *      Fun:   cmUnpkTknStr4
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmUnpkTknStr4
(
TknStr4 *tknStr,             /* token string */
Buffer  *mBuf                /* message buffer */
)
{

   CMUNPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmUnpkTknStr4 */



/*
 *
 *      Fun:   cmUnpkTknStr12
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmUnpkTknStr12
(
TknStr12 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMUNPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmUnpkTknStr12 */


/*
 *
 *      Fun:   cmUnpkTknStr32
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmUnpkTknStr32
(
TknStr32 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMUNPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmUnpkTknStr32 */


/*
 *
 *      Fun:   cmUnpkTknStr64
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmUnpkTknStr64
(
TknStr64 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMUNPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmUnpkTknStr64 */


/*
 *
 *      Fun:   cmUnpkTknStr132
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmUnpkTknStr132
(
TknStr132 *tknStr,             /* token string */
Buffer   *mBuf                /* message buffer */
)
{

   CMUNPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmUnpkTknStr132 */


/*
 *
 *      Fun:   cmUnpkTknStr256
 *
 *      Desc:  This function packs a token string of size 4
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmUnpkTknStr256
(
TknStr256 *tknStr,             /* token string */
Buffer    *mBuf                /* message buffer */
)
{

   CMUNPKTKNSTR(tknStr, mBuf);

   return ROK;

} /* end of cmUnpkTknStr256 */


/*
 *
 *      Fun:   cmUnpkTknOid
 *
 *      Desc:  This function packs a Object Identifier token
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */
  
S16 cmUnpkTknOid
(
TknOid   *tknOid,             /* Object Identifier token */
Buffer   *mBuf                /* message buffer */
)
{
   uint16_t    i;

 
   /* Unpack the token header */
   CMCHKUNPK(oduPackUInt8, &tknOid->pres, mBuf);

   if (tknOid->pres == TRUE)
   {
      /* Unpack the length */
      CMCHKUNPK(oduPackUInt8, &tknOid->len, mBuf);

      /* Pack the value */
      for (i = 1; i <= (uint16_t)tknOid->len; i++)
      {
         /* cm_gen_c_001.main_33: changes for TknOid value from uint16_t to uint32_t
          * with compilation flag TKNOID_UINT16 */
#ifndef TKNOID_UINT16
         CMCHKUNPK(oduPackUInt32, &tknOid->val[tknOid->len - i], mBuf);
#else
         CMCHKUNPK(oduPackUInt16, &tknOid->val[tknOid->len - i], mBuf);
#endif /* !TKNOID_UINT16 */
      }
   }

   return ROK;
} /* end of cmUnpkTknOid */


/*
*
*       Fun:   cmUnpkTknS32
*
*       Desc:  This function unpacks a token S32
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTknS32
(
TknS32 *tknS32,             /* token S32 */
Buffer *mBuf                /* message buffer */
)
{

   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknS32->pres, mBuf);

   if (tknS32->pres)
   {
      /* Value */
      CMCHKUNPK(SUnpkS32, &tknS32->val, mBuf);
   }

   return ROK;
} /* end of cmUnpkTknS32 */


/*
*
*       Fun:   cmUnpkHeader
*
*       Desc:  This function is used to Unpack Header structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkHeader
(
Header *header,   /* header structure */
Buffer *mBuf      /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt16, &header->msgLen, mBuf); 
   CMCHKUNPK(oduPackUInt8, &header->msgType, mBuf); 
   CMCHKUNPK(oduPackUInt8, &header->version, mBuf); 
   CMCHKUNPK(oduPackUInt16, &header->seqNmb, mBuf); 
   CMCHKUNPK(cmUnpkEntityId, &header->entId, mBuf);    
   CMCHKUNPK(cmUnpkElmntId, &header->elmId, mBuf);
#ifdef LMINT3
   CMCHKUNPK(cmUnpkTranId, &header->transId, mBuf);
   CMCHKUNPK(cmUnpkSelector, &header->response.selector, mBuf);
   CMCHKUNPK(cmUnpkPriority, &header->response.prior, mBuf);
   CMCHKUNPK(cmUnpkRoute, &header->response.route, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &header->response.mem, mBuf);
#endif /* LMINT3 */

   return ROK;
} /* end of cmUnpkHeader */


/*
*
*       Fun:   cmUnpkCmStatus
*
*       Desc:  This function unpacks common management status structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkCmStatus
(
CmStatus *sta,              /* status structure */
Buffer *mBuf                /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt16, &sta->status, mBuf);
   CMCHKUNPK(oduPackUInt16, &sta->reason, mBuf);

   return ROK;
} /* end of cmUnpkCmStatus */


/*
*
*       Fun:   cmUnpkCmAlarm
*
*       Desc:  This function unpacks common management alarm structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkCmAlarm
(
CmAlarm *alarm,             /* alarm structure */
Buffer *mBuf                /* message buffer */
)
{

   CMCHKUNPK(cmUnpkDateTime, &alarm->dt, mBuf);
   CMCHKUNPK(oduPackUInt16, &alarm->category, mBuf);
   CMCHKUNPK(oduPackUInt16, &alarm->event, mBuf);
   CMCHKUNPK(oduPackUInt16, &alarm->cause, mBuf);

   return ROK;
} /* end of cmUnpkCmAlarm */


/*
*
*       Fun:   cmUnpkSmCfg
*
*       Desc:  This function unpacks the stack manager structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkSmCfg
(
SmCfg *smCfg,           /* stack manager */
Buffer *mBuf            /* message buffer */
)
{

   CMCHKUNPK(cmUnpkEnt, &smCfg->ent, mBuf); 
   CMCHKUNPK(cmUnpkInst, &smCfg->inst, mBuf); 
   CMCHKUNPK(cmUnpkRegion, &smCfg->region, mBuf); 
   CMCHKUNPK(cmUnpkPool, &smCfg->pool, mBuf); 
   CMCHKUNPK(cmUnpkPrior, &smCfg->prior, mBuf); 
   CMCHKUNPK(cmUnpkRoute, &smCfg->route, mBuf); 
   CMCHKUNPK(cmUnpkSelector, &smCfg->selector, mBuf);

   return ROK;
} /* end of cmUnpkSmCfg */


/*
*
*       Fun:   cmUnpkTmrCfg
*
*       Desc:  This function unpacks the timer configuration structure
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkTmrCfg
(
TmrCfg *tmrCfg,         /* timer configuration */
Buffer *mBuf            /* message buffer */
)
{

   CMCHKUNPK(oduUnpackBool, &tmrCfg->enb, mBuf); 
   CMCHKUNPK(oduPackUInt16, &tmrCfg->val, mBuf);

   return ROK;
} /* end of cmUnpkTmrCfg */

/*
 *
 *      Fun:   cmUnpkTknBuf
 *
 *      Desc:  This function unpacks a token buffer
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
        File:  cm_gen.c
 *
 */

S16 cmUnpkTknBuf
(
TknBuf *tknBuf,                /* token string */
Buffer    **tBuf                /* message buffer */
)
{
   Buffer *mBuf;



   mBuf = *tBuf;
  
   /* Token Header */
   CMCHKUNPK(oduPackUInt8, &tknBuf->pres, mBuf);

   if(tknBuf->pres)
   {
      MsgLen msgLen, buflen;
      Region region;                 /* region */
      Pool   pool;                   /* pool */

      cmUnpkMsgLen(&msgLen, mBuf);
      SFndLenMsg(mBuf, &buflen);
      if (buflen > msgLen)
      {
         if( SSegMsg(mBuf, msgLen, &tknBuf->val) != ROK )
         {
            return RFAILED;
         }
      }
      else
      {
          /* Allocate the buffer for tknbuf->val */
          SGetBufRegionPool(mBuf, &region, &pool);
          SGetMsg(region, pool, &(tknBuf->val));
      }
      
     /* now Swap the two Buffer pointers */
      SSwapMsg(mBuf,tknBuf->val);
      
   }

   *tBuf = mBuf;

   return ROK;
} /* end of cmUnpkTknBuf */

#ifdef TDS_ROLL_UPGRADE_SUPPORT
/*
*
*       Fun:   cmUnpkIntf
*
*       Desc:  This function unpacks the interface information
*
*       Ret:   ROK      - ok
*
*       Notes: None.
*
*       File:  cm_gen.c
*
*/
  
S16 cmUnpkIntf
(
CmIntf *intf,           /* interface info */
Buffer *mBuf            /* message buffer */
)
{

   CMCHKUNPK(cmUnpkIntfVer, &intf->intfVer, mBuf); 
   CMCHKUNPK(cmUnpkIntfId,  &intf->intfId,  mBuf); 

   return ROK;
} /* end of cmUnpkIntf */
#endif /* TDS_ROLL_UPGRADE_SUPPORT */

/*
*
*       Fun:   getTransId
*
*       Desc:  This function return the transaction ID used for interface transactions 
*
*       Ret: gTransId
*
*       File:  cm_gen.c
*
*/
uint16_t getTransId()
{
   gTransId = (gTransId%65535) + 1;
   return gTransId; 
}
/**********************************************************************
         End of file
**********************************************************************/
