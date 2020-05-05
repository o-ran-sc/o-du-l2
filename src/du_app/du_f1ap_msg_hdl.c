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

/* This file contains F1AP message handler functions */

#include "du_mgr.h"
#include "du_mgr_main.h"
#include "du_cell_mgr.h"
#include "du_f1ap_msg_hdl.h"
#include "GNB-DU-System-Information.h"

extern char encBuf[ENC_BUF_MAX_LEN];
extern DuCfgParams duCfgParam;
S16 sctpSend(Buffer *mBuf, U8 itfType);
/*******************************************************************
 *
 * @brief Builds Uplink Info for NR 
 *
 * @details
 *
 *    Function : BuildULNRInfo
 *
 *    Functionality: Building NR Uplink Info
 *
 * @params[in] NRFreqInfo_t *ulnrfreq
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildULNRInfo(NRFreqInfo_t *ulnrfreq)
{
   U8 idx;
   idx = 0;
   ulnrfreq->nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.\
                       fdd.ulNrFreqInfo.nrArfcn;
   ulnrfreq->freqBandListNr.list.count = 1;
   ulnrfreq->freqBandListNr.list.size = sizeof(struct FreqBandNrItem *);
   DU_ALLOC(ulnrfreq->freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
   if(ulnrfreq->freqBandListNr.list.array == NULLP)
   {
      RETVALUE(RFAILED);
   }
   DU_ALLOC(ulnrfreq->freqBandListNr.list.array[idx],sizeof(struct FreqBandNrItem));
   if(ulnrfreq->freqBandListNr.list.array[idx] == NULLP)
   {
      DU_FREE(ulnrfreq->freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
      RETVALUE(RFAILED);
   }
      ulnrfreq->freqBandListNr.list.array[idx]->freqBandIndicatorNr = \
      duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.\
      freqBand[0].nrFreqBand;
      ulnrfreq->freqBandListNr.list.array[idx]->supportedSULBandList.list.count = SUL_BAND_COUNT;
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Downlink NR Info 
 *
 * @details
 *
 *    Function : BuildDLNRInfo
 *
 *    Functionality: Building Downlink NR Info
 *    
 * @params[in] NRFreqInfo_t *dlnrfreq
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildDLNRInfo(NRFreqInfo_t *dlnrfreq)
{
   U8 idx=0;
   dlnrfreq->nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.\
                       fdd.dlNrFreqInfo.nrArfcn;
   dlnrfreq->freqBandListNr.list.count = 1;
   dlnrfreq->freqBandListNr.list.size = sizeof(struct FreqBandNrItem *);
   DU_ALLOC(dlnrfreq->freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
   if(dlnrfreq->freqBandListNr.list.array == NULLP)
   {
      RETVALUE(RFAILED);
   }
   DU_ALLOC(dlnrfreq->freqBandListNr.list.array[idx],sizeof(struct FreqBandNrItem));
   if(dlnrfreq->freqBandListNr.list.array[idx] == NULLP)
   {
      DU_FREE(dlnrfreq->freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
      RETVALUE(RFAILED);
   }
   dlnrfreq->freqBandListNr.list.array[idx]->freqBandIndicatorNr = \
   duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.\
   freqBand[0].nrFreqBand;
   dlnrfreq->freqBandListNr.list.array[idx]->supportedSULBandList.list.count = SUL_BAND_COUNT;
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Nrcgi 
 *
 * @details
 *
 *    Function : BuildNrcgi
 *
 *    Functionality: Building the PLMN ID and NR Cell id
 *
 * @params[in] NRCGI_t *nrcgi
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildNrcgi(NRCGI_t *nrcgi)
{
   S16 ret;
   U8 unused = 4;
   U8 byteSize = 5;
   U8 val = 16;

   /* Allocate Buffer Memory */
   nrcgi->pLMN_Identity.size = PLMN_SIZE * sizeof(U8);
   DU_ALLOC(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   ret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
         &nrcgi->pLMN_Identity); // Building PLMN function 
   if(ret != ROK)
   {
      return ret;
   }
   /*nrCellIdentity*/
   //ret = BuildNrCellId(&nrcgi->nRCellIdentity);
   nrcgi->nRCellIdentity.size = byteSize * sizeof(U8);
   DU_ALLOC(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
	if(nrcgi->nRCellIdentity.buf == NULLP)
   {
	    return RFAILED;
   }
   ret = fillBitString(&nrcgi->nRCellIdentity, unused, byteSize, val);

   if(ret != ROK)
   {
      DU_FREE(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size);
      return ret;
   }

   return ROK;
}
/*******************************************************************
 *
 * @brief Builds FiveGStac 
 *
 * @details
 *
 *    Function : BuildFiveGSTac
 *
 *    Functionality: Building the FiveGSTac
 *
 * @params[in] OCTET_STRING_t *fivegsTac
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildFiveGSTac(Served_Cell_Information_t *servcell)
{
   DU_ALLOC(servcell->fiveGS_TAC,sizeof(FiveGS_TAC_t));
   if(servcell->fiveGS_TAC == NULLP)
   {
      RETVALUE(RFAILED);
   }
   servcell->fiveGS_TAC->size = 3 * sizeof(U8);
   DU_ALLOC(servcell->fiveGS_TAC->buf,\
         sizeof(servcell->fiveGS_TAC->size));
   if(servcell->fiveGS_TAC->buf == NULLP)
   {
      DU_FREE(servcell->fiveGS_TAC,sizeof(FiveGS_TAC_t));
      RETVALUE(RFAILED);
   }
   servcell->fiveGS_TAC->buf[0] = 0;
   servcell->fiveGS_TAC->buf[1] = 0;
   servcell->fiveGS_TAC->buf[2] = duCfgParam.srvdCellLst[0].duCellInfo.tac;
   return ROK;  
}
/*******************************************************************
 *
 * @brief Builds NR Mode 
 *
 * @details
 *
 *    Function : BuildNrMode
 *
 *    Functionality: Building the NR Mode
 *
 * @params[in] NR_Mode_Info_t *fdd
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildNrMode(NR_Mode_Info_t *mode)
{
   S16 ret;
   /* FDD Mode */
   mode->present = NR_Mode_Info_PR_fDD;
   if(mode->present == NR_Mode_Info_PR_fDD)
   {
      DU_ALLOC(mode->choice.fDD,\
            sizeof(FDD_Info_t));
      if(mode->choice.fDD == NULLP)
      {
         RETVALUE(RFAILED);
      }
      ret = BuildULNRInfo(&mode->choice.fDD->uL_NRFreqInfo);
      if(ret != ROK)
      {
         DU_FREE(mode->choice.fDD,\
               sizeof(FDD_Info_t));
         RETVALUE(RFAILED);
      }
      ret = BuildDLNRInfo(&mode->choice.fDD->dL_NRFreqInfo);
      if(ret != ROK)
      {
         DU_FREE(mode->choice.fDD,\
               sizeof(FDD_Info_t));
         DU_FREE(mode->choice.fDD->uL_NRFreqInfo.\
               freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
         DU_FREE(mode->choice.fDD->uL_NRFreqInfo.\
               freqBandListNr.list.array[0],sizeof(struct FreqBandNrItem));
         RETVALUE(RFAILED);
      }
   }
   mode->choice.fDD->uL_Transmission_Bandwidth.nRSCS = \
                                    duCfgParam.srvdCellLst[0].duCellInfo.\
                                    f1Mode.mode.fdd.ulTxBw.nrScs;
   mode->choice.fDD->uL_Transmission_Bandwidth.nRNRB = \
                                    duCfgParam.srvdCellLst[0].duCellInfo.\
                                    f1Mode.mode.fdd.ulTxBw.nrb;

   mode->choice.fDD->dL_Transmission_Bandwidth.nRSCS = \
                                    duCfgParam.srvdCellLst[0].duCellInfo.\
                                    f1Mode.mode.fdd.dlTxBw.nrScs;
   mode->choice.fDD->dL_Transmission_Bandwidth.nRNRB = \
                                    duCfgParam.srvdCellLst[0].duCellInfo.\
                                    f1Mode.mode.fdd.dlTxBw.nrb;
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds IE Extensions for Served PLMNs 
 *
 * @details
 *
 *    Function : BuildExtensions
 *
 *    Functionality: Building the IE Extensions
 *
 * @params[in] struct ProtocolExtensionContainer_4624P3 *buildextend
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildExtensions(struct ProtocolExtensionContainer_4624P3 **ieExtend)
{
   U8 idx;
   U8 plmnidx;
   U8 extensionCnt=1;
   U8 extensionId;
   U8 sliceId;
   U8 sdId;
   DU_ALLOC(*ieExtend,\
         sizeof(struct ProtocolExtensionContainer_4624P3));
   if((*ieExtend) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   (*ieExtend)->list.count = extensionCnt;
   (*ieExtend)->list.size = \
                            extensionCnt * sizeof(struct ServedPLMNs_ItemExtIEs *);
   DU_ALLOC((*ieExtend)->list.array,\
         extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *)); 
   if((*ieExtend)->list.array == NULLP)
   {
      DU_FREE((*ieExtend),sizeof(struct ProtocolExtensionContainer_4624P3));
      RETVALUE(RFAILED);
   }
   for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
   {
      DU_ALLOC((*ieExtend)->list.array[plmnidx],\
            sizeof(struct ServedPLMNs_ItemExtIEs));
      if((*ieExtend)->list.array[plmnidx] == NULLP)
      {
         for(extensionId=0 ; extensionId<plmnidx; extensionId++)
         {
            DU_FREE((*ieExtend)->list.array[extensionId],\
                  sizeof(struct ServedPLMNs_ItemExtIEs));
         }
         DU_FREE((*ieExtend)->list.array,\
               extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *)); 
         DU_FREE((*ieExtend),sizeof(struct ProtocolExtensionContainer_4624P3));
         RETVALUE(RFAILED);
      }
   }
      idx = 0;
      (*ieExtend)->list.array[idx]->id = ProtocolIE_ID_id_TAISliceSupportList;
      (*ieExtend)->list.array[idx]->criticality = Criticality_ignore;
      (*ieExtend)->list.array[idx]->extensionValue.present = \
                 ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList;
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
      list.count = 1;
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
                 list.size = sizeof(SliceSupportItem_t *);
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof(SliceSupportItem_t *));
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array == NULLP)
      {
         for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
         {
            DU_FREE((*ieExtend)->list.array[plmnidx],\
                  sizeof(struct ServedPLMNs_ItemExtIEs));
         }
            DU_FREE((*ieExtend)->list.array,\
                  extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *)); 
            DU_FREE((*ieExtend),sizeof(struct ProtocolExtensionContainer_4624P3));
            RETVALUE(RFAILED);
         
      }
      sliceId = 0;
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId],sizeof(SliceSupportItem_t));
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId] == NULLP) 
      {
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array,sizeof(SliceSupportItem_t *));
         for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
         {
            DU_FREE((*ieExtend)->list.array[plmnidx],\
                  sizeof(struct ServedPLMNs_ItemExtIEs));
         }
         DU_FREE((*ieExtend)->list.array,\
               extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *)); 
         DU_FREE((*ieExtend),sizeof(struct ProtocolExtensionContainer_4624P3));
         RETVALUE(RFAILED);
      }
     
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sST.size = sizeof(U8);
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList\
            .list.array[sliceId]->sNSSAI.sST.buf,(*ieExtend)->list.array[idx]->\
            extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sST.size);
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList\
            .list.array[sliceId]->sNSSAI.sST.buf == NULLP)
      {
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array[sliceId],sizeof(SliceSupportItem_t));
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array,sizeof(SliceSupportItem_t *));
         for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
         {
            DU_FREE((*ieExtend)->list.array[plmnidx],\
                  sizeof(struct ServedPLMNs_ItemExtIEs));
         }
         DU_FREE((*ieExtend)->list.array,\
               extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *)); 
         DU_FREE((*ieExtend),sizeof(struct ProtocolExtensionContainer_4624P3));
         RETVALUE(RFAILED);
      }
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sST.buf[0] = 3;
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD == NULLP)
      {
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList\
               .list.array[sliceId]->sNSSAI.sST.buf,(*ieExtend)->list.array[idx]->\
               extensionValue.choice.SliceSupportList.list.array[sliceId]->sNSSAI.sST.size);
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array[sliceId],sizeof(SliceSupportItem_t));
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array,sizeof(SliceSupportItem_t *));
         for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
         {
            DU_FREE((*ieExtend)->list.array[plmnidx],\
                  sizeof(struct ServedPLMNs_ItemExtIEs));
         }
         DU_FREE((*ieExtend)->list.array,\
               extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *)); 
         DU_FREE((*ieExtend),sizeof(struct ProtocolExtensionContainer_4624P3));
         RETVALUE(RFAILED);
      }
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->size = 3*sizeof(U8);
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD->buf,(*ieExtend)->list.array[idx]->extensionValue.choice.\
            SliceSupportList.list.array[sliceId]->sNSSAI.sD->size);
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD->buf == NULLP)
      {
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array[sliceId]->sNSSAI.sD,sizeof(OCTET_STRING_t));
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList\
               .list.array[sliceId]->sNSSAI.sST.buf,(*ieExtend)->list.array[idx]->extensionValue.\
               choice.SliceSupportList.list.array[sliceId]->sNSSAI.sST.size);
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array[sliceId],sizeof(SliceSupportItem_t));
         DU_FREE((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
               list.array,sizeof(SliceSupportItem_t *));
         for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
         {
            DU_FREE((*ieExtend)->list.array[plmnidx],\
                  sizeof(struct ServedPLMNs_ItemExtIEs));
         }
         DU_FREE((*ieExtend)->list.array,\
               extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *)); 
         DU_FREE((*ieExtend),sizeof(struct ProtocolExtensionContainer_4624P3));
         RETVALUE(RFAILED);
      }
      sdId = 0;
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->buf[sdId] = 3;
      sdId++;
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->buf[sdId] = 6;
      sdId++;
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->buf[sdId] = 9;
       return ROK;
}
/*******************************************************************
 *
 * @brief Builds Served PLMN 
 *
 * @details
 *
 *    Function : BuildServedPlmn
 *
 *    Functionality: Building the Served PLMN
 *
 * @params[in] GNB_DU_Served_Cells_Item_t *srvCellItem
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16  BuildServedPlmn(ServedPLMNs_List_t *srvplmn)
{  
   S16 ret;
   U8  plmnidx;
   U8  servPlmnid;
   U8  servPlmnCnt=1;
   srvplmn->list.count = servPlmnCnt;
   srvplmn->list.size = \
                      servPlmnCnt*sizeof(struct ServedPLMNs_Item *);
   DU_ALLOC(srvplmn->list.array,\
         servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
   if(srvplmn->list.array == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for(plmnidx=0; plmnidx<servPlmnCnt; plmnidx++)
   {
      DU_ALLOC(srvplmn->list.array[plmnidx],\
            sizeof(struct ServedPLMNs_Item));
      if(srvplmn->list.array[plmnidx] == NULLP)
      {
         for(servPlmnid=0; servPlmnid<plmnidx;servPlmnid++)
         {
            DU_FREE(srvplmn->list.array[servPlmnid],\
                  sizeof(struct ServedPLMNs_Item));
         }
         DU_FREE(srvplmn->list.array,\
               sizeof(struct ServedPLMNs_Item *));
         RETVALUE(RFAILED);
      }
   }
   /* Allocate Memory to Buffer */
   srvplmn->list.array[0]->pLMN_Identity.size = PLMN_SIZE * sizeof(U8);
   DU_ALLOC(srvplmn->list.array[0]->pLMN_Identity.buf, srvplmn->list.array[0]->pLMN_Identity.size);
   ret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
         &srvplmn->list.array[0]->pLMN_Identity);
   if(ret!= ROK)
   {
      for(plmnidx=0; plmnidx<servPlmnCnt; plmnidx++)
      {
         DU_FREE(srvplmn->list.array[plmnidx],\
               sizeof(struct ServedPLMNs_Item *));
      }
      DU_FREE(srvplmn->list.array,\
            sizeof(struct ServedPLMNs_Item *));
      RETVALUE(RFAILED);
   }
   ret = BuildExtensions(&srvplmn->list.array[0]->iE_Extensions);
   if(ret != ROK)
   {
      DU_FREE(srvplmn->list.\
            array[0]->pLMN_Identity.buf,(Size)\
            srvplmn->list.array[0]->pLMN_Identity.size * sizeof(U8));
      for(plmnidx=0; plmnidx<servPlmnCnt; plmnidx++)
      {
         DU_FREE(srvplmn->list.array[plmnidx],\
               sizeof(struct ServedPLMNs_Item *));
      }
      DU_FREE(srvplmn->list.array,\
            sizeof(struct ServedPLMNs_Item *));
      RETVALUE(RFAILED);
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Served Cell List
 *
 * @details
 *
 *    Function : BuildServedCellList
 *
 *    Functionality: Building Served Cell List
 *
 * @params[in] PLMNID plmn
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildServedCellList(GNB_DU_Served_Cells_List_t *duServedCell)
{
   S16 ret;
   U8  idx;
   U8  plmnidx;
   U8  plmnId;
   U8  plmnCnt=1;
   U8  servPlmnCnt=1;
   U8  extensionCnt=1;
   U8  sliceId;
   U8  servId;
   U8  ieId;
   GNB_DU_Served_Cells_Item_t *srvCellItem;

   duServedCell->list.size = plmnCnt * sizeof(GNB_DU_Served_Cells_ItemIEs_t *);
   duServedCell->list.count = plmnCnt; 
   DU_ALLOC(duServedCell->list.array, plmnCnt * sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
   if(duServedCell->list.array == NULLP)
   {
      RETVALUE(RFAILED);
   }
   for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
   {
      DU_ALLOC(duServedCell->list.array[plmnidx],\
            sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      if(duServedCell->list.array[plmnidx] == NULLP)
      {
         for(plmnId=0; plmnId<plmnidx ; plmnId++)
         {
            DU_FREE(duServedCell->list.array[plmnId],sizeof(GNB_DU_Served_Cells_ItemIEs_t));
         }
         DU_FREE(duServedCell->list.array,\
               plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
         RETVALUE(RFAILED);
      }
   }
   idx = 0;
   duServedCell->list.array[idx]->id = ProtocolIE_ID_id_GNB_DU_Served_Cells_Item;
   duServedCell->list.array[idx]->criticality = Criticality_reject;
   duServedCell->list.array[idx]->value.present = \
                GNB_DU_Served_Cells_ItemIEs__value_PR_GNB_DU_Served_Cells_Item;
   srvCellItem = \
           &duServedCell->list.array[idx]->value.choice.GNB_DU_Served_Cells_Item;
   /*nRCGI*/
   ret = BuildNrcgi(&srvCellItem->served_Cell_Information.nRCGI);
   if(ret != ROK)
   {
      for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
      {
         DU_FREE(duServedCell->list.array[plmnidx],\
               sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      }   
      DU_FREE(duServedCell->list.array,\
            plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
      RETVALUE(RFAILED);
   }
   
   /*nRPCI*/
   srvCellItem->served_Cell_Information.nRPCI = \
                         duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrPci;

   /*fiveGS_TAC*/
   ret = BuildFiveGSTac(&srvCellItem->served_Cell_Information);
   if(ret != ROK)
   {
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.size * sizeof(U8));
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size * sizeof(U8));
      for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
      {
         DU_FREE(duServedCell->list.array[plmnidx],\
               sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      }   
      DU_FREE(duServedCell->list.array,\
            plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
      RETVALUE(RFAILED);
   }
   /*Served PLMNs*/
   ret = BuildServedPlmn(&srvCellItem->served_Cell_Information.servedPLMNs);
   if(ret !=ROK)
   {
      DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC->buf,\
            sizeof(srvCellItem->served_Cell_Information.fiveGS_TAC->size));
      DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC,\
            sizeof(FiveGS_TAC_t));
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.size * sizeof(U8));
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size  * sizeof(U8));
      for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
      {
         DU_FREE(duServedCell->list.array[plmnidx],\
               sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      }   
      DU_FREE(duServedCell->list.array,\
            plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
      RETVALUE(RFAILED);
   }
   /*nR Mode Info with FDD*/
   ret = BuildNrMode(&srvCellItem->served_Cell_Information.nR_Mode_Info);
   sliceId = 0;
   ieId = 0;
   servId = 0;
   if(ret != ROK)
   {
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
         iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->buf, srvCellItem->served_Cell_Information.servedPLMNs.\
         list.array[servId]->iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->size);
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
          iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
          list.array[sliceId]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
           iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList\
           .list.array[sliceId]->sNSSAI.sST.buf,sizeof(U8));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
          iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
          list.array[sliceId],sizeof(SliceSupportItem_t));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
            iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof(SliceSupportItem_t *));
      for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
      {
         DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
               array[servId]->iE_Extensions->list.array[plmnidx],\
               sizeof(struct ServedPLMNs_ItemExtIEs));
      }
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
            array[servId]->iE_Extensions->list.array,\
            extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
            array[servId]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
            array[servId]->pLMN_Identity.buf,srvCellItem->served_Cell_Information.\
            servedPLMNs.list.array[servId]->pLMN_Identity.size * sizeof(U8));
      for(plmnidx=0; plmnidx<servPlmnCnt; plmnidx++)
      {
         DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[plmnidx],\
               sizeof(struct ServedPLMNs_Item *));
      }
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array,\
            sizeof(struct ServedPLMNs_Item *));
      DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC->buf,\
            sizeof(srvCellItem->served_Cell_Information.fiveGS_TAC->size));
      DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC,\
            sizeof(FiveGS_TAC_t));
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.size * sizeof(U8));
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size * sizeof(U8));
      for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
      {
         DU_FREE(duServedCell->list.array[plmnidx],\
               sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      }   
      DU_FREE(duServedCell->list.array,\
            plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
      RETVALUE(RFAILED);
   }
   

   /*Measurement timing Config*/
   srvCellItem->served_Cell_Information.measurementTimingConfiguration.\
      size = sizeof(U8);
   DU_ALLOC(srvCellItem->served_Cell_Information.\
         measurementTimingConfiguration.buf,sizeof(U8));
   if(srvCellItem->served_Cell_Information.\
         measurementTimingConfiguration.buf == NULLP)
   {
      DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array[0],\
         sizeof(struct FreqBandNrItem));
      DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            dL_NRFreqInfo.freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
      DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.\
            fDD->uL_NRFreqInfo.freqBandListNr.list.array[0],\
            sizeof(struct FreqBandNrItem));
      DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            uL_NRFreqInfo.freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
      DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
            sizeof(FDD_Info_t));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
         iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->buf,srvCellItem->served_Cell_Information.servedPLMNs.\
         list.array[servId]->iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->size);
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
            iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
           iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList\
           .list.array[sliceId]->sNSSAI.sST.buf,sizeof(U8));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
          iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
          list.array[sliceId],sizeof(SliceSupportItem_t));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
            iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof(SliceSupportItem_t *));
      for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
      {
         DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
               array[servId]->iE_Extensions->list.array[plmnidx],\
               sizeof(struct ServedPLMNs_ItemExtIEs));
      }
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
            array[servId]->iE_Extensions->list.array,\
            extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
            array[servId]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
            array[servId]->pLMN_Identity.buf,srvCellItem->served_Cell_Information.\
            servedPLMNs.list.array[servId]->pLMN_Identity.size * sizeof(U8));
      for(plmnidx=0; plmnidx<servPlmnCnt; plmnidx++)
      {
         DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[plmnidx],\
               sizeof(struct ServedPLMNs_Item *));
      }
      DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array,\
            sizeof(struct ServedPLMNs_Item *));
      DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC->buf,\
            sizeof(srvCellItem->served_Cell_Information.fiveGS_TAC->size));
      DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC,\
            sizeof(FiveGS_TAC_t));
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.size * sizeof(U8));
      DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size * sizeof(U8));
      for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
      {
         DU_FREE(duServedCell->list.array[plmnidx],\
               sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      }   
      DU_FREE(duServedCell->list.array,\
            plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
      RETVALUE(RFAILED);
   }
   srvCellItem->served_Cell_Information.measurementTimingConfiguration.buf[0] = \
             duCfgParam.srvdCellLst[0].duCellInfo.measTimeCfg;

   /* GNB DU System Information */
   DU_ALLOC(srvCellItem->gNB_DU_System_Information,
			sizeof(GNB_DU_System_Information_t));
   if(!srvCellItem->gNB_DU_System_Information)
	{
      DU_LOG("\nF1AP: Memory allocation failure for GNB_DU_System_Information");
		return RFAILED;
	}
	/* MIB */
	srvCellItem->gNB_DU_System_Information->mIB_message.size =\
	      strlen(( char *)duCfgParam.srvdCellLst[0].duSysInfo.mibMsg);
	DU_ALLOC(srvCellItem->gNB_DU_System_Information->mIB_message.buf,
	      srvCellItem->gNB_DU_System_Information->mIB_message.size);
   if(!srvCellItem->gNB_DU_System_Information->mIB_message.buf)
	{
      DU_LOG("\nF1AP: Memory allocation failure for mIB message");
		return RFAILED;
	}
   strcpy((char *)srvCellItem->gNB_DU_System_Information->mIB_message.buf,
			(char *)duCfgParam.srvdCellLst[0].duSysInfo.mibMsg);

   /* SIB1 */
	srvCellItem->gNB_DU_System_Information->sIB1_message.size =\
	      duCfgParam.srvdCellLst[0].duSysInfo.sib1Len;
	DU_ALLOC(srvCellItem->gNB_DU_System_Information->sIB1_message.buf,
	      srvCellItem->gNB_DU_System_Information->sIB1_message.size);
   if(!srvCellItem->gNB_DU_System_Information->sIB1_message.buf)
	{
      DU_LOG("\nF1AP: Memory allocation failure for SIB1 message");
		return RFAILED;
	}
	for(int x=0; x<srvCellItem->gNB_DU_System_Information->sIB1_message.size; x++)
	{
	   srvCellItem->gNB_DU_System_Information->sIB1_message.buf[x]=\
      duCfgParam.srvdCellLst[0].duSysInfo.sib1Msg[x];
   }
   /* Free memory */
	DU_FREE(duCfgParam.srvdCellLst[0].duSysInfo.sib1Msg, 
			srvCellItem->gNB_DU_System_Information->sIB1_message.size);
	DU_FREE(duCfgParam.srvdCellLst[0].duSysInfo.mibMsg, 
			strlen((char *)duCfgParam.srvdCellLst[0].duSysInfo.mibMsg));
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds RRC Version 
 *
 * @details
 *
 *    Function : BuildRrcVer
 *
 *    Functionality: Building RRC Version
 *
 * @params[in] int idx,int elementCnt,RRC_Version_t *rrcver
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildRrcVer(RRC_Version_t *rrcVer)
{
   U8 rrcExt;
   U8 rrcLatest;
   rrcVer->latest_RRC_Version.size = sizeof(U8);
   DU_ALLOC(rrcVer->latest_RRC_Version.buf,sizeof(U8));
   if(rrcVer->latest_RRC_Version.buf == NULLP)
   {
      RETVALUE(RFAILED);
   }
   rrcVer->latest_RRC_Version.buf[0] = 0;
   rrcVer->latest_RRC_Version.bits_unused = 5;
   DU_ALLOC(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
   if(rrcVer->iE_Extensions == NULLP)
   {
      DU_FREE(rrcVer->latest_RRC_Version.buf,sizeof(U8));
      RETVALUE(RFAILED);
   }
   rrcVer->iE_Extensions->list.count = 1;
   rrcVer->iE_Extensions->list.size = sizeof(struct RRC_Version_ExtIEs *);
   DU_ALLOC(rrcVer->iE_Extensions->list.array,\
         sizeof(struct RRC_Version_ExtIEs *));
   if(rrcVer->iE_Extensions->list.array == NULLP)
   {
      DU_FREE(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
      DU_FREE(rrcVer->latest_RRC_Version.buf,sizeof(U8));
      RETVALUE(RFAILED);
   }
   rrcExt = 0;
   DU_ALLOC(rrcVer->iE_Extensions->list.array[0],\
         sizeof(struct RRC_Version_ExtIEs));
   if(rrcVer->iE_Extensions->list.array[0] == NULLP)
   {
      DU_FREE(rrcVer->iE_Extensions->list.array,\
            sizeof(struct RRC_Version_ExtIEs *));
      DU_FREE(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
      DU_FREE(rrcVer->latest_RRC_Version.buf,sizeof(U8));
      RETVALUE(RFAILED);
   }
   rrcVer->iE_Extensions->list.array[rrcExt]->id = \
                             ProtocolIE_ID_id_latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[rrcExt]->criticality = Criticality_reject;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.present =\
              RRC_Version_ExtIEs__extensionValue_PR_Latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice\
             .Latest_RRC_Version_Enhanced.size = 3*sizeof(U8);
   DU_ALLOC(rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice\
         .Latest_RRC_Version_Enhanced.buf,rrcVer->iE_Extensions->list.\
         array[rrcExt]->extensionValue.choice.Latest_RRC_Version_Enhanced.size);
   if(rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice\
          .Latest_RRC_Version_Enhanced.buf == NULLP)
   {
      DU_FREE(rrcVer->iE_Extensions->list.array[rrcExt],\
            sizeof(struct RRC_Version_ExtIEs));
      DU_FREE(rrcVer->iE_Extensions->list.array,\
            sizeof(struct RRC_Version_ExtIEs *));
      DU_FREE(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
      DU_FREE(rrcVer->latest_RRC_Version.buf,sizeof(U8));
      RETVALUE(RFAILED);
   }
   rrcLatest = 0;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[rrcLatest] = 15;
   rrcLatest++;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[rrcLatest] = 5;
   rrcLatest++;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[rrcLatest] = 0;
   return ROK;
}
/*******************************************************************
*
* @brief Sends F1 msg over SCTP
*
* @details
*
*    Function : SendF1APMsg
*
*    Functionality: Sends F1 msg over SCTP
*
* @params[in] Region region
*             Pool pool
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
S16 SendF1APMsg(Region region, Pool pool)
{
   Buffer *mBuf;

   if(SGetMsg(region, pool, &mBuf) == ROK)
   {
      if(SAddPstMsgMult((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         SPrntMsg(mBuf, 0,0);
 
         if(sctpSend(mBuf, F1_INTERFACE) != ROK)
         {
            DU_LOG("\nF1AP : SCTP Send failed");
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      else
      {
         DU_LOG("\nF1AP : SAddPstMsgMult failed");
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      SPutMsg(mBuf);
   }
   else
   {
      DU_LOG("\nF1AP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }
   return ROK; 
} /* SendF1APMsg */


/*******************************************************************
 *
 * @brief Builds and Send the F1SetupRequest
 *
 * @details
 *
 *    Function : BuildAndSendF1SetupReq
 *
 * Functionality:Fills the F1SetupRequest
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 BuildAndSendF1SetupReq()
{
  S16  ret,ret1;
   U8   idx,idx2;
   U8   ieId;
   U8   plmnidx;
   U8   servId;
   U8   sliceId;
   U8   elementCnt;
   U8   plmnCnt=1;
   U8   servPlmnCnt=1;
   U8   extensionCnt=1;
   F1AP_PDU_t                 *f1apMsg = NULL;
   F1SetupRequest_t           *f1SetupReq;
   GNB_DU_Served_Cells_List_t *duServedCell;
   GNB_DU_Served_Cells_Item_t *srvCellItem;
   RRC_Version_t              *rrcVer;
   asn_enc_rval_t             encRetVal;        /* Encoder return value */
   DU_LOG("\nF1AP : Building F1 Setup Request\n");
   while(1)
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         break;
      }
      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_F1Setup;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = \
                                    InitiatingMessage__value_PR_F1SetupRequest;

      f1SetupReq = &f1apMsg->choice.initiatingMessage->value.choice.F1SetupRequest;

      elementCnt = (duCfgParam.duName != NULL) ? 5 : 4;

      f1SetupReq->protocolIEs.list.count = elementCnt;
      f1SetupReq->protocolIEs.list.size = elementCnt * sizeof(F1SetupRequestIEs_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(f1SetupReq->protocolIEs.list.array, \
         elementCnt * sizeof(F1SetupRequestIEs_t *));
      if(f1SetupReq->protocolIEs.list.array == NULLP)
      {
         break;
      }
      for(idx=0; idx<elementCnt; idx++)
      {
         DU_ALLOC(f1SetupReq->protocolIEs.list.array[idx],\
            sizeof(F1SetupRequestIEs_t));
         if(f1SetupReq->protocolIEs.list.array[idx] == NULLP)
         {
            break;
         }
      }
      idx2 = 0;

      /*TransactionID*/
      f1SetupReq->protocolIEs.list.array[idx2]->id = ProtocolIE_ID_id_TransactionID;
      f1SetupReq->protocolIEs.list.array[idx2]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[idx2]->value.present =\
                                    F1SetupRequestIEs__value_PR_TransactionID;
      f1SetupReq->protocolIEs.list.array[idx2]->value.choice.TransactionID = \
                                                                   TRANS_ID;

      /*DU ID*/
      idx2++;
      f1SetupReq->protocolIEs.list.array[idx2]->id = ProtocolIE_ID_id_gNB_DU_ID;
      f1SetupReq->protocolIEs.list.array[idx2]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[idx2]->value.present = \
         F1SetupRequestIEs__value_PR_GNB_DU_ID;
      f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_ID.size =\
                                                                    sizeof(U8);

      DU_ALLOC(f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_ID.buf,\
           sizeof(U8));
      if(f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_ID.buf == \
         NULLP)
      {
         break;
      }
                       
      f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_ID.buf[0] =\
                                                             duCfgParam.duId;

      /*DU Name*/
      if(duCfgParam.duName != NULL)
      {
         idx2++;
         f1SetupReq->protocolIEs.list.array[idx2]->id = ProtocolIE_ID_id_gNB_DU_Name ;
         f1SetupReq->protocolIEs.list.array[idx2]->criticality = Criticality_ignore;
         f1SetupReq->protocolIEs.list.array[idx2]->value.present = \
                                          F1SetupRequestIEs__value_PR_GNB_DU_Name;
         f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_Name.size =\
                                                        sizeof(duCfgParam.duName);
         DU_ALLOC(f1SetupReq->protocolIEs.list.array[idx2]->value.choice.\
            GNB_DU_Name.buf, sizeof(duCfgParam.duName));
         if(f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_Name.\
            buf == NULLP)
         {
            break;
         }
         strcpy((char*)f1SetupReq->protocolIEs.list.array[idx2]->value.\
            choice.GNB_DU_Name.buf,
         (char*)&duCfgParam.duName);

      }

       /*Served Cell list */
      idx2++;
      f1SetupReq->protocolIEs.list.array[idx2]->id = \
                                 ProtocolIE_ID_id_gNB_DU_Served_Cells_List;
      f1SetupReq->protocolIEs.list.array[idx2]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[idx2]->value.present = \
                            F1SetupRequestIEs__value_PR_GNB_DU_Served_Cells_List;
      duServedCell = &f1SetupReq->protocolIEs.list.\
                  array[idx2]->value.choice.GNB_DU_Served_Cells_List;
      ret = BuildServedCellList(duServedCell);
      srvCellItem =  &duServedCell->list.array[idx2]->value. \
                       choice.GNB_DU_Served_Cells_Item;
      if(ret != ROK)
      {
         break;
      }
      /*RRC Version*/
      idx2++;
      f1SetupReq->protocolIEs.list.array[idx2]->id = \
                                  ProtocolIE_ID_id_GNB_DU_RRC_Version ;
      f1SetupReq->protocolIEs.list.array[idx2]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[idx2]->value.present = \
                                    F1SetupRequestIEs__value_PR_RRC_Version;
      rrcVer = &f1SetupReq->protocolIEs.list.array[idx2]->value.choice.RRC_Version;
      ret1 = BuildRrcVer(rrcVer);
      ieId = 0;
      sliceId = 0;
      servId = 0;
      if(ret1 != ROK)
      {
         break;
      }
      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
         encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
           DU_LOG("\nF1AP : Could not encode F1SetupRequest structure (at %s)\n",\
                           encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
           break;
      }
      else
      {
           DU_LOG("\nF1AP : Created APER encoded buffer for F1SetupRequest\n");
           for(int i=0; i< encBufSize; i++)
           {
                   printf("%x",encBuf[i]);
           }
      }

      /* Sending msg */
      if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
       {
           DU_LOG("\nF1AP : Sending F1 Setup request failed");
           break;
      }
      break;
   }

   if(f1apMsg != NULLP)
   {

      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
         if(f1SetupReq->protocolIEs.list.array != NULLP)
         {
            if(idx == elementCnt-1)
            {
               if(f1SetupReq->protocolIEs.list.array[idx]->value.choice.GNB_DU_ID.buf !=  NULLP)
               {
                  if(f1SetupReq->protocolIEs.list.array[idx]->value.choice.GNB_DU_Name.buf != NULLP)
                  {
                     if(ret == ROK)
                     {
                       if(ret1 != ROK)
                       {
                          DU_FREE(srvCellItem->served_Cell_Information.\
                            measurementTimingConfiguration.buf,sizeof(U8));
                          DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
                            dL_NRFreqInfo.freqBandListNr.list.array[0],\
                           sizeof(struct FreqBandNrItem));
                                                           
                          DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
                            dL_NRFreqInfo.freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
                          DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.\
                            fDD->uL_NRFreqInfo.freqBandListNr.list.array[0],\
                            sizeof(struct FreqBandNrItem));
                          DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
                            uL_NRFreqInfo.freqBandListNr.list.array,sizeof(struct FreqBandNrItem *));
                          DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
                            sizeof(FDD_Info_t));
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
                            iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
                            list.array[sliceId]->sNSSAI.sD->buf,srvCellItem->served_Cell_Information.servedPLMNs.\
                            list.array[servId]->iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
                            list.array[sliceId]->sNSSAI.sD->size);
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
                            iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
                            list.array[sliceId]->sNSSAI.sD,sizeof(OCTET_STRING_t));
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
                            iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList\
                           .list.array[sliceId]->sNSSAI.sST.buf,sizeof(U8));
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
                          iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
                          list.array[sliceId],sizeof(SliceSupportItem_t));
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
                          iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
                          list.array,sizeof(SliceSupportItem_t *));
                          for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
                          {
                             DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
                            array[servId]->iE_Extensions->list.array[plmnidx],\
                             sizeof(struct ServedPLMNs_ItemExtIEs));
                          }
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
                          array[servId]->iE_Extensions->list.array,\
                           extensionCnt*sizeof(struct ServedPLMNs_ItemExtIEs *));
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
                          array[servId]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
                          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
                          array[servId]->pLMN_Identity.buf,srvCellItem->served_Cell_Information.\
                          servedPLMNs.list.array[servId]->pLMN_Identity.size * sizeof(U8));
                          for(plmnidx=0;plmnidx<servPlmnCnt; plmnidx++)
                          {
                             DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[plmnidx],\
                             sizeof(struct ServedPLMNs_Item *));
                           }
                           DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array,\
                           sizeof(struct ServedPLMNs_Item *));
                           DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC->buf,\
                           sizeof(srvCellItem->served_Cell_Information.fiveGS_TAC->size));
                         /*********/
                           DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC,\
                             sizeof(FiveGS_TAC_t));
                           DU_FREE(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
                             srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.size * sizeof(U8));
                           DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
                             srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size * sizeof(U8));
                           for(plmnidx=0; plmnidx<plmnCnt;plmnidx++)
                           {
                               DU_FREE(duServedCell->list.array[plmnidx],\
                                 sizeof(GNB_DU_Served_Cells_ItemIEs_t));
                           }
                           DU_FREE(duServedCell->list.array,\
                               plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t *));
                       }
                       DU_FREE(f1SetupReq->protocolIEs.list.array[--idx2]->value.\
                       choice.GNB_DU_Name.buf, sizeof(duCfgParam.duName));
                    }
                    DU_FREE(f1SetupReq->protocolIEs.list.array[--idx2]->value.choice.\
                    GNB_DU_ID.buf, sizeof(U8));
                    for(plmnidx=0; plmnidx<elementCnt; plmnidx++)
                    {
                      DU_FREE(f1SetupReq->protocolIEs.list.array[plmnidx],sizeof(F1SetupRequestIEs_t));
                    }
                  }
               }
            }
            else
            {
               if(f1SetupReq->protocolIEs.list.array[idx] == NULLP)
               {
                  for(ieId=0 ; ieId<idx ;ieId++)
                  {
                     DU_FREE(f1SetupReq->protocolIEs.list.array[ieId],\
                     sizeof(F1SetupRequestIEs_t));
                  }
               }
            }
            DU_FREE(f1SetupReq->protocolIEs.list.array,\
               elementCnt * sizeof(F1SetupRequestIEs_t *));
         }
         DU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   return ROK;                                                                                                                                    
}/* End of BuildAndSendF1SetupReq */

/*******************************************************************
 *
 * @brief Builds and sends the DUConfigUpdate
 *
 * @details
 *
 *    Function : BuildAndSendDUConfigUpdate
 *
 *    Functionality: Constructs the DU Update message and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildAndSendDUConfigUpdate()
{
   U8    idx,idy;
   U8    elementCnt,modifyCnt,servPlmnCnt,extensionCnt;
   asn_enc_rval_t encRetVal;      /* Encoder return value */
   F1AP_PDU_t *f1apDuCfg = NULL;
   GNBDUConfigurationUpdate_t *duCfgUpdate;
   Served_Cells_To_Modify_List_t  *cellsToModify;
   Served_Cells_To_Modify_Item_t *modifyItem;
   DU_LOG("\nF1AP : Building DU config update\n");   

   /* Allocate the memory for F1DuCfg */
   DU_ALLOC(f1apDuCfg, (Size)sizeof(F1AP_PDU_t));
   if(f1apDuCfg == NULLP)
   {
	   DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
	   RETVALUE(RFAILED);
   }
 
   f1apDuCfg->present = F1AP_PDU_PR_initiatingMessage;
   DU_ALLOC(f1apDuCfg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
   if(f1apDuCfg->choice.initiatingMessage == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");  
      DU_FREE(f1apDuCfg, (Size)sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   
   f1apDuCfg->choice.initiatingMessage->procedureCode = \
                                    ProcedureCode_id_gNBDUConfigurationUpdate;
   f1apDuCfg->choice.initiatingMessage->criticality = Criticality_reject;
   f1apDuCfg->choice.initiatingMessage->value.present = \
                         InitiatingMessage__value_PR_GNBDUConfigurationUpdate;
   duCfgUpdate = &f1apDuCfg->choice.initiatingMessage->value.\
                 choice.GNBDUConfigurationUpdate;

   elementCnt = 3;
   duCfgUpdate->protocolIEs.list.count = elementCnt;
   duCfgUpdate->protocolIEs.list.size = \
                         elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *);

   /* Initialize the F1Setup members */
   DU_ALLOC(duCfgUpdate->protocolIEs.list.array,\
         elementCnt*sizeof(GNBDUConfigurationUpdateIEs_t *));
   if(duCfgUpdate->protocolIEs.list.array == NULLP)
   {
      DU_LOG("F1AP : Memory allocation for F1RequestIEs failed");
      DU_FREE(f1apDuCfg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }

   for(idx=0;idx<elementCnt;idx++)
   {
      DU_ALLOC(duCfgUpdate->protocolIEs.list.array[idx], \
            sizeof(GNBDUConfigurationUpdateIEs_t));
      if(duCfgUpdate->protocolIEs.list.array[idx] == NULLP)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array,\
               elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
         DU_FREE(f1apDuCfg->choice.initiatingMessage,\
               (Size)sizeof(InitiatingMessage_t));
         DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }

   /*TransactionID*/
   idx = 0;
   duCfgUpdate->protocolIEs.list.array[idx]->id = \
                                                ProtocolIE_ID_id_TransactionID;
   duCfgUpdate->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   duCfgUpdate->protocolIEs.list.array[idx]->value.present = \
                           GNBDUConfigurationUpdateIEs__value_PR_TransactionID;
   duCfgUpdate->protocolIEs.list.array[idx]->value.choice.TransactionID = \
                                                                      TRANS_ID;
#if 1 
   /*Served Cell to Modify */
   idx++;
   duCfgUpdate->protocolIEs.list.array[idx]->id = \
                                  ProtocolIE_ID_id_Served_Cells_To_Modify_List;
   duCfgUpdate->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   duCfgUpdate->protocolIEs.list.array[idx]->value.present = \
              GNBDUConfigurationUpdateIEs__value_PR_Served_Cells_To_Modify_List;
   cellsToModify = &duCfgUpdate->protocolIEs.list.array[1]->value.choice.\
                   Served_Cells_To_Modify_List;
   modifyCnt = 1;
   cellsToModify->list.count = modifyCnt;
   cellsToModify->list.size = \
                      modifyCnt*sizeof(struct Served_Cells_To_Modify_ItemIEs *);
   DU_ALLOC(cellsToModify->list.array,modifyCnt*\
         sizeof(struct Served_Cells_To_Modify_ItemIEs *));
   if(cellsToModify->list.array == NULLP)
   {
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
         DU_FREE(duCfgUpdate->protocolIEs.list.array,\
               elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
         DU_FREE(f1apDuCfg->choice.initiatingMessage,\
               (Size)sizeof(InitiatingMessage_t));
         DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      
   }
   for(idy=0; idy<modifyCnt ;idy++)
   {
      DU_ALLOC(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      if(cellsToModify->list.array[idy] == NULLP)
      {
         DU_FREE(cellsToModify->list.array,modifyCnt*\
               sizeof(struct Served_Cells_To_Modify_ItemIEs *));
         for(idy=0;idy<elementCnt;idy++)
         {
            DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
                  sizeof(GNBDUConfigurationUpdateIEs_t));
         }
         DU_FREE(duCfgUpdate->protocolIEs.list.array,\
               elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
         DU_FREE(f1apDuCfg->choice.initiatingMessage,\
               (Size)sizeof(InitiatingMessage_t));
         DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }
   cellsToModify->list.array[0]->id = \
                                  ProtocolIE_ID_id_Served_Cells_To_Modify_Item;
   cellsToModify->list.array[0]->criticality = Criticality_reject;
   cellsToModify->list.array[0]->value.present = \
           Served_Cells_To_Modify_ItemIEs__value_PR_Served_Cells_To_Modify_Item;
   modifyItem=&cellsToModify->list.array[0]->value.choice.\
              Served_Cells_To_Modify_Item;

   /*pLMN_Identity*/
   modifyItem->oldNRCGI.pLMN_Identity.size = 3;
   DU_ALLOC(modifyItem->oldNRCGI.pLMN_Identity.buf, 3*sizeof(uint8_t));
   if(modifyItem->oldNRCGI.pLMN_Identity.buf == NULLP)
   {
      for(idy=0; idy<modifyCnt ;idy++)
      {
         DU_FREE(cellsToModify->list.array[idy],\
               sizeof(struct Served_Cells_To_Modify_ItemIEs));
         DU_FREE(cellsToModify->list.array,modifyCnt*\
               sizeof(struct Served_Cells_To_Modify_ItemIEs *));
         for(idy=0;idy<elementCnt;idy++)
         {
            DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
                  sizeof(GNBDUConfigurationUpdateIEs_t));
         }
         DU_FREE(duCfgUpdate->protocolIEs.list.array,\
               elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
         DU_FREE(f1apDuCfg->choice.initiatingMessage,\
               (Size)sizeof(InitiatingMessage_t));
         DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
         &modifyItem->oldNRCGI.pLMN_Identity);
   /*nRCellIdentity*/
   modifyItem->oldNRCGI.nRCellIdentity.size = 5;
   DU_ALLOC(modifyItem->oldNRCGI.nRCellIdentity.buf,\
         modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
   if(modifyItem->oldNRCGI.nRCellIdentity.buf == NULLP)
   {
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   for (int tmp = 0 ; tmp < modifyItem->oldNRCGI.nRCellIdentity.size-1 ; tmp++)
   {
      modifyItem->oldNRCGI.nRCellIdentity.buf[tmp] = 0;
   }
   modifyItem->oldNRCGI.nRCellIdentity.buf[4] = 16;
   modifyItem->oldNRCGI.nRCellIdentity.bits_unused = 4;
   /*pLMN_Identity*/
   /*nRCGI*/
   modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.size = 3;
   DU_ALLOC(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
         3*sizeof(U8));
   if(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf == NULLP)
   {
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
         &modifyItem->served_Cell_Information.nRCGI.pLMN_Identity);
   modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.size = 5;
   DU_ALLOC(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
         5*sizeof(uint8_t));
   if(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   for (int tmp = 0 ; tmp < modifyItem->served_Cell_Information.\
         nRCGI.nRCellIdentity.size-1 ; tmp++)
   {
      modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf[tmp] = 0;
   }
   modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf[4] = 16; 
   modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.bits_unused = 4;

   /*nRPCI*/
   modifyItem->served_Cell_Information.nRPCI = \
                           duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrPci;

   /*servedPLMNs*/
   servPlmnCnt = 1;
   modifyItem->served_Cell_Information.servedPLMNs.list.count = servPlmnCnt;
   modifyItem->served_Cell_Information.servedPLMNs.list.size = \
                                servPlmnCnt*sizeof(struct ServedPLMNs_Item *);
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
         servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.array == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   for(idy=0;idy<servPlmnCnt;idy++)
   {
      DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      if(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy] == NULLP)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
         DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
               5*sizeof(uint8_t));
         DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
               3*sizeof(U8));
         DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
               modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
         DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
         DU_FREE(cellsToModify->list.array[idy],\
               sizeof(struct Served_Cells_To_Modify_ItemIEs));
         DU_FREE(cellsToModify->list.array,modifyCnt*\
               sizeof(struct Served_Cells_To_Modify_ItemIEs *));
         for(idy=0;idy<elementCnt;idy++)
         {
            DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
                  sizeof(GNBDUConfigurationUpdateIEs_t));
         }
         DU_FREE(duCfgUpdate->protocolIEs.list.array,\
               elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
         DU_FREE(f1apDuCfg->choice.initiatingMessage,\
               (Size)sizeof(InitiatingMessage_t));
         DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }
   modifyItem->served_Cell_Information.servedPLMNs.list.\
      array[0]->pLMN_Identity.size = 3;
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
         array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.\
         array[0]->pLMN_Identity.buf == NULLP)
   {
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.plmn[0],\
         &modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->pLMN_Identity);
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
       array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.\
         array[0]->iE_Extensions == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   extensionCnt=1;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.count = extensionCnt;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.size = extensionCnt * \
      sizeof(struct ServedPLMNs_ItemExtIEs *);
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
         array[0]->iE_Extensions->list.array,extensionCnt*\
         sizeof(struct ServedPLMNs_ItemExtIEs *));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.\
         array[0]->iE_Extensions->list.array == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   for(idy=0;idy<extensionCnt;idy++)
   {
      DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array[idy],\
            sizeof(struct ServedPLMNs_ItemExtIEs)); 
      if(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array[idy] == NULLP)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array,extensionCnt*\
               sizeof(struct ServedPLMNs_ItemExtIEs *));
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
         for(idy=0;idy<servPlmnCnt;idy++)
         {
            DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
                  servPlmnCnt*sizeof(struct ServedPLMNs_Item));
         }
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
         DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
               5*sizeof(uint8_t));
         DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
               3*sizeof(U8));
         DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
               modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
         DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
         DU_FREE(cellsToModify->list.array[idy],\
               sizeof(struct Served_Cells_To_Modify_ItemIEs));
         DU_FREE(cellsToModify->list.array,modifyCnt*\
               sizeof(struct Served_Cells_To_Modify_ItemIEs *));
         for(idy=0;idy<elementCnt;idy++)
         {
            DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
                  sizeof(GNBDUConfigurationUpdateIEs_t));
         }
         DU_FREE(duCfgUpdate->protocolIEs.list.array,\
               elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
         DU_FREE(f1apDuCfg->choice.initiatingMessage,\
               (Size)sizeof(InitiatingMessage_t));
         DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->id = ProtocolIE_ID_id_TAISliceSupportList;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->criticality = Criticality_ignore;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.present = \
      ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.count = 1;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.size = sizeof(SliceSupportItem_t *);
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array,sizeof( SliceSupportItem_t *));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array == NULLP)
   {
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0],sizeof( SliceSupportItem_t));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0] == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sST.size = sizeof(uint8_t);
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0]->sNSSAI.sST.buf == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue. choice.SliceSupportList.\
      list.array[0]->sNSSAI.sST.buf[0] = 3;
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0]->sNSSAI.sD == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->size = 3*sizeof(uint8_t);
   DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
         iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
         list.array[0]->sNSSAI.sD->buf == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->buf[0] = 3;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->buf[1] = 6;
   modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->buf[2] = 9;

   /*nR Mode Info with FDD*/
   modifyItem->served_Cell_Information.nR_Mode_Info.present = \
                                                         NR_Mode_Info_PR_fDD;
   DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
         sizeof(FDD_Info_t));
   if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
      uL_NRFreqInfo.nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
      uL_NRFreqInfo.freqBandListNr.list.count = 1;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
      uL_NRFreqInfo.freqBandListNr.list.size = \
      sizeof(struct FreqBandNrItem *);
   DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
         fDD->uL_NRFreqInfo.freqBandListNr.list.\
         array,sizeof(struct FreqBandNrItem *));
   if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
         fDD->uL_NRFreqInfo.freqBandListNr.list.array == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
            sizeof(FDD_Info_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,\
            sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         uL_NRFreqInfo.freqBandListNr.list.\
         array[0],sizeof(struct FreqBandNrItem));
   if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         uL_NRFreqInfo.freqBandListNr.list.\
         array[0] == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
            fDD->uL_NRFreqInfo.freqBandListNr.list.\
            array,sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
            sizeof(FDD_Info_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,\
            sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->uL_NRFreqInfo.\
      freqBandListNr.list.array[0]->freqBandIndicatorNr = \
      duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.\
      freqBand[0].nrFreqBand;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->uL_NRFreqInfo.\
      freqBandListNr.list.array[0]->supportedSULBandList.list.count=0;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
      nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.\
      dlNrFreqInfo.nrArfcn;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
      freqBandListNr.list.count = 1;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
      freqBandListNr.list.size = sizeof(struct FreqBandNrItem *);
   DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array,\
         sizeof(struct FreqBandNrItem *));
   if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            uL_NRFreqInfo.freqBandListNr.list.\
            array[0],sizeof(struct FreqBandNrItem));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
            fDD->uL_NRFreqInfo.freqBandListNr.list.\
            array,sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
            sizeof(FDD_Info_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,\
            sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array[0],\
         sizeof(struct FreqBandNrItem));
   if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array[0] == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            dL_NRFreqInfo.freqBandListNr.list.array,\
            sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            uL_NRFreqInfo.freqBandListNr.list.\
            array[0],sizeof(struct FreqBandNrItem));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
            fDD->uL_NRFreqInfo.freqBandListNr.list.\
            array,sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
            sizeof(FDD_Info_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,\
            sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
      freqBandListNr.list.array[0]->freqBandIndicatorNr = \
      duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.\
      freqBand[0].nrFreqBand;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
      freqBandListNr.list.array[0]->supportedSULBandList.list.count=0;

   /*Transmission Bandwidth*/
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
      uL_Transmission_Bandwidth.nRSCS = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulTxBw.nrScs;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
      uL_Transmission_Bandwidth.nRNRB = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulTxBw.nrb;

   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
      dL_Transmission_Bandwidth.nRSCS = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.dlTxBw.nrScs;
   modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
      dL_Transmission_Bandwidth.nRNRB = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.dlTxBw.nrb;
   
   /*Measurement timing Config*/
   modifyItem->served_Cell_Information.measurementTimingConfiguration.size = \
                                                              sizeof(uint8_t);
   DU_ALLOC(modifyItem->served_Cell_Information.measurementTimingConfiguration.\
         buf,sizeof(uint8_t));
   if(modifyItem->served_Cell_Information.measurementTimingConfiguration.buf == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            dL_NRFreqInfo.freqBandListNr.list.array[0],\
            sizeof(struct FreqBandNrItem));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            dL_NRFreqInfo.freqBandListNr.list.array,\
            sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            uL_NRFreqInfo.freqBandListNr.list.\
            array[0],sizeof(struct FreqBandNrItem));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
            fDD->uL_NRFreqInfo.freqBandListNr.list.\
            array,sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
            sizeof(FDD_Info_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,\
            sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   modifyItem->served_Cell_Information.measurementTimingConfiguration.\
      buf[0] = duCfgParam.srvdCellLst[0].duCellInfo.measTimeCfg;

   
   // NOTE :GNB DU SYS INFO:MIB AND SIB1 INFORMATION TO BE BUILT AND FILLED HERE
   /*GNB DU ID */
   duCfgUpdate->protocolIEs.list.array[2]->id = ProtocolIE_ID_id_gNB_DU_ID;
   duCfgUpdate->protocolIEs.list.array[2]->criticality = Criticality_reject;
   duCfgUpdate->protocolIEs.list.array[2]->value.present = \
                             GNBDUConfigurationUpdateIEs__value_PR_GNB_DU_ID;
   duCfgUpdate->protocolIEs.list.array[2]->value.choice.GNB_DU_ID.size = \
                                                             sizeof(uint8_t);
   DU_ALLOC(duCfgUpdate->protocolIEs.list.array[2]->value.choice.GNB_DU_ID.buf,\
         sizeof(uint8_t));
   if(duCfgUpdate->protocolIEs.list.array[2]->value.choice.GNB_DU_ID.buf == NULLP)
   {
      DU_FREE(modifyItem->served_Cell_Information.measurementTimingConfiguration.\
            buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            dL_NRFreqInfo.freqBandListNr.list.array[0],\
            sizeof(struct FreqBandNrItem));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            dL_NRFreqInfo.freqBandListNr.list.array,\
            sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
            uL_NRFreqInfo.freqBandListNr.list.\
            array[0],sizeof(struct FreqBandNrItem));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
            fDD->uL_NRFreqInfo.freqBandListNr.list.\
            array,sizeof(struct FreqBandNrItem *));
      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD,\
            sizeof(FDD_Info_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD->buf,3*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0]->sNSSAI.sST.buf,sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array[0],sizeof( SliceSupportItem_t));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
            iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
            list.array,sizeof( SliceSupportItem_t *));
      for(idy=0;idy<extensionCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
               array[0]->iE_Extensions->list.array[idy],\
               sizeof(struct ServedPLMNs_ItemExtIEs)); 
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions->list.array,extensionCnt*\
            sizeof(struct ServedPLMNs_ItemExtIEs *));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->iE_Extensions,\
            sizeof(struct ProtocolExtensionContainer_4624P3));
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
            array[0]->pLMN_Identity.buf,3*sizeof(uint8_t));
      for(idy=0;idy<servPlmnCnt;idy++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy],\
               servPlmnCnt*sizeof(struct ServedPLMNs_Item));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
            servPlmnCnt*sizeof(struct ServedPLMNs_Item *));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
            5*sizeof(uint8_t));
      DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
            3*sizeof(U8));
      DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
            modifyItem->oldNRCGI.nRCellIdentity.size*sizeof(uint8_t));
      DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,3*sizeof(uint8_t));
      DU_FREE(cellsToModify->list.array[idy],\
            sizeof(struct Served_Cells_To_Modify_ItemIEs));
      DU_FREE(cellsToModify->list.array,modifyCnt*\
            sizeof(struct Served_Cells_To_Modify_ItemIEs *));
      for(idy=0;idy<elementCnt;idy++)
      {
         DU_FREE(duCfgUpdate->protocolIEs.list.array[idy], \
               sizeof(GNBDUConfigurationUpdateIEs_t));
      }
      DU_FREE(duCfgUpdate->protocolIEs.list.array,\
            elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
      DU_FREE(f1apDuCfg->choice.initiatingMessage,\
            (Size)sizeof(InitiatingMessage_t));
      DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   duCfgUpdate->protocolIEs.list.array[2]->value.choice.GNB_DU_ID.buf[0] = \
                                                              duCfgParam.duId;



#endif
   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apDuCfg);
   
   /* Encode the DU Config Update type as APER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apDuCfg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   for(idx=0;idx<elementCnt;idx++)
   {
      DU_FREE(duCfgUpdate->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateIEs_t));
   }
   DU_FREE(duCfgUpdate->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
   DU_FREE(f1apDuCfg->choice.initiatingMessage, (Size)sizeof(InitiatingMessage_t));
   DU_FREE(f1apDuCfg, sizeof(F1AP_PDU_t));

   /* Checking encode results */
   if(encRetVal.encoded == ENCODE_FAIL) 
   {
      DU_LOG("F1AP : Could not encode DUConfigUpdate structure (at %s)\n",encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      RETVALUE(RFAILED);
   } 
   else 
   {
      DU_LOG("\nF1AP : Created APER encoded buffer for DUConfigUpdate\n");
      for(int i=0; i< encBufSize; i++)
      {
         printf("%x",encBuf[i]);
      }
   }

   /* Sending msg */
   if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
   {
      DU_LOG("\nF1AP : Sending GND-DU Config Update failed");
      RETVALUE(RFAILED);
   }
   return ROK;
}/* End of BuildAndSendDUConfigUpdate */

/*******************************************************************
 *
 * @brief Builds and sends the ULRRCMessageTransfer 
 *
 * @details
 *
 *    Function : BuildAndSendULRRCMessageTransfer
 *
 *    Functionality: Constructs the UL RRC Message Transfer and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildAndSendULRRCMessageTransfer()
{
	U8   elementCnt;
	U8   ieId;
	U8   idx;
	F1AP_PDU_t      			*f1apMsg = NULL;
   ULRRCMessageTransfer_t	*ulRRCMsg;
	asn_enc_rval_t  			encRetVal;        /* Encoder return value */

	DU_LOG("\n F1AP : Building UL RRC Message Transfer Message\n");

	DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
	if(f1apMsg == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
		RETVALUE(RFAILED);
	}

	f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
	DU_ALLOC(f1apMsg->choice.initiatingMessage,
			sizeof(InitiatingMessage_t));
	if(f1apMsg->choice.initiatingMessage == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	f1apMsg->choice.initiatingMessage->procedureCode = \
												ProcedureCode_id_ULRRCMessageTransfer;
	f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
	f1apMsg->choice.initiatingMessage->value.present = \
								InitiatingMessage__value_PR_ULRRCMessageTransfer;
	ulRRCMsg =
		&f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;
	elementCnt = 3;
	ulRRCMsg->protocolIEs.list.count = elementCnt;
	ulRRCMsg->protocolIEs.list.size = \
									elementCnt * sizeof(ULRRCMessageTransferIEs_t *);

	/* Initialize the F1Setup members */
	DU_ALLOC(ulRRCMsg->protocolIEs.list.array, \
			elementCnt * sizeof(ULRRCMessageTransferIEs_t *));
	if(ulRRCMsg->protocolIEs.list.array == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for UL RRC MessageTransferIEs failed");
		DU_FREE(f1apMsg->choice.initiatingMessage,
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	for(idx=0; idx<elementCnt; idx++)
	{
		DU_ALLOC(ulRRCMsg->protocolIEs.list.array[idx],\
										sizeof(ULRRCMessageTransferIEs_t));
		if(ulRRCMsg->protocolIEs.list.array[idx] == NULLP)
		{
			for(ieId=0; ieId<idx; ieId++)
			{
				DU_FREE(ulRRCMsg->protocolIEs.list.array[ieId],\
						sizeof(ULRRCMessageTransferIEs_t));
			}
			DU_FREE(ulRRCMsg->protocolIEs.list.array,\
					elementCnt * sizeof(ULRRCMessageTransferIEs_t *));
			DU_FREE(f1apMsg->choice.initiatingMessage,\
												sizeof(InitiatingMessage_t));
			DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
			RETVALUE(RFAILED);
		}
	}

	idx = 0;

	/*GNB CU UE F1AP ID*/
	ulRRCMsg->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
	ulRRCMsg->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ulRRCMsg->protocolIEs.list.array[idx]->value.present = \
                    		ULRRCMessageTransferIEs__value_PR_GNB_CU_UE_F1AP_ID;
	ulRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = CU_ID;

	/*GNB DU UE F1AP ID*/
	idx++;
	ulRRCMsg->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
	ulRRCMsg->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ulRRCMsg->protocolIEs.list.array[idx]->value.present = \
                    		ULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
	ulRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = DU_ID;

	/*SRBID*/
	idx++;
	ulRRCMsg->protocolIEs.list.array[idx]->id	= \
									 			                 ProtocolIE_ID_id_SRBID;
	ulRRCMsg->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ulRRCMsg->protocolIEs.list.array[idx]->value.present = \
				                    		ULRRCMessageTransferIEs__value_PR_SRBID;
	ulRRCMsg->protocolIEs.list.array[idx]->value.choice.SRBID = UL_SRBID;

	/*RRCContainer*/
	//YET TO FILL

	xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

	/* Encode the F1SetupRequest type as APER */
	cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
	encBufSize = 0;
	encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
			encBuf);
	/* Encode results */
	if(encRetVal.encoded == ENCODE_FAIL)
	{
		DU_LOG( "\n F1AP : Could not encode ULRRCMessageTransfer structure (at %s)\n",\
				encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		RETVALUE(RFAILED);
	}
	else
	{
		DU_LOG("\n F1AP : Created APER encoded buffer for ULRRCMessageTransfer\n");
		for(int i=0; i< encBufSize; i++)
		{
			printf("%x",encBuf[i]);
		}
	}

	/* Sending  msg  */
	if(SendF1APMsg(DU_APP_MEM_REGION,DU_POOL)	!=	ROK)
	{
		DU_LOG("\n F1AP : Sending	UL RRC Message Transfer Failed");
		RETVALUE(RFAILED);
	}
   return ROK;
}/* End of BuildAndSendULRRCMessageTransfer*/

/*******************************************************************
 *
 * @brief Builds and sends the InitialULRRCMessage 
 *
 * @details
 *
 *    Function : BuildAndSendRRCSetupReq
 *
 *    Functionality: Constructs the Initial UL RRC Message Transfer and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildAndSendRRCSetupReq()
{
	S16  ret;
	U8   elementCnt;
	U8   ieId;
	U8   idx;
	F1AP_PDU_t      					*f1apMsg = NULL;
   InitialULRRCMessageTransfer_t	*initULRRCMsg;
	asn_enc_rval_t  					encRetVal;        /* Encoder return value */

	DU_LOG("\n F1AP : Building RRC Setup Request\n");

	DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
	if(f1apMsg == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
		RETVALUE(RFAILED);
	}

	f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
	DU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
	if(f1apMsg->choice.initiatingMessage == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	f1apMsg->choice.initiatingMessage->procedureCode = \
									ProcedureCode_id_InitialULRRCMessageTransfer;
	f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
	f1apMsg->choice.initiatingMessage->value.present = \
					 InitiatingMessage__value_PR_InitialULRRCMessageTransfer;
	initULRRCMsg =
		&f1apMsg->choice.initiatingMessage->value.choice.InitialULRRCMessageTransfer;
	elementCnt = 3;
	initULRRCMsg->protocolIEs.list.count = elementCnt;
	initULRRCMsg->protocolIEs.list.size = \
									elementCnt * sizeof(InitialULRRCMessageTransferIEs_t *);

	/* Initialize the F1Setup members */
	DU_ALLOC(initULRRCMsg->protocolIEs.list.array, \
			elementCnt * sizeof(InitialULRRCMessageTransferIEs_t *));
	if(initULRRCMsg->protocolIEs.list.array == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for RRCSetupRequestMessageTransferIEs failed");
		DU_FREE(f1apMsg->choice.initiatingMessage,
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	for(idx=0; idx<elementCnt; idx++)
	{
		DU_ALLOC(initULRRCMsg->protocolIEs.list.array[idx],\
										sizeof(InitialULRRCMessageTransferIEs_t));
		if(initULRRCMsg->protocolIEs.list.array[idx] == NULLP)
		{
			for(ieId=0; ieId<idx; ieId++)
			{
				DU_FREE(initULRRCMsg->protocolIEs.list.array[ieId],\
						sizeof(InitialULRRCMessageTransferIEs_t));
			}
			DU_FREE(initULRRCMsg->protocolIEs.list.array,\
					elementCnt * sizeof(InitialULRRCMessageTransferIEs_t *));
			DU_FREE(f1apMsg->choice.initiatingMessage,\
												sizeof(InitiatingMessage_t));
			DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
			RETVALUE(RFAILED);
		}
	}

	idx = 0;

	/*GNB DU UE F1AP ID*/
	initULRRCMsg->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
	initULRRCMsg->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	initULRRCMsg->protocolIEs.list.array[idx]->value.present = \
                    		InitialULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
	initULRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = DU_ID;

	/*NRCGI*/
	idx++;
	initULRRCMsg->protocolIEs.list.array[idx]->id	= \
									 			                 ProtocolIE_ID_id_NRCGI;
	initULRRCMsg->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	initULRRCMsg->protocolIEs.list.array[idx]->value.present = \
				                    		InitialULRRCMessageTransferIEs__value_PR_NRCGI;
 	ret = \
	BuildNrcgi(&initULRRCMsg->protocolIEs.list.array[idx]->value.choice.NRCGI);
	if(ret != ROK)
	{
		DU_FREE(initULRRCMsg->protocolIEs.list.array[idx]->value.choice.NRCGI.\
					nRCellIdentity.buf,initULRRCMsg->protocolIEs.list.array[idx]->\
					value.choice.NRCGI.nRCellIdentity.size);
		DU_FREE(initULRRCMsg->protocolIEs.list.array[idx]->value.choice.NRCGI.\
					pLMN_Identity.buf,initULRRCMsg->protocolIEs.list.array[idx]->\
					value.choice.NRCGI.pLMN_Identity.size);
		for(idx=0; idx<elementCnt; idx++)
		{
			DU_FREE(initULRRCMsg->protocolIEs.list.array[idx],\
					sizeof(InitialULRRCMessageTransferIEs_t));
		}
		DU_FREE(initULRRCMsg->protocolIEs.list.array,\
				elementCnt * sizeof(InitialULRRCMessageTransferIEs_t *));
		DU_FREE(f1apMsg->choice.initiatingMessage,\
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);

	}

	/*Cell RNTI*/
	idx++;
	initULRRCMsg->protocolIEs.list.array[idx]->id	= \
									 			                 ProtocolIE_ID_id_C_RNTI;
	initULRRCMsg->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	initULRRCMsg->protocolIEs.list.array[idx]->value.present = \
		                    		InitialULRRCMessageTransferIEs__value_PR_C_RNTI;
	initULRRCMsg->protocolIEs.list.array[idx]->value.choice.C_RNTI = CRNTI;

	/*RRCContainer*/
	// Need to fill this.

	/*DUtoCURRCContainer*/
	//Need to fill this too.


	xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

	/* Encode the F1SetupRequest type as APER */
	cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
	encBufSize = 0;
	encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
			encBuf);
	/* Encode results */
	if(encRetVal.encoded == ENCODE_FAIL)
	{
		DU_LOG( "\n F1AP : Could not encode Initial UL RRC Message Transfer structure (at %s)\n",\
				encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		RETVALUE(RFAILED);
	}
	else
	{
		DU_LOG("\n F1AP : Created APER encoded buffer for Initial UL RRC Message transfer\n");
		for(int i=0; i< encBufSize; i++)
		{
			printf("%x",encBuf[i]);
		}
	}

	/* Sending  msg  */
	if(SendF1APMsg(DU_APP_MEM_REGION,DU_POOL)	!=	ROK)
	{
		DU_LOG("\n F1AP : Sending	Initial UL RRC Message Transfer Failed");
		RETVALUE(RFAILED);
	}
   return ROK;
}/* End of BuildAndSendRRCSetupReq*/

/*******************************************************************
 *
 * @brief Builds Special cell list for UE Setup Request 
 *
 * @details
 *
 *    Function : BuildSplCellList
 *
 *    Functionality: Constructs the Special Cell list for UESetReq
 *
 * @params[in] SCell_ToBeSetup_List_t *spCellLst
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildSplCellList(SCell_ToBeSetup_List_t *spCellLst)
{
	U8  cellCnt;
	U8  idx;
	U8  cellidx;
	S16 ret;
	cellCnt = 1;
	spCellLst->list.count = cellCnt;
	spCellLst->list.size = cellCnt * sizeof(struct SCell_ToBeSetup_ItemIEs *);
	DU_ALLOC(spCellLst->list.array,spCellLst->list.size);
	if(spCellLst->list.array == NULLP)
	{
		RETVALUE(RFAILED);
	}
	for(idx=0; idx<cellCnt; idx++)
	{
		DU_ALLOC(spCellLst->list.array[idx],sizeof(struct SCell_ToBeSetup_ItemIEs));
		if(spCellLst->list.array[idx] == NULLP)
		{
			for(cellidx=0; cellidx<idx; cellidx++)
			{
				DU_FREE(spCellLst->list.array[cellidx],sizeof(struct SCell_ToBeSetup_ItemIEs));
			}
			DU_FREE(spCellLst->list.array,spCellLst->list.size);
			RETVALUE(RFAILED);
		}
	}
	idx = 0;
	spCellLst->list.array[idx]->id = ProtocolIE_ID_id_SCell_ToBeSetup_Item;
	spCellLst->list.array[idx]->criticality = Criticality_ignore;
	spCellLst->list.array[idx]->value.present =\
		SCell_ToBeSetup_ItemIEs__value_PR_SCell_ToBeSetup_Item;
	/* Special Cell ID -NRCGI */
	ret = BuildNrcgi(&spCellLst->list.array[idx]->value.choice.SCell_ToBeSetup_Item.sCell_ID);
	if(ret != ROK)
	{
		for(cellidx=0; cellidx<cellCnt; cellidx++)
		{
			DU_FREE(spCellLst->list.array[cellidx],sizeof(struct
			SCell_ToBeSetup_ItemIEs));
		}
		DU_FREE(spCellLst->list.array,spCellLst->list.size);
		RETVALUE(RFAILED);
	}
	/*Special Cell Index*/
	spCellLst->list.array[idx]->value.choice.SCell_ToBeSetup_Item.sCellIndex = 1;
  return ROK;	
}/* End of BuildSplCellList*/

/*******************************************************************
 *
 * @brief Builds SRBS to be setup 
 *
 * @details
 *
 *    Function : BuildSRBSetup
 *
 *    Functionality: Constructs the SRB's for UESetReq
 *
 * @params[in] SRBs_ToBeSetup_List_t *srbSet
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildSRBSetup(SRBs_ToBeSetup_List_t *srbSet)
{
	U8 idx;
	U8 srbidx;
	U8 srbCnt;
	srbCnt = 1;
	srbSet->list.count = srbCnt;
	srbSet->list.size = srbCnt*sizeof(struct SRBs_ToBeSetup_ItemIEs *);
	DU_ALLOC(srbSet->list.array,srbSet->list.size);
	if(srbSet->list.array == NULLP)
	{
		RETVALUE(RFAILED);
	}
	for(idx=0; idx<srbCnt; idx++)
	{
		DU_ALLOC(srbSet->list.array[idx],sizeof(struct SRBs_ToBeSetup_ItemIEs));
		if(srbSet->list.array[idx] == NULLP)
		{
			for(srbidx=0; srbidx<idx; srbidx++)
			{
				DU_FREE(srbSet->list.array[srbidx],sizeof(struct SRBs_ToBeSetup_ItemIEs));
			}
			DU_FREE(srbSet->list.array,srbSet->list.size);
		}
	}
	idx = 0;
	srbSet->list.array[idx]->id = ProtocolIE_ID_id_SRBs_ToBeSetup_Item;
	srbSet->list.array[idx]->criticality = Criticality_ignore;
	srbSet->list.array[idx]->value.present = \
			SRBs_ToBeSetup_ItemIEs__value_PR_SRBs_ToBeSetup_Item;
   srbSet->list.array[idx]->value.choice.SRBs_ToBeSetup_Item.sRBID = 2;
   return ROK;
}/* End of BuildSRBSetup*/

/*******************************************************************
 *
 * @brief Builds QOS Info for DRB Setum Item 
 *
 * @details
 *
 *    Function : BuildQOSInfo
 *
 *    Functionality: Constructs the QOS Info for DRB Setup Item
 *
 * @params[in] QoSInformation_t *qosinfo
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildQOSInfo(QoSFlowLevelQoSParameters_t *drbQos)
{
	/* NonDynamic5QIDescriptor */
	drbQos->qoS_Characteristics.present = QoS_Characteristics_PR_non_Dynamic_5QI;
	DU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
	if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI == NULLP)
	{
		RETVALUE(RFAILED);
	}
	/*FiveQI*/
 	drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI = 0;
	/*AveragingWindow*/
	DU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
												sizeof(AveragingWindow_t));
	if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow == \
																									NULLP)
	{
		DU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,\
						sizeof(NonDynamic5QIDescriptor_t));
		RETVALUE(RFAILED);
	}
	*(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow) = 0;
	/*MaxDataBurstVolume*/
	DU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
												sizeof(MaxDataBurstVolume_t));
	if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume == \
																									NULLP)
	{
		DU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
												sizeof(AveragingWindow_t));
		DU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,\
						sizeof(NonDynamic5QIDescriptor_t));
		RETVALUE(RFAILED);
	}
	*(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume) = 0;

	/*nRGRAN Allocation Retention Priority*/
	drbQos->nGRANallocationRetentionPriority.priorityLevel =	\
												PriorityLevel_highest;
	drbQos->nGRANallocationRetentionPriority.pre_emptionCapability = \
							Pre_emptionCapability_may_trigger_pre_emption;
	drbQos->nGRANallocationRetentionPriority.pre_emptionVulnerability = \
								Pre_emptionVulnerability_not_pre_emptable;

	/* TO DO: GBR_QoSFlowInformation */
        return ROK;
}/*End of BuildQOSInfo*/

/*******************************************************************
 *
 * @brief Builds SNSSAI  
 *
 * @details
 *
 *    Function : BuildSNSSAI
 *
 *    Functionality: Constructs the SNSSAI For DRB list
 *
 * @params[in] SNSSAI_t *snssai
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildSNSSAI(SNSSAI_t *snssai)
{
	/*SNSSAI*/
	  /*ssT*/
	snssai->sST.size = sizeof(U8);
	DU_ALLOC(snssai->sST.buf,snssai->sST.size);
	if(snssai->sST.buf == NULLP)
	{
		RETVALUE(RFAILED);
	}
	snssai->sST.buf[0] = 3;
	  /*sD*/
	DU_ALLOC(snssai->sD,sizeof(OCTET_STRING_t));
	if(snssai->sD == NULLP)
	{
		DU_FREE(snssai->sST.buf,snssai->sST.size);
		RETVALUE(RFAILED);
	}
	snssai->sD->size = 3*sizeof(U8);
	DU_ALLOC(snssai->sD->buf,snssai->sD->size);
	if(snssai->sD->buf == NULLP)
	{
		DU_FREE(snssai->sD,sizeof(OCTET_STRING_t));
		DU_FREE(snssai->sST.buf,snssai->sST.size);
		RETVALUE(RFAILED);
	}
		snssai->sD->buf[0] = 3;
		snssai->sD->buf[1] = 6;
		snssai->sD->buf[2] = 9;
   return ROK;
}/*End of BuildSNSSAI*/

/*******************************************************************
 *
 * @brief Builds the flow map.  
 *
 * @details
 *
 *    Function : BuildFlowsMap
 *
 *    Functionality: Constructs the flowmap For DRB list
 *
 * @params[in] Flows_Mapped_To_DRB_List_t *flowMap
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildFlowsMap(Flows_Mapped_To_DRB_List_t *flowMap)
{
	S16 ret;
	U8  idx;
	U8  flowidx;
	U8 flowCnt;
	flowCnt = 1;
	flowMap->list.count = flowCnt;
	flowMap->list.size = flowCnt * sizeof(struct Flows_Mapped_To_DRB_Item *);
	DU_ALLOC(flowMap->list.array,flowMap->list.size);
	if(flowMap->list.array == NULLP)
	{
		RETVALUE(RFAILED);
	}
	for(idx=0; idx<flowCnt; idx++)
	{
		DU_ALLOC(flowMap->list.array[idx],sizeof(struct Flows_Mapped_To_DRB_Item));
		if(flowMap->list.array[idx] == NULLP)
		{
			for(flowidx=0; flowidx<idx; flowidx++)
			{
				DU_FREE(flowMap->list.array[flowidx],sizeof(struct \
							Flows_Mapped_To_DRB_Item));
			}

			DU_FREE(flowMap->list.array,flowMap->list.size);
		}
	}
	idx = 0;
	flowMap->list.array[idx]->qoSFlowIdentifier = 0;
	ret = BuildQOSInfo(&flowMap->list.array[idx]->qoSFlowLevelQoSParameters);
	if(ret != ROK)
	{
		DU_FREE(flowMap->list.array[idx]->qoSFlowLevelQoSParameters.\
			qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
			sizeof(MaxDataBurstVolume_t));
		DU_FREE(flowMap->list.array[idx]->qoSFlowLevelQoSParameters.\
			qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
			sizeof(AveragingWindow_t));
		DU_FREE(flowMap->list.array[idx]->qoSFlowLevelQoSParameters.\
			qoS_Characteristics.choice.non_Dynamic_5QI,\
			sizeof(NonDynamic5QIDescriptor_t));
		for(idx=0; idx<flowCnt; idx++)
		{
			DU_FREE(flowMap->list.array[idx],sizeof(struct Flows_Mapped_To_DRB_Item));
		}
		DU_FREE(flowMap->list.array,flowMap->list.size);
		RETVALUE(RFAILED);
	}
   return ROK;
}/*End of BuildFlowsMap*/

/*******************************************************************
 *
 * @brief Builds the Uplink Tunnel Info  
 *
 * @details
 *
 *    Function : BuildULTnlInfo
 *
 *    Functionality: Constructs the UL TnlInfo For DRB list
 *
 * @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildULTnlInfo(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
	U8 idx;
	U8 ulidx;
	U8 ulCnt;
	ulCnt = 1;
	ulInfo->list.count = ulCnt;
	ulInfo->list.size = ulCnt * sizeof(struct \
				ULUPTNLInformation_ToBeSetup_Item *);
	DU_ALLOC(ulInfo->list.array,ulInfo->list.size);
	if(ulInfo->list.array == NULLP)
	{
		RETVALUE(RFAILED);
	}
	for(idx=0; idx<ulCnt; idx++)
	{
		DU_ALLOC(ulInfo->list.array[idx],sizeof(struct \
					ULUPTNLInformation_ToBeSetup_Item));
		if(ulInfo->list.array[idx] == NULLP)
		{
			for(ulidx=0; ulidx<idx; ulidx++)
			{
				DU_FREE(ulInfo->list.array[ulidx],sizeof(struct \
							ULUPTNLInformation_ToBeSetup_Item));
			}
			DU_FREE(ulInfo->list.array,ulInfo->list.size);
			RETVALUE(RFAILED);
		}
	}
	idx = 0;
	ulInfo->list.array[idx]->uLUPTNLInformation.present = \
								UPTransportLayerInformation_PR_gTPTunnel;
	/*GTP TUNNEL*/
	DU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel,\
				sizeof(struct GTPTunnel));
	if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel == NULLP)
	{
		for(idx=0; idx<ulCnt; idx++)
		{
			DU_FREE(ulInfo->list.array[idx],sizeof(struct \
						ULUPTNLInformation_ToBeSetup_Item));
		}
		DU_FREE(ulInfo->list.array,ulInfo->list.size);
		RETVALUE(RFAILED);
	}
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.size	= 4*sizeof(U8);
   DU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf,ulInfo->list.array[idx]->\
		uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
	if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf == NULLP)
	{
		DU_FREE(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel,\
				sizeof(struct GTPTunnel));
		for(idx=0; idx<ulCnt; idx++)
		{
			DU_FREE(ulInfo->list.array[idx],sizeof(struct \
						ULUPTNLInformation_ToBeSetup_Item));
		}
		DU_FREE(ulInfo->list.array,ulInfo->list.size);
		RETVALUE(RFAILED);
	}
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf[0] = 4;
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf[1] = 4;
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf[2] = 4;
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf[3] = 5;
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.bits_unused = 0;
	/*GTP TEID*/
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size\
					= 4 * sizeof(U8);
	DU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
				gTP_TEID.buf,ulInfo->list.array[idx]->uLUPTNLInformation.choice.\
				gTPTunnel->gTP_TEID.size);
	if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
				== NULLP)
	{
		DU_FREE(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
				transportLayerAddress.buf,ulInfo->list.array[idx]->\
				uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
		DU_FREE(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel,\
				sizeof(struct GTPTunnel));
		for(idx=0; idx<ulCnt; idx++)
		{
			DU_FREE(ulInfo->list.array[idx],sizeof(struct \
						ULUPTNLInformation_ToBeSetup_Item));
		}
		DU_FREE(ulInfo->list.array,ulInfo->list.size);
		RETVALUE(RFAILED);
	}
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		gTP_TEID.buf[0] = 11;
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		gTP_TEID.buf[1] = 0;
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		gTP_TEID.buf[2] = 0;
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		gTP_TEID.buf[3] = 1;

   return ROK;
}/*End of BuildULTnlInfo*/

/*******************************************************************
 *
 * @brief Builds DRBS to be setup 
 *
 * @details
 *
 *    Function : BuildDRBSetup
 *
 *    Functionality: Constructs the DRB's for UESetReq
 *
 * @params[in] DRBs_ToBeSetup_List_t *drbSet
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildDRBSetup(DRBs_ToBeSetup_List_t *drbSet)
{
	S16 ret;
	U8	 idx;
	U8  drbidx;
	U8  drbCnt;
	U8  flowidx;
	U8  flowCnt;
	U8  ulidx;
	U8  ulCnt;
	DRBs_ToBeSetup_Item_t *drbSetItem;
	drbCnt = 1;
	drbSet->list.count = drbCnt;
	drbSet->list.size = drbCnt*sizeof(struct DRBs_ToBeSetup_ItemIEs *);
	DU_ALLOC(drbSet->list.array,drbSet->list.size);
	if(drbSet->list.array == NULLP)
	{
		RETVALUE(RFAILED);
	}
	for(idx=0; idx<drbCnt; idx++)
	{
		DU_ALLOC(drbSet->list.array[idx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
		if(drbSet->list.array[idx] == NULLP)
		{
			for(drbidx=0; drbidx<idx; drbidx++)
			{
				DU_FREE(drbSet->list.array[drbidx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
			}
			DU_FREE(drbSet->list.array,drbSet->list.size);
			RETVALUE(RFAILED);
		}
	}
	idx = 0;
	drbSet->list.array[idx]->id = ProtocolIE_ID_id_DRBs_ToBeSetup_Item;
	drbSet->list.array[idx]->criticality = Criticality_ignore;
	drbSet->list.array[idx]->value.present = \
			DRBs_ToBeSetup_ItemIEs__value_PR_DRBs_ToBeSetup_Item;
   drbSetItem = &drbSet->list.array[idx]->value.choice.DRBs_ToBeSetup_Item;	
	/*dRBID*/
	drbSetItem->dRBID = 1;	
	/*qoSInformation*/
	drbSetItem->qoSInformation.present = QoSInformation_PR_choice_extension;
	DU_ALLOC(drbSetItem->qoSInformation.choice.choice_extension,sizeof(struct\
					QoSInformation_ExtIEs));
	if(drbSetItem->qoSInformation.choice.choice_extension == NULLP)
	{	
		for(idx=0; idx<drbCnt; idx++)
		{
			DU_FREE(drbSet->list.array[idx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(drbSet->list.array,drbSet->list.size);
		RETVALUE(RFAILED);
	}
	drbSetItem->qoSInformation.choice.choice_extension->id = \
							ProtocolIE_ID_id_DRB_Information;
	drbSetItem->qoSInformation.choice.choice_extension->criticality = \
							Criticality_ignore;
	drbSetItem->qoSInformation.choice.choice_extension->value.present = \
							QoSInformation_ExtIEs__value_PR_DRB_Information;
	ret = BuildQOSInfo(&drbSetItem->qoSInformation.choice.\
							choice_extension->value.choice.DRB_Information.dRB_QoS);
	if(ret != ROK)
	{
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(struct\
					QoSInformation_ExtIEs));
		for(drbidx=0; drbidx<drbCnt; drbidx++)
		{
			DU_FREE(drbSet->list.array[drbidx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(drbSet->list.array,drbSet->list.size);
		RETVALUE(RFAILED);
	}
	/*SNSSAI*/
 	ret = BuildSNSSAI(&drbSetItem->qoSInformation.choice.\
							choice_extension->value.choice.DRB_Information.sNSSAI);
	if(ret != ROK)
	{	
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(struct\
					QoSInformation_ExtIEs));
		for(drbidx=0; drbidx<drbCnt; drbidx++)
		{
			DU_FREE(drbSet->list.array[drbidx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(drbSet->list.array,drbSet->list.size);
		RETVALUE(RFAILED);
	}
	/*Flows mapped to DRB List*/
	ret = BuildFlowsMap(&drbSetItem->qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.flows_Mapped_To_DRB_List);
	if(ret != ROK)
	{
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sD,sizeof(OCTET_STRING_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sD->buf,drbSetItem->qoSInformation.\
				choice.choice_extension->value.choice.DRB_Information.\
				sNSSAI.sD->size);
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sST.buf,drbSetItem->qoSInformation.\
				choice.choice_extension->value.choice.DRB_Information.\
				sNSSAI.sST.size);
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(struct\
					QoSInformation_ExtIEs));
		for(drbidx=0; drbidx<drbCnt; drbidx++)
		{
			DU_FREE(drbSet->list.array[drbidx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(drbSet->list.array,drbSet->list.size);
		RETVALUE(RFAILED);
	}
	/*ULUPTNLInformation To Be Setup List*/
   ret = BuildULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
	if(ret != ROK)
	{
		flowidx=0;
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array[flowidx]->qoSFlowLevelQoSParameters.qoS_Characteristics.\
				choice.non_Dynamic_5QI->maxDataBurstVolume,\
				sizeof(MaxDataBurstVolume_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array[flowidx]->qoSFlowLevelQoSParameters.qoS_Characteristics.\
				choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array[flowidx]->qoSFlowLevelQoSParameters.qoS_Characteristics.\
				choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array,drbSetItem->qoSInformation.choice.choice_extension->\
				value.choice.DRB_Information.flows_Mapped_To_DRB_List.list.size);
				flowCnt = 1;
		for(flowidx=0; flowidx<flowCnt;flowidx++)
		{
			DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
					choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
					array[flowidx],sizeof(struct Flows_Mapped_To_DRB_Item));
		}
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sD,sizeof(OCTET_STRING_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sD->buf,drbSetItem->qoSInformation.\
				choice.choice_extension->value.choice.DRB_Information.\
				sNSSAI.sD->size);
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sST.buf,drbSetItem->qoSInformation.\
				choice.choice_extension->value.choice.DRB_Information.\
				sNSSAI.sST.size);
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(struct\
					QoSInformation_ExtIEs));
		for(drbidx=0; drbidx<drbCnt; drbidx++)
		{
			DU_FREE(drbSet->list.array[drbidx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(drbSet->list.array,drbSet->list.size);
		RETVALUE(RFAILED);
	}
	/*RLCMode*/
	drbSetItem->rLCMode = RLCMode_rlc_um_bidirectional;

	/*UL Configuration*/
	ulCnt = 1;
	DU_ALLOC(drbSetItem->uLConfiguration,sizeof(struct ULConfiguration));
	if(drbSetItem->uLConfiguration == NULLP)
	{
		ulidx=0;
		DU_FREE(drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.array[ulidx]->\
				uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf,\
				drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.array[ulidx]->\
				uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size);
		DU_FREE(drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.array[ulidx]->\
				uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf,\
				drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.array[ulidx]->\
				uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
		DU_FREE(drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.array[ulidx]->\
				uLUPTNLInformation.choice.gTPTunnel,sizeof(struct GTPTunnel));
		DU_FREE(drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.array,\
				drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.size);
		for(ulidx=0; ulidx<ulCnt; ulidx++)
		{
			DU_FREE(drbSetItem->uLUPTNLInformation_ToBeSetup_List.list.array[ulidx],\
					sizeof(struct ULUPTNLInformation_ToBeSetup_Item));
		}
		flowidx=0;
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array[flowidx]->qoSFlowLevelQoSParameters.qoS_Characteristics.\
				choice.non_Dynamic_5QI->maxDataBurstVolume,\
				sizeof(MaxDataBurstVolume_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array[flowidx]->qoSFlowLevelQoSParameters.qoS_Characteristics.\
				choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array[flowidx]->qoSFlowLevelQoSParameters.qoS_Characteristics.\
				choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
				array,drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.flows_Mapped_To_DRB_List.list.size);
		flowCnt = 1;
		for(flowidx=0; flowidx<flowCnt;flowidx++)
		{
			DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
					choice.DRB_Information.flows_Mapped_To_DRB_List.list.\
					array[flowidx],sizeof(struct Flows_Mapped_To_DRB_Item));
		}
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sD,sizeof(OCTET_STRING_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sD->buf,drbSetItem->qoSInformation.\
				choice.choice_extension->value.choice.DRB_Information.\
				sNSSAI.sD->size);
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.sNSSAI.sST.buf,drbSetItem->qoSInformation.\
				choice.choice_extension->value.choice.DRB_Information.\
				sNSSAI.sST.size);
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.\
				choice.DRB_Information.dRB_QoS.qoS_Characteristics.choice.\
				non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
		DU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(struct\
					QoSInformation_ExtIEs));
		for(drbidx=0; drbidx<drbCnt; drbidx++)
		{
			DU_FREE(drbSet->list.array[drbidx],sizeof(struct DRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(drbSet->list.array,drbSet->list.size);
		RETVALUE(RFAILED);
	}
	drbSetItem->uLConfiguration->uLUEConfiguration = ULUEConfiguration_no_data;
        return ROK;
}/* End of BuildDRBSetup*/

/*******************************************************************
 *
 * @brief Builds and sends the UE Setup Request 
 *
 * @details
 *
 *    Function : BuildAndSendUESetReq
 *
 *    Functionality: Constructs the UE Setup Request and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildAndSendUESetReq()
{
	S16  ret;
	U8   elementCnt;
	U8   cellCnt;
	U8   ieId;
	U8   idx;
	U8   spId;
	U8   srbCnt;
	U8   srbId;
	U8   drbCnt;
	U8   drbId;
	U8   flowidx;
	U8   flowCnt;
	U8   ulidx;
	U8   ulCnt;
	F1AP_PDU_t      					*f1apMsg = NULL;
   UEContextSetupRequest_t			*ueSetReq;
	asn_enc_rval_t  					encRetVal;        /* Encoder return value */

	DU_LOG("\n F1AP : Building UE Context Setup Request\n");

	DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
	if(f1apMsg == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
		RETVALUE(RFAILED);
	}

	f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
	DU_ALLOC(f1apMsg->choice.initiatingMessage,
			sizeof(InitiatingMessage_t));
	if(f1apMsg->choice.initiatingMessage == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	f1apMsg->choice.initiatingMessage->procedureCode = \
													ProcedureCode_id_UEContextSetup;
	f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
	f1apMsg->choice.initiatingMessage->value.present = \
		      		   InitiatingMessage__value_PR_UEContextSetupRequest;
	ueSetReq =
		&f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;
	elementCnt = 9;
	ueSetReq->protocolIEs.list.count = elementCnt;
	ueSetReq->protocolIEs.list.size = \
									elementCnt * sizeof(UEContextSetupRequestIEs_t *);

	/* Initialize the UESetup members */
	DU_ALLOC(ueSetReq->protocolIEs.list.array, \
			elementCnt * sizeof(UEContextSetupRequestIEs_t *));
	if(ueSetReq->protocolIEs.list.array == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for UE Context SetupRequest failed");
		DU_FREE(f1apMsg->choice.initiatingMessage,
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	for(idx=0; idx<elementCnt; idx++)
	{
		DU_ALLOC(ueSetReq->protocolIEs.list.array[idx],\
										sizeof(UEContextSetupRequestIEs_t));
		if(ueSetReq->protocolIEs.list.array[idx] == NULLP)
		{
			for(ieId=0; ieId<idx; ieId++)
			{
				DU_FREE(ueSetReq->protocolIEs.list.array[ieId],\
						sizeof(UEContextSetupRequestIEs_t));
			}
			DU_FREE(ueSetReq->protocolIEs.list.array,\
					elementCnt * sizeof(UEContextSetupRequestIEs_t *));
			DU_FREE(f1apMsg->choice.initiatingMessage,\
												sizeof(InitiatingMessage_t));
			DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
			RETVALUE(RFAILED);
		}
	}

	idx = 0;

	/*GNB CU UE F1AP ID*/
	ueSetReq->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
		                 		UEContextSetupRequestIEs__value_PR_GNB_CU_UE_F1AP_ID;
	ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = CU_ID;

	/*GNB DU UE F1AP ID*/
 	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
	              				UEContextSetupRequestIEs__value_PR_GNB_DU_UE_F1AP_ID;
	ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = DU_ID;

	/*Special Cell ID*/
	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
								 			                 ProtocolIE_ID_id_SpCell_ID;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
				                    		UEContextSetupRequestIEs__value_PR_NRCGI;
 	ret = \
	BuildNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI);
	if(ret != ROK)
	{
		idx =2;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
			nRCellIdentity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
			choice.NRCGI.nRCellIdentity.size));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
			pLMN_Identity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
			choice.NRCGI.pLMN_Identity.size));
		for(idx=0; idx<elementCnt; idx++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx],\
					sizeof(InitialULRRCMessageTransferIEs_t));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array,\
				elementCnt * sizeof(InitialULRRCMessageTransferIEs_t *));
		DU_FREE(f1apMsg->choice.initiatingMessage,\
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);

	}

	/*Served Cell Index*/
	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
								 		                 ProtocolIE_ID_id_ServCellIndex;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
		                    		UEContextSetupRequestIEs__value_PR_ServCellIndex;
	ueSetReq->protocolIEs.list.array[idx]->value.choice.ServCellIndex = \
	CELL_INDEX;

	/*CellULConfigured*/
	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
							 		                ProtocolIE_ID_id_SpCellULConfigured;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
									UEContextSetupRequestIEs__value_PR_CellULConfigured;
	ueSetReq->protocolIEs.list.array[idx]->value.choice.CellULConfigured = \
																			CellULConfigured_none;


	/*CUtoDURRCContainer*/
	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
												  ProtocolIE_ID_id_CUtoDURRCInformation;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
		                    		UEContextSetupRequestIEs__value_PR_CUtoDURRCInformation;

	/*Special Cells to be SetupList*/
	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
												  ProtocolIE_ID_id_SCell_ToBeSetup_List;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
		                    		UEContextSetupRequestIEs__value_PR_SCell_ToBeSetup_List;
	cellCnt = 1;
	ret = BuildSplCellList(&ueSetReq->protocolIEs.\
					list.array[idx]->value.choice.SCell_ToBeSetup_List);
	if(ret != ROK)
	{  idx=6;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.nRCellIdentity.buf,\
				ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.nRCellIdentity.size);
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.pLMN_Identity.buf,\
				ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.pLMN_Identity.size);
		for(spId=0; spId<cellCnt; spId++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
					SCell_ToBeSetup_List.list.array[spId],sizeof(struct
						SCell_ToBeSetup_ItemIEs));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array,ueSetReq->protocolIEs.list.\
				array[idx]->value.choice.SCell_ToBeSetup_List.list.size);
		idx =2;
		idx=idx-4;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
				nRCellIdentity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
					choice.NRCGI.nRCellIdentity.size));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
				pLMN_Identity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
					choice.NRCGI.pLMN_Identity.size));
		for(idx=0; idx<elementCnt; idx++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx],\
					sizeof(UEContextSetupRequestIEs_t));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array,\
				elementCnt * sizeof(UEContextSetupRequestIEs_t *));
		DU_FREE(f1apMsg->choice.initiatingMessage,\
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}
	/*SRBs To Be Setup List*/
 	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_SRBs_ToBeSetup_List;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
	              				UEContextSetupRequestIEs__value_PR_SRBs_ToBeSetup_List;
	srbCnt = 1;
	ret =	BuildSRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.\
															choice.SRBs_ToBeSetup_List);
	if(ret != ROK)
	{        
                idx =7;
		for(srbId=0; srbId<srbCnt; srbId++)
		{
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SRBs_ToBeSetup_List.list.array[srbId],\
				sizeof(struct SRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SRBs_ToBeSetup_List.list.array,ueSetReq->protocolIEs.list.\
				array[idx]->value.choice.SRBs_ToBeSetup_List.list.size);
		idx=6;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.nRCellIdentity.buf,\
				ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.nRCellIdentity.size);
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.pLMN_Identity.buf,\
				ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.pLMN_Identity.size);
		for(spId=0; spId<cellCnt; spId++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
					SCell_ToBeSetup_List.list.array[spId],sizeof(struct
						SCell_ToBeSetup_ItemIEs));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array,ueSetReq->protocolIEs.list.\
				array[idx]->value.choice.SCell_ToBeSetup_List.list.size);
		idx=2;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
			nRCellIdentity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
			choice.NRCGI.nRCellIdentity.size));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
			pLMN_Identity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
			choice.NRCGI.pLMN_Identity.size));
		for(idx=0; idx<elementCnt; idx++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx],\
					sizeof(UEContextSetupRequestIEs_t));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array,\
				elementCnt * sizeof(UEContextSetupRequestIEs_t *));
		DU_FREE(f1apMsg->choice.initiatingMessage,\
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}
	/*DRBs to Be Setup List*/
	idx++;
	ueSetReq->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_DRBs_ToBeSetup_List;
	ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetReq->protocolIEs.list.array[idx]->value.present = \
	              				UEContextSetupRequestIEs__value_PR_DRBs_ToBeSetup_List;
	drbCnt = 1;
	flowCnt = 1;
	ulCnt = 1;
	ret = BuildDRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.\
															choice.DRBs_ToBeSetup_List);
	if(ret != ROK)
	{	idx=8;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.uLConfiguration,\
				sizeof(struct ULConfiguration));
		ulidx=0;		
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.uLUPTNLInformation_ToBeSetup_List.list.\
				array[ulidx]->uLUPTNLInformation.choice.gTPTunnel,\
				sizeof(struct GTPTunnel));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.uLUPTNLInformation_ToBeSetup_List.list.\
				array,ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.uLUPTNLInformation_ToBeSetup_List.list.size);
		for(ulidx=0; ulidx<ulCnt;ulidx++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
					DRBs_ToBeSetup_List.list.array[0]->value.choice.\
					DRBs_ToBeSetup_Item.uLUPTNLInformation_ToBeSetup_List.list.\
					array[ulidx],sizeof(struct ULUPTNLInformation_ToBeSetup_Item));
		}
		flowidx = 0;	
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.\
				flows_Mapped_To_DRB_List.list.array[idx]->qoSFlowLevelQoSParameters.\
				qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
				sizeof(AveragingWindow_t));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.\
				flows_Mapped_To_DRB_List.list.array[idx]->qoSFlowLevelQoSParameters.\
				qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
				sizeof(MaxDataBurstVolume_t));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.\
				flows_Mapped_To_DRB_List.list.array,ueSetReq->protocolIEs.list.\
				array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.\
				flows_Mapped_To_DRB_List.list.size);
	   for(flowidx=0;flowidx<flowCnt;flowidx++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
					DRBs_ToBeSetup_List.list.array[0]->value.choice.\
					DRBs_ToBeSetup_Item.qoSInformation.choice.\
					choice_extension->value.choice.DRB_Information.\
					flows_Mapped_To_DRB_List.list.array[flowidx],sizeof(struct
					Flows_Mapped_To_DRB_Item));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.sNSSAI.\
				sD->buf,ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.sNSSAI.\
				sD->size);
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.sNSSAI.\
				sD,sizeof(OCTET_STRING_t));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.sNSSAI.\
				sST.buf,ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.sNSSAI.\
				sST.size);	
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.dRB_QoS.\
				qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
				sizeof(MaxDataBurstVolume_t));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.dRB_QoS.\
				qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
				sizeof(AveragingWindow_t));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.dRB_QoS.\
				qoS_Characteristics.choice.non_Dynamic_5QI,\
				sizeof(NonDynamic5QIDescriptor_t));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array[0]->value.choice.\
				DRBs_ToBeSetup_Item.qoSInformation.choice.\
				choice_extension,sizeof(struct QoSInformation_ExtIEs));
		for(drbId=0; drbId<drbCnt; drbId++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
					DRBs_ToBeSetup_List.list.array[drbId],\
					sizeof(struct DRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				DRBs_ToBeSetup_List.list.array,ueSetReq->protocolIEs.list.\
				array[idx]->value.choice.DRBs_ToBeSetup_List.list.size);
		idx=7;
		for(srbId=0; srbId<srbCnt; srbId++)
		{
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SRBs_ToBeSetup_List.list.array[srbId],\
				sizeof(struct SRBs_ToBeSetup_ItemIEs));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SRBs_ToBeSetup_List.list.array,ueSetReq->protocolIEs.list.\
				array[idx]->value.choice.SRBs_ToBeSetup_List.list.size);
		idx=6;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.nRCellIdentity.buf,\
				ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.nRCellIdentity.size);
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.pLMN_Identity.buf,\
				ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array[0]->value.choice.\
				SCell_ToBeSetup_Item.sCell_ID.pLMN_Identity.size);
		for(spId=0; spId<cellCnt; spId++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
					SCell_ToBeSetup_List.list.array[spId],sizeof(struct
						SCell_ToBeSetup_ItemIEs));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.\
				SCell_ToBeSetup_List.list.array,ueSetReq->protocolIEs.list.\
				array[idx]->value.choice.SCell_ToBeSetup_List.list.size);
		idx =2;
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
			nRCellIdentity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
			choice.NRCGI.nRCellIdentity.size));
		DU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI.\
			pLMN_Identity.buf,sizeof(ueSetReq->protocolIEs.list.array[idx]->value.\
			choice.NRCGI.pLMN_Identity.size));
		for(idx=0; idx<elementCnt; idx++)
		{
			DU_FREE(ueSetReq->protocolIEs.list.array[idx],\
					sizeof(UEContextSetupRequestIEs_t));
		}
		DU_FREE(ueSetReq->protocolIEs.list.array,\
				elementCnt * sizeof(UEContextSetupRequestIEs_t *));
		DU_FREE(f1apMsg->choice.initiatingMessage,\
				sizeof(InitiatingMessage_t));
		DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

	/* Encode the F1SetupRequest type as APER */
	cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
	encBufSize = 0;
	encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
			encBuf);
	/* Encode results */
	if(encRetVal.encoded == ENCODE_FAIL)
	{
		DU_LOG( "\n F1AP : Could not encode UE Context Setup Request structure (at %s)\n",\
				encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		RETVALUE(RFAILED);
	}
	else
	{
		DU_LOG("\n F1AP : Created APER encoded buffer for UE Context Setup Request\n");
		for(int i=0; i< encBufSize; i++)
		{
			printf("%x",encBuf[i]);
		}
	}

	/* Sending  msg  */
	if(SendF1APMsg(DU_APP_MEM_REGION,DU_POOL)	!=	ROK)
	{
		DU_LOG("\n F1AP : Sending	UE Context Setup Request Failed");
		RETVALUE(RFAILED);
	}
        return ROK;
}/* End of BuildAndSendUESetReq*/

/*******************************************************************
*
* @brief Handles received F1AP message and sends back response  
*
* @details
*
*    Function : F1APMsgHdlr
*
*    Functionality:
*         - Decodes received F1AP control message
*         - Prepares response message, encodes and sends to SCTP
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void F1APMsgHdlr(Buffer *mBuf)
{
   int i;
   char *recvBuf;
   MsgLen copyCnt;
   MsgLen recvBufLen;
   F1AP_PDU_t *f1apMsg;
   asn_dec_rval_t rval; /* Decoder return value */
   F1AP_PDU_t f1apasnmsg ;
 
   DU_LOG("\nF1AP : Received F1AP message buffer");
   SPrntMsg(mBuf, 0,0);
 
   /* Copy mBuf into char array to decode it */
   SFndLenMsg(mBuf, &recvBufLen);
   if(SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&recvBuf, (Size)recvBufLen) != ROK)
   {
      DU_LOG("\nF1AP : Memory allocation failed");
      return;
   }
   if(SCpyMsgFix(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nF1AP : Failed while copying %d", copyCnt);
      return;
   }

   printf("\nF1AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
        printf("%x",recvBuf[i]);
   }

   /* Decoding flat buffer into F1AP messsage */
   f1apMsg = &f1apasnmsg;
   memset(f1apMsg, 0, sizeof(F1AP_PDU_t));
 
   rval = aper_decode(0, &asn_DEF_F1AP_PDU, (void **)&f1apMsg, recvBuf, recvBufLen, 0, 0);
   SPutSBuf(DFLT_REGION, DFLT_POOL, (Data *)recvBuf, (Size)recvBufLen);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nF1AP : ASN decode failed");
      return;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   switch(f1apMsg->present)
   {
      case F1AP_PDU_PR_successfulOutcome:
      {
         switch(f1apMsg->choice.successfulOutcome->value.present)
         {
            case SuccessfulOutcome__value_PR_F1SetupResponse:
            {				
#ifndef ODU_TEST_STUB
               procF1SetupRsp(f1apMsg);
#endif
               break;
            }
            
            case SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge:
            {
               procGNBDUCfgUpdAck(f1apMsg);
               break;
            }

            default:
            {
               DU_LOG("\nF1AP : Invalid type of successful outcome [%d]", f1apMsg->choice.successfulOutcome->value.present);
               return;
            }
         }/* End of switch(successfulOutcome) */
         break;
      }
      
      default:
      {
         DU_LOG("\nF1AP : Invalid type of f1apMsg->present [%d]",f1apMsg->present);
         return;
      }

   }/* End of switch(f1apMsg->present) */
 
} /* End of F1APMsgHdlr */
 
/*******************************************************************
 *
 * @brief Processes GNB DU config update ack
 *
 * @details
 *
 *    Function : procGNBDUCfgUpdAck
 *
 *    Functionality: Processes GNB DU config update ack
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 procGNBDUCfgUpdAck(F1AP_PDU_t *f1apMsg)
{
  

   DU_LOG("\nF1AP : GNB-DU config update acknowledgment received");

    
   /* TODO :Check the deallocation */
#if 0
   SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(gNBDuCfgAck->protocolIEs.list.array),\
           (Size)elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
           (Size)sizeof(SuccessfulOutcome_t));
   SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
#endif
    return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
