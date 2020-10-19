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

/**********************************************************************
 
    Name:   LTE MAC HD-FDD 
 
    Type:   C include file
 
    Desc:   Defines required by HD-FDD 
 
    File:   rg_sch_hdfdd.x
 
**********************************************************************/
/**
 *  @file rg_sch_hdfdd.x This file gives the describes the design for Half
 *  Duplex FDD feature.
 *
 *  Half duplex FDD operation is one in which the UE cannot receive and 
 *  transmit at the same time. This is more of a UE limitation, chosen to 
 *  reduce the complexity of the UE's hardware. In LTE the half duplex FDD 
 *  is implemented such that the eNodeB schedules such that the UE doesn't 
 *  transmit and receive in the same subframe. The UE unless informed that a 
 *  subframe is for Uplink transmission continues to look at the PDCCH. 

 *  Due to the delay in switching from downlink to uplink, UE is unable to 
 *  receive the last few symbols of the subframe preceding the subframe 
 *  assigned for uplink transmissions.Half duplex FDD is a UE specific 
 *  configuration.
*/

 
#ifndef __RGHDFDDX__
#define __RGHDFDDX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */




/******************************************************************************
 *                         Start of Data declarations                         *
 ******************************************************************************/

/**
  @brief Half Duplex subframtion information stored per ue. */

typedef struct rgSchUeHdFddSfInfo
{
  uint16_t             sfn;                   /*!< Store the sfn for updated state 
                                              Default Value= 0xffff
                                          */
  uint8_t              state;
                                         /*!< 0x00= DFLT
                                              0x01= DL DATA +(OPT:CNTRL INFO)
                                              0x02= DL CNTRL
                                              0x04= UL DATA+(OPT: CTNRL INFO) 
                                         */ 

}RgSchUeHdFddSfInfo;

/**
  @brief Half Duplex control related information per ue. */

typedef struct rgSchUeHdFddCb
{
   RgSchUeHdFddSfInfo    sf[RG_SCH_HDFDD_MAXSUB_INFO]; 
    
   CmLList               hdFddLstEnt;  /*!< Linked list entity for HD-FDD List */

}RgSchUeHdFddCb;




#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGHDFDD__ */


/**********************************************************************
         End of file
**********************************************************************/
