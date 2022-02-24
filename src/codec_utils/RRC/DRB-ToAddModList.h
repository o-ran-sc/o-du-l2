/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_DRB_ToAddModList_H_
#define	_DRB_ToAddModList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct DRB_ToAddMod;

/* DRB-ToAddModList */
typedef struct DRB_ToAddModList {
	A_SEQUENCE_OF(struct DRB_ToAddMod) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DRB_ToAddModList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DRB_ToAddModList;
extern asn_SET_OF_specifics_t asn_SPC_DRB_ToAddModList_specs_1;
extern asn_TYPE_member_t asn_MBR_DRB_ToAddModList_1[1];
extern asn_per_constraints_t asn_PER_type_DRB_ToAddModList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _DRB_ToAddModList_H_ */
#include <asn_internal.h>
