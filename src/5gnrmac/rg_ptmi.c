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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for portable definitions for Layer Manager
               Interface primitives.
  
     File:     rg_ptmi.c 
  
**********************************************************************/

/** @file rg_ptmi.c
@brief This file contains the definitions for Layer Manager
       Interface primitives that are invoked from MAC to Layer Manager
       (Confirmations and Indications) Portable functions corresponding
       to these primitives are also defined.
*/

/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"           /* LRG Interface defines */
/* header/extern include files (.x) */
#include "lrg.x"           /* LRG Interface includes */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !(defined(LCRGMILRG) && defined(SM))
#define PTRGMILRG
#endif

#define RG_MAX_LRG_USR 2 

/* portable functions at LRG interface */
#ifdef PTRGMILRG
S16 PtMiLrgCfgCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 PtMiLrgSchCfgCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 PtMiLrgStsCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 PtMiLrgStaCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 PtMiLrgStaInd ARGS((Pst *pst, RgMngmt *usta));
S16 PtMiLrgSchStaInd ARGS((Pst *pst, RgMngmt *usta));
S16 PtMiLrgCntrlCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 PtMiLrgSchCntrlCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 PtMiLrgTrcInd ARGS((Pst *pst, RgMngmt *trc,Buffer *mBuf));
#ifdef LTE_L2_MEAS
S16 PtMiLrgSchL2MeasCfm ARGS((Pst *pst, LrgSchMeasCfmInfo *cfm));
S16 PtMiLrgSchL2MeasStopCfm ARGS((Pst *pst,LrgSchMeasCfmInfo *cfm ));
#endif
#endif

/* ***********LRG interface Mapping matrices ********************/

/** @brief Configuration Confirm primitive Matrix 
 *  This matrix defines the mapping between the configuration confirm
 *  primitive called by the upper interface of MAC layer and the 
 *  corresponding primitives of the MAC layer service user(s).
 *
 *  The parameter RG_MAX_LRG_USR defines the maximum number of Layer 
 *  managers of MAC at the LRG interface. This is an array of functions
 *  per primitive invoked by MAC whose size is RG_MAX_LRG_USR.
 * 
 *  The dispatching is performed by the configurable variable: selector.
 *  The selector is configured on a per SAP basis. The selectors are:
 *
 *  LCRGMILRG - loosely coupled LMK interface
 *  TCRGMILRG - Tightly coupled LMK interface
 *  
 *  In a similar fashion, the matrices for Status, Statistics and
 *  Control confirms and, Status and trace indications are defined.
 **/
static const LrgCfgCfm RgMiLrgCfgCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgCfgCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgCfgCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgCfgCfm,                  /* 1 - Tightly coupled SM */
#else
   //PtMiLrgCfgCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Scheduler Config Confirm primitive Matrix */
static const LrgSchCfgCfm RgMiLrgSchCfgCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchCfgCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgSchCfgCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchCfgCfm,                  /* 1 - Tightly coupled SM */
#else
   //PtMiLrgSchCfgCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Statistics Confirm primitive Matrix */
static const LrgStsCfm RgMiLrgStsCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgStsCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgStsCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgStsCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgStsCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Status Confirm primitive Matrix */
static const LrgStaCfm RgMiLrgStaCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgStaCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgStaCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgStaCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgStaCfm,                  /* 1 - Tightly coupled SM  */
#endif
};


/** @brief Status Indication primitive Matrix */
static const LrgStaInd RgMiLrgStaIndMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgStaInd,                  /* 0 - loosely coupled */
#else
   PtMiLrgStaInd,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgStaInd,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgStaInd,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Scheduler  Status Indication primitive Matrix */
static const LrgSchStaInd RgMiLrgSchStaIndMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchStaInd,                  /* 0 - loosely coupled */
#else
   PtMiLrgSchStaInd,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchStaInd,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchStaInd,                  /* 1 - Tightly coupled SM  */
#endif
};


/** @brief Control Confirm primitive Matrix */
static const LrgCntrlCfm RgMiLrgCntrlCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgCntrlCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgCntrlCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgCntrlCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgCntrlCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Scheduler Control Confirm primitive Matrix */
static const LrgSchCntrlCfm RgMiLrgSchCntrlCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchCntrlCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgSchCntrlCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchCntrlCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchCntrlCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Trace Indication primitive Matrix */
static const LrgTrcInd RgMiLrgTrcIndMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgTrcInd,                  /* 0 - loosely coupled */
#else
   PtMiLrgTrcInd,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgTrcInd,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgTrcInd,                  /* 1 - Tightly coupled SM  */
#endif
};
#ifdef LTE_L2_MEAS
/** @brief L2 Meas Cfm primitive Matrix */
static const LrgSchL2MeasCfm RgMiLrgSchL2mMeasCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchL2MeasCfm,                /* 0 - loosely coupled */
#else
   PtMiLrgSchL2MeasCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchL2MeasCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchL2MeasCfm,                  /* 1 - loosely coupled */
#endif
};
/** @brief L2 Meas Stop Cfm primitive Matrix */
static const LrgSchL2MeasStopCfm RgMiLrgSchL2mMeasStopCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchL2MeasStopCfm,                /* 0 - loosely coupled */
#else
   PtMiLrgSchL2MeasStopCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchL2MeasStopCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchL2MeasStopCfm,                  /* 1 - loosely coupled */
#endif
};
#endif /* LTE_L2_MEAS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef RG


/**
 * @brief Layer Manager Configuration confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgCfgCfm() or SmMiLrgCfgCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgCfgCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* config confirm structure  */
)
{
   
   (*RgMiLrgCfgCfmMt[pst->selector])(pst, cfm);

   return ROK;
   
}/*-- RgMiLrgCfgCfm --*/

/**
 * @brief Layer Manager scheduler Configuration confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by Scheduler to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgSchCfgCfm() or SmMiLrgSchCfgCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgSchCfgCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* config confirm structure  */
)
{
   
   (*RgMiLrgSchCfgCfmMt[pst->selector])(pst, cfm);

   return ROK;
   
}/*-- RgMiLrgSchCfgCfm --*/



/**
 * @brief Layer Manager Statistics confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStsCfm
 *     
 *     This function handles the statistics
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgStsCfm() or SmMiLrgStsCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the statistics confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgStsCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* statistics confirm structure  */
)
{
   
   (*RgMiLrgStsCfmMt[pst->selector])(pst, cfm);

   return ROK;
   
}/*-- RgMiLrgStsCfm --*/


/**
 * @brief Layer Manager Status confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStaCfm
 *     
 *     This function handles the status
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgStaCfm() or SmMiLrgStaCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgStaCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* status confirm structure  */
)
{
   
   (*RgMiLrgStaCfmMt[pst->selector])(pst,cfm);

   return ROK;
   
}/*-- RgMiLrgStaCfm --*/


/**
 * @brief Layer Manager Control confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCntrlCfm
 *     
 *     This function handles the control
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgCntrlCfm() or SmMiLrgCntrlCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgCntrlCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* control confirm structure  */
)
{
   
   (*RgMiLrgCntrlCfmMt[pst->selector])(pst,cfm);

   return ROK;
   
}/*-- RgMiLrgCntrlCfm --*/

/**
 * @brief Layer Manager scheduler Control confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCntrlCfm
 *     
 *     This function handles the control
 *     confirm invoked by scheduler to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgSchCntrlCfm() or SmMiLrgSchCntrlCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgSchCntrlCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* control confirm structure  */
)
{
   
   (*RgMiLrgSchCntrlCfmMt[pst->selector])(pst,cfm);

   return ROK;
   
}/*-- RgMiLrgSchCntrlCfm --*/


/**
 * @brief Layer Manager Unsolicited Status Indication handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStaInd
 *     
 *     This function handles the unsolicited status
 *     Indication invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgStaInd() or SmMiLrgStaInd().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *usta, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgStaInd
(
Pst      *pst,    /* post structure  */
RgMngmt  *usta    /* status indication structure  */
)
{
   
   (*RgMiLrgStaIndMt[pst->selector])(pst,usta);

   return ROK;
   
}/*-- RgMiLrgStaInd --*/

/**
 * @brief Layer Manager Unsolicited Status Indication handler from scheduler 
 *
 * @details
 *
 *     Function : RgMiLrgSchStaInd
 *     
 *     This function handles the unsolicited status
 *     Indication invoked by Scheduler to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgSchStaInd() or SmMiLrgSchStaInd().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *usta, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgSchStaInd
(
Pst      *pst,    /* post structure  */
RgMngmt  *usta    /* status indication structure  */
)
{
   
   (*RgMiLrgSchStaIndMt[pst->selector])(pst,usta);

   return ROK;
   
}/*-- RgMiLrgSchStaInd --*/


/**
 * @brief Layer Manager Trace Indication handler. 
 *
 * @details
 *
 *     Function : RgMiLrgTrcInd
 *     
 *     This function handles the trace
 *     Indication invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgTrcInd() or SmMiLrgTrcInd().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *trc, the trace event
 *  @param[in]  Buffer *mBuf, the trace message
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgTrcInd
(
Pst      *pst,    /* post structure  */
RgMngmt  *trc,    /* Trace event  */
Buffer   *mBuf    /* Trace message  */
)
{
   
   (*RgMiLrgTrcIndMt[pst->selector])(pst,trc,mBuf);

   return ROK;
   
}/*-- RgMiLrgTrcInd --*/

#ifdef LTE_L2_MEAS

/* TODO: Function header */
S16 RgMiLrgSchL2MeasCfm
(
Pst               *pst,          /* post structure  */
LrgSchMeasCfmInfo *cfm       /* Meas Cfm Info */
)
{
   
   (*RgMiLrgSchL2mMeasCfmMt[pst->selector])(pst,cfm);
   
   return ROK;

} /* RgMiLrgSchL2MeasCfm */

/**
 * @brief This function used to send L2 measurement stop confirmation 
 *        to Layer manager.
 *         
 *
 * @details
 *
 *     Function : RgMiLrgSchL2MeasStopCfm
 *     This function sends reposnes to L2 Measurement stop request
 *
 *
 *  @param[in]  Pst *pst, the post structure
 *  @param[in]  LrgSchMeasCfmInfo *cfm, theconfirm structure
 *  @return  S16
 *      -# ROK
 **/

S16 RgMiLrgSchL2MeasStopCfm
(        
Pst               *pst,          /* post structure  */
LrgSchMeasCfmInfo *cfm       /* Meas Cfm Info */
)        
{
   (*RgMiLrgSchL2mMeasStopCfmMt[pst->selector])(pst,cfm);
          
   return ROK;
        
} /* RgMiLrgSchL2MeasStopCfm */

#endif /* LTE_L2_MEAS */

#endif /*-- MK --*/

#ifdef PTRGMILRG

/**
 * @brief Portable Function definition for Layer Manager Configuration
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of Configuration Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgCfgCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Config Confirm */
)
{
   return ROK;
}/* end of PtMiLrgCfgCfm */

/**
 * @brief Portable Function definition for Layer Manager Configuration
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by scheduler to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of Configuration Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgSchCfgCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Config Confirm */
)
{

   return ROK;
}/* end of PtMiLrgSchCfgCfm */



/**
 * @brief Portable Function definition for Layer Manager Statistics 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgStsCfm 
 *     
 *     This function handles the statistics 
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of statistics Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the statistics confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgStsCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Statistics Confirm */
)
{

   return ROK;
}/* end of PtMiLrgStsCfm */



/**
 * @brief Portable Function definition for Layer Manager Status 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgStaCfm 
 *     
 *     This function handles the status 
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of status Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgStaCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Status Confirm */
)
{
   return ROK;
}/* end of PtMiLrgStaCfm */



/**
 * @brief Portable Function definition for Layer Manager Status 
 *        Indication handler. 
 *
 * @details
 *
 *     Function : PtMiLrgStaInd 
 *     
 *     This function handles the status 
 *     indication invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of status indication are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgStaInd
(
Pst *pst,               /* post structure */
RgMngmt *usta           /* Status Indication */
)
{
   return ROK;
}/* end of PtMiLrgStaInd */

/**
 * @brief Portable Function definition for Layer Manager Status 
 *        Indication handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchStaInd 
 *     
 *     This function handles the status 
 *     indication invoked by Scheduler to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of status indication are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgSchStaInd
(
Pst *pst,               /* post structure */
RgMngmt *usta           /* Status Indication */
)
{
   return ROK;
}/* end of PtMiLrgSchStaInd */


/**
 * @brief Portable Function definition for Layer Manager Control 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgCntrlCfm 
 *     
 *     This function handles the control 
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of control Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgCntrlCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Control Confirm */
)
{
   return ROK;
}/* end of PtMiLrgCntrlCfm */

/**
 * @brief Portable Function definition for Layer Manager Control 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchCntrlCfm 
 *     
 *     This function handles the control 
 *     confirm invoked by scheduler to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of control Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgSchCntrlCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Control Confirm */
)
{
   return ROK;
}/* end of PtMiLrgSchCntrlCfm */


/**
 * @brief Portable Function definition for Layer Manager Trace 
 *        Indication handler. 
 *
 * @details
 *
 *     Function : PtMiLrgTrcInd 
 *     
 *     This function handles the trace 
 *     indication invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of trace indication are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the trace indication structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgTrcInd
(
Pst *pst,               /* post structure */
RgMngmt *trc,           /* Trace Event */
Buffer *mBuf            /* Trace message */
)
{
   return ROK;
}/* end of PtMiLrgTrcInd */
#ifdef LTE_L2_MEAS
/**
 * @brief Portable Function definition for L2 Measurement Configuration
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchL2MeasCfm
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  *cfm, the measurement confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgSchL2MeasCfm
(
Pst *pst,               /* post structure */
LrgSchMeasCfmInfo *cfm  /* Measurement Confirm */
)
{

   return ROK;
}/* end of PtMiLrgSchL2MeasCfm */

/**
 * @brief Portable Function definition for L2 Measurement stop 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchL2MeasStopCfm
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  *cfm, the measurement confirm structure
 *  @return  S16
 *      -# ROK
 **/
S16 PtMiLrgSchL2MeasStopCfm
(
Pst *pst,               /* post structure */
LrgSchMeasCfmInfo *cfm  /* Measurement Confirm */
)
{

   return ROK;
}/* end of PtMiLrgSchL2MeasStopCfm */
#endif
#endif /*--PTRGMILRG--*/


/**********************************************************************
 
         End of file
**********************************************************************/
