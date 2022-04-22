/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_AggregatedBandwidth_H_
#define	_AggregatedBandwidth_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AggregatedBandwidth {
	AggregatedBandwidth_mhz50	= 0,
	AggregatedBandwidth_mhz100	= 1,
	AggregatedBandwidth_mhz150	= 2,
	AggregatedBandwidth_mhz200	= 3,
	AggregatedBandwidth_mhz250	= 4,
	AggregatedBandwidth_mhz300	= 5,
	AggregatedBandwidth_mhz350	= 6,
	AggregatedBandwidth_mhz400	= 7,
	AggregatedBandwidth_mhz450	= 8,
	AggregatedBandwidth_mhz500	= 9,
	AggregatedBandwidth_mhz550	= 10,
	AggregatedBandwidth_mhz600	= 11,
	AggregatedBandwidth_mhz650	= 12,
	AggregatedBandwidth_mhz700	= 13,
	AggregatedBandwidth_mhz750	= 14,
	AggregatedBandwidth_mhz800	= 15
} e_AggregatedBandwidth;

/* AggregatedBandwidth */
typedef long	 AggregatedBandwidth_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_AggregatedBandwidth_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_AggregatedBandwidth;
extern const asn_INTEGER_specifics_t asn_SPC_AggregatedBandwidth_specs_1;
asn_struct_free_f AggregatedBandwidth_free;
asn_struct_print_f AggregatedBandwidth_print;
asn_constr_check_f AggregatedBandwidth_constraint;
ber_type_decoder_f AggregatedBandwidth_decode_ber;
der_type_encoder_f AggregatedBandwidth_encode_der;
xer_type_decoder_f AggregatedBandwidth_decode_xer;
xer_type_encoder_f AggregatedBandwidth_encode_xer;
oer_type_decoder_f AggregatedBandwidth_decode_oer;
oer_type_encoder_f AggregatedBandwidth_encode_oer;
per_type_decoder_f AggregatedBandwidth_decode_uper;
per_type_encoder_f AggregatedBandwidth_encode_uper;
per_type_decoder_f AggregatedBandwidth_decode_aper;
per_type_encoder_f AggregatedBandwidth_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _AggregatedBandwidth_H_ */
#include <asn_internal.h>
