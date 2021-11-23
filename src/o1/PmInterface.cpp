/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2021] [HCL Technologies Ltd.]                          #
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

/* This file contains the C interface for ODU to access the Performance 
   Management functions */

#include "PmInterface.h"
#include "VesEventHandler.hpp"
#include "SliceMetrics.hpp"
#include <unistd.h>
#include "GlobalDefs.hpp"


/*******************************************************************
 *
 * @brief Send the Slice metrics to SMO as a VES message
 *
 * @details
 *
 *    Function : sendSliceMetric
 *
 *    Functionality:
 *      - Takes the Slice metrics list and sends it to SMO
 *
 *
 * @params[in] pointer to SliceMetricList
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
int sendSliceMetric(SliceMetricList* sliceMetricList) {

   O1_LOG("\n PmInterfce : Call received from the the du_app code !!");

   SliceMetrics metrics;
 
   for(int i = 0; i < sliceMetricList->nRecords; i++)
      metrics.addMetric(sliceMetricList->sliceRecord[i]); 

   VesEventHandler vesEventHandler;
   if (!vesEventHandler.prepare(VesEventType::PM_SLICE, &metrics))
      return O1::FAILURE;
   
   O1_LOG("\n PmInterface : Sending slice PM Data");
   if ( !vesEventHandler.send() )
      return O1::FAILURE;

   return O1::SUCCESS;
}

/**********************************************************************
         End of file
**********************************************************************/
