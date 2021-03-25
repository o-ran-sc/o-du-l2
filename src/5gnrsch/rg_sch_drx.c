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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for DRX realted functions 
  
     File:     rg_sch_drx.c
  
**********************************************************************/

/** @file rg_sch_drx.c
@brief This file implements the DRX processing .
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#ifdef LTEMAC_PH3_HDFDD
#include "rg_sch_hdfdd.h"
#endif /*LTEMAC_PH3_HDFDD*/
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#ifdef LTEMAC_PH3_HDFDD
#include "rg_sch_hdfdd.x"
#endif /*LTEMAC_PH3_HDFDD*/

#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"

 /**
 *  @file rg_sch_drx.c This file gives the describes the design for DRX feature. 
 *
 * DRX is Discontinuous Reception i.e. the UE in order to save some battery
 * would not monitor (decode) PDCCHs at all times. Instead the UE is configured
 * with a periodically occuring duration wherein it shall monitor PDCCHs. The UE
 * can be called ACTIVE at this time, this time is a minimum of a configurable
 * value - onDuration and a maximum of Infinity. 
 *
 * ACTIVE time  MIN (onDuration)  MAX (infinity)
 *
 * A sample configuration is periodicity of 10 subframes (ms) and an offset
 * value of 3. This can be represented as the diagram given below. The portion
 * marked as ACTIVE is the onDuration and the UE monitors PDCCHs. 
 *
 * @code
 *
 *          <-ACTIVE-><---IN-ACTIVE------><-ACTIVE-><---IN-ACTIVE--
 * 
 * |__|__|__|--------|__|__|__|__|__|__|__|--------|__|__|__|__|__|
 *  0   1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
 *
 * @endcode
 */
#ifdef LTE_TDD
/******************************************************************************
 *                         Structure definitions for TDD                      *
 ******************************************************************************/

/** @brief : No of DL subframes in a particular TDD configuration
 *
 *  @details : Special subframes in TDD can carry PDCCH if configured
 *             for DwPTS. For normal CP with subframe configruation (0-8)
 *             & extended CP with subframe configuration (0-6), Special
 *             Subframes can carry PDCCH and are represented in row 0.
 *             Extended CP with subframe configuraton (7-8), which do 
 *             not carry PDCCH are represented in row 1. 
 *             rgSchDrxDlSfTddCfg[1][2] represent number of DL subframes
 *             in TDD config 2 where DwPTS can carry PDCCH and 
 *             rgSchDrxDlSfTddCfg[0][2] represent number of DL subframes
 *             in TDD config 2 where no DwPTS exits.
 */

static uint8_t rgSchDrxDlSfTddCfg[RGSCH_MAX_SFCFG][RGSCH_MAX_TDD_CFG] = 
{
   {2,4,6,6,7,8,3},
   {4,6,8,7,8,9,5}
};

/** @brief : No of DL subframes till next SFN from a particular subframe 
 *
 *  @details :For a given Special subframe config
 *            (refer rgSchDrxDlSfTddCfg for an explanation) and given
 *            TDD config, how many DL subframes till Next SFN from this
 *            subframe onwards.
 *   
 */

static uint8_t rgSchDrxDLSfTillNxtSFN[RGSCH_MAX_SFCFG][RGSCH_MAX_TDD_CFG]
                            [RGSCH_NUM_SUB_FRAMES]= 
{
   {
      {2,1,1,1,1,1,0,0,0,0},
      {4,3,3,3,3,2,1,1,1,1},
      {6,5,5,5,4,3,2,2,2,1},
      {6,5,5,5,5,5,4,3,2,1},
      {7,6,6,6,6,5,4,3,2,1},
      {8,7,7,7,6,5,4,3,2,1},
      {3,2,2,2,2,2,1,1,1,1}

   },
   { 
      {4,3,2,2,2,2,1,0,0,0},
      {6,5,4,4,4,3,2,1,1,1},
      {8,7,6,6,5,4,3,2,1,1},
      {7,6,5,5,5,5,4,3,2,1},
      {8,7,6,6,6,5,4,3,2,1},
      {9,8,7,7,6,5,4,3,2,1},
      {5,4,3,3,3,3,2,1,1,1}
   }
}; /*rgSchDrxDLSfTillNxtSFN*/


/** @brief : Lookup table for DL subframe given the number of subframes 
 *
 *  @details :For a given Special subframe config
 *            (refer rgSchDrxDlSfTddCfg for an explanation) and given
 *            TDD config, the DL subframe index given the number of subframes 
 *   
 */

static uint8_t rgSchDrxDLSftoDLSfIdx[RGSCH_MAX_SFCFG][RGSCH_MAX_TDD_CFG]
                         [RGSCH_NUM_SUB_FRAMES]= 
{
   {
      {5,0},
      {9,0,4,5},
      {9,0,3,4,5,8},
      {9,0,5,6,7,8},
      {9,0,4,5,6,7,8},
      {9,0,3,4,5,6,7,8},
      {9,0,5}
   },
   { 
      {6,0,1,5},
      {9,0,1,4,5,6},
      {9,0,1,3,4,5,6,8},
      {9,0,1,5,6,7,8},
      {9,0,1,4,5,6,7,8},
      {9,0,1,3,4,5,6,7,8},
      {9,0,1,5,6}
   }
};/* rgSchdrxDLSftoDLSfIdx*/
/* ccpu00134196-[Add]-DRX retx timer changes */
/* The k+4 th subframe in TDD at which HARQ RTT expires may be an Uplink SF.
   In such case, the drx retx timer may start at the next pdcch sf instead
   of at k+4 itself */
uint8_t rgSchDrxHarqRetxFirstPsf[RGSCH_MAX_TDD_CFG][RGSCH_NUM_SUB_FRAMES] = {
  {0, 0, 4, 0, 6, 0, 0, 4, 0, 6}, 
  {0, 0, 4, 6, 0, 0, 0, 4, 6, 0}, 
  {0, 0, 4, 0, 0, 0, 0, 4, 0, 0}, 
  {0, 0, 4, 4, 4, 0, 0, 0, 0, 0}, 
  {0, 0, 4, 4, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 4, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 4, 6, 5, 0, 0, 4, 7, 0}, 
};
#endif  /* LTE_TDD */

/******************************************************************************
 *                         Start of Function declarations                     *
 ******************************************************************************/
static Void rgSCHDrxTtiHdlOnDurUl ARGS((
RgSchCellCb        *cell,
uint16_t                ulIndex
));
static Void rgSCHDrxTtiHdlOnDurDl ARGS((
RgSchCellCb    *cell,
uint16_t                dlIndex
));
static Void rgSCHDrxTtiHdlDlHarqRTT ARGS((
RgSchCellCb          *cell,
uint16_t                   dlIndex
));
static Void rgSCHDrxTtiHdlUlHarqRTT ARGS((
RgSchCellCb          *cell,
uint16_t                   ulIndex
));
static S16 rgSCHDrxTtiHdlOnDur ARGS((RgSchCellCb *cellCb, uint16_t dlIndex, 
                                        uint16_t ulIndex));
static S16 rgSCHDrxTtiHdlInActv ARGS((RgSchCellCb *cellCb, uint16_t dlIndex,
                                        uint16_t ulIndex));
static S16 rgSCHDrxTtiHdlShortCycle ARGS((RgSchCellCb *cell, uint16_t dlIndex, 
                                       uint16_t ulIndex));
static S16 rgSCHDrxTtiHdlDlHarq ARGS((RgSchCellCb *cellCb, uint16_t dlIndex,
                                       uint16_t ulIndex));
static S16 rgSCHDrxCpyUeCfg ARGS((RgSchDrxUeCb *drxCb,
                                     RgrUeDrxCfg* ueDrxCfg));
 
static S16 rgSCHDrxGetNxtOnDur ARGS((RgSchCellCb* cell,RgSchDrxUeCb* drxCb,
                                       CmLteTimingInfo* nxtOnDur,
                                       uint8_t delta));
 
static Void rgSCHDrxMvToNxtOnDurOcc ARGS((RgSchCellCb* cell,
                                           RgSchUeCb* ue,
                                           uint16_t      idx,
                                           Bool    calcFrmOffst));
#ifdef LTE_TDD
static Void rgSCHDrxCalcNxtTmrExpry ARGS((RgSchCellCb     *cell,
                                           RgSchUeCb       *ue,
                                           uint16_t              delta,
                                           uint32_t              tmrLen,
                                           S16             *distance,
                                           uint16_t             *idx
                                          ));
static S16 rgSCHDrxGetNxtTmrExpry ARGS((RgSchCellCb *cell,uint16_t curTime,
                                          uint32_t duration, 
                                         CmLteTimingInfo* tmrExpryIdx));
#endif /* LTE_TDD */
#ifdef EMTC_ENABLE
S16 rgSCHEmtcDrxCpyUeCfg
(
 RgSchUeCb     *ueCb,
 RgrUeDrxCfg   *drxCfg
);
S16 rgSCHDrxTtiHdlUlHarq 
(
RgSchCellCb          *cell,
uint16_t                   dlIndex,
uint16_t                   ulIndex
);
Void rgSCHDrxUeUlHqReset
(
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgUeUlHqCb   *hqE
);

#endif

/** @brief This function handles the per TTI handling of the DRX module. 
 *   Invoked by rgSCHTti @sa rgSCHTti. 
 *
 * @details This function goes through the drxQ and marks the UE as ACTIVE or
 * INACTIVE based on the timers and thier status.
 *
 *     Function: rgSCHDrxTtiInd 
 *
 *         Processing steps:
 *         - Processing is divided into respective timer handling. 
 *         - Calculate the DL and UL indices as follows 
 *         @code
 *             dlIndex = (cell->crntTime.sfn * 10 + cell->crntTime.slot +
 *                   RG_SCH_DRX_DL_DELTA) % RG_SCH_MAX_DRXQ_SIZE;
 *
 *             ulIndex = (cell->crntTime.sfn * 10 + cell->crntTime.slot +
 *                   RG_SCH_DRX_UL_DELTA) % RG_SCH_MAX_DRXQ_SIZE;
 *         @endcode
 *         - Call rgSCHDrxTtiHdlOnDur to handle onDurationTimer handling
 *         - Call rgSCHDrxTtiHdlInActv to handle drx-InactivityTimer handling
 *         - Call rgSCHDrxTtiHdlShortCycle  to handle Short cycle timer.
 *         - Call rgSCHDrxTtiHdlDlHarq  to handle HARQ RTT and drx-retransmission
 *         timers.
 *         - Call rgSchDrxTtiHdlUlHarq to handle Uplink HARQ processing -
 *         related to ACTIVITY when a UE is expecting a grant for an uplink
 *         re-transmissions.
 *         - Post this processing the ueCb->dlInactvMask's DRX Bit shall be set
 *         or reset to denote ACTIVITY or INACTIVITY respectively.
 *         - Post this processing the ueCb->ulInactvMask's DRX Bit shall be set
 *         or reset to denote ACTIVITY or INACTIVITY respectively.
 *         - Add the UE to the list of Active/inactive UEs.
 *
 * @param RgSchCellCb *cell
 * @param [out] CmLListCp *dlInactvLst List to which the DL in-active UEs are 
 *                            added*
 * @return 
 */
Void rgSCHDrxTtiInd(RgSchCellCb  *cell)
{
   uint16_t dlIndex;
   uint16_t ulIndex;

   dlIndex = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot +
         RG_SCH_DRX_DL_DELTA) % RG_SCH_MAX_DRXQ_SIZE;

   ulIndex = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot +
         RG_SCH_DRX_UL_DELTA) % RG_SCH_MAX_DRXQ_SIZE; 

#ifdef LTEMAC_R9
   rgSCHDrxTtiHdlDlHarq (cell, dlIndex, ulIndex);
   /* checks the Ul-Retransmission timer */
#ifdef EMTC_ENABLE
   if(cell->emtcEnable)
   {
      rgSCHDrxTtiHdlUlHarq (cell, dlIndex, ulIndex);
   }
#endif 
   rgSCHDrxTtiHdlInActv(cell, dlIndex, ulIndex);
   
   /*Process Short cycle expiry before On duration timer so that long cycles
    * On Duration can be processed if timer has expired*/
   rgSCHDrxTtiHdlShortCycle (cell, dlIndex, ulIndex);
   rgSCHDrxTtiHdlOnDur(cell, dlIndex, ulIndex);

#else /*LTEMAC_R9*/
   rgSCHDrxTtiHdlOnDur(cell, dlIndex, ulIndex);
   rgSCHDrxTtiHdlDlHarq (cell, dlIndex, ulIndex);
   /* checks the Ul-Retransmission timer */
#ifdef EMTC_ENABLE
   if(cell->emtcEnable)
   {
      rgSCHDrxTtiHdlUlHarq (cell, dlIndex, ulIndex);
   }
#endif
   rgSCHDrxTtiHdlInActv(cell, dlIndex, ulIndex);
   rgSCHDrxTtiHdlShortCycle (cell, dlIndex, ulIndex);

#endif /*LTEMAC_R9*/

   return;

}/*rgSCHDrxTtiInd*/

/** @brief This function is called to handle onDurationTimer per TTI processing.
 *
 * @details
 *    Invoked by - rgSCHDrxTtiInd 
 *
 *     Function:  rgSCHDrxTtiHdlOnDur
 *
 *         Processing steps:
 *
 *           - OnDurationTimer is handled using the drxQ @sa RgSchDrxQ
 *
 *           - For Downlink we shall look at an index that is 
 *             n + RG_SCH_DRX_DL_DELTA. 
 *
 *
 *             - For Uplink we shall look at an index that is 
 *               n + RG_SCH_DRX_UL_DELTA as
 *             we are concerned with the PDCCH and not the actual PUSCH.
 *
 *
 * @param  RgSchCellCb  *cellCb
 * @param  uint16_t          dlIndex
 * @param  uint16_t          ulIndex
 * @return ROK/RFAILED 
 */

static S16 rgSCHDrxTtiHdlOnDur(RgSchCellCb *cell,uint16_t dlIndex,uint16_t ulIndex)
{

#if ( ERRCLASS & ERRCLS_INT_PAR )
   if ( cell == (RgSchCellCb* )NULLP )
   {
      return RFAILED;
   }
#endif

   rgSCHDrxTtiHdlOnDurDl(cell,dlIndex);
  
   rgSCHDrxTtiHdlOnDurUl(cell, ulIndex);

   return ROK;

}/*rgSCHDrxTtiHdlOnDur*/


/** @brief This function handles the processing for drxInactityTimer per TTI
 *
 * @details
 * Invoked by - rgSCHDrxTtiInd 
 *
 *     Function: rgSCHDrxTtiHdlInActv
 *
 *         Processing steps:
 *
 *         - For Downlink we shall look at an index that is 
 *             n + RG_SCH_DRX_DL_DELTA of the drxQ. 
 *
 *         - MARK UE AS INACTIVE BASED ON DRX-INACTIVITY TIMER EXPIRY
 *
 *
 *            - For Uplink we shall look at an index that is 
 *              n + RG_SCH_DRX_UL_DELTA as
 *             we are concerned with the PDCCH and not the actual PUSCH.
 *
 *
 * @param  RgSchCellCb *cellCb
 * @param  uint16_t          dlIndex
 * @param  uint16_t          ulIndex
 * @return ROK/RFAILED 
 */

S16 rgSCHDrxTtiHdlInActv(RgSchCellCb  *cell,uint16_t dlIndex,uint16_t ulIndex)
{
   CmLList           *node;
   RgSchDRXCellCb    *drxCell=NULLP;
   RgSchUeCb         *ue=NULLP;
   RgSchDrxUeCb      *drxUe=NULLP;
   uint16_t                shrtCycleExpIndx;
   CmLListCp         dlInactvLst; /* list of UE's becoming DL-inactive */
   CmLListCp         ulInactvLst; /* list of UE's becoming UL-inactive */
   RgSchCmnCell      *cellSch = NULLP;
   Bool              delInUlScan = FALSE;

#if ( ERRCLASS & ERRCLS_INT_PAR )   
   if ( cell == (RgSchCellCb* )NULLP) 
   {
      return RFAILED;
   }
#endif


   drxCell     = (cell->drxCb);
   delInUlScan = drxCell->delInUlScan;
  
  
   /***********************************************************
    *    Scanning inActvitiyQ in DL
    ***********************************************************/

   /* The DL loop will scan for UE's whose inactivity timer has
    * expired. It will switch the cycle to short or long based
    * on the cycle configured.
    * Furhter,if !delInUlScan, then will remove the UE from the
    * inactivity q. 
    */

   node = drxCell->drxQ[dlIndex].inActvTmrQ.first;

   /* Initialize DL inactive list */
   cmLListInit(&dlInactvLst);
   
   /* Initialize UL inactive list */
   cmLListInit(&ulInactvLst);
   
   while (node)
   {
      ue    = (RgSchUeCb*)node->node;
      node  = node->next;
      drxUe = RG_SCH_DRX_GET_UE(ue);

      if ( delInUlScan == TRUE)
      {
         drxUe->drxInactDistance--;
      }

      if ( drxUe->drxInactDistance != DRX_TMR_EXPRD ) 
      {
         continue;
      }

      /* UE is inactive as inactivity timer has expired */
      drxUe->drxDlInactvMask  |= RG_SCH_DRX_INACTVTMR_BITMASK;


      /* update the ue mask only if no condition in drx
       * is keeping ue active
       */
      if ( !RG_SCH_DRX_DL_IS_UE_ACTIVE(drxUe))
      {
         /* set the UE has DRX inactive */
         ue->dl.dlInactvMask |= RG_DRX_INACTIVE;

         /* Add to DL inactive list */
         cmLListAdd2Tail(&dlInactvLst,&(ue->dlDrxInactvLnk));
         ue->dlDrxInactvLnk.node = (PTR)ue;
      }

      /*Remove from the queue if !delInUlScan */
      if( delInUlScan == FALSE )
      {
         cmLListDelFrm(&(drxCell->drxQ[dlIndex].inActvTmrQ), 
               &(drxUe->inActvTmrEnt));

         drxUe->drxInactvIndx = DRX_INVALID;

      }/* if (delInUlScan == FALSE) */

      if (drxUe->isShortCycleCfgd)
      {
         /* add shorty cycle expirty */
         drxUe->isLongCycle = FALSE;

         shrtCycleExpIndx = (dlIndex + (drxUe->shortCycleTmrLen *
                  drxUe->shortDrxCycle)) % RG_SCH_MAX_DRXQ_SIZE;

         drxUe->drxShortCycleDistance = (drxUe->shortCycleTmrLen *
               drxUe->shortDrxCycle) / RG_SCH_MAX_DRXQ_SIZE;

         /*Remove the UE from existing index*/
         if (drxUe->shortCycleIndx != DRX_INVALID)
         {
            cmLListDelFrm(&(drxCell->drxQ[drxUe->shortCycleIndx].shortCycleQ),
                  &(drxUe->shortCycleEnt));
         }

         cmLListAdd2Tail(&(drxCell->drxQ[shrtCycleExpIndx].shortCycleQ),
               &(drxUe->shortCycleEnt));

         drxUe->shortCycleEnt.node = (PTR)ue;
         drxUe->shortCycleIndx     = shrtCycleExpIndx;

         /*Calculate onDuration again & move the
          * ueCb to the next Onduration occurence
          */

         /*we maybe at any position in the RF timeline,
          * need to calculate onDuration from the starting
          * offset
          */
         rgSCHDrxMvToNxtOnDurOcc(cell,ue,RG_SCH_DRX_DL_DELTA,TRUE);

      }/*isShortCycleCfgd */
      else
      {
         /* use the long cycle */
         drxUe->isLongCycle = TRUE;
      }/*isLongCycle*/
   }/*while(node) */



   /***********************************************************
    *    Scanning inActvitiyQ in UL
    ***********************************************************/

   /* The UL loop will scan for UE's whose inactivity timer has
    * expired and mark the UE's UL inactive.
    * Furhter,if delInUlScan, then will remove the UE from the
    * inactivity q. 
    */

   /* For Uplink we shall look at an index that is n + RG_SCH_DRX_UL_DELTA as
      we are concerned with the PDCCH and not the actual PUSCH.*/



   node = drxCell->drxQ[ulIndex].inActvTmrQ.first;


   while (node)
   {
      ue    = (RgSchUeCb*)node->node;
      node  = node->next;
      drxUe = RG_SCH_DRX_GET_UE(ue);

      if ( delInUlScan == FALSE)
      {
         drxUe->drxInactDistance--;
      }
      
      if ( drxUe->drxInactDistance != DRX_TMR_EXPRD ) 
      {
        continue;
      }

      /* Need to mark the UE as inactive due to expiry of
       * DRX inactivity timer */

      /* UE is inactive as inactivity timer has expired */
      drxUe->drxUlInactvMask  |= RG_SCH_DRX_INACTVTMR_BITMASK;

      /* update the ue mask only if no other condition in
       * drx is keeping ue active */

      if (!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
      {
         /* set the inactivity bit */
         ue->ul.ulInactvMask |= RG_DRX_INACTIVE;

         /* Add to Ul inactive list */
         cmLListAdd2Tail(&ulInactvLst,&(ue->ulDrxInactvLnk));
         ue->ulDrxInactvLnk.node = (PTR)ue;
      }

      if ( delInUlScan == TRUE)
      {
         /* remove from queue */
         cmLListDelFrm(&(drxCell->drxQ[ulIndex].inActvTmrQ), 
               &(drxUe->inActvTmrEnt));

         drxUe->drxInactvIndx = DRX_INVALID;
         
      }/* if ( delInUlScan == TRUE) */
   }/*while(node) for uplink */

   
   /* Send the list to the scheduler to mark UE as inactive in UL*/
    cellSch = RG_SCH_CMN_GET_CELL(cell);
    cellSch->apisUl->rgSCHUlInactvtUes(cell, &ulInactvLst);
   
   /* Send the DL inactive list to the scheduler to mark UE as inactive */
   cellSch = RG_SCH_CMN_GET_CELL(cell);
   cellSch->apisDl->rgSCHDlInactvtUes(cell,&dlInactvLst);

      return ROK;
}/*rgSCHDrxTtiHdlInActv*/

 /** @brief This function handles the per TTI processing for DRX short cycle
  * timer.
  *
  * @details
  * Invoked by - rgSCHDrxTtiInd 
  *
  *     Function: rgSCHDrxTtiHdlShortCycle
  *
  *         Processing steps:
  *         -  For downlink
  *
 *             in addition we have to mark the ues based on drx short cycle
 *             expiry
 *
 *
 * @param  RgSchCellCb    *cell
 * @param  uint16_t          dlIndex
 * @param  uint16_t          ulIndex
 * @return ROK/RFAILED 
 */

S16 rgSCHDrxTtiHdlShortCycle(RgSchCellCb  *cell,uint16_t dlIndex,uint16_t ulIndex)
{
   CmLList           *node;
   RgSchDRXCellCb    *drxCell=NULLP;
   RgSchUeCb         *ue=NULLP;
   RgSchDrxUeCb      *drxUe=NULLP;

#if ( ERRCLASS & ERRCLS_INT_PAR )
   if ( cell == (RgSchCellCb* )NULLP )
   {
      return RFAILED;
   }
#endif

   UNUSED(ulIndex);


   drxCell = RG_SCH_DRX_GET_CELL(cell);

   node = drxCell->drxQ[dlIndex].shortCycleQ.first;

   while (node)
   {
      ue    = (RgSchUeCb*)node->node;
      node  = node->next;
      drxUe = RG_SCH_DRX_GET_UE(ue);

      if ( --drxUe->drxShortCycleDistance != DRX_TMR_EXPRD)
      {
         continue;
      }
      
      /* mark the UE's current cycle to be long */
      drxUe->isLongCycle = TRUE;

      /* remove from the shortCycleQ */

      cmLListDelFrm(&(drxCell->drxQ[dlIndex].shortCycleQ), 
            &(drxUe->shortCycleEnt));
      drxUe->shortCycleIndx = DRX_INVALID;

      /* Remove from onDuration queue inserted based on short cycle
       * and calculate onDuration based on long cycle.*/
      rgSCHDrxMvToNxtOnDurOcc(cell,ue,RG_SCH_DRX_DL_DELTA,TRUE);
   }/*while(node)...*/

   return ROK;
}/*rgSCHDrxTtiHdlShortCycle*/


 /** @brief This function handles the HARQ timer's processing per TTI.
  *
  * @details
  * Invoked by - rgSCHDrxTtiInd 
  *
  *     Function: rgSCHDrxTtiHdlDlHarq 
  *
  *         Processing steps:
  *         - In addition per TTI DRX module must look at Downlink HARQ queues
 *             maintained to track HARQ RTT timer and drx-RetransmissionTimer.
 *             Every TTI at the scheduling index we shall check these queues and
 *             process accordingly. 
 *
 *
 *             - Though these timers are related to downlink HARQ processing, they
 *             have an impact on uplink scheduling. The reason is that the UE,
 *             if active for downlink scheduling implies that it is reading
 *             PDCCHs i.e. we can still send uplink allocations to the UE. Hence
 *             every TTI Uplink too would look at the harqRTTQ and harqRetxQ. 
 *
 *
 *
 * @param  RgSchCellCb *cellCb
 * @param  uint16_t          dlIndex
 * @param  uint16_t          ulIndex
  * @return ROK/RFAILED 
  */

static S16 rgSCHDrxTtiHdlDlHarq(RgSchCellCb *cell,uint16_t dlIndex,uint16_t ulIndex)
{

#if ( ERRCLASS & ERRCLS_INT_PAR)
   if ( cell == (RgSchCellCb *)NULLP )
   {
      return RFAILED;
   }
#endif /*ERRCLASS & ERRCLS_INT_PAR*/


   rgSCHDrxTtiHdlDlHarqRTT(cell, dlIndex);

   rgSCHDrxTtiHdlUlHarqRTT(cell, ulIndex);

   return ROK;
}

 /** @brief This function is called by the common scheduler as part of
  * finalization of allocations in downlink.
  *
  * @details
  * Invoked by - 
  *
  *     Function: rgSchDrxStrtInActvTmr
  *     
  *     This function is responsible to starting drx-InactivityTimer 
  *
  *         Processing steps:
  *
  *
  * @param  RgSchCellCb *cell
  * @param  CmLListCp   *ueUlLst
  * @param  uint8_t           direction 
  * @return Void 
  */

Void rgSCHDrxStrtInActvTmr(RgSchCellCb  *cell,CmLListCp *ueLst,uint8_t direction)
{
   CmLList         *node;
   CmLList         *delNode;
   RgSchUeCb       *ueCb;
   RgSchDrxUeCb    *ueDrxCb;
#ifndef LTE_TDD
   uint16_t        index1;
#endif
   uint16_t        inActvTmrExpIndx;
#ifndef LTE_TDD
   uint16_t        curTimeInSf; /* current time in number of subframes */
#endif
#ifdef LTE_TDD
   uint16_t        delta;
#endif /*LTE_TDD*/
   CmLListCp         dlInactvLst; /* list of UE's becoming DL-inactive */
   CmLListCp         ulInactvLst; /* list of UE's becoming UL-inactive */
   RgSchCmnCell      *cellSch = NULLP;
   Bool              delInUlScan = FALSE;

   if ( direction == RG_SCH_DRX_UL)
   {
#ifndef LTE_TDD
      curTimeInSf = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G) +
         (cell->crntTime.slot) +RG_SCH_DRX_UL_DELTA;
#endif

#ifdef LTE_TDD
      delta = RG_SCH_DRX_UL_DELTA;
#endif /*LTE_TDD */
   }
   else
   {
#ifndef LTE_TDD
      curTimeInSf = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G) +
         (cell->crntTime.slot) + RG_SCH_DRX_DL_DELTA; 
#endif

#ifdef LTE_TDD
      delta = RG_SCH_DRX_DL_DELTA;
#endif /*LTE_TDD */
   }


   /* Initialize DL inactive list */
   cmLListInit(&dlInactvLst);

   /* Initialize UL inactive list */
   cmLListInit(&ulInactvLst);
   
   delInUlScan = cell->drxCb->delInUlScan; 

#ifndef LTE_TDD
   index1 = (curTimeInSf) % RG_SCH_MAX_DRXQ_SIZE;
#endif

   node = ueLst->first;

   while(node)
   {
      ueCb    = (RgSchUeCb *)node->node;
      ueDrxCb = ueCb->drxCb;

      /* Stop inactivity timer */
      if ( ueDrxCb->drxInactvIndx != DRX_INVALID )
      {
         cmLListDelFrm(&(cell->drxCb->drxQ[ueDrxCb->drxInactvIndx].inActvTmrQ),
               &(ueDrxCb->inActvTmrEnt));
      }
#ifdef LTE_TDD

      rgSCHDrxCalcNxtTmrExpry(cell,
            ueCb,
            delta,
            ueDrxCb->inactvtyTmrLen,
            &(ueDrxCb->drxInactDistance),
            &inActvTmrExpIndx
            );

#else /*LTE_TDD*/
      inActvTmrExpIndx = (index1 + ueDrxCb->inactvtyTmrLen) 
         % RG_SCH_MAX_DRXQ_SIZE;

      ueDrxCb->drxInactDistance = ueDrxCb->inactvtyTmrLen 
         / RG_SCH_MAX_DRXQ_SIZE;
#endif /*LTE_TDD*/

      cmLListAdd2Tail(&(cell->drxCb->drxQ[inActvTmrExpIndx].inActvTmrQ),
            &(ueDrxCb->inActvTmrEnt));

      ueDrxCb->inActvTmrEnt.node = (PTR)ueCb;

      ueDrxCb->drxInactvIndx     = inActvTmrExpIndx;

      /* Update DRX InActive both masks */
      ueDrxCb->drxUlInactvMask &= ~RG_SCH_DRX_INACTVTMR_BITMASK;
      ueDrxCb->drxDlInactvMask &= ~RG_SCH_DRX_INACTVTMR_BITMASK;

      /* Update UE's Inactive masks */
      ueCb->ul.ulInactvMask &= ~RG_DRX_INACTIVE;
      ueCb->dl.dlInactvMask &= ~RG_DRX_INACTIVE;

      if ( delInUlScan == TRUE) 
      {
         if ( ueDrxCb->inactvtyTmrLen == RGR_DRX_PRD_1PSF)
         {
            ueDrxCb->drxInactDistance = DRX_TMR_EXPRD;
            ueDrxCb->drxDlInactvMask |= RG_SCH_DRX_INACTVTMR_BITMASK;

            /* if no other condition is keeping ue inactive,
             * inactivate ue 
             */
            if ( !RG_SCH_DRX_DL_IS_UE_ACTIVE(ueDrxCb) )
            {
               ueCb->dl.dlInactvMask |= RG_DRX_INACTIVE;

               /* Add to DL inactive list */
               cmLListAdd2Tail(&dlInactvLst,&(ueCb->dlDrxInactvLnk));
               ueCb->dlDrxInactvLnk.node = (PTR)ueCb;
            }
         }/*if ( ueDrxCb->inactvyTmrLen...*/

      }/*delInUlScan==TRUE*/
      else
      {
         if ( ueDrxCb->inactvtyTmrLen == RGR_DRX_PRD_1PSF ) 
         {
            ueDrxCb->drxInactDistance = DRX_TMR_EXPRD;
            ueDrxCb->drxUlInactvMask |= RG_SCH_DRX_INACTVTMR_BITMASK;
            /* if no other condition is keeping ue inactive,
             * inactivate ue 
             */
            if ( !RG_SCH_DRX_DL_IS_UE_ACTIVE(ueDrxCb) )
            {
               ueCb->ul.ulInactvMask |= RG_DRX_INACTIVE;

               if ( !RG_SCH_CMN_UL_IS_UE_ACTIVE(ueCb))
               {
                  /* Add to UL inactive list */
                  cmLListAdd2Tail(&ulInactvLst,&(ueCb->ulDrxInactvLnk));
                  ueCb->ulDrxInactvLnk.node = (PTR)ueCb;
               }
            }/*if ( !RG_SCH_DRX....)*/
         }/*if (ueDrxCb->inactv...*/
      }

      /* move the link list forward */
      delNode = node;
      node = node->next;

      cmLListDelFrm(ueLst, delNode);
      delNode->node = NULLP;

   }/*node*/

   /* Send the list to the scheduler to mark UE as inactive in UL*/
   cellSch = RG_SCH_CMN_GET_CELL(cell);
   cellSch->apisUl->rgSCHUlInactvtUes(cell, &ulInactvLst);

   /* Send the DL inactive list to the scheduler to mark UE as inactive */
   cellSch = RG_SCH_CMN_GET_CELL(cell);
   cellSch->apisDl->rgSCHDlInactvtUes(cell,&dlInactvLst);

   return; 
}/*rgSCHDrxStrtInActvTmr*/

 /** @brief This function is called by the downlink HARQ module on receiving a
  * negative feedback from the UE for a PDSCH transmission. 
  *
  * @details
  * Invoked by - rgSCHDhmHqTrnsFail
  *
  *     Function: rgSCHDrxStartHarqRTTTmr
  *
  *         Processing steps:
 *
  *
  * @param  RgSchCellCb       *cell
  * @param  RgSchDlHqProcCb   *dlHq
  * @param  uint8_t                tbCnt
  * @return Void 
  */
Void rgSCHDrxStartHarqRTTTmr(RgSchCellCb  *cell,RgSchDlHqProcCb *hqP,uint8_t tbCnt)
{
   RgSchDRXCellCb      *drxCell =NULLP;
   RgSchDrxDlHqProcCb  *drxHq   =NULLP;
   uint16_t            harqRTTExpIndx;
   uint8_t             fdbkDelta;
#ifdef LTE_TDD   
   uint8_t             firstDlTxOcassion;
   uint8_t             drxRetxTmrStartSf;
#endif    

   drxCell = RG_SCH_DRX_GET_CELL(cell);
   drxHq   = RG_SCH_DRX_GET_DL_HQ(hqP);

#ifdef LTE_TDD
    /* ccpu00134196-[Add]-DRX retx timer changes */
   firstDlTxOcassion = rgSchDrxHarqRetxFirstPsf[cell->ulDlCfgIdx]
                           [hqP->tbInfo[tbCnt].fdbkTime.subframe];
                            
   harqRTTExpIndx = ((hqP->tbInfo[tbCnt].fdbkTime.sfn * 10) +
         hqP->tbInfo[tbCnt].fdbkTime.subframe + firstDlTxOcassion)
      % RG_SCH_MAX_DRXQ_SIZE;

   fdbkDelta = RGSCH_CALC_SF_DIFF(cell->crntTime,  hqP->tbInfo[tbCnt].fdbkTime);
#else /*LTE_TDD*/

   /* For FDD HARQ RTT expiry index is 8 subframes from the time 
    * corresponding PDSCH was scheduled. We are adding 1 subframe 
    * so that UE is scheduled for retransmission in the next subframe*/
   /* ccpu00134196-[Add]-DRX retx timer changes */
   harqRTTExpIndx = ((hqP->tbInfo[tbCnt].timingInfo.sfn * RGSCH_NUM_SUB_FRAMES_5G) +
         hqP->tbInfo[tbCnt].timingInfo.slot + RG_SCH_MIN_HARQ_RTT)
      % RG_SCH_MAX_DRXQ_SIZE;

   fdbkDelta = RGSCH_CALC_SF_DIFF(cell->crntTime,  hqP->tbInfo[tbCnt].timingInfo);
#endif /*LTE_TDD*/
   /* ccpu00134196-[Add]-DRX retx timer changes */ 
   /* ensure that the insertion into the queue happens at an index
      greater than the dl index, ie, do +1 */
   /* Instead of depending on TTI details of current time and HARQ RTT Expire 
    * time, Handling this check with deltas, because with TTIs it is not possible
    * to handle wrap-around condition*/
#ifdef LTE_TDD
   if(fdbkDelta + RG_SCH_DRX_DL_DELTA >= firstDlTxOcassion)
   {
      /* The retx timer length should be reduced.  
         This means based on the platforms delta between the DL HARQ 
         processing and DL scheduling, drx retx timer lengths of 1ms/2ms 
         may not be possible */ 
      drxRetxTmrStartSf = (hqP->tbInfo[tbCnt].fdbkTime.subframe + 
                                 firstDlTxOcassion) % RGSCH_NUM_SUB_FRAMES;
      uint8_t i;
      /* We are here because the Retx Timer start is moved by atleast one 
         position. Hence the timer will be reduced by minimum one */ 
      drxHq->retxTmrReduction = 1;

      /* Now check the consecutive subframes starting from the actual 
         starting position of the retx tmr till the new position. Reduce the
         timer value only if the sf is a Pdcch sf */
      for(i = 1; i <= fdbkDelta + RG_SCH_DRX_DL_DELTA-firstDlTxOcassion+ 1; i++)
      {
         if (rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx]
                    [(drxRetxTmrStartSf+i)%RGSCH_NUM_SUB_FRAMES] 
                  != RG_SCH_TDD_UL_SUBFRAME)
          {
             drxHq->retxTmrReduction++;
          }
      }
#else                  
   if(fdbkDelta + RG_SCH_DRX_DL_DELTA >= RG_SCH_MIN_HARQ_RTT)
   {
      drxHq->retxTmrReduction = 
         fdbkDelta + RG_SCH_DRX_DL_DELTA - RG_SCH_MIN_HARQ_RTT+ 1;
#endif      
      /* KW_FIX */
      harqRTTExpIndx = (harqRTTExpIndx + drxHq->retxTmrReduction) % 
         RG_SCH_MAX_DRXQ_SIZE;
   }
   else
   {
      drxHq->retxTmrReduction = 0;
   }
   cmLListAdd2Tail (&(drxCell->drxQ[harqRTTExpIndx].harqRTTQ),
         &(drxHq->harqRTTEnt));

   drxHq->harqRTTEnt.node = (PTR)hqP;
   drxHq->rttIndx         = harqRTTExpIndx;

   return;

}/*rgSCHDrxStartHarqRTTTmr*/


/** @brief This function is called by the Configuration module to give a
 * trigger to DRX module for UE configuration. 
 *
 * @details
 *
 *     Function: rgSCHDrxUeCfg 
 *
 *         Processing steps:
 *         - Copy configuration information into drxUe structure.
 *         - Calculate the first occurance of onDuration based on values
 *         provided in the configuration structure. 
 *         - Append the UE to the onDurationQ at that index. 
 *         - The UE must be appened to the list based on the timing calculated
 *         above (nxtSfn, nxtSubframe). 
 *
 * @param  RgSchCellCb  *cell Cell control block.
 * @param  RgSchUeCb    *ue   UE control block.
 * @param  RgrUeCfg     *ueCfg RGR UE configuration information.
 * @return 
 * -# ROK
 * -# RFAILED
 */
S16 rgSCHDrxUeCfg(RgSchCellCb *cell,RgSchUeCb *ue,RgrUeCfg *ueCfg)
{
   S16               ret       = ROK;
   RgSchDrxUeCb      *ueDrxCb;
   CmLteTimingInfo   nxtOnDur;
   uint16_t          onDurIndx;
   uint16_t          nxtOnDurTime;
   uint16_t          curTime;
   uint8_t           cellIdx;


#if ( ERRCLASS & ERRCLS_INT_PAR )   
   if ( cell == (RgSchCellCb* )NULLP) 
   {
      return RFAILED;
   }

   if ((ue == (RgSchUeCb* )NULLP)
         ||
         (ueCfg == (RgrUeCfg* )NULLP))
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHDrxUeCfg():"
               "Invalid params.cell or ue or ueCfg is NULL ");
      return RFAILED;
   }
#endif


   /* allocate and initialize drxCb */
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data**)&ue->drxCb, 
         (sizeof(RgSchDrxUeCb)));

   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for DRX UECB CRBTI:%d",ue->ueId);
      return (ret);
   }

   ueDrxCb = ue->drxCb;

   /* initialize the masks */
   ueDrxCb->drxDlInactvMask = DRX_UE_INACTIVE;
   ueDrxCb->drxUlInactvMask = DRX_UE_INACTIVE;
   ue->dl.dlInactvMask     |= RG_DRX_INACTIVE;
   ue->ul.ulInactvMask     |= RG_DRX_INACTIVE;

   for(cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
   {
      ue->drxCb->drxDlInactvMaskPerCell[cellIdx] = DRX_UE_INACTIVE;
      ue->drxCb->drxUlInactvMaskPerCell[cellIdx] = DRX_UE_INACTIVE;
   }   

   /* Copy the configuration values into the UE's DRX CB. */
   rgSCHDrxCpyUeCfg (ueDrxCb, &ueCfg->ueDrxCfg);
#ifdef EMTC_ENABLE
   if(ue->isEmtcUe)
   {
      rgSCHEmtcDrxCpyUeCfg(ue ,&ueCfg->ueDrxCfg);
   }
#endif

   /* set all indexes to default values */
   ueDrxCb->onDurIndx       = DRX_INVALID;
   ueDrxCb->onDurExpIndx    = DRX_INVALID;
   ueDrxCb->drxInactvIndx   = DRX_INVALID;
   ueDrxCb->shortCycleIndx  = DRX_INVALID;

   /* set all distances to timer expiry */
   ueDrxCb->onDurExpDistance      = DRX_TMR_EXPRD;
   ueDrxCb->drxInactDistance      = DRX_TMR_EXPRD;
   ueDrxCb->drxShortCycleDistance = DRX_TMR_EXPRD;
   ueDrxCb->distance              = DRX_TMR_EXPRD;

   /* Mark the UE in long/short  cycle initially  as per configuration*/
   if(FALSE == ueDrxCb->isShortCycleCfgd)
   {
      ueDrxCb->isLongCycle = TRUE;
   }
   else
   {
      ueDrxCb->isLongCycle = FALSE;
   }

   /* Calculate the next occurance from this point */
   rgSCHDrxGetNxtOnDur (cell, ueDrxCb, &nxtOnDur,RG_SCH_NO_DELTA);


   nxtOnDurTime = ((nxtOnDur.sfn * RGSCH_NUM_SUB_FRAMES_5G) + nxtOnDur.slot);
   curTime      = ((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G) +
                     cell->crntTime.slot);

   onDurIndx         = nxtOnDurTime % RG_SCH_MAX_DRXQ_SIZE;
   
   ueDrxCb->distance = (nxtOnDurTime - curTime) / RG_SCH_MAX_DRXQ_SIZE;
   if (ueDrxCb->distance < 0)
   {
      DU_LOG("\nERROR  -->  SCH : DRXUE. Invalid "
         "value for distance, %d CRNTI:%d", ueDrxCb->distance,ue->ueId);
   }
   //DU_LOG("\nDEBUG  -->  SCH : The onduartion index is: %d\n",(int)onDurIndx);
   cmLListAdd2Tail(&(cell->drxCb->drxQ[onDurIndx].onDurationQ), 
         &(ueDrxCb->onDurationEnt));

   ueDrxCb->onDurationEnt.node = (PTR)ue;
   ueDrxCb->onDurIndx          = onDurIndx;

   /* Starting Short Cycle Timer */
   if(TRUE == ueDrxCb->isShortCycleCfgd)
   {
      ueDrxCb->drxShortCycleDistance = (ueDrxCb->shortCycleTmrLen *
               ueDrxCb->shortDrxCycle) / RG_SCH_MAX_DRXQ_SIZE;
      ueDrxCb->shortCycleIndx  = (curTime + (ueDrxCb->shortCycleTmrLen *
               ueDrxCb->shortDrxCycle)) % RG_SCH_MAX_DRXQ_SIZE; 
      cmLListAdd2Tail(&(cell->drxCb->drxQ[ueDrxCb->shortCycleIndx].shortCycleQ),
               &(ueDrxCb->shortCycleEnt));
      ueDrxCb->shortCycleEnt.node = (PTR)ue;
   }

   return (ret);
} /* end of rgSCHDrxUeCfg */

/** @brief This function gets the next occurance of onDurationTimer from the
 * current time. 
 *
 * @details rgSCHDrxGetNxtOnDur
 *
 *     Function: rgSCHDrxGetNxtOnDur
 *
 *         Processing steps: -
 *         Calculation of first occurance of onDuration is to be done as
 *         follows. 
 *         Assume DRX configuration came at subframe (x, y) the periodicity is
 *         offset = (perd, offset). 
 *         The (sfn, subframe) satisfying the following condition is the first
 *         occurance from this point. 
 *
 *         (sfn * 10  + subframe) mod perd = offset 
 *
          - 
 *
 *
 * @param  RgSchCellCb     *cell
 * @param  RgSchDrxUeCb    *drxCb
 * @param  CmLteTimingInfo *nxtOnDur
 * @param  uint8_t              delta
 * @return ROK/RFAILED 
 */
static S16 rgSCHDrxGetNxtOnDur(RgSchCellCb *cell,RgSchDrxUeCb *drxCb,CmLteTimingInfo *nxtOnDur,uint8_t delta)
{
   uint16_t   curTime;
   uint16_t   curDist;
   uint16_t   cycleLen;
   uint32_t   numOfCycles;
   uint32_t   nxtDist;


#if ( ERRCLASS & ERRCLS_INT_PAR ) 
   if ( cell == (RgSchCellCb* )NULLP ) 
   {
      return RFAILED;
   }

   if( (drxCb == (RgSchDrxUeCb* )NULLP)
           ||
       (nxtOnDur == (CmLteTimingInfo* )NULLP)
      )
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHDrxGetNxOnDur():Invalid params."
                        "cell/drxCb/nxtOnDur is NULL");
      return RFAILED;
   }
#endif

   
   if (TRUE == drxCb->isLongCycle)
   {
      cycleLen = drxCb->longDrxCycle;
   }
   else
   {
      cycleLen = drxCb->shortDrxCycle;
   }

   curTime = ((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G) + cell->crntTime.slot); 

   curTime  += delta;  /*TODO: see if we need to take care of wrap arounds */

   if ( curTime <= drxCb->drxStartOffset )
   {
      /* offset is the nextOnDur */
      nxtOnDur->sfn = drxCb->drxStartOffset / RGSCH_NUM_SUB_FRAMES_5G;
      nxtOnDur->slot = (uint8_t)(drxCb->drxStartOffset % RGSCH_NUM_SUB_FRAMES_5G);
      nxtDist = ((nxtOnDur->sfn * RGSCH_NUM_SUB_FRAMES_5G) + nxtOnDur->slot); 
   }
   else
   {
      curDist = curTime - drxCb->drxStartOffset;
      
      numOfCycles = curDist / cycleLen;

      if (0 == (curDist % cycleLen))
      {
         /* Perfect match pick up the current time */
         /*nxtOnDur should be set to equal to currentTime + DELTA */
         nxtOnDur->sfn      = (uint16_t)curTime / RGSCH_NUM_SUB_FRAMES_5G;
         nxtOnDur->slot = (uint16_t)curTime % RGSCH_NUM_SUB_FRAMES_5G;
         nxtDist = ((nxtOnDur->sfn * RGSCH_NUM_SUB_FRAMES_5G) + nxtOnDur->slot); 

      }
      else
      {
         nxtDist            = drxCb->drxStartOffset + (numOfCycles + 1) * 
                              cycleLen;
         nxtOnDur->sfn      = (uint16_t)nxtDist / RGSCH_NUM_SUB_FRAMES_5G;
         nxtOnDur->slot = (uint16_t)nxtDist % RGSCH_NUM_SUB_FRAMES_5G;

      }
   }

   /*If next On Duration is less than DL DELTA ahead, we will miss it and
    * hence need to move to the On-Duration after that.*/
   if((nxtDist - (curTime - delta)) <= RG_SCH_DRX_MAX_DELTA)
   {
      nxtDist = nxtDist + cycleLen;
      nxtOnDur->sfn      = (uint16_t)nxtDist / RGSCH_NUM_SUB_FRAMES_5G;
      nxtOnDur->slot = (uint16_t)nxtDist % RGSCH_NUM_SUB_FRAMES_5G;
   }
   return ROK;
} /* end of rgSCHDrxGetNxtOnDur */ 

/** @brief This function is a utility function to copy the UE configuration from
 * the RGR structure to the UE control block. 
 *
 * @details
 *   -Invoked by rgSCHDrxUeCfg
 *
 *     Function: rgSCHDrxCpyUeCfg
 *
 *         Processing steps:
 *         - Copies configuration values
 *
 * @param  RgSchDrxUeCb *ueCb
 * @param  RgrUeDrxCfg  *drxCfg
 * @return ROK/RFAILED 
 */
static S16 rgSCHDrxCpyUeCfg(RgSchDrxUeCb  *ueCb,RgrUeDrxCfg   *drxCfg)
{

#if ( ERRCLASS & ERRCLS_INT_PAR ) 
   if ( (ueCb == (RgSchDrxUeCb* )NULLP ) 
           ||
        (drxCfg == (RgrUeDrxCfg* )NULLP)
      )
   {
      return RFAILED;
   }
#endif

      
   /* Copy all values to UE control block */
#ifdef LTEMAC_R9
   ueCb->cqiMask              = drxCfg->cqiMask;
#endif /*LTEMAC_R9*/
   ueCb->onDurTmrLen          = drxCfg->drxOnDurTmr;
   ueCb->inactvtyTmrLen       = drxCfg->drxInactvTmr;
   ueCb->drxRetransTmrLen     = drxCfg->drxRetxTmr;
   ueCb->longDrxCycle         = drxCfg->drxLongCycleOffst.longDrxCycle;
   ueCb->drxStartOffset       = drxCfg->drxLongCycleOffst.drxStartOffst;
   ueCb->isShortCycleCfgd     = drxCfg->drxShortDrx.pres;
   ueCb->shortDrxCycle        = drxCfg->drxShortDrx.shortDrxCycle;
   ueCb->shortCycleTmrLen     = drxCfg->drxShortDrx.drxShortCycleTmr;
   
   return ROK;
} /* end of rgSCHDrxCpyUeCfg */ 

#ifdef RGR_V2
/** @brief This function is called by the configuration module when a UE is
 * reconfigured for DRX parameters. 
 *
 * @details
 * Invoked By - rgSCHCfgRgrUeCfg
 *
 * Function: rgSCHDrxUeReCfg 
 * As per MAC specifications the new values of timers shall be applied only once
 * they are restarted, hence no processing is required for modified timer values. 
 *
 *         Processing steps:
 *         - if offset and/or drxCycleLenght changes then recalculate the next
 *         onDurationIndex
 *          - remove the UE from current index
 *          - add the UE to the new index. 
 *         - if short cycle is enabled 
 *          - set isShortCycleCfgd = TRUE
 *
 * @param   RgSchCellCb    *cell
 * @param   RgSchUeCb      *ue
 * @param   RgrUeRecfg     *ueReCfg
 * @return  ROK/RFAILED
 */
S16 rgSCHDrxUeReCfg(RgSchCellCb *cell,RgSchUeCb *ue,RgrUeRecfg  *ueReCfg)
{
      /* DRX_INFI_LOOP */
   RgSchCmnCell      *cellSch = NULLP;
   CmLListCp         dlInactvLst; /* list of UE's becoming DL-inactive */
   S16               ret       = ROK;
   Inst              instIdx   = cell->instIdx;
   RgSchDrxUeCb      *ueDrxCb;
   CmLteTimingInfo   nxtOnDur;
   uint16_t          nxtOnDurTime;
   uint16_t          onDurIndx;
   uint16_t          curTime;
   uint16_t          shrtCycleExpIndx;
   uint16_t          onDurExpTime;
   uint16_t          cycleLen;
   uint16_t          curIndx;
   uint8_t           cellIdx;


  /* drx was disabled but now enabled for this ue */ 
  if ( (ue->isDrxEnabled == FALSE) 
             &&
       (ueReCfg->ueDrxRecfg.isDrxEnabled == TRUE)
     )
  {
     /* allocated and initialize drxCb */
     ret = rgSCHUtlAllocSBuf(instIdx, (Data**)&ue->drxCb,
                              (sizeof(RgSchDrxUeCb)));

     if ( ret != ROK )
     {
        DU_LOG("\nERROR  -->  SCH : rgSCHdrxUeReCfg():""Memory allocation FAILED for DRX UE Cb CRNTI:%d",
                  ue->ueId);
        return (ret);
     }

     ue->isDrxEnabled = TRUE;    /* sachin */
     /* initialize the masks */
     ue->drxCb->drxDlInactvMask = DRX_UE_INACTIVE;
     ue->drxCb->drxUlInactvMask = DRX_UE_INACTIVE;
     ue->dl.dlInactvMask     |= RG_DRX_INACTIVE;
     ue->ul.ulInactvMask     |= RG_DRX_INACTIVE;

     for(cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
     {
        ue->drxCb->drxDlInactvMaskPerCell[cellIdx] = DRX_UE_INACTIVE;
        ue->drxCb->drxUlInactvMaskPerCell[cellIdx] = DRX_UE_INACTIVE;
     }   

     /* set all indexes to default values */
     ue->drxCb->onDurIndx       = DRX_INVALID;
     ue->drxCb->onDurExpIndx    = DRX_INVALID;
     ue->drxCb->drxInactvIndx   = DRX_INVALID;
     ue->drxCb->shortCycleIndx  = DRX_INVALID;

     /* set all distances to timer expiry */
     ue->drxCb->onDurExpDistance      = DRX_TMR_EXPRD;
     ue->drxCb->drxInactDistance      = DRX_TMR_EXPRD;
     ue->drxCb->drxShortCycleDistance = DRX_TMR_EXPRD;
     ue->drxCb->distance              = DRX_TMR_EXPRD;
     
  }
  if( ue->drxCb == NULLP ) 
  { 
     return RFAILED; 
  }
   ueDrxCb = ue->drxCb;
   
   /*drx was enabled but now disabled for this UE */
   if ( (ue->isDrxEnabled == TRUE )
            &&
        (ueReCfg->ueDrxRecfg.isDrxEnabled == FALSE)
      )
   {
      /* remove UE from all DRX Queues */
      (Void)rgSCHDrxUeDel(cell,ue);

      /* free drxCb */
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx,(Data **)(&((ue->drxCb))),
                              sizeof(RgSchDrxUeCb));

      /* Resetting the DRX Bit set in Inactv Mask */
      ue->dl.dlInactvMask     &= ~RG_DRX_INACTIVE;
      ue->ul.ulInactvMask     &= ~RG_DRX_INACTIVE;

      ue->isDrxEnabled = FALSE;

   }/*isDrxEnabled == FALSE */
   else
   {
      /* If Application is updating DRX params */
      if (ueReCfg->ueRecfgTypes & RGR_UE_DRX_RECFG )
      {
         rgSCHDrxCpyUeCfg (ueDrxCb, &ueReCfg->ueDrxRecfg);
#ifdef EMTC_ENABLE
         if(ue->isEmtcUe)
         {
            rgSCHEmtcDrxCpyUeCfg(ue, &ueReCfg->ueDrxRecfg);
         }
#endif

      }

      /* Removing the UE from existing index of shortcycle, if any*/ 
      if(ueDrxCb->shortCycleIndx != DRX_INVALID)
      {
         cmLListDelFrm(&(cell->drxCb->drxQ[ueDrxCb->shortCycleIndx].shortCycleQ),
               &(ueDrxCb->shortCycleEnt)); 

         ueDrxCb->shortCycleEnt.node = (PTR) NULLP;
         ueDrxCb->shortCycleIndx     = DRX_INVALID;
      }
      
      /* Mark for intiating long/short cycle as per received config */
      if(FALSE == ue->drxCb->isShortCycleCfgd)
      {
         ue->drxCb->isLongCycle = TRUE;
      }
      else
      {
         ue->drxCb->isLongCycle = FALSE;
      }  

      /* Calculate the next occurance from this point */
      rgSCHDrxGetNxtOnDur (cell, ueDrxCb, &nxtOnDur,RG_SCH_NO_DELTA);

      /* remove the UE from the current onDuration Queue */ 
      if ( ueDrxCb->onDurIndx != DRX_INVALID )
      {
         cmLListDelFrm(&(cell->drxCb->drxQ[ueDrxCb->onDurIndx].onDurationQ),
                             &(ueDrxCb->onDurationEnt)); 
      }


      nxtOnDurTime = (nxtOnDur.sfn * RGSCH_NUM_SUB_FRAMES_5G) + nxtOnDur.slot;
      curTime = ((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G) +
                   cell->crntTime.slot);

      /* If Onduration timer of old configuration is already running then waiting till it expires */
      if((ueDrxCb->onDurExpIndx != DRX_INVALID) && (ueDrxCb->onDurExpDistance != DRX_TMR_EXPRD))
      {
         curIndx = (curTime + RG_SCH_DRX_DL_DELTA) % RG_SCH_MAX_DRXQ_SIZE;
         DU_LOG("\nDEBUG  -->  SCH : OLD ONDUR RUNNING-EXPIRES at %d curIdx-%d nxtOnDurTime-%d",
               ueDrxCb->onDurExpIndx, 
               curIndx,
               nxtOnDurTime);  

         /* Manipulating the time when old onDuration timer can expire */ 
         if(curIndx >= ueDrxCb->onDurExpIndx)
         {
            onDurExpTime = curTime + ((ueDrxCb->onDurExpDistance+1) * RG_SCH_MAX_DRXQ_SIZE)+\
	    (ueDrxCb->onDurExpIndx - curIndx + RG_SCH_DRX_DL_DELTA); 
         }
         else
         {
            onDurExpTime = curTime + (ueDrxCb->onDurExpDistance * RG_SCH_MAX_DRXQ_SIZE)+\
	    (ueDrxCb->onDurExpIndx - curIndx + RG_SCH_DRX_DL_DELTA); 
         }         

         if(nxtOnDurTime <= onDurExpTime)
         {
            if(ueDrxCb->isLongCycle)
            {
               cycleLen = ueDrxCb->longDrxCycle;
            }
            else
            {
               cycleLen = ueDrxCb->shortDrxCycle;
            }
            /* Moving to the possible occassion of onduration after current onduration expiry:
             * If both are aligning then going for next cycle */
            nxtOnDurTime += ((onDurExpTime - nxtOnDurTime)/cycleLen + 1 ) *cycleLen ;
         }
      }   
      /* Add the UE to the index which corresponds to the next onduration start*/
      onDurIndx = nxtOnDurTime % RG_SCH_MAX_DRXQ_SIZE;

      ueDrxCb->distance = (nxtOnDurTime - curTime) / RG_SCH_MAX_DRXQ_SIZE;
      if (ueDrxCb->distance < 0)
      {
         DU_LOG("\nERROR  -->  SCH : DRXUE. Invalid "
            "value for distance, %d CRNTI:%d", ueDrxCb->distance,ue->ueId);
      }
    
      cmLListAdd2Tail(&(cell->drxCb->drxQ[onDurIndx].onDurationQ), 
            &(ueDrxCb->onDurationEnt));

      ueDrxCb->onDurationEnt.node = (PTR)ue;
      ueDrxCb->onDurIndx        = onDurIndx;

      /* DRX_INFI_LOOP */
      cmLListInit(&dlInactvLst);
      /* Add to DL inactive list */
      cmLListAdd2Tail(&dlInactvLst,&(ue->dlDrxInactvLnk));
      ue->dlDrxInactvLnk.node = (PTR)ue;
      /* Send the list to the scheduler to mark UE as inactive */
      cellSch = RG_SCH_CMN_GET_CELL(cell);
      cellSch->apisDl->rgSCHDlInactvtUes(cell, &dlInactvLst);
      /* DRX_INFI_LOOP */

      /* Starting short cycle timer as per the existence of old onDuration timer */
      if(TRUE == ueDrxCb->isShortCycleCfgd)
      {
         /* Expiring short DRX cycle Tmr once the number of short DRX cycles done */
         ueDrxCb->drxShortCycleDistance = (nxtOnDurTime + ueDrxCb->onDurTmrLen + (ueDrxCb->shortCycleTmrLen -1 )*
               ueDrxCb->shortDrxCycle) / RG_SCH_MAX_DRXQ_SIZE;
         shrtCycleExpIndx = (nxtOnDurTime + ueDrxCb->onDurTmrLen + ((ueDrxCb->shortCycleTmrLen -1)*
               ueDrxCb->shortDrxCycle)) % RG_SCH_MAX_DRXQ_SIZE; 
         cmLListAdd2Tail(&(cell->drxCb->drxQ[shrtCycleExpIndx].shortCycleQ),
               &(ueDrxCb->shortCycleEnt));
         ueDrxCb->shortCycleEnt.node = (PTR)ue;
         ueDrxCb->shortCycleIndx     = shrtCycleExpIndx;
      }   
   }

   return ROK;

} /* end of rgSCHDrxUeReCfg */
#endif

/** @brief This function Loop through the list of HARQ processes for this 
 *  UE and delete from  the harqRTTQ and harqRetxQ
 *
 *     Function: rgSCHDrxUeHqReset
 *         Invoked by rgSCHDrxUeDel
 *
 *         Processing steps:
           Loop through the list of HARQ processes for this UE and delete from
 *         the harqRTTQ and harqRetxQ. 
 *
 * @param  RgSchCellCb  *cell
 * @return ROK/RFAILED
 */
Void rgSCHDrxUeHqReset(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlHqEnt  *hqE,uint8_t cellIdx)
{ 
   RgSchDlHqProcCb     *hqP;
   RgSchDrxDlHqProcCb  *drxHq   =NULLP;
   uint8_t             i;

   for(i=0; i < hqE->numHqPrcs; i++)
   {
      hqP     = &hqE->procs[i];
      drxHq   = RG_SCH_DRX_GET_DL_HQ(hqP);

      if(drxHq->rttIndx != DRX_INVALID)
      {
         cmLListDelFrm (&(cell->drxCb->drxQ[drxHq->rttIndx].harqRTTQ),
               &(drxHq->harqRTTEnt));

         drxHq->rttIndx = DRX_INVALID;
      }

      if(drxHq->reTxIndx != DRX_INVALID)
      {
         cmLListDelFrm (&(cell->drxCb->drxQ[drxHq->reTxIndx].harqRetxQ),
               &(drxHq->harqRetxEnt));

         drxHq->reTxIndx = DRX_INVALID;
      }
   }

   ue->drxCb->drxDlInactvMaskPerCell[cellIdx] = DRX_UE_INACTIVE;
   ue->drxCb->drxUlInactvMaskPerCell[cellIdx] = DRX_UE_INACTIVE;
}

/** @brief This function Deletes DRX specific context for a UE.
 *
 * @details This funciton is invoked by the Configuration module on RGR UE Deletion.
 * This function removes the UE's context from all of the DRX Queues. 
 * In addition it deletes context of UE's HARQ Processes present in the harqRTTQ
 * and harqRetxQ
 *    
 * 
 *     Function: rgSCHDrxUeDel
 *         Invoked by rgSCHCfgRgrUeDel
 *
 *         Processing steps:
 *         - Remove the UE from the following queues
 *          - onDurationQ - using onDurIndx
 *          - onDurationExpQ - using onDurExpIndx
 *          - inActvTmrQ - using drxInactvIndx
 *          - shortCycleQ - using shortCycleIndx
 *         - Loop through the list of HARQ processes for this UE and delete from
 *         the harqRTTQ and harqRetxQ. 
 *
 * @param  RgSchCellCb  *cell
 * @param  RgSchUeCb    *ue
 * @return ROK/RFAILED
 */
S16 rgSCHDrxUeDel(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchDrxUeCb       *ueDrxCb;
   RgSchDlHqEnt       *hqE = NULLP;
   uint8_t            cellIdx;
   RgSchUeCellInfo    *cellInfo = NULLP;
#ifdef EMTC_ENABLE
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
#endif


   /* ccpu00129899: Moved the drx-enabled check to the caller */   
   ueDrxCb = ue->drxCb;

   /* Remove UE from all queues */
   if ( ueDrxCb->onDurIndx != DRX_INVALID )
   {
      cmLListDelFrm(&(cell->drxCb->drxQ[ueDrxCb->onDurIndx].onDurationQ),
            &(ueDrxCb->onDurationEnt));

      ueDrxCb->onDurIndx = DRX_INVALID;
   }

   if ( ueDrxCb->onDurExpIndx != DRX_INVALID )
   {
      cmLListDelFrm(&(cell->drxCb->drxQ[ueDrxCb->onDurExpIndx].onDurationExpQ),
            &(ueDrxCb->onDurationExpEnt)); 

      ueDrxCb->onDurExpIndx = DRX_INVALID;
   }

   if ( ueDrxCb->drxInactvIndx != DRX_INVALID )
   {
      cmLListDelFrm(&(cell->drxCb->drxQ[ueDrxCb->drxInactvIndx].inActvTmrQ),
            &(ueDrxCb->inActvTmrEnt)); 

      ueDrxCb->drxInactvIndx = DRX_INVALID;
   }

   if ( ueDrxCb->shortCycleIndx != DRX_INVALID )
   {
      cmLListDelFrm(&(cell->drxCb->drxQ[ueDrxCb->shortCycleIndx].shortCycleQ),
            &(ueDrxCb->shortCycleEnt)); 

      ueDrxCb->shortCycleIndx = DRX_INVALID;
   }

   for(cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
   {
      cellInfo = ue->cellInfo[cellIdx];

      if(cellInfo)
      {   
         hqE = cellInfo->hqEnt;
         rgSCHDrxUeHqReset(cell, ue, hqE, cellIdx);
      }
   }
#ifdef EMTC_ENABLE
   if(ue->isEmtcUe)
   {
       rgSCHDrxUeUlHqReset(cell, ue, &(ueUl->hqEnt));
   }
#endif
   /* Resetting the DRX Bit set in Inactv Mask */
   ue->dl.dlInactvMask     &= ~RG_DRX_INACTIVE;
   ue->ul.ulInactvMask     &= ~RG_DRX_INACTIVE;
   ueDrxCb->drxDlInactvMask = DRX_UE_INACTIVE;
   ueDrxCb->drxUlInactvMask = DRX_UE_INACTIVE;

   return ROK;
}/*rgSCHDrxUeDel*/

/** @brief This function is called at the time of RGR cell configuration.
 *
 * @details
 *     Invoked by - rgSCHCfgRgrCellCfg
 *
 *     Function: rgSCHDrxCellCfg 
 *
 *         Processing steps:
 *         - Initializes the following drxQ (memset would do).
 *
 *
 * @param RgSchCellCb   *cell
 * @param RgrCellCfg    *cellCfg
 * @return ROK/RFAILED
 */
S16 rgSCHDrxCellCfg(RgSchCellCb *cell,RgrCellCfg *cellCfg)
{

   S16               ret       = ROK;
   Inst              instIdx   = cell->instIdx;

#if ( ERRCLASS & ERRCLS_INT_PAR )
  /*KWORK_FIX :Removed check for cell being NULL*/ 
   if( (cellCfg == (RgrCellCfg* )NULLP))
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHDrxCellCfg():Invalid Params. cell/cellCfg is NULL");
      return RFAILED;
   }
#endif

   /* allocate and initialize drxCb */
   ret = rgSCHUtlAllocSBuf(instIdx, (Data**)&cell->drxCb, 
         (sizeof(RgSchDRXCellCb)));

   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHDrxCellCfg():"
               "Memory allocation FAILED for DRX cell Cb");
      return (ret);
   }

   /* delInUlScan determines which index scans the queue last.
    * We look at onDurationQ both from ulIndex & dlIndex pov.
    * Consider, onDuration starts at index 5, and current index is 2,
    * while dlIndex is 2 & ulIndex is 3 i.e dl is looking 2 SF ahead
    * and ul is looking 3 SF ahead. In this case, dl will scan the queue
    * last and therefore DL will delete ueCb from onDuration q. 
    * The reverse is true for the other case.*/
   
   if ( RG_SCH_DRX_UL_DELTA > RG_SCH_DRX_DL_DELTA )
   {
      cell->drxCb->delInUlScan = FALSE;
   }
   else
   {
      cell->drxCb->delInUlScan = TRUE;
   }

   return (ret);
} /* end of rgSchDrxCellCfg */



/** @brief This function to delete DRX specific context in the cell control
 * block.
 *
 * @details
 *    Invoked by - rgSCHCfgRgrCellDel
 *
 *     Function: rgSCHDrxCellDel 
 *
 *         Processing steps:
 *         - De-Inits RgSchDRXCellCb (Nothing to be done)
 *         - Assumption: The API is invoked after deletion of UEs from the cell.
 *
 * @param  RgSchCellCb  *cell
 * @return Void
 */
Void rgSCHDrxCellDel(RgSchCellCb *cell)
{
   Inst              instIdx      = cell->instIdx;

   if (cell->drxCb)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx, (Data **)(&(cell->drxCb)),
            sizeof(RgSchDRXCellCb));
   }
   return;
} /* end of rgSchDrxCellDel */

/** @brief This function is called when an SR is received from the UE. In this
 * case the UE should be marked as ACTIVE till we send a UL allocation to the
 * UE.
 *
 * @details
 *    Invoked by - rgSCHCmnSrRcvd
 *    Must be called after calling the specific scheduler.
 *
 *     Function: rgSCHDrxSrInd 
 *
 *         Processing steps:
 *         - Mark the UE as ACTIVE 
 *           ueCb->drxUlInactvMask |= (DRX_SR_ACTIVE);
 *         - Optionally call schedulers to add this UE to their scheduling
 *         queues. 
 *         - Set drxUe->srRcvd = TRUE
 *
 *         Note : the specification state that the UE shall start be active
 *         listening for UL grant, this implies we could potentially exploit
 *         this to send DL transmissions as well. However we have currently
 *         chosen not to do so.
 *
 * @param   RgSchCellCb  *cell
 * @param   RgSchUeCb    *ue
 * @return  ROK/RFAILED
 */
S16 rgSCHDrxSrInd(RgSchCellCb *cell,RgSchUeCb  *ue)
{
   RgSchDrxUeCb      *drxCb;

#if ( ERRCLASS & ERRCLS_INT_PAR )
   if ( cell == (RgSchCellCb* )NULLP)
   {
      return ROK;
   }

   if( (ue == (RgSchUeCb* )NULLP))
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHDrxSrInd():Invalid Params. cell/ue is NULL");
      return RFAILED;
   }
 #endif
   /* KWork fix - shifted this down */
   

   drxCb = RG_SCH_DRX_GET_UE(ue);

   /* Mark the UE as active for UL only */
   drxCb->drxUlInactvMask  &= ~RG_SCH_DRX_SR_BITMASK;
   drxCb->srRcvd = TRUE;
   /* Update UE's inactive mask and if required move UE to ACTIVE state */
   RG_SCH_CMN_UL_UPDT_INACTV_MASK( cell, ue, RG_DRX_INACTIVE);

   return ROK;
} /* rgSCHDrxSrInd */


/** @brief This function handles ACTIVITY due to RACH using a dedicated preamble
 * (PDCCH order) OR Handover. A UE shall remain marked as active from the time
 * we successfully send out a RAR upto the time it receives a PDCCH indicating a
 * new transmission. 
 *
 * @details
 *    Invoked by - rgSCHCmnHdlHoPo
 *
 *     Function: rgSCHDrxDedRa 
 *
 *         Processing steps:
 *         - MARK the UE as active
 *         - set the raRcvd = TRUE for this UE.
 *
 *         @code
 *         ueCb->drxDlInactvMask |= (DRX_RA_ACTIVE);
 *         ueCb->drxUlInactvMask |= (DRX_RA_ACTIVE);
 *         ueCb->raRcvd = TRUE;
 *         @endcode
 *
 * @param  RgSchCellCb  *cellCb
 * @param  RgSchUeCb    *ueCb
 * @return Void 
 */
Void rgSCHDrxDedRa(RgSchCellCb *cellCb, RgSchUeCb *ueCb)
{
   RgSchDrxUeCb      *drxCb;

   drxCb = RG_SCH_DRX_GET_UE(ueCb);

   /* Mark the UE as active for UL & DL */
   drxCb->drxUlInactvMask  &= ~RG_SCH_DRX_RA_BITMASK;
   /* Update UE's inactive mask and if required move UE to ACTIVE state */
   RG_SCH_CMN_UL_UPDT_INACTV_MASK(cellCb, ueCb, RG_DRX_INACTIVE);
   
   drxCb->drxDlInactvMask  &= ~RG_SCH_DRX_RA_BITMASK;
   /* Update UE's inactive mask and if required move UE to ACTIVE state */
   RG_SCH_CMN_DL_UPDT_INACTV_MASK(cellCb, ueCb, RG_DRX_INACTIVE);

   drxCb->raRcvd = TRUE;

   return;
} /* end of rgSCHDrxDedRa */


/** @brief This function calculates the next onDuration Occurence
 * and removes & queue it again in onDurationQ
 *
 * @details
 *    Invoked by - 
 *
 *     Function: rgSCHDrxMvToNxtOnDurOcc
 *
 *         Processing steps:
 *
 *
 * @param  RgSchCellCb     *cell
 * @param  RgSchUeCb       *ueCb
 * @param  uint16_t              idx  - if calcFrmOffst is TRUE,
 *                                 idx is delta to be added
 * @param  Bool             calcFrmOffst
 * @return Void 
 */
static Void rgSCHDrxMvToNxtOnDurOcc(RgSchCellCb *cell,RgSchUeCb *ueCb,uint16_t idx,Bool calcFrmOffst)
{
   uint16_t        nxtOnDurIndex;
   uint16_t        curTime;
   RgSchDrxUeCb    *drxUe;
   RgSchDRXCellCb  *drxCell;
   CmLteTimingInfo nxtOnDur; /* to be used when calcFrmOffset is set */
   uint16_t        nxtOnDurInSf; /* next On Duration in no of subframes */

   drxCell = cell->drxCb;
   drxUe   = ueCb->drxCb;


   if(calcFrmOffst == FALSE)
   {
      if (drxUe->isLongCycle)
      {
         nxtOnDurIndex = ((idx + drxUe->longDrxCycle)
               % RG_SCH_MAX_DRXQ_SIZE  );
         drxUe->distance = drxUe->longDrxCycle/RG_SCH_MAX_DRXQ_SIZE;
      }
      else
      {
         nxtOnDurIndex = ((idx + drxUe->shortDrxCycle)% RG_SCH_MAX_DRXQ_SIZE);

         drxUe->distance = drxUe->shortDrxCycle / RG_SCH_MAX_DRXQ_SIZE;
      }
   }
   else
   {
      rgSCHDrxGetNxtOnDur(cell,drxUe,&nxtOnDur,(uint8_t)idx);
      
      nxtOnDurInSf = ((nxtOnDur.sfn * RGSCH_NUM_SUB_FRAMES_5G) + 
                         nxtOnDur.slot);
      
      curTime = ((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G) +
            cell->crntTime.slot);

      nxtOnDurIndex   = nxtOnDurInSf % RG_SCH_MAX_DRXQ_SIZE;
      drxUe->distance = (nxtOnDurInSf-curTime) / RG_SCH_MAX_DRXQ_SIZE;
      if (drxUe->distance < 0)
      {
         DU_LOG("\nERROR  -->  SCH : DRXUE. Invalid "
            "value for distance, %d CRNTI:%d", drxUe->distance,ueCb->ueId);
      }
   }

   /* First remove from existing location */
   if ( drxUe->onDurIndx != DRX_INVALID )
   {
      cmLListDelFrm(&(drxCell->drxQ[drxUe->onDurIndx].onDurationQ),
            &(drxUe->onDurationEnt));
   }

   /* Add at new location */
   cmLListAdd2Tail(&(drxCell->drxQ[nxtOnDurIndex].onDurationQ),
         &(drxUe->onDurationEnt));

   drxUe->onDurationEnt.node = (PTR)ueCb;
   drxUe->onDurIndx          = nxtOnDurIndex;

   return;
}/*rgSCHDrxMvToNxtOnDurOcc*/

#ifdef LTE_TDD
/** @brief This function calculates the next SFN,subframe a given  
 *  timer is going to expire. Works for all TDD configurations. 
 *
 * @details
 *
 *     Function: rgSCHDrxGetNxtTmrExpry 
 *         We need to count only PDCCH frames in a given TDD
 *         configuration. This is true for onDuration, inActivity
 *         & drx-retransmission timers.
 *         
 *         Processing steps (assume timer starts at (12,2) and duration
 *                           is 23 DL subframes): 
 *              - based on TDD configuration, move to the next SFN and 
 *                count the number of DL subframes consumed. In our example,
 *                moving to (12,9) will consume 6 DL subframes assuming TDD 
 *                config of 2.
 *              - From this point on, determine how many exact Radio Frames
 *                will be consumed for the remaining DL subframes. In our 
 *                example, remaining DL subframes are (23-6) are 17. 
 *                For config 2, the following holds true
 *                  8 DLSF are in 1 RF
 *                  1 DLSF in 1/8 DLSF
 *                  17 DLSF in 17/8 i.e 2 RF + 1 subframe
 *                  In order to consume 17 DLSF, we need to move forward
 *                  2 RFs + 1 subframe. Adding 2 RF's gives us (14,9)
 *              - For the remaining subframe, we need to figure out the next 
 *                available DL subframe. For TDD_configuration, the first DL 
 *                subframe is at index 0. So, the timer will run till (15,0) 
 *                and will expire on (15,1)
 *
 * @param  RgSchUeCb       *ue          Ue control block.
 * @param  uint16_t             curTime      current Time 
 * @param  uint32_t             duration     Timer duration 
 * @param  CmLteTimingInfo *tmrExpryIdx Timer expry (SFN,sf)
 * @return ROK/RFAILED
 */
static S16 rgSCHDrxGetNxtTmrExpry(RgSchCellCb *cell, uint16_t curTime, uint32_t duration,CmLteTimingInfo *tmrExpryIdx)
{
   uint32_t        dlSfTillNxtSFN;  /*!< DL subframes till next SFN */
   uint8_t         tddCfgMode;      /*!< tdd config mode */
   Bool            isDwPtsCnted;    /*!< is DwPts counted as PDCCH sf */
   CmLteTimingInfo crntTime;        /*!< current SFN & sf */


#if ( ERRCLASS & ERRCLS_INT_PAR )
   if ( (cell == (RgSchCellCb* )NULLP) 
         ||
         (tmrExpryIdx == (CmLteTimingInfo* )NULLP)
      )
   {
      return RFAILED;
   }
#endif

   
   isDwPtsCnted = cell->isDwPtsCnted ;

   tddCfgMode        = cell->ulDlCfgIdx;
   crntTime.sfn      = curTime / RGSCH_NUM_SUB_FRAMES_5G;
   crntTime.slot = curTime % RGSCH_NUM_SUB_FRAMES_5G; 



   /* First calculate the number of DL subframes till next SFN */

   dlSfTillNxtSFN = rgSchDrxDLSfTillNxtSFN[isDwPtsCnted][tddCfgMode]
      [(crntTime.slot % RGSCH_NUM_SUB_FRAMES)];


   if ( dlSfTillNxtSFN >= duration )
   {
      /* the timer would expire on the same RF */
      uint32_t diff = dlSfTillNxtSFN - duration;

      tmrExpryIdx->sfn = crntTime.sfn; 

      if ( diff == 0 )
      {
         tmrExpryIdx->subframe = rgSchDrxDLSftoDLSfIdx[isDwPtsCnted][tddCfgMode]
            [0];
      }
      else
      {
         uint8_t arrayIdx;
         /* to know the DL sf index based on diff */
         arrayIdx = rgSchDrxDlSfTddCfg[isDwPtsCnted][tddCfgMode]; 

         tmrExpryIdx->subframe = rgSchDrxDLSftoDLSfIdx[isDwPtsCnted][tddCfgMode]
            [arrayIdx - diff];
      }
   }/* if ( dlSfTillNxtSFN >= duration...*/
   else
   {
      uint32_t             remSf;           /*!< remaining subframes */
      uint32_t             numRf;           /*!< num of complete radio frames */
      uint32_t             numRemSfs;       /*!< num of remaining subframes */

      remSf = duration - dlSfTillNxtSFN;

      /* move to (currSFN,9) having consued dlSfTillNxtSFN DL subframes */
      tmrExpryIdx->sfn      = crntTime.sfn;
      tmrExpryIdx->subframe = (uint8_t)9;

      numRf      = (1 * remSf)/rgSchDrxDlSfTddCfg[isDwPtsCnted][tddCfgMode];
      numRemSfs  = (1 * remSf)%rgSchDrxDlSfTddCfg[isDwPtsCnted][tddCfgMode];

      tmrExpryIdx->sfn = tmrExpryIdx->sfn + numRf;

      /* we are now at (X,9) having consumed dlSfTillNxtSFN + numRf * num of DL
       * subframes in 1 RF */

      if ( numRemSfs == 0 )
      {
         /* we are at subframe 9 i.e. the timer is going to expire using exact
          * radio frames. However, not all TDD_configurations have 9 as their
          * last DL subframe. We might have passed the last DL subfrme. 
          * Therefore, move back */
         tmrExpryIdx->subframe = rgSchDrxDLSftoDLSfIdx[isDwPtsCnted][tddCfgMode]
                                                                  [numRemSfs];
      }
      else
      {
         /* a reminder implies we have to move past this SFN as we are at the 
          * last subframe on that SFN */
         tmrExpryIdx->sfn++; 
         tmrExpryIdx->subframe = rgSchDrxDLSftoDLSfIdx[isDwPtsCnted][tddCfgMode]
                                                                  [numRemSfs];
      }
   }/*else if diff > duration */

   /* timer will expire 1 + tmrExpryIdx */

   if ( ( tmrExpryIdx->subframe + 1) == 10 )
   {
      tmrExpryIdx->sfn++;
      tmrExpryIdx->subframe = 0;
   }
   else
   {
      tmrExpryIdx->subframe++;
   }

   /* check to see if it sfn has crossed its max */
   if ( tmrExpryIdx->sfn > RG_SCH_CMN_MAX_SFN_NUM )
   {
      tmrExpryIdx->sfn = tmrExpryIdx->sfn - (RG_SCH_CMN_MAX_SFN_NUM + 1);
   }

   return ROK;
}/*rgSCHDrxGetNxtTmrExpry*/

/** @brief This function calculates the next onDuration Occurence
 *         for TDD
 * @details
 *    Invoked by - 
 *
 *     Function: rgSCHDrxCalcNxtTmrExpry
 *
 *         Processing steps: a wrapper function to call
 *         rgSCHDrxGetNxtTmrExpry
 *
 * @param  RgSchCellCb      *cell
 * @param  RgSchUeCb        *ue
 * @param  uint16_t               delta
 * @param  uint32_t               tmrLen 
 * @param  uint16_t              *distance
 * @param  S16              *idx
 * @return ROK/RFAILED
 */
static Void rgSCHDrxCalcNxtTmrExpry(RgSchCellCb *cell, RgSchUeCb *ue, uint16_t delta, uint32_t tmrLen, S16 *distance, uint16_t *idx)
{
   uint16_t        curTimeInSf;  /*current time in no of subframes*/
   CmLteTimingInfo tmrExpry;
   uint16_t        tmrExpryInSf; /*timer expry in no of subframes*/

   curTimeInSf = cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G +
                   cell->crntTime.slot;

   /* add delta to curTime */
   curTimeInSf += delta;

   rgSCHDrxGetNxtTmrExpry(ue->cell,curTimeInSf,tmrLen,&tmrExpry);

   /* convert timre Expry in terms of subframes */
   tmrExpryInSf = tmrExpry.sfn * RGSCH_NUM_SUB_FRAMES_5G +
                   tmrExpry.subframe;

  
   *idx = (tmrExpryInSf) % RG_SCH_MAX_DRXQ_SIZE;

   if ( distance != NULLP ) /* hqReTx don't use the concept of distance.
                               They can send NULLP for distance.
                             */
   {
      if ( tmrExpryInSf > curTimeInSf )
      {
         *distance = (tmrExpryInSf - curTimeInSf) /
                   RG_SCH_MAX_DRXQ_SIZE;
      }
      else
      {
         /* in case the RF is at its max and wraps around */

         *distance = ((tmrExpryInSf + (RG_SCH_CMN_MAX_NUM_OF_SFN - 1)) 
                                     -
                                    curTimeInSf) / RG_SCH_MAX_DRXQ_SIZE;
      }
      if (*distance < 0)
      {
         DU_LOG("\nERROR  -->  SCH : DRXUE. Invalid "
            "value for distance, %d CRNTI:%d", *distance,ue->ueId);
      }      
   }
  
   return;
}/*rgSCHDrxCalcNxtTmrExpry*/

/* ccpu00134670- Validating onduration timer versus DRX cycle*/
/***********************************************************
 *
 *     Func : rgSCHCfgVldtTddDrxCycCfg
 *
 *
 *     Desc : Validates DRX Cycle Length  configuration against received 
 *            onDuration timer from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
S16 rgSCHCfgVldtTddDrxCycCfg(RgSchCellCb *cell,uint16_t drxCycle,uint8_t onDurTmr, uint16_t offSet)
{
   uint16_t       startTime;
   uint16_t       endTimeInSf;
   CmLteTimingInfo endTime; 

   startTime = offSet;
   do
   {
      rgSCHDrxGetNxtTmrExpry(cell, startTime, onDurTmr, &endTime);
    
      endTimeInSf = (endTime.sfn* RGSCH_NUM_SUB_FRAMES)+endTime.subframe;
     
      if(((RGSCH_MAX_SUBFRM_5G + endTimeInSf- startTime) % RGSCH_MAX_SUBFRM_5G) >= 
          drxCycle)
      {
          return RFAILED;
      }
      
      startTime = (startTime + drxCycle);
   /* Going for next iteration if the DRX cycle is not multiple of 10. If it is
      multiple of 10(Number of Subframes in a SFN) then subframe, at which 
      onduration timer can start, will be always same, Otherwise the possible 
      subframe numbers, where the onDuration timer can start, is 5. Hence 4 
      more iterations are required. */
   }while((drxCycle % RGSCH_NUM_SUB_FRAMES) && 
       (startTime < (drxCycle * RGSCH_NUM_SUB_FRAMES/2)));
   
   return ROK;
}

#endif /*LTE_TDD */

/** @brief This function is called to handle onDurationTimer per TTI processing.
 *
 * @details
 *    Invoked by - rgSCHDrxTtiInd 
 *
 *     Function:  rgSCHDrxTtiHdlOnDurUl
 *
 *         Processing steps:
 *
 *           - OnDurationTimer is handled using the drxQ @sa RgSchDrxQ
 *
 *             - For Uplink we shall look at an index that is 
 *               n + RG_SCH_DRX_UL_DELTA as
 *             we are concerned with the PDCCH and not the actual PUSCH.
 *
 *
 * @param  RgSchCellCb  *cellCb
 * @param  uint16_t          ulIndex
 * @return Void 
 */

static Void rgSCHDrxTtiHdlOnDurUl(RgSchCellCb *cell,uint16_t ulIndex)
{
   CmLList           *node;
   RgSchDRXCellCb    *drxCell  = NULLP;
   RgSchUeCb         *ue       = NULLP;
   RgSchDrxUeCb      *drxUe    = NULLP;
   CmLListCp         ulInactvLst; /* list of UE's becoming DL-inactive */
   RgSchCmnCell      *cellSch = NULLP;
   Bool              delInUlScan = FALSE;
   
   drxCell     = (cell->drxCb);
   delInUlScan = drxCell->delInUlScan;
   /***********************************************************
    *    Scanning OnDurationQ in UL
    ***********************************************************/

   /* For Uplink we shall look at an index that is n + RG_SCH_DRX_UL_DELTA as
      we are concerned with the PDCCH and not the actual PUSCH.*/

   node = drxCell->drxQ[ulIndex].onDurationQ.first;

   while (node)
   {
      ue    = (RgSchUeCb*)node->node;
      node  = node->next;
      drxUe = RG_SCH_DRX_GET_UE(ue);


      if ( delInUlScan == FALSE)
      {
         drxUe->distance--;
      }

      if ( drxUe->distance != DRX_TMR_EXPRD )
      {
         continue;
      }

      /* reset the bit mask to indicate that onduration has started */
      drxUe->drxUlInactvMask  &= ~RG_SCH_DRX_ONDUR_BITMASK;

      /* if no other condition is keeping UE as inactive,
       * activate UE
       */
      RG_SCH_CMN_UL_UPDT_INACTV_MASK(cell, ue, RG_DRX_INACTIVE);

      if ( delInUlScan == TRUE )
      {
         /*calculate next on duration occurence 
          * and it to the onDuration Queue*/
         rgSCHDrxMvToNxtOnDurOcc(cell,ue,ulIndex,FALSE);
      }/*delInUlScan == FALSE */
   }/*while(node)*/
   
   /***********************************************************
    *    Scanning OnDurationExpQ in UL
    ***********************************************************/

   node = drxCell->drxQ[ulIndex].onDurationExpQ.first;

   /* Initialize UL inactive list */
   cmLListInit(&ulInactvLst);

   while (node)
   {
      ue = (RgSchUeCb*)node->node;
      node = node->next;
      drxUe = RG_SCH_DRX_GET_UE(ue);

      if ( delInUlScan == FALSE )
      {
         drxUe->onDurExpDistance--;
      }

      if ( drxUe->onDurExpDistance != DRX_TMR_EXPRD )
      {
         continue;
      }

      /*UE is inactive as onduration has expired */
      drxUe->drxUlInactvMask  |= RG_SCH_DRX_ONDUR_BITMASK;

      if( !RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
      {
         /* set the inactive bit to indicate UE has now become inactive */  
         ue->ul.ulInactvMask |= RG_DRX_INACTIVE;

         /* Add to DL inactive list */
         cmLListAdd2Tail(&ulInactvLst,&(ue->ulDrxInactvLnk));
         ue->ulDrxInactvLnk.node = (PTR)ue;
      }

      if ( delInUlScan == TRUE)
      {
         /*Remove from DRX queue*/
         cmLListDelFrm(&(drxCell->drxQ[ulIndex].onDurationExpQ),
               &(drxUe->onDurationExpEnt));

         drxUe->onDurExpIndx = DRX_INVALID;
      }

   }/*while(node)*/

   /* Send the list to the scheduler to mark UE as inactive in UL*/
   cellSch = RG_SCH_CMN_GET_CELL(cell);
   cellSch->apisUl->rgSCHUlInactvtUes(cell, &ulInactvLst);
   
   return;
}

/** @brief This function is called to handle onDurationTimer per TTI processing.
 *
 * @details
 *    Invoked by - rgSCHDrxTtiInd 
 *
 *     Function:  rgSCHDrxTtiHdlOnDurDl
 *
 *         Processing steps:
 *
 *           - OnDurationTimer is handled using the drxQ @sa RgSchDrxQ
 *
 *           - For Downlink we shall look at an index that is 
 *             n + RG_SCH_DRX_DL_DELTA. 
 *
 *
 * @param  RgSchCellCb  *cellCb
 * @param  uint16_t          dlIndex
 * @return Void
 */

static Void rgSCHDrxTtiHdlOnDurDl(RgSchCellCb *cell,uint16_t dlIndex)
{
   CmLList           *node;
   RgSchDRXCellCb    *drxCell  = NULLP;
   RgSchUeCb         *ue       = NULLP;
   RgSchDrxUeCb      *drxUe    = NULLP;
   RgSchCmnCell      *cellSch = NULLP;
   uint16_t          expiryIndex;
   CmLListCp         dlInactvLst; /* list of UE's becoming DL-inactive */
   Bool              delInUlScan = FALSE;

   /* The DL loop, if onDurationTimer has started, will add the UeCb
    * in the onDurationTmrExprQ. If !delInUlScan, then calculate the next
    * OnDuration occurence, q it there and remove it from the current location.
    */
   /***********************************************************
    *    Scanning OnDurationQ in DL
    ***********************************************************/
   drxCell     = (cell->drxCb);
   
   delInUlScan = drxCell->delInUlScan;
   //DU_LOG("\nINFO  -->  SCH : CELL  Timer  [SFN : %d],[SF : %d]\n",cell->crntTime.sfn,cell->crntTime.slot);
   
   node = drxCell->drxQ[dlIndex].onDurationQ.first;


   while (node)
   {
      ue = (RgSchUeCb* )node->node;

      node = node->next;

      drxUe = RG_SCH_DRX_GET_UE(ue);

      if ( delInUlScan == TRUE)
      {
         drxUe->distance--;
      }

      if ( drxUe->distance != DRX_TMR_EXPRD )
      {
         continue;
      }


      /* UE is active as onduration is to start */
      drxUe->drxDlInactvMask &= ~RG_SCH_DRX_ONDUR_BITMASK;

      /* set the UE as DRX active*/ 

      /* Update UE's inactive mask and if required move UE to ACTIVE state */
      RG_SCH_CMN_DL_UPDT_INACTV_MASK(cell, ue, RG_DRX_INACTIVE);
      /*ACC-TDD */      
      /* Temporary fix to delete stale entry */
      if (drxUe->onDurExpIndx != DRX_INVALID)
      {
         DU_LOG("\nDEBUG  -->  SCH : UEID:%d PreExisted[%d:%d]in onDurExpQ new[%d]",
               ue->ueId, 
               drxUe->onDurExpIndx, 
               drxUe->onDurExpDistance, 
               dlIndex);
         cmLListDelFrm(&(drxCell->drxQ[drxUe->onDurExpIndx].onDurationExpQ),
               &(drxUe->onDurationExpEnt));

         drxUe->onDurExpIndx = DRX_INVALID;
      }
      /*start the onduration expiry timer*/
#ifdef LTE_TDD
      rgSCHDrxCalcNxtTmrExpry(cell,
            ue,
            RG_DL_DELTA,
            drxUe->onDurTmrLen,
            &(drxUe->onDurExpDistance),
            &(expiryIndex)
            );
#else /*LTE_TDD */
      expiryIndex = ((dlIndex + drxUe->onDurTmrLen) %
            RG_SCH_MAX_DRXQ_SIZE);
      drxUe->onDurExpDistance = (drxUe->onDurTmrLen)/
         RG_SCH_MAX_DRXQ_SIZE;
#endif /*LTE_TDD */

      cmLListAdd2Tail(&(drxCell->drxQ[expiryIndex].onDurationExpQ),
            &(drxUe->onDurationExpEnt));
      //DU_LOG("\nINFO  -->  SCH : DRXOnDuration Timer Started at [SFN : %d],[SF : %d]\n",cell->crntTime.sfn,cell->crntTime.slot);
      drxUe->onDurationExpEnt.node = (PTR)ue;
      drxUe->onDurExpIndx          = expiryIndex;

      //DU_LOG("\nINFO  -->  SCH : DRxOnDuration will Expire = [%d]\n",\
      (cell->crntTime.sfn*10+cell->crntTime.slot+drxUe->onDurTmrLen));

      if ( delInUlScan == FALSE )
      {
         /*calculate next on duration occurence 
          * and it to the onDuration Queue*/
         rgSCHDrxMvToNxtOnDurOcc(cell,ue,dlIndex,FALSE);
      }/*delInUlScan == FALSE */

   }/*while(node)*/

   /***********************************************************
    *    Scanning OnDurationExpQ in DL
    ***********************************************************/

   /* Mark UE as Inactive based on OnDuration Expiry */
   node = drxCell->drxQ[dlIndex].onDurationExpQ.first;

   /* Initialize DL inactive list */
   cmLListInit(&dlInactvLst);

   while (node)
   {
      ue    = (RgSchUeCb*)node->node;
      node  = node->next;
      drxUe = RG_SCH_DRX_GET_UE(ue);

      if ( delInUlScan == TRUE )
      {
         drxUe->onDurExpDistance--;
      }

      if ( drxUe->onDurExpDistance != DRX_TMR_EXPRD )
      {
         continue;
      }


      /* UE is inactive as onduration has expired */
      drxUe->drxDlInactvMask |= (RG_SCH_DRX_ONDUR_BITMASK);

      if( !RG_SCH_DRX_DL_IS_UE_ACTIVE(drxUe))
      {
         /* set the inactive bit to indicate UE has now become inactive */  
         ue->dl.dlInactvMask |= RG_DRX_INACTIVE;

         /* Add to DL inactive list */
         cmLListAdd2Tail(&dlInactvLst,&(ue->dlDrxInactvLnk));
         ue->dlDrxInactvLnk.node = (PTR)ue;
      }

      if ( delInUlScan == FALSE )
      {
         /*Remove from DRX queue*/
         cmLListDelFrm(&(drxCell->drxQ[dlIndex].onDurationExpQ),
               &(drxUe->onDurationExpEnt));

         drxUe->onDurExpIndx = DRX_INVALID;
      }

   }

   /* Send the list to the scheduler to mark UE as inactive */
   cellSch = RG_SCH_CMN_GET_CELL(cell);
   cellSch->apisDl->rgSCHDlInactvtUes(cell, &dlInactvLst);

   return;
}/*rgSCHDrxTtiHdlOnDurDl*/

 /** @brief This function handles the Dl HARQ timer's processing per TTI.
  *
  * @details
  * Invoked by - rgSCHDrxTtiHdlDlHarq 
  *
  *     Function: rgSCHDrxTtiHdlDlHarqRTT 
  *
  *         Processing steps:
  *         - In addition per TTI DRX module must look at Downlink HARQ queues
 *             maintained to track HARQ RTT timer and drx-RetransmissionTimer.
 *             Every TTI at the scheduling index we shall check these queues and
 *             process accordingly. 
 *
 * @param  RgSchCellCb *cellCb
 * @param  uint16_t          dlIndex
 * @return Void 
 */

static Void rgSCHDrxTtiHdlDlHarqRTT(RgSchCellCb *cell,uint16_t dlIndex)
{
   CmLList             *node;
   RgSchDrxDlHqProcCb  *drxHq;
   RgSchDlHqProcCb     *dlHq;
   RgSchDRXCellCb      *drxCell;
   RgSchDrxUeCb        *drxUe;
   uint16_t            reTxExpIndx;
   Bool                delInUlScan;
   RgSchUeCb           *ue;
   CmLListCp           dlInactvLst; /* list of UE's becoming DL-inactive */
   RgSchCmnCell        *cellSch = RG_SCH_CMN_GET_CELL(cell);
   uint8_t             cellIdx;
   uint32_t            dlInactvMask;

   drxCell     = cell->drxCb;
   delInUlScan = drxCell->delInUlScan;

   /***********************************************************
    *    Scanning harqRTTQ in DL 
    ***********************************************************/

   cmLListInit(&dlInactvLst);
   node = drxCell->drxQ[dlIndex].harqRTTQ.first;

   while (node)
   {
      dlHq = (RgSchDlHqProcCb*)node->node;
      node = node->next;
      ue   = dlHq->hqE->ue;
#ifdef EMTC_ENABLE
      if(TRUE == ue->isEmtcUe)
      {
         continue;
      }
#endif
      drxHq = RG_SCH_DRX_GET_DL_HQ(dlHq);
      drxUe = RG_SCH_DRX_GET_UE(ue);
      cellIdx = ue->cellIdToCellIdxMap[RG_SCH_CELLINDEX(dlHq->hqE->cell)];
      /* add the UE to the cell's retransmission queuee before starting 
       * reTx timer, because this will not depend on retx timer trigger*/
      rgSCHUtlDlProcAddToRetx(dlHq->hqE->cell, dlHq);

      if ( delInUlScan == FALSE)
      {
         cmLListDelFrm (&(drxCell->drxQ[dlIndex].harqRTTQ),
               &(drxHq->harqRTTEnt));

         drxHq->rttIndx = DRX_INVALID;
      }
      /* ccpu00134565: Starting retransmission timer only if timerLen is
       * having non-zero value after reduction, Adding to Retx queue is 
       * independent of starting retransmission timer. Hence if part will
       * take care of starting retx timer only*/
      if (drxUe->drxRetransTmrLen > drxHq->retxTmrReduction)
      {
         /* add the harq proc to the re-tx queue--*/
#ifdef LTE_TDD
         /* ccpu00134196-[Add]-DRX retx timer changes */
         rgSCHDrxCalcNxtTmrExpry(cell,
              ue,
              RG_DL_DELTA,
              drxUe->drxRetransTmrLen-drxHq->retxTmrReduction,
              NULLP, /*retransQ does not maintain distance*/
              &reTxExpIndx
              );

#else /*LTE_TDD*/
         /* ccpu00134196-[Add]-DRX retx timer changes */
         reTxExpIndx =  ((dlIndex +
                  drxUe->drxRetransTmrLen-drxHq->retxTmrReduction) % 
                  RG_SCH_MAX_DRXQ_SIZE);
#endif /*LTE_TDD*/
         /* TODO. Workaround to avoid duplicate entry */ 
         if(drxHq->reTxIndx == DRX_INVALID)
         {
            cmLListAdd2Tail (&(drxCell->drxQ[reTxExpIndx].harqRetxQ),
                  &(drxHq->harqRetxEnt));

            drxHq->harqRetxEnt.node = (PTR)dlHq;
            drxHq->reTxIndx         = reTxExpIndx;
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : CRNTI:%d "
               "Adding Retx Node to expire at RetxIndx: %d at dlIndex %d "
               "drxHq->reTxIndx %d", ue->ueId, reTxExpIndx, dlIndex, 
               drxHq->reTxIndx); 
            continue;
         }
         /*mark the ue as active for downlink--*/
         drxUe->drxDlInactvMask  &= ~(RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);
         drxUe->drxDlInactvMaskPerCell[cellIdx]  &= ~(RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);
                                              
         /* Update UE's inactive mask and if required move UE to ACTIVE state */
         RG_SCH_CMN_DL_UPDT_INACTV_MASK(cell, ue, RG_DRX_INACTIVE);
      }
   }/*while(node)*/
   
   /***********************************************************
    *    Scanning harqRetxQ in DL 
    ***********************************************************/

   node = drxCell->drxQ[dlIndex].harqRetxQ.first;
   while (node)
   {
      dlHq = (RgSchDlHqProcCb*)node->node;
      ue   = dlHq->hqE->ue;
      drxUe = RG_SCH_DRX_GET_UE(ue);
      node = node->next;
      drxHq = RG_SCH_DRX_GET_DL_HQ(dlHq);
      cellIdx = ue->cellIdToCellIdxMap[RG_SCH_CELLINDEX(dlHq->hqE->cell)];

      /*mark the ue as in-active for downlink*/
      drxUe->drxDlInactvMaskPerCell[cellIdx]  |= (RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);

      dlInactvMask = RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId;

      for(cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
      {
         dlInactvMask &= drxUe->drxDlInactvMaskPerCell[cellIdx];
      }   

      drxUe->drxDlInactvMask |= dlInactvMask;

      /* if no other condition is keeping ue active,
       * inactivate the Ue 
       */
      if ( !RG_SCH_DRX_DL_IS_UE_ACTIVE(drxUe))
      {
          ue->dl.dlInactvMask |= (RG_DRX_INACTIVE);

         /* Add to DL inactive list */
         cmLListAdd2Tail(&dlInactvLst,&(ue->dlDrxInactvLnk));
         ue->dlDrxInactvLnk.node = (PTR)ue;
      }

      /*remove the harq proc from this queue*/
      if ( delInUlScan == FALSE)
      {
         cmLListDelFrm (&(drxCell->drxQ[dlIndex].harqRetxQ),
               &(drxHq->harqRetxEnt));
         drxHq->reTxIndx = DRX_INVALID;
      }
   }
   /*Call schedulers to inactivate ue*/
   cellSch->apisDl->rgSCHDlInactvtUes(cell, &dlInactvLst);

   return;
}

 /** @brief This function handles the Ul HARQ timer's processing per TTI.
  *
  * @details
  * Invoked by - rgSCHDrxTtiHdlDlHarq 
  *
  *     Function: rgSCHDrxTtiHdlUlHarqRTT 
  *
  *         Processing steps:
  *         - In addition per TTI DRX module must look at Downlink HARQ queues
 *             maintained to track HARQ RTT timer and drx-RetransmissionTimer.
 *             Every TTI at the scheduling index we shall check these queues and
 *             process accordingly. 
 *
 *             - Though these timers are related to downlink HARQ processing, they
 *             have an impact on uplink scheduling. The reason is that the UE,
 *             if active for downlink scheduling implies that it is reading
 *             PDCCHs i.e. we can still send uplink allocations to the UE. Hence
 *             every TTI Uplink too would look at the harqRTTQ and harqRetxQ. 
 *
 *
 *
 * @param  RgSchCellCb *cellCb
 * @param  uint16_t          ulIndex
  * @return Void 
  */

static Void rgSCHDrxTtiHdlUlHarqRTT(RgSchCellCb *cell,uint16_t ulIndex)
{
   CmLList             *node;
   RgSchDrxDlHqProcCb  *drxHq;
   RgSchDlHqProcCb     *dlHq;
   RgSchDRXCellCb      *drxCell;
   RgSchDrxUeCb        *drxUe;
   Bool                delInUlScan;
   RgSchUeCb           *ue;
   CmLListCp           ulInactvLst; /* list of UE's becoming DL-inactive */
   RgSchCmnCell        *cellSch = RG_SCH_CMN_GET_CELL(cell);
   uint8_t             cellIdx;
   uint32_t            ulInactvMask;


   drxCell     = cell->drxCb;
   delInUlScan = drxCell->delInUlScan;

   cmLListInit(&ulInactvLst);

   /***********************************************************
    *    Scanning harqRTTQ in UL 
    ***********************************************************/

   /*
      Though these timers are related to downlink HARQ processing, they
      have an impact on uplink scheduling. The reason is that the UE,
      if active for downlink scheduling implies that it is reading
      PDCCHs i.e. we can still send uplink allocations to the UE. Hence
      every TTI Uplink too would look at the harqRTTQ and harqRetxQ. 
      */

   node = drxCell->drxQ[ulIndex].harqRTTQ.first;
   while (node)
   {
      dlHq = (RgSchDlHqProcCb*)node->node;
      ue   = dlHq->hqE->ue;
      drxUe = RG_SCH_DRX_GET_UE(ue);
      node = node->next;
      drxHq = RG_SCH_DRX_GET_DL_HQ(dlHq);
      cellIdx = ue->cellIdToCellIdxMap[RG_SCH_CELLINDEX(dlHq->hqE->cell)];

      if ( delInUlScan == TRUE )
      {
         /* remove the harq proc from this queue--*/
         cmLListDelFrm (&(drxCell->drxQ[ulIndex].harqRTTQ), 
               &(drxHq->harqRTTEnt));

         drxHq->rttIndx = DRX_INVALID;
      }

      /* mark the ue as active for uplink--*/
      drxUe->drxUlInactvMask  &= ~(RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);
      drxUe->drxUlInactvMaskPerCell[cellIdx]  &= ~(RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);

      /* Update UE's inactive mask and if required move UE to ACTIVE state */
      RG_SCH_CMN_UL_UPDT_INACTV_MASK( cell, ue, RG_DRX_INACTIVE);
   }

   /***********************************************************
    *    Scanning harqRetxQ in UL 
    ***********************************************************/
   node = drxCell->drxQ[ulIndex].harqRetxQ.first;
   while (node)
   {
      dlHq  = (RgSchDlHqProcCb*)node->node;
      ue    = dlHq->hqE->ue;
      drxUe = RG_SCH_DRX_GET_UE(ue);
      drxHq = RG_SCH_DRX_GET_DL_HQ(dlHq);
      cellIdx = ue->cellIdToCellIdxMap[RG_SCH_CELLINDEX(dlHq->hqE->cell)];

      /*mark the ue as in-active for uplink*/

      drxUe->drxUlInactvMaskPerCell[cellIdx]  |= (RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);

      ulInactvMask = RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId;

      for(cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
      {
         ulInactvMask &= drxUe->drxUlInactvMaskPerCell[cellIdx];
      }   

      drxUe->drxUlInactvMask |= ulInactvMask; 

      if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
      {
         ue->ul.ulInactvMask |= (RG_DRX_INACTIVE);

         cmLListAdd2Tail(&ulInactvLst,&(ue->ulDrxInactvLnk));
         ue->ulDrxInactvLnk.node  = (PTR)ue;
      }

      /* remove the harq proc from this queue*/
      if ( delInUlScan == TRUE)
      {
         cmLListDelFrm (&(drxCell->drxQ[ulIndex].harqRetxQ),
               &(drxHq->harqRetxEnt));
         drxHq->reTxIndx = DRX_INVALID;
      }

      node = node->next;
   }
   cellSch->apisUl->rgSCHUlInactvtUes(cell, &ulInactvLst);

   return;

}

/**********************************************************************
 
         End of file
**********************************************************************/
