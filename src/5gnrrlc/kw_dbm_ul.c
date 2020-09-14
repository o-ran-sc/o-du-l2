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
  
        Name:    RLC - Database module file
    
        Type:    C source file
  
        Desc:    Source code for Database Module functions such as, 

        File:    kw_dbm_ul.c
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="DBM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=194;


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"

/** 
 * @file gp_dbm_ul.c
 * @brief RLC Uplink database module
*/

#define RLC_MODULE RLC_DBGMASK_DUT


/**
 * @brief Handler to initialize hash list
 *       
 * @details
 *    This function initializes the UeCb, CellCb and transactions hash lists
 *
 * @param[in] gCb   RLC instance control block
 * 
 * @return  S16
 *     -# ROK 
 *     -# RFAILED 
*/
#ifdef ANSI
S16 rlcDbmUlInit
(
RlcCb *gCb
)
#else
S16 rlcDbmUlInit(gCb)
RlcCb *gCb;
#endif
{
   TRC3(rlcUlDbmInit)

   /* Initialize ueCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.ulCb->ueLstCp), 
                            (U16) RLC_UE_LIST_BUCKET_SIZE,
                            (U16) 0, 
                            (Bool) FALSE, 
                            (U16) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {
      RLOG0(L_ERROR, "UeLstCp Initialization Failed");
      return RFAILED;
   }

   /* Initialize cellCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.ulCb->cellLstCp), 
                            (U16) RLC_CELL_LIST_BUCKET_SIZE,
                            (U16) 0, 
                            (Bool) FALSE, 
                            (U16) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {
      cmHashListDeinit(&gCb->u.ulCb->ueLstCp);
      RLOG0(L_ERROR, "CellLstCp Initialization Failed");
      return RFAILED;
   }

   if(ROK != cmHashListInit(&(gCb->u.ulCb->transIdLstCp), 
                            (U16) RLC_TRANS_ID_LST_BKT_SIZE,
                            (U16) 0, 
                            (Bool) FALSE, 
                            (U16) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {
      cmHashListDeinit(&gCb->u.ulCb->ueLstCp);
      cmHashListDeinit(&gCb->u.ulCb->cellLstCp);
      RLOG0(L_ERROR, "transIdLstCp Initialization Failed");
      return RFAILED;
   }

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
   /* Initialize qcI Hash List */
#endif /* LTE_L2_MEAS */

   return (ROK);
} /* rlcDbmUlInit */


/**
 * @brief Handler to De initialize hash list
 *       
 * @param[in]    gCb      RLC Instance Control Block
 *
 * @return  Void
*/
#ifdef ANSI
Void rlcDbmUlDeInit
(
RlcCb *gCb
)
#else
Void rlcDbmUlDeInit(gCb)
RlcCb *gCb;
#endif
{
   TRC3(rlcDbmUlDeInit);


   /* De Initialize ueCb Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->ueLstCp));

   /* De Initialize cellCb Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->cellLstCp));

   /* De Initialize transaction Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->transIdLstCp));
/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS_RLC
   /* De Initialize qciCb Hash List */
   cmHashListDeinit(&(rlcCb.kwL2Cb.qciHlCp));
#endif /* LTE_L2_MEAS */

   RETVOID;
} /* kwDbmDeInit */


/**
 * @brief Handler to fetch rbCb by the rlcId
 *       
 * @details
 *    This function is invoked by CFG to fetch rbCb from the cellCb/ueCb in
 *    the upper interface (CKW/KWU).
 *
 * @param[in]    gCb     RLC Instance Control Block
 * @param[in]    rlcId   RLC Identifier 
 * @param[out]   rbCb    RB Control Block
 *
 * @return  Void
*/
#ifdef ANSI
Void rlcDbmFetchUlRbCbByRbId
(
RlcCb         *gCb,
CmLteRlcId   *rlcId,  
RlcUlRbCb     **rbCb  
)
#else
Void rlcDbmFetchUlRbCbByRbId(gCb, rlcId, rbCb)
RlcCb         *gCb;
CmLteRlcId   *rlcId;    
RlcUlRbCb     **rbCb;   
#endif
{
   TRC3(rlcDbmFetchUlRbCbByRbId)

   *rbCb= NULLP;

   /* Check for UE CB or CELL CB */
   if (rlcId->ueId == 0)
   {
      RlcUlCellCb *cellCb;

      if(rlcId->rbId >= RLC_MAX_RB_PER_CELL)
      {
         RLOG_ARG3(L_ERROR,DBG_RBID,rlcId->rbId ,
               "Invalid RbId, cellId:%d UEID:%d Max is [%d]",
               rlcId->cellId, 
               rlcId->ueId,
               RLC_MAX_RB_PER_CELL);
         RETVOID;
      }

      rlcDbmFetchUlCellCb(gCb,rlcId->cellId, &cellCb);
      if(!cellCb)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,rlcId->cellId,
               "CellCb not found RBID:%d UEID:%d",
               rlcId->rbId,
               rlcId->ueId);
         RETVOID;
      }

      *rbCb = cellCb->rbCb[rlcId->rbId];
   }
   else
   {
      RlcUlUeCb *ueCb;

      if (!(RLC_VALIDATE_UE_RBID(rlcId->rbType, rlcId->rbId)))
      {
         RLOG_ARG3(L_ERROR,DBG_RBID, rlcId->rbId,
               "Invalid RbId for RbType[%d] CELLID:%d UEID:%d", 
               rlcId->rbType,
               rlcId->cellId,
               rlcId->ueId);
         RETVOID;
      }

      if (rlcDbmFetchUlUeCb(gCb,rlcId->ueId, rlcId->cellId, &ueCb) != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID, rlcId->cellId,
               "UeId [%d]: UeCb not found RBID:%d",
               rlcId->ueId,
               rlcId->rbId);
         RETVOID;
      }

      RLC_DBM_GET_RBCB_FROM_UECB(rlcId->rbId, rlcId->rbType, ueCb, *rbCb);
   }
   RETVOID;
} /* rlcDbmFetchUlRbCbByRbId */


/**
 * @brief Handler to fetch rbCb from Local Channel Id 
 *
 * @details
 *    This function is invoked by CFG to fetch rbCb from the cellCb/ueCb
 *    from local Channel Id (CKW/KWU).
 *
 * @param[in]    gCb      -  RLC Instance Control Block
 * @param[in]    cellId   -  CELL Identifier 
 * @param[in]    ueId     -  UE Identifier 
 * @param[in]    lcId     -  Logical Channel Identifier
 * @param[out]   rbCb     -  RB Control Block
 *
 * @return  Void
 *
*/
#ifdef ANSI
Void rlcDbmFetchUlRbCbFromLchId
(
RlcCb          *gCb,
CmLteRnti     ueId,      
CmLteCellId   cellId,   
CmLteLcId     lcId,    
RlcUlRbCb      **rbCb  
)
#else
Void rlcDbmFetchUlRbCbFromLchId(gCb, ueId, cellId, lcId, rbCb)
RlcCb          *gCb;
CmLteRnti     ueId;     
CmLteCellId   cellId;  
CmLteLcId     lcId;   
RlcUlRbCb      **rbCb;
#endif
{
   RlcUlUeCb *ueCb;

   TRC3(rlcDbmFetchUlRbCbFromLchId)


   *rbCb = NULLP;
   
   /* Check for UE CB or CELL CB */
   if (ueId == 0)
   {
      RlcUlCellCb *cellCb;
      
      rlcDbmFetchUlCellCb(gCb,cellId, &cellCb);
      if(!cellCb)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellId,"CellCb not found UEID:%d",ueId);
         RETVOID;
      }

      *rbCb = cellCb->lCh[lcId - 1].ulRbCb;
      RETVOID;
   }
   
   if (rlcDbmFetchUlUeCb(gCb,ueId, cellId, &ueCb) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID, cellId,"UeId [%d]: UeCb not found",ueId);
      RETVOID;
   }

   *rbCb = ueCb->lCh[lcId - 1].ulRbCb;

   RETVOID;
} /* kwDbmFetchRbCbFromLchId */


/**
 * @brief Handler to delete RbCb
 *       
 * @details 
 *    This function is invoked by CFG to remove RbCb from Ue/Cell hashlist 
 *
 * @param[in] gCb       - RLC Instance Control Block
 * @param[in] rbCbLst   - Rb Cb List
 * @param[in] numRbCb   - Number of rbCbs
 *
 * @return  Void
 *
*/
#ifdef ANSI
Void rlcDbmDelAllUlRb
(
RlcCb       *gCb,
RlcUlRbCb   **rbCbLst,          
U8         numRbCb            
)
#else
Void rlcDbmDelAllUlRb(gCb, rbCbLst, numRbCb)
RlcCb       *gCb;
RlcUlRbCb   **rbCbLst;        
U8         numRbCb;         
#endif
{
   U32 idx; /* Index */

   TRC3(rlcDbmDelAllUlRb)


   for (idx = 0; idx < numRbCb; idx++)
   {
      if (rbCbLst[idx] != NULLP)
      {
         /* Free the Buffers of RbCb */
         if( CM_LTE_MODE_UM == rbCbLst[idx]->mode ) 
         {
            rlcUmmFreeUlRbCb(gCb,rbCbLst[idx]);
         }
         else if(CM_LTE_MODE_AM == rbCbLst[idx]->mode) 
         {
            rlcAmmFreeUlRbCb(gCb,rbCbLst[idx]);
         }
     
         RLC_FREE (gCb,rbCbLst[idx], sizeof (RlcUlRbCb));
      }
   }

   RETVOID;
} /* kwDbmDelAllRb */


/**
 * @brief  Handler to create an UeCb
 *
 * @details  
 *    This function is invoked by CFG to create UeCb and insert into the 
 *    Ue hashlist of RlcCb.
 *
 * @param[in] gCb       RLC Instance Control Block
 * @param[in] ueId      UE Identifier 
 * @param[in] cellId    Cell Identifier 
 * @param[in] ueCb      UE Control Block 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
S16 rlcDbmAddUlUeCb
(
RlcCb          *gCb,
CmLteRnti     ueId,       
CmLteCellId   cellId,    
RlcUlUeCb      *ueCb    
)
#else
S16 rlcDbmAddUlUeCb(gCb, ueId, cellId, ueCb)
RlcCb          *gCb;
CmLteRnti     ueId;    
CmLteCellId   cellId; 
RlcUlUeCb      *ueCb;
#endif
{
   TRC3(rlcDbmAddUlUeCb)


   ueCb->ueId = ueId;
   ueCb->cellId = cellId;

   if(ROK != cmHashListInsert(&(gCb->u.ulCb->ueLstCp), 
                              (PTR)ueCb, 
                              (U8 *)&(ueCb->ueId),
                              (U16) sizeof(CmLteRnti)))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellId,
            "UeId[%u] HashList Insertion Failed",
            ueId);
      return RFAILED;
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe++;


   return ROK;
} 


/**
 * @brief Handler to add a transaction 
 *
 * @details
 *    This function adds a transaction. 
 *
 * @param[in] gCb     RLC Instance Control Block
 * @param[in] cfg     Configuration information
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
S16 rlcDbmAddUlTransaction 
(
RlcCb        *gCb,
RlcUlCfgTmpData   *cfg
)
#else
S16 rlcDbmAddUlTransaction(gCb, cfg)
RlcCb        *gCb;
RlcUlCfgTmpData   *cfg;
#endif
{
   TRC3(rlcDbmAddUlTransaction)

#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG, "(transId(%ld)", cfg->transId);
#else
   RLOG1(L_DEBUG, "(transId(%d))", cfg->transId);
#endif

   return (cmHashListInsert(&(gCb->u.ulCb->transIdLstCp), 
                             (PTR)cfg, 
                             (U8 *)&(cfg->transId), 
                             (U16) sizeof(cfg->transId)));
} 


/**
 * @brief Handler to find a transaction 
 *       
 * @details
 *    This function find transaction using transaction Id 
 *
 *
 * @param[in] gCb       RLC Instance Control Block
 * @param[in] transId   Transaction Id 
 * @param[out] cfg      Configuration information attached to this transaction
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
S16 rlcDbmFindUlTransaction
(
RlcCb             *gCb,
U32              transId,
RlcUlCfgTmpData   **cfg
)
#else
S16 rlcDbmFindUlTransaction(gCb, cfg)
RlcCb             *gCb;
U32              transId;
RlcUlCfgTmpData   **cfg;
#endif
{
   TRC3(rlcDbmFindUlTransaction)

   if(ROK != cmHashListFind(&(gCb->u.ulCb->transIdLstCp),
                            (U8 *) &transId, 
                            sizeof (transId), 
                            RLC_DEF_SEQ_NUM,(PTR *) cfg))
   {
      RLOG1(L_ERROR,"TransId [%ld] not found",transId);
      return RFAILED;
   }
   return ROK;
}


/**
 *
 * @brief Handler to delete a transaction 
 *
 * @details
 *    This function deletes a transaction 
 *
 *
 *  @param[in] gCb     RLC Instance Control Block
 *  @param[in] cfg     Configuration information
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
S16 rlcDbmDelUlTransaction
(
RlcCb             *gCb,
RlcUlCfgTmpData   *cfg       
)
#else
S16 rlcDbmDelUlTransaction(gCb, cfg)
RlcCb             *gCb;
RlcUlCfgTmpData   *cfg;     
#endif
{
   TRC3(rlcDbmDelUlTransaction)


   if(cmHashListDelete(&(gCb->u.ulCb->transIdLstCp),(PTR) (cfg)) != ROK) 
   {
      RLOG0(L_ERROR,"HashList Deletion failed");
      return RFAILED;
   }

   return ROK;
}


/**
 * @brief Handler to delete all transaction 
 *
 * @details
 *    This function deletes all transaction 
 *
 * @param[in] gCb     RLC Instance Control Block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
S16 rlcDbmDelAllUlTransactions
(
RlcCb *gCb
)
#else
S16 rlcDbmDelAllUlTransactions(gCb)
RlcCb *gCb;
#endif
{
   RlcUlCfgTmpData *cfg = NULL;

   TRC3(kwDbmDelAllUlTransctions)

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->transIdLstCp), 
                                   (PTR) cfg, 
                                   (PTR *)&cfg))
   {
      if(rlcDbmDelUlTransaction(gCb, cfg) != ROK)
      {
         return RFAILED;
      }
      
      cfg = NULLP;
   }

   return ROK;
}


/**
 * @brief Handler to Fetch an UeCb
 *
 * @details
 *    This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *    of RlcCb.
 *
 * @param[in]    gCb       RLC Instance Control Block 
 * @param[in]    ueId      UE Identifier 
 * @param[in]    cellId    Cell Identifier 
 * @param[out]   ueCb      UE Control Block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
*/
#ifdef ANSI
S16 rlcDbmFetchUlUeCb
(
RlcCb          *gCb,
CmLteRnti     ueId,  
CmLteCellId   cellId, 
RlcUlUeCb      **ueCb   
)
#else
S16 rlcDbmFetchUlUeCb(gCb,ueId, cellId, ueCb)
RlcCb          *gCb;
CmLteRnti     ueId;     
CmLteCellId   cellId;    
RlcUlUeCb      **ueCb;     
#endif
{
   TRC3(rlcDbmFetchUlUeCb)


   return (cmHashListFind(&(gCb->u.ulCb->ueLstCp), 
                           (U8 *)&(ueId), sizeof(CmLteRnti),
                           RLC_DEF_SEQ_NUM, 
                           (PTR *) ueCb));
}


/**
 * @brief Handler to delete an UeCb
 *
 * @details
 *    This function is invoked by CFG to delete UeCb from the Ue hashlist 
 *    of RlcCb.
 *
 *
 * @param[in] gCb      RLC Instance Control Block
 * @param[in] ueCb     UE Control Block
 * @param[in] abrtFlag Abort Flag
 *
 * @return Void
 *
*/
#ifdef ANSI
Void rlcDbmDelUlUeCb
(
RlcCb       *gCb,
RlcUlUeCb   *ueCb,       
Bool       abortFlag   
)
#else
Void rlcDbmDelUlUeCb(gCb,eCb, abortFlag)
RlcCb       *gCb;
RlcUlUeCb   *ueCb;      
Bool       abortFlag; 
#endif
{
   TRC3(rlcDbmDelUlUeCb)


#if  (!defined(KW_PDCP) || !(defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))
   UNUSED(abortFlag);
#endif /* (!defined(KW_PDCP) || ! (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))*/

   /* Delete all logical channels */
   RLC_MEM_ZERO(ueCb->lCh,sizeof(RlcUlLch) * RLC_MAX_LCH_PER_UE);

   /* Delete all SRB RbCbs in UeCb */
   rlcDbmDelAllUlRb(gCb,ueCb->srbCb, RLC_MAX_SRB_PER_UE);

   /* Delete all DRB RbCbs in UeCb */
   rlcDbmDelAllUlRb(gCb,ueCb->drbCb, RLC_MAX_DRB_PER_UE);

   /* Delete ueCb entry from ueLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb))
   {
      RLOG_ARG1(L_ERROR,DBG_UEID,ueCb->ueId,
            "HashList Deletion Failed cellId(%d)",
            ueCb->cellId);
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe--;
   /* Deallocate ueCb */
   RLC_FREE(gCb,ueCb, sizeof(RlcUlUeCb));

   RETVOID;
}

/**
 * @brief Handler to delete all UeCbs
 *
 * @details
 *    This function is invoked by CFG to delete all UeCbs from the Ue 
 *    hashlist of RlcCb.
 *
 * @param[in] gCb      RLC Instance Control Block
 *
 * @return  Void
*/
#ifdef ANSI
Void rlcDbmDelAllUlUe
(
RlcCb *gCb
)
#else
Void rlcDbmDelAllUlUe(gCb)
RlcCb *gCb;
#endif
{
   RlcUlUeCb *ueCb = NULLP;  /* UE Control Block */

   TRC3(rlcDbmDelAllUlUe)


   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->ueLstCp), 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      rlcDbmDelUlUeCb(gCb,ueCb, TRUE);

      ueCb = NULLP;
   }

   RETVOID;
}


/**
 * @brief Handler to create CellCb
 *
 * @details
 *    This function is invoked by CFG to create CellCb and insert into
 *    the Cell hashlist of RlcCb.
 *
 *  @param[in] gCb      RLC Instance Control Block
 *  @param[in] cellId   Cell Identifier 
 *  @param[in] cellCb   Cell Control Block 
 *
 *  @return  S16
 *     -# ROK 
 *     -# RFAILED 
*/
#ifdef ANSI
S16 rlcDbmAddUlCellCb
(
RlcCb          *gCb,
CmLteCellId   cellId,    
RlcUlCellCb    *cellCb  
)
#else
S16 rlcDbmAddUlCellCb(gCb, cellId, cellCb)
RlcCb          *gCb;
CmLteCellId   cellId;    
RlcUlCellCb    *cellCb;  
#endif
{
   RlcUlCellCb *tCellCb; 

   TRC3(rlcDbmAddUlCellCb)



   tCellCb = cellCb;
   tCellCb->cellId = cellId;

   if(ROK != cmHashListInsert(&(gCb->u.ulCb->cellLstCp), 
                              (PTR) tCellCb,
                              (U8 *)&(tCellCb->cellId), 
                              (U16) sizeof(CmLteCellId)))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,tCellCb->cellId,
            "HashList Insertion Failed");
      return RFAILED;
   }

   return ROK;
} 


/**
 * @brief Handler to Fetch an CellCb
 *       
 * @details
 *    This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *    of RlcCb.
 *
 * @param[in]    gCb      RLC Instance Control Block
 * @param[in]    cellId   Cell Identifier 
 * @param[out]   cellCb   UE Control Block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
Void rlcDbmFetchUlCellCb
(
RlcCb          *gCb,
CmLteCellId   cellId,     
RlcUlCellCb    **cellCb   
)
#else
Void rlcDbmFetchUlCellCb(gCb, cellId, cellCb)
RlcCb          *gCb;
CmLteCellId   cellId;    
RlcUlCellCb    **cellCb;  
#endif
{
   TRC3(rlcDbmFetchUlCellCb)


   *cellCb = NULLP;
   if(ROK != cmHashListFind(&(gCb->u.ulCb->cellLstCp), 
                            (U8 *)&(cellId),sizeof(CmLteCellId), 
                            RLC_DEF_SEQ_NUM, (PTR*) cellCb))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"CellCb not found");
   }

   RETVOID;
}


/**
 * @brief Handler to delete CellCb
 *
 * @details
 *    This function is invoked by CFG to delete CellCb from the Cell hashlist 
 *    of RlcCb.
 *
 *  @param[in] gCb      RLC Instance Control Block
 *  @param[in] cellCb   Cell Control Block
 *
 *  @return  Void
*/
#ifdef ANSI
Void rlcDbmDelUlCellCb
(
RlcCb         *gCb,
RlcUlCellCb   *cellCb     
)
#else
Void rlcDbmDelUlCellCb(gCb, cellCb)
RlcCb         *gCb;
RlcUlCellCb   *cellCb;     
#endif
{
   TRC3(rlcDbmDelUlCellCb)


   /* Delete all rbCbs in cellCb */
   rlcDbmDelAllUlRb(gCb,cellCb->rbCb, RLC_MAX_RB_PER_CELL);

   /* Delete cellCb entry in hash list cellLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.ulCb->cellLstCp), (PTR) cellCb))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,
            "HashList Deletion Failed");
   }
   /* Deallocate cellCb */
   RLC_FREE(gCb, cellCb, sizeof(RlcUlCellCb));

   RETVOID;
} /* kwDbmDelCellCb */


/**
 * @brief Handler to delete all UeCbs
 *       
 * @details
 *    This function is invoked by CFG to delete all UeCbs from the Ue
 *    hashlist of RlcCb.
 * @param[in] gCb      RLC Instance Control Block
 *
 * @return  Void
*/
#ifdef ANSI
Void rlcDbmDelAllUlCell
(
RlcCb *gCb
)
#else
Void rlcDbmDelAllUlCell(gCb)
RlcCb *gCb;
#endif
{
   RlcUlCellCb *cellCb = NULLP; /* Cell Control Block */

   TRC3(rlcDbmDelAllUlCell)


   /* Until no more cellCb is ueLstCp hash list get and delete cellCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->cellLstCp), 
                                   (PTR) cellCb, 
                                   (PTR *)&cellCb))
   {
      rlcDbmDelUlCellCb(gCb,cellCb);

      cellCb = NULLP;
   }

   RETVOID;
} 


/**
 * @brief Handler to delete all UeCbs and CellCbs
 *       
 * @details
 *    This function is invoked by LMM to delete all UeCbs from the Ue
 *    hashlist of RlcCb and cellCbs from the Cell hashlist of rlcCb.
 * 
 * @param[in] gCb      RLC Instance Control Block
 *
*/
#ifdef ANSI
Void rlcDbmUlShutdown
(
RlcCb *gCb
)
#else
Void rlcDbmUlShutdown(gCb)
RlcCb *gCb;
#endif
{
   TRC3(rlcDbmUlShutdown)

   rlcDbmDelAllUlCell(gCb);

   rlcDbmDelAllUlUe(gCb);

   rlcDbmUlDeInit(gCb);

   RETVOID;
}


/********************************************************************30**
  
         End of file
**********************************************************************/
