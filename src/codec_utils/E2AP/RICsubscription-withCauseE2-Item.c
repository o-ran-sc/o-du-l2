/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "/root/svaidhya/ASN1_file/E2APV0300.asn1"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2AP/`
 */

#include "RICsubscription-withCauseE2-Item.h"

asn_TYPE_member_t asn_MBR_RICsubscription_withCauseE2_Item_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct RICsubscription_withCauseE2_Item, ricRequestID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RICrequestID,
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
		"ricRequestID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RICsubscription_withCauseE2_Item, ranFunctionID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RANfunctionID,
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
		"ranFunctionID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RICsubscription_withCauseE2_Item, cause),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_CauseE2,
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
		"cause"
		},
};
static const ber_tlv_tag_t asn_DEF_RICsubscription_withCauseE2_Item_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_RICsubscription_withCauseE2_Item_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ricRequestID */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ranFunctionID */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* cause */
};
asn_SEQUENCE_specifics_t asn_SPC_RICsubscription_withCauseE2_Item_specs_1 = {
	sizeof(struct RICsubscription_withCauseE2_Item),
	offsetof(struct RICsubscription_withCauseE2_Item, _asn_ctx),
	asn_MAP_RICsubscription_withCauseE2_Item_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_RICsubscription_withCauseE2_Item = {
	"RICsubscription-withCauseE2-Item",
	"RICsubscription-withCauseE2-Item",
	&asn_OP_SEQUENCE,
	asn_DEF_RICsubscription_withCauseE2_Item_tags_1,
	sizeof(asn_DEF_RICsubscription_withCauseE2_Item_tags_1)
		/sizeof(asn_DEF_RICsubscription_withCauseE2_Item_tags_1[0]), /* 1 */
	asn_DEF_RICsubscription_withCauseE2_Item_tags_1,	/* Same as above */
	sizeof(asn_DEF_RICsubscription_withCauseE2_Item_tags_1)
		/sizeof(asn_DEF_RICsubscription_withCauseE2_Item_tags_1[0]), /* 1 */
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
	asn_MBR_RICsubscription_withCauseE2_Item_1,
	3,	/* Elements count */
	&asn_SPC_RICsubscription_withCauseE2_Item_specs_1	/* Additional specs */
};

