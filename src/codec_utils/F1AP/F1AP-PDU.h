/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Descriptions"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_F1AP_PDU_H_
#define	_F1AP_PDU_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum F1AP_PDU_PR {
	F1AP_PDU_PR_NOTHING,	/* No components present */
	F1AP_PDU_PR_initiatingMessage,
	F1AP_PDU_PR_successfulOutcome,
	F1AP_PDU_PR_unsuccessfulOutcome,
	F1AP_PDU_PR_choice_extension
} F1AP_PDU_PR;

/* Forward declarations */
struct InitiatingMessage;
struct SuccessfulOutcome;
struct UnsuccessfulOutcome;
struct ProtocolIE_SingleContainer;

/* F1AP-PDU */
typedef struct F1AP_PDU {
	F1AP_PDU_PR present;
	union F1AP_PDU_u {
		struct InitiatingMessage	*initiatingMessage;
		struct SuccessfulOutcome	*successfulOutcome;
		struct UnsuccessfulOutcome	*unsuccessfulOutcome;
		struct ProtocolIE_SingleContainer	*choice_extension;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} F1AP_PDU_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_F1AP_PDU;

#ifdef __cplusplus
}
#endif

#endif	/* _F1AP_PDU_H_ */
#include <asn_internal.h>
