/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SecurityConfig.h"

#include "SecurityAlgorithmConfig.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_keyToUse_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_keyToUse_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_keyToUse_value2enum_3[] = {
	{ 0,	6,	"master" },
	{ 1,	9,	"secondary" }
};
static const unsigned int asn_MAP_keyToUse_enum2value_3[] = {
	0,	/* master(0) */
	1	/* secondary(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_keyToUse_specs_3 = {
	asn_MAP_keyToUse_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_keyToUse_enum2value_3,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_keyToUse_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_keyToUse_3 = {
	"keyToUse",
	"keyToUse",
	&asn_OP_NativeEnumerated,
	asn_DEF_keyToUse_tags_3,
	sizeof(asn_DEF_keyToUse_tags_3)
		/sizeof(asn_DEF_keyToUse_tags_3[0]) - 1, /* 1 */
	asn_DEF_keyToUse_tags_3,	/* Same as above */
	sizeof(asn_DEF_keyToUse_tags_3)
		/sizeof(asn_DEF_keyToUse_tags_3[0]), /* 2 */
	{ &asn_OER_type_keyToUse_constr_3, &asn_PER_type_keyToUse_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_keyToUse_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SecurityConfig_1[] = {
	{ ATF_POINTER, 2, offsetof(struct SecurityConfig, securityAlgorithmConfig),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SecurityAlgorithmConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"securityAlgorithmConfig"
		},
	{ ATF_POINTER, 1, offsetof(struct SecurityConfig, keyToUse),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_keyToUse_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"keyToUse"
		},
};
static const int asn_MAP_SecurityConfig_oms_1[] = { 0, 1 };
static const ber_tlv_tag_t asn_DEF_SecurityConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SecurityConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* securityAlgorithmConfig */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* keyToUse */
};
asn_SEQUENCE_specifics_t asn_SPC_SecurityConfig_specs_1 = {
	sizeof(struct SecurityConfig),
	offsetof(struct SecurityConfig, _asn_ctx),
	asn_MAP_SecurityConfig_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_SecurityConfig_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SecurityConfig = {
	"SecurityConfig",
	"SecurityConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_SecurityConfig_tags_1,
	sizeof(asn_DEF_SecurityConfig_tags_1)
		/sizeof(asn_DEF_SecurityConfig_tags_1[0]), /* 1 */
	asn_DEF_SecurityConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_SecurityConfig_tags_1)
		/sizeof(asn_DEF_SecurityConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SecurityConfig_1,
	2,	/* Elements count */
	&asn_SPC_SecurityConfig_specs_1	/* Additional specs */
};

