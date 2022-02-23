/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "CellGroupConfigRrc.h"

#include "MAC-CellGroupConfig.h"
#include "PhysicalCellGroupConfig.h"
#include "SpCellConfig.h"
#include "RLC-BearerConfig.h"
#include "SCellConfig.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_rlc_BearerToAddModList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_rlc_BearerToReleaseList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_sCellToAddModList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 31)) {
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
memb_sCellToReleaseList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 31)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_rlc_BearerToAddModList_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_type_rlc_BearerToAddModList_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_rlc_BearerToReleaseList_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_type_rlc_BearerToReleaseList_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_sCellToAddModList_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..31)) */};
static asn_per_constraints_t asn_PER_type_sCellToAddModList_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  31 }	/* (SIZE(1..31)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_sCellToReleaseList_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..31)) */};
static asn_per_constraints_t asn_PER_type_sCellToReleaseList_constr_12 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  31 }	/* (SIZE(1..31)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_reportUplinkTxDirectCurrent_v1530_constr_16 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_reportUplinkTxDirectCurrent_v1530_constr_16 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_rlc_BearerToAddModList_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_memb_rlc_BearerToAddModList_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_rlc_BearerToReleaseList_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..32)) */};
static asn_per_constraints_t asn_PER_memb_rlc_BearerToReleaseList_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (SIZE(1..32)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_sCellToAddModList_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..31)) */};
static asn_per_constraints_t asn_PER_memb_sCellToAddModList_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  31 }	/* (SIZE(1..31)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_sCellToReleaseList_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..31)) */};
static asn_per_constraints_t asn_PER_memb_sCellToReleaseList_constr_12 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 5,  5,  1,  31 }	/* (SIZE(1..31)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_rlc_BearerToAddModList_3[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_RLC_BearerConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_rlc_BearerToAddModList_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_rlc_BearerToAddModList_specs_3 = {
	sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList),
	offsetof(struct CellGroupConfigRrc__rlc_BearerToAddModList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_rlc_BearerToAddModList_3 = {
	"rlc-BearerToAddModList",
	"rlc-BearerToAddModList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_rlc_BearerToAddModList_tags_3,
	sizeof(asn_DEF_rlc_BearerToAddModList_tags_3)
		/sizeof(asn_DEF_rlc_BearerToAddModList_tags_3[0]) - 1, /* 1 */
	asn_DEF_rlc_BearerToAddModList_tags_3,	/* Same as above */
	sizeof(asn_DEF_rlc_BearerToAddModList_tags_3)
		/sizeof(asn_DEF_rlc_BearerToAddModList_tags_3[0]), /* 2 */
	{ &asn_OER_type_rlc_BearerToAddModList_constr_3, &asn_PER_type_rlc_BearerToAddModList_constr_3, SEQUENCE_OF_constraint },
	asn_MBR_rlc_BearerToAddModList_3,
	1,	/* Single element */
	&asn_SPC_rlc_BearerToAddModList_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_rlc_BearerToReleaseList_5[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_LogicalChannelIdentity,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_rlc_BearerToReleaseList_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_rlc_BearerToReleaseList_specs_5 = {
	sizeof(struct CellGroupConfigRrc__rlc_BearerToReleaseList),
	offsetof(struct CellGroupConfigRrc__rlc_BearerToReleaseList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_rlc_BearerToReleaseList_5 = {
	"rlc-BearerToReleaseList",
	"rlc-BearerToReleaseList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_rlc_BearerToReleaseList_tags_5,
	sizeof(asn_DEF_rlc_BearerToReleaseList_tags_5)
		/sizeof(asn_DEF_rlc_BearerToReleaseList_tags_5[0]) - 1, /* 1 */
	asn_DEF_rlc_BearerToReleaseList_tags_5,	/* Same as above */
	sizeof(asn_DEF_rlc_BearerToReleaseList_tags_5)
		/sizeof(asn_DEF_rlc_BearerToReleaseList_tags_5[0]), /* 2 */
	{ &asn_OER_type_rlc_BearerToReleaseList_constr_5, &asn_PER_type_rlc_BearerToReleaseList_constr_5, SEQUENCE_OF_constraint },
	asn_MBR_rlc_BearerToReleaseList_5,
	1,	/* Single element */
	&asn_SPC_rlc_BearerToReleaseList_specs_5	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_sCellToAddModList_10[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_SCellConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_sCellToAddModList_tags_10[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_sCellToAddModList_specs_10 = {
	sizeof(struct CellGroupConfigRrc__sCellToAddModList),
	offsetof(struct CellGroupConfigRrc__sCellToAddModList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sCellToAddModList_10 = {
	"sCellToAddModList",
	"sCellToAddModList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_sCellToAddModList_tags_10,
	sizeof(asn_DEF_sCellToAddModList_tags_10)
		/sizeof(asn_DEF_sCellToAddModList_tags_10[0]) - 1, /* 1 */
	asn_DEF_sCellToAddModList_tags_10,	/* Same as above */
	sizeof(asn_DEF_sCellToAddModList_tags_10)
		/sizeof(asn_DEF_sCellToAddModList_tags_10[0]), /* 2 */
	{ &asn_OER_type_sCellToAddModList_constr_10, &asn_PER_type_sCellToAddModList_constr_10, SEQUENCE_OF_constraint },
	asn_MBR_sCellToAddModList_10,
	1,	/* Single element */
	&asn_SPC_sCellToAddModList_specs_10	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_sCellToReleaseList_12[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_SCellIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_sCellToReleaseList_tags_12[] = {
	(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_sCellToReleaseList_specs_12 = {
	sizeof(struct CellGroupConfigRrc__sCellToReleaseList),
	offsetof(struct CellGroupConfigRrc__sCellToReleaseList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sCellToReleaseList_12 = {
	"sCellToReleaseList",
	"sCellToReleaseList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_sCellToReleaseList_tags_12,
	sizeof(asn_DEF_sCellToReleaseList_tags_12)
		/sizeof(asn_DEF_sCellToReleaseList_tags_12[0]) - 1, /* 1 */
	asn_DEF_sCellToReleaseList_tags_12,	/* Same as above */
	sizeof(asn_DEF_sCellToReleaseList_tags_12)
		/sizeof(asn_DEF_sCellToReleaseList_tags_12[0]), /* 2 */
	{ &asn_OER_type_sCellToReleaseList_constr_12, &asn_PER_type_sCellToReleaseList_constr_12, SEQUENCE_OF_constraint },
	asn_MBR_sCellToReleaseList_12,
	1,	/* Single element */
	&asn_SPC_sCellToReleaseList_specs_12	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_reportUplinkTxDirectCurrent_v1530_value2enum_16[] = {
	{ 0,	4,	"true" }
};
static const unsigned int asn_MAP_reportUplinkTxDirectCurrent_v1530_enum2value_16[] = {
	0	/* true(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_reportUplinkTxDirectCurrent_v1530_specs_16 = {
	asn_MAP_reportUplinkTxDirectCurrent_v1530_value2enum_16,	/* "tag" => N; sorted by tag */
	asn_MAP_reportUplinkTxDirectCurrent_v1530_enum2value_16,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_reportUplinkTxDirectCurrent_v1530_tags_16[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_reportUplinkTxDirectCurrent_v1530_16 = {
	"reportUplinkTxDirectCurrent-v1530",
	"reportUplinkTxDirectCurrent-v1530",
	&asn_OP_NativeEnumerated,
	asn_DEF_reportUplinkTxDirectCurrent_v1530_tags_16,
	sizeof(asn_DEF_reportUplinkTxDirectCurrent_v1530_tags_16)
		/sizeof(asn_DEF_reportUplinkTxDirectCurrent_v1530_tags_16[0]) - 1, /* 1 */
	asn_DEF_reportUplinkTxDirectCurrent_v1530_tags_16,	/* Same as above */
	sizeof(asn_DEF_reportUplinkTxDirectCurrent_v1530_tags_16)
		/sizeof(asn_DEF_reportUplinkTxDirectCurrent_v1530_tags_16[0]), /* 2 */
	{ &asn_OER_type_reportUplinkTxDirectCurrent_v1530_constr_16, &asn_PER_type_reportUplinkTxDirectCurrent_v1530_constr_16, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_reportUplinkTxDirectCurrent_v1530_specs_16	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ext1_15[] = {
	{ ATF_POINTER, 1, offsetof(struct CellGroupConfigRrc__ext1, reportUplinkTxDirectCurrent_v1530),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_reportUplinkTxDirectCurrent_v1530_16,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportUplinkTxDirectCurrent-v1530"
		},
};
static const int asn_MAP_ext1_oms_15[] = { 0 };
static const ber_tlv_tag_t asn_DEF_ext1_tags_15[] = {
	(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ext1_tag2el_15[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* reportUplinkTxDirectCurrent-v1530 */
};
static asn_SEQUENCE_specifics_t asn_SPC_ext1_specs_15 = {
	sizeof(struct CellGroupConfigRrc__ext1),
	offsetof(struct CellGroupConfigRrc__ext1, _asn_ctx),
	asn_MAP_ext1_tag2el_15,
	1,	/* Count of tags in the map */
	asn_MAP_ext1_oms_15,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ext1_15 = {
	"ext1",
	"ext1",
	&asn_OP_SEQUENCE,
	asn_DEF_ext1_tags_15,
	sizeof(asn_DEF_ext1_tags_15)
		/sizeof(asn_DEF_ext1_tags_15[0]) - 1, /* 1 */
	asn_DEF_ext1_tags_15,	/* Same as above */
	sizeof(asn_DEF_ext1_tags_15)
		/sizeof(asn_DEF_ext1_tags_15[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ext1_15,
	1,	/* Elements count */
	&asn_SPC_ext1_specs_15	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_CellGroupConfigRrc_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CellGroupConfigRrc, cellGroupId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CellGroupId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cellGroupId"
		},
	{ ATF_POINTER, 8, offsetof(struct CellGroupConfigRrc, rlc_BearerToAddModList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_rlc_BearerToAddModList_3,
		0,
		{ &asn_OER_memb_rlc_BearerToAddModList_constr_3, &asn_PER_memb_rlc_BearerToAddModList_constr_3,  memb_rlc_BearerToAddModList_constraint_1 },
		0, 0, /* No default value */
		"rlc-BearerToAddModList"
		},
	{ ATF_POINTER, 7, offsetof(struct CellGroupConfigRrc, rlc_BearerToReleaseList),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_rlc_BearerToReleaseList_5,
		0,
		{ &asn_OER_memb_rlc_BearerToReleaseList_constr_5, &asn_PER_memb_rlc_BearerToReleaseList_constr_5,  memb_rlc_BearerToReleaseList_constraint_1 },
		0, 0, /* No default value */
		"rlc-BearerToReleaseList"
		},
	{ ATF_POINTER, 6, offsetof(struct CellGroupConfigRrc, mac_CellGroupConfig),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MAC_CellGroupConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mac-CellGroupConfig"
		},
	{ ATF_POINTER, 5, offsetof(struct CellGroupConfigRrc, physicalCellGroupConfig),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PhysicalCellGroupConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"physicalCellGroupConfig"
		},
	{ ATF_POINTER, 4, offsetof(struct CellGroupConfigRrc, spCellConfig),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SpCellConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spCellConfig"
		},
	{ ATF_POINTER, 3, offsetof(struct CellGroupConfigRrc, sCellToAddModList),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		0,
		&asn_DEF_sCellToAddModList_10,
		0,
		{ &asn_OER_memb_sCellToAddModList_constr_10, &asn_PER_memb_sCellToAddModList_constr_10,  memb_sCellToAddModList_constraint_1 },
		0, 0, /* No default value */
		"sCellToAddModList"
		},
	{ ATF_POINTER, 2, offsetof(struct CellGroupConfigRrc, sCellToReleaseList),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		0,
		&asn_DEF_sCellToReleaseList_12,
		0,
		{ &asn_OER_memb_sCellToReleaseList_constr_12, &asn_PER_memb_sCellToReleaseList_constr_12,  memb_sCellToReleaseList_constraint_1 },
		0, 0, /* No default value */
		"sCellToReleaseList"
		},
	{ ATF_POINTER, 1, offsetof(struct CellGroupConfigRrc, ext1),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		0,
		&asn_DEF_ext1_15,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ext1"
		},
};
static const int asn_MAP_CellGroupConfigRrc_oms_1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static const ber_tlv_tag_t asn_DEF_CellGroupConfigRrc_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CellGroupConfigRrc_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* cellGroupId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* rlc-BearerToAddModList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* rlc-BearerToReleaseList */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* mac-CellGroupConfig */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* physicalCellGroupConfig */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* spCellConfig */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* sCellToAddModList */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* sCellToReleaseList */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 } /* ext1 */
};
static asn_SEQUENCE_specifics_t asn_SPC_CellGroupConfigRrc_specs_1 = {
	sizeof(struct CellGroupConfigRrc),
	offsetof(struct CellGroupConfigRrc, _asn_ctx),
	asn_MAP_CellGroupConfigRrc_tag2el_1,
	9,	/* Count of tags in the map */
	asn_MAP_CellGroupConfigRrc_oms_1,	/* Optional members */
	7, 1,	/* Root/Additions */
	8,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CellGroupConfigRrc = {
	"CellGroupConfigRrc",
	"CellGroupConfigRrc",
	&asn_OP_SEQUENCE,
	asn_DEF_CellGroupConfigRrc_tags_1,
	sizeof(asn_DEF_CellGroupConfigRrc_tags_1)
		/sizeof(asn_DEF_CellGroupConfigRrc_tags_1[0]), /* 1 */
	asn_DEF_CellGroupConfigRrc_tags_1,	/* Same as above */
	sizeof(asn_DEF_CellGroupConfigRrc_tags_1)
		/sizeof(asn_DEF_CellGroupConfigRrc_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_CellGroupConfigRrc_1,
	9,	/* Elements count */
	&asn_SPC_CellGroupConfigRrc_specs_1	/* Additional specs */
};

