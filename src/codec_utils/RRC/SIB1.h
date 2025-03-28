/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_SIB1_H_
#define	_SIB1_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CellAccessRelatedInfo.h"
#include <NativeEnumerated.h>
#include <OCTET_STRING.h>
#include "Q-RxLevMin.h"
#include <NativeInteger.h>
#include "Q-QualMin.h"
#include <constr_SEQUENCE.h>
#include "UAC-BarringInfoSetList.h"
#include "UAC-AccessCategory1-SelectionAssistanceInfo.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SIB1__ims_EmergencySupport {
	SIB1__ims_EmergencySupport_true	= 0
} e_SIB1__ims_EmergencySupport;
typedef enum SIB1__eCallOverIMS_Support {
	SIB1__eCallOverIMS_Support_true	= 0
} e_SIB1__eCallOverIMS_Support;
typedef enum SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo_PR {
	SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo_PR_NOTHING,	/* No components present */
	SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo_PR_plmnCommon,
	SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo_PR_individualPLMNList
} SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo_PR;
typedef enum SIB1__useFullResumeID {
	SIB1__useFullResumeID_true	= 0
} e_SIB1__useFullResumeID;

/* Forward declarations */
struct ConnEstFailureControl;
struct SI_SchedulingInfo;
struct ServingCellConfigCommonSIB;
struct UE_TimersAndConstants;
struct UAC_BarringPerCatList;
struct UAC_BarringPerPLMN_List;

/* SIB1 */
typedef struct SIB1 {
	struct SIB1__cellSelectionInfo {
		Q_RxLevMin_t	 q_RxLevMin;
		long	*q_RxLevMinOffset;	/* OPTIONAL */
		Q_RxLevMin_t	*q_RxLevMinSUL;	/* OPTIONAL */
		Q_QualMin_t	*q_QualMin;	/* OPTIONAL */
		long	*q_QualMinOffset;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *cellSelectionInfo;
	CellAccessRelatedInfo_t	 cellAccessRelatedInfo;
	struct ConnEstFailureControl	*connEstFailureControl;	/* OPTIONAL */
	struct SI_SchedulingInfo	*si_SchedulingInfo;	/* OPTIONAL */
	struct ServingCellConfigCommonSIB	*servingCellConfigCommon;	/* OPTIONAL */
	long	*ims_EmergencySupport;	/* OPTIONAL */
	long	*eCallOverIMS_Support;	/* OPTIONAL */
	struct UE_TimersAndConstants	*ue_TimersAndConstants;	/* OPTIONAL */
	struct SIB1__uac_BarringInfo {
		struct UAC_BarringPerCatList	*uac_BarringForCommon;	/* OPTIONAL */
		struct UAC_BarringPerPLMN_List	*uac_BarringPerPLMN_List;	/* OPTIONAL */
		UAC_BarringInfoSetList_t	 uac_BarringInfoSetList;
		struct SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo {
			SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo_PR present;
			union SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo_u {
				UAC_AccessCategory1_SelectionAssistanceInfo_t	 plmnCommon;
				struct SIB1__uac_BarringInfo__uac_AccessCategory1_SelectionAssistanceInfo__individualPLMNList {
					A_SEQUENCE_OF(UAC_AccessCategory1_SelectionAssistanceInfo_t) list;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} *individualPLMNList;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *uac_AccessCategory1_SelectionAssistanceInfo;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *uac_BarringInfo;
	long	*useFullResumeID;	/* OPTIONAL */
	OCTET_STRING_t	*lateNonCriticalExtension;	/* OPTIONAL */
	struct SIB1__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SIB1_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ims_EmergencySupport_12;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_eCallOverIMS_Support_14;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_useFullResumeID_25;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SIB1;
extern asn_SEQUENCE_specifics_t asn_SPC_SIB1_specs_1;
extern asn_TYPE_member_t asn_MBR_SIB1_1[12];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ConnEstFailureControl.h"
#include "SI-SchedulingInfo.h"
#include "ServingCellConfigCommonSIB.h"
#include "UE-TimersAndConstants.h"
#include "UAC-BarringPerCatList.h"
#include "UAC-BarringPerPLMN-List.h"

#endif	/* _SIB1_H_ */
#include <asn_internal.h>
