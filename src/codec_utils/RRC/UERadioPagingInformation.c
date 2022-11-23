/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "UERadioPagingInformation.h"

#include "UERadioPagingInformation-IEs.h"
static asn_oer_constraints_t asn_OER_type_c1_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_c1_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_criticalExtensions_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_criticalExtensions_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_c1_3[] = {
	{ ATF_POINTER, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.ueRadioPagingInformation),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UERadioPagingInformation_IEs,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ueRadioPagingInformation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.spare7),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare7"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.spare6),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare6"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.spare5),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare5"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.spare4),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare4"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.spare3),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare3"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.spare2),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare2"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation__criticalExtensions__c1, choice.spare1),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"spare1"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_c1_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ueRadioPagingInformation */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* spare7 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* spare6 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* spare5 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* spare4 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* spare3 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* spare2 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* spare1 */
};
static asn_CHOICE_specifics_t asn_SPC_c1_specs_3 = {
	sizeof(struct UERadioPagingInformation__criticalExtensions__c1),
	offsetof(struct UERadioPagingInformation__criticalExtensions__c1, _asn_ctx),
	offsetof(struct UERadioPagingInformation__criticalExtensions__c1, present),
	sizeof(((struct UERadioPagingInformation__criticalExtensions__c1 *)0)->present),
	asn_MAP_c1_tag2el_3,
	8,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_c1_3 = {
	"c1",
	"c1",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_c1_constr_3, &asn_PER_type_c1_constr_3, CHOICE_constraint },
	asn_MBR_c1_3,
	8,	/* Elements count */
	&asn_SPC_c1_specs_3	/* Additional specs */
};

static const ber_tlv_tag_t asn_DEF_criticalExtensionsFuture_tags_12[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_criticalExtensionsFuture_specs_12 = {
	sizeof(struct UERadioPagingInformation__criticalExtensions__criticalExtensionsFuture),
	offsetof(struct UERadioPagingInformation__criticalExtensions__criticalExtensionsFuture, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_criticalExtensionsFuture_12 = {
	"criticalExtensionsFuture",
	"criticalExtensionsFuture",
	&asn_OP_SEQUENCE,
	asn_DEF_criticalExtensionsFuture_tags_12,
	sizeof(asn_DEF_criticalExtensionsFuture_tags_12)
		/sizeof(asn_DEF_criticalExtensionsFuture_tags_12[0]) - 1, /* 1 */
	asn_DEF_criticalExtensionsFuture_tags_12,	/* Same as above */
	sizeof(asn_DEF_criticalExtensionsFuture_tags_12)
		/sizeof(asn_DEF_criticalExtensionsFuture_tags_12[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	0, 0,	/* No members */
	&asn_SPC_criticalExtensionsFuture_specs_12	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_criticalExtensions_2[] = {
	{ ATF_POINTER, 0, offsetof(struct UERadioPagingInformation__criticalExtensions, choice.c1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_c1_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"c1"
		},
	{ ATF_POINTER, 0, offsetof(struct UERadioPagingInformation__criticalExtensions, choice.criticalExtensionsFuture),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_criticalExtensionsFuture_12,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"criticalExtensionsFuture"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_criticalExtensions_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* c1 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* criticalExtensionsFuture */
};
static asn_CHOICE_specifics_t asn_SPC_criticalExtensions_specs_2 = {
	sizeof(struct UERadioPagingInformation__criticalExtensions),
	offsetof(struct UERadioPagingInformation__criticalExtensions, _asn_ctx),
	offsetof(struct UERadioPagingInformation__criticalExtensions, present),
	sizeof(((struct UERadioPagingInformation__criticalExtensions *)0)->present),
	asn_MAP_criticalExtensions_tag2el_2,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_criticalExtensions_2 = {
	"criticalExtensions",
	"criticalExtensions",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_criticalExtensions_constr_2, &asn_PER_type_criticalExtensions_constr_2, CHOICE_constraint },
	asn_MBR_criticalExtensions_2,
	2,	/* Elements count */
	&asn_SPC_criticalExtensions_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_UERadioPagingInformation_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UERadioPagingInformation, criticalExtensions),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_criticalExtensions_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"criticalExtensions"
		},
};
static const ber_tlv_tag_t asn_DEF_UERadioPagingInformation_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UERadioPagingInformation_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* criticalExtensions */
};
static asn_SEQUENCE_specifics_t asn_SPC_UERadioPagingInformation_specs_1 = {
	sizeof(struct UERadioPagingInformation),
	offsetof(struct UERadioPagingInformation, _asn_ctx),
	asn_MAP_UERadioPagingInformation_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UERadioPagingInformation = {
	"UERadioPagingInformation",
	"UERadioPagingInformation",
	&asn_OP_SEQUENCE,
	asn_DEF_UERadioPagingInformation_tags_1,
	sizeof(asn_DEF_UERadioPagingInformation_tags_1)
		/sizeof(asn_DEF_UERadioPagingInformation_tags_1[0]), /* 1 */
	asn_DEF_UERadioPagingInformation_tags_1,	/* Same as above */
	sizeof(asn_DEF_UERadioPagingInformation_tags_1)
		/sizeof(asn_DEF_UERadioPagingInformation_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UERadioPagingInformation_1,
	1,	/* Elements count */
	&asn_SPC_UERadioPagingInformation_specs_1	/* Additional specs */
};

