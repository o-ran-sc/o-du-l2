/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_PagingOrigin_H_
#define	_PagingOrigin_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PagingOrigin {
	PagingOrigin_non_3gpp	= 0
	/*
	 * Enumeration is extensible
	 */
} e_PagingOrigin;

/* PagingOrigin */
typedef long	 PagingOrigin_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PagingOrigin;
asn_struct_free_f PagingOrigin_free;
asn_struct_print_f PagingOrigin_print;
asn_constr_check_f PagingOrigin_constraint;
ber_type_decoder_f PagingOrigin_decode_ber;
der_type_encoder_f PagingOrigin_encode_der;
xer_type_decoder_f PagingOrigin_decode_xer;
xer_type_encoder_f PagingOrigin_encode_xer;
oer_type_decoder_f PagingOrigin_decode_oer;
oer_type_encoder_f PagingOrigin_encode_oer;
per_type_decoder_f PagingOrigin_decode_uper;
per_type_encoder_f PagingOrigin_encode_uper;
per_type_decoder_f PagingOrigin_decode_aper;
per_type_encoder_f PagingOrigin_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _PagingOrigin_H_ */
#include <asn_internal.h>
