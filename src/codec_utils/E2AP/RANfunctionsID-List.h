/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "codeathon/E2AP.asn1"
 * 	`asn1c -D ./newE2AP -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RANfunctionsID_List_H_
#define	_RANfunctionsID_List_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolIE_SingleContainerE2;

/* RANfunctionsID-List */
typedef struct RANfunctionsID_List {
	A_SEQUENCE_OF(struct ProtocolIE_SingleContainerE2_1408P4_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RANfunctionsID_List_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RANfunctionsID_List;

#ifdef __cplusplus
}
#endif

#endif	/* _RANfunctionsID_List_H_ */
#include <asn_internal.h>
