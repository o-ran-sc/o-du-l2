/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_Phy_ParametersCommon_H_
#define	_Phy_ParametersCommon_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Phy_ParametersCommon__csi_RS_CFRA_ForHO {
	Phy_ParametersCommon__csi_RS_CFRA_ForHO_supported	= 0
} e_Phy_ParametersCommon__csi_RS_CFRA_ForHO;
typedef enum Phy_ParametersCommon__dynamicPRB_BundlingDL {
	Phy_ParametersCommon__dynamicPRB_BundlingDL_supported	= 0
} e_Phy_ParametersCommon__dynamicPRB_BundlingDL;
typedef enum Phy_ParametersCommon__sp_CSI_ReportPUCCH {
	Phy_ParametersCommon__sp_CSI_ReportPUCCH_supported	= 0
} e_Phy_ParametersCommon__sp_CSI_ReportPUCCH;
typedef enum Phy_ParametersCommon__sp_CSI_ReportPUSCH {
	Phy_ParametersCommon__sp_CSI_ReportPUSCH_supported	= 0
} e_Phy_ParametersCommon__sp_CSI_ReportPUSCH;
typedef enum Phy_ParametersCommon__nzp_CSI_RS_IntefMgmt {
	Phy_ParametersCommon__nzp_CSI_RS_IntefMgmt_supported	= 0
} e_Phy_ParametersCommon__nzp_CSI_RS_IntefMgmt;
typedef enum Phy_ParametersCommon__type2_SP_CSI_Feedback_LongPUCCH {
	Phy_ParametersCommon__type2_SP_CSI_Feedback_LongPUCCH_supported	= 0
} e_Phy_ParametersCommon__type2_SP_CSI_Feedback_LongPUCCH;
typedef enum Phy_ParametersCommon__precoderGranularityCORESET {
	Phy_ParametersCommon__precoderGranularityCORESET_supported	= 0
} e_Phy_ParametersCommon__precoderGranularityCORESET;
typedef enum Phy_ParametersCommon__dynamicHARQ_ACK_Codebook {
	Phy_ParametersCommon__dynamicHARQ_ACK_Codebook_supported	= 0
} e_Phy_ParametersCommon__dynamicHARQ_ACK_Codebook;
typedef enum Phy_ParametersCommon__semiStaticHARQ_ACK_Codebook {
	Phy_ParametersCommon__semiStaticHARQ_ACK_Codebook_supported	= 0
} e_Phy_ParametersCommon__semiStaticHARQ_ACK_Codebook;
typedef enum Phy_ParametersCommon__spatialBundlingHARQ_ACK {
	Phy_ParametersCommon__spatialBundlingHARQ_ACK_supported	= 0
} e_Phy_ParametersCommon__spatialBundlingHARQ_ACK;
typedef enum Phy_ParametersCommon__dynamicBetaOffsetInd_HARQ_ACK_CSI {
	Phy_ParametersCommon__dynamicBetaOffsetInd_HARQ_ACK_CSI_supported	= 0
} e_Phy_ParametersCommon__dynamicBetaOffsetInd_HARQ_ACK_CSI;
typedef enum Phy_ParametersCommon__pucch_Repetition_F1_3_4 {
	Phy_ParametersCommon__pucch_Repetition_F1_3_4_supported	= 0
} e_Phy_ParametersCommon__pucch_Repetition_F1_3_4;
typedef enum Phy_ParametersCommon__ra_Type0_PUSCH {
	Phy_ParametersCommon__ra_Type0_PUSCH_supported	= 0
} e_Phy_ParametersCommon__ra_Type0_PUSCH;
typedef enum Phy_ParametersCommon__dynamicSwitchRA_Type0_1_PDSCH {
	Phy_ParametersCommon__dynamicSwitchRA_Type0_1_PDSCH_supported	= 0
} e_Phy_ParametersCommon__dynamicSwitchRA_Type0_1_PDSCH;
typedef enum Phy_ParametersCommon__dynamicSwitchRA_Type0_1_PUSCH {
	Phy_ParametersCommon__dynamicSwitchRA_Type0_1_PUSCH_supported	= 0
} e_Phy_ParametersCommon__dynamicSwitchRA_Type0_1_PUSCH;
typedef enum Phy_ParametersCommon__pdsch_MappingTypeA {
	Phy_ParametersCommon__pdsch_MappingTypeA_supported	= 0
} e_Phy_ParametersCommon__pdsch_MappingTypeA;
typedef enum Phy_ParametersCommon__pdsch_MappingTypeB {
	Phy_ParametersCommon__pdsch_MappingTypeB_supported	= 0
} e_Phy_ParametersCommon__pdsch_MappingTypeB;
typedef enum Phy_ParametersCommon__interleavingVRB_ToPRB_PDSCH {
	Phy_ParametersCommon__interleavingVRB_ToPRB_PDSCH_supported	= 0
} e_Phy_ParametersCommon__interleavingVRB_ToPRB_PDSCH;
typedef enum Phy_ParametersCommon__interSlotFreqHopping_PUSCH {
	Phy_ParametersCommon__interSlotFreqHopping_PUSCH_supported	= 0
} e_Phy_ParametersCommon__interSlotFreqHopping_PUSCH;
typedef enum Phy_ParametersCommon__type1_PUSCH_RepetitionMultiSlots {
	Phy_ParametersCommon__type1_PUSCH_RepetitionMultiSlots_supported	= 0
} e_Phy_ParametersCommon__type1_PUSCH_RepetitionMultiSlots;
typedef enum Phy_ParametersCommon__type2_PUSCH_RepetitionMultiSlots {
	Phy_ParametersCommon__type2_PUSCH_RepetitionMultiSlots_supported	= 0
} e_Phy_ParametersCommon__type2_PUSCH_RepetitionMultiSlots;
typedef enum Phy_ParametersCommon__pusch_RepetitionMultiSlots {
	Phy_ParametersCommon__pusch_RepetitionMultiSlots_supported	= 0
} e_Phy_ParametersCommon__pusch_RepetitionMultiSlots;
typedef enum Phy_ParametersCommon__pdsch_RepetitionMultiSlots {
	Phy_ParametersCommon__pdsch_RepetitionMultiSlots_supported	= 0
} e_Phy_ParametersCommon__pdsch_RepetitionMultiSlots;
typedef enum Phy_ParametersCommon__downlinkSPS {
	Phy_ParametersCommon__downlinkSPS_supported	= 0
} e_Phy_ParametersCommon__downlinkSPS;
typedef enum Phy_ParametersCommon__configuredUL_GrantType1 {
	Phy_ParametersCommon__configuredUL_GrantType1_supported	= 0
} e_Phy_ParametersCommon__configuredUL_GrantType1;
typedef enum Phy_ParametersCommon__configuredUL_GrantType2 {
	Phy_ParametersCommon__configuredUL_GrantType2_supported	= 0
} e_Phy_ParametersCommon__configuredUL_GrantType2;
typedef enum Phy_ParametersCommon__pre_EmptIndication_DL {
	Phy_ParametersCommon__pre_EmptIndication_DL_supported	= 0
} e_Phy_ParametersCommon__pre_EmptIndication_DL;
typedef enum Phy_ParametersCommon__cbg_TransIndication_DL {
	Phy_ParametersCommon__cbg_TransIndication_DL_supported	= 0
} e_Phy_ParametersCommon__cbg_TransIndication_DL;
typedef enum Phy_ParametersCommon__cbg_TransIndication_UL {
	Phy_ParametersCommon__cbg_TransIndication_UL_supported	= 0
} e_Phy_ParametersCommon__cbg_TransIndication_UL;
typedef enum Phy_ParametersCommon__cbg_FlushIndication_DL {
	Phy_ParametersCommon__cbg_FlushIndication_DL_supported	= 0
} e_Phy_ParametersCommon__cbg_FlushIndication_DL;
typedef enum Phy_ParametersCommon__dynamicHARQ_ACK_CodeB_CBG_Retx_DL {
	Phy_ParametersCommon__dynamicHARQ_ACK_CodeB_CBG_Retx_DL_supported	= 0
} e_Phy_ParametersCommon__dynamicHARQ_ACK_CodeB_CBG_Retx_DL;
typedef enum Phy_ParametersCommon__rateMatchingResrcSetSemi_Static {
	Phy_ParametersCommon__rateMatchingResrcSetSemi_Static_supported	= 0
} e_Phy_ParametersCommon__rateMatchingResrcSetSemi_Static;
typedef enum Phy_ParametersCommon__rateMatchingResrcSetDynamic {
	Phy_ParametersCommon__rateMatchingResrcSetDynamic_supported	= 0
} e_Phy_ParametersCommon__rateMatchingResrcSetDynamic;
typedef enum Phy_ParametersCommon__bwp_SwitchingDelay {
	Phy_ParametersCommon__bwp_SwitchingDelay_type1	= 0,
	Phy_ParametersCommon__bwp_SwitchingDelay_type2	= 1
} e_Phy_ParametersCommon__bwp_SwitchingDelay;
typedef enum Phy_ParametersCommon__ext1__eutra_RS_SINR_measurement {
	Phy_ParametersCommon__ext1__eutra_RS_SINR_measurement_supported	= 0
} e_Phy_ParametersCommon__ext1__eutra_RS_SINR_measurement;

/* Phy-ParametersCommon */
typedef struct Phy_ParametersCommon {
	long	*csi_RS_CFRA_ForHO;	/* OPTIONAL */
	long	*dynamicPRB_BundlingDL;	/* OPTIONAL */
	long	*sp_CSI_ReportPUCCH;	/* OPTIONAL */
	long	*sp_CSI_ReportPUSCH;	/* OPTIONAL */
	long	*nzp_CSI_RS_IntefMgmt;	/* OPTIONAL */
	long	*type2_SP_CSI_Feedback_LongPUCCH;	/* OPTIONAL */
	long	*precoderGranularityCORESET;	/* OPTIONAL */
	long	*dynamicHARQ_ACK_Codebook;	/* OPTIONAL */
	long	*semiStaticHARQ_ACK_Codebook;	/* OPTIONAL */
	long	*spatialBundlingHARQ_ACK;	/* OPTIONAL */
	long	*dynamicBetaOffsetInd_HARQ_ACK_CSI;	/* OPTIONAL */
	long	*pucch_Repetition_F1_3_4;	/* OPTIONAL */
	long	*ra_Type0_PUSCH;	/* OPTIONAL */
	long	*dynamicSwitchRA_Type0_1_PDSCH;	/* OPTIONAL */
	long	*dynamicSwitchRA_Type0_1_PUSCH;	/* OPTIONAL */
	long	*pdsch_MappingTypeA;	/* OPTIONAL */
	long	*pdsch_MappingTypeB;	/* OPTIONAL */
	long	*interleavingVRB_ToPRB_PDSCH;	/* OPTIONAL */
	long	*interSlotFreqHopping_PUSCH;	/* OPTIONAL */
	long	*type1_PUSCH_RepetitionMultiSlots;	/* OPTIONAL */
	long	*type2_PUSCH_RepetitionMultiSlots;	/* OPTIONAL */
	long	*pusch_RepetitionMultiSlots;	/* OPTIONAL */
	long	*pdsch_RepetitionMultiSlots;	/* OPTIONAL */
	long	*downlinkSPS;	/* OPTIONAL */
	long	*configuredUL_GrantType1;	/* OPTIONAL */
	long	*configuredUL_GrantType2;	/* OPTIONAL */
	long	*pre_EmptIndication_DL;	/* OPTIONAL */
	long	*cbg_TransIndication_DL;	/* OPTIONAL */
	long	*cbg_TransIndication_UL;	/* OPTIONAL */
	long	*cbg_FlushIndication_DL;	/* OPTIONAL */
	long	*dynamicHARQ_ACK_CodeB_CBG_Retx_DL;	/* OPTIONAL */
	long	*rateMatchingResrcSetSemi_Static;	/* OPTIONAL */
	long	*rateMatchingResrcSetDynamic;	/* OPTIONAL */
	long	*bwp_SwitchingDelay;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct Phy_ParametersCommon__ext1 {
		long	*eutra_RS_SINR_measurement;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Phy_ParametersCommon_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_csi_RS_CFRA_ForHO_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicPRB_BundlingDL_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_sp_CSI_ReportPUCCH_6;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_sp_CSI_ReportPUSCH_8;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_nzp_CSI_RS_IntefMgmt_10;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_type2_SP_CSI_Feedback_LongPUCCH_12;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_precoderGranularityCORESET_14;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicHARQ_ACK_Codebook_16;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_semiStaticHARQ_ACK_Codebook_18;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_spatialBundlingHARQ_ACK_20;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicBetaOffsetInd_HARQ_ACK_CSI_22;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pucch_Repetition_F1_3_4_24;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ra_Type0_PUSCH_26;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicSwitchRA_Type0_1_PDSCH_28;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicSwitchRA_Type0_1_PUSCH_30;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdsch_MappingTypeA_32;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdsch_MappingTypeB_34;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_interleavingVRB_ToPRB_PDSCH_36;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_interSlotFreqHopping_PUSCH_38;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_type1_PUSCH_RepetitionMultiSlots_40;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_type2_PUSCH_RepetitionMultiSlots_42;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pusch_RepetitionMultiSlots_44;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdsch_RepetitionMultiSlots_46;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_downlinkSPS_48;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_configuredUL_GrantType1_50;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_configuredUL_GrantType2_52;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pre_EmptIndication_DL_54;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_cbg_TransIndication_DL_56;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_cbg_TransIndication_UL_58;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_cbg_FlushIndication_DL_60;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicHARQ_ACK_CodeB_CBG_Retx_DL_62;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_rateMatchingResrcSetSemi_Static_64;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_rateMatchingResrcSetDynamic_66;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_bwp_SwitchingDelay_68;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_eutra_RS_SINR_measurement_73;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_Phy_ParametersCommon;
extern asn_SEQUENCE_specifics_t asn_SPC_Phy_ParametersCommon_specs_1;
extern asn_TYPE_member_t asn_MBR_Phy_ParametersCommon_1[35];

#ifdef __cplusplus
}
#endif

#endif	/* _Phy_ParametersCommon_H_ */
#include <asn_internal.h>
