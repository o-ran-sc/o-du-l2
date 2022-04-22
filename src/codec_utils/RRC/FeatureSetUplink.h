/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FeatureSetUplink_H_
#define	_FeatureSetUplink_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include "FreqSeparationClass.h"
#include "FeatureSetUplinkPerCC-Id.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum FeatureSetUplink__scalingFactor {
	FeatureSetUplink__scalingFactor_f0p4	= 0,
	FeatureSetUplink__scalingFactor_f0p75	= 1,
	FeatureSetUplink__scalingFactor_f0p8	= 2
} e_FeatureSetUplink__scalingFactor;
typedef enum FeatureSetUplink__crossCarrierScheduling_OtherSCS {
	FeatureSetUplink__crossCarrierScheduling_OtherSCS_supported	= 0
} e_FeatureSetUplink__crossCarrierScheduling_OtherSCS;
typedef enum FeatureSetUplink__searchSpaceSharingCA_UL {
	FeatureSetUplink__searchSpaceSharingCA_UL_supported	= 0
} e_FeatureSetUplink__searchSpaceSharingCA_UL;
typedef enum FeatureSetUplink__twoPUCCH_Group {
	FeatureSetUplink__twoPUCCH_Group_supported	= 0
} e_FeatureSetUplink__twoPUCCH_Group;
typedef enum FeatureSetUplink__dynamicSwitchSUL {
	FeatureSetUplink__dynamicSwitchSUL_supported	= 0
} e_FeatureSetUplink__dynamicSwitchSUL;
typedef enum FeatureSetUplink__simultaneousTxSUL_NonSUL {
	FeatureSetUplink__simultaneousTxSUL_NonSUL_supported	= 0
} e_FeatureSetUplink__simultaneousTxSUL_NonSUL;
typedef enum FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_15kHz {
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_15kHz_upto2	= 0,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_15kHz_upto4	= 1,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_15kHz_upto7	= 2
} e_FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_15kHz;
typedef enum FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_30kHz {
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_30kHz_upto2	= 0,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_30kHz_upto4	= 1,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_30kHz_upto7	= 2
} e_FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_30kHz;
typedef enum FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_60kHz {
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_60kHz_upto2	= 0,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_60kHz_upto4	= 1,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_60kHz_upto7	= 2
} e_FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_60kHz;
typedef enum FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_120kHz {
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_120kHz_upto2	= 0,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_120kHz_upto4	= 1,
	FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_120kHz_upto7	= 2
} e_FeatureSetUplink__pusch_DifferentTB_PerSlot__scs_120kHz;

/* Forward declarations */
struct SRS_TxSwitch;
struct SRS_Resources;
struct CSI_ReportFramework;

/* FeatureSetUplink */
typedef struct FeatureSetUplink {
	struct FeatureSetUplink__featureSetListPerUplinkCC {
		A_SEQUENCE_OF(FeatureSetUplinkPerCC_Id_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} featureSetListPerUplinkCC;
	long	*scalingFactor;	/* OPTIONAL */
	long	*crossCarrierScheduling_OtherSCS;	/* OPTIONAL */
	FreqSeparationClass_t	*intraBandFreqSeparationUL;	/* OPTIONAL */
	long	*searchSpaceSharingCA_UL;	/* OPTIONAL */
	struct SRS_TxSwitch	*srs_TxSwitch;	/* OPTIONAL */
	struct SRS_Resources	*supportedSRS_Resources;	/* OPTIONAL */
	long	*twoPUCCH_Group;	/* OPTIONAL */
	long	*dynamicSwitchSUL;	/* OPTIONAL */
	long	*simultaneousTxSUL_NonSUL;	/* OPTIONAL */
	struct FeatureSetUplink__pusch_DifferentTB_PerSlot {
		long	*scs_15kHz;	/* OPTIONAL */
		long	*scs_30kHz;	/* OPTIONAL */
		long	*scs_60kHz;	/* OPTIONAL */
		long	*scs_120kHz;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pusch_DifferentTB_PerSlot;
	struct CSI_ReportFramework	*csi_ReportFramework;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FeatureSetUplink_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_scalingFactor_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_crossCarrierScheduling_OtherSCS_8;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_searchSpaceSharingCA_UL_11;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_twoPUCCH_Group_15;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicSwitchSUL_17;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_simultaneousTxSUL_NonSUL_19;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_scs_15kHz_22;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_scs_30kHz_26;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_scs_60kHz_30;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_scs_120kHz_34;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_FeatureSetUplink;
extern asn_SEQUENCE_specifics_t asn_SPC_FeatureSetUplink_specs_1;
extern asn_TYPE_member_t asn_MBR_FeatureSetUplink_1[12];

#ifdef __cplusplus
}
#endif

#endif	/* _FeatureSetUplink_H_ */
#include <asn_internal.h>
