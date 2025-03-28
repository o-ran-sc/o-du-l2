/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_SS_RSSI_Measurement_H_
#define	_SS_RSSI_Measurement_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SS-RSSI-Measurement */
typedef struct SS_RSSI_Measurement {
	BIT_STRING_t	 measurementSlots;
	long	 endSymbol;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SS_RSSI_Measurement_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SS_RSSI_Measurement;
extern asn_SEQUENCE_specifics_t asn_SPC_SS_RSSI_Measurement_specs_1;
extern asn_TYPE_member_t asn_MBR_SS_RSSI_Measurement_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _SS_RSSI_Measurement_H_ */
#include <asn_internal.h>
