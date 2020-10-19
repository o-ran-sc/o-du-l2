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
 
        File:    kw_dl_ex_ms.c
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RLC_DL";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=195;

/** @filekw_dl_ex_ms.c 
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
#include "kw_udx.h"
#include "kw_dl.h"


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"
#include "du_app_rlc_inf.h"
#include "rlc_mac_inf.h"

#include "ctf.h"
S16 rlcUtlDlBatchProcPkts(Void);
S16 rlcDlBatchProc(Void);
#if (defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS))
uint32_t isDatReqProcessed;
void rlcUtlDlBatchProcHqStaInd ARGS ((Void));
#endif

#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
EXTERN S16 rlcDlBatchProcSplit  ARGS((Void));
#endif
//UDAY
#ifdef L2_OPTMZ
uint32_t rlcAmmStaPduList[512] = {0};
EXTERN S16 ssGetDBufOfSize ARGS((Region region, Size size, Buffer **dBuf));
#endif
S16 rlcDlInitExt ARGS (( Void ));

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
S16 rlcDlInitExt 
(
)
#else
S16 rlcDlInitExt()
#endif
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
#ifdef ANSI
S16 rlcDlActvInit
(
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
)
#else
S16 rlcDlActvInit(ent, inst, region, reason)
Ent    ent;                 /* entity */
Inst   inst;                /* instance */
Region region;              /* region */
Reason reason;              /* reason */
#endif
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

//UDAY
#ifdef L2_OPTMZ
   for(int i = 0; i < 512; i++)
   {
      Buffer *mBuf = NULL;
      Buffer *bufPtr = NULL;
      SGetMsg(1, 0 , &mBuf);
      ssGetDBufOfSize(1 , 1800, &bufPtr);
      SUpdMsg(mBuf, bufPtr, 0);
      rlcAmmStaPduList[i] = (uint32_t)mBuf; 
   }
#endif
   /* call external function for intialization */
   /*
   kwInitExt();
   */

   

   return ROK;
} /* kwActvInit */


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
#if (defined (MAC_FREE_RING_BUF) || defined (RLC_FREE_RING_BUF))
pthread_t gRlcTId = 0;
#endif
#ifdef ANSI
S16 rlcDlActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
S16 rlcDlActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

#ifdef RLC_FREE_RING_BUF
   gRlcTId = pthread_self();
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
#endif  /* LCLKW */

#ifdef LCKWU
               case KWU_EVT_DAT_REQ:              /* Data request */
                  {
                     //ret = cmUnpkKwuDatReq(rlcProcDlData, pst, mBuf);
                     break;
                  }
#endif /* LCKWU */
               
               case EVENT_DL_RRC_MSG_TRANS_TO_RLC:
	          {
		     ret = unpackDlRrcMsgToRlc(RlcProcDlRrcMsgTransfer, pst, mBuf);
		     break;
		  }
               default:
                  ODU_PUT_MSG_BUF(mBuf);
                  if (pst->dstInst < MAX_RLC_INSTANCES)
                  {
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from SM",
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
               case UDX_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkUdxBndReq(rlcDlUdxBndReq, pst, mBuf );
                     break;
                  }

               case UDX_EVT_UBND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkUdxUbndReq(rlcDlUdxUbndReq, pst, mBuf );
                     break;
                  }
               case UDX_EVT_CFG_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkUdxCfgReq(rlcDlUdxCfgReq, pst, mBuf );
                     break;
                  }

               case UDX_EVT_UEIDCHG_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkUdxUeIdChgReq(rlcDlUdxUeIdChgReq, pst, mBuf);
                     break;
                  }

               case UDX_EVT_STA_UPD_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkUdxStaUpdReq(rlcDlUdxStaUpdReq, pst, mBuf);
                     break;
                  }

               case UDX_EVT_STA_PDU_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkUdxStaPduReq(rlcDlUdxStaPduReq, pst, mBuf);
                     break;
                  }

#ifdef LTE_L2_MEAS
               case UDX_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkUdxL2MeasReq(rlcDlUdxL2MeasReq, pst, mBuf);
                     break;
                  }
               case UDX_EVT_L2MEAS_SEND_REQ:
                 {

                    ret = cmUnpkUdxL2MeasSendReq(rlcDlUdxL2MeasSendReq, pst, mBuf); 
  
                     break;
                 }
               case UDX_EVT_L2MEAS_STOP_REQ:
                 {
                     ret = cmUnpkUdxL2MeasStopReq(rlcDlUdxL2MeasStopReq, pst, mBuf);
                     break;
                 }
#endif

#endif  /* LCCKW */
               case UDX_EVT_DL_CLEANUP_MEM:
                  {
                     rlcUtlFreeDlMemory(RLC_GET_RLCCB(pst->dstInst));
                     break;
                  }
               
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
#ifdef L2_L3_SPLIT
               case KWU_EVT_CPLANE_DAT_REQ:       /* C-Plane Data request */
                  {
                     ret = cmUnpkKwuDatReq(rlcProcDlData, pst, mBuf);
                     break;
                  }
#else
               case KWU_EVT_DAT_REQ:              /* Data request */
                  {
                     //ret = cmUnpkKwuDatReq(rlcProcDlData, pst, mBuf);
                     break;
                  }
#endif
               case KWU_EVT_DISC_SDU_REQ:         /* Discard SDU request */
                  {
                     ret = cmUnpkKwuDiscSduReq(RlcUiKwuDiscSduReq, pst, mBuf);
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
#ifdef L2_L3_SPLIT
               case KWU_EVT_CPLANE_DAT_REQ:       /* C-Plane Data request */
               case KWU_EVT_UPLANE_DAT_REQ:       /* U-Plane Data request */
                  {
                     ret = cmUnpkKwuDatReq(rlcProcDlData, pst, mBuf);
                     break;
                  }
#else
               case KWU_EVT_DAT_REQ:              /* Data request */
                  {
                     //ret = cmUnpkKwuDatReq(rlcProcDlData, pst, mBuf);
                     break;
                  }
#endif
               case KWU_EVT_DISC_SDU_REQ:         /* Discard SDU request */
                  {
                     ret = cmUnpkKwuDiscSduReq(RlcUiKwuDiscSduReq, pst, mBuf);
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
               case EVENT_SCHED_RESULT_TO_RLC: 
                  {
                     ret = unpackSchedResultRpt(RlcProcSchedResultRpt, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
               case EVTRGUHQSTAIND:    /* Harq status indication */
                  {
                     ret = cmUnpkRguHqStaInd(RlcLiRguHqStaInd, pst, mBuf);
                     break;
                  }
#endif
               case EVTRGUFLOWCNTRLIND:
                  {
                     ret = cmUnpkRguFlowCntrlInd(RlcLiRguFlowCntrlInd,pst,mBuf);
                     break;
                  }   
#endif  /* LCRGU */
#ifdef RLC_STA_PROC_IN_MAC/* RLC Status PDU Processing */
               case UDX_EVT_STA_UPD_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkUdxStaUpdReq(rlcDlUdxStaUpdReq, pst, mBuf);
                     break;
                  }
#endif

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
#ifdef SS_RBUF 
      case ENTLWRMAC:
      {
            switch(pst->event)
            {
               case EVTCTFBTCHPROCTICK:
               {
                  rlcUtlDlBatchProcPkts();
                  break;
               }
             }
         ODU_PUT_MSG_BUF(mBuf);
         break;
      }
#endif
      case ENTYS:
         {
            switch(pst->event)
            {
               case KWU_EVT_TTI_IND:
                  {
#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
                     rlcDlBatchProcSplit();
#else 
#if defined(PDCP_RLC_DL_RBUF)
                     rlcDlBatchProc();
#endif
#endif

#if (defined(SPLIT_RLC_DL_TASK) && defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS))
                     //RlcDlHarqStaBatchProc();
                     rlcUtlDlBatchProcHqStaInd();
#endif 
#ifndef KWSELFPSTDLCLEAN
                     /* Revanth_chg */
                     /* Moving Cleanup from self post event to TTI event */
                     rlcUtlFreeDlMem();
#endif 

                     ODU_PUT_MSG_BUF(mBuf);
                     break;
                  }
            }
            break;
         }


      default:
         {
            if (pst->dstInst < MAX_RLC_INSTANCES)
            {
               /*RlcCb *tRlcCb = RLC_GET_RLCCB(pst->dstInst);*/
               RLOG1(L_ERROR, "Received Invalid Source Entity[%d]",
                     pst->event);
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
