/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ControlResourceSet_H_
#define	_ControlResourceSet_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ControlResourceSetId.h"
#include <BIT_STRING.h>
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <NULL.h>
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>
#include "TCI-StateId.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ControlResourceSet__cce_REG_MappingType_PR {
	ControlResourceSet__cce_REG_MappingType_PR_NOTHING,	/* No components present */
	ControlResourceSet__cce_REG_MappingType_PR_interleaved,
	ControlResourceSet__cce_REG_MappingType_PR_nonInterleaved
} ControlResourceSet__cce_REG_MappingType_PR;
typedef enum ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize {
	ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n2	= 0,
	ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n3	= 1,
	ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n6	= 2
} e_ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize;
typedef enum ControlResourceSet__cce_REG_MappingType__interleaved__interleaverSize {
	ControlResourceSet__cce_REG_MappingType__interleaved__interleaverSize_n2	= 0,
	ControlResourceSet__cce_REG_MappingType__interleaved__interleaverSize_n3	= 1,
	ControlResourceSet__cce_REG_MappingType__interleaved__interleaverSize_n6	= 2
} e_ControlResourceSet__cce_REG_MappingType__interleaved__interleaverSize;
typedef enum ControlResourceSet__precoderGranularity {
	ControlResourceSet__precoderGranularity_sameAsREG_bundle	= 0,
	ControlResourceSet__precoderGranularity_allContiguousRBs	= 1
} e_ControlResourceSet__precoderGranularity;
typedef enum ControlResourceSet__tci_PresentInDCI {
	ControlResourceSet__tci_PresentInDCI_enabled	= 0
} e_ControlResourceSet__tci_PresentInDCI;

/* ControlResourceSet */
typedef struct ControlResourceSet {
	ControlResourceSetId_t	 controlResourceSetId;
	BIT_STRING_t	 frequencyDomainResources;
	long	 duration;
	struct ControlResourceSet__cce_REG_MappingType {
		ControlResourceSet__cce_REG_MappingType_PR present;
		union ControlResourceSet__cce_REG_MappingType_u {
			struct ControlResourceSet__cce_REG_MappingType__interleaved {
				long	 reg_BundleSize;
				long	 interleaverSize;
				long	*shiftIndex;	/* OPTIONAL */
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *interleaved;
			NULL_t	 nonInterleaved;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} cce_REG_MappingType;
	long	 precoderGranularity;
	struct ControlResourceSet__tci_StatesPDCCH_ToAddList {
		A_SEQUENCE_OF(TCI_StateId_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *tci_StatesPDCCH_ToAddList;
	struct ControlResourceSet__tci_StatesPDCCH_ToReleaseList {
		A_SEQUENCE_OF(TCI_StateId_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *tci_StatesPDCCH_ToReleaseList;
	long	*tci_PresentInDCI;	/* OPTIONAL */
	long	*pdcch_DMRS_ScramblingID;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ControlResourceSet_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_reg_BundleSize_7;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_interleaverSize_11;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_precoderGranularity_17;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_tci_PresentInDCI_24;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_ControlResourceSet;
extern asn_SEQUENCE_specifics_t asn_SPC_ControlResourceSet_specs_1;
extern asn_TYPE_member_t asn_MBR_ControlResourceSet_1[9];

#ifdef __cplusplus
}
#endif

#endif	/* _ControlResourceSet_H_ */
#include <asn_internal.h>
