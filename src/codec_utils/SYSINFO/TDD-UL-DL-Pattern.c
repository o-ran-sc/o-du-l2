/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../mib_sib1.asn1"
 * 	`asn1c -D ./new_out -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "TDD-UL-DL-Pattern.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_nrofDownlinkSlots_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 320)) {
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
memb_nrofDownlinkSymbols_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_nrofUplinkSlots_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 320)) {
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
memb_nrofUplinkSymbols_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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

static asn_oer_constraints_t asn_OER_type_dl_UL_TransmissionPeriodicity_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_dl_UL_TransmissionPeriodicity_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_dl_UL_TransmissionPeriodicity_v1530_constr_17 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_dl_UL_TransmissionPeriodicity_v1530_constr_17 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_nrofDownlinkSlots_constr_11 CC_NOTUSED = {
	{ 2, 1 }	/* (0..320) */,
	-1};
static asn_per_constraints_t asn_PER_memb_nrofDownlinkSlots_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  0,  320 }	/* (0..320) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_nrofDownlinkSymbols_constr_12 CC_NOTUSED = {
	{ 1, 1 }	/* (0..13) */,
	-1};
static asn_per_constraints_t asn_PER_memb_nrofDownlinkSymbols_constr_12 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  13 }	/* (0..13) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_nrofUplinkSlots_constr_13 CC_NOTUSED = {
	{ 2, 1 }	/* (0..320) */,
	-1};
static asn_per_constraints_t asn_PER_memb_nrofUplinkSlots_constr_13 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  0,  320 }	/* (0..320) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_nrofUplinkSymbols_constr_14 CC_NOTUSED = {
	{ 1, 1 }	/* (0..13) */,
	-1};
static asn_per_constraints_t asn_PER_memb_nrofUplinkSymbols_constr_14 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  13 }	/* (0..13) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_dl_UL_TransmissionPeriodicity_value2enum_2[] = {
	{ 0,	5,	"ms0p5" },
	{ 1,	7,	"ms0p625" },
	{ 2,	3,	"ms1" },
	{ 3,	6,	"ms1p25" },
	{ 4,	3,	"ms2" },
	{ 5,	5,	"ms2p5" },
	{ 6,	3,	"ms5" },
	{ 7,	4,	"ms10" }
};
static const unsigned int asn_MAP_dl_UL_TransmissionPeriodicity_enum2value_2[] = {
	0,	/* ms0p5(0) */
	1,	/* ms0p625(1) */
	2,	/* ms1(2) */
	7,	/* ms10(7) */
	3,	/* ms1p25(3) */
	4,	/* ms2(4) */
	5,	/* ms2p5(5) */
	6	/* ms5(6) */
};
static const asn_INTEGER_specifics_t asn_SPC_dl_UL_TransmissionPeriodicity_specs_2 = {
	asn_MAP_dl_UL_TransmissionPeriodicity_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_dl_UL_TransmissionPeriodicity_enum2value_2,	/* N => "tag"; sorted by N */
	8,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_dl_UL_TransmissionPeriodicity_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_dl_UL_TransmissionPeriodicity_2 = {
	"dl-UL-TransmissionPeriodicity",
	"dl-UL-TransmissionPeriodicity",
	&asn_OP_NativeEnumerated,
	asn_DEF_dl_UL_TransmissionPeriodicity_tags_2,
	sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_tags_2)
		/sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_tags_2[0]) - 1, /* 1 */
	asn_DEF_dl_UL_TransmissionPeriodicity_tags_2,	/* Same as above */
	sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_tags_2)
		/sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_tags_2[0]), /* 2 */
	{ &asn_OER_type_dl_UL_TransmissionPeriodicity_constr_2, &asn_PER_type_dl_UL_TransmissionPeriodicity_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_dl_UL_TransmissionPeriodicity_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_dl_UL_TransmissionPeriodicity_v1530_value2enum_17[] = {
	{ 0,	3,	"ms3" },
	{ 1,	3,	"ms4" }
};
static const unsigned int asn_MAP_dl_UL_TransmissionPeriodicity_v1530_enum2value_17[] = {
	0,	/* ms3(0) */
	1	/* ms4(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_dl_UL_TransmissionPeriodicity_v1530_specs_17 = {
	asn_MAP_dl_UL_TransmissionPeriodicity_v1530_value2enum_17,	/* "tag" => N; sorted by tag */
	asn_MAP_dl_UL_TransmissionPeriodicity_v1530_enum2value_17,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_dl_UL_TransmissionPeriodicity_v1530_tags_17[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_dl_UL_TransmissionPeriodicity_v1530_17 = {
	"dl-UL-TransmissionPeriodicity-v1530",
	"dl-UL-TransmissionPeriodicity-v1530",
	&asn_OP_NativeEnumerated,
	asn_DEF_dl_UL_TransmissionPeriodicity_v1530_tags_17,
	sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_v1530_tags_17)
		/sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_v1530_tags_17[0]) - 1, /* 1 */
	asn_DEF_dl_UL_TransmissionPeriodicity_v1530_tags_17,	/* Same as above */
	sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_v1530_tags_17)
		/sizeof(asn_DEF_dl_UL_TransmissionPeriodicity_v1530_tags_17[0]), /* 2 */
	{ &asn_OER_type_dl_UL_TransmissionPeriodicity_v1530_constr_17, &asn_PER_type_dl_UL_TransmissionPeriodicity_v1530_constr_17, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_dl_UL_TransmissionPeriodicity_v1530_specs_17	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ext1_16[] = {
	{ ATF_POINTER, 1, offsetof(struct TDD_UL_DL_Pattern__ext1, dl_UL_TransmissionPeriodicity_v1530),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_dl_UL_TransmissionPeriodicity_v1530_17,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dl-UL-TransmissionPeriodicity-v1530"
		},
};
static const int asn_MAP_ext1_oms_16[] = { 0 };
static const ber_tlv_tag_t asn_DEF_ext1_tags_16[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ext1_tag2el_16[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* dl-UL-TransmissionPeriodicity-v1530 */
};
static asn_SEQUENCE_specifics_t asn_SPC_ext1_specs_16 = {
	sizeof(struct TDD_UL_DL_Pattern__ext1),
	offsetof(struct TDD_UL_DL_Pattern__ext1, _asn_ctx),
	asn_MAP_ext1_tag2el_16,
	1,	/* Count of tags in the map */
	asn_MAP_ext1_oms_16,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ext1_16 = {
	"ext1",
	"ext1",
	&asn_OP_SEQUENCE,
	asn_DEF_ext1_tags_16,
	sizeof(asn_DEF_ext1_tags_16)
		/sizeof(asn_DEF_ext1_tags_16[0]) - 1, /* 1 */
	asn_DEF_ext1_tags_16,	/* Same as above */
	sizeof(asn_DEF_ext1_tags_16)
		/sizeof(asn_DEF_ext1_tags_16[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ext1_16,
	1,	/* Elements count */
	&asn_SPC_ext1_specs_16	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_TDD_UL_DL_Pattern_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_Pattern, dl_UL_TransmissionPeriodicity),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_dl_UL_TransmissionPeriodicity_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dl-UL-TransmissionPeriodicity"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_Pattern, nrofDownlinkSlots),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_nrofDownlinkSlots_constr_11, &asn_PER_memb_nrofDownlinkSlots_constr_11,  memb_nrofDownlinkSlots_constraint_1 },
		0, 0, /* No default value */
		"nrofDownlinkSlots"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_Pattern, nrofDownlinkSymbols),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_nrofDownlinkSymbols_constr_12, &asn_PER_memb_nrofDownlinkSymbols_constr_12,  memb_nrofDownlinkSymbols_constraint_1 },
		0, 0, /* No default value */
		"nrofDownlinkSymbols"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_Pattern, nrofUplinkSlots),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_nrofUplinkSlots_constr_13, &asn_PER_memb_nrofUplinkSlots_constr_13,  memb_nrofUplinkSlots_constraint_1 },
		0, 0, /* No default value */
		"nrofUplinkSlots"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TDD_UL_DL_Pattern, nrofUplinkSymbols),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_nrofUplinkSymbols_constr_14, &asn_PER_memb_nrofUplinkSymbols_constr_14,  memb_nrofUplinkSymbols_constraint_1 },
		0, 0, /* No default value */
		"nrofUplinkSymbols"
		},
	{ ATF_POINTER, 1, offsetof(struct TDD_UL_DL_Pattern, ext1),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		0,
		&asn_DEF_ext1_16,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ext1"
		},
};
static const int asn_MAP_TDD_UL_DL_Pattern_oms_1[] = { 5 };
static const ber_tlv_tag_t asn_DEF_TDD_UL_DL_Pattern_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TDD_UL_DL_Pattern_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* dl-UL-TransmissionPeriodicity */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* nrofDownlinkSlots */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* nrofDownlinkSymbols */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* nrofUplinkSlots */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* nrofUplinkSymbols */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* ext1 */
};
asn_SEQUENCE_specifics_t asn_SPC_TDD_UL_DL_Pattern_specs_1 = {
	sizeof(struct TDD_UL_DL_Pattern),
	offsetof(struct TDD_UL_DL_Pattern, _asn_ctx),
	asn_MAP_TDD_UL_DL_Pattern_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_TDD_UL_DL_Pattern_oms_1,	/* Optional members */
	0, 1,	/* Root/Additions */
	5,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_TDD_UL_DL_Pattern = {
	"TDD-UL-DL-Pattern",
	"TDD-UL-DL-Pattern",
	&asn_OP_SEQUENCE,
	asn_DEF_TDD_UL_DL_Pattern_tags_1,
	sizeof(asn_DEF_TDD_UL_DL_Pattern_tags_1)
		/sizeof(asn_DEF_TDD_UL_DL_Pattern_tags_1[0]), /* 1 */
	asn_DEF_TDD_UL_DL_Pattern_tags_1,	/* Same as above */
	sizeof(asn_DEF_TDD_UL_DL_Pattern_tags_1)
		/sizeof(asn_DEF_TDD_UL_DL_Pattern_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_TDD_UL_DL_Pattern_1,
	6,	/* Elements count */
	&asn_SPC_TDD_UL_DL_Pattern_specs_1	/* Additional specs */
};

