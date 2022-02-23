/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./23_02_rrc1/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "DMRS-DownlinkConfig.h"

#include "PTRS-DownlinkConfig.h"
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
memb_scramblingID0_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 65535)) {
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
memb_scramblingID1_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 65535)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_dmrs_Type_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_dmrs_Type_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_dmrs_AdditionalPosition_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_dmrs_AdditionalPosition_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_maxLength_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_maxLength_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 0,  0,  0,  0 }	/* (0..0) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_phaseTrackingRS_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_phaseTrackingRS_constr_12 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_scramblingID0_constr_10 CC_NOTUSED = {
	{ 2, 1 }	/* (0..65535) */,
	-1};
static asn_per_constraints_t asn_PER_memb_scramblingID0_constr_10 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 16,  16,  0,  65535 }	/* (0..65535) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_scramblingID1_constr_11 CC_NOTUSED = {
	{ 2, 1 }	/* (0..65535) */,
	-1};
static asn_per_constraints_t asn_PER_memb_scramblingID1_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 16,  16,  0,  65535 }	/* (0..65535) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_dmrs_Type_value2enum_2[] = {
	{ 0,	5,	"type2" }
};
static const unsigned int asn_MAP_dmrs_Type_enum2value_2[] = {
	0	/* type2(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_dmrs_Type_specs_2 = {
	asn_MAP_dmrs_Type_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_dmrs_Type_enum2value_2,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_dmrs_Type_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_dmrs_Type_2 = {
	"dmrs-Type",
	"dmrs-Type",
	&asn_OP_NativeEnumerated,
	asn_DEF_dmrs_Type_tags_2,
	sizeof(asn_DEF_dmrs_Type_tags_2)
		/sizeof(asn_DEF_dmrs_Type_tags_2[0]) - 1, /* 1 */
	asn_DEF_dmrs_Type_tags_2,	/* Same as above */
	sizeof(asn_DEF_dmrs_Type_tags_2)
		/sizeof(asn_DEF_dmrs_Type_tags_2[0]), /* 2 */
	{ &asn_OER_type_dmrs_Type_constr_2, &asn_PER_type_dmrs_Type_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_dmrs_Type_specs_2	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_dmrs_AdditionalPosition_value2enum_4[] = {
	{ 0,	4,	"pos0" },
	{ 1,	4,	"pos1" },
	{ 2,	4,	"pos3" }
};
static const unsigned int asn_MAP_dmrs_AdditionalPosition_enum2value_4[] = {
	0,	/* pos0(0) */
	1,	/* pos1(1) */
	2	/* pos3(2) */
};
static const asn_INTEGER_specifics_t asn_SPC_dmrs_AdditionalPosition_specs_4 = {
	asn_MAP_dmrs_AdditionalPosition_value2enum_4,	/* "tag" => N; sorted by tag */
	asn_MAP_dmrs_AdditionalPosition_enum2value_4,	/* N => "tag"; sorted by N */
	3,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_dmrs_AdditionalPosition_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_dmrs_AdditionalPosition_4 = {
	"dmrs-AdditionalPosition",
	"dmrs-AdditionalPosition",
	&asn_OP_NativeEnumerated,
	asn_DEF_dmrs_AdditionalPosition_tags_4,
	sizeof(asn_DEF_dmrs_AdditionalPosition_tags_4)
		/sizeof(asn_DEF_dmrs_AdditionalPosition_tags_4[0]) - 1, /* 1 */
	asn_DEF_dmrs_AdditionalPosition_tags_4,	/* Same as above */
	sizeof(asn_DEF_dmrs_AdditionalPosition_tags_4)
		/sizeof(asn_DEF_dmrs_AdditionalPosition_tags_4[0]), /* 2 */
	{ &asn_OER_type_dmrs_AdditionalPosition_constr_4, &asn_PER_type_dmrs_AdditionalPosition_constr_4, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_dmrs_AdditionalPosition_specs_4	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_maxLength_value2enum_8[] = {
	{ 0,	4,	"len2" }
};
static const unsigned int asn_MAP_maxLength_enum2value_8[] = {
	0	/* len2(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_maxLength_specs_8 = {
	asn_MAP_maxLength_value2enum_8,	/* "tag" => N; sorted by tag */
	asn_MAP_maxLength_enum2value_8,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_maxLength_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_maxLength_8 = {
	"maxLength",
	"maxLength",
	&asn_OP_NativeEnumerated,
	asn_DEF_maxLength_tags_8,
	sizeof(asn_DEF_maxLength_tags_8)
		/sizeof(asn_DEF_maxLength_tags_8[0]) - 1, /* 1 */
	asn_DEF_maxLength_tags_8,	/* Same as above */
	sizeof(asn_DEF_maxLength_tags_8)
		/sizeof(asn_DEF_maxLength_tags_8[0]), /* 2 */
	{ &asn_OER_type_maxLength_constr_8, &asn_PER_type_maxLength_constr_8, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_maxLength_specs_8	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_phaseTrackingRS_12[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DMRS_DownlinkConfig__phaseTrackingRS, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct DMRS_DownlinkConfig__phaseTrackingRS, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PTRS_DownlinkConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_phaseTrackingRS_tag2el_12[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_phaseTrackingRS_specs_12 = {
	sizeof(struct DMRS_DownlinkConfig__phaseTrackingRS),
	offsetof(struct DMRS_DownlinkConfig__phaseTrackingRS, _asn_ctx),
	offsetof(struct DMRS_DownlinkConfig__phaseTrackingRS, present),
	sizeof(((struct DMRS_DownlinkConfig__phaseTrackingRS *)0)->present),
	asn_MAP_phaseTrackingRS_tag2el_12,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_phaseTrackingRS_12 = {
	"phaseTrackingRS",
	"phaseTrackingRS",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_phaseTrackingRS_constr_12, &asn_PER_type_phaseTrackingRS_constr_12, CHOICE_constraint },
	asn_MBR_phaseTrackingRS_12,
	2,	/* Elements count */
	&asn_SPC_phaseTrackingRS_specs_12	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_DMRS_DownlinkConfig_1[] = {
	{ ATF_POINTER, 6, offsetof(struct DMRS_DownlinkConfig, dmrs_Type),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_dmrs_Type_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dmrs-Type"
		},
	{ ATF_POINTER, 5, offsetof(struct DMRS_DownlinkConfig, dmrs_AdditionalPosition),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_dmrs_AdditionalPosition_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dmrs-AdditionalPosition"
		},
	{ ATF_POINTER, 4, offsetof(struct DMRS_DownlinkConfig, maxLength),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_maxLength_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxLength"
		},
	{ ATF_POINTER, 3, offsetof(struct DMRS_DownlinkConfig, scramblingID0),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_scramblingID0_constr_10, &asn_PER_memb_scramblingID0_constr_10,  memb_scramblingID0_constraint_1 },
		0, 0, /* No default value */
		"scramblingID0"
		},
	{ ATF_POINTER, 2, offsetof(struct DMRS_DownlinkConfig, scramblingID1),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_scramblingID1_constr_11, &asn_PER_memb_scramblingID1_constr_11,  memb_scramblingID1_constraint_1 },
		0, 0, /* No default value */
		"scramblingID1"
		},
	{ ATF_POINTER, 1, offsetof(struct DMRS_DownlinkConfig, phaseTrackingRS),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_phaseTrackingRS_12,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"phaseTrackingRS"
		},
};
static const int asn_MAP_DMRS_DownlinkConfig_oms_1[] = { 0, 1, 2, 3, 4, 5 };
static const ber_tlv_tag_t asn_DEF_DMRS_DownlinkConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DMRS_DownlinkConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* dmrs-Type */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* dmrs-AdditionalPosition */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* maxLength */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* scramblingID0 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* scramblingID1 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* phaseTrackingRS */
};
asn_SEQUENCE_specifics_t asn_SPC_DMRS_DownlinkConfig_specs_1 = {
	sizeof(struct DMRS_DownlinkConfig),
	offsetof(struct DMRS_DownlinkConfig, _asn_ctx),
	asn_MAP_DMRS_DownlinkConfig_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_DMRS_DownlinkConfig_oms_1,	/* Optional members */
	6, 0,	/* Root/Additions */
	6,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_DMRS_DownlinkConfig = {
	"DMRS-DownlinkConfig",
	"DMRS-DownlinkConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_DMRS_DownlinkConfig_tags_1,
	sizeof(asn_DEF_DMRS_DownlinkConfig_tags_1)
		/sizeof(asn_DEF_DMRS_DownlinkConfig_tags_1[0]), /* 1 */
	asn_DEF_DMRS_DownlinkConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_DMRS_DownlinkConfig_tags_1)
		/sizeof(asn_DEF_DMRS_DownlinkConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_DMRS_DownlinkConfig_1,
	6,	/* Elements count */
	&asn_SPC_DMRS_DownlinkConfig_specs_1	/* Additional specs */
};

