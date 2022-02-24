/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "UCI-OnPUSCH.h"

#include "BetaOffsets.h"
static int
memb_dynamic_constraint_2(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size == 4)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_dynamic_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	4	/* (SIZE(4..4)) */};
static asn_per_constraints_t asn_PER_type_dynamic_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  4,  4 }	/* (SIZE(4..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_dynamic_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	4	/* (SIZE(4..4)) */};
static asn_per_constraints_t asn_PER_memb_dynamic_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  4,  4 }	/* (SIZE(4..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_betaOffsets_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_betaOffsets_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_scaling_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_scaling_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  3 }	/* (0..3) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_dynamic_3[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_BetaOffsets,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_dynamic_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_dynamic_specs_3 = {
	sizeof(struct UCI_OnPUSCH__betaOffsets__dynamic),
	offsetof(struct UCI_OnPUSCH__betaOffsets__dynamic, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_dynamic_3 = {
	"dynamic",
	"dynamic",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_dynamic_tags_3,
	sizeof(asn_DEF_dynamic_tags_3)
		/sizeof(asn_DEF_dynamic_tags_3[0]) - 1, /* 1 */
	asn_DEF_dynamic_tags_3,	/* Same as above */
	sizeof(asn_DEF_dynamic_tags_3)
		/sizeof(asn_DEF_dynamic_tags_3[0]), /* 2 */
	{ &asn_OER_type_dynamic_constr_3, &asn_PER_type_dynamic_constr_3, SEQUENCE_OF_constraint },
	asn_MBR_dynamic_3,
	1,	/* Single element */
	&asn_SPC_dynamic_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_betaOffsets_2[] = {
	{ ATF_POINTER, 0, offsetof(struct UCI_OnPUSCH__betaOffsets, choice.dynamic),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_dynamic_3,
		0,
		{ &asn_OER_memb_dynamic_constr_3, &asn_PER_memb_dynamic_constr_3,  memb_dynamic_constraint_2 },
		0, 0, /* No default value */
		"dynamic"
		},
	{ ATF_POINTER, 0, offsetof(struct UCI_OnPUSCH__betaOffsets, choice.semiStatic),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BetaOffsets,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"semiStatic"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_betaOffsets_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* dynamic */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* semiStatic */
};
static asn_CHOICE_specifics_t asn_SPC_betaOffsets_specs_2 = {
	sizeof(struct UCI_OnPUSCH__betaOffsets),
	offsetof(struct UCI_OnPUSCH__betaOffsets, _asn_ctx),
	offsetof(struct UCI_OnPUSCH__betaOffsets, present),
	sizeof(((struct UCI_OnPUSCH__betaOffsets *)0)->present),
	asn_MAP_betaOffsets_tag2el_2,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_betaOffsets_2 = {
	"betaOffsets",
	"betaOffsets",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_betaOffsets_constr_2, &asn_PER_type_betaOffsets_constr_2, CHOICE_constraint },
	asn_MBR_betaOffsets_2,
	2,	/* Elements count */
	&asn_SPC_betaOffsets_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_scaling_value2enum_6[] = {
	{ 0,	4,	"f0p5" },
	{ 1,	5,	"f0p65" },
	{ 2,	4,	"f0p8" },
	{ 3,	2,	"f1" }
};
static const unsigned int asn_MAP_scaling_enum2value_6[] = {
	0,	/* f0p5(0) */
	1,	/* f0p65(1) */
	2,	/* f0p8(2) */
	3	/* f1(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_scaling_specs_6 = {
	asn_MAP_scaling_value2enum_6,	/* "tag" => N; sorted by tag */
	asn_MAP_scaling_enum2value_6,	/* N => "tag"; sorted by N */
	4,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_scaling_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_scaling_6 = {
	"scaling",
	"scaling",
	&asn_OP_NativeEnumerated,
	asn_DEF_scaling_tags_6,
	sizeof(asn_DEF_scaling_tags_6)
		/sizeof(asn_DEF_scaling_tags_6[0]) - 1, /* 1 */
	asn_DEF_scaling_tags_6,	/* Same as above */
	sizeof(asn_DEF_scaling_tags_6)
		/sizeof(asn_DEF_scaling_tags_6[0]), /* 2 */
	{ &asn_OER_type_scaling_constr_6, &asn_PER_type_scaling_constr_6, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_scaling_specs_6	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_UCI_OnPUSCH_1[] = {
	{ ATF_POINTER, 1, offsetof(struct UCI_OnPUSCH, betaOffsets),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_betaOffsets_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"betaOffsets"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UCI_OnPUSCH, scaling),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_scaling_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"scaling"
		},
};
static const int asn_MAP_UCI_OnPUSCH_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_UCI_OnPUSCH_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UCI_OnPUSCH_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* betaOffsets */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* scaling */
};
asn_SEQUENCE_specifics_t asn_SPC_UCI_OnPUSCH_specs_1 = {
	sizeof(struct UCI_OnPUSCH),
	offsetof(struct UCI_OnPUSCH, _asn_ctx),
	asn_MAP_UCI_OnPUSCH_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_UCI_OnPUSCH_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UCI_OnPUSCH = {
	"UCI-OnPUSCH",
	"UCI-OnPUSCH",
	&asn_OP_SEQUENCE,
	asn_DEF_UCI_OnPUSCH_tags_1,
	sizeof(asn_DEF_UCI_OnPUSCH_tags_1)
		/sizeof(asn_DEF_UCI_OnPUSCH_tags_1[0]), /* 1 */
	asn_DEF_UCI_OnPUSCH_tags_1,	/* Same as above */
	sizeof(asn_DEF_UCI_OnPUSCH_tags_1)
		/sizeof(asn_DEF_UCI_OnPUSCH_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UCI_OnPUSCH_1,
	2,	/* Elements count */
	&asn_SPC_UCI_OnPUSCH_specs_1	/* Additional specs */
};

