/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../mib_sib1.asn1"
 * 	`asn1c -D ./new_out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PagingCycle_H_
#define	_PagingCycle_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PagingCycle {
	PagingCycle_rf32	= 0,
	PagingCycle_rf64	= 1,
	PagingCycle_rf128	= 2,
	PagingCycle_rf256	= 3
} e_PagingCycle;

/* PagingCycle */
typedef long	 PagingCycle_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_PagingCycle_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_PagingCycle;
extern const asn_INTEGER_specifics_t asn_SPC_PagingCycle_specs_1;
asn_struct_free_f PagingCycle_free;
asn_struct_print_f PagingCycle_print;
asn_constr_check_f PagingCycle_constraint;
ber_type_decoder_f PagingCycle_decode_ber;
der_type_encoder_f PagingCycle_encode_der;
xer_type_decoder_f PagingCycle_decode_xer;
xer_type_encoder_f PagingCycle_encode_xer;
oer_type_decoder_f PagingCycle_decode_oer;
oer_type_encoder_f PagingCycle_encode_oer;
per_type_decoder_f PagingCycle_decode_uper;
per_type_encoder_f PagingCycle_encode_uper;
per_type_decoder_f PagingCycle_decode_aper;
per_type_encoder_f PagingCycle_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _PagingCycle_H_ */
#include <asn_internal.h>
