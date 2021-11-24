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

/* This file contains parameters list of a cell for CLA use case*/

#ifndef __NR_CELL_INFO_HPP__
#define __NR_CELL_INFO_HPP__

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "sysrepo-cpp/Session.hpp"
#include "CmInterface.h"
#include "GlobalDefs.hpp"
#include "Singleton.hpp"
#include "NrCellInfo.hpp"

class NrCellInfo
{
   public:
      inline void setCellId(uint16_t id) { mCellId = id;}
      inline void setOpState(OpState state) { mOpState = state;}
      inline void setCellState( CellState state) { mCellState = state;} 
      inline uint16_t getCellId() const {return mCellId;}
      inline OpState getOpState() const { return mOpState;}
      inline CellState getCellState() const { return mCellState;}
 
      string enumToOperationalStateString(OpState val);
      string enumToCellStateString(CellState val);
      AdminState adminStateToEnum(string val);

   private:
      uint16_t mCellId;
      OpState mOpState;
      CellState mCellState;
};

#endif

/**********************************************************************
         End of file
**********************************************************************/
