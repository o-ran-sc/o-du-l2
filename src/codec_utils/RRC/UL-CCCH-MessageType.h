/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_UL_CCCH_MessageType_H_
#define	_UL_CCCH_MessageType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum UL_CCCH_MessageType_PR {
	UL_CCCH_MessageType_PR_NOTHING,	/* No components present */
	UL_CCCH_MessageType_PR_c1,
	UL_CCCH_MessageType_PR_messageClassExtension
} UL_CCCH_MessageType_PR;
typedef enum UL_CCCH_MessageType__c1_PR {
	UL_CCCH_MessageType__c1_PR_NOTHING,	/* No components present */
	UL_CCCH_MessageType__c1_PR_rrcSetupRequest,
	UL_CCCH_MessageType__c1_PR_rrcResumeRequest,
	UL_CCCH_MessageType__c1_PR_rrcReestablishmentRequest,
	UL_CCCH_MessageType__c1_PR_rrcSystemInfoRequest
} UL_CCCH_MessageType__c1_PR;

/* Forward declarations */
struct RRCSetupRequest;
struct RRCResumeRequest;
struct RRCReestablishmentRequest;
struct RRCSystemInfoRequest;

/* UL-CCCH-MessageType */
typedef struct UL_CCCH_MessageType {
	UL_CCCH_MessageType_PR present;
	union UL_CCCH_MessageType_u {
		struct UL_CCCH_MessageType__c1 {
			UL_CCCH_MessageType__c1_PR present;
			union UL_CCCH_MessageType__c1_u {
				struct RRCSetupRequest	*rrcSetupRequest;
				struct RRCResumeRequest	*rrcResumeRequest;
				struct RRCReestablishmentRequest	*rrcReestablishmentRequest;
				struct RRCSystemInfoRequest	*rrcSystemInfoRequest;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *c1;
		struct UL_CCCH_MessageType__messageClassExtension {
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *messageClassExtension;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UL_CCCH_MessageType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UL_CCCH_MessageType;
extern asn_CHOICE_specifics_t asn_SPC_UL_CCCH_MessageType_specs_1;
extern asn_TYPE_member_t asn_MBR_UL_CCCH_MessageType_1[2];
extern asn_per_constraints_t asn_PER_type_UL_CCCH_MessageType_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _UL_CCCH_MessageType_H_ */
#include <asn_internal.h>
