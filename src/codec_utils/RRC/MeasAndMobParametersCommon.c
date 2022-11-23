/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "MeasAndMobParametersCommon.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_supportedGapPattern_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 22)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_ssb_RLM_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_ssb_RLM_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_ssb_AndCSI_RS_RLM_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_ssb_AndCSI_RS_RLM_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_eventB_MeasAndReport_constr_9 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_eventB_MeasAndReport_constr_9 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_handoverFDD_TDD_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_handoverFDD_TDD_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_eutra_CGI_Reporting_constr_13 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_eutra_CGI_Reporting_constr_13 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_nr_CGI_Reporting_constr_15 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_nr_CGI_Reporting_constr_15 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_supportedGapPattern_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	22	/* (SIZE(22..22)) */};
static asn_per_constraints_t asn_PER_memb_supportedGapPattern_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  22,  22 }	/* (SIZE(22..22)) */,
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_ssb_RLM_value2enum_3[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_ssb_RLM_enum2value_3[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_ssb_RLM_specs_3 = {
	asn_MAP_ssb_RLM_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_ssb_RLM_enum2value_3,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_ssb_RLM_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ssb_RLM_3 = {
	"ssb-RLM",
	"ssb-RLM",
	&asn_OP_NativeEnumerated,
	asn_DEF_ssb_RLM_tags_3,
	sizeof(asn_DEF_ssb_RLM_tags_3)
		/sizeof(asn_DEF_ssb_RLM_tags_3[0]) - 1, /* 1 */
	asn_DEF_ssb_RLM_tags_3,	/* Same as above */
	sizeof(asn_DEF_ssb_RLM_tags_3)
		/sizeof(asn_DEF_ssb_RLM_tags_3[0]), /* 2 */
	{ &asn_OER_type_ssb_RLM_constr_3, &asn_PER_type_ssb_RLM_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_ssb_RLM_specs_3	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_ssb_AndCSI_RS_RLM_value2enum_5[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_ssb_AndCSI_RS_RLM_enum2value_5[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_ssb_AndCSI_RS_RLM_specs_5 = {
	asn_MAP_ssb_AndCSI_RS_RLM_value2enum_5,	/* "tag" => N; sorted by tag */
	asn_MAP_ssb_AndCSI_RS_RLM_enum2value_5,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_ssb_AndCSI_RS_RLM_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ssb_AndCSI_RS_RLM_5 = {
	"ssb-AndCSI-RS-RLM",
	"ssb-AndCSI-RS-RLM",
	&asn_OP_NativeEnumerated,
	asn_DEF_ssb_AndCSI_RS_RLM_tags_5,
	sizeof(asn_DEF_ssb_AndCSI_RS_RLM_tags_5)
		/sizeof(asn_DEF_ssb_AndCSI_RS_RLM_tags_5[0]) - 1, /* 1 */
	asn_DEF_ssb_AndCSI_RS_RLM_tags_5,	/* Same as above */
	sizeof(asn_DEF_ssb_AndCSI_RS_RLM_tags_5)
		/sizeof(asn_DEF_ssb_AndCSI_RS_RLM_tags_5[0]), /* 2 */
	{ &asn_OER_type_ssb_AndCSI_RS_RLM_constr_5, &asn_PER_type_ssb_AndCSI_RS_RLM_constr_5, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_ssb_AndCSI_RS_RLM_specs_5	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_eventB_MeasAndReport_value2enum_9[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_eventB_MeasAndReport_enum2value_9[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_eventB_MeasAndReport_specs_9 = {
	asn_MAP_eventB_MeasAndReport_value2enum_9,	/* "tag" => N; sorted by tag */
	asn_MAP_eventB_MeasAndReport_enum2value_9,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_eventB_MeasAndReport_tags_9[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_eventB_MeasAndReport_9 = {
	"eventB-MeasAndReport",
	"eventB-MeasAndReport",
	&asn_OP_NativeEnumerated,
	asn_DEF_eventB_MeasAndReport_tags_9,
	sizeof(asn_DEF_eventB_MeasAndReport_tags_9)
		/sizeof(asn_DEF_eventB_MeasAndReport_tags_9[0]) - 1, /* 1 */
	asn_DEF_eventB_MeasAndReport_tags_9,	/* Same as above */
	sizeof(asn_DEF_eventB_MeasAndReport_tags_9)
		/sizeof(asn_DEF_eventB_MeasAndReport_tags_9[0]), /* 2 */
	{ &asn_OER_type_eventB_MeasAndReport_constr_9, &asn_PER_type_eventB_MeasAndReport_constr_9, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_eventB_MeasAndReport_specs_9	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_handoverFDD_TDD_value2enum_11[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_handoverFDD_TDD_enum2value_11[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_handoverFDD_TDD_specs_11 = {
	asn_MAP_handoverFDD_TDD_value2enum_11,	/* "tag" => N; sorted by tag */
	asn_MAP_handoverFDD_TDD_enum2value_11,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_handoverFDD_TDD_tags_11[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_handoverFDD_TDD_11 = {
	"handoverFDD-TDD",
	"handoverFDD-TDD",
	&asn_OP_NativeEnumerated,
	asn_DEF_handoverFDD_TDD_tags_11,
	sizeof(asn_DEF_handoverFDD_TDD_tags_11)
		/sizeof(asn_DEF_handoverFDD_TDD_tags_11[0]) - 1, /* 1 */
	asn_DEF_handoverFDD_TDD_tags_11,	/* Same as above */
	sizeof(asn_DEF_handoverFDD_TDD_tags_11)
		/sizeof(asn_DEF_handoverFDD_TDD_tags_11[0]), /* 2 */
	{ &asn_OER_type_handoverFDD_TDD_constr_11, &asn_PER_type_handoverFDD_TDD_constr_11, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_handoverFDD_TDD_specs_11	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_eutra_CGI_Reporting_value2enum_13[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_eutra_CGI_Reporting_enum2value_13[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_eutra_CGI_Reporting_specs_13 = {
	asn_MAP_eutra_CGI_Reporting_value2enum_13,	/* "tag" => N; sorted by tag */
	asn_MAP_eutra_CGI_Reporting_enum2value_13,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_eutra_CGI_Reporting_tags_13[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_eutra_CGI_Reporting_13 = {
	"eutra-CGI-Reporting",
	"eutra-CGI-Reporting",
	&asn_OP_NativeEnumerated,
	asn_DEF_eutra_CGI_Reporting_tags_13,
	sizeof(asn_DEF_eutra_CGI_Reporting_tags_13)
		/sizeof(asn_DEF_eutra_CGI_Reporting_tags_13[0]) - 1, /* 1 */
	asn_DEF_eutra_CGI_Reporting_tags_13,	/* Same as above */
	sizeof(asn_DEF_eutra_CGI_Reporting_tags_13)
		/sizeof(asn_DEF_eutra_CGI_Reporting_tags_13[0]), /* 2 */
	{ &asn_OER_type_eutra_CGI_Reporting_constr_13, &asn_PER_type_eutra_CGI_Reporting_constr_13, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_eutra_CGI_Reporting_specs_13	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_nr_CGI_Reporting_value2enum_15[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_nr_CGI_Reporting_enum2value_15[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_nr_CGI_Reporting_specs_15 = {
	asn_MAP_nr_CGI_Reporting_value2enum_15,	/* "tag" => N; sorted by tag */
	asn_MAP_nr_CGI_Reporting_enum2value_15,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_nr_CGI_Reporting_tags_15[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_nr_CGI_Reporting_15 = {
	"nr-CGI-Reporting",
	"nr-CGI-Reporting",
	&asn_OP_NativeEnumerated,
	asn_DEF_nr_CGI_Reporting_tags_15,
	sizeof(asn_DEF_nr_CGI_Reporting_tags_15)
		/sizeof(asn_DEF_nr_CGI_Reporting_tags_15[0]) - 1, /* 1 */
	asn_DEF_nr_CGI_Reporting_tags_15,	/* Same as above */
	sizeof(asn_DEF_nr_CGI_Reporting_tags_15)
		/sizeof(asn_DEF_nr_CGI_Reporting_tags_15[0]), /* 2 */
	{ &asn_OER_type_nr_CGI_Reporting_constr_15, &asn_PER_type_nr_CGI_Reporting_constr_15, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_nr_CGI_Reporting_specs_15	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ext1_8[] = {
	{ ATF_POINTER, 4, offsetof(struct MeasAndMobParametersCommon__ext1, eventB_MeasAndReport),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_eventB_MeasAndReport_9,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eventB-MeasAndReport"
		},
	{ ATF_POINTER, 3, offsetof(struct MeasAndMobParametersCommon__ext1, handoverFDD_TDD),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_handoverFDD_TDD_11,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"handoverFDD-TDD"
		},
	{ ATF_POINTER, 2, offsetof(struct MeasAndMobParametersCommon__ext1, eutra_CGI_Reporting),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_eutra_CGI_Reporting_13,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eutra-CGI-Reporting"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasAndMobParametersCommon__ext1, nr_CGI_Reporting),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_nr_CGI_Reporting_15,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nr-CGI-Reporting"
		},
};
static const int asn_MAP_ext1_oms_8[] = { 0, 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_ext1_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ext1_tag2el_8[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* eventB-MeasAndReport */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* handoverFDD-TDD */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* eutra-CGI-Reporting */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* nr-CGI-Reporting */
};
static asn_SEQUENCE_specifics_t asn_SPC_ext1_specs_8 = {
	sizeof(struct MeasAndMobParametersCommon__ext1),
	offsetof(struct MeasAndMobParametersCommon__ext1, _asn_ctx),
	asn_MAP_ext1_tag2el_8,
	4,	/* Count of tags in the map */
	asn_MAP_ext1_oms_8,	/* Optional members */
	4, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ext1_8 = {
	"ext1",
	"ext1",
	&asn_OP_SEQUENCE,
	asn_DEF_ext1_tags_8,
	sizeof(asn_DEF_ext1_tags_8)
		/sizeof(asn_DEF_ext1_tags_8[0]) - 1, /* 1 */
	asn_DEF_ext1_tags_8,	/* Same as above */
	sizeof(asn_DEF_ext1_tags_8)
		/sizeof(asn_DEF_ext1_tags_8[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ext1_8,
	4,	/* Elements count */
	&asn_SPC_ext1_specs_8	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_MeasAndMobParametersCommon_1[] = {
	{ ATF_POINTER, 4, offsetof(struct MeasAndMobParametersCommon, supportedGapPattern),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_supportedGapPattern_constr_2, &asn_PER_memb_supportedGapPattern_constr_2,  memb_supportedGapPattern_constraint_1 },
		0, 0, /* No default value */
		"supportedGapPattern"
		},
	{ ATF_POINTER, 3, offsetof(struct MeasAndMobParametersCommon, ssb_RLM),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ssb_RLM_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-RLM"
		},
	{ ATF_POINTER, 2, offsetof(struct MeasAndMobParametersCommon, ssb_AndCSI_RS_RLM),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ssb_AndCSI_RS_RLM_5,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-AndCSI-RS-RLM"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasAndMobParametersCommon, ext1),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_ext1_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ext1"
		},
};
static const int asn_MAP_MeasAndMobParametersCommon_oms_1[] = { 0, 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_MeasAndMobParametersCommon_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MeasAndMobParametersCommon_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* supportedGapPattern */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ssb-RLM */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* ssb-AndCSI-RS-RLM */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* ext1 */
};
asn_SEQUENCE_specifics_t asn_SPC_MeasAndMobParametersCommon_specs_1 = {
	sizeof(struct MeasAndMobParametersCommon),
	offsetof(struct MeasAndMobParametersCommon, _asn_ctx),
	asn_MAP_MeasAndMobParametersCommon_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_MeasAndMobParametersCommon_oms_1,	/* Optional members */
	3, 1,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MeasAndMobParametersCommon = {
	"MeasAndMobParametersCommon",
	"MeasAndMobParametersCommon",
	&asn_OP_SEQUENCE,
	asn_DEF_MeasAndMobParametersCommon_tags_1,
	sizeof(asn_DEF_MeasAndMobParametersCommon_tags_1)
		/sizeof(asn_DEF_MeasAndMobParametersCommon_tags_1[0]), /* 1 */
	asn_DEF_MeasAndMobParametersCommon_tags_1,	/* Same as above */
	sizeof(asn_DEF_MeasAndMobParametersCommon_tags_1)
		/sizeof(asn_DEF_MeasAndMobParametersCommon_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_MeasAndMobParametersCommon_1,
	4,	/* Elements count */
	&asn_SPC_MeasAndMobParametersCommon_specs_1	/* Additional specs */
};

