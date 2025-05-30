/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_RequestedP_MaxFR2_H_
#define	_RequestedP_MaxFR2_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RequestedP-MaxFR2 */
typedef OCTET_STRING_t	 RequestedP_MaxFR2_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RequestedP_MaxFR2;
asn_struct_free_f RequestedP_MaxFR2_free;
asn_struct_print_f RequestedP_MaxFR2_print;
asn_constr_check_f RequestedP_MaxFR2_constraint;
xer_type_decoder_f RequestedP_MaxFR2_decode_xer;
xer_type_encoder_f RequestedP_MaxFR2_encode_xer;
per_type_decoder_f RequestedP_MaxFR2_decode_uper;
per_type_encoder_f RequestedP_MaxFR2_encode_uper;
per_type_decoder_f RequestedP_MaxFR2_decode_aper;
per_type_encoder_f RequestedP_MaxFR2_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _RequestedP_MaxFR2_H_ */
#include <asn_internal.h>
