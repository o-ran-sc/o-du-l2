/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "../../ASN1_Input/E2APV0300.asn1"
 * 	`asn1c -D ../../E2_v3.0_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CauseE2Transport_H_
#define	_CauseE2Transport_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CauseE2Transport {
	CauseE2Transport_unspecified	= 0,
	CauseE2Transport_transport_resource_unavailable	= 1
	/*
	 * Enumeration is extensible
	 */
} e_CauseE2Transport;

/* CauseE2Transport */
typedef long	 CauseE2Transport_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_CauseE2Transport_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_CauseE2Transport;
extern const asn_INTEGER_specifics_t asn_SPC_CauseE2Transport_specs_1;
asn_struct_free_f CauseE2Transport_free;
asn_struct_print_f CauseE2Transport_print;
asn_constr_check_f CauseE2Transport_constraint;
ber_type_decoder_f CauseE2Transport_decode_ber;
der_type_encoder_f CauseE2Transport_encode_der;
xer_type_decoder_f CauseE2Transport_decode_xer;
xer_type_encoder_f CauseE2Transport_encode_xer;
oer_type_decoder_f CauseE2Transport_decode_oer;
oer_type_encoder_f CauseE2Transport_encode_oer;
per_type_decoder_f CauseE2Transport_decode_uper;
per_type_encoder_f CauseE2Transport_encode_uper;
per_type_decoder_f CauseE2Transport_decode_aper;
per_type_encoder_f CauseE2Transport_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _CauseE2Transport_H_ */
#include <asn_internal.h>
