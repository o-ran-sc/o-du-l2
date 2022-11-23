/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_ResumeCause_H_
#define	_ResumeCause_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ResumeCause {
	ResumeCause_emergency	= 0,
	ResumeCause_highPriorityAccess	= 1,
	ResumeCause_mt_Access	= 2,
	ResumeCause_mo_Signalling	= 3,
	ResumeCause_mo_Data	= 4,
	ResumeCause_mo_VoiceCall	= 5,
	ResumeCause_mo_VideoCall	= 6,
	ResumeCause_mo_SMS	= 7,
	ResumeCause_rna_Update	= 8,
	ResumeCause_mps_PriorityAccess	= 9,
	ResumeCause_mcs_PriorityAccess	= 10,
	ResumeCause_spare1	= 11,
	ResumeCause_spare2	= 12,
	ResumeCause_spare3	= 13,
	ResumeCause_spare4	= 14,
	ResumeCause_spare5	= 15
} e_ResumeCause;

/* ResumeCause */
typedef long	 ResumeCause_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ResumeCause_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ResumeCause;
extern const asn_INTEGER_specifics_t asn_SPC_ResumeCause_specs_1;
asn_struct_free_f ResumeCause_free;
asn_struct_print_f ResumeCause_print;
asn_constr_check_f ResumeCause_constraint;
ber_type_decoder_f ResumeCause_decode_ber;
der_type_encoder_f ResumeCause_encode_der;
xer_type_decoder_f ResumeCause_decode_xer;
xer_type_encoder_f ResumeCause_encode_xer;
oer_type_decoder_f ResumeCause_decode_oer;
oer_type_encoder_f ResumeCause_encode_oer;
per_type_decoder_f ResumeCause_decode_uper;
per_type_encoder_f ResumeCause_encode_uper;
per_type_decoder_f ResumeCause_decode_aper;
per_type_encoder_f ResumeCause_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _ResumeCause_H_ */
#include <asn_internal.h>
