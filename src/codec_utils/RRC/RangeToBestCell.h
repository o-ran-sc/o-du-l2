/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RangeToBestCell_H_
#define	_RangeToBestCell_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Q-OffsetRange.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RangeToBestCell */
typedef Q_OffsetRange_t	 RangeToBestCell_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_RangeToBestCell_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_RangeToBestCell;
asn_struct_free_f RangeToBestCell_free;
asn_struct_print_f RangeToBestCell_print;
asn_constr_check_f RangeToBestCell_constraint;
ber_type_decoder_f RangeToBestCell_decode_ber;
der_type_encoder_f RangeToBestCell_encode_der;
xer_type_decoder_f RangeToBestCell_decode_xer;
xer_type_encoder_f RangeToBestCell_encode_xer;
oer_type_decoder_f RangeToBestCell_decode_oer;
oer_type_encoder_f RangeToBestCell_encode_oer;
per_type_decoder_f RangeToBestCell_decode_uper;
per_type_encoder_f RangeToBestCell_encode_uper;
per_type_decoder_f RangeToBestCell_decode_aper;
per_type_encoder_f RangeToBestCell_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _RangeToBestCell_H_ */
#include <asn_internal.h>
