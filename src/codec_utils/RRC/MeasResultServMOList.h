/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasResultServMOList_H_
#define	_MeasResultServMOList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MeasResultServMO;

/* MeasResultServMOList */
typedef struct MeasResultServMOList {
	A_SEQUENCE_OF(struct MeasResultServMO) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasResultServMOList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasResultServMOList;
extern asn_SET_OF_specifics_t asn_SPC_MeasResultServMOList_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasResultServMOList_1[1];
extern asn_per_constraints_t asn_PER_type_MeasResultServMOList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _MeasResultServMOList_H_ */
#include <asn_internal.h>
