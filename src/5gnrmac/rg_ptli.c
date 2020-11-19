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
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_ptli.c
  
**********************************************************************/

/** @file rg_dhm.c
@brief APIs related to Downlink HARQ.
*/

/* header include files (.h) */
#include "common_def.h"
#include "tfu.h"           /* RGU Interface defines */

/* header/extern include files (.x) */
#include "tfu.x"           /* RGU Interface includes */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !(defined(LCRGLITFU)  && defined(TF) && defined(LWLCRGLITFU))
#define PTRGLITFU
#endif


/* MAX Number of Service Providers of RG */
#define RG_MAX_TFU_PROV   3

#ifdef PTRGLITFU
/** @brief This API is used to send a Bind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
 */
S16 PtLiTfuBndReq ARGS((Pst * pst, SuId suId, SpId spId));
/** @brief This API is used to send a Bind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
 */
S16 PtLiTfuSchBndReq ARGS((Pst * pst, SuId suId, SpId spId));
/** @brief This API is used to send an Unbind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
 */
S16 PtLiTfuUbndReq ARGS((Pst * pst, SpId spId, Reason reason));
/** @brief This API is used to send an Unbind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
 */
S16 PtLiTfuSchUbndReq ARGS((Pst * pst, SpId spId, Reason reason));
/** @brief This primitive is sent from Scheduler to PHY.
 * @details This primitive provides PHY with all the information required by 
 * PHY to decode transmissions from the UE on either PUCCH or PUSCH.
 * -# On PUCCH UE can transmit the following
 *    -# SR
 *    -# HARQ feedback
 *    -# CQI report
 *    -# HARQ + CQI
 *    -# HARQ + SR
 * -# On PUSCH UE can transmit the following
 *    -# Data
 *    -# Data + CQI
 *    -# Data + HARQ Feedback
 * This primitive carries all the information for the expected subframe for all
 * the UEs that have been scheduled to transmit.
 * @param pst Pointer to the post structure.
 * @param spId SAP ID of the service provider.
 * @param recpReq Pointer to the TfuRecpReqInfo structure.
 * @return ROK/RFAILED
 */
S16 PtLiTfuRecpReq ARGS((Pst * pst, SpId spId, TfuRecpReqInfo * recpReq));
/** @brief This Primitive is sent from Scheduler to PHY. It provides PHY with
  * all the control information
  * @details This primitive carries the information sent on the following
  * channels - 
  * -# PDCCH
  * -# PHICH
  * -# PCFICH
  * 
  * @param pst
  * @param spId
  * @param cntrlReq pointer to TfuCntrlReqInfo
  * @return ROK/RFAILED
  */
S16 PtLiTfuCntrlReq ARGS((Pst * pst, SpId spId, TfuCntrlReqInfo * cntrlReq));
/** @brief This Primitive carries the Data PDUs from MAC to PHY for
  * transmission. 
  * @details The data being sent in this primitive is meant to be transmitted on
  * the downlink channel PDSCH and PBCH (if present). To facilitate physical
  * layer processing, requisite control information is also sent along with the
  * data. 
  * @sa TfUiTfuCntrlReq
  * @param pst 
  * @param spId
  * @param tfuDatReq pointer to TfuDatReqInfo
  * @return 
  */
S16 PtLiTfuDatReq ARGS((Pst * pst, SpId spId, TfuDatReqInfo * datReq));
#ifdef L2_OPTMZ
/** @brief This Primitive carries cellId and UeId for which datReq need to be deleted.
  * @details This primitive is used to send delDatReq to CL to delete the PDUs of
  * UE which has been deleted in MAC due to ueId change or anyother scenario
  * @details The data being sent in this primitive is meant to be transmitted on
  * @sa TfUiTfuDelDatReq
  * @param pst
  * @param spId
  * @param tfuDelDatReq pointer to TfuDelDatReqInfo
  * @return
 */
S16 PtLiTfuDelDatReq ARGS((Pst * pst, SpId spId, TfuDelDatReqInfo * datReq));
#endif /*L2_OPTMZ*/
#endif /*--#ifdef PTRGLITFU--*/

#ifdef L2_OPTMZ
/** @brief This Primitive carries cellId and UeId for which datReq need to be deleted. 
  * @details This primitive is used to send delDatReq to CL to delete the PDUs of 
  * UE which has been deleted in MAC due to ueId change or anyother scenario
  * NOTE:: This API is only supported for TC because race condition issue
  *        happens only in case of TC
  * @sa TfUiTfuDelDatReq
  * @param pst 
  * @param spId
  * @param tfuDelDatReq pointer to TfuDelDatReqInfo
  * @return 
  */
static const TfuDelDatReq RgLiTfuDelDatReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   PtLiTfuDelDatReq, /*calling dummy api as LC not required for this privitive*/
#else
   PtLiTfuDelDatReq,
#endif
#ifdef TF
   TfUiTfuDelDatReq,
#else
   PtLiTfuDelDatReq,
#endif
#ifdef LWLCRGLITFU
   PtLiTfuDelDatReq, /*calling dummy api as LWLC not required for this privitive*/
#else
   PtLiTfuDelDatReq
#endif
};
#endif /* L2_OPTMZ*/

#ifdef RG


#ifdef L2_OPTMZ

/***********************************************************
*
*     Func : RgLiTfuDelDatReq
*
*
*     Desc : This Primitive is used to delete datReq in CL when there is ueId change. 
  * @details: This primitive is required when L2_OPMZ flag is elabed. this is required
  *           To delete datRq PDUs from CL for the Ue for which Ue Id got changed or
  *           anyother similar scenario
  * @sa TfUiTfuDelDatReq
  * @param pst 
  * @param spId
  * @param tfuDelDatReq pointer to TfuDelDatReqInfo
  * @return
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 RgLiTfuDelDatReq
(
Pst * pst,
SpId spId,
TfuDelDatReqInfo * delDatReq
)
{

   return ((*RgLiTfuDelDatReqMt[pst->selector])(pst, spId, delDatReq));

}
#endif /* L2_OPTMZ*/

#endif /*--ifdef RG--*/

#ifdef PTRGLITFU



/***********************************************************
*
*     Func : PtLiTfuBndReq
*
*
*     Desc : This API is used to send a Bind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   return ROK;

}



/***********************************************************
*
*     Func : PtLiTfuSchBndReq
*
*
*     Desc : This API is used to send a Bind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuSchBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
{

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   return ROK;

}



/***********************************************************
*
*     Func : PtLiTfuUbndReq
*
*
*     Desc : This API is used to send an Unbind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
{

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   return ROK;

}



/***********************************************************
*
*     Func : PtLiTfuSchUbndReq
*
*
*     Desc : This API is used to send an Unbind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuSchUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
{

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   return ROK;

}



/***********************************************************
*
*     Func : PtLiTfuRecpReq
*
*
*     Desc : This primitive is sent from Scheduler to PHY.
 * @details This primitive provides PHY with all the information required by 
 * PHY to decode transmissions from the UE on either PUCCH or PUSCH.
 * -# On PUCCH UE can transmit the following
 *    -# SR
 *    -# HARQ feedback
 *    -# CQI report
 *    -# HARQ + CQI
 *    -# HARQ + SR
 * -# On PUSCH UE can transmit the following
 *    -# Data
 *    -# Data + CQI
 *    -# Data + HARQ Feedback
 * This primitive carries all the information for the expected subframe for all
 * the UEs that have been scheduled to transmit.
 * @param pst Pointer to the post structure.
 * @param spId SAP ID of the service provider.
 * @param recpReq Pointer to the TfuRecpReqInfo structure.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuRecpReq
(
Pst * pst,
SpId spId,
TfuRecpReqInfo * recpReq
)
{

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(recpReq);

   return ROK;

}



/***********************************************************
*
*     Func : PtLiTfuCntrlReq
*
*
*     Desc : This Primitive is sent from Scheduler to PHY. It provides PHY with
  * all the control information
  * @details This primitive carries the information sent on the following
  * channels - 
  * -# PDCCH
  * -# PHICH
  * -# PCFICH
  * 
  * @param pst
  * @param spId
  * @param cntrlReq pointer to TfuCntrlReqInfo
  * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuCntrlReq
(
Pst * pst,
SpId spId,
TfuCntrlReqInfo * cntrlReq
)
{

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(cntrlReq);

   return ROK;

}



/***********************************************************
*
*     Func : PtLiTfuDatReq
*
*
*     Desc : This Primitive carries the Data PDUs from MAC to PHY for
  * transmission. 
  * @details The data being sent in this primitive is meant to be transmitted on
  * the downlink channel PDSCH and PBCH (if present). To facilitate physical
  * layer processing, requisite control information is also sent along with the
  * data. 
  * @sa TfUiTfuCntrlReq
  * @param pst 
  * @param spId
  * @param tfuDatReq pointer to TfuDatReqInfo
  * @return
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuDatReq
(
Pst * pst,
SpId spId,
TfuDatReqInfo * datReq
)
{

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(datReq);

   return ROK;

}


#ifdef L2_OPTMZ
/***********************************************************
*
*     Func : PtLiTfuDelDatReq
*
*
*   @brief This Primitive carries cellId and UeId for which datReq need to be deleted.
  * @details This primitive is used to send delDatReq to CL to delete the PDUs of
    * UE which has been deleted in MAC due to ueId change or anyother scenario
    
  * @details The data being sent in this primitive is meant to be transmitted on
  * @sa TfUiTfuDelDatReq
  * @param pst 
  * @param spId
  * @param tfuDelDatReq pointer to TfuDelDatReqInfo
  * @return
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
S16 PtLiTfuDelDatReq
(
Pst * pst,
SpId spId,
TfuDelDatReqInfo * delDatReq
)
{

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(delDatReq);

   return ROK;

}
#endif /*L2_OPTMZ*/

#endif /*--ifdef PTRGLITFU--*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**********************************************************************
 
         End of file
**********************************************************************/
