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
     Name:    LTE MAC HD-FDD 

     Type:    C header file

     Desc:    Defines required by HD-FDD 

     File:    rg_sch_hdfdd.h

*********************************************************************21*/


#ifndef __RGSCHHDFDDH__
#define __RGSCHHDFDDH__ 
#ifdef __cplusplus
extern "C" {
#endif

/* Half Duplex Specific defines */
/* Maximum Number of subframes information managed */
#define RG_SCH_HDFDD_MAXSUB_INFO   20 
 
/* Subframe States */
#define RG_SCH_SF_DFLT_STATE    0x00
#define RG_SCH_SF_DLDATA_STATE  0x01
#define RG_SCH_SF_DLCNTRL_STATE 0x02
#define RG_SCH_SF_ULDATA_CNTRL_STATE 0x04
 
/* To get the BCH is present or not at subframe */
#define RG_SCH_BCCH_TRUE_FALSE( _time, _bchTrue)\
{\
  _bchTrue = FALSE;\
  /* Call the API is provided by SI module */ \
}

#ifdef __cplusplus
}
#endif
#endif /* __RGSCHCMNH__ */

/********************************************************************30**

         End of file
**********************************************************************/
