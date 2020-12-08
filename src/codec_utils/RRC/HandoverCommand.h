/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_HandoverCommand_H_
#define	_HandoverCommand_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum HandoverCommand__criticalExtensions_PR {
	HandoverCommand__criticalExtensions_PR_NOTHING,	/* No components present */
	HandoverCommand__criticalExtensions_PR_c1,
	HandoverCommand__criticalExtensions_PR_criticalExtensionsFuture
} HandoverCommand__criticalExtensions_PR;
typedef enum HandoverCommand__criticalExtensions__c1_PR {
	HandoverCommand__criticalExtensions__c1_PR_NOTHING,	/* No components present */
	HandoverCommand__criticalExtensions__c1_PR_handoverCommand,
	HandoverCommand__criticalExtensions__c1_PR_spare3,
	HandoverCommand__criticalExtensions__c1_PR_spare2,
	HandoverCommand__criticalExtensions__c1_PR_spare1
} HandoverCommand__criticalExtensions__c1_PR;

/* Forward declarations */
struct HandoverCommand_IEs;

/* HandoverCommand */
typedef struct HandoverCommand {
	struct HandoverCommand__criticalExtensions {
		HandoverCommand__criticalExtensions_PR present;
		union HandoverCommand__criticalExtensions_u {
			struct HandoverCommand__criticalExtensions__c1 {
				HandoverCommand__criticalExtensions__c1_PR present;
				union HandoverCommand__criticalExtensions__c1_u {
					struct HandoverCommand_IEs	*handoverCommand;
					NULL_t	 spare3;
					NULL_t	 spare2;
					NULL_t	 spare1;
				} choice;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *c1;
			struct HandoverCommand__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} HandoverCommand_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HandoverCommand;

#ifdef __cplusplus
}
#endif

#endif	/* _HandoverCommand_H_ */
#include <asn_internal.h>
