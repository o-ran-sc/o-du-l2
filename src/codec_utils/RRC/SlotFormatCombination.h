/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_SlotFormatCombination_H_
#define	_SlotFormatCombination_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SlotFormatCombinationId.h"
#include <NativeInteger.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SlotFormatCombination */
typedef struct SlotFormatCombination {
	SlotFormatCombinationId_t	 slotFormatCombinationId;
	struct SlotFormatCombination__slotFormats {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} slotFormats;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SlotFormatCombination_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SlotFormatCombination;
extern asn_SEQUENCE_specifics_t asn_SPC_SlotFormatCombination_specs_1;
extern asn_TYPE_member_t asn_MBR_SlotFormatCombination_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _SlotFormatCombination_H_ */
#include <asn_internal.h>
