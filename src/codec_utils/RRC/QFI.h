/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_QFI_H_
#define	_QFI_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* QFI */
typedef long	 QFI_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_QFI_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_QFI;
asn_struct_free_f QFI_free;
asn_struct_print_f QFI_print;
asn_constr_check_f QFI_constraint;
xer_type_decoder_f QFI_decode_xer;
xer_type_encoder_f QFI_encode_xer;
per_type_decoder_f QFI_decode_uper;
per_type_encoder_f QFI_encode_uper;
per_type_decoder_f QFI_decode_aper;
per_type_encoder_f QFI_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _QFI_H_ */
#include <asn_internal.h>
