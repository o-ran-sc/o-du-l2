/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_DataInactivityTimer_H_
#define	_DataInactivityTimer_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DataInactivityTimer {
	DataInactivityTimer_s1	= 0,
	DataInactivityTimer_s2	= 1,
	DataInactivityTimer_s3	= 2,
	DataInactivityTimer_s5	= 3,
	DataInactivityTimer_s7	= 4,
	DataInactivityTimer_s10	= 5,
	DataInactivityTimer_s15	= 6,
	DataInactivityTimer_s20	= 7,
	DataInactivityTimer_s40	= 8,
	DataInactivityTimer_s50	= 9,
	DataInactivityTimer_s60	= 10,
	DataInactivityTimer_s80	= 11,
	DataInactivityTimer_s100	= 12,
	DataInactivityTimer_s120	= 13,
	DataInactivityTimer_s150	= 14,
	DataInactivityTimer_s180	= 15
} e_DataInactivityTimer;

/* DataInactivityTimer */
typedef long	 DataInactivityTimer_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_DataInactivityTimer_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_DataInactivityTimer;
extern const asn_INTEGER_specifics_t asn_SPC_DataInactivityTimer_specs_1;
asn_struct_free_f DataInactivityTimer_free;
asn_struct_print_f DataInactivityTimer_print;
asn_constr_check_f DataInactivityTimer_constraint;
ber_type_decoder_f DataInactivityTimer_decode_ber;
der_type_encoder_f DataInactivityTimer_encode_der;
xer_type_decoder_f DataInactivityTimer_decode_xer;
xer_type_encoder_f DataInactivityTimer_encode_xer;
oer_type_decoder_f DataInactivityTimer_decode_oer;
oer_type_encoder_f DataInactivityTimer_encode_oer;
per_type_decoder_f DataInactivityTimer_decode_uper;
per_type_encoder_f DataInactivityTimer_encode_uper;
per_type_decoder_f DataInactivityTimer_decode_aper;
per_type_encoder_f DataInactivityTimer_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _DataInactivityTimer_H_ */
#include <asn_internal.h>
