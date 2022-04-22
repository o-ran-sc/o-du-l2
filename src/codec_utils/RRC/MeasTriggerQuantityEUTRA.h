/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasTriggerQuantityEUTRA_H_
#define	_MeasTriggerQuantityEUTRA_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RSRP-RangeEUTRA.h"
#include "RSRQ-RangeEUTRA.h"
#include "SINR-RangeEUTRA.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasTriggerQuantityEUTRA_PR {
	MeasTriggerQuantityEUTRA_PR_NOTHING,	/* No components present */
	MeasTriggerQuantityEUTRA_PR_rsrp,
	MeasTriggerQuantityEUTRA_PR_rsrq,
	MeasTriggerQuantityEUTRA_PR_sinr
} MeasTriggerQuantityEUTRA_PR;

/* MeasTriggerQuantityEUTRA */
typedef struct MeasTriggerQuantityEUTRA {
	MeasTriggerQuantityEUTRA_PR present;
	union MeasTriggerQuantityEUTRA_u {
		RSRP_RangeEUTRA_t	 rsrp;
		RSRQ_RangeEUTRA_t	 rsrq;
		SINR_RangeEUTRA_t	 sinr;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasTriggerQuantityEUTRA_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasTriggerQuantityEUTRA;
extern asn_CHOICE_specifics_t asn_SPC_MeasTriggerQuantityEUTRA_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasTriggerQuantityEUTRA_1[3];
extern asn_per_constraints_t asn_PER_type_MeasTriggerQuantityEUTRA_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _MeasTriggerQuantityEUTRA_H_ */
#include <asn_internal.h>
