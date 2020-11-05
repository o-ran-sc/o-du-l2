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

/* This file is the entry point for Lower MAC */

#include "common_def.h"
#include "lwr_mac_phy.h"

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : lwrMacActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during Lower MAC's 
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]  Ent entity, the entity ID of this task.     
 * @param[in]  Inst inst, the instance ID of this task.
 * @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 * @param[in]  Reason reason.
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
uint8_t lwrMacActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   return ROK;
}

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : lwrMacActvTsk 
 * 
 *      Functionality:
 *           Primitives invoked by Lower MAC's users/providers through
 *           a loosely coupled interface arrive here by means of 
 *           SSI's message handling. This API is registered with
 *           SSI during the Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t lwrMacActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;

   switch(pst->srcEnt)
   {
      case ENTLWRMAC:
	 {
	    switch(pst->event)
	    {
#ifdef INTEL_WLS_MEM
	       case EVT_START_WLS_RCVR:
		  {
		     SPutMsg(mBuf);
		     DU_LOG("\nLWR MAC: Starting WLS receiver thread");
		     LwrMacRecvPhyMsg();
		     break;
		  }
#endif
	       default:
		  {
		     SPutMsg(mBuf);
		     DU_LOG("\nLWR MAC: Invalid event %d received", pst->event);
		     ret = RFAILED;
		  }
	    }
	    break;
	 }
      default:
	 {
	    SPutMsg(mBuf);
	    DU_LOG("\nLWR MAC: Message from invalid source entity %d", pst->srcEnt);
	    ret = RFAILED;
	 }
   }
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
