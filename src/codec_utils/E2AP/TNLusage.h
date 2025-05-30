/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#ifndef	_TNLusage_H_
#define	_TNLusage_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TNLusage {
	TNLusage_ric_service	= 0,
	TNLusage_support_function	= 1,
	TNLusage_both	= 2
	/*
	 * Enumeration is extensible
	 */
} e_TNLusage;

/* TNLusage */
typedef long	 TNLusage_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_TNLusage_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_TNLusage;
extern const asn_INTEGER_specifics_t asn_SPC_TNLusage_specs_1;
asn_struct_free_f TNLusage_free;
asn_struct_print_f TNLusage_print;
asn_constr_check_f TNLusage_constraint;
xer_type_decoder_f TNLusage_decode_xer;
xer_type_encoder_f TNLusage_encode_xer;
per_type_decoder_f TNLusage_decode_uper;
per_type_encoder_f TNLusage_encode_uper;
per_type_decoder_f TNLusage_decode_aper;
per_type_encoder_f TNLusage_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _TNLusage_H_ */
#include <asn_internal.h>
