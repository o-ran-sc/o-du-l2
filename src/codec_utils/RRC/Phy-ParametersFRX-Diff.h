/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#ifndef	_Phy_ParametersFRX_Diff_H_
#define	_Phy_ParametersFRX_Diff_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <BIT_STRING.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Phy_ParametersFRX_Diff__dynamicSFI {
	Phy_ParametersFRX_Diff__dynamicSFI_supported	= 0
} e_Phy_ParametersFRX_Diff__dynamicSFI;
typedef enum Phy_ParametersFRX_Diff__supportedDMRS_TypeDL {
	Phy_ParametersFRX_Diff__supportedDMRS_TypeDL_type1	= 0,
	Phy_ParametersFRX_Diff__supportedDMRS_TypeDL_type1And2	= 1
} e_Phy_ParametersFRX_Diff__supportedDMRS_TypeDL;
typedef enum Phy_ParametersFRX_Diff__supportedDMRS_TypeUL {
	Phy_ParametersFRX_Diff__supportedDMRS_TypeUL_type1	= 0,
	Phy_ParametersFRX_Diff__supportedDMRS_TypeUL_type1And2	= 1
} e_Phy_ParametersFRX_Diff__supportedDMRS_TypeUL;
typedef enum Phy_ParametersFRX_Diff__semiOpenLoopCSI {
	Phy_ParametersFRX_Diff__semiOpenLoopCSI_supported	= 0
} e_Phy_ParametersFRX_Diff__semiOpenLoopCSI;
typedef enum Phy_ParametersFRX_Diff__csi_ReportWithoutPMI {
	Phy_ParametersFRX_Diff__csi_ReportWithoutPMI_supported	= 0
} e_Phy_ParametersFRX_Diff__csi_ReportWithoutPMI;
typedef enum Phy_ParametersFRX_Diff__csi_ReportWithoutCQI {
	Phy_ParametersFRX_Diff__csi_ReportWithoutCQI_supported	= 0
} e_Phy_ParametersFRX_Diff__csi_ReportWithoutCQI;
typedef enum Phy_ParametersFRX_Diff__twoPUCCH_F0_2_ConsecSymbols {
	Phy_ParametersFRX_Diff__twoPUCCH_F0_2_ConsecSymbols_supported	= 0
} e_Phy_ParametersFRX_Diff__twoPUCCH_F0_2_ConsecSymbols;
typedef enum Phy_ParametersFRX_Diff__pucch_F2_WithFH {
	Phy_ParametersFRX_Diff__pucch_F2_WithFH_supported	= 0
} e_Phy_ParametersFRX_Diff__pucch_F2_WithFH;
typedef enum Phy_ParametersFRX_Diff__pucch_F3_WithFH {
	Phy_ParametersFRX_Diff__pucch_F3_WithFH_supported	= 0
} e_Phy_ParametersFRX_Diff__pucch_F3_WithFH;
typedef enum Phy_ParametersFRX_Diff__pucch_F4_WithFH {
	Phy_ParametersFRX_Diff__pucch_F4_WithFH_supported	= 0
} e_Phy_ParametersFRX_Diff__pucch_F4_WithFH;
typedef enum Phy_ParametersFRX_Diff__freqHoppingPUCCH_F0_2 {
	Phy_ParametersFRX_Diff__freqHoppingPUCCH_F0_2_notSupported	= 0
} e_Phy_ParametersFRX_Diff__freqHoppingPUCCH_F0_2;
typedef enum Phy_ParametersFRX_Diff__freqHoppingPUCCH_F1_3_4 {
	Phy_ParametersFRX_Diff__freqHoppingPUCCH_F1_3_4_notSupported	= 0
} e_Phy_ParametersFRX_Diff__freqHoppingPUCCH_F1_3_4;
typedef enum Phy_ParametersFRX_Diff__mux_SR_HARQ_ACK_CSI_PUCCH {
	Phy_ParametersFRX_Diff__mux_SR_HARQ_ACK_CSI_PUCCH_supported	= 0
} e_Phy_ParametersFRX_Diff__mux_SR_HARQ_ACK_CSI_PUCCH;
typedef enum Phy_ParametersFRX_Diff__uci_CodeBlockSegmentation {
	Phy_ParametersFRX_Diff__uci_CodeBlockSegmentation_supported	= 0
} e_Phy_ParametersFRX_Diff__uci_CodeBlockSegmentation;
typedef enum Phy_ParametersFRX_Diff__onePUCCH_LongAndShortFormat {
	Phy_ParametersFRX_Diff__onePUCCH_LongAndShortFormat_supported	= 0
} e_Phy_ParametersFRX_Diff__onePUCCH_LongAndShortFormat;
typedef enum Phy_ParametersFRX_Diff__twoPUCCH_AnyOthersInSlot {
	Phy_ParametersFRX_Diff__twoPUCCH_AnyOthersInSlot_supported	= 0
} e_Phy_ParametersFRX_Diff__twoPUCCH_AnyOthersInSlot;
typedef enum Phy_ParametersFRX_Diff__intraSlotFreqHopping_PUSCH {
	Phy_ParametersFRX_Diff__intraSlotFreqHopping_PUSCH_supported	= 0
} e_Phy_ParametersFRX_Diff__intraSlotFreqHopping_PUSCH;
typedef enum Phy_ParametersFRX_Diff__pusch_LBRM {
	Phy_ParametersFRX_Diff__pusch_LBRM_supported	= 0
} e_Phy_ParametersFRX_Diff__pusch_LBRM;
typedef enum Phy_ParametersFRX_Diff__tpc_PUSCH_RNTI {
	Phy_ParametersFRX_Diff__tpc_PUSCH_RNTI_supported	= 0
} e_Phy_ParametersFRX_Diff__tpc_PUSCH_RNTI;
typedef enum Phy_ParametersFRX_Diff__tpc_PUCCH_RNTI {
	Phy_ParametersFRX_Diff__tpc_PUCCH_RNTI_supported	= 0
} e_Phy_ParametersFRX_Diff__tpc_PUCCH_RNTI;
typedef enum Phy_ParametersFRX_Diff__tpc_SRS_RNTI {
	Phy_ParametersFRX_Diff__tpc_SRS_RNTI_supported	= 0
} e_Phy_ParametersFRX_Diff__tpc_SRS_RNTI;
typedef enum Phy_ParametersFRX_Diff__absoluteTPC_Command {
	Phy_ParametersFRX_Diff__absoluteTPC_Command_supported	= 0
} e_Phy_ParametersFRX_Diff__absoluteTPC_Command;
typedef enum Phy_ParametersFRX_Diff__twoDifferentTPC_Loop_PUSCH {
	Phy_ParametersFRX_Diff__twoDifferentTPC_Loop_PUSCH_supported	= 0
} e_Phy_ParametersFRX_Diff__twoDifferentTPC_Loop_PUSCH;
typedef enum Phy_ParametersFRX_Diff__twoDifferentTPC_Loop_PUCCH {
	Phy_ParametersFRX_Diff__twoDifferentTPC_Loop_PUCCH_supported	= 0
} e_Phy_ParametersFRX_Diff__twoDifferentTPC_Loop_PUCCH;
typedef enum Phy_ParametersFRX_Diff__pusch_HalfPi_BPSK {
	Phy_ParametersFRX_Diff__pusch_HalfPi_BPSK_supported	= 0
} e_Phy_ParametersFRX_Diff__pusch_HalfPi_BPSK;
typedef enum Phy_ParametersFRX_Diff__pucch_F3_4_HalfPi_BPSK {
	Phy_ParametersFRX_Diff__pucch_F3_4_HalfPi_BPSK_supported	= 0
} e_Phy_ParametersFRX_Diff__pucch_F3_4_HalfPi_BPSK;
typedef enum Phy_ParametersFRX_Diff__almostContiguousCP_OFDM_UL {
	Phy_ParametersFRX_Diff__almostContiguousCP_OFDM_UL_supported	= 0
} e_Phy_ParametersFRX_Diff__almostContiguousCP_OFDM_UL;
typedef enum Phy_ParametersFRX_Diff__sp_CSI_RS {
	Phy_ParametersFRX_Diff__sp_CSI_RS_supported	= 0
} e_Phy_ParametersFRX_Diff__sp_CSI_RS;
typedef enum Phy_ParametersFRX_Diff__sp_CSI_IM {
	Phy_ParametersFRX_Diff__sp_CSI_IM_supported	= 0
} e_Phy_ParametersFRX_Diff__sp_CSI_IM;
typedef enum Phy_ParametersFRX_Diff__tdd_MultiDL_UL_SwitchPerSlot {
	Phy_ParametersFRX_Diff__tdd_MultiDL_UL_SwitchPerSlot_supported	= 0
} e_Phy_ParametersFRX_Diff__tdd_MultiDL_UL_SwitchPerSlot;
typedef enum Phy_ParametersFRX_Diff__multipleCORESET {
	Phy_ParametersFRX_Diff__multipleCORESET_supported	= 0
} e_Phy_ParametersFRX_Diff__multipleCORESET;

/* Phy-ParametersFRX-Diff */
typedef struct Phy_ParametersFRX_Diff {
	long	*dynamicSFI;	/* OPTIONAL */
	BIT_STRING_t	*oneFL_DMRS_TwoAdditionalDMRS;	/* OPTIONAL */
	BIT_STRING_t	*twoFL_DMRS;	/* OPTIONAL */
	BIT_STRING_t	*twoFL_DMRS_TwoAdditionalDMRS;	/* OPTIONAL */
	BIT_STRING_t	*oneFL_DMRS_ThreeAdditionalDMRS;	/* OPTIONAL */
	long	*supportedDMRS_TypeDL;	/* OPTIONAL */
	long	*supportedDMRS_TypeUL;	/* OPTIONAL */
	long	*semiOpenLoopCSI;	/* OPTIONAL */
	long	*csi_ReportWithoutPMI;	/* OPTIONAL */
	long	*csi_ReportWithoutCQI;	/* OPTIONAL */
	BIT_STRING_t	*onePortsPTRS;	/* OPTIONAL */
	long	*twoPUCCH_F0_2_ConsecSymbols;	/* OPTIONAL */
	long	*pucch_F2_WithFH;	/* OPTIONAL */
	long	*pucch_F3_WithFH;	/* OPTIONAL */
	long	*pucch_F4_WithFH;	/* OPTIONAL */
	long	*freqHoppingPUCCH_F0_2;	/* OPTIONAL */
	long	*freqHoppingPUCCH_F1_3_4;	/* OPTIONAL */
	long	*mux_SR_HARQ_ACK_CSI_PUCCH;	/* OPTIONAL */
	long	*uci_CodeBlockSegmentation;	/* OPTIONAL */
	long	*onePUCCH_LongAndShortFormat;	/* OPTIONAL */
	long	*twoPUCCH_AnyOthersInSlot;	/* OPTIONAL */
	long	*intraSlotFreqHopping_PUSCH;	/* OPTIONAL */
	long	*pusch_LBRM;	/* OPTIONAL */
	long	*pdcch_BlindDetectionCA;	/* OPTIONAL */
	long	*tpc_PUSCH_RNTI;	/* OPTIONAL */
	long	*tpc_PUCCH_RNTI;	/* OPTIONAL */
	long	*tpc_SRS_RNTI;	/* OPTIONAL */
	long	*absoluteTPC_Command;	/* OPTIONAL */
	long	*twoDifferentTPC_Loop_PUSCH;	/* OPTIONAL */
	long	*twoDifferentTPC_Loop_PUCCH;	/* OPTIONAL */
	long	*pusch_HalfPi_BPSK;	/* OPTIONAL */
	long	*pucch_F3_4_HalfPi_BPSK;	/* OPTIONAL */
	long	*almostContiguousCP_OFDM_UL;	/* OPTIONAL */
	long	*sp_CSI_RS;	/* OPTIONAL */
	long	*sp_CSI_IM;	/* OPTIONAL */
	long	*tdd_MultiDL_UL_SwitchPerSlot;	/* OPTIONAL */
	long	*multipleCORESET;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Phy_ParametersFRX_Diff_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_dynamicSFI_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_supportedDMRS_TypeDL_8;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_supportedDMRS_TypeUL_11;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_semiOpenLoopCSI_14;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_csi_ReportWithoutPMI_16;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_csi_ReportWithoutCQI_18;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_twoPUCCH_F0_2_ConsecSymbols_21;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pucch_F2_WithFH_23;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pucch_F3_WithFH_25;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pucch_F4_WithFH_27;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_freqHoppingPUCCH_F0_2_29;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_freqHoppingPUCCH_F1_3_4_31;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_mux_SR_HARQ_ACK_CSI_PUCCH_33;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_uci_CodeBlockSegmentation_35;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_onePUCCH_LongAndShortFormat_37;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_twoPUCCH_AnyOthersInSlot_39;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_intraSlotFreqHopping_PUSCH_41;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pusch_LBRM_43;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_tpc_PUSCH_RNTI_46;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_tpc_PUCCH_RNTI_48;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_tpc_SRS_RNTI_50;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_absoluteTPC_Command_52;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_twoDifferentTPC_Loop_PUSCH_54;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_twoDifferentTPC_Loop_PUCCH_56;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pusch_HalfPi_BPSK_58;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pucch_F3_4_HalfPi_BPSK_60;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_almostContiguousCP_OFDM_UL_62;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_sp_CSI_RS_64;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_sp_CSI_IM_66;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_tdd_MultiDL_UL_SwitchPerSlot_68;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_multipleCORESET_70;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_Phy_ParametersFRX_Diff;
extern asn_SEQUENCE_specifics_t asn_SPC_Phy_ParametersFRX_Diff_specs_1;
extern asn_TYPE_member_t asn_MBR_Phy_ParametersFRX_Diff_1[37];

#ifdef __cplusplus
}
#endif

#endif	/* _Phy_ParametersFRX_Diff_H_ */
#include <asn_internal.h>
