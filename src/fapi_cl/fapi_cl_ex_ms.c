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
#include "fapi_cl.h"

/*******************************************************************
*
* @brief Initializes FAPI CL receiver task
*
* @details
*
*    Function : fapiClRecvrActvInit
*
*    Functionality:
*       - Initializes FAPI CL Receiver task
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t fapiClRecvrActvInit(Ent ent, Inst inst, Region reg, Reason reason)
{
   memset(&clCb, 0, sizeof(ClCb));
   return ROK;
}

/*******************************************************************
*
* @brief Receives messages for FAPI CL REceiver task
*
* @details
*
*    Function : fapiClRecvrActvTsk
*
*    Functionality:
*       - Receives messages for fapi cl receiver task
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t fapiClRecvrActvTsk(Pst *pst, Buffer *mBuf)
{
   DU_LOG("\nINFO  -->  FAPI_CL: Received Event [%d]", pst->event);

   switch(pst->srcEnt)
   {
      case ENTFAPICL:
      {
         switch(pst->event)
         {
            case EVT_FAPI_CL_START_RECEIVER:
            {
               ODU_PUT_MSG_BUF(mBuf);
               recvMsgFromUeSim();
               break;
            }
            default:
            DU_LOG("\nERROR  -->  FAPI_CL: Invalid Event");
         }
      }
   }
   return ROK;
}

/*******************************************************************
*
* @brief Initializes FAPI CL slot indication generator task
*
* @details
*
*    Function : fapiClSlotIndActvInit
*
*    Functionality:
*       - Initializes FAPI CL slot indication generator task
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t fapiClSlotIndActvInit(Ent ent, Inst inst, Region reg, Reason reason)
{
   memset(&clCb, 0, sizeof(ClCb));
   return ROK;
}

/*******************************************************************
*
* @brief Receives messages for FAPI CL slot indication generator task
*
* @details
*
*    Function : fapiClSlotIndActvTsk
*
*    Functionality:
*       - Receives messages for fapi cl slot indication generator task
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t fapiClSlotIndActvTsk(Pst *pst, Buffer *mBuf)
{
   DU_LOG("\nINFO  -->  FAPI_CL: Received Event [%d]", pst->event);

   switch(pst->srcEnt)
   {
      case ENTFAPICL:
      {
         switch(pst->event)
         {
            case EVT_FAPI_CL_START_SLOT_IND:
            {
               ODU_PUT_MSG_BUF(mBuf);
               generateTicks();
               break;
            }
            default:
            DU_LOG("\nERROR  -->  FAPI_CL: Invalid Event");
         }
      }
   }
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
