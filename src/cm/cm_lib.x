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

     Name:     common library functions
  
     Type:     C include file
  
     Desc:     Prototypes for common functions that are implemented in
               both a portable and a performance-efficient manner. These
               functions are selected based on the operating system.

     File:     cm_lib.x

*********************************************************************21*/



#ifndef __CMLIBX__
#define __CMLIBX__

#ifdef __cplusplus
extern "C" {
#endif

uint8_t *cmMemcpy      ARGS((uint8_t *tgt, const uint8_t *src, PTR len));
S16 cmMemcmp      ARGS((const uint8_t *s1, const uint8_t *s2, PTR len));
uint8_t *cmMemset      ARGS((uint8_t *src, uint8_t val, PTR len));

S16 cmStrcmp      ARGS((const uint8_t *s1, const uint8_t *s2));
/* cm_lib_x_001.main_8:Changing from S16 to MsgLen.*/
S16 cmStrncmp     ARGS((const uint8_t *s1, const uint8_t *s2, MsgLen len));
MsgLen cmStrlen   ARGS((const uint8_t *s));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif   /* __CMLIBX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
