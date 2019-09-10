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

   Name:    LTE-PDCP Layer - System Services Interface Functions

   Type:    C file

   Desc:    C source code for the interface to System Services of LTE-PDCP

   File:    pj_dl_ex_ms.c

*********************************************************************21*/
  
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=217;
/** @file pj_dl_ex_ms.c
  @brief PDCP System Services Interface
 */

/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "kwu.h"           /* KWU defines */
#include "cpj.h"           /* CPJ defines */
#include "ctf.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#include "pj_err.h"        /* Err defines */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_udx.h"
#include "pj_dl.h"
#include "pj_lib.h"
#include "pj_ptsec.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "kwu.x"           /* KWU */
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#include "pj.x"
#include "pj_udx.x"
#include "pj_dl.x"
#include "pj_ul.x"
#include "pj_lib.x"
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
PUBLIC S16 pjBatchProcTick(Void);
EXTERN S16 pjRBufFreeMsg(void);
#endif
#ifdef LC_EGTP_THREAD
#include "ss_rbuf.h"
#include "ss_rbuf.x"
EXTERN void egWrReadRingBuf(SsRngBufId ringId);
#endif

PUBLIC U32 ysPjPrcTtiInd = 0;   /* UDAY */

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
 *    This function is invoked by system services to initialize the LTE-PDCP
 *    layer. This is an entry point used by LTE_PDCP layer to initialize its
 *    global variables, before becoming operational.
 *
 *    Allowable values for parameters are specified in ssi.h.
 *
 *    @param[in] ent    - Specify the entity id of the LTE-RLC task.
 *    @param[in] inst   - Specify the entity id of the LTE-RLC task.
 *    @param[in] region - Specifies the memory region from which
 *                         LTE-PDCP should allocate structures and buffers.
 *    @param[in] reason - Specifies the reason for calling this
 *                         initialization function.
 *
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlActvInit
(
 Ent    ent,                 /* entity */
 Inst   inst,                /* instance */
 Region region,              /* region */
 Reason reason               /* reason */
 )
#else
PUBLIC S16 pjDlActvInit(ent, inst, region, reason)
  Ent    ent;                 /* entity */
  Inst   inst;                /* instance */
  Region region;              /* region */
  Reason reason;              /* reason */
#endif
{
   PjCb   *tPjCb;
   TRC3(pjDlActvInit)
   if (inst >= PJ_MAX_PDCP_INSTANCES)
   {
       /* intance greater than MAX instances */ 
       RETVALUE(RFAILED); 
   }

   if (pjCb[inst] != NULLP)
   {
      RETVALUE (RFAILED);
   }

   if (SGetSBuf(region, 0, (Data **)&tPjCb,
            (Size)sizeof (PjCb)) != ROK)
   {
      RETVALUE(RFAILED);
   }
   /* Initialize kwCb */
   PJ_MEM_SET(tPjCb, 0, sizeof(PjCb));

   /* Initialize task configuration parameters */
   /* We will take only entity name */
   tPjCb->init.ent     = ent;           /* entity */
   tPjCb->init.inst    = inst;          /* instance */
   tPjCb->init.region  = region;        /* static region */
   tPjCb->init.pool    = 0;             /* static pool */
   tPjCb->init.reason  = reason;        /* reason */
   tPjCb->init.cfgDone = FALSE;         /* configuration done */
   tPjCb->init.acnt    = TRUE;          /* enable accounting */
   tPjCb->init.usta    = TRUE;          /* enable unsolicited status */
   tPjCb->init.trc     = FALSE;         /* enable trace */
#ifdef DEBUGP
   tPjCb->init.dbgMask = 0x00000000;
#endif
   tPjCb->init.procId  = SFndProcId();

   pjCb[inst] = tPjCb;

  RETVALUE(ROK);
} /* pjActvInit */


/**
 *
 * @brief
 *
 *  <b> Activation Task </b>
 *
 *  @b Description:
 *  Processes events received for LTE-PDCP layer via System Services from
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
PUBLIC S16 pjDlActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 pjDlActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

   TRC3(pjDlActvTsk);

   switch(pst->srcEnt)
   {
      case ENTSM:
         {
            switch(pst->event)
            {
#ifdef LCLPJ
               case LPJ_EVT_CFG_REQ:
                  {
                     ret = cmUnpkLpjCfgReq(PjMiLpjCfgReq, pst, mBuf);
                     break;
                  }

               case LPJ_EVT_CNTRL_REQ:
                  {
                     ret = cmUnpkLpjCntrlReq(PjMiLpjCntrlReq, pst, mBuf);
                     break;
                  }

               case LPJ_EVT_STS_REQ:
                  {
                     ret = cmUnpkLpjStsReq(PjMiLpjStsReq, pst, mBuf);
                     break;
                  }

               case LPJ_EVT_STA_REQ:
                  {
                     ret = cmUnpkLpjStaReq(PjMiLpjStaReq, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */

               default:
                  {
                     RLOG0(L_ERROR, "Received Invalid Event from SM");
                     ret = RFAILED;
                     break;
                  }
#endif  /* LCLPJ */
            }
            break;
         }

      case ENTPJ:
         {
            switch(pst->event)
            {
#ifdef LCUDX
            case EVTUDXBNDREQ:
                  {
                     cmUnpkPjUdxBndReq(PjDlUdxBndReq, pst, mBuf);
                     break;
                  }

            case EVTUDXUBNDREQ:
                  {
                     cmUnpkPjUdxUbndReq(PjDlUdxUbndReq, pst, mBuf);
                     break;
                  }

            case EVTUDXCFGREQ:
                  {
                     cmUnpkPjUdxCfgReq(PjDlUdxCfgReq, pst, mBuf);
                     break;
                  }

            case EVTUDXULSTAREP:
                  {
                     cmUnpkPjUdxUlStaRep(PjDlUdxUlStaRep, pst, mBuf);
                     break;
                  }

            case EVTUDXDLSTAREP:
                  {
                     cmUnpkPjUdxDlStaRep(PjDlUdxDlStaRep, pst, mBuf);
                     break;
                  }            

            case EVTUDXULROHCFDB:
                  {
                      cmUnpkPjUdxUlFdbkPkt(PjDlUdxUlFdbkPktInfo, pst, mBuf);
                      break;
                  }

            case EVTUDXDLROHCFDB:
                  {
                      cmUnpkPjUdxDlFdbkPkt(PjDlUdxDlFdbkPktInfo, pst, mBuf);
                      break;
                  }
            
            case EVTUDXREESTREQ:
                  {
                     cmUnpkPjUdxReEstReq(PjDlUdxReEstReq, pst, mBuf);
                     break;
                  }

            case EVTUDXSECCFGREQ:
                  {
                     cmUnpkPjUdxSecCfgReq(PjDlUdxSecCfgReq, pst, mBuf);
                     break;
                  }

            case EVTUDXUEIDCHGREQ:
                  {
                     cmUnpkPjUdxUeIdChgReq(PjDlUdxUeIdChgReq, pst, mBuf);
                     break;
                  }

            case EVTUDXCOUNTREQ:
                  {
                     cmUnpkPjUdxCountReq(PjDlUdxCountReq, pst, mBuf);
                     break;
                  }

            case EVTUDXSDUSTAREQ:
                  {
                     cmUnpkPjUdxSduStaReq(PjDlUdxSduStaReq, pst, mBuf);
                     break;
                  }
             case EVTUDXDATRESUME:
                  {
#ifdef FLAT_BUFFER_OPT
                       cmUnpkPjUdxDatResumeReq(PjDlUdxDatResumeReq, pst, mBuf);
#else
                       cmUnpkPjUdxDatResumeReq(PjDlUdxDatResumeReq, pst, mBuf);
#endif
                       break;
                  }
             case EVTUDXDATFRWD:
                  {
#ifdef FLAT_BUFFER_OPT
                     printf("Received Event EVTUDXDATFRWD at PDCP DL \n");
                     cmUnpkPjUdxDatForwardReq(PjDlUdxDatForwardReqFB, pst, mBuf);
#else
                     cmUnpkPjUdxDatForwardReq(PjDlUdxDatForwardReq, pst, mBuf);
#endif
                     break;
                  }
#ifdef LTE_L2_MEAS
               case PJ_UDX_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkPjUdxL2MeasReq(PjDlUdxL2MeasReq, pst, mBuf);
                     break;
                  }
               case PJ_UDX_EVT_L2MEAS_SEND_REQ:
                 {

                    ret = cmUnpkPjUdxL2MeasSendReq(PjDlUdxL2MeasSendReq, pst, mBuf); 
  
                     break;
                 }
               case PJ_UDX_EVT_L2MEAS_STOP_REQ:
                 {
                     ret = cmUnpkPjUdxL2MeasStopReq(PjDlUdxL2MeasStopReq, pst, mBuf);
                     break;
                 }
#endif
               default:
                  {
                     RLOG0(L_ERROR, "Received Invalid Event from PDCP UL");
                     ret = RFAILED;
                     break;
                  }
#endif  /* LCLPJ */
            }
            break;
         }


      case ENTNH:
         {
            switch(pst->event)
            {
#ifdef LCPJU
               case EVTPJUBNDREQ:
                  {
                     cmUnpkPjuBndReq(PjUiPjuBndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUUBNDREQ:
                  {
                     cmUnpkPjuUbndReq(PjUiPjuUbndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATREQ:
                  {
                     cmUnpkPjuDatReq(PjUiPjuDatReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATFWDREQ:
                  {
                     cmUnpkPjuDatFwdReq(PjUiPjuDatFwdReq, pst, mBuf);
                     break;
                  }
#endif /* LCPJU */
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, " Received Invalid Event From RRC");
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTPX:
         {
            switch(pst->event)
            {
#ifdef LCPJU
               case EVTPJUBNDREQ:
                  {
                     cmUnpkPjuBndReq(PjUiPjuBndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUUBNDREQ:
                  {
                     cmUnpkPjuUbndReq(PjUiPjuUbndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATREQ:
                  {
#ifdef SS_RBUF
                     cmUnpkFpPjuDatReq(PjUiPjuDatReq, pst, mBuf);
#else
                     cmUnpkPjuDatReq(PjUiPjuDatReq, pst, mBuf);
#endif
                     break;
                  }

               case EVTPJUDATFWDREQ:
                  {
                     cmUnpkPjuDatFwdReq(PjUiPjuDatFwdReq, pst, mBuf);
                     break;
                  }
#endif
#ifdef PCAP_IP_DATGEN_LOAD_5GTF
               case EVTPCAPIPDATIND:
                  {
	             //printf("ENTPX!!!!EVTPCAPIPDATIND:\n\r ");
                     //RLOG0(L_ERROR, " Received Invalid Event from Relay");
                     wrDamPcapDatInd(mBuf);
		     break;
                  }
#endif
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, " Received Invalid Event from Relay");
                  ret = RFAILED;
                  break;
            }
            break;
         }

      case ENTOC:
         {
            switch(pst->event)
            {
#ifdef PJ_CMP_ASYNC
               /* Compression Confirmations */
               case OBD_EVT_CMP_INIT_CFM:
                  ret = cmUnpkLibObdCmpInitCfm(PjLibObdCmpInitCfm, pst,mBuf);
                  break;

               case OBD_EVT_CMP_CFM:
                  ret = cmUnpkLibObdCmpCfm(PjLibObdCmpCfm, pst, mBuf);
                  break;

#endif /* PJ_CMP_ASYNC */
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, "Received Invalid Event from Offboard Ciphering");
                  ret = RFAILED;
                  break;
            }
            break;
         }

      case ENTOS:
         {
            switch(pst->event)
            {
#ifdef PJ_SEC_ASYNC
                  /* Integrity Confirmations */
               case OBD_EVT_INT_INIT_CFM:
                  ret = cmUnpkLibObdIntInitCfm(PjLibObdIntInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_INT_PROT_CFM:
                  ret = cmUnpkLibObdIntProtCfm(PjLibObdIntProtCfm, pst, mBuf);
                  break;

                  /* Ciphering Confirmations */
               case OBD_EVT_CP_INIT_CFM:
                  ret = cmUnpkLibObdCpInitCfm(PjLibObdCpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_UP_INIT_CFM:
                  ret = cmUnpkLibObdUpInitCfm(PjLibObdUpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_CIPHER_CFM:
                  ret = cmUnpkLibObdCipherCfm(PjLibObdCipherCfm, pst, mBuf);
                  break;

#endif /* PJ_SEC_ASYNC */
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, "Received Invalid Event from Offboard Security");
                  ret = RFAILED;
                  break;
            }
            break;
         }
      case ENTKW:
         {
            switch(pst->event)
            {
#ifdef LCKWU
               case KWU_EVT_BND_CFM:     /* Bind request */
                  {
                     ret = cmUnpkKwuBndCfm(PjLiKwuBndCfm, pst, mBuf );
                     break;
                  }

                case KWU_EVT_DAT_CFM:    /* Dedicated Channel Data request */
                  {
                     ret = cmUnpkKwuDatCfm(PjLiKwuDatCfm, pst, mBuf);
                     break;
                  }

               case KWU_EVT_DISC_SDU_REQ:    /* Coomon Channel Status Response */
                  {
                     ret = cmUnpkKwuDiscSduReq(PjLiKwuDiscSduReq, pst , mBuf);
                     break;
                  }

               case KWU_EVT_STA_IND:    /* Dedicated Channel Status Response */
                  {
                     ret = cmUnpkKwuStaInd(PjLiKwuStaInd, pst, mBuf);
                     break;
                  }
               case KWU_EVT_DISC_SDU_CFM:    /* Harq status indication */
                  {
                     ret = cmUnpkKwuDiscSduCfm(PjLiKwuDiscSduCfm, pst, mBuf);
                     break;
                  }
               case KWU_EVT_FLOW_CNTRL_IND:    /* Flow control indication*/
                  {
                     ret = cmUnpkKwuFlowCntrlInd(PjLiKwuFlowCntrlInd, pst, mBuf);
                     break;
                  }
               default:
                  {
                     SPutMsg(mBuf);
                     RLOG0(L_ERROR, "Received Invalid Event from RLC");
                     ret = RFAILED;
                     break;
                   }

#endif  /* LCRGU */
            }
            break;
         }
#ifdef TENB_AS_SECURITY
      case ENTYS:
         {
            switch(pst->event)
            {
               case EVTPJUTTIIND:
               {
#ifndef SS_RBUF
                  static U32 ttitick = 0;
#endif

                  ysPjPrcTtiInd++; /* UDAY */
                  SPutMsg(mBuf);
                  /*Trigger batching of the packets queued for ciphering
                   * and deciphering*/
                  /*Note: UL trigger should be posted to UL thread if UL
                   * and DL are on different threads.*/
#ifndef L2_L3_SPLIT
#ifdef MAC_RLC_UL_RBUF
                        kwUlBatchProc();
#endif 
#endif 
#ifdef XEON_SPECIFIC_CHANGES 
#ifdef INTEL_QAT_DP
                        PjCb  *tPjCb = PJ_GET_PJCB(pst->dstInst);
                        pjPollQatDat(tPjCb,ttitick);
#endif /* INTEL_QAT_DP */
#else
#ifndef SS_RBUF
                  pjProcessSpaccRdWr(ttitick);
#endif
#endif
#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
#ifndef UL_RLC_NET_CLUSTER
                  pjUlBatchProcSplit();
#endif
#endif
#ifndef SS_RBUF
                  ttitick++;
#endif
#ifdef LC_EGTP_THREAD
                  egWrReadRingBuf(SS_RNG_BUF_EGTP_FREE_RING);
#endif
                  break;
               } 
            }
            break;
         }
#endif
#ifdef SS_RBUF
      case ENTTF:
      {
            switch(pst->event)
            {
               case EVTCTFBTCHPROCTICK:
               {
                  pjBatchProcTick();
                  break;
               }
               case EVTCTFNRTRBUFCLEAN:
               {
                  pjRBufFreeMsg();
               }
             }
         SPutMsg(mBuf);
         break;
      }
#endif
      default:
         {
            RLOG0(L_ERROR, "Recieved Invalid Source Entity ");
            SPutMsg(mBuf);
            ret = RFAILED;
            break;
         }
   }

   SExitTsk();

   RETVALUE(ret);
} /* pjActvTsk */

  
/********************************************************************30**
         End of file
**********************************************************************/
