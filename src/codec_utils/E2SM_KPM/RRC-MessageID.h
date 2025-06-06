/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-COMMON-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_SM_v2.01.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_RRC_MessageID_H_
#define	_RRC_MessageID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "RRCclass-LTE.h"
#include "RRCclass-NR.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RRC_MessageID__rrcType_PR {
	RRC_MessageID__rrcType_PR_NOTHING,	/* No components present */
	RRC_MessageID__rrcType_PR_lTE,
	RRC_MessageID__rrcType_PR_nR
	/* Extensions may appear below */
	
} RRC_MessageID__rrcType_PR;

/* RRC-MessageID */
typedef struct RRC_MessageID {
	struct RRC_MessageID__rrcType {
		RRC_MessageID__rrcType_PR present;
		union RRC_MessageID__rrcType_u {
			RRCclass_LTE_t	 lTE;
			RRCclass_NR_t	 nR;
			/*
			 * This type is extensible,
			 * possible extensions are below.
			 */
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} rrcType;
	long	 messageID;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRC_MessageID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RRC_MessageID;

#ifdef __cplusplus
}
#endif

#endif	/* _RRC_MessageID_H_ */
#include <asn_internal.h>
