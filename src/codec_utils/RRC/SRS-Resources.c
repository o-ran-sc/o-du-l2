/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SRS-Resources.h"

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
memb_maxNumberAperiodicSRS_PerBWP_PerSlot_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 6)) {
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
memb_maxNumberPeriodicSRS_PerBWP_PerSlot_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 6)) {
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
memb_maxNumberSP_SRS_PerBWP_PerSlot_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 6)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_maxNumberAperiodicSRS_PerBWP_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxNumberAperiodicSRS_PerBWP_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  4 }	/* (0..4) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_maxNumberPeriodicSRS_PerBWP_constr_9 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxNumberPeriodicSRS_PerBWP_constr_9 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  4 }	/* (0..4) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_maxNumberSemiPersitentSRS_PerBWP_constr_16 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxNumberSemiPersitentSRS_PerBWP_constr_16 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  4 }	/* (0..4) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_maxNumberSRS_Ports_PerResource_constr_23 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxNumberSRS_Ports_PerResource_constr_23 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_maxNumberAperiodicSRS_PerBWP_PerSlot_constr_8 CC_NOTUSED = {
	{ 1, 1 }	/* (1..6) */,
	-1};
static asn_per_constraints_t asn_PER_memb_maxNumberAperiodicSRS_PerBWP_PerSlot_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  1,  6 }	/* (1..6) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_maxNumberPeriodicSRS_PerBWP_PerSlot_constr_15 CC_NOTUSED = {
	{ 1, 1 }	/* (1..6) */,
	-1};
static asn_per_constraints_t asn_PER_memb_maxNumberPeriodicSRS_PerBWP_PerSlot_constr_15 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  1,  6 }	/* (1..6) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_maxNumberSP_SRS_PerBWP_PerSlot_constr_22 CC_NOTUSED = {
	{ 1, 1 }	/* (1..6) */,
	-1};
static asn_per_constraints_t asn_PER_memb_maxNumberSP_SRS_PerBWP_PerSlot_constr_22 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  1,  6 }	/* (1..6) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_maxNumberAperiodicSRS_PerBWP_value2enum_2[] = {
	{ 0,	2,	"n1" },
	{ 1,	2,	"n2" },
	{ 2,	2,	"n4" },
	{ 3,	2,	"n8" },
	{ 4,	3,	"n16" }
};
static const unsigned int asn_MAP_maxNumberAperiodicSRS_PerBWP_enum2value_2[] = {
	0,	/* n1(0) */
	4,	/* n16(4) */
	1,	/* n2(1) */
	2,	/* n4(2) */
	3	/* n8(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxNumberAperiodicSRS_PerBWP_specs_2 = {
	asn_MAP_maxNumberAperiodicSRS_PerBWP_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_maxNumberAperiodicSRS_PerBWP_enum2value_2,	/* N => "tag"; sorted by N */
	5,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxNumberAperiodicSRS_PerBWP_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxNumberAperiodicSRS_PerBWP_2 = {
	"maxNumberAperiodicSRS-PerBWP",
	"maxNumberAperiodicSRS-PerBWP",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxNumberAperiodicSRS_PerBWP_tags_2,
	sizeof(asn_DEF_maxNumberAperiodicSRS_PerBWP_tags_2)
		/sizeof(asn_DEF_maxNumberAperiodicSRS_PerBWP_tags_2[0]) - 1, /* 1 */
	asn_DEF_maxNumberAperiodicSRS_PerBWP_tags_2,	/* Same as above */
	sizeof(asn_DEF_maxNumberAperiodicSRS_PerBWP_tags_2)
		/sizeof(asn_DEF_maxNumberAperiodicSRS_PerBWP_tags_2[0]), /* 2 */
	{ &asn_OER_type_maxNumberAperiodicSRS_PerBWP_constr_2, &asn_PER_type_maxNumberAperiodicSRS_PerBWP_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxNumberAperiodicSRS_PerBWP_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_maxNumberPeriodicSRS_PerBWP_value2enum_9[] = {
	{ 0,	2,	"n1" },
	{ 1,	2,	"n2" },
	{ 2,	2,	"n4" },
	{ 3,	2,	"n8" },
	{ 4,	3,	"n16" }
};
static const unsigned int asn_MAP_maxNumberPeriodicSRS_PerBWP_enum2value_9[] = {
	0,	/* n1(0) */
	4,	/* n16(4) */
	1,	/* n2(1) */
	2,	/* n4(2) */
	3	/* n8(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxNumberPeriodicSRS_PerBWP_specs_9 = {
	asn_MAP_maxNumberPeriodicSRS_PerBWP_value2enum_9,	/* "tag" => N; sorted by tag */
	asn_MAP_maxNumberPeriodicSRS_PerBWP_enum2value_9,	/* N => "tag"; sorted by N */
	5,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxNumberPeriodicSRS_PerBWP_tags_9[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxNumberPeriodicSRS_PerBWP_9 = {
	"maxNumberPeriodicSRS-PerBWP",
	"maxNumberPeriodicSRS-PerBWP",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxNumberPeriodicSRS_PerBWP_tags_9,
	sizeof(asn_DEF_maxNumberPeriodicSRS_PerBWP_tags_9)
		/sizeof(asn_DEF_maxNumberPeriodicSRS_PerBWP_tags_9[0]) - 1, /* 1 */
	asn_DEF_maxNumberPeriodicSRS_PerBWP_tags_9,	/* Same as above */
	sizeof(asn_DEF_maxNumberPeriodicSRS_PerBWP_tags_9)
		/sizeof(asn_DEF_maxNumberPeriodicSRS_PerBWP_tags_9[0]), /* 2 */
	{ &asn_OER_type_maxNumberPeriodicSRS_PerBWP_constr_9, &asn_PER_type_maxNumberPeriodicSRS_PerBWP_constr_9, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxNumberPeriodicSRS_PerBWP_specs_9	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_maxNumberSemiPersitentSRS_PerBWP_value2enum_16[] = {
	{ 0,	2,	"n1" },
	{ 1,	2,	"n2" },
	{ 2,	2,	"n4" },
	{ 3,	2,	"n8" },
	{ 4,	3,	"n16" }
};
static const unsigned int asn_MAP_maxNumberSemiPersitentSRS_PerBWP_enum2value_16[] = {
	0,	/* n1(0) */
	4,	/* n16(4) */
	1,	/* n2(1) */
	2,	/* n4(2) */
	3	/* n8(3) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxNumberSemiPersitentSRS_PerBWP_specs_16 = {
	asn_MAP_maxNumberSemiPersitentSRS_PerBWP_value2enum_16,	/* "tag" => N; sorted by tag */
	asn_MAP_maxNumberSemiPersitentSRS_PerBWP_enum2value_16,	/* N => "tag"; sorted by N */
	5,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxNumberSemiPersitentSRS_PerBWP_tags_16[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxNumberSemiPersitentSRS_PerBWP_16 = {
	"maxNumberSemiPersitentSRS-PerBWP",
	"maxNumberSemiPersitentSRS-PerBWP",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxNumberSemiPersitentSRS_PerBWP_tags_16,
	sizeof(asn_DEF_maxNumberSemiPersitentSRS_PerBWP_tags_16)
		/sizeof(asn_DEF_maxNumberSemiPersitentSRS_PerBWP_tags_16[0]) - 1, /* 1 */
	asn_DEF_maxNumberSemiPersitentSRS_PerBWP_tags_16,	/* Same as above */
	sizeof(asn_DEF_maxNumberSemiPersitentSRS_PerBWP_tags_16)
		/sizeof(asn_DEF_maxNumberSemiPersitentSRS_PerBWP_tags_16[0]), /* 2 */
	{ &asn_OER_type_maxNumberSemiPersitentSRS_PerBWP_constr_16, &asn_PER_type_maxNumberSemiPersitentSRS_PerBWP_constr_16, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxNumberSemiPersitentSRS_PerBWP_specs_16	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_maxNumberSRS_Ports_PerResource_value2enum_23[] = {
	{ 0,	2,	"n1" },
	{ 1,	2,	"n2" },
	{ 2,	2,	"n4" }
};
static const unsigned int asn_MAP_maxNumberSRS_Ports_PerResource_enum2value_23[] = {
	0,	/* n1(0) */
	1,	/* n2(1) */
	2	/* n4(2) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxNumberSRS_Ports_PerResource_specs_23 = {
	asn_MAP_maxNumberSRS_Ports_PerResource_value2enum_23,	/* "tag" => N; sorted by tag */
	asn_MAP_maxNumberSRS_Ports_PerResource_enum2value_23,	/* N => "tag"; sorted by N */
	3,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxNumberSRS_Ports_PerResource_tags_23[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxNumberSRS_Ports_PerResource_23 = {
	"maxNumberSRS-Ports-PerResource",
	"maxNumberSRS-Ports-PerResource",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxNumberSRS_Ports_PerResource_tags_23,
	sizeof(asn_DEF_maxNumberSRS_Ports_PerResource_tags_23)
		/sizeof(asn_DEF_maxNumberSRS_Ports_PerResource_tags_23[0]) - 1, /* 1 */
	asn_DEF_maxNumberSRS_Ports_PerResource_tags_23,	/* Same as above */
	sizeof(asn_DEF_maxNumberSRS_Ports_PerResource_tags_23)
		/sizeof(asn_DEF_maxNumberSRS_Ports_PerResource_tags_23[0]), /* 2 */
	{ &asn_OER_type_maxNumberSRS_Ports_PerResource_constr_23, &asn_PER_type_maxNumberSRS_Ports_PerResource_constr_23, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxNumberSRS_Ports_PerResource_specs_23	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SRS_Resources_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_Resources, maxNumberAperiodicSRS_PerBWP),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxNumberAperiodicSRS_PerBWP_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberAperiodicSRS-PerBWP"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_Resources, maxNumberAperiodicSRS_PerBWP_PerSlot),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_maxNumberAperiodicSRS_PerBWP_PerSlot_constr_8, &asn_PER_memb_maxNumberAperiodicSRS_PerBWP_PerSlot_constr_8,  memb_maxNumberAperiodicSRS_PerBWP_PerSlot_constraint_1 },
		0, 0, /* No default value */
		"maxNumberAperiodicSRS-PerBWP-PerSlot"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_Resources, maxNumberPeriodicSRS_PerBWP),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxNumberPeriodicSRS_PerBWP_9,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberPeriodicSRS-PerBWP"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_Resources, maxNumberPeriodicSRS_PerBWP_PerSlot),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_maxNumberPeriodicSRS_PerBWP_PerSlot_constr_15, &asn_PER_memb_maxNumberPeriodicSRS_PerBWP_PerSlot_constr_15,  memb_maxNumberPeriodicSRS_PerBWP_PerSlot_constraint_1 },
		0, 0, /* No default value */
		"maxNumberPeriodicSRS-PerBWP-PerSlot"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_Resources, maxNumberSemiPersitentSRS_PerBWP),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxNumberSemiPersitentSRS_PerBWP_16,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberSemiPersitentSRS-PerBWP"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_Resources, maxNumberSP_SRS_PerBWP_PerSlot),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_maxNumberSP_SRS_PerBWP_PerSlot_constr_22, &asn_PER_memb_maxNumberSP_SRS_PerBWP_PerSlot_constr_22,  memb_maxNumberSP_SRS_PerBWP_PerSlot_constraint_1 },
		0, 0, /* No default value */
		"maxNumberSP-SRS-PerBWP-PerSlot"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SRS_Resources, maxNumberSRS_Ports_PerResource),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxNumberSRS_Ports_PerResource_23,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxNumberSRS-Ports-PerResource"
		},
};
static const ber_tlv_tag_t asn_DEF_SRS_Resources_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SRS_Resources_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* maxNumberAperiodicSRS-PerBWP */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* maxNumberAperiodicSRS-PerBWP-PerSlot */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* maxNumberPeriodicSRS-PerBWP */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* maxNumberPeriodicSRS-PerBWP-PerSlot */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* maxNumberSemiPersitentSRS-PerBWP */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* maxNumberSP-SRS-PerBWP-PerSlot */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* maxNumberSRS-Ports-PerResource */
};
asn_SEQUENCE_specifics_t asn_SPC_SRS_Resources_specs_1 = {
	sizeof(struct SRS_Resources),
	offsetof(struct SRS_Resources, _asn_ctx),
	asn_MAP_SRS_Resources_tag2el_1,
	7,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SRS_Resources = {
	"SRS-Resources",
	"SRS-Resources",
	&asn_OP_SEQUENCE,
	asn_DEF_SRS_Resources_tags_1,
	sizeof(asn_DEF_SRS_Resources_tags_1)
		/sizeof(asn_DEF_SRS_Resources_tags_1[0]), /* 1 */
	asn_DEF_SRS_Resources_tags_1,	/* Same as above */
	sizeof(asn_DEF_SRS_Resources_tags_1)
		/sizeof(asn_DEF_SRS_Resources_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SRS_Resources_1,
	7,	/* Elements count */
	&asn_SPC_SRS_Resources_specs_1	/* Additional specs */
};

