/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_UERadioAccessCapabilityInformation_H_
#define	_UERadioAccessCapabilityInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum UERadioAccessCapabilityInformation__criticalExtensions_PR {
	UERadioAccessCapabilityInformation__criticalExtensions_PR_NOTHING,	/* No components present */
	UERadioAccessCapabilityInformation__criticalExtensions_PR_c1,
	UERadioAccessCapabilityInformation__criticalExtensions_PR_criticalExtensionsFuture
} UERadioAccessCapabilityInformation__criticalExtensions_PR;
typedef enum UERadioAccessCapabilityInformation__criticalExtensions__c1_PR {
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_NOTHING,	/* No components present */
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_ueRadioAccessCapabilityInformation,
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_spare7,
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_spare6,
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_spare5,
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_spare4,
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_spare3,
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_spare2,
	UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_spare1
} UERadioAccessCapabilityInformation__criticalExtensions__c1_PR;

/* Forward declarations */
struct UERadioAccessCapabilityInformation_IEs;

/* UERadioAccessCapabilityInformation */
typedef struct UERadioAccessCapabilityInformation {
	struct UERadioAccessCapabilityInformation__criticalExtensions {
		UERadioAccessCapabilityInformation__criticalExtensions_PR present;
		union UERadioAccessCapabilityInformation__criticalExtensions_u {
			struct UERadioAccessCapabilityInformation__criticalExtensions__c1 {
				UERadioAccessCapabilityInformation__criticalExtensions__c1_PR present;
				union UERadioAccessCapabilityInformation__criticalExtensions__c1_u {
					struct UERadioAccessCapabilityInformation_IEs	*ueRadioAccessCapabilityInformation;
					NULL_t	 spare7;
					NULL_t	 spare6;
					NULL_t	 spare5;
					NULL_t	 spare4;
					NULL_t	 spare3;
					NULL_t	 spare2;
					NULL_t	 spare1;
				} choice;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *c1;
			struct UERadioAccessCapabilityInformation__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UERadioAccessCapabilityInformation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UERadioAccessCapabilityInformation;

#ifdef __cplusplus
}
#endif

#endif	/* _UERadioAccessCapabilityInformation_H_ */
#include <asn_internal.h>
