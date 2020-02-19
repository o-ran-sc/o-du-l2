/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../mib_sib1.asn1"
 * 	`asn1c -D ./new_out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "MIB.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_systemFrameNumber_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_ssb_SubcarrierOffset_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 15)) {
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
memb_spare_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 1)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_subCarrierSpacingCommon_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_subCarrierSpacingCommon_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_dmrs_TypeA_Position_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_dmrs_TypeA_Position_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_cellBarred_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_cellBarred_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_intraFreqReselection_constr_14 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_intraFreqReselection_constr_14 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_systemFrameNumber_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	6	/* (SIZE(6..6)) */};
static asn_per_constraints_t asn_PER_memb_systemFrameNumber_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  6,  6 }	/* (SIZE(6..6)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_ssb_SubcarrierOffset_constr_6 CC_NOTUSED = {
	{ 1, 1 }	/* (0..15) */,
	-1};
static asn_per_constraints_t asn_PER_memb_ssb_SubcarrierOffset_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_spare_constr_17 CC_NOTUSED = {
	{ 0, 0 },
	1	/* (SIZE(1..1)) */};
static asn_per_constraints_t asn_PER_memb_spare_constr_17 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  1,  1 }	/* (SIZE(1..1)) */,
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_subCarrierSpacingCommon_value2enum_3[] = {
	{ 0,	9,	"scs15or60" },
	{ 1,	10,	"scs30or120" }
};
static const unsigned int asn_MAP_subCarrierSpacingCommon_enum2value_3[] = {
	0,	/* scs15or60(0) */
	1	/* scs30or120(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_subCarrierSpacingCommon_specs_3 = {
	asn_MAP_subCarrierSpacingCommon_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_subCarrierSpacingCommon_enum2value_3,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_subCarrierSpacingCommon_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_subCarrierSpacingCommon_3 = {
	"subCarrierSpacingCommon",
	"subCarrierSpacingCommon",
	&asn_OP_NativeEnumerated,
	asn_DEF_subCarrierSpacingCommon_tags_3,
	sizeof(asn_DEF_subCarrierSpacingCommon_tags_3)
		/sizeof(asn_DEF_subCarrierSpacingCommon_tags_3[0]) - 1, /* 1 */
	asn_DEF_subCarrierSpacingCommon_tags_3,	/* Same as above */
	sizeof(asn_DEF_subCarrierSpacingCommon_tags_3)
		/sizeof(asn_DEF_subCarrierSpacingCommon_tags_3[0]), /* 2 */
	{ &asn_OER_type_subCarrierSpacingCommon_constr_3, &asn_PER_type_subCarrierSpacingCommon_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_subCarrierSpacingCommon_specs_3	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_dmrs_TypeA_Position_value2enum_7[] = {
	{ 0,	4,	"pos2" },
	{ 1,	4,	"pos3" }
};
static const unsigned int asn_MAP_dmrs_TypeA_Position_enum2value_7[] = {
	0,	/* pos2(0) */
	1	/* pos3(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_dmrs_TypeA_Position_specs_7 = {
	asn_MAP_dmrs_TypeA_Position_value2enum_7,	/* "tag" => N; sorted by tag */
	asn_MAP_dmrs_TypeA_Position_enum2value_7,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_dmrs_TypeA_Position_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_dmrs_TypeA_Position_7 = {
	"dmrs-TypeA-Position",
	"dmrs-TypeA-Position",
	&asn_OP_NativeEnumerated,
	asn_DEF_dmrs_TypeA_Position_tags_7,
	sizeof(asn_DEF_dmrs_TypeA_Position_tags_7)
		/sizeof(asn_DEF_dmrs_TypeA_Position_tags_7[0]) - 1, /* 1 */
	asn_DEF_dmrs_TypeA_Position_tags_7,	/* Same as above */
	sizeof(asn_DEF_dmrs_TypeA_Position_tags_7)
		/sizeof(asn_DEF_dmrs_TypeA_Position_tags_7[0]), /* 2 */
	{ &asn_OER_type_dmrs_TypeA_Position_constr_7, &asn_PER_type_dmrs_TypeA_Position_constr_7, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_dmrs_TypeA_Position_specs_7	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_cellBarred_value2enum_11[] = {
	{ 0,	6,	"barred" },
	{ 1,	9,	"notBarred" }
};
static const unsigned int asn_MAP_cellBarred_enum2value_11[] = {
	0,	/* barred(0) */
	1	/* notBarred(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_cellBarred_specs_11 = {
	asn_MAP_cellBarred_value2enum_11,	/* "tag" => N; sorted by tag */
	asn_MAP_cellBarred_enum2value_11,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_cellBarred_tags_11[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_cellBarred_11 = {
	"cellBarred",
	"cellBarred",
	&asn_OP_NativeEnumerated,
	asn_DEF_cellBarred_tags_11,
	sizeof(asn_DEF_cellBarred_tags_11)
		/sizeof(asn_DEF_cellBarred_tags_11[0]) - 1, /* 1 */
	asn_DEF_cellBarred_tags_11,	/* Same as above */
	sizeof(asn_DEF_cellBarred_tags_11)
		/sizeof(asn_DEF_cellBarred_tags_11[0]), /* 2 */
	{ &asn_OER_type_cellBarred_constr_11, &asn_PER_type_cellBarred_constr_11, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_cellBarred_specs_11	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_intraFreqReselection_value2enum_14[] = {
	{ 0,	7,	"allowed" },
	{ 1,	10,	"notAllowed" }
};
static const unsigned int asn_MAP_intraFreqReselection_enum2value_14[] = {
	0,	/* allowed(0) */
	1	/* notAllowed(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_intraFreqReselection_specs_14 = {
	asn_MAP_intraFreqReselection_value2enum_14,	/* "tag" => N; sorted by tag */
	asn_MAP_intraFreqReselection_enum2value_14,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_intraFreqReselection_tags_14[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_intraFreqReselection_14 = {
	"intraFreqReselection",
	"intraFreqReselection",
	&asn_OP_NativeEnumerated,
	asn_DEF_intraFreqReselection_tags_14,
	sizeof(asn_DEF_intraFreqReselection_tags_14)
		/sizeof(asn_DEF_intraFreqReselection_tags_14[0]) - 1, /* 1 */
	asn_DEF_intraFreqReselection_tags_14,	/* Same as above */
	sizeof(asn_DEF_intraFreqReselection_tags_14)
		/sizeof(asn_DEF_intraFreqReselection_tags_14[0]), /* 2 */
	{ &asn_OER_type_intraFreqReselection_constr_14, &asn_PER_type_intraFreqReselection_constr_14, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_intraFreqReselection_specs_14	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_MIB_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, systemFrameNumber),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_systemFrameNumber_constr_2, &asn_PER_memb_systemFrameNumber_constr_2,  memb_systemFrameNumber_constraint_1 },
		0, 0, /* No default value */
		"systemFrameNumber"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, subCarrierSpacingCommon),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_subCarrierSpacingCommon_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"subCarrierSpacingCommon"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, ssb_SubcarrierOffset),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_ssb_SubcarrierOffset_constr_6, &asn_PER_memb_ssb_SubcarrierOffset_constr_6,  memb_ssb_SubcarrierOffset_constraint_1 },
		0, 0, /* No default value */
		"ssb-SubcarrierOffset"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, dmrs_TypeA_Position),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_dmrs_TypeA_Position_7,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dmrs-TypeA-Position"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, pdcch_ConfigSIB1),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PDCCH_ConfigSIB1,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pdcch-ConfigSIB1"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, cellBarred),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_cellBarred_11,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cellBarred"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, intraFreqReselection),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_intraFreqReselection_14,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"intraFreqReselection"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MIB, spare),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ &asn_OER_memb_spare_constr_17, &asn_PER_memb_spare_constr_17,  memb_spare_constraint_1 },
		0, 0, /* No default value */
		"spare"
		},
};
static const ber_tlv_tag_t asn_DEF_MIB_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MIB_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* systemFrameNumber */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* subCarrierSpacingCommon */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* ssb-SubcarrierOffset */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* dmrs-TypeA-Position */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* pdcch-ConfigSIB1 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* cellBarred */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* intraFreqReselection */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* spare */
};
static asn_SEQUENCE_specifics_t asn_SPC_MIB_specs_1 = {
	sizeof(struct MIB),
	offsetof(struct MIB, _asn_ctx),
	asn_MAP_MIB_tag2el_1,
	8,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MIB = {
	"MIB",
	"MIB",
	&asn_OP_SEQUENCE,
	asn_DEF_MIB_tags_1,
	sizeof(asn_DEF_MIB_tags_1)
		/sizeof(asn_DEF_MIB_tags_1[0]), /* 1 */
	asn_DEF_MIB_tags_1,	/* Same as above */
	sizeof(asn_DEF_MIB_tags_1)
		/sizeof(asn_DEF_MIB_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_MIB_1,
	8,	/* Elements count */
	&asn_SPC_MIB_specs_1	/* Additional specs */
};

