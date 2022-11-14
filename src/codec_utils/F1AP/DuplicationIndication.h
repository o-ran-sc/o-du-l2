/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_DuplicationIndication_H_
#define	_DuplicationIndication_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DuplicationIndication {
	DuplicationIndication_true	= 0,
	/*
	 * Enumeration is extensible
	 */
	DuplicationIndication_false	= 1
} e_DuplicationIndication;

/* DuplicationIndication */
typedef long	 DuplicationIndication_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_DuplicationIndication_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_DuplicationIndication;
extern const asn_INTEGER_specifics_t asn_SPC_DuplicationIndication_specs_1;
asn_struct_free_f DuplicationIndication_free;
asn_struct_print_f DuplicationIndication_print;
asn_constr_check_f DuplicationIndication_constraint;
ber_type_decoder_f DuplicationIndication_decode_ber;
der_type_encoder_f DuplicationIndication_encode_der;
xer_type_decoder_f DuplicationIndication_decode_xer;
xer_type_encoder_f DuplicationIndication_encode_xer;
oer_type_decoder_f DuplicationIndication_decode_oer;
oer_type_encoder_f DuplicationIndication_encode_oer;
per_type_decoder_f DuplicationIndication_decode_uper;
per_type_encoder_f DuplicationIndication_encode_uper;
per_type_decoder_f DuplicationIndication_decode_aper;
per_type_encoder_f DuplicationIndication_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _DuplicationIndication_H_ */
#include <asn_internal.h>
