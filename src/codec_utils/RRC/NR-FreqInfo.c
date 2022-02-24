/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-InterNodeDefinitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "NR-FreqInfo.h"

asn_TYPE_member_t asn_MBR_NR_FreqInfo_1[] = {
	{ ATF_POINTER, 1, offsetof(struct NR_FreqInfo, measuredFrequency),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ARFCN_ValueNR,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"measuredFrequency"
		},
};
static const int asn_MAP_NR_FreqInfo_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_NR_FreqInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_NR_FreqInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* measuredFrequency */
};
asn_SEQUENCE_specifics_t asn_SPC_NR_FreqInfo_specs_1 = {
	sizeof(struct NR_FreqInfo),
	offsetof(struct NR_FreqInfo, _asn_ctx),
	asn_MAP_NR_FreqInfo_tag2el_1,
	1,	/* Count of tags in the map */
	asn_MAP_NR_FreqInfo_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_NR_FreqInfo = {
	"NR-FreqInfo",
	"NR-FreqInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_NR_FreqInfo_tags_1,
	sizeof(asn_DEF_NR_FreqInfo_tags_1)
		/sizeof(asn_DEF_NR_FreqInfo_tags_1[0]), /* 1 */
	asn_DEF_NR_FreqInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_NR_FreqInfo_tags_1)
		/sizeof(asn_DEF_NR_FreqInfo_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_NR_FreqInfo_1,
	1,	/* Elements count */
	&asn_SPC_NR_FreqInfo_specs_1	/* Additional specs */
};

