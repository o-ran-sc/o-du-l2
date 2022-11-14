/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_EUTRA_CyclicPrefixUL_H_
#define	_EUTRA_CyclicPrefixUL_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EUTRA_CyclicPrefixUL {
	EUTRA_CyclicPrefixUL_normal	= 0,
	EUTRA_CyclicPrefixUL_extended	= 1
	/*
	 * Enumeration is extensible
	 */
} e_EUTRA_CyclicPrefixUL;

/* EUTRA-CyclicPrefixUL */
typedef long	 EUTRA_CyclicPrefixUL_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_EUTRA_CyclicPrefixUL_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_EUTRA_CyclicPrefixUL;
extern const asn_INTEGER_specifics_t asn_SPC_EUTRA_CyclicPrefixUL_specs_1;
asn_struct_free_f EUTRA_CyclicPrefixUL_free;
asn_struct_print_f EUTRA_CyclicPrefixUL_print;
asn_constr_check_f EUTRA_CyclicPrefixUL_constraint;
ber_type_decoder_f EUTRA_CyclicPrefixUL_decode_ber;
der_type_encoder_f EUTRA_CyclicPrefixUL_encode_der;
xer_type_decoder_f EUTRA_CyclicPrefixUL_decode_xer;
xer_type_encoder_f EUTRA_CyclicPrefixUL_encode_xer;
oer_type_decoder_f EUTRA_CyclicPrefixUL_decode_oer;
oer_type_encoder_f EUTRA_CyclicPrefixUL_encode_oer;
per_type_decoder_f EUTRA_CyclicPrefixUL_decode_uper;
per_type_encoder_f EUTRA_CyclicPrefixUL_encode_uper;
per_type_decoder_f EUTRA_CyclicPrefixUL_decode_aper;
per_type_encoder_f EUTRA_CyclicPrefixUL_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _EUTRA_CyclicPrefixUL_H_ */
#include <asn_internal.h>
