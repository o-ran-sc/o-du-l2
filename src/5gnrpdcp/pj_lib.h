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
  
        Name:     LTE PDCP file 
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for PDCP 
 
        File:     pj_lib.h
  
*********************************************************************21*/
/** @file pj_lib.h
@brief PDCP Hash definitions
*/

#ifndef __KW_PJ_LIB_H__
#define __KW_PJ_LIB_H__

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))

#define OBD_SEL_LC                        0

/* Off-board Async Events */
#define OBD_EVT_CMP_INIT_CFM              0x90
#define OBD_EVT_CMP_CFM                   0x91
#define OBD_EVT_DECMP_CFM                 0x92
#define OBD_EVT_CMP_CLOSE_CFM             0x93
#define OBD_EVT_CMP_RESET_CFM             0x94

#define OBD_EVT_INT_INIT_CFM              0xa0
#define OBD_EVT_INT_PROT_CFM              0xa1
#define OBD_EVT_INT_VER_CFM               0xa2
#define OBD_EVT_INT_CLOSE_CFM             0xa3

#define OBD_EVT_CP_INIT_CFM               0xb0
#define OBD_EVT_UP_INIT_CFM               0xb1
#define OBD_EVT_CIPHER_CFM                0xb2
#define OBD_EVT_DECIPHER_CFM              0xb3
#define OBD_EVT_CIPHER_CLOSE_CFM          0xb4

#define ERRLIB                  0

#define ELIB001         (ERRLIB + 1)
#define ELIB002         (ERRLIB + 2)
#define ELIB003         (ERRLIB + 3)
#define ELIB004         (ERRLIB + 4)
#define ELIB005         (ERRLIB + 5)
#define ELIB006         (ERRLIB + 6)
#define ELIB007         (ERRLIB + 7)
#define ELIB008         (ERRLIB + 8)
#define ELIB009         (ERRLIB + 9)
#define ELIB010         (ERRLIB + 10)
#define ELIB011         (ERRLIB + 11)
#define ELIB012         (ERRLIB + 12)
#define ELIB013         (ERRLIB + 13)
#define ELIB014         (ERRLIB + 14)
#define ELIB015         (ERRLIB + 15)
#define ELIB016         (ERRLIB + 16)
#define ELIB017         (ERRLIB + 17)
#define ELIB018         (ERRLIB + 18)
#define ELIB019         (ERRLIB + 19)
#define ELIB020         (ERRLIB + 20)
#define ELIB021         (ERRLIB + 21)
#define ELIB022         (ERRLIB + 22)
#define ELIB023         (ERRLIB + 23)
#define ELIB024         (ERRLIB + 24)
#define ELIB025         (ERRLIB + 25)
#define ELIB026         (ERRLIB + 26)
#define ELIB027         (ERRLIB + 27)
#define ELIB028         (ERRLIB + 28)
#define ELIB029         (ERRLIB + 29)
#define ELIB030         (ERRLIB + 30)
#define ELIB031         (ERRLIB + 31)
#define ELIB032         (ERRLIB + 32)
#define ELIB033         (ERRLIB + 33)
#define ELIB034         (ERRLIB + 34)
#define ELIB035         (ERRLIB + 35)
#define ELIB036         (ERRLIB + 36)
#define ELIB037         (ERRLIB + 37)
#define ELIB038         (ERRLIB + 38)
#define ELIB039         (ERRLIB + 39)
#define ELIB040         (ERRLIB + 40)
#define ELIB041         (ERRLIB + 41)
#define ELIB042         (ERRLIB + 42)
#define ELIB043         (ERRLIB + 43)
#define ELIB044         (ERRLIB + 44)
#define ELIB045         (ERRLIB + 45)
#define ELIB046         (ERRLIB + 46)
#define ELIB047         (ERRLIB + 47)
#define ELIB048         (ERRLIB + 48)
#define ELIB049         (ERRLIB + 49)
#define ELIB050         (ERRLIB + 50)
#define ELIB051         (ERRLIB + 51)
#define ELIB052         (ERRLIB + 52)
#define ELIB053         (ERRLIB + 53)
#define ELIB054         (ERRLIB + 54)
#define ELIB055         (ERRLIB + 55)
#define ELIB056         (ERRLIB + 56)
#define ELIB057         (ERRLIB + 57)
#define ELIB058         (ERRLIB + 58)
#define ELIB059         (ERRLIB + 59)
#define ELIB060         (ERRLIB + 60)
#define ELIB061         (ERRLIB + 61)
#define ELIB062         (ERRLIB + 62)
#define ELIB063         (ERRLIB + 63)
#define ELIB064         (ERRLIB + 64)
#define ELIB065         (ERRLIB + 65)
#define ELIB066         (ERRLIB + 66)
#define ELIB067         (ERRLIB + 67)
#define ELIB068         (ERRLIB + 68)
#define ELIB069         (ERRLIB + 69)
#define ELIB070         (ERRLIB + 70)
#define ELIB071         (ERRLIB + 71)
#define ELIB072         (ERRLIB + 72)
#define ELIB073         (ERRLIB + 73)
#define ELIB074         (ERRLIB + 74)
#define ELIB075         (ERRLIB + 75)

#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
#endif /* __KW_PJ_LIB_H__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
