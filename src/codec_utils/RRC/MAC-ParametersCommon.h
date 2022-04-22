/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MAC_ParametersCommon_H_
#define	_MAC_ParametersCommon_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MAC_ParametersCommon__lcp_Restriction {
	MAC_ParametersCommon__lcp_Restriction_supported	= 0
} e_MAC_ParametersCommon__lcp_Restriction;
typedef enum MAC_ParametersCommon__pucch_SpatialRelInfoMAC_CE {
	MAC_ParametersCommon__pucch_SpatialRelInfoMAC_CE_supported	= 0
} e_MAC_ParametersCommon__pucch_SpatialRelInfoMAC_CE;
typedef enum MAC_ParametersCommon__lch_ToSCellRestriction {
	MAC_ParametersCommon__lch_ToSCellRestriction_supported	= 0
} e_MAC_ParametersCommon__lch_ToSCellRestriction;
typedef enum MAC_ParametersCommon__ext1__recommendedBitRate {
	MAC_ParametersCommon__ext1__recommendedBitRate_supported	= 0
} e_MAC_ParametersCommon__ext1__recommendedBitRate;
typedef enum MAC_ParametersCommon__ext1__recommendedBitRateQuery {
	MAC_ParametersCommon__ext1__recommendedBitRateQuery_supported	= 0
} e_MAC_ParametersCommon__ext1__recommendedBitRateQuery;

/* MAC-ParametersCommon */
typedef struct MAC_ParametersCommon {
	long	*lcp_Restriction;	/* OPTIONAL */
	long	*pucch_SpatialRelInfoMAC_CE;	/* OPTIONAL */
	long	*lch_ToSCellRestriction;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct MAC_ParametersCommon__ext1 {
		long	*recommendedBitRate;	/* OPTIONAL */
		long	*recommendedBitRateQuery;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MAC_ParametersCommon_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_lcp_Restriction_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pucch_SpatialRelInfoMAC_CE_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_lch_ToSCellRestriction_6;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_recommendedBitRate_10;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_recommendedBitRateQuery_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_MAC_ParametersCommon;
extern asn_SEQUENCE_specifics_t asn_SPC_MAC_ParametersCommon_specs_1;
extern asn_TYPE_member_t asn_MBR_MAC_ParametersCommon_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _MAC_ParametersCommon_H_ */
#include <asn_internal.h>
