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

/* Defines API exchanged between MAC and PHY */
#ifndef __RG_CL_PHY_H___
#define __RG_CL_PHY_H___

#define MSG_HDR_SIZE       8
#define DCI_PAYLOAD_BTYE_LEN 1  /* TBD */

#define MAC_DCI_PER_SLOT    1   /* TBD */
#define MAX_UL_PDU_PER_SLOT 1   /* TBD */
#define MAX_UCI_PER_SLOT    1   /* TBD */
#define MAX_DL_PDU_PER_SLOT 1   /* TBD */
#define MAX_PARAM_TLV      54   /* Maximum TLVs that can be reported in param response */
#define MAX_CONFIG_TLV     40   /* Maximum TLVs that can be configured in config request */
#define MAX_NUM_PDU        255  /* Maximum PDUs that can be included in a message */
#define MAX_UE_GRP_DL      255  /* Maximum UE groups that can be included in a DL TTI request */
#define MAX_UE_GRP_UL      8    /* Maximum UE groups that can be included in a UL TTI request */
#define MAX_UE_PER_GRP_DL  12   /* Maximum UEs included in a group in DL */
#define MAX_UE_PER_GRP_UL  6    /* Maximum UEs included in a group in UL */ 
#define MAX_PRG            275  /* Maximum Precoding Resource Block Group */
#define MAX_DIGBF_INTF     255  /* Maximum digBf interfaces */
#define MAX_CODE_WORD_PER_RNTI 2 /* Maximum code word for one RNTI */
#define MAX_UL_PTRS_PORTS  2     /* Maximum number of UL PTRS ports */
#define MAX_REPORT_SYMB    4     /* Maximum reported symbols in SRS indication */
#define MAX_NUM_PRB        272   /* Maximum number of PRBs in SRS indication PDU */
#define MAX_NUM_PRMBL      64    /* Maximum number of preambles */
#define MAX_HARQ_SIZE      213   /* Maximum size of HARQ payload */

//Defining API type values
#define MSG_TYPE_PARAM_REQ     0x00
#define MSG_TYPE_PARAM_RSP     0x01
#define MSG_TYPE_CONFIG_REQ    0x02
#define MSG_TYPE_CONFIG_RSP    0x03
#define MSG_TYPE_START_REQ     0x04
#define MSG_TYPE_STOP_REQ      0x05
#define MSG_TYPE_STOP_IND      0x06
#define MSG_TYPE_ERR_IND       0x07
#define MSG_TYPE_DL_TTI_REQ    0x80
#define MSG_TYPE_UL_TTI_REQ    0x81
#define MSG_TYPE_SLOT_IND      0x82
#define MSG_TYPE_UL_DCI_REQ    0x83
#define MSG_TYPE_TX_DATA_REQ   0x84
#define MSG_TYPE_RX_DATA_IND   0x85
#define MSG_TYPE_CRC_IND       0x86
#define MSG_TYPE_UCI_IND       0x87
#define MSG_TYPE_SRS_IND       0x88
#define MSG_TYPE_RACH_IND      0x89

/* Defining TAG for Parameter message TLVs */
/* Cell Specific Parameters */
#define PARAM_TAG_REL_CAP                        0x0001    /* Release capacity */
#define PARAM_TAG_PHY_STATE                      0x0002   /* PHY state */
#define PARAM_TAG_SKIP_BLNK_DL_CFG               0x0003  
#define PARAM_TAG_SKIP_BLNK_UL_CFG               0x0004
#define PARAM_TAG_NUM_CFG_TLV_REP                0x0005   /* Number of config TLVs which will be reported */

/* Carrier Specific Parameters */
#define PARAM_TAG_CYCLIC_PREFIX                  0x0006   /* Cyclic prefix */
#define PARAM_TAG_DL_SUBC_SPC_SUPP               0x0007   /* DL subcarrier spacing supported */
#define PARAM_TAG_DL_BDW_SUPP                    0x0008   /* DL bandwidth supported */
#define PARAM_TAG_UL_SUBC_SPC_SUPP               0x0009   /* UL subcarrier spacing supported */
#define PARAM_TAG_UL_BDW_SUPP                    0x000A   /* DL bandwidth supported */

/* PDCCH Parameters */
#define PARAM_TAG_CCE_MAP_TYPE                   0x000B   /* CCE Mapping type */
#define PARAM_TAG_CORES_OUT_FISRT_30SYM          0x000C   /* Coreset outside first 30 symbols of slot support */
#define PARAM_TAG_PRECOD_GRANULAR_CORES          0x000D   /* Precoder granularity coreset support */
#define PARAM_TAG_PDCCH_MU_MIMO                  0x000E   /* PDCCH multi user mimo support */
#define PARAM_TAG_PDCCH_PRECOD_CYC               0x000F   /* PDCCH precoder cycling support */
#define PARAM_TAG_MAX_PDCCH_PER_SLOT             0x0010   /* Max PDCCHs per slot */

/* PUCCH Parameters */
#define PARAM_TAG_PUCCH_FORMATS                  0x0011   /* PUCCH formats supported */
#define PARAM_TAG_MAX_PUCCH_PER_SLOT             0x0012   /* Max PUCCHs per slot */

/* PDSCH Parameters */
#define PARAM_TAG_PDSCH_MAP_TYPE                 0x0013   /* PDSCH mapping type */
#define PARAM_TAG_PDSCH_ALLOC_TYPE               0x0014   /* PDSCH allocation type */
#define PARAM_TAG_PDSCH_VRB_PRB_MAP              0x0015   /* PDSCH vrb to prb mapping */
#define PARAM_TAG_PDSCH_CBG                      0x0016   /* PDSCH code block group support */
#define PARAM_TAG_PDSCH_DMRS_CFG_TYPE            0x0017   /* PDSCH dmrs configuration types */
#define PARAM_TAG_PDSCH_DMRS_MAX_LEN             0x0018   /* PDSCH dmrs max length */
#define PARAM_TAG_PDSCH_DMRS_ADD_POS             0x0019   /* PDSCH dmrs additional position */
#define PARAM_TAG_MAX_PDSCH_TB_PER_SLOT          0x001A   /* Max PDSCH trasnport blocks per slot */
#define PARAM_TAG_MAX_MIMO_LAYER_PDSCH           0x001B   /* Max number of MIMO layers supported for PDSCH */
#define PARAM_TAG_MAX_MOD_ORDER_SUPP_DL          0x001C   /* Max modulation order supported for DL */
#define PARAM_TAG_MAX_MU_MIMO_USER_DL            0x001D   /* Max users of MU MIMO in DL */
#define PARAM_TAG_PDSCH_DAT_IN_DMRS_SYM          0x001E   /* PDSCH data in dmrs symbols supported */
#define PARAM_TAG_PREEMPT_SUPP                   0x001F   /* Pre-emption support */
#define PARAM_TAG_PDSCH_NON_SLOT_SUPP            0x0020   /* PDSCH non slot support */

/* PUSCH Parameters */
#define PARAM_TAG_UCI_MUX_ULSCH_IN_PUSCH         0x0021
#define PARAM_TAG_UCI_ONLY_PUSCH                 0x0022   /* UCI only in PUSCH support */
#define PARAM_TAG_PUSCH_FREQ_HOP                 0x0023   /* PUSCH frequency hopping support */
#define PARAM_TAG_PUSCH_DMRS_CFG_TYPE            0x0024   /* PUSCH dmrs configuration types */
#define PARAM_TAG_PUSCH_DMRS_MAX_LEN             0x0025   /* PUSCH dmrs max length */
#define PARAM_TAG_PUSCH_DMRS_ADD_POS             0x0026   /* PUSCH dmrs additional position */
#define PARAM_TAG_PUSCH_CBG                      0x0027   /* PUSCH code block group support */
#define PARAM_TAG_PUSCH_MAP_TYPE                 0x0028   /* PUSCH mapping type */
#define PARAM_TAG_PUSCH_ALLOC_TYPE               0x0029   /* PUSCH allocation type */
#define PARAM_TAG_PUSCH_VRB_PRB_MAP              0x002A   /* PUSCH vrb to prb mapping */
#define PARAM_TAG_PUSCH_MAX_PTRS_PORT            0x002B   /* PUSCH max PTRS ports */
#define PARAM_TAG_MAX_PUSCH_TB_PER_SLOT          0x002C   /* Max PUSCH trasnport blocks per slot */
#define PARAM_TAG_MAX_NUM_MIMO_NON_CB_PUSCH      0x002D   /* Max number of MIMO layers for non-contention based PUSCH */
#define PARAM_TAG_MAX_MOD_ORDER_SUPP_UL          0x002E   /* Max modulation order supported for UL */
#define PARAM_TAG_MAX_MU_MIMO_USER_UL            0x002F   /* Max users of MU MIMO in UL */
#define PARAM_TAG_DFTS_OFDM_SUPP                 0x0030   /* DFTS-OFDM support */
#define PARAM_TAG_PUSCH_AGGR_FACT                0x0031   /* PUSCH aggregation factor */

/* PRACH Parameters */
#define PARAM_TAG_PRACH_LONG_FORMAT              0x0032   /* PRACH long formats */
#define PARAM_TAG_PRACH_SHORT_FORMAT             0x0033   /* PRACH short formats */
#define PARAM_TAG_PRACH_RES_SET                  0x0034   /* PRACH restricted formats */
#define PARAM_TAG_MAX_PRACH_FD_OCC_IN_SLOT       0x0035   /* Max PRACH fd occassions in a slot */

/* Measurement Parameters */
#define PARAM_TAG_RSSI_MEAS_SUPP                 0x0036   /* RSSI measurement support */

/* Defining TAG for Config message TLVs */
/* Carrier Configuration */
#define CFG_TAG_DL_BW                    0x1001   /* DL bandwidth */
#define CFG_TAG_DL_FREQ                  0x1002   /* DL frequency */
#define CFG_TAG_DLK0                     0x1003 
#define CFG_TAG_DL_GRID_SIZE             0x1004   /* DL grid size */
#define CFG_TAG_NUM_TX_ANT               0x1005   /* Number of transmission antennas */
#define CFG_TAG_UL_BW                    0x1006   /* Uplink bandwidth */
#define CFG_TAG_UL_FREQ                  0x1007   /* UL frequency */
#define CFG_TAG_ULK0                     0x1008
#define CFG_TAG_UL_GRID_SIZE             0x1009   /* UL grid size */
#define CFG_TAG_NUM_RX_ANT               0x100A   /* Number of receiver antennas */
#define CFG_TAG_FREQ_SHIFT_7P5KHZ        0x100B   /* Indicates presence of 7.5 KHz frequency shift */

/* Cell Configuration */
#define CFG_TAG_PHY_CELL_ID              0x100C   /* Physical cell id */
#define CFG_TAG_FRAME_DUP_TYPE           0x100D   /* Frame duplex type */

/* SSB Configuration */
#define CFG_TAG_SS_PBCH_PWR              0x100E   /* SSB Block power */
#define CFG_TAG_BCH_PAYLOAD              0x100F   /* BCH payload options */
#define CFG_TAG_SCS_COMM                 0x1010   /* Subcarrier spacing for common */

/* PRACH Configuration */
#define CFG_TAG_PRACH_SEQ_LEN            0x1011   /* RACH sequence length */
#define CFG_TAG_PRACH_SUBC_SPAC          0x1012   /* Subcarrier spacing of PRACH */
#define CFG_TAG_RES_SET_CFG              0x1013   /* PRACH restricted set config */
#define CFG_TAG_NUM_PRACH_FD_OCC         0x1014   /* Number of PRACH frequency domain occassions */
#define CFG_TAG_PRACH_ROOT_SEQ_ID        0x1015   /* Starting logical root sequence index */
#define CFG_TAG_NUM_ROOT_SEQ             0x1016   /* Number of root sequences for a FD occasion */
#define CFG_TAG_K1                       0x1017   /* Frequency offset (for UL Bw) for each FD */
#define CFG_TAG_PRACH_ZERO_CORR_CFG      0x1018   /* PRACH zero correlation config */
#define CFG_TAG_NUM_UNUSED_ROOT_SEQ      0x1019   /* Number of unused root sequences avaialable */
#define CFG_TAG_UNUSED_ROOT_SEQ          0x101A   /* Unused root sequence */
#define CFG_TAG_SSB_PER_RACH             0x101B   /* SSB per RACH */
#define CFG_TAG_PRACH_MULT_CARR_BAND     0x101C   /* PRACH multiple carrier support in a band */

/* SSB Table */
#define CFG_TAG_SSB_OFFS_POINT_A         0x101D   /* Offset of lowest subcarrier of lowest RB of SS/PBCH block*/
#define CFG_TAG_BETA_PSS                 0x101E   /* PSS EPRE to SSS EPRE in as SS/PBCH block */
#define CFG_TAG_SSB_PERIOD               0x101F   /* SSB periodicity in msec */
#define CFG_TAG_SSB_SUBC_OFFS            0x1020   /* Subcarrier offset */
#define CFG_TAG_MIB                      0x1021
#define CFG_TAG_SSB_MASK                 0x1022   /* Bitmap of actually transmitted SSB */
#define CFG_TAG_BEAM_ID                  0x1023   /* Beam id for each SSB in SSB mask */
#define CFG_TAG_SS_PBCH_MULT_CARR_BAND   0X1024   /* Multiple carrier support in a band */
#define CFG_TAG_MULT_CELL_SS_PBCH_CARR   0x1025   /* Multiple cells support in single carrier */

/* TDD Table */
#define CFG_TAG_TDD_PERIOD               0x1026   /* DL UL transmission periodicity */
#define CFG_TAG_SLOT_CFG                 0x1027   /* Slot config */

/* Measurement Configuration */
#define CFG_TAG_RSSI_MEAS                0x1028   /* RSSI Measurement unit */

typedef enum
{
   MSG_OK,
   MSG_INVALID_STATE,
   MSG_INVALID_CONFIG,
   SFN_OUT_OF_SYNC,
   MSG_SLOT_ERR,
   MSG_BCH_MISSING,
   MSG_INVALID_SFN,
   MSG_UL_DCI_ERR,
   MSG_TX_ERR
}ErrorCode;

typedef struct msgHdr
{
   U8    nMsg;       /* Numer of messages in PHY API msg */    
   U16   msgType;    /* Message type Id */
   U32   msgLen;     /* Length of msg body in bytes */
}MsgHdr;

typedef struct paramTlv
{
   U16     tag;
   U16     length;
   U16     value;   /* TBD : decide on data type */
}L1L2Tlv;

typedef struct sfnSlot
{
   U16   sfn;
   U16   slot;
}SfnSlotIE;

typedef struct pduInfo
{
   U32   handle;
   U16   rnti;
   U8    ulCqi;
   U16   timeAdv;
   U16   rssi;
}PduInfo;

typedef struct bwp
{
   U16   bwpSize;      /* Bandwidth part size */
   U16   bwpStart;     /* Bandwidth part start RB index from reference CRB */
   U8    sbcSpacing;   /* Subcarrier spacing */
   U8    cycPrefix;    /* Cyclic prefix */
}L1L2Bwp;

typedef struct coreset
{
   U8    startSymIdx;        /* Starting OFDM symbol for CORESET */
   U8    durSym;             /* Time duration of CORESET in num of symbols */
   U8    freqDomainRes[6];   /* Frequency domain resources */
   U8    cceRegMapType;      /* CORESET-CCE-to-REG mapping type */
   U8    regBundleSize;      /* Number of registers in a bundle */
   U8    interlvrSize;       /* Inter leaver size */
   U8    coreSetType;        /* Core set type */
   U16   shiftIdx;   
   U8    precodGranule;      /* Precoder granularity */
}L1L2Coreset;

typedef struct txPwrInfo
{
   U8    betaPdcch;      /* PDCCH power used for PDCCH format 1_0 */
   U8    pwrCtrlOffSs;   /* Power control offset SS : used for all other PDCCH formats */
}L1L2TxPwrInfo;

typedef struct codeWrdInfo
{
   U16   tgtCodeRt;   /* Target coding rate */
   U8    quamModOdr;  /* QAM Modulation */
   U8    mcsIdx;      /* MCS index */
   U8    mcsTable;    /* MCS-table-PDSCH */
   U8    rvIdx;       /* Redundancy version index */
   U32   tbSize;      /* Transmit block size */
}L1L2CodeWrdInfo;

typedef struct codeWrd
{
   U8   numCodeWrd;   /* Number of code words for this RNTI */
   L1L2CodeWrdInfo  codeWrdInfo[MAX_CODE_WORD_PER_RNTI]; /* Info of each numCodeWrd */
}L1L2CodeWrd;

typedef struct dmrs
{
   U16   symbPos;         /* DMRS symbol position */
   U8    cfgType;         /* DMRS configuration type */
   U16   scrambleId;      /* DMRS scrambling id */
   U8    scId;            /* DMRS sequence initialization */
   U8    numCdmGrpNoDat;  /* Number of DM-RS CDM groups without data */
   U16   ports;           /* DMRS ports */
}L1L2Dmrs;

typedef struct ptrs
{
   U8   portIdx;     /* PTRS antenna ports */
   U8   timeDens;    /* PTRS time density */
   U8   freqDens;    /* PTRS frequency desnity */
   U8   reOffset;    /* PTRS resource element offset */
   U8   epreRatio;   /* PTRS-To-PDSCH EPRE ratio */
}L1L2Ptrs;

typedef struct cbg
{
   U8   isLstCbPres;    /* Is last CB present in CBG retransmission */
   U8   isInlineTbCrc;  /* Is TB CRC part of data paylod or control msg */
   U32  dlTbCrc;        /* TB CRC to be used in last CB if last CB pres */
}L1L2Cbg;

typedef struct resAllocFreqDom
{
   U8    resAllocType;   /* Resource allocation type */
   U8    rbBitmap[36];   /* Bitmap for RBs for alloc type 0 */
   U16   rbStart;        /* PDSCH starting RB in BWP for alloc type 1 */
   U16   rbSize;         /* Num of RB for PDSCH for alloc type 1 */
   U8    vrbToPrbMap;    /* VRB to PRB mapping */
}L1L2ResAllocFreqDom;

typedef struct puschAllocFreqDom
{
   L1L2ResAllocFreqDom   resFreqDom;        /* Resources allocated in frequency domain */
   Bool                  freqHop;           /* Frequency hopping enabled for alloc type 1 */
   U16                   txDirCurLoc;       /* Uplink Tx direct current location for the carrier */
   Bool                  ulFreqShft7p5Khz;  /* Indicates if 7.5 KHz shift is there */ 
}L1L2PuschAllocFreqDom;

typedef struct resAllocTimeDom
{
   U8   startSymbIdx;   /* Start symbol index of PDSCH mapping from the start of slot */
   U8   nrOfSymbols;    /* PDSCH duration in symbols */
}L1L2ResAllocTimeDom;

typedef struct prgInfo
{
   U16   pmIdx;   /* Index to precoding matrix */
   U16   beamIdx[MAX_DIGBF_INTF];  /* Index of digital beam weight vector */
}L1L2PrgInfo;

typedef struct precodBeamfrmPdu
{
   U16   numPrgs;     /* Number of precoding RB group */ 
   U16   prgSize;     /* Size in RBs of a PRG */
   U8    digBfIntf;   /* Number of STD ant ports feeding into digBF */
   L1L2PrgInfo   prgInfo[MAX_PRG];   /* Info for numPrgs */
}L1L2PrecodBeamfrmPdu;

typedef struct rxBeamFrmPdu
{
   U16   numPrgs;     /* Number of precoding RB group */
   U16   prgSize;     /* Size in RBs of a PRG */
   U8    digBfIntf;   /* Number of STD ant ports feeding into digBF */
   U16   beamIdx[MAX_PRG][MAX_DIGBF_INTF];  /* Rx PRG info */
}L1L2RxBeamFrmPdu;

typedef struct phyGenMib
{
   U8   dmrsTypeAPos;    /* Position of first DM-RS for DL or UL */
   U8   pdcchCfgSib1;    /* PDCCH config SIB1 */
   U8   callBarFlag;     /* Flag to indicate if cell is barred */
   U8   intraFreqResel;  /* Controls cell selection/re-selection to intra-frequency cell */
}L1L2PhyGenMib;

typedef struct bchPayld
{
   union
   {
      U32   macGenMibPdu;   /* MAC generated MIB pdu */
      L1L2PhyGenMib   phyGenMibPdu;   /* PHY generated MIB pdu */
   }payld;
}L1L2BchPayld;

typedef struct dlDci 
{
   U16   rnti;            /* UE RNTI */
   U16   scrambleId;      /* PDCCH-DMRS-Scrambling id */ 
   U16   scrambleRnti;    /* PDCCH-DMRS-Scrambling id */
   U8    ceIdx;           /* CCE start index to send DCI */
   U8    aggLvl;          /* Aggregation Level */
   U16   payldSizeBits;   /* Total DCI length including padding bits */
   U8    payld[DCI_PAYLOAD_BTYE_LEN];         /* DCI Paylod. TBD : value of DCI_PAYLOAD_BTYE_LEN */
   L1L2PrecodBeamfrmPdu   precodBeamfrmPdu;   /* Precoding and Beamforming Info */
   L1L2TxPwrInfo          txPwrInfo;          /* Transmission power info */
}L1L2DlDci;

typedef struct puschMandInfo
{
   U16   tgtCodeRt;       /* Target coding rate */
   U8    quamModOdr;      /* QAM Modulation */
   U8    mcsIdx;          /* MCS index */
   U8    mcsTable;        /* MCS-table-PUSCH */
   U8    trnsfrmPrecod;   /* Transform precoding enabled */
   U16   scrambleId;      /* Data scrambling identity */
   U8    numLayer;        /* Number of layers */
}L1L2PuschMandInfo;

typedef struct puschData
{
   U8    rvIdx;       /* Redundancy version index */
   U8    harqProcId;  /* HARQ process number */
   U8    newDatInd;   /* Indicates new data or retransmission */
   U32   tbSize;      /* Transmission block size */
   U16   numCb;       /* Number of Cbs in TB */
   U8    cbPresPos[]; /* Each bit represent if CB is present */ //TBD: size of array
}L1L2PuschData;

typedef struct puschUci
{
   U16   harqAckBitLen;      /* Number of HARQ-ACK bits */
   U16   csiPrt1BitLen;      /* Number of CSI-Part1 bits */
   U16   csiPrt2BitLen;      /* Number of CSI-Part2 bits */
   U8    alphaScale;         /* Alpha scaling */
   U8    betaOffHarqAck;     /* Beta offset for HARQ-ACK bits */
   U8    betaOffCsi1;        /* Beta offset for CSI-Part1 bits */
   U8    betaOffCsi2;        /* Beta offset for CSI-Part2 bits */
}L1L2PuschUci;

typedef struct puschPtrsInfo
{
   U8   portIdx;   /* PTRS antenna ports */
   U8   dmrsPort;  /* DMRS port corresponding to PTRS port */
   U8   reOffset;  /* PTRS resource element offset */
}L1L2PuschPtrsInfo;

typedef struct puschPtrs
{
   U8                  numPorts;   /* Number of UL PT-RS ports */
   L1L2PuschPtrsInfo   ptrsInfo[MAX_UL_PTRS_PORTS];
   U8                  timeDen;    /* PT-RS time density */
   U8                  freqDen;    /* PT-RS frequency density */
   U8                  ulPtrsPwr;  /* PUSCH to PTRS power ratio per layer pere RE */
}L1L2PuschPtrs;

typedef struct dftsOfdm
{
   U8    lowPaprGrpNum;   /* Group numer for low PAPR number generation */
   U16   lowPaprSeqNum;   /* Low PAPR sequence number */
   U8    ulPtrsSamDen;    /* Number of PTRS groups */
   U8    ulPtrsTimeDen;   /* Number of samples per PTRS groups */
}L1L2DftsOfdm;

typedef struct hopInfo
{
   Bool   freqHopFlag;    /* Frequency hopping enabled for PUCCH resource */
   U16    secHopPrb;      /* Index of first PRB after freq hopping */
   Bool   grpHopFlag;     /* Indicates group hopping enabled */
   Bool   seqHopFlag;     /* Indicates sequence hopping enabled */
   U16    hopId;          /* Scrambling Id for group and sequence hopping */
   U16    initCyclShft;   /* Initial cyclic shift used for frequency hopping */
}L1L2HopInfo;

typedef struct pdcchPdu
{
   L1L2Bwp       bwp;                  /* Bandwidth part info */
   L1L2Coreset   coreset;              /* core set info */
   U16           numDlDci;             /* Number of DCI in this core set */
   L1L2DlDci     dlDci[MAC_DCI_PER_SLOT]; /* DL DCI for each numDlDci */
}L1L2PdcchPdu;

typedef struct pdschPdu
{
   U16                    pduBitmap;   /* Indicates presence of optional pdus */
   U16                    rnti;        /* UE RNTI */
   U16                    pduIdx;      /* PDU index for each PDSCH PDU sent */
   L1L2Bwp                bwp;         /* Bandwidth part info */
   L1L2CodeWrd            codeWrd;     /* Code word info */
   U16                    scrambleId;  /* Data scrambling identity */
   U8                     numLayer;    /* Number of layers */
   U8                     txSch;       /* Transmission scheme */
   U8                     refPnt;      /* Reference point */
   L1L2Dmrs               dmrs;
   L1L2ResAllocFreqDom    resFreqDom;  /* PDSCH allocation in frequency domain */
   L1L2ResAllocTimeDom    resTimeDom;  /* PDSCH allocation in time domain */
   L1L2Ptrs               ptrs;
   L1L2PrecodBeamfrmPdu   precodBeamfrmPdu;   /* Precoding and Beamforming Info */
   L1L2TxPwrInfo          txPwrInfo;          /* Transmission power info */
   L1L2Cbg                cbgFields;
}L1L2PdschPdu;

typedef struct csiRsPdu
{
   L1L2Bwp   bwp;         /* Bandwidth part info */
   U16       startRb;     /* PRB where this CSI resource starts */
   U16       nrOfRb;      /* Num of PRBs across which CSI resource spans */
   U8        csiType;     /* CSI type */
   U8        row;         /* Row entry into CSI resource location table */
   U16       freqDom;     /* Bitmap definin frequency domain allocation */
   U8        symbL0;      /* Time domain location l0 and first OFDM symbol in time domain */
   U8        symbL1;      /* Time domain location l1 and first OFDM symbol in time domain */
   U8        cdmType;     /* CDM type */
   U8        freqDens;    /* Frequency desnity */
   U16       scrambleId;  /* Scrambling Id of CSI-RS */
   L1L2TxPwrInfo          txPwrInfo;          /* Transmission power info */
   L1L2PrecodBeamfrmPdu   precodBeamfrmPdu;   /* Precoding and Beamforming Info */
}L1L2CsiRsPdu;

typedef struct ssbPdu 
{
   U16           phyCellId;      /* Physical cell id */
   U8            betaPss;        /* PSS EPRE to SSS EPRE in a SS/PBCH block */
   U8            ssbBlkIdx;      /* SS/PBCH block index within a SSB burst set */
   U8            ssbSubcOff;     /* SSB subcarrier offset */
   U16           ssbOffPointA;   /* Offset of lower subcarrier of lowest RB */
   U8            bchPayldFlag;   /* Indicates how BCH payload is generated */
   L1L2BchPayld  bchPayld;       /* BCH payload */
   L1L2PrecodBeamfrmPdu   precodBeamfrmPdu;   /* Precoding and Beamforming Info */
}L1L2SsbPdu;

typedef struct prachPdu
{
   U16   phyCellId;     /* Physical cell id */
   U8    numPrachOcas;  /* Number of time-domain PRACH occassions */
   U8    prachform;     /* RACH format occassion for current FD occassion */
   U8    numRa;         /* Frequency domain occassion index */
   U8    startSymb;     /* Start symbol for first PRACH TD occassion */
   U16   numCs;         /* Zero correlation zone config number */
   L1L2RxBeamFrmPdu   beamfrmPdu;   /* Rx Beamforming pdu */
}L1L2PrachPdu;

typedef struct puschPdu
{
   U16                    pduBitmap;    /* Indicates optional pdu */
   U16                    rnti;         /* UE RNTI */
   U32                    handle;       /* handling returned in Rx data ind or uci ind */
   L1L2Bwp                bwp;          /* Bandwidth part info */
   L1L2PuschMandInfo      mandInfo;     /* PUSCH info always included */
   L1L2Dmrs               dmrs;         /* DMRS info */
   L1L2PuschAllocFreqDom  resFreqDom;   /* PUSCH allocation in frequency domain */
   L1L2ResAllocTimeDom    resTimeDom;   /* Resource allocation in time domain */
   //Optional Data depending on pduBitMap
   L1L2PuschData          puschData;
   L1L2PuschUci           puschUci;
   L1L2PuschPtrs          puschPtrs;
   L1L2DftsOfdm           dftsOfdm;
   L1L2RxBeamFrmPdu       beamfrmPdu;   /* Rx Beamforming pdu */
}L1L2PuschPdu;

typedef struct pucchPdu
{
   U16                    rnti;            /* UE RNTI */
   U32                    handle;          /* handling returned in uci ind */
   L1L2Bwp                bwp;             /* Bandwidth part info */
   U8                     formType;        /* PUCCH format type */
   U8                     multSlotTxInd;   /* Flush/keep/combine buffer for multi-slot Tx */	   
   Bool                   pi2Bpsk;         /* Indicates if UE used ppi/2 BPSK instead of QPSK */
   U16                    prbStart;        /* Starting PRB within BWP for this PUCCH */
   U16                    prbSize;         /* Number of pRBs within PUCCH */
   L1L2ResAllocTimeDom    resTimeDom;      /* Resource allocation in time domain */
   L1L2HopInfo            hopInfo;         /* Hopping information */
   U16                    scrambleId;      /* Data scrambling Id */
   U8                     timeDomOccId;    /* Index of orthogonal cover code */
   U8                     preDftOccId;     /* Index of orthogonal cover code */
   U8                     preDftOccLen;    /* Length of an orthogonal cover code */
   Bool                   addDmrsFlag;     /* Additional DMRS enabled flag */
   U16                    dmrsScrambleId;  /* DMRS scrambling id */
   U8                     dmrsCyclShft;    /* Cyclic shift index for DMRS */
   Bool                   srFlag;          /* Indicates SR opportunity in UCI */
   U8                     bitLenHarq;      /* Bit length of HARQ payload */
   U16                    bitLenCsi1;      /* Bit length of CSI part 1 payload */
   U16                    bitLenCsi2;      /* Bit length of CSI part 2 patload */
   L1L2RxBeamFrmPdu       beamfrmPdu;      /* Rx Beamforming pdu */
}L1L2PucchPdu;

typedef struct srsPdu
{
   U16       rnti;           /* UE RNTI */
   U32       handle;         /* Handling returned in SRS indication */
   L1L2Bwp   bwp;            /* Bandwidth part info */
   U8        numAntPort;     /* Number of Antenna ports */
   U8        numSymb;        /* Number of Symbols */
   U8        numRep;         /* Repetition factor */
   U8        timeStartPos;   /* Starting position in time domain */
   U8        cfgIdx;         /* SRS bandwidth config index */
   U16       seqId;          /* SRS sequence Id */
   U8        bwIdx;          /* SRS bandwidth index */
   U8        combSize;       /* Transmission comb size */
   U8        combOff;        /* Transmission comb offset */
   U8        cyclShft;       /* Cyclic shift */
   U8        freqPos;        /* Frequency domain position */
   U8        freqShft;       /* Frequency domain shift */
   U8        freqHop;        /* Frequency hopping */
   U8        grpSeqHop;      /* Group or sequence hopping configuration */
   U8        resType;        /* Type of SRS resource allocation */
   U16       tsrs;           /* SRS periodicity in slot */
   U16       tOffset;        /* Slot offset value */
   L1L2RxBeamFrmPdu       beamfrmPdu;      /* Rx Beamforming pdu */
}L1L2SrsPdu;

typedef struct dlPduInfo
{
   U16   pduType;
   U16   pduSize;
   union
   {
      L1L2PdcchPdu   pdcchPdu;
      L1L2PdschPdu   pdschPdu;
      L1L2CsiRsPdu   csiRsPdu;
      L1L2SsbPdu     ssbPdu;
   }pduCfg;
}L1L2DlPduInfo;

typedef struct ulPduInfo
{
   U16   pduType;
   U16   pduSize;
   union
   {
      L1L2PrachPdu   prachPdu;
      L1L2PuschPdu   puschPdu;
      L1L2PucchPdu   pucchPdu;
      L1L2SrsPdu     srsPdu;
   }pduCfg;
}L1L2UlPduInfo;

typedef struct dlUeGrpInfo
{
   U8   nUe;    /* Number of UEs in group */
   U8   pduIdx[MAX_UE_PER_GRP_DL];   /* Index for no. of PDU identified by nPDU in DL msg */
}L1L2DlUeGrpInfo;

typedef struct ulUeGrpInfo
{
   U8   nUe;    /* Number of UEs in group */
   U8   pduIdx[MAX_UE_PER_GRP_UL];   /* Index for no. of PDU identified by nPDU in UL msg */
}L1L2UlUeGrpInfo;

typedef struct crcInfo
{
   PduInfo pduInfo;
   U8    harqId;      /* HARQ proc id */
   U8    tbCrcSta;    /* Indicates CRC status on TB data */
   U16   numCb;       /* Number of CBs in TB */
   U8    cbCrcSta[];  /* CRC status on Cb data */ //TBD : Array size
}L1L2CrcInfo;

typedef struct repSymbInfo
{
   U16   numRb;               /* Number of PRBs to be reported */
   U8    rbSnr[MAX_NUM_PRB];  /* SNR value in db for each Rb */
}L1L2RepSymbInfo;

typedef struct srsIndPdu
{
   U32   handle;       /* Handle sent in UL TTI request SRS PDU */
   U16   rnti;         /* RNTI sent in UL TTI request SRS PDU */
   U16   timeAdv;      /* Timing advance measured for UE */
   U8    numSymb;      /* Number of symbols for SRS */
   U8    wideBandSnr;  /* SNR value measured within configured SRS bandwith */ 
   U8    numRepSymb;   /* Number of symbols reported */
   L1L2RepSymbInfo   repSymbInfo[MAX_REPORT_SYMB];    /* Reported symbol info */
}L1L2SrsIndPdu;

typedef struct prmblInfo
{
   U8    prmblIdx;   /* Preamble index */
   U16   timeAdv;    /* Timing advance PRACH */
   U32   prmblPwr;   /* Received power */
}L1L2PrmblInfo;

typedef struct rachIndPdu
{
   U16   phyCellId;   /* Physical cell id */
   U8    symbIdx;     /* symbol index */
   U8    slotIdx;     /* Slot index */
   U8    freqIdx;     /* Frequency index */
   U8    avgRssi;     /* Average value of RSSI */
   U8    avgSnr;      /* Average value of SNR */
   U8    nmbPrmbl;    /* Number of detected preambles in PRACH occassion */
   L1L2PrmblInfo prmblInfo[MAX_NUM_PRMBL];   /* List of premable info */
}L1L2RachIndPdu;

typedef struct srInfoFrmt01
{
   Bool   srInd;      /* Indicates if SR detected */
   U8     srConfLvl;  /* Confidence level of detected SR */
}L1L2SrInfoFrmt01;

typedef struct srInfoFrmt234
{
   U16   bitLen;      /* Length of SR payload */
   U8    payload;     /* Contents of SR */
}L1L2SrInfoFrmt234;

typedef struct harqInfoFrmt01
{
   U8    numHarq;     /* Number of HARQ bits in UCI */
   U8    confLvl;     /* Confidence level of detected HARQ */
   U8    value[2];    /* Indicates result on HARQ data */
}L1L2HarqInfoFrmt01;

typedef struct csiharqInfoFrmt234
{
   U8    crc;                     /* CRC result in CSI/HARQ data */
   U16   bitLen;                  /* Length of CSI/HARQ payload */
   U8    payload[MAX_HARQ_SIZE];  /* CSI/HARQ pdu for format 2, 3 and 4 */
}L1L2CsiHarqInfoFrmt234;

typedef struct uciPduInfo
{
   U8                       pduBitmap;  /* Indicates presence of optional pdu */
   PduInfo                  pduInfo;    /* Commom PDU info */
   L1L2CsiHarqInfoFrmt234   harqInfo;   /* HARQ info */
   L1L2CsiHarqInfoFrmt234   csi1Info;   /* CSI Part 1 info */
   L1L2CsiHarqInfoFrmt234   csi2Info;   /* CSI Part 2 info */
}L1L2UciPduInfo;

typedef struct uciPucchFrmt234Pdu
{
   L1L2UciPduInfo        uciPucchPduInfo;  
   U8                    pucchFrmt;        /* PUCCH format */
   L1L2SrInfoFrmt234     srInfo;           /* SR PDU for format 2, 3 and 4 */
}L1L2UciPucchFrmt234Pdu;

typedef struct uciPucchFrmt01Pdu
{
   U8                   pduBitmap;  /* Indicates presence of optional pdu */
   PduInfo              pduInfo;    /* Commom PDU info */
   U8                   pucchFrmt;  /* PUCCH format */
   L1L2SrInfoFrmt01     srInfo;     /* SR PDU for format 0 and 1 */
   L1L2HarqInfoFrmt01   harqInfo;   /* HARQ PDU for format 0 and 1 */
}L1L2UciPucchFrmt01Pdu;

typedef struct uciIndPdu
{
   U16   pduType;      /* UCI indication type pdu */
   U16   pduSize;      /* Size of pdu information */
   union
   {
      L1L2UciPduInfo           uciPuschPdu;          /* UCI indication PDU carried on PUSCH */
      L1L2UciPucchFrmt01Pdu    uciPucchFrmt01Pdu;    /* UCI indication PDU carried on PUCCH format 0 or 1*/
      L1L2UciPucchFrmt234Pdu   uciPucchFrmt234Pdu;   /* UCI indication PDU carried on PUCCH format 2 or 3 or 4 */
   }uciPdu;
}L1L2UciIndPdu;

typedef struct rxDatIndPdu
{
   PduInfo    pduInfo;    /* Commom PDU info */
   U8         harqId;     /* HARQ Process id */
   U16        pduLen;     /* Length of Pdu */
   U32        *macPdu;    /* Contents of MAC pdu */ //TBU:data type 
}L1L2RxDatIndPdu;

typedef struct txDatReqPdu
{
   U16       pduLen;      /* PDU length */
   U16       pduIdx;      /* Correlates MAC PDU with DL_TTI PDSCH PDU */
   U32       nTlv;        /* number of TLVs in TB */
   L1L2Tlv   *tlvLst;     /* list of TLVs */ //TBD:value of MAX_TLV
}L1L2TxDatReqPdu;

typedef struct paramReq
{
   MsgHdr   hdr;
/* No message body is defined as per FAPI doc */
}L1L2ParamReq;

typedef struct paramRsp
{
   MsgHdr    hdr;                        /* Message Header */
   ErrorCode status;                        /* Error code */
   U8        nTlv;                       /* Number of TLVs in msg body */
   L1L2Tlv   rspTlvLst[MAX_PARAM_TLV];   /* List of TLVs reported by PHY */ 
}L1L2ParamRsp;

typedef struct configReq
{
   MsgHdr    hdr;              /* Message header */
   U8        carrierId;        /* Carrier Id */
   U8        nTlv;             /* Number of TLVs in msg body */
   L1L2Tlv   *configTlvLst;    /* List of TLVs reported by PHY */
}L1L2ConfigReq;

typedef struct configRsp
{
   MsgHdr    hdr;                   /* Message header */
   U8        carrierId;             /* Carrier Id */
   ErrorCode status;                /* Response status */
   U8        numUnsuppTlv;          /* Number of invalid or unsupported TLVs */
   U8        numInvTlvForPhySta;    /* Number of TLVs not valid in current PHY state */
   U8        numMissingTlv;         /* Number of missing TLVs */
   L1L2Tlv   *unsuppTlvLst;         /* List of invalid or unsupported TLVs */
   L1L2Tlv   *phyIdleCfgTlvLst;     /* List of invalid TLV that can be configured only in PHY IDLE state */
   L1L2Tlv   *phyRunCfgTlvLst;      /* List of invalid TLV that can be configured only in PHY RUNNING state */
   L1L2Tlv   *missingTlvLst;        /* List of missing TLVs */
}L1L2ConfigRsp;

typedef struct startReq
{
   MsgHdr   hdr;
/* No message body is defined as per FAPI doc */
}L1L2StartReq;

typedef struct stopReq
{
   MsgHdr   hdr;
/* No message body is defined as per FAPI doc */
}L1L2StopReq;

typedef struct stopInd
{
   MsgHdr   hdr;
/* No message body is defined as per FAPI doc */
}L1L2StopInd;

typedef struct errInd
{
   MsgHdr      hdr;      /* Message header */
   SfnSlotIE   sfnSlot;  /* SFN and slot info */
   U8          msgId;    /* Indicates which msg received by PHY has error */
   ErrorCode   err;      /* Error code */
}L1L2ErrInd;

typedef struct slotInd
{
   MsgHdr      hdr;      /* Message header */
   SfnSlotIE   sfnSlot;  /* SFN and slot info */
}L1L2SlotInd;

typedef struct ttiReq
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U8                nPdus;     /* Number of PDUs in msg */
   U8                nUeGrps;   /* Number of UE groups */ 
}L1L2TtiReq;

typedef struct dlTtiReq
{
   L1L2TtiReq        ttiReqInfo;              
   L1L2DlPduInfo     pduLst[MAX_NUM_PDU];     /* List of all PDUs in msg */
   L1L2DlUeGrpInfo   ueGrpLst[MAX_UE_GRP_DL]; /* List of all UE groups in msg */
}L1L2DlTtiReq;

typedef struct ulTtiReq
{
   L1L2TtiReq        ttiReqInfo;
   Bool              rachPres;                /* Indicates if RACH PDU will be included in msg */
   U8                nUlschPdu;               /* Number of ULSCH pdus in msg */
   U8                nUlcchPdu;               /* Number of ULCCH pdus in msg */
   L1L2UlPduInfo     pduLst[MAX_NUM_PDU];     /* List of all PDUs in msg */
   L1L2UlUeGrpInfo   ueGrpLst[MAX_UE_GRP_UL]; /* List of all UE groups in msg */
}L1L2UlTtiReq;

typedef struct ulDciReq
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U8                nPdus;     /* Number of PDUs in msg */
   L1L2DlPduInfo     pduLst[MAX_NUM_PDU];     /* List of PDCCH PDUs in msg */
}L1L2UlDciReq;

typedef struct crcInd
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U16               numCrc;    /* Number of CRCs in msg */
   L1L2CrcInfo       crcInfo[MAX_UL_PDU_PER_SLOT];  //TBD : value of MAX_UL_PDU_PER_SLOT
}L1L2CrcInd;

typedef struct srsInd
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U8                nPdus;     /* Number of PDUs in msg */
   L1L2SrsIndPdu     srsIndPdu[MAX_NUM_PDU];  /* SRS indication PDU list */ 
}L1L2SrsInd;

typedef struct rachInd
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U8                nPdus;     /* Number of PDUs in msg */
   L1L2RachIndPdu    rachIndPdu[MAX_NUM_PDU];  /* RACH indication PDU list */
}L1L2RachInd;

typedef struct uciInd
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U16               numUci;    /* Number of UCIs in msg */
   L1L2UciIndPdu     uciIndPdu[MAX_UCI_PER_SLOT]; //TBD : value of MAX_UCI_PER_SLOT
}L1L2UciInd;

typedef struct rxDatInd
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U8                nPdus;     /* Number of PDUs in msg */
   L1L2RxDatIndPdu   rxDatPdu[MAX_UL_PDU_PER_SLOT]; //TBD: value of MAX_UL_PDU_PER_SLOT
}L1L2RxDatInd;

typedef struct txDatReq
{
   MsgHdr            hdr;       /* Message header */
   SfnSlotIE         sfnSlot;   /* SFN and slot info */
   U8                nPdus;     /* Number of PDUs in msg */
   L1L2TxDatReqPdu   txDatPdu[MAX_DL_PDU_PER_SLOT]; //TBD: value of MAX_DL_PDU_PER_SLOT
}L1L2TxDatReq;

#endif

/**********************************************************************
         End of file
**********************************************************************/
