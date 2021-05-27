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

#ifndef __NR_CELL_LIST_HPP__
#define __NR_CELL_LIST_HPP__

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <map>
#include "sysrepo-cpp/Session.hpp"
#include "ConfigInterface.h"
#include "GlobalDefs.hpp"
#include "Singleton.hpp"
#include "NrCellInfo.hpp"

class NrCellList : public Singleton<NrCellList>
{
   public:
      friend Singleton<NrCellList>;
      typedef std::map<uint16_t, NrCellInfo> CellOpStateMap;

      bool setCellOpState(uint16_t cellId, OpState opState, \
                          CellState cellState);
      const std::map<uint16_t, NrCellInfo> & getCellOpStateList();


   private:
      std::map<uint16_t, NrCellInfo> mCellOpStateMap;
};

#endif

/**********************************************************************
         End of file
**********************************************************************/
