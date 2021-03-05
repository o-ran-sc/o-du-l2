/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "E2AP.asn1"
 * 	`asn1c -D ./out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ResetRequest_H_
#define	_ResetRequest_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-ContainerE2.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ResetRequest */
typedef struct ResetRequest {
	ProtocolIE_ContainerE2_1407P14_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ResetRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ResetRequest;
extern asn_SEQUENCE_specifics_t asn_SPC_ResetRequest_specs_1;
extern asn_TYPE_member_t asn_MBR_ResetRequest_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _ResetRequest_H_ */
#include <asn_internal.h>
