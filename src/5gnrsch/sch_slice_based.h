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

typedef struct schSliceBasedCellCb
{
   CmLListCp     ueToBeScheduled;                   /*!< Linked list to store UEs pending to be scheduled, */
}SchSliceBasedCellCb;

typedef struct schSliceBasedLcCb
{
   /* TODO: For Multiple RRMPolicies, Make DedicatedLcInfo as array/Double Pointer 
    * and have separate DedLCInfo for each RRMPolcyMemberList*/
   /* Dedicated LC List will be allocated, if any available*/
   CmLListCp dedLcList;	/*Contain LCInfo per RRMPolicy*/
   CmLListCp defLcList; /*Linklist of LC assoc with Default S-NSSAI(s)*/
   /* SharedPRB number can be used by any LC.
    * Need to calculate in every Slot based on PRB availability*/
   uint16_t sharedNumPrb;
}SchSliceBasedLcCb;

typedef struct schSliceBasedHqProcCb
{
   SchSliceBasedLcCb lcCb; 
}SchSliceBasedHqProcCb;

typedef struct schSliceBasedHqCb
{
   CmLListCp  ulRetxHqList;
   CmLListCp  dlRetxHqList;
}SchSliceBasedHqCb;

typedef struct schSliceBasedUeCb
{
   SchSliceBasedHqCb   hqRetxCb;
}SchSliceBasedUeCb;

uint8_t schSliceBasedAddUeToSchedule(SchCellCb *cellCb, uint16_t ueIdToAdd);
void schSliceBasedAllApisInit(SchAllApis *allSliceBasedApi);

/**********************************************************************
    End of file
 *********************************************************************/

