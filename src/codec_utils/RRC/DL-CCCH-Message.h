/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_DL_CCCH_Message_H_
#define	_DL_CCCH_Message_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DL-CCCH-MessageType.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DL-CCCH-Message */
typedef struct DL_CCCH_Message {
	DL_CCCH_MessageType_t	 message;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DL_CCCH_Message_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DL_CCCH_Message;

#ifdef __cplusplus
}
#endif

#endif	/* _DL_CCCH_Message_H_ */
#include <asn_internal.h>
