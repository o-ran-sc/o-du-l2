/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_PDCP_SN_H_
#define	_PDCP_SN_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PDCP-SN */
typedef long	 PDCP_SN_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_PDCP_SN_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_PDCP_SN;
asn_struct_free_f PDCP_SN_free;
asn_struct_print_f PDCP_SN_print;
asn_constr_check_f PDCP_SN_constraint;
xer_type_decoder_f PDCP_SN_decode_xer;
xer_type_encoder_f PDCP_SN_encode_xer;
per_type_decoder_f PDCP_SN_decode_uper;
per_type_encoder_f PDCP_SN_encode_uper;
per_type_decoder_f PDCP_SN_decode_aper;
per_type_encoder_f PDCP_SN_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _PDCP_SN_H_ */
#include <asn_internal.h>
