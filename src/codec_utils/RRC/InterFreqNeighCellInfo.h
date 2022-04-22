/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_InterFreqNeighCellInfo_H_
#define	_InterFreqNeighCellInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PhysCellId.h"
#include "Q-OffsetRange.h"
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* InterFreqNeighCellInfo */
typedef struct InterFreqNeighCellInfo {
	PhysCellId_t	 physCellId;
	Q_OffsetRange_t	 q_OffsetCell;
	long	*q_RxLevMinOffsetCell;	/* OPTIONAL */
	long	*q_RxLevMinOffsetCellSUL;	/* OPTIONAL */
	long	*q_QualMinOffsetCell;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} InterFreqNeighCellInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_InterFreqNeighCellInfo;
extern asn_SEQUENCE_specifics_t asn_SPC_InterFreqNeighCellInfo_specs_1;
extern asn_TYPE_member_t asn_MBR_InterFreqNeighCellInfo_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _InterFreqNeighCellInfo_H_ */
#include <asn_internal.h>
