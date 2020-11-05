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
 
/* Defines API exchanged between DU_APP and SCTP */
#ifndef __LSCTP_H___
#define __LSCTP_H___

typedef uint8_t (*SctpNtfy) ARGS((Buffer *mBuf, CmInetSctpNotification *ntfy));
uint8_t cmPkSctpNtfy(Pst *pst, CmInetSctpNotification *ntfy);
uint8_t cmUnpkSctpNtfy(SctpNtfy func, Pst *pst, Buffer *mBuf);

#endif
