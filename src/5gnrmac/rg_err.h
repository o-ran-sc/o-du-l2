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

/**********************************************************************
 
     Name:    LTEMAC - Error File
  
     Type:    C include file 
  
     Desc:    Error defines required by LTEMAC layer
 
     File:    rg_err.h

*********************************************************************21*/
 
#ifndef __RGERRH__
#define __RGERRH__

/* defines */


/* Macro definitions */
#define RGLOGERROR(_inst,errCls, errCode, errVal, errDesc) \
   SLogError(rgCb[_inst].rgInit.ent, rgCb[_inst].rgInit.inst, rgCb[inst].rgInit.procId, \
             (Txt *) __FILE__,    \
             (S32) __LINE__,      \
             (ErrCls) (errCls),   \
             (ErrCode) (errCode), \
             (ErrVal) (errVal),   \
             (Txt *) errDesc)

/* error codes */
#define   ERGBASE 0

#define   ERGXXX        (ERGBASE + 0)   /* reserved */
#define   ERRRG         (ERGBASE + 0)   /* reserved */

#define   RGERR_NONE    ERGBASE

#define   ERG001      (ERRRG +    1)    /*     gk_rom.c: 221 */
#define   ERG002      (ERRRG +    2)    /*     gk_rom.c: 235 */
#define   ERG003      (ERRRG +    3)    /*     gk_rom.c: 365 */
#define   ERG004      (ERRRG +    4)    /*     gk_rom.c: 476 */
#define   ERG005      (ERRRG +    5)    /*     gk_rom.c: 623 */
#define   ERG006      (ERRRG +    6)    /*     gk_rom.c: 738 */
#define   ERG007      (ERRRG +    7)    /*     gk_rom.c: 933 */

#define   ERG008      (ERRRG +    8)    /*     gk_uim.c: 227 */
#define   ERG009      (ERRRG +    9)    /*     gk_uim.c: 237 */
#define   ERG010      (ERRRG +   10)    /*     gk_uim.c: 293 */
#define   ERG011      (ERRRG +   11)    /*     gk_uim.c: 302 */
#define   ERG012      (ERRRG +   12)    /*     gk_uim.c: 413 */
#define   ERG013      (ERRRG +   13)    /*     gk_uim.c: 423 */
#define   ERG014      (ERRRG +   14)    /*     gk_uim.c: 520 */
#define   ERG015      (ERRRG +   15)    /*     gk_uim.c: 529 */
#define   ERG016      (ERRRG +   16)    /*     gk_uim.c: 612 */
#define   ERG017      (ERRRG +   17)    /*     gk_uim.c: 621 */
#define   ERG018      (ERRRG +   18)    /*     gk_uim.c: 695 */
#define   ERG019      (ERRRG +   19)    /*     gk_uim.c: 704 */
#define   ERG020      (ERRRG +   20)    /*     gk_uim.c: 777 */
#define   ERG021      (ERRRG +   21)    /*     gk_uim.c: 786 */
#define   ERG022      (ERRRG +   22)    /*     gk_uim.c:1061 */
#define   ERG023      (ERRRG +   23)    /*     gk_uim.c:1071 */
#define   ERG024      (ERRRG +   24)    /*     gk_uim.c:1128 */
#define   ERG025      (ERRRG +   25)    /*     gk_uim.c:1137 */
#define   ERG026      (ERRRG +   26)    /*     gk_uim.c:1253 */
#define   ERG027      (ERRRG +   27)    /*     gk_uim.c:1266 */

#define   ERG028      (ERRRG +   28)    /*     gk_utl.c: 196 */
#define   ERG029      (ERRRG +   29)    /*     gk_utl.c: 267 */
#define   ERG030      (ERRRG +   30)    /*     gk_utl.c: 320 */
#define   ERG031      (ERRRG +   31)    /*     gk_utl.c: 605 */


/* ***********************************************************
 *                       Error Type 
 *************************************************************/
#define RGERR_TYPE_BASE                0
#define RGERR_CAUSE_BASE               0

/* ErrType defines for MUX */
#define RG_MUX_ERRTYPE_BASE            RGERR_TYPE_BASE
#define RGERR_MUX_BLD_PDU              (RG_MUX_ERRTYPE_BASE + 1)
#define RGERR_MUX_BLD_CEHDR_FAIL       (RG_MUX_ERRTYPE_BASE + 2)
#define RGERR_MUX_BLD_CE_FAIL          (RG_MUX_ERRTYPE_BASE + 3)
#define RGERR_MUX_BLD_SDUHDR_FAIL      (RG_MUX_ERRTYPE_BASE + 4)
#define RGERR_MUX_BLD_SDU_FAIL         (RG_MUX_ERRTYPE_BASE + 5)
#define RGERR_MUX_BLD_PADHDR_FAIL      (RG_MUX_ERRTYPE_BASE + 6)
#define RGERR_MUX_BLD_PAD_FAIL         (RG_MUX_ERRTYPE_BASE + 7)
#define RGERR_MUX_BLD_BI_FAIL          (RG_MUX_ERRTYPE_BASE + 8)
#define RGERR_MUX_BLD_RAPIDHDR_FAIL    (RG_MUX_ERRTYPE_BASE + 9)
#define RGERR_MUX_BLD_RAPID_FAIL       (RG_MUX_ERRTYPE_BASE + 10)
#define RGERR_MUX_BLD_RAR_PDU          (RG_MUX_ERRTYPE_BASE + 11)
/* ErrType defines for DHM */
#define RG_DHM_ERRTYPE_BASE            (RGERR_MUX_BLD_RAR_PDU + 1)
#define RGERR_DHM_SND_DAT_REQ           RG_DHM_ERRTYPE_BASE 
#define RGERR_DHM_FDBK_IND             (RG_DHM_ERRTYPE_BASE + 1)
#define RGERR_DHM_SND_STA_IND          (RG_DHM_ERRTYPE_BASE + 2)
/* ErrType defines for L2Measurement */
#ifdef LTE_L2_MEAS
#define RGERR_L2M_ERRTYPE_BASE         RG_DHM_ERRTYPE_BASE
#define RGERR_L2M_MEASREQ              (RGERR_L2M_ERRTYPE_BASE + 1)   
#define RGERR_DHM_SND_HARQ_STA_IND     (RGERR_L2M_ERRTYPE_BASE + 2)
#define RGERR_L2M_INV_PARAM            (RGERR_L2M_ERRTYPE_BASE + 3)
#define RGERR_L2M_INV_CELL_ID          (RGERR_L2M_ERRTYPE_BASE + 4)
#endif /* LTE_L2_MEAS */
#define RGERR_DHM_SND_HQ_FDB_REQ       (RG_DHM_ERRTYPE_BASE + 3)
/* ErrType defines for ROM */
#define RG_ROM_ERRTYPE_BASE            (RGERR_DHM_FDBK_IND + 1)
#define RGERR_ROM_DEDDATREQ            RG_ROM_ERRTYPE_BASE 
#define RGERR_ROM_CMNDATREQ            (RG_ROM_ERRTYPE_BASE + 1)
#define RGERR_ROM_DEDSTARSP            (RG_ROM_ERRTYPE_BASE + 2)
#define RGERR_ROM_CMNSTARSP            (RG_ROM_ERRTYPE_BASE + 3)
/* ErrType defines for TOM */
#define RG_TOM_ERRTYPE_BASE            (RGERR_ROM_CMNSTARSP + 1)
#define RGERR_TOM_RAREQIND             (RG_TOM_ERRTYPE_BASE)
#define RGERR_TOM_HARQACKIND           (RG_TOM_ERRTYPE_BASE + 1)
#define RGERR_TOM_SRIND                (RG_TOM_ERRTYPE_BASE + 2)
#define RGERR_TOM_DLCQIIND             (RG_TOM_ERRTYPE_BASE + 3)
#define RGERR_TOM_DATIND               (RG_TOM_ERRTYPE_BASE + 4)
#define RGERR_TOM_DECFAILIND           (RG_TOM_ERRTYPE_BASE + 5)
#define RGERR_TOM_TTIIND               (RG_TOM_ERRTYPE_BASE + 6)
/* COM Module related error MACROs for error type */
#define RG_COM_ERRTYPE_BASE            (RGERR_TOM_TTIIND + 1)
#define RGERR_COM_CFG_REQ              RG_COM_ERRTYPE_BASE 
#define RGERR_COM_RECFG_REQ            (RG_COM_ERRTYPE_BASE + 1)
#define RGERR_COM_DEL_REQ              (RG_COM_ERRTYPE_BASE + 2)
#define RGERR_COM_RESET_REQ            (RG_COM_ERRTYPE_BASE + 3)
/* GOM Module related error MACROs for error type */
#define RG_GOM_ERRTYPE_BASE            (RGERR_COM_DEL_REQ + 1)
#define RGERR_GOM_CFG_REQ              RG_GOM_ERRTYPE_BASE 
#define RGERR_GOM_RECFG_REQ            (RG_GOM_ERRTYPE_BASE + 1)

/* ***********************************************************
 *                       Error Cause 
 *************************************************************/
/* Errcause defines for MUX */
#define RG_MUX_ERR_CAUSE_BASE          RGERR_CAUSE_BASE
#define RGERR_MUX_MEM_ALLOC_FAIL       RG_MUX_ERR_CAUSE_BASE + 1
#define RGERR_MUX_BLD_HDR_FAIL         RG_MUX_ERR_CAUSE_BASE + 2
#define RGERR_FDBK_IND_INVALID_CB      RG_MUX_ERR_CAUSE_BASE + 3
/* Errcause defines for DHM */
#define RG_DHM_ERRCAUSE_BASE           RGERR_FDBK_IND_INVALID_CB + 1
#define RG_DHM_MEM_ALLOC_FAIL          RG_DHM_ERRCAUSE_BASE 
/* Errcause defines for RAM */
#define RG_RAM_ERRCAUSE_BASE           RGERR_FDBK_IND_INVALID_CB + 1
#define RGERR_RAM_MEM_EXHAUST          RG_RAM_ERRCAUSE_BASE 
#define RGERR_RAM_NO_MSG3_RCVD         RG_RAM_ERRCAUSE_BASE + 1
#define RGERR_RAM_RNTI_EXHAUST         RG_RAM_ERRCAUSE_BASE + 2
/* Errcause defines for ROM */
#define RG_ROM_ERRCAUSE_BASE           RGERR_RAM_RNTI_EXHAUST + 1
#define RGERR_ROM_INV_CELL_ID          RG_ROM_ERRCAUSE_BASE 
#define RGERR_ROM_DELAYED_DATREQ       RG_ROM_ERRCAUSE_BASE + 1
#define RGERR_ROM_INV_UE_ID            RG_ROM_ERRCAUSE_BASE + 2
#define RGERR_ROM_INV_LC_ID            RG_ROM_ERRCAUSE_BASE + 3
#define RGERR_ROM_INV_RNTI             RG_ROM_ERRCAUSE_BASE + 4
#define RGERR_ROM_MEM_EXHAUST          RG_ROM_ERRCAUSE_BASE + 5
#define RGERR_ROM_INV_DAT_LEN          RG_ROM_ERRCAUSE_BASE + 6
/* Errcause defines for RAM */
#define RG_TOM_ERRCAUSE_BASE           RGERR_ROM_INV_DAT_LEN + 1
#define RGERR_TOM_INV_CELL_ID          RG_TOM_ERRCAUSE_BASE 
#define RGERR_TOM_MEM_EXHAUST          RG_TOM_ERRCAUSE_BASE + 1
/* Errcause defines for DUX */
#define RG_DUX_ERRCAUSE_BASE           RGERR_TOM_MEM_EXHAUST + 1
#define RGERR_DUX_INV_LCID_RX          RG_DUX_ERRCAUSE_BASE 
#define RGERR_DUX_MEM_EXHAUST          RG_DUX_ERRCAUSE_BASE + 1
#define RGERR_DUX_UNPACK_FAILURE       RG_DUX_ERRCAUSE_BASE + 2
#define RGERR_DUX_RLC_PDU_CREAT_FAIL   RG_DUX_ERRCAUSE_BASE + 3
#define RGERR_DUX_RLC_DATIND_FAIL      RG_DUX_ERRCAUSE_BASE + 4
#define RGERR_DUX_DBM_FAILURE          RG_DUX_ERRCAUSE_BASE + 5
#define RGERR_DUX_INV_PDU_RX           RG_DUX_ERRCAUSE_BASE + 6
/* CFG Module related error MACROs for error cause */
#define RG_CFG_ERRCAUSE_BASE               RGERR_DUX_INV_PDU_RX + 1
#define RGERR_CFG_INVALID_CRG_CELL_CFG     RG_CFG_ERRCAUSE_BASE 
#define RGERR_CFG_INVALID_CRG_UE_CFG       RG_CFG_ERRCAUSE_BASE + 1
#define RGERR_CFG_INVALID_CRG_DED_LC_CFG   RG_CFG_ERRCAUSE_BASE + 2
#define RGERR_CFG_INVALID_CRG_CMN_LC_CFG   RG_CFG_ERRCAUSE_BASE + 3
#define RGERR_CFG_INVALID_CRG_CELL_RECFG   RG_CFG_ERRCAUSE_BASE + 4
#define RGERR_CFG_INVALID_CRG_UE_RECFG     RG_CFG_ERRCAUSE_BASE + 5
#define RGERR_CFG_INVALID_CRG_LC_RECFG     RG_CFG_ERRCAUSE_BASE + 6
#define RGERR_CFG_INVALID_RGR_CELL_CFG     RG_CFG_ERRCAUSE_BASE + 7
#define RGERR_CFG_INVALID_RGR_CELL_RECFG   RG_CFG_ERRCAUSE_BASE + 8
#define RGERR_CFG_CRG_CELL_CFG             RG_CFG_ERRCAUSE_BASE + 9
#define RGERR_CFG_CRG_UE_CFG               RG_CFG_ERRCAUSE_BASE + 10
#define RGERR_CFG_CRG_DED_LC_CFG           RG_CFG_ERRCAUSE_BASE + 11
#define RGERR_CFG_CRG_CMN_LC_CFG           RG_CFG_ERRCAUSE_BASE + 12
#define RGERR_CFG_CRG_CELL_RECFG           RG_CFG_ERRCAUSE_BASE + 13
#define RGERR_CFG_CRG_UE_RECFG             RG_CFG_ERRCAUSE_BASE + 14
#define RGERR_CFG_CRG_LC_RECFG             RG_CFG_ERRCAUSE_BASE + 15
#define RGERR_CFG_RGR_CELL_CFG             RG_CFG_ERRCAUSE_BASE + 16
#define RGERR_CFG_RGR_CELL_RECFG           RG_CFG_ERRCAUSE_BASE + 17
#define RGERR_CFG_CRG_CELL_DEL             RG_CFG_ERRCAUSE_BASE + 18
#define RGERR_CFG_CRG_UE_DEL               RG_CFG_ERRCAUSE_BASE + 19
#define RGERR_CFG_CRG_LC_DEL               RG_CFG_ERRCAUSE_BASE + 20
#define RGERR_CFG_INVALID_CRG_UE_RESET     RG_CFG_ERRCAUSE_BASE + 21
#ifdef LTE_ADV
#define RGERR_CFG_CRG_UE_SCELL_CFG           RG_CFG_ERRCAUSE_BASE + 22
#endif /* LTE_ADV */
/* Scheduler related error causes */
#define RG_SCH_ERRCAUSE_BASE               RGERR_CFG_CRG_LC_DEL + 1
#define RGERR_SCH_CFG                      RG_SCH_ERRCAUSE_BASE 
#define RGERR_SCH_LCG_NOT_CFGD             RG_SCH_ERRCAUSE_BASE + 1
#define RGERR_SCH_NO_LCG_CFGD              RG_SCH_ERRCAUSE_BASE + 2

   

#endif /* __RGERRH__ */
/********************************************************************30**
  
         End of file
**********************************************************************/
