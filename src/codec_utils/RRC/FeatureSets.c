/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "FeatureSets.h"

#include "FeatureSetDownlink.h"
#include "FeatureSetDownlinkPerCC.h"
#include "FeatureSetUplink.h"
#include "FeatureSetUplinkPerCC.h"
static int
memb_featureSetsDownlink_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 1024)) {
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
memb_featureSetsDownlinkPerCC_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 1024)) {
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
memb_featureSetsUplink_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 1024)) {
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
memb_featureSetsUplinkPerCC_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 1024)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_type_featureSetsDownlink_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_type_featureSetsDownlink_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_featureSetsDownlinkPerCC_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_type_featureSetsDownlinkPerCC_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_featureSetsUplink_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_type_featureSetsUplink_constr_6 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_featureSetsUplinkPerCC_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_type_featureSetsUplinkPerCC_constr_8 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_featureSetsDownlink_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_memb_featureSetsDownlink_constr_2 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_featureSetsDownlinkPerCC_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_memb_featureSetsDownlinkPerCC_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_featureSetsUplink_constr_6 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_memb_featureSetsUplink_constr_6 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_featureSetsUplinkPerCC_constr_8 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(1..1024)) */};
static asn_per_constraints_t asn_PER_memb_featureSetsUplinkPerCC_constr_8 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 10,  10,  1,  1024 }	/* (SIZE(1..1024)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_featureSetsDownlink_2[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_FeatureSetDownlink,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_featureSetsDownlink_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_featureSetsDownlink_specs_2 = {
	sizeof(struct FeatureSets__featureSetsDownlink),
	offsetof(struct FeatureSets__featureSetsDownlink, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_featureSetsDownlink_2 = {
	"featureSetsDownlink",
	"featureSetsDownlink",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_featureSetsDownlink_tags_2,
	sizeof(asn_DEF_featureSetsDownlink_tags_2)
		/sizeof(asn_DEF_featureSetsDownlink_tags_2[0]) - 1, /* 1 */
	asn_DEF_featureSetsDownlink_tags_2,	/* Same as above */
	sizeof(asn_DEF_featureSetsDownlink_tags_2)
		/sizeof(asn_DEF_featureSetsDownlink_tags_2[0]), /* 2 */
	{ &asn_OER_type_featureSetsDownlink_constr_2, &asn_PER_type_featureSetsDownlink_constr_2, SEQUENCE_OF_constraint },
	asn_MBR_featureSetsDownlink_2,
	1,	/* Single element */
	&asn_SPC_featureSetsDownlink_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_featureSetsDownlinkPerCC_4[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_FeatureSetDownlinkPerCC,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_featureSetsDownlinkPerCC_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_featureSetsDownlinkPerCC_specs_4 = {
	sizeof(struct FeatureSets__featureSetsDownlinkPerCC),
	offsetof(struct FeatureSets__featureSetsDownlinkPerCC, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_featureSetsDownlinkPerCC_4 = {
	"featureSetsDownlinkPerCC",
	"featureSetsDownlinkPerCC",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_featureSetsDownlinkPerCC_tags_4,
	sizeof(asn_DEF_featureSetsDownlinkPerCC_tags_4)
		/sizeof(asn_DEF_featureSetsDownlinkPerCC_tags_4[0]) - 1, /* 1 */
	asn_DEF_featureSetsDownlinkPerCC_tags_4,	/* Same as above */
	sizeof(asn_DEF_featureSetsDownlinkPerCC_tags_4)
		/sizeof(asn_DEF_featureSetsDownlinkPerCC_tags_4[0]), /* 2 */
	{ &asn_OER_type_featureSetsDownlinkPerCC_constr_4, &asn_PER_type_featureSetsDownlinkPerCC_constr_4, SEQUENCE_OF_constraint },
	asn_MBR_featureSetsDownlinkPerCC_4,
	1,	/* Single element */
	&asn_SPC_featureSetsDownlinkPerCC_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_featureSetsUplink_6[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_FeatureSetUplink,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_featureSetsUplink_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_featureSetsUplink_specs_6 = {
	sizeof(struct FeatureSets__featureSetsUplink),
	offsetof(struct FeatureSets__featureSetsUplink, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_featureSetsUplink_6 = {
	"featureSetsUplink",
	"featureSetsUplink",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_featureSetsUplink_tags_6,
	sizeof(asn_DEF_featureSetsUplink_tags_6)
		/sizeof(asn_DEF_featureSetsUplink_tags_6[0]) - 1, /* 1 */
	asn_DEF_featureSetsUplink_tags_6,	/* Same as above */
	sizeof(asn_DEF_featureSetsUplink_tags_6)
		/sizeof(asn_DEF_featureSetsUplink_tags_6[0]), /* 2 */
	{ &asn_OER_type_featureSetsUplink_constr_6, &asn_PER_type_featureSetsUplink_constr_6, SEQUENCE_OF_constraint },
	asn_MBR_featureSetsUplink_6,
	1,	/* Single element */
	&asn_SPC_featureSetsUplink_specs_6	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_featureSetsUplinkPerCC_8[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_FeatureSetUplinkPerCC,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_featureSetsUplinkPerCC_tags_8[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_featureSetsUplinkPerCC_specs_8 = {
	sizeof(struct FeatureSets__featureSetsUplinkPerCC),
	offsetof(struct FeatureSets__featureSetsUplinkPerCC, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_featureSetsUplinkPerCC_8 = {
	"featureSetsUplinkPerCC",
	"featureSetsUplinkPerCC",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_featureSetsUplinkPerCC_tags_8,
	sizeof(asn_DEF_featureSetsUplinkPerCC_tags_8)
		/sizeof(asn_DEF_featureSetsUplinkPerCC_tags_8[0]) - 1, /* 1 */
	asn_DEF_featureSetsUplinkPerCC_tags_8,	/* Same as above */
	sizeof(asn_DEF_featureSetsUplinkPerCC_tags_8)
		/sizeof(asn_DEF_featureSetsUplinkPerCC_tags_8[0]), /* 2 */
	{ &asn_OER_type_featureSetsUplinkPerCC_constr_8, &asn_PER_type_featureSetsUplinkPerCC_constr_8, SEQUENCE_OF_constraint },
	asn_MBR_featureSetsUplinkPerCC_8,
	1,	/* Single element */
	&asn_SPC_featureSetsUplinkPerCC_specs_8	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_FeatureSets_1[] = {
	{ ATF_POINTER, 4, offsetof(struct FeatureSets, featureSetsDownlink),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_featureSetsDownlink_2,
		0,
		{ &asn_OER_memb_featureSetsDownlink_constr_2, &asn_PER_memb_featureSetsDownlink_constr_2,  memb_featureSetsDownlink_constraint_1 },
		0, 0, /* No default value */
		"featureSetsDownlink"
		},
	{ ATF_POINTER, 3, offsetof(struct FeatureSets, featureSetsDownlinkPerCC),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_featureSetsDownlinkPerCC_4,
		0,
		{ &asn_OER_memb_featureSetsDownlinkPerCC_constr_4, &asn_PER_memb_featureSetsDownlinkPerCC_constr_4,  memb_featureSetsDownlinkPerCC_constraint_1 },
		0, 0, /* No default value */
		"featureSetsDownlinkPerCC"
		},
	{ ATF_POINTER, 2, offsetof(struct FeatureSets, featureSetsUplink),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_featureSetsUplink_6,
		0,
		{ &asn_OER_memb_featureSetsUplink_constr_6, &asn_PER_memb_featureSetsUplink_constr_6,  memb_featureSetsUplink_constraint_1 },
		0, 0, /* No default value */
		"featureSetsUplink"
		},
	{ ATF_POINTER, 1, offsetof(struct FeatureSets, featureSetsUplinkPerCC),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		0,
		&asn_DEF_featureSetsUplinkPerCC_8,
		0,
		{ &asn_OER_memb_featureSetsUplinkPerCC_constr_8, &asn_PER_memb_featureSetsUplinkPerCC_constr_8,  memb_featureSetsUplinkPerCC_constraint_1 },
		0, 0, /* No default value */
		"featureSetsUplinkPerCC"
		},
};
static const int asn_MAP_FeatureSets_oms_1[] = { 0, 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_FeatureSets_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_FeatureSets_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* featureSetsDownlink */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* featureSetsDownlinkPerCC */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* featureSetsUplink */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* featureSetsUplinkPerCC */
};
asn_SEQUENCE_specifics_t asn_SPC_FeatureSets_specs_1 = {
	sizeof(struct FeatureSets),
	offsetof(struct FeatureSets, _asn_ctx),
	asn_MAP_FeatureSets_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_FeatureSets_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_FeatureSets = {
	"FeatureSets",
	"FeatureSets",
	&asn_OP_SEQUENCE,
	asn_DEF_FeatureSets_tags_1,
	sizeof(asn_DEF_FeatureSets_tags_1)
		/sizeof(asn_DEF_FeatureSets_tags_1[0]), /* 1 */
	asn_DEF_FeatureSets_tags_1,	/* Same as above */
	sizeof(asn_DEF_FeatureSets_tags_1)
		/sizeof(asn_DEF_FeatureSets_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_FeatureSets_1,
	4,	/* Elements count */
	&asn_SPC_FeatureSets_specs_1	/* Additional specs */
};

