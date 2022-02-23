/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_ServingCellConfigCommonSIB_H_
#define	_ServingCellConfigCommonSIB_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DownlinkConfigCommonSIB.h"
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <BIT_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ServingCellConfigCommonSIB__n_TimingAdvanceOffset {
	ServingCellConfigCommonSIB__n_TimingAdvanceOffset_n0	= 0,
	ServingCellConfigCommonSIB__n_TimingAdvanceOffset_n25560	= 1,
	ServingCellConfigCommonSIB__n_TimingAdvanceOffset_n39936	= 2
} e_ServingCellConfigCommonSIB__n_TimingAdvanceOffset;
typedef enum ServingCellConfigCommonSIB__ssb_PeriodicityServingCell {
	ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms5	= 0,
	ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms10	= 1,
	ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms20	= 2,
	ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms40	= 3,
	ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms80	= 4,
	ServingCellConfigCommonSIB__ssb_PeriodicityServingCell_ms160	= 5
} e_ServingCellConfigCommonSIB__ssb_PeriodicityServingCell;

/* Forward declarations */
struct UplinkConfigCommonSIB;
struct TDD_UL_DL_ConfigCommon;

/* ServingCellConfigCommonSIB */
typedef struct ServingCellConfigCommonSIB {
	DownlinkConfigCommonSIB_t	 downlinkConfigCommon;
	struct UplinkConfigCommonSIB	*uplinkConfigCommon;	/* OPTIONAL */
	struct UplinkConfigCommonSIB	*supplementaryUplink;	/* OPTIONAL */
	long	*n_TimingAdvanceOffset;	/* OPTIONAL */
	struct ServingCellConfigCommonSIB__ssb_PositionsInBurst {
		BIT_STRING_t	 inOneGroup;
		BIT_STRING_t	*groupPresence;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} ssb_PositionsInBurst;
	long	 ssb_PeriodicityServingCell;
	struct TDD_UL_DL_ConfigCommon	*tdd_UL_DL_ConfigurationCommon;	/* OPTIONAL */
	long	 ss_PBCH_BlockPower;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ServingCellConfigCommonSIB_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_n_TimingAdvanceOffset_5;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ssb_PeriodicityServingCell_12;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_ServingCellConfigCommonSIB;
extern asn_SEQUENCE_specifics_t asn_SPC_ServingCellConfigCommonSIB_specs_1;
extern asn_TYPE_member_t asn_MBR_ServingCellConfigCommonSIB_1[8];

#ifdef __cplusplus
}
#endif

#endif	/* _ServingCellConfigCommonSIB_H_ */
#include <asn_internal.h>
