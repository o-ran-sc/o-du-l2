/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BWP_UplinkCommon_H_
#define	_BWP_UplinkCommon_H_


#include <asn_application.h>

/* Including external dependencies */
#include "BWP.h"
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BWP_UplinkCommon__rach_ConfigCommon_PR {
	BWP_UplinkCommon__rach_ConfigCommon_PR_NOTHING,	/* No components present */
	BWP_UplinkCommon__rach_ConfigCommon_PR_release,
	BWP_UplinkCommon__rach_ConfigCommon_PR_setup
} BWP_UplinkCommon__rach_ConfigCommon_PR;
typedef enum BWP_UplinkCommon__pusch_ConfigCommon_PR {
	BWP_UplinkCommon__pusch_ConfigCommon_PR_NOTHING,	/* No components present */
	BWP_UplinkCommon__pusch_ConfigCommon_PR_release,
	BWP_UplinkCommon__pusch_ConfigCommon_PR_setup
} BWP_UplinkCommon__pusch_ConfigCommon_PR;
typedef enum BWP_UplinkCommon__pucch_ConfigCommon_PR {
	BWP_UplinkCommon__pucch_ConfigCommon_PR_NOTHING,	/* No components present */
	BWP_UplinkCommon__pucch_ConfigCommon_PR_release,
	BWP_UplinkCommon__pucch_ConfigCommon_PR_setup
} BWP_UplinkCommon__pucch_ConfigCommon_PR;

/* Forward declarations */
struct RACH_ConfigCommon;
struct PUSCH_ConfigCommon;
struct PUCCH_ConfigCommon;

/* BWP-UplinkCommon */
typedef struct BWP_UplinkCommon {
	BWP_t	 genericParameters;
	struct BWP_UplinkCommon__rach_ConfigCommon {
		BWP_UplinkCommon__rach_ConfigCommon_PR present;
		union BWP_UplinkCommon__rach_ConfigCommon_u {
			NULL_t	 release;
			struct RACH_ConfigCommon	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *rach_ConfigCommon;
	struct BWP_UplinkCommon__pusch_ConfigCommon {
		BWP_UplinkCommon__pusch_ConfigCommon_PR present;
		union BWP_UplinkCommon__pusch_ConfigCommon_u {
			NULL_t	 release;
			struct PUSCH_ConfigCommon	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pusch_ConfigCommon;
	struct BWP_UplinkCommon__pucch_ConfigCommon {
		BWP_UplinkCommon__pucch_ConfigCommon_PR present;
		union BWP_UplinkCommon__pucch_ConfigCommon_u {
			NULL_t	 release;
			struct PUCCH_ConfigCommon	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pucch_ConfigCommon;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BWP_UplinkCommon_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BWP_UplinkCommon;
extern asn_SEQUENCE_specifics_t asn_SPC_BWP_UplinkCommon_specs_1;
extern asn_TYPE_member_t asn_MBR_BWP_UplinkCommon_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _BWP_UplinkCommon_H_ */
#include <asn_internal.h>
