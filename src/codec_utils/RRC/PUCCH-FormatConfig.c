/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "PUCCH-FormatConfig.h"

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
static asn_oer_constraints_t asn_OER_type_interslotFrequencyHopping_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_interslotFrequencyHopping_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_additionalDMRS_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_additionalDMRS_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_nrofSlots_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_nrofSlots_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_pi2BPSK_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_pi2BPSK_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_simultaneousHARQ_ACK_CSI_constr_13 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_simultaneousHARQ_ACK_CSI_constr_13 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_interslotFrequencyHopping_value2enum_2[] = {
	{ 0,	7,	"enabled" }
};
static const unsigned int asn_MAP_interslotFrequencyHopping_enum2value_2[] = {
	0	/* enabled(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_interslotFrequencyHopping_specs_2 = {
	asn_MAP_interslotFrequencyHopping_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_interslotFrequencyHopping_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_interslotFrequencyHopping_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_interslotFrequencyHopping_2 = {
	"interslotFrequencyHopping",
	"interslotFrequencyHopping",
	&asn_OP_NativeEnumerated,
	asn_DEF_interslotFrequencyHopping_tags_2,
	sizeof(asn_DEF_interslotFrequencyHopping_tags_2)
		/sizeof(asn_DEF_interslotFrequencyHopping_tags_2[0]) - 1, /* 1 */
	asn_DEF_interslotFrequencyHopping_tags_2,	/* Same as above */
	sizeof(asn_DEF_interslotFrequencyHopping_tags_2)
		/sizeof(asn_DEF_interslotFrequencyHopping_tags_2[0]), /* 2 */
	{ &asn_OER_type_interslotFrequencyHopping_constr_2, &asn_PER_type_interslotFrequencyHopping_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_interslotFrequencyHopping_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_additionalDMRS_value2enum_4[] = {
	{ 0,	4,	"true" }
};
static const unsigned int asn_MAP_additionalDMRS_enum2value_4[] = {
	0	/* true(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_additionalDMRS_specs_4 = {
	asn_MAP_additionalDMRS_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_additionalDMRS_enum2value_4,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_additionalDMRS_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_additionalDMRS_4 = {
	"additionalDMRS",
	"additionalDMRS",
	&asn_OP_NativeEnumerated,
	asn_DEF_additionalDMRS_tags_4,
	sizeof(asn_DEF_additionalDMRS_tags_4)
		/sizeof(asn_DEF_additionalDMRS_tags_4[0]) - 1, /* 1 */
	asn_DEF_additionalDMRS_tags_4,	/* Same as above */
	sizeof(asn_DEF_additionalDMRS_tags_4)
		/sizeof(asn_DEF_additionalDMRS_tags_4[0]), /* 2 */
	{ &asn_OER_type_additionalDMRS_constr_4, &asn_PER_type_additionalDMRS_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_additionalDMRS_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_nrofSlots_value2enum_7[] = {
	{ 0,	2,	"n2" },
	{ 1,	2,	"n4" },
	{ 2,	2,	"n8" }
};
static const unsigned int asn_MAP_nrofSlots_enum2value_7[] = {
	0,	/* n2(0) */
	1,	/* n4(1) */
	2	/* n8(2) */
};
static const asn_INTEGER_specifics_t asn_SPC_nrofSlots_specs_7 = {
	asn_MAP_nrofSlots_value2enum_7,	/* "tag" => N; sorted by tag */
	asn_MAP_nrofSlots_enum2value_7,	/* N => "tag"; sorted by N */
	3,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_nrofSlots_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_nrofSlots_7 = {
	"nrofSlots",
	"nrofSlots",
	&asn_OP_NativeEnumerated,
	asn_DEF_nrofSlots_tags_7,
	sizeof(asn_DEF_nrofSlots_tags_7)
		/sizeof(asn_DEF_nrofSlots_tags_7[0]) - 1, /* 1 */
	asn_DEF_nrofSlots_tags_7,	/* Same as above */
	sizeof(asn_DEF_nrofSlots_tags_7)
		/sizeof(asn_DEF_nrofSlots_tags_7[0]), /* 2 */
	{ &asn_OER_type_nrofSlots_constr_7, &asn_PER_type_nrofSlots_constr_7, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_nrofSlots_specs_7	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_pi2BPSK_value2enum_11[] = {
	{ 0,	7,	"enabled" }
};
static const unsigned int asn_MAP_pi2BPSK_enum2value_11[] = {
	0	/* enabled(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_pi2BPSK_specs_11 = {
	asn_MAP_pi2BPSK_value2enum_11,	/* "tag" => N; sorted by tag */
	asn_MAP_pi2BPSK_enum2value_11,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_pi2BPSK_tags_11[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pi2BPSK_11 = {
	"pi2BPSK",
	"pi2BPSK",
	&asn_OP_NativeEnumerated,
	asn_DEF_pi2BPSK_tags_11,
	sizeof(asn_DEF_pi2BPSK_tags_11)
		/sizeof(asn_DEF_pi2BPSK_tags_11[0]) - 1, /* 1 */
	asn_DEF_pi2BPSK_tags_11,	/* Same as above */
	sizeof(asn_DEF_pi2BPSK_tags_11)
		/sizeof(asn_DEF_pi2BPSK_tags_11[0]), /* 2 */
	{ &asn_OER_type_pi2BPSK_constr_11, &asn_PER_type_pi2BPSK_constr_11, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_pi2BPSK_specs_11	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_simultaneousHARQ_ACK_CSI_value2enum_13[] = {
	{ 0,	4,	"true" }
};
static const unsigned int asn_MAP_simultaneousHARQ_ACK_CSI_enum2value_13[] = {
	0	/* true(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_simultaneousHARQ_ACK_CSI_specs_13 = {
	asn_MAP_simultaneousHARQ_ACK_CSI_value2enum_13,	/* "tag" => N; sorted by tag */
	asn_MAP_simultaneousHARQ_ACK_CSI_enum2value_13,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_simultaneousHARQ_ACK_CSI_tags_13[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_simultaneousHARQ_ACK_CSI_13 = {
	"simultaneousHARQ-ACK-CSI",
	"simultaneousHARQ-ACK-CSI",
	&asn_OP_NativeEnumerated,
	asn_DEF_simultaneousHARQ_ACK_CSI_tags_13,
	sizeof(asn_DEF_simultaneousHARQ_ACK_CSI_tags_13)
		/sizeof(asn_DEF_simultaneousHARQ_ACK_CSI_tags_13[0]) - 1, /* 1 */
	asn_DEF_simultaneousHARQ_ACK_CSI_tags_13,	/* Same as above */
	sizeof(asn_DEF_simultaneousHARQ_ACK_CSI_tags_13)
		/sizeof(asn_DEF_simultaneousHARQ_ACK_CSI_tags_13[0]), /* 2 */
	{ &asn_OER_type_simultaneousHARQ_ACK_CSI_constr_13, &asn_PER_type_simultaneousHARQ_ACK_CSI_constr_13, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_simultaneousHARQ_ACK_CSI_specs_13	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_PUCCH_FormatConfig_1[] = {
	{ ATF_POINTER, 6, offsetof(struct PUCCH_FormatConfig, interslotFrequencyHopping),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_interslotFrequencyHopping_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"interslotFrequencyHopping"
		},
	{ ATF_POINTER, 5, offsetof(struct PUCCH_FormatConfig, additionalDMRS),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_additionalDMRS_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"additionalDMRS"
		},
	{ ATF_POINTER, 4, offsetof(struct PUCCH_FormatConfig, maxCodeRate),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_MaxCodeRate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxCodeRate"
		},
	{ ATF_POINTER, 3, offsetof(struct PUCCH_FormatConfig, nrofSlots),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_nrofSlots_7,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nrofSlots"
		},
	{ ATF_POINTER, 2, offsetof(struct PUCCH_FormatConfig, pi2BPSK),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_pi2BPSK_11,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pi2BPSK"
		},
	{ ATF_POINTER, 1, offsetof(struct PUCCH_FormatConfig, simultaneousHARQ_ACK_CSI),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_simultaneousHARQ_ACK_CSI_13,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"simultaneousHARQ-ACK-CSI"
		},
};
static const int asn_MAP_PUCCH_FormatConfig_oms_1[] = { 0, 1, 2, 3, 4, 5 };
static const ber_tlv_tag_t asn_DEF_PUCCH_FormatConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PUCCH_FormatConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* interslotFrequencyHopping */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* additionalDMRS */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* maxCodeRate */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* nrofSlots */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* pi2BPSK */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* simultaneousHARQ-ACK-CSI */
};
asn_SEQUENCE_specifics_t asn_SPC_PUCCH_FormatConfig_specs_1 = {
	sizeof(struct PUCCH_FormatConfig),
	offsetof(struct PUCCH_FormatConfig, _asn_ctx),
	asn_MAP_PUCCH_FormatConfig_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_PUCCH_FormatConfig_oms_1,	/* Optional members */
	6, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PUCCH_FormatConfig = {
	"PUCCH-FormatConfig",
	"PUCCH-FormatConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_PUCCH_FormatConfig_tags_1,
	sizeof(asn_DEF_PUCCH_FormatConfig_tags_1)
		/sizeof(asn_DEF_PUCCH_FormatConfig_tags_1[0]), /* 1 */
	asn_DEF_PUCCH_FormatConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_PUCCH_FormatConfig_tags_1)
		/sizeof(asn_DEF_PUCCH_FormatConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PUCCH_FormatConfig_1,
	6,	/* Elements count */
	&asn_SPC_PUCCH_FormatConfig_specs_1	/* Additional specs */
};

