/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "E2AP.asn1"
 * 	`asn1c -D ./out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RICserviceQuery_H_
#define	_RICserviceQuery_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-ContainerE2.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RICserviceQuery */
typedef struct RICserviceQuery {
	ProtocolIE_ContainerE2_1407P19_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RICserviceQuery_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RICserviceQuery;
extern asn_SEQUENCE_specifics_t asn_SPC_RICserviceQuery_specs_1;
extern asn_TYPE_member_t asn_MBR_RICserviceQuery_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _RICserviceQuery_H_ */
#include <asn_internal.h>
