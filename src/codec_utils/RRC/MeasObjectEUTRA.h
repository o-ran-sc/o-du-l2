/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasObjectEUTRA_H_
#define	_MeasObjectEUTRA_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ARFCN-ValueEUTRA.h"
#include "EUTRA-AllowedMeasBandwidth.h"
#include "EUTRA-PresenceAntennaPort1.h"
#include "EUTRA-Q-OffsetRange.h"
#include <BOOLEAN.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct EUTRA_CellIndexList;
struct EUTRA_Cell;
struct EUTRA_BlackCell;

/* MeasObjectEUTRA */
typedef struct MeasObjectEUTRA {
	ARFCN_ValueEUTRA_t	 carrierFreq;
	EUTRA_AllowedMeasBandwidth_t	 allowedMeasBandwidth;
	struct EUTRA_CellIndexList	*cellsToRemoveListEUTRAN;	/* OPTIONAL */
	struct MeasObjectEUTRA__cellsToAddModListEUTRAN {
		A_SEQUENCE_OF(struct EUTRA_Cell) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *cellsToAddModListEUTRAN;
	struct EUTRA_CellIndexList	*blackCellsToRemoveListEUTRAN;	/* OPTIONAL */
	struct MeasObjectEUTRA__blackCellsToAddModListEUTRAN {
		A_SEQUENCE_OF(struct EUTRA_BlackCell) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *blackCellsToAddModListEUTRAN;
	EUTRA_PresenceAntennaPort1_t	 eutra_PresenceAntennaPort1;
	EUTRA_Q_OffsetRange_t	*eutra_Q_OffsetRange;	/* OPTIONAL */
	BOOLEAN_t	 widebandRSRQ_Meas;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasObjectEUTRA_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasObjectEUTRA;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasObjectEUTRA_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasObjectEUTRA_1[9];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasObjectEUTRA_H_ */
#include <asn_internal.h>
