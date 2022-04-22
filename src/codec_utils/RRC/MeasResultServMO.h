/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasResultServMO_H_
#define	_MeasResultServMO_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ServCellIndex.h"
#include "MeasResultNR.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MeasResultNR;

/* MeasResultServMO */
typedef struct MeasResultServMO {
	ServCellIndex_t	 servCellId;
	MeasResultNR_t	 measResultServingCell;
	struct MeasResultNR	*measResultBestNeighCell;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasResultServMO_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasResultServMO;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasResultServMO_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasResultServMO_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasResultServMO_H_ */
#include <asn_internal.h>
