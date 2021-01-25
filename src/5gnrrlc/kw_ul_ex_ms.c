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
  
        Name:    LTE-RLC Layer - System Services Interface Functions
    
        Type:    C file
  
        Desc:    C source code for the interface to System Services
                  of LTE-RLC
 
        File:    kw_ul_ex_ms.c
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RLC_UL";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=206;

/** @file kw_ul_ex_ms.c
@brief RLC System Services Interface
*/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_ul.h"
#include "kw_udx.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#include "kw.x"
#include "kw_ul.x"
#include "kw_udx.x"

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
 *    This function is invoked by system services to initialize the LTE-RLC
 *    layer. This is an entry point used by LTE_RLC layer to initialize its
 *    global variables, before becoming operational.
 *
 *    Allowable values for parameters are specified in ssi.h.
 *
 *    @param[in] ent    - Specify the entity id of the LTE-RLC task.
 *    @param[in] inst   - Specify the entity id of the LTE-RLC task.
 *    @param[in] region - Specifies the memory region from which
 *                         LTE-RLC should allocate structures and buffers.
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
   TSL2AllocStatsMem(tRlcCb->init.region, tRlcCb->init.pool); 
#endif

   return ROK;
} /* rlcUlActvInit */


/**
 *
 * @brief
 *
 *  <b> Activation Task </b>
 *
 *  @b Description:
 *  Processes events received for MLTE-RLC layer via System Services from
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
                     RLOG1(L_FATAL,"Received Invalid Event[%d] from SM",
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
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from RLC UL",
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
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from RRC", 
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
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from PDCP", 
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
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from MAC",
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
                  printf("\n ERROR Invalid Event[%d] from CL to PDCPUL\n", 
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
              RLOG1(L_ERROR, "Received Invalid Source Entity[%d]",pst->event);
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
