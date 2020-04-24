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

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "cm_mblk.h"        /* Common LTE Defines */
#include "tfu.h"           /* RGU Interface defines */
//#include "rg.h"
#ifdef FAPI
#include "fapi.h"
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
#include "cm_mblk.x"        /* Common LTE Defines */
#include "tfu.x"           /* RGU Interface includes */
//#include "rg.x"
#include "lwr_mac.h"
#include "du_log.h"

#ifdef FAPI
/* function pointers for packing macCellCfg Request */
typedef S16 (*packSlotIndMsg) ARGS((
   Pst           *pst,
   fapi_slot_ind_t *slotInd
));

S16 packLcSlotInd (Pst *pst, SlotIndInfo *slotInd);
S16 packTcSlotInd (Pst *pst, fapi_slot_ind_t *slotInd);
S16 packLwlcSlotInd (Pst *pst, fapi_slot_ind_t *slotInd);

packSlotIndMsg packSlotIndOpts[] =
{
   packLcSlotInd, /* packing for loosely coupled */
   fapiMacSlotInd, /* packing for tightly coupled */
   packLwlcSlotInd, /* packing for light weight loosly coupled */
};

S16 packLcSlotInd (Pst *pst, SlotIndInfo *slotInd)
{
   Buffer *mBuf = NULLP;
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* pack SFN and slot value */
   CMCHKPK(SPkU16,slotInd->sfn, mBuf);
   CMCHKPK(SPkU16,slotInd->slot, mBuf);

   RETVALUE(SPstTsk(pst,mBuf));
}

S16 packTcSlotInd (Pst *pst, fapi_slot_ind_t *slotInd)
{
   return ROK;
}

S16 packLwlcSlotInd (Pst *pst, fapi_slot_ind_t *slotInd)
{
   return ROK;
}

U16 handleSlotInd(fapi_slot_ind_t *fapiSlotInd)
{
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;
   SlotIndInfo slotInd;

   pst.srcProcId = 0;
   pst.dstProcId = 0;
   pst.srcEnt = ENTTF;
   pst.dstEnt = ENTRG;
   pst.srcInst = 0;
   pst.dstInst = 0;
   pst.event = EVENT_SLOT_IND_TO_MAC;
   pst.region = 0;
   pst.pool =  0;
   pst.selector = MAC_SELECTOR_TC;

   slotInd.sfn = fapiSlotInd->sfn;
   slotInd.slot = fapiSlotInd->slot;

   return (*packSlotIndOpts[pst.selector])(&pst, &slotInd);
}
#endif

void handlePhyMessages(void *msg)
{
#ifdef FAPI
   /* extract the header */
   fapi_msg_t *header;
   header = (fapi_msg_t *)msg;

   switch(header->message_type_id)
   {
      case FAPI_SLOT_INDICATION:
      {
         if(clGlobalCp.phyState == PHY_STATE_CONFIGURED)
         {
            DU_LOG("\nLOWER MAC: PHY has moved to running state \n");
            clGlobalCp.phyState = PHY_STATE_RUNNING;
         }

         fapi_slot_ind_t *slotInd;
         slotInd  = (fapi_slot_ind_t *)msg;
         handleSlotInd(slotInd);
         break;
      }
      case FAPI_ERROR_INDICATION:
      {
         break;
      }
      case FAPI_RX_DATA_INDICATION:
      {
         break;
      }  
      case FAPI_CRC_INDICATION:
      {
         break;
      }  
      case FAPI_UCI_INDICATION:
      {
         break;
      }  
      case FAPI_SRS_INDICATION:
      {
         break;
      }  
      case FAPI_RACH_INDICATION:
      {
         break;
      }  
   }
#endif
}

/**********************************************************************
         End of file
**********************************************************************/
