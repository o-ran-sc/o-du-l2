/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_PriorityLevel_H_
#define	_PriorityLevel_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PriorityLevel {
	PriorityLevel_spare	= 0,
	PriorityLevel_highest	= 1,
	PriorityLevel_lowest	= 14,
	PriorityLevel_no_priority	= 15
} e_PriorityLevel;

/* PriorityLevel */
typedef long	 PriorityLevel_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_PriorityLevel_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_PriorityLevel;
asn_struct_free_f PriorityLevel_free;
asn_struct_print_f PriorityLevel_print;
asn_constr_check_f PriorityLevel_constraint;
xer_type_decoder_f PriorityLevel_decode_xer;
xer_type_encoder_f PriorityLevel_encode_xer;
per_type_decoder_f PriorityLevel_decode_uper;
per_type_encoder_f PriorityLevel_encode_uper;
per_type_decoder_f PriorityLevel_decode_aper;
per_type_encoder_f PriorityLevel_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _PriorityLevel_H_ */
#include <asn_internal.h>
