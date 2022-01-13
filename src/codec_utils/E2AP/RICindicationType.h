/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RICindicationType_H_
#define	_RICindicationType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RICindicationType {
	RICindicationType_report	= 0,
	RICindicationType_insert	= 1
	/*
	 * Enumeration is extensible
	 */
} e_RICindicationType;

/* RICindicationType */
typedef long	 RICindicationType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RICindicationType;
asn_struct_free_f RICindicationType_free;
asn_struct_print_f RICindicationType_print;
asn_constr_check_f RICindicationType_constraint;
ber_type_decoder_f RICindicationType_decode_ber;
der_type_encoder_f RICindicationType_encode_der;
xer_type_decoder_f RICindicationType_decode_xer;
xer_type_encoder_f RICindicationType_encode_xer;
oer_type_decoder_f RICindicationType_decode_oer;
oer_type_encoder_f RICindicationType_encode_oer;
per_type_decoder_f RICindicationType_decode_uper;
per_type_encoder_f RICindicationType_encode_uper;
per_type_decoder_f RICindicationType_decode_aper;
per_type_encoder_f RICindicationType_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _RICindicationType_H_ */
#include <asn_internal.h>
