/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_RedirectedCarrierInfo_H_
#define	_RedirectedCarrierInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RedirectedCarrierInfo_PR {
	RedirectedCarrierInfo_PR_NOTHING,	/* No components present */
	RedirectedCarrierInfo_PR_nr,
	RedirectedCarrierInfo_PR_eutra
	/* Extensions may appear below */
	
} RedirectedCarrierInfo_PR;

/* Forward declarations */
struct CarrierInfoNR;
struct RedirectedCarrierInfo_EUTRA;

/* RedirectedCarrierInfo */
typedef struct RedirectedCarrierInfo {
	RedirectedCarrierInfo_PR present;
	union RedirectedCarrierInfo_u {
		struct CarrierInfoNR	*nr;
		struct RedirectedCarrierInfo_EUTRA	*eutra;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RedirectedCarrierInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RedirectedCarrierInfo;
extern asn_CHOICE_specifics_t asn_SPC_RedirectedCarrierInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_RedirectedCarrierInfo_1[2];
extern asn_per_constraints_t asn_PER_type_RedirectedCarrierInfo_constr_1;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CarrierInfoNR.h"
#include "RedirectedCarrierInfo-EUTRA.h"

#endif	/* _RedirectedCarrierInfo_H_ */
#include <asn_internal.h>
