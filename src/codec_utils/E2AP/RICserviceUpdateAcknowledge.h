/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_RICserviceUpdateAcknowledge_H_
#define	_RICserviceUpdateAcknowledge_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-ContainerE2.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RICserviceUpdateAcknowledge */
typedef struct RICserviceUpdateAcknowledge {
	ProtocolIE_ContainerE2_2530P33_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RICserviceUpdateAcknowledge_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RICserviceUpdateAcknowledge;
extern asn_SEQUENCE_specifics_t asn_SPC_RICserviceUpdateAcknowledge_specs_1;
extern asn_TYPE_member_t asn_MBR_RICserviceUpdateAcknowledge_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _RICserviceUpdateAcknowledge_H_ */
#include <asn_internal.h>
