/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CSI_ResourceConfig_H_
#define	_CSI_ResourceConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CSI-ResourceConfigId.h"
#include "BWP-Id.h"
#include <NativeEnumerated.h>
#include "NZP-CSI-RS-ResourceSetId.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include "CSI-SSB-ResourceSetId.h"
#include <constr_SEQUENCE.h>
#include "CSI-IM-ResourceSetId.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CSI_ResourceConfig__csi_RS_ResourceSetList_PR {
	CSI_ResourceConfig__csi_RS_ResourceSetList_PR_NOTHING,	/* No components present */
	CSI_ResourceConfig__csi_RS_ResourceSetList_PR_nzp_CSI_RS_SSB,
	CSI_ResourceConfig__csi_RS_ResourceSetList_PR_csi_IM_ResourceSetList
} CSI_ResourceConfig__csi_RS_ResourceSetList_PR;
typedef enum CSI_ResourceConfig__resourceType {
	CSI_ResourceConfig__resourceType_aperiodic	= 0,
	CSI_ResourceConfig__resourceType_semiPersistent	= 1,
	CSI_ResourceConfig__resourceType_periodic	= 2
} e_CSI_ResourceConfig__resourceType;

/* CSI-ResourceConfig */
typedef struct CSI_ResourceConfig {
	CSI_ResourceConfigId_t	 csi_ResourceConfigId;
	struct CSI_ResourceConfig__csi_RS_ResourceSetList {
		CSI_ResourceConfig__csi_RS_ResourceSetList_PR present;
		union CSI_ResourceConfig__csi_RS_ResourceSetList_u {
			struct CSI_ResourceConfig__csi_RS_ResourceSetList__nzp_CSI_RS_SSB {
				struct CSI_ResourceConfig__csi_RS_ResourceSetList__nzp_CSI_RS_SSB__nzp_CSI_RS_ResourceSetList {
					A_SEQUENCE_OF(NZP_CSI_RS_ResourceSetId_t) list;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} *nzp_CSI_RS_ResourceSetList;
				struct CSI_ResourceConfig__csi_RS_ResourceSetList__nzp_CSI_RS_SSB__csi_SSB_ResourceSetList {
					A_SEQUENCE_OF(CSI_SSB_ResourceSetId_t) list;
					
					/* Context for parsing across buffer boundaries */
					asn_struct_ctx_t _asn_ctx;
				} *csi_SSB_ResourceSetList;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *nzp_CSI_RS_SSB;
			struct CSI_ResourceConfig__csi_RS_ResourceSetList__csi_IM_ResourceSetList {
				A_SEQUENCE_OF(CSI_IM_ResourceSetId_t) list;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} *csi_IM_ResourceSetList;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} csi_RS_ResourceSetList;
	BWP_Id_t	 bwp_Id;
	long	 resourceType;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CSI_ResourceConfig_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_resourceType_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_CSI_ResourceConfig;
extern asn_SEQUENCE_specifics_t asn_SPC_CSI_ResourceConfig_specs_1;
extern asn_TYPE_member_t asn_MBR_CSI_ResourceConfig_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _CSI_ResourceConfig_H_ */
#include <asn_internal.h>
