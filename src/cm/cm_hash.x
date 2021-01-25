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

     Desc:     Structures, variables and typedefs required by common
               functions.
               (Newer version of functions in cm_bdy1)

     File:     cm_hash.x

*********************************************************************21*/

#ifndef __CMHASHX__
#define __CMHASHX__

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

/* forward definitions */

typedef struct cmHashListCp CmHashListCp;     /*  hash list control point */


/* hash function */

typedef S16 (* CmHashFunc) ARGS((CmHashListCp *hashListCp, uint8_t *key, 
                                  uint16_t keyLen, uint16_t *idx));

/* list entry */
typedef struct cmListEnt CmListEnt;      /* list entry */

struct cmListEnt                         /* list entry */
{
   CmListEnt *next;                /* next entry in list */
   CmListEnt *prev;                /* prev entry in list */
};

#ifdef CM_MT_HASH_BIN
typedef struct cmListBinEnt CmListBinEnt;/* Bin entry */

struct cmListBinEnt                /* Bin entry */
{
   CmListEnt *next;                /* next entry in list */
   CmListEnt *prev;                /* prev entry in list */
   uint16_t nmbEnt;                     /* current number of entries */
};
#endif


/* hash list entry */
   
typedef struct cmHashListEnt       /* hash list entry */
{
   CmListEnt list;                 /* list pointers */
   uint8_t  *key;                       /* pointer to key */
   uint16_t keyLen;                     /* length of key */
   uint16_t hashVal;                    /* computed hash value */
} CmHashListEnt;

/* hash list control point */

struct cmHashListCp        /*  hash list control point */
{
#ifndef CM_MT_HASH_BIN
   CmListEnt *hl;                  /* pointer to hash list bins */
#else
   CmListBinEnt *hl;               /* pointer to hash list bins */
#endif
   Region region;                  /* memory region to allocate bins */
   Pool pool;                      /* memory pool to allocate bins */
   uint16_t nmbBins;                    /* number of hash list bins */
   uint16_t binBitMask;                 /* number of bits if nmbBins is power of 2 */
   uint8_t  nmbBinBits;                 /* number of bits to represent nmbBins */
#ifndef CM_MT_HASH_BIN
   uint16_t nmbEnt;                     /* current number of entries */
#endif
   uint16_t offset;                     /* offset of CmHashListEnt in entries */
   Bool dupFlg;                    /* allow duplicate keys */
   uint16_t  keyType;                   /* key type for selecting hash functions */
   CmHashFunc hashFunc;            /* hash function for this key type */
};


/* functions prototypes */

S16 cmHashListInit ARGS((
              CmHashListCp *hashListCp,  /* hash list to initialize */
              uint16_t          nmbBins,      /* number of hash list bins */
              uint16_t          offset,       /* offset of CmHashListEnt in entries */
              Bool         dupFlg,       /* allow duplicate keys */
              uint16_t          keyType,      /* key type for selecting hash fn */
              Region       region,       /* memory region to allocate bins */
              Pool         pool));       /* memory pool to allocate bins */

S16 cmHashListDeinit ARGS((
              CmHashListCp *hashListCp));/* hash list to initialize */

S16 cmHashListInsert ARGS((
              CmHashListCp *hashListCp,  /* hash list to add to */
              PTR          entry,        /* entry to add */
              uint8_t           *key,         /* pointer to key */
              uint16_t          keyLen));     /* length of key */

S16 cmHashListDelete ARGS((
              CmHashListCp *hashListCp,  /* hash list to delete from */
              PTR          entry));      /* entry to delete */

S16 cmHashListFind ARGS((
              CmHashListCp *hashListCp,  /* hash list to search */
              uint8_t           *key,         /* pointer to key */
              uint16_t          keyLen,       /* length of key */
              uint16_t          seqNmb,       /* used in case of duplicate keys */
              PTR          *entry));     /* entry to be returned */

S16 cmHashListGetNext ARGS((
              CmHashListCp *hashListCp,    /* hash list to get from */
              PTR          prevEnt,        /* previous entry */
              PTR          *entry));       /* entry to be returned */

#ifdef CM_MT_HASH_BIN
S16 cmHashListBinGetNextEntry ARGS((
              CmHashListCp *hashListCp,    /* hash list to get from */
              uint16_t          binIdx,         /* Index of the bin */
              PTR          prevEnt,        /* previous entry */
              PTR          *entry));       /* entry to be returned */
#endif

/* This function is obsoleted! Use macros defined in cm_hash.h instead
 */
S16 cmHashListQuery ARGS((
              CmHashListCp *hashListCp,    /* hash list to query */
              uint8_t           queryType,      /* type of query */
              uint16_t          *result));      /* result of query */

/* Hash list with open addressing
 */
S16 cmHashListOAInsert ARGS((
              CmHashListCp *hashListCp,  /* hash list to add to */
              PTR          entry,        /* entry to add */
              uint8_t           *key,         /* pointer to key */
              uint16_t          keyLen));     /* length of key */

#ifdef __cplusplus
}
#endif

#endif /* __CMHASHX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
