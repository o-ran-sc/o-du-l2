/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_DL_CCCH_MessageType_H_
#define	_DL_CCCH_MessageType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DL_CCCH_MessageType_PR {
	DL_CCCH_MessageType_PR_NOTHING,	/* No components present */
	DL_CCCH_MessageType_PR_c1,
	DL_CCCH_MessageType_PR_messageClassExtension
} DL_CCCH_MessageType_PR;
typedef enum DL_CCCH_MessageType__c1_PR {
	DL_CCCH_MessageType__c1_PR_NOTHING,	/* No components present */
	DL_CCCH_MessageType__c1_PR_rrcReject,
	DL_CCCH_MessageType__c1_PR_rrcSetup,
	DL_CCCH_MessageType__c1_PR_spare2,
	DL_CCCH_MessageType__c1_PR_spare1
} DL_CCCH_MessageType__c1_PR;

/* Forward declarations */
struct RRCReject;
struct RRCSetup;

/* DL-CCCH-MessageType */
typedef struct DL_CCCH_MessageType {
	DL_CCCH_MessageType_PR present;
	union DL_CCCH_MessageType_u {
		struct DL_CCCH_MessageType__c1 {
			DL_CCCH_MessageType__c1_PR present;
			union DL_CCCH_MessageType__c1_u {
				struct RRCReject	*rrcReject;
				struct RRCSetup	*rrcSetup;
				NULL_t	 spare2;
				NULL_t	 spare1;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *c1;
		struct DL_CCCH_MessageType__messageClassExtension {
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *messageClassExtension;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DL_CCCH_MessageType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DL_CCCH_MessageType;
extern asn_CHOICE_specifics_t asn_SPC_DL_CCCH_MessageType_specs_1;
extern asn_TYPE_member_t asn_MBR_DL_CCCH_MessageType_1[2];
extern asn_per_constraints_t asn_PER_type_DL_CCCH_MessageType_constr_1;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RRCReject.h"
#include "RRCSetup.h"

#endif	/* _DL_CCCH_MessageType_H_ */
#include <asn_internal.h>
