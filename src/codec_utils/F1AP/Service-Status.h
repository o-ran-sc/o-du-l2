/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_Service_Status_H_
#define	_Service_Status_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Service-State.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Service_Status__switchingOffOngoing {
	Service_Status__switchingOffOngoing_true	= 0
	/*
	 * Enumeration is extensible
	 */
} e_Service_Status__switchingOffOngoing;

/* Forward declarations */
struct ProtocolExtensionContainer;

/* Service-Status */
typedef struct Service_Status {
	Service_State_t	 service_state;
	long	*switchingOffOngoing;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Service_Status_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_switchingOffOngoing_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_Service_Status;
extern asn_SEQUENCE_specifics_t asn_SPC_Service_Status_specs_1;
extern asn_TYPE_member_t asn_MBR_Service_Status_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _Service_Status_H_ */
#include <asn_internal.h>
