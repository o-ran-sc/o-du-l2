/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_KPM_v2.03.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#include "MatchingUeCondPerSubItem.h"

asn_TYPE_member_t asn_MBR_MatchingUeCondPerSubItem_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct MatchingUeCondPerSubItem, testCondInfo),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TestCondInfo,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"testCondInfo"
		},
	{ ATF_POINTER, 1, offsetof(struct MatchingUeCondPerSubItem, logicalOR),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LogicalOR,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"logicalOR"
		},
};
static const int asn_MAP_MatchingUeCondPerSubItem_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_MatchingUeCondPerSubItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MatchingUeCondPerSubItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* testCondInfo */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* logicalOR */
};
asn_SEQUENCE_specifics_t asn_SPC_MatchingUeCondPerSubItem_specs_1 = {
	sizeof(struct MatchingUeCondPerSubItem),
	offsetof(struct MatchingUeCondPerSubItem, _asn_ctx),
	asn_MAP_MatchingUeCondPerSubItem_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_MatchingUeCondPerSubItem_oms_1,	/* Optional members */
	0, 1,	/* Root/Additions */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MatchingUeCondPerSubItem = {
	"MatchingUeCondPerSubItem",
	"MatchingUeCondPerSubItem",
	&asn_OP_SEQUENCE,
	asn_DEF_MatchingUeCondPerSubItem_tags_1,
	sizeof(asn_DEF_MatchingUeCondPerSubItem_tags_1)
		/sizeof(asn_DEF_MatchingUeCondPerSubItem_tags_1[0]), /* 1 */
	asn_DEF_MatchingUeCondPerSubItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_MatchingUeCondPerSubItem_tags_1)
		/sizeof(asn_DEF_MatchingUeCondPerSubItem_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_MatchingUeCondPerSubItem_1,
	2,	/* Elements count */
	&asn_SPC_MatchingUeCondPerSubItem_specs_1	/* Additional specs */
};

