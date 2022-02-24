/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CounterCheckResponse_H_
#define	_CounterCheckResponse_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RRC-TransactionIdentifier.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CounterCheckResponse__criticalExtensions_PR {
	CounterCheckResponse__criticalExtensions_PR_NOTHING,	/* No components present */
	CounterCheckResponse__criticalExtensions_PR_counterCheckResponse,
	CounterCheckResponse__criticalExtensions_PR_criticalExtensionsFuture
} CounterCheckResponse__criticalExtensions_PR;

/* Forward declarations */
struct CounterCheckResponse_IEs;

/* CounterCheckResponse */
typedef struct CounterCheckResponse {
	RRC_TransactionIdentifier_t	 rrc_TransactionIdentifier;
	struct CounterCheckResponse__criticalExtensions {
		CounterCheckResponse__criticalExtensions_PR present;
		union CounterCheckResponse__criticalExtensions_u {
			struct CounterCheckResponse_IEs	*counterCheckResponse;
			struct CounterCheckResponse__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CounterCheckResponse_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CounterCheckResponse;
extern asn_SEQUENCE_specifics_t asn_SPC_CounterCheckResponse_specs_1;
extern asn_TYPE_member_t asn_MBR_CounterCheckResponse_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _CounterCheckResponse_H_ */
#include <asn_internal.h>
