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

/* This file contains all utility functions */
#if 0
#include<stdbool.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#endif
#include "du_mgr.h"

#define DU_INST 0
#define DU_ID 1
#define DU_IP_V4_ADDR "10.0.2.20"
#define CU_IP_V4_ADDR "10.0.2.25"
#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define DU_PORT 38472
#define CU_PORT 38472
#define NR_PCI 1
#define NR_CELL_ID 1
#define DU_NAME "ORAN_OAM_DU" 
#define CELL_DIR UL_DL
#define CELL_TYPE SMALL
#define DUPLEX_MODE DUP_MODE_TDD
#define DU_TAC 1
#define PLMN_MCC0 3
#define PLMN_MCC1 1
#define PLMN_MCC2 1
#define PLMN_MNC0 4
#define PLMN_MNC1 8
#define PLMN_MNC2 0 
#define NR_ARFCN  2079427 
#define SUL_ARFCN 100
#define NR_FREQ_BAND 257
#define SUL_BAND 2
#define TIME_CFG 10
#define NR_RANAC 150
#define CARRIER_IDX 1
#define NUM_TX_ANT 2
#define NUM_RX_ANT 2
#define FREQ_SHIFT_7P5KHZ FALSE
#define SSB_PBCH_PWR 0
#define BCH_PAYLOAD MAC_GEN_FULL_PBCH_PAYLD
#define SUBCARRIER_SPACING 3
#define PRB_OFFSET 0
#define BETA_PSS BETA_PSS_0DB
#define SSB_PERIODICITTY SSB_PRDCTY_MS_20
#define SSB_SUBCARRIER_OFFSET 0
#define SSB_MULT_CARRIER_BAND FALSE
#define MULT_CELL_CARRIER FALSE

#define PRACH_SEQ_LEN SHORT_SEQUENCE
#define PRACH_SUBCARRIER_SPACING 3
#define PRACH_RESTRICTED_SET_CFG UNRESTRICTED
#define NUM_PRACH_FDM 1
#define ROOT_SEQ_IDX 0
#define NUM_ROOT_SEQ 1
#define ZERO_CORRELATION_ZONE_CFG 2
#define NUM_UNUSED_ROOT_SEQ 1
#define UNUSED_ROOT_SEQ 1
#define SSB_PER_RACH 3
#define PRACH_MULT_CARRIER_BAND FALSE
#define TDD_PERIODICITY TX_PRDCTY_MS_2P5
#define RSS_MEASUREMENT_UNIT DONT_REPORT_RSSI

/* Filling Slot configuration as :
 * Slot Sym 0 Sym 1 Sym 2 Sym 3 Sym 4 Sym 5 Sym 6 Sym 7 Sym 8 Sym 9 Sym10 Sym11 Sym12 Sym13
 *   0   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
 *   1   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
 *   2   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
 *   3   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    GD    GD    GD    UL
 *   4   UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL
*/

/*******************************************************************
 *
 * @brief Fills the Slot configuration 
 *
 * @details
 *
 *    Function : fillSlotConfig
 *
 *    Functionality:Fill the Slot configuration values
 * 
 *  @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillSlotConfig()
{
   U8 slot;
   U8 symbol;

   for(slot = 0; slot <= 3; slot++)
   {
      for(symbol = 0; symbol < MAX_SYMB_PER_SLOT; symbol++)
         duCfgParam.clCellCfg.tddCfg.slotCfg[slot][symbol] = DL_SLOT;
   }

   duCfgParam.clCellCfg.tddCfg.slotCfg[3][10] = GUARD_SLOT;
   duCfgParam.clCellCfg.tddCfg.slotCfg[3][11] = GUARD_SLOT;
   duCfgParam.clCellCfg.tddCfg.slotCfg[3][12] = GUARD_SLOT;
   duCfgParam.clCellCfg.tddCfg.slotCfg[3][13] = UL_SLOT;

   for(symbol = 0; symbol < MAX_SYMB_PER_SLOT; symbol++)
      duCfgParam.clCellCfg.tddCfg.slotCfg[4][symbol] = UL_SLOT;

}

/*******************************************************************
 *
 * @brief Reads the CL Configuration.
 *
 * @details
 *
 *    Function : readClCfg
 *
 *    Functionality:
 *       - Fills up the cell configuration for CL.
 *       - Calls fillSlotConfig()  
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/


/* This function is used to fill up the cell configuration for CL */
S16 readClCfg()
{
   duCfgParam.clCellCfg.carrierId = CARRIER_IDX;

   /* Cell configuration */
   duCfgParam.clCellCfg.cellId = NR_CELL_ID;
   duCfgParam.clCellCfg.phyCellId = NR_PCI;
   duCfgParam.clCellCfg.dupType = DUPLEX_MODE;

   /* DL carrier configuration */
   duCfgParam.clCellCfg.dlCarrCfg.pres = TRUE;
   duCfgParam.clCellCfg.dlCarrCfg.bw = SUL_ARFCN;
   duCfgParam.clCellCfg.dlCarrCfg.freq = NR_ARFCN;
   duCfgParam.clCellCfg.dlCarrCfg.k0[0] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.k0[1] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.k0[2] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.k0[3] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.k0[4] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.gridSize[0] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.gridSize[1] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.gridSize[2] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.gridSize[3] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.gridSize[4] = 1;
   duCfgParam.clCellCfg.dlCarrCfg.numAnt = NUM_TX_ANT;

   /* UL Carrier configuration */
   duCfgParam.clCellCfg.ulCarrCfg.pres = TRUE;
   duCfgParam.clCellCfg.ulCarrCfg.bw = SUL_ARFCN;
   duCfgParam.clCellCfg.ulCarrCfg.freq = NR_ARFCN;
   duCfgParam.clCellCfg.ulCarrCfg.k0[0] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.k0[1] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.k0[2] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.k0[3] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.k0[4] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.gridSize[0] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.gridSize[1] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.gridSize[2] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.gridSize[3] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.gridSize[4] = 1;
   duCfgParam.clCellCfg.ulCarrCfg.numAnt = NUM_RX_ANT;

   duCfgParam.clCellCfg.freqShft = FREQ_SHIFT_7P5KHZ;

   /* SSB configuration */
   duCfgParam.clCellCfg.ssbCfg.ssbPbchPwr = SSB_PBCH_PWR;
   duCfgParam.clCellCfg.ssbCfg.bchPayload = BCH_PAYLOAD;
   duCfgParam.clCellCfg.ssbCfg.scsCmn = SUBCARRIER_SPACING;
   duCfgParam.clCellCfg.ssbCfg.ssbPrbOffset = PRB_OFFSET;
   duCfgParam.clCellCfg.ssbCfg.betaPss = BETA_PSS;
   duCfgParam.clCellCfg.ssbCfg.ssbPeriod = SSB_PERIODICITTY;
   duCfgParam.clCellCfg.ssbCfg.ssbSubcOffset = SSB_SUBCARRIER_OFFSET;
   duCfgParam.clCellCfg.ssbCfg.mibPdu = 104;
   duCfgParam.clCellCfg.ssbCfg.nSSBMask[0] = 2162721;
   duCfgParam.clCellCfg.ssbCfg.nSSBMask[1] = 0;
   duCfgParam.clCellCfg.ssbCfg.multCarrBand = SSB_MULT_CARRIER_BAND;
   duCfgParam.clCellCfg.ssbCfg.multCellCarr = MULT_CELL_CARRIER;

   /* PRACH configuration */
   duCfgParam.clCellCfg.prachCfg.pres = TRUE;
   duCfgParam.clCellCfg.prachCfg.prachSeqLen = PRACH_SEQ_LEN;
   duCfgParam.clCellCfg.prachCfg.prachSubcSpacing = PRACH_SUBCARRIER_SPACING;
   duCfgParam.clCellCfg.prachCfg.prachRstSetCfg = PRACH_RESTRICTED_SET_CFG;
   duCfgParam.clCellCfg.prachCfg.prachFdm = NUM_PRACH_FDM;
   duCfgParam.clCellCfg.prachCfg.fdm[0].rootSeqIdx = ROOT_SEQ_IDX;
   duCfgParam.clCellCfg.prachCfg.fdm[0].numRootSeq = NUM_ROOT_SEQ;
   duCfgParam.clCellCfg.prachCfg.fdm[0].k1 = 1;
   duCfgParam.clCellCfg.prachCfg.fdm[0].zeroCorrZoneCfg = ZERO_CORRELATION_ZONE_CFG;
   duCfgParam.clCellCfg.prachCfg.fdm[0].numUnusedRootSeq = NUM_UNUSED_ROOT_SEQ;
   if(SGetSBuf(DU_APP_MEM_REGION, DU_POOL, (Data **)&(duCfgParam.clCellCfg.prachCfg.fdm[0].unsuedRootSeq), NUM_UNUSED_ROOT_SEQ * sizeof(U8)))
   {
      printf("\nMemory allocation failed");
      RETVALUE(RFAILED);
   }
   *(duCfgParam.clCellCfg.prachCfg.fdm[0].unsuedRootSeq) = UNUSED_ROOT_SEQ;
   duCfgParam.clCellCfg.prachCfg.ssbPerRach = SSB_PER_RACH;
   duCfgParam.clCellCfg.prachCfg.prachMultCarrBand = PRACH_MULT_CARRIER_BAND;

   /* TDD configuration */
   duCfgParam.clCellCfg.tddCfg.pres = TRUE;
   duCfgParam.clCellCfg.tddCfg.tddPeriod = TDD_PERIODICITY;

   fillSlotConfig();

   /* RSSI Measurement configuration */
   duCfgParam.clCellCfg.rssiUnit = RSS_MEASUREMENT_UNIT;

   /* This should be calculated based on
      (number of mandatory parameters) + (number of otional parameters being filled) */
   duCfgParam.clCellCfg.numTlv = 39;

   RETVALUE(ROK);
}
/*******************************************************************
 *
 * @brief Configures the DU Parameters
 *
 * @details
 *
 *    Function : readCfg
 *
 *    Functionality:
 *       - Initializes the DuCfg members.  
 *       - Calls readClCfg()  
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 readCfg()
{
   U8 i,j,k;
   U32 ipv4_du, ipv4_cu;

   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);
   //U32 ipv6_int = inet_addr(DU_IP_V6_ADDR); 	

   /* F1 DU IP Address and Port*/
   duCfgParam.sctpParams.duIpAddr.ipV4Pres = TRUE;
   duCfgParam.sctpParams.duIpAddr.ipV4Addr = ipv4_du;
   duCfgParam.sctpParams.duIpAddr.ipV6Pres = FALSE;
   //strcpy(duCfgParam.sctpParams.duIpAddr.ipV6Addr, (char*)DU_IP_V6_ADDR);
   duCfgParam.sctpParams.duPort = DU_PORT;

   /* F1 CU IP Address and Port*/
   duCfgParam.sctpParams.cuIpAddr.ipV4Pres = TRUE;
   duCfgParam.sctpParams.cuIpAddr.ipV4Addr = ipv4_cu;
   duCfgParam.sctpParams.cuIpAddr.ipV6Pres = FALSE;
   //strcpy(duCfgParam.sctpParams.cuIpAddr.ipV6Addr, DU_IP_V6_ADDR);
   duCfgParam.sctpParams.cuPort = CU_PORT;

   duCfgParam.maxUe = 32; //TODO: Check
   /* DU Info */	
   duCfgParam.duId = DU_ID;	
   strcpy((char*)duCfgParam.duName,DU_NAME);

   for(i=0;i<MAXCELLINGNBDU;i++)
   { 
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrCgi.plmn.mcc[0] = PLMN_MCC0;
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrCgi.plmn.mcc[1] = PLMN_MCC1;
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrCgi.plmn.mcc[2] = PLMN_MCC2;
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrCgi.plmn.mnc[0] = PLMN_MNC0;
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrCgi.plmn.mnc[1] = PLMN_MNC1;
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrCgi.plmn.mnc[2] = PLMN_MNC2;

      /*Cell ID */
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrCgi.cellId = NR_CELL_ID;
      duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.nrPci = NR_PCI;

      /* List of Available PLMN */
      for(j=0;j<MAXNUMOFBPLMN;j++)
      {
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mcc[0] = PLMN_MCC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mcc[1] = PLMN_MCC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mcc[2] = PLMN_MCC2;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mnc[0] = PLMN_MNC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mnc[1] = PLMN_MNC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mnc[2] = PLMN_MNC2;
      }
      /* List of Extended PLMN */
      for(j=0;j<MAXNUMOFBPLMN;j++)
      {
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mcc[0] = PLMN_MCC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mcc[1] = PLMN_MCC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mcc[2] = PLMN_MCC2;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mnc[0] = PLMN_MNC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mnc[1] = PLMN_MNC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mnc[2] = PLMN_MNC2;
      } 

      /* TAC and EPSTAC */
      duCfgParam.srvdCellLst[i].duCellInfo.tac.pres = TRUE;
      duCfgParam.srvdCellLst[i].duCellInfo.tac.tac = DU_TAC;
      duCfgParam.srvdCellLst[i].duCellInfo.epsTac.pres = TRUE;  //to check and fill.
      duCfgParam.srvdCellLst[i].duCellInfo.epsTac.tac =DU_TAC; //to check and fill

      /* NR Mode info */
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.nrArfcn = NR_ARFCN;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.pres = TRUE;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrScs = SCS_15; 		
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrb = NRB_160;	       

      for(j=0;j<MAXNRCELLBANDS;j++)
      {
         duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.freqBand[j].nrFreqBand = NR_FREQ_BAND;
         for(k=0;k<MAXNRCELLBANDS;k++)
         {
            duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.freqBand[j].sulBand[k] = SUL_BAND; 	
         }
      }

      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrTxBw.nrScs = SCS_15;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrTxBw.nrb = NRB_160;

      /*Measurement Config and Cell Config */ 
      duCfgParam.srvdCellLst[i].duCellInfo.measTimeCfg = TIME_CFG; 

      duCfgParam.srvdCellLst[i].duCellInfo.cellDir = CELL_DIR; 

      duCfgParam.srvdCellLst[i].duCellInfo.cellType=CELL_TYPE;

      /* Broadcast PLMN Identity */
      for(j=0;j<MAXBPLMNNRMINUS1;j++)
      {  
         for(k=0;k<MAXNUMOFBPLMN;k++)
         {
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mcc[0] = PLMN_MCC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mcc[1] = PLMN_MCC1;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mcc[2] = PLMN_MCC2;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mnc[0] = PLMN_MNC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mnc[1] = PLMN_MNC1;                                         
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mnc[2] = PLMN_MNC2;
         }
         /* Extended PLMN List */	 
         for(k=0;k<MAXNUMOFBPLMN;k++)
         {
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mcc[0] = PLMN_MCC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mcc[1] = PLMN_MCC1;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mcc[2] = PLMN_MCC2;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mnc[0] = PLMN_MNC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mnc[1] = PLMN_MNC1;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mnc[2] = PLMN_MNC2;
         }

         duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].tac.pres = TRUE;
         duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].tac.tac = DU_TAC;
         duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].nrCellId = NR_CELL_ID;
         duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].ranac = NR_RANAC;
      }

      /*gnb DU System Info */
      //TODO: uncomment duCfgParam.srvdCellLst[i].duSysInfo.mibMsg;  //to do
      //TODO: uncomment duCfgParam.srvdCellLst[i].duSysInfo.sib1Msg; //to do
   }

   /* RRC Version,Extended RRC Version */
   //TODO: uncomment duCfgParam.rrcVersion.rrcVer; //to do
   //TODO: uncomment duCfgParam.rrcVersion.extRrcVer; //to do

   if(readClCfg() != ROK)
   {
      printf("\nFailed while reading CL config");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Reads config and posts message to du_app on completion
 *
 * @details
 *
 *    Function : main
 *
 *    Functionality:
 *      - Calls readCfg()
 *      - Post to du_app for further processing
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duReadCfg()
{
   Pst pst;
   Buffer *mBuf;
   //Read configs into duCfgParams
   if(readCfg() != ROK)
   {
      printf("\nReading configuration failed");
      RETVALUE(RFAILED);
   }

   //Fill pst structure
   cmMemset((U8 *)&(pst), 0, sizeof(Pst));
   pst.srcEnt = (Ent)ENTDUAPP;
   pst.srcInst = (Inst)DU_INST;
   pst.srcProcId = DU_PROC;
   pst.dstEnt = pst.srcEnt;
   pst.dstInst = pst.srcInst;
   pst.dstProcId = pst.srcProcId;
   pst.event = EVTCFG;
   pst.selector = DU_SELECTOR_TC;
   pst.pool= DU_POOL;


   if(SGetMsg(DFLT_REGION, DU_POOL, &mBuf) != ROK)
   {
      printf("\nMemory allocation failed in duReadCfg");
      return RFAILED;
   }

   if (SPstTsk(&pst, mBuf) != ROK)
   {
      printf("\nSPstTsk failed in duReadCfg");
      return RFAILED;
   }

   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/

