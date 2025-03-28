/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_KPM_v2.03.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_MatchingUeCondPerSubList_H_
#define	_MatchingUeCondPerSubList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MatchingUeCondPerSubItem;

/* MatchingUeCondPerSubList */
typedef struct MatchingUeCondPerSubList {
	A_SEQUENCE_OF(struct MatchingUeCondPerSubItem) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MatchingUeCondPerSubList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MatchingUeCondPerSubList;
extern asn_SET_OF_specifics_t asn_SPC_MatchingUeCondPerSubList_specs_1;
extern asn_TYPE_member_t asn_MBR_MatchingUeCondPerSubList_1[1];
extern asn_per_constraints_t asn_PER_type_MatchingUeCondPerSubList_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _MatchingUeCondPerSubList_H_ */
#include <asn_internal.h>
