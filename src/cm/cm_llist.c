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
  
     Name:     common functions - linked list management
  
     Type:     C source file
  
     Desc:     common functions for linked lists
  
     File:     cm_llist.c
  
*********************************************************************21*/
  
/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_llist.x"      /* common functions */


/* Linked List functions */


/*
*
*       Fun:   cmLListInit
*
*       Desc:  initializes a linked list control pointer.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cm_llist.c
*
*/
Void cmLListInit
(
CmLListCp *lCp                /* list control point */
)
{
   
   lCp->first = (CmLList *)NULLP;
   lCp->last  = (CmLList *)NULLP;
   lCp->crnt  = (CmLList *)NULLP;
   lCp->count = 0;

   return;
} /* end of cmLListInit */



/*insert before head*/
/*
*
*       Fun:   cmLListAdd2Head
*
*       Desc:  adds node to linked list before head.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cm_llist.c
*
*/
Void cmLListAdd2Head
(
CmLListCp *lCp,               /* list control point */
CmLList   *node               /* node to be added */
)
{

#ifdef ERRCHK
   if (lCp == (CmLListCp *)NULLP)
      return;
#endif
 
   lCp->count++;

   node->next = lCp->first;
   node->prev = NULLP;
   lCp->first = lCp->crnt = node;
   
   if (!node->next)
   {
      lCp->last = node;
      return;
   }
   
   node->next->prev = node;
   return;
} /* end of cmLListAdd2Head */


/*
*
*       Fun:   cmLListAdd2Tail
*
*       Desc:  adds node to linked list after last.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cm_llist.c
*
*/
Void cmLListAdd2Tail
(
CmLListCp *lCp,               /* list control point */
CmLList   *node               /* node to be added */
)
{

#ifdef ERRCHK
   if (lCp == (CmLListCp *)NULLP)
      return;
#endif
 
   lCp->count++;

   node->prev = lCp->last;
   node->next = NULLP;
   lCp->last = lCp->crnt = node;
   
   if (!node->prev)
   {
      lCp->first = node;
      return;
   }
   
   node->prev->next = node;
   return;
} /* end of cmLListAdd2Tail */


/*
*
*       Fun:   cmLListInsCrnt
*
*       Desc:  adds node to linked list before crnt.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cm_llist.c
*
*/
Void cmLListInsCrnt
(
CmLListCp *lCp,               /* list control point */
CmLList   *node               /* node to be added */
)
{

#ifdef ERRCHK
   if (!lCp)
      return;
#endif
 
   lCp->count++;

   if (lCp->count == 1)
   {
     lCp->crnt = lCp->first = lCp->last = node;
     node->next = NULLP;
     return;
   }

   node->next = lCp->crnt;
   node->prev = lCp->crnt->prev;
   if (node->prev)
      node->prev->next = node;
   node->next->prev = node;
   
   if (lCp->first == lCp->crnt)
      lCp->first = node;
   lCp->crnt = node;
   
   return;
} /* end of cmLListInsCrnt */

/* cm_llist_c_001.main_7 - Add function */
/*
*
*       Fun:   cmLListInsAfterCrnt
*
*       Desc:  adds node to linked list after crnt.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cm_llist.c
*
*/
Void cmLListInsAfterCrnt
(
CmLListCp *lCp,               /* list control point */
CmLList   *node               /* node to be added */
)
{

#ifdef ERRCHK
   if (!lCp)
      return;
#endif
 
   lCp->count++;

   if (lCp->count == 1)
   {
     lCp->crnt = lCp->first = lCp->last = node;
     return;
   }

   node->prev = lCp->crnt;
   node->next = lCp->crnt->next;
   if (node->next)
      node->next->prev = node;
   node->prev->next = node;
  
   if (lCp->last == lCp->crnt)
      lCp->last = node;
   lCp->crnt = node;
   
   return;
} /* end of cmLListInsAfterCrnt */



/*
*
*       Fun:   cmLListDelFrm
*
*       Desc:  remove node pointed to by nodePtr from list and return node.
*              nodePtr could be anywhere in the list.
*              - resets crnt to NULLP.
*
*       Ret:   pointer
*
*       Notes: None
*
*       File:  cm_llist.c
*
*/
CmLList *cmLListDelFrm
(
CmLListCp *lCp,                /* list control pointer */
CmLList *node                  /* node to be removed */
)
{
  
#ifdef ERRCHK
   /* cm_llist_c_001.main_8 : added null check for node */
   if (lCp == (CmLListCp *)NULLP || lCp->count == 0 || !node)
   {
      return (NULLP);
   }
#endif

   if (lCp->count == 1)
   {
      lCp->first = lCp->crnt = lCp->last = (CmLList *)NULLP;
      lCp->count = 0;
      return (node);
   }
   
   lCp->count--;
   lCp->crnt = (CmLList *)NULLP;
   if (lCp->first == node)
   {
      if (node->next)
         node->next->prev = (CmLList *)NULLP;
      lCp->first = node->next;
      node->next = node->prev = (CmLList *)NULLP;
      return (node);
   }
   
   if (lCp->last == node)
   {
      if (node->prev)
         node->prev->next = (CmLList *)NULLP;
      lCp->last = node->prev;
      node->next = node->prev = (CmLList *)NULLP;
      return (node);
   }

   node->prev->next = node->next;
   node->next->prev = node->prev;
   node->next = node->prev = (CmLList *)NULLP;
   return (node);
} /* end of cmLListDelFrm */


/*--
  *
  *       Fun:   cmLListCatLList
  *
  *       Desc:  adds a linked list to the end of the first list. list2 is 
  *              added at the end of list1
  *
  *       Ret:   ROK   - ok
  *
  *       Notes: None
  *
  *       File:  cm_llist.c
  *
  --*/
Void cmLListCatLList
(
CmLListCp *list1,              /*-- list control point --*/
CmLListCp *list2               /*-- node to be added --*/
)
{

   /*-- if the second list is empty nothing to do --*/
   if(list2->count == 0)
   {
      return;
   }

   /*-- if the first list is empty make first the same as second*/
   if(list1->count == 0)
   {
      list1->first = list2->first;
      list1->last  = list2->last;
      list1->count = list2->count;
      list1->crnt  = list1->first;
   }
   else
   {
      list2->first->prev = list1->last;
      list1->last->next = list2->first;
      /* Set the last to the end of the 2nd list */
      list1->last = list2->last;
      list1->count += list2->count;
   }

   cmLListInit(list2);

   return;
} /*-- end of cmLListCatLList --*/

/**********************************************************************
         End of file
**********************************************************************/
