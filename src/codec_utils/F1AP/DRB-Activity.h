/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_DRB_Activity_H_
#define	_DRB_Activity_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DRB_Activity {
	DRB_Activity_active	= 0,
	DRB_Activity_not_active	= 1
} e_DRB_Activity;

/* DRB-Activity */
typedef long	 DRB_Activity_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_DRB_Activity_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_DRB_Activity;
extern const asn_INTEGER_specifics_t asn_SPC_DRB_Activity_specs_1;
asn_struct_free_f DRB_Activity_free;
asn_struct_print_f DRB_Activity_print;
asn_constr_check_f DRB_Activity_constraint;
ber_type_decoder_f DRB_Activity_decode_ber;
der_type_encoder_f DRB_Activity_encode_der;
xer_type_decoder_f DRB_Activity_decode_xer;
xer_type_encoder_f DRB_Activity_encode_xer;
oer_type_decoder_f DRB_Activity_decode_oer;
oer_type_encoder_f DRB_Activity_encode_oer;
per_type_decoder_f DRB_Activity_decode_uper;
per_type_encoder_f DRB_Activity_encode_uper;
per_type_decoder_f DRB_Activity_decode_aper;
per_type_encoder_f DRB_Activity_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _DRB_Activity_H_ */
#include <asn_internal.h>
