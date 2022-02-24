/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "AffectedCarrierFreqCombInfoMRDC.h"

#include "AffectedCarrierFreqCombEUTRA.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_interferenceDirectionMRDC_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_interferenceDirectionMRDC_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_interferenceDirectionMRDC_value2enum_3[] = {
	{ 0,	8,	"eutra-nr" },
	{ 1,	2,	"nr" },
	{ 2,	5,	"other" },
	{ 3,	13,	"utra-nr-other" },
	{ 4,	8,	"nr-other" },
	{ 5,	6,	"spare3" },
	{ 6,	6,	"spare2" },
	{ 7,	6,	"spare1" }
};
static const unsigned int asn_MAP_interferenceDirectionMRDC_enum2value_3[] = {
	0,	/* eutra-nr(0) */
	1,	/* nr(1) */
	4,	/* nr-other(4) */
	2,	/* other(2) */
	7,	/* spare1(7) */
	6,	/* spare2(6) */
	5,	/* spare3(5) */
	3	/* utra-nr-other(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_interferenceDirectionMRDC_specs_3 = {
	asn_MAP_interferenceDirectionMRDC_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_interferenceDirectionMRDC_enum2value_3,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_interferenceDirectionMRDC_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_interferenceDirectionMRDC_3 = {
	"interferenceDirectionMRDC",
	"interferenceDirectionMRDC",
	&asn_OP_NativeEnumerated,
	asn_DEF_interferenceDirectionMRDC_tags_3,
	sizeof(asn_DEF_interferenceDirectionMRDC_tags_3)
		/sizeof(asn_DEF_interferenceDirectionMRDC_tags_3[0]) - 1, /* 1 */
	asn_DEF_interferenceDirectionMRDC_tags_3,	/* Same as above */
	sizeof(asn_DEF_interferenceDirectionMRDC_tags_3)
		/sizeof(asn_DEF_interferenceDirectionMRDC_tags_3[0]), /* 2 */
	{ &asn_OER_type_interferenceDirectionMRDC_constr_3, &asn_PER_type_interferenceDirectionMRDC_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_interferenceDirectionMRDC_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_affectedCarrierFreqCombMRDC_12[] = {
	{ ATF_POINTER, 1, offsetof(struct AffectedCarrierFreqCombInfoMRDC__affectedCarrierFreqCombMRDC, affectedCarrierFreqCombEUTRA),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AffectedCarrierFreqCombEUTRA,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"affectedCarrierFreqCombEUTRA"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AffectedCarrierFreqCombInfoMRDC__affectedCarrierFreqCombMRDC, affectedCarrierFreqCombNR),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AffectedCarrierFreqCombNR,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"affectedCarrierFreqCombNR"
		},
};
static const int asn_MAP_affectedCarrierFreqCombMRDC_oms_12[] = { 0 };
static const ber_tlv_tag_t asn_DEF_affectedCarrierFreqCombMRDC_tags_12[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_affectedCarrierFreqCombMRDC_tag2el_12[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* affectedCarrierFreqCombEUTRA */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* affectedCarrierFreqCombNR */
};
static asn_SEQUENCE_specifics_t asn_SPC_affectedCarrierFreqCombMRDC_specs_12 = {
	sizeof(struct AffectedCarrierFreqCombInfoMRDC__affectedCarrierFreqCombMRDC),
	offsetof(struct AffectedCarrierFreqCombInfoMRDC__affectedCarrierFreqCombMRDC, _asn_ctx),
	asn_MAP_affectedCarrierFreqCombMRDC_tag2el_12,
	2,	/* Count of tags in the map */
	asn_MAP_affectedCarrierFreqCombMRDC_oms_12,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_affectedCarrierFreqCombMRDC_12 = {
	"affectedCarrierFreqCombMRDC",
	"affectedCarrierFreqCombMRDC",
	&asn_OP_SEQUENCE,
	asn_DEF_affectedCarrierFreqCombMRDC_tags_12,
	sizeof(asn_DEF_affectedCarrierFreqCombMRDC_tags_12)
		/sizeof(asn_DEF_affectedCarrierFreqCombMRDC_tags_12[0]) - 1, /* 1 */
	asn_DEF_affectedCarrierFreqCombMRDC_tags_12,	/* Same as above */
	sizeof(asn_DEF_affectedCarrierFreqCombMRDC_tags_12)
		/sizeof(asn_DEF_affectedCarrierFreqCombMRDC_tags_12[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_affectedCarrierFreqCombMRDC_12,
	2,	/* Elements count */
	&asn_SPC_affectedCarrierFreqCombMRDC_specs_12	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_AffectedCarrierFreqCombInfoMRDC_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct AffectedCarrierFreqCombInfoMRDC, victimSystemType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VictimSystemType,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"victimSystemType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AffectedCarrierFreqCombInfoMRDC, interferenceDirectionMRDC),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_interferenceDirectionMRDC_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"interferenceDirectionMRDC"
		},
	{ ATF_POINTER, 1, offsetof(struct AffectedCarrierFreqCombInfoMRDC, affectedCarrierFreqCombMRDC),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_affectedCarrierFreqCombMRDC_12,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"affectedCarrierFreqCombMRDC"
		},
};
static const int asn_MAP_AffectedCarrierFreqCombInfoMRDC_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_AffectedCarrierFreqCombInfoMRDC_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_AffectedCarrierFreqCombInfoMRDC_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* victimSystemType */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* interferenceDirectionMRDC */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* affectedCarrierFreqCombMRDC */
};
asn_SEQUENCE_specifics_t asn_SPC_AffectedCarrierFreqCombInfoMRDC_specs_1 = {
	sizeof(struct AffectedCarrierFreqCombInfoMRDC),
	offsetof(struct AffectedCarrierFreqCombInfoMRDC, _asn_ctx),
	asn_MAP_AffectedCarrierFreqCombInfoMRDC_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_AffectedCarrierFreqCombInfoMRDC_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_AffectedCarrierFreqCombInfoMRDC = {
	"AffectedCarrierFreqCombInfoMRDC",
	"AffectedCarrierFreqCombInfoMRDC",
	&asn_OP_SEQUENCE,
	asn_DEF_AffectedCarrierFreqCombInfoMRDC_tags_1,
	sizeof(asn_DEF_AffectedCarrierFreqCombInfoMRDC_tags_1)
		/sizeof(asn_DEF_AffectedCarrierFreqCombInfoMRDC_tags_1[0]), /* 1 */
	asn_DEF_AffectedCarrierFreqCombInfoMRDC_tags_1,	/* Same as above */
	sizeof(asn_DEF_AffectedCarrierFreqCombInfoMRDC_tags_1)
		/sizeof(asn_DEF_AffectedCarrierFreqCombInfoMRDC_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_AffectedCarrierFreqCombInfoMRDC_1,
	3,	/* Elements count */
	&asn_SPC_AffectedCarrierFreqCombInfoMRDC_specs_1	/* Additional specs */
};

