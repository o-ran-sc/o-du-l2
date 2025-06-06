/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#ifndef	_Notification_Cause_H_
#define	_Notification_Cause_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Notification_Cause {
	Notification_Cause_fulfilled	= 0,
	Notification_Cause_not_fulfilled	= 1
	/*
	 * Enumeration is extensible
	 */
} e_Notification_Cause;

/* Notification-Cause */
typedef long	 Notification_Cause_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_Notification_Cause_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_Notification_Cause;
extern const asn_INTEGER_specifics_t asn_SPC_Notification_Cause_specs_1;
asn_struct_free_f Notification_Cause_free;
asn_struct_print_f Notification_Cause_print;
asn_constr_check_f Notification_Cause_constraint;
xer_type_decoder_f Notification_Cause_decode_xer;
xer_type_encoder_f Notification_Cause_encode_xer;
per_type_decoder_f Notification_Cause_decode_uper;
per_type_encoder_f Notification_Cause_encode_uper;
per_type_decoder_f Notification_Cause_decode_aper;
per_type_encoder_f Notification_Cause_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _Notification_Cause_H_ */
#include <asn_internal.h>
