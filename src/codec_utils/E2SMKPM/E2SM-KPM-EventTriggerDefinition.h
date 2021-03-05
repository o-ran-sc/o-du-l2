/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_E2SM_KPM_EventTriggerDefinition_H_
#define	_E2SM_KPM_EventTriggerDefinition_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum E2SM_KPM_EventTriggerDefinition_PR {
	E2SM_KPM_EventTriggerDefinition_PR_NOTHING,	/* No components present */
	E2SM_KPM_EventTriggerDefinition_PR_eventDefinition_Format1
	/* Extensions may appear below */
	
} E2SM_KPM_EventTriggerDefinition_PR;

/* Forward declarations */
struct E2SM_KPM_EventTriggerDefinition_Format1;

/* E2SM-KPM-EventTriggerDefinition */
typedef struct E2SM_KPM_EventTriggerDefinition {
	E2SM_KPM_EventTriggerDefinition_PR present;
	union E2SM_KPM_EventTriggerDefinition_u {
		struct E2SM_KPM_EventTriggerDefinition_Format1	*eventDefinition_Format1;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E2SM_KPM_EventTriggerDefinition_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E2SM_KPM_EventTriggerDefinition;

#ifdef __cplusplus
}
#endif

#endif	/* _E2SM_KPM_EventTriggerDefinition_H_ */
#include <asn_internal.h>
