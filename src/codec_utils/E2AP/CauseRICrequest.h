/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CauseRICrequest_H_
#define	_CauseRICrequest_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CauseRICrequest {
	CauseRICrequest_ran_function_id_invalid	= 0,
	CauseRICrequest_action_not_supported	= 1,
	CauseRICrequest_excessive_actions	= 2,
	CauseRICrequest_duplicate_action	= 3,
	CauseRICrequest_duplicate_event_trigger	= 4,
	CauseRICrequest_function_resource_limit	= 5,
	CauseRICrequest_request_id_unknown	= 6,
	CauseRICrequest_inconsistent_action_subsequent_action_sequence	= 7,
	CauseRICrequest_control_message_invalid	= 8,
	CauseRICrequest_ric_call_process_id_invalid	= 9,
	CauseRICrequest_control_timer_expired	= 10,
	CauseRICrequest_control_failed_to_execute	= 11,
	CauseRICrequest_system_not_ready	= 12,
	CauseRICrequest_unspecified	= 13
	/*
	 * Enumeration is extensible
	 */
} e_CauseRICrequest;

/* CauseRICrequest */
typedef long	 CauseRICrequest_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_CauseRICrequest_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_CauseRICrequest;
extern const asn_INTEGER_specifics_t asn_SPC_CauseRICrequest_specs_1;
asn_struct_free_f CauseRICrequest_free;
asn_struct_print_f CauseRICrequest_print;
asn_constr_check_f CauseRICrequest_constraint;
ber_type_decoder_f CauseRICrequest_decode_ber;
der_type_encoder_f CauseRICrequest_encode_der;
xer_type_decoder_f CauseRICrequest_decode_xer;
xer_type_encoder_f CauseRICrequest_encode_xer;
oer_type_decoder_f CauseRICrequest_decode_oer;
oer_type_encoder_f CauseRICrequest_encode_oer;
per_type_decoder_f CauseRICrequest_decode_uper;
per_type_encoder_f CauseRICrequest_encode_uper;
per_type_decoder_f CauseRICrequest_decode_aper;
per_type_encoder_f CauseRICrequest_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _CauseRICrequest_H_ */
#include <asn_internal.h>
