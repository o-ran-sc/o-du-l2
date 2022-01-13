/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Descriptions"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_E2AP_PDU_H_
#define	_E2AP_PDU_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum E2AP_PDU_PR {
	E2AP_PDU_PR_NOTHING,	/* No components present */
	E2AP_PDU_PR_initiatingMessage,
	E2AP_PDU_PR_successfulOutcome,
	E2AP_PDU_PR_unsuccessfulOutcome
	/* Extensions may appear below */
	
} E2AP_PDU_PR;

/* Forward declarations */
struct InitiatingMessageE2;
struct SuccessfulOutcomeE2;
struct UnsuccessfulOutcomeE2;

/* E2AP-PDU */
typedef struct E2AP_PDU {
	E2AP_PDU_PR present;
	union E2AP_PDU_u {
		struct InitiatingMessageE2	*initiatingMessage;
		struct SuccessfulOutcomeE2	*successfulOutcome;
		struct UnsuccessfulOutcomeE2	*unsuccessfulOutcome;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E2AP_PDU_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E2AP_PDU;

#ifdef __cplusplus
}
#endif

#endif	/* _E2AP_PDU_H_ */
#include <asn_internal.h>
