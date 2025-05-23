/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_PagingRecord_H_
#define	_PagingRecord_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PagingUE-Identity.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PagingRecord__accessType {
	PagingRecord__accessType_non3GPP	= 0
} e_PagingRecord__accessType;

/* PagingRecord */
typedef struct PagingRecord {
	PagingUE_Identity_t	 ue_Identity;
	long	*accessType;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PagingRecord_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_accessType_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PagingRecord;
extern asn_SEQUENCE_specifics_t asn_SPC_PagingRecord_specs_1;
extern asn_TYPE_member_t asn_MBR_PagingRecord_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _PagingRecord_H_ */
#include <asn_internal.h>
