/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_KPM_v2.03.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#ifndef	_MeasurementCondItem_H_
#define	_MeasurementCondItem_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MeasurementType.h"
#include "MatchingCondList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct BinRangeDefinition;

/* MeasurementCondItem */
typedef struct MeasurementCondItem {
	MeasurementType_t	 measType;
	MatchingCondList_t	 matchingCond;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct BinRangeDefinition	*binRangeDef;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasurementCondItem_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasurementCondItem;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasurementCondItem_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasurementCondItem_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasurementCondItem_H_ */
#include <asn_internal.h>
