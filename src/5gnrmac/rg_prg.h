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
     Name:    MAC to MAC interface (PRG)

     Type:    C header file

     Desc:    Constants needed for PRG(MAC to MAC) interface

     File:    rg_prg.h

*********************************************************************21*/

#ifndef __RGPRG_H__ 
#define __RGPRG_H__

#ifdef LTE_ADV

#define RG_PRG_MAX                     1             /* MAC interface support only LC and TC*/
#define PRG_DIR_TX                     CRG_DIR_TX    /*!< Macreo for Transmit 
                                                      *   direction Mask */
#define PRG_DIR_RX                     CRG_DIR_RX    /*!< Macro For Receive
                                                      *   direction Mask */
#define PRG_DIR_TX_RX                  CRG_DIR_TX_RX /*!< Macro for Transmitt and Receive
                                                      *   direction Mask */
/* Event corresponding to each primitive at this interface. PRG interface
 *events values starts from 50 onwards and 1 to 49 reserved for SCH interface*/
#define EVTPRGUESCELLCFGREQ           50  /* Add Ue SCell Config Req from PMAC
                                            to SMAC*/
#define EVTPRGUESCELLCFGCFM           51  /* Config confirm for Add Ue Scell on PRG
                                            interface from SMAC to PMAC*/
#define EVTPRGUESCELLDELREQ           52  /* Ue SCell delete Req from PMAC to
                                            SMAC*/
#define EVTPRGUESCELLLCHMODREQ        53    /*  SCell Lch recfg Req from PMAC to
                                            SMAC*/

#define EVTPRGUESCELLLCHMODCFM        54    /* Lch ReConfig confirm for Scell on PRG
                                            interface from SMAC to PMAC*/

#define EVTPRGUESCELLLCHDELCFM        55    /* Delete Lch  confirm for Scell on PRG
                                            interface from SMAC to PMAC*/

#define EVTPRGUESCELLLCHDELREQ        56    /* Delete Lch request for Scell on PRG
                                            interface from SMAC to PMAC*/

#define EVTPRGUESCELLLCHADDREQ        57    /* Lch config req for Scell on PRG 
                                                  interface from SMAC to PMAC */

#define EVTPRGUESCELLLCHADDCFM        58    /* Lch Config confirm for Scell on PRG
                                            interface from SMAC to PMAC*/
#define PRG_CFG_CFM_OK    CRG_CFG_CFM_OK  /* mapping to CRG NOK macro*/
#define PRG_CFG_CFM_NOK   CRG_CFG_CFM_NOK /* mapping to CRG NOK macro*/

#define RGPRG_FREE_MSG(_buf)\
{\
   if (NULLP != (_buf)) \
   { \
      ODU_PUT_MSG_BUF((_buf)); \
      _buf = NULLP; \
   } \
}

#endif /* LTE_ADV*/
#endif /* __RGPRG_H__ */

/********************************************************************30**

         End of file:     rg_prg.h
**********************************************************************/
