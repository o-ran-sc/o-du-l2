/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PollByte_H_
#define	_PollByte_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PollByte {
	PollByte_kB1	= 0,
	PollByte_kB2	= 1,
	PollByte_kB5	= 2,
	PollByte_kB8	= 3,
	PollByte_kB10	= 4,
	PollByte_kB15	= 5,
	PollByte_kB25	= 6,
	PollByte_kB50	= 7,
	PollByte_kB75	= 8,
	PollByte_kB100	= 9,
	PollByte_kB125	= 10,
	PollByte_kB250	= 11,
	PollByte_kB375	= 12,
	PollByte_kB500	= 13,
	PollByte_kB750	= 14,
	PollByte_kB1000	= 15,
	PollByte_kB1250	= 16,
	PollByte_kB1500	= 17,
	PollByte_kB2000	= 18,
	PollByte_kB3000	= 19,
	PollByte_kB4000	= 20,
	PollByte_kB4500	= 21,
	PollByte_kB5000	= 22,
	PollByte_kB5500	= 23,
	PollByte_kB6000	= 24,
	PollByte_kB6500	= 25,
	PollByte_kB7000	= 26,
	PollByte_kB7500	= 27,
	PollByte_mB8	= 28,
	PollByte_mB9	= 29,
	PollByte_mB10	= 30,
	PollByte_mB11	= 31,
	PollByte_mB12	= 32,
	PollByte_mB13	= 33,
	PollByte_mB14	= 34,
	PollByte_mB15	= 35,
	PollByte_mB16	= 36,
	PollByte_mB17	= 37,
	PollByte_mB18	= 38,
	PollByte_mB20	= 39,
	PollByte_mB25	= 40,
	PollByte_mB30	= 41,
	PollByte_mB40	= 42,
	PollByte_infinity	= 43,
	PollByte_spare20	= 44,
	PollByte_spare19	= 45,
	PollByte_spare18	= 46,
	PollByte_spare17	= 47,
	PollByte_spare16	= 48,
	PollByte_spare15	= 49,
	PollByte_spare14	= 50,
	PollByte_spare13	= 51,
	PollByte_spare12	= 52,
	PollByte_spare11	= 53,
	PollByte_spare10	= 54,
	PollByte_spare9	= 55,
	PollByte_spare8	= 56,
	PollByte_spare7	= 57,
	PollByte_spare6	= 58,
	PollByte_spare5	= 59,
	PollByte_spare4	= 60,
	PollByte_spare3	= 61,
	PollByte_spare2	= 62,
	PollByte_spare1	= 63
} e_PollByte;

/* PollByte */
typedef long	 PollByte_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_PollByte_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_PollByte;
extern const asn_INTEGER_specifics_t asn_SPC_PollByte_specs_1;
asn_struct_free_f PollByte_free;
asn_struct_print_f PollByte_print;
asn_constr_check_f PollByte_constraint;
ber_type_decoder_f PollByte_decode_ber;
der_type_encoder_f PollByte_encode_der;
xer_type_decoder_f PollByte_decode_xer;
xer_type_encoder_f PollByte_encode_xer;
oer_type_decoder_f PollByte_decode_oer;
oer_type_encoder_f PollByte_encode_oer;
per_type_decoder_f PollByte_decode_uper;
per_type_encoder_f PollByte_encode_uper;
per_type_decoder_f PollByte_decode_aper;
per_type_encoder_f PollByte_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _PollByte_H_ */
#include <asn_internal.h>
