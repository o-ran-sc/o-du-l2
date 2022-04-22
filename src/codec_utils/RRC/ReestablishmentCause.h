/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ReestablishmentCause_H_
#define	_ReestablishmentCause_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ReestablishmentCause {
	ReestablishmentCause_reconfigurationFailure	= 0,
	ReestablishmentCause_handoverFailure	= 1,
	ReestablishmentCause_otherFailure	= 2,
	ReestablishmentCause_spare1	= 3
} e_ReestablishmentCause;

/* ReestablishmentCause */
typedef long	 ReestablishmentCause_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ReestablishmentCause_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ReestablishmentCause;
extern const asn_INTEGER_specifics_t asn_SPC_ReestablishmentCause_specs_1;
asn_struct_free_f ReestablishmentCause_free;
asn_struct_print_f ReestablishmentCause_print;
asn_constr_check_f ReestablishmentCause_constraint;
ber_type_decoder_f ReestablishmentCause_decode_ber;
der_type_encoder_f ReestablishmentCause_encode_der;
xer_type_decoder_f ReestablishmentCause_decode_xer;
xer_type_encoder_f ReestablishmentCause_encode_xer;
oer_type_decoder_f ReestablishmentCause_decode_oer;
oer_type_encoder_f ReestablishmentCause_encode_oer;
per_type_decoder_f ReestablishmentCause_decode_uper;
per_type_encoder_f ReestablishmentCause_encode_uper;
per_type_decoder_f ReestablishmentCause_decode_aper;
per_type_encoder_f ReestablishmentCause_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _ReestablishmentCause_H_ */
#include <asn_internal.h>
