/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-COMMON-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_SM_v2.01.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_Interface_MessageID_H_
#define	_Interface_MessageID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Interface_MessageID__messageType {
	Interface_MessageID__messageType_initiatingMessage	= 0,
	Interface_MessageID__messageType_successfulOutcome	= 1,
	Interface_MessageID__messageType_unsuccessfulOutcome	= 2
	/*
	 * Enumeration is extensible
	 */
} e_Interface_MessageID__messageType;

/* Interface-MessageID */
typedef struct Interface_MessageID {
	long	 interfaceProcedureID;
	long	 messageType;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Interface_MessageID_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_messageType_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_Interface_MessageID;

#ifdef __cplusplus
}
#endif

#endif	/* _Interface_MessageID_H_ */
#include <asn_internal.h>
