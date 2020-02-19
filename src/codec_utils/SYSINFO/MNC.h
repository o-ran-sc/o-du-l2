/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../mib_sib1.asn1"
 * 	`asn1c -D ./new_out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MNC_H_
#define	_MNC_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MCC-MNC-Digit.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* MNC */
typedef struct MNC {
	A_SEQUENCE_OF(MCC_MNC_Digit_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MNC_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MNC;
extern asn_SET_OF_specifics_t asn_SPC_MNC_specs_1;
extern asn_TYPE_member_t asn_MBR_MNC_1[1];
extern asn_per_constraints_t asn_PER_type_MNC_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _MNC_H_ */
#include <asn_internal.h>
