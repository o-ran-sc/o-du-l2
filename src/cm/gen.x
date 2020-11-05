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
  
     Name:     general layer
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required by two
               or more layer service user interfaces.
   
     File:     gen.x
  
*********************************************************************21*/

#ifndef __GENX__
#define __GENX__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RGL_SPECIFIC_CHANGES
#include  "cm_debug.h"
#endif   


/* typedefs */

typedef S8 Txt;                   /* text */

typedef uint8_t Ent;                   /* entity */

typedef uint8_t Inst;                  /* instance */

typedef struct entityId           /* entity id */
{
   Ent ent;                       /* entity */
   Inst inst;                     /* instance */
} EntityId;

/* element id */
 
typedef S16 Elmnt;                /* element */
 
typedef S16 ElmntInst1;           /* element instance 1 */
 
typedef S16 ElmntInst2;           /* element instance 2 */
 
typedef S16 ElmntInst3;           /* element instance 3 */
  
typedef struct elmntId            /* element id */
{
   Elmnt elmnt;                   /* element */
   ElmntInst1 elmntInst1;         /* element instance 1 */
   ElmntInst2 elmntInst2;         /* element instance 2 */
   ElmntInst3 elmntInst3;         /* element instance 3 */
} ElmntId;

typedef uint8_t Region;                /* region */

typedef uint8_t Pool;                  /* pool */

typedef uint8_t Prior;                 /* priority */

typedef Prior Priority;           /* priority */

typedef uint8_t Route;                 /* route */

typedef uint8_t Data;                  /* data */

typedef uint8_t Selector;              /* selector */

typedef S16 Reason;               /* reason */

typedef uint16_t ProcId;               /* processor id */

typedef uint8_t Event;                 /* event */

typedef uint32_t TranId;               /* transaction Id */

typedef S16 IntNum;               /* Interface Number */

typedef uint16_t TranNum;              /* Transaction Number */

typedef uint16_t     CmIntfId;         /* Interface identifier       */
typedef uint16_t     CmIntfVer;        /* Interface version          */

#ifdef TDS_ROLL_UPGRADE_SUPPORT
typedef struct  cmIntf            /* Attributes of an interface */
{
   CmIntfId     intfId;           /* Interface identifier       */
   CmIntfVer    intfVer;          /* Interface version          */
} CmIntf;
#endif

/* post structure *** ALIGNED & ORDERED *** */
typedef struct pst                /* parameters for SPstTsk */
{
   ProcId dstProcId;              /* destination processor id (uint16_t) */
   ProcId srcProcId;              /* source processor id      (uint16_t) */

   Ent dstEnt;                    /* destination entity       (uint8_t)  */
   Inst dstInst;                  /* destination instance     (uint8_t)  */
   Ent srcEnt;                    /* source entity            (uint8_t)  */
   Inst srcInst;                  /* source instance          (uint8_t)  */

   Prior prior;                   /* priority                 (uint8_t)  */   
   Route route;                   /* route                    (uint8_t)  */
   Event event;                   /* event                    (uint8_t)  */
   Region region;                 /* region                   (uint8_t)  */

   Pool pool;                     /* pool                     (uint8_t)  */
   Selector selector;             /* selector                 (uint8_t)  */
   CmIntfVer intfVer;             /* interface version        (uint16_t) */
} Pst;

/* systemId structure */
  
typedef struct systemId           /* system id */
{
   S16 mVer;                      /* main version */
   S16 mRev;                      /* main revision */
   S16 bVer;                      /* branch version */
   S16 bRev;                      /* branch revision */
   Txt *ptNmb;                    /* part number */
} SystemId;

/* Protocol Address Structure */
/* Note: The Order of variables should not be changed as it breaks MPC/MPS
 *       code in PLOA
 */
typedef struct protAddr
{
   uint16_t      protType;                  /* Protocol Type */
   uint8_t       len;                       /* Protocol Address Length in bytes */
   uint8_t       preLen;                    /* prefix length in bits */
   uint8_t       address[MAX_PROTADDR_LEN]; /* Protocol Address */ 
#ifdef CM_ARI2
   Bool     autoSysIdPres;         /* Is Autonomous System Id Present */
   uint32_t      autoSysId;                 /* Autonomous System Id */
#endif /* CM_ARI2 */

}ProtAddr;

typedef struct protAddrTbl
{
   uint8_t         count;                 /* Number of addresses */
   ProtAddr   addr[MAX_PROT_ADDRS];  /* Protocol Address List */
}ProtAddrTbl;

/* addrs structure */

typedef struct addrs              /* address */
{
   uint8_t length;                     /* length (bytes or nibbles) */
   uint8_t strg[ADRLEN];               /* address */
} Addrs;

typedef struct shrtAddr           /* short address */
{
   uint8_t length;                     /* length bytes */
   uint8_t strg[SHRTADRLEN];           /* address */
} ShrtAddrs;

typedef struct lngAddr            /* long address */
{
   uint8_t length;                     /* length bytes */
   uint8_t strg[LNGADRLEN];            /* address */
} LngAddrs;

/* bndCfg structure */

typedef struct bndCfg             /* bind configuration */
{
   Txt *usrId;                    /* user id */
   uint8_t bufOwnshp;                  /* buffer ownership */
   uint8_t flcTyp;                     /* flow control type */
   uint8_t wdw;                        /* window */
   Ent ent;                       /* entity */
   Inst inst;                     /* instance */
   Region region;                 /* region */
   Pool pool;                     /* pool */
   Prior prior;                   /* priority */
   Route route;                   /* route */
   Addrs sapAdr;                  /* SAP Address */
   Selector selector;             /* selector */
} BndCfg;

/* gen_x_001.main_91 : Added Definition of Circular Buffer Structure */
/* Circular Buffer Structure */
#ifdef CBUF_ENABLE
typedef struct cb {
   uint32_t cBufIdx;
   uint32_t cBufSize;
   uint8_t *cBufPtr;
   S8 tmpBuf[CBUFPRNTSZE];
}cBuffer;
#endif /* CBUF_ENABLE */

/* tskInit structure */

typedef struct tskInit            /* task initialization */
{
#ifdef SS_MULTIPLE_PROCS
   ProcId proc;                   /* processor */
#endif /* SS_MULTIPLE_PROCS */
   Ent ent;                       /* entity */
   Inst inst;                     /* instance */
   Region region;                 /* static region */
   Pool pool;                     /* static pool */
   Reason reason;                 /* reason */
   Bool cfgDone;                  /* configuration done */
   Bool acnt;                     /* accounting */ 
   Bool usta;                     /* unsolicited status */
   Bool trc;                      /* trace */
#ifdef DEBUGP
   uint32_t dbgMask;                   /* debug mask */
   Txt prntBuf[255];          /* print buffer */
#endif
   Txt *prntCirBuf; /* print buffer for each system task */
#ifdef SS_DIAG
   /* gen_x_001.main_96 :Added logmask */
   uint32_t logMask;                   /* Logging mask */
#endif
   BndCfg lmBnd;                  /* layer management bind */
   ProcId procId;                 /* processor id */
   Pst lmPst;                     /* layer management post */
   /* gen_x_001.main_91 : Added cBuffer in task initialisation strucutre */
#ifdef CBUF_ENABLE
   cBuffer cBuf;                  /* Circular Buffer */
#endif /* CBUF_ENABLE */
   /* gen_x_001.main_92 : Added timestamp related parameter */
   /* gen_x_001.main_93 : Guarding the Timestamp related changes in the compile time flag*/
#ifdef DBG_TIMESTAMP
   SLockId dbgLock;               /* write lock for the log files */
   Bool lyrMtFlag;                /* Layer is Multithreaded or not flag */
#endif
   
} TskInit;


typedef S32 Cntr;                 /* counter */
 
typedef uint32_t StsCntr;              /* Statistics counter */

typedef S16 LnkNmb;               /* link number */

typedef S8 VBit;                      /*V Bit */

typedef S16 SuId;                 /* service user sap id */

typedef S16 SpId;                 /* service provider sap id */

typedef S16 SuInstId;             /* service user instance id */

typedef S16 SpInstId;             /* service provider instance id */

typedef uint16_t PortId;               /* port id */

typedef uint8_t Sapi;                  /* service access point id */
 
typedef uint8_t Tei;                   /* terminal endpoint id */

typedef uint8_t Ces;                   /* connection endpoint suffix */

typedef uint32_t Dlci;                 /* data link control identifier */ 

typedef uint16_t CalRef;               /* call Reference */

typedef S16 Origin;               /* origin */

typedef uint16_t NwId;                 /* Network Identifier */

typedef S16 Swtch;                /* switch */

typedef uint8_t Cause;                 /* cause code */

typedef uint8_t Dgn;                   /* diagnostic code */

typedef S16 Action;               /* action */

typedef S16 SeqS16;               /* signed 16 bit sequence number */
 
typedef uint16_t SeqUInt16;               /* unsigned 16 bit sequence number */
 
typedef S32 SeqS24;               /* signed 24 bit sequence number */
 
typedef uint32_t SeqUInt24;               /* unsigned 24 bit sequence number */
 
typedef uint8_t SetUpArb;              /* set up arbitration (PASSIVE/ACTIVE) */
 
typedef uint8_t EvntType;              /* event type */

typedef uint8_t State;                 /* state */

typedef uint8_t Mode;                  /* mode */

typedef S32 ConnId;               /* connection id */

typedef uint32_t UConnId;              /* unsigned connection id */

typedef uint16_t ProtId;               /* protocol id */

typedef uint16_t ChannelId;            /* channel id */

typedef uint8_t  Arr64UInt8[64];            /* Array of 64 of type uint8_t*/

typedef uint16_t Efa;                  /* Envelope Address Function */

typedef uint32_t BitState;             /* Sa Bit ID and Value */

typedef uint8_t  CChanId;               /* V5UA Channel ID */

typedef uint16_t MibOpCode;            /* Op code for Mib Request MIB_REQUEST_GET,
                                     MIB_REQUEST_GET_FIRST, MIB_REQUEST */

typedef uint16_t MibStatus;            /* Status returned in Mib Cfm */

typedef uint16_t MibTblType;           /* table type */

typedef S32 MibReqId;             /* request identifier */

typedef uint8_t  UstaType;             /* unsolicited status type */
typedef S8  ChannelNo;            /* Channel Number */

/* ATM typedefs */

#ifndef CMFILE_REORG_1

typedef uint16_t AtmVpci;              /* ATM virtual path connection id */
typedef uint16_t AtmVpi;               /* ATM virtual path id */


typedef uint16_t AtmVci;               /* ATM virtual channel id */

typedef uint8_t AtmLp;                 /* ATM loss priority */

typedef uint8_t AtmCi;                 /* ATM congestion indication */

typedef uint8_t AtmRs;                 /* ATM reception status */

typedef uint8_t AtmUu;                 /* ATM user-to-user field in CPCS PDUs */

typedef uint32_t AtmUui;               /* ATM user-to-user indication field in SSCOP PDUs */

typedef uint8_t AtmPt;                 /* ATM cell payload type */

typedef struct atmQos             /* ATM quality of service */
{
   uint8_t qosFwd;                     /* qos in forward  direction */
   uint8_t qosBwd;                     /* qos in backward direction */
} AtmQos;

/* Vitual channel structure */
 
typedef struct amVccId             /* virtual channel connection identifier */
{
   AtmVpci  vpci;                  /* virtual path connection identifier */
   AtmVci   vci;                   /* virtual channel identifier */
} AmVccId;

/* VCC id */
typedef struct atmVccId         /* VCC id */
{
   AtmVpi   vpi;                /* VPI */
   AtmVci   vci;                /* VCI */
} AtmVccId;

/* VCC table */
typedef struct atmVccTbl                  /* VCC table */
{
   uint16_t        count;                      /* number of VCCs */
   AtmVccId   tbl[MAX_ATMVCCTBL_SZ];      /* VCC list */
} AtmVccTbl;

/* Generic ATM address */
typedef struct atmAddr
{
   uint8_t    type;                  /* type of Address (AESA or E.164) */
   uint8_t    len;                   /* length (bytes) */
   uint8_t    strg[ADRLEN];          /* address string */
} AtmAddr;

/* ATM address table */
typedef struct atmAddrTbl            /* ATM address table */
{
   uint16_t      count;                   /* number of ATM addresses */
   AtmAddr  tbl[MAX_ATMADDRTBL_SZ];  /* ATM address list */
} AtmAddrTbl;

typedef struct atmTfcDesc         /* ATM traffic descriptor */
{
   uint32_t  fwdPeakCellRate0;         /* forward  peak  cell rate, CLP = 0   */
   uint32_t  bwdPeakCellRate0;         /* backward peak  cell rate, CLP = 0   */
   uint32_t  fwdPeakCellRate1;         /* forward  peak  cell rate, CLP = 0+1 */
   uint32_t  bwdPeakCellRate1;         /* backward peak  cell rate, CLP = 0+1 */
   uint32_t  fwdSustCellRate0;         /* forward  sust. cell rate, CLP = 0   */
   uint32_t  bwdSustCellRate0;         /* backward sust. cell rate, CLP = 0   */
   uint32_t  fwdSustCellRate1;         /* forward  sust. cell rate, CLP = 0+1 */
   uint32_t  bwdSustCellRate1;         /* backward sust. cell rate, CLP = 0+1 */
   uint32_t  fwdMeanBurstSize0;        /* forward  mean burst size, CLP = 0   */
   uint32_t  bwdMeanBurstSize0;        /* backward mean burst size, CLP = 0   */
   uint32_t  fwdMeanBurstSize1;        /* forward  mean burst size, CLP = 0+1 */
   uint32_t  bwdMeanBurstSize1;        /* backward mean burst size, CLP = 0+1 */
   Bool bstEffortReq;             /* best effort requested */
   Bool fwdTagReq;                /* tagging requested in forward  direction */
   Bool bwdTagReq;                /* tagging requested in backward direction */
} AtmTfcDesc;

#endif /* CMFILE_REORG_1 */


/* scc typedefs */

typedef S16 Baud;                 /* baud */

typedef S16 PhysIntType;          /* physical interface type */

typedef S16 PathConnType;         /* path connection type */

typedef S16 Parity;               /* parity */

typedef S16 CharLength;           /* character length */

typedef S16 StopBits;             /* stop bits */

/* tcp/ip typedefs */

typedef uint32_t IpAddr;               /* IP address */

typedef uint16_t Port;                 /* TCP/UDP port */

typedef uint8_t Cmd;                   /* command */

typedef uint8_t Flags;                 /* TCP/UDP flags */

typedef uint8_t Ttl;                   /* time to live */

typedef uint8_t Prec;                  /* TCP/UDP precedence */

typedef uint32_t Window;               /* TCP/UDP window */

typedef uint8_t MtpStatus;             /* MTP status */

typedef uint8_t Credit;                /* credit */

/* ISUP typedefs */

typedef uint32_t CirId;                /* circuit Id */

typedef uint16_t Cic;                  /* cic */

typedef uint32_t SiInstId;             /* instance id */

/* B-ISUP typedefs */

typedef uint32_t BiInstId;             /* instance id */

/* TUP typedefs */

typedef uint32_t TpInstId;             /* instance id */

/* LLC/SNAP definitions */

typedef uint32_t Oui;                 /* 3-octet OUI in SNAP header */
typedef uint16_t Pid;                 /* 2-octet protocol id in SNAP header */
typedef uint32_t LlcId;               /* LLC id */


#ifndef CMFILE_REORG_1

/* q.93b typedefs */
 
typedef S32 AmInstId;             /* service user/provider instance id */
typedef uint16_t AmEndptRefType;       /* endpoint reference */
typedef uint32_t AmSeqNmb;             /* sequence number */
 
/* q.saal typedefs */
 
typedef uint16_t AsErrorCode;          /* q.saal error code (Q.SAAL1 Appendix 1) */

/* ume typedefs */

typedef uint32_t UmInteger;            /* ume integer */
 
typedef struct umObjId            /* ume object identifier */
{
   Data length;                   /* length of string */
   UmInteger strg[UM_MAXLEN_OBJ_ID]; /* string of sub-identifiers */
} UmObjId;

typedef struct umMib UmMib;       /* ume mib */


/* 
 * LAN Emulation typedefs 
 */

/* general typedefs */

typedef uint16_t LecId;               /* LEC Id */
typedef uint8_t  Protocol;            /* protocol */
typedef uint8_t  Version;             /* version */
typedef uint16_t OpCode;              /* op code in control frames */
typedef uint16_t LaneStatus;          /* status in control frames */
typedef uint32_t TransId;             /* transaction id */
typedef uint16_t LaneFlags;           /* flags in control frames */
typedef uint8_t  LanType;             /* LAN type */
typedef uint8_t  MtuIdx;              /* max frame size - index */
typedef uint16_t MtuVal;              /* max frame size - value */
typedef uint16_t Tag;                 /* tag indicating LAN destination type */
typedef uint8_t  VccNature;           /* VCC nature - SVC, PVC, etc. */
typedef uint8_t  VccType;             /* VCC type - control, data, etc */
typedef uint8_t  ProfileId;           /* HCC profile id */


typedef struct lanName          /* LAN name */
{
   uint8_t length;                   /* length of string */
   uint8_t strg[MAX_LANNAME];        /* name string */
} LanName;

/* LAN destination typedefs */

typedef struct macAddr          /* MAC address */
{
   uint8_t strg[MACADDRLEN];         /* address string */
} MacAddr;

typedef struct macAddrTblEntry  /* entry in table of MAC addresses */
{
   uint8_t proxyClass;               /* proxy class - local, learned */
   MacAddr macAddr;             /* MAC address */
} MacAddrTblEntry;

typedef struct macAddrTbl       /* table of MAC addresses */
{
   uint8_t count;                    /* number of entries */
   MacAddrTblEntry tbl[MAX_MACADDRTBL]; /* table of MAC addresses */
} MacAddrTbl;

typedef struct rd               /* route designator */
{
   uint16_t lanId;                   /* LAN id, segment id - 12 bits */
   uint8_t  bridgeId;                /* bridge id - 4 bits */
} Rd;

typedef struct rdTblEntry       /* entry in table of route designators */
{
   Rd rd;                       /* route designator */
} RdTblEntry;

typedef struct rdTbl            /* table of route designators */
{
   uint8_t count;                    /* number of entries */
   RdTblEntry tbl[MAX_RDTBL];/* table of route designator */
} RdTbl;

typedef struct lanDst           /* LAN destination - MAC addr, route desg */
{
   Tag tag;                     /* LAN destination type */
   union
   {
      MacAddr macAddr;          /* MAC address */
      Rd rd;                    /* route designator */
   } ld;
} LanDst;


/* control frame typedefs */

typedef struct laneCtrlHdr      /* control frame header */
{
   LecId      marker;           /* control frame marker */
   Protocol   protocol;         /* LANE protocol */
   Version    version;          /* LANE version */
   OpCode     opCode;           /* operation code */
   LaneStatus status;           /* request status */
   TransId    transId;          /* transaction id */
   LecId      reqLecId;         /* requestor LEC id */
   LaneFlags  flags;            /* bit flags */
   LanDst     srcLanDst;        /* source LAN destination */
   LanDst     dstLanDst;        /* target LAN destination */
   Addrs      srcAtmAddr;       /* source ATM address */
   Addrs      dstAtmAddr;       /* target ATM address */
   LanType    lanType;          /* LAN type */
   MtuIdx     mtuIdx;           /* MTU */
   uint8_t         nmbTLV;           /* number of TLV entries in list */
   LanName    lanName;          /* LAN name */
} LaneCtrlHdr;

typedef struct laneTLVEnt       /* type-length-value entry */
{
   uint32_t type;                    /* type of value */
   uint8_t  length;                  /* length of value */
   uint8_t  value[MAX_TLV_LEN];      /* value */
} LaneTLVEnt;

typedef struct laneCfg          /* configuration frame */
{
   LaneTLVEnt  tlvTbl[MAX_TLV_TBL]; /* list of TLV entries */
} LaneCfg;

typedef struct laneCtrlFrame    /* all control frames */
{
   LaneCtrlHdr  hdr;            /* frame header */
   union                        /* remainder of frame */
   {
      LaneCfg   cfg;            /* configuration frame */
   } m;
} LaneCtrlFrame;

/* PNNI typedefs */

/* pnni port id */
typedef uint32_t PnPortId;

/* pnni node Id */
typedef struct pnNodeId
{
   uint8_t id[PN_NODEID_LEN];
} PnNodeId;

#endif /* CMFILE_REORG_1 */



/* header typedefs */

typedef struct octStrg            /* octet string */
{
   S32 length;                    /* length */
   uint8_t val[MF_SIZE_TKNSTR];        /* value */
} OctStrg;

typedef struct tknHdr             /* token header */
{
   uint8_t   pres;                     /* present */
   uint8_t   spare1;                   /* for 16 bit alignment */
   uint16_t  spare2;                   /* for 32 bit alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare3;                   /* for 64 bit alignment */
#endif
} TknHdr;

typedef struct elmtHdr            /* element header */
{
   uint8_t   pres;                     /* present */
   uint8_t   actnInd;                  /* action indicator */
   uint16_t  compInd;                  /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare;                    /* for 64 bit alignment */
#endif
} ElmtHdr;

/* token typedefs */

typedef struct tknUInt8              /* token uint8_t */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   val;                      /* value */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
} TknUInt8;

typedef struct tknS8              /* token S8 */
{
   uint8_t   pres;                     /* present flag */
   S8   val;                      /* value */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
} TknS8;

typedef struct tknUInt16             /* token uint16_t */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   spare1;                   /* for alignment */
   uint16_t  val;                      /* value */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
} TknUInt16;

typedef struct tknUInt32             /* token uint32_t */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   spare1;                   /* for alignment */
   uint16_t  spare2;                   /* for alignment */
   uint32_t  val;                      /* value */
} TknUInt32;

typedef struct tknS32             /* token S32 */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   spare1;                   /* for alignment */
   uint16_t  spare2;                   /* for alignment */
   S32  val;                      /* value */
} TknS32;

typedef struct tknStrS            /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[(MF_SIZE_TKNSTRS + 7) & 0xff8]; /* string value */
#else
   uint8_t   val[(MF_SIZE_TKNSTRS + 3) & 0xffc]; /* string value */
#endif
} TknStrS;

typedef struct tknStrM            /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[(MF_SIZE_TKNSTRM + 7) & 0xff8]; /* string value */
#else
   uint8_t   val[(MF_SIZE_TKNSTRM + 3) & 0xffc]; /* string value */
#endif
} TknStrM;

typedef struct tknStr             /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[(MF_SIZE_TKNSTR + 7) & 0xff8]; /* string value */
#else
   uint8_t   val[(MF_SIZE_TKNSTR + 3) & 0xffc]; /* string value */
#endif
} TknStr;

typedef struct tknStrE            /* token string extended */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[(MF_SIZE_TKNSTRE + 7) & 0xff8]; /* string value */
#else
   uint8_t   val[(MF_SIZE_TKNSTRE + 3) & 0xffc]; /* string value */
#endif
} TknStrE;

typedef struct tknStrXL           /* token string extra long */
{
   uint16_t  len;                      /* length */
   uint8_t   pres;                     /* present flag */
   uint8_t   spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint8_t   *val;                     /* string value (use allocated memory) */
} TknStrXL;
 
typedef struct tknStr4            /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[8];                   /* string value - 8 byte alligned */
#else
   uint8_t   val[4];                   /* string value - 4 byte alligned */
#endif /* ALIGN_64BIT */
} TknStr4;

typedef struct tknStr12           /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[16];                  /* string value - 8 byte alligned */
#else
   uint8_t   val[12];                  /* string value - 4 byte alligned */
#endif /* ALIGN_64BIT */
} TknStr12;

typedef struct tknStr32           /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint8_t   val[32];                  /* string value - 4 byte alligned */
} TknStr32;

typedef struct tknStr64           /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint8_t   val[64];                  /* string value - 4 byte alligned */
} TknStr64;

typedef struct tknStr132          /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[136];                 /* string value - 8 byte alligned */
#else
   uint8_t   val[132];                 /* string value - 4 byte alligned */
#endif /* ALIGN_64BIT */
} TknStr132;

typedef struct tknStr256          /* token string */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   uint8_t   val[256];                 /* string value - 4 byte alligned */
} TknStr256;

typedef struct tknOid             /* Object Identifier */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* length */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
#endif
   /* gen_x_001.main_81 : changed val from uint32_t to uint16_t with comilation flag
    * TKNOID_UINT16 */
#ifndef TKNOID_UINT16
   uint32_t  val[32];                  /* max 32 integers of less than 64k value */
#else
   uint16_t  val[32];                  /* max 32 integers of less than 64k value */
#endif
} TknOid;

typedef struct tknBits            /* token bits */
{
   uint8_t   pres;                     /* present flag */
   uint8_t   len;                      /* for alignment */
   uint16_t  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   uint32_t  spare2;                   /* for 64 bit alignment */
   uint8_t   val[(MF_SIZE_TKNBITS + 7) & 0xff8]; /* string value */
#else
   uint8_t   val[(MF_SIZE_TKNBITS + 3) & 0xffc]; /* string value */
#endif
} TknBits;

typedef struct elmtStr            /* element string */
{
   ElmtHdr eh;                    /* element header */
   TknStr str;                    /* network specific information */
} ElmtStr;

typedef struct cdPtyNmb           /* called party number tokens */
{
   ElmtHdr eh;                    /* element header */
   TknUInt8 nmbPlanId;               /* numbering plan identification */
   TknUInt8 typeNmb0;                /* type of number */
#ifdef CDPTYNMB_32DIGIT
   TknStrM nmbDigits;             /* number digits */
#else
   TknStrS nmbDigits;             /* number digits */
#endif /* CDPTYNMB_32DIGIT */
} CdPtyNmb;

typedef struct redirNmb           /* redirecting number tokens */
{
   ElmtHdr eh;                    /* element header */
   TknUInt8 nmbPlanId;               /* numbering plan identification */
   TknUInt8 typeNmb;                 /* type of number */
   TknUInt8 screenInd;               /* screening indicator */
   TknUInt8 presInd;                 /* presentation indicator */
   TknUInt8 rsnRedirect;             /* reason for redirection */
   TknStrS nmbDigits;             /* number digits */
} RedirNmb;

typedef struct srvClass           /* service class */          
{                                                       
   uint8_t type;                       /* type */                 
   union                                                
   {                                                    
      struct                      /* frame relay */                                
      {                                                 
         Bool cr;                 /* command response bit */
         Bool de;                 /* discard eligibility bit */ 
      } fr;                                             
      struct                      /* mac */
      {                                                 
         Prior prior;             /* priority */            
      } ma;                                             
   } s;                                                 
} SrvClass;                                              

/* ip header, without options */

typedef struct _ip           /* ip header */
{
   uint8_t     ip_hl;            /* header length */
   Prec   ip_tos;           /* type of service */
   uint16_t    ip_len;           /* total length */
   uint16_t    ip_id;            /* identification */
   uint16_t    ip_off;           /* fragment offset field */
   Ttl    ip_ttl;           /* time to live */
   uint8_t     ip_p;             /* protocol */
   uint16_t    ip_sum;           /* checksum */
   IpAddr ip_src;           /* source address */
   IpAddr ip_dst;           /* dest address */
} Ip;
 

/* ATM structures */
 
#ifndef CMFILE_REORG_1

/* info elements that can be used for routing calls */
 
typedef struct amCdPtySad       /* Called Party Sub Address Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   oddEvenInd;          /* odd/even indicator */
   TknUInt8   typeSad;             /* type of sub address */
   TknStrS  sadInfo;            /* address/number information */
} AmCdPtySad;
 
typedef struct amBHiLyrInfo     /* Broadband High Layer Information Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   hiLyrInfoType;       /* high layer information type */
   TknStrS  hiLyrInfo;          /* high layer information */
} AmBHiLyrInfo;
 
typedef struct amBBearCap       /* Broadband Bearer Capability Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   bearClass;           /* bearer class */
   TknUInt8   timingReq;           /* timing requirement */
   TknUInt8   tfcType;             /* traffic type */
   TknUInt8   atmTfrCap;           /* ATM transfer capability */
   TknUInt8   usrPlaneConCfg;      /* user plane connection configuration */
   TknUInt8   suscClip;            /* susceptability to clipping */
} AmBBearCap;

typedef struct amQosParam       /* Quality of Service Parameter Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   qosClassFwd;         /* quality of service class forward */
   TknUInt8   qosClassBwd;         /* quality of service class backward */
} AmQosParam;

typedef struct amEtoeDly        /* End To End Transit Delay Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   cumTransDlyId;       /* cumulative transit delay id */
   TknUInt16  cumTransDly;         /* cumulative transit delay value */
   TknUInt8   reqTransDlyId;       /* requested max end to end transit delay id */
   TknUInt16  reqTransDly;         /* maximum end to end transit delay value */
   TknUInt8   pnniAccFMCTDId;      /* PNNI acceptable forward max. CTD ID */
   TknUInt32  pnniAccFMCTD;        /* PNNI acceptable forward max. CTD */
   TknUInt8   pnniCumFMCTDId;      /* PNNI acceptable forward max. CTD ID */
   TknUInt32  pnniCumFMCTD;        /* PNNI acceptable forward max. CTD */
   TknUInt8   netGenInd;           /* network generated indicator */
} AmEtoeDly;

typedef struct amOamTfcDesc     /* OAM Traffic Descriptor Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   usrNetFaultMgmt;     /* user network fault management indicator */
   TknUInt8   compInd;             /* compliance indicator */
   TknUInt8   shapingInd;          /* shaping indicator */
   TknUInt8   bwdEtoeOamF5FlInd;   /* backward e-to-e OAM F5 flow indicator */
   TknUInt8   fwdEtoeOamF5FlInd;   /* forward e-to-e OAM F5 flow indicator */
} AmOamTfcDesc;

typedef struct amEndptRef       /* Endpoint Reference Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   endptRefType;        /* endpoint reference type */
   TknUInt16  endptRefVal;         /* endpoint reference value */
} AmEndptRef;

typedef struct amAalParam       /* AAL Parameters Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   aalType;             /* AAL type */

   /* Token definition for AAL-1 */
   TknUInt8   subTypeId;           /* Subtype Identifier */
   TknUInt8   subType;             /* Subtype */
   TknUInt8   cbrRateId;           /* CBR Rate Identifier */
   TknUInt8   cbrRate;             /* CBR Rate */
   TknUInt8   multId;              /* Multiplier Identifier */
   TknUInt16  multVal;             /* Multiplier value */
   TknUInt8   srcClkFreqMetId;     /* Source clock Frequency method identifier */
   TknUInt8   srcClkFreqMet;       /* Source Clock frequency method */
   TknUInt8   errCrMetId;          /* Error correction method identifier */
   TknUInt8   errCrMet;            /* Error correction method */
   TknUInt8   strDatTxBlkszId;     /* Structured data transfer blocksize Id. */
   TknUInt8   strDatTxBlksz0;      /* Structured data transfer blocksize - oct 1*/

   /* Token definition for AAL-1, except in UNI 3.0 */
   TknUInt8   strDatTxBlksz1;      /* Structured data transfer blocksize - oct 2*/

   /* Token definition for AAL-1 */
   TknUInt8   prtFillCellId;       /* Partially filled cells Identifier */
   TknUInt8   prtFillCellMet;      /* Partially filled cells method */

   /* Token definition for AAL-3/4 and AAL-5 */
   TknUInt8   fwdMaxCpcsSduSzId;   /* Forward maximum CPCS-SDU size identifier */
   TknUInt16  fwdMaxCpcsSduSz;     /* Forward maximum CPCS-SDU size */
   TknUInt8   bwdMaxCpcsSduSzId;   /* Forward maximum CPCS-SDU size identifier */
   TknUInt16  bwdMaxCpcsSduSz;     /* Forward maximum CPCS-SDU size */

   /* Token definition for AAL-3/4 only */
   TknUInt8   midRangeId;          /* MID Range identifier */
   TknUInt16  loMidRange;          /* MID Range value */

   /* Token definition for AAL-3/4 only, except in UNI 3.0 */
   TknUInt16  hiMidRange;          /* MID Range value */

   /* Token definition for AAL-3/4 and AAL-5 and only for UNI 3.0 */
   TknUInt8   modeId;              /* Mode identifier */
   TknUInt8   mode;                /* Mode - Streaming/Message */

   /* Token definition for AAL-3/4 and AAL-5 */
   TknUInt8   sscsTypeId;          /* SSCS Type Identifier */
   TknUInt8   sscsType;            /* SSCS Type */

   /* Token definition for User defined AAL */
   TknUInt32  usrDefAalInfo;       /* User defined AAL information */
} AmAalParam;

typedef struct amBLoLyrInfo     /* Broadband Low Layer Information Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   usrInfoLyr1Prot;     /* user information layer 1 protocol */
   TknUInt8   lyr1Id;              /* Layer 1 id */
   TknUInt8   usrInfoLyr2Prot;     /* user information layer 2 protocol */
   TknUInt8   lyr2Id;              /* Layer 2 id */
   TknUInt8   q933Use;             /* Q.933 use */
   TknUInt8   lyr2OprMode;         /* Mode of operation */
   TknUInt8   winSize;             /* Window size */
   TknUInt8   usrSpecLyr2ProtInfo; /* User specified layer 2 protocol info */
   TknUInt8   usrInfoLyr3Prot;     /* user information layer 3 protocol */
   TknUInt8   lyr3Id;              /* Layer 3 id */
   TknUInt8   lyr3OprMode;         /* Mode of operation */
   TknUInt8   defPktSize;          /* Default packet size */
   TknUInt8   pktWinSize;          /* Default packet size */
   TknUInt8   usrSpecLyr3ProtInfo; /* User specified layer 3 protocol info */
   TknUInt8   initProtId;          /* Initial protocol Identifier bits 8-2 */
   TknUInt8   snapId;              /* SNAP identifier */
   TknUInt32  oui;                 /* Organisation unique identifier */
   TknUInt16  protId;              /* Protocol identifier */
} AmBLoLyrInfo;

typedef struct amAtmTfcDesc     /* ATM Traffic Descriptor Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   fwdPeakCellRateId0;  /* forward  peak  cell rate id, CLP = 0 */
   TknUInt32  fwdPeakCellRate0;    /* forward  peak  cell rate,    CLP = 0   */
   TknUInt8   bwdPeakCellRateId0;  /* backward peak  cell rate id, CLP = 0 */
   TknUInt32  bwdPeakCellRate0;    /* backward peak  cell rate,    CLP = 0   */
   TknUInt8   fwdPeakCellRateId1;  /* forward  peak  cell rate id, CLP = 0+1 */
   TknUInt32  fwdPeakCellRate1;    /* forward  peak  cell rate,    CLP = 0+1 */
   TknUInt8   bwdPeakCellRateId1;  /* backward peak  cell rate id, CLP = 0+1 */
   TknUInt32  bwdPeakCellRate1;    /* backward peak  cell rate,    CLP = 0+1 */
   TknUInt8   fwdSustCellRateId0;  /* forward  sust. cell rate id, CLP = 0 */
   TknUInt32  fwdSustCellRate0;    /* forward  sust. cell rate,    CLP = 0   */
   TknUInt8   bwdSustCellRateId0;  /* backward sust. cell rate id, CLP = 0 */
   TknUInt32  bwdSustCellRate0;    /* backward sust. cell rate,    CLP = 0   */
   TknUInt8   fwdSustCellRateId1;  /* forward  sust. cell rate id, CLP = 0+1 */
   TknUInt32  fwdSustCellRate1;    /* forward  sust. cell rate,    CLP = 0+1 */
   TknUInt8   bwdSustCellRateId1;  /* backward sust. cell rate id, CLP = 0+1 */
   TknUInt32  bwdSustCellRate1;    /* backward sust. cell rate,    CLP = 0+1 */
   TknUInt8   fwdMeanBurstSizeId0; /* forward  mean burst size id, CLP = 0 */
   TknUInt32  fwdMeanBurstSize0;   /* forward  mean burst size,    CLP = 0   */
   TknUInt8   bwdMeanBurstSizeId0; /* backward mean burst size id, CLP = 0 */
   TknUInt32  bwdMeanBurstSize0;   /* backward mean burst size,    CLP = 0   */
   TknUInt8   fwdMeanBurstSizeId1; /* forward  mean burst size id, CLP = 0+1 */
   TknUInt32  fwdMeanBurstSize1;   /* forward  mean burst size,    CLP = 0+1 */
   TknUInt8   bwdMeanBurstSizeId1; /* backward mean burst size id, CLP = 0+1 */
   TknUInt32  bwdMeanBurstSize1;   /* backward mean burst size,    CLP = 0+1 */
   TknUInt8   bstEffortReq;        /* best effort requested */
   TknUInt8   tfcMgmtOptId;        /* traffic management options identifier */
   TknUInt8   fwdTagReq;           /* tagging requested in forward  direction */
   TknUInt8   bwdTagReq;           /* tagging requested in backward direction */
   TknUInt8   bwdFrmDisc;          /* frame discard in backward direction */
   TknUInt8   fwdFrmDisc;          /* frame discard in forward  direction */
   TknUInt8   fwdAbrMinCellRateId; /* forward  ABR min. cell rate id, CLP = 0+1 */
   TknUInt32  fwdAbrMinCellRate;   /* forward  ABR min. cell rate,    CLP = 0+1 */
   TknUInt8   bwdAbrMinCellRateId; /* backward ABR min. cell rate id, CLP = 0+1 */
   TknUInt32  bwdAbrMinCellRate;   /* backward ABR min. cell rate,    CLP = 0+1 */
} AmAtmTfcDesc;

typedef struct amCauseDgn       /* Cause and Diagnostics Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   location;            /* location */
   TknUInt8   causeVal;            /* cause value */
   TknStrM  dgnVal;              /* Diagnostics value */
} AmCauseDgn;

typedef struct amCgPtyNmb       /* Calling Party Number Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   nmbPlanId;           /* address/numbering plan identification */
   TknUInt8   typeNmb;             /* type of number */
   TknUInt8   screenInd;           /* screening indicator */
   TknUInt8   presInd;             /* presentation indicator */
   TknStrS nmbDigits;           /* address/number information */
} AmCgPtyNmb;

typedef struct amCgPtySad       /* Calling Party Sub Address Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   oddEvenInd;          /* odd/even indicator */
   TknUInt8   typeSad;             /* type of sub address */
   TknStrS sadInfo;             /* address/number information */
} AmCgPtySad;

typedef struct amNBearCap       /* Narrowband Bearer Capability Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8 infoTranCap;           /* information transfer capability */
   TknUInt8 codingStd;             /* coding standard */
   TknUInt8 infoTranRate0;         /* information transfer rate */
   TknUInt8 tranMode;              /* transfer mode */
   TknUInt8 establish;             /* establishment */
   TknUInt8 cfg;                   /* configuration */
   TknUInt8 chanStruct;            /* structure */
   TknUInt8 infoTranRate1;         /* information transfer rate */
   TknUInt8 symmetry;              /* symmetry */
   TknUInt8 usrInfoLyr1Prot;       /* usr information layer 1 protocol */
   TknUInt8 lyr1Ident;             /* layer 1 identity */
   TknUInt8 usrRate;               /* user rate */
   TknUInt8 negot;                 /* negotiation */
   TknUInt8 syncAsync;             /* synchronous/asynchronous */
/*------------------------------------------------------------------------*/
/* The following six tokens represent a uinon of octets 5b.1 and 5b.2 of  */
/* the Narrow Band Bearer Capability.                                     */
/*------------------------------------------------------------------------*/
   TknUInt8 FlcRx_BandNeg;         /* flow control on reception or 
                                   inband/outband negotiation */
   TknUInt8 FlcTx_Assgn;           /* flow control on transmission or 
                                   assignor/assignee*/
   TknUInt8 NicRx_LLINeg;          /* network independent clock on reception or 
                                   logical link identifier negotiation */
   TknUInt8 NicTx_Mode;            /* network independent clock on transmission or
                                   mode of operation */
   TknUInt8 Rate_MFrm;             /* intermediate rate (low bit) or Multi 
                                   frame support */
   TknUInt8 Rate_Hdr;              /* intermediate rate (high bit) or rate 
                                   adaptation Hdr/ no Headr */
   TknUInt8 parity;                /* parity information */
   TknUInt8 nmbDatBits;            /* number of data bits excluding parity bit */
   TknUInt8 nmbStopBits;           /* number of stop bits */
   TknUInt8 modemType;             /* modem type */
   TknUInt8 duplexMode;            /* duplex mode */
   TknUInt8 usrInfoLyr2Prot;       /* user information layer 2 protocol */
   TknUInt8 lyr2Ident;             /* layer 2 identity */
   TknUInt8 usrInfoLyr3Prot;       /* user information layer 3 protocol */
   TknUInt8 lyr3Ident0;            /* layer 3 identity */
} AmNBearCap;

typedef struct amNLoLyrComp     /* Narrowband Low Layer Compatibility Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8 infoTranCap;           /* information transfer capability */
   TknUInt8 codingStd;             /* coding standard */
   TknUInt8 negInd;                /* negotiation indicator */
   TknUInt8 infoTranRate0;         /* information transfer rate */
   TknUInt8 tranMode;              /* transfer mode */
   TknUInt8 establish;             /* establishment */
   TknUInt8 cfg;                   /* configuration */
   TknUInt8 chanStruct;            /* structure */
   TknUInt8 infoTranRate1;         /* information transfer rate */
   TknUInt8 symmetry;              /* symmetry */
   TknUInt8 usrInfoLyr1Prot;       /* usr information layer 1 protocol */
   TknUInt8 lyr1Ident;             /* layer 1 identity */
   TknUInt8 usrRate;               /* user rate */
   TknUInt8 negot;                 /* negotiation */
   TknUInt8 syncAsync;             /* synchronous/asynchronous */
/*------------------------------------------------------------------------*/
/* The following six tokens represent a uinon of octets 5b.1 and 5b.2 of  */
/* the Narrow Band Lower Layer Compatibility                              */
/*------------------------------------------------------------------------*/
   TknUInt8 FlcRx_BandNeg;         /* flow control on reception or 
                                   inband/outband negotiation */
   TknUInt8 FlcTx_Assgn;           /* flow control on transmission or 
                                   assignor/assignee*/
   TknUInt8 NicRx_LLINeg;          /* network independent clock on reception or 
                                   logical link identifier negotiation */
   TknUInt8 NicTx_Mode;            /* network independent clock on transmission or
                                   mode of operation */
   TknUInt8 Rate_MFrm;             /* intermediate rate (low bit) or Multi 
                                   frame support */
   TknUInt8 Rate_Hdr;              /* intermediate rate (high bit) or rate 
                                   adaptation Hdr/ no Headr */
   TknUInt8 parity;                /* parity information */
   TknUInt8 nmbDatBits;            /* number of data bits excluding parity bit */
   TknUInt8 nmbStopBits;           /* number of stop bits */
   TknUInt8 modemType;             /* modem type */
   TknUInt8 duplexMode;            /* duplex mode */
   TknUInt8 usrInfoLyr2Prot;       /* user information layer 2 protocol */
   TknUInt8 lyr2Ident;             /* layer 2 identity */
   TknUInt8 optLyr2ProtInfo;       /* optional layer 2 protocol information */
   TknUInt8 usrInfoLyr3Prot;       /* user information layer 3 protocol */
   TknUInt8 lyr3Ident1;            /* layer 3 identity */
   TknUInt8 optLyr3ProtInfo;       /* optional layer 3 protocol information */
} AmNLoLyrComp;

typedef struct amNHiLyrComp     /* Narrowband High Layer Compatibility Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8 presMethod;            /* presentation method */
   TknUInt8 interpretation;        /* interpretation */
   TknUInt8 codingStd;             /* coding standard */
   TknUInt8 highLyrCharId;         /* high layer characteristics identification */
   TknUInt8 extHighLyrCharId;      /* extended high layer characteristics id */
} AmNHiLyrComp;

typedef struct amProgInd        /* Progress Indicator Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8 progLocation;          /* location */
   TknUInt8 codingStd;             /* coding standard */
   TknUInt8 progDesc;              /* progress description */
} AmProgInd;

#if (DEF_SIG_PNNI | DEF_SIG_AINI)
typedef struct amCrankback      /* Crankback Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   cbLevel;             /* crankback level */
   TknUInt8   blkTransType;        /* blocked transit type */
#ifdef SIG_PNNI
   TknStrS blkNodeId;           /* blocked node id */
   TknStrS blkLinkPreNodeId;    /* blocked link's preceeding node id */
   TknUInt32  blkLinkPortId;       /* blocked link port id */
   TknStrS blkLinkSuccNodeId;   /* blocked link's succeding node id */
#endif /* SIG_PNNI */
   TknUInt8   cbCause;             /* crankback cause */
#ifdef SIG_PNNI
   TknStrM cbDgn;               /* crankback cause diagnostics */
#endif /* SIG_PNNI */
} AmCrankback;

typedef struct amCdPtySoftPvc   /* Called Party Soft PVPC/PVCC Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   selType;             /* VPI/VCI selection Type */
   TknUInt8   vpiId;               /* VPI id */
   TknUInt16  vpi;                 /* VPI */
   TknUInt8   vciId;               /* VCI id */
   TknUInt16  vci;                 /* VCI */
#ifdef SPVC_FR
   TknUInt8   dlciId;              /* DLCI Identifer */
   TknUInt32  dlci;                /* Dlci Value */
#endif /* SPVC_FR */
} AmCdPtySoftPvc;

typedef struct amCgPtySoftPvc   /* Calling Party Soft PVPC/PVCC Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   vpiId;               /* VPI id */
   TknUInt16  vpi;                 /* VPI */
   TknUInt8   vciId;               /* VCI id */
   TknUInt16  vci;                 /* VCI */
#ifdef SPVC_FR
   TknUInt8   dlciId;              /* DLCI Identifer */
   TknUInt32  dlci;                /* Dlci Value */
#endif /* SPVC_FR */
} AmCgPtySoftPvc;
#endif /* DEF_SIG_PNNI | DEF_SIG_AINI */

typedef struct amConnNmb        /* Connected Number Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   nmbPlanId;           /* address/numbering plan identification */
   TknUInt8   typeNmb;             /* type of number */
   TknUInt8   screenInd;           /* screening indicator */
   TknUInt8   presInd;             /* presentation indicator */
   TknStrS nmbDigits;           /* address/number information */
} AmConnNmb;

#if (DEF_SIG_PNNI | DEF_SIG_AINI | DEF_UNI40)
typedef struct amConnSad        /* Calling Party Sub Address Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   oddEvenInd;          /* odd/even indicator */
   TknUInt8   typeSad;             /* type of sub address */
   TknStrS sadInfo;             /* address/number information */
} AmConnSad;

typedef struct amGenIdTrans     /* Generic Identifier Transport Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknStrM genId;               /* generic identifier */
} AmGenIdTrans;

typedef AmAtmTfcDesc   AmAltAtmTfcDesc;  /* Alternative ATM Traffic Desc. */

typedef struct amMinAccAtmTfcDesc  /* Minimum acceptable ATM Traffic Desc. */
{
   ElmtHdr eh;                     /* element header */
   TknUInt8   fwdPeakCellRateId0;     /* forward  peak  cell rate id, CLP = 0 */
   TknUInt32  fwdPeakCellRate0;       /* forward  peak  cell rate,    CLP = 0   */
   TknUInt8   bwdPeakCellRateId0;     /* backward peak  cell rate id, CLP = 0 */
   TknUInt32  bwdPeakCellRate0;       /* backward peak  cell rate,    CLP = 0   */
   TknUInt8   fwdPeakCellRateId1;     /* forward  peak  cell rate id, CLP = 0+1 */
   TknUInt32  fwdPeakCellRate1;       /* forward  peak  cell rate,    CLP = 0+1 */
   TknUInt8   bwdPeakCellRateId1;     /* backward peak  cell rate id, CLP = 0+1 */
   TknUInt32  bwdPeakCellRate1;       /* backward peak  cell rate,    CLP = 0+1 */
} AmMinAccAtmTfcDesc;

typedef struct amExtQosParam       /* Extended QOS parameter */
{
   ElmtHdr eh;                     /* element header */
   TknUInt8   origin;                 /* origin of this IE */
   TknUInt8   accFwdPpCDVId;          /* acceptable forward  peak-to-peak cell 
                                    * delay variation identifier */
   TknUInt32  accFwdPpCDV;            /* acceptable forward  peak-to-peak cell
                                    * delay variation */
   TknUInt8   accBwdPpCDVId;          /* acceptable backward peak-to-peak cell
                                    * delay variation identifier */
   TknUInt32  accBwdPpCDV;            /* acceptable backward peak-to-peak cell
                                    * delay variation */
   TknUInt8   cumFwdPpCDVId;          /* cumulative forward  peak-to-peak cell
                                    * delay variation identifier */
   TknUInt32  cumFwdPpCDV;            /* cumulative forward  peak-to-peak cell
                                    * delay variation */
   TknUInt8   cumBwdPpCDVId;          /* cumulative backward peak-to-peak cell
                                    * delay variation identifier */
   TknUInt32  cumBwdPpCDV;            /* cumulative backward peak-to-peak cell
                                    * delay variation */
   TknUInt8   accFwdCLRId;            /* acceptable forward  cell loss ratio
                                    * identifier */
   TknUInt8   accFwdCLR;              /* acceptable forward  cell loss ratio */
   TknUInt8   accBwdCLRId;            /* acceptable backward cell loss ratio 
                                    * identifier */
   TknUInt8   accBwdCLR;              /* acceptable backward cell loss ratio */
} AmExtQosParam;

typedef struct amAbrAddParam       /* ABR additional parameters */
{
   ElmtHdr eh;                     /* element header */
   TknUInt8   fwdAddParamRecId;       /* forward  additional parameters record identifier */
   TknUInt32  fwdAddParamRec;         /* forward  additional parameters record */
   TknUInt8   bwdAddParamRecId;       /* backward additional parameters record identifier */
   TknUInt32  bwdAddParamRec;         /* backward additional parameters record */
} AmAbrAddParam;

typedef struct amAbrSetupParam     /* ABR Setup parameters */
{
   ElmtHdr eh;                     /* element header */
   TknUInt8   fwdAbrICRId;            /* forward  ABR initial cell rate identifier */
   TknUInt32  fwdAbrICR;              /* forward  ABR initial cell rate */
   TknUInt8   bwdAbrICRId;            /* backward ABR initial cell rate identifier */
   TknUInt32  bwdAbrICR;              /* backward ABR initial cell rate */
   TknUInt8   fwdAbrTBEId;            /* forward  ABR transient buffer exposure identifier */
   TknUInt32  fwdAbrTBE;              /* forward  ABR transient buffer exposure */
   TknUInt8   bwdAbrTBEId;            /* backward ABR transient buffer exposure identifier */
   TknUInt32  bwdAbrTBE;              /* backward ABR transient buffer exposure */
   TknUInt8   cumRmFRTTId;            /* cumulative RM fixed round trip time identifier */
   TknUInt32  cumRmFRTT;              /* cumulative RM fixed round trip time */
   TknUInt8   fwdRIFId;               /* forward  rate increment factor identifier */
   TknUInt8   fwdRIF;                 /* forward  rate increment factor */
   TknUInt8   bwdRIFId;               /* backward rate increment factor identifier */
   TknUInt8   bwdRIF;                 /* backward rate increment factor */
   TknUInt8   fwdRDFId;               /* forward  rate decrement factor identifier */
   TknUInt8   fwdRDF;                 /* forward  rate decrement factor */
   TknUInt8   bwdRDFId;               /* backward rate decrement factor identifier */
   TknUInt8   bwdRDF;                 /* backward rate decrement factor */
} AmAbrSetupParam;
#endif /* DEF_SIG_PNNI | DEF_SIG_AINI | DEF_UNI40 */

#if (DEF_Q2931 | DEF_SIG_PNNI | DEF_SIG_AINI | DEF_UNI40)
typedef struct amNotInd         /* Notification Indicator Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknStrE notDesc;             /* notification description */
} AmNotInd;
#endif /* DEF_Q2931 | DEF_SIG_PNNI | DEF_SIG_AINI | DEF_UNI40 */


/* AAL structure */

typedef struct aalConParam        /* connection parameters for AAL */
{
   AmAalParam     aalParam;       /* AAL Parameters */
   AmAtmTfcDesc   atmTfcDesc;     /* ATM Traffic Descriptor */
   AmBBearCap     bBearCap;       /* Broadband Bearer Capability */
   AmQosParam     qosParam;       /* Qos parameters */
   AmEtoeDly      etoeDly;        /* End to End Transit Delay */
   AmOamTfcDesc   oamTfcDesc;     /* OAM Traffic Descriptor */
} AalConParam;

typedef struct amCdPtyNmb       /* Called Party Number Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknUInt8   nmbPlanId;           /* address/numbering plan identification */
   TknUInt8   typeNmb;             /* type of number */
   TknStrS  nmbDigits;           /* address/number information */
} AmCdPtyNmb;

#endif /* CMFILE_REORG_1 */

 

/* management structures */

typedef struct smCfg            /* stack manager */
{
   Ent ent;                     /* entity */
   Inst inst;                   /* instance */
   Region region;               /* region */
   Pool pool;                   /* pool */
   Priority prior;              /* priority */
   Route route;                 /* route */
   Selector selector;           /* selector */
} SmCfg;

typedef struct mem                /* memory */
{
   Region region;                 /* region */
   Pool pool;                     /* pool */
   uint16_t spare;                     /* spare for alignment */
} Mem;  
   
typedef Mem MemoryId;             /* memory id */


typedef struct resp
{
   Selector selector;           /* selector */
   Priority prior;              /* priority */
   Route route;                 /* route */
   MemoryId mem;                /* memory */
}Resp;

typedef struct tds_header       /* header */
{
   uint16_t msgLen;                  /* message length   - optional */
   uint8_t msgType;                  /* message type     - mandatory */
   uint8_t version;                  /* version          - optional */
   uint16_t seqNmb;                  /* sequence number  - optional */
   EntityId entId;              /* entity id        - mandatory */
   ElmntId elmId;               /* element id       - mandatory */
#ifdef LMINT3
   TranId transId;              /* transaction Id - mandatory */
   Resp response;               /* response parameters - mandatory */
#endif /* LMINT3 */
} Header;

typedef struct tmrCfg           /* timer configuration structure */
{
   Bool enb;                    /* enable */
   uint16_t val;                     /* value */
} TmrCfg;

typedef struct asyncCfg         /* asynchronous configuration */
{
   StopBits stopBits;           /* stop bits */
   CharLength charLength;       /* character length */
   Parity rxParity;             /* receive parity */
   Parity txParity;             /* transmit parity */
} AsyncCfg;


/* dateTime structure */
  
typedef struct dateTime           /* date and time */
{
   uint8_t month;                      /* month */
   uint8_t day;                        /* day */
   uint8_t year;                       /* year */
   uint8_t hour;                       /* hour - 24 hour clock */
   uint8_t min;                        /* minute */
   uint8_t sec;                        /* second */
   uint8_t tenths;                     /* tenths of second */
   /*-- gen_x_001.main_90 - Added variable for microseconds in DateTime--*/
#ifdef SS_DATETIME_USEC
   uint32_t usec;                      /* micro seconds */
#endif /*-- SS_DATETIME_USEC --*/
} DateTime;
/* gen_x_001.main_94: Additions */
/* gen_x_001.main_95: Modifications */
typedef uint64_t EpcTime;

/* common status */
typedef struct cmStatus
{
   uint16_t status;       /* status of request */
   uint16_t reason;       /* failure reason */
}CmStatus;    

/* common alarm */
typedef struct cmAlarm
{
   DateTime dt;      /* data and time */
   uint16_t category;     /* alarm category*/
   uint16_t event;        /* alarm event */
   uint16_t cause;        /* alarm cause */
}CmAlarm;    
  
/* duration structure */
  
typedef struct duration           /* duration */
{
   uint8_t days;                       /* days */
   uint8_t hours;                      /* hours */
   uint8_t mins;                       /* minutes */
   uint8_t secs;                       /* seconds */
   uint8_t tenths;                     /* tenths of seconds */
} Duration;





#ifdef CMFILE_REORG_1

#ifdef SS
typedef struct ssmsgb Buffer;

#ifdef FLAT_BUFFER_OPT
typedef struct _flatBuffer
{
   uint8_t* startAddr;
   uint8_t* ptr;
   uint32_t len;
}FlatBuffer;
#endif

#else /* SS */

#ifdef WINNT_IATM                 /* Windows NT Integrated ATM */
#ifndef CFG_APP                   /* Don't include ndis.h for config app. */ 
#include "ndis.h"                 /* to support NDIS calls (listed above) */
#endif /* CFG_APP */

typedef struct _NDIS_PACKET Buffer; /* forward definition - buffer */
#else
typedef struct ss_buffer Buffer;  /* forward definition - buffer */

#endif /* WINNT_IATM */
#endif /* SS */

typedef struct tknBuf
{
   uint8_t      pres;                  /* Present Flag */
   uint8_t      spare1;                /* for alignment */
   uint16_t     spare2;                /* for 32 bit alignment */
#ifdef ALIGN_64BIT
   uint32_t     spare3;                /* for 64 bit alignment */
#endif
   Buffer  *val;                  /* Buffer type (use allocated memory) */
} TknBuf;

/* defining the CmIpAddr and CmIpAddr6 */
/* socket typedefs and structs */
typedef uint32_t CmIpAddr;        /* 4 byte IP address */
#ifdef IPV6_SUPPORTED
typedef uint8_t  CmIpAddr6[16];   /* 16 byte IPV6 address */
#endif /* IPV6_SUPPORTED */

/* common packing functions */

/* system services structures */
S16 cmPkDateTime ARGS((DateTime *dateTime, Buffer *mBuf));
S16 cmPkDuration ARGS((Duration *duration, Buffer *mBuf));
S16 oduPackPointer ARGS((PTR ptr, Buffer *mBuf));
S16 cmPkEntityId ARGS((EntityId *entityId, Buffer *mBuf));
S16 cmPkElmntId  ARGS((ElmntId  *elmntId,  Buffer *mBuf));
S16 cmPkMemoryId ARGS((MemoryId *memoryId, Buffer *mBuf));

/* general structures */
S16 cmPkSystemId ARGS((SystemId *systemId, Buffer *mBuf));
S16 cmPkAddrs    ARGS((Addrs    *addrs,    Buffer *mBuf));
S16 cmPkProtAddr ARGS((ProtAddr *protAddr, Buffer *mBuf));
S16 cmPkProtAddrTbl ARGS((ProtAddrTbl *protAddr, Buffer *mBuf));
S16 cmPkShrtAddrs ARGS((ShrtAddrs    *addrs,    Buffer *mBuf));
S16 cmPkAddrMask ARGS((uint8_t    *mask,    Buffer *mBuf));
S16 cmPkBndCfg   ARGS((BndCfg   *bndCfg,   Buffer *mBuf));
S16 cmPkPst      ARGS((Pst      *pst,      Buffer *mBuf));
S16 cmPkElmtHdr  ARGS((ElmtHdr  *m,        Buffer *mBuf));
S16 cmPkTknUInt8    ARGS((TknUInt8    *tknUInt8,    Buffer *mBuf));
S16 cmPkTknS8    ARGS((TknS8    *tknS8,    Buffer *mBuf));
S16 cmPkTknUInt16   ARGS((TknUInt16   *tknU16,   Buffer *mBuf));
S16 cmPkTknUInt32   ARGS((TknUInt32   *tknU32,   Buffer *mBuf));
S16 cmPkTknStr   ARGS((TknStr   *tknStr,   Buffer *mBuf));                              
S16 cmPkTknStrM  ARGS((TknStrM  *tknStr,   Buffer *mBuf));
S16 cmPkTknStrS  ARGS((TknStrS  *tknStr,   Buffer *mBuf));
S16 cmPkTknStrE  ARGS((TknStrE  *tknStr,   Buffer *mBuf));

S16 cmPkTknStr4   ARGS((TknStr4   *tknStr,   Buffer *mBuf));
S16 cmPkTknStr12  ARGS((TknStr12  *tknStr,   Buffer *mBuf));
S16 cmPkTknStr32  ARGS((TknStr32  *tknStr,   Buffer *mBuf));
S16 cmPkTknStr64  ARGS((TknStr64  *tknStr,   Buffer *mBuf));
S16 cmPkTknStr132 ARGS((TknStr132 *tknStr,   Buffer *mBuf));
S16 cmPkTknStr256 ARGS((TknStr256 *tknStr,   Buffer *mBuf));

 S16  cmPkTknS32    ARGS((TknS32 *tknS32, Buffer *mBuf));
 S16  cmPkTknOid    ARGS((TknOid *tknOid, Buffer *mBuf));
 S16  cmPkTknBuf    ARGS((TknBuf *tknBuf, Buffer *mBuf));

#ifdef TDS_ROLL_UPGRADE_SUPPORT
 S16  cmPkIntf      ARGS((CmIntf *intf, Buffer *mBuf));
#endif

/* layer management structures */
S16 cmPkHeader   ARGS((Header   *header,   Buffer *mBuf));
S16 cmPkSmCfg    ARGS((SmCfg    *smCfg,    Buffer *mBuf));
S16 cmPkTmrCfg   ARGS((TmrCfg   *tmrCfg,   Buffer *mBuf));
S16 cmPkCmStatus ARGS((CmStatus *status,   Buffer *mBuf));
S16 cmPkCmAlarm  ARGS((CmAlarm  *alrm,     Buffer *mBuf));


/* common unpacking functions */

/* system services structures */
S16 cmUnpkDateTime ARGS((DateTime *dateTime, Buffer *mBuf));
S16 cmUnpkDuration ARGS((Duration *duration, Buffer *mBuf));
S16 oduUnpackPointer ARGS((PTR *ptr, Buffer *mBuf));
S16 cmUnpkEntityId ARGS((EntityId *entityId, Buffer *mBuf));
S16 cmUnpkElmntId  ARGS((ElmntId  *elmntId,  Buffer *mBuf));
S16 cmUnpkMemoryId ARGS((MemoryId *memoryId, Buffer *mBuf));

/* general structures */
S16 cmUnpkSystemId ARGS((SystemId *systemId, Buffer *mBuf));
S16 cmUnpkAddrs    ARGS((Addrs    *addrs,    Buffer *mBuf));
S16 cmUnpkProtAddr ARGS((ProtAddr *protAddr, Buffer *mBuf));
S16 cmUnpkProtAddrTbl ARGS((ProtAddrTbl *protAddr, Buffer *mBuf));
S16 cmUnpkShrtAddrs ARGS((ShrtAddrs    *addrs,    Buffer *mBuf));
S16 cmUnpkAddrMask ARGS((uint8_t    *mask,    Buffer *mBuf));
S16 cmUnpkBndCfg   ARGS((BndCfg   *bndCfg,   Buffer *mBuf));
S16 cmUnpkPst      ARGS((Pst      *pst,      Buffer *mBuf));
S16 cmUnpkElmtHdr  ARGS((ElmtHdr  *m,        Buffer *mBuf));
S16 cmUnpkTknUInt8    ARGS((TknUInt8    *tknUInt8,    Buffer *mBuf));
S16 cmUnpkTknS8    ARGS((TknS8    *tknS8,    Buffer *mBuf));
S16 cmUnpkTknUInt16   ARGS((TknUInt16   *tknU16,   Buffer *mBuf));
S16 cmUnpkTknUInt32   ARGS((TknUInt32   *tknU32,   Buffer *mBuf));
S16 cmUnpkTknStr   ARGS((TknStr   *tknStr,   Buffer *mBuf));                              
S16 cmUnpkTknStrM  ARGS((TknStrM  *tknStr,   Buffer *mBuf));
S16 cmUnpkTknStrS  ARGS((TknStrS  *tknStr,   Buffer *mBuf));
S16 cmUnpkTknStrE  ARGS((TknStrE  *tknStr,   Buffer *mBuf));

S16 cmUnpkTknStr4   ARGS((TknStr4   *tknStr,   Buffer *mBuf));
S16 cmUnpkTknStr12  ARGS((TknStr12  *tknStr,   Buffer *mBuf));
S16 cmUnpkTknStr32  ARGS((TknStr32  *tknStr,   Buffer *mBuf));
S16 cmUnpkTknStr64  ARGS((TknStr64  *tknStr,   Buffer *mBuf));
S16 cmUnpkTknStr132 ARGS((TknStr132 *tknStr,   Buffer *mBuf));
S16 cmUnpkTknStr256 ARGS((TknStr256 *tknStr,   Buffer *mBuf));

 S16  cmUnpkTknS32  ARGS((TknS32 *tknS32, Buffer *mBuf));
 S16  cmUnpkTknOid  ARGS((TknOid *tknOid, Buffer *mBuf));
 S16  cmUnpkTknBuf  ARGS((TknBuf *tknBuf, Buffer **mBuf));

#ifdef TDS_ROLL_UPGRADE_SUPPORT
 S16  cmUnpkIntf    ARGS((CmIntf *intf, Buffer *mBuf));
#endif

/* layer management structures */
S16 cmUnpkHeader   ARGS((Header   *header,   Buffer *mBuf));
S16 cmUnpkSmCfg    ARGS((SmCfg    *smCfg,    Buffer *mBuf));
S16 cmUnpkTmrCfg   ARGS((TmrCfg   *tmrCfg,   Buffer *mBuf));
S16 cmUnpkCmStatus ARGS((CmStatus *status,   Buffer *mBuf));
S16 cmUnpkCmAlarm  ARGS((CmAlarm  *alrm,     Buffer *mBuf));

#endif /* CMFILE_REORG_1 */
#if defined(XEON_SPECIFIC_CHANGES) || defined(INTEL_WLS) || defined(SS_USE_WLS_MEM)
void mtGetWlsHdl(void **hdlr);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __GENX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
