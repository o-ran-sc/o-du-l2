/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_CA_BandwidthClassNR_H_
#define	_CA_BandwidthClassNR_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CA_BandwidthClassNR {
	CA_BandwidthClassNR_a	= 0,
	CA_BandwidthClassNR_b	= 1,
	CA_BandwidthClassNR_c	= 2,
	CA_BandwidthClassNR_d	= 3,
	CA_BandwidthClassNR_e	= 4,
	CA_BandwidthClassNR_f	= 5,
	CA_BandwidthClassNR_g	= 6,
	CA_BandwidthClassNR_h	= 7,
	CA_BandwidthClassNR_i	= 8,
	CA_BandwidthClassNR_j	= 9,
	CA_BandwidthClassNR_k	= 10,
	CA_BandwidthClassNR_l	= 11,
	CA_BandwidthClassNR_m	= 12,
	CA_BandwidthClassNR_n	= 13,
	CA_BandwidthClassNR_o	= 14,
	CA_BandwidthClassNR_p	= 15,
	CA_BandwidthClassNR_q	= 16
	/*
	 * Enumeration is extensible
	 */
} e_CA_BandwidthClassNR;

/* CA-BandwidthClassNR */
typedef long	 CA_BandwidthClassNR_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_CA_BandwidthClassNR_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_CA_BandwidthClassNR;
extern const asn_INTEGER_specifics_t asn_SPC_CA_BandwidthClassNR_specs_1;
asn_struct_free_f CA_BandwidthClassNR_free;
asn_struct_print_f CA_BandwidthClassNR_print;
asn_constr_check_f CA_BandwidthClassNR_constraint;
xer_type_decoder_f CA_BandwidthClassNR_decode_xer;
xer_type_encoder_f CA_BandwidthClassNR_encode_xer;
per_type_decoder_f CA_BandwidthClassNR_decode_uper;
per_type_encoder_f CA_BandwidthClassNR_encode_uper;
per_type_decoder_f CA_BandwidthClassNR_decode_aper;
per_type_encoder_f CA_BandwidthClassNR_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _CA_BandwidthClassNR_H_ */
#include <asn_internal.h>
