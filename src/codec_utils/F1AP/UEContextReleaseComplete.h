/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Contents"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_UEContextReleaseComplete_H_
#define	_UEContextReleaseComplete_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-Container.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UEContextReleaseComplete */
typedef struct UEContextReleaseComplete {
	ProtocolIE_Container_4587P19_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UEContextReleaseComplete_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UEContextReleaseComplete;
extern asn_SEQUENCE_specifics_t asn_SPC_UEContextReleaseComplete_specs_1;
extern asn_TYPE_member_t asn_MBR_UEContextReleaseComplete_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _UEContextReleaseComplete_H_ */
#include <asn_internal.h>
