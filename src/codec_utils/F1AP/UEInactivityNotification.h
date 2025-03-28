/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Contents"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_UEInactivityNotification_H_
#define	_UEInactivityNotification_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtocolIE-Container.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UEInactivityNotification */
typedef struct UEInactivityNotification {
	ProtocolIE_Container_4587P30_t	 protocolIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UEInactivityNotification_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UEInactivityNotification;
extern asn_SEQUENCE_specifics_t asn_SPC_UEInactivityNotification_specs_1;
extern asn_TYPE_member_t asn_MBR_UEInactivityNotification_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _UEInactivityNotification_H_ */
#include <asn_internal.h>
