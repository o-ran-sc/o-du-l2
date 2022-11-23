/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "PUCCH-Resource.h"

#include "PUCCH-format0.h"
#include "PUCCH-format1.h"
#include "PUCCH-format2.h"
#include "PUCCH-format3.h"
#include "PUCCH-format4.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_intraSlotFrequencyHopping_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_intraSlotFrequencyHopping_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_format_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_format_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  4 }	/* (0..4) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_intraSlotFrequencyHopping_value2enum_4[] = {
	{ 0,	7,	"enabled" }
};
static const unsigned int asn_MAP_intraSlotFrequencyHopping_enum2value_4[] = {
	0	/* enabled(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_intraSlotFrequencyHopping_specs_4 = {
	asn_MAP_intraSlotFrequencyHopping_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_intraSlotFrequencyHopping_enum2value_4,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_intraSlotFrequencyHopping_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_intraSlotFrequencyHopping_4 = {
	"intraSlotFrequencyHopping",
	"intraSlotFrequencyHopping",
	&asn_OP_NativeEnumerated,
	asn_DEF_intraSlotFrequencyHopping_tags_4,
	sizeof(asn_DEF_intraSlotFrequencyHopping_tags_4)
		/sizeof(asn_DEF_intraSlotFrequencyHopping_tags_4[0]) - 1, /* 1 */
	asn_DEF_intraSlotFrequencyHopping_tags_4,	/* Same as above */
	sizeof(asn_DEF_intraSlotFrequencyHopping_tags_4)
		/sizeof(asn_DEF_intraSlotFrequencyHopping_tags_4[0]), /* 2 */
	{ &asn_OER_type_intraSlotFrequencyHopping_constr_4, &asn_PER_type_intraSlotFrequencyHopping_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_intraSlotFrequencyHopping_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_format_7[] = {
	{ ATF_POINTER, 0, offsetof(struct PUCCH_Resource__format, choice.format0),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_format0,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"format0"
		},
	{ ATF_POINTER, 0, offsetof(struct PUCCH_Resource__format, choice.format1),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_format1,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"format1"
		},
	{ ATF_POINTER, 0, offsetof(struct PUCCH_Resource__format, choice.format2),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_format2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"format2"
		},
	{ ATF_POINTER, 0, offsetof(struct PUCCH_Resource__format, choice.format3),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_format3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"format3"
		},
	{ ATF_POINTER, 0, offsetof(struct PUCCH_Resource__format, choice.format4),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_format4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"format4"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_format_tag2el_7[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* format0 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* format1 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* format2 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* format3 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* format4 */
};
static asn_CHOICE_specifics_t asn_SPC_format_specs_7 = {
	sizeof(struct PUCCH_Resource__format),
	offsetof(struct PUCCH_Resource__format, _asn_ctx),
	offsetof(struct PUCCH_Resource__format, present),
	sizeof(((struct PUCCH_Resource__format *)0)->present),
	asn_MAP_format_tag2el_7,
	5,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_format_7 = {
	"format",
	"format",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_format_constr_7, &asn_PER_type_format_constr_7, CHOICE_constraint },
	asn_MBR_format_7,
	5,	/* Elements count */
	&asn_SPC_format_specs_7	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_PUCCH_Resource_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PUCCH_Resource, pucch_ResourceId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_ResourceId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pucch-ResourceId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PUCCH_Resource, startingPRB),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PRB_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"startingPRB"
		},
	{ ATF_POINTER, 2, offsetof(struct PUCCH_Resource, intraSlotFrequencyHopping),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_intraSlotFrequencyHopping_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"intraSlotFrequencyHopping"
		},
	{ ATF_POINTER, 1, offsetof(struct PUCCH_Resource, secondHopPRB),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PRB_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"secondHopPRB"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PUCCH_Resource, format),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_format_7,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"format"
		},
};
static const int asn_MAP_PUCCH_Resource_oms_1[] = { 2, 3 };
static const ber_tlv_tag_t asn_DEF_PUCCH_Resource_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PUCCH_Resource_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pucch-ResourceId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* startingPRB */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* intraSlotFrequencyHopping */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* secondHopPRB */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* format */
};
asn_SEQUENCE_specifics_t asn_SPC_PUCCH_Resource_specs_1 = {
	sizeof(struct PUCCH_Resource),
	offsetof(struct PUCCH_Resource, _asn_ctx),
	asn_MAP_PUCCH_Resource_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_PUCCH_Resource_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PUCCH_Resource = {
	"PUCCH-Resource",
	"PUCCH-Resource",
	&asn_OP_SEQUENCE,
	asn_DEF_PUCCH_Resource_tags_1,
	sizeof(asn_DEF_PUCCH_Resource_tags_1)
		/sizeof(asn_DEF_PUCCH_Resource_tags_1[0]), /* 1 */
	asn_DEF_PUCCH_Resource_tags_1,	/* Same as above */
	sizeof(asn_DEF_PUCCH_Resource_tags_1)
		/sizeof(asn_DEF_PUCCH_Resource_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PUCCH_Resource_1,
	5,	/* Elements count */
	&asn_SPC_PUCCH_Resource_specs_1	/* Additional specs */
};

