/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUCCH_SpatialRelationInfo_H_
#define	_PUCCH_SpatialRelationInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PUCCH-SpatialRelationInfoId.h"
#include "ServCellIndex.h"
#include "PUCCH-PathlossReferenceRS-Id.h"
#include "P0-PUCCH-Id.h"
#include <NativeEnumerated.h>
#include "SSB-Index.h"
#include "NZP-CSI-RS-ResourceId.h"
#include "SRS-ResourceId.h"
#include "BWP-Id.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PUCCH_SpatialRelationInfo__referenceSignal_PR {
	PUCCH_SpatialRelationInfo__referenceSignal_PR_NOTHING,	/* No components present */
	PUCCH_SpatialRelationInfo__referenceSignal_PR_ssb_Index,
	PUCCH_SpatialRelationInfo__referenceSignal_PR_csi_RS_Index,
	PUCCH_SpatialRelationInfo__referenceSignal_PR_srs
} PUCCH_SpatialRelationInfo__referenceSignal_PR;
typedef enum PUCCH_SpatialRelationInfo__closedLoopIndex {
	PUCCH_SpatialRelationInfo__closedLoopIndex_i0	= 0,
	PUCCH_SpatialRelationInfo__closedLoopIndex_i1	= 1
} e_PUCCH_SpatialRelationInfo__closedLoopIndex;

/* PUCCH-SpatialRelationInfo */
typedef struct PUCCH_SpatialRelationInfo {
	PUCCH_SpatialRelationInfoId_t	 pucch_SpatialRelationInfoId;
	ServCellIndex_t	*servingCellId;	/* OPTIONAL */
	struct PUCCH_SpatialRelationInfo__referenceSignal {
		PUCCH_SpatialRelationInfo__referenceSignal_PR present;
		union PUCCH_SpatialRelationInfo__referenceSignal_u {
			SSB_Index_t	 ssb_Index;
			NZP_CSI_RS_ResourceId_t	 csi_RS_Index;
			struct PUCCH_SpatialRelationInfo__referenceSignal__srs {
				SRS_ResourceId_t	 resource;
				BWP_Id_t	 uplinkBWP;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *srs;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} referenceSignal;
	PUCCH_PathlossReferenceRS_Id_t	 pucch_PathlossReferenceRS_Id;
	P0_PUCCH_Id_t	 p0_PUCCH_Id;
	long	 closedLoopIndex;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUCCH_SpatialRelationInfo_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_closedLoopIndex_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PUCCH_SpatialRelationInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_PUCCH_SpatialRelationInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_PUCCH_SpatialRelationInfo_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _PUCCH_SpatialRelationInfo_H_ */
#include <asn_internal.h>
