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

#include "NrCellInfo.hpp"

using namespace std;

/*******************************************************************
 *
 * @brief convert admin state to enum
 *
 * @details
 *
 *    Function : adminStateToEnum
 *
 *    Functionality:
 *      - convert admin state to enum
 *
 *
 * @params[in] admin state in string form
 * @return admin state in enum form
 ******************************************************************/

AdminState NrCellInfo::adminStateToEnum(string val)
{
   AdminState ret=LOCKED;
   if( val == "LOCKED")
      ret = LOCKED;
   else if(val == "UNLOCKED")
      ret = UNLOCKED;
   else if(val ==  "SHUTTING_DOWN")
      ret = SHUTTING_DOWN;
   else
      O1_LOG("\nO1 NrCellInfo : %s admin state not handled", \
              val.c_str());

   return ret;
}

/*******************************************************************
 *
 * @brief convert enum of cell state to string
 *
 * @details
 *
 *    Function : enumToCellStateString
 *
 *    Functionality:
 *      - convert enum of cell state to string
 *
 *
 * @params[in] enum of cell state
 * @return cell state in string form
 ******************************************************************/


string NrCellInfo::enumToCellStateString(CellState val)
{
   string ret = "IDLE";
   switch(val)
   {
      case INACTIVE:
         ret = "INACTIVE";
         break;
      case ACTIVE:
         ret = "ACTIVE";
         break;
      case IDLE:
         ret = "IDLE";
         break;
      default :
         O1_LOG("\nO1 NrCellInfo : %d cell state not handled", val);

   }
   return ret;
}

/*******************************************************************
 *
 * @brief convert enum of operational state to string
 *
 * @details
 *
 *    Function : enumToOperationalStateString
 *
 *    Functionality:
 *      - convert enum of operational state to string
 *
 *
 * @params[in] enum of operational state
 * @return cell operational in string form
 ******************************************************************/

string NrCellInfo::enumToOperationalStateString(OpState val)
{
   string ret = "DISABLED";
   switch(val)
   {
      case DISABLED:
         ret = "DISABLED";
         break;
      case ENABLED:
         ret = "ENABLED";
         break;
      default :
         O1_LOG("\nO1 NrCellInfo : %d operational state not handled", val);

   }
   return ret;
}

/**********************************************************************
         End of file
**********************************************************************/

