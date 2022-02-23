/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasResultList2NR_H_
#define	_MeasResultList2NR_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MeasResult2NR;

/* MeasResultList2NR */
typedef struct MeasResultList2NR {
	A_SEQUENCE_OF(struct MeasResult2NR) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasResultList2NR_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasResultList2NR;
extern asn_SET_OF_specifics_t asn_SPC_MeasResultList2NR_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasResultList2NR_1[1];
extern asn_per_constraints_t asn_PER_type_MeasResultList2NR_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _MeasResultList2NR_H_ */
#include <asn_internal.h>
