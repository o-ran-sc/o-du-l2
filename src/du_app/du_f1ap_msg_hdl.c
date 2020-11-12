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
#include "common_def.h"
#include "lrg.h"
#include "legtp.h"
#include "lkw.x"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "du_cfg.h"
#include "du_app_rlc_inf.h"
#include "du_mgr_main.h"
#include "du_utils.h"
#include "GNB-DU-System-Information.h"
#include "CellGroupConfigRrc.h"
#include "MAC-CellGroupConfig.h"
#include "SchedulingRequestConfig.h"
#include "SchedulingRequestToAddMod.h"
#include "BSR-Config.h"
#include "TAG-Config.h"
#include "TAG.h"
#include "PHR-Config.h"
#include "RLC-Config.h"
#include "UL-AM-RLC.h"
#include "DL-AM-RLC.h"
#include "LogicalChannelConfig.h"
#include "RLC-BearerConfig.h"
#include "PhysicalCellGroupConfig.h"
#include "SpCellConfig.h"
#include "TDD-UL-DL-ConfigDedicated.h"
#include "ServingCellConfig.h"
#include "ControlResourceSet.h"
#include "SearchSpace.h"
#include "PDCCH-Config.h"
#include "PDSCH-TimeDomainResourceAllocation.h"
#include "PDSCH-TimeDomainResourceAllocationList.h"
#include "PDSCH-CodeBlockGroupTransmission.h"
#include "PDSCH-ServingCellConfig.h"
#include "DMRS-DownlinkConfig.h"
#include "PDSCH-Config.h"
#include "BWP-DownlinkDedicated.h"
#include "PUSCH-TimeDomainResourceAllocation.h"
#include "PUSCH-TimeDomainResourceAllocationList.h"
#include "DMRS-UplinkConfig.h"
#include "PUSCH-Config.h"
#include "SRS-ResourceId.h"
#include "SRS-Resource.h"
#include "SRS-ResourceSet.h"
#include "SRS-Config.h"
#include "BWP-UplinkDedicated.h"
#include "PUSCH-ServingCellConfig.h"
#include "UplinkConfig.h"
#include "DUtoCURRCContainer.h"
#include "GBR-QoSFlowInformation.h"
#include "QoSFlowLevelQoSParameters.h"
#include "PUCCH-Config.h"
#include "PUCCH-ResourceSet.h"
#include "PUCCH-Resource.h"
#include "PUCCH-PowerControl.h"
#include "P0-PUCCH.h"
#include "PUCCH-PathlossReferenceRS.h"
#include "PUCCH-format0.h"
#include "PUCCH-format1.h"
#include "PUCCH-format2.h"
#include "PUCCH-format3.h"
#include "PUCCH-format4.h"
#include "PUCCH-FormatConfig.h"
#include "SchedulingRequestResourceConfig.h"
#include<ProtocolIE-Field.h>
#include "ProtocolExtensionField.h"
#include "F1AP-PDU.h"
#include "odu_common_codec.h"
#include "du_mgr.h"
#include "du_cell_mgr.h"
#include "du_f1ap_msg_hdl.h"

DuCfgParams duCfgParam;

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
uint8_t BuildULNRInfo(NRFreqInfo_t *ulnrfreq)
{
   uint8_t idx=0;
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
uint8_t BuildDLNRInfo(NRFreqInfo_t *dlnrfreq)
{
   uint8_t idx=0;
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
 * @brief Builds NRCell ID 
 *
 * @details
 *
 *    Function : BuildNrCellId
 *
 *    Functionality: Building the NR Cell ID
 *
 * @params[in] BIT_STRING_t *nrcell
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildNrCellId(BIT_STRING_t *nrcell)
{
   memset(nrcell->buf, 0, nrcell->size);
   nrcell->buf[4]   = 16; 
   nrcell->bits_unused = 4;
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
uint8_t BuildNrcgi(NRCGI_t *nrcgi)
{
   uint8_t ret;
   uint8_t byteSize = 5;
   /* Allocate Buffer Memory */
   nrcgi->pLMN_Identity.size = PLMN_SIZE * sizeof(uint8_t);
   DU_ALLOC(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   if(nrcgi->pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   ret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 nrcgi->pLMN_Identity.buf); // Building PLMN function
   if(ret != ROK)
   {
      return RFAILED;
   }
   /*nrCellIdentity*/
   nrcgi->nRCellIdentity.size = byteSize * sizeof(uint8_t);
   DU_ALLOC(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
   if(nrcgi->nRCellIdentity.buf == NULLP)
   {
      return RFAILED;
   }
   BuildNrCellId(&nrcgi->nRCellIdentity);

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
uint8_t BuildFiveGSTac(Served_Cell_Information_t *servcell)
{
   DU_ALLOC(servcell->fiveGS_TAC,sizeof(FiveGS_TAC_t));
   if(servcell->fiveGS_TAC == NULLP)
   {
      return RFAILED;
   }
   servcell->fiveGS_TAC->size = 3 * sizeof(uint8_t);
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
uint8_t BuildNrMode(NR_Mode_Info_t *mode)
{
   uint8_t BuildDLNRInforet=0;
   uint8_t BuildULNRInforet=0; 
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
uint8_t BuildExtensions(ProtocolExtensionContainer_4624P3_t **ieExtend)
{
   uint8_t idx;
   uint8_t plmnidx;
   uint8_t extensionCnt=1;
   uint8_t sliceId=0;
   uint8_t sdId;
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
      list.array[sliceId]->sNSSAI.sST.size = sizeof(uint8_t);
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
      list.array[sliceId]->sNSSAI.sD->size = 3*sizeof(uint8_t);
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
uint8_t  BuildServedPlmn(ServedPLMNs_List_t *srvplmn)
{  
   uint8_t  plmnidx;
   uint8_t  servPlmnCnt=1;
   uint8_t buildPlmnIdret=0;
   uint8_t BuildExtensionsret=0;
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
   srvplmn->list.array[0]->pLMN_Identity.size = PLMN_SIZE * sizeof(uint8_t);
   DU_ALLOC(srvplmn->list.array[0]->pLMN_Identity.buf, srvplmn->list.array[0]->pLMN_Identity.size);
   buildPlmnIdret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 srvplmn->list.array[0]->pLMN_Identity.buf);
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

uint8_t BuildServedCellList(GNB_DU_Served_Cells_List_t *duServedCell)
{
   uint8_t  BuildNrcgiret=0;
   uint8_t  BuildFiveGSTacret=0;
   uint8_t  BuildServedPlmnret=0;
   uint8_t  BuildNrModeret=0;
   uint8_t  idx;
   uint8_t  plmnidx;
   uint8_t  plmnCnt=1;
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
      size = sizeof(uint8_t);
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
uint8_t BuildRrcVer(RRC_Version_t *rrcVer)
{
   uint8_t rrcExt;
   uint8_t rrcLatest;
   rrcVer->latest_RRC_Version.size = sizeof(uint8_t);
   DU_ALLOC(rrcVer->latest_RRC_Version.buf,sizeof(uint8_t));
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
      .Latest_RRC_Version_Enhanced.size = 3*sizeof(uint8_t);
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
uint8_t SendF1APMsg(Region region, Pool pool)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
	 ODU_PRINT_MSG(mBuf, 0,0);

	 if(sctpSend(mBuf, F1_INTERFACE) != ROK)
	 {
	    DU_LOG("\nF1AP : SCTP Send failed");
	    ODU_PUT_MSG_BUF(mBuf);
	    return RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nF1AP : ODU_ADD_POST_MSG_MULT failed");
	 ODU_PUT_MSG_BUF(mBuf);
	 return RFAILED;
      }
      ODU_PUT_MSG_BUF(mBuf);
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
   uint8_t   plmnCnt=1;
   uint8_t  servId=0;
   uint8_t sliceId=0;
   uint8_t  ieId=0;
   uint8_t   extensionCnt=1;
   uint8_t plmnidx=0;
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
						      sNSSAI.sST.buf,sizeof(uint8_t));
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
				    * sizeof(uint8_t));
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
		     sizeof(uint8_t));
	    }
	    DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
		  srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size *
		  sizeof(uint8_t));
	 }
	 DU_FREE(duServedCell->list.array[plmnidx],sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      }
      DU_FREE(duServedCell->list.array,plmnCnt*sizeof(GNB_DU_Served_Cells_ItemIEs_t*));
   }
}
/*******************************************************************
 *
 * @brief  deallocating the memory of function BuildAndSendF1SetupReq()
 *
 * @details
 *
 *    Function :  FreeF1SetupReq
 *
 *    Functionality:  deallocating the memory of function BuildAndSendF1SetupReq
 *
 * @params[in] F1AP_PDU_t *f1apMsg
 *
 * @return void
 *
 * ****************************************************************/
void FreeF1SetupReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx, ieIdx2;
   F1SetupRequest_t *f1SetupReq=NULLP;

   if(f1apMsg != NULLP)
   {
      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
	 f1SetupReq = &f1apMsg->choice.initiatingMessage->value.choice.F1SetupRequest;
	 if(f1SetupReq->protocolIEs.list.array != NULLP)
	 {
	    for(ieIdx = 0; ieIdx < f1SetupReq->protocolIEs.list.count; ieIdx++)
	    {
	       if(f1SetupReq->protocolIEs.list.array[ieIdx] != NULLP)
	       {
		  switch(f1SetupReq->protocolIEs.list.array[ieIdx]->id)
		  {
		     case ProtocolIE_ID_id_TransactionID:
			break;
		     case ProtocolIE_ID_id_gNB_DU_ID:
			DU_FREE(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf,\
			      f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size);
			break;
		     case ProtocolIE_ID_id_gNB_DU_Name:
			DU_FREE(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.buf,\
			      strlen((char *)duCfgParam.duName));
			break;
		     case ProtocolIE_ID_id_gNB_DU_Served_Cells_List:
			FreeServedCellList(&f1SetupReq->protocolIEs.list.\
			      array[ieIdx]->value.choice.GNB_DU_Served_Cells_List);
			break;
		     case ProtocolIE_ID_id_GNB_DU_RRC_Version:
			FreeRrcVer(&f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.RRC_Version);
			break;
		     default:
			DU_LOG("\nInvalid Event Type %ld", f1SetupReq->protocolIEs.list.array[ieIdx]->id);
			break;
		  }
	       }
	       break;
	    }
	    for(ieIdx2=0; ieIdx2< ieIdx; ieIdx2++)
	    {
	       DU_FREE(f1SetupReq->protocolIEs.list.array[ieIdx2],sizeof(F1SetupRequestIEs_t));
	    }
	    DU_FREE(f1SetupReq->protocolIEs.list.array,\
		  f1SetupReq->protocolIEs.list.size);
	 }
	 DU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
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
uint8_t BuildAndSendF1SetupReq()
{
   uint8_t   ret, ieIdx, elementCnt;
   F1AP_PDU_t                 *f1apMsg = NULLP;
   F1SetupRequest_t           *f1SetupReq=NULLP;
   GNB_DU_Served_Cells_List_t *duServedCell=NULLP;
   RRC_Version_t              *rrcVer=NULLP;
   asn_enc_rval_t             encRetVal;        /* Encoder return value */
   ret= RFAILED;

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
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 DU_ALLOC(f1SetupReq->protocolIEs.list.array[ieIdx],\
	       sizeof(F1SetupRequestIEs_t));
	 if(f1SetupReq->protocolIEs.list.array[ieIdx] == NULLP)
	 {
	    break;
	 }
      }

      ieIdx = 0;
      /*TransactionID*/
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_TransactionID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present =\
							       F1SetupRequestIEs__value_PR_TransactionID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = \
									     TRANS_ID;

      /*DU ID*/
      ieIdx++;
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_ID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = \
								F1SetupRequestIEs__value_PR_GNB_DU_ID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size =\
									     sizeof(uint8_t);

      DU_ALLOC(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf,\
	    f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size);
      if(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf == \
	    NULLP)
      {
	 break;
      }

      f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf[0] =\
         duCfgParam.duId;

      /*DU Name*/
      if(duCfgParam.duName != NULL)
      {
	 ieIdx++;
	 f1SetupReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_Name;
	 f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
	 f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = F1SetupRequestIEs__value_PR_GNB_DU_Name;
	 f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.size =\
	    strlen((char *)duCfgParam.duName);
	 DU_ALLOC(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.\
	       GNB_DU_Name.buf, strlen((char *)duCfgParam.duName));
	 if(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.\
	       buf == NULLP)
	 {
	    break;
	 }
	 strcpy((char*)f1SetupReq->protocolIEs.list.array[ieIdx]->value.\
	       choice.GNB_DU_Name.buf,
	       (char*)&duCfgParam.duName);

      }

      /*Served Cell list */
      ieIdx++;
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = \
						     ProtocolIE_ID_id_gNB_DU_Served_Cells_List;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = \
								F1SetupRequestIEs__value_PR_GNB_DU_Served_Cells_List;
      duServedCell = &f1SetupReq->protocolIEs.list.\
		     array[ieIdx]->value.choice.GNB_DU_Served_Cells_List;
      if(BuildServedCellList(duServedCell))
      {
	 break;
      }
      /*RRC Version*/
      ieIdx++;
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = \
						     ProtocolIE_ID_id_GNB_DU_RRC_Version ;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = \
								F1SetupRequestIEs__value_PR_RRC_Version;
      rrcVer = &f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.RRC_Version;
      if(BuildRrcVer(rrcVer))
      {
	 break;
      }
      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
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
	 for(ieIdx=0; ieIdx< encBufSize; ieIdx++)
	 {
	    printf("%x",encBuf[ieIdx]);
	 }
      }

      /* Sending msg */
      if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
      {
	 DU_LOG("\nF1AP : Sending F1 Setup request failed");
	 break;
      }

      ret=ROK;
      break;
   }while(true);

   FreeF1SetupReq(f1apMsg);

   return ret;
}/* End of BuildAndSendF1SetupReq */

/*******************************************************************
 *
 * @brief Deallocating memory of BuildAndSendDUConfigUpdate
 *
 * @details
 *
 *    Function : FreeDUConfigUpdate
 *
 *    Functionality: Deallocating memory of variables allocated in
 *                    BuildAndSendDUConfigUpdate function
 *
 * @params[in]  F1AP_PDU_t *f1apDuCfg
 *
 * @return ROK     - void
 *
 * ****************************************************************/
void FreeDUConfigUpdate(F1AP_PDU_t *f1apDuCfg)
{
   uint8_t  i;
   uint8_t  idx;
   GNBDUConfigurationUpdate_t *duCfgUpdate = NULLP;
   Served_Cells_To_Modify_List_t  *cellsToModify;
   Served_Cells_To_Modify_Item_t *modifyItem;
   idx=0;
   i=1;
   if(f1apDuCfg != NULLP)
   {
      if(f1apDuCfg->choice.initiatingMessage != NULLP)
      {
	 duCfgUpdate = &f1apDuCfg->choice.initiatingMessage->\
		       value.choice.GNBDUConfigurationUpdate;
	 if(duCfgUpdate->protocolIEs.list.array != NULLP)
	 {
	    if(duCfgUpdate->protocolIEs.list.array[i] != NULLP)
	    {
	       cellsToModify = &duCfgUpdate->protocolIEs.list.array[i]->\
			       value.choice.Served_Cells_To_Modify_List;
	       if(cellsToModify->list.array != NULLP)
	       {
		  if(cellsToModify->list.array[idx] != NULLP)
		  {
		     modifyItem=&cellsToModify->list.array[idx]->value.choice.\
				Served_Cells_To_Modify_Item;
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
				    if(!modifyItem->served_Cell_Information.servedPLMNs.list.array[idx])
				    {
				       if(modifyItem->served_Cell_Information.servedPLMNs.list.\
					     array[idx]->pLMN_Identity.buf != NULLP)
				       {
					  if(modifyItem->served_Cell_Information.servedPLMNs.list.\
						array[idx]->iE_Extensions!= NULLP)
					  {
					     if(modifyItem->served_Cell_Information.servedPLMNs.list.\
						   array[idx]->iE_Extensions->list.array != NULLP)
					     {
						if(modifyItem->served_Cell_Information.servedPLMNs.list.\
						      array[idx]->iE_Extensions->list.array[idx])
						{
						   if(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
							 iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
							 list.array !=NULLP)
						   {
						      if(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
							    iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
							    list.array[idx]!=NULLP)
						      {   
							 if(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
							       iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
							       list.array[idx]->sNSSAI.sST.buf!=NULLP)
							 {
							    if(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
								  iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
								  list.array[idx]->sNSSAI.sD != NULLP)
							    { 
							       if(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
								     iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
								     list.array[idx]->sNSSAI.sD->buf!=NULLP)
							       {
								  if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD
									!=NULLP)
								  {
								     if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
									   fDD->uL_NRFreqInfo.freqBandListNr.list.array!=NULLP)
								     {
									if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
									      fDD->uL_NRFreqInfo.freqBandListNr.list.array[idx]!=NULLP)
									{
									   if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
										 fDD->dL_NRFreqInfo.freqBandListNr.list.array !=NULLP)
									   {
									      if(modifyItem->served_Cell_Information.nR_Mode_Info.\
										    choice.fDD->dL_NRFreqInfo.freqBandListNr.list.\
										    array[idx]!= NULLP)
									      {
										 if(modifyItem->served_Cell_Information.\
										       measurementTimingConfiguration.buf !=NULLP)
										 {
										    idx=2;
										    if(duCfgUpdate->protocolIEs.list.array[idx]->value.\
											  choice.GNB_DU_ID.buf!=NULLP)
										    {
										       DU_FREE(duCfgUpdate->protocolIEs.list.\
											     array[idx]->value.choice.GNB_DU_ID.buf,\
											     duCfgUpdate->protocolIEs.list.array[idx]->\
											     value.choice.GNB_DU_ID.size);
										    }
										    idx=0;
										    DU_FREE(modifyItem->served_Cell_Information.\
											  measurementTimingConfiguration.\
											  buf,modifyItem->served_Cell_Information.\
											  measurementTimingConfiguration.size);
										 }
										 DU_FREE(modifyItem->served_Cell_Information.\
										       nR_Mode_Info.choice.fDD->dL_NRFreqInfo.freqBandListNr.\
										       list.array[idx],sizeof(FreqBandNrItem_t));
									      }
									      DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info\
										    .choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array,\
										    modifyItem->served_Cell_Information.nR_Mode_Info.\
										    choice.fDD->dL_NRFreqInfo.freqBandListNr.list.size);
									   }
									   DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.\
										 choice.fDD->uL_NRFreqInfo.freqBandListNr.list.\
										 array[idx],sizeof(FreqBandNrItem_t));
									}
									DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.\
									      choice.\
									      fDD->uL_NRFreqInfo.freqBandListNr.list.\
									      array,modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
									      fDD->uL_NRFreqInfo.freqBandListNr.list.size);
								     }
								     DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.\
									   fDD,sizeof(FDD_Info_t));
								  }
								  DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
									array[idx]->iE_Extensions->list.array[idx]->extensionValue.choice.\
									SliceSupportList.\
									list.array[idx]->sNSSAI.sD->buf,modifyItem->served_Cell_Information.\
									servedPLMNs.list.array[idx]->iE_Extensions->list.array[idx]->\
									extensionValue.choice.SliceSupportList.list.array[idx]->\
									sNSSAI.sD->size);

							       }
							       DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
								     array[idx]->\
								     iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
								     list.array[idx]->sNSSAI.sD,sizeof(OCTET_STRING_t));
							    }
							    DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
								  iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
								  list.array[idx]->sNSSAI.sST.buf,modifyItem->served_Cell_Information.\
								  servedPLMNs.\
								  list.array[idx]->iE_Extensions->list.array[idx]->extensionValue.choice.\
								  SliceSupportList.list.array[idx]->sNSSAI.sST.size);
							 }
							 DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
							       iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
							       list.array[idx],sizeof(SliceSupportItem_t));
						      }
						      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
							    iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.\
							    list.array,\
							    modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->\
							    iE_Extensions->list.array[idx]->extensionValue.choice.SliceSupportList.list.size);
						   }
						}
						for(i=0;i<modifyItem->served_Cell_Information.servedPLMNs.list.\
						      array[idx]->iE_Extensions->list.count;i++)
						{
						   DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
							 array[idx]->iE_Extensions->list.array[i],\
							 sizeof(ServedPLMNs_ItemExtIEs_t ));
						}
						DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
						      array[idx]->iE_Extensions->list.array,modifyItem->served_Cell_Information.servedPLMNs.
						      list.array[idx]->iE_Extensions->list.size);
					     }
					     DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
						   array[idx]->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P3_t));
					  }
					  DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.\
						array[idx]->pLMN_Identity.buf,
						modifyItem->served_Cell_Information.servedPLMNs.list.array[idx]->pLMN_Identity.size);
				       }
				    }
				    for(i=0;i<modifyItem->served_Cell_Information.servedPLMNs.list.count;i++)
				    {
				       if(modifyItem->served_Cell_Information.servedPLMNs.list.array[i]
					     != NULLP)
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
		  }
		  for(i=0; i<cellsToModify->list.count ;i++)
		  {
		     if(cellsToModify->list.array[i] != NULLP)
		     {
			DU_FREE(cellsToModify->list.array[i],\
			      sizeof(Served_Cells_To_Modify_ItemIEs_t));
		     } 
		  }
		  DU_FREE(cellsToModify->list.array,cellsToModify->list.size);
	       }
	    }
	    for(i=0;i<duCfgUpdate->protocolIEs.list.count;i++)
	    {
	       if(duCfgUpdate->protocolIEs.list.array[i] !=NULLP)
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
}

/*******************************************************************
 *
 * @brief Fills Served Plmns required in ServCellInfo IE
 *
 * @details
 *
 *    Function : fillServedPlmns
 *
 *    Functionality: Fills Served Plmns required in ServCellInfo IE
 *
 * @params[in] Pointer to ServedPLMNs_List_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillServedPlmns(ServedPLMNs_List_t *servedPlmn)
{
   uint8_t ieIdx, ieListCnt;

   servedPlmn->list.array[0]->pLMN_Identity.size = 3*sizeof(uint8_t);
   DU_ALLOC(servedPlmn->list.array[0]->pLMN_Identity.buf, servedPlmn->list.\
	 array[0]->pLMN_Identity.size);
   if(servedPlmn->list.array[0]->pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.plmn[0],\
	 servedPlmn->list.array[0]->pLMN_Identity.buf);
   DU_ALLOC(servedPlmn->list.array[0]->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P3_t));
   if(servedPlmn->list.array[0]->iE_Extensions == NULLP)
   {
      return RFAILED;
   }

   ieListCnt=1;
   servedPlmn->list.array[0]->iE_Extensions->list.count = ieListCnt;
   servedPlmn->list.array[0]->iE_Extensions->list.size = ieListCnt *sizeof(ServedPLMNs_ItemExtIEs_t *);
   DU_ALLOC(servedPlmn->list.array[0]->iE_Extensions->list.array,servedPlmn->list.array[0]->\
	 iE_Extensions->list.size);
   if(servedPlmn->list.array[0]->iE_Extensions->list.array == NULLP)
   {
      return RFAILED;
   }
   for(ieIdx=0;ieIdx<ieListCnt;ieIdx++)
   {
      DU_ALLOC(servedPlmn->list.array[0]->iE_Extensions->list.array[ieIdx],\
	    sizeof(ServedPLMNs_ItemExtIEs_t));
      if(servedPlmn->list.array[0]->iE_Extensions->list.array[ieIdx] == NULLP)
      {
	 return RFAILED;
      }
   }
   //plmnIeExt = servedPlmn->list.array[0]->iE_Extensions; 
   servedPlmn->list.array[0]->iE_Extensions->list.array[0]->id =ProtocolIE_ID_id_TAISliceSupportList;
   servedPlmn->list.array[0]->iE_Extensions->list.array[0]->criticality = Criticality_ignore;
   servedPlmn->list.array[0]->iE_Extensions->list.array[0]->extensionValue.present = \
      ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList;
   servedPlmn->list.array[0]->iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.count = 1;
   servedPlmn->list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.size = sizeof(SliceSupportItem_t *);
   DU_ALLOC(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array,servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.list.size);
   if(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array == NULLP)
   {
      return RFAILED;
   }

   DU_ALLOC(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0],sizeof( SliceSupportItem_t));
   if(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0] == NULLP)
   {
      return RFAILED;
   }
   servedPlmn->list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sST.size = sizeof(uint8_t);
   DU_ALLOC(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0]->sNSSAI.sST.buf,servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.list.array[0]->sNSSAI.sST.size);
   if(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0]->sNSSAI.sST.buf == NULLP)
   {
      return RFAILED;
   }
   servedPlmn->list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sST.buf[0] = 3;
   DU_ALLOC(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0]->sNSSAI.sD,sizeof(OCTET_STRING_t));
   if(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0]->sNSSAI.sD == NULLP)
   {
      return RFAILED;
   }
   servedPlmn->list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->size = 3*sizeof(uint8_t);
   DU_ALLOC(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0]->sNSSAI.sD->buf,servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0]->sNSSAI.sD->size);
   if(servedPlmn->list.array[0]->\
	 iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
	 list.array[0]->sNSSAI.sD->buf == NULLP)
   {
      return RFAILED;
   }
   servedPlmn->list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->buf[0] = 3;
   servedPlmn->list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->buf[1] = 6;
   servedPlmn->list.array[0]->\
      iE_Extensions->list.array[0]->extensionValue.choice.SliceSupportList.\
      list.array[0]->sNSSAI.sD->buf[2] = 9;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Nr Fdd Info required in ServCellInfo IE
 *
 * @details
 *
 *    Function : fillNrFddInfo
 *
 *    Functionality: Fills Nr Fdd Info required in ServCellInfo IE
 *
 * @params[in] Pointer to NR_Mode_Info_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillNrFddInfo(NR_Mode_Info_t *nrFdd)
{
   nrFdd->choice.fDD->uL_NRFreqInfo.nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn;
   nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.count = 1;
   nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.size = sizeof(FreqBandNrItem_t*);
   DU_ALLOC(nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.\
	 array, nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.size);
   if(nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.array == NULLP)
   {
      return RFAILED;
   }
   DU_ALLOC(nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.array[0], \
      sizeof(FreqBandNrItem_t));
   if(nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.array[0] == NULLP)
   {
      return RFAILED;
   }
   nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.array[0]->freqBandIndicatorNr = \
      duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.\
      freqBand[0].nrFreqBand;
   nrFdd->choice.fDD->uL_NRFreqInfo.freqBandListNr.list.array[0]->supportedSULBandList.list.count=0;
   nrFdd->choice.fDD->dL_NRFreqInfo.nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.\
      dlNrFreqInfo.nrArfcn;
   nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.count = 1;
   nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.size = sizeof(FreqBandNrItem_t *);
   DU_ALLOC(nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array,nrFdd->\
	 choice.fDD->dL_NRFreqInfo.freqBandListNr.list.size);
   if(nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array == NULLP)
   {
      return RFAILED;
   }
   DU_ALLOC(nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array[0],\
	 sizeof(FreqBandNrItem_t));
   if(nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array[0] == NULLP)
   {
      return RFAILED;
   }
   nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array[0]->freqBandIndicatorNr = \
      duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.\
      freqBand[0].nrFreqBand;
   nrFdd->choice.fDD->dL_NRFreqInfo.freqBandListNr.list.array[0]->supportedSULBandList.list.count=0;
   
   /*Transmission Bandwidth*/
   nrFdd->choice.fDD->uL_Transmission_Bandwidth.nRSCS = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulTxBw.nrScs;
   nrFdd->choice.fDD->uL_Transmission_Bandwidth.nRNRB = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulTxBw.nrb;
   nrFdd->choice.fDD->dL_Transmission_Bandwidth.nRSCS = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.dlTxBw.nrScs;
   nrFdd->choice.fDD->dL_Transmission_Bandwidth.nRNRB = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.dlTxBw.nrb;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills ServCellInfo IE
 *
 * @details
 *
 *    Function : fillServedCellInfo
 *
 *    Functionality: Fills ServCellInfo
 *
 * @params[in] Pointer to Served_Cell_Information_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillServedCellInfo(Served_Cell_Information_t *srvCellInfo)
{
   uint8_t tmp, ieIdx, ieListCnt;

   /*nRCGI*/
   srvCellInfo->nRCGI.pLMN_Identity.size =3*sizeof(uint8_t);
   DU_ALLOC(srvCellInfo->nRCGI.pLMN_Identity.buf,\
	 srvCellInfo->nRCGI.pLMN_Identity.size);
   if(srvCellInfo->nRCGI.pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 srvCellInfo->nRCGI.pLMN_Identity.buf);
   srvCellInfo->nRCGI.nRCellIdentity.size =5*sizeof(uint8_t);
   DU_ALLOC(srvCellInfo->nRCGI.nRCellIdentity.buf,\
	 srvCellInfo->nRCGI.nRCellIdentity.size);
   if(srvCellInfo->nRCGI.nRCellIdentity.buf == NULLP)
   {
      return RFAILED;
   }
   for (tmp = 0 ; tmp < srvCellInfo->\
	 nRCGI.nRCellIdentity.size-1 ; tmp++)
   {
      srvCellInfo->nRCGI.nRCellIdentity.buf[tmp] = 0;
   }
   srvCellInfo->nRCGI.nRCellIdentity.buf[4] = 16;
   srvCellInfo->nRCGI.nRCellIdentity.bits_unused =4;

   /*nRPCI*/
   srvCellInfo->nRPCI = duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrPci;

   /*servedPLMNs*/
   ieListCnt = 1;
   srvCellInfo->servedPLMNs.list.count = ieListCnt;
   srvCellInfo->servedPLMNs.list.size = ieListCnt*sizeof(ServedPLMNs_Item_t *);
   DU_ALLOC(srvCellInfo->servedPLMNs.list.array,\
	 srvCellInfo->servedPLMNs.list.size);
   if(srvCellInfo->servedPLMNs.list.array == NULLP)
   {
      return RFAILED;
   }
   for(ieIdx=0; ieIdx < ieListCnt; ieIdx++)
   {
      DU_ALLOC(srvCellInfo->servedPLMNs.list.array[ieIdx],\
	    sizeof(ServedPLMNs_Item_t));
      if(srvCellInfo->servedPLMNs.list.array[ieIdx]== NULLP)
      {
	 return RFAILED;
      }
   }
   if(fillServedPlmns(&srvCellInfo->servedPLMNs))
   {
      return RFAILED;
   }

   /*nR Mode Info with FDD*/
   srvCellInfo->nR_Mode_Info.present = NR_Mode_Info_PR_fDD;
   DU_ALLOC(srvCellInfo->nR_Mode_Info.choice.fDD,\
         sizeof(FDD_Info_t));
   if(srvCellInfo->nR_Mode_Info.choice.fDD == NULLP)
   {
      return RFAILED;
   }
   if(fillNrFddInfo(&srvCellInfo->nR_Mode_Info))
      return RFAILED;

   /*Measurement timing Config*/
   srvCellInfo->measurementTimingConfiguration.size = sizeof(uint8_t);
   DU_ALLOC(srvCellInfo->measurementTimingConfiguration.\
         buf,srvCellInfo->measurementTimingConfiguration.size);
   if(srvCellInfo->measurementTimingConfiguration.buf == NULLP)
   {
      return RFAILED;
   }
   srvCellInfo->measurementTimingConfiguration.\
	 buf[0] = duCfgParam.srvdCellLst[0].duCellInfo.measTimeCfg;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills ServCellToModItem IE
 *
 * @details
 *
 *    Function : fillServCellToModItem
 *
 *    Functionality: Fills ServCellToModItem IE
 *
 * @params[in] Pointer to Served_Cells_To_Modify_Item_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillServCellToModItem(Served_Cells_To_Modify_Item_t *modifyItem)
{
   uint8_t ieIdx;

   /*pLMN_Identity*/
   modifyItem->oldNRCGI.pLMN_Identity.size = 3*sizeof(uint8_t);
   DU_ALLOC(modifyItem->oldNRCGI.pLMN_Identity.buf,modifyItem->oldNRCGI.pLMN_Identity.size);
   if(modifyItem->oldNRCGI.pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 modifyItem->oldNRCGI.pLMN_Identity.buf);

   /*nRCellIdentity*/
   modifyItem->oldNRCGI.nRCellIdentity.size = 5*sizeof(uint8_t);
   DU_ALLOC(modifyItem->oldNRCGI.nRCellIdentity.buf,\
	 modifyItem->oldNRCGI.nRCellIdentity.size);
   if(modifyItem->oldNRCGI.nRCellIdentity.buf == NULLP)
   {
      return RFAILED;
   }
   for(ieIdx = 0; ieIdx < modifyItem->oldNRCGI.nRCellIdentity.size-1; ieIdx++)
   {
      modifyItem->oldNRCGI.nRCellIdentity.buf[ieIdx] = 0;
   }
   modifyItem->oldNRCGI.nRCellIdentity.buf[4] = 16;
   modifyItem->oldNRCGI.nRCellIdentity.bits_unused = 4;

   if(fillServedCellInfo(&modifyItem->served_Cell_Information))
      return RFAILED;
   else
      return ROK;
}

/*******************************************************************
 *
 * @brief Builds ServCellToModList
 *
 * @details
 *
 *    Function : buildServCellToModList
 *
 *    Functionality: Builds the serv cell to Mod List
 *
 * @params[in] Pointer to Served_Cells_To_Modify_List_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t buildServCellToModList(Served_Cells_To_Modify_List_t *cellsToModify)
{
   uint8_t ieListCnt, ieIdx;
   Served_Cells_To_Modify_Item_t *modifyItem = NULLP;

   ieListCnt = 1;
   cellsToModify->list.count = ieListCnt;
   cellsToModify->list.size = ieListCnt*sizeof(Served_Cells_To_Modify_ItemIEs_t *);
   DU_ALLOC(cellsToModify->list.array,cellsToModify->list.size);
   if(cellsToModify->list.array == NULLP)
   {
      return RFAILED;
   }
   for(ieIdx=0; ieIdx< ieListCnt; ieIdx++)
   {
      DU_ALLOC(cellsToModify->list.array[ieIdx],sizeof(Served_Cells_To_Modify_ItemIEs_t));
      if(cellsToModify->list.array[ieIdx] == NULLP)
      {
	 return RFAILED;
      }
   }
   cellsToModify->list.array[0]->id = ProtocolIE_ID_id_Served_Cells_To_Modify_Item;
   cellsToModify->list.array[0]->criticality = Criticality_reject;
   cellsToModify->list.array[0]->value.present =\
      Served_Cells_To_Modify_ItemIEs__value_PR_Served_Cells_To_Modify_Item;
   modifyItem=&cellsToModify->list.array[0]->value.choice.Served_Cells_To_Modify_Item;

   if(fillServCellToModItem(modifyItem))
      return RFAILED;
   else
      return ROK;
}

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
uint8_t BuildAndSendDUConfigUpdate()
{
   uint8_t ret, ieIdx, elementCnt;
   F1AP_PDU_t                 *f1apDuCfg = NULLP;
   GNBDUConfigurationUpdate_t *duCfgUpdate = NULLP;
   asn_enc_rval_t encRetVal;     /* Encoder return value */
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));
   ret= RFAILED;

   while(true)
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
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 DU_ALLOC(duCfgUpdate->protocolIEs.list.array[ieIdx],sizeof(GNBDUConfigurationUpdateIEs_t));
	 if(duCfgUpdate->protocolIEs.list.array[ieIdx] == NULLP)
	 {
	    break;
	 }
      }

      /*TransactionID*/
      ieIdx = 0;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->id=ProtocolIE_ID_id_TransactionID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality= Criticality_reject;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
         GNBDUConfigurationUpdateIEs__value_PR_TransactionID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = TRANS_ID;

      /*Served Cell to Modify */
      ieIdx++;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->id = \
						      ProtocolIE_ID_id_Served_Cells_To_Modify_List;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality =Criticality_reject;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
	 GNBDUConfigurationUpdateIEs__value_PR_Served_Cells_To_Modify_List;
      if(buildServCellToModList(&duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.\
	 Served_Cells_To_Modify_List))
         break;

      // NOTE :GNB DU SYS INFO:MIB AND SIB1 INFORMATION TO BE BUILT AND FILLED HERE
      /*GNB DU ID */
      ieIdx++;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_ID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
	GNBDUConfigurationUpdateIEs__value_PR_GNB_DU_ID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size = sizeof(uint8_t);
      DU_ALLOC(duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf,\
	    duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size);
      if(duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf == NULLP)
      {
	 break;
      }
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf[0] = duCfgParam.duId;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apDuCfg);

      /* Encode the DU Config Update type as APER */
      memset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
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
	 for(ieIdx =0; ieIdx < encBufSize; ieIdx++)
	 {
	    printf("%x",encBuf[ieIdx]);
	 }
      }
      /* Sending msg */
      if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
      {
	 DU_LOG("\nF1AP : Sending GND-DU Config Update failed");
	 break;
      }

      ret = ROK;
      break;
   }
   FreeDUConfigUpdate(f1apDuCfg);

   return ret;
}


/*******************************************************************
 *
 * @brief free the ULRRCMessageTransfer
 *
 * @details
 *
 *    Function : FreeULRRCMessageTransfer
 *
 *    Functionality: Deallocating the memory of variable allocated in
 *                      FreeULRRCMessageTransfer
 *
 * @params[in]
 *
 * @return ROK     - void
 *
 ******************************************************************/
void FreeULRRCMessageTransfer( F1AP_PDU_t *f1apMsg)
{
   uint8_t idx1;
   ULRRCMessageTransfer_t  *ulRRCMsg;

   if(f1apMsg != NULLP)
   { 
      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
	 ulRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;
	 if(ulRRCMsg->protocolIEs.list.array != NULLP)
	 {
	    for(idx1=0;idx1<ulRRCMsg->protocolIEs.list.count;idx1++)
	    {
	       if(ulRRCMsg->protocolIEs.list.array[idx1] != NULLP)
	       {
		  if(ulRRCMsg->protocolIEs.list.array[idx1]->value.present ==
			ULRRCMessageTransferIEs__value_PR_RRCContainer)
		  {
		     DU_FREE(ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf,
			   ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size);
		  }
		  DU_FREE(ulRRCMsg->protocolIEs.list.array[idx1],sizeof(ULRRCMessageTransferIEs_t));
	       }
	    }
	    DU_FREE(ulRRCMsg->protocolIEs.list.array,ulRRCMsg->protocolIEs.list.size );
	 }
	 DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
   }
}
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
uint8_t BuildAndSendULRRCMessageTransfer(DuUeCb  ueCb, uint8_t lcId, \
      uint16_t msgLen, uint8_t *rrcMsg)
{
   uint8_t   elementCnt;
   uint8_t   idx1;
   uint8_t   idx;
   F1AP_PDU_t      		*f1apMsg = NULL;
   ULRRCMessageTransfer_t	*ulRRCMsg;
   asn_enc_rval_t  		encRetVal;        /* Encoder return value */
   uint8_t ret =RFAILED;
   while(true)
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
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_ULRRCMessageTransfer;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
      f1apMsg->choice.initiatingMessage->value.present = \
							 InitiatingMessage__value_PR_ULRRCMessageTransfer;
      ulRRCMsg =
	 &f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;
      elementCnt = 4;
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
      ulRRCMsg->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.GNB_CU_UE_F1AP_ID = ueCb.gnbCuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx1++;
      ulRRCMsg->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.GNB_DU_UE_F1AP_ID = ueCb.gnbDuUeF1apId;

      /*SRBID*/
      idx1++;
      ulRRCMsg->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_SRBID;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_SRBID;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.SRBID = lcId;

      /*RRCContainer*/
      idx1++;
      ulRRCMsg->protocolIEs.list.array[idx1]->id  = ProtocolIE_ID_id_RRCContainer;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_RRCContainer;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size = msgLen;
      DU_ALLOC(ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf,
	    ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size)
	 if(!ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf)
	 {
	    DU_LOG(" F1AP : Memory allocation for BuildAndSendULRRCMessageTransfer failed");
	    break;
	 }
      memcpy(ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf, \
	    rrcMsg, ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size);

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
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
      ret = ROK;
      break;
   }
   FreeULRRCMessageTransfer(f1apMsg);

   return ret;
}/* End of BuildAndSendULRRCMessageTransfer*/

/*******************************************************************
 *
 * @brief Builds tag config 
 *
 * @details
 *
 *    Function : BuildTagConfig 
 *
 *    Functionality: Builds tag config in MacCellGroupConfig
 *
 * @params[in] TAG_Config *tag_Config
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildTagConfig(struct TAG_Config *tagConfig)
{
   struct TAG_Config__tag_ToAddModList *tagList;
   uint8_t                     idx, elementCnt;

   tagConfig->tag_ToReleaseList = NULLP;
   tagConfig->tag_ToAddModList = NULLP;
   DU_ALLOC(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
   if(!tagConfig->tag_ToAddModList)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildTagConfig");
      return RFAILED;
   }

   elementCnt = 1; //ODU_VALUE_ONE;
   tagList = tagConfig->tag_ToAddModList;
   tagList->list.count = elementCnt;
   tagList->list.size  =  elementCnt * sizeof(struct TAG *);

   tagList->list.array = NULLP;
   DU_ALLOC(tagList->list.array, tagList->list.size);
   if(!tagList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildTagConfig");
      return RFAILED;
   }

   for(idx=0; idx<tagList->list.count; idx++)
   {
      tagList->list.array[idx] = NULLP;
      DU_ALLOC(tagList->list.array[idx], sizeof(struct TAG));
      if(!tagList->list.array[idx])
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildTagConfig");
	 return RFAILED;
      }
   }

   idx = 0;
   tagList->list.array[idx]->tag_Id = TAG_ID;
   tagList->list.array[idx]->timeAlignmentTimer = TIME_ALIGNMENT_TMR;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PHR Config 
 *
 * @details
 *
 *    Function : BuildPhrConfig
 *
 *    Functionality: Builds phrConfig in MacCellGroupConfig
 *
 * @params[in] PHR Config *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPhrConfig(struct MAC_CellGroupConfig__phr_Config *phrConfig)
{

   phrConfig->present = MAC_CellGroupConfig__phr_Config_PR_setup;
   phrConfig->choice.setup = NULLP;
   DU_ALLOC(phrConfig->choice.setup, sizeof(struct PHR_Config));
   if(!phrConfig->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildPhrConfig");
      return RFAILED;
   }

   phrConfig->choice.setup->phr_PeriodicTimer        = PHR_PERIODIC_TMR;
   phrConfig->choice.setup->phr_ProhibitTimer        = PHR_PROHIBHIT_TMR;
   phrConfig->choice.setup->phr_Tx_PowerFactorChange = PHR_PWR_FACTOR_CHANGE;
   phrConfig->choice.setup->multiplePHR              = false;
   phrConfig->choice.setup->dummy                    = false;
   phrConfig->choice.setup->phr_Type2OtherCell       = false;
   phrConfig->choice.setup->phr_ModeOtherCG          = PHR_MODE_OTHER_CG;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BSR Config 
 *
 * @details
 *
 *    Function : BuildBsrConfig
 *
 *    Functionality: Builds BuildBsrConfig in MacCellGroupConfig
 *
 * @params[in] BSR_Config *bsrConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBsrConfig(struct BSR_Config *bsrConfig)
{
   bsrConfig->periodicBSR_Timer = PERIODIC_BSR_TMR;
   bsrConfig->retxBSR_Timer     = RETX_BSR_TMR;
   bsrConfig->logicalChannelSR_DelayTimer = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds scheduling request config 
 *
 * @details
 *
 *    Function : BuildSchedulingReqConfig 
 *
 *    Functionality: Builds BuildSchedulingReqConfig in MacCellGroupConfig
 *
 * @params[in] SchedulingRequestConfig *schedulingRequestConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSchedulingReqConfig(struct SchedulingRequestConfig *schedulingRequestConfig)
{
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList;
   uint8_t                     idx, elementCnt;

   schedulingRequestConfig->schedulingRequestToAddModList = NULLP;
   DU_ALLOC(schedulingRequestConfig->schedulingRequestToAddModList,
	 sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));
   if(!schedulingRequestConfig->schedulingRequestToAddModList)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }

   elementCnt = 1; //ODU_VALUE_ONE;
   schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
   schReqList->list.count = elementCnt;
   schReqList->list.size  = elementCnt * sizeof(struct SchedulingRequestToAddMod *);

   schReqList->list.array = NULLP;
   DU_ALLOC(schReqList->list.array, schReqList->list.size);
   if(!schReqList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }

   for(idx=0;idx<schReqList->list.count; idx++)
   {
      schReqList->list.array[idx] = NULLP;
      DU_ALLOC(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
      if(!schReqList->list.array[idx])
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
	 return RFAILED;
      }
   }

   idx = 0;
   schReqList->list.array[idx]->schedulingRequestId = SCH_REQ_ID;

   schReqList->list.array[idx]->sr_ProhibitTimer = NULLP;
   DU_ALLOC(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
   if(!schReqList->list.array[idx]->sr_ProhibitTimer)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }
   *(schReqList->list.array[idx]->sr_ProhibitTimer) = SR_PROHIBIT_TMR;
   schReqList->list.array[idx]->sr_TransMax = SR_TRANS_MAX;
   schedulingRequestConfig->schedulingRequestToReleaseList = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Config
 *
 * @details
 *
 *    Function : BuildRlcConfig
 *
 *    Functionality: Builds RLC Config in BuildRlcBearerToAddModList 
 *
 * @params[in] RLC_Config *rlcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcConfig(struct RLC_Config *rlcConfig)
{

   rlcConfig->present = RLC_Config_PR_am;

   rlcConfig->choice.am = NULLP;
   DU_ALLOC(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
   if(!rlcConfig->choice.am)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildRlcConfig");
      return RFAILED;
   }

   /* UL */
   rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
   if(!rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildRlcConfig");
      return RFAILED;
   }
   *(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength) = SN_FIELD_LEN;
   rlcConfig->choice.am->ul_AM_RLC.t_PollRetransmit  = T_POLL_RETRANSMIT;
   rlcConfig->choice.am->ul_AM_RLC.pollPDU           = POLL_PDU;
   rlcConfig->choice.am->ul_AM_RLC.pollByte          = POLL_BYTE;
   rlcConfig->choice.am->ul_AM_RLC.maxRetxThreshold  = MAX_RETX_THRESHOLD;

   /* DL */
   rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
   if(!rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildRlcConfig");
      return RFAILED;
   }
   *(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength) = SN_FIELD_LEN;
   rlcConfig->choice.am->dl_AM_RLC.t_Reassembly      = T_REASSEMBLY;
   rlcConfig->choice.am->dl_AM_RLC.t_StatusProhibit  = T_STATUS_PROHIBHIT;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds MAC LC Config
 *
 * @details
 *
 *    Function : BuildMacLCConfig 
 *
 *    Functionality: Builds MAC LC Config in BuildRlcBearerToAddModList 
 *
 * @params[in] struct LogicalChannelConfig macLcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildMacLCConfig(struct LogicalChannelConfig *macLcConfig)
{

   macLcConfig->ul_SpecificParameters = NULLP;
   DU_ALLOC(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
   if(!macLcConfig->ul_SpecificParameters)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }

   macLcConfig->ul_SpecificParameters->priority = MAC_LC_PRIORITY;
   macLcConfig->ul_SpecificParameters->prioritisedBitRate =	PRIORTISIED_BIT_RATE;
   macLcConfig->ul_SpecificParameters->bucketSizeDuration =	BUCKET_SIZE_DURATION;
   macLcConfig->ul_SpecificParameters->allowedServingCells = NULLP;
   macLcConfig->ul_SpecificParameters->allowedSCS_List = NULLP;
   macLcConfig->ul_SpecificParameters->maxPUSCH_Duration = NULLP;
   macLcConfig->ul_SpecificParameters->configuredGrantType1Allowed = NULLP;

   macLcConfig->ul_SpecificParameters->logicalChannelGroup = NULLP;
   DU_ALLOC(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
   if(!macLcConfig->ul_SpecificParameters->logicalChannelGroup)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }
   *(macLcConfig->ul_SpecificParameters->logicalChannelGroup) = LC_GRP;

   macLcConfig->ul_SpecificParameters->schedulingRequestID = NULLP;
   DU_ALLOC(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
   if(!macLcConfig->ul_SpecificParameters->schedulingRequestID)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }
   *(macLcConfig->ul_SpecificParameters->schedulingRequestID) = SCH_REQ_ID;

   macLcConfig->ul_SpecificParameters->logicalChannelSR_Mask = false;
   macLcConfig->ul_SpecificParameters->logicalChannelSR_DelayTimerApplied = false;
   macLcConfig->ul_SpecificParameters->bitRateQueryProhibitTimer = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Bearer to Add/Mod list
 *
 * @details
 *
 *    Function :BuildRlcBearerToAddModList 
 *
 *    Functionality: Builds RLC Bearer to Add/Mod list in DuToCuRrcContainer
 *
 * @params[in] rlc_BearerToAddModList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcBearerToAddModList(struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList)
{
   uint8_t                     idx, elementCnt;

   elementCnt = 1;
   rlcBearerList->list.count = elementCnt;
   rlcBearerList->list.size  = elementCnt * sizeof(struct RLC_BearerConfig *);

   rlcBearerList->list.array = NULLP;
   DU_ALLOC(rlcBearerList->list.array, rlcBearerList->list.size);
   if(!rlcBearerList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
      return RFAILED;
   }

   for(idx=0; idx<rlcBearerList->list.count; idx++)
   {
      rlcBearerList->list.array[idx] = NULLP;
      DU_ALLOC(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
      if(!rlcBearerList->list.array[idx])
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
	 return RFAILED;
      }
   }

   idx = 0;
   rlcBearerList->list.array[idx]->logicalChannelIdentity = SRB1_LCID;

   DU_ALLOC(rlcBearerList->list.array[idx]->servedRadioBearer, \
      sizeof(struct RLC_BearerConfig__servedRadioBearer));
   if(!rlcBearerList->list.array[idx]->servedRadioBearer)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
      return RFAILED;
   }

   rlcBearerList->list.array[idx]->servedRadioBearer->present = \
      RLC_BearerConfig__servedRadioBearer_PR_srb_Identity;
   rlcBearerList->list.array[idx]->servedRadioBearer->choice.srb_Identity = \
      SRB1_LCID;

   rlcBearerList->list.array[idx]->reestablishRLC = NULLP;
   rlcBearerList->list.array[idx]->rlc_Config = NULLP;
   DU_ALLOC(rlcBearerList->list.array[idx]->rlc_Config, sizeof(struct RLC_Config));
   if(!rlcBearerList->list.array[idx]->rlc_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
      return RFAILED;
   }

   if(BuildRlcConfig(rlcBearerList->list.array[idx]->rlc_Config) != ROK)
   {
      DU_LOG("\nF1AP : BuildRlcConfig failed");
      return RFAILED;
   }

   rlcBearerList->list.array[idx]->mac_LogicalChannelConfig = NULLP;
   DU_ALLOC(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, \
      sizeof(struct LogicalChannelConfig));
   if(!rlcBearerList->list.array[idx]->mac_LogicalChannelConfig)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildRlcBearerToAddModList");
      return RFAILED;
   }

   if(BuildMacLCConfig(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig) != ROK)
   {
      DU_LOG("\nF1AP : BuildMacLCConfig failed");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Build Control resource set to add/modify list 
 *
 * @details
 *
 *    Function : BuildControlRSetToAddModList
 *
 *    Functionality: Build Control resource set to add/modify list
 *
 * @params[in] 
 * struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildControlRSetToAddModList
(
 struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList
 )
{
   uint8_t idx;
   uint8_t elementCnt;
   uint8_t numBytes, bitsUnused;
   struct ControlResourceSet *controlRSet;
   uint8_t freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   uint8_t coreset0EndPrb, coreset1StartPrb, coreset1NumPrb;


   elementCnt = 1;
   controlRSetList->list.count = elementCnt;
   controlRSetList->list.size = \
				elementCnt * sizeof(struct ControlResourceSet *);

   controlRSetList->list.array = NULLP;
   DU_ALLOC(controlRSetList->list.array, controlRSetList->list.size);
   if(!controlRSetList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      controlRSetList->list.array[idx] = NULLP;
      DU_ALLOC(controlRSetList->list.array[idx], sizeof(struct ControlResourceSet));
      if(!controlRSetList->list.array[idx])
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
	 return RFAILED;
      }
   }

   idx=0;
   controlRSet = controlRSetList->list.array[idx];

   controlRSet->controlResourceSetId = PDCCH_CTRL_RSRC_SET_ONE_ID;

   /* size 6 bytes
    * 3 LSBs unsued
    * Bit string stored ff0000000000
    */
   numBytes = 6;
   bitsUnused = 3;
   controlRSet->frequencyDomainResources.size = numBytes * sizeof(uint8_t);

   controlRSet->frequencyDomainResources.buf = NULLP;
   DU_ALLOC(controlRSet->frequencyDomainResources.buf, \
	 controlRSet->frequencyDomainResources.size);
   if(!controlRSet->frequencyDomainResources.buf)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   memset(controlRSet->frequencyDomainResources.buf, 0, FREQ_DOM_RSRC_SIZE);
   coreset0EndPrb = CORESET0_END_PRB;
   coreset1StartPrb = coreset0EndPrb + 6;
   coreset1NumPrb = CORESET1_NUM_PRB;
   /* calculate the PRBs */
   freqDomRscAllocType0(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
   memcpy(controlRSet->frequencyDomainResources.buf, freqDomainResource, FREQ_DOM_RSRC_SIZE);
   controlRSet->frequencyDomainResources.bits_unused = bitsUnused;

   controlRSet->duration = PDCCH_CTRL_RSRC_SET_ONE_DURATION;
   controlRSet->cce_REG_MappingType.present = \
					      ControlResourceSet__cce_REG_MappingType_PR_nonInterleaved;

   controlRSet->precoderGranularity = PDCCH_CTRL_RSRC_SET_ONE_PRECOD_GRANULARITY;
   controlRSet->tci_StatesPDCCH_ToAddList = NULLP;
   controlRSet->tci_StatesPDCCH_ToReleaseList = NULLP;
   controlRSet->tci_PresentInDCI = NULLP;
#if 0
   uint8_t tciStateIdx;

   DU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList, \
	 sizeof(struct ControlResourceSet__tci_StatesPDCCH_ToAddList));
   if(!controlRset->tci_StatesPDCCH_ToAddList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   elementCnt = 1;
   controlRset->tci_StatesPDCCH_ToAddList->list.count = elementCnt;
   controlRset->tci_StatesPDCCH_ToAddList->list.size = elementCnt * sizeof(TCI_StateId_t *);
   DU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array, \
	 controlRset->tci_StatesPDCCH_ToAddList->list.size)
      if(!controlRset->tci_StatesPDCCH_ToAddList->list.array)
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
	 return RFAILED;
      }

   for(tciStateIdx = 0; tciStateIdx <elementCntl; tciStateIdx++)
   {
      DU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx], sizeof(TCI_StateId_t));
      if(!controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx])
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
	 return RFAILED;
      }
   }

   tciStateIdx = 0;
   /* TODO */
   *(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx]);

   DU_ALLOC(controlRset->tci_PresentInDCI, sizeof(long));
   if(!controlRset->tci_PresentInDCI)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }
   /* TODO */
   *(controlRset->tci_PresentInDCI);
#endif

   controlRSet->pdcch_DMRS_ScramblingID = NULLP;
   DU_ALLOC(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
   if(!controlRSet->pdcch_DMRS_ScramblingID)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }
   *(controlRSet->pdcch_DMRS_ScramblingID) = SCRAMBLING_ID;

   return ROK;
} /* End BuildControlRSetToAddModList */

/*******************************************************************
 *
 * @brief Build search space to add/modify list
 *
 * @details
 *
 *    Function : BuildSearchSpcToAddModList
 *
 *    Functionality: Build search space to add/modify list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildSearchSpcToAddModList
(
 struct PDCCH_Config__searchSpacesToAddModList *searchSpcList
 )
{
   uint8_t idx;
   uint8_t numBytes;
   uint8_t byteIdx;
   uint8_t bitsUnused;
   uint8_t elementCnt;
   struct SearchSpace *searchSpc;

   elementCnt = 1;
   searchSpcList->list.count = elementCnt;
   searchSpcList->list.size = elementCnt * sizeof(struct SearchSpace *);

   searchSpcList->list.array = NULLP;
   DU_ALLOC(searchSpcList->list.array, searchSpcList->list.size);
   if(!searchSpcList->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      searchSpcList->list.array[idx] = NULLP;
      DU_ALLOC(searchSpcList->list.array[idx], sizeof(struct SearchSpace));
      if(!searchSpcList->list.array[idx])
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
	 return RFAILED;
      }
   }

   idx = 0;
   searchSpc = searchSpcList->list.array[idx];

   searchSpc->searchSpaceId = PDCCH_SRCH_SPC_TWO_ID;

   searchSpc->controlResourceSetId = NULLP;
   DU_ALLOC(searchSpc->controlResourceSetId, sizeof(ControlResourceSetId_t));
   if(!searchSpc->controlResourceSetId)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }
   *(searchSpc->controlResourceSetId) = PDCCH_CTRL_RSRC_SET_ONE_ID;

   searchSpc->monitoringSlotPeriodicityAndOffset = NULLP;
   DU_ALLOC(searchSpc->monitoringSlotPeriodicityAndOffset, \
	 sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
   if(!searchSpc->monitoringSlotPeriodicityAndOffset)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }
   searchSpc->monitoringSlotPeriodicityAndOffset->present = \
							    SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1;

   searchSpc->duration = NULLP;
   searchSpc->monitoringSymbolsWithinSlot = NULLP;
   DU_ALLOC(searchSpc->monitoringSymbolsWithinSlot, sizeof(BIT_STRING_t));
   if(!searchSpc->monitoringSymbolsWithinSlot)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   /* Values taken from reference logs :
    * size 2 bytes
    * 2 LSBs unsued
    * Bit string stores 8000
    */
   numBytes = 2;
   bitsUnused = 2;

   searchSpc->monitoringSymbolsWithinSlot->size = numBytes * sizeof(uint8_t);
   searchSpc->monitoringSymbolsWithinSlot->buf = NULLP;
   DU_ALLOC(searchSpc->monitoringSymbolsWithinSlot->buf, \
	 searchSpc->monitoringSymbolsWithinSlot->size);
   if(!searchSpc->monitoringSymbolsWithinSlot->buf)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   byteIdx = 0;
   searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = \
							    PDCCH_SYMBOL_WITHIN_SLOT /* setting MSB to 128 i.e. 0x80 */;
   searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = 0;
   searchSpc->monitoringSymbolsWithinSlot->bits_unused = bitsUnused;

   searchSpc->nrofCandidates = NULLP;
   DU_ALLOC(searchSpc->nrofCandidates, sizeof(struct SearchSpace__nrofCandidates));
   if(!searchSpc->nrofCandidates)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   searchSpc->nrofCandidates->aggregationLevel1 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL1_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel2 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL2_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel4 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL4_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel8 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL8_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel16 = \
						   PDCCH_SRCH_SPC_TWO_AGG_LVL16_CANDIDATE;

   searchSpc->searchSpaceType = NULLP;
   DU_ALLOC(searchSpc->searchSpaceType, sizeof(struct SearchSpace__searchSpaceType));
   if(!searchSpc->searchSpaceType)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   searchSpc->searchSpaceType->present = SearchSpace__searchSpaceType_PR_ue_Specific;

   searchSpc->searchSpaceType->choice.ue_Specific = NULLP;
   DU_ALLOC(searchSpc->searchSpaceType->choice.ue_Specific, \
	 sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
   if(!searchSpc->searchSpaceType->choice.ue_Specific)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }  
   searchSpc->searchSpaceType->choice.ue_Specific->dci_Formats = \
								 PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT;

   return ROK;
}/* End BuildSearchSpcToAddModList */

/*******************************************************************
 *
 * @brief Builds BWP DL dedicated PDCCH config
 *
 * @details
 *
 *    Function : BuildBWPDlDedPdcchCfg
 *
 *    Functionality: Builds BWP DL dedicated PDCCH config
 *
 * @params[in] struct PDCCH_Config *pdcchCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPDlDedPdcchCfg(struct PDCCH_Config *pdcchCfg)
{
   pdcchCfg->controlResourceSetToAddModList = NULLP;
   DU_ALLOC(pdcchCfg->controlResourceSetToAddModList, \
	 sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
   if(!pdcchCfg->controlResourceSetToAddModList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
      return RFAILED;
   }

   if(BuildControlRSetToAddModList(pdcchCfg->controlResourceSetToAddModList) != ROK)
   {
      return RFAILED;
   }

   pdcchCfg->controlResourceSetToReleaseList = NULLP;

   pdcchCfg->searchSpacesToAddModList = NULLP;
   DU_ALLOC(pdcchCfg->searchSpacesToAddModList, \
	 sizeof(struct PDCCH_Config__searchSpacesToAddModList));
   if(!pdcchCfg->searchSpacesToAddModList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
      return RFAILED;
   }

   if(BuildSearchSpcToAddModList(pdcchCfg->searchSpacesToAddModList) != ROK)
   {
      return RFAILED;
   }

   pdcchCfg->searchSpacesToReleaseList = NULLP;
   pdcchCfg->downlinkPreemption = NULLP;
   pdcchCfg->tpc_PUSCH = NULLP;
   pdcchCfg->tpc_PUCCH = NULLP;
   pdcchCfg->tpc_SRS = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds DMRS DL PDSCH Mapping type A
 *
 * @details
 *
 *    Function : BuildDMRSDLPdschMapTypeA
 *
 *    Functionality: Builds DMRS DL PDSCH Mapping type A
 *
 * @params[in]
 * struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildDMRSDLPdschMapTypeA
(
 struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg
 )
{
   dmrsDlCfg->present = PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA_PR_setup;
   dmrsDlCfg->choice.setup = NULLP;
   DU_ALLOC(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
   if(!dmrsDlCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }

   dmrsDlCfg->choice.setup->dmrs_Type = NULLP;
   dmrsDlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   DU_ALLOC(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsDlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSDLPdschMapTypeA");
      return RFAILED;
   }
   *(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS;

   dmrsDlCfg->choice.setup->maxLength = NULLP;
   dmrsDlCfg->choice.setup->scramblingID0 = NULLP;
   dmrsDlCfg->choice.setup->scramblingID1 = NULLP;
   dmrsDlCfg->choice.setup->phaseTrackingRS = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds TCI states to add/modify list
 *
 * @details
 *
 *    Function : BuildTCIStatesToAddModList
 *
 *    Functionality:Builds TCI states to add/modify list
 *
 * @params[in] 
 * struct PDSCH_Config__tci_StatesToAddModList *tciStatesList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildTCIStatesToAddModList(struct PDSCH_Config__tci_StatesToAddModList *tciStatesList)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH time domain allocation list
 *
 * @details
 *
 *    Function : BuildPdschTimeDomAllocList
 *
 *    Functionality: Builds PDSCH time domain allocation list
 *
 * @params[in] 
 * struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildPdschTimeDomAllocList
(
 struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 )
{
   uint8_t idx;
   uint8_t elementCnt;
   struct PDSCH_TimeDomainResourceAllocation *timeDomAlloc;

   timeDomAllocList->present = \
			       PDSCH_Config__pdsch_TimeDomainAllocationList_PR_setup;

   timeDomAllocList->choice.setup = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup, \
	 sizeof(struct PDSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
      return RFAILED;
   }

   elementCnt = 1;
   timeDomAllocList->choice.setup->list.count = elementCnt;
   timeDomAllocList->choice.setup->list.size = \
					       elementCnt * sizeof(struct PDSCH_TimeDomainResourceAllocation *);

   timeDomAllocList->choice.setup->list.array = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup->list.array, \
	 timeDomAllocList->choice.setup->list.size);
   if(!timeDomAllocList->choice.setup->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      timeDomAllocList->choice.setup->list.array[idx] = NULLP;
      DU_ALLOC(timeDomAllocList->choice.setup->list.array[idx], \
	    sizeof(struct PDSCH_TimeDomainResourceAllocation));
      if(!timeDomAllocList->choice.setup->list.array[idx])
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
	 return RFAILED;
      }
   }

   idx = 0;
   timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];

   timeDomAlloc->k0 = NULLP;
   timeDomAlloc->mappingType = PDSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = \
					calcSliv(PDSCH_START_SYMBOL, PDSCH_LENGTH_SYMBOL);

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH PRB Bundling type
 *
 * @details
 *
 *    Function : BuildPdschPrbBundlingType
 *
 *    Functionality: Builds PDSCH PRB Bundling type
 *
 * @params[in] 
 * struct PDSCH_Config__prb_BundlingType *prbBndlType
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildPdschPrbBundlingType
(
 struct PDSCH_Config__prb_BundlingType *prbBndlType
 )
{
   prbBndlType->present = PDSCH_Config__prb_BundlingType_PR_staticBundling;

   prbBndlType->choice.staticBundling = NULLP;
   DU_ALLOC(prbBndlType->choice.staticBundling, \
	 sizeof(struct PDSCH_Config__prb_BundlingType__staticBundling));
   if(!prbBndlType->choice.staticBundling)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPdschPrbBundlingType");
      return RFAILED;
   }
   prbBndlType->choice.staticBundling->bundleSize = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP DL dedicated PDSCH config 
 *
 * @details
 *
 *    Function : BuildBWPDlDedPdschCfg
 *
 *    Functionality: Builds BWP DL dedicated PDSCH config
 *
 * @params[in] struct PDSCH_Config *pdschCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPDlDedPdschCfg(struct PDSCH_Config *pdschCfg)
{
   pdschCfg->dataScramblingIdentityPDSCH = NULLP;

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA = NULLP;
   DU_ALLOC(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, \
	 sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
   if(!pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }

   if(BuildDMRSDLPdschMapTypeA(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeB = NULLP;
   pdschCfg->tci_StatesToAddModList = NULLP;
   pdschCfg->tci_StatesToReleaseList = NULLP;
   pdschCfg->vrb_ToPRB_Interleaver = NULLP;
#if 0
   DU_ALLOC(pdschCfg->tci_StatesToAddModList, sizeof(struct PDSCH_Config__tci_StatesToAddModList));
   if(!pdschCfg->tci_StatesToAddModList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   if(BuildTCIStatesToAddModList(pdschCfg->tci_StatesToAddModList) != ROK)
   {
      return RFAILED;
   }
#endif

   pdschCfg->resourceAllocation = RES_ALLOC_TYPE;

   pdschCfg->pdsch_TimeDomainAllocationList = NULLP;
   DU_ALLOC(pdschCfg->pdsch_TimeDomainAllocationList, \
	 sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
   if(!pdschCfg->pdsch_TimeDomainAllocationList)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }

   if(BuildPdschTimeDomAllocList(pdschCfg->pdsch_TimeDomainAllocationList) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->pdsch_AggregationFactor = NULLP;
   pdschCfg->rateMatchPatternToAddModList = NULLP;
   pdschCfg->rateMatchPatternToReleaseList = NULLP;
   pdschCfg->rateMatchPatternGroup1 = NULLP;
   pdschCfg->rateMatchPatternGroup2 = NULLP;
   pdschCfg->rbg_Size = PDSCH_RBG_SIZE;
   pdschCfg->mcs_Table = NULLP;

   pdschCfg->maxNrofCodeWordsScheduledByDCI = NULLP;
   DU_ALLOC(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
   if(!pdschCfg->maxNrofCodeWordsScheduledByDCI)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   *(pdschCfg->maxNrofCodeWordsScheduledByDCI) = PDSCH_MAX_CODEWORD_SCH_BY_DCI;

   if(BuildPdschPrbBundlingType(&pdschCfg->prb_BundlingType) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->zp_CSI_RS_ResourceToAddModList = NULLP;
   pdschCfg->zp_CSI_RS_ResourceToReleaseList = NULLP;
   pdschCfg->aperiodic_ZP_CSI_RS_ResourceSetsToAddModList = NULLP;
   pdschCfg->aperiodic_ZP_CSI_RS_ResourceSetsToReleaseList = NULLP;
   pdschCfg->sp_ZP_CSI_RS_ResourceSetsToAddModList = NULLP;
   pdschCfg->sp_ZP_CSI_RS_ResourceSetsToReleaseList = NULLP;
   pdschCfg->p_ZP_CSI_RS_ResourceSet = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds intitial DL BWP
 * @details
 *
 *    Function : BuildInitialDlBWP 
 *
 *    Functionality: Builds intitial DL BWP in spCellCfgDed
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildInitialDlBWP(BWP_DownlinkDedicated_t *dlBwp)
{
   dlBwp->pdcch_Config = NULLP;
   DU_ALLOC(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
   if(!dlBwp->pdcch_Config)
   {
      DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   dlBwp->pdcch_Config->present = BWP_DownlinkDedicated__pdcch_Config_PR_setup; 

   dlBwp->pdcch_Config->choice.setup = NULLP;
   DU_ALLOC(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
   if(!dlBwp->pdcch_Config->choice.setup)
   {
      DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   if(BuildBWPDlDedPdcchCfg(dlBwp->pdcch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   dlBwp->pdsch_Config = NULLP;
   DU_ALLOC(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
   if(!dlBwp->pdsch_Config)
   {
      DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   dlBwp->pdsch_Config->present = BWP_DownlinkDedicated__pdsch_Config_PR_setup;

   dlBwp->pdsch_Config->choice.setup = NULLP;
   DU_ALLOC(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
   if(!dlBwp->pdsch_Config->choice.setup)
   {
      DU_LOG("\nF1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }

   if(BuildBWPDlDedPdschCfg(dlBwp->pdsch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   dlBwp->sps_Config = NULLP;
   dlBwp->radioLinkMonitoringConfig = NULLP; 
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds DMRS UL Pusch Mapping type A
 *
 * @details
 *
 *    Function : BuildDMRSULPuschMapTypeA
 *
 *    Functionality: Builds DMRS UL Pusch Mapping type A
 *
 * @params[in] 
 *    struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildDMRSULPuschMapTypeA
(
 struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg
 )
{
   dmrsUlCfg->present = PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup;
   dmrsUlCfg->choice.setup= NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup, sizeof(DMRS_UplinkConfig_t));
   if(!dmrsUlCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }

   dmrsUlCfg->choice.setup->dmrs_Type = NULLP;
   dmrsUlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   *(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS; 

   dmrsUlCfg->choice.setup->phaseTrackingRS = NULLP;
   dmrsUlCfg->choice.setup->maxLength = NULLP;
   dmrsUlCfg->choice.setup->transformPrecodingDisabled = NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled, \
	 sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }

   dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0 = NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
	 sizeof(long));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   *(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0) = SCRAMBLING_ID;

   dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID1 = NULLP;
   dmrsUlCfg->choice.setup->transformPrecodingEnabled = NULLP;
   return ROK;
}

/*******************************************************************
 *
 * @brief Build PUSCH time domain allocation list
 *
 * @details
 *
 *    Function : BuildPuschTimeDomAllocList
 *
 *    Functionality: Build PUSCH time domain allocation list
 *
 * @params[in] 
 * struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildPuschTimeDomAllocList
(
 struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList
 )
{
   uint8_t idx;
   uint8_t elementCnt;
   PUSCH_TimeDomainResourceAllocation_t  *timeDomAlloc;

   timeDomAllocList->present = PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup;
   timeDomAllocList->choice.setup = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup, \
	 sizeof(struct PUSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }

   elementCnt = 1;
   timeDomAllocList->choice.setup->list.count = elementCnt;
   timeDomAllocList->choice.setup->list.size = \
					       elementCnt * sizeof(PUSCH_TimeDomainResourceAllocation_t *);
   timeDomAllocList->choice.setup->list.array = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup->list.array, \
	 timeDomAllocList->choice.setup->list.size);
   if(!timeDomAllocList->choice.setup->list.array)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      timeDomAllocList->choice.setup->list.array[idx] = NULLP;
      DU_ALLOC(timeDomAllocList->choice.setup->list.array[idx],\
	    sizeof(PUSCH_TimeDomainResourceAllocation_t));
      if(!timeDomAllocList->choice.setup->list.array[idx])
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
	 return RFAILED;
      }
   }

   idx = 0;
   timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
   DU_ALLOC(timeDomAlloc->k2, sizeof(long));
   if(!timeDomAlloc->k2)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }
   *(timeDomAlloc->k2) = PUSCH_K2;
   timeDomAlloc->mappingType = PUSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = calcSliv(PUSCH_START_SYMBOL, PUSCH_LENGTH_SYMBOL);
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP UL dedicated PUSCH Config
 *
 * @details
 *
 *    Function : BuildBWPUlDedPuschCfg
 *
 *    Functionality:
 *      Builds BWP UL dedicated PUSCH Config
 *
 * @params[in] : PUSCH_Config_t *puschCfg
 *    
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPUlDedPuschCfg(PUSCH_Config_t *puschCfg)
{
   puschCfg->dataScramblingIdentityPUSCH = NULLP;
   DU_ALLOC(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
   if(!puschCfg->dataScramblingIdentityPUSCH)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   *(puschCfg->dataScramblingIdentityPUSCH) = SCRAMBLING_ID;

   puschCfg->txConfig = NULLP;
   puschCfg->dmrs_UplinkForPUSCH_MappingTypeA = NULLP;
   DU_ALLOC(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, \
	 sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
   if(!puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildDMRSULPuschMapTypeA(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   puschCfg->dmrs_UplinkForPUSCH_MappingTypeB = NULLP;
   puschCfg->pusch_PowerControl = NULLP;
   puschCfg->frequencyHopping = NULLP;
   puschCfg->frequencyHoppingOffsetLists = NULLP;
   puschCfg->resourceAllocation = RES_ALLOC_TYPE;

   puschCfg->pusch_TimeDomainAllocationList = NULLP;
   DU_ALLOC(puschCfg->pusch_TimeDomainAllocationList, \
	 sizeof(struct PUSCH_Config__pusch_TimeDomainAllocationList));
   if(!puschCfg->pusch_TimeDomainAllocationList)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildPuschTimeDomAllocList(puschCfg->pusch_TimeDomainAllocationList) != ROK)
   {
      return RFAILED;
   }

   puschCfg->pusch_AggregationFactor = NULLP;
   puschCfg->mcs_Table = NULLP;
   puschCfg->mcs_TableTransformPrecoder = NULLP;
   puschCfg->transformPrecoder = NULLP;
   DU_ALLOC(puschCfg->transformPrecoder, sizeof(long));
   if(!puschCfg->transformPrecoder)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   *(puschCfg->transformPrecoder) = PUSCH_TRANSFORM_PRECODER;

   puschCfg->codebookSubset = NULLP;
   puschCfg->maxRank = NULLP;
   puschCfg->rbg_Size = NULLP;
   puschCfg->uci_OnPUSCH = NULLP;
   puschCfg->tp_pi2BPSK = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills SRS resource to add/modify list 
 *
 * @details
 *
 *    Function : BuildSrsRsrcAddModList
 *
 *    Functionality: Fills SRS resource to add/modify list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSrsRsrcAddModList(struct SRS_Config__srs_ResourceToAddModList *resourceList)
{
   uint8_t   elementCnt;
   uint8_t   rsrcIdx;

   elementCnt = 1;
   resourceList->list.count = elementCnt;
   resourceList->list.size = elementCnt * sizeof(SRS_Resource_t *);
   resourceList->list.array = NULLP;
   DU_ALLOC(resourceList->list.array, resourceList->list.size);
   if(!resourceList->list.array)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }

   for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
   {
      DU_ALLOC(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
      if(!resourceList->list.array[rsrcIdx])
      {
	 DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
	 return RFAILED;
      }
   }

   rsrcIdx = 0;
   resourceList->list.array[rsrcIdx]->srs_ResourceId = SRS_RSRC_ID;
   resourceList->list.array[rsrcIdx]->nrofSRS_Ports = SRS_Resource__nrofSRS_Ports_port1;
   resourceList->list.array[rsrcIdx]->transmissionComb.present = SRS_Resource__transmissionComb_PR_n2;

   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2 = NULLP;
   DU_ALLOC(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2, \
	 sizeof(struct SRS_Resource__transmissionComb__n2));
   if(!resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->combOffset_n2\
      = SRS_COMB_OFFSET_N2;
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->cyclicShift_n2\
      = SRS_CYCLIC_SHIFT_N2;

   resourceList->list.array[rsrcIdx]->resourceMapping.startPosition = \
								      PUSCH_START_SYMBOL;
   resourceList->list.array[rsrcIdx]->resourceMapping.nrofSymbols =  \
								     SRS_Resource__resourceMapping__nrofSymbols_n1;
   resourceList->list.array[rsrcIdx]->resourceMapping.repetitionFactor = \
									 SRS_Resource__resourceMapping__repetitionFactor_n1;

   resourceList->list.array[rsrcIdx]->freqDomainPosition = SRS_FREQ_DOM_POS;
   resourceList->list.array[rsrcIdx]->freqDomainShift = SRS_FREQ_DOM_SHIFT;
   resourceList->list.array[rsrcIdx]->freqHopping.c_SRS = C_SRS;
   resourceList->list.array[rsrcIdx]->freqHopping.b_SRS = B_SRS;
   resourceList->list.array[rsrcIdx]->freqHopping.b_hop = B_HOP;
   resourceList->list.array[rsrcIdx]->groupOrSequenceHopping = \
							       SRS_Resource__groupOrSequenceHopping_neither;

   /* Setting resource type to aperiodic for intergration purposes */
   resourceList->list.array[rsrcIdx]->resourceType.present = \
							     SRS_Resource__resourceType_PR_aperiodic;
   resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic = NULLP;
   DU_ALLOC(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,
	 sizeof(struct SRS_Resource__resourceType__aperiodic));
   if(!resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }
   resourceList->list.array[rsrcIdx]->sequenceId = SRS_SEQ_ID;

   return ROK;
}

/*******************************************************************
 *
 * @brief Build SRS resource set Add/mod list
 *
 * @details
 *
 *    Function : BuildSrsRsrcSetAddModList
 *
 *    Functionality: Build SRS resource set Add/mod list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildSrsRsrcSetAddModList
(
 struct SRS_Config__srs_ResourceSetToAddModList *rsrcSetList
 )
{
   uint8_t  elementCnt;
   uint8_t  rSetIdx;
   uint8_t  rsrcIdx;
   struct SRS_ResourceSet__srs_ResourceIdList *rsrcIdList;

   elementCnt = 1;
   rsrcSetList->list.count = elementCnt;
   rsrcSetList->list.size = elementCnt * sizeof(SRS_ResourceSet_t *);
   rsrcSetList->list.array = NULLP;
   DU_ALLOC(rsrcSetList->list.array, rsrcSetList->list.size);
   if(!rsrcSetList->list.array)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
   {
      DU_ALLOC(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
      if(!rsrcSetList->list.array[rSetIdx])
      {
	 DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
	 return RFAILED;
      }
   }

   rSetIdx = 0;
   rsrcSetList->list.array[rSetIdx]->srs_ResourceSetId = SRS_RSET_ID;

   /* Fill Resource Id list in resource set */
   rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList = NULLP;
   DU_ALLOC(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
	 sizeof(struct SRS_ResourceSet__srs_ResourceIdList));
   if(!rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   elementCnt = 1;
   rsrcIdList = rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList;
   rsrcIdList->list.count = elementCnt;
   rsrcIdList->list.size = elementCnt * sizeof(SRS_ResourceId_t *);
   rsrcIdList->list.array = NULLP;
   DU_ALLOC(rsrcIdList->list.array, rsrcIdList->list.size);
   if(!rsrcIdList->list.array)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count; rsrcIdx++)
   {
      DU_ALLOC(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
      if(!rsrcIdList->list.array[rsrcIdx])
      {
	 DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
	 return RFAILED;
      }
   }

   rsrcIdx = 0;
   *rsrcIdList->list.array[rsrcIdx] = SRS_RSRC_ID;

   /* Fill resource type */
   rsrcSetList->list.array[rSetIdx]->resourceType.present = \
							    SRS_ResourceSet__resourceType_PR_aperiodic;

   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic = NULLP;
   DU_ALLOC(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
	 sizeof(struct SRS_ResourceSet__resourceType__aperiodic));
   if(!rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->aperiodicSRS_ResourceTrigger \
      = APERIODIC_SRS_RESRC_TRIGGER;

   /* TODO : Fill values for below IEs as expected by Viavi */
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->csi_RS = NULLP;
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->slotOffset = NULLP;


   rsrcSetList->list.array[rSetIdx]->usage = SRS_ResourceSet__usage_codebook;
   rsrcSetList->list.array[rSetIdx]->alpha = NULLP;
   rsrcSetList->list.array[rSetIdx]->p0 = NULLP;
   rsrcSetList->list.array[rSetIdx]->pathlossReferenceRS = NULLP;
   rsrcSetList->list.array[rSetIdx]->srs_PowerControlAdjustmentStates = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP UL dedicated SRS Config
 *
 * @details
 *
 *    Function : BuildBWPUlDedSrsCfg
 *
 *    Functionality: Builds BWP UL dedicated SRS Config
 *
 * @params[in] SRS Config 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPUlDedSrsCfg(SRS_Config_t *srsCfg)
{
   srsCfg->srs_ResourceSetToReleaseList = NULLP;
   srsCfg->srs_ResourceSetToAddModList = NULLP;
   DU_ALLOC(srsCfg->srs_ResourceSetToAddModList, \
	 sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
   if(!srsCfg->srs_ResourceSetToAddModList)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedSrsCfg");
      return RFAILED;
   }
   if(BuildSrsRsrcSetAddModList(srsCfg->srs_ResourceSetToAddModList) != ROK)
   {
      return RFAILED;
   }

   srsCfg->srs_ResourceToReleaseList = NULLP;

   /* Resource to Add/Modify list */
   srsCfg->srs_ResourceToAddModList = NULLP;
   DU_ALLOC(srsCfg->srs_ResourceToAddModList, \
	 sizeof(struct SRS_Config__srs_ResourceToAddModList));
   if(!srsCfg->srs_ResourceToAddModList)
   {
      DU_LOG("\nF1AP: Memory allocation failed in BuildBWPUlDedSrsCfg");
      return RFAILED;
   }

   if(BuildSrsRsrcAddModList(srsCfg->srs_ResourceToAddModList) != ROK)
   {
      return RFAILED;
   }

   srsCfg->tpc_Accumulation = NULLP;

   return ROK;
}



/*******************************************************************
 *
 * @brief Builds Pusch Serving cell Config
 *
 * @details
 *
 *    Function : BuildPuschSrvCellCfg
 *
 *    Functionality: Builds Pusch Serving cell Config
 *
 * @params[in] struct UplinkConfig__pusch_ServingCellConfig *puschCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPuschSrvCellCfg(struct UplinkConfig__pusch_ServingCellConfig *puschCfg)
{
   puschCfg->present = UplinkConfig__pusch_ServingCellConfig_PR_setup;
   puschCfg->choice.setup = NULLP;
   DU_ALLOC(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
   if(!puschCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
   puschCfg->choice.setup->rateMatching = NULLP;
   puschCfg->choice.setup->xOverhead = NULLP;
   puschCfg->choice.setup->ext1 = NULLP;
   DU_ALLOC(puschCfg->choice.setup->ext1, sizeof(struct PUSCH_ServingCellConfig__ext1));
   if(!puschCfg->choice.setup->ext1)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->ext1->maxMIMO_Layers = NULLP;
   DU_ALLOC(puschCfg->choice.setup->ext1->maxMIMO_Layers, sizeof(long));
   if(!puschCfg->choice.setup->ext1->maxMIMO_Layers)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }
   *(puschCfg->choice.setup->ext1->maxMIMO_Layers) = PUSCH_MAX_MIMO_LAYERS;

   puschCfg->choice.setup->ext1->processingType2Enabled= NULLP;
   DU_ALLOC(puschCfg->choice.setup->ext1->processingType2Enabled,sizeof(BOOLEAN_t));
   if(!puschCfg->choice.setup->ext1->processingType2Enabled)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }
   *(puschCfg->choice.setup->ext1->processingType2Enabled) = PUSCH_PROCESS_TYPE2_ENABLED;
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds inital UL BWP
 *
 * @details
 *
 *    Function : BuildInitialUlBWP
 *
 *    Functionality: Builds initial UL BWP
 *
 * @params[in] BWP_UplinkDedicated_t *ulBwp
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildInitialUlBWP(BWP_UplinkDedicated_t *ulBwp)
{
   ulBwp->pucch_Config = NULLP;

   /* Fill BWP UL dedicated PUSCH config */
   ulBwp->pusch_Config = NULLP;
   DU_ALLOC(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));
   if(!ulBwp->pusch_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->pusch_Config->present = BWP_UplinkDedicated__pusch_Config_PR_setup;
   ulBwp->pusch_Config->choice.setup = NULLP;
   DU_ALLOC(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
   if(!ulBwp->pusch_Config->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedPuschCfg(ulBwp->pusch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   ulBwp->configuredGrantConfig = NULLP;

   /* Fill BPW UL dedicated SRS config */
   ulBwp->srs_Config = NULLP;
   DU_ALLOC(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
   if(!ulBwp->srs_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->srs_Config->present = BWP_UplinkDedicated__srs_Config_PR_setup;
   ulBwp->srs_Config->choice.setup = NULLP;
   DU_ALLOC(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
   if(!ulBwp->srs_Config->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedSrsCfg(ulBwp->srs_Config->choice.setup) != ROK)
   {
      return RFAILED;   
   }

   ulBwp->beamFailureRecoveryConfig = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds UL config
 * @details
 *
 *    Function : BuildUlCfg 
 *
 *    Functionality: Builds UL config in spCellCfgDed
 *
 * @params[in] UplinkConfig_t *ulCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildUlCfg(UplinkConfig_t *ulCfg)
{
   ulCfg->initialUplinkBWP = NULLP;
   DU_ALLOC(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
   if(!ulCfg->initialUplinkBWP)
   {
      DU_LOG("\nF1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }

   if(BuildInitialUlBWP(ulCfg->initialUplinkBWP) != ROK)
   {
      return RFAILED;
   }

   ulCfg->uplinkBWP_ToReleaseList = NULLP;
   ulCfg->uplinkBWP_ToAddModList = NULLP;
   ulCfg->firstActiveUplinkBWP_Id = NULLP;
   DU_ALLOC(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
   if(!ulCfg->firstActiveUplinkBWP_Id)
   {
      DU_LOG("\nF1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }
   *(ulCfg->firstActiveUplinkBWP_Id) = ACTIVE_UL_BWP_ID;

   ulCfg->pusch_ServingCellConfig = NULLP;
   DU_ALLOC(ulCfg->pusch_ServingCellConfig, \
	 sizeof(struct UplinkConfig__pusch_ServingCellConfig));
   if(!ulCfg->pusch_ServingCellConfig)
   {
      DU_LOG("\nF1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }

   if(BuildPuschSrvCellCfg(ulCfg->pusch_ServingCellConfig) != ROK)
   {
      return RFAILED;
   }

   ulCfg->carrierSwitching = NULLP;
   ulCfg->ext1 = NULLP;
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH serving cell config
 * @details
 *
 *    Function : BuildPdschSrvCellCfg
 *
 *    Functionality: Builds PDSCH serving cell config in spCellCfgDed
 *
 * @params[in] struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPdschSrvCellCfg(struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg)
{
   pdschCfg->present =  ServingCellConfig__pdsch_ServingCellConfig_PR_setup;
   pdschCfg->choice.setup = NULLP;
   DU_ALLOC(pdschCfg->choice.setup, sizeof( struct PDSCH_ServingCellConfig));
   if(!pdschCfg->choice.setup)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }

   pdschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
   pdschCfg->choice.setup->xOverhead = NULLP;
   pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH = NULLP;
   DU_ALLOC(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH, sizeof(long));
   if(!pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH)
   {
      DU_LOG("\nF1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }
   *(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH)= PDSCH_NUM_HARQ_PROC;
   pdschCfg->choice.setup->pucch_Cell = NULLP;
   pdschCfg->choice.setup->ext1 = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds CSI Meas config
 * @details
 *
 *    Function : BuildCsiMeasCfg 
 *
 *    Functionality: Builds CSI Meas config in spCellCfgDed
 *
 * @params[in] struct ServingCellConfig__csi_MeasConfig *csiMeasCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildCsiMeasCfg(struct ServingCellConfig__csi_MeasConfig *csiMeasCfg)
{

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Spcell config dedicated
 * @details
 *
 *    Function : BuildSpCellCfgDed
 *
 *    Functionality: Builds sp cell config dedicated in spCellCfg
 *
 * @params[in] ServingCellConfig_t srvCellCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSpCellCfgDed(ServingCellConfig_t *srvCellCfg)
{
   srvCellCfg->tdd_UL_DL_ConfigurationDedicated = NULLP;
   DU_ALLOC(srvCellCfg->tdd_UL_DL_ConfigurationDedicated, sizeof(TDD_UL_DL_ConfigDedicated_t));
   if(!srvCellCfg->tdd_UL_DL_ConfigurationDedicated)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   srvCellCfg->initialDownlinkBWP = NULLP;
   DU_ALLOC(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
   if(!srvCellCfg->initialDownlinkBWP)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildInitialDlBWP(srvCellCfg->initialDownlinkBWP) != ROK)
   {
      DU_LOG("\nF1AP : BuildInitialDlBWP failed");
      return RFAILED;
   }
   srvCellCfg->downlinkBWP_ToReleaseList = NULLP;
   srvCellCfg->downlinkBWP_ToAddModList = NULLP;

   srvCellCfg->firstActiveDownlinkBWP_Id = NULLP;
   DU_ALLOC(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));
   if(!srvCellCfg->firstActiveDownlinkBWP_Id)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }
   *(srvCellCfg->firstActiveDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;

   srvCellCfg->bwp_InactivityTimer = NULLP;

   srvCellCfg->defaultDownlinkBWP_Id = NULLP;
   DU_ALLOC(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));
   if(!srvCellCfg->defaultDownlinkBWP_Id)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }
   *(srvCellCfg->defaultDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;

   srvCellCfg->uplinkConfig = NULLP;
   DU_ALLOC(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));
   if(!srvCellCfg->uplinkConfig)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildUlCfg(srvCellCfg->uplinkConfig) != ROK)
   {
      DU_LOG("\nF1AP : BuildUlCfg failed");
      return RFAILED;
   }
   srvCellCfg->supplementaryUplink = NULLP;
   srvCellCfg->pdcch_ServingCellConfig = NULLP;

   srvCellCfg->pdsch_ServingCellConfig = NULLP;
   DU_ALLOC(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct	ServingCellConfig__pdsch_ServingCellConfig));
   if(!srvCellCfg->pdsch_ServingCellConfig)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildPdschSrvCellCfg(srvCellCfg->pdsch_ServingCellConfig) != ROK)
   {
      DU_LOG("\nF1AP : BuildPdschSrvCellCfg failed");
      return RFAILED;
   }

   srvCellCfg->csi_MeasConfig = NULLP;
#if 0
   DU_ALLOC(srvCellCfg->csi_MeasConfig, sizeof(struct	ServingCellConfig__csi_MeasConfig))
      if(!srvCellCfg->csi_MeasConfig)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfgDed");
	 return RFAILED;
      }

   if(BuildCsiMeasCfg(srvCellCfg->csi_MeasConfig) != ROK)
   {
      DU_LOG("\nF1AP : BuildCsiMeasCfg failed");
      return RFAILED;
   }
#endif
   srvCellCfg->sCellDeactivationTimer = NULLP;
   srvCellCfg->crossCarrierSchedulingConfig = NULLP;
   srvCellCfg->tag_Id = TAG_ID;
   srvCellCfg->dummy = NULLP;
   srvCellCfg->pathlossReferenceLinking = NULLP;
   srvCellCfg->servingCellMO = NULLP;
   srvCellCfg->ext1 = NULLP;

   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Spcell config 
 *
 * @details
 *
 *    Function : BuildSpCellCfg 
 *
 *    Functionality: Builds sp cell config in DuToCuRrcContainer
 *
 * @params[in] SpCellConfig_t spCellCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSpCellCfg(SpCellConfig_t *spCellCfg)
{

   spCellCfg->servCellIndex = NULLP;
   DU_ALLOC(spCellCfg->servCellIndex, sizeof(long));
   if(!spCellCfg->servCellIndex)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   *(spCellCfg->servCellIndex) = SERV_CELL_IDX;

   spCellCfg->reconfigurationWithSync = NULLP;
   spCellCfg->rlf_TimersAndConstants = NULLP;
   spCellCfg->rlmInSyncOutOfSyncThreshold = NULLP;
   DU_ALLOC(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));
   if(!spCellCfg->rlmInSyncOutOfSyncThreshold)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   *(spCellCfg->rlmInSyncOutOfSyncThreshold) = RLM_SYNC_OUT_SYNC_THRESHOLD;

   spCellCfg->spCellConfigDedicated = NULLP;
   DU_ALLOC(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
   if(!spCellCfg->spCellConfigDedicated)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   if(BuildSpCellCfgDed(spCellCfg->spCellConfigDedicated) != ROK)
   {
      DU_LOG("\nF1AP : BuildSpCellCfgDed failed");
      return RFAILED;
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Phy cell group config 
 *
 * @details
 *
 *    Function : BuildPhyCellGrpCfg 
 *
 *    Functionality: Builds Phy cell group config in DuToCuRrcContainer
 *
 * @params[in] PhysicalCellGroupConfig_t *phyCellGrpCfg 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPhyCellGrpCfg(PhysicalCellGroupConfig_t *phyCellGrpCfg)
{
   phyCellGrpCfg->harq_ACK_SpatialBundlingPUCCH = NULLP;
   phyCellGrpCfg->harq_ACK_SpatialBundlingPUSCH = NULLP;

   phyCellGrpCfg->p_NR_FR1 = NULLP;
   DU_ALLOC(phyCellGrpCfg->p_NR_FR1, sizeof(long));
   if(!phyCellGrpCfg->p_NR_FR1)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildPhyCellGrpCfg");
      return RFAILED;
   }
   *(phyCellGrpCfg->p_NR_FR1)             = P_NR_FR1;
   phyCellGrpCfg->pdsch_HARQ_ACK_Codebook = PDSCH_HARQ_ACK_CODEBOOK;
   phyCellGrpCfg->tpc_SRS_RNTI = NULLP;
   phyCellGrpCfg->tpc_PUCCH_RNTI = NULLP;
   phyCellGrpCfg->tpc_PUSCH_RNTI = NULLP;
   phyCellGrpCfg->sp_CSI_RNTI = NULLP;
   phyCellGrpCfg->cs_RNTI = NULLP;
   phyCellGrpCfg->ext1 = NULLP;
   phyCellGrpCfg->ext2 = NULLP;

   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Mac cell group config 
 *
 * @details
 *
 *    Function : BuildMacCellGrpCfg 
 *
 *    Functionality: Builds Mac cell group config in DuToCuRrcContainer
 *
 * @params[in] MAC_CellGroupConfig_t *macCellGrpCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildMacCellGrpCfg(MAC_CellGroupConfig_t *macCellGrpCfg)
{
   macCellGrpCfg->drx_Config = NULLP;
   macCellGrpCfg->schedulingRequestConfig = NULLP;
   DU_ALLOC(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
   if(!macCellGrpCfg->schedulingRequestConfig)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildSchedulingReqConfig(macCellGrpCfg->schedulingRequestConfig) != ROK)
   {
      DU_LOG("\nF1AP : BuildSchedulingReqConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->bsr_Config = NULLP;
   DU_ALLOC(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
   if(!macCellGrpCfg->bsr_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildBsrConfig(macCellGrpCfg->bsr_Config) != ROK)
   {
      DU_LOG("\nF1AP : BuildBsrConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->tag_Config = NULLP;
   DU_ALLOC(macCellGrpCfg->tag_Config, sizeof(struct TAG_Config));
   if(!macCellGrpCfg->tag_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildTagConfig(macCellGrpCfg->tag_Config) != ROK)
   {
      DU_LOG("\nF1AP : BuildTagConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->phr_Config = NULLP;
   DU_ALLOC(macCellGrpCfg->phr_Config, sizeof(struct MAC_CellGroupConfig__phr_Config));
   if(!macCellGrpCfg->phr_Config)
   {
      DU_LOG("\nF1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildPhrConfig(macCellGrpCfg->phr_Config) != ROK)
   {
      DU_LOG("\nF1AP : BuildPhrConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->skipUplinkTxDynamic = false;
   macCellGrpCfg->ext1 = NULLP;

   return ROK;
}
/*******************************************************************
 *
 * @brief Frees memeory allocated for SearchSpcToAddModList
 *
 * @details
 *
 *    Function : FreeSearchSpcToAddModList
 *
 *    Functionality: Deallocating memory of SearchSpcToAddModList
 *
 * @params[in] struct PDCCH_Config__searchSpacesToAddModList *searchSpcList
 *
 * @return void
 *
 4221 * ****************************************************************/
void FreeSearchSpcToAddModList(struct PDCCH_Config__searchSpacesToAddModList *searchSpcList)
{
   uint8_t idx1=0;
   uint8_t idx2=0;
   struct  SearchSpace *searchSpc=NULLP;

   if(searchSpcList->list.array)
   {
      if(searchSpcList->list.array[idx2])
      {
	 searchSpc = searchSpcList->list.array[idx2];
	 if(searchSpc->controlResourceSetId)
	 {
	    if(searchSpc->monitoringSlotPeriodicityAndOffset)
	    {
	       if(searchSpc->monitoringSymbolsWithinSlot)
	       {
		  if(searchSpc->monitoringSymbolsWithinSlot->buf)
		  {
		     if(searchSpc->nrofCandidates)
		     {
			if(searchSpc->searchSpaceType)
			{
			   DU_FREE(searchSpc->searchSpaceType->choice.ue_Specific,\
				 sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
			   DU_FREE(searchSpc->searchSpaceType, sizeof(struct
				    SearchSpace__searchSpaceType));
			}
			DU_FREE(searchSpc->nrofCandidates,
			      sizeof(struct SearchSpace__nrofCandidates));
		     }
		     DU_FREE(searchSpc->monitoringSymbolsWithinSlot->buf, \
			   searchSpc->monitoringSymbolsWithinSlot->size);
		  }
		  DU_FREE(searchSpc->monitoringSymbolsWithinSlot,
			sizeof(BIT_STRING_t));
	       }
	       DU_FREE(searchSpc->monitoringSlotPeriodicityAndOffset, \
		     sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
	    }
	    DU_FREE(searchSpc->controlResourceSetId,
		  sizeof(ControlResourceSetId_t));
	 }
      }
      for(idx1 = 0; idx1 < searchSpcList->list.count; idx1++)
      {
	 DU_FREE(searchSpcList->list.array[idx1],
	       sizeof(struct SearchSpace));
      }
      DU_FREE(searchSpcList->list.array,searchSpcList->list.size);
   }
}
/*******************************************************************
 *
 * @brief Frees memory allocated for PdschTimeDomAllocList
 *
 * @details
 *
 *    Function : FreePdschTimeDomAllocList
 *
 *    Functionality: Deallocating memory of PdschTimeDomAllocList
 *
 * @params[in] struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return void
 *
 4221 * ****************************************************************/
void FreePdschTimeDomAllocList( struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList)
{
   uint8_t idx1=0;

   if(timeDomAllocList->choice.setup)
   {
      if(timeDomAllocList->choice.setup->list.array)
      {
	 for(idx1 = 0; idx1 <timeDomAllocList->choice.setup->list.count ; idx1++)
	 {
	    DU_FREE(timeDomAllocList->choice.setup->list.array[idx1],
		  sizeof(struct PDSCH_TimeDomainResourceAllocation));
	 }
	 DU_FREE(timeDomAllocList->choice.setup->list.array, \
	       timeDomAllocList->choice.setup->list.size);
      }
      DU_FREE(timeDomAllocList->choice.setup,\
	    sizeof(struct PDSCH_TimeDomainResourceAllocationList));
   }
}
/*******************************************************************
 *
 * @brief Frees memory allocated for PuschTimeDomAllocList
 *
 *@details
 *
 *    Function : FreePuschTimeDomAllocList
 *
 *    Functionality: Deallocating memory of PuschTimeDomAllocList
 *
 * @params[in] PUSCH_Config_t *puschCfg
 *
 * @return void
 *
 4221 * ****************************************************************/
void FreePuschTimeDomAllocList(PUSCH_Config_t *puschCfg)
{
   uint8_t idx1=0;
   uint8_t idx2=0;
   struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList_t=NULLP;

   if(puschCfg->pusch_TimeDomainAllocationList)
   {
      timeDomAllocList_t=puschCfg->pusch_TimeDomainAllocationList;
      if(timeDomAllocList_t->choice.setup)
      {
	 if(timeDomAllocList_t->choice.setup->list.array)
	 {
	    DU_FREE(timeDomAllocList_t->choice.setup->list.array[idx2]->k2, sizeof(long));
	    for(idx1 = 0; idx1<timeDomAllocList_t->choice.setup->list.count; idx1++)
	    {
	       DU_FREE(timeDomAllocList_t->choice.setup->list.array[idx1],\
		     sizeof(PUSCH_TimeDomainResourceAllocation_t));
	    }
	    DU_FREE(timeDomAllocList_t->choice.setup->list.array, \
		  timeDomAllocList_t->choice.setup->list.size);
	 }
	 DU_FREE(timeDomAllocList_t->choice.setup, \
	       sizeof(struct PUSCH_TimeDomainResourceAllocationList));
      }
      DU_FREE(puschCfg->transformPrecoder, sizeof(long));
      DU_FREE(puschCfg->pusch_TimeDomainAllocationList, \
	    sizeof(struct PUSCH_Config__pusch_TimeDomainAllocationList));
   }

}
/*******************************************************************
 *
 * @brief Frees memory allocated for InitialUlBWP
 *
 * @details
 *
 *    Function : FreeInitialUlBWP
 *
 *    Functionality: Deallocating memory of InitialUlBWP
 *
 * @params[in] BWP_UplinkDedicated_t *ulBwp
 *
 * @return void
 *
 * ****************************************************************/
void FreeInitialUlBWP(BWP_UplinkDedicated_t *ulBwp)
{
   uint8_t  rSetIdx, rsrcIdx;
   SRS_Config_t   *srsCfg = NULLP;
   PUSCH_Config_t *puschCfg = NULLP;
   struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg = NULLP;
   struct SRS_Config__srs_ResourceSetToAddModList *rsrcSetList = NULLP;
   struct SRS_ResourceSet__srs_ResourceIdList *rsrcIdList = NULLP;
   struct SRS_Config__srs_ResourceToAddModList *resourceList = NULLP;

   if(ulBwp->pusch_Config)
   {
      if(ulBwp->pusch_Config->choice.setup)
      {
	 puschCfg=ulBwp->pusch_Config->choice.setup;
	 if(puschCfg->dataScramblingIdentityPUSCH)
	 {
	    if(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
	    {
	       FreePuschTimeDomAllocList(puschCfg);
	       dmrsUlCfg=puschCfg->dmrs_UplinkForPUSCH_MappingTypeA;
	       if(dmrsUlCfg->choice.setup)
	       {
		  if(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
		  {
		     if(dmrsUlCfg->choice.setup->transformPrecodingDisabled)
		     {
			DU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
			      sizeof(long));
			DU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled,
			      sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
		     }
		     DU_FREE(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition,
			   sizeof(long));
		  }
		  DU_FREE(dmrsUlCfg->choice.setup,sizeof(DMRS_UplinkConfig_t));
	       }
	       DU_FREE(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, \
		     sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
	    }
	    DU_FREE(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
	 }
	 DU_FREE(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
      }
      DU_FREE(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));

      /* Free SRS-Config */
      if(ulBwp->srs_Config)
      {
	 if(ulBwp->srs_Config->choice.setup)
	 {
	    srsCfg = ulBwp->srs_Config->choice.setup;

	    /* Free Resource Set to add/mod list */
	    if(srsCfg->srs_ResourceSetToAddModList)
	    {
	       rsrcSetList = srsCfg->srs_ResourceSetToAddModList;
	       if(rsrcSetList->list.array)
	       {
		  rSetIdx = 0;

		  /* Free SRS resource Id list in this SRS resource set */
		  if(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList)
		  {
		     rsrcIdList = rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList;

		     if(rsrcIdList->list.array)
		     {
			for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count; rsrcIdx++)
			{
			   DU_FREE(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
			}
			DU_FREE(rsrcIdList->list.array, rsrcIdList->list.size);
		     }
		     DU_FREE(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
			   sizeof(struct SRS_ResourceSet__srs_ResourceIdList));
		  }

		  /* Free resource type info for this SRS resource set */
		  DU_FREE(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
			sizeof(struct SRS_ResourceSet__resourceType__aperiodic));

		  /* Free memory for each resource set */
		  for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
		  {
		     DU_FREE(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
		  }
		  DU_FREE(rsrcSetList->list.array, rsrcSetList->list.size); 
	       }
	       DU_FREE(srsCfg->srs_ResourceSetToAddModList, \
		     sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
	    }

	    /* Free resource to add/modd list */
	    if(srsCfg->srs_ResourceToAddModList)
	    {
	       resourceList = srsCfg->srs_ResourceToAddModList;
	       if(resourceList->list.array)
	       {
		  rsrcIdx = 0;
		  DU_FREE(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2,\
			sizeof(struct SRS_Resource__transmissionComb__n2));
		  DU_FREE(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,\
			sizeof(struct SRS_Resource__resourceType__aperiodic));

		  for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
		  {
		     DU_FREE(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
		  }
		  DU_FREE(resourceList->list.array, resourceList->list.size);
	       }
	       DU_FREE(srsCfg->srs_ResourceToAddModList, \
		     sizeof(struct SRS_Config__srs_ResourceToAddModList));
	    }

	    DU_FREE(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
	 }
	 DU_FREE(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
      }
   }
}	
/*******************************************************************
 *
 * @brief Frees memory allocated for initialUplinkBWP
 *
 * @details
 *
 *    Function : FreeinitialUplinkBWP
 *
 *    Functionality: Deallocating memory of initialUplinkBWP
 *
 * @params[in] UplinkConfig_t *ulCfg
 *
 * @return void
 *         
 *
 * ****************************************************************/
void FreeinitialUplinkBWP(UplinkConfig_t *ulCfg)
{
   BWP_UplinkDedicated_t *ulBwp=NULLP; 
   struct UplinkConfig__pusch_ServingCellConfig *puschCfg=NULLP;

   if(ulCfg->initialUplinkBWP)
   {
      ulBwp=ulCfg->initialUplinkBWP;
      if(ulCfg->firstActiveUplinkBWP_Id)
      {
	 if(ulCfg->pusch_ServingCellConfig)
	 {
	    puschCfg=ulCfg->pusch_ServingCellConfig;
	    if(puschCfg->choice.setup)
	    {
	       if(puschCfg->choice.setup->ext1)
	       {
		  DU_FREE(puschCfg->choice.setup->ext1->\
			processingType2Enabled,sizeof(BOOLEAN_t));
		  DU_FREE(puschCfg->choice.setup->ext1->\
			maxMIMO_Layers,sizeof(long));
		  DU_FREE(puschCfg->choice.setup->ext1, \
			sizeof(struct PUSCH_ServingCellConfig__ext1));
	       }
	       DU_FREE(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
	    }
	    DU_FREE(ulCfg->pusch_ServingCellConfig, sizeof(struct UplinkConfig__pusch_ServingCellConfig));
	 }
	 DU_FREE(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
      }
      FreeInitialUlBWP(ulBwp);
      DU_FREE(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
   }
}
/*******************************************************************
 *
 * @brief Frees emmory allocated for BWPDlDedPdschCfg
 *
 * @details
 *
 *    Function : FreeBWPDlDedPdschCfg
 *
 *    Functionality: Deallocating memory of BWPDlDedPdschCfg
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return void
 *
 *
 * ****************************************************************/
void FreeBWPDlDedPdschCfg(BWP_DownlinkDedicated_t *dlBwp)
{
   struct PDSCH_Config *pdschCfg=NULLP;
   struct PDSCH_Config__prb_BundlingType *prbBndlType=NULLP;
   struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList=NULLP;
   struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg=NULLP;

   if(dlBwp->pdsch_Config->choice.setup)
   {
      pdschCfg=dlBwp->pdsch_Config->choice.setup;
      if(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
      {
	 if(pdschCfg->pdsch_TimeDomainAllocationList)
	 {
	    timeDomAllocList=pdschCfg->pdsch_TimeDomainAllocationList;
	    if(pdschCfg->maxNrofCodeWordsScheduledByDCI)
	    {
	       prbBndlType=&pdschCfg->prb_BundlingType;
	       DU_FREE(prbBndlType->choice.staticBundling,\
		     sizeof(struct PDSCH_Config__prb_BundlingType__staticBundling));
	       DU_FREE(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
	    }
	    FreePdschTimeDomAllocList(timeDomAllocList);
	    DU_FREE(pdschCfg->pdsch_TimeDomainAllocationList, \
		  sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
	 }
	 dmrsDlCfg=pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA;
	 if(dmrsDlCfg->choice.setup)
	 {
	    DU_FREE(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition,
		  sizeof(long));
	    DU_FREE(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
	 }
	 DU_FREE(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, \
	       sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
      }
      DU_FREE(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
   }
}
/*******************************************************************
 *
 * @brief Frees emmory allocated for BWPDlDedPdcchCfg
 *
 * @details
 *
 *    Function : FreeBWPDlDedPdcchCfg
 *
 *    Functionality: Deallocating memory of BWPDlDedPdcchCfg
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return void
 *         
 *
 * ****************************************************************/
void FreeBWPDlDedPdcchCfg(BWP_DownlinkDedicated_t *dlBwp)
{
   uint8_t idx1=0;
   uint8_t idx2=0;
   struct PDCCH_Config *pdcchCfg=NULLP;
   struct ControlResourceSet *controlRSet=NULLP;
   struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList=NULLP;

   if(dlBwp->pdcch_Config->choice.setup)
   {
      pdcchCfg=dlBwp->pdcch_Config->choice.setup;
      if(pdcchCfg->controlResourceSetToAddModList)
      {
	 controlRSetList = pdcchCfg->controlResourceSetToAddModList;
	 if(controlRSetList->list.array)
	 {
	    controlRSet = controlRSetList->list.array[idx2];
	    if(controlRSet)
	    {
	       if(controlRSet->frequencyDomainResources.buf)
	       {
		  if(controlRSet->pdcch_DMRS_ScramblingID)
		  {
		     if(pdcchCfg->searchSpacesToAddModList)
		     {
			FreeSearchSpcToAddModList(pdcchCfg->searchSpacesToAddModList);
			DU_FREE(pdcchCfg->searchSpacesToAddModList, \
			      sizeof(struct PDCCH_Config__searchSpacesToAddModList));
		     }
		     DU_FREE(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
		  }
		  DU_FREE(controlRSet->frequencyDomainResources.buf, \
			controlRSet->frequencyDomainResources.size);
	       }
	    }
	    for(idx1 = 0; idx1 <controlRSetList->list.count; idx1++)
	    {
	       DU_FREE(controlRSetList->list.array[idx1], sizeof(struct ControlResourceSet));
	    }
	    DU_FREE(controlRSetList->list.array, controlRSetList->list.size);
	 }
	 DU_FREE(pdcchCfg->controlResourceSetToAddModList, \
	       sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
      }
      DU_FREE(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
   }
}	

/*******************************************************************
 *
 * @brief Frees emmory allocated for DUToCURRCContainer 
 *
 * @details
 *
 *    Function : FreeMemDuToCuRrcCont
 *
 *    Functionality: Deallocating memory of DuToCuRrcContainer
 *
 * @params[in] DuToCuRRCContainer, DuToCuRRCContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t FreeMemDuToCuRrcCont(CellGroupConfigRrc_t *cellGrpCfg)
{
   uint8_t idx=0;
   SpCellConfig_t *spCellCfg=NULLP;
   ServingCellConfig_t *srvCellCfg=NULLP;
   BWP_DownlinkDedicated_t *dlBwp=NULLP;
   MAC_CellGroupConfig_t *macCellGrpCfg=NULLP;
   PhysicalCellGroupConfig_t *phyCellGrpCfg=NULLP;
   struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList=NULLP;
   struct RLC_Config *rlcConfig=NULLP;
   struct LogicalChannelConfig *macLcConfig=NULLP;
   struct SchedulingRequestConfig *schedulingRequestConfig=NULLP;
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList=NULLP;
   struct TAG_Config *tagConfig=NULLP;
   struct TAG_Config__tag_ToAddModList *tagList=NULLP;
   struct MAC_CellGroupConfig__phr_Config *phrConfig=NULLP;
   struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg=NULLP;

   rlcBearerList = cellGrpCfg->rlc_BearerToAddModList;
   if(rlcBearerList)
   {
      if(rlcBearerList->list.array)
      {
	 for(idx=0; idx<rlcBearerList->list.count; idx++)
	 {
	    if(rlcBearerList->list.array[idx])
	    {  
	       rlcConfig   = rlcBearerList->list.array[idx]->rlc_Config;
	       macLcConfig = rlcBearerList->list.array[idx]->mac_LogicalChannelConfig;
	       if(rlcConfig)
	       {
		  if(rlcConfig->choice.am)
		  {
		     DU_FREE(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
		     DU_FREE(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
		     DU_FREE(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
		  }	
		  DU_FREE(rlcConfig, sizeof(struct RLC_Config));
	       }
	       DU_FREE(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
	       if(macLcConfig)
	       {
		  if(macLcConfig->ul_SpecificParameters)
		  {
		     DU_FREE(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
		     DU_FREE(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
		     DU_FREE(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
		  }
		  DU_FREE(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
	       }
	       DU_FREE(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
	    }	
	 }
	 DU_FREE(rlcBearerList->list.array, rlcBearerList->list.size);
      }
      DU_FREE(cellGrpCfg->rlc_BearerToAddModList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
   }

   macCellGrpCfg = cellGrpCfg->mac_CellGroupConfig;
   if(macCellGrpCfg)
   {
      schedulingRequestConfig = macCellGrpCfg->schedulingRequestConfig; 
      if(schedulingRequestConfig)
      {
	 schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
	 if(schReqList)
	 {
	    if(schReqList->list.array)
	    {
	       for(idx=0;idx<schReqList->list.count; idx++)
	       {
		  if(schReqList->list.array[idx])
		  {
		     DU_FREE(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
		     DU_FREE(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
		  }
	       }
	       DU_FREE(schReqList->list.array, schReqList->list.size);
	    }
	    DU_FREE(schedulingRequestConfig->schedulingRequestToAddModList,\
		  sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));    }
	    DU_FREE(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
      }
      if(macCellGrpCfg->bsr_Config)
      {
	 DU_FREE(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
      }
      tagConfig = macCellGrpCfg->tag_Config;
      if(tagConfig)
      {
	 tagList = tagConfig->tag_ToAddModList;
	 if(tagList)
	 {
	    if(tagList->list.array)
	    {
	       for(idx=0; idx<tagList->list.count; idx++)
	       {
		  DU_FREE(tagList->list.array[idx], sizeof(struct TAG));
	       }
	       DU_FREE(tagList->list.array, tagList->list.size);
	    }
	    DU_FREE(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
	 }
	 DU_FREE(tagConfig, sizeof(struct TAG_Config));
      }

      phrConfig = macCellGrpCfg->phr_Config;
      if(phrConfig)
      {
	 DU_FREE(phrConfig->choice.setup, sizeof(struct PHR_Config));
	 DU_FREE(phrConfig, sizeof(struct MAC_CellGroupConfig__phr_Config));
      }

      DU_FREE(macCellGrpCfg, sizeof(MAC_CellGroupConfig_t));
   }

   phyCellGrpCfg = cellGrpCfg->physicalCellGroupConfig;
   if(phyCellGrpCfg)
   {
      DU_FREE(phyCellGrpCfg->p_NR_FR1, sizeof(long));
      DU_FREE(phyCellGrpCfg, sizeof(PhysicalCellGroupConfig_t));
   }

   spCellCfg = cellGrpCfg->spCellConfig;
   if(spCellCfg)
   {
      if(spCellCfg->servCellIndex)
      {
	 if(spCellCfg->rlmInSyncOutOfSyncThreshold)
	 {
	    if(spCellCfg->spCellConfigDedicated)
	    {
	       srvCellCfg = spCellCfg->spCellConfigDedicated;
	       if(srvCellCfg->tdd_UL_DL_ConfigurationDedicated)
	       {
	          if(srvCellCfg->initialDownlinkBWP)
	          {
		     dlBwp = srvCellCfg->initialDownlinkBWP;
		     if(srvCellCfg->firstActiveDownlinkBWP_Id)
		     {
		        if(srvCellCfg->defaultDownlinkBWP_Id)
		        {
			   if(srvCellCfg->uplinkConfig)
			   {
			      if(srvCellCfg->pdsch_ServingCellConfig)
			      {
			         pdschCfg= srvCellCfg->pdsch_ServingCellConfig;
			         if(pdschCfg->choice.setup)
			         {
				    DU_FREE(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH,sizeof(long));
				    DU_FREE(pdschCfg->choice.setup, sizeof( struct PDSCH_ServingCellConfig));
			         }
			         DU_FREE(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct
				       ServingCellConfig__pdsch_ServingCellConfig));
			      }  
			      FreeinitialUplinkBWP(srvCellCfg->uplinkConfig);
			      DU_FREE(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));	
			   }
			   DU_FREE(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));
		        }
		        DU_FREE(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));
		     }
		     if(dlBwp->pdcch_Config)
		     {
		        if(dlBwp->pdsch_Config)
		        {
			   FreeBWPDlDedPdschCfg(dlBwp);
			   DU_FREE(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
		        }
		        FreeBWPDlDedPdcchCfg(dlBwp);
		        DU_FREE(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
		    }
		    DU_FREE(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
	          }
		  DU_FREE(srvCellCfg->tdd_UL_DL_ConfigurationDedicated, sizeof(TDD_UL_DL_ConfigDedicated_t));
	       }
	       DU_FREE(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
	    }
	    DU_FREE(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));
	 }
	 DU_FREE(spCellCfg->servCellIndex, sizeof(long));
      }
      DU_FREE(spCellCfg,sizeof(SpCellConfig_t));
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds DU To CU RRC Container 
 *
 * @details
 *
 *    Function : BuildDuToCuRrcContainer 
 *
 *    Functionality: Builds DuToCuRrcContainer
 *
 * @params[in] idx, index in F1AP msg
 *             DuToCuRRCContainer, DuToCuRRCContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDuToCuRrcContainer(DUtoCURRCContainer_t *duToCuRrcContainer)
{
   uint8_t  ret = ROK;
   CellGroupConfigRrc_t  cellGrpCfg;
   asn_enc_rval_t        encRetVal;
   memset(&cellGrpCfg, 0, sizeof(CellGroupConfigRrc_t));
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));

   while(true)
   {
      cellGrpCfg.cellGroupId = CELL_GRP_ID;

      cellGrpCfg.rlc_BearerToAddModList = NULLP;
      DU_ALLOC(cellGrpCfg.rlc_BearerToAddModList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
      if(!cellGrpCfg.rlc_BearerToAddModList)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildDuToCuRrcContainer");
	 ret = RFAILED;
	 break;
      }
      if(BuildRlcBearerToAddModList(cellGrpCfg.rlc_BearerToAddModList) != ROK)
      {
	 DU_LOG("\nF1AP : BuildRlcBearerToAddModList failed");
	 ret = RFAILED;
	 break;
      }

      cellGrpCfg.rlc_BearerToReleaseList = NULLP;
      cellGrpCfg.mac_CellGroupConfig = NULLP;
      DU_ALLOC(cellGrpCfg.mac_CellGroupConfig, sizeof(MAC_CellGroupConfig_t));
      if(!cellGrpCfg.mac_CellGroupConfig)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildDuToCuRrcContainer");
	 ret = RFAILED;
	 break;
      }
      if(BuildMacCellGrpCfg(cellGrpCfg.mac_CellGroupConfig) != ROK)
      {
	 DU_LOG("\nF1AP : BuildMacCellGrpCfg failed");
	 ret = RFAILED;
	 break;
      }

      cellGrpCfg.physicalCellGroupConfig = NULLP;
      DU_ALLOC(cellGrpCfg.physicalCellGroupConfig, sizeof(PhysicalCellGroupConfig_t));
      if(!cellGrpCfg.physicalCellGroupConfig)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildDuToCuRrcContainer");
	 ret = RFAILED;
	 break;
      }
      if(BuildPhyCellGrpCfg(cellGrpCfg.physicalCellGroupConfig) != ROK)
      {
	 DU_LOG("\nF1AP : BuildPhyCellGrpCfg failed");
	 ret = RFAILED;
	 break;
      }

      cellGrpCfg.spCellConfig = NULLP;
      DU_ALLOC(cellGrpCfg.spCellConfig, sizeof(SpCellConfig_t));
      if(!cellGrpCfg.spCellConfig)
      {
	 DU_LOG("\nF1AP : Memory allocation failure in BuildDuToCuRrcContainer");
	 ret = RFAILED;
	 break;
      }
      if(BuildSpCellCfg(cellGrpCfg.spCellConfig) != ROK)
      {
	 DU_LOG("\nF1AP : BuildSpCellCfg failed");
	 ret = RFAILED;
	 break;
      }

      cellGrpCfg.sCellToAddModList = NULLP;
      cellGrpCfg.sCellToReleaseList = NULLP;
      cellGrpCfg.ext1 = NULLP;

      /* encode cellGrpCfg into duToCuRrcContainer */
      xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, &cellGrpCfg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_CellGroupConfigRrc, 0, &cellGrpCfg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG( "\n F1AP : Could not encode DuToCuRrcContainer (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 ret = RFAILED;
	 break;
      }
      else
      {
	 DU_LOG("\n F1AP : Created APER encoded buffer for DuToCuRrcContainer\n");
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
      }

      duToCuRrcContainer->size = encBufSize;
      DU_ALLOC(duToCuRrcContainer->buf, duToCuRrcContainer->size);
      if(!duToCuRrcContainer->buf)
      {
	 DU_LOG("\nF1AP : Memory allocation failed in BuildDuToCuRrcContainer");
	 ret = RFAILED;
	 break;
      }
      if(ret == ROK)
      {
         memcpy(duToCuRrcContainer->buf, encBuf, duToCuRrcContainer->size);
      }
      break;
   }
   FreeMemDuToCuRrcCont(&cellGrpCfg);
   return ret;
}

/*******************************************************************
 *
 * @brief Free memory allocated in InitialULRRCMessage
 *
 * @details
 *
 *    Function : freeInitUlRrcMsgTransfer
 *
 *    Functionality: Free memory allocated in InitialULRRCMessage
 *
 * @params[in]F1AP_PDU_t  *f1apMsg)
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t freeInitUlRrcMsgTransfer(F1AP_PDU_t  *f1apMsg)
{
   uint8_t ieIdx, arrIdx;
   InitialULRRCMessageTransfer_t *initULRRCMsg=NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
         initULRRCMsg = &f1apMsg->choice.initiatingMessage->value.\
	    choice.InitialULRRCMessageTransfer;
	 if(initULRRCMsg->protocolIEs.list.array)
	 {
	    for(ieIdx = 0; ieIdx < initULRRCMsg->protocolIEs.list.count; ieIdx++)
	    {
	       switch(initULRRCMsg->protocolIEs.list.array[ieIdx]->id)
	       {
	          case ProtocolIE_ID_id_NRCGI:
		  {
	             if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.pLMN_Identity.buf)
	             {
	                if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.nRCellIdentity.buf)
	                {
	                   DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.nRCellIdentity.buf,\
	              	   initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.nRCellIdentity.size);
	                }
	                DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.pLMN_Identity.buf,\
	              	initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.pLMN_Identity.size);
	             }
                     break;
		  }
		  case ProtocolIE_ID_id_RRCContainer:
	          {
	             if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf)
	             {
	                DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf,\
	              	initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size);
	             }
                     break;
		  }
		  case ProtocolIE_ID_id_DUtoCURRCContainer:
		  {
	             if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer.buf)
	             {
	                DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer.buf,
	              	initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer.size);
	             }
		     break;
		  }
		  default:
		     break;
	       }
	     }
	     for(arrIdx = 0; arrIdx < ieIdx; arrIdx++)
	     {
	        if(initULRRCMsg->protocolIEs.list.array[arrIdx])
		{
		   DU_FREE(initULRRCMsg->protocolIEs.list.array[arrIdx],\
		      sizeof(InitialULRRCMessageTransferIEs_t));
		}
	     }
	     DU_FREE(initULRRCMsg->protocolIEs.list.array, initULRRCMsg->protocolIEs.list.size);
	  }
	 DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
   else
   {
      DU_LOG("\nDU_APP : Recevied F1ap Pdu is null at freeInitUlRrcMsgTransfer()");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends the InitialULRRCMessage 
 *
 * @details
 *
 *    Function : BuildAndSendInitialRrcMsgTransfer 
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
uint8_t BuildAndSendInitialRrcMsgTransfer(uint32_t gnbDuUeF1apId, uint16_t crnti,
      uint16_t rrcContSize, uint8_t *rrcContainer)
{
   uint8_t   ret;
   uint8_t   elementCnt;
   uint8_t   ieIdx;
   asn_enc_rval_t  encRetVal;
   F1AP_PDU_t  *f1apMsg = NULLP;
   InitialULRRCMessageTransfer_t *initULRRCMsg = NULLP;
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));

   while(true)
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
      elementCnt = 5;
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
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 DU_ALLOC(initULRRCMsg->protocolIEs.list.array[ieIdx],\
	       sizeof(InitialULRRCMessageTransferIEs_t));
	 if(initULRRCMsg->protocolIEs.list.array[ieIdx] == NULLP)
	 {
	    break;
	 }
      }
      ieIdx = 0;
      /*GNB DU UE F1AP ID*/
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality  = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present = \
								  InitialULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID = gnbDuUeF1apId;


      /*NRCGI*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_NRCGI;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality  =Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present = \
								  InitialULRRCMessageTransferIEs__value_PR_NRCGI;

      ret =\
	   BuildNrcgi(&initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI);
      if(ret!=ROK)
      {
	 break;
      }

      /*CRNTI*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_C_RNTI;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present =\
								 InitialULRRCMessageTransferIEs__value_PR_C_RNTI;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.C_RNTI = crnti;

      /*RRCContainer*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_RRCContainer;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present =\
								 InitialULRRCMessageTransferIEs__value_PR_RRCContainer;

      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size = rrcContSize;
      DU_ALLOC(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf,
	    initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size)
      if(!initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf)
      {
         DU_LOG(" F1AP : Memory allocation for RRCSetupRequestMessageTransferIEs failed");
         break;
      
      }
      memcpy(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf, rrcContainer,
	    initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size);


      /*DUtoCURRCContainer*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DUtoCURRCContainer;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present =\
								 InitialULRRCMessageTransferIEs__value_PR_DUtoCURRCContainer;

      ret = BuildDuToCuRrcContainer(&initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer);
      if(ret != ROK)
      {
	 break;
      }

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG( "\n F1AP : Could not encode Initial UL RRC Message Transfer\
	       structure (at %s)\n",encRetVal.failed_type ? \
	       encRetVal.failed_type->name : "unknown");
	 ret = RFAILED;
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
	 ret = RFAILED;
	 break;
      }
      break;
   }
   freeInitUlRrcMsgTransfer(f1apMsg);
   return ret;
}/* End of BuildAndSendInitialRrcMsgTransfer*/

/*****  UE SETUP REQUEST *****/

/*******************************************************************
 *
 * @brief Free Qos And Snssai Drb Info
 *
 * @details
 *
 *    Function : freeDrbQosAndSnssaiInfo
 *
 *    Functionality: Free Qos And Snssai Drb Info
 *
 * @params[in] LcCfg *lcCfg,
 * @return void
 *
 * ****************************************************************/
void freeDrbQosAndSnssaiInfo(LcCfg *lcCfg)
{
   if(lcCfg->snssai)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->snssai, sizeof(Snssai));
   }
   if(lcCfg->drbQos)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->drbQos, sizeof(DrbQosInfo));
   }
}

/******************************************************************
*
* @brief Function to delete the RLC Lc cfg from UE APP DB
*
* @details
*
*  Function : freeRlcLcCfg
*
*  Functionality: Function to delete the RLC Lc cfg from UE APP DB
*
*
 *****************************************************************/

void freeRlcLcCfg(RlcBearerCfg *lcCfg)
{
   switch(lcCfg->rlcMode)
   {
      case RLC_AM :
      {
         if(lcCfg->u.amCfg)
	 {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.amCfg, sizeof(AmBearerCfg));
            lcCfg->u.amCfg = NULLP;
	 }
         break;
      }
      case RLC_UM_BI_DIRECTIONAL :
      {
         if(lcCfg->u.umBiDirCfg)
	 {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umBiDirCfg, sizeof(UmBiDirBearerCfg));
	    lcCfg->u.umBiDirCfg = NULLP;
         }
         break;
      }
      case RLC_UM_UNI_DIRECTIONAL_UL :
      {
         if(lcCfg->u.umUniDirUlCfg)
	 {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umUniDirUlCfg, sizeof(UmUniDirUlBearerCfg));
	    lcCfg->u.umUniDirUlCfg = NULLP;
	 }
         break;

      }
      case RLC_UM_UNI_DIRECTIONAL_DL :
      {
         if(lcCfg->u.umUniDirDlCfg)
	 {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umUniDirDlCfg, sizeof(UmUniDirDlBearerCfg));
	    lcCfg->u.umUniDirDlCfg = NULLP;
         }
         break;
      }
      default:
         DU_LOG("\nDU_APP: Invalid Rlc Mode %d at freeRlcLcCfg()", lcCfg->rlcMode);
	 break;
   }
   memset(lcCfg, 0, sizeof(LcCfg));
}
/*******************************************************************
 *
 * @brief Function to free MacLcCfg
 *
 * @details
 *
 *    Function : freeMacLcCfg
 *
 *    Functionality: Function to free MacLcCfg
 *
 * @params[in] LcCfg *lcCfg,
 * @return void
 *
 * ****************************************************************/

void  freeMacLcCfg(LcCfg *lcCfg)
{
    /* Deleting DRBQOS */
   if(lcCfg->drbQos)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->drbQos, sizeof(DrbQosInfo));
      lcCfg->drbQos = NULLP;
   }
   /* Deleting SNSSAI */
   if(lcCfg->snssai)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->snssai, sizeof(Snssai));
      lcCfg->snssai = NULLP;
   }
   memset(lcCfg, 0, sizeof(LcCfg));
}

/*******************************************************************
 *
 * @brief Function to free DuUeCfg
 *
 * @details
 *
 *    Function : freeDuUeCfg
 *
 *    Functionality: Function to free DuUeCfg
 *
 * @params[in] DuUeCfg *ueCfg
 * @return void
 *
 * ****************************************************************/
void freeDuUeCfg(DuUeCfg *ueCfg)
{
   uint8_t lcIdx;

   if(ueCfg->cellGrpCfg)
   {
      DU_FREE(ueCfg->cellGrpCfg, sizeof(CellGroupConfigRrc_t));
      ueCfg->cellGrpCfg = NULLP;
   }
   if(ueCfg->ambrCfg)
   {
      memset(ueCfg->ambrCfg, 0, sizeof(AmbrCfg));
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCfg->ambrCfg, sizeof(AmbrCfg));
   }
   for(lcIdx = 0; lcIdx < ueCfg->numRlcLcs; lcIdx++)
   {
      freeRlcLcCfg(&ueCfg->rlcLcCfg[lcIdx]);
   }
   for(lcIdx = 0; lcIdx < ueCfg->numMacLcs; lcIdx++)
   {
      freeMacLcCfg(&ueCfg->macLcCfg[lcIdx]);
   }
}

/*******************************************************************
 *
 * @brief Function to free UecontextSetupDb
 *
 * @details
 *
 *    Function : freeF1UeDb
 *
 *    Functionality: Function to free UecontextSetupDb
 *
 * @params[in] UecontextSetupDb *
 * @return void
 *
 * ****************************************************************/

void freeF1UeDb(F1UeContextSetupDb *f1UeDb)
{
   
   if(f1UeDb->dlRrcMsg)
   {
      if(f1UeDb->dlRrcMsg->rrcMsgPdu)
      {
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL,\
	    f1UeDb->dlRrcMsg->rrcMsgPdu, f1UeDb->dlRrcMsg->rrcMsgSize);
      }
      memset(f1UeDb->dlRrcMsg, 0, sizeof(F1DlRrcMsg));
   }
   freeDuUeCfg(&f1UeDb->duUeCfg);
   memset(f1UeDb, 0, sizeof(F1UeContextSetupDb));
   DU_FREE(f1UeDb, sizeof(F1UeContextSetupDb));
}

/*******************************************************************
 *
 * @brief Function to build Am cfg Info
 *
 * @details
 *
 *    Function : extractRlcAmCfg
 *
 *    Functionality: Function to build Am cfg Info
 *
 * @params[in] AmBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/

void extractRlcAmCfg(AmBearerCfg *amCfgToSet, struct RLC_Config__am *rlcAmCfg)
{
   if(rlcAmCfg)
   {
      /* UL AM */
      if(rlcAmCfg->dl_AM_RLC.sn_FieldLength)
      {
	 amCfgToSet->ulAmCfg.snLenUl = *(rlcAmCfg->dl_AM_RLC.sn_FieldLength);
	 amCfgToSet->ulAmCfg.reAssemTmr = rlcAmCfg->dl_AM_RLC.t_Reassembly;
	 amCfgToSet->ulAmCfg.statProhTmr = rlcAmCfg->dl_AM_RLC.t_StatusProhibit;
      }

      /* DL AM */
      if(rlcAmCfg->ul_AM_RLC.sn_FieldLength)
      {
	 amCfgToSet->dlAmCfg.snLenDl = *(rlcAmCfg->ul_AM_RLC.sn_FieldLength);
	 amCfgToSet->dlAmCfg.pollRetxTmr = rlcAmCfg->ul_AM_RLC.t_PollRetransmit;
	 amCfgToSet->dlAmCfg.pollPdu   = rlcAmCfg->ul_AM_RLC.pollPDU;
	 amCfgToSet->dlAmCfg.pollByte  = rlcAmCfg->ul_AM_RLC.pollByte;
	 amCfgToSet->dlAmCfg.maxRetxTh = rlcAmCfg->ul_AM_RLC.maxRetxThreshold;
      }
   }
}

/*******************************************************************
 *
 * @brief Function to build Um Bi Info
 *
 * @details
 *
 *    Function : extractRlcUmBiCfg
 *
 *    Functionality: Function to build Um Bi Info
 *
 * @params[in] UmBiDirBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/

void extractRlcUmBiCfg(UmBiDirBearerCfg *umBiCfgToSet, struct RLC_Config__um_Bi_Directional *rlcBiCfg)
{
   if(rlcBiCfg)
   {
      /* UL UM BI DIR Cfg */
      if(rlcBiCfg->dl_UM_RLC.sn_FieldLength)
      {
	 umBiCfgToSet->ulUmCfg.snLenUlUm  = *(rlcBiCfg->dl_UM_RLC.sn_FieldLength);
	 umBiCfgToSet->ulUmCfg.reAssemTmr = rlcBiCfg->dl_UM_RLC.t_Reassembly;
      }

      /* DL UM BI DIR Cfg */
      if(rlcBiCfg->ul_UM_RLC.sn_FieldLength)
	 umBiCfgToSet->dlUmCfg.snLenDlUm  = *(rlcBiCfg->ul_UM_RLC.sn_FieldLength);
   }
}

/*******************************************************************
 *
 * @brief Function to build Um Ul Info
 *
 * @details
 *
 *    Function : extractRlcUmUlCfg
 *
 *    Functionality: Function to build Um Ul Info
 *
 * @params[in] UmUniDirUlBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/

void extractRlcUmUlCfg(UmUniDirUlBearerCfg *umUlCfgToSet, struct RLC_Config__um_Uni_Directional_DL *umUlCfg)
{
   if(umUlCfg)
   {
      if(umUlCfg->dl_UM_RLC.sn_FieldLength)
      {
         umUlCfgToSet->ulUmCfg.snLenUlUm  = *(umUlCfg->dl_UM_RLC.sn_FieldLength);
         umUlCfgToSet->ulUmCfg.reAssemTmr = umUlCfg->dl_UM_RLC.t_Reassembly;
      }
   }
}

/*******************************************************************
 *
 * @brief Function to build Um Uni Dl Info
 *
 * @details
 *
 *    Function : extractRlcUmDlCfg
 *
 *    Functionality: Function to build Um Uni Dl Info
 *
 * @params[in] UmUniDirDlBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/
void extractRlcUmDlCfg(UmUniDirDlBearerCfg *umDlCfgToSet, struct RLC_Config__um_Uni_Directional_UL *umDlCfg)
{
   if(umDlCfg)
   {
      if(umDlCfg->ul_UM_RLC.sn_FieldLength)
         umDlCfgToSet->dlUmCfg.snLenDlUm  = *(umDlCfg->ul_UM_RLC.sn_FieldLength);
   }
}

/*******************************************************************
 *
 * @brief Function to extractRlcModeCfg
 *
 * @details
 *
 *    Function : extractRlcModeCfg
 *
 *    Functionality: Function to extractRlcModeCfg
 *
 * @params[in] RLC_Config_t *
 *             RlcBearerCfg *
 *             void  *    
 * @return ROK/RFAILED
 *
 * ****************************************************************/
void extractRlcModeCfg(uint8_t rlcMode, RlcBearerCfg *rlcDbCfg, RLC_Config_t *lcCfg)
{
   if(lcCfg)
   {
      switch(rlcMode)
      {
         case RLC_AM :
            {
               if(lcCfg->choice.am)
	       {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.amCfg, sizeof(AmBearerCfg));
		  if(rlcDbCfg->u.amCfg)
		     extractRlcAmCfg(rlcDbCfg->u.amCfg, lcCfg->choice.am);
	       }
               break;
            }
         case RLC_UM_BI_DIRECTIONAL :
            {
               if(lcCfg->choice.um_Bi_Directional)
	       {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.umBiDirCfg, sizeof(UmBiDirBearerCfg));
	          if(rlcDbCfg->u.umBiDirCfg)
                     extractRlcUmBiCfg(rlcDbCfg->u.umBiDirCfg, lcCfg->choice.um_Bi_Directional);
	       }
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_UL :
            {
               if(lcCfg->choice.um_Uni_Directional_DL)
               {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.umUniDirUlCfg, sizeof(UmUniDirUlBearerCfg));
	          if(rlcDbCfg->u.umUniDirUlCfg)
                     extractRlcUmUlCfg(rlcDbCfg->u.umUniDirUlCfg, lcCfg->choice.um_Uni_Directional_DL);
	       }
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_DL :
            {
               if(lcCfg->choice.um_Uni_Directional_UL)
               {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.umUniDirDlCfg, sizeof(UmUniDirDlBearerCfg));
	          if(rlcDbCfg->u.umUniDirDlCfg)
                     extractRlcUmDlCfg(rlcDbCfg->u.umUniDirDlCfg, lcCfg->choice.um_Uni_Directional_UL);
	       }
               break;
            }
         default:
            DU_LOG("\nDUAPP: Invalid rlcMode %d at extractRlcCfgToAddMod()", rlcMode);
            break;
      }
   }
}

/*******************************************************************
 *
 * @brief Function to extract extractUlLcCfg
 *
 * @details
 *
 *    Function : extractUlLcCfg
 *
 *    Functionality: Function to extract extractUlLcCfg
 *
 * @params[in] UlLcCfg *f1UlLcCfg, LogicalChannelConfig_t *ulLcCfg
 * @return void
 *
 * ****************************************************************/

void extractUlLcCfg(UlLcCfg *f1UlLcCfg, LogicalChannelConfig_t *ulLcCfg)
{
   if(ulLcCfg)
   {
      if(ulLcCfg->ul_SpecificParameters)
      {
         f1UlLcCfg->priority = \
            ulLcCfg->ul_SpecificParameters->priority;
      if(ulLcCfg->ul_SpecificParameters->logicalChannelGroup)
      {
         f1UlLcCfg->lcGroup = \
           *(ulLcCfg->ul_SpecificParameters->logicalChannelGroup);
      }
      if(ulLcCfg->ul_SpecificParameters->schedulingRequestID)
      {
         f1UlLcCfg->schReqId = \
           *(ulLcCfg->ul_SpecificParameters->schedulingRequestID);
      }
      f1UlLcCfg->pbr = \
         ulLcCfg->ul_SpecificParameters->prioritisedBitRate;
      f1UlLcCfg->bsd = \
         ulLcCfg->ul_SpecificParameters->bucketSizeDuration;
      }
   }
}

/*******************************************************************
 *
 * @brief Function to procRlcLcCfg
 *
 * @details
 *
 *    Function : procRlcLcCfg
 *
 *    Functionality: Function to procRlcLcCfg
 *
 * @params[in] rbId, lcId, rbType, rlcMod
 *             RLC_Config_t *, RlcBearerCfg * , 
 * @return void
 *
 * ****************************************************************/

void procRlcLcCfg(uint8_t rbId, uint8_t lcId, uint8_t rbType, uint8_t rlcMode,\
   uint8_t configType, RLC_Config_t *f1RlcCfg, RlcBearerCfg *lcCfg)
{

   lcCfg->rbId   = rbId;
   lcCfg->configType = configType;

   if(rbType == RB_TYPE_SRB)
   {
      lcCfg->rbType = RB_TYPE_SRB;
      lcCfg->lcId   = rbId;
      lcCfg->lcType = LCH_DCCH;
      lcCfg->rlcMode = RLC_AM;
   }
   else if(rbType == RB_TYPE_DRB)
   {
      lcCfg->rbType = RB_TYPE_DRB;
      lcCfg->lcId   = lcId;
      lcCfg->lcType = LCH_DTCH;
      lcCfg->rlcMode = rlcMode;
   }
   if(f1RlcCfg) /* rlc mode config recived */
   {
      extractRlcModeCfg(lcCfg->rlcMode, lcCfg, f1RlcCfg);
   }
}



/*******************************************************************
 *
 * @brief Fills DrbQos Info received by CU
 *
 * @details
 *
 *    Function : extractQosInfo
 *
 *    Functionality: Fills DrbQos Info received  by CU
 *
 * @params[in] DrbQosInfo *qosToAdd, 
 *             QoSFlowLevelQoSParameters_t *qosFlowCfg
 * @return void
 *
 * ****************************************************************/

void extractQosInfo(DrbQosInfo *qosToAdd, QoSFlowLevelQoSParameters_t *qosFlowCfg)
{
   qosToAdd->fiveQiType = qosFlowCfg->qoS_Characteristics.present;
   qosToAdd->u.nonDyn5Qi.fiveQi     =\
         qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI;
   if(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow)
   {
      qosToAdd->u.nonDyn5Qi.avgWindow = \
        *(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow);
   }
   qosToAdd->u.nonDyn5Qi.maxDataBurstVol = \
      *(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume);
   if(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->qoSPriorityLevel)
   {
      qosToAdd->u.nonDyn5Qi.priorLevel = \
         *(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->qoSPriorityLevel);
   }
   qosToAdd->ngRanRetPri.priorityLevel = \
      qosFlowCfg->nGRANallocationRetentionPriority.priorityLevel; 
   qosToAdd->ngRanRetPri.preEmptionCap = \
      qosFlowCfg->nGRANallocationRetentionPriority.pre_emptionCapability;
   qosToAdd->ngRanRetPri.preEmptionVul = \
      qosFlowCfg->nGRANallocationRetentionPriority.pre_emptionVulnerability;
   if(qosFlowCfg->gBR_QoS_Flow_Information)
   {
      memcpy(&qosToAdd->grbQosInfo.maxFlowBitRateDl, \
         qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateDownlink.buf, \
         qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateDownlink.size);
      memcpy(&qosToAdd->grbQosInfo.maxFlowBitRateUl, \
         qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateUplink.buf, \
         qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateUplink.size);
      memcpy(&qosToAdd->grbQosInfo.guarFlowBitRateDl,\
         qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateDownlink.buf, \
         qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateDownlink.size);
      memcpy(&qosToAdd->grbQosInfo.guarFlowBitRateUl,\
         qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateUplink.buf, \
         qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateUplink.size);
   }
   qosToAdd->pduSessionId = 0;
   qosToAdd->ulPduSessAggMaxBitRate = 0;
}

uint8_t extractDrbCfg(DRBs_ToBeSetup_Item_t *drbItem, LcCfg *macLcToAdd)
{
   DRB_Information_t *drbInfo = NULLP;

   if(drbItem->qoSInformation.present == QoSInformation_PR_choice_extension)
   {
      if(drbItem->qoSInformation.choice.choice_extension->value.present ==
	    QoSInformation_ExtIEs__value_PR_DRB_Information)
      {
	 drbInfo = &drbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information;

	 if(!macLcToAdd->drbQos)
	 {
	    DU_ALLOC_SHRABL_BUF(macLcToAdd->drbQos, sizeof(DrbQosInfo));
	    if(macLcToAdd->drbQos == NULLP)
	    {
	       DU_LOG("\n DUAPP:Memory failed at allocating DrbQos at extractDrbCfg()");
	       return RFAILED;
	    }

	 }
	 if(drbInfo->dRB_QoS.qoS_Characteristics.present == QoS_Characteristics_PR_non_Dynamic_5QI)
	 {
	    extractQosInfo(macLcToAdd->drbQos, &drbInfo->dRB_QoS);
            macLcToAdd->dlLcCfg.lcp = macLcToAdd->drbQos->ngRanRetPri.priorityLevel;
	 }
	 if(!macLcToAdd->snssai)
	 {
	    DU_ALLOC_SHRABL_BUF(macLcToAdd->snssai, sizeof(Snssai));
	    if(macLcToAdd->snssai == NULLP)
	    {
	       DU_LOG("\n DUAPP:Memory failed at allocating SNSSAI at extractDrbCfg()");
	       return RFAILED;
	    }
	 }
	 memcpy(&macLcToAdd->snssai->sst, drbInfo->sNSSAI.sST.buf, \
	       drbInfo->sNSSAI.sST.size);
	 if(drbInfo->sNSSAI.sD)
	 {
	    memcpy(macLcToAdd->snssai->sd, drbInfo->sNSSAI.sD->buf, \
		  drbInfo->sNSSAI.sD->size);
	 }
      }/*End of DRB Info*/
   }
   return ROK;
}

uint8_t extractMacRbCfg(uint8_t lcId, DRBs_ToBeSetup_Item_t *drbCfg, LogicalChannelConfig_t *ulLcCfg, LcCfg *lcCfg)
{
   uint8_t ret = ROK;

   if(drbCfg)
   {
      ret = extractDrbCfg(drbCfg, lcCfg);
      if(ret == RFAILED)
      {
         DU_LOG("F1AP: Failed to build Drb Qos at extractMacRbCfg()");
	 return ret;
      }
   }
   else
   {
      lcCfg->drbQos = NULLP;
      lcCfg->snssai = NULLP;
      if(lcCfg->lcId == SRB2_LCID)
         lcCfg->dlLcCfg.lcp = LC_PRIORITY_3;
      else
         lcCfg->dlLcCfg.lcp = LC_PRIORITY_1;
   }
   if(ulLcCfg)
   {
      lcCfg->ulLcCfgPres = true;
      extractUlLcCfg(&lcCfg->ulLcCfg, ulLcCfg);
   }
   else
      lcCfg->ulLcCfgPres = false;
   return ret;
}

uint8_t procMacLcCfg(uint8_t lcId, uint8_t rbType, uint8_t configType,\
   DRBs_ToBeSetup_Item_t *drbItem, LogicalChannelConfig_t *ulLcCfg, LcCfg *lcCfg)
{
   uint8_t ret = ROK;

   lcCfg->lcId = lcId;
   lcCfg->configType = configType;
   if(rbType == RB_TYPE_SRB)
   {
      ret = extractMacRbCfg(lcId, NULL, ulLcCfg, lcCfg);
   }
   else if(rbType == RB_TYPE_DRB)
   {
      ret = extractMacRbCfg(lcId, drbItem, ulLcCfg, lcCfg);
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Function to extract Rlc cfg To Add/Mod in CellGrp Info
 *
 * @details
 *
 *    Function : extractRlcCfgToAddMod
 *
 *    Functionality: Function to extract Rlc cfg To Add/Mod in CellGrp Info
 *
 * @params[in] CellGroupConfigRrc__rlc_BearerTo pointer
 *             DuUeCfg Pointer
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractRlcCfgToAddMod(struct CellGroupConfigRrc__rlc_BearerToAddModList *lcCfg, \
   DuUeCfg *ueCfgDb)
{
  uint8_t ret, idx, rbId, lcId, rlcMode, rbType;
  RLC_Config_t *f1RlcCfg = NULLP;
  LogicalChannelConfig_t *macUlLcCfg = NULLP;

  for(idx = 0; idx < lcCfg->list.count; idx++)
  {
     lcId = lcCfg->list.array[idx]->logicalChannelIdentity;
     if(lcCfg->list.array[idx]->servedRadioBearer)
     {
        /* RadioBearer for SRB/DRB */
        if(lcCfg->list.array[idx]->servedRadioBearer->present ==\
        RLC_BearerConfig__servedRadioBearer_PR_srb_Identity)
        {
           rbId = lcCfg->list.array[idx]->servedRadioBearer->choice.srb_Identity;
           rbType = RB_TYPE_SRB;
        }
        else if(lcCfg->list.array[idx]->servedRadioBearer->present ==\
        RLC_BearerConfig__servedRadioBearer_PR_drb_Identity)
        {
           rbId = lcCfg->list.array[idx]->servedRadioBearer->choice.drb_Identity;
           rbType = RB_TYPE_DRB;
        }
        else
        {
           DU_LOG("\n No components present in Bearer config to ADD/MOD");
           return RFAILED;
        }
        /* MAC UL LC Config */
        if(lcCfg->list.array[idx]->mac_LogicalChannelConfig)
        {
           macUlLcCfg = lcCfg->list.array[idx]->mac_LogicalChannelConfig;
        }
     }
     else
     {
        DU_LOG("\nDUAPP: Received RadioBearer config is NULL");
        return RFAILED;
     }
     /* RLC Mode Config */
     if(lcCfg->list.array[idx]->rlc_Config)
     {
        rlcMode  = lcCfg->list.array[idx]->rlc_Config->present;
        f1RlcCfg = lcCfg->list.array[idx]->rlc_Config;
     }
     
     /* Filling RLC/MAC Config*/
     memset(&ueCfgDb->macLcCfg[idx], 0, sizeof(LcCfg));
     memset(&ueCfgDb->rlcLcCfg[idx], 0, sizeof(RlcBearerCfg));
     procRlcLcCfg(rbId, lcId, rbType, rlcMode, CONFIG_UNKNOWN, f1RlcCfg, &(ueCfgDb->rlcLcCfg[idx]));
     ret = procMacLcCfg(lcId, rbType, CONFIG_UNKNOWN, NULL, macUlLcCfg, &ueCfgDb->macLcCfg[idx]);
     if(ret == RFAILED)
     {
        DU_LOG("\nDUAPP: Failed while filling MAC LC config at extractRlcCfgToAddMod()");
        return ret;
     }
     (ueCfgDb->numRlcLcs)++;
     (ueCfgDb->numMacLcs)++;
  }
  //TODO: To send the failure cause in UeContextSetupRsp 
  return ret;
}

/*******************************************************************
 *
 * @brief DeAlloc pdsch serv cell config info
 *
 * @details
 *
 *    Function : freeMacPdschServCellInfo
 *
 *    Functionality: DeAlloc pdsch serv cell config info
 *
 * @params[in] PdschServCellCfg pointer
 * @return void
 *
 * ****************************************************************/

void freeMacPdschServCellInfo(PdschServCellCfg *pdsch)
{
   if(pdsch->xOverhead)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->xOverhead, sizeof(uint8_t));
   }
   if(pdsch->codeBlkGrpFlushInd)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->codeBlkGrpFlushInd, sizeof(bool));
   }
   if(pdsch->maxCodeBlkGrpPerTb)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->maxCodeBlkGrpPerTb, sizeof(uint8_t));
   }
   if(pdsch->maxMimoLayers)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->maxMimoLayers, sizeof(uint8_t));
   }
}

/*******************************************************************
 *
 * @brief Free Serving cell Info
 *
 * @details
 *
 *    Function : freeMacServingCellInfo
 *
 *    Functionality: Free Serving cell Info
 *
 * @params[in] ServCellCfgInfo *srvCellCfg
 * @return void
 *
 * ****************************************************************/
void freeMacServingCellInfo(ServCellCfgInfo *srvCellCfg)
{
   freeMacPdschServCellInfo(&srvCellCfg->pdschServCellCfg);
   if(srvCellCfg->bwpInactivityTmr)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, srvCellCfg->bwpInactivityTmr, sizeof(uint8_t));
   }
}

/*******************************************************************
 *
 * @brief Free cell Grp Cfg Info
 *
 * @details
 *
 *    Function : freeUeReCfgCellGrpInfo
 *
 *    Functionality: Free cell Grp Cfg Info
 *
 * @params[in] MacUeCfg*  duUeCfg
 * @return void
 *
 * ****************************************************************/

void freeUeReCfgCellGrpInfo(MacUeCfg *macUeCfg)
{
   freeMacServingCellInfo(&macUeCfg->spCellCfg.servCellCfg);
}

/*******************************************************************
 *
 * @brief Fills Reconfig SchReqReConfig
 *
 * @details
 *
 *    Function : extractSchReqReConfig
 *
 *    Functionality: Fills Reconfig SchReqReConfig
 *
 * @params[in] SchedulingRequestConfig_t *cuSchedReq
 *             SchedReqCfg*  macSchedReq
 * @return void
 *
 * ****************************************************************/
void extractSchReqReConfig(SchedulingRequestConfig_t *cuSchedReq, SchedReqCfg *macSchedReq)
{
   uint8_t schReqIdx = 0;
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqListToAdd = NULLP;
   struct SchedulingRequestConfig__schedulingRequestToReleaseList *schReqListToRel = NULLP;

   if(cuSchedReq->schedulingRequestToAddModList)
   {
      schReqListToAdd = cuSchedReq->schedulingRequestToAddModList;
      if(schReqListToAdd->list.count)
      {
	 macSchedReq->addModListCount = schReqListToAdd->list.count;
	 for(schReqIdx = 0; schReqIdx < schReqListToAdd->list.count; schReqIdx++)
	 {
	    macSchedReq->addModList[schReqIdx].schedReqId = \
	       schReqListToAdd->list.array[schReqIdx]->schedulingRequestId;
	    macSchedReq->addModList[schReqIdx].srProhibitTmr = \
	       *(schReqListToAdd->list.array[schReqIdx]->sr_ProhibitTimer);
	    macSchedReq->addModList[schReqIdx].srTransMax    =\
	       schReqListToAdd->list.array[schReqIdx]->sr_TransMax;
	 }
      }
   }
   /* Scheduling Req To release */
   if(cuSchedReq->schedulingRequestToReleaseList)
   {
      schReqListToRel = cuSchedReq->schedulingRequestToReleaseList;
      if(schReqListToRel->list.count)
      {
	 macSchedReq->relListCount = schReqListToRel->list.count;
	 for(schReqIdx = 0; schReqIdx < schReqListToRel->list.count; schReqIdx++)
	 {
	    macSchedReq->relList[schReqIdx] = \
	       *schReqListToRel->list.array[schReqIdx];
	 }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills TagReconfig
 *
 * @details
 *
 *    Function : extractTagReconfig
 *
 *    Functionality: Fills extractTagReconfig
 *
 * @params[in] TAG_Config_t *cuTagCfg
 *             TagCfg *macTagCfg
 * @return void
 *
 * ****************************************************************/

void extractTagReconfig(TAG_Config_t *cuTagCfg, TagCfg *macTagCfg)
{
  uint8_t tagIdx = 0;
  struct TAG_Config__tag_ToAddModList  *tagListToAddMod = NULLP;
  struct TAG_Config__tag_ToReleaseList *tagListToRel = NULLP;

  /* Tag config to AddMod */
  if(cuTagCfg->tag_ToAddModList)
  {
     tagListToAddMod = cuTagCfg->tag_ToAddModList; 
     if(tagListToAddMod->list.count)
     {
        macTagCfg->addModListCount = tagListToAddMod->list.count;
        for(tagIdx = 0; tagIdx < tagListToAddMod->list.count; tagIdx++)
	{
           macTagCfg->addModList[tagIdx].tagId =\
              tagListToAddMod->list.array[tagIdx]->tag_Id;     
           macTagCfg->addModList[tagIdx].timeAlignTimer = \

              tagListToAddMod->list.array[tagIdx]->timeAlignmentTimer;
	}
     }
  }
  /* Tag config to release */
  if(cuTagCfg->tag_ToReleaseList)
  {
     tagListToRel = cuTagCfg->tag_ToReleaseList;
     if(tagListToRel->list.count)
     {
        macTagCfg->relListCount = tagListToRel->list.count;
        for(tagIdx = 0; tagIdx < tagListToRel->list.count; tagIdx++)
	{
           macTagCfg->relList[tagIdx] = *tagListToRel->list.array[tagIdx];     
	}
     }
  }
}

/*******************************************************************
 *
 * @brief Fills PdcchCfg received by CU
 *
 * @details
 *
 *    Function : extractPdcchCfg
 *
 *    Functionality: Fills PdcchCfg received  by CU
 *
 * @params[in] PDCCH_Config_t *cuPdcchCfg,
 *             PdcchConfig *duPdcchCfg
 * @return void
 *
 * ****************************************************************/

void extractPdcchCfg(PDCCH_Config_t *cuPdcchCfg, PdcchConfig *macPdcchCfg)
{
   uint8_t cRsetIdx = 0;
   uint8_t srchSpcIdx = 0;

   struct PDCCH_Config__controlResourceSetToAddModList *cRsetToAddModList = NULLP;
   struct PDCCH_Config__controlResourceSetToReleaseList *cRsetToRelList = NULLP;
   struct PDCCH_Config__searchSpacesToAddModList *srchSpcToAddModList = NULLP;
   struct PDCCH_Config__searchSpacesToReleaseList *srchSpcToRelList = NULLP;


   /* Control Resource Set To Add/Mod List */
   if(cuPdcchCfg->controlResourceSetToAddModList)
   {
      cRsetToAddModList = cuPdcchCfg->controlResourceSetToAddModList;
      if(cRsetToAddModList->list.count)
      {
         macPdcchCfg->numCRsetToAddMod = cRsetToAddModList->list.count;
	 for(cRsetIdx = 0; cRsetIdx < cRsetToAddModList->list.count; cRsetIdx++)
	 {
	    macPdcchCfg->cRSetToAddModList[cRsetIdx].cRSetId = \
	      cRsetToAddModList->list.array[cRsetIdx]->controlResourceSetId;
	    bitStringToInt(&cRsetToAddModList->list.array[cRsetIdx]->frequencyDomainResources,\
	       macPdcchCfg->cRSetToAddModList[cRsetIdx].freqDomainRsrc);
            macPdcchCfg->cRSetToAddModList[cRsetIdx].duration = \
	      cRsetToAddModList->list.array[cRsetIdx]->duration;

	    macPdcchCfg->cRSetToAddModList[cRsetIdx].cceRegMappingType = \
	      cRsetToAddModList->list.array[cRsetIdx]->cce_REG_MappingType.present;   
            if(macPdcchCfg->cRSetToAddModList[cRsetIdx].cceRegMappingType == CCE_REG_MAPPINGTYPE_PR_INTERLEAVED)
	    {
	       //TODO: handle the case for Interleaved
            }
            macPdcchCfg->cRSetToAddModList[cRsetIdx].precoderGranularity = \
	      cRsetToAddModList->list.array[cRsetIdx]->precoderGranularity;
	    if(cRsetToAddModList->list.array[cRsetIdx]->pdcch_DMRS_ScramblingID)
	    {
	       macPdcchCfg->cRSetToAddModList[cRsetIdx].dmrsScramblingId= \
	          *(cRsetToAddModList->list.array[cRsetIdx]->pdcch_DMRS_ScramblingID);
	    }
         }
      }

   }
   /* Control Resource Set To Release List */
   if(cuPdcchCfg->controlResourceSetToReleaseList)
   {
      cRsetToRelList = cuPdcchCfg->controlResourceSetToReleaseList;
      if(cRsetToRelList->list.count)
      {
         macPdcchCfg->numCRsetToRel = cRsetToRelList->list.count;
	 for(cRsetIdx = 0; cRsetIdx < cRsetToRelList->list.count; cRsetIdx++)
	 {
            macPdcchCfg->cRSetToRelList[cRsetIdx] = *(cRsetToRelList->list.array[cRsetIdx]);
	 }
      }
   }

   /* Search space To Add/Mod List */
   if(cuPdcchCfg->searchSpacesToAddModList)
   {
      srchSpcToAddModList = cuPdcchCfg->searchSpacesToAddModList;
      if(srchSpcToAddModList->list.count)
      {
         macPdcchCfg->numSearchSpcToAddMod = srchSpcToAddModList->list.count;
	 for(srchSpcIdx = 0; srchSpcIdx < srchSpcToAddModList->list.count; srchSpcIdx++)
	 {
            macPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceId =\
	       srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceId;
            macPdcchCfg->searchSpcToAddModList[srchSpcIdx].cRSetId =\
	       *(srchSpcToAddModList->list.array[srchSpcIdx]->controlResourceSetId);
	    if(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSlotPeriodicityAndOffset)
	    {
               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].mSlotPeriodicityAndOffset =\
	          srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSlotPeriodicityAndOffset->present;
            }
            if(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSymbolsWithinSlot)
            {
	       bitStringToInt(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSymbolsWithinSlot,\
	          macPdcchCfg->searchSpcToAddModList[srchSpcIdx].mSymbolsWithinSlot);
            }
	    if(srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates)
            {
	      macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel1 = \
	          srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel1;
              macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel2 = \
	          srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel2;
              macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel4 = \
              	  srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel4;
              
              macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel8 = \
              	  srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel8;
              
              macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel16 = \
	          srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel16;
	    }
            if(srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType)
	    {
	       macPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceType =\
	          srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType->present;
	       if(macPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceType == SEARCHSPACETYPE_PR_UE_SPECIFIC)
	       {
		  macPdcchCfg->searchSpcToAddModList[srchSpcIdx].ueSpecificDciFormat =\
		     srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType->choice.ue_Specific->dci_Formats;
	       }
         
	    }
	 }
      }
   }
   /* Search space To Rel List */
   if(cuPdcchCfg->searchSpacesToReleaseList)
   {
      srchSpcToRelList = cuPdcchCfg->searchSpacesToReleaseList;
      if(srchSpcToRelList->list.count)
      {
         macPdcchCfg->numSearchSpcToRel = srchSpcToRelList->list.count;
	 for(srchSpcIdx = 0; srchSpcIdx < srchSpcToRelList->list.count; srchSpcIdx++)
	 {
            macPdcchCfg->searchSpcToRelList[srchSpcIdx] =\
	       *(srchSpcToRelList->list.array[srchSpcIdx]);
	 }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills PdschCfg received by CU
 *
 * @details
 *
 *    Function : extractPdschCfg
 *
 *    Functionality: Fills PdschCfg received  by CU
 *
 * @params[in] PDSCH_Config_t *cuPdschCfg,
 *             PdschConfig *macPdschCfg
 * @return void
 *
 * ****************************************************************/

void extractPdschCfg(PDSCH_Config_t *cuPdschCfg, PdschConfig *macPdschCfg)
{
   uint8_t timeDomIdx;
   struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAlloc = NULLP;

   if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
   {
      if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->present == \
         PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA_PR_setup)
      {
         if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->choice.setup)
	 {
            macPdschCfg->dmrsDlCfgForPdschMapTypeA.addPos = \
	       *(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->choice.setup->dmrs_AdditionalPosition);
	 }
      }
   }
   macPdschCfg->resourceAllocType = cuPdschCfg->resourceAllocation;
   if(cuPdschCfg->pdsch_TimeDomainAllocationList)
   {
      timeDomAlloc = cuPdschCfg->pdsch_TimeDomainAllocationList;
      if(timeDomAlloc->present ==\
         PDSCH_Config__pdsch_TimeDomainAllocationList_PR_setup)
      {
         if(timeDomAlloc->choice.setup)
	 {
	    macPdschCfg->numTimeDomRsrcAlloc  = timeDomAlloc->choice.setup->list.count;
            for(timeDomIdx = 0; timeDomIdx < timeDomAlloc->choice.setup->list.count; timeDomIdx++)
            {
	       macPdschCfg->timeDomRsrcAllociList[timeDomIdx].mappingType = \
	          timeDomAlloc->choice.setup->list.array[timeDomIdx]->mappingType;
	       macPdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbolAndLength = \
	          timeDomAlloc->choice.setup->list.array[timeDomIdx]->startSymbolAndLength;
	    }
	 }
      }
   }
   macPdschCfg->rbgSize = cuPdschCfg->rbg_Size; 
   if(cuPdschCfg->maxNrofCodeWordsScheduledByDCI)
      macPdschCfg->numCodeWordsSchByDci = *(cuPdschCfg->maxNrofCodeWordsScheduledByDCI);
   if(cuPdschCfg->prb_BundlingType.present == PDSCH_Config__prb_BundlingType_PR_staticBundling)
   {
      macPdschCfg->bundlingType = cuPdschCfg->prb_BundlingType.present;
      if(cuPdschCfg->prb_BundlingType.choice.staticBundling)
      {
         if(cuPdschCfg->prb_BundlingType.choice.staticBundling->bundleSize)
	 {
            macPdschCfg->bundlingInfo.StaticBundling.size = \
	       *(cuPdschCfg->prb_BundlingType.choice.staticBundling->bundleSize);
	 }
      }
   }
   else if(cuPdschCfg->prb_BundlingType.present == PDSCH_Config__prb_BundlingType_PR_dynamicBundling)
   {
      macPdschCfg->bundlingType = cuPdschCfg->prb_BundlingType.present;
   }

}

/*******************************************************************
 *
 * @brief Fills PdschServingCellCfg received by CU
 *
 * @details
 *
 *    Function : extractPdschServingCellCfg
 *
 *    Functionality: Fills PdschCfg received  by CU
 *
 * @params[in] PDSCH_ServingCellConfig_t *cuPdschSrvCellCfg,
 *             PdschServCellCfg *macUePdschSrvCellCfg
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractPdschServingCellCfg(PDSCH_ServingCellConfig_t *cuPdschSrvCellCfg, PdschServCellCfg *macUePdschSrvCellCfg)
{
   if(cuPdschSrvCellCfg->codeBlockGroupTransmission)
   {
      if(cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup)
      {
         if(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)
         {
            *(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)  = \
	       cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->maxCodeBlockGroupsPerTransportBlock;
	 }
	 else
	 {
            DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb, sizeof(uint8_t));
            if(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)
	    {
               *(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)  = \
	          cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->maxCodeBlockGroupsPerTransportBlock;
	    }
	    else
	    {
	       DU_LOG("\nDUAPP: Memory allocation failed for maxCodeBlkGrpPerTb at extractPdschServingCellCfg()");
	       return RFAILED;
	    }
	 }
         if(macUePdschSrvCellCfg->codeBlkGrpFlushInd)
         {
            *(macUePdschSrvCellCfg->codeBlkGrpFlushInd)  = \
	       cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->codeBlockGroupFlushIndicator;
	 }
	 else
	 {
            DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb, sizeof(bool));
            if(macUePdschSrvCellCfg->codeBlkGrpFlushInd)
	    {
               *(macUePdschSrvCellCfg->codeBlkGrpFlushInd)  = \
	          cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->codeBlockGroupFlushIndicator;
	    }
	    else
	    {
	       DU_LOG("\nDUAPP: Memory allocation failed for codeBlkGrpFlushInd at extractPdschServingCellCfg()");
	       return RFAILED;
	    }
	 }
      }
   }
   if(cuPdschSrvCellCfg->nrofHARQ_ProcessesForPDSCH)
   {
      macUePdschSrvCellCfg->numHarqProcForPdsch = *(cuPdschSrvCellCfg->nrofHARQ_ProcessesForPDSCH); 
   }
   if(cuPdschSrvCellCfg->ext1)
   {
      if(cuPdschSrvCellCfg->ext1->maxMIMO_Layers)
      {
        if(macUePdschSrvCellCfg->maxMimoLayers)
	{
           *(macUePdschSrvCellCfg->maxMimoLayers)  = *(cuPdschSrvCellCfg->ext1->maxMIMO_Layers);
	}
	else
	{
           DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->maxMimoLayers, sizeof(uint8_t));
           if(macUePdschSrvCellCfg->maxMimoLayers)
	   {
              *(macUePdschSrvCellCfg->maxMimoLayers)  = *(cuPdschSrvCellCfg->ext1->maxMIMO_Layers);
	   }
	   else
	   {
	      DU_LOG("\nDUAPP: Memory allocation failed for maxMimoLayers at extractPdschServingCellCfg()");
	      return RFAILED;
	   }
	}
      }
   }
   if(cuPdschSrvCellCfg->xOverhead)
   {
      if(macUePdschSrvCellCfg->xOverhead)
      {
         *(macUePdschSrvCellCfg->xOverhead)  = *(cuPdschSrvCellCfg->xOverhead);
      }
      else
      {
         DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->xOverhead, sizeof(uint8_t));
         if(macUePdschSrvCellCfg->xOverhead)
         {
            *(macUePdschSrvCellCfg->xOverhead)  = *(cuPdschSrvCellCfg->xOverhead);
         }
         else
         {
            DU_LOG("\nDUAPP: Memory allocation failed for xOverhead at extractPdschServingCellCfg()");
	    return RFAILED;
         }
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills PuschCfg received by CU
 *
 * @details
 *
 *    Function : extractPuschCfg
 *
 *    Functionality: Fills PuschCfg received  by CU
 *
 * @params[in] BWP_UplinkDedicated__pusch_Config *cuPuschCfg,
 *             PuschCfg *macPuschCfg
 * @return void
 *
 * ****************************************************************/

void extractPuschCfg(struct BWP_UplinkDedicated__pusch_Config *cuPuschCfg, PuschCfg *macPuschCfg)
{
   uint8_t timeDomIdx = 0;
   DMRS_UplinkConfig_t *dmrsUlCfg = NULLP;
   struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList = NULLP;

   if(cuPuschCfg->present == BWP_UplinkDedicated__pusch_Config_PR_setup)
   {
      if(cuPuschCfg->choice.setup)
      {
         if(cuPuschCfg->choice.setup->dataScramblingIdentityPUSCH)
	 {
             macPuschCfg->dataScramblingId = \
	        *(cuPuschCfg->choice.setup->dataScramblingIdentityPUSCH);
	 }
	 if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA)
	 {
	    if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->present == PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup)
	    {
	       if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->choice.setup)
	       {
	          dmrsUlCfg = (cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->choice.setup);
	          if(dmrsUlCfg->dmrs_AdditionalPosition)
	          {
                     macPuschCfg->dmrsUlCfgForPuschMapTypeA.addPos =\
	                *(dmrsUlCfg->dmrs_AdditionalPosition);
	          }
                  if(dmrsUlCfg->transformPrecodingDisabled)
	          {
	             if(dmrsUlCfg->transformPrecodingDisabled->scramblingID0)
	             {
	                macPuschCfg->dmrsUlCfgForPuschMapTypeA.transPrecodDisabled.scramblingId0 = \
	                   *(dmrsUlCfg->transformPrecodingDisabled->scramblingID0);
                     }
	          }
	       }
	    }
	 }
	 /*Res Alloc Type for UL */
	 if(cuPuschCfg->choice.setup->resourceAllocation)
	 {
            macPuschCfg->resourceAllocType = \
	       cuPuschCfg->choice.setup->resourceAllocation;
	 }
	 if(cuPuschCfg->choice.setup->pusch_TimeDomainAllocationList)
	 {
	    timeDomAllocList = cuPuschCfg->choice.setup->pusch_TimeDomainAllocationList;
	    if(timeDomAllocList->present == PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup)
	    {
               if(timeDomAllocList->choice.setup)
	       {
	          macPuschCfg->numTimeDomRsrcAlloc = timeDomAllocList->choice.setup->list.count;
	          for(timeDomIdx = 0; timeDomIdx <timeDomAllocList->choice.setup->list.count; timeDomIdx++)
		  {
		     macPuschCfg->timeDomRsrcAllocList[timeDomIdx].k2 = \
		        *(timeDomAllocList->choice.setup->list.array[timeDomIdx]->k2);
                     macPuschCfg->timeDomRsrcAllocList[timeDomIdx].mappingType = \
                        timeDomAllocList->choice.setup->list.array[timeDomIdx]->mappingType;
                     macPuschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbolAndLength = \
		        timeDomAllocList->choice.setup->list.array[timeDomIdx]->startSymbolAndLength;
                  }
	       }
	    }
	 }
	 if(cuPuschCfg->choice.setup->transformPrecoder)
            macPuschCfg->transformPrecoder = *(cuPuschCfg->choice.setup->transformPrecoder);
      }
   }
}

void extractPucchPowerControl(PucchPowerControl *pwrCtrl, struct PUCCH_PowerControl *cuPwrCtrlCfg)
{
   uint8_t arrIdx;

   if(cuPwrCtrlCfg->deltaF_PUCCH_f0)
      pwrCtrl->deltaF_Format0 = *cuPwrCtrlCfg->deltaF_PUCCH_f0;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f1)
      pwrCtrl->deltaF_Format1 = *cuPwrCtrlCfg->deltaF_PUCCH_f1;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f2)
      pwrCtrl->deltaF_Format2 = *cuPwrCtrlCfg->deltaF_PUCCH_f2;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f3)
      pwrCtrl->deltaF_Format3 = *cuPwrCtrlCfg->deltaF_PUCCH_f3;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f4)
      pwrCtrl->deltaF_Format4 = *cuPwrCtrlCfg->deltaF_PUCCH_f4;
   if(cuPwrCtrlCfg->p0_Set)
   {
      pwrCtrl->p0SetCount = cuPwrCtrlCfg->p0_Set->list.count;
      for(arrIdx=0; arrIdx < pwrCtrl->p0SetCount; arrIdx++)
      {
         pwrCtrl->p0Set[arrIdx].p0PucchId =\
	    cuPwrCtrlCfg->p0_Set->list.array[arrIdx]->p0_PUCCH_Id;
         pwrCtrl->p0Set[arrIdx].p0PucchVal =\
	    cuPwrCtrlCfg->p0_Set->list.array[arrIdx]->p0_PUCCH_Value;
      }
   }
   if(cuPwrCtrlCfg->pathlossReferenceRSs)
   {
      pwrCtrl->pathLossRefRSListCount = cuPwrCtrlCfg->pathlossReferenceRSs->list.count;
      for(arrIdx = 0; arrIdx < pwrCtrl->pathLossRefRSListCount; arrIdx++)
      {
         pwrCtrl->pathLossRefRSList[arrIdx].pathLossRefRSId =\
	    cuPwrCtrlCfg->pathlossReferenceRSs->list.array[arrIdx]->pucch_PathlossReferenceRS_Id;

      }
   }
}
 
 /*******************************************************************
 *
 * @brief Function to extractResrcSetToAddModList sent by CU
 *
 * @details
 *
 *    Function : extractResrcSetToAddModList
 *
 *    Functionality: Fucntion to extractResrcSetToAddModList
 *
 * @params[in] PucchResrcSetCfg pointer,
 *             struct PUCCH_Config__resourceSetToAddModList pointer
 * @return void
 *
 * ****************************************************************/

void extractResrcSetToAddModList(PucchResrcSetCfg *macRsrcSetList, struct PUCCH_Config__resourceSetToAddModList *cuRsrcSetList)
{
   uint8_t arrIdx, rsrcListIdx;

   macRsrcSetList->resrcSetToAddModListCount = cuRsrcSetList->list.count; 
   for(arrIdx = 0; arrIdx < macRsrcSetList->resrcSetToAddModListCount; arrIdx++)
   {
      macRsrcSetList->resrcSetToAddModList[arrIdx].resrcSetId     =\
         cuRsrcSetList->list.array[arrIdx]->pucch_ResourceSetId;
      macRsrcSetList->resrcSetToAddModList[arrIdx].resrcListCount =\
         cuRsrcSetList->list.array[arrIdx]->resourceList.list.count;
      for(rsrcListIdx = 0; rsrcListIdx < macRsrcSetList->resrcSetToAddModList[arrIdx].resrcListCount; rsrcListIdx++)
      {
         macRsrcSetList->resrcSetToAddModList[arrIdx].resrcList[rsrcListIdx] =\
            *cuRsrcSetList->list.array[arrIdx]->resourceList.list.array[rsrcListIdx];
      }
      macRsrcSetList->resrcSetToAddModList[arrIdx].maxPayLoadSize =\
         *cuRsrcSetList->list.array[arrIdx]->maxPayloadMinus1;
   }
}/* End of extractResrcSetToAddModList */

/*******************************************************************
 *
 * @brief Fills extractResrcToAddModList sent by CU
 *
 * @details
 *
 *    Function : extractResrcToAddModList
 *
 *    Functionality: Fills extractResrcToAddModList
 *
 * @params[in] PucchResrcCfg pointer,
 *             struct PUCCH_Config__resourceToAddModList pointer
 * @return ROk/RFAILED
 *
 * ****************************************************************/

uint8_t extractResrcToAddModList(PucchResrcCfg *macResrcList, struct PUCCH_Config__resourceToAddModList *cuResrcList)
{
   uint8_t arrIdx;
   
   macResrcList->resrcToAddModListCount = cuResrcList->list.count;
   for(arrIdx = 0; arrIdx < macResrcList->resrcToAddModListCount; arrIdx++)
   {
      macResrcList->resrcToAddModList[arrIdx].resrcId      =\
        cuResrcList->list.array[arrIdx]->pucch_ResourceId; 
      macResrcList->resrcToAddModList[arrIdx].startPrb     =\
        cuResrcList->list.array[arrIdx]->startingPRB;
      if(cuResrcList->list.array[arrIdx]->intraSlotFrequencyHopping)
      {
         macResrcList->resrcToAddModList[arrIdx].intraFreqHop =\
           *cuResrcList->list.array[arrIdx]->intraSlotFrequencyHopping;
      }
      if(cuResrcList->list.array[arrIdx]->secondHopPRB)
      {
         macResrcList->resrcToAddModList[arrIdx].secondPrbHop =\
           *cuResrcList->list.array[arrIdx]->secondHopPRB;
      }
      /* PUCCH RSRC FORMAT */
      if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format0)
      {
         if(cuResrcList->list.array[arrIdx]->format.choice.format0)
	 {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0, sizeof(PucchFormat0));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0 == NULLP)
	    {
               DU_LOG("\nF1AP: Failed to allocate memory for Format0 in extractResrcToAddModList()");
	       return RFAILED;
	    }
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0->initialCyclicShift =\
               cuResrcList->list.array[arrIdx]->format.choice.format0->initialCyclicShift;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0->numSymbols =\
               cuResrcList->list.array[arrIdx]->format.choice.format0->nrofSymbols;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0->startSymbolIdx =\
               cuResrcList->list.array[arrIdx]->format.choice.format0->startingSymbolIndex;
	 }
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format1)
      {
         if(cuResrcList->list.array[arrIdx]->format.choice.format1)
	 {
            DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1, sizeof(PucchFormat1));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1 == NULLP)
	    {
               DU_LOG("\nF1AP: Failed to allocate memory for Format1 in extractResrcToAddModList()");
	       return RFAILED;
	    }
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->initialCyclicShift =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->initialCyclicShift;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->nrofSymbols;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->startingSymbolIndex;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->timeDomOCC =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->timeDomainOCC;
	 }
	 else
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1 = NULLP;
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format2)
      {
         if(cuResrcList->list.array[arrIdx]->format.choice.format2)
	 {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2, sizeof(PucchFormat2_3));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2 == NULLP)
	    {
               DU_LOG("\nF1AP: Failed to allocate memory for Format2 in extractResrcToAddModList()");
	       return RFAILED;
	    }
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2->numPrbs =\
                cuResrcList->list.array[arrIdx]->format.choice.format2->nrofPRBs;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format2->nrofSymbols;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format2->startingSymbolIndex;
         }
	 else
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2 = NULLP;
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format3)
      {
         if(cuResrcList->list.array[arrIdx]->format.choice.format3)
	 {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3, sizeof(PucchFormat2_3));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3 == NULLP)
	    {
               DU_LOG("\nF1AP: Failed to allocate memory for Format3 in extractResrcToAddModList()");
	       return RFAILED;
	    }
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3->numPrbs =\
                cuResrcList->list.array[arrIdx]->format.choice.format3->nrofPRBs;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format3->nrofSymbols;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format3->startingSymbolIndex;
         }
	 else
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3 = NULLP;
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format4)
      {
         if(cuResrcList->list.array[arrIdx]->format.choice.format4)
         {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4, sizeof(PucchFormat4));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4 == NULLP)
	    {
               DU_LOG("\nF1AP: Failed to allocate memory for Format4 in extractResrcToAddModList()");
	       return RFAILED;
	    }
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->nrofSymbols;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->occLen =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->occ_Length;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->occIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->occ_Index;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->startingSymbolIndex;
	 }
	 else
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4 = NULLP;
      }
   }
   return ROK;
}/* End of extractResrcToAddModList */

/*******************************************************************
 *
 * @brief Fills fillPucchSchedReqPeriodAndOffset sent by CU
 *
 * @details
 *
 *    Function : fillPucchSchedReqPeriodAndOffset
 *
 *    Functionality: To fillPucchSchedReqPeriodAndOffset
 *
 * @params[in] macPeriodicty,
 *     SchedulingRequestResourceConfig__periodicityAndOffset pointer
 * @return void
 *
 * ****************************************************************/

void fillPucchSchedReqPeriodAndOffset(uint8_t macPeriodicty, struct
SchedulingRequestResourceConfig__periodicityAndOffset *cuPeriodicty)
{
   if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sym2)
   {
      macPeriodicty = cuPeriodicty->choice.sym2;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sym6or7)
   {
      macPeriodicty = cuPeriodicty->choice.sym6or7;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl1)
   {
      macPeriodicty = cuPeriodicty->choice.sl1;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl2)
   {
      macPeriodicty = cuPeriodicty->choice.sl2;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl4)
   {
      macPeriodicty = cuPeriodicty->choice.sl4;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl5)
   {
      macPeriodicty = cuPeriodicty->choice.sl5;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl8)
   {
      macPeriodicty = cuPeriodicty->choice.sl8;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl10)
   {
      macPeriodicty = cuPeriodicty->choice.sl10;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl16)
   {
      macPeriodicty = cuPeriodicty->choice.sl16;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl20)
   {
      macPeriodicty = cuPeriodicty->choice.sl20;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl40)
   {
      macPeriodicty = cuPeriodicty->choice.sl40;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl80)
   {
      macPeriodicty = cuPeriodicty->choice.sl80;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl160)
   {
      macPeriodicty = cuPeriodicty->choice.sl160;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl320)
   {
      macPeriodicty = cuPeriodicty->choice.sl320;
   }
   else if(cuPeriodicty->present == SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl640)
   {
      macPeriodicty = cuPeriodicty->choice.sl640;
   }
}

/*******************************************************************
 *
 * @brief Function to extractPucchFormatCfg sent by CU
 *
 * @details
 *
 *    Function : extractPucchFormatCfg
 *
 *    Functionality: Function to extractPucchFormatCfg
 *
 * @params[in] PucchFormatCfg pointer,
 *             PUCCH_FormatConfig_t pointer
 * @return void
 *
 * ****************************************************************/

void extractPucchFormatCfg(PucchFormatCfg *macFormatCfg, PUCCH_FormatConfig_t *cuFormatCfg)
 {
    if(cuFormatCfg->interslotFrequencyHopping)
       macFormatCfg->interSlotFreqHop = *cuFormatCfg->interslotFrequencyHopping;
    if(cuFormatCfg->additionalDMRS)  
       macFormatCfg->addDmrs = *cuFormatCfg->additionalDMRS;
    if(cuFormatCfg->maxCodeRate)
       macFormatCfg->maxCodeRate = *cuFormatCfg->maxCodeRate;
    if(cuFormatCfg->nrofSlots)  
       macFormatCfg->numSlots = *cuFormatCfg->nrofSlots;
    if(cuFormatCfg->pi2BPSK)  
       macFormatCfg->pi2BPSK = *cuFormatCfg->pi2BPSK;
    if(cuFormatCfg->simultaneousHARQ_ACK_CSI)  
       macFormatCfg->harqAckCSI = *cuFormatCfg->simultaneousHARQ_ACK_CSI;
 }/* End of extractPucchFormatCfg */

/*******************************************************************
 *
 * @brief Function to extractSchedReqCfgToAddMod sent by CU
 *
 * @details
 *
 *    Function : extractSchedReqCfgToAddMod
 *
 *    Functionality: Function to extractSchedReqCfgToAddMod
 *
 * @params[in] PucchSchedReqCfg pointer,
 *      PUCCH_Config__schedulingRequestResourceToAddModList pointer
 * @return void
 *
 * ****************************************************************/

void extractSchedReqCfgToAddMod(PucchSchedReqCfg *macSchedReqCfg, struct PUCCH_Config__schedulingRequestResourceToAddModList *cuSchedReqList)
{
   uint8_t arrIdx;

   macSchedReqCfg->schedAddModListCount = cuSchedReqList->list.count;
   for(arrIdx = 0; arrIdx <  macSchedReqCfg->schedAddModListCount; arrIdx++)
   {
      macSchedReqCfg->schedAddModList[arrIdx].resrcId =\
         cuSchedReqList->list.array[arrIdx]->schedulingRequestResourceId;
      macSchedReqCfg->schedAddModList[arrIdx].requestId =\
         cuSchedReqList->list.array[arrIdx]->schedulingRequestID;
      if(cuSchedReqList->list.array[arrIdx]->periodicityAndOffset)
      {
         fillPucchSchedReqPeriodAndOffset(macSchedReqCfg->schedAddModList[arrIdx].periodicityAndOffset, cuSchedReqList->list.array[arrIdx]->periodicityAndOffset);
      }
      if(cuSchedReqList->list.array[arrIdx]->resource)
      {
         macSchedReqCfg->schedAddModList[arrIdx].resrc =\
    	    *cuSchedReqList->list.array[arrIdx]->resource;
      }
   }

}/* End of extractSchedReqCfgToAddMod */

 /*******************************************************************
 *
 * @brief Fills PucchCfg received by CU
 *
 * @details
 *
 *    Function : extractPucchCfg
 *
 *    Functionality: Fills PucchCfg received  by CU
 *
 * @params[in] BWP_UplinkDedicated__pucch_Config *cuPucchCfg,
 *             PucchCfg *macPucchCfg
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractPucchCfg(struct BWP_UplinkDedicated__pucch_Config *cuPucchCfg, PucchCfg *macPucchCfg)         
{
   uint8_t arrIdx;

   if(cuPucchCfg->present == BWP_UplinkDedicated__pucch_Config_PR_setup)
   {
      if(cuPucchCfg->choice.setup)
      {
         /* Resource Set Cfg */ 
	 if(cuPucchCfg->choice.setup->resourceSetToAddModList)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->resrcSet, sizeof(PucchResrcSetCfg));
	    if(macPucchCfg->resrcSet == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract Resrc set List in extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->resrcSet, 0, sizeof(PucchResrcSetCfg));
            extractResrcSetToAddModList(macPucchCfg->resrcSet, cuPucchCfg->choice.setup->resourceSetToAddModList);
	 }
	 else
            macPucchCfg->resrcSet = NULLP;
         
	 /* Resource Cfg */ 
	 if(cuPucchCfg->choice.setup->resourceToAddModList)
	 {
            DU_ALLOC_SHRABL_BUF(macPucchCfg->resrc, sizeof(PucchResrcCfg));
	    if(macPucchCfg->resrc == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract Resrc List in extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->resrc, 0, sizeof(PucchResrcCfg));
            extractResrcToAddModList(macPucchCfg->resrc, cuPucchCfg->choice.setup->resourceToAddModList);
	 }
	 else
            macPucchCfg->resrc = NULLP;
         
	 /* Format 1 Cfg */ 
	 if(cuPucchCfg->choice.setup->format1)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format1, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format1 == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract format 1 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format1, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format1,\
	       cuPucchCfg->choice.setup->format1->choice.setup);
	 }
	 else
            macPucchCfg->format1 = NULLP;
         
	 /* Format 2 Cfg */
         if(cuPucchCfg->choice.setup->format2)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format2, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format2 == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract format 2 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format2, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format2,\
	       cuPucchCfg->choice.setup->format2->choice.setup);
	 }
	 else
            macPucchCfg->format2 = NULLP;
         
	 /* Format 3 Cfg */
         if(cuPucchCfg->choice.setup->format3)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format3, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format3 == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract format 3 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format3, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format3,\
	       cuPucchCfg->choice.setup->format3->choice.setup);
	 }
	 else
            macPucchCfg->format3 = NULLP;

         /* Format 4 Cfg */
         if(cuPucchCfg->choice.setup->format4)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format4, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format4 == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract format 4 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format4, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format4,\
	       cuPucchCfg->choice.setup->format4->choice.setup);
	 }
	 else
            macPucchCfg->format4 = NULLP;

         /* Sched Req List */
         if(cuPucchCfg->choice.setup->schedulingRequestResourceToAddModList)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->schedReq, sizeof(PucchSchedReqCfg));
	    if(macPucchCfg->schedReq == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract schedReqCfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->schedReq, 0, sizeof(PucchSchedReqCfg));
            extractSchedReqCfgToAddMod(macPucchCfg->schedReq,\
	    cuPucchCfg->choice.setup->schedulingRequestResourceToAddModList);
	 }
	 else
            macPucchCfg->schedReq = NULLP;

         /* Spatial Info */
         macPucchCfg->spatialInfo = NULLP;

	 /* MultiCsiCfg */
         if(cuPucchCfg->choice.setup->multi_CSI_PUCCH_ResourceList)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->multiCsiCfg, sizeof(PucchMultiCsiCfg));
	    if(macPucchCfg->multiCsiCfg == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract multiCsiCfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->multiCsiCfg, 0, sizeof(PucchMultiCsiCfg));
            macPucchCfg->multiCsiCfg->multiCsiResrcListCount = cuPucchCfg->choice.setup->multi_CSI_PUCCH_ResourceList->list.count;
	    for(arrIdx =0; arrIdx < macPucchCfg->multiCsiCfg->multiCsiResrcListCount; arrIdx++)
	    {
	       macPucchCfg->multiCsiCfg->multiCsiResrcList[arrIdx] =\
	         *cuPucchCfg->choice.setup->multi_CSI_PUCCH_ResourceList->list.array[arrIdx];
	    }
	 }
	 else
	    macPucchCfg->multiCsiCfg = NULLP;
         /* Dl_DataToUL_ACK */ 
	 if(cuPucchCfg->choice.setup->dl_DataToUL_ACK)
	 {
            DU_ALLOC_SHRABL_BUF(macPucchCfg->dlDataToUlAck, sizeof(PucchDlDataToUlAck));
	    if(macPucchCfg->dlDataToUlAck == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract Dl_DataToUL_ACK in extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->dlDataToUlAck, 0, sizeof(PucchDlDataToUlAck));
            macPucchCfg->dlDataToUlAck->dlDataToUlAckListCount = cuPucchCfg->choice.setup->dl_DataToUL_ACK->list.count;
	    for(arrIdx = 0; arrIdx < macPucchCfg->dlDataToUlAck->dlDataToUlAckListCount; arrIdx++)
            {
               macPucchCfg->dlDataToUlAck->dlDataToUlAckList[arrIdx] =\
                  *cuPucchCfg->choice.setup->dl_DataToUL_ACK->list.array[arrIdx];

	    }
	 }
	 else
	    macPucchCfg->dlDataToUlAck = NULLP;
	 /* Power Control */
         if(cuPucchCfg->choice.setup->pucch_PowerControl)
	 {
            DU_ALLOC_SHRABL_BUF(macPucchCfg->powerControl, sizeof(PucchPowerControl));
	    if(macPucchCfg->powerControl == NULLP)
	    {
	       DU_LOG("\nF1AP:Failed to extract power control in extractPucchCfg()");
	       return RFAILED;
	    }
            extractPucchPowerControl(macPucchCfg->powerControl,\
	       cuPucchCfg->choice.setup->pucch_PowerControl);
	 }
	 else
	    macPucchCfg->powerControl = NULLP;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills ServingCellReconfig received by CU
 *
 * @details
 *
 *    Function : extractServingCellReconfig
 *
 *    Functionality: Fills ServingCellReconfig received  by CU
 *
 * @params[in] ServingCellConfig_t *cuSrvCellCfg
 *             ServCellCfgInfo *macSrvCellCfg
 * @return ROK/RFAILD
 *
 * ****************************************************************/
uint8_t extractServingCellReconfig(ServingCellConfig_t *cuSrvCellCfg, ServCellCfgInfo *macSrvCellCfg)
{
   uint8_t ret = ROK;
   BWP_DownlinkDedicated_t *dlBwp = NULLP;
   BWP_UplinkDedicated_t   *ulBwp = NULLP;

   if(cuSrvCellCfg->initialDownlinkBWP)
   {
      dlBwp = ((BWP_DownlinkDedicated_t *)(cuSrvCellCfg->initialDownlinkBWP));
      if(dlBwp->pdcch_Config)
      {
         if(dlBwp->pdcch_Config->choice.setup)
	 {
	    extractPdcchCfg(dlBwp->pdcch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdcchCfg);
	 }
      }
      if(dlBwp->pdsch_Config)
      {
         if(dlBwp->pdsch_Config->choice.setup)
	 {
	    extractPdschCfg(dlBwp->pdsch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdschCfg);
	 }
      }
   }
   if(cuSrvCellCfg->firstActiveDownlinkBWP_Id)
      macSrvCellCfg->firstActvDlBwpId = *(cuSrvCellCfg->firstActiveDownlinkBWP_Id);
   if(cuSrvCellCfg->defaultDownlinkBWP_Id)
      macSrvCellCfg->defaultDlBwpId = *(cuSrvCellCfg->defaultDownlinkBWP_Id);
   if(cuSrvCellCfg->bwp_InactivityTimer)
   {
      if(macSrvCellCfg->bwpInactivityTmr)
      {
         memcpy(macSrvCellCfg->bwpInactivityTmr, cuSrvCellCfg->bwp_InactivityTimer, sizeof(uint8_t));
      }
      else
      {
         macSrvCellCfg->bwpInactivityTmr = NULLP;
         DU_ALLOC_SHRABL_BUF(macSrvCellCfg->bwpInactivityTmr, sizeof(uint8_t));
	 if(macSrvCellCfg->bwpInactivityTmr)
	 {
            memcpy(macSrvCellCfg->bwpInactivityTmr, cuSrvCellCfg->bwp_InactivityTimer, sizeof(uint8_t));
	 }
	 else
	 {
	    DU_LOG("\nMemory Alloc failed for bwpInactivityTmr at extractServingCellReconfig()");
	    return RFAILED;
	 }
      }
   }
   if(cuSrvCellCfg->pdsch_ServingCellConfig)
   {
      if(cuSrvCellCfg->pdsch_ServingCellConfig->choice.setup)
      {
         ret = extractPdschServingCellCfg(cuSrvCellCfg->pdsch_ServingCellConfig->choice.setup, &macSrvCellCfg->pdschServCellCfg);
         if(ret == RFAILED)
	 {
	    DU_LOG("\nFailed at extractPdschServingCellCfg()");
	    return RFAILED;
	 }
      }
   }
   if(cuSrvCellCfg->uplinkConfig)
   {
     if(cuSrvCellCfg->uplinkConfig->initialUplinkBWP)
     {
        ulBwp = ((BWP_UplinkDedicated_t *)(cuSrvCellCfg->uplinkConfig->initialUplinkBWP));
	if(ulBwp->pusch_Config)
	{
	   macSrvCellCfg->initUlBwp.puschPresent = true;
           extractPuschCfg(ulBwp->pusch_Config, &macSrvCellCfg->initUlBwp.puschCfg);
	}
	if(ulBwp->pucch_Config)
	{
	   macSrvCellCfg->initUlBwp.pucchPresent = true;
           extractPucchCfg(ulBwp->pucch_Config, &macSrvCellCfg->initUlBwp.pucchCfg); 
	}
     }
     if(cuSrvCellCfg->uplinkConfig->firstActiveUplinkBWP_Id)
        macSrvCellCfg->firstActvUlBwpId = *(cuSrvCellCfg->uplinkConfig->firstActiveUplinkBWP_Id);

   }
   return ret;
}
/*******************************************************************
 *
 * @brief Fills Reconfig Cell group Info received by CU
 *
 * @details
 *
 *    Function : extractUeReCfgCellInfo
 *
 *    Functionality: Fills Reconfig Cell group Info received by CU
 *
 * @params[in] CellGroupConfigRrc_t *macCellGrpCfg
 *             MacUeCfg*  macUeCfg
 * @return ROK/RFAILED
 *
 * ****************************************************************/
uint8_t extractUeReCfgCellInfo(CellGroupConfigRrc_t *cellGrp, MacUeCfg *macUeCfg)
{
   uint8_t ret = ROK;
   MAC_CellGroupConfig_t     *macCellGroup = NULLP;
   PhysicalCellGroupConfig_t *phyCellGrpCfg = NULLP;
   SpCellConfig_t            *spcellCfg = NULLP;
   ServingCellConfig_t       *servCellCfg = NULLP;

   if(cellGrp)
   {
      /* Fill MacCell Group Reconfig  */
      if(cellGrp->mac_CellGroupConfig)
      {
         macCellGroup = ((MAC_CellGroupConfig_t *)(cellGrp->mac_CellGroupConfig));
         if(macCellGroup->schedulingRequestConfig)
	 {
            extractSchReqReConfig(macCellGroup->schedulingRequestConfig, &macUeCfg->macCellGrpCfg.schReqCfg);
	 }
         if(macCellGroup->tag_Config)
	 {
            extractTagReconfig(macCellGroup->tag_Config, &macUeCfg->macCellGrpCfg.tagCfg);
	 }
	 if(macCellGroup->bsr_Config)
	 {
            macUeCfg->macCellGrpCfg.bsrTmrCfg.periodicTimer = macCellGroup->bsr_Config->periodicBSR_Timer;
            macUeCfg->macCellGrpCfg.bsrTmrCfg.retxTimer     = macCellGroup->bsr_Config->retxBSR_Timer;
	    if(macCellGroup->bsr_Config->logicalChannelSR_DelayTimer)
	    {
               macUeCfg->macCellGrpCfg.bsrTmrCfg.srDelayTimer  =\
	          *(macCellGroup->bsr_Config->logicalChannelSR_DelayTimer);
	    }
	 }
	 if(macCellGroup->phr_Config)
	 {
	    if(macCellGroup->phr_Config->present == MAC_CellGroupConfig__phr_Config_PR_setup)
	    {
               macUeCfg->macCellGrpCfg.phrCfgSetupPres = true;
               if(macCellGroup->phr_Config->choice.setup)
	       {
                macUeCfg->macCellGrpCfg.phrCfg.periodicTimer     = \
		   macCellGroup->phr_Config->choice.setup->phr_PeriodicTimer;
                macUeCfg->macCellGrpCfg.phrCfg.prohibitTimer     = \
		   macCellGroup->phr_Config->choice.setup->phr_ProhibitTimer;
                macUeCfg->macCellGrpCfg.phrCfg.txPowerFactor     = \
		   macCellGroup->phr_Config->choice.setup->phr_Tx_PowerFactorChange;
                macUeCfg->macCellGrpCfg.phrCfg.multiplePHR       = \
		   macCellGroup->phr_Config->choice.setup->multiplePHR;
                macUeCfg->macCellGrpCfg.phrCfg.dummy             = \
		   macCellGroup->phr_Config->choice.setup->dummy;
                macUeCfg->macCellGrpCfg.phrCfg.phrType2OtherCell = \
		   macCellGroup->phr_Config->choice.setup->phr_Type2OtherCell;
                macUeCfg->macCellGrpCfg.phrCfg.phrOtherCG        = \
		   macCellGroup->phr_Config->choice.setup->phr_ModeOtherCG;
	       }
	    }
	 }
      }
      /* Fill Physical Cell Group Reconfig */
      if(cellGrp->physicalCellGroupConfig)
      {
         phyCellGrpCfg = ((PhysicalCellGroupConfig_t *)(cellGrp->physicalCellGroupConfig));
         if(phyCellGrpCfg->p_NR_FR1)
	 {
	    if(*(phyCellGrpCfg->p_NR_FR1) != macUeCfg->phyCellGrpCfg.pNrFr1)
               macUeCfg->phyCellGrpCfg.pNrFr1 = *(phyCellGrpCfg->p_NR_FR1);
	 }
         macUeCfg->phyCellGrpCfg.pdschHarqAckCodebook = phyCellGrpCfg->pdsch_HARQ_ACK_Codebook;
      }
      /* Fill SpCell Reconfig */
      if(cellGrp->spCellConfig)
      {
         spcellCfg = ((SpCellConfig_t *)(cellGrp->spCellConfig));  
         if(spcellCfg->servCellIndex)
	 {
            macUeCfg->spCellCfg.servCellIdx = *(spcellCfg->servCellIndex);
	 }
         /* Fill Serving cell Reconfig info */
	 if(cellGrp->spCellConfig->spCellConfigDedicated)
	 {
	    servCellCfg = ((ServingCellConfig_t *)(cellGrp->spCellConfig->spCellConfigDedicated));
            ret = extractServingCellReconfig(servCellCfg, &macUeCfg->spCellCfg.servCellCfg);
	    if(ret == RFAILED)
	    {
	       DU_LOG("\n Failed at extractServingCellReconfig()");
	    }
	 }
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief builds Mac Cell Cfg
 *
 * @details
 *
 *    Function : procUeReCfgCellInfo
 *
 *    Functionality: builds Mac Cell Cfg
 *
 * @params[in] MacUeCfg pointer
 *             void pointer
 *
 * @return void 
 *
 * ****************************************************************/
uint8_t procUeReCfgCellInfo(MacUeCfg *macUeCfg, void *cellInfo)
{
   uint8_t ret = ROK;
   CellGroupConfigRrc_t *cellGrp = NULLP;

   if(cellInfo)
   {
      cellGrp = (CellGroupConfigRrc_t *)cellInfo;
      ret = extractUeReCfgCellInfo(cellGrp, macUeCfg);
      if(ret == RFAILED)
         DU_LOG("\nF1AP: Failed at procUeReCfgCellInfo()");
   }
   if(ret == RFAILED)
   {
      freeUeReCfgCellGrpInfo(macUeCfg);
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Function to extract cellGrp Info present in cutoDu cont
 *
 * @details
 *
 *    Function : extractCellGrpInfo
 *
 *    Functionality: Function to extract cellGrp Info present
 *                   in cutoDu cont
 *
 * @params[in] ProtocolExtensionContainer_4624P16_t pointer
 *
 * @return CellGroupConfigRrc_t *
 *
 * ****************************************************************/

CellGroupConfigRrc_t *extractCellGrpInfo(ProtocolExtensionContainer_4624P16_t *protocolIeExtn,\
   DuUeCfg *ueCfgDb)
{
   uint8_t idx2;
   uint16_t id;
   uint16_t recvBufLen;
   CellGroupConfigRrc_t *cellGrpCfg = NULLP;
   CUtoDURRCInformation_ExtIEs_t *extIeInfo = NULLP;
   asn_dec_rval_t rval; /* Decoder return value */
   memset(&rval, 0, sizeof(asn_dec_rval_t));

   if(protocolIeExtn)
   {
      for(idx2 = 0; idx2 < protocolIeExtn->list.count; idx2++)
      {
         extIeInfo = ((CUtoDURRCInformation_ExtIEs_t *)(protocolIeExtn->list.array[idx2]));
	 id = extIeInfo->id;
         switch(id)
         {
            case ProtocolIE_ID_id_CellGroupConfig:
            {
	       recvBufLen = extIeInfo->extensionValue.choice.CellGroupConfig.size;
	       /* decoding the CellGroup Buf received */
	       DU_ALLOC(cellGrpCfg, sizeof(CellGroupConfigRrc_t));
	       if(cellGrpCfg)
	       {
                  memset(cellGrpCfg, 0, sizeof(CellGroupConfigRrc_t));
                  rval = aper_decode(0, &asn_DEF_CellGroupConfigRrc, (void **)&cellGrpCfg,
	             extIeInfo->extensionValue.choice.CellGroupConfig.buf, recvBufLen, 0, 0);
                  if(rval.code == RC_FAIL || rval.code == RC_WMORE)
                  {
                     DU_LOG("\nF1AP : ASN decode failed at decodeCellGrpCfg()");
                     return NULLP;
                  }
                  xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, cellGrpCfg);
		  if(extractRlcCfgToAddMod(cellGrpCfg->rlc_BearerToAddModList, ueCfgDb))
		     return NULLP;
	       }
	       break;
            }
            default:
               DU_LOG("\nF1AP : Invalid IE received CUtoDURRCInformation:%d at decodeCellGrpCfg()", id);
      	       break;
         }
      }
   }
   return cellGrpCfg;
}

/*******************************************************************
 *
 * @brief Fills Srb List received by CU
 *
 * @details
 *
 *    Function : procSrbListToSetup
 *
 *    Functionality: Fills Srb List received  by CU
 *
 * @params[in] SRBs_ToBeSetup_Item_t *cuSrbItem
 *             LcCfg pointer
 *             RlcBearerCfg pointer
 * @return void
 *
 * ****************************************************************/
uint8_t procSrbListToSetup(SRBs_ToBeSetup_Item_t * srbItem, LcCfg *macLcToAdd, RlcBearerCfg *rlcLcToAdd)
{
   uint8_t ret = ROK;

   /* Filling RLC INFO */
   procRlcLcCfg(srbItem->sRBID, srbItem->sRBID, RB_TYPE_SRB, RLC_AM, CONFIG_ADD, NULL, rlcLcToAdd);

   /* Filling MAC INFO */
   ret = procMacLcCfg(srbItem->sRBID, RB_TYPE_SRB, CONFIG_ADD, NULL, NULL, macLcToAdd);
   if(ret == RFAILED)
   { 
      DU_LOG("\nF1AP:Failed at MAC LC Cfg in procSrbListToSetup()");
      return ret;
   }
   return ret;
}



/*******************************************************************
 *
 * @brief extract Srb List received by CU
 *
 * @details
 *
 *    Function : extractSrbListToSetup
 *
 *    Functionality: extract Srb List received by CU
 *                   for both MAC and RLC
 *
 * @params[in] SRBs_ToBeSetup_Item_t pointer
 *             DuUeCfg pointer
 * @return ROK/RFAIED
 *
 * ****************************************************************/

uint8_t extractSrbListToSetup(SRBs_ToBeSetup_List_t *srbCfg, DuUeCfg *ueCfgDb)
{
   uint8_t ret, srbIdx;
   SRBs_ToBeSetup_Item_t *srbItem = NULLP;

   if(srbCfg)
   {
      for(srbIdx = 0; srbIdx < srbCfg->list.count; srbIdx++)
      {
         srbItem = &srbCfg->list.array[srbIdx]->value.choice.SRBs_ToBeSetup_Item;
         if(ueCfgDb->numMacLcs > MAX_NUM_LC)
	 { 
            DU_LOG("\nF1AP:  MAX LC Reached in MAC");
	    ret = RFAILED;
	    break;
	 }
         if(ueCfgDb->numRlcLcs > MAX_NUM_LC)
	 {
            DU_LOG("\nF1AP:  MAX LC Reached in RLC");
	    ret = RFAILED;
	    break;
	 }
	 memset(&ueCfgDb->macLcCfg[ueCfgDb->numMacLcs], 0, sizeof(LcCfg));
	 memset(&ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs], 0, sizeof(RlcBearerCfg));
         ret = procSrbListToSetup(srbItem, &ueCfgDb->macLcCfg[ueCfgDb->numMacLcs],\
	    &ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs]);
	 ueCfgDb->numRlcLcs++;
	 ueCfgDb->numMacLcs++;
	 if(ret == RFAILED)
	 {
            DU_LOG("\nF1AP:  Failed at extractSrbListToSetup()");
	    break;
	 }
      }
   }
   else
      ret = RFAILED;

   return ret;
}

/*******************************************************************
 *
 * @brief Fills Drb List received by CU
 *
 * @details
 *
 *    Function : procDrbListToSetup
 *
 *    Functionality: Fills Drb List received by CU
 *                   for both MAC and RLC
 *
 * @params[in] SRBs_ToBeSetup_Item_t pointer
 *             LcCfg pointer,
 *             RlcBearerCfg pointer
 * @return void
 *
 * ****************************************************************/

uint8_t procDrbListToSetup(uint8_t lcId, DRBs_ToBeSetup_Item_t *drbItem, LcCfg *macLcToAdd, RlcBearerCfg *rlcLcToAdd)
{
   uint8_t ret = ROK;

   /* Filling RLC INFO */
   procRlcLcCfg(drbItem->dRBID, lcId, RB_TYPE_DRB, drbItem->rLCMode, CONFIG_ADD, NULL, rlcLcToAdd);

   /* Filling MAC INFO */
   ret = procMacLcCfg(lcId, RB_TYPE_DRB, CONFIG_ADD, drbItem, NULL, macLcToAdd);
   if(ret == RFAILED)
   { 
      DU_LOG("\nF1AP:Failed at RLC LC Cfg in extractDrbListToSetup()");
      return ret;
   }

   return ret;
}

/*******************************************************************
 *
 * @brief extract Drb List received by CU
 *
 * @details
 *
 *    Function : extractDrbListToSetup
 *
 *    Functionality: extract Drb List received by CU
 *                   for both MAC and RLC
 *
 * @params[in] DRBs_ToBeSetup_Item_t pointer
 *             DuUeCfg pointer
 * @return ROK/RFAIED
 *
 * ****************************************************************/

uint8_t extractDrbListToSetup(uint8_t lcId, DRBs_ToBeSetup_List_t *drbCfg, DuUeCfg *ueCfgDb)
{
   uint8_t ret, drbIdx;
   DRBs_ToBeSetup_Item_t *drbItem = NULLP;

   ret = ROK;
   if(drbCfg)
   {
      for(drbIdx = 0; drbIdx < drbCfg->list.count; drbIdx++)
      {
         drbItem = &drbCfg->list.array[drbIdx]->value.choice.DRBs_ToBeSetup_Item;
	 if(ueCfgDb->numMacLcs > MAX_NUM_LC)
	 { 
            DU_LOG("\nF1AP:  MAX LC Reached in MAC ");
	    ret = RFAILED;
	    break;
	 }
	 if(ueCfgDb->numRlcLcs > MAX_NUM_LC)
	 {
            DU_LOG("\nF1AP:  MAX LC Reached in RLC");
	    ret = RFAILED;
	    break;
	 }
	 memset(&ueCfgDb->macLcCfg[ueCfgDb->numMacLcs], 0, sizeof(LcCfg));
	 memset(&ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs], 0, sizeof(RlcBearerCfg));
         ret = procDrbListToSetup(lcId, drbItem, &ueCfgDb->macLcCfg[ueCfgDb->numMacLcs],\
	    &ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs]);
	 ueCfgDb->numRlcLcs++;
	 ueCfgDb->numMacLcs++;
	 if(ret == RFAILED)
	 {
            DU_LOG("\nF1AP:  Failed at extractDrbListToSetup()");
	    break;
	 }
      }
   }
   else
      ret = RFAILED;

   return ret;
}

/*******************************************************************
 *
 * @brief Function to extract Dl RRC Msg received from CU
 *
 * @details
 *
 *    Function : extractDlRrcMsg
 *
 *    Functionality: Function to extract Dl RRC Msg received from CU
 *
 * @params[in] F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t extractDlRrcMsg(uint32_t gnbDuUeF1apId, uint32_t gnbCuUeF1apId, \
   F1DlRrcMsg *dlRrcMsg, RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;
   dlRrcMsg->rrcMsgSize = rrcContainer->size;
   if(dlRrcMsg->rrcMsgSize > 0)
   {
      DU_ALLOC_SHRABL_BUF(dlRrcMsg->rrcMsgPdu, dlRrcMsg->rrcMsgSize);
      if(!dlRrcMsg->rrcMsgPdu)
      {
         DU_LOG("\nDU APP : Memory allocation failed for RRC Msg in procUeCtxtSetupReq");
         ret = RFAILED;
      }
      else
      {
         dlRrcMsg->gnbDuUeF1apId = gnbDuUeF1apId;
         dlRrcMsg->gnbCuUeF1apId = gnbCuUeF1apId;
         dlRrcMsg->srbId = SRB1_LCID;
         memcpy(dlRrcMsg->rrcMsgPdu, rrcContainer->buf, dlRrcMsg->rrcMsgSize);
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Process UE context setup request from CU
 *
 * @details
 *
 *    Function : procF1UeContextSetupReq
 *
 *    Functionality: Process UE context setup request from CU
 *
 * @params[in] F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1UeContextSetupReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t    ret, ieIdx, ueIdx, lcId, cellIdx;
   bool ueCbFound = false;
   uint32_t gnbCuUeF1apId, gnbDuUeF1apId, bitRateSize;
   DuUeCb   *duUeCb = NULLP;
   UEContextSetupRequest_t   *ueSetReq = NULLP;
    
   ret = ROK;
   
   ueSetReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;
   for(ieIdx=0; (ieIdx < ueSetReq->protocolIEs.list.count && ret == ROK); ieIdx++)
   {
      switch(ueSetReq->protocolIEs.list.array[ieIdx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       gnbCuUeF1apId = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       gnbDuUeF1apId = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_ServCellIndex:
	    {
	       cellIdx = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.ServCellIndex;
               for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
	       {
	          if((duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbDuUeF1apId == gnbDuUeF1apId)&&\
	             (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbCuUeF1apId == gnbCuUeF1apId))
		  {
		     ueCbFound = true;
                     duUeCb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx];
                     DU_ALLOC(duUeCb->f1UeDb, sizeof(F1UeContextSetupDb));
                     if(duUeCb->f1UeDb)
                     {
                        memset(duUeCb->f1UeDb, 0, sizeof(F1UeContextSetupDb));
		        duUeCb->f1UeDb->actionType = UE_CTXT_SETUP;
		        duUeCb->f1UeDb->cellIdx = cellIdx;
		     }
		     else
		     {
                        DU_LOG("\nF1AP: Memory Alloc Failed at procF1UeContextSetupReq()");
                        ret = RFAILED;
		     }
		  }
		  else
		     ueCbFound = false;
		  
	       }
	       if(!ueCbFound)
	       {
                  DU_LOG("\nF1AP: DuUeCb is not found at procF1UeContextSetupReq()");
                  ret = RFAILED;
	       }
	       break;
	    }
         case ProtocolIE_ID_id_SpCellULConfigured:
	    /* Indicates whether the gNB-CU requests the gNB-DU to configure the uplink as no UL, 
	    UL, SUL or UL+SUL for the indicated cell for the UE */
	    break;
         case ProtocolIE_ID_id_CUtoDURRCInformation:
	    {
	       if(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.CUtoDURRCInformation.iE_Extensions)
	       {
                  duUeCb->f1UeDb->duUeCfg.cellGrpCfg = extractCellGrpInfo(ueSetReq->protocolIEs.list.array[ieIdx]->\
		     value.choice.CUtoDURRCInformation.iE_Extensions, &duUeCb->f1UeDb->duUeCfg);
		  if(!duUeCb->f1UeDb->duUeCfg.cellGrpCfg)
		  {
		     DU_LOG("\n F1AP: Failed to extract cell Grp Info");
		     //TODO: Update the failure cause in ue context Setup Response
		     ret = RFAILED;
		  }
	       }
               break;
            } 
         case ProtocolIE_ID_id_SCell_ToBeSetup_List:
	    {
	       DU_LOG("\nDU_APP: Received SCell to be added");
	       DU_LOG("\nDU_APP: Not processing the SCell_ToBeSetup_List");
	    break;
	    }
         case ProtocolIE_ID_id_SRBs_ToBeSetup_List:
	    {
	       if(extractSrbListToSetup(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.SRBs_ToBeSetup_List,\
	          &duUeCb->f1UeDb->duUeCfg))
	       {
	          DU_LOG("\nDU_APP: Failed at extractSrbListToSetup()");
		  //TODO: Update the failure cause in ue context Setup Response
	          ret = RFAILED;
	       }
	       break;
	    }
         case ProtocolIE_ID_id_DRBs_ToBeSetup_List:
	    {
	       lcId = getDrbLcId(&duUeCb->drbBitMap);
	       if(lcId != RFAILED)
	       {
                  if(extractDrbListToSetup(lcId, &ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRBs_ToBeSetup_List,\
	             &duUeCb->f1UeDb->duUeCfg))
	          {
	             DU_LOG("\nDU_APP: Failed at extractDrbListToSetup()");
	             //TODO: Update the failure cause in ue context Setup Response
	             ret = RFAILED;
	          }
	       }
	       else 
	          ret = RFAILED;
               break;
	    }
	 case ProtocolIE_ID_id_RRCContainer:
	    {
	       /* Filling Dl RRC Msg Info */
	       DU_ALLOC_SHRABL_BUF(duUeCb->f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
	       if(!duUeCb->f1UeDb->dlRrcMsg)
	       {
	          DU_LOG("\nDU APP : Memory allocation failed for DL RRC Msg in procUeCtxtSetupReq()");
		  ret = RFAILED;
	       }
	       else
	       {
	          memset(duUeCb->f1UeDb->dlRrcMsg, 0, sizeof(F1DlRrcMsg));
	          ret = extractDlRrcMsg(gnbDuUeF1apId, gnbCuUeF1apId, duUeCb->f1UeDb->dlRrcMsg,\
		     &ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer);
	       }	  
	       break;
	    }
	 case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
	    {
	       if(duUeCb->f1UeDb->dlRrcMsg->rrcMsgPdu)
	       {
		  duUeCb->f1UeDb->dlRrcMsg->deliveryStatRpt = true;
	       }
	       else
	       {
		  DU_LOG("\nIgnoring delivery report, since rrcContainer is not present");
	       }
	       break;
	    }
	 case ProtocolIE_ID_id_GNB_DU_UE_AMBR_UL:
	     {
               /* MaximumBitRate Uplink */
	       bitRateSize = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.BitRate.size;
	       if(bitRateSize > 0)
	       {
                  DU_ALLOC_SHRABL_BUF(duUeCb->f1UeDb->duUeCfg.ambrCfg, bitRateSize);
		  if(duUeCb->f1UeDb->duUeCfg.ambrCfg == NULLP)
		  {
	             DU_LOG("\nDU APP : Memory allocation failed for bitRate in procUeCtxtSetupReq");
	             ret = RFAILED;
		  }
		  else
		  {
		     memset(duUeCb->f1UeDb->duUeCfg.ambrCfg, 0, sizeof(AmbrCfg)); 
                     memcpy(&duUeCb->f1UeDb->duUeCfg.ambrCfg->ulBr,
		     ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.BitRate.buf, bitRateSize);
                     duUeCb->f1UeDb->duUeCfg.ambrCfg->dlBr = 0;
		  }
	       }
	       else
	          ret = RFAILED;
	       break;
	    }
	 default:
	    {
	       break;
	    }
      }
   }
   if(ret == RFAILED)
   {
      /*TODO : Negative case*/
      // BuildAndSendUeContextSetupRsp(ueIdx, cellId);
      DU_LOG("F1AP: Failed to process UE CNTXT SETUP REQ at procF1UeContextSetupReq()"); 
   }
   else
      ret = duProcUeContextSetupRequest(duUeCb);

   return ret;

}

/*******************************************************************
 * @brief Free the memory allocated for UE Setup response
 *
 * @details
 *
 *    Function : FreeUeContextSetupRsp
 *
 *    Functionality:
 *       Free the memory allocated for UE Setup response
 *
 * @params[in] F1AP PDU for UE setup response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeUeContextSetupRsp(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx;
   UEContextSetupResponse_t *ueSetRsp = NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.successfulOutcome)
      {
	 ueSetRsp = &f1apMsg->choice.successfulOutcome->value.choice.\
		    UEContextSetupResponse;
	 if(ueSetRsp->protocolIEs.list.array)
	 {
	    for(idx = 0; idx < ueSetRsp->protocolIEs.list.count; idx++)
	    {
	       if(ueSetRsp->protocolIEs.list.array[idx])
	       {
		  switch(ueSetRsp->protocolIEs.list.array[idx]->id)
		  {
		     case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
			break;
		     case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
			break;
		     case ProtocolIE_ID_id_DUtoCURRCInformation:
			{
			   CellGroupConfig_t *cellGrpCfg = NULLP;
			   cellGrpCfg  = &ueSetRsp->protocolIEs.list.array[idx]->value.choice.\
					 DUtoCURRCInformation.cellGroupConfig;
			   if(cellGrpCfg->buf != NULLP)
			   {
			      DU_FREE(cellGrpCfg->buf, cellGrpCfg->size);
			      cellGrpCfg = NULLP;
			   }
			   break;
			}
		     default:
		        DU_LOG("\nDUAPP: Invalid Id %ld at FreeUeContextSetupRsp()", ueSetRsp->protocolIEs.list.array[idx]->id);
		        break;
		  }
		  DU_FREE(ueSetRsp->protocolIEs.list.array[idx],\
			sizeof(UEContextSetupResponseIEs_t));
	       }
	    }
	    DU_FREE(ueSetRsp->protocolIEs.list.array, \
		  ueSetRsp->protocolIEs.list.size);
	 }
	 DU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds Ue context Setup Rsp DU To CU Info
 *
 * @details
 *
 *    Function : EncodeUeCntxtDuToCuInfo
 *
 *    Functionality: Builds Ue context Setup Rsp DU To CU Info
 *
 * @params[in] CellGroupConfig_t *, CellGroupConfigRrc_t * 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t EncodeUeCntxtDuToCuInfo(CellGroupConfig_t *duToCuCellGrp, CellGroupConfigRrc_t *duCellGrpCfg)
{
   asn_enc_rval_t        encRetVal;

   xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, duCellGrpCfg);
   memset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_CellGroupConfigRrc, 0, duCellGrpCfg, PrepFinalEncBuf, encBuf);
   /* Encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG( "\n F1AP : Could not encode UeCntxtDuToCuInfo (at %s)\n",\
	    encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;
   }
   else
   {
      DU_LOG("\n F1AP : Created APER encoded buffer for UeCntxtDuToCuInfo\n");
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x",encBuf[i]);
      }
   }
   duToCuCellGrp->size = encBufSize;
   DU_ALLOC(duToCuCellGrp->buf, duToCuCellGrp->size);
   if(!duToCuCellGrp->buf)
   {
      DU_LOG("\nF1AP : Memory allocation failed in UeCntxtDuToCuInfo");
   }
   memcpy(duToCuCellGrp->buf, encBuf, duToCuCellGrp->size);
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends the UE Setup Response
 *
 * @details
 *
 *    Function : BuildAndSendUeContextSetupRsp
 *
 *    Functionality: Constructs the UE Setup Response and sends
 *                   it to the DU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendUeContextSetupRsp(uint8_t ueIdx, uint8_t cellId)
{
   uint8_t   idx, ret, cellIdx, elementCnt;
   uint32_t  gnbCuUeF1apId;   /* gNB-CU UE F1AP Id */
   uint32_t  gnbDuUeF1apId;   /* gNB-DU UE F1AP Id */
   asn_enc_rval_t  encRetVal;        /* Encoder return value */
   F1AP_PDU_t               *f1apMsg = NULLP;
   UEContextSetupResponse_t *ueSetRsp = NULLP;
   CellGroupConfigRrc_t     *cellGrpCfg = NULLP;
   DuUeCb                   *ueCb = NULLP;

   DU_LOG("\n F1AP : Building UE Context Setup Response for cellId %d, ueIdx %d\n", cellId, ueIdx);

   while(true)
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
	 ret = RFAILED;
	 break;
      }

      f1apMsg->present = F1AP_PDU_PR_successfulOutcome;
      DU_ALLOC(f1apMsg->choice.successfulOutcome,
	    sizeof(SuccessfulOutcome_t));
      if(f1apMsg->choice.successfulOutcome == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
	 ret = RFAILED;
	 break;
      }

      f1apMsg->choice.successfulOutcome->procedureCode = \
							 ProcedureCode_id_UEContextSetup;
      f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
      f1apMsg->choice.successfulOutcome->value.present = \
							 SuccessfulOutcome__value_PR_UEContextSetupResponse;

      ueSetRsp =
	 &f1apMsg->choice.successfulOutcome->value.choice.UEContextSetupResponse;
      elementCnt = 3;
      ueSetRsp->protocolIEs.list.count = elementCnt;
      ueSetRsp->protocolIEs.list.size = \
					elementCnt * sizeof(UEContextSetupResponse_t *);

      /* Initialize the UESetup members */
      DU_ALLOC(ueSetRsp->protocolIEs.list.array, \
	    ueSetRsp->protocolIEs.list.size);
      if(ueSetRsp->protocolIEs.list.array == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for UE Setup Response failed");
	 ret = RFAILED;
	 break;
      }

      for(idx=0; idx<elementCnt; idx++)
      {
	 DU_ALLOC(ueSetRsp->protocolIEs.list.array[idx],\
	       sizeof(UEContextSetupResponseIEs_t));
	 if(ueSetRsp->protocolIEs.list.array[idx] == NULLP)
	 {
	    DU_LOG(" F1AP : Memory allocation for UE Setup Response failed");
	    ret = RFAILED;
	    break;
	 }
      }
      /* Fetching Ue Cb Info*/
      GET_CELL_IDX(cellId, cellIdx);
      gnbDuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].gnbDuUeF1apId;
      gnbCuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].gnbCuUeF1apId;
      ueCb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1];

      idx = 0;
      /*GNB CU UE F1AP ID*/
      ueSetRsp->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupResponseIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = gnbCuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx++;
      ueSetRsp->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupResponseIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = gnbDuUeF1apId;


      /*DUtoCURRC Information */
      idx++;
      ueSetRsp->protocolIEs.list.array[idx]->id  = \
						   ProtocolIE_ID_id_DUtoCURRCInformation;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present =\
							    UEContextSetupResponseIEs__value_PR_DUtoCURRCInformation;
      if(ueCb->f1UeDb)
      {
         if(ueCb->f1UeDb->duUeCfg.cellGrpCfg)
	 {
	    cellGrpCfg = (CellGroupConfigRrc_t*)ueCb->f1UeDb->duUeCfg.cellGrpCfg;
	    ret = EncodeUeCntxtDuToCuInfo(&ueSetRsp->protocolIEs.list.array[idx]->value.\
	             choice.DUtoCURRCInformation.cellGroupConfig, cellGrpCfg);
	    /* Free UeContext Db created during Ue context Req */
	    freeF1UeDb(ueCb->f1UeDb);
	    ueCb->f1UeDb = NULLP;
         }
      }
      else
      {
         DU_LOG("\nF1AP: Failed to form DUtoCU RRCInfo at BuildAndSendUeContextSetupRsp()");
         ret = RFAILED;
      }
      if(ret == RFAILED)
         break;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the UE context setup response type as APER */
      memset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	    encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG( "\n F1AP : Could not encode UE Context Setup Request structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 ret = RFAILED;
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
      if(SendF1APMsg(DU_APP_MEM_REGION,DU_POOL)	!= ROK)
      {
	 DU_LOG("\n F1AP : Sending UE Context Setup Request Failed");
	 ret = RFAILED;
	 break;
      }
      break;
   }
   FreeUeContextSetupRsp(f1apMsg);
   return ret;
}/* End of BuildAndSendUeContextSetupRsp */


uint8_t BuildAndSendUeCtxtRsp(uint8_t ueIdx, uint8_t cellId)
{
   uint8_t cellIdx = 0, actionType = 0; 

   GET_CELL_IDX(cellId, cellIdx);
   actionType = duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].f1UeDb->actionType;

   switch(actionType)
   {
      case UE_CTXT_SETUP:
         BuildAndSendUeContextSetupRsp(ueIdx, cellId);
         break;
      case UE_CTXT_MOD:
         //TODO: Build Ue context Modification Rsp
         break;
      default:
         DU_LOG("F1AP: Invalid Action Type %d at BuildAndSendUeCtxtRsp()", actionType);
         break;

   }
   return ROK;
}

/*******************************************************************
 *
 * @brief deallocating the memory of  F1reset msg
 *
 * @details
 *
 *    Function : FreeF1ResetReq
 *
 *    Functionality :
 *         - freeing memory of F1reset request msg
 *
 * @params[in]
 * @return void
 *
 *
 * ****************************************************************/
void FreeF1ResetReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx =0 ;
   Reset_t *f1ResetMsg;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
	 f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

	 if(f1ResetMsg->protocolIEs.list.array)
	 {
	    for(idx=0 ;idx < f1ResetMsg->protocolIEs.list.count ; idx++)
	    {
	       if(f1ResetMsg->protocolIEs.list.array[idx])
	       {
		  DU_FREE(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
	       }
	    }
	    DU_FREE(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
	 }
	 DU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}
/*******************************************************************
 *
 * @brief Build and Send F1reset request 
 *
 * @details
 *
 *    Function : BuildAndSendF1ResetReq
 *
 *    Functionality:
 *         - Build and Send F1reset request msg
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendF1ResetReq()
{
   uint8_t          elementCnt=0;
   uint8_t          idx=0;
   uint8_t          ret= RFAILED;
   Reset_t          *f1ResetMsg = NULLP;
   F1AP_PDU_t       *f1apMsg = NULLP;
   asn_enc_rval_t   encRetVal;
   DU_LOG("\nF1AP : Building F1 Reset request \n");
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
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_Reset;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_Reset;

      f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

      elementCnt = 3;
      f1ResetMsg->protocolIEs.list.count = elementCnt;
      f1ResetMsg->protocolIEs.list.size = elementCnt * sizeof(ResetIEs_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
      if(f1ResetMsg->protocolIEs.list.array == NULLP)
      {
	 break;
      }
      for(idx=0; idx<elementCnt; idx++)
      {
	 DU_ALLOC(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
	 if(f1ResetMsg->protocolIEs.list.array[idx] == NULLP)
	 {
	    break;
	 }
      }

      /*TransactionID*/
      idx=0;
      f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
      f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_TransactionID;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.TransactionID = 1;

      /*Cause*/
      idx++;
      f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_Cause;
      f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
      f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_Cause;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.Cause.present = Cause_PR_radioNetwork;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.Cause.choice.radioNetwork = CauseRadioNetwork_action_desirable_for_radio_reasons;

      /*Reset Type*/
      idx++;
      f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_ResetType;
      f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_ResetType;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.ResetType.present = ResetType_PR_f1_Interface;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.ResetType.choice.f1_Interface = ResetAll_reset_all;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	    encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG("\nF1AP : Could not encode F1Reset structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nF1AP : Created APER encoded buffer for F1Reset\n");
	 for(idx=0; idx< encBufSize; idx++)
	 {
	    printf("%x",encBuf[idx]);
	 }
      }

      if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
      {
	 DU_LOG("\nF1AP : Sending F1 Reset request failed");
	 break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeF1ResetReq(f1apMsg);
   return ret;
}
/*******************************************************************
 *
 * @brief Build And Send F1ResetAck
 *
 * @details
 *
 *    Function : BuildAndSendF1ResetAck
 *
 *    Functionality:
 *         - Build And Send  F1ResetRSP
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeF1ResetAck(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx;
   ResetAcknowledge_t *f1ResetAck;

   if(f1apMsg)
   {
      if(f1apMsg->choice.successfulOutcome)
      {
	 f1ResetAck= &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

	 if(f1ResetAck->protocolIEs.list.array)
	 {
	    for(idx=0; idx<f1ResetAck->protocolIEs.list.count ; idx++)
	    {
	       if(f1ResetAck->protocolIEs.list.array[idx])
	       {
		  DU_FREE(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
	       }
	    }
	    DU_FREE(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size );
	 }
	 DU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build And Send F1ResetAck
 *
 * @details
 *
 *    Function : BuildAndSendF1ResetAck
 *
 *    Functionality:
 *         - Build And Send  F1ResetRSP
 *
 *  @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendF1ResetAck()
{
   uint8_t                idx = 0;
   uint8_t                elementCnt = 0;
   uint8_t                ret = RFAILED;
   F1AP_PDU_t             *f1apMsg = NULL;
   ResetAcknowledge_t     *f1ResetAck = NULLP;
   asn_enc_rval_t         encRetVal;
   DU_LOG("\nF1AP : Building F1 Reset Acknowledgment \n");

   do{
      /* Allocate the memory for F1ResetRequest_t */
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }

      f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

      DU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      if(f1apMsg->choice.successfulOutcome == NULLP)
      {
	 DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }
      f1apMsg->choice.successfulOutcome->criticality   = Criticality_reject;
      f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_ResetAcknowledge;
      f1ResetAck = &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

      elementCnt = 1;

      f1ResetAck->protocolIEs.list.count = elementCnt;
      f1ResetAck->protocolIEs.list.size = elementCnt*sizeof(ResetAcknowledgeIEs_t *);

      DU_ALLOC(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size );
      if(f1ResetAck->protocolIEs.list.array == NULLP)
      {
	 DU_LOG("\nF1AP : Memory allocation for F1ResetAckIEs failed");
	 break;
      }

      for(idx=0; idx<elementCnt; idx++)
      {
	 DU_ALLOC(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
	 if(f1ResetAck->protocolIEs.list.array[idx] == NULLP)
	 {
	    break;
	 }
      }
      /*TransactionID*/
      idx = 0;
      f1ResetAck->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
      f1ResetAck->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      f1ResetAck->protocolIEs.list.array[idx]->value.present = ResetAcknowledgeIEs__value_PR_TransactionID;
      f1ResetAck->protocolIEs.list.array[idx]->value.choice.TransactionID = TRANS_ID;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as UPER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG("\nF1AP : Could not encode F1ResetAck structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nF1AP : Created APER encoded buffer for F1ResetAck\n");
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
      }
      /* Sending msg */
      if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
      {
	 DU_LOG("\nF1AP : Sending F1 Reset Acknowledgement failed");
	 break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeF1ResetAck(f1apMsg);
   return ret;
}

/******************************************************************
 *
 * @brief Processes DL RRC Message Transfer  sent by CU
 *
 * @details
 *
 *    Function : procF1ResetReq
 *
 *    Functionality: Processes DL RRC Message Transfer sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1ResetReq(F1AP_PDU_t *f1apMsg)
{
   /* we are currently not supporting Ue release. right now we are supporting only init case of fireset */
   uint8_t       idx = 0;
   uint8_t        ret = ROK;
   Reset_t       *f1ResetMsg = NULLP;

   DU_LOG("\nProcessing F1 reset request");
   f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

   for(idx=0; idx<f1ResetMsg->protocolIEs.list.count; idx++)
   {
      switch(f1ResetMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_TransactionID:
	    break;

	 case ProtocolIE_ID_id_Cause:
	    break;

	 case ProtocolIE_ID_id_ResetType:
	    {
	       DU_LOG("\nReceived F1 Reset request");
	       break;
	    }

	 default:
	    break;
      }
   }
   ret = BuildAndSendF1ResetAck();
   DU_LOG("\nUE release is not supported for now");
   return ret;
}

/*******************************************************************
 *
 * @brief free the RRC delivery report
 *
 * @details
 *
 *    Function : freeRrcDeliveryReport
 *
 *    Functionality: free the RRC delivery report
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeRrcDeliveryReport(F1AP_PDU_t  *f1apMsg )
{
   uint8_t idx=0;
   RRCDeliveryReport_t *rrcDeliveryReport= NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
	 rrcDeliveryReport = &f1apMsg->choice.initiatingMessage->value.choice.RRCDeliveryReport;
	 if(rrcDeliveryReport->protocolIEs.list.array)
	 {
	    for(idx =0 ;idx <rrcDeliveryReport->protocolIEs.list.count ;
		  idx++)
	    {
	       if(rrcDeliveryReport->protocolIEs.list.array[idx])
	       {
		  DU_FREE(rrcDeliveryReport->protocolIEs.list.array[idx],
			sizeof(RRCDeliveryReportIEs_t));
	       }   
	    }
	    DU_FREE(rrcDeliveryReport->protocolIEs.list.array,
		  rrcDeliveryReport->protocolIEs.list.size);
	 }
	 DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg,
	    sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
*
* @brief Builds and sends the RRC delivery report
*
* @details
*
*    Function : BuildAndSendRrcDeliveryReport
*
*    Functionality: Builds and sends the RRC delivery report
*
* @params[in]
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildAndSendRrcDeliveryReport(uint32_t gnbCuUeF1apId, \
   uint32_t gnbDuUeF1apId, RrcDeliveryReport *rrcDelivery)
{
   uint8_t             ret = RFAILED;
   uint8_t             idx    = 0;
   uint8_t             idx1   = 0;
   uint8_t             elementCnt = 0;
   F1AP_PDU_t          *f1apMsg = NULLP;
   asn_enc_rval_t      encRetVal;  
   RRCDeliveryReport_t *rrcDeliveryReport= NULLP;

   do{

      DU_LOG("\nF1AP : Building RRC delivery Message Transfer Message\n");
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
	 DU_LOG(" F1AP : Memory allocation for  F1AP-PDU  failed");
	 break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_RRCDeliveryReport;
      f1apMsg->choice.initiatingMessage->criticality   = Criticality_ignore;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_RRCDeliveryReport;

      rrcDeliveryReport = &f1apMsg->choice.initiatingMessage->value.choice.RRCDeliveryReport;
      elementCnt = 4;
      rrcDeliveryReport->protocolIEs.list.count = elementCnt;
      rrcDeliveryReport->protocolIEs.list.size = elementCnt * sizeof(RRCDeliveryReportIEs_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(rrcDeliveryReport->protocolIEs.list.array, rrcDeliveryReport->protocolIEs.list.size);
      if(rrcDeliveryReport->protocolIEs.list.array == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for RRC Delivery  failed");
	 break;
      }
      for(idx =0 ;idx <elementCnt; idx++)
      {
	 DU_ALLOC(rrcDeliveryReport->protocolIEs.list.array[idx], sizeof(RRCDeliveryReportIEs_t));
	 if(rrcDeliveryReport->protocolIEs.list.array[idx] == NULLP)
	 {
	    break;
	 }
      }

      idx1 = 0;

      /*GNB CU UE F1AP ID*/
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_GNB_CU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.GNB_CU_UE_F1AP_ID = gnbCuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx1++;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_GNB_DU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.GNB_DU_UE_F1AP_ID = gnbDuUeF1apId;

      /*RRC delivery status*/
      idx1++;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_RRCDeliveryStatus;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_ignore;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_RRCDeliveryStatus;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.RRCDeliveryStatus.delivery_status =\
      rrcDelivery->rrcDeliveryStatus.deliveryStatus;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.RRCDeliveryStatus.triggering_message =\
      rrcDelivery->rrcDeliveryStatus.triggeringMessage;

      /* SRB ID */ 
      idx1++;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_SRBID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_ignore;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_SRBID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.SRBID =  rrcDelivery->srbId;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the RRC DELIVERY REPORT type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	    encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG("\nF1AP : Could not encode RRC Delivery Msg structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nF1AP : Created APER encoded buffer for RRC Delivery Msg \n");
	 for(idx=0; idx< encBufSize; idx++)
	 {
	    printf("%x",encBuf[idx]);
	 }
      }

      /* Sending msg */
      if(SendF1APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
      {
	 DU_LOG("\nF1AP : Sending RRC delivery  msg request failed");
	 break;
      }
      ret = ROK;
      break;

   }while(true);

   freeRrcDeliveryReport(f1apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Processes cells to be activated
 *
 * @details
 *
 *    Function : extractCellsToBeActivated
 *
 *    Functionality:
 *      - Processes cells to be activated list received in F1SetupRsp
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t extractCellsToBeActivated(Cells_to_be_Activated_List_t cellsToActivate)
{
   uint8_t  ret = ROK;
   uint16_t idx, nci, pci = 0;
   Cells_to_be_Activated_List_Item_t cell;

   for(idx=0; idx<cellsToActivate.list.count; idx++)
   {
      nci = 0;
      cell = cellsToActivate.list.array[idx]->value.choice.Cells_to_be_Activated_List_Item;
      bitStringToInt(&cell.nRCGI.nRCellIdentity, &nci);

      if(cell.nRPCI)
      {
	 pci = *cell.nRPCI;
      }
      ret = duProcCellsToBeActivated(cell.nRCGI.pLMN_Identity.buf, nci, pci);
   }
   return ret;
}

/******************************************************************
 *
 * @brief Processes F1 Setup Response sent by CU
 *
 * @details
 *
 *    Function : procF1SetupRsp
 *
 *    Functionality: Processes F1 Setup Response sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1SetupRsp(F1AP_PDU_t *f1apMsg)
{
   uint8_t ret = ROK;
   uint16_t idx;
   F1SetupResponse_t *f1SetRspMsg = NULLP;
   GNB_CU_Name_t     *cuName = NULLP;
   RRC_Version_t     *rrc_Ver = NULLP;
   F1SetupRsp  f1SetRspDb;
   memset(&f1SetRspDb, 0, sizeof(F1SetupRsp));

   DU_LOG("\nF1AP : F1 Setup Response received");
   f1SetRspMsg = &f1apMsg->choice.successfulOutcome->value.choice.F1SetupResponse;

   for(idx=0; idx<f1SetRspMsg->protocolIEs.list.count; idx++)
   {
      switch(f1SetRspMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_Cells_to_be_Activated_List:
	    {
	       extractCellsToBeActivated(f1SetRspMsg->protocolIEs.list.array[idx]->\
		     value.choice.Cells_to_be_Activated_List);
	       break;
	    }
	 case ProtocolIE_ID_id_TransactionID:
	    {
	       f1SetRspDb.transId = f1SetRspMsg->protocolIEs.list.array[idx]->\
				    value.choice.TransactionID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_CU_Name:
	    {
	       cuName = &f1SetRspMsg->protocolIEs.list.array[idx]->\
			value.choice.GNB_CU_Name;
	       strcpy(f1SetRspDb.cuName, (const char*)cuName->buf);
	       break;
	    }
	 case ProtocolIE_ID_id_GNB_CU_RRC_Version:
	    {
	       rrc_Ver = &f1SetRspMsg->protocolIEs.list.array[idx]->\
			 value.choice.RRC_Version;
	       strcpy(f1SetRspDb.rrcVersion.rrcVer,
		     (const char*)rrc_Ver->latest_RRC_Version.buf);
	       break;
	    }
	 default:
	    DU_LOG("\nDU_APP : Invalid IE received in F1SetupRsp:%ld",
		  f1SetRspMsg->protocolIEs.list.array[idx]->id);
      }
      duProcF1SetupRsp();
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes GNB DU config update ack
 *
 * @details
 *
 *    Function : procF1GNBDUCfgUpdAck
 *
 *    Functionality: Processes GNB DU config update ack
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1GNBDUCfgUpdAck(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx, transId;
   GNBDUConfigurationUpdateAcknowledge_t *gnbDuAck = NULLP;

   gnbDuAck = &f1apMsg->choice.successfulOutcome->value.choice.GNBDUConfigurationUpdateAcknowledge;
   
   for(ieIdx=0; ieIdx < gnbDuAck->protocolIEs.list.count; ieIdx++)
   {
      switch(gnbDuAck->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_TransactionID:
	    transId = gnbDuAck->protocolIEs.list.array[ieIdx]->\
	       value.choice.TransactionID;
            DU_LOG("\nF1AP : GNB-DU config update acknowledgment received for transId %d", transId);
	    break;
         case ProtocolIE_ID_id_Cells_to_be_Activated_List:
	    break;
         default :
            DU_LOG("\nF1AP: Invalid IE Received: %ld, at procF1GNBDUCfgUpdAck()", \
	    gnbDuAck->protocolIEs.list.array[ieIdx]->id);
	    break;
      }
   }
   duProcGnbDuCfgUpdAckMsg();
#if 0
   if(BuildAndSendF1ResetReq() != ROK)
   {
      return RFAILED;
   }
#endif
   return ROK;
}

/******************************************************************
 *
 * @brief Processes DL RRC Message Transfer  sent by CU
 *
 * @details
 *
 *    Function : procF1DlRrcMsgTrans
 *
 *    Functionality: Processes DL RRC Message Transfer sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1DlRrcMsgTrans(F1AP_PDU_t *f1apMsg)
{
   uint8_t  idx, ret;
   DLRRCMessageTransfer_t *f1DlRrcMsg = NULLP;
   F1DlRrcMsg dlMsg;
   memset(&dlMsg, 0, sizeof(F1DlRrcMsg));

   DU_LOG("\nDU_APP : DL RRC message transfer Recevied");
   f1DlRrcMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;

   ret = ROK;

   for(idx=0; idx<f1DlRrcMsg->protocolIEs.list.count; idx++)
   {
      switch(f1DlRrcMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       dlMsg.gnbCuUeF1apId = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       dlMsg.gnbDuUeF1apId = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_SRBID:
	    {
	       dlMsg.srbId = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.SRBID;
	       break;
	    }
	 case ProtocolIE_ID_id_ExecuteDuplication:
	    dlMsg.execDup = true;
	    break;

	 case ProtocolIE_ID_id_RRCContainer:
	    {
	       if(f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size > 0)
	       {
		  dlMsg.rrcMsgSize = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size;
		  DU_ALLOC(dlMsg.rrcMsgPdu, dlMsg.rrcMsgSize);
		  if(dlMsg.rrcMsgPdu)
		  {
		     memcpy(dlMsg.rrcMsgPdu, f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,\
			dlMsg.rrcMsgSize);
	          }
		  else
		  {
		     DU_LOG("\nDU_APP : Memory alloc Failed at RRC Container at procF1DlRrcMsgTrans()");
                     return RFAILED;
		  }
	       }
	       else
	       {
		  DU_LOG("\nDU_APP : RRC Container Size is invalid:%ld",\
	          f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
		  return RFAILED;
	       }
	       break;
	    }
         case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
	    {
	       dlMsg.deliveryStatRpt = true;
	       break;
	    }
	 default:
	    DU_LOG("\nDU_APP : Invalid IE received in DL RRC Msg Transfer:%ld",
		  f1DlRrcMsg->protocolIEs.list.array[idx]->id);
      }
   }

   ret = duProcDlRrcMsg(&dlMsg);
   return ret;
}

/*****************************************************************i
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
   ODU_PRINT_MSG(mBuf, 0,0);

   /* Copy mBuf into char array to decode it */
   ODU_GET_MSG_LEN(mBuf, &recvBufLen);
   DU_ALLOC(recvBuf, (Size)recvBufLen);

   if(recvBuf == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation failed");
      return;
   }
   if(ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
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
   DU_FREE(recvBuf, (Size)recvBufLen);

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
	       case SuccessfulOutcome__value_PR_ResetAcknowledge:
		  {
		     DU_LOG("\nF1AP : F1ResetAcknowledge is received successfully ");
		     break;
		  }
	       case SuccessfulOutcome__value_PR_F1SetupResponse:
		  {				
#ifndef ODU_TEST_STUB
		     procF1SetupRsp(f1apMsg);
#endif
		     break;
		  }

	       case SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge:
		  {
		     procF1GNBDUCfgUpdAck(f1apMsg);
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
      case F1AP_PDU_PR_initiatingMessage:
	 {
	    switch(f1apMsg->choice.initiatingMessage->value.present)
	    {
	       case InitiatingMessage__value_PR_Reset:
		  {
		     DU_LOG("\nF1AP : F1 reset request received");
		     procF1ResetReq(f1apMsg);
		     break;
		  }
	       case InitiatingMessage__value_PR_DLRRCMessageTransfer:
		  {
		     procF1DlRrcMsgTrans(f1apMsg);
		     break;
		  }
	       case InitiatingMessage__value_PR_UEContextSetupRequest:
		  {
		     procF1UeContextSetupReq(f1apMsg);
		     break;
		  }

	       default:
		  {
		     DU_LOG("\nF1AP : Invalid type of initiating message[%d]",
			   f1apMsg->choice.initiatingMessage->value.present);
		     return;
		  }
	    }/* End of switch(initiatingMessage) */
	    break;
	 }

      default:
	 {
	    DU_LOG("\nF1AP : Invalid type of f1apMsg->present [%d]",f1apMsg->present);
	    return;
	 }

   }/* End of switch(f1apMsg->present) */

} /* End of F1APMsgHdlr */

/**********************************************************************
  End of file
 **********************************************************************/
