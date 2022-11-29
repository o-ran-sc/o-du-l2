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

#define SCH_DRX_INVALID_DISTANCE -1
#define SCH_DRX_INVALID_INDEX 0xFFFFFF 
#define SCH_DRX_MAX_DELTA 3
#define SCH_DRX_TMRS_EXP_DELTA 1
#define SCH_DRX_DL_HARQ_BITMASK  0x000001
#define SCH_DRX_UL_HARQ_BITMASK  0x000001
#define UE_ACTIVE_FOR_ONDURATION 1
#define UE_ACTIVE_FOR_INACTIVE_TIMER 2
#define UE_ACTIVE_FOR_SR 4

/** @brief Macro to convert milli second to slots */
#define SCH_CNVRT_MS_TO_SLOT(_numSlot, _timeInMs, _mu)\
{\
   _numSlot = _timeInMs * (1 << _mu);\
}

/** @brief Macro to convert number symbols to slots */
#define SCH_CNVRT_SYMBL_TO_SLOT(_numSlot, _numSymbl)\
{\
   _numSlot = _numSymbl/MAX_SYMB_PER_SLOT; \
}

/**  @brief Macro to find the slot difference */
#define SCH_CALC_SLOT_DIFF(_newTime, _oldTime, _numOfSlotsPerRadioFrame)\
((_newTime.sfn*_numOfSlotsPerRadioFrame+_newTime.slot) >= (_oldTime.sfn*_numOfSlotsPerRadioFrame+_oldTime.slot)?\
(_newTime.sfn*_numOfSlotsPerRadioFrame+_newTime.slot) - (_oldTime.sfn*_numOfSlotsPerRadioFrame+_oldTime.slot) : \
(_newTime.sfn*_numOfSlotsPerRadioFrame+_newTime.slot) + (1024*_numOfSlotsPerRadioFrame - (_oldTime.sfn*_numOfSlotsPerRadioFrame+_oldTime.slot)))

/** @brief Macro to used calculate timer index */
#define SCH_CALCULATE_TIMER_INDEX(_numSlots, _timerIndx)\
{\
   _timerIndx = _numSlots % MAX_DRX_SIZE; \
}

void schInitDrxUeCb(SchUeCb *ueCb);
void schFillDrxUeCb(uint8_t numerology, SchDrxCfg drxCfg, SchDrxUeCb *drxUeCb);
void schDeleteUeDrxInfo(SchCellCb  *cell, SchUeCb *ueCb);
void schHandleStartDrxTimer(SchCellCb  *cell);
void schHdlDrxInActvStrtTmr(SchCellCb  *cell,  SchUeCb *ueCb, uint8_t delta);
void schInitDrxHarqCb(SchDrxHarqCb *hqCb);
void schAddUeInOndurationList(SchCellCb *cell, SchUeCb *ueCb, uint8_t delta);
void schDrxUeReCfgTimer(SchCellCb *cell, SchUeCb *ueCb);
void schHdlDrxShortCycleExpiryTimer(SchCellCb  *cell);
void schHdlDrxOnDurExpiryTimer(SchCellCb  *cell);
void schHandleExpiryDrxTimer(SchCellCb  *cell);
void schDrxStrtDlHqRttTmr(SchDlHqProcCb *hqP);
void schDrxStopDlHqRetxTmr(SchCellCb  *cell, SchUeCb *uecb, SchDlHqProcCb **hqP);
void schDrxStopUlHqRetxTmr(SchCellCb  *cell, SchUeCb *ueCb, SchUlHqProcCb **hqP);
void schDrxStrtUlHqRttTmr(SchUlHqProcCb *hqP);
void schHdlDrxUlHqRetxStrtTimer(SchCellCb  *cell);
/**********************************************************************
  End of file
 **********************************************************************/
