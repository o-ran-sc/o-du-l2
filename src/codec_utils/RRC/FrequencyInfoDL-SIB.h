/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FrequencyInfoDL_SIB_H_
#define	_FrequencyInfoDL_SIB_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MultiFrequencyBandListNR-SIB.h"
#include <NativeInteger.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SCS_SpecificCarrier;

/* FrequencyInfoDL-SIB */
typedef struct FrequencyInfoDL_SIB {
	MultiFrequencyBandListNR_SIB_t	 frequencyBandList;
	long	 offsetToPointA;
	struct FrequencyInfoDL_SIB__scs_SpecificCarrierList {
		A_SEQUENCE_OF(struct SCS_SpecificCarrier) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} scs_SpecificCarrierList;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FrequencyInfoDL_SIB_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FrequencyInfoDL_SIB;
extern asn_SEQUENCE_specifics_t asn_SPC_FrequencyInfoDL_SIB_specs_1;
extern asn_TYPE_member_t asn_MBR_FrequencyInfoDL_SIB_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _FrequencyInfoDL_SIB_H_ */
#include <asn_internal.h>
