/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [Radisys]                                             #
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
/* Utility definitions to be used in du app */
#include "common_def.h"
#include "du_tmr.h"
#include "lrg.h"
#include "lrg.x"
#include "lkw.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_e2ap_mgr.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_utils.h"

/* Spec Ref-38.214-Table 5.1.2.1-1 */
uint8_t slivCfgIdxTable[MAX_SLIV_CONFIG_IDX][3] = {
/*S    L     SLIV*/
{ 0,   1,    0  },/* index0  */
{ 0,   2,    14 },/* index1  */
{ 0,   3,    28 },/* index2  */
{ 0,   4,    42 },/* index3  */
{ 0,   5,    56 },/* index4  */
{ 0,   6,    70 },/* index5  */
{ 0,   7,    84 },/* index6  */
{ 0,   8,    98 },/* index7  */
{ 0,   9,    97 },/* index8  */
{ 0,   10,   83 },/* index9  */
{ 0,   11,   69 },/* index10 */
{ 0,   12,   55 },/* index11 */
{ 0,   13,   41 },/* index12 */
{ 0,   14,   27 },/* index13 */
{ 1,   1,    1  },/* index14 */
{ 1,   2,    15 },/* index15 */
{ 1,   3,    29 },/* index16 */
{ 1,   4,    43 },/* index17 */
{ 1,   5,    57 },/* index18 */
{ 1,   6,    71 },/* index19 */
{ 1,   7,    85 },/* index20 */
{ 1,   8,    99 },/* index21 */
{ 1,   9,    96 },/* index22 */
{ 1,   10,   82 },/* index23 */
{ 1,   11,   68 },/* index24 */
{ 1,   12,   54 },/* index25 */
{ 1,   13,   40 },/* index26 */
{ 2,   1,    2  },/* index27 */
{ 2,   2,    16 },/* index28 */
{ 2,   3,    30 },/* index29 */
{ 2,   4,    44 },/* index30 */
{ 2,   5,    58 },/* index31 */
{ 2,   6,    72 },/* index32 */
{ 2,   7,    86 },/* index33 */
{ 2,   8,    100},/* index34 */
{ 2,   9,    95 },/* index35 */
{ 2,   10,   81 },/* index36 */
{ 2,   11,   67 },/* index37 */
{ 2,   12,   53 },/* index38 */
{ 3,   1,    3  },/* index39 */
{ 3,   2,    17 },/* index40 */
{ 3,   3,    31 },/* index41 */
{ 3,   4,    45 },/* index42 */
{ 3,   5,    59 },/* index43 */
{ 3,   6,    73 },/* index44 */
{ 3,   7,    87 },/* index45 */
{ 3,   8,    101},/* index46 */
{ 3,   9,    94 },/* index47 */
{ 3,   10,   80 },/* index48 */
{ 3,   11,   66 },/* index49 */
{ 4,   1,    4  },/* index50 */
{ 4,   2,    18 },/* index51 */
{ 4,   3,    32 },/* index52 */
{ 4,   4,    46 },/* index53 */
{ 4,   5,    60 },/* index54 */
{ 4,   6,    74 },/* index55 */
{ 4,   7,    88 },/* index56 */
{ 4,   8,    102},/* index57 */
{ 4,   9,    93 },/* index58 */
{ 4,   10,   79 },/* index59 */
{ 5,   1,    5  },/* index60 */
{ 5,   2,    19 },/* index61 */
{ 5,   3,    33 },/* index62 */
{ 5,   4,    47 },/* index63 */
{ 5,   5,    61 },/* index64 */
{ 5,   6,    75 },/* index65 */
{ 5,   7,    89 },/* index66 */
{ 5,   8,    103},/* index67 */
{ 5,   9,    92 },/* index68 */
{ 6,   1,    6  },/* index69 */
{ 6,   2,    20 },/* index70 */
{ 6,   3,    34 },/* index71 */
{ 6,   4,    48 },/* index72 */
{ 6,   5,    62 },/* index73 */
{ 6,   6,    76 },/* index74 */
{ 6,   7,    90 },/* index75 */
{ 6,   8,    104},/* index76 */
{ 7,   1,    7  },/* index77 */
{ 7,   2,    21 },/* index78 */
{ 7,   3,    35 },/* index79 */
{ 7,   4,    49 },/* index80 */
{ 7,   5,    63 },/* index81 */
{ 7,   6,    77 },/* index82 */
{ 7,   7,    91 },/* index83 */
{ 8,   1,    8  },/* index84 */
{ 8,   2,    22 },/* index85 */
{ 8,   3,    36 },/* index86 */
{ 8,   4,    50 },/* index87 */
{ 8,   5,    64 },/* index88 */
{ 8,   6,    78 },/* index89 */
{ 9,   1,    9  },/* index90 */
{ 9,   2,    23 },/* index91 */
{ 9,   3,    37 },/* index92 */
{ 9,   4,    51 },/* index93 */
{ 9,   5,    65 },/* index94 */
{ 10,  1,    10 },/* index95 */
{ 10,  2,    24 },/* index96 */
{ 10,  3,    38 },/* index97 */
{ 10,  4,    52 },/* index98 */
{ 11,  1,    11 },/* index99 */
{ 11,  2,    25 },/* index100*/
{ 11,  3,    39 },/* index101*/
{ 12,  1,    12 },/* index102*/
{ 12,  2,    26 },/* index103*/
{ 13,  1,    13 } /* index104*/
};

/*******************************************************************
 *
 * @brief Function to fill the start Symbol and Symbol Len from the 
 * sliv Config Idx Table
 *
 * @details
 *
 *    Function : fillStartSymbolAndLen
 *
 *    Functionality: Function to fill the start Symbol and Symbol Len 
 *                   from the sliv Config Idx Table
 *
 * @params[in] startSymbolIdx pointer,
 *             symbolLen pointer,
 *             sliv
 * @return void
 *
 * ****************************************************************/
void fillStartSymbolAndLen(uint8_t numRsrcAlloc, PdschConfig *pdschCfg, PuschCfg *puschCfg)
{
   uint8_t slivIdx, timeDomIdx;

   for(timeDomIdx = 0; timeDomIdx < numRsrcAlloc; timeDomIdx++)
   {
      for(slivIdx = 0; slivIdx < MAX_SLIV_CONFIG_IDX; slivIdx++)
      {
         if(pdschCfg)/* PDSCH Config */
	 {
            if(pdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbolAndLength ==\
               slivCfgIdxTable[slivIdx][2])
            {
               pdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbol  = slivCfgIdxTable[slivIdx][0];
               pdschCfg->timeDomRsrcAllociList[timeDomIdx].symbolLength = slivCfgIdxTable[slivIdx][1];
               break;
            }
         }
	 if(puschCfg)/* PUSCH Config */
	 {
            if(puschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbolAndLength ==\
               slivCfgIdxTable[slivIdx][2])
            {
               puschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbol  = slivCfgIdxTable[slivIdx][0];
               puschCfg->timeDomRsrcAllocList[timeDomIdx].symbolLength = slivCfgIdxTable[slivIdx][1];
               break;
            }

	 }
      }
   }
}

/*******************************************************************
 * @brief Function to add a node to a linked list
 *
 * @details
 *
 *     Function: duAddNodeToLList
 *
 *     This function adds a new node to the linked list
 *
 *  @param[in]  Pointer to the list
 *              Pointer to node to be added
 *              Pointer to current node
 *  @return     ROK
 *              RFAILED
*******************************************************************/
uint8_t duAddNodeToLList(CmLListCp *llist, void *blockToAdd, CmLList *currNode)
{
   CmLList  *newNode = NULLP;

   DU_ALLOC(newNode, sizeof(CmLList));
   if(newNode)
   {
      newNode->node = (PTR)blockToAdd;
      
      if(currNode == NULLP)
         cmLListAdd2Tail(llist, newNode);
      else
      {
         llist->crnt = currNode;
         cmLListInsAfterCrnt(llist, newNode);
      }
      return ROK;
   } 
   return RFAILED;
}

/*******************************************************************
 * @brief Function to delete a node from linked list
 *
 * @details
 *
 *     Function: duDelNodeFromLList
 *
 *     This function deletes a node from the linked list
 *
 *  @param[in]  Pointer to the list
 *              Pointer to node to be deleted
 *  @return     Pointer to the deleted node
*******************************************************************/

uint8_t duDelNodeFromLList(CmLListCp *llist, CmLList *node)
{
   node = cmLListDelFrm(llist, node);
   DU_FREE(node, sizeof(CmLList));

   return ROK;
}

/*******************************************************************
 * @brief Handle the PageUe List
 *
 * @details
 *
 *    Function : handlePageUeLL
 *
 *    Functionality: Handling the (SEARCH,CREATE,DELETE) PageUeList
 *
 * @params[in] DuPagingMsg *pagingParam, CmLListCp *pageUeLL, ActionTypeLL
 * action
 *
 * @return DuPagUeRecord 
 *
 * ****************************************************************/
DuPagUeRecord* handlePageUeLL(uint16_t pagUeId, uint64_t sTmsi, CmLListCp *pageUeLL, ActionTypeLL action)
{
   CmLList  *node = NULLP;
   DuPagUeRecord *ueRecord = NULLP;
   bool found = FALSE;

   if((pageUeLL == NULLP) ||
          ((pageUeLL->first == NULLP) && (action != CREATE)))
   {
      DU_LOG("\nERROR  -->  DU APP: UE Page Record LL is empty");
      return NULLP;
   }
   node = pageUeLL->first;

   while(node)
   {
      ueRecord = (DuPagUeRecord *)node->node;
      if(action == PRINT)
      {
         DU_LOG("\n  INFO   -->  DU APP ueId:%d, sTmsi:%lu",\
                 ueRecord->pagUeId, ueRecord->sTmsi);
      }
      else if(ueRecord && (ueRecord->pagUeId == pagUeId && 
               ueRecord->sTmsi == sTmsi))
      {
         found = TRUE;
         break;
      }
      node = node->next;
   }

   switch(action)
   {
      case SEARCH:
         {
            if(!found)
            {
               ueRecord = NULLP;
            }
            return ueRecord;
         }

      case CREATE:
         {
            if(node != NULLP)
               return ueRecord;

            /*Need to add a new node for this LC*/

            /*List is empty; Initialize the LL ControlPointer*/
            if(pageUeLL->count == 0)
            {
               cmLListInit(pageUeLL);
            }

            ueRecord = NULLP;
            /*Allocate the List*/
            DU_ALLOC(ueRecord, sizeof(DuPagUeRecord));
            if(ueRecord)
            {
               ueRecord->pagUeId = pagUeId;
               ueRecord->sTmsi = sTmsi;
            }
            else
            {
               DU_LOG("\nERROR  -->  DU APP : Allocation of UE Record failed,ueId:%d",pagUeId);
               return NULLP;
            }

            if(duAddNodeToLList(pageUeLL, ueRecord, NULLP) == RFAILED)
            {
               DU_LOG("\nERROR  -->  DU APP : failed to Add Ue Record Node,ueId:%d",pagUeId);
               DU_FREE(ueRecord, sizeof(DuPagUeRecord));
               return NULLP;
            }
            return ueRecord;
         }
      case DELETE:
         {
            if(!found ||  ueRecord == NULLP)
            {
               DU_LOG("\nERROR  -->  DU APP: UeId:%d not found; thus Deletion unsuccessful",pagUeId);
            }
            else
            {
               if(duDelNodeFromLList(pageUeLL, node) == ROK)
                  DU_FREE(ueRecord, sizeof(DuPagUeRecord));
            }
            return NULLP;
         }
      case PRINT:
      case TRAVERSE_ALL:
         {
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  DU APP: Incorrect ActionType:%d on UeRecord",action);
         }
   }
   return NULLP;
}

/*******************************************************************
 * @brief Handle the PageInfo List
 *
 * @details
 *
 *    Function : handlePageInfoLL
 *
 *    Functionality: Handling the (SEARCH,CREATE,DELETE) PageInfoList
 *
 * @params[in] uint8_t i_s, CmLListCp *pagInfoLL, ActionTypeLL action
 *
 * @return DuPagUeList 
 *
 * ****************************************************************/
DuPagUeList* handlePageInfoLL(uint16_t pf, uint8_t i_s, CmLListCp *pagInfoLL, ActionTypeLL action)
{
   CmLList  *node = NULLP, *next = NULLP;
   DuPagUeList *pagInfo = NULLP;
   bool found = FALSE;
   
   if((pagInfoLL == NULLP) || 
         ((pagInfoLL->first == NULLP) && (action != CREATE)))
   {
      DU_LOG("\nERROR  -->  DU APP: PagInfo LL is empty");
      return NULLP;
   }
   node = pagInfoLL->first;

   while(node)
   {
      next = node->next;
      pagInfo = (DuPagUeList *)node->node;
      if(action == PRINT)
      {
         DU_LOG("\n INFO   -->  DU APP: Paging Index (i_s):%d",pagInfo->i_s);
         handlePageUeLL(NULLD, NULLD, &(pagInfo->pagUeList), PRINT);
      }
      else if(action == TRAVERSE_ALL)
      {
          return pagInfo;
      }
      else if(pagInfo->i_s == i_s)
      {
         found = TRUE;
         break;
      }
      node = next;
   }

   switch(action)
   {
      case SEARCH:
         {
            if(!found)
            {
               pagInfo = NULLP;
            }
            return pagInfo;
         }

      case CREATE:
         {
            if(node != NULLP)
               return pagInfo;

            /*Need to add a new node for this LC*/

            /*List is empty; Initialize the LL ControlPointer*/
            if(pagInfoLL->count == 0)
            {
               cmLListInit(pagInfoLL);
            }

            pagInfo = NULLP;
            /*Allocate the List*/
            DU_ALLOC(pagInfo, sizeof(DuPagUeList));
            if(pagInfo)
            {
               pagInfo->i_s = i_s;
            }
            else
            {
               DU_LOG("\nERROR  -->  DU APP : Allocation of List failed,i_s:%d",i_s);
               return NULLP;
            }

            if(duAddNodeToLList(pagInfoLL, pagInfo, NULLP) == RFAILED)
            {
               DU_LOG("\nERROR  -->  DU APP : failed to Add Node,i_s:%d",i_s);
               DU_FREE(pagInfo, sizeof(DuPagUeList));
               return NULLP;
            }
            return pagInfo;
         }
      case DELETE:
         {
            if(!found ||  pagInfo == NULLP)
            {
               DU_LOG("\nERROR  -->  DU APP: i_s:%d not found; thus Deletion unsuccessful",i_s);
            }
            else
            {
               if(duDelNodeFromLList(pagInfoLL, node) == ROK)
                  DU_FREE(pagInfo, sizeof(DuPagUeList));
            }
            return NULLP;
         }
      case PRINT:
      case TRAVERSE_ALL:
         {
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  DU APP: Incorrect ActionType:%d on PageInfo List",action);
         }
   }
   return NULLP;
}

/*******************************************************************
 * @brief Find the PageInfo List from HashMap 
 *
 * @details
 *
 *    Function : findPagingInfoFromMap
 *
 *    Functionality: Search for the PageInfoList for a PF from HashMap
 *
 * @params[in] uint16_t pf, CmHashListCp *pagingInfoMap
 *
 * @return DuPagInfoList 
 *
 * ****************************************************************/
DuPagInfoList* findPagingInfoFromMap(uint16_t pf, CmHashListCp *pagingInfoMap)
{
   DuPagInfoList *pagInfoLL = NULLP;

   cmHashListFind(pagingInfoMap, (uint8_t *)&(pf), sizeof(uint16_t), 0, (PTR *)&pagInfoLL);
   
   return pagInfoLL;
}

/*Below function for printing will be used in future so disabling it for now*/
#if 0 
/*******************************************************************
 * @brief Print the Page Info List and UE Records
 *
 * @details
 *
 *    Function : printPageList
 *
 *    Functionality: Print the Page Info List and UE Records
 *
 * @params[in] CmHashListCp *pagingInfoMap
 *
 * @return void
 *
 * ****************************************************************/
void printPageList(CmHashListCp *pagingInfoMap)
{
   uint8_t ret = ROK;
   DuPagInfoList *pagInfoLLFromPF = NULLP, *prevPageInfoLL = NULLP;

   do
   {
      ret = cmHashListGetNext(pagingInfoMap, (PTR)prevPageInfoLL, (PTR *)&pagInfoLLFromPF);
      if(ret == ROK)
      {
         DU_LOG("\nDEBUG  --> DUAPP: Page List for PF:%d",pagInfoLLFromPF->pf);
         handlePageInfoLL(NULLD, NULLD, &(pagInfoLLFromPF->pagInfoList), PRINT);
         prevPageInfoLL = pagInfoLLFromPF;
      }
   }while(ret == ROK);
   
}
#endif

/**********************************************************************
End of file
**********************************************************************/

