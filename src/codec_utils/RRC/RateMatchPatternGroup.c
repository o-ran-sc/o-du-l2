/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "RateMatchPatternGroup.h"

static asn_oer_constraints_t asn_OER_type_Member_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_Member_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_RateMatchPatternGroup_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..8)) */};
asn_per_constraints_t asn_PER_type_RateMatchPatternGroup_constr_1 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 3,  3,  1,  8 }	/* (SIZE(1..8)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_Member_2[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct RateMatchPatternGroup__Member, choice.cellLevel),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RateMatchPatternId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cellLevel"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RateMatchPatternGroup__Member, choice.bwpLevel),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RateMatchPatternId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"bwpLevel"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_Member_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* cellLevel */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* bwpLevel */
};
static asn_CHOICE_specifics_t asn_SPC_Member_specs_2 = {
	sizeof(struct RateMatchPatternGroup__Member),
	offsetof(struct RateMatchPatternGroup__Member, _asn_ctx),
	offsetof(struct RateMatchPatternGroup__Member, present),
	sizeof(((struct RateMatchPatternGroup__Member *)0)->present),
	asn_MAP_Member_tag2el_2,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_Member_2 = {
	"CHOICE",
	"CHOICE",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_Member_constr_2, &asn_PER_type_Member_constr_2, CHOICE_constraint },
	asn_MBR_Member_2,
	2,	/* Elements count */
	&asn_SPC_Member_specs_2	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_RateMatchPatternGroup_1[] = {
	{ ATF_POINTER, 0, 0,
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_Member_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_RateMatchPatternGroup_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_SET_OF_specifics_t asn_SPC_RateMatchPatternGroup_specs_1 = {
	sizeof(struct RateMatchPatternGroup),
	offsetof(struct RateMatchPatternGroup, _asn_ctx),
	2,	/* XER encoding is XMLValueList */
};
asn_TYPE_descriptor_t asn_DEF_RateMatchPatternGroup = {
	"RateMatchPatternGroup",
	"RateMatchPatternGroup",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_RateMatchPatternGroup_tags_1,
	sizeof(asn_DEF_RateMatchPatternGroup_tags_1)
		/sizeof(asn_DEF_RateMatchPatternGroup_tags_1[0]), /* 1 */
	asn_DEF_RateMatchPatternGroup_tags_1,	/* Same as above */
	sizeof(asn_DEF_RateMatchPatternGroup_tags_1)
		/sizeof(asn_DEF_RateMatchPatternGroup_tags_1[0]), /* 1 */
	{ &asn_OER_type_RateMatchPatternGroup_constr_1, &asn_PER_type_RateMatchPatternGroup_constr_1, SEQUENCE_OF_constraint },
	asn_MBR_RateMatchPatternGroup_1,
	1,	/* Single element */
	&asn_SPC_RateMatchPatternGroup_specs_1	/* Additional specs */
};

