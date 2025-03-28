/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/root/svaidhya/ASN1_file/F1.asn1"
 * 	`asn1c -pdu=all -fno-include-deps -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/F1AP/`
 */

#include "Broadcast-To-Be-Cancelled-Item.h"

#include "ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_Broadcast_To_Be_Cancelled_Item_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Broadcast_To_Be_Cancelled_Item, nRCGI),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NRCGI,
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
		"nRCGI"
		},
	{ ATF_POINTER, 1, offsetof(struct Broadcast_To_Be_Cancelled_Item, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_4624P9,
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
		"iE-Extensions"
		},
};
static const int asn_MAP_Broadcast_To_Be_Cancelled_Item_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_Broadcast_To_Be_Cancelled_Item_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Broadcast_To_Be_Cancelled_Item_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* nRCGI */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_Broadcast_To_Be_Cancelled_Item_specs_1 = {
	sizeof(struct Broadcast_To_Be_Cancelled_Item),
	offsetof(struct Broadcast_To_Be_Cancelled_Item, _asn_ctx),
	asn_MAP_Broadcast_To_Be_Cancelled_Item_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_Broadcast_To_Be_Cancelled_Item_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Broadcast_To_Be_Cancelled_Item = {
	"Broadcast-To-Be-Cancelled-Item",
	"Broadcast-To-Be-Cancelled-Item",
	&asn_OP_SEQUENCE,
	asn_DEF_Broadcast_To_Be_Cancelled_Item_tags_1,
	sizeof(asn_DEF_Broadcast_To_Be_Cancelled_Item_tags_1)
		/sizeof(asn_DEF_Broadcast_To_Be_Cancelled_Item_tags_1[0]), /* 1 */
	asn_DEF_Broadcast_To_Be_Cancelled_Item_tags_1,	/* Same as above */
	sizeof(asn_DEF_Broadcast_To_Be_Cancelled_Item_tags_1)
		/sizeof(asn_DEF_Broadcast_To_Be_Cancelled_Item_tags_1[0]), /* 1 */
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
	asn_MBR_Broadcast_To_Be_Cancelled_Item_1,
	2,	/* Elements count */
	&asn_SPC_Broadcast_To_Be_Cancelled_Item_specs_1	/* Additional specs */
};

