/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUCCH_MaxCodeRate_H_
#define	_PUCCH_MaxCodeRate_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PUCCH_MaxCodeRate {
	PUCCH_MaxCodeRate_zeroDot08	= 0,
	PUCCH_MaxCodeRate_zeroDot15	= 1,
	PUCCH_MaxCodeRate_zeroDot25	= 2,
	PUCCH_MaxCodeRate_zeroDot35	= 3,
	PUCCH_MaxCodeRate_zeroDot45	= 4,
	PUCCH_MaxCodeRate_zeroDot60	= 5,
	PUCCH_MaxCodeRate_zeroDot80	= 6
} e_PUCCH_MaxCodeRate;

/* PUCCH-MaxCodeRate */
typedef long	 PUCCH_MaxCodeRate_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_PUCCH_MaxCodeRate_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_MaxCodeRate;
extern const asn_INTEGER_specifics_t asn_SPC_PUCCH_MaxCodeRate_specs_1;
asn_struct_free_f PUCCH_MaxCodeRate_free;
asn_struct_print_f PUCCH_MaxCodeRate_print;
asn_constr_check_f PUCCH_MaxCodeRate_constraint;
ber_type_decoder_f PUCCH_MaxCodeRate_decode_ber;
der_type_encoder_f PUCCH_MaxCodeRate_encode_der;
xer_type_decoder_f PUCCH_MaxCodeRate_decode_xer;
xer_type_encoder_f PUCCH_MaxCodeRate_encode_xer;
oer_type_decoder_f PUCCH_MaxCodeRate_decode_oer;
oer_type_encoder_f PUCCH_MaxCodeRate_encode_oer;
per_type_decoder_f PUCCH_MaxCodeRate_decode_uper;
per_type_encoder_f PUCCH_MaxCodeRate_encode_uper;
per_type_decoder_f PUCCH_MaxCodeRate_decode_aper;
per_type_encoder_f PUCCH_MaxCodeRate_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_MaxCodeRate_H_ */
#include <asn_internal.h>
