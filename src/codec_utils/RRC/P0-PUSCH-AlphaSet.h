/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_P0_PUSCH_AlphaSet_H_
#define	_P0_PUSCH_AlphaSet_H_


#include <asn_application.h>

/* Including external dependencies */
#include "P0-PUSCH-AlphaSetId.h"
#include <NativeInteger.h>
#include "Alpha.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* P0-PUSCH-AlphaSet */
typedef struct P0_PUSCH_AlphaSet {
	P0_PUSCH_AlphaSetId_t	 p0_PUSCH_AlphaSetId;
	long	*p0;	/* OPTIONAL */
	Alpha_t	*alpha;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} P0_PUSCH_AlphaSet_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_P0_PUSCH_AlphaSet;
extern asn_SEQUENCE_specifics_t asn_SPC_P0_PUSCH_AlphaSet_specs_1;
extern asn_TYPE_member_t asn_MBR_P0_PUSCH_AlphaSet_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _P0_PUSCH_AlphaSet_H_ */
#include <asn_internal.h>
