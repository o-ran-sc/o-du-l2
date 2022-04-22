/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BeamFailureRecoveryConfig_H_
#define	_BeamFailureRecoveryConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "RSRP-Range.h"
#include <NativeEnumerated.h>
#include "SearchSpaceId.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include "SubcarrierSpacing.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BeamFailureRecoveryConfig__ssb_perRACH_Occasion {
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_oneEighth	= 0,
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_oneFourth	= 1,
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_oneHalf	= 2,
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_one	= 3,
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_two	= 4,
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_four	= 5,
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_eight	= 6,
	BeamFailureRecoveryConfig__ssb_perRACH_Occasion_sixteen	= 7
} e_BeamFailureRecoveryConfig__ssb_perRACH_Occasion;
typedef enum BeamFailureRecoveryConfig__beamFailureRecoveryTimer {
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms10	= 0,
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms20	= 1,
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms40	= 2,
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms60	= 3,
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms80	= 4,
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms100	= 5,
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms150	= 6,
	BeamFailureRecoveryConfig__beamFailureRecoveryTimer_ms200	= 7
} e_BeamFailureRecoveryConfig__beamFailureRecoveryTimer;

/* Forward declarations */
struct RACH_ConfigGeneric;
struct RA_Prioritization;
struct PRACH_ResourceDedicatedBFR;

/* BeamFailureRecoveryConfig */
typedef struct BeamFailureRecoveryConfig {
	long	*rootSequenceIndex_BFR;	/* OPTIONAL */
	struct RACH_ConfigGeneric	*rach_ConfigBFR;	/* OPTIONAL */
	RSRP_Range_t	*rsrp_ThresholdSSB;	/* OPTIONAL */
	struct BeamFailureRecoveryConfig__candidateBeamRSList {
		A_SEQUENCE_OF(struct PRACH_ResourceDedicatedBFR) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *candidateBeamRSList;
	long	*ssb_perRACH_Occasion;	/* OPTIONAL */
	long	*ra_ssb_OccasionMaskIndex;	/* OPTIONAL */
	SearchSpaceId_t	*recoverySearchSpaceId;	/* OPTIONAL */
	struct RA_Prioritization	*ra_Prioritization;	/* OPTIONAL */
	long	*beamFailureRecoveryTimer;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct BeamFailureRecoveryConfig__ext1 {
		SubcarrierSpacing_t	*msg1_SubcarrierSpacing_v1530;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BeamFailureRecoveryConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ssb_perRACH_Occasion_7;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_beamFailureRecoveryTimer_19;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_BeamFailureRecoveryConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_BeamFailureRecoveryConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_BeamFailureRecoveryConfig_1[10];

#ifdef __cplusplus
}
#endif

#endif	/* _BeamFailureRecoveryConfig_H_ */
#include <asn_internal.h>
