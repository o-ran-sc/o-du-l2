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
  
     Name:     common - linked list functions
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required by the
               linked list management routines.
 
     File:     rg_sch_clist.x
 
*********************************************************************21*/
  
#ifndef __RGSCHRRCLIST__
#define __RGSCHRRCLIST__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct rgSchRrCList   RgSchRrCList;
typedef struct rgSchRrCListCp RgSchRrCListCp;

/* doubly linked list */
struct rgSchRrCList
{
   RgSchRrCList *next;        /* next */
   RgSchRrCList *prev;        /* previous */
   PTR    node;          /* node */
};

struct rgSchRrCListCp
{
   RgSchRrCList *first;       /* first entry in list */
   RgSchRrCList *crnt;        /* entry last accessed */
   uint32_t     count;        /* number of entries */
};

Void         rgSCHRrCListInit     ARGS ((RgSchRrCListCp *lList));
Void         rgSCHRrCListAdd2Tail ARGS ((RgSchRrCListCp *lList, \
                                                    RgSchRrCList *node));
/* Renamed functions to start with rgSCH */
RgSchRrCList *rgSCHRrCListDelFrm   ARGS ((RgSchRrCListCp *lList, \
                                                    RgSchRrCList *node));
Void         rgSCHRrCListInsrtAtCrnt ARGS ((RgSchRrCListCp *lList, \
                                                    RgSchRrCList *node));
/* LTE_ADV_FLAG_REMOVED_START */
Void rgSCHRrCListAdd2Crnt ARGS ((RgSchRrCListCp *lList, \
                                        RgSchRrCList   *node));
/* LTE_ADV_FLAG_REMOVED_END */


#ifdef __cplusplus
}
#endif

#endif /* __RRCLIST__ */

/**********************************************************************
 
         End of file
**********************************************************************/
