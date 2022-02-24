/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./24_RRC_Handover_MTC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "NR-MultiBandInfo.h"

#include "NR-NS-PmaxList.h"
asn_TYPE_member_t asn_MBR_NR_MultiBandInfo_1[] = {
	{ ATF_POINTER, 2, offsetof(struct NR_MultiBandInfo, freqBandIndicatorNR),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FreqBandIndicatorNR,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"freqBandIndicatorNR"
		},
	{ ATF_POINTER, 1, offsetof(struct NR_MultiBandInfo, nr_NS_PmaxList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NR_NS_PmaxList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nr-NS-PmaxList"
		},
};
static const int asn_MAP_NR_MultiBandInfo_oms_1[] = { 0, 1 };
static const ber_tlv_tag_t asn_DEF_NR_MultiBandInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_NR_MultiBandInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* freqBandIndicatorNR */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* nr-NS-PmaxList */
};
asn_SEQUENCE_specifics_t asn_SPC_NR_MultiBandInfo_specs_1 = {
	sizeof(struct NR_MultiBandInfo),
	offsetof(struct NR_MultiBandInfo, _asn_ctx),
	asn_MAP_NR_MultiBandInfo_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_NR_MultiBandInfo_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_NR_MultiBandInfo = {
	"NR-MultiBandInfo",
	"NR-MultiBandInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_NR_MultiBandInfo_tags_1,
	sizeof(asn_DEF_NR_MultiBandInfo_tags_1)
		/sizeof(asn_DEF_NR_MultiBandInfo_tags_1[0]), /* 1 */
	asn_DEF_NR_MultiBandInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_NR_MultiBandInfo_tags_1)
		/sizeof(asn_DEF_NR_MultiBandInfo_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_NR_MultiBandInfo_1,
	2,	/* Elements count */
	&asn_SPC_NR_MultiBandInfo_specs_1	/* Additional specs */
};

