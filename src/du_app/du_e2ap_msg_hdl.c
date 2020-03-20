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
#include "du_e2ap_msg_hdl.h"
#include "du_mgr_main.h"
#include "du_cfg.h"
#include "GlobalE2node-gNB-ID.h"

#define ENC_BUF_MAX_LEN 100

/* Global variable */
DuCfgParams duCfgParam;
char encBuf[ENC_BUF_MAX_LEN];
int  encBufSize;

S16 SendE2APMsg(Region , Pool );


/*******************************************************************
 *
 * @brief Writes the encoded chunks into a buffer
 *
 * @details
 *
 *    Function : PrepFinalEncBuf
 *
 *    Functionality:Fills the encoded buffer
 *
 * @params[in] void *buffer,initial encoded data
 * @params[in] size_t size,size of buffer
 * @params[in] void *encodedBuf,final buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
static int PrepFinalEncBuf(const void *buffer, size_t size, void *encodedBuf)
{
   memcpy(encodedBuf + encBufSize, buffer, size);
   encBufSize += size;
   return 0;
} /* PrepFinalEncBuf */

/*******************************************************************
 *
 * @brief Builds PLMN ID 
 *
 * @details
 *
 *    Function : BuildPlmnId
 *
 *    Functionality: Building the PLMN ID
 *
 * @params[in] PLMNID plmn
 *             OCTET_STRING_t *plmnid
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildPlmnId(PlmnId plmn, OCTET_STRING_t *plmnid)
{
   U8 mncCnt;
   plmnid->size = 3;
   DU_ALLOC(plmnid->buf,  plmnid->size * sizeof(U8));
   if(plmnid->buf == NULLP)
   {
      RETVALUE(RFAILED);
   }
   mncCnt = 2;
   plmnid->buf[0] = ((plmn.mcc[1] << 4) | (plmn.mcc[0]));
   if(mncCnt == 2)
   {
      plmnid->buf[1]  = ((0xf0) | (plmn.mcc[2]));
      plmnid->buf[2] = ((plmn.mnc[1] << 4) | (plmn.mnc[0]));
   }
   else
   {
      plmnid->buf[1] = ((plmn.mnc[0] << 4) | (plmn.mcc[2]));
      plmnid->buf[2] = ((plmn.mnc[2] << 4) | (plmn.mnc[1]));
   }
  RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Builds NodeB Id
 *
 * @details
 *
 *    Function : BuildNodeBId
 *
 *    Functionality: Building the NodeBId
 *
 * @params[in] BIT_STRING_t *nbid,
 *             U8 unusedBits
 *             U8 byteSize
 *             U8 val
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildNodeBId(BIT_STRING_t *nbid, U8 unusedBits, U8 byteSize, U8 val)
{
   U8 tmp;
   nbid->size = byteSize;
   DU_ALLOC(nbid->buf, nbid->size * sizeof(U8));
   if(nbid->buf == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (tmp = 0 ; tmp < ((nbid->size)-1); tmp++)
   {
      nbid->buf[tmp] = 0;
   }
   nbid->buf[byteSize-1] = val; 
   nbid->bits_unused = unusedBits;
   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Builds Global gNodeB Params
 *
 * @details
 *
 *    Function : BuildGlobalgNB
 *
 *    Functionality: Building the Plmn and gNB id
 *
 * @params[in] GlobalE2node_gNB_ID_t *gNbId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildGlobalgNB(GlobalE2node_gNB_ID_t *gNbId)
{
   U8 unused = 0;
   U8 byteSize = 4;
   U8 val = 1;
   if(gNbId != NULLP)
   {
      BuildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
       &gNbId->global_gNB_ID.plmn_id); 
      /* fill gND Id */
      gNbId->global_gNB_ID.gnb_id.present = GNB_ID_Choice_PR_gnb_ID; 
      BuildNodeBId(&gNbId->global_gNB_ID.gnb_id.choice.gnb_ID, unused, byteSize, val);
   }
   RETVALUE(ROK);   
}

#if 0
RANfunction_ItemIEs_t* BuildRanFuncItems(RANfunction_ItemIEs_t *items)
{
   U8 byteSize;
   U8 tmp;
   byteSize = 3;

   items->id = ProtocolIE_IDE2_id_RANfunctionID;   
   items->criticality = CriticalityE2_ignore;   
   items->value.present = RANfunction_ItemIEs__value_PR_RANfunction_Item;

   items->value.choice.RANfunction_Item.ranFunctionID = 1;
   BuildPlmn(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, &items->value.choice.RANfunction_Item.ranFunctionDefinition);
   items->value.choice.RANfunction_Item.ranFunctionRevision = 4; 

   RETVALUE(items);
}

S16 BuildRANList(RANfunctions_List_t *ranFuncList)
{
   U8 elementCnt;
   U8 eleidx;
   S16 ret;
   RANfunction_ItemIEs_t *funcItems;

   elementCnt = 1; 
   ranFuncList->list.count = elementCnt;
   ranFuncList->list.size = elementCnt * sizeof(RANfunction_ItemIEs_t);
   DU_ALLOC(ranFuncList->list.array, \
             ranFuncList->list.size);
   if(ranFuncList->list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for RAN Function List failed");
      RETVALUE(RFAILED);
   }
   /* Fill RAN function Params */
   DU_ALLOC(ranFuncList->list.array[0], sizeof(RANfunction_ItemIEs_t));
   BuildRanFuncItems(ranFuncList->list.array[0]);

   RETVALUE(ROK);
}
#endif

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

S16 BuildAndSendE2SetupReq()
{
   E2AP_PDU_t                 *e2apMsg = NULLP;
   E2setupRequest_t           *e2SetupReq;
   U8   elementCnt;
   U8   idx;
   U8   ieId;
   S16  ret; 
   asn_enc_rval_t             encRetVal;        /* Encoder return value */

   DU_LOG("\nE2AP : Building E2 Setup Request\n");

   DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
   if(e2apMsg == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      RETVALUE(RFAILED);
   }

   e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
   DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
   if(e2apMsg->choice.initiatingMessage == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }
   e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
   e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_E2setup;
   e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_E2setupRequest;
   
   DU_ALLOC(e2SetupReq, sizeof(E2setupRequest_t));
   e2SetupReq = &e2apMsg->choice.initiatingMessage->value.choice.E2setupRequest;
   
   elementCnt = 1;
   
   e2SetupReq->protocolIEs.list.count = elementCnt;
   e2SetupReq->protocolIEs.list.size = elementCnt * sizeof(E2setupRequestIEs_t);

   /* Initialize the E2Setup members */
   DU_ALLOC(e2SetupReq->protocolIEs.list.array, \
            e2SetupReq->protocolIEs.list.size);
   if(e2SetupReq->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2RequestIEs failed");
      DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      DU_FREE(e2apMsg, (Size)sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }
   
   for(idx=0; idx<elementCnt; idx++)
   {
      DU_ALLOC(e2SetupReq->protocolIEs.list.array[idx],\
            sizeof(E2setupRequestIEs_t));
      if(e2SetupReq->protocolIEs.list.array[idx] == NULLP)
      {
         for(ieId=0; ieId<idx; ieId++)
         {
            DU_FREE(e2SetupReq->protocolIEs.list.array[ieId],\
                  sizeof(E2setupRequestIEs_t));
         }
         DU_FREE(e2SetupReq->protocolIEs.list.array,\
                 e2SetupReq->protocolIEs.list.size);
         DU_FREE(e2apMsg->choice.initiatingMessage, \
               sizeof(InitiatingMessageE2_t));
         DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }

   idx = 0;
   /* GlobalE2node_gNB_ID */
   e2SetupReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_GlobalE2node_ID;
   e2SetupReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   e2SetupReq->protocolIEs.list.array[idx]->value.present =\
                                    E2setupRequestIEs__value_PR_GlobalE2node_ID;
   e2SetupReq->protocolIEs.list.array[idx]->value.choice.GlobalE2node_ID.present = \
                                                                   GlobalE2node_ID_PR_gNB;
   
   GlobalE2node_gNB_ID_t *gNbId;
   DU_ALLOC(gNbId, sizeof(GlobalE2node_gNB_ID_t));
   BuildGlobalgNB(gNbId);
   e2SetupReq->protocolIEs.list.array[idx]->value.choice.GlobalE2node_ID.choice.gNB = gNbId;

   /* Prints the Msg formed */
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);


   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
               encBuf);
   if(encRetVal.encoded == ENCODE_FAIL)
   {
	   DU_LOG("\nE2AP : Could not encode E2SetupRequest structure (at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	   RETVALUE(RFAILED);
   }
   else
   {
           DU_LOG("\nE2AP : Created APER encoded buffer for E2SetupRequest\n");
	   for(int i=0; i< encBufSize; i++)
	   {
		   printf("%x",encBuf[i]);
	   } 
   }

   if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
   {
	   DU_LOG("\nE2AP : Sending E2 Setup request failed");
	   RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of BuildAndSendE2SetupReq */

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

S16 BuildRicRequestId(RICrequestID_t *ricReqId)
{
   if(ricReqId != NULLP)
   {
      ricReqId->ricRequestorID = 1;
      ricReqId->ricInstanceID  = 1;
   }
   RETVALUE(ROK);
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

S16 fillRicAdmitList(RICaction_Admitted_ItemIEs_t *ricAdmitItems)
{

   if(ricAdmitItems != NULLP)
   {
      ricAdmitItems->id = ProtocolIE_IDE2_id_RICaction_Admitted_Item;
      ricAdmitItems->criticality = CriticalityE2_reject;
      ricAdmitItems->value.present = RICaction_Admitted_ItemIEs__value_PR_RICaction_Admitted_Item;
      ricAdmitItems->value.choice.RICaction_Admitted_Item.ricActionID = 1; 
   }
   RETVALUE(ROK);
}
/*******************************************************************
 *
 * @brief Builds the mandatory RicAdmitted List Params
 *
 * @details
 *
 *    Function : fillRicAdmitList
 *
 *    Functionality: Builds the mandatory Ric Admitted List Params
 *
 * @params[in] RICaction_Admitted_List_t *admitListPtr
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildRicAdmitList(RICaction_Admitted_List_t *admitListPtr)
{
   U8 elementCnt; 

   elementCnt = 1; 
   admitListPtr->list.count = elementCnt;
   admitListPtr->list.size  = elementCnt * sizeof(RICaction_Admitted_ItemIEs_t);
   DU_ALLOC(admitListPtr->list.array, admitListPtr->list.size);
   if(admitListPtr->list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for RIC Admit List failed");
      RETVALUE(RFAILED);
   }
   DU_ALLOC(admitListPtr->list.array[0], sizeof(RICaction_Admitted_ItemIEs_t));
   fillRicAdmitList(admitListPtr->list.array[0]);

   RETVALUE(ROK);
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

S16 BuildAndSendRicSubscriptionRsp()
{

   E2AP_PDU_t         *e2apRicMsg = NULLP;
   RICsubscriptionResponse_t  *ricSubscriptionRsp;
   asn_enc_rval_t     encRetVal; 
   U8 idx;
   U8 elementCnt;

 
   DU_LOG("\nE2AP : Building RIC Subscription Response\n");

   DU_ALLOC(e2apRicMsg, sizeof(E2AP_PDU_t)); 
   if(e2apRicMsg == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      RETVALUE(RFAILED);
   }
   e2apRicMsg->present =  E2AP_PDU_PR_successfulOutcome;
   DU_ALLOC(e2apRicMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
   if(e2apRicMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for Ric subscription Response failed");
      DU_FREE(e2apRicMsg, sizeof(RICsubscriptionResponse_t));
      RETVALUE(RFAILED);  
   }

   e2apRicMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_RICsubscription;
   e2apRicMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
   e2apRicMsg->choice.successfulOutcome->value.present = \
         SuccessfulOutcomeE2__value_PR_RICsubscriptionResponse;
   ricSubscriptionRsp = &e2apRicMsg->choice.successfulOutcome->value.choice.RICsubscriptionResponse;

   elementCnt = 3;
   ricSubscriptionRsp->protocolIEs.list.count = elementCnt;
   ricSubscriptionRsp->protocolIEs.list.size  = elementCnt * sizeof(RICsubscriptionResponse_IEs_t);

   DU_ALLOC(ricSubscriptionRsp->protocolIEs.list.array, \
              ricSubscriptionRsp->protocolIEs.list.size);
   if(ricSubscriptionRsp->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for RICsubscriptionResponseIE failed");
      DU_FREE(e2apRicMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      DU_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      DU_ALLOC(ricSubscriptionRsp->protocolIEs.list.array[idx], \
            sizeof(RICsubscriptionResponse_IEs_t)); 
      if(ricSubscriptionRsp->protocolIEs.list.array[idx] == NULLP)
      {  
         DU_FREE(ricSubscriptionRsp->protocolIEs.list.array,\
                   ricSubscriptionRsp->protocolIEs.list.size);
         DU_FREE(e2apRicMsg->choice.successfulOutcome, \
               sizeof(SuccessfulOutcomeE2_t));
         DU_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
         RETVALUE(RFAILED);
      }    
   }
   idx = 0;
   ricSubscriptionRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
   ricSubscriptionRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricSubscriptionRsp->protocolIEs.list.array[idx]->value.present =\
            RICsubscriptionRequest_IEs__value_PR_RICrequestID;
   BuildRicRequestId(&ricSubscriptionRsp->protocolIEs.list.array[idx]->value.choice.RICrequestID);
   
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
   BuildRicAdmitList(&ricSubscriptionRsp->protocolIEs.list.array[idx]->value.choice.RICaction_Admitted_List);


   /* Prints the Msg formed */
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apRicMsg);

   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apRicMsg, PrepFinalEncBuf,\
               encBuf);
   if(encRetVal.encoded == ENCODE_FAIL)
   {
	   DU_LOG("\nE2AP : Could not encode RIC Subscription Response structure (at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	   RETVALUE(RFAILED);
   }
   else
   {
           DU_LOG("\nE2AP : Created APER encoded buffer for RIC subscription response \n");
	   for(int i=0; i< encBufSize; i++)
	   {
		   printf("%x",encBuf[i]);
	   } 
   }

   if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
   {
      DU_LOG("\nE2AP : Sending RIC Subscription Response failed");      
      RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
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

S16 BuildAndSendRicIndication()
{
   E2AP_PDU_t                 *e2apMsg = NULLP;
   RICindication_t            *ricIndicationMsg;
   E2apMsgDb                   e2apMsgDb;
   U8   elementCnt;
   U8   idx;
   U8   ieId;
   S16  ret; 
   asn_enc_rval_t             encRetVal;        /* Encoder return value */

   DU_LOG("\nE2AP : Building Ric Indication Message\n");

   DU_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
   if(e2apMsg == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      RETVALUE(RFAILED);
   }

   e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
   DU_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
   if(e2apMsg->choice.initiatingMessage == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }
   e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICindication;
   e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
   e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICindication;
   
   DU_ALLOC(ricIndicationMsg, sizeof(RICindication_t));
   ricIndicationMsg = &e2apMsg->choice.initiatingMessage->value.choice.RICindication;
   
   elementCnt = 6;
   
   ricIndicationMsg->protocolIEs.list.count = elementCnt;
   ricIndicationMsg->protocolIEs.list.size  = elementCnt * sizeof(RICindication_t);

   /* Initialize the Ric Indication members */
   DU_ALLOC(ricIndicationMsg->protocolIEs.list.array, \
            ricIndicationMsg->protocolIEs.list.size);
   if(ricIndicationMsg->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for RICindicationIEs failed");
      DU_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      DU_FREE(e2apMsg, (Size)sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }
   
   for(idx=0; idx<elementCnt; idx++)
   {
      DU_ALLOC(ricIndicationMsg->protocolIEs.list.array[idx],\
            sizeof(RICindication_IEs_t));
      if(ricIndicationMsg->protocolIEs.list.array[idx] == NULLP)
      {
         for(ieId=0; ieId<idx; ieId++)
         {
            DU_FREE(ricIndicationMsg->protocolIEs.list.array[ieId],\
                  sizeof(RICindication_IEs_t));
         }
         DU_FREE(ricIndicationMsg->protocolIEs.list.array,\
                 ricIndicationMsg->protocolIEs.list.size);
         DU_FREE(e2apMsg->choice.initiatingMessage, \
               sizeof(InitiatingMessageE2_t));
         DU_FREE(e2apMsg, sizeof(E2AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }
   idx = 0;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
                                    RICindication_IEs__value_PR_RICrequestID;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricRequestorID = e2apMsgDb.ricReqId;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricInstanceID = e2apMsgDb.ricInstanceId;

   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
                                   RICindication_IEs__value_PR_RANfunctionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RANfunctionID = e2apMsgDb.ranFuncId;

   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICactionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
                                   RICindication_IEs__value_PR_RICactionID;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICactionID = e2apMsgDb.ricActionId;


   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationType;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
                                   RICindication_IEs__value_PR_RICindicationType;

   ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationType = e2apMsgDb.ricActionType;


   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationHeader;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
                                   RICindication_IEs__value_PR_RICindicationHeader;
   BuildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
                &ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationHeader);

   /* TO BE CHANGED: RIC INDICATION DATA */
   /* Foe now filling a dummy octect data, need to tested with PRBs*/ 
   idx++;
   ricIndicationMsg->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICindicationMessage;
   ricIndicationMsg->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricIndicationMsg->protocolIEs.list.array[idx]->value.present = \
                                  RICindication_IEs__value_PR_RICindicationMessage;
   BuildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn, \
                &ricIndicationMsg->protocolIEs.list.array[idx]->value.choice.RICindicationMessage);
  
   /* Prints the Msg formed */
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
               encBuf);
   if(encRetVal.encoded == ENCODE_FAIL)
   {
	   DU_LOG("\nE2AP : Could not encode RIC Indication Message (at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	   RETVALUE(RFAILED);
   }
   else
   {
           DU_LOG("\nE2AP : Created APER encoded buffer for RIC Indication Message \n");
	   for(int i=0; i< encBufSize; i++)
	   {
		   printf("%x",encBuf[i]);
	   } 
   }

   if(SendE2APMsg(DU_APP_MEM_REGION, DU_POOL) != ROK)
   {
      DU_LOG("\nE2AP : Sending RIC Indication Message");      
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
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

S16 SendE2APMsg(Region region, Pool pool)
{
   Buffer *mBuf;

   if(SGetMsg(region, pool, &mBuf) == ROK)
   {
      if(SAddPstMsgMult((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         SPrntMsg(mBuf, 0,0);
 
         if(sctpSend(mBuf, duCfgParam.sctpParams.itfType.e2Itf) != ROK)
         {
            DU_LOG("\nE2AP : SCTP Send for E2  failed");
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
 
   RETVALUE(ROK);
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
   int i;
   char *recvBuf;
   MsgLen copyCnt;
   MsgLen recvBufLen;
   E2AP_PDU_t *e2apMsg;
   asn_dec_rval_t rval; /* Decoder return value */
   E2AP_PDU_t e2apasnmsg ;
 
   DU_LOG("\nE2AP : Received E2AP message buffer");
   SPrntMsg(mBuf, 0,0);
 
   /* Copy mBuf into char array to decode it */
   SFndLenMsg(mBuf, &recvBufLen);
   if(SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&recvBuf, (Size)recvBufLen) != ROK)
   {
      DU_LOG("\nE2AP : Memory allocation failed");
      return;
   }
   if(SCpyMsgFix(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nE2AP : Failed while copying %d", copyCnt);
      return;
   }

   printf("\nE2AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
        printf("%x",recvBuf[i]);
   }

   /* Decoding flat buffer into E2AP messsage */
   e2apMsg = &e2apasnmsg;
   memset(e2apMsg, 0, sizeof(E2AP_PDU_t));
 
   rval = aper_decode(0, &asn_DEF_E2AP_PDU, (void **)&e2apMsg, recvBuf, recvBufLen, 0, 0);
   SPutSBuf(DFLT_REGION, DFLT_POOL, (Data *)recvBuf, (Size)recvBufLen);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nE2AP : ASN decode failed");
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
                 DU_LOG("\nE2AP : Store E2 setup response Params");
                 procE2SetupRsp(e2apMsg);
               }
	       break;
            }
            default:
            {
               DU_LOG("\nE2AP : Invalid type of intiating message [%d]", e2apMsg->choice.initiatingMessage->value.present);
               return;
            }
         }/* End of switch(successfulOutcome) */
         break;
      }
      case E2AP_PDU_PR_initiatingMessage:
      {
         switch(e2apMsg->choice.initiatingMessage->value.present)
         {
            case InitiatingMessageE2__value_PR_RICsubscriptionRequest: 
            {
               DU_LOG("\nE2AP : Calling RIC Subscription Response");
	       if(procRicSubsReq(e2apMsg) == ROK)
               {
                  BuildAndSendRicIndication();
               }
               break;
            }
            default:
            {
               DU_LOG("\nE2AP : Invalid type of successfulOutcome message [%d]", e2apMsg->choice.successfulOutcome->value.present);
               return;
            }
         }/* End of switch(initiatingMessage) */
         break;
      }
      default:
      {
         DU_LOG("\nE2AP : Invalid type of e2apMsg->present [%d]",e2apMsg->present);
         return;
      }

   }/* End of switch(e2apMsg->present) */
 
} /* End of E2APMsgHdlr */
