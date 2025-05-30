/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_RRCSetup_H_
#define	_RRCSetup_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RRC-TransactionIdentifier.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RRCSetup__criticalExtensions_PR {
	RRCSetup__criticalExtensions_PR_NOTHING,	/* No components present */
	RRCSetup__criticalExtensions_PR_rrcSetup,
	RRCSetup__criticalExtensions_PR_criticalExtensionsFuture
} RRCSetup__criticalExtensions_PR;

/* Forward declarations */
struct RRCSetup_IEs;

/* RRCSetup */
typedef struct RRCSetup {
	RRC_TransactionIdentifier_t	 rrc_TransactionIdentifier;
	struct RRCSetup__criticalExtensions {
		RRCSetup__criticalExtensions_PR present;
		union RRCSetup__criticalExtensions_u {
			struct RRCSetup_IEs	*rrcSetup;
			struct RRCSetup__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCSetup_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCSetup;
extern asn_SEQUENCE_specifics_t asn_SPC_RRCSetup_specs_1;
extern asn_TYPE_member_t asn_MBR_RRCSetup_1[2];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RRCSetup-IEs.h"

#endif	/* _RRCSetup_H_ */
#include <asn_internal.h>
