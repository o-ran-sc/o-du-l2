/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-KPM-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_KPM_v2.03.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#include "MatchingUEidItem-PerGP.h"

asn_TYPE_member_t asn_MBR_MatchingUEidItem_PerGP_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct MatchingUEidItem_PerGP, ueID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_UEID,
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
		"ueID"
		},
};
static const ber_tlv_tag_t asn_DEF_MatchingUEidItem_PerGP_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_MatchingUEidItem_PerGP_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* ueID */
};
asn_SEQUENCE_specifics_t asn_SPC_MatchingUEidItem_PerGP_specs_1 = {
	sizeof(struct MatchingUEidItem_PerGP),
	offsetof(struct MatchingUEidItem_PerGP, _asn_ctx),
	asn_MAP_MatchingUEidItem_PerGP_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_MatchingUEidItem_PerGP = {
	"MatchingUEidItem-PerGP",
	"MatchingUEidItem-PerGP",
	&asn_OP_SEQUENCE,
	asn_DEF_MatchingUEidItem_PerGP_tags_1,
	sizeof(asn_DEF_MatchingUEidItem_PerGP_tags_1)
		/sizeof(asn_DEF_MatchingUEidItem_PerGP_tags_1[0]), /* 1 */
	asn_DEF_MatchingUEidItem_PerGP_tags_1,	/* Same as above */
	sizeof(asn_DEF_MatchingUEidItem_PerGP_tags_1)
		/sizeof(asn_DEF_MatchingUEidItem_PerGP_tags_1[0]), /* 1 */
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
	asn_MBR_MatchingUEidItem_PerGP_1,
	1,	/* Elements count */
	&asn_SPC_MatchingUEidItem_PerGP_specs_1	/* Additional specs */
};

