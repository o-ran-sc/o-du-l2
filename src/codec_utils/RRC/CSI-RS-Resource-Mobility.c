/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "CSI-RS-Resource-Mobility.h"

static int
memb_ms4_constraint_3(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 31)) {
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
memb_ms5_constraint_3(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 39)) {
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
memb_ms10_constraint_3(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 79)) {
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
memb_ms20_constraint_3(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 159)) {
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
memb_ms40_constraint_3(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 319)) {
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
memb_row1_constraint_12(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 4)) {
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
memb_row2_constraint_12(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 12)) {
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
memb_firstOFDMSymbolInTimeDomain_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 13)) {
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
memb_sequenceGenerationConfig_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1023)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_ms4_constr_4 CC_NOTUSED = {
	{ 1, 1 }	/* (0..31) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ms4_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5,  0,  31 }	/* (0..31) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_ms5_constr_5 CC_NOTUSED = {
	{ 1, 1 }	/* (0..39) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ms5_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 6,  6,  0,  39 }	/* (0..39) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_ms10_constr_6 CC_NOTUSED = {
	{ 1, 1 }	/* (0..79) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ms10_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  0,  79 }	/* (0..79) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_ms20_constr_7 CC_NOTUSED = {
	{ 1, 1 }	/* (0..159) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ms20_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  0,  159 }	/* (0..159) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_ms40_constr_8 CC_NOTUSED = {
	{ 2, 1 }	/* (0..319) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ms40_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  0,  319 }	/* (0..319) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_slotConfig_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_slotConfig_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  4 }	/* (0..4) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_row1_constr_13 CC_NOTUSED = {
	{ 0, 0 },
	4	/* (SIZE(4..4)) */};
static asn_per_constraints_t asn_PER_memb_row1_constr_13 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  4,  4 }	/* (SIZE(4..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_row2_constr_14 CC_NOTUSED = {
	{ 0, 0 },
	12	/* (SIZE(12..12)) */};
static asn_per_constraints_t asn_PER_memb_row2_constr_14 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  12,  12 }	/* (SIZE(12..12)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_frequencyDomainAllocation_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_frequencyDomainAllocation_constr_12 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_firstOFDMSymbolInTimeDomain_constr_15 CC_NOTUSED = {
	{ 1, 1 }	/* (0..13) */,
	-1};
static asn_per_constraints_t asn_PER_memb_firstOFDMSymbolInTimeDomain_constr_15 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  13 }	/* (0..13) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_sequenceGenerationConfig_constr_16 CC_NOTUSED = {
	{ 2, 1 }	/* (0..1023) */,
	-1};
static asn_per_constraints_t asn_PER_memb_sequenceGenerationConfig_constr_16 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 10,  10,  0,  1023 }	/* (0..1023) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_slotConfig_3[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__slotConfig, choice.ms4),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ms4_constr_4, &asn_PER_memb_ms4_constr_4,  memb_ms4_constraint_3 },
		0, 0, /* No default value */
		"ms4"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__slotConfig, choice.ms5),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ms5_constr_5, &asn_PER_memb_ms5_constr_5,  memb_ms5_constraint_3 },
		0, 0, /* No default value */
		"ms5"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__slotConfig, choice.ms10),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ms10_constr_6, &asn_PER_memb_ms10_constr_6,  memb_ms10_constraint_3 },
		0, 0, /* No default value */
		"ms10"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__slotConfig, choice.ms20),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ms20_constr_7, &asn_PER_memb_ms20_constr_7,  memb_ms20_constraint_3 },
		0, 0, /* No default value */
		"ms20"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__slotConfig, choice.ms40),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ms40_constr_8, &asn_PER_memb_ms40_constr_8,  memb_ms40_constraint_3 },
		0, 0, /* No default value */
		"ms40"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_slotConfig_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ms4 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ms5 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* ms10 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* ms20 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* ms40 */
};
static asn_CHOICE_specifics_t asn_SPC_slotConfig_specs_3 = {
	sizeof(struct CSI_RS_Resource_Mobility__slotConfig),
	offsetof(struct CSI_RS_Resource_Mobility__slotConfig, _asn_ctx),
	offsetof(struct CSI_RS_Resource_Mobility__slotConfig, present),
	sizeof(((struct CSI_RS_Resource_Mobility__slotConfig *)0)->present),
	asn_MAP_slotConfig_tag2el_3,
	5,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_slotConfig_3 = {
	"slotConfig",
	"slotConfig",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_slotConfig_constr_3, &asn_PER_type_slotConfig_constr_3, CHOICE_constraint },
	asn_MBR_slotConfig_3,
	5,	/* Elements count */
	&asn_SPC_slotConfig_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_associatedSSB_9[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__associatedSSB, ssb_Index),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SSB_Index,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-Index"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__associatedSSB, isQuasiColocated),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"isQuasiColocated"
		},
};
static const ber_tlv_tag_t asn_DEF_associatedSSB_tags_9[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_associatedSSB_tag2el_9[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ssb-Index */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* isQuasiColocated */
};
static asn_SEQUENCE_specifics_t asn_SPC_associatedSSB_specs_9 = {
	sizeof(struct CSI_RS_Resource_Mobility__associatedSSB),
	offsetof(struct CSI_RS_Resource_Mobility__associatedSSB, _asn_ctx),
	asn_MAP_associatedSSB_tag2el_9,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_associatedSSB_9 = {
	"associatedSSB",
	"associatedSSB",
	&asn_OP_SEQUENCE,
	asn_DEF_associatedSSB_tags_9,
	sizeof(asn_DEF_associatedSSB_tags_9)
		/sizeof(asn_DEF_associatedSSB_tags_9[0]) - 1, /* 1 */
	asn_DEF_associatedSSB_tags_9,	/* Same as above */
	sizeof(asn_DEF_associatedSSB_tags_9)
		/sizeof(asn_DEF_associatedSSB_tags_9[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_associatedSSB_9,
	2,	/* Elements count */
	&asn_SPC_associatedSSB_specs_9	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_frequencyDomainAllocation_12[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__frequencyDomainAllocation, choice.row1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_row1_constr_13, &asn_PER_memb_row1_constr_13,  memb_row1_constraint_12 },
		0, 0, /* No default value */
		"row1"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility__frequencyDomainAllocation, choice.row2),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_row2_constr_14, &asn_PER_memb_row2_constr_14,  memb_row2_constraint_12 },
		0, 0, /* No default value */
		"row2"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_frequencyDomainAllocation_tag2el_12[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* row1 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* row2 */
};
static asn_CHOICE_specifics_t asn_SPC_frequencyDomainAllocation_specs_12 = {
	sizeof(struct CSI_RS_Resource_Mobility__frequencyDomainAllocation),
	offsetof(struct CSI_RS_Resource_Mobility__frequencyDomainAllocation, _asn_ctx),
	offsetof(struct CSI_RS_Resource_Mobility__frequencyDomainAllocation, present),
	sizeof(((struct CSI_RS_Resource_Mobility__frequencyDomainAllocation *)0)->present),
	asn_MAP_frequencyDomainAllocation_tag2el_12,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_frequencyDomainAllocation_12 = {
	"frequencyDomainAllocation",
	"frequencyDomainAllocation",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_frequencyDomainAllocation_constr_12, &asn_PER_type_frequencyDomainAllocation_constr_12, CHOICE_constraint },
	asn_MBR_frequencyDomainAllocation_12,
	2,	/* Elements count */
	&asn_SPC_frequencyDomainAllocation_specs_12	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_CSI_RS_Resource_Mobility_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility, csi_RS_Index),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSI_RS_Index,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"csi-RS-Index"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility, slotConfig),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_slotConfig_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"slotConfig"
		},
	{ ATF_POINTER, 1, offsetof(struct CSI_RS_Resource_Mobility, associatedSSB),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_associatedSSB_9,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"associatedSSB"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility, frequencyDomainAllocation),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_frequencyDomainAllocation_12,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"frequencyDomainAllocation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility, firstOFDMSymbolInTimeDomain),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_firstOFDMSymbolInTimeDomain_constr_15, &asn_PER_memb_firstOFDMSymbolInTimeDomain_constr_15,  memb_firstOFDMSymbolInTimeDomain_constraint_1 },
		0, 0, /* No default value */
		"firstOFDMSymbolInTimeDomain"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CSI_RS_Resource_Mobility, sequenceGenerationConfig),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_sequenceGenerationConfig_constr_16, &asn_PER_memb_sequenceGenerationConfig_constr_16,  memb_sequenceGenerationConfig_constraint_1 },
		0, 0, /* No default value */
		"sequenceGenerationConfig"
		},
};
static const int asn_MAP_CSI_RS_Resource_Mobility_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_CSI_RS_Resource_Mobility_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CSI_RS_Resource_Mobility_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* csi-RS-Index */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* slotConfig */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* associatedSSB */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* frequencyDomainAllocation */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* firstOFDMSymbolInTimeDomain */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* sequenceGenerationConfig */
};
asn_SEQUENCE_specifics_t asn_SPC_CSI_RS_Resource_Mobility_specs_1 = {
	sizeof(struct CSI_RS_Resource_Mobility),
	offsetof(struct CSI_RS_Resource_Mobility, _asn_ctx),
	asn_MAP_CSI_RS_Resource_Mobility_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_CSI_RS_Resource_Mobility_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	6,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CSI_RS_Resource_Mobility = {
	"CSI-RS-Resource-Mobility",
	"CSI-RS-Resource-Mobility",
	&asn_OP_SEQUENCE,
	asn_DEF_CSI_RS_Resource_Mobility_tags_1,
	sizeof(asn_DEF_CSI_RS_Resource_Mobility_tags_1)
		/sizeof(asn_DEF_CSI_RS_Resource_Mobility_tags_1[0]), /* 1 */
	asn_DEF_CSI_RS_Resource_Mobility_tags_1,	/* Same as above */
	sizeof(asn_DEF_CSI_RS_Resource_Mobility_tags_1)
		/sizeof(asn_DEF_CSI_RS_Resource_Mobility_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_CSI_RS_Resource_Mobility_1,
	6,	/* Elements count */
	&asn_SPC_CSI_RS_Resource_Mobility_specs_1	/* Additional specs */
};

