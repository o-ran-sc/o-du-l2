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

/* This file contains all SCTP related functionality */
#include "common_def.h"
#include "lrg.h"
#include "legtp.h"
#include "lrg.x"
#include "lkw.x"
#include "cm_inet.h"
#include "cm_inet.x"
#include "du_app_mac_inf.h"
#include "du_cfg.h"
#include "du_sctp.h"
#include "lsctp.h"
#include "du_utils.h"

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : sctpActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during SCTP's 
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]  Ent entity, the entity ID of this task.     
 * @param[in]  Inst inst, the instance ID of this task.
 * @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 * @param[in]  Reason reason.
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
uint8_t sctpActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   DU_LOG("\n\nDEBUG  -->  SCTP : Initializing");
   ODU_SET_PROC_ID(DU_PROC);
   connUp = FALSE;
   
   memset(&f1Params, 0, sizeof(DuSctpDestCb));
   f1Params.assocId = -1;
   memset(&ricParams, 0, sizeof(DuSctpDestCb));
   ricParams.assocId = -1;
   nonblocking = FALSE;
   return ROK;

}

/**************************************************************************
* @brief Function prints src, dest, msg infor about all the msgs that received.
*
* @details
*
*     Function : callFlowSctpActvTsk
*
*     Function prints src, dest, msg infor about all the msgs that received
*
*  @param[in]  Pst     *pst, Post structure of the primitive.
*
*  @return  void
***************************************************************************/
uint8_t callFlowSctpActvTsk(Pst *pst)
{
   char sourceTask[50];
   char destTask[50]="ENTSCTP";
   char message[100];

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            strcpy(sourceTask,"ENTDUAPP");
            switch(pst->event)
            {
               case EVTSTARTPOLL:
                  {
                     strcpy(message,"EVTSTARTPOLL");
                     break;
                  }
            }
            break;
         }
   }
   DU_LOG("\nCall Flow: %s -> %s : %s\n", sourceTask, destTask, message);
}

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : sctpActvTsk 
 * 
 *      Functionality:
 *           This function handles all SCTP messages received
 *           This API is registered with SSI during the 
 *           Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t sctpActvTsk(Pst *pst, Buffer *mBuf)
{

#ifdef CALL_FLOW_DEBUG_LOG
   callFlowSctpActvTsk(pst);
#endif


   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            switch(pst->event)
            {
               case EVTSTARTPOLL:
               {
                  sctpSockPoll();
                  break;
               } 
            }
            break;
         }
   }
   ODU_EXIT_TASK();
   return ROK;
}
/*******************************************************************
 *
 * @brief Checks the status of the received information
 *
 * @details
 *
 *    Function : duCheckReqStatus
 *
 *    Functionality:
 *       Checks the status of the received information
 *
 * @params[in] Confirm status
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t duCheckReqStatus(CmStatus *cfm)
{
   uint8_t ret = ROK;
   if(cfm->status != LCM_PRIM_OK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to process the request successfully");
      ret = RFAILED;
   }
   return (ret); 
}

/**************************************************************************
 * @brief Function to configure the Sctp Params during config Request
 *
 * @details
 *
 *      Function : duSctpCfgReq
 * 
 *      Functionality:
 *           This function configures SCTP Params during the config Request
 *     
 * @param[in]  SctpParams sctpCfg, common structure which has all the configuration
 * @param[in]  CmStatus cfm, Builds the cfm status and reason
 *  buffer.
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/

uint8_t duSctpCfgReq(SctpParams sctpCfg)
{
   uint8_t  ret = ROK;
   CmStatus cfm;

#ifdef CALL_FLOW_DEBUG_LOG
    DU_LOG("\nCall Flow: ENTDUAPP -> ENTSCTP : EVENT_CFG_REQ_TO_SCTP\n");
#endif
/* Fill F1 Params */
   f1Params.destIpAddr.ipV4Pres  = sctpCfg.cuIpAddr.ipV4Pres;
   f1Params.destIpAddr.ipV4Addr  = sctpCfg.cuIpAddr.ipV4Addr;
   f1Params.destPort             = sctpCfg.cuPort;
   f1Params.itfState             = DU_SCTP_DOWN;
   f1Params.srcPort              = sctpCfg.duPort[F1_INTERFACE];
   f1Params.recvMsgSet           = ROK;
   memset (&f1Params.sockFd, -1, sizeof(CmInetFd));
   fillDestNetAddr(&f1Params.destIpNetAddr, &f1Params.destIpAddr);
   fillAddrLst(&f1Params.destAddrLst, &f1Params.destIpAddr);

/* Fill RIC Params */
   ricParams.destIpAddr.ipV4Pres = sctpCfg.ricIpAddr.ipV4Pres;
   ricParams.destIpAddr.ipV4Addr = sctpCfg.ricIpAddr.ipV4Addr;
   ricParams.destPort            = sctpCfg.ricPort;
   ricParams.itfState            = DU_SCTP_DOWN;
   ricParams.srcPort             = sctpCfg.duPort[E2_INTERFACE];
   ricParams.recvMsgSet          = ROK;
   memset (&ricParams.sockFd, -1, sizeof(CmInetFd));
   fillDestNetAddr(&ricParams.destIpNetAddr, &ricParams.destIpAddr);
   fillAddrLst(&ricParams.destAddrLst, &ricParams.destIpAddr);

/* Fill AddressList */
   fillAddrLst(&localAddrLst, &sctpCfg.duIpAddr);

/* Set polling to FALSE */
   pollingState = FALSE;  

/* Fill Cfm Status */
   cfm.status = LCM_PRIM_OK;
   cfm.reason = LCM_REASON_NOT_APPL;

   ret = duCheckReqStatus(&cfm);

   return (ret);
}

/*******************************************************************
 *
 * @brief Fills the address List of the source Ip Address
 *
 * @details
 *
 *    Function : fillAddrLst
 *
 *    Functionality:
 *       Fills the address List of source Ip Address
 *
 * @params[in] CmInetNetAddrLst *addrLstPtr, Address List pointer
 * @params[in] F1IpAddr *srcIpAddr, src Ip Adrress to be filled in the Address List
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, F1IpAddr *ipAddr)
{ 
   addrLstPtr->count++;
   addrLstPtr->addrs[(addrLstPtr->count - 1)].type = CM_INET_IPV4ADDR_TYPE;
   addrLstPtr->addrs[(addrLstPtr->count - 1)].u.ipv4NetAddr = CM_INET_NTOH_UINT32(ipAddr->ipV4Addr);

   return ROK;
}

/******************************************************************************
 *
 * @brief Fills the address List of the source Ip Address
 *
 * @details
 *
 *    Function : fillDestNetAddr
 *
 *    Functionality:
 *       Fills the address List of destinatoion Ip Address
 *
 * @params[in] CmInetNetAddr *destAddrPtr, Address List pointer
 * @params[in] F1IpAddr *dstIpAddr, destIp Address to be filled in the Address List
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************************/
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, F1IpAddr *dstIpPtr)
{
   /* Filling destination address */
   destAddrPtr->type = CM_INET_IPV4ADDR_TYPE;
   destAddrPtr->u.ipv4NetAddr = CM_INET_NTOH_UINT32(dstIpPtr->ipV4Addr);
   return ROK;
}

/******************************************************************************
 *
 * @brief Establishes the Assoc Req for the received interface type
 *
 * @details
 *
 *    Function : establishAssocReq
 *
 *    Functionality:
 *       Eastablishes the Assoc Req for the received interface type
 *
 * @params[in] DuSctpDestCb *paramPtr
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************************/

uint8_t establishReq(DuSctpDestCb *paramPtr)
{
   Pst pst;
   uint8_t ret = ROK;
   socket_type = CM_INET_STREAM;

   if((ret = cmInetSocket(socket_type, &paramPtr->sockFd, IPPROTO_SCTP)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP : Failed while opening a socket in ODU");
   } 
   else if((ret = cmInetSctpBindx(&paramPtr->sockFd, &localAddrLst, paramPtr->srcPort)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP:  Failed during Binding in ODU");
   }
   else if((ret = sctpSetSockOpts(&paramPtr->sockFd)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP : Failed to set Socket Opt in ODU");
   }     
   else
   { 
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP : Failed while establishing Req at DU");
         ret = RFAILED;
      }
      else 
      {
         ret = cmInetSctpConnectx(&paramPtr->sockFd, &paramPtr->destIpNetAddr, &paramPtr->destAddrLst, paramPtr->destPort);
         /* 115 error_code indicates that Operation is in progress and hence ignored if SctpConnect failed due to this */
         if(ret == 18)             
         {
           ret = ROK; 
         }
      }
   }
   if((ret == ROK) & (paramPtr->itfState == DU_SCTP_DOWN))
   {
      paramPtr->itfState = DU_SCTP_CONNECTING;
   }

   /* Post the EVTSTARTPOLL Msg */
   if(!pollingState)
   {
      pollingState = TRUE;
      duFillSctpPst(&pst, EVTSTARTPOLL);
   }
   
   return (ret);
}

/******************************************************************************
 *
 * @brief Processes the assoc Req for the received interface type
 *
 * @details
 *
 *    Function : duSctpAssocReq
 *
 *    Functionality:
 *       Processes the Assoc Req for the received interface type
 *
 * @params[in] itfType interface Type, 
 * @params[in] cfm , represents the status of request
 * 
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************************/

uint8_t duSctpAssocReq(uint8_t itfType)
{
   uint8_t      ret = ROK;
   CmStatus     cfm;
   DuSctpDestCb *paramPtr = NULLP;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTDUAPP -> ENTSCTP : EVENT_ASSOC_REQ_TO_SCTP\n");
#endif
 
   DU_ALLOC(paramPtr, sizeof(DuSctpDestCb));
   if(paramPtr == NULLP)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to allocate memory");
      return RFAILED;
   }
   switch(itfType)
   {
      case F1_INTERFACE:
      {
         paramPtr = &f1Params;
         ret = establishReq(paramPtr);
         break;
      }
      case E2_INTERFACE:
      {
         paramPtr = &ricParams;
         ret = establishReq(paramPtr);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  SCTP : Invalid Interface Type %d", itfType);
         break;
      }
   }  
   if(ret != ROK)
   { 
      DU_LOG("\nERROR  -->  SCTP : ASSOC Req Failed.");
      cfm.status = LCM_PRIM_NOK;
      cfm.reason = LCM_REASON_NOT_APPL;
   }
   else
   {
      cfm.status = LCM_PRIM_OK;
      cfm.reason = LCM_REASON_NOT_APPL;
   }
   ret = duCheckReqStatus(&cfm);

   return (ret);
}
/*******************************************************************
 *
 * @brief  Fills Pst struct for ENTSCTP
 *
 * @details
 *
 *    Function : duFillSctpPst
 *
 *    Functionality:
 *       Fills Pst struct for ENTSCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duFillSctpPst(Pst *pst, Event event)
{
   Buffer *mBuf;
   if(ODU_GET_MSG_BUF(DFLT_REGION, DU_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to allocate memory");
      return RFAILED;
   }
   memset(pst, 0, sizeof(Pst));
   pst->srcEnt = (Ent)ENTDUAPP;
   pst->srcInst = (Inst)DU_INST;
   pst->srcProcId = DU_PROC;
   pst->dstEnt = (Ent)ENTSCTP;
   pst->dstInst = (Inst)SCTP_INST;
   pst->dstProcId = pst->srcProcId;
   pst->event = event;
   pst->selector = ODU_SELECTOR_LC;
   pst->pool= DU_POOL;
   ODU_POST_TASK(pst, mBuf); 

   return ROK;
}

/*******************************************************************
 *
 * @brief Sets socket options as per requirement
 *
 * @details
 *
 *    Function : sctpSetSockOpts
 *
 *    Functionality: 
 *       Sets socket options as per requirement
 *
 * @params[in] sock_Fd  determines the sockFd to be set
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpSetSockOpts(CmInetFd *sock_Fd)
{
    uint8_t ret = ROK;
    CmSctpEvent sctpEvent;

   sctpEvent.dataIoEvent          = TRUE;
   sctpEvent.associationEvent     = TRUE;
   sctpEvent.addressEvent         = TRUE;
   sctpEvent.sendFailureEvent     = TRUE;
   sctpEvent.peerErrorEvent       = TRUE;
   sctpEvent.shutdownEvent        = TRUE;
   sctpEvent.partialDeliveryEvent = TRUE;
   sctpEvent.adaptationLayerEvent = TRUE;

   if((ret = cmInetSetOpt(sock_Fd, CM_SOCKOPT_LEVEL_SCTP, CM_SOCKOPT_OPT_SCTP_EVENTS, &sctpEvent) != ROK))
   {
     ret = RFAILED;
   }

   return (ret);
}

/*******************************************************************
 *
 * @brief Post received data/notification to DU APP 
 *
 * @details
 *
 *    Function : sendToDuApp 
 *
 *    Functionality:
 *         Post received data/notification to DU APP
 *
 * @params[in]  Message buffer
 *              Message event
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void sendToDuApp(Buffer *mBuf, Event event)
{
   Pst pst;
   DU_LOG("\nDEBUG   -->  SCTP : Forwarding received message to duApp");
   ODU_PRINT_MSG(mBuf, 0, 0);


   memset(&(pst), 0, sizeof(Pst));
   pst.srcEnt = (Ent)ENTSCTP;
   pst.srcInst = (Inst)SCTP_INST;
   pst.srcProcId = DU_PROC;
   pst.dstEnt = (Ent)ENTDUAPP;
   pst.dstInst = (Inst)DU_INST;
   pst.dstProcId = pst.srcProcId;
   pst.event = event;
   pst.selector = ODU_SELECTOR_LC;
   pst.pool= DU_POOL;
   pst.region = DFLT_REGION;

   if (ODU_POST_TASK(&pst, mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP : ODU_POST_TASK failed in duReadCfg");
   }
}

/*******************************************************************
 *
 * @brief Handles an SCTP notification message
 *
 * @Sending *
 *    Function : sctpNtfyHdlr
 *
 *    Functionality:
 *      Handles an SCTP notification message
 *
 * @params[in] Notify message
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpNtfyHdlr(CmInetSctpNotification *ntfy, uint8_t *itfState)
{
   Pst pst;

   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE :
         DU_LOG("\nDEBUG   -->  SCTP : Assoc change notification received");
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               DU_LOG("INFO   -->  Event : COMMUNICATION UP");
               *itfState = DU_SCTP_UP;
               break;
            case CM_INET_SCTP_COMM_LOST:
               DU_LOG("INFO   -->  Event : COMMUNICATION LOST");
               *itfState = DU_SCTP_DOWN;
               break;
            case CM_INET_SCTP_RESTART:
               DU_LOG("INFO   -->  Event : SCTP RESTART");
               *itfState = DU_SCTP_DOWN;
               break;
            case CM_INET_SCTP_SHUTDOWN_COMP: /* association gracefully shutdown */
               DU_LOG("INFO   -->  Event : SHUTDOWN COMPLETE");
               *itfState = DU_SCTP_DOWN;
               break;
            case CM_INET_SCTP_CANT_STR_ASSOC:
               DU_LOG("INFO   -->  Event : CANT START ASSOC");
               *itfState = DU_SCTP_DOWN;
               break;
            default:
               DU_LOG("\nERROR  -->  Invalid event %d", ntfy->u.assocChange.state);
               break;
         }
         break;
      case CM_INET_SCTP_PEER_ADDR_CHANGE :
         DU_LOG("\nINFO   -->  SCTP : Peer Address Change notificarion received");
         /* Need to add handler */
         break;
      case CM_INET_SCTP_REMOTE_ERROR :
         DU_LOG("\nINFO   -->  SCTP : Remote Error notification received");
         break;
      case CM_INET_SCTP_SEND_FAILED :
         DU_LOG("\nINFO   -->  SCTP : Send Failed notification received\n");
         break;
      case CM_INET_SCTP_SHUTDOWN_EVENT : /* peer socket gracefully closed */
         DU_LOG("\nINFO   -->  SCTP : Shutdown Event notification received\n");
         *itfState = DU_SCTP_DOWN;
         exit(0);
         break;
      case CM_INET_SCTP_ADAPTATION_INDICATION :
         DU_LOG("\nINFO   -->  SCTP : Adaptation Indication received\n");
         break;
      case CM_INET_SCTP_PARTIAL_DELIVERY_EVENT:
         DU_LOG("\nINFO   -->  SCTP : Partial Delivery Event received\n");
         break;
      default:
         DU_LOG("\nERROR  -->  SCTP : Invalid sctp notification type %d", ntfy->header.nType);
         break;
   }

   /* Pack notification and send to APP */
   DU_LOG("\nDEBUG   -->  SCTP : Forwarding received message to duApp");
    
   memset(&(pst), 0, sizeof(Pst));
   pst.srcEnt = (Ent)ENTSCTP;
   pst.srcInst = (Inst)SCTP_INST;
   pst.srcProcId = DU_PROC;
   pst.dstEnt = (Ent)ENTDUAPP;
   pst.dstInst = (Inst)DU_INST;
   pst.dstProcId = pst.srcProcId;
   pst.event = EVENT_SCTP_NTFY;
   pst.selector = ODU_SELECTOR_LC;
   pst.pool= DU_POOL;
   pst.region = DU_APP_MEM_REGION;
   
   if(cmPkSctpNtfy(&pst, ntfy) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP : Failed to pack SCTP notification");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief checks for valid readFd and process the InetSctpRecvMsg
 * during polling 
 *
 * @details
 *
 *    Function : processPolling
 *
 *    Functionality:
 *         checks for valid readFd and process the InetSctpRecvMsg
 *         during polling
 *
 * @params[in]  Params required for polling
 * @params[in]  SockFd for file descriptor
 * @params[in]  timeoutPtr indicates the timeout value
 * @params[in]  MemInfo, recvMsgSet
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t  processPolling(sctpSockPollParams *pollParams, CmInetFd *sockFd, uint32_t *timeoutPtr, CmInetMemInfo *memInfo, bool recvMsgSet)
{
   uint8_t ret = ROK;
   CM_INET_FD_SET(sockFd, &pollParams->readFd);
   ret = cmInetSelect(&pollParams->readFd, NULLP, timeoutPtr, (int16_t *)&pollParams->numFd);
   if(CM_INET_FD_ISSET(sockFd, &pollParams->readFd))
   {
      CM_INET_FD_CLR(sockFd, &pollParams->readFd);
      ret = cmInetSctpRecvMsg(sockFd, &pollParams->addr, &pollParams->port, memInfo, &(pollParams->mBuf), &pollParams->bufLen, &pollParams->info, &pollParams->flag, &pollParams->ntfy);
        
      if(ret != ROK)
      {
         DU_LOG("\nERROR   -->  SCTP: Failed to receive sctp msg for sockFd[%d]\n", sockFd->fd);
         recvMsgSet = RFAILED;
      }
      else
      {
#ifdef CALL_FLOW_DEBUG_LOG
         if(pollParams->port == f1Params.destPort)
            DU_LOG("\nCall Flow: CU -> ENTSCTP : EVENT_SCTP_MSG\n");
         else
            DU_LOG("\nCall Flow: RIC -> ENTSCTP : EVENT_SCTP_MSG\n");
#endif
         if((((pollParams->flag & CM_INET_SCTP_MSG_NOTIFICATION) != 0)) && ret == ROK)
         {
            if(pollParams->port == f1Params.destPort)
            {
               f1Params.assocId = pollParams->ntfy.u.assocChange.assocId;
               DU_LOG("\nDEBUG   -->  SCTP : AssocId assigned to F1Params from PollParams [%d]\n", f1Params.assocId);
               ret = sctpNtfyHdlr(&pollParams->ntfy, &f1Params.itfState);
            }
            else if(pollParams->port == ricParams.destPort)
            {
               ricParams.assocId = pollParams->ntfy.u.assocChange.assocId;
               DU_LOG("\nDEBUG   -->  SCTP : AssocId assigned to ricParams from PollParams [%d]\n", ricParams.assocId);
               ret = sctpNtfyHdlr(&pollParams->ntfy, &ricParams.itfState);
            }
            else
            {
               DU_LOG("\nERROR  -->  SCTP : Failed to fill AssocId\n");
               return RFAILED;
            }
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCTP : Failed to process sctp notify msg\n");
            }
         }
         else if(f1Params.itfState & (pollParams->port == f1Params.destPort))
         {  
            sendToDuApp(pollParams->mBuf, EVENT_CU_DATA);
         }
         else if(ricParams.itfState & (pollParams->port == ricParams.destPort))
         {  
            sendToDuApp(pollParams->mBuf, EVENT_RIC_DATA);
         }

         else
         {
            ODU_PUT_MSG_BUF(pollParams->mBuf);
         }
      }
  }
  return ROK;
}
/*******************************************************************
 *
 * @brief Receives message on the socket
 *
 * @details
 *
 *    Function : sctpSockPoll
 *
 *    Functionality:
 *      Receives message on the socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpSockPoll()
{
   uint8_t  ret       = ROK;
   uint32_t timeout;
   uint32_t *timeout_Ptr;
   CmInetMemInfo memInfo;
   sctpSockPollParams f1PollParams, e2PollParams;

   memset(&f1PollParams, 0, sizeof(sctpSockPollParams));
   memset(&e2PollParams, 0, sizeof(sctpSockPollParams));

   if (f1Params.sockFd.blocking & ricParams.sockFd.blocking)
   {
      /* blocking */
      timeout_Ptr = NULLP;
   }
   else
   {
      /* non-blocking */
      timeout = 0;
      timeout_Ptr = &timeout;
   }
   memInfo.region = DU_APP_MEM_REGION;
   memInfo.pool   = DU_POOL;

   CM_INET_FD_ZERO(&f1PollParams.readFd);
   CM_INET_FD_ZERO(&e2PollParams.readFd);

   DU_LOG("\nINFO   -->  SCTP : Polling started at DU\n");
   while(true)
   {
      if(f1Params.itfState)
      {
         if((ret = processPolling(&f1PollParams, &f1Params.sockFd, timeout_Ptr, &memInfo, f1Params.recvMsgSet)) != ROK)
         {
            DU_LOG("\nERROR  -->  SCTP : Failed to RecvMsg for F1\n");
         }
      }
      if(ricParams.itfState)
      {
         if((ret = processPolling(&e2PollParams, &ricParams.sockFd, timeout_Ptr, &memInfo, ricParams.recvMsgSet)) != ROK)
         {
            DU_LOG("\nERROR  -->  SCTP : Failed to RecvMsg for E2\n");
         }
      }
   };
   return (ret);
}/* End of sctpSockPoll() */

/*******************************************************************
 *
 * @brief Send message on SCTP socket
 *
 * @details
 *
 *    Function : sctpSend 
 *
 *    Functionality:
 *        Send message on SCTP socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpSend(Buffer *mBuf, uint8_t itfType)
{
   uint8_t          ret =0;
   MsgLen           len =0;          /* number of actually sent octets */
   CmInetMemInfo    memInfo;                        
   
   memInfo.region = DU_APP_MEM_REGION;               
   memInfo.pool   = DU_POOL;
   
#ifdef CALL_FLOW_DEBUG_LOG
   if(itfType == F1_INTERFACE)
   {
      DU_LOG("\nCall Flow: ENTDUAPP -> ENTSCTP : EVENT_F1AP_MSG_TO_SCTP\n");
   }
   else
   {
      DU_LOG("\nCall Flow: ENTDUAPP -> ENTSCTP : EVENT_E2AP_MSG_TO_SCTP\n");
   }
#endif

   if(itfType == F1_INTERFACE)
   {
      DU_LOG("\nDEBUG   -->  SCTP : sending the message to CU");
#ifdef CALL_FLOW_DEBUG_LOG
      DU_LOG("\nCall Flow: ENTSCTP -> CU : EVENT_F1AP_MSG_TO_CU\n");
#endif
      ret = cmInetSctpSendMsg(&f1Params.sockFd, &f1Params.destIpNetAddr, f1Params.destPort, &memInfo, mBuf, &len, 0, FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);
   }

   if(itfType == E2_INTERFACE)
   {
      DU_LOG("\nDEBUG   -->  SCTP : sending the message to ric");
#ifdef CALL_FLOW_DEBUG_LOG
      DU_LOG("\nCall Flow: ENTSCTP -> RIC : EVENT_E2AP_MSG_TO_RIC\n");
#endif
      ret = cmInetSctpSendMsg(&ricParams.sockFd, &ricParams.destIpNetAddr, ricParams.destPort, &memInfo, mBuf, &len, 0, FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);
   }

   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("\nERROR  -->  SCTP : Failed sending the message");
      return RFAILED;
   }

   return ROK;
} /* End of sctpSend */

/**********************************************************************
         End of file
**********************************************************************/
