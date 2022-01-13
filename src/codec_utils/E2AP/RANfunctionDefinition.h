/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "2022_E2AP.asn1"
 * 	`asn1c -D ./E2AP/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RANfunctionDefinition_H_
#define	_RANfunctionDefinition_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RANfunctionDefinition */
typedef OCTET_STRING_t	 RANfunctionDefinition_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RANfunctionDefinition;
asn_struct_free_f RANfunctionDefinition_free;
asn_struct_print_f RANfunctionDefinition_print;
asn_constr_check_f RANfunctionDefinition_constraint;
ber_type_decoder_f RANfunctionDefinition_decode_ber;
der_type_encoder_f RANfunctionDefinition_encode_der;
xer_type_decoder_f RANfunctionDefinition_decode_xer;
xer_type_encoder_f RANfunctionDefinition_encode_xer;
oer_type_decoder_f RANfunctionDefinition_decode_oer;
oer_type_encoder_f RANfunctionDefinition_encode_oer;
per_type_decoder_f RANfunctionDefinition_decode_uper;
per_type_encoder_f RANfunctionDefinition_encode_uper;
per_type_decoder_f RANfunctionDefinition_decode_aper;
per_type_encoder_f RANfunctionDefinition_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _RANfunctionDefinition_H_ */
#include <asn_internal.h>
