/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_LogicalChannelIdentity_H_
#define	_LogicalChannelIdentity_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LogicalChannelIdentity */
typedef long	 LogicalChannelIdentity_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_LogicalChannelIdentity_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_LogicalChannelIdentity;
asn_struct_free_f LogicalChannelIdentity_free;
asn_struct_print_f LogicalChannelIdentity_print;
asn_constr_check_f LogicalChannelIdentity_constraint;
ber_type_decoder_f LogicalChannelIdentity_decode_ber;
der_type_encoder_f LogicalChannelIdentity_encode_der;
xer_type_decoder_f LogicalChannelIdentity_decode_xer;
xer_type_encoder_f LogicalChannelIdentity_encode_xer;
oer_type_decoder_f LogicalChannelIdentity_decode_oer;
oer_type_encoder_f LogicalChannelIdentity_encode_oer;
per_type_decoder_f LogicalChannelIdentity_decode_uper;
per_type_encoder_f LogicalChannelIdentity_encode_uper;
per_type_decoder_f LogicalChannelIdentity_decode_aper;
per_type_encoder_f LogicalChannelIdentity_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _LogicalChannelIdentity_H_ */
#include <asn_internal.h>
