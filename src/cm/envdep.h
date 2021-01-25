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
  
     Name:     environment dependent
  
     Type:     C include file
  
     Desc:     Environment dependent defines required by the
               by TRILLIUM software. The defines in this file need to
               be changed by the customer to reflect the
               processor family under which the TRILLIUM software
               will run.
  
     File:     envdep.h
  
*********************************************************************21*/
#ifndef __ENVDEPH__
#define __ENVDEPH__

#ifdef __cplusplus
extern "C" {
#endif

/* envdep_h_001.main_70: Additions */
/* envdep_h_001.main_73: Not required for SUNOS */
#ifdef SS_LINUX
#ifndef _GNU_SOURCE
#include <stdint.h>
#define __USE_UNIX98
#endif /* _GNU_SOURCE */
#endif /* SS_LINUX */
/*envdep_h_001.main_75 : RMIOS related changes*/
#ifdef SS_RMIOS
#include "printk.h"
#endif


/* conversion of MSCDOS and MSCUNIX for backwards compatibility */

#ifdef MSCDOS              /* microsoft, dos */
#define ANSI               /* ansi */
#define DOS                /* dos */
#define MSC51              /* microsoft */
#else                      /* not microsoft, dos */
#ifdef MSCUNIX             /* microsoft, unix */
#define ANSI               /* ansi */
#define UNIX               /* unix */
#define MSC51              /* microsoft */
#else                      /* not microsoft, dos or unix */
#endif
#endif

/* ensure microsoft 7.0 backwards compatible with microsoft 5.1 */

#ifdef MSC70
#define MSC51
#endif

/* ensure microsoft 6.0 backwards compatible with microsoft 5.1 */

#ifdef MSC60
#define MSC51
#endif

/* default to sunos 5.1 for sunos */

#ifdef SUNOS
#define SUNOS51
#endif

/* ensure sunos 4.1 backwards compatible with sunos */

#ifdef SUNOS41
#ifndef SUNOS
#define SUNOS
#endif
#endif

/* ensure sunos 5.1 backwards compatible with sunos */

#ifdef SUNOS51
#ifndef SUNOS
#define SUNOS
#endif
#endif

/* ensure elvis backwards compatible with sunos */
#ifdef ELVIS_STACK
#ifndef SUNOS
#define SUNOS
#endif
#endif

#ifdef ELVIS_APP
#ifndef SUNOS
#define SUNOS
#endif
#endif

#ifdef POWER_STACK
#define UNIX
#define AIX
#define PROC_PPC
#endif /* POWER_STACK */

/* Fujitsu ALC evalutation board */
#ifdef ALC_EVAL              
#define PROC_68349
#define PROC_68XXX
#define SOLMRI43
#endif /* ALC_EVAL */

/* argument manipulation for ansi or non ansi function prototypes */

#ifdef ANSI                /* ansi */
#define ARGS(m)            m
#else                      /* non ansi */
#define ARGS(m)            ()
#endif

/* type definition for ansi or non ansi pointers */
#ifdef ANSI
typedef void *Ptr; /* ANSI vanilla pointer type definition */
#else
typedef char *Ptr; /* K&R vanilla pointer type definition */
#endif


/**********************************************************************
   memory models
**********************************************************************/

/*
   these defines are arranged first by environment (DOS, UNIX, SUNOS or
   embedded) followed by compiler (microsoft c, borland, unix or
   microtec c) as appropriate.
*/
/*envdep_h_001.main_77 SSI-4GMX specfic changes*/
#ifndef SS_4GMX_LCORE
/* envdep_h_001.main_76  Force definition of WIN32 when detecting WIN64 */
#if  defined(_WIN32) || defined(_WIN64)                     /* dos, nt win32 */
#ifndef WIN32
  #define WIN32
#endif
#endif
#endif

#ifdef  WIN32                     /* dos, nt win32 */
#define INTERRPT                    /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
 
#define M_I86CM                     /* Intel 80x86 compact model */
 
#define BIOSKEY _bios_keybrd
#define BIOSTOD _bios_timeofday
#define DOSKEEP _dos_keep
#define DOSGETDATE _dos_getdate
#define DOSGETTIME _dos_gettime
#define DOSDATE dosdate_t
#define DOSTIME dostime_t
#define DOSSIZE size_t
#define DOSENABLE _enable
#define DOSDISABLE _disable
#define DOSGETVECT _dos_getvect
#define DOSSETVECT _dos_setvect
#define DOSINT86 int86
#define DOSINT86X int86x
#define DOSMALLOC malloc
#define DOSEXIT _exit
#define NEAR _NEAR
 
#define KEYBRD_READY  _KEYBRD_READY
#define KEYBRD_READ   _KEYBRD_READ
#define TIME_GETCLOCK _TIME_GETCLOCK
#endif /* WIN32 */
 
/*envdep_h_001.main_75 : RMIOS related changes*/
#ifdef SS_RMIOS
#define INTERRPT                 /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#endif

#if (defined(VXWORKS_PENTIUM) || defined(VXWORKS_DIAB))
#define INTERRPT                    /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */

#define M_I86CM                     /* Intel 80x86 compact model */

#define BIOSKEY _bios_keybrd
#define BIOSTOD _bios_timeofday
#define DOSKEEP _dos_keep
#define DOSGETDATE _dos_getdate
#define DOSGETTIME _dos_gettime
#define DOSDATE dosdate_t
#define DOSTIME dostime_t
#define DOSSIZE size_t
#define DOSENABLE _enable
#define DOSDISABLE _disable
#define DOSGETVECT _dos_getvect
#define DOSSETVECT _dos_setvect
#define DOSINT86 int86
#define DOSINT86X int86x 
#define DOSMALLOC malloc
#define DOSEXIT _exit
#define NEAR _NEAR

#define KEYBRD_READY  _KEYBRD_READY
#define KEYBRD_READ   _KEYBRD_READ
#define TIME_GETCLOCK _TIME_GETCLOCK
#endif /* VXWORKS_PENTIUM */


#ifdef DOS                        /* dos */
#ifdef MSC51                      /* dos, microsoft c */
#ifdef M_I86HM                    /* Intel 80x86 huge model */
#define INTERRPT _interrupt      /* interrupt */
#else
#define INTERRPT _interrupt far  /* interrupt */
#endif /* M_I86HM */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#define BIOSKEY _bios_keybrd
#define BIOSTOD _bios_timeofday
#define DOSGETDATE _dos_getdate
#define DOSGETTIME _dos_gettime
#define DOSDATE dosdate_t
#define DOSTIME dostime_t
#define DOSENABLE _enable
#define DOSDISABLE _disable
#define DOSGETVECT _dos_getvect
#define DOSSETVECT _dos_setvect
#define DOSEXIT _exit
#define NEAR _NEAR
#define KEYBRD_READY  _KEYBRD_READY
#define KEYBRD_READ   _KEYBRD_READ
#define TIME_GETCLOCK _TIME_GETCLOCK

#ifdef M_I86SM                    /* Intel 80x86 small model */
#define I86SM
#endif /* M_I86SM */

#ifdef M_I86MM                    /* Intel 80x86 medium model */
#define I86MM
#endif /* M_I86MM */

#ifdef M_I86CM                    /* Intel 80x86 compact model */
#define I86CM
#endif /* M_I86CM */

#ifdef M_I86LM                    /* Intel 80x86 large model */
#define I86LM
#endif /* M_I86LM */

#ifdef M_I86HM                    /* Intel 80x86 huge model */
#define I86HM
#endif /* M_I86HM */
#else  /* not MSC51*/ 
#ifdef BOR31                      /* dos, borland c */
#define INTERRPT _interrupt far  /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */

#define I86LM                     /* Intel 80x86 large model */

#define BIOSKEY _bios_keybrd
#define BIOSTOD _bios_timeofday
#define DOSKEEP _dos_keep
#define DOSGETDATE _dos_getdate
#define DOSGETTIME _dos_gettime
#define DOSDATE dosdate_t
#define DOSTIME dostime_t
#define DOSSIZE size_t
#define DOSENABLE _enable
#define DOSDISABLE _disable
#define DOSGETVECT _dos_getvect
#define DOSSETVECT _dos_setvect
#define DOSINT86 int86
#define DOSINT86X int86x
#define DOSMALLOC malloc
#define DOSEXIT _exit
#define NEAR _NEAR

#define KEYBRD_READY  _KEYBRD_READY
#define KEYBRD_READ   _KEYBRD_READ
#define TIME_GETCLOCK _TIME_GETCLOCK
#else  /* not BOR31 */
#endif /* BOR31 */
#endif /* MSC51 */
#else  /* not DOS */
#ifdef UNIX                       /* unix */
#define INTERRPT                 /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */


#ifdef __alpha                    /* alpha */
#define ALPHA
#undef OTHMOD
#endif /* __alpha */
#else /* not UNIX */
#ifdef SUNOS                      /* sun os */
#define PROC_SPARC                /* sparc model */
#define UNIX                      /* unix */

#define INTERRPT                 /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#else /* not SUNOS */
#ifdef HPOS                       /* HP os */
#define PROC_PARISC               /* HP PA model */
#define UNIX                      /* unix */

#define INTERRPT                  /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop enviroment */
#else /* not HPOS */
#ifdef MOTADS                     /* embedded, motorola ads */
#define PROC_68302                /* enable 68302 specific defines */
#define M68                       /* motorola 68000 */

#ifdef DOSMRI42D                  /* DOS microtec c cross-compiler */
#define MRI68K                    /* microtec 68K c compiler */
#define INTERRPT interrupt       /* interrupt */
#define ASM asm                   /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push environment */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop environment */
#endif /* DOSMRI42D */

#ifdef SOLMRI43                   /* solaris microtec c cross-compiler */
#define MRI68K                    /* microtec 68K c compiler */
#define INTERRPT interrupt       /* interrupt */
#define ASM asm                   /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push environment */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop environment */
#endif /* SOLMRI43 */

#ifdef SOLOAS186                  /* solaris oasys c cross-compiler */
#define INTERRPT                 /* interrupt */
#ifdef ANSI
#define ASM __asm                 /* in-line assembly code */
#else /* not ANSI */
#define ASM asm                   /* in-line assembly code */
#endif /* ANSI */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#endif /* SOLOAS186 */

#else /* not MOTADS */
#ifdef ELVIS                      /* elvis specific (not sun os) */
#define M68                       /* motorola 68000 */

#ifdef DOSMRI42D                  /* DOS microtec c cross-compiler */
#define MRI68K                    /* microtec 68K c compiler */
#define INTERRPT interrupt       /* interrupt */
#define ASM asm                   /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push environment */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop environment */
#endif /* DOSMRI42D */

#ifdef SOLMRI43                   /* solaris microtec c cross-compiler */
#define MRI68K                    /* microtec 68K c compiler */
#define INTERRPT interrupt       /* interrupt */
#define ASM asm                   /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push environment */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop environment */
#endif /* SOLMRI43 */

#ifdef SOLOAS186                  /* solaris oasys c cross-compiler */
#define INTERRPT                 /* interrupt */
#ifdef ANSI
#define ASM __asm                 /* in-line assembly code */
#else /* not ANSI */
#define ASM asm                   /* in-line assembly code */
#endif /* ANSI */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#endif /* SOLOAS186 */

#else /* not ELVIS */

#ifdef MSC51                      /* embedded, microsoft c */
#ifdef M_I86HM                    /* Intel 80x86 huge model */
#define INTERRPT _interrupt      /* interrupt */
#else /* not M_I86HM */
#define INTERRPT _interrupt far  /* interrupt */
#endif /* M_I86HM */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#define BIOSKEY _bios_keybrd
#define BIOSTOD _bios_timeofday
#define DOSGETDATE _dos_getdate
#define DOSGETTIME _dos_gettime
#define DOSDATE dosdate_t
#define DOSTIME dostime_t
#define DOSENABLE _enable
#define DOSDISABLE _disable
#define DOSGETVECT _dos_getvect
#define DOSSETVECT _dos_setvect
#define DOSEXIT _exit
#define NEAR _NEAR

#define KEYBRD_READY  _KEYBRD_READY
#define KEYBRD_READ   _KEYBRD_READ
#define TIME_GETCLOCK _TIME_GETCLOCK

#ifdef M_I86SM                    /* Intel 80x86 small model */
#define I86SM
#endif /* M_I86SM */

#ifdef M_I86MM                    /* Intel 80x86 medium model */
#define I86MM
#endif /* M_I86MM */

#ifdef M_I86CM                    /* Intel 80x86 compact model */
#define I86CM
#endif /* M_I86CM */

#ifdef M_I86LM                    /* Intel 80x86 large model */
#define I86LM
#endif /* M_I86CM */

#ifdef M_I86HM                    /* Intel 80x86 huge model */
#define I86HM
#endif /* M_I86HM */
#else  /* not MSC51 */
#ifdef BOR31                      /* embedded, borland c */
#define INTERRPT                 /* interrupt */
#define ASM                       /* in-line assembly code */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#define I86LM                     /* Intel 80x86 large model */

#define BIOSKEY _bios_keybrd
#define BIOSTOD _bios_timeofday
#define DOSKEEP _dos_keep
#define DOSGETDATE _dos_getdate
#define DOSGETTIME _dos_gettime
#define DOSDATE dosdate_t
#define DOSTIME dostime_t
#define DOSSIZE size_t
#define DOSENABLE _enable
#define DOSDISABLE _disable
#define DOSGETVECT _dos_getvect
#define DOSSETVECT _dos_setvect
#define DOSINT86 int86
#define DOSINT86X int86x
#define DOSMALLOC malloc
#define DOSEXIT _exit
#define NEAR _NEAR

#define KEYBRD_READY  _KEYBRD_READY
#define KEYBRD_READ   _KEYBRD_READ
#define TIME_GETCLOCK _TIME_GETCLOCK
#else  /* not BOR31 */
#ifdef DOSMRI42D                  /* dos microtec c cross-compiler */
#define MRI68K                    /* microtec 68K c compiler */
#define INTERRPT interrupt       /* interrupt */
#define ASM asm                   /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push environment */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop environment */
#ifdef _M68
#define M68                       /* motorola 68000 */
#endif /* _M68 */
#else  /* DOSMRI42D */
#ifdef SOLMRI43                   /* solaris microtec c cross-compiler */
#define MRI68K                    /* microtec 68K c compiler */
#define INTERRPT interrupt       /* interrupt */
#define ASM asm                   /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push environment */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop environment */
#ifdef _M68
#define M68                       /* motorola 68000 */
#endif /* _M68 */
#else  /* not SOLMRI43 */
#ifdef SOLOAS186                  /* solaris oasys c cross-compiler */
#define INTERRPT                 /* interrupt */
#ifdef ANSI
#define ASM __asm                 /* in-line assembly code */
#else /* not ANSI */
#define ASM asm                   /* in-line assembly code */
#endif /* ANSI */
#define ISR_PUSH_ENV              /* isr push environment */
#define ISR_POP_ENV               /* isr pop environment */
#ifdef m68k                       /* motorola 68000 series */
#define M68                       /* motorola 68000 */
#endif /* m68k */
#else  /* not SOLOAS186 */
#ifdef MOTDCI_68360
#define M68
#define PROC_68XXX
#define PROC_68360
#ifdef __GNUC__
#define INTERRPT
#define ASM __asm__                /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push environment */
#define ISR_POP_ENV                /* isr pop environment */
#else
#ifdef _MCC68K
#define INTERRPT interrupt
#define ASM asm                    /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push env */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop env */
#endif /* _MCC68K */
#endif /* __GNUCC__ */
#else
#ifdef QUICCADS
#define M68
#define PROC_68XXX
#define PROC_68360
#ifdef __GNUC__
#define INTERRPT
#define ASM __asm__                /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push environment */
#define ISR_POP_ENV                /* isr pop environment */
#else /* not gnu cc */
#ifdef _MCC68K
#define INTERRPT interrupt
#define ASM asm                    /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push env */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop env */
#endif /* _MCC68K */
#endif /* __GNUCC__ */
#else
#ifdef ALC_EVAL
#ifdef SOLMRI43
#define MRI68K                    /* microtec 68K c compiler */
#define INTERRPT interrupt        /* interrupt */
#define ASM asm                   /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push environment */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop environment */
#ifdef _M68
#define M68                       /* motorola 68000 */
#endif /* _M68 */
#endif /* SOLMRI43 */
#else
#ifdef GSIP302
#define M68
#define PROC_68XXX
#define PROC_68302
#ifdef __GNUC__
#define INTERRPT
#define ASM __asm__                /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push environment */
#define ISR_POP_ENV                /* isr pop environment */
#else /* not gnu cc */
#ifdef _MCC68K
#define INTERRPT interrupt
#define ASM asm                    /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push env */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop env */
#endif /* _MCC68K */
#endif /* __GNUCC__ */
#else  /* not GSIP302 */
#ifdef VB_360
#define M68
#define PROC_68XXX
#define PROC_68360
#ifdef __GNUC__
#define INTERRPT
#define ASM __asm__                /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push environment */
#define ISR_POP_ENV                /* isr pop environment */
#else
#ifdef _MCC68K
#define INTERRPT interrupt
#define ASM asm                    /* in-line assembly code */
#define ISR_PUSH_ENV ASM(" movem.l d2-d7/a2-a6,-(a7)") /* isr push env */
#define ISR_POP_ENV  ASM(" movem.l (a7)+,d2-d7/a2-a6") /* isr pop env */
#endif /* _MCC68K */
#endif /* __GNUCC__ */
#else  /* not VB_360 */
#ifdef PQ_PPC860                   /* Force PowerQUICC board */
#define PROC_PPC                   /* any PowerPC processor */
#define PROC_PPC860                /* MPC860 processor */
#ifdef __GNUC__                    /* gnu C compiler */
#define INTERRPT
#define ASM __asm__                /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push environment */
#define ISR_POP_ENV                /* isr pop environment */
#else
#ifdef _MCCPPC                     /* micortech C compiler */
#define INTERRPT
#define ASM asm                    /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push env */
#define ISR_POP_ENV                /* isr pop env */
#endif /* _MCC68K */
#endif /* __GNUCC__ */
#else  /* NOT PQ_PPC860 */
#ifdef PCORE
#define PROC_PPC
#define PROC_PPC604
#ifdef __GNUC__
#define INTERRPT
#define ASM __asm__                /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push environment */
#define ISR_POP_ENV                /* isr pop environment */
#else
#ifdef _MCCPPC
#define INTERRPT
#define ASM asm                    /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push env */
#define ISR_POP_ENV                /* isr pop env */
#endif /* _MCC68K */
#endif /* __GNUCC__ */
#else  /* PCORE */
#ifdef SS_PS                       /* pSOS RTOS */
#define INTERRPT 
#define ASM  __asm                 /* in-line assembly code */
#define ISR_PUSH_ENV               /* isr push env */
#define ISR_POP_ENV                /* isr pop env */
#else /* not SS_PS */
#define OTHMOD                     /* other model */
#endif /* SS_PS */
#endif /* PCORE */
#endif /* PQ_PPC860 */
#endif /* VB_360 */
#endif /* GSIP302 */
#endif /* ALC_EVAL */
#endif /* QUICCADS */
#endif /* MOTDCI_68360 */
#endif /* SOLOAS186 */
#endif /* SOLMRI43 */
#endif /* DOSMRI42D */
#endif /* BOR31 */
#endif /* MSC51 */
#endif /* ELVIS */
#endif /* MOTADS */
#endif /* HPOS */
#endif /* SUNOS */
#endif /* UNIX */
#endif /* DOS */

  
/**********************************************************************
   typedefs
**********************************************************************/

/*
   these typedefs are arranged first by environment (DOS, UNIX or
   embedded) followed by compiler (microsoft c, borland c, unix or
   microtec c) as appropriate.
*/

#ifdef DOS                        /* dos */
#ifdef MSC51                      /* dos, microsoft c */

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef int             S16;      /* signed - 16 bits */

typedef long            S32;      /* signed - 32 bits */
/* envdep_h_001.main_71 */
typedef __int64			 S64;     /* signed - 64 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

/* envdep_h_001.main_78 Change typedef void to #define void*/
#define Void void               

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (INTERRPT *PIF) ARGS((void ));     /* pointer to interrupt function */

#else /* not MSC51 */
#ifdef BOR31                      /* dos, borland c */

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef int             S16;      /* signed - 16 bits */

typedef long            S32;      /* signed - 32 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void Void;                /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (INTERRPT far *PIF) ARGS((void )); /* pointer to interrupt function */

#else /* not BOR31 */
#endif /* BOR31 */
#endif /* MSC51 */
#else  /* not DOS */
#ifdef UNIX                       /* unix */

typedef unsigned char   Bool;     /* boolean */
/*envdep_h_001.main_77 SSI-4GMX specfic changes*/
#if !(defined(SS_4GMX_LCORE) || defined(SS_4GMX_UCORE))
typedef char            S8;       /* signed - 8 bits */
#else
/*envdep_h_001.main_78 :removed signed to supress sprintf' differ in signedness warnings */
typedef char     S8;       /* signed - 8 bits*/
#endif

typedef short           S16;      /* signed - 16 bits */

#if !(defined(ALPHA) || defined(BIT_64))
typedef long            S32;      /* signed - 32 bits */
/* envdep_h_001.main_71 */
#ifdef SS_LINUX
/* envdep_h_001.main_79 RVDT compiler warning fix */
#ifndef SS_4GMX_LCORE
/* envdep_h_001.main_72: Support for C89 standard */
__extension__ typedef long long       S64;      /* signed - 64 bits */
#else
typedef long long       S64;      /* signed - 64 bits */
#define Void void /* RVDT compiler warning fix */
#endif /* SS_4GMX_LCORE */
#else
typedef long long       S64;      /* signed - 64 bits */
#endif /* SS_LINUX */
#else /* ALPHA  & BIT_64 */
typedef int             S32;      /* signed - 32 bits */
/* envdep_h_001.main_71 */
typedef long            S64;      /* signed - 64 bits */
#endif /* ALPHA  & BIT_64 */
/* 1. Added F32 and F64 */
#ifdef SS_FLOAT
typedef float           F32;      /* float - 32 bits */
typedef double          F64;      /* double - 64 bits */
#endif /* SS_FLOAT */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

#ifdef __cplusplus
#define Void void
#else
/* envdep_h_001.main_79 RVDT compiler warning fix */
#ifndef SS_4GMX_LCORE
typedef void Void;                /* void */
#endif
#endif

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning VOID */
typedef Void (INTERRPT *PIF) ARGS((void ));    /* pointer to interrupt function */

#else /* not UNIX */
#ifdef PROC_68302                 /* embedded, motorola ads */
#ifdef DOSMRI42D                  /* dos microtec c cross-compiler */

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */

typedef int             S32;      /* signed - 32 bits */

#ifndef ANSI
/* size_t is not defined, for some odd reason, in mcc68k's stddef.h */
typedef unsigned size_t;          /* size_t */
#endif /* not ANSI */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void  Void;               /* void */
typedef Void  VOID;               /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (*PIF) ARGS((void )); /* pointer to interrupt function */

#else /* not DOSMRI42D */
#ifdef SOLMRI43                   /* solaris microtec c cross-compiler */

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */

typedef int             S32;      /* signed - 32 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void  Void;               /* void */
typedef Void  VOID;               /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (*PIF) ARGS((void ));    /* pointer to interrupt function */

#else  /* not SOLMRI43 */
#ifdef SOLOAS186                  /* solaris oasys c cross-compiler */

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */

typedef int             S32;      /* signed - 32 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void  Void;               /* void */
typedef Void  VOID;               /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (INTERRPT *PIF) ARGS((void )); /* pointer to interrupt function */

#else /* not SOLOAS186 */
#ifdef __GNUC__    /* if  GNUC cross compiler */

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */

typedef int             S32;      /* signed - 32 bits */
/* envdep_h_001.main_71 */
typedef long long       S64;      /* signed - 64 bits */
/* 1. Added F32 and F64 types  */
#ifdef SS_FLOAT
typedef float           F32;      /* float - 32 bits */
typedef double          F64;      /* double - 64 bits */
#endif /* SS_FLOAT */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void  Void;               /* void */
typedef Void  VOID;               /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (INTERRPT *PIF) ARGS((void )); /* pointer to interrupt function */

#else /* not __GNUC__ */
#endif /* __GNUC */
#endif /* SOLOAS186 */
#endif /* SOLMRI43 */
#endif /* DOSMRI42D */
#else  /* not PROC_68302 */
#ifdef PROC_68040

#ifdef SOLMRI43                   /* solaris microtec c cross-compiler */
typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */

typedef int             S32;      /* signed - 32 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void  Void;               /* void */
typedef Void  VOID;               /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef INTERRPT Void (*PIF) ARGS((void)); /* pointer to interrupt function */
#endif /* SOLMRI43 */
#else /* not PROC_68040 */
#ifdef PROC_68360

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */

typedef long            S32;      /* signed - 32 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void Void;                /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef INTERRPT Void (*PIF) ARGS((void )); /* pointer to interrupt function */

#else /* not PROC_68360 */

#ifdef WIN32
typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */
 
typedef int             S32;      /* signed - 32 bits */
/* envdep_h_001.main_76  Take care of the windows compiler non compliance
 * to ANSI-C for data type ranges
*/ 
typedef long long          S64;      /* signed - 64 bits */

/* 1. Added F32 and F64 types  */
#ifdef SS_FLOAT
typedef float           F32;      /* float - 32 bits */
typedef double          F64;      /* double - 64 bits */
#endif /* SS_FLOAT */
/*
   void
 
   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/
 
typedef void Void;                /* void */
 
typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (INTERRPT *PIF) ARGS((void )); /* pointer to interrupt function */

#else /* not WIN32 */
#ifdef PROC_68349
typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef short           S16;      /* signed - 16 bits */

typedef long            S32;      /* signed - 32 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void Void;                /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef INTERRPT Void (*PIF) ARGS((void )); /* pointer to interrupt function */

#else  /* not PROC_68349 */

#ifdef PROC_PPC
typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */
 
typedef short           S16;      /* signed - 16 bits */
#ifndef BIT_64
typedef long            S32;      /* signed - 32 bits */

/* envdep_h_001.main_74 - additions */
typedef long long          S64;		/* signed - 64 bits */
#else /* BIT_64 */
typedef int             S32;      /* signed - 32 bits */
typedef long            S64;      /* signed - 64 bits */
#endif /* BIT_64 */

/* 1. Added F32 and F64 types  */
#ifdef SS_FLOAT
typedef float           F32;      /* float - 32 bits */
typedef double          F64;      /* double - 64 bits */
#endif /* SS_FLOAT */
/*
   void
 
   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void Void;                /* void */
 
typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef INTERRPT Void (*PIF) ARGS((void )); /* pointer to interrupt function */
 
#else /* not PROC_PPC */

#ifdef SS_PS
typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */
 
typedef short           S16;      /* signed - 16 bits */
 
#ifndef BIT_64
typedef long            S32;      /* signed - 32 bits */

/* envdep_h_001.main_71 - additions */
typedef int64_t         S64;		/* signed - 64 bits */
#else /* BIT_64 */
typedef int             S32;      /* signed - 32 bits */
typedef long            S64;      /* signed - 64 bits */
#endif /* BIT_64 */

/*
   void
 
   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/
 
typedef void Void;                /* void */
 
typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef Void (INTERRPT *PIF) ARGS((void )); /* pointer to interrupt function */
 
#else /* not SS_PS */

typedef unsigned char   Bool;     /* boolean */
typedef char            S8;       /* signed - 8 bits */

typedef int             S16;      /* signed - 16 bits */

typedef long            S32;      /* signed - 32 bits */

/*
   void

   note: if typedef Void is set to S16 then RETVOID must be set
   to: return(ROK). if typedef Void is set to void then RETVOID
   must be set to: return
*/

typedef void Void;                /* void */

typedef S8   (*PFS8) ARGS((void ));   /* pointer to function returning S8 */
typedef S16  (*PFS16) ARGS((void ));  /* pointer to function returning S16 */
typedef S32  (*PFS32) ARGS((void ));  /* pointer to function returning S32 */
typedef Void (*PFVOID) ARGS((void )); /* pointer to function returning Void */
typedef INTERRPT Void (*PIF) ARGS((void )); /* pointer to interrupt function */

#endif /* SS_PS */
#endif /* PROC_PPC */
#endif /* PROC_68349 */
#endif /* WIN32 */
#endif /* PROC_68360 */
#endif /* PROC_68040 */
#endif /* PROC_68302 */
#endif /* UNIX */
#endif /* DOS */


/* scope control keywords */
  
#ifdef PUBLIC
#undef PUBLIC
#define PUBLIC                    /* public is c default scope */
#else /* not PUBLIC */
#define PUBLIC
#endif /* PUBLIC */



/*envdep_h_001.main_77 SSI-4GMX specfic changes*/
/*envdep_h_001.main_78 : removed SSI-4GMX specfic changes*/



/**********************************************************************
   prototypes for sprintf
**********************************************************************/

/*envdep_h_001.main_75 : RMIOS related changes*/
#ifndef SS_RMIOS
#ifndef STDIO_INCLD

#ifdef SUNOS41
S8 *sprintf ARGS((S8 *buffer, const S8 *format, /* args */ ...));
#else
#ifdef SUNOS51
#else /* not SUNOS51 */
#ifdef HPOS 
int sprintf ARGS((S8 *s, const S8 *format, /* args */ ... ));
#else
#ifdef _MCC68K
Void sprintf ARGS((S8 *outbuf, S8 *fmt, /* args */ ...)); /* td68k.x */
#else
/* other os */
#ifdef WIN32
int sprintf ARGS((S8 *buffer,const S8 *format, /* args */ ...)); 
#else
#ifdef VW
int sprintf ARGS((S8 *buffer,const S8 *format, /* args */ ...)); 
#else
#ifdef SS_PS
int sprintf ARGS((S8 *buffer,const S8 *format, /* args */ ...)); 
#endif /* SS_PS */
#endif /* VW */
#endif /* WIN32 */
#endif /* _MCC68K */
#endif /* HPOS */
#endif /* SUNOS51 */
#endif /* SUNOS41 */
#endif /* STDIO_INCLD */
#endif /*SS_RMIOS*/


/**********************************************************************
   defines for pointers, pointer sizes, stack alignment and registers
**********************************************************************/

/*
   defines are provided for: intel 80x86 small, intel 80x86 medium,
   intel 80x86 compact, intel 80x86 large, intel huge, motorola 68000,
   sparc, vax and other processors.
*/
 
#ifdef I86SM          /* Intel 80x86 small model */

#define NULLP         0                /* null pointer */
#define NULLD         0L               /* null data */
#define PTR           uint16_t         /* pointer */
#define PTRSIZE       sizeof(uint16_t) /* pointer size */
#define STKALIGN      2           /* stack alignment */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3                      /* register 3 */
#define REG4                      /* register 4 */
#define REG5                      /* register 5 */
#define REG6                      /* register 6 */
#define REG7                      /* register 7 */
#define REG8                      /* register 8 - lowest priority */

#define FAR                       /* for small model */
#define PTRSTRIDE     1           /* stride */
#define MEM_SEG                   /* memory - segmented */
#endif /* I86SM */

#ifdef I86MM          /* Intel 80x86 medium model */

#define NULLP         0                /* null pointer */
#define NULLD         0L               /* null data */
#define PTR           uint16_t         /* pointer */
#define PTRSIZE       sizeof(uint16_t) /* pointer size */
#define STKALIGN      2           /* stack alignment */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3                      /* register 3 */
#define REG4                      /* register 4 */
#define REG5                      /* register 5 */
#define REG6                      /* register 6 */
#define REG7                      /* register 7 */
#define REG8                      /* register 8 - lowest priority */

#define FAR                       /* for medium model */
#define PTRSTRIDE     1           /* stride */
#define MEM_SEG                   /* memory - segmented */
#endif /* I86MM */

#ifdef I86CM          /* Intel 80x86 compact model */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      4           /* stack alignment */
#else /* BIT_64 */
#define PTR           uint64_t         /* pointer */
/* envdep_h_001.main_78:Warning fix*/
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      8           /* stack alignment */
#endif /* BIT_64 */


#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3                      /* register 3 */
#define REG4                      /* register 4 */
#define REG5                      /* register 5 */
#define REG6                      /* register 6 */
#define REG7                      /* register 7 */
#define REG8                      /* register 8 - lowest priority */

#define FAR           far         /* for compact model */
#define PTRSTRIDE     1           /* stride */
#define MEM_SEG                   /* memory - segmented */
#endif /* I86CM */


#ifdef I86LM          /* Intel 80x86 large model */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
#define PTRFAR                    /* pointer far */
#define STKALIGN      4           /* stack alignment */
#else /* BIT_64 */
#define PTR           uint64_t         /* pointer */
/* envdep_h_001.main_78:Warning fix*/
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      8           /* stack alignment */
#endif /* BIT_64 */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3                      /* register 3 */
#define REG4                      /* register 4 */
#define REG5                      /* register 5 */
#define REG6                      /* register 6 */
#define REG7                      /* register 7 */
#define REG8                      /* register 8 - lowest priority */

#define FAR           far         /* for large model */
#define PTRSTRIDE     1           /* stride */
#define MEM_SEG                   /* memory - segmented */
#endif /* I86LM */

#ifdef I86HM          /* Intel 80x86 huge model */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
#define PTRFAR                    /* pointer far */
#define STKALIGN      4           /* stack alignment */
#else  /* BIT_64 */
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
#define PTRFAR                    /* pointer far */
#define STKALIGN      8           /* stack alignment */
#endif  /* BIT_64 */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3                      /* register 3 */
#define REG4                      /* register 4 */
#define REG5                      /* register 5 */
#define REG6                      /* register 6 */
#define REG7                      /* register 7 */
#define REG8                      /* register 8 - lowest priority */

#define FAR           far         /* for huge model */
#define PTRSTRIDE     1           /* stride */
#define MEM_SEG                   /* memory - segmented */
#endif /* I86HM */

#ifdef PROC_68XXX                 /* Motorola 68040 model */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      4           /* stack alignment */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */

#ifdef PROC_68302
#define PTRSTRIDE     2           /* stride (simulation case: e.g. for elvis) */
#else
#ifdef PROC_68040
#define PTRSTRIDE     4           /* stride (simulation case: e.g. for elvis) */
#else /* other */
#define PTRSTRIDE     4           /* stride (simulation case: e.g. for elvis) */
#endif /* PROC_68040 */
#endif /* PROC_68302 */

#define MEM_NONSEG                /* memory - non-segmented */
#endif /* PROC_68XXX */


#ifdef PROC_SPARC                 /* SPARC */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      4           /* stack alignment */
#else  /* BIT_64 */
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      8           /* stack alignment */
#endif /* BIT_64 */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
/*envdep_h_001.main_77 SSI-4GMX specfic changes*/
#ifndef SS_4GMX_LCORE
#define REG8          register    /* register 8 - lowest priority */
#endif

#define FAR                       /* for large model */
#define PTRSTRIDE     4           /* stride */
#define MEM_NONSEG                /* memory - non-segmented */
#endif /* PROC_SPARC */
#ifdef PROC_PARISC                /* HP PA RISC */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      4           /* stack alignment */
#else
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      8           /* stack alignment */
#endif /* BIT_64 */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */
#define PTRSTRIDE     4           /* stride */
#define MEM_NONSEG                /* memory - non-segmented */
#endif /* PROC_PARISC */

/*envdep_h_001.main_75 : RMIOS related changes*/
#ifdef SS_RMIOS
#define SPtr2l(ptr)		((unsigned long)(ptr))
#define SPtr2ll(ptr)	((unsigned long long)(unsigned long)(ptr))
#define SL2Ptr(l)		((PTR)(l))
#define SLL2Ptr(ll)		((PTR)(ll & 0xFFFFFFFF))

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
#define STKALIGN      4           /* stack alignment */
#else  /* BIT_64 */
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
#define STKALIGN      8           /* stack alignment */
#endif /* BIT_64 */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */
#endif

#ifdef VAX            /* VAX */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      4           /* stack alignment */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */
#define PTRSTRIDE     2           /* stride */
#define MEM_NONSEG                /* memory - non-segmented */
#endif


#ifdef ALPHA          /* ALPHA */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      8           /* stack alignment */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */
#define PTRSTRIDE     2           /* stride */
#define MEM_NONSEG                /* memory - non-segmented */
#endif

#ifdef PROC_68360                 /* Motorola 68040 model */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      4           /* stack alignment */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */

#define PTRSTRIDE     4           /* stride (simulation case: e.g. for elvis) */
#define MEM_NONSEG                /* memory - non-segmented */
#endif /* PROC_68360 */


#ifdef PROC_PPC                   /* SPARC */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      4           /* stack alignment */
#else  /* BIT_64 */
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
/* #define PTRFAR                     pointer far */
#define STKALIGN      8           /* stack alignment */
#endif /* BIT_64 */

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */
#define PTRSTRIDE     4           /* stride */
#define MEM_NONSEG                /* memory - non-segmented */
#endif /* PROC_PPC */


#ifdef SS_PS                      /* 32-bit pSOS */
#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
#ifndef BIT_64
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
#define STKALIGN      4           /* stack alignment */
#else  /* BIT_64 */
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
#define STKALIGN      8           /* stack alignment */
#endif /* BIT_64 */


#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */
#define PTRSTRIDE     4           /* stride */
#endif /* SS_PS */


#ifdef OTHMOD         /* All other models */

#define NULLP         0L          /* null pointer */
#define NULLD         0L          /* null data */
/* envdep_h_001.main_76    WIN32 and WIN64 falls here, so be sure PTR is correctly defined
 * when compiling 64bits 
 */
/*envdep_h_001.main_77 SSI-4GMX specfic changes*/
#ifndef SS_4GMX_LCORE
#ifdef _WIN64
#define PTR           uint64_t         /* pointer */
#define PTRSIZE       sizeof(uint64_t) /* pointer size */
#define STKALIGN      8           /* stack alignment */
#else /* _WIN64 */
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
#define STKALIGN      4           /* stack alignment */
#endif
#else
#define PTR           uint32_t         /* pointer */
#define PTRSIZE       sizeof(uint32_t) /* pointer size */
#define STKALIGN      4           /* stack alignment */
#endif

#define REG1          register    /* register 1 - highest priority */
#define REG2          register    /* register 2 */
#define REG3          register    /* register 3 */
#define REG4          register    /* register 4 */
#define REG5          register    /* register 5 */
#define REG6          register    /* register 6 */
#define REG7          register    /* register 7 */
#define REG8          register    /* register 8 - lowest priority */

#define FAR                       /* for large model */
#define PTRSTRIDE     2           /* stride */
#define MEM_NONSEG                /* memory - non-segmented */
#endif


/* pointer alignment macro */

#define PTRALIGN(s) ((PTR)(s) % PTRSTRIDE ? ((PTR)(s) + (PTRSTRIDE - ((PTR)(s) % PTRSTRIDE))) : (PTR)(s))
/*
 * This macro (DATAALIGN) should only be used in our
 * static memory algorithms (SGetSBuf). 
 *
 * Otherwise, use at your own risk!
 */
#define DATAALIGN(s) ((Size)(s) % PTRSTRIDE ? ((Size)(s) - (PTRSTRIDE - ((Size)(s) % PTRSTRIDE))) : (Size)(s))


/* message function type macro */



/* typedef and defines for argument manipulation */
  
typedef S8               *ARGTYPE;

#define ARG(argp,type)     *((type*)argp)

#define ARGINC(argp,type)  argp=(ARGTYPE)(argp + (sizeof(type) < STKALIGN ? STKALIGN : sizeof(type)))

/* Unused function parameter macro */

#ifdef UNUSED
#undef UNUSED
#define UNUSED(x) (x=x)
#else
#define UNUSED(x) (x=x)
#endif /* UNUSED */


/**********************************************************************
   defines for trace
**********************************************************************/

/*

   there is one additional trace macro which turns on trace macros 0 - 4
   and pushes the text within trace macros 0 - 4 onto a stack. the text
   is popped of of the stack upon RETVOID or RETVALUE. this macro is:

   TRACE5  - trace call stack

*/
  
#ifdef TRACE5           /* trace 5 - call stack */

#define T5M 64          /* trace 5 - maximum call stack height */
S8 *t5s[];       /* trace 5 - call stack */
S16 t5t;         /* trace 5 - top of call stack */
#ifdef ERRCHK
Void FAR exit(int);
#endif
#endif

  
/**********************************************************************
   returns
**********************************************************************/

/*
   these returns are arranged first by environment (DOS, UNIX or
   embedded) followed by compiler (microsoft c, borland c, unix or
   microtec c) as appropriate.
*/

#ifdef DOS                        /* dos */
#ifdef MSC51                      /* dos, microsoft c */
#ifdef TRACE5
#define RETVOID     return(--t5t?(Void) 0 :(Void) 0)  /* return void */
/* envdep_h_001.main_76: Removed void type casting */
#define RETVALUE(v) return(--t5t?(v):(v)) /* return value */
#else
#define RETVOID     return        /* void */
#define RETVALUE(v) return(v)     /* return value */
#endif
#else                             /* not microsoft c */
#ifdef BOR31                      /* dos, borland c */
#ifdef TRACE5
#define RETVOID     return(--t5t?(Void) 0 :(Void) 0)  /* return void */
/* envdep_h_001.main_76: Removed void type casting */
#define RETVALUE(v) return(--t5t?(v):(v)) /* return value */
#else
#define RETVOID     return        /* void */
#define RETVALUE(v) return(v)     /* return value */
#endif
#else                             /* not borland c */
#endif
#endif
#else                             /* not dos */
#ifdef UNIX                       /* unix */
#ifdef TRACE5
#define RETVOID     return(--t5t?(Void) 0 :(Void) 0)  /* return void */
/* envdep_h_001.main_76: Removed void type casting */
#define RETVALUE(v) return(--t5t?(v):(v)) /* return value */

#else
#define RETVOID     return        /* void */
#define RETVALUE(v) return(v)     /* return value */
#endif
#else                             /* not unix */
#ifdef MOTADS                     /* embedded, motorola ads */
#ifdef DOSMRI42D                  /* dos microtec c cross-compiler */
#ifdef TRACE5
#define RETVOID     return(--t5t?(Void) 0 :(Void) 0)  /* return void */
/* envdep_h_001.main_76: Removed void type casting */
#define RETVALUE(v) return(--t5t?(v):(v)) /* return value */
#else
#define RETVOID     return        /* void */
#define RETVALUE(v) return(v)     /* return value */
#endif
#else                             /* not dos microtec c */
#ifdef SOLMRI43                   /* solaris microtec c cross-compiler */
#ifdef TRACE5
#define RETVOID     return(--t5t?(Void) 0 :(Void) 0)  /* return void */
/* envdep_h_001.main_76: Removed void type casting */
#define RETVALUE(v) return(--t5t?(v):(v)) /* return value */
#else
#define RETVOID     return        /* void */
#define RETVALUE(v) return(v)     /* return value */
#endif
#else                             /* not solaris microtec c cross-compiler */
#ifdef SOLOAS186                  /* solaris oasys c cross-compiler */
#ifdef TRACE5
#define RETVOID     return(--t5t?(Void) 0 :(Void) 0)  /* return void */
/* envdep_h_001.main_76: Removed void type casting */
#define RETVALUE(v) return(--t5t?(v):(v)) /* return value */
#else
#define RETVOID     return        /* void */
#define RETVALUE(v) return(v)     /* return value */
#endif
#else                             /* not microtec c, not oasys c */
#endif
#endif
#endif
#else                             /* not motorola ads */
#ifdef TRACE5
#define RETVOID     return(--t5t?(Void) 0 :(Void) 0)  /* return void */
/* envdep_h_001.main_76: Removed void type casting */
#define RETVALUE(v) return(--t5t?(v):(v)) /* return value */
#else
#define RETVOID     return        /* void */
#define RETVALUE(v) return(v)     /* return value */
#endif
#endif                            /* motorola ads */
#endif                            /* unix */
#endif                            /* dos */


/* General Macros - Moved from envind.h */
#ifndef HPOS
#ifdef MIN
#undef MIN
#define MIN(x, y) ((x) > (y) ? (y) : (x))            /* get minimum value */
#else
#define MIN(x, y) ((x) > (y) ? (y) : (x))            /* get minimum value */
#endif /* MIN */

#ifdef MAX
#undef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))            /* get maximum value */
#else
#define MAX(x, y) ((x) > (y) ? (x) : (y))            /* get maximum value */
#endif /* MAX */
#endif /* HPOS */



/*

   The following defines are used as switches throughout the
   TRILLIUM software 

   PORTVER   Portable version
   MOSVER    MOS version

   LCINT     Loosely coupled - isdn network layer
   TCINT     Tightly coupled - isdn network layer 
   LCXNT     Loosely coupled - x.25 network layer
   TCXNT     Tightly coupled - x.25 network layer 
   LCDAT     Loosely coupled - data link layer
   TCDAT     Tightly coupled - data link layer 
   LCMAC     Loosely coupled - mac layer
   TCMAC     Tightly coupled - mac layer
   LCLM      Loosely coupled - layer management
   TCLM      Tightly coupled - layer management

   ANSI      ansi

   DOS       dos environment
   UNIX      unix environment
 
   MSC51     microsoft c 5.1 compiler
   MSC60     microsoft c 6.0 compiler
   MSC70     microsoft c 7.0 compiler
   DOSMRI42D dos microtec c 4.2D cross-compiler
   SOLMRI43  solaris microtec c 4.3 cross-compiler
   SOLOAS186 solaris oasys 1.8.6 cross-compiler
   BOR31     borland c 3.1 compiler

   SUNOS     sun os environment
   SUNOS41   sun os 4.1 environment and compiler
   SUNOS51   sun os 5.1 environment and compiler
  
   MSCDOS    microsoft c, DOS environment - being phased out
   MSCUNIX   microsoft c, UNIX environment - being phased out

   UC2       Franklin UC2A interface
   WT3       Retix WT-330 interface
   FUT       Dassault AT-FUT interface
   MDP       Dassault MOS/Driver interface

   ERRCHK    Error check

   TRC0      Trace 0 - MOS support functions
   TRC1      Trace 1 - MOS interface functions
   TRC2      Trace 2 - layer support functions
   TRC3      Trace 3 - layer interface functions
   TRC4      Trace 4 - interrupt service functions

   X28X29    X.28 and X.29 support

*/
    
#ifdef TENB_TTI_PERF
#define TTI_THRESHOLD_VALUE 800
#endif

#ifdef __cplusplus
}
#endif


#endif /* __ENVDEPH__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
