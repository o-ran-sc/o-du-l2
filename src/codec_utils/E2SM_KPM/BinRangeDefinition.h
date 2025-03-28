/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_KPM_v2.03.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_BinRangeDefinition_H_
#define	_BinRangeDefinition_H_


#include <asn_application.h>

/* Including external dependencies */
#include "BinRangeList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct BinRangeList;

/* BinRangeDefinition */
typedef struct BinRangeDefinition {
	BinRangeList_t	 binRangeListX;
	struct BinRangeList	*binRangeListY;	/* OPTIONAL */
	struct BinRangeList	*binRangeListZ;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BinRangeDefinition_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BinRangeDefinition;
extern asn_SEQUENCE_specifics_t asn_SPC_BinRangeDefinition_specs_1;
extern asn_TYPE_member_t asn_MBR_BinRangeDefinition_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _BinRangeDefinition_H_ */
#include <asn_internal.h>
