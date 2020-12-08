/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_EstablishmentCause_H_
#define	_EstablishmentCause_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EstablishmentCause {
	EstablishmentCause_emergency	= 0,
	EstablishmentCause_highPriorityAccess	= 1,
	EstablishmentCause_mt_Access	= 2,
	EstablishmentCause_mo_Signalling	= 3,
	EstablishmentCause_mo_Data	= 4,
	EstablishmentCause_mo_VoiceCall	= 5,
	EstablishmentCause_mo_VideoCall	= 6,
	EstablishmentCause_mo_SMS	= 7,
	EstablishmentCause_mps_PriorityAccess	= 8,
	EstablishmentCause_mcs_PriorityAccess	= 9,
	EstablishmentCause_spare6	= 10,
	EstablishmentCause_spare5	= 11,
	EstablishmentCause_spare4	= 12,
	EstablishmentCause_spare3	= 13,
	EstablishmentCause_spare2	= 14,
	EstablishmentCause_spare1	= 15
} e_EstablishmentCause;

/* EstablishmentCause */
typedef long	 EstablishmentCause_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_EstablishmentCause_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_EstablishmentCause;
extern const asn_INTEGER_specifics_t asn_SPC_EstablishmentCause_specs_1;
asn_struct_free_f EstablishmentCause_free;
asn_struct_print_f EstablishmentCause_print;
asn_constr_check_f EstablishmentCause_constraint;
ber_type_decoder_f EstablishmentCause_decode_ber;
der_type_encoder_f EstablishmentCause_encode_der;
xer_type_decoder_f EstablishmentCause_decode_xer;
xer_type_encoder_f EstablishmentCause_encode_xer;
oer_type_decoder_f EstablishmentCause_decode_oer;
oer_type_encoder_f EstablishmentCause_encode_oer;
per_type_decoder_f EstablishmentCause_decode_uper;
per_type_encoder_f EstablishmentCause_encode_uper;
per_type_decoder_f EstablishmentCause_decode_aper;
per_type_encoder_f EstablishmentCause_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _EstablishmentCause_H_ */
#include <asn_internal.h>
