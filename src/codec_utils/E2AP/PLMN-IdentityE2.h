/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "codeathon/E2AP.asn1"
 * 	`asn1c -D ./newE2AP -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PLMN_IdentityE2_H_
#define	_PLMN_IdentityE2_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PLMN-IdentityE2 */
typedef OCTET_STRING_t	 PLMN_IdentityE2_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_PLMN_IdentityE2_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_PLMN_IdentityE2;
asn_struct_free_f PLMN_IdentityE2_free;
asn_struct_print_f PLMN_IdentityE2_print;
asn_constr_check_f PLMN_IdentityE2_constraint;
ber_type_decoder_f PLMN_IdentityE2_decode_ber;
der_type_encoder_f PLMN_IdentityE2_encode_der;
xer_type_decoder_f PLMN_IdentityE2_decode_xer;
xer_type_encoder_f PLMN_IdentityE2_encode_xer;
oer_type_decoder_f PLMN_IdentityE2_decode_oer;
oer_type_encoder_f PLMN_IdentityE2_encode_oer;
per_type_decoder_f PLMN_IdentityE2_decode_uper;
per_type_encoder_f PLMN_IdentityE2_encode_uper;
per_type_decoder_f PLMN_IdentityE2_decode_aper;
per_type_encoder_f PLMN_IdentityE2_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _PLMN_IdentityE2_H_ */
#include <asn_internal.h>
