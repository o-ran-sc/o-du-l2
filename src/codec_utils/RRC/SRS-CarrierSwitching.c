/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SRS-CarrierSwitching.h"

#include "SRS-TPC-PDCCH-Config.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_typeA_constraint_6(const asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_srs_SwitchFromServCellIndex_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 31)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_monitoringCells_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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

static asn_oer_constraints_t asn_OER_type_srs_SwitchFromCarrier_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_srs_SwitchFromCarrier_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_typeA_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_type_typeA_constr_7 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_typeA_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_memb_typeA_constr_7 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_srs_TPC_PDCCH_Group_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_srs_TPC_PDCCH_Group_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_monitoringCells_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_type_monitoringCells_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_srs_SwitchFromServCellIndex_constr_2 CC_NOTUSED = {
	{ 1, 1 }	/* (0..31) */,
	-1};
static asn_per_constraints_t asn_PER_memb_srs_SwitchFromServCellIndex_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5,  0,  31 }	/* (0..31) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_monitoringCells_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_memb_monitoringCells_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_srs_SwitchFromCarrier_value2enum_3[] = {
	{ 0,	3,	"sUL" },
	{ 1,	3,	"nUL" }
};
static const unsigned int asn_MAP_srs_SwitchFromCarrier_enum2value_3[] = {
	1,	/* nUL(1) */
	0	/* sUL(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_srs_SwitchFromCarrier_specs_3 = {
	asn_MAP_srs_SwitchFromCarrier_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_srs_SwitchFromCarrier_enum2value_3,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_srs_SwitchFromCarrier_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_srs_SwitchFromCarrier_3 = {
	"srs-SwitchFromCarrier",
	"srs-SwitchFromCarrier",
	&asn_OP_NativeEnumerated,
	asn_DEF_srs_SwitchFromCarrier_tags_3,
	sizeof(asn_DEF_srs_SwitchFromCarrier_tags_3)
		/sizeof(asn_DEF_srs_SwitchFromCarrier_tags_3[0]) - 1, /* 1 */
	asn_DEF_srs_SwitchFromCarrier_tags_3,	/* Same as above */
	sizeof(asn_DEF_srs_SwitchFromCarrier_tags_3)
		/sizeof(asn_DEF_srs_SwitchFromCarrier_tags_3[0]), /* 2 */
	{ &asn_OER_type_srs_SwitchFromCarrier_constr_3, &asn_PER_type_srs_SwitchFromCarrier_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_srs_SwitchFromCarrier_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_typeA_7[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_SRS_TPC_PDCCH_Config,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_typeA_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_typeA_specs_7 = {
	sizeof(struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group__typeA),
	offsetof(struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group__typeA, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_typeA_7 = {
	"typeA",
	"typeA",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_typeA_tags_7,
	sizeof(asn_DEF_typeA_tags_7)
		/sizeof(asn_DEF_typeA_tags_7[0]) - 1, /* 1 */
	asn_DEF_typeA_tags_7,	/* Same as above */
	sizeof(asn_DEF_typeA_tags_7)
		/sizeof(asn_DEF_typeA_tags_7[0]), /* 2 */
	{ &asn_OER_type_typeA_constr_7, &asn_PER_type_typeA_constr_7, SEQUENCE_OF_constraint },
	asn_MBR_typeA_7,
	1,	/* Single element */
	&asn_SPC_typeA_specs_7	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_srs_TPC_PDCCH_Group_6[] = {
	{ ATF_POINTER, 0, offsetof(struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group, choice.typeA),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_typeA_7,
		0,
		{ &asn_OER_memb_typeA_constr_7, &asn_PER_memb_typeA_constr_7,  memb_typeA_constraint_6 },
		0, 0, /* No default value */
		"typeA"
		},
	{ ATF_POINTER, 0, offsetof(struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group, choice.typeB),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SRS_TPC_PDCCH_Config,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"typeB"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_srs_TPC_PDCCH_Group_tag2el_6[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* typeA */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* typeB */
};
static asn_CHOICE_specifics_t asn_SPC_srs_TPC_PDCCH_Group_specs_6 = {
	sizeof(struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group),
	offsetof(struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group, _asn_ctx),
	offsetof(struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group, present),
	sizeof(((struct SRS_CarrierSwitching__srs_TPC_PDCCH_Group *)0)->present),
	asn_MAP_srs_TPC_PDCCH_Group_tag2el_6,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_srs_TPC_PDCCH_Group_6 = {
	"srs-TPC-PDCCH-Group",
	"srs-TPC-PDCCH-Group",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_srs_TPC_PDCCH_Group_constr_6, &asn_PER_type_srs_TPC_PDCCH_Group_constr_6, CHOICE_constraint },
	asn_MBR_srs_TPC_PDCCH_Group_6,
	2,	/* Elements count */
	&asn_SPC_srs_TPC_PDCCH_Group_specs_6	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_monitoringCells_10[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_ServCellIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_monitoringCells_tags_10[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_monitoringCells_specs_10 = {
	sizeof(struct SRS_CarrierSwitching__monitoringCells),
	offsetof(struct SRS_CarrierSwitching__monitoringCells, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_monitoringCells_10 = {
	"monitoringCells",
	"monitoringCells",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_monitoringCells_tags_10,
	sizeof(asn_DEF_monitoringCells_tags_10)
		/sizeof(asn_DEF_monitoringCells_tags_10[0]) - 1, /* 1 */
	asn_DEF_monitoringCells_tags_10,	/* Same as above */
	sizeof(asn_DEF_monitoringCells_tags_10)
		/sizeof(asn_DEF_monitoringCells_tags_10[0]), /* 2 */
	{ &asn_OER_type_monitoringCells_constr_10, &asn_PER_type_monitoringCells_constr_10, SEQUENCE_OF_constraint },
	asn_MBR_monitoringCells_10,
	1,	/* Single element */
	&asn_SPC_monitoringCells_specs_10	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SRS_CarrierSwitching_1[] = {
	{ ATF_POINTER, 1, offsetof(struct SRS_CarrierSwitching, srs_SwitchFromServCellIndex),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_srs_SwitchFromServCellIndex_constr_2, &asn_PER_memb_srs_SwitchFromServCellIndex_constr_2,  memb_srs_SwitchFromServCellIndex_constraint_1 },
		0, 0, /* No default value */
		"srs-SwitchFromServCellIndex"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_CarrierSwitching, srs_SwitchFromCarrier),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_srs_SwitchFromCarrier_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"srs-SwitchFromCarrier"
		},
	{ ATF_POINTER, 2, offsetof(struct SRS_CarrierSwitching, srs_TPC_PDCCH_Group),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_srs_TPC_PDCCH_Group_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"srs-TPC-PDCCH-Group"
		},
	{ ATF_POINTER, 1, offsetof(struct SRS_CarrierSwitching, monitoringCells),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_monitoringCells_10,
		0,
		{ &asn_OER_memb_monitoringCells_constr_10, &asn_PER_memb_monitoringCells_constr_10,  memb_monitoringCells_constraint_1 },
		0, 0, /* No default value */
		"monitoringCells"
		},
};
static const int asn_MAP_SRS_CarrierSwitching_oms_1[] = { 0, 2, 3 };
static const ber_tlv_tag_t asn_DEF_SRS_CarrierSwitching_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SRS_CarrierSwitching_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* srs-SwitchFromServCellIndex */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* srs-SwitchFromCarrier */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* srs-TPC-PDCCH-Group */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* monitoringCells */
};
asn_SEQUENCE_specifics_t asn_SPC_SRS_CarrierSwitching_specs_1 = {
	sizeof(struct SRS_CarrierSwitching),
	offsetof(struct SRS_CarrierSwitching, _asn_ctx),
	asn_MAP_SRS_CarrierSwitching_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_SRS_CarrierSwitching_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SRS_CarrierSwitching = {
	"SRS-CarrierSwitching",
	"SRS-CarrierSwitching",
	&asn_OP_SEQUENCE,
	asn_DEF_SRS_CarrierSwitching_tags_1,
	sizeof(asn_DEF_SRS_CarrierSwitching_tags_1)
		/sizeof(asn_DEF_SRS_CarrierSwitching_tags_1[0]), /* 1 */
	asn_DEF_SRS_CarrierSwitching_tags_1,	/* Same as above */
	sizeof(asn_DEF_SRS_CarrierSwitching_tags_1)
		/sizeof(asn_DEF_SRS_CarrierSwitching_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SRS_CarrierSwitching_1,
	4,	/* Elements count */
	&asn_SPC_SRS_CarrierSwitching_specs_1	/* Additional specs */
};

