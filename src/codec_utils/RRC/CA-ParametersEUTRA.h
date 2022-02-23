/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CA_ParametersEUTRA_H_
#define	_CA_ParametersEUTRA_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <BIT_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CA_ParametersEUTRA__multipleTimingAdvance {
	CA_ParametersEUTRA__multipleTimingAdvance_supported	= 0
} e_CA_ParametersEUTRA__multipleTimingAdvance;
typedef enum CA_ParametersEUTRA__simultaneousRx_Tx {
	CA_ParametersEUTRA__simultaneousRx_Tx_supported	= 0
} e_CA_ParametersEUTRA__simultaneousRx_Tx;
typedef enum CA_ParametersEUTRA__additionalRx_Tx_PerformanceReq {
	CA_ParametersEUTRA__additionalRx_Tx_PerformanceReq_supported	= 0
} e_CA_ParametersEUTRA__additionalRx_Tx_PerformanceReq;
typedef enum CA_ParametersEUTRA__ue_CA_PowerClass_N {
	CA_ParametersEUTRA__ue_CA_PowerClass_N_class2	= 0
} e_CA_ParametersEUTRA__ue_CA_PowerClass_N;

/* CA-ParametersEUTRA */
typedef struct CA_ParametersEUTRA {
	long	*multipleTimingAdvance;	/* OPTIONAL */
	long	*simultaneousRx_Tx;	/* OPTIONAL */
	BIT_STRING_t	*supportedNAICS_2CRS_AP;	/* OPTIONAL */
	long	*additionalRx_Tx_PerformanceReq;	/* OPTIONAL */
	long	*ue_CA_PowerClass_N;	/* OPTIONAL */
	BIT_STRING_t	*supportedBandwidthCombinationSetEUTRA_v1530;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CA_ParametersEUTRA_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_multipleTimingAdvance_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_simultaneousRx_Tx_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_additionalRx_Tx_PerformanceReq_7;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ue_CA_PowerClass_N_9;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_CA_ParametersEUTRA;
extern asn_SEQUENCE_specifics_t asn_SPC_CA_ParametersEUTRA_specs_1;
extern asn_TYPE_member_t asn_MBR_CA_ParametersEUTRA_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _CA_ParametersEUTRA_H_ */
#include <asn_internal.h>
