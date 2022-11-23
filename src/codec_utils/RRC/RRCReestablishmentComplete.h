/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_RRCReestablishmentComplete_H_
#define	_RRCReestablishmentComplete_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RRC-TransactionIdentifier.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RRCReestablishmentComplete__criticalExtensions_PR {
	RRCReestablishmentComplete__criticalExtensions_PR_NOTHING,	/* No components present */
	RRCReestablishmentComplete__criticalExtensions_PR_rrcReestablishmentComplete,
	RRCReestablishmentComplete__criticalExtensions_PR_criticalExtensionsFuture
} RRCReestablishmentComplete__criticalExtensions_PR;

/* Forward declarations */
struct RRCReestablishmentComplete_IEs;

/* RRCReestablishmentComplete */
typedef struct RRCReestablishmentComplete {
	RRC_TransactionIdentifier_t	 rrc_TransactionIdentifier;
	struct RRCReestablishmentComplete__criticalExtensions {
		RRCReestablishmentComplete__criticalExtensions_PR present;
		union RRCReestablishmentComplete__criticalExtensions_u {
			struct RRCReestablishmentComplete_IEs	*rrcReestablishmentComplete;
			struct RRCReestablishmentComplete__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCReestablishmentComplete_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCReestablishmentComplete;
extern asn_SEQUENCE_specifics_t asn_SPC_RRCReestablishmentComplete_specs_1;
extern asn_TYPE_member_t asn_MBR_RRCReestablishmentComplete_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _RRCReestablishmentComplete_H_ */
#include <asn_internal.h>
