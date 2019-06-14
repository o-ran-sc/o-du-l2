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

typedef S16 (* CmHashFunc) ARGS((CmHashListCp *hashListCp, U8 *key, 
                                  U16 keyLen, U16 *idx));

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
   U16 nmbEnt;                     /* current number of entries */
};
#endif


/* hash list entry */
   
typedef struct cmHashListEnt       /* hash list entry */
{
   CmListEnt list;                 /* list pointers */
   U8  *key;                       /* pointer to key */
   U16 keyLen;                     /* length of key */
   U16 hashVal;                    /* computed hash value */
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
   U16 nmbBins;                    /* number of hash list bins */
   U16 binBitMask;                 /* number of bits if nmbBins is power of 2 */
   U8  nmbBinBits;                 /* number of bits to represent nmbBins */
#ifndef CM_MT_HASH_BIN
   U16 nmbEnt;                     /* current number of entries */
#endif
   U16 offset;                     /* offset of CmHashListEnt in entries */
   Bool dupFlg;                    /* allow duplicate keys */
   U16  keyType;                   /* key type for selecting hash functions */
   CmHashFunc hashFunc;            /* hash function for this key type */
};


/* functions prototypes */

EXTERN S16 cmHashListInit ARGS((
              CmHashListCp *hashListCp,  /* hash list to initialize */
              U16          nmbBins,      /* number of hash list bins */
              U16          offset,       /* offset of CmHashListEnt in entries */
              Bool         dupFlg,       /* allow duplicate keys */
              U16          keyType,      /* key type for selecting hash fn */
              Region       region,       /* memory region to allocate bins */
              Pool         pool));       /* memory pool to allocate bins */

EXTERN S16 cmHashListDeinit ARGS((
              CmHashListCp *hashListCp));/* hash list to initialize */

EXTERN S16 cmHashListInsert ARGS((
              CmHashListCp *hashListCp,  /* hash list to add to */
              PTR          entry,        /* entry to add */
              U8           *key,         /* pointer to key */
              U16          keyLen));     /* length of key */

EXTERN S16 cmHashListDelete ARGS((
              CmHashListCp *hashListCp,  /* hash list to delete from */
              PTR          entry));      /* entry to delete */

EXTERN S16 cmHashListFind ARGS((
              CmHashListCp *hashListCp,  /* hash list to search */
              U8           *key,         /* pointer to key */
              U16          keyLen,       /* length of key */
              U16          seqNmb,       /* used in case of duplicate keys */
              PTR          *entry));     /* entry to be returned */

EXTERN S16 cmHashListGetNext ARGS((
              CmHashListCp *hashListCp,    /* hash list to get from */
              PTR          prevEnt,        /* previous entry */
              PTR          *entry));       /* entry to be returned */

#ifdef CM_MT_HASH_BIN
EXTERN S16 cmHashListBinGetNextEntry ARGS((
              CmHashListCp *hashListCp,    /* hash list to get from */
              U16          binIdx,         /* Index of the bin */
              PTR          prevEnt,        /* previous entry */
              PTR          *entry));       /* entry to be returned */
#endif

/* This function is obsoleted! Use macros defined in cm_hash.h instead
 */
EXTERN S16 cmHashListQuery ARGS((
              CmHashListCp *hashListCp,    /* hash list to query */
              U8           queryType,      /* type of query */
              U16          *result));      /* result of query */

/* Hash list with open addressing
 */
EXTERN S16 cmHashListOAInsert ARGS((
              CmHashListCp *hashListCp,  /* hash list to add to */
              PTR          entry,        /* entry to add */
              U8           *key,         /* pointer to key */
              U16          keyLen));     /* length of key */

#ifdef __cplusplus
}
#endif

#endif /* __CMHASHX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
