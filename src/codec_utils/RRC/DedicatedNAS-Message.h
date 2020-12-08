/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_DedicatedNAS_Message_H_
#define	_DedicatedNAS_Message_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DedicatedNAS-Message */
typedef OCTET_STRING_t	 DedicatedNAS_Message_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DedicatedNAS_Message;
asn_struct_free_f DedicatedNAS_Message_free;
asn_struct_print_f DedicatedNAS_Message_print;
asn_constr_check_f DedicatedNAS_Message_constraint;
ber_type_decoder_f DedicatedNAS_Message_decode_ber;
der_type_encoder_f DedicatedNAS_Message_encode_der;
xer_type_decoder_f DedicatedNAS_Message_decode_xer;
xer_type_encoder_f DedicatedNAS_Message_encode_xer;
oer_type_decoder_f DedicatedNAS_Message_decode_oer;
oer_type_encoder_f DedicatedNAS_Message_encode_oer;
per_type_decoder_f DedicatedNAS_Message_decode_uper;
per_type_encoder_f DedicatedNAS_Message_encode_uper;
per_type_decoder_f DedicatedNAS_Message_decode_aper;
per_type_encoder_f DedicatedNAS_Message_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _DedicatedNAS_Message_H_ */
#include <asn_internal.h>
