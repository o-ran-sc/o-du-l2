/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "E2AP.asn1"
 * 	`asn1c -D ./out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CriticalityDiagnostics_IE_ListE2_H_
#define	_CriticalityDiagnostics_IE_ListE2_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CriticalityDiagnostics_IE_ItemE2;

/* CriticalityDiagnostics-IE-ListE2 */
typedef struct CriticalityDiagnostics_IE_ListE2 {
	A_SEQUENCE_OF(struct CriticalityDiagnostics_IE_ItemE2) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CriticalityDiagnostics_IE_ListE2_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CriticalityDiagnostics_IE_ListE2;
extern asn_SET_OF_specifics_t asn_SPC_CriticalityDiagnostics_IE_ListE2_specs_1;
extern asn_TYPE_member_t asn_MBR_CriticalityDiagnostics_IE_ListE2_1[1];
extern asn_per_constraints_t asn_PER_type_CriticalityDiagnostics_IE_ListE2_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _CriticalityDiagnostics_IE_ListE2_H_ */
#include <asn_internal.h>
