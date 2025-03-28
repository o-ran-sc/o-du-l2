/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_SIB7_H_
#define	_SIB7_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SIB7__warningMessageSegmentType {
	SIB7__warningMessageSegmentType_notLastSegment	= 0,
	SIB7__warningMessageSegmentType_lastSegment	= 1
} e_SIB7__warningMessageSegmentType;

/* SIB7 */
typedef struct SIB7 {
	BIT_STRING_t	 messageIdentifier;
	BIT_STRING_t	 serialNumber;
	long	 warningMessageSegmentType;
	long	 warningMessageSegmentNumber;
	OCTET_STRING_t	 warningMessageSegment;
	OCTET_STRING_t	*dataCodingScheme;	/* OPTIONAL */
	OCTET_STRING_t	*lateNonCriticalExtension;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SIB7_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_warningMessageSegmentType_4;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SIB7;
extern asn_SEQUENCE_specifics_t asn_SPC_SIB7_specs_1;
extern asn_TYPE_member_t asn_MBR_SIB7_1[7];

#ifdef __cplusplus
}
#endif

#endif	/* _SIB7_H_ */
#include <asn_internal.h>
