/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_CauseRadioNetwork_H_
#define	_CauseRadioNetwork_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CauseRadioNetwork {
	CauseRadioNetwork_unspecified	= 0,
	CauseRadioNetwork_rl_failure_rlc	= 1,
	CauseRadioNetwork_unknown_or_already_allocated_gnb_cu_ue_f1ap_id	= 2,
	CauseRadioNetwork_unknown_or_already_allocated_gnd_du_ue_f1ap_id	= 3,
	CauseRadioNetwork_unknown_or_inconsistent_pair_of_ue_f1ap_id	= 4,
	CauseRadioNetwork_interaction_with_other_procedure	= 5,
	CauseRadioNetwork_not_supported_qci_Value	= 6,
	CauseRadioNetwork_action_desirable_for_radio_reasons	= 7,
	CauseRadioNetwork_no_radio_resources_available	= 8,
	CauseRadioNetwork_procedure_cancelled	= 9,
	CauseRadioNetwork_normal_release	= 10,
	/*
	 * Enumeration is extensible
	 */
	CauseRadioNetwork_cell_not_available	= 11,
	CauseRadioNetwork_rl_failure_others	= 12,
	CauseRadioNetwork_ue_rejection	= 13,
	CauseRadioNetwork_resources_not_available_for_the_slice	= 14,
	CauseRadioNetwork_amf_initiated_abnormal_release	= 15,
	CauseRadioNetwork_release_due_to_pre_emption	= 16
} e_CauseRadioNetwork;

/* CauseRadioNetwork */
typedef long	 CauseRadioNetwork_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_CauseRadioNetwork_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_CauseRadioNetwork;
extern const asn_INTEGER_specifics_t asn_SPC_CauseRadioNetwork_specs_1;
asn_struct_free_f CauseRadioNetwork_free;
asn_struct_print_f CauseRadioNetwork_print;
asn_constr_check_f CauseRadioNetwork_constraint;
xer_type_decoder_f CauseRadioNetwork_decode_xer;
xer_type_encoder_f CauseRadioNetwork_encode_xer;
per_type_decoder_f CauseRadioNetwork_decode_uper;
per_type_encoder_f CauseRadioNetwork_encode_uper;
per_type_decoder_f CauseRadioNetwork_decode_aper;
per_type_encoder_f CauseRadioNetwork_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _CauseRadioNetwork_H_ */
#include <asn_internal.h>
