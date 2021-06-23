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
#include "phy_stub_utils.h"
#ifdef INTEL_FAPI
#include "fapi.h"
#include "fapi_vendor_extension.h"
#endif
#include "phy_stub.h"

/*******************************************************************
*
* @brief Initializes Phy stub slot indication generator task
*
* @details
*
*    Function : phyStubActvInit
*
*    Functionality:
*       - Initializes Phy stub slot indication generator task
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t phyStubActvInit(Ent ent, Inst inst, Region reg, Reason reason)
{
   slotIndicationStarted = false;
   sfnValue = 0;
   slotValue = 0;

   return ROK;
}


/*******************************************************************
*
* @brief Receives messages for Phy stub slot indication generator task
*
* @details
*
*    Function : phyStubActvTsk
*
*    Functionality:
*       - Receives messages for Phy stub slot indication generator task
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t phyStubActvTsk(Pst *pst, Buffer *mBuf)
{
   DU_LOG("\nPHY_STUB: Received Event [%d]", pst->event);
   switch(pst->srcEnt)
   {
      case ENTPHYSTUB:
      {
         switch(pst->event)
         {
            case EVT_PHY_START_SLOT_IND:
            {
               ODU_PUT_MSG_BUF(mBuf);
               GenerateTicks();
            }
         }
      }
   }
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
