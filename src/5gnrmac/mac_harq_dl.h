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

void addDlHqProcInUe(SlotTimingInfo dlMsgTime, MacUeCb *ueCb, DlMsgSchInfo *schedInfo);
uint8_t updateNewTbInDlHqProcCb(SlotTimingInfo slotInfo, MacUeCb *ueCb, uint32_t tbSize, uint8_t *txPdu);
uint8_t* fetchTbfromDlHarqProc(SlotTimingInfo slotInfo, MacUeCb *ueCb, uint8_t hqProcId, uint32_t tbSize);
uint8_t MacSchReleaseDlHarqProc(Pst *pst, SchRlsHqInfo *rlsHqInfo);

/**********************************************************************
  End of file
 **********************************************************************/

