/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "./22April22_Paging/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./22April22_Paging -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "LocationMeasurementIndication-IEs.h"

#include "LocationMeasurementInfo.h"
static asn_oer_constraints_t asn_OER_type_measurementIndication_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_measurementIndication_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_measurementIndication_2[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct LocationMeasurementIndication_IEs__measurementIndication, choice.release),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"release"
		},
	{ ATF_POINTER, 0, offsetof(struct LocationMeasurementIndication_IEs__measurementIndication, choice.setup),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_LocationMeasurementInfo,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"setup"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_measurementIndication_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* release */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* setup */
};
static asn_CHOICE_specifics_t asn_SPC_measurementIndication_specs_2 = {
	sizeof(struct LocationMeasurementIndication_IEs__measurementIndication),
	offsetof(struct LocationMeasurementIndication_IEs__measurementIndication, _asn_ctx),
	offsetof(struct LocationMeasurementIndication_IEs__measurementIndication, present),
	sizeof(((struct LocationMeasurementIndication_IEs__measurementIndication *)0)->present),
	asn_MAP_measurementIndication_tag2el_2,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_measurementIndication_2 = {
	"measurementIndication",
	"measurementIndication",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_measurementIndication_constr_2, &asn_PER_type_measurementIndication_constr_2, CHOICE_constraint },
	asn_MBR_measurementIndication_2,
	2,	/* Elements count */
	&asn_SPC_measurementIndication_specs_2	/* Additional specs */
};

static const ber_tlv_tag_t asn_DEF_nonCriticalExtension_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_nonCriticalExtension_specs_6 = {
	sizeof(struct LocationMeasurementIndication_IEs__nonCriticalExtension),
	offsetof(struct LocationMeasurementIndication_IEs__nonCriticalExtension, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_nonCriticalExtension_6 = {
	"nonCriticalExtension",
	"nonCriticalExtension",
	&asn_OP_SEQUENCE,
	asn_DEF_nonCriticalExtension_tags_6,
	sizeof(asn_DEF_nonCriticalExtension_tags_6)
		/sizeof(asn_DEF_nonCriticalExtension_tags_6[0]) - 1, /* 1 */
	asn_DEF_nonCriticalExtension_tags_6,	/* Same as above */
	sizeof(asn_DEF_nonCriticalExtension_tags_6)
		/sizeof(asn_DEF_nonCriticalExtension_tags_6[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	0, 0,	/* No members */
	&asn_SPC_nonCriticalExtension_specs_6	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_LocationMeasurementIndication_IEs_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct LocationMeasurementIndication_IEs, measurementIndication),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_measurementIndication_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measurementIndication"
		},
	{ ATF_POINTER, 2, offsetof(struct LocationMeasurementIndication_IEs, lateNonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"lateNonCriticalExtension"
		},
	{ ATF_POINTER, 1, offsetof(struct LocationMeasurementIndication_IEs, nonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_nonCriticalExtension_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nonCriticalExtension"
		},
};
static const int asn_MAP_LocationMeasurementIndication_IEs_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_LocationMeasurementIndication_IEs_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_LocationMeasurementIndication_IEs_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* measurementIndication */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* lateNonCriticalExtension */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* nonCriticalExtension */
};
asn_SEQUENCE_specifics_t asn_SPC_LocationMeasurementIndication_IEs_specs_1 = {
	sizeof(struct LocationMeasurementIndication_IEs),
	offsetof(struct LocationMeasurementIndication_IEs, _asn_ctx),
	asn_MAP_LocationMeasurementIndication_IEs_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_LocationMeasurementIndication_IEs_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_LocationMeasurementIndication_IEs = {
	"LocationMeasurementIndication-IEs",
	"LocationMeasurementIndication-IEs",
	&asn_OP_SEQUENCE,
	asn_DEF_LocationMeasurementIndication_IEs_tags_1,
	sizeof(asn_DEF_LocationMeasurementIndication_IEs_tags_1)
		/sizeof(asn_DEF_LocationMeasurementIndication_IEs_tags_1[0]), /* 1 */
	asn_DEF_LocationMeasurementIndication_IEs_tags_1,	/* Same as above */
	sizeof(asn_DEF_LocationMeasurementIndication_IEs_tags_1)
		/sizeof(asn_DEF_LocationMeasurementIndication_IEs_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_LocationMeasurementIndication_IEs_1,
	3,	/* Elements count */
	&asn_SPC_LocationMeasurementIndication_IEs_specs_1	/* Additional specs */
};

