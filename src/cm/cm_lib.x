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

PUBLIC U8 *cmMemcpy      ARGS((U8 *tgt, CONSTANT U8 *src, PTR len));
PUBLIC S16 cmMemcmp      ARGS((CONSTANT U8 *s1, CONSTANT U8 *s2, PTR len));
PUBLIC U8 *cmMemset      ARGS((U8 *src, U8 val, PTR len));

PUBLIC S16 cmStrcmp      ARGS((CONSTANT U8 *s1, CONSTANT U8 *s2));
/* cm_lib_x_001.main_8:Changing from S16 to MsgLen.*/
PUBLIC S16 cmStrncmp     ARGS((CONSTANT U8 *s1, CONSTANT U8 *s2, MsgLen len));
PUBLIC MsgLen cmStrlen   ARGS((CONSTANT U8 *s));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif   /* __CMLIBX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
