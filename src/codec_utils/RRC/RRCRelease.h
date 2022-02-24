/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RRCRelease_H_
#define	_RRCRelease_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RRC-TransactionIdentifier.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RRCRelease__criticalExtensions_PR {
	RRCRelease__criticalExtensions_PR_NOTHING,	/* No components present */
	RRCRelease__criticalExtensions_PR_rrcRelease,
	RRCRelease__criticalExtensions_PR_criticalExtensionsFuture
} RRCRelease__criticalExtensions_PR;

/* Forward declarations */
struct RRCRelease_IEs;

/* RRCRelease */
typedef struct RRCRelease {
	RRC_TransactionIdentifier_t	 rrc_TransactionIdentifier;
	struct RRCRelease__criticalExtensions {
		RRCRelease__criticalExtensions_PR present;
		union RRCRelease__criticalExtensions_u {
			struct RRCRelease_IEs	*rrcRelease;
			struct RRCRelease__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCRelease_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRCRelease;
extern asn_SEQUENCE_specifics_t asn_SPC_RRCRelease_specs_1;
extern asn_TYPE_member_t asn_MBR_RRCRelease_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _RRCRelease_H_ */
#include <asn_internal.h>
