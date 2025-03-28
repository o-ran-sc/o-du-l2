/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_RRCReject_H_
#define	_RRCReject_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RRCReject__criticalExtensions_PR {
	RRCReject__criticalExtensions_PR_NOTHING,	/* No components present */
	RRCReject__criticalExtensions_PR_rrcReject,
	RRCReject__criticalExtensions_PR_criticalExtensionsFuture
} RRCReject__criticalExtensions_PR;

/* Forward declarations */
struct RRCReject_IEs;

/* RRCReject */
typedef struct RRCReject {
	struct RRCReject__criticalExtensions {
		RRCReject__criticalExtensions_PR present;
		union RRCReject__criticalExtensions_u {
			struct RRCReject_IEs	*rrcReject;
			struct RRCReject__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCReject_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCReject;
extern asn_SEQUENCE_specifics_t asn_SPC_RRCReject_specs_1;
extern asn_TYPE_member_t asn_MBR_RRCReject_1[1];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RRCReject-IEs.h"

#endif	/* _RRCReject_H_ */
#include <asn_internal.h>
