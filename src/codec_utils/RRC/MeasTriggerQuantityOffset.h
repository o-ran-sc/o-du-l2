/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasTriggerQuantityOffset_H_
#define	_MeasTriggerQuantityOffset_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasTriggerQuantityOffset_PR {
	MeasTriggerQuantityOffset_PR_NOTHING,	/* No components present */
	MeasTriggerQuantityOffset_PR_rsrp,
	MeasTriggerQuantityOffset_PR_rsrq,
	MeasTriggerQuantityOffset_PR_sinr
} MeasTriggerQuantityOffset_PR;

/* MeasTriggerQuantityOffset */
typedef struct MeasTriggerQuantityOffset {
	MeasTriggerQuantityOffset_PR present;
	union MeasTriggerQuantityOffset_u {
		long	 rsrp;
		long	 rsrq;
		long	 sinr;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasTriggerQuantityOffset_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasTriggerQuantityOffset;
extern asn_CHOICE_specifics_t asn_SPC_MeasTriggerQuantityOffset_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasTriggerQuantityOffset_1[3];
extern asn_per_constraints_t asn_PER_type_MeasTriggerQuantityOffset_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _MeasTriggerQuantityOffset_H_ */
#include <asn_internal.h>
