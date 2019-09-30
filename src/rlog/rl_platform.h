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

/********************************************************************20**

     Name:     Radisys Logging Framework
     Type:     C include file
     Desc:     This file contains logging framework include file.
     File:     rl_interface.h

*********************************************************************21*/
/***************************************************************************
@ description: This is header file is used for platform specific 
implementaion.
****************************************************************************/

#ifndef __RL_PLATFORME_H__

void* rlAlloc(size_t memSize);
void rlFree(void* pMem);
void* rlCalloc(size_t memSize);
void deInitThread(void* pThreadData);
void* rlGetThreadSpecData(void);
void rlSetThreadSpecificData(const void *pThrData);
void rlInitPlatformSpecific(void);

#endif /* __RL_PLATFORME_H__ */

/**********************************************************************
         End of file
**********************************************************************/
