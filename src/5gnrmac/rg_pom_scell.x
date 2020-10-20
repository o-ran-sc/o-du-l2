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

/* *****************************************************************************
  Name:     LTE-MAC layer 
  
  Type:     C Include File 
  
  Desc:     Structures, variables, and typedefs required by Secondary cell
            received through PRG interface.

  File:     rg_pom_scell.x 

**********************************************************************/
/** 
  @file rg_pom_scell.x 
  @brief Structure declarations and definitions for Secondary cell received from  (PRG) interface.
  */

#ifndef __RGPOMSCELL_X__ 
#define __RGPOMSCELL_X__

#ifdef __cplusplus
extern "C" {
#endif
#ifdef LTE_ADV
S16 RgPrgPMacSMacUeSCellLchModReq ARGS
((
 Pst                      *pst,
 RgPrgUeSCellLchModInfo   *lchCfgCb
));
S16 RgPrgPMacSMacUeSCellLchAddReq ARGS
((
 Pst                      *pst,
 RgPrgUeSCellLchAddInfo   *lchCfgCb
));
S16 RgPrgPMacSMacUeSCellLchDelReq ARGS
((
 Pst                       *pst,
 RgPrgUeSCellLchDelInfo    *delLcCb
));
S16 rgPomSndUeSCellLchDelToSmac ARGS((Inst inst,CrgDel *lcDel,
                                              Bool *isCfmRqrd));
S16 rgPomSndUeSCellLchAddToSmac ARGS((Inst inst,RgCellCb *cell,
                                              RgUeCb *ue,CrgLchCfg *lchCfg,
                                              Bool *isCfmRqrd));
S16 rgPomSndUeSCellLchModToSmac ARGS((Inst inst,RgCellCb *cell,
                                              RgUeCb *ue,CrgLchRecfg *lchRecfg,
                                              Bool *isCfmRqrd));
S16 rgPomVldtAddLch ARGS((Inst                    inst,
                                 RgPrgUeSCellLchAddInfo  *lcCfg,
                                 RgCellCb                **cell,
                                 RgUeCb                  **ue
                               ));
S16 rgPomUeSCellLcAdd ARGS((Inst                     inst,
                                   RgCellCb                 *cell,
                                   RgUeCb                   *ue,
                                   RgPrgUeSCellLchAddInfo   *lcCfg
                                 ));
S16 rgPomVltdModLch ARGS ((Inst                     inst,
                                  RgPrgUeSCellLchModInfo   *lchCfgCb,
                                  RgCellCb                 **cell,
                                  RgUeCb                   **ue,
                                  RgUlLcCb                 **ulLc
                                 ));
S16 rgPomUeSCellLcMod ARGS((Inst                     inst,
                                    RgUeCb                  *ue,
                                    RgUlLcCb                *ulLc,
                                    RgPrgUeSCellLchModInfo  *lchCfgCb
                                  ));
S16 rgPomUeSCellLcDel ARGS((Inst                   inst,
                                 RgPrgUeSCellLchDelInfo   *delLcCb,
                                 RgUeCb                   *ue,
                                 RgUlLcCb                 *ulLc,
                                 RgDlLcCb                 *dlLc
                               ));
S16 rgPomVltdDelLch ARGS ((Inst                      inst,
                                  RgPrgUeSCellLchDelInfo    *delLcCb,                              
                                  RgCellCb                  **cell,
                                  RgUeCb                    **ue,
                                  RgUlLcCb                  **ulLc,
                                  RgDlLcCb                  **dlLc
                                ));
#endif

#ifdef __cplusplus
}
#endif
#endif /* __RGPRG_X__*/

/**********************************************************************
  
         End of file:     rg_pom_scell.x
**********************************************************************/
