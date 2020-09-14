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
  
        Name:    LTE-RLC Layer 
    
        Type:     C source file
  
        Desc:     <description of file contents>
 
        File:     kw_id.c

*********************************************************************21*/
 

  
/* header include files (.h) */
  
#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */

/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */

  
/* local defines */

#define KWSWMV 2             /* rlc - main version */
#define KWSWMR 1             /* rlc - main revision */
#define KWSWBV 0             /* rlc - branch version */
#define KWSWBR 5             /* rlc -  kw005.201 patch level */
#define KWSWPN "1000371"     /* rlc - part number */

/* local typedefs */

/* local externs */
  
/* forward references */

/* public variable declarations */

#ifdef __cplusplus
EXTERN "C" {
#endif /* CPLUSPLUS */

EXTERN S16 kwGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* CPLUSPLUS */

/* copyright banner */

PUBLIC CONSTANT Txt kwBan1[] =
   {"(c) COPYRIGHT 1989-2002, Trillium Digital Systems, Inc."};

PUBLIC CONSTANT Txt kwBan2[] =
   {"                 All rights reserved."};

/* system id */

PRIVATE CONSTANT SystemId sId ={
   KWSWMV,              /* rlc - main version */
   KWSWMR,              /* rlc - main revision */
   KWSWBV,              /* rlc - branch version */
   KWSWBR,              /* rlc - branch revision */
   KWSWPN               /* rlc - part number */
};

/*
 *     support functions
 */

/*
 *
 *       Fun:   get system id
 *
 *       Desc:  Get system id consisting of part number, main version and
 *              revision and branch version and branch.
 *
 *       Ret:   TRUE      - ok
 *
 *       Notes: None
 *
 *       File:  kw_id.c
 *
 */

#ifdef ANSI
   PUBLIC S16 kwGetSId
(
 SystemId *s                 /* system id */
 )
#else
PUBLIC S16 kwGetSId(s)
   SystemId *s;                /* system id */
#endif
{
   TRC2(kwGetSId);

   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   s->ptNmb = sId.ptNmb;

   RETVALUE(TRUE);

} /* end of kwGetSId */

  
/********************************************************************30**

  End of file
 **********************************************************************/
