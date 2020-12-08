/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "MeasObjectEUTRA.h"

#include "EUTRA-CellIndexList.h"
#include "EUTRA-Cell.h"
#include "EUTRA-BlackCell.h"
static int
memb_cellsToAddModListEUTRAN_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 32)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_blackCellsToAddModListEUTRAN_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 32)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_cellsToAddModListEUTRAN_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_type_cellsToAddModListEUTRAN_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_blackCellsToAddModListEUTRAN_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_type_blackCellsToAddModListEUTRAN_constr_8 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_cellsToAddModListEUTRAN_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_memb_cellsToAddModListEUTRAN_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_blackCellsToAddModListEUTRAN_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_memb_blackCellsToAddModListEUTRAN_constr_8 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_cellsToAddModListEUTRAN_5[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_EUTRA_Cell,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_cellsToAddModListEUTRAN_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_cellsToAddModListEUTRAN_specs_5 = {
	sizeof(struct MeasObjectEUTRA__cellsToAddModListEUTRAN),
	offsetof(struct MeasObjectEUTRA__cellsToAddModListEUTRAN, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_cellsToAddModListEUTRAN_5 = {
	"cellsToAddModListEUTRAN",
	"cellsToAddModListEUTRAN",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_cellsToAddModListEUTRAN_tags_5,
	sizeof(asn_DEF_cellsToAddModListEUTRAN_tags_5)
		/sizeof(asn_DEF_cellsToAddModListEUTRAN_tags_5[0]) - 1, /* 1 */
	asn_DEF_cellsToAddModListEUTRAN_tags_5,	/* Same as above */
	sizeof(asn_DEF_cellsToAddModListEUTRAN_tags_5)
		/sizeof(asn_DEF_cellsToAddModListEUTRAN_tags_5[0]), /* 2 */
	{ &asn_OER_type_cellsToAddModListEUTRAN_constr_5, &asn_PER_type_cellsToAddModListEUTRAN_constr_5, SEQUENCE_OF_constraint },
	asn_MBR_cellsToAddModListEUTRAN_5,
	1,	/* Single element */
	&asn_SPC_cellsToAddModListEUTRAN_specs_5	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_blackCellsToAddModListEUTRAN_8[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_EUTRA_BlackCell,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_blackCellsToAddModListEUTRAN_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_blackCellsToAddModListEUTRAN_specs_8 = {
	sizeof(struct MeasObjectEUTRA__blackCellsToAddModListEUTRAN),
	offsetof(struct MeasObjectEUTRA__blackCellsToAddModListEUTRAN, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_blackCellsToAddModListEUTRAN_8 = {
	"blackCellsToAddModListEUTRAN",
	"blackCellsToAddModListEUTRAN",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_blackCellsToAddModListEUTRAN_tags_8,
	sizeof(asn_DEF_blackCellsToAddModListEUTRAN_tags_8)
		/sizeof(asn_DEF_blackCellsToAddModListEUTRAN_tags_8[0]) - 1, /* 1 */
	asn_DEF_blackCellsToAddModListEUTRAN_tags_8,	/* Same as above */
	sizeof(asn_DEF_blackCellsToAddModListEUTRAN_tags_8)
		/sizeof(asn_DEF_blackCellsToAddModListEUTRAN_tags_8[0]), /* 2 */
	{ &asn_OER_type_blackCellsToAddModListEUTRAN_constr_8, &asn_PER_type_blackCellsToAddModListEUTRAN_constr_8, SEQUENCE_OF_constraint },
	asn_MBR_blackCellsToAddModListEUTRAN_8,
	1,	/* Single element */
	&asn_SPC_blackCellsToAddModListEUTRAN_specs_8	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_MeasObjectEUTRA_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct MeasObjectEUTRA, carrierFreq),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ARFCN_ValueEUTRA,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"carrierFreq"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MeasObjectEUTRA, allowedMeasBandwidth),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_AllowedMeasBandwidth,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"allowedMeasBandwidth"
		},
	{ ATF_POINTER, 4, offsetof(struct MeasObjectEUTRA, cellsToRemoveListEUTRAN),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_CellIndexList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cellsToRemoveListEUTRAN"
		},
	{ ATF_POINTER, 3, offsetof(struct MeasObjectEUTRA, cellsToAddModListEUTRAN),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_cellsToAddModListEUTRAN_5,
		0,
		{ &asn_OER_memb_cellsToAddModListEUTRAN_constr_5, &asn_PER_memb_cellsToAddModListEUTRAN_constr_5,  memb_cellsToAddModListEUTRAN_constraint_1 },
		0, 0, /* No default value */
		"cellsToAddModListEUTRAN"
		},
	{ ATF_POINTER, 2, offsetof(struct MeasObjectEUTRA, blackCellsToRemoveListEUTRAN),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_CellIndexList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"blackCellsToRemoveListEUTRAN"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasObjectEUTRA, blackCellsToAddModListEUTRAN),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		0,
		&asn_DEF_blackCellsToAddModListEUTRAN_8,
		0,
		{ &asn_OER_memb_blackCellsToAddModListEUTRAN_constr_8, &asn_PER_memb_blackCellsToAddModListEUTRAN_constr_8,  memb_blackCellsToAddModListEUTRAN_constraint_1 },
		0, 0, /* No default value */
		"blackCellsToAddModListEUTRAN"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MeasObjectEUTRA, eutra_PresenceAntennaPort1),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_PresenceAntennaPort1,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eutra-PresenceAntennaPort1"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasObjectEUTRA, eutra_Q_OffsetRange),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eutra-Q-OffsetRange"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MeasObjectEUTRA, widebandRSRQ_Meas),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"widebandRSRQ-Meas"
		},
};
static const int asn_MAP_MeasObjectEUTRA_oms_1[] = { 2, 3, 4, 5, 7 };
static const ber_tlv_tag_t asn_DEF_MeasObjectEUTRA_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MeasObjectEUTRA_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* carrierFreq */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* allowedMeasBandwidth */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* cellsToRemoveListEUTRAN */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* cellsToAddModListEUTRAN */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* blackCellsToRemoveListEUTRAN */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* blackCellsToAddModListEUTRAN */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* eutra-PresenceAntennaPort1 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* eutra-Q-OffsetRange */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 } /* widebandRSRQ-Meas */
};
asn_SEQUENCE_specifics_t asn_SPC_MeasObjectEUTRA_specs_1 = {
	sizeof(struct MeasObjectEUTRA),
	offsetof(struct MeasObjectEUTRA, _asn_ctx),
	asn_MAP_MeasObjectEUTRA_tag2el_1,
	9,	/* Count of tags in the map */
	asn_MAP_MeasObjectEUTRA_oms_1,	/* Optional members */
	5, 0,	/* Root/Additions */
	9,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MeasObjectEUTRA = {
	"MeasObjectEUTRA",
	"MeasObjectEUTRA",
	&asn_OP_SEQUENCE,
	asn_DEF_MeasObjectEUTRA_tags_1,
	sizeof(asn_DEF_MeasObjectEUTRA_tags_1)
		/sizeof(asn_DEF_MeasObjectEUTRA_tags_1[0]), /* 1 */
	asn_DEF_MeasObjectEUTRA_tags_1,	/* Same as above */
	sizeof(asn_DEF_MeasObjectEUTRA_tags_1)
		/sizeof(asn_DEF_MeasObjectEUTRA_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_MeasObjectEUTRA_1,
	9,	/* Elements count */
	&asn_SPC_MeasObjectEUTRA_specs_1	/* Additional specs */
};

