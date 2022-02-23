/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SpCellConfig.h"

#include "ReconfigurationWithSync.h"
#include "ServingCellConfig.h"
#include "RLF-TimersAndConstants.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_rlf_TimersAndConstants_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_rlf_TimersAndConstants_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_rlmInSyncOutOfSyncThreshold_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_rlmInSyncOutOfSyncThreshold_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_rlf_TimersAndConstants_4[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SpCellConfig__rlf_TimersAndConstants, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct SpCellConfig__rlf_TimersAndConstants, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RLF_TimersAndConstants,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_rlf_TimersAndConstants_tag2el_4[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_rlf_TimersAndConstants_specs_4 = {
	sizeof(struct SpCellConfig__rlf_TimersAndConstants),
	offsetof(struct SpCellConfig__rlf_TimersAndConstants, _asn_ctx),
	offsetof(struct SpCellConfig__rlf_TimersAndConstants, present),
	sizeof(((struct SpCellConfig__rlf_TimersAndConstants *)0)->present),
	asn_MAP_rlf_TimersAndConstants_tag2el_4,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_rlf_TimersAndConstants_4 = {
	"rlf-TimersAndConstants",
	"rlf-TimersAndConstants",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_rlf_TimersAndConstants_constr_4, &asn_PER_type_rlf_TimersAndConstants_constr_4, CHOICE_constraint },
	asn_MBR_rlf_TimersAndConstants_4,
	2,	/* Elements count */
	&asn_SPC_rlf_TimersAndConstants_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_rlmInSyncOutOfSyncThreshold_value2enum_7[] = {
	{ 0,	2,	"n1" }
};
static const unsigned int asn_MAP_rlmInSyncOutOfSyncThreshold_enum2value_7[] = {
	0	/* n1(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_rlmInSyncOutOfSyncThreshold_specs_7 = {
	asn_MAP_rlmInSyncOutOfSyncThreshold_value2enum_7,	/* "tag" => N; sorted by tag */
	asn_MAP_rlmInSyncOutOfSyncThreshold_enum2value_7,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_rlmInSyncOutOfSyncThreshold_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_rlmInSyncOutOfSyncThreshold_7 = {
	"rlmInSyncOutOfSyncThreshold",
	"rlmInSyncOutOfSyncThreshold",
	&asn_OP_NativeEnumerated,
	asn_DEF_rlmInSyncOutOfSyncThreshold_tags_7,
	sizeof(asn_DEF_rlmInSyncOutOfSyncThreshold_tags_7)
		/sizeof(asn_DEF_rlmInSyncOutOfSyncThreshold_tags_7[0]) - 1, /* 1 */
	asn_DEF_rlmInSyncOutOfSyncThreshold_tags_7,	/* Same as above */
	sizeof(asn_DEF_rlmInSyncOutOfSyncThreshold_tags_7)
		/sizeof(asn_DEF_rlmInSyncOutOfSyncThreshold_tags_7[0]), /* 2 */
	{ &asn_OER_type_rlmInSyncOutOfSyncThreshold_constr_7, &asn_PER_type_rlmInSyncOutOfSyncThreshold_constr_7, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_rlmInSyncOutOfSyncThreshold_specs_7	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SpCellConfig_1[] = {
	{ ATF_POINTER, 5, offsetof(struct SpCellConfig, servCellIndex),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServCellIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"servCellIndex"
		},
	{ ATF_POINTER, 4, offsetof(struct SpCellConfig, reconfigurationWithSync),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReconfigurationWithSync,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reconfigurationWithSync"
		},
	{ ATF_POINTER, 3, offsetof(struct SpCellConfig, rlf_TimersAndConstants),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_rlf_TimersAndConstants_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rlf-TimersAndConstants"
		},
	{ ATF_POINTER, 2, offsetof(struct SpCellConfig, rlmInSyncOutOfSyncThreshold),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_rlmInSyncOutOfSyncThreshold_7,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rlmInSyncOutOfSyncThreshold"
		},
	{ ATF_POINTER, 1, offsetof(struct SpCellConfig, spCellConfigDedicated),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServingCellConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spCellConfigDedicated"
		},
};
static const int asn_MAP_SpCellConfig_oms_1[] = { 0, 1, 2, 3, 4 };
static const ber_tlv_tag_t asn_DEF_SpCellConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SpCellConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* servCellIndex */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* reconfigurationWithSync */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* rlf-TimersAndConstants */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* rlmInSyncOutOfSyncThreshold */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* spCellConfigDedicated */
};
asn_SEQUENCE_specifics_t asn_SPC_SpCellConfig_specs_1 = {
	sizeof(struct SpCellConfig),
	offsetof(struct SpCellConfig, _asn_ctx),
	asn_MAP_SpCellConfig_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_SpCellConfig_oms_1,	/* Optional members */
	5, 0,	/* Root/Additions */
	5,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SpCellConfig = {
	"SpCellConfig",
	"SpCellConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_SpCellConfig_tags_1,
	sizeof(asn_DEF_SpCellConfig_tags_1)
		/sizeof(asn_DEF_SpCellConfig_tags_1[0]), /* 1 */
	asn_DEF_SpCellConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_SpCellConfig_tags_1)
		/sizeof(asn_DEF_SpCellConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SpCellConfig_1,
	5,	/* Elements count */
	&asn_SPC_SpCellConfig_specs_1	/* Additional specs */
};

