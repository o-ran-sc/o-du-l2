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

typedef U8 Ent;                   /* entity */

typedef U8 Inst;                  /* instance */

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

typedef U8 Region;                /* region */

typedef U8 Pool;                  /* pool */

typedef U8 Prior;                 /* priority */

typedef Prior Priority;           /* priority */

typedef U8 Route;                 /* route */

typedef U8 Data;                  /* data */

typedef U8 Selector;              /* selector */

typedef S16 Reason;               /* reason */

typedef U16 ProcId;               /* processor id */

typedef U8 Event;                 /* event */

typedef U32 TranId;               /* transaction Id */

typedef S16 IntNum;               /* Interface Number */

typedef U16 TranNum;              /* Transaction Number */

typedef U16     CmIntfId;         /* Interface identifier       */
typedef U16     CmIntfVer;        /* Interface version          */

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
   ProcId dstProcId;              /* destination processor id (U16) */
   ProcId srcProcId;              /* source processor id      (U16) */

   Ent dstEnt;                    /* destination entity       (U8)  */
   Inst dstInst;                  /* destination instance     (U8)  */
   Ent srcEnt;                    /* source entity            (U8)  */
   Inst srcInst;                  /* source instance          (U8)  */

   Prior prior;                   /* priority                 (U8)  */   
   Route route;                   /* route                    (U8)  */
   Event event;                   /* event                    (U8)  */
   Region region;                 /* region                   (U8)  */

   Pool pool;                     /* pool                     (U8)  */
   Selector selector;             /* selector                 (U8)  */
   CmIntfVer intfVer;             /* interface version        (U16) */
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
   U16      protType;                  /* Protocol Type */
   U8       len;                       /* Protocol Address Length in bytes */
   U8       preLen;                    /* prefix length in bits */
   U8       address[MAX_PROTADDR_LEN]; /* Protocol Address */ 
#ifdef CM_ARI2
   Bool     autoSysIdPres;         /* Is Autonomous System Id Present */
   U32      autoSysId;                 /* Autonomous System Id */
#endif /* CM_ARI2 */

}ProtAddr;

typedef struct protAddrTbl
{
   U8         count;                 /* Number of addresses */
   ProtAddr   addr[MAX_PROT_ADDRS];  /* Protocol Address List */
}ProtAddrTbl;

/* addrs structure */

typedef struct addrs              /* address */
{
   U8 length;                     /* length (bytes or nibbles) */
   U8 strg[ADRLEN];               /* address */
} Addrs;

typedef struct shrtAddr           /* short address */
{
   U8 length;                     /* length bytes */
   U8 strg[SHRTADRLEN];           /* address */
} ShrtAddrs;

typedef struct lngAddr            /* long address */
{
   U8 length;                     /* length bytes */
   U8 strg[LNGADRLEN];            /* address */
} LngAddrs;

/* bndCfg structure */

typedef struct bndCfg             /* bind configuration */
{
   Txt *usrId;                    /* user id */
   U8 bufOwnshp;                  /* buffer ownership */
   U8 flcTyp;                     /* flow control type */
   U8 wdw;                        /* window */
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
   U32 cBufIdx;
   U32 cBufSize;
   U8 *cBufPtr;
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
   U32 dbgMask;                   /* debug mask */
   Txt prntBuf[255];          /* print buffer */
#endif
   Txt *prntCirBuf; /* print buffer for each system task */
#ifdef SS_DIAG
   /* gen_x_001.main_96 :Added logmask */
   U32 logMask;                   /* Logging mask */
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
 
typedef U32 StsCntr;              /* Statistics counter */

typedef S16 LnkNmb;               /* link number */

typedef S8 VBit;                      /*V Bit */

typedef S16 SuId;                 /* service user sap id */

typedef S16 SpId;                 /* service provider sap id */

typedef S16 SuInstId;             /* service user instance id */

typedef S16 SpInstId;             /* service provider instance id */

typedef U16 PortId;               /* port id */

typedef U8 Sapi;                  /* service access point id */
 
typedef U8 Tei;                   /* terminal endpoint id */

typedef U8 Ces;                   /* connection endpoint suffix */

typedef U32 Dlci;                 /* data link control identifier */ 

typedef U16 CalRef;               /* call Reference */

typedef S16 Origin;               /* origin */

typedef U16 NwId;                 /* Network Identifier */

typedef S16 Swtch;                /* switch */

typedef U8 Cause;                 /* cause code */

typedef U8 Dgn;                   /* diagnostic code */

typedef S16 Action;               /* action */

typedef S16 SeqS16;               /* signed 16 bit sequence number */
 
typedef U16 SeqU16;               /* unsigned 16 bit sequence number */
 
typedef S32 SeqS24;               /* signed 24 bit sequence number */
 
typedef U32 SeqU24;               /* unsigned 24 bit sequence number */
 
typedef U8 SetUpArb;              /* set up arbitration (PASSIVE/ACTIVE) */
 
typedef U8 EvntType;              /* event type */

typedef U8 State;                 /* state */

typedef U8 Mode;                  /* mode */

typedef S32 ConnId;               /* connection id */

typedef U32 UConnId;              /* unsigned connection id */

typedef U16 ProtId;               /* protocol id */

typedef U16 ChannelId;            /* channel id */

typedef U8  Arr64U8[64];            /* Array of 64 of type U8*/

typedef U16 Efa;                  /* Envelope Address Function */

typedef U32 BitState;             /* Sa Bit ID and Value */

typedef U8  CChanId;               /* V5UA Channel ID */

typedef U16 MibOpCode;            /* Op code for Mib Request MIB_REQUEST_GET,
                                     MIB_REQUEST_GET_FIRST, MIB_REQUEST */

typedef U16 MibStatus;            /* Status returned in Mib Cfm */

typedef U16 MibTblType;           /* table type */

typedef S32 MibReqId;             /* request identifier */

typedef U8  UstaType;             /* unsolicited status type */
typedef S8  ChannelNo;            /* Channel Number */

/* ATM typedefs */

#ifndef CMFILE_REORG_1

typedef U16 AtmVpci;              /* ATM virtual path connection id */
typedef U16 AtmVpi;               /* ATM virtual path id */


typedef U16 AtmVci;               /* ATM virtual channel id */

typedef U8 AtmLp;                 /* ATM loss priority */

typedef U8 AtmCi;                 /* ATM congestion indication */

typedef U8 AtmRs;                 /* ATM reception status */

typedef U8 AtmUu;                 /* ATM user-to-user field in CPCS PDUs */

typedef U32 AtmUui;               /* ATM user-to-user indication field in SSCOP PDUs */

typedef U8 AtmPt;                 /* ATM cell payload type */

typedef struct atmQos             /* ATM quality of service */
{
   U8 qosFwd;                     /* qos in forward  direction */
   U8 qosBwd;                     /* qos in backward direction */
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
   U16        count;                      /* number of VCCs */
   AtmVccId   tbl[MAX_ATMVCCTBL_SZ];      /* VCC list */
} AtmVccTbl;

/* Generic ATM address */
typedef struct atmAddr
{
   U8    type;                  /* type of Address (AESA or E.164) */
   U8    len;                   /* length (bytes) */
   U8    strg[ADRLEN];          /* address string */
} AtmAddr;

/* ATM address table */
typedef struct atmAddrTbl            /* ATM address table */
{
   U16      count;                   /* number of ATM addresses */
   AtmAddr  tbl[MAX_ATMADDRTBL_SZ];  /* ATM address list */
} AtmAddrTbl;

typedef struct atmTfcDesc         /* ATM traffic descriptor */
{
   U32  fwdPeakCellRate0;         /* forward  peak  cell rate, CLP = 0   */
   U32  bwdPeakCellRate0;         /* backward peak  cell rate, CLP = 0   */
   U32  fwdPeakCellRate1;         /* forward  peak  cell rate, CLP = 0+1 */
   U32  bwdPeakCellRate1;         /* backward peak  cell rate, CLP = 0+1 */
   U32  fwdSustCellRate0;         /* forward  sust. cell rate, CLP = 0   */
   U32  bwdSustCellRate0;         /* backward sust. cell rate, CLP = 0   */
   U32  fwdSustCellRate1;         /* forward  sust. cell rate, CLP = 0+1 */
   U32  bwdSustCellRate1;         /* backward sust. cell rate, CLP = 0+1 */
   U32  fwdMeanBurstSize0;        /* forward  mean burst size, CLP = 0   */
   U32  bwdMeanBurstSize0;        /* backward mean burst size, CLP = 0   */
   U32  fwdMeanBurstSize1;        /* forward  mean burst size, CLP = 0+1 */
   U32  bwdMeanBurstSize1;        /* backward mean burst size, CLP = 0+1 */
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

typedef U32 IpAddr;               /* IP address */

typedef U16 Port;                 /* TCP/UDP port */

typedef U8 Cmd;                   /* command */

typedef U8 Flags;                 /* TCP/UDP flags */

typedef U8 Ttl;                   /* time to live */

typedef U8 Prec;                  /* TCP/UDP precedence */

typedef U32 Window;               /* TCP/UDP window */

typedef U8 MtpStatus;             /* MTP status */

typedef U8 Credit;                /* credit */

/* ISUP typedefs */

typedef U32 CirId;                /* circuit Id */

typedef U16 Cic;                  /* cic */

typedef U32 SiInstId;             /* instance id */

/* B-ISUP typedefs */

typedef U32 BiInstId;             /* instance id */

/* TUP typedefs */

typedef U32 TpInstId;             /* instance id */

/* LLC/SNAP definitions */

typedef U32 Oui;                 /* 3-octet OUI in SNAP header */
typedef U16 Pid;                 /* 2-octet protocol id in SNAP header */
typedef U32 LlcId;               /* LLC id */


#ifndef CMFILE_REORG_1

/* q.93b typedefs */
 
typedef S32 AmInstId;             /* service user/provider instance id */
typedef U16 AmEndptRefType;       /* endpoint reference */
typedef U32 AmSeqNmb;             /* sequence number */
 
/* q.saal typedefs */
 
typedef U16 AsErrorCode;          /* q.saal error code (Q.SAAL1 Appendix 1) */

/* ume typedefs */

typedef U32 UmInteger;            /* ume integer */
 
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

typedef U16 LecId;               /* LEC Id */
typedef U8  Protocol;            /* protocol */
typedef U8  Version;             /* version */
typedef U16 OpCode;              /* op code in control frames */
typedef U16 LaneStatus;          /* status in control frames */
typedef U32 TransId;             /* transaction id */
typedef U16 LaneFlags;           /* flags in control frames */
typedef U8  LanType;             /* LAN type */
typedef U8  MtuIdx;              /* max frame size - index */
typedef U16 MtuVal;              /* max frame size - value */
typedef U16 Tag;                 /* tag indicating LAN destination type */
typedef U8  VccNature;           /* VCC nature - SVC, PVC, etc. */
typedef U8  VccType;             /* VCC type - control, data, etc */
typedef U8  ProfileId;           /* HCC profile id */


typedef struct lanName          /* LAN name */
{
   U8 length;                   /* length of string */
   U8 strg[MAX_LANNAME];        /* name string */
} LanName;

/* LAN destination typedefs */

typedef struct macAddr          /* MAC address */
{
   U8 strg[MACADDRLEN];         /* address string */
} MacAddr;

typedef struct macAddrTblEntry  /* entry in table of MAC addresses */
{
   U8 proxyClass;               /* proxy class - local, learned */
   MacAddr macAddr;             /* MAC address */
} MacAddrTblEntry;

typedef struct macAddrTbl       /* table of MAC addresses */
{
   U8 count;                    /* number of entries */
   MacAddrTblEntry tbl[MAX_MACADDRTBL]; /* table of MAC addresses */
} MacAddrTbl;

typedef struct rd               /* route designator */
{
   U16 lanId;                   /* LAN id, segment id - 12 bits */
   U8  bridgeId;                /* bridge id - 4 bits */
} Rd;

typedef struct rdTblEntry       /* entry in table of route designators */
{
   Rd rd;                       /* route designator */
} RdTblEntry;

typedef struct rdTbl            /* table of route designators */
{
   U8 count;                    /* number of entries */
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
   U8         nmbTLV;           /* number of TLV entries in list */
   LanName    lanName;          /* LAN name */
} LaneCtrlHdr;

typedef struct laneTLVEnt       /* type-length-value entry */
{
   U32 type;                    /* type of value */
   U8  length;                  /* length of value */
   U8  value[MAX_TLV_LEN];      /* value */
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
typedef U32 PnPortId;

/* pnni node Id */
typedef struct pnNodeId
{
   U8 id[PN_NODEID_LEN];
} PnNodeId;

#endif /* CMFILE_REORG_1 */



/* header typedefs */

typedef struct octStrg            /* octet string */
{
   S32 length;                    /* length */
   U8 val[MF_SIZE_TKNSTR];        /* value */
} OctStrg;

typedef struct tknHdr             /* token header */
{
   U8   pres;                     /* present */
   U8   spare1;                   /* for 16 bit alignment */
   U16  spare2;                   /* for 32 bit alignment */
#ifdef ALIGN_64BIT
   U32  spare3;                   /* for 64 bit alignment */
#endif
} TknHdr;

typedef struct elmtHdr            /* element header */
{
   U8   pres;                     /* present */
   U8   actnInd;                  /* action indicator */
   U16  compInd;                  /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare;                    /* for 64 bit alignment */
#endif
} ElmtHdr;

/* token typedefs */

typedef struct tknU8              /* token U8 */
{
   U8   pres;                     /* present flag */
   U8   val;                      /* value */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
} TknU8;

typedef struct tknS8              /* token S8 */
{
   U8   pres;                     /* present flag */
   S8   val;                      /* value */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
} TknS8;

typedef struct tknU16             /* token U16 */
{
   U8   pres;                     /* present flag */
   U8   spare1;                   /* for alignment */
   U16  val;                      /* value */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
} TknU16;

typedef struct tknU32             /* token U32 */
{
   U8   pres;                     /* present flag */
   U8   spare1;                   /* for alignment */
   U16  spare2;                   /* for alignment */
   U32  val;                      /* value */
} TknU32;

typedef struct tknS32             /* token S32 */
{
   U8   pres;                     /* present flag */
   U8   spare1;                   /* for alignment */
   U16  spare2;                   /* for alignment */
   S32  val;                      /* value */
} TknS32;

typedef struct tknStrS            /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[(MF_SIZE_TKNSTRS + 7) & 0xff8]; /* string value */
#else
   U8   val[(MF_SIZE_TKNSTRS + 3) & 0xffc]; /* string value */
#endif
} TknStrS;

typedef struct tknStrM            /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[(MF_SIZE_TKNSTRM + 7) & 0xff8]; /* string value */
#else
   U8   val[(MF_SIZE_TKNSTRM + 3) & 0xffc]; /* string value */
#endif
} TknStrM;

typedef struct tknStr             /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[(MF_SIZE_TKNSTR + 7) & 0xff8]; /* string value */
#else
   U8   val[(MF_SIZE_TKNSTR + 3) & 0xffc]; /* string value */
#endif
} TknStr;

typedef struct tknStrE            /* token string extended */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[(MF_SIZE_TKNSTRE + 7) & 0xff8]; /* string value */
#else
   U8   val[(MF_SIZE_TKNSTRE + 3) & 0xffc]; /* string value */
#endif
} TknStrE;

typedef struct tknStrXL           /* token string extra long */
{
   U16  len;                      /* length */
   U8   pres;                     /* present flag */
   U8   spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
   U8   *val;                     /* string value (use allocated memory) */
} TknStrXL;
 
typedef struct tknStr4            /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[8];                   /* string value - 8 byte alligned */
#else
   U8   val[4];                   /* string value - 4 byte alligned */
#endif /* ALIGN_64BIT */
} TknStr4;

typedef struct tknStr12           /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[16];                  /* string value - 8 byte alligned */
#else
   U8   val[12];                  /* string value - 4 byte alligned */
#endif /* ALIGN_64BIT */
} TknStr12;

typedef struct tknStr32           /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
   U8   val[32];                  /* string value - 4 byte alligned */
} TknStr32;

typedef struct tknStr64           /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
   U8   val[64];                  /* string value - 4 byte alligned */
} TknStr64;

typedef struct tknStr132          /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[136];                 /* string value - 8 byte alligned */
#else
   U8   val[132];                 /* string value - 4 byte alligned */
#endif /* ALIGN_64BIT */
} TknStr132;

typedef struct tknStr256          /* token string */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
   U8   val[256];                 /* string value - 4 byte alligned */
} TknStr256;

typedef struct tknOid             /* Object Identifier */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* length */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
#endif
   /* gen_x_001.main_81 : changed val from U32 to U16 with comilation flag
    * TKNOID_U16 */
#ifndef TKNOID_U16
   U32  val[32];                  /* max 32 integers of less than 64k value */
#else
   U16  val[32];                  /* max 32 integers of less than 64k value */
#endif
} TknOid;

typedef struct tknBits            /* token bits */
{
   U8   pres;                     /* present flag */
   U8   len;                      /* for alignment */
   U16  spare1;                   /* for alignment */
#ifdef ALIGN_64BIT
   U32  spare2;                   /* for 64 bit alignment */
   U8   val[(MF_SIZE_TKNBITS + 7) & 0xff8]; /* string value */
#else
   U8   val[(MF_SIZE_TKNBITS + 3) & 0xffc]; /* string value */
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
   TknU8 nmbPlanId;               /* numbering plan identification */
   TknU8 typeNmb0;                /* type of number */
#ifdef CDPTYNMB_32DIGIT
   TknStrM nmbDigits;             /* number digits */
#else
   TknStrS nmbDigits;             /* number digits */
#endif /* CDPTYNMB_32DIGIT */
} CdPtyNmb;

typedef struct redirNmb           /* redirecting number tokens */
{
   ElmtHdr eh;                    /* element header */
   TknU8 nmbPlanId;               /* numbering plan identification */
   TknU8 typeNmb;                 /* type of number */
   TknU8 screenInd;               /* screening indicator */
   TknU8 presInd;                 /* presentation indicator */
   TknU8 rsnRedirect;             /* reason for redirection */
   TknStrS nmbDigits;             /* number digits */
} RedirNmb;

typedef struct srvClass           /* service class */          
{                                                       
   U8 type;                       /* type */                 
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
   U8     ip_hl;            /* header length */
   Prec   ip_tos;           /* type of service */
   U16    ip_len;           /* total length */
   U16    ip_id;            /* identification */
   U16    ip_off;           /* fragment offset field */
   Ttl    ip_ttl;           /* time to live */
   U8     ip_p;             /* protocol */
   U16    ip_sum;           /* checksum */
   IpAddr ip_src;           /* source address */
   IpAddr ip_dst;           /* dest address */
} Ip;
 

/* ATM structures */
 
#ifndef CMFILE_REORG_1

/* info elements that can be used for routing calls */
 
typedef struct amCdPtySad       /* Called Party Sub Address Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   oddEvenInd;          /* odd/even indicator */
   TknU8   typeSad;             /* type of sub address */
   TknStrS  sadInfo;            /* address/number information */
} AmCdPtySad;
 
typedef struct amBHiLyrInfo     /* Broadband High Layer Information Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   hiLyrInfoType;       /* high layer information type */
   TknStrS  hiLyrInfo;          /* high layer information */
} AmBHiLyrInfo;
 
typedef struct amBBearCap       /* Broadband Bearer Capability Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   bearClass;           /* bearer class */
   TknU8   timingReq;           /* timing requirement */
   TknU8   tfcType;             /* traffic type */
   TknU8   atmTfrCap;           /* ATM transfer capability */
   TknU8   usrPlaneConCfg;      /* user plane connection configuration */
   TknU8   suscClip;            /* susceptability to clipping */
} AmBBearCap;

typedef struct amQosParam       /* Quality of Service Parameter Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   qosClassFwd;         /* quality of service class forward */
   TknU8   qosClassBwd;         /* quality of service class backward */
} AmQosParam;

typedef struct amEtoeDly        /* End To End Transit Delay Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   cumTransDlyId;       /* cumulative transit delay id */
   TknU16  cumTransDly;         /* cumulative transit delay value */
   TknU8   reqTransDlyId;       /* requested max end to end transit delay id */
   TknU16  reqTransDly;         /* maximum end to end transit delay value */
   TknU8   pnniAccFMCTDId;      /* PNNI acceptable forward max. CTD ID */
   TknU32  pnniAccFMCTD;        /* PNNI acceptable forward max. CTD */
   TknU8   pnniCumFMCTDId;      /* PNNI acceptable forward max. CTD ID */
   TknU32  pnniCumFMCTD;        /* PNNI acceptable forward max. CTD */
   TknU8   netGenInd;           /* network generated indicator */
} AmEtoeDly;

typedef struct amOamTfcDesc     /* OAM Traffic Descriptor Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   usrNetFaultMgmt;     /* user network fault management indicator */
   TknU8   compInd;             /* compliance indicator */
   TknU8   shapingInd;          /* shaping indicator */
   TknU8   bwdEtoeOamF5FlInd;   /* backward e-to-e OAM F5 flow indicator */
   TknU8   fwdEtoeOamF5FlInd;   /* forward e-to-e OAM F5 flow indicator */
} AmOamTfcDesc;

typedef struct amEndptRef       /* Endpoint Reference Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   endptRefType;        /* endpoint reference type */
   TknU16  endptRefVal;         /* endpoint reference value */
} AmEndptRef;

typedef struct amAalParam       /* AAL Parameters Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   aalType;             /* AAL type */

   /* Token definition for AAL-1 */
   TknU8   subTypeId;           /* Subtype Identifier */
   TknU8   subType;             /* Subtype */
   TknU8   cbrRateId;           /* CBR Rate Identifier */
   TknU8   cbrRate;             /* CBR Rate */
   TknU8   multId;              /* Multiplier Identifier */
   TknU16  multVal;             /* Multiplier value */
   TknU8   srcClkFreqMetId;     /* Source clock Frequency method identifier */
   TknU8   srcClkFreqMet;       /* Source Clock frequency method */
   TknU8   errCrMetId;          /* Error correction method identifier */
   TknU8   errCrMet;            /* Error correction method */
   TknU8   strDatTxBlkszId;     /* Structured data transfer blocksize Id. */
   TknU8   strDatTxBlksz0;      /* Structured data transfer blocksize - oct 1*/

   /* Token definition for AAL-1, except in UNI 3.0 */
   TknU8   strDatTxBlksz1;      /* Structured data transfer blocksize - oct 2*/

   /* Token definition for AAL-1 */
   TknU8   prtFillCellId;       /* Partially filled cells Identifier */
   TknU8   prtFillCellMet;      /* Partially filled cells method */

   /* Token definition for AAL-3/4 and AAL-5 */
   TknU8   fwdMaxCpcsSduSzId;   /* Forward maximum CPCS-SDU size identifier */
   TknU16  fwdMaxCpcsSduSz;     /* Forward maximum CPCS-SDU size */
   TknU8   bwdMaxCpcsSduSzId;   /* Forward maximum CPCS-SDU size identifier */
   TknU16  bwdMaxCpcsSduSz;     /* Forward maximum CPCS-SDU size */

   /* Token definition for AAL-3/4 only */
   TknU8   midRangeId;          /* MID Range identifier */
   TknU16  loMidRange;          /* MID Range value */

   /* Token definition for AAL-3/4 only, except in UNI 3.0 */
   TknU16  hiMidRange;          /* MID Range value */

   /* Token definition for AAL-3/4 and AAL-5 and only for UNI 3.0 */
   TknU8   modeId;              /* Mode identifier */
   TknU8   mode;                /* Mode - Streaming/Message */

   /* Token definition for AAL-3/4 and AAL-5 */
   TknU8   sscsTypeId;          /* SSCS Type Identifier */
   TknU8   sscsType;            /* SSCS Type */

   /* Token definition for User defined AAL */
   TknU32  usrDefAalInfo;       /* User defined AAL information */
} AmAalParam;

typedef struct amBLoLyrInfo     /* Broadband Low Layer Information Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   usrInfoLyr1Prot;     /* user information layer 1 protocol */
   TknU8   lyr1Id;              /* Layer 1 id */
   TknU8   usrInfoLyr2Prot;     /* user information layer 2 protocol */
   TknU8   lyr2Id;              /* Layer 2 id */
   TknU8   q933Use;             /* Q.933 use */
   TknU8   lyr2OprMode;         /* Mode of operation */
   TknU8   winSize;             /* Window size */
   TknU8   usrSpecLyr2ProtInfo; /* User specified layer 2 protocol info */
   TknU8   usrInfoLyr3Prot;     /* user information layer 3 protocol */
   TknU8   lyr3Id;              /* Layer 3 id */
   TknU8   lyr3OprMode;         /* Mode of operation */
   TknU8   defPktSize;          /* Default packet size */
   TknU8   pktWinSize;          /* Default packet size */
   TknU8   usrSpecLyr3ProtInfo; /* User specified layer 3 protocol info */
   TknU8   initProtId;          /* Initial protocol Identifier bits 8-2 */
   TknU8   snapId;              /* SNAP identifier */
   TknU32  oui;                 /* Organisation unique identifier */
   TknU16  protId;              /* Protocol identifier */
} AmBLoLyrInfo;

typedef struct amAtmTfcDesc     /* ATM Traffic Descriptor Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   fwdPeakCellRateId0;  /* forward  peak  cell rate id, CLP = 0 */
   TknU32  fwdPeakCellRate0;    /* forward  peak  cell rate,    CLP = 0   */
   TknU8   bwdPeakCellRateId0;  /* backward peak  cell rate id, CLP = 0 */
   TknU32  bwdPeakCellRate0;    /* backward peak  cell rate,    CLP = 0   */
   TknU8   fwdPeakCellRateId1;  /* forward  peak  cell rate id, CLP = 0+1 */
   TknU32  fwdPeakCellRate1;    /* forward  peak  cell rate,    CLP = 0+1 */
   TknU8   bwdPeakCellRateId1;  /* backward peak  cell rate id, CLP = 0+1 */
   TknU32  bwdPeakCellRate1;    /* backward peak  cell rate,    CLP = 0+1 */
   TknU8   fwdSustCellRateId0;  /* forward  sust. cell rate id, CLP = 0 */
   TknU32  fwdSustCellRate0;    /* forward  sust. cell rate,    CLP = 0   */
   TknU8   bwdSustCellRateId0;  /* backward sust. cell rate id, CLP = 0 */
   TknU32  bwdSustCellRate0;    /* backward sust. cell rate,    CLP = 0   */
   TknU8   fwdSustCellRateId1;  /* forward  sust. cell rate id, CLP = 0+1 */
   TknU32  fwdSustCellRate1;    /* forward  sust. cell rate,    CLP = 0+1 */
   TknU8   bwdSustCellRateId1;  /* backward sust. cell rate id, CLP = 0+1 */
   TknU32  bwdSustCellRate1;    /* backward sust. cell rate,    CLP = 0+1 */
   TknU8   fwdMeanBurstSizeId0; /* forward  mean burst size id, CLP = 0 */
   TknU32  fwdMeanBurstSize0;   /* forward  mean burst size,    CLP = 0   */
   TknU8   bwdMeanBurstSizeId0; /* backward mean burst size id, CLP = 0 */
   TknU32  bwdMeanBurstSize0;   /* backward mean burst size,    CLP = 0   */
   TknU8   fwdMeanBurstSizeId1; /* forward  mean burst size id, CLP = 0+1 */
   TknU32  fwdMeanBurstSize1;   /* forward  mean burst size,    CLP = 0+1 */
   TknU8   bwdMeanBurstSizeId1; /* backward mean burst size id, CLP = 0+1 */
   TknU32  bwdMeanBurstSize1;   /* backward mean burst size,    CLP = 0+1 */
   TknU8   bstEffortReq;        /* best effort requested */
   TknU8   tfcMgmtOptId;        /* traffic management options identifier */
   TknU8   fwdTagReq;           /* tagging requested in forward  direction */
   TknU8   bwdTagReq;           /* tagging requested in backward direction */
   TknU8   bwdFrmDisc;          /* frame discard in backward direction */
   TknU8   fwdFrmDisc;          /* frame discard in forward  direction */
   TknU8   fwdAbrMinCellRateId; /* forward  ABR min. cell rate id, CLP = 0+1 */
   TknU32  fwdAbrMinCellRate;   /* forward  ABR min. cell rate,    CLP = 0+1 */
   TknU8   bwdAbrMinCellRateId; /* backward ABR min. cell rate id, CLP = 0+1 */
   TknU32  bwdAbrMinCellRate;   /* backward ABR min. cell rate,    CLP = 0+1 */
} AmAtmTfcDesc;

typedef struct amCauseDgn       /* Cause and Diagnostics Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   location;            /* location */
   TknU8   causeVal;            /* cause value */
   TknStrM  dgnVal;              /* Diagnostics value */
} AmCauseDgn;

typedef struct amCgPtyNmb       /* Calling Party Number Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   nmbPlanId;           /* address/numbering plan identification */
   TknU8   typeNmb;             /* type of number */
   TknU8   screenInd;           /* screening indicator */
   TknU8   presInd;             /* presentation indicator */
   TknStrS nmbDigits;           /* address/number information */
} AmCgPtyNmb;

typedef struct amCgPtySad       /* Calling Party Sub Address Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   oddEvenInd;          /* odd/even indicator */
   TknU8   typeSad;             /* type of sub address */
   TknStrS sadInfo;             /* address/number information */
} AmCgPtySad;

typedef struct amNBearCap       /* Narrowband Bearer Capability Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8 infoTranCap;           /* information transfer capability */
   TknU8 codingStd;             /* coding standard */
   TknU8 infoTranRate0;         /* information transfer rate */
   TknU8 tranMode;              /* transfer mode */
   TknU8 establish;             /* establishment */
   TknU8 cfg;                   /* configuration */
   TknU8 chanStruct;            /* structure */
   TknU8 infoTranRate1;         /* information transfer rate */
   TknU8 symmetry;              /* symmetry */
   TknU8 usrInfoLyr1Prot;       /* usr information layer 1 protocol */
   TknU8 lyr1Ident;             /* layer 1 identity */
   TknU8 usrRate;               /* user rate */
   TknU8 negot;                 /* negotiation */
   TknU8 syncAsync;             /* synchronous/asynchronous */
/*------------------------------------------------------------------------*/
/* The following six tokens represent a uinon of octets 5b.1 and 5b.2 of  */
/* the Narrow Band Bearer Capability.                                     */
/*------------------------------------------------------------------------*/
   TknU8 FlcRx_BandNeg;         /* flow control on reception or 
                                   inband/outband negotiation */
   TknU8 FlcTx_Assgn;           /* flow control on transmission or 
                                   assignor/assignee*/
   TknU8 NicRx_LLINeg;          /* network independent clock on reception or 
                                   logical link identifier negotiation */
   TknU8 NicTx_Mode;            /* network independent clock on transmission or
                                   mode of operation */
   TknU8 Rate_MFrm;             /* intermediate rate (low bit) or Multi 
                                   frame support */
   TknU8 Rate_Hdr;              /* intermediate rate (high bit) or rate 
                                   adaptation Hdr/ no Headr */
   TknU8 parity;                /* parity information */
   TknU8 nmbDatBits;            /* number of data bits excluding parity bit */
   TknU8 nmbStopBits;           /* number of stop bits */
   TknU8 modemType;             /* modem type */
   TknU8 duplexMode;            /* duplex mode */
   TknU8 usrInfoLyr2Prot;       /* user information layer 2 protocol */
   TknU8 lyr2Ident;             /* layer 2 identity */
   TknU8 usrInfoLyr3Prot;       /* user information layer 3 protocol */
   TknU8 lyr3Ident0;            /* layer 3 identity */
} AmNBearCap;

typedef struct amNLoLyrComp     /* Narrowband Low Layer Compatibility Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8 infoTranCap;           /* information transfer capability */
   TknU8 codingStd;             /* coding standard */
   TknU8 negInd;                /* negotiation indicator */
   TknU8 infoTranRate0;         /* information transfer rate */
   TknU8 tranMode;              /* transfer mode */
   TknU8 establish;             /* establishment */
   TknU8 cfg;                   /* configuration */
   TknU8 chanStruct;            /* structure */
   TknU8 infoTranRate1;         /* information transfer rate */
   TknU8 symmetry;              /* symmetry */
   TknU8 usrInfoLyr1Prot;       /* usr information layer 1 protocol */
   TknU8 lyr1Ident;             /* layer 1 identity */
   TknU8 usrRate;               /* user rate */
   TknU8 negot;                 /* negotiation */
   TknU8 syncAsync;             /* synchronous/asynchronous */
/*------------------------------------------------------------------------*/
/* The following six tokens represent a uinon of octets 5b.1 and 5b.2 of  */
/* the Narrow Band Lower Layer Compatibility                              */
/*------------------------------------------------------------------------*/
   TknU8 FlcRx_BandNeg;         /* flow control on reception or 
                                   inband/outband negotiation */
   TknU8 FlcTx_Assgn;           /* flow control on transmission or 
                                   assignor/assignee*/
   TknU8 NicRx_LLINeg;          /* network independent clock on reception or 
                                   logical link identifier negotiation */
   TknU8 NicTx_Mode;            /* network independent clock on transmission or
                                   mode of operation */
   TknU8 Rate_MFrm;             /* intermediate rate (low bit) or Multi 
                                   frame support */
   TknU8 Rate_Hdr;              /* intermediate rate (high bit) or rate 
                                   adaptation Hdr/ no Headr */
   TknU8 parity;                /* parity information */
   TknU8 nmbDatBits;            /* number of data bits excluding parity bit */
   TknU8 nmbStopBits;           /* number of stop bits */
   TknU8 modemType;             /* modem type */
   TknU8 duplexMode;            /* duplex mode */
   TknU8 usrInfoLyr2Prot;       /* user information layer 2 protocol */
   TknU8 lyr2Ident;             /* layer 2 identity */
   TknU8 optLyr2ProtInfo;       /* optional layer 2 protocol information */
   TknU8 usrInfoLyr3Prot;       /* user information layer 3 protocol */
   TknU8 lyr3Ident1;            /* layer 3 identity */
   TknU8 optLyr3ProtInfo;       /* optional layer 3 protocol information */
} AmNLoLyrComp;

typedef struct amNHiLyrComp     /* Narrowband High Layer Compatibility Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8 presMethod;            /* presentation method */
   TknU8 interpretation;        /* interpretation */
   TknU8 codingStd;             /* coding standard */
   TknU8 highLyrCharId;         /* high layer characteristics identification */
   TknU8 extHighLyrCharId;      /* extended high layer characteristics id */
} AmNHiLyrComp;

typedef struct amProgInd        /* Progress Indicator Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8 progLocation;          /* location */
   TknU8 codingStd;             /* coding standard */
   TknU8 progDesc;              /* progress description */
} AmProgInd;

#if (DEF_SIG_PNNI | DEF_SIG_AINI)
typedef struct amCrankback      /* Crankback Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   cbLevel;             /* crankback level */
   TknU8   blkTransType;        /* blocked transit type */
#ifdef SIG_PNNI
   TknStrS blkNodeId;           /* blocked node id */
   TknStrS blkLinkPreNodeId;    /* blocked link's preceeding node id */
   TknU32  blkLinkPortId;       /* blocked link port id */
   TknStrS blkLinkSuccNodeId;   /* blocked link's succeding node id */
#endif /* SIG_PNNI */
   TknU8   cbCause;             /* crankback cause */
#ifdef SIG_PNNI
   TknStrM cbDgn;               /* crankback cause diagnostics */
#endif /* SIG_PNNI */
} AmCrankback;

typedef struct amCdPtySoftPvc   /* Called Party Soft PVPC/PVCC Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   selType;             /* VPI/VCI selection Type */
   TknU8   vpiId;               /* VPI id */
   TknU16  vpi;                 /* VPI */
   TknU8   vciId;               /* VCI id */
   TknU16  vci;                 /* VCI */
#ifdef SPVC_FR
   TknU8   dlciId;              /* DLCI Identifer */
   TknU32  dlci;                /* Dlci Value */
#endif /* SPVC_FR */
} AmCdPtySoftPvc;

typedef struct amCgPtySoftPvc   /* Calling Party Soft PVPC/PVCC Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   vpiId;               /* VPI id */
   TknU16  vpi;                 /* VPI */
   TknU8   vciId;               /* VCI id */
   TknU16  vci;                 /* VCI */
#ifdef SPVC_FR
   TknU8   dlciId;              /* DLCI Identifer */
   TknU32  dlci;                /* Dlci Value */
#endif /* SPVC_FR */
} AmCgPtySoftPvc;
#endif /* DEF_SIG_PNNI | DEF_SIG_AINI */

typedef struct amConnNmb        /* Connected Number Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   nmbPlanId;           /* address/numbering plan identification */
   TknU8   typeNmb;             /* type of number */
   TknU8   screenInd;           /* screening indicator */
   TknU8   presInd;             /* presentation indicator */
   TknStrS nmbDigits;           /* address/number information */
} AmConnNmb;

#if (DEF_SIG_PNNI | DEF_SIG_AINI | DEF_UNI40)
typedef struct amConnSad        /* Calling Party Sub Address Tokens */
{
   ElmtHdr eh;                  /* element header */
   TknU8   oddEvenInd;          /* odd/even indicator */
   TknU8   typeSad;             /* type of sub address */
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
   TknU8   fwdPeakCellRateId0;     /* forward  peak  cell rate id, CLP = 0 */
   TknU32  fwdPeakCellRate0;       /* forward  peak  cell rate,    CLP = 0   */
   TknU8   bwdPeakCellRateId0;     /* backward peak  cell rate id, CLP = 0 */
   TknU32  bwdPeakCellRate0;       /* backward peak  cell rate,    CLP = 0   */
   TknU8   fwdPeakCellRateId1;     /* forward  peak  cell rate id, CLP = 0+1 */
   TknU32  fwdPeakCellRate1;       /* forward  peak  cell rate,    CLP = 0+1 */
   TknU8   bwdPeakCellRateId1;     /* backward peak  cell rate id, CLP = 0+1 */
   TknU32  bwdPeakCellRate1;       /* backward peak  cell rate,    CLP = 0+1 */
} AmMinAccAtmTfcDesc;

typedef struct amExtQosParam       /* Extended QOS parameter */
{
   ElmtHdr eh;                     /* element header */
   TknU8   origin;                 /* origin of this IE */
   TknU8   accFwdPpCDVId;          /* acceptable forward  peak-to-peak cell 
                                    * delay variation identifier */
   TknU32  accFwdPpCDV;            /* acceptable forward  peak-to-peak cell
                                    * delay variation */
   TknU8   accBwdPpCDVId;          /* acceptable backward peak-to-peak cell
                                    * delay variation identifier */
   TknU32  accBwdPpCDV;            /* acceptable backward peak-to-peak cell
                                    * delay variation */
   TknU8   cumFwdPpCDVId;          /* cumulative forward  peak-to-peak cell
                                    * delay variation identifier */
   TknU32  cumFwdPpCDV;            /* cumulative forward  peak-to-peak cell
                                    * delay variation */
   TknU8   cumBwdPpCDVId;          /* cumulative backward peak-to-peak cell
                                    * delay variation identifier */
   TknU32  cumBwdPpCDV;            /* cumulative backward peak-to-peak cell
                                    * delay variation */
   TknU8   accFwdCLRId;            /* acceptable forward  cell loss ratio
                                    * identifier */
   TknU8   accFwdCLR;              /* acceptable forward  cell loss ratio */
   TknU8   accBwdCLRId;            /* acceptable backward cell loss ratio 
                                    * identifier */
   TknU8   accBwdCLR;              /* acceptable backward cell loss ratio */
} AmExtQosParam;

typedef struct amAbrAddParam       /* ABR additional parameters */
{
   ElmtHdr eh;                     /* element header */
   TknU8   fwdAddParamRecId;       /* forward  additional parameters record identifier */
   TknU32  fwdAddParamRec;         /* forward  additional parameters record */
   TknU8   bwdAddParamRecId;       /* backward additional parameters record identifier */
   TknU32  bwdAddParamRec;         /* backward additional parameters record */
} AmAbrAddParam;

typedef struct amAbrSetupParam     /* ABR Setup parameters */
{
   ElmtHdr eh;                     /* element header */
   TknU8   fwdAbrICRId;            /* forward  ABR initial cell rate identifier */
   TknU32  fwdAbrICR;              /* forward  ABR initial cell rate */
   TknU8   bwdAbrICRId;            /* backward ABR initial cell rate identifier */
   TknU32  bwdAbrICR;              /* backward ABR initial cell rate */
   TknU8   fwdAbrTBEId;            /* forward  ABR transient buffer exposure identifier */
   TknU32  fwdAbrTBE;              /* forward  ABR transient buffer exposure */
   TknU8   bwdAbrTBEId;            /* backward ABR transient buffer exposure identifier */
   TknU32  bwdAbrTBE;              /* backward ABR transient buffer exposure */
   TknU8   cumRmFRTTId;            /* cumulative RM fixed round trip time identifier */
   TknU32  cumRmFRTT;              /* cumulative RM fixed round trip time */
   TknU8   fwdRIFId;               /* forward  rate increment factor identifier */
   TknU8   fwdRIF;                 /* forward  rate increment factor */
   TknU8   bwdRIFId;               /* backward rate increment factor identifier */
   TknU8   bwdRIF;                 /* backward rate increment factor */
   TknU8   fwdRDFId;               /* forward  rate decrement factor identifier */
   TknU8   fwdRDF;                 /* forward  rate decrement factor */
   TknU8   bwdRDFId;               /* backward rate decrement factor identifier */
   TknU8   bwdRDF;                 /* backward rate decrement factor */
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
   TknU8   nmbPlanId;           /* address/numbering plan identification */
   TknU8   typeNmb;             /* type of number */
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
   U16 spare;                     /* spare for alignment */
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
   U16 msgLen;                  /* message length   - optional */
   U8 msgType;                  /* message type     - mandatory */
   U8 version;                  /* version          - optional */
   U16 seqNmb;                  /* sequence number  - optional */
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
   U16 val;                     /* value */
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
   U8 month;                      /* month */
   U8 day;                        /* day */
   U8 year;                       /* year */
   U8 hour;                       /* hour - 24 hour clock */
   U8 min;                        /* minute */
   U8 sec;                        /* second */
   U8 tenths;                     /* tenths of second */
   /*-- gen_x_001.main_90 - Added variable for microseconds in DateTime--*/
#ifdef SS_DATETIME_USEC
   U32 usec;                      /* micro seconds */
#endif /*-- SS_DATETIME_USEC --*/
} DateTime;
/* gen_x_001.main_94: Additions */
/* gen_x_001.main_95: Modifications */
typedef U64 EpcTime;

/* common status */
typedef struct cmStatus
{
   U16 status;       /* status of request */
   U16 reason;       /* failure reason */
}CmStatus;    

/* common alarm */
typedef struct cmAlarm
{
   DateTime dt;      /* data and time */
   U16 category;     /* alarm category*/
   U16 event;        /* alarm event */
   U16 cause;        /* alarm cause */
}CmAlarm;    
  
/* duration structure */
  
typedef struct duration           /* duration */
{
   U8 days;                       /* days */
   U8 hours;                      /* hours */
   U8 mins;                       /* minutes */
   U8 secs;                       /* seconds */
   U8 tenths;                     /* tenths of seconds */
} Duration;





#ifdef CMFILE_REORG_1

#ifdef SS
typedef struct ssmsgb Buffer;

#ifdef FLAT_BUFFER_OPT
typedef struct _flatBuffer
{
   U8* startAddr;
   U8* ptr;
   U32 len;
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
   U8      pres;                  /* Present Flag */
   U8      spare1;                /* for alignment */
   U16     spare2;                /* for 32 bit alignment */
#ifdef ALIGN_64BIT
   U32     spare3;                /* for 64 bit alignment */
#endif
   Buffer  *val;                  /* Buffer type (use allocated memory) */
} TknBuf;

/* defining the CmIpAddr and CmIpAddr6 */
/* socket typedefs and structs */
typedef U32 CmIpAddr;        /* 4 byte IP address */
#ifdef IPV6_SUPPORTED
typedef U8  CmIpAddr6[16];   /* 16 byte IPV6 address */
#endif /* IPV6_SUPPORTED */

/* common packing functions */

/* system services structures */
EXTERN S16 cmPkDateTime ARGS((DateTime *dateTime, Buffer *mBuf));
EXTERN S16 cmPkDuration ARGS((Duration *duration, Buffer *mBuf));
EXTERN S16 oduPackPointer ARGS((PTR ptr, Buffer *mBuf));
EXTERN S16 cmPkEntityId ARGS((EntityId *entityId, Buffer *mBuf));
EXTERN S16 cmPkElmntId  ARGS((ElmntId  *elmntId,  Buffer *mBuf));
EXTERN S16 cmPkMemoryId ARGS((MemoryId *memoryId, Buffer *mBuf));

/* general structures */
EXTERN S16 cmPkSystemId ARGS((SystemId *systemId, Buffer *mBuf));
EXTERN S16 cmPkAddrs    ARGS((Addrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmPkProtAddr ARGS((ProtAddr *protAddr, Buffer *mBuf));
EXTERN S16 cmPkProtAddrTbl ARGS((ProtAddrTbl *protAddr, Buffer *mBuf));
EXTERN S16 cmPkShrtAddrs ARGS((ShrtAddrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmPkAddrMask ARGS((U8    *mask,    Buffer *mBuf));
EXTERN S16 cmPkBndCfg   ARGS((BndCfg   *bndCfg,   Buffer *mBuf));
EXTERN S16 cmPkPst      ARGS((Pst      *pst,      Buffer *mBuf));
EXTERN S16 cmPkElmtHdr  ARGS((ElmtHdr  *m,        Buffer *mBuf));
EXTERN S16 cmPkTknU8    ARGS((TknU8    *tknU8,    Buffer *mBuf));
EXTERN S16 cmPkTknS8    ARGS((TknS8    *tknS8,    Buffer *mBuf));
EXTERN S16 cmPkTknU16   ARGS((TknU16   *tknU16,   Buffer *mBuf));
EXTERN S16 cmPkTknU32   ARGS((TknU32   *tknU32,   Buffer *mBuf));
EXTERN S16 cmPkTknStr   ARGS((TknStr   *tknStr,   Buffer *mBuf));                              
EXTERN S16 cmPkTknStrM  ARGS((TknStrM  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStrS  ARGS((TknStrS  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStrE  ARGS((TknStrE  *tknStr,   Buffer *mBuf));

EXTERN S16 cmPkTknStr4   ARGS((TknStr4   *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr12  ARGS((TknStr12  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr32  ARGS((TknStr32  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr64  ARGS((TknStr64  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr132 ARGS((TknStr132 *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr256 ARGS((TknStr256 *tknStr,   Buffer *mBuf));

PUBLIC  S16  cmPkTknS32    ARGS((TknS32 *tknS32, Buffer *mBuf));
PUBLIC  S16  cmPkTknOid    ARGS((TknOid *tknOid, Buffer *mBuf));
PUBLIC  S16  cmPkTknBuf    ARGS((TknBuf *tknBuf, Buffer *mBuf));

#ifdef TDS_ROLL_UPGRADE_SUPPORT
PUBLIC  S16  cmPkIntf      ARGS((CmIntf *intf, Buffer *mBuf));
#endif

/* layer management structures */
EXTERN S16 cmPkHeader   ARGS((Header   *header,   Buffer *mBuf));
EXTERN S16 cmPkSmCfg    ARGS((SmCfg    *smCfg,    Buffer *mBuf));
EXTERN S16 cmPkTmrCfg   ARGS((TmrCfg   *tmrCfg,   Buffer *mBuf));
EXTERN S16 cmPkCmStatus ARGS((CmStatus *status,   Buffer *mBuf));
EXTERN S16 cmPkCmAlarm  ARGS((CmAlarm  *alrm,     Buffer *mBuf));


/* common unpacking functions */

/* system services structures */
EXTERN S16 cmUnpkDateTime ARGS((DateTime *dateTime, Buffer *mBuf));
EXTERN S16 cmUnpkDuration ARGS((Duration *duration, Buffer *mBuf));
EXTERN S16 oduUnpackPointer ARGS((PTR *ptr, Buffer *mBuf));
EXTERN S16 cmUnpkEntityId ARGS((EntityId *entityId, Buffer *mBuf));
EXTERN S16 cmUnpkElmntId  ARGS((ElmntId  *elmntId,  Buffer *mBuf));
EXTERN S16 cmUnpkMemoryId ARGS((MemoryId *memoryId, Buffer *mBuf));

/* general structures */
EXTERN S16 cmUnpkSystemId ARGS((SystemId *systemId, Buffer *mBuf));
EXTERN S16 cmUnpkAddrs    ARGS((Addrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmUnpkProtAddr ARGS((ProtAddr *protAddr, Buffer *mBuf));
EXTERN S16 cmUnpkProtAddrTbl ARGS((ProtAddrTbl *protAddr, Buffer *mBuf));
EXTERN S16 cmUnpkShrtAddrs ARGS((ShrtAddrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmUnpkAddrMask ARGS((U8    *mask,    Buffer *mBuf));
EXTERN S16 cmUnpkBndCfg   ARGS((BndCfg   *bndCfg,   Buffer *mBuf));
EXTERN S16 cmUnpkPst      ARGS((Pst      *pst,      Buffer *mBuf));
EXTERN S16 cmUnpkElmtHdr  ARGS((ElmtHdr  *m,        Buffer *mBuf));
EXTERN S16 cmUnpkTknU8    ARGS((TknU8    *tknU8,    Buffer *mBuf));
EXTERN S16 cmUnpkTknS8    ARGS((TknS8    *tknS8,    Buffer *mBuf));
EXTERN S16 cmUnpkTknU16   ARGS((TknU16   *tknU16,   Buffer *mBuf));
EXTERN S16 cmUnpkTknU32   ARGS((TknU32   *tknU32,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr   ARGS((TknStr   *tknStr,   Buffer *mBuf));                              
EXTERN S16 cmUnpkTknStrM  ARGS((TknStrM  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStrS  ARGS((TknStrS  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStrE  ARGS((TknStrE  *tknStr,   Buffer *mBuf));

EXTERN S16 cmUnpkTknStr4   ARGS((TknStr4   *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr12  ARGS((TknStr12  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr32  ARGS((TknStr32  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr64  ARGS((TknStr64  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr132 ARGS((TknStr132 *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr256 ARGS((TknStr256 *tknStr,   Buffer *mBuf));

PUBLIC  S16  cmUnpkTknS32  ARGS((TknS32 *tknS32, Buffer *mBuf));
PUBLIC  S16  cmUnpkTknOid  ARGS((TknOid *tknOid, Buffer *mBuf));
PUBLIC  S16  cmUnpkTknBuf  ARGS((TknBuf *tknBuf, Buffer **mBuf));

#ifdef TDS_ROLL_UPGRADE_SUPPORT
PUBLIC  S16  cmUnpkIntf    ARGS((CmIntf *intf, Buffer *mBuf));
#endif

/* layer management structures */
EXTERN S16 cmUnpkHeader   ARGS((Header   *header,   Buffer *mBuf));
EXTERN S16 cmUnpkSmCfg    ARGS((SmCfg    *smCfg,    Buffer *mBuf));
EXTERN S16 cmUnpkTmrCfg   ARGS((TmrCfg   *tmrCfg,   Buffer *mBuf));
EXTERN S16 cmUnpkCmStatus ARGS((CmStatus *status,   Buffer *mBuf));
EXTERN S16 cmUnpkCmAlarm  ARGS((CmAlarm  *alrm,     Buffer *mBuf));

#endif /* CMFILE_REORG_1 */
#if defined(XEON_SPECIFIC_CHANGES) || defined(INTEL_WLS)
void * mtGetWlsHdl();
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __GENX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
