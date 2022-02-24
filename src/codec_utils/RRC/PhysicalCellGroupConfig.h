/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PhysicalCellGroupConfig_H_
#define	_PhysicalCellGroupConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include "P-Max.h"
#include "RNTI-Value.h"
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PhysicalCellGroupConfig__harq_ACK_SpatialBundlingPUCCH {
	PhysicalCellGroupConfig__harq_ACK_SpatialBundlingPUCCH_true	= 0
} e_PhysicalCellGroupConfig__harq_ACK_SpatialBundlingPUCCH;
typedef enum PhysicalCellGroupConfig__harq_ACK_SpatialBundlingPUSCH {
	PhysicalCellGroupConfig__harq_ACK_SpatialBundlingPUSCH_true	= 0
} e_PhysicalCellGroupConfig__harq_ACK_SpatialBundlingPUSCH;
typedef enum PhysicalCellGroupConfig__pdsch_HARQ_ACK_Codebook {
	PhysicalCellGroupConfig__pdsch_HARQ_ACK_Codebook_semiStatic	= 0,
	PhysicalCellGroupConfig__pdsch_HARQ_ACK_Codebook_dynamic	= 1
} e_PhysicalCellGroupConfig__pdsch_HARQ_ACK_Codebook;
typedef enum PhysicalCellGroupConfig__cs_RNTI_PR {
	PhysicalCellGroupConfig__cs_RNTI_PR_NOTHING,	/* No components present */
	PhysicalCellGroupConfig__cs_RNTI_PR_release,
	PhysicalCellGroupConfig__cs_RNTI_PR_setup
} PhysicalCellGroupConfig__cs_RNTI_PR;
typedef enum PhysicalCellGroupConfig__ext2__xScale {
	PhysicalCellGroupConfig__ext2__xScale_dB0	= 0,
	PhysicalCellGroupConfig__ext2__xScale_dB6	= 1,
	PhysicalCellGroupConfig__ext2__xScale_spare2	= 2,
	PhysicalCellGroupConfig__ext2__xScale_spare1	= 3
} e_PhysicalCellGroupConfig__ext2__xScale;

/* PhysicalCellGroupConfig */
typedef struct PhysicalCellGroupConfig {
	long	*harq_ACK_SpatialBundlingPUCCH;	/* OPTIONAL */
	long	*harq_ACK_SpatialBundlingPUSCH;	/* OPTIONAL */
	P_Max_t	*p_NR_FR1;	/* OPTIONAL */
	long	 pdsch_HARQ_ACK_Codebook;
	RNTI_Value_t	*tpc_SRS_RNTI;	/* OPTIONAL */
	RNTI_Value_t	*tpc_PUCCH_RNTI;	/* OPTIONAL */
	RNTI_Value_t	*tpc_PUSCH_RNTI;	/* OPTIONAL */
	RNTI_Value_t	*sp_CSI_RNTI;	/* OPTIONAL */
	struct PhysicalCellGroupConfig__cs_RNTI {
		PhysicalCellGroupConfig__cs_RNTI_PR present;
		union PhysicalCellGroupConfig__cs_RNTI_u {
			NULL_t	 release;
			RNTI_Value_t	 setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *cs_RNTI;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct PhysicalCellGroupConfig__ext1 {
		RNTI_Value_t	*mcs_C_RNTI;	/* OPTIONAL */
		P_Max_t	*p_UE_FR1;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	struct PhysicalCellGroupConfig__ext2 {
		long	*xScale;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext2;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PhysicalCellGroupConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_harq_ACK_SpatialBundlingPUCCH_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_harq_ACK_SpatialBundlingPUSCH_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdsch_HARQ_ACK_Codebook_7;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_xScale_22;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PhysicalCellGroupConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_PhysicalCellGroupConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_PhysicalCellGroupConfig_1[11];

#ifdef __cplusplus
}
#endif

#endif	/* _PhysicalCellGroupConfig_H_ */
#include <asn_internal.h>
