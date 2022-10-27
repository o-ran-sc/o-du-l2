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

#define SCH_DRX_INVALID_IDX -1
#define SCH_DRX_MAX_DELTA 1

/** @brief Macro to convert milli second to slots */
#define SCH_CNVRT_MS_TO_SLOT(_numSlot, _numMs, _mu)\
{\
   _numSlot = _numMs * (1 << _mu);\
}

/** @brief Macro to convert number symbols to slots */
#define SCH_CNVRT_SYMBL_TO_SLOT(_numSlot, _numSymbl)\
{\
   _numSlot = _numSymbl/14; \
}

/**  @brief Macro to find the slot difference */
#define SCH_CALC_SLOT_DIFF(_newTime, _oldTime, _numOfSlotsPerRadioFrame)\
((_newTime.sfn*_numOfSlotsPerRadioFrame+_newTime.slot) >= (_oldTime.sfn*_numOfSlotsPerRadioFrame+_oldTime.slot)?\
(_newTime.sfn*_numOfSlotsPerRadioFrame+_newTime.slot) - (_oldTime.sfn*_numOfSlotsPerRadioFrame+_oldTime.slot) : \
(_newTime.sfn*_numOfSlotsPerRadioFrame+_newTime.slot) + (1024*_numOfSlotsPerRadioFrame - (_oldTime.sfn*_numOfSlotsPerRadioFrame+_oldTime.slot)))

void schInitDrxUeCb(SchUeCb *ueCb);
void schFillDrxUeCb(uint8_t numerology, SchDrxCfg drxCfg, SchDrxUeCb *drxUeCb);
void schDeleteUeDrxInfo(SchCellCb  *cell, SchUeCb *ueCb);
void schHandleStartDrxTimer(SchCellCb  *cell);
void schHdlDrxInActvStrtTmr(SchCellCb  *cell,  SchUeCb *ueCb, uint8_t delta);
/**********************************************************************
  End of file
 **********************************************************************/
