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
static int check=0;
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
   U8 idx=0;
   ulnrfreq->nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.\
                       fdd.ulNrFreqInfo.nrArfcn;
   ulnrfreq->freqBandListNr.list.count = 1;
   ulnrfreq->freqBandListNr.list.size = sizeof(FreqBandNrItem_t *);
	DU_ALLOC(ulnrfreq->freqBandListNr.list.array,ulnrfreq->freqBandListNr.list.size);
   if(ulnrfreq->freqBandListNr.list.array == NULLP)
   {
       return RFAILED;
   }
   for(idx=0;idx<ulnrfreq->freqBandListNr.list.count;idx++)
   {
       DU_ALLOC(ulnrfreq->freqBandListNr.list.array[idx],sizeof(FreqBandNrItem_t));
       if(ulnrfreq->freqBandListNr.list.array[idx] == NULLP)
      {
         return RFAILED;
	   }
   }
   ulnrfreq->freqBandListNr.list.array[0]->freqBandIndicatorNr = \
   duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.\
   freqBand[0].nrFreqBand;
   ulnrfreq->freqBandListNr.list.array[0]->supportedSULBandList.list.count = SUL_BAND_COUNT;
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
   dlnrfreq->freqBandListNr.list.size = sizeof(FreqBandNrItem_t *);
   DU_ALLOC(dlnrfreq->freqBandListNr.list.array,dlnrfreq->freqBandListNr.list.size);
   if(dlnrfreq->freqBandListNr.list.array == NULLP)
   {
      return RFAILED;   
	}
   for(idx=0;idx< dlnrfreq->freqBandListNr.list.count;idx++)
	{
	   DU_ALLOC(dlnrfreq->freqBandListNr.list.array[idx],sizeof(FreqBandNrItem_t));
      if(dlnrfreq->freqBandListNr.list.array[idx] == NULLP)
      {
         return RFAILED;
		}
   }   
   dlnrfreq->freqBandListNr.list.array[0]->freqBandIndicatorNr = \
   duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.\
      freqBand[0].nrFreqBand;
   dlnrfreq->freqBandListNr.list.array[0]->supportedSULBandList.list.count = SUL_BAND_COUNT;
      
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
   if(nrcgi->pLMN_Identity.buf == NULLP)
	{
	   return RFAILED;
   }
	ret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
				 &nrcgi->pLMN_Identity); // Building PLMN function
	if(ret != ROK)
   {
	   return RFAILED;
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
		return RFAILED;
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
	   return RFAILED;
   }
   servcell->fiveGS_TAC->size = 3 * sizeof(U8);
   DU_ALLOC(servcell->fiveGS_TAC->buf,\
         sizeof(servcell->fiveGS_TAC->size));
   if(servcell->fiveGS_TAC->buf == NULLP)
   {
	   return RFAILED;
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
   S16 BuildDLNRInforet=0;
	S16 BuildULNRInforet=0; 
   /* FDD Mode */
	mode->present = NR_Mode_Info_PR_fDD;
   if(mode->present == NR_Mode_Info_PR_fDD)
   {
      DU_ALLOC(mode->choice.fDD,sizeof(FDD_Info_t));
      if(mode->choice.fDD == NULLP)
      {
		   return RFAILED;
      }
      BuildULNRInforet = BuildULNRInfo(&mode->choice.fDD->uL_NRFreqInfo);
      if(BuildULNRInforet != ROK)
      {
          return RFAILED;    
	   }
	   BuildDLNRInforet = BuildDLNRInfo(&mode->choice.fDD->dL_NRFreqInfo);
      if(BuildDLNRInforet != ROK)
      {
		   return RFAILED;
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
S16 BuildExtensions(ProtocolExtensionContainer_4624P3_t **ieExtend)
{
   U8 idx;
   U8 plmnidx;
   U8 extensionCnt=1;
   U8 sliceId=0;
   U8 sdId;
   DU_ALLOC(*ieExtend,sizeof(ProtocolExtensionContainer_4624P3_t));
   if((*ieExtend) == NULLP)
   {
	   return RFAILED;
   }
   (*ieExtend)->list.count = extensionCnt;
   (*ieExtend)->list.size = \
                            extensionCnt * sizeof(ServedPLMNs_ItemExtIEs_t *);
   DU_ALLOC((*ieExtend)->list.array,(*ieExtend)->list.size);
   if((*ieExtend)->list.array == NULLP)
   {
	   return RFAILED;
   }
   for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
   {
      DU_ALLOC((*ieExtend)->list.array[plmnidx],\
            sizeof(ServedPLMNs_ItemExtIEs_t));
      if((*ieExtend)->list.array[plmnidx] == NULLP)
      {
		   return RFAILED;
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
	    return RFAILED;
    }
    DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId],sizeof(SliceSupportItem_t));
    if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId] == NULLP) 
    {
	     return RFAILED;
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
	   return RFAILED;
   }
  (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sST.buf[0] = 3;
   DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD,sizeof(OCTET_STRING_t));
   if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD == NULLP)
   {
	   return RFAILED;
   }
   (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceId]->sNSSAI.sD->size = 3*sizeof(U8);
   DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD->buf,(*ieExtend)->list.array[idx]->extensionValue.choice.\
            SliceSupportList.list.array[sliceId]->sNSSAI.sD->size);
   if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceId]->sNSSAI.sD->buf == NULLP)
   {
       return RFAILED;
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
   U8  plmnidx;
   U8  servPlmnCnt=1;
	S16 buildPlmnIdret=0;
	S16 BuildExtensionsret=0;
   srvplmn->list.count = servPlmnCnt;
   srvplmn->list.size = \
                      servPlmnCnt*sizeof(ServedPLMNs_Item_t *);
	DU_ALLOC(srvplmn->list.array,srvplmn->list.size);
   if(srvplmn->list.array == NULLP)
   {
       return RFAILED;
   }
   for(plmnidx=0; plmnidx<servPlmnCnt; plmnidx++)
   {   
      DU_ALLOC(srvplmn->list.array[plmnidx],\
            sizeof(ServedPLMNs_Item_t));
      if(srvplmn->list.array[plmnidx] == NULLP)
      {
		   return RFAILED;
      }  
   }
	srvplmn->list.array[0]->pLMN_Identity.size = PLMN_SIZE * sizeof(U8);
   DU_ALLOC(srvplmn->list.array[0]->pLMN_Identity.buf, srvplmn->list.array[0]->pLMN_Identity.size);
   buildPlmnIdret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
         &srvplmn->list.array[0]->pLMN_Identity);
   if(buildPlmnIdret!= ROK)
   {
	   return RFAILED;
   }
   BuildExtensionsret = BuildExtensions(&srvplmn->list.array[0]->iE_Extensions);
   if(BuildExtensionsret!= ROK)
   {
	   return RFAILED;
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
   S16 BuildNrcgiret=0;
   S16 BuildFiveGSTacret=0;
	S16 BuildServedPlmnret=0;
	S16 BuildNrModeret=0;
   U8  idx;
   U8  plmnidx;
   U8  plmnCnt=1;
   GNB_DU_Served_Cells_Item_t *srvCellItem;
   duServedCell->list.size = plmnCnt * sizeof(GNB_DU_Served_Cells_ItemIEs_t *);
   duServedCell->list.count = plmnCnt;
 
   DU_ALLOC(duServedCell->list.array, duServedCell->list.size);
   if(duServedCell->list.array == NULLP)
   {
	   return RFAILED;
   }
   for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
   {
      DU_ALLOC(duServedCell->list.array[plmnidx],\
             sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      if(duServedCell->list.array[plmnidx] == NULLP)
      {
		   return RFAILED;
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
   BuildNrcgiret = BuildNrcgi(&srvCellItem->served_Cell_Information.nRCGI);
   if(BuildNrcgiret != ROK)
   {
	   return RFAILED;
   }
      /*nRPCI*/
   srvCellItem->served_Cell_Information.nRPCI = \
                         duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrPci;

   /*fiveGS_TAC*/
   BuildFiveGSTacret = BuildFiveGSTac(&srvCellItem->served_Cell_Information);
   if(BuildFiveGSTacret != ROK)
   {
	   return RFAILED;
   }
   /*Served PLMNs*/
   BuildServedPlmnret = BuildServedPlmn(&srvCellItem->served_Cell_Information.servedPLMNs);
   if(BuildServedPlmnret !=ROK)
   {
	    return RFAILED;
   }
   /*nR Mode Info with FDD*/
   BuildNrModeret = BuildNrMode(&srvCellItem->served_Cell_Information.nR_Mode_Info);
   if(BuildNrModeret != ROK)
   {
	   return RFAILED;
   }
   /*Measurement timing Config*/
   srvCellItem->served_Cell_Information.measurementTimingConfiguration.\
      size = sizeof(U8);
   DU_ALLOC(srvCellItem->served_Cell_Information.\
         measurementTimingConfiguration.buf,srvCellItem->served_Cell_Information.measurementTimingConfiguration.size);
   if(srvCellItem->served_Cell_Information.\
         measurementTimingConfiguration.buf == NULLP)
   {
	    return RFAILED;
   }
   srvCellItem->served_Cell_Information.measurementTimingConfiguration.buf[0] = \
             duCfgParam.srvdCellLst[0].duCellInfo.measTimeCfg;

   /* GNB DU System Information */
   DU_ALLOC(srvCellItem->gNB_DU_System_Information,
                        sizeof(GNB_DU_System_Information_t));
   if(!srvCellItem->gNB_DU_System_Information)
   {
	   return RFAILED;
   }
   /* MIB */
   srvCellItem->gNB_DU_System_Information->mIB_message.size =\
              strlen(( char *)duCfgParam.srvdCellLst[0].duSysInfo.mibMsg);
   DU_ALLOC(srvCellItem->gNB_DU_System_Information->mIB_message.buf,
              srvCellItem->gNB_DU_System_Information->mIB_message.size);
   if(!srvCellItem->gNB_DU_System_Information->mIB_message.buf)
   {
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
	    return RFAILED;
   }
   for(int x=0; x<srvCellItem->gNB_DU_System_Information->sIB1_message.size; x++)
   {
         srvCellItem->gNB_DU_System_Information->sIB1_message.buf[x]=\
           duCfgParam.srvdCellLst[0].duSysInfo.sib1Msg[x];
   }
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
	   return RFAILED;
   }
   rrcVer->latest_RRC_Version.buf[0] = 0;
   rrcVer->latest_RRC_Version.bits_unused = 5;
   DU_ALLOC(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
   if(rrcVer->iE_Extensions == NULLP)
   {  
	   return RFAILED;
   }
   rrcVer->iE_Extensions->list.count = 1;
   rrcVer->iE_Extensions->list.size = sizeof(RRC_Version_ExtIEs_t *);
   DU_ALLOC(rrcVer->iE_Extensions->list.array,rrcVer->iE_Extensions->list.size);
   if(rrcVer->iE_Extensions->list.array == NULLP)
   {
	   return RFAILED;
   }
   rrcExt = 0;
   DU_ALLOC(rrcVer->iE_Extensions->list.array[0],\
         sizeof(RRC_Version_ExtIEs_t));
   if(rrcVer->iE_Extensions->list.array[0] == NULLP)
   {
	   return RFAILED;
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
	   return RFAILED;
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
            return RFAILED;
         }
      }
      else
      {
         DU_LOG("\nF1AP : SAddPstMsgMult failed");
         SPutMsg(mBuf);
         return RFAILED;
      }
      SPutMsg(mBuf);
   }
   else
   {
      DU_LOG("\nF1AP : Failed to allocate memory");
      return RFAILED;
   }
   return ROK; 
} /* SendF1APMsg */

/*******************************************************************
*
* @brief  deallocating the memory of function BuildAndSendF1SetupReq()
*
* @details
*
*    Function :  FreeRrcVer
*
*    Functionality: deallocating the memory of function BuildRrcVer
*
* @params[in] RRC_Version_t *rrcVer
* 
* @return void
*
*****************************************************************/
void FreeRrcVer(RRC_Version_t *rrcVer)
{
   if(rrcVer->latest_RRC_Version.buf != NULLP)
	{
	   if(rrcVer->iE_Extensions != NULLP)
		{
		   if(rrcVer->iE_Extensions->list.array != NULLP)
			{
			   if(rrcVer->iE_Extensions->list.array[0] != NULLP)
				{
				   if(rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf
					!= NULLP)
					{
					   DU_FREE(rrcVer->iE_Extensions->list.array[0]->extensionValue.choice\
						.Latest_RRC_Version_Enhanced.buf,rrcVer->iE_Extensions->list.\
						array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.size);
					}
					DU_FREE(rrcVer->iE_Extensions->list.array[0],sizeof(RRC_Version_ExtIEs_t));
				}
				DU_FREE(rrcVer->iE_Extensions->list.array,sizeof(RRC_Version_ExtIEs_t*));
			}
			DU_FREE(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
		}
		DU_FREE(rrcVer->latest_RRC_Version.buf,rrcVer->latest_RRC_Version.size);
	}
}
/*******************************************************************
*
* @brief  deallocating the memory of function BuildAndSendF1SetupReq()
*
* @details
*
*    Function :  FreeServedCellList
*
*    Functionality:  deallocating the memory of function BuildServedCellList

*
* @params[in] GNB_DU_Served_Cells_List_t *duServedCell
*
* @return void
*
* ****************************************************************/
void FreeServedCellList( GNB_DU_Served_Cells_List_t *duServedCell)
{
   U8   plmnCnt=1;
   U8  servId=0;
	U8 sliceId=0;
	U8  ieId=0;
	U8   extensionCnt=1;
	U8 plmnidx=0;
   GNB_DU_Served_Cells_Item_t *srvCellItem= &duServedCell->list.array[0]->value.choice.GNB_DU_Served_Cells_Item;
   if(duServedCell->list.array!=NULLP)
   {
     if(duServedCell->list.array[0]!=NULLP)
	  {
	     if(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf!=NULLP)
	     {
		     if(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf !=NULLP)
			  {
			     if(srvCellItem->served_Cell_Information.fiveGS_TAC!=NULLP)
				  {
				     if(srvCellItem->served_Cell_Information.fiveGS_TAC->buf!=NULLP)
					  {
					     if(srvCellItem->served_Cell_Information.servedPLMNs.list.array!=NULLP)
						  {
						     if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[0]!=NULLP)
							  {
							     if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->pLMN_Identity.buf!=NULLP)
								  {
								     if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions!=NULLP)
									  {
									     if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array!=NULLP)
										  {
										     if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array[0]!=NULLP)
											  {
											    if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array[0]->\
												 extensionValue.choice.SliceSupportList.list.array!=NULLP)
												 {
												    if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array[0]->\
													 extensionValue.choice.SliceSupportList.list.array[sliceId]!= NULLP)
													 {
											           if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array[0]->\
												        extensionValue.choice.SliceSupportList.list.array[sliceId]->sNSSAI.sST.buf!=NULLP)
												        {
												           if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array[0]->\
													        extensionValue.choice.SliceSupportList.list.array[sliceId]->sNSSAI.sD!=NULLP)
													        {
													           if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->\
														        list.array[0]->extensionValue.choice.SliceSupportList.list.array[sliceId]->sNSSAI.sD->buf!=NULLP)
													           {
                                                     if(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD!=NULLP)
															        {
															           if(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->uL_NRFreqInfo.\
																        freqBandListNr.list.array!=NULLP)
																        {
																            if(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->uL_NRFreqInfo.\
																	         freqBandListNr.list.array[0]!=NULLP)
																	         {
																	            if(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
																		         freqBandListNr.list.array)
																		         {
																		            if(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
																		            freqBandListNr.list.array[0]!=NULLP)
																			         {
																			            if(srvCellItem->served_Cell_Information.measurementTimingConfiguration.buf!=NULLP)
																				         {
																				            if(!srvCellItem->gNB_DU_System_Information)
																					         {
																					            if(srvCellItem->gNB_DU_System_Information->mIB_message.buf!=NULLP)
																						         {
																							          if(srvCellItem->gNB_DU_System_Information->sIB1_message.buf!=NULLP)
																							          { 
																							             DU_FREE(srvCellItem->gNB_DU_System_Information->sIB1_message.buf,\
																							              srvCellItem->gNB_DU_System_Information->sIB1_message.size);
																											 DU_FREE(duCfgParam.srvdCellLst[0].duSysInfo.sib1Msg,\
																											  srvCellItem->gNB_DU_System_Information->sIB1_message.size);
																							          }
																						             DU_FREE(srvCellItem->gNB_DU_System_Information->mIB_message.buf,\
																						             srvCellItem->gNB_DU_System_Information->mIB_message.size);
																										 DU_FREE(duCfgParam.srvdCellLst[0].duSysInfo.mibMsg,\
																										 strlen((char*)duCfgParam.srvdCellLst[0].duSysInfo.mibMsg));
																					             }
																					             DU_FREE(srvCellItem->gNB_DU_System_Information,sizeof(GNB_DU_System_Information_t));
																					          }
																				             DU_FREE(srvCellItem->served_Cell_Information.measurementTimingConfiguration.buf,\
																					          srvCellItem->served_Cell_Information.measurementTimingConfiguration.size);
																				         }
																			            DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
																				         freqBandListNr.list.array[0],sizeof(FreqBandNrItem_t));
																			         }
																		            DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->dL_NRFreqInfo.\
																			         freqBandListNr.list.array,sizeof(FreqBandNrItem_t *));
																		        }
																	           DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->uL_NRFreqInfo.freqBandListNr.\
																				  list.array[0],sizeof(FreqBandNrItem_t));
																	        }
																           DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD->uL_NRFreqInfo.freqBandListNr.\
																			  list.array,sizeof(FreqBandNrItem_t*));
																        }
															           DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD,sizeof(FDD_Info_t));
															        }
														           DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
															        iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.list.array[sliceId]->\
																	  sNSSAI.sD->buf,srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->
																	 list.array[ieId]->extensionValue.choice.SliceSupportList.list.array[sliceId]->sNSSAI.sD->size);
																 }
													          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
														       iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.\
														        list.array[sliceId]->sNSSAI.sD,sizeof(OCTET_STRING_t));
													       }
												          DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->\
													       iE_Extensions->list.array[ieId]->extensionValue.choice.SliceSupportList.list.array[sliceId]->\
															 sNSSAI.sST.buf,sizeof(U8));
													    }
														 DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array[0]->\
                                           extensionValue.choice.SliceSupportList.list.array[sliceId],sizeof(SliceSupportItem_t));
												    }
                                        DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[servId]->iE_Extensions->list.array[0]->\
													  extensionValue.choice.SliceSupportList.list.array,sizeof(SliceSupportItem_t*));
                                     }
											    DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
												 array[servId]->iE_Extensions->list.array[plmnidx],sizeof(ServedPLMNs_ItemExtIEs_t));
											  }
										     DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
											  array[servId]->iE_Extensions->list.array,\
											  extensionCnt*sizeof(ServedPLMNs_ItemExtIEs_t*));
										  }
									     DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
										  array[servId]->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P3_t));
									  }
								     DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.\
									  array[servId]->pLMN_Identity.buf,srvCellItem->served_Cell_Information.\
									  servedPLMNs.list.array[servId]->pLMN_Identity.size
									  * sizeof(U8));
								  }
							     DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[plmnidx],\
								  sizeof(ServedPLMNs_Item_t *));
							  }
						     DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array,\
							  sizeof(ServedPLMNs_Item_t *));
						  }
					     DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC->buf,\
						  sizeof(srvCellItem->served_Cell_Information.fiveGS_TAC->size));
					  }
				     DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC,sizeof(FiveGS_TAC_t));
				  }
			     DU_FREE(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
				  srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.size *
				  sizeof(U8));
			  }
		     DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
			  srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size *
			  sizeof(U8));
		  }
	     DU_FREE(duServedCell->list.array[plmnidx],sizeof(GNB_DU_Served_Cells_ItemIEs_t));
	  }
     DU_FREE(duServedCell->list.array,plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t*));
  }
}
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
   S16  BuildServedCellListreturn=0;
	S16  BuildRrcVerreturn=0; 
   U8   idx=0,idx2=0,i=0;
   U8   ieId=0;
   U8   elementCnt=0;
   F1AP_PDU_t                 *f1apMsg = NULLP;
   F1SetupRequest_t           *f1SetupReq=NULLP;
   GNB_DU_Served_Cells_List_t *duServedCell=NULLP;
   RRC_Version_t              *rrcVer=NULLP;
   asn_enc_rval_t             encRetVal;        /* Encoder return value */
	bool ret=false;
   DU_LOG("\nF1AP : Building F1 Setup Request\n");
   do
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
      f1SetupReq->protocolIEs.list.size = elementCnt * sizeof(F1SetupRequestIEs_t );

      /* Initialize the F1Setup members */
      DU_ALLOC(f1SetupReq->protocolIEs.list.array,f1SetupReq->protocolIEs.list.size);
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
           f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_ID.size);
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
         f1SetupReq->protocolIEs.list.array[idx2]->value.present = F1SetupRequestIEs__value_PR_GNB_DU_Name;
         f1SetupReq->protocolIEs.list.array[idx2]->value.choice.GNB_DU_Name.size =\
                                                        strlen((char *)duCfgParam.duName);
         DU_ALLOC(f1SetupReq->protocolIEs.list.array[idx2]->value.choice.\
            GNB_DU_Name.buf, strlen((char *)duCfgParam.duName));
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
      BuildServedCellListreturn = BuildServedCellList(duServedCell);
      if(BuildServedCellListreturn != ROK)
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
      BuildRrcVerreturn = BuildRrcVer(rrcVer);
      ieId = 0;
      if(BuildRrcVerreturn != ROK)
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
           for(i=0; i< encBufSize; i++)
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
		ret=true;
      break;
   }while(1);

   if(f1apMsg != NULLP)
   {

      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
         if(f1SetupReq->protocolIEs.list.array != NULLP)
         {
            if(idx == elementCnt-1)
            {
               if(f1SetupReq->protocolIEs.list.array[1]->value.choice.GNB_DU_ID.buf !=  NULLP)
               {
                   if(f1SetupReq->protocolIEs.list.array[2]->value.choice.GNB_DU_Name.buf != NULLP)
                   {
						     if(BuildServedCellListreturn == ROK)
							  {   
							      FreeRrcVer(rrcVer);
							  }
							  FreeServedCellList(duServedCell);
							  DU_FREE(f1SetupReq->protocolIEs.list.array[2]->value.choice.GNB_DU_Name.buf, strlen((char *)duCfgParam.duName));
                   }
                   DU_FREE(f1SetupReq->protocolIEs.list.array[1]->value.choice.GNB_DU_ID.buf, sizeof(U8));
                }
		          for(idx=0; idx<elementCnt; idx++)
                {
                    DU_FREE(f1SetupReq->protocolIEs.list.array[idx],sizeof(F1SetupRequestIEs_t));
                }
            }
            else
            {
                for(ieId=0 ; ieId<idx ;ieId++)
                {
					    if(f1SetupReq->protocolIEs.list.array[ieId]!=NULLP)
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
   }

	if(ret==true)
   return ROK;  
	else
	return RFAILED;
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
   U8 idx;
	U8 idy;
	U8 idx1;
	U8 idy1;
	U8 idy2;
   U8 elementCnt;
	U8 modifyCnt;
	U8 servPlmnCnt;
	U8 extensionCnt;
   asn_enc_rval_t encRetVal;      /* Encoder return value */
   F1AP_PDU_t *f1apDuCfg = NULL;
   GNBDUConfigurationUpdate_t *duCfgUpdate;
   Served_Cells_To_Modify_List_t  *cellsToModify;
   Served_Cells_To_Modify_Item_t *modifyItem;
   bool checkvar=false;
   U8 i;

	while(1)
	{
	    DU_LOG("\nF1AP : Building DU config update\n");
		 /* Allocate the memory for F1DuCfg */
	    DU_ALLOC(f1apDuCfg, sizeof(F1AP_PDU_t));
		 if(f1apDuCfg == NULLP)
		 {
		   DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
		   break;
		 }
		
		 f1apDuCfg->present = F1AP_PDU_PR_initiatingMessage;
       DU_ALLOC(f1apDuCfg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
       if(f1apDuCfg->choice.initiatingMessage == NULLP)
		 {
		    DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
			 break;
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
		                 elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t*);
     
	   /* Initialize the F1Setup members */
		 DU_ALLOC(duCfgUpdate->protocolIEs.list.array,duCfgUpdate->protocolIEs.list.size);
		 if(duCfgUpdate->protocolIEs.list.array == NULLP)
		 {
		   DU_LOG("F1AP : Memory allocation for F1RequestIEs failed");
			break;
		 }
		 for(idx=0;idx<elementCnt;idx++)
		 {
		    DU_ALLOC(duCfgUpdate->protocolIEs.list.array[idx],sizeof(GNBDUConfigurationUpdateIEs_t));
			  if(duCfgUpdate->protocolIEs.list.array[idx] == NULLP)
			  {
				  break;
			  }
		 }
		
		 /*TransactionID*/
		 idx1 = 0;
		 duCfgUpdate->protocolIEs.list.array[idx1]->id=ProtocolIE_ID_id_TransactionID;
		 duCfgUpdate->protocolIEs.list.array[idx1]->criticality= Criticality_reject;
		 duCfgUpdate->protocolIEs.list.array[idx1]->value.present = \
		                             GNBDUConfigurationUpdateIEs__value_PR_TransactionID;
		 duCfgUpdate->protocolIEs.list.array[idx1]->value.choice.TransactionID = \
		        TRANS_ID;

       /*Served Cell to Modify */
		 idx1++;
       duCfgUpdate->protocolIEs.list.array[idx1]->id = \
		                            ProtocolIE_ID_id_Served_Cells_To_Modify_List;
       duCfgUpdate->protocolIEs.list.array[idx1]->criticality =Criticality_reject;
       duCfgUpdate->protocolIEs.list.array[idx1]->value.present = \
		                                  GNBDUConfigurationUpdateIEs__value_PR_Served_Cells_To_Modify_List;
	    cellsToModify = &duCfgUpdate->protocolIEs.list.array[idx1]->value.choice.\
	                     Served_Cells_To_Modify_List;
       modifyCnt = 1;
		 cellsToModify->list.count = modifyCnt;
		 cellsToModify->list.size = \
		                 modifyCnt*sizeof(Served_Cells_To_Modify_ItemIEs_t *);
		 DU_ALLOC(cellsToModify->list.array,cellsToModify->list.size);
		 if(cellsToModify->list.array == NULLP)
		 {
	       break;
		 }
		 for(idy=0; idy<modifyCnt ;idy++)
		 {
		    DU_ALLOC(cellsToModify->list.array[idy],sizeof(\
			 Served_Cells_To_Modify_ItemIEs_t));
			 if(cellsToModify->list.array[idy] == NULLP)
			 {
				 break;
			 }
		 }
	    cellsToModify->list.array[0]->id = \
		                   ProtocolIE_ID_id_Served_Cells_To_Modify_Item;
	 	 cellsToModify->list.array[0]->criticality = Criticality_reject;
	    cellsToModify->list.array[0]->value.present =\
		       Served_Cells_To_Modify_ItemIEs__value_PR_Served_Cells_To_Modify_Item;
	    modifyItem=&cellsToModify->list.array[0]->value.choice.\
		   Served_Cells_To_Modify_Item;

       /*pLMN_Identity*/
		 modifyItem->oldNRCGI.pLMN_Identity.size = 3*sizeof(uint8_t);
		 DU_ALLOC(modifyItem->oldNRCGI.pLMN_Identity.buf,modifyItem->oldNRCGI.pLMN_Identity.size);
		 if(modifyItem->oldNRCGI.pLMN_Identity.buf == NULLP)
		 { 
		    break;
		 }
       buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
		              &modifyItem->oldNRCGI.pLMN_Identity);
	 
	    /*nRCellIdentity*/
		 modifyItem->oldNRCGI.nRCellIdentity.size = 5*sizeof(uint8_t);
		 DU_ALLOC(modifyItem->oldNRCGI.nRCellIdentity.buf,\
		     modifyItem->oldNRCGI.nRCellIdentity.size);
		 if(modifyItem->oldNRCGI.nRCellIdentity.buf == NULLP)
		 {
          break;
		 }
       for (int tmp = 0;tmp < modifyItem->oldNRCGI.nRCellIdentity.size-1;tmp++)
		 {
           modifyItem->oldNRCGI.nRCellIdentity.buf[tmp] = 0;
		 }
		 modifyItem->oldNRCGI.nRCellIdentity.buf[4] = 16;
		 modifyItem->oldNRCGI.nRCellIdentity.bits_unused = 4;
		 /*pLMN_Identity*/
		 /*nRCGI*/
		 modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.size =3*sizeof(uint8_t);
       DU_ALLOC(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
		     modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.size);
		 if(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf == NULLP)
       {
			 break;
		 }
       buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
		     &modifyItem->served_Cell_Information.nRCGI.pLMN_Identity);
		 modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.size =5*sizeof(uint8_t);
		 DU_ALLOC(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
		    modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.size);
		 if(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf == NULLP)
		 {
			break;
		 } 
       for (int tmp = 0 ; tmp < modifyItem->served_Cell_Information.\
		     nRCGI.nRCellIdentity.size-1 ; tmp++)
	    {
		    modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf[tmp] = 0;
		 }
	    modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf[4] = 16;
       modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.bits_unused =4;
       /*nRPCI*/
	    modifyItem->served_Cell_Information.nRPCI = \
		               duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrPci;
       /*servedPLMNs*/
	    servPlmnCnt = 1;
	    modifyItem->served_Cell_Information.servedPLMNs.list.count = servPlmnCnt;
       modifyItem->served_Cell_Information.servedPLMNs.list.size = \
                     servPlmnCnt*sizeof(ServedPLMNs_Item_t *);
		 DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
		       modifyItem->served_Cell_Information.servedPLMNs.list.size);
	    if(modifyItem->served_Cell_Information.servedPLMNs.list.array == NULLP)
		 {
			break;
		 }
       for(idy1=0;idy1<servPlmnCnt;idy1++)
		 {
		     DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy1],\
			   sizeof(ServedPLMNs_Item_t));
				if(modifyItem->served_Cell_Information.servedPLMNs.list.array[idy1]== NULLP)
				{
					 break;
				}
		 }
		 modifyItem->served_Cell_Information.servedPLMNs.list.\
		     array[0]->pLMN_Identity.size = 3*sizeof(uint8_t);
		 DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
		           array[0]->pLMN_Identity.buf,modifyItem->served_Cell_Information.servedPLMNs.list.\
					  array[0]->pLMN_Identity.size);
		 if(modifyItem->served_Cell_Information.servedPLMNs.list.\
		         array[0]->pLMN_Identity.buf == NULLP)
		 {
			 break;
		 }
       buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.plmn[0],\
		     &modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->pLMN_Identity);
       DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
		       array[0]->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P3_t));
		 if(modifyItem->served_Cell_Information.servedPLMNs.list.\
		       array[0]->iE_Extensions == NULLP)
		 {
			 break;
		 }
       extensionCnt=1;
		 modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		       iE_Extensions->list.count = extensionCnt;
		 modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		     iE_Extensions->list.size = extensionCnt *sizeof(ServedPLMNs_ItemExtIEs_t *);
		 DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
		      array[0]->iE_Extensions->list.array,modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
				 iE_Extensions->list.size);
		 if(modifyItem->served_Cell_Information.servedPLMNs.list.\
		        array[0]->iE_Extensions->list.array == NULLP)
		 {
		   break;
		 }
       for(idy2=0;idy2<extensionCnt;idy2++)
		 {
		    DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.\
			   array[0]->iE_Extensions->list.array[idy2],\
				sizeof(ServedPLMNs_ItemExtIEs_t));
			 if(modifyItem->served_Cell_Information.servedPLMNs.list.\
			           array[0]->iE_Extensions->list.array[idy2] == NULLP)
			 {
				  break;
			 }
		 }
		 modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		    iE_Extensions->list.array[0]->id =ProtocolIE_ID_id_TAISliceSupportList;
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
          list.array,modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
			 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.list.size);
	    if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		    iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			  list.array == NULLP)
		 {
			break;
		 }

		 DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			list.array[0],sizeof( SliceSupportItem_t));
		 if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		    iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			  list.array[0] == NULLP)
		 {
		   break;
		 }
		 modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			list.array[0]->sNSSAI.sST.size = sizeof(uint8_t);
		 DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			list.array[0]->sNSSAI.sST.buf,modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
			iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.list.array[0]->sNSSAI.sST.size);
	    if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			list.array[0]->sNSSAI.sST.buf == NULLP)
		 {
			 break;
		 }
       modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		    iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			 list.array[0]->sNSSAI.sST.buf[0] = 3;
	    DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		  iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
		   list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
		 if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 		list.array[0]->sNSSAI.sD == NULLP)
	    {
			 break;
		 }
		 modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			list.array[0]->sNSSAI.sD->size = 3*sizeof(uint8_t);
       DU_ALLOC(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
		   list.array[0]->sNSSAI.sD->buf,modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
			iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			list.array[0]->sNSSAI.sD->size);
       if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
		   iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
			list.array[0]->sNSSAI.sD->buf == NULLP)
		 {
			 break;
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
			 break;
		 }
       modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
		     uL_NRFreqInfo.nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.\
			   f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn;
		 modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
		     uL_NRFreqInfo.freqBandListNr.list.count = 1;
		 modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
		     uL_NRFreqInfo.freqBandListNr.list.size = sizeof(FreqBandNrItem_t*);
		 DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
		     fDD->uL_NRFreqInfo.freqBandListNr.list.\
			  array, modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
			   uL_NRFreqInfo.freqBandListNr.list.size);
       if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
		            fDD->uL_NRFreqInfo.freqBandListNr.list.array == NULLP)
		 {
			 break;
		 }
		 DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
		       uL_NRFreqInfo.freqBandListNr.list.array[0],sizeof(FreqBandNrItem_t));
		 if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->
		      uL_NRFreqInfo.freqBandListNr.list.array[0] == NULLP)
		 {
          break;
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
          freqBandListNr.list.size = sizeof(FreqBandNrItem_t *);
       DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array,modifyItem->served_Cell_Information.nR_Mode_Info.\
			choice.fDD->dL_NRFreqInfo.freqBandListNr.list.size);
       if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array == NULLP)
       {
			break;
       }
	    DU_ALLOC(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array[0],\
         sizeof(FreqBandNrItem_t));
       if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD->\
         dL_NRFreqInfo.freqBandListNr.list.array[0] == NULLP)
       {
		     break;
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
         buf,modifyItem->served_Cell_Information.measurementTimingConfiguration.size);
       if(modifyItem->served_Cell_Information.measurementTimingConfiguration.buf == NULLP)
       {
		  	break;
       }
       modifyItem->served_Cell_Information.measurementTimingConfiguration.\
          buf[0] = duCfgParam.srvdCellLst[0].duCellInfo.measTimeCfg;
   
       // NOTE :GNB DU SYS INFO:MIB AND SIB1 INFORMATION TO BE BUILT AND FILLED HERE
       /*GNB DU ID */
       idx1++;
       duCfgUpdate->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_DU_ID;
       duCfgUpdate->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
       duCfgUpdate->protocolIEs.list.array[idx1]->value.present = \
                             GNBDUConfigurationUpdateIEs__value_PR_GNB_DU_ID;
       duCfgUpdate->protocolIEs.list.array[idx1]->value.choice.GNB_DU_ID.size = \
                                                             sizeof(uint8_t);
       DU_ALLOC(duCfgUpdate->protocolIEs.list.array[idx1]->value.choice.GNB_DU_ID.buf,\
          duCfgUpdate->protocolIEs.list.array[idx1]->value.choice.GNB_DU_ID.size);
       if(duCfgUpdate->protocolIEs.list.array[idx1]->value.choice.GNB_DU_ID.buf == NULLP)
       {
			 break;
       }
       duCfgUpdate->protocolIEs.list.array[idx1]->value.choice.GNB_DU_ID.buf[0] = \
                                                              duCfgParam.duId;

       xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apDuCfg);
   
       /* Encode the DU Config Update type as APER */
       cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
       encBufSize = 0;
       encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apDuCfg, PrepFinalEncBuf, encBuf);

       /* Checking encode results */
       if(encRetVal.encoded == ENCODE_FAIL) 
       {
            DU_LOG("F1AP : Could not encode DUConfigUpdate structure (at %s)\n",encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
				break;
		 } 
       else 
       {
           DU_LOG("\nF1AP : Created APER encoded buffer for DUConfigUpdate\n");
           for(i=0; i< encBufSize; i++)
          {
              printf("%x",encBuf[i]);
          }
       } 
       /* Sending msg */
       if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
       {
          DU_LOG("\nF1AP : Sending GND-DU Config Update failed");
			 break;
       }
		 checkvar=true;
		 break;
	}
   if(f1apDuCfg != NULLP)
	{
	    if(f1apDuCfg->choice.initiatingMessage != NULLP)
		 {
		     if(duCfgUpdate->protocolIEs.list.array != NULLP)
			  {
			      if(idx==elementCnt)
					{
					    if(cellsToModify->list.array != NULLP)
						 {
						     if(idy==modifyCnt)
							  {
							      if(modifyItem->oldNRCGI.pLMN_Identity.buf != NULLP)
                           {
									    if(modifyItem->oldNRCGI.nRCellIdentity.buf != NULLP)
										 {
										      if(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf != NULLP)
												{
												     if(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf
													  != NULLP)
													  { 
													      if(modifyItem->served_Cell_Information.servedPLMNs.list.array\
															!= NULLP)
															 {
															     if(idy1==servPlmnCnt)
																  {
																      if(modifyItem->served_Cell_Information.servedPLMNs.list.\
																	    array[0]->pLMN_Identity.buf != NULLP)
                                                      {
                                                           if(modifyItem->served_Cell_Information.servedPLMNs.list.\
																			    array[0]->iE_Extensions!= NULLP)
																				 {
																				     if(modifyItem->served_Cell_Information.servedPLMNs.list.\
                                                                 array[0]->iE_Extensions->list.array != NULLP)
																					  {
																					      if(idy2==extensionCnt)
																							{
																							   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																								iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																								list.array !=NULLP)
																								{
																								   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																									 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																									 list.array[0]!=NULLP)
																									 {   
																									     if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																										  iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																										  list.array[0]->sNSSAI.sST.buf!=NULLP)
																										  {
																										     if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																											  iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																											  list.array[0]->sNSSAI.sD != NULLP)
																											  {
																											       if(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																													 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																													  list.array[0]->sNSSAI.sD->buf!=NULLP)
																													 {
																													     if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD
																														  !=NULLP)
																														  {
																														     if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
																															  fDD->uL_NRFreqInfo.freqBandListNr.list.array!=NULLP)
																														     {
																															      if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
																																	fDD->uL_NRFreqInfo.freqBandListNr.list.array[0]!=NULLP)
																																	{
																																	    if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
																																		 fDD->dL_NRFreqInfo.freqBandListNr.list.array !=NULLP)
																																	    {
																																		    if(modifyItem->served_Cell_Information.nR_Mode_Info.\
																																			 choice.fDD->dL_NRFreqInfo.freqBandListNr.list.\
																																			 array[0]!= NULLP)
																																			 {
																																			    if(modifyItem->served_Cell_Information.\
																																				 measurementTimingConfiguration.buf !=NULLP)
																																				 {
																																				   if(duCfgUpdate->protocolIEs.list.array[2]->value.\
																																					choice.GNB_DU_ID.buf!=NULLP)
                                                                                                               {
																																					    DU_FREE(duCfgUpdate->protocolIEs.list.\
																																						 array[2]->value.choice.GNB_DU_ID.buf,\
																																					 	 duCfgUpdate->protocolIEs.list.array[2]->\
																																						 value.choice.GNB_DU_ID.size);
																																					}
																																				   DU_FREE(modifyItem->served_Cell_Information.\
																																					measurementTimingConfiguration.\
																																					buf,modifyItem->served_Cell_Information.\
																																					measurementTimingConfiguration.size);
																																				 }
																																			    DU_FREE(modifyItem->served_Cell_Information.\
																																				nR_Mode_Info.choice.fDD->dL_NRFreqInfo.freqBandListNr.\
																																			   list.array[0],sizeof(FreqBandNrItem_t));
																																			 }
																																			 DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info\
																																			 .choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array,\
																																			 modifyItem->served_Cell_Information.nR_Mode_Info.\
																																			 choice.fDD->dL_NRFreqInfo.freqBandListNr.list.size);
																																		 }
																																		 DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.\
																																		 choice.fDD->uL_NRFreqInfo.freqBandListNr.list.\
																																		 array[0],sizeof(FreqBandNrItem_t));
																																	}
																															     DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
																																  fDD->uL_NRFreqInfo.freqBandListNr.list.\
																																  array,modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
																																  fDD->uL_NRFreqInfo.freqBandListNr.list.size);
																															  }
																														     DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
																															  fDD,sizeof(FDD_Info_t));
																														  }
																													    DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																														 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																														  list.array[0]->sNSSAI.sD->buf,modifyItem->served_Cell_Information.\
																														 servedPLMNs.list.array[0]->iE_Extensions->list.array[0]->\
																														 extensionValue.choice.SliceSupportList.list.array[0]->sNSSAI.sD->size);

																													 }
																											      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																													iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																													list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
																											  }
																										     DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																											  iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																											  list.array[0]->sNSSAI.sST.buf,modifyItem->served_Cell_Information.servedPLMNs.\
																											  list.array[0]->iE_Extensions->list.array[0]->extensionValue.choice.\
																											  SliceSupportList.list.array[0]->sNSSAI.sST.size);
																										  }
																									     DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																										  iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																										  list.array[0],sizeof(SliceSupportItem_t));
																									 }
																								   DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																									iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
																								    list.array,\
																									 modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->\
																									 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.list.size);
																								}
																							   for(i=0;i<extensionCnt;i++)
																								{
																								    DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
																									 array[0]->iE_Extensions->list.array[i],\
																									 sizeof(ServedPLMNs_ItemExtIEs_t ));
																								}
																							}
																							else
																							{
																							    for(i=0;i<idy2;i++)
																								 {
																								    DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
																									  array[0]->iE_Extensions->list.array[i],\
																									  sizeof(ServedPLMNs_ItemExtIEs_t));
																								 }
																							}
																					      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
																						   array[0]->iE_Extensions->list.array,modifyItem->served_Cell_Information.servedPLMNs.
																							list.array[0]->iE_Extensions->list.size);
																					  }
																				     DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
																					    array[0]->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P3_t));
																				 }
																	       DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
																		    array[0]->pLMN_Identity.buf,
																			 modifyItem->served_Cell_Information.servedPLMNs.list.array[0]->pLMN_Identity.size);
																		}
                                                      for(i=0;i<idy1;i++)
																		{
																		   DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[i],\
																			sizeof(ServedPLMNs_Item_t));
																		}
																  }
																  else
																  {
																     for(i=0;i<idy1;i++)
																	  {
																	     DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[i],\
																		  sizeof(ServedPLMNs_Item_t));
																	  }
																  }
															     DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
																  modifyItem->served_Cell_Information.servedPLMNs.list.size);
															 }
													       DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
															  modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.size);		
													  }
												    DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
													  modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.size);
												}
										     DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf,\
											     modifyItem->oldNRCGI.nRCellIdentity.size);
                               }
									    DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf,\
										  modifyItem->oldNRCGI.pLMN_Identity.size);
									}
									 for(int iy=0; iy<modifyCnt ;iy++)
									 {
									    DU_FREE(cellsToModify->list.array[iy],\
										  sizeof(Served_Cells_To_Modify_ItemIEs_t));
									 }
							  }
							  else
							  {
							     for(int iy=0; iy<idy ;iy++)
								  {
								     DU_FREE(cellsToModify->list.array[iy],\
									   sizeof(Served_Cells_To_Modify_ItemIEs_t));
								  }	
							  }
						     DU_FREE(cellsToModify->list.array,cellsToModify->list.size);
						 }
					    for(i=0;i<elementCnt;i++)
						 {
					       DU_FREE(duCfgUpdate->protocolIEs.list.array[i],\
							   sizeof(GNBDUConfigurationUpdateIEs_t));
						 }
					}
					else
					{
					   for(i=0;i<idx;i++)
						{
						   DU_FREE(duCfgUpdate->protocolIEs.list.array[i],\
							 sizeof(GNBDUConfigurationUpdateIEs_t));
						}
					}
               DU_FREE(duCfgUpdate->protocolIEs.list.array,duCfgUpdate->protocolIEs.list.size);
			  }
			   DU_FREE(f1apDuCfg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
		 }
		  DU_FREE(f1apDuCfg, (Size)sizeof(F1AP_PDU_t));
	}
   if(checkvar==true)
      return ROK;
	else 
	   return RFAILED;
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
	U8   idx1;
	U8   idx;
	F1AP_PDU_t      			*f1apMsg = NULL;
   ULRRCMessageTransfer_t	*ulRRCMsg;
	asn_enc_rval_t  			encRetVal;        /* Encoder return value */
   bool checkvar=false;
	while(1)
	{
	   DU_LOG("\n F1AP : Building UL RRC Message Transfer Message\n");

	   DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
	   if(f1apMsg == NULLP)
	   {
		   DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
			break;
	   }

	   f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
   	DU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
	   if(f1apMsg->choice.initiatingMessage == NULLP)
	   {
		   DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
			break;
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
	   DU_ALLOC(ulRRCMsg->protocolIEs.list.array, ulRRCMsg->protocolIEs.list.size);
	   if(ulRRCMsg->protocolIEs.list.array == NULLP)
	   {
		    DU_LOG(" F1AP : Memory allocation for UL RRC MessageTransferIEs failed");
			 break;
    	}
	   for(idx=0; idx<elementCnt; idx++)
	   {
		   DU_ALLOC(ulRRCMsg->protocolIEs.list.array[idx],sizeof(ULRRCMessageTransferIEs_t));
	    	if(ulRRCMsg->protocolIEs.list.array[idx] == NULLP)
		   {
			   break;
		   }
	   }

	   idx1 = 0;

	    /*GNB CU UE F1AP ID*/
	   ulRRCMsg->protocolIEs.list.array[idx1]->id	= \
					 			                 ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
	   ulRRCMsg->protocolIEs.list.array[idx1]->criticality	= 	Criticality_reject;
	   ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
                    		ULRRCMessageTransferIEs__value_PR_GNB_CU_UE_F1AP_ID;
   	ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.GNB_CU_UE_F1AP_ID = CU_ID;

	   /*GNB DU UE F1AP ID*/
	   idx1++;
   	ulRRCMsg->protocolIEs.list.array[idx1]->id	= \
					 			                 ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
	   ulRRCMsg->protocolIEs.list.array[idx1]->criticality	= 	Criticality_reject;
	   ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
                    		ULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
	   ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.GNB_DU_UE_F1AP_ID = DU_ID;

	   /*SRBID*/
	   idx1++;
	   ulRRCMsg->protocolIEs.list.array[idx1]->id	= \
									 			                 ProtocolIE_ID_id_SRBID;
	   ulRRCMsg->protocolIEs.list.array[idx1]->criticality	= 	Criticality_reject;
	   ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
				                    		ULRRCMessageTransferIEs__value_PR_SRBID;
	   ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.SRBID = UL_SRBID;

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
			  break;
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
			 break;
		}
      checkvar=true;
		break;
	}

	if(f1apMsg != NULLP)
	{
	   if(f1apMsg->choice.initiatingMessage != NULLP) 
	   {
		   if(ulRRCMsg->protocolIEs.list.array != NULLP)
			{
				 for(idx1=0;idx1<idx;idx1++)
				 {
				      if(ulRRCMsg->protocolIEs.list.array[idx1] != NULLP)
					  {
						  DU_FREE(ulRRCMsg->protocolIEs.list.array[idx1],sizeof(ULRRCMessageTransferIEs_t));
					  }
				 }
			    DU_FREE(ulRRCMsg->protocolIEs.list.array,ulRRCMsg->protocolIEs.list.size ); 
			}
		   DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
		}
	   DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
	}

	if(checkvar==true)
      return ROK;
	else
	   return RFAILED;
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
	U8   idx1;
	F1AP_PDU_t  *f1apMsg = NULLP;
	InitialULRRCMessageTransfer_t *initULRRCMsg=NULLP;
   asn_enc_rval_t                encRetVal;
	bool  checkvar=false;
	while(1)
	{
	     DU_LOG("\n F1AP : Building RRC Setup Request\n");
		  DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
		  if(f1apMsg == NULLP)
		  {
		      DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
			   break;
		  }
		  f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
		  DU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
		  if(f1apMsg->choice.initiatingMessage == NULLP)
		  {
		     DU_LOG(" F1AP : Memory allocation for  F1AP-PDU failed");
			  break;
		  }
		  f1apMsg->choice.initiatingMessage->procedureCode =\
		         ProcedureCode_id_InitialULRRCMessageTransfer;
		  f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
		  f1apMsg->choice.initiatingMessage->value.present = \
		         InitiatingMessage__value_PR_InitialULRRCMessageTransfer;
		  initULRRCMsg =\
		         &f1apMsg->choice.initiatingMessage->value.choice.InitialULRRCMessageTransfer;
        elementCnt = 3;
		  initULRRCMsg->protocolIEs.list.count = elementCnt;
		  initULRRCMsg->protocolIEs.list.size = \
		         elementCnt * sizeof(InitialULRRCMessageTransferIEs_t *);
		  /* Initialize the F1Setup members */
		   DU_ALLOC(initULRRCMsg->protocolIEs.list.array,initULRRCMsg->protocolIEs.list.size);
		   if(initULRRCMsg->protocolIEs.list.array == NULLP)
		   {
		      DU_LOG(" F1AP : Memory allocation for\
				RRCSetupRequestMessageTransferIEs failed");
				break;
		   }
		   for(idx=0; idx<elementCnt; idx++)
	      {
			   DU_ALLOC(initULRRCMsg->protocolIEs.list.array[idx],\
				 sizeof(InitialULRRCMessageTransferIEs_t));
				 if(initULRRCMsg->protocolIEs.list.array[idx] == NULLP)
				 {
				     break;
				 }
		   }
		   idx1 = 0;
	 	   /*GNB DU UE F1AP ID*/
		   initULRRCMsg->protocolIEs.list.array[idx1]->id  = \
			              ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
			initULRRCMsg->protocolIEs.list.array[idx1]->criticality  = Criticality_reject;
		   initULRRCMsg->protocolIEs.list.array[idx1]->value.present = \
			              InitialULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
		   initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.GNB_DU_UE_F1AP_ID= DU_ID;
			/*NRCGI*/
			idx1++;
			initULRRCMsg->protocolIEs.list.array[idx1]->id  = \
		                 ProtocolIE_ID_id_NRCGI;
			initULRRCMsg->protocolIEs.list.array[idx1]->criticality  =Criticality_reject;
			initULRRCMsg->protocolIEs.list.array[idx1]->value.present = \
                       InitialULRRCMessageTransferIEs__value_PR_NRCGI;
		   
			ret =\
			BuildNrcgi(&initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.NRCGI);
	      if(ret!=ROK)
			{
			    break;
			}
			initULRRCMsg->protocolIEs.list.array[idx1]->id  = \
			               ProtocolIE_ID_id_C_RNTI;
			initULRRCMsg->protocolIEs.list.array[idx1]->criticality  =Criticality_reject;
			initULRRCMsg->protocolIEs.list.array[idx1]->value.present =\
		       	         InitialULRRCMessageTransferIEs__value_PR_C_RNTI;
			initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.C_RNTI =CRNTI;
         /*RRCContainer*/
			// Need to fill this.

			/*DUtoCURRCContainer*/
			//Need to fill this too.
			xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);
			/* Encode the F1SetupRequest type as APER */
			cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
			encBufSize = 0;
			encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg,\
			PrepFinalEncBuf,encBuf);
			/* Encode results */
			if(encRetVal.encoded == ENCODE_FAIL)
			{
			    DU_LOG( "\n F1AP : Could not encode Initial UL RRC Message Transfer\
				 structure (at %s)\n",encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
				 break;
			}
			else
			{
			    
				  DU_LOG("\n F1AP : Created APER encoded buffer for Initial UL RRC\
				  Message transfer\n");
				   for(int i=0; i< encBufSize; i++)
					{
					    printf("%x",encBuf[i]);
					}
			}
			/* Sending  msg  */
			if(SendF1APMsg(DU_APP_MEM_REGION,DU_POOL) != ROK)
			{
			    DU_LOG("\n F1AP : Sending Initial UL RRC Message Transfer Failed");
				 break;
			}
			checkvar=true;
			break;
	}
   
	if(f1apMsg != NULLP)
   {
	    if(f1apMsg->choice.initiatingMessage != NULLP)
		 {
		     if(initULRRCMsg->protocolIEs.list.array != NULLP)
			  {
		         if(idx == elementCnt)
			      {
			 	       idx1=1;
			          if(initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.NRCGI.pLMN_Identity.buf!=NULLP)
				       {
				           if(initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.NRCGI.nRCellIdentity.buf!=NULLP)
					        {
					            DU_FREE(initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.NRCGI.nRCellIdentity.buf,
						         initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.NRCGI.nRCellIdentity.size);
					        }
					        DU_FREE(initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.NRCGI.pLMN_Identity.buf,\
					         initULRRCMsg->protocolIEs.list.array[idx1]->value.choice.NRCGI.pLMN_Identity.size);
				        }
				        for(ieId=0; ieId<elementCnt; ieId++)
				        {
				           DU_FREE(initULRRCMsg->protocolIEs.list.array[ieId],sizeof(InitialULRRCMessageTransferIEs_t));
				        }
			       }
			       else
			       {
			            for(ieId=0; ieId<idx; ieId++)
					      {
					          DU_FREE(initULRRCMsg->protocolIEs.list.array[ieId],sizeof(InitialULRRCMessageTransferIEs_t));
					      }
			        }
			     DU_FREE(initULRRCMsg->protocolIEs.list.array,initULRRCMsg->protocolIEs.list.size);
			 }
			 DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
		 }
		 DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
	}
	if(checkvar==true)
	   return ROK;
	else
	   return RFAILED;
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
	S16 ret;
	cellCnt = 1;
	spCellLst->list.count = cellCnt;
	spCellLst->list.size = cellCnt * sizeof(SCell_ToBeSetup_ItemIEs_t *);
	DU_ALLOC(spCellLst->list.array,spCellLst->list.size);
	if(spCellLst->list.array == NULLP)
	{
		return RFAILED;
	}
	for(idx=0; idx<cellCnt; idx++)
	{
		DU_ALLOC(spCellLst->list.array[idx],sizeof(SCell_ToBeSetup_ItemIEs_t));
		if(spCellLst->list.array[idx] == NULLP)
		{
			return RFAILED;
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
		return RFAILED;
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
	U8 srbCnt;
	srbCnt = 1;
	srbSet->list.count = srbCnt;
	srbSet->list.size = srbCnt*sizeof(SRBs_ToBeSetup_ItemIEs_t *);
	DU_ALLOC(srbSet->list.array,srbSet->list.size);
	if(srbSet->list.array == NULLP)
	{
		return RFAILED;
	}
	for(idx=0; idx<srbCnt; idx++)
	{
		DU_ALLOC(srbSet->list.array[idx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
		if(srbSet->list.array[idx] == NULLP)
	   {
		   return RFAILED;
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
		return RFAILED;
	}
	/*FiveQI*/
 	drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI = 0;
	/*AveragingWindow*/
	DU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
												sizeof(AveragingWindow_t));
	if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow == \
																									NULLP)
	{
		return RFAILED;
	}
	*(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow) = 0;
	/*MaxDataBurstVolume*/
	DU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
												sizeof(MaxDataBurstVolume_t));
	if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume == \
																									NULLP)
	{
		return RFAILED;
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
		return RFAILED;
	}
	snssai->sST.buf[0] = 3;
	  /*sD*/
	DU_ALLOC(snssai->sD,sizeof(OCTET_STRING_t));
	if(snssai->sD == NULLP)
	{
		return RFAILED;
	}
	snssai->sD->size = 3*sizeof(U8);
	DU_ALLOC(snssai->sD->buf,snssai->sD->size);
	if(snssai->sD->buf == NULLP)
	{
		return RFAILED;
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
	U8 flowCnt;
	flowCnt = 1;
	flowMap->list.count = flowCnt;
	flowMap->list.size = flowCnt * sizeof(Flows_Mapped_To_DRB_Item_t *);
	DU_ALLOC(flowMap->list.array,flowMap->list.size);
	if(flowMap->list.array == NULLP)
	{
		return RFAILED;
	}
	for(idx=0; idx<flowCnt; idx++)
	{
		DU_ALLOC(flowMap->list.array[idx],sizeof(Flows_Mapped_To_DRB_Item_t));
		if(flowMap->list.array[idx] == NULLP)
		{
		   return RFAILED;
		}
	}
	idx = 0;
	flowMap->list.array[idx]->qoSFlowIdentifier = 0;
	ret = BuildQOSInfo(&flowMap->list.array[idx]->qoSFlowLevelQoSParameters);
	if(ret != ROK)
	{
		return RFAILED;
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
	U8 ulCnt;
	ulCnt = 1;
	ulInfo->list.count = ulCnt;
	ulInfo->list.size = ulCnt * sizeof(ULUPTNLInformation_ToBeSetup_Item_t *);
	DU_ALLOC(ulInfo->list.array,ulInfo->list.size);
	if(ulInfo->list.array == NULLP)
	{
		return RFAILED;
	}
	for(idx=0; idx<ulCnt; idx++)
	{
		DU_ALLOC(ulInfo->list.array[idx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
		if(ulInfo->list.array[idx] == NULLP)
		{
			return RFAILED;
		}
	}
	idx = 0;
	ulInfo->list.array[idx]->uLUPTNLInformation.present = \
								UPTransportLayerInformation_PR_gTPTunnel;
	/*GTP TUNNEL*/
	DU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel,\
				sizeof(GTPTunnel_t));
	if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel == NULLP)
	{
		return RFAILED;
	}
	ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.size	= 4*sizeof(U8);
   DU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf,ulInfo->list.array[idx]->\
		uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
	if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
		transportLayerAddress.buf == NULLP)
	{
		return RFAILED;
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
		return RFAILED;
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
	S16 BuildQOSInforet;
	S16 BuildSNSSAIret;
	S16 BuildFlowsMapret;
	S16 BuildULTnlInforet;
	U8	 idx;
	U8  drbCnt;
	DRBs_ToBeSetup_Item_t *drbSetItem;
	drbCnt = 1;
	drbSet->list.count = drbCnt;
	drbSet->list.size = drbCnt*sizeof(DRBs_ToBeSetup_ItemIEs_t *);
	DU_ALLOC(drbSet->list.array,drbSet->list.size);
	if(drbSet->list.array == NULLP)
	{
		return RFAILED;
	}
	for(idx=0; idx<drbCnt; idx++)
	{
		DU_ALLOC(drbSet->list.array[idx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
		if(drbSet->list.array[idx] == NULLP)
		{
			return RFAILED;
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
	DU_ALLOC(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
	if(drbSetItem->qoSInformation.choice.choice_extension == NULLP)
	{	
		return RFAILED;
	}
	drbSetItem->qoSInformation.choice.choice_extension->id = \
							ProtocolIE_ID_id_DRB_Information;
	drbSetItem->qoSInformation.choice.choice_extension->criticality = \
							Criticality_ignore;
	drbSetItem->qoSInformation.choice.choice_extension->value.present = \
							QoSInformation_ExtIEs__value_PR_DRB_Information;
	BuildQOSInforet = BuildQOSInfo(&drbSetItem->qoSInformation.choice.\
							choice_extension->value.choice.DRB_Information.dRB_QoS);
	if(BuildQOSInforet != ROK)
	{
		return RFAILED;
	}
	/*SNSSAI*/
 	BuildSNSSAIret = BuildSNSSAI(&drbSetItem->qoSInformation.choice.\
							choice_extension->value.choice.DRB_Information.sNSSAI);
	if(BuildSNSSAIret != ROK)
	{	
		return RFAILED;
	}
	/*Flows mapped to DRB List*/
	BuildFlowsMapret = BuildFlowsMap(&drbSetItem->qoSInformation.choice.\
				choice_extension->value.choice.DRB_Information.flows_Mapped_To_DRB_List);
	if(BuildFlowsMapret != ROK)
	{
		return RFAILED;
	}
	/*ULUPTNLInformation To Be Setup List*/
   BuildULTnlInforet = BuildULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
	if(BuildULTnlInforet != ROK)
	{
		return RFAILED;
	}
	/*RLCMode*/
	check=1;
	drbSetItem->rLCMode = RLCMode_rlc_um_bidirectional;

	/*UL Configuration*/
	DU_ALLOC(drbSetItem->uLConfiguration,sizeof(ULConfiguration_t));
	if(drbSetItem->uLConfiguration == NULLP)
	{
	   return RFAILED;
	}
	drbSetItem->uLConfiguration->uLUEConfiguration = ULUEConfiguration_no_data;
        return ROK;
}/* End of BuildDRBSetup*/
/*******************************************************************
*
* @brief Deallocating memory of function BuildAndSendUESetReq
*
* @details
*
*    Function : FreeNrcgi
*
*    Functionality: Deallocating memory for function BuildNrcgi
*
* @params[in] NRCGI_t *nrcgi
*
* @return void
*
*******************************************************************/
void FreeNrcgi(NRCGI_t *nrcgi)
{
   U8 byteSize = 5;
   nrcgi->pLMN_Identity.size = PLMN_SIZE * sizeof(U8);
   nrcgi->nRCellIdentity.size = byteSize * sizeof(U8);
	if(nrcgi->pLMN_Identity.buf != NULLP)
	{
	     if(nrcgi->nRCellIdentity.buf != NULLP)
	     {
           DU_FREE(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
		  }
	     DU_FREE(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
	}
}
/*******************************************************************
*
* @brief  Deallocating memory of function BuildAndSendUESetReq
*
* @details
*
*    Function : FreeSplCellList
*
*    Functionality: Deallocating memory for function BuildSplCellList
*
* @params[in] SCell_ToBeSetup_List_t *spCellLst
*
* @return void
*      
*
* *****************************************************************/
void FreeSplCellList(SCell_ToBeSetup_List_t *spCellLst)
{
    U8  cellidx;
	 U8  cellCnt=1;
    spCellLst->list.size = cellCnt*sizeof(SCell_ToBeSetup_ItemIEs_t);
    if(spCellLst->list.array != NULLP)
	 {
	      for(cellidx=0; cellidx<cellCnt; cellidx++)
			{
			    if(cellidx==0&&spCellLst->list.array[cellidx]!=NULLP)
				 {
				    FreeNrcgi(&spCellLst->list.array[cellidx]->value.choice.SCell_ToBeSetup_Item.sCell_ID);
				 }
				 if(spCellLst->list.array[cellidx]!=NULLP)
				 {
				     DU_FREE(spCellLst->list.array[cellidx],sizeof(SCell_ToBeSetup_ItemIEs_t));
				 }
			}
			DU_FREE(spCellLst->list.array,spCellLst->list.size);
	 }
}
 /*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeSRBSetup
 *
 *    Functionality: Deallocating memory for function BuildSRBSetup
 *
 * @params[in] SRBs_ToBeSetup_List_t *srbSet
 *
 * @return void
 *        
 *
 * ******************************************************************/
void FreeSRBSetup(SRBs_ToBeSetup_List_t *srbSet)
{
    U8 srbidx;
	 U8 srbCnt=1;
	 srbSet->list.size=srbCnt*sizeof(SRBs_ToBeSetup_ItemIEs_t);
	 if(srbSet->list.array != NULLP)
	 {
	     for(srbidx=0; srbidx<srbCnt; srbidx++)
		  {
		      if(srbSet->list.array[srbidx]!=NULLP)
				{
				    DU_FREE(srbSet->list.array[srbidx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
				}
		  }
		  DU_FREE(srbSet->list.array,srbSet->list.size);
	 }
}
 /*******************************************************************
  *
  * @brief Deallocating memory of function BuildAndSendUESetReq
  *
  * @details
  *
  *    Function : FreeQOSInfo
  *
  *    Functionality:  Deallocating memory for function BuildQOSInfo
  *
  * @params[in] QoSFlowLevelQoSParameters_t *drbQos
  *
  * @return void
  *          
  * ****************************************************************/
void FreeQOSInfo(QoSFlowLevelQoSParameters_t *drbQos)
{
    if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI != NULLP)
	 {
	     if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
		  {
		      if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
				{
				    DU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
					 sizeof(MaxDataBurstVolume_t));
				}
				 DU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
				 sizeof(AveragingWindow_t));
		  }
		  DU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,\
		  sizeof(NonDynamic5QIDescriptor_t));
	 }
}
 /*******************************************************************
  *
  * @brief Deallocating memory of function BuildAndSendUESetReq
  *
  * @details
  *
  *    Function : FreeULTnlInfo
  *
  *    Functionality:  Deallocating memory for function BuildULTnlInfo
  *
  * @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
  *
  * @return void
  *         
 
 * ****************************************************************/
void FreeULTnlInfo(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
    U8 ulidx=0;
	 U8 ulCnt=1;
	 ulInfo->list.size=ulCnt*sizeof(ULUPTNLInformation_ToBeSetup_Item_t *);
	 if(ulInfo->list.array != NULLP)
	 {
	     for(ulidx=0; ulidx<ulCnt; ulidx++)
		  {
		      if(ulidx==0&&ulInfo->list.array[ulidx]!=NULLP)
				{
				    if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel
					 !=NULLP)
					 {
					     if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
						  transportLayerAddress.buf != NULLP)
						  {
						      if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
								!=NULLP)
								{
								     DU_ALLOC(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
									  gTP_TEID.buf,ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.\
									  gTPTunnel->gTP_TEID.size);
								}
								DU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
								transportLayerAddress.buf,ulInfo->list.array[ulidx]->\
								uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
						  }
						  DU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel,\
						  sizeof(GTPTunnel_t));
					 }

				}
				if(ulInfo->list.array[ulidx]!=NULLP)
				{
				    DU_FREE(ulInfo->list.array[ulidx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
				}
		  }
		  DU_FREE(ulInfo->list.array,ulInfo->list.size);
	 }
}
/*******************************************************************
 *
 * @brief Deallocating memory for BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeDRBSetup
 *
 *    Functionality:  Deallocating memory for BuildDRBSetup
 *
 * @params[in] DRBs_ToBeSetup_List_t *drbSet
 *
 * @return void
 *
 * ****************************************************************/
void FreeDRBSetup(DRBs_ToBeSetup_List_t *drbSet)
{
    DRBs_ToBeSetup_Item_t *drbSetItem;
	 U8  flowidx;
	 U8  flowCnt=1;
	 U8  drbidx;
	 U8  idx=1;
    if(drbSet->list.array == NULLP)
	 {
	     for(drbidx=0; drbidx<idx; drbidx++)
		  {
		       if(drbidx==0&&drbSet->list.array[idx] != NULLP)
				 {
				    drbSetItem =&drbSet->list.array[idx]->value.choice.DRBs_ToBeSetup_Item;
				    if(drbSetItem->qoSInformation.choice.choice_extension != NULLP)
					 {
					       if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
							 qoS_Characteristics.choice.non_Dynamic_5QI !=NULLP)
							 {
							     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
								  qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
								  {
								       if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
										 qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
										 {
										     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf!=
											  NULLP)
											  {
											      if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD!=NULLP)
													{
													    if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf!=NULLP)
														 {
														      if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
																flows_Mapped_To_DRB_List.list.array != NULLP)
																{
																    for(flowidx=0;flowidx<flowCnt; flowidx++)
																	 {
																	     if(flowidx==0&&drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																		  DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]!=NULLP)
																		  {
																		      if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																				DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
																				 qoS_Characteristics.choice.non_Dynamic_5QI!=NULLP)
																				{
																				     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																					  DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
																					   qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
																					  {
																					      if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																							DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
																							qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
																						   {	
																								 if(check==1&&drbSetItem->uLConfiguration!= NULLP)
																								 {
																								     DU_FREE(drbSetItem->uLConfiguration,sizeof(ULConfiguration_t));
																								 }
																								 FreeULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
                                                                         DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																								 DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
																						       qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
																								 sizeof(MaxDataBurstVolume_t));	  
																						   }
																							DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																							DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
																							qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
																					  }
																					  DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																					  DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
                                                                 qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
																				}
																		  }
																		  if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																		  DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]!=NULLP)
																		  {
																		      DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
																				DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx],sizeof(Flows_Mapped_To_DRB_Item_t));
																		  }
																	 }
																    DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
																	 flows_Mapped_To_DRB_List.list.array,drbSetItem->qoSInformation.choice.choice_extension->value.\
																	 choice.DRB_Information.flows_Mapped_To_DRB_List.list.size);
																}
														      DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf,\
																3*sizeof(U8));
														 }
														 DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD,\
														 sizeof(OCTET_STRING_t));
													}
													 DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf,\
													 sizeof(U8));

											  }
											  DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
										     qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
										 }
								       DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
										 qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
								  }
							     DU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
								  qoS_Characteristics.choice.non_Dynamic_5QI, sizeof(NonDynamic5QIDescriptor_t));
							 }
						   DU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
					 }
				 }
		       if(drbSet->list.array[drbidx]!=NULLP)
				 {
				     DU_FREE(drbSet->list.array[drbidx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
				}
		  }
	     DU_FREE(drbSet->list.array,drbSet->list.size);
	 }
}
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
	U8   elementCnt,BuildNrcgiret,BuildSplCellListret,BuildSRBSetupret;
	U8   idx;
	F1AP_PDU_t      					*f1apMsg = NULL;
   UEContextSetupRequest_t			*ueSetReq;
	asn_enc_rval_t  					encRetVal;        /* Encoder return value */
   bool checkvar=false;
	while(1)
	{
	    DU_LOG("\n F1AP : Building UE Context Setup Request\n");

	    DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
	    if(f1apMsg == NULLP)
	    {
		    DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
			 break;
	    }

	    f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
	    DU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
	    if(f1apMsg->choice.initiatingMessage == NULLP)
	    {
		     DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
			  break;
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
	    DU_ALLOC(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);
	    
		 if(ueSetReq->protocolIEs.list.array == NULLP)
	    {
		     DU_LOG(" F1AP : Memory allocation for UE Context SetupRequest failed");
			  break;
	    }

	    for(idx=0; idx<elementCnt; idx++)
	    {
		     DU_ALLOC(ueSetReq->protocolIEs.list.array[idx],sizeof(UEContextSetupRequestIEs_t));
		     if(ueSetReq->protocolIEs.list.array[idx] == NULLP)
		     {
			     break;
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
 	    BuildNrcgiret = BuildNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI);
	    if(BuildNrcgiret != ROK)
	    {
		    break;
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
	     BuildSplCellListret = BuildSplCellList(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SCell_ToBeSetup_List);
	     if(BuildSplCellListret != ROK)
	     {  
		     break;
	     }
	     /*SRBs To Be Setup List*/
 	     idx++;
	     ueSetReq->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_SRBs_ToBeSetup_List;
	     ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	     ueSetReq->protocolIEs.list.array[idx]->value.present = \
	              				UEContextSetupRequestIEs__value_PR_SRBs_ToBeSetup_List;
 	     BuildSRBSetupret =	BuildSRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List);
	     if(BuildSRBSetupret != ROK)
	     {        
		     break;
	     }
	     /*DRBs to Be Setup List*/
	     idx++;
	     ueSetReq->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_DRBs_ToBeSetup_List;
	     ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	     ueSetReq->protocolIEs.list.array[idx]->value.present = \
	              				UEContextSetupRequestIEs__value_PR_DRBs_ToBeSetup_List;
	     ret = BuildDRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List);
	     if(ret != ROK)
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
	      	DU_LOG( "\n F1AP : Could not encode UE Context Setup Request structure (at %s)\n",\
				encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		      break;
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
		      break;
	    }
		 checkvar=true;
		 break;
   }
   if(f1apMsg != NULLP)
	{
	    if(f1apMsg->choice.initiatingMessage != NULLP)
		 {
		      if(ueSetReq->protocolIEs.list.array != NULLP)
				{
			       for(idx=0; idx<elementCnt; idx++)
					 {
					     if(idx==0&&ueSetReq->protocolIEs.list.array[idx] != NULLP)
						  {
						      if(BuildNrcgiret==ROK)
								{
								     if(BuildSplCellListret==ROK)
									  {
									      if(BuildSRBSetupret == ROK)
											{
											   FreeDRBSetup(&ueSetReq->protocolIEs.list.array[8]->value.choice.DRBs_ToBeSetup_List); 
											}
											FreeSRBSetup(&ueSetReq->protocolIEs.list.array[7]->value.choice.SRBs_ToBeSetup_List);
								     }    
									  FreeSplCellList(&ueSetReq->protocolIEs.list.array[6]->value.choice.SCell_ToBeSetup_List); 
								}
						      FreeNrcgi(&ueSetReq->protocolIEs.list.array[2]->value.choice.NRCGI);   
						  }
                    if(ueSetReq->protocolIEs.list.array[idx] != NULLP)
						  {
						     DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));  
						  }
					 }
					 DU_FREE(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);	   
				}
		      DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
		 }
	    DU_FREE(f1apMsg, sizeof(F1AP_PDU_t)); 
	}
	if(checkvar==true)
        return ROK;
	else
	     return RFAILED;
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
