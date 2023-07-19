/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "../../ASN1_Input/E2SM_KPM_V_3_0.asn1"
 * 	`asn1c -D ./../../E2_output/E2SM_KPM_v3.0_output -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MatchingUEidPerGP_H_
#define	_MatchingUEidPerGP_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MatchingUEidPerGP_Item;

/* MatchingUEidPerGP */
typedef struct MatchingUEidPerGP {
	A_SEQUENCE_OF(struct MatchingUEidPerGP_Item) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MatchingUEidPerGP_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MatchingUEidPerGP;
extern asn_SET_OF_specifics_t asn_SPC_MatchingUEidPerGP_specs_1;
extern asn_TYPE_member_t asn_MBR_MatchingUEidPerGP_1[1];
extern asn_per_constraints_t asn_PER_type_MatchingUEidPerGP_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _MatchingUEidPerGP_H_ */
#include <asn_internal.h>
