/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_Alpha_H_
#define	_Alpha_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Alpha {
	Alpha_alpha0	= 0,
	Alpha_alpha04	= 1,
	Alpha_alpha05	= 2,
	Alpha_alpha06	= 3,
	Alpha_alpha07	= 4,
	Alpha_alpha08	= 5,
	Alpha_alpha09	= 6,
	Alpha_alpha1	= 7
} e_Alpha;

/* Alpha */
typedef long	 Alpha_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_Alpha_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_Alpha;
extern const asn_INTEGER_specifics_t asn_SPC_Alpha_specs_1;
asn_struct_free_f Alpha_free;
asn_struct_print_f Alpha_print;
asn_constr_check_f Alpha_constraint;
ber_type_decoder_f Alpha_decode_ber;
der_type_encoder_f Alpha_encode_der;
xer_type_decoder_f Alpha_decode_xer;
xer_type_encoder_f Alpha_encode_xer;
oer_type_decoder_f Alpha_decode_oer;
oer_type_encoder_f Alpha_encode_oer;
per_type_decoder_f Alpha_decode_uper;
per_type_encoder_f Alpha_encode_uper;
per_type_decoder_f Alpha_decode_aper;
per_type_encoder_f Alpha_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _Alpha_H_ */
#include <asn_internal.h>
