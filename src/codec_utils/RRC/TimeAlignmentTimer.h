/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_TimeAlignmentTimer_H_
#define	_TimeAlignmentTimer_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TimeAlignmentTimer {
	TimeAlignmentTimer_ms500	= 0,
	TimeAlignmentTimer_ms750	= 1,
	TimeAlignmentTimer_ms1280	= 2,
	TimeAlignmentTimer_ms1920	= 3,
	TimeAlignmentTimer_ms2560	= 4,
	TimeAlignmentTimer_ms5120	= 5,
	TimeAlignmentTimer_ms10240	= 6,
	TimeAlignmentTimer_infinity	= 7
} e_TimeAlignmentTimer;

/* TimeAlignmentTimer */
typedef long	 TimeAlignmentTimer_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_TimeAlignmentTimer_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_TimeAlignmentTimer;
extern const asn_INTEGER_specifics_t asn_SPC_TimeAlignmentTimer_specs_1;
asn_struct_free_f TimeAlignmentTimer_free;
asn_struct_print_f TimeAlignmentTimer_print;
asn_constr_check_f TimeAlignmentTimer_constraint;
ber_type_decoder_f TimeAlignmentTimer_decode_ber;
der_type_encoder_f TimeAlignmentTimer_encode_der;
xer_type_decoder_f TimeAlignmentTimer_decode_xer;
xer_type_encoder_f TimeAlignmentTimer_encode_xer;
oer_type_decoder_f TimeAlignmentTimer_decode_oer;
oer_type_encoder_f TimeAlignmentTimer_encode_oer;
per_type_decoder_f TimeAlignmentTimer_decode_uper;
per_type_encoder_f TimeAlignmentTimer_encode_uper;
per_type_decoder_f TimeAlignmentTimer_decode_aper;
per_type_encoder_f TimeAlignmentTimer_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _TimeAlignmentTimer_H_ */
#include <asn_internal.h>
