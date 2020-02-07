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
     Name:    LTE MAC SC1 scheduler 

     Type:    C header file

     Desc:    Defines required by SC1 scheduler

     File:    rg_sch_sc1.h

*********************************************************************21*/


#ifndef __RGSCHSC1H__
#define __RGSCHSC1H__ 
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
                         Macro Definitions 
 ***********************************************************************/

/* Scheduler1 tunable params */

#define RG_GET_SC1_CELL_UL(cell) \
             ((RgSchSc1UlCell *)((RgSchCmnCell*)((cell)->sc.sch))->ul.schSpfc)
#define RG_GET_SC1_CELL_DL(cell) \
             ((RgSchSc1DlCell *)((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc)

#define RG_GET_SC1_UE_DL(ue, cell) \
             ((RgSchSc1DlUe *)((RgSchCmnUe *)((ue->cellInfo[ue->cellIdToCellIdxMap\
                            [RG_SCH_CELLINDEX(cell)]])->sch))->dl.schSpfc)
#define RG_GET_SC1_UE_UL(ue, cell) \
             ((RgSchSc1UlUe *)((RgSchCmnUe *)((ue->cellInfo[ue->cellIdToCellIdxMap\
                 [RG_SCH_CELLINDEX(cell)]])->sch))->ul.schSpfc)

#define RG_GET_SC1_SVC_DL(_ue,_svc,_cell) RG_SCH_CMN_GET_LC_SCH_SPFC(_ue,_svc,_cell)

#define RG_GET_SC1_HQP_DL(hqP) \
             ((RgSchSc1DlHqProc *)((RgSchCmnDlHqProc *)((hqP)->sch))->schSpfc)

#define RG_SC1_SVC_HAS_DATA(_svc,_cell) \
   (((RgSchSc1DlSvc *)(((RgSchCmnDlSvc *)(_svc->sch))->schSpfc[\
                       svc->ue->cellIdToCellIdxMap[RG_SCH_CELLINDEX(_cell)]]))->effMbr != 0)


#define RG_SC1_ISGBRPRIO(qciPrio) ((qciPrio) >=2 && (qciPrio) <= 5)

/* SC1 configuration error types */ 
#define RGSCHERR_SCH_SC1_DL_CFG (RGSCHERR_SCH_CFG + 10)
#define RGSCHERR_SCH_SC1_UL_CFG (RGSCHERR_SCH_CFG + 11)

/* Scheduler1 fixed params */
#define RG_SC1_UL_INVALID_QID      255
#define RG_SC1_QCIPRIOVAL_MAX      9
#define RG_SC1_QCIPRIOVAL_MIN      1
#define RG_SC1_UL_CONT_RES_QID     0
#define RG_SC1_MAX_DL_AMBR         0xffffffff
#define RG_SC1_UL_NUM_Q            (RG_SCH_CMN_NUM_QCI + 1)
#define RG_SC1_DL_NUM_Q            (RG_SCH_CMN_MAX_PRIO + 1) 
#ifdef RG_UNUSED
#define RG_SC1_UL_INVALID_BETA8    0xffff
#endif
#define RG_SCH_SC1_DL_GBR_PRIO_START     1
#define RG_SCH_SC1_DL_GBR_PRIO_END       4
#define RG_SCH_SC1_DL_PRIOS              RG_SCH_CMN_MAX_PRIO + 1

#ifdef __cplusplus
}
#endif
#endif /* __RGSCHSC1H__ */

/********************************************************************30**

         End of file
**********************************************************************/
