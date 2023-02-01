
void schFcFsCellCfgReq(SchCellCb *cellCb)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   
   SCH_ALLOC(fcfsCellCb, sizeof(SchFcfsCellCb));
   cmLListInit(&fcfsCellCb->ueToBeScheduled);
   cellCb->schSpcCell = (void *)fcfsCellCb;
}

void schFcFsCellDelReq(SchCellCb *cellCb)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   CmLList *node=NULL, *next=NULL;

   fcfsCellCb = (SchFcfsCellCb *)cellCb->schSpcCell;
   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = fcfsCellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      SCH_FREE(node->node, sizeof(uint8_t));
      cmLListDelFrm(&fcfsCellCb->ueToBeScheduled, node);
      SCH_FREE(node, sizeof(CmLList));
      node = next;
   }
   SCH_FREE(fcfsCellCb, sizeof(SchFcfsCellCb));
   cellCb->schSpcCell = NULLP;
}

void SchFcfsAddUeConfigReq(SchUeCb *ueCb)
{
   SchFcfsUeCb *ueFcfsCb;

   SCH_ALLOC(ueFcfsCb, sizeof(SchFcfsHqCb));
   
   cmLListInit(&ueFcfsCb->hqReTxCb.ulRetxHqList);
   cmLListInit(&ueFcfsCb->hqReTxCb.dlRetxHqList);
   ueCb->schSpcUeCb = (void *)ueFcfsCb;
   
   return;
}

void SchFcfsModUeConfigReq(SchUeCb *ueCb)
{
   /*TBD: No action required for FCFS*/
   return;
}

void SchFcfsUeDeleteReq(SchUeCb *ueCb)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   SchFcfsUeCb *ueFcfsCb = NULLP;
   CmLList *node=NULL, *next=NULL;
   uint8_t ueId = 0;

   fcfsCellCb = (SchFcfsCellCb *)ueCb->cellCb->schSpcCell;
   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = fcfsCellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      ueId = *(uint8_t *)node->node;
      if(ueId == ueCb->ueId)
      {
        SCH_FREE(node->node, sizeof(uint8_t));
        cmLListDelFrm(&fcfsCellCb->ueToBeScheduled, node);
        SCH_FREE(node, sizeof(CmLList));
        break;
      }
      node = next;
   }
   
   ueFcfsCb = (SchFcfsUeCb *)ueCb->schSpcUeCb;
   cmLListDeleteLList(&ueFcfsCb->ulRetxHqList);
   cmLListDeleteLList(&ueFcfsCb->dlRetxHqList);

   SCH_FREE(ueFcfsCb, sizeof(SchFcfsUeCb));
   ueCb->schSpcUeCb = NULLP;
   return;
}

void schFcfsInitDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP;

   SCH_ALLOC(fcfsHqP, sizeof(SchFcfsHqProcCb));
   cmLListInit(&fcfsHqP->lcCb.dedLcList);
   cmLListInit(&fcfsHqP->lcCb.defLcList);
   hqP->schSpcDlHqProcCb = (void *)fcfsHqP;
}

void schFcfsInitUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP;

   SCH_ALLOC(fcfsHqP, sizeof(SchFcfsHqProcCb));
   cmLListInit(&fcfsHqP->lcCb.dedLcList);
   cmLListInit(&fcfsHqP->lcCb.defLcList);
   hqP->schSpcUlHqProcCb = (void *)fcfsHqP;
}

void schFcfsFreeDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP;
   fcfsHqP = hqP->scSpcDlHqProcCb;

   cmLListDeleteLList(&fcfsHqP->lcCb.dedLcList);
   cmLListDeleteLList(&fcfsHqP->lcCb.defLcList);

}

void schFcfsFreeUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP;
   fcfsHqP = hqP->schSpcUlHqProcCb;

   cmLListDeleteLList(&fcfsHqP->lcCb.dedLcList);
   cmLListDeleteLList(&fcfsHqP->lcCb.defLcList);
}

void schFcfsAddToDlHqReTxList(SchDlHqProcCb *hqP)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)hqP->hqEnt->ue->schSpcUeCb;
   cmLListAdd2Tail(&(fcfsUeCb->hqReTxCb.dlRetxHqList),&hqP->dlHqProcLink);  
#ifdef NR_DRX
   if(hqP->hqEnt->ue->ueDrxInfoPres == true)
   {
      schDrxStrtDlHqRttTmr(hqP);
   }
   else
#endif
   {
      schFcfsAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
   }
}

void schFcfsAddToUlHqReTxList(SchUlHqProcCb *hqP)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)hqP->hqEnt->ue->schSpcUeCb;
   cmLListAdd2Tail(&(fcfsUeCb->hqReTxCb.ulRetxHqList),&hqP->ulHqProcLink);  
#ifdef NR_DRX
   if(hqP->hqEnt->ue->ueDrxInfoPres == true)
   {
      schDrxStrtUlHqRttTmr(hqP);
   }
   else
#endif
   {
      schFcfsAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
   }   
}

/*******************************************************************
 *
 * @brief Add UE to ueToBeScheduled List
 *
 * @details
 *
 *    Function : addUeToBeScheduled
 *
 *    Functionality:
 *      Search if UE entry present in the list
 *      If yes, return.
 *      If no, add UE to the list
 *
 * @params[in] Cell control block
 *             Ue Idx to be added
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schFcfsAddUeToSchedule(SchCellCb *cellCb, uint8_t ueId)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   uint8_t *ueId;
   CmLList *node;

   fcfsCellCb = (SchFcfsCellCb *)cellCb->schSpcCell;
   /* Search if UE entry is already present in ueToBeScheduled list.
    * If yes, another entry for same UE not needed. Hence, return */
   node = fcfsCellCb->ueToBeScheduled.first;
   while(node)
   {
      ueId = (uint8_t *)node->node;
      if(*ueId == ueIdToAdd)
         return ROK;
      node = node->next;
   }

   /* If UE entry not present already, add UE to the end of ueToBeScheduled list */
   SCH_ALLOC(ueId, sizeof(uint8_t));
   if(!ueId)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failure in addUeToBeScheduled");
      return RFAILED;
   }
   *ueId = ueIdToAdd;
   if(addNodeToLList(&fcfsCellCb->ueToBeScheduled, ueId, NULLP) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : Failed to add ueId [%d] to cell->ueToBeScheduled list", *ueId);
      return RFAILED;
   }
   return ROK;
}

void schFcfsProcessCrcInd(SchCellCb *cellCb, uint8_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}


void schFcfsDlRlcBoInfo(SchCellCb *cellCb, uint8_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

void schFcfsBsr(SchCellCb *cellCb, uint8_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

void schFcfsSrUciInd(SchCellCb *cellCb, uint8_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

void schFcfsProcessRachInd(SchCellCb *cellCb, uint8_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

void SchFcfsDlHarqInd()
{
   return;
}

void schFcfsPagingInd()
{
   return;
}

void schFcfsRachRsrcReq()
{
   return;
}

void schFcfsRachRsrcRel()
{
   return;
}

void schFcfsRemoveFrmDlHqReTxList(SchUeCb *ueCb, CmLList *node)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)ueCb->schSpcUeCb;
   cmLListDelFrm(&fcfsUeCb->hqReTxCb.dlRetxHqList, node);
}

void schFcfsRemoveFrmUlHqReTxList(SchUeCb *ueCb, CmLList *node)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)ueCb->schSpcUeCb;
   cmLListDelFrm(&fcfsUeCb->hqReTxCb.ulRetxHqList, node);

}

void schFcfsRemoveUeFrmScheduleLst(SchCellCb *cell, CmLList *node)
{
   SchFcfsCellCb *fcfsCell;

   fcfsCell = (SchFcfsCellCb *)cell->schSpcCell;
   SCH_FREE(node->node, sizeof(uint8_t));
   deleteNodeFromLList(&fcfsCell->ueToBeScheduled, node);
}

/*******************************************************************
 *
 * @brief  Handler to calculate TBS size for BSR requested
 *
 * @details
 *
 *    Function :  schCalculateUlTbs
 *
 *    Functionality: Function will note the required TBS for each LCGIDX and use
 *    the Priority LCG List and RRM policy to allocate the TBS size
 *
 * @params [in] ueCb (Pointer to UE CB)
 *         [in] puschTime (Time slot where PUSCH will be sent)
 *         [in] symbLen (No of Symbols used for PUSCH transmission)
 *         [out] startPrb(Pointer to startPRB which will be calculated while
 *         finding the best Free Block)
 *         [out] totTBS(Pointer to total TBS size)
 *         [in] isRetx (to indicate retransmission)
 *         [in] hqP (UL Harq process pointer)
 *
 * @return uint8_t : ROK > Scheduling of UL grant is successful
 *                   RFAILED > vice versa
 *
 * ****************************************************************/
uint8_t schCalculateUlTbs(SchUeCb *ueCb, SlotTimingInfo puschTime, uint8_t symbLen,\
                          uint16_t *startPrb, uint32_t *totTBS, bool isRetx, SchUlHqProcCb *hqP)
{
   SchFcfsUeCb *ueFcfsCb = NULLP;
   uint16_t mcsIdx = 0;
   CmLListCp *lcLL = NULLP;
   uint16_t lcgIdx = 0, lcId =0, maxFreePRB = 0;
   uint16_t rsvdDedicatedPRB;
   *startPrb = 0;
   *totTBS = 0;

   ueFcfsCb = (SchFcfsUeCb *)ueCb->schSpcUeCb;
   /* check for BSR */
   for(lcgIdx=0; lcgIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcgIdx++)
   {
      if(ueCb->bsrInfo[lcgIdx].dataVol == 0)
      {
         continue;
      }

      /*TODO: lcgIdx and LCID has been implemented as one to one mapping.
       * Need to check the mapping to figure out the LCID and lcgIdx once L2
       * spec specifies any logic*/
      lcId = lcgIdx;
      if(ueCb->ulInfo.ulLcCtxt[lcId].isDedicated)
      {
         lcLL = &(ueFcfsCb->lcCb.dedLcList);
         rsvdDedicatedPRB = ueCb->ulInfo.ulLcCtxt[lcId].rsvdDedicatedPRB;
      }
      else
      {
         lcLL = &(ueFcfsCb->lcCb.defLcList);
      }

      /*[Step2]: Update the reqPRB and Payloadsize for this LC in the appropriate List*/
      if(updateLcListReqPRB(lcLL, lcId, ueCb->bsrInfo[lcgIdx].dataVol) != ROK)
      {
         DU_LOG("\nERROR  --> SCH: LcgId:%d updation failed",lcId);         
         return RFAILED;
      }
   }

   if ((ueFcfsCb->lcCb.defLcList.count == 0) && (hqP->ulLcPrbEst.dedLcList.count == 0))
   {
      if( (ueCb->srRcvd) || (isRetx) )
      {
         *startPrb = MAX_NUM_RB;
         *totTBS = schCalcTbSize(UL_GRANT_SIZE);
      }
      /*Returning true when NO Grant is there for UE as this is not scheduling
       * error*/      
      return ROK;
   }

   maxFreePRB = searchLargestFreeBlock(ueCb->cellCb, puschTime, startPrb, DIR_UL);

   /*[Step4]: Estimation of PRB and BO which can be allocated to each LC in
    * the list based on RRM policy*/

   /*Either this UE contains no reservedPRB pool fir dedicated S-NSSAI or 
    * Num of Free PRB available is not enough to reserve Dedicated PRBs*/
   if(maxFreePRB != 0)
   {
      mcsIdx = ueCb->ueCfg.ulModInfo.mcsIndex;
      if((hqP->ulLcPrbEst.dedLcList.count == 0) || ((maxFreePRB < rsvdDedicatedPRB)))
      {
         hqP->ulLcPrbEst.sharedNumPrb = maxFreePRB;
         DU_LOG("\nDEBUG  -->  SCH : UL Only Default Slice is scheduled, sharedPRB Count:%d",\
               hqP->ulLcPrbEst.sharedNumPrb);

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(hqP->ulLcPrbEst.defLcList), FALSE, mcsIdx, symbLen,\
               &(hqP->ulLcPrbEst.sharedNumPrb), NULLP, NULLP,&(ueCb->srRcvd));
      }
      else
      {
         hqP->ulLcPrbEst.sharedNumPrb = maxFreePRB - rsvdDedicatedPRB;

         /*PRB Alloc for Dedicated LCs*/
         prbAllocUsingRRMPolicy(&(hqP->ulLcPrbEst.dedLcList), TRUE, mcsIdx, symbLen,\
               &(hqP->ulLcPrbEst.sharedNumPrb), &(rsvdDedicatedPRB),\
               NULLP,&(ueCb->srRcvd));

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(hqP->ulLcPrbEst.defLcList), FALSE, mcsIdx, symbLen, \
               &(hqP->ulLcPrbEst.sharedNumPrb), &(rsvdDedicatedPRB),\
               NULLP,&(ueCb->srRcvd));
      }
   }
   /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
    * using allocated BO per LC and Update dlMsgAlloc(BO report for MAC*/ 
   if(hqP->ulLcPrbEst.dedLcList.count != 0)
      updateGrantSizeForBoRpt(&(hqP->ulLcPrbEst.dedLcList), NULLP, ueCb->bsrInfo, totTBS);

   updateGrantSizeForBoRpt(&(hqP->ulLcPrbEst.defLcList), NULLP, ueCb->bsrInfo, totTBS);

   /*Below case will hit if NO LC(s) are allocated due to resource crunch*/
   if (*totTBS == 0)
   {
      if(maxFreePRB == 0)
      {
         DU_LOG("\nERROR  --> SCH : NO FREE PRB!!");
      }
      else
      {
         /*Schedule the LC for next slot*/
         DU_LOG("\nDEBUG  -->  SCH : No LC has been scheduled");
      }      
      return RFAILED;
   }   
   return ROK;
}

/*******************************************************************
 *
 * @brief sch Process pending Sr or Bsr Req
 *
 * @details
 *
 *    Function : schProcessSrOrBsrReq
 *
 *    Functionality:
 *       sch Process pending Sr or Bsr Req
 *
 * @params[in] SchCellCb *cell,  SlotTimingInfo currTime
 * @params[in] uint8_t ueId, Bool isRetx, SchUlHqProcCb **hqP
 * @return true  - success
 *         false - failure
 *
 *******************************************************************/
bool schProcessSrOrBsrReq(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchUlHqProcCb **hqP)
{
   SchFcfsHqProcCb *fcfsHqProcCb;
   bool k2Found = FALSE;
   uint8_t ret = RFAILED;
   uint8_t startSymb = 0, symbLen = 0;
   uint8_t k2TblIdx = 0, k2Index = 0, k2Val = 0;
   uint16_t startPrb = 0;
   uint32_t totDataReq = 0; /* in bytes */
   SchUeCb *ueCb;
   SchPuschInfo *puschInfo;
   DciInfo  *dciInfo = NULLP;
   SchK2TimingInfoTbl *k2InfoTbl=NULLP;
   SlotTimingInfo dciTime, puschTime;

   fcfsHqProcCb = (SchFcfsHqProcCb *)(*hqP)->schSpcUlHqProcCb;
   if(cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH: schProcessSrOrBsrReq() : Cell is NULL");
      return false;
   }

   ueCb = &cell->ueCb[ueId-1];

   if(ueCb == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH: schProcessSrOrBsrReq() : UE is NULL");
      return false;
   }

   if (isRetx == FALSE)
   {
      if (schUlGetAvlHqProcess(cell, ueCb, hqP) != ROK)
      {
         return RFAILED;
      }
   }

   /* Calculating time frame to send DCI for SR */
   ADD_DELTA_TO_TIME(currTime, dciTime, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
#ifdef NR_TDD
   if(schGetSlotSymbFrmt(dciTime.slot, cell->slotFrmtBitMap) == DL_SLOT)
#endif
   {     
      if(ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.k2TblPrsnt)
         k2InfoTbl = &ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.k2InfoTbl;
      else
         k2InfoTbl =  &cell->cellCfg.schInitialUlBwp.k2InfoTbl;

      for(k2TblIdx = 0; k2TblIdx < k2InfoTbl->k2TimingInfo[dciTime.slot].numK2; k2TblIdx++)
      {
         k2Index = k2InfoTbl->k2TimingInfo[dciTime.slot].k2Indexes[k2TblIdx];

         if(!ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.k2TblPrsnt)
         {
            k2Val = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].k2;
            startSymb = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].startSymbol;
            symbLen = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].symbolLength;
         }
         else
         {
            k2Val = ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.puschCfg.timeDomRsrcAllocList[k2Index].k2;
            startSymb =  ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.puschCfg.timeDomRsrcAllocList[k2Index].startSymbol;
            symbLen =  ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.puschCfg.timeDomRsrcAllocList[k2Index].symbolLength;
         }
         /* Check for number of Symbol of PUSCH should be same as original in case of transmisson*/
         /* Calculating time frame to send PUSCH for SR */
         ADD_DELTA_TO_TIME(dciTime, puschTime, k2Val, cell->numSlots);
#ifdef NR_TDD
         if(schGetSlotSymbFrmt(puschTime.slot, cell->slotFrmtBitMap) == DL_SLOT)
            continue;
#endif
         if(cell->schUlSlotInfo[puschTime.slot]->puschUe != 0)
         {
            continue;
         }
         k2Found = true;
         if(hqP)
         {
            ADD_DELTA_TO_TIME(puschTime, (*hqP)->puschTime, 0, cell->numSlots);
         }
         break;
      }
   }
   
   if(k2Found == true)
   {
      ret = schCalculateUlTbs(ueCb, puschTime, symbLen, &startPrb, &totDataReq, isRetx, *hqP);
   
      if(totDataReq > 0 && ret == ROK)
      {
         SCH_ALLOC(dciInfo, sizeof(DciInfo));
         if(!dciInfo)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciInfo alloc");
            if(isRetx != TRUE)
            {
               if(fcfsHqProcCb->lcCb.dedLcList.count != 0)
                  updateBsrAndLcList(&(fcfsHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, RFAILED);

               updateBsrAndLcList(&(fcfsHqProcCb->lcCb.defLcList), ueCb->bsrInfo, RFAILED);
            }
            return false;
         }
         cell->schDlSlotInfo[dciTime.slot]->ulGrant = dciInfo;
         memset(dciInfo,0,sizeof(DciInfo));

         /* Update PUSCH allocation */
         if(schFillPuschAlloc(ueCb, puschTime, totDataReq, startSymb, symbLen, startPrb, isRetx, *hqP) == ROK)
         {
            if(cell->schUlSlotInfo[puschTime.slot]->schPuschInfo)
            {
               puschInfo = cell->schUlSlotInfo[puschTime.slot]->schPuschInfo;
               if(puschInfo != NULLP)
               {
                  /* Fill DCI for UL grant */
                  schFillUlDci(ueCb, puschInfo, dciInfo, isRetx, *hqP);
                  memcpy(&dciInfo->slotIndInfo, &dciTime, sizeof(SlotTimingInfo));
                  ueCb->srRcvd = false;
                  ueCb->bsrRcvd = false;
                  cell->schUlSlotInfo[puschTime.slot]->puschUe = ueId;
                  if(fcfsHqProcCb->lcCb.dedLcList.count != 0)
                     updateBsrAndLcList(&(fcfsHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, ROK);
                  updateBsrAndLcList(&(fcfsHqProcCb->lcCb.defLcList), ueCb->bsrInfo, ROK);
                  cmLListAdd2Tail(&(ueCb->hqUlmap[puschTime.slot]->hqList), &(*hqP)->ulSlotLnk);                  
                  return true;
               }
            }
         }
         if(fcfsHqProcCb->lcCb.dedLcList.count != 0)
            updateBsrAndLcList(&(fcfsHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, RFAILED);
         updateBsrAndLcList(&(fcfsHqProcCb->lcCb.defLcList), ueCb->bsrInfo, RFAILED);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : schProcessSrOrBsrReq(): K2 value is not found");
      return false;     
   }
   return true;
}

/*******************************************************************
 *
 * @brief 
 *
 * @details
 *
 *    Function : schFillBoGrantDlSchedInfo 
 *
 *    Functionality:
 
 *
 * @params[in] SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId
 * @params[in] bool isRetx, SchDlHqProcCb **hqP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
bool schFillBoGrantDlSchedInfo(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchDlHqProcCb **hqP)
{
   SchFcfsHqProcCb *fcfsHqProcCb;
   uint8_t lcIdx = 0;
   uint8_t pdschNumSymbols = 0, pdschStartSymbol = 0;
   uint16_t startPrb = 0, maxFreePRB = 0;
   uint16_t crnti = 0, mcsIdx = 0;
   uint32_t accumalatedSize = 0;
   SchUeCb *ueCb = NULLP;
   CmLListCp *lcLL = NULLP;
   DlMsgAlloc *dciSlotAlloc, *dlMsgAlloc;
   SlotTimingInfo pdcchTime, pdschTime, pucchTime;
   uint16_t rsvdDedicatedPRB = 0;

   /* TX_PAYLOAD_HDR_LEN: Overhead which is to be Added once for any UE while estimating Accumulated TB Size
    * Following flag added to keep the record whether TX_PAYLOAD_HDR_LEN is added to the first Node getting allocated.
    * If both Dedicated and Default LC lists are present then First LC in Dedicated List will include this overhead
    * else if only Default list is present then first node in this List will add this overhead len*/
   bool isTxPayloadLenAdded = FALSE;
   GET_CRNTI(crnti,ueId);
   ueCb = &cell->ueCb[ueId-1];

   if (isRetx == FALSE)
   {
      if(schDlGetAvlHqProcess(cell, ueCb, hqP) != ROK)
      {
         return false;
      }
   }

   fcfsHqProcCb = (SchFcfsHqProcCb *)hqP->schSpcDlHqProcCb;
   if(findValidK0K1Value(cell, currTime, ueId, ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.k0K1TblPrsnt,\
            &pdschStartSymbol, &pdschNumSymbols, &pdcchTime, &pdschTime, &pucchTime, isRetx, *hqP) != true )
   {
      /* If a valid combination of slots to scheduled PDCCH, PDSCH and PUCCH is
       * not found, do not perform resource allocation. Return from here. */
      return false;
   }
   
   /* allocate PDCCH and PDSCH resources for the ue */
   if(cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] == NULL)
   {

      SCH_ALLOC(dciSlotAlloc, sizeof(DlMsgAlloc));
      if(!dciSlotAlloc)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for ded DL msg alloc");
         return false;
      }
      cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = dciSlotAlloc;
      memset(dciSlotAlloc, 0, sizeof(DlMsgAlloc));
      dciSlotAlloc->crnti = crnti;
   }
   else
   {
      dciSlotAlloc = cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1];
   }
   /* Dl ded Msg info is copied, this was earlier filled in macSchDlRlcBoInfo */
   fillDlMsgInfo(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].dlMsgInfo, dciSlotAlloc->crnti, isRetx, *hqP);
   dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].isRetx = isRetx;


   if (isRetx == FALSE)
   {
      /*Re-Initalization per UE*/
      /* scheduled LC data fill */
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].numLc = 0;
      isTxPayloadLenAdded = FALSE; /*Re-initlaize the flag for every UE*/
      accumalatedSize = 0;

      for(lcIdx = 0; lcIdx < MAX_NUM_LC; lcIdx++)
      {
         if(ueCb->dlInfo.dlLcCtxt[lcIdx].bo)
         {
            /*Check the LC is Dedicated or default and accordingly LCList will
            * be used*/
            if(ueCb->dlInfo.dlLcCtxt[lcIdx].isDedicated)
            {
               lcLL = &(fcfsHqProcCb->lcCb.dedLcList);
               rsvdDedicatedPRB = ueCb->dlInfo.dlLcCtxt[lcIdx].rsvdDedicatedPRB;
            }
            else
            {
               lcLL = &(fcfsHqProcCb->lcCb.defLcList);
            }

            /*[Step2]: Update the reqPRB and Payloadsize for this LC in the appropriate List*/
            if(updateLcListReqPRB(lcLL, ueCb->dlInfo.dlLcCtxt[lcIdx].lcId,\
                     (ueCb->dlInfo.dlLcCtxt[lcIdx].bo + MAC_HDR_SIZE)) != ROK)
            {
               DU_LOG("\nERROR  --> SCH : Updation in LC List Failed");
               /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
               if(dciSlotAlloc->numSchedInfo == 0)
               {
                  SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
                  cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = NULL;
               }
               else
                  memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
               return false;
            }
         }
         ueCb->dlInfo.dlLcCtxt[lcIdx].bo = 0;
      }//End of for loop
      if ((fcfsHqProcCb->lcCb.defLcList.count == 0) && (fcfsHqProcCb->lcCb.dedLcList.count == 0))
      {
         DU_LOG("\nDEBUG  -->  SCH : No pending BO for any LC id\n");
         UNSET_ONE_BIT(ueId, cell->boIndBitMap);

         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         if(dciSlotAlloc->numSchedInfo == 0)
         {
            SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
            cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = NULL;
         }
         else
            memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));

         /*TRUE because this UE has nothing to be scheduled*/
         return true;
      }
   }

   /*[Step3]: Calculate Best FREE BLOCK with MAX PRB count*/
   maxFreePRB = searchLargestFreeBlock(cell, pdschTime, &startPrb, DIR_DL);

   /*[Step4]: Estimation of PRB and BO which can be allocated to each LC in
    * the list based on RRM policy*/

   /*Either this UE contains no reservedPRB pool fir dedicated S-NSSAI or 
    * Num of Free PRB available is not enough to reserve Dedicated PRBs*/
   if(isRetx == FALSE)
   {
      if(maxFreePRB != 0)
      {
         mcsIdx = ueCb->ueCfg.dlModInfo.mcsIndex;

         if((fcfsHqProcCb->lcCb.dedLcList.count == NULLP) 
               || ((maxFreePRB < rsvdDedicatedPRB)))
         { 
            fcfsHqProcCb->lcCb.sharedNumPrb = maxFreePRB;
            DU_LOG("\nDEBUG  --> SCH : DL Only Default Slice is scheduled, sharedPRB Count:%d",\
                  fcfsHqProcCb->lcCb.sharedNumPrb);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&(fcfsHqProcCb->lcCb.defLcList), FALSE, mcsIdx, pdschNumSymbols,\
                  &(fcfsHqProcCb->lcCb.sharedNumPrb), NULLP, &isTxPayloadLenAdded, NULLP);
         }
         else
         {
            fcfsHqProcCb->lcCb.sharedNumPrb = maxFreePRB - rsvdDedicatedPRB;
            /*PRB Alloc for Dedicated LCs*/
            prbAllocUsingRRMPolicy(&(fcfsHqProcCb->lcCb.dedLcList), TRUE, mcsIdx, pdschNumSymbols,\
                  &(fcfsHqProcCb->lcCb.sharedNumPrb), &(rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&((fcfsHqProcCb->lcCb.defLcList), FALSE, mcsIdx, pdschNumSymbols, \
                  &(fcfsHqProcCb->lcCb.sharedNumPrb), &(rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);
         }
      }
   }

   /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
    * using allocated BO per LC and Update dlMsgAlloc(BO report for MAC*/
   if (isRetx == FALSE)
   {
      if((fcfsHqProcCb->lcCb.dedLcList.count != 0)
         updateGrantSizeForBoRpt(&((fcfsHqProcCb->lcCb.dedLcList), dciSlotAlloc, NULLP, &(accumalatedSize));

      updateGrantSizeForBoRpt(&(fcfsHqProcCb->lcCb.defLcList), dciSlotAlloc, NULLP, &(accumalatedSize));
   }
   else
   {
      accumalatedSize = (*hqP)->tbInfo[0].tbSzReq;
   }

   /*Below case will hit if NO LC(s) are allocated due to resource crunch*/
   if (!accumalatedSize)
   {
      if(maxFreePRB == 0)
      {
         DU_LOG("\nERROR  --> SCH : NO FREE PRB!!");
      }
      else
      {
         /*Schedule the LC for next slot*/
         DU_LOG("\nDEBUG  -->  SCH : No LC has been scheduled");
      }
      /* Not Freeing dlMsgAlloc as ZERO BO REPORT to be sent to RLC so that
       * Allocation can be done in next slot*/
      return false;
   }

   /*[Step6]: pdcch and pdsch data is filled */
   if((schDlRsrcAllocDlMsg(cell, pdschTime, crnti, accumalatedSize, dciSlotAlloc, startPrb, pdschStartSymbol, pdschNumSymbols, isRetx, *hqP)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : Scheduling of DL dedicated message failed");

      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
      if(dciSlotAlloc->numSchedInfo == 0)
      {
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
         cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = NULL;
      }
      else
      {
         memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
      }
      return false;
   }

   /* TODO : Update the scheduling byte report for multiple LC based on QCI
    * and Priority */
   /* As of now, the total number of bytes scheduled for a slot is divided
    * equally amongst all LC with pending data. This is avoid starving of any
    * LC 
    * */
#if 0
   accumalatedSize = accumalatedSize/dlMsgAlloc->numLc;
   for(lcIdx = 0; lcIdx < dlMsgAlloc->numLc; lcIdx ++)
      dlMsgAlloc->lcSchInfo[lcIdx].schBytes = accumalatedSize;
#endif
   
   /* Check if both DCI and DL_MSG are sent in the same slot.
    * If not, allocate memory for DL_MSG PDSCH slot to store PDSCH info */

   if(pdcchTime.slot == pdschTime.slot)
   {
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pduPres = BOTH;
      dciSlotAlloc->numSchedInfo++;
   }
   else
   {
      /* Allocate memory to schedule dlMsgAlloc to send DL_Msg, pointer will be checked at schProcessSlotInd() */
      if(cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] == NULLP)
      {
         SCH_ALLOC(dlMsgAlloc, sizeof(DlMsgAlloc));
         if(dlMsgAlloc == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dlMsgAlloc");
            if(dciSlotAlloc->numSchedInfo == 0)
            {
               SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
               cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = NULLP;
            }
            else
               memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
            return false;
         }
         cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] = dlMsgAlloc;
         memset(dlMsgAlloc, 0, sizeof(DlMsgAlloc));
         dlMsgAlloc->crnti = dciSlotAlloc->crnti;
      }
      else
         dlMsgAlloc = cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1];

      /* Copy all DL_MSG info */
      memcpy(&dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo], \
            &dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], sizeof(DlMsgSchInfo));
      dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].dlMsgPdcchCfg.dci.pdschCfg = \
            &dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].dlMsgPdschCfg;

      /* Assign correct PDU types in corresponding slots */
      dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].pduPres = PDSCH_PDU;
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pduPres = PDCCH_PDU;
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pdschSlot = pdschTime.slot;

      dciSlotAlloc->numSchedInfo++;
      dlMsgAlloc->numSchedInfo++;
   }

   schAllocPucchResource(cell, pucchTime, crnti, ueCb, isRetx, *hqP);

   cell->schDlSlotInfo[pdcchTime.slot]->pdcchUe = ueId;
   cell->schDlSlotInfo[pdschTime.slot]->pdschUe = ueId;
   cell->schUlSlotInfo[pucchTime.slot]->pucchUe = ueId;

   /* after allocation is done, unset the bo bit for that ue */
   UNSET_ONE_BIT(ueId, cell->boIndBitMap);
   return true;
}

void schFcfsDlScheduling(SchCellCb *cell, SlotTimingInfo *slotInd, Inst schInst)
{
   SchFcfsCellCb *fcfsCell;
   SchFcfsUeCb   *fcfsUeCb;
   SchDlHqProcCb  *hqP = NULLP;
   CmLList        *pendingUeNode;
   CmLList        *node;
   uint8_t         ueId;
   uint8_t*       ueNode;
   bool      isRarPending = false, isRarScheduled = false;
   bool      isMsg4Pending = false, isMsg4Scheduled = false;

   fcfsCell = (SchFcfsCellCb *)cell->schSpcCell;
   fcfsUeCb = (SchFcfsUeCb *)cell->ueCb[ueId-1]->schSpcUeCb;
   
   /* Select first UE in the linked list to be scheduled next */
   pendingUeNode = fcfsCell->ueToBeScheduled.first;
   if(pendingUeNode)
   {
      if(pendingUeNode->node)
      {
         ueNode = (uint8_t *)pendingUeNode->node;
         ueId = *(uint8_t *)(pendingUeNode->node);
         /* If RAR is pending for this UE, schedule PDCCH,PDSCH to send RAR and 
          * PUSCH to receive MSG3 as per k0-k2 configuration*/
         if(cell->raReq[ueId-1] != NULLP)
         {
            isRarPending = true;
            isRarScheduled = schProcessRaReq(schInst, cell, *slotInd, ueId);
         }

         /*MSG3 retransmisson*/
         if(cell->raCb[ueId-1].retxMsg3HqProc)
         {            
            schMsg3RetxSchedulingForUe(&(cell->raCb[ueId-1]));
         }

         /* If MSG4 is pending for this UE, schedule PDCCH,PDSCH to send MSG4 and
          * PUCCH to receive UL msg as per k0-k1 configuration  */
         if (cell->ueCb[ueId-1].retxMsg4HqProc) //should work from dlmap later tbd
         {
            /* Retransmission of MSG4 */
            isMsg4Pending = true;
            if(schProcessMsg4Req(cell, *slotInd, ueId, TRUE, &cell->ueCb[ueId-1].retxMsg4HqProc) == ROK)
               isMsg4Scheduled = true;
         }
         else
         {
            /* First transmission of MSG4 */
            if(cell->raCb[ueId-1].msg4recvd)
            {
               isMsg4Pending = true;
               if(schProcessMsg4Req(cell, *slotInd, ueId, FALSE, &cell->ueCb[ueId-1].msg4Proc) == ROK)
                  isMsg4Scheduled = true;

               /* If MSG4 scheduling failed, free the newly assigned HARQ process */
               if(!isMsg4Scheduled)
                  schDlReleaseHqProcess(cell->ueCb[ueId-1].msg4Proc);
            }
         }

         if(isRarPending || isMsg4Pending)
         {
            /* If RAR or MSG is successfully scheduled then
             * remove UE from linked list since no pending msgs for this UE */
            if(isRarScheduled || isMsg4Scheduled)
            {
               schFcfsRemoveUeFrmScheduleLst(cell, pendingUeNode);
            }
            /* If RAR/MSG4 is pending but couldnt be scheduled then,
             * put this UE at the end of linked list to be scheduled later */
            else
            {
               cmLListAdd2Tail(&fcfsCell->ueToBeScheduled, cmLListDelFrm(&fcfsCell->ueToBeScheduled, pendingUeNode));
            }
         }

#ifdef NR_DRX 
         if((cell->ueCb[ueId-1].ueDrxInfoPres == true) && (cell->ueCb[ueId-1].drxUeCb.drxDlUeActiveStatus != true))
         {
            if(pendingUeNode->node)
            {
               cmLListAdd2Tail(&fcfsCell->ueToBeScheduled, cmLListDelFrm(&fcfsCell->ueToBeScheduled, pendingUeNode));
            }
         }
         else 
#endif
         {

            /* DL Data */
            node = fcfsUeCb->hqReTxCb.dlRetxHqList.first;
            if(node != NULLP)
            {
               /* DL Data ReTransmisson */
               isDlMsgPending = true;
               isDlMsgScheduled = schFillBoGrantDlSchedInfo(cell, *slotInd, ueId, TRUE, ((SchDlHqProcCb**) &(node->node)));
               if(isDlMsgScheduled)
               {
#ifdef NR_DRX 
                  schDrxStopDlHqRetxTmr(cell, &cell->ueCb[ueId-1], ((SchDlHqProcCb**) &(node->node)));
#endif
                  schFcfsRemoveFrmDlHqReTxList(&cell->ueCb[ueId-1], node);
               }
            }
            else
            {
               /* DL Data new transmission */
               if((cell->boIndBitMap) & (1<<ueId))
               {
                  isDlMsgPending = true;               
                  isDlMsgScheduled = schFillBoGrantDlSchedInfo(cell, *slotInd, ueId, FALSE, &hqP);

                  /* If DL scheduling failed, free the newly assigned HARQ process */
                  if(!isDlMsgScheduled)
                     schDlReleaseHqProcess(hqP);
                  else
                  {
#ifdef NR_DRX
                     schHdlDrxInActvStrtTmr(cell, &cell->ueCb[ueId-1], PHY_DELTA_DL + SCHED_DELTA);
#endif
                  }
               }
            }

            /* Scheduling of UL grant */
            node = fcfsUeCb->hqReTxCb.ulRetxHqList.first;
            if(node != NULLP)
            {
               /* UL Data ReTransmisson */
               isUlGrantPending = true;
               isUlGrantScheduled = schProcessSrOrBsrReq(cell, *slotInd, ueId, TRUE, (SchUlHqProcCb**) &(node->node));
               if(isUlGrantScheduled)
               {
#ifdef NR_DRX 
                  schDrxStopUlHqRetxTmr(cell, &cell->ueCb[ueId-1], ((SchUlHqProcCb**) &(node->node)));
#endif
                  schFcfsRemoveFrmUlHqReTxList(&cell->ueCb[ueId-1], node);
               }
            }
            else
            {
               /* UL Data new transmission */
               if(cell->ueCb[ueId-1].srRcvd || cell->ueCb[ueId-1].bsrRcvd)
               {
                  isUlGrantPending = true;
                  isUlGrantScheduled = schProcessSrOrBsrReq(cell, *slotInd, ueId, FALSE, &ulHqP);
                  if(!isUlGrantScheduled)
                     schUlReleaseHqProcess(ulHqP, FALSE);
                  else
                  {
#ifdef NR_DRX
                     schHdlDrxInActvStrtTmr(cell, &cell->ueCb[ueId-1], PHY_DELTA_UL + SCHED_DELTA);
#endif
                  }
               }
            }

            if(!isUlGrantPending && !isDlMsgPending)
            {
               /* No action required */  
            }
            else if((isUlGrantPending && !isUlGrantScheduled) || (isDlMsgPending && !isDlMsgScheduled))
            {
               cmLListAdd2Tail(&fcfsCell->ueToBeScheduled, cmLListDelFrm(&fcfsCell->ueToBeScheduled, pendingUeNode));
            }
            else
            {
               schFcfsRemoveUeFrmScheduleLst(cell, pendingUeNode);
            }
         }
      }
   }
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
    allFcfsApi->SchAddToDlHqReTxList = schFcfsAddToDlHqReTxList;
    allFcfsApi->SchAddToUlHqReTxList = schFcfsAddToUlHqReTxList;
    allFcfsApi->SchRemoveFrmDlHqReTxList = schFcfsRemoveFrmDlHqReTxList
    allFcfsApi->SchRemoveFrmUlHqReTxList = schFcfsRemoveFrmUlHqReTxList
    allFcfsApi->SchAddUeToSchedule = schFcfsAddUeToSchedule;
    allFcfsApi->SchRemoveUeFrmScheduleLst = schFcfsRemoveUeFrmScheduleLst;
    allFcfsApi->SchInitDlHqProcCb = schFcfsInitDlHqProcCb;
    allFcfsApi->SchInitUlHqProcCb = schFcfsInitUlHqProcCb;
    allFcfsApi->SchFreeDlHqProcCb = schFcfsFreeDlHqProcCb;
    allFcfsApi->SchFreeUlHqProcCb = schFcfsFreeUlHqProcCb;
    allFcfsApi->SchDlScheduling = schFcfsDlScheduling;
}
