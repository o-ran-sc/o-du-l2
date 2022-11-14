/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_SliceSupportItem_H_
#define	_SliceSupportItem_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SNSSAI.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* SliceSupportItem */
typedef struct SliceSupportItem {
	SNSSAI_t	 sNSSAI;
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SliceSupportItem_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SliceSupportItem;
extern asn_SEQUENCE_specifics_t asn_SPC_SliceSupportItem_specs_1;
extern asn_TYPE_member_t asn_MBR_SliceSupportItem_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _SliceSupportItem_H_ */
#include <asn_internal.h>
