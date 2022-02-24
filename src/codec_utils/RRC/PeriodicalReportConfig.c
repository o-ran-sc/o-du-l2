/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PeriodicalReportConfig.h"

#include "MeasReportQuantity.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_maxReportCells_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 8)) {
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
memb_maxNrofRS_IndexesToReport_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 32)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_reportAmount_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_reportAmount_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_maxReportCells_constr_14 CC_NOTUSED = {
	{ 1, 1 }	/* (1..8) */,
	-1};
static asn_per_constraints_t asn_PER_memb_maxReportCells_constr_14 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  1,  8 }	/* (1..8) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_maxNrofRS_IndexesToReport_constr_16 CC_NOTUSED = {
	{ 1, 1 }	/* (1..32) */,
	-1};
static asn_per_constraints_t asn_PER_memb_maxNrofRS_IndexesToReport_constr_16 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5,  1,  32 }	/* (1..32) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_reportAmount_value2enum_4[] = {
	{ 0,	2,	"r1" },
	{ 1,	2,	"r2" },
	{ 2,	2,	"r4" },
	{ 3,	2,	"r8" },
	{ 4,	3,	"r16" },
	{ 5,	3,	"r32" },
	{ 6,	3,	"r64" },
	{ 7,	8,	"infinity" }
};
static const unsigned int asn_MAP_reportAmount_enum2value_4[] = {
	7,	/* infinity(7) */
	0,	/* r1(0) */
	4,	/* r16(4) */
	1,	/* r2(1) */
	5,	/* r32(5) */
	2,	/* r4(2) */
	6,	/* r64(6) */
	3	/* r8(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_reportAmount_specs_4 = {
	asn_MAP_reportAmount_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_reportAmount_enum2value_4,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_reportAmount_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_reportAmount_4 = {
	"reportAmount",
	"reportAmount",
	&asn_OP_NativeEnumerated,
	asn_DEF_reportAmount_tags_4,
	sizeof(asn_DEF_reportAmount_tags_4)
		/sizeof(asn_DEF_reportAmount_tags_4[0]) - 1, /* 1 */
	asn_DEF_reportAmount_tags_4,	/* Same as above */
	sizeof(asn_DEF_reportAmount_tags_4)
		/sizeof(asn_DEF_reportAmount_tags_4[0]), /* 2 */
	{ &asn_OER_type_reportAmount_constr_4, &asn_PER_type_reportAmount_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_reportAmount_specs_4	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_PeriodicalReportConfig_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PeriodicalReportConfig, rsType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NR_RS_Type,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rsType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PeriodicalReportConfig, reportInterval),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReportInterval,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportInterval"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PeriodicalReportConfig, reportAmount),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_reportAmount_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportAmount"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PeriodicalReportConfig, reportQuantityCell),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasReportQuantity,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportQuantityCell"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PeriodicalReportConfig, maxReportCells),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_maxReportCells_constr_14, &asn_PER_memb_maxReportCells_constr_14,  memb_maxReportCells_constraint_1 },
		0, 0, /* No default value */
		"maxReportCells"
		},
	{ ATF_POINTER, 2, offsetof(struct PeriodicalReportConfig, reportQuantityRS_Indexes),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasReportQuantity,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reportQuantityRS-Indexes"
		},
	{ ATF_POINTER, 1, offsetof(struct PeriodicalReportConfig, maxNrofRS_IndexesToReport),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_maxNrofRS_IndexesToReport_constr_16, &asn_PER_memb_maxNrofRS_IndexesToReport_constr_16,  memb_maxNrofRS_IndexesToReport_constraint_1 },
		0, 0, /* No default value */
		"maxNrofRS-IndexesToReport"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PeriodicalReportConfig, includeBeamMeasurements),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"includeBeamMeasurements"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PeriodicalReportConfig, useWhiteCellList),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"useWhiteCellList"
		},
};
static const int asn_MAP_PeriodicalReportConfig_oms_1[] = { 5, 6 };
static const ber_tlv_tag_t asn_DEF_PeriodicalReportConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PeriodicalReportConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* rsType */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* reportInterval */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* reportAmount */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* reportQuantityCell */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* maxReportCells */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* reportQuantityRS-Indexes */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* maxNrofRS-IndexesToReport */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* includeBeamMeasurements */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 } /* useWhiteCellList */
};
asn_SEQUENCE_specifics_t asn_SPC_PeriodicalReportConfig_specs_1 = {
	sizeof(struct PeriodicalReportConfig),
	offsetof(struct PeriodicalReportConfig, _asn_ctx),
	asn_MAP_PeriodicalReportConfig_tag2el_1,
	9,	/* Count of tags in the map */
	asn_MAP_PeriodicalReportConfig_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	9,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PeriodicalReportConfig = {
	"PeriodicalReportConfig",
	"PeriodicalReportConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_PeriodicalReportConfig_tags_1,
	sizeof(asn_DEF_PeriodicalReportConfig_tags_1)
		/sizeof(asn_DEF_PeriodicalReportConfig_tags_1[0]), /* 1 */
	asn_DEF_PeriodicalReportConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_PeriodicalReportConfig_tags_1)
		/sizeof(asn_DEF_PeriodicalReportConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PeriodicalReportConfig_1,
	9,	/* Elements count */
	&asn_SPC_PeriodicalReportConfig_specs_1	/* Additional specs */
};

