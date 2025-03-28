/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_RICactionDefinition_H_
#define	_RICactionDefinition_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RICactionDefinition */
typedef OCTET_STRING_t	 RICactionDefinition_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RICactionDefinition;
asn_struct_free_f RICactionDefinition_free;
asn_struct_print_f RICactionDefinition_print;
asn_constr_check_f RICactionDefinition_constraint;
xer_type_decoder_f RICactionDefinition_decode_xer;
xer_type_encoder_f RICactionDefinition_encode_xer;
per_type_decoder_f RICactionDefinition_decode_uper;
per_type_encoder_f RICactionDefinition_encode_uper;
per_type_decoder_f RICactionDefinition_decode_aper;
per_type_encoder_f RICactionDefinition_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _RICactionDefinition_H_ */
#include <asn_internal.h>
