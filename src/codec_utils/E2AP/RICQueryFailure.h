/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RICQueryFailure_H_
#define	_RICQueryFailure_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-ContainerE2.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RICQueryFailure */
typedef struct RICQueryFailure {
	ProtocolIE_ContainerE2_2530P19_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RICQueryFailure_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RICQueryFailure;
extern asn_SEQUENCE_specifics_t asn_SPC_RICQueryFailure_specs_1;
extern asn_TYPE_member_t asn_MBR_RICQueryFailure_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _RICQueryFailure_H_ */
#include <asn_internal.h>
