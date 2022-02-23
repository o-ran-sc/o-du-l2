/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MIMO_LayersUL_H_
#define	_MIMO_LayersUL_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MIMO_LayersUL {
	MIMO_LayersUL_oneLayer	= 0,
	MIMO_LayersUL_twoLayers	= 1,
	MIMO_LayersUL_fourLayers	= 2
} e_MIMO_LayersUL;

/* MIMO-LayersUL */
typedef long	 MIMO_LayersUL_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_MIMO_LayersUL_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_MIMO_LayersUL;
extern const asn_INTEGER_specifics_t asn_SPC_MIMO_LayersUL_specs_1;
asn_struct_free_f MIMO_LayersUL_free;
asn_struct_print_f MIMO_LayersUL_print;
asn_constr_check_f MIMO_LayersUL_constraint;
ber_type_decoder_f MIMO_LayersUL_decode_ber;
der_type_encoder_f MIMO_LayersUL_encode_der;
xer_type_decoder_f MIMO_LayersUL_decode_xer;
xer_type_encoder_f MIMO_LayersUL_encode_xer;
oer_type_decoder_f MIMO_LayersUL_decode_oer;
oer_type_encoder_f MIMO_LayersUL_encode_oer;
per_type_decoder_f MIMO_LayersUL_decode_uper;
per_type_encoder_f MIMO_LayersUL_encode_uper;
per_type_decoder_f MIMO_LayersUL_decode_aper;
per_type_encoder_f MIMO_LayersUL_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _MIMO_LayersUL_H_ */
#include <asn_internal.h>
