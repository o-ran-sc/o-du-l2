/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasGapSharingScheme_H_
#define	_MeasGapSharingScheme_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasGapSharingScheme {
	MeasGapSharingScheme_scheme00	= 0,
	MeasGapSharingScheme_scheme01	= 1,
	MeasGapSharingScheme_scheme10	= 2,
	MeasGapSharingScheme_scheme11	= 3
} e_MeasGapSharingScheme;

/* MeasGapSharingScheme */
typedef long	 MeasGapSharingScheme_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_MeasGapSharingScheme_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_MeasGapSharingScheme;
extern const asn_INTEGER_specifics_t asn_SPC_MeasGapSharingScheme_specs_1;
asn_struct_free_f MeasGapSharingScheme_free;
asn_struct_print_f MeasGapSharingScheme_print;
asn_constr_check_f MeasGapSharingScheme_constraint;
ber_type_decoder_f MeasGapSharingScheme_decode_ber;
der_type_encoder_f MeasGapSharingScheme_encode_der;
xer_type_decoder_f MeasGapSharingScheme_decode_xer;
xer_type_encoder_f MeasGapSharingScheme_encode_xer;
oer_type_decoder_f MeasGapSharingScheme_decode_oer;
oer_type_encoder_f MeasGapSharingScheme_encode_oer;
per_type_decoder_f MeasGapSharingScheme_decode_uper;
per_type_encoder_f MeasGapSharingScheme_encode_uper;
per_type_decoder_f MeasGapSharingScheme_decode_aper;
per_type_encoder_f MeasGapSharingScheme_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _MeasGapSharingScheme_H_ */
#include <asn_internal.h>
