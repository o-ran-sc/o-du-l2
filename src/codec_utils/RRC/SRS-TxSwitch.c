/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SRS-TxSwitch.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_supportedSRS_TxPortSwitch_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_supportedSRS_TxPortSwitch_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  4 }	/* (0..4) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_txSwitchImpactToRx_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_txSwitchImpactToRx_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_supportedSRS_TxPortSwitch_value2enum_2[] = {
	{ 0,	4,	"t1r2" },
	{ 1,	4,	"t1r4" },
	{ 2,	4,	"t2r4" },
	{ 3,	9,	"t1r4-t2r4" },
	{ 4,	8,	"tr-equal" }
};
static const unsigned int asn_MAP_supportedSRS_TxPortSwitch_enum2value_2[] = {
	0,	/* t1r2(0) */
	1,	/* t1r4(1) */
	3,	/* t1r4-t2r4(3) */
	2,	/* t2r4(2) */
	4	/* tr-equal(4) */
};
static const asn_INTEGER_specifics_t asn_SPC_supportedSRS_TxPortSwitch_specs_2 = {
	asn_MAP_supportedSRS_TxPortSwitch_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_supportedSRS_TxPortSwitch_enum2value_2,	/* N => "tag"; sorted by N */
	5,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_supportedSRS_TxPortSwitch_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_supportedSRS_TxPortSwitch_2 = {
	"supportedSRS-TxPortSwitch",
	"supportedSRS-TxPortSwitch",
	&asn_OP_NativeEnumerated,
	asn_DEF_supportedSRS_TxPortSwitch_tags_2,
	sizeof(asn_DEF_supportedSRS_TxPortSwitch_tags_2)
		/sizeof(asn_DEF_supportedSRS_TxPortSwitch_tags_2[0]) - 1, /* 1 */
	asn_DEF_supportedSRS_TxPortSwitch_tags_2,	/* Same as above */
	sizeof(asn_DEF_supportedSRS_TxPortSwitch_tags_2)
		/sizeof(asn_DEF_supportedSRS_TxPortSwitch_tags_2[0]), /* 2 */
	{ &asn_OER_type_supportedSRS_TxPortSwitch_constr_2, &asn_PER_type_supportedSRS_TxPortSwitch_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_supportedSRS_TxPortSwitch_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_txSwitchImpactToRx_value2enum_8[] = {
	{ 0,	4,	"true" }
};
static const unsigned int asn_MAP_txSwitchImpactToRx_enum2value_8[] = {
	0	/* true(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_txSwitchImpactToRx_specs_8 = {
	asn_MAP_txSwitchImpactToRx_value2enum_8,	/* "tag" => N; sorted by tag */
	asn_MAP_txSwitchImpactToRx_enum2value_8,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_txSwitchImpactToRx_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_txSwitchImpactToRx_8 = {
	"txSwitchImpactToRx",
	"txSwitchImpactToRx",
	&asn_OP_NativeEnumerated,
	asn_DEF_txSwitchImpactToRx_tags_8,
	sizeof(asn_DEF_txSwitchImpactToRx_tags_8)
		/sizeof(asn_DEF_txSwitchImpactToRx_tags_8[0]) - 1, /* 1 */
	asn_DEF_txSwitchImpactToRx_tags_8,	/* Same as above */
	sizeof(asn_DEF_txSwitchImpactToRx_tags_8)
		/sizeof(asn_DEF_txSwitchImpactToRx_tags_8[0]), /* 2 */
	{ &asn_OER_type_txSwitchImpactToRx_constr_8, &asn_PER_type_txSwitchImpactToRx_constr_8, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_txSwitchImpactToRx_specs_8	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SRS_TxSwitch_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_TxSwitch, supportedSRS_TxPortSwitch),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_supportedSRS_TxPortSwitch_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"supportedSRS-TxPortSwitch"
		},
	{ ATF_POINTER, 1, offsetof(struct SRS_TxSwitch, txSwitchImpactToRx),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_txSwitchImpactToRx_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"txSwitchImpactToRx"
		},
};
static const int asn_MAP_SRS_TxSwitch_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_SRS_TxSwitch_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SRS_TxSwitch_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* supportedSRS-TxPortSwitch */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* txSwitchImpactToRx */
};
asn_SEQUENCE_specifics_t asn_SPC_SRS_TxSwitch_specs_1 = {
	sizeof(struct SRS_TxSwitch),
	offsetof(struct SRS_TxSwitch, _asn_ctx),
	asn_MAP_SRS_TxSwitch_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_SRS_TxSwitch_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SRS_TxSwitch = {
	"SRS-TxSwitch",
	"SRS-TxSwitch",
	&asn_OP_SEQUENCE,
	asn_DEF_SRS_TxSwitch_tags_1,
	sizeof(asn_DEF_SRS_TxSwitch_tags_1)
		/sizeof(asn_DEF_SRS_TxSwitch_tags_1[0]), /* 1 */
	asn_DEF_SRS_TxSwitch_tags_1,	/* Same as above */
	sizeof(asn_DEF_SRS_TxSwitch_tags_1)
		/sizeof(asn_DEF_SRS_TxSwitch_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SRS_TxSwitch_1,
	2,	/* Elements count */
	&asn_SPC_SRS_TxSwitch_specs_1	/* Additional specs */
};

