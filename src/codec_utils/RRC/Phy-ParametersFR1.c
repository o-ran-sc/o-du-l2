/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "Phy-ParametersFR1.h"

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
static asn_oer_constraints_t asn_OER_type_pdcchMonitoringSingleOccasion_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_pdcchMonitoringSingleOccasion_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_scs_60kHz_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_scs_60kHz_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_pdsch_256QAM_FR1_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_pdsch_256QAM_FR1_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_pdsch_RE_MappingFR1_PerSymbol_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_pdsch_RE_MappingFR1_PerSymbol_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_pdcchMonitoringSingleOccasion_value2enum_2[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_pdcchMonitoringSingleOccasion_enum2value_2[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_pdcchMonitoringSingleOccasion_specs_2 = {
	asn_MAP_pdcchMonitoringSingleOccasion_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_pdcchMonitoringSingleOccasion_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_pdcchMonitoringSingleOccasion_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pdcchMonitoringSingleOccasion_2 = {
	"pdcchMonitoringSingleOccasion",
	"pdcchMonitoringSingleOccasion",
	&asn_OP_NativeEnumerated,
	asn_DEF_pdcchMonitoringSingleOccasion_tags_2,
	sizeof(asn_DEF_pdcchMonitoringSingleOccasion_tags_2)
		/sizeof(asn_DEF_pdcchMonitoringSingleOccasion_tags_2[0]) - 1, /* 1 */
	asn_DEF_pdcchMonitoringSingleOccasion_tags_2,	/* Same as above */
	sizeof(asn_DEF_pdcchMonitoringSingleOccasion_tags_2)
		/sizeof(asn_DEF_pdcchMonitoringSingleOccasion_tags_2[0]), /* 2 */
	{ &asn_OER_type_pdcchMonitoringSingleOccasion_constr_2, &asn_PER_type_pdcchMonitoringSingleOccasion_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_pdcchMonitoringSingleOccasion_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_scs_60kHz_value2enum_4[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_scs_60kHz_enum2value_4[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_scs_60kHz_specs_4 = {
	asn_MAP_scs_60kHz_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_scs_60kHz_enum2value_4,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_scs_60kHz_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_scs_60kHz_4 = {
	"scs-60kHz",
	"scs-60kHz",
	&asn_OP_NativeEnumerated,
	asn_DEF_scs_60kHz_tags_4,
	sizeof(asn_DEF_scs_60kHz_tags_4)
		/sizeof(asn_DEF_scs_60kHz_tags_4[0]) - 1, /* 1 */
	asn_DEF_scs_60kHz_tags_4,	/* Same as above */
	sizeof(asn_DEF_scs_60kHz_tags_4)
		/sizeof(asn_DEF_scs_60kHz_tags_4[0]), /* 2 */
	{ &asn_OER_type_scs_60kHz_constr_4, &asn_PER_type_scs_60kHz_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_scs_60kHz_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_pdsch_256QAM_FR1_value2enum_6[] = {
	{ 0,	9,	"supported" }
};
static const unsigned int asn_MAP_pdsch_256QAM_FR1_enum2value_6[] = {
	0	/* supported(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_pdsch_256QAM_FR1_specs_6 = {
	asn_MAP_pdsch_256QAM_FR1_value2enum_6,	/* "tag" => N; sorted by tag */
	asn_MAP_pdsch_256QAM_FR1_enum2value_6,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_pdsch_256QAM_FR1_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pdsch_256QAM_FR1_6 = {
	"pdsch-256QAM-FR1",
	"pdsch-256QAM-FR1",
	&asn_OP_NativeEnumerated,
	asn_DEF_pdsch_256QAM_FR1_tags_6,
	sizeof(asn_DEF_pdsch_256QAM_FR1_tags_6)
		/sizeof(asn_DEF_pdsch_256QAM_FR1_tags_6[0]) - 1, /* 1 */
	asn_DEF_pdsch_256QAM_FR1_tags_6,	/* Same as above */
	sizeof(asn_DEF_pdsch_256QAM_FR1_tags_6)
		/sizeof(asn_DEF_pdsch_256QAM_FR1_tags_6[0]), /* 2 */
	{ &asn_OER_type_pdsch_256QAM_FR1_constr_6, &asn_PER_type_pdsch_256QAM_FR1_constr_6, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_pdsch_256QAM_FR1_specs_6	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_pdsch_RE_MappingFR1_PerSymbol_value2enum_8[] = {
	{ 0,	3,	"n10" },
	{ 1,	3,	"n20" }
};
static const unsigned int asn_MAP_pdsch_RE_MappingFR1_PerSymbol_enum2value_8[] = {
	0,	/* n10(0) */
	1	/* n20(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_pdsch_RE_MappingFR1_PerSymbol_specs_8 = {
	asn_MAP_pdsch_RE_MappingFR1_PerSymbol_value2enum_8,	/* "tag" => N; sorted by tag */
	asn_MAP_pdsch_RE_MappingFR1_PerSymbol_enum2value_8,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_pdsch_RE_MappingFR1_PerSymbol_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pdsch_RE_MappingFR1_PerSymbol_8 = {
	"pdsch-RE-MappingFR1-PerSymbol",
	"pdsch-RE-MappingFR1-PerSymbol",
	&asn_OP_NativeEnumerated,
	asn_DEF_pdsch_RE_MappingFR1_PerSymbol_tags_8,
	sizeof(asn_DEF_pdsch_RE_MappingFR1_PerSymbol_tags_8)
		/sizeof(asn_DEF_pdsch_RE_MappingFR1_PerSymbol_tags_8[0]) - 1, /* 1 */
	asn_DEF_pdsch_RE_MappingFR1_PerSymbol_tags_8,	/* Same as above */
	sizeof(asn_DEF_pdsch_RE_MappingFR1_PerSymbol_tags_8)
		/sizeof(asn_DEF_pdsch_RE_MappingFR1_PerSymbol_tags_8[0]), /* 2 */
	{ &asn_OER_type_pdsch_RE_MappingFR1_PerSymbol_constr_8, &asn_PER_type_pdsch_RE_MappingFR1_PerSymbol_constr_8, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_pdsch_RE_MappingFR1_PerSymbol_specs_8	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_Phy_ParametersFR1_1[] = {
	{ ATF_POINTER, 4, offsetof(struct Phy_ParametersFR1, pdcchMonitoringSingleOccasion),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_pdcchMonitoringSingleOccasion_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pdcchMonitoringSingleOccasion"
		},
	{ ATF_POINTER, 3, offsetof(struct Phy_ParametersFR1, scs_60kHz),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_scs_60kHz_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"scs-60kHz"
		},
	{ ATF_POINTER, 2, offsetof(struct Phy_ParametersFR1, pdsch_256QAM_FR1),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_pdsch_256QAM_FR1_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pdsch-256QAM-FR1"
		},
	{ ATF_POINTER, 1, offsetof(struct Phy_ParametersFR1, pdsch_RE_MappingFR1_PerSymbol),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_pdsch_RE_MappingFR1_PerSymbol_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pdsch-RE-MappingFR1-PerSymbol"
		},
};
static const int asn_MAP_Phy_ParametersFR1_oms_1[] = { 0, 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_Phy_ParametersFR1_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Phy_ParametersFR1_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pdcchMonitoringSingleOccasion */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* scs-60kHz */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* pdsch-256QAM-FR1 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* pdsch-RE-MappingFR1-PerSymbol */
};
asn_SEQUENCE_specifics_t asn_SPC_Phy_ParametersFR1_specs_1 = {
	sizeof(struct Phy_ParametersFR1),
	offsetof(struct Phy_ParametersFR1, _asn_ctx),
	asn_MAP_Phy_ParametersFR1_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_Phy_ParametersFR1_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Phy_ParametersFR1 = {
	"Phy-ParametersFR1",
	"Phy-ParametersFR1",
	&asn_OP_SEQUENCE,
	asn_DEF_Phy_ParametersFR1_tags_1,
	sizeof(asn_DEF_Phy_ParametersFR1_tags_1)
		/sizeof(asn_DEF_Phy_ParametersFR1_tags_1[0]), /* 1 */
	asn_DEF_Phy_ParametersFR1_tags_1,	/* Same as above */
	sizeof(asn_DEF_Phy_ParametersFR1_tags_1)
		/sizeof(asn_DEF_Phy_ParametersFR1_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Phy_ParametersFR1_1,
	4,	/* Elements count */
	&asn_SPC_Phy_ParametersFR1_specs_1	/* Additional specs */
};

