/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../ASN1_Input/rrc_15_3_asn.asn1"
 * 	`asn1c -D ../RRC_output_14Nov/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER`
 */

#include "BWP-Uplink.h"

#include "BWP-UplinkCommon.h"
#include "BWP-UplinkDedicated.h"
asn_TYPE_member_t asn_MBR_BWP_Uplink_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BWP_Uplink, bwp_Id),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BWP_Id,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"bwp-Id"
		},
	{ ATF_POINTER, 2, offsetof(struct BWP_Uplink, bwp_Common),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BWP_UplinkCommon,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"bwp-Common"
		},
	{ ATF_POINTER, 1, offsetof(struct BWP_Uplink, bwp_Dedicated),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BWP_UplinkDedicated,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"bwp-Dedicated"
		},
};
static const int asn_MAP_BWP_Uplink_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_BWP_Uplink_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_BWP_Uplink_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* bwp-Id */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* bwp-Common */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* bwp-Dedicated */
};
asn_SEQUENCE_specifics_t asn_SPC_BWP_Uplink_specs_1 = {
	sizeof(struct BWP_Uplink),
	offsetof(struct BWP_Uplink, _asn_ctx),
	asn_MAP_BWP_Uplink_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_BWP_Uplink_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_BWP_Uplink = {
	"BWP-Uplink",
	"BWP-Uplink",
	&asn_OP_SEQUENCE,
	asn_DEF_BWP_Uplink_tags_1,
	sizeof(asn_DEF_BWP_Uplink_tags_1)
		/sizeof(asn_DEF_BWP_Uplink_tags_1[0]), /* 1 */
	asn_DEF_BWP_Uplink_tags_1,	/* Same as above */
	sizeof(asn_DEF_BWP_Uplink_tags_1)
		/sizeof(asn_DEF_BWP_Uplink_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_BWP_Uplink_1,
	3,	/* Elements count */
	&asn_SPC_BWP_Uplink_specs_1	/* Additional specs */
};

