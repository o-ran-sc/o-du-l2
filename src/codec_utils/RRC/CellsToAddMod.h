/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_CellsToAddMod_H_
#define	_CellsToAddMod_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PhysCellId.h"
#include "Q-OffsetRangeList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CellsToAddMod */
typedef struct CellsToAddMod {
	PhysCellId_t	 physCellId;
	Q_OffsetRangeList_t	 cellIndividualOffset;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CellsToAddMod_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CellsToAddMod;
extern asn_SEQUENCE_specifics_t asn_SPC_CellsToAddMod_specs_1;
extern asn_TYPE_member_t asn_MBR_CellsToAddMod_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _CellsToAddMod_H_ */
#include <asn_internal.h>
