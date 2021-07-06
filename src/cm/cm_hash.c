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
  
     Name:     common hash functions
  
     Type:     C source file
  
     Desc:     Hashing functions used by various layers.
               (Newer version of functions in cm_bdy1)
  
               Using hash lists in a product:
               ------------------------------

               Wherever a hash list is needed, a corresponding hash list
               control point (CmHashListCp) should be declared. The structure 
               definition of the entries that belong to the hash list must 
               include a declaration of the hash list entry header
               (CmHashListEnt) along with the key for the hash list (this
               may be any scalar or structure type subfield of the entry).
               For example, we need a hash list in a SAP to maintain a table
               of addresses:

               typedef struct xySAPCb       (SAP control block)
               {
                  ...
                  CmHashListCp addrHlCp;    (hash list for addresses)
                  ...
               } XySAPCb;

               typedef struct xyAddrEnt     (hash list entry for an address)
               {
                  ...
                  CmHashListEnt hl;         (hash list entry header)
                  ...
                  XyAddr addr;              (hash list key)
                  ...
               } XyAddrEnt;

               Functions available:
               --------------------

               The functions available for using hash lists are defined
               below. The accompanying comments explain the usage in detail.

               Implementation details:
               -----------------------

               A hash list is identified by its control point
               (CmHashListCp). This control point stores the characteristics
               of the hash list as well as a pointer to the hash list bins.
               The storage for the bins is allocated when the hash list is
               initialized. Each bin is organized as a doubly linked list of
               entries whose key maps to this bin. The hash function used to
               map keys to a bin simply adds up all the octets of the key
               and then divides the sum by the number of bins. Variable size
               keys are allowed. Duplicate keys may be present if explicitly
               allowed; if so, they can be retrieved by supplying a sequence
               number in the find routine. A given structure may be attached
               to more than one hash list if it contains multiple hash list
               entry header fields.

     File:     cm_hash.c
  
*********************************************************************21*/
  
  
/* header include files -- defines (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_hash.h"       /* common hash functions */
#include "cm_err.h"        /* common functions error */

/* header include -- typedef structs (.x) */

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_lib.x"        /* common library functions */
#include "cm_hash.x"       /* common hash functions */


/* local defines */

/* local externs */

/* forward references */
/* cm_hash_c_001.main_22: Fixing warnings on GCC compiler */
#ifdef __cplusplus
 extern "C" {
#endif

static S16 cmHashFuncBCD8   ARGS((CmHashListCp *hashListCp, uint8_t *key, 
                                         uint16_t keyLen, uint16_t *idx));

static S16 cmHashFuncConId  ARGS((CmHashListCp *hashListCp, uint8_t *key, 
                                         uint16_t keyLen, uint16_t *idx));

static S16 cmHashFuncU32Mod  ARGS((CmHashListCp *hashListCp, uint8_t *key, 
                                         uint16_t keyLen, uint16_t *idx));

static S16 cmHashFuncString  ARGS((CmHashListCp *hashListCp, uint8_t *key, 
                                         uint16_t keyLen, uint16_t *idx));

static S16 cmHashFuncDefault ARGS((CmHashListCp *hashListCp, uint8_t *key, 
                                         uint16_t keyLen, uint16_t *idx));

static S16 cmHashFuncAnyKey  ARGS((CmHashListCp *hashListCp, uint8_t *key, 
                                         uint16_t keyLen, uint16_t *idx));

static S16 cmHashMatchKey ARGS((uint8_t *key1, uint16_t keyLen1, uint8_t *key2, uint16_t keyLen2));

static S16 cmListInsert   ARGS((CmListEnt *oldEntry, CmListEnt *newEntry));

static S16 cmListDelete   ARGS((CmListEnt *entry));

/* cm_hash_c_001.main_22: Fixing warnings on GCC compiler */
static S16 cmHashFuncMult24 ARGS((CmHashListCp *hashListCp, uint8_t *key, uint16_t keyLen, uint16_t *idx));

static S16 cmHashFuncDirIdx ARGS((CmHashListCp *hashListCp, uint8_t *key, uint16_t keyLen, uint16_t *idx));

#ifdef __cplusplus
 }
#endif

/* functions in other modules */
  
/* public variable declarations */

/* private variable declarations */


/*
 *     private support functions
 */

/*
*
*       Fun:   cmHashFuncAnyKey
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_ANY. 
*
*              return index to hash table 
*
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncAnyKey
(
CmHashListCp  *hashListCp,        /* hash list control point */
uint8_t       *key,               /* key string */
uint16_t      keyLen,             /* length of key string */
uint16_t      *idx                /* idx to return */
) 
{
   uint32_t   a;                 /* hash variables */
   uint32_t   b;                 /* hash variables */         
   uint32_t   c;                 /* hash variables */
   uint32_t   len;               /* length */

   /*cm_hash_c_001.main_23 : Fix for TRACE5 feature crash due to missing TRC MACRO*/
   /* Set up the internal state */
   len = keyLen;    /* key length */
   a = 0x9e3779b9;  /* a = b = the golden ratio; an arbitrary value */
   b = 0x9e3779b9;
   c = 0x12345678;  /* some value */

   /*---------------------------------------- handle most of the key */
   while (len >= 12)
   {
      a += (key[0] +((uint32_t)key[1]<<8) +((uint32_t)key[2]<<16) +((uint32_t)key[3]<<24));
      b += (key[4] +((uint32_t)key[5]<<8) +((uint32_t)key[6]<<16) +((uint32_t)key[7]<<24));
      c += (key[8] +((uint32_t)key[9]<<8) +((uint32_t)key[10]<<16)+((uint32_t)key[11]<<24));
      CM_HASH_MIX(a, b, c);
      key += 12; len -= 12;
   }

   /*------------------------------------- handle the last 11 bytes */
   c += keyLen;
   switch(len)              /* all the case statements fall through */
   {
   case 11: c+=((uint32_t)key[10]<<24);
   case 10: c+=((uint32_t)key[9]<<16);
   case 9 : c+=((uint32_t)key[8]<<8);
      /* the first byte of c is reserved for the keyLen */
   case 8 : b+=((uint32_t)key[7]<<24);
   case 7 : b+=((uint32_t)key[6]<<16);
   case 6 : b+=((uint32_t)key[5]<<8);
   case 5 : b+=key[4];
   case 4 : a+=((uint32_t)key[3]<<24);
   case 3 : a+=((uint32_t)key[2]<<16);
   case 2 : a+=((uint32_t)key[1]<<8);
   case 1 : a+=key[0];
     /* case 0: nothing left to add */
   }
   CM_HASH_MIX(a,b,c);
   /*-------------------------------------------- report the result */

   /* if nmbBins is a power of 2, use shift, else use division */
   if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
      *idx = (uint16_t) (c & hashListCp->binBitMask);
   else
      *idx = (uint16_t) (c % hashListCp->nmbBins);

   return ROK;
} /* end of cmHashFuncAnyKey */


/*
*
*       Fun:   cmHashFuncU32Mod
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_MOD. 
*
*              return (idx % hash_table_size);
*
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncU32Mod
(
CmHashListCp *hashListCp,        /* hash list control point */
uint8_t      *key,               /* key string */
uint16_t     keyLen,             /* length of key string */
uint16_t     *idx                /* idx to return */
) 
{
   uint32_t  sum;                /* Sum of octets for hash function */


   /* keyLen is marked Unused to remove compilation 
    * warnings. */
   UNUSED(keyLen);

   sum = *((uint32_t *)key);

   /* if nmbBins is a power of 2, use shift, else use division */
   if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
      *idx = (uint16_t) (sum & hashListCp->binBitMask);
   else
      *idx = (uint16_t) (sum % hashListCp->nmbBins);

   return ROK;

} /* end of cmHashFuncU32Mod () */

/*
*
*       Fun:   cmHashFuncBCD8
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_BCD8. 
*
*       Steps:
*              1. Converts the 8 BCD coded octets into 2 uint32_ts 
*              2. Adds 2 uint32_ts to get one uint32_t. 
*              3. Apply uint32_tMod technique to get the index 
*              4. Return the index
*
*       Note: 
*              Here we are no bothered if the keyLen is more than 8. 
*              We are interested only in the first 8 octets.
*              
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncBCD8
(
CmHashListCp  *hashListCp,        /* hash list control point */
uint8_t       *key,               /* key string */
uint16_t      keyLen,             /* length of key string */
uint16_t      *idx                /* idx to return */
) 
{
   uint16_t  tmp16 = 0;
   uint32_t  firstUInt32 = 0;    /* First  uint32_t prepared for lower 4 octets */
   uint32_t  secondUInt32 = 0;   /* Second uint32_t prepared for higher 4 octets */
   uint32_t  sum;             /* Sum of the above 2 octets to get the index */


   /* keyLen is marked Unused to remove compilation 
    * warnings. */
   UNUSED(keyLen);

   /* Compute second uint32_t */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) key[0]); 
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) key[1]);
   secondUInt32 = (uint32_t) PutHiWord(secondUInt32, (uint16_t) tmp16); 
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) key[2]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) key[3]);
   secondUInt32 = (uint32_t) PutLoWord(secondUInt32, (uint16_t) tmp16); 


   /* Compute first uint32_t */
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) key[4]); 
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) key[5]);
   firstUInt32 = (uint32_t) PutHiWord(firstUInt32, (uint16_t) tmp16); 
   tmp16 = (uint16_t) PutHiByte(tmp16, (uint8_t) key[6]);
   tmp16 = (uint16_t) PutLoByte(tmp16, (uint8_t) key[7]);
   firstUInt32 = (uint32_t) PutLoWord(firstUInt32, (uint16_t) tmp16); 

   sum = firstUInt32 + secondUInt32;

   /* if nmbBins is a power of 2, use shift, else use division */
   if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
      *idx = (uint16_t) (sum & hashListCp->binBitMask);
   else
      *idx = (uint16_t) (sum % hashListCp->nmbBins);

   return ROK;
} /* end of cmHashFuncBCD8 () */

/*
*
*       Fun:   cmHashFuncString
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_STR. 
*
*              for (length of string)
*                 idx = (31 * idx) + *string;
*
*              return (idx % hash_table_size);
*
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncString
(
CmHashListCp *hashListCp,        /* hash list control point */
uint8_t      *key,               /* key string */
uint16_t     keyLen,             /* length of key string */
uint16_t     *idx                /* idx to return */
) 
{
   uint16_t  cntr;               /* Index */
   uint32_t  sum;                /* Sum of octets for hash function */


   sum = 0;

   for (cntr = 0; cntr < keyLen; cntr++)
   {
      sum = (CM_STR_HASHFUNC_CONSTANT * sum) + key[cntr];
   }

   /* if nmbBins is a power of 2, use shift, else use division */
   if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
      *idx = (uint16_t) (sum & hashListCp->binBitMask);
   else
      *idx = (uint16_t) (sum % hashListCp->nmbBins);

   return ROK;

} /* end of cmHashFuncString () */


  
/*
*
*       Fun:   cmHashFuncDefault
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_DEF. 
*
*              Adds up all the octets of the key string
*              and divides the sum by the range to get the desired index.
*
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncDefault
(
CmHashListCp *hashListCp,        /* hash list control point */
uint8_t      *key,               /* key string */
uint16_t     keyLen,             /* length of key string */
uint16_t     *idx                /* index to return */
) 
{
   uint32_t  sum;                /* sum of key string octets */


   /* add all bytes of the key */
   sum = 0;
   while (keyLen--)
      sum += (uint32_t) (*key++);

   /* compute index by dividing the range into the sum */

   /* if nmbBins is a power of 2, use shift, else use division */
   if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
      *idx = (uint16_t) (sum & hashListCp->binBitMask);
   else
      *idx = (uint16_t) (sum % hashListCp->nmbBins);

   return ROK;

} /* end of cmHashFuncDefault */

  
/*
*
*       Fun:   cmHashFuncMult24
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_MULT24. 
*
*              Multiplies the given key (max k bits) with a constant
*              multiplier and extracts p bits of the result, from the 
*              bit position k-1 to bit position k-p, to get the hash
*              list index. p is such that 2^p is number of bins.
*
*              The constant multiplier is the floor of A * 2^k, for
*              some constant A.
*
*              This function uses a pre-computed constant multiplier
*              CM_HASH_MULTMETHOD_CNST24, which is computed for 
*              A = (sqrt(5) - 1)/2, and k = 24 bits.
*
*              This hashing method is explained in section 12.3.2 of
*              "Introduction to Algorithms" by Thomas H. Cormen et al.,
*              The MIT Press.
*
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncMult24
(
CmHashListCp *hashListCp,        /* hash list control point */
uint8_t      *key,               /* key string */
uint16_t     keyLen,             /* length of key string */
uint16_t     *idx                /* index to return */
) 
{
   uint32_t  prod;               /* (constant multiplier * key) */
   uint8_t   shift;              /* Bits to be shifted to get index */


   UNUSED(keyLen);

#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if (hashListCp->binBitMask == CM_HASH_NOBITMASK)
      return RFAILED;
#endif

   prod = CM_HASH_MULT24_CONST * *((uint32_t *)key);

   shift = CM_HASH_MULT24_BITPOS - hashListCp->nmbBinBits;
   *idx = ((uint16_t) (prod & (hashListCp->binBitMask << shift))) >> shift;

   return ROK;
} /* end of cmHashFuncMult24 */



  
/*
*
*       Fun:   cmHashFuncConId
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_CONID. 
*
*              This function can be used for keys that are an integer whose 
*              size is uint8_t, uint16_t or uint32_t. Instead of adding all octets of the key,
*              this fn computes the "index" of the bin in which the entry
*              needs to be inserted by taking a modulo of the integer with the 
*              total number of bins.
*              This function is typically suitable for a sequentially increasing
*              number like suConnId/spConnId
*
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncConId
(
CmHashListCp *hashListCp,        /* hash list control point */
uint8_t      *key,               /* key string */
uint16_t     keyLen,             /* length of key string */
uint16_t     *idx                /* index to return */
) 
{


   /* switch based on the length of the key */
   switch (keyLen)
   {
      case CM_HASHKEYLEN_UINT32:
        if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
         *idx = (uint16_t) (((uint32_t) *((uint32_t *)key)) & hashListCp->binBitMask);
        else
         *idx = (uint16_t) (((uint32_t) *((uint32_t *)key)) % hashListCp->nmbBins);  
        break;

      case CM_HASHKEYLEN_UINT16:
         if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
           *idx = (uint16_t) (((uint16_t)*((uint16_t *)key)) & hashListCp->binBitMask);
         else
           *idx = (uint16_t) (((uint16_t)*((uint16_t *)key)) % hashListCp->nmbBins);
         break;

      case CM_HASHKEYLEN_UINT8:
         if (hashListCp->binBitMask != CM_HASH_NOBITMASK)
           *idx = (uint16_t) (((uint8_t)*((uint8_t *)key)) & hashListCp->binBitMask);
         else
           *idx = (uint16_t) (((uint8_t)*((uint8_t *)key)) % hashListCp->nmbBins);
         break;

      default:
         return RFAILED;
   }
   return ROK;

} /* end of cmHashFuncConId */


  
/*
*
*       Fun:   cmHashFuncDirIdx
*
*       Desc:  Computes the hash list index (bin number) for a specified
*              key of type CM_HASH_KEYTYPE_DIRINDEX. 
*
*              The key is the hash table index.
*
*       Ret:   ROK     - successful, *idx contains computed index 
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashFuncDirIdx
(
CmHashListCp *hashListCp,        /* hash list control point */
uint8_t      *key,               /* key string */
uint16_t     keyLen,             /* length of key string */
uint16_t     *idx                /* index to return */
) 
{

   UNUSED(hashListCp);
   UNUSED(keyLen);

   *idx = *((uint16_t *) key);

   return ROK;
} /* end of cmHashFuncDirIdx */

  
/*
*
*       Fun:   cmHashMatchKey
*
*       Desc:  Compares two keys and determines if they match.
*
*       Ret:   ROK     - match successful
*              RFAILED - match failed (non-matching key values)
*
*       Notes: None.
*
*       File:  cm_hash.c
*
*/

static S16 cmHashMatchKey
(
uint8_t *key1,                         /* first key string */
uint16_t keyLen1,                      /* length of first key string */
uint8_t *key2,                         /* second key string */
uint16_t keyLen2                       /* length of second key string */
) 
{

   /* compare key lengths */
   if (keyLen1 != keyLen2)
      return RFAILED;

   /* compare key strings */
   return (cmMemcmp(key1, key2, (PTR) keyLen1));
} /* end of cmHashMatchKey */

  
/*
*
*       Fun:   cmListInsert
*
*       Desc:  Adds an entry into a doubly linked list
*
*       Ret:   ROK      - insertion successful
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/

static S16 cmListInsert
(
CmListEnt *oldEntry,                    /* add new entry after this entry */
CmListEnt *newEntry                     /* new entry to add */
) 
{

   newEntry->next         = oldEntry->next;
   newEntry->prev         = oldEntry;
   oldEntry->next         = newEntry;
   (newEntry->next)->prev = newEntry;

   return ROK;
} /* end of cmListInsert */

  
/*
*
*       Fun:   cmListDelete
*
*       Desc:  Deletes an entry from a doubly linked list
*
*       Ret:   ROK      - deletion successful
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/

static S16 cmListDelete
(
CmListEnt *entry                        /* entry to delete */
) 
{

   if (entry == NULLP) 
      return RFAILED;

   if (entry->prev != NULLP)
      (entry->prev)->next = entry->next;

   if (entry->next != NULLP)
      (entry->next)->prev = entry->prev;

   return ROK;
} /* end of cmListDelete */



/*
 *     public functions
 */


/*
*
*       Fun:   cmHashListInit
*
*       Desc:  Initializes a hash list. Parameters are: 
*
*              hashListCp   control point for hash list
*              nmbBins      number of bins in the hash list. Storage will
*                           be allocated for them from the indicated memory
*                           region and pool.
*              offset       if the entries in this hash list are also going
*                           to be attached to another hash list, they will
*                           contain multiple hash list entry headers. The
*                           offset indicates the offset of the entry header
*                           for this hash list in the entry structure.
*              dupFlg       whether entries with duplicate keys are allowed
*                           to be inserted in the hash list.
*              keyType      indicates type of key which can be used to select
*                           different hash functions. Ignored in this
*                           implementation.
*              region       
*              pool         for allocating storage for bins.
*
*       Ret:   ROK      - initialization successful
*              RFAILED  - initialization failed, lack of memory
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/
S16 cmHashListInit
(
CmHashListCp *hashListCp,  /* hash list to initialize */
uint16_t     nmbBins,      /* number of hash list bins */
uint16_t     offset,       /* offset of CmHashListEnt in entries */
Bool         dupFlg,       /* allow duplicate keys */
uint16_t     keyType,      /* key type for selecting hash fn */
Region       region,       /* memory region to allocate bins */
Pool         pool          /* memory pool to allocate bins */
)
{
   uint16_t i;
#ifndef CM_MT_HASH_BIN
   CmListEnt *hl;
#else
   CmListBinEnt *hl;
#endif


#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if (hashListCp == NULLP) 
      return RFAILED;
#endif

   /* initialize control point fields */
   hashListCp->hl      = NULLP;
   hashListCp->region  = region;
   hashListCp->pool    = pool;
   hashListCp->nmbBins = 0;
   hashListCp->binBitMask = CM_HASH_NOBITMASK;
   hashListCp->nmbBinBits = 0;
#ifndef CM_MT_HASH_BIN
   hashListCp->nmbEnt  = 0;
#endif
   hashListCp->offset  = offset;
   hashListCp->dupFlg  = dupFlg;
   hashListCp->keyType = keyType;
   hashListCp->hashFunc = NULLP;

   /* initialize hash function for this key type */
   switch (keyType)
   {
      case CM_HASH_KEYTYPE_MULT24:
         /* return failure if number of bins is not a power of 2 */
         if (((nmbBins) & (nmbBins - 1)) != 0)
            return  (RFAILED);

         hashListCp->hashFunc = cmHashFuncMult24;
         break;

      case CM_HASH_KEYTYPE_DIRIDX:
         hashListCp->hashFunc = cmHashFuncDirIdx;
         break;

      case CM_HASH_KEYTYPE_STR:
         hashListCp->hashFunc = cmHashFuncString;
         break;

      case CM_HASH_KEYTYPE_UINT32_MOD:
         hashListCp->hashFunc = cmHashFuncU32Mod;
         break;

      case CM_HASH_KEYTYPE_BCD8:
         hashListCp->hashFunc = cmHashFuncBCD8;
         break;

      case CM_HASH_KEYTYPE_ANY:
         hashListCp->hashFunc = cmHashFuncAnyKey;
         break;

      case CM_HASH_KEYTYPE_CONID:
        hashListCp->hashFunc = cmHashFuncConId;
        break;

      case CM_HASH_KEYTYPE_DEF:      /* default hash function */
      default:                       /* illegal key type */
         hashListCp->hashFunc = cmHashFuncDefault;
         break;
   }

   /* allocate memory for bins */
   if (nmbBins)
   {
#ifndef CM_MT_HASH_BIN
      if (SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,region, pool, (Data **) &hashListCp->hl, 
                   (Size) (nmbBins * sizeof(CmListEnt))) != ROK)
         return RFAILED;
#else
      if (SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,region, pool, (Data **) &hashListCp->hl, 
                   (Size) (nmbBins * sizeof(CmListBinEnt))) != ROK)
         return RFAILED;
#endif

      /* initialize bin pointers */
      hl = hashListCp->hl;
      for(i = 0; i < nmbBins; i++)
      {
#ifndef CM_MT_HASH_BIN
         hl[i].next = hl[i].prev = &hl[i];
#else
         /* This initialization is being done as a part of checking 
          * the presence of empty/non-empty bins. 
          */
         hl[i].next = hl[i].prev = (CmListEnt*)&hl[i];
         hl[i].nmbEnt = 0;
#endif
      }

      /* initialize bin size */
      hashListCp->nmbBins = nmbBins;

      /* initialize bin bit mask */
      if (((nmbBins) & (nmbBins - 1)) == 0)
      {
         uint16_t   binBitMask;

         /* number of bins is a power of 2 */
         hashListCp->binBitMask = nmbBins - 1;

         /* compute number of bits in the bit mask */
         for (binBitMask = hashListCp->binBitMask; binBitMask; binBitMask >>= 1)
            hashListCp->nmbBinBits++;

      }
   }

   return ROK;
} /* end of cmHashListInit */


/*
*
*       Fun:   cmHashListDeinit
*
*       Desc:  Deinitializes a hash list. Deallocates memory for bins
*              and resets header fields. Parameters are: 
*
*              hashListCp   control point for hash list
*
*       Ret:   ROK      - successful
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/
S16 cmHashListDeinit
(
CmHashListCp *hashListCp   /* hash list to deinitialize */
)
{

#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if (hashListCp == NULLP) 
      return RFAILED;
#endif

   /* deallocate memory for bins */
   if (hashListCp->nmbBins)
#ifndef CM_MT_HASH_BIN
      (Void) SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,hashListCp->region, hashListCp->pool, 
                      (Data *) hashListCp->hl, 
                      (Size) (hashListCp->nmbBins * sizeof(CmListEnt)));
#else
      (Void) SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,hashListCp->region, hashListCp->pool, 
                      (Data *) hashListCp->hl, 
                      (Size) (hashListCp->nmbBins * sizeof(CmListBinEnt)));
#endif

   /* deinitialize control point fields */
   hashListCp->hl      = NULLP;
   hashListCp->region  = REGIONNC;
   hashListCp->pool    = 0;
   hashListCp->nmbBins = 0;
   hashListCp->binBitMask = CM_HASH_NOBITMASK;
#ifndef CM_MT_HASH_BIN
   hashListCp->nmbEnt  = 0;
#endif
   hashListCp->offset  = 0;
   hashListCp->dupFlg  = FALSE;
   hashListCp->keyType = CM_HASH_KEYTYPE_DEF;
   hashListCp->hashFunc = NULLP;

   return ROK;
} /* end of cmHashListDeinit */

  
/*
*
*       Fun:   cmHashListInsert
*
*       Desc:  Inserts a new entry in the hash list. Parameters are: 
*
*              hashListCp   control point for hash list
*              entry        pointer to new entry to add in the hash list
*              key          pointer to key string in the new entry
*              keyLen       length of key string
*
*       Ret:   ROK      - insertion successful
*              ROKDUP   - insertion failed (duplicate key not allowed)
*              RFAILED  - insertion failed (incorrect parameter values)
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/

S16 cmHashListInsert
(
CmHashListCp *hashListCp,  /* hash list to add to */
PTR          entry,        /* entry to add */
uint8_t      *key,         /* pointer to key */
uint16_t     keyLen        /* length of key */
)
{
   CmHashListEnt *hashListEnt;    /* pointer to hash list entry header */
   PTR dupEntry;                  /* pointer to entry with duplicate key */
   uint16_t idx;                       /* index for insertion into hash list */


#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if ((hashListCp == NULLP) || (entry == NULLP) || 
       (key == NULLP) || (keyLen == 0))
      return RFAILED;
#endif

   /* check for duplicates */
   if (hashListCp->dupFlg == FALSE)
   {
      /* no duplicates allowed, check if key already exists */
      if (cmHashListFind(hashListCp, key, keyLen, 0, &dupEntry) == ROK)
         return (ROKDUP);
   }

   /* get pointer to hash list entry header */
   hashListEnt = (CmHashListEnt *) (((uint8_t *) entry) + hashListCp->offset);

   /* initialize hash list entry header */
   hashListEnt->list.next = NULLP;
   hashListEnt->list.prev = NULLP;
   hashListEnt->keyLen    = keyLen;
   hashListEnt->key       = key;

   /* compute index for insertion */
   if ((*hashListCp->hashFunc)(hashListCp, key, keyLen, &idx) != ROK)
      return RFAILED;

   hashListEnt->hashVal   = idx;

   /* insert into list */
   cmListInsert(hashListCp->hl[idx].prev, &hashListEnt->list);

   /* increment count of entries in hash list */
#ifndef CM_MT_HASH_BIN
   hashListCp->nmbEnt++;
#else
   hashListCp->hl[idx].nmbEnt++;
#endif /* #ifndef CM_MT_HASH_BIN */

   return ROK;
} /* end of cmHashListInsert */

  
/*
*
*       Fun:   cmHashListDelete
*
*       Desc:  Deletes an entry from the hash list. Parameters are:
*
*              hashListCp   control point for hash list
*              entry        pointer to entry to delete from the hash list
*
*       Ret:   ROK      - deletion successful
*              RFAILED  - deletion failed (incorrect parameter values)
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/

S16 cmHashListDelete
(
CmHashListCp *hashListCp,  /* hash list to delete from */
PTR          entry         /* entry to delete */
)
{
   CmHashListEnt *hashListEnt;    /* pointer to hash list entry header */
#ifdef CM_MT_HASH_BIN
   uint16_t idx;           /* index for selecting the right hash list bin */
#endif


#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if ((hashListCp == NULLP) || (entry == NULLP)) 
      return RFAILED;
#endif

   /* get pointer to hash list entry header */
   hashListEnt = (CmHashListEnt *) (((uint8_t *) entry) + hashListCp->offset);

   /* check if entry is already deleted if yes then return OK */
   if((hashListEnt->list.next == NULLP) ||
      (hashListEnt->list.prev == NULLP))
      return ROK;

#ifdef CM_MT_HASH_BIN
   /* compute index for insertion */
   if ((*hashListCp->hashFunc)(hashListCp, hashListEnt->key, 
                              hashListEnt->keyLen, &idx) != ROK)
      return RFAILED;
#endif /* #ifdef CM_MT_HASH_BIN */

   /* delete entry from list */
   cmListDelete(&hashListEnt->list);

   /* reinitialize hash list entry header */
   hashListEnt->list.next = NULLP;
   hashListEnt->list.prev = NULLP;
   hashListEnt->keyLen    = 0;
   hashListEnt->key       = NULLP;
   hashListEnt->hashVal   = 0;

   /* decrement count of entries in hash list */
#ifndef CM_MT_HASH_BIN
   hashListCp->nmbEnt--;
#else
   /* Find the right bin index and decrement the nmbEnt counter */
   hashListCp->hl[idx].nmbEnt--;
#endif /* #ifndef CM_MT_HASH_BIN */

   return ROK;
} /* end of cmHashListDelete */

  
/*
*
*       Fun:   cmHashListFind
*
*       Desc:  Finds an entry in the hash list. Parameters are:
*
*              hashListCp   control point for hash list
*              key          pointer to key string for search
*              keyLen       length of key string
*              seqNmb       sequence number in case duplicates allowed
*              entry        pointer to found entry
*
*       Ret:   ROK      - find successful, *entry points to found entry
*              RFAILED  - find failed, *entry is unchanged 
*                         (incorrect parameter values, key not found)
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/

S16 cmHashListFind
(
CmHashListCp *hashListCp,  /* hash list to search */
uint8_t      *key,         /* pointer to key */
uint16_t     keyLen,       /* length of key */
uint16_t     seqNmb,       /* used in case of duplicate keys */
PTR          *entry        /* entry to be returned */
)
{
   CmHashListEnt *hashListEnt;    /* pointer to hash list entry header */
#ifndef CM_MT_HASH_BIN
   CmListEnt *hashListBin;        /* pointer to hash list bin */
#else
   CmListBinEnt *hashListBin;        /* pointer to hash list bin */
   uint16_t entCnt=0;                     /* counter for number of entries */
#endif
   uint16_t i;                         /* counter for sequence number */
   uint16_t idx;                       /* index for insertion into hash list */


#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if ((hashListCp == NULLP) || (key == NULLP) || 
       (keyLen == 0) || (entry == NULLP))
      return RFAILED;
#endif

   /* compute hash table index */
   if ((*hashListCp->hashFunc)(hashListCp, key, keyLen, &idx) != ROK)
      return RFAILED;

   /* search this bin for exact key match */
   hashListBin = &hashListCp->hl[idx];
   hashListEnt = (CmHashListEnt *) hashListBin->next;

   /* examine each entry in bin */
   i = 0;
#ifndef CM_MT_HASH_BIN
   while (hashListBin != (CmListEnt *) hashListEnt)
#else
   for (entCnt=0; entCnt < hashListBin->nmbEnt; entCnt++)
#endif
   {
      /* check if key matches */
      if (cmHashMatchKey(hashListEnt->key, hashListEnt->keyLen, key, keyLen) 
          == ROK)
      {
         /* matching key */
         *entry = (PTR) (((uint8_t *) hashListEnt) - hashListCp->offset);

         /* check for duplicates */
         if (!hashListCp->dupFlg)
            return ROK;

         /* for duplicate key, check sequence number */
         if (i++ == seqNmb)
            return ROK;
      }

      /* go to next entry */
      hashListEnt = (CmHashListEnt *) hashListEnt->list.next;
   }

   /* no matching key found */
   return RFAILED;
} /* end of cmHashListFind */


/*
*
*       Fun:   cmHashListGetNext
*
*       Desc:  Gets next entry in hash list with respect to the specified
*              previous entry. If previous entry is NULLP, gets first
*              entry in hash list. Parameters are:
*
*              hashListCp   control point for hash list
*              prevEnt      pointer to previous entry
*              entry        pointer to next entry to be returned
*
*       Ret:   ROK      - get successful, *entry points to found entry
*                         (at beginning of list or in the list)
*              RFAILED  - get failed, *entry is unchanged 
*                         (incorrect parameter values)
*              ROKDNA   - get failed, *entry is unchanged.
*                         (end of list)
*
*       Notes:  This function has to be used cautiously while the 
*               CM Hash Module is being compiled with CM_MT_HASH_BIN.
*               In such cases, this function should be used only after
*               ensuring that none of the other threads are operating
*               on the common hash list.
*
*       File:  cm_hash.c
*
*/
S16 cmHashListGetNext
(
CmHashListCp *hashListCp,    /* hash list to get from */
PTR          prevEnt,        /* previous entry */
PTR          *entry          /* entry to be returned */
)
{
#ifndef CM_MT_HASH_BIN
   CmListEnt     *hashListBin;   /* temporary hash list bin pointer */
#else
   CmListBinEnt  *hashListBin;   /* temporary hash list bin pointer */
#endif
   CmHashListEnt *hashListEnt;   /* temporary hash list entry pointer */
   CmHashListEnt *prevListEnt;   /* previous hash list entry pointer */
   uint16_t           i;              /* hash list counter */


#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if ((hashListCp == NULLP) || (entry == NULLP))
      return RFAILED;
#endif

   /* check if need to get first entry */
   if (prevEnt == NULLP)
   {
      /* get first entry in hash list */
      for (i = 0; i < hashListCp->nmbBins; i++)
         /* check for non-empty bin */
#ifndef CM_MT_HASH_BIN
         if (hashListCp->hl[i].next != &hashListCp->hl[i])
#else
         if (hashListCp->hl[i].next != (CmListEnt*)&hashListCp->hl[i])
#endif
         {
            /* get first entry in bin */
            hashListEnt = (CmHashListEnt *) hashListCp->hl[i].next;

            /* requested entry is in nxtEnt */
            *entry = (PTR) (((uint8_t *) hashListEnt) - hashListCp->offset);

            return ROK;
         }

      /* no more entries */
      return (ROKDNA);
   }

   /* use previous entry to find next entry */

   /* get pointer to previous hash list entry header */
   prevListEnt = (CmHashListEnt *) (((uint8_t *) prevEnt) + hashListCp->offset);

   /* get index of previous entry */
   i = prevListEnt->hashVal;

   /* set pointers to get next entry */
   hashListBin = &hashListCp->hl[i];
   prevListEnt = (CmHashListEnt *) prevListEnt->list.next;
   for (;;)
   {
      /* check if more entries in this bin */
      if (prevListEnt != (CmHashListEnt *) hashListBin)
      {
         /* found next entry */
         *entry = (PTR) (((uint8_t *) prevListEnt) - hashListCp->offset);

         return ROK;
      }

      /* no more entries in this bin, go to next bin, check if more bins */
      if (++i >= hashListCp->nmbBins)
         /* no more entries */
         break;

      /* reset pointers for next bin */
      hashListBin = &hashListCp->hl[i];
      prevListEnt = (CmHashListEnt *) hashListBin->next;
   }

   /* no more entries */
   return (ROKDNA);
} /* end of cmHashListGetNext */

#ifdef CM_MT_HASH_BIN

/*
*
*       Fun:   cmHashListBinGetNextEntry
*
*       Desc:  Gets next entry in  a given hash bin respect to the specified
*              previous entry. If previous entry is NULLP, gets first
*              entry in hash bin. Parameters are:
*
*              hashListCp   control point for hash list
*              binIdx       Bin Index to find the entry in
*              prevEnt      pointer to previous entry
*              entry        pointer to next entry to be returned
*
*       Ret:   ROK      - get successful, *entry points to found entry
*                         (at beginning of list or in the list)
*              RFAILED  - get failed, *entry is unchanged 
*                         (incorrect parameter values)
*              ROKDNA   - get failed, *entry is unchanged.
*                         (end of list)
*       Notes:  None.
*
*       File:  cm_hash.c
*
*/
S16 cmHashListBinGetNextEntry
(
CmHashListCp *hashListCp,    /* hash list to get from */
uint16_t     binIdx,         /* Bin Index to retreive the entry */
PTR          prevEnt,        /* previous entry */
PTR          *entry          /* entry to be returned */
)
{
   CmListBinEnt  *hashListBin;   /* temporary hash list bin pointer */
   CmHashListEnt *hashListEnt;   /* temporary hash list entry pointer */
   CmHashListEnt *prevListEnt;   /* previous hash list entry pointer */


#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if ((hashListCp == NULLP) || (entry == NULLP))
      return RFAILED;
#endif

   /* check if need to get first entry */
   if (prevEnt == NULLP)
   {
      /* get first entry in hash list */
      /* check for non-empty bin */
      if (hashListCp->hl[binIdx].next != (CmListEnt*)&hashListCp->hl[binIdx])
      {
         /* get first entry in bin */
         hashListEnt = (CmHashListEnt *) hashListCp->hl[binIdx].next;

         /* requested entry is in nxtEnt */
         *entry = (PTR) (((uint8_t *) hashListEnt) - hashListCp->offset);

         return ROK;
      }

      /* no more entries */
      return (ROKDNA);
   }

   /* use previous entry to find next entry */

   /* get pointer to previous hash list entry header */
   prevListEnt = (CmHashListEnt *) (((uint8_t *) prevEnt) + hashListCp->offset);

   /* set pointers to get next entry */
   hashListBin = &hashListCp->hl[binIdx];
   prevListEnt = (CmHashListEnt *) prevListEnt->list.next;

   /* check if more entries in this bin */
   if (prevListEnt != (CmHashListEnt *) hashListBin)
   {
      /* found next entry */
      *entry = (PTR) (((uint8_t *) prevListEnt) - hashListCp->offset);

      return ROK;
   }

   /* no more entries */
   return (ROKDNA);
} /* end of cmHashListBinGetNextEntry */
#endif


/*
*
*       Fun:   cmHashListQuery
*
*       Desc:  Gets hash list attributes.  Parameters are:
*
*              hashListCp   control point for hash list
*              queryType    type of attribute being queried
*              result       result of query, to be returned
*
*       Ret:   ROK      - successful, *result contains query result
*              RFAILED  - failed, *result unchanged (incorrect parameter values)
*
*       Notes: This function is obsoleted! 
*              Use macros defined in cm_hash.h instead
*
*       File:  cm_hash.c
*
*/
S16 cmHashListQuery
(
CmHashListCp *hashListCp,    /* hash list to query */
uint8_t      queryType,      /* type of query */
uint16_t     *result         /* result of query */
)
{
#ifdef CM_MT_HASH_BIN
   uint8_t       i;
#endif


   /* deal with queries that do not need hashListCp */

#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if (result == NULLP)
      return RFAILED;
#endif

   /* respond depending on query type */
   if (queryType == CM_HASH_QUERYTYPE_BINSIZE)
   {
      /* storage for each bin */
#ifndef CM_MT_HASH_BIN
      *result = (uint16_t) sizeof(CmListEnt);
#else
      *result = (uint16_t) sizeof(CmListBinEnt);
#endif
      return ROK;
   }

   /* deal with queries that do need hashListCp */

#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if (hashListCp == NULLP)
      return RFAILED;
#endif

   /* respond depending on query type */
   switch (queryType)
   {
      case CM_HASH_QUERYTYPE_ENTRIES:   /* current number of entries */
#ifndef CM_MT_HASH_BIN
         *result = (uint16_t) hashListCp->nmbEnt;
#else
         *result = 0;
         for (i=0; i < hashListCp->nmbBins; i++)
         {
            *result += hashListCp->hl[i].nmbEnt;
         }
#endif
         return ROK;

      case CM_HASH_QUERYTYPE_BINS:      /* number of bins */
         *result = (uint16_t) hashListCp->nmbBins;
         return ROK;

      case CM_HASH_QUERYTYPE_OFFSET:    /* offset of CmHashListEnt in entries */
         *result = (uint16_t) hashListCp->offset;
         return ROK;

      case CM_HASH_QUERYTYPE_DUPFLG:    /* allow duplicate keys */
         *result = (uint16_t) hashListCp->dupFlg;
         return ROK;

      case CM_HASH_QUERYTYPE_KEYTYPE:   /* key type for selecting hash functions */
         *result = (uint16_t) hashListCp->keyType;
         return ROK;

      default:                          /* process other query types */
         break;
   }

   /* illegal query type */
   return RFAILED;
} /* end of cmHashListQuery */

#ifdef HASH_OPEN_ADDRESSING
  
/*
*
*       Fun:   cmHashListOAInsert
*
*       Desc:  Inserts a new entry in the hash list with open addressing.
*              Parameters are: 
*
*              hashListCp   control point for hash list
*              entry        pointer to new entry to add in the hash list
*              key          pointer to key string in the new entry
*              keyLen       length of key string
*
*       Ret:   ROK      - insertion successful
*              ROKDUP   - insertion failed (duplicate key not allowed)
*              RFAILED  - insertion failed (incorrect parameter values)
*
*       Notes: None
*
*       File:  cm_hash.c
*
*/

S16 cmHashListOAInsert
(
CmHashListCp *hashListCp,  /* hash table to add to */
PTR          entry,        /* entry to add */
uint8_t      *key,         /* pointer to key */
uint16_t     keyLen        /* length of key */
)
{
/* cm_hash_c_001.main_21. Modify. Compilation Issue resolved. */
#ifndef CM_MT_HASH_BIN
   CmListEnt     *hashBin;        /* temporary hash list bin pointer */
#else
   CmListBinEnt  *hashBin;        /* temporary hash list bin pointer */
#endif
   CmHashListEnt *hashListEnt;    /* pointer to hash list entry header */
   uint16_t idx;                       /* index for insertion into hash list */
   uint16_t hashSize;                  /* hash size */
   uint16_t i;
   /* cm_hash_c_001.main_21. Modify. Compilation Issue resolved. */
   uint16_t nmbEnt;


#if (ERRCLASS & ERRCLS_DEBUG)
   /* error check on parameters */
   if ((hashListCp == NULLP) || (entry == NULLP) || 
       (key == NULLP) || (keyLen == 0))
      return RFAILED;
#endif

#ifndef CM_MT_HASH_BIN
   nmbEnt = hashListCp->nmbEnt;
#else
   nmbEnt = 0; 
   for (i=0; i < hashListCp->nmbBins; i++)
   {
      nmbEnt += hashListCp->hl[i].nmbEnt;
   }
#endif
   /* check if table is full */
   if (hashListCp->nmbBins == nmbEnt)
      return (ROUTRES);

   /* get pointer to hash list entry header */
   hashListEnt = (CmHashListEnt *) (((uint8_t *) entry) + hashListCp->offset);

   /* initialize hash list entry header */
   hashListEnt->list.next = NULLP;
   hashListEnt->list.prev = NULLP;
   hashListEnt->keyLen    = keyLen;
   hashListEnt->key       = key;

   /* compute index for insertion */
   if ((*hashListCp->hashFunc)(hashListCp, key, keyLen, &idx) != ROK)
      return RFAILED;

   /*
    *  find an empty bin
    */
   hashSize = hashListCp->nmbBins;
   hashBin = &hashListCp->hl[idx];
   for (i = hashSize; i > 0; i--)
   {
      if (hashBin->next == hashBin)
         break;                            /* found */
      if (++idx >= hashSize)
      {
         idx = 0;
         hashBin = &hashListCp->hl[0];
      }
      else
         hashBin++;
   }

   /* insert into list */
   if (cmListInsert(hashBin->prev, &hashListEnt->list) != ROK)
      return RFAILED;

   hashListEnt->hashVal   = idx;

#ifndef CM_MT_HASH_BIN
   /* increment count of entries in hash list */
   hashListCp->nmbEnt++;
#else
   hashBin->nmbEnt++;
#endif

   return ROK;
} /* end of cmHashListOAInsert */


#endif /* HASH_OPENADDRESSING */

/**********************************************************************
         End of file
**********************************************************************/
