/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "RadioLinkMonitoringRS.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_purpose_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_purpose_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_type_detectionResource_constr_7 CC_NOTUSED = {
	{ 0, 0 },
	-1};
static asn_per_constraints_t asn_PER_type_detectionResource_constr_7 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_purpose_value2enum_3[] = {
	{ 0,	11,	"beamFailure" },
	{ 1,	3,	"rlf" },
	{ 2,	4,	"both" }
};
static const unsigned int asn_MAP_purpose_enum2value_3[] = {
	0,	/* beamFailure(0) */
	2,	/* both(2) */
	1	/* rlf(1) */
};
static const asn_INTEGER_specifics_t asn_SPC_purpose_specs_3 = {
	asn_MAP_purpose_value2enum_3,	/* "tag" => N; sorted by tag */
	asn_MAP_purpose_enum2value_3,	/* N => "tag"; sorted by N */
	3,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_purpose_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_purpose_3 = {
	"purpose",
	"purpose",
	&asn_OP_NativeEnumerated,
	asn_DEF_purpose_tags_3,
	sizeof(asn_DEF_purpose_tags_3)
		/sizeof(asn_DEF_purpose_tags_3[0]) - 1, /* 1 */
	asn_DEF_purpose_tags_3,	/* Same as above */
	sizeof(asn_DEF_purpose_tags_3)
		/sizeof(asn_DEF_purpose_tags_3[0]), /* 2 */
	{ &asn_OER_type_purpose_constr_3, &asn_PER_type_purpose_constr_3, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_purpose_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_detectionResource_7[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct RadioLinkMonitoringRS__detectionResource, choice.ssb_Index),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SSB_Index,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-Index"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RadioLinkMonitoringRS__detectionResource, choice.csi_RS_Index),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NZP_CSI_RS_ResourceId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"csi-RS-Index"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_detectionResource_tag2el_7[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ssb-Index */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* csi-RS-Index */
};
static asn_CHOICE_specifics_t asn_SPC_detectionResource_specs_7 = {
	sizeof(struct RadioLinkMonitoringRS__detectionResource),
	offsetof(struct RadioLinkMonitoringRS__detectionResource, _asn_ctx),
	offsetof(struct RadioLinkMonitoringRS__detectionResource, present),
	sizeof(((struct RadioLinkMonitoringRS__detectionResource *)0)->present),
	asn_MAP_detectionResource_tag2el_7,
	2,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_detectionResource_7 = {
	"detectionResource",
	"detectionResource",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_detectionResource_constr_7, &asn_PER_type_detectionResource_constr_7, CHOICE_constraint },
	asn_MBR_detectionResource_7,
	2,	/* Elements count */
	&asn_SPC_detectionResource_specs_7	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_RadioLinkMonitoringRS_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct RadioLinkMonitoringRS, radioLinkMonitoringRS_Id),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RadioLinkMonitoringRS_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"radioLinkMonitoringRS-Id"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RadioLinkMonitoringRS, purpose),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_purpose_3,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"purpose"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RadioLinkMonitoringRS, detectionResource),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_detectionResource_7,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"detectionResource"
		},
};
static const ber_tlv_tag_t asn_DEF_RadioLinkMonitoringRS_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_RadioLinkMonitoringRS_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* radioLinkMonitoringRS-Id */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* purpose */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* detectionResource */
};
asn_SEQUENCE_specifics_t asn_SPC_RadioLinkMonitoringRS_specs_1 = {
	sizeof(struct RadioLinkMonitoringRS),
	offsetof(struct RadioLinkMonitoringRS, _asn_ctx),
	asn_MAP_RadioLinkMonitoringRS_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_RadioLinkMonitoringRS = {
	"RadioLinkMonitoringRS",
	"RadioLinkMonitoringRS",
	&asn_OP_SEQUENCE,
	asn_DEF_RadioLinkMonitoringRS_tags_1,
	sizeof(asn_DEF_RadioLinkMonitoringRS_tags_1)
		/sizeof(asn_DEF_RadioLinkMonitoringRS_tags_1[0]), /* 1 */
	asn_DEF_RadioLinkMonitoringRS_tags_1,	/* Same as above */
	sizeof(asn_DEF_RadioLinkMonitoringRS_tags_1)
		/sizeof(asn_DEF_RadioLinkMonitoringRS_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_RadioLinkMonitoringRS_1,
	3,	/* Elements count */
	&asn_SPC_RadioLinkMonitoringRS_specs_1	/* Additional specs */
};

