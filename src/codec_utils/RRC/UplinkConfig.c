/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "UplinkConfig.h"

#include "BWP-UplinkDedicated.h"
#include "BWP-Uplink.h"
#include "PUSCH-ServingCellConfig.h"
#include "SRS-CarrierSwitching.h"
#include "SCS-SpecificCarrier.h"
static int
memb_uplinkChannelBW_PerSCS_List_constraint_15(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 5)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_uplinkBWP_ToReleaseList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 4)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_uplinkBWP_ToAddModList_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 4)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_uplinkBWP_ToReleaseList_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_type_uplinkBWP_ToReleaseList_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_uplinkBWP_ToAddModList_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_type_uplinkBWP_ToAddModList_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_pusch_ServingCellConfig_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_pusch_ServingCellConfig_constr_8 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_carrierSwitching_constr_11 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_carrierSwitching_constr_11 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_uplinkChannelBW_PerSCS_List_constr_17 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..5)) */};
static asn_per_constraints_t asn_PER_type_uplinkChannelBW_PerSCS_List_constr_17 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 3,  3,  1,  5 }	/* (SIZE(1..5)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_uplinkChannelBW_PerSCS_List_constr_17 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..5)) */};
static asn_per_constraints_t asn_PER_memb_uplinkChannelBW_PerSCS_List_constr_17 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 3,  3,  1,  5 }	/* (SIZE(1..5)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_uplinkBWP_ToReleaseList_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_memb_uplinkBWP_ToReleaseList_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_uplinkBWP_ToAddModList_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..4)) */};
static asn_per_constraints_t asn_PER_memb_uplinkBWP_ToAddModList_constr_5 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 2,  2,  1,  4 }	/* (SIZE(1..4)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_uplinkBWP_ToReleaseList_3[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_BWP_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_uplinkBWP_ToReleaseList_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_uplinkBWP_ToReleaseList_specs_3 = {
	sizeof(struct UplinkConfig__uplinkBWP_ToReleaseList),
	offsetof(struct UplinkConfig__uplinkBWP_ToReleaseList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_uplinkBWP_ToReleaseList_3 = {
	"uplinkBWP-ToReleaseList",
	"uplinkBWP-ToReleaseList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_uplinkBWP_ToReleaseList_tags_3,
	sizeof(asn_DEF_uplinkBWP_ToReleaseList_tags_3)
		/sizeof(asn_DEF_uplinkBWP_ToReleaseList_tags_3[0]) - 1, /* 1 */
	asn_DEF_uplinkBWP_ToReleaseList_tags_3,	/* Same as above */
	sizeof(asn_DEF_uplinkBWP_ToReleaseList_tags_3)
		/sizeof(asn_DEF_uplinkBWP_ToReleaseList_tags_3[0]), /* 2 */
	{ &asn_OER_type_uplinkBWP_ToReleaseList_constr_3, &asn_PER_type_uplinkBWP_ToReleaseList_constr_3, SEQUENCE_OF_constraint },
	asn_MBR_uplinkBWP_ToReleaseList_3,
	1,	/* Single element */
	&asn_SPC_uplinkBWP_ToReleaseList_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_uplinkBWP_ToAddModList_5[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_BWP_Uplink,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_uplinkBWP_ToAddModList_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_uplinkBWP_ToAddModList_specs_5 = {
	sizeof(struct UplinkConfig__uplinkBWP_ToAddModList),
	offsetof(struct UplinkConfig__uplinkBWP_ToAddModList, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_uplinkBWP_ToAddModList_5 = {
	"uplinkBWP-ToAddModList",
	"uplinkBWP-ToAddModList",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_uplinkBWP_ToAddModList_tags_5,
	sizeof(asn_DEF_uplinkBWP_ToAddModList_tags_5)
		/sizeof(asn_DEF_uplinkBWP_ToAddModList_tags_5[0]) - 1, /* 1 */
	asn_DEF_uplinkBWP_ToAddModList_tags_5,	/* Same as above */
	sizeof(asn_DEF_uplinkBWP_ToAddModList_tags_5)
		/sizeof(asn_DEF_uplinkBWP_ToAddModList_tags_5[0]), /* 2 */
	{ &asn_OER_type_uplinkBWP_ToAddModList_constr_5, &asn_PER_type_uplinkBWP_ToAddModList_constr_5, SEQUENCE_OF_constraint },
	asn_MBR_uplinkBWP_ToAddModList_5,
	1,	/* Single element */
	&asn_SPC_uplinkBWP_ToAddModList_specs_5	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_pusch_ServingCellConfig_8[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UplinkConfig__pusch_ServingCellConfig, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct UplinkConfig__pusch_ServingCellConfig, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUSCH_ServingCellConfig,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_pusch_ServingCellConfig_tag2el_8[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_pusch_ServingCellConfig_specs_8 = {
	sizeof(struct UplinkConfig__pusch_ServingCellConfig),
	offsetof(struct UplinkConfig__pusch_ServingCellConfig, _asn_ctx),
	offsetof(struct UplinkConfig__pusch_ServingCellConfig, present),
	sizeof(((struct UplinkConfig__pusch_ServingCellConfig *)0)->present),
	asn_MAP_pusch_ServingCellConfig_tag2el_8,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_pusch_ServingCellConfig_8 = {
	"pusch-ServingCellConfig",
	"pusch-ServingCellConfig",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_pusch_ServingCellConfig_constr_8, &asn_PER_type_pusch_ServingCellConfig_constr_8, CHOICE_constraint },
	asn_MBR_pusch_ServingCellConfig_8,
	2,	/* Elements count */
	&asn_SPC_pusch_ServingCellConfig_specs_8	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_carrierSwitching_11[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UplinkConfig__carrierSwitching, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct UplinkConfig__carrierSwitching, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SRS_CarrierSwitching,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_carrierSwitching_tag2el_11[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_carrierSwitching_specs_11 = {
	sizeof(struct UplinkConfig__carrierSwitching),
	offsetof(struct UplinkConfig__carrierSwitching, _asn_ctx),
	offsetof(struct UplinkConfig__carrierSwitching, present),
	sizeof(((struct UplinkConfig__carrierSwitching *)0)->present),
	asn_MAP_carrierSwitching_tag2el_11,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_carrierSwitching_11 = {
	"carrierSwitching",
	"carrierSwitching",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_carrierSwitching_constr_11, &asn_PER_type_carrierSwitching_constr_11, CHOICE_constraint },
	asn_MBR_carrierSwitching_11,
	2,	/* Elements count */
	&asn_SPC_carrierSwitching_specs_11	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_uplinkChannelBW_PerSCS_List_17[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_SCS_SpecificCarrier,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_uplinkChannelBW_PerSCS_List_tags_17[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_uplinkChannelBW_PerSCS_List_specs_17 = {
	sizeof(struct UplinkConfig__ext1__uplinkChannelBW_PerSCS_List),
	offsetof(struct UplinkConfig__ext1__uplinkChannelBW_PerSCS_List, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_uplinkChannelBW_PerSCS_List_17 = {
	"uplinkChannelBW-PerSCS-List",
	"uplinkChannelBW-PerSCS-List",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_uplinkChannelBW_PerSCS_List_tags_17,
	sizeof(asn_DEF_uplinkChannelBW_PerSCS_List_tags_17)
		/sizeof(asn_DEF_uplinkChannelBW_PerSCS_List_tags_17[0]) - 1, /* 1 */
	asn_DEF_uplinkChannelBW_PerSCS_List_tags_17,	/* Same as above */
	sizeof(asn_DEF_uplinkChannelBW_PerSCS_List_tags_17)
		/sizeof(asn_DEF_uplinkChannelBW_PerSCS_List_tags_17[0]), /* 2 */
	{ &asn_OER_type_uplinkChannelBW_PerSCS_List_constr_17, &asn_PER_type_uplinkChannelBW_PerSCS_List_constr_17, SEQUENCE_OF_constraint },
	asn_MBR_uplinkChannelBW_PerSCS_List_17,
	1,	/* Single element */
	&asn_SPC_uplinkChannelBW_PerSCS_List_specs_17	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ext1_15[] = {
	{ ATF_POINTER, 2, offsetof(struct UplinkConfig__ext1, powerBoostPi2BPSK),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"powerBoostPi2BPSK"
		},
	{ ATF_POINTER, 1, offsetof(struct UplinkConfig__ext1, uplinkChannelBW_PerSCS_List),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_uplinkChannelBW_PerSCS_List_17,
		0,
		{ &asn_OER_memb_uplinkChannelBW_PerSCS_List_constr_17, &asn_PER_memb_uplinkChannelBW_PerSCS_List_constr_17,  memb_uplinkChannelBW_PerSCS_List_constraint_15 },
		0, 0, /* No default value */
		"uplinkChannelBW-PerSCS-List"
		},
};
static const int asn_MAP_ext1_oms_15[] = { 0, 1 };
static const ber_tlv_tag_t asn_DEF_ext1_tags_15[] = {
	(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ext1_tag2el_15[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* powerBoostPi2BPSK */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* uplinkChannelBW-PerSCS-List */
};
static asn_SEQUENCE_specifics_t asn_SPC_ext1_specs_15 = {
	sizeof(struct UplinkConfig__ext1),
	offsetof(struct UplinkConfig__ext1, _asn_ctx),
	asn_MAP_ext1_tag2el_15,
	2,	/* Count of tags in the map */
	asn_MAP_ext1_oms_15,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ext1_15 = {
	"ext1",
	"ext1",
	&asn_OP_SEQUENCE,
	asn_DEF_ext1_tags_15,
	sizeof(asn_DEF_ext1_tags_15)
		/sizeof(asn_DEF_ext1_tags_15[0]) - 1, /* 1 */
	asn_DEF_ext1_tags_15,	/* Same as above */
	sizeof(asn_DEF_ext1_tags_15)
		/sizeof(asn_DEF_ext1_tags_15[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ext1_15,
	2,	/* Elements count */
	&asn_SPC_ext1_specs_15	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_UplinkConfig_1[] = {
	{ ATF_POINTER, 7, offsetof(struct UplinkConfig, initialUplinkBWP),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BWP_UplinkDedicated,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"initialUplinkBWP"
		},
	{ ATF_POINTER, 6, offsetof(struct UplinkConfig, uplinkBWP_ToReleaseList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_uplinkBWP_ToReleaseList_3,
		0,
		{ &asn_OER_memb_uplinkBWP_ToReleaseList_constr_3, &asn_PER_memb_uplinkBWP_ToReleaseList_constr_3,  memb_uplinkBWP_ToReleaseList_constraint_1 },
		0, 0, /* No default value */
		"uplinkBWP-ToReleaseList"
		},
	{ ATF_POINTER, 5, offsetof(struct UplinkConfig, uplinkBWP_ToAddModList),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_uplinkBWP_ToAddModList_5,
		0,
		{ &asn_OER_memb_uplinkBWP_ToAddModList_constr_5, &asn_PER_memb_uplinkBWP_ToAddModList_constr_5,  memb_uplinkBWP_ToAddModList_constraint_1 },
		0, 0, /* No default value */
		"uplinkBWP-ToAddModList"
		},
	{ ATF_POINTER, 4, offsetof(struct UplinkConfig, firstActiveUplinkBWP_Id),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BWP_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"firstActiveUplinkBWP-Id"
		},
	{ ATF_POINTER, 3, offsetof(struct UplinkConfig, pusch_ServingCellConfig),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_pusch_ServingCellConfig_8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pusch-ServingCellConfig"
		},
	{ ATF_POINTER, 2, offsetof(struct UplinkConfig, carrierSwitching),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_carrierSwitching_11,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"carrierSwitching"
		},
	{ ATF_POINTER, 1, offsetof(struct UplinkConfig, ext1),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		0,
		&asn_DEF_ext1_15,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ext1"
		},
};
static const int asn_MAP_UplinkConfig_oms_1[] = { 0, 1, 2, 3, 4, 5, 6 };
static const ber_tlv_tag_t asn_DEF_UplinkConfig_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UplinkConfig_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* initialUplinkBWP */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* uplinkBWP-ToReleaseList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* uplinkBWP-ToAddModList */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* firstActiveUplinkBWP-Id */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* pusch-ServingCellConfig */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* carrierSwitching */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* ext1 */
};
asn_SEQUENCE_specifics_t asn_SPC_UplinkConfig_specs_1 = {
	sizeof(struct UplinkConfig),
	offsetof(struct UplinkConfig, _asn_ctx),
	asn_MAP_UplinkConfig_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_UplinkConfig_oms_1,	/* Optional members */
	6, 1,	/* Root/Additions */
	6,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UplinkConfig = {
	"UplinkConfig",
	"UplinkConfig",
	&asn_OP_SEQUENCE,
	asn_DEF_UplinkConfig_tags_1,
	sizeof(asn_DEF_UplinkConfig_tags_1)
		/sizeof(asn_DEF_UplinkConfig_tags_1[0]), /* 1 */
	asn_DEF_UplinkConfig_tags_1,	/* Same as above */
	sizeof(asn_DEF_UplinkConfig_tags_1)
		/sizeof(asn_DEF_UplinkConfig_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UplinkConfig_1,
	7,	/* Elements count */
	&asn_SPC_UplinkConfig_specs_1	/* Additional specs */
};

