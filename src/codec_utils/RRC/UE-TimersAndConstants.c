/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "UE-TimersAndConstants.h"

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
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_t300_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_t300_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_t301_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_t301_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_t310_constr_20 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_t310_constr_20 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  6 }	/* (0..6) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_n310_constr_28 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_n310_constr_28 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_t311_constr_37 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_t311_constr_37 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  6 }	/* (0..6) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_n311_constr_45 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_n311_constr_45 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_t319_constr_54 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_t319_constr_54 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_t300_value2enum_2[] = {
	{ 0,	5,	"ms100" },
	{ 1,	5,	"ms200" },
	{ 2,	5,	"ms300" },
	{ 3,	5,	"ms400" },
	{ 4,	5,	"ms600" },
	{ 5,	6,	"ms1000" },
	{ 6,	6,	"ms1500" },
	{ 7,	6,	"ms2000" }
};
static const unsigned int asn_MAP_t300_enum2value_2[] = {
	0,	/* ms100(0) */
	5,	/* ms1000(5) */
	6,	/* ms1500(6) */
	1,	/* ms200(1) */
	7,	/* ms2000(7) */
	2,	/* ms300(2) */
	3,	/* ms400(3) */
	4	/* ms600(4) */
};
static const asn_INTEGER_specifics_t asn_SPC_t300_specs_2 = {
	asn_MAP_t300_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_t300_enum2value_2,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_t300_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_t300_2 = {
	"t300",
	"t300",
	&asn_OP_NativeEnumerated,
	asn_DEF_t300_tags_2,
	sizeof(asn_DEF_t300_tags_2)
		/sizeof(asn_DEF_t300_tags_2[0]) - 1, /* 1 */
	asn_DEF_t300_tags_2,	/* Same as above */
	sizeof(asn_DEF_t300_tags_2)
		/sizeof(asn_DEF_t300_tags_2[0]), /* 2 */
	{ &asn_OER_type_t300_constr_2, &asn_PER_type_t300_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_t300_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_t301_value2enum_11[] = {
	{ 0,	5,	"ms100" },
	{ 1,	5,	"ms200" },
	{ 2,	5,	"ms300" },
	{ 3,	5,	"ms400" },
	{ 4,	5,	"ms600" },
	{ 5,	6,	"ms1000" },
	{ 6,	6,	"ms1500" },
	{ 7,	6,	"ms2000" }
};
static const unsigned int asn_MAP_t301_enum2value_11[] = {
	0,	/* ms100(0) */
	5,	/* ms1000(5) */
	6,	/* ms1500(6) */
	1,	/* ms200(1) */
	7,	/* ms2000(7) */
	2,	/* ms300(2) */
	3,	/* ms400(3) */
	4	/* ms600(4) */
};
static const asn_INTEGER_specifics_t asn_SPC_t301_specs_11 = {
	asn_MAP_t301_value2enum_11,	/* "tag" => N; sorted by tag */
	asn_MAP_t301_enum2value_11,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_t301_tags_11[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_t301_11 = {
	"t301",
	"t301",
	&asn_OP_NativeEnumerated,
	asn_DEF_t301_tags_11,
	sizeof(asn_DEF_t301_tags_11)
		/sizeof(asn_DEF_t301_tags_11[0]) - 1, /* 1 */
	asn_DEF_t301_tags_11,	/* Same as above */
	sizeof(asn_DEF_t301_tags_11)
		/sizeof(asn_DEF_t301_tags_11[0]), /* 2 */
	{ &asn_OER_type_t301_constr_11, &asn_PER_type_t301_constr_11, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_t301_specs_11	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_t310_value2enum_20[] = {
	{ 0,	3,	"ms0" },
	{ 1,	4,	"ms50" },
	{ 2,	5,	"ms100" },
	{ 3,	5,	"ms200" },
	{ 4,	5,	"ms500" },
	{ 5,	6,	"ms1000" },
	{ 6,	6,	"ms2000" }
};
static const unsigned int asn_MAP_t310_enum2value_20[] = {
	0,	/* ms0(0) */
	2,	/* ms100(2) */
	5,	/* ms1000(5) */
	3,	/* ms200(3) */
	6,	/* ms2000(6) */
	1,	/* ms50(1) */
	4	/* ms500(4) */
};
static const asn_INTEGER_specifics_t asn_SPC_t310_specs_20 = {
	asn_MAP_t310_value2enum_20,	/* "tag" => N; sorted by tag */
	asn_MAP_t310_enum2value_20,	/* N => "tag"; sorted by N */
	7,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_t310_tags_20[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_t310_20 = {
	"t310",
	"t310",
	&asn_OP_NativeEnumerated,
	asn_DEF_t310_tags_20,
	sizeof(asn_DEF_t310_tags_20)
		/sizeof(asn_DEF_t310_tags_20[0]) - 1, /* 1 */
	asn_DEF_t310_tags_20,	/* Same as above */
	sizeof(asn_DEF_t310_tags_20)
		/sizeof(asn_DEF_t310_tags_20[0]), /* 2 */
	{ &asn_OER_type_t310_constr_20, &asn_PER_type_t310_constr_20, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_t310_specs_20	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_n310_value2enum_28[] = {
	{ 0,	2,	"n1" },
	{ 1,	2,	"n2" },
	{ 2,	2,	"n3" },
	{ 3,	2,	"n4" },
	{ 4,	2,	"n6" },
	{ 5,	2,	"n8" },
	{ 6,	3,	"n10" },
	{ 7,	3,	"n20" }
};
static const unsigned int asn_MAP_n310_enum2value_28[] = {
	0,	/* n1(0) */
	6,	/* n10(6) */
	1,	/* n2(1) */
	7,	/* n20(7) */
	2,	/* n3(2) */
	3,	/* n4(3) */
	4,	/* n6(4) */
	5	/* n8(5) */
};
static const asn_INTEGER_specifics_t asn_SPC_n310_specs_28 = {
	asn_MAP_n310_value2enum_28,	/* "tag" => N; sorted by tag */
	asn_MAP_n310_enum2value_28,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_n310_tags_28[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_n310_28 = {
	"n310",
	"n310",
	&asn_OP_NativeEnumerated,
	asn_DEF_n310_tags_28,
	sizeof(asn_DEF_n310_tags_28)
		/sizeof(asn_DEF_n310_tags_28[0]) - 1, /* 1 */
	asn_DEF_n310_tags_28,	/* Same as above */
	sizeof(asn_DEF_n310_tags_28)
		/sizeof(asn_DEF_n310_tags_28[0]), /* 2 */
	{ &asn_OER_type_n310_constr_28, &asn_PER_type_n310_constr_28, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_n310_specs_28	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_t311_value2enum_37[] = {
	{ 0,	6,	"ms1000" },
	{ 1,	6,	"ms3000" },
	{ 2,	6,	"ms5000" },
	{ 3,	7,	"ms10000" },
	{ 4,	7,	"ms15000" },
	{ 5,	7,	"ms20000" },
	{ 6,	7,	"ms30000" }
};
static const unsigned int asn_MAP_t311_enum2value_37[] = {
	0,	/* ms1000(0) */
	3,	/* ms10000(3) */
	4,	/* ms15000(4) */
	5,	/* ms20000(5) */
	1,	/* ms3000(1) */
	6,	/* ms30000(6) */
	2	/* ms5000(2) */
};
static const asn_INTEGER_specifics_t asn_SPC_t311_specs_37 = {
	asn_MAP_t311_value2enum_37,	/* "tag" => N; sorted by tag */
	asn_MAP_t311_enum2value_37,	/* N => "tag"; sorted by N */
	7,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_t311_tags_37[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_t311_37 = {
	"t311",
	"t311",
	&asn_OP_NativeEnumerated,
	asn_DEF_t311_tags_37,
	sizeof(asn_DEF_t311_tags_37)
		/sizeof(asn_DEF_t311_tags_37[0]) - 1, /* 1 */
	asn_DEF_t311_tags_37,	/* Same as above */
	sizeof(asn_DEF_t311_tags_37)
		/sizeof(asn_DEF_t311_tags_37[0]), /* 2 */
	{ &asn_OER_type_t311_constr_37, &asn_PER_type_t311_constr_37, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_t311_specs_37	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_n311_value2enum_45[] = {
	{ 0,	2,	"n1" },
	{ 1,	2,	"n2" },
	{ 2,	2,	"n3" },
	{ 3,	2,	"n4" },
	{ 4,	2,	"n5" },
	{ 5,	2,	"n6" },
	{ 6,	2,	"n8" },
	{ 7,	3,	"n10" }
};
static const unsigned int asn_MAP_n311_enum2value_45[] = {
	0,	/* n1(0) */
	7,	/* n10(7) */
	1,	/* n2(1) */
	2,	/* n3(2) */
	3,	/* n4(3) */
	4,	/* n5(4) */
	5,	/* n6(5) */
	6	/* n8(6) */
};
static const asn_INTEGER_specifics_t asn_SPC_n311_specs_45 = {
	asn_MAP_n311_value2enum_45,	/* "tag" => N; sorted by tag */
	asn_MAP_n311_enum2value_45,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_n311_tags_45[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_n311_45 = {
	"n311",
	"n311",
	&asn_OP_NativeEnumerated,
	asn_DEF_n311_tags_45,
	sizeof(asn_DEF_n311_tags_45)
		/sizeof(asn_DEF_n311_tags_45[0]) - 1, /* 1 */
	asn_DEF_n311_tags_45,	/* Same as above */
	sizeof(asn_DEF_n311_tags_45)
		/sizeof(asn_DEF_n311_tags_45[0]), /* 2 */
	{ &asn_OER_type_n311_constr_45, &asn_PER_type_n311_constr_45, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_n311_specs_45	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_t319_value2enum_54[] = {
	{ 0,	5,	"ms100" },
	{ 1,	5,	"ms200" },
	{ 2,	5,	"ms300" },
	{ 3,	5,	"ms400" },
	{ 4,	5,	"ms600" },
	{ 5,	6,	"ms1000" },
	{ 6,	6,	"ms1500" },
	{ 7,	6,	"ms2000" }
};
static const unsigned int asn_MAP_t319_enum2value_54[] = {
	0,	/* ms100(0) */
	5,	/* ms1000(5) */
	6,	/* ms1500(6) */
	1,	/* ms200(1) */
	7,	/* ms2000(7) */
	2,	/* ms300(2) */
	3,	/* ms400(3) */
	4	/* ms600(4) */
};
static const asn_INTEGER_specifics_t asn_SPC_t319_specs_54 = {
	asn_MAP_t319_value2enum_54,	/* "tag" => N; sorted by tag */
	asn_MAP_t319_enum2value_54,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_t319_tags_54[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_t319_54 = {
	"t319",
	"t319",
	&asn_OP_NativeEnumerated,
	asn_DEF_t319_tags_54,
	sizeof(asn_DEF_t319_tags_54)
		/sizeof(asn_DEF_t319_tags_54[0]) - 1, /* 1 */
	asn_DEF_t319_tags_54,	/* Same as above */
	sizeof(asn_DEF_t319_tags_54)
		/sizeof(asn_DEF_t319_tags_54[0]), /* 2 */
	{ &asn_OER_type_t319_constr_54, &asn_PER_type_t319_constr_54, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_t319_specs_54	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_UE_TimersAndConstants_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UE_TimersAndConstants, t300),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_t300_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t300"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UE_TimersAndConstants, t301),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_t301_11,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t301"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UE_TimersAndConstants, t310),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_t310_20,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t310"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UE_TimersAndConstants, n310),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_n310_28,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"n310"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UE_TimersAndConstants, t311),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_t311_37,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t311"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UE_TimersAndConstants, n311),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_n311_45,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"n311"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UE_TimersAndConstants, t319),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_t319_54,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t319"
		},
};
static const ber_tlv_tag_t asn_DEF_UE_TimersAndConstants_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UE_TimersAndConstants_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* t300 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* t301 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* t310 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* n310 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* t311 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* n311 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* t319 */
};
asn_SEQUENCE_specifics_t asn_SPC_UE_TimersAndConstants_specs_1 = {
	sizeof(struct UE_TimersAndConstants),
	offsetof(struct UE_TimersAndConstants, _asn_ctx),
	asn_MAP_UE_TimersAndConstants_tag2el_1,
	7,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	7,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UE_TimersAndConstants = {
	"UE-TimersAndConstants",
	"UE-TimersAndConstants",
	&asn_OP_SEQUENCE,
	asn_DEF_UE_TimersAndConstants_tags_1,
	sizeof(asn_DEF_UE_TimersAndConstants_tags_1)
		/sizeof(asn_DEF_UE_TimersAndConstants_tags_1[0]), /* 1 */
	asn_DEF_UE_TimersAndConstants_tags_1,	/* Same as above */
	sizeof(asn_DEF_UE_TimersAndConstants_tags_1)
		/sizeof(asn_DEF_UE_TimersAndConstants_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UE_TimersAndConstants_1,
	7,	/* Elements count */
	&asn_SPC_UE_TimersAndConstants_specs_1	/* Additional specs */
};

