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

#include "du_app_rlc_inf.h"

#ifdef TENB_STATS 
#include "l2_tenb_stats.x"   
#endif

PUBLIC S16 rlcUlInitExt ARGS (( Void ));

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
  
#ifdef ANSI
PUBLIC S16 rlcUlInitExt 
(
)
#else
PUBLIC S16 rlcUlInitExt()
#endif
{
   TRC2(rlcUlInitExt);

   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 rlcUlActvInit
(
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
)
#else
PUBLIC S16 rlcUlActvInit(ent, inst, region, reason)
Ent    ent;                 /* entity */
Inst   inst;                /* instance */
Region region;              /* region */
Reason reason;              /* reason */
#endif
{
   RlcCb    *tRlcCb;
   TRC3(rlcUlActvInit)

   if (inst >= MAX_RLC_INSTANCES)
   {
       /* intance greater than MAX instances */ 
       RETVALUE(RFAILED); 
   }

   if (rlcCb[inst] != NULLP)
   {
       RETVALUE (RFAILED);
   }
  
   if (SGetSBuf(region, 0, (Data **)&tRlcCb,
                (Size)sizeof (RlcCb)) != ROK)    
   {                     
      RETVALUE(RFAILED);
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
   tRlcCb->init.procId  = SFndProcId();

   rlcCb[inst] = tRlcCb;

   /* call external function for intialization */
   /*
   kwInitExt();
   */
#ifdef TENB_STATS 
   TSL2AllocStatsMem(tRlcCb->init.region, tRlcCb->init.pool); 
#endif

   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 rlcUlActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 rlcUlActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

   TRC3(kwActvTsk);

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            switch(pst->event)
            {
#ifdef LCLKW
               case LKW_EVT_CFG_REQ:
                  {
                     ret = cmUnpkLrlcCfgReq(KwMiLrlcCfgReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_CNTRL_REQ:
                  {
                     ret = cmUnpkLkwCntrlReq(KwMiLkwCntrlReq, pst, mBuf);
                     break;
                  }
               
               case EVENT_RLC_UL_UE_CREATE_REQ:        /* UE Create Request */
                  {
                     ret = unpackRlcUlUeCreateReq(RlcUlProcUeCreateReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_STS_REQ:
                  {
                     ret = cmUnpkLkwStsReq(KwMiLkwStsReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_STA_REQ:
                  {
                     ret = cmUnpkLkwStaReq(KwMiLkwStaReq, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
               case LKW_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkLkwL2MeasReq(KwMiLkwL2MeasReq, pst, mBuf);
                     break;
                  }
               case LKW_EVT_L2MEAS_SEND_REQ:
                 {

                    ret = cmUnpkLkwL2MeasSendReq(KwMiLkwL2MeasSendReq, pst, mBuf); 
  
                     break;
                 }
               case LKW_EVT_L2MEAS_STOP_REQ:
                 {
                     ret = cmUnpkLkwL2MeasStopReq(KwMiLkwL2MeasStopReq, pst, mBuf);
                     break;
                 }
#endif
#endif  /* LCLKW */
               default:
                  SPutMsg(mBuf);
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

      case ENTKW:
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
                  SPutMsg(mBuf);
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
                     ret = cmUnpkCkwBndReq(KwUiCkwBndReq, pst, mBuf );
                     break;
                  }

               case CKW_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkCkwUbndReq(KwUiCkwUbndReq, pst, mBuf );
                     break;
                  }

               case CKW_EVT_UEIDCHG_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkCkwUeIdChgReq(KwUiCkwUeIdChgReq, pst, mBuf);
                     break;
                  }

#endif  /* LCCKW */

#ifdef LCKWU
               case KWU_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkKwuBndReq(KwUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(KwUiKwuUbndReq, pst, mBuf );
                     break;
                  }
#endif  /* LCKWU */
               default:
                  SPutMsg(mBuf);
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
                     ret = cmUnpkKwuBndReq(KwUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(KwUiKwuUbndReq, pst, mBuf );
                     break;
                  }

               default:
                  SPutMsg(mBuf);
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

      case ENTRG:
         {
            switch(pst->event)
            {
#ifdef LCRGU
               case EVTRGUBNDCFM:     /* Bind request */
                  {
                     ret = cmUnpkRguBndCfm(KwLiRguBndCfm, pst, mBuf );
                     break;
                  }

               case EVTRLCULDAT:    /* Dedicated Channel Data request */
                  {
                     ret = unpackRcvdUlData(RlcMacProcUlData, pst, mBuf);
                     break;
                  }

#endif  /* LCRGU */

               default:
                  SPutMsg(mBuf);
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
                     SPutMsg(mBuf);
                     break;
                  }
            }
            break;
         }
#endif/* End for TENB_T2K3K_SPECIFIC_CHANGES and L2_L3_SPLIT */
#ifndef UL_RLC_NET_CLUSTER
#ifdef TENB_STATS
      case ENTTF:
         {
            switch(pst->event)
            {
               case TENBSTATSINIT:
               {
                  
                  RlcCb *tRlcCb;
                  tRlcCb = RLC_GET_RLCCB(pst->dstInst);

                  TSL2SendStatsToApp(&(tRlcCb->genCfg.lmPst), 0);
                  SPutMsg(mBuf);
                  break;
               }
               default:
               {
                  printf("\n ERROR Invalid Event[%d] from CL to PDCPUL\n", 
                         pst->event);
                  SPutMsg(mBuf);
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
            SPutMsg(mBuf);
            ret = RFAILED;
            break;
         }
    }
   SExitTsk();

   RETVALUE(ret);
} /* kwActvTsk */


  
/********************************************************************30**
         End of file
**********************************************************************/
