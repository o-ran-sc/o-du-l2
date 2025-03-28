/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/root/svaidhya/ASN1_file/rrc_15_3.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-UPER -no-gen-BER -no-gen-JER -no-gen-OER -gen-APER -no-gen-example -findirect-choice -D /root/svaidhya/28March_ASN1_l2/l2/src/codec_utils/RRC/`
 */

#include "ResultsPerCSI-RS-Index.h"

asn_TYPE_member_t asn_MBR_ResultsPerCSI_RS_Index_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ResultsPerCSI_RS_Index, csi_RS_Index),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSI_RS_Index,
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
		"csi-RS-Index"
		},
	{ ATF_POINTER, 1, offsetof(struct ResultsPerCSI_RS_Index, csi_RS_Results),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasQuantityResults,
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
		"csi-RS-Results"
		},
};
static const int asn_MAP_ResultsPerCSI_RS_Index_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_ResultsPerCSI_RS_Index_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ResultsPerCSI_RS_Index_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* csi-RS-Index */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* csi-RS-Results */
};
asn_SEQUENCE_specifics_t asn_SPC_ResultsPerCSI_RS_Index_specs_1 = {
	sizeof(struct ResultsPerCSI_RS_Index),
	offsetof(struct ResultsPerCSI_RS_Index, _asn_ctx),
	asn_MAP_ResultsPerCSI_RS_Index_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_ResultsPerCSI_RS_Index_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_ResultsPerCSI_RS_Index = {
	"ResultsPerCSI-RS-Index",
	"ResultsPerCSI-RS-Index",
	&asn_OP_SEQUENCE,
	asn_DEF_ResultsPerCSI_RS_Index_tags_1,
	sizeof(asn_DEF_ResultsPerCSI_RS_Index_tags_1)
		/sizeof(asn_DEF_ResultsPerCSI_RS_Index_tags_1[0]), /* 1 */
	asn_DEF_ResultsPerCSI_RS_Index_tags_1,	/* Same as above */
	sizeof(asn_DEF_ResultsPerCSI_RS_Index_tags_1)
		/sizeof(asn_DEF_ResultsPerCSI_RS_Index_tags_1[0]), /* 1 */
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
	asn_MBR_ResultsPerCSI_RS_Index_1,
	2,	/* Elements count */
	&asn_SPC_ResultsPerCSI_RS_Index_specs_1	/* Additional specs */
};

