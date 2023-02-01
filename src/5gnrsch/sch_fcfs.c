
void schFcFsCellCfgReq(SchCellCb *cellCb)
{
   SchRrCellCb *rrCellCb = NULLP;
   
   SCH_ALLOC(rrCellCb, sizeof(SchRrCellCb));
   cellCb->schSpcCell = (void *)rrCellCb;
   cmLListInit(&cell->ueToBeScheduled);
}

void schFcFsCellDelReq(SchCellCb *cellCb)
{
   SchRrCellCb *rrCellCb = NULLP;
   CmLList *node=NULL, *next=NULL;

   rrCellCb = (SchRrCellCb *)cellCb->schSpcCell;
   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = rrCellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      SCH_FREE(node->node, sizeof(uint8_t));
      cmLListDelFrm(&rrCellCb->ueToBeScheduled, node);
      SCH_FREE(node, sizeof(CmLList));
      node = next;
   }
   SCH_FREE(rrCellCb, sizeof(SchRrCellCb));
   cellCb->schSpcCell = NULLP;
}

void schFcfsAllApisInit(SchAllApis *allFcfsApi)
{
    allFcfsApi->SchSlotInd = schFcfsProcessSlotInd;
    allFcfsApi->SchCellCfgReq = schFcFsCellCfgReq;
    allFcfsApi->SchCellDeleteReq = schFcFsCellDelReq;
    allFcfsApi->SchAddUeConfigReq = SchFcfsAddUeConfigReq;
    allFcfsApi->SchModUeConfigReq = SchFcfsModUeConfigReq;
    allFcfsApi->SchUeDeleteReq = SchFcfsUeDeleteReq;
    allFcfsApi->SchDlHarqInd = SchFcfsDlHarqInd;
    allFcfsApi->SchCrcInd = schFcfsProcessCrcInd;
    allFcfsApi->SchRachInd = schFcfsProcessRachInd;
    allFcfsApi->SchPagingInd = schFcfsPagingInd;
    allFcfsApi->SchRachRsrcReq = schFcfsRachRsrcReq; 
    allFcfsApi->SchRachRsrcRel = schFcfsRachRsrcRel;
    allFcfsApi->SchDlRlcBoInfo = schFcfsDlRlcBoInfo;
    allFcfsApi->SchSrUciInd = schFcfsSrUciInd;
    allFcfsApi->SchBsr = schFcfsBsr;
}
