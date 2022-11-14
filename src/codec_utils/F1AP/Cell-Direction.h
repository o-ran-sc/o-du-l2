/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_Cell_Direction_H_
#define	_Cell_Direction_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Cell_Direction {
	Cell_Direction_dl_only	= 0,
	Cell_Direction_ul_only	= 1
} e_Cell_Direction;

/* Cell-Direction */
typedef long	 Cell_Direction_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Cell_Direction;
asn_struct_free_f Cell_Direction_free;
asn_struct_print_f Cell_Direction_print;
asn_constr_check_f Cell_Direction_constraint;
ber_type_decoder_f Cell_Direction_decode_ber;
der_type_encoder_f Cell_Direction_encode_der;
xer_type_decoder_f Cell_Direction_decode_xer;
xer_type_encoder_f Cell_Direction_encode_xer;
oer_type_decoder_f Cell_Direction_decode_oer;
oer_type_encoder_f Cell_Direction_encode_oer;
per_type_decoder_f Cell_Direction_decode_uper;
per_type_encoder_f Cell_Direction_encode_uper;
per_type_decoder_f Cell_Direction_decode_aper;
per_type_encoder_f Cell_Direction_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _Cell_Direction_H_ */
#include <asn_internal.h>
