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

/* This file contains base class for Performance Measurement VES Event*/


#include "PerfMeasurementEvent.hpp"

/* Constructor */
PerfMeasurementEvent::PerfMeasurementEvent(VesEventType eventType)
                :VesEvent(eventType)
{

}
/* Default Destructor*/
PerfMeasurementEvent::~PerfMeasurementEvent()
{
    
}

/* overriding */
/*******************************************************************
 *
 * @brief Get SMO Ves Collector configuration 
 *
 * @details
 *
 *    Function : getConfig
 *
 *    Functionality:
 *      - Gets Ves Collector configuration
 *
 *
 * @params[in] void
 * @return void
 ******************************************************************/
void PerfMeasurementEvent::getConfig()
{
    mVesServerIp = ConfigLoader::instance().getSmoConfigFile().getVesServerIp();
    mVesServerPort = ConfigLoader::instance().getSmoConfigFile().getVesServerPort();
    mVesServerUsername = ConfigLoader::instance().getSmoConfigFile().getVesServerUsername();
    mVesServerPassword = ConfigLoader::instance().getSmoConfigFile().getVesServerPassword();
}

/*******************************************************************
 *
 * @brief Create the URL for sending VES messages
 *
 * @details
 *
 *    Function : createUrl (overridden)
 *
 *    Functionality:
 *      - Creates the VES URL
 *
 *
 * @params[in] void
 * @return void
 ******************************************************************/
void PerfMeasurementEvent::createUrl()
{
   mVesUrl = "https://" + mVesServerIp + ":" + mVesServerPort + "/eventListener/v7/events";
   O1_LOG("\nURL=%s", mVesUrl.c_str());
}

/**********************************************************************
         End of file
**********************************************************************/
