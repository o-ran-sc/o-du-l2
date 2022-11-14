/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "../F1.asn1"
 * 	`asn1c -D ../F1_output/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "GBR-QoSFlowInformation.h"

#include "ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_GBR_QoSFlowInformation_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct GBR_QoSFlowInformation, maxFlowBitRateDownlink),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BitRate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxFlowBitRateDownlink"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GBR_QoSFlowInformation, maxFlowBitRateUplink),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BitRate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxFlowBitRateUplink"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GBR_QoSFlowInformation, guaranteedFlowBitRateDownlink),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BitRate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"guaranteedFlowBitRateDownlink"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GBR_QoSFlowInformation, guaranteedFlowBitRateUplink),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BitRate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"guaranteedFlowBitRateUplink"
		},
	{ ATF_POINTER, 3, offsetof(struct GBR_QoSFlowInformation, maxPacketLossRateDownlink),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MaxPacketLossRate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxPacketLossRateDownlink"
		},
	{ ATF_POINTER, 2, offsetof(struct GBR_QoSFlowInformation, maxPacketLossRateUplink),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MaxPacketLossRate,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"maxPacketLossRateUplink"
		},
	{ ATF_POINTER, 1, offsetof(struct GBR_QoSFlowInformation, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_4624P53,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_GBR_QoSFlowInformation_oms_1[] = { 4, 5, 6 };
static const ber_tlv_tag_t asn_DEF_GBR_QoSFlowInformation_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_GBR_QoSFlowInformation_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* maxFlowBitRateDownlink */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* maxFlowBitRateUplink */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* guaranteedFlowBitRateDownlink */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* guaranteedFlowBitRateUplink */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* maxPacketLossRateDownlink */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* maxPacketLossRateUplink */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_GBR_QoSFlowInformation_specs_1 = {
	sizeof(struct GBR_QoSFlowInformation),
	offsetof(struct GBR_QoSFlowInformation, _asn_ctx),
	asn_MAP_GBR_QoSFlowInformation_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_GBR_QoSFlowInformation_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	7,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_GBR_QoSFlowInformation = {
	"GBR-QoSFlowInformation",
	"GBR-QoSFlowInformation",
	&asn_OP_SEQUENCE,
	asn_DEF_GBR_QoSFlowInformation_tags_1,
	sizeof(asn_DEF_GBR_QoSFlowInformation_tags_1)
		/sizeof(asn_DEF_GBR_QoSFlowInformation_tags_1[0]), /* 1 */
	asn_DEF_GBR_QoSFlowInformation_tags_1,	/* Same as above */
	sizeof(asn_DEF_GBR_QoSFlowInformation_tags_1)
		/sizeof(asn_DEF_GBR_QoSFlowInformation_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_GBR_QoSFlowInformation_1,
	7,	/* Elements count */
	&asn_SPC_GBR_QoSFlowInformation_specs_1	/* Additional specs */
};

