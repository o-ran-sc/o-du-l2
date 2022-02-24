/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FrequencyInfoDL_H_
#define	_FrequencyInfoDL_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ARFCN-ValueNR.h"
#include "MultiFrequencyBandListNR.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SCS_SpecificCarrier;

/* FrequencyInfoDL */
typedef struct FrequencyInfoDL {
	ARFCN_ValueNR_t	*absoluteFrequencySSB;	/* OPTIONAL */
	MultiFrequencyBandListNR_t	 frequencyBandList;
	ARFCN_ValueNR_t	 absoluteFrequencyPointA;
	struct FrequencyInfoDL__scs_SpecificCarrierList {
		A_SEQUENCE_OF(struct SCS_SpecificCarrier) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} scs_SpecificCarrierList;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FrequencyInfoDL_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FrequencyInfoDL;
extern asn_SEQUENCE_specifics_t asn_SPC_FrequencyInfoDL_specs_1;
extern asn_TYPE_member_t asn_MBR_FrequencyInfoDL_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _FrequencyInfoDL_H_ */
#include <asn_internal.h>
