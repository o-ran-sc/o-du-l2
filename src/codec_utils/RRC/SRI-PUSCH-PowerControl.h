/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_SRI_PUSCH_PowerControl_H_
#define	_SRI_PUSCH_PowerControl_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SRI-PUSCH-PowerControlId.h"
#include "PUSCH-PathlossReferenceRS-Id.h"
#include "P0-PUSCH-AlphaSetId.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SRI_PUSCH_PowerControl__sri_PUSCH_ClosedLoopIndex {
	SRI_PUSCH_PowerControl__sri_PUSCH_ClosedLoopIndex_i0	= 0,
	SRI_PUSCH_PowerControl__sri_PUSCH_ClosedLoopIndex_i1	= 1
} e_SRI_PUSCH_PowerControl__sri_PUSCH_ClosedLoopIndex;

/* SRI-PUSCH-PowerControl */
typedef struct SRI_PUSCH_PowerControl {
	SRI_PUSCH_PowerControlId_t	 sri_PUSCH_PowerControlId;
	PUSCH_PathlossReferenceRS_Id_t	 sri_PUSCH_PathlossReferenceRS_Id;
	P0_PUSCH_AlphaSetId_t	 sri_P0_PUSCH_AlphaSetId;
	long	 sri_PUSCH_ClosedLoopIndex;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SRI_PUSCH_PowerControl_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_sri_PUSCH_ClosedLoopIndex_5;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SRI_PUSCH_PowerControl;
extern asn_SEQUENCE_specifics_t asn_SPC_SRI_PUSCH_PowerControl_specs_1;
extern asn_TYPE_member_t asn_MBR_SRI_PUSCH_PowerControl_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _SRI_PUSCH_PowerControl_H_ */
#include <asn_internal.h>
