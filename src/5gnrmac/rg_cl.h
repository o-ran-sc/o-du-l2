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

/* Contains definitions for MAC CL modules */
#ifndef __RG_CL_H__
#define __RG_CL_H__

#define MAX_NUM_CELL_SUPP 1

typedef enum
{
   PHY_STATE_IDLE,
   PHY_STATE_CONFIGURED,
   PHY_STATE_RUNNING,
   PHY_STATE_INVALID
}PhyState;

typedef struct clCb
{
   Region          region;
   Pool            pool;
   Bool            clCfgDone;   /* CL configuration done */
   CmHashListCp    cellCbLst;   /* List of Cells configured */
   U8              numOfCells;  /* Number of Cells configured */
   PhyState        phyState;    /* State of PHY */
}ClCb;

typedef struct cellCb
{
   U16         cellId;
   ClCellCfg   cellCfg;
   PhyState    phyState;
}ClCellCb;

EXTERN ClCb clGlobalCp; 
EXTERN ClCellCb * rgClUtlGetCellCb ARGS((U16 cellId));

#endif

/**********************************************************************
         End of file
**********************************************************************/
