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
  
     Type:     C include file
  
     Desc:     Defines required by common functions.
               (Newer version of functions in cm_bdy1)
  
     File:     cm_hash.h

*********************************************************************21*/
  
#ifndef __CMHASHH__
#define __CMHASHH__ 

  
/* defines */

/* key types */

#define CM_HASH_KEYTYPE_DEF         0  /* default key type - selects 
                                          Trillium supplied default function */
#define CM_HASH_KEYTYPE_MULT24      1  /* key type which uses multiplication 
                                          method to compute hash list index 
                                          - function supplied by Trillium */
#define CM_HASH_KEYTYPE_DIRIDX      2  /* direct indexing of hash tables */

#define CM_HASH_KEYTYPE_STR         3  /* Hash Function for Strings */

#define CM_HASH_KEYTYPE_UINT32_MOD      4  /* Mods the key with number of bins
                                        * useful if key is uint32_t numeric */

#define CM_HASH_KEYTYPE_CONID       5  /* Uses diff computation for keylen
                                         < uint32_t. Ideal fo conId type params */

#define CM_HASH_KEYTYPE_BCD8        6  /* Converts the 8 BCD coded octets 
                                        * into 2 uint32_ts and then adds 2 
                                        * uint32_ts to get one uint32_t. Then applies the 
                                        * uint32_tMod technique to get the index */
#define CM_HASH_KEYTYPE_ANY         7  /* Converts a variable length key into
                                        * a uint32_t which is then mapped to number
                                        * of hash bins
                                        */

/* Constants */
#define CM_STR_HASHFUNC_CONSTANT    31 /* Constant for CM_HASH_KEYTYPE_STR */

/* constS for CmHashFuncConId */
/* cm_hash_h_001.main_13 : Fixed for 64 Bit */
#define CM_HASHKEYLEN_UINT32              sizeof(uint32_t)  /* size of uint32_t */
#define CM_HASHKEYLEN_UINT16              sizeof(uint16_t)  /* size of uint16_t */
#define CM_HASHKEYLEN_UINT8               sizeof(uint8_t)  /* size of uint8_t */

/* query types */

#define CM_HASH_QUERYTYPE_BINS     1  /* number of bins */
#define CM_HASH_QUERYTYPE_BINSIZE  2  /* storage for each bin */
#define CM_HASH_QUERYTYPE_ENTRIES  3  /* current number of entries */
#define CM_HASH_QUERYTYPE_OFFSET   4  /* offset of CmHashListEnt in entries */
#define CM_HASH_QUERYTYPE_DUPFLG   5  /* allow duplicate keys */
#define CM_HASH_QUERYTYPE_KEYTYPE  6  /* key type for selecting hash functions */

#define CM_HASH_VALUE(entry)       (entry)->hashVal  /* computed hash value */
#define CM_HASH_SIZE(tbl)          (tbl)->nmbBins    /* hash table size */

#ifndef CM_MT_HASH_BIN
#define CM_HASH_NMBENT(tbl)        (tbl)->nmbEnt     /* number of entries in
                                                      * table
                                                      */
#endif

#define CM_HASH_DUPFLG(tbl)        (tbl)->dupFlg     /* allow duplicate keys */

#define CM_HASH_OFFSET(tbl)        (tbl)->offset     /* offset of CmHashListEnt
                                                      * structure in hash list
                                                      * entry
                                                      */

#define CM_HASH_KEYTYPE(tbl)       (tbl)->keyType    /* key type for selecting
                                                      * hash function
                                                      */

#define CM_HASH_BINSIZE       sizeof(CmListEnt) /* size of a single bin
                                                 */
/* bin bit mask */

#define CM_HASH_NOBITMASK          0x8000  /* illegal bin bit mask */

/* constant multiplier for multiplication method of computing hash index */
#define CM_HASH_MULT24_CONST       10368890   /* when key is of max 24 bits */

/* bit position where the hash index is extracted in multiplication method */
#define CM_HASH_MULT24_BITPOS        24       /* when key is of max 24 bits */

/*
 *  delete an entry from the hash table with open addressing
 */
#define cmHashListOADelete(hashListCp, entry) cmHashListDelete(hashListCp, (PTR)entry)


/* 
 * CM_HASH_MIX -- mix 3 32-bit values reversibly.
 * For every delta with one or two bits set, and the deltas of all three
 * high bits or all three low bits, whether the original value of a,b,c
 * is almost all zero or is uniformly distributed,
 * If CM_HASH_MIX() is run forward or backward, at least 32 bits in a,b,c
 * have at least 1/4 probability of changing.
 * If CM_HASH_MIX() is run forward, every bit of c will change between 1/3 and
 * 2/3 of the time.  (Well, 22/100 and 78/100 for some 2-bit deltas.)
 * CM_HASH_MIX() was built out of 36 single-cycle latency instructions in a 
 * structure that could supported 2x parallelism, like so:
 *     a -= b; 
 *     a -= c; x = (c>>13);
 *     b -= c; a ^= x;
 *     b -= a; x = (a<<8);
 *     c -= a; b ^= x;
 *     c -= b; x = (b>>13);
 *     ...
 */
#define CM_HASH_MIX(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8);  \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12); \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5);  \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

#endif /* __CMHASHH__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
