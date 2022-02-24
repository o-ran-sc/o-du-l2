/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FR_InfoList_H_
#define	_FR_InfoList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct FR_Info;

/* FR-InfoList */
typedef struct FR_InfoList {
	A_SEQUENCE_OF(struct FR_Info) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FR_InfoList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FR_InfoList;
extern asn_SET_OF_specifics_t asn_SPC_FR_InfoList_specs_1;
extern asn_TYPE_member_t asn_MBR_FR_InfoList_1[1];
extern asn_per_constraints_t asn_PER_type_FR_InfoList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _FR_InfoList_H_ */
#include <asn_internal.h>
