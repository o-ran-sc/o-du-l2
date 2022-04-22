/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasGapSharingConfig_H_
#define	_MeasGapSharingConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include "MeasGapSharingScheme.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasGapSharingConfig__gapSharingFR2_PR {
	MeasGapSharingConfig__gapSharingFR2_PR_NOTHING,	/* No components present */
	MeasGapSharingConfig__gapSharingFR2_PR_release,
	MeasGapSharingConfig__gapSharingFR2_PR_setup
} MeasGapSharingConfig__gapSharingFR2_PR;
typedef enum MeasGapSharingConfig__ext1__gapSharingFR1_PR {
	MeasGapSharingConfig__ext1__gapSharingFR1_PR_NOTHING,	/* No components present */
	MeasGapSharingConfig__ext1__gapSharingFR1_PR_release,
	MeasGapSharingConfig__ext1__gapSharingFR1_PR_setup
} MeasGapSharingConfig__ext1__gapSharingFR1_PR;
typedef enum MeasGapSharingConfig__ext1__gapSharingUE_PR {
	MeasGapSharingConfig__ext1__gapSharingUE_PR_NOTHING,	/* No components present */
	MeasGapSharingConfig__ext1__gapSharingUE_PR_release,
	MeasGapSharingConfig__ext1__gapSharingUE_PR_setup
} MeasGapSharingConfig__ext1__gapSharingUE_PR;

/* MeasGapSharingConfig */
typedef struct MeasGapSharingConfig {
	struct MeasGapSharingConfig__gapSharingFR2 {
		MeasGapSharingConfig__gapSharingFR2_PR present;
		union MeasGapSharingConfig__gapSharingFR2_u {
			NULL_t	 release;
			MeasGapSharingScheme_t	 setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *gapSharingFR2;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct MeasGapSharingConfig__ext1 {
		struct MeasGapSharingConfig__ext1__gapSharingFR1 {
			MeasGapSharingConfig__ext1__gapSharingFR1_PR present;
			union MeasGapSharingConfig__ext1__gapSharingFR1_u {
				NULL_t	 release;
				MeasGapSharingScheme_t	 setup;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *gapSharingFR1;
		struct MeasGapSharingConfig__ext1__gapSharingUE {
			MeasGapSharingConfig__ext1__gapSharingUE_PR present;
			union MeasGapSharingConfig__ext1__gapSharingUE_u {
				NULL_t	 release;
				MeasGapSharingScheme_t	 setup;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *gapSharingUE;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasGapSharingConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasGapSharingConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasGapSharingConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasGapSharingConfig_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasGapSharingConfig_H_ */
#include <asn_internal.h>
