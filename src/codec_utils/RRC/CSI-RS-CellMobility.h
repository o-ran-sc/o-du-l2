/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#ifndef	_CSI_RS_CellMobility_H_
#define	_CSI_RS_CellMobility_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PhysCellId.h"
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs {
	CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size24	= 0,
	CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size48	= 1,
	CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size96	= 2,
	CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size192	= 3,
	CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size264	= 4
} e_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs;
typedef enum CSI_RS_CellMobility__density {
	CSI_RS_CellMobility__density_d1	= 0,
	CSI_RS_CellMobility__density_d3	= 1
} e_CSI_RS_CellMobility__density;

/* Forward declarations */
struct CSI_RS_Resource_Mobility;

/* CSI-RS-CellMobility */
typedef struct CSI_RS_CellMobility {
	PhysCellId_t	 cellId;
	struct CSI_RS_CellMobility__csi_rs_MeasurementBW {
		long	 nrofPRBs;
		long	 startPRB;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} csi_rs_MeasurementBW;
	long	*density;	/* OPTIONAL */
	struct CSI_RS_CellMobility__csi_rs_ResourceList_Mobility {
		A_SEQUENCE_OF(struct CSI_RS_Resource_Mobility) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} csi_rs_ResourceList_Mobility;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CSI_RS_CellMobility_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_nrofPRBs_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_density_11;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_CSI_RS_CellMobility;
extern asn_SEQUENCE_specifics_t asn_SPC_CSI_RS_CellMobility_specs_1;
extern asn_TYPE_member_t asn_MBR_CSI_RS_CellMobility_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _CSI_RS_CellMobility_H_ */
#include <asn_internal.h>
