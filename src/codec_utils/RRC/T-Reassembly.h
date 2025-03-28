/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_T_Reassembly_H_
#define	_T_Reassembly_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum T_Reassembly {
	T_Reassembly_ms0	= 0,
	T_Reassembly_ms5	= 1,
	T_Reassembly_ms10	= 2,
	T_Reassembly_ms15	= 3,
	T_Reassembly_ms20	= 4,
	T_Reassembly_ms25	= 5,
	T_Reassembly_ms30	= 6,
	T_Reassembly_ms35	= 7,
	T_Reassembly_ms40	= 8,
	T_Reassembly_ms45	= 9,
	T_Reassembly_ms50	= 10,
	T_Reassembly_ms55	= 11,
	T_Reassembly_ms60	= 12,
	T_Reassembly_ms65	= 13,
	T_Reassembly_ms70	= 14,
	T_Reassembly_ms75	= 15,
	T_Reassembly_ms80	= 16,
	T_Reassembly_ms85	= 17,
	T_Reassembly_ms90	= 18,
	T_Reassembly_ms95	= 19,
	T_Reassembly_ms100	= 20,
	T_Reassembly_ms110	= 21,
	T_Reassembly_ms120	= 22,
	T_Reassembly_ms130	= 23,
	T_Reassembly_ms140	= 24,
	T_Reassembly_ms150	= 25,
	T_Reassembly_ms160	= 26,
	T_Reassembly_ms170	= 27,
	T_Reassembly_ms180	= 28,
	T_Reassembly_ms190	= 29,
	T_Reassembly_ms200	= 30,
	T_Reassembly_spare1	= 31
} e_T_Reassembly;

/* T-Reassembly */
typedef long	 T_Reassembly_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_T_Reassembly_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_T_Reassembly;
extern const asn_INTEGER_specifics_t asn_SPC_T_Reassembly_specs_1;
asn_struct_free_f T_Reassembly_free;
asn_struct_print_f T_Reassembly_print;
asn_constr_check_f T_Reassembly_constraint;
xer_type_decoder_f T_Reassembly_decode_xer;
xer_type_encoder_f T_Reassembly_encode_xer;
per_type_decoder_f T_Reassembly_decode_uper;
per_type_encoder_f T_Reassembly_encode_uper;
per_type_decoder_f T_Reassembly_decode_aper;
per_type_encoder_f T_Reassembly_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _T_Reassembly_H_ */
#include <asn_internal.h>
