/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_ResumeCause_H_
#define	_ResumeCause_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ResumeCause {
	ResumeCause_emergency	= 0,
	ResumeCause_highPriorityAccess	= 1,
	ResumeCause_mt_Access	= 2,
	ResumeCause_mo_Signalling	= 3,
	ResumeCause_mo_Data	= 4,
	ResumeCause_mo_VoiceCall	= 5,
	ResumeCause_mo_VideoCall	= 6,
	ResumeCause_mo_SMS	= 7,
	ResumeCause_rna_Update	= 8,
	ResumeCause_mps_PriorityAccess	= 9,
	ResumeCause_mcs_PriorityAccess	= 10,
	ResumeCause_spare1	= 11,
	ResumeCause_spare2	= 12,
	ResumeCause_spare3	= 13,
	ResumeCause_spare4	= 14,
	ResumeCause_spare5	= 15
} e_ResumeCause;

/* ResumeCause */
typedef long	 ResumeCause_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ResumeCause_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ResumeCause;
extern const asn_INTEGER_specifics_t asn_SPC_ResumeCause_specs_1;
asn_struct_free_f ResumeCause_free;
asn_struct_print_f ResumeCause_print;
asn_constr_check_f ResumeCause_constraint;
xer_type_decoder_f ResumeCause_decode_xer;
xer_type_encoder_f ResumeCause_encode_xer;
per_type_decoder_f ResumeCause_decode_uper;
per_type_encoder_f ResumeCause_encode_uper;
per_type_decoder_f ResumeCause_decode_aper;
per_type_encoder_f ResumeCause_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _ResumeCause_H_ */
#include <asn_internal.h>
