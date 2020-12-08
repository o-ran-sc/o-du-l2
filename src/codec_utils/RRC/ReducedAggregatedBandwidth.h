/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ReducedAggregatedBandwidth_H_
#define	_ReducedAggregatedBandwidth_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ReducedAggregatedBandwidth {
	ReducedAggregatedBandwidth_mhz0	= 0,
	ReducedAggregatedBandwidth_mhz10	= 1,
	ReducedAggregatedBandwidth_mhz20	= 2,
	ReducedAggregatedBandwidth_mhz30	= 3,
	ReducedAggregatedBandwidth_mhz40	= 4,
	ReducedAggregatedBandwidth_mhz50	= 5,
	ReducedAggregatedBandwidth_mhz60	= 6,
	ReducedAggregatedBandwidth_mhz80	= 7,
	ReducedAggregatedBandwidth_mhz100	= 8,
	ReducedAggregatedBandwidth_mhz200	= 9,
	ReducedAggregatedBandwidth_mhz300	= 10,
	ReducedAggregatedBandwidth_mhz400	= 11
} e_ReducedAggregatedBandwidth;

/* ReducedAggregatedBandwidth */
typedef long	 ReducedAggregatedBandwidth_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ReducedAggregatedBandwidth_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ReducedAggregatedBandwidth;
extern const asn_INTEGER_specifics_t asn_SPC_ReducedAggregatedBandwidth_specs_1;
asn_struct_free_f ReducedAggregatedBandwidth_free;
asn_struct_print_f ReducedAggregatedBandwidth_print;
asn_constr_check_f ReducedAggregatedBandwidth_constraint;
ber_type_decoder_f ReducedAggregatedBandwidth_decode_ber;
der_type_encoder_f ReducedAggregatedBandwidth_encode_der;
xer_type_decoder_f ReducedAggregatedBandwidth_decode_xer;
xer_type_encoder_f ReducedAggregatedBandwidth_encode_xer;
oer_type_decoder_f ReducedAggregatedBandwidth_decode_oer;
oer_type_encoder_f ReducedAggregatedBandwidth_encode_oer;
per_type_decoder_f ReducedAggregatedBandwidth_decode_uper;
per_type_encoder_f ReducedAggregatedBandwidth_encode_uper;
per_type_decoder_f ReducedAggregatedBandwidth_decode_aper;
per_type_encoder_f ReducedAggregatedBandwidth_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _ReducedAggregatedBandwidth_H_ */
#include <asn_internal.h>
