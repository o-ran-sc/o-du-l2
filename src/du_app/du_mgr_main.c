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

/* This file is the entry point for DU APP */
#include "common_def.h"
#include "lrg.h"
#include "legtp.h"
#include "lrg.x"
#include "lkw.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_mgr_main.h"
#include "du_sctp.h"
#include "du_egtp.h"

#ifdef O1_ENABLE

#include "O1Interface.h"
#include "ConfigInterface.h"

#endif

uint8_t rlcUlActvTsk (Pst *, Buffer *);
uint8_t rlcUlActvInit (Ent, Inst, Region, Reason);
uint8_t rlcDlActvTsk (Pst *, Buffer *);
uint8_t rlcDlActvInit (Ent, Inst, Region, Reason);
uint8_t rgActvTsk (Pst *, Buffer *);
uint8_t rgActvInit (Ent, Inst, Region, Reason);
uint8_t lwrMacActvTsk(Pst *, Buffer *);
uint8_t lwrMacActvInit(Ent, Inst, Region, Reason);
#ifndef INTEL_WLS_MEM
uint8_t phyStubActvTsk(Pst *, Buffer *);
uint8_t phyStubActvInit(Ent, Inst, Region, Reason);
#endif

/* Global variable */
DuCfgParams duCfgParam;

/*******************************************************************
 *
 * @brief Initializes DU APP
 *
 * @details
 *
 *    Function : duAppInit
 *
 *    Functionality:
 *       - Registers and attaches TAPA tasks belonging to 
 *         DU_APP sys task
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duAppInit(SSTskId sysTskId)
{
   /* Register DU APP TAPA Task for DU */
   if(ODU_REG_TTSK((Ent)ENTDUAPP, (Inst)DU_INST, (Ttype)TTNORM, (Prior)PRIOR0,
      duActvInit, (ActvTsk)duActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach DU APP TAPA Task for DU */
   if (ODU_ATTACH_TTSK((Ent)ENTDUAPP, (Inst)0, sysTskId)!= ROK)
   {
      return RFAILED;
   }

   DU_LOG("\nINFO   -->  DU_APP : DU APP created and registered \
   to %d sys task", sysTskId);
   return ROK;
}

#ifdef O1_ENABLE
/*******************************************************************
 *
 * @brief Bring the cell Up
 *
 * @details
 *
 *    Function : bringCellUp
 *
 *    Functionality:
 *       - Bring the cell Up when requested from OAM
 *
 * @params[in] Cell Id
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool bringCellUp(uint16_t cellId)
{
   duProcCfgComplete();
   BuildAndSendF1SetupReq();
   return true;
}

/*******************************************************************
 *
 * @brief Bring the cell Down
 *
 * @details
 *
 *    Function : bringCellDown
 *
 *    Functionality:
 *       - Bring the cell Down when requested from OAM
 *
 * @params[in] Cell Id
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool bringCellDown(uint16_t cellId)
{
   BuildAndSendDUConfigUpdate(SERV_CELL_TO_DELETE);
   return true;
}
#endif

/*******************************************************************
 *
 * @brief Initializes EGTP
 *
 * @details
 *
 *    Function : egtpInit
 *
 *    Functionality:
 *       - Registers and attaches TAPA tasks belonging to 
 *         DU_APP sys task
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t egtpInit(SSTskId sysTskId)
{
   /* Register DU APP TAPA Task for DU */
   if(ODU_REG_TTSK((Ent)ENTEGTP, (Inst)EGTP_INST, (Ttype)TTNORM, (Prior)PRIOR0,
             egtpActvInit, (ActvTsk)egtpActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach DU APP TAPA Task for DU */
   if (ODU_ATTACH_TTSK((Ent)ENTEGTP, (Inst)0, sysTskId)!= ROK)
   {
      return RFAILED;
   }
 
    DU_LOG("\nINFO   -->  DU_APP : EGTP created and registered \
    to %d sys task", sysTskId);
    return ROK;
}
 

/*******************************************************************
 *
 * @brief Initializes SCTP task
 *
 * @details
 *
 *    Function : sctpInit
 *
 *    Functionality:
 *       - Registers and attaches TAPA tasks for SCTP receiver 
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpInit(SSTskId sysTskId)
{
   /* Register SCTP TAPA Task */
   if(ODU_REG_TTSK((Ent)ENTSCTP, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            sctpActvInit, (ActvTsk)sctpActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach SCTP TAPA Task */
   if (ODU_ATTACH_TTSK((Ent)ENTSCTP, (Inst)SCTP_INST, sysTskId)!= ROK)
   {
      return RFAILED;
   }

   DU_LOG("\nINFO   -->  DU_APP : SCTP TAPA task created and registered to %d sys task", 
         sysTskId);
   return ROK;
}
/*******************************************************************
 *
 * @brief Initializes RLC DL, MAC TAPA task
 *
 * @details
 *
 *    Function : rlcDlInit
 *
 *    Functionality:
 *       - Registers and attaches TAPA tasks for MAC and RLC DL
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcDlInit(SSTskId sysTskId)
{
   /* Register RLC DL TAPA Task */
   if(ODU_REG_TTSK((Ent)ENTRLC, (Inst)1, (Ttype)TTNORM, (Prior)PRIOR0,
            rlcDlActvInit, (ActvTsk)rlcDlActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach RLC DL Task */
   if (ODU_ATTACH_TTSK((Ent)ENTRLC, (Inst)1, sysTskId)!= ROK)
   {
      return RFAILED;
   }

   /* Register MAC TAPA Task */
   if(ODU_REG_TTSK((Ent)ENTMAC, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            rgActvInit, (ActvTsk)rgActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach MAC Task */
   if (ODU_ATTACH_TTSK((Ent)ENTMAC, (Inst)0, sysTskId)!= ROK)
   {
      return RFAILED;
   }

   DU_LOG("\nINFO   -->  DU_APP : RLC DL and MAC TAPA task created and registered to \
   %d sys task", sysTskId);
   return ROK;
}

/*******************************************************************
 *
 * @brief Initializes RLC UL TAPA task
 *
 * @details
 *
 *    Function : rlcUlInit
 *
 *    Functionality:
 *       - Registers and attaches TAPA task for RLC UL
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcUlInit(SSTskId sysTskId)
{
   /* Register RLC UL TAPA Task */
   if(ODU_REG_TTSK((Ent)ENTRLC, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            rlcUlActvInit, (ActvTsk)rlcUlActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach RLC DL Task */
   if (ODU_ATTACH_TTSK((Ent)ENTRLC, (Inst)0, sysTskId)!= ROK)
   {
      return RFAILED;
   }
   DU_LOG("\nINFO   -->  DU_APP : RLC UL TAPA task created and registered to \
   %d sys task", sysTskId);
   return ROK;
}

/*******************************************************************
 *
 * @brief Initializes Lower MAC receiver task
 *
 * @details
 *
 *    Function : lwrMacInit
 *
 *    Functionality:
 *       - Registers and attaches TAPA tasks for Lower MAC receiver
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t lwrMacInit(SSTskId sysTskId)
{
   /* Register SCTP TAPA Task */
   if(ODU_REG_TTSK((Ent)ENTLWRMAC, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            lwrMacActvInit, (ActvTsk)lwrMacActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach SCTP TAPA Task */
   if (ODU_ATTACH_TTSK((Ent)ENTLWRMAC, (Inst)0, sysTskId)!= ROK)
   {
      return RFAILED;
   }

   DU_LOG("\nINFO   -->  DU_APP : LWR MAC TAPA task created and registered to %d sys task",
         sysTskId);
   return ROK;
}

#ifndef INTEL_WLS_MEM
/*******************************************************************
 *
 * @brief Initializes Phy stub slot indication generator task
 *
 * @details
 *
 *    Function : phyStubInit
 *
 *    Functionality:
 *       - Registers and attaches TAPA tasks for Phy stub's slot
 *       indication generator
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t phyStubInit(SSTskId sysTskId)
{
   /* Register PHY stub slot indication TAPA Task */
   if(ODU_REG_TTSK((Ent)ENTPHYSTUB, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            phyStubActvInit, (ActvTsk)phyStubActvTsk) != ROK)
   {
      return RFAILED;
   }
   /* Attach Phy stub slot indication TAPA Task */
   if (ODU_ATTACH_TTSK((Ent)ENTPHYSTUB, (Inst)0, sysTskId)!= ROK)
   {
      return RFAILED;
   }

   DU_LOG("\nINFO   -->  DU_APP : PHY stub slot indication TAPA task created and registered to %d sys task",
         sysTskId);
   return ROK;
}
#endif

/*******************************************************************
 *
 * @brief Initializes system and TAPA tasks
 *
 * @details
 *
 *    Function : commonInit
 *
 *    Functionality:
 *       - Registers and attaches system and TAPA tasks
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t commonInit()
{
   /* Declare system task Ids */
   SSTskId du_app_stsk, egtp_stsk, sctp_stsk, rlc_ul_stsk, rlc_mac_cl_stsk, lwr_mac_stsk, phy_stub_slot_ind_stsk;

   pthread_attr_t attr;

   ODU_SET_PROC_ID(DU_PROC);

   /* Intel L1 using core 0-15. ODU-High using 16-21 */
   /* system task for DU APP */
   if(ODU_CREATE_TASK(PRIOR0, &du_app_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : System Task creation for DU APP failed");
      return RFAILED;
   }
   ODU_SET_THREAD_AFFINITY(&du_app_stsk, SS_AFFINITY_MODE_EXCL, 16, 0);

   /* system task for EGTP */
   if(ODU_CREATE_TASK(PRIOR0, &egtp_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : System Task creation for EGTP failed");
      return RFAILED;
   }
   ODU_SET_THREAD_AFFINITY(&egtp_stsk, SS_AFFINITY_MODE_EXCL, 17, 0);

   /* system task for RLC_DL and MAC */
   if(ODU_CREATE_TASK(PRIOR0, &rlc_mac_cl_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : System Task creation for RLC DL/MAC failed");
      return RFAILED;
   }
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   ODU_SET_THREAD_AFFINITY(&rlc_mac_cl_stsk, SS_AFFINITY_MODE_EXCL, 18, 0);

   /* system task for RLC UL */
   if(ODU_CREATE_TASK(PRIOR1, &rlc_ul_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : System Task creation for RLC UL failed");
      return RFAILED;
   }
   ODU_SET_THREAD_AFFINITY(&rlc_ul_stsk, SS_AFFINITY_MODE_EXCL, 19, 0);

   /* system task for SCTP receiver thread */
   if(ODU_CREATE_TASK(PRIOR0, &sctp_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : System Task creation for SCTP failed");
      return RFAILED;
   }
   ODU_SET_THREAD_AFFINITY(&sctp_stsk, SS_AFFINITY_MODE_EXCL, 20, 0);

   /* system task for lower-mac receiver thread */
   if(ODU_CREATE_TASK(PRIOR0, &lwr_mac_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : System Task creation for Lower MAC failed");
      return RFAILED;
   }
   ODU_SET_THREAD_AFFINITY(&lwr_mac_stsk, SS_AFFINITY_MODE_EXCL, 21, 0);

#ifndef INTEL_WLS_MEM
   /* system task for phy stub's slot indication generator thread */
   if(ODU_CREATE_TASK(PRIOR0, &phy_stub_slot_ind_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : System Task creation for Phy stub slot indication generator failed. MAX STSK [%d]", SS_MAX_STSKS);
      return RFAILED;
   }

#endif

   /* Create TAPA tasks */
   if(duAppInit(du_app_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : DU APP TAPA Task initialization failed");
      return RFAILED;
   }

   if(egtpInit(egtp_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : EGTP TAPA Task initialization failed");
      return RFAILED;
   }

   if(sctpInit(sctp_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : SCTP TAPA Task initialization failed");
      return RFAILED;
   }

   if(rlcDlInit(rlc_mac_cl_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : RLC DL Tapa Task initialization failed");
      return RFAILED;
   } 

   if(rlcUlInit(rlc_ul_stsk) != ROK)
   {
     DU_LOG("\nERROR  -->  DU_APP : RLC UL Tapa Task initialization failed");
     return RFAILED;
   } 

   if(lwrMacInit(lwr_mac_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Lower MAC Tapa Task initialization failed");
      return RFAILED;
   }

#ifndef INTEL_WLS_MEM
   if(phyStubInit(phy_stub_slot_ind_stsk) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : PHY stub slot indication Tapa Task initialization failed");
      return RFAILED;
   }
#endif

   return ROK;
}

/*******************************************************************
 *
 * @brief Initializes the DU
 *
 * @details
 *
 *    Function : duInit
 *
 *    Functionality:
 *       - Calls commonInit
 *       - Registers DU Layers
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duInit()
{
   int ret = ROK;
   if(commonInit() != ROK)
   {
      ret = RFAILED;
   } 
   return ret;
}

void init_log()
{
	openlog("ODU",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

/*******************************************************************
 *
 * @brief Entry point for the DU APP 
 *
 * @details
 *
 *    Function : main
 *
 *    Functionality:
 *      - Read config params into duCfgParams
 *      - Initiate the init functions
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t tst(void)
{
   init_log();

#ifdef O1_ENABLE
   if(start_O1_module() != ROK)
      return RFAILED;
#endif
   //Initialize TAPA layers
   if(duInit() != ROK)
   {
      return RFAILED;
   } 

   //Read all the configs from du_utils.c into duCfgParams
   duReadCfg();

#ifdef O1_ENABLE
   //Send VES PNF registration message to SMO
   sendPnfRegistration();
#endif

   return ROK;

}/* end of main()*/

/**********************************************************************
         End of file
**********************************************************************/
