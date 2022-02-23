/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "EUTRA-MBSFN-SubframeConfig.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_oneFrame_constraint_10(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 6)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_fourFrames_constraint_10(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 24)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_oneFrame_constraint_13(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 2)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_fourFrames_constraint_13(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 8)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_radioframeAllocationOffset_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 7)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_radioframeAllocationPeriod_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_radioframeAllocationPeriod_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  5 }	/* (0..5) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_oneFrame_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	6	/* (SIZE(6..6)) */};
static asn_per_constraints_t asn_PER_memb_oneFrame_constr_11 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  6,  6 }	/* (SIZE(6..6)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_fourFrames_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	24	/* (SIZE(24..24)) */};
static asn_per_constraints_t asn_PER_memb_fourFrames_constr_12 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  24,  24 }	/* (SIZE(24..24)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_subframeAllocation1_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_subframeAllocation1_constr_10 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_oneFrame_constr_14 CC_NOTUSED = {
	{ 0, 0 },
	2	/* (SIZE(2..2)) */};
static asn_per_constraints_t asn_PER_memb_oneFrame_constr_14 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  2,  2 }	/* (SIZE(2..2)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_fourFrames_constr_15 CC_NOTUSED = {
	{ 0, 0 },
	8	/* (SIZE(8..8)) */};
static asn_per_constraints_t asn_PER_memb_fourFrames_constr_15 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  8,  8 }	/* (SIZE(8..8)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_subframeAllocation2_constr_13 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_subframeAllocation2_constr_13 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_radioframeAllocationOffset_constr_9 CC_NOTUSED = {
	{ 1, 1 }	/* (0..7) */,
	-1};
static asn_per_constraints_t asn_PER_memb_radioframeAllocationOffset_constr_9 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_radioframeAllocationPeriod_value2enum_2[] = {
	{ 0,	2,	"n1" },
	{ 1,	2,	"n2" },
	{ 2,	2,	"n4" },
	{ 3,	2,	"n8" },
	{ 4,	3,	"n16" },
	{ 5,	3,	"n32" }
};
static const unsigned int asn_MAP_radioframeAllocationPeriod_enum2value_2[] = {
	0,	/* n1(0) */
	4,	/* n16(4) */
	1,	/* n2(1) */
	5,	/* n32(5) */
	2,	/* n4(2) */
	3	/* n8(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_radioframeAllocationPeriod_specs_2 = {
	asn_MAP_radioframeAllocationPeriod_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_radioframeAllocationPeriod_enum2value_2,	/* N => "tag"; sorted by N */
	6,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_radioframeAllocationPeriod_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_radioframeAllocationPeriod_2 = {
	"radioframeAllocationPeriod",
	"radioframeAllocationPeriod",
	&asn_OP_NativeEnumerated,
	asn_DEF_radioframeAllocationPeriod_tags_2,
	sizeof(asn_DEF_radioframeAllocationPeriod_tags_2)
		/sizeof(asn_DEF_radioframeAllocationPeriod_tags_2[0]) - 1, /* 1 */
	asn_DEF_radioframeAllocationPeriod_tags_2,	/* Same as above */
	sizeof(asn_DEF_radioframeAllocationPeriod_tags_2)
		/sizeof(asn_DEF_radioframeAllocationPeriod_tags_2[0]), /* 2 */
	{ &asn_OER_type_radioframeAllocationPeriod_constr_2, &asn_PER_type_radioframeAllocationPeriod_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_radioframeAllocationPeriod_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_subframeAllocation1_10[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation1, choice.oneFrame),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_oneFrame_constr_11, &asn_PER_memb_oneFrame_constr_11,  memb_oneFrame_constraint_10 },
		0, 0, /* No default value */
		"oneFrame"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation1, choice.fourFrames),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_fourFrames_constr_12, &asn_PER_memb_fourFrames_constr_12,  memb_fourFrames_constraint_10 },
		0, 0, /* No default value */
		"fourFrames"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_subframeAllocation1_tag2el_10[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* oneFrame */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* fourFrames */
};
static asn_CHOICE_specifics_t asn_SPC_subframeAllocation1_specs_10 = {
	sizeof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation1),
	offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation1, _asn_ctx),
	offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation1, present),
	sizeof(((struct EUTRA_MBSFN_SubframeConfig__subframeAllocation1 *)0)->present),
	asn_MAP_subframeAllocation1_tag2el_10,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_subframeAllocation1_10 = {
	"subframeAllocation1",
	"subframeAllocation1",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_subframeAllocation1_constr_10, &asn_PER_type_subframeAllocation1_constr_10, CHOICE_constraint },
	asn_MBR_subframeAllocation1_10,
	2,	/* Elements count */
	&asn_SPC_subframeAllocation1_specs_10	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_subframeAllocation2_13[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation2, choice.oneFrame),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_oneFrame_constr_14, &asn_PER_memb_oneFrame_constr_14,  memb_oneFrame_constraint_13 },
		0, 0, /* No default value */
		"oneFrame"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation2, choice.fourFrames),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_fourFrames_constr_15, &asn_PER_memb_fourFrames_constr_15,  memb_fourFrames_constraint_13 },
		0, 0, /* No default value */
		"fourFrames"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_subframeAllocation2_tag2el_13[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* oneFrame */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* fourFrames */
};
static asn_CHOICE_specifics_t asn_SPC_subframeAllocation2_specs_13 = {
	sizeof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation2),
	offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation2, _asn_ctx),
	offsetof(struct EUTRA_MBSFN_SubframeConfig__subframeAllocation2, present),
	sizeof(((struct EUTRA_MBSFN_SubframeConfig__subframeAllocation2 *)0)->present),
	asn_MAP_subframeAllocation2_tag2el_13,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_subframeAllocation2_13 = {
	"subframeAllocation2",
	"subframeAllocation2",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_subframeAllocation2_constr_13, &asn_PER_type_subframeAllocation2_constr_13, CHOICE_constraint },
	asn_MBR_subframeAllocation2_13,
	2,	/* Elements count */
	&asn_SPC_subframeAllocation2_specs_13	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_EUTRA_MBSFN_SubframeConfig_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_MBSFN_SubframeConfig, radioframeAllocationPeriod),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_radioframeAllocationPeriod_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"radioframeAllocationPeriod"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_MBSFN_SubframeConfig, radioframeAllocationOffset),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_radioframeAllocationOffset_constr_9, &asn_PER_memb_radioframeAllocationOffset_constr_9,  memb_radioframeAllocationOffset_constraint_1 },
		0, 0, /* No default value */
		"radioframeAllocationOffset"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct EUTRA_MBSFN_SubframeConfig, subframeAllocation1),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_subframeAllocation1_10,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"subframeAllocation1"
		},
	{ ATF_POINTER, 1, offsetof(struct EUTRA_MBSFN_SubframeConfig, subframeAllocation2),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_subframeAllocation2_13,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"subframeAllocation2"
		},
};
static const int asn_MAP_EUTRA_MBSFN_SubframeConfig_oms_1[] = { 3 };
static const ber_tlv_tag_t asn_DEF_EUTRA_MBSFN_SubframeConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_EUTRA_MBSFN_SubframeConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* radioframeAllocationPeriod */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* radioframeAllocationOffset */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* subframeAllocation1 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* subframeAllocation2 */
};
asn_SEQUENCE_specifics_t asn_SPC_EUTRA_MBSFN_SubframeConfig_specs_1 = {
	sizeof(struct EUTRA_MBSFN_SubframeConfig),
	offsetof(struct EUTRA_MBSFN_SubframeConfig, _asn_ctx),
	asn_MAP_EUTRA_MBSFN_SubframeConfig_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_EUTRA_MBSFN_SubframeConfig_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_EUTRA_MBSFN_SubframeConfig = {
	"EUTRA-MBSFN-SubframeConfig",
	"EUTRA-MBSFN-SubframeConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_EUTRA_MBSFN_SubframeConfig_tags_1,
	sizeof(asn_DEF_EUTRA_MBSFN_SubframeConfig_tags_1)
		/sizeof(asn_DEF_EUTRA_MBSFN_SubframeConfig_tags_1[0]), /* 1 */
	asn_DEF_EUTRA_MBSFN_SubframeConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_EUTRA_MBSFN_SubframeConfig_tags_1)
		/sizeof(asn_DEF_EUTRA_MBSFN_SubframeConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_EUTRA_MBSFN_SubframeConfig_1,
	4,	/* Elements count */
	&asn_SPC_EUTRA_MBSFN_SubframeConfig_specs_1	/* Additional specs */
};

