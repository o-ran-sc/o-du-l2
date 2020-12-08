/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_EUTRA_AllowedMeasBandwidth_H_
#define	_EUTRA_AllowedMeasBandwidth_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EUTRA_AllowedMeasBandwidth {
	EUTRA_AllowedMeasBandwidth_mbw6	= 0,
	EUTRA_AllowedMeasBandwidth_mbw15	= 1,
	EUTRA_AllowedMeasBandwidth_mbw25	= 2,
	EUTRA_AllowedMeasBandwidth_mbw50	= 3,
	EUTRA_AllowedMeasBandwidth_mbw75	= 4,
	EUTRA_AllowedMeasBandwidth_mbw100	= 5
} e_EUTRA_AllowedMeasBandwidth;

/* EUTRA-AllowedMeasBandwidth */
typedef long	 EUTRA_AllowedMeasBandwidth_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_EUTRA_AllowedMeasBandwidth_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_EUTRA_AllowedMeasBandwidth;
extern const asn_INTEGER_specifics_t asn_SPC_EUTRA_AllowedMeasBandwidth_specs_1;
asn_struct_free_f EUTRA_AllowedMeasBandwidth_free;
asn_struct_print_f EUTRA_AllowedMeasBandwidth_print;
asn_constr_check_f EUTRA_AllowedMeasBandwidth_constraint;
ber_type_decoder_f EUTRA_AllowedMeasBandwidth_decode_ber;
der_type_encoder_f EUTRA_AllowedMeasBandwidth_encode_der;
xer_type_decoder_f EUTRA_AllowedMeasBandwidth_decode_xer;
xer_type_encoder_f EUTRA_AllowedMeasBandwidth_encode_xer;
oer_type_decoder_f EUTRA_AllowedMeasBandwidth_decode_oer;
oer_type_encoder_f EUTRA_AllowedMeasBandwidth_encode_oer;
per_type_decoder_f EUTRA_AllowedMeasBandwidth_decode_uper;
per_type_encoder_f EUTRA_AllowedMeasBandwidth_encode_uper;
per_type_decoder_f EUTRA_AllowedMeasBandwidth_decode_aper;
per_type_encoder_f EUTRA_AllowedMeasBandwidth_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _EUTRA_AllowedMeasBandwidth_H_ */
#include <asn_internal.h>
