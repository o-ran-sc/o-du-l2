/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SN_FieldLengthUM_H_
#define	_SN_FieldLengthUM_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SN_FieldLengthUM {
	SN_FieldLengthUM_size6	= 0,
	SN_FieldLengthUM_size12	= 1
} e_SN_FieldLengthUM;

/* SN-FieldLengthUM */
typedef long	 SN_FieldLengthUM_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_SN_FieldLengthUM_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_SN_FieldLengthUM;
extern const asn_INTEGER_specifics_t asn_SPC_SN_FieldLengthUM_specs_1;
asn_struct_free_f SN_FieldLengthUM_free;
asn_struct_print_f SN_FieldLengthUM_print;
asn_constr_check_f SN_FieldLengthUM_constraint;
ber_type_decoder_f SN_FieldLengthUM_decode_ber;
der_type_encoder_f SN_FieldLengthUM_encode_der;
xer_type_decoder_f SN_FieldLengthUM_decode_xer;
xer_type_encoder_f SN_FieldLengthUM_encode_xer;
oer_type_decoder_f SN_FieldLengthUM_decode_oer;
oer_type_encoder_f SN_FieldLengthUM_encode_oer;
per_type_decoder_f SN_FieldLengthUM_decode_uper;
per_type_encoder_f SN_FieldLengthUM_encode_uper;
per_type_decoder_f SN_FieldLengthUM_decode_aper;
per_type_encoder_f SN_FieldLengthUM_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _SN_FieldLengthUM_H_ */
#include <asn_internal.h>
