/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SIB_Mapping_H_
#define	_SIB_Mapping_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SIB_TypeInfo;

/* SIB-Mapping */
typedef struct SIB_Mapping {
	A_SEQUENCE_OF(struct SIB_TypeInfo) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SIB_Mapping_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SIB_Mapping;
extern asn_SET_OF_specifics_t asn_SPC_SIB_Mapping_specs_1;
extern asn_TYPE_member_t asn_MBR_SIB_Mapping_1[1];
extern asn_per_constraints_t asn_PER_type_SIB_Mapping_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _SIB_Mapping_H_ */
#include <asn_internal.h>
