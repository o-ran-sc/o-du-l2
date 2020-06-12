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

#include "cu_stub_sctp.h"
#include "cu_f1ap_msg_hdl.h"

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
   Buffer *mBuf = NULLP;

   if(SGetMsg(region, pool, &mBuf) == ROK)
   {
      if(SAddPstMsgMult((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         SPrntMsg(mBuf, 0,0);
 
         if(sctpSend(mBuf) != ROK)
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
 
   RETVALUE(ROK);
} /* SendF1APMsg */

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
   U8 tmp;
   for (tmp = 0 ; tmp < nrcell->size-1; tmp++)
   {
      nrcell->buf[tmp] = 0;
   }
   nrcell->buf[4]   = 16; 
   nrcell->bits_unused = 4;
   nrcell->size = 5 * sizeof(uint8_t);
   RETVALUE(ROK);
}

/********************************************************************
 *
 * @brief Builds and sends the F1SetupResponse
 *
 * @details
 *
 *    Function : BuildAndSendF1SetupRsp
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
S16 BuildAndSendF1SetupRsp()
{
   U8    idx,idy;
   U8    elementCnt,cellCnt;
   F1AP_PDU_t         *f1apMsg = NULL;
   F1SetupResponse_t  *f1SetupRsp;
   GNB_CU_Name_t      *cuName;
   Cells_to_be_Activated_List_t *cellToActivate;
	RRC_Version_t      *rrcVer;
	asn_enc_rval_t     encRetVal; 
	DU_LOG("\nF1AP : Building F1 Setup Response\n");

   /* Allocate the memory for F1SetupRequest_t */
   CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t)); 
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      RETVALUE(RFAILED);
   }
   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);  
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_F1Setup;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = \
         SuccessfulOutcome__value_PR_F1SetupResponse;
   f1SetupRsp = &f1apMsg->choice.successfulOutcome->value.choice.F1SetupResponse;

   elementCnt = 4;
   f1SetupRsp->protocolIEs.list.count = elementCnt;
   f1SetupRsp->protocolIEs.list.size = elementCnt*sizeof(F1SetupResponseIEs_t *);

   CU_ALLOC(f1SetupRsp->protocolIEs.list.array, \
         elementCnt * sizeof(F1SetupResponseIEs_t *));
   if(f1SetupRsp->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1ResponseIEs failed");
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(f1SetupRsp->protocolIEs.list.array[idx], \
            sizeof(F1SetupResponseIEs_t)); 
      if(f1SetupRsp->protocolIEs.list.array[idx] == NULLP)
      {  
         CU_FREE(f1SetupRsp->protocolIEs.list.array,\
               elementCnt * sizeof(F1SetupResponseIEs_t *));
         CU_FREE(f1apMsg->choice.successfulOutcome, \
               sizeof(SuccessfulOutcome_t));
         CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }    
   }

   /*TransactionID*/
   idx = 0;
   f1SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                     F1SetupResponseIEs__value_PR_TransactionID;
   f1SetupRsp->protocolIEs.list.array[idx]->value.choice.TransactionID =\
                                                                       TRANS_ID;

   /*CU Name*/
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_gNB_CU_Name;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                       F1SetupResponseIEs__value_PR_GNB_CU_Name;
   cuName = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_Name;
   cuName->size = sizeof(cuCfgParams.cuName);

   CU_ALLOC(cuName->buf, sizeof(cuName->size)); 
   if(cuName->buf == NULLP)
      {
         for(idy=0; idy<elementCnt; idy++)
         {
            CU_FREE(f1SetupRsp->protocolIEs.list.array[idy],\
                  sizeof(F1SetupResponseIEs_t));
         }
         CU_FREE(f1SetupRsp->protocolIEs.list.array,\
               elementCnt * sizeof(F1SetupResponseIEs_t *));
         CU_FREE(f1apMsg->choice.successfulOutcome,\
               sizeof(SuccessfulOutcome_t));
         CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   strcpy((char*)cuName->buf, (char*)cuCfgParams.cuName);
 
   /*Cells to be activated list*/
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = \
                              ProtocolIE_ID_id_Cells_to_be_Activated_List ;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
                     F1SetupResponseIEs__value_PR_Cells_to_be_Activated_List;
   cellToActivate = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.\
                    Cells_to_be_Activated_List;
   cellCnt=1;
   cellToActivate->list.count = cellCnt;
   cellToActivate->list.size = \
               cellCnt*sizeof(struct Cells_to_be_Activated_List_ItemIEs  *);
   CU_ALLOC(cellToActivate->list.array,\
         sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
   if(cellToActivate->list.array == NULLP)
   {
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idy++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy],\
               sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   for(idy=0; idy<cellCnt; idy++)
   {
      CU_ALLOC(cellToActivate->list.array[idy],\
            sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      if(cellToActivate->list.array[idy] == NULLP)
      {
         CU_FREE(cellToActivate->list.array,\
               sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
         CU_FREE(cuName->buf, sizeof(cuName->size));
         for(idy=0; idy<elementCnt; idy++)
         {
            CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
                  sizeof(F1SetupResponseIEs_t));
         }
         CU_FREE(f1SetupRsp->protocolIEs.list.array, \
               elementCnt * sizeof(F1SetupResponseIEs_t *));
         CU_FREE(f1apMsg->choice.successfulOutcome, \
               sizeof(SuccessfulOutcome_t));
         CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }
   cellToActivate->list.array[0]->id = \
                              ProtocolIE_ID_id_Cells_to_be_Activated_List_Item;
   cellToActivate->list.array[0]->criticality = Criticality_ignore;
   cellToActivate->list.array[0]->value.present = \
       Cells_to_be_Activated_List_ItemIEs__value_PR_Cells_to_be_Activated_List_Item;
   cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
      nRCGI.pLMN_Identity.size = 3*sizeof(uint8_t);
   CU_ALLOC(cellToActivate->list.array[0]->\
         value.choice.Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf,\
         3*sizeof(uint8_t));
   if(cellToActivate->list.array[0]->value.choice.\
         Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf == NULLP)
   {

      for(idy=0; idy<cellCnt; idy++)
      {
         CU_FREE(cellToActivate->list.array[idy],\
               sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
            sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idy++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
               sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
            sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
    buildPlmnId(cuCfgParams.plmn , &cellToActivate->list.array[0]->value.choice.\
         Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity);
   cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
      nRCGI.nRCellIdentity.size = 5;
   CU_ALLOC(cellToActivate->list.array[0]->value.choice.\
         Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf,\
         5*sizeof(uint8_t));
   if(cellToActivate->list.array[0]->value.choice.\
       Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf == NULLP)
   {
      CU_FREE(cellToActivate->list.array[0]->\
          value.choice.Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf,\
          3*sizeof(uint8_t));
      for(idy=0; idy<cellCnt; idy++)
      {
         CU_FREE(cellToActivate->list.array[idy],\
               sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
            sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idy++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
               sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
            sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   BuildNrCellId(&(cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity));
   /* RRC Version */
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = \
                                        ProtocolIE_ID_id_GNB_CU_RRC_Version;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                    F1SetupResponseIEs__value_PR_RRC_Version;
   rrcVer = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.RRC_Version;
   rrcVer->latest_RRC_Version.size = RRC_SIZE; 

   CU_ALLOC(rrcVer->latest_RRC_Version.buf, sizeof(U8));
   if(rrcVer->latest_RRC_Version.buf == NULLP)
   {  
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }

  /* Need to check RRC Version */
   rrcVer->latest_RRC_Version.buf[0] = cuCfgParams.rrcVersion.rrcVer; 
   rrcVer->latest_RRC_Version.bits_unused = 5; //TODO: pick from cuCfgParam. If not present, add it
   CU_ALLOC(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
   if(rrcVer->iE_Extensions == NULLP)
   {
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(U8));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idy++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   rrcVer->iE_Extensions->list.count = 1;
   rrcVer->iE_Extensions->list.size = sizeof(struct RRC_Version_ExtIEs *);
   CU_ALLOC(rrcVer->iE_Extensions->list.array,\
         sizeof(struct RRC_Version_ExtIEs *));
   if(rrcVer->iE_Extensions->list.array == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions,\
            sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(U8));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idy++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   CU_ALLOC(rrcVer->iE_Extensions->list.array[0],\
         sizeof(struct RRC_Version_ExtIEs));
   if(rrcVer->iE_Extensions->list.array[0] == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array,\
            sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
            sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(U8));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idy++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   rrcVer->iE_Extensions->list.array[0]->id = \
                                ProtocolIE_ID_id_latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->criticality = Criticality_reject;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.present = \
             RRC_Version_ExtIEs__extensionValue_PR_Latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.size = 3*sizeof(U8);
   CU_ALLOC(rrcVer->iE_Extensions->list.\
         array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf,\
         3*sizeof(U8));
   if(rrcVer->iE_Extensions->list.\
      array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array[0],\
             sizeof(struct RRC_Version_ExtIEs));
      CU_FREE(rrcVer->iE_Extensions->list.array,\
            sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
            sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(U8));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(idy=0; idy<elementCnt; idy++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[idy], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[0] = 0;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[1] = 5;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[2] = 15;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(U8));
   CU_FREE(cuName->buf, sizeof(cuName->size));
   for(idx=0; idx<elementCnt; idx++)
   {
      CU_FREE(f1SetupRsp->protocolIEs.list.array[idx], sizeof(F1SetupResponseIEs_t));
   }             
   CU_FREE(f1SetupRsp->protocolIEs.list.array, elementCnt * sizeof(F1SetupResponseIEs_t *));
   CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));

   /* Check encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
	   DU_LOG("\nF1AP : Could not encode F1SetupResponse structure (at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	   RETVALUE(RFAILED);   
   } 
   else 
   {
	   DU_LOG("\nF1AP : Created APER encoded buffer for F1SetupResponse\n");
	   for(int i=0; i< encBufSize; i++)
	   {
		   printf("%x",encBuf[i]);
	   } 
   }

   /* Sending msg */
   if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
   {
	   DU_LOG("\nF1AP : Sending F1 Setup Response failed");      
	   RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of BuildAndSendF1SetupRsp */

/*******************************************************************
 *
 * @brief Builds and sends the DUUpdateAcknowledge
 *
 * @details
 *
 *    Function : BuildAndSendDUUpdateAck
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

S16 BuildAndSendDUUpdateAck()
{
   U8   idx;
   U8   elementCnt;
   F1AP_PDU_t *f1apMsg = NULL;
   GNBDUConfigurationUpdateAcknowledge_t *gNBDuCfgAck;
   asn_enc_rval_t enRetVal; /* Encoder return value */

   DU_LOG("\nF1AP : Building GNB-DU Config Update Ack\n");

   /* Allocate the memory for F1SetupRequest_t */
   CU_ALLOC(f1apMsg, (Size)sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      RETVALUE(RFAILED);
   }

   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_gNBDUConfigurationUpdate;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge;
   gNBDuCfgAck = &f1apMsg->choice.successfulOutcome->value.choice.GNBDUConfigurationUpdateAcknowledge;

   elementCnt = 1;
   gNBDuCfgAck->protocolIEs.list.count = elementCnt;
   gNBDuCfgAck->protocolIEs.list.size = elementCnt*sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t);

   /* Initialize the F1Setup members */
   CU_ALLOC(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   if(gNBDuCfgAck->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for DuUpdateAcknowledgeIEs failed");
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
      RETVALUE(RFAILED);
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(gNBDuCfgAck->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
      if(gNBDuCfgAck->protocolIEs.list.array[idx] == NULLP)
      {
         CU_FREE(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
         CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
         CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }

   /*TransactionID*/ 
   idx = 0;
   gNBDuCfgAck->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID ;
   gNBDuCfgAck->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   gNBDuCfgAck->protocolIEs.list.array[idx]->value.present = GNBDUConfigurationUpdateAcknowledgeIEs__value_PR_TransactionID;
   gNBDuCfgAck->protocolIEs.list.array[idx]->value.choice.TransactionID = TRANS_ID;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);
 
   /* Encode the F1SetupRequest type as UPER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   enRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   for(idx=0; idx<elementCnt; idx++)
   {
      CU_FREE(gNBDuCfgAck->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
   }
   CU_FREE(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));

   /* Checking encode results */
   if(enRetVal.encoded == ENCODE_FAIL) 
   {
      DU_LOG("\nF1AP : Could not encode DUConfigUpdateAcknowledge structure (at %s)",enRetVal.failed_type ? enRetVal.failed_type->name : "unknown");
      RETVALUE(RFAILED); 
   } 
   else 
   {
    DU_LOG("\nF1AP : Created APER encoded buffer for DuConfigUpdateAcknowledge\n");  
      for(int i=0; i< encBufSize; i++)
      {
         printf("%x",encBuf[i]);
      } 
   }

   /* Sending msg */
   if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
   {
      DU_LOG("\nF1AP : Sending GNB-DU Config Update Ack failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}/* End of BuildAndSendDUUpdateAck*/

/*******************************************************************
 *
 * @brief Fills Radio Bearer Config 
 *
 * @details
 *
 *    Function : fillRadioBearerConfig
 *
 *    Functionality: Fills Radio Bearer Config
 *
 * @params[in] SRB_ToAddModList *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRadioBearerConfig(SRB_ToAddModList_t *bearerCfg)
{
   uint8_t elementCnt;
   uint8_t idx;
   uint8_t ied;
   if(bearerCfg != NULLP)
   {
      elementCnt = 1;
      bearerCfg->list.count = elementCnt;
      bearerCfg->list.size =\
           elementCnt * sizeof(SRB_ToAddMod_t);
      CU_ALLOC(bearerCfg->list.array, sizeof(bearerCfg->list.size));
      if(bearerCfg->list.array != NULLP)
      {
         for(idx = 0; idx < elementCnt; idx++)
         {
            CU_ALLOC(bearerCfg->list.array[idx], sizeof(SRB_ToAddMod_t));
            if(bearerCfg->list.array[idx] == NULLP)
            {
               for(ied = 0; ied < idx; ied++)
               {
                  CU_FREE(bearerCfg->list.array[idx], sizeof(SRB_ToAddMod_t));
               }
               CU_FREE(bearerCfg->list.array, sizeof(bearerCfg->list.size));
               return RFAILED;
            }
         }
      }
		else
		{
		   return RFAILED;
		}
      idx = 0;
      bearerCfg->list.array[idx]->srb_Identity = SRB1;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Master CellGroup Info 
 *
 * @details
 *
 *    Function : fillMasterCellGroup
 *
 *    Functionality: Fills Master Cell Group IE
 *
 * @params[in] RRCSetup_IEs_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillMasterCellGroup(OCTET_STRING_t *masterCellGroup)
{
   masterCellGroup->buf = NULLP;
	if(f1apMsgDb.duToCuContainer.buf)
	{
      masterCellGroup->size = f1apMsgDb.duToCuContainer.size;
      CU_ALLOC(masterCellGroup->buf, masterCellGroup->size);
      if(masterCellGroup->buf != NULLP)
      {
         memcpy(masterCellGroup->buf, f1apMsgDb.duToCuContainer.buf,\
			  masterCellGroup->size);
      }
	   else
	   {
	      return RFAILED;
	   }
	}
	else
	{
	   return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills RRC setup IE 
 *
 * @details
 *
 *    Function : fillRRCSetupIE
 *
 *    Functionality: Fills RRC Setup IE
 *
 * @params[in] RRCSetup_IEs_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillRRCSetupIE(RRCSetup_IEs_t *rrcSetupIE)
{
   uint8_t ret = ROK;
   if(rrcSetupIE)
   {
      CU_ALLOC(rrcSetupIE->radioBearerConfig.srb_ToAddModList, sizeof(SRB_ToAddModList_t));
		if(rrcSetupIE->radioBearerConfig.srb_ToAddModList != NULLP)
		{
         ret = fillRadioBearerConfig(rrcSetupIE->radioBearerConfig.srb_ToAddModList);
	   }		
      if(!ret)
      {
         ret = fillMasterCellGroup(&rrcSetupIE->masterCellGroup);
      }
		else
		{
		   CU_FREE(rrcSetupIE->radioBearerConfig.srb_ToAddModList, sizeof(SRB_ToAddModList_t));
			ret = RFAILED;
	   }
   }
   return ret;
}
/*******************************************************************
 *
 * @brief Builds RRC Container IE required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : BuildDLRRCContainer
 *
 *    Functionality: Builds RRC Container IE required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t	BuildDLRRCContainer(RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;
	uint16_t idx2;
   DL_CCCH_Message_t dl_CCCH_Msg;
   asn_enc_rval_t        encRetVal;

   if(rrcContainer != NULLP)
   {
      dl_CCCH_Msg.message.present = DL_CCCH_MessageType_PR_c1;

      CU_ALLOC(dl_CCCH_Msg.message.choice.c1 , sizeof(DL_CCCH_MessageType_t));
      if(dl_CCCH_Msg.message.choice.c1 != NULLP)
      {
         dl_CCCH_Msg.message.choice.c1->present = DL_CCCH_MessageType__c1_PR_rrcSetup;
         CU_ALLOC(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup, sizeof(RRCSetup_t));
         if(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup != NULLP)
         {
            dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->rrc_TransactionIdentifier = 0;
            dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->criticalExtensions.\
              present = RRCSetup__criticalExtensions_PR_rrcSetup;
				/* Fill RRC Setup IE */
            CU_ALLOC(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->\
              criticalExtensions.choice.rrcSetup, sizeof(RRCSetup_IEs_t));
            if(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->\
                criticalExtensions.choice.rrcSetup != NULLP)
            {
               ret = fillRRCSetupIE(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->\
               criticalExtensions.choice.rrcSetup);

               if(!ret)
					{
                  /* encode DL-CCCH message into RRC Container */
                  xer_fprint(stdout, &asn_DEF_DL_CCCH_MessageType, &dl_CCCH_Msg);
                  cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
                  encBufSize = 0;
                  encRetVal = aper_encode(&asn_DEF_DL_CCCH_MessageType, 0, &dl_CCCH_Msg, PrepFinalEncBuf, encBuf);
                  /* Encode results */
                  if(encRetVal.encoded == ENCODE_FAIL)
                  {
                     DU_LOG( "\n F1AP : Could not encode RRCContainer (at %s)\n",\
                  	  encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
                  	  return RFAILED;
                  }
                  else
                  {
                     DU_LOG("\n F1AP : Created APER encoded buffer for RRCContainer\n");
                  	  for(int i = 0; i< encBufSize; i++)
                  	  {
                  	     printf("%x",encBuf[i]);
                  	  }
                  }
                  rrcContainer->size = encBufSize;
                  CU_ALLOC(rrcContainer->buf, rrcContainer->size);
                  if(rrcContainer->buf != NULLP)
                  {
                     memset(rrcContainer->buf, 0, encBufSize);
	                  for(idx2 = 0; idx2 < encBufSize; idx2++)
	                  {
	                     rrcContainer->buf[idx2] =	encBuf[idx2];
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
 * @brief Builds and sends the DLRRCMessageTransfer 
 *
 * @details
 *
 *    Function : BuildAndSendDLRRCMessageTransfer
 *
 *    Functionality: Constructs the DL RRC Message Transfer and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildAndSendDLRRCMessageTransfer()
{
   uint8_t ret = ROK ;
   uint8_t   elementCnt = 0;
   uint8_t  ieId;
   uint8_t  idx;
	uint16_t idx2;
   F1AP_PDU_t  *f1apMsg = NULLP;
   DLRRCMessageTransfer_t  *dlRRCMsg = NULLP;
   asn_enc_rval_t   encRetVal;        /* Encoder return value */

   DU_LOG("\n F1AP : Building DL RRC Message Transfer Message\n");

   CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }

   f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
   CU_ALLOC(f1apMsg->choice.initiatingMessage,
      sizeof(InitiatingMessage_t));
   if(f1apMsg->choice.initiatingMessage == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for  F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_DLRRCMessageTransfer;
   f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
   f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_DLRRCMessageTransfer;
   dlRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;

   elementCnt = 4;
   dlRRCMsg->protocolIEs.list.count = elementCnt;
   dlRRCMsg->protocolIEs.list.size = elementCnt * sizeof(DLRRCMessageTransferIEs_t);

   /* Initialize the F1Setup members */
   CU_ALLOC(dlRRCMsg->protocolIEs.list.array, dlRRCMsg->protocolIEs.list.size);
   if(dlRRCMsg->protocolIEs.list.array == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for DL RRC MessageTransferIEs failed");
      CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      CU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(dlRRCMsg->protocolIEs.list.array[idx], sizeof(DLRRCMessageTransferIEs_t));
      if(dlRRCMsg->protocolIEs.list.array[idx] == NULLP)
      {
         for(ieId=0; ieId<idx; ieId++)
         {
             CU_FREE(dlRRCMsg->protocolIEs.list.array[ieId],\
                       sizeof(DLRRCMessageTransferIEs_t));
         }
         CU_FREE(dlRRCMsg->protocolIEs.list.array,\
           dlRRCMsg->protocolIEs.list.size);
         CU_FREE(f1apMsg->choice.initiatingMessage,\
           sizeof(InitiatingMessage_t));
         CU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
         return RFAILED;
      }
   }

   /* GNB CU UE F1AP ID */
   idx = 0;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  =   Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
                         DLRRCMessageTransferIEs__value_PR_GNB_CU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = CU_ID;
 
   /* GNB DU UE F1AP ID */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = \
                                   ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  =   Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
                         DLRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = DU_ID;
 
   /* SRBID */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_SRBID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  =   Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
                                 DLRRCMessageTransferIEs__value_PR_SRBID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.SRBID = SRB1;
         
   /* RRCContainer */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_RRCContainer;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality   = Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = \
	                             DLRRCMessageTransferIEs__value_PR_RRCContainer;
    #if 0
    char rrcBuf[270] = { 
    0x20, 0x40, 0x04, 0x08, 0x4a, 0xe2, 0x05, 0x80, 0x08, 0x80, 0x57, 0x64, 0x0e, 0x83,
	 0x0f, 0x50, 0x03, 0xe1, 0x15, 0x04, 0xa8, 0x3d, 0xc6, 0x01, 0x03, 0xd4, 0xc0, 0x4b,
	 0x30, 0x32, 0xa0, 0x16, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0x40, 0x01, 0x00,
	 0x0b, 0x1d, 0xc2, 0x10, 0x80, 0x03, 0xf8, 0x8c, 0xdc, 0x31, 0x08, 0x00, 0x3f, 0x88,
	 0xd9, 0x48, 0x10, 0x12, 0x00, 0x10, 0x03, 0x00, 0x81, 0x00, 0x58, 0x04, 0x19, 0xb8,
	 0xd4, 0x6a, 0x35, 0x1a, 0x99, 0x0c, 0x86, 0x43, 0x20, 0x00, 0x99, 0x8a, 0x92, 0x44,
	 0x0e, 0x14, 0x30, 0x71, 0x02, 0x45, 0x0b, 0x19, 0x4e, 0x54, 0xb1, 0x73, 0x06, 0x4d,
	 0x1b, 0x38, 0x29, 0x80, 0x10, 0x80, 0x88, 0x00, 0x08, 0x01, 0xa0, 0x50, 0x50, 0x06,
	 0x81, 0x81, 0x40, 0xda, 0x07, 0x05, 0x80, 0x68, 0x20, 0x16, 0x0d, 0xa0, 0x90, 0x60,
	 0x06, 0x82, 0x81, 0x80, 0xda, 0x0b, 0x06, 0x80, 0x68, 0x30, 0x1a, 0x0d, 0xa1, 0x50,
	 0x92, 0x26, 0x85, 0x82, 0xa8, 0x9a, 0x17, 0x0c, 0x22, 0x68, 0x60, 0x36, 0x89, 0xa1,
	 0x90, 0xf2, 0x26, 0x86, 0x84, 0x28, 0x9a, 0x1b, 0x12, 0x22, 0x68, 0x70, 0x4e, 0x89,
	 0xa0, 0x40, 0x32, 0x36, 0xc8, 0xc3, 0x01, 0xe4, 0x07, 0x45, 0x67, 0x89, 0x44, 0xaa,
	 0x44, 0x00, 0x5c, 0x90, 0x80, 0x02, 0xfe, 0xc1, 0x94, 0x0c, 0x10, 0x00, 0x01, 0x9c,
	 0x52, 0x98, 0xc5, 0x31, 0xca, 0x64, 0x14, 0xc6, 0x00, 0x40, 0x70, 0x22, 0xb5, 0x01,
	 0x40, 0x05, 0xc0, 0x1e, 0x02, 0x20, 0x80, 0x00, 0xa1, 0x01, 0xff, 0x40, 0x00, 0x1c,
	 0x05, 0xe0, 0x22, 0x08, 0x00, 0x0a, 0x10, 0x1f, 0xf4, 0x02, 0x04, 0x06, 0x00, 0x10,
	 0x00, 0x18, 0x00, 0x00, 0x80, 0x01, 0x03, 0x12, 0x00, 0x01, 0x00, 0x60, 0x09, 0x70,
	 0x68, 0x01, 0x01, 0x00, 0x8d, 0x02, 0x00, 0x80, 0x40, 0x04, 0x38, 0x01, 0xe0, 0x6c,
	 0x00, 0x20, 0x20, 0x00};
   uint16_t rrcBufLen = 270;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size = rrcBufLen;
   CU_ALLOC(dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,\
            rrcBufLen);
	memset(dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf, 0,\
	rrcBufLen);
	for(idx2 = 0; idx2 < rrcBufLen; idx2++)
	{
	   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf[idx2] =\
		rrcBuf[idx2];
   }
	#endif
   BuildDLRRCContainer(&dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer);
 
   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);
 
   /* Encode the F1SetupRequest type as APER */
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
       encBuf);
   /* Encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
     DU_LOG( "\n F1AP : Could not encode DL RRC Message Transfer structure (at %s)\n",\
         encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
     RETVALUE(RFAILED);
   }
   else
   {
     DU_LOG("\n F1AP : Created APER encoded buffer for DL RRC Message transfer\n");
     for(int i=0; i< encBufSize; i++)
     {
       printf("%x",encBuf[i]);
     }
   }
 
   /* Sending  msg  */
   if(SendF1APMsg(CU_APP_MEM_REG,CU_POOL)  !=  ROK)
   {
     DU_LOG("\n F1AP : Sending  DL RRC Message Transfer Failed");
     return RFAILED;
   }
 
   return ROK;
}/* End of BuildAndSendDLRRCMessageTransfer */


/*******************************************************************
 *
 * @brief Builds and sends the UE Setup Response
 *
 * @details
 *
 *    Function : BuildAndSendUESetRsp
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
S16 BuildAndSendUESetRsp()
{
	S16  ret;
	U8   elementCnt;
	U8   cellCnt;
	U8   ieId;
	U8   idx;
	U8   drbCnt;
	U8   drbId;
	F1AP_PDU_t      						*f1apMsg = NULL;
   UEContextSetupResponse_t			*ueSetRsp;
	asn_enc_rval_t  						encRetVal;        /* Encoder return value */

	DU_LOG("\n F1AP : Building UE Context Setup Response\n");

	CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
	if(f1apMsg == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
		RETVALUE(RFAILED);
	}

	f1apMsg->present = F1AP_PDU_PR_successfulOutcome;
	CU_ALLOC(f1apMsg->choice.successfulOutcome,
			sizeof(SuccessfulOutcome_t));
	if(f1apMsg->choice.successfulOutcome == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
		CU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	f1apMsg->choice.successfulOutcome->procedureCode = \
													ProcedureCode_id_UEContextSetup;
	f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
	f1apMsg->choice.successfulOutcome->value.present = \
		      		   SuccessfulOutcome__value_PR_UEContextSetupResponse;
	ueSetRsp =
		&f1apMsg->choice.successfulOutcome->value.choice.UEContextSetupResponse;
	elementCnt = 2;
	ueSetRsp->protocolIEs.list.count = elementCnt;
	ueSetRsp->protocolIEs.list.size = \
									elementCnt * sizeof(UEContextSetupResponse_t *);

	/* Initialize the UESetup members */
	CU_ALLOC(ueSetRsp->protocolIEs.list.array, \
				ueSetRsp->protocolIEs.list.size);
	if(ueSetRsp->protocolIEs.list.array == NULLP)
	{
		DU_LOG(" F1AP : Memory allocation for UE Setup Response failed");
		CU_FREE(f1apMsg->choice.successfulOutcome,
				sizeof(SuccessfulOutcome_t));
		CU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
		RETVALUE(RFAILED);
	}

	for(idx=0; idx<elementCnt; idx++)
	{
		CU_ALLOC(ueSetRsp->protocolIEs.list.array[idx],\
										sizeof(UEContextSetupResponseIEs_t));
		if(ueSetRsp->protocolIEs.list.array[idx] == NULLP)
		{
			for(ieId=0; ieId<idx; ieId++)
			{
				CU_FREE(ueSetRsp->protocolIEs.list.array[ieId],\
						sizeof(UEContextSetupResponseIEs_t));
			}
			CU_FREE(ueSetRsp->protocolIEs.list.array,\
						 ueSetRsp->protocolIEs.list.size);
			CU_FREE(f1apMsg->choice.successfulOutcome,\
												sizeof(SuccessfulOutcome_t));
			CU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
			RETVALUE(RFAILED);
		}
	}

	idx = 0;

	/*GNB CU UE F1AP ID*/
	ueSetRsp->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
	ueSetRsp->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetRsp->protocolIEs.list.array[idx]->value.present = \
		                 		UEContextSetupResponseIEs__value_PR_GNB_CU_UE_F1AP_ID;
	ueSetRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = CU_ID;
	
	/*GNB DU UE F1AP ID*/
	idx++;
	ueSetRsp->protocolIEs.list.array[idx]->id	= \
					 			                 ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
	ueSetRsp->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
	ueSetRsp->protocolIEs.list.array[idx]->value.present = \
		                 		UEContextSetupResponseIEs__value_PR_GNB_DU_UE_F1AP_ID;
	ueSetRsp->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = DU_ID;
	

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
	if(SendF1APMsg(CU_APP_MEM_REG,CU_POOL)	!=	ROK)
	{
		DU_LOG("\n F1AP : Sending	UE Context Setup Request Failed");
		RETVALUE(RFAILED);
	}

	RETVALUE(ROK);
}/* End of BuildAndSendUESetRsp */


uint8_t procInitULRRCMsg(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx;
   uint8_t ret =ROK;
   InitialULRRCMessageTransfer_t *initULRRCMsg = NULLP;
   DU_LOG("\n filling the required values in DB in procInitULRRCMsg");

   initULRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.InitialULRRCMessageTransfer;

   for(idx=0; idx < initULRRCMsg->protocolIEs.list.count; idx++)
   {
      switch(initULRRCMsg->protocolIEs.list.array[idx]->id)
      {
         case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
         break;
         case ProtocolIE_ID_id_NRCGI:
         break;
         case ProtocolIE_ID_id_C_RNTI:
         break;
         case ProtocolIE_ID_id_RRCContainer:
         break;
         case ProtocolIE_ID_id_DUtoCURRCContainer:
			   {
				   if((initULRRCMsg->protocolIEs.list.array[idx]->value.choice\
					   .DUtoCURRCContainer.size > 0) && (initULRRCMsg->protocolIEs\
						.list.array[idx]->value.choice.DUtoCURRCContainer.buf != NULLP))
               {
                  DU_LOG("\n Received Du to Cu RRC Container ");
                  f1apMsgDb.duToCuContainer.size = initULRRCMsg->protocolIEs\
                   .list.array[idx]->value.choice.DUtoCURRCContainer.size;
                  CU_ALLOC(f1apMsgDb.duToCuContainer.buf, \
						 f1apMsgDb.duToCuContainer.size);
                  if(f1apMsgDb.duToCuContainer.buf != NULLP)
                  { 
                     memcpy(f1apMsgDb.duToCuContainer.buf, initULRRCMsg->protocolIEs\
                      .list.array[idx]->value.choice.DUtoCURRCContainer.buf, f1apMsgDb\
                      .duToCuContainer.size);
                  }
               }
				   else
				   {
                  DU_LOG("\n Failed to receive Du to Cu RRC Container ");
				   	ret = RFAILED;
			      }
               break;
			   }
         default:
            DU_LOG("\n Invalid Event %ld", initULRRCMsg->protocolIEs.list.array[idx]->id);
            break;
      }
   }
	if(!ret)
	   ret = BuildAndSendDLRRCMessageTransfer();
   return ret;
}
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
   F1AP_PDU_t *f1apMsg = NULLP;
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
      case F1AP_PDU_PR_initiatingMessage:
      {
         switch(f1apMsg->choice.initiatingMessage->value.present)
         {
            case InitiatingMessage__value_PR_F1SetupRequest:
            {
               DU_LOG("\nF1AP : F1 setup request received");
               BuildAndSendF1SetupRsp();
	       break;
            }

            case InitiatingMessage__value_PR_GNBDUConfigurationUpdate:
            {
               DU_LOG("\nF1AP : GNB-DU config update received");
               BuildAndSendDUUpdateAck();
               break;
            }
            case InitiatingMessage__value_PR_InitialULRRCMessageTransfer:
            {
               DU_LOG("\nF1AP : Received InitialULRRCMessageTransfer");
               procInitULRRCMsg(f1apMsg);
               break;
            }
            default:
            {
               DU_LOG("\nF1AP : Invalid type of intiating message [%d]",f1apMsg->choice.initiatingMessage->value.present);
               return;
            }
         }/* End of switch(initiatingMessage) */
         break;
      }

   }/* End of switch(f1apMsg->present) */
 
} /* End of F1APMsgHdlr */
 
/**********************************************************************
  End of file
 **********************************************************************/
