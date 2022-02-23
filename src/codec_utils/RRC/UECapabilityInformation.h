/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_UECapabilityInformation_H_
#define	_UECapabilityInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RRC-TransactionIdentifier.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum UECapabilityInformation__criticalExtensions_PR {
	UECapabilityInformation__criticalExtensions_PR_NOTHING,	/* No components present */
	UECapabilityInformation__criticalExtensions_PR_ueCapabilityInformation,
	UECapabilityInformation__criticalExtensions_PR_criticalExtensionsFuture
} UECapabilityInformation__criticalExtensions_PR;

/* Forward declarations */
struct UECapabilityInformation_IEs;

/* UECapabilityInformation */
typedef struct UECapabilityInformation {
	RRC_TransactionIdentifier_t	 rrc_TransactionIdentifier;
	struct UECapabilityInformation__criticalExtensions {
		UECapabilityInformation__criticalExtensions_PR present;
		union UECapabilityInformation__criticalExtensions_u {
			struct UECapabilityInformation_IEs	*ueCapabilityInformation;
			struct UECapabilityInformation__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UECapabilityInformation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UECapabilityInformation;
extern asn_SEQUENCE_specifics_t asn_SPC_UECapabilityInformation_specs_1;
extern asn_TYPE_member_t asn_MBR_UECapabilityInformation_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _UECapabilityInformation_H_ */
#include <asn_internal.h>
