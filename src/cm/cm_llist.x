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
 
     File:     cm_llist.x
 
*********************************************************************21*/
  
#ifndef __CMLLISTX__
#define __CMLLISTX__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct cmLList CmLList;
typedef struct cmLListCp CmLListCp;

/* doubly linked list */
struct cmLList
{
   CmLList *next;        /* next */
   CmLList *prev;        /* previous */
   PTR    node;          /* node */
};

struct cmLListCp
{
   CmLList *first;       /* first entry in list */
   CmLList *last;        /* last entry in list */
   CmLList *crnt;        /* entry last accessed */
   uint32_t     count;        /* number of entries */
};

Void     cmLListInit     ARGS ((CmLListCp *lList));
Void     cmLListAdd2Head ARGS ((CmLListCp *lList, CmLList *node));
Void     cmLListAdd2Tail ARGS ((CmLListCp *lList, CmLList *node));
Void     cmLListInsCrnt  ARGS ((CmLListCp *lList, CmLList *node));
/* cm_llist_x_001.main_6 - Add function */
Void     cmLListInsAfterCrnt  ARGS ((CmLListCp *lList, CmLList *node));
CmLList *cmLListDelFrm   ARGS ((CmLListCp *lList, CmLList *node));
Void cmLListCatLList ARGS (( CmLListCp *list1, CmLListCp *list2));

#ifdef __cplusplus
}
#endif

#endif /* __CMLLISTX__ */

 
/********************************************************************30**
  
         End of file
**********************************************************************/
