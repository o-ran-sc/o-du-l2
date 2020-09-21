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
  
     Name:     system services interface
  
     Type:     C include file
  
     Desc:     Defines required by the system services service user.
  
     File:     ssi.h
  
*********************************************************************21*/
  
#ifndef __SSIH__
#define __SSIH__


/* defines */
  
/* order for SCatQueue */
  
#define Q1Q2        0                 /* Q1 in front of Q2 into Q1 */
#define Q2Q1        1                 /* Q2 in front of Q1 into Q1 */
  
/* order for SCatMsg */
  
#define M1M2        0                 /* M1 in front of M2 into M1 */
#define M2M1        1                 /* M2 in front of M1 into M1 */
  
/* period for SRegTmr */
  
#ifdef WINNT_IATM
#define PONE        1000              /* number of ticks to a second */
#else
#define PTENTH      1                 /* tenth of second */
#define PONE       10                 /* one second */
#define PTEN      100                 /* ten seconds */
#define PHUND    1000                 /* one hundred seconds */
#endif /* WINNT_IATM */  

/* reason for SActvInit and logRstHlt function */
  
#define NRM_TERM    0                 /* normal termination */
#define PWR_UP      1                 /* power up */
#define SWITCH      2                 /* switch depressed */
#define SW_ERROR    3                 /* software error */
#define DMT_FIRED   4                 /* deadman timer fired */
#define EXTERNAL    5                 /* external, another board */
#define SHUTDOWN    6                 /* shutdown interrupt */
  
/* To indicate how the data is to be attached */
#define PREBUF         0    /* pre buffer */
#define PSTBUF         1    /* post buffer */

/* return codes for all functions */
  
#define ROK         0                 /* ok */
#define RFAILED     1                 /* failed, general */
#define ROKDNA      2                 /* ok, data not available */
#define RNA         3                 /* failed, not available */
#define ROUTRES     4                 /* failed, out of resources */
#define ROKIGNORE   5                 /* ok, ignore */
#define RIGNORE     6                 /* failed, ignore */
#define ROKCRSIG    7                 /* ok, connect request (signal) */
#define ROKCRPRM    8                 /* ok, connect request (primitive) */
#define ROKDRSIG    9                 /* ok, disconnect request (signal) */
#define ROKDRPRM    10                /* ok, disconnect request (primitive) */
#define ROKDCPRM    11                /* ok, disconnect confirm (primitive) */
#define ROKDUP      12                /* ok, duplicate entry found */
#define RSKIP       13                /* ok, skip the token */
#define ROKDRPRMSP  14                /* ok, disconnect request (primitive)
                                       * with service provider conn. id.
                                       */
#define ROKDRPRMSU  15                /* ok, disconnect request (primitive)
                                       * with service user conn. id.
                                       */
#define ROKPENDING  16                /* OK - operation still pending */
#define RLONG       17                /* long */

#define RINPROGRESS 18                /* connection in progress: cm_inet */
#define RWOULDBLOCK 19                /* would block:   cm_inet */
#define RCLOSED     20                /* socket closed: cm_inet */
#define RTIMEOUT    21                /* timeout:       cm_inet */
#define RISCONN     22                /* connected :    cm_inet */
/* new return code added for rolling upgrade */
#define RINVIFVER   23                /* invalid interface version */
/* ssi_h_001.main_121 - addition of new return types for double free and trampling detection */
#if (defined( SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1))
#define RDBLFREE      24              /* double deallocation : cm_mem */
#define RTRAMPLINGOK  25              /* memory trample - not serious : cm_mem */
#define RTRAMPLINGNOK 26              /* memory trample -serious : cm_mem */
#endif /* SSI_DEBUG_LEVEL1 */

/* print buffer size */

#define PRNTSZE     255               /* print buffer size */

/* end of file */

#ifndef EOF
#define EOF         (-1)              /* end of file */
#endif

/* static buffer size calculations */

#ifdef MOSVER                         /* mos version */
#define MINBUFSIZE      (PTRALIGN(sizeof(Chain))+PTRALIGN(sizeof(MemoryId))+PTRALIGN(sizeof(S8))+PTRALIGN(sizeof(Size)))
#define SBUFSIZE(size)  (size < (Size) MINBUFSIZE ? (Size) MINBUFSIZE : PTRALIGN(size))
#else
#ifdef MSVER2                         /* mos version 2 */
#define MINBUFSIZE      (PTRALIGN(sizeof(SBuf)))
#define SBUFSIZE(size)  (size < (Size) MINBUFSIZE ? (Size) MINBUFSIZE : (Size)PTRALIGN(size))
#define SLockId           U8     /* lock type */
#define SInitLock(l, t)   ((*l) = ROK)
#define SLock(l)          ((*l) = ROK)
#define SUnlock(l)        ((*l) = ROK)
#define SDestroyLock(l)   ((*l) = ROK)
#else                                 /* default version */
#ifdef PS 
#define UNIT_HEAP_SIZE 32
#define SBUFSIZE(size)  (((size/UNIT_HEAP_SIZE) + 1) * UNIT_HEAP_SIZE)
#else
#ifdef MT                         /* multi-threaded */
#define MINBUFSIZE      sizeof(SBuf)
#define SBUFSIZE(size)  (size < (Size) MINBUFSIZE ? (Size) MINBUFSIZE : (Size)size)
#else
#define SBUFSIZE(size)  (size)
#endif /* MT */
#endif /* PS */
#endif /* MSVER2 */
#endif /* MOSVER */

/* maximum value for system timer used for wrap around counter */

/* note:  this value must agree with the current typedef for Ticks
   or timer wraparound and statistics based on duration may not work */

#define MAXTICKS   0xffffffff        /* maximum value for Tick */


/* error codes */

/* base plus offset is passed as first parameter to sError function */
/* ERRZERO is passed as second parameter to sError function if no */
/* additional information is available */  
  
#define ERRZERO        0            /* no additional information */


/* base error codes */

#define  ERRSS          0           /* system services */
#define  ERRLM        500           /* layer management */
#define  ERRAP       1000           /* async pad */
#define  ERRXN       2000           /* x.25/x.75 */
#define  ERRFM       3000           /* SS7 - Fault Manager*/
#define  ERRVX       3200           /* vrtxsa */
#define  ERRPS       3300           /* psos */
#define  ERRVW       3400           /* vxworks */
#define  ERRHE       3600           /* hello world */
#define  ERRWN       3600           /* MTP 3 Wrapper */
#define  ERRQI       3700           /* QUICC driver */
#define  ERRWI       3800           /* ISUP Wrapper */
#define  ERRWU       3900           /* TUP Wrapper */
#define  ERRLB       4000           /* lapb */
#define  ERRBD       4000           /* lapb, lapd, lapb/lapd */
#define  ERRSC       5000           /* scc */
#define  ERRMS       5500           /* mos */
#define  ERRSM       6500           /* stack manager - see below */
#define  ERRLL       8500           /* logical link control (obsolete) */
#define  ERRBI       8500           /* b-isup */
#define  ERRTC       8500           /* TCP/IP (obsolete) */
#define  ERRIX       9000           /* x.31 */
#define  ERRMF       9500           /* message functions */
#define  ERRAR       9600           /* arp (obsolete) */
#define  ERRMA       9600           /* ss7 - map */
#define  ERROD      10600           /* ODI mac */
#define  ERRSE      10700           /* sec */
#define  ERRSI      11000           /* ss7 - isup */
#define  ERRSN      12000           /* ss7 - mtp, level 3 */
#define  ERRSD      13000           /* ss7 - mtp, level 2 */
#define  ERRSA      13500           /* q.2140 */
#define  ERRIN      14000           /* q.930/q.931 */
#define  ERRIS      15000           /* siemens isac-s */
#define  ERRLC      15500           /* llc */
#define  ERRTP      16000           /* ss7 - tup */
#define  ERRCM      17000           /* common files */
#define  ERRST      18000           /* ss7 - tcap */
#define  ERRFN      18500           /* FN - Q.933 Frame Relay Network Layer */
#define  ERRSP      19000           /* ss7 - sccp */
#define  ERRME      20000           /* message exchange*/
#define  ERREV      20100           /* V5.1 Envelope Function */
#define  ERRNV      20500           /* V5.1 PSTN */
#define  ERRLV      20300           /* LAPV5-DL */
#define  ERRAC      21000           /* atm - cpcs */
#define  ERRAS      22000           /* atm - q.saal */
#define  ERRAM      23000           /* atm - q.93b */
#define  ERRM6      24000           /* dummy mac (MACINT6) */
#define  ERRPN      24500           /* PNNI */
#define  ERRUM      25000           /* atm - ume */
#define  ERRMU      26000           /* atm - management user */
#define  ERRFR      26100           /* Q.922 - frame data link */
#define  ERRMPOFR   26500           /* MPOFR - library error defines */
#define  ERRLR      27000           /* loader */
#define  ERRLU      28000           /* loader service user */
#define  ERRRY      29000           /* relay */
#define  ERROS      30000           /* OS library */
#define  ERREC      31000           /* ATM - LANE Client (LEC) */
#define  ERRES      31500           /* ATM - LANE Services (LEServices) */
#define  ERRTU      32000           /* TCAP - User */
#define  ERRFA      32200           /* ATM - Fujitsu ALC driver */
#define  ERRSR      32500           /* MTP Level 3 Simple Router */
#define  ERRPL      32700           /* ATM - PMC Sierra LASAR driver */
#define  ERRAF      33000           /* FR-ATM Interworking Layer */
#define  ERRCV      34000           /* Control Protocol */
#define  ERRMV      34200           /* V5.X System Manager layer */
#define  ERRCMDRV   34400           /* Common Driver */
#define  ERRPQ      34500           /* Power Quicc */
#define  ERRGL      34600           /* GPRS Logical Link Control */
#define  ERRVO      34700           /* Power Quicc II MPC8260 */
#define  ERRZP      34800           /* PSF - SCCP */
#define  ERRP1      34900           /* PLOA PXY dummy */
#define  ERRIB      35000           /* 3GPP NBAP  */
#define  ERRRN      35100           /* RNSAP  */
#define  ERRVU      35300           /* End Driver  */
#define  ERRFP      35500           /* Fraiming Protocol  */

/* Nuera PS Entries for M1UA */
#define  ERRTX      35700           /* Threadx SSI */
#define  ERRMZ      35900           /* M1UA */
#define  ERRNEXT    ERRMZ + 200     /* Modify it to indicate how much 
                                       you'll be using */
#define ERRD2        4000           /*DPNSS/DASS2*/


/* note: release 1.6 and earlier of q.930/q.931 used base code 1500 */

/* 
 * management interface error code bases
 *
 * ERRSM = 6500
 *
 * Each layer that uses this file need to reserve a range of errnos.
 * These errnos are used in stack management packing/unpaking routines, 
 * and also in primitives terminated by the stack manager.  
 *
 * The next base of error codes is ERRLL = 8500, so we have a range
 * of 2000 error numbers. By allocating 50 error codes to each layer,
 * we are restricting the stack manager to have no more than 39 layer
 * interfaces.
 *
 */

#define  ERRSMST   (ERRSM + 50)    /* TCAP:   6550 - 6599 */
#define  ERRSMSP   (ERRSM + 100)   /* SCCP:   6600 - 6649 */
#define  ERRSMSN   (ERRSM + 150)   /* MTP3:   6650 - 6699 */
#define  ERRSMSD   (ERRSM + 200)   /* MTP2:   6700 - 6724 */
#define  ERRSMSA   (ERRSM + 225)   /* Q.2140: 6725 - 6749 */
#define  ERRSMAM   (ERRSM + 250)   /* Q.93B:  6750 - 6799 */
#define  ERRSMAS   (ERRSM + 300)   /* Q.SAAL: 6800 - 6849 */
#define  ERRSMUM   (ERRSM + 350)   /* UME:    6850 - 6899 */
#define  ERRSMM6   (ERRSM + 400)   /* M6:     6900 - 6949 */
#define  ERRSMMS   (ERRSM + 450)   /* MOS:    6950 - 6999 */
#define  ERRSMEC   (ERRSM + 500)   /* LEC:    7000 - 7049 */
#define  ERRSMSI   (ERRSM + 550)   /* ISUP:   7050 - 7074 */
#define  ERRSMBI   (ERRSM + 575)   /* BI:     7075 - 7099 */
#define  ERRSMTU   (ERRSM + 600)   /* TC-User:7100 - 7149 */
#define  ERRSMFA   (ERRSM + 650)   /* Fuj.ALC:7150 - 7199 */
#define  ERRSMSE   (ERRSM + 700)   /* SE:     7200 - 7249 */
#define  ERRSMSR   (ERRSM + 750)   /* SR:     7250 - 7299 */
#define  ERRSMRY   (ERRSM + 800)   /* RY:     7300 - 7349 */
#define  ERRSMTP   (ERRSM + 850)   /* TP:     7350 - 7399 */
#define  ERRSMES   (ERRSM + 900)   /* LEServices:7400 - 7449 */
#define  ERRSMIN   (ERRSM + 950)   /* IN:     7450 - 7499 */
#define  ERRSMLB   (ERRSM + 1000)  /* LB:     7500 - 7549 */
#define  ERRSMLD   (ERRSM + 1050)  /* LD:     7550 - 7599 */
#define  ERRSMBD   (ERRSM + 1100)  /* BD:     7600 - 7649 */
#define  ERRSMBR   (ERRSM + 1150)  /* BR:     7650 - 7699 */
#define  ERRSMER   (ERRSM + 1200)  /* ER:     7700 - 7749 */
#define  ERRSMPN   (ERRSM + 1250)  /* PN:     7750 - 7799 */
#define  ERRSMME   (ERRSM + 1300)  /* ME:     7800 - 7849 */
#define  ERRSMXN   (ERRSM + 1350)  /* x25/x75:7850 - 7899 */
#define  ERRSMQI   (ERRSM + 1400)  /* QI:     7900 - 7949 */
#define  ERRSMWN   (ERRSM + 1450)  /* WN:     7950 - 7999 */
#define  ERRSMMA   (ERRSM + 1500)  /* MA:     8000 - 8049 */
#define  ERRSMNV   (ERRSM + 1550)  /* NV:     8050 - 8074 */
#define  ERRSMLV   (ERRSM + 1575)  /* LV:     8075 - 8099 */
#define  ERRSMEV   (ERRSM + 1600)  /* EV:     8100 - 8124 */
#define  ERRSMAF   (ERRSM + 1625)  /* AF:     8125 - 8224 */
#define  ERRSMFR   (ERRSM + 1725)  /* FR:     8225 - 8324 */
#define  ERRSMAC   (ERRSM + 1825)  /* AC:     8325 - 8324 */
#define  ERRSMIA   (ERRSM + 1925)  /* IA:     8425 - 8524 */
#define  ERRSMPU   (ERRSM + 2625)  /* PU:     9125 - 9200 */
#define  ERRSMSG   (ERRSM + 2700)  /* SG:     9200 - 9300 */
#define  ERRSMPQ   (ERRSM + 2800)  /* PQ:     9300 - 9349 */
#define  ERRSMVO   (ERRSM + 2900)  /* VO:     9400 - 9499 */
#define  ERRSMZP   (ERRSM + 3000)  /* ZP:     9400 - 9499 */
#define  ERRSMZL   (ERRSM + 3100)  /* ZL:     9400 - 9499 */
/* Nuera PS Changes - M1UA Entries */
#define  ERRSMMZ   (ERRSM + 4000)  /* MZ:     9400 - 9499 */
#define  ERRSMD2   (ERRSM + 4100)  /* D2: */


/* assign error numbers less than ERRLL = 8500 only */


/* management interface - general error codes */

#define  ESMBASE     (ERRSM + 0)       /* reserved */
#define  ESMXXX      (ESMBASE + 0)     /* reserved */
#define  ESM001      (ESMBASE + 1)     /* smActvTsk: invalid source entity */


/* management interface - UME error codes  - should move to smum_err.h */

#define  ESMUMBASE   (ERRSMUM   + 0)   /* reserved */
#define  ESMUMXXX    (ESMUMBASE + 0)   /* reserved */


/* management interface - TCAP error codes  - should move to smst_err.h */

#define  ESMSTBASE   (ERRSMST   + 0)   /* reserved */
#define  ESMSTXXX    (ESMSTBASE + 0)   /* reserved */
#define  ESMSTX      ESMSTXXX          /* reserved - should be deleted */


/* management interface - SCCP error codes - should move to smsp_err.h */

#define  ESMSPBASE   (ERRSMSP   + 0)   /* reserved */
#define  ESMSPXXX    (ESMSPBASE + 0)   /* reserved */
#define  ESMSPX      ESMSPXXX          /* reserved - should be deleted */


/* management interface - MTP3 error codes - should move to smsn_err.h */

#define  ESMSNBASE   (ERRSMSN   + 0)   /* reserved */
#define  ESMSNXXX    (ESMSNBASE + 0)   /* reserved */
#define  ESMSNX      ESMSNXXX          /* reserved - should be deleted */


/* management interface - MTP2 error codes - should move to smsd_err.h */

#define  ESMSDBASE   (ERRSMSD   + 0)   /* reserved */
#define  ESMSDXXX    (ESMSDBASE + 0)   /* reserved */
#define  ESMSDX      ESMSDXXX          /* reserved - should be deleted */


/* layer management */
  
#define  ELMBASE     (ERRLM + 0)    /* reserved */
#define  ELMXXX      (ELMBASE)      /* reserved */
  
#define  ELM001      (ERRLM + 1)    /*  */
#define  ELM002      (ERRLM + 2)    /*  */
#define  ELM003      (ERRLM + 3)    /*  */
#define  ELM004      (ERRLM + 4)    /*  */
#define  ELM005      (ERRLM + 5)    /*  */
#define  ELM006      (ERRLM + 6)    /*  */
#define  ELM007      (ERRLM + 7)    /*  */
#define  ELM008      (ERRLM + 8)    /*  */
#define  ELM009      (ERRLM + 9)    /*  */
#define  ELM010      (ERRLM + 10)   /*  */
#define  ELM011      (ERRLM + 11)   /*  */
#define  ELM012      (ERRLM + 12)   /*  */
#define  ELM013      (ERRLM + 13)   /*  */
#define  ELM014      (ERRLM + 14)   /*  */
#define  ELM015      (ERRLM + 15)   /*  */
#define  ELM016      (ERRLM + 16)   /*  */
#define  ELM017      (ERRLM + 17)   /*  */
#define  ELM018      (ERRLM + 18)   /*  */
#define  ELM019      (ERRLM + 19)   /*  */
#define  ELM020      (ERRLM + 20)   /*  */
#define  ELM021      (ERRLM + 21)   /*  */
#define  ELM022      (ERRLM + 22)   /*  */
#define  ELM023      (ERRLM + 23)   /*  */
#define  ELM024      (ERRLM + 24)   /*  */
#define  ELM025      (ERRLM + 25)   /*  */
#define  ELM026      (ERRLM + 26)   /*  */
#define  ELM027      (ERRLM + 27)   /* pkMAMCfgReq: illegal elmt id type */
#define  ELM028      (ERRLM + 28)   /* pkMAMStaReq: illegal elmt id type */
#define  ELM029      (ERRLM + 29)   /* pkMAMStaCfm: illegal elmt id type */
#define  ELM030      (ERRLM + 30)   /* unpkMAMStaCfm */
#define  ELM031      (ERRLM + 31)   /* unpkMUMStaCfm */
#define  ELM032      (ERRLM + 32)   /* unpkMUMStaCfm */
#define  ELM033      (ERRLM + 33)   /* unpkMUMStsCfm */
#define  ELM034      (ERRLM + 34)   /* unpkMUMStaInd */
#define  ELM035      (ERRLM + 35)   /* unpkMASStaCfm */
#define  ELM036      (ERRLM + 36)   /* unpkMASStaCfm */
#define  ELM037      (ERRLM + 37)   /* unpkMASStsCfm */
#define  ELM038      (ERRLM + 38)   /* unpkMASStaInd */
#define  ELM039      (ERRLM + 39)   /* unpkMASUDatInd */
#define  ELM040      (ERRLM + 40)   /* lmActvTsk: illegal event code */
#define  ELM041      (ERRLM + 41)   /* lmActvTsk: illegal event code */
#define  ELM042      (ERRLM + 42)   /* lmActvTsk: illegal event code */
#define  ELM043      (ERRLM + 43)   /* lmActvTsk: illegal event code */
#define  ELM044      (ERRLM + 44)   /* lmActvTsk: illegal event code */
#define  ELM045      (ERRLM + 45)   /* lmActvTsk: illegal event code */
#define  ELM046      (ERRLM + 46)   /* lmActvTsk: illegal event code */
#define  ELM047      (ERRLM + 47)   /* lmActvTsk: illegal event code */
#define  ELM048      (ERRLM + 48)   /* lmActvTsk: illegal event code */
#define  ELM049      (ERRLM + 49)   /* lmActvTsk: illegal event code */
#define  ELM050      (ERRLM + 50)   /* lmActvTsk: illegal event code */
#define  ELM051      (ERRLM + 51)   /* lmActvTsk: illegal event code */
#define  ELM052      (ERRLM + 52)   /* lmActvTsk: illegal event code */
#define  ELM053      (ERRLM + 53)   /* lmActvTsk: illegal event code */
#define  ELM054      (ERRLM + 54)   /* lmActvTsk: illegal event code */
#define  ELM055      (ERRLM + 55)   /* lmActvTsk: illegal event code */
#define  ELM056      (ERRLM + 56)   /* lmActvTsk: illegal event code */
#define  ELM057      (ERRLM + 57)   /* lmActvTsk: illegal event code */
#define  ELM058      (ERRLM + 58)   /* pkMUMStaCfm */
#define  ELM059      (ERRLM + 59)   /* pkMM6StaCfm */
#define  ELM060      (ERRLM + 60)   /* pkMM6CfgReq */
#define  ELM061      (ERRLM + 61)   /* pkMM6StaReq */
#define  ELM062      (ERRLM + 62)   /* pkMUMCfgReq */
#define  ELM063      (ERRLM + 63)   /* pkMUMStaReq */
#define  ELM064      (ERRLM + 64)   /* pkMTPCfgReq */
#define  ELM065      (ERRLM + 65)   /* pkMSTCfgReq */
#define  ELM066      (ERRLM + 66)   /* lmActvTsk: illegal event code */
#define  ELM067      (ERRLM + 67)   /* lmActvTsk: illegal event code */
#define  ELM068      (ERRLM + 68)   /* lmActvTsk: illegal event code */
#define  ELM069      (ERRLM + 69)   /* lmActvTsk: illegal event code */
#define  ELM070      (ERRLM + 70)   /* lmActvTsk: illegal event code */
#define  ELM071      (ERRLM + 71)   /* lmActvTsk: illegal event code */
#define  ELM072      (ERRLM + 72)   /* lmActvTsk: illegal event code */
#define  ELM073      (ERRLM + 73)   /* lmActvTsk: illegal event code */
#define  ELM074      (ERRLM + 74)   /* lmActvTsk: illegal event code */
#define  ELM075      (ERRLM + 75)   /* lmActvTsk */


/* lapb */

#ifdef LB            /* lapb */

/* following error codes are for version 1.x of lapb */

#define  ELBBASE     (ERRLB + 0)    /* reserved */
#define  ELBXXX      (ELBBASE)      /* reserved */

#define  ELB001      (ERRLB + 1)    /*  */
#define  ELB002      (ERRLB + 2)    /*  */
#define  ELB003      (ERRLB + 3)    /*  */
#define  ELB004      (ERRLB + 4)    /*  */
#define  ELB005      (ERRLB + 5)    /*  */
#define  ELB006      (ERRLB + 6)    /*  */
#define  ELB007      (ERRLB + 7)    /*  */
#define  ELB008      (ERRLB + 8)    /*  */
#define  ELB009      (ERRLB + 9)    /*  */
#define  ELB010      (ERRLB + 10)   /*  */
#define  ELB011      (ERRLB + 11)   /*  */
#define  ELB012      (ERRLB + 12)   /*  */
#define  ELB013      (ERRLB + 13)   /*  */
#define  ELB014      (ERRLB + 14)   /*  */
#define  ELB015      (ERRLB + 15)   /*  */
#define  ELB016      (ERRLB + 16)   /*  */
#define  ELB017      (ERRLB + 17)   /*  */
#define  ELB018      (ERRLB + 18)   /*  */
#define  ELB019      (ERRLB + 19)   /*  */
#define  ELB020      (ERRLB + 20)   /*  */
#define  ELB021      (ERRLB + 21)   /*  */
#define  ELB022      (ERRLB + 22)   /*  */
#define  ELB023      (ERRLB + 23)   /*  */
#define  ELB024      (ERRLB + 24)   /*  */
#define  ELB025      (ERRLB + 25)   /*  */
#define  ELB026      (ERRLB + 26)   /*  */
#define  ELB027      (ERRLB + 27)   /*  */
#define  ELB028      (ERRLB + 28)   /*  */
#define  ELB029      (ERRLB + 29)   /*  */
#define  ELB030      (ERRLB + 30)   /*  */
#define  ELB031      (ERRLB + 31)   /*  */
#define  ELB032      (ERRLB + 32)   /*  */
#define  ELB033      (ERRLB + 33)   /*  */
#define  ELB034      (ERRLB + 34)   /*  */
#define  ELB035      (ERRLB + 35)   /*  */
#define  ELB036      (ERRLB + 36)   /*  */
#define  ELB037      (ERRLB + 37)   /*  */
#define  ELB038      (ERRLB + 38)   /*  */
#define  ELB039      (ERRLB + 39)   /*  */
#define  ELB040      (ERRLB + 40)   /*  */
#define  ELB041      (ERRLB + 41)   /*  */
#define  ELB042      (ERRLB + 42)   /*  */
#define  ELB043      (ERRLB + 43)   /*  */
#define  ELB044      (ERRLB + 44)   /*  */
#define  ELB045      (ERRLB + 45)   /*  */
#define  ELB046      (ERRLB + 46)   /*  */
#define  ELB047      (ERRLB + 47)   /*  */
#define  ELB048      (ERRLB + 48)   /*  */
#define  ELB049      (ERRLB + 49)   /*  */
#define  ELB050      (ERRLB + 50)   /*  */
#define  ELB051      (ERRLB + 51)   /*  */
#define  ELB052      (ERRLB + 52)   /*  */
#define  ELB053      (ERRLB + 53)   /*  */
#define  ELB054      (ERRLB + 54)   /*  */
#define  ELB055      (ERRLB + 55)   /*  */
#define  ELB056      (ERRLB + 56)   /*  */
#define  ELB057      (ERRLB + 57)   /*  */
#define  ELB058      (ERRLB + 58)   /*  */
#define  ELB059      (ERRLB + 59)   /*  */
#define  ELB060      (ERRLB + 60)   /*  */
#define  ELB061      (ERRLB + 61)   /*  */
#define  ELB062      (ERRLB + 62)   /*  */
#define  ELB063      (ERRLB + 63)   /*  */
#define  ELB064      (ERRLB + 64)   /*  */
#define  ELB065      (ERRLB + 65)   /*  */
#define  ELB066      (ERRLB + 66)   /*  */
#define  ELB067      (ERRLB + 67)   /*  */
#define  ELB068      (ERRLB + 68)   /*  */
#define  ELB069      (ERRLB + 69)   /*  */
#define  ELB070      (ERRLB + 70)   /*  */
#define  ELB071      (ERRLB + 71)   /*  */
#define  ELB072      (ERRLB + 72)   /*  */
#define  ELB073      (ERRLB + 73)   /*  */
#define  ELB074      (ERRLB + 74)   /*  */
#define  ELB075      (ERRLB + 75)   /*  */
#define  ELB076      (ERRLB + 76)   /*  */
#define  ELB077      (ERRLB + 77)   /*  */
#define  ELB078      (ERRLB + 78)   /*  */
#define  ELB079      (ERRLB + 79)   /*  */
#define  ELB080      (ERRLB + 80)   /*  */
#define  ELB081      (ERRLB + 81)   /*  */
#define  ELB082      (ERRLB + 82)   /*  */
#define  ELB083      (ERRLB + 83)   /*  */
#define  ELB084      (ERRLB + 84)   /*  */
#define  ELB085      (ERRLB + 85)   /*  */
#define  ELB086      (ERRLB + 86)   /*  */
#define  ELB087      (ERRLB + 87)   /*  */
#define  ELB088      (ERRLB + 88)   /*  */
#define  ELB089      (ERRLB + 89)   /*  */
#define  ELB090      (ERRLB + 90)   /*  */
#define  ELB091      (ERRLB + 91)   /*  */
#define  ELB092      (ERRLB + 92)   /*  */
#define  ELB093      (ERRLB + 93)   /*  */
#define  ELB094      (ERRLB + 94)   /*  */
#define  ELB095      (ERRLB + 95)   /*  */
#define  ELB096      (ERRLB + 96)   /*  */
#define  ELB097      (ERRLB + 97)   /*  */
#define  ELB098      (ERRLB + 98)   /*  */
#define  ELB099      (ERRLB + 99)   /*  */
#define  ELB100      (ERRLB + 100)  /*  */
#define  ELB101      (ERRLB + 101)  /*  */
#define  ELB102      (ERRLB + 102)  /*  */
#define  ELB103      (ERRLB + 103)  /*  */
#define  ELB104      (ERRLB + 104)  /*  */
#define  ELB105      (ERRLB + 105)  /*  */
#define  ELB106      (ERRLB + 106)  /*  */
#define  ELB107      (ERRLB + 107)  /*  */
#define  ELB108      (ERRLB + 108)  /*  */
#define  ELB109      (ERRLB + 109)  /*  */
#define  ELB110      (ERRLB + 110)  /*  */
#define  ELB111      (ERRLB + 111)  /*  */
#define  ELB112      (ERRLB + 112)  /*  */
#define  ELB113      (ERRLB + 113)  /*  */
#define  ELB114      (ERRLB + 114)  /*  */
#define  ELB115      (ERRLB + 115)  /*  */
#define  ELB116      (ERRLB + 116)  /*  */
#define  ELB117      (ERRLB + 117)  /*  */
#define  ELB118      (ERRLB + 118)  /*  */
#define  ELB119      (ERRLB + 119)  /*  */
#define  ELB120      (ERRLB + 120)  /*  */
#define  ELB121      (ERRLB + 121)  /*  */
#define  ELB122      (ERRLB + 122)  /*  */
#define  ELB123      (ERRLB + 123)  /*  */
#define  ELB124      (ERRLB + 124)  /*  */
#define  ELB125      (ERRLB + 125)  /*  */
#define  ELB126      (ERRLB + 126)  /*  */
#define  ELB127      (ERRLB + 127)  /*  */
#define  ELB128      (ERRLB + 128)  /*  */
#define  ELB129      (ERRLB + 129)  /*  */
#define  ELB130      (ERRLB + 130)  /*  */
#define  ELB131      (ERRLB + 131)  /*  */
#define  ELB132      (ERRLB + 132)  /*  */
#define  ELB133      (ERRLB + 133)  /*  */
#define  ELB134      (ERRLB + 134)  /*  */
#define  ELB135      (ERRLB + 135)  /*  */
#define  ELB136      (ERRLB + 136)  /*  */
#define  ELB137      (ERRLB + 137)  /*  */
#define  ELB138      (ERRLB + 138)  /*  */
#define  ELB139      (ERRLB + 139)  /*  */
#define  ELB140      (ERRLB + 140)  /*  */
#define  ELB141      (ERRLB + 141)  /*  */
#define  ELB142      (ERRLB + 142)  /*  */
#define  ELB143      (ERRLB + 143)  /*  */
#define  ELB144      (ERRLB + 144)  /*  */
#define  ELB145      (ERRLB + 145)  /*  */
#define  ELB146      (ERRLB + 146)  /*  */
#define  ELB147      (ERRLB + 147)  /*  */
#define  ELB148      (ERRLB + 148)  /*  */
#define  ELB149      (ERRLB + 149)  /*  */
#define  ELB150      (ERRLB + 150)  /*  */
#define  ELB151      (ERRLB + 151)  /*  */
#define  ELB152      (ERRLB + 152)  /*  */
#define  ELB153      (ERRLB + 153)  /*  */
#define  ELB154      (ERRLB + 154)  /*  */
#define  ELB155      (ERRLB + 155)  /*  */
#define  ELB156      (ERRLB + 156)  /*  */
#define  ELB157      (ERRLB + 157)  /*  */
#define  ELB158      (ERRLB + 158)  /*  */
#define  ELB159      (ERRLB + 159)  /*  */
#define  ELB160      (ERRLB + 160)  /*  */
#define  ELB161      (ERRLB + 161)  /*  */
#define  ELB162      (ERRLB + 162)  /*  */
#define  ELB163      (ERRLB + 163)  /*  */
#define  ELB164      (ERRLB + 164)  /*  */
#define  ELB165      (ERRLB + 165)  /*  */
#define  ELB166      (ERRLB + 166)  /*  */
#define  ELB167      (ERRLB + 167)  /*  */
#define  ELB168      (ERRLB + 168)  /*  */
#define  ELB169      (ERRLB + 169)  /*  */
#define  ELB170      (ERRLB + 170)  /*  */
#define  ELB171      (ERRLB + 171)  /*  */
#define  ELB172      (ERRLB + 172)  /*  */
#define  ELB173      (ERRLB + 173)  /*  */
#define  ELB174      (ERRLB + 174)  /*  */
#define  ELB175      (ERRLB + 175)  /*  */
#define  ELB176      (ERRLB + 176)  /*  */
#define  ELB177      (ERRLB + 177)  /*  */
#define  ELB178      (ERRLB + 178)  /*  */
#define  ELB179      (ERRLB + 179)  /*  */
#define  ELB180      (ERRLB + 180)  /*  */
#define  ELB181      (ERRLB + 181)  /*  */
#define  ELB182      (ERRLB + 182)  /*  */
#define  ELB183      (ERRLB + 183)  /*  */
#define  ELB184      (ERRLB + 184)  /*  */
#define  ELB185      (ERRLB + 185)  /*  */
#define  ELB186      (ERRLB + 186)  /*  */
#define  ELB187      (ERRLB + 187)  /*  */
#define  ELB188      (ERRLB + 188)  /*  */
#define  ELB189      (ERRLB + 189)  /*  */
#define  ELB190      (ERRLB + 190)  /*  */
#define  ELB191      (ERRLB + 191)  /*  */
#define  ELB192      (ERRLB + 192)  /*  */
#define  ELB193      (ERRLB + 193)  /*  */
#define  ELB194      (ERRLB + 194)  /*  */
#define  ELB195      (ERRLB + 195)  /*  */
#define  ELB196      (ERRLB + 196)  /*  */
#define  ELB197      (ERRLB + 197)  /*  */
#define  ELB198      (ERRLB + 198)  /*  */
#define  ELB199      (ERRLB + 199)  /*  */
#define  ELB200      (ERRLB + 200)  /*  */
#define  ELB201      (ERRLB + 201)  /*  */
#define  ELB202      (ERRLB + 202)  /*  */
#define  ELB203      (ERRLB + 203)  /*  */
#define  ELB204      (ERRLB + 204)  /*  */
#define  ELB205      (ERRLB + 205)  /*  */
#define  ELB206      (ERRLB + 206)  /*  */
#define  ELB207      (ERRLB + 207)  /*  */
#define  ELB208      (ERRLB + 208)  /*  */
#define  ELB209      (ERRLB + 209)  /*  */
#define  ELB210      (ERRLB + 210)  /*  */
#define  ELB211      (ERRLB + 211)  /*  */
#define  ELB212      (ERRLB + 212)  /*  */
#define  ELB213      (ERRLB + 213)  /*  */
#define  ELB214      (ERRLB + 214)  /*  */
#define  ELB215      (ERRLB + 215)  /*  */
#define  ELB216      (ERRLB + 216)  /*  */
#define  ELB217      (ERRLB + 217)  /*  */
#define  ELB218      (ERRLB + 218)  /*  */
#define  ELB219      (ERRLB + 219)  /*  */
#define  ELB220      (ERRLB + 220)  /*  */
#define  ELB221      (ERRLB + 221)  /*  */
#define  ELB222      (ERRLB + 222)  /*  */
#define  ELB223      (ERRLB + 223)  /*  */
#define  ELB224      (ERRLB + 224)  /*  */
#define  ELB225      (ERRLB + 225)  /*  */
#define  ELB226      (ERRLB + 226)  /*  */
#define  ELB227      (ERRLB + 227)  /*  */
#define  ELB228      (ERRLB + 228)  /*  */
#define  ELB229      (ERRLB + 229)  /*  */
#define  ELB230      (ERRLB + 230)  /*  */
#define  ELB231      (ERRLB + 231)  /*  */
#define  ELB232      (ERRLB + 232)  /*  */
#define  ELB233      (ERRLB + 233)  /*  */
#define  ELB234      (ERRLB + 234)  /*  */
#define  ELB235      (ERRLB + 235)  /*  */
#define  ELB236      (ERRLB + 236)  /*  */
#define  ELB237      (ERRLB + 237)  /*  */
#define  ELB238      (ERRLB + 238)  /*  */
#define  ELB239      (ERRLB + 239)  /*  */
#define  ELB240      (ERRLB + 240)  /*  */
#define  ELB241      (ERRLB + 241)  /*  */
#define  ELB242      (ERRLB + 242)  /*  */
#define  ELB243      (ERRLB + 243)  /*  */
#define  ELB244      (ERRLB + 244)  /*  */
#define  ELB245      (ERRLB + 245)  /*  */
#define  ELB246      (ERRLB + 246)  /*  */
#define  ELB247      (ERRLB + 247)  /*  */
#define  ELB248      (ERRLB + 248)  /*  */
#define  ELB249      (ERRLB + 249)  /*  */
#define  ELB250      (ERRLB + 250)  /*  */
#define  ELB251      (ERRLB + 251)  /*  */
#define  ELB252      (ERRLB + 252)  /*  */
#define  ELB253      (ERRLB + 253)  /*  */
#define  ELB254      (ERRLB + 254)  /*  */
#define  ELB255      (ERRLB + 255)  /*  */
#define  ELB256      (ERRLB + 256)  /*  */
#define  ELB257      (ERRLB + 257)  /*  */
#define  ELB258      (ERRLB + 258)  /*  */
#define  ELB259      (ERRLB + 259)  /*  */
#define  ELB260      (ERRLB + 260)  /*  */
#define  ELB261      (ERRLB + 261)  /*  */
#define  ELB262      (ERRLB + 262)  /*  */
#define  ELB263      (ERRLB + 263)  /*  */
#define  ELB264      (ERRLB + 264)  /*  */
#define  ELB265      (ERRLB + 265)  /*  */
#define  ELB266      (ERRLB + 266)  /*  */
#define  ELB267      (ERRLB + 267)  /*  */
#define  ELB268      (ERRLB + 268)  /*  */
#define  ELB269      (ERRLB + 269)  /*  */
#define  ELB270      (ERRLB + 270)  /*  */
#define  ELB271      (ERRLB + 271)  /*  */
#define  ELB272      (ERRLB + 272)  /*  */
#define  ELB273      (ERRLB + 273)  /*  */
#define  ELB274      (ERRLB + 274)  /*  */
#define  ELB275      (ERRLB + 275)  /*  */
#define  ELB276      (ERRLB + 276)  /*  */
#define  ELB277      (ERRLB + 277)  /*  */
#define  ELB278      (ERRLB + 278)  /*  */
#define  ELB279      (ERRLB + 279)  /*  */
#define  ELB280      (ERRLB + 280)  /*  */
#define  ELB281      (ERRLB + 281)  /*  */
#define  ELB282      (ERRLB + 282)  /*  */
#define  ELB283      (ERRLB + 283)  /*  */
#define  ELB284      (ERRLB + 284)  /*  */
#define  ELB285      (ERRLB + 285)  /*  */
#define  ELB286      (ERRLB + 286)  /*  */
#define  ELB287      (ERRLB + 287)  /*  */
#define  ELB288      (ERRLB + 288)  /*  */
#define  ELB289      (ERRLB + 289)  /*  */
#define  ELB290      (ERRLB + 290)  /*  */
#define  ELB291      (ERRLB + 291)  /*  */
#define  ELB292      (ERRLB + 292)  /*  */
#define  ELB293      (ERRLB + 293)  /*  */
#define  ELB294      (ERRLB + 294)  /*  */
#define  ELB295      (ERRLB + 295)  /*  */
#define  ELB296      (ERRLB + 296)  /*  */
#define  ELB297      (ERRLB + 297)  /*  */
#define  ELB298      (ERRLB + 298)  /*  */
#define  ELB299      (ERRLB + 299)  /*  */
#define  ELB300      (ERRLB + 300)  /*  */
#define  ELB301      (ERRLB + 301)  /*  */
#define  ELB302      (ERRLB + 302)  /*  */
#define  ELB303      (ERRLB + 303)  /*  */
#define  ELB304      (ERRLB + 304)  /*  */
#define  ELB305      (ERRLB + 305)  /*  */
#define  ELB306      (ERRLB + 306)  /*  */
#define  ELB307      (ERRLB + 307)  /*  */
#define  ELB308      (ERRLB + 308)  /*  */
#define  ELB309      (ERRLB + 309)  /*  */
#define  ELB310      (ERRLB + 310)  /*  */
#define  ELB311      (ERRLB + 311)  /*  */
#define  ELB312      (ERRLB + 312)  /*  */
#define  ELB313      (ERRLB + 313)  /*  */
#define  ELB314      (ERRLB + 314)  /*  */
#define  ELB315      (ERRLB + 315)  /*  */
#define  ELB316      (ERRLB + 316)  /*  */
#define  ELB317      (ERRLB + 317)  /*  */
#define  ELB318      (ERRLB + 318)  /*  */
#define  ELB319      (ERRLB + 319)  /*  */
#define  ELB320      (ERRLB + 320)  /*  */
#define  ELB321      (ERRLB + 321)  /*  */
#define  ELB322      (ERRLB + 322)  /*  */
#define  ELB323      (ERRLB + 323)  /*  */
#define  ELB324      (ERRLB + 324)  /*  */
#define  ELB325      (ERRLB + 325)  /*  */
#define  ELB326      (ERRLB + 326)  /*  */
#define  ELB327      (ERRLB + 327)  /*  */
#define  ELB328      (ERRLB + 328)  /*  */
#define  ELB329      (ERRLB + 329)  /*  */
#define  ELB330      (ERRLB + 330)  /*  */
#define  ELB331      (ERRLB + 331)  /*  */
#define  ELB332      (ERRLB + 332)  /*  */
#define  ELB333      (ERRLB + 333)  /*  */
#define  ELB334      (ERRLB + 334)  /*  */
#define  ELB335      (ERRLB + 335)  /*  */
#define  ELB336      (ERRLB + 336)  /*  */
#define  ELB337      (ERRLB + 337)  /*  */
#define  ELB338      (ERRLB + 338)  /*  */
#define  ELB339      (ERRLB + 339)  /*  */
#define  ELB340      (ERRLB + 340)  /*  */
#define  ELB341      (ERRLB + 341)  /*  */
#define  ELB342      (ERRLB + 342)  /*  */
#define  ELB343      (ERRLB + 343)  /*  */
#define  ELB344      (ERRLB + 344)  /*  */
#define  ELB345      (ERRLB + 345)  /*  */
#define  ELB346      (ERRLB + 346)  /*  */
#define  ELB347      (ERRLB + 347)  /*  */
#define  ELB348      (ERRLB + 348)  /*  */
#define  ELB349      (ERRLB + 349)  /*  */
#define  ELB350      (ERRLB + 350)  /*  */
#define  ELB351      (ERRLB + 351)  /*  */
#define  ELB352      (ERRLB + 352)  /*  */
#define  ELB353      (ERRLB + 353)  /*  */
#define  ELB354      (ERRLB + 354)  /*  */
  
#endif


/* Error classes */
#ifndef ERRCLS_ADD_RES
#define ERRCLS_ADD_RES 0x1
#endif /* ERRCLS_ADD_RES */

#ifndef ERRCLS_INT_PAR
#define ERRCLS_INT_PAR 0x2
#endif /* ERRCLS_INT_PAR */

#ifndef ERRCLS_DEBUG
#define ERRCLS_DEBUG   0x4
#endif /* ERRCLS_DEBUG */

/* New error class for FTHA added */
#ifndef ERRCLS_FTHA
#define ERRCLS_FTHA    0x8
#endif /* ERRCLS_FTHA */

#ifndef NO_ERRCLS
/* New error class for FTHA added */
#define ERRCLASS ( ERRCLS_ADD_RES|ERRCLS_INT_PAR|ERRCLS_DEBUG|ERRCLS_FTHA )
#else
#define ERRCLASS 0
#endif /* NO_ERRCLS */

/* macros to validate packing functions */

#if (ERRCLASS & ERRCLS_ADD_RES)

#define CMCHKPK(func, val, mBuf) \
   { \
      S16 ret; \
      if ((ret = func(val, mBuf)) != ROK) \
         return (ret); \
   }

#define CMCHKPKLOG(func, val, mBuf, errCode, pst) \
   { \
      S16 ret; \
      if ((ret = func(val, mBuf)) != ROK) \
      { \
         SPutMsg(mBuf); \
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_ADD_RES, errCode, (ErrVal)ret, \
                   "Packing failure"); \
          return (ret); \
       } \
   }

#define CMCHKPKVERLOG(func, val, mBuf, errCode, pst) \
   { \
      S16 ret; \
      if ((ret = func(pst, val, mBuf)) != ROK) \
      { \
         SPutMsg(mBuf); \
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_ADD_RES, errCode, (ErrVal)ret, \
                   "Packing failure"); \
         return (ret); \
      } \
   }

#else   /* (ERRCLASS & ERRCLS_ADD_RES) */

#define CMCHKPK(func, val, mBuf) \
   func(val, mBuf);

#define CMCHKPKLOG(func, val, mBuf, errCode, pst) \
   func(val, mBuf);

#define CMCHKPKVERLOG(func, val, mBuf, errCode, pst) \
   func(pst, val, mBuf);

#endif   /* (ERRCLASS & ERRCLS_ADD_RES) */

/* macros to validate unpacking functions */
/*ssi_h_001.main_151 : changed check so that return value will return 
 * to the caller incase of unpacking failures
 */
#if (ERRCLASS & ERRCLS_ADD_RES)

#define CMCHKUNPK(func, val, mBuf) \
   { \
      S16 ret; \
      if ((ret = func(val, mBuf)) != ROK) \
         return (ret); \
   }

#define CMCHKUNPKLOG(func, val, mBuf, errCode, pst) \
   { \
      S16 ret; \
      if ((ret = func(val, mBuf)) != ROK) \
      { \
         SPutMsg(mBuf); \
         SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_ADD_RES, (ErrVal)errCode, (ErrVal)ret, \
                   "Unpacking failure"); \
          return (ret); \
       } \
   }

#define CMCHKUNPKVERLOG(func, val, mBuf, errCode, pst) \
   { \
      S16 ret; \
      if ((ret = func(pst, val, mBuf)) != ROK) \
      { \
         SPutMsg(mBuf); \
         SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_ADD_RES, (ErrVal)errCode, (ErrVal)ret, \
                   "Unpacking failure"); \
         return (ret); \
      } \
   }

#else   /* (ERRCLASS & ERRCLS_ADD_RES) */

#define CMCHKUNPK(func, val, mBuf) \
   func(val, mBuf);

#define CMCHKUNPKLOG(func, val, mBuf, errCode, pst) \
   func(val, mBuf);

#define CMCHKUNPKVERLOG(func, val, mBuf, errCode, pst) \
   func(pst, val, mBuf);
#endif   /* (ERRCLASS & ERRCLS_ADD_RES) */


/* strtIdx and endIdx for buffer.t.dat */

#ifdef DOS                          /* dos environment */
#ifdef OD                           /* odi environment */
#define DSTRTIDX    00              /* buffer start index */
#define DENDIDX     127             /* buffer end index */
#else                               /* non-odi environment */
#define DSTRTIDX    00              /* buffer start index */
#define DENDIDX     31              /* buffer end index */
#endif /* DOS */
#else  /* not dos */
#ifdef UNIX                         /* unix environment */
#define DSTRTIDX    00              /* buffer start index */
#define DENDIDX     31              /* buffer end index */
#else  /* not unix */
#ifdef MOTADS                       /* motorola ads environment */
#define DSTRTIDX    00              /* buffer start index */
#define DENDIDX     63              /* buffer end index */
#else  /* not MOTADS */
#ifdef PS
#define DSTRTIDX    00              /* buffer start index */
#define DENDIDX     31              /* buffer end index */
#else  /* not PS */
#ifdef QUICCADS
#define DSTRTIDX    00              /* buffer start index */
#define DENDIDX     31              /* buffer end index */
#else /* not quiccads */
#ifdef ALC_EVAL
/*
 * The ALC receive buffers should be of size
 *    ((n+1)*48) + 47
 * in order take care of bug ALC02001
 *   where n > 0 for chaining
 *
 * setting n = 2, we get size = 143
 */ 
#define  DSTRTIDX   00              /* buffer start index */
#define  DENDIDX    142             /* buffer end index */
#else /* not alceval */
#ifdef ELVIS_68302
#define DSTRTIDX    00
#define DENDIDX     63
#else
#ifdef ELVIS_68360
#define DSTRTIDX    00
#define DENDIDX     63
#else
#define DSTRTIDX    00              /* buffer start index */
#define DENDIDX     31              /* buffer end index */
#endif  /* ELVIS_68360 */
#endif  /* ELVIS_68302 */
#endif  /* ALC_EVAL */
#endif /* QUICCADS */
#endif /* PS */
#endif /* MOTADS */
#endif /* UNIX */
#endif /* DOS */

#define DSIZE       (DENDIDX+1)       /* buffer size */

#ifdef SUNATMADPT                   /* sun atm adaptor */
#define DSTRTPAD     8
#else
#define DSTRTPAD     0
#endif /* SUNATMADPT */

/* region and pool macros */

#define SGETBUFREGION(buf)   (DFLT_REGION)
#define SGETBUFPOOL(buf)     (DFLT_POOL)

/* bufType for buffer */
  
#define BDATA       0x01              /* data buffer */
#define BMSG        0x02              /* message buffer */
#define BTSK        0x03              /* task buffer */
/* #define BEXT        0x04               extension buffer */
/* #define BDMA        0x05               dma buffer */
#define BDPOOL      0x06              /* dynamic pool buffer */
#define BSPOOL      0x07              /* static pool buffer */
#define BUNK        0x08              /* unknown buffer */
   
/* ttype for SRegTsk function */
  
#define TTUND       0x00              /* undefined task */
#define TTNORM      0x01              /* normal task - non preemptive */
#define TTPERM      0x02              /* permanent task */
#define TTPREEMPT   0x03              /* normal task - preemptive */
#define TTTMR       0x04              /* timer task */
#define TTMOS       0x05              /* MOS */

#ifdef MOSVER
#define MAXEXTENT   0x04              /* maximum extension entries */
#endif
#ifdef PS
#define MAXEXTENT   0x04              /* maximum extension entries */
#endif


/* priority, route, entity and instance are passed to SPostTsk function */
/* for loosely coupled interface */

/* process id for mos header */
#define PROCIDNC    0xFFFF            /* processor id, not configured */

/* TUCL priority for Create Task */
#ifdef HI_MULTI_THREADED
#ifndef TENB_RTLIN_CHANGES
#define HI_STSK_PRI             13
#else
#define HI_STSK_PRI             35
#endif
#endif

/* priority for mos header, maximum of 16 allowed */
  
#define PRIOR0      0x00              /* priority 0 - highest */
#define PRIOR1      0x01              /* priority 1 */
#define PRIOR2      0x02              /* priority 2 */
#define PRIOR3      0x03              /* priority 3 - lowest */
#define PRIORNC     0xFF              /* priority not configured */
  
/* route for mos header */
  
#define RTESPEC     0x00              /* route to specific instance */
#define RTEFRST     0x01              /* route to first available instance */
#define RTEALL      0x02              /* route to all available instances */
#define RTESHM      0x03              /* route for shared memory */
#define RTETRNQ     0x04              /* route for SWFT messages */
#define RTEQONLY    0x05              /* for messages towards non SWFT nodes*/
#define RTE_PROTO   0x06              /* for messages towards protocl layers*/
#define RTE_RT_UPD  0x07              /* for messages towards standby nodes*/
#define RTE_REV_UPD 0x08              /* for messages towards master nodes*/
#define RTENC       0xFF              /* route not configured */

/* entity for mos header */

#define ENTPRM      0x00              /* Permanent task */
#define ENTTST      0x00              /* Test */
#define ENTSS       0x01              /* Common System Services */
#define ENTMS       ENTSS             /* Multiprocessor Operating System */
#define ENTNS       ENTSS             /* NTSS */
#define ENTSC       0x02              /* Physical Layer - SCC */
#define ENTLB       0x03              /* Data Link Layer - LAPB */
#define ENTLD       0x04              /* Data Link Layer - LAPD */
#define ENTML       0x05              /* Data Link Layer - Multilink */
#define ENTBD       0x06              /* Data Link Layer - LAPB & LAPD */
#define ENTXI       0x07              /* Network Layer - X.25 Intermediate System */
#define ENTXN       0x08              /* Network Layer - X.25 End System */
#define ENTXG       0x09              /* Network Layer - X.75 */
#define ENTIN       0x0A              /* Network Layer - Q.930/Q.931 */
#define ENTAP       0x0B              /* Application Layer - Asynchronous PAD */
#define ENTBR       0x0C              /* Data Link Layer - Basic Frame Relay */
                                      /* OBSOLETE */
#ifndef ENTER
#define ENTER       0x0D              /* Data Link Layer - Extended Frame Relay */
#endif
                                      /* OBSOLETE */
#define ENTSM       0x0E              /* Stack Manager */
#define ENTMD       0x0F              /* Mos Driver */
#define ENTAL       0x10              /* Application Program */
#define ENTLDLM     0x11              /* Lapd Layer Manager */
#define ENTTC       0x12              /* TCP/IP */
#define ENTIX       0x13              /* X.31 */
#define ENTSI       0x14              /* SS7 - isup */
#define ENTSN       0x15              /* SS7 - mtp, level 3 */
#define ENTSD       0x16              /* SS7 - mtp, level 2 */
#define ENTST       0x17              /* SS7 - tcap */
#define ENTDB       0x18              /* 68000 Debugger */
#define ENTIS       0x19              /* Physical - ISAC-S */
#define ENTLC       0x1A              /* Data Link Layer - LLC */
#define ENTOD       0x1B              /* Novell ODI */
#define ENTSE       0x1C              /* 68302 */
#define ENTHE       0x1D              /* Hello World */
#define ENTSP       0x1E              /* SS7 - sccp */
#define ENTCC       0x1F              /* Interworking call control */
#define ENTTU       0x20              /* SS7 - tcap user */
#define ENTUD       0x21              /* UDP/IP (without TCP) */
#define ENTM1       0x22              /* Dummy MAC (with MACINT1) */
#define ENTM2       0x23              /* Dummy MAC (with MACINT2) */
#define ENTM3       0x24              /* Dummy MAC (with MACINT3) */
#define ENTTP       0x25              /* SS7 - tup */
#define ENTBS       0x26              /* ATM - LANE Broadcast Server (BUS) */
#define ENTM4       0x27              /* Dummy MAC (with MACINT4) */
#define ENTM5       0x28              /* Dummy MAC (with MACINT5) */
#define ENTM6       0x29              /* Dummy MAC (with MACINT6) */
#define ENTM7       0x2A              /* Dummy MAC (with MACINT7) */
#define ENTM8       0x2B              /* Dummy MAC (with MACINT8) */
#define ENTM9       0x2C              /* Dummy MAC (with MACINT9) */
#define ENTME       0x2D              /* Message Exchange */
#define ENTAC       0x2E              /* ATM - cpcs */
#define ENTAS       0x2F              /* ATM - q.saal */
#define ENTAM       0x30              /* ATM - q.93b */
#define ENTUM       0x31              /* ATM - ume */
#define ENTMU       0x32              /* ATM - management user */
#define ENTLR       0x33              /* loader */
#define ENTLU       0x34              /* loader user */
#define ENTRY       0x35              /* relay */
#define ENTEC       0x36              /* ATM - LANE Client (LEC) */
#define ENTFA       0x37              /* ATM - Fujitsu ALC driver */
#define ENTSR       0x38              /* MTP 3 Simple Router */
#define ENTES       0x39              /* ATM - LANE Services (LEServices) */
#define ENTPN       0x3A              /* ATM - PNNI */
#define ENTQI       0x3B              /* QUICC Driver */
#define ENTWD       0x3C              /* MTP Level 2 Wrapper */
#define ENTWN       0x3D              /* MTP Level 3 Wrapper */
#define ENTWI       0x3E              /* ISUP Wrapper */
#define ENTWU       0x3F              /* TUP Wrapper */
#define ENTWS       0x40              /* SCCP Wrapper */
#define ENTWC       0x41              /* TCAP Wrapper */
#define ENTPU       0x42              /* ATM PNNI User */
#define ENTSA       0x43              /* ATM - Q.2140 */
#define ENTFM       0x44              /* SS7 - Fault Manager */
#define ENTBI       0x45              /* ATM - b-isup */
#define ENTMA       0x46              /* SS7 - MAP*/
#define ENTFN       0x47              /* Frame Relay Network Layer - Q.933 */
#define ENTNV       0x48              /* V5.X PSTN */
#define ENTLV       0x49              /* LAPV5-DL */
#define ENTEV       0x4a              /* V5.X Envelope Function */
#define ENTPL       0x4b              /* ATM - PMC Sierra LASAR driver */
#define ENTAU       0x4c              /* MAP Upper User */
#define ENTVM       0x4d              /* V5.1 mac layer */
#define ENTAF       0x4e              /* Frame Relay ATM IW Layer */
#define ENTFR       0x4f              /* Frame Relay */
#define ENTMT       0x50              /* mt_ss */
#define ENTCV       0x51              /* V5.X Control Protocol */
#define ENTMV       0x52              /* V5.X System Management Layer */
#define ENTIM       0x53              /* ATM - IME */
#define ENTBV       0x54              /* V5 - BCC. */
#define ENTPA       0x55              /* PLOA */
#define ENTPV       0x56              /* V5 - Protection Protocol. */
#define ENTLK       0x57              /* V5 - LCP */
#define ENTL1       0x58              /* V5 - Link Layer 1 */
#define ENTIA       0x59              /* map is41 */
#define ENTIU       0x5a              /* map is41 user */
#define ENTRM       0x5b              /* Resource Manager */
#define ENTRT       0x5c              /* Router */
#define ENTSG       0x5d              /* System manager */
#define ENTPQ       0x5e              /* Power QUICC Driver */
#define ENTMC       0x5f              /* Mac over sockets */
#define ENTIE       0x60              /* INAP */
#define ENTBW       ENTBI             /* BISUP Wrapper */
#define ENTIW       ENTSI             /* ISUP Wrapper */
#define ENTQW       ENTIN             /* ISDN Wrapper */
#define ENTAW       ENTAM             /* q.93B Wrapper */
#define ENTSF       0x61              /* Switching Fabric */
#define ENTVF       0x62              /* Integrated V5 layer 3 */ 
#define ENTCL       0x63              /* ASN.1 encoder/decoder */ 
#define ENTSH       0x65              /* System Agent */ 
#define ENTMR       0x66              /* Message Router */ 
#define ENTL4       0x67              /* dummy layer4 */
#define ENTTT       0x68              /* TCAP over TCP/IP */
#define ENTPR       0x69              /* MPC860SAR Driver */
#define ENTGN       0x6a              /* GPRS Network Service */
#define ENTGG       0x6b              /* GPRS BSSGP */
#define ENTAR       0x6c              /* ARI module */
#define ENTGT       0x6d              /* GPRS GTP */
#define ENTGM       0x6e              /* GPRS Mobility Management */
#define ENTGR       0x6f              /* GPRS Relay */
#define ENTGW       0x70              /* GPRS Ineterworking unit */
#define ENTGL       0x71              /* GPRS LLC */
#define ENTGS       0x72              /* GPRS SNDCP */
#define ENTGZ       0x73              /* GPRS RLC/MAC */
#define ENTGY       0x74              /* GPRS SMS */
#define ENTHI       0x75              /* TUCL */
#define ENTHC       0x76              /* H.323 */
#define ENTHU       0x77              /* H.323 user */
#define ENTHR       0x78              /* RTP/RTCP */
#define ENTNM       0x79              /* GPRS Network Management */
#define ENTGB       0x7a              /* GPRS BSS Relay */
#define ENTGP       0x7b              /* GPRS PDP at MS */
#define ENTIQ       0x7c              /* Q.930/931-Q.SAAL Convergence Layer */
#define ENTXM       0x7d              /* Connection Manager */
#define ENTMG       0x7e              /* MGCP */
#define ENTHG       0x7f              /* Annex G */
#define ENTDN       0x80              /* LDF-MTP3 */
#define ENTTS       0x81              /* Trillium Stack Manager */
#define ENTVO       0x82              /* MPC8260 Driver */
#define ENTGO       0x83              /* GTP Location-MGMT */
#define ENTGI       0x84              /* CC-3G */
#define ENTGH       0x85              /* GTP-Charging */
#define ENTGU       0x86              /* Relay on IU for GTP */
#define ENTLN       0x87              /* MPLS */
#define ENTSB       0x88              /* SCTP */
#define ENTIT       0x89              /* M3UA */
#define ENTFW       0x8a              /* MPLS - Forwarder */
#define ENTRL       0x8b              /* 3GPP-RLC */
#define ENTAL2      0x8c              /* AAL2 Signaling */
#define ENTAA       0x8d              /* AAL2 Signaling User */
#define ENTRA       0x8f              /* RANAP */
#define ENTNF       0x90              /* M3UA-NIF */
#define ENTRN       0x91              /* RNSAP */
#define ENTDP       0x92              /* LDF-SCCP */
#define ENTDT       0x93              /* LDF-TCAP */
#define ENTNP       0x94              /* SUA NIF */
#define ENTP1       0x95              /* PXY dummy for PLOA */
#define ENTID       0x96              /* IUA */
#define ENTND       0x97              /* IUA-NIF */
#define ENTDM       0x98              /* IWF */
#define ENTSO       0x99              /* SIP */
#define ENTSU       0x9a              /* SUA */
#ifndef OP_IU
/*ssi_h_001.main_145-defined new RANAP user*/
#define ENTRU       0x9b              /* RANAP User */
#else
/* Map ENTRU to ENTPP */
#define ENTRU       ENTPP              /* RANAP User */
#endif
#define ENTQC       0x9c              /* CAP */
#define ENTCU       0x9d              /* CAP User */
#define ENTMM       0x9e              /* Mobility Management at VLR */
#define ENTGA       0x9f              /* BSSAP+ */
#define ENTGE       0xa0              /* GMM Application at SGSN */
#define ENTSV       0xa1              /* SIP Application */ 
#define ENTMW       0xa2              /* M2UA */
#define ENTNW       0xa3              /* M2UA-NIF */
#define ENTDI       0xa4              /* ISUP-LDF */
#define ENTMK       0xa5              /* 3GPP-MAC */
#define ENTRR       0xa6              /* 3GPP-RRC */
#define ENTIB       0xa7              /* 3GPP-NBAP */
#define ENTPH       0xa8              /* 3GPP-PH */
#define ENTII       ENTSM             /* ISM */
#define ENTUL       0xa9              /* 3GPP-NBAP User - Dummy */
#define ENTGD       0xaa              /* RM Application at SGSN */
#define ENTGX       0xab              /* SM-APPL Application at SGSN */
#define ENTDG       0xac              /* LDF-GCP */

#define ENTXX       0xad              /* 3GPP-RRC User */
#define ENTPD       0xae              /* 3GPP-PDCP */
#define ENTUS       0xaf              /* 3GPP-USIM */
#define ENTBM       0xb0              /* 3GPP-BMC */

#define ENTLA       0xb1              /* MPLS User */
#define ENTLT       0xb2              /* MPLS Resource Manager */
#define ENTRD       0xb3              /* RNSAP user         */

#define ENTGF       0xb4              /* GMM SS User */
#define ENTDV       0xb5              /* LDF-M3UA */
#define ENTCS       0xb6              /* SIP Compression module */
#define ENTFP       0xb7              /* FP Layer    */
#define ENTDU       0xb8              /* NodeB IWF Dummy */
#define ENTFU       0xb9              /* FP Control User */
#define ENTVU       0xba              /* End Driver */
/* Nuera PS Changes - M1UA Entry */
#define ENTMZ       0xbb              /* M1UA*/
/* M2PA changes */
#define ENTMX       0xbc              /* M2PA */

#define ENTD3       0xbd              /* DPNSS/DASS2 L3 */ 
#define ENTD2       0xbe              /* DPNSS/DASS2 L3 */

#define ENTNDD3     0xbf              /* NIF for DUA */

#define ENTRP       0xc0              /* LDF - RANAP */
#define ENTDK       0xc1              /* LDF - RANAP */
#define ENTTM       0xc2              /* TOM entity  Patch ssi_h_001.main_115 */
#define ENTSPU      0xc3              /* Generic SCCPU User */
#define ENTGC       0xc4              /* Phy Layer */ 
#define ENTLM       0xc5              /* LAPDm */

/**MAC-HS ssi_h_001.main_120 */
#define ENTHS       0xc6              /* MAC-HS */
#define ENTCH       0xc7             /*MAC-HS Control Part*/ 
#define ENTUH       0xc8             /*MAC-HS User Part*/ 
/**ssi_h_001.main_122: Added Diameter and Diameter User entities */
#define ENTAQ       0xc9             /* Diameter Base Protocol */ 
#define ENTAB       0xca             /* Diameter User */ 

/* ssi_h_001.main_124: Added LDF-SUA & PSF-SUA User entities */
#define ENTAH       0xcb             /* LDF-SUA */ 
#define ENTAE       0xcc             /* PSF-SUA */ 

/* ssi_h_001.main_125 : for RRC */
#define ENTRX       0xcd             /* RRC User */
/* ssi_h_001.main_128: Entity for FAP is added */
/*ssi_h_001.main_149 warning removal*/
#ifndef OP_IU
#define ENTPP       ENTSM             /* FAP behaves as User and SM */  
#endif
/*-- ssi_h_001.main_129 --*/
#define ENTLX       0xce             /* IuUP --*/

/*ssi_h_001.main_145-defined new IuUP user*/
#ifndef OP_IU
#define ENTXU       0xcf             /* IuUP user --*/
#else
/* Map ENTXU to ENTPP */
#define ENTXU       ENTPP             /* IuUP user --*/
#endif
/* ssi_h_001.main_148: Fixed compiler warning */
/*#define ENTXU       0xcf */            /* IuUP user --*/

/* ssi_h_001.main_130 - Entity for DNRC MAC user added */
#define ENTXY       0xd0             /* App/Relay on DRNC MAC */

/* ssi_h_001.main_132 Entity added for S1AP and S1AP user */
#define ENTSZ       0xd1             /* S1AP */
#define ENTUZ       0xd2             /* S1AP User */
/* ssi_h_001.main_134 : Adtion for eGTP protocol */
#define ENTEG       0xd3            /*  EGTP Layer */
#define ENTEU       0xd4            /* EGTP USER */
/* ssi_h_001.main_135 : addition of LTE related entities */
#define ENTLWRMAC       0xd5             /* LTE-PHY */
#define ENTMAC       0xd6             /* LTE-MAC */
#define ENTRLC       0xd7             /* LTE-RLC */
#define ENTPJ       0xd8             /* LTE-PDCP */
#define ENTNH       0xd9             /* LTE-RRC */

/* ssi_h_001.main_137 : addition of LTE-X2AP related entities */
/* ssi_h_001.main_138 : rearrange entity list */
#define ENTNX       0xda             /* LTE-RRC user */
#define ENTCZ       0xdb             /* LTE-X2AP User */
#define ENTRS       0xdc             /* LTE-X2AP */

/* ssi_h_001.main_139 : addition of Iuh layer entries */
#define ENTHM       0xdd             /* Iuh */
#define ENTHX       0xde             /* Iuh-HNBAP User */

/* ssi_h_001.main_140 : addition of ENTVE Macro  */
#define ENTVE       0xdf             /* For eNodeB Application */
#define ENTWR       0xe3             /* For WR eNodeB Application */

/* ssi_h_001.main_141 : addition of PDCP layer off-board entries */
#define ENTOS       0xe0             /* Off-board Security */
#define ENTOC       0xe1             /* Off-board Compression */

#define ENTPX       0xe2             /* LTE PDCP Data User */

#ifdef SS_FAP
/* ssi_h_001.main_128: Entity for FAP is added */
/*ssi_h_001.main_145-FAP specific aditions*/
/* ssi_h_001.main_148: Entity for LTE convergence layer is added */
/* ssi_h_001.main_149 changed for warnings*/
#ifdef OP_IU
#define ENTPP       0xe3
#endif
#endif

/* ssi_h_001.main_148: Entity for LTE convergence layer is added */
#define ENTYS       0xe4             /* LTE PDCP Data User */
/* ssi_h_001.main_150: Entity for LTE MME application layer is added */
#define ENTVB       0xe5             /* Lte-CNE */
#ifdef WR_DG_OAM
#define ENTOAMSOCK  0xe6
#define ENTOAMSTAT  0xe7
#define ENTNL       0xe8             /* LTE SON Feature*/    
#define ENTLAST     ENTNL            /* last entity id */
#else /* WR_DG_OAM */
/* ssi_h_001.main_152: Entity for SIGTRAN,IuPS,IuCS and LTES1 monitoring stack added */
#define ENTVN       0xe6             /*SIGTRAN Trillium Integrated Monitoring Stack*/
#define ENTQZ       0xe7             /*Iu-PS Trillium Integrated Monitoring Stack*/
#define ENTAX       0xe8             /*Iu-CS Trillium Integrated Monitoring Stack*/
#define ENTQB       0xe9             /*LTE-S1 Trillium Integrated Monitoring Stack*/
#define ENTNL       0xea             /* LTE SON Feature */

#define ENTLAST     ENTNL            /* last entity id */
#endif /* WR_DG_OAM */

#ifdef ODU
#define ENTDUAPP    0xeb             /* ORAN DU APP */
#define ENTSCTP     0xec             /* ORAN SCTP */
#define ENTF1AP     0xed             /* ORAN F1AP */
#define ENTEGTP     0xee             /* ORAN EGTP */
#endif //ORAN
/* un-configured procId */ 
#ifdef SS_MULTIPLE_PROCS
#define PROCNC      0xFFFF            /* Processor not configured */
#endif /* SS_MULTIPLE_PROCS */

#define ENTNC       0xFF              /* Entity not configured */

/* instance for mos header */

#define INSTNC      0xFF              /* Instance not configured */

/* service user id */
#define SUIDNC      0xFFFF            /* service user id not configured */

/* service provider id */
#define SPIDNC      0xFFFF            /* service provider id not configured */

/* region id for mos header */
#define REGIONNC    0xFF              /* region not configured */

/* pool id for mos header */
#define POOLNC      0xFF              /* pool not configured */


/* event code for task to task header */

#define EVTNONE         0x00             /* None */

#define EVTPERM         0x01             /* Permanent */
#define EVT_TIMEOUT     0x02             /* Timeout */

#define EVTBNDREQ       0x04             /* Bind request */
/* #define EVTBNDCFM    0x05              Bind confirm */
/* #define EVTBNDIND    0x06              Bind indication */
/* #define EVTBNDRSP    0x07              Bind response */
  
#define EVTUBNDREQ      0x08             /* Unbind request */
/* #define EVTUBNDCFM   0x09              Unbind confirm */
/* #define EVTUBNDIND   0x0A              Unbind indication */
/* #define EVTUBNDRSP   0x0B              Unbind response */
  
#define EVTCONREQ       0x0C             /* Connect request */
#define EVTCONCFM       0x0D             /* Connect confirm */
#define EVTCONIND       0x0E             /* Connect indication */
#define EVTCONRSP       0x0F             /* Connect response */
  
#define EVTDISCREQ      0x10             /* Disconnect request */
#define EVTDISCCFM      0x11             /* Disconnect confirm */
#define EVTDISCIND      0x12             /* Disconnect indication */
#define EVTDISCRSP      0x13             /* Disconnect response */

#define EVTDATREQ       0x14             /* Data request */
#define EVTDATCFM       0x15             /* Data confirm */
#define EVTDATIND       0x16             /* Data indication */
#define EVTDATRSP       0x17             /* Data response */
  
#define EVTUDATREQ      0x18             /* Unit data request */
#define EVTUDATCFM      0x19             /* Unit data confirm */
#define EVTUDATIND      0x1A             /* Unit data indication */
#define EVTUDATRSP      0x1B             /* Unit data response */
  
#define EVTEDATREQ      0x1C             /* Expedited data request */
#define EVTEDATCFM      0x1D             /* Expedited data confirm */
#define EVTEDATIND      0x1E             /* Expedited data indication */
#define EVTEDATRSP      0x1F             /* Expedited data response */
  
#define EVTDATCHRREQ    0x20             /* Data character request */
#define EVTDATCHRCFM    0x21             /* Data character confirm */
#define EVTDATCHRIND    0x22             /* Data character indication */
#define EVTDATCHRRSP    0x23             /* Data character response */
  
#define EVTDATACKREQ    0x24             /* Data acknowledge request */
#define EVTDATACKCFM    0x25             /* Data acknowledge confirm */
#define EVTDATACKIND    0x26             /* Data acknowledge indication */
#define EVTDATACKRSP    0x27             /* Data acknowledge response */
  
#define EVTFLCREQ       0x28             /* Flow control request */
/* #define EVTFLCCFM    0x29              Flow control confirm */
#define EVTFLCIND       0x2A             /* Flow control indication */
/* #define EVTFLCRSP    0x2B              Flow control response */
                     
#define EVTRSTREQ       0x2C             /* Reset request */
#define EVTRSTCFM       0x2D             /* Reset confirmation */
#define EVTRSTIND       0x2E             /* Reset indication */
#define EVTRSTRSP       0x2F             /* Reset response */
  
#define EVTCFGXXREQ     0x30             /* Configuration request */
/* #define EVTCFGXXCFM  0x31              Configuration confirmation */
/* #define EVTCFGXXIND  0x32              Configuration indication */
/* #define EVTCFGXXRSP  0x33              Configuration response */
   
#define EVTRESMREQ      0x34             /* Call resume request */
/* #define EVTRESCFM    0x35              Call resume confirm */
#define EVTRESMIND      0x36             /* Call resume indication */
/* #define EVTRESRSP    0x37              Call resume response */

#define EVTSUSPREQ      0x38             /* Call suspend request */
/* #define EVTSUSPCFM   0x39              Call suspend confirm */
#define EVTSUSPIND      0x3A             /* Call suspend indication */
/* #define EVTSUSPRSP   0x3B              Call suspend response */

#define EVTCFGREQ       0x3C             /* Configuration request */
#define EVTCFGCFM       0x3D             /* Configuration confirmation */
#define EVTCFGIND       0x3E             /* Configuration indication */
/* #define EVTCFGXXRSP  0x3F              Configuration response */

#define EVTSTAREQ       0x40             /* Status request */
#define EVTSTACFM       0x41             /* Status confirm */
#define EVTSTAIND       0x42             /* Status indication */
/* #define EVTSTARSP    0x43              Status response */

#define EVTSTSREQ       0x44             /* Statistics request */
#define EVTSTSCFM       0x45             /* Statistics confirm */
/* #define EVTSTSIND    0x46              Statistics indication */
/* #define EVTSTSRSP    0x47              Statistics response */

#define EVTTRCIND       0x48             /* Trace indication */
/* #define EVTTRCCFM    0x49              Trace confirm */
/* #define EVTTRCIND    0x4A              Trace indication */
/* #define EVTTRCRSP    0x4B              Trace response */

#define EVTCNTRLREQ     0x4C             /* Control request */
#define EVTCNTRLCFM     0x4D             /* Control confirm */
#define EVTCNTRLIND     0x4E             /* Control indication */
#define EVTCNTRLRSP     0x4F             /* Control response */
   
#define EVTXIDREQ       0x50             /* Exchange id request */
#define EVTXIDCFM       0x51             /* Exchange id confirm */
#define EVTXIDIND       0x52             /* Exchange id indication */
#define EVTXIDRSP       0x53             /* Exchange id response */

#define EVTACNTREQ      0x54             /* Accounting request */
/* #define EVTACNTCFM   0x55              Accounting confirm */
#define EVTACNTIND      0x56             /* Accounting indication */
/* #define EVTACNTRSP   0x57              Accounting response */

#define EVTCNSTREQ      0x58             /* Connection progress status request */
/* #define EVTCNSTCFM   0x59              Connection progress status confirm */
#define EVTCNSTIND      0x5A             /* Connection progress status indication */
/* #define EVTCNSTRSP   0x5B              Connection progress status response */

#define EVTRELREQ       0x5C             /* Connection release request */
#define EVTRELCFM       0x5D             /* Connection release confirmation */
#define EVTRELIND       0x5E             /* Connection release indication */
#define EVTRELRSP       0x5F             /* Connection release response */

#define EVTSSHLREQ      0x60             /* Connection suspend and hold request */
#define EVTSSHLCFM      0x61             /* COnnection suspend and hold confirmation */
#define EVTSSHLIND      0x62             /* COnnection suspend and hold indication */
#define EVTSSHLRSP      0x63             /* Connection suspend and hold response */

#define EVTRMRTREQ      0x64             /* Call resume/retrieve request */
#define EVTRMRTCFM      0x65             /* Call resume/retrieve confirmation */
#define EVTRMRTIND      0x66             /* Call resume/retrieve indication */
#define EVTRMRTRSP      0x67             /* Call resume/retrieve response */

#define EVTFACREQ       0x68             /* Call facility request */
#define EVTFACCFM       0x69             /* Call facility confirm */
#define EVTFACIND       0x6A             /* Call facility indication */
#define EVTFACRSP       0x6B             /* Call facility response */

#define EVTSTAENQREQ    0x6C             /* Status enquiry request */
#define EVTSTAENQCFM    0x6D             /* Status enquiry confirmation */
#define EVTSTAENQIND    0x6E             /* Status enquiry indication */
#define EVTSTAENQRSP    0x6F             /* Status enquiry response */

#define EVTSRVREQ       0x70             /* Service request */
#define EVTSRVCFM       0x71             /* Service confirmation */
#define EVTSRVIND       0x72             /* Service indication */
/* #define EVTSRVRSP    0x73              Service response */

/* #define EVTALRTREQ   0x74              Alert request */
/* #define EVTALRTCFM   0x75              Alert confirmation */
#define EVTALRTIND      0x76             /* Alert indication */
/* #define EVTALRTRSP   0x77              Alert response */

#define EVTSTAXXREQ     0x78             /* Status request */
#define EVTSTAXXCFM     0x79             /* Status confirm */
#define EVTSTAXXIND     0x7A             /* Status indication */
/* #define EVTSTAXXRSP  0x7B              Status response */

#define EVTLMUDATREQ    0x7C             /* LM - unit data request */
/* #define EVTLMUDATCFM 0x7D              LM - unit data confirm */
#define EVTLMUDATIND    0x7E             /* LM - unit data indication */
/* #define EVTLMUDATRSP 0x7F              LM - unit data response */
  
#define EVTADDREQ       0x80             /* Add request */
#define EVTADDCFM       0x81             /* Add confirm */
#define EVTADDIND       0x82             /* Add indication */
#define EVTADDRSP       0x83             /* Add response */
  
#define EVTREMREQ       0x84             /* Remove request */
#define EVTREMCFM       0x85             /* Remove confirm */
#define EVTREMIND       0x86             /* Remove indication */
/* #define EVTREMRSP    0x87              Remove response */
  
#define EVTSYNREQ       0x88             /* Synchronize request */
#define EVTSYNCFM       0x89             /* Synchronize confirm */
#define EVTSYNIND       0x8A             /* Synchronize indication */
/* #define EVTSYNRSP    0x8B              Synchronize response */
  
#define EVTEXAMREQ      0x8C             /* Examine request */
#define EVTEXAMCFM      0x8D             /* Examine confirm */
/* #define EVTEXAMIND   0x8E              Examine indication */
/* #define EVTEXAMRSP   0x8F              Examine response */

#define EVTADDPTYREQ    0x90             /* Add Party Request */
#define EVTADDPTYCFM    0x91             /* Add Party Confirm */
#define EVTADDPTYIND    0x92             /* Add Party Indication */
#define EVTADDPTYRSP    0x93             /* Add Party Response */

#define EVTADDPTYREJREQ 0x94             /* Add Party Reject Request */
#define EVTADDPTYREJCFM 0x95             /* Add Party Reject Confirm */
#define EVTADDPTYREJIND 0x96             /* Add Party Reject Indication */
#define EVTADDPTYREJRSP 0x97             /* Add Party Reject Response */

#define EVTDROPPTYREQ   0x98             /* Drop Party Request */
#define EVTDROPPTYCFM   0x99             /* Drop Party Confirm */
#define EVTDROPPTYIND   0x9A             /* Drop Party Indication */
#define EVTDROPPTYRSP   0x9B             /* Drop Party Response */

#define EVTNEXTREQ      0x9C             /* Next request */
#define EVTNEXTCFM      0x9D             /* Next confirm */
#define EVTNEXTIND      0x9E             /* Next indication */
/* #define EVTNEXTRSP   0x9F              Next response */

/* #define EVTERRREQ    0xA0              Error request */
/* #define EVTERRCFM    0xA1              Error confirm */
#define EVTERRIND       0xA2             /* Error indication */
#define EVTERRRSP       0xA3             /* Error response */

#define EVT_BNDREQ      0xA4             /* Bind request */
/* #define EVT_BNDCFM   0xA5              Bind confirm */
/* #define EVT_BNDIND   0xA6              Bind indication */
/* #define EVT_BNDRSP   0xA7              Bind response */
  
#define EVT_UBNDREQ     0xA8             /* Unbind request */
/* #define EVT_UBNDCFM  0xA9              Unbind confirm */
/* #define EVT_UBNDIND  0xAA              Unbind indication */
/* #define EVT_UBNDRSP  0xAB              Unbind response */

#define EVT_DATREQ      0xAC             /* Data request */
#define EVT_DATCFM      0xAD             /* Data confirm */
#define EVT_DATIND      0xAE             /* Data indication */
#define EVT_DATRSP      0xAF             /* Data response */
  
#define EVT_UDATREQ     0xB0             /* Unit data request */
#define EVT_UDATCFM     0xB1             /* Unit data confirm */
#define EVT_UDATIND     0xB2             /* Unit data indication */
#define EVT_UDATRSP     0xB3             /* Unit data response */
  
#define EVTESTREQ       0xB4             /* Establish request */
#define EVTESTCFM       0xB5             /* Establish confirm */
#define EVTESTIND       0xB6             /* Establish indication */
#define EVTESTRSP       0xB7             /* Establish response */

#define EVTMODREQ       0xC0             /* Modify request */
#define EVTMODCFM       0xC1             /* Modify confirm */
#define EVTMODIND       0xC2             /* Modify indication */
#define EVTMODRSP       0xC3             /* Modify response */

#define EVTTSKDOWNREQ    0xF0            /* Task shutdown request */
#define EVTTSKDOWNCFM    0xF1            /* Task shutdown confirm */
/* #define EVTTSKDOWNIND 0xF2             Task shutdown indication */
/* #define EVTTSKDOWNRSP 0xF3             Task shutdown response */

#define EVTTSKUPREQ     0xF4             /* Task startup request */
#define EVTTSKUPCFM     0xF5             /* Task startup confirm */
/* #define EVTTSKUPIND  0xF6              Task startup indication */
/* #define EVTTSKUPRSP  0xF7              Task startup response */

#define EVTCRDREQ       0xF8             /* SCCP mgmt - Coordinated Request */
#define EVTCRDCFM       0xF9             /* SCCP mgmt - Coordinated Confirm */
#define EVTCRDIND       0xFA             /* SCCP mgmt - Coordinated Indication */
#define EVTCRDRSP       0xFB             /* SCCP mgmt - Coordinated Response */

#define EVTSTEREQ       0xFC             /* SCCP mgmt - State Request  */
#define EVTSTEIND       0xFD             /* SCCP mgmt - State Indication */
#define EVTPCSTEIND     0xFE             /* SCCP mgmt - PC-State Indication */

#define EVTSHIFT        0xFF             /* Shift to new event code set */


/*
   This section contains specific defines for the following operating systems:
 
   MOS      - MOSVER
   MOS, v2  - MSVER2
   psos     - PS
   vrtxsa   - VX
   vxworks  - VW
   Windows NT  - WINNT_IATM
   sslib    - system service library for elvis
   other    - 

   The defines PORTVER, MOSVER and MSVER2 are mutually exclusive. The define PORTVER
   must be set in conjunction with the define PS, VX, VW or portable/other.

*/
 

#ifdef MOSVER                     /* mos version */
/* mos defines */


#else
#ifdef MSVER2                     /* mos version 2 */
/* mos version 2 defines */
#define DFLT_REGION 0
#define DFLT_POOL 0

#ifdef SS_FAP
/*ssi_h_001.main_145- Defined new region for FAP*/
#define SS_WL_REGION 1
#endif

#define OWNREGION DFLT_REGION     /* own region */ 

#else
#ifdef PS                         /* psos version */
/* psos defines */
#define DFLT_REGION 0
#define DFLT_POOL 0

#ifdef SS_FAP
/*ssi_h_001.main_145 - Defined new region for FAP*/
#define SS_WL_REGION 1
#endif

#define OWNREGION DFLT_REGION     /* own region */ 

#else
#ifdef VX                         /* vrtxsa version */
/* vrtxsa defines */
#define DFLT_REGION 0
#define DFLT_POOL 0

#ifdef SS_FAP
/*ssi_h_001.main_145 - Defined new region for FAP*/
#define SS_WL_REGION 1
#endif

#define OWNREGION DFLT_REGION     /* own region */ 

/* events */

#define EVTTMRTCKREQ  0x80        /* Timer Tick request */

/* return codes */

#define VX_RET_OK     0x00        /* good return value */
#define VX_ER_IIP     0x12        /* input parameter error */
#define VX_ER_NOCB    0x30        /* no control block error */
#define VX_ER_NMB     0x04        /* not a memory block error */
#define VX_ER_ID      0x31        /* ID error */
#define VX_ER_MEM     0x03        /* no memory error */
#define VX_ER_PID     0x0E        /* partition id error */
#define VX_ER_QID     0x0C        /* queue id error */
#define VX_ER_QFL     0x0D        /* queue full error */

#else
#ifdef VW                         /* vxworks version */

/* vxworks defines */
#define VW_MBUF_POOL 3            /* message header pool id */
#define VW_TMR_POOL  2            /* timer pool id */
#define VW_WRAP_POOL 1            /* wrapper pool id */
#define VW_PROT_POOL 0            /* protocol pool id */

#define DFLT_REGION 0
#define DFLT_POOL VW_PROT_POOL    /* protocol data pool */

#ifdef SS_FAP
/*ssi_h_001.main_145 - Defined new region for FAP*/
#define SS_WL_REGION 1
#endif

#define OWNREGION DFLT_REGION     /* own region */ 

#else                             
#if (  defined(SSLIB) || defined(SSRYLIB) )
#define DFLT_REGION 0
#define DFLT_POOL 0
#ifdef SS_FAP
/*ssi_h_001.main_145 - Defined new region for FAP*/
#define SS_WL_REGION 1
#endif

#define MAXBUFSIZ 2048 
#define STARTOFFSET 1024
#else                             /* portable/other version */
#ifdef SS                         /* Common System Services */

/* CTL operation codes */
#define SS_MEM_V_TO_P           1
#define SS_MEM_CHK_RES          2

#define DFLT_REGION       0
#define DFLT_POOL         0
#ifdef SS_FAP
/*ssi_h_001.main_145 - Defined new region for FAP*/
#define SS_WL_REGION 1
#endif

#define OWNREGION         DFLT_REGION     /* own region */ 

/* basic lock types */
#define SS_LOCK_MUTEX                   1
#define SS_LOCK_SPIN                    2
#define SS_LOCK_CRITSEC                 3

#ifdef SS_OLD_THREAD
/* flags for SGetThread() */
#define SS_THR_NOFLAGS   0x00
#define SS_THR_SUSPENDED 0x01
#define SS_THR_DETACHED  0x02
#define SS_THR_BOUND     0x04
#define SS_THR_NEW_LWP   0x08
#define SS_THR_DAEMON    0x10
#endif /* SS_OLD_THREAD */

/* Queue Macros */
#ifdef SS_ENABLE_MACROS
#define SQueueFirst(mBuf, q) \
     SAddQueue(mBuf, (q), 0)

#define SDequeueFirst(mBuf, q) \
     SRemQueue(mBuf, (q), 0)

#define SQueueLast(mBuf, q) \
     ((q) == NULLP ? RFAILED : SAddQueue(mBuf, (q), ((Queue *)(q))->crntSize))

#define SDequeueLast(mBuf, q) \
     ((q) == NULLP ? RFAILED :                          \
      (((Queue *)(q))->crntSize == 0 ? ROKDNA : SRemQueue(mBuf, q, \
                                               (((Queue *)q))->crntSize-1)))

/*
 * this depends on region and pool being the first two
 * parameters of ssMsgInfo
 */

#define SGetBufRegionPool(buf, region, pool)   \
        (((buf) == NULLP ||                                     \
          ((region) == NULLP && (pool) == NULLP)) ? RFAILED :   \
         ((region) == NULLP ||                                  \
          (*(region) = ((Region)*((Region *)((buf)->b_rptr))))),\
         ((pool) == NULLP   ||                                  \
          (*(pool) = ((Pool)*((Pool *)((buf)->b_rptr + sizeof(Region)))))),\
         ROK)

#endif /* SS_ENABLE_MACROS */

/* These are outside SS_ENABLE_MACROS for backward compatibility 
 *
 * redefine these to be different from default
 * these depend on region and pool being the first two
 * parameters of ssMsgInfo
 */

#undef SGETBUFREGION
#define SGETBUFREGION(buf)   \
         (((buf) == NULLP) ? REGIONNC : (Region)*((Region *)(buf)->b_rptr))

#undef SGETBUFPOOL
#define SGETBUFPOOL(buf)     \
         (((buf) == NULLP) ? POOLNC :  \
                           (Pool)*((Pool *)((buf)->b_rptr + sizeof(Region))))

/*ssi_h_001.main_147 ss_dep.h guarded under flag*/
#ifdef SS_CAVIUM
#include "jz_dep.h"
#else
#ifdef SS_4GMX_LCORE
#include "ss_dep.h"
#else
#include "ss_dep.h"
#endif /* SS_4GMX_LCORE*/
#endif /*SS_CAVIUM*/

#else   /* Not SS */
#ifdef MT                         /* mt version */

/* mtworks defines */
#define MT_MBUF_POOL 3            /* message header pool id */
#define MT_TMR_POOL  2            /* timer pool id */
#define MT_WRAP_POOL 1            /* wrapper pool id */
#define MT_PROT_POOL 0            /* protocol pool id */

#define DFLT_REGION 0
#define DFLT_POOL MT_PROT_POOL    /* protocol data pool */

#define OWNREGION DFLT_REGION     /* own region (bc, use DFLT_REGION) */ 

/* thread types */
#define MT_THR_NOFLAGS   0x00
#define MT_THR_SUSPENDED 0x01
#define MT_THR_DETACHED  0x02
#define MT_THR_BOUND     0x04
#define MT_THR_NEW_LWP   0x08
#define MT_THR_DAEMON    0x10

/* thread priorities */
#define MT_LOW_PRI          0
#define MT_NORM_PRI         10
#define MT_HIGH_PRI         20

#else /* not MT */
#ifdef WINNT_IATM
/* WINT defines */
#define DFLT_REGION 0
#define DFLT_POOL 0
#ifdef SS_FAP
/*ssi_h_001.main_145 - Defined new region for FAP*/
#define SS_WL_REGION 1
#endif

#define PHY_CONTIG_REGION 1

#define OWNREGION DFLT_REGION     /* own region */ 
#else /* WINNT_IATM */
/* portable/other defines */
#define DFLT_REGION 0             /* required by portable */
#define DFLT_POOL 0               /* required by portable */
#ifdef SS_FAP
/*ssi_h_001.main_145 - Defined new region for FAP*/
#define SS_WL_REGION 1
#endif
#define OWNREGION DFLT_REGION     /* own region */ 

/* lock data types */
#define SsSemaId                        U8
#define SLockId                         U8
/* ssi_h_001.main_143: Additions */
#define SLockInfo                       U8

/* basic lock types */
#define SS_LOCK_MUTEX                   1
#define SS_LOCK_SPIN                    2
#define SS_LOCK_CRITSEC                 3

#endif /* WINNT_IATM */
#endif /* MT */
#endif /* SS */
#endif /* SSLIB || SSRYLIB*/
#endif /* VW */
#endif /* VX */
#endif /* PS */
#endif /* MSVER2 */
#endif /* MOSVER */

/* Macros for REGION1 and REGION2 support */
#define SS_REGION1              1
#define SS_REGION2              2

/* Time in seconds from UTC EPOCH */
#define SS_REFTIME_01_01_1970   0
#define SS_REFTIME_01_01_2002   1009843200

/* Ticks per second */
#define SS_100MS                         10
#define SS_10MS                          100
#define SS_1MS                           1000

/*ssi_h_001.main_145-definednew value for SS_TICS_SEC for FAP*/
#ifndef SS_FAP
#ifndef SS_TICKS_SEC
/* ssi_h_001.main_152: Modified SS_TICKS_SEC value for 4GMX */
#ifdef SS_4GMX_LCORE
#define SS_TICKS_SEC                     SS_1MS
#else
#define SS_TICKS_SEC                     SS_100MS
#endif /*SS_4GMX_LCORE*/
#endif
#else
#ifndef SS_TICKS_SEC
#define SS_TICKS_SEC                     SS_1MS
#endif
#endif

/* procId added */ 
/* ssi_h_001.main_127 Replaced SS_100MS with SS_TICKS_SEC in SRegCfgTmr/SDeregCfgTmr */
#ifndef SS_MULTIPLE_PROCS

/* ssi_h_001.main_146 */
#ifndef SS_FAP
#define SRegTmr(ent,inst,period,fun)     SRegCfgTmr(ent,inst,period,SS_TICKS_SEC,fun)
#define SDeregTmr(ent,inst,period,fun)   SDeregCfgTmr(ent,inst,period,SS_TICKS_SEC,fun)
#else /* SS_FAP */
#define SRegTmr(ent,inst,period,fun)     SRegCfgTmr(ent,inst,period,SS_10MS,fun)
#define SDeregTmr(ent,inst,period,fun)   SDeregCfgTmr(ent,inst,period,SS_10MS,fun)
#endif 

/* ssi_h_001.main_131 */
#ifdef SS_MT_TMR
/* ssi_h_001.main_126 */
#define SRegTmrMt(ent,inst,period,fun)     SRegCfgTmrMt(ent,inst,period,SS_TICKS_SEC,fun)
#define SDeregTmrMt(ent,inst,period,fun)   SDeregCfgTmrMt(ent,inst,period,SS_TICKS_SEC,fun)
#endif

#else

/* ssi_h_001.main_146 */
#ifndef SS_FAP
#define SRegTmr(proc, ent,inst,period,fun)\
        SRegCfgTmr(proc, ent,inst,period,SS_TICKS_SEC,fun)
#define SDeregTmr(proc, ent,inst,period,fun)\
        SDeregCfgTmr(proc, ent,inst,period,SS_TICKS_SEC,fun)
#else /* SS_FAP */
#define SRegTmr(proc, ent,inst,period,fun)\
        SRegCfgTmr(proc, ent,inst,period,SS_100MS,fun)
#define SDeregTmr(proc, ent,inst,period,fun)\
        SDeregCfgTmr(proc, ent,inst,period,SS_100MS,fun)
#endif

#endif /* SS_MULTIPLE_PROCS */

/* RELAY ONLY DEFINES */
#ifdef ENB_RELAY

#define RY_SHM_KEY   0x3000
#define RY_SHM_PERMS 0666

#define RY_MSG_KEY   0x2000
#define RY_MSG_PERMS 0666

#define RY_BASECHANID 0

#ifdef INTNT_PMC860

/* 
 * Processor Id map for the Integrated Windows NT platform 
 *
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |A A A A S S S R R I I I I I I I| 
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * A - Application Id
 * S - Stack Id
 * R - Reserved
 * I - IPM Id
 *
 */

/* processor id masks */
#define APP_MSK 0xf
#define STK_MSK 0x7
#define BUS_MSK 0x0
#define VCM_MSK 0x0
#define IPM_MSK 0x7f
#define BVM_MSK 0x0

/* processor id  offsets */
#define APP_OFF 12
#define STK_OFF 9
#define BUS_OFF 0
#define VCM_OFF 0
#define IPM_OFF 0
#define BVM_OFF 0
#else
/* processor id masks */
#define APP_MSK 0xf
#define STK_MSK 0x7
#define BUS_MSK 0x3
#define VCM_MSK 0xf
#define IPM_MSK 0x7
#define BVM_MSK 0x3f

/* processor id  offsets */
#define APP_OFF 12
#define STK_OFF 9
#define BUS_OFF 7
#define VCM_OFF 3
#define IPM_OFF 0
#define BVM_OFF 3
#endif /* INTNT_PMC860 */

/* set processor id macros */
#define PID_APP(a)          ((a) << APP_OFF)
#define PID_STK(h)          ((h) << STK_OFF)
#define PID_B_V(b, v)       (((b) << BUS_OFF) | ((v) << VCM_OFF))
#define PID_BV(bv)          ((bv) << BVM_OFF)
#define PID_B_V_I(b, v, i)  (((b) << BUS_OFF) | ((v) << VCM_OFF) | (i) )
#define PID_BV_I(bv, i)     (((bv) << BVM_OFF) | (i))

/* get processor id macros */
#define GET_APP_ID(pid)   (((pid) & (APP_MSK << APP_OFF)) >> APP_OFF)
#define GET_STK_ID(pid)   (((pid) & (STK_MSK << STK_OFF)) >> STK_OFF)
#define GET_BUS_ID(pid)   (((pid) & (BUS_MSK << BUS_OFF)) >> BUS_OFF)
#define GET_VCM_ID(pid)   (((pid) & (VCM_MSK << VCM_OFF)) >> VCM_OFF)
#define GET_IPM_ID(pid)   ((pid) & IPM_MSK)
#define GET_BV_ID(pid)    (((pid) & (BVM_MSK << BVM_OFF)) >> BVM_OFF)

#ifdef ELVIS_IMP
#define MAXBRDS 16         /* 16 MVME's */
#endif /* ELVIS_IMP */

#ifdef ELVIS_BIT3
#define MAXBRDS 64         /* 16*4 MVME's */
#endif /* ELVIS_BIT3 */

#ifdef ELVIS_FORCE
#define MAXBRDS 16         /* 16 MVME's or Power Core's */
#endif /* ELVIS_FORCE */

#ifdef ELVIS_68040
#define MAXBRDS  4         /* 4 IPM's */
#endif /* ELVIS_68040 */

#ifdef ELVIS_PPC60X
#define MAXBRDS 2          /* 2 PMCS's Maximum */
#endif /* ELVIS_PPC60X */

#ifdef INTNT_PMC860
#define MAXBRDS 64         /* Maximum number of PMCS in system */
#endif /* ELVIS_PPC60X */

#else /* not ENB_RELAY */
#define PID_STK(h)        (h)
#endif /* ENB_RELAY */

/* Inter CPU messages */
#define SS_ICPU_DATA       6
/*ssi_h_001.main_133*/
/*ssi_h_001.main_151 ADDED SS_AFFINITY_SUPPORT*/
#if defined(SS_MULTICORE_SUPPORT) || defined(SS_AFFINITY_SUPPORT)
/* CPU affinity modes */
#define SS_AFFINITY_MODE_DEFAULT 0
#define SS_AFFINITY_MODE_SPECIFIC 1
#define SS_AFFINITY_MODE_ASSOC 2
#define SS_AFFINITY_MODE_EXCL 3

#define SS_MAX_CORES 64
#define SS_MAX_THREADS_PER_CORE 16
#define SS_DEFAULT_CORE ~0

#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT*/

/* ssi_h_001.main_136: SSI-Phse-2, Added defines for memory 
 * allocation/de-allocation from static memory pool 
 */
#ifdef SS_HISTOGRAM_SUPPORT 
#define SGetSBuf(region, pool, bufPtr, size) SGetSBufNew(region,pool,bufPtr,size, __LINE__, (U8*)__FILE__)
#define SPutSBuf(region, pool, buf, size) SPutSBufNew(region, pool, buf, size, __LINE__, (U8*)__FILE__)
#define SGetMsg(region, pool, mBufPtr) SGetMsgNew(region, pool, mBufPtr, __LINE__,  (U8*)__FILE__)
#define SPutMsg(mBuf) SPutMsgNew(mBuf, __LINE__,  (U8*)__FILE__)
#elif defined(SS_LIGHT_MEM_LEAK_STS)
#define SGetSBuf(region, pool, bufPtr, size) SGetSBufNew(region,pool,bufPtr,size, __LINE__, (U8*)__func__)
#define SGetMsg(region, pool, mBufPtr) SGetMsgNew(region, pool, mBufPtr, __LINE__, (U8*)__func__)
#define SGetDBuf(region, pool, bufPtr) SGetDBufNew(region,pool,bufPtr, __LINE__, (U8*)__func__)
#define SPutSBuf(region, pool, buf, size) SPutSBufNew(region, pool, buf, size, __LINE__, (U8 *)__func__)
#define SPutMsg(mBuf) SPutMsgNew(mBuf, __LINE__,  (U8*)__func__)
#define SPutDBuf(region, pool, buf) SPutDBufNew(region, pool, buf, __LINE__, (U8*)__func__)
#endif /* SS_HISTOGRAM_SUPPORT */
/* ssi_h_001.main_142 Readwrite additions */
/* ssi_h_001.main_144 Readwrite locks Guarded */
#ifdef SS_RDWR_LOCK_SUPPORT
#define SRDWRLOCK 1
#define SRDLOCK 2
#define SWRLOCK 3
#define STRYRDLOCK 4
#define STRYWRLOCK 5
#endif /* SS_RDWR_LOCK_SUPPORT */
#ifdef SS_REC_LOCK_SUPPORT
#define SMUTEXRECUR 6
#endif /* SS_REC_LOCK_SUPPORT */

/* The below range from 30001 - 30099 is for 4GMX SSI and common */
#ifndef TENB_T2K3K_SPECIFIC_CHANGES
#define PID_SVSRALLOC_ICMSG           30001
#define PID_SVSRSEND_ICMSG            30002
#define PID_SVSRALLOC_MSG             30003
#define PID_SSI_CORE3                 30004
#define PID_SSI_CORE2                 30005
#define PID_SSI_ICORE_MSG             30006
#define PID_SSI_ICPU_MSG              30007
#else
#define PID_SSI_TSK_L2                30005
#define PID_PHY_RCVR_THRD             30005
#define PID_SSI_TSK                   30007
#define PID_HI_TSK                   30331
#define PID_SB_TSK                   30332
#define PID_WR_TSK                   30333
#define PID_S1_TSK                   30334
#define PID_EG_TSK                   30335
#define PID_SVSRALLOC_MSG             30003
#endif /* TENB_T2K3K_SPECIFIC_CHANGES */
/* The below range from 30101 - 30199 is for CL */
#define PID_CRC_IND_DUMMY             30101
#define PID_CRC_IND_REAL              30102
#define PID_CL_CNTRL_REQ_DL           30103
#define PID_CL_DAT_REQ                30104
#define PID_CL_RECP_REQ               30105
#define PID_CL_SEND_TO_PHY            30106
#define PID_CL_RXEND_IND              30107
#define PID_CL_PHY_MSG                30108
#define PID_CL_RXSDU_IND              30109
#define PID_CL_RXSDUANDEND_IND        30110
#define PID_CL_FILL_DLCHNL_DESC       30111
#define PID_CL_FILL_TXSDU             30112
#define PID_CL_SND_VECT_TO_PHY        30113
#define PID_CL_PHICH_DAT_REQ          30114
#define PID_CL_PDCCH_DAT_REQ          30115
#define PID_CL_PDSCH_DAT_REQ          30116
#define PID_CL_PBCH_DAT_REQ           30117
#define PID_CL_MAC_TTI_IND            30118
#define PID_CL_CNTRL_REQ_UL           30119
#define PID_CL_SPACC_BATCH_UL_BLK     30120
#define PID_CL_SPACC_BATCH_DL_BLK     30121




/* The below range from 30200 - 30299 is for MAC/RLC/PDCP */
#define PID_SCH_TTI_IND               30200
#define PID_MAC_DAT_IND               30201
#define PID_MAC_SF_ALLOC_REQ          30202
#define PID_MAC_STA_RSP               30203
#define PID_MAC_DL_SCHD               30204
#define PID_MAC_DL_CQI_IND            30205
#define PID_MAC_UL_CQI_IND            30206
#define PID_MAC_SR_IND                30207
#define PID_MAC_HARQ_IND              30208
#define PID_MAC_TA_IND                30209
#define PID_MAC_UL_SCHD               30210
#define PID_MAC_TTI_IND               30211
#define PID_MAC_UE_CFG                30212
#define PID_MAC_UE_RECFG              30213
#define PID_MAC_UE_DEL                30214
#define PID_MAC_UE_RST                30215
#define PID_SCH_UE_CFG                30216
#define PID_SCH_UE_RECFG              30217
#define PID_SCH_UE_DEL                30218
#define PID_SCH_UE_RST                30219
#define PID_MAC_DDATREQ               30220
#define PID_MAC_FILLDREQ              30221
#define PID_MAC_TFUDREQ               30222
#define PID_SCHUTL_CMALLCEVT          30223
#define PID_TOMUTL_CMALLCEVT          30224
#define PID_TOMINF_CMALLCEVT          30225
#define PID_MACUTL_CMALLCEVT          30226
#define PID_SCH_DUMMY_TTI_IND         30227
#define PID_MAC_TFU_DATIND            30228
#define PID_UL_SCHEDULING             30229  /* UL scheduling: CRC indication */
#define PID_ysMsPrcUlSchSduInd        30230
#define PID_CL_RCV_PHY_MSG            30231
#define PID_CL_HARQ_STA_IND           30232 
#define PID_CL_DL_BATCH_PROC          30233
#define PID_CL_DLM_PRC_TTI_IND        30234
#define PID_TTI_LATENCY               30235
#define PID_RECPREQ_PROC              30236
#define PID_MAC_UL_SRS_IND            30237

#define PID_RLC_STA_IND               30240
#define PID_RLC_REASSEMBLY            30241
#define PID_RLC_AM_SELF_PST_DL        30242
#define PID_RLC_AM_PDU_RLS            30243
#define PID_RLC_CHK_SDU_MAP           30244
#define PID_RLC_SND_DAT_CFM           30245
#define PID_RLC_AM_SELF_PDU_RSMBL     30246
#define PID_RLC_AM_RGU_DATIND         30247
#define PID_RLC_AM_ADDTOLIST_UL       30248
#define PID_RLC_AM_FREEBUF            30249
#define PID_RLC_AM_RETXLIST           30250
#define PID_RLC_AM_POST_ULSELFMSG     30251
#define PID_RLC_AM_UTL_RSMBL          30252
#define PID_RLC_AM_STA_PDU            30253
#define PID_RLC_CFG_REQ               30254
#define PID_RLC_CFG_REQ_REEST         30255
#define PID_RLC_CFG_REQ_REM           30256
#define PID_RLC_AM_RLSPDUSET          30257
#define PID_RLC_AM_RB_DELETE          30258
#define PID_RLC_AM_REEST              30259
#define PID_RLC_CFG_UE_DELETE         30260
#ifndef BRDCM
#define PID_RLC_STA_IND_MEMSET        30261
#define PID_RLC_STA_IND_PRC           30262
#define PID_RLC_KWUDAT_IND            30263
#endif
#define PID_RLC_UM_REEST              30264
#define PID_RLC_AM_RLC_UE_DEL         30260
#define PID_RLC_AM_QSDU               30261
#define PID_RLC_TM_CSTAIND_SUCCESS    30265
#define PID_RLC_TM_CSTAIND_FAIL       30266
#define PID_RLC_TM_SEARCH_RNTI        30267
#define PID_RLC_TM_QSDU               30268

/* UL IP THROUGHPUT PROFILING */
#define PID_RLC_IP_TPT_INCTTI         30269
#define PID_RLC_IP_TPT_INCVOL         30270
#define PID_RLC_MEAS_REPORT           30271
#define PID_RLC_MEAS_START            30272
#define PID_RLC_MEAS_STOP             30273
/* Discard SDU Profiling */
#define PID_RLC_AMM_REASSEMBLE_SDUS   30274
#define PID_RLC_AMM_DISC_SDUS         30275
#define PID_RLC_UMM_REASSEMBLE_SDUS   30276
#define PID_RLC_UMM_DISC_SDUS         30277

/* DL IP THROUGHPUT PROFILING */
#define PID_RLC_DLIP_TPT_BURSTCALC    30278
#define PID_RLC_DLIP_TPT_PRCHARQIND   30279

#define PID_MT_AM_SVSR_HDLR           30008
#define PID_MT_AM_SVS_MSG             30009
#define PID_MAC_AM_HARQ_RLS           30010
#define PID_PDCP_DL_PKT               30280
#define PID_PDCP_DAT_IND              30281
#define PID_PDCP_CFG_REQ              30282
#define PID_PDCP_CFG_REQ_REEST        30283
#define PID_PDCP_CFG_REQ_REM          30284
#define PID_PDCP_CFG_UE_DELETE        30285

#define PID_CM_PRC_TMR                30286

/* The below range is for detailed analysis of common functions */
#define PID_SAddPstMsgMult    30600
#define PID_SAddPreMsgMult    30601
#define PID_SRemPreMsgMult    30602
#define PID_SRemPstMsgMult    30603
#define PID_SGetDataFrmMsg    30604
#define PID_SSegMsg           30605
#define PID_SCpyFixMsg        30607
#define PID_SCpyMsgFix        30608
#define PID_SCpyMsgMsg        30610
#define PID_SAddMsgRef        30609
#define PID_SCatMsg           30611
#define PID_SCompressMsg      30612
#define PID_SGetPstMsgMult    30613
#define PID_SSwapMsg          30614
#define PID_SCpyPartMsg       30615
#define PID_SRepPartMsg       30616
#define PID_SPkMsgMult        30617
#define PID_SMovPartMsg       30618

#define PID_L2_PROCESSING     30998  /* Processing for all L2 tasks on CPU3 */

/* This captures all the processing invoked through PHY */
#define PID_PHY_RCV_PROC              30999

#ifdef MSPD
#ifdef SS_4GMX_LCORE
#define RB_GET_CALLER() (__return_address()-4)
#define RB_GET_SP() (__current_sp())
#define RB_TRC() rbCallstackPush(RB_GET_CALLER(), RB_GET_SP())
#define RB_TRC_RET()  rbCallstackPop();
#endif

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#ifndef XEON_SPECIFIC_CHANGES
#include "tl.h"
#else
#include "mlog_com.h"
#endif

#define RESOURCE_LINL2 RESOURCE_L2
#define RESOURCE_LARM  RESOURCE_L2
#ifndef XEON_SPECIFIC_CHANGES
#define GetTIMETICK()  TL_MLOG_TICK
#else
#define GetTIMETICK() 0
#endif
#ifdef PHY_SHUTDOWN_ENABLE
#define stop_printf(...) do {EXTERN U8 sndPhyShutDwn;printf(__VA_ARGS__); mtStopHndlr(); sndPhyShutDwn = 1;} while (0)
#else
#define stop_printf(...) do {printf(__VA_ARGS__); mtStopHndlr(); exit(-1);} while (0)
#endif
#define uart_printf(...) printf(__VA_ARGS__)
#ifdef XEON_SPECIFIC_CHANGES
#define MLogTask(a, b, c, d)  tlMlogTask(a, c, d)
#else
#define MLogTask  tlMlogTask
#endif
#define MLogAddVariables(a, b, c)
#define rbCallstackShow()
#define Ad9361RadioInit(a,b,c,d) 0

#ifndef CA_PHY
#define SS_MLOG_BUF_SIZE        1591328 
#else
#define SS_MLOG_BUF_SIZE        7175200 
#endif
#define SS_MEMLOG_MAXSTRLEN     256

#ifdef MS_MUBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
#define MS_BUF_ADD_CALLER()\
{\
   extern U32 cmFreeCaller[4];\
   if(cmFreeCaller[MxGetCpuID()] == NULLP)\
      cmFreeCaller[MxGetCpuID()] = __return_address()-4;\
}
#endif /* */
#endif
#endif/* MSPD */

#ifdef BRDCM
extern char logBuf[100];
extern int  glbTime;
#define BRDCM_PROF(s) do {\
} while(0) 

#define PID_CRC_IND_START                   3000
#define PID_CRC_IND_END                     3001
#define PID_CL_CNTRL_REQ_START              3002
#define PID_CL_CNTRL_REQ_END                3003
#define PID_CL_DAT_REQ_START                3004
#define PID_CL_DAT_REQ_END                  3005
#define PID_CL_RECP_REQ_START               3006
#define PID_CL_RECP_REQ_END                 3007
#define PID_CL_SEND_TO_PHY_START            3008
#define PID_CL_SEND_TO_PHY_END              3009
#define PID_CL_RXSDU_IND_START              3010
#define PID_CL_RXSDU_IND_END                3011
#define PID_MAC_TTI_IND_START               3400
#define PID_MAC_TTI_IND_END                 3401
#define PID_SCH_TTI_IND_START               3402
#define PID_SCH_TTI_IND_END                 3403
#define PID_MAC_DAT_IND_START               3404
#define PID_MAC_DAT_IND_END                 3405
#define PID_MAC_SF_ALLOC_REQ_START          3406
#define PID_MAC_SF_ALLOC_REQ_END            3407
#define PID_MAC_STA_RSP_START               3408
#define PID_MAC_STA_RSP_END                 3409
#define PID_MAC_CMN_STA_RSP_START           3410
#define PID_MAC_CMN_STA_RSP_END             3411
#define PID_MAC_DL_SCHD_START               3412
#define PID_MAC_DL_SCHD_END                 3413
#define PID_MAC_DL_CQI_IND            30205
#define PID_MAC_UL_CQI_IND            30206
#define PID_MAC_SR_IND                30207
#define PID_MAC_HARQ_IND              30208
#define PID_MAC_TA_IND                30209
#define PID_MAC_UL_SCHD               30210
#define PID_MAC_TTI_IND               30211
#define PID_MAC_UE_CFG                30212
#define PID_MAC_UE_RECFG              30213
#define PID_MAC_UE_DEL                30214
#define PID_MAC_UE_RST                30215
#define PID_SCH_UE_CFG                30216
#define PID_SCH_UE_RECFG              30217
#define PID_SCH_UE_DEL                30218
#define PID_SCH_UE_RST                30219
#define PID_MAC_DDATREQ               30220
#define PID_MAC_FILLDREQ              30221
#define PID_MAC_TFUDREQ               30222
#define PID_SCHUTL_CMALLCEVT          30223
#define PID_TOMUTL_CMALLCEVT          30224
#define PID_TOMINF_CMALLCEVT          30225
#define PID_MACUTL_CMALLCEVT          30226
#define PID_SCH_DUMMY_TTI_IND         30227
#define PID_MAC_TFU_DATIND            30228
#define PID_RLC_STA_IND               30240
#define PID_RLC_REASSEMBLY            30241
#define PID_RLC_AM_SELF_PST_DL        30242
#define PID_RLC_AM_PDU_RLS            30243
#define PID_RLC_CHK_SDU_MAP           30244
#define PID_RLC_SND_DAT_CFM           30245
#define PID_RLC_AM_SELF_PDU_RSMBL     30246
#define PID_RLC_AM_RGU_DATIND         30247
#define PID_RLC_AM_ADDTOLIST_UL       30248
#define PID_RLC_AM_FREEBUF            30249
#define PID_RLC_AM_RETXLIST           30250
#define PID_RLC_AM_POST_ULSELFMSG     30251
#define PID_RLC_AM_UTL_RSMBL          30252
#define PID_RLC_AM_STA_PDU            30253
#define PID_RLC_CFG_REQ               30254
#define PID_RLC_CFG_REQ_REEST         30255
#define PID_RLC_CFG_REQ_REM           30256
#define PID_RLC_AM_RLSPDUSET          30257
#define PID_RLC_AM_RLSPDUSET_DATA     30258
#define PID_RLC_AM_RLSPDUSET_REEST    30259
#define PID_RLC_AM_RLC_UE_DEL         30260
#define PID_RLC_AM_QSDU               30261
#define PID_MT_AM_SVSR_HDLR           30008
#define PID_MT_AM_SVS_MSG             30009
#define PID_MAC_AM_HARQ_RLS           30010
#define PID_RLC_KWUDAT_IND            30012
#define PID_RLC_STA_IND_MEMSET        30014
#define PID_RLC_STA_IND_PRC           30015
#define PID_PDCP_DL_PKT               30280
#define PID_PDCP_DAT_IND              30281
#define PID_PDCP_CFG_REQ              30282
#define PID_PDCP_CFG_REQ_REEST        30283
#define PID_PDCP_CFG_REQ_REM          30284
#define PID_SAddPstMsgMult    30600
#define PID_SAddPreMsgMult    30601
#define PID_SRemPreMsgMult    30602
#define PID_SRemPstMsgMult    30603
#define PID_SGetDataFrmMsg    30604
#define PID_SSegMsg           30605
#define PID_SCpyFixMsg        30607
#define PID_SCpyMsgFix        30608
#define PID_SCpyMsgMsg        30610
#define PID_SAddMsgRef        30609
#define PID_SCatMsg           30611
#define PID_SCompressMsg      30612
#define PID_SGetPstMsgMult    30613
#define PID_SSwapMsg          30614
#define PID_SCpyPartMsg       30615
#define PID_SRepPartMsg       30616
#define PID_SPkMsgMult        30617
#define PID_SMovPartMsg       30618
#define PID_cmUnpkHitUDatInd_START           3300
#define PID_cmUnpkHitUDatInd_END             3301
#define PID_cmPkPjuDatReq_START              3310
#define PID_cmPkPjuDatReq_END                3311
#endif /* BRDCM */
#endif /* __SSIH__ */
/* ssi_h_001.main_120 */

/********************************************************************30**
  
         End of file
**********************************************************************/
