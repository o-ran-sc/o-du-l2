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
#include "common_def.h"
#include "du_tmr.h"
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "legtp.h"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_e2ap_mgr.h"
#include "du_e2ap_msg_hdl.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_mgr_main.h"
#include "du_utils.h"
#include "GlobalE2node-gNB-ID.h"
#include "ProtocolIE-FieldE2.h"
#include "E2setupRequest.h"
#include "InitiatingMessageE2.h"
#include "SuccessfulOutcomeE2.h"
#include "UnsuccessfulOutcomeE2.h"
#include "E2AP-PDU.h"
#include "odu_common_codec.h"
#include "E2nodeComponentInterfaceF1.h"
#include "E2setupRequest.h"
#include "du_e2_conversions.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "RANfunction-Name.h"
#include "RIC-EventTriggerStyle-Item.h"
#include "RIC-ReportStyle-Item.h"
#include "MeasurementInfo-Action-Item.h"
#include "E2SM-KPM-EventTriggerDefinition.h"
#include "E2SM-KPM-EventTriggerDefinition-Format1.h"
#include "E2SM-KPM-ActionDefinition.h"
#include "E2SM-KPM-ActionDefinition-Format1.h"
#include "MeasurementInfoItem.h"
#include "RANfunctionsIDcause-List.h"
#include "MeasurementRecord.h"
#include "MeasurementData.h"
#include "MeasurementRecordItem.h"
#include "MeasurementDataItem.h"
#include "E2SM-KPM-IndicationMessage-Format1.h"
#include "E2SM-KPM-IndicationMessage.h"
#include "E2SM-KPM-IndicationHeader.h"
#include "E2SM-KPM-IndicationHeader-Format1.h"
#include "LabelInfoItem.h"

/*******************************************************************
 *
 * @brief Fill E2 Failure Cause
 *
 * @details
 *
 *    Function : fillE2Cause
 *
 *    Functionality: Fill E2 Failure Cause
 *
 * @params[in] E2 Cause pointer to be filled in
 *             E2 Cause to be filled from 
 * @return void
 *
 ******************************************************************/
void fillE2Cause(CauseE2_t *e2Cause, E2FailureCause failureCause)
{
   e2Cause->present = failureCause.causeType;
   switch(e2Cause->present)
   {
      case CauseE2_PR_ricRequest:
         {
            e2Cause->choice.ricRequest = failureCause.cause;
            break;
         }
      case CauseE2_PR_ricService:
         {
            e2Cause->choice.ricService = failureCause.cause;
            break;
         }
      case CauseE2_PR_e2Node:
         {
            e2Cause->choice.e2Node = failureCause.cause;
            break;
         }
      case CauseE2_PR_transport:
         {
            e2Cause->choice.transport = failureCause.cause;
            break;
         }
      case CauseE2_PR_protocol:
         {
            e2Cause->choice.protocol = failureCause.cause;
            break;
         }
      case CauseE2_PR_misc:
         {
            e2Cause->choice.misc = failureCause.cause;
            break;
         }
      case CauseE2_PR_NOTHING:
      default:
         break;
   }
}

/*******************************************************************
 *
 * @brief Free the ErrorIndication Message
 *
 * @details
 *
 *    Function : FreeRicIndication
 *
 * Functionality: Free the ErrorIndication Message
 *
 * @params[in] 
 *    E2AP_PDU is to freed
 * @return void
 *
 ******************************************************************/
void FreeErrorIndication(E2AP_PDU_t  *e2apMsg) 
{
   uint8_t arrIdx = 0;
   ErrorIndicationE2_t *errorIndicationMsg= NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         errorIndicationMsg = &e2apMsg->choice.initiatingMessage->value.choice.ErrorIndicationE2;
         if(errorIndicationMsg!= NULLP)
         {
            if(errorIndicationMsg->protocolIEs.list.array != NULLP)
            {
               for(arrIdx=0; arrIdx<errorIndicationMsg->protocolIEs.list.count; arrIdx++)
               {
                  DU_FREE(errorIndicationMsg->protocolIEs.list.array[arrIdx],sizeof(ErrorIndicationE2_t));
               }
               DU_FREE(errorIndicationMsg->protocolIEs.list.array,errorIndicationMsg->protocolIEs.list.size);
            }
         }
         DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds and Send the ErrorIndication Message
 *
 * @details
 *
 *    Function : BuildAndSendErrorIndication
 *
 * Functionality:Fills the ErrorIndication Message
 *
 * @params[in] 
 *    Trans id
 *    Ric req id
 *    Ran function id
 *    Cause of failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendErrorIndication(int8_t transId, RicRequestId requestId, uint16_t ranFuncId,  E2FailureCause failureCause)
{
   uint8_t elementCnt =0, arrIdx=0, ret = RFAILED;
   E2AP_PDU_t         *e2apMsg = NULLP;
   ErrorIndicationE2_t *errorIndicationMsg=NULLP;
   asn_enc_rval_t     encRetVal;        /* Encoder return value */

   while(true)
   {
      DU_LOG("\nINFO   -->  E2AP : Building Error Indication Message\n");

      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed in %s at line %d",__func__, __LINE__);
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed in %s at line %d",__func__, __LINE__);
         break;
      }
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_ErrorIndicationE2;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_ErrorIndicationE2;

      errorIndicationMsg = &e2apMsg->choice.initiatingMessage->value.choice.ErrorIndicationE2;
      
      /* Element count is 2 for TransactionID/RICrequestID and Cause.
       * If the RAN function id is present, the count will be increased.*/
      elementCnt = 2;
      if(ranFuncId>0)
      {
         elementCnt++;
      }
      
      errorIndicationMsg->protocolIEs.list.count = elementCnt;
      errorIndicationMsg->protocolIEs.list.size = elementCnt * sizeof(ErrorIndicationE2_IEs_t*);

      /* Initialize the E2Setup members */
      DU_ALLOC(errorIndicationMsg->protocolIEs.list.array, errorIndicationMsg->protocolIEs.list.size);
      if(errorIndicationMsg->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for array elements in %s at line %d",__func__, __LINE__);
         break;
      }
      
      for(arrIdx = 0; arrIdx < elementCnt; (arrIdx)++)
      {
         DU_ALLOC(errorIndicationMsg->protocolIEs.list.array[arrIdx], sizeof(ErrorIndicationE2_IEs_t));
         if(errorIndicationMsg->protocolIEs.list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for array [%d] elements in %s at line %d", arrIdx, __func__, __LINE__);
            break;
         }
      }
      if(arrIdx < elementCnt)
         break;

      arrIdx = 0;

      if(transId >=0 && transId<=255)
      {
         /* TransactionID */
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_TransactionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.TransactionID = transId;
      }
      else
      {
         /* RICrequestID */
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RICrequestID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_RICrequestID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.RICrequestID.ricRequestorID = requestId.requestorId;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.RICrequestID.ricInstanceID = requestId.instanceId;
      }
      
      if(ranFuncId>0)
      {
         /* RAN Function ID */
         arrIdx++;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_RANfunctionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionID = ranFuncId;
      }

      /* Cause */
      arrIdx++;
      errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_ignore;
      errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_CauseE2;
      fillE2Cause(&errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.CauseE2, failureCause);

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode Error Indication Message (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for Error Indication Message \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         } 
#endif
      }

      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nINFO   -->  E2AP : Sending Error Indication Message");      

      }
      ret = ROK;
      break;
   }
   FreeErrorIndication(e2apMsg);	
   return ret;
}

/******************************************************************
 *
 * @brief Deallocation of memory allocated by aper decoder for e2 
 * Config Update Failure
 *
 * @details
 *
 *    Function : freeAperDecodingOfE2Node Config UpdateFailure
 *
 *    Functionality: Deallocation of memory allocated by  aper decoder 
 *    for e2 Config Update Failure
 *
 * @params[in] E2nodeConfigurationUpdateFailure_t to be deallocated 
 * @return void
 *
 * ****************************************************************/

void freeAperDecodingOfE2NodeConfigUpdateFailure(E2nodeConfigurationUpdateFailure_t *e2NodeCfgUpdFail)
{
   uint8_t arrIdx =0;

   if(e2NodeCfgUpdFail)
   {
      if(e2NodeCfgUpdFail->protocolIEs.list.array)
      {
         for(arrIdx=0; arrIdx<e2NodeCfgUpdFail->protocolIEs.list.count; arrIdx++)
         {
            if(e2NodeCfgUpdFail->protocolIEs.list.array[arrIdx])
            {
               free(e2NodeCfgUpdFail->protocolIEs.list.array[arrIdx]);
            }
         }
         free(e2NodeCfgUpdFail->protocolIEs.list.array);
      }
   }
}

/******************************************************************
 *
 * @brief Processes E2 Node Config Update Failure sent by RIC
 *
 * @details
 *
 *    Function : procE2NodeConfigUpdateFailure
 *
 *    Functionality: Processes E2 Node Config Update failure sent by RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void procE2NodeConfigUpdateFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx =0, transId =0, timerValue=0;
   E2nodeConfigurationUpdateFailure_t *e2NodeCfgUpdFail=NULL;

   DU_LOG("\nINFO   -->  E2AP : E2 Node Config Update failure received");
   e2NodeCfgUpdFail = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2nodeConfigurationUpdateFailure;

   for(arrIdx=0; arrIdx<e2NodeCfgUpdFail->protocolIEs.list.count; arrIdx++)
   {
      switch(e2NodeCfgUpdFail->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            {
               transId = e2NodeCfgUpdFail->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
               if((duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId == transId) &&\
                     (duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode == e2apMsg->choice.unsuccessfulOutcome->procedureCode))
               {
                  memset(&duCb.e2apDb.e2TransInfo.e2InitTransaction[transId], 0, sizeof(E2TransInfo));
               }
               else
               {
                  DU_LOG("\nERROR  -->  E2AP : Invalid transaction id [%d]", transId);
               }
               break;
         }
         case ProtocolIE_IDE2_id_TimeToWaitE2:
            {
               timerValue = convertE2WaitTimerEnumToValue(e2NodeCfgUpdFail->protocolIEs.list.array[arrIdx]->value.choice.TimeToWaitE2);
               if((duChkTmr((PTR)&(duCb.e2apDb), EVENT_E2_NODE_CONFIG_UPDATE_TMR)) == FALSE)
               {
                  duStartTmr((PTR)&(duCb.e2apDb), EVENT_E2_NODE_CONFIG_UPDATE_TMR, timerValue);
               }
               else
               {
                  DU_LOG("\nERROR   -->  E2AP : EVENT_E2_NODE_CONFIG_UPDATE_TMR timer is already running");
               }
               break;
            }
      }
   }

   freeAperDecodingOfE2NodeConfigUpdateFailure(e2NodeCfgUpdFail);
}

/*******************************************************************
 *
 * @brief Builds Global gNodeB Params
 *
 * @details
 *
 *    Function : BuildGlobalgNBId
 *
 *    Functionality: Building the Plmn and gNB id
 *
 * @params[in] GlobalE2node_gNB_ID_t *gNbId
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildGlobalgNBId(GlobalE2node_gNB_ID_t *gNbId)
{
   uint8_t unused = 0;
   uint8_t byteSize = 4;
   uint8_t gnbId = duCb.gnbId;
   uint8_t ret = ROK;

   /* fill Global gNB ID Id */
   gNbId->global_gNB_ID.plmn_id.size = 3 * sizeof(uint8_t);
   gNbId->global_gNB_ID.plmn_id.buf = NULLP;
   DU_ALLOC(gNbId->global_gNB_ID.plmn_id.buf , gNbId->global_gNB_ID.plmn_id.size);
   if(gNbId->global_gNB_ID.plmn_id.buf == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP: Memory allocation failed for Plmn buffer");
      ret = RFAILED;
   }
   else
   {
      buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
            gNbId->global_gNB_ID.plmn_id.buf);
      gNbId->global_gNB_ID.gnb_id.present = GNB_ID_Choice_PR_gnb_ID;
      /* Allocate Buffer size */
      gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.size = byteSize * sizeof(uint8_t);
      gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.buf = NULLP;
      DU_ALLOC(gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.buf, \
            gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.size);
      if(gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.buf == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP: Memory allocation failed for gnb buffer");
         ret = RFAILED;
      }
      else
      {
         fillBitString(&gNbId->global_gNB_ID.gnb_id.choice.gnb_ID, unused, byteSize, gnbId);
      }
   }

   /* fill gNB-DU ID */ 
   DU_ALLOC( gNbId->gNB_DU_ID, sizeof(GNB_DU_ID_t));
   if(gNbId->gNB_DU_ID == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP: Memory allocation failed for gNB_DU_ID ");
      ret = RFAILED;
   }
   else
   {
      gNbId->gNB_DU_ID->size = sizeof(uint8_t);
      DU_ALLOC( gNbId->gNB_DU_ID->buf, sizeof(uint8_t));
      if(gNbId->gNB_DU_ID->buf)
      {
         gNbId->gNB_DU_ID->buf[0] =duCb.e2apDb.e2NodeId;
      }
      else
      {
         DU_LOG("\nERROR  -->  E2AP: Memory allocation failed for gNB_DU_ID buffer");
         ret = RFAILED;
      }
   }

   return ret;
}

/*******************************************************************
 *
 * @brief fill the E2 node config information
 *
 * @details
 *
 *    Function : fillE2NodeConfig
 *
 *    Functionality: fill E2 node config information
 *
 * @params[in]  
 *       Pointer to e2NodeCfg to be filled
 *       E2 Node Component information
 *       Type of configuration 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillE2NodeConfig(PTR e2NodeCfg, E2NodeComponent *e2NodeComponentInfo, ConfigType configType)
{
   E2nodeComponentInterfaceType_t *interfaceType=NULLP;
   E2nodeComponentID_t *componentID =NULLP;
   E2nodeComponentConfiguration_t *configuration=NULLP;
   E2nodeComponentConfigAddition_Item_t *e2NodeAddItem=NULL;
   E2nodeComponentConfigUpdate_Item_t *e2NodeUpdateItem =NULL;
   E2nodeComponentConfigRemoval_Item_t *e2NodeRemovalItem=NULL;
   
   switch(configType)
   {
      case CONFIG_ADD:
      {
         e2NodeAddItem = (E2nodeComponentConfigAddition_Item_t*)e2NodeCfg;
         interfaceType = &e2NodeAddItem->e2nodeComponentInterfaceType;
         componentID   = &e2NodeAddItem->e2nodeComponentID;
         configuration = &e2NodeAddItem->e2nodeComponentConfiguration; 
         break;
      }
      case CONFIG_MOD:
      {
         e2NodeUpdateItem = (E2nodeComponentConfigUpdate_Item_t *) e2NodeCfg;
         interfaceType = &e2NodeUpdateItem->e2nodeComponentInterfaceType;
         componentID   = &e2NodeUpdateItem->e2nodeComponentID;
         configuration = &e2NodeUpdateItem->e2nodeComponentConfiguration; 
         break;
      }
      case CONFIG_DEL:
      {
         e2NodeRemovalItem = (E2nodeComponentConfigRemoval_Item_t*) e2NodeCfg;
         interfaceType = &e2NodeRemovalItem->e2nodeComponentInterfaceType;
         componentID  = &e2NodeRemovalItem->e2nodeComponentID;
         break;
      }
      default:
      {
         DU_LOG("\nERROR  --> E2AP : Configuration type %d does not supported ", configType);
         return RFAILED;
      }
   }
   /* E2nodeComponentInterfaceType */
   *interfaceType = convertInterfaceToE2ComponentInterfaceType(e2NodeComponentInfo->interfaceType);
   
   /*  We now only support the F1 interface out of these interfaces
    * (NG,XN,E1,F1,W1,S1,X2), therefore only the F1 component identifier was filled in. */
   
   if(*interfaceType == F1)
   {
      /* E2 Node Component ID */
      componentID->present = E2nodeComponentID_PR_e2nodeComponentInterfaceTypeF1;
      DU_ALLOC(componentID->choice.e2nodeComponentInterfaceTypeF1,sizeof(E2nodeComponentInterfaceF1_t));
      if(componentID->choice.e2nodeComponentInterfaceTypeF1 == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at line %d",__func__,__LINE__);
         return RFAILED;
      }
      componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size = sizeof(uint8_t);
      DU_ALLOC(componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf,\
            componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size);

      if(componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at line %d",__func__,__LINE__);
         return RFAILED;
      }
      memcpy(componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf, &e2NodeComponentInfo->componentId,\
            componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size);
   }
  
   if(configType == CONFIG_DEL)
   {
      /* We don't need to fill out the E2 Node Component Request and Response
       * information in the case of CONFIG_DEL, therefore returning ROK from here. */
      return ROK;
   }

   /* E2 Node Component Request Part */
   if(e2NodeComponentInfo->componentRequestPart)
   {
      configuration->e2nodeComponentRequestPart.size = e2NodeComponentInfo->reqBufSize ;
      DU_ALLOC(configuration->e2nodeComponentRequestPart.buf,\
            configuration->e2nodeComponentRequestPart.size);
      if(configuration->e2nodeComponentRequestPart.buf == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at line %d",__func__,__LINE__);
         return RFAILED;
      }

      memcpy(configuration->e2nodeComponentRequestPart.buf,\
            e2NodeComponentInfo->componentRequestPart, configuration->\
            e2nodeComponentRequestPart.size);
   }
   else
   {
      DU_LOG("\nERROR  --> E2AP: componentRequestPart is null ");
      return RFAILED;
   }

   /* E2 Node Component Response Part */
   if(e2NodeComponentInfo->componentResponsePart)
   {
      configuration->e2nodeComponentResponsePart.size = e2NodeComponentInfo->rspBufSize; 
      DU_ALLOC(configuration->e2nodeComponentResponsePart.buf, configuration->e2nodeComponentResponsePart.size);
      if(configuration->e2nodeComponentResponsePart.buf == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at line %d",__func__,__LINE__);
         return RFAILED;
      }
      memcpy(configuration->e2nodeComponentResponsePart.buf,  e2NodeComponentInfo->componentResponsePart, configuration->\
            e2nodeComponentResponsePart.size);
   }
   else
   {
      DU_LOG("\nERROR  --> E2AP: componentResponsePart is null");
      return RFAILED;
   }
   
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds E2 node config addition list 
 *
 * @details
 *
 *    Function : BuildE2NodeConfigAddList
 *
 *    Functionality: Building E2 node config addition list
 *
 * @params[in] 
 *    E2nodeComponentConfigAddition_List_t to be filled
 *    Procedure Code
 *    Count of E2 node to be added in the list    
 *    Received list of E2 node configuration
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildE2NodeConfigAddList(E2nodeComponentConfigAddition_List_t *e2NodeAddList, uint8_t procedureCode, uint16_t count, E2NodeConfigItem *e2NodeList)
{
   uint8_t arrIdx = 0;
   CmLList         *node =NULL;
   E2NodeComponent *e2NodeComponentInfo=NULL;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItemIe=NULL;
   E2nodeComponentConfigAddition_Item_t *e2NodeAddItem=NULL;
   

   /* For ProcedureCodeE2_id_E2setup, the number of E2 node configuration list items is
    * equal to the number of E2 node configuration entries stored in the database.
    * For any other procedure, the E2 node configuration list count is equal
    * to the count of E2 node configuration obtained from the function's caller */

   if(procedureCode == ProcedureCodeE2_id_E2setup)
      e2NodeAddList->list.count = duCb.e2apDb.e2NodeComponentList.count;
   else
      e2NodeAddList->list.count = count;

   e2NodeAddList->list.size = e2NodeAddList->list.count * sizeof(E2nodeComponentConfigAddition_ItemIEs_t *);
   DU_ALLOC(e2NodeAddList->list.array, e2NodeAddList->list.size);
   if(e2NodeAddList->list.array == NULLP)
   {
       DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2NodeConfigAddList %d",__LINE__);
       return RFAILED;
   }

   for(arrIdx = 0; arrIdx< e2NodeAddList->list.count; arrIdx++)
   {
      DU_ALLOC(e2NodeAddList->list.array[arrIdx], sizeof(E2nodeComponentConfigAddition_ItemIEs_t));
      if(e2NodeAddList->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2NodeConfigAddList %d",__LINE__);
         return RFAILED;
      }
      
      if(procedureCode == ProcedureCodeE2_id_E2setup)
      {
         /* Getting all of the E2 node configuration's information from DuCb one by one*/
         if(arrIdx == 0)
         {
            CM_LLIST_FIRST_NODE(&duCb.e2apDb.e2NodeComponentList, node); 
         }
         else
         {
            node = node->next;
         }
         if(!node)
         {
            DU_LOG("\nERROR  --> E2AP : E2 node component list node is null");
            return RFAILED;
         }
         e2NodeComponentInfo = (E2NodeComponent*)node->node;
      }
      else
      {
         /* Getting only those E2 node configuration from DuCb whose interface
          * and action type is present in the received array */
         e2NodeComponentInfo = fetchE2NodeComponentInfo(e2NodeList[arrIdx].interface, e2NodeList[arrIdx].actionType, &node);
      }
      
      if(!e2NodeComponentInfo)
      {
         DU_LOG("\nERROR  --> E2AP : Received null e2NodeComponentInfo at line number %d",__LINE__);
         return RFAILED;
      }

      e2NodeAddItemIe = (E2nodeComponentConfigAddition_ItemIEs_t *) e2NodeAddList->list.array[arrIdx];
      e2NodeAddItemIe->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAddition_Item;
      e2NodeAddItemIe->criticality = CriticalityE2_reject;
      e2NodeAddItemIe->value.present = E2nodeComponentConfigAddition_ItemIEs__value_PR_E2nodeComponentConfigAddition_Item;
      e2NodeAddItem = &e2NodeAddItemIe->value.choice.E2nodeComponentConfigAddition_Item;
      if(fillE2NodeConfig((PTR)e2NodeAddItem, e2NodeComponentInfo, CONFIG_ADD) != ROK)
      {
         DU_LOG("\nERROR  --> E2AP : Failed to fill the E2 node configuration");
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds E2 node config update list 
 *
 * @details
 *
 *    Function : BuildE2NodeConfigUpdateList
 *
 *    Functionality: Building E2 node config update list
 *
 * @params[in] 
 *    E2nodeComponentConfigUpdate_List_t to be filled
 *    Count of E2 node to be update in the list    
 *    Received list of E2 node configuration
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildE2NodeConfigUpdateList(E2nodeComponentConfigUpdate_List_t *e2NodeUpdateList, uint16_t count,  E2NodeConfigItem *updateE2Node)
{
   uint8_t arrIdx = 0;
   CmLList         *node =NULL;
   E2NodeComponent *e2NodeComponentInfo =NULL;
   E2nodeComponentConfigUpdate_ItemIEs_t *e2NodeUpdateItemIe =NULL;
   E2nodeComponentConfigUpdate_Item_t *e2NodeUpdateItem =NULL;

   e2NodeUpdateList->list.count = count;
   e2NodeUpdateList->list.size = e2NodeUpdateList->list.count * sizeof(E2nodeComponentConfigUpdate_ItemIEs_t *);
   DU_ALLOC(e2NodeUpdateList->list.array, e2NodeUpdateList->list.size);
   if(e2NodeUpdateList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2NodeConfigUpdateList %d",__LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< e2NodeUpdateList->list.count; arrIdx++)
   {
      DU_ALLOC(e2NodeUpdateList->list.array[arrIdx], sizeof(E2nodeComponentConfigUpdate_ItemIEs_t));
      if(e2NodeUpdateList->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2NodeConfigUpdateList %d",__LINE__);
         return RFAILED;
      }

      e2NodeComponentInfo= fetchE2NodeComponentInfo(updateE2Node[arrIdx].interface, updateE2Node[arrIdx].actionType, &node);
      if(!e2NodeComponentInfo)
      {
         DU_LOG("\nERROR  --> E2AP : Received null e2NodeComponentInfo at line number %d",__LINE__);
         return RFAILED;
      }

      e2NodeUpdateItemIe = (E2nodeComponentConfigUpdate_ItemIEs_t *) e2NodeUpdateList->list.array[arrIdx];
      e2NodeUpdateItemIe->id = ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate_Item;
      e2NodeUpdateItemIe->criticality = CriticalityE2_reject;
      e2NodeUpdateItemIe->value.present = E2nodeComponentConfigUpdate_ItemIEs__value_PR_E2nodeComponentConfigUpdate_Item;
      e2NodeUpdateItem = &e2NodeUpdateItemIe->value.choice.E2nodeComponentConfigUpdate_Item;

      if(fillE2NodeConfig((PTR)e2NodeUpdateItem, e2NodeComponentInfo, CONFIG_MOD) != ROK)
      {
         DU_LOG("\nERROR  --> E2AP : Failed to fill the E2 node configuration");
         return RFAILED;
      }

   }
   return ROK;

}


/*******************************************************************
 *
 * @brief Builds E2 node config remove list 
 *
 * @details
 *
 *    Function :BuildE2NodeConfigRemoveList 
 *
 *    Functionality: Building E2 node config remove list
 *
 * @params[in] 
 *    E2nodeComponentConfigRemoval_List_t to be filled
 *    Count of E2 node to be remove in the list    
 *    Received list of E2 node configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildE2NodeConfigRemoveList(E2nodeComponentConfigRemoval_List_t *e2NodeRemoveList, uint16_t count,  E2NodeConfigItem *updateE2Node)
{
   uint8_t arrIdx = 0;
   CmLList         *node=NULL;
   E2NodeComponent *e2NodeComponentInfo=NULL;
   E2nodeComponentConfigRemoval_ItemIEs_t *e2NodeRemovalItemIe=NULL;
   E2nodeComponentConfigRemoval_Item_t *e2NodeRemovalItem=NULL;

   e2NodeRemoveList->list.count = count;
   e2NodeRemoveList->list.size = e2NodeRemoveList->list.count * sizeof(E2nodeComponentConfigRemoval_ItemIEs_t *);
   DU_ALLOC(e2NodeRemoveList->list.array, e2NodeRemoveList->list.size);
   if(e2NodeRemoveList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2NodeConfigRemoveList %d",__LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< e2NodeRemoveList->list.count; arrIdx++)
   {
      DU_ALLOC(e2NodeRemoveList->list.array[arrIdx], sizeof(E2nodeComponentConfigRemoval_ItemIEs_t));
      if(e2NodeRemoveList->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2NodeConfigRemoveList %d",__LINE__);
         return RFAILED;
      }

      e2NodeComponentInfo= fetchE2NodeComponentInfo(updateE2Node[arrIdx].interface, updateE2Node[arrIdx].actionType, &node);
      if(!e2NodeComponentInfo)
      {
         DU_LOG("\nERROR  --> E2AP : Received null e2NodeComponentInfo at line number %d",__LINE__);
         return RFAILED;
      }

      e2NodeRemovalItemIe = (E2nodeComponentConfigRemoval_ItemIEs_t *) e2NodeRemoveList->list.array[arrIdx];
      e2NodeRemovalItemIe->id = ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval_Item;
      e2NodeRemovalItemIe->criticality = CriticalityE2_reject;
      e2NodeRemovalItemIe->value.present = E2nodeComponentConfigRemoval_ItemIEs__value_PR_E2nodeComponentConfigRemoval_Item;
      e2NodeRemovalItem = &e2NodeRemovalItemIe->value.choice.E2nodeComponentConfigRemoval_Item;

      if(fillE2NodeConfig((PTR)e2NodeRemovalItem, e2NodeComponentInfo, CONFIG_DEL) != ROK)
      {
         DU_LOG("\nERROR  --> E2AP : Failed to fill the E2 node configuration");
         return RFAILED;
      }

   }
   return ROK;
}
/*******************************************************************
 *
 * @brief deallocation of E2SM_KPM_RANfunction_Description_t
 *
 * @details
 *
 *    Function : freeE2smKpmRanFunctionDefinition
 *
 *    Functionality: deallocation of E2SM_KPM_RANfunction_Description_t
 *
 * @params[in]  E2SM_KPM_RANfunction_Description_t *ranFunctionDefinition
 * @return void
 *
 ******************************************************************/

void freeE2smKpmRanFunctionDefinition(E2SM_KPM_RANfunction_Description_t *ranFunctionDefinition)
{
   MeasurementInfo_Action_Item_t *measInfoList;
   uint8_t eventTriggerIdx, reportStyleIdx, measInfoIdx;
   RANfunction_Name_t *ranFuncName;
   struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List *ricReportStyle;
   struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List *eventTriggerStyle;
   if(ranFunctionDefinition)
   {
      ranFuncName = &ranFunctionDefinition->ranFunction_Name;
      /* Free RAN function Name */     
      DU_FREE(ranFuncName->ranFunction_ShortName.buf,  ranFuncName->ranFunction_ShortName.size);
      DU_FREE(ranFuncName->ranFunction_E2SM_OID.buf, ranFuncName->ranFunction_E2SM_OID.size);
      DU_FREE(ranFuncName->ranFunction_Description.buf, ranFuncName->ranFunction_Description.size);

      /* Sequence of Event Trigger styles */
      eventTriggerStyle = ranFunctionDefinition->ric_EventTriggerStyle_List;
      if(eventTriggerStyle)
      {
         if(eventTriggerStyle->list.array)
         {
            for(eventTriggerIdx =0;eventTriggerIdx<eventTriggerStyle->list.count; eventTriggerIdx++)
            {
               if(eventTriggerStyle->list.array[eventTriggerIdx])
               {
                  DU_FREE(eventTriggerStyle->list.array[eventTriggerIdx]->ric_EventTriggerStyle_Name.buf,\
                        eventTriggerStyle->list.array[eventTriggerIdx]->ric_EventTriggerStyle_Name.size);
                  DU_FREE(eventTriggerStyle->list.array[eventTriggerIdx], sizeof(RIC_EventTriggerStyle_Item_t ));
               }
            }
            DU_FREE(eventTriggerStyle->list.array, eventTriggerStyle->list.size)
         }
         DU_FREE(eventTriggerStyle, sizeof(struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List));
      }
      
      /* Sequence of Report styles */
      ricReportStyle = ranFunctionDefinition->ric_ReportStyle_List;
      if(ricReportStyle)
      {
         if(ricReportStyle->list.array)
         {
            for(reportStyleIdx =0;reportStyleIdx<ricReportStyle->list.count; reportStyleIdx++)
            {
               if(ricReportStyle->list.array[reportStyleIdx])
               {
                  if(ricReportStyle->list.array[reportStyleIdx]->ric_ReportStyle_Name.buf)
                  {
                     DU_FREE(ricReportStyle->list.array[reportStyleIdx]->ric_ReportStyle_Name.buf,\
                           ricReportStyle->list.array[reportStyleIdx]->ric_ReportStyle_Name.size);
                  }
                  if(ricReportStyle->list.array[reportStyleIdx]->measInfo_Action_List.list.array)
                  {
                     for(measInfoIdx=0; measInfoIdx<ricReportStyle->list.array[reportStyleIdx]->measInfo_Action_List.list.count; \
                           measInfoIdx++)
                     {
                        measInfoList = ricReportStyle->list.array[reportStyleIdx]->measInfo_Action_List.list.array[measInfoIdx];
                        if(measInfoList)
                        {
                           DU_FREE(measInfoList->measID, sizeof(long));
                           DU_FREE(measInfoList->measName.buf, measInfoList->measName.size);
                           DU_FREE(measInfoList,sizeof(MeasurementInfo_Action_Item_t)); 
                        }
                     }
                     DU_FREE(measInfoList,ricReportStyle->list.array[reportStyleIdx]->measInfo_Action_List.list.size);
                  }
                  DU_FREE(ricReportStyle->list.array[reportStyleIdx], sizeof(RIC_ReportStyle_Item_t));
               }
            }
            DU_FREE(ricReportStyle->list.array, ricReportStyle->list.size);
         }
         DU_FREE(ricReportStyle, sizeof(struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List));
      }
      DU_FREE(ranFunctionDefinition, sizeof(E2SM_KPM_RANfunction_Description_t)); 
   }
}

/*******************************************************************
 *
 * @brief fill the e2sm ric report style
 *
 * @details
 *
 *    Function : fillRicReportStyle
 *
 *    Functionality: fill the report style
 *
 * @params[in]   RanFunction *ranFuncDb, struct
 * E2SM_KPM_RANfunction_Description__ric_ReportStyle_List *ricReportStyle
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillRicReportStyle(RanFunction *ranFuncDb, struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List *ricReportStyle)
{
   uint8_t styleIdx, measInfoIdx;
   MeasurementInfo_Action_List_t *measInfo;
   CmLList  *node;
   
   ricReportStyle->list.count = ranFuncDb->numOfReportStyleSupported;
   ricReportStyle->list.size = ricReportStyle->list.count * sizeof(RIC_ReportStyle_Item_t*);
   DU_ALLOC(ricReportStyle->list.array, ricReportStyle->list.size);
   if(!ricReportStyle->list.array)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for ranFuncDefinition %d",__LINE__);
      return RFAILED;
   }

   for(styleIdx =0;styleIdx<ricReportStyle->list.count; styleIdx++)
   {
      DU_ALLOC(ricReportStyle->list.array[styleIdx], sizeof(RIC_ReportStyle_Item_t));
      if(!ricReportStyle->list.array[styleIdx])
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         return RFAILED;
      }
      
      /* RIC Report Style Type */
      ricReportStyle->list.array[styleIdx]->ric_ReportStyle_Type = ranFuncDb->reportStyleList[styleIdx].reportStyle.styleType;
      
      /* RIC Report Style Format Type */
      ricReportStyle->list.array[styleIdx]->ric_ActionFormat_Type = ranFuncDb->reportStyleList[styleIdx].reportStyle.formatType;
      
      /* RIC Report Style Name */
      ricReportStyle->list.array[styleIdx]->ric_ReportStyle_Name.size = strlen(ranFuncDb->reportStyleList[styleIdx].reportStyle.name);
      DU_ALLOC(ricReportStyle->list.array[styleIdx]->ric_ReportStyle_Name.buf,\
            ricReportStyle->list.array[styleIdx]->ric_ReportStyle_Name.size);
      if(!ricReportStyle->list.array[styleIdx]->ric_ReportStyle_Name.buf)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         return RFAILED;
      }
      memcpy(ricReportStyle->list.array[styleIdx]->ric_ReportStyle_Name.buf, ranFuncDb->reportStyleList[styleIdx].reportStyle.name,\
            ricReportStyle->list.array[styleIdx]->ric_ReportStyle_Name.size);

      /* RIC Indication Header Format Type*/
      ricReportStyle->list.array[styleIdx]->ric_IndicationHeaderFormat_Type = ranFuncDb->ricIndicationHeaderFormat;

      /* RIC Indication Message Format Type*/
      ricReportStyle->list.array[styleIdx]->ric_IndicationMessageFormat_Type = ranFuncDb->ricIndicationMessageFormat;
      
      /* Measurement Info Action List */
      CmLListCp measInfoList =ranFuncDb->reportStyleList[styleIdx].measurementInfoList;
      if(!measInfoList.count)
      {
         continue;      
      }

      CM_LLIST_FIRST_NODE(&ranFuncDb->reportStyleList[styleIdx].measurementInfoList, node);
      measInfo = &ricReportStyle->list.array[styleIdx]->measInfo_Action_List;

      measInfo->list.count = measInfoList.count; 
      measInfo->list.size =  measInfoList.count*sizeof(MeasurementInfo_Action_Item_t*);
      DU_ALLOC(measInfo->list.array, measInfo->list.size);
      if(!measInfo->list.array)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         return RFAILED;
      }

      for(measInfoIdx=0; measInfoIdx<measInfo->list.count; measInfoIdx++)
      {
         if(!node)
         {
            DU_LOG("\nERROR  --> E2AP: Measurement info node is null");
            return RFAILED;
         }

         DU_ALLOC(measInfo->list.array[measInfoIdx],sizeof(MeasurementInfo_Action_Item_t));  
         if(!measInfo->list.array[measInfoIdx])
         {
            DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
            return RFAILED;
         }
         MeasurementInfoForAction *measInfoForAction= (MeasurementInfoForAction*)node->node;
         DU_ALLOC(measInfo->list.array[measInfoIdx]->measID, sizeof(long));
         if(!measInfo->list.array[measInfoIdx]->measID)
         {
            DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
            return RFAILED;
         }
         
         memcpy(measInfo->list.array[measInfoIdx]->measID, &measInfoForAction->measurementTypeId,sizeof(long));
         measInfo->list.array[measInfoIdx]->measName.size= strlen(measInfoForAction->measurementTypeName);
         DU_ALLOC(measInfo->list.array[measInfoIdx]->measName.buf, measInfo->list.array[measInfoIdx]->measName.size);
         if(!measInfo->list.array[measInfoIdx]->measName.size)
         {
            DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
            return RFAILED;
         }

         memcpy(measInfo->list.array[measInfoIdx]->measName.buf, \
               measInfoForAction->measurementTypeName,\
               measInfo->list.array[measInfoIdx]->measName.size);
         node = node->next;
      }

   }
   return ROK;
}
/*******************************************************************
 *
 * @brief fill the ric event trigger style
 *
 * @details
 *
 *    Function : fillRicEventTriggerStyle
 *
 *    Functionality: fill the ric event trigger style
 *
 * @params[in]   
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillRicEventTriggerStyle(RanFunction *ranFuncDb, struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List *ricEventTriggerStyle)
{
   uint8_t styleIdx;

   ricEventTriggerStyle->list.count = ranFuncDb->numOfEventTriggerStyleSupported;
   ricEventTriggerStyle->list.size = ricEventTriggerStyle->list.count*  sizeof(RIC_EventTriggerStyle_Item_t *);
   DU_ALLOC(ricEventTriggerStyle->list.array, ricEventTriggerStyle->list.size);
   if(!ricEventTriggerStyle->list.array)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for ric_EventTriggerStyle_List %d",__LINE__);
      return RFAILED;
   }

   for(styleIdx =0;styleIdx<ricEventTriggerStyle->list.count; styleIdx++)
   {
      DU_ALLOC(ricEventTriggerStyle->list.array[styleIdx], sizeof(RIC_EventTriggerStyle_Item_t ));
      if(!ricEventTriggerStyle->list.array[styleIdx])
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         return RFAILED;
      }
      ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerStyle_Type = ranFuncDb->eventTriggerStyleList[styleIdx].styleType;

      ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerFormat_Type = ranFuncDb->eventTriggerStyleList[styleIdx].formatType;

      ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerStyle_Name.size = strlen(ranFuncDb->eventTriggerStyleList[styleIdx].name);
      DU_ALLOC(ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerStyle_Name.buf,\
            ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerStyle_Name.size);
      if(!ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerStyle_Name.buf)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         return RFAILED;
      }
      memcpy(ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerStyle_Name.buf,ranFuncDb->eventTriggerStyleList[styleIdx].name,\
            ricEventTriggerStyle->list.array[styleIdx]->ric_EventTriggerStyle_Name.size);
   
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Ran function item
 *
 * @details
 *
 *    Function : BuildRanFunctionItem  
 *
 *    Functionality: Building RAN function item
 *
 * @params[in] 
 *             RAN function item that has to be filled 
 *             Stored RAN Function information
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildRanFunctionItem(RANfunction_Item_t *ranFuncItem, RanFunction *ranFuncDb)
{
   uint8_t ret =RFAILED;
   RANfunctionDefinition_t  *ranFunctionDefinition;
   RANfunction_Name_t *ranFuncName;
   asn_enc_rval_t encRetVal;
   E2SM_KPM_RANfunction_Description_t *ranFuncDefinition;
   
   while(true)
   {
      /* RAN function Id*/
      ranFuncItem->ranFunctionID = ranFuncDb->id;

      /* RAN Function Revision*/
      ranFuncItem->ranFunctionRevision = ranFuncDb->revisionCounter;

      /* RAN function OID*/
      ranFuncItem->ranFunctionOID.size = strlen(ranFuncDb->name.serviceModelOID);
      DU_ALLOC(ranFuncItem->ranFunctionOID.buf, ranFuncItem->ranFunctionOID.size);
      if(!ranFuncItem->ranFunctionOID.buf)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         break;
      }
      memcpy(ranFuncItem->ranFunctionOID.buf, ranFuncDb->name.serviceModelOID, ranFuncItem->ranFunctionOID.size);

      /* RAN function Definition */
      DU_ALLOC(ranFuncDefinition, sizeof(E2SM_KPM_RANfunction_Description_t));
      if(!ranFuncDefinition)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         break;
      }

      /* RAN function Name */
      ranFuncName = &ranFuncDefinition->ranFunction_Name;

      /* RAN function ShortName */
      ranFuncName->ranFunction_ShortName.size = strlen(ranFuncDb->name.shortName); 
      DU_ALLOC(ranFuncName->ranFunction_ShortName.buf,  ranFuncName->ranFunction_ShortName.size);
      if(!ranFuncName->ranFunction_ShortName.buf)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         break;
      }
      memcpy(ranFuncName->ranFunction_ShortName.buf, ranFuncDb->name.shortName, strlen(ranFuncDb->name.shortName));

      /* RAN function E2SM_OID */
      ranFuncName->ranFunction_E2SM_OID.size = strlen(ranFuncDb->name.serviceModelOID);
      DU_ALLOC(ranFuncName->ranFunction_E2SM_OID.buf, ranFuncName->ranFunction_E2SM_OID.size);
      if(!ranFuncName->ranFunction_E2SM_OID.buf)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         break;
      }
      memcpy(ranFuncName->ranFunction_E2SM_OID.buf, ranFuncDb->name.serviceModelOID, ranFuncName->ranFunction_E2SM_OID.size);

      /* RAN Function Name Description */
      ranFuncName->ranFunction_Description.size = strlen(ranFuncDb->name.description);
      DU_ALLOC(ranFuncName->ranFunction_Description.buf, ranFuncName->ranFunction_Description.size);
      if(!ranFuncName->ranFunction_Description.buf)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         break;
      }
      memcpy(ranFuncName->ranFunction_Description.buf, ranFuncDb->name.description, ranFuncName->ranFunction_Description.size);

      /* RIC Event Trigger Style List */
      DU_ALLOC(ranFuncDefinition->ric_EventTriggerStyle_List, sizeof(struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List));
      if(!ranFuncDefinition->ric_EventTriggerStyle_List)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         break;
      }

      if(fillRicEventTriggerStyle(ranFuncDb, ranFuncDefinition->ric_EventTriggerStyle_List)!=ROK)
      {
         DU_LOG("\nERROR  --> E2AP: failed to fill ric event trigger style");
         break;
      }

      /* RIC Report Style List */
      DU_ALLOC(ranFuncDefinition->ric_ReportStyle_List, sizeof(struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List));
      if(!ranFuncDefinition->ric_ReportStyle_List)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in function %s at %d",__func__,__LINE__);
         break;
      }
      if(fillRicReportStyle(ranFuncDb, ranFuncDefinition->ric_ReportStyle_List) != ROK)
      {
         DU_LOG("\nERROR  --> E2AP: failed to fill ric report style");
         break;
      }

      /* Encode the F1SetupRequest type as APER */
      xer_fprint(stdout, &asn_DEF_E2SM_KPM_RANfunction_Description, ranFuncDefinition);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2SM_KPM_RANfunction_Description, 0, ranFuncDefinition, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode RAN function definition  (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for RAN function definition \n");
         for(uint8_t measIeIdx=0; measIeIdx< encBufSize; measIeIdx++)
         {
            printf("%x",encBuf[measIeIdx]);
         }
         ranFunctionDefinition = &ranFuncItem->ranFunctionDefinition; 
         ranFunctionDefinition->size = encBufSize;
         DU_ALLOC(ranFunctionDefinition->buf, encBufSize);
         if(ranFunctionDefinition->buf == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for RAN function definition buffer");
            break;
         }
         memcpy(ranFunctionDefinition->buf, &encBuf, encBufSize);
         ret = ROK;
         break;
      }
   }
   freeE2smKpmRanFunctionDefinition(ranFuncDefinition);
   return ret;
}

/*******************************************************************
 *
 * @brief Builds Ran function add list based on the procedure code
 *
 * @details
 *
 *    Function : BuildRanFunctionAddList 
 *
 *    Functionality: Building RAN addition addition list
 *       In case of ProcedureCodeE2_id_E2setup we add all the RAN Function list
 *       which is present in E2 database.
 *       In the case of other procedures, we just fill the RAN functions whose ID 
 *       is contained in recvList
 *
 * @params[in] 
 *       RAN Function list
 *       Procedure code
 *       Count of ran functions to be added in the list
 *       Received list of RAN functions
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildRanFunctionAddList(RANfunctions_List_t *ranFunctionsList, uint8_t procedureCode, uint8_t count, RanFuncInfo *recvList)
{
   uint16_t id;
   RanFunction *ranFuncDb;
   uint8_t ranFuncIdx;
   RANfunction_ItemIEs_t *ranFuncItemIe;
   
   /* For ProcedureCodeE2_id_E2setup, the number of RAN function list items is
    * equal to the number of ran function entries stored in the database.
    * For any other procedure, the RAN function list count is equal
    * to the count of ran functions obtained from the function's caller */

   if(procedureCode == ProcedureCodeE2_id_E2setup)
      ranFunctionsList->list.count = duCb.e2apDb.numOfRanFunction;
   else
      ranFunctionsList->list.count = count;

   ranFunctionsList->list.size = ranFunctionsList->list.count * sizeof(RANfunction_ItemIEs_t*);
   DU_ALLOC(ranFunctionsList->list.array, ranFunctionsList->list.size);
   if(ranFunctionsList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed in %s at %d",__func__, __LINE__);
      return RFAILED;
   }

   for(ranFuncIdx = 0; ranFuncIdx< ranFunctionsList->list.count; ranFuncIdx++)
   {
      DU_ALLOC(ranFunctionsList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
      if(ranFunctionsList->list.array[ranFuncIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in %s at %d",__func__, __LINE__);
         return RFAILED;
      }
      if(procedureCode == ProcedureCodeE2_id_E2setup) 
      {
         /* Getting all of the RAN function's information from DuCb one by one*/
         ranFuncDb = &duCb.e2apDb.ranFunction[ranFuncIdx];
      }
      else
      {
         /* Getting only the RAN function information from DuCb whose Id is
          * present in the received array */
         id =recvList[ranFuncIdx].id;
         ranFuncDb = &duCb.e2apDb.ranFunction[id-1];
      }
      ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFunctionsList->list.array[ranFuncIdx];
      ranFuncItemIe->id = ProtocolIE_IDE2_id_RANfunction_Item;
      ranFuncItemIe->criticality = CriticalityE2_ignore;
      ranFuncItemIe->value.present = RANfunction_ItemIEs__value_PR_RANfunction_Item;
      BuildRanFunctionItem(&ranFuncItemIe->value.choice.RANfunction_Item, ranFuncDb);
   }
   return ROK;
}   

/*******************************************************************
 *
 * @brief De Allocate E2 Setup Request Message
 *
 * @details
 *
 *    Function : FreeE2SetupReq
 *
 *    Functionality: De-Allocating E2 Setup request Message
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 
 * @return void
 *
 * ****************************************************************/

void FreeE2SetupReq(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0;
   uint8_t e2NodeAddListIdx =0, ranFuncAddListIdx;
   E2setupRequest_t *e2SetupReq;
   E2nodeComponentConfigAddition_List_t *e2NodeAddList;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItem;
   RANfunctions_List_t *ranFunctionsList;
   RANfunction_ItemIEs_t *ranFuncItemIe;
   RANfunction_Item_t  *ranFunItem;

   /* De-allocating Memory */
   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         e2SetupReq = &e2apMsg->choice.initiatingMessage->value.choice.E2setupRequest; 
         if(e2SetupReq->protocolIEs.list.array != NULLP)
         {
            for(arrIdx = 0; arrIdx < e2SetupReq->protocolIEs.list.count; arrIdx++)
            {
               if(e2SetupReq->protocolIEs.list.array[arrIdx] != NULLP)
               {
                  switch(e2SetupReq->protocolIEs.list.array[arrIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_TransactionID:
                          break;
                     case ProtocolIE_IDE2_id_GlobalE2node_ID:
                        {
                           if(e2SetupReq->protocolIEs.list.array[arrIdx]->\
                                 value.choice.GlobalE2node_ID.choice.gNB != NULLP)
                           {
                              GlobalE2node_gNB_ID_t *gNbId = NULLP;
                              gNbId = e2SetupReq->protocolIEs.list.array[arrIdx]->\
                                      value.choice.GlobalE2node_ID.choice.gNB;
                              if(gNbId->global_gNB_ID.plmn_id.buf != NULLP)
                              {
                                 DU_FREE(gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.buf,\
                                       gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.size);
                                 DU_FREE(gNbId->global_gNB_ID.plmn_id.buf,\
                                       gNbId->global_gNB_ID.plmn_id.size);
                              }

                              if(gNbId->gNB_DU_ID != NULLP)
                              {
                                 DU_FREE( gNbId->gNB_DU_ID->buf, gNbId->gNB_DU_ID->size);
                                 DU_FREE(gNbId->gNB_DU_ID, sizeof(GNB_DU_ID_t));
                              }
                              DU_FREE(e2SetupReq->protocolIEs.list.array[arrIdx]->value.\
                                    choice.GlobalE2node_ID.choice.gNB, sizeof(GlobalE2node_gNB_ID_t));
                           }
                           break;
                        }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigAddition:
                     {
                         e2NodeAddList = &e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAddition_List;
                         if(e2NodeAddList->list.array)
                         {
                             for(e2NodeAddListIdx = 0; e2NodeAddListIdx< e2NodeAddList->list.count; e2NodeAddListIdx++)
                             {
                                e2NodeAddItem = (E2nodeComponentConfigAddition_ItemIEs_t *) e2NodeAddList->list.array[e2NodeAddListIdx];
                                
                                /* Free E2 Node Component Request Part */
                                DU_FREE(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentConfiguration.e2nodeComponentRequestPart.buf,\
                                      e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentConfiguration.e2nodeComponentRequestPart.size);
                                
                                /* Free E2 Node Component Response Part */
                                DU_FREE(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentConfiguration.\
                                      e2nodeComponentResponsePart.buf, \
                                      e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentConfiguration.e2nodeComponentResponsePart.size);
                                 
                                 /* Free E2 Node Component ID */
                                if(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1)
                                {
                                    DU_FREE(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.\
                                    e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf,\
                                    e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.\
                                    e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size);
                                    DU_FREE(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1,\
                                    sizeof(E2nodeComponentInterfaceF1_t));
                                }
                                DU_FREE(e2NodeAddList->list.array[e2NodeAddListIdx], sizeof(E2nodeComponentConfigAddition_ItemIEs_t));
                             }
                             DU_FREE(e2NodeAddList->list.array, e2NodeAddList->list.size);
                         }
                         break;
                     }
                     case ProtocolIE_IDE2_id_RANfunctionsAdded:
                     {
                        ranFunctionsList = &(e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List);  
                        if(ranFunctionsList->list.array)
                        {  
                           for(ranFuncAddListIdx= 0; ranFuncAddListIdx< ranFunctionsList->list.count; ranFuncAddListIdx++)
                           {
                              if(ranFunctionsList->list.array[ranFuncAddListIdx])
                              {
                                 ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFunctionsList->list.array[ranFuncAddListIdx];
                                 ranFunItem = &ranFuncItemIe->value.choice.RANfunction_Item;
                                 DU_FREE(ranFunItem->ranFunctionOID.buf, ranFunItem->ranFunctionOID.size);
                                 DU_FREE(ranFunItem->ranFunctionDefinition.buf, ranFunItem->ranFunctionDefinition.size);
                                 DU_FREE(ranFunctionsList->list.array[ranFuncAddListIdx], sizeof(RANfunction_ItemIEs_t));
                              }
                           }
                           DU_FREE(ranFunctionsList->list.array, ranFunctionsList->list.size);
                        }
                        break;
                     }

                     default:
                        DU_LOG("\nERROR  --> E2AP: Invalid event at e2SetupRequet %ld ",\
                              (e2SetupReq->protocolIEs.list.array[arrIdx]->id));
                        break;
                  }
                  DU_FREE(e2SetupReq->protocolIEs.list.array[arrIdx], sizeof(E2setupRequestIEs_t));
               }
            }
            DU_FREE(e2SetupReq->protocolIEs.list.array, e2SetupReq->protocolIEs.list.size);
         }
         DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds and Send the E2SetupRequest
 *
 * @details
 *
 *    Function : BuildAndSendE2SetupReq
 *
 * Functionality:Fills the E2SetupRequest
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendE2SetupReq()
{
   uint8_t arrIdx = 0, elementCnt=0;
   uint8_t transId = 0, ret = ROK;
   bool memAllocFailed;
   E2AP_PDU_t        *e2apMsg = NULLP;
   E2setupRequest_t  *e2SetupReq = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Setup Request\n");
   do
   {
      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_E2setup;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_E2setupRequest;
      e2SetupReq = &e2apMsg->choice.initiatingMessage->value.choice.E2setupRequest;

      elementCnt = 4;
      e2SetupReq->protocolIEs.list.count = elementCnt;
      e2SetupReq->protocolIEs.list.size = elementCnt * sizeof(E2setupRequestIEs_t*);

      /* Initialize the E2Setup members */
      DU_ALLOC(e2SetupReq->protocolIEs.list.array, \
            e2SetupReq->protocolIEs.list.size);
      if(e2SetupReq->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for array elements");
         break;
      }
      for(arrIdx = 0; arrIdx < elementCnt; (arrIdx)++)
      {
         DU_ALLOC(e2SetupReq->protocolIEs.list.array[arrIdx],\
               sizeof(E2setupRequestIEs_t));
         if(e2SetupReq->protocolIEs.list.array[arrIdx] == NULLP)
         {
            memAllocFailed = true;
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for arrayarrIdx [%d]", arrIdx);
            break;
         }
      }
      if(memAllocFailed == true)
         break;

      arrIdx = 0;

      /* TransactionID */
      e2SetupReq->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      e2SetupReq->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2SetupReq->protocolIEs.list.array[arrIdx]->value.present = E2setupRequestIEs__value_PR_TransactionID;
      transId = assignTransactionId();
      e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.TransactionID = transId;

      arrIdx++;
      /* GlobalE2node_gNB_ID */
      e2SetupReq->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_GlobalE2node_ID;
      e2SetupReq->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2SetupReq->protocolIEs.list.array[arrIdx]->value.present = E2setupRequestIEs__value_PR_GlobalE2node_ID;
      e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.GlobalE2node_ID.present = GlobalE2node_ID_PR_gNB;

      DU_ALLOC(e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.\
            GlobalE2node_ID.choice.gNB, sizeof(GlobalE2node_gNB_ID_t));
      if(e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.\
            GlobalE2node_ID.choice.gNB == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for gNbId");
         break;
      }
      else
      {
         ret = BuildGlobalgNBId(e2SetupReq->protocolIEs.list.array[arrIdx]->value.\
               choice.GlobalE2node_ID.choice.gNB);
         if(ret != ROK)
         {
             DU_LOG("\nERROR  -->  E2AP : Failed to build Global Gnb Id");
             break;
         }
      }
      
      /* RAN Functions Added List */
      arrIdx++;
      e2SetupReq->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsAdded;
      e2SetupReq->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2SetupReq->protocolIEs.list.array[arrIdx]->value.present = E2setupRequestIEs__value_PR_RANfunctions_List;
      if(BuildRanFunctionAddList(&(e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List), ProcedureCodeE2_id_E2setup, 0, NULL)!=ROK)      
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to create RAN Function");
         break;
      }

      /* E2 Node Component Configuration Addition List */
      arrIdx++;
      e2SetupReq->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAddition;
      e2SetupReq->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2SetupReq->protocolIEs.list.array[arrIdx]->value.present = E2setupRequestIEs__value_PR_E2nodeComponentConfigAddition_List;
      if(BuildE2NodeConfigAddList(&(e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAddition_List), ProcedureCodeE2_id_E2setup, 0, NULL)!=ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to create E2 Node config list");
         break;
      }



      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2SetupRequest structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for E2SetupRequest\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }
      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 Setup request failed");
      }
      break;
   }while(true);

   duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId = transId;
   duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode = e2apMsg->choice.initiatingMessage->procedureCode;
   
   FreeE2SetupReq(e2apMsg);
   return ret;
}/* End of BuildAndSendE2SetupReq */

/*******************************************************************
 *
 * @brief Builds RIC Action Admitted List
 *
 * @details
 *
 *    Function : BuildRicActionAdmitList
 *
 *    Functionality: Builds RIC Action Admitted List
 *
 * @params[in] Pointer to RIC Action Admitted List to be filled
 *             Subscription Response information
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRicActionAdmitList(RICaction_Admitted_List_t *admitList, PendingSubsRspInfo *subsRspInfo)
{
   uint8_t idx = 0;
   uint8_t elementCnt = 0;  
   RICaction_Admitted_ItemIEs_t *admitItem = NULLP;

   elementCnt = subsRspInfo->numOfAcceptedActions;

   admitList->list.count = elementCnt;
   admitList->list.size  = elementCnt * sizeof(RICaction_Admitted_ItemIEs_t *);

   DU_ALLOC(admitList->list.array, admitList->list.size);
   if(admitList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      DU_ALLOC(admitList->list.array[idx], sizeof(RICaction_Admitted_ItemIEs_t));
      if(admitList->list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         return RFAILED;
      }

      admitItem = (RICaction_Admitted_ItemIEs_t *)admitList->list.array[idx];
      admitItem->id = ProtocolIE_IDE2_id_RICaction_Admitted_Item;
      admitItem->criticality = CriticalityE2_reject;
      admitItem->value.present = RICaction_Admitted_ItemIEs__value_PR_RICaction_Admitted_Item;
      admitItem->value.choice.RICaction_Admitted_Item.ricActionID = subsRspInfo->acceptedActionList[idx]; 
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RIC Action Not Admitted List
 *
 * @details
 *
 *    Function : BuildRicActionNotAdmitList
 *
 *    Functionality: Builds RIC Action Not Admitted List
 *
 * @params[in] Pointer to RIC Action Not Admitted List to be filled
 *             Subscription Response information
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRicActionNotAdmitList(RICaction_NotAdmitted_List_t *notAdmitList, PendingSubsRspInfo *subsRspInfo)
{
   uint8_t idx = 0;
   uint8_t elementCnt = 0;  
   RICaction_NotAdmitted_ItemIEs_t *notAdmitItem = NULLP;

   elementCnt = subsRspInfo->numOfRejectedActions;

   notAdmitList->list.count = elementCnt;
   notAdmitList->list.size  = elementCnt * sizeof(RICaction_NotAdmitted_ItemIEs_t *);

   DU_ALLOC(notAdmitList->list.array, notAdmitList->list.size);
   if(notAdmitList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      DU_ALLOC(notAdmitList->list.array[idx], sizeof(RICaction_NotAdmitted_ItemIEs_t));
      if(notAdmitList->list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         return RFAILED;
      }

      notAdmitItem = (RICaction_NotAdmitted_ItemIEs_t *)notAdmitList->list.array[idx];
      notAdmitItem->id = ProtocolIE_IDE2_id_RICaction_NotAdmitted_Item;
      notAdmitItem->criticality = CriticalityE2_reject;
      notAdmitItem->value.present = RICaction_NotAdmitted_ItemIEs__value_PR_RICaction_NotAdmitted_Item;
      notAdmitItem->value.choice.RICaction_NotAdmitted_Item.ricActionID = \
         subsRspInfo->rejectedActionList[idx].id; 
      fillE2Cause(&notAdmitItem->value.choice.RICaction_NotAdmitted_Item.cause, \
         subsRspInfo->rejectedActionList[idx].failureCause);
   }
   return ROK;
}

/*******************************************************************
 *
 * @breif Deallocation of BuildAndSendRicSubscriptionRsp memory
 *
 * @details
 *
 *    Function : FreeRicSubscriptionRsp
 *
 * Functionality:Free the RicSubscriptionRsp
 *
 * @param[in] E2AP_PDU_t *e2apRicMsg
 *
 * @return void
 *      
 ******************************************************************/
void FreeRicSubscriptionRsp(E2AP_PDU_t  *e2apRicMsg)
{
   RICsubscriptionResponse_t  *ricSubscriptionRsp= NULLP;
   uint8_t idx=0;
   uint8_t listIdx=0;
   RICaction_Admitted_List_t *admitList = NULLP;
   RICaction_NotAdmitted_List_t *notAdmitList = NULLP;

   if(e2apRicMsg != NULLP)
   {
      if(e2apRicMsg->choice.successfulOutcome != NULLP)
      {
         ricSubscriptionRsp = &e2apRicMsg->choice.successfulOutcome->value.choice.RICsubscriptionResponse;
         if(ricSubscriptionRsp)
         {
            if(ricSubscriptionRsp->protocolIEs.list.array != NULLP)
            {
               for(idx=0; idx<ricSubscriptionRsp->protocolIEs.list.count; idx++)
               {
                  if(ricSubscriptionRsp->protocolIEs.list.array[idx] != NULLP)
                  {
                     switch(ricSubscriptionRsp->protocolIEs.list.array[idx]->id)
                     {
                        case ProtocolIE_IDE2_id_RICactions_Admitted:
                           {
                              admitList = &ricSubscriptionRsp->protocolIEs.list.\
                                             array[idx]->value.choice.RICaction_Admitted_List;
                              if(admitList->list.array != NULLP)
                              {
                                 for(listIdx=0 ; listIdx < admitList->list.count; listIdx++)
                                 {
                                    DU_FREE(admitList->list.array[listIdx], sizeof(RICaction_Admitted_ItemIEs_t));
                                 }
                                 DU_FREE(admitList->list.array, admitList->list.size);	 
                              }
                              break;
                           }
                        case ProtocolIE_IDE2_id_RICactions_NotAdmitted:
                           {
                              notAdmitList = &ricSubscriptionRsp->protocolIEs.list.\
                                             array[idx]->value.choice.RICaction_NotAdmitted_List;
                              if(notAdmitList->list.array != NULLP)
                              {
                                 for(listIdx=0 ; listIdx < notAdmitList->list.count; listIdx++)
                                 {
                                    DU_FREE(notAdmitList->list.array[listIdx], sizeof(RICaction_NotAdmitted_ItemIEs_t));
                                 }
                                 DU_FREE(notAdmitList->list.array, notAdmitList->list.size);	 
                              }
                              break;
                           }
                        default:
                           break;
                     }
                     DU_FREE(ricSubscriptionRsp->protocolIEs.list.array[idx], sizeof(RICsubscriptionResponse_IEs_t));
                  }
               }
               DU_FREE(ricSubscriptionRsp->protocolIEs.list.array, ricSubscriptionRsp->protocolIEs.list.size);
            }
         }   
         DU_FREE(e2apRicMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }		
      DU_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));	      
   }
}

/*******************************************************************
 *
 * @brief Fill RIC Subscription Response IEs
 *
 * @details
 *
 *    Function : fillRicSubscriptionRsp
 *
 * functionality: Fill RIC Subscription Response IEs
 *
 * @param  Pointer to RIC subscription response
 *         Subscription response information
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillRicSubscriptionRsp(RICsubscriptionResponse_t *ricSubscriptionRsp, PendingSubsRspInfo *subsRspInfo)
{
   uint8_t ieIdx = 0;
   uint8_t elementCnt = 0;
   RICsubscriptionResponse_IEs_t *subsRspIe = NULLP;

   elementCnt = 3;
   if(subsRspInfo->numOfRejectedActions)
      elementCnt++;

   ricSubscriptionRsp->protocolIEs.list.count = elementCnt;
   ricSubscriptionRsp->protocolIEs.list.size  = elementCnt * sizeof(RICsubscriptionResponse_IEs_t);
   DU_ALLOC(ricSubscriptionRsp->protocolIEs.list.array, ricSubscriptionRsp->protocolIEs.list.size);
   if(ricSubscriptionRsp->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at %s : line %d", __func__, __LINE__);
      return RFAILED;
   }

   for(ieIdx=0; ieIdx<ricSubscriptionRsp->protocolIEs.list.count; ieIdx++)
   {
      DU_ALLOC(ricSubscriptionRsp->protocolIEs.list.array[ieIdx], sizeof(RICsubscriptionResponse_IEs_t));
      if(ricSubscriptionRsp->protocolIEs.list.array[ieIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d] : ieIdx [%d]", __func__, __LINE__,ieIdx);
         return RFAILED;
      }
   }

   /* RIC Request ID */
   ieIdx=0;
   subsRspIe = ricSubscriptionRsp->protocolIEs.list.array[ieIdx];
   subsRspIe->id = ProtocolIE_IDE2_id_RICrequestID;
   subsRspIe->criticality = CriticalityE2_reject;
   subsRspIe->value.present = RICsubscriptionRequest_IEs__value_PR_RICrequestID;
   subsRspIe->value.choice.RICrequestID.ricRequestorID = subsRspInfo->requestId.requestorId;
   subsRspIe->value.choice.RICrequestID.ricInstanceID = subsRspInfo->requestId.instanceId;

   /* RAN Function ID */
   ieIdx++;
   subsRspIe = ricSubscriptionRsp->protocolIEs.list.array[ieIdx];
   subsRspIe->id = ProtocolIE_IDE2_id_RANfunctionID;
   subsRspIe->criticality = CriticalityE2_reject;
   subsRspIe->value.present = RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
   subsRspIe->value.choice.RANfunctionID = subsRspInfo->ranFuncId;

   /* RIC Action Admitted List */
   ieIdx++;
   subsRspIe = ricSubscriptionRsp->protocolIEs.list.array[ieIdx];
   subsRspIe->id = ProtocolIE_IDE2_id_RICactions_Admitted;
   subsRspIe->criticality = CriticalityE2_reject;
   subsRspIe->value.present = RICsubscriptionResponse_IEs__value_PR_RICaction_Admitted_List;
   if(BuildRicActionAdmitList(&subsRspIe->value.choice.RICaction_Admitted_List, subsRspInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to fill RIC Action Admitted List in RIC Subscription Response");
      return RFAILED;
   }

   /* RIC Action Not Admitted List */
   if(subsRspInfo->numOfRejectedActions)
   {
      ieIdx++;
      subsRspIe = ricSubscriptionRsp->protocolIEs.list.array[ieIdx];
      subsRspIe->id = ProtocolIE_IDE2_id_RICactions_NotAdmitted;
      subsRspIe->criticality = CriticalityE2_reject;
      subsRspIe->criticality = CriticalityE2_reject;
      subsRspIe->value.present = RICsubscriptionResponse_IEs__value_PR_RICaction_NotAdmitted_List;
      if(BuildRicActionNotAdmitList(&subsRspIe->value.choice.RICaction_NotAdmitted_List, subsRspInfo) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to fill RIC Action Not Admitted List in RIC Subscription Response");
         return RFAILED;
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and Send the RicSubscriptionRsp
 *
 * @details
 *
 *    Function : BuildAndSendRicSubscriptionRsp
 *
 * Functionality:Fills the RicSubscriptionRsp
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendRicSubscriptionRsp(PendingSubsRspInfo *subsRspInfo)
{
   uint8_t      ret = RFAILED;
   E2AP_PDU_t   *e2apRicMsg = NULLP;
   RICsubscriptionResponse_t  *ricSubscriptionRsp=NULLP;
   asn_enc_rval_t encRetVal; 

   while(true)
   {
      DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Response\n");

      DU_ALLOC(e2apRicMsg, sizeof(E2AP_PDU_t)); 
      if(e2apRicMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apRicMsg->present =  E2AP_PDU_PR_successfulOutcome;
      DU_ALLOC(e2apRicMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      if(e2apRicMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for RIC subscription Response failed");
         break;
      }

      e2apRicMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_RICsubscription;
      e2apRicMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apRicMsg->choice.successfulOutcome->value.present = SuccessfulOutcomeE2__value_PR_RICsubscriptionResponse;

      ricSubscriptionRsp = &e2apRicMsg->choice.successfulOutcome->value.choice.RICsubscriptionResponse;

      if(fillRicSubscriptionRsp(ricSubscriptionRsp, subsRspInfo) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICsubscriptionResponseIE failed");
         break;
      }

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apRicMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apRicMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC Subscription Response structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for RIC subscription response \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         } 
#endif
      } 

      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending RIC Subscription Response failed");      
         break;
      }

      ret = ROK;
      break;

   }

   FreeRicSubscriptionRsp(e2apRicMsg);
   return ret;
}

/******************************************************************
 *
 * @brief Deallocation of memory allocated by aper decoder for e2 setup response
 *
 * @details
 *
 *    Function : freeAperDecodingOfE2SetupRsp
 *
 *    Functionality: Deallocation of memory allocated by aper decoder for e2
 *    setup response
 *
 * @params[in] E2setupResponse_t *e2SetRspMsg;
 * @return void
 *
 * ****************************************************************/
void freeAperDecodingOfE2SetupRsp(E2setupResponse_t *e2SetRspMsg)
{
   uint8_t arrIdx, e2NodeConfigAddAckListIdx;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeAddAckItem;
   E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAddAckList;

   if(e2SetRspMsg)
   {
      if(e2SetRspMsg->protocolIEs.list.array)
      {
         for(arrIdx=0; arrIdx<e2SetRspMsg->protocolIEs.list.count; arrIdx++)
         {
            if(e2SetRspMsg->protocolIEs.list.array[arrIdx])
            {
               switch(e2SetRspMsg->protocolIEs.list.array[arrIdx]->id)
               {
                  case ProtocolIE_IDE2_id_TransactionID:
                     break;

                  case ProtocolIE_IDE2_id_GlobalRIC_ID:
                     {
                        free(e2SetRspMsg->protocolIEs.list.array[arrIdx]->value.choice.GlobalRIC_ID.pLMN_Identity.buf);
                        free(e2SetRspMsg->protocolIEs.list.array[arrIdx]->value.choice.GlobalRIC_ID.ric_ID.buf);
                        break;
                     }

                  case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
                     {
                        e2NodeConfigAddAckList = &e2SetRspMsg->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAdditionAck_List;
                        if(e2NodeConfigAddAckList->list.array )
                        {
                           for(e2NodeConfigAddAckListIdx = 0; e2NodeConfigAddAckListIdx< e2NodeConfigAddAckList->list.count; e2NodeConfigAddAckListIdx++)
                           {
                              if(e2NodeConfigAddAckList->list.array[e2NodeConfigAddAckListIdx])
                              {
                                 e2NodeAddAckItem = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) e2NodeConfigAddAckList->list.array[e2NodeConfigAddAckListIdx];
                                 free(e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.\
                                       e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf);
                                 free(e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.\
                                       e2nodeComponentInterfaceTypeF1);
                                 free(e2NodeConfigAddAckList->list.array[e2NodeConfigAddAckListIdx]);
                              }
                           }
                           free(e2NodeConfigAddAckList->list.array);
                        }
                        break;
                     }
               }
               free(e2SetRspMsg->protocolIEs.list.array[arrIdx]);  
            }
         }
         free(e2SetRspMsg->protocolIEs.list.array);
      }
   }
}
/******************************************************************
 *
 * @brief Processes E2 Setup Response sent by RIC
 *
 * @details
 *
 *    Function : procE2SetupRsp
 *
 *    Functionality: Processes E2 Setup Response sent by RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procE2SetupRsp(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx =0, transId=0, idx=0; 
   uint32_t recvBufLen;             
   E2setupResponse_t *e2SetRspMsg=NULL;
   CmLList         *node=NULL;
   E2NodeComponent *e2NodeComponentInfo=NULL;
   E2nodeComponentConfigAdditionAck_List_t *e2NodeCfgAckList=NULL;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeAddAckItem=NULL;

   DU_LOG("\nINFO   -->  E2AP : E2 Setup Response received"); 
   duCb.e2Status = TRUE; //Set E2 status as true
   e2SetRspMsg = &e2apMsg->choice.successfulOutcome->value.choice.E2setupResponse;

   for(arrIdx=0; arrIdx<e2SetRspMsg->protocolIEs.list.count; arrIdx++)
   {
      switch(e2SetRspMsg->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            {
               transId = e2SetRspMsg->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
               if((duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId == transId) &&\
                     (duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode == e2apMsg->choice.successfulOutcome->procedureCode))
               {
                  memset(&duCb.e2apDb.e2TransInfo.e2InitTransaction[transId], 0, sizeof(E2TransInfo));
               }
               else
               {
                  DU_LOG("\nERROR  -->  E2AP : Invalid transaction id [%d]", transId);
                  return RFAILED;
               }
               break;
            }

         case ProtocolIE_IDE2_id_GlobalRIC_ID:
            {
               /* To store the Ric Id Params */
               recvBufLen = sizeof(e2SetRspMsg->protocolIEs.list.array[arrIdx]->value\
                     .choice.GlobalRIC_ID.pLMN_Identity.size);
                  memcpy(&duCb.e2apDb.ricId.plmnId, e2SetRspMsg->protocolIEs.list.array[arrIdx]\
                        ->value.choice.GlobalRIC_ID.pLMN_Identity.buf, recvBufLen);
               bitStringToInt(&e2SetRspMsg->protocolIEs.list.array[arrIdx]->value.choice.GlobalRIC_ID.ric_ID, &duCb.e2apDb.ricId);
               /*TODO : duCb.e2apDb.ricId.plmnId memory to be deallocated after the usage */
               break;
            }

         case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
         {
            e2NodeCfgAckList = &e2SetRspMsg->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAdditionAck_List;
            for(idx =0; idx <e2NodeCfgAckList->list.count; idx++)
            {
               e2NodeAddAckItem = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) e2NodeCfgAckList->list.array[idx];
               switch(e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.present)
               {
                  case E2nodeComponentID_PR_e2nodeComponentInterfaceTypeF1:
                     {
                        e2NodeComponentInfo = fetchE2NodeComponentInfo(F1, E2_NODE_COMPONENT_ADD, &node);
                        if(!e2NodeComponentInfo)
                        {
                           DU_LOG("\nERROR  --> E2AP : Received null e2NodeComponentInfo at line number %d",__LINE__);
                           return RFAILED;
                        }
                        else
                        {
                           cmLListDelFrm(&duCb.e2apDb.e2NodeComponentList, node);
                           DU_FREE(e2NodeComponentInfo->componentRequestPart, e2NodeComponentInfo->reqBufSize);
                           DU_FREE(e2NodeComponentInfo->componentResponsePart, e2NodeComponentInfo->rspBufSize);
                           DU_FREE(e2NodeComponentInfo, sizeof(E2NodeComponent));
                           DU_FREE(node, sizeof(CmLList));
                        }
                        break;
                     }
                  default:
                     break;
               }
            }
            break;
         }

         default:
            DU_LOG("\nERROR  -->  E2AP : Invalid IE received in E2SetupRsp:%ld",
                  e2SetRspMsg->protocolIEs.list.array[arrIdx]->id);
            break;
      }
   }
   freeAperDecodingOfE2SetupRsp(e2SetRspMsg);

   if(duSendE2NodeConfigurationUpdate() != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to send E2 node config update");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Free RIC Subscription Request
 *
 * @details
 *
 *    Function : freeAperDecodingOfRicSubsReq
 *
 * Functionality : Free RIC Subscription Request
 *
 * @return void
 *
 ******************************************************************/
void freeAperDecodingOfRicSubsReq(RICsubscriptionRequest_t *ricSubscriptionReq)
{
   uint8_t idx = 0;
   uint8_t elementIdx = 0;
   RICsubscriptionDetails_t *subsDetails = NULLP;
   RICaction_ToBeSetup_ItemIEs_t *actionItem = NULLP;

   if(ricSubscriptionReq->protocolIEs.list.array)
   {
      for(idx=0; idx < ricSubscriptionReq->protocolIEs.list.count; idx++)
      {
         switch(ricSubscriptionReq->protocolIEs.list.array[idx]->id)
         {
            case ProtocolIE_IDE2_id_RICsubscriptionDetails:
               {
                  subsDetails = &(ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails);
                  free(subsDetails->ricEventTriggerDefinition.buf);

                  if(subsDetails->ricAction_ToBeSetup_List.list.array)
                  {
                     for(elementIdx = 0; elementIdx < subsDetails->ricAction_ToBeSetup_List.list.count; elementIdx++)
                     {
                        if(subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx])
                        {
                           actionItem = (RICaction_ToBeSetup_ItemIEs_t *)subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx];
                           if(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition)
                           {
                              free(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition->buf);
                              free(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition);
                           }
                           free(subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx]);
                        }
                     }
                     free(subsDetails->ricAction_ToBeSetup_List.list.array);
                  }
                  break;
               }
         }
         free(ricSubscriptionReq->protocolIEs.list.array[idx]);
      }
      free(ricSubscriptionReq->protocolIEs.list.array);
   }
}

/*******************************************************************
 *
 * @brief Free Event Trigger Definition
 *
 * @details
 *
 *    Function : freeAperDecodingOfEventTriggerDef
 *
 *    Functionality: Free Event Trigger Definition
 *
 * @params[in] E2SM-KPM Event Trigger Definition
 * @return void
 *
 * ****************************************************************/
void  freeAperDecodingOfEventTriggerDef(E2SM_KPM_EventTriggerDefinition_t *eventTiggerDef)
{
   if(eventTiggerDef)
   {
      switch(eventTiggerDef->eventDefinition_formats.present)
      {
         case E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_NOTHING:
            break;

         case E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_eventDefinition_Format1:
            free(eventTiggerDef->eventDefinition_formats.choice.eventDefinition_Format1);
            break;
      }
   }
}

/*******************************************************************
 *
 * @brief Extract E2SM-KPM Event trigger definition
 *
 * @details
 *
 *    Function : extractEventTriggerDef
 *
 * Functionality : This function :
 *     - Decodes E2SM-KPM Event Trigger Definition
 *     - Validates that even trigger style is supported by E2 node
 *     - Stores event trigger details in local DB
 *
 * @params[in] RAN Function Database structure
 *             RIC Subscription Info to be added to RAN function
 *             RIC Event Trigger Definition buffer received from RIC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t extractEventTriggerDef(RanFunction *ranFuncDb, RicSubscription *ricSubscriptionInfo, \
   RICeventTriggerDefinition_t *ricEventTriggerDef, E2FailureCause *failureCause)
{
   uint8_t ret = RFAILED;
   uint8_t eventIdx = 0;
   asn_dec_rval_t rval ={0};
   E2SM_KPM_EventTriggerDefinition_t eventTiggerDef, *eventTiggerDefPtr = NULLP;

   /* Decoding E2SM-KPM Even Trigger Definition */
   eventTiggerDefPtr = &eventTiggerDef;
   memset(eventTiggerDefPtr, 0, sizeof(E2SM_KPM_EventTriggerDefinition_t));

   rval = aper_decode(0, &asn_DEF_E2SM_KPM_EventTriggerDefinition, (void **)&eventTiggerDefPtr, ricEventTriggerDef->buf,\
         ricEventTriggerDef->size, 0, 0);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  E2AP : ASN decode failed for E2SM-KPM Event Trigger Definition");
      failureCause->causeType = E2_PROTOCOL; 
      failureCause->cause = E2_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE;
      return RFAILED;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_E2SM_KPM_EventTriggerDefinition, eventTiggerDefPtr);

   /* Validating the received event trigger definition format */
   for(eventIdx = 0; eventIdx < ranFuncDb->numOfEventTriggerStyleSupported; eventIdx++)
   {
      if((eventTiggerDefPtr->eventDefinition_formats.present != \
         E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_NOTHING) && \
         (eventTiggerDefPtr->eventDefinition_formats.present == ranFuncDb->eventTriggerStyleList[eventIdx].formatType))
      {
         ricSubscriptionInfo->eventTriggerDefinition.formatType = ranFuncDb->eventTriggerStyleList[eventIdx].formatType;
         ricSubscriptionInfo->eventTriggerDefinition.choice.format1.reportingPeriod = \
            eventTiggerDefPtr->eventDefinition_formats.choice.eventDefinition_Format1->reportingPeriod;

         ret = ROK;
         break;
      }
   }

   if(ret == RFAILED)
   {
      failureCause->causeType = E2_RIC_REQUEST;
      failureCause->cause = E2_EVENT_TRIGGER_NOT_SUPPORTED;
   }
   /* Free E2SM_KPM_EventTriggerDefinition_t */
   freeAperDecodingOfEventTriggerDef(eventTiggerDefPtr);
   return ret;
}

/*******************************************************************
 *
 * @brief Free RIC Action Definition
 *
 * @details
 *
 *    Function :  freeAperDecodingOfRicActionDefinition
 *
 *    Functionality: Free RIC Action Definition
 *
 * @params[in] E2SM-KPM Action definition
 * @return void
 *
 * ****************************************************************/
void  freeAperDecodingOfRicActionDefinition(E2SM_KPM_ActionDefinition_t *actionDef)
{
   uint8_t  elementIdx = 0;
   E2SM_KPM_ActionDefinition_Format1_t *actionFormat1 = NULLP;
   MeasurementInfoItem_t *measItem = NULLP;

   switch(actionDef->actionDefinition_formats.present)
   {
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format1:
         {
            if(actionDef->actionDefinition_formats.choice.actionDefinition_Format1)
            {
               actionFormat1 = actionDef->actionDefinition_formats.choice.actionDefinition_Format1;
               if(actionFormat1->measInfoList.list.array)
               {
                  for(elementIdx = 0; elementIdx < actionFormat1->measInfoList.list.count; elementIdx++)
                  {
                     if(actionFormat1->measInfoList.list.array[elementIdx])
                     {
                        measItem = actionFormat1->measInfoList.list.array[elementIdx];
                        switch(measItem->measType.present)
                        {
                           case MeasurementType_PR_NOTHING:
                              break;

                           case MeasurementType_PR_measName:
                           {
                              free(measItem->measType.choice.measName.buf);
                              break;
                           }

                           case MeasurementType_PR_measID:
                              break;
                        }
                        free(measItem);
                     }
                  }
                  free(actionFormat1->measInfoList.list.array);
               }
               free(actionFormat1);
            }
            break;
         }
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format2:
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format3:
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format4:
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format5:
      default:
         break;   
   }
}

/*******************************************************************
 *
 * @brief Extract Measurement Info list from action definition
 *
 * @details
 *
 *    Function : extractMeasInfoList
 *
 * Functionality : This function :
 *     - Traverses Measurement-to-be-subscribed list
 *     - Validates that each measurement in Measurement-to-be-subscribed
 *       list is supported in RAN-Function->Measurement-supported list.
 *     - If all measurements in an action is supported by RAN function,
 *       it is added to measurement-subscribed list in local DB
 *
 * @params[in] Measurement Info supported list by RAN function
 *             Measurement Info to be subscribed as requested by RIC
 *             Measurement Info finally subscribed
 *             Memory failure indicator
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t extractMeasInfoList(CmLListCp *measInfoSupportedList, MeasurementInfoList_t *measInfoToBeSubscribedList, \
   CmLListCp *measInfoSubscribedList, bool *memFailure)
{
   uint8_t elementIdx = 0;
   MeasurementInfoForAction *measInfoSupportedDb = NULLP;
   MeasurementInfo *measInfoSubscribedDb = NULLP, *measInfoToDel = NULLP;
   CmLList *supportedMeasNode = NULLP, *measToAddNode = NULLP, *measToDelNode = NULLP;;
   MeasurementInfoItem_t *measItem = NULLP;

   /* Validate Measurement list is supported by E2 node. 
    *
    * Traverse and compare the Measurement-Supported List in E2
    * node with Measurement-to-be-subscribed list received from RIC.
    * If a match is found, add it to measurement-subscription list.
    */
   for(elementIdx = 0; elementIdx < measInfoToBeSubscribedList->list.count; elementIdx++)
   {
      measInfoSubscribedDb = NULLP;
      measToAddNode = NULLP;
      measItem = measInfoToBeSubscribedList->list.array[elementIdx];

      CM_LLIST_FIRST_NODE(measInfoSupportedList, supportedMeasNode);
      while(supportedMeasNode)
      {
         measInfoSupportedDb = (MeasurementInfoForAction*)supportedMeasNode->node;
         switch(measItem->measType.present)
         {
            case MeasurementType_PR_measName:
               {
                  if(!strcmp(measInfoSupportedDb->measurementTypeName, (char *)measItem->measType.choice.measName.buf))
                  {
                     DU_ALLOC(measInfoSubscribedDb, sizeof(MeasurementInfo));
                  }
                  break;
               }

            case MeasurementType_PR_measID:
               {
                  if(measInfoSupportedDb->measurementTypeId == measItem->measType.choice.measID)
                  {
                     DU_ALLOC(measInfoSubscribedDb, sizeof(MeasurementInfo));
                  }
                  break;
               }

            default:
               {
                  DU_LOG("\nERROR  ->  DUAPP: Invalid Measurement-type identifier in \
                        E2SM-KPM Action Definition Format");
                  break;
               }
         } /* End of switch, for measurement type identifier */

         /* If measurement type is supported, add to measurement-subscription list */
         if(measInfoSubscribedDb)
         {
            measInfoSubscribedDb->measurementTypeId = measInfoSupportedDb->measurementTypeId;
            memcpy(measInfoSubscribedDb->measurementTypeName, measInfoSupportedDb->measurementTypeName, \
                  strlen(measInfoSupportedDb->measurementTypeName));

            DU_ALLOC(measToAddNode, sizeof(CmLList));
            if(measToAddNode)
            {
               measToAddNode->node = (PTR) measInfoSubscribedDb;
               cmLListAdd2Tail(measInfoSubscribedList, measToAddNode);

               /* Break out of while loop if measurement info is found in measurement-supported list  */
               break;
            }
            else
            {
               DU_FREE(measInfoSubscribedDb, sizeof(MeasurementInfo));
               measInfoSubscribedDb = NULLP;
               *memFailure = true;
               break;
            }
         }

         supportedMeasNode = supportedMeasNode->next;  

      } /* End of while for traversing measurement-supported list in a report style */

      /* If a measurement-to-be-subscribed is not found in measurement-supported list in this report style
       * Then :
       * Delete all entries from measurement-subscription list and
       * Break out of for loop to search in next report style */
      if(!measInfoSubscribedDb)
      {
         while(measInfoSubscribedList->count)
         {
            measToDelNode = cmLListDelFrm(measInfoSubscribedList, measInfoSubscribedList->first);
            measInfoToDel = (MeasurementInfo*)measToDelNode->node;
            DU_FREE(measInfoToDel, sizeof(MeasurementInfo));
            DU_FREE(measToDelNode, sizeof(CmLList));
         }
         break;
      }

   } /* End of for loop , traversing measurement-to-be-subscribed list */

   /* If all measurement-to-be-subscribed was found in measurement-supported list and 
    * was added to measurement-subscription list successfully, return from here */
   if(measInfoToBeSubscribedList->list.count == measInfoSubscribedList->count)
      return ROK;

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Extract E2SM-KPM Action definition
 *
 * @details
 *
 *    Function : extractRicActionDef
 *
 * Functionality : This function :
 *     - Decodes E2SM-KPM Action Definition
 *     - Validates that action is supported by E2 node
 *     - Stores action details in local DB
 *
 * @params[in] RAN Function Database structure
 *             RIC subscription's Action definition to be added to 
 *                RAN function
 *             RIC Action Definition buffer received from RIC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t extractRicActionDef(RanFunction *ranFuncDb, ActionDefinition *actionDefDb, RICactionDefinition_t *ricActionDef,\
   E2FailureCause *failureCause)
{
   bool memFailure = false;
   uint8_t styleIdx = 0;
   asn_dec_rval_t rval ={0};

   E2SM_KPM_ActionDefinition_t actionDef, *actionDefPtr = NULLP;
   E2SM_KPM_ActionDefinition_Format1_t *actionFormat1 = NULLP;
   CmLListCp *measInfoSupportedList = NULLP;
   CmLListCp *measInfoSubscribedList = NULLP;

   /* Decoding E2SM-KPM Action Definition */
   actionDefPtr = &actionDef;
   memset(actionDefPtr, 0, sizeof(E2SM_KPM_EventTriggerDefinition_t));

   rval = aper_decode(0, &asn_DEF_E2SM_KPM_ActionDefinition, (void **)&actionDefPtr, ricActionDef->buf,\
         ricActionDef->size, 0, 0);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  E2AP : ASN decode failed for E2SM-KPM Action Definition");
      failureCause->causeType = E2_PROTOCOL;
      failureCause->cause = E2_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE;
      return RFAILED;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_E2SM_KPM_ActionDefinition, actionDefPtr);


   /* Validate if Report style to subscribe is supported by E2 Node */
   for(styleIdx= 0; styleIdx < ranFuncDb->numOfReportStyleSupported; styleIdx++)
   {
      /* Validate Report style type and report style format type is supported by E2 Node */
      if((ranFuncDb->reportStyleList[styleIdx].reportStyle.styleType == actionDefPtr->ric_Style_Type) &&
            (ranFuncDb->reportStyleList[styleIdx].reportStyle.formatType == actionDefPtr->actionDefinition_formats.present))
      {
         /* Fetch Report stype type and format type */
         actionDefDb->styleType = actionDefPtr->ric_Style_Type;
         actionDefDb->formatType = actionDefPtr->actionDefinition_formats.present;

         switch(actionDefPtr->actionDefinition_formats.present)
         {
            case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format1:
               {
                  actionFormat1 = actionDefPtr->actionDefinition_formats.choice.actionDefinition_Format1; 

                  /* Fetch granularity period */
                  actionDefDb->choice.format1.granularityPeriod = actionFormat1->granulPeriod;

                  /* Validate and add the Measurement to subscription list */
                  measInfoSupportedList = &ranFuncDb->reportStyleList[styleIdx].measurementInfoList;
                  measInfoSubscribedList = &actionDefDb->choice.format1.measurementInfoList;
                  if(extractMeasInfoList(measInfoSupportedList, &actionFormat1->measInfoList, \
                     measInfoSubscribedList, &memFailure) == ROK)
                  {
                     if(!memFailure)
                     {
                        /* Free E2SM_KPM_ActionDefinition_t */
                        freeAperDecodingOfRicActionDefinition(actionDefPtr);
                        return ROK;
                     }
                  }

                  break;  /* End of E2SM-KPM Action definition format 1 case */
               }

            default :
               {
                  DU_LOG("\nERROR  ->  DUAPP: Only E2SM-KPM Action Definition Format 1 is supported");
                  break;
               }
         } /* End of switch for E2SM-KPM Action definition formats */
      }

      if(memFailure)
      {
         failureCause->causeType = E2_MISCELLANEOUS;
         failureCause->cause = E2_MISCELLANEOUS_CAUSE_UNSPECIFIED; 
         break;
      }
   } /* End of for loop, traversing Report-styles-supported list in E2 node */

   /* Memset action Db and Free E2SM_KPM_ActionDefinition_t */
   memset(actionDefDb, 0, sizeof(ActionDefinition));
   freeAperDecodingOfRicActionDefinition(actionDefPtr);

   if(failureCause->causeType == E2_NOTHING)
   {
      failureCause->causeType = E2_RIC_REQUEST;
      failureCause->cause = E2_ACTION_NOT_SUPPORTED;
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Extract RIC Action to be setup
 *
 * @details
 *
 *    Function : extractRicActionToBeSetup
 *
 * Functionality : This function :
 *     - Validates that each action-to-be-setup is supported by E2 node
 *     - Stores event trigger details in local DB
 *
 * @params[in] RAN Function Database structure
 *             RIC Subscription Info to be added to RAN function
 *             RIC Action To Be Setup List received from RIC
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t extractRicActionToBeSetup(RanFunction *ranFuncDb, RicSubscription *ricSubscriptionInfo, \
   RICactions_ToBeSetup_List_t *actionList, E2FailureCause *failureCause, PendingSubsRspInfo *subsRsp)
{
   uint8_t actionIdx = 0;
   uint8_t ricActionId = 0;
   RICaction_ToBeSetup_ItemIEs_t *actionItem = NULLP;

   if(actionList->list.array)
   {
      for(actionIdx = 0; actionIdx < actionList->list.count; actionIdx++)
      {
         actionItem =(RICaction_ToBeSetup_ItemIEs_t *)actionList->list.array[actionIdx];
         switch(actionItem->id)
         {
            case ProtocolIE_IDE2_id_RICaction_ToBeSetup_Item:
               {
                  /* If Action type is REPORT and 
                   * If RIC action definition's extraction and validation passes, 
                   * Then : 
                   * This action is added to action sequence list of subscription info */
                  ricActionId = actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionID;

                  if(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionType == RICactionType_report)
                  {
                     ricSubscriptionInfo->actionSequence[ricActionId].actionId = ricActionId;
                     ricSubscriptionInfo->actionSequence[ricActionId].type = REPORT;

                     if(extractRicActionDef(ranFuncDb, &ricSubscriptionInfo->actionSequence[ricActionId].definition, \
                        actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition, failureCause) == ROK)
                     {
                        ricSubscriptionInfo->actionSequence[ricActionId].action = CONFIG_ADD;
                        ricSubscriptionInfo->numOfActions++;
                        break;
                     }
                  }

                  /* In case of any failure, action is rejected
                   * Added to rejected-action-list in subscription response */
                  memset(&ricSubscriptionInfo->actionSequence[ricActionId], 0, sizeof(ActionInfo));
                  ricSubscriptionInfo->actionSequence[ricActionId].actionId = -1;

                  subsRsp->rejectedActionList[subsRsp->numOfRejectedActions].id = ricActionId;
                  if(failureCause->causeType == E2_NOTHING)
                  {
                     failureCause->causeType = E2_RIC_REQUEST;
                     failureCause->cause = E2_ACTION_NOT_SUPPORTED;
                  }
                  memcpy(&subsRsp->rejectedActionList[subsRsp->numOfRejectedActions].failureCause, \
                        failureCause, sizeof(E2FailureCause));
                  subsRsp->numOfRejectedActions++;
                  break;
               }
            default:
               DU_LOG("\nERROR  -->  E2AP : Invalid IE received in RicSetupLst:%ld",actionItem->id);
               break;
         }
      }
   }

   /* If there is even 1 action that can be added, return ROK */
   if(ricSubscriptionInfo->numOfActions)
      return ROK;

   if(failureCause->causeType == E2_NOTHING)
   {
      failureCause->causeType = E2_RIC_REQUEST;
      failureCause->cause = E2_ACTION_NOT_SUPPORTED;
   }
   return RFAILED;
}

/******************************************************************
 *
 * @brief Processes RIC Subscription Req sent by RIC
 *
 * @details
 *
 *    Function : procRicSubscriptionRequest
 *
 *    Functionality: Processes RIC Subscription Request from RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procRicSubscriptionRequest(E2AP_PDU_t *e2apMsg)
{
   uint8_t idx = 0, actionIdx = 0; 
   uint8_t ret = ROK;
   uint16_t ranFuncId = 0;
   RicRequestId ricReqId;
   CmLList  *ricSubscriptionNode = NULLP;
   RanFunction *ranFuncDb = NULLP;
   RICsubscriptionRequest_t *ricSubsReq = NULLP;
   RICsubscriptionDetails_t *subsDetails = NULLP;
   RicSubscription *ricSubscriptionInfo = NULLP;
   E2FailureCause failureCause;

   DU_LOG("\nINFO   -->  E2AP : RIC Subscription request received"); 

   memset(&failureCause, 0, sizeof(E2FailureCause));
   memset(&ricReqId, 0, sizeof(RicRequestId));

   ricSubsReq = &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionRequest;
   for(idx=0; idx<ricSubsReq->protocolIEs.list.count; idx++)
   {
      if(ricSubsReq->protocolIEs.list.array[idx])
      {
         switch(ricSubsReq->protocolIEs.list.array[idx]->id)
         {
            case ProtocolIE_IDE2_id_RICrequestID:
               {
                  ricReqId.requestorId = ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricRequestorID;
                  ricReqId.instanceId  = ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricInstanceID;

                  break;
               }

            case ProtocolIE_IDE2_id_RANfunctionID:
               {
                  ranFuncId = ricSubsReq->protocolIEs.list.array[idx]->value.choice.RANfunctionID; 

                  /* Validating RAN Function id */
                  ranFuncDb = fetchRanFuncFromRanFuncId(ranFuncId);

                  if(!ranFuncDb)
                  {
                     failureCause.causeType = E2_RIC_REQUEST;
                     failureCause.cause = E2_RAN_FUNCTION_ID_INVALID;
                     ret = RFAILED;
                     break;
                  }

                  if(ranFuncDb->numPendingSubsRsp >= MAX_PENDING_SUBSCRIPTION_RSP)
                  {
                     failureCause.causeType = E2_RIC_REQUEST;
                     failureCause.cause = E2_FUNCTION_RESOURCE_LIMIT; 
                     ret = RFAILED;
                     break;
                  }

                  DU_ALLOC(ricSubscriptionInfo, sizeof(RicSubscription));
                  if(!ricSubscriptionInfo)
                  {
                     DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for ricSubscriptionInfo");
                     failureCause.causeType = E2_MISCELLANEOUS;
                     failureCause.cause = E2_MISCELLANEOUS_CAUSE_UNSPECIFIED;
                     ret = RFAILED;
                     break;
                  }
                  ricSubscriptionInfo->requestId.requestorId = ricReqId.requestorId;
                  ricSubscriptionInfo->requestId.instanceId = ricReqId.instanceId;
                  ricSubscriptionInfo->ranFuncId = ranFuncId;

                  for(actionIdx = 0; actionIdx < MAX_RIC_ACTION; actionIdx++)
                  {
                     ricSubscriptionInfo->actionSequence[actionIdx].actionId = -1;
                  }

                  memset(&ranFuncDb->pendingSubsRspInfo[ranFuncDb->numPendingSubsRsp], 0, sizeof(PendingSubsRspInfo));
                  memcpy(&ranFuncDb->pendingSubsRspInfo[ranFuncDb->numPendingSubsRsp].requestId, 
                        &ricReqId, sizeof(RicRequestId));
                  ranFuncDb->pendingSubsRspInfo[ranFuncDb->numPendingSubsRsp].ranFuncId = ranFuncId;
                  break;
               }

            case ProtocolIE_IDE2_id_RICsubscriptionDetails:
               {
                  subsDetails = &ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails;

                  /* Decode, Validate and record Event Trigger Definition */
                  if(extractEventTriggerDef(ranFuncDb, ricSubscriptionInfo, &subsDetails->ricEventTriggerDefinition, \
                     &failureCause) != ROK)
                  {
                     ret = RFAILED;
                     break;
                  }

                  /* Decode, Validate and record RIC actions */
                  if(extractRicActionToBeSetup(ranFuncDb, ricSubscriptionInfo, &subsDetails->ricAction_ToBeSetup_List, \
                     &failureCause, &ranFuncDb->pendingSubsRspInfo[ranFuncDb->numPendingSubsRsp]) != ROK)
                  {
                     ret = RFAILED;
                     break;
                  }
               }
               break;

            default:
               DU_LOG("\nERROR  -->  E2AP : Invalid IE received in RIC SubsReq:%ld",
                     ricSubsReq->protocolIEs.list.array[idx]->id);
               break;
         }

         if(ret == RFAILED)
            break;
      }
   }

   freeAperDecodingOfRicSubsReq(ricSubsReq);

   if(ret == ROK)
   {
      cmInitTimers(&(ricSubscriptionInfo->ricSubsReportTimer), 1);

      /* Add RAN subcription detail to RAN function */
      DU_ALLOC(ricSubscriptionNode, sizeof(CmLList));
      if(ricSubscriptionNode)
      {
         ricSubscriptionNode->node = (PTR) ricSubscriptionInfo;
         cmLListAdd2Tail(&ranFuncDb->subscriptionList, ricSubscriptionNode);
      }

      ranFuncDb->numPendingSubsRsp++;

#ifdef KPI_CALCULATION
      /* Send statistics request to other DU entities */
      BuildAndSendStatsReq(ricSubscriptionInfo);
#endif      
   }
   else
   {
      DU_FREE(ricSubscriptionInfo, sizeof(RicSubscription));

      if(ranFuncDb)
      {
         memset(&ranFuncDb->pendingSubsRspInfo[ranFuncDb->numPendingSubsRsp], 0, sizeof(PendingSubsRspInfo));
      }

      /* Send RIC Subcription Failure */
      BuildAndSendRicSubscriptionFailure(ricReqId, ranFuncId, failureCause);
   }

   return ret;
}

/******************************************************************
 *
 * @brief Free RIC Subscription Failure
 *
 * @details
 *
 *    Function : FreeRicSubscriptionFailure
 *
 *    Functionality: Free RIC Subscription Failure
 *
 * @params[in] E2AP PDU
 * @return void
 *
 * ****************************************************************/
void FreeRicSubscriptionFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t elemIdx = 0;
   RICsubscriptionFailure_t *ricSubscriptionFailure = NULLP;

   if(e2apMsg)
   {
      if(e2apMsg->choice.unsuccessfulOutcome)
      {
         ricSubscriptionFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICsubscriptionFailure;
         if(ricSubscriptionFailure->protocolIEs.list.array)
         {
            for(elemIdx = 0; elemIdx < ricSubscriptionFailure->protocolIEs.list.count; elemIdx++)
            {
               DU_ALLOC(ricSubscriptionFailure->protocolIEs.list.array[elemIdx], sizeof(RICsubscriptionFailure_IEs_t));
            }
            DU_ALLOC(ricSubscriptionFailure->protocolIEs.list.array, ricSubscriptionFailure->protocolIEs.list.size);
         }
         DU_ALLOC(e2apMsg->choice.unsuccessfulOutcome, sizeof(UnsuccessfulOutcomeE2_t));
      }
      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/******************************************************************
 *
 * @brief Fill and Send RIC Subscription Failure to RIC
 *
 * @details
 *
 *    Function : BuildAndSendRicSubscriptionFailure
 *
 *    Functionality: Fill and Send RIC Subscription Failure to RIC
 *
 * @params[in] RIC Request ID
 *             RAN Function ID
 *             Cause of Failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendRicSubscriptionFailure(RicRequestId ricReqId, uint16_t ranFuncId, E2FailureCause failureCause)
{
   uint8_t          ret = RFAILED;
   uint8_t          elementCnt = 0, elemIdx = 0;
   E2AP_PDU_t       *e2apMsg = NULLP;
   asn_enc_rval_t   encRetVal;        /* Encoder return value */
   RICsubscriptionFailure_t *ricSubscriptionFailure = NULLP;
   RICsubscriptionFailure_IEs_t *ricSubsFailIe = NULLP;

   while(true)
   {
      DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Failure\n");

      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation at [%s] : Line [%d]", __func__, __LINE__); 
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_unsuccessfulOutcome;
      DU_ALLOC(e2apMsg->choice.unsuccessfulOutcome, sizeof(UnsuccessfulOutcomeE2_t));
      if(e2apMsg->choice.unsuccessfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation at [%s] : Line [%d]", __func__, __LINE__); 
         break;
      }
      e2apMsg->choice.unsuccessfulOutcome->procedureCode = ProcedureCodeE2_id_RICsubscription;
      e2apMsg->choice.unsuccessfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.unsuccessfulOutcome->value.present = UnsuccessfulOutcomeE2__value_PR_RICsubscriptionFailure;

      ricSubscriptionFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICsubscriptionFailure;

      elementCnt = 3;
      ricSubscriptionFailure->protocolIEs.list.count = elementCnt;
      ricSubscriptionFailure->protocolIEs.list.size = elementCnt * sizeof(RICsubscriptionFailure_IEs_t *);
      DU_ALLOC(ricSubscriptionFailure->protocolIEs.list.array, ricSubscriptionFailure->protocolIEs.list.size);
      if(!ricSubscriptionFailure->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation at [%s] : Line [%d]", __func__, __LINE__); 
         break;
      }

      for(elemIdx = 0; elemIdx < elementCnt; elemIdx++)
      {
         DU_ALLOC(ricSubscriptionFailure->protocolIEs.list.array[elemIdx], sizeof(RICsubscriptionFailure_IEs_t));
         if(!ricSubscriptionFailure->protocolIEs.list.array[elemIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation at [%s] : Line [%d] for IE at index [%d]", \
               __func__, __LINE__, elemIdx);
            break;
         }
      }
      if(elemIdx < elementCnt)
         break;

      elemIdx = 0;

      /* RIC Request ID */
      ricSubsFailIe = ricSubscriptionFailure->protocolIEs.list.array[elemIdx++];
      ricSubsFailIe->id = ProtocolIE_IDE2_id_RICrequestID;
      ricSubsFailIe->criticality = CriticalityE2_reject;
      ricSubsFailIe->value.present = RICsubscriptionFailure_IEs__value_PR_RICrequestID;
      ricSubsFailIe->value.choice.RICrequestID.ricRequestorID = ricReqId.requestorId;
      ricSubsFailIe->value.choice.RICrequestID.ricInstanceID = ricReqId.instanceId;

      /* RAN Function ID */
      ricSubsFailIe = ricSubscriptionFailure->protocolIEs.list.array[elemIdx++];
      ricSubsFailIe->id = ProtocolIE_IDE2_id_RANfunctionID;
      ricSubsFailIe->criticality = CriticalityE2_reject;
      ricSubsFailIe->value.present = RICsubscriptionFailure_IEs__value_PR_RANfunctionID;
      ricSubsFailIe->value.choice.RANfunctionID = ranFuncId;

      /* Cause */
      ricSubsFailIe = ricSubscriptionFailure->protocolIEs.list.array[elemIdx++];
      ricSubsFailIe->id = ProtocolIE_IDE2_id_CauseE2;
      ricSubsFailIe->criticality = CriticalityE2_reject;
      ricSubsFailIe->value.present = RICsubscriptionFailure_IEs__value_PR_CauseE2;
      fillE2Cause(&ricSubsFailIe->value.choice.CauseE2, failureCause);

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC Subscription Failure Message (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC Subscription Failure Message \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }

      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nINFO   -->  E2AP : Sending RIC Subscription Failure");

      }
      ret = ROK;
      break;
   }
   FreeRicSubscriptionFailure(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Free the RicIndication Message
 *
 * @details
 *
 *    Function : FreeRicIndication
 *
 * Functionality: Free the RicIndication Message
 *
 * @return void
 *         
 *
 ******************************************************************/
void FreeRicIndication(E2AP_PDU_t  *e2apMsg) 
{
   uint8_t idx = 0;
   RICindication_t *ricIndicationMsg= NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         ricIndicationMsg = &e2apMsg->choice.initiatingMessage->value.choice.RICindication;
         if(ricIndicationMsg!= NULLP)
         {
            if(ricIndicationMsg->protocolIEs.list.array != NULLP)
            {
               for(idx=0; idx<ricIndicationMsg->protocolIEs.list.count; idx++)
               {
                  if(ricIndicationMsg->protocolIEs.list.array[idx] != NULLP)
                  {
                     switch(ricIndicationMsg->protocolIEs.list.array[idx]->id)
                     {
                        case ProtocolIE_IDE2_id_RICrequestID:
                        case ProtocolIE_IDE2_id_RANfunctionID:
                        case ProtocolIE_IDE2_id_RICactionID:
                        case ProtocolIE_IDE2_id_RICindicationType:
                           break;

                        case ProtocolIE_IDE2_id_RICindicationHeader:
                           {
                              DU_FREE(ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader.buf,\
                                    ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader.size);
                              break;
                           }
                        case ProtocolIE_IDE2_id_RICindicationMessage:
                           {
                              DU_FREE(ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage.buf,\
                                    ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage.size);
                              break;
                           }
                        default:
                           break;
                     }
                     DU_FREE(ricIndicationMsg->protocolIEs.list.array[idx],sizeof(RICindication_IEs_t));
                  }
               }
               DU_FREE(ricIndicationMsg->protocolIEs.list.array,ricIndicationMsg->protocolIEs.list.size);
            }
         }
         DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Free measurement record
 *
 * @details
 *
 *    Function : freeMeasRecord
 *
 * Functionality: Free all measurement recorded for a measurement
 *    within an action in a RIC subscription
 *
 * @param  Measurement data to be freed
 * @return void
 *
 ******************************************************************/
void freeMeasData(MeasurementData_t *measData)
{
   uint8_t measIdx = 0, measRecIdx = 0;
   MeasurementRecord_t *measRecord = NULLP;

   if(measData->list.array)
   {
      for(measIdx = 0; measIdx < measData->list.count; measIdx++)
      {
         if(measData->list.array[measIdx])
         {
            measRecord = &measData->list.array[measIdx]->measRecord;
            if(measRecord->list.array)
            {
               for(measRecIdx = 0; measRecIdx < measRecord->list.count; measRecIdx++)
               {
                  DU_FREE(measRecord->list.array[measRecIdx], sizeof(MeasurementRecordItem_t));
               }
               DU_FREE(measRecord->list.array, measRecord->list.size);
            }
            DU_FREE(measData->list.array[measIdx], sizeof(MeasurementDataItem_t));
         }
      }
      DU_FREE(measData->list.array, measData->list.size);
   }
}

/*******************************************************************
 *
 * @brief Fill measurement info list
 *
 * @details
 *
 *    Function : freeMeasInfoList
 *
 * Functionality: Fills all measurement info within an action 
 *    in a RIC subscription
 *
 * @param  Measurement Info list to be freed
 * @return void
 *
 ******************************************************************/
void freeMeasInfoList(MeasurementInfoList_t *measInfoList)
{
   uint8_t measInfoIdx = 0;

   if(measInfoList->list.array)
   {
      for(measInfoIdx = 0; measInfoIdx < measInfoList->list.count; measInfoIdx++)
      {
         if(measInfoList->list.array[measInfoIdx])
         {
            DU_FREE(measInfoList->list.array[measInfoIdx]->measType.choice.measName.buf, \
                  measInfoList->list.array[measInfoIdx]->measType.choice.measName.size);

            DU_FREE(measInfoList->list.array[measInfoIdx], measInfoList->list.size);
         }
      }
      DU_FREE(measInfoList->list.array, measInfoList->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free E2SM-KPM Indication Message
 *
 * @details
 *
 *    Function : FreeE2smKpmIndicationMessage
 *
 * Functionality: Free E2SM-KPM Indication Message
 *
 * @param  E2SM-KPM Indication message to be freed
 * @return void
 *
 ******************************************************************/
void FreeE2smKpmIndicationMessage(E2SM_KPM_IndicationMessage_t *e2smKpmIndMsg)
{
   E2SM_KPM_IndicationMessage_Format1_t *format1Msg = NULLP;

   switch(e2smKpmIndMsg->indicationMessage_formats.present)
   {
      case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format1:
         {
            if(e2smKpmIndMsg->indicationMessage_formats.choice.indicationMessage_Format1)
            {
               format1Msg = e2smKpmIndMsg->indicationMessage_formats.choice.indicationMessage_Format1;
               
               /* Measurement Data */
               freeMeasData(&format1Msg->measData);

               /* Measurement Info List */
               if(format1Msg->measInfoList)
               {
                  freeMeasInfoList(format1Msg->measInfoList);
                  DU_FREE(format1Msg->measInfoList, sizeof(MeasurementInfoList_t));
               }

               /* Granularity Period */
               DU_FREE(format1Msg->granulPeriod, sizeof(GranularityPeriod_t));

               DU_FREE(format1Msg, sizeof(E2SM_KPM_IndicationMessage_Format1_t));
            }
            break;
         }

      case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_NOTHING:
      case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format2:
      default:
         break;
   }
}

/*******************************************************************
 *
 * @brief Fill measurement record
 *
 * @details
 *
 *    Function : fillMeasRecord
 *
 * Functionality: Fills all measurement value for a measurement
 *    within an action in a RIC subscription
 *
 * @param  Measurement record to be filled
 *         Measurement database with measurement records
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillMeasRecord(MeasurementRecord_t *measRecord, MeasurementInfo *measInfoDb)
{
   uint8_t measRecIdx = 0;
   CmLList *measValNode = NULLP;
   double  measVal = 0;

   measRecord->list.count = measInfoDb->measuredValue.count;
   measRecord->list.size = measRecord->list.count * sizeof(MeasurementRecordItem_t *);

   DU_ALLOC(measRecord->list.array, measRecord->list.size);
   if(!measRecord->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   for(measRecIdx = 0; measRecIdx < measRecord->list.count; measRecIdx++)
   {
      DU_ALLOC(measRecord->list.array[measRecIdx], sizeof(MeasurementRecordItem_t));
      if(!measRecord->list.array[measRecIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
         return RFAILED;
      }
   }

   measRecIdx = 0;
   CM_LLIST_FIRST_NODE(&measInfoDb->measuredValue, measValNode);
   while(measValNode)
   {
     measVal = *(double *)measValNode->node;
     if(measVal == (int)measVal)
     {
        measRecord->list.array[measRecIdx]->present = MeasurementRecordItem_PR_integer;
        measRecord->list.array[measRecIdx]->choice.integer = (int)measVal;
     }
     else
     {
         measRecord->list.array[measRecIdx]->present = MeasurementRecordItem_PR_real;
         measRecord->list.array[measRecIdx]->choice.real = measVal;
     }
     measRecIdx++;

     /* Once the measurement record is added to the message, delete it from DB */
     cmLListDelFrm(&measInfoDb->measuredValue, measValNode);
     DU_FREE(measValNode->node, sizeof(double));
     DU_FREE(measValNode, sizeof(CmLList));

     CM_LLIST_FIRST_NODE(&measInfoDb->measuredValue, measValNode);
     measVal = 0;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills measuerement data
 *
 * @details
 *
 *    Function : fillMeasData
 *
 * Functionality: Fill all measurement recorded for all measurements
 *    in an action in a RIC subscription
 *
 * @param  Measurement data to be filled
 *         Measurement info list from an action DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillMeasData(MeasurementData_t *measData, CmLListCp *measInfoListDb)
{
  uint8_t measIdx = 0;
  CmLList *measInfoNode = NULLP;
  MeasurementInfo *measInfoDb = NULLP;
  MeasurementRecord_t *measRecord = NULLP;

  measData->list.count = measInfoListDb->count;
  measData->list.size = measData->list.count * sizeof(MeasurementDataItem_t *);

  DU_ALLOC(measData->list.array, measData->list.size);
  if(!measData->list.array)
  {
     DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
     return RFAILED;
  }

  measIdx = 0;
  CM_LLIST_FIRST_NODE(measInfoListDb, measInfoNode);
  while(measInfoNode)
  {
     measInfoDb = (MeasurementInfo *)measInfoNode->node;
     if(measInfoDb)
     {
        DU_ALLOC(measData->list.array[measIdx], sizeof(MeasurementDataItem_t));
        if(!measData->list.array[measIdx])
        {
           DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
           return RFAILED;
        }

        measRecord = &measData->list.array[measIdx]->measRecord;
        if(fillMeasRecord(measRecord, measInfoDb) != ROK)
        {
           DU_LOG("\nERROR  -->  E2AP : Failed to fill measurement record");
           return RFAILED;
        }
        measIdx++;
     }
     measInfoNode = measInfoNode->next;
  }

  return ROK;
}

/*******************************************************************
 *
 * @brief Fill all measurement info
 *
 * @details
 *
 *    Function : fillMeasInfoList
 *
 * Functionality: Fills all measurement info belonging to an action
 *  in a RIC subscription
 *
 * @param   Measurement Info list to be filled
 *          Measurement Info list from E2AP DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillMeasInfoList(MeasurementInfoList_t *measInfoList, CmLListCp *measInfoListDb)
{
   uint8_t measInfoIdx = 0;
   CmLList *measInfoNode = NULLP;
   MeasurementInfo *measInfoDb = NULLP;
   MeasurementInfoItem_t *measInfoItem = NULLP;

   measInfoList->list.count = measInfoListDb->count;
   measInfoList->list.size = measInfoList->list.count * sizeof(MeasurementInfoItem_t *);

   DU_ALLOC(measInfoList->list.array, measInfoList->list.size);
   if(!measInfoList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   measInfoIdx = 0;
   CM_LLIST_FIRST_NODE(measInfoListDb, measInfoNode);
   while(measInfoNode)
   {
      DU_ALLOC(measInfoList->list.array[measInfoIdx], sizeof(MeasurementInfoItem_t));
      if(!measInfoList->list.array[measInfoIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
         return RFAILED;
      }

      measInfoItem = measInfoList->list.array[measInfoIdx];
      measInfoDb = (MeasurementInfo *)measInfoNode->node;
      if(measInfoDb)
      {
         /* Measurement Type */
         measInfoItem->measType.present = MeasurementType_PR_measName;
         measInfoItem->measType.choice.measName.size = strlen(measInfoDb->measurementTypeName);

         DU_ALLOC(measInfoItem->measType.choice.measName.buf, measInfoItem->measType.choice.measName.size);
         if(!measInfoItem->measType.choice.measName.buf)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
            return RFAILED;
         }

         memcpy(measInfoItem->measType.choice.measName.buf, measInfoDb->measurementTypeName,\
            measInfoItem->measType.choice.measName.size);

         measInfoIdx++;
      }
      measInfoNode = measInfoNode->next;
      measInfoDb = NULLP;
   }

   return ROK;
}

 /*******************************************************************
 *
 * @brief Fill E2SM-KPM Indication Message Format 1
 *
 * @details
 *
 *    Function : fillE2smKpmIndMsgFormat1
 *
 * Functionality: Fill E2SM-KPM Indication Message Format 1
 *
 * @param  Format 1 Message to be filled
 *         Action Definition format 1 from E2AP DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillE2smKpmIndMsgFormat1(E2SM_KPM_IndicationMessage_Format1_t *format1Msg, ActionDefFormat1 *format1)
{
  /* Measurement Data */
  if(fillMeasData(&format1Msg->measData, &format1->measurementInfoList) != ROK)
  {
     DU_LOG("\nERROR  -->  E2AP : Failed to fill measurement data");
     return RFAILED;
  }

  /* Measurement Information */
  DU_ALLOC(format1Msg->measInfoList, sizeof(MeasurementInfoList_t));
  if(!format1Msg->measInfoList)
  {
     DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
     return RFAILED;
  }

  if(fillMeasInfoList(format1Msg->measInfoList, &format1->measurementInfoList) != ROK)
  {
     DU_LOG("\nERROR  -->  E2AP : Failed to fill measurement information list");
     return RFAILED;
  }

  /* Granularity Period */
  DU_ALLOC(format1Msg->granulPeriod, sizeof(GranularityPeriod_t));
  if(!format1Msg->granulPeriod)
  {
     DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
     return RFAILED;
  }
  *(format1Msg->granulPeriod) = format1->granularityPeriod;

  return ROK;
}

/*******************************************************************
 *
 * @brief Fill RIC Indication Message buffer
 *
 * @details
 *
 *    Function : fillRicIndMsgBuf
 *
 * Functionality: Fill E2SM-KPM Indication Message
 *    Encode this message and copy to RIC Indication Message buffer
 * 
 * @param  RIC Indication Message buffer to be filled
 *         Source action info from E2AP DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillRicIndMsgBuf(RICindicationMessage_t *ricIndMsgBuf, ActionInfo *actionInfo)
{
   uint8_t ret = RFAILED;
   bool failedInFormat = false;
   E2SM_KPM_IndicationMessage_t e2smKpmIndMsg;
   asn_enc_rval_t   encRetVal;        /* Encoder return value */

   memset(&e2smKpmIndMsg, 0, sizeof(E2SM_KPM_IndicationMessage_t));

   while(true)
   {
      /* E2SM-KPM Indication message format type */
      e2smKpmIndMsg.indicationMessage_formats.present = actionInfo->definition.formatType;
      switch(e2smKpmIndMsg.indicationMessage_formats.present)
      {
         case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format1:
            {
               /* E2SM-KPM Indication message format 1 */
               DU_ALLOC(e2smKpmIndMsg.indicationMessage_formats.choice.indicationMessage_Format1, \
                     sizeof(E2SM_KPM_IndicationMessage_Format1_t));
               if(!e2smKpmIndMsg.indicationMessage_formats.choice.indicationMessage_Format1)
               {
                  DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
                  failedInFormat = true;
                  break;
               }

               if(fillE2smKpmIndMsgFormat1(e2smKpmIndMsg.indicationMessage_formats.choice.indicationMessage_Format1, \
                  &actionInfo->definition.choice.format1) != ROK)
               {
                  DU_LOG("\nERROR  -->  E2AP : Failed to fill E2SM-KPM Indication message format 1");
                  failedInFormat = true;
                  break;
               }
               break;
            }

         case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_NOTHING:
         case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format2:
         default:
            {
               DU_LOG("\nERROR  -->  E2AP : fillRicIndMsgBuf: Only Format 1 supported");
               failedInFormat = true;
               break;
            }
      }

      if(failedInFormat)
         break;

      /* Encode E2SM-KPM Indication Message */
      xer_fprint(stdout, &asn_DEF_E2SM_KPM_IndicationMessage, &e2smKpmIndMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2SM_KPM_IndicationMessage, 0, &e2smKpmIndMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2SM-KPM Indication Message (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2SM-KPM Indication Message \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         } 
#endif
      }

      /* Copy encoded string to RIC Indication Message buffer */
      ricIndMsgBuf->size = encBufSize;
      DU_ALLOC(ricIndMsgBuf->buf, ricIndMsgBuf->size);
      if(!ricIndMsgBuf->buf)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
         break;
      }
      memset(ricIndMsgBuf->buf, 0, ricIndMsgBuf->size);
      memcpy(ricIndMsgBuf->buf, encBuf, encBufSize);

      ret = ROK;
      break;
   }

   /* Free E2SM-KPM Indication Message */
   FreeE2smKpmIndicationMessage(&e2smKpmIndMsg);

   return ret;
}

/*******************************************************************
 *
 * @brief Free E2SM-KPM Indication Header
 *
 * @details
 *
 *    Function : FreeE2smKpmIndicationHeader
 *
 * Functionality: Free E2SM-KPM Indication Header
 * 
 * @param  E2SM-KPM Indication Header to be free
 * @return void
 *
 ******************************************************************/
void FreeE2smKpmIndicationHeader(E2SM_KPM_IndicationHeader_t *e2smKpmIndHdr)
{
   E2SM_KPM_IndicationHeader_Format1_t *format1 = NULLP;

   if(e2smKpmIndHdr)
   {
      switch(e2smKpmIndHdr->indicationHeader_formats.present)
      {
         case E2SM_KPM_IndicationHeader__indicationHeader_formats_PR_indicationHeader_Format1:
            {
               if(e2smKpmIndHdr->indicationHeader_formats.choice.indicationHeader_Format1)
               {
                  format1 = e2smKpmIndHdr->indicationHeader_formats.choice.indicationHeader_Format1;

                  DU_FREE(format1->colletStartTime.buf, format1->colletStartTime.size);
                  DU_FREE(format1, sizeof(E2SM_KPM_IndicationHeader_Format1_t));
               }
               break;
            }
         case E2SM_KPM_IndicationHeader__indicationHeader_formats_PR_NOTHING:
         default:
            break;
      }
   }
}

/*******************************************************************
 *
 * @brief Fill RIC Indication Header buffer
 *
 * @details
 *
 *    Function : fillRicIndHeader
 *
 * Functionality: Fill E2SM-KPM Indication Header
 *    Encode this message and copy to RIC Indication Header buffer
 * 
 * @param  RIC Indication Header buffer to be filled
 *         Source RIC subscription info from E2AP DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillRicIndHeader(RICindicationHeader_t *ricIndHdr, RicSubscription *ricSubsInfo)
{
   uint8_t ret = RFAILED;
   uint8_t secBufIdx = 0, milliSecBufIdx = 0;
   int8_t byteIdx = 0;
   bool formatFailure = false;
   RanFunction *ranFunc = NULLP;
   ReportStartTime *startTime = NULLP;
   E2SM_KPM_IndicationHeader_t e2smKpmIndHdr;
   E2SM_KPM_IndicationHeader_Format1_t *format1 = NULLP;
   asn_enc_rval_t   encRetVal;        /* Encoder return value */

   while(true)
   {
      ranFunc = fetchRanFuncFromRanFuncId(ricSubsInfo->ranFuncId);
      if(ranFunc == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : RAN Function ID [%d] not found", ricSubsInfo->ranFuncId);
         break;
      }

      memset(&e2smKpmIndHdr, 0, sizeof(E2SM_KPM_IndicationHeader_t));

      e2smKpmIndHdr.indicationHeader_formats.present = ranFunc->ricIndicationHeaderFormat;
      switch(e2smKpmIndHdr.indicationHeader_formats.present)
      {
         case E2SM_KPM_IndicationHeader__indicationHeader_formats_PR_indicationHeader_Format1:
            {
               DU_ALLOC(e2smKpmIndHdr.indicationHeader_formats.choice.indicationHeader_Format1, \
                     sizeof(E2SM_KPM_IndicationHeader_Format1_t));
               if(!e2smKpmIndHdr.indicationHeader_formats.choice.indicationHeader_Format1)
               {
                  DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
                  formatFailure = true;
                  break;
               }
               format1 = e2smKpmIndHdr.indicationHeader_formats.choice.indicationHeader_Format1;

               /* Fetch reporting period start time from DB */
               switch(ricSubsInfo->eventTriggerDefinition.formatType)
               {
                  case 1:
                  {
                     startTime = &ricSubsInfo->eventTriggerDefinition.choice.format1.startTime;
                  }
               }

               format1->colletStartTime.size = 8 * sizeof(uint8_t);
               DU_ALLOC(format1->colletStartTime.buf, format1->colletStartTime.size);
               if(!format1->colletStartTime.buf)
               {
                  DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
                  formatFailure = true;
                  break;
               }

               /* As per O-RAN.WG3.E2SM-KPM-R003-v03.00, section 8.3.12 and
                * RFC 5905, section 6 :
                * Time stamp has a 64-bit format where first 32-bit is seconds
                * and next 32-bit is fraction in picosecond-level.
                * This fraction has been rounded in microseconds.
                *
                * Hence,
                * Storing 32-bit seconds at MSB 0-3 and
                * 32-bit milliseconds at next 4 bytes i.e. bytes 4-7
                */
               secBufIdx = 0;
               milliSecBufIdx = 4;
               for(byteIdx = 3; byteIdx >= 0; byteIdx--)
               {
                  format1->colletStartTime.buf[secBufIdx++] = startTime->timeInSec >> (8*byteIdx);
                  format1->colletStartTime.buf[milliSecBufIdx++] = startTime->timeInMilliSec >> (8*byteIdx);
               }
               break;
            }

         case E2SM_KPM_IndicationHeader__indicationHeader_formats_PR_NOTHING:
         default:
         {
             DU_LOG("\nERROR  -->  E2AP : Only E2SM-KPM Indication Header Format 1 supported");
             formatFailure = true;
             break;
         }
      }

      if(formatFailure)
         break;

      /* Encode E2SM-KPM Indication Header */
      xer_fprint(stdout, &asn_DEF_E2SM_KPM_IndicationHeader, &e2smKpmIndHdr);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2SM_KPM_IndicationHeader, 0, &e2smKpmIndHdr, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2SM-KPM Indication Header (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2SM-KPM Indication Header \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         } 
#endif
      }

      /* Copy encoded string to RIC Indication Header buffer */
      ricIndHdr->size = encBufSize;
      DU_ALLOC(ricIndHdr->buf, ricIndHdr->size);
      if(!ricIndHdr->buf)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
         break;
      }
      memset(ricIndHdr->buf, 0, ricIndHdr->size);
      memcpy(ricIndHdr->buf, encBuf, encBufSize);
      ret = ROK;
      break;
   }

   /* Free E2SM-KPM Indication Header */
   FreeE2smKpmIndicationHeader(&e2smKpmIndHdr);

   return ret;
}

/*******************************************************************
 *
 * brief Fill the RIC Indication Message
 *
 * @details
 *
 *    Function : fillRicIndication
 *
 * Functionality: Fills the RIC Indication Message
 *
 * @param  RIC Indication Message to be filled
 *         RIC Subscription DB
 *         Action DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillRicIndication(RICindication_t *ricIndicationMsg, RicSubscription *ricSubscriptionInfo, ActionInfo *actionInfo)
{
   uint8_t elementCnt = 0, idx = 0;
   uint8_t ret = ROK;

   elementCnt = 6;

   ricIndicationMsg->protocolIEs.list.count = elementCnt;
   ricIndicationMsg->protocolIEs.list.size  = elementCnt * sizeof(RICindication_IEs_t *);

   /* Initialize the Ric Indication members */
   DU_ALLOC(ricIndicationMsg->protocolIEs.list.array, ricIndicationMsg->protocolIEs.list.size);
   if(ricIndicationMsg->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      DU_ALLOC(ricIndicationMsg->protocolIEs.list.array[idx], sizeof(RICindication_IEs_t));
      if(ricIndicationMsg->protocolIEs.list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation in [%s] at line [%d]", __func__, __LINE__);
         return RFAILED;
      }
   }

   /* RIC Request ID */
   idx = 0;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = RICindication_IEs__value_PR_RICrequestID;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricRequestorID = \
      ricSubscriptionInfo->requestId.requestorId;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricInstanceID = \
      ricSubscriptionInfo->requestId.instanceId;

   /* RAN Function ID */
   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = RICindication_IEs__value_PR_RANfunctionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RANfunctionID = ricSubscriptionInfo->ranFuncId;

   /* RIC Action ID */
   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICactionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = RICindication_IEs__value_PR_RICactionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICactionID = actionInfo->actionId;

   /* RIC Indication Type */
   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationType;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = RICindication_IEs__value_PR_RICindicationType;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationType = actionInfo->type;

   /* RIC Indication Header */
   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationHeader;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = RICindication_IEs__value_PR_RICindicationHeader;
   if(fillRicIndHeader(&ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader, \
      ricSubscriptionInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to fill RIC Indication header");
      return RFAILED;
   }

   /* RIC Indication Message */
   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationMessage;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = RICindication_IEs__value_PR_RICindicationMessage;
   if(fillRicIndMsgBuf(&ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage, \
      actionInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to fill RIC Indication Message");
      return RFAILED;
   }

   return ret;
}

/*******************************************************************
 *
 * @brief Builds and Send the RicIndication Message
 *
 * @details
 *
 *    Function : BuildAndSendRicIndication
 *
 * Functionality:Fills the RicIndication Message
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendRicIndication(RicSubscription *ricSubscriptionInfo, ActionInfo *actionInfo)
{
   uint8_t          ret = RFAILED; 
   E2AP_PDU_t       *e2apMsg = NULLP;
   RICindication_t  *ricIndicationMsg = NULLP;
   asn_enc_rval_t   encRetVal;        /* Encoder return value */

   while(true)
   {
      DU_LOG("\nINFO   -->  E2AP : Building RIC Indication Message\n");

      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICindication;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICindication;

      ricIndicationMsg = &e2apMsg->choice.initiatingMessage->value.choice.RICindication;

      if(fillRicIndication(ricIndicationMsg, ricSubscriptionInfo, actionInfo) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to fill RIC Indication message");
         break;
      }

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC Indication Message (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC Indication Message \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         } 
#endif
      }

      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nINFO   -->  E2AP : Sending RIC Indication Message");      

      }
      ret = ROK;
      break;
   }
   FreeRicIndication(e2apMsg);	
   return ret;
}

/*******************************************************************
 *
 * @brief free e2 node component configuration req and rsp
 *
 * @details
 *
 *    Function : freeE2NodeComponentConfiguration 
 *
 *    Functionality:
 *       - free e2 node component configuration req and rsp
 *
 * @params[in] E2nodeComponentConfiguration_t *e2nodeComponentConfiguration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void freeE2NodeComponentConfiguration(E2nodeComponentConfiguration_t *e2nodeComponentConfiguration)
{
   /* Free E2 Node Component Request Part */
   DU_FREE(e2nodeComponentConfiguration->e2nodeComponentRequestPart.buf, e2nodeComponentConfiguration->e2nodeComponentRequestPart.size);

   /* Free E2 Node Component Response Part */
   DU_FREE(e2nodeComponentConfiguration->e2nodeComponentResponsePart.buf, e2nodeComponentConfiguration->e2nodeComponentResponsePart.size);
                                 
}

/*******************************************************************
 *
 * @brief free e2 node component component identifier
 *
 * @details
 *
 *    Function : freeE2NodeComponentIdentifier
 *
 *    Functionality:
 *       - free e2 node component component identifier
 *
 * @params[in] E2nodeComponentID_t  *componentID 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void freeE2NodeComponentIdentifier(E2nodeComponentID_t *componentID)
{
   if(componentID->choice.e2nodeComponentInterfaceTypeF1)
   {
      DU_FREE(componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf, componentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size);
      DU_FREE(componentID->choice.e2nodeComponentInterfaceTypeF1, sizeof(E2nodeComponentInterfaceF1_t));
   }
                                 
}

/*******************************************************************
 *
 * @brief Deallocate the memory allocated for E2nodeConfigurationUpdate msg 
 *
 * @details
 *
 *    Function : FreeE2NodeConfigUpdate 
 *
 *    Functionality:
 *       - freeing the memory allocated for E2nodeConfigurationUpdate
 *
 * @params[in] E2AP_PDU_t *e2apMsg 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void FreeE2NodeConfigUpdate(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx =0, e2NodeUpdateListIdx=0, e2NodeRemovalListIdx=0, e2NodeAddListIdx=0;
   E2nodeConfigurationUpdate_t *e2NodeConfigUpdate =NULL;
   E2nodeComponentConfigUpdate_List_t *e2NodeUpdateList  =NULL;
   E2nodeComponentConfigUpdate_ItemIEs_t *e2NodeUpdateItem =NULL;
   E2nodeComponentConfigRemoval_List_t *e2NodeRemovalList =NULL;
   E2nodeComponentConfigRemoval_ItemIEs_t *e2NodeRemovalItem =NULL;
   E2nodeComponentConfigAddition_List_t *e2NodeAddList =NULL;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItem =NULL;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         e2NodeConfigUpdate = &e2apMsg->choice.initiatingMessage->value.choice.E2nodeConfigurationUpdate;
         if(e2NodeConfigUpdate->protocolIEs.list.array != NULLP)
         {
            for(arrIdx = 0; arrIdx < e2NodeConfigUpdate->protocolIEs.list.count; arrIdx++)
            {
               if(e2NodeConfigUpdate->protocolIEs.list.array[arrIdx])
               {

                  switch(e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_TransactionID:
                        break;

                     case ProtocolIE_IDE2_id_E2nodeComponentConfigAddition:
                     {
                         e2NodeAddList = &e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAddition_List;
                         if(e2NodeAddList->list.array)
                         {
                             for(e2NodeAddListIdx = 0; e2NodeAddListIdx< e2NodeAddList->list.count; e2NodeAddListIdx++)
                             {
                                e2NodeAddItem = (E2nodeComponentConfigAddition_ItemIEs_t *) e2NodeAddList->list.array[e2NodeAddListIdx];
                                 
                                freeE2NodeComponentConfiguration(&e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentConfiguration);
                                freeE2NodeComponentIdentifier(&e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID);
                                DU_FREE(e2NodeAddItem, sizeof(E2nodeComponentConfigAddition_ItemIEs_t));
                             }
                             DU_FREE(e2NodeAddList->list.array, e2NodeAddList->list.size);
                         }
                         break;
                     }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate:
                        {
                           e2NodeUpdateList = &e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigUpdate_List;
                           if(e2NodeUpdateList->list.array)
                           {
                              for(e2NodeUpdateListIdx = 0; e2NodeUpdateListIdx< e2NodeUpdateList->list.count; e2NodeUpdateListIdx++)
                              {
                                 e2NodeUpdateItem = (E2nodeComponentConfigUpdate_ItemIEs_t *) e2NodeUpdateList->list.array[e2NodeUpdateListIdx];
                                 
                                 freeE2NodeComponentConfiguration(&e2NodeUpdateItem->value.choice.E2nodeComponentConfigUpdate_Item.e2nodeComponentConfiguration);
                                 freeE2NodeComponentIdentifier(&e2NodeUpdateItem->value.choice.E2nodeComponentConfigUpdate_Item.e2nodeComponentID);
                                 DU_FREE(e2NodeUpdateItem, sizeof(E2nodeComponentConfigUpdate_ItemIEs_t));
                              }
                              DU_FREE(e2NodeUpdateList->list.array, e2NodeUpdateList->list.size);
                           }
                           break;
                        }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval:
                        {
                           e2NodeRemovalList = &e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigRemoval_List;
                           if(e2NodeRemovalList->list.array)
                           {
                              for(e2NodeRemovalListIdx = 0; e2NodeRemovalListIdx< e2NodeRemovalList->list.count; e2NodeRemovalListIdx++)
                              {
                                 e2NodeRemovalItem = (E2nodeComponentConfigRemoval_ItemIEs_t *) e2NodeRemovalList->list.array[e2NodeRemovalListIdx];

                                 freeE2NodeComponentIdentifier(&e2NodeRemovalItem->value.choice.E2nodeComponentConfigRemoval_Item.e2nodeComponentID);
                                 DU_FREE(e2NodeRemovalItem, sizeof(E2nodeComponentConfigRemoval_ItemIEs_t));
                              }
                              DU_FREE(e2NodeRemovalList->list.array, e2NodeRemovalList->list.size);
                           }
                           break;
                        }
                           
                     default:
                        break;
                  }
                  DU_FREE(e2NodeConfigUpdate->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdate_IEs_t));
               }
            }
            DU_FREE(e2NodeConfigUpdate->protocolIEs.list.array, e2NodeConfigUpdate->protocolIEs.list.size);
         }
         DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 node config update msg 
 *
 * @details
 *
 *    Function : BuildAndSendE2NodeConfigUpdate
 *
 *    Functionality:
 *         - Buld and send the E2 node config update msg
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2NodeConfigUpdate(E2NodeConfigList *e2NodeList)
{
   uint8_t ret = RFAILED;
   uint8_t arrIdx = 0,elementCnt = 0, transId=0;
   E2AP_PDU_t  *e2apMsg = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */
   E2nodeConfigurationUpdate_t *e2NodeConfigUpdate = NULLP;

   DU_LOG("\nINFO   -->  E2AP : Building E2 Node config update\n");
   do
   {
      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_E2nodeConfigurationUpdate;
      e2apMsg->choice.initiatingMessage->value.present = \
      InitiatingMessageE2__value_PR_E2nodeConfigurationUpdate;
      e2NodeConfigUpdate = &e2apMsg->choice.initiatingMessage->value.choice.E2nodeConfigurationUpdate;
      
      elementCnt =1;
      if(e2NodeList->addE2NodeCount)
         elementCnt++;
      if(e2NodeList->updateE2NodeCount)
         elementCnt++;
      if(e2NodeList->removeE2NodeCount)
         elementCnt++;

      e2NodeConfigUpdate->protocolIEs.list.count = elementCnt;
      e2NodeConfigUpdate->protocolIEs.list.size  = elementCnt * sizeof(E2nodeConfigurationUpdate_IEs_t*);
      DU_ALLOC(e2NodeConfigUpdate->protocolIEs.list.array, e2NodeConfigUpdate->protocolIEs.list.size);
      if(e2NodeConfigUpdate->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for e2NodeConfigUpdate failed");
         break;
      }
      
      for(arrIdx =0; arrIdx<elementCnt; arrIdx++)
      {
         DU_ALLOC(e2NodeConfigUpdate->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdate_IEs_t));
         if(e2NodeConfigUpdate->protocolIEs.list.array[arrIdx] == NULLP)
         {
            
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for e2NodeConfigUpdate failed");
            break;
         }
      }
      
      if(arrIdx<elementCnt)
         break;

      arrIdx = 0;
      e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdate_IEs__value_PR_TransactionID;
      transId = assignTransactionId();
      e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.choice.TransactionID = transId;

      if(e2NodeList->addE2NodeCount)
      {
         arrIdx++;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAddition;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdate_IEs__value_PR_E2nodeComponentConfigAddition_List;
         if(BuildE2NodeConfigAddList(&(e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAddition_List),\
         ProcedureCodeE2_id_E2nodeConfigurationUpdate, e2NodeList->addE2NodeCount, e2NodeList->addE2Node)!=ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to create E2 Node config list");
            break;
         }
      }
      
      if(e2NodeList->updateE2NodeCount)
      {
         arrIdx++;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdate_IEs__value_PR_E2nodeComponentConfigUpdate_List;
         if(BuildE2NodeConfigUpdateList(&e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigUpdate_List,\
         e2NodeList->updateE2NodeCount, e2NodeList->updateE2Node) != ROK)
         {

            DU_LOG("\nERROR  -->  E2AP : Failed to update the E2 node configuration");
            break;
         }
      }
      
      if(e2NodeList->removeE2NodeCount)
      {
         arrIdx++;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdate_IEs__value_PR_E2nodeComponentConfigRemoval_List;
         if(BuildE2NodeConfigRemoveList(&e2NodeConfigUpdate->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigRemoval_List,\
         e2NodeList->removeE2NodeCount, e2NodeList->removeE2Node) != ROK)
         {

            DU_LOG("\nERROR  -->  E2AP : Failed to remove the E2 node configuration");
            break;
         }
      }

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2nodeConfigurationUpdate structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for E2nodeConfigurationUpdate\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }
      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize))
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 node config update failed");
         break;
      }

      duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId = transId;
      duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode = e2apMsg->choice.initiatingMessage->procedureCode;
      memcpy(&duCb.e2apDb.e2TimersInfo.e2Timers.e2NodeConfigUpdate.configList, e2NodeList, sizeof(E2NodeConfigList));
      ret = ROK;
      break;
   }while(true);
   
   FreeE2NodeConfigUpdate(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Deallocate the memory allocated for E2ResetRequest msg
 *
 * @details
 *
 *    Function : FreeE2ResetRequest
 *
 *    Functionality:
 *       - freeing the memory allocated for E2ResetRequest
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeE2ResetRequest(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   ResetRequestE2_t  *resetReq = NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         resetReq = &e2apMsg->choice.initiatingMessage->value.choice.ResetRequestE2;
         if(resetReq->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < resetReq->protocolIEs.list.count; ieIdx++)
            {
               DU_FREE(resetReq->protocolIEs.list.array[ieIdx], sizeof(ResetRequestIEs_t));
            }
            DU_FREE(resetReq->protocolIEs.list.array, resetReq->protocolIEs.list.size);
         }
         DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build and send the E2 reset request msg
 *
 * @details
 *
 *    Function : BuildAndSendE2ResetRequest
 *
 *    Functionality:
 *         - Buld and send the E2 reset request msg to RIC
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendE2ResetRequest(E2FailureCause resetCause)
{
   uint8_t ieIdx = 0, elementCnt = 0, transId = 0;
   uint8_t ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   ResetRequestE2_t  *resetReq = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Reset Request\n");

   do
   {
      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendE2ResetRequest(): Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendE2ResetRequest(): Memory allocation for initiatingMessage");
         break;
      }

      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_Reset;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_ResetRequestE2;
      resetReq = &e2apMsg->choice.initiatingMessage->value.choice.ResetRequestE2;

      elementCnt = 2;
      resetReq->protocolIEs.list.count = elementCnt;
      resetReq->protocolIEs.list.size = elementCnt * sizeof(ResetRequestIEs_t *);

      DU_ALLOC(resetReq->protocolIEs.list.array, resetReq->protocolIEs.list.size);
      if(!resetReq->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendE2ResetRequest(): Memory allocation failed for \
            Reset Request IE array");
         break;
      }

      for(ieIdx = 0; ieIdx < elementCnt; ieIdx++)
      {
         DU_ALLOC(resetReq->protocolIEs.list.array[ieIdx], sizeof(ResetRequestIEs_t));
         if(!resetReq->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : BuildAndSendE2ResetRequest(): Memory allocation failed for \
            Reset Request IE array element");
            break;
         }
      }

      /* In case of failure */
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0;
      resetReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      resetReq->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      resetReq->protocolIEs.list.array[ieIdx]->value.present = ResetRequestIEs__value_PR_TransactionID;
      transId = assignTransactionId();
      resetReq->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = transId;

      ieIdx++;
      resetReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      resetReq->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_ignore;
      resetReq->protocolIEs.list.array[ieIdx]->value.present = ResetRequestIEs__value_PR_CauseE2;
      fillE2Cause(&resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2, resetCause);

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2SetupRequest structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for E2SetupRequest\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }
      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 Setup request failed");
         break;
      }

      /* In case the message is sent successfully, store the transaction info to
       * be used when response is received */
      duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId = transId;
      duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode = e2apMsg->choice.initiatingMessage->procedureCode;

      ret = ROK;
      break;
   }while(true);

   /* Free all memory */
   FreeE2ResetRequest(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Deallocate the memory allocated for Reset Response msg
 *
 * @details
 *
 *    Function : freeAperDecodingOfE2ResetRsp
 *
 *    Functionality:
 *       - freeing the memory allocated for Reset response
 *
 * @params[in] ResetResponseE2_t *resetResponse
 * @return void
 *
 * ****************************************************************/
void freeAperDecodingOfE2ResetRsp(ResetResponseE2_t *resetResponse)
{
   uint8_t ieIdx;

   if(resetResponse)
   {
      if(resetResponse->protocolIEs.list.array)
      {
         for(ieIdx=0; ieIdx < resetResponse->protocolIEs.list.count; ieIdx++)
         {
            if(resetResponse->protocolIEs.list.array[ieIdx])
            {
               switch(resetResponse->protocolIEs.list.array[ieIdx]->id)
               {
                  case ProtocolIE_IDE2_id_TransactionID:
                     break;

                  case ProtocolIE_IDE2_id_CriticalityDiagnosticsE2:
                     break;
               }
               free(resetResponse->protocolIEs.list.array[ieIdx]);
            }
         }
         free(resetResponse->protocolIEs.list.array);
      }
   }
}

/******************************************************************
 *
 * @brief Processes E2 Reset Response sent by RIC
 *
 * @details
 *
 *    Function : procResetResponse
 *
 *    Functionality: Processes E2 Reset Response sent by RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procResetResponse(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0, transId;
   ResetResponseE2_t *resetResponse;

   DU_LOG("\nINFO   -->  E2AP : E2 Reset Response received");
   resetResponse = &e2apMsg->choice.successfulOutcome->value.choice.ResetResponseE2;;

   for(ieIdx=0; ieIdx < resetResponse->protocolIEs.list.count; ieIdx++)
   {
      switch(resetResponse->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            transId = resetResponse->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
            if((duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId == transId) && \
                  (duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode == e2apMsg->choice.successfulOutcome->procedureCode))
            {
               memset(&duCb.e2apDb.e2TransInfo.e2InitTransaction[transId], 0, sizeof(E2TransInfo));
            }
            else
            {
               DU_LOG("\nERROR  -->  E2AP : Invalid transaction id [%d]", transId);
               return RFAILED;
            }
            break;
         case ProtocolIE_IDE2_id_CriticalityDiagnosticsE2:
            /* As per ORAN WG3 E2AP spec v3.0, section 9.2.2
               Criticality Diagnostics IE is sent by Near-RT RIC when parts of a received message i.e. 
               Reset Request in this case, have not been comprehended or were missing, or if the message 
               contained logical errors.

               Processing of this ID should be implemented when negative call flows are to be supported.
             */
            break;
         default:
            DU_LOG("\nERROR  -->  E2AP : Invalid IE received in E2 Reset Response : %ld",
                  resetResponse->protocolIEs.list.array[ieIdx]->id);
            break;
      }
   }

   freeAperDecodingOfE2ResetRsp(resetResponse);
   return ROK;
}

/******************************************************************
 *
 * @brief Deallocation of memory allocated by aper decoder for e2 setup Failure
 *
 * @details
 *
 *    Function : freeAperDecodingOfE2SetupFailure
 *
 *    Functionality: Deallocation of memory allocated by aper decoder for e2
 *    setup Failure
 *
 * @params[in] E2setupFailure_t *e2SetupFailure;
 * @return void
 *
 * ****************************************************************/
void freeAperDecodingOfE2SetupFailure(E2setupFailure_t *e2SetupFailure)
{
   uint8_t arrIdx;

   if(e2SetupFailure)
   {
      if(e2SetupFailure->protocolIEs.list.array)
      {
         for(arrIdx=0; arrIdx<e2SetupFailure->protocolIEs.list.count; arrIdx++)
         {
            if(e2SetupFailure->protocolIEs.list.array[arrIdx])
            {
               free(e2SetupFailure->protocolIEs.list.array[arrIdx]);  
            }
         }
         free(e2SetupFailure->protocolIEs.list.array);
      }
   }
}
/******************************************************************
 *
 * @brief Processes E2 Setup Failure sent by RIC
 *
 * @details
 *
 *    Function : procE2SetupFailure
 *
 *    Functionality: Processes E2 Setup failure sent by RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void procE2SetupFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx =0, transId =0, timerValue=0; 
   E2setupFailure_t *e2SetupFailure;

   DU_LOG("\nINFO   -->  E2AP : E2 Setup failure received"); 
   e2SetupFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2setupFailure;

   for(arrIdx=0; arrIdx<e2SetupFailure->protocolIEs.list.count; arrIdx++)
   {
      switch(e2SetupFailure->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
         {
            transId = e2SetupFailure->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
            if((duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId == transId) &&\
                  (duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode == e2apMsg->choice.unsuccessfulOutcome->procedureCode))
            {
               memset(&duCb.e2apDb.e2TransInfo.e2InitTransaction[transId], 0, sizeof(E2TransInfo));
            }
            else
            {
               DU_LOG("\nERROR  -->  E2AP : Invalid transaction id [%d]", transId);
               return ;
            }
            break;
         }
         case ProtocolIE_IDE2_id_TimeToWaitE2:
            {
               timerValue = convertE2WaitTimerEnumToValue(e2SetupFailure->protocolIEs.list.array[arrIdx]->value.choice.TimeToWaitE2);
               if((duChkTmr((PTR)&(duCb.e2apDb.e2TimersInfo.e2Timers.e2SetupTimer), EVENT_E2_SETUP_TMR)) == FALSE)
               {
                  duStartTmr((PTR)&(duCb.e2apDb.e2TimersInfo.e2Timers.e2SetupTimer), EVENT_E2_SETUP_TMR, timerValue);
               }
               else
               {
                  DU_LOG("\nERROR   -->  E2AP : EVENT_E2_SETUP_TMR timer is already running");
                  return;
               }
               break; 
            }
      }
   }

   freeAperDecodingOfE2SetupFailure(e2SetupFailure);
}
/******************************************************************
 *
 * @brief Deallocation of memory allocated by aper decoder for RIC service Query
 *
 * @details
 *
 *    Function : freeAperDecodingOfRicServiceQuery
 *
 *    Functionality: Deallocation of memory allocated by aper decoder for RIC
 *    service Query
 *
 * @params[in] RICserviceQuery_t *ricServiceQuery;
 * @return void
 *
 * ****************************************************************/

void freeAperDecodingOfRicServiceQuery(RICserviceQuery_t *ricServiceQuery)
{
   uint8_t arrIdx,ranFuncIdx;
    RANfunctionsID_List_t *ranFuncAddedList;

   if(ricServiceQuery)
   {
      if(ricServiceQuery->protocolIEs.list.array)
      {
         for(arrIdx=0; arrIdx<ricServiceQuery->protocolIEs.list.count; arrIdx++)
         {
            if(ricServiceQuery->protocolIEs.list.array[arrIdx])
            {
               switch(ricServiceQuery->protocolIEs.list.array[arrIdx]->id)
               {
                  case ProtocolIE_IDE2_id_RANfunctionsAccepted:
                  {
                     ranFuncAddedList= &ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                     if(ranFuncAddedList->list.array)
                     {
                        for(ranFuncIdx=0;ranFuncIdx<ranFuncAddedList->list.count; ranFuncIdx++)
                        {
                           free(ranFuncAddedList->list.array[ranFuncIdx]);
                        }
                        free(ranFuncAddedList->list.array);;
                     }
                     break;
                  }
                  default:
                     break;
               }
               free(ricServiceQuery->protocolIEs.list.array[arrIdx]);
            }
         }
         free(ricServiceQuery->protocolIEs.list.array);
      }
   }
}
/*******************************************************************
 *
 * @brief Build RanFunction Delete List
 *
 * @details
 *
 *    Function : BuildRanFunctionDeleteList
 *
 * Functionality:  Build RanFunction Delete List
 *
 * @params[in]
 *    RANfunctionsID List
 *    Count of the RAN function
 *    Received RAN function list
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildRanFunctionDeleteList(RANfunctionsID_List_t *deleteList, uint8_t count, RanFuncInfo *recvdRanFunc)
{
   uint8_t ranFuncIdx=0;
   RANfunctionID_ItemIEs_t *delRanFuncItem;

   if(count)
   {
      deleteList->list.count = count;
      deleteList->list.size = deleteList->list.count * sizeof(RANfunctionID_ItemIEs_t*);
      DU_ALLOC(deleteList->list.array, deleteList->list.size);
      if(deleteList->list.array == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed in %s at %d",__func__, __LINE__);
         return RFAILED;
      }
      for(ranFuncIdx = 0; ranFuncIdx< deleteList->list.count; ranFuncIdx++)
      {
         DU_ALLOC(deleteList->list.array[ranFuncIdx], sizeof(RANfunctionID_ItemIEs_t));
         if(deleteList->list.array[ranFuncIdx] == NULLP)
         {
            DU_LOG("\nERROR  --> E2AP: Memory allocation failed in %s at %d",__func__, __LINE__);
            return RFAILED;
         }
         delRanFuncItem= (RANfunctionID_ItemIEs_t *) deleteList->list.array[ranFuncIdx];
         delRanFuncItem->id = ProtocolIE_IDE2_id_RANfunctionID_Item;
         delRanFuncItem->criticality = CriticalityE2_ignore;
         delRanFuncItem->value.choice.RANfunctionID_Item.ranFunctionID = recvdRanFunc[ranFuncIdx].id;
         delRanFuncItem->value.choice.RANfunctionID_Item.ranFunctionRevision = recvdRanFunc[ranFuncIdx].revisionCounter;

      }
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief De Allocate  Ric Service Update message
 *
 * @details
 *
 *    Function : FreeRicServiceUpdate
 *
 *    Functionality: De-Allocating Ric Service Update message
 *
 * @params[in] E2AP_PDU_t *e2apMsg

 * @return void
 *
 * ****************************************************************/

void FreeRicServiceUpdate(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0;
   uint8_t ranFuncAddListIdx=0, ranFuncDelIdx=0;
   RICserviceUpdate_t *ricServiceUpdate;
   RANfunctions_List_t *ranFunctionsList;
   RANfunction_ItemIEs_t *ranFuncItemIe;
   RANfunction_Item_t  *ranFunItem;
   RANfunctionsID_List_t *deleteList;

   /* De-allocating Memory */
   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         ricServiceUpdate = &e2apMsg->choice.initiatingMessage->value.choice.RICserviceUpdate;
         if(ricServiceUpdate->protocolIEs.list.array != NULLP)
         {
            for(arrIdx = 0; arrIdx < ricServiceUpdate->protocolIEs.list.count; arrIdx++)
            {
               if(ricServiceUpdate->protocolIEs.list.array[arrIdx] != NULLP)
               {
                  switch(ricServiceUpdate->protocolIEs.list.array[arrIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_TransactionID:
                        break;

                     case ProtocolIE_IDE2_id_RANfunctionsAdded:
                     case ProtocolIE_IDE2_id_RANfunctionsModified:
                        {
                           ranFunctionsList = &(ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List);
                           if(ranFunctionsList->list.array)
                           {
                              for(ranFuncAddListIdx= 0; ranFuncAddListIdx< ranFunctionsList->list.count; ranFuncAddListIdx++)
                              {
                                 if(ranFunctionsList->list.array[ranFuncAddListIdx])
                                 {
                                    ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFunctionsList->list.array[ranFuncAddListIdx];
                                    ranFunItem = &ranFuncItemIe->value.choice.RANfunction_Item;
                                    DU_FREE(ranFunItem->ranFunctionOID.buf, ranFunItem->ranFunctionOID.size);
                                    DU_FREE(ranFunItem->ranFunctionDefinition.buf, ranFunItem->ranFunctionDefinition.size);
                                    DU_FREE(ranFunctionsList->list.array[ranFuncAddListIdx], sizeof(RANfunction_ItemIEs_t));
                                 }
                              }
                              DU_FREE(ranFunctionsList->list.array, ranFunctionsList->list.size);
                           }
                           break;
                        }
                     case ProtocolIE_IDE2_id_RANfunctionsDeleted:
                        {
                           deleteList= &ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                           if(deleteList->list.array)
                           {
                              for(ranFuncDelIdx = 0; ranFuncDelIdx< deleteList->list.count; ranFuncDelIdx++)
                              {
                                 DU_FREE(deleteList->list.array[ranFuncDelIdx], sizeof(RANfunctionID_ItemIEs_t));
                              }
                              DU_FREE(deleteList->list.array, deleteList->list.size);
  
                           }
                           break;
                        }
                     default:
                        DU_LOG("\nERROR  --> E2AP: Invalid event at ricServiceUpdate %ld ",\
                              (ricServiceUpdate->protocolIEs.list.array[arrIdx]->id));
                        break;
                  }
                  DU_FREE(ricServiceUpdate->protocolIEs.list.array[arrIdx], sizeof(RICserviceUpdate_IEs_t));
               }
            }
            DU_FREE(ricServiceUpdate->protocolIEs.list.array, ricServiceUpdate->protocolIEs.list.size);
         }
         DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds and Send the RicServiceUpdateuest
 *
 * @details
 *
 *    Function : BuildAndSendRicServiceUpdate
 *
 * Functionality:Fills the RicServiceUpdateuest
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendRicServiceUpdate(RicServiceUpdate serviceUpdate)
{
   uint8_t arrIdx = 0, elementCnt=0;
   uint8_t transId = 0, ret = RFAILED;
   bool memAllocFailed =false;
   E2AP_PDU_t        *e2apMsg = NULLP;
   RICserviceUpdate_t  *ricServiceUpdate = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building Ric Service Update\n");
   do
   {
      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICserviceUpdate;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICserviceUpdate;
      ricServiceUpdate = &e2apMsg->choice.initiatingMessage->value.choice.RICserviceUpdate;
      
      /* For TransId IE, set elementCnt to 1.
      If there is any item in the RAN function add list, RAN function modification list, or RAN function delete list, increment the elementCnt.*/

      elementCnt =1;
      if(serviceUpdate.recvRanFuncList.numOfRanFunToBeAdded)
        elementCnt++;
      if(serviceUpdate.recvRanFuncList.numOfRanFunToBeModified)
         elementCnt++;
      if(serviceUpdate.recvRanFuncList.numOfRanFunToBeDeleted)
         elementCnt++;
       
      ricServiceUpdate->protocolIEs.list.count = elementCnt;
      ricServiceUpdate->protocolIEs.list.size = elementCnt * sizeof(RICserviceUpdate_IEs_t*);

      /* Initialize the E2Setup members */
      DU_ALLOC(ricServiceUpdate->protocolIEs.list.array, ricServiceUpdate->protocolIEs.list.size);
      if(ricServiceUpdate->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for array elements");
         break;
      }
      
      for(arrIdx = 0; arrIdx < elementCnt; (arrIdx)++)
      {
         DU_ALLOC(ricServiceUpdate->protocolIEs.list.array[arrIdx], sizeof(RICserviceUpdate_IEs_t));
         if(ricServiceUpdate->protocolIEs.list.array[arrIdx] == NULLP)
         {
            memAllocFailed = true;
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for arrayIdx [%d]", arrIdx);
            break;
         }
      }
      if(memAllocFailed == true)
         break;

      arrIdx = 0;

      /* TransactionID */
      ricServiceUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      ricServiceUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.present = E2setupRequestIEs__value_PR_TransactionID;
      if(serviceUpdate.dir == E2_NODE_INITIATED)
         transId = assignTransactionId();
      else
        transId = serviceUpdate.transId;
      ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.TransactionID = transId;

      if(serviceUpdate.recvRanFuncList.numOfRanFunToBeAdded)
      {
         arrIdx++;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsAdded;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdate_IEs__value_PR_RANfunctions_List;
         if(BuildRanFunctionAddList(&ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List,\
         e2apMsg->choice.initiatingMessage->procedureCode, serviceUpdate.recvRanFuncList.numOfRanFunToBeAdded, serviceUpdate.recvRanFuncList.ranFunToBeAdded) !=ROK)
         {
            break;
         }
      }

      if(serviceUpdate.recvRanFuncList.numOfRanFunToBeModified)
      {
         arrIdx++;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsModified;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdate_IEs__value_PR_RANfunctions_List;
         if(BuildRanFunctionAddList(&ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List,
         e2apMsg->choice.initiatingMessage->procedureCode, serviceUpdate.recvRanFuncList.numOfRanFunToBeModified, serviceUpdate.recvRanFuncList.ranFunToBeModified) !=ROK)
         {
            break;
         }
      }

      if(serviceUpdate.recvRanFuncList.numOfRanFunToBeDeleted)
      {
         arrIdx++;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsDeleted;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdate_IEs__value_PR_RANfunctionsID_List;
         if(BuildRanFunctionDeleteList(&ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List,\
         serviceUpdate.recvRanFuncList.numOfRanFunToBeDeleted, serviceUpdate.recvRanFuncList.ranFunToBeDeleted) != ROK)
         {
            break;
         }
      }
      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RicServiceUpdateuest structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for RicServiceUpdateuest\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }
      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 Setup request failed");
         break;
      }
      ret = ROK;
      break;
   }while(true);
   
   if(ret == ROK)
   {
      if(serviceUpdate.dir == E2_NODE_INITIATED)
      {
         duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId = transId;
         duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode = e2apMsg->choice.initiatingMessage->procedureCode;
      }
      else
      {
         duCb.e2apDb.e2TransInfo.ricInitTransaction[transId].transactionId = transId;
         duCb.e2apDb.e2TransInfo.ricInitTransaction[transId].procedureCode = e2apMsg->choice.initiatingMessage->procedureCode;
      }
      duCb.e2apDb.e2TimersInfo.e2Timers.ricServiceUpdateTimer.ricService.dir = serviceUpdate.dir;
      duCb.e2apDb.e2TimersInfo.e2Timers.ricServiceUpdateTimer.ricService.transId =transId;
      memcpy(&duCb.e2apDb.e2TimersInfo.e2Timers.ricServiceUpdateTimer.ricService.recvRanFuncList, &serviceUpdate.recvRanFuncList, sizeof(E2TmpRanFunList));
   }
   FreeRicServiceUpdate(e2apMsg);
   return ret;
}
/******************************************************************
 *
 * @brief Processes RIC service Query sent by RIC
 *
 * @details
 *
 *    Function : procRicServiceQuery
 *
 *    Functionality: Processes RIC service Query sent by RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void procRicServiceQuery(E2AP_PDU_t *e2apMsg)
{
   ConfigType action;
   uint16_t arrIdx =0, ranFuncIdx=0,tmpIdx=0;
   uint16_t id,revisionCcounter;
   bool tmpArray[MAX_RAN_FUNCTION] = {false};
   RICserviceQuery_t *ricServiceQuery=NULL;
   RicServiceUpdate ricUpdate;
   RANfunctionID_ItemIEs_t *ranFuncAddedItemIe;
   RANfunctionsID_List_t *ranFuncAddedList;

   DU_LOG("\nINFO   -->  E2AP : RIC Service Query received");
   memset(&ricUpdate, 0, sizeof(RicServiceUpdate));
   ricUpdate.dir = RIC_INITIATED;
   ricServiceQuery = &e2apMsg->choice.initiatingMessage->value.choice.RICserviceQuery;

   for(arrIdx=0; arrIdx<ricServiceQuery->protocolIEs.list.count; arrIdx++)
   {
      switch(ricServiceQuery->protocolIEs.list.array[arrIdx]->id)
      {
         /* TODO completing in next patch/gerrit */
         case ProtocolIE_IDE2_id_TransactionID:
         {
            ricUpdate.transId = ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
            break;
         }

         case ProtocolIE_IDE2_id_RANfunctionsAccepted:
         {
            ranFuncAddedList= &ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
            if(ranFuncAddedList->list.array)
            {
               for(ranFuncIdx=0;ranFuncIdx<ranFuncAddedList->list.count; ranFuncIdx++)
               {
                  if(ranFuncAddedList->list.array[ranFuncIdx])
                  {
                     /* Using the RAN function Id, identify the RAN function to be modified or deleted.  */
                     
                     ranFuncAddedItemIe = (RANfunctionID_ItemIEs_t*)ranFuncAddedList->list.array[ranFuncIdx];
                     id = ranFuncAddedItemIe->value.choice.RANfunctionID_Item.ranFunctionID;
                     revisionCcounter = ranFuncAddedItemIe->value.choice.RANfunctionID_Item.ranFunctionRevision;
                     
                     if((id != duCb.e2apDb.ranFunction[id-1].id))
                     {
                        action = CONFIG_DEL;
                     }
                     else if((id == duCb.e2apDb.ranFunction[id-1].id)&&(revisionCcounter!=duCb.e2apDb.ranFunction[id-1].revisionCounter))
                     {
                        action = CONFIG_MOD;
                     }

                     if(action == CONFIG_DEL)
                     {
                        ricUpdate.recvRanFuncList.ranFunToBeDeleted[ricUpdate.recvRanFuncList.numOfRanFunToBeDeleted].id = id;
                        ricUpdate.recvRanFuncList.ranFunToBeDeleted[ricUpdate.recvRanFuncList.numOfRanFunToBeDeleted].revisionCounter = revisionCcounter;
                        ricUpdate.recvRanFuncList.numOfRanFunToBeDeleted++;
                     }
                     else if(action == CONFIG_MOD)
                     {
                        ricUpdate.recvRanFuncList.ranFunToBeModified[ricUpdate.recvRanFuncList.numOfRanFunToBeModified].id = id;
                        ricUpdate.recvRanFuncList.ranFunToBeModified[ricUpdate.recvRanFuncList.numOfRanFunToBeModified].revisionCounter = revisionCcounter;
                        ricUpdate.recvRanFuncList.numOfRanFunToBeModified++;
                     }

                     /* If any ID is set to true, it means that the ID has been used in either modification or deletion list. 
                      * Else we will add the IDs into the added list */
                     tmpArray[id-1] = true;
                  }
               }
            }
            break;
         }
      }
   }

   /*  Traversing the whole RAN function list in ducb to check if any new Ran function ids have been added. */
   for(arrIdx =0; arrIdx<MAX_RAN_FUNCTION; arrIdx++)
   {
      tmpIdx= ricUpdate.recvRanFuncList.numOfRanFunToBeAdded;
      if((duCb.e2apDb.ranFunction[arrIdx].id >0)&&(!tmpArray[arrIdx]))
      {
         ricUpdate.recvRanFuncList.ranFunToBeAdded[tmpIdx].id = duCb.e2apDb.ranFunction[arrIdx].id;
         ricUpdate.recvRanFuncList.ranFunToBeAdded[tmpIdx].revisionCounter = duCb.e2apDb.ranFunction[arrIdx].revisionCounter;
         ricUpdate.recvRanFuncList.numOfRanFunToBeAdded++;
      }
   }

   if(BuildAndSendRicServiceUpdate(ricUpdate)!= ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to build and send ric service update message");
   }

   freeAperDecodingOfRicServiceQuery(ricServiceQuery);
}

/******************************************************************
 *
 * @brief Deallocation of memory allocated by aper decoder for 
 *    RIC service update ack
 *
 * @details
 *
 *    Function : freeAperDecodingOfRicServiceUpdateAck
 *
 *    Functionality: Deallocation of memory allocated by aper decoder 
 *    for RIC service update ack
 *
 * @params[in] RICserviceUpdateAck_t *ricServiceAck;
 * @return void
 *
 * ****************************************************************/

void freeAperDecodingOfRicServiceUpdateAck(RICserviceUpdateAcknowledge_t *ricServiceAck)
{
   uint8_t arrIdx=0,ranFuncIdx=0;
   RANfunctionsID_List_t *ranFuncAddedList=NULL;

   if(ricServiceAck)
   {
      if(ricServiceAck->protocolIEs.list.array)
      {
         for(arrIdx=0; arrIdx<ricServiceAck->protocolIEs.list.count; arrIdx++)
         {
            if(ricServiceAck->protocolIEs.list.array[arrIdx])
            {
               switch(ricServiceAck->protocolIEs.list.array[arrIdx]->id)
               {
                  case ProtocolIE_IDE2_id_RANfunctionsAccepted:
                  {
                     ranFuncAddedList= &ricServiceAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                     if(ranFuncAddedList->list.array)
                     {
                        for(ranFuncIdx=0;ranFuncIdx<ranFuncAddedList->list.count; ranFuncIdx++)
                        {
                           free(ranFuncAddedList->list.array[ranFuncIdx]);
                        }
                        free(ranFuncAddedList->list.array);
                     }
                     break;
                  }
                  default:
                     break;
               }
               free(ricServiceAck->protocolIEs.list.array[arrIdx]);  
            }
         }
         free(ricServiceAck->protocolIEs.list.array);
      }
   }
}

/******************************************************************
 *
 * @brief Processes RIC service update ack sent by RIC
 *
 * @details
 *
 *    Function : procRicServiceUpdateAck
 *
 *    Functionality: Processes RIC service update ack sent by RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void procRicServiceUpdateAck(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx =0, transId =0; 
   uint16_t id =0, tmpIdx=0, ranFuncIdx=0;
   RicServiceUpdate serviceUpdate;
   RANfunctionsIDcause_List_t *rejectedList=NULL;
   RICserviceUpdateAcknowledge_t *ricServiceAck=NULL;
   RANfunctionIDcause_ItemIEs_t *ranFuncRejectedItemIe=NULL;
   
   DU_LOG("\nINFO   -->  E2AP : RIC service update ack received"); 
   memset(&serviceUpdate, 0, sizeof(RicServiceUpdate));
   ricServiceAck = &e2apMsg->choice.successfulOutcome->value.choice.RICserviceUpdateAcknowledge;
   
   for(arrIdx=0; arrIdx<ricServiceAck->protocolIEs.list.count; arrIdx++)
   {
      switch(ricServiceAck->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
         {
            transId = ricServiceAck->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
            if((duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].transactionId == transId) &&\
            (duCb.e2apDb.e2TransInfo.e2InitTransaction[transId].procedureCode == e2apMsg->choice.unsuccessfulOutcome->procedureCode))
            {
              memset(&duCb.e2apDb.e2TransInfo.e2InitTransaction[transId], 0, sizeof(E2TransInfo));
            }
            else if((duCb.e2apDb.e2TransInfo.ricInitTransaction[transId].transactionId == transId) &&\
            (duCb.e2apDb.e2TransInfo.ricInitTransaction[transId].procedureCode == e2apMsg->choice.unsuccessfulOutcome->procedureCode))
            {
              memset(&duCb.e2apDb.e2TransInfo.ricInitTransaction[transId], 0, sizeof(E2TransInfo));
            }
            else
            {
               DU_LOG("\nERROR  -->  E2AP : Invalid transaction id [%d]", transId);
               return ;
            }
            break;
         }
         
         case ProtocolIE_IDE2_id_RANfunctionsAccepted:
            break;

         case ProtocolIE_IDE2_id_RANfunctionsRejected:
         {
            rejectedList= &ricServiceAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsIDcause_List;
            if(rejectedList->list.array)
            {
               for(ranFuncIdx=0;ranFuncIdx<rejectedList->list.count; ranFuncIdx++)
               {
                  ranFuncRejectedItemIe =  (RANfunctionIDcause_ItemIEs_t*)rejectedList->list.array[ranFuncIdx];
                  id = ranFuncRejectedItemIe->value.choice.RANfunctionIDcause_Item.ranFunctionID;
                  tmpIdx= serviceUpdate.recvRanFuncList.numOfRanFunToBeAdded;
                  serviceUpdate.recvRanFuncList.ranFunToBeAdded[tmpIdx].id = duCb.e2apDb.ranFunction[id-1].id;
                  serviceUpdate.recvRanFuncList.ranFunToBeAdded[tmpIdx].revisionCounter = duCb.e2apDb.ranFunction[id-1].revisionCounter;
                  serviceUpdate.recvRanFuncList.numOfRanFunToBeAdded++;
               }
            }
            break;
         }

      }
   }

   if(serviceUpdate.recvRanFuncList.numOfRanFunToBeAdded)
   {
      serviceUpdate.dir = E2_NODE_INITIATED;
      BuildAndSendRicServiceUpdate(serviceUpdate);
   }
   freeAperDecodingOfRicServiceUpdateAck(ricServiceAck);
}

/******************************************************************
 *
 * @brief Deallocation of memory allocated by aper decoder for 
 *       RIC service update failure
 *
 * @details
 *
 *    Function : freeAperDecodingOfRicServiceUpdateFailure
 *
 *    Functionality: Deallocation of memory allocated by aper decoder 
 *    for RIC service update failure
 *
 * @params[in] RICserviceUpdateFailure_t *ricServiceFailure;
 * @return void
 *
 * ****************************************************************/

void freeAperDecodingOfRicServiceUpdateFailure(RICserviceUpdateFailure_t *ricServiceFailure)
{
   uint8_t arrIdx=0;

   if(ricServiceFailure)
   {
      if(ricServiceFailure->protocolIEs.list.array)
      {
         for(arrIdx=0; arrIdx<ricServiceFailure->protocolIEs.list.count; arrIdx++)
         {
            if(ricServiceFailure->protocolIEs.list.array[arrIdx])
            {
               free(ricServiceFailure->protocolIEs.list.array[arrIdx]);  
            }
         }
         free(ricServiceFailure->protocolIEs.list.array);
      }
   }
}

/******************************************************************
 *
 * @brief Processes RIC service update failure sent by RIC
 *
 * @details
 *
 *    Function : procRicServiceUpdateFailure
 *
 *    Functionality: Processes RIC service update failure sent by RIC
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void procRicServiceUpdateFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx =0, timerValue=0; 
   RICserviceUpdateFailure_t *ricServiceFailure=NULL;

   DU_LOG("\nINFO   -->  E2AP : RIC service update failure received"); 
   ricServiceFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICserviceUpdateFailure;

   for(arrIdx=0; arrIdx<ricServiceFailure->protocolIEs.list.count; arrIdx++)
   {
      switch(ricServiceFailure->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            {
               break;
            }
         case ProtocolIE_IDE2_id_TimeToWaitE2:
            {
               timerValue = convertE2WaitTimerEnumToValue(ricServiceFailure->protocolIEs.list.array[arrIdx]->value.choice.TimeToWaitE2);
               if((duChkTmr((PTR)&(duCb.e2apDb.e2TimersInfo.e2Timers.ricServiceUpdateTimer), EVENT_RIC_SERVICE_UPDATE_TMR)) == FALSE)
               {
                  duStartTmr((PTR)&(duCb.e2apDb.e2TimersInfo.e2Timers.ricServiceUpdateTimer), EVENT_RIC_SERVICE_UPDATE_TMR, timerValue);
               }
               else
               {
                  DU_LOG("\nERROR   -->  E2AP : EVENT_RIC_SERVICE_UPDATE_TMR  timer is already running");
                  return;
               }
               break; 
            }
         case ProtocolIE_IDE2_id_CauseE2:
            {
               break;
            }
      }
   }

   freeAperDecodingOfRicServiceUpdateFailure(ricServiceFailure);
}

/******************************************************************
 *
 * @brief DU Send E2 Node Configuration Update
 *
 * @details
 *
 *    Function : duSendE2NodeConfigurationUpdate 
 *
 *    Functionality: DU Send E2 Node Configuration Update
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duSendE2NodeConfigurationUpdate()
{
   E2NodeConfigList e2NodeList;
   CmLList *node =NULL;
   E2NodeComponent *e2NodeComponentInfo=NULL;

   memset(&e2NodeList, 0, sizeof(E2NodeConfigList));
   CM_LLIST_FIRST_NODE(&duCb.e2apDb.e2NodeComponentList, node);
   while(node)
   {
      e2NodeComponentInfo = (E2NodeComponent*)node->node;

      if(e2NodeComponentInfo->componentRequestPart && e2NodeComponentInfo->componentResponsePart)
      {
         switch(e2NodeComponentInfo->componentActionType)
         {
            case E2_NODE_COMPONENT_ADD:
               {
                  e2NodeList.addE2Node[e2NodeList.addE2NodeCount].interface = e2NodeComponentInfo->interfaceType;
                  e2NodeList.addE2Node[e2NodeList.addE2NodeCount].actionType = e2NodeComponentInfo->componentActionType;
                  e2NodeList.removeE2NodeCount++;
                  break;
               }
            case E2_NODE_COMPONENT_UPDATE:
               {
                  e2NodeList.updateE2Node[e2NodeList.updateE2NodeCount].interface = e2NodeComponentInfo->interfaceType;
                  e2NodeList.updateE2Node[e2NodeList.updateE2NodeCount].actionType = e2NodeComponentInfo->componentActionType;
                  e2NodeList.updateE2NodeCount++;
                  break;

               }
            case E2_NODE_COMPONENT_DEL:
               {
                  e2NodeList.removeE2Node[e2NodeList.removeE2NodeCount].interface = e2NodeComponentInfo->interfaceType;
                  e2NodeList.removeE2Node[e2NodeList.removeE2NodeCount].actionType = e2NodeComponentInfo->componentActionType;
                  e2NodeList.removeE2NodeCount++;
                  break;
               }
         }
      }
      node = node->next;
   }

   if(BuildAndSendE2NodeConfigUpdate(&e2NodeList) !=ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to build and send e2 node config update message to RIC_stub");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Free RIC Subscription Modification Required
 *
 * @details
 *
 *    Function : FreeRicSubsModRequired
 *
 * Functionality: Freqq RIC Subscription Modification required
 *
 * @param  E2AP Message PDU to be freed
 * @return void
 *
 ******************************************************************/
void FreeRicSubsModRequired(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx = 0, arrIdx = 0;
   RICsubscriptionModificationRequired_t  *ricSubsModReqd = NULLP;
   RICsubscriptionModificationRequired_IEs_t *ricSubsModReqdIe = NULLP;
   RICactions_RequiredToBeModified_List_t *actionToBeModList = NULLP;
   RICactions_RequiredToBeRemoved_List_t  *actionToBeRmvList = NULLP;

   if(e2apMsg)
   {
      if(e2apMsg->choice.initiatingMessage)
      {
         ricSubsModReqd = &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionModificationRequired;
         if(ricSubsModReqd->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < ricSubsModReqd->protocolIEs.list.count; ieIdx++)
            {
               if(ricSubsModReqd->protocolIEs.list.array[ieIdx])
               {
                  ricSubsModReqdIe = ricSubsModReqd->protocolIEs.list.array[ieIdx];
                  switch(ricSubsModReqdIe->id)
                  {
                     case ProtocolIE_IDE2_id_RICactionsRequiredToBeModified_List:
                        {
                           actionToBeModList = &ricSubsModReqdIe->value.choice.RICactions_RequiredToBeModified_List;
                           if(actionToBeModList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < actionToBeModList->list.count; arrIdx++)
                              { 
                                 DU_FREE(actionToBeModList->list.array[arrIdx], \
                                    sizeof(RICaction_RequiredToBeModified_ItemIEs_t));
                              }
                              DU_FREE(actionToBeModList->list.array, actionToBeModList->list.size);
                           }
                           break;
                        }

                     case ProtocolIE_IDE2_id_RICactionsRequiredToBeRemoved_List:
                        {
                           actionToBeRmvList = &ricSubsModReqdIe->value.choice.RICactions_RequiredToBeRemoved_List;
                           if(actionToBeRmvList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < actionToBeRmvList->list.count; arrIdx++)
                              { 
                                 DU_FREE(actionToBeRmvList->list.array[arrIdx], \
                                    sizeof(RICaction_RequiredToBeRemoved_ItemIEs_t));
                              }
                              DU_FREE(actionToBeRmvList->list.array, actionToBeRmvList->list.size);
                           }
                           break;
                        }

                     default:
                        break;
                  }
                  DU_FREE(ricSubsModReqd->protocolIEs.list.array[ieIdx], \
                        sizeof(RICsubscriptionModificationRequired_IEs_t));
               }
            }
            DU_FREE(ricSubsModReqd->protocolIEs.list.array, ricSubsModReqd->protocolIEs.list.size);
         }
         DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/* A RIC Subscription includes RIC subsequent action only for RIC Insert service.
 * However, E2SM-KPM supports only RIC Report service.
 * Hence there is no subsequent action in RIC subscription that may require modification.
 * So commenting the action-modification IEs for the time being
 */
#if 0
/*******************************************************************
 *
 * @brief Fill Action required to be modified list
 *
 * @details
 *
 *    Function : FillActionReqdToBeModList
 *
 * Functionality: Fill Action required to be modified list
 *
 * @param  RIC Actions Required To Be Modified List to be filled
 *         Number of actions to be modified
 *         RIC Subscription DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t FillActionReqdToBeModList(RICactions_RequiredToBeModified_List_t *actionToBeModList, uint8_t numActionsMod, \
   RicSubscription *ricSubscription)
{
   uint8_t arrIdx = 0, actionIdx = 0;
   RICaction_RequiredToBeModified_ItemIEs_t *actionToBeMod = NULL;

   actionToBeModList->list.count = numActionsMod;
   actionToBeModList->list.size = numActionsMod * sizeof(RICaction_RequiredToBeModified_ItemIEs_t *);
   DU_ALLOC(actionToBeModList->list.array, actionToBeModList->list.size);
   if(!actionToBeModList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
      return RFAILED;
   }

   arrIdx = 0;
   for(actionIdx = 0; actionIdx < MAX_RIC_ACTION; actionIdx++)
   {
      if(ricSubscription->actionSequence[actionIdx].action == CONFIG_MOD)
      {
         DU_ALLOC(actionToBeModList->list.array[arrIdx], sizeof(RICaction_RequiredToBeModified_ItemIEs_t));
         if(!actionToBeModList->list.array[arrIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
            return RFAILED;
         }
         actionToBeMod = (RICaction_RequiredToBeModified_ItemIEs_t *)actionToBeModList->list.array[arrIdx];

         actionToBeMod->id = ProtocolIE_IDE2_id_RICaction_RequiredToBeModified_Item;
         actionToBeMod->criticality = CriticalityE2_reject;
         actionToBeMod->value.present = \
            RICaction_RequiredToBeModified_ItemIEs__value_PR_RICaction_RequiredToBeModified_Item;
         actionToBeMod->value.choice.RICaction_RequiredToBeModified_Item.ricActionID = \
            ricSubscription->actionSequence[actionIdx].actionId;
         actionToBeMod->value.choice.RICaction_RequiredToBeModified_Item.ricTimeToWait = RICtimeToWait_w5ms;

         arrIdx++;
      }
   }

   return ROK;
}
#endif

/*******************************************************************
 *
 * @brief Fill Action required to be removed list
 *
 * @details
 *
 *    Function : FillActionReqdToBeRmvList
 *
 * Functionality: Fill Action required to be removed list
 *
 * @param  RIC Actions Required To Be Removed List to be filled
 *         Number of actions to be removed
 *         RIC Subscription DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t FillActionReqdToBeRmvList(RICactions_RequiredToBeRemoved_List_t *actionToBeRmvList, uint8_t numActionsRmv, \
   RicSubscription *ricSubscription)
{
   uint8_t arrIdx = 0, actionIdx = 0;
   RICaction_RequiredToBeRemoved_ItemIEs_t *actionToBeRmv = NULL;

   actionToBeRmvList->list.count = numActionsRmv;
   actionToBeRmvList->list.size = numActionsRmv * sizeof(RICaction_RequiredToBeRemoved_ItemIEs_t *);
   DU_ALLOC(actionToBeRmvList->list.array, actionToBeRmvList->list.size);
   if(!actionToBeRmvList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed at line %d", __func__, __LINE__);
      return RFAILED;
   }

   arrIdx = 0;
   for(actionIdx = 0; actionIdx < MAX_RIC_ACTION; actionIdx++)
   {
      if(ricSubscription->actionSequence[actionIdx].action == CONFIG_DEL)
      {
         DU_ALLOC(actionToBeRmvList->list.array[arrIdx], sizeof(RICaction_RequiredToBeRemoved_ItemIEs_t));
         if(!actionToBeRmvList->list.array[arrIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed at line %d", __func__, __LINE__);
            return RFAILED;
         }
         actionToBeRmv = (RICaction_RequiredToBeRemoved_ItemIEs_t *)actionToBeRmvList->list.array[arrIdx];

         actionToBeRmv->id = ProtocolIE_IDE2_id_RICaction_RequiredToBeRemoved_Item;
         actionToBeRmv->criticality = CriticalityE2_reject;
         actionToBeRmv->value.present = \
            RICaction_RequiredToBeRemoved_ItemIEs__value_PR_RICaction_RequiredToBeRemoved_Item;
         actionToBeRmv->value.choice.RICaction_RequiredToBeRemoved_Item.ricActionID = \
            ricSubscription->actionSequence[actionIdx].actionId;
         fillE2Cause(&actionToBeRmv->value.choice.RICaction_RequiredToBeRemoved_Item.cause, \
            ricSubscription->actionSequence[actionIdx].failureCause);

         arrIdx++;
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RIC Subscription Modification Required IEs
 *
 * @details
 *
 *    Function : FillRicSubsModRequired
 *
 * Functionality: Fill RIC Subscription Modification Required IEs
 *
 * @param  RIC Subscription Modification Required IEs to be filled
 *         RIC Subscription DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t FillRicSubsModRequired(RICsubscriptionModificationRequired_t *ricSubsModReqd, RicSubscription *ricSubscription)
{
   uint8_t ieIdx = 0, elementCnt=0, actionIdx = 0;
   uint8_t numActionsMod = 0, numActionsRmv = 0;
   RICsubscriptionModificationRequired_IEs_t *ricSubsModReqdIe = NULLP;
   RICactions_RequiredToBeModified_List_t *actionToBeModList = NULLP;
   RICactions_RequiredToBeRemoved_List_t  *actionToBeRmvList = NULLP;

   /* Count number of Actions to be modified or deleted */
   for(actionIdx = 0; actionIdx < MAX_RIC_ACTION; actionIdx++)
   {
      if(ricSubscription->actionSequence[actionIdx].action == CONFIG_MOD)
         numActionsMod++;
      else if(ricSubscription->actionSequence[actionIdx].action == CONFIG_DEL)
         numActionsRmv++;
   }

   /* Count number of IEs to be added to messages */
   elementCnt = 2;
   if(numActionsMod)
      elementCnt++;
   if(numActionsRmv)
      elementCnt++;

   ricSubsModReqd->protocolIEs.list.count = elementCnt;
   ricSubsModReqd->protocolIEs.list.size = elementCnt * sizeof(RICsubscriptionModificationRequired_IEs_t *);
   DU_ALLOC(ricSubsModReqd->protocolIEs.list.array, ricSubsModReqd->protocolIEs.list.size);
   if(!ricSubsModReqd->protocolIEs.list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed at line %d", __func__, __LINE__);
      return RFAILED;
   }

   for(ieIdx = 0; ieIdx < elementCnt; ieIdx++)
   {
      DU_ALLOC(ricSubsModReqd->protocolIEs.list.array[ieIdx], sizeof(RICsubscriptionModificationRequired_IEs_t));
      if(!ricSubsModReqd->protocolIEs.list.array[ieIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed at line %d", __func__, __LINE__);
         return RFAILED;
      }
   }

   /* RIC Request ID */
   ieIdx = 0;
   ricSubsModReqdIe = ricSubsModReqd->protocolIEs.list.array[ieIdx];
   ricSubsModReqdIe->id = ProtocolIE_IDE2_id_RICrequestID;
   ricSubsModReqdIe->criticality = CriticalityE2_reject;
   ricSubsModReqdIe->value.present = RICsubscriptionModificationRequired_IEs__value_PR_RICrequestID;
   ricSubsModReqdIe->value.choice.RICrequestID.ricRequestorID = ricSubscription->requestId.requestorId;
   ricSubsModReqdIe->value.choice.RICrequestID.ricInstanceID = ricSubscription->requestId.instanceId;

   /* RAN Function ID */
   ieIdx++;
   ricSubsModReqdIe = ricSubsModReqd->protocolIEs.list.array[ieIdx];
   ricSubsModReqdIe->id = ProtocolIE_IDE2_id_RANfunctionID;
   ricSubsModReqdIe->criticality = CriticalityE2_reject;
   ricSubsModReqdIe->value.present = RICsubscriptionModificationRequired_IEs__value_PR_RANfunctionID;
   ricSubsModReqdIe->value.choice.RANfunctionID = ricSubscription->ranFuncId;

/* A RIC Subscription includes RIC subsequent action only for RIC Insert service.
 * However, E2SM-KPM supports only RIC Report service.
 * Hence there is no subsequent action in RIC subscription that may require modification.
 * So commenting the action-modification IEs for the time being
 */
#if 0
   /* RIC Actions Required to be Modified */
   if(numActionsMod)
   {
      ieIdx++;
      ricSubsModReqdIe = ricSubsModReqd->protocolIEs.list.array[ieIdx];
      ricSubsModReqdIe->id = ProtocolIE_IDE2_id_RICactionsRequiredToBeModified_List;
      ricSubsModReqdIe->criticality = CriticalityE2_reject;
      ricSubsModReqdIe->value.present = \
         RICsubscriptionModificationRequired_IEs__value_PR_RICactions_RequiredToBeModified_List;
      actionToBeModList = &ricSubsModReqdIe->value.choice.RICactions_RequiredToBeModified_List;

      if(FillActionReqdToBeModList(actionToBeModList, numActionsMod, ricSubscription) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Failed to fill actions required to be modified list", __func__);
         return RFAILED;
      }
   }
#endif

   /* RIC Actions Required to be removed */
   if(numActionsRmv)
   {
      ieIdx++;
      ricSubsModReqdIe = ricSubsModReqd->protocolIEs.list.array[ieIdx];
      ricSubsModReqdIe->id = ProtocolIE_IDE2_id_RICactionsRequiredToBeRemoved_List;
      ricSubsModReqdIe->criticality = CriticalityE2_reject;
      ricSubsModReqdIe->value.present = \
         RICsubscriptionModificationRequired_IEs__value_PR_RICactions_RequiredToBeRemoved_List;
      actionToBeRmvList = &ricSubsModReqdIe->value.choice.RICactions_RequiredToBeRemoved_List;

      if(FillActionReqdToBeRmvList(actionToBeRmvList, numActionsRmv, ricSubscription) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Failed to fill actions required to be removed list", __func__);
         return RFAILED;
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and Send RIC Subscription Modification Required
 *        message
 *
 * @details
 *
 *    Function : BuildAndSendRicSubsModRequired
 *
 * Functionality:  Builds and Send RIC Subscription Modification 
 *    Required message
 *
 * @param  RIC Subscription DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t BuildAndSendRicSubsModRequired(RicSubscription *ricSubscription)
{
   uint8_t ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   RICsubscriptionModificationRequired_t  *ricSubsModReqd = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Modification Required \n");
   while(true)
   {
      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed at line %d", __func__, __LINE__);
         break;
      }
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICsubscriptionModificationRequired;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICsubscriptionModificationRequired;

      ricSubsModReqd = &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionModificationRequired;

      if(FillRicSubsModRequired(ricSubsModReqd, ricSubscription) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Failed to fill RIC Subscription Modification Required IEs", __func__);
         break;
      }
      
      /* Encode */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC Subscription Modifiction Required structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for RIC Subscription Modification Required \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }
      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending RIC Subscription Modification Required failed");
      }

      ret = ROK;
      break;
   }

   /* Free RIC Subscription modification required */
   FreeRicSubsModRequired(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Free APER decoding of RIC Subscription Modification Confirm
 *
 * @details
 *
 *    Function : freeAperDecodingOfRicSubsModConfirm
 *
 * Functionality:  Free APER decoding of RIC Subscription 
 *   Modification Confirm
 *
 * @param  E2AP Message PDU
 * @return void
 *
 ******************************************************************/
void freeAperDecodingOfRicSubsModConfirm(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx = 0, arrIdx=0;
   RICsubscriptionModificationConfirm_t *ricSubsModCfm = NULLP;
   RICsubscriptionModificationConfirm_IEs_t *ricSubsModCfmIe = NULLP;
   RICactions_ConfirmedForModification_List_t *modCfmList = NULLP;
   RICactions_RefusedToBeModified_List_t *modRefusedList = NULLP;
   RICactions_ConfirmedForRemoval_List_t *rmvCfmList = NULLP;
   RICactions_RefusedToBeRemoved_List_t *rmvFailList = NULLP;

   if(e2apMsg && e2apMsg->choice.successfulOutcome)
   {
      ricSubsModCfm = &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionModificationConfirm;
      if(ricSubsModCfm->protocolIEs.list.array)
      {
         for(ieIdx = 0; ieIdx < ricSubsModCfm->protocolIEs.list.count; ieIdx++)
         {
            if(ricSubsModCfm->protocolIEs.list.array[ieIdx])
            {
               ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
               switch(ricSubsModCfmIe->id)
               {
                  case ProtocolIE_IDE2_id_RICactionsConfirmedForModification_List:
                     {
                        modCfmList = &ricSubsModCfmIe->value.choice.RICactions_ConfirmedForModification_List;
                        if(modCfmList->list.array)
                        {
                           for(arrIdx = 0; arrIdx < modCfmList->list.count; arrIdx++)
                           {
                              if(modCfmList->list.array[arrIdx])
                                 free(modCfmList->list.array[arrIdx]);
                           }
                           free(modCfmList->list.array);
                        }
                        break;
                     }

                  case ProtocolIE_IDE2_id_RICactionsRefusedToBeModified_List:
                     {
                        modRefusedList = &ricSubsModCfmIe->value.choice.RICactions_RefusedToBeModified_List;
                        if(modRefusedList->list.array)
                        {
                           for(arrIdx = 0; arrIdx < modRefusedList->list.count; arrIdx++)
                           {
                              if(modRefusedList->list.array[arrIdx])
                                 free(modRefusedList->list.array[arrIdx]);
                           }
                           free(modRefusedList->list.array);
                        }
                        break;
                     }

                  case ProtocolIE_IDE2_id_RICactionsConfirmedForRemoval_List:
                     {
                        rmvCfmList = &ricSubsModCfmIe->value.choice.RICactions_ConfirmedForRemoval_List;
                        if(rmvCfmList->list.array)
                        {
                           for(arrIdx = 0; arrIdx < rmvCfmList->list.count; arrIdx++)
                           {
                              if(rmvCfmList->list.array[arrIdx])
                                 free(rmvCfmList->list.array[arrIdx]);
                           }
                           free(rmvCfmList->list.array);
                        }
                        break;
                     }

                  case ProtocolIE_IDE2_id_RICactionsRefusedToBeRemoved_List:
                     {
                        rmvFailList = &ricSubsModCfmIe->value.choice.RICactions_RefusedToBeRemoved_List;
                        if(rmvFailList->list.array)
                        {
                           for(arrIdx = 0; arrIdx < rmvFailList->list.count; arrIdx++)
                           {
                              if(rmvFailList->list.array[arrIdx])
                                 free(rmvFailList->list.array[arrIdx]);
                           }
                           free(rmvFailList->list.array);
                        }
                        break;
                     }

                  default:
                     break;

               }
               free(ricSubsModCfmIe);
            }
         }
         free(ricSubsModCfm->protocolIEs.list.array);
      }
   }
}

/*******************************************************************
 *
 * @brief Process RIC Subscription Modification Confirm Message
 *
 * @details
 *
 *    Function : procRicSubscriptionModificationConfirm
 *
 * Functionality:  Process RIC Subscription Modification Confirm
 *    Message received from RIC. 
 *
 * @param  E2AP Message PDU
 * @return void
 *
 ******************************************************************/
void procRicSubscriptionModificationConfirm(E2AP_PDU_t *e2apMsg)
{
   uint8_t actionId = 0, ieIdx = 0, arrIdx = 0;
   uint16_t ranFuncId = 0;
   bool procFailure = false;
   RicRequestId ricReqId;
   RanFunction *ranFuncDb = NULLP;
   CmLList *ricSubsNode = NULLP;
   RicSubscription *ricSubsDb = NULLP;
   ActionInfo *actionDb = NULLP;

   RICsubscriptionModificationConfirm_t *ricSubsModCfm = NULLP;
   RICsubscriptionModificationConfirm_IEs_t *ricSubsModCfmIe = NULLP;

   RICactions_ConfirmedForModification_List_t *modCfmList = NULLP;
   RICaction_ConfirmedForModification_ItemIEs_t *modCfmListItem = NULLP;

   RICactions_RefusedToBeModified_List_t *modRefusedList = NULLP;
   RICaction_RefusedToBeModified_ItemIEs_t *modRefusedListItem = NULLP;

   RICactions_ConfirmedForRemoval_List_t *rmvCfmList = NULLP;
   RICaction_ConfirmedForRemoval_ItemIEs_t *rmvCfmListItem = NULLP;

   RICactions_RefusedToBeRemoved_List_t *rmvFailList = NULLP;
   RICaction_RefusedToBeRemoved_ItemIEs_t *rmvFailListItem = NULLP;

   DU_LOG("\nINFO   -->  E2AP : %s: Received RIC Subscription Modification Confirm", __func__);

   do{
      if(!e2apMsg)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: E2AP Message is NULL", __func__);
         break;
      }

      if(!e2apMsg->choice.successfulOutcome)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Successful Outcome in E2AP message is NULL", __func__);
         break;
      }

      ricSubsModCfm = &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionModificationConfirm;
      if(!ricSubsModCfm->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Array conatining E2AP message IEs is null", __func__);
         break;
      }

      for(ieIdx = 0; ieIdx < ricSubsModCfm->protocolIEs.list.count; ieIdx++)
      {
         if(!ricSubsModCfm->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : %s: IE at index [%d] in E2AP message IEs list is null", __func__, ieIdx);
            break;
         }

         ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
         switch(ricSubsModCfmIe->id)
         {
            case ProtocolIE_IDE2_id_RICrequestID:
               {
                  memset(&ricReqId, 0, sizeof(RicRequestId));
                  ricReqId.requestorId = ricSubsModCfmIe->value.choice.RICrequestID.ricRequestorID;
                  ricReqId.instanceId = ricSubsModCfmIe->value.choice.RICrequestID.ricInstanceID;
                  break;
               }

            case ProtocolIE_IDE2_id_RANfunctionID:
               {
                  ranFuncId = ricSubsModCfmIe->value.choice.RANfunctionID;
                  ranFuncDb = fetchRanFuncFromRanFuncId(ranFuncId);
                  if(!ranFuncDb)
                  {
                     DU_LOG("\nERROR  -->  E2AP : %s: RAN Function ID [%d] not found", __func__, ranFuncId);
                     procFailure = true;
                     break;
                  }

                  ricSubsDb = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb, &ricSubsNode); 
                  if(!ricSubsDb)
                  {
                     DU_LOG("\nERROR  -->  E2AP : %s: RIC Subscription not found for Requestor_ID [%d] Instance_ID [%d]",\
                           __func__, ricReqId.requestorId, ricReqId.instanceId);
                     procFailure = true;
                     break;
                  }

                  break;
               }

/* A RIC Subscription includes RIC subsequent action only for RIC Insert service. 
 * However, E2SM-KPM supports only RIC Report service. 
 * Hence there is no subsequent action in RIC subscription that may require modification. 
 * So commenting the action-modification IEs for the time being 
 */
#if 0
            case ProtocolIE_IDE2_id_RICactionsConfirmedForModification_List:
               {
                  modCfmList = &ricSubsModCfmIe->value.choice.RICactions_ConfirmedForModification_List;
                  for(arrIdx = 0; arrIdx < modCfmList->list.count; arrIdx++)
                  {
                     modCfmListItem = (RICaction_ConfirmedForModification_ItemIEs_t *)modCfmList->list.array[arrIdx];
                     actionId = modCfmListItem->value.choice.RICaction_ConfirmedForModification_Item.ricActionID;

                     actionDb = fetchActionInfoFromActionId(actionId, ricSubsDb);
                     if(!actionDb)
                     {
                        DU_LOG("\nERROR  -->  E2AP : %s: Action ID [%d] not found", __func__, actionId);
                     }
                     else
                     {
                        actionDb->action = CONFIG_UNKNOWN;
                        /* Further handling can be added here in future once the
                         * use case of this procedure is identified */
                     }
                     actionDb = NULLP;
                  }
                  break;
               }

            case ProtocolIE_IDE2_id_RICactionsRefusedToBeModified_List:
               {
                  modRefusedList = &ricSubsModCfmIe->value.choice.RICactions_RefusedToBeModified_List;
                  for(arrIdx = 0; arrIdx < modRefusedList->list.count; arrIdx++)
                  {
                    modRefusedListItem = (RICaction_RefusedToBeModified_ItemIEs_t *)modRefusedList->list.array[arrIdx];
                    actionId = modRefusedListItem->value.choice.RICaction_RefusedToBeModified_Item.ricActionID;
                    actionDb = fetchActionInfoFromActionId(actionId, ricSubsDb);
                    if(!actionDb)
                    {
                       DU_LOG("\nERROR  -->  E2AP : %s: Action ID [%d] not found", __func__, actionId);
                    }
                    else
                    {
                       /* Spec doesnt mention if in case of failure, DU should retry for modify action 
                        * Hence, chaging the action from CONFIG_MOD to CONFIG_UNKNOWN
                        */
                        actionDb->action = CONFIG_UNKNOWN;
                    }
                    actionDb = NULLP;
                  }
                  break;
               }
#endif

            case ProtocolIE_IDE2_id_RICactionsConfirmedForRemoval_List:
               {
                  rmvCfmList = &ricSubsModCfmIe->value.choice.RICactions_ConfirmedForRemoval_List;
                  for(arrIdx = 0; arrIdx < rmvCfmList->list.count; arrIdx++)
                  {
                     rmvCfmListItem = (RICaction_ConfirmedForRemoval_ItemIEs_t *)rmvCfmList->list.array[arrIdx];
                     actionId = rmvCfmListItem->value.choice.RICaction_ConfirmedForRemoval_Item.ricActionID;
                     actionDb = fetchActionInfoFromActionId(actionId, ricSubsDb);
                     if(!actionDb)
                     {
                        DU_LOG("\nERROR  -->  E2AP : %s: Action ID [%d] not found", __func__, actionId);
                     }
                     else
                     {
                        memset(actionDb, 0, sizeof(ActionInfo));
                        actionDb->actionId = -1;
                        ricSubsDb->numOfActions--;
                        /* Further handling can include :
                         * Deletion of this action from all DU layers 
                         */
                     }
                     actionDb = NULLP;
                  }
                  break;
               }

            case ProtocolIE_IDE2_id_RICactionsRefusedToBeRemoved_List:
               {
                  rmvFailList = &ricSubsModCfmIe->value.choice.RICactions_RefusedToBeRemoved_List;
                  for(arrIdx = 0; arrIdx < rmvFailList->list.count; arrIdx++)
                  {
                     rmvFailListItem = (RICaction_RefusedToBeRemoved_ItemIEs_t *)rmvFailList->list.array[arrIdx];
                     actionId = rmvFailListItem->value.choice.RICaction_RefusedToBeRemoved_Item.ricActionID;
                     actionDb = fetchActionInfoFromActionId(actionId, ricSubsDb);
                     if(!actionDb)
                     {
                        DU_LOG("\nERROR  -->  E2AP : %s: Action ID [%d] not found", __func__, actionId);
                     }
                     else
                     {
                        actionDb->action = CONFIG_UNKNOWN;
                     }
                     actionDb = NULLP;
                  }
                  break;
               }

            default:
               break;
         } /* End of switch for Protocol IE Id */

         if(procFailure)
            break;
      } /* End of for loop for Protocol IE list */

      break;
   }while(true);

   freeAperDecodingOfRicSubsModConfirm(e2apMsg);
   return;
}

/******************************************************************
* @brief Deallocate the memory allocated for E2 Reset Response
*
* @details
*
*    Function : FreeE2ResetResponse
*
*    Functionality:
*       - freeing the memory allocated for E2ResetResponse
*
* @params[in] E2AP_PDU_t *e2apMsg
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void FreeE2ResetResponse(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   ResetResponseE2_t *resetResponse;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.successfulOutcome != NULLP)
      {
         resetResponse = &e2apMsg->choice.successfulOutcome->value.choice.ResetResponseE2;
         if(resetResponse->protocolIEs.list.array)
         {
            for(ieIdx=0; ieIdx < resetResponse->protocolIEs.list.count; ieIdx++)
            {
               if(resetResponse->protocolIEs.list.array[ieIdx])
               {
                  DU_FREE(resetResponse->protocolIEs.list.array[ieIdx], sizeof(ResetResponseIEs_t));
               }
            }
            DU_FREE(resetResponse->protocolIEs.list.array, resetResponse->protocolIEs.list.size);
         }

         DU_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 Reset Response msg
 *
 * @details
 *
 *    Function : BuildAndSendE2ResetResponse
 *
 *    Functionality:
 *         - Buld and send the E2 Reset Response Message
 *
 * @params[in] Trans Id
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendResetResponse(uint8_t transId)
{
   uint8_t           ieIdx = 0, elementCnt = 0;
   uint8_t           ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   ResetResponseE2_t *resetResponse;
   asn_enc_rval_t    encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Reset Response Message\n");
   do
   {
      DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse(): Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_successfulOutcome;

      DU_ALLOC(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse: Memory allocation failed for successfulOutcome");
         break;
      }

      e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_Reset;
      e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.successfulOutcome->value.present = SuccessfulOutcomeE2__value_PR_ResetResponseE2;
      resetResponse = &e2apMsg->choice.successfulOutcome->value.choice.ResetResponseE2;

      elementCnt = 1;
      resetResponse->protocolIEs.list.count = elementCnt;
      resetResponse->protocolIEs.list.size = elementCnt * sizeof(ResetResponseIEs_t *);
      DU_ALLOC(resetResponse->protocolIEs.list.array, resetResponse->protocolIEs.list.size);
      if(!resetResponse->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse: Memory allocation failed for protocol IE array");
         break;
      }

      for(ieIdx=0; ieIdx < elementCnt; ieIdx++)
      {
         DU_ALLOC(resetResponse->protocolIEs.list.array[ieIdx], sizeof(ResetResponseIEs_t));
         if(!resetResponse->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse: Memory allocation failed for protocol IE array element");
            break;
         }
      }
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0;
      resetResponse->protocolIEs.list.array[ieIdx]->id =  ProtocolIE_IDE2_id_TransactionID;
      resetResponse->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      resetResponse->protocolIEs.list.array[ieIdx]->value.present = ResetResponseIEs__value_PR_TransactionID;
      resetResponse->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = transId;

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 reset response structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Reset Response \n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      /* Sending msg */
      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL, encBuf, encBufSize) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send E2 Reset Response");
         break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeE2ResetResponse(e2apMsg);
   return ret;
}

/******************************************************************
 *
 * @brief Deallocation of memory allocated by aper decoder for reset req
 *
 * @details
 *
 *    Function : freeAperDecodingOfE2ResetReq
 *
 *    Functionality: Deallocation of memory allocated by aper decoder for
 *    reset req
 *
 * @params[in] Pointer to resetReq
 * @return void
 *
 * ****************************************************************/
void freeAperDecodingOfE2ResetReq(ResetRequestE2_t *resetReq)
{
   uint8_t arrIdx=0;

   if(resetReq)
   {
      if(resetReq->protocolIEs.list.array)
      {
         for(arrIdx=0; arrIdx<resetReq->protocolIEs.list.count; arrIdx++)
         {
            if(resetReq->protocolIEs.list.array[arrIdx])
            {
               free(resetReq->protocolIEs.list.array[arrIdx]);
            }
         }
         free(resetReq->protocolIEs.list.array);
      }
   }
}

/*******************************************************************
 *
 * @brief Process reset req received from RIC
 *
 * @details
 *
 *    Function : procE2ResetRequest
 *
 * Functionality: Process reset req received from RIC
 *
 * @param  E2AP_PDU_t  *e2apMsg
 * @return void
 *
 ******************************************************************/

void procE2ResetRequest(E2AP_PDU_t  *e2apMsg)
{
   uint16_t ranFuncIdx=0;
   uint8_t arrIdx =0, transId =0, pendingSubsRspIdx=0;
   ResetRequestE2_t *resetReq;

   DU_LOG("\nINFO   -->  E2AP : E2 Reset request received");
   resetReq = &e2apMsg->choice.initiatingMessage->value.choice.ResetRequestE2;

   for(arrIdx=0; arrIdx<resetReq->protocolIEs.list.count; arrIdx++)
   {
      switch(resetReq->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            {
               transId = resetReq->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
               break;
            }

         case ProtocolIE_IDE2_id_CauseE2:
            {
               for(ranFuncIdx=0; ranFuncIdx<MAX_RAN_FUNCTION; ranFuncIdx++)
               {
                  if(duCb.e2apDb.ranFunction[ranFuncIdx].id >0)
                  {
                     deleteRicSubscriptionList(&(duCb.e2apDb.ranFunction[ranFuncIdx].subscriptionList));
                     memset(&(duCb.e2apDb.ranFunction[ranFuncIdx].pendingSubsRspInfo), 0, MAX_PENDING_SUBSCRIPTION_RSP*sizeof(PendingSubsRspInfo));
                  }
               }
               break;
            }
      }
   }
   if(BuildAndSendResetResponse(transId) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to build and send reset response");
   }
   freeAperDecodingOfE2ResetReq(resetReq);
}

/*******************************************************************
 *
 * @brief Handles received E2AP message and sends back response  
 *
 * @details
 *
 *    Function : E2APMsgHdlr
 *
 *    Functionality:
 *         - Decodes received E2AP control message
 *         - Prepares response message, encodes and sends to SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void E2APMsgHdlr(Buffer *mBuf)
{
   int i =0;
   char *recvBuf = NULLP;
   MsgLen copyCnt =0;
   MsgLen recvBufLen =0;
   E2AP_PDU_t *e2apMsg = NULLP;
   asn_dec_rval_t rval ={0}; /* Decoder return value */
   E2AP_PDU_t e2apasnmsg={0} ;

   DU_LOG("\nDEBUG   -->  E2AP : Received E2AP message buffer");
   ODU_PRINT_MSG(mBuf, 0,0);

   /* Copy mBuf into char array to decode it */
   ODU_GET_MSG_LEN(mBuf, &recvBufLen);
   DU_ALLOC(recvBuf, (Size)recvBufLen);

   if(recvBuf == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed");
      return;
   }
   if(ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed while copying %d", copyCnt);
      return;
   }

#ifdef DEBUG_ASN_PRINT
   printf("\nDEBUG   -->  E2AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
      printf("%x",recvBuf[i]);
   }
#endif

   /* Decoding flat buffer into E2AP messsage */
   e2apMsg = &e2apasnmsg;
   memset(e2apMsg, 0, sizeof(E2AP_PDU_t));

   rval = aper_decode(0, &asn_DEF_E2AP_PDU, (void **)&e2apMsg, recvBuf, recvBufLen, 0, 0);
   DU_FREE(recvBuf, (Size)recvBufLen);

   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  E2AP : ASN decode failed");
      return;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

   switch(e2apMsg->present)
   {
      case E2AP_PDU_PR_unsuccessfulOutcome:
         {
            switch(e2apMsg->choice.unsuccessfulOutcome->value.present)
            {
               case UnsuccessfulOutcomeE2__value_PR_E2setupFailure:
                  {
                     procE2SetupFailure(e2apMsg);
                     break;
                  }
               case UnsuccessfulOutcomeE2__value_PR_E2nodeConfigurationUpdateFailure:
                  {
                     procE2NodeConfigUpdateFailure(e2apMsg);
                     break;
                  }
               case UnsuccessfulOutcomeE2__value_PR_RICserviceUpdateFailure:
                  {
                     procRicServiceUpdateFailure(e2apMsg);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid type of E2AP_PDU_PR_unsuccessfulOutcome  [%d]",\
                           e2apMsg->choice.unsuccessfulOutcome->value.present);
                     return;
                  }
            }
            free(e2apMsg->choice.unsuccessfulOutcome);
            break;
         }
      case E2AP_PDU_PR_successfulOutcome:
         {
            switch(e2apMsg->choice.successfulOutcome->value.present)
            {
               case SuccessfulOutcomeE2__value_PR_E2setupResponse:
                  {
                     if(!duCb.e2Status)
                     {
                        procE2SetupRsp(e2apMsg);
                     }
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_E2nodeConfigurationUpdateAcknowledge:
                  {
                     DU_LOG("\nDEBUG   -->  E2AP : E2 node Config update ack message recevied");
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_ResetResponseE2:
                  {
                     procResetResponse(e2apMsg);
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_RICserviceUpdateAcknowledge:
                  {
                     procRicServiceUpdateAck(e2apMsg);
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_RICsubscriptionModificationConfirm:
                  {
                     procRicSubscriptionModificationConfirm(e2apMsg);
                     break;
                  }

               default:
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid type of E2AP_PDU_PR_successfulOutcome  [%d]",\
                           e2apMsg->choice.successfulOutcome->value.present);
                     return;
                  }
            }/* End of switch(successfulOutcome) */
            free(e2apMsg->choice.successfulOutcome);
            break;
         }

      case E2AP_PDU_PR_initiatingMessage:
         {
            switch(e2apMsg->choice.initiatingMessage->value.present)
            {
               case InitiatingMessageE2__value_PR_RICsubscriptionRequest: 
                  {
                     procRicSubscriptionRequest(e2apMsg);
                     break;
                  }
               case InitiatingMessageE2__value_PR_RICserviceQuery:
                  {
                     procRicServiceQuery(e2apMsg);
                     break;
                  }
               case InitiatingMessageE2__value_PR_ErrorIndicationE2:
                  {
                     DU_LOG("\nINFO  -->  E2AP : Error indication received");
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid type of E2AP_PDU_PR_initiatingMessage [%d]",\
                           e2apMsg->choice.initiatingMessage->value.present);
                     return;
                  }
            }/* End of switch(initiatingMessage) */
            free(e2apMsg->choice.initiatingMessage);
            break;
         }
      default:
	 {
	    DU_LOG("\nERROR  -->  E2AP : Invalid type of e2apMsg->present [%d]",e2apMsg->present);
	    return;
	 }
	 free(e2apMsg);

   }/* End of switch(e2apMsg->present) */

} /* End of E2APMsgHdlr */

/**********************************************************************
  End of file
 **********************************************************************/
