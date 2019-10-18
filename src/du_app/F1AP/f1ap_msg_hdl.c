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

#include "du_mgr_main.h"
#include "cu_stub_sctp.h"
#include "f1ap_msg_hdl.h"

char encBuf[ENC_BUF_MAX_LEN];

/*******************************************************************
 *
 * @brief Writes the encoded chunks into a buffer
 *
 * @details
 *
 *    Function : write_out
 *
 *    Functionality:Fills the encoded buffer
 *
 * @params[in] void *buffer,initial encoded data
 * @params[in] size_t size,size of buffer
 * @params[in] void *app_key,final buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
static int PrepFinalEncBuf(const void *buffer, size_t size, void *encodedBuf)
{
   memcpy(encodedBuf + encBufSize, buffer, size);
   encBufSize += size;
   return 0;
}

/*******************************************************************
 *
 * @brief Builds the F1SetupRequest
 *
 * @details
 *
 *    Function : BuildF1SetupReq
 *
 * Functionality:Fills the F1SetupRequest
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
S16 BuildF1SetupReq()
{
   S16  ret;
   U8   idx;
   U8   elementCnt;
   F1AP_PDU_t         *f1apMsg = NULL;
   F1SetupRequest_t   *f1SetupReq;
   asn_enc_rval_t     encRetVal;        /* Encoder return value */
   
   printf("\nBuilding F1 Setup Request\n");

   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&f1apMsg, (Size)sizeof(F1AP_PDU_t));
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      RETVALUE(ret);
   }

   f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&(f1apMsg->choice.initiatingMessage),\
         (Size)sizeof(InitiatingMessage_t));
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }

   f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_F1Setup;
   f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
   f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_F1SetupRequest;

   f1SetupReq = &f1apMsg->choice.initiatingMessage->value.choice.F1SetupRequest;

   elementCnt = 3;
   f1SetupReq->protocolIEs.list.count = elementCnt;
   f1SetupReq->protocolIEs.list.size = elementCnt * sizeof(F1SetupRequestIEs_t *);

   /* Initialize the F1Setup members */
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL,(Data **)&(f1SetupReq->protocolIEs.list.array),\
         (Size)elementCnt * sizeof(F1SetupRequestIEs_t *));
   if(ret != ROK)
   {
      printf("Memory allocation for F1RequestIEs failed");
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(f1apMsg->choice.initiatingMessage),\
            (Size)sizeof(InitiatingMessage_t));
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,\
            (Data **)&(f1SetupReq->protocolIEs.list.array[idx]),\
            (Size)sizeof(F1SetupRequestIEs_t));
      if(ret != ROK)
      {
         SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(f1SetupReq->protocolIEs.list.array),\
               (Size)elementCnt * sizeof(F1SetupRequestIEs_t *));
         SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(f1apMsg->choice.initiatingMessage),\
               (Size)sizeof(InitiatingMessage_t));
         SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
         RETVALUE(ret);
      }
   }

   /*TransactionID*/
   f1SetupReq->protocolIEs.list.array[0]->id = ProtocolIE_ID_id_TransactionID ;
   f1SetupReq->protocolIEs.list.array[0]->criticality = Criticality_reject;
   f1SetupReq->protocolIEs.list.array[0]->value.present = F1SetupRequestIEs__value_PR_TransactionID;
   f1SetupReq->protocolIEs.list.array[0]->value.choice.TransactionID = TRANS_ID;

  /*DU ID*/
   f1SetupReq->protocolIEs.list.array[1]->id = ProtocolIE_ID_id_gNB_DU_ID;
   f1SetupReq->protocolIEs.list.array[1]->criticality = Criticality_reject;
   f1SetupReq->protocolIEs.list.array[1]->value.present = \
         F1SetupRequestIEs__value_PR_GNB_DU_ID;
   f1SetupReq->protocolIEs.list.array[1]->value.choice.GNB_DU_ID.size = sizeof(U8);
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL,\
         (Data **)&(f1SetupReq->protocolIEs.list.array[1]->value.choice.GNB_DU_ID.buf),\
         (Size)sizeof(uint8_t));

   if(ret != ROK)
   {
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(f1SetupReq->protocolIEs.list.array),\
            (Size)elementCnt * sizeof(F1SetupRequestIEs_t *));
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(f1apMsg->choice.initiatingMessage),\
            (Size)sizeof(InitiatingMessage_t));
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }
   f1SetupReq->protocolIEs.list.array[1]->value.choice.GNB_DU_ID.buf[0] = duCfgParam.duId;

   /*DU Name*/
   f1SetupReq->protocolIEs.list.array[2]->id = ProtocolIE_ID_id_gNB_DU_Name ;
   f1SetupReq->protocolIEs.list.array[2]->criticality = Criticality_ignore;
   f1SetupReq->protocolIEs.list.array[2]->value.present = \
         F1SetupRequestIEs__value_PR_GNB_DU_Name;
   f1SetupReq->protocolIEs.list.array[2]->value.choice.GNB_DU_Name.size = \
         sizeof(duCfgParam.duName);
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL,\
         (Data **)&(f1SetupReq->protocolIEs.list.array[2]->value.choice.GNB_DU_Name.buf),\
         (Size)sizeof(duCfgParam.duName));
   if(ret != ROK)
   {
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&(f1SetupReq->protocolIEs.list.array[1]->value.choice.GNB_DU_ID.buf),\
            (Size)sizeof(uint8_t));
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&(f1SetupReq->protocolIEs.list.array),\
            (Size)elementCnt * sizeof(F1SetupRequestIEs_t *));
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&(f1apMsg->choice.initiatingMessage),\
            (Size)sizeof(InitiatingMessage_t));
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }

   strcpy((char*)f1SetupReq->protocolIEs.list.array[2]->value.choice.GNB_DU_Name.buf,
         (char*)&duCfgParam.duName);

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   if(encRetVal.encoded == ENCODE_FAIL)
   {
      printf( "\nCould not encode F1SetupRequest structure (at %s)\n",\
            encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      RETVALUE(RFAILED);
   }
   else
   {
      printf("\nCreated APER encoded buffer for F1SetupRequest\n");
      for(int i=0; i< encBufSize; i++)
      {
         printf("%x",encBuf[i]);
      } 
   }
   RETVALUE(ROK);
}/* End of BuildF1SetupReq */

/*******************************************************************
 *
 * @brief Builds and sends the F1SetupResponse
 *
 * @details
 *
 *    Function : ExtractSendF1SetupRsp
 *
 *    Functionality: Constructs the F1SetupResponse message and sends
 *                   it back to the DU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildF1SetupRsp()
{
   S16   ret;
   U8    idx;
   U8    elementCnt;
   F1AP_PDU_t         *f1apMsg = NULL;
   F1SetupResponse_t  *f1SetupRsp;
   GNB_CU_Name_t      *cuName;
   RRC_Version_t      *rrc_Ver;
   asn_enc_rval_t     encRetVal; 

   printf("\nBuilding F1 Setup Response\n");

   /* Allocate the memory for F1SetupRequest_t */
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&f1apMsg, (Size)sizeof(F1AP_PDU_t)); 
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      RETVALUE(ret);
   }
   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,(Data **)&(f1apMsg->choice.successfulOutcome),\
         (Size)sizeof(SuccessfulOutcome_t));
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);  
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_F1Setup;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = \
         SuccessfulOutcome__value_PR_F1SetupResponse;
   f1SetupRsp = &f1apMsg->choice.successfulOutcome->value.choice.F1SetupResponse;

   elementCnt = 3;
   f1SetupRsp->protocolIEs.list.count = elementCnt;
   f1SetupRsp->protocolIEs.list.size = elementCnt*sizeof(F1SetupResponseIEs_t *);

   ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,(Data **)&(f1SetupRsp->protocolIEs.list.array),\
         (Size)elementCnt * sizeof(F1SetupResponseIEs_t *));
   if(ret != ROK)
   {
      printf("Memory allocation for F1ResponseIEs failed");
      SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
            (Size)sizeof(SuccessfulOutcome_t));
      SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,\
            (Data **)&(f1SetupRsp->protocolIEs.list.array[idx]),\
            (Size)sizeof(F1SetupResponseIEs_t)); 
      if(ret != ROK)
      {  
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1SetupRsp->protocolIEs.list.array),\
               (Size)elementCnt * sizeof(F1SetupResponseIEs_t *));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
               (Size)sizeof(SuccessfulOutcome_t));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
         RETVALUE(ret);
      }    
   }

   /*TransactionID*/
   f1SetupRsp->protocolIEs.list.array[0]->id = ProtocolIE_ID_id_TransactionID ;
   f1SetupRsp->protocolIEs.list.array[0]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[0]->value.present = \
         F1SetupResponseIEs__value_PR_TransactionID;
   f1SetupRsp->protocolIEs.list.array[0]->value.choice.TransactionID = TRANS_ID;

   /*CU Name*/
   f1SetupRsp->protocolIEs.list.array[1]->id = ProtocolIE_ID_id_gNB_CU_Name;
   f1SetupRsp->protocolIEs.list.array[1]->criticality = Criticality_ignore;
   f1SetupRsp->protocolIEs.list.array[1]->value.present = \
         F1SetupResponseIEs__value_PR_GNB_CU_Name;
   cuName = &f1SetupRsp->protocolIEs.list.array[1]->value.choice.GNB_CU_Name;
   cuName->size = sizeof(cuCfgParams.cuName);

   ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL, (Data **)&(cuName->buf),\
         (Size)sizeof(cuName->size)); 
   if(ret != ROK)
      {
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1SetupRsp->protocolIEs.list.array),\
               (Size)elementCnt * sizeof(F1SetupResponseIEs_t *));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
               (Size)sizeof(SuccessfulOutcome_t));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
         RETVALUE(ret);
      }
   strcpy((char*)cuName->buf, (char*)cuCfgParams.cuName);

   /* RRC Version */
   f1SetupRsp->protocolIEs.list.array[2]->id = ProtocolIE_ID_id_GNB_CU_RRC_Version;
   f1SetupRsp->protocolIEs.list.array[2]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[2]->value.present = \
         F1SetupResponseIEs__value_PR_RRC_Version;
   rrc_Ver = &f1SetupRsp->protocolIEs.list.array[2]->value.choice.RRC_Version;
   rrc_Ver->latest_RRC_Version.size = RRC_SIZE; 

   ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,\
         (Data **)&(rrc_Ver->latest_RRC_Version.buf), sizeof(U8));
   if(ret != ROK)
      {  
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(cuName->buf),(Size)sizeof(cuName->size));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1SetupRsp->protocolIEs.list.array),\
               (Size)elementCnt * sizeof(F1SetupResponseIEs_t *));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
               (Size)sizeof(SuccessfulOutcome_t));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
         RETVALUE(ret);
      }

  /* Need to check RRC Version */
   rrc_Ver->latest_RRC_Version.buf[0] = cuCfgParams.rrcVersion.rrcVer; 
   rrc_Ver->latest_RRC_Version.bits_unused = 5; //TODO: pick from cuCfgParam. If not present, add it

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   if(encRetVal.encoded == ENCODE_FAIL)
   {
      printf("Could not encode F1SetupResponse structure (at %s)\n",\
            encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      ret = RFAILED;
   } 
   else 
   {
      printf("\nCreated APER encoded buffer for F1SetupResponse\n");
      for(int i=0; i< encBufSize; i++)
      {
         printf("%x",encBuf[i]);
      } 
   }
   return ret;
}/* End of BuildF1SetupRsp */

/*******************************************************************
 *
 * @brief Builds and sends the DUConfigUpdate
 *
 * @details
 *
 *    Function : BuildDUConfigUpdate
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

S16 BuildDUConfigUpdate()
{
   S16   ret;
   U8    idx;
   U8    elementCnt;
   asn_enc_rval_t encRetVal;      /* Encoder return value */
   F1AP_PDU_t *f1apDuCfg = NULL;
   GNBDUConfigurationUpdate_t *duCfgUpdate;

   printf("\nBuilding DU config update\n");

   /* Allocate the memory for F1DuCfg */
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&f1apDuCfg, (Size)sizeof(F1AP_PDU_t));
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      RETVALUE(ret);
   }
 
   f1apDuCfg->present = F1AP_PDU_PR_initiatingMessage;
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&(f1apDuCfg->choice.initiatingMessage),\
         (Size)sizeof(InitiatingMessage_t));
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&f1apDuCfg, (Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }
   
   f1apDuCfg->choice.initiatingMessage->procedureCode = ProcedureCode_id_gNBDUConfigurationUpdate;
   f1apDuCfg->choice.initiatingMessage->criticality = Criticality_reject;
   f1apDuCfg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_GNBDUConfigurationUpdate;
   duCfgUpdate = &f1apDuCfg->choice.initiatingMessage->value.choice.GNBDUConfigurationUpdate;

   elementCnt = 1;
   duCfgUpdate->protocolIEs.list.count = elementCnt;
   duCfgUpdate->protocolIEs.list.size = elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *);

   /* Initialize the F1Setup members */
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&(duCfgUpdate->protocolIEs.list.array),\
         (Size)elementCnt*sizeof(GNBDUConfigurationUpdateIEs_t *));
   if(ret != ROK)
   {
      printf("Memory allocation for F1RequestIEs failed");
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(f1apDuCfg->choice.initiatingMessage),\
              (Size)sizeof(InitiatingMessage_t));
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&f1apDuCfg, (Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }

   for(idx=0;idx<elementCnt;idx++)
   {
      ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL,\
        (Data **)&(duCfgUpdate->protocolIEs.list.array[idx]),\
        (Size)sizeof(GNBDUConfigurationUpdateIEs_t));
      if(ret != ROK)
      {
         SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(duCfgUpdate->protocolIEs.list.array),\
            (Size)elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t *));
         SPutSBuf(DU_APP_MEM_REGION, DU_POOL, (Data *)&(f1apDuCfg->choice.initiatingMessage), (Size)sizeof(InitiatingMessage_t));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apDuCfg,(Size)sizeof(F1AP_PDU_t));
         RETVALUE(ret);
      }
   }

   /*TransactionID*/
   duCfgUpdate->protocolIEs.list.array[0]->id = ProtocolIE_ID_id_TransactionID ;
   duCfgUpdate->protocolIEs.list.array[0]->criticality = Criticality_reject;
   duCfgUpdate->protocolIEs.list.array[0]->value.present = GNBDUConfigurationUpdateIEs__value_PR_TransactionID;
   duCfgUpdate->protocolIEs.list.array[0]->value.choice.TransactionID = TRANS_ID;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apDuCfg);
   
   /* Encode the DU Config Update type as APER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apDuCfg, PrepFinalEncBuf, encBuf);
   if(encRetVal.encoded == ENCODE_FAIL) 
   {
      printf("Could not encode DUConfigUpdate structure (at %s)\n",encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      RETVALUE(RFAILED);
   } 
   else 
   {
      printf("\nCreated APER encoded buffer for DUConfigUpdate\n");
      for(int i=0; i< encBufSize; i++)
      {
         printf("%x",encBuf[i]);
      }
   }
   RETVALUE(ROK);
}/* End of BuildDUConfigUpdate */

/*******************************************************************
 *
 * @brief Builds and sends the DUUpdateAcknowledge
 *
 * @details
 *
 *    Function : BuildDUUpdateAck
 *
 *    Functionality: Constructs the DU Update Acknowledge message and sends
 *                   it to the DU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildDUUpdateAck()
{
   S16  ret;
   U8   idx;
   U8   elementCnt;
   F1AP_PDU_t *f1apMsg = NULL;
   GNBDUConfigurationUpdateAcknowledge_t *gNBDuCfgAck;
   asn_enc_rval_t enRetVal; /* Encoder return value */

   printf("\nBuilding GNB-DU Config Update Ack\n");

   /* Allocate the memory for F1SetupRequest_t */
   ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&f1apMsg, (Size)sizeof(F1AP_PDU_t));
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      RETVALUE(ret);
   }

   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,(Data **)&(f1apMsg->choice.successfulOutcome),\
         (Size)sizeof(SuccessfulOutcome_t));
   if(ret != ROK)
   {
      printf("Memory allocation for F1AP-PDU failed");
      SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_gNBDUConfigurationUpdate;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge;
   gNBDuCfgAck = &f1apMsg->choice.successfulOutcome->value.choice.GNBDUConfigurationUpdateAcknowledge;

   elementCnt = 1;
   gNBDuCfgAck->protocolIEs.list.count = elementCnt;
   gNBDuCfgAck->protocolIEs.list.size = elementCnt*sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t);

   /* Initialize the F1Setup members */
   ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,(Data **)&(gNBDuCfgAck->protocolIEs.list.array),\
         (Size)elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   if(ret != ROK)
   {
      printf("Memory allocation for DuUpdateAcknowledgeIEs failed");
      SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
            (Size)sizeof(SuccessfulOutcome_t));
      SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(ret);
   }

    for(idx=0; idx<elementCnt; idx++)
   {
      ret = SGetSBuf(DU_APP_MEM_REGION,DU_POOL,\
            (Data **)&(gNBDuCfgAck->protocolIEs.list.array[idx]),\
            (Size)sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
      if(ret != ROK)
      {
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(gNBDuCfgAck->protocolIEs.list.array),\
               (Size)elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
            (Size)sizeof(SuccessfulOutcome_t));

         SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
         RETVALUE(ret);
      }
   }

   /*TransactionID*/ 
   gNBDuCfgAck->protocolIEs.list.array[0]->id = ProtocolIE_ID_id_TransactionID ;
   gNBDuCfgAck->protocolIEs.list.array[0]->criticality = Criticality_reject;
   gNBDuCfgAck->protocolIEs.list.array[0]->value.present = GNBDUConfigurationUpdateAcknowledgeIEs__value_PR_TransactionID;
   gNBDuCfgAck->protocolIEs.list.array[0]->value.choice.TransactionID = TRANS_ID;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);
 
   /* Encode the F1SetupRequest type as UPER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   enRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);
   if(enRetVal.encoded == ENCODE_FAIL) 
   {
      printf("\nCould not encode DUConfigUpdateAcknowledge structure (at %s)",enRetVal.failed_type ? enRetVal.failed_type->name : "unknown");
      RETVALUE(RFAILED); 
   } 
   else 
   {
      printf("\nCreated APER encoded buffer for DuConfigUpdateAcknowledge\n");
      for(int i=0; i< encBufSize; i++)
      {
         printf("%x",encBuf[i]);
      } 
   }
   RETVALUE(ROK);

}/* End of BuildDUUpdateAck*/

/*******************************************************************
*
* @brief Handles received F1AP message and sends back response  
*
* @details
*
*    Function : F1InmsgHdlr
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
void F1InmsgHdlr(Buffer *mBuf)
{
   int i,j;
   char *recvBuf;
   char *finalBuf;
   MsgLen copyCnt;
   MsgLen recvBufLen;
   F1AP_PDU_t *f1apmsg;
   asn_dec_rval_t rval; /* Decoder return value */
   F1AP_PDU_t f1apasnmsg ;
   Buffer *respBuf;
 
   printf("\nReceived F1AP message buffer");
   SPrntMsg(mBuf, 0,0);
 
   /* Copy mBuf into char array to decode it */
   SFndLenMsg(mBuf, &recvBufLen);
   if(SGetSBuf(1, 1, (Data **)&recvBuf, (Size)recvBufLen) != ROK)
   {
      printf("Memory allocation failed");
      return;
   }
   if(SCpyMsgFix(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      printf("\nFailed while copying %d", copyCnt);
      return;
   }

   printf("\nReceived flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
        printf("%x",recvBuf[i]);
   }

   /* Decoding flat buffer into F1AP messsage */
   f1apmsg = &f1apasnmsg;
   memset(f1apmsg, 0, sizeof(F1AP_PDU_t));
 
   rval = aper_decode(0, &asn_DEF_F1AP_PDU, (void **)&f1apmsg, recvBuf, recvBufLen, 0, 0);
   SPutSBuf(1, 1, (Data *)recvBuf, (Size)recvBufLen);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      printf("\nASN decode failed");
      return;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apmsg);

   switch(f1apmsg->present)
   {
      case F1AP_PDU_PR_initiatingMessage:
      {
         switch(f1apmsg->choice.initiatingMessage->value.present)
         {
            case InitiatingMessage__value_PR_F1SetupRequest:
            {
               printf("\nF1 setup request received");

               BuildF1SetupRsp();
               
               /* Reversing the encoded string */
               if(SGetSBuf(1, 1, (Data **)&finalBuf, (Size)encBufSize) != ROK)
               {
                  printf("Memory allocation failed");
                  return;
               }
               for(i = 0, j = encBufSize-1; i<encBufSize; i++, j--)
               {
                  finalBuf[j] = encBuf[i];
               }

               if(SGetMsg(1, 1, &respBuf) == ROK)
               {
                  if(SCpyFixMsg((Data *)finalBuf, respBuf, 0, encBufSize, &copyCnt) == ROK)
                  {
                     printf("\nSending F1 setup response");
                     SPrntMsg(respBuf, 0,0);
                     if(sctpOutMsgSend(respBuf) != ROK)
                     {
                        printf("\nFailed Sending");
                     }
                     SPutMsg(respBuf);
                  }
               }
               else
               {
                  printf("\nFailed to allocate memory");
                  return;
               }
               break;
            }

            case InitiatingMessage__value_PR_GNBDUConfigurationUpdate:
            {
               printf("\nGNB-DU config update received");
               BuildDUUpdateAck();
               /* Reversing the encoded string */
               if(SGetSBuf(1, 1, (Data **)&finalBuf, (Size)encBufSize) != ROK)
               {
                  printf("Memory allocation failed");
                  return;
               }
               for(i = 0, j = encBufSize-1; i<encBufSize; i++, j--)
               {
                  finalBuf[j] = encBuf[i];
               }

               if(SGetMsg(1, 1, &respBuf) == ROK)
               {
                  if(SCpyFixMsg((Data *)finalBuf, respBuf, 0, encBufSize, &copyCnt) == ROK)
                  {
                     printf("\nSending GNB-DU Config Update Acknowledgement");
                     SPrntMsg(respBuf, 0,0);
                     if(sctpOutMsgSend(respBuf) != ROK)
                     {
                        printf("\nFailed Sending");
                     }
                     SPutMsg(respBuf);
                  }
               }
               else
               {
                  printf("\nFailed to allocate memory");
                  return;
               }  
               break;
            }

            default:
            {
               printf("\nInvalid type of intiating message [%d]",f1apmsg->choice.initiatingMessage->value.present);
               return;
            }
         }/* End of switch(initiatingMessage) */
         break;
      }

      case F1AP_PDU_PR_successfulOutcome:
      {
         switch(f1apmsg->choice.successfulOutcome->value.present)
         {
            case SuccessfulOutcome__value_PR_F1SetupResponse:
            {
               F1SetupResponse_t *f1SetRspMsg;
               F1SetupRsp    f1SetRspDb;
               GNB_CU_Name_t *cuName;
               RRC_Version_t *rrc_Ver;

               printf("\nF1 Setup Response");
               
               /* Store the received info in local database */
               f1SetRspMsg = &f1apmsg->choice.successfulOutcome->value.choice.F1SetupResponse;
               cuName = &f1SetRspMsg->protocolIEs.list.array[1]->value.choice.GNB_CU_Name;
               rrc_Ver = &f1SetRspMsg->protocolIEs.list.array[2]->value.choice.RRC_Version;

               f1SetRspDb.transId = f1SetRspMsg->protocolIEs.list.array[0]->value.choice.TransactionID;
               strcpy(f1SetRspDb.cuName, cuName->buf);
               //strcpy(f1SetRspDb.rrcVersion.rrcVer, rrc_Ver->latest_RRC_Version.buf[0]);

               /* TODO :Check the deallocation */
#if 0
               SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&(f1SetupRsp->protocolIEs.list.array),\
                  (Size)elementCnt * sizeof(F1SetupResponseIEs_t *));
               SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
                  (Size)sizeof(SuccessfulOutcome_t));
               SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
#endif

               /* Build and send GNB-DU config update */
               BuildDUConfigUpdate();
               
               /* Reversing the encoded string */
               if(SGetSBuf(1, 1, (Data **)&finalBuf, (Size)encBufSize) != ROK)
               {
                  printf("Memory allocation failed");
                  return;
               }
               for(i = 0, j = encBufSize-1; i<encBufSize; i++, j--)
               {
                  finalBuf[j] = encBuf[i];
               }

               if(SGetMsg(1, 1, &respBuf) == ROK)
               {
                  if(SCpyFixMsg((Data *)finalBuf, respBuf, 0, encBufSize, &copyCnt) == ROK)
                  {
                     printf("\nSending GNB-DU Config Update");
                     SPrntMsg(respBuf, 0,0);
                     if(sctpOutMsgSend(respBuf) != ROK)
                     {
                        printf("\nFailed Sending");
                     }
                     SPutMsg(respBuf);
                  }
               }
               else
               {  
                  printf("\nFailed to allocate memory");
                  return;
               }

               break;
            }
            
            case SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge:
            {
               F1GnbDuCfgUpdAck duCfgUpdAckDb;
               GNBDUConfigurationUpdateAcknowledge_t *gnbDuCfgUpdAckMsg;

               printf("\nGNB-DU config update acknowledgment received");

               /* Store the received info in local database */ 
               gnbDuCfgUpdAckMsg = &f1apmsg->choice.successfulOutcome->value.choice.GNBDUConfigurationUpdateAcknowledge;
               duCfgUpdAckDb.transId = gnbDuCfgUpdAckMsg->protocolIEs.list.array[0]->value.choice.TransactionID;
              
               /* TODO :Check the deallocation */
#if 0
               SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(gNBDuCfgAck->protocolIEs.list.array),\
                  (Size)elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
               SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
                  (Size)sizeof(SuccessfulOutcome_t));
               SPutSBuf(DU_APP_MEM_REGION,DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
#endif     
               break;
            }

            default:
            {
               printf("\nInvalid type of successful outcome [%d]", f1apmsg->choice.successfulOutcome->value.present);
               return;
            }
         }/* End of switch(successfulOutcome) */
         break;
      }
      
      default:
      {
         printf("\nInvalie type of f1apMsg->present [%d]",f1apmsg->present);
         return;
      }

   }/* End of switch(f1apmsg->present) */
 
} /* End of F1InmsgHdlr */
 

/**********************************************************************
         End of file
 **********************************************************************/
