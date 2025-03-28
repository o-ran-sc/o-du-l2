/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#include "DL-DCCH-Message.h"

static asn_TYPE_member_t asn_MBR_DL_DCCH_Message_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DL_DCCH_Message, message),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_DL_DCCH_MessageType,
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
		"message"
		},
};
static const ber_tlv_tag_t asn_DEF_DL_DCCH_Message_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DL_DCCH_Message_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* message */
};
static asn_SEQUENCE_specifics_t asn_SPC_DL_DCCH_Message_specs_1 = {
	sizeof(struct DL_DCCH_Message),
	offsetof(struct DL_DCCH_Message, _asn_ctx),
	asn_MAP_DL_DCCH_Message_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_DL_DCCH_Message = {
	"DL-DCCH-Message",
	"DL-DCCH-Message",
	&asn_OP_SEQUENCE,
	asn_DEF_DL_DCCH_Message_tags_1,
	sizeof(asn_DEF_DL_DCCH_Message_tags_1)
		/sizeof(asn_DEF_DL_DCCH_Message_tags_1[0]), /* 1 */
	asn_DEF_DL_DCCH_Message_tags_1,	/* Same as above */
	sizeof(asn_DEF_DL_DCCH_Message_tags_1)
		/sizeof(asn_DEF_DL_DCCH_Message_tags_1[0]), /* 1 */
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
	asn_MBR_DL_DCCH_Message_1,
	1,	/* Elements count */
	&asn_SPC_DL_DCCH_Message_specs_1	/* Additional specs */
};

