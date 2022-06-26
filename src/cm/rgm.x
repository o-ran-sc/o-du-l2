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
     Name:    Upper Layer Interface

     Type:    C header file

     Desc:    Structures, variables and typedefs required by 
              RGU interface

     File:    rgu.x

*********************************************************************21*/


#ifndef __RGMX__
#define __RGMX__
#ifdef __cplusplus
extern "C" {
#endif

/** 
  @file rgu.x 
  @brief Structure declarations and definitions for RGU interface.
 */

/***********************************************************************
                          typedefs and data structures
 ***********************************************************************/

#define RGM_PRB_REPORT_STOP     0 
#define RGM_PRB_REPORT_START    1 

#define RGM_MAX_QCI_REPORTS     4

/* RRM_SP1_START */
typedef struct rgmPrbRptPerQci
{
   uint8_t       bQci;
   uint8_t       bAvgPrbUlUsage;
   uint8_t       bAvgPrbDlUsage;
}RgmPrbRptPerQci;

typedef struct rgmPrbRprtInd
{
   RgmPrbRptPerQci   stQciPrbRpts[RGM_MAX_QCI_REPORTS];
/* RRM_SP1_END */
   uint8_t                bCellId;
   /* TDD: DL PRB Usage pres = 2 and 
    *      UL PRB Usage pres = 1 
    * FDD: DL and UL Usage Pres = 3     
    */
   uint8_t                bPrbUsageMask; 
}RgmPrbRprtInd;

typedef enum
{
   RGM_TXN_MODE1,
   RGM_TXN_MODE2,
   RGM_TXN_MODE3,
   RGM_TXN_MODE4,
   RGM_TXN_MODE5,
   RGM_TXN_MODE6,
   RGM_TXN_MODE7,
   RGM_TXN_MODE8
} RgmTxnMode;

typedef struct rgmTransModeInd
{
   RgmTxnMode       eMode;         /* Indicate TM Mode */ 
   uint16_t              usCrnti;       /* UE Crnti value     */
   uint8_t               bCellId;       /* Cell Id */
}RgmTransModeInd;
/***********************************************************************
          type definitions for upper layer interface - RLC primitives
 ***********************************************************************/
 /** @brief Data Indication from MAC to RLC to 
  *  * forward the data received for common channels */
typedef S16 (*RgmPrbRprtIndFptr) ARGS((
         Pst*                 pst,
         SuId                 suId,
         RgmPrbRprtInd  *    prbRprtInd));
/** @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels */
typedef S16 (*RgmTransModeIndFptr) ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgmTransModeInd  *   transModeInd));

#ifdef RM_INTF
/** @brief Data Indication from MAC to RRM to 
 * change the transmission mode*/
S16 RmLiRgmTransModeInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgmTransModeInd*     transModeInd
));
#endif

/** @brief Data Indication from MAC to RLC to 
 *  * forward the data received for common channels*/
S16 cmPkRgmPrbRprtInd ARGS((
         Pst*                 pst,
         SuId                 suId,
         RgmPrbRprtInd  *    prbRprtInd
         ));


S16 cmUnpkPrbRprtInd ARGS((
         RgmPrbRprtInd  * prbRprtInd,
         Buffer *mBuf
         ));

S16 cmPkTransModeInd ARGS((
RgmTransModeInd *transModeInd,
Buffer *mBuf
));

S16 cmUnpkTransModeInd ARGS((
RgmTransModeInd *transModeInd,
Buffer *mBuf
));
S16 cmPkRgmTransModeInd ARGS((
   Pst*                 pst,
   SuId                 suId,
   RgmTransModeInd *transModeInd
));


S16 cmUnpkRgmTransModeInd ARGS((
         RgmTransModeIndFptr    func,
         Pst*                 pst,
         Buffer               *mBuf
         ));

/** @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels*/
S16 cmUnpkRgmPrbRprtInd ARGS((
   RgmPrbRprtIndFptr    func,
   Pst*                 pst,
   Buffer               *mBuf
));

S16 cmPkRgmPrbQciRpt ARGS((
RgmPrbRptPerQci *qciPrbRprt,
Buffer *mBuf
));

S16 cmUnpkRgmPrbQciRpt ARGS((
RgmPrbRptPerQci *qciPrbRprt,
Buffer *mBuf
));

#ifdef __cplusplus
}
#endif
#endif /* __RGUX__ */

/********************************************************************30**

         End of file
**********************************************************************/
