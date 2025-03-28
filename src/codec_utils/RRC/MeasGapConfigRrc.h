/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_MeasGapConfigRrc_H_
#define	_MeasGapConfigRrc_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasGapConfigRrc__gapFR2_PR {
	MeasGapConfigRrc__gapFR2_PR_NOTHING,	/* No components present */
	MeasGapConfigRrc__gapFR2_PR_release,
	MeasGapConfigRrc__gapFR2_PR_setup
} MeasGapConfigRrc__gapFR2_PR;
typedef enum MeasGapConfigRrc__ext1__gapFR1_PR {
	MeasGapConfigRrc__ext1__gapFR1_PR_NOTHING,	/* No components present */
	MeasGapConfigRrc__ext1__gapFR1_PR_release,
	MeasGapConfigRrc__ext1__gapFR1_PR_setup
} MeasGapConfigRrc__ext1__gapFR1_PR;
typedef enum MeasGapConfigRrc__ext1__gapUE_PR {
	MeasGapConfigRrc__ext1__gapUE_PR_NOTHING,	/* No components present */
	MeasGapConfigRrc__ext1__gapUE_PR_release,
	MeasGapConfigRrc__ext1__gapUE_PR_setup
} MeasGapConfigRrc__ext1__gapUE_PR;

/* Forward declarations */
struct GapConfig;

/* MeasGapConfigRrc */
typedef struct MeasGapConfigRrc {
	struct MeasGapConfigRrc__gapFR2 {
		MeasGapConfigRrc__gapFR2_PR present;
		union MeasGapConfigRrc__gapFR2_u {
			NULL_t	 release;
			struct GapConfig	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *gapFR2;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct MeasGapConfigRrc__ext1 {
		struct MeasGapConfigRrc__ext1__gapFR1 {
			MeasGapConfigRrc__ext1__gapFR1_PR present;
			union MeasGapConfigRrc__ext1__gapFR1_u {
				NULL_t	 release;
				struct GapConfig	*setup;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *gapFR1;
		struct MeasGapConfigRrc__ext1__gapUE {
			MeasGapConfigRrc__ext1__gapUE_PR present;
			union MeasGapConfigRrc__ext1__gapUE_u {
				NULL_t	 release;
				struct GapConfig	*setup;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *gapUE;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasGapConfigRrc_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasGapConfigRrc;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasGapConfigRrc_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasGapConfigRrc_1[2];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "GapConfig.h"

#endif	/* _MeasGapConfigRrc_H_ */
#include <asn_internal.h>
