
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
#include "common_def.h"
#include "tfu.h"
#include "lrg.h"
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"
#include "sch_drx.h"

#ifdef NR_DRX
/**
 * @brief delete UE drx timers and information 
 *
 * @details
 *
 *     Function : schDeleteUeDrxInfo
 *      
 *     delete UE drx timers and information 
 *           
 *  @param[in] SchCellCb  *cell, SchUeCb *ueCb 
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
void schInitDrxUeCb(SchUeCb *ueCb)
{
   memset(&ueCb->drxUeCb, 0, sizeof(SchDrxUeCb));
   ueCb->drxUeCb.shortCycleDistance = -1;
   ueCb->drxUeCb.onDurationStartDistance = -1;
   ueCb->drxUeCb.onDurationExpiryDistance = -1;
   ueCb->drxUeCb.inActiveTmrExpiryDistance = -1;
}
/**
 * @brief delete UE drx timers and information 
 *
 * @details
 *
 *     Function : schDeleteUeDrxInfo
 *      
 *     delete UE drx timers and information 
 *           
 *  @param[in] SchCellCb  *cell, SchUeCb *ueCb 
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
void schDeleteUeDrxInfo(SchCellCb  *cell, SchUeCb *ueCb)
{
   uint16_t tmrIdx = 0;
   SchDrxUeCb *drxUeCb;
   CmLList  *node = NULLP;

   if(ueCb->ueDrxInfoPres == true)
   {
      drxUeCb = &ueCb->drxUeCb;
      
      /* delete on duration start timer from ueCb */
      tmrIdx = drxUeCb->onDurationStartIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationStartList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationStartList, node);
         SCH_FREE(node, sizeof(CmLList));
      }
      
      /* delete on duration expiry timer from ueCb */
      tmrIdx = drxUeCb->onDurationExpiryIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationExpiryList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationExpiryList, node);
         SCH_FREE(node, sizeof(CmLList));
      }
      
      /* delete inActv Expiry Index timer from ueCb */
      tmrIdx = drxUeCb->inActvExpiryIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].inActvTmrExpiryList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].inActvTmrExpiryList, node);
         SCH_FREE(node, sizeof(CmLList));
      }
      
      /* delete short cycle expiry timer from ueCb */
      tmrIdx = drxUeCb->shortCycleExpiryIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].shortCycleExpiryList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].shortCycleExpiryList, node);
         SCH_FREE(node, sizeof(CmLList));
      }
      schInitDrxUeCb(ueCb);
   }
}
#endif
/**********************************************************************
  End of file
 **********************************************************************/
