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
#include "du_cfg.h"
#include "MIB.h"
#include "PLMN-IdentityInfo.h"
#include "odu_common_codec.h"

extern DuCfgParams duCfgParam;
extern char encBuf[ENC_BUF_MAX_LEN];


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
 *    Function : FillSlotConfig
 *
 *    Functionality:Fill the Slot configuration values
 * 
 *  @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FillSlotConfig()
{
   U8 slot;
   U8 symbol;

   for(slot = 0; slot <= 3; slot++)
   {
      for(symbol = 0; symbol < MAX_SYMB_PER_SLOT; symbol++)
         duCfgParam.macCellCfg.tddCfg.slotCfg[slot][symbol] = DL_SLOT;
   }

   duCfgParam.macCellCfg.tddCfg.slotCfg[3][10] = GUARD_SLOT;
   duCfgParam.macCellCfg.tddCfg.slotCfg[3][11] = GUARD_SLOT;
   duCfgParam.macCellCfg.tddCfg.slotCfg[3][12] = GUARD_SLOT;
   duCfgParam.macCellCfg.tddCfg.slotCfg[3][13] = UL_SLOT;

   for(symbol = 0; symbol < MAX_SYMB_PER_SLOT; symbol++)
      duCfgParam.macCellCfg.tddCfg.slotCfg[4][symbol] = UL_SLOT;

}

/*******************************************************************
 *
 * @brief Reads the CL Configuration.
 *
 * @details
 *
 *    Function : readMacCfg
 *
 *    Functionality:
 *       - Fills up the cell configuration for CL.
 *       - Calls FillSlotConfig()  
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/


/* This function is used to fill up the cell configuration for CL */
S16 readMacCfg()
{
   duCfgParam.macCellCfg.carrierId = CARRIER_IDX;

   /* Cell configuration */
   duCfgParam.macCellCfg.cellId = NR_CELL_ID;
   duCfgParam.macCellCfg.phyCellId = NR_PCI;
   duCfgParam.macCellCfg.dupType = DUPLEX_MODE;

   /* DL carrier configuration */
   duCfgParam.macCellCfg.dlCarrCfg.pres = TRUE;
   duCfgParam.macCellCfg.dlCarrCfg.bw = SUL_ARFCN;
   duCfgParam.macCellCfg.dlCarrCfg.freq = NR_ARFCN;
   duCfgParam.macCellCfg.dlCarrCfg.k0[0] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[1] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[2] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[3] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[4] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[0] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[1] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[2] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[3] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[4] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.numAnt = NUM_TX_ANT;

   /* UL Carrier configuration */
   duCfgParam.macCellCfg.ulCarrCfg.pres = TRUE;
   duCfgParam.macCellCfg.ulCarrCfg.bw = SUL_ARFCN;
   duCfgParam.macCellCfg.ulCarrCfg.freq = NR_ARFCN;
   duCfgParam.macCellCfg.ulCarrCfg.k0[0] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[1] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[2] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[3] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[4] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[0] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[1] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[2] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[3] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[4] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.numAnt = NUM_RX_ANT;

   duCfgParam.macCellCfg.freqShft = FREQ_SHIFT_7P5KHZ;

   /* SSB configuration */
   duCfgParam.macCellCfg.ssbCfg.ssbPbchPwr = SSB_PBCH_PWR;
   duCfgParam.macCellCfg.ssbCfg.bchPayloadOption = BCH_PAYLOAD;
   duCfgParam.macCellCfg.ssbCfg.scsCmn = SUBCARRIER_SPACING;
   duCfgParam.macCellCfg.ssbCfg.ssbPrbOffset = PRB_OFFSET;
   duCfgParam.macCellCfg.ssbCfg.betaPss = BETA_PSS;
   duCfgParam.macCellCfg.ssbCfg.ssbPeriod = SSB_PERIODICITTY;
   duCfgParam.macCellCfg.ssbCfg.ssbSubcOffset = SSB_SUBCARRIER_OFFSET;
   duCfgParam.macCellCfg.ssbCfg.mibPdu = 104;
   duCfgParam.macCellCfg.ssbCfg.nSSBMask[0] = 2162721;
   duCfgParam.macCellCfg.ssbCfg.nSSBMask[1] = 0;
   duCfgParam.macCellCfg.ssbCfg.multCarrBand = SSB_MULT_CARRIER_BAND;
   duCfgParam.macCellCfg.ssbCfg.multCellCarr = MULT_CELL_CARRIER;

   /* PRACH configuration */
   duCfgParam.macCellCfg.prachCfg.pres = TRUE;
   duCfgParam.macCellCfg.prachCfg.prachSeqLen = PRACH_SEQ_LEN;
   duCfgParam.macCellCfg.prachCfg.prachSubcSpacing = PRACH_SUBCARRIER_SPACING;
   duCfgParam.macCellCfg.prachCfg.prachRstSetCfg = PRACH_RESTRICTED_SET_CFG;
   duCfgParam.macCellCfg.prachCfg.prachFdm = NUM_PRACH_FDM;
   duCfgParam.macCellCfg.prachCfg.fdm[0].rootSeqIdx = ROOT_SEQ_IDX;
   duCfgParam.macCellCfg.prachCfg.fdm[0].numRootSeq = NUM_ROOT_SEQ;
   duCfgParam.macCellCfg.prachCfg.fdm[0].k1 = 1;
   duCfgParam.macCellCfg.prachCfg.fdm[0].zeroCorrZoneCfg = ZERO_CORRELATION_ZONE_CFG;
   duCfgParam.macCellCfg.prachCfg.fdm[0].numUnusedRootSeq = NUM_UNUSED_ROOT_SEQ;
   DU_ALLOC(duCfgParam.macCellCfg.prachCfg.fdm[0].unsuedRootSeq, NUM_UNUSED_ROOT_SEQ * sizeof(U8));
   if(duCfgParam.macCellCfg.prachCfg.fdm[0].unsuedRootSeq == NULLP)
   {
      DU_LOG("\nDU_APP : Memory allocation failed");
      RETVALUE(RFAILED);
   }
   *(duCfgParam.macCellCfg.prachCfg.fdm[0].unsuedRootSeq) = UNUSED_ROOT_SEQ;
   duCfgParam.macCellCfg.prachCfg.ssbPerRach = SSB_PER_RACH;
   duCfgParam.macCellCfg.prachCfg.prachMultCarrBand = PRACH_MULT_CARRIER_BAND;

   /* TDD configuration */
   duCfgParam.macCellCfg.tddCfg.pres = TRUE;
   duCfgParam.macCellCfg.tddCfg.tddPeriod = TDD_PERIODICITY;

   FillSlotConfig();

   /* RSSI Measurement configuration */
   duCfgParam.macCellCfg.rssiUnit = RSS_MEASUREMENT_UNIT;

   /* This should be calculated based on
      (number of mandatory parameters) + (number of otional parameters being filled) */
   duCfgParam.macCellCfg.numTlv = 39;

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
 *       - Calls readMacCfg()  
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
	MibParams mib;
   Sib1Params sib1;	

   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);

   /* F1 DU IP Address and Port*/
   duCfgParam.sctpParams.duIpAddr.ipV4Pres = TRUE;
   duCfgParam.sctpParams.duIpAddr.ipV4Addr = ipv4_du;
   duCfgParam.sctpParams.duPort = DU_PORT;

   /* F1 CU IP Address and Port*/
   duCfgParam.sctpParams.cuIpAddr.ipV4Pres = TRUE;
   duCfgParam.sctpParams.cuIpAddr.ipV4Addr = ipv4_cu;
   duCfgParam.sctpParams.cuPort = CU_PORT;


   /* EGTP Parameters */
   duCfgParam.egtpParams.localIp.ipV4Pres = TRUE;
   duCfgParam.egtpParams.localIp.ipV4Addr = ipv4_du;
   duCfgParam.egtpParams.localPort = DU_EGTP_PORT;
   duCfgParam.egtpParams.destIp.ipV4Pres = TRUE;
   duCfgParam.egtpParams.destIp.ipV4Addr = ipv4_cu;
   duCfgParam.egtpParams.destPort = CU_EGTP_PORT;
   duCfgParam.egtpParams.minTunnelId = 0;
   duCfgParam.egtpParams.maxTunnelId = 10;

   duCfgParam.maxUe = 32; //TODO: Check
   /* DU Info */	
   duCfgParam.duId = DU_ID;	
   strcpy((char*)duCfgParam.duName,DU_NAME);

   /* Mib Params */
	mib.sysFrmNum = SYS_FRAME_NUM;
	mib.subCarrierSpacingCommon = MIB__subCarrierSpacingCommon_scs15or60;
	mib.ssb_SubcarrierOffset = SSB_SC_OFFSET; 
	mib.dmrs_TypeA_Position = MIB__dmrs_TypeA_Position_pos2;
	mib.controlResourceSetZero = CORESET_ZERO;
	mib.searchSpaceZero = SEARCH_SPACE_ZERO;
	mib.cellBarred = MIB__cellBarred_barred;
	mib.intraFreqReselection =
		MIB__intraFreqReselection_notAllowed;
	duCfgParam.mibParams = mib;

   /* SIB1 Params */
	sib1.plmn.mcc[0] = PLMN_MCC0;
	sib1.plmn.mcc[1] = PLMN_MCC1;
	sib1.plmn.mcc[2] = PLMN_MCC2;
	sib1.plmn.mnc[0] = PLMN_MNC0;
	sib1.plmn.mnc[1] = PLMN_MNC1;
	sib1.plmn.mnc[2] = PLMN_MNC2;
	sib1.tac = DU_TAC;
	sib1.ranac = DU_RANAC;
	sib1.cellIdentity = CELL_IDENTITY;
	sib1.cellResvdForOpUse =\ 
		PLMN_IdentityInfo__cellReservedForOperatorUse_notReserved;
	duCfgParam.sib1Params = sib1;

   for(i=0; i<DEFAULT_CELLS; i++)
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
      for(j=0;j<MAX_PLMN;j++)
      {
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mcc[0] = PLMN_MCC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mcc[1] = PLMN_MCC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mcc[2] = PLMN_MCC2;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mnc[0] = PLMN_MNC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mnc[1] = PLMN_MNC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.plmn[j].mnc[2] = PLMN_MNC2;
      }
      /* List of Extended PLMN */
      for(j=0;j<MAX_PLMN;j++)
      {
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mcc[0] = PLMN_MCC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mcc[1] = PLMN_MCC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mcc[2] = PLMN_MCC2;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mnc[0] = PLMN_MNC0;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mnc[1] = PLMN_MNC1;
         duCfgParam.srvdCellLst[i].duCellInfo.cellInfo.extPlmn[j].mnc[2] = PLMN_MNC2;
      } 

      /* TAC and EPSTAC */
      duCfgParam.srvdCellLst[i].duCellInfo.tac = DU_TAC;
      duCfgParam.srvdCellLst[i].duCellInfo.epsTac = DU_TAC; //to check and fill
      /* NR Mode info */
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn = NR_ARFCN;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulTxBw.nrScs = SCS_120;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulTxBw.nrb = NRB_66;

#if 0
      /* NR Mode info */
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.nrArfcn = NR_ARFCN;
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
#endif
      for(j=0;j<MAXNRCELLBANDS;j++)
      {
         duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.freqBand[j].nrFreqBand = NR_FREQ_BAND;
         for(k=0;k<MAXNRCELLBANDS;k++)
         {
            duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.freqBand[j].sulBand[k] = SUL_BAND;
         }
      }
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.nrArfcn = NR_ARFCN;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulTxBw.nrScs = SCS_120;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulTxBw.nrb = NRB_66;
      for(j=0;j<MAXNRCELLBANDS;j++)
      {
         duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.freqBand[j].nrFreqBand = NR_FREQ_BAND;
         for(k=0;k<MAXNRCELLBANDS;k++)
         {
            duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.freqBand[j].sulBand[k] = SUL_BAND;
         }
      }

      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulTxBw.nrScs = SCS_120;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.ulTxBw.nrb = NRB_66;

      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlTxBw.nrScs = SCS_120;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.fdd.dlTxBw.nrb = NRB_66;

#if 0
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrTxBw.nrScs = SCS_15;
      duCfgParam.srvdCellLst[i].duCellInfo.f1Mode.mode.tdd.nrTxBw.nrb = NRB_160;
#endif
      /*Measurement Config and Cell Config */ 
      duCfgParam.srvdCellLst[i].duCellInfo.measTimeCfg = TIME_CFG; 

      duCfgParam.srvdCellLst[i].duCellInfo.cellDir = DL_UL; 

      duCfgParam.srvdCellLst[i].duCellInfo.cellType=CELL_TYPE;

      /* Broadcast PLMN Identity */
      for(j=0;j<MAXBPLMNNRMINUS1;j++)
      {  
         for(k=0;k<MAX_PLMN;k++)
         {
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mcc[0] = PLMN_MCC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mcc[1] = PLMN_MCC1;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mcc[2] = PLMN_MCC2;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mnc[0] = PLMN_MNC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mnc[1] = PLMN_MNC1;                                         
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].plmn[k].mnc[2] = PLMN_MNC2;
         }
         /* Extended PLMN List */	 
         for(k=0;k<MAX_PLMN;k++)
         {
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mcc[0] = PLMN_MCC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mcc[1] = PLMN_MCC1;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mcc[2] = PLMN_MCC2;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mnc[0] = PLMN_MNC0;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mnc[1] = PLMN_MNC1;
            duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].extPlmn[k].mnc[2] = PLMN_MNC2;
         }

         duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].tac = DU_TAC;
         duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].nrCellId = NR_CELL_ID;
         duCfgParam.srvdCellLst[i].duCellInfo.brdcstPlmnInfo[j].ranac = NR_RANAC;
      }

      /*gnb DU System Info mib msg*/
	   BuildMibMsg();
		DU_ALLOC(duCfgParam.srvdCellLst[i].duSysInfo.mibMsg,\
				strlen(encBuf));
	   if(!(duCfgParam.srvdCellLst[i].duSysInfo.mibMsg))
		{
         DU_LOG("\nDU_APP: Memory allocation failure");
			return RFAILED;
		}
		strcpy(duCfgParam.srvdCellLst[i].duSysInfo.mibMsg, encBuf);

      /*gnb DU System Info mib msg*/
      BuildSib1Msg();
		DU_ALLOC(duCfgParam.srvdCellLst[i].duSysInfo.sib1Msg,\
				encBufSize);
	   if(!(duCfgParam.srvdCellLst[i].duSysInfo.sib1Msg))
		{
         DU_LOG("\nDU_APP: Memory allocation failure");
			return RFAILED;
		}
		for(int x=0; x<encBufSize; x++)
		{
		   duCfgParam.srvdCellLst[i].duSysInfo.sib1Msg[x]=\
			encBuf[x];
	   }

   }

   if(readMacCfg() != ROK)
   {
      DU_LOG("\nDU_APP : Failed while reading MAC config");
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

   cmMemset((U8 *)&duCfgParam, 0, sizeof(DuCfgParams));
   //Read configs into duCfgParams
   if(readCfg() != ROK)
   {
      DU_LOG("\nDU_APP : Reading configuration failed");
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
      DU_LOG("\nDU_APP : Memory allocation failed in duReadCfg");
      return RFAILED;
   }

   if (SPstTsk(&pst, mBuf) != ROK)
   {
      DU_LOG("\nDU_APP : SPstTsk failed in duReadCfg");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Converts bit strings to integer
 *
 * @details
 *
 *    Function : bitStringToInt
 *
 *    Functionality:
 *      - Converts ASN bit string format IEs to integer type
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 bitStringToInt(BIT_STRING_t *bitString, U16 *val)
{
   U16 numOctets, idx;
   if(bitString->buf == NULL || bitString->size <= 0)
   {
      DU_LOG("\nDU_APP : Bit string is empty");
      return RFAILED;
   }

   numOctets = (bitString->size + 7 )/8;
   for(idx=0; idx< numOctets; idx++)
   {
      *val |= bitString->buf[idx];
      *val <<= 8;
   }

   *val |= bitString->buf[numOctets -1];
   *val >>= ((numOctets * 8) - bitString->size);

   return ROK;
}


/**********************************************************************
  End of file
 **********************************************************************/

