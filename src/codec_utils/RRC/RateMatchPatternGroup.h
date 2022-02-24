/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RateMatchPatternGroup_H_
#define	_RateMatchPatternGroup_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "RateMatchPatternId.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RateMatchPatternGroup__Member_PR {
	RateMatchPatternGroup__Member_PR_NOTHING,	/* No components present */
	RateMatchPatternGroup__Member_PR_cellLevel,
	RateMatchPatternGroup__Member_PR_bwpLevel
} RateMatchPatternGroup__Member_PR;

/* Forward definitions */
typedef struct RateMatchPatternGroup__Member {
	RateMatchPatternGroup__Member_PR present;
	union RateMatchPatternGroup__Member_u {
		RateMatchPatternId_t	 cellLevel;
		RateMatchPatternId_t	 bwpLevel;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RateMatchPatternGroup__Member;

/* RateMatchPatternGroup */
typedef struct RateMatchPatternGroup {
	A_SEQUENCE_OF(RateMatchPatternGroup__Member) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RateMatchPatternGroup_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RateMatchPatternGroup;
extern asn_SET_OF_specifics_t asn_SPC_RateMatchPatternGroup_specs_1;
extern asn_TYPE_member_t asn_MBR_RateMatchPatternGroup_1[1];
extern asn_per_constraints_t asn_PER_type_RateMatchPatternGroup_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _RateMatchPatternGroup_H_ */
#include <asn_internal.h>
