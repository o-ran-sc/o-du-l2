/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PDSCH_ServingCellConfig_H_
#define	_PDSCH_ServingCellConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include "ServCellIndex.h"
#include <NULL.h>
#include <constr_CHOICE.h>
#include <NativeInteger.h>
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PDSCH_ServingCellConfig__codeBlockGroupTransmission_PR {
	PDSCH_ServingCellConfig__codeBlockGroupTransmission_PR_NOTHING,	/* No components present */
	PDSCH_ServingCellConfig__codeBlockGroupTransmission_PR_release,
	PDSCH_ServingCellConfig__codeBlockGroupTransmission_PR_setup
} PDSCH_ServingCellConfig__codeBlockGroupTransmission_PR;
typedef enum PDSCH_ServingCellConfig__xOverhead {
	PDSCH_ServingCellConfig__xOverhead_xOh6	= 0,
	PDSCH_ServingCellConfig__xOverhead_xOh12	= 1,
	PDSCH_ServingCellConfig__xOverhead_xOh18	= 2
} e_PDSCH_ServingCellConfig__xOverhead;
typedef enum PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH {
	PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH_n2	= 0,
	PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH_n4	= 1,
	PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH_n6	= 2,
	PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH_n10	= 3,
	PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH_n12	= 4,
	PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH_n16	= 5
} e_PDSCH_ServingCellConfig__nrofHARQ_ProcessesForPDSCH;

/* Forward declarations */
struct PDSCH_CodeBlockGroupTransmission;

/* PDSCH-ServingCellConfig */
typedef struct PDSCH_ServingCellConfig {
	struct PDSCH_ServingCellConfig__codeBlockGroupTransmission {
		PDSCH_ServingCellConfig__codeBlockGroupTransmission_PR present;
		union PDSCH_ServingCellConfig__codeBlockGroupTransmission_u {
			NULL_t	 release;
			struct PDSCH_CodeBlockGroupTransmission	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *codeBlockGroupTransmission;
	long	*xOverhead;	/* OPTIONAL */
	long	*nrofHARQ_ProcessesForPDSCH;	/* OPTIONAL */
	ServCellIndex_t	*pucch_Cell;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct PDSCH_ServingCellConfig__ext1 {
		long	*maxMIMO_Layers;	/* OPTIONAL */
		BOOLEAN_t	*processingType2Enabled;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDSCH_ServingCellConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_xOverhead_5;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_nrofHARQ_ProcessesForPDSCH_9;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PDSCH_ServingCellConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_PDSCH_ServingCellConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_PDSCH_ServingCellConfig_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _PDSCH_ServingCellConfig_H_ */
#include <asn_internal.h>
