/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-COMMON-IEs"
 * 	found in "/root/svaidhya/ASN1_file/ASN.1_SM_v2.01.asn"
 * 	`asn1c -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/E2SM_KPM/`
 */

#include "NRFrequencyInfo.h"

static asn_TYPE_member_t asn_MBR_NRFrequencyInfo_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct NRFrequencyInfo, nrARFCN),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NR_ARFCN,
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
		"nrARFCN"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct NRFrequencyInfo, frequencyBand_List),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NRFrequencyBand_List,
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
		"frequencyBand-List"
		},
	{ ATF_POINTER, 1, offsetof(struct NRFrequencyInfo, frequencyShift7p5khz),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NRFrequencyShift7p5khz,
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
		"frequencyShift7p5khz"
		},
};
static const int asn_MAP_NRFrequencyInfo_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_NRFrequencyInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_NRFrequencyInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* nrARFCN */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* frequencyBand-List */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* frequencyShift7p5khz */
};
static asn_SEQUENCE_specifics_t asn_SPC_NRFrequencyInfo_specs_1 = {
	sizeof(struct NRFrequencyInfo),
	offsetof(struct NRFrequencyInfo, _asn_ctx),
	asn_MAP_NRFrequencyInfo_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_NRFrequencyInfo_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_NRFrequencyInfo = {
	"NRFrequencyInfo",
	"NRFrequencyInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_NRFrequencyInfo_tags_1,
	sizeof(asn_DEF_NRFrequencyInfo_tags_1)
		/sizeof(asn_DEF_NRFrequencyInfo_tags_1[0]), /* 1 */
	asn_DEF_NRFrequencyInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_NRFrequencyInfo_tags_1)
		/sizeof(asn_DEF_NRFrequencyInfo_tags_1[0]), /* 1 */
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
	asn_MBR_NRFrequencyInfo_1,
	3,	/* Elements count */
	&asn_SPC_NRFrequencyInfo_specs_1	/* Additional specs */
};

