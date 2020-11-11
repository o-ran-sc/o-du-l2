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
  
     File:     rg_sch_utl_clist.c
  
*********************************************************************21*/
  
/* header include files (.h) */
#include "common_def.h"

/* header/extern include files (.x) */
#include "rg_sch_clist.x"      /* common functions */


/* Linked List functions */


/*
*
*       Fun:   rgSCHRrCListInit
*
*       Desc:  initializes a linked list control pointer.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
Void rgSCHRrCListInit
(
RgSchRrCListCp *lCp                /* list control point */
)
{
   
   lCp->first = (RgSchRrCList *)NULLP;
   lCp->crnt  = (RgSchRrCList *)NULLP;
   lCp->count = 0;

   return;
} /* end of rgSCHRrCListInit */

/* LTE_ADV_FLAG_REMOVED_START */
/*
 *        Fun:   rgSCHRrCListAdd2Crnt
 * 
 *        Desc:  adds node to linked list behind crnt.
 * 
 *        Ret:   ROK   - ok
 * 
 *        Notes: None
 * 
 *        File:  rr_clist.c
 */
Void rgSCHRrCListAdd2Crnt
(
RgSchRrCListCp *lCp,               /* list control point */
RgSchRrCList   *node               /* node to be added */
)
{
#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      return;
#endif

   lCp->count++;

   if(!lCp->first)
   {
      node->prev = node;
      node->next = node;
      lCp->first = node;

      lCp->crnt = lCp->first;

      return;
   }

   node->next = lCp->crnt;
   node->prev = lCp->crnt->prev;
   lCp->crnt->prev->next = node;
   lCp->crnt->prev = node;

   return;
}
/* LTE_ADV_FLAG_REMOVED_END */

/*
*
*       Fun:   rgSCHRrCListAdd2Tail
*
*       Desc:  adds node to linked list after last.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
Void rgSCHRrCListAdd2Tail
(
RgSchRrCListCp *lCp,               /* list control point */
RgSchRrCList   *node               /* node to be added */
)
{

#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      return;
#endif
 
   lCp->count++;

   if(!lCp->first)
   {
      node->prev = node;
      node->next = node;
      lCp->first = node;

      lCp->crnt = lCp->first;

      return;
   }

   node->next = lCp->first;
   node->prev = lCp->first->prev;
   lCp->first->prev->next = node;
   lCp->first->prev = node;

   return;
} /* end of rgSCHRrCListAdd2Tail */

/*
*
*       Fun:   rgSCHRrCListDelFrm
*
*       Desc:  remove node pointed to by nodePtr from list and return node.
*              nodePtr could be anywhere in the list.
*              - resets crnt to NULLP.
*
*       Ret:   pointer
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
RgSchRrCList *rgSCHRrCListDelFrm
(
RgSchRrCListCp *lCp,                /* list control pointer */
RgSchRrCList *node                  /* node to be removed */
)
{
  
#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      return (NULLP);
#endif

   if(lCp->count == 0)
   {
      return (NULLP);
   }
   if (lCp->count == 1)
   {
      if(lCp->first == node)
      {
         lCp->first = lCp->crnt = (RgSchRrCList *)NULLP;
         lCp->count = 0;
         node->next = node->prev = (RgSchRrCList *)NULLP;
         return (node);
      }
      return (NULLP);
   }
   
   if (lCp->first == node)
   {
      lCp->first->prev->next = node->next;
      node->next->prev = lCp->first->prev;
      lCp->first = node->next;
      if(lCp->crnt == node)
      {
         lCp->crnt = node->next;
      }
      node->next = node->prev = (RgSchRrCList *)NULLP;
       /* Adding this check and guarding the decrement of counter when
       node is deleted with reshuffling */
      lCp->count--;
      return (node);
   }

   if(node->prev && node->next)
   {
      node->prev->next = node->next;
      node->next->prev = node->prev;
      lCp->count--;
   }
   if(lCp->crnt == node)
   {
      lCp->crnt = node->next;
   }
   node->next = node->prev = (RgSchRrCList *)NULLP;
   return (node);
} /* end of rgSCHRrCListDelFrm */

/*
*
*       Fun:   rgSCHRrCListInsrtAtCrnt
*
*       Desc:  Inserting the given node at CuRRENT and Moving present CURRENT 
*              node to next.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
Void rgSCHRrCListInsrtAtCrnt
(
RgSchRrCListCp *lCp,                /* list control pointer */
RgSchRrCList *node                  /* node to be removed */
)
{
   RgSchRrCList *crnt;

#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      return;
#endif

   crnt = lCp->crnt;
   lCp->crnt = node;

   node->prev = crnt->prev;
   crnt->prev->next = node;
   node->next = crnt;
   crnt->prev = node;

   lCp->count++;

   return;
}

/********************************************************************30**
  
         End of file
**********************************************************************/
