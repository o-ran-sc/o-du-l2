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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_utl.c
  
**********************************************************************/

/** @file rg_utl.c
@brief This file implements utility functions for LTE MAC
*/


/* header include files (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"    /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */
#include  "mac_utils.h"

#include "rg_prg.h"        /* PRG(MAC-MAC) Interface includes */
#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "cm5.x"           /* Timer */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"    /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */
#include "rg_prg.x"        /* PRG(MAC-MAC) Interface includes */

#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC includes */

/* local defines */
#define RG_NON_MIMO_IDX 0

/***********************************************************
 *
 *     Func : rgAllocSBuf
 *
 *     Desc : Utility Function to Allocate static buffer. 
 *            Memory allocated is assumed contiguous.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
S16 rgAllocSBuf
(
Inst    inst,
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      return RFAILED;
   }

   /* allocate buffer */
#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   MAC_ALLOC(pData, size);
   if(pData == NULLP)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb[inst].rgInit.region;
     dgn.u.mem.pool = rgCb[inst].rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(inst,LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      DU_LOG("\nERROR  -->  MAC : Unable to Allocate Buffer");
      return RFAILED;
   }

   /* zero out the allocated memory */
   memset(*pData, 0x00, size);

   return ROK;

} /* end of rgAllocSBuf */

/*
*
*       Fun:   rgFreeSharableSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   void
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
Void rgFreeSharableSBuf
(
Inst inst,
Data **data,         /* address of pointer to data */
Size size            /* size */
)
{

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      return;
   }

   /* Deallocate buffer */
   MAC_FREE_SHRABL_BUF(rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool, *data, size);
   
   if (data != NULLP)
   {
      return;
   }

   *data = NULLP;

   return;

} /* end of rgFreeSharableBuf */



/*
*
*       Fun:   rgFreeSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   void
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
Void rgFreeSBuf
(
Inst inst,
Data **data,         /* address of pointer to data */
Size size            /* size */
)
{

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      return;
   }


#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_CALLER();
#endif /* */
   /* Deallocate buffer */
   MAC_FREE(data, size);

   if (data != NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : rgFreeSBuf failed.\n");
      return;
   }

   *data = NULLP;

   return;

} /* end of rgFreeSBuf */


/***********************************************************
 *
 *     Func : rgFillDgnParams
 *
 *     Desc : Utility Function to Fill Diagonostic params. 
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
Void rgFillDgnParams
(
Inst        inst,
RgUstaDgn   *dgn,
uint8_t     dgnType
)
{

   switch(dgnType)
   {
      case LRG_USTA_DGNVAL_MEM:
         dgn->type = (uint8_t) LRG_USTA_DGNVAL_MEM;
         dgn->u.mem.region  = rgCb[inst].rgInit.region;
         dgn->u.mem.pool    = rgCb[inst].rgInit.pool;
      break;

      default:
      break;
   }

   return;
} /* end of rgFillDgnParams */


/**********************************************************************
 
         End of file
**********************************************************************/
