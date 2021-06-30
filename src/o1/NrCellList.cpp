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

/* This file contains List of the Cell and it's parameters values for CLA 
   use case*/

#include <sstream>
#include "NrCellList.hpp"
#define MAX_LEN 100

using namespace std;

/*******************************************************************
 *
 * @brief update cell and operational state in the map
 *
 * @details
 *
 *    Function : setCellOpState
 *
 *    Functionality:
 *      - update cell and operational state in the map using cellId
 *        as key
 *
 *
 * @params[in] cellId, opState, cellState
 * @return true   - success
 *         false  - failure
 ******************************************************************/


bool NrCellList::setCellOpState(uint16_t cellId, \
                                OpState opState, \
                                CellState cellState)
{
   O1_LOG("\nO1 NrCellList : Setting cellId = %d, opState=%d, \
cellState=%d", cellId, opState, cellState);
   NrCellInfo cellInfo ;
   cellInfo.setCellId( cellId);
   cellInfo.setOpState(opState);
   cellInfo.setCellState (cellState);
   mCellOpStateMap[cellId] = cellInfo;
   return true;
}

/*******************************************************************
 *
 * @brief provide the cell list 
 *
 * @details
 *
 *    Function : getCellOpStateList
 *
 *    Functionality:
 *      - provide the cell list
 *
 *
 * @params[in] na
 * @return map of cell   - success
 ******************************************************************/

const std::map<uint16_t, NrCellInfo> & NrCellList::getCellOpStateList()
{
   return mCellOpStateMap;
}
/**********************************************************************
         End of file
**********************************************************************/

