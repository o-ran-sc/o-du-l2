/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_OtherConfig_H_
#define	_OtherConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum OtherConfig__delayBudgetReportingConfig_PR {
	OtherConfig__delayBudgetReportingConfig_PR_NOTHING,	/* No components present */
	OtherConfig__delayBudgetReportingConfig_PR_release,
	OtherConfig__delayBudgetReportingConfig_PR_setup
} OtherConfig__delayBudgetReportingConfig_PR;
typedef enum OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer {
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s0	= 0,
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s0dot4	= 1,
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s0dot8	= 2,
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s1dot6	= 3,
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s3	= 4,
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s6	= 5,
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s12	= 6,
	OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer_s30	= 7
} e_OtherConfig__delayBudgetReportingConfig__setup__delayBudgetReportingProhibitTimer;

/* OtherConfig */
typedef struct OtherConfig {
	struct OtherConfig__delayBudgetReportingConfig {
		OtherConfig__delayBudgetReportingConfig_PR present;
		union OtherConfig__delayBudgetReportingConfig_u {
			NULL_t	 release;
			struct OtherConfig__delayBudgetReportingConfig__setup {
				long	 delayBudgetReportingProhibitTimer;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *delayBudgetReportingConfig;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} OtherConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_delayBudgetReportingProhibitTimer_5;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_OtherConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_OtherConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_OtherConfig_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _OtherConfig_H_ */
#include <asn_internal.h>
