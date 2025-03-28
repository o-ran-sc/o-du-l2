/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_UplinkTxDirectCurrentListInformation_H_
#define	_UplinkTxDirectCurrentListInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UplinkTxDirectCurrentListInformation */
typedef OCTET_STRING_t	 UplinkTxDirectCurrentListInformation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UplinkTxDirectCurrentListInformation;
asn_struct_free_f UplinkTxDirectCurrentListInformation_free;
asn_struct_print_f UplinkTxDirectCurrentListInformation_print;
asn_constr_check_f UplinkTxDirectCurrentListInformation_constraint;
xer_type_decoder_f UplinkTxDirectCurrentListInformation_decode_xer;
xer_type_encoder_f UplinkTxDirectCurrentListInformation_encode_xer;
per_type_decoder_f UplinkTxDirectCurrentListInformation_decode_uper;
per_type_encoder_f UplinkTxDirectCurrentListInformation_encode_uper;
per_type_decoder_f UplinkTxDirectCurrentListInformation_decode_aper;
per_type_encoder_f UplinkTxDirectCurrentListInformation_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _UplinkTxDirectCurrentListInformation_H_ */
#include <asn_internal.h>
