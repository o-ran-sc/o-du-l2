/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "QoSFlowLevelQoSParameters.h"

#include "GBR-QoSFlowInformation.h"
#include "ProtocolExtensionContainer.h"
/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_reflective_QoS_Attribute_constr_5 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_reflective_QoS_Attribute_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  0,  0,  0,  0 }	/* (0..0,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_reflective_QoS_Attribute_value2enum_5[] = {
	{ 0,	10,	"subject-to" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_reflective_QoS_Attribute_enum2value_5[] = {
	0	/* subject-to(0) */
	/* This list is extensible */
};
static const asn_INTEGER_specifics_t asn_SPC_reflective_QoS_Attribute_specs_5 = {
	asn_MAP_reflective_QoS_Attribute_value2enum_5,	/* "tag" => N; sorted by tag */
	asn_MAP_reflective_QoS_Attribute_enum2value_5,	/* N => "tag"; sorted by N */
	1,	/* Number of elements in the maps */
	2,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_reflective_QoS_Attribute_tags_5[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_reflective_QoS_Attribute_5 = {
	"reflective-QoS-Attribute",
	"reflective-QoS-Attribute",
	&asn_OP_NativeEnumerated,
	asn_DEF_reflective_QoS_Attribute_tags_5,
	sizeof(asn_DEF_reflective_QoS_Attribute_tags_5)
		/sizeof(asn_DEF_reflective_QoS_Attribute_tags_5[0]) - 1, /* 1 */
	asn_DEF_reflective_QoS_Attribute_tags_5,	/* Same as above */
	sizeof(asn_DEF_reflective_QoS_Attribute_tags_5)
		/sizeof(asn_DEF_reflective_QoS_Attribute_tags_5[0]), /* 2 */
	{ &asn_OER_type_reflective_QoS_Attribute_constr_5, &asn_PER_type_reflective_QoS_Attribute_constr_5, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_reflective_QoS_Attribute_specs_5	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_QoSFlowLevelQoSParameters_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct QoSFlowLevelQoSParameters, qoS_Characteristics),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_QoS_Characteristics,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"qoS-Characteristics"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct QoSFlowLevelQoSParameters, nGRANallocationRetentionPriority),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NGRANAllocationAndRetentionPriority,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nGRANallocationRetentionPriority"
		},
	{ ATF_POINTER, 3, offsetof(struct QoSFlowLevelQoSParameters, gBR_QoS_Flow_Information),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GBR_QoSFlowInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"gBR-QoS-Flow-Information"
		},
	{ ATF_POINTER, 2, offsetof(struct QoSFlowLevelQoSParameters, reflective_QoS_Attribute),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_reflective_QoS_Attribute_5,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"reflective-QoS-Attribute"
		},
	{ ATF_POINTER, 1, offsetof(struct QoSFlowLevelQoSParameters, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_4624P74,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_QoSFlowLevelQoSParameters_oms_1[] = { 2, 3, 4 };
static const ber_tlv_tag_t asn_DEF_QoSFlowLevelQoSParameters_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_QoSFlowLevelQoSParameters_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* qoS-Characteristics */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* nGRANallocationRetentionPriority */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* gBR-QoS-Flow-Information */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* reflective-QoS-Attribute */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_QoSFlowLevelQoSParameters_specs_1 = {
	sizeof(struct QoSFlowLevelQoSParameters),
	offsetof(struct QoSFlowLevelQoSParameters, _asn_ctx),
	asn_MAP_QoSFlowLevelQoSParameters_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_QoSFlowLevelQoSParameters_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_QoSFlowLevelQoSParameters = {
	"QoSFlowLevelQoSParameters",
	"QoSFlowLevelQoSParameters",
	&asn_OP_SEQUENCE,
	asn_DEF_QoSFlowLevelQoSParameters_tags_1,
	sizeof(asn_DEF_QoSFlowLevelQoSParameters_tags_1)
		/sizeof(asn_DEF_QoSFlowLevelQoSParameters_tags_1[0]), /* 1 */
	asn_DEF_QoSFlowLevelQoSParameters_tags_1,	/* Same as above */
	sizeof(asn_DEF_QoSFlowLevelQoSParameters_tags_1)
		/sizeof(asn_DEF_QoSFlowLevelQoSParameters_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_QoSFlowLevelQoSParameters_1,
	5,	/* Elements count */
	&asn_SPC_QoSFlowLevelQoSParameters_specs_1	/* Additional specs */
};

