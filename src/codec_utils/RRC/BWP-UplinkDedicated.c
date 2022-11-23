/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "BWP-UplinkDedicated.h"

#include "PUCCH-Config.h"
#include "PUSCH-Config.h"
#include "ConfiguredGrantConfig.h"
#include "SRS-Config.h"
#include "BeamFailureRecoveryConfig.h"
static asn_oer_constraints_t asn_OER_type_pucch_Config_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_pucch_Config_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_pusch_Config_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_pusch_Config_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_configuredGrantConfig_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_configuredGrantConfig_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_srs_Config_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_srs_Config_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_beamFailureRecoveryConfig_constr_14 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_beamFailureRecoveryConfig_constr_14 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_pucch_Config_2[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BWP_UplinkDedicated__pucch_Config, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct BWP_UplinkDedicated__pucch_Config, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_Config,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_pucch_Config_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_pucch_Config_specs_2 = {
	sizeof(struct BWP_UplinkDedicated__pucch_Config),
	offsetof(struct BWP_UplinkDedicated__pucch_Config, _asn_ctx),
	offsetof(struct BWP_UplinkDedicated__pucch_Config, present),
	sizeof(((struct BWP_UplinkDedicated__pucch_Config *)0)->present),
	asn_MAP_pucch_Config_tag2el_2,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pucch_Config_2 = {
	"pucch-Config",
	"pucch-Config",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_pucch_Config_constr_2, &asn_PER_type_pucch_Config_constr_2, CHOICE_constraint },
	asn_MBR_pucch_Config_2,
	2,	/* Elements count */
	&asn_SPC_pucch_Config_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_pusch_Config_5[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BWP_UplinkDedicated__pusch_Config, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct BWP_UplinkDedicated__pusch_Config, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUSCH_Config,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_pusch_Config_tag2el_5[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_pusch_Config_specs_5 = {
	sizeof(struct BWP_UplinkDedicated__pusch_Config),
	offsetof(struct BWP_UplinkDedicated__pusch_Config, _asn_ctx),
	offsetof(struct BWP_UplinkDedicated__pusch_Config, present),
	sizeof(((struct BWP_UplinkDedicated__pusch_Config *)0)->present),
	asn_MAP_pusch_Config_tag2el_5,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pusch_Config_5 = {
	"pusch-Config",
	"pusch-Config",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_pusch_Config_constr_5, &asn_PER_type_pusch_Config_constr_5, CHOICE_constraint },
	asn_MBR_pusch_Config_5,
	2,	/* Elements count */
	&asn_SPC_pusch_Config_specs_5	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_configuredGrantConfig_8[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BWP_UplinkDedicated__configuredGrantConfig, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct BWP_UplinkDedicated__configuredGrantConfig, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ConfiguredGrantConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_configuredGrantConfig_tag2el_8[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_configuredGrantConfig_specs_8 = {
	sizeof(struct BWP_UplinkDedicated__configuredGrantConfig),
	offsetof(struct BWP_UplinkDedicated__configuredGrantConfig, _asn_ctx),
	offsetof(struct BWP_UplinkDedicated__configuredGrantConfig, present),
	sizeof(((struct BWP_UplinkDedicated__configuredGrantConfig *)0)->present),
	asn_MAP_configuredGrantConfig_tag2el_8,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_configuredGrantConfig_8 = {
	"configuredGrantConfig",
	"configuredGrantConfig",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_configuredGrantConfig_constr_8, &asn_PER_type_configuredGrantConfig_constr_8, CHOICE_constraint },
	asn_MBR_configuredGrantConfig_8,
	2,	/* Elements count */
	&asn_SPC_configuredGrantConfig_specs_8	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_srs_Config_11[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BWP_UplinkDedicated__srs_Config, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct BWP_UplinkDedicated__srs_Config, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SRS_Config,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_srs_Config_tag2el_11[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_srs_Config_specs_11 = {
	sizeof(struct BWP_UplinkDedicated__srs_Config),
	offsetof(struct BWP_UplinkDedicated__srs_Config, _asn_ctx),
	offsetof(struct BWP_UplinkDedicated__srs_Config, present),
	sizeof(((struct BWP_UplinkDedicated__srs_Config *)0)->present),
	asn_MAP_srs_Config_tag2el_11,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_srs_Config_11 = {
	"srs-Config",
	"srs-Config",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_srs_Config_constr_11, &asn_PER_type_srs_Config_constr_11, CHOICE_constraint },
	asn_MBR_srs_Config_11,
	2,	/* Elements count */
	&asn_SPC_srs_Config_specs_11	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_beamFailureRecoveryConfig_14[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BWP_UplinkDedicated__beamFailureRecoveryConfig, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct BWP_UplinkDedicated__beamFailureRecoveryConfig, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BeamFailureRecoveryConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_beamFailureRecoveryConfig_tag2el_14[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_beamFailureRecoveryConfig_specs_14 = {
	sizeof(struct BWP_UplinkDedicated__beamFailureRecoveryConfig),
	offsetof(struct BWP_UplinkDedicated__beamFailureRecoveryConfig, _asn_ctx),
	offsetof(struct BWP_UplinkDedicated__beamFailureRecoveryConfig, present),
	sizeof(((struct BWP_UplinkDedicated__beamFailureRecoveryConfig *)0)->present),
	asn_MAP_beamFailureRecoveryConfig_tag2el_14,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_beamFailureRecoveryConfig_14 = {
	"beamFailureRecoveryConfig",
	"beamFailureRecoveryConfig",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_beamFailureRecoveryConfig_constr_14, &asn_PER_type_beamFailureRecoveryConfig_constr_14, CHOICE_constraint },
	asn_MBR_beamFailureRecoveryConfig_14,
	2,	/* Elements count */
	&asn_SPC_beamFailureRecoveryConfig_specs_14	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_BWP_UplinkDedicated_1[] = {
	{ ATF_POINTER, 5, offsetof(struct BWP_UplinkDedicated, pucch_Config),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_pucch_Config_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pucch-Config"
		},
	{ ATF_POINTER, 4, offsetof(struct BWP_UplinkDedicated, pusch_Config),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_pusch_Config_5,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pusch-Config"
		},
	{ ATF_POINTER, 3, offsetof(struct BWP_UplinkDedicated, configuredGrantConfig),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_configuredGrantConfig_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"configuredGrantConfig"
		},
	{ ATF_POINTER, 2, offsetof(struct BWP_UplinkDedicated, srs_Config),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_srs_Config_11,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"srs-Config"
		},
	{ ATF_POINTER, 1, offsetof(struct BWP_UplinkDedicated, beamFailureRecoveryConfig),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_beamFailureRecoveryConfig_14,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"beamFailureRecoveryConfig"
		},
};
static const int asn_MAP_BWP_UplinkDedicated_oms_1[] = { 0, 1, 2, 3, 4 };
static const ber_tlv_tag_t asn_DEF_BWP_UplinkDedicated_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_BWP_UplinkDedicated_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pucch-Config */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* pusch-Config */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* configuredGrantConfig */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* srs-Config */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* beamFailureRecoveryConfig */
};
asn_SEQUENCE_specifics_t asn_SPC_BWP_UplinkDedicated_specs_1 = {
	sizeof(struct BWP_UplinkDedicated),
	offsetof(struct BWP_UplinkDedicated, _asn_ctx),
	asn_MAP_BWP_UplinkDedicated_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_BWP_UplinkDedicated_oms_1,	/* Optional members */
	5, 0,	/* Root/Additions */
	5,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_BWP_UplinkDedicated = {
	"BWP-UplinkDedicated",
	"BWP-UplinkDedicated",
	&asn_OP_SEQUENCE,
	asn_DEF_BWP_UplinkDedicated_tags_1,
	sizeof(asn_DEF_BWP_UplinkDedicated_tags_1)
		/sizeof(asn_DEF_BWP_UplinkDedicated_tags_1[0]), /* 1 */
	asn_DEF_BWP_UplinkDedicated_tags_1,	/* Same as above */
	sizeof(asn_DEF_BWP_UplinkDedicated_tags_1)
		/sizeof(asn_DEF_BWP_UplinkDedicated_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_BWP_UplinkDedicated_1,
	5,	/* Elements count */
	&asn_SPC_BWP_UplinkDedicated_specs_1	/* Additional specs */
};

