/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "codeathon/E2SM_KPM.asn1"
 * 	`asn1c -D ./codeathon/e2smkpm/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RAN_Container_H_
#define	_RAN_Container_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RAN-Container */
typedef OCTET_STRING_t	 RAN_Container_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RAN_Container;
asn_struct_free_f RAN_Container_free;
asn_struct_print_f RAN_Container_print;
asn_constr_check_f RAN_Container_constraint;
ber_type_decoder_f RAN_Container_decode_ber;
der_type_encoder_f RAN_Container_encode_der;
xer_type_decoder_f RAN_Container_decode_xer;
xer_type_encoder_f RAN_Container_encode_xer;
oer_type_decoder_f RAN_Container_decode_oer;
oer_type_encoder_f RAN_Container_encode_oer;
per_type_decoder_f RAN_Container_decode_uper;
per_type_encoder_f RAN_Container_encode_uper;
per_type_decoder_f RAN_Container_decode_aper;
per_type_encoder_f RAN_Container_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _RAN_Container_H_ */
#include <asn_internal.h>
