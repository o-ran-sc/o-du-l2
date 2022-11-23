/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_DL_DCCH_MessageType_H_
#define	_DL_DCCH_MessageType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DL_DCCH_MessageType_PR {
	DL_DCCH_MessageType_PR_NOTHING,	/* No components present */
	DL_DCCH_MessageType_PR_c1,
	DL_DCCH_MessageType_PR_messageClassExtension
} DL_DCCH_MessageType_PR;
typedef enum DL_DCCH_MessageType__c1_PR {
	DL_DCCH_MessageType__c1_PR_NOTHING,	/* No components present */
	DL_DCCH_MessageType__c1_PR_rrcReconfiguration,
	DL_DCCH_MessageType__c1_PR_rrcResume,
	DL_DCCH_MessageType__c1_PR_rrcRelease,
	DL_DCCH_MessageType__c1_PR_rrcReestablishment,
	DL_DCCH_MessageType__c1_PR_securityModeCommand,
	DL_DCCH_MessageType__c1_PR_dlInformationTransfer,
	DL_DCCH_MessageType__c1_PR_ueCapabilityEnquiry,
	DL_DCCH_MessageType__c1_PR_counterCheck,
	DL_DCCH_MessageType__c1_PR_mobilityFromNRCommand,
	DL_DCCH_MessageType__c1_PR_spare7,
	DL_DCCH_MessageType__c1_PR_spare6,
	DL_DCCH_MessageType__c1_PR_spare5,
	DL_DCCH_MessageType__c1_PR_spare4,
	DL_DCCH_MessageType__c1_PR_spare3,
	DL_DCCH_MessageType__c1_PR_spare2,
	DL_DCCH_MessageType__c1_PR_spare1
} DL_DCCH_MessageType__c1_PR;

/* Forward declarations */
struct RRCReconfiguration;
struct RRCResume;
struct RRCRelease;
struct RRCReestablishment;
struct SecurityModeCommand;
struct DLInformationTransfer;
struct UECapabilityEnquiry;
struct CounterCheck;
struct MobilityFromNRCommand;

/* DL-DCCH-MessageType */
typedef struct DL_DCCH_MessageType {
	DL_DCCH_MessageType_PR present;
	union DL_DCCH_MessageType_u {
		struct DL_DCCH_MessageType__c1 {
			DL_DCCH_MessageType__c1_PR present;
			union DL_DCCH_MessageType__c1_u {
				struct RRCReconfiguration	*rrcReconfiguration;
				struct RRCResume	*rrcResume;
				struct RRCRelease	*rrcRelease;
				struct RRCReestablishment	*rrcReestablishment;
				struct SecurityModeCommand	*securityModeCommand;
				struct DLInformationTransfer	*dlInformationTransfer;
				struct UECapabilityEnquiry	*ueCapabilityEnquiry;
				struct CounterCheck	*counterCheck;
				struct MobilityFromNRCommand	*mobilityFromNRCommand;
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
		struct DL_DCCH_MessageType__messageClassExtension {
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *messageClassExtension;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DL_DCCH_MessageType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DL_DCCH_MessageType;
extern asn_CHOICE_specifics_t asn_SPC_DL_DCCH_MessageType_specs_1;
extern asn_TYPE_member_t asn_MBR_DL_DCCH_MessageType_1[2];
extern asn_per_constraints_t asn_PER_type_DL_DCCH_MessageType_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _DL_DCCH_MessageType_H_ */
#include <asn_internal.h>
