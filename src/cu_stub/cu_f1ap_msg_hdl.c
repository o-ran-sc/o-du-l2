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
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "odu_common_codec.h"
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

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         ODU_PRINT_MSG(mBuf, 0,0);
 
         if(sctpSend(mBuf) != ROK)
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
   uint8_t tmp;
   for (tmp = 0 ; tmp < nrcell->size-1; tmp++)
   {
      nrcell->buf[tmp] = 0;
   }
   nrcell->buf[4]   = 16; 
   nrcell->bits_unused = 4;
   nrcell->size = 5 * sizeof(uint8_t);
   return ROK;
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
   uint8_t    idx,ieIdx;
   uint8_t    elementCnt,cellCnt;
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
      return RFAILED;
   }
   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;  
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
      return RFAILED;
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
	 return RFAILED;
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
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx],\
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome,\
	    sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
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
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx],\
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
   {
      CU_ALLOC(cellToActivate->list.array[ieIdx],sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      if(cellToActivate->list.array[ieIdx] == NULLP)
      {
	 CU_FREE(cellToActivate->list.array,\
	       sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
	 CU_FREE(cuName->buf, sizeof(cuName->size));
	 for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
	 {
	    CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
		  sizeof(F1SetupResponseIEs_t));
	 }
	 CU_FREE(f1SetupRsp->protocolIEs.list.array, \
	       elementCnt * sizeof(F1SetupResponseIEs_t *));
	 CU_FREE(f1apMsg->choice.successfulOutcome, \
	       sizeof(SuccessfulOutcome_t));
	 CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
	 return RFAILED;
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

      for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
      {
	 CU_FREE(cellToActivate->list.array[ieIdx],\
	       sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
	    sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
	    sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   buildPlmnId(cuCfgParams.plmn , cellToActivate->list.array[0]->value.choice.\
	 Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf);
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
      for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
      {
	 CU_FREE(cellToActivate->list.array[ieIdx],\
	       sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
	    sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
	    sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
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

   CU_ALLOC(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
   if(rrcVer->latest_RRC_Version.buf == NULLP)
   {  
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; idx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   /* Need to check RRC Version */
   rrcVer->latest_RRC_Version.buf[0] = cuCfgParams.rrcVersion.rrcVer; 
   rrcVer->latest_RRC_Version.bits_unused = 5; //TODO: pick from cuCfgParam. If not present, add it
   CU_ALLOC(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
   if(rrcVer->iE_Extensions == NULLP)
   {
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.count = 1;
   rrcVer->iE_Extensions->list.size = sizeof(struct RRC_Version_ExtIEs *);
   CU_ALLOC(rrcVer->iE_Extensions->list.array,\
	 sizeof(struct RRC_Version_ExtIEs *));
   if(rrcVer->iE_Extensions->list.array == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions,\
	    sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   CU_ALLOC(rrcVer->iE_Extensions->list.array[0],\
	 sizeof(struct RRC_Version_ExtIEs));
   if(rrcVer->iE_Extensions->list.array[0] == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array,\
	    sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
	    sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.array[0]->id = \
					      ProtocolIE_ID_id_latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->criticality = Criticality_reject;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.present = \
								  RRC_Version_ExtIEs__extensionValue_PR_Latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.size = 3*sizeof(uint8_t);
   CU_ALLOC(rrcVer->iE_Extensions->list.\
	 array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf,\
	 3*sizeof(uint8_t));
   if(rrcVer->iE_Extensions->list.\
	 array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array[0],\
	    sizeof(struct RRC_Version_ExtIEs));
      CU_FREE(rrcVer->iE_Extensions->list.array,\
	    sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
	    sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
	       sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
	    elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[0] = 0;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[1] = 5;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[2] = 15;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
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
      return RFAILED;   
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
      return RFAILED;
   }

   return ROK;
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
   uint8_t   idx;
   uint8_t   elementCnt;
   F1AP_PDU_t *f1apMsg = NULL;
   GNBDUConfigurationUpdateAcknowledge_t *gNBDuCfgAck;
   asn_enc_rval_t enRetVal; /* Encoder return value */

   DU_LOG("\nF1AP : Building GNB-DU Config Update Ack\n");

   /* Allocate the memory for F1SetupRequest_t */
   CU_ALLOC(f1apMsg, (Size)sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }

   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
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
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(gNBDuCfgAck->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
      if(gNBDuCfgAck->protocolIEs.list.array[idx] == NULLP)
      {
	 CU_FREE(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
	 CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
	 CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
	 return RFAILED;
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
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
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
      return RFAILED; 
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
      return RFAILED;
   }

   return ROK;

}/* End of BuildAndSendDUUpdateAck*/
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
    uint8_t idx = 0;
    Reset_t *f1ResetMsg = NULLP;

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
                    CU_FREE(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
                 }
             }
             CU_FREE(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
          }
          CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
       }
       CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
    }
}
/*******************************************************************
*
* @brief build ansld ans send f1reset msg 
*
* @details
*
*    Function : BuildAndSendF1ResetReq
*
*    Functionality: build and send f1reset msg 
*
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
       CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
       if(f1apMsg == NULLP)
       {
          DU_LOG("\nF1AP : Memory allocation for the BuildAndSendF1ResetReq's F1AP-PDU failed"); 
	  break;
       }
       f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
       CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
       if(f1apMsg->choice.initiatingMessage == NULLP)
       {
          DU_LOG("\nF1AP : Memory allocation for BuildAndSendF1ResetReq failed");
          break;
       }
       f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_Reset;
       f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
       f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_Reset;

       f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

       elementCnt = 3;
       f1ResetMsg->protocolIEs.list.count = elementCnt;
       f1ResetMsg->protocolIEs.list.size = elementCnt * sizeof(ResetIEs_t *);

       /* Initialize the F1Reset members */
       CU_ALLOC(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
       if(f1ResetMsg->protocolIEs.list.array == NULLP)
       {
          DU_LOG("\nF1AP : Memory allocation failed for BuildAndSendF1ResetReq");
	  break;
       }
       for(idx=0; idx<elementCnt; idx++)
       {
           CU_ALLOC(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
           if(f1ResetMsg->protocolIEs.list.array[idx] == NULLP)
           {
	      DU_LOG("\nF1AP : Memory allocation failed for BuildAndSendF1ResetReq msg array");
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

       if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
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
 * @brief Fills Radio Bearer Config 
 *
 * @details
 *
 *    Function : fillSrbCfg
 *
 *    Functionality: Fills Radio Bearer Config
 *
 * @params[in] SRB_ToAddModList *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillSrbCfg(uint8_t srbId, SRB_ToAddModList_t *bearerCfg)
{
   uint8_t elementCnt;
   uint8_t idx, ieId;
   if(bearerCfg != NULLP)
   {
      elementCnt = 1;
      bearerCfg->list.count = elementCnt;
      bearerCfg->list.size =\
           elementCnt * sizeof(SRB_ToAddMod_t *);
      CU_ALLOC(bearerCfg->list.array, bearerCfg->list.size);
      if(bearerCfg->list.array != NULLP)
      {
         for(idx = 0; idx < elementCnt; idx++)
         {
            CU_ALLOC(bearerCfg->list.array[idx], sizeof(SRB_ToAddMod_t));
            if(bearerCfg->list.array[idx] == NULLP)
            {
               for(ieId = 0; ieId < idx; ieId++)
               {
                  CU_FREE(bearerCfg->list.array[ieId], sizeof(SRB_ToAddMod_t));
               }
               CU_FREE(bearerCfg->list.array, bearerCfg->list.size);
               return RFAILED;
            }
         }
      }
      else
      {
         return RFAILED;
      }
      idx = 0;
      bearerCfg->list.array[idx]->srb_Identity = srbId;
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
   uint8_t ret = ROK;
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
	 ret = RFAILED;
      }
   }
   else
   {
      ret =  RFAILED;
   }
   return ret;
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
	 ret = fillSrbCfg(SRB1, rrcSetupIE->radioBearerConfig.srb_ToAddModList);
      }		
      if(ret == ROK)
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
 * @brief Fills DL DCCCH Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillDlCcchRrcMsg
 *
 *    Functionality: Fills DL DCCCH Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCContainer_t *rrcContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlCcchRrcMsg(RRCContainer_t *rrcContainer)
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

	       if(ret == ROK)
	       {
		  /* encode DL-CCCH message into RRC Container */
		  xer_fprint(stdout, &asn_DEF_DL_CCCH_MessageType, &dl_CCCH_Msg);
		  memset(encBuf, 0, ENC_BUF_MAX_LEN);
		  encBufSize = 0;
		  encRetVal = aper_encode(&asn_DEF_DL_CCCH_MessageType, 0, &dl_CCCH_Msg, PrepFinalEncBuf, encBuf);
		  /* Encode results */
		  if(encRetVal.encoded == ENCODE_FAIL)
		  {
		     DU_LOG( "\n F1AP : Could not encode RRCContainer for DL-CCCH Msg(at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		     return RFAILED;
		  }
		  else
		  {
		     DU_LOG("\n F1AP : Created APER encoded buffer for RRCContainer for DL-CCCH Msg\n");
		     for(int i = 0; i< encBufSize; i++)
		     {
			printf("%x",encBuf[i]);
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
	       else
	       {
		  ret = RFAILED;
	       }
	    }
	    else
	    {
	       DU_LOG("\nF1AP: Memory Alloc failed for RRC Setup Msg at fillDlCcchRrcMsg()");
	       ret = RFAILED;
	    }
	 }
	 else
	 {
	    DU_LOG("\nF1AP: Memory Alloc failed for RRC Msg at fillDlCcchRrcMsg()");
	    ret = RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nF1AP: Memory Alloc failed for DL Ccch Msg choice at fillDlCcchRrcMsg()");
	 ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nF1AP: RRC Container is NULLP at fillDlCcchRrcMsg()");
      ret = RFAILED;
   }
}

uint8_t fillQosFlowsToAdd(struct SDAP_Config__mappedQoS_FlowsToAdd *qosFlow)
{
   uint8_t idx, ied, elementCnt;

   elementCnt = 1;
   qosFlow->list.count = elementCnt;
   qosFlow->list.size  = elementCnt * sizeof(QFI_t *);
   CU_ALLOC(qosFlow->list.array, qosFlow->list.size);
   if(qosFlow->list.array != NULLP)
   {
      for(idx = 0; idx < elementCnt; idx++)
      {
         CU_ALLOC(qosFlow->list.array[idx], sizeof(QFI_t));
         if(qosFlow->list.array[idx] == NULLP)
         {
            for(ied = 0; ied < idx; ied++)
            {
               CU_FREE(qosFlow->list.array[idx], sizeof(QFI_t));
            }
            CU_FREE(qosFlow->list.array, qosFlow->list.size);
            return RFAILED;
         }
      }
   }
   idx = 0;
   *qosFlow->list.array[idx] = 9;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills CN Assoc for Drb to Add/Mod List
 *
 * @details
 *
 *    Function : fillCnAssoc
 *
 *    Functionality: Fills CN Assoc for Drb to Add/Mod List
 *
 * @params[in] struct DRB_ToAddMod__cnAssociation *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillCnAssoc(struct DRB_ToAddMod__cnAssociation *cnAssoc)
{
   uint8_t ret = ROK;

   cnAssoc->present = DRB_ToAddMod__cnAssociation_PR_sdap_Config;
   if(cnAssoc->present == DRB_ToAddMod__cnAssociation_PR_eps_BearerIdentity)
   {
      cnAssoc->choice.eps_BearerIdentity = 5;
   }
   if(cnAssoc->present == DRB_ToAddMod__cnAssociation_PR_sdap_Config)
   {
      CU_ALLOC(cnAssoc->choice.sdap_Config, sizeof(SDAP_Config_t));
      if(cnAssoc->choice.sdap_Config)
      {
         cnAssoc->choice.sdap_Config->pdu_Session = 5;
         cnAssoc->choice.sdap_Config->sdap_HeaderDL = 0;
         cnAssoc->choice.sdap_Config->sdap_HeaderUL = 0;
         cnAssoc->choice.sdap_Config->defaultDRB = true;
         cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd = NULLP;
         cnAssoc->choice.sdap_Config->mappedQoS_FlowsToRelease = NULLP;
         CU_ALLOC(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd, \
            sizeof(struct SDAP_Config__mappedQoS_FlowsToAdd));
         if(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd)
         {
            ret = fillQosFlowsToAdd(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd);
         }
         else
         {
            DU_LOG("\nF1AP: Memory alloc failed at mappedQoS_FlowsToAdd in fillCnAssoc()");
            CU_FREE(cnAssoc->choice.sdap_Config, sizeof(SDAP_Config_t));
            ret = RFAILED;
         }
      }
      else
      {
         DU_LOG("\nF1AP: Mem alloc failed at fillCnAssoc()");
         ret = RFAILED;
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills Radio Bearer Config for Drb 
 *
 * @details
 *
 *    Function : fillDrbCfg
 *
 *    Functionality: Fills Radio Bearer Config for Drb
 *
 * @params[in] drbId, DRB_ToAddModList *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillDrbCfg(uint8_t drbId, DRB_ToAddModList_t *drbCfg)
{
   uint8_t idx, ied, ret, elementCnt;
   
   ret = ROK;
   if(drbCfg != NULLP)
   {
      elementCnt = 1;
      drbCfg->list.count = elementCnt;
      drbCfg->list.size =\
           elementCnt * sizeof(DRB_ToAddMod_t *);
      CU_ALLOC(drbCfg->list.array, drbCfg->list.size);
      if(drbCfg->list.array != NULLP)
      {
         for(idx = 0; idx < elementCnt; idx++)
         {
            CU_ALLOC(drbCfg->list.array[idx], sizeof(DRB_ToAddMod_t));
            if(drbCfg->list.array[idx] == NULLP)
            {
               for(ied = 0; ied < idx; ied++)
               {
                  CU_FREE(drbCfg->list.array[idx], sizeof(DRB_ToAddMod_t));
               }
               CU_FREE(drbCfg->list.array, drbCfg->list.size);
               return RFAILED;
            }
         }
      }
      else
      {
         return RFAILED;
      }
      idx = 0;
      /* CN ASSOCIATION */
      CU_ALLOC(drbCfg->list.array[idx]->cnAssociation, sizeof(struct DRB_ToAddMod__cnAssociation));
      if(drbCfg->list.array[idx]->cnAssociation)
      {
         ret = fillCnAssoc(drbCfg->list.array[idx]->cnAssociation);
      }
      /* DRB */
      drbCfg->list.array[idx]->drb_Identity = drbId;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills RRC Reconfig Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillRrcReconfigIE
 *
 *    Functionality: Fills RRC Reconfig Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCReconfiguration_IEs_t* rrcReconfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillRrcReconfigIE(RRCReconfiguration_IEs_t *rrcReconfigMsg)
{
   uint8_t ret = ROK;
   CU_ALLOC(rrcReconfigMsg->radioBearerConfig, sizeof(RadioBearerConfig_t));
   if(rrcReconfigMsg->radioBearerConfig)
   {
      CU_ALLOC(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      if(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList != NULLP)
      {
         ret = fillSrbCfg(SRB2, rrcReconfigMsg->radioBearerConfig->srb_ToAddModList);
	 
      }
      if(ret == ROK)
      {
         CU_ALLOC(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
         if(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList != NULLP)
	 {
            ret = fillDrbCfg(DRB1, rrcReconfigMsg->radioBearerConfig->drb_ToAddModList);
	    if(ret == RFAILED)
	    {
               DU_LOG("\n F1AP: Failed to fill DrbCfg at fillRrcReconfigIE()");
	       CU_FREE(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
               CU_FREE(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
	    }
	 }
      }
      else
      {
         DU_LOG("\n F1AP: memory Alloc failed at fillRrcReconfigIE()");
	 CU_FREE(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      }
   }

   return ret;
}
/*******************************************************************
 *
 * @brief Fills DL DCCH Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillDlDcchRrcMsg
 *
 *    Functionality: Fills DL DCCH Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCContainer_t *rrcContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlDcchRrcMsg(RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;
   uint16_t idx2;
   DL_DCCH_Message_t dl_DCCH_Msg;
   memset(&dl_DCCH_Msg, 0, sizeof(DL_DCCH_Message_t));
   asn_enc_rval_t        encRetVal;

   if(rrcContainer != NULLP)
   {
      dl_DCCH_Msg.message.present = DL_DCCH_MessageType_PR_c1;

      CU_ALLOC(dl_DCCH_Msg.message.choice.c1 , sizeof(DL_DCCH_MessageType_t));
      if(dl_DCCH_Msg.message.choice.c1 != NULLP)
      {
	 dl_DCCH_Msg.message.choice.c1->present = DL_DCCH_MessageType__c1_PR_rrcReconfiguration;
	 CU_ALLOC(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration, sizeof(RRCReconfiguration_t));
	 if(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration != NULLP)
	 {
	    dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->rrc_TransactionIdentifier = 0;
	    dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->criticalExtensions.\
	       present = RRCReconfiguration__criticalExtensions_PR_rrcReconfiguration;
	    /* Fill RRC Reconfig IE */
	    CU_ALLOC(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->\
		  criticalExtensions.choice.rrcReconfiguration, sizeof(RRCReconfiguration_IEs_t));
	    if(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->\
		  criticalExtensions.choice.rrcReconfiguration != NULLP)
	    {
	       ret = fillRrcReconfigIE(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration->\
		     criticalExtensions.choice.rrcReconfiguration);

	       if(ret == ROK)
	       {
		  /* encode DL-DCCH message into RRC Container */
		  xer_fprint(stdout, &asn_DEF_DL_DCCH_MessageType, &dl_DCCH_Msg);
		  memset(encBuf, 0, ENC_BUF_MAX_LEN);
		  encBufSize = 0;
		  encRetVal = aper_encode(&asn_DEF_DL_DCCH_MessageType, 0, &dl_DCCH_Msg, PrepFinalEncBuf, encBuf);
		  /* Encode results */
		  if(encRetVal.encoded == ENCODE_FAIL)
		  {
		     DU_LOG( "\n F1AP : Could not encode RRCContainer for DL-DCCH Msg (at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
		     return RFAILED;
		  }
		  else
		  {
		     DU_LOG("\n F1AP : Created APER encoded buffer for RRCContainer for DL-DCCH Msg\n");
		     for(int i = 0; i< encBufSize; i++)
		     {
			printf("%x",encBuf[i]);
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
	       else
	       {
	          DU_LOG("\nF1AP: Failed to fill RrcReconfig IE at fillDlDcchRrcMsg()");
	       }
	    }
	    else
	    {
	       DU_LOG("\nF1AP: Memory Alloc failed for RRC Reconfig at fillDlDcchRrcMsg()");
	       ret = RFAILED;
	    }
	 }
	 else
	 {
	    DU_LOG("\nF1AP: Memory Alloc failed for RRC Msg at fillDlDcchRrcMsg()");
	    ret = RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nF1AP: Memory Alloc failed for DL Dcch Msg choice at fillDlDcchRrcMsg()");
	 ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nF1AP: RRC Container is NULLP at fillDlDcchRrcMsg()");
      ret = RFAILED;
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

uint8_t	BuildDLRRCContainer(uint8_t rrcMsgType, RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;

   if(rrcMsgType == RRC_SETUP)
   { 
      ret = fillDlCcchRrcMsg(rrcContainer);
      if(ret == RFAILED)
         DU_LOG("\n F1AP: Failed to fill DL-CCCH Msg at BuildDLRRCContainer()");
   }
   else if(rrcMsgType == RRC_RECONFIG)
   {
      ret = fillDlDcchRrcMsg(rrcContainer);
      if(ret == RFAILED)
         DU_LOG("\n F1AP: Failed at fill DL-DCCH Msg at BuildDLRRCContainer()");
   }
   return ret;
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
uint8_t BuildAndSendDLRRCMessageTransfer(uint8_t rrcMsgType)
{
   uint8_t   elementCnt = 0;
   uint8_t  ieId;
   uint8_t  idx;
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
   dlRRCMsg->protocolIEs.list.size = elementCnt * sizeof(DLRRCMessageTransferIEs_t *);

   /* Initialize the F1Setup members */
   CU_ALLOC(dlRRCMsg->protocolIEs.list.array, dlRRCMsg->protocolIEs.list.size);
   if(dlRRCMsg->protocolIEs.list.array == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for DL RRC MessageTransferIEs failed");
      CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
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
   BuildDLRRCContainer(rrcMsgType, &dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer);

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as APER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	 encBuf);
   /* Encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG( "\n F1AP : Could not encode DL RRC Message Transfer structure (at %s)\n",\
	    encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;
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
   uint8_t   elementCnt;
   uint8_t   cellCnt;
   uint8_t   ieId;
   uint8_t   idx;
   uint8_t   drbCnt;
   uint8_t   drbId;
   F1AP_PDU_t      						*f1apMsg = NULL;
   UEContextSetupResponse_t			*ueSetRsp;
   asn_enc_rval_t  						encRetVal;        /* Encoder return value */

   DU_LOG("\n F1AP : Building UE Context Setup Response\n");

   CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }

   f1apMsg->present = F1AP_PDU_PR_successfulOutcome;
   CU_ALLOC(f1apMsg->choice.successfulOutcome,
	 sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG(" F1AP : Memory allocation for	F1AP-PDU failed");
      CU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
      return RFAILED;
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
      return RFAILED;
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
	 return RFAILED;
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
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	 encBuf);
   /* Encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG( "\n F1AP : Could not encode UE Context Setup Request structure (at %s)\n",\
	    encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;
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
      return RFAILED;
   }

   return ROK;
}/* End of BuildAndSendUESetRsp */

/*******************************************************************
 *
 * @brief Function to set the Dl RRC Msg Type
 *
 * @details
 *
 *    Function : setDlRRCMsgType
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

uint8_t setDlRRCMsgType()
{
   uint8_t rrcMsgType = 0;
   switch(f1apMsgDb.dlRrcMsgCount)
   {
      case RRC_SETUP:
        rrcMsgType = RRC_SETUP;
        break;
      case RRC_SECURITY_MODE_COMMAND:
        rrcMsgType = RRC_SECURITY_MODE_COMMAND;
        break;
      case RRC_RECONFIG:
        rrcMsgType = RRC_RECONFIG;
        break;
      default:
        break;
   }
   return rrcMsgType;   
}

/*******************************************************************
 *
 * @brief Function to build Initial UL RRC Message
 *
 * @details
 *
 *    Function : procInitULRRCMsg
 *
 *    Functionality: Function to build Initial UL RRC Message
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procInitULRRCMsg(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx, rrcMsgType;
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
   if(ret == ROK)
   {
      f1apMsgDb.dlRrcMsgCount++;
      rrcMsgType = setDlRRCMsgType();
      ret = BuildAndSendDLRRCMessageTransfer(rrcMsgType);
   }
   return ret;
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
   uint8_t unused = 4;
   uint8_t byteSize = 5;
   uint8_t val = 16;
   /* Allocate Buffer Memory */
   nrcgi->pLMN_Identity.size = 3 * sizeof(uint8_t);
   CU_ALLOC(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   if(nrcgi->pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   ret = buildPlmnId(cuCfgParams.plmn , nrcgi->pLMN_Identity.buf);

   if(ret != ROK)
   {
      return RFAILED;
   }
   nrcgi->nRCellIdentity.size = byteSize * sizeof(uint8_t);
   CU_ALLOC(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
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
uint8_t BuildSplCellList(SCell_ToBeSetup_List_t *spCellLst)
{
   uint8_t  cellCnt;
   uint8_t  idx;
   uint8_t  ret;
   cellCnt = 1;
   spCellLst->list.count = cellCnt;
   spCellLst->list.size = cellCnt * sizeof(SCell_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(spCellLst->list.array,spCellLst->list.size);
   if(spCellLst->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<cellCnt; idx++)
   {
      CU_ALLOC(spCellLst->list.array[idx],sizeof(SCell_ToBeSetup_ItemIEs_t));
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
uint8_t BuildSRBSetup(SRBs_ToBeSetup_List_t *srbSet)
{
   uint8_t idx;
   uint8_t srbCnt;
   srbCnt = 1;
   srbSet->list.count = srbCnt;
   srbSet->list.size = srbCnt*sizeof(SRBs_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(srbSet->list.array,srbSet->list.size);
   if(srbSet->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<srbCnt; idx++)
   {
      CU_ALLOC(srbSet->list.array[idx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
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
uint8_t BuildQOSInfo(QoSFlowLevelQoSParameters_t *drbQos)
{
   /* NonDynamic5QIDescriptor */
   drbQos->qoS_Characteristics.present = QoS_Characteristics_PR_non_Dynamic_5QI;
   CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI == NULLP)
   {
      return RFAILED;
   }
   /*FiveQI*/
   drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI = 0;
   /*AveragingWindow*/
   CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
	 sizeof(AveragingWindow_t));
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow == \
	 NULLP)
   {
      return RFAILED;
   }
   *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow) = 0;
   /*MaxDataBurstVolume*/
   CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
	 sizeof(MaxDataBurstVolume_t));
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume == \
	 NULLP)
   {
      return RFAILED;
   }
   *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume) = 0;

   /*nRGRAN Allocation Retention Priority*/
   drbQos->nGRANallocationRetentionPriority.priorityLevel = PriorityLevel_highest;
   drbQos->nGRANallocationRetentionPriority.pre_emptionCapability = Pre_emptionCapability_may_trigger_pre_emption;
   drbQos->nGRANallocationRetentionPriority.pre_emptionVulnerability = Pre_emptionVulnerability_not_pre_emptable;

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
uint8_t BuildSNSSAI(SNSSAI_t *snssai)
{
   /*SNSSAI*/
   /*ssT*/
   snssai->sST.size = sizeof(uint8_t);
   CU_ALLOC(snssai->sST.buf,snssai->sST.size);
   if(snssai->sST.buf == NULLP)
   {
      return RFAILED;
   }
   snssai->sST.buf[0] = 3;
   /*sD*/
   CU_ALLOC(snssai->sD,sizeof(OCTET_STRING_t));
   if(snssai->sD == NULLP)
   {
      return RFAILED;
   }
   snssai->sD->size = 3*sizeof(uint8_t);
   CU_ALLOC(snssai->sD->buf,snssai->sD->size);
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
uint8_t BuildFlowsMap(Flows_Mapped_To_DRB_List_t *flowMap)
{
   uint8_t  ret;
   uint8_t  idx;
   uint8_t  flowCnt;
   flowCnt = 1;
   flowMap->list.count = flowCnt;
   flowMap->list.size = flowCnt * sizeof(Flows_Mapped_To_DRB_Item_t *);
   CU_ALLOC(flowMap->list.array,flowMap->list.size);
   if(flowMap->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<flowCnt; idx++)
   {
      CU_ALLOC(flowMap->list.array[idx],sizeof(Flows_Mapped_To_DRB_Item_t));
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
uint8_t BuildULTnlInfo(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
   uint8_t idx;
   uint8_t ulCnt;
   ulCnt = 1;
   ulInfo->list.count = ulCnt;
   ulInfo->list.size = ulCnt * sizeof(ULUPTNLInformation_ToBeSetup_Item_t *);
   CU_ALLOC(ulInfo->list.array,ulInfo->list.size);
   if(ulInfo->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<ulCnt; idx++)
   {
      CU_ALLOC(ulInfo->list.array[idx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
      if(ulInfo->list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   idx = 0;
   ulInfo->list.array[idx]->uLUPTNLInformation.present = \
							 UPTransportLayerInformation_PR_gTPTunnel;
   /*GTP TUNNEL*/
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel,\
	 sizeof(GTPTunnel_t));
   if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel == NULLP)
   {
      return RFAILED;
   }
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.size	= 4*sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
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
      = 4 * sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->\
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
uint8_t BuildDRBSetup(DRBs_ToBeSetup_List_t *drbSet)
{
   uint8_t BuildQOSInforet;
   uint8_t BuildSNSSAIret;
   uint8_t BuildFlowsMapret;
   uint8_t BuildULTnlInforet;
   uint8_t idx;
   uint8_t drbCnt;
   DRBs_ToBeSetup_Item_t *drbSetItem;
   drbCnt = 1;
   drbSet->list.count = drbCnt;
   drbSet->list.size = drbCnt*sizeof(DRBs_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(drbSet->list.array,drbSet->list.size);
   if(drbSet->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<drbCnt; idx++)
   {
      CU_ALLOC(drbSet->list.array[idx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
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
   CU_ALLOC(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
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
  BuildQOSInforet =  BuildQOSInfo(&drbSetItem->qoSInformation.choice.\
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
   drbSetItem->rLCMode = RLCMode_rlc_um_bidirectional;

   /*UL Configuration*/
   CU_ALLOC(drbSetItem->uLConfiguration,sizeof(ULConfiguration_t));
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
   if(nrcgi->pLMN_Identity.buf != NULLP)
   {
      if(nrcgi->nRCellIdentity.buf != NULLP)
      {
	 CU_FREE(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
      }
      CU_FREE(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
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
   uint8_t  cellidx;
   if(spCellLst->list.array != NULLP)
   {
      for(cellidx=0; cellidx<spCellLst->list.count; cellidx++)
      {
	 if(cellidx==0&&spCellLst->list.array[cellidx]!=NULLP)
	 {
	    FreeNrcgi(&spCellLst->list.array[cellidx]->value.choice.SCell_ToBeSetup_Item.sCell_ID);
	 }
	 if(spCellLst->list.array[cellidx]!=NULLP)
	 {
	    CU_FREE(spCellLst->list.array[cellidx],sizeof(SCell_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(spCellLst->list.array,spCellLst->list.size);
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
   uint8_t srbidx;
   if(srbSet->list.array != NULLP)
   {
      for(srbidx=0; srbidx<srbSet->list.count; srbidx++)
      {
	 if(srbSet->list.array[srbidx]!=NULLP)
	 {
	    CU_FREE(srbSet->list.array[srbidx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(srbSet->list.array,srbSet->list.size);
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
	    CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
		  sizeof(MaxDataBurstVolume_t));
	 }
	 CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
	       sizeof(AveragingWindow_t));
      }
      CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,\
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
   uint8_t ulidx=0;
   if(ulInfo->list.array != NULLP)
   {
      for(ulidx=0; ulidx<ulInfo->list.count; ulidx++)
      {
	 if(ulidx==0&&ulInfo->list.array[ulidx]!=NULLP)
	 {
	    if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel!=NULLP)
	    {
	       if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
		     transportLayerAddress.buf != NULLP)
	       {
		  if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
			!=NULLP)
		  {
		     CU_ALLOC(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
			   gTP_TEID.buf,ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.\
			   gTPTunnel->gTP_TEID.size);
		  }
		  CU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
			transportLayerAddress.buf,ulInfo->list.array[ulidx]->\
			uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
	       }
	       CU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel,\
		     sizeof(GTPTunnel_t));
	    }
	 }
	 if(ulInfo->list.array[ulidx]!=NULLP)
	 {
	    CU_FREE(ulInfo->list.array[ulidx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
	 }
      }
      CU_FREE(ulInfo->list.array,ulInfo->list.size);
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
   uint8_t  flowidx;
   uint8_t  drbidx;
   if(drbSet->list.array == NULLP)
   {
      for(drbidx=0; drbidx<drbSet->list.count; drbidx++)
      {
	 if(drbidx==0&&drbSet->list.array[drbidx] != NULLP)
	 {
	    drbSetItem =&drbSet->list.array[drbidx]->value.choice.DRBs_ToBeSetup_Item;
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
			if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf!=NULLP)
			{
			   if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD!=NULLP)
			   {
			      if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf!=NULLP)
			      {
				 if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
				       flows_Mapped_To_DRB_List.list.array != NULLP)
				 {
				    for(flowidx=0;flowidx<drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
					  flows_Mapped_To_DRB_List.list.count; flowidx++)
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
						   FreeULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
						   CU_FREE(drbSetItem->uLConfiguration,sizeof(ULConfiguration_t));

						   CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
							 DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
							 qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
							 sizeof(MaxDataBurstVolume_t));	  
						}
						CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						      DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						      qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
					     }
					     CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						   DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						   qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
					  }
				       }
				       if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
					     DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]!=NULLP)
				       {
					  CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx],sizeof(Flows_Mapped_To_DRB_Item_t));
				       }
				    }
				    CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
					  flows_Mapped_To_DRB_List.list.array,drbSetItem->qoSInformation.choice.choice_extension->value.\
					  choice.DRB_Information.flows_Mapped_To_DRB_List.list.size);
				 }
				 CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf,\
				       drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->size);
			      }
			      CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD,\
				    sizeof(OCTET_STRING_t));
			   }
			   CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf,\
				 drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.size);
			}
			CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			      qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
		     }
		     CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			   qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		  }
		  CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			qoS_Characteristics.choice.non_Dynamic_5QI, sizeof(NonDynamic5QIDescriptor_t));
	       }
	       CU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
	    }
	 }
	 if(drbSet->list.array[drbidx]!=NULLP)
	 {
	    CU_FREE(drbSet->list.array[drbidx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(drbSet->list.array,drbSet->list.size);
   }
}


/*******************************************************************
 *
 * @brief Free the UE Setup Request
 *
 * @details
 *
 *    Function : FreeUESetReq
 *
 *    Functionality: Deallocate the memory of BuildUESetReq
 *
 * @params[in]  F1AP_PDU_t  *f1apMsg
 *
 * @return void
 *
 *
 * ****************************************************************/
void FreeUESetReq(F1AP_PDU_t  *f1apMsg)
{
   uint8_t idx=2;
   uint8_t ieId;
   UEContextSetupRequest_t       *ueSetReq;

   if(f1apMsg != NULLP)
   {
      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
	 ueSetReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;
	 if(ueSetReq->protocolIEs.list.array != NULLP)
	 {
	    if(ueSetReq->protocolIEs.list.array[idx])
	    {
	       FreeNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI);
	       idx=6;
	       if(ueSetReq->protocolIEs.list.array[idx])
	       {
	          FreeSplCellList(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SCell_ToBeSetup_List);
		  idx++;
		  if(ueSetReq->protocolIEs.list.array[idx])
		  {
		     FreeSRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List);
	             idx++;
		     if(ueSetReq->protocolIEs.list.array[idx])
		     {
		        FreeDRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List);
		     }   
		  }	 
	       }
	    }
	    for(ieId=0; ieId<ueSetReq->protocolIEs.list.count; ieId++)
	    {
	       if(ueSetReq->protocolIEs.list.array[ieId] != NULLP)
	       {
		  CU_FREE(ueSetReq->protocolIEs.list.array[ieId],sizeof(UEContextSetupRequestIEs_t));
	       }
	    }
	    CU_FREE(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);
	 }
	 CU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
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
uint8_t BuildAndSendUESetReq(uint8_t cuUeF1apId, uint8_t duUeF1apId, \
   uint16_t rrcContLen, uint8_t *rrcContainer)
{
   uint8_t   Nrcgiret;
   uint8_t   SplCellListret;
   uint8_t   SrbSetupret;
   uint8_t   elementCnt;
   uint8_t   idx;
   uint8_t   idx1;
   F1AP_PDU_t      	*f1apMsg = NULLP;
   UEContextSetupRequest_t *ueSetReq = NULLP;
   asn_enc_rval_t encRetVal;        /* Encoder return value */
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));
   uint8_t ret= RFAILED;
   uint8_t ret1;
   while(true)
   {
      DU_LOG("\n F1AP : Building UE Context Setup Request\n");

      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }

      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      CU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
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

      elementCnt = 11;
      ueSetReq->protocolIEs.list.count = elementCnt;
      ueSetReq->protocolIEs.list.size = \
					elementCnt * sizeof(UEContextSetupRequestIEs_t *);

      /* Initialize the UESetup members */
      CU_ALLOC(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);

      if(ueSetReq->protocolIEs.list.array == NULLP)
      {
	 DU_LOG(" F1AP : Memory allocation for UE Context SetupRequest failed");
	 break;
      }

      for(idx1=0; idx1<elementCnt; idx1++)
      {
	 CU_ALLOC(ueSetReq->protocolIEs.list.array[idx1],sizeof(UEContextSetupRequestIEs_t));
	 if(ueSetReq->protocolIEs.list.array[idx1] == NULLP)
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
      ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = cuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = duUeF1apId;

      /*Special Cell ID*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= \
						  ProtocolIE_ID_id_SpCell_ID;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
							     UEContextSetupRequestIEs__value_PR_NRCGI;
      Nrcgiret = BuildNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI);
      if(Nrcgiret != ROK)
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
      SplCellListret = BuildSplCellList(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SCell_ToBeSetup_List);
      if(SplCellListret != ROK)
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
      SrbSetupret =	BuildSRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List);
      if(SrbSetupret != ROK)
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
      ret1 = BuildDRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List);
      if(ret1 != ROK)
      {	
	 break;
      }
      
      /* RRC Container */
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_RRCContainer;
      ueSetReq->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
         UEContextSetupRequestIEs__value_PR_RRCContainer;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size = rrcContLen;
      CU_ALLOC(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,
          ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
      if(!ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf)
      {
         DU_LOG(" F1AP : Memory allocation for BuildAndSendUESetReq failed");
         break;
      }
      memcpy(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf, \
         rrcContainer, ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size); 

      /* RRC delivery status request */
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id = \
         ProtocolIE_ID_id_RRCDeliveryStatusRequest;
      ueSetReq->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
      ueSetReq->protocolIEs.list.array[idx]->value.present = \
         UEContextSetupRequestIEs__value_PR_RRCDeliveryStatusRequest;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCDeliveryStatusRequest = \
         RRCDeliveryStatusRequest_true;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
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
      if(SendF1APMsg(CU_APP_MEM_REG,CU_POOL)	!=	ROK)
      {
	 DU_LOG("\n F1AP : Sending UE Context Setup Request Failed");
	 break;
      }
      ret = ROK;
      break;
   }
   FreeUESetReq(f1apMsg);

   return ret;
}/* End of BuildAndSendUESetReq*/

uint8_t procUlRrcMsg(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx, ret, srbId, rrcMsgType;
   uint8_t cuUeF1apId, duUeF1apId;
   uint8_t *rrcContainer = NULLP;
   uint16_t rrcContLen;
   ULRRCMessageTransfer_t *ulRrcMsg = NULLP;

   ret = ROK;
   ulRrcMsg = &f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;

   for(idx=0; idx < ulRrcMsg->protocolIEs.list.count; idx++)
   {
      switch(ulRrcMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       cuUeF1apId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       duUeF1apId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_SRBID:
	    srbId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.SRBID;
	    break;
	 case ProtocolIE_ID_id_RRCContainer:
	    {
	       rrcContLen = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size;
	       CU_ALLOC(rrcContainer, rrcContLen)
	       if(!rrcContainer)
	       {
	          DU_LOG("\nCU_STUB: Failed to allocated memory in procUlRrcMsg");
	          return RFAILED;
	       }
	       memcpy(rrcContainer, ulRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,\
		     rrcContLen);
	       break;
	    }

	 default:
	    DU_LOG("\n Invalid Event %ld", ulRrcMsg->protocolIEs.list.array[idx]->id);
	    break;
      }
   }
   if(srbId == 1)
   {
      f1apMsgDb.dlRrcMsgCount++;
      rrcMsgType = setDlRRCMsgType();
      if(rrcMsgType == RRC_SECURITY_MODE_COMMAND)
      {
         ret = BuildAndSendUESetReq(cuUeF1apId, duUeF1apId, rrcContLen, rrcContainer);
      }
      if(rrcMsgType == RRC_RECONFIG)
      {
         ret = BuildAndSendDLRRCMessageTransfer(rrcMsgType);
      }
   }
   return ret;
}

/****************************************************************
 * @brief Build And Send F1ResetAck 
 *
 * @details
 *
 *    Function : FreeF1ResetAck
 *
 *    Functionality:
 *         - Build And Send  F1ResetRSP
 *
 *  @params[in]
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
                   CU_FREE(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
                }
             }
             CU_FREE(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size );
          }
          CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
       }
       CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
    }
}

/****************************************************************
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
   uint8_t  	          elementCnt = 0;
   uint8_t                ret = RFAILED;
   F1AP_PDU_t             *f1apMsg = NULL;
   ResetAcknowledge_t     *f1ResetAck = NULLP;
   asn_enc_rval_t         encRetVal;
   DU_LOG("\nF1AP : Building F1 Reset Acknowledgment \n");

   do{
        /* Allocate the memory for F1ResetRequest_t */
        CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
        if(f1apMsg == NULLP)
        {
	   DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
	   break;
        }

        f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

        CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
        if(f1apMsg->choice.successfulOutcome == NULLP)
        {
 	   DU_LOG("\nF1AP : Memory allocation for F1AP-PDU failed");
	   break;
        }

        f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_Reset;
        f1apMsg->choice.successfulOutcome->criticality   = Criticality_reject;
        f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_ResetAcknowledge;
        f1ResetAck = &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

        elementCnt = 1;

        f1ResetAck->protocolIEs.list.count = elementCnt;
        f1ResetAck->protocolIEs.list.size = elementCnt*sizeof(ResetAcknowledgeIEs_t *);

        CU_ALLOC(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size ); 
        if(f1ResetAck->protocolIEs.list.array == NULLP) 
        {
 	   DU_LOG("\nF1AP : Memory allocation for F1ResetAckIEs failed");
	   break;
        }

        for(idx=0; idx<elementCnt; idx++)     
        {
	   CU_ALLOC(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
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
	   DU_LOG("\nF1AP : Created APER encoded buffer for F1ResetAck \n");
	   for(int i=0; i< encBufSize; i++)
	   {
	      printf("%x",encBuf[i]);
	   }
        }
        /* Sending msg */
        if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL) != ROK)
        {
	   DU_LOG("\nF1AP : Sending F1 Reset Response failed");
	   break;
        }

        ret = ROK;
        break;
   }while(true);
   
   FreeF1ResetAck(f1apMsg);
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
   ODU_PRINT_MSG(mBuf, 0,0);

   /* Copy mBuf into char array to decode it */
   ODU_GET_MSG_LEN(mBuf, &recvBufLen);
   CU_ALLOC(recvBuf, (Size)recvBufLen);

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
   CU_FREE(recvBuf, (Size)recvBufLen);

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
            case InitiatingMessage__value_PR_Reset:
	    {
	        DU_LOG("\nF1AP : F1 reset request received ");
	        BuildAndSendF1ResetAck();
	        break;
	    }

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
	       DU_LOG("\nF1AP : Sending F1 reset request");
	       BuildAndSendF1ResetReq();
               break;
            }
            case InitiatingMessage__value_PR_InitialULRRCMessageTransfer:
            {
               DU_LOG("\nF1AP : Received InitialULRRCMessageTransfer");
               procInitULRRCMsg(f1apMsg);
               break;
            }
	    case InitiatingMessage__value_PR_ULRRCMessageTransfer:
	    {
	       DU_LOG("\nF1AP : Received ULRRCMessageTransfer");
	       procUlRrcMsg(f1apMsg);
	       break;
	    }
	    
	    case InitiatingMessage__value_PR_RRCDeliveryReport:
	    {
	       DU_LOG("\nF1AP : Received RRC delivery report");
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
      
      case F1AP_PDU_PR_successfulOutcome:
      {
         switch(f1apMsg->choice.successfulOutcome->value.present)
	 {
	    case SuccessfulOutcome__value_PR_ResetAcknowledge:
            {
	       DU_LOG("\nF1Reset Acknowledgement is received successfully ");
	       break;
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

/**********************************************************************
  End of file
 **********************************************************************/
