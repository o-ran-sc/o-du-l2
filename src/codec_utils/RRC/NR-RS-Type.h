/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_NR_RS_Type_H_
#define	_NR_RS_Type_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NR_RS_Type {
	NR_RS_Type_ssb	= 0,
	NR_RS_Type_csi_rs	= 1
} e_NR_RS_Type;

/* NR-RS-Type */
typedef long	 NR_RS_Type_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_NR_RS_Type_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_NR_RS_Type;
extern const asn_INTEGER_specifics_t asn_SPC_NR_RS_Type_specs_1;
asn_struct_free_f NR_RS_Type_free;
asn_struct_print_f NR_RS_Type_print;
asn_constr_check_f NR_RS_Type_constraint;
ber_type_decoder_f NR_RS_Type_decode_ber;
der_type_encoder_f NR_RS_Type_encode_der;
xer_type_decoder_f NR_RS_Type_decode_xer;
xer_type_encoder_f NR_RS_Type_encode_xer;
oer_type_decoder_f NR_RS_Type_decode_oer;
oer_type_encoder_f NR_RS_Type_encode_oer;
per_type_decoder_f NR_RS_Type_decode_uper;
per_type_encoder_f NR_RS_Type_encode_uper;
per_type_decoder_f NR_RS_Type_decode_aper;
per_type_encoder_f NR_RS_Type_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _NR_RS_Type_H_ */
#include <asn_internal.h>
