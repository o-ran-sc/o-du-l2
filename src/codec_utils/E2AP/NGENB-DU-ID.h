/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_NGENB_DU_ID_H_
#define	_NGENB_DU_ID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <INTEGER.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NGENB-DU-ID */
typedef INTEGER_t	 NGENB_DU_ID_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_NGENB_DU_ID_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_NGENB_DU_ID;
asn_struct_free_f NGENB_DU_ID_free;
asn_struct_print_f NGENB_DU_ID_print;
asn_constr_check_f NGENB_DU_ID_constraint;
ber_type_decoder_f NGENB_DU_ID_decode_ber;
der_type_encoder_f NGENB_DU_ID_encode_der;
xer_type_decoder_f NGENB_DU_ID_decode_xer;
xer_type_encoder_f NGENB_DU_ID_encode_xer;
oer_type_decoder_f NGENB_DU_ID_decode_oer;
oer_type_encoder_f NGENB_DU_ID_encode_oer;
per_type_decoder_f NGENB_DU_ID_decode_uper;
per_type_encoder_f NGENB_DU_ID_encode_uper;
per_type_decoder_f NGENB_DU_ID_decode_aper;
per_type_encoder_f NGENB_DU_ID_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _NGENB_DU_ID_H_ */
#include <asn_internal.h>
