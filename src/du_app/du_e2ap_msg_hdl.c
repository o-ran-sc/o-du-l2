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
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "legtp.h"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_mgr_main.h"
#include "du_utils.h"
#include "GlobalE2node-gNB-ID.h"
#include<ProtocolIE-FieldE2.h>
#include "E2setupRequest.h"
#include "InitiatingMessageE2.h"
#include "SuccessfulOutcomeE2.h"
#include "E2AP-PDU.h"
#include "du_e2ap_msg_hdl.h"
#include "odu_common_codec.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "RIC-EventTriggerStyle-List.h"
#include "RIC-ReportStyle-List.h"
/* Global variable */
DuCfgParams duCfgParam;
uint8_t fillRanFunctionsItemIe(RANfunction_ItemIEs_t *RanFunctionsItemIe);
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
   uint8_t val = 1;
   uint8_t ret = ROK;

   /* Allocate Buffer size */
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
      /* fill gND Id */
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
	 fillBitString(&gNbId->global_gNB_ID.gnb_id.choice.gnb_ID, unused, byteSize, val);
      }
   }
   return ret;
}
/*******************************************************************
 *
 * @brief fill Ric Report Style List
 *
 * @details
 *
 *    Function : fillRicReportStyleList 
 *
 *    Functionality: fill Ric Report Style List 
 *
 * @params[in] RIC_ReportStyle_List_t *RicReportStyleList
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
                                
uint8_t fillRicReportStyleList(RIC_ReportStyle_List_t *RicReportStyleList)
{
   RicReportStyleList->ric_ReportStyle_Type = 1;
   RicReportStyleList->ric_ReportStyle_Name.size = 3 * sizeof(uint8_t);
   DU_ALLOC(RicReportStyleList->ric_ReportStyle_Name.buf, \
   RicReportStyleList->ric_ReportStyle_Name.size)
   if(RicReportStyleList->ric_ReportStyle_Name.buf == NULLP)
   {
      return RFAILED;
   }
   else
   {
      buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
      RicReportStyleList->ric_ReportStyle_Name.buf);
      RicReportStyleList->ric_IndicationHeaderFormat_Type = 1;
      RicReportStyleList->ric_IndicationMessageFormat_Type = 1;
   }
   return ROK;
}


/*******************************************************************
*
* @brief fill Ric Event Trigger StyleList 
*
* @details 
*
*    Function : fillRicEventTriggerStyleList 
*
*    Functionality: fill Ric Event Trigger StyleList
*
* @params[in] RIC_EventTriggerStyle_List_t
* @return ROK     - success
*         RFAILED - failure
*
******************************************************************/
uint8_t fillRicEventTriggerStyleList(RIC_EventTriggerStyle_List_t *RicEventTriggerStyleList)
{
    RicEventTriggerStyleList->ric_EventTriggerStyle_Type = 1;
    RicEventTriggerStyleList->ric_EventTriggerStyle_Name.size = 3 * sizeof(uint8_t);
    DU_ALLOC(RicEventTriggerStyleList->ric_EventTriggerStyle_Name.buf,\
			RicEventTriggerStyleList->ric_EventTriggerStyle_Name.size);
    if(RicEventTriggerStyleList->ric_EventTriggerStyle_Name.buf == NULLP)
    {
	return RFAILED;
    }
    else
    {
       buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
       RicEventTriggerStyleList->ric_EventTriggerStyle_Name.buf);
       RicEventTriggerStyleList->ric_EventTriggerFormat_Type = 1;
    }
    return ROK;

}

/*******************************************************************
*
* @brief Fill Ric Report Style List
*
* @details
*
*    Function : FillRicReportStyleList
*
*    Functionality: Fill Ric Report Style List
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
******************************************************************/

uint8_t FillRicReportStyleList(struct E2SM_KPM_RANfunction_Description__e2SM_KPM_RANfunction_Item__ric_ReportStyle_List\
*RICReportStyleList)
{
   uint8_t idx;
   RICReportStyleList->list.count = 1;
   RICReportStyleList->list.size =  sizeof(struct RIC_ReportStyle_List *);
   DU_ALLOC(RICReportStyleList->list.array ,RICReportStyleList->list.size);
   if(RICReportStyleList->list.array == NULLP)
   {
      return RFAILED;
   }
   else
   {
      for( idx =0 ;idx< RICReportStyleList->list.count ;idx++)
      {
         DU_ALLOC(RICReportStyleList->list.array[idx],sizeof(struct RIC_ReportStyle_List ));
         if(RICReportStyleList->list.array[idx] == NULLP)
         {
            return RFAILED;
         }
         else
         {
            fillRicReportStyleList(RICReportStyleList->list.array[idx]);
         }
      }
   }
   return ROK;
}
/*******************************************************************
*
* @brief fill RAN functions List
*
* @details
*
*    Function : fillRANfunctions_List
*
*    Functionality: fill RAN functions List
*
* @params[in] RANfunctions_List_t *RanFunctionsList 
* @return ROK     - success
*         RFAILED - failure
*
******************************************************************/

uint8_t fillRANfunctions_List(RANfunctions_List_t *RanFunctionsList)
{
   uint8_t arrIdx =0;
   uint8_t ranFunCnt =0;
   RANfunction_ItemIEs_t *RanFunctionsItemIe;
 
   ranFunCnt = 1;
   RanFunctionsList->list.count = ranFunCnt;
   RanFunctionsList->list.size = ranFunCnt * sizeof(RANfunction_ItemIEs_t *);
   DU_ALLOC(RanFunctionsList->list.array, RanFunctionsList->list.size );
   if(RanFunctionsList->list.array ==  NULLP)
   { 
       return RFAILED;  
     
   }
   else
   {
      for(arrIdx=0; arrIdx<ranFunCnt; arrIdx++)
      {
         DU_ALLOC(RanFunctionsList->list.array[arrIdx], sizeof(RANfunction_ItemIEs_t));
         if(RanFunctionsList->list.array[arrIdx] == NULLP)
            return RFAILED;
         else
         {
            RanFunctionsItemIe =(RANfunction_ItemIEs_t*) RanFunctionsList->list.array[arrIdx];
            RanFunctionsItemIe->id = ProtocolIE_IDE2_id_RANfunction_Item;
            RanFunctionsItemIe->criticality = CriticalityE2_ignore;
            RanFunctionsItemIe->value.present = RANfunctionID_ItemIEs__value_PR_RANfunctionID_Item;
            if(fillRanFunctionsItemIe(RanFunctionsItemIe) != ROK)
              return RFAILED;
            else
              return ROK;
         }
      }
   }
   return ROK;
}
/*******************************************************************
*
* @brief fill RAN functions Item IE
* @details
*
*    Function : fillRanFunctionsItemIe
*
*    Functionality: fill RAN functions Item IE
*
* @params[in] RANfunction_ItemIEs_t * 
* @return ROK     - success
*         RFAILED - failure
*
******************************************************************/
uint8_t fillRanFunctionsItemIe(RANfunction_ItemIEs_t *RanFunctionsItemIe)      
{
    uint8_t idx;
    E2SM_KPM_RANfunction_Description_t RanFunctionDescription;
    asn_enc_rval_t   encRetVal;
	
    RanFunctionsItemIe->value.choice.RANfunction_Item.ranFunctionID =1 ;

    while(true)
    {
	RanFunctionDescription.ranFunction_Name.ranFunction_ShortName.size = sizeof(uint8_t);
	DU_ALLOC(RanFunctionDescription.ranFunction_Name.ranFunction_ShortName.buf,\
	RanFunctionDescription.ranFunction_Name.ranFunction_ShortName.size);  
	if(RanFunctionDescription.ranFunction_Name.ranFunction_ShortName.buf == NULLP)
	{
	   return RFAILED;
	}
	else
	{
	   RanFunctionDescription.ranFunction_Name.ranFunction_E2SM_OID.size = sizeof(uint8_t);
	   DU_ALLOC(RanFunctionDescription.ranFunction_Name.ranFunction_E2SM_OID.buf,\
	   RanFunctionDescription.ranFunction_Name.ranFunction_E2SM_OID.size); 
	   if(RanFunctionDescription.ranFunction_Name.ranFunction_E2SM_OID.buf == NULLP)
	   {
		return RFAILED;
	   }
     	   else
           {
	       RanFunctionDescription.ranFunction_Name.ranFunction_Description.size = sizeof(uint8_t);
	       DU_ALLOC(RanFunctionDescription.ranFunction_Name.ranFunction_Description.buf,\
	       RanFunctionDescription.ranFunction_Name.ranFunction_Description.size);
	       if(RanFunctionDescription.ranFunction_Name.ranFunction_Description.buf == NULLP)
	        {
		   return RFAILED;
		}
		else
		{
		   DU_ALLOC(RanFunctionDescription.ranFunction_Name.ranFunction_Instance , sizeof(uint16_t));
		   if(RanFunctionDescription.ranFunction_Name.ranFunction_Instance == NULLP) 
     	 	   {
		       return RFAILED;
		   }
      		   else
   		   {
  	               RanFunctionDescription.e2SM_KPM_RANfunction_Item.ric_EventTriggerStyle_List->list.count = 1;
  		       RanFunctionDescription.e2SM_KPM_RANfunction_Item.ric_EventTriggerStyle_List->list.size = 1*\
		       sizeof(struct RIC_EventTriggerStyle_List *);
		       DU_ALLOC(RanFunctionDescription.e2SM_KPM_RANfunction_Item.ric_EventTriggerStyle_List->\
                       list.array, RanFunctionDescription.e2SM_KPM_RANfunction_Item.\
                       ric_EventTriggerStyle_List->list.size);      
		       if(RanFunctionDescription.e2SM_KPM_RANfunction_Item.ric_EventTriggerStyle_List->\
                       list.array == NULLP)
		       {
			  return RFAILED;
		       }
		       else
		       {                           
          	          for(idx=0; idx<1; idx++)
	                  {
	                     DU_ALLOC(RanFunctionDescription.e2SM_KPM_RANfunction_Item.\
	                     ric_EventTriggerStyle_List->list.array[idx], sizeof(struct RIC_EventTriggerStyle_List));
			     if(RanFunctionDescription.e2SM_KPM_RANfunction_Item.ric_EventTriggerStyle_List->list.\
			     array[idx])
			     {
		   	        if(fillRicEventTriggerStyleList(RanFunctionDescription.e2SM_KPM_RANfunction_Item.\
	                        ric_EventTriggerStyle_List->list.array[idx]) == ROK)
	                        { 
	                           if(FillRicReportStyleList(RanFunctionDescription.\
	                           e2SM_KPM_RANfunction_Item.ric_ReportStyle_List) == ROK)
	                           {
	                              xer_fprint(stdout, &asn_DEF_E2SM_KPM_RANfunction_Description,\
                                      &RanFunctionDescription);
   				      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
				      encBufSize = 0;
				      encRetVal = aper_encode(&asn_DEF_E2SM_KPM_RANfunction_Description, 0, \
			              &RanFunctionDescription, PrepFinalEncBuf, encBuf);
	                              /* Encode results */
				      if(encRetVal.encoded == ENCODE_FAIL)
				      {
				         DU_LOG( "\nERROR  -->  F1AP : Could not encode RANfunction Container\
					 (at %s)\n",encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
					 return RFAILED;
				       }
				       else
				       {
					   DU_LOG("\nDEBUG  -->  F1AP : Created APER  for RANfunction Container\n");
					   for(int i=0; i< encBufSize; i++)
	                                   {
                            	              DU_LOG("%x",encBuf[i]);
	                                   }
				        
                                           RanFunctionsItemIe->value.choice.RANfunction_Item.\
				           ranFunctionDefinition.size = encBufSize;
                                           DU_ALLOC(RanFunctionsItemIe->value.choice.RANfunction_Item.\
                                           ranFunctionDefinition.buf,\
                                           RanFunctionsItemIe->value.choice.RANfunction_Item.ranFunctionDefinition.size);
                                           if(RanFunctionsItemIe->value.choice.RANfunction_Item.\
                                           ranFunctionDefinition.buf == NULLP)
                                           {
                                              return RFAILED;
                                           }
                                           else
                                           {
                                              memcpy(RanFunctionsItemIe->value.choice.RANfunction_Item.\
                                              ranFunctionDefinition.buf,encBuf, RanFunctionsItemIe->value.\
                                              choice.RANfunction_Item.ranFunctionDefinition.size);
                                              RanFunctionsItemIe->value.choice.RANfunction_Item.ranFunctionRevision = 1;
                                              return ROK;
                                           }
 
                                         }
				      }
                                      else
                                      {
                                          return RFAILED;
                                      }
                                   }
                                   else
                                       return RFAILED;		

                                  }
                             }
                         }

                     }
                 }
             }
         }
     }
     return ROK;

}
/*******************************************************************
 *
 * @brief Fills the initiating IE for E2 Setup Request
 *
 * @details
 *
 *    Function : fillE2SetupReq
 *
 * Functionality:Fills the Initiating message for
 *               E2SetupRequest
 *
 * @params[in] E2setupRequest_t *e2SetupReq,
 *             uint8_t *idx
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillE2SetupReq(E2setupRequest_t **e2SetupReq, uint8_t *idx)
{
   uint8_t elementCnt = 0;
   uint8_t idx2 = 0;
   uint8_t ret = ROK;
   RANfunctions_List_t *RanfunctionsList = NULLP;
   if(*e2SetupReq != NULLP)
   {
      elementCnt = 2;
      (*e2SetupReq)->protocolIEs.list.count = elementCnt;
      (*e2SetupReq)->protocolIEs.list.size = \
					     elementCnt * sizeof(E2setupRequestIEs_t);

      /* Initialize the E2Setup members */
      DU_ALLOC((*e2SetupReq)->protocolIEs.list.array, \
	    (*e2SetupReq)->protocolIEs.list.size);
      if((*e2SetupReq)->protocolIEs.list.array == NULLP)
      {
	 DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for array elements");
	 ret = RFAILED;
      }
      else
      {
	 for(*idx = 0; *idx < elementCnt; (*idx)++)
	 {
	    DU_ALLOC((*e2SetupReq)->protocolIEs.list.array[*idx],\
		  sizeof(E2setupRequestIEs_t));
	    if((*e2SetupReq)->protocolIEs.list.array[*idx] == NULLP)
	    {
	       DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for arrayidx [%d]", *idx);
	       ret = RFAILED;
	    }
          }
	  
	 idx2 =0;

	 /* GlobalE2node_gNB_ID */
	 (*e2SetupReq)->protocolIEs.list.array[idx2]->id = \
							   ProtocolIE_IDE2_id_GlobalE2node_ID;
	 (*e2SetupReq)->protocolIEs.list.array[idx2]->criticality = \
								    CriticalityE2_reject;
	 (*e2SetupReq)->protocolIEs.list.array[idx2]->value.present =\
								     E2setupRequestIEs__value_PR_GlobalE2node_ID;
	 (*e2SetupReq)->protocolIEs.list.array[idx2]->value.choice.\
		 GlobalE2node_ID.present = GlobalE2node_ID_PR_gNB;

	 DU_ALLOC((*e2SetupReq)->protocolIEs.list.array[idx2]->value.choice.\
			 GlobalE2node_ID.choice.gNB, sizeof(GlobalE2node_gNB_ID_t));
	 if((*e2SetupReq)->protocolIEs.list.array[idx2]->value.choice.\
			 GlobalE2node_ID.choice.gNB == NULLP)
	 {
		 DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for gNbId");
		 ret = RFAILED;
	 }
	 else
	 {
		 ret = BuildGlobalgNBId((*e2SetupReq)->protocolIEs.list.array[idx2]->value.\
				 choice.GlobalE2node_ID.choice.gNB);
		 if(ret != ROK)
		 {
			 ret = RFAILED;
		 }
		 idx2++;
		 /* RANfunctions_List */
		 (*e2SetupReq)->protocolIEs.list.array[idx2]->id = \
								   ProtocolIE_IDE2_id_RANfunctionsAdded;
		 (*e2SetupReq)->protocolIEs.list.array[idx2]->criticality = \
									    CriticalityE2_reject;
		 (*e2SetupReq)->protocolIEs.list.array[idx2]->value.present =\
									     E2setupRequestIEs__value_PR_RANfunctions_List;

		 RanfunctionsList = &(*e2SetupReq)->protocolIEs.list.array[idx2]->value.choice.RANfunctions_List;
		 if(fillRANfunctions_List(RanfunctionsList) == ROK)
			 return ROK;
		 else
			 return RFAILED;


	     }

      }
   }
   else
   {
      ret = RFAILED;
      DU_LOG("\nERROR  -->  E2AP : received e2SetupReq is NULL");
   }
   return ret;
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
   uint8_t idx = 0;
   uint8_t ret = ROK;
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
	 DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
	 return RFAILED;
      }
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_E2setup;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_E2setupRequest;
      e2SetupReq = &e2apMsg->choice.initiatingMessage->value.choice.E2setupRequest;

      ret = fillE2SetupReq(&e2SetupReq, &idx);
      if(ret != ROK)
      {
	 DU_LOG("\nERROR  -->  E2AP : fillE2SetupReq() failed");
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
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
      }
      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
      {
	 DU_LOG("\nERROR  -->  E2AP : Sending E2 Setup request failed");
      }

      break;
   }while(true);

   deAllocateE2SetupReqMsg(e2apMsg, e2SetupReq, idx);
   return ret;
}/* End of BuildAndSendE2SetupReq */

/*******************************************************************
 *
 * @brief De Allocate E2 Setup Request Message
 *
 * @details
 *
 *    Function : deAllocateE2SetupReqMsg
 *
 *    Functionality: De-Allocating E2 Setup request Message
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *             E2setupRequest_t *e2SetupReq
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t deAllocateE2SetupReqMsg(E2AP_PDU_t *e2apMsg, \
      E2setupRequest_t *e2SetupReq, uint8_t idx)
{
   uint8_t idx2;
   uint8_t ret = ROK;
   /* De-allocating Memory */
   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
	 if(e2SetupReq->protocolIEs.list.array != NULLP)
	 {
	    for(idx2 = 0; idx2 < idx; idx2++)
	    {
	       if(e2SetupReq->protocolIEs.list.array[idx2] != NULLP)
	       {
		  switch(e2SetupReq->protocolIEs.list.array[idx2]->id)
		  {
		     case ProtocolIE_IDE2_id_GlobalE2node_ID:
			{
			   if(e2SetupReq->protocolIEs.list.array[idx2]->\
				 value.choice.GlobalE2node_ID.choice.gNB != NULLP)
			   {
			      GlobalE2node_gNB_ID_t *gNbId = NULLP;
			      gNbId = e2SetupReq->protocolIEs.list.array[idx2]->\
				      value.choice.GlobalE2node_ID.choice.gNB;
			      if(gNbId->global_gNB_ID.plmn_id.buf != NULLP)
			      {
				 if(gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.buf != NULLP)
				 {
				    DU_FREE(gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.buf,\
					  gNbId->global_gNB_ID.gnb_id.choice.gnb_ID.size);
				 }
				 DU_FREE(gNbId->global_gNB_ID.plmn_id.buf,\
				       gNbId->global_gNB_ID.plmn_id.size);
			      }
			      DU_FREE(e2SetupReq->protocolIEs.list.array[idx2]->value.\
				    choice.GlobalE2node_ID.choice.gNB, sizeof(GlobalE2node_gNB_ID_t));
			   }
			   DU_FREE(e2SetupReq->protocolIEs.list.array[idx2],\
				 sizeof(E2setupRequestIEs_t));
			   break;
			}
         case ProtocolIE_IDE2_id_RANfunctionsAdded:
             break;
		     default:
			DU_LOG("\nERROR  --> E2AP: Invalid event at e2SetupRequet %ld ",\
			      (e2SetupReq->protocolIEs.list.array[idx2]->id));
			break;
		  }
	       }
	    }
	    DU_FREE(e2SetupReq->protocolIEs.list.array, e2SetupReq->protocolIEs.list.size);
	 }
	 DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
   return ret;  
}
/*******************************************************************
 *
 * @brief Builds Ric Request Id
 *
 * @details
 *
 *    Function : BuildRicRequestId
 *
 *    Functionality: Building the Ric Request Id
 *
 * @params[in] RICrequestID_t *ricReqId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildRicRequestId(RICrequestID_t *ricReqId)
{
   if(ricReqId == NULLP)
   {
      return RFAILED;
   }

   ricReqId->ricRequestorID = 1;
   ricReqId->ricInstanceID  = 1;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills the mandatory RicAdmitted List Items
 *
 * @details
 *
 *    Function : fillRicAdmitList
 *
 *    Functionality: Fills the mandatory Ric Admitted List Items
 *
 * @params[in] RICaction_Admitted_ItemIEs_t *ricAdmitItems
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillRicAdmitList(RICaction_Admitted_ItemIEs_t *ricAdmitItems)
{

   if(ricAdmitItems != NULLP)
   {
      ricAdmitItems->id = ProtocolIE_IDE2_id_RICaction_Admitted_Item;
      ricAdmitItems->criticality = CriticalityE2_reject;
      ricAdmitItems->value.present = RICaction_Admitted_ItemIEs__value_PR_RICaction_Admitted_Item;
      ricAdmitItems->value.choice.RICaction_Admitted_Item.ricActionID = 1; 
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds the mandatory RicAdmitted List Params
 *
 * @details
 *
 *    Function : BuildRicAdmitList
 *
 *    Functionality: Builds the mandatory Ric Admitted List Params
 *
 * @params[in] RICaction_Admitted_List_t *admitListPtr
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildRicAdmitList(RICaction_Admitted_List_t *admitListPtr)
{
   uint8_t idx ;
   uint8_t elementCnt;  
   uint8_t ret= ROK;
   elementCnt = 1;

   if(admitListPtr == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for RIC Admit List failed");
      ret = RFAILED;
   }
   else
   {
      admitListPtr->list.count = elementCnt;
      admitListPtr->list.size  = elementCnt * sizeof(RICaction_Admitted_ItemIEs_t);
      DU_ALLOC(admitListPtr->list.array, admitListPtr->list.size);
      if(admitListPtr->list.array == NULLP)
      {
	 DU_LOG("\nERROR  -->  E2AP : Memory allocation for RIC Admit List failed");
	 ret = RFAILED;
      }
      else
      {
	 for(idx=0 ; idx<elementCnt ; idx++ )
	 {
	    DU_ALLOC(admitListPtr->list.array[idx], sizeof(RICaction_Admitted_ItemIEs_t));
	    if(admitListPtr->list.array[idx] == NULLP)
	    {
	       ret = RFAILED;
	    }
	 }
	 if(ret != RFAILED)
	 {
	    idx=0;
	    fillRicAdmitList((RICaction_Admitted_ItemIEs_t *)admitListPtr->list.array[idx]);
	 }
      }
   }	
   return ret;
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
 *
 ******************************************************************/
void FreeRicSubscriptionRsp(E2AP_PDU_t  *e2apRicMsg)
{
   RICsubscriptionResponse_t  *ricSubscriptionRsp= NULLP;
   uint8_t idx=0;
   uint8_t idx1=0;
   RICaction_Admitted_List_t *admitListPtr;

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
			case ProtocolIE_IDE2_id_RICrequestID:
			   break;

			case ProtocolIE_IDE2_id_RANfunctionID:
			   break;

			case ProtocolIE_IDE2_id_RICactions_Admitted:
			   {
			      admitListPtr = &ricSubscriptionRsp->protocolIEs.list.\
					     array[idx]->value.choice.RICaction_Admitted_List;
			      if(admitListPtr->list.array != NULLP)
			      {
				 for(idx1=0 ; idx1<admitListPtr->list.count; idx1++ )
				 {
				    if(admitListPtr->list.array[idx1] != NULLP)
				    {
				       DU_FREE(admitListPtr->list.array[idx1],
					     sizeof(RICaction_Admitted_ItemIEs_t));
				    }
				 }
				 DU_FREE(admitListPtr->list.array, admitListPtr->list.size);	 
			      }
			      break;
			   }
			default:
			   break;
		     }
		     DU_FREE(ricSubscriptionRsp->protocolIEs.list.array[idx], \
			   sizeof(RICsubscriptionResponse_IEs_t));
		  }
	       }
	       DU_FREE(ricSubscriptionRsp->protocolIEs.list.array, \
		     ricSubscriptionRsp->protocolIEs.list.size);
	    }
	 }   
	 DU_FREE(e2apRicMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }		
      DU_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));	      
   }
}
/*******************************************************************
 *
 * @brief Builds and Send the RicSubscriptionRsp
 *
 * @details
 *
 *    Function : BuildAndSendRicSubscriptionRsp
 *
 * functionality:Fills the RicSubscriptionRsp
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t  FillRicSubscriptionRsp(RICsubscriptionResponse_t  *ricSubscriptionRsp )
{
   uint8_t idx=0;
   uint8_t ret = ROK;
   uint8_t elementCnt = 0;
   uint8_t BuildRicRequestIdret=ROK;
   uint8_t BuildRicAdmitListret=ROK;

   elementCnt=3;
   ricSubscriptionRsp->protocolIEs.list.count = elementCnt;
   ricSubscriptionRsp->protocolIEs.list.size  = elementCnt * sizeof(RICsubscriptionResponse_IEs_t);
   DU_ALLOC(ricSubscriptionRsp->protocolIEs.list.array, \
	 ricSubscriptionRsp->protocolIEs.list.size);
   if(ricSubscriptionRsp->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for FillRicSubscriptionRsp  failed");
      ret = RFAILED;
   }
   else
   {
      for(idx=0; idx<ricSubscriptionRsp->protocolIEs.list.count; idx++)
      {
	 DU_ALLOC(ricSubscriptionRsp->protocolIEs.list.array[idx], \
	       sizeof(RICsubscriptionResponse_IEs_t));
	 if(ricSubscriptionRsp->protocolIEs.list.array[idx] == NULLP)
	 {
	    ret = RFAILED;
	 }
      }
      if(ret != RFAILED)
      {

	 idx=0;
	 ricSubscriptionRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
	 ricSubscriptionRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	 ricSubscriptionRsp->protocolIEs.list.array[idx]->value.present =\
									 RICsubscriptionRequest_IEs__value_PR_RICrequestID;
	 BuildRicRequestIdret =
	    BuildRicRequestId(&ricSubscriptionRsp->protocolIEs.list.array[idx]->value.choice.RICrequestID);
	 if(BuildRicRequestIdret != ROK)
	 {
	    ret = RFAILED;
	 }
	 else
	 {
	    idx++;
	    ricSubscriptionRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
	    ricSubscriptionRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	    ricSubscriptionRsp->protocolIEs.list.array[idx]->value.present =\
									    RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
	    ricSubscriptionRsp->protocolIEs.list.array[idx]->value.choice.RANfunctionID = 1;

	    idx++;
	    ricSubscriptionRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICactions_Admitted;
	    ricSubscriptionRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	    ricSubscriptionRsp->protocolIEs.list.array[idx]->value.present =\
									    RICsubscriptionResponse_IEs__value_PR_RICaction_Admitted_List;
	    BuildRicAdmitListret =
	       BuildRicAdmitList(&ricSubscriptionRsp->protocolIEs.list.array[idx]->value.choice.RICaction_Admitted_List);
	    if(BuildRicAdmitListret != ROK)
	    {
	       ret = RFAILED;
	    }
	 }
      }
   }	
   return ret;
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

uint8_t BuildAndSendRicSubscriptionRsp()
{

   E2AP_PDU_t         *e2apRicMsg = NULLP;
   RICsubscriptionResponse_t  *ricSubscriptionRsp=NULLP;
   asn_enc_rval_t     encRetVal; 
   uint8_t ret = RFAILED;
   uint8_t FillRicricSubscriptionRspret;

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
      e2apRicMsg->choice.successfulOutcome->value.present = \
							    SuccessfulOutcomeE2__value_PR_RICsubscriptionResponse;
      ricSubscriptionRsp = &e2apRicMsg->choice.successfulOutcome->value.choice.RICsubscriptionResponse;

      FillRicricSubscriptionRspret = FillRicSubscriptionRsp(ricSubscriptionRsp);
      if(FillRicricSubscriptionRspret != ROK)
      {
	 DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICsubscriptionResponseIE failed");
	 break;
      }

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apRicMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apRicMsg, PrepFinalEncBuf,\
	    encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG("\nERROR  -->  E2AP : Could not encode RIC Subscription Response structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for RIC subscription response \n");
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 } 
      } 

      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
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
   uint8_t idx; 
   uint32_t recvBufLen;             
   E2setupResponse_t *e2SetRspMsg;

   DU_LOG("\nINFO   -->  E2AP : E2 Setup Response received"); 
   duCb.e2Status = TRUE; //Set E2 status as true
   e2SetRspMsg = &e2apMsg->choice.successfulOutcome->value.choice.E2setupResponse;

   for(idx=0; idx<e2SetRspMsg->protocolIEs.list.count; idx++)
   {
      switch(e2SetRspMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_IDE2_id_GlobalRIC_ID:
	    {
	       /* To store the Ric Id Params */
	       recvBufLen = sizeof(e2SetRspMsg->protocolIEs.list.array[idx]->value\
		     .choice.GlobalRIC_ID.pLMN_Identity.size);
	       e2apMsgDb.plmn = NULLP;
	       DU_ALLOC(e2apMsgDb.plmn, recvBufLen);
	       if(e2apMsgDb.plmn)
	       {
		  memcpy(e2apMsgDb.plmn, e2SetRspMsg->protocolIEs.list.array[idx]\
			->value.choice.GlobalRIC_ID.pLMN_Identity.buf, recvBufLen);
		  free(e2SetRspMsg->protocolIEs.list.array[idx]->value.choice.\
			GlobalRIC_ID.pLMN_Identity.buf);
	       }
	       bitStringToInt(&e2SetRspMsg->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID.ric_ID, &e2apMsgDb.ricId);
	       free(e2SetRspMsg->protocolIEs.list.array[idx]->value.choice.\
		     GlobalRIC_ID.ric_ID.buf);
	       /*TODO : e2apMsgDb.plmn memory to be deallocated after the usage */
	       break;
	    }
	 default:
	    DU_LOG("\nERROR  -->  E2AP : Invalid IE received in E2SetupRsp:%ld",
		  e2SetRspMsg->protocolIEs.list.array[idx]->id);
	    break;
      }
      free(e2SetRspMsg->protocolIEs.list.array[idx]);
   }
   free(e2SetRspMsg->protocolIEs.list.array);
   return ROK;
}

/******************************************************************
 *
 * @brief Processes RIC Subscription Req sent by RIC
 *
 * @details
 *
 *    Function : procRicSubsReq
 *
 *    Functionality: Processes E2 Setup Response sent by CU
 *
 * @params[in] E2AP_PDU_t ASN decoded E2AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procRicSubsReq(E2AP_PDU_t *e2apMsg)
{
   uint8_t idx; 
   uint8_t ied; 
   uint8_t ret = ROK;
   uint32_t recvBufLen;             
   RICsubscriptionRequest_t *ricSubsReq;
   RICaction_ToBeSetup_ItemIEs_t *actionItem;

   DU_LOG("\nINFO   -->  E2AP : RIC Subscription request received"); 
   ricSubsReq = &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionRequest;

   for(idx=0; idx<ricSubsReq->protocolIEs.list.count; idx++)
   {
      if(ricSubsReq->protocolIEs.list.array[idx])
      {
	 switch(ricSubsReq->protocolIEs.list.array[idx]->id)
	 {
	    case ProtocolIE_IDE2_id_RICrequestID:
	       {
		  e2apMsgDb.ricReqId = ricSubsReq->protocolIEs.list.array[idx]->\
				       value.choice.RICrequestID.ricRequestorID;
		  e2apMsgDb.ricInstanceId = ricSubsReq->protocolIEs.list.array[idx]-> \
					    value.choice.RICrequestID.ricInstanceID;
		  break;
	       }
	    case ProtocolIE_IDE2_id_RANfunctionID:
	       {
		  e2apMsgDb.ranFuncId = ricSubsReq->protocolIEs.list.array[idx]-> \
					value.choice.RANfunctionID; 
		  break;
	       }
	    case ProtocolIE_IDE2_id_RICsubscriptionDetails:
	       {
		  recvBufLen = sizeof(ricSubsReq->protocolIEs.list.array[idx]->value\
			.choice.RICsubscriptionDetails.ricEventTriggerDefinition.size);
		  e2apMsgDb.ricEventTrigger = NULLP;
		  DU_ALLOC(e2apMsgDb.ricEventTrigger, recvBufLen);
		  /*TODO : e2apMsgDb.ricEventTrigger memory to be deallocated after the usage */
		  if(e2apMsgDb.ricEventTrigger)
		  {
		     memcpy(e2apMsgDb.ricEventTrigger, ricSubsReq->protocolIEs.list.array[idx]\
			   ->value.choice.RICsubscriptionDetails.ricEventTriggerDefinition.buf, \
			   recvBufLen);
		     free(ricSubsReq->protocolIEs.list.array[idx]->value.choice.\
			   RICsubscriptionDetails.ricEventTriggerDefinition.buf);
		  }
		  if(ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails.ricAction_ToBeSetup_List.\
			list.array)
		  {
		     actionItem =(RICaction_ToBeSetup_ItemIEs_t *)ricSubsReq->protocolIEs.list\
				 .array[idx]->value.choice.RICsubscriptionDetails.ricAction_ToBeSetup_List\
				 .list.array[0];

		     for(ied = 0; ied < ricSubsReq->protocolIEs.list.array[idx]->value.choice.\
			   RICsubscriptionDetails.ricAction_ToBeSetup_List.list.count; ied++)
		     {
			switch(actionItem->id)
			{
			   case ProtocolIE_IDE2_id_RICaction_ToBeSetup_Item:
			      {
				 e2apMsgDb.ricActionId = actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionID;
				 e2apMsgDb.ricActionType = actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionType;
				 break;
			      }
			   default:
			      DU_LOG("\nERROR  -->  E2AP : Invalid IE received in RicSetupLst:%ld",actionItem->id);
			      break;
			}
			free(actionItem);
		     }
		     free(ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails.ricAction_ToBeSetup_List.\
		     list.array);
		  }
		  break;
	       }

	    default:
	       DU_LOG("\nERROR  -->  E2AP : Invalid IE received in RIC SubsReq:%ld",
		     ricSubsReq->protocolIEs.list.array[idx]->id);
	       break;
	 }
	 free(ricSubsReq->protocolIEs.list.array[idx]);
      }
   }
   free(ricSubsReq->protocolIEs.list.array);
   ret = BuildAndSendRicSubscriptionRsp();

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
   uint8_t idx=0;
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
			   break;

			case ProtocolIE_IDE2_id_RANfunctionID:
			   break;

			case ProtocolIE_IDE2_id_RICactionID:
			   break;

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
 * brief Fill the RicIndication Message
 *
 * @details
 *
 *    Function : FillRicIndication
 *
 * Functionality:Fills the RicIndication Message
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t FillRicIndication(RICindication_t *ricIndicationMsg)
{
   uint8_t elementCnt=0;
   uint8_t idx=0;
   uint8_t ret = ROK;
   elementCnt = 6;

   ricIndicationMsg->protocolIEs.list.count = elementCnt;
   ricIndicationMsg->protocolIEs.list.size  = elementCnt * sizeof(RICindication_t);
   /* Initialize the Ric Indication members */
   DU_ALLOC(ricIndicationMsg->protocolIEs.list.array, \
	 ricIndicationMsg->protocolIEs.list.size);
   if(ricIndicationMsg->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICindicationIEs failed");
      ret = RFAILED;
   }
   else
   {
      for(idx=0; idx<elementCnt; idx++)
      {
	 DU_ALLOC(ricIndicationMsg->protocolIEs.list.array[idx],\
	       sizeof(RICindication_IEs_t));
	 if(ricIndicationMsg->protocolIEs.list.array[idx] == NULLP)
	 {
	    DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICindicationIEs failed");
	    ret = RFAILED;
	 }
      }
      if(ret != RFAILED)
      {
	 idx = 0;

	 ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
	 ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
									RICindication_IEs__value_PR_RICrequestID;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricRequestorID =\
												  e2apMsgDb.ricReqId;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricInstanceID =\
												 e2apMsgDb.ricInstanceId;

	 idx++;
	 ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
	 ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
									RICindication_IEs__value_PR_RANfunctionID;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RANfunctionID =
	    e2apMsgDb.ranFuncId;

	 idx++;
	 ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICactionID;
	 ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
									RICindication_IEs__value_PR_RICactionID;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICactionID =
	    e2apMsgDb.ricActionId;

	 idx++;
	 ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationType;
	 ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
									RICindication_IEs__value_PR_RICindicationType;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationType =
	    e2apMsgDb.ricActionType;

	 idx++;
	 ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationHeader;
	 ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
									RICindication_IEs__value_PR_RICindicationHeader;
	 ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader.size = 3 *
	    sizeof(uint8_t);
	 DU_ALLOC(ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader.buf ,\
	       ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader.size);
	 if(ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader.buf == NULLP)
	 {
	    DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICindicationIEs failed");
	    ret = RFAILED;
	 }
	 else
	 {
	    buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
		  ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader.buf);
	    idx++;
	    /* TO BE CHANGED: RIC INDICATION DATA */
	    /* For now filling a dummy octect data, need to tested with PRBs*/
	    ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationMessage;
	    ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
	    ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
									   RICindication_IEs__value_PR_RICindicationMessage;
	    ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage.size = 3 *
	       sizeof(uint8_t);
	    DU_ALLOC(ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage.buf ,\
		  ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage.size);
	    if(ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage.buf == NULLP)
	    {
	       DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICindicationIEs failed");
	       ret = RFAILED;
	    }
	    else
	    {
	       buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
		     ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage.buf);
	    }
	 }
      }
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

uint8_t BuildAndSendRicIndication()
{
   E2AP_PDU_t                 *e2apMsg = NULLP;
   RICindication_t            *ricIndicationMsg=NULLP;
   asn_enc_rval_t             encRetVal;        /* Encoder return value */
   uint8_t ret = RFAILED; 
   uint8_t FillRicIndicationret = ROK;

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

      FillRicIndicationret = FillRicIndication(ricIndicationMsg);
      if(FillRicIndicationret != ROK)
      {
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
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 } 
      }

      if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
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
 * @brief Sends E2 msg over SCTP
 *
 * @details
 *
 *    Function : SendE2APMsg
 *
 *    Functionality: Sends E2 msg over SCTP
 *
 * @params[in] Region region
 *             Pool pool
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t SendE2APMsg(Region region, Pool pool)
{
   Buffer *mBuf=NULLP;

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
	 ODU_PRINT_MSG(mBuf, 0,0);

	 if(sctpSend(mBuf, E2_INTERFACE) != ROK)
	 {
	    DU_LOG("\nERROR  -->  E2AP : SCTP Send for E2  failed");
	    ODU_PUT_MSG_BUF(mBuf);
	    return RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nERROR  -->  E2AP : ODU_ADD_POST_MSG_MULT failed");
	 ODU_PUT_MSG_BUF(mBuf);
	 return RFAILED;
      }
      ODU_PUT_MSG_BUF(mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to allocate memory");
      return RFAILED;
   }

   return ROK;
} /* SendE2APMsg */

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

   printf("\nDEBUG   -->  E2AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
      printf("%x",recvBuf[i]);
   }

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
      case E2AP_PDU_PR_successfulOutcome:
	 {
	    switch(e2apMsg->choice.successfulOutcome->value.present)
	    {
	       case SuccessfulOutcomeE2__value_PR_E2setupResponse:
		  {
		     if(!duCb.e2Status)
		     {
			DU_LOG("\nDEBUG   -->  E2AP : Store E2 setup response Params");
			procE2SetupRsp(e2apMsg);
		     }
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
		     if(procRicSubsReq(e2apMsg) == ROK)
		     {
			BuildAndSendRicIndication();
		     }
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
