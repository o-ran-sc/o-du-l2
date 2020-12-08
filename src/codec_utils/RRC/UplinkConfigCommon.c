/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "UplinkConfigCommon.h"

#include "FrequencyInfoUL.h"
#include "BWP-UplinkCommon.h"
asn_TYPE_member_t asn_MBR_UplinkConfigCommon_1[] = {
	{ ATF_POINTER, 2, offsetof(struct UplinkConfigCommon, frequencyInfoUL),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FrequencyInfoUL,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"frequencyInfoUL"
		},
	{ ATF_POINTER, 1, offsetof(struct UplinkConfigCommon, initialUplinkBWP),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BWP_UplinkCommon,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"initialUplinkBWP"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UplinkConfigCommon, dummy),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeAlignmentTimer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dummy"
		},
};
static const int asn_MAP_UplinkConfigCommon_oms_1[] = { 0, 1 };
static const ber_tlv_tag_t asn_DEF_UplinkConfigCommon_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UplinkConfigCommon_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* frequencyInfoUL */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* initialUplinkBWP */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* dummy */
};
asn_SEQUENCE_specifics_t asn_SPC_UplinkConfigCommon_specs_1 = {
	sizeof(struct UplinkConfigCommon),
	offsetof(struct UplinkConfigCommon, _asn_ctx),
	asn_MAP_UplinkConfigCommon_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_UplinkConfigCommon_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_UplinkConfigCommon = {
	"UplinkConfigCommon",
	"UplinkConfigCommon",
	&asn_OP_SEQUENCE,
	asn_DEF_UplinkConfigCommon_tags_1,
	sizeof(asn_DEF_UplinkConfigCommon_tags_1)
		/sizeof(asn_DEF_UplinkConfigCommon_tags_1[0]), /* 1 */
	asn_DEF_UplinkConfigCommon_tags_1,	/* Same as above */
	sizeof(asn_DEF_UplinkConfigCommon_tags_1)
		/sizeof(asn_DEF_UplinkConfigCommon_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_UplinkConfigCommon_1,
	3,	/* Elements count */
	&asn_SPC_UplinkConfigCommon_specs_1	/* Additional specs */
};

