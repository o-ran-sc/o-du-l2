/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "FR-Info.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_fr_Type_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_fr_Type_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_fr_Type_value2enum_3[] = {
	{ 0,	3,	"fr1" },
	{ 1,	3,	"fr2" }
};
static const unsigned int asn_MAP_fr_Type_enum2value_3[] = {
	0,	/* fr1(0) */
	1	/* fr2(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_fr_Type_specs_3 = {
	asn_MAP_fr_Type_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_fr_Type_enum2value_3,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_fr_Type_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_fr_Type_3 = {
	"fr-Type",
	"fr-Type",
	&asn_OP_NativeEnumerated,
	asn_DEF_fr_Type_tags_3,
	sizeof(asn_DEF_fr_Type_tags_3)
		/sizeof(asn_DEF_fr_Type_tags_3[0]) - 1, /* 1 */
	asn_DEF_fr_Type_tags_3,	/* Same as above */
	sizeof(asn_DEF_fr_Type_tags_3)
		/sizeof(asn_DEF_fr_Type_tags_3[0]), /* 2 */
	{ &asn_OER_type_fr_Type_constr_3, &asn_PER_type_fr_Type_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_fr_Type_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_FR_Info_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct FR_Info, servCellIndex),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServCellIndex,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"servCellIndex"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct FR_Info, fr_Type),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_fr_Type_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"fr-Type"
		},
};
static const ber_tlv_tag_t asn_DEF_FR_Info_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_FR_Info_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* servCellIndex */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* fr-Type */
};
asn_SEQUENCE_specifics_t asn_SPC_FR_Info_specs_1 = {
	sizeof(struct FR_Info),
	offsetof(struct FR_Info, _asn_ctx),
	asn_MAP_FR_Info_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_FR_Info = {
	"FR-Info",
	"FR-Info",
	&asn_OP_SEQUENCE,
	asn_DEF_FR_Info_tags_1,
	sizeof(asn_DEF_FR_Info_tags_1)
		/sizeof(asn_DEF_FR_Info_tags_1[0]), /* 1 */
	asn_DEF_FR_Info_tags_1,	/* Same as above */
	sizeof(asn_DEF_FR_Info_tags_1)
		/sizeof(asn_DEF_FR_Info_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_FR_Info_1,
	2,	/* Elements count */
	&asn_SPC_FR_Info_specs_1	/* Additional specs */
};

