/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "UL-AM-RLC.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_maxRetxThreshold_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxRetxThreshold_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_maxRetxThreshold_value2enum_6[] = {
	{ 0,	2,	"t1" },
	{ 1,	2,	"t2" },
	{ 2,	2,	"t3" },
	{ 3,	2,	"t4" },
	{ 4,	2,	"t6" },
	{ 5,	2,	"t8" },
	{ 6,	3,	"t16" },
	{ 7,	3,	"t32" }
};
static const unsigned int asn_MAP_maxRetxThreshold_enum2value_6[] = {
	0,	/* t1(0) */
	6,	/* t16(6) */
	1,	/* t2(1) */
	2,	/* t3(2) */
	7,	/* t32(7) */
	3,	/* t4(3) */
	4,	/* t6(4) */
	5	/* t8(5) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxRetxThreshold_specs_6 = {
	asn_MAP_maxRetxThreshold_value2enum_6,	/* "tag" => N; sorted by tag */
	asn_MAP_maxRetxThreshold_enum2value_6,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxRetxThreshold_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxRetxThreshold_6 = {
	"maxRetxThreshold",
	"maxRetxThreshold",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxRetxThreshold_tags_6,
	sizeof(asn_DEF_maxRetxThreshold_tags_6)
		/sizeof(asn_DEF_maxRetxThreshold_tags_6[0]) - 1, /* 1 */
	asn_DEF_maxRetxThreshold_tags_6,	/* Same as above */
	sizeof(asn_DEF_maxRetxThreshold_tags_6)
		/sizeof(asn_DEF_maxRetxThreshold_tags_6[0]), /* 2 */
	{ &asn_OER_type_maxRetxThreshold_constr_6, &asn_PER_type_maxRetxThreshold_constr_6, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxRetxThreshold_specs_6	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_UL_AM_RLC_1[] = {
	{ ATF_POINTER, 1, offsetof(struct UL_AM_RLC, sn_FieldLength),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SN_FieldLengthAM,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sn-FieldLength"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_AM_RLC, t_PollRetransmit),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_T_PollRetransmit,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t-PollRetransmit"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_AM_RLC, pollPDU),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PollPDU,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pollPDU"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_AM_RLC, pollByte),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PollByte,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pollByte"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_AM_RLC, maxRetxThreshold),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxRetxThreshold_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxRetxThreshold"
		},
};
static const int asn_MAP_UL_AM_RLC_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_UL_AM_RLC_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UL_AM_RLC_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* sn-FieldLength */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* t-PollRetransmit */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* pollPDU */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* pollByte */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* maxRetxThreshold */
};
asn_SEQUENCE_specifics_t asn_SPC_UL_AM_RLC_specs_1 = {
	sizeof(struct UL_AM_RLC),
	offsetof(struct UL_AM_RLC, _asn_ctx),
	asn_MAP_UL_AM_RLC_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_UL_AM_RLC_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UL_AM_RLC = {
	"UL-AM-RLC",
	"UL-AM-RLC",
	&asn_OP_SEQUENCE,
	asn_DEF_UL_AM_RLC_tags_1,
	sizeof(asn_DEF_UL_AM_RLC_tags_1)
		/sizeof(asn_DEF_UL_AM_RLC_tags_1[0]), /* 1 */
	asn_DEF_UL_AM_RLC_tags_1,	/* Same as above */
	sizeof(asn_DEF_UL_AM_RLC_tags_1)
		/sizeof(asn_DEF_UL_AM_RLC_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UL_AM_RLC_1,
	5,	/* Elements count */
	&asn_SPC_UL_AM_RLC_specs_1	/* Additional specs */
};

