/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "EUTRA-PhysCellIdRange.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_range_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_range_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_range_value2enum_3[] = {
	{ 0,	2,	"n4" },
	{ 1,	2,	"n8" },
	{ 2,	3,	"n12" },
	{ 3,	3,	"n16" },
	{ 4,	3,	"n24" },
	{ 5,	3,	"n32" },
	{ 6,	3,	"n48" },
	{ 7,	3,	"n64" },
	{ 8,	3,	"n84" },
	{ 9,	3,	"n96" },
	{ 10,	4,	"n128" },
	{ 11,	4,	"n168" },
	{ 12,	4,	"n252" },
	{ 13,	4,	"n504" },
	{ 14,	6,	"spare2" },
	{ 15,	6,	"spare1" }
};
static const unsigned int asn_MAP_range_enum2value_3[] = {
	2,	/* n12(2) */
	10,	/* n128(10) */
	3,	/* n16(3) */
	11,	/* n168(11) */
	4,	/* n24(4) */
	12,	/* n252(12) */
	5,	/* n32(5) */
	0,	/* n4(0) */
	6,	/* n48(6) */
	13,	/* n504(13) */
	7,	/* n64(7) */
	1,	/* n8(1) */
	8,	/* n84(8) */
	9,	/* n96(9) */
	15,	/* spare1(15) */
	14	/* spare2(14) */
};
static const asn_INTEGER_specifics_t asn_SPC_range_specs_3 = {
	asn_MAP_range_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_range_enum2value_3,	/* N => "tag"; sorted by N */
	16,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_range_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_range_3 = {
	"range",
	"range",
	&asn_OP_NativeEnumerated,
	asn_DEF_range_tags_3,
	sizeof(asn_DEF_range_tags_3)
		/sizeof(asn_DEF_range_tags_3[0]) - 1, /* 1 */
	asn_DEF_range_tags_3,	/* Same as above */
	sizeof(asn_DEF_range_tags_3)
		/sizeof(asn_DEF_range_tags_3[0]), /* 2 */
	{ &asn_OER_type_range_constr_3, &asn_PER_type_range_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_range_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_EUTRA_PhysCellIdRange_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_PhysCellIdRange, start),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EUTRA_PhysCellId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"start"
		},
	{ ATF_POINTER, 1, offsetof(struct EUTRA_PhysCellIdRange, range),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_range_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"range"
		},
};
static const int asn_MAP_EUTRA_PhysCellIdRange_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_EUTRA_PhysCellIdRange_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_EUTRA_PhysCellIdRange_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* start */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* range */
};
asn_SEQUENCE_specifics_t asn_SPC_EUTRA_PhysCellIdRange_specs_1 = {
	sizeof(struct EUTRA_PhysCellIdRange),
	offsetof(struct EUTRA_PhysCellIdRange, _asn_ctx),
	asn_MAP_EUTRA_PhysCellIdRange_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_EUTRA_PhysCellIdRange_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_EUTRA_PhysCellIdRange = {
	"EUTRA-PhysCellIdRange",
	"EUTRA-PhysCellIdRange",
	&asn_OP_SEQUENCE,
	asn_DEF_EUTRA_PhysCellIdRange_tags_1,
	sizeof(asn_DEF_EUTRA_PhysCellIdRange_tags_1)
		/sizeof(asn_DEF_EUTRA_PhysCellIdRange_tags_1[0]), /* 1 */
	asn_DEF_EUTRA_PhysCellIdRange_tags_1,	/* Same as above */
	sizeof(asn_DEF_EUTRA_PhysCellIdRange_tags_1)
		/sizeof(asn_DEF_EUTRA_PhysCellIdRange_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_EUTRA_PhysCellIdRange_1,
	2,	/* Elements count */
	&asn_SPC_EUTRA_PhysCellIdRange_specs_1	/* Additional specs */
};

