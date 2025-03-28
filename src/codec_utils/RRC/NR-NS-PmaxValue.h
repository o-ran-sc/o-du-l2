/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_NR_NS_PmaxValue_H_
#define	_NR_NS_PmaxValue_H_


#include <asn_application.h>

/* Including external dependencies */
#include "P-Max.h"
#include "AdditionalSpectrumEmission.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NR-NS-PmaxValue */
typedef struct NR_NS_PmaxValue {
	P_Max_t	*additionalPmax;	/* OPTIONAL */
	AdditionalSpectrumEmission_t	 additionalSpectrumEmission;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NR_NS_PmaxValue_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NR_NS_PmaxValue;
extern asn_SEQUENCE_specifics_t asn_SPC_NR_NS_PmaxValue_specs_1;
extern asn_TYPE_member_t asn_MBR_NR_NS_PmaxValue_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _NR_NS_PmaxValue_H_ */
#include <asn_internal.h>
