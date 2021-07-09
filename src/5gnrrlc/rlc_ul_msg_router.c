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

/********************************************************************20**
  
        Name:    NR RLC Layer - System Services Interface Functions
    
        Type:    C file
  
        Desc:    C source code for the interface to System Services
                  of NR RLC
 
        File:    rlc_ul_msg_router.c
  
*********************************************************************21*/

/** @file rlc_ul_msg_router.c
@brief RLC System Services Interface
*/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_err.h"        /* Err defines */
#include "rlc_env.h"        /* RLC environment options */


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_ul.h"
#include "rlc_mac_inf.h"
#include "du_app_rlc_inf.h"

#ifdef TENB_STATS 
#include "l2_tenb_stats.x"   
#endif


S16 rlcUlInitExt ARGS (( Void ));

/**
 *
 * @brief
 *
 *  <b> Initialize External </b>
 *
 *  @b Description:
 *  Initializes variables used to interface with Upper/Lower Layer  
 *
 *  @return  S16
 *      -# ROK 
 *
*/
  
S16 rlcUlInitExt()
{

   return ROK;
} /* kwInitExt */



/***********************************************************************
                      System Service Interface Functions
 ***********************************************************************/
/**
 *
 * @brief
 *
 *    <b> Activates Initialization </b>
 *
 *    @b Description:
 *    This function is invoked by system services to initialize the NR RLC
 *    layer. This is an entry point used by LTE_RLC layer to initialize its
 *    global variables, before becoming operational.
 *
 *    Allowable values for parameters are specified in ssi.h.
 *
 *    @param[in] ent    - Specify the entity id of the NR RLC task.
 *    @param[in] inst   - Specify the entity id of the NR RLC task.
 *    @param[in] region - Specifies the memory region from which
 *                         NR RLC should allocate structures and buffers.
 *    @param[in] reason - Specifies the reason for calling this
 *                         initialization function.
 *
 *  @return  S16
 *      -# ROK 
 *
 */
S16 rlcUlActvInit
(
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
)
{
   RlcCb    *tRlcCb;

   if (inst >= MAX_RLC_INSTANCES)
   {
       /* intance greater than MAX instances */ 
       return RFAILED; 
   }

   if (rlcCb[inst] != NULLP)
   {
       return  (RFAILED);
   }
  
   if (SGetSBuf(region, 0, (Data **)&tRlcCb,
                (Size)sizeof (RlcCb)) != ROK)    
   {                     
      return RFAILED;
   }

   /* Initialize rlcCb */
   RLC_MEM_SET(tRlcCb, 0, sizeof(RlcCb));

   /* Initialize task configuration parameters */
   tRlcCb->init.ent     = ent;           /* entity */
   tRlcCb->init.inst    = inst;          /* instance */
   tRlcCb->init.region  = region;        /* static region */
   tRlcCb->init.pool    = 0;             /* static pool */
   tRlcCb->init.reason  = reason;        /* reason */
   tRlcCb->init.cfgDone = FALSE;         /* configuration done */
   tRlcCb->init.acnt    = TRUE;          /* enable accounting */
   tRlcCb->init.usta    = TRUE;          /* enable unsolicited status */
   tRlcCb->init.trc     = FALSE;         /* enable trace */
   tRlcCb->init.procId  = ODU_GET_PROCID();

   rlcCb[inst] = tRlcCb;

   /* call external function for intialization */
   /*
   kwInitExt();
   */
#ifdef TENB_STATS 
   TSL2AllocStatsMem(tRlcCb->init.inst); 
#endif

   return ROK;
} /* rlcUlActvInit */

/*
* @brief
*
* Function:
*   name : callFlowRlcUlActvTsk 
*
*  @b Description:
*  Function used to print values of src, dest, message
*  received at the layer
*
*  @param[in] pst   - Pst Structure
*
*  @return void
*/

void callFlowRlcUlActvTsk(Pst *pst)
{
   char sourceTask[50];
   char destTask[50]="ENTRLCUL";
   char message[100];
   
   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            strcpy(sourceTask,"ENTDUAPP");
            switch(pst->event)
            {
#ifdef LCLKW
               case LKW_EVT_CFG_REQ:
                  {
                     strcpy(message,"LKW_EVT_CFG_REQ");
                     break;
                  }

               case LKW_EVT_CNTRL_REQ:
                  {
                     strcpy(message,"LKW_EVT_CNTRL_REQ");
                     break;
                  }
               
               case EVENT_RLC_UE_CREATE_REQ:        /* UE Create Request */
                  {
                     strcpy(message,"EVENT_RLC_UE_CREATE_REQ");
                     break;
                  }
              case EVENT_RLC_UE_RECONFIG_REQ:      /* UE Reconfig Request */
                  {
                     strcpy(message,"EVENT_RLC_UE_RECONFIG_REQ");
                     break;
                  }
               case EVENT_RLC_UE_DELETE_REQ:
                  {
                     strcpy(message,"EVENT_RLC_UE_DELETE_REQ");
                     break;
                  }

               case LKW_EVT_STS_REQ:
                  {
                     strcpy(message,"LKW_EVT_STS_REQ");
                     break;
                  }

               case LKW_EVT_STA_REQ:
                  {
                     strcpy(message,"LKW_EVT_STA_REQ");
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */
#endif  /* LCLKW */
               default:
                  {
                     strcpy(message,"Invalid Event");
                  break;
                 }
            }
            break;
         }

      case ENTRLC:
         {
            strcpy(sourceTask,"ENTRLC");
            switch(pst->event)
            {
#ifdef LCUDX
               case UDX_EVT_BND_CFM:              /* Bind request */
                  {
                     strcpy(message,"UDX_EVT_BND_CFM");
                     break;
                  }

               case UDX_EVT_CFG_CFM:             /* Unbind request */
                  {
                     strcpy(message,"UDX_EVT_CFG_CFM");
                     break;
                  }

               case UDX_EVT_UEIDCHG_CFM:              /* Configuration request */
                  {
                     strcpy(message,"UDX_EVT_UEIDCHG_CFM");
                     break;
                  }
               
               case UDX_EVT_STA_PHBT_TMR_START:              /* Status Prohibit Timer Start */
                  {
                     strcpy(message,"UDX_EVT_STA_PHBT_TMR_START");
                     break;
                  }               

#endif  /* LCCKW */

               default:
                     strcpy(message,"Invalid Event");
                  break;

            }
            break;
         }

      case ENTMAC:
         {
            strcpy(sourceTask,"ENTMAC");
            switch(pst->event)
            {
#ifdef LCRGU
               case EVTRGUBNDCFM:     /* Bind request */
                  {
                     strcpy(message,"EVTRGUBNDCFM");
                     break;
                  }

               case EVENT_UL_DATA_TO_RLC:    /* UL Data request */
                  {
                     strcpy(message,"EVENT_UL_DATA_TO_RLC");
                     break;
                  }

#endif  /* LCRGU */

               default:
                     strcpy(message,"Invalid Event");
                  break;
            }
            break;
         }
      default:
         {
            strcpy(sourceTask,"Invalid Source Entity Id");
         }
    }
    DU_LOG("\nCall Flow: %s -> %s : %s\n", sourceTask, destTask, message);

} 


  

/**
 *
 * @brief
 *
 *  <b> Activation Task </b>
 *
 *  @b Description:
 *  Processes events received for NR RLC layer via System Services from
 *  other layers.
 *
 *  @param[in] pst   - Pst Structure
 *  @param[in] mBuf  - Message Buffer
 *
 *  @return  S16
 *      -# ROK 
 *
 */
S16 rlcUlActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
{
   S16 ret = ROK;

#ifdef CALL_FLOW_DEBUG_LOG
   callFlowRlcUlActvTsk(pst);
#endif

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            switch(pst->event)
            {
#ifdef LCLKW
               case LKW_EVT_CFG_REQ:
                  {
                     ret = unpackRlcConfigReq(RlcMiRlcConfigReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_CNTRL_REQ:
                  {
                     ret = cmUnpkLkwCntrlReq(RlcMiLkwCntrlReq, pst, mBuf);
                     break;
                  }
               
               case EVENT_RLC_UE_CREATE_REQ:        /* UE Create Request */
                  {
                     ret = unpackRlcUeCreateReq(RlcProcUeCreateReq, pst, mBuf);
                     break;
                  }
              case EVENT_RLC_UE_RECONFIG_REQ:      /* UE Reconfig Request */
                  {
                     ret = unpackRlcUeReconfigReq(RlcProcUeReconfigReq, pst, mBuf);
                     break;
                  }
               case EVENT_RLC_UE_DELETE_REQ:
                  {
                     ret = unpackRlcUeDeleteReq(RlcProcUeDeleteReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_STS_REQ:
                  {
                     ret = cmUnpkLkwStsReq(RlcMiLkwStsReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_STA_REQ:
                  {
                     ret = cmUnpkLkwStaReq(RlcMiLkwStaReq, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
               case LKW_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkLkwL2MeasReq(RlcMiLkwL2MeasReq, pst, mBuf);
                     break;
                  }
               case LKW_EVT_L2MEAS_SEND_REQ:
                 {

                    ret = cmUnpkLkwL2MeasSendReq(RlcMiLkwL2MeasSendReq, pst, mBuf); 
  
                     break;
                 }
               case LKW_EVT_L2MEAS_STOP_REQ:
                 {
                     ret = cmUnpkLkwL2MeasStopReq(RlcMiLkwL2MeasStopReq, pst, mBuf);
                     break;
                 }
#endif
#endif  /* LCLKW */
               default:
                  ODU_PUT_MSG_BUF(mBuf);
                  if (pst->dstInst < MAX_RLC_INSTANCES)
                  {
                     DU_LOG("\nERROR  -->  RLC_UL : Received Invalid Event[%d] from SM",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTRLC:
         {
            switch(pst->event)
            {
#ifdef LCUDX
               case UDX_EVT_BND_CFM:              /* Bind request */
                  {
                     ret = cmUnpkUdxBndCfm(rlcUlUdxBndCfm, pst, mBuf );
                     break;
                  }

               case UDX_EVT_CFG_CFM:             /* Unbind request */
                  {
                     ret = cmUnpkUdxCfgCfm(rlcUlUdxCfgCfm, pst, mBuf );
                     break;
                  }

               case UDX_EVT_UEIDCHG_CFM:              /* Configuration request */
                  {
                     ret = cmUnpkUdxUeIdChgCfm(rlcUlUdxUeIdChgCfm, pst, mBuf);
                     break;
                  }
               
               case UDX_EVT_STA_PHBT_TMR_START:              /* Status Prohibit Timer Start */
                  {
                     ret = cmUnpkUdxStaProhTmrStart(rlcUlUdxStaProhTmrStart, pst, mBuf);
                     break;
                  }               

#endif  /* LCCKW */

               default:
                  ODU_PUT_MSG_BUF(mBuf);
                  if (pst->dstInst < MAX_RLC_INSTANCES)
                  {
                      DU_LOG("\nERROR  -->  RLC_UL : Received Invalid Event[%d] from RLC UL",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTNH:
         {
            switch(pst->event)
            {
#ifdef LCCKW
               case CKW_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkCkwBndReq(RlcUiCkwBndReq, pst, mBuf );
                     break;
                  }

               case CKW_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkCkwUbndReq(RlcUiCkwUbndReq, pst, mBuf );
                     break;
                  }

               case CKW_EVT_UEIDCHG_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkCkwUeIdChgReq(RlcUiCkwUeIdChgReq, pst, mBuf);
                     break;
                  }

#endif  /* LCCKW */

#ifdef LCKWU
               case KWU_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkKwuBndReq(RlcUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(RlcUiKwuUbndReq, pst, mBuf );
                     break;
                  }
#endif  /* LCKWU */
               default:
                  ODU_PUT_MSG_BUF(mBuf);
                  if (pst->dstInst < MAX_RLC_INSTANCES)
                  {
                      DU_LOG("\nERROR  -->  RLC_UL : Received Invalid Event[%d] from RRC", 
                            pst->event);
                  }
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTPJ:
         {
            switch(pst->event)
            {
#ifdef LCKWU
               case KWU_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkKwuBndReq(RlcUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(RlcUiKwuUbndReq, pst, mBuf );
                     break;
                  }

               default:
                  ODU_PUT_MSG_BUF(mBuf);
                  if (pst->dstInst < MAX_RLC_INSTANCES)
                  {
                      DU_LOG("\nERROR  -->  RLC_UL : Received Invalid Event[%d] from PDCP", 
                            pst->event);
                  }
                  ret = RFAILED;
                  break;
#endif  /* LCKWU */
               }
            break;
         }

      case ENTMAC:
         {
            switch(pst->event)
            {
#ifdef LCRGU
               case EVTRGUBNDCFM:     /* Bind request */
                  {
                     ret = cmUnpkRguBndCfm(RlcLiRguBndCfm, pst, mBuf );
                     break;
                  }

               case EVENT_UL_DATA_TO_RLC:    /* UL Data request */
                  {
                     ret = unpackRlcUlData(RlcProcUlData, pst, mBuf);
                     break;
                  }

#endif  /* LCRGU */

               default:
                  ODU_PUT_MSG_BUF(mBuf);
                  if (pst->dstInst < MAX_RLC_INSTANCES)
                  {
                      DU_LOG("\nERROR  -->  RLC_UL : Received Invalid Event[%d] from MAC",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;
            }
            break;
         }
#if defined(L2_L3_SPLIT) && defined (TENB_T2K3K_SPECIFIC_CHANGES) && defined (MAC_RLC_UL_RBUF)
      case ENTYS:
         {
            switch(pst->event)
            {
               case KWU_EVT_TTI_IND:
                  {
                     rlcUlBatchProc();
                     ODU_PUT_MSG_BUF(mBuf);
                     break;
                  }
            }
            break;
         }
#endif/* End for TENB_T2K3K_SPECIFIC_CHANGES and L2_L3_SPLIT */
#ifndef UL_RLC_NET_CLUSTER
#ifdef TENB_STATS
      case ENTLWRMAC:
         {
            switch(pst->event)
            {
               case TENBSTATSINIT:
               {
                  
                  RlcCb *tRlcCb;
                  tRlcCb = RLC_GET_RLCCB(pst->dstInst);

                  TSL2SendStatsToApp(&(tRlcCb->genCfg.lmPst), 0);
                  ODU_PUT_MSG_BUF(mBuf);
                  break;
               }
               default:
               {
                  DU_LOG("\nERROR  -->  RLC_UL : Invalid Event[%d] from CL to PDCPUL\n", 
                         pst->event);
                  ODU_PUT_MSG_BUF(mBuf);
                  break;
               }
            }
            break;
         }
#endif
#endif
      default:
         {
          if (pst->dstInst < MAX_RLC_INSTANCES)
           {
              DU_LOG("\nERROR  -->  RLC_UL : Received Invalid Source Entity[%d]",pst->event);
           }
            ODU_PUT_MSG_BUF(mBuf);
            ret = RFAILED;
            break;
         }
    }
   ODU_EXIT_TASK();

   return (ret);
} /* kwActvTsk */


  
/********************************************************************30**
         End of file
**********************************************************************/
