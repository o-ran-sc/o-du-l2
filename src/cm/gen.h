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
  
     Desc:     Defines required by two or more layer service user
               interfaces.
   
     File:     gen.h
  
*********************************************************************21*/

#ifndef __GENH__
#define __GENH__

/* defines */

#define  RTLIN_DUMP_DEBUG printf

/* Protocol Address Structure related defines ..*/
#define MAX_PROTADDR_LEN     16       /* Right now defined for IP, IPX 
                                       * and IPV6 
                                       */
#define MAX_PROT_ADDRS       4        /* Number of Protocols supported */

/* addrs structure for xxConReq, xxDiscReq, xxCfgReq */

#define ADRLEN        20    /* Address Length */
#define SHRTADRLEN    32    /* Short Addres Length */
#define LNGADRLEN     64    /* Long Address Length */

/* token or element header values */

#define NOTPRSNT       0    /* not present */
#define PRSNT_NODEF    1    /* present - no default */
#define PRSNT_DEF      2    /* present - default */

#define MF_SIZE_TKNSTRS  22 /* token string size - small */
#define MF_SIZE_TKNSTRM  32 /* token string size - medium */
#define MF_SIZE_TKNSTR  132 /* token string size - regular */
#define MF_SIZE_TKNSTRE 255 /* token string size - extra large */
#define MF_SIZE_TKNBITS 7   /* token bits size */

#define NOTUSED           0      /* not used */

/* defines for ATM */

#ifndef CMFILE_REORG_1

/* MAC address length */

#define MACADDRLEN                    6  /* 48 bit MAC address length */

/* maximum sizes for typedef arrays */

#define MAX_LANNAME                  32  /* size of LAN name string */
#define MAX_MACADDRTBL               16  /* size of MAC address table */
#define MAX_RDTBL                    16  /* size of route descriptor table */
#define MAX_TLV_LEN                   4  /* max length of value in TLV entry */
#define MAX_TLV_TBL                  16  /* size of TLV table */


#endif /* CMFILE_REORG_1 */


/* defines for SPstTsk */

#define SEL_LC_NEW     0     /* loosely coupled interface - new */
#define SEL_LC_OLD     1     /* loosely coupled interface - old */


/* defines */
  
/* Mngmt.hdr.msgType */
  
#define TCFG             1           /* configuration */
#define TCNTRL           2           /* control */
#define TSTS             3           /* statistics */
#define TSSTA            4           /* solicited status */
#if 0
#define TUSTA            5           /* unsolicited status */
#define TTRC             6           /* trace */
#define TACNT            7           /* billing */
#define TUDAT            8           /* unit data */
#define TWRMSTRT         9           /* warm start */
#define TSWP             10          /* swapping */
#define TSNAP            11          /* register snapshot */
#define TUCFG            12          /* unconfig */
#define TSNMP            13          /* snmp req */
#define TOBJ             14          /* obj req */
#define TMIB             15          /* mib req */
/* gen_h_001.main_124 RRC 2.0 Release*/
#define TAUDT            16          /* Audit req */
#endif 

/* Mngmt.hdr.elmId.elmnt */
  
#define STGEN            1           /* general */
#define STSID            8           /* system id */
#if 0
#define STTSAP           2           /* transport SAP */
#define STNSAP           3           /* network SAP */
#define STLLSAP          4           /* logical link SAP */
#define STDLSAP          5           /* data link SAP */
#define STMSAP           6           /* MAC SAP */
#define STPSAP           7           /* physical SAP */
#define STHG             9           /* hunt Group */
#define STROUT          10           /* route */
#define STDLC           11           /* data link connection */
#define STINTCUG        12           /* international cugs */
#define STBCHNPROF      13           /* B channel profile */
#define STPVC           14           /* PVC configuration */
#define STMCGRP         15           /* multicast group */
#define STFRROUT        16           /* frame relay route */
#define STDCHAN         17           /* X.31 D channel */
#define STADRMAP        18           /* address mapping */
#define STDELADRMAP     19           /* delete X.31 address mapping */

#define STLOOP          20           /* loop */
#define STREG           21           /* region */
#define STDPOOL         22           /* dynamic pool */
#define STSPOOL         23           /* static pool */
#define STDQ            24           /* demand queue */
#define STENT           25           /* entity */
#define STTSK           26           /* task */

#define STDELROUT       30           /* delete route */
#define STDELPVC        31           /* delete PVC */
#define STIFDSTADR      32           /* interface destination address */
    
#define STLNKSET        33           /* link set */
#define STISAP          34           /* isup SAP */
#define STICIR          35           /* isup circuit */
#define STSPSAP         36           /* tcap lower sccp sap */
#define STTCUSAP        37           /* tcap upper user sap */
#define STTPSAP         38           /* tup SAP */
#define STTPCIR         39           /* tup circuit */
#define STVCC           40           /* virtual channel connection */
#define STCGPTYNMB      41           /* calling party number */
#define STWPSAP         42           /* wrapper SAP */
#define STAALSAP        43           /* AAL SAP */
#define STLANDST        44           /* LAN destination */
#define STLES           45           /* LAN Emulation Server */
#define STBUS           46           /* LAN Emulation Broadcast Server */

/* used by mtp level 3 */

#define STDLSAPACT      47           /* data link SAP - initial state = ACTIVE */
#define STDLSAPDIS      STDLSAP      /* data link SAP - initial state = DISABLED */
#define STTFCMTRC       49           /* traffic metrics */
#define STPNNBR         50           /* PNNI Neighbor */
#define STPNPTSE        51           /* PNNI PTSE */
#define STLECS          52           /* LECS */
#define STLECSLES       53           /* LECS LES */
#define STLECSROUT      54           /* LECS ROUT */
#define STERR           55           /* Software Error */

#define STMAP           56           /* map */
#define STMAPNODE       57           /* node map */
#define STRCC           58           /* Routing Control Channel */
#define STRTEADDR       59           /* route to address */
#define STRTENODE       60           /* route to node */
#define STRTETNS        61           /* route to transit network */
#define STRTETBL        62           /* routing table */
#define STPNPG          63           /* PNNI PEER GROUP */
#define STLINK          64           /* ATM Physical Link */
#define STINFOGRP       65           /* information group */
#define STNISAP         66           /* FR-ATM network interworking sap */
#define STSISAP         67           /* FR-ATM service interworking sap */
#define STPROF          68           /* FR-ATM aal connection's profile */
#define STNETPFX        69           /* network prefix */
#define STUSRPART       70           /* user part */
#define STADDR          71           /* address */
#define STSRVC          72           /* service registry */
#define STCIPSAP        73           /* PLOA's CIPSAP */
/* define for Envelope Function EFadr mapping */
#define STEVEFADR       74           /* EV EFadr to SAP mapping */

/* Used by PNNI for horizontal link hello protocol and summary addresses */
#define STHLHP          75           /* Horizontal link hello protocol */
#define STSUMMADDR      76           /* Summary addresses */

/* used by LEC for static ARP entry */
#define STSTAARP        77           /* static ARP entry */

/* used by PQ for TDM Configuration */
#define STTDM           78           /* TDM Configuration */

/* Used by LES */
#define STLECSMPOATLV   79           /* LECS MPOA TLV entry */

#define STMPCCP         80           /* PLOA's MPC Control Point */
#define STMPSAP         81           /* PLOA's MPC/MPS SAP */
#define STMPSCP         82           /* PLOA's MPS Control Point */

/* used for group saps */
#define STGRTSAP        85           /* transport sap group */
#define STGRNSAP        86           /* netwrok sap group */
#define STGRDLSAP       87           /* data link  sap group */
#define STALLSAP        88           /* all upper/lower SAPs */
#define STPEERSAP       89           /* configure peer sap */
/* Used by V5 */
#define STVINTERFACE    83           /* V5 Interface */
#define STVPORT         84           /* V5 port */
#define STVLINK         85           /* V5 Link */

/* Used by PLOA-CIP */
#define STCIPPVC        86           /* PLOA Cfg. request to configure some 
                                        more PVC on the fly */
#define STCIPARP        87           /* PLOA Cfg. request to configure some 
                                        extra IP-ATM binding on the fly */
/* used by Q.93B for AalConParam of signaling channels */
#define STSIGCONPARAM   90           /* config Signaling connection parameters */
#define STDELSIGPARAM   91           /* Delete Signaling connection parameters */

/* used by System Manager */
#define STVPROC         92           /* Configure virtual node */
#define STPPROC         93           /* Configure physical node */
#define STASSOC         94           /* Configire association between vnodes */

/* used by TCAP over TCP/IP */
#define STSERVER        95           /* configure TCP/UDP server */

/* Used by Lan Emulation Services */
#define STLECSTLV       96           /* PLOA LECS TLVs */
#define STLESLEC        97           /* PLOA LEC in LES database */
#define STLESSMS        98           /* PLOA Multicast Server Known to LES */
#define STLESLECSMS     99           /* PLOA LEC assoc. with SMS */
#define STBUSLEC        100          /* PLOA LEC in BUS Database */
#define STSMS           101          /* PLOA LANE Selective M-cast Server */
#define STSMSGRP        102          /* PLOA SMS Multicast Group */
#define STSMSLEC        103          /* PLOA LEC Associated with SMS */
/* used by PLOA - PPPoA */
#define STMASAP         104          /* POOA MASAP - PPPoA upper SAP */

/* Used By PNNI */
#define STTFCMTRCNODE   105          /* configure metrics of the lowest level
                                     / * node */
#define STNHSSAP        106          /* PLOA NHS SAP */
#define STMPSDISCTBL    107          /* PLOA MPS */
#define STMPSINGRTBL    108          /* PLOA MPS */
#define STMPSEGRTBL     109          /* PLOA MPS */
#define STARISAP        110          /* PLOA ARI Sap */

#define STNHSCP         111          /* PLOA NHS */
#define STNHSNXTHOP     112          /* PLOA NHS */
#define STNHSTRCACHE    113          /* PLOA NHS */
#define STNHSEXTNS      114          /* PLOA NHS */
#define STNHSEGRCACHE   115          /* PLOA NHS */
#define STNHSVENDATA    116          /* PLOA NHS */
/* used by H.323 Control */
#define STSSAP          117          /* H.323 Session SAP element */
#define ST323ENT        118          /* H323 entity element */
#define STCALL          119          /* H.323 call element */
#define STGRSSAP        120          /* H.323 group SSAP */

/* used by Q.93B for SVPC status */
#define STVPC           121          /* Switched virtual path */

#define STCIPSRVR       122          /* PLOA CIP Server */

/* used by RTP/RTCP */
#define STSESSION       123          /* RTP session */

/* Used by Q.930 */
#define STCTLDPCB       124          /* Configure controlled interface */

#define STGCPENT        125          /* MGCP peer entity */

/* Used by SCCP */
#define STNW            126          /* Network */
#define STASSO          127          /* SCCP GTT Association */
#define STDELASSO       128          /* SCCP Delete GTT Association */

/* Used by IME auto configuration of PVCs */
#define STSRVCTYPE      129           /* service type */
#define STSRVCCONNINFO  130           /* service connection info */
#define STAALPROFILE    131           /* AAL[x] profiles; x = 1, 2, 34, 5 */
/* used by MPLS-LDP (+CR) */
#define STPEER          132          /* LDP Peer */
#define STFEC           133          /* MPLS FEC  */
#define STEXPRTE        134          /* CR-LDP Explicit Route */
#define STRMSAP         135          /* LDP RM SAP */
#define STINT           136          /* LDP Interface */
#define STROUTSAP       137          /* LDP ARI SAP */
#define STFWDSAP        138          /* LDP Forwarder SAP */
#define STSESS          139          /* LDP Session */
#define STADDRESS       140          /* LDP Address */

/* used by AAL2 Signaling */
#define STALTSAP        141          /* AAL2 Service user */
#define STSNTSAP        142          /* MTP3B SAP */
#define STGRALTSAP      143          /* ALT Group */
#define STGRSNTSAP      144          /* SNT Group */
#define STPATH          145          /* AAL2 path  */
#define STDPC           146          /* DPC - MTP3B */
/* used by Annex G */
#define STBE                       147
#define STGRBESAP                  148
#define STTPTSRV                   149
#define STDESC                     150
/* used by PLOA-PXY */
#define STPXYSAP        151          /* PLOA PXY SAP configuration */
#define STCIPARPENT     152          /* PLOA StaReq & CntrlReq for single arpEnt */
#define STMACONLST      153          /* PLOA StaReq for list of conns */
#define STMACON         154          /* PLOA StaReq for single conn */


/* Used by SIP */
#define STSIPENT        155          /* SIP Entity */

/* Used by GCP 1.2 and above */
#define STGCPMGC        156          /* MGC Entity */
#define STGCPMG         157          /* MG Entity */

/* Used bu HU - H.323 Service User */
#define STLWRSAP        158

/* new elements added in sccp3.2 */
#define STNIDPC         159          /* NID to DPC mapping */
#define STNWSS7         160          /* NW specific NID to SS7-NID mapping */
#define STTRFLIM        161          /* Traffic limitation data config */
#define STMSGIMP        162          /* Message importance data config */

/* new elements for SCCP */
#define STDELNW         163          /* delete network */
#define STDELTSAP       164          /* delete transport sap */
#define STDELNSAP       165          /* delete network sap */
/* new elements for 3GPP-RLC */
#define STCRSAP         166          /* RLC Control SAP */
#define STRLSAP         167          /* RLC Data SAP */
#define STMKSAP         168          /* RLC MAC SAP */

/* new elements for 3GPP-NBAP */
#define STNBUSAP        169          /* Upper SAP to NBAP */
#define STIBPROT        170          /* NBAP Protocol     */

/* new elements for MPLS-RSVP (+TE) */
#define STLASAP         171  /* MPLS-Appl. Sap */
#define STNBR           172  /* RSVP Neighbor */
#define STRVINT         173  /* RSVP Interface */
#define STLTSAP         174  /* MPLS Resource Manager SAP */

/* new elements for FP */
#define STCSAP          175  /* FP Control User */ 
#define STUSAP          176  /* FP Data User    */
#define STTPTENDP       177  /* SCTP endpoint   */

/* new elements for LAPDm */
#define STLSAP          178
#define STRRSAP         179

/*gen_h_001.main_127 - incremented values*/
/* gen_h_001.main_123 - Add  new elements for 3GPP-PDCP */
#define STCTSAP         180          /* PDCP Control SAP */
#ifndef QC /* gen_h_001.main_136: Fixing compilation warnings */
#ifndef IE
#define STTCSAP         181          /* PDCP Data SAP */
#endif
#endif
#define STPDCPENT       182          /* PDCP Entity */
/*-- gen_h_001.main_130 --*/
#define STINST          183          /* IuUP Instance */
#endif
/* gen_h_001.main_134 - lte rlc 2.1 */
/* elements for NR RLC SAPs */
#define STCKWSAP        184          /*!< RLC Control SAP element. */
#define STKWUSAP        185          /*!< RLC Data SAP element. */
#define STRGUSAP        186          /*!< RLC MAC SAP element. */
/* elements for LTE-PDCP SAPs */
#ifdef RM_INTF
#define STRMUSAP        189          /*!< LTE RRM control SAP element. */
#define STRGMSAP        190
#endif

/* Mngmt.t.cntrl.action, Mngmt.hdr.elmId.elmntInst1 */

#define AENA             1           /* enable */
#define ADISIMM          2           /* disable - immediately */
#define ADISGRC          3           /* disable - gracefully */
#define ARST             4           /* reset */
#define ADEL             7           /* delete */
#define ABND             30          /* bind */
#define AUBND            31          /* disable + unbind */
#define AUBND_DIS        AUBND       /* unbound disable */

/* actions for mtp3 */
#if 0
#define ADELROUT         32          /* delete rout control block */
#define ADELLNKSET       33          /* delete all linkset control blocks, link control blocks 
                                        for a given linkset id */
#define ADELCMBLNKSET    34          /* delete linkset control block for a given combined linkset id */
#define ADELLNK          35          /* delete link control block */
#endif
#define ASHUTDOWN        36          /* shutdown the layer */
#if 0
#define ATRAP            37          /* trap */
#define AGEN_STA         38          /* generate status indications -MSOC */
/* actions for fault tolerance */
#define AGO_ACT                  38   /* go active */
#define AGO_SBY                  39   /* go standby */
#define AWARMSTART               40   /* start warm start */
#define AABORT                   41   /* abort warm start or controlled sw */
#define ASYNCHRONIZE             42   /* start controlled switchover */
#define ANOACT                   43   /* null action (nop) */
#define AHOLDQUEUE               44   /* hold queue, for the message router */
#define ARLSQUEUE                45   /* hold queue, for the message router */
#define APEER_PING               46   /* peer ping, for peer system agent */
#define ADIS_PEER_SAP            47   /* enable/disable peer sap */
#define AGEN_FAULT               48   /* fault injection action */
#define AAUDIT                   49   /* Perform Audits on the layer */
#define ADISIMM_L2               50   /* Disable Layer 2 only */
#define ADEACTLNKSET_L2          51   /* deactivate link set: all links should be deactivated
                                         only at L2 level */
#define AGEN_MSG                 52   /* message generation control action */
/* For PLOA */
#define ASNDTRIGG                53   /* Generate Flow detection trigger  */

#define AXIDINI                  54   /* GPRS: start XID initialization */
#define APMLEN                   55   /* GPRS: change the N202 value of 
                                         an LLC link */
/* For Q.930/Q.931 */
#define AADD_BCHAN               56   /* Provision B Channel */     
#define ADEL_BCHAN               57   /* De-Provision B Channel */
#define AMOOS                    58   /* Put the D-Channel MOOS State */
#define ASRVMSG_ON               59   /* Turn on service message capability */ 
#define ASRVMSG_OFF              60   /* Turn off service message capability */
#define ARESTART                 61   /* Restart Interface/Channel */
/* For TCR 0004.01 */
#define AMODIFY                  62   /* Modify trace length */

/* For AAL2 Signaling */
#define ASTOPRST                 63   /* Stop Reset procedure */ 
#define ABLK                     64   /* Block Procedure - AAL2 */ 
#define AUBLK                    65   /* unblock procedure */ 

/* For GCP 1.2 */
#define AHANDOFF                 66   /* Handoff procedure */
#define AMATEDCFG_ADD            67   /* Configure MG as mated pair */
#define AMATEDCFG_RMV            68   /* Disassociate two MGs which were in a mated
                                         pair cfg */
#define AFAILOVER                69   /* Failover Procedure */
/* For SIP */
#define AADD_ASSOC_TPTSRV        70   /* Add transport server association */
#define ADEL_ASSOC_TPTSRV        71   /* Delete transport server association */
/* for SCCP congestion control (sccp3.2) */
#define ACONG                    72   /* congestion cntrl - start sending SSC */
/* For OAM */
#define ASTRTOAMLL               72   /* OAM Start Loopback */
#define AENDOAMLL                73   /* OAM End Loopback */
#define ASTRTPMG                 74   /* OAM Start Performance Monitoring and Generation */
#define AENDPMG                  75   /* OAM End Performance Monitoring and Generation */
#define ASTRTPM                  76   /* OAM Start Performance Monitoring */
#define ASTRTPG                  77   /* OAM Start Performance Generation */
#define ASTRTPMLL                78   /* OAM Start PM Loopback of FMC's */
#define AENDPMLL                 79   /* OAM End Performance Generation */
#define ARSTPM                   80   /* OAM Reset counters of PM block */
#define AMODABORT                81 /* Abort Modification Request */
#define AADD_ASSOC_ENDP          82
#define ADEL_ASSOC_ENDP          83
/* for support of adding/deleting PC (sccp3.2) */
/* gen_h_001.main_121 - Support for adding/deleting PC */
#define AADD_OPC                 84
#define ADEL_OPC                 85
/* gen_h_001.main_126 - Addition - New hash defines start/stop of MTP3 Audit */
/* gen_h_001.main_127 - changed macro values */
#define ASTRT_AUDIT              90
#define ASTOP_AUDIT              91
#define ASTRT_LNK_AUDIT          92
#define ASTRT_LNKSET_AUDIT       93
#define ASTRT_RTE_AUDIT          94
#define ASTOP_LNK_AUDIT          95
#define ASTOP_LNKSET_AUDIT       96
#define ASTOP_RTE_AUDIT          97
/* gen_h_001.main_125 - Addition - SUA support*/
#ifdef LSPV2_8 
#define AADD_ASP                 86
#define ADEL_ASP                 87
#define ADEL_ASPSSN              88
#define ADEL_ASPCPC              89
#endif
/* Added for NBAP to support SCT(SCTP) as lower interface */
#define AEOPENR 100
#define AECLOSER 101
#define AESTABLISH 102
#define ATERMINATE 103
#define AHBEAT_ENB_ASSOC 104
#define AHBEAT_DIS_ASSOC 105
#define AHBEAT_ENB_DSTADDR 106
#define AHBEAT_DIS_DSTADDR 107
#endif
/* Mngmt.t.cntrl.subAction */
  
#define SAELMNT          1           /* specified element */
#define SAACNT           2           /* accounting generation */
#define SAUSTA           3           /* unsolicited status generation */
#define SATRC            4           /* trace generation */
#define SADBG            5           /* debugging */
/* defines for MxxStsReq */
  
#define ZEROSTS          0           /* zero statistics counters */
#define NOZEROSTS        1           /* dont zero statistics counters */
#if 0
/* defines for MxxCfgReq */

#define THRSHA           0           /* 00% resources available */
                                     /* scc drops frames */
#define THRSHB           1           /* 10% resources available */
                                     /* lapb and lapd send rnr frames */
#define THRSHC           2           /* 20% resources available */
                                     /* lapb and lapd send rr frames */
#define THRSHD           3           /* 30% resources available */
                                     /* x.25 es and x.25 is send reset packets */
#define THRSHE           4           /* 40% resources available */
                                     /* x.25 es and x.25 is send clear packets */
                                     /* to call packets */
#define LSAP             1           /* Link Level SAP Type */
#define XTSAP            2           /* Transport SAP Type */
#endif

#ifdef IGNORE
#undef IGNORE
#define IGNORE           0           /* Ignore SAP Type */
#else
#define IGNORE           0           /* Ignore SAP Type */
#endif /* IGNORE */



#ifndef CMFILE_REORG_1

/* defines for Q.93B */
 
/* parameter values that are at management and at upper interface */
/* addressing related defines */

/* maximum size of VCC table */
#define MAX_ATMVCCTBL_SZ      16

/* maximum number of ATM addresses in the ATM address table */
#define  MAX_ATMADDRTBL_SZ    4
 
/* authority and format identifiers for OSI NSAP addresses */
#if 0 
/* low layer information - layer 2 id */

#define AM_LLI_L2ID             2       /* layer 2 id */

/* low layer information - layer 2 protocol */

#define AM_LLI_L2PROT_ISO1745   0x01    /* basic mode ISO 1745 */
#define AM_LLI_L2PROT_Q921      0x02    /* CCITT Rec. Q.921 */
#define AM_LLI_L2PROT_X25LNK    0x06    /* CCITT Rec. X.25, link layer */
#define AM_LLI_L2PROT_X25MLNK   0x07    /* CCITT Rec. X.25, multilink */
#define AM_LLI_L2PROT_LAPB      0x08    /* extended LAPB (half duplex) */
#define AM_LLI_L2PROT_HDLC_ARM  0x09    /* HDLC ARM (ISO 4335) */
#define AM_LLI_L2PROT_HDLC_NRM  0x0a    /* HDLC NRM (ISO 4335) */
#define AM_LLI_L2PROT_HDLC_ABM  0x0b    /* HDLC ABM (ISO 4335) */
#define AM_LLI_L2PROT_LLC       0x0c    /* LAN LLC (ISO 8802/2) */
#define AM_LLI_L2PROT_X75SLP    0x0d    /* CCITT Rec. X.75 SLP */
#define AM_LLI_L2PROT_Q922      0x0e    /* CCITT Rec. Q.922 */
#define AM_LLI_L2PROT_USER      0x10    /* user specified */
#define AM_LLI_L2PROT_ISO7776   0x11    /* ISO 7776 DTE-DTE operation */

/* low layer information - layer 2 mode of operation */

#define AM_LLI_L2MODE_NORMAL    1       /* normal mode of operation */
#define AM_LLI_L2MODE_EXT       2       /* extended mode of operation */

/* low layer information - layer 2 Q.933 use */

#define AM_LLI_L2Q933_USE       0       /* when Rec. Q.933 coding not used */

/* low layer information - layer 3 id */

#define AM_LLI_L3ID             3       /* layer 3 id */

/* low layer information - layer 3 protocol */

#define AM_LLI_L3PROT_X25PKT    0x06    /* CCITT Rec. X.25, packet layer */
#define AM_LLI_L3PROT_ISO8208   0x07    /* ISO/IEC 8208 */
#define AM_LLI_L3PROT_ISO8878   0x08    /* X.223/ISO 8878 */
#define AM_LLI_L3PROT_ISO8473   0x09    /* ISO/IEC 8473 */
#define AM_LLI_L3PROT_T70       0x0a    /* CCITT Rec. T.70 */
#define AM_LLI_L3PROT_ISO9577   0x0b    /* ISO/IEC TR 9577 */
#define AM_LLI_L3PROT_USER      0x80    /* user specified */

/* low layer information - layer 3 mode of operation */

#define AM_LLI_L3MODE_NORMAL    1       /* normal packet sequence numbering */
#define AM_LLI_L3MODE_EXT       2       /* extended packet sequence numbering */

/* low layer information - layer 3 default packet size */

#define AM_LLI_L3PKTSIZE_16     0x04    /* default packet size 16   octets */
#define AM_LLI_L3PKTSIZE_32     0x05    /* default packet size 32   octets */
#define AM_LLI_L3PKTSIZE_64     0x06    /* default packet size 64   octets */
#define AM_LLI_L3PKTSIZE_128    0x07    /* default packet size 128  octets */
#define AM_LLI_L3PKTSIZE_256    0x08    /* default packet size 256  octets */
#define AM_LLI_L3PKTSIZE_512    0x09    /* default packet size 512  octets */
#define AM_LLI_L3PKTSIZE_1024   0x0a    /* default packet size 1024 octets */
#define AM_LLI_L3PKTSIZE_2048   0x0b    /* default packet size 2048 octets */
#define AM_LLI_L3PKTSIZE_4096   0x0c    /* default packet size 4096 octets */

/* low layer information - layer 3 ISO/IEC TR 9577 Initial Protocol Id */

/* ec005.12: corrected value */
#define AM_LLI_ISO9577_IPI_SNAP 0x80    /* IPI - SNAP */
#define AM_LLI_ISO9577_IPI_IP   0xCC    /* IPI - Internet Protocol */

/* low layer information - layer 3 SNAP Id */

#define AM_LLI_SNAP_ID          0x00    /* SNAP id */

/* high layer information type */

#define AM_HLITYP_ISO        0x00    /* ISO */
#define AM_HLITYP_USR        0x01    /* user specific */
#define AM_HLITYP_HLPROF     0x02    /* high layer profile */
#define AM_HLITYP_APPID      0x03    /* vendor-specific application id */
#define AM_HLITYP_BISDN      0x04    /* reference to ITU-T SG1 B-ISDN teleservice recommendation */

/* ABR Additional parameter identifiers */

#define AM_AAP_FAPR_ID    0xC2  /* forward additional parameter record identifier */
#define AM_AAP_BAPR_ID    0xC3  /* forward additional parameter record identifier */

/* Extended QOS parameter identifiers */

#define AM_EQP_AFPCDV_ID  0x94  /* acceptable forward   peak-to-peak cell delay variation identifier */
#define AM_EQP_ABPCDV_ID  0x95  /* acceptable backward  peak-to-peak cell delay variation identifier */
#define AM_EQP_CFPCDV_ID  0x96  /* cumulative forward   peak-to-peak cell delay variation identifier */
#define AM_EQP_CBPCDV_ID  0x97  /* cumulative backward  peak-to-peak cell delay variation identifier */
#define AM_EQP_AFCLR_ID   0xA2  /* acceptable forward  cell loss ratio identifier */
#define AM_EQP_ABCLR_ID   0xA3  /* acceptable backward cell loss ratio identifier */

/* Extended QOS parameter origin token values */

#define AM_EQP_ORG_USR    0x00  /* originating user */
#define AM_EQP_ORG_INTNET 0x01  /* intermediate network */
/* ABR Setup parameter identifiers */

#define AM_ASP_FAICR_ID   0xC2  /* forward  ABR initial cell rate identifier */
#define AM_ASP_BAICR_ID   0xC3  /* backward ABR initial cell rate identifier */
#define AM_ASP_FATBE_ID   0xC4  /* forward  ABR transient buffer exposure identifier */
#define AM_ASP_BATBE_ID   0xC5  /* backward ABR transient buffer exposure identifier */
#define AM_ASP_CRMFRTT_ID 0xC6  /* cumulative RM fixed round trip time identifier */
#define AM_ASP_FRIF_ID    0xC8  /* forward  rate increment factor identifier */
#define AM_ASP_BRIF_ID    0xC9  /* backward rate increment factor identifier */
#define AM_ASP_FRDF_ID    0xCA  /* forward  rate decrement factor identifier */
#define AM_ASP_BRDF_ID    0xCB  /* backward rate decrement factor identifier */

/* ATM Transfer capability */

#define AM_ATC_NRTVBR1        0x00   /* Non-real time VBR */
#define AM_ATC_RTVBR1         0x01   /* Real time VBR */
#define AM_ATC_NRTVBR2        0x02   /* Non-real time VBR */
#define AM_ATC_CBR1           0x04   /* CBR */
#define AM_ATC_CBR2           0x05   /* CBR */
#define AM_ATC_CBR3           0x06   /* CBR */
#define AM_ATC_CBRCLR         0x07   /* CBR with CLR commitment on CLP=0+1 */
#define AM_ATC_NRTVBR3        0x08   /* Non-real time VBR */
#define AM_ATC_RTVBR2         0x09   /* Real time VBR */
#define AM_ATC_NRTVBR4        0x0A   /* Non-real time VBR */
#define AM_ATC_NRTVBRCLR      0x0B   /* Non-real time VBR with CLR commitment on CLP=0+1 */
#define AM_ATC_ABR            0x0C   /* ABR */
#define AM_ATC_RTVBRCLR       0x13   /* Real time VBR with CLR commitment on CLP=0+1 */

#endif

/* defines for UME */

#define UM_MAXLEN_OBJ_ID     34      /* max length of an object-id */
#if 0
/* PNNI qos class definitions
 */
#define PN_QOS_CLASS_UBR          0    /* unspecified */
#define PN_QOS_CLASS_CBR          1    /* constant bit rate */
#define PN_QOS_CLASS_VBR_RT       2    /* variable bit rate - real time */
#define PN_QOS_CLASS_VBR_NRT      3    /* variable bit rate - non real time */
#define PN_QOS_CLASS_ABR          4    /* available bit rate */
 
/* maximum number of ports per neighbor -- change if you expect more
 */
#define PN_MAX_PORTS_NBR           5
#endif

/* Peer group and node identifier length */
#define PN_PGID_LEN   14
#define PN_NODEID_LEN 22

#if 0
/* maximum number of qos classes */
#define PN_NUM_QOS_CLASSES        5

/* defines for PNNI */
 
#define SW_ATMF_RTE_PNNI1    0       /* PNNI Phase 1 */
 
/* OAM Traffic Descriptor */
 
#define AM_OTD_NUSRORGFMI       0x00      /* No user originated fault mgmt ind. */
#define AM_OTD_USRORGFMI        0x01      /* user originated fault mgmt ind. */
 
#define AM_OTD_OAMF5OPT         0x00      /* The use of end-to-end OAM F5 Flow is optional
*/
#define AM_OTD_OAMF5MAND        0x01      /* The use of end-to-end OAM F5 Flow is mandatory
 */
 
#define AM_OTD_NUSRSHAPEREQ     0X00      /* No user req. on shaping by network */
 
#define AM_OTD_NAGGUSRSHAPE     0X01      /* Aggregate shaping not allowed */
 
#define AM_OTD_ETOEF5_0         0X00      /* 0% of FPCR/BPCR (CLP=0+1) */
#define AM_OTD_ETOEF5_01        0x01      /* 0.1% of FPCR/BPCR (CLP=0+1) */
#define AM_OTD_ETOEF5_1         0x04      /* 1% of FPCR/BPCR (CLP=0+1) */
#endif
#endif /* CMFILE_REORG_1 */

/* degines for management confirms */

/* status */
#define LCM_PRIM_OK             0         /* OK , activity completed */
#define LCM_PRIM_NOK            1         /* NOK, activity completed */
#define LCM_PRIM_OK_NDONE       2         /* OK, activity not completed */

/* reason */
#define LCM_REASON_NOT_APPL             0    /* not applicable */
#define LCM_REASON_INVALID_ENTITY       1    /* invalid entity */
#define LCM_REASON_INVALID_INSTANCE     2    /* invalid instance */
#define LCM_REASON_INVALID_MSGTYPE      3    /* invalid message type */
#define LCM_REASON_MEM_NOAVAIL          4    /* memory not avail */
#define LCM_REASON_INVALID_ELMNT        5    /* invalid hdr.elmnt */
#define LCM_REASON_RECONFIG_FAIL        6    /* reconfiguration */
#define LCM_REASON_REGTMR_FAIL          7    /* timer registration failed */
#define LCM_REASON_GENCFG_NOT_DONE      8    /* gene config not done */
#define LCM_REASON_INVALID_ACTION       9    /* invalid control action */
#define LCM_REASON_INVALID_SUBACTION    10   /* invalid control subaction */
#define LCM_REASON_INVALID_STATE        11   /* invalid state */
#define LCM_REASON_INVALID_SAP          12   /* invalid sap identifier */
#define LCM_REASON_INVALID_PAR_VAL      13   /* invalid parameter value */
#define LCM_REASON_QINIT_FAIL           15   /* queue initialization failed */
#define LCM_REASON_NEG_CFM              16   /* negative confirmation */
#define LCM_REASON_HASHING_FAILED       20    /* Hashing failed */
#if 0
#define LCM_REASON_UPDTMR_EXPIRED       17   /* update timer expired */
#define LCM_REASON_MISC_FAILURE         18    /* miscellaneous failures */
#define LCM_REASON_EXCEED_CONF_VAL      19   /* Exceeds configured value */
#define LCM_REASON_PEERCFG_NOT_DONE     21   /* swft -peer sap not configured */
#define LCM_REASON_PRTLYRCFG_NOT_DONE   22   /* swft -portable lyr not configured */
/* common Distributed FT/HA related failure reasons */      
#define LCM_REASON_INV_RSET             23   /* Inv. rset */
#define LCM_REASON_INV_RSET_RANGE       24   /* Inv. rset range */
#define LCM_REASON_INV_RSET_TYPE        25   /* Inv. type  of rset type */
#define LCM_REASON_INV_RSET_QUAL        26   /* Inv. qual. of rset type*/
#define LCM_REASON_INV_INTERFACE        27   /* Inv. Dist. Fn interface */
#define LCM_REASON_INV_DIST_TYPE        28   /* Inv. type  of dist type*/
#define LCM_REASON_INV_DIST_QUAL        29   /* Inv. qual. of dist type*/
#define LCM_REASON_NAK_RCVD             30   /* Got a nak failure */
#define LCM_REASON_TIMEOUT              31   /* Got a timeout failure  */
#define LCM_REASON_PURE_FTHA            32   /* A req for Dist FTHA is recv by a Pure FTHA mod */
#define LCM_REASON_DIST_FTHA            33   /* A req for Pure FTHA is recv 
                                              * by a Dist FTHA mod */
#define LCM_REASON_INV_KEY              34   /* Invalid key */
#define LCM_REASON_SW_INCOMP            35   /* enable node fail reason because
                                              * of software interface version
                                              * incompatability */
#define LCM_REASON_VERSION_MISMATCH     36   /* interface version mismatch */
#define LCM_REASON_SWVER_NAVAIL         37   /* interface version not found */
#define LCM_REASON_INVALID_RTENT        38   /* invalid routing entery */
/* gen_h_001.main_121 - Added LCM_REASONS for reconfiguration of PC at SCCP */
#define LCM_REASON_MAXSPC_EXCEEDING     39   /* maximum configured SPC's exceeding */
#define LCM_REASON_WRONG_DEFAULT_SPC    40   /* wrong default spc rcvd in cntrl req */
#define LCM_REASON_SPC_EXISTS           41   /* spc already exists in the network */
#define LCM_REASON_MINSPC_REACHED       42   /* network is already reached MINSPC */
#define LCM_REASON_MORE_SPC_THAN_CONFIGURED   43  /* control request has got more spcs that configured in the network */
#define LCM_REASON_DFL_SPC_DEL_NOT_ALLOWED    44  /* control request has got default spcs that configured in the network */
#define LCM_REASON_NOTHING_TO_DELETE    45   /* control request has got zero spcs to delete */
#define LCM_REASON_SPC_ALREADY_DELETED  46   /* spc's are already deleted */
#define LCM_REASON_LYR_SPECIFIC         256   /* protocol specific reasons */

/* gen_h_001.main_125- Addition - SUA support*/
#define LCM_REASON_ASP_CONFIG          47   /* ASP Configuration Error */
/* gen_h_001.main_142:Added new error type*/
#define LCM_REASON_ULLOCK_INIT_FAILED  48   /* UL Lock creation Error */
#endif

/* category */
#define LCM_CATEGORY_PROTOCOL           1     /* protocol related */
#define LCM_CATEGORY_INTERFACE          2     /* interface related */
#define LCM_CATEGORY_INTERNAL           3     /* internal errors */
#define LCM_CATEGORY_RESOURCE           4     /* system resources  */
#define LCM_CATEGORY_PSF_FTHA           5     /* PSF-Fault tolernace High Av.*/
#define LCM_CATEGORY_PERF_MONIT         6     /* SCCP Error performance */
/*gen_h_001.main_137:moved category of states of DNS records to lso.h*/

#define LCM_CATEGORY_LYR_SPECIFIC       256   /* protocol specific category */

/* events */
#define LCM_EVENT_UI_INV_EVT            1   /* upper interface invalid event */
#define LCM_EVENT_BND_FAIL              8   /* Bind failure */
#define LCM_EVENT_BND_OK                11  /* bind ok  */
#define LCM_EVENT_SMEM_ALLOC_FAIL       12  /* static memory alloc fail */
#define LCM_EVENT_DMEM_ALLOC_FAIL       13  /* Dynamic mmemory alloc fail */

#if 0
#define LCM_EVENT_LI_INV_EVT            2   /* lower interface invalid event */
#define LCM_EVENT_PI_INV_EVT            3   /* peer interface invalid event */
#define LCM_EVENT_INV_EVT               4   /* general invalid event */
#define LCM_EVENT_INV_STATE             5   /* invalid internal state */
#define LCM_EVENT_INV_TMR_EVT           6   /* invalid timer event */
#define LCM_EVENT_MI_INV_EVT            7   /* mngmnt interface invalid event */
#define LCM_EVENT_NAK                   9   /* destination naked a request */
#define LCM_EVENT_TIMEOUT               10  /* timeout when waiting for reply */
/*Alarms for distributed FTHA environment*/
#define LCM_EVENT_SEQERR                14  /* seq error (standby) */
#define LCM_EVENT_OOM                   15  /* board mem threshold exceeded */
#define LCM_EVENT_UPDMSG_ERR            16  /* update message decode error */
#define LCM_EVENT_HTBT_EXP              17  /* Heartbeat timer expiry */
#define LCM_EVENT_TRANSLATION_FAILURE   18  /* primitive translation failure */
#endif
/*gen_h_001.main_137:Moved Events for alarm indication at various Dns Events to lso.h*/

#define LCM_EVENT_LYR_SPECIFIC          256   /* protocol specific events */

/* causes */
#define LCM_CAUSE_UNKNOWN               0     /* unknown cause - not filled */
#define LCM_CAUSE_OUT_OF_RANGE          1     /* out of range */
#define LCM_CAUSE_INV_SAP               2     /* NULL/unknown sap */
#define LCM_CAUSE_INV_SPID              3     /* invalid service provider */
#define LCM_CAUSE_INV_SUID              4     /* invalid service user */
#define LCM_CAUSE_INV_NETWORK_MSG       5     /* invalid network message */
#define LCM_CAUSE_DECODE_ERR            6     /* message decoding problem */
#define LCM_CAUSE_USER_INITIATED        7     /* user initiated */
#define LCM_CAUSE_MGMT_INITIATED        8     /* mgmt initiated */
#define LCM_CAUSE_INV_STATE             9     /* Invalid state */
#define LCM_CAUSE_TMR_EXPIRED           10    /* Invalid state */
#define LCM_CAUSE_PROT_NOT_ACTIVE       12    /* protocol layer not active */
#define LCM_CAUSE_INV_PAR_VAL           13    /* invalid parameter value */
#define LCM_CAUSE_NEG_CFM               14    /* negative confirmation */
#define LCM_CAUSE_MEM_ALLOC_FAIL        15    /* memory allocation failure */
#if 0
#define LCM_CAUSE_INV_MSG_LENGTH        11    /* Invalid state */
#define LCM_CAUSE_HASH_FAIL             16    /* hashing failure */
#define LCM_CAUSE_VERSION_MISMATCH      17    /* interface version mismatch */
#define LCM_CAUSE_SWVER_NAVAIL          18    /* intf ver not found */
#define LCM_CAUSE_DLGFAIL_DUETO_RECOVERY 19   /* dialogue has failed dur to recovery */
#define LCM_CAUSE_INV_ACTION            20    /* Invalid Control Action */
/* gen_h_001.main_129  - defined new cause */
#define LCM_CAUSE_NETWK_INITIATED       21    /* user initiated */
#endif
#define LCM_CAUSE_LYR_SPECIFIC          256   /* protocol specific causes */

#if 0
/* Interface identifier names for rolling upgrade. Each of the product *
 * interface is given a unique number */
/* Layer management interface identifiers */
#define LMSIF   1     /* MOS stack manager interface ID */
#define LNSIF   2     /* NTSS stack manager interface ID */
#define LRYIF   3     /* relay stack manager interface ID */
#define LAMIF   4     /* Q.93B stack manager interface ID */
#define LASIF   5     /* Q.SAAL stack manager interface ID */
#define LPNIF   6     /* PNNI stack manager interface ID */
#define LACIF   7     /* AC stack manager interface ID */
#define LECIF   8     /* LEC stack manager interface ID */
#define LESIF   9     /* LE Services stack manager interface ID */
#define LUMIF   10    /* UME stack manager interface ID */
#define LIMIF   11    /* IME stack manager interface ID */
#define LSAIF   12    /* Q.2140 stack manager interface ID */
#define LSEIF   13    /* SE stack manager interface ID */
#define LISIF   14    /* IS stack manager interface ID */
#define LQIIF   15    /* QI stack manager interface ID */
#define LPQIF   16    /* PQ stack manager interface ID */
#define LSDIF   17    /* MTP-2 stack manager interface ID */
#define LSNIF   18    /* MTP-3 stack manager interface ID */
#define LSPIF   19    /* SCCP  stack manager interface ID */
#define LSTIF   20    /* TCAP  stack manager interface ID */
#define LIEIF   21    /* INAP  stack manager interface ID */
#define LQCIF   22    /* CAP   stack manager interface ID */
#define LTTIF   23    /* TCAP over TCP/IP stack manager interface ID */
#define LMAIF   24    /* MAP  stack manager interface ID */
#define LSIIF   25    /* ISUP  stack manager interface ID */
#define LTPIF   26    /* TUP  stack manager interface ID */
#define LSRIF   27    /* MTP-3 Simple Router stack manager interface ID */
#define LINIF   28    /* Q.930/Q.931 stack manager interface ID */
#define LXNIF   29    /* network layer - XG, EI, XI, XE stack manager interface ID */
#define LBDIF   30    /* data link layer- ER, BR, BD, LD, LB stack manager interface ID */
#define LAPIF   31    /* APAD stack manager interface ID */
#define LFRIF   32    /* Frame Relay stack manager interface ID */
#define LMEIF   33    /* data link layer - message exchange stack manager interface ID */
#define LWDIF   34    /* MTP-2 Wrapper stack manager interface ID */
#define LWNIF   35    /* MTP-3 Wrapper stack manager interface ID */
#define LWIIF   36    /* ISUP Wrapper stack manager interface ID */
#define LWUIF   37    /* TUP Wrapper stack manager interface ID */
#define LWSIF   38    /* SCCP Wrapper stack manager interface ID */
#define LWCIF   39    /* TCAP Wrapper stack manager interface ID */
#define LBIIF   40    /* B-ISUP  stack manager interface ID */
#define LFMIF   41    /* fault manager stack manager interface ID */
#define LFNIF   42    /* Q933 stack manager interface ID */
#define LEVIF   43    /* V5 Envelope Function stack manager interface ID */
#define LLVIF   44    /* LAPV stack manager interface ID */
#define LNVIF   45    /* V5.1 PSTN stack manager interface ID */
#define LVFIF   46    /* VF Layer stack manager interface ID */
#define LIXIF   47    /* X.31 stack manager interface ID */
#define LFAIF   48    /* Fujitsu ALC/NTC/ATC  driver stack manager interface ID */
#define LPLIF   49    /* PMC Sierra LASAR driver stack manager interface ID */
#define LAFIF   50    /* FR-ATM interworking stack manager interface ID */
#define LCVIF   51    /* V5.X Control Protocol stack manager interface ID */
#define LMVIF   52    /* V5.X System Manager stack manager interface ID */
#define LBVIF   53    /* V5.X BCC Protocol stack manager interface ID */
#define LLKIF   54    /* V5.2 Link stack manager interface ID */
#define LPVIF   55    /* V5.X Protection Protocol stack manager interface ID */
#define LCCIF   56    /* CC  stack manager interface ID */
#define LZCIF   57    /* ZC  stack manager interface ID */
#define LRMIF   58    /* RM  stack manager interface ID */
#define LZRIF   59    /* ZR  stack manager interface ID */
#define LRTIF   60    /* RT  stack manager interface ID */
#define LZSIF   61    /* ZS  stack manager interface ID */
#define LSFIF   62    /* SF  stack manager interface ID */
#define LXMIF   63    /* XM  stack manager interface ID */
#define LBWIF   64    /* PSIF - B-ISUP stack manager interface ID */
#define LIWIF   65    /* PSIF - ISUP stack manager interface ID */
#define LQWIF   66    /* PSIF - ISDN stack manager interface ID */
#define LAWIF   67    /* PSIF - Q.93B stack manager interface ID */
#define LZTIF   68    /* PSF - TCAP stack manager interface ID */
#define LDTIF   69    /* PSF - TCAP stack manager interface ID */
#define LZPIF   70    /* PSF - SCCP stack manager interface ID */
#define LZNIF   71    /* PSF-MTP3 (FT/HA) stack manager interface ID */
#define LZIIF   72    /* PSF-ISUP (FT/HA) stack manager interface ID */
#define LZQIF   73    /* PSF-ISDN (FT/HA) stack manager interface ID */
#define LZMIF   74    /* PSF-Q.93B (FT/HA) stack manager interface ID */
#define LSGIF   75    /* System Manager stack manager interface ID */
#define LSHIF   76    /* System Agent stack manager interface ID */
#define LMRIF   77    /* Message Router stack manager interface ID */
#define LPRIF   78    /* MPC860SAR driver stack manager interface ID */
#define LGNIF   79    /* network service stack manager interface ID */
#define LGTIF   80    /* GTP stack manager interface ID */
#define LGLIF   81    /* GPRS LLC stack manager interface ID */
#define LGGIF   82    /* GPRS-BSSGP stack manager interface ID */
#define LGSIF   83    /* SNDCP stack manager interface ID */
#define LHCIF   84    /* H.323 Control stack manager interface ID */
#define LHIIF   85    /* TUCL stack manager interface ID */
#define LHRIF   86    /* RTP/RTCP stack manager interface ID */
#define LIQIF   87    /* ISDN-Q.SAAL Convergence Layer stack manager interface ID */
#define LMGIF   88    /* MGCP stack manager interface ID */
#define LDNIF   89    /* MTP-3 LDF stack manager interface ID */
#define LDPIF   90    /* LDF-SCCP stack manager interface ID */
#define LLNIF   91    /* MPLS-LDP(+CR) stack manager interface ID */
#define LPAIF   92    /* PLOA stack manager interface ID */
#define LALIF   93    /* AAL2 Signaling stack manager interface ID */
#define LSBIF   94    /* SCTP stack manager interface ID */
#define LSOIF   95    /* SIP stack manager interface ID */
#define LITIF   96    /* M3UA stack manager interface ID */
#define LNFIF   97    /* M3UA-NIF stack manager interface ID */
#define LIDIF   98    /* IUA stack manager interface ID */
#define LNDIF   99    /* IUA-NIF stack manager interface ID */
#define LSUIF   100   /* SUA stack manager interface ID */
#define LNPIF   101   /* SUA-NIF stack manager interface ID */
#define LRAIF   102   /* RANAP stack manager interface ID */
#define LZGIF   103   /* PSF-GCP (FT/HA) stack manager interface ID */
#define LDGIF   104   /* LDF-GCP stack manager interface ID */
#define LRNIF   105   /* RNSAP stack manager interface ID */
#define LZJIF   106   /* PSF - MAP-GSM stack manager interface ID */
#define LZFIF   107   /* PSF - CAP stack manager interface ID */
#define LGMIF   108   /* GMM/SM stack manager interface ID */
#define LZKIF   109   /* PSF - GMM/SM stack manager interface ID */
/* gen_h_001.main_122 Additions */
#define LAEIF   110   /* PSF - SUA stack manager interface ID */
/* gen_h_001.main_138: Added the interface version for PSF S1AP */
#define LYTIF   111   /* PSF - S1AP stack manager interface ID */
/* gen_h_001.main_141: Added the interface version for PSF DIAMETER */
#define LJCIF   112   /* PSF - DIAMETER stack manager interface ID */

/* Upper and Lower interface identifiers */
#define AALIF    150   /* AAL interface ID */
#define ALTIF    151   /* ALT interface ID */
#define AMTIF    152   /* AMT interface ID */
#define APLIF    153   /* APL interface ID */
#define ARIIF    154   /* ARI interface ID */
#define ASDIF    155   /* ASD interface ID */
#define ASIIF    156   /* ASI interface ID */
#define BITIF    157   /* BIT interface ID */
#define CCTIF    158   /* CCT interface ID */
#define CIPIF    159   /* CIP interface ID */
#define DATIF    160   /* DAT interface ID */
#define ECMIF    161   /* ECM interface ID */
#define EVTIF    162   /* EVT interface ID */
#define FNTIF    163   /* FNT interface ID */
#define FRDIF    164   /* FRD interface ID */
#define GBRIF    165   /* GBR interface ID */
#define GGUIF    166   /* GGU interface ID */
#define GLSIF    167   /* GLS interface ID */
#define GNSIF    168   /* GNS interface ID */
#define GSPIF    169   /* GSP interface ID */
#define HCTIF    170   /* HCT interface ID */
#define HGTIF    171   /* HGT interface ID */
#define HITIF    172   /* HIT interface ID */
#define HRTIF    173   /* HRT interface ID */
#define IATIF    174   /* IAT interface ID */
#define IETIF    175   /* IET interface ID */
#define IFPIF    176   /* IFP interface ID */
#define INTIF    177   /* INT interface ID */
#define MACIF    178   /* MAC interface ID */
#define MATIF    179   /* MAT interface ID */
#define MDPIF    180   /* MDP interface ID */
#define MGTIF    181   /* MGT interface ID */
#define MPAIF    182   /* MPA interface ID */
#define MVTIF    183   /* MVT interface ID */
#define NUIIF    184   /* NUI interface ID */
#define NVTIF    185   /* NVT interface ID */
#define PCIIF    186   /* PCI interface ID */
#define PXYIF    187   /* PXY interface ID */
#define RLSIF    188   /* RLS interface ID */
#define RMTIF    189   /* RMT interface ID */
#define RTTIF    190   /* RTT interface ID */
#define RYTIF    191   /* RYT interface ID */
#define SCTIF    192   /* SCT interface ID */
#define SDTIF    193   /* SDT interface ID */
#define SFTIF    194   /* SFT interface ID */
#define SITIF    195   /* SIT interface ID */
#define SNTIF    196   /* SNT interface ID */
#define SPTIF    197   /* SPT interface ID */
#define STUIF    198   /* STU interface ID */
#define TCTIF    199   /* TCT interface ID */
#define TPTIF    200   /* TPT interface ID */
#define TUTIF    201   /* TUT interface ID */
#define UMEIF    202   /* UME interface ID */
#define VFTIF    203   /* VFT interface ID */
#define WITIF    204   /* WIT interface ID */
#define WNTIF    205   /* WNT interface ID */
#define WUTIF    206   /* WUT interface ID */
#define XMTIF    207   /* XMT interface ID */
#define XNTIF    208   /* XNT interface ID */
#define RPTIF    209   /* RPT interface ID */
#define RNTIF    210   /* RNT interface ID */
#define CAPIF    211   /* CAP interface ID */
#define LFPIF    212   /* LFP interface ID */
#define FPUIF    213   /* FPU interface ID */
#define CMKIF    214   /* CMK interface ID */
#define MKUIF    215   /* MKU interface ID */
#define PHUIF    216   /* PHU interface ID */
#define GMUIF    212   /* GMU interface ID */
/* gen_h_001.main_138: Added the interface version for SZT */
#define SZTIF    213   /* SZT interface ID */
/* gen_h_001.main_140: Added the interface version for AQU */
#define AQUIF    214   /* AQU interface ID */

/* peer interface identifiers */
#define SGPIF   250     /* System manager peer interface ID */
#define SHPIF   251     /* System agent peer interface ID   */
#define MRPIF   252     /* Message router peer interface ID */
#define ZNPIF   253     /* MTP-3 peer interface ID          */
#define ZPPIF   254     /* SCCP peer interface ID           */ 
#define ZTPIF   255     /* TCAP peer interface ID           */
#define ZIPIF   256     /* ISUP peer interface ID           */
#define ZQPIF   257     /* Q.931 peer interface ID          */
#define ZMPIF   258     /* Q.93B peer interface ID          */
#define ZCPIF   259     /* ICC peer interface ID            */
#define ZRPIF   260     /* ICC peer interface ID            */
#define ZSPIF   261     /* RT peer interface ID             */
#define ZAPIF   262     /* RANAP peer interface ID          */
#define ZGPIF   263     /* MGCP peer interface ID           */ 
#define ZBPIF   264     /* GTP peer interface ID            */
#define ZVPIF   265     /* PSF-M3UA peer interface ID       */
#define ZJPIF   266     /* MAP-GSM peer interface ID        */
#define ZFPIF   267     /* CAP peer interface ID            */
#define ZLPIF   268     /* ALCAP peer interface ID          */
#define ZKPIF   269     /* PSF GMMSM peer interface ID      */
/* gen_h_001.main_122 Additions */
#define AEPIF   270     /* PSF SUA peer interface ID        */
/* gen_h_001.main_138: Added the interface version for PSF S1AP */
#define YTPIF   271     /* PSF S1AP peer interface ID       */
/* gen_h_001.main_139: Added the interface version for eGTP-C PSF */
#define HWPIF   272     /* eGTP-C PSF peer interface ID     */
/* gen_h_001.main_141: Added the interface version for PSF DIAMETER */
#define JCPIF   273     /* PSF DIAMETER peer interface ID       */
#endif


/* bind status */
#define CM_BND_OK            1       /* bind request OK */
#define CM_BND_NOK           2       /* bind request not OK */

/* New definition */
#define CM_IPV4ADDR_TYPE      4
#define CM_IPV6ADDR_TYPE      6


/* debug masks and macros */

#ifdef DEBUGP

#define DBGMASK_SI   0x00000001      /* system service interface */
#define DBGMASK_MI   0x00000002      /* layer management interface */
#define DBGMASK_UI   0x00000004      /* upper interface */
#define DBGMASK_LI   0x00000008      /* lower interface */
#define DBGMASK_PI   0x00000010      /* peer interface */
#define DBGMASK_PLI  0x00000020      /* PSF's protocol layer interface */

#define DBGMASK_LYR  0x00000100      /* layer specific */

/* gen_h_001.main_132 : Guarding the Timestamp related changes in the compile time flag*/
#ifdef DBG_TIMESTAMP
/* gen_h_001.main_131 : Added lock related macros */
#define _DBG_INIT_WRITELCK(x)  SInitLock(x, SS_LOCK_MUTEX)
#define _DBG_GET_WRITELCK(x)   SLock(x)
#define _DBG_REL_WRITELCK(x)   SUnlock(x)
#define _DBG_DEL_WRITELCK(x)   SDestroyLock(x)

#define DBGPRNTBUFSIZE 80

/* gen_h_001.main_131 : Modified the DBGP for the timestamp*/
/* gen_h_001.main_133-modified macro */
/* gen_h_001.main_140 : modified macro for printing the ProcId as well
 * and changed sprintf to snprintf */
#define DBGP(_tskInit, _layerName, _msgClass, _arg) \
        { \
           S8 _buf[DBGPRNTBUFSIZE];                    \
           if ((_tskInit)->dbgMask & (_msgClass)) \
           { \
              if((_tskInit)->lyrMtFlag)   \
              { \
                 _DBG_GET_WRITELCK(&((_tskInit)->dbgLock));    \
              } \
              (Void) SGetTimeStamp(_buf);     \
              snprintf((_tskInit)->prntBuf, PRNTSZE, \
                    "[%s] [%s %d:0x%x:%x] %s:%d ", _buf, _layerName, \
                    (_tskInit)->procId, (_tskInit)->ent, (_tskInit)->inst, \
                    __FILE__, __LINE__); \
              SPrint((_tskInit)->prntBuf); \
              sprintf _arg; \
              SPrint((_tskInit)->prntBuf); \
              if((_tskInit)->lyrMtFlag)   \
              { \
                 _DBG_REL_WRITELCK(&((_tskInit)->dbgLock));    \
              } \
           } \
       }
#define DBGPN(_tskInit, _layerName, _msgClass, _arg) \
        { \
           S8 _buf[DBGPRNTBUFSIZE];                    \
           if ((_tskInit)->dbgMask & (_msgClass)) \
           { \
              if((_tskInit)->lyrMtFlag)   \
              { \
                 _DBG_GET_WRITELCK(&((_tskInit)->dbgLock));    \
              } \
              (Void) SGetTimeStamp(_buf);     \
              snprintf((_tskInit)->prntBuf, PRNTSZE, \
                    "[%s] [%s %d:0x%x:%x] %s:%d ", _buf, _layerName, \
                    (_tskInit)->procId, (_tskInit)->ent, (_tskInit)->inst, \
                    __FILE__, __LINE__); \
              SPrint((_tskInit)->prntBuf); \
              snprintf _arg; \
              SPrint((_tskInit)->prntBuf); \
              if((_tskInit)->lyrMtFlag)   \
              { \
                 _DBG_REL_WRITELCK(&((_tskInit)->dbgLock));    \
              } \
           } \
       }
           
#else
#define DBGP(_tskInit, _layerName, _msgClass, _arg) \
        { \
           if ((_tskInit)->dbgMask & (_msgClass)) \
           { \
              snprintf((_tskInit)->prntBuf, PRNTSZE, "[%s %d:0x%x:%x] %s:%d ", \
                    _layerName, (_tskInit)->procId, (_tskInit)->ent, \
                    (_tskInit)->inst,  __FILE__, __LINE__); \
              SPrint((_tskInit)->prntBuf); \
              sprintf _arg; \
              SPrint((_tskInit)->prntBuf); \
           } \
        }
#define DBGPN(_tskInit, _layerName, _msgClass, _arg) \
        { \
           if ((_tskInit)->dbgMask & (_msgClass)) \
           { \
              snprintf((_tskInit)->prntBuf, PRNTSZE, "[%s %d:0x%x:%x] %s:%d ", \
                    _layerName, (_tskInit)->procId, (_tskInit)->ent, \
                    (_tskInit)->inst,  __FILE__, __LINE__); \
              SPrint((_tskInit)->prntBuf); \
              snprintf _arg; \
              SPrint((_tskInit)->prntBuf); \
           } \
        }

#endif
#else

#define DBGP(_tskInit, _layerName, _msgClass, _arg)
#define DBGPN(_tskInit, _layerName, _msgClass, _arg)

#endif /* DEBUGP */


/* gen_h_001.main_126 : Added Circular Buffer */

/*************************   CIRCULAR BUFFER   ************************/
/*
 * Circular Buffer implementation provides two macro's 
 *
 *      1. CBUF_INIT  --> To initialize the Circular Bufffer
 *      2. CBUF_WRITE --> To log data into the circular Buffer
 *
 * Usage:
 *      1. CBUF_INIT must be called from the product's initialization
 *         function, prior to using the maro CBUF_WRITE. 
 *
 *      2. CBUF_WRITE takes variable number of arguments
 *
 *      3. Enable the flag "CBUF_ENABLE"
 *
 * Note:
 *      1. structure "cBuffer" is defined in gen.x
 *      2. The structure cBuffer is added in "TskInit" under the flag
 *         "CBUF_ENABLE"

 *
 * Limitation:
 *      1. CBUF_WRITE can take a maximum of 255 characters, which is the
 *         value of the define PRNTSZE (defined in ssi.h)
 *
 * CBUF_INIT:
 *         This macro, allocates the memory to the circular Buffer and 
 *         initializes the memory region with zeroes.
 *
 *         E.g. CBUF_INIT(xxInit,100);
 *         where xxCb is the task initialisation structure,
 *         100 - is the size of the circular buffer (bytes)
 *
 * CBUF_WRITE:
 *         This Macro, writes the data into circular buffer. Once the
 *         end of the buffer is reached, it starts again from the beggining 
 *         of the circular Buffer.
 *
 *         E.g. CBUF_WRITE(xxInit.cBuf,(xxInit.cBuf.tmpBuf,"%d |",10));
 *         where xxInit is the task initialisation structure
 *         cBuf is the circular buffer's control structure
 *         tmpBuf is the temporary buffer declared inside "cBuffer"
 *
 *         It is suggested to use a delimiter with every CBUF_WRITE. In this
 *         example "|" is the delimiter, which will be helpful in analyzing
 *         the circular buffer contents.
 *
 * CBUF_PRINT:
 *         This macro is used internally by the macro CBUF_WRITE. If the flag
 *         CBUF_DISPLAY is defined, then this macro is defined to SPrint. Else
 *         it does nothing.
 *
 */      

#ifdef CBUF_ENABLE

#define CBUFPRNTSZE 255
#ifdef CBUF_DISPLAY
#define CBUF_PRINT(cBufPtr) SPrint(cBufPtr)
#else
#define CBUF_PRINT(cBufPtr)
#endif

#define CBUF_INIT(_class,size)                                        \
{                                                                     \
   SGetSBuf((_class).region,(_class).pool,                  \
         &(_class).cBuf.cBufPtr,size);                                \
   (_class).cBuf.cBufSize = size;                                     \
   (_class).cBuf.cBufIdx = 0;                                          \
   memset((_class).cBuf.cBufPtr,'|',size);                      \
}

#define CBUF_WRITE(_class,_argList)                                   \
{                                                                     \
   S32 tmpLen = 0;                                                    \
   uint32_t idx;                                                            \
   tmpLen = sprintf _argList;                                         \
   if(CBUFPRNTSZE >= tmpLen)                                              \
   {                                                                  \
      for(idx = 0; idx < tmpLen; idx++)                               \
      {                                                               \
         (_class).cBufIdx = (_class).cBufIdx % (_class).cBufSize;     \
         (_class).cBufPtr[(_class).cBufIdx++] = (_class).tmpBuf[idx]; \
      }                                                               \
   }                                                                  \
   CBUF_PRINT((_class).tmpBuf);                                       \
}

#define CBUF_DATA_PRINT(_class)                                       \
{                                                                     \
    S8 *tmpBuf = NULLP ;\
    uint32_t cBufIdx;                                                             \
    uint32_t tmpIdx=0;                                                           \
    SGetSBuf((_class).region,(_class).pool,                  \
         (uint8_t **)&tmpBuf,(_class).cBuf.cBufSize);                                \
    memset(tmpBuf,0,(_class).cBuf.cBufSize);                      \
    for(cBufIdx = (_class).cBuf.cBufIdx; cBufIdx < (_class).cBuf.cBufSize; cBufIdx++)            \
    {                                                                 \
       tmpBuf[tmpIdx++] = (_class).cBuf.cBufPtr[cBufIdx];                             \
       tmpIdx = tmpIdx % ((_class).cBuf.cBufSize); \
    }                                                                 \
    for(cBufIdx=0;cBufIdx < (_class).cBuf.cBufIdx;cBufIdx++)                             \
    {                                                                 \
       tmpBuf[tmpIdx++] = (_class).cBuf.cBufPtr[cBufIdx];                             \
       tmpIdx = tmpIdx % ((_class).cBuf.cBufSize); \
    }                                                                 \
   SPrint(tmpBuf); \
   SPutSBuf((_class).region,(_class).pool,                            \
         (uint8_t *)tmpBuf,(_class).cBuf.cBufSize);                        \
}
#else

#define CBUF_INIT(_class,size)
#define CBUF_WRITE(_class,_argList)
#define CBUF_DATA_PRINT(_class)
#endif /* CBUF_ENABLE */

  

#ifdef CMFILE_REORG_1

/* defines */

/* packing macros */

/* system services typedefs */

#define oduPackBool(x, mBuf)    oduUnpackUInt8(x, mBuf)       /* pack Bool */
#define cmPkStatus(x, mBuf)     SPkS16(x, mBuf)      /* pack Status */
#define cmPkTicks(x, mBuf)      oduUnpackUInt32(x, mBuf)      /* pack Ticks */
#define cmPkQLen(x, mBuf)       SPkS16(x, mBuf)      /* pack QLen */
#define cmPkOrder(x, mBuf)      SPkS16(x, mBuf)      /* pack Order */
#define cmPkData(x, mBuf)       oduUnpackUInt8(x, mBuf)       /* pack Data */
#define cmPkRandom(x, mBuf)     oduUnpackUInt16(x, mBuf)      /* pack Random */
#define cmPkSeq(x, mBuf)        SPkS16(x, mBuf)      /* pack Seq */
#define cmPkReason(x, mBuf)     SPkS16(x, mBuf)      /* pack Reason */
#define cmPkProcId(x, mBuf)     oduUnpackUInt16(x, mBuf)      /* pack ProcId */
#define cmPkVectNmb(x, mBuf)    SPkS16(x, mBuf)      /* pack VectNmb */
#define cmPkPrior(x, mBuf)      oduUnpackUInt8(x, mBuf)       /* pack Prior*/
#define cmPkPriority(x, mBuf)   cmPkPrior(x, mBuf)   /* pack Priority */
#define cmPkRoute(x, mBuf)      oduUnpackUInt8(x, mBuf)       /* pack Route */
#define cmPkTtype(x, mBuf)      SPkS16(x, mBuf)      /* pack Ttype */
#define cmPkSema(x, mBuf)       SPkS8(x, mBuf)       /* pack Sema */
#define cmPkTxt(x, mBuf)        SPkS8(x, mBuf)       /* pack Txt */
#define cmPkEnt(x, mBuf)        oduUnpackUInt8(x, mBuf)       /* pack Ent */
#define cmPkInst(x, mBuf)       oduUnpackUInt8(x, mBuf)       /* pack Inst */
#define cmPkElmnt(x, mBuf)      SPkS16(x, mBuf)      /* pack Elmnt */
#define cmPkElmntInst1(x, mBuf) SPkS16(x, mBuf)      /* pack ElmntInst1 */
#define cmPkElmntInst2(x, mBuf) SPkS16(x, mBuf)      /* pack ElmntInst2 */
#define cmPkElmntInst3(x, mBuf) SPkS16(x, mBuf)      /* pack ElmntInst3 */
#define cmPkRegion(x, mBuf)     oduUnpackUInt8(x, mBuf)       /* pack Region */
#define cmPkPool(x, mBuf)       oduUnpackUInt8(x, mBuf)       /* pack Pool */
#ifdef LONG_MSG
#define cmPkMsgLen(x, mBuf)     SPkS32(x, mBuf)      /* pack MsgLen */
#else
#define cmPkMsgLen(x, mBuf)     SPkS16(x, mBuf)      /* pack MsgLen */
#endif
#ifdef DOS
#define cmPkSize(x, mBuf)       oduUnpackUInt16(x, mBuf)      /* pack Size */
#else
#define cmPkSize(x, mBuf)       oduUnpackUInt32(x, mBuf)      /* pack Size */
#endif /* DOS */

/* general typedefs */

#define cmPkSelector(x, mBuf)   oduUnpackUInt8(x, mBuf)       /* pack Selector */
#define cmPkEvent(x, mBuf)      oduUnpackUInt8(x, mBuf)       /* pack Event */
#define cmPkCntr(x, mBuf)       SPkS32(x, mBuf)      /* pack Cntr */
#define cmPkStsCntr(x, mBuf)    oduUnpackUInt32(x, mBuf)      /* pack StsCntr */
#define cmPkLnkNmb(x, mBuf)     SPkS16(x, mBuf)      /* pack LnkNmb */
#define cmPkSuId(x, mBuf)       SPkS16(x, mBuf)      /* pack SuId */
#define cmPkSpId(x, mBuf)       SPkS16(x, mBuf)      /* pack SpId */
#define cmPkSuInstId(x, mBuf)   SPkS16(x, mBuf)      /* pack SuInstId */
#define cmPkSpInstId(x, mBuf)   SPkS16(x, mBuf)      /* pack SpInstId */
#define cmPkSapi(x, mBuf)       oduUnpackUInt8(x, mBuf)       /* pack Sapi */
#define cmPkTei(x, mBuf)        oduUnpackUInt8(x, mBuf)       /* pack Tei */
#define cmPkchNo(x, mBuf)       oduUnpackUInt8(x, mBuf)       /* pack Channel Number */
#define cmPkCes(x, mBuf)        oduUnpackUInt8(x, mBuf)       /* pack Ces */
#define cmPkDlci(x, mBuf)       oduUnpackUInt32(x, mBuf)      /* pack Dlci */
#define cmPkCalRef(x, mBuf)     oduUnpackUInt16(x, mBuf)      /* pack CalRef */
#define cmPkOrigin(x, mBuf)     SPkS16(x, mBuf)      /* pack Origin */
#define cmPkNwId(x, mBuf)       oduUnpackUInt16(x, mBuf)      /* pack NwId */
#define cmPkSwtch(x, mBuf)      SPkS16(x, mBuf)      /* pack Swtch */
#define cmPkCause(x, mBuf)      oduUnpackUInt8(x, mBuf)       /* pack Cause */
#define cmPkDgn(x, mBuf)        oduUnpackUInt8(x, mBuf)       /* pack Dgn */
#define cmPkAction(x, mBuf)     SPkS16(x, mBuf)      /* pack Action */
#define cmPkSeqS16(x, mBuf)     SPkS16(x, mBuf)      /* pack SeqS16 */
#define cmPkSeqUInt16(x, mBuf)  oduUnpackUInt16(x, mBuf)      /* pack SeqUInt16 */
#define cmPkSeqS24(x, mBuf)     SPkS32(x, mBuf)      /* pack SeqS24 */
#define cmPkSeqUInt24(x, mBuf)  oduUnpackUInt32(x, mBuf)      /* pack SeqUInt24 */
#define cmPkSetUpArb(x, mBuf)   oduUnpackUInt8(x, mBuf)       /* pack SetUpArb */
#define cmPkEvntType(x, mBuf)   oduUnpackUInt8(x, mBuf)       /* pack EvntType */
#define cmPkState(x, mBuf)      oduUnpackUInt8(x, mBuf)       /* pack State */
#define cmPkMode(x, mBuf)       oduUnpackUInt8(x, mBuf)       /* pack Mode */
#define cmPkConnId(x, mBuf)     SPkS32(x, mBuf)      /* pack ConnId */
#define cmPkUConnId(x, mBuf)    oduUnpackUInt32(x, mBuf)      /* pack unsigned ConnId */
#define cmPkMibOpCode(x, mBuf)  oduUnpackUInt16(x, mBuf)      /* pack mib opCode*/
#define cmPkMibStatus(x, mBuf)  oduUnpackUInt16(x, mBuf)      /* Pack mib cfm status*/
#define cmPkMibTblType(x, mBuf) oduUnpackUInt16(x, mBuf)      /* Pack mib table type */
#define cmPkMibReqId(x, mBuf)   SPkS32(x, mBuf)      /* Pack mib request Id */
#define cmPkProfileId(x, mBuf)  oduUnpackUInt8(x, mBuf)       /* Pack HCC Profile Id */
#define cmPkIntfId(x, mBuf)     oduUnpackUInt16(x, mBuf)      /* Pack intf id  */
#define cmPkIntfVer(x, mBuf)    oduUnpackUInt16(x, mBuf)      /* Pack intf ver */
#define cmPkuaType(x, mBuf)     oduUnpackUInt8(x, mBuf)      /* Pack uaType */
/* scc specific typedefs */

#define cmPkBaud(x, mBuf)           SPkS16(x, mBuf)  /* pack Baud */
#define cmPkPhysIntType(x, mBuf)    SPkS16(x, mBuf)  /* pack PhysIntType */
#define cmPkPathConnType(x, mBuf)   SPkS16(x, mBuf)  /* pack PathConnType */
#define cmPkParity(x, mBuf)         SPkS16(x, mBuf)  /* pack Parity */
#define cmPkCharLength(x, mBuf)     SPkS16(x, mBuf)  /* pack CharLength */
#define cmPkStopBits(x, mBuf)       SPkS16(x, mBuf)  /* pack StopBits */

/* tcp/ip specific typedefs */

#define cmPkIpAddr(x, mBuf)     oduUnpackUInt32(x, mBuf)      /* pack IpAddr */
#define cmPkPort(x, mBuf)       oduUnpackUInt16(x, mBuf)      /* pack Port */
#define cmPkCmd(x, mBuf)        oduUnpackUInt8(x, mBuf)       /* pack Cmd */
#define cmPkFlags(x, mBuf)      oduUnpackUInt8(x, mBuf)       /* pack Flags */
#define cmPkTtl(x, mBuf)        oduUnpackUInt8(x, mBuf)       /* pack Ttl */
#define cmPkPrec(x, mBuf)       oduUnpackUInt8(x, mBuf)       /* pack Prec */
#define cmPkWindow(x, mBuf)     oduUnpackUInt32(x, mBuf)      /* pack Window */

/* LLC/SNAP specific defines */

#define cmPkOui(x, mBuf)        oduUnpackUInt32(x, mBuf)       /* pack Oui */
#define cmPkPid(x, mBuf)        oduUnpackUInt16(x, mBuf)       /* pack Pid */
#define cmPkLlcId(x, mBuf)      oduUnpackUInt32(x, mBuf)       /* pack LLcId */


/* unpacking macros */

/* system services typedefs */

#define oduUnpackBool(x, mBuf)    oduPackUInt8(x, mBuf)     /* unpack Bool */
#define cmUnpkStatus(x, mBuf)     SUnpkS16(x, mBuf)    /* unpack Status */
#define cmUnpkTicks(x, mBuf)      oduPackUInt32(x, mBuf)    /* unpack Ticks */
#define cmUnpkQLen(x, mBuf)       SUnpkS16(x, mBuf)    /* unpack QLen */
#define cmUnpkOrder(x, mBuf)      SUnpkS16(x, mBuf)    /* unpack Order */
#define cmUnpkData(x, mBuf)       oduPackUInt8(x, mBuf)     /* unpack Data */
#define cmUnpkRandom(x, mBuf)     oduPackUInt16(x, mBuf)    /* unpack Random */
#define cmUnpkSeq(x, mBuf)        SUnpkS16(x, mBuf)    /* unpack Seq */
#define cmUnpkReason(x, mBuf)     SUnpkS16(x, mBuf)    /* unpack Reason */
#define cmUnpkProcId(x, mBuf)     oduPackUInt16(x, mBuf)    /* unpack ProcId */
#define cmUnpkVectNmb(x, mBuf)    SUnpkS16(x, mBuf)    /* unpack VectNmb */
#define cmUnpkPrior(x, mBuf)      oduPackUInt8(x, mBuf)     /* unpack Prior */
#define cmUnpkPriority(x, mBuf)   cmUnpkPrior(x, mBuf) /* unpack Priority */
#define cmUnpkRoute(x, mBuf)      oduPackUInt8(x, mBuf)     /* unpack Route */
#define cmUnpkTtype(x, mBuf)      SUnpkS16(x, mBuf)    /* unpack Ttype */
#define cmUnpkSema(x, mBuf)       SUnpkS8(x, mBuf)     /* unpack Sema */
#define cmUnpkTxt(x, mBuf)        SUnpkS8(x, mBuf)     /* unpack Txt */
#define cmUnpkEnt(x, mBuf)        oduPackUInt8(x, mBuf)     /* unpack Ent */
#define cmUnpkInst(x, mBuf)       oduPackUInt8(x, mBuf)     /* unpack Inst */
#define cmUnpkElmnt(x, mBuf)      SUnpkS16(x, mBuf)    /* unpack Elmnt */
#define cmUnpkElmntInst1(x, mBuf) SUnpkS16(x, mBuf)    /* unpack ElmntInst1 */
#define cmUnpkElmntInst2(x, mBuf) SUnpkS16(x, mBuf)    /* unpack ElmntInst2 */
#define cmUnpkElmntInst3(x, mBuf) SUnpkS16(x, mBuf)    /* unpack ElmntInst3 */
#define cmUnpkRegion(x, mBuf)     oduPackUInt8(x, mBuf)     /* unpack Region */
#define cmUnpkPool(x, mBuf)       oduPackUInt8(x, mBuf)     /* unpack Pool */
#ifdef LONG_MSG
#define cmUnpkMsgLen(x, mBuf)     SUnpkS32(x, mBuf)    /* unpack MsgLen */
#else
#define cmUnpkMsgLen(x, mBuf)     SUnpkS16(x, mBuf)    /* unpack MsgLen */
#endif
#ifdef DOS
#define cmUnpkSize(x, mBuf)       oduPackUInt16(x, mBuf)    /* unpack Size */
#else
#define cmUnpkSize(x, mBuf)       oduPackUInt32(x, mBuf)    /* unpack Size */
#endif /* DOS */

/* general typedefs */

#define cmUnpkSelector(x, mBuf)   oduPackUInt8(x, mBuf)   /* unpack Selector */
#define cmUnpkEvent(x, mBuf)      oduPackUInt8(x, mBuf)   /* unpack Event */
#define cmUnpkCntr(x, mBuf)       SUnpkS32(x, mBuf)  /* unpack Cntr */
#define cmUnpkStsCntr(x, mBuf)    oduPackUInt32(x, mBuf)  /* unpack StsCntr */
#define cmUnpkLnkNmb(x, mBuf)     SUnpkS16(x, mBuf)  /* unpack LnkNmb */
#define cmUnpkSuId(x, mBuf)       SUnpkS16(x, mBuf)  /* unpack SuId */
#define cmUnpkSpId(x, mBuf)       SUnpkS16(x, mBuf)  /* unpack SpId */
#define cmUnpkSuInstId(x, mBuf)   SUnpkS16(x, mBuf)  /* unpack SuInstId */
#define cmUnpkSpInstId(x, mBuf)   SUnpkS16(x, mBuf)  /* unpack SpInstId */
#define cmUnpkSapi(x, mBuf)       oduPackUInt8(x, mBuf)   /* unpack Sapi */
#define cmUnpkTei(x, mBuf)        oduPackUInt8(x, mBuf)   /* unpack Tei */
#define cmUnpkchNo(x, mBuf)       SUnpkS8(x, mBuf)   /* unpack Channel Number */
#define cmUnpkCes(x, mBuf)        oduPackUInt8(x, mBuf)   /* unpack Ces */
#define cmUnpkDlci(x, mBuf)       oduPackUInt32(x, mBuf)  /* unpack Dlci */
#define cmUnpkCalRef(x, mBuf)     oduPackUInt16(x, mBuf)  /* unpack CalRef */
#define cmUnpkOrigin(x, mBuf)     SUnpkS16(x, mBuf)  /* unpack Origin */
#define cmUnpkNwId(x, mBuf)       oduPackUInt16(x, mBuf)  /* unpack NwId */
#define cmUnpkSwtch(x, mBuf)      SUnpkS16(x, mBuf)  /* unpack Swtch */
#define cmUnpkCause(x, mBuf)      oduPackUInt8(x, mBuf)   /* unpack Cause */
#define cmUnpkDgn(x, mBuf)        oduPackUInt8(x, mBuf)   /* unpack Dgn */
#define cmUnpkAction(x, mBuf)     SUnpkS16(x, mBuf)  /* unpack Action */
#define cmUnpkSeqS16(x, mBuf)     SUnpkS16(x, mBuf)  /* unpack SeqS16 */
#define cmUnpkSeqUInt16(x, mBuf)  oduPackUInt16(x, mBuf)  /* unpack SeqUInt16 */
#define cmUnpkSeqS24(x, mBuf)     SUnpkS32(x, mBuf)  /* unpack SeqS24 */
#define cmUnpkSeqUInt24(x, mBuf)  oduPackUInt32(x, mBuf)  /* unpack SeqUInt24 */
#define cmUnpkSetUpArb(x, mBuf)   oduPackUInt8(x, mBuf)   /* unpack SetUpArb */
#define cmUnpkEvntType(x, mBuf)   oduPackUInt8(x, mBuf)   /* unpack EvntType */
#define cmUnpkState(x, mBuf)      oduPackUInt8(x, mBuf)   /* unpack State */
#define cmUnpkMode(x, mBuf)       oduPackUInt8(x, mBuf)   /* unpack Mode */
#define cmUnpkConnId(x, mBuf)     SUnpkS32(x, mBuf)  /* unpack ConnId */
#define cmUnpkUConnId(x, mBuf)    oduPackUInt32(x, mBuf)  /* unpack uns ConnId */
#define cmUnpkMibOpCode(x, mBuf)  oduPackUInt16(x, mBuf)  /* unpack mib opCode*/
#define cmUnpkMibStatus(x, mBuf)  oduPackUInt16(x, mBuf)  /* unpack mib cfm status*/
#define cmUnpkMibTblType(x, mBuf) oduPackUInt16(x, mBuf)  /* Pack mib table type */
#define cmUnpkMibReqId(x, mBuf)   SUnpkS32(x, mBuf)  /* Pack mib request Id */
#define cmUnpkProfileId(x, mBuf)  oduPackUInt8(x, mBuf)   /* Pack HCC Profile Id */
#define cmUnpkIntfId(x, mBuf)     oduPackUInt16(x, mBuf)      /* unpack intf id  */
#define cmUnpkIntfVer(x, mBuf)    oduPackUInt16(x, mBuf)      /* unpack intf ver */
#define cmUnpkuaType(x, mBuf)     oduPackUInt8(x, mBuf)       /* Unpack uaType */
/* scc specific typedefs */

#define cmUnpkBaud(x, mBuf)           SUnpkS16(x, mBuf)  /* unpack Baud */
#define cmUnpkPhysIntType(x, mBuf)    SUnpkS16(x, mBuf)  /* unpack PhysIntType */
#define cmUnpkPathConnType(x, mBuf)   SUnpkS16(x, mBuf)  /* unpack PathConnType */
#define cmUnpkParity(x, mBuf)         SUnpkS16(x, mBuf)  /* unpack Parity */
#define cmUnpkCharLength(x, mBuf)     SUnpkS16(x, mBuf)  /* unpack CharLength */
#define cmUnpkStopBits(x, mBuf)       SUnpkS16(x, mBuf)  /* unpack StopBits */

/* tcp/ip specific typedefs */

#define cmUnpkIpAddr(x, mBuf)     oduPackUInt32(x, mBuf)      /* unpack IpAddr */
#define cmUnpkPort(x, mBuf)       oduPackUInt16(x, mBuf)      /* unpack Port */
#define cmUnpkCmd(x, mBuf)        oduPackUInt8(x, mBuf)       /* unpack Cmd */
#define cmUnpkFlags(x, mBuf)      oduPackUInt8(x, mBuf)       /* unpack Flags */
#define cmUnpkTtl(x, mBuf)        oduPackUInt8(x, mBuf)       /* unpack Ttl */
#define cmUnpkPrec(x, mBuf)       oduPackUInt8(x, mBuf)       /* unpack Prec */
#define cmUnpkWindow(x, mBuf)     oduPackUInt32(x, mBuf)      /* unpack Window */

/* LLC/SNAP specific defines */

#define cmUnpkOui(x, mBuf)         oduPackUInt32(x, mBuf)  /* unpack Oui */
#define cmUnpkPid(x, mBuf)         oduPackUInt16(x, mBuf)  /* unpack Pid */
#define cmUnpkLlcId(x, mBuf)       oduPackUInt32(x, mBuf)  /* unpack LLcId */

/* packing and unpacking for token strings */

#define CMPKTKNSTR(tknStr, mBuf)                   \
{                                                  \
   Cntr i;                                         \
                                                   \
   if(tknStr->pres)                                \
   {                                               \
      /* Value */                                  \
      for (i = 0; i < (S16) tknStr->len; i++)      \
      {                                            \
         CMCHKPK(oduUnpackUInt8, tknStr->val[i], mBuf);     \
      }                                            \
                                                   \
      /* Length */                                 \
      CMCHKPK(oduUnpackUInt8, tknStr->len, mBuf);           \
   }                                               \
                                                   \
   /* Token Header */                              \
   CMCHKPK(oduUnpackUInt8, tknStr->pres, mBuf);             \
}

#define CMUNPKTKNSTR(tknStr, mBuf)                                 \
{                                                                  \
   Cntr i;                                                         \
                                                                   \
   /* Token Header */                                              \
   CMCHKUNPK(oduPackUInt8, &tknStr->pres, mBuf);                        \
                                                                   \
   if(tknStr->pres)                                                \
   {                                                               \
      /* Length */                                                 \
      CMCHKUNPK(oduPackUInt8, &tknStr->len, mBuf);                      \
                                                                   \
      /* Value */                                                  \
      for (i = 1; i <= (S16) tknStr->len; i++)                     \
      {                                                            \
         CMCHKUNPK(oduPackUInt8, &tknStr->val[tknStr->len - i], mBuf);  \
      }                                                            \
   }                                                               \
                                                                   \
}

#endif /* CMFILE_REORG_1 */

#define cmPkTranId(x, mBuf)    oduUnpackUInt32(x, mBuf)      /* Pack transaction Id */
#define cmUnpkTranId(x, mBuf)  oduPackUInt32(x, mBuf)  /* unpack transaction Id */

#ifdef L2_L3_SPLIT
typedef struct _debugInfo
{
   uint32_t   mBuf;
   uint32_t   res[8];
   uint32_t   count;
} DebugInfo;

DebugInfo debugInfo;
#endif

#endif /* __GENH__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
