#ifdef FILE_ID
#undef FILE_ID
#endif /*FILE_ID */
#define FILE_ID 12411
/*******************************************************************************
*
* <COPYRIGHT_TAG>
*
*******************************************************************************/
/*******************************************************************************
* @file		- Common_typedef.h
* @brief	- This header file defines those data type both used by eNB and UE.
* @author	- Leifeng Ruan(leifeng.ruan@intel.com), Xuebin Yang(xuebin.yang@intel.com) Intel Labs China
*******************************************************************************/

/**********************************************************************
* Attentation and assumption:
*     LSBs are used when one variable needs only 2, 4 etc bits when there 
*     is no explicit declarition.
**********************************************************************/

#ifndef _COMMON_TYPEDEF_H_
#define _COMMON_TYPEDEF_H_

/** \brief Usage: BOOLEAN bool; */
typedef Bool BOOLEAN;

/** \brief Usage: UWORD8 u8Tmp; */
typedef unsigned char UWORD8;

/** \brief Usage: WORD8 i8Tmp; */
typedef char WORD8;

/** \brief Usage: UWORD16 u16Tmp; */
typedef unsigned short UWORD16;

/** \brief Usage: WORD16 i16Tmp; */
typedef short WORD16;

/** \brief Usage: UWORD32 u32Tmp; */
typedef unsigned int UWORD32;

/** \brief Usage: WORD32 i32Tmp; */
typedef int WORD32;


/** \brief Usage: WORD64 i64Tmp; */
typedef long WORD64;

/** \brief Usage: UWORD64 u64Tmp; */
typedef unsigned long long UWORD64;

/** \brief Usage: FLOAT32  floatTmp; */
typedef float FLOAT32;

/** \brief Usage: DOUBLE64 doubleTmp; */
typedef double DOUBLE64;

/** \brief Usage: COMPLEX16 cp16Tmp; */
typedef struct {
    WORD16 re; /*!<description>*/
    WORD16 im; /*!<description>*/
} COMPLEX16;
/** \value it may take 0,1 or 1,-1*/
typedef	signed short  Bits; 

#ifndef TRUE
#define TRUE (1==1)
#endif
#ifndef FALSE
#define FALSE (0==1)
#endif

#ifndef SUCCESS
/** SUCCESS = 0 */
#define SUCCESS     0
#endif /* #ifndef SUCCESS */

#ifndef FAILURE
/** FAILURE = 1 */
#define FAILURE     1
#endif /* #ifndef FAILURE */

#endif /* #ifndef _COMMON_TYPEDEF_H_ */
