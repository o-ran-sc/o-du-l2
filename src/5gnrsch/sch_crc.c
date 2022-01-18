#include "common_def.h"
#include "tfu.h"
#include "lrg.h"
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"

uint8_t schProcessCrcInd(CrcIndInfo *crcInd, Inst schInst)
{
   SchCellCb *cell = schCb[schInst].cells[crcInd->cellId];
   uint16_t count=0;
   uint8_t  ueId=0;
   CmLList         *node = NULLP;

   while(count<crcInd->numCrcInd)
   {
       GET_UE_ID(crcInd->crnti, ueId);
       if (cell->raCb[ueId].raState == SCH_RA_STATE_MSG3_PENDING)
       {
          SCH_ALLOC(node, sizeof(CmLList));
          node->node = (PTR)&(cell->raCb[ueId]);
          if (crcInd->crcInd[count])
          {
             /* failure case*/
             cmLListAdd2Tail( &(schCb[schInst].ulPrioLst[UL_MSG3_QUEUE]), node);
          }
          else
          {
             /* pass case*/
             cmLListAdd2Tail( &(schCb[schInst].ulPrioLst[UL_MSG3_RETX_QUEUE]), node);
          }
       }
       count++;
   }

   return ROK;
}
