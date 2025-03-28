/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Contents"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_SCell_ToBeSetup_List_H_
#define	_SCell_ToBeSetup_List_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolIE_SingleContainer;

/* SCell-ToBeSetup-List */
typedef struct SCell_ToBeSetup_List {
	A_SEQUENCE_OF(struct ProtocolIE_SingleContainer) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SCell_ToBeSetup_List_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SCell_ToBeSetup_List;
extern asn_SET_OF_specifics_t asn_SPC_SCell_ToBeSetup_List_specs_1;
extern asn_TYPE_member_t asn_MBR_SCell_ToBeSetup_List_1[1];
extern asn_per_constraints_t asn_PER_type_SCell_ToBeSetup_List_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _SCell_ToBeSetup_List_H_ */
#include <asn_internal.h>
