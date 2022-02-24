/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FeatureSet_H_
#define	_FeatureSet_H_


#include <asn_application.h>

/* Including external dependencies */
#include "FeatureSetEUTRA-DownlinkId.h"
#include "FeatureSetEUTRA-UplinkId.h"
#include <constr_SEQUENCE.h>
#include "FeatureSetDownlinkId.h"
#include "FeatureSetUplinkId.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum FeatureSet_PR {
	FeatureSet_PR_NOTHING,	/* No components present */
	FeatureSet_PR_eutra,
	FeatureSet_PR_nr
} FeatureSet_PR;

/* FeatureSet */
typedef struct FeatureSet {
	FeatureSet_PR present;
	union FeatureSet_u {
		struct FeatureSet__eutra {
			FeatureSetEUTRA_DownlinkId_t	 downlinkSetEUTRA;
			FeatureSetEUTRA_UplinkId_t	 uplinkSetEUTRA;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *eutra;
		struct FeatureSet__nr {
			FeatureSetDownlinkId_t	 downlinkSetNR;
			FeatureSetUplinkId_t	 uplinkSetNR;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *nr;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FeatureSet_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FeatureSet;
extern asn_CHOICE_specifics_t asn_SPC_FeatureSet_specs_1;
extern asn_TYPE_member_t asn_MBR_FeatureSet_1[2];
extern asn_per_constraints_t asn_PER_type_FeatureSet_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _FeatureSet_H_ */
#include <asn_internal.h>
