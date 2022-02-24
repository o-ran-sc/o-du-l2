/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SDAP-Config.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_mappedQoS_FlowsToAdd_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 64)) {
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
memb_mappedQoS_FlowsToRelease_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 64)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_sdap_HeaderDL_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_sdap_HeaderDL_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_sdap_HeaderUL_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_sdap_HeaderUL_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_mappedQoS_FlowsToAdd_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..64)) */};
static asn_per_constraints_t asn_PER_type_mappedQoS_FlowsToAdd_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 6,  6,  1,  64 }	/* (SIZE(1..64)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_mappedQoS_FlowsToRelease_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..64)) */};
static asn_per_constraints_t asn_PER_type_mappedQoS_FlowsToRelease_constr_12 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 6,  6,  1,  64 }	/* (SIZE(1..64)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_mappedQoS_FlowsToAdd_constr_10 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..64)) */};
static asn_per_constraints_t asn_PER_memb_mappedQoS_FlowsToAdd_constr_10 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 6,  6,  1,  64 }	/* (SIZE(1..64)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_mappedQoS_FlowsToRelease_constr_12 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..64)) */};
static asn_per_constraints_t asn_PER_memb_mappedQoS_FlowsToRelease_constr_12 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 6,  6,  1,  64 }	/* (SIZE(1..64)) */,
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_sdap_HeaderDL_value2enum_3[] = {
	{ 0,	7,	"present" },
	{ 1,	6,	"absent" }
};
static const unsigned int asn_MAP_sdap_HeaderDL_enum2value_3[] = {
	1,	/* absent(1) */
	0	/* present(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_sdap_HeaderDL_specs_3 = {
	asn_MAP_sdap_HeaderDL_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_sdap_HeaderDL_enum2value_3,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_sdap_HeaderDL_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sdap_HeaderDL_3 = {
	"sdap-HeaderDL",
	"sdap-HeaderDL",
	&asn_OP_NativeEnumerated,
	asn_DEF_sdap_HeaderDL_tags_3,
	sizeof(asn_DEF_sdap_HeaderDL_tags_3)
		/sizeof(asn_DEF_sdap_HeaderDL_tags_3[0]) - 1, /* 1 */
	asn_DEF_sdap_HeaderDL_tags_3,	/* Same as above */
	sizeof(asn_DEF_sdap_HeaderDL_tags_3)
		/sizeof(asn_DEF_sdap_HeaderDL_tags_3[0]), /* 2 */
	{ &asn_OER_type_sdap_HeaderDL_constr_3, &asn_PER_type_sdap_HeaderDL_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_sdap_HeaderDL_specs_3	/* Additional specs */
};

static const asn_INTEGER_enum_map_t asn_MAP_sdap_HeaderUL_value2enum_6[] = {
	{ 0,	7,	"present" },
	{ 1,	6,	"absent" }
};
static const unsigned int asn_MAP_sdap_HeaderUL_enum2value_6[] = {
	1,	/* absent(1) */
	0	/* present(0) */
};
static const asn_INTEGER_specifics_t asn_SPC_sdap_HeaderUL_specs_6 = {
	asn_MAP_sdap_HeaderUL_value2enum_6,	/* "tag" => N; sorted by tag */
	asn_MAP_sdap_HeaderUL_enum2value_6,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_sdap_HeaderUL_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sdap_HeaderUL_6 = {
	"sdap-HeaderUL",
	"sdap-HeaderUL",
	&asn_OP_NativeEnumerated,
	asn_DEF_sdap_HeaderUL_tags_6,
	sizeof(asn_DEF_sdap_HeaderUL_tags_6)
		/sizeof(asn_DEF_sdap_HeaderUL_tags_6[0]) - 1, /* 1 */
	asn_DEF_sdap_HeaderUL_tags_6,	/* Same as above */
	sizeof(asn_DEF_sdap_HeaderUL_tags_6)
		/sizeof(asn_DEF_sdap_HeaderUL_tags_6[0]), /* 2 */
	{ &asn_OER_type_sdap_HeaderUL_constr_6, &asn_PER_type_sdap_HeaderUL_constr_6, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_sdap_HeaderUL_specs_6	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_mappedQoS_FlowsToAdd_10[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_QFI,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_mappedQoS_FlowsToAdd_tags_10[] = {
	(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_mappedQoS_FlowsToAdd_specs_10 = {
	sizeof(struct SDAP_Config__mappedQoS_FlowsToAdd),
	offsetof(struct SDAP_Config__mappedQoS_FlowsToAdd, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_mappedQoS_FlowsToAdd_10 = {
	"mappedQoS-FlowsToAdd",
	"mappedQoS-FlowsToAdd",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_mappedQoS_FlowsToAdd_tags_10,
	sizeof(asn_DEF_mappedQoS_FlowsToAdd_tags_10)
		/sizeof(asn_DEF_mappedQoS_FlowsToAdd_tags_10[0]) - 1, /* 1 */
	asn_DEF_mappedQoS_FlowsToAdd_tags_10,	/* Same as above */
	sizeof(asn_DEF_mappedQoS_FlowsToAdd_tags_10)
		/sizeof(asn_DEF_mappedQoS_FlowsToAdd_tags_10[0]), /* 2 */
	{ &asn_OER_type_mappedQoS_FlowsToAdd_constr_10, &asn_PER_type_mappedQoS_FlowsToAdd_constr_10, SEQUENCE_OF_constraint },
	asn_MBR_mappedQoS_FlowsToAdd_10,
	1,	/* Single element */
	&asn_SPC_mappedQoS_FlowsToAdd_specs_10	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_mappedQoS_FlowsToRelease_12[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_QFI,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_mappedQoS_FlowsToRelease_tags_12[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_mappedQoS_FlowsToRelease_specs_12 = {
	sizeof(struct SDAP_Config__mappedQoS_FlowsToRelease),
	offsetof(struct SDAP_Config__mappedQoS_FlowsToRelease, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_mappedQoS_FlowsToRelease_12 = {
	"mappedQoS-FlowsToRelease",
	"mappedQoS-FlowsToRelease",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_mappedQoS_FlowsToRelease_tags_12,
	sizeof(asn_DEF_mappedQoS_FlowsToRelease_tags_12)
		/sizeof(asn_DEF_mappedQoS_FlowsToRelease_tags_12[0]) - 1, /* 1 */
	asn_DEF_mappedQoS_FlowsToRelease_tags_12,	/* Same as above */
	sizeof(asn_DEF_mappedQoS_FlowsToRelease_tags_12)
		/sizeof(asn_DEF_mappedQoS_FlowsToRelease_tags_12[0]), /* 2 */
	{ &asn_OER_type_mappedQoS_FlowsToRelease_constr_12, &asn_PER_type_mappedQoS_FlowsToRelease_constr_12, SEQUENCE_OF_constraint },
	asn_MBR_mappedQoS_FlowsToRelease_12,
	1,	/* Single element */
	&asn_SPC_mappedQoS_FlowsToRelease_specs_12	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_SDAP_Config_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SDAP_Config, pdu_Session),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PDU_SessionID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pdu-Session"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SDAP_Config, sdap_HeaderDL),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_sdap_HeaderDL_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sdap-HeaderDL"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SDAP_Config, sdap_HeaderUL),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_sdap_HeaderUL_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"sdap-HeaderUL"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SDAP_Config, defaultDRB),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"defaultDRB"
		},
	{ ATF_POINTER, 2, offsetof(struct SDAP_Config, mappedQoS_FlowsToAdd),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		0,
		&asn_DEF_mappedQoS_FlowsToAdd_10,
		0,
		{ &asn_OER_memb_mappedQoS_FlowsToAdd_constr_10, &asn_PER_memb_mappedQoS_FlowsToAdd_constr_10,  memb_mappedQoS_FlowsToAdd_constraint_1 },
		0, 0, /* No default value */
		"mappedQoS-FlowsToAdd"
		},
	{ ATF_POINTER, 1, offsetof(struct SDAP_Config, mappedQoS_FlowsToRelease),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		0,
		&asn_DEF_mappedQoS_FlowsToRelease_12,
		0,
		{ &asn_OER_memb_mappedQoS_FlowsToRelease_constr_12, &asn_PER_memb_mappedQoS_FlowsToRelease_constr_12,  memb_mappedQoS_FlowsToRelease_constraint_1 },
		0, 0, /* No default value */
		"mappedQoS-FlowsToRelease"
		},
};
static const int asn_MAP_SDAP_Config_oms_1[] = { 4, 5 };
static const ber_tlv_tag_t asn_DEF_SDAP_Config_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SDAP_Config_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pdu-Session */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* sdap-HeaderDL */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* sdap-HeaderUL */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* defaultDRB */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* mappedQoS-FlowsToAdd */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* mappedQoS-FlowsToRelease */
};
asn_SEQUENCE_specifics_t asn_SPC_SDAP_Config_specs_1 = {
	sizeof(struct SDAP_Config),
	offsetof(struct SDAP_Config, _asn_ctx),
	asn_MAP_SDAP_Config_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_SDAP_Config_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	6,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SDAP_Config = {
	"SDAP-Config",
	"SDAP-Config",
	&asn_OP_SEQUENCE,
	asn_DEF_SDAP_Config_tags_1,
	sizeof(asn_DEF_SDAP_Config_tags_1)
		/sizeof(asn_DEF_SDAP_Config_tags_1[0]), /* 1 */
	asn_DEF_SDAP_Config_tags_1,	/* Same as above */
	sizeof(asn_DEF_SDAP_Config_tags_1)
		/sizeof(asn_DEF_SDAP_Config_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SDAP_Config_1,
	6,	/* Elements count */
	&asn_SPC_SDAP_Config_specs_1	/* Additional specs */
};

