/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "../../../rrc_15.3_asn.asn1"
 * 	`asn1c -D ./25_02_2022_RRC/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "SIB3.h"

#include "IntraFreqNeighCellList.h"
#include "IntraFreqBlackCellList.h"
asn_TYPE_member_t asn_MBR_SIB3_1[] = {
	{ ATF_POINTER, 3, offsetof(struct SIB3, intraFreqNeighCellList),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IntraFreqNeighCellList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"intraFreqNeighCellList"
		},
	{ ATF_POINTER, 2, offsetof(struct SIB3, intraFreqBlackCellList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IntraFreqBlackCellList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"intraFreqBlackCellList"
		},
	{ ATF_POINTER, 1, offsetof(struct SIB3, lateNonCriticalExtension),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"lateNonCriticalExtension"
		},
};
static const int asn_MAP_SIB3_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_SIB3_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SIB3_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* intraFreqNeighCellList */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* intraFreqBlackCellList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* lateNonCriticalExtension */
};
asn_SEQUENCE_specifics_t asn_SPC_SIB3_specs_1 = {
	sizeof(struct SIB3),
	offsetof(struct SIB3, _asn_ctx),
	asn_MAP_SIB3_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_SIB3_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SIB3 = {
	"SIB3",
	"SIB3",
	&asn_OP_SEQUENCE,
	asn_DEF_SIB3_tags_1,
	sizeof(asn_DEF_SIB3_tags_1)
		/sizeof(asn_DEF_SIB3_tags_1[0]), /* 1 */
	asn_DEF_SIB3_tags_1,	/* Same as above */
	sizeof(asn_DEF_SIB3_tags_1)
		/sizeof(asn_DEF_SIB3_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SIB3_1,
	3,	/* Elements count */
	&asn_SPC_SIB3_specs_1	/* Additional specs */
};

