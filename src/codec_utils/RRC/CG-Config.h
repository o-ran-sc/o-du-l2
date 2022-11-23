/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_CG_Config_H_
#define	_CG_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CG_Config__criticalExtensions_PR {
	CG_Config__criticalExtensions_PR_NOTHING,	/* No components present */
	CG_Config__criticalExtensions_PR_c1,
	CG_Config__criticalExtensions_PR_criticalExtensionsFuture
} CG_Config__criticalExtensions_PR;
typedef enum CG_Config__criticalExtensions__c1_PR {
	CG_Config__criticalExtensions__c1_PR_NOTHING,	/* No components present */
	CG_Config__criticalExtensions__c1_PR_cg_Config,
	CG_Config__criticalExtensions__c1_PR_spare3,
	CG_Config__criticalExtensions__c1_PR_spare2,
	CG_Config__criticalExtensions__c1_PR_spare1
} CG_Config__criticalExtensions__c1_PR;

/* Forward declarations */
struct CG_Config_IEs;

/* CG-Config */
typedef struct CG_Config {
	struct CG_Config__criticalExtensions {
		CG_Config__criticalExtensions_PR present;
		union CG_Config__criticalExtensions_u {
			struct CG_Config__criticalExtensions__c1 {
				CG_Config__criticalExtensions__c1_PR present;
				union CG_Config__criticalExtensions__c1_u {
					struct CG_Config_IEs	*cg_Config;
					NULL_t	 spare3;
					NULL_t	 spare2;
					NULL_t	 spare1;
				} choice;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *c1;
			struct CG_Config__criticalExtensions__criticalExtensionsFuture {
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *criticalExtensionsFuture;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} criticalExtensions;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CG_Config_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CG_Config;

#ifdef __cplusplus
}
#endif

#endif	/* _CG_Config_H_ */
#include <asn_internal.h>
