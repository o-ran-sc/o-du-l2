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

/* This file contains all utility functions for MAC CL */

#include "stdio.h"
#include "envopt.h"
#include "envdep.h"
#include "envind.h"
#include "gen.h"
#include "ssi.h"
#include "cm_hash.h"
#include "gen.x"
#include "ssi.x"
#include "cm_hash.x"
#include "lwr_mac.h"

 /*******************************************************************
  *
  * @brief Fetch cellCb from Hash list
  *
  * @details
  *
  *    Function : rgClUtlGetCellCb
  *
  *    Functionality:
  *       - Searches for a cell entry at MAC CL 
  *
  * @params[in] cell Id
  * @return Pointer to cellCb - success
  *         NULLP - failure
  *
  * ****************************************************************/
PUBLIC ClCellCb * rgClUtlGetCellCb
(
   U16 cellId
)
{
   ClCellCb *cellCb;

   if(cellId >= MAX_NUM_CELL_SUPP)
   {
      printf("\n Invalid Cell Id [%d]. rgClUtlGetCellCb failed.", cellId);
      RETVALUE(NULLP);
   }

   cmHashListFind((CmHashListCp *)&clGlobalCp.cellCbLst, (U8 *)&cellId, sizeof(U16), 0, (PTR *)&cellCb);
   
   RETVALUE(cellCb);
}

uint32_t reverseBits(uint32_t num, uint32_t numBits)
{
   uint32_t reverse_num = 0;
   int i;
   for (i = 0; i < numBits; i++)
   {
      if((num & (1 << i)))
         reverse_num |= 1 << ((numBits - 1) - i);
   }
   return reverse_num;
}


/**********************************************************************
         End of file
**********************************************************************/
