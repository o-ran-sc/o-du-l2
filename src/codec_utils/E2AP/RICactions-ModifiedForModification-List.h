/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RICactions_ModifiedForModification_List_H_
#define	_RICactions_ModifiedForModification_List_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolIE_SingleContainerE2;

/* RICactions-ModifiedForModification-List */
typedef struct RICactions_ModifiedForModification_List {
	A_SEQUENCE_OF(struct ProtocolIE_SingleContainerE2) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RICactions_ModifiedForModification_List_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RICactions_ModifiedForModification_List;

#ifdef __cplusplus
}
#endif

#endif	/* _RICactions_ModifiedForModification_List_H_ */
#include <asn_internal.h>
