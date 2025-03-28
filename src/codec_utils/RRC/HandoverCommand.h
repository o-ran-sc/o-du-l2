/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
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

/* Referred external types */
#include "HandoverCommand-IEs.h"

#endif	/* _HandoverCommand_H_ */
#include <asn_internal.h>
