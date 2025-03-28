/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-COMMON-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_SM_v2.01.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_QosFlowIdentifier_H_
#define	_QosFlowIdentifier_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* QosFlowIdentifier */
typedef long	 QosFlowIdentifier_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_QosFlowIdentifier_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_QosFlowIdentifier;
asn_struct_free_f QosFlowIdentifier_free;
asn_struct_print_f QosFlowIdentifier_print;
asn_constr_check_f QosFlowIdentifier_constraint;
xer_type_decoder_f QosFlowIdentifier_decode_xer;
xer_type_encoder_f QosFlowIdentifier_encode_xer;
per_type_decoder_f QosFlowIdentifier_decode_uper;
per_type_encoder_f QosFlowIdentifier_encode_uper;
per_type_decoder_f QosFlowIdentifier_decode_aper;
per_type_encoder_f QosFlowIdentifier_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _QosFlowIdentifier_H_ */
#include <asn_internal.h>
